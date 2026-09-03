/******************************************************************************
*    	File name	: GroupBox.c 
*		Description : TEECO GUI Window Control Widget [GroupBox]
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

int	GroupBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			text_width;
	int			text_height;
	int			text_height2;
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	Gr->SetFont(win->FontNo);
	
	text_width = GetTextWidth(win->Text);
	text_height = GetTextHeight(g_FontNo);
	text_height2 = text_height / 2;
	//전체바탕색을 칠한다.
	Gr->SetColor(win->BackColor);
	Gr->FillRect(sx, sy, win->width, win->height);

	Gr->SetColor(GRAY5);
	Gr->Rectangle(sx, sy+text_height2, win->width-2, win->height-text_height2-2);
	Gr->SetColor(WHITE);
	Gr->Rectangle(sx+1, sy+text_height2+1, win->width-2, win->height-text_height2-2);

	Gr->SetColor(win->BackColor);
	Gr->FillRect(sx+5, sy, text_width, text_height);

	//Gr->LoadFont(win->FontName);
	Gr->SetFontGap(win->FontGap);
	Gr->SetFontSpaceGap(win->FontSpaceGap);
	Gr->SetFontColor(win->FontColor);

	Gr->TextOut(sx+7, sy, win->Text);
	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


U32 GroupBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		fontcolor, 
			int		backcolor, 
			int		font_no, 
			char *Text)

{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"GroupBox");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	//strcpy(wid_struct.FontName, "SystemFontGL8.fnt");
	wid_struct.FontGap = 4;
	wid_struct.FontNo = font_no;
	wid_struct.FontSpaceGap = 7;	
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"GroupBox",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				GroupBoxPaintEvent,			// PaintEvent
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
				Text,						// Windows Text
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
