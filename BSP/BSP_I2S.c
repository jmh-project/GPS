/******************************************************************************
*    	File name	: BSP_I2S.c
*		Company		:
*		Version		:
*		Data		:
*		Author		:
*		Contact		:
******************************************************************************/



/******************************************************************************
							Include File
******************************************************************************/
#include "Teeco_System.h"
#include "BSP_I2S.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
 /* This is an audio file stored in the Flash memory as a constant table of 16-bit data.
    The audio format should be WAV (raw / PCM) 16-bits, Stereo (sampling rate may be modified) */
extern uint16_t AUDIO_SAMPLE[];
/* Audio file size and start address are defined here since the audio file is 
    stored in Flash memory as a constant table of 16-bit data */
#define AUDIO_FILE_SZE          990000

#define AUIDO_START_ADDRESS     58 /* Offset relative to audio file header size */

/* Delay for the Codec to be correctly reset */
#define CODEC_RESET_DELAY               0x4FFF

/* Mask for the bit EN of the I2S CFGR register */
#define I2S_ENABLE_MASK                 0x0400

/* Codec audio Standards */
#ifdef I2S_STANDARD_PHILLIPS
    #define  CODEC_STANDARD                0x04
    #define I2S_STANDARD                   I2S_Standard_Phillips         
#elif defined(I2S_STANDARD_MSB)
    #define  CODEC_STANDARD                0x00
    #define I2S_STANDARD                   I2S_Standard_MSB    
#elif defined(I2S_STANDARD_LSB)
    #define  CODEC_STANDARD                0x08
    #define I2S_STANDARD                   I2S_Standard_LSB    
#else 
    #error "Error: No audio communication standard selected !"
#endif /* I2S_STANDARD */

/* The 7 bits Codec address (sent through I2C interface) */
#define CODEC_ADDRESS                   0x94  /* b00100111 */
/******************************************************************************
							Private & Local Variables
******************************************************************************/
__IO uint32_t XferCplt = 0;
__IO uint8_t volume = 70, AudioPlayStart = 0;
__IO uint32_t WaveCounter;
uint8_t Buffer[6];
__IO uint32_t WaveDataLength = 0;

__IO uint8_t RepeatState = 0;
__IO uint32_t CurrAudioInterface = AUDIO_INTERFACE_I2S; //AUDIO_INTERFACE_DAC

/* DMA Stream definitions */
uint32_t AUDIO_MAL_DMA_CLOCK    = AUDIO_I2S_DMA_CLOCK;
DMA_Stream_TypeDef * AUDIO_MAL_DMA_STREAM   = AUDIO_I2S_DMA_STREAM ;       
uint32_t AUDIO_MAL_DMA_DREG     = AUDIO_I2S_DMA_DREG;
uint32_t AUDIO_MAL_DMA_CHANNEL  = AUDIO_I2S_DMA_CHANNEL;
uint32_t AUDIO_MAL_DMA_IRQ      = AUDIO_I2S_DMA_IRQ  ;
uint32_t AUDIO_MAL_DMA_FLAG_TC  = AUDIO_I2S_DMA_FLAG_TC;
uint32_t AUDIO_MAL_DMA_FLAG_HT  = AUDIO_I2S_DMA_FLAG_HT;
uint32_t AUDIO_MAL_DMA_FLAG_FE  = AUDIO_I2S_DMA_FLAG_FE;
uint32_t AUDIO_MAL_DMA_FLAG_TE  = AUDIO_I2S_DMA_FLAG_TE;
uint32_t AUDIO_MAL_DMA_FLAG_DME = AUDIO_I2S_DMA_FLAG_DME;

/* This structure is declared global because it is handled by two different functions */
DMA_InitTypeDef DMA_InitStructure; 
DMA_InitTypeDef AUDIO_MAL_DMA_InitStructure;

uint32_t AudioTotalSize = 0xFFFF; /* This variable holds the total size of the audio file */
uint32_t AudioRemSize   = 0xFFFF; /* This variable holds the remaining data in audio file */
uint16_t *CurrentPos ;             /* This variable holds the current position of audio pointer */

__IO uint32_t  CODECTimeout = CODEC_LONG_TIMEOUT;   
__IO uint8_t OutputDev = 0;

