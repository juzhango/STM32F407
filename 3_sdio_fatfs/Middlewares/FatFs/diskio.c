/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"		/* Obtains integer types */
#include "diskio.h" /* Declarations of disk functions */

#include "stdio.h"
#include "sdio.h"
#include "u_malloc.h"

/* Definitions of physical drive number for each drive */
// #define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
// #define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
// #define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */
#define DEV_SDCARD 0

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv)
	{
	case DEV_SDCARD:
		stat = RES_OK;
		break;
	default:
		stat = RES_ERROR;
		break;
	}
	return stat;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(
	BYTE pdrv /* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;

	switch (pdrv)
	{
	case DEV_SDCARD:
		while (BSP_SD_Init() != MSD_OK)
		{
			printf("sd init ko!\r\n");
			HAL_Delay(1);
		}
		stat = RES_OK;
		break;
	default:
		stat = RES_ERROR;
		break;
	}
	return stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
	BYTE pdrv,	  /* Physical drive nmuber to identify the drive */
	BYTE *buff,	  /* Data buffer to store read data */
	LBA_t sector, /* Start sector in LBA */
	UINT count	  /* Number of sectors to read */
)
{
	DRESULT stat;

	BYTE sdRES;
	switch (pdrv)
	{
	case DEV_SDCARD:
		sdRES = BSP_SD_ReadBlocks((uint32_t *)buff, (uint32_t)sector, (uint32_t)count, SD_DATATIMEOUT);
		while(sdRES)
		{
			HAL_Delay(1);
			BSP_SD_Init();
			sdRES = BSP_SD_ReadBlocks((uint32_t *)buff, (uint32_t)sector, (uint32_t)count, SD_DATATIMEOUT);
		}
		stat = RES_OK;
		break;
	default:
		stat = RES_ERROR;
		break;
	}

	return stat;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(
	BYTE pdrv,		  /* Physical drive nmuber to identify the drive */
	const BYTE *buff, /* Data to be written */
	LBA_t sector,	  /* Start sector in LBA */
	UINT count		  /* Number of sectors to write */
)
{
	DRESULT stat;
	BYTE sdRES;
	switch (pdrv)
	{
	case DEV_SDCARD:
		sdRES = BSP_SD_WriteBlocks((uint32_t *)buff, (uint32_t)sector, (uint32_t)count, SD_DATATIMEOUT);
		while(sdRES)
		{
			HAL_Delay(1);
			BSP_SD_Init();
			sdRES = BSP_SD_WriteBlocks((uint32_t *)buff, (uint32_t)sector, (uint32_t)count, SD_DATATIMEOUT);
		}
		stat = RES_OK;
		break;
	default:
		stat = RES_ERROR;
		break;
	}

	return stat;
}

#endif

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(
	BYTE pdrv, /* Physical drive nmuber (0..) */
	BYTE cmd,  /* Control code */
	void *buff /* Buffer to send/receive control data */
)
{
	DRESULT stat;
	
	if(pdrv == DEV_SDCARD)
	{
		switch(cmd)
	    {
		    case CTRL_SYNC:
				stat = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
				*(DWORD*)buff = 512; 	// sector即扇区，对于SD卡而言，扇区是最小的可编程单元一般固定为512Byte
		        stat = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
				*(WORD*)buff = SDCardInfo.LogBlockSize;	// 对于V2.0版本以后的SD卡是固定为512Byte（即一个sector）大小。
		        stat = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = SDCardInfo.LogBlockNbr;
		        stat = RES_OK;
		        break;
		    default:
		        stat = RES_PARERR;
		        break;
	    }
	}
	return stat;
}

DWORD get_fattime (void)
{
	return 0;
}
/* Allocate memory block */
void* ff_memalloc (UINT msize)
{
	return (void *)u_malloc(SRAMEX, (uint32_t) msize);
}

/* Free memory block */		
void ff_memfree (void* mblock)
{
	u_free(SRAMEX, mblock);
}		

