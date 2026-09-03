/**
  ******************************************************************************
  * @file       CTextout.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월일
  * @brief    이파일은  GUI에 클리어폰트를 출력하는 파일이다.(윈도우에서 사용한다)
                
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
#ifdef __cplusplus
 extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"
/* Private typedef -----------------------------------------------------------*/
/**
	@brief CFontInfo CTextout 함수가 참조해서 출력하는 구조체이다.
*/
typedef struct 
{
	int	FontScreen;		//!< 폰트디자인된 화면이 로딩된 스크린페이지
	int	Size_x, Size_y;		//!< 폰트의 사이즈
	int	Font_Y_Position;	//!< 바탕색별로 폰트위치를 계산한값
	int	Font_Distance;	//!< 문자와문자사이의 간격지정
	uint8_t     *Font_width_array;	//!< 폰트별로 문자의 간격계산을 위한 배열
} _CFontInfo;

/* Private macro -------------------------------------------------------------*/

#define     TEXT8_INTERVAL   1 //!< 문자와 문자사이의 공간을 지정한다.
#define     TEXT16_INTERVAL   1 //!< 문자와 문자사이의 공간을 지정한다.
#define     TEXT20_INTERVAL   1 //!< 문자와 문자사이의 공간을 지정한다.
#define     TEXT24_INTERVAL   1 //!< 문자와 문자사이의 공간을 지정한다.

/* Private variables ---------------------------------------------------------*/
_CFontInfo CFontInfo;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
const uint8_t FontSpace1616[95] = { 
	4, //SPACE
	3 , // !  
	5 , // "  
	9 , // #  
	8 , // $  
	13, // %  
	10, // &  
	3 , // `  
	5 , // (  
	5 , // )  
	7 , // *  
	9 , // +  
	3 , // ,  
	5 , // -  
	3 , // .  
	6 , // /  
	8 , // 0  
	4 , // 1  
	7 , // 2  
	7 , // 3  
	9 , // 4  
	7 , // 5  
	8 , // 6  
	7 , // 7  
	8 , // 8  
	8 , // 9  
	3 , // , //  
	3 , // ;  
	7 , // <  
	9 , // =  
	7 , // >  
	6 , // ?  
	13, // @	
	11, // A  
	7 , // B  
	9, // C  
	9 , // D  
	7 , // E  
	6 , // F  
	10, // G  
	9 , // H  
	3 , // I  
	4 , // J  
	8 , // K  
	6 , // L  
	13, // M  
	9 , // N  
	11, // O  
	8 , // P  
	11, // Q  
	8 , // R  
	7 , // S  
	9 , // T  
	9 , // U  
	10, // V  
	15, // W  
	9 , // X  
	10, // Y  
	8 , // Z  
	4 , // [  
	14, // \  
	3 , // ]  
	9 , // ^  
	9 , // _  
	4 , // `  	
	8 , // a  
	8 , // b  
	7 , // c  
	7 , // d  
	7 , // e  
	5 , // f  
	7 , // g  
	7 , // h  
	3 , // i  
	4 , // j  
	6 , // k  
	2 , // l  
	11, // m  
	7 , // n  
	9 , // o  
	8 , // p  
	7 , // q  
	5 , // r  
	6 , // s  
	5 , // t  
	7 , // u  
	7 , // v  
	13, // w  
	7 , // x  
	7 , // y  
	7 , // z  
	4 , // {  
	3 , // |  
	4 , // }  
	7  // ~  
};

const uint8_t FontSpace2020[95] = {
	5, //SPACE
	3 , //     ! 
	6 , //     " 
	11, //     # 
	9 , //     $ 
	17, //     % 
	14, //     & 
	4 , //     ` 
	6 , //     ( 
	6 , //     ) 
	9 , //     * 
	12, //     + 
	4 , //     , 
	6 , //     - 
	4 , //     . 
	8 , //     / 
	11, //     0 
	5 , //     1 
	10, //     2 
	10, //     3 
	12, //     4 
	9 , //     5 
	11, //     6 
	10, //     7 
	11, //     8 
	11, //     9 	
	4 , //     :
	4 , //     ; 
	9 , //     < 
	12, //     = 
	10, //     > 
	8 , //     ? 
	16, //     @ 	
	13, //     A 
	10, //     B 
	11, //     C 
	11, //     D 
	9 , //     E 
	9 , //     F 
	13, //     G 
	12, //     H 
	3 , //     I 
	7 , //     J 
	11, //     K 
	9 , //     L 
	16, //     M 
	12, //     N 
	14, //     O 
	10, //     P 
	14, //     Q 
	11, //     R 
	10, //     S 
	11, //     T 
	12, //     U 
	14, //     V 
	19, //     W 
	12, //     X 
	13, //     Y 
	11, //     Z 
	4 , //     [ 
	17, //     \ 
	5 , //     ] 
	11, //     ^ 
	12, //     _ 
	5 , //     ` 			  
	10, //     a 
	11, //     b 
	9 , //     c 
	10, //     d 
	10, //     e 
	7 , //     f 
	10, //     g 
	10, //     h 
	3 , //     i 
	5 , //     j 
	9 , //     k 
	3 , //     l 
	16, //     m 
	10, //     n 
	11, //     o 
	11, //     p 
	10, //     q 
	6 , //     r 
	8 , //     s 
	8 , //     t 
	10, //     u 
	10, //     v 
	16, //     w 
	9 , //     x 
	9 , //     y 
	9 , //     z 
	4 , //     { 
	3 , //     | 
	6 , //     } 
	11 //     ~ 	
};

