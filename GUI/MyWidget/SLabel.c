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


/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

int	SLabelPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			page=0;
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
	
	SetFontAliasing("ng_eh_24.sif");
	int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
	sx = (win->width -text_width) / 2;
	sy = (win->height - 24) / 2;	
	if(win->FontColor==clBlack )
		Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 0, "ng_eh_24.sif");
	if(win->FontColor==clBlue || win->FontColor==clMagenta || win->FontColor==clCyan)
		Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 1, "ng_eh_24.sif");
	else if(win->FontColor==clGreen)
		Gr->TextOutAliasing(sx, sy, win->Text, clGreen, 0, "ng_eh_24.sif");
	else if(win->FontColor==clWhite)
		Gr->TextOutAliasing(sx, sy, win->Text, clWhite, 0, "ng_eh_24.sif");
	else if(win->FontColor==clYellow)
		Gr->TextOutAliasing(sx, sy, win->Text, clYellow, 0, "ng_eh_24.sif");
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//style 이 0이면 투명, 1이면 배경색을 칠한다.
U32 SLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style)
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"SLabel");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	wid_struct.Style=style;
	handle=makeWindow(
				LCD,						// Display Section Position
				"SLabel",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Window   s Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				SLabelPaintEvent,			// PaintEvent
				NULL,						// MouseOnEvent
				NULL,						// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				NULL,						// ClickEvent
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
	id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	//Dprintf("TextBox::id = %08x\n", id);	
	return(id);
}				
#ifdef __cplusplus
}
#endif
