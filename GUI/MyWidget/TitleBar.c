/******************************************************************************
*    	File name	: TitleBarc 
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
#include "dos.h"
#include "DataLog.h"
     

/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
void VoicePlay(uint8_t bank);		 
void OuterPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int outline);	 
#define		TEXTBOX_FONT_HEIGHT	24
//좌우가 볼록한 패널을 그릴때 사용한다.
void TitleBarPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->FillRect(sx, sy, win->width, win->height);	

	//좌측
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->VLine(sx, sy, height);
	Gr->SetColor(RGB565CONVERT(204, 204, 204));
	Gr->VLine(sx+1, sy, height);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx+2, sy, height);
	Gr->SetColor(RGB565CONVERT(209, 209, 209));
	Gr->VLine(sx+3, sy, height);
	Gr->SetColor(RGB565CONVERT(211, 211, 211));
	Gr->VLine(sx+4, sy, height);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->VLine(sx+5, sy, height);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx+6, sy, height);
	Gr->SetColor(RGB565CONVERT(206, 206, 206));
	Gr->VLine(sx+7, sy, height);
	Gr->SetColor(RGB565CONVERT(202, 202, 202));
	Gr->VLine(sx+8, sy, height);
	Gr->SetColor(RGB565CONVERT(197, 197, 197));
	Gr->VLine(sx+9, sy, height);
	//우측
	Gr->SetColor(RGB565CONVERT(64, 64, 64));
	Gr->VLine(sx+width, sy, height);
	Gr->SetColor(RGB565CONVERT(112, 112, 112));
	Gr->VLine(sx+width-1, sy, height);
	Gr->SetColor(RGB565CONVERT(130, 130, 130));
	Gr->VLine(sx+width-2, sy, height);
	Gr->SetColor(RGB565CONVERT(139, 139, 139));
	Gr->VLine(sx+width-3, sy, height);
	Gr->SetColor(RGB565CONVERT(147, 147, 147));
	Gr->VLine(sx+width-4, sy, height);
	Gr->SetColor(RGB565CONVERT(153, 153, 153));
	Gr->VLine(sx+width-5, sy, height);
	Gr->SetColor(RGB565CONVERT(157, 157, 157));
	Gr->VLine(sx+width-6, sy, height);
	Gr->SetColor(RGB565CONVERT(162, 162, 162));
	Gr->VLine(sx+width-7, sy, height);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+width-8, sy, height);
	Gr->SetColor(RGB565CONVERT(170, 170, 170));
	Gr->VLine(sx+width-9, sy, height);
	Gr->SetColor(RGB565CONVERT(174, 174, 174));
	Gr->VLine(sx+width-10, sy, height);
	Gr->SetColor(RGB565CONVERT(180, 180, 180));
	Gr->VLine(sx+width-11, sy, height);
	Gr->SetColor(RGB565CONVERT(186, 186, 186));
	Gr->VLine(sx+width-12, sy, height);
	//상단
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->HLine(sx+1, sy, width-4);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->HLine(sx+3, sy+1, width-8);
	//하단
	Gr->SetColor(RGB565CONVERT(133, 133, 133));
	Gr->HLine(sx+1, sy+height, width-4);
	Gr->SetColor(RGB565CONVERT(165, 165, 165));
	Gr->HLine(sx+3, sy+height-1, width-8);	
	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
	//IconOut48G(0, 5, "TitleLogo.bmp");
	//IconDisplay(15, 0, "TitleLogo.bmp", RGB565CONVERT(191, 191, 191));
}

void TitleBarPanelDraw1(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->FillRect(sx, sy, win->width, win->height);	

	//좌측
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->VLine(sx, sy, height);
	Gr->SetColor(RGB565CONVERT(204, 204, 204));
	Gr->VLine(sx+1, sy+1, height-2);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx+2, sy+2, height-4);
	Gr->SetColor(RGB565CONVERT(209, 209, 209));
	Gr->VLine(sx+3, sy+3, height-6);
	Gr->SetColor(RGB565CONVERT(211, 211, 211));
	Gr->VLine(sx+4, sy+4, height-8);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->VLine(sx+5, sy+5, height-10);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx+6, sy+6, height-12);
	Gr->SetColor(RGB565CONVERT(206, 206, 206));
	Gr->VLine(sx+7, sy+7, height-14);
	Gr->SetColor(RGB565CONVERT(202, 202, 202));
	Gr->VLine(sx+8, sy+8, height-16);
	Gr->SetColor(RGB565CONVERT(197, 197, 197));
	Gr->VLine(sx+9, sy+9, height-18);
	//우측
	Gr->SetColor(RGB565CONVERT(64, 64, 64));
	Gr->VLine(sx+width, sy, height);
	Gr->SetColor(RGB565CONVERT(112, 112, 112));
	Gr->VLine(sx+width-1, sy+1, height-2);
	Gr->SetColor(RGB565CONVERT(130, 130, 130));
	Gr->VLine(sx+width-2, sy+2, height-4);
	Gr->SetColor(RGB565CONVERT(139, 139, 139));
	Gr->VLine(sx+width-3, sy+3, height-6);
	Gr->SetColor(RGB565CONVERT(147, 147, 147));
	Gr->VLine(sx+width-4, sy+4, height-8);
	Gr->SetColor(RGB565CONVERT(153, 153, 153));
	Gr->VLine(sx+width-5, sy+5, height-10);
	Gr->SetColor(RGB565CONVERT(157, 157, 157));
	Gr->VLine(sx+width-6, sy+6, height-12);
	Gr->SetColor(RGB565CONVERT(162, 162, 162));
	Gr->VLine(sx+width-7, sy+7, height-14);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+width-8, sy+8, height-16);
	Gr->SetColor(RGB565CONVERT(170, 170, 170));
	Gr->VLine(sx+width-9, sy+9, height-18);
	Gr->SetColor(RGB565CONVERT(174, 174, 174));
	Gr->VLine(sx+width-10, sy+10, height-20);
	Gr->SetColor(RGB565CONVERT(180, 180, 180));
	Gr->VLine(sx+width-11, sy+11, height-22);
	Gr->SetColor(RGB565CONVERT(186, 186, 186));
	Gr->VLine(sx+width-12, sy+12, height-24);
	//상단
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->HLine(sx+1, sy, width-4);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->HLine(sx+3, sy+1, width-8);
	//하단
	Gr->SetColor(RGB565CONVERT(133, 133, 133));
	Gr->HLine(sx+1, sy+height, width-4);
	Gr->SetColor(RGB565CONVERT(165, 165, 165));
	Gr->HLine(sx+3, sy+height-1, width-8);
	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
	//IconDisplay(15, 1, "TitleLogo.bmp", RGB565CONVERT(191, 191, 191));
}

void TitleBarPanelDraw2(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
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

    //IconDisplay(15, 1, "TitleLogo.bmp", RGB565CONVERT(191, 191, 191));	
}

int TitleBarTextDraw(Windows *win, Graphics *Gr)
{
	int	sx, sy;
	//!< 글씨를 쓰기위한 루틴, 문자가 있을때만 출력하게한다.
	if(strlen(win->Text))		
	{
		int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
		
		sx = (win->width - text_width) / 2;
		sx = 48+30;//ICON 사용시
        sx = 10;   //ICON 사용하지 않는 경우
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

void TitleBarDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy)
{
    //Dprintf("TitleBarDraw :: win->WidgetStyle = %d\n",win->WidgetStyle);
	if(win->WidgetStyle==1)
		TitleBarPanelDraw(win, Gr, sx, sy, 0);
	else if(win->WidgetStyle==2)
		TitleBarPanelDraw1(win, Gr, sx, sy, 0); 
	else if(win->WidgetStyle==3)
		TitleBarPanelDraw2(win, Gr, sx, sy, 0); 
	else if(win->WidgetStyle==4)
		OuterPanelDraw(win, Gr, sx, sy, 0); 
	
	TitleBarTextDraw(win, Gr);
}
int gCaptureToggle=0;
int	TitleBarMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
//	int			sx=0, sy=0;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	//마우스의 좌표를 읽을때 사용한다.
	//if(win->Attr == READ_ONLY) return(0);
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);	

	//if(WaterCellData.Capture==ON)
//	int 		tx = LOWORD(param);
//	int 		ty = HIWORD(param);	
//	static int	file_cnt=0;
//	{        
//		if(tx > 0 && tx < 50 && ty > 0 && ty < 50 && gCaptureToggle==0)
//		{
//            gCaptureToggle=1;
//            VoicePlay(2);
//			char	buf[64];
//			sprintf(buf,"%s_%d.bmp", win->Name, file_cnt++);
//			strcpy(Disk_Kind,"1:");	//USB Select
//			BMPCreateSD("0", "0", "1024", "768", buf);	
//            Dprintf("SCREEN COPY = %s\n ",win->Name);
//            DataLog_MessageBox_Window("화면캡쳐", "화면캡처완료.");
//            gCaptureToggle=0;
//		}
//	}	
	
	//TitleBarDraw(ON, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	TitleBarMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
//	int sx=0, sy=0;
	
	//if(win->Attr == READ_ONLY) return(0);	
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	//TitleBarDraw(OFF, win, Gr, sx, sy);

	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	TitleBarPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	TitleBarDraw(OFF, win, Gr, sx, sy);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 TitleBar(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		backcolor, 
			int		fontcolor,
			int		align,		//Left, Right, Center
			int		style,		//모양을 결정한다.
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )


{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "");

	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	wid_struct.FontColor = BLACK;
	wid_struct.FontNo = font_no;
	wid_struct.Style=style;
	
	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;	
//	wid_struct.Attr = attr;	
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"TitleBar",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				TitleBarPaintEvent,			// PaintEvent
				TitleBarMouseOnEvent,		// MouseOnEvent
				NULL,		// MouseOffEvent
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
