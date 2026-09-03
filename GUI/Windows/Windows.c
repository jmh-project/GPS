/******************************************************************************
*    	File name	: Windows.c 
*		Description : TEECO Project Entry File
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

Graphics		*gp_Graphics;

/******************************************************************************
							Constant & Macros
******************************************************************************/
#ifndef PSRAM_USE
//VRAM 사용시 메모리 맵
#define	MALLOC_START		    0x6B802000
#define	MALLOC_END			0x6B802000+0x100000		// MaxSize 1MByte

#define	WINDOWS_START		(0x6B802000)
#define	WIDGET_START		    (0x6B804000)

#define	CONTAINERWINDOW_1	(0x6B802000)			// 512Byte
#define	CONTAINERWINDOW_2	(0x6B802000+0x200) 	// 512Byte
#define	CONTAINERWINDOW_3	(0x6B802000+0x400)		// 512Byte
#define	CONTAINERWINDOW_4	(0x6B802000+0x600)		// 512Byte
#define	CONTAINERWINDOW_5	(0x6B802000+0x800)		// 512Byte
#define	LCD_ALPHA_CONTAINERWINDOW	(0x6B802000+0xA00)		// 512Byte
#define	VIDEO_ALPHA_CONTAINERWINDOW	(0x6B802000+0xC00)		// 512Byte

#define	WINDOW_MANAGER		(0x6B802000+0xE00)		// 512Byte
#define	GRAPHICS_MEMORY		(0x6B802000+0x1000)		// 512Byte

#define	GRAPHICS_MEMORY1		(0x6B442000)		// 512Byte
#define	GRAPHICS_MEMORY2		(0x6B442000+0x200)		// 512Byte
#define	GRAPHICS_MEMORY3		(0x6B442000+0x400)		// 512Byte
#define	GRAPHICS_MEMORY4		(0x6B442000+0x600)		// 512Byte
#define	GRAPHICS_MEMORY5		(0x6B442000+0x800)		// 512Byte
#define	GRAPHICS_MEMORY6		(0x6B442000+0xA00)		// 512Byte
#define	GRAPHICS_MEMORY7		(0x6B442000+0xC00)		// 512Byte
#define	GRAPHICS_MEMORY8		(0x6B442000+0xF00)		// 512Byte
#define	GRAPHICS_MEMORY9		(0x6B442000+0x1000)		// 512Byte
#define	GRAPHICS_MEMORY10		(0x6B442000+0x1200)		// 512Byte

#define	WIDGETWINDOW_1		(0x6B804000)			// 0x20000(128KByte)
#define	WIDGETWINDOW_2		(0x6B804000+0x20000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_3		(0x6B804000+0x40000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_4		(0x6B804000+0x60000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_5		(0x6B804000+0x80000)	// 0x20000(128KByte)
#define	LCDALPHA_WIDGETWINDOW		(0x6B804000+0xA0000)	// 0x20000(128KByte)
#define	VIDEOALPHA_WIDGETWINDOW		(0x6B804000+0xC0000)	// 0x20000(128KByte)
#endif

#ifdef PSRAM_USE
//PSRAM 사용시 메모리 맵
#define	CONTAINERWINDOW_1	(0x64020000)				// 1 KB 로 확장한다(2015/0301)
#define	CONTAINERWINDOW_2	(0x64020000+0x400) 	 
#define	CONTAINERWINDOW_3	(0x64020000+0x800)	
#define	CONTAINERWINDOW_4	(0x64020000+0xC00)	
#define	CONTAINERWINDOW_5	(0x64020000+0x1000)	
#define	CONTAINERWINDOW_6	(0x64020000+0x1400)	
#define	CONTAINERWINDOW_7	(0x64020000+0x1800)	
#define	CONTAINERWINDOW_8	(0x64020000+0x1C00)	
#define	CONTAINERWINDOW_9	(0x64020000+0x2000)	
#define	CONTAINERWINDOW_10	(0x64020000+0x2400)	

#define	LCD_ALPHA_CONTAINERWINDOW	(0x64020000+0x2800)		// 1KByte
#define	VIDEO_ALPHA_CONTAINERWINDOW	(0x64020000+0x2C00)		// 1KByte

#define	WINDOW_MANAGER		(0x64020000+0x3000)		// 1KByte
#define	GRAPHICS_MEMORY		(0x64020000+0x3400)		// 1KByte


#define	WIDGETWINDOW_1		(0x64024000)			// 0x20000(128KByte)
#define	WIDGETWINDOW_2		(0x64024000+0x20000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_3		(0x64024000+0x40000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_4		(0x64024000+0x60000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_5		(0x64024000+0x80000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_6		(0x64024000+0xA0000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_7		(0x64024000+0xC0000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_8		(0x64024000+0xE0000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_9		(0x64024000+0x100000)	// 0x20000(128KByte)
#define	WIDGETWINDOW_10		(0x64024000+0x120000)	// 0x20000(128KByte)
#define	LCDALPHA_WIDGETWINDOW		(0x64024000+0x140000)	// 0x20000(128KByte)
#define	VIDEOALPHA_WIDGETWINDOW		(0x64024000+0x160000)	// 0x20000(128KByte)
#endif

/******************************************************************************
							Private & Local Variables
******************************************************************************/
Windows			*gp_ContainerWindow_1;
Windows			*gp_ContainerWindow_2;
Windows			*gp_ContainerWindow_3;
Windows			*gp_ContainerWindow_4;
Windows			*gp_ContainerWindow_5;
Windows			*gp_LcdAlpha_ContainerWindow;
Windows			*gp_VideoAlpha_ContainerWindow;

Windows			*gp_WidgetWindow_1;			//위젯윈도우의 시작포인터
Windows			*gp_WidgetWindow_2;
Windows			*gp_WidgetWindow_3;
Windows			*gp_WidgetWindow_4;
Windows			*gp_WidgetWindow_5;
Windows			*gp_LcdAlphaWidgetWindow;
Windows			*gp_VideoAlphaWidgetWindow;

WindowsManager	*gp_WinManager;

volatile U32	g_Widget1_malloc_address;
volatile U32	g_Widget2_malloc_address;
volatile U32	g_Widget3_malloc_address;
volatile U32	g_Widget4_malloc_address;
volatile U32	g_Widget5_malloc_address;
volatile U32	g_LcdAlphaWidget_malloc_address;
volatile U32	g_VideoAlphaWidget_malloc_address;

volatile int gSoundLoad1, gSoundLoad2, gSoundLoad3;

volatile int gPaintDrawUse;	//PaintDraw()함수는 위젯하나만 그려도 빈번히 호출되기때문에 전역변수로 함수를 제어한다.

