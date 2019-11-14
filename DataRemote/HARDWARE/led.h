#ifndef __LED_H
#define __LED_H

#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo.h"
#include <stdlib.h>

extern uint8_t LED_ON;
extern uint8_t	LED_OFF;

void LED_Init(void);
void LED_Control(uint8_t flag);

#endif
