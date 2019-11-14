#ifndef _MYIIC_H
#define _MYIIC_H
#include "sys.h"

//EEPROM
//IO方向设置
#define SDA_IN_E()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9输入模式
#define SDA_OUT_E() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9输出模式

//IO操作
#define IIC_SCL_E_1    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_SET) //SCL高电平--PB8
#define IIC_SCL_E_0    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET) //SCL低电平
#define IIC_SDA_E_1    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_SET)//SDA高电平--PB9
#define IIC_SDA_E_0    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET) 
#define READ_SDA_E  	 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)//读取SDA上的信号


//IO方向设置
#define SDA_IN_M()  {GPIOC->MODER&=~(3<<(1*2));GPIOC->MODER|=0<<1*2;}	//PC1输入模式
#define SDA_OUT_M() {GPIOC->MODER&=~(3<<(1*2));GPIOC->MODER|=1<<1*2;} //PC1输出模式

//IO操作
#define IIC_SCL_M_1    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET) //SCL高电平--PC0
#define IIC_SCL_M_0    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET) //SCL低电平
#define IIC_SDA_M_1    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_SET)//SDA高电平--PC1
#define IIC_SDA_M_0    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,GPIO_PIN_RESET) 
#define READ_SDA_M 	   HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)//读取SDA上的信号

//IIC所有操作函数
void IIC_Init_E(void);                //初始化IIC的IO口				 
void IIC_Start_E(void);				//发送IIC开始信号
void IIC_Stop_E(void);	  			//发送IIC停止信号
void IIC_Send_Byte_E(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte_E(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack_E(void); 				//IIC等待ACK信号
void IIC_Ack_E(void);					//IIC发送ACK信号
void IIC_NAck_E(void);				//IIC不发送ACK信号
void IIC_Write_One_Byte_E(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte_E(u8 daddr,u8 addr);	 


void IIC_Init_M(void);                //初始化IIC的IO口				 
void IIC_Start_M(void);				//发送IIC开始信号
void IIC_Stop_M(void);	  			//发送IIC停止信号
void IIC_Send_Byte_M(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte_M(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack_M(void); 				//IIC等待ACK信号
void IIC_Ack_M(void);					//IIC发送ACK信号
void IIC_NAck_M(void);				//IIC不发送ACK信号
void IIC_Write_One_Byte_M(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte_M(u8 daddr,u8 addr);	 
#endif

