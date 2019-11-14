#include "usart4.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用os,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os 使用	  
#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 

int fputc(int ch, FILE *f)
{ 
	while((USART4->ISR&0X40)==0);//循环发送,直到发送完毕   
	USART4->TDR = (uint8_t) ch;      
	return ch;
}

#endif 

//#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
//u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
//u16 USART_RX_STA=0;       //接收状态标记	
#if EN_USART4_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	

u8 aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲


USART_HandleTypeDef USART4_Handler; //UART句柄


//初始化IO 串口4
//bound:波特率
void usart4_init(uint32_t bound)
{
	
	USART4_Handler.Instance=USART4;					    //USART4
	USART4_Handler.Init.BaudRate=bound;				    //波特率
	USART4_Handler.Init.WordLength=USART_WORDLENGTH_8B;   //字长为8位数据格式
	USART4_Handler.Init.StopBits=USART_STOPBITS_1;	    //一个停止位
	USART4_Handler.Init.Parity=USART_PARITY_NONE;		    //无奇偶校验位
	USART4_Handler.Init.Mode=USART_MODE_TX_RX;		    //收发模式
	HAL_USART_Init(&USART4_Handler);					    //HAL_UART_Init()会使能UART4
	
	//HAL_USART_Receive_IT(&USART4_Handler, (uint8_t *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
  
}

//UART底层初始化，时钟使能，引脚配置，中断配置
//此函数会被HAL_UART_Init()调用
//huart:串口句柄

void HAL_USART_MspInit(USART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART4)//如果是串口4，进行串口4 MSP初始化
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();			//使能GPIOC时钟
		__HAL_RCC_USART4_CLK_ENABLE();			//使能USART4时钟
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PC10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//高速
		GPIO_Initure.Alternate=GPIO_AF6_USART4;	//复用为USART4
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	//初始化PC10
	
		GPIO_Initure.Pin=GPIO_PIN_11;			
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	//初始化PC11
		
#if EN_USART4_RX
		HAL_NVIC_EnableIRQ(USART4_5_IRQn);		//使能USART1中断通道
		HAL_NVIC_SetPriority(USART4_5_IRQn,3,3);	//抢占优先级3，子优先级3
#endif	
	}

}

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *huart)
{
	if(huart->Instance==USART4)//如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}

	}
}
 
//串口4中断服务程序
void USART4_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntEnter();    
#endif
	
	HAL_USART_IRQHandler(&USART4_Handler);	//调用HAL库中断处理公用函数
	
	timeout=0;
    while (HAL_USART_GetState(&USART4_Handler) != HAL_UART_STATE_READY)//等待就绪
	{
	 timeout++;////超时处理
     if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_USART_Receive_IT(&USART4_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
	 timeout++; //超时处理
	 if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//使用OS
	OSIntExit();  											 
#endif
}

#endif	

