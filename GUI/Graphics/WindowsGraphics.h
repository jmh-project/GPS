/******************************************************************************
*    	File name	: WidowsGraphics.h 
*		Description : Configuration File
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 07 / 10
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifndef TEECO_WIDOWS_GRAPHICS_PROFILE_H
    #define TEECO_WIDOWS_GRAPHICS_PROFILE_H

#ifdef __cplusplus
 extern "C" {
#endif
// Hardware mapping Screen
#define		LCD_SCREEN							1
#define		LCD_ALPHA_SCREEN				2
//#define		CRT_SCREEN						3
//#define		CRT_ALPHA_SCREEN			4
#define		VIDEO_SCREEN						3
#define		VIDEO_ALPHA_SCREEN			4
#define		CAPTURE1_SCREEN					5
#define		CAPTURE2_SCREEN					6

// Window mapping Screen
#define		CONTAINER_WINDOW1_SCREEN		7
#define		CONTAINER_WINDOW2_SCREEN		8
#define		CONTAINER_WINDOW3_SCREEN		9
#define		CONTAINER_WINDOW4_SCREEN		10
#define		CONTAINER_WINDOW5_SCREEN		11

#define		CONTAINER_WIDGET1_SCREEN		12
#define		CONTAINER_WIDGET2_SCREEN		13
#define		CONTAINER_WIDGET3_SCREEN		14
#define		CONTAINER_WIDGET4_SCREEN		15
#define		CONTAINER_WIDGET5_SCREEN		16

#define		DOUBLE_SCREEN						17
//#define		DESKTOP_SCREEN					20
//#define		DESKTOPWIDGET_SCREEN		21

// SYSTEM Reserved Screen

#define		LCD_ALPHA_CONTAINER_SCREEN		18
#define		LCD_ALPHA_WIDGET_SCREEN			19
#define		VIDEO_ALPHA_CONTAINER_SCREEN	20
#define		VIDEO_ALPHA_WIDGET_SCREEN		21
#define		LCD_ALPHA_DOUBLE_SCREEN			22
#define		VIDEO_ALPHA_DOUBLE_SCREEN		23

// User mapping Screen
#define		USER_SCREEN1					24
#define		USER_SCREEN2					25
#define		USER_SCREEN3					26
#define		USER_SCREEN4					27
#define		USER_SCREEN5					28
#define		USER_SCREEN6					29
//아래는 8인치 까지만 사용할수 있다.
#define		USER_SCREEN7					30
#define		USER_SCREEN8					31
#define		USER_SCREEN9					32
#define		USER_SCREEN10					33
//아래는 7인치까지만 사용할수있다.
#define		USER_SCREEN11					34
#define		USER_SCREEN12					35
#define		USER_SCREEN13					36
#define		USER_SCREEN14					37
#define		USER_SCREEN15					38
#define		USER_SCREEN16					39
#define		USER_SCREEN17					40
#define		USER_SCREEN18					41
#define		USER_SCREEN19					42
#define		USER_SCREEN20					43
#define		USER_SCREEN21					44
#define		USER_SCREEN22					45
#define		USER_SCREEN23					46
#define		USER_SCREEN24					47
#define		USER_SCREEN25					48
#define		USER_SCREEN26					49
#define		USER_SCREEN27					50
#define		USER_SCREEN28					51
#define		USER_SCREEN29					52
#define		USER_SCREEN30					53
#define		USER_SCREEN31					54
#define		USER_SCREEN32					55
#define		USER_SCREEN33					56
#define		USER_SCREEN34					57
#define		USER_SCREEN35					58
#define		USER_SCREEN36					59
#define		USER_SCREEN37					60
#define		USER_SCREEN38					61
#define		USER_SCREEN39					62
#define		USER_SCREEN40					63
#define		USER_SCREEN41					64

#define		AUX_USER_SCREEN1				VIDEO_SCREEN 	//VIDEO_SCREEN
#define		AUX_USER_SCREEN2				VIDEO_ALPHA_SCREEN 	//VIDEO_ALPHA_SCREEN
#define		AUX_USER_SCREEN3				CAPTURE1_SCREEN 	//CAPTURE1_SCREEN
#define		AUX_USER_SCREEN4				CAPTURE2_SCREEN 	//CAPTURE2_SCREEN
#define		AUX_USER_SCREEN5				VIDEO_ALPHA_CONTAINER_SCREEN 	//VIDEO_ALPHA_CONTAINER_SCREEN
#define		AUX_USER_SCREEN6				VIDEO_ALPHA_WIDGET_SCREEN 	//VIDEO_ALPHA_WIDGET_SCREEN
#define		AUX_USER_SCREEN7				VIDEO_ALPHA_DOUBLE_SCREEN 	//VIDEO_ALPHA_DOUBLE_SCREEN


//!< LCD 의 해상도가 1024X768 일경우의 리소스 스크린지정
#ifdef LCD17INCH 
//64스크린은 커파일에러를방지하기웨해 지정은 했지만 사용하지 않는다.
#define		SYSTEM_ICON64_WHITE_SCREEN	VIDEO_SCREEN
#define		SYSTEM_ICON64_GRAY_SCREEN		VIDEO_ALPHA_SCREEN
#define		SYSTEM_ICON48_SCREEN				CAPTURE1_SCREEN
#define		SYSTEM_ICON32_SCREEN				CAPTURE2_SCREEN
#endif


//!< LCD 의 해상도가 1024X768 일경우의 리소스 스크린지정
#if defined (LCD_TS8080) || defined(LCD_1024) || defined(LCD1024X600) 

#define		SYSTEM_ICON64_WHITE_SCREEN	VIDEO_SCREEN
#define		SYSTEM_ICON64_GRAY_SCREEN		VIDEO_ALPHA_SCREEN
#define		SYSTEM_ICON48_SCREEN				CAPTURE1_SCREEN
#define		SYSTEM_ICON32_SCREEN				CAPTURE2_SCREEN
#endif

//!< LCD 의 해상도가 800X600 일경우의 리소스 스크린지정
#ifdef LCD_TAIWAN_CLAA080
#define		SYSTEM_ICON64_WHITE_SCREEN	VIDEO_SCREEN
#define		SYSTEM_ICON64_WHITE_SCREEN2	VIDEO_ALPHA_SCREEN
#define		SYSTEM_ICON64_GRAY_SCREEN	    CAPTURE1_SCREEN
#define		SYSTEM_ICON64_GRAY_SCREEN2	CAPTURE2_SCREEN
#define		SYSTEM_ICON48_SCREEN				VIDEO_ALPHA_CONTAINER_SCREEN
#define		SYSTEM_ICON48_SCREEN2			VIDEO_ALPHA_WIDGET_SCREEN
#define		SYSTEM_ICON32_SCREEN				VIDEO_ALPHA_DOUBLE_SCREEN
#endif
//====================================================================
#define		GRAM_BASE						(0x68000000)
#define		LCD_SCREEN_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 0)
#define		LCD_ALPHA_SCREEN_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 1)
//#define		CRT_SCREEN_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 2)
//#define		CRT_ALPHA_SCREEN_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 3)
#define		VIDEO_SCREEN_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 2)
#define		VIDEO_ALPHA_SCREEN_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) *3)

#define		CAPTURE1_SCREEN_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 4)
#define		CAPTURE2_SCREEN_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 5)

#define		CONTAINER_WINDOW1_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 6)
#define		CONTAINER_WINDOW2_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 7)
#define		CONTAINER_WINDOW3_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 8)
#define		CONTAINER_WINDOW4_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 9)
#define		CONTAINER_WINDOW5_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 10)

#define		CONTAINER_WIDGET1_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 11)
#define		CONTAINER_WIDGET2_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 12)
#define		CONTAINER_WIDGET3_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 13)
#define		CONTAINER_WIDGET4_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 14)
#define		CONTAINER_WIDGET5_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 15)

#define		DOUBLE_SCREEN_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 16)

//#define		DESKTOP_SCREEN_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 19)
//#define		DESKTOPWIDGET_SCREEN_ADDR		GRAM_BASE+((LCD_X * LCD_Y * 2) * 20)

//#define		LCD_ALPHA_CONTAINER_ADDR		GRAM_BASE+((LCD_X * LCD_Y * 2) * 17)
#define		LCD_ALPHA_CONTAINER_ADDR		((LCD_X * LCD_Y * 2) * 17)
#define		LCD_ALPHA_WIDGET_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 18)
#define		VIDEO_ALPHA_CONTAINER_ADDR		GRAM_BASE+((LCD_X * LCD_Y * 2) * 19)
#define		VIDEO_ALPHA_WIDGET_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 20)
#define		LCD_ALPHA_DOUBLE_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 21)
#define		VIDEO_ALPHA_DOUBLE_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 22)

#ifdef LCD17INCH
#define		USER_SCREEN1_ADDR					LCD_ALPHA_SCREEN_ADDR
#define		USER_SCREEN2_ADDR					LCD_ALPHA_CONTAINER_ADDR
#define		USER_SCREEN3_ADDR					LCD_ALPHA_WIDGET_ADDR
#define		USER_SCREEN4_ADDR					LCD_ALPHA_DOUBLE_ADDR
#endif

#ifndef LCD17INCH
#define		USER_SCREEN1_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 23)
#define		USER_SCREEN2_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 24)
#define		USER_SCREEN3_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 25)
#define		USER_SCREEN4_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 26)
#endif


#define		USER_SCREEN5_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 27)
#define		USER_SCREEN6_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 28)
#define		USER_SCREEN7_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 29)
#define		USER_SCREEN8_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 30)
#define		USER_SCREEN9_ADDR					GRAM_BASE+((LCD_X * LCD_Y * 2) * 31)
#define		USER_SCREEN10_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 32)

#define		USER_SCREEN11_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 33)
#define		USER_SCREEN12_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 34)
#define		USER_SCREEN13_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 35)
#define		USER_SCREEN14_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 36)
#define		USER_SCREEN15_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 37)

#define		USER_SCREEN16_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 38)
#define		USER_SCREEN17_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 39)
#define		USER_SCREEN18_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 40)
#define		USER_SCREEN19_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 41)
#define		USER_SCREEN20_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 42)

#define		USER_SCREEN21_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 43)
#define		USER_SCREEN22_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 44)
#define		USER_SCREEN23_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 45)
#define		USER_SCREEN24_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 46)

#define		USER_SCREEN25_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 47)
#define		USER_SCREEN26_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 48)
#define		USER_SCREEN27_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 49)
#define		USER_SCREEN28_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 50)


#define		USER_SCREEN29_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 51)
#define		USER_SCREEN30_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 52)


#define		USER_SCREEN31_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 53)
#define		USER_SCREEN32_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 54)
#define		USER_SCREEN33_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 55)
#define		USER_SCREEN34_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 56)
#define		USER_SCREEN35_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 57)
#define		USER_SCREEN36_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 58)
#define		USER_SCREEN37_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 59)
#define		USER_SCREEN38_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 60)

#define		USER_SCREEN39_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 61)
#define		USER_SCREEN40_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 62)
#define		USER_SCREEN41_ADDR				GRAM_BASE+((LCD_X * LCD_Y * 2) * 63)
//여기까지 800X600사용가능

#define		AUX_USER_SCREEN1_ADDR			VIDEO_SCREEN_ADDR
#define		AUX_USER_SCREEN2_ADDR			VIDEO_ALPHA_SCREEN_ADDR
#define		AUX_USER_SCREEN3_ADDR			CAPTURE1_SCREEN_ADDR
#define		AUX_USER_SCREEN4_ADDR			CAPTURE2_SCREEN_ADDR
#define		AUX_USER_SCREEN5_ADDR			VIDEO_ALPHA_CONTAINER_ADDR

#define		AUX_USER_SCREEN6_ADDR			VIDEO_ALPHA_WIDGET_ADDR
#define		AUX_USER_SCREEN7_ADDR			VIDEO_ALPHA_DOUBLE_ADDR
//#define		AUX_USER_SCREEN8_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 19)
//#define		AUX_USER_SCREEN9_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 20)
//#define		AUX_USER_SCREEN10_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 21)
//#define		AUX_USER_SCREEN11_ADDR			GRAM_BASE+((LCD_X * LCD_Y * 2) * 22)

// GPU 에서 사용하는 맵의 정의
#define		GPU_LCD_SCREEN					((LCD_X * LCD_Y * 2) * 0)
#define		GPU_LCD_ALPHA_SCREEN			((LCD_X * LCD_Y * 2) * 1)
//#define		GPU_CRT_SCREEN					((LCD_X * LCD_Y * 2) * 2)
//#define		GPU_CRT_ALPHA_SCREEN			((LCD_X * LCD_Y * 2) * 3)
#define		GPU_VIDEO_SCREEN				((LCD_X * LCD_Y * 2) * 2)
//#define		GPU_VIDEO_ALPHA_SCREEN			((LCD_X * LCD_Y * 2) * 3)
#define		GPU_VIDEO_ALPHA_SCREEN			((LCD_X * LCD_Y * 2) * 3)

#define		GPU_CAPTURE1_SCREEN			((LCD_X * LCD_Y * 2) * 4)
#define		GPU_CAPTURE2_SCREEN			((LCD_X * LCD_Y * 2) * 5)

#define		GPU_CONTAINER_WINDOW1			((LCD_X * LCD_Y * 2) * 6)
#define		GPU_CONTAINER_WINDOW2			((LCD_X * LCD_Y * 2) * 7)
#define		GPU_CONTAINER_WINDOW3			((LCD_X * LCD_Y * 2) * 8)
#define		GPU_CONTAINER_WINDOW4			((LCD_X * LCD_Y * 2) * 9)
#define		GPU_CONTAINER_WINDOW5			((LCD_X * LCD_Y * 2) * 10)

#define		GPU_CONTAINER_WIDGET1			((LCD_X * LCD_Y * 2) * 11)
#define		GPU_CONTAINER_WIDGET2			((LCD_X * LCD_Y * 2) * 12)
#define		GPU_CONTAINER_WIDGET3			((LCD_X * LCD_Y * 2) * 13)
#define		GPU_CONTAINER_WIDGET4			((LCD_X * LCD_Y * 2) * 14)
#define		GPU_CONTAINER_WIDGET5			((LCD_X * LCD_Y * 2) * 15)

#define		GPU_DOUBLE_SCREEN				((LCD_X * LCD_Y * 2) * 16)
//#define		GPU_DESKTOP_SCREEN				((LCD_X * LCD_Y * 2) * 19)
//#define		GPU_DESKTOPWIDGET_SCREEN		((LCD_X * LCD_Y * 2) * 20)

#define		GPU_LCD_ALPHA_CONTAINER_ADDR	((LCD_X * LCD_Y * 2) * 17)
#define		GPU_LCD_ALPHA_WIDGET_ADDR		((LCD_X * LCD_Y * 2) * 18)
#define		GPU_VIDEO_ALPHA_CONTAINER_ADDR	((LCD_X * LCD_Y * 2) * 19)
#define		GPU_VIDEO_ALPHA_WIDGET_ADDR		((LCD_X * LCD_Y * 2) * 20)
#define		GPU_LCD_ALPHA_DOUBLE_ADDR		((LCD_X * LCD_Y * 2) * 21)
#define		GPU_VIDEO_ALPHA_DOUBLE_ADDR		((LCD_X * LCD_Y * 2) * 22)

#ifdef LCD17INCH
#define		GPU_USER_SCREEN1				LCD_ALPHA_SCREEN_ADDR
#define		GPU_USER_SCREEN2				GPU_LCD_ALPHA_CONTAINER_ADDR
#define		GPU_USER_SCREEN3				GPU_LCD_ALPHA_WIDGET_ADDR
#define		GPU_USER_SCREEN4				GPU_LCD_ALPHA_DOUBLE_ADDR
#endif

#ifndef LCD17INCH
#define		GPU_USER_SCREEN1				((LCD_X * LCD_Y * 2) * 23)
#define		GPU_USER_SCREEN2				((LCD_X * LCD_Y * 2) * 24)
#define		GPU_USER_SCREEN3				((LCD_X * LCD_Y * 2) * 25)
#define		GPU_USER_SCREEN4				((LCD_X * LCD_Y * 2) * 26)
#endif

#define		GPU_USER_SCREEN5				((LCD_X * LCD_Y * 2) * 27)
#define		GPU_USER_SCREEN6				((LCD_X * LCD_Y * 2) * 28)
#define		GPU_USER_SCREEN7				((LCD_X * LCD_Y * 2) * 29)
#define		GPU_USER_SCREEN8				((LCD_X * LCD_Y * 2) * 30)
#define		GPU_USER_SCREEN9				((LCD_X * LCD_Y * 2) * 31)
#define		GPU_USER_SCREEN10				((LCD_X * LCD_Y * 2) * 32)

#define		GPU_USER_SCREEN11				((LCD_X * LCD_Y * 2) * 33)
#define		GPU_USER_SCREEN12				((LCD_X * LCD_Y * 2) * 34)
#define		GPU_USER_SCREEN13				((LCD_X * LCD_Y * 2) * 35)
#define		GPU_USER_SCREEN14				((LCD_X * LCD_Y * 2) * 36)
#define		GPU_USER_SCREEN15				((LCD_X * LCD_Y * 2) * 37)

#define		GPU_USER_SCREEN16				((LCD_X * LCD_Y * 2) * 38)
#define		GPU_USER_SCREEN17				((LCD_X * LCD_Y * 2) * 39)
#define		GPU_USER_SCREEN18				((LCD_X * LCD_Y * 2) * 40)
#define		GPU_USER_SCREEN19				((LCD_X * LCD_Y * 2) * 41)
#define		GPU_USER_SCREEN20				((LCD_X * LCD_Y * 2) * 42)

#define		GPU_USER_SCREEN21				((LCD_X * LCD_Y * 2) * 43)
#define		GPU_USER_SCREEN22				((LCD_X * LCD_Y * 2) * 44)
#define		GPU_USER_SCREEN23				((LCD_X * LCD_Y * 2) * 45)
#define		GPU_USER_SCREEN24				((LCD_X * LCD_Y * 2) * 46)
#define		GPU_USER_SCREEN25				((LCD_X * LCD_Y * 2) * 47)
#define		GPU_USER_SCREEN26				((LCD_X * LCD_Y * 2) * 48)
#define		GPU_USER_SCREEN27				((LCD_X * LCD_Y * 2) * 49)
#define		GPU_USER_SCREEN28				((LCD_X * LCD_Y * 2) * 50)
#define		GPU_USER_SCREEN29				((LCD_X * LCD_Y * 2) * 51)
#define		GPU_USER_SCREEN30				((LCD_X * LCD_Y * 2) * 52)
#define		GPU_USER_SCREEN31				((LCD_X * LCD_Y * 2) * 53)
#define		GPU_USER_SCREEN32				((LCD_X * LCD_Y * 2) * 54)
#define		GPU_USER_SCREEN33				((LCD_X * LCD_Y * 2) * 55)
#define		GPU_USER_SCREEN34				((LCD_X * LCD_Y * 2) * 56)
#define		GPU_USER_SCREEN35				((LCD_X * LCD_Y * 2) * 57)
#define		GPU_USER_SCREEN36				((LCD_X * LCD_Y * 2) * 58)
#define		GPU_USER_SCREEN37				((LCD_X * LCD_Y * 2) * 59)
#define		GPU_USER_SCREEN38				((LCD_X * LCD_Y * 2) * 60)
#define		GPU_USER_SCREEN39				((LCD_X * LCD_Y * 2) * 61)
#define		GPU_USER_SCREEN40				((LCD_X * LCD_Y * 2) * 62)
#define		GPU_USER_SCREEN41				((LCD_X * LCD_Y * 2) * 63)

#define		AUX_GPU_USER_SCREEN1			GPU_VIDEO_SCREEN
#define		AUX_GPU_USER_SCREEN2			GPU_VIDEO_ALPHA_SCREEN
#define		AUX_GPU_USER_SCREEN3			GPU_CAPTURE1_SCREEN
#define		AUX_GPU_USER_SCREEN4			GPU_CAPTURE2_SCREEN
#define		AUX_GPU_USER_SCREEN5			GPU_VIDEO_ALPHA_CONTAINER_ADDR
#define		AUX_GPU_USER_SCREEN6			GPU_VIDEO_ALPHA_WIDGET_ADDR
#define		AUX_GPU_USER_SCREEN7			GPU_VIDEO_ALPHA_DOUBLE_ADDR
//#define		AUX_GPU_USER_SCREEN8			((LCD_X * LCD_Y * 2) * 19)
//#define		AUX_GPU_USER_SCREEN9			((LCD_X * LCD_Y * 2) * 20)
//#define		AUX_GPU_USER_SCREEN10		((LCD_X * LCD_Y * 2) * 21)
//#define		AUX_GPU_USER_SCREEN11		((LCD_X * LCD_Y * 2) * 22)

typedef enum 
{
	Font_Abadi10 = 1,
	Font_Abadi12 = 2,
	Font_Arial12 = 3,
	Font_Arial18 = 4,
	Font_IBM15 = 5,
	Font_Roma8b = 6,
	Font_TimeRoma9 = 7,
	Font_TimeRoma9b = 8,
	Font_TimeRoma12 = 9,
	Font_TimeRoma12b = 11,
	Font_TimeRoma16 = 11,
	Font_TimeRoma16b = 12,
	Font_Time18 = 13,
	Font_Roma29 = 14	
}FontType;



void SetFont(int no);
void SetColor(U16 color);
void Plot(U16 x, U16 y);
void SetGraphicsPage(int Page_No);
void LineTo(U16 endx, U16 endy);
void Arc(U16 xc, U16 yc, U16 r, U16 start, U16 end);
void MoveTo(U16 x, U16 y);
void Line(U16 startx, U16 starty, U16 endx, U16 endy);
void Elipse(U16 xc, U16 yc, U16 a, U16 b);
void FillElipse(U16 xc, U16 yc, U16 a, U16 b);
void FillRect(U16 sx, U16 sy, U16 xlen, U16 ylen);
void HLine(U16 x, U16 y, U16 len);
void Hline(U16 x1, U16 x2, U16 y);
void Swap(U16 *a, U16 *b);
void VLine(U16 x, U16 y, U16 len);
void CrossLine(U16 x, U16 y, U16 len);
void HLineDot(U16 x, U16 y, U16 len);
void VLineDot(U16 x, U16 y, U16 len);
void Rectangle(U16 sx, U16 sy, U16 xlen, U16 ylen);
void SetFont(int no);
void TextOut(u16 sx, u16 sy, const char *s);
void Init2D(void);
void FillRect2D(int xs, int ys, int xe, int ye);
void Rotate(U32 src, U32 dest, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy);
void HostBlt(U16 dx, U16 dy, U16 dimx, U16 dimy);
void BitBlt(int srcPage, int destPage, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy);
void Line2D(int x1, int y1, int x2, int y2);
void HLine2D(int x1, int x2, int y);
void VLine2D(int y1, int y2, int x);
void WaitForNotBusy(void);
void Wait2D_Engine(void);
int BmpLoad(int page, U16 sx, U16 sy, char *name);
int SetTransparency(U32 enable,  U32 tSelect, U32 tMatch, U32 ulColor);
int SetClip(U16 enable, int left, int top, int right, int bottom, int side);
void SM718Reset(void);
void SetPixelFormat( unsigned long bpp );
void SetBrush(int brush);
void SetPen(int pen);
void PaintBitBlt(int srcPage, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy);
void FontBitBlt(int page, int CompPage, int ref_page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy);
void FontBitBltOne(int page, int CompPage, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy);
void BitBlt16(int srcPage, int destPage, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy);
int LowTextLen(char *s, int Font);

void WinPlot(U16 x, U16 y);
void WinLine2D(int x1, int y1, int x2, int y2);
void WinHLine2D(int x1, int x2, int y);
void WinVLine2D(int y1, int y2, int x);
void WinFillRect2D(int xs, int ys, int xe, int ye);
void PlotWin(U16 x, U16 y);
int PaintBmpLoad(U16 sx, U16 sy, char *filename);
void WinFillElipse(U16 xc, U16 yc, U16 a, U16 b);
void WinElipse(U16 xc, U16 yc, U16 a, U16 b);
void WinArc(U16 xc, U16 yc, U16 r, U16 start, U16 end);
void WinLineTo(U16 endx, U16 endy);
void WinMoveTo(U16 x, U16 y);
void WinRect(U16 sx, U16 sy, U16 xlen, U16 ylen);
int LoadFont(int no, char *fontname);
int LoadHangulFont(int no, char *fontname);
int DrawText(int sx, int sy, const char *text);

//이함수는 ClearTextOut 을 사용하기전에 반드시 설정하고 사용해야한다.
int CFontSet(int font_screen_page, int font_xsize, int font_ysize, int start_y);
int ClearTextOut(int sx, int sy, const char *string);
//클리어 폰트 사용할시 문자의 길이를 구한다.
int Get_CFontText_Width(int font_xsize, char *str);
//클리어폰트를 사용하기전에 폰트를 설정한다.
int SystemFontSet(int backcolor, int font);

//!< 아이콘을 현재화면에 출력하는 함수
int IconOut64W(int sx, int sy, char *icon_name);
int IconOut64G(int sx, int sy, char *icon_name);
int IconOut48W(int sx, int sy, char *icon_name);
int IconOut48G(int sx, int sy, char *icon_name);
int IconOut32W(int sx, int sy, char *icon_name);
int IconOut32G(int sx, int sy, char *icon_name);

void SetFontGap(int gap);
void SetFontSpaceGap(int gap);
void MemoryDMA(U32 src, U32 dest, U32 size);
int DMABmpDisplay(U16 sx, U16 sy, char *filename);
void SetHangulFont(int no);
int LoadAliasingFont(char *fontname);
int SetFontAliasing(char *fontname);
int TextOutAliasing(int sx, int sy, char *str, uint16_t color, uint16_t back_color, char *font);
int TextLenAliasing(char *str, char *font);
/**********************************************************************
 *
 * DrawingEngineRegs.h
 *
 * This file contains the defintions for the Drawing Engine registers.
 *
 * (C) 2002 Silicon Motion, Inc.
 * Written by Frido Garritsen
 *
 * Version 1.4 @ 10/21/02 17:05:10
 *
 **********************************************************************/

