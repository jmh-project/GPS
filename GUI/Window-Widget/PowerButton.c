/******************************************************************************
*    	File name	: Button.c 
*		Description : TEECO GUI Window Control Widget [Button]
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
							Description
******************************************************************************/
//버튼의 종류는 3가지로 작성이 되었다.
//텍스트버튼, 이미지버튼, 아이콘버튼 
//아이콘버튼지정시 아이콘의 바탕색은 백색으로한다.

/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

/// 윈도우모양의 라운드버튼을 그리는 함수이다 누르기전의 모양을그린다..
/// 
extern int ButtonTextDraw(Windows *win, Graphics *Gr, int icon_size);
extern int ButtonIconDraw(Windows *win);
//선이겁나게 굵은 패널을 그릴때 사용한다.(파워포인트에 있는모양)
void PowerPanelDraw(int press, Windows *win, Graphics *Gr, int sx, int sy, int outline);


void PowerButtonDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	PowerPanelDraw(press, win, Gr, sx, sy, outline);
	ButtonTextDraw(win, Gr, 0);	
}

int	PowerButtonMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
//	int			text_width=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);
	//!< 버튼의 외각을그린다.
	PowerButtonDraw(ON, win, Gr, sx, sy, 1);

	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	PowerButtonMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
//	int			text_width;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);	
	//!< 버튼의 외각을그린다.
	PowerButtonDraw(OFF, win, Gr, sx, sy, 1);
	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	PowerButtonPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
	int			sx=0, sy=0;		//윈도우에 진입하면 좌표는 상대적으로 0, 0 부터 시작한다.
//	int			text_width;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	Gr->FillRect(0, 0, 0, 0);
	
	//!< 버튼의 외각을그린다.
	PowerButtonDraw(OFF, win, Gr, sx, sy, 1);

	if(command == UPDATE)
		Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 PowerButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			char	*text,								// 버튼에 표시되는 문자열
			U32		id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )
{
//	PARAM	parameter;
	HANDLE	handle;
	U32		w_id;
	WidgetStruct wid_struct;	
	
	Windows		*parent_win = GetWindow(NULL, ContainerHandle, "");
    if(parent_win==0)
    {
        Dprintf("PowerButton::Alloc Error\n");
		return(0);
    }
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );

	//!< 반드시 지워야한다.
	memset(&wid_struct, 0, sizeof(WidgetStruct));
	
	wid_struct.ID = id;	//생성되는 윈도우마다 가지는 구조체의 ID

    handle=makeWindow(
				LCD,						// Display Section Position
				"PowerButton",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,			// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				PowerButtonPaintEvent,			// PaintEvent
				PowerButtonMouseOnEvent,			// MouseOnEvent
				PowerButtonMouseOffEvent,		// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				ClickEvent,					// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				NULL,  						// ChangeEvent
                NULL,                       // KeyEvent
                NULL,                       // EncoderEvent
                NULL,           				// FocusEvent
                NULL,                       // CloseEvent
				NULL,					// PARAMETER
				NULL,				// Normal Touch Screen or BMP
				NULL,				// Touch Screen or ICON
				text,						// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	w_id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	//Dprintf("%s:: ID=%08x\n", __FUNCTION__, w_id);
	return(w_id);				
}				

#ifdef __cplusplus
}
#endif
