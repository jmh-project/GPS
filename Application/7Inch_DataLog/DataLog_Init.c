/******************************************************************************
*       File name	: DataLog_Init.c 
*		Description 	: 7인치 데이터로거 하드웨어 구조체 초기화
*		Company	: ziontech co.,
*		Version		: 0.10 (2015년 4월23일 Start)
*		Contact		: www.ziontek.co.kr
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"
#include "stm32f4xx_exti.h"
/******************************************************************************
							Constant & Macros
******************************************************************************/
#define PSRAM_STRUCT_ADDRESS  0x64661A00    

__SmartLogdata flash;
__globalVal gv;              

RealTime currentDate;  

/*
	USART4 TO SYSTEM COM 1
*/
void SmartLog_Uart4Init(void)
{
	USART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	UART_InitStructure.USART_BaudRate = flash.Uart[USART_1]->Rate; 
  	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	UART_InitStructure.USART_StopBits = USART_StopBits_1;
  	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    UART_InitStructure.USART_Parity = USART_Parity_No;
    
    #if defined(YSI_ODOCT_ENABLE) || defined(YSI_ODD_ENABLE)    //ODO 변경한다.
        UART_InitStructure.USART_WordLength = USART_WordLength_9b;    
        UART_InitStructure.USART_Parity = USART_Parity_Even;
    #endif
    
    
   
  	/* Enable GPIO clock */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

   	/* Enable UART clock */
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

  	/* Connect PXx to USARTx_Tx*/
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);

  	/* Connect PXx to USARTx_Rx*/
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);

  	/* Configure USART Tx as alternate function  */
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	/* Configure USART Rx as alternate function  */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); 	
  	/* UART configuration */
  	USART_Init(UART4, &UART_InitStructure);   
  	/* Enable UART */
  	USART_Cmd(UART4, ENABLE);
}
/*

	USART3 TO SYSTEM COM 3

*/
void SmartLog_Uart3Init(void)
{
	USART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	UART_InitStructure.USART_BaudRate = flash.Uart[USART_3]->Rate;
 
  	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_Parity = USART_Parity_No;
  	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

   
  	/* Enable GPIO clock */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

   	/* Enable UART clock */
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  	/* Connect PXx to USARTx_Tx*/
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);

  	/* Connect PXx to USARTx_Rx*/
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);

  	/* Configure USART Tx as alternate function  */
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
  	/* Configure USART Rx as alternate function  */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);


	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 	
  	/* UART configuration */
  	USART_Init(USART3, &UART_InitStructure);   
  	/* Enable UART */
  	USART_Cmd(USART3, ENABLE);
}
/*

	USART6 TO SYSTEM COM 4

*/
void SmartLog_Uart6Init(void)
{
	USART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	UART_InitStructure.USART_BaudRate = flash.Uart[USART_4]->Rate;//flash.Uart[USART_1]->Rate
	
	
	//UART_InitStructure.USART_BaudRate = 19200;
	
	//Dprintf("flash.Uart[0]->Rate = %d\n",flash.Uart[0]->Rate);
 
  	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_Parity = USART_Parity_No;
  	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

   
  	/* Enable GPIO clock */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

   	/* Enable UART clock */
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

  	/* Connect PXx to USARTx_Tx*/
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);

  	/* Connect PXx to USARTx_Rx*/
  	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

  	/* Configure USART Tx as alternate function  */
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
  	/* Configure USART Rx as alternate function  */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);


	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); 	
  	/* UART configuration */
  	USART_Init(USART6, &UART_InitStructure);   
  	/* Enable UART */
  	USART_Cmd(USART6, ENABLE);
}
#if 1
void SmartLog_Uart7Init(void)
{
	USART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = UART7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	UART_InitStructure.USART_BaudRate = 115200;//flash.Uart[USART_4]->Rate;//flash.Uart[USART_1]->Rate
	//UART_InitStructure.USART_BaudRate = 19200;
	//Dprintf("flash.Uart[0]->Rate = %d\n",flash.Uart[0]->Rate);
 
  	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_Parity = USART_Parity_No;
  	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

   
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
  	GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_UART7);
  	GPIO_PinAFConfig(GPIOF, GPIO_PinSource7, GPIO_AF_UART7);

  	/* Configure USART Tx as alternate function  */
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOF, &GPIO_InitStructure);
  	/* Configure USART Rx as alternate function  */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  	GPIO_Init(GPIOF, &GPIO_InitStructure);

	USART_ITConfig(UART7, USART_IT_RXNE, ENABLE); 	
  	/* UART configuration */
  	USART_Init(UART7, &UART_InitStructure);   
  	/* Enable UART */
  	USART_Cmd(UART7, ENABLE);
}
#endif
/*

	USART1 TO SYSTEM COM 2

*/
void SmartLog_Uart1Init(void)
{
	USART_InitTypeDef UART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  	GPIO_InitTypeDef GPIO_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	UART_InitStructure.USART_BaudRate = flash.Uart[USART_2]->Rate;
 
  	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_Parity = USART_Parity_No;
  	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

   
  	/* Enable GPIO clock */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

   	/* Enable UART clock */
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  	/* Connect PXx to USARTx_Tx*/
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);

  	/* Connect PXx to USARTx_Rx*/
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

  	/* Configure USART Tx as alternate function  */
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//OPEN 으로 바꾸면 통신이 안된다.
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	/* Configure USART Rx as alternate function  */
	
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 	
  	/* UART configuration */
  	USART_Init(USART1, &UART_InitStructure);   
  	/* Enable UART */
  	USART_Cmd(USART1, ENABLE);}

