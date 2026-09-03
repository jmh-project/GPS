/******************************************************************************
*    	File name	: MemoryButton.c 
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

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define     MEMORY_BUTTON_SOUND         "dog_sound_1.wav"
/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

int	 MemoryButtonCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	if(win->Toggle==TOGGLE_BUTTON || win->Toggle==SELECT_BUTTON ) 
	{
		win->MouseOffEvent = NULL;
		win->ToggleStatus=OFF;
		win->ChangeEvent = win->ClickEvent;
		win->ClickEvent = NULL;
	}	
	return(1);
}

int MemoryButtonPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int 		normal_screen;
	int 		touch_screen;
	PARAM		parameter;
	int			sx=0, sy=0;
	int			text_width;
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	
	parameter = win->Param;
	
	normal_screen = LOWORD(parameter);
	touch_screen = HIWORD(parameter);

	Gr->FillRect(0,0,0,0);;//dummy

	if(command==OFF)
	{
		win->ToggleStatus=OFF;
		Gr->BitBlt(normal_screen, win->sx, win->sy, 0, 0, win->width, win->height);
	}
	else
	{
		win->ToggleStatus=ON;
		Gr->BitBlt(touch_screen, win->sx, win->sy, 0, 0, win->width, win->height);
	}

	if(strlen(win->Text))
	{
		Gr->SetFont(win->FontNo);
		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
		text_width = GetTextWidth(win->Text);

		if(win->TextAlignMode==RIGHT_ALIGN)
		{
			sx = (win->width - text_width)-2;
		}
		else if(win->TextAlignMode==CENTER_ALIGN)
		{
			sx = (win->width - text_width) / 2;
		}
		else if(win->TextAlignMode==LEFT_ALIGN)
		{
			sx=5;
		}
		sy = (win->height - GetTextHeight(g_FontNo)) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->TextOut(sx, sy, win->Text);
	}
	
	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	 MemoryButtonMouseMoveEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	static int loop=0;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	if(win->Toggle == PUSH_BUTTON)
	{
		loop++;
		if(loop > 3)
		{
			if(win->ClickEvent!=NULL) win->ClickEvent(parent, handle, ON, 0, 0);
			loop=0;
		}
	}
	return(1);
}

int	 MemoryButtonMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int 		normal_screen;
	int 		touch_screen;
	PARAM		parameter;
	int			sx=0, sy=0;
	int			text_width;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	parameter = win->Param;
	normal_screen = LOWORD(parameter);
	touch_screen = HIWORD(parameter);

	Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	Gr->FillRect(0,0,0,0);;//dummy
	if(win->Toggle == TOGGLE_BUTTON)
	{
		if(win->ToggleStatus==OFF)
		{
			Gr->BitBlt(touch_screen, win->sx, win->sy, 0, 0, win->width, win->height);
			win->ToggleStatus = ON;
			//if(win->ChangeEvent!=NULL) win->ChangeEvent(parent, handle, ON, 0, 0);
		}
		else
		{
			Gr->BitBlt(normal_screen, win->sx, win->sy, 0, 0, win->width, win->height);
			win->ToggleStatus = OFF;
			//if(win->ChangeEvent!=NULL) win->ChangeEvent(parent, handle, OFF, 0, 0);
		}
	}
	else if(win->Toggle == PUSH_BUTTON)
	{
		Gr->BitBlt(touch_screen, win->sx, win->sy, 0, 0, win->width, win->height);
	}
	else if(win->Toggle == SELECT_BUTTON)
	{
		if(win->ToggleStatus==OFF)
		{
			Gr->BitBlt(touch_screen, win->sx, win->sy, 0, 0, win->width, win->height);
			win->ToggleStatus = ON;
			//if(win->ChangeEvent!=NULL) win->ChangeEvent(parent, handle, ON, 0, 0);
		}		
	}
	if(strlen(win->Text))
	{
		Gr->SetFont(win->FontNo);
		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
		text_width = GetTextWidth(win->Text);

		if(win->TextAlignMode==RIGHT_ALIGN)
		{
			sx = (win->width - text_width)-2;
		}
		else if(win->TextAlignMode==CENTER_ALIGN)
		{
			sx = (win->width - text_width) / 2;
		}
		else if(win->TextAlignMode==LEFT_ALIGN)
		{
			sx=5;
		}
		sy = (win->height - GetTextHeight(g_FontNo)) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->TextOut(sx, sy, win->Text);
	}
	
	Gr->Draw(parent, handle, command, param, Data);	

	if(win->Toggle == TOGGLE_BUTTON)
	{
		if(win->ToggleStatus==ON)
		{
			if(win->ChangeEvent!=NULL) win->ChangeEvent(parent, handle, ON, 0, 0);
		}
		else
		{
			if(win->ChangeEvent!=NULL) win->ChangeEvent(parent, handle, OFF, 0, 0);
		}
	}
	else if(win->Toggle == SELECT_BUTTON)
	{
		if(win->ToggleStatus==ON)
		{
			if(win->ChangeEvent!=NULL) win->ChangeEvent(parent, handle, ON, 0, 0);
		}		
	}		
	//PlaySoundExt(MEMORY_BUTTON_SOUND);
	return(1);
}

int	 MemoryButtonMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int 		normal_screen=0;
	int 		touch_screen=0;
	PARAM		parameter;
	int			sx=0, sy=0;
	int			text_width;
	
	touch_screen=touch_screen;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	parameter = win->Param;
	
	normal_screen = LOWORD(parameter);
	touch_screen = HIWORD(parameter);

	gp_Graphics->BitBlt(normal_screen, win->sx, win->sy, 0, 0, win->width, win->height);
	if(strlen(win->Text))
	{
		Gr->SetFont(win->FontNo);
		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
		text_width = GetTextWidth(win->Text);

		if(win->TextAlignMode==RIGHT_ALIGN)
		{
			sx = (win->width - text_width)-2;
		}
		else if(win->TextAlignMode==CENTER_ALIGN)
		{
			sx = (win->width - text_width) / 2;
		}
		else if(win->TextAlignMode==LEFT_ALIGN)
		{
			sx=5;
		}
		sy = (win->height - GetTextHeight(g_FontNo)) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->TextOut(sx, sy, win->Text);
	}
	
	gp_Graphics->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 MemoryButton(HANDLE ContainerHandle, int sx, int sy, int width, int height, 
			int font_no, char *text, int align, int NormalPage, int TouchPage, int function, int id, 
			int (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *))
{
	U32		w_id;
	HANDLE	handle;
	PARAM 	parameter;
	WidgetStruct wid_struct;	
	
	parameter = MAKELONG(NormalPage, TouchPage);
	wid_struct.Toggle = function;
	wid_struct.ID = id;
	wid_struct.FontNo = font_no;

	wid_struct.FontColor = BLACK;
	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;

	wid_struct.TextAlignMode = align;
	
	handle = makeWindow(
				LCD,
				"mem_Button",				// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				MemoryButtonCreateEvent,	// CreateEvent
				MemoryButtonPaintEvent,		// PaintEvent
				MemoryButtonMouseOnEvent,	// MouseOnEvent
				MemoryButtonMouseOffEvent,	// MouseOffEvent
				MemoryButtonMouseMoveEvent,	// MouseMoveEvent
				NULL,						// GestureEvent
				ClickEvent,					// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				NULL,  						// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				parameter,					// PARAMETER
				0,							// Normal Touch Screen
				0,							// Touch Screen
				text,						// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	w_id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	return(w_id);
}


#ifdef __cplusplus
}
#endif

