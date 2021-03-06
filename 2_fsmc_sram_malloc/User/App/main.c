/*
 * @Author: luozw
 * @Date: 2022-01-05 13:50:59
 * @LastEditTime: 2022-03-10 23:55:54
 * @LastEditors: Please set LastEditors
 * @Description:
 * @FilePath: \stm32-f407-explorer\2_fsms_sram_malloc\User\App\main.c
 * @version:
 * 可以输入预定的版权声明、个性签名、空行等
 */
/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	uint8_t key;
	uint8_t *p;
	uint16_t i;
	uint16_t testWriteValue = 0;
	uint16_t testReadValue = 0;

	/* STM32F4xx HAL library initialization:
		  - Configure the Flash prefetch, instruction and Data caches
		  - Configure the Systick to generate an interrupt each 1 msec
		  - Set NVIC Group Priority to 4
		  - Global MSP (MCU Support Package) initialization
		*/
	HAL_Init();

	/* Configure the system clock to 168 MHz */
	SystemClock_Config();

	Usart1Init(115200);
	LED_Init();
	KEY_Init();
	TIM3_Init(200); // 10Khz / 200 = 50Hz
	SRAM_Init();

	/* Add your application code here */
	mallco_dev.init(SRAMIN);
	mallco_dev.init(SRAMEX);

	/* Infinite loop */

	while (1)
	{
		key = Key_Get();
		if (key == KEY3_EVENT_DOWN)
		{
			printf("k3d\r\n");
			p = (uint8_t *)u_malloc(SRAMEX, sizeof(uint8_t) * 1024);
			if (p == NULL)
			{
				printf("*p null\r\n");
			}
			else
			{
				memset(p, 0x5a, 1024);
				for (i = 0; i < 1024; i++)
				{
					if (*(p + i) != 0x5a)
					{
						printf("error:%d\r\n", i);
					}
					else
					{
						printf("ok:%d\r\n", i);
					}
				}
			}
			u_free(SRAMEX, p);
		}
		if (key == KEY2_EVENT_DOWN)
		{
			printf("k2d\r\n");
			testWriteValue = 600;
			printf("FSMC_SRAM_WriteBuffer\r\n");
			FSMC_SRAM_WriteBuffer(&testWriteValue, 0, 1);
		}
		if (key == KEY1_EVENT_DOWN)
		{
			printf("k1d\r\n");
			FSMC_SRAM_ReadBuffer(&testReadValue, 0, 1);
			printf("FSMC_SRAM_ReadBuffer:%d\r\n", testReadValue);
		}
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 168000000(CPU Clock)
 *            HCLK(Hz)                       = 168000000(CPU Clock)
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 336
 *            PLL_P                          = 2
 *            PLL_Q                          = 7(PLL48CK: USB SDIO...)
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale2 mode
 *            Flash Latency(WS)              = 3
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	  clocked below the maximum system frequency, to update the voltage scaling value
	  regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	   clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}
/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
static void Error_Handler(void)
{
	while (1)
	{
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