#define	ROP2			1
#define	ROP3			2


/* Raster Op 2 functions */
#define ROP2_XOR        0x06
#define ROP2_AND        0x08
#define ROP2_COPY       0x0C
#define ROP2_OR         0x0E

#define ROP2_BLACK      0x00
#define ROP2_DSon       0x01
#define ROP2_DSna       0x02
#define ROP2_Sn         0x03
#define ROP2_SDna       0x04
#define ROP2_Dn         0x05
#define ROP2_DSx        0x06
#define ROP2_SDan       0x07
#define ROP2_DSa        0x08
#define ROP2_SDnx       0x09
#define ROP2_D          0x0A
#define ROP2_DSno       0x0B
#define ROP2_S          0x0C
#define ROP2_SDno       0x0D
#define ROP2_DSo        0x0E
#define ROP2_WHITE      0x0F



#define 	REG_DE_SOURCE                   *(volatile U32 *)(0x6BE00000+0x100000)
#define 	REG_DE_DESTINATION              *(volatile U32 *)(0x6BE00000+0x100004)
#define 	REG_DE_DIMENSION                *(volatile U32 *)(0x6BE00000+0x100008)
#define 	REG_DE_CONTROL                  *(volatile U32 *)(0x6BE00000+0x10000C)
#define 	REG_DE_PITCH                    *(volatile U32 *)(0x6BE00000+0x100010)
#define 	REG_DE_FOREGROUND               *(volatile U32 *)(0x6BE00000+0x100014)
#define 	REG_DE_BACKGROUND               *(volatile U32 *)(0x6BE00000+0x100018)
#define 	REG_DE_STRETCH_FORMAT           *(volatile U32 *)(0x6BE00000+0x10001C)
#define 	REG_DE_COLOR_COMPARE            *(volatile U32 *)(0x6BE00000+0x100020)
#define 	REG_DE_COLOR_COMPARE_MASK       *(volatile U32 *)(0x6BE00000+0x100024)
#define 	REG_DE_MASKS                    *(volatile U32 *)(0x6BE00000+0x100028)
#define 	REG_DE_CLIP_TL                  *(volatile U32 *)(0x6BE00000+0x10002C)
#define 	REG_DE_CLIP_BR                  *(volatile U32 *)(0x6BE00000+0x100030)
#define 	REG_DE_MONO_PATTERN_LOW         *(volatile U32 *)(0x6BE00000+0x100034)
#define 	REG_DE_MONO_PATTERN_HIGH        *(volatile U32 *)(0x6BE00000+0x100038)
#define 	REG_DE_WINDOW_WIDTH             *(volatile U32 *)(0x6BE00000+0x10003C)
#define 	REG_DE_WINDOW_SOURCE_BASE       *(volatile U32 *)(0x6BE00000+0x100040)
#define 	REG_DE_WINDOW_DESTINATION_BASE  *(volatile U32 *)(0x6BE00000+0x100044)
#define 	REG_DE_ALPHA                    *(volatile U32 *)(0x6BE00000+0x100048)
#define 	REG_DE_WRAP                     *(volatile U32 *)(0x6BE00000+0x10004C)
#define 	REG_DE_STATUS                   *(volatile U32 *)(0x6BE00000+0x100050)
#define 	REG_DE_DATA_PORT                *(volatile U16 *)(0x6BE00000+0x110000)