/******************************************************************************
							Function Prototype
******************************************************************************/
/**
  * @brief  Inserts a delay time (not accurate timing).
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void Delay_1( __IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}

/*
일반 모드 설명 :
   오디오 파일을 (DMA 스트림을 사용) 재생 시작합니다.
   이 모드를 사용하여 응용 프로그램부터 병렬로 다른 작업을 실행할 수 있습니다
   DMA는 CPU 대신 오디오 전송을 처리합니다.
   CPU에 남아있는 유일한 작업은 DMA의 관리 될 것입니다
   다시 버퍼를로드하고 나머지 데이터를 계산하기 위해 전체 인터럽트 또는 하프 전송 완료 인터럽트를 전송합니다.
   
원형 모드 설명 :
   DMA는 항상 실행 활성화되면, 원형 버퍼에서 파일을 재생 시작합니다. 사용자는 전송이 완료 및 / 또는 
   반 전송 완료 인터럽트 콜백을 사용하여 오디오 데이터가 주기적으로 버퍼를 작성합니다
   (EVAL_AUDIO_TransferComplete_CallBack () 나 EVAL_AUDIO_HalfTransfer_CallBack () ...
   이 경우 오디오 데이터 파일은 DMA 최대 버퍼보다 작습니다
   크기가 65,535 그래서 지속적으로 버퍼를로드하거나 관리를 할 필요가 없습니다
   전체 또는 하프 전송 인터럽트 콜백을 전송할 수 있습니다.*/


void EVAL_AUDIO_SetAudioInterface(uint32_t Interface)
{    
    CurrAudioInterface = Interface;
  
    if (CurrAudioInterface == AUDIO_INTERFACE_I2S)
    {
        /* DMA Stream definitions */
        AUDIO_MAL_DMA_CLOCK    = AUDIO_I2S_DMA_CLOCK;
        AUDIO_MAL_DMA_STREAM   = AUDIO_I2S_DMA_STREAM;        
        AUDIO_MAL_DMA_DREG     = AUDIO_I2S_DMA_DREG;
        AUDIO_MAL_DMA_CHANNEL  = AUDIO_I2S_DMA_CHANNEL;
        AUDIO_MAL_DMA_IRQ      = AUDIO_I2S_DMA_IRQ  ;
        AUDIO_MAL_DMA_FLAG_TC  = AUDIO_I2S_DMA_FLAG_TC;
        AUDIO_MAL_DMA_FLAG_HT  = AUDIO_I2S_DMA_FLAG_HT;
        AUDIO_MAL_DMA_FLAG_FE  = AUDIO_I2S_DMA_FLAG_FE;
        AUDIO_MAL_DMA_FLAG_TE  = AUDIO_I2S_DMA_FLAG_TE;
        AUDIO_MAL_DMA_FLAG_DME = AUDIO_I2S_DMA_FLAG_DME;
    }
    else if (Interface == AUDIO_INTERFACE_DAC)
    {
        /* DMA Stream definitions */
        AUDIO_MAL_DMA_CLOCK    = AUDIO_DAC_DMA_CLOCK;
        AUDIO_MAL_DMA_STREAM   = AUDIO_DAC_DMA_STREAM;        
        AUDIO_MAL_DMA_DREG     = AUDIO_DAC_DMA_DREG;
        AUDIO_MAL_DMA_CHANNEL  = AUDIO_DAC_DMA_CHANNEL;
        AUDIO_MAL_DMA_IRQ      = AUDIO_DAC_DMA_IRQ  ;
        AUDIO_MAL_DMA_FLAG_TC  = AUDIO_DAC_DMA_FLAG_TC;
        AUDIO_MAL_DMA_FLAG_HT  = AUDIO_DAC_DMA_FLAG_HT;
        AUDIO_MAL_DMA_FLAG_FE  = AUDIO_DAC_DMA_FLAG_FE;
        AUDIO_MAL_DMA_FLAG_TE  = AUDIO_DAC_DMA_FLAG_TE;
        AUDIO_MAL_DMA_FLAG_DME = AUDIO_DAC_DMA_FLAG_DME;    
    }
}

/**
  * @brief Initializes IOs used by the Audio Codec (on the control and audio 
  *        interfaces).
  * @param  None
  * @retval None
  */
static void Codec_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
    /* Enable Reset GPIO Clock */
    RCC_AHB1PeriphClockCmd(AUDIO_RESET_GPIO_CLK,ENABLE);
  
    /* Audio reset pin configuration -------------------------------------------------*/
    GPIO_InitStructure.GPIO_Pin = AUDIO_RESET_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(AUDIO_RESET_GPIO, &GPIO_InitStructure);    
  
    /* Enable I2S and I2C GPIO clocks */
    RCC_AHB1PeriphClockCmd(CODEC_I2C_GPIO_CLOCK | CODEC_I2S_GPIO_CLOCK, ENABLE);

    /* CODEC_I2C SCL and SDA pins configuration -------------------------------------*/
    GPIO_InitStructure.GPIO_Pin = CODEC_I2C_SCL_PIN | CODEC_I2C_SDA_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(CODEC_I2C_GPIO, &GPIO_InitStructure);     
    /* Connect pins to I2C peripheral */
    GPIO_PinAFConfig(CODEC_I2C_GPIO, CODEC_I2S_SCL_PINSRC, CODEC_I2C_GPIO_AF);  
    GPIO_PinAFConfig(CODEC_I2C_GPIO, CODEC_I2S_SDA_PINSRC, CODEC_I2C_GPIO_AF);  

    /* CODEC_I2S pins configuration: WS, SCK and SD pins -----------------------------*/
    GPIO_InitStructure.GPIO_Pin = CODEC_I2S_WS_PIN | CODEC_I2S_SCK_PIN | CODEC_I2S_SD_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CODEC_I2S_GPIO, &GPIO_InitStructure);
  
    /* Connect pins to I2S peripheral  */
    GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_WS_PINSRC, CODEC_I2S_GPIO_AF);  
    GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SCK_PINSRC, CODEC_I2S_GPIO_AF);
    GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SD_PINSRC, CODEC_I2S_GPIO_AF);

