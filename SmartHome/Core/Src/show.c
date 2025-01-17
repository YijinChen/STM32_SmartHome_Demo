#include "FreeRTOS.h"
#include "show.h"
#include "cmsis_os2.h"
#include "oled.h"
#include "semphr.h"
#include "main.h"
#include <stdio.h>
#include "link.h"
#include <string.h>

extern SemaphoreHandle_t semHandle;
extern EnvInfo gInfo;
extern uint8_t gMenu;

EnvInfo preInfo = {0};
uint8_t curMenu = 0;  //record the number of current menu
TimeInfo preTime = {0};

uint8_t Get_Number_Len(uint16_t number){
	if(number == 0){
		return 1;
	}

	int len = 0;

	while(number > 0){
		len++;
		number /= 10;
	}

	return len;
}

void Show_OLED_Light(uint8_t level){   //adjust contrast according to the illumination
	OLED_WR_CMD(0x81); //send command to oled to adjust contrast
	switch(level){  //set contrast 0x01-0xFF
	case 1:
		OLED_WR_CMD(0x0f);
		break;
	case 2:
		OLED_WR_CMD(0x8f);
		break;
	default:
		OLED_WR_CMD(0xff);
		break;
	}

}


void Show_Environment(void){
	//printf("show environment\r\n");

	uint8_t temp, humi, i;
	uint16_t light;

	xSemaphoreTake(semHandle, portMAX_DELAY);
	temp = gInfo.temp;
	humi = gInfo.humi;
	light = gInfo.light;
	xSemaphoreGive(semHandle);

	if(light < 20){
		Show_OLED_Light(1);
	}
	else if(light < 40){
		Show_OLED_Light(2);
	}
	else{
		Show_OLED_Light(3);
	}

	if(gMenu != curMenu){
		for(i = 0; i < 16; i++){
			OLED_ShowChar(i * 8, 4, '-', 16);   //show the line at the middle of the screen
		}
		OLED_Clearrow(6);
		OLED_Clearrow(7);

		OLED_ShowChinese(40, 6, 10);  //show 'C' (the unit of temp)
		OLED_ShowChar(90, 6, '%', 16);  //show '%' (the unit of humi)


	}

	uint8_t len = Get_Number_Len(light);

	if(light != preInfo.light || gMenu != curMenu){  //when the value of light changes, clear the screen for the new value
		for(i = 0; i < 4; i++){
			OLED_Clearrow(i);
		}
	}

	if(len % 2){  //if the number of digits is odd
		OLED_ShowNum((8 - len) / 2 * 16 + 8, 0, light, len, 32);  //let the value of light be at the middle of the screen
	}
	else{ //if the number of digits is even
		OLED_ShowNum((8 - len) / 2 * 16, 0, light, len, 32);  //let the value of light be at the middle of the screen
	}

	if(temp != preInfo.temp || gMenu != curMenu){
		len = Get_Number_Len(temp);
		if(len == 1){  //if the number of digits of temp is odd
			OLED_ShowString(20, 6, (uint8_t *)" ", 16);
			OLED_ShowNum(28, 6, temp, Get_Number_Len(temp), 16);
		}
		else{
			OLED_ShowNum(20, 6, temp, Get_Number_Len(temp), 16);
		}
	}

	if(humi != preInfo.humi || gMenu != curMenu){
		len = Get_Number_Len(humi);
		OLED_ShowNum(70, 6, humi, len, 16);
	}

	preInfo.humi = humi;
	preInfo.temp = temp;
	preInfo.light = light;
}

