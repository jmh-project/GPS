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
static U32	usb_label=0;
//static U32	file_label=0;
//static U32	progress=0;
/******************************************************************************
							Function Prototype
******************************************************************************/
		
U32 BIcon(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *iconame, int backcolor);
static int  Save_ClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "");
	Windows		*parent_win = FindContainerWindow(parent,  "");

	if(parent_win == NULL)
	{
		Dprintf("Save_ClickEvent:: Windows Error\n");
	}
    VoicePlay(TOUCH_SOUND);   
	if(win->ID == 30)
	{
		if(parent_win->ClickEvent != 0)
			parent_win->ClickEvent(parent_win->Parent, parent_win->Handle, 1, 0, 0); //close
		CloseWindow(NULL, parent);
	}
	return(1);
}

static int	 Save_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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

	BIcon(handle, 20, 50, 48, 48 , "48G_USB_Icon1.bmp", RGB565CONVERT(230,  230,  230));
	usb_label = SLabel(handle, 20+70, 50+10, 520, 30, "USB Storage Mounting...", clBlack, RGB565CONVERT(191, 191, 191),  0);	 //
	//BIcon(handle, 20, 150, 48, 48 , "48G_Save_Icon1.bmp", RGB565CONVERT(230,  230,  230));
	//file_label = SLabel(handle, 20+60, 150+15, 520, 30, "", clBlack, RGB565CONVERT(191, 191, 191),  0);	 //

	//progress = ProgressBar(handle, 20, 220, 500, 32, 	0,	clWhite,	clBlue, 0);
	
	//Close
	SButton(handle, 320, 180, 200, 54, "48G_LogOut_Icon1.bmp", "Close",  PUSH_BUTTON, 30, Save_ClickEvent);
	
	PaintDrawControl(ON);
	
	return(1);
}

static int	 Save_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	if(command==1)
	{
		SetWidgetText(usb_label, "USB Mounting OK");
	}
	else if(command==2)
	{
		SetWidgetText(usb_label, (char *)Data);		
	}
	else if(command==3)
	{
		char	buf[128];
		sprintf(buf,"[%s] File Copy OK",  (char *)Data);
		SetWidgetText(usb_label, buf);		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Save_Window(char *title, int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *))
{
	if(GetWindowName("Save_Window")) 
	{
		Dprintf("Save_Window:: Windows already Create\n");
		return 0;		
	}		
	int sx = (LCD_X - 560 ) / 2;
	int sy = (LCD_Y - 300 ) / 2;
	//strcpy(Info_Text, text);
	makeWindow(
				LCD,
				"Save_Window",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				560, 300,				// Size
				SHOW,					// Create after Show
				Save_CreateEvent,		// CreateEvent
				Save_PaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				ClickEvent,					// ClickEvent
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