#ifdef CODEC_MCLK_ENABLED
Dprintf("Codec_GPIO_Init::  MCLK Enable\n");     
    /* CODEC_I2S pins configuration: MCK pin */
    GPIO_InitStructure.GPIO_Pin = CODEC_I2S_MCK_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CODEC_I2S_MCK_GPIO, &GPIO_InitStructure);   
    /* Connect pins to I2S peripheral  */
    GPIO_PinAFConfig(CODEC_I2S_MCK_GPIO, CODEC_I2S_MCK_PINSRC, CODEC_I2S_GPIO_AF); 
#endif /* CODEC_MCLK_ENABLED */ 
}

/**
  * @brief  Resets the audio codec. It restores the default configuration of the 
  *         codec (this function shall be called before initializing the codec).
  * @note   This function calls an external driver function: The IO Expander driver.
  * @param  None
  * @retval None
  */
static void Codec_Reset(void)
{
    /* Power Down the codec */
    GPIO_WriteBit(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, Bit_RESET);

    /* wait for a delay to insure registers erasing */
    Delay_1(CODEC_RESET_DELAY); 
  
    /* Power on the codec */
    GPIO_WriteBit(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, Bit_SET);
}

/**
  * @brief  Initializes the Audio Codec control interface (I2C).
  * @param  None
  * @retval None
  */
static void Codec_CtrlInterface_Init(void)
{
    I2C_InitTypeDef I2C_InitStructure;
  
    /* Enable the CODEC_I2C peripheral clock */
    RCC_APB1PeriphClockCmd(CODEC_I2C_CLK, ENABLE);
  
    /* CODEC_I2C peripheral configuration */
    I2C_DeInit(CODEC_I2C);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x33;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
    /* Enable the I2C peripheral */
    I2C_Cmd(CODEC_I2C, ENABLE);  
    I2C_Init(CODEC_I2C, &I2C_InitStructure);
}

uint32_t Codec_TIMEOUT_UserCallback(void)
{
    Dprintf("Codec TimeOut\n");
    while(1);
}


/**
  * @brief  Reads and returns the value of an audio codec register through the
  *         control interface (I2C).
  * @param  RegisterAddr: Address of the register to be read.
  * @retval Value of the register to be read or dummy value if the communication
  *         fails.
  */
static uint32_t Codec_ReadRegister(uint8_t RegisterAddr)
{
    uint32_t result = 0;

    /*!< While the bus is busy */
    CODECTimeout = CODEC_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }
  
    /* Start the config sequence */
    I2C_GenerateSTART(CODEC_I2C, ENABLE);

    /* Test on EV5 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }
  
    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }

    /* Transmit the register address to be read */
    I2C_SendData(CODEC_I2C, RegisterAddr);

    /* Test on EV8 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF) == RESET)
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }
  
    /*!< Send START condition a second time */  
    I2C_GenerateSTART(CODEC_I2C, ENABLE);
  
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    } 
  
    /*!< Send Codec address for read */
    I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Receiver);  
  
    /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_ADDR) == RESET)
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }     
  
    /*!< Disable Acknowledgment */
    I2C_AcknowledgeConfig(CODEC_I2C, DISABLE);   
  
    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)CODEC_I2C->SR2;
  
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(CODEC_I2C, ENABLE);
  
    /* Wait for the byte to be received */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_RXNE) == RESET)
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }
  
    /*!< Read the byte received from the Codec */
    result = I2C_ReceiveData(CODEC_I2C);
  
    /* Wait to make sure that STOP flag has been cleared */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while(CODEC_I2C->CR1 & I2C_CR1_STOP)
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }  
  
    /*!< Re-Enable Acknowledgment to be ready for another reception */
    I2C_AcknowledgeConfig(CODEC_I2C, ENABLE);  
  
    /* Clear AF flag for next communication */
    I2C_ClearFlag(CODEC_I2C, I2C_FLAG_AF); 
  
    /* Return the byte read from Codec */
    return result;
}


