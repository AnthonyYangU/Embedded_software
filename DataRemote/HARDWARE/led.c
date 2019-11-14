#include "led.h"

uint8_t LED_ON = 1;
uint8_t	LED_OFF = 0;

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/* Enable the GPIO_LED Clock */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	/* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	
}

void LED_Control(uint8_t flag)
{
	if (flag == LED_ON)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
	}
	else if (flag == LED_OFF)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
	}
}
