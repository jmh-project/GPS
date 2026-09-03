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

int STextBoxTextDraw(Windows *win, Graphics *Gr)
{
	int	sx, sy;
	int text_width;
	//!< 글씨를 쓰기위한 루틴, 문자가 있을때만 출력하게한다.
	if(strlen(win->Text))		
	{
		//Dprintf("Font No=%d\n", win->FontNo);
		if(win->FontNo == 24)
		{
			SetFontAliasing("ng_eh_24.sif");
			text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
			//sx = (win->width - text_width) / 2;
			sy = (win->height - 24) / 2;
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
			//Dprintf("%s:: 24Size=> text_wid=%d sx=%d sy=%d \n", __FILE__, text_width,  sx, sy);
			if(win->BackColor==clWhite)
				Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 0, "ng_eh_24.sif");
			else if(win->BackColor==clBlack && (win->FontColor==clWhite || win->FontColor==clYellow))
				Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 0, "ng_eh_24.sif");
			else
			{
			}
		}
		else if(win->FontNo == 32)
		{
		}
		else if(win->FontNo == 48)
		{
			SetFontAliasing("mg48_e_Bold.sif");
			text_width = TextLenAliasing(win->Text, "mg48_e_Bold.sif");
			//sx = (win->width - text_width) / 2;
			sy = (win->height - 48) / 2;
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
			if(win->BackColor==clWhite)
				Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 0, "mg48_e_Bold.sif");
			else if(win->BackColor==clBlack && (win->FontColor==clWhite || win->FontColor==clYellow))
			{
				Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 0, "mg48_e_Bold.sif");
			}
			else {
				Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 0, "mg48_e_Bold.sif");
			}
		}
		else if(win->FontNo == 64)
		{
			//SetFontAliasing("mg64_e_Bold.sif"); //ng_e_64_Bold.sif
			//text_width = TextLenAliasing(win->Text, "mg64_e_Bold.sif"); //ng_e_64_Bold.sif
			SetFontAliasing("mg64_e_Bold.sif"); //ng_e_64_Bold.sif
			text_width = TextLenAliasing(win->Text, "mg64_e_Bold.sif"); //ng_e_64_Bold.sif
			//sx = (win->width - text_width) / 2;
			sy = (win->height - 64) / 2;
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
			if(win->BackColor==clWhite)
				Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 0, "mg64_e_Bold.sif");
			else if(win->BackColor==clBlack && (win->FontColor==clWhite || win->FontColor==clYellow))
				Gr->TextOutAliasing(sx, sy, win->Text, win->FontColor, 0, "mg64_e_Bold.sif");			
		}
	}		
	return(1);
}

void STextBoxStyle_Draw(Windows *win, Graphics *Gr, int sx, int sy, int width, int height)
{
	//스타일을 그린다.
	Gr->SetColor(BLACK);
	Gr->Line(sx, sy, sx, sy+height-1);		//바깥쪽 좌측 수직라인그리기		
	Gr->Line(sx, sy, sx+width-1, sy);			//바깥쪽 상단 수평라인그리기		
	
	Gr->SetColor(WHITE);	
	Gr->Line(sx+width-1, sy+1, sx+width-1, sy+1+height);		//바깥쪽우측 수직라인
	Gr->Line(sx+1, sy+height-1, sx+1+width, sy+height-1);	//바깥쪽아래 수평라인
	
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
	Gr->Line(sx+1, sy+1, sx+1, sy+height-2);	//좌측 안쪽회색 수직라인		
	Gr->Line(sx+1, sy+1, sx+width-2, sy+1);		//상단 안쪽회색 수평라인
}

void STextBoxDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy)
{
	int	width = win->width-1;
	int	height = win->height-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(win->BackColor);		
	Gr->FillRect(sx, sy, win->width, win->height);	

	STextBoxStyle_Draw(win, Gr,  sx, sy, width, height);
	if(press==ON)
	{
		Gr->SetColor(clRed);		
		Gr->Rectangle(sx+3, sy+3, win->width-6, win->height-6);	
	}	
	STextBoxTextDraw(win, Gr);
}
		
int	STextBoxMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	if(win->Attr == READ_ONLY) return(0);
	
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);	
	STextBoxDraw(ON, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	STextBoxMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	
	if(win->Attr == READ_ONLY) return(0);	
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	STextBoxDraw(OFF, win, Gr, sx, sy);

	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	STextBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	STextBoxDraw(OFF, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 STextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,		//24 는 24사이즈, 48, 64 3가지가 준비되어있다.
			char	*text,		//init Character
			int	fontcolor, 
			int	backcolor,
			int	align,		//Left, Right, Center
			int	attr,		//READ_ONLY, RW_MODE
			U32	id,
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )


{
	HANDLE		handle;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "STextBox");

	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	//wid_struct.FontColor = BLACK;
	wid_struct.FontNo = font_no;

	wid_struct.Attr = attr;	
	wid_struct.ID = id;	//생성되는 윈도우마다 가지는 구조체의 ID
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"STextBox",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				STextBoxPaintEvent,			// PaintEvent
				STextBoxMouseOnEvent,		// MouseOnEvent
				STextBoxMouseOffEvent,		// MouseOffEvent
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
#ifdef __cplusplus
}
#endif					
