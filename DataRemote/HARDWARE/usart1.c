#include "usart1.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "timer.h"


//串口发送缓存区 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	      //发送缓冲,最大USART1_MAX_SEND_LEN字节
#ifdef USART1_RX_EN   								      //如果使能了接收   	  
//串口接收缓存区 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				      //接收缓冲,最大USART1_MAX_RECV_LEN个字节.

UART_HandleTypeDef UART1_Handler;                         //UART句柄
static const u8 HeadId[2] = {'\x5a','\x5b'};
static const u8 deviceId[2] = {'\x10','\x01'};
//初始化IO,串口1
//bound:波特率
void UART1_init(u32 bound)
{	
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();			              //使能GPIOA时钟
	__HAL_RCC_USART1_CLK_ENABLE();			              //使能USART1时钟

	//UART 初始化设置
	UART1_Handler.Instance=USART1;					      //USART1
	UART1_Handler.Init.BaudRate=bound;				      //波特率
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;     //字长为8位数据格式
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	      //一个停止位
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		      //无奇偶校验位
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;     //无硬件流控
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		      //收发模式
	HAL_UART_Init(&UART1_Handler);					      //HAL_UART_Init()会使能UART3	

	GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10;			  //PA2,PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		              //复用推挽输出
	GPIO_Initure.Pull=GPIO_PULLUP;			              //上拉
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		              //高速
	GPIO_Initure.Alternate=GPIO_AF4_USART1;	              //复用为USART1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	            	  //初始化PB10,和PB11
	
	__HAL_UART_DISABLE_IT(&UART1_Handler,UART_IT_TC);
#if USART1_RX_EN 
	__HAL_UART_ENABLE_IT(&UART1_Handler,UART_IT_RXNE);    //开启接收中断
	HAL_NVIC_EnableIRQ(USART1_IRQn);				      //使能USART1中断通道
	HAL_NVIC_SetPriority(USART1_IRQn,2,3);			      //抢占优先级2，子优先级3
#endif	
	TIM7_Init(100-1,9000-1);                              //重新设置为10ms中断	
	HAL_TIM_Base_Stop(&TIM7_Handler);                     //关闭定时器7
	USART1_RX_STA=0;				                      //清零 
	
  
}

//蓝牙扫描下：   timer=1S
//非蓝牙扫描下： timer=10ms
//通过判断接收连续2个字符之间的时间差不大于timer来决定是不是一次连续的数据.
//如果2个字符接收间隔超过timer,则认为不是1次连续数据.也就是超过timer没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART1_RX_STA=0;  

//串口3中断服务程序
void USART1_IRQHandler(void)
{
	  u8 Res;
	  if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))
	  {
		  HAL_UART_Receive(&UART1_Handler,&Res,1,1000);
		  if((USART1_RX_STA&0x8000)==0)                //接收完的一批数据,还没有被处理,则不再接收其他数据
		  { 
			 if(USART1_RX_STA<USART1_MAX_RECV_LEN)     //还可以接收数据
			 {
				  TIM7->CNT = 0;                       //计数器清空
				  if(USART1_RX_STA==0)                 //如果没有接收任何数据
				  {
					 HAL_TIM_Base_Start(&TIM7_Handler);//开启定时器7 
				  }
				  USART1_RX_BUF[USART1_RX_STA++]=Res;  //记录接收到的值
			 }
			 else
			 {
				  USART1_RX_STA|=1<<15;			       //强制标记接收完成
			 }
		  }
	  }
	  HAL_UART_IRQHandler(&UART1_Handler);	
}

//串口1,printf 函数
//确保一次发送数据不超过USART1_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);//此次发送数据的长度
	for(j=0;j<i;j++)                     //循环发送数据
	{
		while((USART1->ISR&0X40)==0);     //循环发送,直到发送完毕   
		USART1->TDR=USART1_TX_BUF[j];  
	}
}

void send_multi_data(testValue_t* str,u16 len)
{
	u16 i;
	memcpy(&USART1_TX_BUF[0],&HeadId,2);
	memcpy(&USART1_TX_BUF[2],&deviceId,2);
	memcpy(&USART1_TX_BUF[4],str,len);
	for(i=0;i<len+4;i++)
	{
		while((USART1->ISR&0X40)==0);     //循环发送,直到发送完毕   
		USART1->TDR=USART1_TX_BUF[i];
	}
}

void send_data(u8* str,u16 len)
{
	u16 i;
	memcpy(&USART1_TX_BUF[0],&HeadId,2);
	memcpy(&USART1_TX_BUF[2],&deviceId,2);
	memcpy(&USART1_TX_BUF[4],str,len);
	for(i=0;i<len+4;i++)
	{
		while((USART1->ISR&0X40)==0);     //循环发送,直到发送完毕   
		USART1->TDR=USART1_TX_BUF[i];
	}
}
#endif











