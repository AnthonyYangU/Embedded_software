#include "MS5837.h"
#include "myiic.h"
#include "usart4.h"
#include "delay.h"

/*
C1 ѹ�������� SENS|T1
C2  ѹ������  OFF|T1
C3	�¶�ѹ��������ϵ�� TCS
C4	�¶�ϵ����ѹ������ TCO
C5	�ο��¶� T|REF
C6 	�¶�ϵ�����¶� TEMPSENS
*/
uint32_t  Cal_C[7];	        //���ڴ��PROM�е�6������1-6

double OFF_;
float Aux;
/*
dT ʵ�ʺͲο��¶�֮��Ĳ���
TEMP ʵ���¶�	
*/
uint64_t dT,TEMP;
/*
OFF ʵ���¶Ȳ���
SENS ʵ���¶�������
*/
uint64_t SENS;
uint32_t D1_Pres,D2_Temp;	// ����ѹ��ֵ,�����¶�ֵ
uint32_t TEMP2,T2,OFF2,SENS2;	//�¶�У��ֵ


uint32_t Pressure=0;				//��ѹ
uint32_t Atmdsphere_Pressure=0;//����ѹ
uint32_t Depth=0;

/*******************************************************************************
  * @��������	MS583730BA_RESET
  * @����˵��   ��λMS5611
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void MS583703BA_RESET(void)
{
		IIC_Start_M();
		IIC_Send_Byte_M(0xEC);//CSB�ӵأ�������ַ��0XEE������ 0X77
	  IIC_Wait_Ack_M();
    IIC_Send_Byte_M(0x1E);//���͸�λ����
	  IIC_Wait_Ack_M();
    IIC_Stop_M();	
}

/*******************************************************************************
  * @��������	MS5611_init
  * @����˵��   ��ʼ��5611
  * @�������  	��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void MS5837_init(void)
{	 
  uint8_t inth,intl;
  uint8_t i; 
	 
	IIC_Init_M();	         //��ʼ��IIC PC0 PC1��
	MS583703BA_RESET();	 // Reset Device  ��λMS5837
	delay_ms(20);       //��λ����ʱ��ע�������ʱ��һ����Ҫ�ģ��������̵��ƺ���������20ms��
	 
  for (i=1;i<=6;i++) 
	{
		IIC_Start_M();
    IIC_Send_Byte_M(0xEC);
		IIC_Wait_Ack_M();
		IIC_Send_Byte_M(0xA0 + (i*2));
		IIC_Wait_Ack_M();
    IIC_Stop_M();
		delay_us(5);
		IIC_Start_M();
		IIC_Send_Byte_M(0xEC+0x01);  //�������ģʽ
		delay_us(1);
		IIC_Wait_Ack_M();
		inth = IIC_Read_Byte_M(1);  		//��ACK�Ķ�����
		delay_us(1);
		intl = IIC_Read_Byte_M(0); 			//���һ���ֽ�NACK
		IIC_Stop_M();
    Cal_C[i] = (((uint16_t)inth << 8) | intl);
	}
	
	for(i=0;i<5;i++)
	{
		delay_ms(1);
		MS5837_Getdata();   //��ȡ����ѹ
    Atmdsphere_Pressure+=Pressure;	
		printf("%d\t",Pressure);               //�������ԭʼ����
	}
	Atmdsphere_Pressure=Atmdsphere_Pressure/5;
	printf("Atmdsphere_Pressure:%d\r\n",Atmdsphere_Pressure);               //�������ԭʼ����
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:unsigned long MS561101BA_getConversion(void)
*��������:    ��ȡ MS5837 ��ת����� 
*******************************************************************************/
unsigned long MS583703BA_getConversion(uint8_t command)
{
	unsigned long conversion = 0;
	uint8_t temp[3];

	IIC_Start_M();
	IIC_Send_Byte_M(0xEC); 		//д��ַ
	IIC_Wait_Ack_M();
	IIC_Send_Byte_M(command); //дת������
	IIC_Wait_Ack_M();
	IIC_Stop_M();

	delay_ms(10);
	IIC_Start_M();
	IIC_Send_Byte_M(0xEC); 		//д��ַ
	IIC_Wait_Ack_M();
	IIC_Send_Byte_M(0);				// start read sequence
	IIC_Wait_Ack_M();
	IIC_Stop_M();

	IIC_Start_M();
	IIC_Send_Byte_M(0xEC+0x01);  //�������ģʽ
	IIC_Wait_Ack_M();
	temp[0] = IIC_Read_Byte_M(1);  //��ACK�Ķ�����  bit 23-16
	temp[1] = IIC_Read_Byte_M(1);  //��ACK�Ķ�����  bit 8-15
	temp[2] = IIC_Read_Byte_M(0);  //��NACK�Ķ����� bit 0-7
	IIC_Stop_M();

	conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
	return conversion;
}

///***********************************************
//  * @brief  ��ȡ��ѹ
//  * @param  None
//  * @retval None
//************************************************/
void MS5837_Getdata(void)
{
	D1_Pres= MS583703BA_getConversion(0x48);
	D2_Temp = MS583703BA_getConversion(0x58);
	
	if(D2_Temp > (((uint32_t)Cal_C[5])*256))
	{		
		dT=D2_Temp - (((uint32_t)Cal_C[5])*256);
		TEMP=2000+dT*((uint32_t)Cal_C[6])/8388608;	
		OFF_=(uint32_t)Cal_C[2]*65536+((uint32_t)Cal_C[4]*dT)/128;
		SENS=(uint32_t)Cal_C[1]*32768+((uint32_t)Cal_C[3]*dT)/256;		
	}
	else 
	{	
		dT=(((uint32_t)Cal_C[5])*256) - D2_Temp;			
		TEMP=2000-dT*((uint32_t)Cal_C[6])/8388608;	
		OFF_=(uint32_t)Cal_C[2]*65536-((uint32_t)Cal_C[4]*dT)/128;
		SENS=(uint32_t)Cal_C[1]*32768-((uint32_t)Cal_C[3]*dT)/256;		
	}
//	printf("1:%d\t%d\t%llu\t%llu\t%f\t%llu\r\n",D1_Pres,D2_Temp,dT,TEMP,OFF_,SENS);
//printf("%d\t",Cal_C[5]);
	if(TEMP<2000)  // low temp
	{
	  Aux = (2000-TEMP)*(2000-TEMP);	
		T2 = 3*(dT*dT)/8589934592; 
		OFF2 = 3*Aux/2;
		SENS2 = 5*Aux/8;	
//		printf("2:%f\t%d\t%d\t%d\r\n",Aux,T2,OFF2,SENS2);		
	}
	else
	{
//		printf("!!");
		
	  Aux = (TEMP-2000)*(TEMP-2000);		
	  T2 = 2*(dT*dT)/137438953472;
		OFF2 = 1*Aux/16;
		SENS2 = 0;	
	}
	OFF_ = OFF_ - OFF2;
	SENS = SENS - SENS2;
	//printf("D1_Pres:%u\r\n",D1_Pres);
	TEMP=(TEMP-T2)/100;	
  Pressure= ((D1_Pres*SENS/2097152-OFF_)/8192)/10;
	Depth=0.983615*(Pressure-Atmdsphere_Pressure);	 
}