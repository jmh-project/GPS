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
#define		MAX_INPUT_DIGIT		20
//Sound 효과음의 정의
#define		TOUCH_SOUND			1	 
/******************************************************************************
							Private & Local Variables
******************************************************************************/
static char Info_Text[128];
/******************************************************************************
							Function Prototype
******************************************************************************/
		
U32 BIcon(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *iconame, int backcolor);
static int	  Info_ClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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

static int	  Info_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	Gr->FillRect(0, 0, win->width-2, win->height-2);
//	if(flash.systemColorNo==6) {
//		Gr->SetColor(RGB565CONVERT(230,  230,  230));
//	}
//	else {
//		Gr->SetColor(flash.systemBackGroundColor);
//	}
//	Gr->FillRect(0, 0, win->width-2, win->height-2);
	

	//타이틀바를 그린다.
	//TitleBar(handle, 0, 0, win->width, 50, 1, win->Text, clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	BIcon(handle, 20, 50, 48, 48 , "48G_Warning_Icon1.bmp", RGB565CONVERT(230,  230,  230));
	SLabel(handle, 20+60, 50+15, 590, 30, Info_Text, clBlack, RGB565CONVERT(191, 191, 191),  0);	 //
	//SLabel(handle, 20+60, 50+15+40, 500, 30, "", clBlack, RGB565CONVERT(191, 191, 191),  0);	

	//Close
	SButton(handle, 460, 180, 200, 54, " ", "Close",  PUSH_BUTTON, 30, Info_ClickEvent);
	
	PaintDrawControl(ON);
	
	return(1);
}

static int	  Info_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	Gr->FillRect(0, 0, 0, 0);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Info_Window(char *title, char *text) {
	if(GetWindowName("Info_Window")) 
	{
		Dprintf("Info_Window:: Windows already Create\n");
		return 0;		
	}		
	int sx = (LCD_X - 690 ) / 2;
	int sy = (LCD_Y - 300 ) / 2;
	strcpy(Info_Text, text);
	makeWindow(
				LCD,
				"Info_Window",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				690, 300,				// Size
				SHOW,					// Create after Show
				Info_CreateEvent,		// CreateEvent
				Info_PaintEvent,		// PaintEvent
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
				title,					// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	return(1);
}
#ifdef __cplusplus
}
#endif