extern volatile int gFontSize_X[10];
extern volatile int gFontSize_Y[10];
/******************************************************************************
							Function Prototype
******************************************************************************/
int 	T_BootLoader(void);		//BootLoader.c
void 	Windows_Init(void);
U32		Malloc(int win_no, size_t size);	// Video Memory Use memory Allocation
Windows *FindWindowChain(HANDLE parent, HANDLE handle);
Windows *FindWidgetChain(HANDLE parent, HANDLE handle);
Windows *FindContainerChain(int handle, char *func);
int 	CloseWindow(HANDLE parent, HANDLE handle);
void 	Free(int window_no);
HANDLE 	GetWindowHandle(void);
int 	GetWindowCount(void);
int 	ConnectEventFunc(int kind, HANDLE parent, HANDLE handle, EventType event, int (*func)(HANDLE, HANDLE, WCOMMAND, PARAM, void *));
int 	GetHiddenWindowCount(char *func);
int 	PaintDraw(HANDLE parent, HANDLE handle, U32 command, U32 para, void *data);
Windows *GetZorderWindow(int zorder);
void 	WindowBitBlt(Windows *container_winow, int no);
int	 	StyleEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data);
int	 	XPStyleEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data);
int	    TeecoStyleEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data);
int    LowTextOut(int sx, int sy, const char *s, int fontcolor, int font);
void ComboWidget_Init(void);
/******************************************************************************
		Function name 			: Windows_Init
		Function prototype 		: void Windows_Init(void)
		Behavior description	: Windows Structure Init
		Input parameter			: None 
		Output parameter 		: None
		Return parameter		: None
		Required preconditions	: None
		Called functions		: Windows Program Start
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void Windows_Init(void)
{
    int     i;
//Dprintf("Windows_Init:: Start\n");    
	//컨테이너윈도우의 시작메모리를 할당 받는다.
	gp_ContainerWindow_1 = (Windows *)CONTAINERWINDOW_1;//)0x6B802000; 	//CONTAINERWINDOW_1
	memset(gp_ContainerWindow_1, 0, sizeof(Windows));
    //Dprintf("Windows1 Address=%x size=%d %x\n", gp_ContainerWindow_1, sizeof(Windows), CONTAINERWINDOW_1);
	
	gp_ContainerWindow_2 = (Windows *)CONTAINERWINDOW_2;//0x6B802200;	//CONTAINERWINDOW_2
	memset(gp_ContainerWindow_2, 0, sizeof(Windows));
	//Dprintf("Windows2 Address=%x size=%d %x\n", gp_ContainerWindow_2, sizeof(Windows), CONTAINERWINDOW_2);

	gp_ContainerWindow_3 = (Windows *)CONTAINERWINDOW_3;//0x6B802400;	//CONTAINERWINDOW_3
	memset(gp_ContainerWindow_3, 0, sizeof(Windows));
	//Dprintf("Windows3 Address=%x size=%d %x\n", gp_ContainerWindow_3, sizeof(Windows), CONTAINERWINDOW_3);

	gp_ContainerWindow_4 = (Windows *)CONTAINERWINDOW_4;//0x6B802600;	//CONTAINERWINDOW_4
	memset(gp_ContainerWindow_4, 0, sizeof(Windows));
	//Dprintf("Windows4 Address=%x size=%d %x\n", gp_ContainerWindow_4, sizeof(Windows), CONTAINERWINDOW_4);

	gp_ContainerWindow_5 = (Windows *)CONTAINERWINDOW_5;//0x6B802800;	//CONTAINERWINDOW_5
	memset(gp_ContainerWindow_5, 0, sizeof(Windows));
	//Dprintf("Windows5 Address=%x size=%d %x\n", gp_ContainerWindow_5, sizeof(Windows), CONTAINERWINDOW_5);
//Dprintf("Container allocation\n");
	
    gp_LcdAlpha_ContainerWindow = (Windows *)LCD_ALPHA_CONTAINERWINDOW;//0x6B802A00;//LCD_ALPHA_CONTAINERWINDOW;
	memset(gp_LcdAlpha_ContainerWindow, 0, sizeof(Windows));

	gp_VideoAlpha_ContainerWindow = (Windows *)VIDEO_ALPHA_CONTAINERWINDOW;//0x6B802C00;//VIDEO_ALPHA_CONTAINERWINDOW;
	memset(gp_VideoAlpha_ContainerWindow, 0, sizeof(Windows));
	

	//위젯윈도우의 시작메모리를 할당 받는다.
	gp_WidgetWindow_1 = (Windows *)WIDGETWINDOW_1;//0x6B804000; //WIDGETWINDOW_1
	memset(gp_WidgetWindow_1, 0, sizeof(Windows));
	//Dprintf("Widget1 Address=%p size=%d %x\n", gp_WidgetWindow_1, sizeof(Windows), WIDGETWINDOW_1);
//Dprintf("Widget allocation--1\n");	
	gp_WidgetWindow_2 = (Windows *)WIDGETWINDOW_2;//0x6B824000;//WIDGETWINDOW_2
	memset(gp_WidgetWindow_2, 0, sizeof(Windows));
	//Dprintf("Widget2 Address=%p size=%d %x\n", gp_WidgetWindow_2, sizeof(Windows), WIDGETWINDOW_2);
//Dprintf("Widget allocation--2\n");	

	gp_WidgetWindow_3 = (Windows *)WIDGETWINDOW_3;//0x6B844000;//WIDGETWINDOW_3
	memset(gp_WidgetWindow_3, 0, sizeof(Windows));
	//Dprintf("Widget3 Address=%p size=%d %x\n", gp_WidgetWindow_3, sizeof(Windows), WIDGETWINDOW_3);
//Dprintf("Widget allocation--3\n");	

	gp_WidgetWindow_4 = (Windows *)WIDGETWINDOW_4;//0x6B864000;//WIDGETWINDOW_4
	memset(gp_WidgetWindow_4, 0, sizeof(Windows));
	//Dprintf("Widget4 Address=%p size=%d %x\n", gp_WidgetWindow_4, sizeof(Windows), WIDGETWINDOW_4);
//Dprintf("Widget allocation--4\n");	

	gp_WidgetWindow_5 = (Windows *)WIDGETWINDOW_5;//0x6B884000;//WIDGETWINDOW_5
	memset(gp_WidgetWindow_5, 0, sizeof(Windows));	
	//Dprintf("Widget5 Address=%p size=%d %x\n", gp_WidgetWindow_5, sizeof(Windows), WIDGETWINDOW_5);
    //Dprintf("Widget allocation\n");

	gp_LcdAlphaWidgetWindow = (Windows *)LCDALPHA_WIDGETWINDOW;//0x6B8A4000; //LCDALPHA_WIDGETWINDOW;
	memset(gp_LcdAlphaWidgetWindow, 0, sizeof(Windows));	

	gp_VideoAlphaWidgetWindow = (Windows *)LCDALPHA_WIDGETWINDOW;//0x6B8C4000; //LCDALPHA_WIDGETWINDOW;
	memset(gp_VideoAlphaWidgetWindow, 0, sizeof(Windows));	


	//윈도우관리자의 시작메모리를 할당 받는다.
	gp_WinManager = (WindowsManager *)WINDOW_MANAGER;// 0x6B802E00; //WINDOW_MANAGER
	memset(gp_WinManager, 0, sizeof(WindowsManager));
	//Dprintf("manager Address=%p size=%d %x\n", gp_WinManager, sizeof(Windows), WINDOW_MANAGER);

	//위젯은 이주소를 시작으로 계속 자동 할당된다.
	g_Widget1_malloc_address = 	WIDGETWINDOW_1;//0x6B804000;//WIDGETWINDOW_1
	g_Widget2_malloc_address = 	WIDGETWINDOW_2;//0x6B824000;//WIDGETWINDOW_2
	g_Widget3_malloc_address = 	WIDGETWINDOW_3;//0x6B844000;//WIDGETWINDOW_3
	g_Widget4_malloc_address = 	WIDGETWINDOW_4;//0x6B864000;//WIDGETWINDOW_4
	g_Widget5_malloc_address = 	WIDGETWINDOW_5;//0x6B884000;//WIDGETWINDOW_5
	g_LcdAlphaWidget_malloc_address = LCDALPHA_WIDGETWINDOW;//0x6B8A4000; //LCDALPHA_WIDGETWINDOW;
	g_VideoAlphaWidget_malloc_address = VIDEOALPHA_WIDGETWINDOW;//0x6B8C4000; //VIDEOALPHA_WIDGETWINDOW;

	ComboWidget_Init(); //!< 콤보위젯을 사용하기위한 초기화

	gMsgWCount = 0;		//메시지 라이트 포인트
	gMsgRCount = 0;		//메시지 리드포인트
	gTimerWCount = 0;
	gTimerRCount = 0;
	gSystemTime = 0;	//시스템 타임
	
	g_TextColor = BLACK;

	gp_Graphics = (Graphics *)GRAPHICS_MEMORY;//0x6B803000;   //GRAPHICS_MEMORY
	memset(gp_Graphics, 0, sizeof(Graphics));

	gWinFocus = -1;		//포커스받은 윈도우 번호.
	WindowsTimerInit();
	
	IOProcessInit();
	IOTimerInit();
	
    //
    for(i=0; i<10; i++)
    {
        gFontSize_X[i] = 0;
        gFontSize_Y[i] = 0;
    }    
    
	gPaintDrawUse = ON;	//PaintDraw()함수의 활성화
    g_TextStartLine = 0;
	
    BSP_Timer();		//터치용 인터럽트와 사운드용 인터럽트 할당.
    gROP2 = 12;
//Dprintf("Windows_Init:: End\n");        
}

void PaintDrawControl(int s)
{
	if(s==ON)
		gPaintDrawUse = ON;
	else
		gPaintDrawUse = OFF;
}

void LcdAlphaTouchControl(int s)
{
    if(s==ON)
        gp_WinManager->LcdAlpha_Touch=ON;
    else
        gp_WinManager->LcdAlpha_Touch=OFF;
}

/******************************************************************************
		Function name 			: Malloc
		Function prototype 		: U32	Malloc(size_t size)
		Behavior description	: Video Memory Use memory Allocation
								  위젯의 메모리를 할당받을경우 사용한다.	
		Input parameter			: Alocation Size(Byte) 
		Output parameter 		: None
		Return parameter		: Address Value
		Required preconditions	: None
		Called functions		: Windows Program Start
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
U32	Malloc(int window_no, size_t size)
{
	static	int win_no1=0;
	static	int win_no2=0;
	static	int win_no3=0;
	static	int win_no4=0;
	static	int win_no5=0;

	static	int lcdalpha_win_no=0;
	static	int videoalpha_win_no=0;

	switch(window_no)
	{
		case 1:   if(win_no1==0) { win_no1=1;    return(g_Widget1_malloc_address); } break;
		case 2:   if(win_no2==0) { win_no2=1;     return(g_Widget2_malloc_address); } break;
		case 3:   if(win_no3==0) { win_no3=1;     return(g_Widget3_malloc_address);	} break;
		case 4:   if(win_no4==0) { win_no4=1;     return(g_Widget4_malloc_address);	} break;
		case 5:   if(win_no5==0) { win_no5=1;     return(g_Widget5_malloc_address);	} break;
		case 6:   if(lcdalpha_win_no==0) { lcdalpha_win_no=1;    return(g_LcdAlphaWidget_malloc_address);	} break;
		case 7:   if(videoalpha_win_no==0) { videoalpha_win_no=1;   return(g_VideoAlphaWidget_malloc_address);	} break;
		default:  Dprintf("%s::Err window no = %d\n", __FUNCTION__, window_no); return(0);
	}

	if(window_no==1) 
	{
		g_Widget1_malloc_address += size; 
		//memset(&g_Widget1_malloc_address, 0, sizeof(Windows)); 
		return(g_Widget1_malloc_address);
	}
	else if(window_no==2) 
	{
		g_Widget2_malloc_address += size; 
		//memset(&g_Widget2_malloc_address, 0, sizeof(Windows)); 
		return(g_Widget2_malloc_address);
	}
	else if(window_no==3) 
	{
		g_Widget3_malloc_address += size; 
		//memset(&g_Widget3_malloc_address, 0, sizeof(Windows)); 
		return(g_Widget3_malloc_address);
	}
	else if(window_no==4) 
	{
		g_Widget4_malloc_address += size; 
		//memset(&g_Widget4_malloc_address, 0, sizeof(Windows)); 
		return(g_Widget4_malloc_address);
	}
	else if(window_no==5) 
	{
		g_Widget5_malloc_address += size; 
		//memset(&g_Widget5_malloc_address, 0, sizeof(Windows)); 
		return(g_Widget5_malloc_address);
	}
	else if(window_no==6) 
	{
		g_LcdAlphaWidget_malloc_address += size; 
		//memset(&g_LcdAlphaWidget_malloc_address, 0, sizeof(Windows)); 
		return(g_LcdAlphaWidget_malloc_address);
	}
	else if(window_no==7) 
	{
		g_VideoAlphaWidget_malloc_address += size; 
		//memset(&g_VideoAlphaWidget_malloc_address, 0, sizeof(Windows)); 
		return(g_VideoAlphaWidget_malloc_address);
	}
	return(0);
}

// 컨테이너 윈도우가 클로즈될경우는 이함수를 호출하여 위젯을 메모리를 클리어 해야한다.
void Free(int window_no)
{
	switch (window_no)
	{
		case 1:	 g_Widget1_malloc_address = WIDGETWINDOW_1; break;
		case 2:	 g_Widget2_malloc_address = WIDGETWINDOW_2; break;
		case 3:	 g_Widget3_malloc_address = WIDGETWINDOW_3; break;
		case 4:	 g_Widget4_malloc_address = WIDGETWINDOW_4; break;
		case 5:	 g_Widget5_malloc_address = WIDGETWINDOW_5; break;
		case 6:	 g_LcdAlphaWidget_malloc_address = LCDALPHA_WIDGETWINDOW; break;
		case 7:	 g_VideoAlphaWidget_malloc_address = VIDEOALPHA_WIDGETWINDOW; break;
	}
}

/******************************************************************************
		Function name 			: FindWidgetChain
		Function prototype 		: Windows *FindWidgetChain(int win_no, HANDLE handle)
		Behavior description	: 주어진 위젯핸들값의 구조체체인을 찾는다.
		Input parameter			: win_no: Container Windows no, handle: Window Handle
		Output parameter 		: None
		Return parameter		: Windows structure Pointer
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

Windows *FindWidgetChain(HANDLE parent, HANDLE handle)
{
	Windows *win=NULL;

	switch(parent)
	{
		case 1: 	win = gp_WidgetWindow_1;	break;
		case 2: 	win = gp_WidgetWindow_2;	break;
		case 3: 	win = gp_WidgetWindow_3;	break;
		case 4: 	win = gp_WidgetWindow_4;	break;
		case 5: 	win = gp_WidgetWindow_5;	break;
		case 6: 	win = gp_LcdAlphaWidgetWindow;	break; 		//
		case 7: 	win = gp_VideoAlphaWidgetWindow;	break; 	//
		default:  Dprintf("%s::Err parent = %d\n", __FUNCTION__,parent); return(0);
	}

	if(win== NULL) 
	{
		Dprintf("FindWidgetChain::Invalid Handle\n");
		return (NULL);
	}
	while(1)
	{
		if(win->Handle==handle) return(win);
		if(win->NextWindow ==NULL) return(NULL);
		win = (Windows *)win->NextWindow;
	}
}

/******************************************************************************
		Function name 			: FindContainerChain
		Function prototype 		: Windows *FindContainerChain(int win_no, HANDLE handle)
		Behavior description	: 주어진 컨테이너핸들값의 구조체체인을 찾는다.
		Input parameter			: win_no: Container Windows no, handle: Window Handle
		Output parameter 		: None
		Return parameter		: Windows structure Pointer
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
Windows *FindContainerChain(HANDLE handle, char *func)
{
	Windows *win = NULL;
    //Dprintf("FindContainerChain:: handle=%d\n", handle);
	switch(handle)
	{
		case 1: 	
				win = gp_ContainerWindow_1;	
		break;
		case 2: 	
				win = gp_ContainerWindow_2;	
		break;
		case 3: 	
				win = gp_ContainerWindow_3;	
		break;
		case 4: 	
				win = gp_ContainerWindow_4;	
		break;
		case 5: 	
				win = gp_ContainerWindow_5;	
		break;
		case 6: 	
				win = gp_LcdAlpha_ContainerWindow;	
		break;
		case 7: 	
				win = gp_VideoAlpha_ContainerWindow;	
		break;
		default:
			Dprintf("FindContainerChain::Windows_no mistyping Handle=%d  func=%s\n", handle, func); 
		break;
	}

	if(win== NULL) 
	{
		Dprintf("FindContainerChain::Invalid Handle func=%s\n", func);
		return (NULL);
	}
	return(win);
}
extern volatile int 	gTouchX, gTouchY, gPress;
Windows *FindContainerWindow(HANDLE handle, char *func)
{
	Windows *win = NULL;
//Dprintf("FindContainerChain:: handle=%d\n", handle);
	switch(handle)
	{
		case 1: 	
			if(gp_WinManager->ContainerWin[1]==true)
				win = gp_ContainerWindow_1;	
		break;
		case 2: 	
			if(gp_WinManager->ContainerWin[2]==true)
				win = gp_ContainerWindow_2;	
		break;
		case 3: 	
			if(gp_WinManager->ContainerWin[3]==true)
				win = gp_ContainerWindow_3;	
		break;
		case 4: 	
			if(gp_WinManager->ContainerWin[4]==true)
				win = gp_ContainerWindow_4;	
		break;
		case 5: 	
			if(gp_WinManager->ContainerWin[5]==true)
				win = gp_ContainerWindow_5;	
		break;
		case 6: 	
			if(gp_WinManager->LcdAlphaContainerWin==true)
				win = gp_LcdAlpha_ContainerWindow;	
		break;
		case 7: 	
			if(gp_WinManager->VideoAlphaContainerWin==true)
				win = gp_VideoAlpha_ContainerWindow;	
		break;
		default:	
			Dprintf("FindContainerWindow::Windows_no mistyping Handle=%d %s  x=%d y=%d\n", handle, func, gTouchX, gTouchY); 
		break;
	}

	if(win== NULL) 
	{
#ifdef DEBUG_WINIDOW					
		Dprintf("FindContainerWindow::Invalid Handle Func:%s\n", func);
#endif		
		return (NULL);
	}
	return(win);
}



Windows *GetWindow(HANDLE parent, HANDLE handle, char *func)
{
	char	buf[64];
	Windows	*win=NULL;
	
	strcpy(buf, "GetWindow");
	strcat(buf, func);
	
	if(parent==NULL) win = FindContainerWindow(handle, buf);
	else win = FindWidgetChain(parent, handle);
	
	if(win==NULL)
	{
#ifdef DEBUG_WINIDOW			
		Dprintf("GetWindow::window not found parent=%d handle=%d\n", parent, handle);
#endif
		return(0);
	}
	return(win);
}

//모드에 따른 윈도우를 찾는데 사용한다.
//키패드윈도우를 찾기위해 만들었다.
Windows *GetWindowByMode(int mode)
{
	int		i;
	int		cnt = GetWindowCount();
	Windows	*win=NULL;
	
	//윈도우가 없으면 탈출
	if(cnt==0) return(win);
	//컨테이너 윈도우중에서 먼저 찾는다.
	for(i=1; i<=cnt; i++)
	{
		win = GetWindow(NULL, i,"GetWindowByMode");
		if(win->Mode == mode)
		{
			return(win);
		}
	}
	return(0);	
}

int IsWindowName(char *name)
{
	int		i;
	int		cnt = GetWindowCount();
	Windows	*win=NULL;
	
	//윈도우가 없으면 탈출
	if(cnt==0) return(0);
	//컨테이너 윈도우중에서 먼저 찾는다.
	for(i=1; i<=7; i++)
	{
		win = GetWindow(NULL, i,"IsGetWindowName");
        if(win != NULL)
        {
            if(!strcmp(win->Name, name))
            {
                return(1);
            }
        }
	}
	return(0);	
}

int GetWindowName(char *name)
{
	int		i;
	int		cnt = GetWindowCount();
	Windows	*win=NULL;
	
	//윈도우가 없으면 탈출
	if(cnt==0) return(0);
	//컨테이너 윈도우중에서 먼저 찾는다.
	for(i=1; i<=cnt; i++)
	{
		win = GetWindow(NULL, i,"IsGetWindowName");
		if(!strcmp(win->Name, name))
		{
			return(1);
		}
	}
	return(0);	
}


Windows *GetWindowByName(char *name)
{
	int		i;
	int		cnt = GetWindowCount();
	Windows	*win=NULL;
	
	//윈도우가 없으면 탈출
	if(cnt==0) return(0);
	//컨테이너 윈도우중에서 먼저 찾는다.
	for(i=1; i<=7; i++)
	{
		win = GetWindow(NULL, i,"GetWindowByName");
        if(win != NULL)
        {        
            if(!strcmp(win->Name, name))
            {
                return(win);
            }
        }
	}
	return(0);	
}

/******************************************************************************
		Function name 			: FindStartWindowChain
		Function prototype 		: Windows *FindStartWindowChain(int win_no
		Behavior description	: 윈도우구조체의 내용중 PreviousWindow 가 NULL 인 체인을 찾는다.
								  첫번째윈도우를 찾는다.	
							      512 개이상을 검색하게되면 실패로 간주하고 리턴한다.
		Input parameter			: kind: Windows Kind
		Output parameter 		: None
		Return parameter		: Windows structure Pointer
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

Windows *FindStartWindowChain(int win_no)
{
	int		count=0;
	Windows *win=0;

	switch(win_no)
	{
		case 1: 	win = gp_WidgetWindow_1;	break;
		case 2: 	win = gp_WidgetWindow_2;	break;
		case 3: 	win = gp_WidgetWindow_3;	break;
		case 4: 	win = gp_WidgetWindow_4;	break;
		case 5: 	win = gp_WidgetWindow_5;	break;
		case 6: 	win = gp_LcdAlphaWidgetWindow;	break; //
		case 7: 	win = gp_VideoAlphaWidgetWindow;	break; //
		default:	Dprintf("FindNullWindowChain::Windows_no mistyping\n"); return(0);
	}

	while(1)
	{
		if(win->PreviousWindow ==NULL) return(win);
		else
		{
			win = (Windows *)win->NextWindow;
			count++;
			if(count > 128) return(NULL);
		}
	}
}



/******************************************************************************
		Function name 			: FindNullWindowChain
		Function prototype 		: Windows *FindNullWindowChain(int kind)
		Behavior description	: 윈도우구조체의 내용중 NextWindow 가 NULL 인 체인을 찾는다.
								  마지막윈도우를 찾는다.	
							      512 개이상을 검색하게되면 실패로 간주하고 리턴한다.
		Input parameter			: kind: Windows Kind
		Output parameter 		: None
		Return parameter		: Windows structure Pointer
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

Windows *FindNullWindowChain(int win_no)
{
	int		count=0;
	Windows *win=NULL;

	switch(win_no)
	{
		case 1: 	win = gp_WidgetWindow_1;	break;
		case 2: 	win = gp_WidgetWindow_2;	break;
		case 3: 	win = gp_WidgetWindow_3;	break;
		case 4: 	win = gp_WidgetWindow_4;	break;
		case 5: 	win = gp_WidgetWindow_5;	break; //
		case 6: 	win = gp_LcdAlphaWidgetWindow;	break; //
		case 7: 	win = gp_VideoAlphaWidgetWindow;	break; //
		default:	Dprintf("FindNullWindowChain::Windows_no mistyping\n"); return(0);
	}

	while(1)
	{
		if(win->NextWindow ==NULL) return(win);
		else
		{
			win = (Windows *)win->NextWindow;
			count++;
			if(count > 128) return(NULL);
		}
	}
}

/******************************************************************************
		Function name 			: GetWindowHandle
		Function prototype 		: HANDLE GetWindowHandle(void)
		Behavior description	: 컨테이너 윈도우의 핸들을 부여받는다.
		Input parameter			: None
		Output parameter 		: None
		Return parameter		: 성공시 핸들값을 
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
HANDLE GetWindowHandle(void)
{
	if(gp_WinManager->ContainerWin[1] == false) return(1);
	if(gp_WinManager->ContainerWin[2] == false) return(2);
	if(gp_WinManager->ContainerWin[3] == false) return(3);
	if(gp_WinManager->ContainerWin[4] == false) return(4);
	if(gp_WinManager->ContainerWin[5] == false) return(5);
	return(0);
}


/******************************************************************************
		Function name 			: GetContainerZorder
		Function prototype 		: int GetContainerZorder(void)
		Behavior description	: 컨테이너 윈도우의 Z군위를 부여받는다.
		Input parameter			: None
		Output parameter 		: None
		Return parameter		: 성공시 핸들값 
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
//버그가 있음 할당된 윈도우가 1번의 Z 을 가져야한다.
int GetContainerZorder(void)
{
	int		win_cnt = GetWindowCount();

	if(win_cnt==5) return(0);	//윈도우가 5개 다만들어졌으므로 0을 리턴
	
	if(gp_WinManager->ContainerWin[1] == false) return(1);
	if(gp_WinManager->ContainerWin[2] == false) return(2);
	if(gp_WinManager->ContainerWin[3] == false) return(3);
	if(gp_WinManager->ContainerWin[4] == false) return(4);
	if(gp_WinManager->ContainerWin[5] == false) return(5);

	return(1);
}

/******************************************************************************
		Function name 			: GetWindowCount
		Function prototype 		: int GetWindowCount(void)
		Behavior description	: 컨테이너 윈도우의 현재갯수얻기
		Input parameter			: None
		Output parameter 		: None
		Return parameter		: Count Window
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
int GetWindowCount(void)
{
	int		cnt=0;
	if(gp_WinManager->ContainerWin[1] == true) cnt++;
	if(gp_WinManager->ContainerWin[2] == true) cnt++;
	if(gp_WinManager->ContainerWin[3] == true) cnt++;
	if(gp_WinManager->ContainerWin[4] == true) cnt++;
	if(gp_WinManager->ContainerWin[5] == true) cnt++;
	return(cnt);
}

//컨테윈도우중에서 히든윈도우의 수를 구한다.
int GetHiddenWindowCount(char *func)
{			 
	char	buf[64];
	Windows *win;
	int		i, cnt=0;

	strcpy(buf,func);
	strcat(buf,"::GetHiddenWindowCount");
	for(i=1; i<=5; i++)
	{
		win = FindContainerWindow(i, buf);
		//윈도우가 생성이 되어있고 히든상태인것만 찾는다.
		if(win!=NULL)
		{
			if(gp_WinManager->ContainerWin[i] == true && win->ShowStatus==HIDDEN) cnt++;
		}
	}
	return(cnt);
}

//컨테윈도우중에서 히든윈도우의 수를 구한다.
int GetWindowList(char *func)
{			 
	char	buf[64];
	Windows *win;
	int		i, cnt=0;

	strcpy(buf,func);
	strcat(buf,"::GetHiddenWindowCount");
	for(i=1; i<=5; i++)
	{
		win = FindContainerWindow(i, buf);
		//윈도우가 생성이 되어있고 히든상태인것만 찾는다.
		if(win!=NULL)
		{
			Dprintf("Windows Name=%s\n", win->Name);
		}
	}
	return(cnt);
}

Windows *GetZorderWindow(int zorder)
{
	int		i;			 
	Windows *win;

	for(i=1; i<=5; i++)
	{
		win = FindContainerWindow(i, "GetZorderWindow");
		//윈도우가 생성이 되어있고 Z 번호가 같은 윈도우를 찾는다.
		if(win != NULL)
		{
			if(gp_WinManager->ContainerWin[i] == true && win->Zorder==zorder) 
			{
				return(win);
			}
		}
	}
	Dprintf("GetZorderWindow:: Windows Not Found=%d\n", zorder);
	return(NULL);
}

int GetWindowWidth(HANDLE parent, HANDLE handle)
{
	Windows		*win;
	if(parent==NULL)
		win = FindContainerWindow(handle,"GetWindowWidth");
	else
		win = FindWidgetChain(parent, handle);
	if(win==NULL)
	{
		Dprintf("GetWindowWidth:: Windows Handle Error\n");
	}	
	return(win->client_width);
}

int GetWindowHeight(HANDLE parent, HANDLE handle)
{
	Windows		*win;
	if(parent==NULL)
		win = FindContainerWindow(handle,"GetWindowHeight");
	else
		win = FindWidgetChain(parent, handle);
	if(win==NULL)
	{
		Dprintf("GetWindowHeight:: Windows Handle Error\n");
	}
	return(win->client_height);
}

int GetWindowZorder(HANDLE parent, HANDLE handle)
{
	Windows		*win;
	if(parent==NULL)
		win = FindContainerWindow(handle,"GetWindowZorder");
	else
		win = FindWidgetChain(parent, handle);
	
	if(win==NULL)
	{
		Dprintf("GetWindowZorder:: Windows Handle Error\n");
	}	
	return(win->Zorder);
}

/******************************************************************************
		Function name 			: CloseWindow
		Function prototype 		: int CloseWindow(int kind, int handle)
		Behavior description	: 지정된 윈도우를 화면에서 지우고 메모리에서도 지운다.
		 						  컨테이너 윈도우만 사용이 가능하다.
		Input parameter			: kind: Windows Kind Handle:
		Output parameter 		: None
		Return parameter		: 성공시 핸들값을 
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
int HiddenWindow(HANDLE parent, HANDLE handle)
{
	Windows *win = FindContainerWindow(handle,"HiddenWindow");
	Graphics 	*Gr = GetGraphics(NULL, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(parent != NULL) return(0);
	
	if(win==NULL)
	{
		Dprintf("HiddenWindow:: Windows Handle Error\n");
		return(0);
	}
	
	win->ShowStatus = HIDDEN;

	if(win->Mode==INPUT_WINDOW)
	{
		gp_WinManager->Input_Show = HIDDEN;
	}
	else if(win->Mode==MODAL_WINDOW)
	{
		gp_WinManager->Modal_Show = HIDDEN;
	}
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(NULL, handle, 0, 0, 0);		
	
	//클로즈된윈도우가 있을시는 더블버퍼를 1회 클리어 해주어야한다.
	//ScreenClear(DOUBLE_SCREEN);
	return(1);
}

int ShowWindow(HANDLE parent, HANDLE handle)
{
	Graphics *Gr;
	Windows *win = FindContainerWindow(handle,"ShowWindow");
	if(win==NULL)
	{
		Dprintf("ShowWindow:: Windows Handle Error\n");
		return(0);
	}
	if(parent != NULL) return(0);
	win->ShowStatus = SHOW;
	if(win->Mode==INPUT_WINDOW)
	{
		gp_WinManager->Input_Show = SHOW;
	}
	else if(win->Mode==MODAL_WINDOW)
	{
		gp_WinManager->Modal_Show = SHOW;
	}
	
	Gr = GetGraphics(parent, handle, WINDOW_AREA); 	//그래픽영역을 얻는다.
	if(win->Style==XP_STYLE) win->StyleDrawEvent(win->Parent, win->Handle, 0, 0, win->Text);
	Gr->Draw(parent, handle, 0, 0, 0);	
	return(1);
}

int MoveWindow(HANDLE parent, HANDLE handle, int dx, int dy)
{
	Windows *window = FindContainerWindow(handle,"MoveWindow");
	Graphics 	*Gr;
	if(window==NULL)
	{
		Dprintf("MoveWindow:: Windows Handle Error\n");
		return(0);
	}	
	if(parent != NULL)
	{
		Dprintf("MoveWindow::Only use Container window\n");
		return(0);
	}
	window->abs_sx = dx;
	window->abs_sy = dy;
	window->abs_ex = window->abs_sx + window->width;
	window->abs_ey = window->abs_sy + window->height;

	window->client_abs_sx = window->abs_sx;
	window->client_abs_sy = window->abs_sy;
	window->client_abs_ex = window->abs_ex;
	window->client_abs_ey = window->abs_ey;	

	//윈도우가 이동되면 더블버퍼영역을 클리어 시킨다.
	ScreenClear(DOUBLE_SCREEN);
	Gr = GetGraphics(parent, handle, WINDOW_AREA); 	//그래픽영역을 얻는다.
	if(window->Style==XP_STYLE) window->StyleDrawEvent(parent, handle, 0, 0, window->Text);
	
	window->client_sx = window->sx + WS_LINE_WIDTH;
	window->client_sy = window->sy + (WS_TITLE_HEIGHT);
	
	window->client_width = window->width - (WS_LINE_WIDTH * 2);
	window->client_height= window->height - (WS_TITLE_HEIGHT+WS_LINE_WIDTH);

	window->client_abs_sx = SCREEN_SX + window->client_sx;			
	window->client_abs_sy = SCREEN_SY + window->client_sy;
	
	window->client_abs_ex = window->client_abs_sx + window->client_width;
	window->client_abs_ey = window->client_abs_sy + window->client_height;	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(NULL, handle, 0, 0, 0);
	return(1);	

}

int ResizeWindow(HANDLE parent, HANDLE handle, int width, int height)
{
	Graphics *Gr;
	Windows *window = FindContainerWindow(handle,"ResizeWindow");
	//Graphics 	*Gr = GetGraphics(NULL, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(window==NULL)
	{
		Dprintf("ResizeWindow:: Windows Handle Error\n");
		return(0);
	}	
	if(parent != NULL)
	{
		Dprintf("ResizeWindow::Only use Container window\n");
		return(0);
	}
	window->width = width;
	window->height = height;
	
	window->abs_ex = window->abs_sx + width;
	window->abs_ey = window->abs_sy + height;

	window->client_abs_sx = window->abs_sx;
	window->client_abs_sy = window->abs_sy;
	window->client_abs_ex = window->abs_ex;
	window->client_abs_ey = window->abs_ey;	

	//윈도우가 이동되면 더블버퍼영역을 클리어 시킨다.
	ScreenClear(DOUBLE_SCREEN);
	Gr = GetGraphics(parent, handle, WINDOW_AREA); 	//그래픽영역을 얻는다.
	if(window->Style==XP_STYLE) window->StyleDrawEvent(parent, handle, 0, 0, window->Text);
	
	window->client_sx = window->sx + WS_LINE_WIDTH;
	window->client_sy = window->sy + (WS_TITLE_HEIGHT);
	
	window->client_width = window->width - (WS_LINE_WIDTH * 2);
	window->client_height= window->height - (WS_TITLE_HEIGHT+WS_LINE_WIDTH);

	window->client_abs_sx = SCREEN_SX + window->client_sx;			
	window->client_abs_sy = SCREEN_SY + window->client_sy;
	
	window->client_abs_ex = window->client_abs_sx + window->client_width;
	window->client_abs_ey = window->client_abs_sy + window->client_height;	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(NULL, handle, 0, 0, 0);	
	return(1);	
}
void Free_Combo(int window_no);
int CloseWindow(HANDLE parent, HANDLE handle)
{
	int		i;
    
	Windows *win = FindContainerWindow(handle, "CloseWindow");
	//여기서는 1번의 영역을 얻어야한다.
	Graphics 	*Gr = GetGraphics(NULL, 1, WINDOW_AREA); 	//그래픽영역을 얻는다.
	//int		zorder = win->Zorder;
	int		cnt = GetWindowCount();
	int		mode = win->Mode;
    Windows *temp_win=NULL;

//Dprintf("CloseWindow::Z=%d  WinCnt=%d\n", zorder, cnt);    
	if(parent==0 && handle==0)
	{
		Dprintf("CloseWindow::Dummy Close\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("CloseWindow: Graphics Error\n");
		return(0);
	}	
	if(win==NULL)
	{
		Dprintf("CloseWindow: Window Error parent=%d handle=%d\n", parent, handle);
		return(0);
	}
//Dprintf("CloseWindow::handle=%d  zorder=%d totalWinCnt=%d\n", handle, zorder, cnt);
	if(parent != NULL) 
	{
		Dprintf("CloseWindow: Widget is not use\n");
		return(0);
	}   
    
	/**< 해당되는 윈도우의 CloseEvent를 호출한다 */
	if(win->CloseEvent != 0)
		win->CloseEvent(parent, handle, 0, 0, 0);
	
	//클로즈되는 윈도우의 타이머를 확인후 타이머를 해제시킨다.	
	WindowCloseAuto(parent, handle);
    //클로즈되는 윈도우가 있을경우 기존윈도우의 Z 순서도 변경해주어야한다.		
    for(i=1; i<=5; i++)             //윈도우를 5개를 모두검사해서 
    {
        if(gp_WinManager->ContainerWin[i] == true) // 활성화된 윈도우를 찾으면
        {
            temp_win = FindContainerChain(i, "CloseWindow"); //주어진 번호의 윈도우핸들을 리턴한다. 
            //Dprintf("CloseWindow:: Current win No=%d Name=%s Z=%d\n", i, temp_win->Name, temp_win->Zorder);
            if(temp_win->Zorder > win->Zorder)  // 삭제할 윈도우보다 번호가 큰윈도우는 
            {
                temp_win->Zorder--;     // 1을 빼준다.
            }
        }
    }

	Free(handle);	//소속된 위젯의 영역을 클리어한다.
	Free_Combo(handle);	//소속된 콤보위젯의 영역을 클리어한다.
	switch(handle)
	{
		case 1:  gp_WinManager->WidgetCount[1] = 0; gp_WinManager->ContainerWin[1] = false; memset(gp_ContainerWindow_1, 0, sizeof(Windows)); memset(gp_WidgetWindow_1, 0, sizeof(Windows)); break;
		case 2:  gp_WinManager->WidgetCount[2] = 0; gp_WinManager->ContainerWin[2] = false; memset(gp_ContainerWindow_2, 0, sizeof(Windows)); memset(gp_WidgetWindow_2, 0, sizeof(Windows));  break;
		case 3:  gp_WinManager->WidgetCount[3] = 0; gp_WinManager->ContainerWin[3] = false; memset(gp_ContainerWindow_3, 0, sizeof(Windows)); memset(gp_WidgetWindow_3, 0, sizeof(Windows));  break;
		case 4:  gp_WinManager->WidgetCount[4] = 0; gp_WinManager->ContainerWin[4] = false; memset(gp_ContainerWindow_4, 0, sizeof(Windows)); memset(gp_WidgetWindow_4, 0, sizeof(Windows));  break;
		case 5:  gp_WinManager->WidgetCount[5] = 0; gp_WinManager->ContainerWin[5] = false; memset(gp_ContainerWindow_5, 0, sizeof(Windows)); memset(gp_WidgetWindow_5, 0, sizeof(Windows));  break;
		case 6:  
		{
			gp_WinManager->LcdAlphaWidgetCount = 0; gp_WinManager->LcdAlphaContainerWin = false; 
			memset(gp_LcdAlpha_ContainerWindow, 0, sizeof(Windows)); 
			LCD_AlphaScreen(OFF);
			memset(gp_LcdAlphaWidgetWindow, 0, sizeof(Windows)); 
			break;
		}
		case 7: 
		{	
			gp_WinManager->VideoAlphaWidgetCount = 0; gp_WinManager->VideoAlphaContainerWin = false;
			memset(gp_VideoAlpha_ContainerWindow, 0, sizeof(Windows)); 
			VideoAlphaScreen(OFF);
			VideoScreen(OFF);
			memset(gp_VideoAlphaWidgetWindow, 0, sizeof(Windows)); 
			break;
		}
		default:	Dprintf("CloseWindow::Windows_no mistyping\n"); break;
	}

	//클로즈되는 윈도우가 입력윈도우면 관리자에서 삭제한다.
	if(mode==INPUT_WINDOW) gp_WinManager->InputWindow_No = 0;
