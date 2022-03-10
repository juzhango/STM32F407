/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SDIO_H_
#define _SDIO_H_


/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/** 
  * @brief  SD transfer state definition  
  */     
#define   MSD_OK                        ((uint8_t)0x00)
#define   MSD_ERROR                     ((uint8_t)0x01)

#define   SD_TRANSFER_OK                ((uint8_t)0x00)
#define   SD_TRANSFER_BUSY              ((uint8_t)0x01)
   
#define SD_DATATIMEOUT           ((uint32_t)100000000)
   
uint8_t BSP_SD_Init(void);
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout);
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr);
void show_sdcard_info(void);
#endif

