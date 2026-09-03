#ifndef LCD5X7
    #define LCD5X7
#ifdef __cplusplus
 extern "C" {
#endif

#include "TEECO_HW_Config.h"
	 
#ifdef LOW_FONT_USE

#include "TEECO_System.h"
#include "Windows.h"
#include "WindowsGraphics.h"

extern const char Abadi10[95][33];
extern const char Abadi12[95][33];
extern const char Arial18[95][73];
extern const char Arial12[95][33];
extern const char IBM15[95][73];    	
extern const char TimeRoma12b[95][33];    	
extern const char TimeRoma12[95][33];    	
extern const char TimeRoma9b[95][33];   	
extern const char TimeRoma9[95][33];      	
extern const char TimeRoma16[95][73];    
extern const char TimeRoma16b[95][73];    
extern const char Time18[95][73];   
extern const char Roma29[95][201];
extern const char ascii[128][12];     	
extern const char MS_Serief9[95][33];
//extern const char EngFont[95][16];
//extern const char Hangul_g12b[2400][72];

//static int CRTX, CRTY;
extern volatile int CRTX, CRTY;
extern volatile U32	Graphics_Address;
extern volatile U16 	g_Color;       

//===============================================================================================================
//Panel에 사용되는 함수.
//출력되는 텍스트 길이를 구하는 함수이다.
//===============================================================================================================
int LowTextLen(char *s, int Font)
{
	int	len=0;
	char 	t=0;
	
	len=len;
	t = t;
	switch(Font)
	{
#ifdef	ABADI10_FONT_USE
		case	fAbadi10:
		{
			while(*s)
			{
				t= *s - 0x20;
          		len += (int)Abadi10[t][0];
           		s++;
			}
			break;
		}
#endif				
#ifdef	ABADI12_FONT_USE
		case	fAbadi12:
		{
			while(*s)
			{
				t= *s - 0x20;
           		len += (int)(Abadi12[t][0]);
           		s++;
			}
			break;
		}
#endif				
#ifdef ARIAL12_FONT_USE
		case	fArial12:
		{
			while(*s)
			{
				t= *s - 0x20;
           		len += (int)(Arial12[t][0]);
          		s++;
			}
			break;
		}
#endif				
#ifdef ARIAL18_FONT_USE
		case	fArial18:
		{
			while(*s)
			{
				t= *s - 0x20;
           		len += (int)(Arial18[t][0]);
           		s++;
			}
			break;
		}
#endif				
#ifdef 	IBM15_FONT_USE			
		case	fIBM15:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(IBM15[t][0]);
		        s++;
			}
			break;
		}
#endif				
#ifdef 	TIMEROMA12B_FONT_USE			
		case	fTimeRoma12b:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(TimeRoma12b[t][0]);
		        s++;
			}
			break;
		}
#endif				
#ifdef 	TIMEROMA12_FONT_USE			
		case	fTimeRoma12:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(TimeRoma12[t][0]);
		        s++;
			}
			break;
		}
#endif				
#ifdef TIMEROMA9B_FONT_USE
		case	fTimeRoma9b:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(TimeRoma9b[t][0]);
		        s++;
			}
			break;
		}
#endif				
#ifdef TIMEROMA9_FONT_USE
		case	fTimeRoma9:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(TimeRoma9[t][0]);
		        s++;
			}
			break;
		}
#endif				
#ifdef TIMEROMA16B_FONT_USE
		case	fTimeRoma16b:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(TimeRoma16b[t][0]);
		        s++;
			}
			break;
		}
#endif				
#ifdef TIMEROMA16_FONT_USE
		case	fTimeRoma16:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(TimeRoma16[t][0]);
		        s++;
			}
			break;
		}
#endif				
#ifdef 	TIME18_FONT_USE			
		case	fTime18:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(Time18[t][0]);
		        s++;
			}
			break;
		}
#endif				
#ifdef 	ROMA29_FONT_USE			
		case	fRoma29:
		{
			while(*s)
			{
				t= *s - 0x20;
		        len += (int)(Roma29[t][0]);
		        s++;
			}
			break;
		}
#endif				
	}
	return(len);
}


/*
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
*/


void LowFontPlot(U16 x, U16 y, U16 color)
{    
/*
	volatile U16 	*pScreen= (U16 *)Graphics_Address;
    U32 position= ((sy * LCD_X)+ sx);
    pScreen += position;
    *pScreen = color; //g_Color; //color;	
*/

	volatile U16 	*pScreen = (U16 *)Graphics_Address;
	int		sx = x + gp_Graphics->Offset_X;
	int		sy = y + gp_Graphics->Offset_Y;

    U32 position= ((sy * LCD_X)+ sx);
    pScreen += position;
    *pScreen = color; //g_TextColor;	    
}

