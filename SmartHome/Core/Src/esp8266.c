#include "esp8266.h"

extern UART_HandleTypeDef huart3;
uint8_t g_UART_Buf[512];
uint16_t g_Index;

#define MQTT_SUCCESS 0
#define MQTT_FAILURE 1

void ESP8266_Init(void){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
	HAL_Delay(20);

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_Delay(20);

	ESP8266_Response("csum 0xde\r\n", 500);
	ESP8266_SetBaud(115200);
	ESP8266_Response("ready\r\n", 500);

	//close echo
	ESP8266_Send_Cmd("ATE0");

	ESP8266_Refresh_Buffer();
}

uint8_t ESP8266_Response(const char *msg, uint32_t timeout){
	//printf("baud: %d\n", (int)huart3.Init.BaudRate);
	uint32_t Cur_Tick = HAL_GetTick();
	uint8_t data, i = 0;
	while(1){
		if(HAL_GetTick() - Cur_Tick >= timeout){
			return 0;  //failed
		}

		if(HAL_UART_Receive(&huart3, &data, 1, 10) == HAL_TIMEOUT) continue;
		g_UART_Buf[g_Index++] = data;
		if(data == msg[i]){
			i++;
			if(i == strlen(msg))
				return 1;  //succeed
		}
		else{
			i = 0;
		}
	}

}

void ESP8266_SetBaud(uint32_t baud){
	  huart3.Instance = USART3;
	  huart3.Init.BaudRate = baud;
	  huart3.Init.WordLength = UART_WORDLENGTH_8B;
	  huart3.Init.StopBits = UART_STOPBITS_1;
	  huart3.Init.Parity = UART_PARITY_NONE;
	  huart3.Init.Mode = UART_MODE_TX_RX;
	  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	  HAL_UART_Init(&huart3);
}

void ESP8266_Refresh_Buffer(void)
{
	printf("%s\r\n", g_UART_Buf);

	g_Index = 0;
	memset(g_UART_Buf, 0, sizeof(g_UART_Buf));
}

void ESP8266_Send_Cmd(const char *cmd)
{
	HAL_UART_Transmit(&huart3, (uint8_t *)cmd, strlen(cmd), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
}

uint8_t ESP8266_Connect_AP(const char *ssid, const char *passwd){
	char cmd[128] = {0};
	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, passwd);
	ESP8266_Send_Cmd(cmd);
	if (ESP8266_Response("OK\r\n", 60000))
	{
		ESP8266_Refresh_Buffer();
		return 1;
	}
	else
	{
		ESP8266_Refresh_Buffer();
		printf("Failed to connect wifi\r\n");
		return 0;
	}
}


uint8_t ESP8266_Connect_MQTT(const char *ip, int port, const char *user, const char *passwd)
{
	char cmd[128] = {0};

	sprintf(cmd, "AT+MQTTUSERCFG=0,1,\"0001\",\"%s\",\"%s\",0,0,\" \"", user, passwd);

	ESP8266_Send_Cmd(cmd);

	if (ESP8266_Response("OK\r\n", 5000))
	{
		ESP8266_Refresh_Buffer();
	}
	else
	{
		ESP8266_Refresh_Buffer();
		printf("CONFIG MQTT FAILURE\r\n");
	}

	memset(cmd, 0, sizeof(cmd));

	sprintf(cmd, "AT+MQTTCONN=0,\"%s\",%d,1", ip, port);

	ESP8266_Send_Cmd(cmd);

	if (ESP8266_Response("OK", 10000))
	{
		ESP8266_Refresh_Buffer();
		printf("CONNECT MQTT SUCCEED\r\n");
		return 1;
	}
	else
	{
		ESP8266_Refresh_Buffer();
		printf("CONNECT MQTT FAILURE\r\n");
		return 0;
	}
}

void ESP8266_Publish(const char *topic, const char *msg)
{
	char cmd[256] = {0};

	sprintf(cmd, "AT+MQTTPUB=0,\"%s\",\"%s\",0,0", topic, msg);

	//printf("%s\r\n", cmd);
	ESP8266_Send_Cmd(cmd);

//	if (ESP8266_Response("OK\r\n", 10000))
//	{
//		ESP8266_Refresh_Buffer();
//		printf("PUBLISH SUCCEED\r\n");
//		return 1;
//	}
//	else
//	{
//		ESP8266_Refresh_Buffer();
//		printf("PUBLISH FAILURE\r\n");
//		return 0;
//	}
}

uint8_t ESP8266_Subscribe(const char *topic)
{
	char cmd[128] = {0};

	sprintf(cmd, "AT+MQTTSUB=0,\"%s\",0", topic);

	ESP8266_Send_Cmd(cmd);

	if (ESP8266_Response("OK\r\n", 10000))
	{
		ESP8266_Refresh_Buffer();
		//printf("SUBSCRIBE SUCCEED\r\n");
		return 1;
	}
	else
	{
		ESP8266_Refresh_Buffer();
		printf("SUBSCRIBE FAILURE\r\n");
		return 0;
	}
}

