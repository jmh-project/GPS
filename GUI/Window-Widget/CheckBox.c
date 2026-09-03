/******************************************************************************
*    	File name	: CheckBox.c 
*		Description : TEECO GUI Window Control Widget [CheckBox]
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

int	CheckBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=3, sy=3;
//	int			text_width=0;
	
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
	
	//전체바탕색을 칠한다.
	Gr->SetColor(win->BackColor);
	Gr->FillRect(0, 0, win->width, win->height);


	Gr->SetColor(WHITE);
	Gr->FillRect(sx, sy, 16, 16);

	//스타일을 그린다.
	Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
	Gr->Line(sx, sy, sx, sy+16-1);			//좌측수직라인그리기		
	Gr->Line(sx, sy, sx+16-1, sy);			//상단수직라인		
	Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));	//우측수직라인
	Gr->Line(sx+16-1, sy+1, sx+16-1, sy+1+16);		
	Gr->Line(sx+1, sy+16-1, sx+1+16, sy+16-1);//아래백색라인
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
	Gr->Line(sx+1, sy+1, sx+1, sy+16-2);	//좌측회색라인		
	Gr->Line(sx+1, sy+1, sx+16-2, sy+1);	//위회색라인

	if(win->Param==TRUE)
	{
		Gr->SetColor(BLACK);
		Gr->Line(sx+3, sy+3, sx+16-2, sy+16-2);	
		Gr->Line(sx+4, sy+3, sx+16-2, sy+16-2-1);	
		
		Gr->Line(sx+3, sy+16-3, sx+16-2, sy+2);
		Gr->Line(sx+4, sy+16-3, sx+16-2+1, sy+2);
	}
	else
	{
		//Gr->SetColor(GRAY2);
	}	
	if(win->Text != NULL)
	{
		//Gr->LoadFont(win->FontName);
		Gr->SetFont(win->FontNo);
		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
		int text_width = GetTextWidth(win->Text);

		sx = 22;
		sy = 4;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->TextOut(sx, sy, win->Text);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	CheckBoxMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	if(win->Param == TRUE) win->Param=FALSE;
	else win->Param=TRUE;
	win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);	
	return(1);	
}

U32 CheckBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	style,
			int		font_no,
			char	*text,		//init Character
			int		backcolor, 
			int		fontcolor,
			int 	check,
			int		(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )

{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"CheckBox");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	wid_struct.Style = style;
	wid_struct.FontColor = BLACK;

	wid_struct.FontNo = font_no;
	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;	
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"CheckBox",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				CheckBoxPaintEvent,			// PaintEvent
				CheckBoxMouseOnEvent,		// MouseOnEvent
				NULL,						// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				NULL,						// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				ChangeEvent,				// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				check,						// PARAMETER
				NULL,						// Back Image
				NULL,						// 
				text,						// Windows Text
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
					
