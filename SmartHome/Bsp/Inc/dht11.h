#ifndef DHT11_H
#define DHT11_H

#include "stm32f4xx_hal.h"

void DHT11_Init(void);
void DHT11_Start(void);
void DHT11_Read_Data(uint8_t *pData);

//void Delay_US(int us);
//void start_signal(void);
//uint8_t check_response(void);
//uint8_t read_byte(void);
//void process_sensor_data(void);

#endif

