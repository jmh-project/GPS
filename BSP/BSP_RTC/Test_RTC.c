/******************************************************************************
*    	File name	: Test_RTC.c
*		Description : RTC/HW_Calendar
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 07 / 30
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
  Description	: This example demonstrates and explains how to use the peripherals 
  				  available on Backup Domain. These peripherals are: the RCC BDCR 
				  register containing the LSE Osc configuration and the RTC Clock 
				  enable/disable bits. It embeds also the RTC peripheral with its 
				  associated Backup Data registers. And finally the Backup SRAM (4KB) 
				  with its low power regulator allowing to preserve its contents 
				  when the product is powered by VBAT pin.
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

void RTC_Config(void);
void Time_Regulate(void);
void Time_Adjust(void);
void Time_Display(void);
void Date_Regulate(void);
void Date_Adjust(void);
void Date_Display(void);
void Calendar_Show(void);
void WriteToBackupReg(uint16_t FirstBackupData);
uint32_t CheckBackupReg(uint16_t FirstBackupData);

#define RTC_CLOCK_SOURCE_LSE
//#define RTC_CLOCK_SOURCE_LSI
                                   
#define RTC_BKP_DR_NUMBER   0x14
#define FIRST_DATA          0x32F2

RTC_InitTypeDef   RTC_InitStructure;
RTC_TimeTypeDef   RTC_TimeStructure;
RTC_DateTypeDef   RTC_DateStructure;
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;
__IO uint32_t TimeDisplay = 0;
uint32_t errorindex = 0, i = 0;

uint32_t BKPDataReg[RTC_BKP_DR_NUMBER] =
{
    RTC_BKP_DR0, RTC_BKP_DR1, RTC_BKP_DR2, 
    RTC_BKP_DR3, RTC_BKP_DR4, RTC_BKP_DR5,
    RTC_BKP_DR6, RTC_BKP_DR7, RTC_BKP_DR8, 
    RTC_BKP_DR9, RTC_BKP_DR10, RTC_BKP_DR11, 
    RTC_BKP_DR12, RTC_BKP_DR13, RTC_BKP_DR14, 
    RTC_BKP_DR15, RTC_BKP_DR16, RTC_BKP_DR17, 
    RTC_BKP_DR18,  RTC_BKP_DR19
};

  
int RTC_TEST(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef  EXTI_InitStructure;

    Dprintf("RTC Backup Domain Example\n");
    Dprintf ("  Copyright (c) STMicroelectronics  " );

    /* Display the default RCC BDCR and RTC TAFCR Registers */
    Dprintf ("Entry Point \n");
    Dprintf ("RCC BDCR = 0x%x\n", RCC->BDCR);
    Dprintf ("RTC TAFCR = 0x%x\n", RTC->TAFCR);

    /* Enable the PWR APB1 Clock Interface */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* EXTI configuration *******************************************************/
    EXTI_ClearITPendingBit(EXTI_Line22);
    EXTI_InitStructure.EXTI_Line = EXTI_Line22;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    if(RTC_ReadBackupRegister(RTC_BKP_DR0) != FIRST_DATA)
    {
        Dprintf ("RTC Config PLZ Wait. \n");

        /* RTC Configuration */
        RTC_Config();

        /* Adjust Current Time */
        Time_Adjust();

        /* Adjust Current Date */
        Date_Adjust();
    }
    else
    {
        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();
        RTC_ClearITPendingBit(RTC_IT_WUT);
        EXTI_ClearITPendingBit(EXTI_Line22);

        /*  Backup SRAM ***************************************************************/
        /* Enable BKPSRAM Clock */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);

        /* Check the written Data */
        for (i = 0x0; i < 0x1000; i += 4)
        {
            if ((*(__IO uint32_t *) (BKPSRAM_BASE + i)) != i)
            {
                errorindex++;
            }
        }
    
        if(errorindex)
        {
            Dprintf ("BKP SRAM Number of errors = %d\n", errorindex);
        }
        else
        {
            Dprintf ("BKP SRAM Content OK  \n");
        }
        /* RTC Backup Data Registers **************************************************/
        /* Check if RTC Backup DRx registers data are correct */
        if (CheckBackupReg(FIRST_DATA) == 0x00)
        { 
            /* OK, RTC Backup DRx registers data are correct */
            Dprintf ("OK, RTC Backup DRx registers data are correct. \n");
        }
        else
        { 
            /* Error, RTC Backup DRx registers data are not correct */
            Dprintf ("RTC Backup DRx registers data are not correct\n");
        }
    }
    /* Infinite loop */
    Calendar_Show();

    while (1)
    {
    }
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

    /* Write to the first RTC Backup Data Register */
    RTC_WriteBackupRegister(RTC_BKP_DR0, FIRST_DATA);

    /* Display the new RCC BDCR and RTC TAFCR Registers */
    Dprintf ("RTC Reconfig \n");
    Dprintf ("RCC BDCR = 0x%x\n", RCC->BDCR);
    Dprintf ("RTC TAFCR = 0x%x\n", RTC->TAFCR); 

    /* Set the Time */
    RTC_TimeStructure.RTC_Hours   = 0x08;
    RTC_TimeStructure.RTC_Minutes = 0x00;
    RTC_TimeStructure.RTC_Seconds = 0x00;

    /* Set the Date */
    RTC_DateStructure.RTC_Month = RTC_Month_March;
    RTC_DateStructure.RTC_Date = 0x18;  
    RTC_DateStructure.RTC_Year = 0x11; 
    RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Friday; 

    /* Calendar Configuration */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv =  SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);
  
    /* Set Current Time and Date */
    RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure); 

    /* Configure the RTC Wakeup Clock source and Counter (Wakeup event each 1 second) */
    RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);
    RTC_SetWakeUpCounter(0x7FF);
  
    /* Enable the Wakeup Interrupt */
    RTC_ITConfig(RTC_IT_WUT, ENABLE);

    /* Enable Wakeup Counter */
    RTC_WakeUpCmd(ENABLE); 

    /*  Backup SRAM ***************************************************************/
    /* Enable BKPRAM Clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);

    /* Write to Backup SRAM with 32-Bit Data */
    for (i = 0x0; i < 0x1000; i += 4)
    {
        *(__IO uint32_t *) (BKPSRAM_BASE + i) = i;
    }
    /* Check the written Data */
    for (i = 0x0; i < 0x1000; i += 4)
    {
        if ((*(__IO uint32_t *) (BKPSRAM_BASE + i)) != i)
        {
            errorindex++;
        }
    }

    if(errorindex)
    {
        Dprintf ("BKP SRAM Number of errors = %d\n", errorindex);
    }
    else
    {
        Dprintf ("BKP SRAM write OK \n");
    }

    /* Enable the Backup SRAM low power Regulator to retain it's content in VBAT mode */
    PWR_BackupRegulatorCmd(ENABLE);

    /* Wait until the Backup SRAM low power Regulator is ready */
    while(PWR_GetFlagStatus(PWR_FLAG_BRR) == RESET)
    {
    }

    /* RTC Backup Data Registers **************************************************/
    /* Write to RTC Backup Data Registers */
    WriteToBackupReg(FIRST_DATA);
}