//#define 	PITCH(width, bpp)           	(((width) * (bpp) / 8 + 15) & ~15)

#define		SYSTEM_CONTROL					*(volatile U32 *)0x6BE00000

#define 	BPP16_RED    	0x0000f800
#define 	BPP16_GREEN  	0x000007e0
#define 	BPP16_BLUE   	0x0000001f
#define 	BPP16_WHITE  	0x0000ffff
#define 	BPP16_GRAY   	0x00008410
#define 	BPP16_YELLOW 	0x0000ffe0
#define 	BPP16_CYAN   	0x000007ff
#define 	BPP16_PINK   	0x0000f81f
#define 	BPP16_BLACK  	0x00000000

// 2D registers.

#define DE_SOURCE                                       0x100000
#define DE_SOURCE_WRAP                                  31:31
#define DE_SOURCE_WRAP_DISABLE                          0
#define DE_SOURCE_WRAP_ENABLE                           1
#define DE_SOURCE_X_K1                                  29:16
#define DE_SOURCE_Y_K2                                  15:0

#define DE_DESTINATION                                  0x100004
#define DE_DESTINATION_WRAP                             31:31
#define DE_DESTINATION_WRAP_DISABLE                     0
#define DE_DESTINATION_WRAP_ENABLE                      1
#define DE_DESTINATION_X                                28:16
#define DE_DESTINATION_Y                                15:0

