/******************************************************************************
*    	File name	: WindowsGraphics.c
*		Description : GRAPHICS File
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif


/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "Windows.h"
#include "memorymanager.h"

volatile U32	Graphics_Address;
volatile U32	position;
volatile int	gi_ScreenX=0, gi_ScreenY=0;
volatile U16 	g_Color;
volatile U32	g_GPU_addr;

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define VGX_MMIO_ARENA					0x6be00000
//#define POKE_16(address, value)         *(volatile unsigned short int *)(VGX_MMIO_ARENA + (address)) = (value)
//#define POKE_32(address, value)         *(volatile unsigned long *)(VGX_MMIO_ARENA + (address)) = (value)
//#define PEEK_32(address)                *(volatile unsigned long *)(VGX_MMIO_ARENA + (address))

#define PITCH(width, bpp)               (((width) * (bpp) / 8 + 15) & ~15)
/******************************************************************************
							Private & Local Variables
******************************************************************************/

extern 	volatile U8		FlashPageBuf[2048];	//Nand Flash Memory의 페이지를 조작하기위한 버퍼, BootFile.c
int IsBadBlock(int block);
void SetColor(U16 color)
{
	g_Color = color;	
}

/******************************************************************************
		Function name 			: GraphicsChange
		Function prototype 		: void GraphicsChange(int Page_No)
		Behavior description	: 설정하면 이페이지에 모든 그래픽명령이 적용된다.
		Input parameter			: PageNo 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: OnPaint
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void SetGraphicsPage(int Page_No)
{
	switch(Page_No)
	{
		case LCD_SCREEN:				Graphics_Address = LCD_SCREEN_ADDR; 		g_GPU_addr = GPU_LCD_SCREEN; break;

		case LCD_ALPHA_SCREEN:			Graphics_Address = LCD_ALPHA_SCREEN_ADDR; 	g_GPU_addr = GPU_LCD_ALPHA_SCREEN; break;
//		case CRT_SCREEN:				Graphics_Address = CRT_SCREEN_ADDR; 		g_GPU_addr = GPU_CRT_SCREEN; break;
//		case CRT_ALPHA_SCREEN:			Graphics_Address = CRT_ALPHA_SCREEN_ADDR;	g_GPU_addr = GPU_CRT_ALPHA_SCREEN; break;
//		case VIDEO_SCREEN:				Graphics_Address = VIDEO_SCREEN_ADDR; 		g_GPU_addr = GPU_VIDEO_SCREEN; break;
#ifdef LCD17INCH		
		case VIDEO_ALPHA_SCREEN:		Graphics_Address = VIDEO_ALPHA_SCREEN_ADDR; g_GPU_addr = GPU_VIDEO_ALPHA_SCREEN; break;
#endif		
//		case CAPTURE1_SCREEN:			Graphics_Address = CAPTURE1_SCREEN_ADDR; 	g_GPU_addr = GPU_CAPTURE1_SCREEN; break;
//		case CAPTURE2_SCREEN:			Graphics_Address = CAPTURE2_SCREEN_ADDR; 	g_GPU_addr = GPU_CAPTURE2_SCREEN; break;

		case CONTAINER_WINDOW1_SCREEN:	Graphics_Address = CONTAINER_WINDOW1_ADDR; 	g_GPU_addr = GPU_CONTAINER_WINDOW1; break;
		case CONTAINER_WINDOW2_SCREEN:	Graphics_Address = CONTAINER_WINDOW2_ADDR; 	g_GPU_addr = GPU_CONTAINER_WINDOW2; break;
		case CONTAINER_WINDOW3_SCREEN:	Graphics_Address = CONTAINER_WINDOW3_ADDR; 	g_GPU_addr = GPU_CONTAINER_WINDOW3; break;
		case CONTAINER_WINDOW4_SCREEN:	Graphics_Address = CONTAINER_WINDOW4_ADDR; 	g_GPU_addr = GPU_CONTAINER_WINDOW4; break;
		case CONTAINER_WINDOW5_SCREEN:	Graphics_Address = CONTAINER_WINDOW5_ADDR; 	g_GPU_addr = GPU_CONTAINER_WINDOW5; break;

		case CONTAINER_WIDGET1_SCREEN:	Graphics_Address = CONTAINER_WIDGET1_ADDR; 	g_GPU_addr = GPU_CONTAINER_WIDGET1; break;
		case CONTAINER_WIDGET2_SCREEN:	Graphics_Address = CONTAINER_WIDGET2_ADDR; 	g_GPU_addr = GPU_CONTAINER_WIDGET2; break;
		case CONTAINER_WIDGET3_SCREEN:	Graphics_Address = CONTAINER_WIDGET3_ADDR; 	g_GPU_addr = GPU_CONTAINER_WIDGET3; break;
		case CONTAINER_WIDGET4_SCREEN:	Graphics_Address = CONTAINER_WIDGET4_ADDR; 	g_GPU_addr = GPU_CONTAINER_WIDGET4; break;
		case CONTAINER_WIDGET5_SCREEN:	Graphics_Address = CONTAINER_WIDGET5_ADDR; 	g_GPU_addr = GPU_CONTAINER_WIDGET5; break;

		case DOUBLE_SCREEN:				Graphics_Address = DOUBLE_SCREEN_ADDR; 		g_GPU_addr = GPU_DOUBLE_SCREEN; break;
//		case DESKTOP_SCREEN:			Graphics_Address = DESKTOP_SCREEN_ADDR; 	g_GPU_addr = GPU_DESKTOP_SCREEN; break;
//		case DESKTOPWIDGET_SCREEN:		Graphics_Address = DESKTOPWIDGET_SCREEN_ADDR; g_GPU_addr = GPU_DESKTOPWIDGET_SCREEN; break;


		case LCD_ALPHA_CONTAINER_SCREEN:	Graphics_Address = LCD_ALPHA_CONTAINER_ADDR; 	g_GPU_addr = GPU_LCD_ALPHA_CONTAINER_ADDR; break;
		case LCD_ALPHA_WIDGET_SCREEN:		Graphics_Address = LCD_ALPHA_WIDGET_ADDR; 	 	g_GPU_addr = GPU_LCD_ALPHA_WIDGET_ADDR; break;
//		case VIDEO_ALPHA_CONTAINER_SCREEN:	Graphics_Address = VIDEO_ALPHA_CONTAINER_ADDR; 	g_GPU_addr = GPU_VIDEO_ALPHA_CONTAINER_ADDR; break;
//		case VIDEO_ALPHA_WIDGET_SCREEN:		Graphics_Address = VIDEO_ALPHA_WIDGET_ADDR; 	g_GPU_addr = GPU_VIDEO_ALPHA_WIDGET_ADDR; break;
		case LCD_ALPHA_DOUBLE_SCREEN:		Graphics_Address = LCD_ALPHA_DOUBLE_ADDR; 		g_GPU_addr = GPU_LCD_ALPHA_DOUBLE_ADDR; break;
//		case VIDEO_ALPHA_DOUBLE_SCREEN:		Graphics_Address = VIDEO_ALPHA_DOUBLE_ADDR; 	g_GPU_addr = GPU_VIDEO_ALPHA_DOUBLE_ADDR; break;

		case USER_SCREEN1:				Graphics_Address = USER_SCREEN1_ADDR; 		g_GPU_addr = GPU_USER_SCREEN1; break;
		case USER_SCREEN2:				Graphics_Address = USER_SCREEN2_ADDR; 		g_GPU_addr = GPU_USER_SCREEN2; break;
		case USER_SCREEN3:				Graphics_Address = USER_SCREEN3_ADDR; 		g_GPU_addr = GPU_USER_SCREEN3; break;
		case USER_SCREEN4:				Graphics_Address = USER_SCREEN4_ADDR; 		g_GPU_addr = GPU_USER_SCREEN4; break;
		case USER_SCREEN5:				Graphics_Address = USER_SCREEN5_ADDR; 		g_GPU_addr = GPU_USER_SCREEN5; break;
		case USER_SCREEN6:				Graphics_Address = USER_SCREEN6_ADDR; 		g_GPU_addr = GPU_USER_SCREEN6; break;
		case USER_SCREEN7:				Graphics_Address = USER_SCREEN7_ADDR; 		g_GPU_addr = GPU_USER_SCREEN7; break;
		case USER_SCREEN8:				Graphics_Address = USER_SCREEN8_ADDR; 		g_GPU_addr = GPU_USER_SCREEN8; break;
		case USER_SCREEN9:				Graphics_Address = USER_SCREEN9_ADDR; 		g_GPU_addr = GPU_USER_SCREEN9; break;
		case USER_SCREEN10:				Graphics_Address = USER_SCREEN10_ADDR; 		g_GPU_addr = GPU_USER_SCREEN10; break;

		case USER_SCREEN11:				Graphics_Address = USER_SCREEN11_ADDR; 		g_GPU_addr = GPU_USER_SCREEN11; break;
		case USER_SCREEN12:				Graphics_Address = USER_SCREEN12_ADDR; 		g_GPU_addr = GPU_USER_SCREEN12; break;
		case USER_SCREEN13:				Graphics_Address = USER_SCREEN13_ADDR; 		g_GPU_addr = GPU_USER_SCREEN13; break;
		case USER_SCREEN14:				Graphics_Address = USER_SCREEN14_ADDR; 		g_GPU_addr = GPU_USER_SCREEN14; break;
		case USER_SCREEN15:				Graphics_Address = USER_SCREEN15_ADDR; 		g_GPU_addr = GPU_USER_SCREEN15; break;

		case USER_SCREEN16:				Graphics_Address = USER_SCREEN16_ADDR; 		g_GPU_addr = GPU_USER_SCREEN16; break;
		case USER_SCREEN17:				Graphics_Address = USER_SCREEN17_ADDR; 		g_GPU_addr = GPU_USER_SCREEN17; break;
		case USER_SCREEN18:				Graphics_Address = USER_SCREEN18_ADDR; 		g_GPU_addr = GPU_USER_SCREEN18; break;
		case USER_SCREEN19:				Graphics_Address = USER_SCREEN19_ADDR; 		g_GPU_addr = GPU_USER_SCREEN19; break;
		case USER_SCREEN20:				Graphics_Address = USER_SCREEN20_ADDR; 		g_GPU_addr = GPU_USER_SCREEN20; break;

        case USER_SCREEN21:				Graphics_Address = USER_SCREEN21_ADDR; 		g_GPU_addr = GPU_USER_SCREEN21; break;
		case USER_SCREEN22:				Graphics_Address = USER_SCREEN22_ADDR; 		g_GPU_addr = GPU_USER_SCREEN22; break;
		case USER_SCREEN23:				Graphics_Address = USER_SCREEN23_ADDR; 		g_GPU_addr = GPU_USER_SCREEN23; break;
		case USER_SCREEN24:				Graphics_Address = USER_SCREEN24_ADDR; 		g_GPU_addr = GPU_USER_SCREEN24; break;
		case USER_SCREEN25:				Graphics_Address = USER_SCREEN25_ADDR; 		g_GPU_addr = GPU_USER_SCREEN25; break;
		case USER_SCREEN26:				Graphics_Address = USER_SCREEN26_ADDR; 		g_GPU_addr = GPU_USER_SCREEN26; break;
		case USER_SCREEN27:				Graphics_Address = USER_SCREEN27_ADDR; 		g_GPU_addr = GPU_USER_SCREEN27; break;
		case USER_SCREEN28:				Graphics_Address = USER_SCREEN28_ADDR; 		g_GPU_addr = GPU_USER_SCREEN28; break;
		case USER_SCREEN29:				Graphics_Address = USER_SCREEN29_ADDR; 		g_GPU_addr = GPU_USER_SCREEN29; break;
		case USER_SCREEN30:				Graphics_Address = USER_SCREEN30_ADDR; 		g_GPU_addr = GPU_USER_SCREEN30; break;
		case USER_SCREEN31:				Graphics_Address = USER_SCREEN31_ADDR; 		g_GPU_addr = GPU_USER_SCREEN31; break;
		case USER_SCREEN32:				Graphics_Address = USER_SCREEN32_ADDR; 		g_GPU_addr = GPU_USER_SCREEN32; break;
		case USER_SCREEN33:				Graphics_Address = USER_SCREEN33_ADDR; 		g_GPU_addr = GPU_USER_SCREEN33; break;
		case USER_SCREEN34:				Graphics_Address = USER_SCREEN34_ADDR; 		g_GPU_addr = GPU_USER_SCREEN34; break;
		case USER_SCREEN35:				Graphics_Address = USER_SCREEN35_ADDR; 		g_GPU_addr = GPU_USER_SCREEN35; break;
		case USER_SCREEN36:				Graphics_Address = USER_SCREEN36_ADDR; 		g_GPU_addr = GPU_USER_SCREEN36; break;
		case USER_SCREEN37:				Graphics_Address = USER_SCREEN37_ADDR; 		g_GPU_addr = GPU_USER_SCREEN37; break;
		case USER_SCREEN38:				Graphics_Address = USER_SCREEN38_ADDR; 		g_GPU_addr = GPU_USER_SCREEN38; break;
		case USER_SCREEN39:				Graphics_Address = USER_SCREEN39_ADDR; 		g_GPU_addr = GPU_USER_SCREEN39; break;
		case USER_SCREEN40:				Graphics_Address = USER_SCREEN40_ADDR; 		g_GPU_addr = GPU_USER_SCREEN40; break;
		case USER_SCREEN41:				Graphics_Address = USER_SCREEN41_ADDR; 		g_GPU_addr = GPU_USER_SCREEN41; break;

		case AUX_USER_SCREEN1:			Graphics_Address = AUX_USER_SCREEN1_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN1; break;
#ifndef LCD17INCH			
		case AUX_USER_SCREEN2:			Graphics_Address = AUX_USER_SCREEN2_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN2; break;
#endif		
		case AUX_USER_SCREEN3:			Graphics_Address = AUX_USER_SCREEN3_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN3; break;
		case AUX_USER_SCREEN4:			Graphics_Address = AUX_USER_SCREEN4_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN4; break;
		case AUX_USER_SCREEN5:			Graphics_Address = AUX_USER_SCREEN5_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN5; break;
		case AUX_USER_SCREEN6:			Graphics_Address = AUX_USER_SCREEN6_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN6; break;
		case AUX_USER_SCREEN7:			Graphics_Address = AUX_USER_SCREEN7_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN7; break;
//		case AUX_USER_SCREEN8:			Graphics_Address = AUX_USER_SCREEN8_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN8; break;
//		case AUX_USER_SCREEN9:			Graphics_Address = AUX_USER_SCREEN9_ADDR; 		g_GPU_addr = AUX_GPU_USER_SCREEN9; break;
//		case AUX_USER_SCREEN10:		Graphics_Address = AUX_USER_SCREEN10_ADDR; 	g_GPU_addr = AUX_GPU_USER_SCREEN10; break;
//		case AUX_USER_SCREEN11:		Graphics_Address = AUX_USER_SCREEN11_ADDR; 	g_GPU_addr = AUX_GPU_USER_SCREEN11; break;

		default: Dprintf("GraphicsChange:: PageNo=%d not found\n", Page_No);
	}
	//Dprintf("SetGraphicsPage:: Page=%d Graphics_Address=%p 	 g_GPU_addr=%08x\n", Page_No, Graphics_Address, 	g_GPU_addr);
}

U32 GetGraphicsAddr(int Page_No)
{
	switch(Page_No)
	{
		case LCD_SCREEN:				return(GPU_LCD_SCREEN); 	
		case LCD_ALPHA_SCREEN:			return(GPU_LCD_ALPHA_SCREEN); 	
//		case CRT_SCREEN:				return(GPU_CRT_SCREEN); 		
//		case CRT_ALPHA_SCREEN:			return(GPU_CRT_ALPHA_SCREEN);	
//		case VIDEO_SCREEN:				return(GPU_VIDEO_SCREEN); 		
//		case VIDEO_ALPHA_SCREEN:		return(GPU_VIDEO_ALPHA_SCREEN); 
//		case CAPTURE1_SCREEN:			return(GPU_CAPTURE1_SCREEN); 
//		case CAPTURE2_SCREEN:			return(GPU_CAPTURE2_SCREEN); 
		case CONTAINER_WINDOW1_SCREEN:	return(GPU_CONTAINER_WINDOW1); 	
		case CONTAINER_WINDOW2_SCREEN:	return(GPU_CONTAINER_WINDOW2); 	
		case CONTAINER_WINDOW3_SCREEN:	return(GPU_CONTAINER_WINDOW3); 	
		case CONTAINER_WINDOW4_SCREEN:	return(GPU_CONTAINER_WINDOW4); 	
		case CONTAINER_WINDOW5_SCREEN:	return(GPU_CONTAINER_WINDOW5); 	

		case CONTAINER_WIDGET1_SCREEN:	return(GPU_CONTAINER_WIDGET1); 	
		case CONTAINER_WIDGET2_SCREEN:	return(GPU_CONTAINER_WIDGET2); 	
		case CONTAINER_WIDGET3_SCREEN:	return(GPU_CONTAINER_WIDGET3); 	
		case CONTAINER_WIDGET4_SCREEN:	return(GPU_CONTAINER_WIDGET4); 	
		case CONTAINER_WIDGET5_SCREEN:	return(GPU_CONTAINER_WIDGET5); 	

		case DOUBLE_SCREEN:				return(GPU_DOUBLE_SCREEN); 		
//		case DESKTOP_SCREEN:			return(GPU_DESKTOP_SCREEN); 	
//		case DESKTOPWIDGET_SCREEN:		return(GPU_DESKTOPWIDGET_SCREEN); 

		case LCD_ALPHA_CONTAINER_SCREEN:	return(GPU_LCD_ALPHA_CONTAINER_ADDR); 	
		case LCD_ALPHA_WIDGET_SCREEN:		return(GPU_LCD_ALPHA_WIDGET_ADDR); 	
//		case VIDEO_ALPHA_CONTAINER_SCREEN:	return(GPU_VIDEO_ALPHA_CONTAINER_ADDR); 	
//		case VIDEO_ALPHA_WIDGET_SCREEN:		return(GPU_VIDEO_ALPHA_WIDGET_ADDR); 	
		case LCD_ALPHA_DOUBLE_SCREEN:		return(GPU_LCD_ALPHA_DOUBLE_ADDR); 	
//		case VIDEO_ALPHA_DOUBLE_SCREEN:		return(GPU_VIDEO_ALPHA_DOUBLE_ADDR); 	

		case USER_SCREEN1:				return(GPU_USER_SCREEN1); 		
		case USER_SCREEN2:				return(GPU_USER_SCREEN2); 		
		case USER_SCREEN3:				return(GPU_USER_SCREEN3); 		
		case USER_SCREEN4:				return(GPU_USER_SCREEN4); 		
		case USER_SCREEN5:				return(GPU_USER_SCREEN5); 		
		case USER_SCREEN6:				return(GPU_USER_SCREEN6); 		
		case USER_SCREEN7:				return(GPU_USER_SCREEN7); 		
		case USER_SCREEN8:				return(GPU_USER_SCREEN8); 		
		case USER_SCREEN9:				return(GPU_USER_SCREEN9); 		
		case USER_SCREEN10:				return(GPU_USER_SCREEN10); 		

		case USER_SCREEN11:				return(GPU_USER_SCREEN11); 		
		case USER_SCREEN12:				return(GPU_USER_SCREEN12); 		
		case USER_SCREEN13:				return(GPU_USER_SCREEN13); 		
		case USER_SCREEN14:				return(GPU_USER_SCREEN14); 		
		case USER_SCREEN15:				return(GPU_USER_SCREEN15); 		
		case USER_SCREEN16:				return(GPU_USER_SCREEN16); 		
		case USER_SCREEN17:				return(GPU_USER_SCREEN17); 		
		case USER_SCREEN18:				return(GPU_USER_SCREEN18); 		
		case USER_SCREEN19:				return(GPU_USER_SCREEN19); 		
		case USER_SCREEN20:				return(GPU_USER_SCREEN20); 		

		case USER_SCREEN21:				return(GPU_USER_SCREEN21); 		
		case USER_SCREEN22:				return(GPU_USER_SCREEN22); 		
		case USER_SCREEN23:				return(GPU_USER_SCREEN23); 		
		case USER_SCREEN24:				return(GPU_USER_SCREEN24); 		
		case USER_SCREEN25:				return(GPU_USER_SCREEN25); 		
		case USER_SCREEN26:				return(GPU_USER_SCREEN26); 		
		case USER_SCREEN27:				return(GPU_USER_SCREEN27); 		
		case USER_SCREEN28:				return(GPU_USER_SCREEN28); 		
		case USER_SCREEN29:				return(GPU_USER_SCREEN29); 		
		case USER_SCREEN30:				return(GPU_USER_SCREEN30); 		
		case USER_SCREEN31:				return(GPU_USER_SCREEN31); 		
		case USER_SCREEN32:				return(GPU_USER_SCREEN32); 		
		case USER_SCREEN33:				return(GPU_USER_SCREEN33); 		
		case USER_SCREEN34:				return(GPU_USER_SCREEN34); 		
		case USER_SCREEN35:				return(GPU_USER_SCREEN35); 		
		case USER_SCREEN36:				return(GPU_USER_SCREEN36); 		
		case USER_SCREEN37:				return(GPU_USER_SCREEN37); 		
		case USER_SCREEN38:				return(GPU_USER_SCREEN38); 		
		case USER_SCREEN39:				return(GPU_USER_SCREEN39); 		
		case USER_SCREEN40:				return(GPU_USER_SCREEN40); 		
		case USER_SCREEN41:				return(GPU_USER_SCREEN41); 		

		case AUX_USER_SCREEN1:				return(AUX_GPU_USER_SCREEN1); 		
		case AUX_USER_SCREEN2:				return(AUX_GPU_USER_SCREEN2); 		
		case AUX_USER_SCREEN3:				return(AUX_GPU_USER_SCREEN3); 		
		case AUX_USER_SCREEN4:				return(AUX_GPU_USER_SCREEN4); 		
		case AUX_USER_SCREEN5:				return(AUX_GPU_USER_SCREEN5); 		
		case AUX_USER_SCREEN6:				return(AUX_GPU_USER_SCREEN6); 		
		case AUX_USER_SCREEN7:				return(AUX_GPU_USER_SCREEN7); 		
//		case AUX_USER_SCREEN8:				return(AUX_GPU_USER_SCREEN8); 		
//		case AUX_USER_SCREEN9:				return(AUX_GPU_USER_SCREEN9); 		
//		case AUX_USER_SCREEN10:			return(AUX_GPU_USER_SCREEN10); 		
//		case AUX_USER_SCREEN11:			return(AUX_GPU_USER_SCREEN11); 		


        default: Dprintf("GetGraphicsAddr:: PageNo=%d not found\n", Page_No);
	}
	return(0);
}



void DMA_Config(U32 src, U32 dest, U32 size);


/******************************************************************************
		Function name 			: SetPen
		Function prototype 		: void SetPen(int pen)
		Behavior description	: Color Change
		Input parameter			: 16Bit Color
		Output parameter 		: g_Color
		Return parameter		: None
		Required preconditions	: None
		Called functions		: OnPaint
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void SetPen(int pen)
{
}
/******************************************************************************
		Function name 			: SetColor
		Function prototype 		: void SetBrush(int brush)
		Behavior description	: Color Change
		Input parameter			: 16Bit Color
		Output parameter 		: g_Color
		Return parameter		: None
		Required preconditions	: None
		Called functions		: OnPaint
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void SetBrush(int brush)
{
}

/******************************************************************************
		Function name 			: Plot
		Function prototype 		: void Plot(U16 x, U16 y)
		Behavior description	: 
		Input parameter			: X, Y Position
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

void WinPlot(U16 x, U16 y)
{
	Windows		*win = GetWindow(gp_Graphics->Parent, gp_Graphics->Handle,"WinPlot");
	//int		width, height;
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	volatile U16 	*pScreen = (U16 *)Graphics_Address;
	int		sx = x + gp_Graphics->Offset_X;
	int		sy = y + gp_Graphics->Offset_Y;
	

	if(sx < gp_Graphics->Offset_X + win->client_width && sy < gp_Graphics->Offset_Y + win->client_height)
	{
		position= ((sy * LCD_X)+ sx);
		pScreen += position;
		*pScreen = g_Color;	
	}
}

/******************************************************************************
		Function name 			: Line
		Function prototype 		: void Line(U16 startx, U16 starty, U16 endx, U16 endy)
		Behavior description	: Absolute Screen Line Draw
		Input parameter			: sx, sy, ex, ey
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinLine(U16 startx, U16 starty, U16 endx, U16 endy)
{
    int  t, distance;

    int xerr=0, yerr=0, delta_x, delta_y;
    int incx, incy;

    delta_x = endx-startx;
    delta_y = endy-starty;
    if(delta_x > 0) incx=1;
    else if(delta_x==0) incx=0;
    else incx= -1;

    if(delta_y > 0) incy=1;
    else if(delta_y==0) incy=0;
    else incy= -1;

    delta_x=abs(delta_x);
    delta_y=abs(delta_y);
    if(delta_x > delta_y) distance = delta_x;
    else distance = delta_y;
    for(t=0; t<=distance+1; t++)
    {
        WinPlot(startx, starty);
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr > distance)
        {
            xerr -= distance;
            startx+=incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            starty+=incy;
        }
    }
}

/******************************************************************************
		Function name 			: MoveTo
		Function prototype 		: void MoveTo(U16 x, U16 y);
		Behavior description	: Line Point
		Input parameter			: None 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinLine2DForTo(int sx1, int sy1, int ex2, int ey2);

void WinMoveTo(U16 x, U16 y)
{
	Windows 	*win;
	int		sx = x + gp_Graphics->Offset_X;
	int		sy = y + gp_Graphics->Offset_Y;
//Dprintf("WinMoveTo::sx=%d sy=%d\n", sx, sy);
	win = GetWindow(gp_Graphics->Parent, gp_Graphics->Handle, "WinMoveTo");
	win->LineTo_X = sx;
	win->LineTo_Y = sy;
//Dprintf("WinMoveTo::sx=%d sy=%d	\n", sx, sy);
}
void WinLineTo(U16 endx, U16 endy)
{
	Windows 	*win;
	int		ex = endx + gp_Graphics->Offset_X;
	int		ey = endy + gp_Graphics->Offset_Y;	

	win = GetWindow(gp_Graphics->Parent, gp_Graphics->Handle, "WinLineTo");

	WinLine2DForTo(win->LineTo_X, win->LineTo_Y, ex, ey);
	

	win->LineTo_X = ex;
	win->LineTo_Y = ey;	
//Dprintf("WinLineTo::sx=%d sy=%d\n", ex, ey);
//Dprintf("WinLineTo::sx=%d sy=%d ex=%d ey=%d	\n", win->LineTo_X, win->LineTo_Y, ex, ey);
}

void WinLine2DForTo(int sx1, int sy1, int ex2, int ey2)
{
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	int		x1 = sx1;
	int		y1 = sy1;
	int		x2 = ex2;
	int		y2 = ey2;

    DWORD 	cmd = 0x0000800c;
    int 	dx, dy, k1, k2, et, w;
    DWORD 	quad = 0;
  
    if (x1==x2)
    {
        VLine2D(y1, y2, x1);
        return ;
    }
    else if (y1==y2)
    {
        HLine2D(x1, x2, y1);
        return;
    }
    /* check line x dir */
    if (x1 <= x2)
    {
        dx = x2 - x1;
        quad |= 0x2;
    }
    else
    {
        dx = x1 - x2;
    }
    
    /* check line y dir */
    if (y1 <= y2)
    {
        dy = y2 - y1;
        quad |= 0x1;
    }
    else
    {
        dy = y1 - y2;
    }
    
    if (dx < dy)
    {
        quad |= 0x4;
    }
    
    if (dx == dy) /* diagonal line */
    {
        //WaitForNotBusy();
		Wait2D_Engine();
		
        
        POKE_32(DE_FOREGROUND , g_Color);
        POKE_32(DE_DESTINATION, ((((DWORD) x1)<<16)|(WORD)y1));
        POKE_32(DE_DIMENSION  , ((1L<<16)| (WORD)dx));
        POKE_32(DE_CONTROL    , (cmd | (quad << 24) | 0x80060000));
    }
    else
    {
        if (quad & 0x04)
        {
            k1 = 2 * dx;
            et = k1 - dy;
            k2 = et - dy;
            w = dy + 1;
        } 
        else 
        {
            k1 = 2 * dy;
            et = k1 - dx;
            k2 = et - dx;
            w = dx + 1;
        }
       
        //WaitForNotBusy();
		Wait2D_Engine();
		POKE_32(DE_WINDOW_SOURCE_BASE     , g_GPU_addr);  //init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    	POKE_32(DE_WINDOW_DESTINATION_BASE, g_GPU_addr);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0

        POKE_32(DE_FOREGROUND, g_Color);
        POKE_32(DE_SOURCE, ((((DWORD) k1)<<16)|(WORD)k2));
        POKE_32(DE_DESTINATION, ((((DWORD) x1)<<16)|(WORD)y1));
        POKE_32(DE_DIMENSION  , ((((DWORD) w)<<16)|(WORD)et));
        POKE_32(DE_CONTROL    , (cmd | (quad << 24) | 0x80070000));
    }
}


