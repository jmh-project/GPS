/******************************************************************************
*    	File name	: BSP_SM718.h 
*		Description : Hardware Configuration File
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 06 / 04
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifndef TEECO_SM718_H
    #define TEECO_SM718_H

#ifdef __cplusplus
 extern "C" {
#endif
/******************************************************************************
							Function Prototype
******************************************************************************/
void SM718Test(void);
void SM718_LCDScreenChange(int page);
void LCD_AlphaScreen(int s);
void VideoScreen(int s);
void VideoAlphaScreen(int s);	 
void Lcd_AlphaTransparency(int value);
void SM718_AlphaWiddow(int sx, int sy, int width, int height);	 
#ifdef __cplusplus
}
#endif

#endif // TEECO_SM718_H
