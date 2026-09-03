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
void STextBoxStyle_Draw(Windows *win, Graphics *Gr, int sx, int sy, int width, int height);

/******************************************************************************
							Function Prototype
******************************************************************************/
void TextPanelDraw(Windows *win, Graphics *Gr, int sx, int sy)
{	
	Gr->SetColor(win->BackColor);
	Gr->FillRect(sx, sy, win->width, win->height);
	STextBoxStyle_Draw(win, Gr, sx, sy, win->width, win->height);
}

int	TextPanelPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	TextPanelDraw(win, Gr, sx, sy);
	return(1);
}

U32 TextPanel(HANDLE ContainerHandle, int sx, int sy, int width, int height , int backcolor)
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"SLabel");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	//wid_struct.FontColor = fontcolor;
	//wid_struct.Style=style;
	handle=makeWindow(
				LCD,						// Display Section Position
				"GroupPanel",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				TextPanelPaintEvent,			// PaintEvent
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
				NULL, //text,							// Windows Text
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
