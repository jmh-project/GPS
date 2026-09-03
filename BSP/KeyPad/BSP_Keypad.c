/******************************************************************************
*    	File name	: BSP_Keypad.c
*		Company		:
*		Version		:
*		Data		:
*		Author		:
*		Contact		:
******************************************************************************/



/******************************************************************************
							Include File
******************************************************************************/
#include "BSP_Keypad.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "Debug_Uart.h"
#include "TEECO_Define.h"

void Delay(__IO uint32_t nTime);

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define     LEFT_KEY    1
#define     UP_KEY      2
#define     DN_KEY      3
#define     RIGHT_KEY   4
#define     ENTER_KEY   5
#define     BACK_KEY    6
#define     SAVE_KEY    7

/******************************************************************************
							Private & Local Variables
******************************************************************************/
volatile int g_KeyPad_Repeat;

/******************************************************************************
							Function Prototype
******************************************************************************/
int BSP_Keyin(void)
{
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)==0)
    {
        return(1);
    }
    else if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)==0)
    {
        return(2);
    }
    else if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)
    {
        return(3);
    }
    else if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)==0)
    {
        return(4);
    }
    else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)==0)
    {
        return(5);
    }
    else if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_9)==0)
    {
        return(6);
    }
    else if(GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_10)==0)
    {
        return(7);
    }
    else
    {
        return(0);
    }
}

void KeyPad_Repeat(int s)
{
    if(s==ON) g_KeyPad_Repeat = ON;
    else g_KeyPad_Repeat = OFF;
}   

