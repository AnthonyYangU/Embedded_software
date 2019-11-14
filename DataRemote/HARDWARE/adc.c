#include "adc.h"



ADC_HandleTypeDef ADC1_Handler;//ADC���

//��ʼ��ADC
//ch: ADC_channels 
//ͨ��ֵ 0~16ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
void MY_ADC_Init(void)
{ 
    ADC1_Handler.Instance=ADC1;
    ADC1_Handler.Init.ClockPrescaler=ADC_CLOCK_SYNC_PCLK_DIV4;   //4��Ƶ��ADCCLK=PCLK2/4=90/4=22.5MHZ
    ADC1_Handler.Init.Resolution=ADC_RESOLUTION_12B;             //12λģʽ
    ADC1_Handler.Init.DataAlign=ADC_DATAALIGN_RIGHT;             //�Ҷ���
    ADC1_Handler.Init.ScanConvMode=DISABLE;                      //��ɨ��ģʽ
    ADC1_Handler.Init.EOCSelection=DISABLE;                      //�ر�EOC�ж� 
    ADC1_Handler.Init.ContinuousConvMode=DISABLE;                //�ر�����ת��
    ADC1_Handler.Init.DiscontinuousConvMode=DISABLE;             //��ֹ����������ģʽ
    ADC1_Handler.Init.ExternalTrigConv=ADC_SOFTWARE_START;       //�������
    ADC1_Handler.Init.ExternalTrigConvEdge=ADC_EXTERNALTRIGCONVEDGE_NONE;//ʹ���������
    ADC1_Handler.Init.DMAContinuousRequests=DISABLE;             //�ر�DMA����
		ADC1_Handler.Init.LowPowerAutoPowerOff=ENABLE;
	  ADC1_Handler.Init.LowPowerAutoWait=DISABLE;
		ADC1_Handler.Init.LowPowerFrequencyMode=ENABLE;
		ADC1_Handler.Init.Overrun=ADC_OVR_DATA_PRESERVED;
		ADC1_Handler.Init.OversamplingMode=DISABLE;
		ADC1_Handler.Init.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;
	
    HAL_ADC_Init(&ADC1_Handler);                                 //��ʼ�� 
}

//ADC�ײ��������������ã�ʱ��ʹ��
//�˺����ᱻHAL_ADC_Init()����
//hadc:ADC���
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	//�������P2 ADC�ɼ�
	 GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOC_CLK_ENABLE();			//����GPIOCʱ��
		__HAL_RCC_GPIOA_CLK_ENABLE();	
	  __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_2;//PC1
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
			
	  GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_0;
	  HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
		GPIO_Initure.Pin=GPIO_PIN_1|GPIO_PIN_3;
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	  
	
	//�����±�P5 ADC�ɼ�
	/* 
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_ADC1_CLK_ENABLE();            //ʹ��ADC1ʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();			//����GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_1;            //PB1
    GPIO_Initure.Mode=GPIO_MODE_ANALOG;     //ģ��
    GPIO_Initure.Pull=GPIO_NOPULL;          //����������
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	*/
}

//���ADCֵ
//ch: ͨ��ֵ 0~16��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_16
//����ֵ:ת�����
uint16_t Get_Adc(uint32_t ch)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
    
    ADC1_ChanConf.Channel=ch;                                   //ͨ��
   // ADC1_ChanConf.Rank=9;                                       //��9�����У�����9
     
		ADC1_ChanConf.Rank=12;                                       //��12�����У�����12
	
    HAL_ADC_ConfigChannel(&ADC1_Handler,&ADC1_ChanConf);        //ͨ������
	
    HAL_ADC_Start(&ADC1_Handler);                               //����ADC
	
    HAL_ADC_PollForConversion(&ADC1_Handler,10);                //��ѯת��
 
	return (uint16_t)HAL_ADC_GetValue(&ADC1_Handler);	        //�������һ��ADC1�������ת�����
}
//��ȡָ��ͨ����ת��ֵ��ȡtimes��,Ȼ��ƽ�� 
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
uint16_t Get_Adc_Average(uint32_t ch,uint8_t times)
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
	}
	return temp_val/times;
} 
