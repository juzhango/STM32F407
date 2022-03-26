/*
 * @Author: luozw
 * @Date: 2022-01-05 13:50:59
 * @LastEditTime: 2022-03-26 16:16:56
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
uint16_t sd_size = 0;
uint8_t a = 0;

FATFS fs;					 /* FatFs文件系统对象 */
FIL fnew;					 /* 文件对象 */
FRESULT res_sd;				 /* 文件操作结果 */
UINT fnum;					 /* 文件成功读写数量 */
BYTE ReadBuffer[1024] = {0}; /* 读缓冲区 */
BYTE WriteBuffer[] =		 /* 写缓冲区*/
	"abcdefghijklmnopqrstuvwxyz1234567890~!@#$%^&*()_+";

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
/* Private functions ---------------------------------------------------------*/

void FATFS_Test()
{
	
	BYTE work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
	printf("fs size:%d,fnew size:%d\r\n",sizeof(fs),sizeof(fnew));
	printf("\r\n****** this is sd card fatfs test ******\r\n");

	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	res_sd = f_mount(&fs, "0:", 1);

	/*----------------------- 格式化测试 ---------------------------*/
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if (res_sd == FR_NO_FILESYSTEM)
	{
		printf("> SD card no fs will be format\r\n");
		/* 格式化 */
		printf("f_mkfs s\r\n");
		res_sd = f_mkfs("0", NULL, work, sizeof(work)); //格式化
		printf("f_mkfs e\r\n");
		if (res_sd == FR_OK)
		{
			printf(">SDcard format ok\r\n");
			/* 格式化后，先取消挂载 */
			res_sd = f_mount(NULL, "0:", 1);
			/* 重新挂载	*/
			res_sd = f_mount(&fs, "0:", 1);
		}
		else
		{
			printf("<< sd card format err >>\r\n");
			while (1)
				;
		}
	}
	else if (res_sd != FR_OK)
	{
		printf("!! sd card f_mount err(%d)\r\n", res_sd);
		printf("!! maybe init no ok \r\n");
		while (1)
			;
	}
	else
	{
		printf("> fs fmount ok , can be read write test\r\n");
	}
	
		/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
	printf("\r\n****** wii be wrete test... ******\r\n");	
	res_sd = f_open(&fnew, "0:wstest.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_sd == FR_OK )
	{
		printf("> open/creat FatFs rwtest.txt ok, write data\r\n");
		/* 将指定存储区内容写入到文件内 */
		res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
		if(res_sd==FR_OK)
		{
			printf("> write ok , data num: %d\n",fnum);
			printf(">  date is:\r\n%s\r\n",WriteBuffer);
		}
		else
		{
			printf("!! file wreti error(%d)\n",res_sd);
		}    
		/* 不再读写，关闭文件 */
		f_close(&fnew);
	}
	else
	{	
		printf("!! open/creat file error \r\n");
	}
	
/*------------------- 文件系统测试：读测试 ------------------------------------*/
	printf("****** will be read test... ******\r\n");
	res_sd = f_open(&fnew, "0:wstest.txt", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{
		printf("> open file ok\r\n");
		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
		if(res_sd==FR_OK)
		{
			printf("> file read ok, read byte num:%d\r\n",fnum);
			printf("> read data is:\r\n%s \r\n", ReadBuffer);	
		}
		else
		{
		  printf("!! file read error(%d)\n",res_sd);
		}		
	}
	else
	{
		printf("!! open file error\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
}

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	uint8_t key;
	uint8_t *buf;

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
	printf("reset!\r\n");
	LED_Init();
	KEY_Init();
	TIM3_Init(200); // 10Khz / 200 = 50Hz
	SRAM_Init();

	/* Add your application code here */
	mallco_dev.init(SRAMIN);
	mallco_dev.init(SRAMEX);

	BSP_SD_Init();
	//FATFS_Test();

	//while (1)
		;

	/* Infinite loop */

	while (1)
	{
		key = Key_Get();
		if (key == KEY3_EVENT_DOWN)
		{

			printf("k3d\r\n");

			a--;
			printf("a--:%d\r\n", a);
		}
		if (key == KEY2_EVENT_DOWN)
		{
			printf("k2d\r\n");

			buf = u_malloc(0, 1024); //申请内存
			memset(buf, 0xff, 1024);
			if (BSP_SD_ReadBlocks((uint32_t *)buf, 0, 2, SD_DATATIMEOUT) == 0) //读取0扇区的内容
			{
				printf("read SECTOR 0 DATA:\r\n");
				for (sd_size = 0; sd_size < 1024; sd_size++)
					printf("%x ", buf[sd_size]); //打印0扇区数据
				printf("\r\nDATA ENDED\r\n");
			}
			u_free(0, buf); //释放内存
		}
		if (key == KEY1_EVENT_DOWN)
		{
			printf("k1d\r\n");

			buf = u_malloc(0, 1024); //申请内存
			memset(buf, a, 1024);
			if (BSP_SD_WriteBlocks((uint32_t *)buf, 0, 2, SD_DATATIMEOUT) == 0) //读取0扇区的内容
			{
				printf("write SECTOR 0 DATA:\r\n");
				for (sd_size = 0; sd_size < 1024; sd_size++)
					printf("%x ", buf[sd_size]); //打印0扇区数据
				printf("\r\nDATA ENDED\r\n");
			}
			u_free(0, buf); //释放内存
		}
		if (key == KEY0_EVENT_DOWN)
		{
			printf("k0d\r\n");

			a++;
			printf("a++:%d\r\n", a);
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
