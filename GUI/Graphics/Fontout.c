#include "Teeco_System.h" 
#include "font_Arial12.h" 
#include "Font_TimeRoma16.h" 
#include "TimeRoma16B.h" 
#include "Arial18b.h" 
#include "memorymanager.h" 
#ifdef __cplusplus
 extern "C" {
#endif

volatile int CRTX, CRTY;

volatile int 	gFontLeft, gFontRight, gFontGap, gSpaceFontGap, gFontSize;
volatile int	g_FontKind;
volatile u16	g_TextColor;
volatile int    g_TextStartLine;
volatile int	g_FontNo;
volatile int	g_HangulFontNo;
volatile int	gHangulFontSize;

volatile U8		FontBuf[2048];
extern volatile U32	Graphics_Address;
extern volatile U32	position;

volatile int gFontSize_X[40];
volatile int gFontSize_Y[40];


int IsBadBlock(int block);

//VRAM 사용시
#ifndef PSRAM_USE
#define ENG_FONT16_START_ADDRESS		    0x6B5F0000 	 
#define ENG_FONT24_START_ADDRESS		    0x6B600000 	 

#define ENG_FONTALL_START_ADDRESS_1		0x6B610000 	 
#define ENG_FONTALL_START_ADDRESS_2		0x6B620000 	 
#define ENG_FONTALL_START_ADDRESS_3		0x6B630000 	 
#define ENG_FONTALL_START_ADDRESS_4		0x6B640000 	 
#define ENG_FONTALL_START_ADDRESS_5		0x6B650000 	 
#define ENG_FONTALL_START_ADDRESS_6		0x6B660000 	 
#define ENG_FONTALL_START_ADDRESS_7		0x6B670000 	 
#define ENG_FONTALL_START_ADDRESS_8		0x6B680000 	 

#define HANGUL_FONT16_START_ADDRESS_1	0x6B690000 	 
#define HANGUL_FONT16_START_ADDRESS_2	0x6B6F0000 	 
#define HANGUL_FONT24_START_ADDRESS	0x6B8E4000 	 
#endif

//PSRAM 사용시
#ifdef PSRAM_USE
#define ENG_FONT16_START_ADDRESS		    0x645FEA00 	 
#define ENG_FONT24_START_ADDRESS		    0x6460EA00 	 

#define ENG_FONTALL_START_ADDRESS_1		0x6461EA00 	 
#define ENG_FONTALL_START_ADDRESS_2		0x6462EA00 	 
#define ENG_FONTALL_START_ADDRESS_3		0x6463EA00 	 
#define ENG_FONTALL_START_ADDRESS_4		0x6464EA00 	 
#define ENG_FONTALL_START_ADDRESS_5		0x6465EA00 	 
#define ENG_FONTALL_START_ADDRESS_6		0x6466EA00 	 
#define ENG_FONTALL_START_ADDRESS_7		0x6467EA00 	 
#define ENG_FONTALL_START_ADDRESS_8		0x6468EA00 	 

#define HANGUL_FONT16_START_ADDRESS_1	0x6469EA00 	 
#define HANGUL_FONT16_START_ADDRESS_2	0x646FEA00 	 
#define HANGUL_FONT24_START_ADDRESS	0x646FEA00 	
#endif
extern const unsigned short int Unicode_Table[2350][2];

void WinTextPlot(U16 x, U16 y)
{
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	volatile U16 	*pScreen = (U16 *)Graphics_Address;
	int		sx = x + gp_Graphics->Offset_X;
	int		sy = y + gp_Graphics->Offset_Y;

    position= ((sy * LCD_X)+ sx);
    pScreen += position;
    *pScreen = g_TextColor;
}

void SetFontColor(U16 color)
{
	g_TextColor = color;	
}

void SetFontStartLine(int start)
{
	g_TextStartLine = start;	
}

void SetFont(int no)
{
    //영문폰트일경우
    if(no >= 0 && no <=36)
    {
        if(gFontSize_X[no] != 0)
        {
            g_FontNo = no; 
            gFontSize = gFontSize_X[no];
        }
        else
        {
            Dprintf("SetFont:: Font Data Not Load = %d\n", no);
        }
    }
    else
    {
        Dprintf("SetFont:: no Error=%d\n", no);
    }
}

    //한글폰트일경우
void SetHangulFont(int no)
{
    if(no >= 37 && no <=39)
    {
        if(gFontSize_X[no] != 0)    //로드를 한경우라면 이값은 사이즈가 들어있다.
        {
            g_HangulFontNo = no; 
            gHangulFontSize = gFontSize_X[no];            
        }
        else
        {
            Dprintf("SetHangulFont:: Hangul Font Data Not Load = %d\n", no);
        }
    }
    else
    {
        Dprintf("SetHangulFont:: no Error=%d\n", no);
    }
}

void WinFont16X16Putc(void)
{
    int     i;
    int     x, y, space;
	U8		*s = (U8 *)FontBuf;
    int     skip;
    
    skip = g_TextStartLine * 2;
    
    s += skip;
    
    
    x=CRTX; y=CRTY;
    for(i=g_TextStartLine; i<16; i++)
	{
		switch(gFontLeft)
		{
			case 1:	if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
			case 2:	if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
			case 3:	if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
			case 4:	if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
			case 5:	if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
			case 6:	if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
			case 7:	if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
			case 8:	if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
		}
        s++;
		switch(gFontRight)
		{
			case 9:		
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				break;
			case 10:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				break;
			case 11:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				break;
			case 12:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				break;
			case 13:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				break;
			case 14:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				break;
			case 15:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
				break;
			case 16:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
				break;
		}
        s++;
		
        y++;
        x = CRTX;
    }
	space = (gFontRight-gFontLeft)+gFontGap+1;
	CRTX += space;
} 

void WinFont24X24Putc(void)
{
    int     i;
    int     x, y, space;
	U8		*s = (U8 *)FontBuf;
    int     skip;

    x=CRTX; y=CRTY;
    
    skip = g_TextStartLine * 3;
    
    s += skip;
    
    for(i=g_TextStartLine; i<24; i++) //24라인을 출력하기위해서
	{
		switch(gFontLeft)
		{
			case 1:	if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
			case 2:	if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
			case 3:	if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
			case 4:	if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
			case 5:	if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
			case 6:	if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
			case 7:	if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
			case 8:	if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        }
        s++;
    
        if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
        if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
        if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
        if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
        if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
        if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
        if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
        if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;

		switch(gFontRight)
		{
			case 17:		
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				break;
			case 18:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				break;
			case 19:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				break;
			case 20:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				break;
			case 21:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				break;
			case 22:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				break;
			case 23:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
				break;
			case 24:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
				break;
		}
        s++;		
        y++;
        x = CRTX;
    }
	space = (gFontRight-gFontLeft)+gFontGap+1;
	CRTX += space;
} 


void WinFont32X32Putc(void)
{
    int     i;
    int     x, y, space;
	U8		*s = (U8 *)FontBuf;

    int     skip;
    
    skip = g_TextStartLine * 4;
    
    s += skip;    
    x=CRTX; y=CRTY;
    
    for(i=g_TextStartLine; i<32; i++)
	{
		switch(gFontLeft)
		{
			case 1:	if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
			case 2:	if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
			case 3:	if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
			case 4:	if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
			case 5:	if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
			case 6:	if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
			case 7:	if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
			case 8:	if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
		}
        s++;

		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;
		
		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;

		
		switch(gFontRight)
		{
			case 25:		
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				break;
			case 26:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				break;
			case 27:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				break;
			case 28:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				break;
			case 29:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				break;
			case 30:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				break;
			case 31:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
				break;
			case 32:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
				break;
		}
        s++;
		
        y++;
        x = CRTX;
    }
	space = (gFontRight-gFontLeft)+gFontGap+1;
	CRTX += space;
} 

void WinFont64X64Putc(void)
{
    int     i;
    int     x, y, space;
	U8		*s = (U8 *)FontBuf;
    int     skip;
    
    skip = g_TextStartLine * 8;
    
    s += skip;   
    x=CRTX; y=CRTY;
    
    for(i=g_TextStartLine; i<64; i++)
	{
		switch(gFontLeft)
		{
			case 9:		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
			case 10:	if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
			case 11:	if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
			case 12:	if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
			case 13:	if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
			case 14:	if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
			case 15:	if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
			case 16:	if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
		}
        s++;

		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;
		
		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;
		
		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;
		
		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;
		
		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;
		
		if(*s & 0x80) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
		if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
        s++;

		
		switch(gFontRight)
		{
			case 57:		
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				break;
			case 58:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				break;
			case 59:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				break;
			case 60:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				break;
			case 61:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				break;
			case 62:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				break;
			case 63:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
				break;
			case 64:	
				if(*s & 0x80) WinTextPlot(x++,y);  else  x++; 
				if(*s & 0x40) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x20) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x10) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x08) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x04) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x02) WinTextPlot(x++,y);  else  x++;
				if(*s & 0x01) WinTextPlot(x++,y);  else  x++;
				break;
		}
        s++;
		
        y++;
        x = CRTX;
    }
	space = (gFontRight-gFontLeft)+gFontGap+1;
	CRTX += space;
} 