/**
  * @brief  Writes a Byte to a given register into the audio codec through the 
            control interface (I2C)
  * @param  RegisterAddr: The address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint8_t RegisterValue)
{
    uint32_t result = 0;

    /*!< While the bus is busy */
    CODECTimeout = CODEC_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }
  
    /* Start the config sequence */
    I2C_GenerateSTART(CODEC_I2C, ENABLE);

    /* Test on EV5 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }
  
    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }

    /* Transmit the first address for write operation */
    I2C_SendData(CODEC_I2C, RegisterAddr);

    /* Test on EV8 and clear it */
    CODECTimeout = CODEC_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
    }
  
    /* Prepare the register value to be sent */
    I2C_SendData(CODEC_I2C, RegisterValue);
  
    /*!< Wait till all data have been physically transferred on the bus */
    CODECTimeout = CODEC_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF))
    {
        if((CODECTimeout--) == 0) Codec_TIMEOUT_UserCallback();
    }
  
    /* End the configuration sequence */
    I2C_GenerateSTOP(CODEC_I2C, ENABLE);  
  
#ifdef VERIFY_WRITTENDATA
    /* Verify that the data has been correctly written */  
    result = (Codec_ReadRegister(RegisterAddr) == RegisterValue)? 0:1;
#endif /* VERIFY_WRITTENDATA */

    /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return result;  
}

/**
  * @brief  Sets higher or lower the codec volume level.
  * @param  Volume: a byte value from 0 to 255 (refer to codec registers 
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_VolumeCtrl(uint8_t Volume)
{
    uint32_t counter = 0;
  
    if (Volume > 0xE6)
    {
        /* Set the Master volume */
        counter += Codec_WriteRegister(0x20, Volume - 0xE7); 
        counter += Codec_WriteRegister(0x21, Volume - 0xE7);     
    }
    else
    {
        /* Set the Master volume */
        counter += Codec_WriteRegister(0x20, Volume + 0x19); 
        counter += Codec_WriteRegister(0x21, Volume + 0x19); 
    }
    return counter;  
}

/**
  * @brief  Initializes the Audio Codec audio interface (I2S)
  * @note   This function assumes that the I2S input clock (through PLL_R in 
  *         Devices RevA/Z and through dedicated PLLI2S_R in Devices RevB/Y)
  *         is already configured and ready to be used.    
  * @param  AudioFreq: Audio frequency to be configured for the I2S peripheral. 
  * @retval None
  */
static void Codec_AudioInterface_Init(uint32_t AudioFreq)
{
    I2S_InitTypeDef I2S_InitStructure;

    /* Enable the CODEC_I2S peripheral clock */
    RCC_APB1PeriphClockCmd(CODEC_I2S_CLK, ENABLE);
  
    /* CODEC_I2S peripheral configuration */
    SPI_I2S_DeInit(CODEC_I2S);
    I2S_InitStructure.I2S_AudioFreq = AudioFreq;
    I2S_InitStructure.I2S_Standard = I2S_STANDARD;
    I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
    I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
    I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
    
#ifdef CODEC_MCLK_ENABLED
    I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#elif defined(CODEC_MCLK_DISABLED)
    I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#endif /* CODEC_MCLK_ENABLED */
  
    /* Initialize the I2S peripheral with the structure above */
    I2S_Init(CODEC_I2S, &I2S_InitStructure);
}


/*   CS43L22 Audio Codec Control Functions
  * @brief  Initializes the audio codec and all related interfaces (control 
  *         interface: I2C and audio interface: I2S)
  * @param  OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
    uint32_t counter = 0; 

    /* Configure the Codec related IOs */
    Codec_GPIO_Init();   
  
    /* Reset the Codec Registers */
    Codec_Reset();

    /* Initialize the Control interface of the Audio Codec */
    Codec_CtrlInterface_Init();     
  
    /* Keep Codec powered OFF */
    counter += Codec_WriteRegister(0x02, 0x01);  
      
    counter += Codec_WriteRegister(0x04, 0xAF); /* SPK always OFF & HP always ON */
    OutputDev = 0xAF;
  
    /* Clock configuration: Auto detection */  
    counter += Codec_WriteRegister(0x05, 0x81);
  
    /* Set the Slave Mode and the audio Standard */  
    counter += Codec_WriteRegister(0x06, CODEC_STANDARD);
      
    /* Set the Master volume */
    Codec_VolumeCtrl(Volume);
  
    if (CurrAudioInterface == AUDIO_INTERFACE_DAC)
    {
        /* Enable the PassThrough on AIN1A and AIN1B */
        counter += Codec_WriteRegister(0x08, 0x01);
        counter += Codec_WriteRegister(0x09, 0x01);
    
        /* Route the analog input to the HP line */
        counter += Codec_WriteRegister(0x0E, 0xC0);
    
        /* Set the Passthough volume */
        counter += Codec_WriteRegister(0x14, 0x00);
        counter += Codec_WriteRegister(0x15, 0x00);
    }

    /* Power on the Codec */
    counter += Codec_WriteRegister(0x02, 0x9E);  
  
    /* Additional configuration for the CODEC. These configurations are done to reduce
      the time needed for the Codec to power off. If these configurations are removed, 
      then a long delay should be added between powering off the Codec and switching 
      off the I2S peripheral MCLK clock (which is the operating clock for Codec).
      If this delay is not inserted, then the codec will not shut down properly and
      it results in high noise after shut down. */
  
    /* Disable the analog soft ramp */
    counter += Codec_WriteRegister(0x0A, 0x00);
    if (CurrAudioInterface != AUDIO_INTERFACE_DAC)
    {  
        /* Disable the digital soft ramp */
        counter += Codec_WriteRegister(0x0E, 0x04);
    }
    /* Disable the limiter attack level */
    counter += Codec_WriteRegister(0x27, 0x00);
    /* Adjust Bass and Treble levels */
    counter += Codec_WriteRegister(0x1F, 0x0F);
    /* Adjust PCM volume level */
    counter += Codec_WriteRegister(0x1A, 0x0A);
    counter += Codec_WriteRegister(0x1B, 0x0A);

    /* Configure the I2S peripheral */
    Codec_AudioInterface_Init(AudioFreq);  
  
    /* Return communication control value */
    return counter;  
}


