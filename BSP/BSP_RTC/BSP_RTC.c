/******************************************************************************
*    	File name	: BSP_RTC.c
*		Description : RTC/HW_Calendar
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 07 / 30
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "BSP_Rtc.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */ 

/******************************************************************************
							Private & Local Variables
******************************************************************************/
RTC_TimeTypeDef RTC_TimeStructure;
RTC_InitTypeDef RTC_InitStructure;
RTC_AlarmTypeDef  RTC_AlarmStructure;
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;


/******************************************************************************
							Function Prototype
******************************************************************************/

void BSP_RtcTest(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */     
	//NVIC_InitTypeDef  NVIC_InitStructure;
	//EXTI_InitTypeDef  EXTI_InitStructure;
       
	Dprintf("*********************** RTC Hardware Calendar Example ***********************\n");
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
    
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
	{  
		/* RTC configuration  */
		RTC_Config();

		/* Configure the RTC data register and RTC prescaler */
		RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
   
		/* Check on RTC init */
		if (RTC_Init(&RTC_InitStructure) == ERROR)
		{
			Dprintf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n\r");
		}

		/* Configure the time register */
		RTC_TimeRegulate(); 
	}
	else
	{
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			Dprintf("\r\n Power On Reset occurred....\n\r");
		}
		/* Check if the Pin Reset flag is set */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			Dprintf("\r\n External Reset occurred....\n\r");
		}

		Dprintf("\n\r No need to configure RTC....\n\r");
    
		/* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();

		/* Clear the RTC Alarm Flag */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
		EXTI_ClearITPendingBit(EXTI_Line17);

		/* Display the RTC Time and Alarm */
		RTC_TimeShow();
//RTC_AlarmShow();
	}
   
	/* Enable the RTC Alarm Interrupt */
	//NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);	
}

/**
  * @brief  Configure the RTC peripheral by selecting the clock source.
  * @param  None
  * @retval None
  */
void RTC_Config(void)
{
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);
    
#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
	/* The RTC Clock may varies due to LSI frequency dispersion. */   
	/* Enable the LSI OSC */ 
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
	SynchPrediv = 0xFF;
	AsynchPrediv = 0x7F;

#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
	/* Enable the LSE OSC */
	RCC_LSEConfig(RCC_LSE_ON);

	/* Wait till LSE is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}

	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
	SynchPrediv = 0xFF;
	AsynchPrediv = 0x7F;

#else
  #error Please select the RTC Clock source inside the main.c file
#endif /* RTC_CLOCK_SOURCE_LSI */
  
	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);

	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();
}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeRegulate(void)
{
	uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

	Dprintf("\n\r==============Time Settings=====================================\n\r");
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	Dprintf("  Please Set Hours:\n\r");
	while (tmp_hh == 0xFF)
	{
		tmp_hh = USART_Scanf(23);
		RTC_TimeStructure.RTC_Hours = tmp_hh;
	}
	Dprintf("  %0.2d\n\r", tmp_hh);

	Dprintf("  Please Set Minutes:\n\r");
	while (tmp_mm == 0xFF)
	{
		tmp_mm = USART_Scanf(59);
		RTC_TimeStructure.RTC_Minutes = tmp_mm;
	}
	Dprintf("  %0.2d\n\r", tmp_mm);

	Dprintf("  Please Set Seconds:\n\r");
	while (tmp_ss == 0xFF)
	{
		tmp_ss = USART_Scanf(59);
		RTC_TimeStructure.RTC_Seconds = tmp_ss;
	}
	Dprintf("  %0.2d\n\r", tmp_ss);

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		Dprintf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
		Dprintf("\n\r>> !! RTC Set Time success. !! <<\n\r");
		RTC_TimeShow();
		/* Indicator for the RTC configuration */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}

	tmp_hh = 0xFF;
	tmp_mm = 0xFF;
	tmp_ss = 0xFF;

	/* Disable the Alarm A */
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	Dprintf("\n\r==============Alarm A Settings=====================================\n\r");
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_AM;
	Dprintf("  Please Set Alarm Hours:\n\r");
	while (tmp_hh == 0xFF)
	{
		tmp_hh = USART_Scanf(23);
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = tmp_hh;
	}
	Dprintf("  %0.2d\n\r", tmp_hh);

	Dprintf("  Please Set Alarm Minutes:\n\r");
	while (tmp_mm == 0xFF)
	{
		tmp_mm = USART_Scanf(59);
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = tmp_mm;
	}
	Dprintf("  %0.2d\n\r", tmp_mm);

	Dprintf("  Please Set Alarm Seconds:\n\r");
	while (tmp_ss == 0xFF)
	{
		tmp_ss = USART_Scanf(59);
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = tmp_ss;
	}
	Dprintf("  %0.2d", tmp_ss);

	/* Set the Alarm A */
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

	/* Configure the RTC Alarm A register */
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	Dprintf("\n\r>> !! RTC Set Alarm success. !! <<\n\r");
	RTC_AlarmShow();

	/* Enable the RTC Alarm A Interrupt */
//RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	/* Enable the alarm  A */
//RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeShow(void)
{
	//Date_Display();
	/* Get the current Time */	
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	Dprintf("\n\r  The current time is :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_AlarmShow(void)
{
	/* Get the current Alarm */
	RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	Dprintf("\n\r  The current alarm is :  %0.2d:%0.2d:%0.2d \n\r", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
}

uint8_t USART_Scanf(uint32_t value)
{
	uint32_t index = 0;
	uint32_t tmp[2] = {0, 0};

	while (index < 2)
	{
		/* Loop until RXNE = 1 */
		while (Dstatus() == RESET)
		{}
		tmp[index++] = Dgetch();
		if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
		{
			Dprintf("\n\r Please enter valid number between 0 and 9 \n\r");
			index--;
		}
	}
	/* Calculate the Corresponding value */
	index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
	/* Checks */
	if (index > value)
	{
		Dprintf("\n\r Please enter valid number between 0 and %d \n\r", value);
		return 0xFF;
	}
	return index;
}

void Date_Display(void)
{
	RTC_DateTypeDef   RTC_DateStructure;
	/* Get the current Date */
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);

	/* Display Date WeekDay */
	Dprintf("Week=%d\n", RTC_DateStructure.RTC_WeekDay);

	/* Display Date Day */
	Dprintf("Date=%d\n", RTC_DateStructure.RTC_Date );

	/* Display Date Month */
	Dprintf("Month=%d\n", RTC_DateStructure.RTC_Month);

	/* Display Date Year */
	Dprintf("Year=%d\n", RTC_DateStructure.RTC_Year+2000);
}

void RTC_DateSet(void)
{
	//RTC_DateStructure.RTC_WeekDay = weekday;
	//RTC_DateStructure.RTC_Date = date;
	//RTC_DateStructure.RTC_Month = month;
	//RTC_DateStructure.RTC_Year = year;
	//RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
}


