/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define	  YES	    1
#define      NO		0

#define	  ON	    1
#define      OFF	0

#define	  HIGH	1
#define      LOW	0
/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

//PA5: SPI1_CLK
//PA6: SPI1_MISO
//PA7: SPI1_MOSI
void SPI1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;    

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable the SPI clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    /* GPIOG Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* SPI GPIO Configuration --------------------------------------------------*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;

    /* SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   //PA5
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI  MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;  //PA7
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //MISO Pin
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;  //PA6
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* Connect SPI pins to AF5 */  
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    /* SPI configuration -------------------------------------------------------*/
    SPI_I2S_DeInit(SPI1);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // [84 / 128 = ]
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_Init(SPI1, &SPI_InitStructure);
    /* The Data transfer is performed in the SPI interrupt routine */
  	/* Enable the SPI peripheral */
  	SPI_Cmd(SPI1, ENABLE);  
}

uint8_t SPI1_Send(uint8_t data)
{
    uint8_t rdata;
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, data);           
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));                    
    rdata = SPI_I2S_ReceiveData(SPI1);   
    return(rdata);    
}

void SPI1_CS(int s)
{
    if(s==1)
    {
        //SPI_CS ->High
        GPIOA->BSRRL = GPIO_Pin_4;        
    }
    else
    {
        //SPI_CS ->Low
        GPIOA->BSRRH = GPIO_Pin_4;        
    }
}

void AudioDelay(__IO uint32_t nTime) //1 ->100uSec
{ 

	//1 = 100us
	//2 = 202us
	//3 = 304us
	//4 = 404us
	//5 = 504us
	//6 = 604us 
	//7 = 700us
	//8 = 800us
	//9 = 900us
	//10 = 1010us
	int i;
	int count = 4240 * nTime;
	
	for(i=0; i <count; i++);	
}

void VoiceChip_Reset(void)
{
	GPIOB->BSRRH =GPIO_Pin_12; //Low
	Delay(1);
	GPIOB->BSRRL =GPIO_Pin_12; //High
}

#define     DELAY500    7

void Audio_Init(void)
{
    //D Command
     SPI1_CS(0);      AudioDelay(1);  uint8_t data1 = SPI1_Send(0x44);    AudioDelay(DELAY500);   data1 = SPI1_Send(0x15); AudioDelay(DELAY500);   data1 = SPI1_Send(0x00);    AudioDelay(DELAY500);   SPI1_CS(1);    AudioDelay(DELAY500);
    //K Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x4B);    AudioDelay(DELAY500);   data1 = SPI1_Send(0xFE); AudioDelay(DELAY500);   data1 = SPI1_Send(0xE1);    AudioDelay(DELAY500);   SPI1_CS(1);    AudioDelay(DELAY500);
    //O Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x4F);    AudioDelay(DELAY500);   data1 = SPI1_Send(0x84); AudioDelay(DELAY500);   data1 = SPI1_Send(0xFF);    AudioDelay(DELAY500);    SPI1_CS(1);    AudioDelay(DELAY500);
    //V Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x56);    AudioDelay(DELAY500);   data1 = SPI1_Send(0x06); AudioDelay(DELAY500);   data1 = SPI1_Send(0x00);    AudioDelay(DELAY500);   SPI1_CS(1);    AudioDelay(DELAY500);
    //M Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x4D);    AudioDelay(DELAY500);   data1 = SPI1_Send(0x7F); AudioDelay(DELAY500);   data1 = SPI1_Send(0x00);    AudioDelay(DELAY500);    SPI1_CS(1);    AudioDelay(DELAY500);
    //N Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x4E);    AudioDelay(DELAY500);   data1 = SPI1_Send(0x7F); AudioDelay(DELAY500);   data1 = SPI1_Send(0x00);    AudioDelay(DELAY500);   SPI1_CS(1);    AudioDelay(DELAY500);
    //O Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x4F);    AudioDelay(DELAY500);   data1 = SPI1_Send(0x08); AudioDelay(DELAY500);   data1 = SPI1_Send(0x02);    AudioDelay(DELAY500);    SPI1_CS(1);    AudioDelay(DELAY500);
    //O Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x4F);    AudioDelay(DELAY500);   data1 = SPI1_Send(0xD9); AudioDelay(DELAY500);   data1 = SPI1_Send(0x02);    AudioDelay(DELAY500);    SPI1_CS(1);    AudioDelay(DELAY500);
    //C Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x43);    AudioDelay(DELAY500);   data1 = SPI1_Send(0x05); AudioDelay(DELAY500);   data1 = SPI1_Send(0x00);    AudioDelay(DELAY500);   SPI1_CS(1);    AudioDelay(DELAY500);
    //C Command
     SPI1_CS(0);      AudioDelay(1);   data1 = SPI1_Send(0x48);    AudioDelay(DELAY500);   data1 = SPI1_Send(0x01); AudioDelay(DELAY500);   data1 = SPI1_Send(0x00);    AudioDelay(DELAY500);   SPI1_CS(1);    AudioDelay(DELAY500);
}

