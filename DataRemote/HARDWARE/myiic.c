#include "myiic.h"
#include "delay.h"



//IIC��ʼ��
void IIC_Init_E(void)
{
    GPIO_InitTypeDef GPIO_Initure;
   
	
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
    //PB8,9��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
   
	/*
	 __HAL_RCC_GPIOC_CLK_ENABLE();   //ʹ��GPIOCʱ��
    
    //PC0,1��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	*/	
    IIC_SDA_E_1;
    IIC_SCL_E_1;  
}

//����IIC��ʼ�ź�
void IIC_Start_E(void)
{
	SDA_OUT_E();     //sda�����
	IIC_SDA_E_1;	  	  
	IIC_SCL_E_1;
	delay_us(4);
 	IIC_SDA_E_0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_E_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop_E(void)
{
	SDA_OUT_E();//sda�����
	IIC_SCL_E_0;
	IIC_SDA_E_0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_E_1; 
	delay_us(4);			
	IIC_SDA_E_1;//����I2C���߽����ź�				   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack_E(void)
{
	u8 ucErrTime=0;
	SDA_IN_E();      //SDA����Ϊ����  
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
	IIC_SCL_E_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte_E(u8 txd)
{                        
    u8 t;   
	SDA_OUT_E(); 	    
    IIC_SCL_E_0;//����ʱ�ӿ�ʼ���ݴ���
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
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_E_1;
		delay_us(2); 
		IIC_SCL_E_0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte_E(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN_E();//SDA����Ϊ����
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
        IIC_NAck_E();//����nACK
    else
        IIC_Ack_E(); //����ACK   
    return receive;
}



//IIC��ʼ��
void IIC_Init_M(void)
{
    GPIO_InitTypeDef GPIO_Initure;
   
/*	
    __HAL_RCC_GPIOB_CLK_ENABLE();   //ʹ��GPIOBʱ��
    
    //PB8,9��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
*/    
	
	 __HAL_RCC_GPIOC_CLK_ENABLE();   //ʹ��GPIOCʱ��
    
    //PC0,1��ʼ������
    GPIO_Initure.Pin=GPIO_PIN_0|GPIO_PIN_1;
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
		
    IIC_SDA_M_1;
    IIC_SCL_M_1;  
}

//����IIC��ʼ�ź�
void IIC_Start_M(void)
{
	SDA_OUT_M();     //sda�����
	IIC_SDA_M_1;	  	  
	IIC_SCL_M_1;
	delay_us(4);
 	IIC_SDA_M_0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_M_0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop_M(void)
{
	SDA_OUT_M();//sda�����
	IIC_SCL_M_0;
	IIC_SDA_M_0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_M_1; 
	delay_us(4);			
	IIC_SDA_M_1;//����I2C���߽����ź�				   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack_M(void)
{
	u8 ucErrTime=0;
	SDA_IN_M();      //SDA����Ϊ����  
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
	IIC_SCL_M_0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte_M(u8 txd)
{                        
    u8 t;   
	SDA_OUT_M(); 	    
    IIC_SCL_M_0;//����ʱ�ӿ�ʼ���ݴ���
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
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_M_1;
		delay_us(2); 
		IIC_SCL_M_0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte_M(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN_M();//SDA����Ϊ����
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
        IIC_NAck_M();//����nACK
    else
        IIC_Ack_M(); //����ACK   
    return receive;
}


