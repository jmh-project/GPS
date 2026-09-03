/**
  ******************************************************************************
  * @file       	MemoryManager.c
  * @author  	(주)자이언텍 기술개발지원팀
  * @version 	V0.1
  * @date    	2014년 12월19일
  * @brief    	이파일은  발효기콘트롤러로 사용할 메인화면프로그램이다.
                
  *          
  * @attention
  * 이파일은 자이언텍 지적재산권이므로 타사에 소스를 제공해서는 안됩니다. 
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc.</center></h2>
  * @warning  
  * @verbatim
  *  Edit History
  * @endverbatim  
  ******************************************************************************  
  */
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"
#include "Windows.h"
#include "MemoryManager.h"
#include "stm32f4xx_fmc.h"
#include "DataLog.h"
/* Private typedef -----------------------------------------------------------*/
#ifdef PSRAM_USE
#define		NAND_MBR_ADDRESS_RESOURCE	    		0x64000000
#define		SOURCE_BMP_ADDR_RESOURCE				0x64020000
#define		CONV_BMP_ADDR_RESOURCE	    			0x643E0000
//#define		DIR_ADDR_RESOURCE			    		0x645A2000	// 2MB Allocation
#endif	 
// 아래의 설정은 VRAM 을 사용할때 사용한다.
#ifdef VRAM_USE
#define		NAND_MBR_ADDRESS_RESOURCE				0x6B1B2000
#define		SOURCE_BMP_ADDR_RESOURCE		0x6ABF2000
#define		CONV_BMP_ADDR_RESOURCE					0x6AED2000
//#define		DIR_ADDR_RESOURCE						0x6B7F2000	// 2MB Allocation
#endif
	 
#define 	DMA_STREAM               DMA2_Stream0
#define	 	DMA_CHANNEL              DMA_Channel_0
#define 	DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define 	DMA_STREAM_IRQ           DMA2_Stream0_IRQn
#define 	DMA_IT_TCIF              DMA_IT_TCIF0
#define 	DMA_STREAM_IRQHANDLER    DMA2_Stream0_IRQHandler

#define 	BUFFER_SIZE              		32
#define 	TIMEOUT_MAX              	10000 /* Maximum timeout value */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay;
volatile int g_Timer_msec;
volatile int  g_Wifi_Time;
volatile int  g_Wifi_TimeOut;
volatile int g_Touch_Time; //g_TouchKeyPad_Time
volatile int g_TouchKeyPad_Time; //g_TouchKeyPad_Time
volatile int g_Touch_Enable;
volatile int gUSB_Select=0;

extern volatile int gCardTime;
extern volatile U16 	g_Color;
extern __IO uint32_t nowPlay;
     
extern __IO U32 gSoundSize, gSoundSize1, gSoundSize2, gSoundSize3;
extern __IO U32 gSoundCnt, gSoundCnt1, gSoundCnt2, gSoundCnt3;
extern volatile S16 gTouch, gTouchKeyPad;

extern volatile U32	g_BadBlock[100];
extern volatile int	g_BadBlockCount;
extern volatile int	OS_START;
extern volatile int	g_XMIN;		// X축 보정시좌표 최소점 mV
extern volatile int 	g_XMAX;		// X축 보정시좌표 최대점 mV
extern volatile int	g_YMIN;		// Y축 보정시좌표 최소점 mV
extern volatile int	g_YMAX;		// Y축 보정시좌표 최대점 mV

extern volatile int 	gTouchX, gTouchY, gPress;
extern volatile int	g_TouchCalStatus;
extern volatile int 	g_TouchAverX[5];
extern volatile int 	g_TouchAverY[5];
/* Private function prototypes -----------------------------------------------*/
void BSP_SoundVolumeControl_PortInit( void );
int   BSP_AudioSoundVolume(U8 vol);
void LCD_ON(void);
void SmartX_UartInit(void);
void SM718_2DInit(void);
void TeecoBmpDMA_Config(void);
void DS1302PortInit(void);
void DS1302Init(void);
void BSP_ScreenClear(void);
void FillRect2D(int xs, int ys, int xe, int ye);
void TimingDelay_Decrement(void);
void Voice_Port_Init(void);
void VoiceChipReset(void);
void Voice_Init(void);
void NandMBRCashe(void);
void USB_Lib_Load(void);
void Uart4_Config(void);
/* Private functions ---------------------------------------------------------*/
void Delay(__IO uint32_t nTime)
{ 
  	TimingDelay = nTime;
  	while(TimingDelay != 0);
}

//SD Card 가 연결되어 있을때 초기화를 시킨다.
SD_Error DiskInit(void)
{
	SD_Error Status = SD_OK;
	
	Status = Status;

	if((Status = SD_Init()) != SD_OK)
  	{
    	Dprintf("SD Card Init Error\n");
		return(SD_ERROR);
  	}
	return(SD_OK);
}

