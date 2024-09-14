#ifndef MQ135_H
#define MQ135_H

#include "stm32f4xx_hal.h"

void MQ135_Init(ADC_HandleTypeDef* hadc);
float MQ135_ReadPPM(void);

#endif
