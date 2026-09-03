/**
  ******************************************************************************
  * @file       IconBox.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월7일
  * @brief    이파일은  윈도우 위젯의 아이콘박스를 구현한 파일이다.
                
  *          
  * @attention
  * 이파일은 자이언텍 지적재산권이므로 타사에 소스를 제공해서는 안됩니다. 
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc.</center></h2>
  * @warning  
  * @verbatim
  *  Edit History
  * @endverbatim  
  ******************************************************************************  
  */
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"
#include "WinStyleDraw.h"

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int IconBox_IconDraw(Windows *win)
{
	char	icon_buf[32];
	int	icon_size=0;
	int	sy_icon = 0;
	int	sx_icon = 0;
	
	//!< 아이콘이름이 지정이 되어있으면 아이콘을 그린다.
	if(strlen(win->IconName)) 
	{
		//!< 아이콘 파일명의 첫글자를 보고 사이즈를 유츨한다.
		strncpy(icon_buf, win->IconName, 3);		// 32_. 48_, 64_, 32G, 48G, 64G
		icon_buf[3] = 0;
		//Dprintf("%s:: iconName=%s Header=%s\n", __FUNCTION__, win->IconName, icon_buf);
		//!< 아이콘이 들어갈영역이 되어야만 아이콘을 그린다.
		if(win->height > icon_size)
		{
			if(!(strcmp(icon_buf, "32_")))	
			{ 
				icon_size = 32;    
				sx_icon = (win->width - icon_size) / 2;
				IconOut32W(sx_icon, sy_icon, win->IconName); 
			}
			else if(!(strcmp(icon_buf, "48_")))	
			{ 
				icon_size = 48;	
				sx_icon = (win->width - icon_size) / 2;
				IconOut48W(sx_icon, sy_icon, win->IconName); 
			}
#ifdef LCD_TS8080			
			else if(!(strcmp(icon_buf, "64_")))	
			{ 
				icon_size = 64;	
				sx_icon = (win->width - icon_size) / 2;
				IconOut64W(sx_icon, sy_icon, win->IconName); 
			}
#endif			
			else if(!(strcmp(icon_buf, "32G")))	
			{ 
				icon_size = 32;	
				sx_icon = (win->width - icon_size) / 2;
				IconOut32G(sx_icon, sy_icon, win->IconName); 
			}
			else if(!(strcmp(icon_buf, "48G")))	
			{ 
				icon_size = 48;	
				sx_icon = (win->width - icon_size) / 2;
				IconOut48G(sx_icon, sy_icon, win->IconName); 
			}
#ifdef LCD_TS8080
			else if(!(strcmp(icon_buf, "64G")))	
			{ 
				icon_size = 64;	
				sx_icon = (win->width - icon_size) / 2;
				IconOut64G(sx_icon, sy_icon, win->IconName); 
			}		
#endif			
			//Dprintf("iconsize=%d sx=%d sy=%d \n", icon_size, sx_icon, sy_icon);	
		}
		else		
		{
			Dprintf("%s:: ICON Draw Size Small\n", __FUNCTION__);
			icon_size=0;
		}
	}
	else 
	{
		icon_size=0;
	}
	return(icon_size);
}

#define		ICONBOX_FONT_HEIGHT	24
#define		ICON_TEXT_INTERVAL		6
int IconTextDraw(Windows *win, Graphics *Gr, int icon_size)
{
	int	sx=0, sy=0;
	//!< 글씨를 쓰기위한 루틴, 문자가 있을때만 출력하게한다.
	if(strlen(win->Text))		
	{
		/*
		int text_width = Get_CFontText_Width(20, win->Text);
		sx = (win->width - text_width) / 2;
		*/
		//sy = (win->height - ICONBOX_FONT_HEIGHT) / 2;

		sy += icon_size;
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		
		//SystemFontSet(WHITE, SYSTEM_FONT_20);
		//Gr->CTextOut(sx, sy+ICON_TEXT_INTERVAL, win->Text);
		SetFontAliasing("ng_eh_24.sif");
		TextOutAliasing(sx, sy+ICON_TEXT_INTERVAL, win->Text, clBlack, 0, "ng_eh_24.sif");		
		//Dprintf("Text = %s x=%d y=%d \n", win->Text, sx, sy);
	}		
	return(1);
}

void IconBoxDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy)
{
	Gr->SetColor(WHITE);
	Gr->FillRect(sx, sy, win->width, win->height);	
	int icon_size = IconBox_IconDraw(win);

	if(press==OFF)
	{			
		//Gr->SetColor(WHITE);
		//WinRoundRect2D(sx, sy, win->width-1, win->height-1);
		OuterPanel_Draw(win, Gr, sx, sy, win->width, win->height);
	}
	else
	{
		//Gr->SetColor(BLACK);
		//WinRoundRect2D(sx, sy, win->width-1, win->height-1);
		InnerPanel_Draw(win, Gr, sx, sy, win->width, win->height);
	}
	IconTextDraw(win, Gr, icon_size);
}

int	IconBoxMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	IconBoxDraw(ON, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	IconBoxMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	IconBoxDraw(OFF, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	IconBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	IconBoxDraw(OFF, win, Gr, sx, sy);
	if(command==UPDATE)
		Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


U32 IconBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			char	*IconName,
			char	*Text,
			int	id,
			int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) 
)
{
	HANDLE		handle;
	Windows		*parent_win;
	WidgetStruct wid_struct;	
	parent_win = GetWindow(NULL, ContainerHandle,"IconBox");

	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	//!< memset를 사용해서 내부를 반드시 지워야한다.
	memset(&wid_struct, 0, sizeof(WidgetStruct));	
	//!< 아이콘이름은 32자를 넘으면 안된다.
	int len = strlen(IconName);
	if(len < 32)
		strcpy(wid_struct.IconName, IconName);
	wid_struct.ID = id;	//생성되는 윈도우마다 가지는 구조체의 ID
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"IconBox",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				IconBoxPaintEvent,			// PaintEvent
				IconBoxMouseOnEvent,						// MouseOnEvent
				IconBoxMouseOffEvent,						// MouseOffEvent
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
				NULL,						// PARAMETER
				IconName,						// Image
				NULL,						// 
				Text,						// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	U32 w_id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	return(w_id);
}				

#ifdef __cplusplus
}
#endif
