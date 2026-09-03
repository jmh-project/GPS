/******************************************************************************
*    	File name	: BSP_PortI2C.h 
*		Description : Hardware Configuration File
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 06 / 19
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifndef TEECO_BSPPORTI2C_PROFILE_H
    #define TEECO_BSPPORTI2C_PROFILE_H

#ifdef __cplusplus
 extern "C" {
#endif

#define		SDA			GPIO_Pin_8
#define		SCL			GPIO_Pin_7
     
#define		SCL_High()	GPIOB->BSRRL = SCL
#define		SCL_Low()	GPIOB->BSRRH = SCL
#define		SDA_High()	GPIOB->BSRRL = SDA
#define		SDA_Low()	GPIOB->BSRRH = SDA
     
void I2C_Delay(void);
void I2C_PortInit(void);
void I2CSDAPort_Input(void);
void I2CSDAPort_Output(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Clk(void);
void I2C_Clock_Data( u8 data);
void I2C_ACK(void);
void I2C_ACK_M(void);


#ifdef __cplusplus
}
#endif

#endif // TEECO_BSPPORTI2C_PROFILE_H