const uint8_t FontSpace2424[95] = { 
    5,     // Space
    3,      // !
    6,     // "
    13,     // #    
    9,     // $
    19,     // %
    14,     // &
    3,     // '
    5,     // (
    6,     // )
    9,     // *
    13,     // +
    3,     // , (COMMA)
    7,     // - (minus)
    3,     // . (DOT)
    9,     // /
    11,     // 0
    5,     // 1
    10,     // 2
    10,     // 3
    13,     // 4
    10,     // 5
    11,     // 6
    11,     // 7
    11,     // 8
    11,     // 9
    3,     // :
    4,     // ;
    10,     // <
    13,     // =
    10,     // >
    9,     // ?
    17,     // @
    15,     // A
    11,     // B
    12,     // C
    12,     // D
    9,     // E
    9,     // F
    14,     // G
    13,     // H
    3,     // I
    6,     // J
    11,     // K
    9,     // L
    17,     // M
    13,     // N
    15,     // O
    10,     // P
    15,     // Q
    12,     // R
    10,     // S
    12,     // T
    13,     // U
    15,     // V
    22,     // W
    15,     // X
    14,     // Y
    11,     // Z
    5,     // [
    19,     // 
    5,     // ]
    11,     // ^
    13,     // _
    4,     // '
    10,     // a
    11,     // b
    9,     // c
    11,     // d
    11,     // e
    8,     // f
    11,     // g
    11,     // h
    5,     // i
    4,     // j
    10,     // k
    3,     // l
    17,     // m
    11,     // n
    13,     // o
    11,     // p
    11,     // q
    7,     // r
    9,     // s
    8,     // t
    11,     // u
    11,     // v
    19,     // w
    11,     // x
    10,     // y
    9,     // z
    6,     // {
    3,     // |
    6,     // }
    11     // ~  
};

uint8_t *GetFontArray(int font_xsize)
{
	uint8_t	*fontarray;
	if(font_xsize==10) fontarray = (uint8_t *)FontSpace2424;
	else if(font_xsize==16)  fontarray = (uint8_t *)FontSpace1616;
	else if(font_xsize==20)  fontarray = (uint8_t *)FontSpace2020;
	else if(font_xsize==24)  fontarray = (uint8_t *)FontSpace2424;
	return(fontarray);
}

int GetFontDistance(int font_xsize)
{
	int 	font_distance;
	if(font_xsize==10) font_distance = TEXT8_INTERVAL;
	else if(font_xsize==16)  font_distance = TEXT16_INTERVAL;
	else if(font_xsize==20)  font_distance =TEXT20_INTERVAL;
	else if(font_xsize==24)  font_distance = TEXT24_INTERVAL;
	return(font_distance);
}

//이함수는 ClearTextOut 을 사용하기전에 반드시 설정하고 사용해야한다.
int CFontSet(	int font_screen_page, 	/**< 폰트가 로딩된 페이지 */
					int font_xsize, 	/**< 폰트사이즈 */
					int font_ysize, 	/**< 폰트사이즈 */
					int start_y			/**< 폰트위치 */
			)
{
	CFontInfo.FontScreen = font_screen_page;
	CFontInfo.Size_x = font_xsize;
	CFontInfo.Size_y = font_ysize;
	CFontInfo.Font_Y_Position =  start_y;
	CFontInfo.Font_width_array = GetFontArray(font_xsize);
	CFontInfo.Font_Distance = GetFontDistance(font_xsize);	
	return(1);
}

/// 폰트의 종류는 10X16, 16X20, 20X20, 24X24 각각 Bold가 존재한다.
int ClearTextOut(int sx, int sy, const char *string)
{
    int     FontX_pos;//, FontY;
    int     font_x_size = CFontInfo.Size_x;
    int     font_y_size = CFontInfo.Size_y;
    int     next_pos;
    //int     offset;
    int     out_x_size = CFontInfo.Size_x;	//!< FontX Size


    next_pos = 0;
    while(*string)
    {
        out_x_size = CFontInfo.Font_width_array[*string - 0x20] + CFontInfo.Font_Distance; //문자의 넓이를 구한다.

		if(*string >= ' ' && *string < 'A')
		{
			FontX_pos = font_x_size * (*string - 0x20);
			//void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)			
			//page 의 sx, sy 부터 dimx, dimy만큼을 현재윈도우의 dx1, dy1으로 카피한다.
			PaintBitBlt(CFontInfo.FontScreen, FontX_pos, CFontInfo.Font_Y_Position, sx+next_pos, sy, out_x_size, font_y_size);
		}
		else if(*string >= 'A' && *string < 'a')
		{
			FontX_pos = font_x_size * (*string - 'A');
			PaintBitBlt(CFontInfo.FontScreen, FontX_pos, CFontInfo.Font_Y_Position+font_y_size, sx+next_pos, sy, out_x_size, font_y_size);
		}
		else // 'a' 부터 ~ 까지
		{
			FontX_pos = font_x_size * (*string - 'a');
			PaintBitBlt(CFontInfo.FontScreen, FontX_pos, CFontInfo.Font_Y_Position+(font_y_size*2), sx+next_pos, sy, out_x_size, font_y_size);
		}
        next_pos += out_x_size;
        string++;
    }   
	return (1);	
}

