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
//문자와 문자사이의 공간을 지정한다.
#define     TEXT_INTERVAL   3

//static int parent_screen=0;

const uint8_t CFontSpace[95] = { 
    8,     // Space
    3,      // !
    6,     // "
    13,     // #
    
    9,     // $
    19,     // %
    14,     // &
    3,     // '
    5,     // (
    6,     // )
    9,     // *
    13,     // +
    3,     // , (COMMA)
    7,     // - (minus)
    3,     // . (DOT)
    9,     // /
    11,     // 0
    5,     // 1
    10,     // 2
    10,     // 3
    13,     // 4
    10,     // 5
    11,     // 6
    11,     // 7
    11,     // 8
    11,     // 9
    3,     // :
    4,     // ;
    10,     // <
    13,     // =
    10,     // >
    9,     // ?
    17,     // @
    15,     // A
    11,     // B
    13,     // C
    13,     // D
    9,     // E
    9,     // F
    14,     // G
    13,     // H
    3,     // I
    6,     // J
    11,     // K
    9,     // L
    17,     // M
    13,     // N
    15,     // O
    10,     // P
    15,     // Q
    12,     // R
    10,     // S
    12,     // T
    13,     // U
    15,     // V
    22,     // W
    15,     // X
    14,     // Y
    11,     // Z
    5,     // [
    19,     // 
    5,     // ]
    11,     // ^
    13,     // _
    4,     // '
    10,     // a
    11,     // b
    9,     // c
    11,     // d
    11,     // e
    8,     // f
    11,     // g
    11,     // h
    3,     // i
    4,     // j
    10,     // k
    3,     // l
    17,     // m
    11,     // n
    13,     // o
    11,     // p
    11,     // q
    7,     // r
    9,     // s
    8,     // t
    11,     // u
    11,     // v
    19,     // w
    11,     // x
    10,     // y
    9,     // z
    6,     // {
    3,     // |
    6,     // }
    11     // ~  
};

int CButtonFontLabelClearFontOut(Windows *win, Graphics *Gr, char *string, int sx, int sy)
{
    int     FontX_pos;
    int     font_x_size = win->c_font_x_size;
    int     font_y_size = win->c_font_y_size;
    int     next_pos;
//    int     offset;
    int     out_x_size = font_x_size;
    //gROP2 = 14;

    next_pos = 0;
    while(*string)
    {
        out_x_size = win->c_font_width_array[*string - 0x20] + TEXT_INTERVAL; //문자의 넓이를 구한다.

        //Dprintf("KeyBoardLabelClearFontOut:: %s\  SX=%d SY=%d out_x_size=%d next_pos=%d\n", string, sx, sy,  out_x_size, next_pos);
        if(win->c_font_col_no==3)
        {
            if(*string >= win->c_font_col1_start_code && *string < win->c_font_col2_start_code)
            {
                FontX_pos = font_x_size * (*string - 0x20);
                Gr->BitBlt(win->c_font_screen, FontX_pos, win->c_font_start_y1, sx+next_pos, sy, out_x_size, font_y_size);
            }
            else if(*string >= win->c_font_col2_start_code && *string < win->c_font_col3_start_code)
            {
                FontX_pos = font_x_size * (*string - 'A');
                Gr->BitBlt(win->c_font_screen, FontX_pos, win->c_font_start_y2, sx+next_pos, sy, out_x_size, font_y_size);
            }
            else
            {
                FontX_pos = font_x_size * (*string - 'a');
                Gr->BitBlt(win->c_font_screen, FontX_pos, win->c_font_start_y3, sx+next_pos, sy, out_x_size, font_y_size);
            }
        }
        next_pos += out_x_size;
        string++;
    }    
    //gROP2 = 12;
	return(1);
}

//이폰트를 사용할시 문자의 길이를 구한다.
int CButtonFontTextWid_Cal(char *str)
{
    int     cnt=0;
    int     wid = 0;
    while(*str)
    {
        cnt = CFontSpace[*str-0x20]+TEXT_INTERVAL;
        wid += cnt;
        str++;
    }
    return(wid);
}

int CButtonMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
    
	page = win->BackColor;
	Gr->BitBlt(page, win->sx, win->sy, sx, sy, win->width, win->height);    
   

	//Gr->SetColor(clRed);
	//Gr->Rectangle(0, 0, win->width, win->height);
	//Gr->Rectangle(0+1, 0+1, win->width-2, win->height-2);
	if(win->Text != NULL)
	{
		//text_width = strlen(win->Text) * 24; //20=Font X size
        text_width = CButtonFontTextWid_Cal(win->Text);

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
		sy = (win->height - win->c_font_y_size) / 2; //33 은 폰트 Y Size
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;

        CButtonFontLabelClearFontOut(win, Gr, win->Text, sx, sy/2);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int CButtonPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
    
	page = win->FontColor;
	Gr->BitBlt(page, win->sx, win->sy, sx, sy, win->width, win->height);      

	if(win->Text != NULL)
	{
		//text_width = strlen(win->Text) * 24; //20=Font X size
        text_width = CButtonFontTextWid_Cal(win->Text);

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

		sy = (win->height - win->c_font_y_size) / 2; //24 은 폰트 Y Size
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;

        CButtonFontLabelClearFontOut(win, Gr, win->Text, sx, sy/2);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


//폰트를 디자인할때는 0x20 ~ 부터 순서대로 디자인되어야 한다.
U32 CButton(HANDLE ContainerHandle, 
                int 	sx, int sy,         //컨테이너에서의 시작위치
                int width, int height,    //위젯의크기  
                char	*text,		        //표시할문자열
                int		align,		        //정렬모드 RIGHT_ALIGN, , 
                uint16_t    font_x_size, //폰트의 X 사이즈
                uint16_t    font_y_size, //폰트의 Y 사이즈
                uint16_t    font_screen, //폰트가 로딩된 페이지
                uint8_t     *font_width_array, //폰트의 폭을조정하기위한 배열
                uint8_t     font_col_no,  //폰트가 몇행으로 구성되어있는가?
                uint8_t     font_col1_start_code, //1번째행의 시작코드 ex) ' '
                uint16_t    font_start_y1,
                uint8_t     font_col2_start_code, //2번째행의 시작코드 ex) '@'
                uint16_t    font_start_y2,
                uint8_t     font_col3_start_code, //3번째행의 시작코드    
                uint16_t    font_start_y3,
                int		normal_screen, 
				int		touch_screen,
				int		id,
				int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *)
			)
{
	HANDLE		handle;
	U32			w_id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
    wid_struct.c_font_x_size=font_x_size;
    wid_struct.c_font_y_size=font_y_size;
    wid_struct.c_font_screen=font_screen;
    wid_struct.c_font_width_array=font_width_array;
    wid_struct.c_font_col_no=font_col_no;
    wid_struct.c_font_col1_start_code=font_col1_start_code;
    wid_struct.c_font_col2_start_code=font_col2_start_code;
    wid_struct.c_font_col3_start_code=font_col3_start_code;
    wid_struct.c_font_start_y1 = font_start_y1;
    wid_struct.c_font_start_y2 = font_start_y2;
    wid_struct.c_font_start_y3 = font_start_y3;
    //wid_struct.rop_mode=rop_mode;
	wid_struct.BackColor = touch_screen;
	wid_struct.FontColor = normal_screen;
	wid_struct.ID = id;	
    
	handle=makeWindow(
				LCD,						// Display Section Position
				"CButton",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				CButtonPaintEvent,			// PaintEvent,LabelPaintEvent
				CButtonMouseOnEvent,		// MouseOnEvent
				CButtonPaintEvent,		// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				ClickEvent,					// ClickEvent
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
	w_id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	return(w_id);
}				
#ifdef __cplusplus
}
#endif