void SetFontGap(int gap)
{
	gFontGap = gap;
}

void SetFontSpaceGap(int gap)
{
	gSpaceFontGap = gap;
}

void FontAlign16(void)
{
	int		i;
	int		left=9;
	int		right=0;	
	U8		*Font = (U8 *)FontBuf;
	
	for(i=0; i<32; i+=2) 
	{
		if(Font[i] & 0x01) left=8; 
		if(Font[i] & 0x02) left=7; 
		if(Font[i] & 0x04) left=6; 
		if(Font[i] & 0x08) left=5; 
		if(Font[i] & 0x10) left=4; 
		if(Font[i] & 0x20) left=3; 
		if(Font[i] & 0x40) left=2; 
		if(Font[i] & 0x80) left=1; 

		if(i==0) gFontLeft = 9;
		if(left < gFontLeft) gFontLeft = left;
	}	

	for(i=1; i<32; i+=2) 
	{
		if(Font[i] & 0x80) right=9; 
		if(Font[i] & 0x40) right=10;
		if(Font[i] & 0x20) right=11;
		if(Font[i] & 0x10) right=12;
		if(Font[i] & 0x08) right=13;
		if(Font[i] & 0x04) right=14;
		if(Font[i] & 0x02) right=15;
		if(Font[i] & 0x01) right=16;

		if(i==1) gFontRight = 9;
		if(right > gFontRight) gFontRight = right;
	}
}