void EXTILine15_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOF clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

#if defined (SAMAN_ENABLE)	//인터럽트를 사용설정
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource15);

	/* Configure EXTI Line13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	//EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line13 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif	
}

void EXTILine3_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	/* Enable GPIOF clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

#if 0	
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);

	/* Configure EXTI Line13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line3;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;//EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line13 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif	
}
void EXTILine7_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOF clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource7);

	/* Configure EXTI Line13 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line13 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void IoportInit( void )
{
    GPIO_InitTypeDef   GPIO_InitStructure;				 
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOF, ENABLE);
	
#ifdef  DATALOG_8INCH
	/*
		SM718 POWER 제어
	*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
    GPIO_ResetBits(GPIOC,GPIO_Pin_5);
    /*
		RELAY 1channel 2channel
	*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	/*
		입력접점 
	*/
	#if 0
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	#endif
	
#ifdef WIZ_ENABLE
    RelayControl(0, OFF);
    RelayControl(1, OFF);
#endif
#ifdef NORMAL_PORT
	Dprintf("FUNCTION :: IoportInit :: RELAY INIT\n");
	
	flash.relayPortControlFlag[0]=OFF;
	flash.relayPortControlFlag[1]=OFF;
	flash.relayPortControlFlag[2]=OFF;
	flash.relayPortControlFlag[3]=OFF;
	flash.relayPortControlFlag[4]=OFF;
	flash.relayPortControlFlag[5]=OFF;
	flash.relayPortControlFlag[6]=OFF;
	/*
	RelayControl(   
	RELAY_1_PORT, OFF,
	RELAY_2_PORT, flash.relayPortControlFlag[1], 
	RELAY_3_PORT, flash.relayPortControlFlag[2], 
	RELAY_4_PORT, flash.relayPortControlFlag[3], 
	RELAY_5_PORT, flash.relayPortControlFlag[4],
	RELAY_6_PORT, flash.relayPortControlFlag[5], 
	RELAY_7_PORT, flash.relayPortControlFlag[6] 
	);
	*/
#endif
    
    
    //EXTILine13_Config();//강수량 측정용
#endif //DATALOG_8INCH    
#ifdef  DATALOG_7INCH     
    //input
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
    //relay
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_13 | GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

    //AD420
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_13;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 