//Dprintf("CloseWindow:: mode=%d\n", mode);
	//현재 클로즈된 윈도우의 Z보다 큰윈도우를 -1 한다.
	
	if(cnt==0) return(1);
    /*
	for(i=1; i<=cnt; i++)
	{
		win = FindContainerWindow(i,"CloseWindow");
		if(win != NULL)
		{
			if(win->Zorder > zorder)  win->Zorder--;
		}
	}
    */
	//클로즈된윈도우가 있을시는 더블버퍼를 1회 클리어 해주어야한다.
	//

//Dprintf("CloseWindow:: ScreenClear-Double Screen  parent=%d handle=%d\n", parent, handle);
	/*
	ScreenClear(DOUBLE_SCREEN);	
	SM718_RAMClear(CONTAINER_WIDGET2_ADDR, clBlack);	
	SM718_RAMClear(CONTAINER_WINDOW2_ADDR, clBlack);	
	SM718_RAMClear(DOUBLE_SCREEN_ADDR, clBlack);		
	PaintDraw(handle, 0, 0, 0, 0);	
	*/
	
	Gr->SetColor(RED);
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(NULL, handle, 0, 0, 0);		
	//클로즈되는 윈도우가 모달이라면 모달을 해제한다.
	if(gp_WinManager->ModalWindow_No == handle)
	{
		gp_WinManager->ModalWindow_No = 0;
	}    
    
	return(1);
}

