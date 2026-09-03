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

#define		BUTTON_FONT_HEIGHT	24
#define		ICON_TEXT_INTERVAL		5
char gLabelTwoLineText[20];
int ButtonIconDraw(Windows *win);
void NInnerBoxDraw(Graphics *Gr, int sx, int sy, int width, int height);
int LineTwoLabelTextDraw(Windows *win, Graphics *Gr, int icon_size, int line)
{
	int	sx, sy;
	//!< 글씨를 쓰기위한 루틴, 문자가 있을때만 출력하게한다.
	if(strlen(win->Text))		
	{
		//int text_width = Get_CFontText_Width(20, win->Text);
		SetFontAliasing("ng_eh_24.sif");
		int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
		
		text_width += 0;
		sx = (win->width - text_width) / 2;
        if(line == 1)
        {
            sy = 4;
        }
        else if(line == 2)
        {
            sy = win->height / 2;
        }

		sx += icon_size;
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;

		Gr->TextOutAliasing(sx+ICON_TEXT_INTERVAL, sy, win->Text, clBlack, 0, "ng_eh_24.sif");
 	}		
	return(1);
}
void LineTwoLabelDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy)
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
	//Gr->SetColor(RGB565CONVERT(230, 230, 230));
    Gr->SetColor(RGB565CONVERT(190,  190,  190));
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
		NInnerBoxDraw(Gr,  sx, sy, win->width, win->height);
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
    icon_size = 0;
    //win->height -= 20;
	LineTwoLabelTextDraw(win, Gr, icon_size, 1);	

    //win->height += 40;
    strcpy(win->Text, gLabelTwoLineText);
	LineTwoLabelTextDraw(win, Gr, icon_size, 2);	
    
}
int	LineTwoLabelPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	//부모윈도우의 바탕색을 칠한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	
	//Gr->SetColor(RGB565CONVERT(255, 255,  0));
    /*
    Gr->SetColor(RGB565CONVERT(190,  190,  190));
	Gr->FillRect(0, 0, win->width-2, win->height-2);
    
	SetFontAliasing("ng_eh_24.sif");
	int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
    
    sx = (win->width -text_width) / 2;
	sy = (win->height - 24) / 2;	
    
	if(win->FontColor==clBlack)
		Gr->TextOutAliasing(sx, sy, win->Text, clBlack, 0, "ng_eh_24.sif");
	else if(win->FontColor==clWhite)
		Gr->TextOutAliasing(sx, sy, win->Text, clWhite, 0, "ng_eh_24.sif");
	else if(win->FontColor==clYellow)
		Gr->TextOutAliasing(sx, sy, win->Text, clYellow, 0, "ng_eh_24.sif");
	*/
    LineTwoLabelDraw(OFF, win, Gr, sx, sy);
    Gr->Draw(parent, handle, command, param, Data);	
	
    return(1);
}

U32 LineTwoLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int	fontcolor, char *text2)
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"LineBar");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
    	
	//wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
    strcpy(gLabelTwoLineText, text2);
	handle=makeWindow(
				LCD,						// Display Section Position
				"Label",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				LineTwoLabelPaintEvent,			// PaintEvent
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
				text,							// Windows Text
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
