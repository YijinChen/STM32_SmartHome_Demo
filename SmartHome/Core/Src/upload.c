#include "upload.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "main.h"
#include "cJSON.h"
#include "esp8266.h"

//test on server: mosquitto_sub -h "18.185.92.160" -p 1883 -u test -P test -t "0001upload"

extern SemaphoreHandle_t semHandle;
extern EnvInfo gInfo;

void UploadHandle(void)
{
	xSemaphoreTake(semHandle, portMAX_DELAY);
	//printf("Semaphore taken in Task upload\n");
	uint16_t light = gInfo.light;
	uint8_t temp = gInfo.temp;
	uint8_t humi = gInfo.humi;
	xSemaphoreGive(semHandle);
	//printf("Semaphore given in Task upload\n");

	//create json object
	cJSON *sendObject = cJSON_CreateObject();

	//add key/value
	cJSON_AddNumberToObject(sendObject, "light", light);
	cJSON_AddNumberToObject(sendObject, "temp", temp);
	cJSON_AddNumberToObject(sendObject, "humi", humi);

	//convert Json object to string
	char *sendStr = cJSON_Print(sendObject);

	char *p = sendStr;
	char output[256] = {0};
	int i = 0;


	//add \\ before " and ,
	while (*p != '\0')
	{
		if (*p == '"' || *p == ',')
		{
			output[i++] = '\\';
			output[i++] = *p;
		}
		else
			output[i++] = *p;

		p++;
	}
	ESP8266_Publish("0001upload", output);
	cJSON_Delete(sendObject);
	vPortFree(sendStr);
}

