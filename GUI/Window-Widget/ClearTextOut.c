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

extern const uint8_t FontSpace1[];
//static int parent_screen=0;

#define	ENG_FONT_SCREEN	USER_SCREEN2

#define	ENG_Y1_POS		89
#define	ENG_Y2_POS		119
#define	ENG_Y3_POS		149

int ClearTextLabelFontOut(Graphics *Gr, char *string, int sx, int sy)
{
    int     FontX_pos;
    int     font_x_size = 21;
    int     font_y_size = 21;

//    int     row_cnt=0;
    int     next_pos=0;
//    int     offset=0;
    int     out_x_size;
    uint16_t    code, high, low;
    int     page, row, col;
//Dprintf("Hagul Code Test-------------\n");
    while(*string)
    {
		//영문일경우
		//영문의 BMP화면은 USER_SCREEN2에 있는걸로 가정한다.
		//영문의 폰트사이즈는 21X21로 한다. 일정해야한다.
		if(*string >=0x20 && *string < 0x7f)
		{
			out_x_size = FontSpace1[*string - 0x20] + TEXT_INTERVAL; //문자의 넓이를 구한다.
			if(*string >= ' ' && *string < 'A')
			{
				FontX_pos = font_x_size * (*string - 0x20);
				Gr->BitBlt(ENG_FONT_SCREEN, FontX_pos, ENG_Y1_POS, sx+next_pos, sy, out_x_size, font_y_size);
			}
			else if(*string >= 'A' && *string < 'a')
			{
				FontX_pos = font_x_size * (*string - 'A');
				Gr->BitBlt(ENG_FONT_SCREEN, FontX_pos, ENG_Y2_POS, sx+next_pos, sy, out_x_size, font_y_size);
			}
			else
			{
				FontX_pos = font_x_size * (*string - 'a');
				Gr->BitBlt(ENG_FONT_SCREEN, FontX_pos, ENG_Y3_POS, sx+next_pos, sy, out_x_size, font_y_size);
			}
			next_pos += out_x_size;
			string++;						
		}
		else	//한글일경우
		{
			high = *string++;
			high = high - 0xb0;
			high = high * 96;
			
			low = *string++;
			low = low - 0xa0;
			
			code = high + low;	

			//Dprintf("\tCode = %04x %d\n", code, code);
			col = code / 38;
			row = code % 38;
			//Dprintf("\tcol = %d row=%d\n", col, row);
			page = col / 22;
			//Dprintf("\tpage=%d  x=%d y=%d \n", page, row*font_x_size, col*font_y_size);
			
			out_x_size=font_x_size;        
			if(page==0)      
			{
				Gr->BitBlt(USER_SCREEN3, row*font_x_size, col* font_y_size, sx+next_pos, sy, out_x_size, font_y_size);
			}
			else if(page==1)      
			{
				col -= 22;
				Gr->BitBlt(USER_SCREEN4, row*font_x_size, col* font_y_size, sx+next_pos, sy, out_x_size, font_y_size);
			}
			else if(page==2)      
			{
				col -= 44;
				Gr->BitBlt(USER_SCREEN5, row*font_x_size, col* font_y_size, sx+next_pos, sy, out_x_size, font_y_size);
			}
			next_pos += out_x_size;        
		}
    }
	return(1);
}

int	ClearTextPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			text_width, page=0;;
	
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

	//if(win->Text != NULL)
    if(strlen(win->Text) != 0)
	{
        text_width = strlen(win->Text) * 20; //20=Font X size

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
		sy = (win->height - 20) / 2; //20 은 폰트 Y Size
			
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;

        ClearTextLabelFontOut(Gr, win->Text, sx, sy);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


U32 ClearTextLabel(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			char	*text,		//init Character
			int		align)		//Left, Right, Center
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "");

	sx = sx + (parent_win->client_sx - parent_win->sx);
	sy = sy + (parent_win->client_sy - parent_win->sy);
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"ClearTextLabel",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				ClearTextPaintEvent,			// PaintEvent,LabelPaintEvent
				NULL,		// MouseOnEvent
				NULL,		// MouseOffEvent
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