/**
  * @brief  DAC  Channel1 Configuration
  * @param  None
  * @retval None
  */
void DAC_Config(void)
{
    DAC_InitTypeDef  DAC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* DMA1 clock and GPIOA clock enable (to be used with DAC) */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOA, ENABLE);

    /* DAC Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

    /* DAC channel 1 & 2 (DAC_OUT1 = PA.4) configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* DAC channel1 Configuration */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
    DAC_Init(AUDIO_DAC_CHANNEL, &DAC_InitStructure);

    /* Enable DAC Channel1 */
    DAC_Cmd(AUDIO_DAC_CHANNEL, ENABLE);
}


/**
  * @brief  Initializes and prepares the Media to perform audio data transfer 
  *         from Media to the I2S peripheral.
  * @param  None
  * @retval None
  */
static void Audio_MAL_Init(void)  
{ 
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
    NVIC_InitTypeDef NVIC_InitStructure;
#endif

    /* Enable the DMA clock */
    RCC_AHB1PeriphClockCmd(AUDIO_MAL_DMA_CLOCK, ENABLE); 

    /* Configure the DMA Stream */
    DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);
    DMA_DeInit(AUDIO_MAL_DMA_STREAM);
    /* Set the parameters to be configured */
    DMA_InitStructure.DMA_Channel = AUDIO_MAL_DMA_CHANNEL;  
    DMA_InitStructure.DMA_PeripheralBaseAddr = CODEC_I2S_ADDRESS;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;      /* This field will be configured in play function */
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)0xFFFE;      /* This field will be configured in play function */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = AUDIO_MAL_DMA_PERIPH_DATA_SIZE;
    DMA_InitStructure.DMA_MemoryDataSize = AUDIO_MAL_DMA_MEM_DATA_SIZE; 
#ifdef AUDIO_MAL_MODE_NORMAL
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
#elif defined(AUDIO_MAL_MODE_CIRCULAR)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
#else
#error "AUDIO_MAL_MODE_NORMAL or AUDIO_MAL_MODE_CIRCULAR should be selected !!"
#endif /* AUDIO_MAL_MODE_NORMAL */  
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
    DMA_Init(AUDIO_MAL_DMA_STREAM, &DMA_InitStructure);  
    
    /* Enable the selected DMA interrupts (selected in "stm32f4_discovery_eval_audio_codec.h" defines) */
#ifdef AUDIO_MAL_DMA_IT_TC_EN
    DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_TC, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TC_EN */
#ifdef AUDIO_MAL_DMA_IT_HT_EN
    DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_HT, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
#ifdef AUDIO_MAL_DMA_IT_TE_EN
    DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_TE | DMA_IT_FE | DMA_IT_DME, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TE_EN */
  /* Enable the I2S DMA request */
    SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, ENABLE);    
    
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
        /* I2S DMA IRQ Channel configuration */
    NVIC_InitStructure.NVIC_IRQChannel = AUDIO_MAL_DMA_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EVAL_AUDIO_IRQ_PREPRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EVAL_AUDIO_IRQ_SUBRIO;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif     
}



/**
  * @brief  Configure the audio peripherals.
  * @param  OutputDevice: OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{    
    /* Perform low layer Codec initialization */
    if (Codec_Init(OutputDevice, VOLUME_CONVERT(Volume), AudioFreq) != 0)
    {
        Dprintf("CODEC Init Error\n");
        return 1;                
    }
    else
    {    
        /* I2S data transfer preparation:
        Prepare the Media to be used for the audio transfer from memory to I2S peripheral */
        Audio_MAL_Init();    
        /* Return 0 when all operations are OK */
        return 0;
    }
}

