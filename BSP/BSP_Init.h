/******************************************************************************
*    	File name	: BSP_Init.h 
*		Description : Hardware Configuration File
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 06 / 19
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifndef TEECO_BSPINIT_PROFILE_H
    #define TEECO_BSPINIT_PROFILE_H

#ifdef __cplusplus
 extern "C" {
#endif

void NVIC_Configuration(void);
void BSP_Timer(void);
void BSP_UartInit(void);
void COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);
void TEECO_BSP_Init(void);
void BSP_FSMC_Init(void);
void BSP_I2C_Init(void);
void BSP_SM718Init(void);
void SAA7113H_Setting(void);
void Delay(__IO uint32_t nTime);
void VoicePlay(uint8_t bank);;	 
#ifdef __cplusplus
}
#endif

#endif // TEECO_BSPINIT_PROFILE_H



