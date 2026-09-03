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
//static int parent_screen=0;
//static int rop2=0;

int FontNumLabelClearFontOut(Windows *win, Graphics *Gr, char *string, int sx, int sy)
{
    int     FontX_pos;
    int     font_x_size = win->c_font_x_size;
    int     font_y_size = win->c_font_y_size;
    int     next_pos;
//    int     offset;
    int     out_x_size = font_x_size;
//	int		t_rop2;
	
	/*
	if(rop2==12 || rop2==3) 
	{
		t_rop2 = gROP2;
		gROP2=rop2;
	}
	*/
    next_pos = 0;
    while(*string)
    {
        if(*string == ' ') *string = 0x3B;
        else if(*string == '-') *string = 0x3C;
        else if(*string == '.') 
        {
            *string = 0x3a;
            out_x_size = win->c_font_col_no; //Dot 의 크기지정
        }
        else out_x_size=font_x_size;        

        FontX_pos = font_x_size * (*string & 0x0F);
        
        Gr->BitBlt(win->c_font_screen, FontX_pos, win->c_font_start_y1, sx+next_pos, sy, out_x_size, font_y_size); 
        
        string++;        
        next_pos += out_x_size;
    } 
	//gROP2 = t_rop2;
	return(1);
}

int Search_Dot(char *str)
{
    while(*str)
    {
        if(*str++ == '.') return (1);
    }
    return(0);
}

int FontNumLabelMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->SetColor(clCyan);
	Gr->Rectangle(0, 0, win->width, win->height);
	Gr->Rectangle(0+1, 0+1, win->width-2, win->height-2);

	return(1);
}

int FontNumLabelMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	win->PaintEvent(parent, handle, 0, 12, 0);

	return(1);
}

int FontNumLabelPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			text_width, page=0;
	
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

	if(win->Text != NULL)
	{
		//text_width = strlen(win->Text) * 24; //20=Font X size
        text_width = strlen(win->Text) * win->c_font_x_size;
        if(Search_Dot(win->Text)) 
        {
            text_width -= (win->c_font_x_size - win->c_font_col_no); //Dot 의 폭만큼 뺀다.
            //text_width -= 20;
        }
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
			sx = 5;
		}
		sy = (win->height - win->c_font_x_size) / 2; 
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		
        FontNumLabelClearFontOut(win, Gr, win->Text, sx, sy/2);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


//폰트를 디자인할때는 0x20 ~ 부터 순서대로 디자인되어야 한다. 각폰트의 간격은 사이즈대로 일정해야한다.
U32 FontNumLabel(HANDLE ContainerHandle, 
                int 	sx, int sy,         //컨테이너에서의 시작위치
                int width, int height,    //위젯의크기  
                char	*text,		        //표시할문자열
                int		align,		        //정렬모드 RIGHT_ALIGN, , 
                uint16_t    font_x_size, //폰트의 X 사이즈
                uint16_t    font_y_size, //폰트의 Y 사이즈
                uint16_t    font_screen, //폰트가 로딩된 페이지
                uint16_t    font_start_y, //스크린에서의 폰트시작위치
                uint16_t    font_dot_width, //스크린에서의 폰트시작위치
                uint8_t     rop_mode,       //AND, OR
				int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *)				
			)
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "FontNumLabel");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
    wid_struct.c_font_x_size=font_x_size;
    wid_struct.c_font_y_size=font_y_size;
    wid_struct.c_font_screen=font_screen;
    wid_struct.rop_mode=rop_mode;
    wid_struct.c_font_start_y1 = font_start_y;
    wid_struct.c_font_col_no=font_dot_width;
    
	handle=makeWindow(
				LCD,						// Display Section Position
				"FontNumLabel",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				FontNumLabelPaintEvent,			// PaintEvent,LabelPaintEvent
				FontNumLabelMouseOnEvent,		// MouseOnEvent
				FontNumLabelMouseOffEvent,		// MouseOffEvent
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
