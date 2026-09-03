/******************************************************************************
*    	File name	: TextBox.c 
*		Description : TEECO GUI Window Control Widget [TextBox]
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
#define		TEXTBOX_FONT_HEIGHT	16

/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

/******************************************************************************
							Example Program
******************************************************************************/
/*

int	tbox_ClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win;

	win = GetWindow(parent, handle, "TextBoxMouseOnEvent");
	if(win==NULL)
	{
		Dprintf("tbox_ClickEvent::GetWindow Error\n");
		return(0);
	}	
	Dprintf("TextBox Press\n");
}

U32	textbox_id;

textbox_id =  TextBox(ContainerHandle, 
				100, 100, 		// Start Pointer
				120, 80, 		// Size
				0,				// Font Number
				"10.0",			//init Character
				YELLOW, 		// Back Color
				BLACK,			// fontcolor,
				LEFT_ALIGN,		// RIGHT_ALIGN, LEFT_ALIGN, CENTER_ALIGN
				READ_ONLY,
				tbox_ClickEvent);
*/			
			
int	TextBoxMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			text_width=0;
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	if(win->Attr == READ_ONLY) return(0);

	
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	
	Gr->SetColor(win->BackColor + GREEN);	//박스를 누를때 컬러가 바뀌는색
	Gr->FillRect(0, 0, win->width, win->height);

	//스타일을 그린다.
	Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
	Gr->Line(sx, sy, sx, sy+win->height-1);			//좌측수직라인그리기		
	Gr->Line(sx, sy, sx+win->width-1, sy);			//상단수직라인		
	Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));	//우측수직라인
	Gr->Line(sx+win->width-1, sy+1, sx+win->width-1, sy+1+win->height);		
	Gr->Line(sx+1, sy+win->height-1, sx+1+win->width, sy+win->height-1);//아래백색라인
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
	Gr->Line(sx+1, sy+1, sx+1, sy+win->height-2);	//좌측회색라인		
	Gr->Line(sx+1, sy+1, sx+win->width-2, sy+1);	//위회색라인


	if(win->Text != NULL)
	{
		Gr->SetFontColor(win->FontColor);
#ifdef LOW_FONT_USE		
		text_width = LowTextLen(win->Text, fArial12);
#endif
		if(win->Param==RIGHT_ALIGN)
		{
			sx = (win->width - text_width)-2;
		}
		else if(win->Param==CENTER_ALIGN)
		{
			sx = (win->width - text_width) / 2;
		}
		else if(win->Param==LEFT_ALIGN)
		{
			sx=5;
		}
		sy = (win->height - TEXTBOX_FONT_HEIGHT) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->LowTextOut(sx, sy, win->Text, win->FontColor, fArial12);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	TextBoxMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	int			sx=0, sy=0;
	int			text_width;
	
	if(win->Attr == READ_ONLY) return(0);	
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	
	Gr->SetColor(win->BackColor);
	Gr->FillRect(0, 0, win->width, win->height);

	//스타일을 그린다.
	Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
	Gr->Line(sx, sy, sx, sy+win->height-1);			//좌측수직라인그리기		
	Gr->Line(sx, sy, sx+win->width-1, sy);			//상단수직라인		
	Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));	//우측수직라인
	Gr->Line(sx+win->width-1, sy+1, sx+win->width-1, sy+1+win->height);		
	Gr->Line(sx+1, sy+win->height-1, sx+1+win->width, sy+win->height-1);//아래백색라인
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
	Gr->Line(sx+1, sy+1, sx+1, sy+win->height-2);	//좌측회색라인		
	Gr->Line(sx+1, sy+1, sx+win->width-2, sy+1);	//위회색라인

	if(win->Text != NULL)
	{
		Gr->SetFontColor(win->FontColor);
#ifdef LOW_FONT_USE		
		text_width = LowTextLen(win->Text, fArial12);
#endif
		if(win->Param==RIGHT_ALIGN)
		{
			sx = (win->width - text_width)-2;
		}
		else if(win->Param==CENTER_ALIGN)
		{
			sx = (win->width - text_width) / 2;
		}
		else if(win->Param==LEFT_ALIGN)
		{
			sx=5;
		}
		sy = (win->height - TEXTBOX_FONT_HEIGHT) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->LowTextOut(sx, sy, win->Text, win->FontColor, fArial12);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	TextBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			text_width=0;
	
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
	
	Gr->SetColor(win->BackColor);
	Gr->FillRect(0, 0, win->width, win->height);

	//스타일을 그린다.
	Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
	Gr->Line(sx, sy, sx, sy+win->height-1);			//좌측수직라인그리기		
	Gr->Line(sx, sy, sx+win->width-1, sy);			//상단수직라인		
	Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));	//우측수직라인
	Gr->Line(sx+win->width-1, sy+1, sx+win->width-1, sy+1+win->height);		
	Gr->Line(sx+1, sy+win->height-1, sx+1+win->width, sy+win->height-1);//아래백색라인
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
	Gr->Line(sx+1, sy+1, sx+1, sy+win->height-2);	//좌측회색라인		
	Gr->Line(sx+1, sy+1, sx+win->width-2, sy+1);	//위회색라인

	if(win->Text != NULL)
	{
		Gr->SetFontColor(win->FontColor);
#ifdef LOW_FONT_USE		
		text_width = LowTextLen(win->Text, fArial18);
#endif
		if(win->Param==RIGHT_ALIGN)
		{
			sx = (win->width - text_width)-2;
		}
		else if(win->Param==CENTER_ALIGN)
		{
			sx = (win->width - text_width) / 2;
		}
		else if(win->Param==LEFT_ALIGN)
		{
			sx=5;
		}
		sy = (win->height - TEXTBOX_FONT_HEIGHT) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->LowTextOut(sx, sy, win->Text, win->FontColor, fArial18);
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 TextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		backcolor, 
			int		fontcolor,
			int		align,		//Left, Right, Center
			int		attr,		//READ_ONLY, RW_MODE
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )


{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "TextBox");

	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	wid_struct.FontColor = BLACK;
	wid_struct.FontNo = font_no;

	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;	
	wid_struct.Attr = attr;	
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"TextBox",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				TextBoxPaintEvent,			// PaintEvent
				TextBoxMouseOnEvent,		// MouseOnEvent
				TextBoxMouseOffEvent,		// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				ClickEvent,						// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				NULL,						// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				align,						// PARAMETER
				NULL,						// Back Image
				NULL,						// 
				text,						// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	return(id);
}				
#ifdef __cplusplus
}
#endif					
