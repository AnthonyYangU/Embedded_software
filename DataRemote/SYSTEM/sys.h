#ifndef __SYS_H
#define __SYS_H

#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo.h"


typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  
typedef __I uint16_t vuc16; 
typedef __I uint8_t vuc8;  

typedef unsigned short	WCHAR;

void SystemClock_Config(void);

#endif
