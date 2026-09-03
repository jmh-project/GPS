/**
  ******************************************************************************
  * @file       IconOut.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월6일
  * @brief    이파일은  아이콘을 로드하고 표시하는 파일이다.
                
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
#include "IconOut.h"

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//!< 48 Icon 과 32 아이콘은 1페이지를 2개로 나누어서 사용한다.
/// +---------------------------+
/// |                                  |
/// |         Gray                   |
/// |                                  |
/// |                                  |
/// | -------------------------   |
/// |                                  |
/// |         White                 |
/// |                                  |
/// |                                  |
/// +---------------------------+

void Icon64White_Load(void);
void Icon48White_Load(void);
void Icon32White_Load(void)	;
void Icon64Gray_Load(void);
void Icon48Gray_Load(void);
void Icon32Gray_Load(void);

/**
	@brief	윈도우를 시작하기전에 필요한 64X64 White 배경아이콘을 로드한다.
*/

void Icon64White_Load(void)
{
	int	i, sx=0, sy=0;
	
	for(i=0; i<161; i++)
	{
		BmpLoad(SYSTEM_ICON64_WHITE_SCREEN, sx, sy,  (char *)Icon64_White[i]); 
		sx+=64;
		if(sx > LCD_X-64) //!< 한줄이 꽉찼으면 다음칸으로 내려간다.
		{
			sx=0;
			sy+=64;
			if(sy > LCD_Y - 64) //!< 화면에 꽉찼으면 다음화면으로
			{
				Dprintf("%s:: Icon ScreenOut Exit\n", __FUNCTION__);
				break;
			}				
		}
	}
}

/**
	@brief	윈도우를 시작하기전에 필요한 48X48 White 배경아이콘을 로드한다.
*/
void Icon48White_Load(void)
{
	int	i, sx=0, sy=384;
	
	for(i=0; i<161; i++)
	{
		BmpLoad(SYSTEM_ICON48_SCREEN, sx, sy,  (char *)Icon48_White[i]); 
		sx+=48;
		if(sx > LCD_X-48) 
		{
			sx=0;
			sy+=48;
			if(sy > LCD_Y - 48)  //!< 화면에 꽉찼으면 다음화면으로
			{
				Dprintf("%s:: Icon ScreenOut Exit\n", __FUNCTION__);
				break;
			}				
		}
	}
}

/**
	@brief	윈도우를 시작하기전에 필요한 32X32 White 배경아이콘을 로드한다.
*/
void Icon32White_Load(void)
{
	int	i, sx=0, sy=384;
	
	for(i=0; i<161; i++)
	{
		BmpLoad(SYSTEM_ICON32_SCREEN, sx, sy,  (char *)Icon32_White[i]); 
		sx+=32;
		if(sx > LCD_X-32) 
		{
			sx=0;
			sy+=32;
			if(sy > LCD_Y - 32) //!< 화면에 꽉찼으면 다음화면으로
			{
				Dprintf("%s:: Icon ScreenOut Exit\n", __FUNCTION__);
				break;
			}				
		}
	}
}

/**
	@brief	윈도우를 시작하기전에 필요한 64X64 White 배경아이콘을 로드한다.
*/

void Icon64Gray_Load(void)
{
	int	i, sx=0, sy=0;
	
	for(i=0; i<161; i++)
	{
		BmpLoad(SYSTEM_ICON64_GRAY_SCREEN, sx, sy,  (char *)Icon64_Gray[i]); 
		sx+=64;
		if(sx > LCD_X-64) 
		{
			sx=0;
			sy+=64;
			if(sy > LCD_Y - 64)  //!< 화면에 꽉찼으면 다음화면으로
			{
				Dprintf("%s:: Icon ScreenOut Exit\n", __FUNCTION__);
				break;
			}				
		}
	}
}

