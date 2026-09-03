/******************************************************************************
*    	File name	: ScrollBar.c 
*		Description : TEECO GUI Window Control Widget [ScrollBar]
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

int	ScrollBarPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	//int			text_width;
	int			bar_wid, bar_height;
	int			i,r=9,g=52,b=128;
	
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
	
	Gr->SetColor(win->BackColor);
	Gr->FillRect(0, 0, win->width, win->height);
	Gr->SetColor(BLACK);
	Gr->Rectangle(0, 0, win->width-1, win->height-1);

	r=170,g=170,b=170;
	if(win->WidgetStyle==HORIZONTAL)
	{
		bar_wid = 24-2;
		bar_height = win->height-2;
		sx=1, sy=1;
		sx += win->Position;
		//길이가 길어지면 그라데이션이 골고루 적용되게 한다.
		for( i=win->height-2; i> 0; i-- )
		{
			r+=3;g+=3;b+=3;
			Gr->SetColor( RGB565CONVERT(r,g,b) );
			Gr->Line(sx,i,sx+bar_wid,i);
		}		
		bar_wid = 24-1;

		Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));
		Gr->Line(sx, sy, sx, sy+bar_height-1);		//좌측수직라인그리기
		Gr->Line(sx, sy, sx+bar_wid-1, sy); 	
		Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
		Gr->Line(sx+bar_wid-1, sy, sx+bar_wid-1, sy+bar_height);	//우측수직라인
		Gr->Line(sx, sy+bar_height-1, sx+bar_wid, sy+bar_height-1);//아래흑색라인
		Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
		Gr->Line(sx+bar_wid-2, sy+1, sx+bar_wid-2, sy+bar_height-2);//우측회색라인	
		Gr->Line(sx+1, sy+bar_height-2, sx+bar_wid-2, sy+bar_height-2);//아래회색라인	
	}
	else
	{
		bar_wid = win->width-2;
		bar_height = 24-2;
		sx=1, sy=1;
		sy += win->Position;
		//길이가 길어지면 그라데이션이 골고루 적용되게 한다.
		for( i=win->width-2; i> 0; i-- )
		{
			r+=3;g+=3;b+=3;
			Gr->SetColor( RGB565CONVERT(r,g,b) );
			Gr->Line(i, sy, i,sy+bar_height);
		}
		bar_height = 24-1;		

		Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));
		Gr->Line(sx, sy, sx, sy+bar_height-1);		//좌측수직라인그리기
		Gr->Line(sx, sy, sx+bar_wid-1, sy); 	
		Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
		Gr->Line(sx+bar_wid-1, sy, sx+bar_wid-1, sy+bar_height);	//우측수직라인
		Gr->Line(sx, sy+bar_height-1, sx+bar_wid, sy+bar_height-1);//아래흑색라인
		Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
		Gr->Line(sx+bar_wid-2, sy+1, sx+bar_wid-2, sy+bar_height-2);//우측회색라인	
		Gr->Line(sx+1, sy+bar_height-2, sx+bar_wid-2, sy+bar_height-2);//아래회색라인	

	}	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	ScrollBarMouseMoveEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int 		tx = LOWORD(param);
	int 		ty = HIWORD(param);	
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	//여기에 좌표를 이탈할경우 좌표를 고정해야한다.
	if(win->WidgetStyle==HORIZONTAL) win->Position = tx;
	else  win->Position = ty;
	ScrollBarPaintEvent(parent, handle, 0, 0, 0);
	return(1);
}


U32 ScrollBar(HANDLE ContainerHandle, int style, int sx, int sy, int width, int height, 
			int 	backcolor, 
			int 	position,
			int		(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )

{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "ScrollBar");

	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	//wid_struct.FontColor = fontcolor;
	wid_struct.Style = style;
	wid_struct.Position = position;
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"ScrollBar",				// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,					// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				ScrollBarPaintEvent,		// PaintEvent
				NULL,						// MouseOnEvent
				NULL,						// MouseOffEvent
				ScrollBarMouseMoveEvent,	// MouseMoveEvent
				NULL,						// GestureEvent
				NULL,						// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				ChangeEvent,				// ChangeEvent
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
	return(id);
}				
#ifdef __cplusplus
}
#endif
