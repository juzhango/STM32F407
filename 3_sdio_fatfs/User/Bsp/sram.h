/*
 * @Author: luozw
 * @Date: 2022-01-06 11:10:28
 * @LastEditTime: 2022-01-07 20:38:18
 * @LastEditors: luozw
 * @Description: 
 * @FilePath: \stm32-f407-explorer\2_fsms_sram_malloc\User\Bsp\sram.h
 * @version:  
 * 可以输入预定的版权声明、个性签名、空行等
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SRAM_H
#define __SRAM_H

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"


/* Exported types ------------------------------------------------------------*/
//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported constants --------------------------------------------------------*/


//#define SRAM_BANK_ADDR                 ((uint32_t)0x68000000)

/* #define SRAM_MEMORY_WIDTH            FSMC_NORSRAM_MEM_BUS_WIDTH_8  */
#define SRAM_MEMORY_WIDTH               FSMC_NORSRAM_MEM_BUS_WIDTH_16
/* #define SRAM_MEMORY_WIDTH            FSMC_NORSRAM_MEM_BUS_WIDTH_32 */


//#define SRAM_TIMEOUT     ((uint32_t)0xFFFF) 

void SRAM_Init(void);
//void FSMC_SRAM_WriteBuffer(uint16_t *pBuffer,uint32_t WriteAddr,uint32_t n);
//void FSMC_SRAM_ReadBuffer(uint16_t *pBuffer,uint32_t ReadAddr,uint32_t n);

#endif /* __SRAM_H */
