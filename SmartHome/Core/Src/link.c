#include "link.h"
#include "FreeRtos.h"
#include <stdio.h>
#include "main.h"
#include <string.h>

AlarmNode *pxAlarmHead = NULL;
int8_t minFlag = -1;  //record if the minute of an alarm already passed
extern EnvInfo gInfo;

void Init_Link(void){
	pxAlarmHead = pvPortMalloc(sizeof(AlarmNode));
	if(pxAlarmHead == NULL){
		printf("Failed to allocate memory for pxAlarmHead\n");
		return;
	}

	pxAlarmHead->next = NULL;
}

void Add_Alarm_Link(uint8_t hour, uint8_t min){
	AlarmNode *p = pxAlarmHead->next;

	while (p)  //find a empty node for saving the time of new alarm
	{
		if (p->alarm_type == ALARMTYPE){
			break;
		}

		p = p->next;
	}

	if (p)          //alarm already exist
	{
		p->hour = hour;
		p->min = min;
	}
	else            //alarm not exist
	{
		p = pxAlarmHead;

		while (p->next)
			p = p->next;

		AlarmNode *newNode = pvPortMalloc(sizeof(AlarmNode));
		if (newNode == NULL){
			printf("Failed to allocate memory for newNode\r\n");
			return;
		}

		newNode->hour = hour;
		newNode->min = min;
		newNode->alarm_type = ALARMTYPE;
		newNode->next = NULL;

		p->next = newNode;
	}
}


void Link_Find_Alarm(uint8_t *h, uint8_t *m)
{
    if (pxAlarmHead == NULL) {
        //printf("pxAlarmHead is NULL\n");
        *h = 0;
        *m = 0;
        return;
    }

	AlarmNode *p = pxAlarmHead->next;
	if (p == NULL) {
	    //printf("The linked list is empty\n");
	    *h = 0;
	    *m = 0;
	    return;
	}

	while (p)
	{
		if (p->alarm_type == ALARMTYPE)
		{
			*h = p->hour;
			*m = p->min;
			return;
		}
		p = p->next;

	}

	if (!p)
	{
		*h = 0;
		*m = 0;
	}
}

//if the current time (a minute) match the time of an alarm.
uint8_t Link_Alarm_Isset(void)
{
	time_t curTime = gInfo.cur_time + 3600 * 1; //for german time (GMT+1), use 3600 * 1

	struct tm *t = localtime(&curTime);

	uint8_t hour = t->tm_hour;
	uint8_t min = t->tm_min;

	if (minFlag == min)
	{
		return 0;
	}
	else
	{
		minFlag = -1;   //at the last second of this minute, close the flag for next use
	}


	AlarmNode *p = pxAlarmHead->next;
	while (p)
	{
		if (p->hour == hour && p->min == min)
		{
			minFlag = min;  //indicate that the program already find an alarm
			return 1;
		}

		p = p->next;
	}
	return 0;
}

void Add_Note_Link(uint8_t hour, uint8_t min, char *event)
{
	AlarmNode *p = pxAlarmHead->next;

	while (p)
	{
		if (p->alarm_type == NOTETYPE)
			break;
		p = p->next;
	}

	if (p)          //if note already exist, cover it
	{
		p->hour = hour;
		p->min = min;
		strcpy(p->event, event);
	}
	else            //alarm not exist
	{
		p = pxAlarmHead;

		while (p->next)
			p = p->next;

		AlarmNode *newNode = pvPortMalloc(sizeof(AlarmNode));
		if (newNode == NULL)
			return;

		newNode->hour = hour;
		newNode->min = min;
		newNode->alarm_type = NOTETYPE;
		strcpy(newNode->event, event);
		newNode->next = NULL;
		p->next = newNode;
	}
}

void Link_Find_Note(uint8_t *h, uint8_t *m, char *event)
{
	AlarmNode *p = pxAlarmHead->next;

	while (p)
	{
		if (p->alarm_type == NOTETYPE)
		{
			*h = p->hour;
			*m = p->min;
			strcpy(event, p->event);
			return;
		}

		p = p->next;
	}
}