int TouchDataLoad(void)
{
	int		i, data;
	U8		*VRAM_FileBuf = (U8 *)(NAND_MBR_ADDRESS_RESOURCE);
	char	buf[16];
	
	VRAM_FileBuf = (U8 *)(NAND_MBR_ADDRESS_RESOURCE+426);
	for(i=0; i<4; i++) buf[i] =  *VRAM_FileBuf++;
	buf[i]=0;
	data = atoi(buf);
	if(data!=7777)  
	{
		g_TouchCalStatus=NO;
		return(0);
	}

	VRAM_FileBuf = (U8 *)(NAND_MBR_ADDRESS_RESOURCE+410);
	for(i=0; i<4; i++) buf[i] =  *VRAM_FileBuf++;
	buf[i]=0;
	g_XMIN = atoi(buf);

	for(i=0; i<4; i++) buf[i] =  *VRAM_FileBuf++;
	buf[i]=0;
	g_YMIN = atoi(buf);

	for(i=0; i<4; i++) buf[i] =  *VRAM_FileBuf++;
	buf[i]=0;
	g_XMAX = atoi(buf);

	for(i=0; i<4; i++) buf[i] =  *VRAM_FileBuf++;
	buf[i]=0;
	g_YMAX = atoi(buf);

	g_TouchCalStatus = YES;
	//Dprintf("\nTouch Data %d %d %d %d\n", g_XMIN, g_YMIN, g_XMAX, g_YMAX);		
	return(1);
}


#ifdef ECD_PH_ENABLE
volatile int gModbusReceiveTimer = 0;
#endif
#ifdef WS501_ENABLE
    extern int gUart2StatusCodeTimer;
#endif
#ifdef SONAR_ENABLE
    extern int gUart4StatusCodeTimer;
#endif
#if defined (REMOTE_TERMINAL_UNIT) || defined (FARM_PONTOON_ENABLE)
    int gUart5StatusCodeTimer=0;
#endif

#if defined(WINCH_ENABLE)     
extern volatile int gWitchControlTImer;
extern volatile int gStopRpmStartFlag;
#endif 


void SystemProcessFunction(void) {
	#if defined(EXO_ENABLE)
	gv.findDelayTimeCnt++;
    #endif
	if(gv.usbSaveWaitFlag==0) { //USB 다운로드시에는 타이머가 증가하지 않는다.
        gv.systemWatshTimerCnt++;//DataLog_Hardware_IOProcess.c 에서 초기화한다.
        if(gv.systemWatshTimerCnt > 10000) {//10초
            //비상정지시 필요한 함수를 여기에 추가한다.
            DataLog_MessageBox_Window("CPU", "시스템 점검을 시작합니다.");
            EventMessage("시스템 점검을 시작합니다.");
            NVIC_SystemReset();
            while(1);
        }
//        gv.screenWatchTimerCount++;//5분
//        if(gv.screenWatchTimerCount>300000) { //DispatchMessage, TouchProcessing, USB
//            DataLog_MessageBox_Window("SM718", "[INTT] SM718 복구명령실행");
//            EventMessage("[INTT] TOUCH EVENT SM718 복구명령실행");
//            NVIC_SystemReset();
//            while(1);
//        }
        gv.buttonWatchTimerCount++;//시간이 정상적으로 동작하지만 버튼 터치 동작이 안되는 경우 프로그램 리셋을 한다. 
        if(gv.buttonWatchTimerCount>300000) {//SButtonDraw, TouchProcessing, NButtonDraw, USB
            DataLog_MessageBox_Window("SM718", "[INTT] SM718 BUTTON 복구명령실행");
            EventMessage("[INTT] SM718 BUTTON 복구명령실행");
            NVIC_SystemReset();
            while(1);
        }
    }    
    else if(gv.usbSaveWaitFlag==1 || gv.tmsStatusCode[USART_1]==3) { //USB가 동작하는 경우 모든 타이머값이 초기화 된다.
        gv.systemWatshTimerCnt=0;
//        gv.screenWatchTimerCount=0;
        gv.buttonWatchTimerCount=0;
        GPIO_ToggleBits(GPIOB,GPIO_Pin_0);//USB 가 장시간 동작시에는 여기서 처리한다.
    }
}
void TimingDelay_Decrement(void)//1ms
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
	g_Timer_msec++;    
	g_Touch_Time++;
    g_TouchKeyPad_Time++;    

    #if defined (SAMAN_ENABLE)
	flash.timerCnt200ms++;
	#endif
    SystemProcessFunction();
}