/**
	@brief	윈도우를 시작하기전에 필요한 48X48 White 배경아이콘을 로드한다.
*/
void Icon48Gray_Load(void)
{
	int	i, sx=0, sy=0;
	
	for(i=0; i<161; i++)
	{
		BmpLoad(SYSTEM_ICON48_SCREEN, sx, sy,  (char *)Icon48_Gray[i]); 
		sx+=48;
		if(sx > LCD_X-48) 
		{
			sx=0;
			sy+=48;
			if(sy > LCD_Y - 48)  //!< 화면에 꽉찼으면 다음화면으로
			{
				Dprintf("%s:: Icon ScreenOut Exit\n", __FUNCTION__);
				break;
			}				
		}
	}
}

/**
	@brief	윈도우를 시작하기전에 필요한 32X32 White 배경아이콘을 로드한다.
*/
void Icon32Gray_Load(void)
{
	int	i, sx=0, sy=0;
	
	for(i=0; i<161; i++)
	{
		BmpLoad(SYSTEM_ICON32_SCREEN, sx, sy,  (char *)Icon32_Gray[i]); 
		sx+=32;
		if(sx > LCD_X-32) 
		{
			sx=0;
			sy+=32;
			if(sy > LCD_Y - 32)  //!< 화면에 꽉찼으면 다음화면으로
			{
				Dprintf("%s:: Icon ScreenOut Exit\n", __FUNCTION__);
				break;
			}				
		}
	}
}

/**
	@brief	현재윈도우에 64X64 (Backcolor:WHITE) ICON을 표시한다.
*/

int IconOut64W(int sx, int sy, char *icon_name)
{
	int	i, xs, ys;
	
	for(i=0; i<ICON_MAX_CNT; i++)
	{
		if(!strcmp(icon_name, Icon64_White[i]))
		{
			ys = (i / (LCD_X/64)) * 64;
			xs = (i % (LCD_X/64)) * 64;
			//void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)			
			//page 의 sx, sy 부터 dimx, dimy만큼을 현재윈도우의 dx1, dy1으로 카피한다.				
			//PaintBitBltRop(SYSTEM_ICON64_WHITE_SCREEN, xs, ys, sx, sy, 64, 64, ROP2, ROP2_AND); //ROP2_OR, ROP2_AND
			PaintBitBlt(SYSTEM_ICON64_WHITE_SCREEN, xs, ys, sx, sy, 64, 64);
			return(1);
		}
	}
	Dprintf("IconOut64G:: Error = %s\n", icon_name);
	return(0);	//Error
}


/**
	@brief	현재윈도우에 64X64 (Backcolor:GRAY) ICON을 표시한다.
*/

int IconOut64G(int sx, int sy, char *icon_name)
{
	int	i, xs, ys;
	
	for(i=0; i<ICON_MAX_CNT; i++)
	{
		if(!strcmp(icon_name, Icon64_Gray[i]))
		{
			ys = (i / (LCD_X/64)) * 64;
			xs = (i % (LCD_X/64)) * 64;
			//void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)			
			//page 의 sx, sy 부터 dimx, dimy만큼을 현재윈도우의 dx1, dy1으로 카피한다.				
			//PaintBitBlt(SYSTEM_ICON64_GRAY_SCREEN, xs, ys, sx, sy, 64, 64);
			PaintBitBlt(SYSTEM_ICON64_GRAY_SCREEN, xs, ys, sx, sy, 64, 64);
			//PaintBitBltRop(SYSTEM_ICON64_GRAY_SCREEN, xs, ys, sx, sy, 64, 64, ROP2, ROP2_AND);
			return(1);
		}
	}
	Dprintf("IconOut64G:: Error = %s\n", icon_name);
	return(0);	//Error
}

