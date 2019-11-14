#include "usart4.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 

int fputc(int ch, FILE *f)
{ 
	while((USART4->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART4->TDR = (uint8_t) ch;      
	return ch;
}

#endif 

//#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
//u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
//u16 USART_RX_STA=0;       //����״̬���	
#if EN_USART4_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���


USART_HandleTypeDef USART4_Handler; //UART���


//��ʼ��IO ����4
//bound:������
void usart4_init(uint32_t bound)
{
	
	USART4_Handler.Instance=USART4;					    //USART4
	USART4_Handler.Init.BaudRate=bound;				    //������
	USART4_Handler.Init.WordLength=USART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	USART4_Handler.Init.StopBits=USART_STOPBITS_1;	    //һ��ֹͣλ
	USART4_Handler.Init.Parity=USART_PARITY_NONE;		    //����żУ��λ
	USART4_Handler.Init.Mode=USART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_USART_Init(&USART4_Handler);					    //HAL_UART_Init()��ʹ��UART4
	
	//HAL_USART_Receive_IT(&USART4_Handler, (uint8_t *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
  
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_USART_MspInit(USART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART4)//����Ǵ���4�����д���4 MSP��ʼ��
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();			//ʹ��GPIOCʱ��
		__HAL_RCC_USART4_CLK_ENABLE();			//ʹ��USART4ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_10;			//PC10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF6_USART4;	//����ΪUSART4
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	//��ʼ��PC10
	
		GPIO_Initure.Pin=GPIO_PIN_11;			
		HAL_GPIO_Init(GPIOC,&GPIO_Initure);	   	//��ʼ��PC11
		
#if EN_USART4_RX
		HAL_NVIC_EnableIRQ(USART4_5_IRQn);		//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART4_5_IRQn,3,3);	//��ռ���ȼ�3�������ȼ�3
#endif	
	}

}

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *huart)
{
	if(huart->Instance==USART4)//����Ǵ���1
	{
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(aRxBuffer[0]!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(aRxBuffer[0]==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}

	}
}
 
//����4�жϷ������
void USART4_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	
	HAL_USART_IRQHandler(&USART4_Handler);	//����HAL���жϴ������ú���
	
	timeout=0;
    while (HAL_USART_GetState(&USART4_Handler) != HAL_UART_STATE_READY)//�ȴ�����
	{
	 timeout++;////��ʱ����
     if(timeout>maxDelay) break;		
	}
     
	timeout=0;
	while(HAL_USART_Receive_IT(&USART4_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
	{
	 timeout++; //��ʱ����
	 if(timeout>maxDelay) break;	
	}
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}

#endif	
