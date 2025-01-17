#include "dht11.h"
//#include "tim.h"
#include "FreeRtos.h"
#include "task.h"
#include <stdio.h>

void Delay_US(uint32_t us)
{
	uint32_t old_time = SysTick->VAL;
	uint32_t new_time;
	uint32_t ticks = us * 99;
	uint32_t cnt = 0;
	uint32_t load = SysTick->LOAD;

	while (1)
	{
		new_time = SysTick->VAL;

		if (new_time != old_time)
		{
			if (new_time < old_time)
				cnt += (old_time - new_time);
			else
				cnt += (load - new_time + old_time);

			if (cnt >= ticks)
				break;

			old_time = new_time;
		}
	}
}


void DHT11_Init(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_Init_Struct;

	GPIO_Init_Struct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_Init_Struct.Pin = GPIO_PIN_6;
	GPIO_Init_Struct.Pull = GPIO_PULLUP;
	GPIO_Init_Struct.Speed = GPIO_SPEED_FREQ_HIGH;

	HAL_GPIO_Init(GPIOC, &GPIO_Init_Struct);
}

void DHT11_Start(void)
{
	//printf("start\r\n");
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

	HAL_Delay(20);
	//printf("start2\r\n");
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);

	Delay_US(30);
	//printf("start3\r\n");
}

uint8_t DHT11_Response(void)
{
	uint16_t time = 0;

	while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) && time < 100)
	{
		Delay_US(1);
		time++;
	}

	if (time >= 100)
		return 1;

	time = 0;

	while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) && time < 100)
	{
		Delay_US(1);
		time++;
	}

	if (time >= 100)
		return 1;

	return 0;
}

uint8_t DHT11_Read_Bit(void)
{
	uint16_t time = 0;

	while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) && time < 100)
	{
		Delay_US(1);
		time++;
	}

	if (time >= 100)
		return 2;

	while (!HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) && time < 100)
	{
		Delay_US(1);
		time++;
	}

	if (time >= 100)
		return 2;

	Delay_US(30);

	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0)
		return 0;
	else
		return 1;
}

uint8_t DHT11_Read_Byte(void)
{
	uint8_t data = 0;
	uint8_t i = 0;

	for (i = 0; i < 8; i++)
	{
		data <<= 1;
		data = data | DHT11_Read_Bit();
	}

	return data;
}

void DHT11_Read_Data(uint8_t *pData)
{
	DHT11_Start();
	//printf("read data\r\n");

	if (DHT11_Response()){
		//printf("response\r\n");
		return;
	}

	//printf("no response\r\n");


	uint8_t i;
	for (i = 0; i < 5; i++)
	{
		pData[i] = DHT11_Read_Byte();
	}

	if (pData[4] != pData[0] + pData[1] + pData[2] + pData[3])
	{
		for (i = 0; i < 5; i++)
		{
			pData[i] = 0;
		}
	}
}
