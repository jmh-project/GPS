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

//깊이들어간 패널을 그린다.
void SInnerBoxDraw(Graphics *Gr, int sx, int sy, int width, int height) {
	//채우는색
	Gr->SetColor(RGB565CONVERT(164, 164, 164));		
	Gr->FillRect(sx, sy, width, height);	

	//좌측, 상단라인
	Gr->SetColor(RGB565CONVERT(164, 164, 164));
	Gr->Rectangle(sx, sy, width, height);

	Gr->SetColor(RGB565CONVERT(147, 147, 147));
	Gr->Rectangle(sx+1, sy+1, width-2, height-2);

	Gr->SetColor(RGB565CONVERT(136, 136, 136));
	Gr->Rectangle(sx+2, sy+2, width-4, height-4);

	Gr->SetColor(RGB565CONVERT(130, 130, 130));
	Gr->Rectangle(sx+3, sy+3, width-6, height-6);
	//하단라인
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->HLine(sx, sy+height, width);

	Gr->SetColor(RGB565CONVERT(195, 195, 195));
	Gr->HLine(sx+1, sy+height-1, width-2);

	Gr->SetColor(RGB565CONVERT(190, 190, 190));
	Gr->HLine(sx+2, sy+height-2, width-4);
	
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->HLine(sx+3, sy+height-3, width-6);
	//우측라인
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->VLine(sx+width, sy+1, height-1);

	Gr->SetColor(RGB565CONVERT(195, 195, 195));
	Gr->VLine(sx+width-1, sy+2, height-4);

	Gr->SetColor(RGB565CONVERT(190, 190, 190));
	Gr->VLine(sx+width-2, sy+3, height-6);
	
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->VLine(sx+width-3, sy+4, height-8);	
}