//24X24 Font의 좌측의 시작점과 우측의 끝점을 조사한다.
void FontAlign24(void)
{
	int		i;
	int		left=9;
	int		right=0;	
	U8		*Font = (U8 *)FontBuf;
    
	
	for(i=0; i<72; i+=3) 
	{
		if(Font[i] & 0x01) left=8; 
		if(Font[i] & 0x02) left=7; 
		if(Font[i] & 0x04) left=6; 
		if(Font[i] & 0x08) left=5; 
		if(Font[i] & 0x10) left=4; 
		if(Font[i] & 0x20) left=3; 
		if(Font[i] & 0x40) left=2; 
		if(Font[i] & 0x80) left=1; 

        //24 라인을 모두조사해서 제일 작은것을 취한다.
		if(i==0) gFontLeft = 9;     //첫플래그
		if(left < gFontLeft) gFontLeft = left;
	}	
    /*
    if(gFontLeft==9) //첫바이트가 0x00 일경우
    {
        for(i=0; i<72; i+=3) 
        {
            if(Font[i] & 0x01) left=16; 
            if(Font[i] & 0x02) left=15; 
            if(Font[i] & 0x04) left=14; 
            if(Font[i] & 0x08) left=13; 
            if(Font[i] & 0x10) left=12; 
            if(Font[i] & 0x20) left=11; 
            if(Font[i] & 0x40) left=10; 
            if(Font[i] & 0x80) left=9; 

            //24 라인을 모두조사해서 제일 작은것을 취한다.
            if(i==0) gFontLeft = 16;     //첫플래그
            if(left < gFontLeft) gFontLeft = left;
        }	
    }    
    */
    /*
    //2번째 바이트부터 우측의 빈공간을 찾는다.
	for(i=2; i<72; i+=3) 
	{
		if(Font[i] & 0x80) right=9; 
		if(Font[i] & 0x40) right=10;
		if(Font[i] & 0x20) right=11;
		if(Font[i] & 0x10) right=12;
		if(Font[i] & 0x08) right=13;
		if(Font[i] & 0x04) right=14;
		if(Font[i] & 0x02) right=15;
		if(Font[i] & 0x01) right=16;

		if(i==2) gFontRight = 17;
		if(right > gFontRight) gFontRight = right;
	}
    */
    
	for(i=2; i<72; i+=3) 
	{
		if(Font[i] & 0x80) right=17; 
		if(Font[i] & 0x40) right=18;
		if(Font[i] & 0x20) right=19;
		if(Font[i] & 0x10) right=20;
		if(Font[i] & 0x08) right=21;
		if(Font[i] & 0x04) right=22;
		if(Font[i] & 0x02) right=23;
		if(Font[i] & 0x01) right=24;

		if(i==2) gFontRight = 17;
		if(right > gFontRight) gFontRight = right;
	}
}

void FontAlign32(void)
{
	int		i;
	int		left=9;
	int		right=0;	
	U8		*Font = (U8 *)FontBuf;
	
	for(i=0; i<128; i+=4) 
	{
		if(Font[i] & 0x01) left=8; 
		if(Font[i] & 0x02) left=7; 
		if(Font[i] & 0x04) left=6; 
		if(Font[i] & 0x08) left=5; 
		if(Font[i] & 0x10) left=4; 
		if(Font[i] & 0x20) left=3; 
		if(Font[i] & 0x40) left=2; 
		if(Font[i] & 0x80) left=1; 

		if(i==0) gFontLeft = 9;
		if(left < gFontLeft) gFontLeft = left;
	}	

	for(i=3; i<128; i+=4) 
	{
		if(Font[i] & 0x80) right=25; 
		if(Font[i] & 0x40) right=26;
		if(Font[i] & 0x20) right=27;
		if(Font[i] & 0x10) right=28;
		if(Font[i] & 0x08) right=29;
		if(Font[i] & 0x04) right=30;
		if(Font[i] & 0x02) right=31;
		if(Font[i] & 0x01) right=32;

		if(i==3) gFontRight = 25;
		if(right > gFontRight) gFontRight = right;
	}
}