void Font8X16Putc(char *s, int color, int backcolor) // 예전의 MiddlePut();
{
    int i;
    int x , y;

    x=CRTX; y=CRTY;


    for(i=0; i<=11; i++)
	{
        if(*s & 0x80) LowFontPlot(x++, y,color);
        else  x++;
        if(*s & 0x40) LowFontPlot(x++, y,color);
        else  x++;
        if(*s & 0x20) LowFontPlot(x++, y,color);
        else  x++;
        if(*s & 0x10) LowFontPlot(x++, y,color);
        else  x++;
        if(*s & 0x08) LowFontPlot(x++, y,color);
        else  x++;
        if(*s & 0x04) LowFontPlot(x++, y,color);
        else  x++;
        if(*s & 0x02) LowFontPlot(x++, y,color);
        else  x++;
        if(*s & 0x01) LowFontPlot(x++, y,color);
        else  x++;
        s++;
        y++;
        x = CRTX;        
    }
    CRTX += 8;
}


void Font16X16Putc(char *s, int color, int backcolor ) //예전의 Arial12Putc();
{
    int     i, rowcount;
    int     x, y;

    x=CRTX; y=CRTY;

    rowcount= *s++;

    for(i=0; i<=15; i++)
	{
        switch(rowcount)
        {
            case 8:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                 else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 0:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                /*
                if(*s & 0x08) LowFontPlot(x++,y,color);
                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);
                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color);
                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color);
                else  x++;
                */
                break;
            }
            case 7:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 6:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 5:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 4:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 3:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 2:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 1:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            default:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
        }
        s++;
        switch(rowcount)
        {
            case 16:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 15:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 14:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 13:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 12:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 11:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 10:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 9:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                break;
            }
        }
        s++;
        y++;
        x = CRTX;
    }
    if(rowcount==0) CRTX+=4;
    else  CRTX+= (16-(16-rowcount));
}

void Font24X24Putc(char *s, int color, int backcolor) // 예전의 Arial18Putc();
{
    int     i, rowcount;
    int     x, y;

    x=CRTX; y=CRTY;

    rowcount= *s++;	// 폰트의 옆넓이이다.

    for(i=0; i<=23; i++) // 23은 폰트의 길이이다. 
	{
        switch(rowcount)
        {
            case 0:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color);                else  x++;
                /*
                if(*s & 0x08) LowFontPlot(x++,y,color);
                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color);
                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color);
                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color);
                else  x++;
                */
                break;
            }
            case 7:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02)  LowFontPlot(x++,y,color);                else  x++;
                break;        
            }
            case 6:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 5:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 4:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 3:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 2:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 1:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            default:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x01)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
        }

        s++;
        switch(rowcount)
        {
            case 16:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x01)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 15:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 14:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 13:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 12:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 11:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 10:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 9:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            default:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x01)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
        }
        s++;
        switch(rowcount)
        {
            case 24:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x01)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 23:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x02)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 22:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x04)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 21:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x08)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 20:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x10)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 19:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x20)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 18:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                if(*s & 0x40)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
            case 17:
            {
                if(*s & 0x80)  LowFontPlot(x++,y,color);                else  x++;
                break;
            }
        }
        s++;
        y++;
        x = CRTX;
    }
    if(rowcount==0) CRTX+=4;
    else  CRTX += (24- (24-rowcount) );
}

void Font40X40Putc(char *s, int color, int backcolor) //예전의 Roma29Putc();
{
    int     i, j, rowcount;
    int     x, y;


    x=CRTX; y=CRTY;

    rowcount= *s++;

    for(i=0; i<=39; i++)
	{
        switch(rowcount)
        {
            case 0:
            {
                for(j=0; j< 20; j++) x++;
                break;
            }
            case 7:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 6:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 5:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 4:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 3:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 2:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 1:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            default:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
        }

        s++;
        switch(rowcount)
        {
            case 16:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 15:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 14:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 13:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 12:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 11:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 10:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 9:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            default:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
        }
        s++;
        switch(rowcount)
        {
            case 24:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 23:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 22:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 21:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 20:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 19:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 18:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 17:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            default:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
        }
        s++;
        switch(rowcount)
        {
            case 32:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 31:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 30:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 29:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 28:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 27:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 26:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 25:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            default:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
        }
        s++;
        switch(rowcount)
        {
            case 40:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x01) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 39:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x02) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 38:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x04) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 37:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x08) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 36:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x10) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 35:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x20) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 34:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                if(*s & 0x40) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
            case 33:
            {
                if(*s & 0x80) LowFontPlot(x++,y,color) ;                else  x++;
                break;
            }
        }
        s++;
        y++;
        x = CRTX;
    }
    if(rowcount==0) CRTX+=8;
    else  CRTX += (40-(40-rowcount));
}

