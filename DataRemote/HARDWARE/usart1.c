#include "usart1.h"
#include "delay.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "timer.h"


//���ڷ��ͻ����� 	
__align(8) u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 	      //���ͻ���,���USART1_MAX_SEND_LEN�ֽ�
#ifdef USART1_RX_EN   								      //���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				      //���ջ���,���USART1_MAX_RECV_LEN���ֽ�.

UART_HandleTypeDef UART1_Handler;                         //UART���
static const u8 HeadId[2] = {'\x5a','\x5b'};
static const u8 deviceId[2] = {'\x10','\x01'};
//��ʼ��IO,����1
//bound:������
void UART1_init(u32 bound)
{	
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_GPIOA_CLK_ENABLE();			              //ʹ��GPIOAʱ��
	__HAL_RCC_USART1_CLK_ENABLE();			              //ʹ��USART1ʱ��

	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					      //USART1
	UART1_Handler.Init.BaudRate=bound;				      //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;     //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	      //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		      //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;     //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		      //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					      //HAL_UART_Init()��ʹ��UART3	

	GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10;			  //PA2,PA3
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		              //�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			              //����
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		              //����
	GPIO_Initure.Alternate=GPIO_AF4_USART1;	              //����ΪUSART1
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	            	  //��ʼ��PB10,��PB11
	
	__HAL_UART_DISABLE_IT(&UART1_Handler,UART_IT_TC);
#if USART1_RX_EN 
	__HAL_UART_ENABLE_IT(&UART1_Handler,UART_IT_RXNE);    //���������ж�
	HAL_NVIC_EnableIRQ(USART1_IRQn);				      //ʹ��USART1�ж�ͨ��
	HAL_NVIC_SetPriority(USART1_IRQn,2,3);			      //��ռ���ȼ�2�������ȼ�3
#endif	
	TIM7_Init(100-1,9000-1);                              //��������Ϊ10ms�ж�	
	HAL_TIM_Base_Stop(&TIM7_Handler);                     //�رն�ʱ��7
	USART1_RX_STA=0;				                      //���� 
	
  
}

//����ɨ���£�   timer=1S
//������ɨ���£� timer=10ms
//ͨ���жϽ�������2���ַ�֮���ʱ������timer�������ǲ���һ������������.
//���2���ַ����ռ������timer,����Ϊ����1����������.Ҳ���ǳ���timerû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART1_RX_STA=0;  

//����3�жϷ������
void USART1_IRQHandler(void)
{
	  u8 Res;
	  if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))
	  {
		  HAL_UART_Receive(&UART1_Handler,&Res,1,1000);
		  if((USART1_RX_STA&0x8000)==0)                //�������һ������,��û�б�����,���ٽ�����������
		  { 
			 if(USART1_RX_STA<USART1_MAX_RECV_LEN)     //�����Խ�������
			 {
				  TIM7->CNT = 0;                       //���������
				  if(USART1_RX_STA==0)                 //���û�н����κ�����
				  {
					 HAL_TIM_Base_Start(&TIM7_Handler);//������ʱ��7 
				  }
				  USART1_RX_BUF[USART1_RX_STA++]=Res;  //��¼���յ���ֵ
			 }
			 else
			 {
				  USART1_RX_STA|=1<<15;			       //ǿ�Ʊ�ǽ������
			 }
		  }
	  }
	  HAL_UART_IRQHandler(&UART1_Handler);	
}

//����1,printf ����
//ȷ��һ�η������ݲ�����USART1_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	u16 i,j;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART1_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART1_TX_BUF);//�˴η������ݵĳ���
	for(j=0;j<i;j++)                     //ѭ����������
	{
		while((USART1->ISR&0X40)==0);     //ѭ������,ֱ���������   
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
		while((USART1->ISR&0X40)==0);     //ѭ������,ֱ���������   
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
		while((USART1->ISR&0X40)==0);     //ѭ������,ֱ���������   
		USART1->TDR=USART1_TX_BUF[i];
	}
}
#endif











