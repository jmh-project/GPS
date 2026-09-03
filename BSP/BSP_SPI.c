/******************************************************************************
*    	File name	: BSP_SPI.c
*		Version		: 1.00
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

#define SPI_MASTER  
/******************************************************************************
							Constant & Macros
******************************************************************************/
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/* SPIx Communication boards Interface */
#define SPIx                           SPI1
#define SPIx_CLK                       RCC_APB2Periph_SPI1//RCC_APB2Periph_SPI1
#define SPIx_CLK_INIT                  RCC_APB2PeriphClockCmd
#define SPIx_IRQn                      SPI1_IRQn
#define SPIx_IRQHANDLER                SPI1_IRQHandler

#define SPIx_SCK_PIN                   GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT             GPIOA
#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define SPIx_SCK_SOURCE                GPIO_PinSource5
#define SPIx_SCK_AF                    GPIO_AF_SPI1

#define SPIx_MISO_PIN                  GPIO_Pin_6
#define SPIx_MISO_GPIO_PORT            GPIOA
#define SPIx_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define SPIx_MISO_SOURCE               GPIO_PinSource6
#define SPIx_MISO_AF                   GPIO_AF_SPI1

#define SPIx_MOSI_PIN                  GPIO_Pin_7
#define SPIx_MOSI_GPIO_PORT            GPIOA
#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define SPIx_MOSI_SOURCE               GPIO_PinSource7
#define SPIx_MOSI_AF                   GPIO_AF_SPI1

#define TXBUFFERSIZE                    (countof(TxBuffer) - 1)
#define RXBUFFERSIZE                    TXBUFFERSIZE

/******************************************************************************
							Private & Local Variables
******************************************************************************/
uint8_t TxBuffer[] = "SPI Interrupt Example: Communication between two SPI using Interrupts";
__IO uint32_t TimeOut = 0;

#ifdef SPI_SLAVE
__IO uint8_t RxBuffer [RXBUFFERSIZE];
__IO uint8_t Rx_Idx = 0;
#endif
#ifdef SPI_MASTER
__IO uint8_t Tx_Idx = 0;
__IO uint8_t CmdTransmitted = 0;
__IO uint8_t CmdStatus = 0x00;
__IO uint8_t NumberOfByte = 0;
#endif

SPI_InitTypeDef  SPI_InitStructure;

/******************************************************************************
							Function Prototype
******************************************************************************/
#ifdef SPI_MASTER
//static void TimeOut_UserCallback(void);
#endif

#ifdef SPI_SLAVE
 static TestStatus Buffercmp(uint8_t* pBuffer1, __IO uint8_t* pBuffer2, uint16_t BufferLength);
 static void Fill_Buffer(__IO uint8_t *pBuffer, uint16_t BufferLength);
#endif

#ifdef SPI_MASTER
 uint8_t GetVar_NbrOfData(void);
#endif

/******************************************************************************
							Program Entry
******************************************************************************/
void SPI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable the SPI clock */
    SPIx_CLK_INIT(SPIx_CLK, ENABLE);

    /* Enable GPIO clocks */
    RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);

    /* SPI GPIO Configuration --------------------------------------------------*/
    /* Connect SPI pins to AF5 */  
    GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
    GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

    /* SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
    GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

    /* SPI  MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
    GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

    /* SPI configuration -------------------------------------------------------*/
    SPI_I2S_DeInit(SPIx);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

    /* Configure the Priority Group to 1 bit */                
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /* Configure the SPI interrupt priority */
    NVIC_InitStructure.NVIC_IRQChannel = SPIx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

uint8_t GetVar_NbrOfData(void)
{
    return NumberOfByte;
}

void SPIx_IRQHANDLER(void)
{
#ifdef SPI_SLAVE
    /* SPI in Slave Receiver mode--------------------------------------- */
    if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_RXNE) == SET)
    {
        RxBuffer[Rx_Idx++] = SPI_I2S_ReceiveData(SPIx);
    }
#endif 
#ifdef SPI_MASTER
    /* SPI in Master Tramitter mode--------------------------------------- */
    if (SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_TXE) == SET)
    {
        if (CmdStatus == 0x00)
        {
            /* Send Transaction code */
            SPI_I2S_SendData(SPIx, CmdTransmitted);
            CmdStatus = 0x01;
        }
        else
        {
            if (Tx_Idx < GetVar_NbrOfData())
            {
                /* Send Transaction data */
                SPI_I2S_SendData(SPIx, TxBuffer[Tx_Idx++]);
            }
            else
            {
                /* Disable the Tx buffer empty interrupt */
                SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
            }
        }
    }
#endif /* SPI_SLAVE */
}

void SPIDelay(int s)
{
    while(s--);
}

void SPI_MasterTest(void)
{
  	TimeOut = USER_TIMEOUT; // 100=1sec
  
    SPI_Config();
  	/* Initializes the SPI communication */
  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_Init(SPIx, &SPI_InitStructure);
  
  	/* The Data transfer is performed in the SPI interrupt routine */
  	/* Enable the SPI peripheral */
  	SPI_Cmd(SPIx, ENABLE);

#if 0    
    while(1)
    {
        Dprintf("Press any key... SPI Out---1\n");
        Dgetch();
        SPI_I2S_SendData(SPIx, 0x11);
        SPIDelay(3);
        //while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
        SPI_I2S_SendData(SPIx, 0x22);
        SPIDelay(3);
        //while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
        SPI_I2S_SendData(SPIx, 0x33);
        SPIDelay(3);
        //while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    }
    
  	while (1)
  	{
        Dprintf("Press any key... SPI Out\n");
        Dgetch();
        CmdTransmitted = 0x77;
        NumberOfByte = 1;
    
        /* Master Transmit Command followed by Data Transaction ------------------*/
        CmdStatus = 0x00;
        Tx_Idx = 0;
        /* Enable the Tx buffer empty interrupt */
        SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);
  
        /* Waiting the end of Data transfer */
        while (Tx_Idx < GetVar_NbrOfData())
        {
        }
    }
#endif	
}


