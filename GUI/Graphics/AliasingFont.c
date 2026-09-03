/**
  ******************************************************************************
  * @file       AliasingFont.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월24일
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
  
/**
  emWin용 폰트컨버터를 사용해서 폰트를 생성해야한다.
  폰트의사이즈는 상관이 없으며 앤티앨리어싱은 4비트를 사용해야한다.
  sif 파일로 생성해야하며 sif파일의 정보는 다음과같다.
  0 ~ 3: 	"GUIP"->구분자
  4~5:		width
  6~7:		height
  8~9:		Base Line
  10~11:	소문자높이
  12~13:	자본문자높이
  14~15: XX
  16~17:	시작코드 0x20
  18~19: 끝코드 0x7F
  20~21:	0x00A0
  22~23: 0x00FF
  24: 0x20부터의 정보 <0700(width), 0700(width), 0400(byte), 0000(not use), 18090000 (Start address)>
*/
/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"
#include "Memorymanager.h"
#include "WindowsGraphics.h"

/* Private typedef -----------------------------------------------------------*/

//!< 폰트가 로딩되는 어드레스는 여기서부터 6.8Mbyte (6815774Byte)까지 사용할수있다.
//!< 폰트는 메모리가 허락된다면 100개까지 사용할수 있다.
#define 		AFONT_START_ADDRESS		    0x6B780000
#define 		AFONT_END_ADDRESS		    	0x6BE00000


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern volatile 		U8	FlashPageBuf[2048];
extern const unsigned short int Unicode_Table[2350][2];
extern volatile int CRTX, CRTY;
/* Private function prototypes -----------------------------------------------*/
int 	IsBadBlock(int block);
void 	WinTextPlot(U16 x, U16 y);
/* Private functions ---------------------------------------------------------*/
#define RGB565(red, green, blue) (u16) ( (red  << 11) | (green << 5) | blue )

const int BlackColorConvert[][16] = {
 {	RGB565CONVERT(0x0,  0x0,  0x0 ), 
	RGB565CONVERT(230,  230,  230 ),
	RGB565CONVERT(210,  210,  210 ),
	RGB565CONVERT(190,  190,  190 ),
	RGB565CONVERT(170,  170,  170 ),
	RGB565CONVERT(150,  150,  150 ),
	RGB565CONVERT(130,  130,  130 ),
	RGB565CONVERT(110,  110,  110 ),
	RGB565CONVERT(90,   90,   90  ),
	RGB565CONVERT(70,   70,   70  ),
	RGB565CONVERT(50,   50,   50  ),
	RGB565CONVERT(30,   30,   30  ),
	RGB565CONVERT(30,   30,   30  ),
	RGB565CONVERT(30,   30,   30  ),
	RGB565CONVERT(30,   30,   30  ),
	RGB565CONVERT(0x05, 0x05, 0x05) }, 
 
 {	RGB565CONVERT(0x0,  0x0,  0x0 ), 
	RGB565CONVERT(230,  30,  230 ),
	RGB565CONVERT(210,  30,  210 ),
	RGB565CONVERT(190,  30,  190 ),
	RGB565CONVERT(170,  30,  170 ),
	RGB565CONVERT(150,  50,  150 ),
	RGB565CONVERT(130,  70,  130 ),
	RGB565CONVERT(110,  90,  110 ),
	RGB565CONVERT(90,   110,   90  ),
	RGB565CONVERT(70,   130,   70  ),
	RGB565CONVERT(50,   150,   50  ),
	RGB565CONVERT(30,   170,   30  ),
	RGB565CONVERT(30,   190,   30  ),
	RGB565CONVERT(30,   210,   30  ),
	RGB565CONVERT(30,   230,   30  ),
	RGB565CONVERT(0x05, 0x05, 0x05) }, 
};

