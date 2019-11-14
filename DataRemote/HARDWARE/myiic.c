#include "myiic.h"
#include "delay.h"



//IIC初始化
void IIC_Init_E(void)
{
    GPIO_InitTypeDef GPIO_Initure;
   
	
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    
    //PB8,9初始化设置
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
   
	/*
	 __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    
    //PC0,1初始化设置
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	*/	
    IIC_SDA_E_1;
    IIC_SCL_E_1;  
}

//产生IIC起始信号
void IIC_Start_E(void)
{
	SDA_OUT_E();     //sda线输出
	IIC_SDA_E_1;	  	  
	IIC_SCL_E_1;
	delay_us(4);
 	IIC_SDA_E_0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_E_0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop_E(void)
{
	SDA_OUT_E();//sda线输出
	IIC_SCL_E_0;
	IIC_SDA_E_0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_E_1; 
	delay_us(4);			
	IIC_SDA_E_1;//发送I2C总线结束信号				   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack_E(void)
{
	u8 ucErrTime=0;
	SDA_IN_E();      //SDA设置为输入  
	IIC_SDA_E_1;delay_us(1);	   
	IIC_SCL_E_1;delay_us(1);	 
	while(READ_SDA_E)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop_E();
			return 1;
		}
	}
	IIC_SCL_E_0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack_E(void)
{
	IIC_SCL_E_0;
	SDA_OUT_E();
	IIC_SDA_E_0;
	delay_us(2);
	IIC_SCL_E_1;
	delay_us(2);
	IIC_SCL_E_0;
}
//不产生ACK应答		    
void IIC_NAck_E(void)
{
	IIC_SCL_E_0;
	SDA_OUT_E();
	IIC_SDA_E_1;
	delay_us(2);
	IIC_SCL_E_1;
	delay_us(2);
	IIC_SCL_E_0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte_E(u8 txd)
{                        
    u8 t;   
	SDA_OUT_E(); 	    
    IIC_SCL_E_0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
			if((txd&0x80)>>7)
			{
				IIC_SDA_E_1;
			}
		  else
			{
				IIC_SDA_E_0;				
			}
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_E_1;
		delay_us(2); 
		IIC_SCL_E_0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte_E(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN_E();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_E_0; 
        delay_us(2);
		IIC_SCL_E_1;
        receive<<=1;
        if(READ_SDA_E)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck_E();//发送nACK
    else
        IIC_Ack_E(); //发送ACK   
    return receive;
}



//IIC初始化
void IIC_Init_M(void)
{
    GPIO_InitTypeDef GPIO_Initure;
   
/*	
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    
    //PB8,9初始化设置
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
*/    
	
	 __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    
    //PC0,1初始化设置
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
    IIC_SDA_M_1;
    IIC_SCL_M_1;  
}

//产生IIC起始信号
void IIC_Start_M(void)
{
	SDA_OUT_M();     //sda线输出
	IIC_SDA_M_1;	  	  
	IIC_SCL_M_1;
	delay_us(4);
 	IIC_SDA_M_0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_M_0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop_M(void)
{
	SDA_OUT_M();//sda线输出
	IIC_SCL_M_0;
	IIC_SDA_M_0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_M_1; 
	delay_us(4);			
	IIC_SDA_M_1;//发送I2C总线结束信号				   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack_M(void)
{
	u8 ucErrTime=0;
	SDA_IN_M();      //SDA设置为输入  
	IIC_SDA_M_1;delay_us(1);	   
	IIC_SCL_M_1;delay_us(1);	 
	while(READ_SDA_M)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop_M();
			return 1;
		}
	}
	IIC_SCL_M_0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack_M(void)
{
	IIC_SCL_M_0;
	SDA_OUT_M();
	IIC_SDA_M_0;
	delay_us(2);
	IIC_SCL_M_1;
	delay_us(2);
	IIC_SCL_M_0;
}
//不产生ACK应答		    
void IIC_NAck_M(void)
{
	IIC_SCL_M_0;
	SDA_OUT_M();
	IIC_SDA_M_1;
	delay_us(2);
	IIC_SCL_M_1;
	delay_us(2);
	IIC_SCL_M_0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte_M(u8 txd)
{                        
    u8 t;   
	SDA_OUT_M(); 	    
    IIC_SCL_M_0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
			if((txd&0x80)>>7)
			{
				IIC_SDA_M_1;
			}
		  else
			{
				IIC_SDA_M_0;				
			}
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_M_1;
		delay_us(2); 
		IIC_SCL_M_0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte_M(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN_M();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_M_0; 
        delay_us(2);
		IIC_SCL_M_1;
        receive<<=1;
        if(READ_SDA_M)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck_M();//发送nACK
    else
        IIC_Ack_M(); //发送ACK   
    return receive;
}