int WavePlayerInit(uint32_t AudioFreq)
{ 
    /* Initialize I2S interface */  
    EVAL_AUDIO_SetAudioInterface(AUDIO_INTERFACE_I2S); //DMA Setting Prepare 
    /* Initialize the Audio codec and all related peripherals (I2S, I2C, IOExpander, IOs...) */  
    EVAL_AUDIO_Init(OUTPUT_DEVICE_BOTH, volume, AudioFreq );    
    return 0;
}

/**
  * @brief  Start the audio Codec play feature.
  * @note   For this codec no Play options are required.
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Play(void)
{
    /* 
     No actions required on Codec level for play command
    */  

    /* Return communication control value */
    return 0;  
}

/**
  * @brief  Starts playing audio stream from the audio Media.
  * @param  None
  * @retval None
  */
void Audio_MAL_Play(uint32_t Addr, uint32_t Size)
{         
    /* Configure the buffer address and size */
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Addr;
    DMA_InitStructure.DMA_BufferSize = (uint32_t)Size/2;
    
    /* Configure the DMA Stream with the new parameters */
    DMA_Init(AUDIO_MAL_DMA_STREAM, &DMA_InitStructure);
    
    /* Enable the I2S DMA Stream*/
    DMA_Cmd(AUDIO_MAL_DMA_STREAM, ENABLE);   
  
    /* If the I2S peripheral is still not enabled, enable it */
    if ((CODEC_I2S->I2SCFGR & I2S_ENABLE_MASK) == 0)
    {
        I2S_Cmd(CODEC_I2S, ENABLE);
    }
}



/**
  * @brief  Starts playing audio stream from a data buffer for a determined size. 
  * @param  pBuffer: Pointer to the buffer 
  * @param  Size: Number of audio data BYTES.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size)
{
    /* Set the total number of data to be played (count in half-word) */
    AudioTotalSize = Size;

    /* Call the audio Codec Play function */
    Codec_Play();
  
    /* Update the Media layer and enable it for play */  
    Audio_MAL_Play((uint32_t)pBuffer, (uint32_t)(DMA_MAX(Size/4)));
  
    /* Update the remaining number of data to be played */
    AudioRemSize = (Size/2) - DMA_MAX(AudioTotalSize);
  
    /* Update the current audio pointer position */
    CurrentPos = pBuffer + DMA_MAX(AudioTotalSize);
  
    return 0;
}


/**
* @brief  Play wave file from internal Flash
* @param  None
* @retval None
*/
uint32_t AudioFlashPlay(uint16_t* pBuffer, uint32_t FullSize, uint32_t StartAdd)
{ 
    EVAL_AUDIO_Play((uint16_t*)pBuffer, (FullSize - StartAdd));
    return 0;
}

/**
  * @brief  Enables or disables the mute feature on the audio codec.
  * @param  Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Mute(uint32_t Cmd)
{
    uint32_t counter = 0;  
  
    /* Set the Mute mode */
    if (Cmd == AUDIO_MUTE_ON)
    {
        counter += Codec_WriteRegister(0x04, 0xFF);
    }
    else /* AUDIO_MUTE_OFF Disable the Mute */
    {
        counter += Codec_WriteRegister(0x04, OutputDev);
    }  
    return counter; 
}


/**
  * @brief  Pauses and resumes playing on the audio codec.
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *         from 0) to resume. 
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_PauseResume(uint32_t Cmd)
{
    uint32_t counter = 0;   
  
    /* Pause the audio file playing */
    if (Cmd == AUDIO_PAUSE)
    { 
        /* Mute the output first */
        counter += Codec_Mute(AUDIO_MUTE_ON);

        /* Put the Codec in Power save mode */    
        counter += Codec_WriteRegister(0x02, 0x01);    
    }
    else /* AUDIO_RESUME */
    {
        /* Unmute the output first */
        counter += Codec_Mute(AUDIO_MUTE_OFF);
    
        counter += Codec_WriteRegister(0x04, OutputDev);
    
        /* Exit the Power save mode */
        counter += Codec_WriteRegister(0x02, 0x9E); 
    }
    return counter;
}

/**
  * @brief  Pauses or Resumes the audio stream playing from the Media.
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *              from 0) to resume. 
  * @param  Addr: Address from/at which the audio stream should resume/pause.
  * @retval None
  */