const uint16_t WhiteColorConvert[][16] = {
 {	RGB565CONVERT(0x0,  0x0,  0x0), 
	RGB565CONVERT(30 , 30  , 30 ),
	RGB565CONVERT(30 , 30  , 30 ),
	RGB565CONVERT(30 , 30  , 30 ),
	RGB565CONVERT(30 , 30  , 30 ),
	RGB565CONVERT(50 , 50  , 50 ),
	RGB565CONVERT(70 , 70  , 70 ),
	RGB565CONVERT(90 , 90  , 90 ),
	RGB565CONVERT(110, 110 , 110),
	RGB565CONVERT(130, 130 , 130),
	RGB565CONVERT(150, 150 , 150),
	RGB565CONVERT(170, 170 , 170),
	RGB565CONVERT(190, 190 , 190),
	RGB565CONVERT(210, 210 , 210),
	RGB565CONVERT(230, 230 , 230),
	RGB565CONVERT(250, 250 , 250) },
};

const uint16_t RedColorConvert[][16] = {	
 {	RGB565CONVERT(0x0, 0x0 , 0x0),
	RGB565CONVERT(30, 0x0 , 0x0),
	RGB565CONVERT(30, 0x0, 0x0), 	
	RGB565CONVERT(30, 0x0 , 0x0),	
	RGB565CONVERT(30, 0x0 , 0x0),	
	RGB565CONVERT(50, 0x0 , 0x0), 	
	RGB565CONVERT(70, 0x0 , 0x0),	
	RGB565CONVERT(90, 0x0 , 0x0),	
	RGB565CONVERT(110, 0x0 , 0x0),	
	RGB565CONVERT(130, 0x0 , 0x0),	
	RGB565CONVERT(150, 0x0 , 0x0), 	
	RGB565CONVERT(170, 0x0 , 0x0),	
	RGB565CONVERT(190, 0x0 , 0x0),	
	RGB565CONVERT(210, 0x0 , 0x0),	
	RGB565CONVERT(230, 0x0 , 0x0),	
	RGB565CONVERT(250, 0x00 , 0x0)	},
 
{	RGB565CONVERT(0x0, 0x0 , 0x0),
	RGB565CONVERT(255, 0x00 , 0x00),
	RGB565CONVERT(210, 0x00, 0x00), 	
	RGB565CONVERT(190, 0x00 , 0x00),	
	RGB565CONVERT(170, 0x00 , 0x00),	
	RGB565CONVERT(150, 0x00 , 0x00), 	
	RGB565CONVERT(130, 0x00 , 0x00),	
	RGB565CONVERT(110, 0x00 , 0x00),	
	RGB565CONVERT(90, 0x00 , 0x00),	
	RGB565CONVERT(70, 0x00 , 0x00),	
	RGB565CONVERT(50, 0x00 , 0x00), 	
	RGB565CONVERT(30, 0x00 , 0x00),	
	RGB565CONVERT(30, 0x00 , 0x00),	
	RGB565CONVERT(30, 0x00 , 0x00),	
	RGB565CONVERT(30, 0x00 , 0x00),	
	RGB565CONVERT(30, 0x00 , 0x0)	},	
};

const uint16_t BlueColorConvert[][16] = {	
 {	RGB565CONVERT(0x0, 0x0 , 0),
	RGB565CONVERT(0x0, 0x0 , 30),
	RGB565CONVERT(0x0, 0x0 , 30), 	
	RGB565CONVERT(0x0, 0x0 , 30),	
	RGB565CONVERT(0x0, 0x0 , 30),	
	RGB565CONVERT(0x0, 0x0 , 50), 	
	RGB565CONVERT(0x0, 0x0 , 70),	
	RGB565CONVERT(0x0, 0x0 , 90),	
	RGB565CONVERT(0x0, 0x0 , 110),	
	RGB565CONVERT(0x0, 0x0 , 130),	
	RGB565CONVERT(0x0, 0x0 , 150), 	
	RGB565CONVERT(0x0, 0x0 , 170),	
	RGB565CONVERT(0x0, 0x0 , 190),	
	RGB565CONVERT(0x0, 0x0 , 210),	
	RGB565CONVERT(0x0, 0x0 , 230),	
	RGB565CONVERT(0x00, 0x00 , 255) },
 
{	RGB565CONVERT(0x0, 0x0 , 0),
	RGB565CONVERT(0x0, 0x0 , 255),
	RGB565CONVERT(0x0, 0x0 , 210), 	
	RGB565CONVERT(0x0, 0x0 , 190),	
	RGB565CONVERT(0x0, 0x0 , 170),	
	RGB565CONVERT(0x0, 0x0 , 150), 	
	RGB565CONVERT(0x0, 0x0 , 130),	
	RGB565CONVERT(0x0, 0x0 , 110),	
	RGB565CONVERT(0x0, 0x0 , 90),	
	RGB565CONVERT(0x0, 0x0 , 70),	
	RGB565CONVERT(0x0, 0x0 , 50), 	
	RGB565CONVERT(0x0, 0x0 , 30),	
	RGB565CONVERT(0x0, 0x0 , 30),	
	RGB565CONVERT(0x0, 0x0 , 30),	
	RGB565CONVERT(0x0, 0x0 , 30),	
	RGB565CONVERT(0x00, 0x00 , 30) },
 };