/**
	@brief	현재윈도우에 48 (Backcolor:WHITE) ICON을 표시한다.
*/
int IconOut48W(int sx, int sy, char *icon_name)
{
	int	i, xs, ys;
	
	for(i=0; i<ICON_MAX_CNT; i++)
	{
		if(!strcmp(icon_name, Icon48_White[i]))
		{
			ys = ((i / (LCD_X/48)) * 48) + (LCD_Y / 2 );
			xs = (i % (LCD_X/48)) * 48;
			//void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)			
			//page 의 sx, sy 부터 dimx, dimy만큼을 현재윈도우의 dx1, dy1으로 카피한다.				
			PaintBitBlt(SYSTEM_ICON48_SCREEN, xs, ys, sx, sy, 48, 48);
			//PaintBitBltRop(SYSTEM_ICON48_SCREEN, xs, ys, sx, sy, 48, 48, ROP2, ROP2_AND);
			return(1);
		}
	}
	Dprintf("IconOut48W:: Error = %s\n", icon_name);
	return(0);	//Error
}

/**
	@brief	현재윈도우에 48X48 (Backcolor:GRAY) ICON을 표시한다.
*/
int IconOut48G(int sx, int sy, char *icon_name)
{
	int	i, xs, ys;
	
	for(i=0; i<ICON_MAX_CNT; i++)
	{
		if(!strcmp(icon_name, Icon48_Gray[i]))
		{
			ys = ((i / (LCD_X/48)) * 48);
			xs = (i % (LCD_X/48)) * 48;
			//Dprintf("IconOut48G::xs=%d ys=%d\n", xs, ys);
			//void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)			
			//page 의 sx, sy 부터 dimx, dimy만큼을 현재윈도우의 dx1, dy1으로 카피한다.				
			PaintBitBlt(SYSTEM_ICON48_SCREEN, xs, ys, sx, sy, 48, 48);
			//PaintBitBltRop(SYSTEM_ICON48_SCREEN, xs, ys, sx, sy, 48, 48, ROP2, ROP2_AND);
			return(1);
		}
	}
	Dprintf("IconOut48G:: Error = %s\n", icon_name);
	return(0);	//Error
}

/**
	@brief	현재윈도우에 32 (Backcolor:WHITE) ICON을 표시한다.
*/
int IconOut32W(int sx, int sy, char *icon_name)
{
	int	i, xs, ys;
	
	for(i=0; i<ICON_MAX_CNT; i++)
	{
		if(!strcmp(icon_name, Icon32_White[i]))
		{
			ys = ((i / (LCD_X/32)) * 32) + (LCD_Y / 2 );
			xs = (i % (LCD_X/32)) * 32;
			//void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)			
			//page 의 sx, sy 부터 dimx, dimy만큼을 현재윈도우의 dx1, dy1으로 카피한다.				
			PaintBitBlt(SYSTEM_ICON32_SCREEN, xs, ys, sx, sy, 32, 32);
			//PaintBitBltRop(SYSTEM_ICON32_SCREEN, xs, ys, sx, sy, 32, 32, ROP2, ROP2_AND);
			return(1);
		}
	}
	Dprintf("IconOut32W:: Error = %s\n", icon_name);
	return(0);	//Error
}

/**
	@brief	현재윈도우에 32X32 (Backcolor:GRAY) ICON을 표시한다.
*/
int IconOut32G(int sx, int sy, char *icon_name)
{
	int	i, xs, ys;
	
	for(i=0; i<ICON_MAX_CNT; i++)
	{
		if(!strcmp(icon_name, Icon32_Gray[i]))
		{
			ys = ((i / (LCD_X/32)) * 32);
			xs = (i % (LCD_X/32)) * 32;
			//Dprintf("IconOut32G::xs=%d ys=%d\n", xs, ys);
			//void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)			
			//page 의 sx, sy 부터 dimx, dimy만큼을 현재윈도우의 dx1, dy1으로 카피한다.				
			PaintBitBlt(SYSTEM_ICON32_SCREEN, xs, ys, sx, sy, 32, 32);
			//PaintBitBltRop(SYSTEM_ICON32_SCREEN, xs, ys, sx, sy, 32, 32, ROP2, ROP2_AND);
			return(1);
		}
	}
	Dprintf("IconOut32G:: Error = %s\n", icon_name);
	return(0);	//Error
}

#ifdef __cplusplus
}
#endif
