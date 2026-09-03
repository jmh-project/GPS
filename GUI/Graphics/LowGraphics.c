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
#include "WindowsGraphics.h"
#include "MemoryManager.h"

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
extern volatile U32	Graphics_Address;
extern volatile U32	position;
extern volatile int	gi_ScreenX, gi_ScreenY;
extern volatile U16 	g_Color;
extern volatile U32	g_GPU_addr;
extern 	volatile U8		FlashPageBuf[2048];	//Nand Flash Memory의 페이지를 조작하기위한 버퍼, BootFile.c
volatile uint8_t gROP2=12;
int IsBadBlock(int block);
void DMA_Config(U32 src, U32 dest, U32 size);
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

void BitBltROP2(int mode)
{
    
}

void Plot(U16 sx, U16 sy)
{
	volatile U16 	*pScreen= (U16 *)Graphics_Address;

    position= ((sy * LCD_X)+ sx);
    pScreen += position;
    *pScreen = g_Color;	
}

/******************************************************************************
		Function name 			: BmpPlot
		Function prototype 		: void BmpPlot(U16 x, U16 y, U16 color)
		Behavior description	: For Test(BootLoader)
		Input parameter			: X, Y Position, Color
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void BmpPlot(U16 x, U16 y, U16 color)
{
	volatile U16 	*pScreen= (volatile U16 *)Graphics_Address;
    position= ((y * LCD_X)+ x);
    pScreen += position;
    *pScreen = color;	
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
void Line(U16 startx, U16 starty, U16 endx, U16 endy)
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
        Plot(startx, starty);
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
void MoveTo(U16 x, U16 y)
{
	gi_ScreenX=x;
	gi_ScreenY=y;
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
void LineTo(U16 endx, U16 endy)
{
    int         startx, starty, t, distance;
    int         xerr=0, yerr=0, delta_x, delta_y;
    int         incx, incy;

    startx = gi_ScreenX;
    starty = gi_ScreenY;

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
        Plot(startx, starty);
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
    gi_ScreenX=endx;
    gi_ScreenY=endy;
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

#define DELTA   5
void Arc(U16 xc, U16 yc, U16 r, U16 start, U16 end)
{
    int     x, y, x_old, y_old, theta;
    theta=start;
    x_old = r * cos(theta * 3.141592/180.0) + 0.5;
    y_old = r * sin(theta * 3.141592/180.0) + 0.5;
    for(theta +=DELTA; theta <= end; theta+=DELTA)
    {
        x = r * cos(theta * 3.141592/180.0) + 0.5;
        y = r * sin(theta * 3.141592/180.0) + 0.5;
        Line(x_old+xc, y_old+yc, x+xc, y+yc);
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
void Elipse(U16 xc, U16 yc, U16 a, U16 b)
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
        Plot( x+xc,  y+yc);
        Plot( x+xc, -y+yc);
        Plot(-x+xc, -y+yc);
        Plot(-x+xc,  y+yc);
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
        Plot( x+xc,  y+yc);
        Plot( x+xc, -y+yc);
        Plot(-x+xc, -y+yc);
        Plot(-x+xc,  y+yc);
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
void Rectangle(U16 sx, U16 sy, U16 xlen, U16 ylen)
{
    HLine(sx, sy+ylen, xlen);
    HLine(sx, sy, xlen);
    VLine(sx, sy, ylen);
    VLine(sx+xlen, sy, ylen+1);
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
void FillElipse(U16 xc, U16 yc, U16 a, U16 b)
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
        Hline(-x+xc, x+xc, y+yc);
        Hline(-x+xc, x+xc,-y+yc);
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
        Hline(-x+xc, x+xc, y+yc);
        Hline(-x+xc, x+xc,-y+yc);
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
		Function name 			: FillRect
		Function prototype 		: void FillRect(U16 sx, U16 sy, U16 xlen, U16 ylen);
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
void FillRect(U16 sx, U16 sy, U16 xlen, U16 ylen)
{
    int 	i;

    for(i=sy+ylen; i >= sy; i--)
    {
        HLine(sx, i, xlen);
    }
}

/******************************************************************************
		Function name 			: HLine
		Function prototype 		: void HLine(U16 x, U16 y, U16 len)
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
void HLine(U16 x, U16 y, U16 len)
{
	while(len--)
	{
        Plot(x++, y);
	}
    //Plot(x++, y);
}

/******************************************************************************
		Function name 			: Swap
		Function prototype 		: void Swap(U16 *a, U16 *b)
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
void Swap(U16 *a, U16 *b)
{
    int     temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

/******************************************************************************
		Function name 			: Hline
		Function prototype 		: void Hline(U16 x1, U16 x2, U16 y)
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
void Hline(U16 x1, U16 x2, U16 y)
{
    if(x1 > x2) Swap(&x1, &x2);
	HLine(x1, y, x2-x1);
}

/******************************************************************************
		Function name 			: VLine
		Function prototype 		: void VLine(U16 x, U16 y, U16 len)
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
void VLine(U16 x, U16 y, U16 len)
{
	while(len--) Plot(x, y++);
}

/******************************************************************************
		Function name 			: CrossLine
		Function prototype 		: void CrossLine(U16 x, U16 y, int len)
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
void CrossLine(U16 x, U16 y, U16 len)
{
	int	harf = len / 2;

	HLine(x-harf, y, len);
	VLine(x, y-harf, len);	
}


/******************************************************************************
		Function name 			: SM718Reset
		Function prototype 		: void SM718Reset(void)
		Behavior description	: 
								 * Reset 2D engine by 
								 * 1) Aborting the current 2D operation.
								 * 2) Re-enable 2D engine to normal state.
		Input parameter			: None 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

void SM718Reset(void)
{
    U32 	sysCtrl;

    /* Abort current 2D operation */
    sysCtrl = SYSTEM_CONTROL;
    sysCtrl |= 0x00002000;	//D13:Abort=1
    SYSTEM_CONTROL = sysCtrl;

    /* Re-enable 2D engine to normal state */
    sysCtrl = SYSTEM_CONTROL;
    sysCtrl &= ~0x00002000;	//D13:Abort=1
    SYSTEM_CONTROL = sysCtrl;
}