int LowTextOut(int sx, int sy, const char *s, int fontcolor, int font)
{
	char	buf[300];
	int	i=0;
	char	sd=0;	
    int backcolor=0;
	CRTX = sx, CRTY = sy;	// 그래픽 포인터를 이동한다.	

	//!< 컴파일러 경고방지용
	i=i; sd=sd;buf[0] = buf[0];
	backcolor = backcolor;
    while(*s)
    {
        switch(font)
        {
#ifdef MS_SERIEF9_FONT_USE
			case fMS12:
				sd= *s - 0x20;
			for(i=0; i<=33; i++) buf[i] = MS_Serief9[sd][i];
       			Font16X16Putc(buf, fontcolor, backcolor);
       			break;
#endif			
#ifdef ABADI10_FONT_USE
			case fAbadi10:
				sd= *s - 0x20;
			for(i=0; i<=33; i++) buf[i] = Abadi10[sd][i];
       			Font16X16Putc(buf, fontcolor, backcolor);
       			break;
#endif			
            
#ifdef ABADI12_FONT_USE
			case fAbadi12:
				sd= *s - 0x20;
				for(i=0; i<=33; i++) buf[i] = Abadi12[sd][i];
       			Font16X16Putc(buf, fontcolor, backcolor);
       			break;
#endif			
            
#ifdef ARIAL12_FONT_USE
			case fArial12:
				sd= *s - 0x20;
				for(i=0; i<=33; i++) buf[i] = Arial12[sd][i];
       			Font16X16Putc(buf, fontcolor, backcolor);
       			break;
#endif			
#ifdef 	ARIAL18_FONT_USE		
	       	case fArial18:
				sd= *s - 0x20;
		        for(i=0; i<=72; i++) buf[i] = Arial18[sd][i];
		        Font24X24Putc(buf, fontcolor, backcolor);
		        break;
#endif			
#ifdef 	TIMEROMA9_FONT_USE		
			case fTimeRoma9:	
				sd= *s - 0x20;
		        for(i=0; i<=33; i++) buf[i] = TimeRoma9[sd][i];
       			Font16X16Putc(buf, fontcolor, backcolor);
				break;	        
#endif			
#ifdef 	TIMEROMA9B_FONT_USE		
			case fTimeRoma9b:	
				sd= *s - 0x20;
		        for(i=0; i<=33; i++) buf[i] = TimeRoma9b[sd][i];
       			Font16X16Putc(buf, fontcolor, backcolor);
				break;	        
#endif			
#ifdef 	LCD5X7_FONT_USE			
			case fRoma8b:	
		        for(i=0; i<=11; i++) buf[i] = ascii[*s][i];
        		Font8X16Putc(buf, fontcolor, backcolor);
        		break;            
#endif			
#ifdef 	TIMEROMA12_FONT_USE		
			case fTimeRoma12:	
				sd= *s - 0x20;
		        for(i=0; i<=33; i++) buf[i] = TimeRoma12[sd][i];
       			Font16X16Putc(buf, fontcolor, backcolor);
       			break;       
#endif			
#ifdef 	TIMEROMA12B_FONT_USE		
			case fTimeRoma12b:	
				sd= *s - 0x20;
		        for(i=0; i<=33; i++) buf[i] = TimeRoma12b[sd][i];
       			Font16X16Putc(buf, fontcolor, backcolor);
       			break;
#endif			
            
#ifdef  TIMEROMA16_FONT_USE
			case fTimeRoma16:	
				sd= *s - 0x20;
				for(i=0; i<=72; i++) buf[i] = TimeRoma16[sd][i];
       			Font24X24Putc(buf, fontcolor, backcolor);
       			break;       
#endif			
#ifdef 	TIMEROMA16B_FONT_USE		
			case fTimeRoma16b:	
				sd= *s - 0x20;
	        	for(i=0; i<=72; i++) buf[i] = TimeRoma16b[sd][i];
       			Font24X24Putc(buf, fontcolor, backcolor);
       			break;
#endif			
#ifdef 	IBM15_FONT_USE		
			case fIBM15:	
				sd= *s - 0x20;
		        for(i=0; i<=72; i++) buf[i] = IBM15[sd][i];
       			Font24X24Putc(buf, fontcolor, backcolor);
       			break;
#endif			
#ifdef 	TIME18_FONT_USE		
			case fTime18:		
				sd= *s - 0x20;
		        for(i=0; i<=72; i++) buf[i] = Time18[sd][i];
       			Font24X24Putc(buf, fontcolor, backcolor);
       			break;
#endif			
#ifdef 	ROMA29_FONT_USE		
			case fRoma29:		
				sd= *s - 0x20;
		        for(i=0; i<=200; i++) buf[i] = Roma29[sd][i];
        		Font40X40Putc(buf, fontcolor, backcolor);
				break;			
#endif			
            
        }
        s++;
    } 
	return(1);	
}

#endif

#ifdef __cplusplus
}
#endif

#endif
