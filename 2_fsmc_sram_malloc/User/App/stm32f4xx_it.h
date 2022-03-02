/*
 * @Author: luozw
 * @Date: 2022-01-05 13:50:59
 * @LastEditTime: 2022-01-05 14:29:50
 * @LastEditors: luozw
 * @Description:
 * @FilePath: \template\User\App\stm32f4xx_it.h
 * @version:
 * 可以输入预定的版权声明、个性签名、空行等
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
extern "C"
{
#endif

	/* Includes ------------------------------------------------------------------*/
	/* Exported types ------------------------------------------------------------*/
	/* Exported constants --------------------------------------------------------*/
	/* Exported macro ------------------------------------------------------------*/
	/* Exported functions ------------------------------------------------------- */

	void NMI_Handler(void);
	void HardFault_Handler(void);
	void MemManage_Handler(void);
	void BusFault_Handler(void);
	void UsageFault_Handler(void);
	void SVC_Handler(void);
	void DebugMon_Handler(void);
	void PendSV_Handler(void);
	void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