/******************************************************************************
		Function name 			: ConnectEventFunc
		Function prototype 		: int ConnectEventFunc(
									int kind, HANDLE handle, 
									EventType event, 
									int (*func)(HANDLE, WCOMMAND, PARAM, void *));
		Behavior description	: 설명:특정윈도우에 이벤트함수를 등록하는 함수이다.
								  초기생성시 등록안한경우 동적으로 등록할수있게하는 함수이다.
		Input parameter			: kind: Windows Kind , Handle:Window Handle
										func *: Interface Function Pointer
		Output parameter 		: None
		Return parameter		: sucess:1 fail:0
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
int ConnectEventFunc(int kind, HANDLE parent, HANDLE handle, EventType event, int (*func)(HANDLE, HANDLE, WCOMMAND, PARAM, void *))
{
	Windows	*window=NULL;

	if(kind==CONTAINER_WINDOW)		
	{
		switch(handle)
		{
			case 1:	window = gp_ContainerWindow_1;	break;
			case 2:	window = gp_ContainerWindow_2;	break;
			case 3:	window = gp_ContainerWindow_3;	break;
			case 4:	window = gp_ContainerWindow_4;	break;
			case 5:	window = gp_ContainerWindow_5;	break;
		}
	}
	else if(kind==WIDGET_WINDOW) 	
	{
		window = FindWidgetChain(parent, handle);
		if(window==NULL) return(0);
	}
	switch(event)
	{
		case MainFunction:		window->MainFunction = func;	break; //0
		//case CreateEvent:  		window->CreateEvent = func;		break; //1
		case PaintEvent:  		window->PaintEvent = func;		break; //2
		case MouseOnEvent: 		window->MouseOnEvent = func;	break; //3
		case MouseOffEvent: 	window->MouseOffEvent = func;	break; //4
		case MouseMoveEvent: 	window->MouseMoveEvent = func;	break; //5
		case GestureEvent:		window->GestureEvent = func;	break; //6
		case MoveEvent:			window->MoveEvent = func;		break; //7
		case ResizeEvent:		window->ResizeEvent = func;		break; //8
		case ClickEvent:  		window->ClickEvent = func;		break; //9
		case DoubleClickEvent:  window->DoubleClickEvent=func;	break; //10
		case KeyEvent:		    window->KeyEvent = func;		break; //11
		case EncoderEvent:		window->EncoderEvent = func;	break; //12
		case TimerEvent:		window->TimerEvent = func;		break; //13
		case ShowEvent:			window->ShowEvent = func;		break; //14
		case HiddenEvent:		window->HiddenEvent = func;		break; //15
		case ControlEvent:		window->ControlEvent = func;	break; //16
		case ChangeEvent:		window->ChangeEvent = func;		break; //17
		case CloseEvent:		window->CloseEvent = func;		break; //18
		default: break;
	}
	return(1);
}

/******************************************************************************
		Function name 			: MakeWindow
		Function prototype 		: 
		Behavior description	: 윈도우를 만드는 함수.
		Input parameter			: Handle:Windows Handle, 
		Output parameter 		: None
		Return parameter		: sucess:1 , fail:0
		Required preconditions	: None
		Called functions		: None
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/

int makeWindow(	int screen,				//윈도우가 생성될 스크린
		char *name, 					//Window Name(검색에도사용)->Name에대입
		HANDLE ParentHandle, int kind, 	//CONTAINER_WINDOW, WIDGET_WINDOW
		int style, 						//XP_STYLE, TEECO_STYLE, STYLE_NONE
		int sx, int sy, int width, int height, int	show,
		int	(*CreateEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	//1
		int	(*PaintEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		//2
		int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	//3
		int	(*MouseOffEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	//4
		int	(*MouseMoveEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	//5
		int	(*GestureEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	//6
		int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		//7
		int	(*TimerEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		//8
		int	(*ControlEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	//9
		int	(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	//10
		int	(*KeyEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	    //11
		int	(*EncoderEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	//12
		int	(*FocusEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	    //13
		int	(*CloseEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	    //14
		PARAM param,				//임의로 정의된다.
		char	*NormalName,		//메모리버튼용, BMP용
		char	*TouchName,			//메모리버튼용, ICON용
		char	*text,				//속성->Text에 개입된다.
		int 	mode,				//MODAL, MODALLESS, INPUT (WINDOW의 종류)
		WidgetStruct *wid_struct    //위젯뿐이아니고 윈도우의 속성제어에도 사용한다.
		)
{
	Windows		*window=NULL, *win=NULL;	//새로 생성할 윈도우 구조체
	Windows		*LastWindow;
	Windows		*ParentWindow;
	HANDLE		handle;
    //int         win_cnt=0;
	int			i;
	
	if(kind==CONTAINER_WINDOW)
	{
		if(screen==LCD_ALPHA)
		{
            if(gp_WinManager->LcdAlpha_On==true)
            {
                Dprintf("makeWindow::LCD Alpha Windows Already Exist\n");
                return(0);
            }
			window = gp_LcdAlpha_ContainerWindow;
			memset(window, 0, sizeof(Windows));
			window->Parent = NULL;
			window->Handle = 6;		

			gp_WinManager->LcdAlpha_On = true;	// LCD알파윈도우가 생성되었음을 체크한다.
			gp_WinManager->LcdAlphaContainerWin	= true;		
			gp_WinManager->LcdAlphaWidgetCount = 0;		//재확인용
			window->abs_sx = sx;
			window->abs_sy = sy;
			window->abs_ex = window->abs_sx + width;
			window->abs_ey = window->abs_sy + height;
			window->Kind = CONTAINER_WINDOW;
			window->MainFunction = ContainerMSGParsing;	
			//여기에 사이즈를 지정하여 오픈시킨다.
			Lcd_AlphaTransparency(5);
			LCD_AlphaScreen(ON);
		}
		else if(screen==VIDEO_ALPHA)
		{
            if(gp_WinManager->VideoAlphaContainerWin==true)
            {
                Dprintf("makeWindow::Video Alpha Windows Already Exist\n");
                return(0);
            }
            
			handle = 1;
			window = gp_VideoAlpha_ContainerWindow;
			memset(window, 0, sizeof(Windows));
			window->Parent = NULL;
			window->Handle = 7;			

			gp_WinManager->VideoAlphaContainerWin = true;
			gp_WinManager->VideoAlpha_On = true;	// LCD알파윈도우가 생성되었음을 체크한다.
			gp_WinManager->VideoAlphaWidgetCount = 0;		//재확인용
			window->abs_sx = sx;
			window->abs_sy = sy;
			window->abs_ex = window->abs_sx + width;
			window->abs_ey = window->abs_sy + height;
			window->Kind = CONTAINER_WINDOW;
			window->MainFunction = ContainerMSGParsing;	
			//여기에 사이즈를 지정하여 오픈시킨다.
			//Video_AlphaTransparency(10);
			
			VideoScreen(ON);
			VideoAlphaScreen(ON);
		}
		else if(screen==LCD)
		{
			handle = GetWindowHandle(); //1 ~5번 윈도우중 비어있는 윈도우를 찾는다. 할당할수 없으면 0을 리턴한다.
            if(handle==0)
            {
                Dprintf("makeWindow::LCD Window not allocation-> Container Windows is FULL\n");
                return(0);
            }
            //#define		MODALLESS_WINDOW	0
            //#define		MODAL_WINDOW		1
            //#define		INPUT_WINDOW		2
			if(mode < MODALLESS_WINDOW || mode > INPUT_WINDOW) //미리정의된 윈도우모드가 아님 다른값이 들어오면 윈도우 에러발생한다.
			{
				Dprintf("MakeWindow::Window Mode Input Error(MODAL~)\n");			
				return(0);
			}
			//모달이나 키패드가  떠있으면 윈도우를 생성할수 없다.
			if((gp_WinManager->ModalWindow_No != 0 && gp_WinManager->Modal_Show==SHOW) || (gp_WinManager->InputWindow_No != 0 && gp_WinManager->Input_Show==SHOW))
			{
				Dprintf("MakeWindow::Use after ModalWindow closing\n");
				Dprintf("Modal=%d Inout=%d\n",gp_WinManager->ModalWindow_No, gp_WinManager->InputWindow_No);
				return(0);
			}
			//입력키패드가 존재한다면 다시 입력윈도우를 만들수 없다.
			if(mode==INPUT_WINDOW && gp_WinManager->InputWindow_No != 0)
			{
				Dprintf("MakeWindow::Input Window Can't make already existing\n");			
				return(0);
			}
			//모달윈도우가 존재한다면  입력윈도우 이외에는 아무것도 올수없다.
            //지금 만들윈도우가 모달윈도우인데 기존에 모달이 있으면 생성실패를 알린다.
			if(mode==MODAL_WINDOW && gp_WinManager->ModalWindow_No != 0)
			{
				Dprintf("MakeWindow::Modal Window Can't make already existing\n");			
				return(0);
			}

            //지금만들윈도우가 모달이면 관리자구조체에 등록한다.
			if(mode==MODAL_WINDOW && gp_WinManager->ModalWindow_No == 0)
			{
				gp_WinManager->ModalWindow_No = handle;
				if(show==SHOW) gp_WinManager->Modal_Show = SHOW;
				//Dprintf("MakeWindow::MODAL_WINDOW Handle=%d\n", handle);			
			}
            //지금만들윈도우가 입력모달이면 관리자구조체에 등록한다.
			if(mode==INPUT_WINDOW && gp_WinManager->InputWindow_No == 0)
			{
				gp_WinManager->InputWindow_No = handle;
				if(show==SHOW) gp_WinManager->Input_Show = SHOW;
				//Dprintf("MakeWindow::INPUT_WINDOW Handle=%d mode=%d\n", handle, mode);			
			}		
			//여기부터는 일반윈도우던 모달이던 입력이던 윈도우를 생성하게된다.
			switch(handle) //핸들은 비어있는 핸들을 할당 받았다.
			{
				case 1:	window = gp_ContainerWindow_1; break;
				case 2:	window = gp_ContainerWindow_2; break;
				case 3:	window = gp_ContainerWindow_3; break;
				case 4:	window = gp_ContainerWindow_4; break;
				case 5:	window = gp_ContainerWindow_5; break;
                default: Dprintf("makeWindow::Handle Find Error\n"); //이값이라면 애초에 진입을 하지 않지만 관례상기입한다.
			}
			memset(window, 0, sizeof(Windows));
			window->Parent = NULL;
			window->Handle = handle;
			window->Mode = mode;		

            //==================================================================================================================
            //여기가 문제이다. Z 오다를 할당을 잘못하고 있다.
            //핸들은 만들어진 순서와 상관이 없다. 1번 핸들이 오더가 1순위는 아니다.
            //win->은 임시로 사용하는 변수이다.
            //제일나중에 만들어지는 윈도우가 제일 1순위를 갖는다.
            
            //지금만들려는 윈도우가 모달또는 입력윈도우일경우는 입력윈도우는 최상위윈도우 이므로
            //MODAL 윈도우를 생성할때는 (입력윈도우가 없을경우만 진입하므로 같은코드를 사용)
            if(mode==INPUT_WINDOW || mode==MODAL_WINDOW)
            {
                window->Zorder = 1;         //Z Order 를 1순위로 할당하고
                
                //win_cnt = GetWindowCount();	//  현재 활성화된 총컨테이너 윈도우의 수를 구한다. 			
               
                for(i=1; i<=5; i++) //윈도우를 5개를 모두검사해서 활성화된 윈도우의 Z Order를 1씩 증가시킨다.
                {
                    if(gp_WinManager->ContainerWin[i] == true)
                    {
                        win = FindContainerChain(i, "makeWindow"); //주어진 번호의 윈도우핸들을 리턴한다.
                        win->Zorder++;
                    }
                }
            }
            else    //일반 Modalless Window일경우는 입력이나 모달이 있을경우는 1, 2 번은 사용하므로
            {       //다른 모달리스를 조사해서 Z 값을 1씩증가하고 제일낮은값을 지금만드는 윈도우에 할당한다.
                window->Zorder = 1;             //일단 1순위로 할당후
                
                
                for(i=1; i<=5; i++)             //윈도우를 5개를 모두검사해서 
                {
                    if(gp_WinManager->ContainerWin[i] == true) // 활성화된 윈도우를 찾으면
                    {
                        win = FindContainerChain(i, "makeWindow"); //주어진 번호의 윈도우핸들을 리턴한다.                        
                        
                        if(win->Mode == MODAL_WINDOW)   //모달이 있을경우는 순위가 1개 밀리고
                        {
                           window->Zorder++; 
                        }                        
                        else if(win->Mode == INPUT_WINDOW)  //입력윈도우가 있어도 순위가 1개 더 밀린다.
                        {
                           window->Zorder++; 
                        }
                        else if(win->Mode == MODALLESS_WINDOW) //일반 윈도우일경우는 기존값을 1개 증가시킨다.
                        {
                            win = FindContainerChain(i, "makeWindow"); //주어진 번호의 윈도우핸들을 리턴한다.
                            win->Zorder++;
                        }
                    }
                }                
                
            }
//Dprintf("MakeWindow Container::P=%08x[%dbyte] handle=%d Zorder=%d\n",window, sizeof(Windows), window->Handle, window->Zorder);
            
            //==================================================================================================================

			gp_WinManager->ContainerWin[handle] = true;	//윈도우가 생성되었음을 체크한다.
			gp_WinManager->WidgetCount[handle] = 0;		//재확인용(새로만들어 졌기때문에 위젯은 다시 시작한다)
			
            window->abs_sx = SCREEN_SX + sx;
			window->abs_sy = SCREEN_SY + sy;
			window->abs_ex = window->abs_sx + width;
			window->abs_ey = window->abs_sy + height;
			window->Kind = CONTAINER_WINDOW;
			window->MainFunction = ContainerMSGParsing;	
			//입력윈도우일경우는 번호를 저장해둔다.
			if(mode==INPUT_WINDOW)
			{
				gp_WinManager->InputWindow_No = handle;
			}
            //Dprintf("MakeWindow::sx=%d sy=%d ex=%d ey=%d\n",window->abs_sx, window->abs_sy, window->abs_ex, window->abs_ey);
		}
		window->BackColor = param;
	}
	else if(kind==WIDGET_WINDOW)
	{
		if(screen==LCD_ALPHA)
		{
			window = (Windows *)Malloc(6, sizeof(Windows));
			if(window==NULL)
			{
				Dprintf("MakeWindow::Widget Windows Create Error\n");
				return(INVALID_DATA);
			}
			memset(window, 0, sizeof(Windows));

			//부모윈도우의 위젯의 마지막을 찾는다.
			LastWindow = FindNullWindowChain(6);
			//Dprintf("\tMakeWindow::Widget LastWin=%08x\n", LastWindow);
			
			if(window != LastWindow)//처음만들어지는 윈도우가 아니라면 그전윈도우의 다음을 지금것으로 연결			
				LastWindow->NextWindow = window;
			else	//처음만들어지는 경우일때는
				window->PreviousWindow = NULL;
				
			window->NextWindow = (void *)NULL;	//지금만들어진 윈도우의 다음은 없으므로 NULL 지정
			window->PreviousWindow = LastWindow; //지금만들어진 윈도우의 전 윈도우를 등록한다.

			window->Parent = 6;		 
			window->Handle = gp_WinManager->Handle++;
			window->Zorder = gp_WinManager->LcdAlphaWidgetCount++;
			gp_WinManager->LcdAlphaWidgetCount++;
			
			ParentWindow = gp_LcdAlpha_ContainerWindow;

			window->abs_sx = ParentWindow->abs_sx + sx;
			window->abs_sy = ParentWindow->abs_sy + sy;
			window->abs_ex = window->abs_sx + width;
			window->abs_ey = window->abs_sy + height;
			window->Kind = WIDGET_WINDOW;
			window->MainFunction = WidgetMSGParsing;
			//Dprintf("\tMakeWindow::handle=%d abs_sx=%d abs_sy=%d abs_ex=%d abs_ey=%d\n", window->Handle, window->abs_sx, window->abs_sy, window->abs_ex, window->abs_ey);
		}
		else if(screen==VIDEO_ALPHA)
		{
			window = (Windows *)Malloc(7, sizeof(Windows));
			if(window==NULL)
			{
				Dprintf("MakeWindow::Widget Windows Create Error\n");
				return(INVALID_DATA);
			}		
			memset(window, 0, sizeof(Windows));
			//부모윈도우의 위젯의 마지막을 찾는다.
			LastWindow = FindNullWindowChain(7);
			//Dprintf("\tMakeWindow::Widget LastWin=%08x\n", LastWindow);
			
			if(window != LastWindow)//처음만들어지는 윈도우가 아니라면 그전윈도우의 다음을 지금것으로 연결			
				LastWindow->NextWindow = window;
			else	//처음만들어지는 경우일때는
				window->PreviousWindow = NULL;
				
			window->NextWindow = (void *)NULL;	//지금만들어진 윈도우의 다음은 없으므로 NULL 지정
			window->PreviousWindow = LastWindow; //지금만들어진 윈도우의 전 윈도우를 등록한다.

			window->Parent = 7;		 
			window->Handle = gp_WinManager->Handle++;
			window->Zorder = gp_WinManager->LcdAlphaWidgetCount++;
			gp_WinManager->LcdAlphaWidgetCount++;
			
			ParentWindow = gp_LcdAlpha_ContainerWindow;

			window->abs_sx = ParentWindow->abs_sx + sx;
			window->abs_sy = ParentWindow->abs_sy + sy;
			window->abs_ex = window->abs_sx + width;
			window->abs_ey = window->abs_sy + height;
			window->Kind = WIDGET_WINDOW;
			window->MainFunction = WidgetMSGParsing;
			//Dprintf("\tMakeWindow::handle=%d abs_sx=%d abs_sy=%d abs_ex=%d abs_ey=%d\n", window->Handle, window->abs_sx, window->abs_sy, window->abs_ex, window->abs_ey);
		}
		else if(screen==LCD)
		{
			//Dprintf("\tMakeWindow::Widget Window Create handle=%d \n", ParentHandle);
			window = (Windows *)Malloc(ParentHandle, sizeof(Windows));
			if(window==NULL)
			{
				Dprintf("MakeWindow::Widget Windows Create Error\n");
				return(INVALID_DATA);
			}
			//Dprintf("\tMakeWindow::Widget Window Malloc=%08x\n", window);
			memset(window, 0, sizeof(Windows));

			//부모윈도우의 위젯의 마지막을 찾는다.
			LastWindow = FindNullWindowChain(ParentHandle);
			//Dprintf("\tMakeWindow::Widget LastWin=%08x\n", LastWindow);
			
			if(window != LastWindow)//처음만들어지는 윈도우가 아니라면 그전윈도우의 다음을 지금것으로 연결			
			{
				LastWindow->NextWindow = window;
				window->PreviousWindow = LastWindow; //지금만들어진 윈도우의 전 윈도우를 등록한다.
			}
			else	//처음만들어지는 경우일때는
			{
				window->PreviousWindow = NULL;
			}
				
			window->NextWindow = (void *)NULL;	//지금만들어진 윈도우의 다음은 없으므로 NULL 지정

			window->Parent = ParentHandle;		 
			window->Handle = gp_WinManager->Handle++;
			window->Zorder = gp_WinManager->WidgetCount[ParentHandle]++;
			gp_WinManager->WidgetCount[ParentHandle]++;
			switch(ParentHandle)
			{
				case 1:	ParentWindow = gp_ContainerWindow_1; break;
				case 2:	ParentWindow = gp_ContainerWindow_2; break;
				case 3:	ParentWindow = gp_ContainerWindow_3; break;
				case 4:	ParentWindow = gp_ContainerWindow_4; break;
				case 5:	ParentWindow = gp_ContainerWindow_5; break;
			}
			window->abs_sx = ParentWindow->abs_sx + sx;
			window->abs_sy = ParentWindow->abs_sy + sy;
			window->abs_ex = window->abs_sx + width;
			window->abs_ey = window->abs_sy + height;
			window->Kind = WIDGET_WINDOW;
			window->MainFunction = WidgetMSGParsing;
			//Dprintf("\tMakeWindow::handle=%d abs_sx=%d abs_sy=%d abs_ex=%d abs_ey=%d\n", window->Handle, window->abs_sx, window->abs_sy, window->abs_ex, window->abs_ey);
		}
	}	
	//추가속성을 대입한다.
	window->BackColor = wid_struct->BackColor;
	window->FontColor = wid_struct->FontColor;	
	window->Position = wid_struct->Position;
	window->BarColor = wid_struct->BarColor;
	window->WidgetStyle = wid_struct->Style;
	window->Toggle = wid_struct->Toggle;
	window->ID = wid_struct->ID;
	window->FontNo = wid_struct->FontNo;
	window->Attr = wid_struct->Attr;	//속성

	//strcpy(window->FontName, wid_struct->FontName);
	window->FontGap = wid_struct->FontGap;
	window->FontSpaceGap = wid_struct->FontSpaceGap;


    //이스타일 에서 폰트넘버에서 에러가 발생해서 수정한다.
	if(window->Style==XP_STYLE) 
        window->FontNo = 2;
    else
        window->FontNo = wid_struct->FontNo;
	//입력윈도우의경우 입력윈도우를 호출한 위젯의 아이디를 저장한다.
	window->CallWidget_ID = wid_struct->CallWidget_ID;
	window->TextAlignMode = wid_struct->TextAlignMode;
	
	if(strlen(NormalName))
		strcpy(window->NormalName, NormalName);	//BMP
	if(strlen(TouchName))
		strcpy(window->TouchName, TouchName);	//
	if(strlen(wid_struct->IconName))
		strcpy(window->IconName, wid_struct->IconName);	//ICON
	
	if(strlen(text) < 128)
		strcpy(window->Text, text);
	if(strlen(name) < 64)
		strcpy(window->Name, name);
	
	window->Style = style;
	window->sx = sx;
	window->sy = sy;
	window->width = width;
	window->height = height;
	//
	window->client_sx = sx;
	window->client_sy = sy;
	window->client_width = width;
	window->client_height = height;
	
	window->client_abs_sx = window->abs_sx;
	window->client_abs_sy = window->abs_sy;
	window->client_abs_ex = window->abs_ex;
	window->client_abs_ey = window->abs_ey;
	//
	if(window->Style==XP_STYLE)
        window->StyleDrawEvent = XPStyleEvent;
    else if(window->Style==TEECO_STYLE)
        window->StyleDrawEvent = TeecoStyleEvent;
        
	window->CreateEvent = CreateEvent;
	window->PaintEvent = PaintEvent;
	window->MouseOnEvent = MouseOnEvent;
	window->MouseOffEvent = MouseOffEvent;
	window->MouseMoveEvent = MouseMoveEvent;
	window->GestureEvent = GestureEvent;
	window->ClickEvent = ClickEvent;
	window->TimerEvent = TimerEvent;
	window->ControlEvent = ControlEvent;
	window->ChangeEvent = ChangeEvent;

	window->KeyEvent = KeyEvent;
	window->EncoderEvent = EncoderEvent;
	window->FocusEvent = FocusEvent;
	window->CloseEvent = CloseEvent;

    window->ShowStatus=show;
	window->Param=param;
	
//Dprintf("MakeWindow::sx=%d sy=%d width=%d height=%d\n", window->sx, window->sy, window->width, window->height);
//Dprintf("MakeWindow::abs_sx=%d abs_sy=%d abs_ex=%d abs_ey=%d\n", window->abs_sx, window->abs_sy, window->abs_ex, window->abs_ey);
//Dprintf("MakeWindow::client->sx=%d client_sy=%d client_width=%d client_height=%d\n", window->client_sx, window->client_sy, window->client_width, window->client_height);
//Dprintf("MakeWindow::client->abs_sx=%d client->abs_sy=%d client->abs_ex=%d client->abs_ey=%d\n", window->client_abs_sx, window->client_abs_sy, window->client_abs_ex, window->client_abs_ey);
	
	if(window->CreateEvent != NULL) 
	{
		//Create 에서 위젯을 만들경우는 미리 좌표계산이 이루어져야한다.
		if(window->Style==XP_STYLE || window->Style==TEECO_STYLE) 
		{
			window->client_sx = window->sx + WS_LINE_WIDTH;
 			window->client_sy = window->sy + (WS_TITLE_HEIGHT);
			
 			window->client_width = window->width - (WS_LINE_WIDTH * 2);
 			window->client_height= window->height - (WS_TITLE_HEIGHT+WS_LINE_WIDTH);

 			window->client_abs_sx = SCREEN_SX + window->client_sx;			
 			window->client_abs_sy = SCREEN_SY + window->client_sy;
			
 			window->client_abs_ex = window->client_abs_sx + window->client_width;
 			window->client_abs_ey = window->client_abs_sy + window->client_height;
		}
		//먼저그리면 깜박임은 없어진다.
		//차후문제의 소지가 있을수 있다.
		//메모리 버튼에서 문제가 발생한다.
		//window->PaintEvent(ParentHandle, window->Handle, 0, 0, 0);
		window->CreateEvent(ParentHandle, window->Handle, 0, 0, 0);
	}
	if(window->PaintEvent != NULL) 
	{
		//CreateEvent 를 만들지 않은 사용자를 위해 계산을 행한다.
		if(window->CreateEvent == NULL) 
		{
			if(window->Style==XP_STYLE || window->Style==TEECO_STYLE) 
			{
				window->client_sx = window->sx + WS_LINE_WIDTH;
				window->client_sy = window->sy + (WS_TITLE_HEIGHT);
				
				window->client_width = window->width - (WS_LINE_WIDTH * 2);
				window->client_height= window->height - (WS_TITLE_HEIGHT+WS_LINE_WIDTH);

				window->client_abs_sx = SCREEN_SX + window->client_sx;			
				window->client_abs_sy = SCREEN_SY + window->client_sy;
				
				window->client_abs_ex = window->client_abs_sx + window->client_width;
				window->client_abs_ey = window->client_abs_sy + window->client_height;
			}		
		}
		if(window->Style==XP_STYLE || window->Style==TEECO_STYLE)
		{
			window->StyleDrawEvent(ParentHandle, window->Handle, 0, param, window->Text);
			
			//Dprintf("\nMakeWindow::client->sx=%d client_sy=%d client_width=%d client_height=%d\n", window->client_sx, window->client_sy, window->client_width, window->client_height);
			//Dprintf("MakeWindow::client->abs_sx=%d client->abs_sy=%d client->abs_ex=%d client->abs_ey=%d\n", window->client_abs_sx, window->client_abs_sy, window->client_abs_ex, window->client_abs_ey);
		}
		window->PaintEvent(ParentHandle, window->Handle, 0, 0, 0);
	}	
	
	
	return(window->Handle);
}

int	 TeecoStyleEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int	    inColor = RGB565CONVERT(122, 167, 250);

	Windows *window = GetWindow(parent, handle, "TeecoStyleEvent");
	Graphics *Gr = GetGraphics(parent, handle, WINDOW_AREA); 

    if(window == NULL)
    {
        Dprintf("TeecoStyleEvent:: windows Error\n");
        return(0);
    }
    if(Gr == NULL)
    {
        Dprintf("TeecoStyleEvent:: Grpahics Error\n");
        return(0);
    }
	
	Gr->FillRect(0, 0, 0, 0);

	//좌측세로 
    /*
	x=0;y=0;w=width;h=height;
	Gr->SetColor(outColor);
	Gr->Line(x,y,x,h);
	Gr->Line(++x,y,x,h);
	Gr->SetColor(inColor);
	Gr->Line(++x,y,x,h);
	Gr->Line(++x,y,x,h);

	//우측세로 
	x=0;y=0;w=width;h=height;
	Gr->SetColor(outColor);
	Gr->Line(w,y,w,h);
	Gr->Line(--w,y,w,h);
	Gr->SetColor(inColor);
	Gr->Line(--w,y,w,h);
	Gr->Line(--w,y,w,h);
    
	//하단  
	x=2; y=0; w=width-2; h=height;
	Gr->SetColor(outColor);
	Gr->Line(x,h,w,h);
	Gr->Line(x,--h,w,h);
	Gr->SetColor(inColor);
	Gr->Line(x,--h,w,h);
	Gr->Line(x,--h,w,h);

	//상단  
	x=2; y=0;w=width-2; h=height;
	Gr->SetColor(outColor);
	Gr->Line(x,y,w,y);
	Gr->Line(x,++y,w,y);
    */