void FontAlign64(void)
{
	int		i;
	int		left=9;
	int		right=0;	
	U8		*Font = (U8 *)FontBuf;
	
	for(i=1; i<512; i+=8) 
	{
		if(Font[i] & 0x01) left=16; 
		if(Font[i] & 0x02) left=15; 
		if(Font[i] & 0x04) left=14; 
		if(Font[i] & 0x08) left=13; 
		if(Font[i] & 0x10) left=12; 
		if(Font[i] & 0x20) left=11; 
		if(Font[i] & 0x40) left=10; 
		if(Font[i] & 0x80) left=9; 

		if(i==1) gFontLeft = 17;
		if(left < gFontLeft) gFontLeft = left;
	}	

	for(i=7; i<512; i+=8) 
	{
		if(Font[i] & 0x80) right=57; 
		if(Font[i] & 0x40) right=58;
		if(Font[i] & 0x20) right=59;
		if(Font[i] & 0x10) right=60;
		if(Font[i] & 0x08) right=61;
		if(Font[i] & 0x04) right=62;
		if(Font[i] & 0x02) right=63;
		if(Font[i] & 0x01) right=64;

		if(i==7) gFontRight = 57;
		if(right > gFontRight) gFontRight = right;
	}
}

void HangulPutc16(void)
{    
	int		x, y, i;
	U8		*s = (U8 *)FontBuf;

    x=CRTX; y=CRTY;

    for(i=0; i< 16; i++)
	{
        if(*s & 0x80) WinTextPlot(x++,y); else x++;
        if(*s & 0x40) WinTextPlot(x++,y); else x++;
        if(*s & 0x20) WinTextPlot(x++,y); else x++;
        if(*s & 0x10) WinTextPlot(x++,y); else x++;
        if(*s & 0x08) WinTextPlot(x++,y); else x++;
        if(*s & 0x04) WinTextPlot(x++,y); else x++;
        if(*s & 0x02) WinTextPlot(x++,y); else x++;
        if(*s & 0x01) WinTextPlot(x++,y); else x++;
        s++;

        if(*s & 0x80) WinTextPlot(x++,y); else x++;
        if(*s & 0x40) WinTextPlot(x++,y); else x++;
        if(*s & 0x20) WinTextPlot(x++,y); else x++;
        if(*s & 0x10) WinTextPlot(x++,y); else x++;
        if(*s & 0x08) WinTextPlot(x++,y); else x++;
        if(*s & 0x04) WinTextPlot(x++,y); else x++;
        if(*s & 0x02) WinTextPlot(x++,y); else x++;
        if(*s & 0x01) WinTextPlot(x++,y); else x++;
        s++;
		
        y++;
        x = CRTX;		
    }
   	CRTX += 16;
}

void HangulPutc24(void)
{    
	int		x, y, i;
	U8		*s = (U8 *)FontBuf;

    x=CRTX; y=CRTY;

    for(i=0; i< 24; i++)
	{
        if(*s & 0x80) WinTextPlot(x++,y); else x++;
        if(*s & 0x40) WinTextPlot(x++,y); else x++;
        if(*s & 0x20) WinTextPlot(x++,y); else x++;
        if(*s & 0x10) WinTextPlot(x++,y); else x++;
        if(*s & 0x08) WinTextPlot(x++,y); else x++;
        if(*s & 0x04) WinTextPlot(x++,y); else x++;
        if(*s & 0x02) WinTextPlot(x++,y); else x++;
        if(*s & 0x01) WinTextPlot(x++,y); else x++;
        s++;

        if(*s & 0x80) WinTextPlot(x++,y); else x++;
        if(*s & 0x40) WinTextPlot(x++,y); else x++;
        if(*s & 0x20) WinTextPlot(x++,y); else x++;
        if(*s & 0x10) WinTextPlot(x++,y); else x++;
        if(*s & 0x08) WinTextPlot(x++,y); else x++;
        if(*s & 0x04) WinTextPlot(x++,y); else x++;
        if(*s & 0x02) WinTextPlot(x++,y); else x++;
        if(*s & 0x01) WinTextPlot(x++,y); else x++;
        s++;

        if(*s & 0x80) WinTextPlot(x++,y); else x++;
        if(*s & 0x40) WinTextPlot(x++,y); else x++;
        if(*s & 0x20) WinTextPlot(x++,y); else x++;
        if(*s & 0x10) WinTextPlot(x++,y); else x++;
        if(*s & 0x08) WinTextPlot(x++,y); else x++;
        if(*s & 0x04) WinTextPlot(x++,y); else x++;
        if(*s & 0x02) WinTextPlot(x++,y); else x++;
        if(*s & 0x01) WinTextPlot(x++,y); else x++;
        s++;
		
        y++;
        x = CRTX;		
    }
   	CRTX += 24;
}


