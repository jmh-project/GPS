/******************************************************************************
*    	File name	: BSP_CAN.c
*		Company		:
*		Version		:
*		Data		:
*		Author		:
*		Contact		:
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************
							Include File
******************************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "BSP_Can.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "Debug_Uart.h"

void Delay(__IO uint32_t nTime);

/******************************************************************************
							Constant & Macros
******************************************************************************/
/*
#define     USE_CAN2

#ifdef  USE_CAN1
  #define CANx                       CAN1
  #define CAN_CLK                    RCC_APB1Periph_CAN1
  #define CAN_RX_PIN                 GPIO_Pin_9		//GPIO_Pin_0
  #define CAN_TX_PIN                 GPIO_Pin_13	//GPIO_Pin_1
  #define CAN_GPIO_RX_PORT           GPIOI
  #define CAN_GPIO_TX_PORT			 GPIOH
  #define CAN_GPIO_CLK               (RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOH)
  #define CAN_AF_PORT                GPIO_AF_CAN1
  #define CAN_RX_SOURCE              GPIO_PinSource9 //GPIO_PinSource0
  #define CAN_TX_SOURCE              GPIO_PinSource13 //GPIO_PinSource1       
#else
  #define CANx                       CAN2
  #define CAN_CLK                    (RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2)
  #define CAN_RX_PIN                 GPIO_Pin_5
  #define CAN_TX_PIN                 GPIO_Pin_13
  #define CAN_GPIO_PORT              GPIOB

  #define CAN_GPIO_RX_PORT           GPIOB
  #define CAN_GPIO_TX_PORT			 GPIOB
  
  #define CAN_GPIO_CLK               RCC_AHB1Periph_GPIOB
  #define CAN_AF_PORT                GPIO_AF_CAN2
  #define CAN_RX_SOURCE              GPIO_PinSource5
  #define CAN_TX_SOURCE              GPIO_PinSource13    
#endif
*/
/******************************************************************************
							Private & Local Variables
******************************************************************************/
CanRxMsg    RxMessage;
volatile    uint8_t     Rx_Flag = 0;
CanTxMsg    TxMessage;
/******************************************************************************
							Function Prototype
******************************************************************************/
void CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

#ifdef  USE_CAN1 
    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void CAN_Config(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;
  
    /* CAN GPIOs configuration **************************************************/
    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);
  
    /* Configure CAN RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(CAN_GPIO_RX_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
    GPIO_Init(CAN_GPIO_TX_PORT, &GPIO_InitStructure);

    /* Connect CAN pins to AF9 */
    GPIO_PinAFConfig(CAN_GPIO_RX_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
    GPIO_PinAFConfig(CAN_GPIO_TX_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 

    /* CAN configuration ********************************************************/  
    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
    /* CAN register init */
    CAN_DeInit(CANx);
    CAN_StructInit(&CAN_InitStructure);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //CAN_Mode_LoopBack; //CAN_Mode_Normal; //
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
    /* CAN Baudrate = 500kBps (CAN clocked at 42 MHz) */
    CAN_InitStructure.CAN_BS1 = CAN_BS1_14tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
    CAN_InitStructure.CAN_Prescaler = 8;
    CAN_Init(CANx, &CAN_InitStructure);

    /* CAN filter init */
#ifdef  USE_CAN1
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
#else /* USE_CAN2 */
    CAN_FilterInitStructure.CAN_FilterNumber = 14;
#endif  /* USE_CAN1 */
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
  
    /* Transmit Structure preparation */
    TxMessage.StdId = 0x0;
    TxMessage.ExtId = 0x1000;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_EXT;
    TxMessage.DLC = 8;
  
    /* Enable FIFO 0 message pending Interrupt */
    CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}

/**
  * @brief  Initializes the Rx Message.
  * @param  RxMessage: pointer to the message to initialize
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
    uint8_t i = 0;

    RxMessage->StdId = 0x00;
    RxMessage->ExtId = 0x00;
    RxMessage->IDE = CAN_ID_EXT;
    RxMessage->DLC = 0;
    RxMessage->FMI = 0;
    for (i = 0;i < 8;i++)
    {
        RxMessage->Data[i] = 0x00;
    }
}

void CanDelay(void)
{
    uint16_t nTime = 0x0000;

    for(nTime = 0; nTime <0xFFF; nTime++)
    {
    }
}

void CANTest(void)
{
    int     i;
    char    buf[64];
    int     count=0;
    
    Dprintf("CAN Network Test... Any Key Press Send Data\n");
    CAN_NVIC_Config();
    CAN_Config();

    while(1)
    {	
        /* check the receive flag */
        if (Rx_Flag==1) 
        {
            Rx_Flag = 0;
            for(i=0; i<8; i++) buf[i] = RxMessage.Data[i];
            buf[8] = 0;
            Dprintf("CAN Receive Data:%s\n", buf);
        }
		else if(Rx_Flag==2) 
		{
			Dprintf("Receive Data but ID different=%08x\n", RxMessage.ExtId);
		}

        /* send Data */
        if(Dstatus())		
        {
            char sd = Dgetch();    
            Dprintf("Teeco Board CAN TX Start\n");

            TxMessage.StdId = 0x0;
            TxMessage.ExtId = 0x1000; //나의 ID
            TxMessage.RTR = CAN_RTR_DATA;
            TxMessage.IDE = CAN_ID_EXT;
            TxMessage.DLC = 8;            
            
            sprintf((char *)TxMessage.Data,"%02dDataTx", count);

            if (CAN_TxStatus_NoMailBox == CAN_Transmit(CANx, &TxMessage)) 
            {
                Dprintf("TX Error\n");
            }
            CanDelay();
            count++;
            if(count >= 100) count=0;
        }		
    }    
}

/*
#ifdef USE_CAN2
void CAN2_RX0_IRQHandler(void)
{
    CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);
    //수신할 ID
    if ((RxMessage.ExtId == 0x2000)&&(RxMessage.IDE == CAN_ID_EXT) && (RxMessage.DLC == 8))
    {
        Rx_Flag = 1;
    }
}
#endif
*/

#ifdef __cplusplus
}
#endif

