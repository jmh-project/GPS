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
#define		TITLE_HEIGHT		45

/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
//다이아몬드스타일의 버튼을 그린다.
void DiamondPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int swidth, int sheight, int outline) {
	int	width = swidth-1;
	int	height = sheight-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
    
	Gr->FillRect(sx, sy, width, height);	

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
}

//좌우가 볼록한 패널을 그릴때 사용한다. 위젯용이다.
void GroupPanelDraw_New(Windows *win, Graphics *Gr, int sx, int sy, int swidth, int sheight, int outline) {
	int	width = swidth-1;
	int	height = sheight-1;
	// System Color로  사각영역을 그린다(채운다).
	
    //Gr->SetColor(RGB565CONVERT(191, 191, 191));
    //Gr->SetColor(RGB565CONVERT(25, 33, 37));
    Gr->SetColor(flash.systemGroupPanelColor);
	Gr->FillRect(sx, sy, width, height);	

	//좌측
//	Gr->SetColor(RGB565CONVERT(187, 187, 187));
//	Gr->VLine(sx, sy, height);
//	Gr->SetColor(RGB565CONVERT(204, 204, 204));
//	Gr->VLine(sx+1, sy, height);
//	Gr->SetColor(RGB565CONVERT(208, 208, 208));
//	Gr->VLine(sx+2, sy, height);
//	Gr->SetColor(RGB565CONVERT(209, 209, 209));
//	Gr->VLine(sx+3, sy, height);
//	Gr->SetColor(RGB565CONVERT(211, 211, 211));
//	Gr->VLine(sx+4, sy, height);
//	Gr->SetColor(RGB565CONVERT(210, 210, 210));
//	Gr->VLine(sx+5, sy, height);
//	Gr->SetColor(RGB565CONVERT(208, 208, 208));
//	Gr->VLine(sx+6, sy, height);
//	Gr->SetColor(RGB565CONVERT(206, 206, 206));
//	Gr->VLine(sx+7, sy, height);
//	Gr->SetColor(RGB565CONVERT(202, 202, 202));
//	Gr->VLine(sx+8, sy, height);
//	Gr->SetColor(RGB565CONVERT(197, 197, 197));
//	Gr->VLine(sx+9, sy, height);
//	//우측
//	Gr->SetColor(RGB565CONVERT(64, 64, 64));
//	Gr->VLine(sx+width, sy, height);
//	Gr->SetColor(RGB565CONVERT(112, 112, 112));
//	Gr->VLine(sx+width-1, sy, height);
//	Gr->SetColor(RGB565CONVERT(130, 130, 130));
//	Gr->VLine(sx+width-2, sy, height);
//	Gr->SetColor(RGB565CONVERT(139, 139, 139));
//	Gr->VLine(sx+width-3, sy, height);
//	Gr->SetColor(RGB565CONVERT(147, 147, 147));
//	Gr->VLine(sx+width-4, sy, height);
//	Gr->SetColor(RGB565CONVERT(153, 153, 153));
//	Gr->VLine(sx+width-5, sy, height);
//	Gr->SetColor(RGB565CONVERT(157, 157, 157));
//	Gr->VLine(sx+width-6, sy, height);
//	Gr->SetColor(RGB565CONVERT(162, 162, 162));
//	Gr->VLine(sx+width-7, sy, height);
//	Gr->SetColor(RGB565CONVERT(166, 166, 166));
//	Gr->VLine(sx+width-8, sy, height);
//	Gr->SetColor(RGB565CONVERT(170, 170, 170));
//	Gr->VLine(sx+width-9, sy, height);
//	Gr->SetColor(RGB565CONVERT(174, 174, 174));
//	Gr->VLine(sx+width-10, sy, height);
//	Gr->SetColor(RGB565CONVERT(180, 180, 180));
//	Gr->VLine(sx+width-11, sy, height);
//	Gr->SetColor(RGB565CONVERT(186, 186, 186));
//	Gr->VLine(sx+width-12, sy, height);
//	//상단
//	Gr->SetColor(RGB565CONVERT(208, 208, 208));
//	Gr->HLine(sx+1, sy, width-4);
//	Gr->SetColor(RGB565CONVERT(210, 210, 210));
//	Gr->HLine(sx+3, sy+1, width-8);
//	//하단
//	Gr->SetColor(RGB565CONVERT(133, 133, 133));
//	Gr->HLine(sx+1, sy+height, width-4);
//	Gr->SetColor(RGB565CONVERT(165, 165, 165));
//	Gr->HLine(sx+3, sy+height-1, width-8);	
	if(outline==1)
	{
		Gr->SetColor(clWhite);
		Gr->Rectangle(sx, sy, width, height);			
	}
}

void GroupPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int swidth, int sheight, int outline)
{
	int	width = swidth-1;
	int	height = sheight-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->FillRect(sx, sy, width, height);	

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
}
int	GroupPanelPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	Gr->FillRect(0,0,0,0);	
	//Gr->SetColor(win->BackColor);
	//Gr->FillRect(sx, sy, win->width, win->height);
	//타이틀 패널
	if(flash.systemColorNo!=6) {
		GroupPanelDraw_New(win, Gr, sx, sy, win->width, TITLE_HEIGHT, 1);
		GroupPanelDraw_New(win, Gr, sx, sy+TITLE_HEIGHT, win->width, win->height-TITLE_HEIGHT, 1);

		SetFontAliasing("ng_eh_24.sif");
		int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
		sx = (win->width -text_width) / 2;
		sy = (TITLE_HEIGHT - 24) / 2;
		
		Gr->TextOutAliasing(sx, sy, win->Text, flash.systemSetupPanelFontColor, 0, "ng_eh_24.sif");
	}
	else {
		DiamondPanelDraw(win, Gr, sx, sy, win->width, TITLE_HEIGHT, 0);
		GroupPanelDraw(win, Gr, sx, sy+TITLE_HEIGHT, win->width, win->height-TITLE_HEIGHT, 0);

		SetFontAliasing("ng_eh_24.sif");
		int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
		sx = (win->width -text_width) / 2;
		sy = (TITLE_HEIGHT - 24) / 2;
		Gr->TextOutAliasing(sx, sy, win->Text, clBlack, 0, "ng_eh_24.sif");
	}
	return(1);
}

U32 GroupPanel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text)
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"SLabel");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	//wid_struct.BackColor = backcolor;
	//wid_struct.FontColor = fontcolor;
	//wid_struct.Style=style;
	handle=makeWindow(
				LCD,						// Display Section Position
				"GroupPanel",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				GroupPanelPaintEvent,			// PaintEvent
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
