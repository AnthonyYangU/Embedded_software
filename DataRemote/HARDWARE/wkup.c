#include "wkup.h"
#include "led.h"
#include "delay.h"

//extern RTC_HandleTypeDef RTC_Handler; 

//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{
    __HAL_RCC_AHB_FORCE_RESET();       //��λ����IO�� 
	
	//while(WKUP_KD);                     //�ȴ�WK_UP�����ɿ�(����RTC�ж�ʱ,�����WK_UP�ɿ��ٽ������)
   
	__HAL_RCC_PWR_CLK_ENABLE();         //ʹ��PWRʱ��
    __HAL_RCC_BACKUPRESET_FORCE();      //��λ��������
    HAL_PWR_EnableBkUpAccess();         //���������ʹ��  
	
	/*
	//STM32F4,��������RTC����жϺ�,�����ȹر�RTC�ж�,�����жϱ�־λ,Ȼ����������
	//RTC�ж�,�ٽ������ģʽ�ſ�����������,�����������.	
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    __HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//�ر�RTCд����
    
    //�ر�RTC����жϣ�������RTCʵ�����
    __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
    __HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
    __HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);
    
    //���RTC����жϱ�־λ
    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
    
    __HAL_RCC_BACKUPRESET_RELEASE();                    //��������λ����
    __HAL_RTC_WRITEPROTECTION_ENABLE(&RTC_Handler);     //ʹ��RTCд����
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //���Wake_UP��־
	
		*/
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU); 
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2|PWR_WAKEUP_PIN1);           //����WKUP���ڻ���
    HAL_PWR_EnterSTANDBYMode();                         //�������ģʽ     
}
 
     
//���WKUP�ŵ��ź�
//����ֵ1:�ߵ�ƽ
//      0:�͵�ƽ	
uint8_t Check_WKUP(void) 
{
		if(WKUP_KD||WKUP_KD0)
		{
				return 1; 	
		}else 
		{ 
			return 0; 
		}
}
	

//�ⲿ�ж���13�жϷ�����


//void EXTI4_15_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
//}



//�ж���13�жϴ������
//�˺����ᱻHAL_GPIO_EXTI_IRQHandler()����
//GPIO_Pin:����

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
////    if(GPIO_Pin==GPIO_PIN_13)//PC13
////    {
////        if(Check_WKUP())//�ػ�
////        {
////            Sys_Enter_Standby();//�������ģʽ
////        }
////    } 

//}

//PC13 WKUP���ѳ�ʼ��

void WKUP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOCʱ��
		__HAL_RCC_GPIOA_CLK_ENABLE();
	
	
    GPIO_Initure.Pin=GPIO_PIN_13;            //PC13
    //GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //�ж�,������
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_0;            //PA0
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //����
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    //����Ƿ�����������
	/*
    if(Check_WKUP()==0)
    {
       Sys_Enter_Standby();//���ǿ������������ģʽ
    }
	 */
//    HAL_NVIC_SetPriority(EXTI4_15_IRQn,0x00,0x03);//��ռ���ȼ�2�������ȼ�2
//    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}