const uint16_t MagentaColorConvert[][16] = {	
 {	RGB565CONVERT(0x0,0x0 ,  0x0),
	RGB565CONVERT(30,  0x0 , 30 ),
	RGB565CONVERT(30,  0x0 , 30 ), 	
	RGB565CONVERT(30,  0x0 , 30 ),	
	RGB565CONVERT(30,  0x0 , 30 ),	
	RGB565CONVERT(50,  0x0 , 50 ), 	
	RGB565CONVERT(70,  0x0 , 70 ),	
	RGB565CONVERT(90,  0x0 , 90 ),	
	RGB565CONVERT(110, 0x0 , 110),	
	RGB565CONVERT(130, 0x0 , 130),	
	RGB565CONVERT(150, 0x0 , 150), 	
	RGB565CONVERT(170, 0x0 , 170),	
	RGB565CONVERT(190, 0x0 , 190),	
	RGB565CONVERT(210, 0x0 , 210),	
	RGB565CONVERT(230, 0x0 , 230),	
	RGB565CONVERT(250, 0x0 , 250) }, 

 {	RGB565CONVERT(0x0,0x0 ,  0x0),
	RGB565CONVERT(230,  0x0 , 230 ),
	RGB565CONVERT(210,  0x0 , 210 ), 	
	RGB565CONVERT(190,  0x0 , 190 ),	
	RGB565CONVERT(170,  0x0 , 170 ),	
	RGB565CONVERT(150,  0x0 , 150 ), 	
	RGB565CONVERT(130,  0x0 , 130 ),	
	RGB565CONVERT(110,  0x0 , 110 ),	
	RGB565CONVERT(90, 0x0 , 90),	
	RGB565CONVERT(70, 0x0 , 70),	
	RGB565CONVERT(50, 0x0 , 50), 	
	RGB565CONVERT(30, 0x0 , 30),	
	RGB565CONVERT(30, 0x0 , 30),	
	RGB565CONVERT(30, 0x0 , 30),	
	RGB565CONVERT(30, 0x0 , 30),	
	RGB565CONVERT(30, 0x0 , 30) }, 
};


const uint16_t GreenColorConvert[][16] = {	
 {	RGB565CONVERT(0x0, 0x0 , 0x0),
	RGB565CONVERT(0x0, 30 , 0x0),
	RGB565CONVERT(0x0, 30 , 0x0), 	
	RGB565CONVERT(0x0, 30 , 0x0),	
	RGB565CONVERT(0x0, 30 , 0x0),	
	RGB565CONVERT(0x0, 50 , 0x0), 	
	RGB565CONVERT(0x0, 70 , 0x0),	
	RGB565CONVERT(0x0, 90 , 0x0),	
	RGB565CONVERT(0x0, 110 , 0x0),	
	RGB565CONVERT(0x0, 130 , 0x0),	
	RGB565CONVERT(0x0, 150 , 0x0), 	
	RGB565CONVERT(0x0, 170 , 0x0),	
	RGB565CONVERT(0x0, 190 , 0x0),	
	RGB565CONVERT(0x0, 210 , 0x0),	
	RGB565CONVERT(0x0, 230 , 0x0),	
	RGB565CONVERT(0x0, 250 , 0x0)	},

{	RGB565CONVERT(0x0, 0x0 , 0x0),
	RGB565CONVERT(0x0, 255 , 0x0),
	RGB565CONVERT(0x0, 210 , 0x0), 	
	RGB565CONVERT(0x0, 190 , 0x0),	
	RGB565CONVERT(0x0, 170 , 0x0),	
	RGB565CONVERT(0x0, 150 , 0x0), 	
	RGB565CONVERT(0x0, 130 , 0x0),	
	RGB565CONVERT(0x0, 110 , 0x0),	
	RGB565CONVERT(0x0, 90 , 0x0),	
	RGB565CONVERT(0x0, 70 , 0x0),	
	RGB565CONVERT(0x0, 50 , 0x0), 	
	RGB565CONVERT(0x0, 40 , 0x0),	
	RGB565CONVERT(0x0, 40 , 0x0),	
	RGB565CONVERT(0x0, 30 , 0x0),	
	RGB565CONVERT(0x0, 20 , 0x0),	
	RGB565CONVERT(0x0, 10 , 0x0)	},
 
};