void TeecoBmpDMA_Config(void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;
  	DMA_InitTypeDef  DMA_InitStructure;
  	__IO uint32_t    Timeout = TIMEOUT_MAX;
    
  	/* Enable DMA clock */
  	RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE); //RCC_AHB1Periph_DMA2
  
  	/* Reset DMA Stream registers (for debug purpose) */
  	DMA_DeInit(DMA_STREAM); // DMA2_Stream0

  	/* Check if the DMA Stream is disabled before enabling it.
     Note that this step is useful when the same Stream is used multiple times:
     enabled, then disabled then re-enabled... In this case, the DMA Stream disable
     will be effective only at the end of the ongoing data transfer and it will 
     not be possible to re-configure it before making sure that the Enable bit 
     has been cleared by hardware. If the Stream is used only once, this step might 
     be bypassed. */
  	while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE)//DMA2_Stream0
  	{
  	}
  
  	/* Configure DMA Stream */
  	DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  //DMA_Channel_0
  	DMA_InitStructure.DMA_PeripheralBaseAddr = 0x68000000;
  	DMA_InitStructure.DMA_Memory0BaseAddr = 0x68000000;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
	DMA_InitStructure.DMA_BufferSize = (uint32_t)1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_STREAM, &DMA_InitStructure);
	
	/* Enable DMA Stream Transfer Complete interrupt */
	DMA_ITConfig(DMA_STREAM, DMA_IT_TC, ENABLE);
	
	/* DMA Stream enable */
	DMA_Cmd(DMA_STREAM, ENABLE);
	
	/* Check if the DMA Stream has been effectively enabled.
	 The DMA Stream Enable bit is cleared immediately by hardware if there is an 
	 error in the configuration parameters and the transfer is no started (ie. when
	 wrong FIFO threshold is configured ...) */

	Timeout = TIMEOUT_MAX;
	while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0))
	{
	}
   
	if (Timeout == 0)
	{
		while (1)
		{
			Dprintf("DMA ERROR\n");
			break;
		}
	}
	
	/* Enable the DMA Stream IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = DMA_STREAM_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);     
    
}

void DMA_Config(U32 src, U32 dest, U32 size)
{
//  	NVIC_InitTypeDef NVIC_InitStructure;
  	DMA_InitTypeDef  DMA_InitStructure;
  	__IO uint32_t    Timeout = TIMEOUT_MAX;
    
  	/* Enable DMA clock */
  	//RCC_AHB1PeriphClockCmd(DMA_STREAM_CLOCK, ENABLE);
  
  	/* Reset DMA Stream registers (for debug purpose) */
  	//DMA_DeInit(DMA_STREAM);

  	/* Check if the DMA Stream is disabled before enabling it.
     Note that this step is useful when the same Stream is used multiple times:
     enabled, then disabled then re-enabled... In this case, the DMA Stream disable
     will be effective only at the end of the ongoing data transfer and it will 
     not be possible to re-configure it before making sure that the Enable bit 
     has been cleared by hardware. If the Stream is used only once, this step might 
     be bypassed. */
  	//while (DMA_GetCmdStatus(DMA_STREAM) != DISABLE)
  	//{
  	//}
  
  	/* Configure DMA Stream */
  	DMA_InitStructure.DMA_Channel = DMA_CHANNEL;  
  	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)src;
  	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)dest;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;
	DMA_InitStructure.DMA_BufferSize = (uint32_t)size;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA_STREAM, &DMA_InitStructure);
	
	/* Enable DMA Stream Transfer Complete interrupt */
	//DMA_ITConfig(DMA_STREAM, DMA_IT_TC, ENABLE);
	
	/* DMA Stream enable */
	DMA_Cmd(DMA_STREAM, ENABLE);
	
	/* Check if the DMA Stream has been effectively enabled.
	 The DMA Stream Enable bit is cleared immediately by hardware if there is an 
	 error in the configuration parameters and the transfer is no started (ie. when
	 wrong FIFO threshold is configured ...) */

	Timeout = TIMEOUT_MAX;
	while ((DMA_GetCmdStatus(DMA_STREAM) != ENABLE) && (Timeout-- > 0))
	{
	}
   
	if (Timeout == 0)
	{
		while (1)
		{
		}
	}
	
	/* Enable the DMA Stream IRQ Channel */
	//NVIC_InitStructure.NVIC_IRQChannel = DMA_STREAM_IRQ;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);     
}

//디버깅용 통신 포트
void COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  	GPIO_InitTypeDef GPIO_InitStructure;

  	/* Enable GPIO clock */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  	if (COM == COM1)
  	{
    	/* Enable UART clock */
    	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  	}

  	/* Connect PXx to USARTx_Tx*/
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

  	/* Connect PXx to USARTx_Rx*/
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

  	/* Configure USART Tx as alternate function  */
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/* Configure USART Rx as alternate function  */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  	/* USART configuration */
  	USART_Init(USART2, USART_InitStruct);
    
  	/* Enable USART */
  	USART_Cmd(USART2, ENABLE);
}

//디버깅용 통신포트
void BSP_UartInit(void)
{
	USART_InitTypeDef USART_InitStructure;

    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  	COMInit(COM1, &USART_InitStructure);
}

void BSP_Timer(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	uint16_t PrescalerValue = 0;

  	NVIC_InitTypeDef NVIC_InitStructure;

	/* TIM3 clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	

  	/* Enable the TIM3 gloabal Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5; //7
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);

  	/* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 6 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /6 MHz) - 1
                                              
    CC1 update rate = TIM3 counter clock / CCR1_Val(6000) = 1000.0 Hz
  ----------------------------------------------------------------------- */   

  	/* Compute the prescaler value */
  	PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000000) - 1; //1MHZ

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period = 65535;
  	TIM_TimeBaseStructure.TIM_Prescaler = 0;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  	/* Prescaler configuration */
  	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
	
  	// Output Compare Timing Mode configuration: Channel1 
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = 40000; //100000 / 50000 = 20 Hz(50msec)
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);
 
  	// TIM Interrupts enable 
  	TIM_ITConfig(TIM3, TIM_IT_CC1 , ENABLE);

/*************************************************************************/
	//윈도우용 타이머 설정 1msec
	// Output Compare Timing Mode configuration: Channel1 
	
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = 1000; //1Khz
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable); 
  	// TIM Interrupts enable 
  	TIM_ITConfig(TIM3, TIM_IT_CC2 , ENABLE);

/*************************************************************************/
	//IO 용 타이머 설정 1msec
	// Output Compare Timing Mode configuration: Channel1 
	
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = 1000; //1Khz
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
  	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Disable); 
  	// TIM Interrupts enable 
  	TIM_ITConfig(TIM3, TIM_IT_CC4 , ENABLE);

	// TIM3 enable counter 
  	TIM_Cmd(TIM3, ENABLE);
}

void NVIC_Configuration(void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;

  	/* Configure the NVIC Preemption Priority Bits */
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
  	NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_Init(&NVIC_InitStructure);  
}

