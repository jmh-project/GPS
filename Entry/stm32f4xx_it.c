/**
  ******************************************************************************
  * @file       	stm32f4xx_it.c
  * @author  	(주)자이언텍 기술개발지원팀
  * @version 	V0.1
  * @date    	2014년 12월19일
  * @brief    	TEECO Interrupt 처리 함수
                
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
#include "core_cm4.h"
#include "DataLog.h"

/* Private typedef -----------------------------------------------------------*/
//VRAM
#ifdef VRAM_USE     
#define	WINTIMER_MALLOC_START		0x6B3F2000
#define	IOTIMER_MALLOC_START			0x6B3FA000
#endif
     
//PSRAM
#ifdef PSRAM_USE     
#define	WINTIMER_MALLOC_START		0x645ACA00
#define	IOTIMER_MALLOC_START			0x645A4A00     
#endif     

#ifdef MEDIA_USB_KEY
#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"
#endif

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile int gPosition=0;
volatile int gPosition1=0;
volatile int gOldPosition=0;
volatile int gEncoderSW=0;
uint16_t 		capture = 0;
volatile 		S16 gTouch, gTouchKeyPad;
volatile MSG	gKeyPadMsg;
volatile MSG	gTouchMsg;
volatile MSG	gTimerMsg;
volatile MSG	gIOTimerMsg;
volatile 		U32 gSystemTime;
volatile int    old_x, old_y;
volatile int    old_xm, old_ym;
volatile int    old_key=0;

volatile int gWinTimerCount;
WinTimer 	*gWinTimer_Int;
IOTimer 	*gIOTimer_Int;

volatile int gTest_Timer1, gTest_Timer2, gTest_Timer3; 
volatile int gSmartX_Encoder_Process=0;
volatile int gEncoder_Process=0;
volatile uint16_t TouchBuf_X[32];
volatile uint16_t TouchBuf_Y[32];
volatile uint16_t TouchCnt=0;
volatile int averTouchX, averTouchY;

extern __IO uint16_t CCR_Val;
extern volatile int gUSB_Select;
extern USB_OTG_CORE_HANDLE  USB_OTG_dev;
extern volatile U8		IOTimerFlag[64];
extern volatile int g_Touch_Time;
extern volatile int g_TouchKeyPad_Time;
extern volatile int g_Touch_Enable;
extern volatile int gi_Dir;
extern volatile int 	gTouchX, gTouchY, gPress;
extern volatile MSG	gTouchMsg;
extern __IO uint32_t TimeDisplay;

#if defined MEDIA_USB_KEY
extern USB_OTG_CORE_HANDLE          	USB_OTG_Core;
extern USBH_HOST                    			USB_Host;
extern void USB_OTG_BSP_TimerIRQ (void);
#endif /* MEDIA_USB_KEY */

/* Private function prototypes -----------------------------------------------*/
void TimingDelay_Decrement(void);
int  BSP_TouchRead( void );
void EncoderProcess(void);
void IOTimerMSGInsert(MSG *message);
int BSP_Keyin(void);
void SD_Mount(void);
/* Private functions ---------------------------------------------------------*/


#ifdef REMOTE_TERMINAL_UNIT
void RpmSet(unsigned int uartPort, int rpm);
int BoatControl(void);
#endif