#if 1	
    Gr->BmpDisplay(0, 0, "ws_caption_blue_s.bmp");          //100 X 32
    Gr->BmpDisplay(100, 0, "ws_caption_blue_s.bmp");          //100 X 32
    Gr->BmpDisplay(200, 0, "ws_caption_blue_m.bmp");        //50 X 32
    Gr->BmpDisplay(250, 0, "ws_caption_blue.bmp");          //100 X 32
    Gr->BmpDisplay(350, 0, "ws_caption_blue.bmp");          //100 X 32
    Gr->BmpDisplay(450, 0, "ws_caption_blue.bmp");          //100 X 32
    Gr->BmpDisplay(550, 0, "ws_caption_blue.bmp");          //100 X 32
    Gr->BmpDisplay(650, 0, "ws_caption_blue.bmp");          //100 X 32
    Gr->BmpDisplay(750, 0, "ws_caption_blue.bmp");          //100 X 32
    Gr->BmpDisplay(850, 0, "ws_caption_blue.bmp");          //100 X 32
    Gr->BmpDisplay(1024-100, 0, "ws_caption_blue.bmp");      //100 X 32
    
	//최상단에 2줄그린다.
	Gr->SetColor(inColor);
	//Gr->Line(x,++y,w,y);
	//Gr->Line(x,++y,w,y);

	//Gr->SetFont(2);
	//Gr->SetFontGap(4);
	//Gr->SetFontSpaceGap(7);
	//Gr->SetFontColor(WHITE);
    Gr->LowTextOut(24, 6, window->Text, clWhite,  fTimeRoma16);
