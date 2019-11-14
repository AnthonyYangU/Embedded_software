#include "spi.h"

SPI_HandleTypeDef SPI_Handler;//sdø®
SPI_HandleTypeDef SPI_Handler2;//Õ”¬›“«

void SPI_Init(void)
{
   	SPI_Handler.Instance=SPI2;                                     
    SPI_Handler.Init.Mode=SPI_MODE_MASTER;                         
    SPI_Handler.Init.Direction=SPI_DIRECTION_2LINES;            
    SPI_Handler.Init.DataSize=SPI_DATASIZE_8BIT;               
    SPI_Handler.Init.CLKPolarity=SPI_POLARITY_HIGH;                 
    SPI_Handler.Init.CLKPhase=SPI_PHASE_2EDGE;                     
    SPI_Handler.Init.NSS=SPI_NSS_SOFT;                          
    SPI_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256; 
    SPI_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;                   
    SPI_Handler.Init.TIMode=SPI_TIMODE_DISABLE;                   
    SPI_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;     
    SPI_Handler.Init.CRCPolynomial=7;                               

    HAL_SPI_Init(&SPI_Handler);                                     

    __HAL_SPI_ENABLE(&SPI_Handler);                               

   SPI_ReadWriteByte(0Xff);                                         
}

void SPI_Init2(void)
{
   	SPI_Handler2.Instance=SPI1;                                     
    SPI_Handler2.Init.Mode=SPI_MODE_MASTER;                         
    SPI_Handler2.Init.Direction=SPI_DIRECTION_2LINES;            
    SPI_Handler2.Init.DataSize=SPI_DATASIZE_8BIT;               
    SPI_Handler2.Init.CLKPolarity=SPI_POLARITY_HIGH;                 
    SPI_Handler2.Init.CLKPhase=SPI_PHASE_2EDGE;                     
    SPI_Handler2.Init.NSS=SPI_NSS_SOFT;                          
    SPI_Handler2.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_256; 
    SPI_Handler2.Init.FirstBit=SPI_FIRSTBIT_MSB;                   
    SPI_Handler2.Init.TIMode=SPI_TIMODE_DISABLE;                   
    SPI_Handler2.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;     
    SPI_Handler2.Init.CRCPolynomial=0;                               

    HAL_SPI_Init(&SPI_Handler2);                                     

    __HAL_SPI_ENABLE(&SPI_Handler2);                               
		
    //SPI_WriteByte(0Xff);                                         
}


void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
		GPIO_InitTypeDef GPIO_Initure;
		if(hspi == &SPI_Handler)
		{
			

			__HAL_RCC_GPIOB_CLK_ENABLE();                            
			__HAL_RCC_SPI2_CLK_ENABLE();                            

			GPIO_Initure.Pin=GPIO_PIN_15|GPIO_PIN_13|GPIO_PIN_14;       
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;                      
			GPIO_Initure.Pull=GPIO_PULLUP;                          
			GPIO_Initure.Speed=GPIO_SPEED_FAST;                     
			GPIO_Initure.Alternate=GPIO_AF0_SPI2;                    

			HAL_GPIO_Init(GPIOB,&GPIO_Initure);                     

			GPIO_Initure.Pin=GPIO_PIN_12;                            
			GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;                  
			GPIO_Initure.Pull=GPIO_PULLUP;                          
			GPIO_Initure.Speed=GPIO_SPEED_FAST;                      
			HAL_GPIO_Init(GPIOB,&GPIO_Initure); 
		}else
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
			__HAL_RCC_GPIOA_CLK_ENABLE();  			
			__HAL_RCC_SPI1_CLK_ENABLE();                            

			GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;       
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;                      
			GPIO_Initure.Pull=GPIO_PULLUP;                          
			GPIO_Initure.Speed=GPIO_SPEED_FAST;                     
			GPIO_Initure.Alternate=GPIO_AF0_SPI1;                    

			HAL_GPIO_Init(GPIOB,&GPIO_Initure);                     

			GPIO_Initure.Pin=GPIO_PIN_4;                            
			GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;                  
			GPIO_Initure.Pull=GPIO_PULLUP;                          
			GPIO_Initure.Speed=GPIO_SPEED_FAST;                      
			HAL_GPIO_Init(GPIOA,&GPIO_Initure); 			
		}
                     
}

//*******”√”⁄SDø®
void SPI_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    __HAL_SPI_DISABLE(&SPI_Handler);
    SPI_Handler.Instance->CR1&=0XFFC7;
    SPI_Handler.Instance->CR1|=SPI_BaudRatePrescaler;
    __HAL_SPI_ENABLE(&SPI_Handler);
}

u8 SPI_ReadWriteByte(u8 TxData)
{
    u8 Rxdata=0;
    HAL_SPI_TransmitReceive(&SPI_Handler,&TxData,&Rxdata,1, 1000);
		return Rxdata;
}

//*****”√”⁄Õ”¬›“«
/*
typedef enum 
{
  HAL_OK       = 0x00,
  HAL_ERROR    = 0x01,
  HAL_BUSY     = 0x02,
  HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;
*/
void SPI_WriteByte(u8 TxData)
{
	HAL_SPI_Transmit(&SPI_Handler2,&TxData,1,1000);
	//printf("Status is %d",status);
}

u8 SPI_ReadByte(void)
{
	u8 rxData = 0;
	HAL_SPI_Receive(&SPI_Handler2,&rxData,1,1000);
	//printf("rxData is %d\n",rxData);
	return rxData;
}