/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	Dprintf("NMI Handler Gen...\n");
}

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)) //Rising A
        {
            if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))  gPosition++;            
			else  gPosition--;  
		}
        else	//Falling A
        {
            if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)) gPosition--;
			else  gPosition++;
		}
        
        gTouchMsg.message = KEY_ROT;
        gTouchMsg.Parent = 0x3000;	//Encoder
        gTouchMsg.Handle = 0;
        gTouchMsg.wParam = gPosition;
        gTouchMsg.lParam = 0;
        //gTouchMsg.time = gSystemTime;
        SystemMSGInsert((MSG *)&gTouchMsg);
        
		/* Clear the EXTI line 8 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
    }
}


void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{		
		EXTI_ClearITPendingBit(EXTI_Line1);        
    }

}
void EXTI3_IRQHandler(void)
{
	//전력량계 1펄스당 1W 정수
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{						
		//SmartLogFlash.rsc.powerPulsCount++;
#ifdef RAIN_STORAGE_CONTROL        
		SmartLogFlash.rsc.powerMeter += 1.0F;
#endif
#ifdef INPUT_PORT_SELECT_1 
			if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)==1)
			{
				SmartLogFlash.Contact.status[1] = 1;
			}
			else
			{
				SmartLogFlash.Contact.status[1] = 0;
			}
#endif		
		EXTI_ClearITPendingBit(EXTI_Line3);        
    }	
}
void EXTI15_10_IRQHandler(void)
{
	//NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
    //Rotary Encoder SW--------------------------------------------------------------
	if(EXTI_GetITStatus(EXTI_Line13) != RESET)
	{
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)) //Rising B
        {			
            if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)) gPosition1++;
			else  	gPosition1--;  
		}
        else	//Falling B
        {
            if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)) gPosition1--;
			else  gPosition1++;
		}	
        
        gTouchMsg.message = KEY_ROT;
        gTouchMsg.Parent = 0x3000;	//Encoder
        gTouchMsg.Handle = 0;
        gTouchMsg.wParam = gPosition1;
        gTouchMsg.lParam = 1;
        //gTouchMsg.time = gSystemTime;
        SystemMSGInsert((MSG *)&gTouchMsg);
        
		/* Clear the EXTI line 0 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line13);
	}  
    if(EXTI_GetITStatus(EXTI_Line15) != RESET)//유량계
	{

#if defined (SAMAN_ENABLE)	//인터럽트를 사용설정
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)==0)
		{
            flash.connection[0]=1;//1.접점신호를 받는다.
			flash.timerCnt200ms=0;
		}
		else {
			if(flash.timerCnt200ms < 250 && flash.timerCnt200ms > 90) {
				flash.pulsCountPv++;//적산량중가한다.
			}
			//Dprintf("%d\n",flash.timerCnt200ms);
			flash.timerCnt200ms=0;
		}
#endif			
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
    if(EXTI_GetITStatus(EXTI_Line14) != RESET)
	{
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0) 
		{
			SD_Mount();
		}
		/* Clear the EXTI line 14 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line14);
	}		
}
//                    DataToDisk(&SmartLogFlash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");

//HardFaultException 이 발생한 경우 스택에서 이전 PC값을 출력해 주는 것입니다.
//스택까지 깨진 경우는 소용이 없겠지만요.
//PC값이 PC = 8005C18 라고 나온다면 *.lss 파일에서 이 주소를 찾아보면 어느 부분을 실행하다가 에러가 발생한 것인지 알 수 있습니다

/******************************************************************************* 
* Function Name : HardFaultException 
* Description : This function handles Hard Fault exception. 
* Input : None 
* Output : None 
* Return : None 
*******************************************************************************/ 
void hard_fault_handler_c (unsigned int * hardfault_args)
{
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;

    stacked_r0 = ((unsigned long) hardfault_args[0]);
    stacked_r1 = ((unsigned long) hardfault_args[1]);
    stacked_r2 = ((unsigned long) hardfault_args[2]);
    stacked_r3 = ((unsigned long) hardfault_args[3]);

    stacked_r12 = ((unsigned long) hardfault_args[4]);
    stacked_lr = ((unsigned long) hardfault_args[5]);
    stacked_pc = ((unsigned long) hardfault_args[6]);
    stacked_psr = ((unsigned long) hardfault_args[7]);

    Dprintf ("\n\n[Hard fault handler - all numbers in hex]\n");
    Dprintf ("R0 = %x\n", stacked_r0);
    Dprintf ("R1 = %x\n", stacked_r1);
    Dprintf ("R2 = %x\n", stacked_r2);
    Dprintf ("R3 = %x\n", stacked_r3);
    Dprintf ("R12 = %x\n", stacked_r12);
    Dprintf ("LR [R14] = %x  subroutine call return address\n", stacked_lr);
    Dprintf ("PC [R15] = %x  program counter\n", stacked_pc);
    Dprintf ("PSR = %x\n", stacked_psr);
    Dprintf ("BFAR = %x\n", (*((volatile unsigned long *)(0xE000ED38))));
    Dprintf ("CFSR = %x\n", (*((volatile unsigned long *)(0xE000ED28))));
    Dprintf ("HFSR = %x\n", (*((volatile unsigned long *)(0xE000ED2C))));
    Dprintf ("DFSR = %x\n", (*((volatile unsigned long *)(0xE000ED30))));
    Dprintf ("AFSR = %x\n", (*((volatile unsigned long *)(0xE000ED3C))));
    Dprintf ("SCB_SHCSR = %x\n", SCB->SHCSR);
	NVIC_SystemReset();
    while (1);
}