void SButtonDraw_New(int  press, Windows *win, Graphics *Gr, int sx, int sy){
	int icon_size;
	// ColorCode
	//	230, 134, 112, 112-----------------------------------134, 230
	//	136, 143,          -----------------------------------  143, 136,
	//  112, 112, 256, 256
	//  112, 112, 256, 256
	//  112, 112, 256, 256
	//	136, 143,          -----------------------------------  143, 136,
	//	230, 134, 112, 112-----------------------------------134, 230		

	// System Color로 버튼의 사각영역을 그린다(채운다).
    
	//Gr->SetColor(RGB565CONVERT(230, 230, 230));
	//Gr->SetColor(RGB565CONVERT(25, 34, 37));
    //Gr->SetColor(RGB565CONVERT(2, 52, 77));
    Gr->SetColor(flash.systemColor);
    Gr->FillRect(sx, sy, win->width, win->height);	

	//라운드효과를위한 포인트처리
	if(press==OFF)
	{
//		icon_size = ButtonIconDraw(win) + 7;
//		Gr->SetColor(RGB565CONVERT(122, 122, 122));
//		// 122 Gray로 사각을 두겹그린다.
//		Gr->Rectangle(sx, sy, win->width-1, win->height-1);
//		Gr->Rectangle(sx+1, sy+1, win->width-3, win->height-3);		
//		Gr->SetColor(clWhite);
//		// 그안쪽으로 화이트로 사각을 두겹그린다.	
//		Gr->Rectangle(sx+2, sy+2, win->width-5, win->height-5);		
//		Gr->Rectangle(sx+3, sy+3, win->width-6, win->height-6);		

//		//모퉁이를 지운다.
//		Gr->SetColor(SYSTEMGRAY);
//		Gr->Plot(sx, sy);
//		Gr->Plot(sx+win->width-1, sy);
//		Gr->Plot(sx, sy+win->height-1);
//		Gr->Plot(sx+win->width-1, sy+win->height-1);

//		// 136 Color를 그린다.
//		Gr->SetColor(RGB565CONVERT(136, 136, 136));
//		Gr->Plot(sx, sy+1);
//		Gr->Plot(sx+win->width-1, sy+1);
//		Gr->Plot(sx, sy+win->height-2);
//		Gr->Plot(sx+win->width-1, sy+win->height-2);

//		Gr->SetColor(RGB565CONVERT(134, 134, 134));
//		Gr->Plot(sx+1, sy);
//		Gr->Plot(sx+win->width-1-1, sy);
//		Gr->Plot(sx+1, sy+win->height-1);
//		Gr->Plot(sx+win->width-1-1, sy+win->height-1);

//		Gr->SetColor(RGB565CONVERT(143, 143, 143));
//		Gr->Plot(sx+1, sy+1);
//		Gr->Plot(sx+win->width-1-1, sy+1);
//		Gr->Plot(sx+1, sy+win->height-1-1);
//		Gr->Plot(sx+win->width-1-1, sy+win->height-1-1);
//		
//		//내부흰색을 원처럼 보이게한다.
//		Gr->SetColor(RGB565CONVERT(143, 143, 143));		
//		Gr->Plot(sx+2, sy+2);
//		Gr->Plot(sx+win->width-1-2, sy+2);
//		Gr->Plot(sx+2, sy+win->height-1-2);
//		Gr->Plot(sx+win->width-1-2, sy+win->height-1-2);

		icon_size = ButtonIconDraw(win) + 7;
		Gr->SetColor(RGB565CONVERT(2, 52, 93));
		// 122 Gray로 사각을 두겹그린다.
		Gr->Rectangle(sx, sy, win->width-1, win->height-1);
		Gr->Rectangle(sx+1, sy+1, win->width-3, win->height-3);		
		Gr->SetColor(RGB565CONVERT(2, 52, 93));
		// 그안쪽으로 화이트로 사각을 두겹그린다.	
        Gr->SetColor(RGB565CONVERT(22, 22, 0));        
		Gr->Rectangle(sx+2, sy+2, win->width-5, win->height-5);		
        //Gr->SetColor(RGB565CONVERT(255, 255, 255));
		Gr->Rectangle(sx+3, sy+3, win->width-6, win->height-6);		
		
//		//모퉁이를 지운다.
//		Gr->SetColor(SYSTEMGRAY);
//		Gr->Plot(sx, sy);
//		Gr->Plot(sx+win->width-1, sy);
//		Gr->Plot(sx, sy+win->height-1);
//		Gr->Plot(sx+win->width-1, sy+win->height-1);

//		// 136 Color를 그린다.
//		Gr->SetColor(RGB565CONVERT(136, 136, 136));
//		Gr->Plot(sx, sy+1);
//		Gr->Plot(sx+win->width-1, sy+1);
//		Gr->Plot(sx, sy+win->height-2);
//		Gr->Plot(sx+win->width-1, sy+win->height-2);

//		Gr->SetColor(RGB565CONVERT(134, 134, 134));
//		Gr->Plot(sx+1, sy);
//		Gr->Plot(sx+win->width-1-1, sy);
//		Gr->Plot(sx+1, sy+win->height-1);
//		Gr->Plot(sx+win->width-1-1, sy+win->height-1);

//		Gr->SetColor(RGB565CONVERT(143, 143, 143));
//		Gr->Plot(sx+1, sy+1);
//		Gr->Plot(sx+win->width-1-1, sy+1);
//		Gr->Plot(sx+1, sy+win->height-1-1);
//		Gr->Plot(sx+win->width-1-1, sy+win->height-1-1);
//		
//		//내부흰색을 원처럼 보이게한다.
//		Gr->SetColor(RGB565CONVERT(143, 143, 143));		
//		Gr->Plot(sx+2, sy+2);
//		Gr->Plot(sx+win->width-1-2, sy+2);
//		Gr->Plot(sx+2, sy+win->height-1-2);
//		Gr->Plot(sx+win->width-1-2, sy+win->height-1-2);
	}
	else
	{
		SInnerBoxDraw(Gr,  sx, sy, win->width, win->height);
		icon_size = ButtonIconDraw(win) + 7;

		/*
		Gr->SetColor(clWhite);
		// 122 Gray로 사각을 두겹그린다.
		Gr->Rectangle(sx, sy, win->width-1, win->height-1);
		Gr->Rectangle(sx+1, sy+1, win->width-3, win->height-3);		
		Gr->SetColor(RGB565CONVERT(122, 122, 122));
		// 그안쪽으로 화이트로 사각을 두겹그린다.	
		Gr->Rectangle(sx+2, sy+2, win->width-5, win->height-5);		
		Gr->Rectangle(sx+3, sy+3, win->width-6, win->height-6);		

		//모퉁이를 지운다.
		Gr->SetColor(clWhite);
		Gr->Plot(sx, sy);
		Gr->Plot(sx+win->width-1, sy);
		Gr->Plot(sx, sy+win->height-1);
		Gr->Plot(sx+win->width-1, sy+win->height-1);		
		*/
	}	
	ButtonTextDraw  (win, Gr, icon_size);	
    gv.buttonWatchTimerCount=0;
}

void SButtonDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy)
{
	int icon_size;
	// ColorCode
	//	230, 134, 112, 112-----------------------------------134, 230
	//	136, 143,          -----------------------------------  143, 136,
	//  112, 112, 256, 256
	//  112, 112, 256, 256
	//  112, 112, 256, 256
	//	136, 143,          -----------------------------------  143, 136,
	//	230, 134, 112, 112-----------------------------------134, 230		

	// System Color로 버튼의 사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(230, 230, 230));
	Gr->FillRect(sx, sy, win->width, win->height);	

	//라운드효과를위한 포인트처리
	if(press==OFF)
	{
		icon_size = ButtonIconDraw(win) + 7;
		Gr->SetColor(RGB565CONVERT(122, 122, 122));
		// 122 Gray로 사각을 두겹그린다.
		Gr->Rectangle(sx, sy, win->width-1, win->height-1);
		Gr->Rectangle(sx+1, sy+1, win->width-3, win->height-3);		
		Gr->SetColor(clWhite);
		// 그안쪽으로 화이트로 사각을 두겹그린다.	
		Gr->Rectangle(sx+2, sy+2, win->width-5, win->height-5);		
		Gr->Rectangle(sx+3, sy+3, win->width-6, win->height-6);		

		//모퉁이를 지운다.
		Gr->SetColor(SYSTEMGRAY);
		Gr->Plot(sx, sy);
		Gr->Plot(sx+win->width-1, sy);
		Gr->Plot(sx, sy+win->height-1);
		Gr->Plot(sx+win->width-1, sy+win->height-1);

		// 136 Color를 그린다.
		Gr->SetColor(RGB565CONVERT(136, 136, 136));
		Gr->Plot(sx, sy+1);
		Gr->Plot(sx+win->width-1, sy+1);
		Gr->Plot(sx, sy+win->height-2);
		Gr->Plot(sx+win->width-1, sy+win->height-2);

		Gr->SetColor(RGB565CONVERT(134, 134, 134));
		Gr->Plot(sx+1, sy);
		Gr->Plot(sx+win->width-1-1, sy);
		Gr->Plot(sx+1, sy+win->height-1);
		Gr->Plot(sx+win->width-1-1, sy+win->height-1);

		Gr->SetColor(RGB565CONVERT(143, 143, 143));
		Gr->Plot(sx+1, sy+1);
		Gr->Plot(sx+win->width-1-1, sy+1);
		Gr->Plot(sx+1, sy+win->height-1-1);
		Gr->Plot(sx+win->width-1-1, sy+win->height-1-1);
		
		//내부흰색을 원처럼 보이게한다.
		Gr->SetColor(RGB565CONVERT(143, 143, 143));		
		Gr->Plot(sx+2, sy+2);
		Gr->Plot(sx+win->width-1-2, sy+2);
		Gr->Plot(sx+2, sy+win->height-1-2);
		Gr->Plot(sx+win->width-1-2, sy+win->height-1-2);
	}
	else
	{
		SInnerBoxDraw(Gr,  sx, sy, win->width, win->height);
		icon_size = ButtonIconDraw(win) + 7;

		/*
		Gr->SetColor(clWhite);
		// 122 Gray로 사각을 두겹그린다.
		Gr->Rectangle(sx, sy, win->width-1, win->height-1);
		Gr->Rectangle(sx+1, sy+1, win->width-3, win->height-3);		
		Gr->SetColor(RGB565CONVERT(122, 122, 122));
		// 그안쪽으로 화이트로 사각을 두겹그린다.	
		Gr->Rectangle(sx+2, sy+2, win->width-5, win->height-5);		
		Gr->Rectangle(sx+3, sy+3, win->width-6, win->height-6);		

		//모퉁이를 지운다.
		Gr->SetColor(clWhite);
		Gr->Plot(sx, sy);
		Gr->Plot(sx+win->width-1, sy);
		Gr->Plot(sx, sy+win->height-1);
		Gr->Plot(sx+win->width-1, sy+win->height-1);		
		*/
	}	
	ButtonTextDraw(win, Gr, icon_size);	
    gv.buttonWatchTimerCount=0;
}

