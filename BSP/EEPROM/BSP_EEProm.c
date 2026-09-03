/******************************************************************************
*    	File name	: BSP_EEProm.h 
*		Description : I2C EEPROM Control 
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 06 / 01
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "BSP_EEProm.h"

void EEPROMWrite( U16 addr, U8 data)
{
    I2C_Start();
    I2C_Clock_Data(0xa0);           

    I2C_ACK();                      

    I2C_Clock_Data(addr>>8);        

    I2C_ACK();                      

    I2C_Clock_Data(addr);           

	I2C_ACK();                      

    I2C_Clock_Data(data);           

    I2C_ACK();                      

    I2C_Stop();    
    Delay(5);
}

U8 EEPROMRead( U16 addr )
{
    u8 data=0x00;

    I2C_Start();
    I2C_Clock_Data(0xa0);           
    I2C_ACK();                      

    I2C_Clock_Data(addr>>8);
    I2C_ACK();                      

    I2C_Clock_Data(addr);
    I2C_ACK();                      

	I2C_Start();

    I2C_Clock_Data(0xa1);
    I2C_ACK();                      

    I2CSDAPort_Input();

	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x80;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x40;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x20;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x10;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x08;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x04;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x02;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x01;
	I2C_Clk();

    I2C_Stop();
    Delay(5);    
    return ( data );
}
#ifdef __cplusplus
}
#endif