/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	Dprintf("MemManage_Handler Gen...\n");
	NVIC_SystemReset();	
	while (1)
	{}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	Dprintf("BusFault_Handler Gen...\n");
	NVIC_SystemReset();		
	while (1)
	{}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	Dprintf("UsageFault_Handler Gen...\n");
	NVIC_SystemReset();		
	while (1)
	{}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
	Dprintf("DebugMon_Handler Gen...\n");	
}

//*****************************************************************************
//평소에는 1msec 기준시간타이머로 사용하지만 OS 기동시에는 Tick 으로 사용한다.
//그것의 구분은 OS_START 라는 전역변수로 구분한다.
//*****************************************************************************
extern volatile int	OS_START;
void xPortSysTickHandler( void );
void SysTick_Handler(void)
{
#ifdef OS_USE
	if(OS_START==1)
		xPortSysTickHandler();
#endif
	TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/******************************************************************************/
/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
	/* Process All SDIO Interrupt Sources */
	//NVIC_ClearPendingIRQ(SDIO_IRQn);
	SD_ProcessIRQSrc();
}

/**
  * @brief  This function handles DMA2 Stream3 or DMA2 Stream6 global interrupts
  *         requests.
  * @param  None
  * @retval None
  */
void SD_SDIO_DMA_IRQHANDLER(void)
{
	//NVIC_ClearPendingIRQ(
	/* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
	SD_ProcessDMAIRQ();
}

/******************************************************************************
		설명 : 		터치 인터럽트 설정 함수이다.
		전달인자 : 	없음 
		리턴값 :	없음 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
extern void WaterCell_ADC_Interrupt(void);
void EXTI9_5_IRQHandler(void)
{
	//NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)==0) //Faling
        {
			gTouchMsg.message = KEY_ON;
			gTouchMsg.Parent = 0x3000;	//Encoder
			gTouchMsg.Handle = 0;
			gTouchMsg.wParam = gPosition;
			gTouchMsg.lParam = 0;	//ENC No. SW동작상태의 구분
			//gTouchMsg.time = gSystemTime;
			SystemMSGInsert((MSG *)&gTouchMsg);			
		}
		/*
		else
		{
			gTouchMsg.message = KEY_OFF;
			gTouchMsg.Parent = 0x3000;	//Encoder
			gTouchMsg.Handle = 0;
			gTouchMsg.wParam = gPosition;
			gTouchMsg.lParam = 0;	//ENC No. SW동작상태의 구분
			//gTouchMsg.time = gSystemTime;
			SystemMSGInsert((MSG *)&gTouchMsg);			
		}
		*/
		/* Clear the EXTI line 1 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
    if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
#ifdef WATERCELL_PROJECT		
		WaterCell_ADC_Interrupt();		//!< 워터셀 인터럽트
#endif		
		//UPS_ADC_Interrupt();				//!<UPS 인터럽트
		/* Clear the EXTI line 6 pending bit */
#ifdef ENCODER_USE        
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0) //Faling
        {
			gTouchMsg.message = KEY_ON;
			gTouchMsg.Parent = 0x3000;	//Encoder
			gTouchMsg.Handle = 0;
			gTouchMsg.wParam = gPosition1;
			gTouchMsg.lParam = 1;	//ENC No. SW동작상태의 구분
			//gTouchMsg.time = gSystemTime;
			SystemMSGInsert((MSG *)&gTouchMsg);						
		}		