const uint16_t CyanColorConvert[][16] = {	
 {	RGB565CONVERT(0x0, 0x0, 0x0),
	RGB565CONVERT(0x0, 30  ,30 ),
	RGB565CONVERT(0x0, 30  ,30 ), 	
	RGB565CONVERT(0x0, 30  ,30 ),	
	RGB565CONVERT(0x0, 30  ,30 ),	
	RGB565CONVERT(0x0, 50  ,50 ), 	
	RGB565CONVERT(0x0, 70  ,70 ),	
	RGB565CONVERT(0x0, 90  ,90 ),	
	RGB565CONVERT(0x0, 110 ,110),	
	RGB565CONVERT(0x0, 130 ,130),	
	RGB565CONVERT(0x0, 150 ,150), 	
	RGB565CONVERT(0x0, 170 ,170),	
	RGB565CONVERT(0x0, 190 ,190),	
	RGB565CONVERT(0x0, 210 ,210),	
	RGB565CONVERT(0x0, 230 ,230),	
	RGB565CONVERT(0x0, 250 ,250) },	
 
{	RGB565CONVERT(0x0, 0x0, 0x0),
	RGB565CONVERT(0x0, 255  ,255 ),
	RGB565CONVERT(0x0, 210  ,210 ), 	
	RGB565CONVERT(0x0, 190  ,190 ),	
	RGB565CONVERT(0x0, 170  ,170 ),	
	RGB565CONVERT(0x0, 150  ,150 ), 	
	RGB565CONVERT(0x0, 130  ,130 ),	
	RGB565CONVERT(0x0, 110  ,110 ),	
	RGB565CONVERT(0x0, 90 ,90),	
	RGB565CONVERT(0x0, 70 ,70),	
	RGB565CONVERT(0x0, 50 ,50), 	
	RGB565CONVERT(0x0, 30 ,30),	
	RGB565CONVERT(0x0, 30 ,30),	
	RGB565CONVERT(0x0, 30 ,30),	
	RGB565CONVERT(0x0, 30 ,30),	
	RGB565CONVERT(0x0, 30 ,30) },		
};


const uint16_t YellowColorConvert[][16] = {	
 {	RGB565CONVERT(0x0, 0x0, 0x0),
	RGB565CONVERT(30,  30  , 0x0),
	RGB565CONVERT(30,  30  , 0x0), 	
	RGB565CONVERT(30,  30  , 0x0),	
	RGB565CONVERT(30,  30  , 0x0),	
	RGB565CONVERT(50,  50  , 0x0), 	
	RGB565CONVERT(70,  70  , 0x0),	
	RGB565CONVERT(90,  90  , 0x0),	
	RGB565CONVERT(110, 110 , 0x0),	
	RGB565CONVERT(130, 130 , 0x0),	
	RGB565CONVERT(150, 150 , 0x0), 	
	RGB565CONVERT(170, 170 , 0x0),	
	RGB565CONVERT(190, 190 , 0x0),	
	RGB565CONVERT(210, 210 , 0x0),	
	RGB565CONVERT(230, 230 , 0x0),	
	RGB565CONVERT(250, 250 , 0x0) },	
 
{	RGB565CONVERT(0x0, 0x0, 0x0),
	RGB565CONVERT(230,  230  , 0x0),
	RGB565CONVERT(210,  210  , 0x0), 	
	RGB565CONVERT(190,  190  , 0x0),	
	RGB565CONVERT(170,  170  , 0x0),	
	RGB565CONVERT(150,  150  , 0x0), 	
	RGB565CONVERT(130,  130  , 0x0),	
	RGB565CONVERT(110,  110  , 0x0),	
	RGB565CONVERT(90, 90 , 0x0),	
	RGB565CONVERT(70, 70 , 0x0),	
	RGB565CONVERT(50, 50 , 0x0), 	
	RGB565CONVERT(30, 30 , 0x0),	
	RGB565CONVERT(30, 30 , 0x0),	
	RGB565CONVERT(30, 30 , 0x0),	
	RGB565CONVERT(30, 30 , 0x0),	
	RGB565CONVERT(30, 30 , 0x0) },	
 };

