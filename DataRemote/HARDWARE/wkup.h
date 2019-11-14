#ifndef __WKUP_H
#define __WKUP_H
#include "sys.h"


#define WKUP_KD HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)  //PC13 检测
#define WKUP_KD0 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

uint8_t Check_WKUP(void);  			//检测WKUP脚的信号
void WKUP_Init(void); 			//PC13 WKUP唤醒初始化
void Sys_Enter_Standby(void);	//系统进入待机模式
#endif