void EXTILine2_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Enable GPIOG clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Configure PG15 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOI, &GPIO_InitStructure);
	
	/* Connect EXTI Line15 to PG15 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource2);
	
	/* Configure EXTI Line15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI15_10 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

#define SDRAM_BANK_ADDR     ((uint32_t)0xC0000000)
/* #define SDRAM_MEMORY_WIDTH   FMC_SDMemory_Width_8b  */
#define SDRAM_MEMORY_WIDTH    FMC_SDMemory_Width_16b 
//#define SDRAM_MEMORY_WIDTH    FMC_SDMemory_Width_32b  /* Default configuration used with LCD */
#define SDCLOCK_PERIOD    FMC_SDClock_Period_2        /* Default configuration used with LCD */
#define SDRAM_TIMEOUT     ((uint32_t)0xFFFF)

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200) 

void SDRAM_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOs clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | 
							RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG , ENABLE);

	/* GPIOD configuration */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC); 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
	                            GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	/* GPIOE configuration */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2 , GPIO_AF_FMC);   //PE2: A23
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5 , GPIO_AF_FMC);   //PE5 : A21
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6 , GPIO_AF_FMC);   //PE6 : A22

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
	                             GPIO_Pin_5  |  GPIO_Pin_6 | GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* GPIOF configuration */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource11 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15 , GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 | 
	                            GPIO_Pin_4  | GPIO_Pin_5  |  GPIO_Pin_11 |  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;      
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* GPIOG configuration */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource2 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource3 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource8 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource10 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource15 , GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 | 
	                            GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |  GPIO_Pin_12 |  GPIO_Pin_13|  GPIO_Pin_15;      
	
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* GPIOC configuration */
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource0 , GPIO_AF_FMC); 	//SDNWE
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource2 , GPIO_AF_FMC);	//SDNE0
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource3 , GPIO_AF_FMC);	//SDCKE0

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);      
}

/**
  * @brief  Executes the SDRAM memory initialization sequence. 
  * @param  None. 
  * @retval None.
  */
void SDRAM_InitSequence(void)
{
	FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
	uint32_t tmpr = 0;
	uint32_t timeout = SDRAM_TIMEOUT; 

	/* Step 3 --------------------------------------------------------------------*/
	/* Configure a clock configuration enable command */
	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_CLK_Enabled;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;
	/* Wait until the SDRAM controller is ready */ 
	while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
	{
	timeout--;
	}
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);  

	/* Step 4 --------------------------------------------------------------------*/
	/* Insert 100 ms delay */
	Delay(100);

	/* Step 5 --------------------------------------------------------------------*/
	/* Configure a PALL (precharge all) command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_PALL;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;

	/* Wait until the SDRAM controller is ready */  
	timeout = SDRAM_TIMEOUT; 
	while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
	{
	timeout--;
	}
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Step 6 --------------------------------------------------------------------*/
	/* Configure a Auto-Refresh command */ 
	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_AutoRefresh;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 8;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;

	/* Wait until the SDRAM controller is ready */ 
	timeout = SDRAM_TIMEOUT; 
	while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
	{
	timeout--;
	}
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Step 7 --------------------------------------------------------------------*/
	/* Program the external memory mode register */
	tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
				   SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
				   SDRAM_MODEREG_CAS_LATENCY_3           |
				   SDRAM_MODEREG_OPERATING_MODE_STANDARD |
				   SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	/* Configure a load Mode register command*/ 
	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_LoadMode;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_Command_Target_bank1;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = tmpr;

	/* Wait until the SDRAM controller is ready */ 
	timeout = SDRAM_TIMEOUT; 
	while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
	{
		timeout--;
	}
	/* Send the command */
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	/* Step 8 --------------------------------------------------------------------*/

	/* Set the refresh rate counter */
	/* (15.62 us x Freq) - 20 */
	/* Set the device refresh counter */
	FMC_SetRefreshCount(1539); //1539

	/* Wait until the SDRAM controller is ready */ 
	timeout = SDRAM_TIMEOUT; 
	while((FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET) && (timeout > 0))
	{
	timeout--;
	}
}


void SDRAM_Init(void)
{
	FMC_SDRAMInitTypeDef  FMC_SDRAMInitStructure;
	FMC_SDRAMTimingInitTypeDef  FMC_SDRAMTimingInitStructure; 

	/* GPIO configuration for FMC SDRAM bank */
	SDRAM_GPIOConfig();

	/* Enable FMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);

	/* FMC Configuration ---------------------------------------------------------*/
	/* FMC SDRAM Bank configuration */   
	/* Timing configuration for 90 Mhz of SD clock frequency (180Mhz/2) */
	/* TMRD: 2 Clock cycles */
	FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay    = 2;      
	/* TXSR: min=70ns (7x11.11ns) */
	FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 5;//7;
	/* TRAS: min=42ns (4x11.11ns) max=120k (ns) */
	FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime      = 4;
	/* TRC:  min=70 (7x11.11ns) */        
	FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay        =5;// 7;         
	/* TWR:  min=1+ 7ns (1+1x11.11ns) */
	FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime    = 2;      
	/* TRP:  20ns => 2x11.11ns */
	FMC_SDRAMTimingInitStructure.FMC_RPDelay              = 2;                
	/* TRCD: 20ns => 2x11.11ns */
	FMC_SDRAMTimingInitStructure.FMC_RCDDelay             = 2;

	/* FMC SDRAM control configuration */
	FMC_SDRAMInitStructure.FMC_Bank               = FMC_Bank1_SDRAM;
	/* Row addressing: [7:0] */
	FMC_SDRAMInitStructure.FMC_ColumnBitsNumber   = FMC_ColumnBits_Number_9b;
	/* Column addressing: [10:0] */
	FMC_SDRAMInitStructure.FMC_RowBitsNumber      = FMC_RowBits_Number_13b;
	FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth  = SDRAM_MEMORY_WIDTH;
	FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4;
	/* CL: Cas Latency = 3 clock cycles */
	FMC_SDRAMInitStructure.FMC_CASLatency         = FMC_CAS_Latency_3; 
	FMC_SDRAMInitStructure.FMC_WriteProtection    = FMC_Write_Protection_Disable;
	FMC_SDRAMInitStructure.FMC_SDClockPeriod      = SDCLOCK_PERIOD;  
	FMC_SDRAMInitStructure.FMC_ReadBurst          = FMC_Read_Burst_Enable;
	FMC_SDRAMInitStructure.FMC_ReadPipeDelay      = FMC_ReadPipe_Delay_0;
	FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct  = &FMC_SDRAMTimingInitStructure;

	/* FMC SDRAM bank initialization */
	FMC_SDRAMInit(&FMC_SDRAMInitStructure); 

	/* FMC SDRAM device initialization sequence */
	SDRAM_InitSequence(); 	
}