void TextDraw(int sx, int sy, const unsigned char *Font, int width, int height, int byteofwidth, uint16_t fontcolor, uint16_t back_color) {
	int		    colume = 0;
	int		    row=0;
	int		    arr_cnt=0;
	uint16_t	color=0xffff;
	uint8_t	    data;
	for(colume=0; colume<height; colume++) {
		int 	x_offset=0;
		for(row=0; row < byteofwidth; row++)
		{
			data = Font[arr_cnt++];
			if(fontcolor==clWhite)	
			{
				color = WhiteColorConvert[back_color][(data >> 4) & 0x0f];
				//Dprintf("TextDraw::WHITE Color\n");
			}
			else if(fontcolor==clBlack)	color = BlackColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clRed)	color = RedColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clBlue)	color = BlueColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clGreen)	color = GreenColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clYellow)	color = YellowColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clCyan)	color = CyanColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clMagenta)	color = MagentaColorConvert[back_color][(data >> 4) & 0x0f];
			SetFontColor(color);
			if(color != 0) WinTextPlot(sx+x_offset, sy+colume);
			x_offset++;
			
			if(fontcolor==clWhite)	color = WhiteColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clBlack)	color = BlackColorConvert[back_color][data  & 0x0f];
			else if(fontcolor==clRed)	color = RedColorConvert[back_color][data  & 0x0f];
			else if(fontcolor==clBlue)	color = BlueColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clGreen)	color = GreenColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clYellow)	color = YellowColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clCyan)	color = CyanColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clMagenta)	color = MagentaColorConvert[back_color][data & 0x0f];
			SetFontColor(color);
			if(color != 0) WinTextPlot(sx+x_offset, sy+colume);
			x_offset++;
		}
	}
}

void TextDrawUD(int sx, int sy, const unsigned char *Font, int width, int height, int byteofwidth, uint16_t fontcolor, uint16_t back_color)
{
	int		colume = 0;
	int		row=0;
	int		arr_cnt=0;
	uint16_t	color=0xffff;
	uint8_t	data;
	
	for(colume=height; colume>0; colume--)	//전체라인도트를 그린다.
	{
		int 	x_offset=0;
		for(row=0; row < byteofwidth; row++) // 한라인을 그린다.
		{
			data = Font[arr_cnt++];
			if(fontcolor==clWhite)	color = WhiteColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clBlack)	color = BlackColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clRed)	color = RedColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clBlue)	color = BlueColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clGreen)	color = GreenColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clYellow)	color = YellowColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clCyan)	color = CyanColorConvert[back_color][(data >> 4) & 0x0f];
			else if(fontcolor==clMagenta)	color = MagentaColorConvert[back_color][(data >> 4) & 0x0f];
			SetFontColor(color);
			if(color != 0) WinTextPlot(sx+colume, sy+x_offset);//WinTextPlot(sx+x_offset, sy+colume);
			x_offset++;
			
			if(fontcolor==clWhite)	color = WhiteColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clBlack)	color = BlackColorConvert[back_color][data  & 0x0f];
			else if(fontcolor==clRed)	color = RedColorConvert[back_color][data  & 0x0f];
			else if(fontcolor==clBlue)	color = BlueColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clGreen)	color = GreenColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clYellow)	color = YellowColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clCyan)	color = CyanColorConvert[back_color][data & 0x0f];
			else if(fontcolor==clMagenta)	color = MagentaColorConvert[back_color][data & 0x0f];
			SetFontColor(color);
			if(color != 0) WinTextPlot(sx+colume, sy+x_offset);//WinTextPlot(sx+x_offset, sy+colume);
			x_offset++;
		}
	}
}


