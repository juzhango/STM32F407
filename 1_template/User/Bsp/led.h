/*
 * @Author: luozw
 * @Date: 2022-01-05 19:17:49
 * @LastEditTime: 2022-01-05 19:20:26
 * @LastEditors: luozw
 * @Description: 
 * @FilePath: \stm32-f407-explorer\1_template\User\Bsp\led.h
 * @version:  
 * 可以输入预定的版权声明、个性签名、空行等
 */
#ifndef _LED_H
#define _LED_H
#include "bsp.h"

#define LED0 PFout(9)  //LED0
#define LED1 PFout(10) //LED1

#define LED0_ON()			LED0=0
#define LED0_OFF()		LED0=1
#define LED0_TOG()		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9)


#define LED1_ON()			LED1=0
#define LED1_OFF()		LED1=1
#define LED1_TOG()		HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_10)
void LED_Init(void);

#endif