void SDRAM_WriteBuffer(uint8_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize)
{
	__IO uint32_t write_pointer = (uint32_t)uwWriteAddress;

	/* Disable write protection */
	FMC_SDRAMWriteProtectionConfig(FMC_Bank1_SDRAM, DISABLE);

	/* Wait until the SDRAM controller is ready */ 
	while(FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET)
	{
	}

	/* While there is data to write */
	for (; uwBufferSize != 0; uwBufferSize--) 
	{
		/* Transfer data to the memory */
		*(uint8_t *) (SDRAM_BANK_ADDR + write_pointer) = *pBuffer++;

		/* Increment the address*/
		write_pointer ++;
	}    
}

void SDRAM_ReadBuffer(uint8_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize)
{
	__IO uint32_t write_pointer = (uint32_t)uwReadAddress;

	/* Wait until the SDRAM controller is ready */ 
	while(FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET)
	{
	}

	/* Read data */
	for(; uwBufferSize != 0x00; uwBufferSize--)
	{
		*pBuffer++ = *(__IO uint8_t *)(SDRAM_BANK_ADDR + write_pointer );

		/* Increment the address*/
		write_pointer ++;
	} 
}



// FSMC Bank1 의 CS0, 1, 2, 3 을 초기화 시킨다.
void BSP_FSMC_Init(void)
{
	FMC_NORSRAMInitTypeDef  FMC_NORSRAMInitStructure;
	FMC_NORSRAMTimingInitTypeDef  p;
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	/* Enable GPIOs clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF |
	                     RCC_AHB1Periph_GPIOG, ENABLE);
	
	/* Enable FSMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE); 
  
	/*-- GPIOs Configuration -----------------------------------------------------*/
	/*
	 +-------------------+--------------------+------------------+------------------+
	 +                       SRAM pins assignment                                   +
	 +-------------------+--------------------+------------------+------------------+
	 | PD0  <-> FSMC_D2  | PE0  <-> FSMC_NBL0 | PF0  <-> FSMC_A0 | PG0 <-> FSMC_A10 | 
	 | PD1  <-> FSMC_D3  | PE1  <-> FSMC_NBL1 | PF1  <-> FSMC_A1 | PG1 <-> FSMC_A11 | 
	 | PD4  <-> FSMC_NOE | PE3  <-> FSMC_A19  | PF2  <-> FSMC_A2 | PG2 <-> FSMC_A12 | 
	 | PD5  <-> FSMC_NWE | PE4  <-> FSMC_A20  | PF3  <-> FSMC_A3 | PG3 <-> FSMC_A13 | 
	 | PD8  <-> FSMC_D13 | PE7  <-> FSMC_D4   | PF4  <-> FSMC_A4 | PG4 <-> FSMC_A14 | 
	 | PD9  <-> FSMC_D14 | PE8  <-> FSMC_D5   | PF5  <-> FSMC_A5 | PG5 <-> FSMC_A15 | 
	 | PD10 <-> FSMC_D15 | PE9  <-> FSMC_D6   | PF12 <-> FSMC_A6 | PG9 <-> FSMC_NE2 | 
	 | PD11 <-> FSMC_A16 | PE10 <-> FSMC_D7   | PF13 <-> FSMC_A7 |------------------+
	 | PD12 <-> FSMC_A17 | PE11 <-> FSMC_D8   | PF14 <-> FSMC_A8 | PG13 <-> A24
	 | PD13 <-> FSMC_A18 | PE12 <-> FSMC_D9   | PF15 <-> FSMC_A9 | PG10 <-> FSMC_NE3
	 | PD14 <-> FSMC_D0  | PE13 <-> FSMC_D10  |------------------+ PG6 <-> BUSY(NAND)
	 | PD15 <-> FSMC_D1  | PE14 <-> FSMC_D11  |					   PG12 <-> NE4
	 | PD6<->WAIT        | PE15 <-> FSMC_D12  |
	 +-------------------+--------------------+
	   PD7  <-> NE1		   PE5  <-> A21 
						   PE6	<-> A22
						   PE2	<-> A23
	*/
	/* GPIOD configuration */
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FMC);	
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FMC); 
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
	                            GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	/* GPIOE configuration */
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource0 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource1 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource2 , GPIO_AF_FMC);   //PE2: A23
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource3 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource4 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5 , GPIO_AF_FMC);   //PE5 : A21
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6 , GPIO_AF_FMC);   //PE6 : A22

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
	                             GPIO_Pin_5  |  GPIO_Pin_6 | GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
	                            GPIO_Pin_15;
	
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* GPIOF configuration */
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource13 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource14 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource15 , GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 | 
	                            GPIO_Pin_4  | GPIO_Pin_5  |  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;      
	
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* GPIOG configuration */
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource0 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource1 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource2 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource3 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource4 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource5 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource9 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource10 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource12 , GPIO_AF_FMC);
	GPIO_PinAFConfig(GPIOG, GPIO_PinSource13 , GPIO_AF_FMC);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3 | 
	                            GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_9 | GPIO_Pin_10 |  GPIO_Pin_12 |  GPIO_Pin_13;      
	
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	//NAND Ready Signal input Port
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/*-- NE2 SRAM  FSMC Configuration W5100A & 16C554 ------------------------------------------------------*/
	p.FMC_AddressSetupTime = 6;//7;//6;//3;
	p.FMC_AddressHoldTime = 2;//3;//2;//0;
	p.FMC_DataSetupTime = 8;//10;//8;//6;
	p.FMC_BusTurnAroundDuration = 1;
	p.FMC_CLKDivision = 0;
	p.FMC_DataLatency = 0;
	p.FMC_AccessMode = FMC_AccessMode_A;
	
	FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM2;
	FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
	FMC_NORSRAMInitStructure.FMC_MemoryType = FMC_MemoryType_PSRAM;
	FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_16b;
	FMC_NORSRAMInitStructure.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
	FMC_NORSRAMInitStructure.FMC_AsynchronousWait = FMC_AsynchronousWait_Disable;  
	FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
	FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;
	FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;
	FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Disable;
	FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WriteBurst = FMC_WriteBurst_Disable;
	FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &p;
	FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &p;
	FMC_NORSRAMInit(&FMC_NORSRAMInitStructure); 
	/*!< Enable FSMC Bank1_SRAM2 Bank */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM2, ENABLE); 
	
	/*-- GPU(SM718) FSMC Configuration ------------------------------------------------------*/
	p.FMC_AddressSetupTime = 6; 	//3 3 6
	p.FMC_AddressHoldTime = 2; 		//1 0 2
	p.FMC_DataSetupTime = 8; 		//7 6 8
	p.FMC_BusTurnAroundDuration = 1;
	p.FMC_CLKDivision = 0;
	p.FMC_DataLatency = 0;
	p.FMC_AccessMode = FSMC_AccessMode_A;

	FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM3;
	FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
	FMC_NORSRAMInitStructure.FMC_MemoryType = FMC_MemoryType_SRAM;
	FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_16b;
	FMC_NORSRAMInitStructure.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
	FMC_NORSRAMInitStructure.FMC_AsynchronousWait = FMC_AsynchronousWait_Enable;  
	FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
	FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_DuringWaitState;
	FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;
	FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Enable;
	FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WriteBurst = FMC_WriteBurst_Disable;
	FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &p;
	FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &p;
	
	FMC_NORSRAMInit(&FMC_NORSRAMInitStructure); 
	
	/*!< Enable FSMC Bank1_SRAM3 Bank */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM3, ENABLE); 
	
	/*--  NAND FSMC Configuration ------------------------------------------------------*/
    // 240Mhz로 동작시킬때는 타이밍을 6,2,8로 사용한다.
	p.FMC_AddressSetupTime = 3; //6
	p.FMC_AddressHoldTime = 0; //2
	p.FMC_DataSetupTime = 6; //8
	p.FMC_BusTurnAroundDuration = 1;
	p.FMC_CLKDivision = 0;
	p.FMC_DataLatency = 0;
	p.FMC_AccessMode = FSMC_AccessMode_A;

	FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM1;
	FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
	FMC_NORSRAMInitStructure.FMC_MemoryType = FMC_MemoryType_SRAM;
	FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_8b;
	FMC_NORSRAMInitStructure.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
	FMC_NORSRAMInitStructure.FMC_AsynchronousWait = FMC_AsynchronousWait_Enable;  
	FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
	FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_DuringWaitState;
	FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;
	FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Disable;
	FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WriteBurst = FMC_WriteBurst_Disable;
	FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &p;
	FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &p;
	
	FMC_NORSRAMInit(&FMC_NORSRAMInitStructure); 
	
	/*!< Enable FSMC Bank1_SRAM1 Bank */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM1, ENABLE); 

    /*-- EXT NE4   FSMC Configuration ------------------------------------------------------*/
	// UPS Monitor 사용시는 8비트로 사용한다.
	p.FMC_AddressSetupTime = 6; //5
	p.FMC_AddressHoldTime = 2;  //1
	p.FMC_DataSetupTime = 8;    //7
	p.FMC_BusTurnAroundDuration = 1;
	p.FMC_CLKDivision = 0;
	p.FMC_DataLatency = 0;
	p.FMC_AccessMode = FSMC_AccessMode_A;
	
	FMC_NORSRAMInitStructure.FMC_Bank = FMC_Bank1_NORSRAM4;
	FMC_NORSRAMInitStructure.FMC_DataAddressMux = FMC_DataAddressMux_Disable;
	FMC_NORSRAMInitStructure.FMC_MemoryType = FMC_MemoryType_PSRAM;
	FMC_NORSRAMInitStructure.FMC_MemoryDataWidth = FMC_NORSRAM_MemoryDataWidth_8b;
	FMC_NORSRAMInitStructure.FMC_BurstAccessMode = FMC_BurstAccessMode_Disable;
	FMC_NORSRAMInitStructure.FMC_AsynchronousWait = FMC_AsynchronousWait_Disable;  
	FMC_NORSRAMInitStructure.FMC_WaitSignalPolarity = FMC_WaitSignalPolarity_Low;
	FMC_NORSRAMInitStructure.FMC_WrapMode = FMC_WrapMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WaitSignalActive = FMC_WaitSignalActive_BeforeWaitState;
	FMC_NORSRAMInitStructure.FMC_WriteOperation = FMC_WriteOperation_Enable;
	FMC_NORSRAMInitStructure.FMC_WaitSignal = FMC_WaitSignal_Disable;
	FMC_NORSRAMInitStructure.FMC_ExtendedMode = FMC_ExtendedMode_Disable;
	FMC_NORSRAMInitStructure.FMC_WriteBurst = FMC_WriteBurst_Disable;
	FMC_NORSRAMInitStructure.FMC_ReadWriteTimingStruct = &p;
	FMC_NORSRAMInitStructure.FMC_WriteTimingStruct = &p;
	FMC_NORSRAMInit(&FMC_NORSRAMInitStructure); 
	/*!< Enable FSMC Bank1_SRAM4 Bank */
	FMC_NORSRAMCmd(FMC_Bank1_NORSRAM4, ENABLE); 
}


