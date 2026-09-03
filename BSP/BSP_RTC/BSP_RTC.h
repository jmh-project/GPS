#ifndef _BSP_RTC
#define _BSP_RTC

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "TEECO_C_Define.h"

void BSP_RtcTest(void);	 
void RTC_Config(void);
void RTC_TimeRegulate(void);
void RTC_TimeShow(void);
void RTC_AlarmShow(void);
uint8_t USART_Scanf(uint32_t value);
void Date_Display(void);
	 
#ifdef __cplusplus
}
#endif

#endif
