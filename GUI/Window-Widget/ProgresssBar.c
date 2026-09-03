/******************************************************************************
*    	File name	: ProgressBar.c 
*		Description : TEECO GUI Window Control Widget [ProgressBar]
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

int	ProgressBarPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
//	int			text_width;
	int			percent, progress_value;
//	char		buf[10];
	
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
	
	//전체바탕색을 칠한다.
	Gr->SetColor(win->BackColor);
	Gr->FillRect(0, 0, win->width, win->height);

	//스타일을 그린다.
	Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
	Gr->Line(sx, sy, sx, sy+win->height-1);			//좌측수직라인그리기		
	Gr->Line(sx, sy, sx+win->width-1, sy);			//상단수직라인		
	Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));	//우측수직라인
	Gr->Line(sx+win->width-1, sy+1, sx+win->width-1, sy+1+win->height);		
	Gr->Line(sx+1, sy+win->height-1, sx+1+win->width, sy+win->height-1);//아래백색라인
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
	Gr->Line(sx+1, sy+1, sx+1, sy+win->height-2);	//좌측회색라인		
	Gr->Line(sx+1, sy+1, sx+win->width-2, sy+1);	//위회색라인	
	
	percent = win->width / 100;
	progress_value = win->Position * percent;
	
	Gr->SetColor(win->BarColor);
	Gr->FillRect(sx+3, sy+3, progress_value, win->height-4);
	
	/*
	Gr->SetFontColor(BLACK);
	//Gr->LoadFont("SystemFontGL8.fnt");
	Gr->SetFont(win->FontNo);
	Gr->SetFontGap(4);
	Gr->SetFontSpaceGap(7);

	sprintf(buf,"%2d%c",win->Position, '%');
	text_width = GetTextWidth(buf);

	sx = (win->width - text_width) / 2;
	sy = (win->height - GetTextHeight(g_FontNo)) / 2;
			
	if(sx < 0) sx = 0;
	if(sy < 0) sy = 0;
	Gr->TextOut(sx, sy, buf);
	*/
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


U32 ProgressBar(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		font_no,
			int		backcolor,
			int		barcolor,
			int 	value
)

{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"ProgressBar");
	
//Dprintf("Button:: parent sx=%d sy=%d\n", parent_win->sx, parent_win->sy);
//Dprintf("Button:: parent csx=%d csy=%d\n", parent_win->client_sx, parent_win->client_sy);
//Dprintf("Button:: parent abs_sx=%d abs_sy=%d\n", parent_win->abs_sx, parent_win->abs_sy);
//Dprintf("Button:: parent cabs_sx=%d cabs_sy=%d\n", parent_win->client_abs_sx, parent_win->client_abs_sy);
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.BarColor = barcolor;
	wid_struct.FontNo = font_no;
	if(value >=0 && value <=100)
		wid_struct.Position = value;
	else 
		wid_struct.Position = 0;
	
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"Progress",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				ProgressBarPaintEvent,		// PaintEvent
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
				NULL,						// Windows Text
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
