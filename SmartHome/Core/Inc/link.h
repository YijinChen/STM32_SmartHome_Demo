#ifndef LINK_H
#define LINK_H

#include "time.h"
#include "stm32f4xx_hal.h"

#define ALARMTYPE   0
#define NOTETYPE    1

typedef struct AlarmNode{
	uint8_t hour;
	uint8_t min;
	uint8_t alarm_type;
	char event[16];

	struct AlarmNode *next;
}AlarmNode;

void Init_Link(void);
void Add_Alarm_Link(uint8_t hour, uint8_t min);
void Link_Find_Alarm(uint8_t *h, uint8_t *m);
uint8_t Link_Alarm_Isset(void);
void Add_Note_Link(uint8_t hour, uint8_t min, char *event);
void Link_Find_Note(uint8_t *h, uint8_t *m, char *event);



#endif /* LINK_H */