#define DE_DIMENSION                                    0x100008
#define DE_DIMENSION_X                                  28:16
#define DE_DIMENSION_Y_ET                               15:0

#define DE_CONTROL                                      0x10000C
#define DE_CONTROL_STATUS                               31:31
#define DE_CONTROL_STATUS_STOP                          0
#define DE_CONTROL_STATUS_START                         1
#define DE_CONTROL_PATTERN                              30:30
#define DE_CONTROL_PATTERN_MONO                         0
#define DE_CONTROL_PATTERN_COLOR                        1
#define DE_CONTROL_UPDATE_DESTINATION_X                 29:29
#define DE_CONTROL_UPDATE_DESTINATION_X_DISABLE         0
#define DE_CONTROL_UPDATE_DESTINATION_X_ENABLE          1
#define DE_CONTROL_QUICK_START                          28:28
#define DE_CONTROL_QUICK_START_DISABLE                  0
#define DE_CONTROL_QUICK_START_ENABLE                   1
#define DE_CONTROL_DIRECTION                            27:27
#define DE_CONTROL_DIRECTION_LEFT_TO_RIGHT              0
#define DE_CONTROL_DIRECTION_RIGHT_TO_LEFT              1
#define DE_CONTROL_MAJOR                                26:26
#define DE_CONTROL_MAJOR_X                              0
#define DE_CONTROL_MAJOR_Y                              1
#define DE_CONTROL_STEP_X                               25:25
#define DE_CONTROL_STEP_X_POSITIVE                      0
#define DE_CONTROL_STEP_X_NEGATIVE                      1
#define DE_CONTROL_STEP_Y                               24:24
#define DE_CONTROL_STEP_Y_POSITIVE                      0
#define DE_CONTROL_STEP_Y_NEGATIVE                      1
#define DE_CONTROL_STRETCH                              23:23
#define DE_CONTROL_STRETCH_DISABLE                      0
#define DE_CONTROL_STRETCH_ENABLE                       1
#define DE_CONTROL_HOST                                 22:22
#define DE_CONTROL_HOST_COLOR                           0
#define DE_CONTROL_HOST_MONO                            1
#define DE_CONTROL_LAST_PIXEL                           21:21
#define DE_CONTROL_LAST_PIXEL_OFF                       0
#define DE_CONTROL_LAST_PIXEL_ON                        1
#define DE_CONTROL_COMMAND                              20:16
#define DE_CONTROL_COMMAND_BITBLT                       0
#define DE_CONTROL_COMMAND_RECTANGLE_FILL               1
#define DE_CONTROL_COMMAND_DE_TILE                      2
#define DE_CONTROL_COMMAND_TRAPEZOID_FILL               3
#define DE_CONTROL_COMMAND_ALPHA_BLEND                  4
#define DE_CONTROL_COMMAND_RLE_STRIP                    5
#define DE_CONTROL_COMMAND_SHORT_STROKE                 6
#define DE_CONTROL_COMMAND_LINE_DRAW                    7
#define DE_CONTROL_COMMAND_HOST_WRITE                   8
#define DE_CONTROL_COMMAND_HOST_READ                    9
#define DE_CONTROL_COMMAND_HOST_WRITE_BOTTOM_UP         10
#define DE_CONTROL_COMMAND_ROTATE                       11
#define DE_CONTROL_COMMAND_FONT                         12
#define DE_CONTROL_COMMAND_TEXTURE_LOAD                 15
#define DE_CONTROL_ROP_SELECT                           15:15
#define DE_CONTROL_ROP_SELECT_ROP3                      0
#define DE_CONTROL_ROP_SELECT_ROP2                      1
#define DE_CONTROL_ROP2_SOURCE                          14:14
#define DE_CONTROL_ROP2_SOURCE_BITMAP                   0
#define DE_CONTROL_ROP2_SOURCE_PATTERN                  1
#define DE_CONTROL_MONO_DATA                            13:12
#define DE_CONTROL_MONO_DATA_NOT_PACKED                 0
#define DE_CONTROL_MONO_DATA_8_PACKED                   1
#define DE_CONTROL_MONO_DATA_16_PACKED                  2
#define DE_CONTROL_MONO_DATA_32_PACKED                  3
#define DE_CONTROL_REPEAT_ROTATE                        11:11
#define DE_CONTROL_REPEAT_ROTATE_DISABLE                0
#define DE_CONTROL_REPEAT_ROTATE_ENABLE                 1
#define DE_CONTROL_TRANSPARENCY_MATCH                   10:10
#define DE_CONTROL_TRANSPARENCY_MATCH_OPAQUE            0
#define DE_CONTROL_TRANSPARENCY_MATCH_TRANSPARENT       1
#define DE_CONTROL_TRANSPARENCY_SELECT                  9:9
#define DE_CONTROL_TRANSPARENCY_SELECT_SOURCE           0
#define DE_CONTROL_TRANSPARENCY_SELECT_DESTINATION      1
#define DE_CONTROL_TRANSPARANCY                         8:8
#define DE_CONTROL_TRANSPARANCY_DISABLE                 0
#define DE_CONTROL_TRANSPARANCY_ENABLE                  1
#define DE_CONTROL_ROP                                  7:0

