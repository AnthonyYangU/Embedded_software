#ifndef __USART1_H
#define __USART1_H	 
#include "sys.h"  
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//����1��ʼ��		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.csom
//�޸�����:2016/6/17
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//V1.0�޸�˵�� 
////////////////////////////////////////////////////////////////////////////////// 	

#define USART1_MAX_RECV_LEN		400					//�����ջ����ֽ���
#define USART1_MAX_SEND_LEN		1460					//����ͻ����ֽ���
#define USART1_RX_EN 			1					//0,������;1,����.
#define ARR_LEN 26
typedef struct
{
		double battery;
    double temperature;
		double deepth;
    double force1[ARR_LEN];
	  double force2[ARR_LEN];
		double stress1[ARR_LEN];
	  double stress2[ARR_LEN];
	  double current[ARR_LEN];
}testValue_t;

extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//���ջ���,���USART1_MAX_RECV_LEN�ֽ�
extern u8  USART1_TX_BUF[USART1_MAX_SEND_LEN]; 		//���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
extern u16 USART1_RX_STA;   						//��������״̬

extern UART_HandleTypeDef UART1_Handler;//UART���
void send_multi_data(testValue_t* str,u16 len);
void send_data(u8* str,u16 len);
void UART1_init(u32 bound);				//����3��ʼ�� 
void u2_printf(char* fmt, ...);

#endif



