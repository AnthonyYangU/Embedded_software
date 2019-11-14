/* Includes ------------------------------------------------------------------*/
#include "sys.h"
#include "delay.h" 
#include "usart4.h"
#include "stdlib.h"
#include "string.h"
#include "led.h"
#include "wkup.h"
#include "pwr.h"
#include "adc.h"
#include "24cxx.h"
#include "MS5837.h"
#include "usart1.h"
#include "sim800c.h"
#include "spi_sdcard.h"
#include "spi.h"
#include "l3g4200d.h"
//double adc=0;
//uint16_t adcx=0;
/*
//uint16_t adcx=0;
double adc=0;
uint8_t iic=56;
uint8_t a=0;


//要写入到24c128的字符串数组
const u8 TEXT_Buffer[]={"STM32L071 IIC TEST SUCCEED~~~YEAH"};
#define SIZE sizeof(TEXT_Buffer)
	
extern char  RxBuffer[100];
*/
void printArr(double arr[],u8 len){
	u8 ite;
	for(ite=0;ite<len;ite++){
		printf("Arr[%d] data is %f. \r\n",ite,arr[ite]);
	}
}


int main(void)
{
	//double adcx=0;
	//double val[20],dep[20];
	//double iteration,col,remind,finalData;
	
	//u8 receive_data[32]; 
//	u8 send_data[32];
//	u8 i,j,ite;
	//************系统初始化

	HAL_Init();
  SystemClock_Config();
  delay_init(32);
	usart4_init(115200);
	UART1_init(115200);
	LED_Init();
	LED_Control(LED_ON);//PD2
	//
	
	//Port which transmits data to Sim800c
	
	//Port transmits data to the computer
	
	//Gyro initialization
	//L3g4200d_Init();
	//Standby and wakeup function initialization
	
	//***********************触发/待机模式
	//唤醒管脚PC13(WAKEUP2)
	//唤醒管脚PA0(WAKEUP1)
	WKUP_Init();
	delay_ms(5);
  if(Check_WKUP()== 0)Sys_Enter_Standby();	
	
	
	//***********************电源管理控制测试
//	PWRA_Init();
//	PWRB_Init();
//	PWRC_Init(); 
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);//电流采集/水深传感器传感器/E2PROM/I2C/先电流13s
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);//深度传感器,PB1,PB0
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);//板载温度/陀螺仪
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);//拉压力传感器电桥供电,sd卡供电
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);//Sim800c
 //delay_ms(100);
	//********************ADC类型数据传输传感器电源使能
	//ADC使能
	//PC2(电流采集),传输通道为ADC_CHANNEL_12
	//PC1(压力传感器(水深传感器)),传输通道为ADC_CHANNEL_11
	//PA1(拉压力传感器(电桥)),传输通道为ADC_CHANNEL_1
	//PA3(电池电压),传输通道为ADC_CHANNEL_3
	//PB1，备用ADC口,传输通道为通道9
	//MY_ADC_Init();
	
	//*********************唤醒电流测量
//		for(iteration=0;iteration<200;iteration++){
//			col = Get_Adc(ADC_CHANNEL_12);
//			remind = (remind>col)?remind:col;
//		}
//		finalData = remind*3.3/4096;
//		printf("The vol is:%f",finalData);
//		
//		remind = 0;
	//********************水深传感器测试
//	for(i=0;i<20;i++){
//		adcx=Get_Adc_Average(ADC_CHANNEL_11,10);//获取通道12的转换值，20次取平均
//	  val[i] = 3.3*adcx/4096;
//		dep[i] = (val[i]-0.5)/0.05+1.0;
//		delay_ms(5);
//	}
//	for(ite=0;ite<20;ite++){
//		printf("Val[%d] data is %f. \r\n",ite,val[ite]);
//		delay_ms(10);
//	}

//	for(ite=0;ite<20;ite++){
//		printf("Dep[%d] data is %f. \r\n",ite,dep[ite]);
//		delay_ms(10);
//	}
//	delay_ms(12000);
//	if(Check_WKUP()== 0)Sys_Enter_Standby();	
	
	//******************PB1备用传感器
//	for(i=0;i<20;i++){
//		adcx=Get_Adc_Average(ADC_CHANNEL_9,10);//获取通道12的转换值，20次取平均
//	  val[i] = 3.3*adcx/4096;
//		dep[i] = (val[i]-0.5)/0.05;
//		delay_ms(5);
//	}
//	for(ite=0;ite<20;ite++){
//		printf("Val[%d] data is %f. \r\n",ite,val[ite]);
//		delay_ms(10);
//	}

//	for(ite=0;ite<20;ite++){
//		printf("Dep[%d] data is %f. \r\n",ite,dep[ite]);
//		delay_ms(10);
//	}
//	delay_ms(12000);
//	if(Check_WKUP()== 0)Sys_Enter_Standby();		
	//*********************拉压力传感器测试
	
	//*********************电池电压测试
	
	
	//*****************GPRS通信测试
		//sim800c_test();

	
	//**********************SD初始化