void AudioPlay(uint8_t bank)
{
//    uint8_t    data1;    
    SPI1_CS(0);      
    AudioDelay(1);   
    uint8_t data1 = SPI1_Send(0x50);    
    AudioDelay(DELAY500);   
    data1 = SPI1_Send(bank); 
    AudioDelay(DELAY500);   
    data1 = SPI1_Send(0x00);    
    AudioDelay(DELAY500);   
    SPI1_CS(1);    
}

//현재 재생 중인 음성을 정지합니다.
void SpeexAudioStop()
{
//    uint8_t    data1;    
    SPI1_CS(0);      
    AudioDelay(1);   
    uint8_t data1 = SPI1_Send(0x53);    
    AudioDelay(DELAY500);   
    data1 = SPI1_Send(0x01); 
    AudioDelay(DELAY500);   
    data1 = SPI1_Send(0x00);    
    AudioDelay(DELAY500);   
    SPI1_CS(1);       
}

//현재 재생 중인 음성을 정지합니다.
void ADPCMAudioStop()
{
    //uint8_t    data1;    
    SPI1_CS(0);      
    AudioDelay(1);   
    uint8_t data1 = SPI1_Send(0x53);    
    AudioDelay(DELAY500);   
    data1 = SPI1_Send(0x02); 
    AudioDelay(DELAY500);   
    data1 = SPI1_Send(0x00);    
    AudioDelay(DELAY500);   
    SPI1_CS(1);       
}

//현재 재생 중 이거나 예약된 모든 음성 재생을 취소합니다.
void AudioStop(void)
{
    //uint8_t    data1;    
    SPI1_CS(0);      
    AudioDelay(1);   
    uint8_t data1 = SPI1_Send(0x58);    
    AudioDelay(DELAY500);   
    data1 = SPI1_Send(0x00); 
    AudioDelay(DELAY500);   
    data1 = SPI1_Send(0x00);    
    AudioDelay(DELAY500);   
    SPI1_CS(1);       
}

static volatile uint16_t	capture=0;
static volatile int gFreeTime=0;

void TIM5_IRQHandler(void)
{
  	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
  	{
        if(gFreeTime !=0x00)
        {
            gFreeTime--;
        }
        //GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));   		
		capture = TIM_GetCapture1(TIM5);
    	TIM_SetCompare1(TIM5, capture + 100);
    	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
    }    
}

void Timer5_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	uint16_t PrescalerValue = 0;

  	NVIC_InitTypeDef NVIC_InitStructure;

	/* TIM5 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	

  	/* Enable the TIM3 gloabal Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0xF; //7
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0xF; //
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

  	/* Compute the prescaler value */
  	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000000) - 1; //500KHZ

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period = 65535;
  	TIM_TimeBaseStructure.TIM_Prescaler = 0;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  	/* Prescaler configuration */
  	TIM_PrescalerConfig(TIM5, PrescalerValue, TIM_PSCReloadMode_Immediate);

	
  	// Output Compare Timing Mode configuration: Channel1 
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = 100;    //10KHZ
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OC1Init(TIM5, &TIM_OCInitStructure);
  	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Disable);
   	// TIM Interrupts enable 
  	TIM_ITConfig(TIM5, TIM_IT_CC1 , ENABLE);
    
	// TIM5 enable counter 
  	TIM_Cmd(TIM5, ENABLE);
}

/*
void ProgramMode_Port(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    
	`//GPIOG Peripheral clock enable 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
}
*/

void BSP_Voice_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    /* GPIOG Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIOA->BSRRL =GPIO_Pin_4;	//CS High	
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	GPIOB->BSRRL =GPIO_Pin_12;	//Reset High	
	
	VoiceChip_Reset();
	Delay(10);
	//Timer5_Config();		//for 100uSec
	SPI1_Config();		//SPI
	Audio_Init();
}


void AudioTest_Help(void)
{
	Dprintf("=========Voice Chip Test==========\n");
	Dprintf("1. Audio Play\n");
	Dprintf("2. Audio Stop\n");
	
}

void VoiceChip_Audio_Test(void)
{
	char	sd;
	int	num=0;
	
	BSP_Voice_Init();
	
	AudioTest_Help();
	while(1)
	{
		sd = Dgetch();
		if(sd=='1')
		{
			Dprintf("\n재생할 뱅크번호를 입력하시오:");
			num = DgetIntNum();
			AudioPlay(num);
			Dprintf("\n");
		}
		else if(sd=='2')
		{
			Dprintf("Audio Stop Command\n");
			AudioStop();
		}
	}	
}