/******************************************************************************
		Function name 			: LineTo
		Function prototype 		: void LineTo(U16 endx, U16 endy)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinLineTo1(U16 endx1, U16 endy1)
{
    int         startx, starty, t, distance;
    int         xerr=0, yerr=0, delta_x, delta_y;
    int         incx, incy;
	Windows 	*win=NULL;
	int		endx = endx1 + gp_Graphics->Offset_X;
	int		endy = endy1 + gp_Graphics->Offset_Y;
	
    startx = win->LineTo_X;
    starty = win->LineTo_Y;

    delta_x = endx-startx;
    delta_y = endy-starty;
    if(delta_x > 0) incx=1;
    else if(delta_x==0) incx=0;
    else incx= -1;

    if(delta_y > 0) incy=1;
    else if(delta_y==0) incy=0;
    else incy= -1;

    delta_x=abs(delta_x);
    delta_y=abs(delta_y);
    if(delta_x > delta_y) distance = delta_x;
    else distance = delta_y;

    for(t=0; t<=distance+1; t++)
    {
        WinPlot(startx, starty);
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr > distance)
        {
            xerr -= distance;
            startx+=incx;
        }
        if(yerr > distance)
        {
            yerr -= distance;
            starty+=incy;
        }
    }
    win->LineTo_X=endx;
    win->LineTo_Y=endy;
}
/******************************************************************************
		Function name 			: Arc
		Function prototype 		: void Arc(U16 xc, U16 yc, U16 r, U16 start, U16 end)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

#define DELTA   1

void WinArc(U16 xc, U16 yc, U16 r, U16 start, U16 end)
{
    int     x, y, x_old, y_old, theta;
    theta=start;
    x_old = r * cos(theta * 3.141592/180.0) + 0.5;
    y_old = r * sin(theta * 3.141592/180.0) + 0.5;
    for(theta +=DELTA; theta <= end; theta+=DELTA)
    {
        x = r * cos(theta * 3.141592/180.0) + 0.5;
        y = r * sin(theta * 3.141592/180.0) + 0.5;
        WinLine(x_old+xc, y_old+yc, x+xc, y+yc);
        x_old=x;
        y_old=y;
    }
}

/******************************************************************************
		Function name 			: Elipse
		Function prototype 		: void Elipse(U16 xc, U16 yc, U16 a, U16 b);
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinElipse(U16 xc, U16 yc, U16 a, U16 b)
{
    long    h;
    long    a2 = (long)a * a, b2 = (long)b * b;
    long    a82 = 8 * a2, b82 = 8 * b2;
    int     x, y;
    long    deltaE, deltaNE;
    x=0;
    y=b;
    h=4 * b2 + a2 - 4 * a2 * b;
    deltaE = 12 * b2;
    deltaNE = 12 * b2 - 8 * a2 * b + 8 * a2;

    while(b2 * x <= a2 * y)
    {
        WinPlot( x+xc,  y+yc);
        WinPlot( x+xc, -y+yc);
        WinPlot(-x+xc, -y+yc);
        WinPlot(-x+xc,  y+yc);
        if(h < 0)
        {
            h+=deltaE;
            deltaE+= b82;
            deltaNE+=b82;
        }
        else
        {
            h+=deltaNE;
            deltaE+=b82;
            deltaNE+= b82 + a82;
            y--;
        }
        x++;
    }
    x=a;
    y=0;
    h=4 * a2 + a2 - 4 * a * b2;
    deltaE = 12 * a2;
    deltaNE = 12 * a2 - 8 * b2 * a + 8 * b2;
    while(b2 * x > a2 * y)
    {
        WinPlot( x+xc,  y+yc);
        WinPlot( x+xc, -y+yc);
        WinPlot(-x+xc, -y+yc);
        WinPlot(-x+xc,  y+yc);
        if(h < 0)
        {
            h+=deltaE;
            deltaE+= a82;
            deltaNE+=a82;
        }
        else
        {
            h+=deltaNE;
            deltaE+=a82;
            deltaNE+= b82 + a82;
            x--;
        }
        y++;
    }
}

void __HLine(U16 x, U16 y, U16 len)
{
	while(len--)
	{
        WinPlot(x++, y);
	}
    //Plot(x++, y);
}

void __Swap(U16 *a, U16 *b)
{
    int     temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void __Hline(U16 x1, U16 x2, U16 y)
{
    if(x1 > x2) __Swap(&x1, &x2);
	__HLine(x1, y, x2-x1);
}

void __VLine(U16 x, U16 y, U16 len)
{
	while(len--) WinPlot(x, y++);
}
/******************************************************************************
		Function name 			: Rectangle
		Function prototype 		: void Rectangle(U16 sx, U16 sy, U16 xlen, U16 ylen)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinRect(U16 sx, U16 sy, U16 xlen, U16 ylen)
{
    __HLine(sx, sy+ylen, xlen);
    __HLine(sx, sy, xlen);
    __VLine(sx, sy, ylen);
    __VLine(sx+xlen, sy, ylen+1);
}

void WinRect2D(U16 sx, U16 sy, U16 width, U16 height)
{
    WinLine2D(sx, sy, sx+width, sy);	//H1
    WinLine2D(sx, sy+height, sx+width, sy+height);	//H2
    WinLine2D(sx, sy, sx, sy+height);	//V1
	WinLine2D(sx+width, sy, sx+width, sy+height); //V2	
}

void WinRoundRect2D(U16 sx, U16 sy, U16 width, U16 height)
{
	WinArc(sx+10, sy+10, 10, 180, 270);
	WinArc(sx+10, sy+height-10, 10, 90, 180);
	WinArc(sx+width-10, sy+10, 10, 270, 360);
	WinArc(sx+width-10, sy+height-10, 10, 0, 90);

    WinLine2D(sx+10, sy, sx+width-10, sy);	//H1
    WinLine2D(sx+10, sy+height, sx+width-10, sy+height);	//H2
    WinLine2D(sx, sy+10, sx, sy+height-10);	//V1
	WinLine2D(sx+width, sy+10, sx+width, sy+height-10); //V2	

}
/******************************************************************************
		Function name 			: FillElipse
		Function prototype 		: void FillElipse(U16 xc, U16 yc, U16 a, U16 b)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

void WinFillElipse(U16 xc, U16 yc, U16 a, U16 b)
{
    long    h;
    long    a2 = (long)a * a, b2 = (long)b * b;
    long    a82 = 8 * a2, b82 = 8 * b2;
    int     x, y;
    long    deltaE, deltaNE;
    x=0;
    y=b;
    h=4 * b2 + a2 - 4 * a2 * b;
    deltaE = 12 * b2;
    deltaNE = 12 * b2 - 8 * a2 * b + 8 * a2;
    while(b2 * x <= a2 * y)
    {
        __Hline(-x+xc, x+xc, y+yc);
        __Hline(-x+xc, x+xc,-y+yc);
        if(h < 0)
        {
            h+=deltaE;
            deltaE+= b82;
            deltaNE+=b82;
        }
        else
        {
            h+=deltaNE;
            deltaE+=b82;
            deltaNE+= b82 + a82;
            y--;
        }
        x++;
    }
    x=a;
    y=0;
    h=4 * a2 + a2 - 4 * a * b2;
    deltaE = 12 * a2;
    deltaNE = 12 * a2 - 8 * b2 * a + 8 * b2;
    while(b2 * x > a2 * y)
    {
        __Hline(-x+xc, x+xc, y+yc);
        __Hline(-x+xc, x+xc,-y+yc);
        if(h < 0)
        {
            h+=deltaE;
            deltaE+= a82;
            deltaNE+=a82;
        }
        else
        {
            h+=deltaNE;
            deltaE+=a82;
            deltaNE+= b82 + a82;
            x--;
        }
        y++;
    }
}



/******************************************************************************
		Function name 			: VLine2D
		Function prototype 		: void VLine2D(int y1, int y2, int x)
		Behavior description	: 
		Input parameter			: y1, y2:스크린절대좌표, x:스크린절대좌표
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: Windows Program Start
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinVLine2D(int sx, int sy, int height)
{
    WinLine2D(sx, sy, sx, sy+height);	//V1
	//Dprintf("WinVLine2D:: x=%d y1=%d y2=%d len=%d\n", x1, y1, y1+len, len);
}

/******************************************************************************
		Function name 			: HLine2D
		Function prototype 		: void HLine2D(int x1, int x2, int y)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinHLine2D(int sx, int sy, int width)
{
    WinLine2D(sx, sy, sx+width, sy);	//H1
	//Dprintf("WinHLine2D:: y=%d x1=%d x2=%d len=%d\n", y1, x1, x1+len, len);
}

/******************************************************************************
		Function name 			: Line2D
		Function prototype 		: void Line2D(int x1, int y1, int x2, int y2)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinLine2D(int sx1, int sy1, int ex2, int ey2)
{
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	int		x1 = sx1 + gp_Graphics->Offset_X;
	int		y1 = sy1 + gp_Graphics->Offset_Y;
	int		x2 = ex2 + gp_Graphics->Offset_X;
	int		y2 = ey2 + gp_Graphics->Offset_Y;

    DWORD 	cmd = 0x0000800c;
    int 	dx, dy, k1, k2, et, w;
    DWORD 	quad = 0;
  
    if (x1==x2)
    {
        VLine2D(y1, y2, x1);
        return ;
    }
    else if (y1==y2)
    {
        HLine2D(x1, x2, y1);
        return;
    }
    /* check line x dir */
    if (x1 <= x2)
    {
        dx = x2 - x1;
        quad |= 0x2;
    }
    else
    {
        dx = x1 - x2;
    }
    
    /* check line y dir */
    if (y1 <= y2)
    {
        dy = y2 - y1;
        quad |= 0x1;
    }
    else
    {
        dy = y1 - y2;
    }
    
    if (dx < dy)
    {
        quad |= 0x4;
    }
    
    if (dx == dy) /* diagonal line */
    {
        //WaitForNotBusy();
		Wait2D_Engine();		
        
        POKE_32(DE_FOREGROUND , g_Color);
        POKE_32(DE_DESTINATION, ((((DWORD) x1)<<16)|(WORD)y1));
        POKE_32(DE_DIMENSION  , ((1L<<16)| (WORD)dx));
        POKE_32(DE_CONTROL    , (cmd | (quad << 24) | 0x80060000));
    }
    else
    {
        if (quad & 0x04)
        {
            k1 = 2 * dx;
            et = k1 - dy;
            k2 = et - dy;
            w = dy + 1;
        } 
        else 
        {
            k1 = 2 * dy;
            et = k1 - dx;
            k2 = et - dx;
            w = dx + 1;
        }
       
        //WaitForNotBusy();
		Wait2D_Engine();
		
		POKE_32(DE_WINDOW_SOURCE_BASE     , g_GPU_addr);  //init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    	POKE_32(DE_WINDOW_DESTINATION_BASE, g_GPU_addr);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0

        POKE_32(DE_FOREGROUND, g_Color);
        POKE_32(DE_SOURCE, ((((DWORD) k1)<<16)|(WORD)k2));
        POKE_32(DE_DESTINATION, ((((DWORD) x1)<<16)|(WORD)y1));
        POKE_32(DE_DIMENSION  , ((((DWORD) w)<<16)|(WORD)et));
        POKE_32(DE_CONTROL    , (cmd | (quad << 24) | 0x80070000));
    }
}

