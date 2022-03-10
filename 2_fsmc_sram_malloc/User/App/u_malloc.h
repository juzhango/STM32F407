/*
 * @Author: luozw
 * @Date: 2022-01-07 20:57:01
 * @LastEditTime: 2022-03-10 23:15:58
 * @LastEditors: Please set LastEditors
 * @Description:
 * @FilePath: \stm32-f407-explorer\2_fsms_sram_malloc\User\App\u_malloc.h
 * @version:
 * 可以输入预定的版权声明、个性签名、空行等
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __U_MALLOC_H
#define __U_MALLOC_H
/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Exported types ------------------------------------------------------------*/
#if 0
#define SRAMIN 0
#define SRAMEX 1
#define SRAMBANK 2
#else
typedef enum _MEM_LIST
{
	SRAMIN = 0,
	SRAMEX,
	SRAMBANK,
} MEM_LIST;
#endif

struct _m_mallco_dev
{
	void (*init)(uint8_t);		 //初始化
	uint8_t (*perused)(uint8_t); //内存使用率
	uint8_t *membase[SRAMBANK];	 //内存池 管理SRAMBANK个区域的内存
	uint16_t *memmap[SRAMBANK];	 //内存管理状态表
	uint8_t memrdy[SRAMBANK];	 //内存管理是否就绪
};

extern struct _m_mallco_dev mallco_dev; //在mallco.c里面定义

/* Exported constants --------------------------------------------------------*/
/* MEM byte size*/
#define MEM1_MAX_SIZE (50 * 1024)  // 内部内粗
#define MEM2_MAX_SIZE (400 * 1024) // 外部扩展内存

/* MEM block size*/
#define MEM1_BLOCK_SIZE 32
#define MEM2_BLOCK_SIZE 32

/* MEM table size */
#define MEM1_ALLOC_TABLE_SIZE (MEM1_MAX_SIZE / MEM1_BLOCK_SIZE)
#define MEM2_ALLOC_TABLE_SIZE (MEM2_MAX_SIZE / MEM2_BLOCK_SIZE)
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
void u_free(uint8_t memx, void *ptr);
void *u_malloc(uint8_t memx, uint32_t size);
void *u_realloc(uint8_t memx, void *ptr, uint32_t size);

#endif