/******************************************************************************
		Function name 			: WaitForNotBusy
		Function prototype 		: void WaitForNotBusy(void)
		Behavior description	: 
					 * Wait until 2D engine is not busy.
					 * All 2D operations are recommand to check 2D engine idle before start.
					 *
					 * Return: 0 = return because engine is idle and normal.
					 *        -1 = return because time out (2D engine may have problem).
		Input parameter			: None 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void WaitForNotBusy(void)
{
	U32		addr = (VGX_MMIO_ARENA+DE_CONTROL+2);
	U16		data;
    U32 	i = 0x1000000;
    //U32 	i = 0x10;
	while(i--)
	{
		data = *(U16 *)addr;
		if(data & 0x8000) {  }
		else break;
	}
    gv.sm718WaitMessage=4;
}

void WaitFor2DBusy(void)
{
    U32 	i = 0x1000000;
	while(i--)
	{
		if(REG_DE_CONTROL & 0x80000000) {  }
		else break;
	}
    gv.sm718WaitMessage=5;
}

//#define VGX_MMIO_ARENA	0x6be00000
//#define DE_CONTROL            0x10000C

void Wait2D_Engine(void)
{	
	/*
	U32		dwVal;
    while (1)
    {
        dwVal = SYSTEM_CONTROL;
        {
			if((dwVal & 0x00F00000) == 0x00A00000) break;
        }
    }
	*/
	U32		addr = (VGX_MMIO_ARENA+DE_CONTROL+2);
	U16		data;
    U32 	i = 0x1000000;
    int     status=0;
	while(i--)
	{
		data = *(U16 *)addr;
		if(data & 0x8000) {  }
		else {
            status=1;        
            break;
        }
	}
    if(status==0) {
        gv.sm718WaitMessage=1;    
    }
}

long deWaitForNotBusy(void)
{
    U32 	i = 0x1000000;
	U32		dwVal;
    while (i--)
    {
        dwVal = SYSTEM_CONTROL;
        {
			if((dwVal & 0x00F00000) == 0x00A00000)
            return 0; /* Return because engine idle */
        }
    }
    gv.sm718WaitMessage=2;
    return -1; /* Return because time out */
}

int SM718_2D_Engine_Wait(void)
{
    U32 	i = 0x1000000;
	U32		dwVal;
    
    while (i--)
    {
        dwVal = SYSTEM_CONTROL;
        {
			if((dwVal & 0x00F00000) == 0x00A00000)
            return 1; /* Return because engine idle */
        }
    }
    gv.sm718WaitMessage=3;
    return 0; /* Return because time out */
}

