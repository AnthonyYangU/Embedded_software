#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"

//EEPROM
//IO��������
#define SDA_IN_E()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
#define SDA_OUT_E() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ

//IO����
#define IIC_SCL_E_1    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET) //SCL�ߵ�ƽ--PB8
#define IIC_SCL_E_0    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET) //SCL�͵�ƽ
#define IIC_SDA_E_1    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)//SDA�ߵ�ƽ--PB9
#define IIC_SDA_E_0    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET) 
#define READ_SDA_E  	 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)//��ȡSDA�ϵ��ź�


//IO��������
#define SDA_IN_M()  {GPIOC->MODER&=~(3<<(1*2));GPIOC->MODER|=0<<1*2;}	//PC1����ģʽ
#define SDA_OUT_M() {GPIOC->MODER&=~(3<<(1*2));GPIOC->MODER|=1<<1*2;} //PC1���ģʽ

//IO����
#define IIC_SCL_M_1    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET) //SCL�ߵ�ƽ--PC0
#define IIC_SCL_M_0    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET) //SCL�͵�ƽ
#define IIC_SDA_M_1    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET)//SDA�ߵ�ƽ--PC1
#define IIC_SDA_M_0    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET) 
#define READ_SDA_M 	   HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)//��ȡSDA�ϵ��ź�

//IIC���в�������
void IIC_Init_E(void);                //��ʼ��IIC��IO��				 
void IIC_Start_E(void);				//����IIC��ʼ�ź�
void IIC_Stop_E(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte_E(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte_E(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack_E(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack_E(void);					//IIC����ACK�ź�
void IIC_NAck_E(void);				//IIC������ACK�ź�
void IIC_Write_One_Byte_E(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte_E(u8 daddr,u8 addr);	 


void IIC_Init_M(void);                //��ʼ��IIC��IO��				 
void IIC_Start_M(void);				//����IIC��ʼ�ź�
void IIC_Stop_M(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte_M(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte_M(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack_M(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack_M(void);					//IIC����ACK�ź�
void IIC_NAck_M(void);				//IIC������ACK�ź�
void IIC_Write_One_Byte_M(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte_M(u8 daddr,u8 addr);	 
#endif

