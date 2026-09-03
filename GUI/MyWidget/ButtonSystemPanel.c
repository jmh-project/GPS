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
#if 0
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
#define		TEXTBOX_FONT_HEIGHT	24
//좌우가 볼록한 패널을 그릴때 사용한다.
void LeftRightPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int outline);
//다이아몬드스타일의 버튼을 그린다.
void DiamondButton(Windows *win, Graphics *Gr, int sx, int sy, int outline);
void OuterPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int outline);
void UpDownPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int outline);
void TabPanelDrawRow(Windows *win, Graphics *Gr, int sx, int sy, int outline);
void TabPanelDrawColume(Windows *win, Graphics *Gr, int sx, int sy, int outline);
void PowerPanelDraw(int press, Windows *win, Graphics *Gr, int sx, int sy, int outline);

int ButtonSystemPanelTextDraw(Windows *win, Graphics *Gr)
{
	int	sx, sy;
	//!< 글씨를 쓰기위한 루틴, 문자가 있을때만 출력하게한다.
	if(strlen(win->Text))		
	{
		int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
		
		sx = (win->width - text_width) / 2;
		sy = (win->height - TEXTBOX_FONT_HEIGHT) / 2;

		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;

		SetFontAliasing("ng_eh_24.sif");
		if(win->FontColor==0)
			Gr->TextOutAliasing(sx, sy, win->Text, clBlack, 0, "ng_eh_24.sif");
		else
			Gr->TextOutAliasing(sx, sy, win->Text, clBlack, 1, "ng_eh_24.sif");
	}		
	return(1);
}

void ButtonSystemPanelDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy)
{
	if(press==OFF)
	{
		if(win->WidgetStyle==1)
			LeftRightPanelDraw(win, Gr, sx, sy, 0); //좌우가 볼록한 패널을 그릴때 사용한다.(아웃라인이없다)
		else if(win->WidgetStyle==2)
			DiamondButton(win, Gr, sx, sy, 0);	
		else if(win->WidgetStyle==3)
			LeftRightPanelDraw(win, Gr, sx, sy, 1);//좌우가 볼록한 패널을 그릴때 사용한다.(아웃라인이있다)
		else if(win->WidgetStyle==4)
			DiamondButton(win, Gr, sx, sy, 1);	
		else if(win->WidgetStyle==5)
			OuterPanelDraw(win, Gr, sx, sy, 0);	 //선이굵은 패널을 그릴때 사용한다.
		else if(win->WidgetStyle==6)
			OuterPanelDraw(win, Gr, sx, sy, 1);	//선이굵은 패널을 그릴때 사용한다.
		else if(win->WidgetStyle==7)
			UpDownPanelDraw(win, Gr, sx, sy, 0);	//상하가 볼록한 패널을 그릴때 사용한다.

		else if(win->WidgetStyle==8)
			UpDownPanelDraw(win, Gr, sx, sy, 1);	//상하가 볼록한 패널을 그릴때 사용한다.
	}
	else
	{
		Gr->SetColor(clCyan);
		Gr->FillRect(0, 0, win->width, win->height);
		Gr->SetColor(clBlack);
		Gr->Rectangle(0, 0, win->width, win->height);
	}
	ButtonSystemPanelTextDraw(win, Gr);
}
#define		TOUCH_SOUND			1
void VoicePlay(uint8_t bank);
int	ButtonSystemPanelMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);	
    VoicePlay(TOUCH_SOUND);    	
	if(win->Toggle==PUSH_BUTTON || (win->Toggle==TOGGLE_BUTTON && win->ToggleStatus==OFF))
	{
		win->ToggleStatus = ON;
		//!< 버튼의 외각을그린다.
		ButtonSystemPanelDraw(ON, win, Gr, sx, sy);
	}
	else
	{
		win->ToggleStatus = OFF;
		ButtonSystemPanelDraw(OFF, win, Gr, sx, sy);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	ButtonSystemPanelMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	
	//if(win->Attr == READ_ONLY) return(0);	
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	ButtonSystemPanelDraw(OFF, win, Gr, sx, sy);

	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	ButtonSystemPanelPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	
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
	//토글기능에서는 OFF 이벤트가 필요없다.
	if(win->Toggle==TOGGLE_BUTTON) 
	{
		win->MouseOffEvent = NULL;
		win->ToggleStatus=command;		
	}	
	ButtonSystemPanelDraw(command, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 ButtonSystemPanel(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			char	*text,		//init Character
			int	align,		//Left, Right, Center
			int	style,		//모양을 결정한다.
			int	function,							// 토글버튼, 푸쉬버튼의 구분								
			int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )

{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "");

	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
//	wid_struct.BackColor = backcolor;
//	wid_struct.FontColor = fontcolor;
	wid_struct.FontColor = BLACK;
//	wid_struct.FontNo = font_no;
	wid_struct.Style=style;
	
//	wid_struct.FontGap = 4;
//	wid_struct.FontSpaceGap = 7;	
//	wid_struct.Attr = attr;	
	wid_struct.Toggle = function;
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"ButtonSystemPanel",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				ButtonSystemPanelPaintEvent,			// PaintEvent
				ButtonSystemPanelMouseOnEvent,		// MouseOnEvent
				ButtonSystemPanelMouseOffEvent,		// MouseOffEvent
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
#endif
#ifdef __cplusplus
}
#endif					