static int	FontCount=0;
typedef struct _Font
{
	uint32_t	start_addr;
	char		name[32];
	uint32_t	file_size;
	uint32_t	font;
	char		FontKind[4];
} _FontInfo;

_FontInfo		FontInfo[100] = { 0 };

//!< 시작은 AFONT_START_ADDRESS 부터 시작한다.
uint32_t	FontMalloc(void)
{
	static int offset=0;
	if(FontCount==0)
	{
		offset=0;
		//Dprintf("FontMalloc:;malloc first addr=%08x\n", AFONT_START_ADDRESS);
		return(AFONT_START_ADDRESS);
	}
	else
	{		
		offset += FontInfo[FontCount-1].file_size;
		//Dprintf("FontMalloc::malloc addr=%08x\n", AFONT_START_ADDRESS+offset);
		if( (AFONT_START_ADDRESS+offset) >= AFONT_END_ADDRESS)
		{
			Dprintf("FontMalloc:: memory over error\n");
			return(0);
		}
		return(AFONT_START_ADDRESS + offset);
	}
}

int LoadAliasingFont(char *fontname)
{
	//int 		size;
	st_NandFAT NandFat;
	int		start_block;
	int		end_block;
	int		page, y, i;
	U8		*VRAM_FileBuf;
	int		cnt=0;
	uint32_t addr=0;
	
	addr = FontMalloc();
	if(addr != 0)
	{
		VRAM_FileBuf = (U8 *)addr;
	}
	else
	{
		Dprintf("LoadAliasingFont:: meory allocation error\n");
		return(0);
	}
	
	if(FontCount >=100)
	{
		Dprintf("LoadAliasingFont:: 폰트는 100개까지만 사용할수있다\n");
		return(0);
	}
    //Dprintf("LoadAliasingFont::fontname=%s Addr=%08x\n", 	fontname, VRAM_FileBuf);
	
	if(SearchNandFile(fontname, &NandFat))
	{
		start_block = NandFat.start_blk;
		end_block = NandFat.end_blk;
		strcpy(FontInfo[FontCount].FontKind, NandFat.FontKind);
		//Dprintf("LoadAliasingFont::Font Load %s\n", NandFat.FontKind);
		//!< 구조체에 폰트정보를 기입한다.
		FontInfo[FontCount].start_addr = addr;
		//Dprintf("LoadAliasingFont::LoadFont Addr=%08x\n", addr);
		strcpy(FontInfo[FontCount].name, fontname);	//폰트의 종류를 저장한다.
		for(i=start_block; i<=end_block; i++)
		{
			for(page=0; page <64; page++)
			{
				BSP_NandReadpage(start_block, page, (U8 *)FlashPageBuf);
				for(y=0; y<2048; y++)
				{	
					*VRAM_FileBuf++ = FlashPageBuf[y]; 
				}
				cnt += 2048;
				if(cnt > NandFat.Size)  //실제 폰트사이즈 용량을 로드하면 탈출한다. 
                {
					//Dprintf("LoadAliasingFont::Load Size = %d\n\n", cnt);
					FontInfo[FontCount].file_size = cnt;
					FontCount++;
                    return(cnt);
                }
			}
			start_block++;
			//int errorOutCnt=0;
			while(1)
			{
				if(!IsBadBlock(start_block)) 
				{
					break;
				}
				else start_block++;
			}
		}
		return(cnt);
	}
	//Dprintf("FONT READ OK\n");
	return(0);
}

uint32_t	gFont_start_addr=0;
int		gFont_kind=0;

int SetFontAliasing(char *fontname)
{
	int start_addr = 0;
	//Dprintf("SetFontAliasing:: %s \n", fontname);
	for(int i=0; i < FontCount; i++)	//등록된폰트의 정보를 검색한다.
	{
		if(!strcmp(fontname, FontInfo[i].name))
		{
			//Dprintf("SetFontAliasing::name=%s\n", FontInfo[i].name);
			//Dprintf("SetFontAliasing::Addr=%08x\n", FontInfo[i].start_addr);
			//Dprintf("SetFontAliasing::Font=%s\n", FontInfo[i].FontKind);
			gFont_start_addr = FontInfo[i].start_addr;	
			start_addr = 1;
			if(!strcmp(FontInfo[i].FontKind, "NA")) gFont_kind = ENGLISH;
			else if(!strcmp(FontInfo[i].FontKind, "NU")) gFont_kind = HANGUL;
			else if(!strcmp(FontInfo[i].FontKind, "NE")) gFont_kind = ENG_HANGUL;
			return(1);
		}
	}	
	if(start_addr==0)	//검색결과가 없을시는
	{
		Dprintf("SetFontAliasing:: %s Font not found\n", fontname);
		return(0);
	}
	return(1);	
}

