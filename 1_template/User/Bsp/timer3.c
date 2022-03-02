/*
 * @Author: luozw
 * @Date: 2022-01-05 19:57:56
 * @LastEditTime: 2022-01-05 20:00:50
 * @LastEditors: luozw
 * @Description: 
 * @FilePath: \stm32-f407-explorer\1_template\User\Bsp\timer3.c
 * @version:  
 * 可以输入预定的版权声明、个性签名、空行等
 */
 /* Includes ------------------------------------------------------------------*/
#include "timer3.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/



/* Timer handler declaration */
TIM_HandleTypeDef    TimHandle;

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/
/* SET TIM3 CLK = 10KHz */
void TIM3_Init(uint16_t arr)
{
	/*##-1- Configure the TIM peripheral #######################################*/ 
  /* -----------------------------------------------------------------------
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
    To get TIM3 counter clock at [TIM3_CLK] Hz, the Prescaler is computed as following:
    Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    Prescaler = ((SystemCoreClock /2) /[TIM3_CLK]) - 1
       
    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to update SystemCoreClock 
     variable value. Otherwise, any configuration based on this variable will be incorrect.
     This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency  
  ----------------------------------------------------------------------- */  
  
  /* Compute the prescaler value to have TIM3 counter clock equal to 10 KHz */
  uint32_t uwPrescalerValue = (uint32_t) ((SystemCoreClock /2) / TIM3_CLK) - 1;
  
  /* Set TIMx instance */
  TimHandle.Instance = TIMx;
   
  /* Initialize TIM3 peripheral as follow:
       + Period = [arr] - 1
       + Prescaler = ((SystemCoreClock/2)/[TIM3_CLK]) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  TimHandle.Init.Period = arr - 1;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if(HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
  /* Start Channel1 */
  if(HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	

	
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  while(1)
  {
  }
}
