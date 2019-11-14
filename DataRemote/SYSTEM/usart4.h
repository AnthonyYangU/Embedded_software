#ifndef __USART4_H
#define __USART4_H

#include "stm32l0xx_hal.h"
#include <stdlib.h>
#include "sys.h"

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern USART_HandleTypeDef USART4_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

void usart4_init(uint32_t bound);

#endif