/******************************************************************************
		Function name 			: FillRect2D
		Function prototype 		: void FillRect2D(int xs, int ys, int xe, int ye)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WinFillRect2D(int sx, int sy, int width, int height)
{
	U32		data;
	u16		rop = 0x8000 | 0x0c;	//0x0c:ROP2    
	//U32		addr;

	int		xs = sx + gp_Graphics->Offset_X;
	int		ys = sy + gp_Graphics->Offset_Y;
//Dprintf("\tWinFillRect2D:: off_x=%d off_y=%d\n",gp_Graphics->Offset_X, gp_Graphics->Offset_Y 	);
	Wait2D_Engine();

	POKE_32(DE_WINDOW_SOURCE_BASE     , g_GPU_addr);  //init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_WINDOW_DESTINATION_BASE, g_GPU_addr);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_FOREGROUND , g_Color);

	data = (xs << 16) | ys;
    //POKE_32(DE_SOURCE, data);
    POKE_32(DE_DESTINATION, data);

	data = (width << 16) | height;
    POKE_32(DE_DIMENSION  , data);

    //POKE_32(DE_PITCH, LCD_X << 16 | LCD_X);
    //POKE_32(DE_COLOR_COMPARE, RED);
    //POKE_32(DE_COLOR_COMPARE_MASK, 0xffffffff);	//1이면 해당되는 비트는 비교하게된다.
    POKE_32(DE_CONTROL, 0x80030000 | rop);
//Dprintf("WinFillRect2D::End\n");
    //WaitForNotBusy();
}

/*
void WinFillRect2D(int sx, int sy, int ex, int ey)
{
	U32		addr;
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	int		xs = sx + gp_Graphics->Offset_X;
	int		ys = sy + gp_Graphics->Offset_Y;
	int		xe = ex + gp_Graphics->Offset_X;
	int		ye = ey + gp_Graphics->Offset_Y;
    DWORD 	cmd, dx, dy;
    
    cmd = 0x8003;	//사다리꼴을 채우는 명령인데 FillRect에 문제가 생겨서 이걸로 대치한다.
    if (xe < xs)
    {
        dx = xs - xe + 1;
        xs = xe;
    }
    else
    {
        dx = xe - xs + 1;
    }
    
    if (ye < ys)
    {
        dy = ys - ye + 1;
        ys = ye;
    }
    else
    {
        dy = ye - ys + 1;
    }
    
    WaitForNotBusy();
	if(gp_Graphics->Parent = NULL && gp_Graphics->Handle==1)
		addr = GPU_CONTAINER_WINDOW1;
    
	POKE_32(DE_WINDOW_SOURCE_BASE     , g_GPU_addr);  //init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_WINDOW_DESTINATION_BASE, g_GPU_addr);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_FOREGROUND , g_Color);
    POKE_32(DE_DESTINATION, (((ULONG) xs) <<16) | (ULONG)ys);
    POKE_32(DE_DIMENSION  , dx << 16 | dy);
    POKE_32(DE_CONTROL    , cmd << 16| 0x800c);
    //dump_2Dregs();
}
*/