#endif //DATALOG_7INCH
    //GPIO_SetBits(GPIOF, GPIO_Pin_11);
}


/**
  * @fn 	ADS1110_Init
  * @brief  
  * @note   
  * @param  
  * @param  
  * @retval None
  * @sa  ADS1110.c
  */

#ifdef DATALOG_7INCH
#define		I2C_ADDR    0x90    //Marking :: ED0

#define		SDA_High_ADS1110	GPIO_SetBits(GPIOB, GPIO_Pin_9)
#define		SDA_Low_ADS1110	GPIO_ResetBits(GPIOB, GPIO_Pin_9)

#define		SCL_High_ADS1110	GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define		SCL_Low_ADS1110	GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#endif
#ifdef DATALOG_8INCH
#define		I2C_ADDR    0x90    //Marking :: ED0

#define		SDA_High_ADS1110	GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define		SDA_Low_ADS1110	GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define		SCL_High_ADS1110	GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define		SCL_Low_ADS1110	GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#endif
volatile unsigned int  gADC_Value;
volatile unsigned char gADC_Reg;

#if 0
void I2C_Delay(void)
{
	int s=2000;
	while(s--);
}
/**
	* @DATA 포트를 출력으로 설정한다.
	*/
void I2CSDAPort_Output(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	
#ifdef DATALOG_7INCH	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
#endif    
#ifdef DATALOG_8INCH	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
#endif    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_Delay();
}

/**
	* @DATA 포트를 입력으로 설정한다.
	*/
void I2CSDAPort_Input(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	
#ifdef DATALOG_7INCH	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
#endif    
#ifdef DATALOG_8INCH	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
#endif    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_Delay();
}

//I2C Start 시그널발생
void I2C_Start(void)
{
    I2CSDAPort_Output();
	SDA_High_ADS1110;
	SCL_High_ADS1110;	
	I2C_Delay();

	SDA_Low_ADS1110;	I2C_Delay();	I2C_Delay();
	SCL_Low_ADS1110;	I2C_Delay();
}

//I2C Stop 시그널발생
void I2C_Stop(void)
{
    I2CSDAPort_Output();
	SDA_Low_ADS1110;		
	SCL_Low_ADS1110;
			
	SCL_High_ADS1110;	I2C_Delay(); I2C_Delay();
	SDA_High_ADS1110;	I2C_Delay(); I2C_Delay();
}

void I2C_Clk(void)
{
	SCL_High_ADS1110;	I2C_Delay();
	SCL_Low_ADS1110;	I2C_Delay();
}

void I2C_Clock_Data( unsigned char data)
{
   	if(data & 0x80) SDA_High_ADS1110;
   	else  SDA_Low_ADS1110;
	I2C_Delay();
	I2C_Clk();   
    
   	if(data & 0x40) SDA_High_ADS1110;
   	else  SDA_Low_ADS1110;
	I2C_Delay();
	I2C_Clk();
   	
    if(data & 0x20) SDA_High_ADS1110;
   	else  SDA_Low_ADS1110;
	I2C_Delay();
	I2C_Clk();
   	
    if(data & 0x10) SDA_High_ADS1110;
   	else  SDA_Low_ADS1110;
	I2C_Delay();
	I2C_Clk();
   	
    if(data & 0x08) SDA_High_ADS1110;
   	else  SDA_Low_ADS1110;
	I2C_Delay();
	I2C_Clk();
   	
    if(data & 0x04) SDA_High_ADS1110;
   	else  SDA_Low_ADS1110;
	I2C_Delay();
	I2C_Clk();
   	
    if(data & 0x02) SDA_High_ADS1110;
   	else  SDA_Low_ADS1110;
	I2C_Delay();
	I2C_Clk();
   	
    if(data & 0x01) SDA_High_ADS1110;
   	else  SDA_Low_ADS1110;
	I2C_Delay();
	I2C_Clk();
}