#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
  	while (1);
}
#endif

//PB6, PB9를 사용하는 I2C1 을 사용한다.
void BSP_I2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  	I2C_InitTypeDef I2C_InitStructure;

  	/* Enable I2S and I2C GPIO clocks */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);

  	/* CODEC_I2C SCL and SDA pins configuration -------------------------------------*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);     
  	/* Connect pins to I2C peripheral */
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //SCL 
  	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1); //SDA 
  
  	/* Enable the CODEC_I2C peripheral clock */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
  
  	/* CODEC_I2C peripheral configuration */
  	I2C_DeInit(I2C1);
  	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  	I2C_InitStructure.I2C_OwnAddress1 = 0x33;
  	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  	I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  	/* Enable the I2C peripheral */
  	I2C_Cmd(I2C1, ENABLE);  
  	I2C_Init(I2C1, &I2C_InitStructure);
}


//터치인터럽트로 사용한다.
void EXTILine6_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource6);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void Touch_EXTILine6_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOF clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PF11 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	/* Connect EXTI Line11 to PF11 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource6);

	/* Configure EXTI Line6 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line7 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Encoder_EXTILine0_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOB clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PB1 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect EXTI Line8 to PB8 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource0);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  //, EXTI_Trigger_Rising_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line1 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Encoder_EXTILine8_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOB clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PB1 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect EXTI Line8 to PB8 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //, EXTI_Trigger_Rising_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line1 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void Encoder_EXTILine5_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOB clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PB1 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect EXTI Line8 to PB8 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);

	/* Configure EXTI Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //, EXTI_Trigger_Rising_Falling
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line1 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void Encoder_EXTILine13_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOB clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PB1 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Connect EXTI Line1 to PB9 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

	/* Configure EXTI Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line13;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line1 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USB_Select_Power_PortInit(void)
{
 	GPIO_InitTypeDef   GPIO_InitStructure;				 

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	// Enable SYSCFG clock 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);   
    
    //PB2:Power Enable
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
    
    GPIO_ResetBits(GPIOB, GPIO_Pin_2);  //    
}

//5.00 에서는 PC5 로 변경되었다.
void GPU_PowerControl(unsigned int status)
{
	GPIO_InitTypeDef   GPIO_InitStructure;				 

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
	// Enable SYSCFG clock 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
  	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	
	
    if(status==0) {
        GPIO_SetBits(GPIOB, GPIO_Pin_1);        //LCD Power ON
    }
    else GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    if(status==0) {
		GPIO_SetBits(GPIOC, GPIO_Pin_5);        //Power ON
	}
	else GPIO_ResetBits(GPIOC, GPIO_Pin_5);
	
    if(status==0)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);       //SM718 Reset OFF
        Delay(10);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);     //SM718 Reset ON
        Delay(10);
        GPIO_SetBits(GPIOC, GPIO_Pin_13);       //SM718 Reset OFF
    }
}

void SD_EXTILine14_Config(void) //PB14
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	/* Enable GPIOB clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Configure PB1 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Connect EXTI Line8 to PB8 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource14);

	/* Configure EXTI Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI Line1 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void SD_DetectPortInit(void)
{
	SD_EXTILine14_Config();
}

void SD_DetectGPIOPortInit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;				 

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	// Enable SYSCFG clock 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void USB_Power(int s)
{
	if(s==OFF)
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);  //USB Power Off	
		Dprintf("USB_Power:: OFF\n");
	}
	else
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_2);  //USB Power ON
		Dprintf("USB_Power:: ON\n");
	}
}

void EncoderInit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;				 

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	// Enable SYSCFG clock 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure); 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 	
	
	Encoder_EXTILine0_Config();	//Encoder 0 의 A, B상처리
	Encoder_EXTILine8_Config();	// Encoder 0의 스위치처리
	Encoder_EXTILine13_Config();	//Encoder1 의 A,B
	Encoder_EXTILine5_Config();	//Encoder 1의 스위치처리
}

void SDRAM_Test(int no)
{
	Dprintf("================= SDRAM Test=%d\n", no);
	uint8_t	testBuf[200];
	//int	i;
	strcpy((char *)testBuf,"1234567890");
	SDRAM_WriteBuffer(testBuf, 0, 100);
	memset(testBuf, 0, 100);
	strcpy((char *)testBuf,"abcdefghijkl");
	
	*(uint8_t *)0xc0000000 = 'T';
	*(uint8_t *)0xc0000001 = 'S';
	SDRAM_ReadBuffer(testBuf, 0, 100);	
	Dprintf("Test Result = %s\n", testBuf);		
}
void PowerStatusPortInit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;				 

    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void TEECO_BSP_Init(void)
{
	gTouch = -1;			//Touch Interrupt Use
    gTouchKeyPad = -1;      //KeyPad Interrupt Use
	g_Touch_Time = 0;		//터치의 채터링을 방지하기 위한 1msec 시간변수
	g_TouchKeyPad_Time = 0;	//키패드의 채터링을 방지하기 위한 1msec 시간변수
	g_Touch_Enable = ON;
    PowerStatusPortInit();
	if (SysTick_Config(SystemCoreClock / 1000))  //1000:1msec  100: 10msec
	{ 
		while (1);
	}
    Uart2_Config();
	BSP_FSMC_Init();
  	NVIC_Configuration();// SD Interrupt
    DiskInit();	
    SD_DetectPortInit();
    SD_Mount();
    Dputc(0x0d);
    
	gv.usbSaveWaitFlag=1;
    DataLog_VarInit();
	    
	//USB_Select_Power_PortInit();
    
    //CAN_NVIC_Config();
    //CAN_Config();    
    GPU_PowerControl(flash.powerControl);
    if(flash.powerControl==0)
    {
        Dprintf("SM718 INIT\n");
        TeecoBmpDMA_Config();
        BSP_SM718Init();
        SM718_2DInit();
        Init2D();
        BSP_ScreenClear();
        SetGraphicsPage(LCD_SCREEN);        
        LCD_ON();		
    }
    else {
        LCD_OFF();		
    }
    DS1302PortInit();
	
	//BSP_I2C_PortInit();	//PORT 를 사용해서 I2C를 구현한다.
	//BSP_I2C_Init();		//IP 를 사용해서 I2C를 사용한다.
    //Voice_Port_Init();
	//VoiceChipReset();
	//Delay(10);
	//Voice_Init();

	NandMBRCashe();
	TouchDataLoad();
	gMsgWCount = 0;
	gMsgRCount = 0;
	g_Wifi_Time = 0;
	
	W5500_Init();

    //BKSRAM을 사용하면(아래코드를 인에이블 시키면 SD Card가 인식이 안되는경우가 있다
    /* Enable BKPSRAM Clock */
    //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);	
	//USB_Lib_Load();
