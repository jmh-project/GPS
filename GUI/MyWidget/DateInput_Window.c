/******************************************************************************
*    	File name	: KeyPad.c 
*		Description : TEECO Project User Program File
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
#include "dos.h"
#include "BSP_DS1302.h"	 
/******************************************************************************
							Constant & Macros
******************************************************************************/
//Sound 효과음의 정의
#define		TOUCH_SOUND			1	 
/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
		

static int  DateInput_ClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "");
	Windows		*parent_win = FindContainerWindow(parent,  "");

	if(parent_win == NULL)
	{
		Dprintf("SConfirm_ClickEvent:: Windows Error\n");
	}
    VoicePlay(TOUCH_SOUND);   
	if(win->ID == 30)
	{
		CloseWindow(NULL, parent);
	}
	return(1);
}

static int	 DateInput_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	PaintDrawControl(OFF);
	
	Gr->SetColor(RGB565CONVERT(230,  230,  230));
	Gr->FillRect(0, 0, win->width, win->height);
	//타이틀바를 그린다.
	TitleBar(handle, 0, 0, win->width, 50, 1, "Date & Time Input", clYellow, 1, RIGHT_ALIGN, 2, NULL);	

	//Close
	SButton(handle, 460, 280, 200, 54, "48G_LogOut_Icon1.bmp", "Close",  PUSH_BUTTON, 30, DateInput_ClickEvent);
	
	PaintDrawControl(ON);
	
	return(1);
}

static int	 DateInput_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	Gr->FillRect(0, 0, 0, 0);	
	
//    Gr->BitBlt(SYSTEM_BMP_SCREEN, 0, 0, 100, 100, 64, 64);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int DateInput_Window(void)
{
	if(GetWindowName("DateInput_Window")) 
	{
		Dprintf("DateInput_Window:: Windows already Create\n");
		return 0;		
	}		
	int sx = (LCD_X - 800 ) / 2;
	int sy = (LCD_Y - 600 ) / 2;
	makeWindow(
				LCD,
				"DateInput_Window",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				STYLE_NONE,				// Windows Style
				sx, sy, 				// Start Position
				800, 600,				// Size
				SHOW,					// Create after Show
				DateInput_CreateEvent,		// CreateEvent
				DateInput_PaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				NULL,					// TimerEvent
				NULL,  					// ControlEvent
				NULL,  					// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				NULL,					// PARAMETER
				NULL,					// Normal Touch Screen
				NULL,						// Touch Screen
				NULL,					// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	return(1);
}
#ifdef __cplusplus
}
#endif
