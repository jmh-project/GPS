#ifndef TEECO_DEFINE
#define TEECO_DEFINE

#ifdef __cplusplus
 extern "C" {
#endif

#include "TEECO_HW_Config.h"
	 
#define 		USB			0
#define 		SD			1     
#define 		RAM			2     
#define  	RAM_DEVICE_ADDR   	0x68180000	//
#define  	RAM_DEVICE_SIZE   	0x1E80000		//

#define		ENGLISH	            1		//!< 로딩된폰트의 종류
#define		HANGUL	            2
#define		ENG_HANGUL	        3	 
	 
#ifdef PSRAM_USE //!< 부트로더에서 사용한다.
#define		NAND_MBR_ADDRESS_RESOURCE	    		0x64000000
#define		SOURCE_BMP_ADDR_RESOURCE				0x64020000
#define		CONV_BMP_ADDR_RESOURCE	    			0x643E0000
#endif

// 아래의 설정은 VRAM 을 사용할때 사용한다.부트로더에서 사용한다.
#ifdef VRAM_USE
#define		NAND_MBR_ADDRESS_RESOURCE				0x6B1B2000
#define		SOURCE_BMP_ADDR_RESOURCE				0x6ABF2000
#define		CONV_BMP_ADDR_RESOURCE					0x6AED2000
#endif

// 아래의 설정은 PS-RAM 을 사용할때 사용한다. 시스템에서 사용한다.
#ifdef PSRAM_USE
#define		NAND_MBR_ADDRESS	    0x64000000
#define		SOURCE_BMP_ADDR		0x641A4000
#define		CONV_BMP_ADDR		0x643A4000
//#define		DIR_ADDR			    0x645A4000	// 2MB Allocation
#endif

// 아래의 설정은 VRAM 을 사용할때 사용한다.
#ifndef PSRAM_USE
#define		NAND_MBR_ADDRESS	0x6B1B2000
#define		SOURCE_BMP_ADDR		0x6ABF2000
#define		CONV_BMP_ADDR		    0x6AED2000
#define		DIR_ADDR			0x6B7F2000	// 2MB Allocation
#endif
	 
//KeyPad Code Define     
#define     LEFT_KEY    1
#define     UP_KEY      2
#define     DN_KEY      3
#define     RIGHT_KEY   4
#define     ENTER_KEY   5
#define     BACK_KEY    6
#define     SAVE_KEY    7

//USB Select
#define     DEVICE     0
#define     HOST       1

	 
extern volatile int g_Touch_Enable;
#define		TOUCH_Enable()		g_Touch_Enable = ON	 
#define		TOUCH_Disable()		g_Touch_Enable = OFF	 
	 
#ifndef   	TRUE
#define 	TRUE 		1   
#endif

#define 	true 		1   
#define 	ON 			1   
#define     GOOD        1
#define     YES         1

#ifndef FALSE
#define 	FALSE 		0
#endif

#define 	false 		0
#define 	OFF 		0
#define     ERROR       0
#define     NO          0

#define 	OK			1
#define 	FAIL		0

#define		RUN			1
#define		START		1
#define		STOP		0

#define		fArial12			1	// 16X16 Font
#define		fArial18			2	// 24X24 Font
#define		fIBM15			3	// 24X24 Font
#define		fRoma8b			4	// 8X16 Font
#define		fTimeRoma9		5	// 16X16 Font
#define		fTimeRoma9b		6	// 16X16 Font
#define		fTimeRoma12		7	// 16X16 Font
#define		fTimeRoma12b	8	// 16X16 Font
#define		fTimeRoma16		9	// 24X24 Font
#define		fTimeRoma16b	10	// 24X24 Font
#define		fTime18			11	// 24X24 Font
#define		fRoma29			12	// 40X40 Font
#define		fAbadi10			13	// 16X16 Font
#define		fAbadi12			14	// 16X16 Font
#define		fMS12			    15	// 16X16 Font

#ifdef __cplusplus
}
#endif

#endif

