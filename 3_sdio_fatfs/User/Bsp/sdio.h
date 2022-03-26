/*
 * @Author: your name
 * @Date: 2022-03-02 21:58:01
 * @LastEditTime: 2022-03-26 12:03:25
 * @LastEditors: your name
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: \3_sdio_fatfs\User\Bsp\sdio.h
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SDIO_H_
#define _SDIO_H_

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/**
 * @brief  SD transfer state definition
 */
#define MSD_OK ((uint8_t)0x00)
#define MSD_ERROR ((uint8_t)0x01)

#define SD_TRANSFER_OK ((uint8_t)0x00)
#define SD_TRANSFER_BUSY ((uint8_t)0x01)

#define SD_DATATIMEOUT ((uint32_t)100000000)

extern HAL_SD_CardInfoTypeDef SDCardInfo;

uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
void show_sdcard_info(void);
#endif