/******************************************************************************
		Function name 			: PaintBitBlt
		Function prototype 		: void BitBlt(U32 src, U32 dest, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy);
		Behavior description	: 화면의 특정영역을 카피하여 복사하는 기능을한다.
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)
{
	//Windows	*window;
	HANDLE	parent;
	//U32		src, dest;
	int		dest=0;
	int		dx = dx1 + gp_Graphics->Offset_X;
	int		dy = dy1 + gp_Graphics->Offset_Y;
	
//Dprintf("PaintBitBlt::srcPage=%d sx=%d sy=%d dx1=%d dy1=%d xlen=%d ylen=%d\n", page, sx, sy, dx1, dy1, dimx, dimy);	
	//SetClip(1, dx, dy, dimx-dx1, dimy-dy1, 1);
	//src = GetGraphicsAddr(page);
	if(gp_Graphics->Parent==NULL)	//Container Window
	{
		parent = gp_Graphics->Handle;
		switch(parent)
		{
			case 1: dest = CONTAINER_WINDOW1_SCREEN; break;
			case 2: dest = CONTAINER_WINDOW2_SCREEN; break;
			case 3: dest = CONTAINER_WINDOW3_SCREEN; break;
			case 4: dest = CONTAINER_WINDOW4_SCREEN; break;
			case 5: dest = CONTAINER_WINDOW5_SCREEN; break;
			case 6: dest = LCD_ALPHA_CONTAINER_SCREEN; break;
//			case 7: dest = VIDEO_ALPHA_CONTAINER_SCREEN; break;
			default: Dprintf("PaintBitBlt:: parent handle not found\n"); break;
		}
	}
	else 	//Widget Window
	{
		parent = gp_Graphics->Parent;
		switch(parent)
		{
			case 1: dest = CONTAINER_WIDGET1_SCREEN; break;
			case 2: dest = CONTAINER_WIDGET2_SCREEN; break;
			case 3: dest = CONTAINER_WIDGET3_SCREEN; break;
			case 4: dest = CONTAINER_WIDGET4_SCREEN; break;
			case 5: dest = CONTAINER_WIDGET5_SCREEN; break;
			case 6: dest = LCD_ALPHA_WIDGET_SCREEN; break;
//			case 7: dest = VIDEO_ALPHA_WIDGET_SCREEN; break;
			default: Dprintf("PaintBitBlt:: parent handle not found\n"); break;
		}
	}
//Dprintf("PaintBitBlt::page=%d dest=%d sx=%d sy=%d dx=%d dy=%d wid=%d height=%d\n" , page, dest, sx, sy, dx, dy, dimx, dimy);
	BitBlt(page, dest, sx, sy, dx, dy, dimx, dimy);
    //WaitForNotBusy();
	//SetClip(0, dx, dy, dimx-dx1, dimy-dy1, 1);	
} 

//page의 sx, sy로 시작하는 좌표의 dimx, dimy의 사이즈대로 현재윈도우의 dx1, dy1으로 Bitblt한다.

void FontBitBlt(int page, int CompPage, int ref_page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)
{
	//Windows	*window;
	HANDLE	parent;
	//U32		src, dest;
	int		dest=0;
	int		dx = dx1 + gp_Graphics->Offset_X;
	int		dy = dy1 + gp_Graphics->Offset_Y;
	
//Dprintf("PaintBitBlt::srcPage=%d sx=%d sy=%d dx1=%d dy1=%d xlen=%d ylen=%d\n", page, sx, sy, dx1, dy1, dimx, dimy);	
	//SetClip(1, dx, dy, dimx-dx1, dimy-dy1, 1);
	//src = GetGraphicsAddr(page);
	if(gp_Graphics->Parent==NULL)	//Container Window
	{
		parent = gp_Graphics->Handle;
		switch(parent)
		{
			case 1: dest = CONTAINER_WINDOW1_SCREEN; break;
			case 2: dest = CONTAINER_WINDOW2_SCREEN; break;
			case 3: dest = CONTAINER_WINDOW3_SCREEN; break;
			case 4: dest = CONTAINER_WINDOW4_SCREEN; break;
			case 5: dest = CONTAINER_WINDOW5_SCREEN; break;
			case 6: dest = LCD_ALPHA_CONTAINER_SCREEN; break;
//			case 7: dest = VIDEO_ALPHA_CONTAINER_SCREEN; break;
			default: Dprintf("PaintBitBlt:: parent handle not found\n"); break;
		}
	}
	else 	//Widget Window
	{
		parent = gp_Graphics->Parent;
		switch(parent)
		{
			case 1: dest = CONTAINER_WIDGET1_SCREEN; break;
			case 2: dest = CONTAINER_WIDGET2_SCREEN; break;
			case 3: dest = CONTAINER_WIDGET3_SCREEN; break;
			case 4: dest = CONTAINER_WIDGET4_SCREEN; break;
			case 5: dest = CONTAINER_WIDGET5_SCREEN; break;
			case 6: dest = LCD_ALPHA_WIDGET_SCREEN; break;
//			case 7: dest = VIDEO_ALPHA_WIDGET_SCREEN; break;
			default: Dprintf("PaintBitBlt:: parent handle not found\n"); break;
		}
	}
    //Dprintf("FontBitBlt::page=%d Comppage=%d dest=%d sx=%d sy=%d dx=%d dy=%d wid=%d height=%d\n" , page, CompPage, dest, sx, sy, dx, dy, dimx, dimy);

    //현재윈도우의 글자(바탕)를 쓸영역을 지정된 페이지에 복사한다.
    BitBlt(ref_page, CompPage, dx, dy, 0, 0, dimx, dimy);    
    //page 의 sx,sy 로시작하는 좌표의 dimx, dimy의 사이즈를 dest의 dx, dy로 Bitblt한다.
    //즉폰트를 합성한다.
    BitBlt16(page, CompPage, sx, sy, 0, 0, dimx, dimy);
    //page 의 sx,sy 로시작하는 좌표의 dimx, dimy의 사이즈를 dest의 dx, dy로 Bitblt한다.
    BitBlt(CompPage, dest, 0, 0, dx, dy, dimx, dimy);    

    //BitBlt(page, dest, sx, sy, dx, dy, dimx, dimy);

    //WaitForNotBusy();
	//SetClip(0, dx, dy, dimx-dx1, dimy-dy1, 1);	
} 

void FontBitBltOne(int page, int CompPage, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy)
{
	//Windows	*window;
	HANDLE	parent;
	//U32		src, dest;
	int		dest=0;
	int		dx = dx1 + gp_Graphics->Offset_X;
	int		dy = dy1 + gp_Graphics->Offset_Y;
	
//Dprintf("PaintBitBlt::srcPage=%d sx=%d sy=%d dx1=%d dy1=%d xlen=%d ylen=%d\n", page, sx, sy, dx1, dy1, dimx, dimy);	
	//SetClip(1, dx, dy, dimx-dx1, dimy-dy1, 1);
	//src = GetGraphicsAddr(page);
	if(gp_Graphics->Parent==NULL)	//Container Window
	{
		parent = gp_Graphics->Handle;
		switch(parent)
		{
			case 1: dest = CONTAINER_WINDOW1_SCREEN; break;
			case 2: dest = CONTAINER_WINDOW2_SCREEN; break;
			case 3: dest = CONTAINER_WINDOW3_SCREEN; break;
			case 4: dest = CONTAINER_WINDOW4_SCREEN; break;
			case 5: dest = CONTAINER_WINDOW5_SCREEN; break;
#ifdef ALPHA_USE
			case 6: dest = LCD_ALPHA_CONTAINER_SCREEN; break;
//			case 7: dest = VIDEO_ALPHA_CONTAINER_SCREEN; break;
#endif
			default: Dprintf("PaintBitBlt:: parent handle not found\n"); break;
		}
	}
	else 	//Widget Window
	{
		parent = gp_Graphics->Parent;
		switch(parent)
		{
			case 1: dest = CONTAINER_WIDGET1_SCREEN; break;
			case 2: dest = CONTAINER_WIDGET2_SCREEN; break;
			case 3: dest = CONTAINER_WIDGET3_SCREEN; break;
			case 4: dest = CONTAINER_WIDGET4_SCREEN; break;
			case 5: dest = CONTAINER_WIDGET5_SCREEN; break;
#ifdef ALPHA_USE
			case 6: dest = LCD_ALPHA_WIDGET_SCREEN; break;
//			case 7: dest = VIDEO_ALPHA_WIDGET_SCREEN; break;
#endif
			default: Dprintf("PaintBitBlt:: parent handle not found\n"); break;
		}
	}
    //Dprintf("FontBitBlt::page=%d Comppage=%d dest=%d sx=%d sy=%d dx=%d dy=%d wid=%d height=%d\n" , page, CompPage, dest, sx, sy, dx, dy, dimx, dimy);

    //현재윈도우의 글자(바탕)를 쓸영역을 지정된 페이지에 복사한다.
    //BitBlt(dest, CompPage, dx, dy, 0, 0, dimx, dimy);    
    //page 의 sx,sy 로시작하는 좌표의 dimx, dimy의 사이즈를 dest의 dx, dy로 Bitblt한다.
    //즉폰트를 합성한다.
    BitBlt16(page, dest, sx, sy, dx, dy, dimx, dimy);
    //page 의 sx,sy 로시작하는 좌표의 dimx, dimy의 사이즈를 dest의 dx, dy로 Bitblt한다.
    //BitBlt(CompPage, dest, 0, 0, dx, dy, dimx, dimy);    

    //BitBlt(page, dest, sx, sy, dx, dy, dimx, dimy);

    //WaitForNotBusy();
	//SetClip(0, dx, dy, dimx-dx1, dimy-dy1, 1);	
} 

/*
mode::
#define	ROP2			1
#define	ROP3			2
// Raster Op 2 functions 
#define ROP2_XOR        0x06
#define ROP2_AND        0x08
#define ROP2_COPY       0x0C
#define ROP2_OR         0x0E
rop::
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
*/
void PaintBitBltRop(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy, int mode, U8 rop)
{
	//Windows	*window;
	HANDLE	parent;
	//U32		src, dest;
	int		dest=0;
	int		dx = dx1 + gp_Graphics->Offset_X;
	int		dy = dy1 + gp_Graphics->Offset_Y;
	
//Dprintf("PaintBitBlt::srcPage=%d sx=%d sy=%d dx1=%d dy1=%d xlen=%d ylen=%d\n", page, sx, sy, dx1, dy1, dimx, dimy);	
	//SetClip(1, dx, dy, dimx-dx1, dimy-dy1, 1);
	//src = GetGraphicsAddr(page);
	if(gp_Graphics->Parent==NULL)	//Container Window
	{
		parent = gp_Graphics->Handle;
		switch(parent)
		{
			case 1: dest = CONTAINER_WINDOW1_SCREEN; break;
			case 2: dest = CONTAINER_WINDOW2_SCREEN; break;
			case 3: dest = CONTAINER_WINDOW3_SCREEN; break;
			case 4: dest = CONTAINER_WINDOW4_SCREEN; break;
			case 5: dest = CONTAINER_WINDOW5_SCREEN; break;
			case 6: dest = LCD_ALPHA_CONTAINER_SCREEN; break;
//			case 7: dest = VIDEO_ALPHA_CONTAINER_SCREEN; break;			
            default: Dprintf("PaintBitBlt:: parent handle not found\n"); break;
		}
	}
	else 	//Widget Window
	{
		parent = gp_Graphics->Parent;
		switch(parent)
		{
			case 1: dest = CONTAINER_WIDGET1_SCREEN; break;
			case 2: dest = CONTAINER_WIDGET1_SCREEN; break;
			case 3: dest = CONTAINER_WIDGET1_SCREEN; break;
			case 4: dest = CONTAINER_WIDGET1_SCREEN; break;
			case 5: dest = CONTAINER_WIDGET1_SCREEN; break;
			case 6: dest = LCD_ALPHA_WIDGET_SCREEN; break;
//			case 7: dest = VIDEO_ALPHA_WIDGET_SCREEN; break;            
			default: Dprintf("PaintBitBlt:: parent handle not found\n"); break;
		}
	}
	BitBltRop(page, dest, sx, sy, dx, dy, dimx, dimy, mode, rop);
    //WaitForNotBusy();
	//SetClip(0, dx, dy, dimx-dx1, dimy-dy1, 1);	
} 

