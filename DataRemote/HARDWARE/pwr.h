#ifndef __PWR_H
#define __PWR_H

#include "stm32l0xx_hal.h"
#include <stdlib.h>

#define VCC2 	 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)

void PWRA_Init(void);
void PWRB_Init(void);
void PWRC_Init(void);

#endif