void I2C_ACK(void) //ADS1110
{
    I2CSDAPort_Input();
	I2C_Delay();   
    
    SCL_High_ADS1110;
	I2C_Delay();
	I2C_Delay();
          
    while(1)
    {
#ifdef DATALOG_7INCH	
        if(!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9))) break;
#endif    
#ifdef DATALOG_8INCH	
        if(!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))) break;
#endif    
    }
    SCL_Low_ADS1110;

    I2CSDAPort_Output();
}

void I2C_ACK_M(void) //Master
{
    I2CSDAPort_Output();  
	I2C_Delay();
        
    SDA_Low_ADS1110;
	I2C_Clk();
      
    I2CSDAPort_Input();
	I2C_Delay();          
}

void ADS1110_Write(unsigned char data)
{
    I2C_Start();
    I2C_Clock_Data(I2C_ADDR); //0x90          

    I2C_ACK();                      

    I2C_Clock_Data(data);        

    I2C_ACK();                      

    I2C_Stop();    
}

void ADS1110_Read(void)
{
    unsigned int 	data=0x00;  
    unsigned char 	reg=0;

    I2C_Start();
    I2C_Clock_Data(I2C_ADDR+1); //0x91           
    I2C_ACK();                      

    I2CSDAPort_Input();
  	I2C_Delay();
    
    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x8000;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x8000;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();

    SCL_High_ADS1110; I2C_Delay();
	
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x4000;  
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x4000;
#endif    
    SCL_Low_ADS1110; I2C_Delay();
    
    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x2000;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x2000;
#endif    
    SCL_Low_ADS1110; I2C_Delay();
    
    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x1000;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x1000;
#endif    
    SCL_Low_ADS1110; I2C_Delay();
    
    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0800;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0800;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();
    
    SCL_High_ADS1110;  I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0400;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0400;
#endif    
    SCL_Low_ADS1110;   I2C_Delay();
    
    SCL_High_ADS1110;  I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0200;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0200;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();
    
    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0100;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0100;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();
    
    
    I2C_ACK_M();
        
    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0080;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0080;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();

    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0040;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0040;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();

    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0020;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0020;
#endif    
    SCL_Low_ADS1110;   I2C_Delay();

    SCL_High_ADS1110;   I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0010;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0010;
#endif    
    SCL_Low_ADS1110;    I2C_Delay();

    SCL_High_ADS1110;  I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0008;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0008;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();

    SCL_High_ADS1110;  I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0004;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0004;
#endif    
    SCL_Low_ADS1110;   I2C_Delay();

    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0002;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0002;
#endif    
    SCL_Low_ADS1110;   I2C_Delay();

    SCL_High_ADS1110;  I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x0001;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x0001;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();

    I2C_ACK_M();
    
    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x80;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x80;
#endif    
    SCL_Low_ADS1110; I2C_Delay();

    SCL_High_ADS1110;  I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x40;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x40;
#endif    
    SCL_Low_ADS1110;   I2C_Delay();

    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x20;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x20;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();

    SCL_High_ADS1110;  I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x10;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x10;
#endif    
    SCL_Low_ADS1110;   I2C_Delay();

    SCL_High_ADS1110;   I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x08;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x08;
#endif    
    SCL_Low_ADS1110;    I2C_Delay();

    SCL_High_ADS1110;   I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x04;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x04;
#endif    
    SCL_Low_ADS1110;   I2C_Delay();

    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x02;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x02;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();

    SCL_High_ADS1110; I2C_Delay();
#ifdef DATALOG_7INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) data |= 0x01;
#endif    
#ifdef DATALOG_8INCH	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) data |= 0x01;
#endif    
    SCL_Low_ADS1110;  I2C_Delay();

    I2C_ACK_M();
    
    I2C_Stop();
    gADC_Value =  data;
    gADC_Reg = reg;   
}


void ADS1110_Init( void )
{
#ifdef DATALOG_7INCH     
	GPIO_InitTypeDef   GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOG, &GPIO_InitStructure);