static void Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr)
{
    /* Pause the audio file playing */
    if (Cmd == AUDIO_PAUSE)
    {   
        /* Disable the I2S DMA request */
        SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, DISABLE);

        /* Pause the I2S DMA Stream 
        Note. For the STM32F40x devices, the DMA implements a pause feature, 
              by disabling the stream, all configuration is preserved and data 
              transfer is paused till the next enable of the stream.
              This feature is not available on STM32F40x devices. */
        DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);
    }
    else /* AUDIO_RESUME */
    {
        /* Enable the I2S DMA request */
        SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, ENABLE);
  
        /* Resume the I2S DMA Stream 
        Note. For the STM32F40x devices, the DMA implements a pause feature, 
              by disabling the stream, all configuration is preserved and data 
              transfer is paused till the next enable of the stream.
              This feature is not available on STM32F40x devices. */
        DMA_Cmd(AUDIO_MAL_DMA_STREAM, ENABLE);
    
        /* If the I2S peripheral is still not enabled, enable it */
        if ((CODEC_I2S->I2SCFGR & I2S_ENABLE_MASK) == 0)
        {
            I2S_Cmd(CODEC_I2S, ENABLE);
        }    
    } 
}


/**
  * @brief  This function Pauses or Resumes the audio file stream. In case
  *         of using DMA, the DMA Pause feature is used. In all cases the I2S 
  *         peripheral is disabled. 
  * 
  * @WARNING When calling EVAL_AUDIO_PauseResume() function for pause, only
  *          this function should be called for resume (use of EVAL_AUDIO_Play() 
  *          function for resume could lead to unexpected behavior).
  * 
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *         from 0) to resume. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd)
{    
    /* Call the Audio Codec Pause/Resume function */
    if (Codec_PauseResume(Cmd) != 0)
    {
        return 1;
    }
    else
    {
        /* Call the Media layer pause/resume function */
        Audio_MAL_PauseResume(Cmd, 0);
    
        /* Return 0 if all operations are OK */
        return 0;
    }
}

/**
  * @brief  Pause or Resume a played wave
  * @param  state: if it is equal to 0 pause Playing else resume playing
  * @retval None
  */
void WavePlayerPauseResume(uint8_t state)
{ 
    EVAL_AUDIO_PauseResume(state);   
}


/**
  * @brief  Stops audio Codec playing. It powers down the codec.
  * @param  CodecPdwnMode: selects the  power down mode.
  *          - CODEC_PDWN_SW: only mutes the audio codec. When resuming from this 
  *                           mode the codec keeps the previous initialization
  *                           (no need to re-Initialize the codec registers).
  *          - CODEC_PDWN_HW: Physically power down the codec. When resuming from this
  *                           mode, the codec is set to default configuration 
  *                           (user should re-Initialize the codec in order to 
  *                            play again the audio stream).
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Stop(uint32_t CodecPdwnMode)
{
    uint32_t counter = 0;   

    /* Mute the output first */
    Codec_Mute(AUDIO_MUTE_ON);
  
    if (CodecPdwnMode == CODEC_PDWN_SW)
    {    
        /* Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
        counter += Codec_WriteRegister(0x02, 0x9F);
    }
    else /* CODEC_PDWN_HW */
    { 
        /* Power down the DAC components */
        counter += Codec_WriteRegister(0x02, 0x9F);
    
        /* Wait at least 100us */
        Delay_1(0xFFF);
    
        /* Reset The pin */
        GPIO_WriteBit(AUDIO_RESET_GPIO, AUDIO_RESET_PIN, Bit_RESET);
    }  
    return counter;    
}

/**
  * @brief  Stops audio stream playing on the used Media.
  * @param  None
  * @retval None
  */
static void Audio_MAL_Stop(void)
{   
    /* Stop the Transfer on the I2S side: Stop and disable the DMA stream */
    DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);

    /* Clear all the DMA flags for the next transfer */
    DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC |AUDIO_MAL_DMA_FLAG_HT | \
                                  AUDIO_MAL_DMA_FLAG_FE | AUDIO_MAL_DMA_FLAG_TE);
  
    /*  
           The I2S DMA requests are not disabled here.
                                                            */
  
    /* In all modes, disable the I2S peripheral */
    I2S_Cmd(CODEC_I2S, DISABLE);
}

/**
  * @brief  Stops audio playing and Power down the Audio Codec. 
  * @param  Option: could be one of the following parameters 
  *           - CODEC_PDWN_SW: for software power off (by writing registers). 
  *                            Then no need to reconfigure the Codec after power on.
  *           - CODEC_PDWN_HW: completely shut down the codec (physically). 
  *                            Then need to reconfigure the Codec after power on.  
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Stop(uint32_t Option)
{
    /* Call Audio Codec Stop function */
    if (Codec_Stop(Option) != 0)
    {
        return 1;
    }
    else
    {
        /* Call Media layer Stop function */
        Audio_MAL_Stop();
    
        /* Update the remaining data number */
        AudioRemSize = AudioTotalSize;    
    
        /* Return 0 when all operations are correctly done */
        return 0;
    }
}