/******************************************************************************
		Function name 			: Init2D
		Function prototype 		: void Init2D(void)
		Behavior description	: 
		Input parameter			: None 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

void Init2D(void)
{
    POKE_32(DE_PITCH, LCD_X << 16| LCD_X);
    POKE_32(DE_FOREGROUND, 0x00ffffff);
    POKE_32(DE_BACKGROUND, 0x00000000);
    POKE_32(DE_STRETCH_FORMAT, ( 16 / 8-1 )<<20 | 0x30003);
    POKE_32(DE_COLOR_COMPARE, 0x12345678);
//    POKE_32(DE_COLOR_COMPARE_MASK, 0x0);
    POKE_32(DE_COLOR_COMPARE_MASK, 0xffffffff);
    POKE_32(DE_MASKS, 0xffffffff);
    POKE_32(DE_CLIP_TL, 0x00000000);
    POKE_32(DE_CLIP_BR, ((LCD_X-1)<<16) | (LCD_Y-1));
    POKE_32(DE_MONO_PATTERN_LOW, 0xFFFFFFFF);
    POKE_32(DE_MONO_PATTERN_HIGH, 0xFFFFFFFF);
    POKE_32(DE_WINDOW_WIDTH, LCD_X<<16|LCD_X);	// screen width ==> 640
    POKE_32(DE_WINDOW_SOURCE_BASE, 0);	// Source Addr
    POKE_32(DE_WINDOW_DESTINATION_BASE, 0);	// Source Addr
}

void SM718_2DInit(void)
{
	DWORD 	command;
	//U16		hi, lo;
	//1.2DEngine Clock (0x40)을 Enable 시킨다.(이작업은 SM718Init()에서 실행한다)

	//2.   Drawing Engine을 취소 시킨후 다시 Normal Mode 로 둔다.
	//2-1. Just be sure no left-over operations from other applications 
	SM718Reset();

	//3. Drawing engine bus and pixel mask, always want to enable.
	REG_DE_MASKS = 0xffffffff;

    /* Pixel format, which can be 8, 16 or 32.
       Assuming setmode is call before 2D init, then pixel format
       is available in reg 0x80000 (Panel Display Control)
    */
	command =  	(0 << 30) | // Pattern XY Mode Select
				(0 << 27) | // Pattern Yvalue=0;
				(0 << 23) | // Pattern Xvalue=0;
				(1 << 20) | // Pixel Format 16Bit Color
				(0 << 16) | // Addressing Mode is XYMode
				 3;			// Source Height is 3
   	REG_DE_STRETCH_FORMAT = command;

    // Clipping and transparent are disable after INIT 
	SetClip(0, 0, 0, 0, 0, 0);
	SetTransparency(0, 0, 0, 0);
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
void VLine2D(int y1, int y2, int x)
{
    int 	dy = y2 - y1 + 1;
    
	Wait2D_Engine();
    //WaitForNotBusy();
    POKE_32(DE_FOREGROUND , g_Color);
    POKE_32(DE_DESTINATION, x << 16 | y1);
    POKE_32(DE_DIMENSION  , 1 << 16 | dy);
    POKE_32(DE_CONTROL    , 0x8506800C);
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
void HLine2D(int x1, int x2, int y)
{
    int 	dx = x2-x1+1;

	Wait2D_Engine();    
//    WaitForNotBusy();
    POKE_32(DE_FOREGROUND , g_Color);
    POKE_32(DE_DESTINATION, x1 << 16 | y);
    POKE_32(DE_DIMENSION  , dx << 16 | 1);
    POKE_32(DE_CONTROL    , 0x8A06800C);
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
void Line2D(int x1, int y1, int x2, int y2)
{
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
void FillRect2D(int xs, int ys, int xe, int ye)
{
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
    
    //WaitForNotBusy();
	Wait2D_Engine();
	
    POKE_32(DE_WINDOW_SOURCE_BASE     , 0);  //init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_WINDOW_DESTINATION_BASE, 0);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_FOREGROUND , g_Color);
    POKE_32(DE_DESTINATION, (((ULONG) xs) <<16) | (ULONG)ys);
    POKE_32(DE_DIMENSION  , dx << 16 | dy);
    POKE_32(DE_CONTROL    , cmd << 16| 0x800c);
    SM718_2D_Engine_Wait();
    
    //dump_2Dregs();
}

/******************************************************************************
		Function name 			: BitBlt
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
//Bit10 에 따라 찌꺼기가 생기기도 하는데 확인방법은 Opaqe 로 테스트한다.
void BitBlt(int srcPage, int destPage, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy)
{
	U32		src, dest;
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	U32 cmd = 0x80000000 | //!< 	(1 << 31) |	// Drawing Engine Status 1:Start
				(0 << 30) | // Pattern Select 1:Color, 0:Mono
				(0 << 29) | // Update Destination X after operation Control
				(0 << 28) | // Quick Start Control
				(0 << 27) | // Direction Control for Operation 0:LtoR 1:RtoL
				(0 << 26) | // Major Axis for Line Drawing 0:XAxis 1:YAxis
				(0 << 25) | // X Step Control for Line Drawing 0:+ 1:-
				(0 << 24) | // Y Step Control for Line Drawing 0:+ 1:-
				(0 << 23) | // Strectch in Y Direction Control
				(0 << 22) | // Host Bitblt Select 0:color 1:mono
				(0 << 21) | // Draw Last Pixel Control for Line Drawing 0:Don't 1:Draw
				(0 << 16) | // Command Code 00000:BitBlt
				(1 << 15) | // ROP Control 0:ROP3 1:ROP2
				(0 << 14) | // ROP2 Control 0:Source is Bitmap 1:Pattern
				(2 << 12) | // Mono Data Pack Control 0:not 1: 8bit 2:16bit ???
				(0 << 11) | // Repeat Rotation Control (Only Rotate Command)
				(0 << 10) | // Transparensy match Control  0:opaque 1:transparent ???
				(0 << 9)  | // Transparensy Control 0: by Source 1:by Destination ???
				(0 << 8)  | // Transparensy Control 0:Disabled 1:Enabled ???
				//gROP2;
                (8+4);		// ROP2 Control 
				
	
	src = GetGraphicsAddr(srcPage);
	dest= GetGraphicsAddr(destPage);
	
	//Dprintf("BitBlt:: src = %08x dest=%08x\n", src, dest);
	
    //WaitForNotBusy();
	Wait2D_Engine();
    POKE_32(DE_WINDOW_SOURCE_BASE     , src);  	//init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_WINDOW_DESTINATION_BASE, dest);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_SOURCE     , (ULONG)sx << 16 | sy);
    POKE_32(DE_DESTINATION, (ULONG)dx << 16 | dy);
    POKE_32(DE_DIMENSION  , (ULONG)dimx << 16 | dimy);
    POKE_32(DE_ALPHA, 128);
    POKE_32(DE_CONTROL    , cmd);

    SM718_2D_Engine_Wait();   
} 

void BitBltRop(int srcPage, int destPage, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy, int mode, U8 rop)
{
	U32		src, dest;
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	U32 cmd =  (0x80000000 | //	(1 << 31) |	// Drawing Engine Status 1:Start
				(0 << 30) | // Pattern Select 1:Color, 0:Mono
				(0 << 29) | // Update Destination X after operation Control
				(0 << 28) | // Quick Start Control
				(0 << 27) | // Direction Control for Operation 0:LtoR 1:RtoL
				(0 << 26) | // Major Axis for Line Drawing 0:XAxis 1:YAxis
				(0 << 25) | // X Step Control for Line Drawing 0:+ 1:-
				(0 << 24) | // Y Step Control for Line Drawing 0:+ 1:-
				(0 << 23) | // Strectch in Y Direction Control
				(0 << 22) | // Host Bitblt Select 0:color 1:mono
				(0 << 21) | // Draw Last Pixel Control for Line Drawing 0:Don't 1:Draw
				(0 << 16) | // Command Code 00000:BitBlt
				(1 << 15) | // ROP Control 0:ROP3 1:ROP2
				(0 << 14) | // ROP2 Control 0:Source is Bitmap 1:Pattern
				(0 << 12) | // Mono Data Pack Control 0:not 1: 8bit 2:16bit
				(0 << 11) | // Repeat Rotation Control (Only Rotate Command)
				(1 << 10) | // Transparensy match Control  0:opaque 1:transparent
				(0 << 9)  | // Transparensy Control 0: by Source 1:by Destination
				(1 << 8)  | // Transparensy Control 0:Disabled 1:Enabled
				(rop));		// ROP2 Control 
	
	if(mode==ROP2) cmd |= 0x00008000;
	else  cmd &= ~0x00008000;
	
	src = GetGraphicsAddr(srcPage);
	dest= GetGraphicsAddr(destPage);
	
    //WaitForNotBusy();
	Wait2D_Engine();
	
    POKE_32(DE_WINDOW_SOURCE_BASE     , src);  	//init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_WINDOW_DESTINATION_BASE, dest);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_SOURCE     , (ULONG)sx << 16 | sy);
    POKE_32(DE_DESTINATION, (ULONG)dx << 16 | dy);
    POKE_32(DE_DIMENSION  , (ULONG)dimx << 16 | dimy);
    POKE_32(DE_ALPHA, 128);
    POKE_32(DE_CONTROL    , cmd);
    SM718_2D_Engine_Wait();    
} 


//Bit10 에 따라 찌꺼기가 생기기도 하는데 확인방법은 Opaqe 로 테스트한다.

void BitBlt16(int srcPage, int destPage, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy)
{
	U32		src, dest;
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	U32 cmd = 0x80000000 | //	(1 << 31) |	// Drawing Engine Status 1:Start
				(0 << 30) | // Pattern Select 1:Color, 0:Mono
				(0 << 29) | // Update Destination X after operation Control
				(0 << 28) | // Quick Start Control
				(0 << 27) | // Direction Control for Operation 0:LtoR 1:RtoL
				(0 << 26) | // Major Axis for Line Drawing 0:XAxis 1:YAxis
				(0 << 25) | // X Step Control for Line Drawing 0:+ 1:-
				(0 << 24) | // Y Step Control for Line Drawing 0:+ 1:-
				(0 << 23) | // Strectch in Y Direction Control
				(0 << 22) | // Host Bitblt Select 0:color 1:mono
				(0 << 21) | // Draw Last Pixel Control for Line Drawing 0:Don't 1:Draw
				(0 << 16) | // Command Code 00000:BitBlt
				(1 << 15) | // ROP Control 0:ROP3 1:ROP2
				(0 << 14) | // ROP2 Control 0:Source is Bitmap 1:Pattern
				(2 << 12) | // Mono Data Pack Control 0:not 1: 8bit 2:16bit ???
				(0 << 11) | // Repeat Rotation Control (Only Rotate Command)
				(1 << 10) | // Transparensy match Control  0:opaque 1:transparent ???
				(0 << 9)  | // Transparensy Control 0: by Source 1:by Destination ???
				(0 << 8)  | // Transparensy Control 0:Disabled 1:Enabled ???
				12;
                
				
	
	src = GetGraphicsAddr(srcPage);
	dest= GetGraphicsAddr(destPage);
	
    //WaitForNotBusy();
	Wait2D_Engine();
    POKE_32(DE_WINDOW_SOURCE_BASE     , src);  	//init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_WINDOW_DESTINATION_BASE, dest);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_SOURCE     , (ULONG)sx << 16 | sy);
    POKE_32(DE_DESTINATION, (ULONG)dx << 16 | dy);
    POKE_32(DE_DIMENSION  , (ULONG)dimx << 16 | dimy);
    POKE_32(DE_ALPHA, 128);
    POKE_32(DE_CONTROL    , cmd);

    SM718_2D_Engine_Wait();    
} 




/******************************************************************************
		Function name 			: HostBlt
		Function prototype 		: void HostBlt(U16 dx, U16 dy, U16 dimx, U16 dimy)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: Windows Program Start
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void HostBlt(U16 dx, U16 dy, U16 dimx, U16 dimy)
{
	U32 str=0;	
	U32 cmd = 0x80000000 | // (1 << 31) |	// Drawing Engine Status 1:Start
				(0 << 30) | // Pattern Select 1:Color, 0:Mono
				(1 << 29) | // Update Destination X after operation Control
				(0 << 28) | // Quick Start Control
				(0 << 27) | // Direction Control for Operation 0:LtoR 1:RtoL
				(0 << 26) | // Major Axis for Line Drawing 0:XAxis 1:YAxis
				(0 << 25) | // X Step Control for Line Drawing 0:+ 1:-
				(0 << 24) | // Y Step Control for Line Drawing 0:+ 1:-
				(0 << 23) | // Strectch in Y Direction Control
				(0 << 22) | // Host Bitblt Select 0:color 1:mono
				(0 << 21) | // Draw Last Pixel Control for Line Drawing 0:Don't 1:Draw
				(8 << 16) | // Command Code 00000:BitBlt 01000:Host Write
				(1 << 15) | // ROP Control 0:ROP3 1:ROP2
				(0 << 14) | // ROP2 Control 0:Source is Bitmap 1:Pattern
				(0 << 12) | // Mono Data Pack Control 0:not 1: 8bit 2:16bit
				(0 << 11) | // Repeat Rotation Control (Only Rotate Command)
				(0 << 10) | // Transparensy match Control  0:opaque 1:transparent
				(0 << 9)  | // Transparensy Control 0: by Source 1:by Destination
				(0 << 8)  | // Transparensy Control 0:Disabled 1:Enabled
				(8+4);		// ROP2 Control 
    
    //WaitForNotBusy();
	Wait2D_Engine();

    
    POKE_32(DE_WINDOW_SOURCE_BASE, 0);			//0x100040:2D Source Base Register
    POKE_32(DE_WINDOW_DESTINATION_BASE, 0);		//0x100044:2D Destination Base Register
    
    POKE_32(DE_PITCH, (LCD_X << 16) | LCD_X);	//0x100010	
    //POKE_32(DE_PITCH, 0);	    
    
    POKE_32(DE_WINDOW_WIDTH, LCD_X<<16|LCD_X);	//0x10003C
    //POKE_32(DE_WINDOW_WIDTH, 0);

    POKE_32(DE_SOURCE, 0);

    POKE_32(DE_DESTINATION, (ULONG)dx << 16 | dy);
    POKE_32(DE_DIMENSION  , (ULONG)dimx << 16 | dimy);
    str = (1 << 30) | 
    	  (6 << 27) |
    	  (0 << 23) |
    	  (1 << 20) |		//01:16Bit per Pixel
    	  (0 << 16) | 		//0: XY Mode f:Linear Mode
    	  (0 << 0);
    //POKE_32(DE_STRETCH_FORMAT, ( 16 / 8-1 )<<20 | 0x30003);
    POKE_32(DE_STRETCH_FORMAT, str);  		//0x10001C
    POKE_32(DE_CONTROL,cmd);
} 

/******************************************************************************
		Function name 			: Rotate
		Function prototype 		: void Rotate(U32 src, U32 dest, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy)
		Behavior description	: 
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: Windows Program Start
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void Rotate(U32 src, U32 dest, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy)
{
	U32 str;	
	U32 cmd = 0x80000000 | //!< 	(1 << 31) |	// Drawing Engine Status 1:Start
				(0 << 30) | // Pattern Select 1:Color, 0:Mono
				(0 << 29) | // Update Destination X after operation Control
				(0 << 28) | // Quick Start Control
				(0 << 27) | // Direction Control for Operation 0:LtoR 1:RtoL
				(0 << 26) | // Major Axis for Line Drawing 0:XAxis 1:YAxis
				(1 << 25) | // X Step Control for Line Drawing 0:+ 1:- angle
				(0 << 24) | // Y Step Control for Line Drawing 0:+ 1:-
				(0 << 23) | // Strectch in Y Direction Control
				(0 << 22) | // Host Bitblt Select 0:color 1:mono
				(0 << 21) | // Draw Last Pixel Control for Line Drawing 0:Don't 1:Draw
				(0x0b << 16) | // Command Code 00000:BitBlt 01011:Rotate
				(1 << 15) | // ROP Control 0:ROP3 1:ROP2
				(0 << 14) | // ROP2 Control 0:Source is Bitmap 1:Pattern
				(0 << 12) | // Mono Data Pack Control 0:not 1: 8bit 2:16bit
				(0 << 11) | // Repeat Rotation Control (Only Rotate Command)
				(0 << 10) | // Transparensy match Control  0:opaque 1:transparent
				(0 << 9)  | // Transparensy Control 0: by Source 1:by Destination
				(0 << 8)  | // Transparensy Control 0:Disabled 1:Enabled
				(8+4);		// ROP2 Control 
    
    //WaitForNotBusy();
	Wait2D_Engine();
	
    
    POKE_32(DE_WINDOW_SOURCE_BASE, src);			//0x100040:2D Source Base Register
    POKE_32(DE_WINDOW_DESTINATION_BASE, dest);		//0x100044:2D Destination Base Register
    
    POKE_32(DE_PITCH, (LCD_X << 16) | LCD_X);	
    //POKE_32(DE_PITCH, 0);	
   
    POKE_32(DE_WINDOW_WIDTH, LCD_X<<16|LCD_X);
    //POKE_32(DE_WINDOW_WIDTH, 0);

    POKE_32(DE_SOURCE, (ULONG)sx << 16 | sy);

    POKE_32(DE_DESTINATION, (ULONG)dx << 16 | dy);
    POKE_32(DE_DIMENSION  , (ULONG)dimx << 16 | dimy);
    str = (1 << 30) | 
    	  (0 << 27) |
    	  (0 << 23) |
    	  (1 << 20) |		//01:16Bit per Pixel
    	  (0 << 16) | 	//0: XY Mode f:Linear Mode
    	  (0 << 0);
    //POKE_32(DE_STRETCH_FORMAT, ( 16 / 8-1 )<<20 | 0x30003);
    POKE_32(DE_STRETCH_FORMAT, str);
    POKE_32(DE_CONTROL,cmd);
} 

/******************************************************************************
		Function name 			: BmpLoad
		Function prototype 		: void BmpLoad(int page, U16 sx, U16 sy, char *name)
		Behavior description	: NandBMP File Display
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: Windows Program Start
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
int BmpLoad(int graphics_page, U16 sx, U16 sy, char *filename)
{
	//Graphics gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //OnPaint 
	int		start_block;
	int		end_block;
	int		y, j, i;
	int		xs;
	int		ys;
	int		page;
	U32		lcd_addr, gpu_addr;
    int     size, cnt=0;

    int     position, lcd_x, xs_addr;
    U32     g_addr=Graphics_Address;

	U32		addr;
	st_NandFAT NandFat;
	
	U8	*VRAM_FileBuf = (U8 *)SOURCE_BMP_ADDR;

	if(SearchNandFile(filename, &NandFat))
	{
		xs = NandFat.x_size;
		ys = NandFat.y_size;
        size = xs * ys * 2;
        if(xs==0 || ys==0)
        {
            Dprintf("BmpLoad::Size Zero Error %s\n", filename);            
            return(0);
        }
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
                    cnt++;
                    if(cnt >= size) goto bmp_load_start;
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
bmp_load_start:		
		lcd_addr = Graphics_Address;
		gpu_addr = g_GPU_addr;
		
		SetGraphicsPage(graphics_page);
		addr = SOURCE_BMP_ADDR;
        
        xs =  *(U16 *)addr; addr+=2;
        ys =  *(U16 *)addr; addr+=2;

        if(gDMA_USE==YES)
        {
            xs_addr = xs * 2;
            lcd_x = LCD_X * 2;
            ys--;
            for(i=ys; i>=0; i--)
            {
                position= (((sy+i) * lcd_x)+ (sx*2));
                g_addr = Graphics_Address + position;
                DMA_Config(addr, g_addr, xs);
                while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);             
                addr = addr + xs_addr;
            }
        }
        else
        {
            for(i=ys; i> 0; i--)
            {
                for(j=0; j < xs; j++)
                {
                    BmpPlot(sx+j, sy+i, *(U16 *)addr);
                    addr+=2;
                }
            }
        }
		Graphics_Address = lcd_addr;
		g_GPU_addr = gpu_addr;	
        return(1);        
	}
	else 
	{
		Dprintf("BmpLoad::File not Found %s\n", filename);
        return(0);
	}
}
/******************************************************************************
		Function name 			: SetClipping
		Function prototype 		: int SetClip(U16 enable, int left, int top, int right, int bottom)
		Behavior description	: 2D Engine Config.
		Input parameter			:
								//	U16 enable 	: 0 = disable clipping, 1 = enable clipping 
								//	U32 x1     	: x1, y1 is the upper left corner of the clipping area 
								//	U32 y1     	: Note that the region includes x1 and y1 
								//	U32 x2     	: x2, y2 is the lower right corner of the clippiing area 
								//	U32 y2     	: Note that the region will not include x2 and y2 
		 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: Windows Program Start
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

int SetClip(U16 enable, int left, int top, int right, int bottom, int side)    
{
	DWORD 	command;
	//int waitCount=0;
	if (deWaitForNotBusy() != 0)
	{
		Dprintf("SetClip:: Not Busy\n");
		return -1;
	} 

	if(enable==1)
	{
		command =  	(top << 16)  | 	// TOP
					(1 << 13) | 	// 1:Enable
					(0 << 12) |		// 1:Write inside clipping rectangle disabled,  0:Write outside clipping rectangle disabled
					 left;			// Left
	}
	else
	{
		command =  	(top << 16) | 	// TOP
					(0 << 13) | 	// 0:Disable
					(0 << 12) |		// 1:Write inside clipping rectangle disabled,  0:Write outside clipping rectangle disabled
					 left;			// Left
	}
	REG_DE_CLIP_TL = command;
	//Dprintf("clip::TL=%08x ",  command);
	command =  	(bottom+top) << 16 | 	// BOTTOM
				(right+left);		 	// Right
	REG_DE_CLIP_BR = command;
	//Dprintf("clip::BR=%08x \n",  command);
	return(1);
}

/******************************************************************************
		Function name 			: SetClipping
		Function prototype 		: int SetClip(U16 enable, int left, int top, int right, int bottom)
		Behavior description	: 
								 * When transparency is enable, the blt engine compares each pixel value 
								 * (either source or destination) with DE_COLOR_COMPARE register.
								 * If match, the destination pixel will NOT be updated.
								 * If not match, the destination pixel will be updated.		 
		Input parameter			:
									U32 enable :   0 = disable, 1 = enable transparency feature 
									U32 tSelect:   0 = compare source, 1 = compare destination 
									U32 tMatch :   0 = Opaque mode, 1 = transparent mode 
									U32 ulColor:   Color to compare. 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: Windows Program Start
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
int SetTransparency(U32 enable,  U32 tSelect, U32 tMatch, U32 ulColor)
{
    U32 	de_ctrl;

    if (deWaitForNotBusy() != 0)
    {
        return -1;
    }

    /* Set mask */
    if (enable)
    {
        REG_DE_COLOR_COMPARE_MASK = 0x00ffffff;        
		REG_DE_COLOR_COMPARE = ulColor; // Set compare color 
    }
    else
    {
        REG_DE_COLOR_COMPARE_MASK = 0x0;        
		REG_DE_COLOR_COMPARE = 0x0;
    }

    /* Set up transparency control, without affecting other bits
       Note: There are two operatiing modes: Transparent and Opague.
       We only use transparent mode because Opaque mode may have bug.
    */
    de_ctrl = REG_DE_CONTROL;
	if(enable==1)
		de_ctrl |= 0x00000100;	// Transparency Control Enable, D8
	else 
		de_ctrl &= ~0x00000100;	// Transparency Control Disable,D8

	if(tMatch==1)
		de_ctrl |= 0x00000400;	// Transparency Match Select, 0:Opaque 1:transparent
	else
		de_ctrl &= ~0x00000400;	// Transparency Match Select, 0:Opaque 1:transparent
	if(tSelect==1)
		de_ctrl |=  0x00000200;	// Transparency is controlled, 0:Source 1:Destination
	else
		de_ctrl &= ~0x00000200;	// Transparency is controlled, 0:Source 1:Destination

    /* For DE_CONTROL_TRANSPARENCY_MATCH bit, always set it
       to TRANSPARENT mode, OPAQUE mode don't seem working.
    */
    REG_DE_CONTROL = de_ctrl;
    return 0;
}

/*

 */
/******************************************************************************
		Function name 			: SetPixelFormat
		Function prototype 		: void SetPixelFormat( unsigned long bpp )
		Behavior description	: 
 					-This function sets the pixel format that will apply to the 2D Engine.	
 		Input parameter			:
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void SetPixelFormat( unsigned long bpp )
{
    unsigned long de_format;
    
    de_format = REG_DE_STRETCH_FORMAT;
    
    switch (bpp)
    {
        case 8:
            break;
        default:
        case 16:
            de_format =  de_format & ~0x00300000;
            de_format =  de_format |  0x00100000;
            break;
        case 32:
            break;
    }
    REG_DE_STRETCH_FORMAT = de_format;
}

void InnerPanel(int sx, int sy, int width, int len, int color)
{
	g_Color = color;
    FillRect2D(sx+2,sy+2, sx+2+width-4, sy+2+len-4);

    /* Left Vertical Line */
	g_Color = clBlack;
    VLine(sx,sy,len);         /* White */
    //VLine(sx+1,sy+1,len-2);   /* White */

    /* Right Vertical Line */
	g_Color = clWhite;
    VLine(sx+width,sy,len); /* Black */
    //VLine(sx+width-1,sy+1,len-2); /* Black */

    /* Upper Horizontal Line */
	g_Color = clBlack;
    HLine(sx,sy,width);      /* White */
    //HLine(sx+1,sy+1,width-2);      /* White */

    /* Under Horizontal Line */
	g_Color = clWhite;
    HLine(sx,sy+len,width);      /* Black */
    //HLine(sx+1,sy-1+len,width-2);    /* Black */
}

void OutPanel(int sx, int sy, int width, int height, int color)
{
	g_Color = color;
	FillRect2D(sx, sy, sx+width, sy+height);

	g_Color = clWhite;
	VLine(sx, sy, height-1);
	HLine(sx, sy, width-1);
	g_Color = clBlack;
	VLine(sx+width, sy, height);
	HLine(sx, sy+height, width);
	g_Color = clDarkGray;
	VLine(sx+width-1, sy+1, height-2);
	HLine(sx+1, sy+height-1, width-2);		
}
#ifdef __cplusplus
}
#endif