#endif	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	 XPStyleEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int	width, height;
	int	inColor = RGB565CONVERT(122, 167, 250);
	int	outColor = RGB565CONVERT(9, 52, 128);
	int	x,y,w,h,i;
	int r=9,g=52,b=128;
	//char *Text = (char *)Data;

	Windows *window = GetWindow(parent, handle, "XPStyleEvent");
	Graphics *Gr = GetGraphics(parent, handle, WINDOW_AREA); 

//Dprintf("XPStyleEvent::Entry\n");

    if(window == NULL)
    {
        Dprintf("XPStyleEvent:: windows Error\n");
        return(0);
    }
    if(Gr == NULL)
    {
        Dprintf("XPStyleEvent:: Grpahics Error\n");
        return(0);
    }
	width = window->width;
	height = window->height;

	//Gr->SetColor(RED);
	Gr->FillRect(0, 0, 0, 0);
//Dprintf("XPStyleEvent::Entry-----------1\n");

	//좌측세로 
	x=0;y=0;w=width;h=height;
	Gr->SetColor(outColor);
	Gr->Line(x,y,x,h);
	Gr->Line(++x,y,x,h);
	Gr->SetColor(inColor);
	Gr->Line(++x,y,x,h);
	Gr->Line(++x,y,x,h);
//Dprintf("XPStyleEvent::Entry-----------2\n");

	//우측세로 
	x=0;y=0;w=width;h=height;
	Gr->SetColor(outColor);
	Gr->Line(w,y,w,h);
	Gr->Line(--w,y,w,h);
	Gr->SetColor(inColor);
	Gr->Line(--w,y,w,h);
	Gr->Line(--w,y,w,h);
//Dprintf("XPStyleEvent::Entry-----------3\n");
    
	//하단  
	x=2; y=0; w=width-2; h=height;
	Gr->SetColor(outColor);
	Gr->Line(x,h,w,h);
	Gr->Line(x,--h,w,h);
	Gr->SetColor(inColor);
	Gr->Line(x,--h,w,h);
	Gr->Line(x,--h,w,h);
#if 1
	//상단  
	x=2; y=0;w=width-2; h=height;
	Gr->SetColor(outColor);
	Gr->Line(x,y,w,y);
	Gr->Line(x,++y,w,y);
	for( i=0; i< WS_TITLE_HEIGHT-4; i++ )
	{
		r+=3;g+=3;b+=3;
		Gr->SetColor( RGB565CONVERT(r,g,b) );
		Gr->Line(x,++y,w,y);
	}
#endif	
	//최상단에 2줄그린다.
	Gr->SetColor(inColor);
	Gr->Line(x,++y,w,y);
	Gr->Line(x,++y,w,y);
//Dprintf("XPStyleEvent::Entry-----------4\n");

	//작업영역 
	//Gr->SetColor((U16)param);
	//x=0;y=0;w=width;h=height;	
	//Gr->FillRect(x+5,y+(WS_TITLE_HEIGHT+1),w-9,h-(WS_TITLE_HEIGHT+5));
	
	//Gr->LoadFont("SystemFontGL8.fnt");
//Dprintf("Windows:: Font_No=%d\n", window->FontNo);
//Dprintf("XPStyleEvent::FontNo=%d\n", 2);
	//Gr->SetFont(2);
//Dprintf("XPStyleEvent::Entry-----------4.1\n");
	//Gr->SetFontGap(4);
//Dprintf("XPStyleEvent::Entry-----------4.2\n");
	//Gr->SetFontSpaceGap(7);
//Dprintf("XPStyleEvent::Entry-----------4.3\n");
	//Gr->SetFontColor(WHITE);
//Dprintf("XPStyleEvent::Entry-----------4.4\n");
	//Gr->TextOut(50, 8, window->Text);
	
	SetFontAliasing("ng_eh_24.sif");
	Gr->TextOutAliasing(10, 10, window->Text, clWhite, 0, "ng_eh_24.sif");
	
//Dprintf("XPStyleEvent::Entry-----------5 %s\n", window->Text);
	
	Gr->Draw(parent, handle, command, param, Data);	
//Dprintf("XPStyleEvent::Entry-----------6\n");
	return(1);
}

int PaintDraw(HANDLE parent, HANDLE handle, U32 command, U32 para, void *data)
{
	Windows	*window;
	Windows	*widget;
	Windows	*temp_window;
	//HANDLE	Parent, Handle;

	Windows	*window_Zorder1=NULL;
	Windows	*window_Zorder2=NULL;
	Windows	*window_Zorder3=NULL;
	Windows	*window_Zorder4=NULL;
	Windows	*window_Zorder5=NULL;

	int		win_cnt;
	int		hidden_cnt;
	int		i, bitblt_loop;

	if(gPaintDrawUse==OFF) return(1);
	
	for(i=1; i <=5; i++)	// 컨테이너윈도우 5개를 검사하는데
	{
		temp_window = FindContainerWindow(i, "PaintDraw-1");
		if(temp_window != NULL)
		{
			if(gp_WinManager->ContainerWin[i]==true) // 해당윈도우가 메모리에 있으면
			{
				if(temp_window->Zorder==1 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder1 = FindContainerChain(i, "PaintDraw-2");
//Dprintf("Search1::win->Zorder = %d Show=%d handle=%d %d\n", window_Zorder1->Zorder, window_Zorder1->ShowStatus, window_Zorder1->Parent, window_Zorder1->Handle);		
                    
				}
				if(temp_window->Zorder==2 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder2 = FindContainerChain(i, "PaintDraw-3");
//Dprintf("Search2::win->Zorder = %d Show=%d handle=%d %d\n", window_Zorder2->Zorder, window_Zorder2->ShowStatus, window_Zorder2->Parent, window_Zorder2->Handle);		
				}
				if(temp_window->Zorder==3 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder3 = FindContainerChain(i, "PaintDraw-4");
				}
				if(temp_window->Zorder==4 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder4 = FindContainerChain(i, "PaintDraw-5");
				}
				if(temp_window->Zorder==5 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder5 = FindContainerChain(i, "PaintDraw-6");
				}
			} //윈도우의 Z순서대로 정렬한다.
		}
	}
	
	win_cnt = GetWindowCount();	                        //총컨테이너 윈도우의 수를 구한다.
	hidden_cnt = GetHiddenWindowCount("PaintDraw::1");  //숨겨진 윈도우의 수를 구한다.
	bitblt_loop =  win_cnt-hidden_cnt;
    //Dprintf("PaintDraw::TotalWin=%d HiddenWin=%d ActiveWin =%d\n", win_cnt, hidden_cnt, bitblt_loop);
	
	//Z1 윈도우의 사이즈가 LCD 사이즈이면 Z1 하나만 출력한다.
	//속도향상을 위함이고 불필요한 시간을 줄인다.
	if(bitblt_loop >=1 ) //표시해야할 윈도우가 1개이상이 있을경우
	{
		window = GetZorderWindow(1);    //Z순서가 1번인 윈도우의 핸들을 얻는다.
        if(window == NULL)
        {
            Dprintf("PaintDraw::Zorder 1 Window Not Found\n");
            return(0);
        }
//Dprintf("sx=%d sy=%d width=%d height=%d\n", window->sx, window->sy, window->width,         
        //Z 이 1인 윈도우가 전화면을 차지하고 잇을경우에는 이화면만 출력하고 종료한다.
		if(window->sx == 0 && window->sy==0 && window->width==LCD_X && window->height==LCD_Y)
		{			
//Dprintf("PaintDraw::One Window\n");
			//window = GetZorderWindow(1);	
			//여기서는 window핸들의 스크린중 컨테이너와 위젯을 더즐스크린에 비트빌트시킨다.
            WindowBitBlt(window, 1);
            
            //더블스크린에 옮겨진 화면을 눈에보이는 스크린으로 이동시킨다.
			BitBlt(	DOUBLE_SCREEN, 				// Source Screen
					LCD_SCREEN, 				// Dest Screen
					0, 
					0,			// Source Position
					0, 
					0,			// Dest Position
					LCD_X,	
					LCD_Y);				
            //알파나 비디오가 활성중이면 알파나 비디오를 출력해야한다.
            if(gp_WinManager->VideoAlphaContainerWin!=true && 	gp_WinManager->LcdAlphaContainerWin!=true)
            {
                return(1);			
            }
            else
            {
                bitblt_loop=0;  //아래의 코드를 수행하지 않기위해 클리어한다.
            }
		}
	}
    //Z order가 1번인 화면이 풀화면을 차지 하지 않고 있다면 전체윈도우를 비트빌트한다.
    //2번, 3번 순으로 풀화면인지를 검사하는것이 좋은방법일것 같다.
	switch(bitblt_loop)	//히든윈도우를 제외하고 활성윈도우를 화면고속이동한다.
	{
		case 5: window = window_Zorder5;	WindowBitBlt(window_Zorder5, bitblt_loop); 
		//Dprintf("PaintDraw---5 Z=%d\n", window->Zorder);
		case 4: window = window_Zorder4;	WindowBitBlt(window_Zorder4, bitblt_loop);
		//Dprintf("PaintDraw---4 Z=%d\n", window->Zorder);
		case 3: window = window_Zorder3;	WindowBitBlt(window_Zorder3, bitblt_loop);
		//Dprintf("PaintDraw---3 Z=%d\n", window->Zorder);
		case 2: 
		{
			window = window_Zorder2;	
		//Dprintf("PaintDraw---2 Z=%d\n", window->Zorder);
//Dprintf("WinCnt=2:PaintDraw::win->Zorder = %d Show=%d handle=%d %d\n", window_Zorder2->Zorder, window_Zorder2->ShowStatus, window_Zorder2->Parent, window_Zorder2->Handle);		
			WindowBitBlt(window_Zorder2, bitblt_loop);
		}
		case 1: 
		{
			window = window_Zorder1;	
		//Dprintf("PaintDraw---1 Z=%d\n", window->Zorder);
//Dprintf("WinCnt=1:PaintDraw::win->Zorder = %d Show=%d handle=%d %d\n", window_Zorder1->Zorder, window_Zorder1->ShowStatus, window_Zorder1->Parent, window_Zorder1->Handle);		
			WindowBitBlt(window_Zorder1, bitblt_loop);
		}
		
        //위에서 통합된 더블스크린을 보이는 스크린으로 전송한다.
		BitBlt(	DOUBLE_SCREEN, 				// Source Screen
				LCD_SCREEN, 					// Dest Screen
				0, 
				0,			// Source Position
				0, 
				0,			// Dest Position
				LCD_X,	
				LCD_Y);		
		
	}
/*
	//비디오 알파가 온되어 있으면 비디오알파를 전송한다.
	if(gp_WinManager->VideoAlphaContainerWin==true)
	{
		BitBlt(	VIDEO_ALPHA_CONTAINER_SCREEN, 				// Source Screen
				VIDEO_ALPHA_DOUBLE_SCREEN, 					// Dest Screen
				gp_VideoAlpha_ContainerWindow->abs_sx, 
				gp_VideoAlpha_ContainerWindow->abs_sy,			// Source Position
				gp_VideoAlpha_ContainerWindow->abs_sx, 
				gp_VideoAlpha_ContainerWindow->abs_sy,			// Dest Position
				gp_VideoAlpha_ContainerWindow->width,	
				gp_VideoAlpha_ContainerWindow->height);

		if(gp_WinManager->VideoAlphaWidgetCount != 0) //컨테이너1 의 위젯이 존재하면
		{
			//Dprintf("WindowBitBlt:: CONTAINER_WIN1--Widget Bitblt Start\n");
			// 컨테이너의 위젯윈도우를 더블버퍼로 전송한다.
			widget = gp_VideoAlphaWidgetWindow;			//위젯 저장소의 첫번째 포인터
			while(1)
			{
				//Dprintf("WindowBitBlt:: abs_sx=%d abs_sy=%d width=%d height=%d\n",widget->abs_sx, widget->abs_sy, widget->width, widget->height);

				// 위젯윈도우를 전송한다.
				BitBlt(	VIDEO_ALPHA_WIDGET_SCREEN, 				// Source Screen
						VIDEO_ALPHA_DOUBLE_SCREEN, 					// Dest Screen
						widget->abs_sx, widget->abs_sy,		// Source Position
						widget->abs_sx, widget->abs_sy,		// Dest Position
						widget->width,	 					//
						widget->height);
				if(widget->NextWindow == NULL) break;
				widget = (Windows *)widget->NextWindow;
			}
		}	
		BitBlt(	VIDEO_ALPHA_DOUBLE_SCREEN, 				// Source Screen
				VIDEO_ALPHA_SCREEN, 					// Dest Screen
				0, 
				0,			// Source Position
				0, 
				0,			// Dest Position
				LCD_X,	
				LCD_Y);			
	}
*/	
	//LCD 알파가 온되어 있으면 LCD알파를 전송한다.
	if(gp_WinManager->LcdAlphaContainerWin==true)
	{
		BitBlt(	LCD_ALPHA_CONTAINER_SCREEN, 				// Source Screen
				LCD_ALPHA_DOUBLE_SCREEN, 					// Dest Screen
				gp_LcdAlpha_ContainerWindow->abs_sx, 
				gp_LcdAlpha_ContainerWindow->abs_sy,			// Source Position
				gp_LcdAlpha_ContainerWindow->abs_sx, 
				gp_LcdAlpha_ContainerWindow->abs_sy,			// Dest Position
				gp_LcdAlpha_ContainerWindow->width,	
				gp_LcdAlpha_ContainerWindow->height);

		if(gp_WinManager->LcdAlphaWidgetCount != 0) //컨테이너1 의 위젯이 존재하면
		{
			//Dprintf("WindowBitBlt:: CONTAINER_WIN1--Widget Bitblt Start\n");
			// 컨테이너의 위젯윈도우를 더블버퍼로 전송한다.
			widget = gp_LcdAlphaWidgetWindow;			//위젯 저장소의 첫번째 포인터
			while(1)
			{
				//Dprintf("WindowBitBlt:: abs_sx=%d abs_sy=%d width=%d height=%d\n",widget->abs_sx, widget->abs_sy, widget->width, widget->height);

				// 위젯윈도우를 전송한다.
				BitBlt(	LCD_ALPHA_WIDGET_SCREEN, 				// Source Screen
						LCD_ALPHA_DOUBLE_SCREEN, 					// Dest Screen
						widget->abs_sx, widget->abs_sy,		// Source Position
						widget->abs_sx, widget->abs_sy,		// Dest Position
						widget->width,	 					//
						widget->height);
				if(widget->NextWindow == NULL) break;
				widget = (Windows *)widget->NextWindow;
			}
		}
		BitBlt(	LCD_ALPHA_DOUBLE_SCREEN, 				// Source Screen
				LCD_ALPHA_SCREEN, 					// Dest Screen
				0, 
				0,			// Source Position
				0, 
				0,			// Dest Position
				LCD_X,	
				LCD_Y);			
		
	}	

    SM718_2D_Engine_Wait();
	return(1);
}

