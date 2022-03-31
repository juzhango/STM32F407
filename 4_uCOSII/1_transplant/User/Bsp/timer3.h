/*
 * @Author: luozw
 * @Date: 2022-01-05 19:58:05
 * @LastEditTime: 2022-01-05 20:05:45
 * @LastEditors: luozw
 * @Description: 
 * @FilePath: \stm32-f407-explorer\1_template\User\Bsp\timer3.h
 * @version:  
 * 可以输入预定的版权声明、个性签名、空行等
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef TIMER3_H
#define TIMER3_H

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define TIM3_CLK			10000		


/* User can use this section to tailor TIMx instance used and associated 
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM3
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM3_CLK_ENABLE

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM3_IRQn
#define TIMx_IRQHandler                TIM3_IRQHandler



/* Exported macro ------------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */
void TIM3_Init(uint16_t arr);

#endif

