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

int	SIconPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	IconDisplay(0, 0, win->IconName, win->FontColor);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//style 이 0이면 투명, 1이면 배경색을 칠한다. FontColor를 PassColor로 사용한다.
//아이콘이름은 bmp file을 지정해야한다.
U32 SIcon(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *iconame, int passcolor, int backcolor, int style)
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"SLabel");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );

	//!< 반드시 지워야한다.
	memset(&wid_struct, 0, sizeof(WidgetStruct));
	
	strcpy(wid_struct.IconName, iconame);		
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = passcolor;
	wid_struct.Style=style;
	handle=makeWindow(
				LCD,						// Display Section Position
				"SIcon",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				SIconPaintEvent,			// PaintEvent
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
				NULL,							// Windows Text
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