#endif	
#ifdef DATALOG_8INCH     
	GPIO_InitTypeDef   GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif	
	SDA_High_ADS1110;
	SCL_High_ADS1110;
}
extern void AD_Port_Select( int nChannel );
#ifdef DATALOG_8INCH
void AD1110_Channel(unsigned int ch)
{
    gv.DA_AddressData &= ~0x70;
    DAC_PORT_ADDRESS = gv.DA_AddressData;
    switch(ch)
    {
        case 0:
            gv.DA_AddressData &= ~0x70;
            DAC_PORT_ADDRESS |= gv.DA_AddressData;
            break;
        case 1:
            gv.DA_AddressData |= 0x10;
            DAC_PORT_ADDRESS |= gv.DA_AddressData;
            break;
        case 2:
            gv.DA_AddressData |= 0x20;
            DAC_PORT_ADDRESS |= gv.DA_AddressData;
            break;
        case 3:
            gv.DA_AddressData |= 0x30;
            DAC_PORT_ADDRESS |= gv.DA_AddressData;
            break;
        case 4:
            gv.DA_AddressData |= 0x40;
            DAC_PORT_ADDRESS |= gv.DA_AddressData;
            break;
        case 5:
            gv.DA_AddressData |= 0x50;
            DAC_PORT_ADDRESS |= gv.DA_AddressData;
            break;
        case 6:
            gv.DA_AddressData |= 0x60;
            DAC_PORT_ADDRESS |= gv.DA_AddressData;
            break;
        case 7:
            gv.DA_AddressData |= 0x70;
            DAC_PORT_ADDRESS |= gv.DA_AddressData;
            break;
    }
}
#endif
float ADCReading( void )
{
#ifdef DATALOG_7INCH
    float   min = 2048.0f / 32768.0f;   
    float   mv;
    static int toggleADPort = 0;
	
    if( toggleADPort == 0)
    {
        //Dprintf("ADC CH1\n");
        GPIO_SetBits(GPIOG, GPIO_Pin_14) ;
        toggleADPort = 1;
        ADS1110_Read(); 
        flash.Input[0]->mV = min *  (float)gADC_Value; 
    }
    else 
    {
        //Dprintf("ADC CH2\n");
        GPIO_ResetBits(GPIOG, GPIO_Pin_14); 
        toggleADPort = 0;
        ADS1110_Read(); 
        flash.Input[1]->mV = min *  (float)gADC_Value; 
	}
	
	if(mv > 2000.0F) mv = 0.0F;
#endif
#ifdef DATALOG_8INCH
    float   min = 2048.0f / 32768.0f;   
    float   mv;
    static int toggleADPort = 0;
	
    switch(toggleADPort)
    {
        case 0:
            ADS1110_Read(); 
            flash.Input[0]->mV = min *  (float)gADC_Value; 
            AD1110_Channel(1);
            break;
        case 1:
            ADS1110_Read(); 
            flash.Input[1]->mV = min *  (float)gADC_Value; 
            AD1110_Channel(2);
            break;
        case 2:
            ADS1110_Read(); 
            flash.Input[2]->mV = min *  (float)gADC_Value; 
            AD1110_Channel(3);
            break;
        case 3:
            ADS1110_Read(); 
            flash.Input[3]->mV = min *  (float)gADC_Value; 
            AD1110_Channel(4);
            break;
        case 4:
            ADS1110_Read(); 
            flash.Input[4]->mV = min *  (float)gADC_Value; 
            AD1110_Channel(5);
            break;
        case 5:
            ADS1110_Read(); 
            flash.Input[5]->mV = min *  (float)gADC_Value; 
            AD1110_Channel(6);
            break;
        case 6:
            ADS1110_Read(); 
            flash.Input[6]->mV = min *  (float)gADC_Value; 
            AD1110_Channel(7);
            break;
        case 7:
            ADS1110_Read(); 
            flash.Input[7]->mV = min *  (float)gADC_Value; 
            AD1110_Channel(0);
            break;
    }
    //측정후에 여기서 4051채널을 변경한다. 변경후 1초의 여유시간이 있다.
    
    if(flash.Input[toggleADPort]->mV > 2500.0F) flash.Input[toggleADPort]->mV = 0.0F;
    toggleADPort++;    
    
    if(toggleADPort>=8)toggleADPort=0;
	
#endif
	return(mv);     
}
#endif