/**
  * @brief  Stop playing wave
  * @param  None
  * @retval None
  */
void WavePlayerStop(void)
{ 
    EVAL_AUDIO_Stop(CODEC_PDWN_SW);
}
 

//내부 플래시에 들어있는 데이터를 이용해서 오디오를 재생한다.
void WavePlayBack_Flash(uint32_t AudioFreq)
{
    char    sd;
    /* Start playing */
    AudioPlayStart = 1;
    RepeatState =0;
    CurrAudioInterface = AUDIO_INTERFACE_I2S;
    
    Dprintf("WavePlayBack_Flash::Codec Init\n");
    /* Initialize wave player (Codec, DMA, I2C) */
    WavePlayerInit(AudioFreq); 
    /* Play on */
    Dprintf("WavePlayBack_Flash::Audio Play Start\n");
    //AudioFlashPlay((uint16_t*)(0x40000000 + AUIDO_START_ADDRESS), AUDIO_FILE_SZE, AUIDO_START_ADDRESS);
    AudioFlashPlay((uint16_t*)(0x40000000 + AUIDO_START_ADDRESS), 2000, AUIDO_START_ADDRESS);
    /*
    while(1)
    {
        sd = Dgetch();
        if(sd=='1')
        {
            Dprintf("WavePlayBack_Flash::WavePlayerPauseResume=0\n");
            WavePlayerPauseResume(0);
        }
        else if(sd=='2')
        {
            Dprintf("WavePlayBack_Flash::WavePlayerPauseResume=1\n");
            WavePlayerPauseResume(1);
        }
        else if(sd=='3')
        {
            Dprintf("WavePlayBack_Flash::WavePlayerStop\n");
            WavePlayerStop();            
        }
    }    
    */
}


void AudioTest(void)
{
	U8	ckey, data, addr;

    /*
	Dprintf("AudioTest:: Verify Reg[0x02]=%02x\n",Codec_ReadRegister(0x02));
	Dprintf("AudioTest:: Verify Reg[0x04]=%02x\n",Codec_ReadRegister(0x04));
	Dprintf("AudioTest:: Verify Reg[0x05]=%02x\n",Codec_ReadRegister(0x05));
	Dprintf("AudioTest:: Verify Reg[0x06]=%02x\n",Codec_ReadRegister(0x06));
	Dprintf("AudioTest:: Verify Reg[0x0D]=%02x\n",Codec_ReadRegister(0x0D));
	Dprintf("AudioTest:: Verify Reg[0x0E]=%02x\n",Codec_ReadRegister(0x0E));
	Dprintf("AudioTest:: Verify Reg[0x0F]=%02x\n",Codec_ReadRegister(0x0F));
	Dprintf("AudioTest:: Verify Reg[0x14]=%02x\n",Codec_ReadRegister(0x14));
	Dprintf("AudioTest:: Verify Reg[0x15]=%02x\n",Codec_ReadRegister(0x15));
	Dprintf("AudioTest:: Verify Reg[0x1A]=%02x\n",Codec_ReadRegister(0x1a));
	Dprintf("AudioTest:: Verify Reg[0x1B]=%02x\n",Codec_ReadRegister(0x1b));
	Dprintf("AudioTest:: Verify Reg[0x1F]=%02x\n",Codec_ReadRegister(0x1f));
	Dprintf("AudioTest:: Verify Reg[0x22]=%02x\n",Codec_ReadRegister(0x22));
	Dprintf("AudioTest:: Verify Reg[0x23]=%02x\n",Codec_ReadRegister(0x23));
	Dprintf("AudioTest:: Verify Reg[0x27]=%02x\n",Codec_ReadRegister(0x27));

	Dprintf("AudioTest:: Verify Reg[0x34]=%02x\n",Codec_ReadRegister(0x34));

	Dprintf("AudioTest:: Verify Reg[0x08]=%02x\n",Codec_ReadRegister(0x08));
	Dprintf("AudioTest:: Verify Reg[0x09]=%02x\n",Codec_ReadRegister(0x09));
    */
    
	while(1)
	{
		if(Dstatus())
		{
			ckey = Dgetch();
			if(ckey=='w')
			{
				Dprintf("Address:");
				addr = DgetIntNum();
				Dprintf("\tData:");
				data = DgetIntNum();
				Codec_WriteRegister(addr, data);
			}
			else if(ckey=='r')
			{
				Dprintf("Address:");
				addr = DgetIntNum();
				data = Codec_ReadRegister(addr);
				Dprintf("Codec Addr:%02x=%02x\n", addr, data);
			}
		}		
	}
}

void I2S_Test(void)
{
    Dprintf("I2S_Test::Codec Test Start.................\n");
    WavePlayBack_Flash(I2S_AudioFreq_48k); 
    AudioTest();
}