#endif		
		EXTI_ClearITPendingBit(EXTI_Line5);
	}	

    if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		if(g_Touch_Enable==ON)
		{
			if(!GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_6)) //Low
			{		
				gPress=ON;	//터치가 눌렸음을 알리는 전역변수
				g_Touch_Time = 0;
			}
			else //High
			{
				gPress=OFF;	//터치가 눌렸음을 알리는 전역변수
				g_Touch_Time = 0;
			}
		}
		/* Clear the EXTI line 6 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
#ifdef RAIN_STORAGE_CONTROL    
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
        SmartLogFlash.pulsCount++;
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
#endif 
#ifdef INPUT_PORT_SELECT_1	
    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
    {
		Dputc('X');
        EXTI_ClearITPendingBit(EXTI_Line7);
    }
#endif 
}

void TIM2_IRQHandler(void)
{
	//NVIC_ClearPendingIRQ(TIM2_IRQn);
	if(gUSB_Select == HOST)
		USB_OTG_BSP_TimerIRQ();    
}

//=============================================================================
//    터치용 타이머  25msec 의 주기를 갖는다.
//=============================================================================


void TIM3_IRQHandler(void)
{
	//NVIC_ClearPendingIRQ(TIM3_IRQn);
	int 	i;
	U32		addr;
	//윈도우 타이머용
  	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
  	{
    	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
		capture = TIM_GetCapture2(TIM3);
    	TIM_SetCompare2(TIM3, capture + 1000);

        //GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));   
		
		if(gp_WinManager->TimerCount != 0)
		{
			for(i=0; i<64; i++)
			{
				if(WinTimerFlag[i]==TRUE)
				{
					addr = (WINTIMER_MALLOC_START + (i * sizeof(WinTimer)));
					gWinTimer_Int = (WinTimer *)addr;
					if(gWinTimer_Int->Status==START)
					{
						gWinTimer_Int->Count++;
						if(gWinTimer_Int->Count >= gWinTimer_Int->Interval)
						{
							gWinTimer_Int->Count = 0;
							gTimerMsg.message = TIMER_EVENT;
							gTimerMsg.Parent = gWinTimer_Int->Parent;
							gTimerMsg.Handle = gWinTimer_Int->Handle;
							gTimerMsg.wParam = gWinTimer_Int->id;
							gTimerMsg.lParam = 0;
							//gTimerMsg.time = gSystemTime;
                            //__disable_irq();							
							TimerMSGInsert((MSG *)&gTimerMsg);	
                            //__enable_irq();                            
							//gWinTimerCount++;							
						}
					}
				}
			}
		}
	}
   
  	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
  	{
    	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM3);
    	TIM_SetCompare1(TIM3, capture + 40000);

#ifdef KEYPAD_USE
        if(BSP_Keyin()) //키가눌렸는가?
        {
            if(gTouchKeyPad == -1)
            {
                //처음눌린상태라면
                g_TouchKeyPad_Time = 0;
                gTouchKeyPad = 1;
            }
            if(g_TouchKeyPad_Time > 10) //키가 눌린지 10msec 가 지났는가?
            {
                int key = BSP_Keyin();
                if(g_KeyPad_Repeat==ON) //키 리피트 동작이 이네이블 되어있으면
                {
                    if(key != 0 )
                    {                
                        gKeyPadMsg.message = KEY_ON;
                        gKeyPadMsg.Parent = 0x2000;	//KeyPad
                        gKeyPadMsg.Handle = 0;
                        gKeyPadMsg.wParam = key;
                        gKeyPadMsg.lParam = 0;
                        SystemMSGInsert((MSG *)&gKeyPadMsg);
                        old_key = key;
                    }
                    else
                    {
                        gTouchKeyPad = -1; //채터링이었다면 원위치한다.
                        old_key = 0;
                    }
                }
                else    //키리피트동작이 꺼져있으면 같은키는 1회만 입력받는다.
                {
                    if(key != 0 && key != old_key)
                    {                
                        gKeyPadMsg.message = KEY_ON;
                        gKeyPadMsg.Parent = 0x2000;	//KeyPad
                        gKeyPadMsg.Handle = 0;
                        gKeyPadMsg.wParam = key;
                        gKeyPadMsg.lParam = 0;
                        SystemMSGInsert((MSG *)&gKeyPadMsg);
                        old_key = key;
                    }
                }
            }
        }
        else //키패드가 눌리지 않았고
        {
            if(gTouchKeyPad == 1)	//그전에 눌렸었다면 터치가 떨어진 상태임
            {
                gTouchKeyPad = -1; 
                gKeyPadMsg.Parent = 0x2000;	//KeyPad
                gKeyPadMsg.Handle = 0;
                gKeyPadMsg.message = KEY_OFF;
                gKeyPadMsg.wParam = 0;
                gKeyPadMsg.lParam = 0;
                SystemMSGInsert((MSG *)&gKeyPadMsg);
                old_key = 0;
            }
		}
#endif //KEYPAD_USE  

#ifdef ENCODER_USE
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) && gEncoderSW==OFF)  //Encoder SW Press
        {
            gEncoderSW = ON;
            gTouchMsg.message = KEY_ON;
            gTouchMsg.Parent = 0x3000;	//Encoder
            gTouchMsg.Handle = 0;
            gTouchMsg.wParam = gPosition;
            gTouchMsg.lParam = 0;
            //gTouchMsg.time = gSystemTime;
            SystemMSGInsert((MSG *)&gTouchMsg);
        }
        if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)==0 && gEncoderSW==ON)  //Encoder SW Release
        {
            gEncoderSW = OFF;
            gTouchMsg.message = KEY_OFF;
            gTouchMsg.Parent = 0x3000;	//Encoder
            gTouchMsg.Handle = 0;
            gTouchMsg.wParam = gPosition;
            gTouchMsg.lParam = 0;
            //gTouchMsg.time = gSystemTime;
            SystemMSGInsert((MSG *)&gTouchMsg);
        }
        if(gOldPosition != gPosition)
        {
            gOldPosition = gPosition;
            gTouchMsg.message = KEY_ROT;
            gTouchMsg.Parent = 0x3000;	//Encoder
            gTouchMsg.Handle = 0;
            gTouchMsg.wParam = gPosition;
            gTouchMsg.lParam = 0;
            //gTouchMsg.time = gSystemTime;
            SystemMSGInsert((MSG *)&gTouchMsg);            
        }
#endif        
        
#ifdef TOUCH_USE
        //터치용 (메시지 인터럽트 방식이다)
		if(g_Touch_Enable==ON)
		{
			gSystemTime++;
			if(gPress==ON)
			{
				if(g_Touch_Time > 10) //터치가 눌린지 10msec 가 지났는가?
				{
					BSP_TouchRead();
                    if(gTouch == -1)//처음눌린상태임
                    {
                        gTouch = 1;
                        gTouchMsg.message = TOUCH_ON;
                        gTouchMsg.Parent = 0x1000;	//Touch
                        gTouchMsg.Handle = 0;
                        gTouchMsg.wParam = 0;
                        gTouchMsg.lParam = 0;
                        //gTouchMsg.time = gSystemTime;
                        old_x = gTouchMsg.pt.x = gTouchX;
                        old_y = gTouchMsg.pt.y = gTouchY;
                        //__disable_irq();
                        SystemMSGInsert((MSG *)&gTouchMsg);
                        //__enable_irq();
                    }
				}
			}
			else //터치가 눌리지 않았고
			{
				if(gTouch == 1)	//그전에 눌렸었다면 터치가 떨어진 상태임
				{
					gTouch = -1; 
					gTouchMsg.Parent = 0x1000;	//Touch
					gTouchMsg.Handle = 0;
					gTouchMsg.message = TOUCH_OFF;
					gTouchMsg.wParam = 0;
					gTouchMsg.lParam = 0;
					//gTouchMsg.time = gSystemTime;
					gTouchMsg.pt.x = gTouchX;
					gTouchMsg.pt.y = gTouchY;
				    //__disable_irq();
					SystemMSGInsert((MSG *)&gTouchMsg);
				    //__enable_irq();
				}
			}
		}
#endif // TOUCH_USE   
  	}

	//IO Process용 타이머 인터럽트
  	if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
  	{
    	TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);
		capture = TIM_GetCapture4(TIM3);

        //GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));    

		if(gp_WinManager->IOTimerCount != 0)
		{
			for(i=0; i<64; i++)
			{
				if(IOTimerFlag[i]==TRUE)
				{
					addr = (IOTIMER_MALLOC_START + (i * sizeof(IOTimer)));
					gIOTimer_Int = (IOTimer *)addr;
					if(gIOTimer_Int->Status==START)
					{
						gIOTimer_Int->Count++;
						if(gIOTimer_Int->Count >= gIOTimer_Int->Interval)
						{
							gIOTimer_Int->Count = 0;
							gIOTimerMsg.message = TIMER_EVENT;
							gIOTimerMsg.Handle = gIOTimer_Int->Handle;
							gIOTimerMsg.wParam = gIOTimer_Int->id;
							gIOTimerMsg.lParam = 0;
							//gIOTimerMsg.time = gSystemTime;
							IOTimerMSGInsert((MSG *)&gIOTimerMsg);	
							//gWinTimerCount++;							
						}
					}
				}
			}
		}
    	TIM_SetCompare4(TIM3, capture + 1000);        
	}	
}

#define 	DMA_STREAM               DMA2_Stream0
#define	    DMA_CHANNEL              DMA_Channel_0
#define 	DMA_STREAM_CLOCK         RCC_AHB1Periph_DMA2 
#define 	DMA_STREAM_IRQ           DMA2_Stream0_IRQn
#define 	DMA_IT_TCIF              DMA_IT_TCIF0
#define 	DMA_STREAM_IRQHANDLER    DMA2_Stream0_IRQHandler

void DMA_STREAM_IRQHANDLER(void)
{
	//NVIC_ClearPendingIRQ
	/* Test on DMA Stream Transfer Complete interrupt */
	if(DMA_GetITStatus(DMA_STREAM, DMA_IT_TCIF))
	{
		/* Clear DMA Stream Transfer Complete interrupt pending bit */
		DMA_ClearITPendingBit(DMA_STREAM, DMA_IT_TCIF);  
	}
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