//윈도우그래픽에서 TextOut함수로 사용된다.
int DrawText(int sx, int sy, const char *text)
{
	//U8		buf[32];
	int 	position;	
	//WORD	count;
	int		i;
	U8		*VRAM_FileBuf = NULL;
	U32		font_addr=0;
	
	gFontLeft = 0;
	gFontRight = 0;
	
	CRTX = sx, CRTY = sy;	// 그래픽 포인터를 이동한다.	

    if(gFontSize_X[g_FontNo] == 0)
    {
        Dprintf("DrawText::Font Data Not Load\n");
        return(0);
    }
	switch(g_FontNo)
	{

        //0~19까지는 16,16폰트만 전용으로 로드하는 공간이다.
		case 0:  VRAM_FileBuf = (U8 *)ENG_FONT16_START_ADDRESS;   font_addr = ENG_FONT16_START_ADDRESS;	break;
		case 1:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 2:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 3:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 4:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 5:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 6:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 7:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 8:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 9:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 10: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 11: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 12: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 13: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 14: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 15: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 16: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 17: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 18: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
		case 19: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo)); font_addr=(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));  break;
        
        //20~28까지는 24,24폰트만 전용으로 로드하는 공간이다.
		case 20: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
		case 21: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
		case 22: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
		case 23: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
		case 24: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
		case 25: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
		case 26: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
		case 27: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
		case 28: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); font_addr=(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20))); break;
        
		case 29: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_1;	font_addr = ENG_FONTALL_START_ADDRESS_1;	break;
		case 30: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_2;	font_addr = ENG_FONTALL_START_ADDRESS_2;	break;
		case 31: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_3;	font_addr = ENG_FONTALL_START_ADDRESS_3;	break;
		case 32: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_4;	font_addr = ENG_FONTALL_START_ADDRESS_4;	break;
		case 33: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_5;	font_addr = ENG_FONTALL_START_ADDRESS_5;	break;
		case 34: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_6;	font_addr = ENG_FONTALL_START_ADDRESS_6;	break;
		case 35: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_7;	font_addr = ENG_FONTALL_START_ADDRESS_7;	break;
		case 36: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_8;	font_addr = ENG_FONTALL_START_ADDRESS_8;	break;
		//case 37: VRAM_FileBuf = (U8 *)0x6B690000;	font_addr = 0x6B690000;	break;
		//case 38: VRAM_FileBuf = (U8 *)0x6B6F0000;	font_addr = 0x6B6F0000;	break;
		default: Dprintf("DrawText:: English Font Not Load=%d\n", g_FontNo);
	}
#ifdef PC_FONT_USE	
	U8		*HAN_VRAM_FileBuf = NULL;
	U32		han_font_addr=0;
	//!< 컴파일러경고방지용
	han_font_addr= han_font_addr;
	HAN_VRAM_FileBuf=HAN_VRAM_FileBuf;
	switch(g_HangulFontNo)
	{
		case 37: HAN_VRAM_FileBuf = (U8 *)HANGUL_FONT16_START_ADDRESS_1;	han_font_addr = HANGUL_FONT16_START_ADDRESS_1;	break;  //16 Font
		case 38: HAN_VRAM_FileBuf = (U8 *)HANGUL_FONT16_START_ADDRESS_2;	han_font_addr = HANGUL_FONT16_START_ADDRESS_2;	break;  //16 Font
		case 39: HAN_VRAM_FileBuf = (U8 *)HANGUL_FONT24_START_ADDRESS;	han_font_addr = HANGUL_FONT24_START_ADDRESS;	break;  //24 Font
		default: Dprintf("DrawText:: hangul Font Not Load=%d\n", g_HangulFontNo);
    }
