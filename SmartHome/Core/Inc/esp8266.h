
#ifndef ESP8266_H
#define ESP8266_H
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

void ESP8266_Init(void);
uint8_t ESP8266_Response(const char *msg, uint32_t timeout);
void ESP8266_SetBaud(uint32_t baud);
void ESP8266_Refresh_Buffer(void);
uint8_t ESP8266_Connect_AP(const char *ssid, const char *passwd);
uint8_t ESP8266_Connect_MQTT(const char *ip, int port, const char *user, const char *passwd);
void ESP8266_Publish(const char *topic, const char *msg);
uint8_t ESP8266_Subscribe(const char *topic);
void ESP8266_Send_Cmd(const char *cmd);


#endif /* ESP8266_H */
