/******************************************************************************
*    	File name	: TransParencyTextBox.c 
*		Description : TEECO GUI Window Control Widget [TransParencyTextBox]
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

/******************************************************************************
							Example Program
******************************************************************************/
/*


*/

int	TransParencyTextBoxMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int	sx=0, sy=0;
	int	text_width;
	int 	page=0;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	if(win->Attr == READ_ONLY) return(0);
	//PlaySound(1);	
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	//부모윈도우의 바탕색을 칠한다.
	page = page;
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	//Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	Gr->SetColor(YELLOW);
	Gr->FillRect(sx, sy, win->width, win->height); 
	if(win->Text != NULL)
	{
		//Gr->LoadFont(win->FontName);
		Gr->SetFont(win->FontNo);
//Dprintf("TransParencyTextBoxPaintEvent::FontNo=%d\n", win->FontNo);
		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
		text_width = GetTextWidth(win->Text);

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
		sy = (win->height - GetTextHeight(g_FontNo)) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->TextOut(sx, sy, win->Text);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	TransParencyTextBoxMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			text_width, page=0;
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
	//부모윈도우의 바탕색을 칠한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	
	if(win->Text != NULL)
	{
		//Gr->LoadFont(win->FontName);
		Gr->SetFont(win->FontNo);
//Dprintf("TransParencyTextBoxPaintEvent::FontNo=%d\n", win->FontNo);
		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
		text_width = GetTextWidth(win->Text);

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
		sy = (win->height - GetTextHeight(g_FontNo)) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->TextOut(sx, sy, win->Text);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	TransParencyTextBoxCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	Gr->FillRect(0, 0, 0, 0);
	//부모윈도우의 바탕색을 칠한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	Gr->Draw(parent, handle, command, param, Data);
    return(1);
}


int	TransParencyTextBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			text_width, page=0;
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
	//부모윈도우의 바탕색을 칠한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	
	if(strlen(win->Text))
	{
		//Gr->LoadFont(win->FontName);
		Gr->SetFont(win->FontNo);
//Dprintf("TransParencyTextBoxPaintEvent::FontNo=%d\n", win->FontNo);
		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
        //한글용 for Test
        //Gr->SetHangulFont(37);
        //Gr->SetFontStartLine(0);    //영문만 적용된다.        
        
		text_width = GetTextWidth(win->Text);

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
		sy = (win->height - GetTextHeight(g_FontNo)) / 2;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->TextOut(sx, sy, win->Text);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	TransParencyTextBoxChangeEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	return(1);
}

U32 TransParencyTextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		align,		//Left, Right, Center
			int		attr,		//READ_ONLY, RW_MODE
			int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )

{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"TransParencyTextBox");

	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.FontColor = BLACK;
	wid_struct.FontNo = font_no;
	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;	
	wid_struct.Attr = attr;	
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"TPTextBox",				// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				TransParencyTextBoxCreateEvent,						// CreateEvent
				TransParencyTextBoxPaintEvent,			// PaintEvent
				TransParencyTextBoxMouseOnEvent,				// MouseOnEvent
				TransParencyTextBoxMouseOffEvent,						// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				MouseOnEvent,						// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				TransParencyTextBoxChangeEvent,			// ChangeEvent
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
	//Dprintf("TextBox::id = %08x\n", id);	
	return(id);
}				
#ifdef __cplusplus
}
#endif