int TextOutAliasing(int sx, int sy, char *str, uint16_t color, uint16_t back_color, char *font)
{
	U32	addr; 
	U8	*FontAddress;
	U16	width, height, byte; 	
	U16	high, low;
	U16   ks5601_code;
	U16   unicode;
	int	position=0;

    //color = flash.systemFontColor;
    
	if(gFont_start_addr==0)
	{
		Dprintf("TextOutAliasing:: %s Font not found\n", font);
		return(0);
	}	
	CRTX = sx, CRTY = sy;	// 그래픽 포인터를 이동한다.	
	
	while(*str)
	{
    	if(*str >= 0x20 && *str <=0x7f)//영문일경우에 실행된다.
		{			
			if(gFont_kind==ENGLISH)
			{
				addr = *(U32 *)(gFont_start_addr + 20 + ((*str - ' ') * 12) + 8);  //!< 20 는 옵세번지, 12는 테일블정보크기, 8은 간접어드레스 옵셋
				FontAddress = (U8 *)(addr+gFont_start_addr); //간접어드레싱
				height = *(U16 *)(gFont_start_addr+6);
				width = *(U16 *)(gFont_start_addr + 20 + ((*str - ' ') * 12) + 2);
				byte = *(U16 *)(gFont_start_addr + 20 + ((*str - ' ') * 12) + 4);
			}
			else if(gFont_kind==ENG_HANGUL)
			{
				addr = *(U32 *)(gFont_start_addr + 24 + ((*str - ' ') * 12) + 8);  //!< 20 는 옵세번지, 12는 테일블정보크기, 8은 간접어드레스 옵셋
				FontAddress = (U8 *)(addr+gFont_start_addr); //간접어드레싱
				height = *(U16 *)(gFont_start_addr+6);
				width = *(U16 *)(gFont_start_addr + 24 + ((*str - ' ') * 12) + 2);
				byte = *(U16 *)(gFont_start_addr + 24 + ((*str - ' ') * 12) + 4);
			}			
			//Dprintf("E+H Code[E]-> height=%d width=%d byte=%d\n", height, width, byte);
			TextDraw(sx , sy,  FontAddress, width, height, byte, color, back_color);
			sx+=width;	//일반적인 사용
			//sy+=width;		//위에서 아래로 글씨를 쓸때
			str++;
		}
		else
		{			
			high = *str++;
			low = *str++;
            ks5601_code = (high << 8) | low;

            for(int i=0; i<2350; i++)
            {
                if(Unicode_Table[i][0] == ks5601_code)
                {
                    unicode = Unicode_Table[i][1];
                    break;
                }
            }
			position = unicode - 0xAC00;
			//Dprintf("position = %d\n", position);
			if(gFont_kind==HANGUL)
			{
				addr = *(U32 *)(gFont_start_addr + 20 + (position * 12) + 8);  
				FontAddress = (U8 *)(addr+gFont_start_addr); //간접어드레싱
				height = *(U16 *)(gFont_start_addr+6);
				width = *(U16 *)(gFont_start_addr + 20  + (position * 12) + 2);
				byte = *(U16 *)(gFont_start_addr + 20  + (position * 12) + 4);				
				//Dprintf("H Code-> height=%d width=%d byte=%d\n", height, width, byte);
			}
			else if(gFont_kind==ENG_HANGUL)
			{
				addr = *(U32 *)(gFont_start_addr + (24 + (0x7E- 0x20+1) * 12)+ (position * 12) + 8);  
				FontAddress = (U8 *)(addr+gFont_start_addr); //간접어드레싱

				height = *(U16 *)(gFont_start_addr+6);
				width = *(U16 *)(gFont_start_addr + (24 + (0x7E- 0x20+1) * 12) + (position * 12) + 2);
				byte = *(U16 *)(gFont_start_addr + (24 + (0x7E- 0x20+1) * 12) + (position * 12) + 4);
				//Dprintf("E+H Code[H]-> height=%d width=%d byte=%d\n", height, width, byte);
			}			
			TextDraw(sx , sy,  FontAddress, width, height, byte, color, back_color);
			sx+=width;	//일반적인 사용
			//sy+=width;		//위에서 아래로 글씨를 쓸때
		}		
	}	
	return(1);
}