void OTG_FS_IRQHandler(void)
{
	//NVIC_ClearPendingIRQ(OTG_FS_IRQn);
	if(gUSB_Select==HOST)
		USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

void RTC_WKUP_IRQHandler(void)
{
	//NVIC_ClearPendingIRQ(RTC_WKUP_IRQn);
    if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        TimeDisplay = 1;
        RTC_ClearITPendingBit(RTC_IT_WUT);
        EXTI_ClearITPendingBit(EXTI_Line22);
    }
}

char TouchBuf[140];
int	Touch_Count=0;
void TouchConversion(int xdata, int ydata);
void TouchProcessing(void) {	
	if(TouchBuf[0] == 'A' && Touch_Count==13)
	{
		char	buf[32];
		int	xdata=0;
		int	ydata=0;
		
		buf[0] = TouchBuf[3];
		buf[1] = TouchBuf[4];
		buf[2] = TouchBuf[5];
		buf[3] = TouchBuf[6];
		buf[4] = 0;
		xdata = atoi(buf);
		buf[0] = TouchBuf[8];
		buf[1] = TouchBuf[9];
		buf[2] = TouchBuf[10];
		buf[3] = TouchBuf[11];
		buf[4] = 0;
		ydata = atoi(buf);
		
        //Dprintf("TOUCH ::X=%d Y=%d   \n", xdata, ydata);
		if(xdata<130&&ydata>3600){
            gv.screenCreateFlag=1;
        }
        
        if(xdata==1995 && ydata==3768) {        //< VER4.0 > 버튼
			gv.touchReceiveErrorCount=0;        //터치에러타이머초기화
            gv.systemWatchTimerFlag=1;          //
            if(flash.powerControl!=2) {         //절전모드에서는 동작하지 않는다.
                TouchConversion(xdata, ydata);            
                if(TouchBuf[1]=='1')
                {
                    gPress=ON;
                }			
                else
                {
                    gPress=OFF;
                    //Dprintf("EVENT\n");
                }
            } 
            else {
                //gv.screenWatchTimerCount=0;     //절전모드에서 동작하는 초기화 명령
                gv.buttonWatchTimerCount=0;
            }
		} else if(xdata==3906 && ydata==3540) { //1024 . 768
			gv.touchReceiveErrorCount = 0;      //TOUCH 보드로 부터 리부팅했다는 명령을 수신하여 EventMessage.csv에 저장한다.
            gv.touchRebootFlag=1;
		} else {
			gv.touchReceiveErrorCount = 0;
			TouchConversion(xdata, ydata);
			if(TouchBuf[1]=='1')
			{
				gPress=ON;
			}			
			else
			{
				gPress=OFF;
			}
		}
	}	
}
void USART2_IRQHandler(void) {
	char	tc_data;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
        tc_data = (USART_ReceiveData(USART2) & 0xFF);
        //Dputc(tc_data);
		if(tc_data == 'A')//시작신호
		{
			Touch_Count = 0;
			TouchBuf[Touch_Count++] = tc_data; 
		}
		else if(tc_data=='B')//종료신호
		{
			TouchBuf[Touch_Count++] = tc_data; 			
			TouchProcessing();// TEECO 에 메시지를 보낸다.
			Touch_Count=0;
		}
		else TouchBuf[Touch_Count++] = tc_data; 
        
		if(TouchBuf[1]=='1') { 
			gv.touchErrorCount++;//터치가 눌려있습니다. 메시지 출력용
		}else if(TouchBuf[1]=='0') {
			gv.touchErrorCount=0;
		}
		
		if(Touch_Count >= 64) 
		{
			Dprintf("TOUCH ERROR\n");
			Touch_Count = 0;
		}
    }
}
#ifdef __cplusplus
}
#endif