#ifdef DMA_USE
    gDMA_USE = YES;
#else
    gDMA_USE = NO;
#endif
}
void RTOS_After_Init(void)
{
	gTouch = -1;			//Touch Interrupt Use
    gTouchKeyPad = -1;      //KeyPad Interrupt Use
	g_Touch_Time = 0;		//터치의 채터링을 방지하기 위한 1msec 시간변수
	g_TouchKeyPad_Time = 0;	//키패드의 채터링을 방지하기 위한 1msec 시간변수
	g_Touch_Enable = ON;
    
    GPU_PowerControl(0);

	BSP_FSMC_Init();		// FSMC Bank1 NE1, NE2, NE3 Init Code
    
	//USB Host Power Enable Port init PB2
    USB_Select_Power_PortInit();
    DS1302PortInit();    
    
  	NVIC_Configuration();  	// SD Interrupt

    TeecoBmpDMA_Config();

	//SM718의 전원을 테스트하기위해 임시로 막아놓았다.
    
    BSP_SM718Init();
	SM718_2DInit();
	Init2D();
    BSP_ScreenClear();
    SetGraphicsPage(LCD_SCREEN);
	LCD_ON();	
	
	BSP_ADS7843_PortInit();
	//터치칩의 인터럽트를 설정한다.
	Touch_EXTILine6_Config();    

    //사운드함수를 사용하기위한 초기설정 보이스 전용칩을 사용한다.
	Voice_Port_Init();
	VoiceChipReset();
	Delay(10);
	Voice_Init();

	DiskInit();	//SD Init
	
	NandMBRCashe();
	TouchDataLoad();

	gMsgWCount = 0;
	gMsgRCount = 0;
	
	SD_DetectPortInit();
	
	//!< 윈도우용이지만 부트로더에서 일부사용하는 관계로 여기서 먼저 초기화한다.
#define	GRAPHICS_MEMORY		(0x6B802000+0x1000)		// 512Byte
	gp_Graphics = (Graphics *)GRAPHICS_MEMORY;//0x6B803000;   //GRAPHICS_MEMORY
	memset(gp_Graphics, 0, sizeof(Graphics));
	
	USB_Lib_Load();
	
#ifdef DMA_USE
    gDMA_USE = YES;
#else
    gDMA_USE = NO;
#endif
}

void RTOS_Init(void)
{
	SD_DetectGPIOPortInit();
    BSP_UartInit();			// For Debug UART Port COM2
	DiskInit();	//SD Init

	if (SysTick_Config(SystemCoreClock / 1000))  //1000:1msec  100: 10msec
	{ 
		while (1);
	}		
}

#ifdef __cplusplus
}
#endif