// Pseudo fields.

#define DE_CONTROL_SHORT_STROKE_DIR                     27:24
#define DE_CONTROL_SHORT_STROKE_DIR_225                 0
#define DE_CONTROL_SHORT_STROKE_DIR_135                 1
#define DE_CONTROL_SHORT_STROKE_DIR_315                 2
#define DE_CONTROL_SHORT_STROKE_DIR_45                  3
#define DE_CONTROL_SHORT_STROKE_DIR_270                 4
#define DE_CONTROL_SHORT_STROKE_DIR_90                  5
#define DE_CONTROL_SHORT_STROKE_DIR_180                 8
#define DE_CONTROL_SHORT_STROKE_DIR_0                   10
#define DE_CONTROL_ROTATION                             25:24
#define DE_CONTROL_ROTATION_0                           0
#define DE_CONTROL_ROTATION_270                         1
#define DE_CONTROL_ROTATION_90                          2
#define DE_CONTROL_ROTATION_180                         3

#define DE_PITCH                                        0x100010
#define DE_PITCH_DESTINATION                            28:16
#define DE_PITCH_SOURCE                                 12:0

#define DE_FOREGROUND                                   0x100014
#define DE_FOREGROUND_COLOR                             31:0

#define DE_BACKGROUND                                   0x100018
#define DE_BACKGROUND_COLOR                             31:0

