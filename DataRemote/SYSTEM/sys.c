#include "sys.h"

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = HSI
  *            SYSCLK(Hz)                     = 32000000
  *            HCLK(Hz)                       = 32000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            Flash Latency(WS)              = 1
  *            Main regulator output voltage  = Scale1 mode
  * @retval None
  */

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	
	__HAL_RCC_PWR_CLK_ENABLE();  
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	
  /* Enable HSI Oscillator */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSEState=RCC_HSE_OFF;
	RCC_OscInitStruct.HSIState=RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
	RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
	RCC_OscInitStruct.HSICalibrationValue = 0x10;	
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);  
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; 
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1; 
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1; 
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; 
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
	 
  
}

//void SystemClock_Config(void)
//{
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  
//  /* Enable MSI Oscillator */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
//  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
//  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
//  RCC_OscInitStruct.MSICalibrationValue=0x00;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
//  {
//    /* Initialization Error */
//    while(1); 
//  }
//  
//  /* Select MSI as system clock source and configure the HCLK, PCLK1 and PCLK2 
//     clocks dividers */
//  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0)!= HAL_OK)
//  {
//    /* Initialization Error */
//    while(1); 
//  }
//  /* Enable Power Control clock */
//  __HAL_RCC_PWR_CLK_ENABLE();
//  
//  /* The voltage scaling allows optimizing the power consumption when the device is 
//     clocked below the maximum system frequency, to update the voltage scaling value 
//     regarding system frequency refer to product datasheet.  */
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
//  
//  /* Disable Power Control clock */
//  __HAL_RCC_PWR_CLK_DISABLE();
//  
//}

