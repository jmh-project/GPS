/******************************************************************************
*    	File name	: ImageBox.c 
*		Description : TEECO GUI Window Control Widget [ImageBox]
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

int	ImageBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
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
	//Gr->SetColor(BLACK);
	//Gr->Rectangle(0, 0, win->width, win->height);
	Gr->BmpDisplay(0, 0, win->NormalName);

	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


U32 ImageBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			char	*image )

{
	HANDLE		handle;
	U32			id;
    
	Windows		*parent_win;
	parent_win = GetWindow(NULL, ContainerHandle,"ImageBox");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"Progress",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				ImageBoxPaintEvent,			// PaintEvent
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
				image,						// Image
				NULL,						// 
				NULL,						// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	return(id);
}				

#ifdef __cplusplus
}
#endif
