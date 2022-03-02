
/* Includes ------------------------------------------------------------------*/
#include "key.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
static uint8_t KEY0_ReadPin(void);
static uint8_t KEY1_ReadPin(void);
static uint8_t KEY2_ReadPin(void);
static uint8_t KEY3_ReadPin(void);
static void Get_Key_Level(void);
/* fifo */
static void Key_Put(uint8_t key_num, uint8_t event);

/* Private variables ---------------------------------------------------------*/
static KEY_COMPONENTS Key_Buf[KEY_NUM];
static KEY_FIFO_T s_tKey; // key fifo

void KEY_vCfg()
{
    /* 
        定义变量时初始化 Key_Buf[KEY0].shield bug        
     */
    Key_Buf[KEY0].shield = 0; // 不屏蔽
    Key_Buf[KEY0].count = 0;  // 持续按压计数
    Key_Buf[KEY0].is_down = 0;
    Key_Buf[KEY0].act_level = 0; // 按下时的电平
    Key_Buf[KEY0].fluent = 1;    // 使能连按
    Key_Buf[KEY0].status = S_NULL;
    Key_Buf[KEY0].READ_PIN = KEY0_ReadPin;

    Key_Buf[KEY1].shield = 0;
    Key_Buf[KEY1].count = 0;
    Key_Buf[KEY1].is_down = 0;
    Key_Buf[KEY1].act_level = 0;
    Key_Buf[KEY1].fluent = 1;
    Key_Buf[KEY1].status = S_NULL;
    Key_Buf[KEY1].READ_PIN = KEY1_ReadPin;

    Key_Buf[KEY2].shield = 0;
    Key_Buf[KEY2].count = 0;
    Key_Buf[KEY2].is_down = 0;
    Key_Buf[KEY2].act_level = 0;
    Key_Buf[KEY2].fluent = 1;
    Key_Buf[KEY2].status = S_NULL;
    Key_Buf[KEY2].READ_PIN = KEY2_ReadPin;

    Key_Buf[KEY3].shield = 0;
    Key_Buf[KEY3].count = 0;
    Key_Buf[KEY3].is_down = 0;
    Key_Buf[KEY3].act_level = 1;
    Key_Buf[KEY3].fluent = 1;
    Key_Buf[KEY3].status = S_NULL;
    Key_Buf[KEY3].READ_PIN = KEY3_ReadPin;
}

/* Private functions ---------------------------------------------------------*/

/* 初始化按键 */
void KEY_Init()
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE(); //开启GPIOA时钟
    __HAL_RCC_GPIOE_CLK_ENABLE(); //开启GPIOE时钟

    GPIO_Initure.Pin = GPIO_PIN_0;        //PA0
    GPIO_Initure.Mode = GPIO_MODE_INPUT;  //输入
    GPIO_Initure.Pull = GPIO_PULLDOWN;    //下拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH; //高速
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4; //PE2,3,4
    GPIO_Initure.Mode = GPIO_MODE_INPUT;                     //输入
    GPIO_Initure.Pull = GPIO_PULLUP;                         //上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;                    //高速
    HAL_GPIO_Init(GPIOE, &GPIO_Initure);

    KEY_vCfg();
}

/* 直接返回IO电平 */
static uint8_t KEY0_ReadPin(void)
{
    return IO_SW0;
}

static uint8_t KEY1_ReadPin(void)
{
    return IO_SW1;
}

static uint8_t KEY2_ReadPin(void)
{
    return IO_SW2;
}

static uint8_t KEY3_ReadPin(void)
{
    return IO_SW3;
}

static void Get_Key_Level(void)
{
    uint8_t i;

    for (i = 0; i < KEY_NUM; i++)
    {
        if (Key_Buf[i].shield == 1)
            continue;

        if (Key_Buf[i].READ_PIN() == Key_Buf[i].act_level)
        {
            Key_Buf[i].is_down = 1;
        }
        else
        {
            Key_Buf[i].is_down = 0;
        }
    }
}

void KeyRead_50HzStatus(void)
{
    uint8_t i;

    Get_Key_Level();

    for (i = 0; i < KEY_NUM; i++)
    {
        switch (Key_Buf[i].status)
        {
        case S_NULL:
            if (Key_Buf[i].is_down == 1)
            {
                Key_Buf[i].status = S_FILTER;
            }
            break;
        case S_FILTER:
            if (Key_Buf[i].is_down == 1) //确认和上次相同
            {
                Key_Buf[i].status = S_DOWN;
                Key_Buf[i].count = 0;
                Key_Put(i, KEY_EVENT_DOWN);
            }
            else
            {
                Key_Buf[i].status = S_NULL;
            }
            break;
        case S_DOWN:
            if (Key_Buf[i].is_down != 1) //按键释放
            {
                Key_Buf[i].status = S_NULL;
                Key_Put(i, KEY_EVENT_UP);
            }
            else if ((Key_Buf[i].is_down == 1) && (++Key_Buf[i].count >= KEY_LONG_DOWN_DELAY)) //超过KEY_LONG_DOWN_DELAY没有释放
            {
                Key_Buf[i].status = S_LONG;
                Key_Put(i, KEY_EVENT_LONG);
                Key_Buf[i].count = 0;
            }
            break;
        case S_LONG:
            if (Key_Buf[i].is_down != 1) //按键释放，端口高电平
            {
                Key_Buf[i].status = S_NULL;
                Key_Put(i, KEY_EVENT_UP);
            }
            else if ((Key_Buf[i].is_down == 1) && (++Key_Buf[i].count >= KEY_FLUENT_DOWN_DELAY)) //超过KEY_LONG_DOWN_DELAY没有释放
            {
                if (Key_Buf[i].fluent)
                {
                    Key_Put(i, KEY_EVENT_FLUENT); // 连按
                }
                Key_Buf[i].count = 0;
            }
            break;
        }
    }
}

static void Key_Put(uint8_t key_num, uint8_t event)
{
    event = (key_num + 1) * 4 + event;
    s_tKey.Buf[s_tKey.Write] = event;

    if (++s_tKey.Write >= KEY_FIFO_SIZE)
    {
        s_tKey.Write = 0;
    }
}

uint8_t Key_Get(void)
{
    uint8_t ret;

    if (s_tKey.Read == s_tKey.Write)
    {
        return KEY_EVENT_NONE;
    }
    else
    {
        ret = s_tKey.Buf[s_tKey.Read];

        if (++s_tKey.Read >= KEY_FIFO_SIZE)
        {
            s_tKey.Read = 0;
        }
        return ret;
    }
}