#define DE_STRETCH_FORMAT                               0x10001C
#define DE_STRETCH_FORMAT_PATTERN_XY                    30:30
#define DE_STRETCH_FORMAT_PATTERN_XY_NORMAL             0
#define DE_STRETCH_FORMAT_PATTERN_XY_OVERWRITE          1
#define DE_STRETCH_FORMAT_PATTERN_Y                     29:27
#define DE_STRETCH_FORMAT_PATTERN_X                     25:23
#define DE_STRETCH_FORMAT_PIXEL_FORMAT                  21:20
#define DE_STRETCH_FORMAT_PIXEL_FORMAT_8                0
#define DE_STRETCH_FORMAT_PIXEL_FORMAT_16               1
#define DE_STRETCH_FORMAT_PIXEL_FORMAT_32               2
#define DE_STRETCH_FORMAT_ADDRESSING                    19:16
#define DE_STRETCH_FORMAT_ADDRESSING_XY                 0
#define DE_STRETCH_FORMAT_ADDRESSING_LINEAR             15
#define DE_STRETCH_FORMAT_SOURCE_HEIGHT                 11:0

#define DE_COLOR_COMPARE                                0x100020
#define DE_COLOR_COMPARE_COLOR                          23:0

#define DE_COLOR_COMPARE_MASK                           0x100024
#define DE_COLOR_COMPARE_MASK_MASKS                     23:0

