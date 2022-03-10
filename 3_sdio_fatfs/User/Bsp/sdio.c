
/* Includes ------------------------------------------------------------------*/
#include "sdio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
SD_HandleTypeDef uSdHandle;
HAL_SD_CardInfoTypeDef SDCardInfo; 
/**
 * @brief  Initializes the SD card device.
 * @retval SD status.
 */
uint8_t BSP_SD_Init(void)
{
    uint8_t SD_state = MSD_OK;

    /* uSD device interface configuration */
    uSdHandle.Instance = SDIO;

    uSdHandle.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;                      
    uSdHandle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;               
    uSdHandle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;         
    uSdHandle.Init.BusWide = SDIO_BUS_WIDE_1B;                              
    uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE; 
    uSdHandle.Init.ClockDiv = SDIO_TRANSFER_CLK_DIV;

    if (HAL_SD_Init(&uSdHandle) != HAL_OK)
    {
        SD_state = MSD_ERROR;
    }

    /* Configure SD Bus width */
    if (SD_state == MSD_OK)
    {
        /* Enable wide operation */
        if (HAL_SD_ConfigWideBusOperation(&uSdHandle, SDIO_BUS_WIDE_4B) != HAL_OK)
        {
            SD_state = MSD_ERROR;
        }
        else
        {
            SD_state = MSD_OK;
        }
    }

    return SD_state;
}


void show_sdcard_info(void)
{
	uint64_t CardCap;	
	HAL_SD_CardCIDTypeDef SDCard_CID;

	HAL_SD_GetCardCID(&uSdHandle,&SDCard_CID);	
	HAL_SD_GetCardInfo(&uSdHandle,&SDCardInfo);					
	switch(SDCardInfo.CardType)
	{
		case CARD_SDSC:
		{
			if(SDCardInfo.CardVersion == CARD_V1_X)
				printf("Card Type:SDSC V1\r\n");
			else if(SDCardInfo.CardVersion == CARD_V2_X)
				printf("Card Type:SDSC V2\r\n");
		}
		break;
		case CARD_SDHC_SDXC:printf("Card Type:SDHC\r\n");break;
	}	
	CardCap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);
  	printf("Card ManufacturerID:%d\r\n",SDCard_CID.ManufacturerID);					
 	printf("Card RCA:%d\r\n",SDCardInfo.RelCardAdd);								
	printf("LogBlockNbr:%d \r\n",(uint32_t)(SDCardInfo.LogBlockNbr));				
	printf("LogBlockSize:%d \r\n",(uint32_t)(SDCardInfo.LogBlockSize));				
	printf("Card Capacity:%d MB\r\n",(uint32_t)(CardCap>>20));						
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.BlockSize);	
	
	printf("===========================\r\n\r\n");	
	printf("SDCardInfo.BlockNbr:%d\r\n",SDCardInfo.BlockNbr);	
	printf("SDCardInfo.BlockSize:%d\r\n",SDCardInfo.BlockSize);	
	printf("SDCardInfo.CardType:%d\r\n",SDCardInfo.CardType);	
	printf("SDCardInfo.CardVersion:%d\r\n",SDCardInfo.CardVersion);	
	printf("SDCardInfo.Class:%d\r\n",SDCardInfo.Class);	
	printf("SDCardInfo.LogBlockNbr:%d\r\n",SDCardInfo.LogBlockNbr);	
	printf("SDCardInfo.LogBlockSize:%d\r\n",SDCardInfo.LogBlockSize);
	printf("SDCardInfo.RelCardAdd:%d\r\n",SDCardInfo.RelCardAdd);
	printf("===========================\r\n\r\n");	
}



/**
 * @brief  Reads block(s) from a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @param  Timeout: Timeout for read operation
 * @retval SD status
 */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
	HAL_StatusTypeDef ret;
	uint16_t	timeout=0xff;
	
	DISABLE_INT();
	ret = HAL_SD_ReadBlocks(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks, Timeout);
	while(HAL_SD_GetCardState(&uSdHandle) != HAL_SD_CARD_TRANSFER)
    {
		if(timeout-- == 0)
		{	
			return SD_TRANSFER_BUSY;
		}
    }
	ENABLE_INT();
	
    if ( ret != HAL_OK)
    {
        return SD_TRANSFER_BUSY;
    }
    else
    {
        return SD_TRANSFER_OK;
    }
}

/**
 * @brief  Writes block(s) to a specified address in an SD card, in polling mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @param  Timeout: Timeout for write operation
 * @retval SD status
 */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks, uint32_t Timeout)
{
	uint16_t	timeout=0xffff;
	HAL_StatusTypeDef ret = HAL_OK;
	DISABLE_INT();
	
	ret = HAL_SD_WriteBlocks(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks, Timeout) ;
	
	while(HAL_SD_GetCardState(&uSdHandle) != HAL_SD_CARD_TRANSFER)
    {
		if(timeout-- == 0)
		{	
			return SD_TRANSFER_BUSY;
		}
    }

	ENABLE_INT();

	if ( ret != HAL_OK)
    {
        return SD_TRANSFER_BUSY;
    }
    else
    {
        return SD_TRANSFER_OK;
    }
}



/**
 * @brief  Erases the specified memory area of the given SD card.
 * @param  StartAddr: Start byte address
 * @param  EndAddr: End byte address
 * @retval SD status
 */
uint8_t BSP_SD_Erase(uint32_t StartAddr, uint32_t EndAddr)
{
    if (HAL_SD_Erase(&uSdHandle, StartAddr, EndAddr) != HAL_OK)
    {
        return MSD_ERROR;
    }
    else
    {
        return MSD_OK;
    }
}


/**
 * @brief  Reads block(s) from a specified address in an SD card, in DMA mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  ReadAddr: Address from where data is to be read
 * @param  NumOfBlocks: Number of SD blocks to read
 * @retval SD status
 */
//uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData, uint32_t ReadAddr, uint32_t NumOfBlocks)
//{
//    /* Read block(s) in DMA transfer mode */
//    if (HAL_SD_ReadBlocks_DMA(&uSdHandle, (uint8_t *)pData, ReadAddr, NumOfBlocks) != HAL_OK)
//    {
//        return MSD_ERROR;
//    }
//    else
//    {
//        return MSD_OK;
//    }
//}

/**
 * @brief  Writes block(s) to a specified address in an SD card, in DMA mode.
 * @param  pData: Pointer to the buffer that will contain the data to transmit
 * @param  WriteAddr: Address from where data is to be written
 * @param  NumOfBlocks: Number of SD blocks to write
 * @retval SD status
 */
//uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData, uint32_t WriteAddr, uint32_t NumOfBlocks)
//{
//    /* Write block(s) in DMA transfer mode */
//	uint16_t	timeout=0xff;
//	HAL_StatusTypeDef ret = HAL_OK;
//	
//	ret = HAL_SD_WriteBlocks_DMA(&uSdHandle, (uint8_t *)pData, WriteAddr, NumOfBlocks);
//	
//	while(HAL_SD_GetCardState(&uSdHandle) != HAL_SD_CARD_TRANSFER)
//    {
//		printf("dma w status:%d\r\n",HAL_SD_GetCardState(&uSdHandle));
//		if(timeout-- == 0)
//		{	
//			return SD_TRANSFER_BUSY;
//		}
//    }
//	
//    if ( ret != HAL_OK)
//    {
//        return SD_TRANSFER_BUSY;
//    }
//    else
//    {
//        return SD_TRANSFER_OK;
//    }
//}