#endif    
	while(*text)
    {
    	if(*text >= 0x20 && *text <=0x7f)//영문일경우에 실행된다.
		{            
            //Dputc(*text);
            
			if(*text != 0x20)
			{
				position = *text - 0x21;
				if(gFontSize==16)
				{
					position = position * 32;
					VRAM_FileBuf = (U8 *)(font_addr+position);
//Dprintf("DrawText::Addr=%08x\n", VRAM_FileBuf);
					//VRAM_FileBuf += position;
					for(i=0; i<32; i++) FontBuf[i] = *VRAM_FileBuf++;
					FontAlign16();
					WinFont16X16Putc();
				}
				else if(gFontSize==24)
				{
					position = position * 72;
					VRAM_FileBuf = (U8 *)(font_addr+position);
//Dprintf("DrawText::Addr=%08x\n", VRAM_FileBuf);
					//VRAM_FileBuf += position;
					for(i=0; i<72; i++) FontBuf[i] = *VRAM_FileBuf++;
					FontAlign24();
//Dprintf("Font Left=%d Right=%d\n", gFontLeft, gFontRight);				
					WinFont24X24Putc();
				}
				else if(gFontSize==32)
				{
					position = position * 128;
					VRAM_FileBuf = (U8 *)(font_addr+position);
//Dprintf("DrawText::Addr=%08x\n", VRAM_FileBuf);
					//VRAM_FileBuf += position;
					for(i=0; i<128; i++) FontBuf[i] = *VRAM_FileBuf++;
					FontAlign32();
//Dprintf("Font Left=%d Right=%d\n", gFontLeft, gFontRight);				
					WinFont32X32Putc();
				}
				else if(gFontSize==64)
				{
					position = position * 512;
					VRAM_FileBuf = (U8 *)(font_addr+position);
//Dprintf("DrawText::Addr=%08x\n", VRAM_FileBuf);
					//VRAM_FileBuf += position;
					for(i=0; i<512; i++) FontBuf[i] = *VRAM_FileBuf++;
					FontAlign64();
//Dprintf("Font Left=%d Right=%d\n", gFontLeft, gFontRight);				
					WinFont64X64Putc();
				}
			}
			else
			{
				CRTX += gSpaceFontGap;
			}
			text++;
		}
#ifdef  PC_FONT_USE
		else //한글출력(유니코드용)
		{
			U16		high, low;
			U8		*HAN_VRAM_FileBuf = NULL;
			U32		han_font_addr=0;
			U16     ks5601_code;
			U16     unicode;
			
            //gHangulFontSize = gFontSize_X[no]; //참조용

			high = *text++;
			low = *text++;
            ks5601_code = (high << 8) | low;

            //Dprintf("ks5601=%04x\n", ks5601_code);
            for(i=0; i<2350; i++)
            {
                if(Unicode_Table[i][0] == ks5601_code)
                {
                    //Dprintf("UniCode Found Src=%04x Unicode=%04x\n", ks5601_code, Unicode_Table[i][1]);
                    unicode = Unicode_Table[i][1];
                    break;
                }
            }
            if(gHangulFontSize==16)
            {
                //한글폰트가 들어있는 메모리영역
                position = unicode - 0xAC00;
                position *= 32;
                HAN_VRAM_FileBuf = (U8 *)(han_font_addr+position);
                //Dprintf("DrawText::Hangul Position = %d  Addr=%p\n", position, HAN_VRAM_FileBuf);
                for(i=0; i<32; i++) FontBuf[i] = *HAN_VRAM_FileBuf++;
                HangulPutc16();			
            }
            else if(gHangulFontSize==24)
            {
                //한글폰트가 들어있는 메모리영역
                position = unicode - 0xAC00;
                position *= 72;
                HAN_VRAM_FileBuf = (U8 *)(han_font_addr+position);
                //Dprintf("DrawText::Hangul Position = %d  Addr=%p\n", position, HAN_VRAM_FileBuf);
                for(i=0; i<72; i++) FontBuf[i] = *HAN_VRAM_FileBuf++;
                HangulPutc24();			
            }
		}
#endif						
/**************************************  이코드는 내장된 완성형출력코드이다
		else //한글출력
		{
			high = *text++;
			high = high - 0xb0;
			high = high * 96;
			
			low = *text++;
			low = low - 0xa0;
			
			count = high + low;	  
				
			for(i=0; i<32; i++) 
			{			 			
				//Dprintf("Data::%02x \n", Hangul_t[count][i]);
				//FontBuf[i] = ~ Hangul_t[count][i];
				FontBuf[i] = ~ Hangul_g12b[count][i];
			}
			HangulPutc();			
		}
***************************************************************/        
    }
    return(1);
}

int GetTextHeight(int no)
{
	return(gFontSize_Y[no]);
}


U8 *GetTextWidth_Pointer(void)
{
	U8		*VRAM_FileBuf = NULL;    
	switch(g_FontNo)
	{
        //0~19까지는 16,16폰트만 전용으로 로드하는 공간이다.
		case 0:  VRAM_FileBuf = (U8 *)ENG_FONT16_START_ADDRESS;   	break;
		case 1:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 2:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 3:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 4:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 5:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 6:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 7:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 8:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 9:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 10: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 11: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 12: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 13: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 14: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 15: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 16: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 17: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 18: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
		case 19: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*g_FontNo));   break;
        
        //20~28까지는 24,24폰트만 전용으로 로드하는 공간이다.
		case 20: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
		case 21: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
		case 22: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
		case 23: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
		case 24: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
		case 25: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
		case 26: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
		case 27: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
		case 28: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(g_FontNo-20)));  break;
        
		case 29: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_1;		break;
		case 30: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_2;		break;
		case 31: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_3;		break;
		case 32: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_4;		break;
		case 33: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_5;		break;
		case 34: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_6;		break;
		case 35: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_7;		break;
		case 36: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_8;		break;
		//case 37: VRAM_FileBuf = (U8 *)0x6B690000;	font_addr = 0x6B690000;	break;
		//case 38: VRAM_FileBuf = (U8 *)0x6B6F0000;	font_addr = 0x6B6F0000;	break;
		default: Dprintf("DrawText:: English Font Not Load=%d\n", g_FontNo);
	}
    return(VRAM_FileBuf);
}