#define DE_MASKS                                        0x100028
#define DE_MASKS_BYTE_MASK                              31:16
#define DE_MASKS_BIT_MASK                               15:0

#define DE_CLIP_TL                                      0x10002C
#define DE_CLIP_TL_TOP                                  31:16
#define DE_CLIP_TL_STATUS                               13:13
#define DE_CLIP_TL_STATUS_DISABLE                       0
#define DE_CLIP_TL_STATUS_ENABLE                        1
#define DE_CLIP_TL_INHIBIT                              12:12
#define DE_CLIP_TL_INHIBIT_OUTSIDE                      0
#define DE_CLIP_TL_INHIBIT_INSIDE                       1
#define DE_CLIP_TL_LEFT                                 11:0

#define DE_CLIP_BR                                      0x100030
#define DE_CLIP_BR_BOTTOM                               31:16
#define DE_CLIP_BR_RIGHT                                12:0

#define DE_MONO_PATTERN_LOW                             0x100034
#define DE_MONO_PATTERN_LOW_PATTERN                     31:0

#define DE_MONO_PATTERN_HIGH                            0x100038
#define DE_MONO_PATTERN_HIGH_PATTERN                    31:0

#define DE_WINDOW_WIDTH                                 0x10003C
#define DE_WINDOW_WIDTH_DESTINATION                     28:16
#define DE_WINDOW_WIDTH_SOURCE                          12:0

#define DE_WINDOW_SOURCE_BASE                           0x100040
#define DE_WINDOW_SOURCE_BASE_EXT                       27:27
#define DE_WINDOW_SOURCE_BASE_EXT_LOCAL                 0
#define DE_WINDOW_SOURCE_BASE_EXT_EXTERNAL              1
#define DE_WINDOW_SOURCE_BASE_CS                        26:26
#define DE_WINDOW_SOURCE_BASE_CS_0                      0
#define DE_WINDOW_SOURCE_BASE_CS_1                      1
#define DE_WINDOW_SOURCE_BASE_ADDRESS                   25:0

#define DE_WINDOW_DESTINATION_BASE                      0x100044
#define DE_WINDOW_DESTINATION_BASE_EXT                  27:27
#define DE_WINDOW_DESTINATION_BASE_EXT_LOCAL            0
#define DE_WINDOW_DESTINATION_BASE_EXT_EXTERNAL         1
#define DE_WINDOW_DESTINATION_BASE_CS                   26:26
#define DE_WINDOW_DESTINATION_BASE_CS_0                 0
#define DE_WINDOW_DESTINATION_BASE_CS_1                 1
#define DE_WINDOW_DESTINATION_BASE_ADDRESS              25:0

#define DE_ALPHA                                        0x100048
#define DE_ALPHA_VALUE                                  7:0

#define DE_WRAP                                         0x10004C
#define DE_WRAP_X                                       31:16
#define DE_WRAP_Y                                       15:0

#define DE_STATUS                                       0x100050
#define DE_STATUS_CSC                                   1:1
#define DE_STATUS_CSC_CLEAR                             0
#define DE_STATUS_CSC_NOT_ACTIVE                        0
#define DE_STATUS_CSC_ACTIVE                            1
#define DE_STATUS_2D                                    0:0
#define DE_STATUS_2D_CLEAR                              0
#define DE_STATUS_2D_NOT_ACTIVE                         0
#define DE_STATUS_2D_ACTIVE                             1

// Color Space Conversion registers.

#define CSC_Y_SOURCE_BASE                               0x1000C8
#define CSC_Y_SOURCE_BASE_EXT                           27:27
#define CSC_Y_SOURCE_BASE_EXT_LOCAL                     0
#define CSC_Y_SOURCE_BASE_EXT_EXTERNAL                  1
#define CSC_Y_SOURCE_BASE_CS                            26:26
#define CSC_Y_SOURCE_BASE_CS_0                          0
#define CSC_Y_SOURCE_BASE_CS_1                          1
#define CSC_Y_SOURCE_BASE_ADDRESS                       25:0

#define CSC_CONSTANTS                                   0x1000CC
#define CSC_CONSTANTS_Y                                 31:24
#define CSC_CONSTANTS_R                                 23:16
#define CSC_CONSTANTS_G                                 15:8
#define CSC_CONSTANTS_B                                 7:0

#define CSC_Y_SOURCE_X                                  0x1000D0
#define CSC_Y_SOURCE_X_INTEGER                          26:16
#define CSC_Y_SOURCE_X_FRACTION                         15:3

#define CSC_Y_SOURCE_Y                                  0x1000D4
#define CSC_Y_SOURCE_Y_INTEGER                          27:16
#define CSC_Y_SOURCE_Y_FRACTION                         15:3

#define CSC_U_SOURCE_BASE                               0x1000D8
#define CSC_U_SOURCE_BASE_EXT                           27:27
#define CSC_U_SOURCE_BASE_EXT_LOCAL                     0
#define CSC_U_SOURCE_BASE_EXT_EXTERNAL                  1
#define CSC_U_SOURCE_BASE_CS                            26:26
#define CSC_U_SOURCE_BASE_CS_0                          0
#define CSC_U_SOURCE_BASE_CS_1                          1
#define CSC_U_SOURCE_BASE_ADDRESS                       25:0

