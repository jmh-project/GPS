/******************************************************************************
							Include File
******************************************************************************/
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "Debug_Uart.h"
#include "BSP_Encoder.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
/******************************************************************************
		설명 : 		터치 인터럽트 함수이다.
		전달인자 : 	없음 
		리턴값 :	터치값을 읽어서 전역변수에 저장한다. 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 
******************************************************************************/

U32	Encoder_Puls;
void EXTI9_5_IRQHandler(void)
{
	static int	nApulsCnt=0;
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		if(!(GPIOC->IDR & GPIO_Pin_13))
		{
			Uart_SendByte2('.');	
		}	
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}
void EXTI15_10_IRQHandler(void)
{
	static int	nApulsCnt=0;
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
		if(!(GPIOC->IDR & GPIO_Pin_7))
		{
			Uart_SendByte2('+');	
		}	
		EXTI_ClearITPendingBit(EXTI_Line13);
	}
}
/******************************************************************************
		설명 : 		터치 인터럽트 설정 함수이다.
		전달인자 : 	없음 
		리턴값 :	없음 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
void EXTILine7_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void EXTILine13_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/******************************************************************************
		설명 : 		ENCODER 포트 초기화 
		전달인자 : 	없음 
		리턴값 :	없음 
		수정이력 :	없음 
		작성일시 :	12/06/12
		Note : 
******************************************************************************/
void BSP_Encoder_Portinit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);				
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);				

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_13;	//A상 B상 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	//Z상
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

	EXTILine7_Config();
	EXTILine13_Config();
}