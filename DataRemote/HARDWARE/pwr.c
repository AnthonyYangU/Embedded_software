#include "pwr.h"


void PWRA_Init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
		/* Configure the GPIO_PWRAEnable pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
	//GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  //GPIO_InitStruct.Pull = GPIO_PULLUP;
	
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

void PWRB_Init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Configure the GPIO_PWRBEnable pin */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;

  //GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	
}

void PWRC_Init()
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	/* Configure the GPIO_PWRBEnable pin */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_5;
	//GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  //GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
}