int	SButtonMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	if(flash.systemColorNo!=6) {
		if(win->Toggle==PUSH_BUTTON || (win->Toggle==TOGGLE_BUTTON && win->ToggleStatus==OFF))
		{
			win->ToggleStatus = ON;
			//!< 버튼의 외각을그린다.
			if(flash.systemColorNo!=6) {
				SButtonDraw_New(ON, win, Gr, sx, sy);
			}
			else {
				SButtonDraw(ON, win, Gr, sx, sy);
			}
		}
		else
		{
			win->ToggleStatus = OFF;
			if(flash.systemColorNo!=6) {
				SButtonDraw_New(OFF, win, Gr, sx, sy);
			}
			else {
				SButtonDraw(OFF, win, Gr, sx, sy);
			}
		}
		Gr->Draw(parent, handle, command, param, Data);	
	}
	else {
		if(win->Toggle==PUSH_BUTTON || (win->Toggle==TOGGLE_BUTTON && win->ToggleStatus==OFF))
		{
			win->ToggleStatus = ON;
			//!< 버튼의 외각을그린다.
			if(flash.systemColorNo!=6) {
				SButtonDraw_New(ON, win, Gr, sx, sy);
			}
			else {
				SButtonDraw(ON, win, Gr, sx, sy);
			}
		}
		else
		{
			win->ToggleStatus = OFF;
			if(flash.systemColorNo!=6) {
				SButtonDraw_New(OFF, win, Gr, sx, sy);
			}
			else {
				SButtonDraw(OFF, win, Gr, sx, sy);
			}
		}
		Gr->Draw(parent, handle, command, param, Data);	
	}
    //Dprintf("MOUSE ON\n");
	return(1);
}

int	SButtonMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	if(flash.systemColorNo!=6) {
		SButtonDraw_New(OFF, win, Gr, sx, sy);
	}
	else {
		SButtonDraw(OFF, win, Gr, sx, sy);
	}
	
	Gr->Draw(parent, handle, command, param, Data);	
    
    
    
    //Dprintf("MOUSE OFF\n");
	return(1);
}

int	SButtonPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	//토글기능에서는 OFF 이벤트가 필요없다.
	if(win->Toggle==TOGGLE_BUTTON) 
	{
		win->MouseOffEvent = NULL;
		win->ToggleStatus=command;		
	}

	Gr->FillRect(0, 0, 0, 0);
	
	//!< 버튼의 외각을그린다.
	if(flash.systemColorNo!=6) {
		SButtonDraw_New(command, win, Gr, sx, sy);
	}
	else {
		SButtonDraw(command, win, Gr, sx, sy);
	}

	if(command == UPDATE)
		Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 SButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			char	*IconName,								// 버튼에 표시되는 아이콘
			char	*text,								// 버튼에 표시되는 문자열
			int	function,							// 토글버튼, 푸쉬버튼의 구분				
			U32	id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )
{
//	PARAM	parameter;
	HANDLE	handle;
	U32		w_id;
	WidgetStruct wid_struct;	
	
	Windows		*parent_win = GetWindow(NULL, ContainerHandle, "");
    if(parent_win==0)
    {
        Dprintf("SButton::Alloc Error\n");
		return(0);
    }
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );

	//!< 반드시 지워야한다.
	memset(&wid_struct, 0, sizeof(WidgetStruct));
	
	wid_struct.ID = id;	//생성되는 윈도우마다 가지는 구조체의 ID
	strcpy(wid_struct.IconName, IconName);
	wid_struct.Toggle = function;
	
    handle=makeWindow(
				LCD,						// Display Section Position
				"SButton",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,			// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				SButtonPaintEvent,			// PaintEvent
				SButtonMouseOnEvent,			// MouseOnEvent
				SButtonMouseOffEvent,		// MouseOffEvent
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
