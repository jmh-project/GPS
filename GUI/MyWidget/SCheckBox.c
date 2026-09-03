/******************************************************************************
*    	File name	: LineBar.c 
*		Description : TEECO GUI Window Control Widget [LineBar]
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: www.ziontek.co.kr
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
#define		ICONTOTEXT_INTERVAL		45

/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
void SCheckDraw(HANDLE parent, int  press, Windows *win, Graphics *Gr, int sx, int sy)
{
	int			page=0;
	//부모윈도우의 바탕색을 칠한다.
	if(win->WidgetStyle==0)
	{
		switch(parent)
		{
			case 1: page = CONTAINER_WINDOW1_SCREEN; break;
			case 2: page = CONTAINER_WINDOW2_SCREEN; break;
			case 3: page = CONTAINER_WINDOW3_SCREEN; break;
			case 4: page = CONTAINER_WINDOW4_SCREEN; break;
			case 5: page = CONTAINER_WINDOW5_SCREEN; break;
		}
		Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	}
	else
	{
		Gr->SetColor(win->BackColor);
		Gr->FillRect(sx, sy, win->width, win->height);
	}	
	
	int sy_icon = (win->height - 32) / 2;	
	if(press==OFF)
	{
		IconOut32G(sx, sy_icon, "32G_Check_Icon3.bmp"); 
	}
	else
	{
		IconOut32G(sx, sy_icon, "32G_Check_Icon1.bmp"); 
	}	
	sx+=ICONTOTEXT_INTERVAL;
	
	SetFontAliasing("ng_eh_24.sif");
	int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
	sy = (win->height - 24) / 2;	
	if(win->FontColor==clBlack)
		Gr->TextOutAliasing(sx, sy, win->Text, clBlack, 0, "ng_eh_24.sif");
	else if(win->FontColor==clWhite)
		Gr->TextOutAliasing(sx, sy, win->Text, clWhite, 0, "ng_eh_24.sif");
	else if(win->FontColor==clYellow)
		Gr->TextOutAliasing(sx, sy, win->Text, clYellow, 0, "ng_eh_24.sif");	
}
	 
int	SCheckMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
 	//Dummy
	Gr->FillRect(0,0,0,0);
	if(win->ToggleStatus==OFF)
	{
		win->ToggleStatus = ON;
		SCheckDraw(parent, ON, win, Gr, sx, sy);
	}
	else
	{
		win->ToggleStatus = OFF;
		SCheckDraw(parent, OFF, win, Gr, sx, sy);
	}	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);	
}

int	SCheckPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
 	//Dummy
	Gr->FillRect(0,0,0,0);
	if(command==0)
	{
		SCheckDraw(parent, OFF, win, Gr, sx, sy);
		win->ToggleStatus=OFF;
	}
	else
	{
		SCheckDraw(parent, ON, win, Gr, sx, sy);
		win->ToggleStatus=ON;
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//style 이 0이면 투명, 1이면 배경색을 칠한다.
U32 SCheck(HANDLE ContainerHandle, int sx, int sy, int width, int height , 
	char *text, int fontcolor, int backcolor, int style, int id,
	int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )
{
	HANDLE		handle;
	U32			w_id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"SCheck");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.ID = id;	//생성되는 윈도우마다 가지는 구조체의 ID	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	wid_struct.Style=style;
	handle=makeWindow(
				LCD,						// Display Section Position
				"SCheck",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				SCheckPaintEvent,			// PaintEvent
				SCheckMouseOnEvent,	// MouseOnEvent
				NULL,						// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				ClickEvent,					// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				NULL,						// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				NULL,						// PARAMETER
				NULL,						// Back Image
				NULL,						// 
				text,							// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	w_id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	//Dprintf("TextBox::id = %08x\n", id);	
	return(w_id);
}				
#ifdef __cplusplus
}
#endif