#ifdef DATALOG_7INCH
#define		AD420_CLR_HIGH			GPIO_SetBits(GPIOC, GPIO_Pin_2) 
#define		AD420_CLR_LOW			    GPIO_ResetBits(GPIOC, GPIO_Pin_2) 

#define 		AD420_LATCH_HIGH		GPIO_SetBits(GPIOC, GPIO_Pin_13) 
#define		AD420_LATCH_LOW			GPIO_ResetBits(GPIOC, GPIO_Pin_13) 

#define		AD420_DATA_HIGH			GPIO_SetBits(GPIOB, GPIO_Pin_7) 
#define		AD420_DATA_LOW			GPIO_ResetBits(GPIOB, GPIO_Pin_7) 

#define		AD420_CLK_HIGH			GPIO_SetBits(GPIOD, GPIO_Pin_3) 
#define		AD420_CLK_LOW			    GPIO_ResetBits(GPIOD, GPIO_Pin_3) 
#define 		AD420_DELAY			        20
#endif
#ifdef DATALOG_8INCH

#define		AD420_CLR_HIGH			(gv.DA_AddressData  |= 0x01)
#define		AD420_CLR_LOW			(gv.DA_AddressData &= ~0x01)

#define 		AD420_LATCH_HIGH		(gv.DA_AddressData |= 0x02)
#define		AD420_LATCH_LOW		(gv.DA_AddressData &= ~0x02)

#define		AD420_DATA_HIGH		(gv.DA_AddressData |= 0x08)
#define		AD420_DATA_LOW			(gv.DA_AddressData &= ~0x08) 

#define		AD420_CLK_HIGH			(gv.DA_AddressData |= 0x04)
#define		AD420_CLK_LOW			(gv.DA_AddressData &= ~0x04)
#define 		AD420_DELAY			        20

#endif
 
#ifdef DATALOG_7INCH
void IntAD420Control(unsigned short value1, unsigned short value2, unsigned short value3, unsigned short value4)
{
	unsigned short		nCnt=0, delay=0, channel = 0;
    unsigned short        valueBuf[4];
    
	AD420_LATCH_LOW;
	AD420_CLK_LOW;
	AD420_CLR_LOW;

    valueBuf[0] = value1;
    valueBuf[1] = value2;
    valueBuf[2] = value3;
    valueBuf[3] = value4;
    for( channel = 0; channel < 4; channel++ )
    {
        for(nCnt=15; nCnt>0; nCnt--)
        {
            if((valueBuf[channel] >> nCnt) & 0x0001)	AD420_DATA_HIGH; 
            else 							AD420_DATA_LOW;
            for(delay=0;delay<AD420_DELAY;delay++);
            AD420_CLK_HIGH;
            for(delay=0;delay<AD420_DELAY;delay++);
            AD420_CLK_LOW;
        }
        if(valueBuf[channel] & 0x0001)	AD420_DATA_HIGH; 
        else 					AD420_DATA_LOW;
        for(delay=0;delay<AD420_DELAY;delay++);
        AD420_CLK_HIGH;
        for(delay=0;delay<AD420_DELAY;delay++);
        AD420_CLK_LOW;
    }
	AD420_LATCH_HIGH;
	for(delay=0;delay<AD420_DELAY;delay++);
	AD420_LATCH_LOW;
}
#endif
void UART_AD420_Control( unsigned short value1, unsigned short value2 )
{
	
    char buf1[50],buf2[50];
    
    sprintf(buf1,"%05d",value1);
    sprintf(buf2,"%05d",value2);
    
    Uart_SendByte7(0x02);
    
    Uart_SendByte7('A');
    Uart_SendByte7(buf1[0]);
    Uart_SendByte7(buf1[1]);
    Uart_SendByte7(buf1[2]);
    Uart_SendByte7(buf1[3]);
    Uart_SendByte7(buf1[4]);

    Uart_SendByte7(buf2[0]);
    Uart_SendByte7(buf2[1]);
    Uart_SendByte7(buf2[2]);
    Uart_SendByte7(buf2[3]);
    Uart_SendByte7(buf2[4]);
	
	if(flash.Uart[USART_1]->UartOr485Select==0)	{
		Uart_SendByte7('0');//RS232
	} else {
		Uart_SendByte7('1');
	}
	if(flash.Uart[USART_2]->UartOr485Select==0)	{
		Uart_SendByte7('0');
	} else {
		Uart_SendByte7('1');
	}
	if(flash.Uart[USART_3]->UartOr485Select==0)	{
		Uart_SendByte7('0');
	} else {
		Uart_SendByte7('1');
	}
	if(flash.Uart[USART_4]->UartOr485Select==0)	{
		Uart_SendByte7('0');
	} else {
		Uart_SendByte7('1');
	}
    Uart_SendByte7(0x0D);
}