//클리어 폰트 사용할시 문자의 길이를 구한다.
int Get_CFontText_Width(int font_xsize, char *str)
{
    int     cnt=0;
    int     wid = 0;
	int	text_distance;	
	uint8_t	*font_arr = GetFontArray(font_xsize);
	
	/**< 폰트별로 자간간격이 다르다 */
	if(font_xsize==16) text_distance = TEXT16_INTERVAL;
	else if(font_xsize==20) text_distance = TEXT20_INTERVAL;
	else if(font_xsize==24) text_distance = TEXT24_INTERVAL;
	
    while(*str)
    {
        cnt = font_arr[*str-0x20]+text_distance;
        wid += cnt;
        str++;
    }
    return(wid);
}

/**
	@brief Gr->CTextOut 을 사용하기전에 폰트를 설정해야한다.
	@arg	 backcolor 바탕색은 4가지가 준비되어 있고
	@arg	 font  는 5종류가 준비되어 있다.
*/
int SystemFontSet(int backcolor, int font)
{
	switch(backcolor)
	{
		case BLACK:
			if(font==SYSTEM_FONT_16)	CFontSet(SYSTEM_FONT_SCREEN_1, 16, 20, 0);			
			else if(font==SYSTEM_FONT_16B)	CFontSet(SYSTEM_FONT_SCREEN_1, 16, 20, 60);			
			else if(font==SYSTEM_FONT_20)	CFontSet(SYSTEM_FONT_SCREEN_1, 20, 24, 120);			
			else if(font==SYSTEM_FONT_20B)	CFontSet(SYSTEM_FONT_SCREEN_1, 20, 24, 192);			
			else if(font==SYSTEM_FONT_24B)	CFontSet(SYSTEM_FONT_SCREEN_1, 24, 24, 264);	
			else	return 0;
			break;
		case WHITE:
			if(font==SYSTEM_FONT_16)	CFontSet(SYSTEM_FONT_SCREEN_1, 16, 20, (LCD_Y/2)+0);			
			else if(font==SYSTEM_FONT_16B)	CFontSet(SYSTEM_FONT_SCREEN_1, 16, 20, (LCD_Y/2)+60);			
			else if(font==SYSTEM_FONT_20)	CFontSet(SYSTEM_FONT_SCREEN_1, 20, 24, (LCD_Y/2)+120);			
			else if(font==SYSTEM_FONT_20B)	CFontSet(SYSTEM_FONT_SCREEN_1, 20, 24, (LCD_Y/2)+192);			
			else if(font==SYSTEM_FONT_24B)	CFontSet(SYSTEM_FONT_SCREEN_1, 24, 24, (LCD_Y/2)+264);						
			else	return 0;
			break;
		case BLUE:
			if(font==SYSTEM_FONT_16)	CFontSet(SYSTEM_FONT_SCREEN_2, 16, 20, 0);			
			else if(font==SYSTEM_FONT_16B)	CFontSet(SYSTEM_FONT_SCREEN_2, 16, 20, 60);			
			else if(font==SYSTEM_FONT_20)	CFontSet(SYSTEM_FONT_SCREEN_2, 20, 24, 120);			
			else if(font==SYSTEM_FONT_20B)	CFontSet(SYSTEM_FONT_SCREEN_2, 20, 24, 192);			
			else if(font==SYSTEM_FONT_24B)	CFontSet(SYSTEM_FONT_SCREEN_2, 24, 24, 264);						
			else	return 0;
			break;			
		case SYSTEMGRAY:
			if(font==SYSTEM_FONT_16)	CFontSet(SYSTEM_FONT_SCREEN_2, 16, 20, (LCD_Y/2)+0);			
			else if(font==SYSTEM_FONT_16B)	CFontSet(SYSTEM_FONT_SCREEN_2, 16, 20, (LCD_Y/2)+60);			
			else if(font==SYSTEM_FONT_20)	CFontSet(SYSTEM_FONT_SCREEN_2, 20, 24, (LCD_Y/2)+120);			
			else if(font==SYSTEM_FONT_20B)	CFontSet(SYSTEM_FONT_SCREEN_2, 20, 24, (LCD_Y/2)+192);			
			else if(font==SYSTEM_FONT_24B)	CFontSet(SYSTEM_FONT_SCREEN_2, 24, 24, (LCD_Y/2)+264);						
			else	return 0;
			break;			
		default: 	return 0;
	}		
	return(1);
}

#ifdef __cplusplus
}
#endif