#define CSC_V_SOURCE_BASE                               0x1000DC
#define CSC_V_SOURCE_BASE_EXT                           27:27
#define CSC_V_SOURCE_BASE_EXT_LOCAL                     0
#define CSC_V_SOURCE_BASE_EXT_EXTERNAL                  1
#define CSC_V_SOURCE_BASE_CS                            26:26
#define CSC_V_SOURCE_BASE_CS_0                          0
#define CSC_V_SOURCE_BASE_CS_1                          1
#define CSC_V_SOURCE_BASE_ADDRESS                       25:0

#define CSC_SOURCE_DIMENSION                            0x1000E0
#define CSC_SOURCE_DIMENSION_X                          31:16
#define CSC_SOURCE_DIMENSION_Y                          15:0

#define CSC_SOURCE_PITCH                                0x1000E4
#define CSC_SOURCE_PITCH_Y                              31:16
#define CSC_SOURCE_PITCH_UV                             15:0

#define CSC_DESTINATION                                 0x1000E8
#define CSC_DESTINATION_WRAP                            31:31
#define CSC_DESTINATION_WRAP_DISABLE                    0
#define CSC_DESTINATION_WRAP_ENABLE                     1
#define CSC_DESTINATION_X                               27:16
#define CSC_DESTINATION_Y                               11:0

#define CSC_DESTINATION_DIMENSION                       0x1000EC
#define CSC_DESTINATION_DIMENSION_X                     31:16
#define CSC_DESTINATION_DIMENSION_Y                     15:0

#define CSC_DESTINATION_PITCH                           0x1000F0
#define CSC_DESTINATION_PITCH_X                         31:16
#define CSC_DESTINATION_PITCH_Y                         15:0

#define CSC_SCALE_FACTOR                                0x1000F4
#define CSC_SCALE_FACTOR_HORIZONTAL                     31:16
#define CSC_SCALE_FACTOR_VERTICAL                       15:0

#define CSC_DESTINATION_BASE                            0x1000F8
#define CSC_DESTINATION_BASE_EXT                        27:27
#define CSC_DESTINATION_BASE_EXT_LOCAL                  0
#define CSC_DESTINATION_BASE_EXT_EXTERNAL               1
#define CSC_DESTINATION_BASE_CS                         26:26
#define CSC_DESTINATION_BASE_CS_0                       0
#define CSC_DESTINATION_BASE_CS_1                       1
#define CSC_DESTINATION_BASE_ADDRESS                    25:0

#define CSC_CONTROL                                     0x1000FC
#define CSC_CONTROL_STATUS                              31:31
#define CSC_CONTROL_STATUS_STOP                         0
#define CSC_CONTROL_STATUS_START                        1
#define CSC_CONTROL_SOURCE_FORMAT                       30:28
#define CSC_CONTROL_SOURCE_FORMAT_YUV422                0
#define CSC_CONTROL_SOURCE_FORMAT_YUV420I               1
#define CSC_CONTROL_SOURCE_FORMAT_YUV420                2
#define CSC_CONTROL_SOURCE_FORMAT_YVU9                  3
#define CSC_CONTROL_SOURCE_FORMAT_IYU1                  4
#define CSC_CONTROL_SOURCE_FORMAT_IYU2                  5
#define CSC_CONTROL_SOURCE_FORMAT_RGB565                6
#define CSC_CONTROL_SOURCE_FORMAT_RGB8888               7
#define CSC_CONTROL_DESTINATION_FORMAT                  27:26
#define CSC_CONTROL_DESTINATION_FORMAT_RGB565           0
#define CSC_CONTROL_DESTINATION_FORMAT_RGB8888          1
#define CSC_CONTROL_HORIZONTAL_FILTER                   25:25
#define CSC_CONTROL_HORIZONTAL_FILTER_DISABLE           0
#define CSC_CONTROL_HORIZONTAL_FILTER_ENABLE            1
#define CSC_CONTROL_VERTICAL_FILTER                     24:24
#define CSC_CONTROL_VERTICAL_FILTER_DISABLE             0
#define CSC_CONTROL_VERTICAL_FILTER_ENABLE              1
#define CSC_CONTROL_BYTE_ORDER                          23:23
#define CSC_CONTROL_BYTE_ORDER_YUYV                     0
#define CSC_CONTROL_BYTE_ORDER_UYVY                     1

#define DE_DATAPORT                                     0x110000
#define DE_DATAPORT_END                                 0x150000


#define VGX_MMIO_ARENA					0x6be00000
#define POKE_16(address, value)         (*(volatile unsigned short int *)(VGX_MMIO_ARENA + (address))) = (value)
#define POKE_32(address, value)         (*(volatile unsigned long *)(VGX_MMIO_ARENA + (address))) = (value)
#define PEEK_32(address)                (*(volatile unsigned long *)(VGX_MMIO_ARENA + (address)))

#define PITCH(width, bpp)               (((width) * (bpp) / 8 + 15) & ~15)


int SM718_2D_Engine_Wait(void);


typedef enum _disp_control_t
{
    PRIMARY_CTRL = 0,
    SECONDARY_CTRL   = 1,
}
disp_control_t;

typedef struct _logicalMode_t
{
    unsigned long x;            /* X resolution */
    unsigned long y;            /* Y resolution */
    unsigned long bpp;          /* Bits per pixel */
    unsigned long hz;           /* Refresh rate */

    unsigned long baseAddress;  /* Offset from beginning of frame buffer.
                                   It is used to control the starting location of a mode.
                                   Calling function must initialize this field.
                                 */

    unsigned long pitch;        /* Mode pitch in byte.
                                   If initialized to 0, setMode function will set
                                   up this field.
                                   If not zero, setMode function will use this value.
                                 */

    disp_control_t dispCtrl;    /* SECONDARY or PRIMARY display control channel */
    
    /* These two parameters are used in the setModeEx. */
    unsigned long xLCD;         /* Panel width */
    unsigned long yLCD;         /* Panel height */
    
    void *userData;             /* Not used now, set it to 0 (for future used only) */
}
logicalMode_t;


#ifdef __cplusplus
}
#endif

#endif // TEECO_WIDOWS_GRAPHICS_PROFILE_H

