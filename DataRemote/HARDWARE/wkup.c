#include "wkup.h"
#include "led.h"
#include "delay.h"

//extern RTC_HandleTypeDef RTC_Handler; 

//系统进入待机模式
void Sys_Enter_Standby(void)
{
    __HAL_RCC_AHB_FORCE_RESET();       //复位所有IO口 
	
	//while(WKUP_KD);                     //等待WK_UP按键松开(在有RTC中断时,必须等WK_UP松开再进入待机)
   
	__HAL_RCC_PWR_CLK_ENABLE();         //使能PWR时钟
    __HAL_RCC_BACKUPRESET_FORCE();      //复位备份区域
    HAL_PWR_EnableBkUpAccess();         //后备区域访问使能  
	
	/*
	//STM32F4,当开启了RTC相关中断后,必须先关闭RTC中断,再清中断标志位,然后重新设置
	//RTC中断,再进入待机模式才可以正常唤醒,否则会有问题.	
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    __HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//关闭RTC写保护
    
    //关闭RTC相关中断，可能在RTC实验打开了
    __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
    __HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
    __HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);
    
    //清除RTC相关中断标志位
    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
    
    __HAL_RCC_BACKUPRESET_RELEASE();                    //备份区域复位结束
    __HAL_RTC_WRITEPROTECTION_ENABLE(&RTC_Handler);     //使能RTC写保护
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //清除Wake_UP标志
	
		*/
		
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2|PWR_WAKEUP_PIN1);           //设置WKUP用于唤醒
    HAL_PWR_EnterSTANDBYMode();                         //进入待机模式     
}
 
     
//检测WKUP脚的信号
//返回值1:高电平
//      0:低电平	
//uint8_t Check_WKUP(void) 
//{
//	delay_ms(1);
//		if(WKUP_KD||WKUP_KD0)
//		{
//				return 1; 	
//		}else 
//		{ 
//			return 0; 
//		}
//}
u8 Check_WKUP(void) 
{
	u8 t=0;	//记录按下的时间
	//LED0=0; //亮灯DS0 
	while(1)
	{
		if(WKUP_KD)
		{
			t++;			//已经按下了 
			delay_ms(10);
			if( t> 1)		//按下超过1ms�
			{
				//LED0=0;	 	//点亮DS0 
				return 1; 	//按下1ms以上了
			}
		}else 
		{ 
			//LED0=1;
			return 0; //按下不足3秒
		}
	}
}
	

//外部中断线13中断服务函数


void EXTI4_15_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	
}



//中断线13中断处理过程
//此函数会被HAL_GPIO_EXTI_IRQHandler()调用
//GPIO_Pin:引脚

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  
    if(GPIO_Pin==GPIO_PIN_13)//PC13
    {
        if(Check_WKUP())//关机
        {
            Sys_Enter_Standby();//进入待机模式
        }
    }
/*    
			if(GPIO_Pin==GPIO_PIN_13)//PC13
		{
			if(my_signal>=20 && Check_WKUP())
			{		  
				Sys_Enter_Standby();  
			}			
		}
*/
}

//PC13 WKUP唤醒初始化

void WKUP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
		__HAL_RCC_GPIOA_CLK_ENABLE();
	
	
    GPIO_Initure.Pin=GPIO_PIN_13;            //PC13
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //中断,上升沿
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_0;            //PC13
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    //检查是否是正常开机
	/*
    if(Check_WKUP()==0)
    {
       Sys_Enter_Standby();//不是开机，进入待机模式
    }
	 */
    HAL_NVIC_SetPriority(EXTI4_15_IRQn,0x00,0x03);//抢占优先级2，子优先级2
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}