//	delay_ms(50);
//		switch(SD_Init())
//		{
//			case 0: 
//				printf("\r\nSD Card Init Success!\r\n");
//			break;
//			case 1:  
//						printf("Time Out!\n");
//						break;
//			case 99: 
//						printf("No Card!\n");
//						break;
//			default: printf("unknown err\n");
//			break;
//		}		
//		delay_ms(100);
//		
//	for(i=0;i<32;i++)
//    send_data[i]='D';
//		
//	//SD_WriteSingleBlock(40,send_data);
//	SD_ReadSingleBlock(40,receive_data);
//	delay_ms(10);
//	printf("receive_data is %s\n",receive_data);

	//****************SIM800C电源控制(长板)
	
	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);	
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);	
//	delay_ms(1300);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);	
//	
//	sim800c_test();
	
 //****************SIM800C电源控制1(跳线组合板)
	/*
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);	
	delay_ms(1300);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);	
	sim800c_test();  
	*/
	//*****************E2PROM控制
	/*
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	MS5837_init();	
	
	AT24CXX_Init();
	while(AT24CXX_Check())//检测不到24c128
	{
		printf("24C128 Check Failed!\r\n");
	}
	printf("24C128 Check Succeed!\r\n");
	printf("24C128 Ready!\r\n");
	printf("Start Write 24C128....\r\n");
	AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE);
	printf("24C128 Write Finished!\r\n");
	printf("Start Read 24C128....\r\n");
	AT24CXX_Read(0,datatemp,SIZE);
	printf("The Data Readed Is:\r\n%s\r\n",datatemp);
	*/
	
	/*
	MS5837_init();	
	MS5837_Getdata();   //获取大气压
	printf("Temp:%llu\r\n",TEMP);               //串口输出原始数据
	printf("Pressure:%u\r\n",Pressure); //串口输出原始数据
	printf("Depth:%u\r\n\r\n",Depth); //串口输出原始数据
	*/

 while (1)
  {
//		for(i=0;i<20;i++){
//			adcx=Get_Adc_Average(ADC_CHANNEL_11,10);//获取通道12的转换值，20次取平均
//			val[i] = 3.3*adcx/4096;
//			//dep[i] = (val[i]-0.5)/0.05+1.0;
//			delay_ms(5);
//	  }
//		for(ite=0;ite<20;ite++){
//			printf("Val[%d] data is %f. \r\n",ite,val[ite]);
//			delay_ms(10);
//		}

//	for(ite=0;ite<20;ite++){
//		printf("Dep[%d] data is %f. \r\n",ite,dep[ite]);
//		delay_ms(10);
//	}
//	delay_ms(12000);
//	if(Check_WKUP()== 0)Sys_Enter_Standby();	
		//**************唤醒电流峰值测量
//		for(iteration=0;iteration<200;iteration++){
//			col = Get_Adc_Average(ADC_CHANNEL_12,5);
//			remind = (remind>col)?remind:col;
//		}
//		finalData = remind*3.3/4096;
//		printf("The vol is:%f  \n",finalData);
//		remind = 0;		
		
		//************** Sim800c测试
		//printf("Test start\r\n");

		
		
//			for(i=0;i<20;i++){
//		adcx=Get_Adc_Average(ADC_CHANNEL_9,10);//获取通道12的转换值，20次取平均
//	  val[i] = 3.3*adcx/4096;
//		//dep[i] = (val[i]-0.5)/0.05;
//		delay_ms(5);
//	}
//	for(ite=0;ite<20;ite++){
//		printf("Val[%d] data is %f. \r\n",ite,val[ite]);
//		delay_ms(10);
//	}

//	for(ite=0;ite<20;ite++){
//		printf("Dep[%d] data is %f. \r\n",ite,dep[ite]);
//		delay_ms(10);
//	}
	}
		//delay_ms(100);
		/*
		delay_ms(10);
    memsx = 0;
		memsy = 0;
		memsz = 0;
		//printf("ss is\n");
		
		for(j = 0 ; j < 4 ; j++)
     {

       Read_AngularRate();

       memsx += ang_rate.Out_X ;
			 
       memsy += ang_rate.Out_Y ;

       memsz += ang_rate.Out_Z ;
     }
		
		 memsx = memsx * SENSITIVITY *0.25;
		 memsy = memsy * SENSITIVITY *0.25;
		 memsz = memsz * SENSITIVITY *0.25;
		 
		 
		  Read_AngularRate();
		   memsx = ang_rate.Out_X ;
			 
       memsy = ang_rate.Out_Y ;

       memsz = ang_rate.Out_Z ;
			 
		 printf("Memsx is:%f \n Memsy is:%f \n Memsz is:%f \n",memsx,memsy,memsz);
		 */

		
		//*************SD卡测试
		
		
		

		
		//finalDate = 0;
		//管脚电平高低控制
		/*
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		delay_ms(10000);
		
		
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		delay_ms(10000);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
		delay_ms(10000);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		delay_ms(10000);
		*/
		
	
	
	
	
	
	
	
	
	
		/*
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		HAL_Delay(2000);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);	
		*/
		/*
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
		AT24CXX_Write(0,(u8*)TEXT_Buffer,SIZE); 
		printf("24C128 Write Finished!\r\n");		
		delay_ms(50);		
		AT24CXX_Read(0,datatemp,SIZE);
		printf("The Data Readed Is:%s\r\n",datatemp);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		*/					    
		/*
		delay_ms(200);
		LED_Control(LED_OFF);
		delay_ms(200);
		*/
		
		/*
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	  MS5837_Getdata();   
		printf("Temp:%llu\r\n",TEMP);               //串口输出原始数据
		printf("Pressure:%u\r\n",Pressure); //串口输出原始数据
		printf("Depth:%d\r\n\r\n",Depth); //串口输出原始数据
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
		delay_ms(1000);
    */
	
	/*
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		adcx=Get_Adc_Average(ADC_CHANNEL_9,20);//获取通道9的转换值，20次取平均
		adc=3.3*adcx/4096;
		printf("%f\r\n",adc);
	*/
		
	
	//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	 
	 /*
	  LED_Control(LED_ON);
		delay_ms(500);
		LED_Control(LED_OFF);
		delay_ms(500);
		*/
		
		//HAL_Delay(1);
		//HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);
	
  
 // }

}