void ScreenClear(int page)
{
	int		temp_x = gp_Graphics->Offset_X;
	int		temp_y = gp_Graphics->Offset_Y;
	U16		color = g_Color;
	U32		addr = GetGraphicsAddr(page);
	U32		temp_addr = g_GPU_addr;
	
	g_GPU_addr = addr;
//Dprintf("ScreenClear:: addr=%08x \n", addr);
	
	g_Color = 0x0000;
	WinFillRect2D(0, 0, LCD_X-1, LCD_Y-1);
	gp_Graphics->Offset_X = temp_x;
	gp_Graphics->Offset_Y = temp_y;
	g_Color = color;
	g_GPU_addr = temp_addr;	
}

void WinBmpPlot(U16 sx, U16 sy, U16 data, Windows *win)
{
	//Windows		*win;
	//int		width, height;
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	volatile U16 	*pScreen = (U16 *)Graphics_Address;
	
	//win = GetWindow(gp_Graphics->Parent, gp_Graphics->Handle);

	if(sx < gp_Graphics->Offset_X + win->client_width && sy < gp_Graphics->Offset_Y + win->client_height)
	{
		position= ((sy * LCD_X)+ sx);
		pScreen += position;
		*pScreen = data;	
	}
}

int PaintBmpDisplay(U16 sx, U16 sy, char *filename)
{
	int		start_block;
	int		end_block;
	int		y, j, i;
	int		xs, ys, gx, gy;
	int		page;
    int     b_size, read_cnt=0;
    
	U32		addr;
	Windows *win = GetWindow(gp_Graphics->Parent, gp_Graphics->Handle, "PaintBmpDisplay");	
	st_NandFAT NandFat;
	
	U8	*VRAM_FileBuf = (U8 *)SOURCE_BMP_ADDR;

	if(SearchNandFile(filename, &NandFat))
	{
		xs = NandFat.x_size;
		ys = NandFat.y_size;
        b_size = (xs * ys * 2) + 4;   //4는 사이즈정보
        
        if(xs ==0 || ys == 0)
        {
            Dprintf("PaintBmpDisplay::%s File Size is Zero Error\n", filename);   
            return(0);            
        }            
		start_block = NandFat.start_blk;
		end_block = NandFat.end_blk;
        read_cnt=0;
		for(i=start_block; i<=end_block; i++)
		{
			for(page=0; page <64; page++)
			{
				BSP_NandReadpage(start_block, page, (U8 *)FlashPageBuf);
				for(y=0; y<2048; y++)
				{	
					*VRAM_FileBuf++ = FlashPageBuf[y]; 
                    read_cnt++;
                    if(read_cnt == b_size) goto again1;
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
again1:
		//SetGraphicsPage(graphics_page);
		gx = sx + gp_Graphics->Offset_X;
		gy = sy + gp_Graphics->Offset_Y;
		if(gy > 0) gy--;
//Dprintf("PaintBmpDisplay::xs=%d ys=%d gx=%d gy=%d\n", xs, ys, gx, gy);		
		addr = SOURCE_BMP_ADDR;
        
        xs =  *(U16 *)addr; addr+=2;
        ys =  *(U16 *)addr; addr+=2;

        //SM718_LCDScreenChange(CONTAINER_WINDOW1_SCREEN);
        
		for(i=ys-1; i>= 0; i--)
		{
			for(j=0; j < xs; j++)
			{
				WinBmpPlot(gx+j, gy+i, *(U16 *)addr, win);				
				addr+=2;
			}
		}
        return(1);
	}
	else 
	{
		Dprintf("PaintBmpDisplay::%s File not Found\n", filename);
        return(0);
	}
}


	//win = GetWindow(gp_Graphics->Parent, gp_Graphics->Handle);

int DMABmpDisplay(U16 sx, U16 sy, char *filename)
{
	int		start_block;
	int		end_block;
	int		y,  i;
	int		xs, ys, gx, gy;
	int		page;
    int     b_size, read_cnt=0;
    int     position, lcd_x, xs_addr;
	//U16 	*pScreen = (U16 *)Graphics_Address;
    U32     g_addr=Graphics_Address;
		
	U32		addr;
	Windows *win = GetWindow(gp_Graphics->Parent, gp_Graphics->Handle, "PaintBmpDisplay");	
	st_NandFAT NandFat;
	
	U8	*VRAM_FileBuf = (U8 *)SOURCE_BMP_ADDR;
//Dprintf("DMABmpDisplay:: Entry\n");
	if(SearchNandFile(filename, &NandFat))
	{
		xs = NandFat.x_size;
		ys = NandFat.y_size;
        b_size = (xs * ys * 2) + 4;   //4는 사이즈정보
//Dprintf("xs=%d ys=%d\n", xs, ys);        
        if(xs ==0 || ys == 0)
        {
            Dprintf("DMABmpDisplay::%s File Size is Zero Error\n", filename);   
            return(0);            
        }            
		start_block = NandFat.start_blk;
		end_block = NandFat.end_blk;
        read_cnt=0;
		for(i=start_block; i<=end_block; i++)
		{
			for(page=0; page <64; page++)
			{
				BSP_NandReadpage(start_block, page, (U8 *)FlashPageBuf);
				for(y=0; y<2048; y++)
				{	
					*VRAM_FileBuf++ = FlashPageBuf[y]; 
                    read_cnt++;
                    if(read_cnt == b_size) goto again;
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
                //그럴일은 없겠지만 여기서 무한루프에대한 대책을 세워야한다.
			}
		}		
again:
		//SetGraphicsPage(graphics_page);
		gx = sx + gp_Graphics->Offset_X;
		gy = sy + gp_Graphics->Offset_Y;
		
        if(gy > 0) gy--;
//Dprintf("PaintBmpDisplay::xs=%d ys=%d gx=%d gy=%d\n", xs, ys, gx, gy);		
		addr = SOURCE_BMP_ADDR;
        
        xs =  *(U16 *)addr; addr+=2;
        ys =  *(U16 *)addr; addr+=2;
        
//Dprintf("DMABmpDisplay::xsize=%d ysize=%d sx=%d sy=%d gx=%d gy=%d Graphics_Address=%08x\n", xs, ys, sx, sy, gx, gy, Graphics_Address);
        xs_addr = xs * 2;
        lcd_x = LCD_X * 2;
        
        //ys-=1; //이값이 첫라인을 아그릴수도 있다.
        
        //SM718_LCDScreenChange(CONTAINER_WINDOW1_SCREEN);
		for(i=ys-1; i>= 0; i--)
		{
            position= (((gy+i) * lcd_x)+ (gx*2));
            g_addr = Graphics_Address + position;
            //MemoryDMA(addr, g_addr, xs); 
            DMA_Config(addr, g_addr, xs);
            while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);             
//Dprintf("i=%d position=%d addr=%08x g_addr=%08x\n", i, position, addr, g_addr);
            addr = addr + xs_addr;
            //Dgetch();
            //Delay(1000);
		}
        return(1);
	}
	else 
	{
		Dprintf("PaintBmpDisplay::%s File not Found\n", filename);
        return(0);
	}
}


void IconDisplay(U16 sx, U16 sy, char *filename, U16 pass_color)
{
	int		start_block;
	int		end_block;
	int		y, j, i;
	int		xs, ys, gx, gy;
	int		page;
	U16		data;

	U32		addr;
	Windows *win = GetWindow(gp_Graphics->Parent, gp_Graphics->Handle, "");	
	
	st_NandFAT NandFat;
	
	U8	*VRAM_FileBuf = (U8 *)SOURCE_BMP_ADDR;

	if(SearchNandFile(filename, &NandFat))
	{
		xs = NandFat.x_size;
		ys = NandFat.y_size;
		start_block = NandFat.start_blk;
		end_block = NandFat.end_blk;

		for(i=start_block; i<=end_block; i++)
		{
			for(page=0; page <64; page++)
			{
				BSP_NandReadpage(start_block, page, (U8 *)FlashPageBuf);
				for(y=0; y<2048; y++)
				{	
					*VRAM_FileBuf++ = FlashPageBuf[y]; 
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
		//SetGraphicsPage(graphics_page);
		gx = sx + gp_Graphics->Offset_X;
		gy = sy + gp_Graphics->Offset_Y;		
		addr = SOURCE_BMP_ADDR;

        xs =  *(U16 *)addr; addr+=2;
        ys =  *(U16 *)addr; addr+=2;        
        
		for(i=ys-1; i>= 0; i--)
		{
			for(j=0; j < xs; j++)
			{
				data = *(U16 *)addr;
				if( data != pass_color)
				{
					WinBmpPlot(gx+j, gy+i, data, win);
				}
				addr+=2;
			}
		}
	}
	else 
	{
		Dprintf("BmpLoad::%s File not Found\n", filename);
	}
}


void GetImage(char *filename, BitmapSize *bmp)
{
	int		start_block; 
    int     xs, ys, y;    
	st_NandFAT NandFat;
    U32     addr;

  	U8	*VRAM_FileBuf = (U8 *)SOURCE_BMP_ADDR;

	
	if(SearchNandFile(filename, &NandFat))
	{
		start_block = NandFat.start_blk;

        BSP_NandReadpage(start_block, 0, (U8 *)FlashPageBuf);
        for(y=0; y<2048; y++)
        {	
            *VRAM_FileBuf++ = FlashPageBuf[y]; 
        }
		addr = SOURCE_BMP_ADDR;

        xs =  *(U16 *)addr; addr+=2;
        ys =  *(U16 *)addr; addr+=2;        

		bmp->width = xs;
		bmp->height = ys;        
	}
	else 
	{
		Dprintf("GetImage::%s File not Found\n", filename);
	}
}
#ifdef __cplusplus
}
#endif