int GetTextWidth(const char *text)
{
	//U8		buf[32];
	int		i;
	int 	position;	
	U8		*VRAM_FileBuf = NULL;
	int		width = 0;
	int		diff;

	
	while(*text)
    {
		gFontLeft = 0;	gFontRight = 0;		
		if(*text >= 0x80) //한글일때 처리한다, 16 사이로만 적용된다.
		{
			width+=8;
		}
		else if(*text > 0x20 && *text < 0x80)
		{
            VRAM_FileBuf = GetTextWidth_Pointer();
			position = *text - 0x21;
			if(gFontSize==16)
			{
				position = position * 32;
				//VRAM_FileBuf = (U8 *)(FONT_START_ADDRESS+position);
				VRAM_FileBuf += position;
				for(i=0; i<32; i++) FontBuf[i] = *VRAM_FileBuf++;
				FontAlign16();				
			}
			else if(gFontSize==24)
			{
				position = position * 72;
				//VRAM_FileBuf = (U8 *)(FONT_START_ADDRESS+position);
				VRAM_FileBuf += position;
				for(i=0; i<72; i++) FontBuf[i] = *VRAM_FileBuf++;
				FontAlign24();
				//Dprintf("Font Left=%d Right=%d\n", gFontLeft, gFontRight);				
			}
			else if(gFontSize==32)
			{
				position = position * 128;
				//VRAM_FileBuf = (U8 *)(FONT_START_ADDRESS+position);
				VRAM_FileBuf += position;
				for(i=0; i<128; i++) FontBuf[i] = *VRAM_FileBuf++;
				FontAlign32();
				//Dprintf("Font Left=%d Right=%d\n", gFontLeft, gFontRight);				
			}
			else if(gFontSize==64)
			{
				position = position * 512;
				//VRAM_FileBuf = (U8 *)(FONT_START_ADDRESS+position);
				VRAM_FileBuf += position;
				for(i=0; i<512; i++) FontBuf[i] = *VRAM_FileBuf++;
				FontAlign64();
				//Dprintf("Font Left=%d Right=%d\n", gFontLeft, gFontRight);				
			}
            //Dprintf("GetTextWidth::gFontRight=%d gFontLeft=%d\n", gFontRight, gFontLeft);
			diff = gFontRight - gFontLeft;
			width += diff;
			width += gFontGap;
            width++;
            //Dprintf("GetTextWidth::width=%d\n", width);
		}
		else if(*text==0x20)
		{
			width += gSpaceFontGap;
		}
        text++;
    }
	//Dprintf("GetTextWidth:: Len=%d\n", width);	
	return(width);
}