void Show_Time(void){
	if(curMenu != gMenu){
		OLED_Clear();
		OLED_ShowString(30, 0, (uint8_t *)"DE(GMT+1)", 16);
	}

	uint8_t x_pos = 16;
	time_t curTime = gInfo.cur_time + 3600 * 1;
	struct tm *t = localtime(&curTime);

	uint16_t year = t->tm_year + 1900;
	uint16_t month = t->tm_mon + 1;
	uint16_t day = t->tm_mday;
	uint16_t hour = t->tm_hour;
	uint16_t min = t->tm_min;
	uint16_t sec = t->tm_sec;
	uint16_t weekday = t->tm_wday;

	if(year != preTime.year || gMenu != curMenu){
		OLED_ShowNum(x_pos, 2, year, 4, 16);
	}
	x_pos += 32;

	if(gMenu != curMenu){
		OLED_ShowChar(x_pos, 2, '-', 16);
	}
	x_pos += 8;

	if(month != preTime.month || gMenu != curMenu){
		OLED_ShowNum(x_pos, 2, month, Get_Number_Len(month), 16);
	}
	if(Get_Number_Len(month) == 1){
		x_pos += 8;
	}
	else{
		x_pos += 16;
	}

	if(gMenu != curMenu){
		OLED_ShowChar(x_pos, 2, '-', 16);
	}
	x_pos += 8;

	if(day != preTime.day || gMenu != curMenu){
		OLED_ShowNum(x_pos, 2, day, Get_Number_Len(day), 16);
	}
	if(Get_Number_Len(day) == 1){
		x_pos += 8;
	}
	else{
		x_pos += 16;
	}

	if(weekday != preTime.weekday || gMenu != curMenu){
		const char *weekdays[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
		OLED_ShowString(88, 2, (uint8_t *)weekdays[weekday], 16);
	}

	x_pos = 0;
	if(hour != preTime.hour || gMenu != curMenu){
		if(hour < 10){
			OLED_ShowNum(x_pos, 4, 0, 1, 32);
			x_pos += 16;
			OLED_ShowNum(x_pos, 4, hour, 1, 32);
			x_pos += 16;
		}
		else{
			OLED_ShowNum(x_pos, 4, hour, 2, 32);
			x_pos += 32;
		}
	}
	else{
		x_pos += 32;
	}

	if(gMenu != curMenu){
		OLED_ShowChar(x_pos, 4, ':', 32);
	}

	x_pos += 16;

	if(min != preTime.min || gMenu != curMenu){
		if(min < 10){
			OLED_ShowNum(x_pos, 4, 0, 1, 32);
			x_pos += 16;
			OLED_ShowNum(x_pos, 4, min, 1, 32);
			x_pos += 16;
		}
		else{
			OLED_ShowNum(x_pos, 4, min, 2, 32);
			x_pos += 32;
		}
	}
	else{
		x_pos += 32;
	}

	if(gMenu != curMenu){
		OLED_ShowChar(x_pos, 4, ':', 32);
	}

	x_pos += 16;

	if(sec != preTime.sec || gMenu != curMenu){
		if(sec < 10){
			OLED_ShowNum(x_pos, 4, 0, 1, 32);
			x_pos += 16;
			OLED_ShowNum(x_pos, 4, sec, 1, 32);
			x_pos += 16;
		}
		else{
			OLED_ShowNum(x_pos, 4, sec, 2, 32);
			x_pos += 32;
		}
	}
	else{
		x_pos += 32;
	}

	preTime.year = year;
	preTime.month = month;
	preTime.day = day;
	preTime.hour = hour;
	preTime.min = min;
	preTime.sec = sec;
	preTime.weekday = weekday;

}

//mosquitto_pub -h "18.185.92.160" -p 1883 -u test -P test -t "0001" -m "{\"cmd\":\"menu\", \"tab\":\"3\"}"
void Show_Alarm(void)
{
	uint8_t hour, min, x_pos = 16;

	Link_Find_Alarm(&hour, &min);

	if (gMenu != curMenu)
	{
		OLED_Clear();

		OLED_ShowString(46, 1, (uint8_t *)"ALARM", 16);
	}

	if (gMenu != curMenu || hour != preTime.hour)
	{
		if (hour < 10)
		{
			OLED_ShowNum(x_pos, 3, 0, 1, 40);
			x_pos += 20;
			OLED_ShowNum(x_pos, 3, hour, 1, 40);
			x_pos += 20;
		}
		else
		{
			OLED_ShowNum(x_pos, 3, hour, 2, 40);
			x_pos += 40;
		}
	}
	else
	{
		x_pos += 40;
	}

	if (gMenu != curMenu)
		OLED_ShowChar(x_pos, 3, ':', 40);

	x_pos += 20;

	if (gMenu != curMenu || min != preTime.min)
	{
		if (min < 10)
		{
			OLED_ShowNum(x_pos, 3, 0, 1, 40);
			x_pos += 20;
			OLED_ShowNum(x_pos, 3, min, 1, 40);
			x_pos += 20;
		}
		else
		{
			OLED_ShowNum(x_pos, 3, min, 2, 40);
			x_pos += 40;
		}
	}
	else
	{
		x_pos += 40;
	}

	preTime.hour = hour;
	preTime.min = min;
}

void Show_Note(void)
{
	uint8_t hour = 0, min = 0, x_pos = 24;
	char event[15] = {0};

	if (gMenu != curMenu)
	{
		OLED_Clear();

		OLED_ShowString(36, 0, (uint8_t *)"Reminder", 16);
	}

	//Show the time of the event
	Link_Find_Note(&hour, &min, event);

	if (gMenu != curMenu || hour != preTime.hour)
	{
		if (hour < 10)
		{
			OLED_ShowNum(x_pos, 2, 0, 1, 32);
			x_pos += 16;
			OLED_ShowNum(x_pos, 2, hour, 1, 32);
			x_pos += 16;
		}
		else
		{
			OLED_ShowNum(x_pos, 2, hour, 2, 32);
			x_pos += 32;
		}
	}
	else
	{
		x_pos += 32;
	}

	OLED_ShowChar(x_pos, 2, ':', 32);

	x_pos += 16;

	if (gMenu != curMenu || min != preTime.min)
	{
		if (min < 10)
		{
			OLED_ShowNum(x_pos, 2, 0, 1, 32);
			x_pos += 16;
			OLED_ShowNum(x_pos, 2, min, 1, 32);
			x_pos += 16;
		}
		else
		{
			OLED_ShowNum(x_pos, 2, min, 2, 32);
			x_pos += 32;
		}
	}
	else
	{
		x_pos += 32;
	}

	if (event[0] == 0)
	{
		OLED_Clearrow(6);
		OLED_Clearrow(7);

		preTime.hour = hour;
		preTime.min = min;
		strcpy(preTime.event, event);

		return;
	}

	if (curMenu != gMenu || strcmp(preTime.event, event))
	{
		OLED_Clearrow(6);
		OLED_Clearrow(7);
	}

	OLED_ShowString((128 - 8*strlen(event))/2, 6, (uint8_t *)event, 16);

	preTime.hour = hour;
	preTime.min = min;
	strcpy(preTime.event, event);
}

void OLED_Show_Menu(void){
	OLED_Clear();
	uint8_t light;

	while(1){
		xSemaphoreTake(semHandle, portMAX_DELAY);
		light = gInfo.light;
		xSemaphoreGive(semHandle);

		if (light < 20)
			Show_OLED_Light(1);
		else if (light < 40)
			Show_OLED_Light(2);
		else
			Show_OLED_Light(3);

		if (gMenu == 1)
		{
			Show_Environment();
			curMenu = gMenu;
		}
		else if (gMenu == 2)
		{
			Show_Time();
			curMenu = gMenu;
		}
		else if (gMenu == 3)
		{
			Show_Alarm();
			curMenu = gMenu;
		}
		else if (gMenu == 4)
		{
			Show_Note();
			curMenu = gMenu;
		}
		osDelay(500);
	}
}
