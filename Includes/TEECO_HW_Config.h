/**
  ******************************************************************************
  * @file       TEECO_HW_Config.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월13일
  * @brief    이파일은  컴파일옵션및 기기설정에 관한 정의 헤더파일이다.
                
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
#ifndef TEECO_HARDWARE_PROFILE_H
    #define TEECO_HARDWARE_PROFILE_H
#ifdef __cplusplus
 extern "C" {
#endif
/**
	 @todo 보드마다 DDR 속도가 다르므로 주의해야한다.
*/	 
//#define		WATERCELL_PROJECT
	 
//===============================================================================
//#define		UTF8CODE_USE	 				//!< UTF8 Code를 사용할시 컴파일한다.
//#define		KOREA_FS							//!< F/S 에서 한글지원이 되도록한다.
//===============================================================================
#define		LOW_FONT_USE						//!< 내장된 폰트를 사용할때 정의한다.
//!< 아래에서 사용할 폰트를 해제한다.
#define		ARIAL12_FONT_USE			//TableWidget에서 사용한다.
//#define		IBM15_FONT_USE	 
//#define		TIMEROMA12_FONT_USE
//#define		TIMEROMA16_FONT_USE
//#define		ABADI12_FONT_USE
#define		ARIAL18_FONT_USE
//#define		TIME18_FONT_USE
//#define		ABADI10_FONT_USE
//#define		ARIAL18B_FONT_USE
//#define		ROMA29_FONT_USE 
//#define		TIMEROMA9_FONT_USE
//#define		TIMEROMA9B_FONT_USE
//#define		TIMEROMA12B_FONT_USE
#define		TIMEROMA16B_FONT_USE	//TableWidget에서 사용한다.
//#define		MS_SERIEF9_FONT_USE
//#define		LCD5X7_FONT_USE	 
//-------------------------------------------------------------------------------------------------------------------------------------------------			
#define		PC_FONT_USE						//!< PC 로 변환된 폰트를 사용할때 정의한다.
//===============================================================================
//#define   OS_USE                      			//!< FreeRTOS 를 사용할때
//#define	DEBUG_WINIDOW	            	//윈도우관련 디버그메시지 출력
//===============================================================================
#define 	TOUCH_INT_Q	 		        		//터치리딩방식의 조정(터치인터럽트와 타이머의 조합으로 사용한다)
#define     TOUCH_USE                 			//터치를 사용하는 응용
//#define     KEYPAD_USE                  		//키패드를 사용할때
//#define     ENCODER_USE                 		//엔코더스위치를 사용할때
#define     DMA_USE                     			//BMP출력시 DMA를 사용할때
//===============================================================================
//하드웨어 추가삭제에 관련된 정의문
#define	    DISPLAY_CONTROLLER	SM718   //기본적으로 사용한다.
//#define 	W5100A                      			//위즈넷사의 인터넷칩을 사용할때 사용한다. 
#define		W5500	 
//#define     SAA7113H                  			//비디오 디코더를 사용할때 정의한다.
//#define		WIZFI250_USE						//와이파이 모듈을 사용할시 컴파일한다.
//#define		ADV7611_USE						//HDMI를 받아서 비디오에 표시할때 사용한다.
//===============================================================================
//CPU 클록설정 정의문     

//#define		STM32F405			// 405를 사용할때 사용한다.
#define		STM32F427		//427을 사용할때, SDRAM, FMC를 사용한다.
#define     CPUCLK_168MHZ     				//OSC 를 25MHZ 사용해야한다.
//#define     CPUCLK_180MHZ     				//OSC 를 25MHZ 사용해야한다.     
//#define     CPUCLK_200MHZ     				//OSC 를 25MHZ 사용해야한다.     
//#define     CPUCLK_216MHZ     				//OSC 를 25MHZ 사용해야한다.     
//===============================================================================
//SM718의 메모리종류에 따른 정의문이다.
//#define     DDR2_D_TYPE     
#define     DDR2_J_TYPE     
//===============================================================================
//보드구분 관련된 정의문

//반드시 둘중 하나는 선택되어야 한다.
#define     PSRAM_USE       						//PS-RAM 8MByte를 사용할시에 적용한다.
//#define     VRAM_USE       					//PS-RAM 8MByte를 사용할시에 적용한다.
#define	ALPHA_USE							//윈도우의 USER_SCREEN을 더사용하기위해 알파를 제거한다.

//===============================================================================
//LCD 의 기종에따른 설정
//#define	LCD_5Inch      //MQBic 5 Inch
//#define	LCD_SAMSUNG_LMS700KF06      	//삼성전자의 7인치 LCD
//#define	    LCD_TAIWAN_CLAA080          	//대만의 8인치 LCD
#define     LCD_TS8080          						//이노룩스 1024X768
//#define     LCD_1024          						// CHUNGHWA 10.4인치 LCD  1024X768
//#define		LCD1024X600								//7인치 LCD
//#define		LCD17INCH						//1280X1024

//===============================================================================
     
#ifdef LCD_5Inch
#define	LCD_X					800
#define	LCD_Y					480
#endif

#ifdef LCD_SAMSUNG_LMS700KF06
#define	LCD_X					800
#define	LCD_Y					480
#endif

#ifdef LCD_TAIWAN_CLAA080
#define	LCD_X					800
#define	LCD_Y					600
#endif

#ifdef LCD_TS8080
#define	LCD_X					1024
#define	LCD_Y					768
#endif

#ifdef LCD_1024
#define	LCD_X					1024
#define	LCD_Y					768
#endif

#ifdef LCD1024X600
#define	LCD_X					1024
#define	LCD_Y					600
#endif

#ifdef LCD17INCH
#define	LCD_X					1280
#define	LCD_Y					1024
#endif

#define	TSC2003

#ifdef __cplusplus
}
#endif

#endif // TEECO_HARDWARE_PROFILE_H