int TextLenAliasing(char *str, char *font)
{
	U16	width;
	U16	high, low;
	U16   ks5601_code;
	U16   unicode;
	int	position=0;
	int	text_len=0;

	if(gFont_start_addr==0)
	{
		Dprintf("TextLenAliasing:: %s Font not found\n", font);
		return(0);
	}	
	
	
	while(*str)
	{
		//
    	if(*str >= 0x20 && *str <=0x7f)//영문일경우에 실행된다.
		{			
			if(gFont_kind==ENGLISH)
			{
				width = *(U16 *)(gFont_start_addr + 20 + ((*str - ' ') * 12) + 2);
			}
			else if(gFont_kind==ENG_HANGUL)
			{
				width = *(U16 *)(gFont_start_addr + 24 + ((*str - ' ') * 12) + 2);
			}			
			text_len+=width;
			str++;
		}
		else
		{			
			high = *str++;
			low = *str++;
            ks5601_code = (high << 8) | low;

            for(int i=0; i<2350; i++)
            {
                if(Unicode_Table[i][0] == ks5601_code)
                {
                    unicode = Unicode_Table[i][1];
                    break;
                }
            }
			position = unicode - 0xAC00;
			if(gFont_kind==HANGUL)
			{
				width = *(U16 *)(gFont_start_addr + 20  + (position * 12) + 2);
			}
			else if(gFont_kind==ENG_HANGUL)
			{
				width = *(U16 *)(gFont_start_addr + (24 + (0x7E- 0x20+1) * 12) + (position * 12) + 2);
			}			
			text_len+=width;
		}		
	}	
	return(text_len);
}


void MyTestFunc(void)
{
	static int s=0;
	//uint32_t	size=0;
	//SetColor(RGB565CONVERT(230,  230,  230));
	SetColor(clWhite);
	FillRect2D(0, 0, LCD_X, LCD_Y);

	if(s==0)
	{
		LoadAliasingFont("ng_eh_24.sif");	//처음은 0을 지고 진입한다.
		LoadAliasingFont("ng32.sif");	//처음은 0을 지고 진입한다.
		Dprintf(">Name=%s Addr=%08x Size=%d\n", FontInfo[0].name, FontInfo[0].start_addr, FontInfo[0].file_size);
		Dprintf(">Name=%s Addr=%08x Size=%d\n", FontInfo[1].name, FontInfo[1].start_addr, FontInfo[1].file_size);
		s=1;
	}	
	SetFontAliasing("ng_eh_24.sif");
	TextOutAliasing(100, 70, "대한민국은 민주공화국이다12345ABCDE~!@#ajqijw", clBlack, 0, "ng_eh_24.sif");		
	TextOutAliasing(100, 70+24*1, "대한민국은 민주공화국이다12345ABCDE~!@#ajqijw", clBlue, 1, "ng_eh_24.sif");		
	TextOutAliasing(100, 70+24*2, "대한민국은 민주공화국이다12345ABCDE~!@#ajqijw", clRed, 1, "ng_eh_24.sif");		
	TextOutAliasing(100, 70+24*3, "대한민국은 민주공화국이다12345ABCDE~!@#ajqijw", clGreen, 1, "ng_eh_24.sif");		
	TextOutAliasing(100, 70+24*4, "대한민국은 민주공화국이다12345ABCDE~!@#ajqijw", clCyan, 1, "ng_eh_24.sif");		
	TextOutAliasing(100, 70+24*5, "대한민국은 민주공화국이다12345ABCDE~!@#ajqijw", clYellow, 1, "ng_eh_24.sif");		
	TextOutAliasing(100, 70+24*6, "대한민국은 민주공화국이다12345ABCDE~!@#ajqijw", clMagenta, 1, "ng_eh_24.sif");		
}

#ifdef __cplusplus
}
#endif
