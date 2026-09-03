/******************************************************************************
*    	File name	: RadioButton.c 
*		Description : TEECO GUI Window Control Widget [RadioButton]
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

int	RadioButtonPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			text_width = 0;;
	
	text_width=text_width;
	
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
	Gr->SetColor(WHITE);
	Gr->FillElipse(8, 8+3, 8, 8);

	Gr->SetColor(BLACK);
	Gr->Elipse(8, 8+3, 8, 8);

	//Gr->SetColor(BLACK);
	//Gr->Arc(8, 8, 6, 185, 265);

	if(win->Param==TRUE)
	{
		Gr->SetColor(BLACK);
		Gr->FillElipse(8, 8+3, 5, 5);
	}
	else
	{
		Gr->SetColor(GRAY2);
		Gr->FillElipse(8, 8+3, 5, 5);
	}
	
	if(win->Text != NULL)
	{
		//Gr->LoadFont(win->FontName);
		Gr->SetFont(win->FontNo);
		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
		text_width = GetTextWidth(win->Text);

		sx = 20;
		sy = 4;
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->TextOut(sx, sy, win->Text);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	RadioButtonMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
    
	if(win->ChangeEvent != NULL) win->ChangeEvent(win->Parent, win->Handle, win->CallWidget_ID, win->Param, 0);	
	return(1);
}

U32 RadioButton(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		font_no,
			char	*text,		//init Character
			int		backcolor, 
			int		fontcolor,
			int 	check,
            int     call_id,
			int		(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )

{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"RadioButton");

	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;

	wid_struct.FontNo = font_no;
	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;	
    wid_struct.CallWidget_ID = call_id;
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"RadioButton",				// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				RadioButtonPaintEvent,		// PaintEvent
				RadioButtonMouseOnEvent,	// MouseOnEvent
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
