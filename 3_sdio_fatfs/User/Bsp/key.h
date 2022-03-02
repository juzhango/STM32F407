
#ifndef KEY_H
#define KEY_H
#include "bsp.h"

/* 
k0      PE4    按下低电平
k1      PE3    按下低电平
k2      PE2    按下低电平
k3      PA0    按下高电平
*/
#define KEY_FIFO_SIZE 10
#define KEY_EVENT_NUM 4

#define KEY_LONG_DOWN_DELAY 50  // 1HZ
#define KEY_FLUENT_DOWN_DELAY 5 // 10HZ
/* Exported types ------------------------------------------------------------*/

typedef struct
{
    uint8_t Buf[KEY_FIFO_SIZE]; /* 键值缓冲区 */
    uint8_t Read;               /* 缓冲区读指针1 */
    uint8_t Write;              /* 缓冲区写指针 */
    uint8_t Read2;              /* 缓冲区读指针2 */
} KEY_FIFO_T;

typedef enum _KEY_EVENT_LIST
{
    KEY_EVENT_NONE = 0,

    KEY_EVENT_UP,
    KEY_EVENT_DOWN,
    KEY_EVENT_LONG,
    KEY_EVENT_FLUENT,

    KEY0_EVENT_UP,
    KEY0_EVENT_DOWN,
    KEY0_EVENT_LONG,
    KEY0_EVENT_FLUENT,

    KEY1_EVENT_UP,
    KEY1_EVENT_DOWN,
    KEY1_EVENT_LONG,
    KEY1_EVENT_FLUENT,

    KEY2_EVENT_UP,
    KEY2_EVENT_DOWN,
    KEY2_EVENT_LONG,
    KEY2_EVENT_FLUENT,

    KEY3_EVENT_UP,
    KEY3_EVENT_DOWN,
    KEY3_EVENT_LONG,
    KEY3_EVENT_FLUENT,
} KEY_EVENT_LIST;

typedef enum _KEY_STATUS_LIST
{
    S_NULL = 0x00, // 无动作
    S_FILTER,
    S_UP,
    S_DOWN,
    S_LONG,
} KEY_STATUS_LIST;

typedef enum _KEY_LIST
{
    KEY0,
    KEY1,
    KEY2,
    KEY3,
    KEY_NUM, // 按键总数
} KEY_LIST;

typedef struct _KEY_COMPONENTS
{
    uint8_t shield;            //按键屏蔽1:屏蔽，0:不屏蔽
    uint8_t count;             //按键长按计数
    uint8_t fluent;            //连按 1:使能，0:不使能
    uint8_t is_down;           //是否按下，按下1，抬起0
    uint8_t act_level;         //按下时IO电平，高电平1，低电平0
    uint8_t status;            //按键状态
    uint8_t (*READ_PIN)(void); //读IO电平函数
} KEY_COMPONENTS;

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

#define IO_SW0 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) //KEY0按键PE4
#define IO_SW1 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) //KEY1按键PE3
#define IO_SW2 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2) //KEY2按键PE2
#define IO_SW3 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) //WKUP按键PA0

/* Exported functions ------------------------------------------------------- */

void KEY_Init(void);
void KeyRead_50HzStatus(void);
uint8_t Key_Get(void);

#endif