/**
  * @brief  Returns the time entered by user, using menu navigation keys.
  * @param  None
  * @retval Current time RTC counter value
  */
void Time_Regulate(void)
{
    RTC_TimeStructure.RTC_Hours = 12;
    RTC_TimeStructure.RTC_Minutes = 0;
    RTC_TimeStructure.RTC_Seconds = 0;
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
}

/**
  * @brief  Returns the time entered by user, using demo board keys.
  * @param  None
  * @retval None
  */

void Time_Adjust(void)
{
    /* Display the current time */
    Time_Display();

    /* Change the current time */
    Time_Regulate();
}

/**
  * @brief  Displays the current time.
  * @param  None
  * @retval None
  */
void Time_Display(void)
{
    /* Get the current Time */
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
    Dprintf("Time:%2d:%d.%2d\n", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/**
  * @brief  Returns the time entered by user, using menu navigation keys.
  * @param  None
  * @retval Current date value
  */
void Date_Regulate(void)
{
    RTC_DateStructure.RTC_WeekDay = 1;
    RTC_DateStructure.RTC_Date = 27;
    RTC_DateStructure.RTC_Month = 7;
    RTC_DateStructure.RTC_Year = 65;
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
}

/**
  * @brief  Returns the date entered by user, using demoboard keys.
  * @param  None
  * @retval None
  */
void Date_Adjust(void)
{
    /* Display the current Date */
    Date_Display();

    /* Change the current Date */
    Date_Regulate();
}

/**
  * @brief  Displays the current date.
  * @param  None
  * @retval None
  */
void Date_Display(void)
{
    /* Get the current Date */
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
    Dprintf("Date:%d %d %d %d\n", RTC_DateStructure.RTC_WeekDay, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
}

/**
  * @brief  Shows the current time and date on LCD.
  * @param  None
  * @retval None
  */
void Calendar_Show(void)
{
    while(1)
    {
        /* If 1s has elapsed */
        if (TimeDisplay == 1)
        {
            /* Display current time */
            Time_Display();

            /* Display current date */
            Date_Display();

        TimeDisplay = 0;
        }
    }
}

/**
  * @brief  Writes data to all Backup data registers.
  * @param  FirstBackupData: data to write to first backup data register.
  * @retval None
  */
void WriteToBackupReg(uint16_t FirstBackupData)
{
    uint32_t index = 0;

    for (index = 0; index < RTC_BKP_DR_NUMBER; index++)
    {
        RTC_WriteBackupRegister(BKPDataReg[index], FirstBackupData + (index * 0x5A));
    }
}

/**
  * @brief  Checks if the Backup data registers values are correct or not.
  * @param  FirstBackupData: data to read from first backup data register
  * @retval - 0: All Backup DRx registers data are correct
  *         - Value different from 0: Number of the first Backup register which 
  *           value is not correct
  */
uint32_t CheckBackupReg(uint16_t FirstBackupData)
{
    uint32_t index = 0;

    for (index = 0; index < RTC_BKP_DR_NUMBER; index++)
    {
        if (RTC_ReadBackupRegister(BKPDataReg[index]) != (FirstBackupData + (index * 0x5A)))
        {
            return (index + 1);
        }
    }
    return 0;
}

#ifdef __cplusplus
}
#endif

