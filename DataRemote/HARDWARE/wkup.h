#ifndef __WKUP_H
#define __WKUP_H
#include "sys.h"


#define WKUP_KD HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)  //PC13 ���
#define WKUP_KD0 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)

uint8_t Check_WKUP(void);  			//���WKUP�ŵ��ź�
void WKUP_Init(void); 			//PC13 WKUP���ѳ�ʼ��
void Sys_Enter_Standby(void);	//ϵͳ�������ģʽ
#endif

