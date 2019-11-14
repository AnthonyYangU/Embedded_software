#include "wkup.h"
#include "led.h"
#include "delay.h"

//extern RTC_HandleTypeDef RTC_Handler; 

//ÏµÍ³½øÈë´ý»úÄ£Ê½
void Sys_Enter_Standby(void)
{
    __HAL_RCC_AHB_FORCE_RESET();       //¸´Î»ËùÓÐIO¿Ú 
	
	//while(WKUP_KD);                     //µÈ´ýWK_UP°´¼üËÉ¿ª(ÔÚÓÐRTCÖÐ¶ÏÊ±,±ØÐëµÈWK_UPËÉ¿ªÔÙ½øÈë´ý»ú)
   
	__HAL_RCC_PWR_CLK_ENABLE();         //Ê¹ÄÜPWRÊ±ÖÓ
    __HAL_RCC_BACKUPRESET_FORCE();      //¸´Î»±¸·ÝÇøÓò
    HAL_PWR_EnableBkUpAccess();         //ºó±¸ÇøÓò·ÃÎÊÊ¹ÄÜ  
	
	/*
	//STM32F4,µ±¿ªÆôÁËRTCÏà¹ØÖÐ¶Ïºó,±ØÐëÏÈ¹Ø±ÕRTCÖÐ¶Ï,ÔÙÇåÖÐ¶Ï±êÖ¾Î»,È»ºóÖØÐÂÉèÖÃ
	//RTCÖÐ¶Ï,ÔÙ½øÈë´ý»úÄ£Ê½²Å¿ÉÒÔÕý³£»½ÐÑ,·ñÔò»áÓÐÎÊÌâ.	
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    __HAL_RTC_WRITEPROTECTION_DISABLE(&RTC_Handler);//¹Ø±ÕRTCÐ´±£»¤
    
    //¹Ø±ÕRTCÏà¹ØÖÐ¶Ï£¬¿ÉÄÜÔÚRTCÊµÑé´ò¿ªÁË
    __HAL_RTC_WAKEUPTIMER_DISABLE_IT(&RTC_Handler,RTC_IT_WUT);
    __HAL_RTC_TIMESTAMP_DISABLE_IT(&RTC_Handler,RTC_IT_TS);
    __HAL_RTC_ALARM_DISABLE_IT(&RTC_Handler,RTC_IT_ALRA|RTC_IT_ALRB);
    
    //Çå³ýRTCÏà¹ØÖÐ¶Ï±êÖ¾Î»
    __HAL_RTC_ALARM_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_ALRAF|RTC_FLAG_ALRBF);
    __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_TSF); 
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&RTC_Handler,RTC_FLAG_WUTF);
    
    __HAL_RCC_BACKUPRESET_RELEASE();                    //±¸·ÝÇøÓò¸´Î»½áÊø
    __HAL_RTC_WRITEPROTECTION_ENABLE(&RTC_Handler);     //Ê¹ÄÜRTCÐ´±£»¤
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //Çå³ýWake_UP±êÖ¾
	
		*/
		
    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN2|PWR_WAKEUP_PIN1);           //ÉèÖÃWKUPÓÃÓÚ»½ÐÑ
    HAL_PWR_EnterSTANDBYMode();                         //½øÈë´ý»úÄ£Ê½     
}
 
     
//¼ì²âWKUP½ÅµÄÐÅºÅ
//·µ»ØÖµ1:¸ßµçÆ½
//      0:µÍµçÆ½	
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
	u8 t=0;	//¼ÇÂ¼°´ÏÂµÄÊ±¼ä
	//LED0=0; //ÁÁµÆDS0 
	while(1)
	{
		if(WKUP_KD)
		{
			t++;			//ÒÑ¾­°´ÏÂÁË 
			delay_ms(10);
			if( t> 1)		//°´ÏÂ³¬¹ý1msÓ
			{
				//LED0=0;	 	//µãÁÁDS0 
				return 1; 	//°´ÏÂ1msÒÔÉÏÁË
			}
		}else 
		{ 
			//LED0=1;
			return 0; //°´ÏÂ²»×ã3Ãë
		}
	}
}
	

//Íâ²¿ÖÐ¶ÏÏß13ÖÐ¶Ï·þÎñº¯Êý


void EXTI4_15_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	
}



//ÖÐ¶ÏÏß13ÖÐ¶Ï´¦Àí¹ý³Ì
//´Ëº¯Êý»á±»HAL_GPIO_EXTI_IRQHandler()µ÷ÓÃ
//GPIO_Pin:Òý½Å

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  
    if(GPIO_Pin==GPIO_PIN_13)//PC13
    {
        if(Check_WKUP())//¹Ø»ú
        {
            Sys_Enter_Standby();//½øÈë´ý»úÄ£Ê½
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

//PC13 WKUP»½ÐÑ³õÊ¼»¯

void WKUP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOC_CLK_ENABLE();			//¿ªÆôGPIOCÊ±ÖÓ
		__HAL_RCC_GPIOA_CLK_ENABLE();
	
	
    GPIO_Initure.Pin=GPIO_PIN_13;            //PC13
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;  //ÖÐ¶Ï,ÉÏÉýÑØ
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //ÏÂÀ­
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //¿ìËÙ
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_0;            //PC13
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //ÏÂÀ­
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //¿ìËÙ
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
    
    //¼ì²éÊÇ·ñÊÇÕý³£¿ª»ú
	/*
    if(Check_WKUP()==0)
    {
       Sys_Enter_Standby();//²»ÊÇ¿ª»ú£¬½øÈë´ý»úÄ£Ê½
    }
	 */
    HAL_NVIC_SetPriority(EXTI4_15_IRQn,0x00,0x03);//ÇÀÕ¼ÓÅÏÈ¼¶2£¬×ÓÓÅÏÈ¼¶2
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
}
