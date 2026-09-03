#ifndef TEECO_COLOR
#define TEECO_COLOR

#ifdef __cplusplus
 extern "C" {
#endif

#define RGB565CONVERT(red, green, blue) (u16) (((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3))
   

//----------- 16 비트 컬러를 일반적으로 사용하는 컬러를 정의 한다---------
#define 	clBlack 		0x0000
#define     clDarkRed       0x8000
#define     clDarkGreen     0x0400
#define     clDarkYellow    0x8400
#define     clDarkBlue      0x0010
#define     clDarkMagenta   0x1010
#define     clDarkCyan      0x0410
#define     clLightGray     0xb618

#define     clDarkGray      0x38e7
#define     clRed           0xf800
#define     clGreen         0x07e0
#define     clYellow        0xffe0
#define     clBlue          0x001f
#define     clMagenta       0xf81f
#define     clCyan          0x07ff
#define     clWhite         0xFFFF
#define 		clWinColor    RGB565CONVERT(230,  230,  230)
/*********************************************************************
* Overview: Some basic colors definitions.
*********************************************************************/
/*
#define BLACK               RGB565CONVERT(0,    0,      0)
#define BRIGHTBLUE          RGB565CONVERT(0,    0,      255)
#define BRIGHTGREEN         RGB565CONVERT(0,    255,    0)
#define BRIGHTCYAN          RGB565CONVERT(0,    255,    255)
#define BRIGHTRED           RGB565CONVERT(255,  0,      0)
#define BRIGHTMAGENTA       RGB565CONVERT(255,  0,      255)
#define BRIGHTYELLOW        RGB565CONVERT(255,  255,    0)
#define BLUE                RGB565CONVERT(0,    0,      128)
#define GREEN               RGB565CONVERT(0,    128,    0)
#define CYAN                RGB565CONVERT(0,    128,    128)
#define RED                 RGB565CONVERT(128,  0,      0)
#define MAGENTA             RGB565CONVERT(128,  0,      128)
#define BROWN               RGB565CONVERT(255,  128,    0)
#define LIGHTGRAY           RGB565CONVERT(128,  128,    128)
#define DARKGRAY            RGB565CONVERT(64,   64,     64)
#define LIGHTBLUE           RGB565CONVERT(128,  128,    255)
#define LIGHTGREEN          RGB565CONVERT(128,  255,    128)
#define LIGHTCYAN           RGB565CONVERT(128,  255,    255)
#define LIGHTRED            RGB565CONVERT(255,  128,    128)
#define LIGHTMAGENTA        RGB565CONVERT(255,  128,    255)
#define YELLOW              RGB565CONVERT(255,  255,    128)
#define WHITE               RGB565CONVERT(255,  255,    255)
*/
#define BLACK               RGB565CONVERT(0,    0,      0)
#define WHITE               RGB565CONVERT(255,  255,    255)
#define LIGHTBLUE          RGB565CONVERT(0,    0,      255)
#define LIGHTGREEN         RGB565CONVERT(0,    255,    0)
#define LIGHTCYAN          RGB565CONVERT(0,    255,    255)
#define LIGHTRED           RGB565CONVERT(255,  0,      0)
#define LIGHTMAGENTA       RGB565CONVERT(255,  0,      255)
#define LIGHTYELLOW        RGB565CONVERT(255,  255,    0)
#define BROWN               RGB565CONVERT(255,  128,    0)

#define YELLOW              RGB565CONVERT(255,  255,    128)
#define GREEN          RGB565CONVERT(128,  255,    128)
#define GRAY           RGB565CONVERT(128,  128,    128)
#define CYAN                RGB565CONVERT(0,    128,    128)

#define BLUE                RGB565CONVERT(0,    0,      128)
#define RED                 RGB565CONVERT(128,  0,      0)
//#define SYSTEMGRAY    RGB565CONVERT(230,  230,  230)
#define SYSTEMGRAY    RGB565CONVERT(179,  179,  179)


#define GRAY0       	    RGB565CONVERT(224,  224,    224)
#define GRAY1         	    RGB565CONVERT(192,  192,    192)   
#define GRAY2               RGB565CONVERT(160,  160,    160)   
#define GRAY3               RGB565CONVERT(128,  128,    128)
#define GRAY4               RGB565CONVERT(96,   96,     96)
#define GRAY5               RGB565CONVERT(64,   64,     64)
#define GRAY6	            RGB565CONVERT(32,   32,     32)
#define GRAY7	            RGB565CONVERT(0xcc, 0xcc, 0xcc)

//시스템용 폰트를 정의한다.
#define	SYSTEM_FONT_16	1
#define	SYSTEM_FONT_16B	2
#define	SYSTEM_FONT_20	3
#define	SYSTEM_FONT_20B	4
#define	SYSTEM_FONT_24B	5

#ifdef __cplusplus
}
#endif

#endif
