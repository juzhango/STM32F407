/*
 * @Author: luozw
 * @Date: 2022-01-06 11:10:23
 * @LastEditTime: 2022-03-10 23:56:01
 * @LastEditors: Please set LastEditors
 * @Description:
 * @FilePath: \stm32-f407-explorer\2_fsms_sram_malloc\User\Bsp\sram.c
 * @version:
 * 可以输入预定的版权声明、个性签名、空行等
 */

/*
bank1:0x60000000~0x6fffffff
4 * 64M
bank1_1:0x60000000~0x63ffffff
bank1_2:0x64000000~0x67ffffff
bank1_3:0x68000000~0x6bffffff
bank1_4:0x6c000000~0x63ffffff
*/

/* Includes ------------------------------------------------------------------*/
#include "sram.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

SRAM_HandleTypeDef hsram;
FSMC_NORSRAM_TimingTypeDef SRAM_Timing;

/* Private function prototypes -----------------------------------------------*/
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

void SRAM_Init()
{
	/*##-1- Configure the SRAM device ##########################################*/
	/* SRAM device configuration */
	hsram.Instance = FSMC_NORSRAM_DEVICE;
	hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

	SRAM_Timing.AddressSetupTime = 0; //地址建立时间（ADDSET）为1个HCLK 1/36M=27ns
	SRAM_Timing.AddressHoldTime = 0;  //地址保持时间（ADDHLD）模式A未用到
	SRAM_Timing.DataSetupTime = 6;	  //数据保持时间（DATAST）为9个HCLK 6*9=54ns
	SRAM_Timing.BusTurnAroundDuration = 0;
	SRAM_Timing.CLKDivision = 0;
	SRAM_Timing.DataLatency = 0;
	SRAM_Timing.AccessMode = FSMC_ACCESS_MODE_A;

	hsram.Init.NSBank = FSMC_NORSRAM_BANK3;
	hsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	hsram.Init.MemoryDataWidth = SRAM_MEMORY_WIDTH;
	hsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hsram.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	hsram.Init.ContinuousClock = FSMC_CONTINUOUS_CLOCK_SYNC_ASYNC; // ??

	/* Initialize the SRAM controller */
	if (HAL_SRAM_Init(&hsram, &SRAM_Timing, &SRAM_Timing) != HAL_OK)
	{
		/* Initialization Error */
		Error_Handler();
	}
}

//在指定地址(WriteAddr+Bank1_SRAM3_ADDR)开始,连续写入n个字节.
// pBuffer:字节指针
// WriteAddr:要写入的地址
// n:要写入的字节数
void FSMC_SRAM_WriteBuffer(uint16_t *pBuffer, uint32_t WriteAddr, uint32_t n)
{
	for (; n != 0; n--)
	{
		*(volatile uint16_t *)(SRAM_BANK_ADDR + WriteAddr) = *pBuffer;
		WriteAddr++;
		pBuffer++;
	}
}

//在指定地址((WriteAddr+Bank1_SRAM3_ADDR))开始,连续读出n个字节.
// pBuffer:字节指针
// ReadAddr:要读出的起始地址
// n:要写入的字节数
void FSMC_SRAM_ReadBuffer(uint16_t *pBuffer, uint32_t ReadAddr, uint32_t n)
{
	for (; n != 0; n--)
	{
		*pBuffer++ = *(volatile uint16_t *)(SRAM_BANK_ADDR + ReadAddr);
		ReadAddr++;
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
