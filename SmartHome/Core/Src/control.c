#include "control.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "cJSON.h"
#include "main.h"
#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"
#include "link.h"

extern uint8_t gMenu;
extern TimerHandle_t buzzerTimerHandle;
extern EnvInfo gInfo;
extern SemaphoreHandle_t semHandle; 
extern uint8_t gBuzzerFlag;


//test on server: mosquitto_pub -h "18.185.92.160" -p 1883 -u test -P test -t "0001" -m "{\"cmd\":\"menu\", \"tab\":\"2\"}"
void Control_Menu(cJSON *obj){
	//printf("control menu\r\n");
	cJSON *tabObject = cJSON_GetObjectItem(obj, "tab");
	if (tabObject)
	{
		if (!strcmp(tabObject->valuestring, "1"))
			gMenu = 1;
		else if (!strcmp(tabObject->valuestring, "2"))
			gMenu = 2;
		else if (!strcmp(tabObject->valuestring, "3"))
			gMenu = 3;
		else if (!strcmp(tabObject->valuestring, "4"))
			gMenu = 4;
	}
}

//test on server:
//mosquitto_pub -h "18.185.92.160" -p 1883 -u test -P test -t "0001" -m "{\"cmd\":\"light\", \"which\":2, \"state\":1}"
void Control_Light(cJSON *obj){
	cJSON *idObject = cJSON_GetObjectItem(obj, "which");
	if (!idObject)
		return;

	cJSON *stateObject = cJSON_GetObjectItem(obj, "state");
	if (!stateObject)
		return;

	int id = idObject->valueint;
	int state = stateObject->valueint;

	if (id < 1 || id > 4 || state < 0 || state > 1)
		return;
	switch(id){
		case 1:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, (state == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, (state == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, (state == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, (state == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
			break;
	}
}

void Control_Buzzer_Start(){
	//printf("start buzzer\r\n");
	xTimerStart(buzzerTimerHandle, portMAX_DELAY);
	gBuzzerFlag = 1;
}

void Control_Buzzer_Stop(){
	//printf("stop buzzer\r\n");
	xTimerStop(buzzerTimerHandle, portMAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	gBuzzerFlag = 0;
}

//test on server: mosquitto_pub -h "18.185.92.160" -p 1883 -u test -P test -t "0001" -m "{\"cmd\":\"buzzer\", \"state\":1}"
void Control_Buzzer(cJSON *obj){
	cJSON *stateObj = cJSON_GetObjectItem(obj, "state");
	if (!stateObj)
		return;

	int state = stateObj->valueint;

	if (state == 1)
	{
		Control_Buzzer_Start();
	}
	else if (state == 0)
	{
		Control_Buzzer_Stop();
	}
}

//test on server: mosquitto_pub -h "18.185.92.160" -p 1883 -u test -P test -t "0001" -m "{\"cmd\":\"date\", \"time\":1736200264}"
void Control_Date(cJSON *obj){
	cJSON *timeObj = cJSON_GetObjectItem(obj, "time");
	if (!timeObj)
		return;

	uint32_t time = timeObj->valueint;

	xSemaphoreTake(semHandle, portMAX_DELAY);

	gInfo.cur_time = time;

	xSemaphoreGive(semHandle);
}

//test on server:
//mosquitto_pub -h "18.185.92.160" -p 1883 -u test -P test -t "0001" -m "{\"cmd\":\"alarm\", \"hour\":10, \"min\":28}"
void Control_Alarm(cJSON *obj){
	//printf("control alarm\r\n");
	cJSON *hourObj = cJSON_GetObjectItem(obj, "hour");
	cJSON *minObj = cJSON_GetObjectItem(obj, "min");
	if (!hourObj || !minObj)
		return;

	uint8_t hour = hourObj->valueint;
	uint8_t min = minObj->valueint;

	Add_Alarm_Link(hour, min);
}

//test on server:
//mosquitto_pub -h "18.185.92.160" -p 1883 -u test -P test -t "0001" -m "{\"cmd\":\"note\", \"hour\":10, \"min\":28, \"event\":\"learn german\"}"
void Control_Note(cJSON *obj)
{
	//printf("control note\r\n");
	cJSON *hourObj = cJSON_GetObjectItem(obj, "hour");
	cJSON *minObj = cJSON_GetObjectItem(obj, "min");
	cJSON *eventObj = cJSON_GetObjectItem(obj, "event");
	if (!hourObj || !minObj || !eventObj)
		return;

	uint8_t hour = hourObj->valueint;
	uint8_t min = minObj->valueint;
	char event[16] = {0};
	//event = pvPortMalloc(sizeof(uint8_t)*10);
	strcpy(event, eventObj->valuestring);
	printf("event: %s\r\n", event);

	Add_Note_Link(hour, min, event);
}

void Parse_Message(char *msg){
	//MQTTSUBRECV:0,"0001",10,helloworld
	char *p = msg, *p1 ,*p2;  //p1 point to 10, p2 point to helloworld
	int comma = 0, len = 0;
	char str[8] = {0}; 
	char content[256] = {0};
	
	while (*p != '\0')
	{
		if (*p == ',')
			comma++;
		
		if (comma == 2 && *p == ',')
			p1 = p + 1;
		
		if (comma == 3 && *p == ',')
			p2 = p + 1;
		
		p++;
	}
	
	memcpy(str, p1, p2 - p1);  //get length 10
	len = atoi(str);
	
	memcpy(content, p2, len);   //get content hello world

	cJSON *jsonObject = cJSON_Parse(content);
	if (jsonObject == NULL)
	{
		cJSON_Delete(jsonObject);
		return;
	}
	
	cJSON *cmdObject = cJSON_GetObjectItem(jsonObject, "cmd");
	if (!strcmp(cmdObject->valuestring, "menu"))
	{
		Control_Menu(jsonObject);
	}
	else if (!strcmp(cmdObject->valuestring, "light"))
	{
		Control_Light(jsonObject);
	}
	else if (!strcmp(cmdObject->valuestring, "buzzer"))
	{
		Control_Buzzer(jsonObject);
	}
	else if (!strcmp(cmdObject->valuestring, "date"))
	{
		Control_Date(jsonObject);
	}
	else if (!strcmp(cmdObject->valuestring, "alarm"))
	{
		Control_Alarm(jsonObject);
	}
	else if (!strcmp(cmdObject->valuestring, "note"))
	{
		printf("parse node\r\n");
		Control_Note(jsonObject);
	}
	
	cJSON_Delete(jsonObject);
	
}

