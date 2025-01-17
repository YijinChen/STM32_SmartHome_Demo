/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "esp8266.h"
#include "oled.h"
#include <stdio.h>
#include <string.h>
#include "dht11.h"
#include "semphr.h"
#include "task.h"
#include "bh1750.h"
#include "show.h"
#include "timers.h"
#include "upload.h"
#include "control.h"
#include "link.h"
#include "queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern UART_HandleTypeDef huart3;
//extern TaskHandle_t xTimerTaskHandle;

EnvInfo gInfo = {.cur_time = 1736200229,};

xQueueHandle uartHandle;
SemaphoreHandle_t semHandle;
TimerHandle_t dataTimerHandle;
TimerHandle_t buzzerTimerHandle;
uint8_t gMenu = 1;  //4 pages can be shown on OLED, use gMenu to select the page
uint8_t gBuzzerFlag = 0;  //use for record buzzer state, as to check if the button is used for closing the buzzer
uint8_t gUartBuf[256] = {0};  //use for receiving message from uart

osThreadId_t recvTaskHandle;
const osThreadAttr_t recvTask_attributes = {
  .name = "recvTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t tempTaskHandle;
const osThreadAttr_t tempTask_attributes = {
  .name = "tempTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t lightTaskHandle;
const osThreadAttr_t lightTask_attributes = {
  .name = "lightTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

osThreadId_t OLEDTaskHandle;
const osThreadAttr_t OLEDTask_attributes = {
  .name = "OLEDTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal1,
};

osThreadId_t UploadTaskHandle;
const osThreadAttr_t UploadTask_attributes = {
  .name = "UploadTask",
  .stack_size = 128 * 16,
  .priority = (osPriority_t) osPriorityNormal1,
};

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 8,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void RecvTask(void *arg);
void Task_Recv_Msg(uint8_t *msg);
void GetTempTask(void *arg);
void GetLightTask(void *arg);
void OLED_Show_Task(void *arg);
uint8_t Key_Scan(void);
void Data_Increase_Callback(TimerHandle_t xTimer);
void UploadTask(void *arg);
void Buzzer_Callback(TimerHandle_t xTimer);
//void PrintAllTaskInfo(void);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
	int timeid = 1;
	dataTimerHandle = xTimerCreate("data", 1000, pdTRUE, &timeid, Data_Increase_Callback);  //create timer
	int timeid2 = 2;
	buzzerTimerHandle = xTimerCreate("buzzer", 1, pdTRUE, &timeid2, Buzzer_Callback);
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	uartHandle = xQueueCreate(5, sizeof(gUartBuf));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	semHandle = xSemaphoreCreateBinary();
	xSemaphoreGive(semHandle);  //initialize semHandle

	OLED_Init();
	OLED_Clear();
	OLED_ShowString(0,0,(uint8_t *)"[1]START WORKING", 16);

	ESP8266_Init();
	if(ESP8266_Connect_AP(SSID, PASSWORD)){
		OLED_ShowString(0,2,(uint8_t *)"[2]WIFI OK", 16);
	}
	else{
		OLED_ShowString(0,2,(uint8_t *)"[2]WIFI ERR", 16);
	}

	if(ESP8266_Connect_MQTT(MQTTIP, MQTTPORT, "test", "test")){
		OLED_ShowString(0,4,(uint8_t *)"[3]MQTT OK", 16);
	}
	else{
		OLED_ShowString(0,4,(uint8_t *)"[3]MQTT ERR", 16);
	}
	if(ESP8266_Subscribe(DEVICEID)){
		OLED_ShowString(0,6,(uint8_t *)"[4]SUBSCRIBE OK", 16);
	}
	else{
		OLED_ShowString(0,6,(uint8_t *)"[4]SUBSCRIBE ERR", 16);
	}
	osDelay(1000);

	//start timer
	if (xTimerStart(dataTimerHandle, portMAX_DELAY) != pdPASS) {
	    printf("Failed to start the timer\r\n");
	}

	HAL_UARTEx_ReceiveToIdle_IT(&huart3, gUartBuf, sizeof(gUartBuf)); //Open the Idle interrupt of uart3



	//create task
	recvTaskHandle = osThreadNew(RecvTask, NULL, &recvTask_attributes);

	osDelay(1000);

	tempTaskHandle = osThreadNew(GetTempTask, NULL, &tempTask_attributes);
	lightTaskHandle = osThreadNew(GetLightTask, NULL, &lightTask_attributes);
	OLEDTaskHandle = osThreadNew(OLED_Show_Task, NULL, &OLEDTask_attributes);
	UploadTaskHandle = osThreadNew(UploadTask, NULL, &UploadTask_attributes);

  /* Infinite loop */
  for(;;)
  {
	  //if buzzer is work, press button will close it, else the menu will be changed by button
	if (Key_Scan() == 1)
	{
		if (gBuzzerFlag == 1)
		{
			Control_Buzzer_Stop();
		}
		else
		{
			gMenu++;
			if (gMenu == 5)
				gMenu = 1;
		}
	}

	//find the alarm which satisfy the requirement and turn on it
	if (gBuzzerFlag == 0 && Link_Alarm_Isset())
	{
		Control_Buzzer_Start();
	}
	osDelay(50);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void RecvTask(void *arg){


	uint8_t msg[256] = {0};
	while(1){
		//Task_Recv_Msg(msg);
		xQueueReceive(uartHandle, msg, portMAX_DELAY); //read data from uart
		if(strlen((char *)msg) > 10){ //when strlen(msg) < 10, this message is from esp8266
			//printf("Get message from server: %s\r\n", msg);
			Parse_Message((char *)msg);
		}
		memset(msg, 0, sizeof(msg));
	}
}

//void Task_Recv_Msg(uint8_t *msg){
//	uint8_t data, flag = 0, i = 0;
//	uint32_t old_tick = 0;
//
//	while (1)
//	{
//		if (HAL_GetTick() - old_tick > 100 && flag)
//		{
//			break;
//		}
//
//		if (HAL_UART_Receive(&huart3, &data, 1, 10) == HAL_TIMEOUT)
//		{
//			continue;
//		}
//		else
//		{
//			msg[i++] = data;
//			old_tick = HAL_GetTick();
//			flag = 1;
//		}
//	}
//}

void GetTempTask(void *arg){
//    UBaseType_t priority = uxTaskPriorityGet(NULL);
//    printf("temp priority: %lu\n", priority);

	uint8_t data[5] = {0};
	//printf("%d %d\r\n", data[0], data[2]);
	while(1){
		vTaskSuspendAll();  //close allocator
		DHT11_Read_Data(data);
		xTaskResumeAll();

		xSemaphoreTake(semHandle, portMAX_DELAY);
		//printf("Semaphore taken in Task temp\n");
		gInfo.humi = data[0];
		gInfo.temp = data[2];
		xSemaphoreGive(semHandle);
		//printf("Semaphore given in Task temp\n");
		printf("humi: %d.%d %%  temp: %d.%d C\r\n", data[0], data[1], data[2], data[3]);
		osDelay(500);
	}
}

void GetLightTask(void *arg){
//    UBaseType_t priority = uxTaskPriorityGet(NULL);
//    printf("light priority: %lu\n", priority);

	//printf("light function\r\n");
	uint8_t data[2] = {0};
	uint16_t light = 0;
	while(1){
		BH1750_Send_CMD(0x20);
		osDelay(150);
		BH1750_Read_Data(data);
		light = BH1750_Data_To_Lx(data);
		xSemaphoreTake(semHandle, portMAX_DELAY);
		//printf("Semaphore taken in Task light\n");
		gInfo.light = light;
		xSemaphoreGive(semHandle);
		//printf("Semaphore given in Task light\n");
		osDelay(350);
		printf("light: %d\r\n", light);
	}
}

void OLED_Show_Task(void *arg){
	OLED_Show_Menu();
}

uint8_t Key_Scan(void){

//	if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == 0){  //read key1(GPIO6)
//		osDelay(20);
//		if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == 0){
//			while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == 0);
//			return 1;
//		}
//	}

    static uint8_t prevState = GPIO_PIN_SET;
    uint8_t currState = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);

    if (prevState == GPIO_PIN_SET && currState == GPIO_PIN_RESET) { // Button pressed
        osDelay(30); // Debounce delay
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6) == GPIO_PIN_RESET) { // Confirm press
            prevState = GPIO_PIN_RESET;
            return 1; // Button press detected
        }
    } else if (currState == GPIO_PIN_SET) { // Button released
        prevState = GPIO_PIN_SET;
    }

	return 0;
}

void Data_Increase_Callback(TimerHandle_t xTimer){
	xSemaphoreTake(semHandle, portMAX_DELAY);
	gInfo.cur_time++;
	xSemaphoreGive(semHandle);
}

void Buzzer_Callback(TimerHandle_t xTimer)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
}

void UploadTask(void *arg){
	while(1){
		UploadHandle();
		osDelay(500);
	}
}

/* USER CODE END Application */

