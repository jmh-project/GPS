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
extern int ButtonTextDraw(Windows *win, Graphics *Gr, int icon_size);
extern int ButtonIconDraw(Windows *win);


/// 윈도우모양의 라운드버튼을 그리는 함수이다 누르기전의 모양을그린다..
/// 
void RoundRectButtonDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy)
{
	Gr->SetColor(SYSTEMGRAY);
	Gr->FillRect(sx, sy, win->width, win->height);	
	int icon_size = ButtonIconDraw(win);

	if(press==OFF)
	{			
		Gr->SetColor(RGB565CONVERT(122, 122, 122));
		WinRoundRect2D(sx, sy, win->width-1, win->height-1);
		WinRoundRect2D(sx+1, sy+1, win->width-2-1, win->height-2-1);
		Gr->SetColor(WHITE);
		WinRoundRect2D(sx+2, sy+2, win->width-4-1, win->height-4-1);
		WinRoundRect2D(sx+3, sy+3, win->width-6-1, win->height-6-1);
	}
	else
	{
		Gr->SetColor(WHITE);
		WinRoundRect2D(sx, sy, win->width-1, win->height-1);
		WinRoundRect2D(sx+1, sy+1, win->width-2-1, win->height-2-1);
		Gr->SetColor(RGB565CONVERT(122, 122, 122));
		WinRoundRect2D(sx+2, sy+2, win->width-4-1, win->height-4-1);
		WinRoundRect2D(sx+3, sy+3, win->width-6-1, win->height-6-1);
	}
	ButtonTextDraw(win, Gr, icon_size);
}

int	RoundRectButtonMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;

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
	RoundRectButtonDraw(ON, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	RoundRectButtonMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;

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
	RoundRectButtonDraw(OFF, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	RoundRectButtonPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
	int			sx=0, sy=0;		//윈도우에 진입하면 좌표는 상대적으로 0, 0 부터 시작한다.

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);
	//!< 버튼의 외각을그린다.
	RoundRectButtonDraw(OFF, win, Gr, sx, sy);
	if(command == UPDATE)
		Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 RoundRectButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
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
        Dprintf("RoundRectButton::Alloc Error\n");
		return(0);
    }
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	//!< 반드시 지워야한다.
	memset(&wid_struct, 0, sizeof(WidgetStruct));
	
	wid_struct.ID = id;	//생성되는 윈도우마다 가지는 구조체의 ID

    handle=makeWindow(
				LCD,						// Display Section Position
				"RoundRectButton",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,			// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				RoundRectButtonPaintEvent,			// PaintEvent
				RoundRectButtonMouseOnEvent,			// MouseOnEvent
				RoundRectButtonMouseOffEvent,		// MouseOffEvent
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
	return(w_id);				
}				
#ifdef __cplusplus
}
#endif
