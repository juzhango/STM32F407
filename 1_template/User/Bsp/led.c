/*
 * @Author: luozw
 * @Date: 2022-01-05 19:17:44
 * @LastEditTime: 2022-01-05 19:20:56
 * @LastEditors: luozw
 * @Description: 
 * @FilePath: \stm32-f407-explorer\1_template\User\Bsp\led.c
 * @version:  
 * 可以输入预定的版权声明、个性签名、空行等
 */
#include "bsp.h"
#include "led.h"
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOF_CLK_ENABLE(); //开启GPIOF时钟

    GPIO_Initure.Pin = GPIO_PIN_9 | GPIO_PIN_10; //PF9,10
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;     //推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;             //上拉
    GPIO_Initure.Speed = GPIO_SPEED_LOW;        //高速
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);

    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET);  //PF9置1，默认初始化后灯灭
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET); //PF10置1，默认初始化后灯灭
}

