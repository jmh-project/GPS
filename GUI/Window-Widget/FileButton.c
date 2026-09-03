/******************************************************************************
*    	File name	: FileButton.c 
*		Description : TEECO Project Entry File
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: Naver TEECO Cafe
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
int	 FileButtonPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	 FileButtonMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);

}

int	 FileButtonMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->Draw(parent, handle, command, param, Data);
	return(1);	
}

int FileButton(HANDLE ContainerHandle, int sx, int sy, int width, int height, char *NormalName, char *TouchName, int (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *))
{
	makeWindow(
				LCD,
				"File_Button",			// Windows Name
				ContainerHandle,		// Parent Window Handle
				WIDGET_WINDOW,			// Windows Kind
				STYLE_NONE,				// Windows Style
				sx, sy, 				// Start Position
				width, height,			// Size
				SHOW,					// Create after Show
				NULL,					// CreateEvent
				FileButtonPaintEvent,	// PaintEvent
				FileButtonMouseOnEvent,	// MouseOnEvent
				FileButtonMouseOffEvent,// MouseOffEvent
				NULL,					// MouseMoveEvent
				ClickEvent,				// GestureEvent
				NULL,					// ClickEvent
				NULL,					// TimerEvent
				NULL,  					// ControlEvent
				NULL,  					// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				0,						// PARAMETER
				0,						// Normal Touch Screen
				0,						// Touch Screen
				0,						// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	return(1);
}


#ifdef __cplusplus
}
#endif