//주어진 이름의 폰트가 낸드의 리스트에 있는지 검사하는 함수이다.영문전용
int LoadFont(int no, char *fontname)
{
	int 	rowbyte, size;
	st_NandFAT NandFat;
	int		start_block;
	int		end_block;
	int		page, y, i;
	int		xs;
	int		ys;
	//U32		addr;
	U8		*VRAM_FileBuf;
	int		cnt=0;

	if(no > 36)
    {
        Dprintf("LoadFont::Font Address Error=%d\n", no);
        return(0);
    }
    
	switch(no)
	{
        //0~19까지는 16,16폰트만 전용으로 로드하는 공간이다.
		case 0:  VRAM_FileBuf = (U8 *)ENG_FONT16_START_ADDRESS; break;
		case 1:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 2:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 3:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 4:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 5:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 6:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 7:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 8:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 9:  VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 10: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 11: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 12: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 13: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 14: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 15: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 16: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 17: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 18: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
		case 19: VRAM_FileBuf = (U8 *)(ENG_FONT16_START_ADDRESS+(3200*no)); break;
        
        //20~28까지는 24,24폰트만 전용으로 로드하는 공간이다.
		case 20: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
		case 21: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
		case 22: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
		case 23: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
		case 24: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
		case 25: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
		case 26: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
		case 27: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
		case 28: VRAM_FileBuf = (U8 *)(ENG_FONT24_START_ADDRESS+(7200*(no-20))); break;
        
		case 29: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_1; break;
		case 30: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_2; break;
		case 31: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_3; break;
		case 32: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_4; break;
		case 33: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_5; break;
		case 34: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_6; break;
		case 35: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_7; break;
		case 36: VRAM_FileBuf = (U8 *)ENG_FONTALL_START_ADDRESS_8; break;
	}
    //Dprintf("LoadFont::fontname=%s No=%d Addr=%08x\n", 	fontname, no, VRAM_FileBuf);
	if(SearchNandFile(fontname, &NandFat))
	{
		xs = NandFat.x_size;
		ys = NandFat.y_size;
		if(no >=0 && no < 20)
        {
            if(xs != 16)
            {
                Dprintf("LoadFont::FontNo 0 ~ 19 is FONT Size 16 Only Area[now font=%d]\n", xs);
                return(0);
            }
        }
		if(no >=20 && no < 29)
        {
            if(xs != 24)
            {
                Dprintf("LoadFont::FontNo 20 ~ 28 is FONT Size 24 Only Area[now font=%d]\n", xs);
                return(0);
            }
        }        
        
        gFontSize_X[no] = xs;
		gFontSize_Y[no] = ys;
		start_block = NandFat.start_blk;
		end_block = NandFat.end_blk;
		
		rowbyte = xs / 8;
		size = ys * rowbyte * 100;
        //Dprintf("LoadFont::Size=%d\n", 	size);
		
		for(i=start_block; i<=end_block; i++)
		{
			for(page=0; page <64; page++)
			{
				BSP_NandReadpage(start_block, page, (U8 *)FontBuf);
				for(y=0; y<2048; y++)
				{	
					*VRAM_FileBuf++ = FontBuf[y]; 
				}
				cnt += 2048;
				if(cnt > size)  //실제 폰트사이즈 용량을 로드하면 탈출한다. 
                {
                    gFontSize = xs;
                    return(1);
                }
			}
			start_block++;
			while(1)
			{
				if(!IsBadBlock(start_block)) 
				{
					break;
				}
				else start_block++;
			}
		}
		//Dprintf("LoadFont::Name=%s xs=%d ys=%d start=%d end=%d\n", fontname, xs, ys, NandFat.start_blk, NandFat.end_blk);
		gFontSize = xs;
		return(1);
	}
	else 
	{
		Dprintf("LoadFont::File not Found %s\n", fontname);
		return 0;
	}
}

//유니코드한글은 용량이 크기때문에 2개만 로드하기로 한다.
//영역은 AC00 ~ D7AF 까지이다.
int LoadHangulFont(int no, char *fontname)
{
	//int 	rowbyte;
    int     size;
	st_NandFAT NandFat;
	int		start_block;
	int		end_block;
	int		page, y, i;
	int		xs;
	int		ys;

	U8		*VRAM_FileBuf = NULL;
	int		cnt=0;

	if(no < 37 || no > 39)
    {
        Dprintf("LoadHangulFont::Font Address Error[37~38]=%d\n", no);
        return(0);
    }
      
	switch(no)
	{
		case 37:  VRAM_FileBuf = (U8 *)HANGUL_FONT16_START_ADDRESS_1; break;
		case 38:  VRAM_FileBuf = (U8 *)HANGUL_FONT16_START_ADDRESS_2; break;
		case 39:  VRAM_FileBuf = (U8 *)HANGUL_FONT24_START_ADDRESS; break;
	}

	if(SearchNandFile(fontname, &NandFat))
	{
		xs = NandFat.x_size;
		ys = NandFat.y_size;
        
        gFontSize_X[no] = xs;
		gFontSize_Y[no] = ys;
		start_block = NandFat.start_blk;
		end_block = NandFat.end_blk;
		
		size = NandFat.Size;
Dprintf("LoadHangulFont::start=%d end=%d xs=%d ys=%d size=%d\n", start_block, end_block, xs, ys, size);		
		for(i=start_block; i<=end_block; i++)
		{
			for(page=0; page <64; page++)
			{
				BSP_NandReadpage(start_block, page, (U8 *)FontBuf);
				for(y=0; y<2048; y++)
				{	
					*VRAM_FileBuf++ = FontBuf[y]; 
				}
				cnt += 2048;
				if(cnt > size)  //실제 폰트사이즈 용량을 로드하면 탈출한다. 
                {
                    gHangulFontSize = xs;
                    return(1);
                }
			}
			start_block++;
			while(1)
			{
				if(!IsBadBlock(start_block)) 
				{
					break;
				}
				else start_block++;
			}
		}
		Dprintf("LoadFont::Name=%s xs=%d ys=%d start=%d end=%d\n", fontname, xs, ys, NandFat.start_blk, NandFat.end_blk);
		gHangulFontSize = xs;
		return(1);
	}
	else 
	{
		Dprintf("LoadHangulFont::File not Found %s\n", fontname);
		return 0;
	}
}
#ifdef __cplusplus
}
#endif