#ifdef DATALOG_7INCH
void RelayControl( unsigned int relayCh, unsigned int relayStatus)
{
    switch( relayCh )
    {
        case 1:  
            if(relayStatus==ON) GPIO_SetBits(GPIOC, GPIO_Pin_5);
            else                    GPIO_ResetBits(GPIOC, GPIO_Pin_5);
        break;
        case 2:  
            if(relayStatus==ON) GPIO_SetBits(GPIOB, GPIO_Pin_1);
            else                        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
        break;
        case 3:  
            if(relayStatus==ON) GPIO_SetBits(GPIOB, GPIO_Pin_13);
            else                        GPIO_ResetBits(GPIOB, GPIO_Pin_13);
        break;
        case 4:  
            if(relayStatus==ON) GPIO_SetBits(GPIOB, GPIO_Pin_5);
            else                        GPIO_ResetBits(GPIOB, GPIO_Pin_5);
        break;
    }
}
#endif
#ifdef DATALOG_8INCH
#if 0 //2020113 수정
int RelayControl( 
    unsigned char port1, unsigned int status1, 
    unsigned char port2, unsigned int status2,
    unsigned char port3, unsigned int status3,
    unsigned char port4, unsigned int status4,
    unsigned char port5, unsigned int status5,
    unsigned char port6, unsigned int status6,
    unsigned char port7, unsigned int status7
    )
{
    if( status1 == ON ) GPIO_SetBits(GPIOB, GPIO_Pin_7);
    else GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    if( status2 == ON ) GPIO_SetBits(GPIOB, GPIO_Pin_8);
    else GPIO_ResetBits(GPIOB, GPIO_Pin_8);
    
    Uart_SendByte7(0x02);
    Uart_SendByte7('R');
    flash.relayPortControlFlag[2]=status3;
    flash.relayPortControlFlag[3]=status4;
    flash.relayPortControlFlag[4]=status5;
    flash.relayPortControlFlag[5]=status6;
	flash.relayPortControlFlag[6]=status7;
    
    if(status3==1)Uart_SendByte7('1');
    else Uart_SendByte7('0');
    if(status4==1)Uart_SendByte7('1');
    else Uart_SendByte7('0');
    if(status5==1)Uart_SendByte7('1');
    else Uart_SendByte7('0');
    if(status6==1)Uart_SendByte7('1');
    else Uart_SendByte7('0');
    if(status7==1)Uart_SendByte7('1');
    else Uart_SendByte7('0');
    Uart_SendByte7(0x03);
    return 0;
}    
#endif
#endif


#ifdef __cplusplus
}
#endif
