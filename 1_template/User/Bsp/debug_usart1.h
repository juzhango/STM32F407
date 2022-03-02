/*
 * @Author: luozw
 * @Date: 2022-01-05 14:25:38
 * @LastEditTime: 2022-01-05 14:40:38
 * @LastEditors: luozw
 * @Description: 
 * @FilePath: \template\User\Bsp\debug_usart1.h
 * @version:  
 * 可以输入预定的版权声明、个性签名、空行等
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEBUG_USART1_H
#define DEBUG_USART1_H
#include "stdint.h"

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE();
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA  
#define USARTx_TX_AF                     GPIO_AF7_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA 
#define USARTx_RX_AF                     GPIO_AF7_USART1
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void Usart1Init(uint32_t baudrate);

#endif