extern volatile U16 	g_Color;
//전달되는 윈도우는 컨테이너 윈도우이다.
void WindowBitBlt(Windows *container_winow, int no)
{
	Windows	*widget;
	
	
	/* 사용하지말것
	g_Color = clBlack;
	SetGraphicsPage(DOUBLE_SCREEN);
	FillRect2D(0, 0, LCD_X, LCD_Y);
	SetGraphicsPage(LCD_SCREEN);
	*/
	
	//Dprintf("TEST::Handle=%d abs_sx=%d abs_sy=%d width=%d height=%d\n", container_winow->Handle, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height);
	if(container_winow->Handle==1 && container_winow->ShowStatus==SHOW)	// CONTAINER_WINDOW_1 일경우
	{		
		//Dprintf("WindowBitBlt:: CONTAINER_WIN1 Bitblt Start\n");
		// 컨테이너윈도우1을 더블버퍼영역으로 전송한다.
		SetClip(0, 0, 0, LCD_X, LCD_Y, 1);			
		
		BitBlt(	CONTAINER_WINDOW1_SCREEN, 				// Source Screen
				DOUBLE_SCREEN, 							// Dest Screen
				container_winow->abs_sx, 
				container_winow->abs_sy,				// Source Position
				container_winow->abs_sx, 
				container_winow->abs_sy,				// Dest Position
				container_winow->width,	
				container_winow->height);
		
		if(gp_WinManager->WidgetCount[1] != 0) //컨테이너1 의 위젯이 존재하면
		{
			//Dprintf("WindowBitBlt:: CONTAINER_WIN1--Widget Bitblt Start\n");
			// 컨테이너의 위젯윈도우를 더블버퍼로 전송한다.
			widget = gp_WidgetWindow_1;			//위젯 저장소의 첫번째 포인터
            SetClip(1, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
			while(1)
			{
				//Dprintf("WindowBitBlt:: abs_sx=%d abs_sy=%d width=%d height=%d\n",widget->abs_sx, widget->abs_sy, widget->width, widget->height);

				if(widget->ShowStatus==SHOW)
				{
					// 위젯윈도우를 전송한다.
					BitBlt(	CONTAINER_WIDGET1_SCREEN, 				// Source Screen
							DOUBLE_SCREEN, 					// Dest Screen
							widget->abs_sx, widget->abs_sy,		// Source Position
							widget->abs_sx, widget->abs_sy,		// Dest Position
							widget->width,	 					//
							widget->height);
				}
				if(widget->NextWindow == NULL) break;
				widget = (Windows *)widget->NextWindow;
			}
            SetClip(0, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
		}
	}
	else if(container_winow->Handle==2 && container_winow->ShowStatus==SHOW)	// CONTAINER_WINDOW_2 일경우
	{
		//Dprintf("WindowBitBlt:: CONTAINER_WIN2 Bitblt Start\n");
		//Dprintf("sx=%d sy=%d width=%d heigt=%d\n", container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height);

		SetClip(0, 0, 0, LCD_X, LCD_Y, 1);			

		// 컨테이너윈도우2을 더블버퍼영역으로 전송한다.
		BitBlt(	CONTAINER_WINDOW2_SCREEN, 				// Source Screen
				DOUBLE_SCREEN, 					// Dest Screen
				container_winow->abs_sx, 
				container_winow->abs_sy,			// Source Position
				container_winow->abs_sx, 
				container_winow->abs_sy,			// Dest Position
				container_winow->width,	
				container_winow->height);

		
		if(gp_WinManager->WidgetCount[2] != 0) //컨테이너1 의 위젯이 존재하면
		{
			//Dprintf("WindowBitBlt:: WIDGET_WIN2 Bitblt Start\n");
			// 컨테이너의 위젯윈도우를 더블버퍼로 전송한다.
			widget = gp_WidgetWindow_2;			//위젯 저장소의 첫번째 포인터
			
            //각기의 위젯을 뿌리면 속도도 문제가 되고, 클립영역이외의것도 출력하게된다.
            
            SetClip(1, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
            
            while(1)
			{
				//Dprintf("WindowBitBlt:: abs_sx=%d abs_sy=%d width=%d height=%d\n",widget->abs_sx, widget->abs_sy, widget->width, widget->height);
				// 위젯윈도우를 전송한다.
				if(widget->ShowStatus==SHOW)
				{
					BitBlt(	CONTAINER_WIDGET2_SCREEN, 				// Source Screen
							DOUBLE_SCREEN, 					// Dest Screen
							widget->abs_sx, widget->abs_sy,		// Source Position
							widget->abs_sx, widget->abs_sy,		// Dest Position
							widget->width,	 					//
							widget->height);
				}
				if(widget->NextWindow == NULL) break;
				widget = (Windows *)widget->NextWindow;
			}
            SetClip(0, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);

		}
	}
	else if(container_winow->Handle==3 && container_winow->ShowStatus==SHOW)	// CONTAINER_WINDOW_3 일경우
	{
		//Dprintf("WindowBitBlt:: CONTAINER_WIN3 Bitblt Start\n");
		// 컨테이너윈도우3을 더블버퍼영역으로 전송한다.
		SetClip(0, 0, 0, LCD_X, LCD_Y, 1);			
		BitBlt(	CONTAINER_WINDOW3_SCREEN, 				// Source Screen
				DOUBLE_SCREEN, 					// Dest Screen
				container_winow->abs_sx, 
				container_winow->abs_sy,			// Source Position
				container_winow->abs_sx, 
				container_winow->abs_sy,			// Dest Position
				container_winow->width,	
				container_winow->height);

		if(gp_WinManager->WidgetCount[3] != 0) //컨테이너1 의 위젯이 존재하면
		{
			//Dprintf("WindowBitBlt:: WIDGET_WIN3 Bitblt Start\n");
			// 컨테이너의 위젯윈도우를 더블버퍼로 전송한다.
			widget = gp_WidgetWindow_3;			//위젯 저장소의 첫번째 포인터
            SetClip(1, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
			while(1)
			{
				if(widget->ShowStatus==SHOW)
				{
					// 위젯윈도우를 전송한다.
					BitBlt(	CONTAINER_WIDGET3_SCREEN, 				// Source Screen
							DOUBLE_SCREEN, 					// Dest Screen
							widget->abs_sx, widget->abs_sy,		// Source Position
							widget->abs_sx, widget->abs_sy,		// Dest Position
							widget->width,	 					//
							widget->height);
				}
				if(widget->NextWindow == NULL) break;
				widget = (Windows *)widget->NextWindow;
			}
            SetClip(0, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
		}
	}
	else if(container_winow->Handle==4 && container_winow->ShowStatus==SHOW)	// CONTAINER_WINDOW_4 일경우
	{
		//Dprintf("WindowBitBlt:: CONTAINER_WIN4 Bitblt Start\n");
		// 컨테이너윈도우4을 더블버퍼영역으로 전송한다.
		SetClip(0, 0, 0, LCD_X, LCD_Y, 1);			
		BitBlt(	CONTAINER_WINDOW4_SCREEN, 				// Source Screen
				DOUBLE_SCREEN, 					// Dest Screen
				container_winow->abs_sx, 
				container_winow->abs_sy,			// Source Position
				container_winow->abs_sx, 
				container_winow->abs_sy,			// Dest Position
				container_winow->width,	
				container_winow->height);

		if(gp_WinManager->WidgetCount[4] != 0) //컨테이너1 의 위젯이 존재하면
		{
			// 컨테이너의 위젯윈도우를 더블버퍼로 전송한다.
			widget = gp_WidgetWindow_4;			//위젯 저장소의 첫번째 포인터
            SetClip(1, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
			while(1)
			{
				if(widget->ShowStatus==SHOW)
				{
					// 위젯윈도우를 전송한다.
					BitBlt(	CONTAINER_WIDGET4_SCREEN, 				// Source Screen
							DOUBLE_SCREEN, 					// Dest Screen
							widget->abs_sx, widget->abs_sy,		// Source Position
							widget->abs_sx, widget->abs_sy,		// Dest Position
							widget->width,	 					//
							widget->height);
				}
				if(widget->NextWindow == NULL) break;
				widget = (Windows *)widget->NextWindow;
			}
            SetClip(0, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
		}
	}
	else if(container_winow->Handle==5 && container_winow->ShowStatus==SHOW)	// CONTAINER_WINDOW_5 일경우
	{
		//Dprintf("WindowBitBlt:: CONTAINER_WIN5 Bitblt Start\n");
		// 컨테이너윈도우5을 더블버퍼영역으로 전송한다.
		SetClip(0, 0, 0, LCD_X, LCD_Y, 1);			
		BitBlt(	CONTAINER_WINDOW5_SCREEN, 				// Source Screen
				DOUBLE_SCREEN, 					// Dest Screen
				container_winow->abs_sx, 
				container_winow->abs_sy,			// Source Position
				container_winow->abs_sx, 
				container_winow->abs_sy,			// Dest Position
				container_winow->width,	
				container_winow->height);

		if(gp_WinManager->WidgetCount[5] != 0) //컨테이너1 의 위젯이 존재하면
		{
			// 컨테이너의 위젯윈도우를 더블버퍼로 전송한다.
			widget = gp_WidgetWindow_5;			//위젯 저장소의 첫번째 포인터
            SetClip(1, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
			while(1)
			{
				if(widget->ShowStatus==SHOW)
				{
					// 위젯윈도우를 전송한다.
					BitBlt(	CONTAINER_WIDGET5_SCREEN, 				// Source Screen
							DOUBLE_SCREEN, 					// Dest Screen
							widget->abs_sx, widget->abs_sy,		// Source Position
							widget->abs_sx, widget->abs_sy,		// Dest Position
							widget->width,	 					//
							widget->height);
				}
				if(widget->NextWindow == NULL) break;
				widget = (Windows *)widget->NextWindow;
			}
            SetClip(0, container_winow->abs_sx, container_winow->abs_sy, container_winow->width, container_winow->height, 1);
		}
	}
}



/******************************************************************************
		Function name 			: GetGraphics
		Function prototype 		: Graphics *GetGraphics(HANDLE parent, HANDLE handle);
		Behavior description	: 윈도우Paint Event Use
		Input parameter			: 
		Output parameter 		: None
		Return parameter		: 
		Required preconditions	: None
		Called functions		: None
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
void GraphicsInit(void)
{
	gp_Graphics = (Graphics *)GRAPHICS_MEMORY; //0x6B803000; //GRAPHICS_MEMORY; //gp_Graphics = (Graphics *)0x6B802C00; //GRAPHICS_MEMORY;
	memset(gp_Graphics, 0, sizeof(Graphics));
}

Graphics *GetGraphics(HANDLE parent, HANDLE handle, int area)
{
	//int		sx, sy;
	Windows	*window;
	//Dprintf("GetGraphics::Start parent=%d handle=%d\n", parent, handle);
	//Dprintf(" gp_Graphics =%p address=%08x\n", gp_Graphics, GRAPHICS_MEMORY);

	//그래픽포인터를 얻는 페인트에서는 이후 모든 그래픽함수는 윈도우의 좌표가 상대적으로 0,0 처리된다.
	//All Graphics function use under position
	if(parent==NULL) //Container Windows
	{
		//Dprintf("GetGraphics::Find Container Chain Handle=%d\n", handle);
		window = FindContainerChain(handle,"GraphicsInit");
	}
	else
	{
		window = FindWidgetChain(parent, handle);
	}
    
	if(window==NULL)
	{
		Dprintf("GetGraphics:: Window Handle Error\n");
		return(0);
	}
	gp_Graphics = (Graphics *)GRAPHICS_MEMORY;//0x6B803000;
	memset(gp_Graphics, 0, sizeof(Graphics));

	gp_Graphics->Parent = parent;
	gp_Graphics->Handle = handle;

	if(area==CLIENT_AREA)
	{
		gp_Graphics->Offset_X = window->client_abs_sx; 
		gp_Graphics->Offset_Y = window->client_abs_sy;
	}
	else
	{
		gp_Graphics->Offset_X = window->abs_sx; 
		gp_Graphics->Offset_Y = window->abs_sy;
	}

	gp_Graphics->SetGraphicsPage = SetGraphicsPage;
	gp_Graphics->SetClip=SetClip;
	gp_Graphics->SetColor=SetColor;
	gp_Graphics->SetFontColor=SetFontColor;
	gp_Graphics->SetBrush=SetBrush;
	gp_Graphics->SetFont=SetFont;

	gp_Graphics->SetFontStartLine=SetFontStartLine;

	gp_Graphics->SetHangulFont=SetHangulFont;
	gp_Graphics->Plot=WinPlot;
	gp_Graphics->Line=WinLine2D;
	gp_Graphics->MoveTo=WinMoveTo;
	gp_Graphics->LineTo=WinLineTo;
	gp_Graphics->Arc=WinArc;
	gp_Graphics->Elipse=WinElipse;
	gp_Graphics->Rectangle=WinRect2D;
	gp_Graphics->RoundRect=WinRoundRect2D;
	gp_Graphics->FillElipse=WinFillElipse;
	gp_Graphics->FillRect=WinFillRect2D;
	gp_Graphics->HLine=WinHLine2D;
//	gp_Graphics->Hline=Hline;
	gp_Graphics->VLine=WinVLine2D;
//	gp_Graphics->CrossLine=CrossLine;
//	gp_Graphics->VLine2D=WinVLine2D;
//	gp_Graphics->HLine2D=WinHLine2D;
	gp_Graphics->BitBlt=PaintBitBlt;
	gp_Graphics->FontBitBlt=FontBitBlt;
	gp_Graphics->FontBitBltOne=FontBitBltOne;
	gp_Graphics->ScreenClear = ScreenClear;
	gp_Graphics->BitBltRop=PaintBitBltRop;
	gp_Graphics->Rotate=Rotate;

    if(gDMA_USE==YES)
    {
        gp_Graphics->BmpDisplay=DMABmpDisplay;//DMABmpDisplay; //PaintBmpDisplay;
    }
    else
    {
        gp_Graphics->BmpDisplay=PaintBmpDisplay;
    }
    gp_Graphics->BmpDisplay1=PaintBmpDisplay;

	gp_Graphics->BmpLoad=BmpLoad;
	gp_Graphics->TextOut=DrawText;	//!< 폰트생성기로 생성된 폰트를 뿌릴때 사용한다. 
#ifdef LOW_FONT_USE	
    gp_Graphics->LowTextOut=LowTextOut; //!< 내장된 폰트를 사용한다.
#endif	
	//gp_Graphics->CTextOut = ClearTextOut;  //!< 클리어폰트를 출력하는 함수이다.
    gp_Graphics->TextOutAliasing=TextOutAliasing; //!< 내장된 폰트를 사용한다.

	//gp_Graphics->HangulOut = HangulOut;  //!< Textout에서 한글은 자동출력되므로 제거한다.
	gp_Graphics->Draw = PaintDraw;
	gp_Graphics->SetFontGap = SetFontGap;
	gp_Graphics->SetFontSpaceGap = SetFontSpaceGap;
	gp_Graphics->LoadFont = LoadFont;
	gp_Graphics->IconDisplay = IconDisplay;

	if(parent==NULL)	// Select Container Screen
	{
		switch(handle)
		{
			case 1:	
			{
				SetGraphicsPage(CONTAINER_WINDOW1_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW1_SCREEN;
				//Dprintf("GRAPHICS ADDRESS CONTAINER--1 ADDR=%08x\n", CONTAINER_WINDOW1_SCREEN);
				break;
			}
			case 2:
			{
				SetGraphicsPage(CONTAINER_WINDOW2_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW2_SCREEN;
				//Dprintf("GRAPHICS ADDRESS CONTAINER--2 ADDR=%08x\n", CONTAINER_WINDOW2_SCREEN);
				break;
			}
			case 3:	SetGraphicsPage(CONTAINER_WINDOW3_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW3_SCREEN;
			break;
			case 4:	SetGraphicsPage(CONTAINER_WINDOW4_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW4_SCREEN;
			break;
			case 5:	SetGraphicsPage(CONTAINER_WINDOW5_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW5_SCREEN;
			break;
#ifdef ALPHA_USE
            //LCD Alpha 스크린영역이다.
			case 6:	SetGraphicsPage(LCD_ALPHA_CONTAINER_SCREEN); 
				gp_Graphics->Page = LCD_ALPHA_CONTAINER_SCREEN;
			break;
#endif
		}
	}
	else 	// Select Widget Screen 
	{
		switch(parent)
		{
			case 1:	
			{
				SetGraphicsPage(CONTAINER_WIDGET1_SCREEN); 
				gp_Graphics->Page = CONTAINER_WIDGET1_SCREEN;
				//Dprintf("GRAPHICS ADDRESS WIDGET--1 ADDR=%08x\n", CONTAINER_WIDGET1_SCREEN);
				break;
			}
			case 2:	
			{
				SetGraphicsPage(CONTAINER_WIDGET2_SCREEN); 
				//Dprintf("GRAPHICS ADDRESS WIDGET--2 ADDR=%08x\n", CONTAINER_WIDGET1_SCREEN);
				gp_Graphics->Page = CONTAINER_WIDGET2_SCREEN;
				break;
			}
			case 3:	SetGraphicsPage(CONTAINER_WIDGET3_SCREEN); 
				gp_Graphics->Page = CONTAINER_WIDGET3_SCREEN;
			break;
			case 4:	SetGraphicsPage(CONTAINER_WIDGET4_SCREEN); 
				gp_Graphics->Page = CONTAINER_WIDGET4_SCREEN;
			break;
			case 5:	SetGraphicsPage(CONTAINER_WIDGET5_SCREEN); 
				gp_Graphics->Page = CONTAINER_WIDGET5_SCREEN;			
			break;
#ifdef ALPHA_USE
			case 6:	SetGraphicsPage(LCD_ALPHA_WIDGET_SCREEN); 
				gp_Graphics->Page = LCD_ALPHA_WIDGET_SCREEN;			
			break;
#endif
		}		
	}
	//해당되는 윈도우 영역의 클립영역을 설정하여 그래픽의 오버가 발생하지 않게한다.
	if(area==CLIENT_AREA)
	{	
		SetClip(1, window->client_abs_sx, window->client_abs_sy, window->client_width, window->client_height, 1);
		//Dprintf("\nGetGraphics::client->sx=%d client_sy=%d client_width=%d client_height=%d\n", window->client_sx, window->client_sy, window->client_width, window->client_height);
		//Dprintf("GetGraphics::client->abs_sx=%d client->abs_sy=%d client->abs_ex=%d client->abs_ey=%d\n", window->client_abs_sx, window->client_abs_sy, window->client_abs_ex, window->client_abs_ey);
	}
	else
	{
		SetClip(1, window->abs_sx, window->abs_sy, window->width, window->height, 1);
	}
	return(gp_Graphics);
}


Graphics *GetGraphicsExt(HANDLE parent, HANDLE handle, int area, U32 address)
{
	Windows	*window;

	//그래픽포인터를 얻는 페인트에서는 이후 모든 그래픽함수는 윈도우의 좌표가 상대적으로 0,0 처리된다.
	//All Graphics function use under position
	if(parent==NULL) //Container Windows
	{
		//Dprintf("GetGraphics::Find Container Chain Handle=%d\n", handle);
		window = FindContainerChain(handle,"GraphicsInit");
	}
	else
	{
		window = FindWidgetChain(parent, handle);
	}
    
	if(window==NULL)
	{
		Dprintf("GetGraphics:: Window Handle Error\n");
		return(0);
	}
	gp_Graphics = (Graphics *)address; //GRAPHICS_MEMORY;//0x6B803000;
	memset(gp_Graphics, 0, sizeof(Graphics));

	gp_Graphics->Parent = parent;
	gp_Graphics->Handle = handle;

	if(area==CLIENT_AREA)
	{
		gp_Graphics->Offset_X = window->client_abs_sx; 
		gp_Graphics->Offset_Y = window->client_abs_sy;
	}
	else
	{
		gp_Graphics->Offset_X = window->abs_sx; 
		gp_Graphics->Offset_Y = window->abs_sy;
	}

	gp_Graphics->SetGraphicsPage = SetGraphicsPage;
	gp_Graphics->SetClip=SetClip;
	gp_Graphics->SetColor=SetColor;
	gp_Graphics->SetFontColor=SetFontColor;
	gp_Graphics->SetBrush=SetBrush;
	gp_Graphics->SetFont=SetFont;

	gp_Graphics->SetFontStartLine=SetFontStartLine;

	gp_Graphics->SetHangulFont=SetHangulFont;
	gp_Graphics->Plot=WinPlot;
	gp_Graphics->Line=WinLine2D;
	gp_Graphics->MoveTo=WinMoveTo;
	gp_Graphics->LineTo=WinLineTo;
	gp_Graphics->Arc=WinArc;
	gp_Graphics->Elipse=WinElipse;
	gp_Graphics->Rectangle=WinRect2D;
	gp_Graphics->RoundRect=WinRoundRect2D;
	gp_Graphics->FillElipse=WinFillElipse;
	gp_Graphics->FillRect=WinFillRect2D;
//	gp_Graphics->HLine=WinHLine2D;
//	gp_Graphics->Hline=Hline;
//	gp_Graphics->VLine=WinVLine2D;
//	gp_Graphics->CrossLine=CrossLine;
//	gp_Graphics->VLine2D=WinVLine2D;
//	gp_Graphics->HLine2D=WinHLine2D;
//	gp_Graphics->Line2D=WinLine2D;
//	gp_Graphics->FillRect2D=WinFillRect2D;
	gp_Graphics->BitBlt=PaintBitBlt;
	gp_Graphics->ScreenClear = ScreenClear;
	gp_Graphics->BitBltRop=PaintBitBltRop;
	gp_Graphics->Rotate=Rotate;

    if(gDMA_USE==YES)
    {
        gp_Graphics->BmpDisplay=DMABmpDisplay;//DMABmpDisplay; //PaintBmpDisplay;
    }
    else
    {
        gp_Graphics->BmpDisplay=PaintBmpDisplay;
    }

	gp_Graphics->BmpLoad=BmpLoad;
	gp_Graphics->TextOut=DrawText;	//!< 폰트생성기로 생성된 폰트를 뿌릴때 사용한다.
#ifdef LOW_FONT_USE	
    gp_Graphics->LowTextOut=LowTextOut;	//!< 내장된 폰트를 사용한다.
#endif	
	gp_Graphics->Draw = PaintDraw;
	gp_Graphics->SetFontGap = SetFontGap;
	gp_Graphics->SetFontSpaceGap = SetFontSpaceGap;
	gp_Graphics->LoadFont = LoadFont;
	gp_Graphics->IconDisplay = IconDisplay;
	//gp_Graphics->HangulOut = HangulOut;

	if(parent==NULL)	// Select Container Screen
	{
		switch(handle)
		{
			case 1:	
			{
				SetGraphicsPage(CONTAINER_WINDOW1_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW1_SCREEN;
				//Dprintf("GRAPHICS ADDRESS CONTAINER--1 ADDR=%08x\n", CONTAINER_WINDOW1_SCREEN);
				break;
			}
			case 2:
			{
				SetGraphicsPage(CONTAINER_WINDOW2_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW2_SCREEN;
				//Dprintf("GRAPHICS ADDRESS CONTAINER--2 ADDR=%08x\n", CONTAINER_WINDOW2_SCREEN);
				break;
			}
			case 3:	SetGraphicsPage(CONTAINER_WINDOW3_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW3_SCREEN;
			break;
			case 4:	SetGraphicsPage(CONTAINER_WINDOW4_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW4_SCREEN;
			break;
			case 5:	SetGraphicsPage(CONTAINER_WINDOW5_SCREEN); 
				gp_Graphics->Page = CONTAINER_WINDOW5_SCREEN;
			break;
#ifdef ALPHA_USE
            //LCD Alpha 스크린영역이다.
			case 6:	SetGraphicsPage(LCD_ALPHA_CONTAINER_SCREEN); 
				gp_Graphics->Page = LCD_ALPHA_CONTAINER_SCREEN;
			break;
#endif
		}
	}
	else 	// Select Widget Screen 
	{
		switch(parent)
		{
			case 1:	
			{
				SetGraphicsPage(CONTAINER_WIDGET1_SCREEN); 
				gp_Graphics->Page = CONTAINER_WIDGET1_SCREEN;
				//Dprintf("GRAPHICS ADDRESS WIDGET--1 ADDR=%08x\n", CONTAINER_WIDGET1_SCREEN);
				break;
			}
			case 2:	
			{
				SetGraphicsPage(CONTAINER_WIDGET2_SCREEN); 
				//Dprintf("GRAPHICS ADDRESS WIDGET--2 ADDR=%08x\n", CONTAINER_WIDGET1_SCREEN);
				gp_Graphics->Page = CONTAINER_WIDGET2_SCREEN;
				break;
			}
			case 3:	SetGraphicsPage(CONTAINER_WIDGET3_SCREEN); 
				gp_Graphics->Page = CONTAINER_WIDGET3_SCREEN;
			break;
			case 4:	SetGraphicsPage(CONTAINER_WIDGET4_SCREEN); 
				gp_Graphics->Page = CONTAINER_WIDGET4_SCREEN;
			break;
			case 5:	SetGraphicsPage(CONTAINER_WIDGET5_SCREEN); 
				gp_Graphics->Page = CONTAINER_WIDGET5_SCREEN;			
			break;
#ifdef ALPHA_USE
			case 6:	SetGraphicsPage(LCD_ALPHA_WIDGET_SCREEN); 
				gp_Graphics->Page = LCD_ALPHA_WIDGET_SCREEN;			
			break;
#endif
		}		
	}
	//해당되는 윈도우 영역의 클립영역을 설정하여 그래픽의 오버가 발생하지 않게한다.
	if(area==CLIENT_AREA)
	{	
		SetClip(1, window->client_abs_sx, window->client_abs_sy, window->client_width, window->client_height, 1);
		//Dprintf("\nGetGraphics::client->sx=%d client_sy=%d client_width=%d client_height=%d\n", window->client_sx, window->client_sy, window->client_width, window->client_height);
		//Dprintf("GetGraphics::client->abs_sx=%d client->abs_sy=%d client->abs_ex=%d client->abs_ey=%d\n", window->client_abs_sx, window->client_abs_sy, window->client_abs_ex, window->client_abs_ey);
	}
	else
	{
		SetClip(1, window->abs_sx, window->abs_sy, window->width, window->height, 1);
	}
	return(gp_Graphics);
}

void Icon64White_Load(void);
void Icon48White_Load(void);
void Icon32White_Load(void);
void Icon64Gray_Load(void);
void Icon48Gray_Load(void);
void Icon32Gray_Load(void);
/**
	@brief	윈도우를 시작하기전에 필요한 리소스를 로드한다.
*/
#define		PRO_BAR	59
int TextOutAliasing(int sx, int sy, char *str, uint16_t color, uint16_t back_color, char *font);
void Window_Resource_Load(void)
{
	int color = clWhite;
	g_Timer_msec = 0;	
	LoadAliasingFont("ng_eh_24.sif");
	SetFontAliasing("ng_eh_24.sif");	
	g_Color=color;
    //TextOutAliasing(100, ((768-32)/2) - 30, "Resource loading", color, 0, "ng_eh_24.sif");		
    TextOutAliasing(100, ((768-32)/2) - 30, "BOOTLOADER VER 3.150", color, 0, "ng_eh_24.sif");		
	Rectangle(100, (768-32)/2, 1024-200, 32);	
	
	LoadAliasingFont("mg48_e_Bold.sif");
	g_Color=color;
	FillRect(100, (768-32)/2, PRO_BAR, 32);	

	//LoadAliasingFont("mg64_e_Bold.sif");
	FillRect(100, (768-32)/2, PRO_BAR*2, 32);	

	//LoadAliasingFont("mg20_e_Bold.sif");
	FillRect(100, (768-32)/2, PRO_BAR*3, 32);	

	//LoadAliasingFont("mg24_e_Bold.sif");
	FillRect(100, (768-32)/2, PRO_BAR*4, 32);	

	//LoadAliasingFont("ng_e_24_Bold.sif");
	FillRect(100, (768-32)/2, PRO_BAR*5, 32);	

	//LoadAliasingFont("ng_e_32_Bold.sif");
	FillRect(100, (768-32)/2, PRO_BAR*6, 32);	
	
	//LoadAliasingFont("ng_e_48_Bold.sif");
	FillRect(100, (768-32)/2, PRO_BAR*7, 32);	
	
	LoadAliasingFont("ng_e_64_Bold.sif");	
	FillRect(100, (768-32)/2, PRO_BAR*8, 32);	
	
	//LoadAliasingFont("ng20_botong.sif");	
	FillRect(100, (768-32)/2, PRO_BAR*9, 32);		

	//LoadAliasingFont("ng128_e.sif");
	FillRect(100, (768-32)/2, PRO_BAR*10, 32);	

#ifndef LCD17INCH
	//BmpLoad(SYSTEM_ICON64_GRAY_SCREEN, 0, 0, "Icon64_Gray.bmp");
	FillRect(100, (768-32)/2, PRO_BAR*11, 32);	
	//BmpLoad(SYSTEM_ICON64_WHITE_SCREEN, 0, 0, "Icon64_White.bmp");
	FillRect(100, (768-32)/2, PRO_BAR*12, 32);	
	BmpLoad(SYSTEM_ICON48_SCREEN, 0, 0, "Icon48_All.bmp");
	FillRect(100, (768-32)/2, PRO_BAR*13, 32);	
	//BmpLoad(SYSTEM_ICON32_SCREEN, 0, 0, "Icon32_All.bmp");	
	FillRect(100, (768-32)/2, PRO_BAR*14, 32);	
#endif	
#ifdef LCD17INCH 
	//BmpLoad(SYSTEM_ICON48_SCREEN, 0, 0, "Icon48_All.bmp");
	FillRect(100, (768-32)/2, PRO_BAR*11, 32);	
	//BmpLoad(SYSTEM_ICON32_SCREEN, 0, 0, "Icon32_All.bmp");	
	FillRect(100, (768-32)/2, PRO_BAR*12, 32);	
#endif	
	//TextOutAliasing(100, ((768-32)/2) - 30, "3-3 Resource loading", clBlack, 0, "ng_eh_24.sif");	
	//TextOutAliasing(100, ((768-32)/2) - 30, "System startup", clWhite, 0, "ng_eh_24.sif");		
	//Dprintf("Time=%d\n", g_Timer_msec);
	
}

#ifdef __cplusplus
}
#endif
