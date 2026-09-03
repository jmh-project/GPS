/******************************************************************************
*    	File name	: Button.c 
*		Description : TEECO GUI Window Control Widget [Button]
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
							Description
******************************************************************************/
//버튼의 종류는 3가지로 작성이 되었다.
//텍스트버튼, 이미지버튼, 아이콘버튼 
//아이콘버튼지정시 아이콘의 바탕색은 백색으로한다.

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define		TEXT_RIGHT_OFFSET		5
#define		BUTTON_FONT_HEIGHT	16
/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
int	ButtonFocusEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			i,r=9,g=52,b=128;
	int			text_width=0;
	int			style;
	BitmapSize  image_size;
	int			h1, h2;
//	int			dir=0;
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	//토글기능에서는 OFF 이벤트가 필요없다.
	if(win->Toggle==TOGGLE_BUTTON) 
	{
		win->MouseOffEvent = NULL;
		win->ToggleStatus=OFF;		
	}
		
	style = LOWORD(win->Param);

	Gr->FillRect(0, 0, 0, 0); //Dummy
	
	if(style!=BMP_BUTTON)
	{
		r=110,g=110,b=110;
		if(win->height <= 48)
		{
			for( i=win->height; i> 0; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0, i, win->width,i);
			}	
		}
		else
		{
			h1 = win->height - 48;
			h2 = h1 / 2;
			//하단라인을 그린다.
			for( i=win->height; i> win->height - h2; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}	
			
			for( i=win->height-h2; i> 48; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}	
			for( i=win->height-h2+48; i> 0; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}
		}
	}
	if(style==TEXT_BUTTON || style==ICON_BUTTON) //BMP가 지정이 안되어 있으면
	{
		Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));
		Gr->Line(sx, sy, sx, sy+win->height-1);		//좌측수직라인그리기
		Gr->Line(sx, sy, sx+win->width-1, sy); 	
		Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
		Gr->Line(sx+win->width-1, sy, sx+win->width-1, sy+win->height);	//우측수직라인
		Gr->Line(sx, sy+win->height-1, sx+win->width, sy+win->height-1);//아래흑색라인
		Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
		Gr->Line(sx+win->width-2, sy+1, sx+win->width-2, sy+win->height-2);//우측회색라인	
		Gr->Line(sx+1, sy+win->height-2, sx+win->width-2, sy+win->height-2);//아래회색라인
	}	
	else if(style==BMP_BUTTON && win->NormalName != NULL)	
	{
		Gr->BmpDisplay1(0, 0, win->NormalName);
		if(strlen(win->IconName))
		{
			Gr->IconDisplay(0, 0, win->IconName, 0xffff);
		}
	}	
	if(strlen(win->Text))		
	{
#ifdef LOW_FONT_USE
		text_width = LowTextLen(win->Text, fArial12);
#endif
		if(style==ICON_BUTTON)	
		{
			image_size.width=0;
			image_size.height=0;
			GetImage(win->NormalName, &image_size);
			
			sx = (win->width - (text_width+image_size.width)) / 2;
			sy = (win->height - image_size.height) / 2;
			Gr->IconDisplay(sx, sy, win->NormalName, 0xffff);		
			sx += image_size.width;
			sy = (win->height - 16) / 2;
		}
		else
		{
			sx = (win->width - text_width) / 2;
			sy = (win->height - 16) / 2;
		}

		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		
		Gr->LowTextOut(sx, sy, win->Text, win->FontColor, fArial12);
	}
    //포커스에 대응하는 동작을 그린다.
	if(command==1) //Focus
    {
        Gr->SetColor(YELLOW);
        Gr->Rectangle(0, 0, win->width, win->height);
        Gr->Rectangle(0+1, 0+1, win->width-2, win->height-2);
    }
    else //포커스의 해제
    {
        
    }
	Gr->Draw(parent, handle, command, param, Data);	
	
	return(1);
}


int	ButtonPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			i,r=9,g=52,b=128;
	int			text_width=0;
	int			style;
	BitmapSize  image_size;
	int			h1, h2;
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	//토글기능에서는 OFF 이벤트가 필요없다.
	if(win->Toggle==TOGGLE_BUTTON) 
	{
		win->MouseOffEvent = NULL;
		win->ToggleStatus=OFF;		
	}
		
	style = LOWORD(win->Param);
	Gr->FillRect(0, 0, 0, 0);
	
	if(style!=BMP_BUTTON)
	{
		r=110,g=110,b=110;
		if(win->height <= 48)
		{
			for( i=win->height; i> 0; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0, i, win->width,i);
			}	
		}
		else
		{
			h1 = win->height - 48;
			h2 = h1 / 2;
			//하단라인을 그린다.
			for( i=win->height; i> win->height - h2; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}	
			
			for( i=win->height-h2; i> 48; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}	
			for( i=win->height-h2+48; i> 0; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}
		}
	}
	if(style==TEXT_BUTTON || style==ICON_BUTTON) //BMP가 지정이 안되어 있으면
	{
		Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));
		Gr->Line(sx, sy, sx, sy+win->height-1);		//좌측수직라인그리기
		Gr->Line(sx, sy, sx+win->width-1, sy); 	
		Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
		Gr->Line(sx+win->width-1, sy, sx+win->width-1, sy+win->height);	//우측수직라인
		Gr->Line(sx, sy+win->height-1, sx+win->width, sy+win->height-1);//아래흑색라인
		Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
		Gr->Line(sx+win->width-2, sy+1, sx+win->width-2, sy+win->height-2);//우측회색라인	
		Gr->Line(sx+1, sy+win->height-2, sx+win->width-2, sy+win->height-2);//아래회색라인
	}	
	else if(style==BMP_BUTTON && win->NormalName != NULL)	
	{
		Gr->BmpDisplay1(0, 0, win->NormalName);
	}
	
	if(strlen(win->Text))		
	{
#ifdef LOW_FONT_USE
		text_width = LowTextLen(win->Text, fArial12);
#endif		
		if(style==ICON_BUTTON)	
		{
			image_size.width=0;
			image_size.height=0;
			GetImage(win->NormalName, &image_size);
			
			sx = (win->width - (text_width+image_size.width)) / 2;
			sy = (win->height - image_size.height) / 2;
			Gr->IconDisplay(sx, sy, win->NormalName, 0xffff);		
			sx += image_size.width;
			sy = (win->height - BUTTON_FONT_HEIGHT) / 2;
		}
		else
		{
			sx = (win->width - text_width) / 2;
			sy = (win->height - BUTTON_FONT_HEIGHT) / 2;
		}

		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->LowTextOut(sx, sy, win->Text, win->FontColor, fArial12);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	
	return(1);
}

int	ButtonMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			i, r=9, g=52, b=128;
	int			text_width=0;
	int			style=0;
	BitmapSize  image_size = {0, 0};
	int			h1, h2;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	style = LOWORD(win->Param);

	Gr->FillRect(0, 0, 0, 0);
	if(win->Toggle==PUSH_BUTTON || (win->Toggle==TOGGLE_BUTTON && win->ToggleStatus==OFF))
	{
		win->ToggleStatus = ON;
		if(style!=BMP_BUTTON)
		{
			r=110,g=110,b=110;
			if(win->height <=48)
			{
				for( i=0; i< win->height; i++ )
				{
					r+=3;g+=3;b+=3;
					Gr->SetColor( RGB565CONVERT(r,g,b) );
					Gr->Line(0,i,win->width,i);
				}	
			}
			else
			{
				r=110,g=110,b=110;
				h1 = win->height - 48;
				h2 = h1 / 2;
				for( i=0; i< h2; i++ )
				{
					r+=1;g+=1;b+=1;
					Gr->SetColor( RGB565CONVERT(r,g,b) );
					Gr->Line(0,i,win->width,i);
				}	
				//r=110,g=110,b=110;
				for( i=h2; i< h2+48; i++ )
				{
					r+=1;g+=1;b+=1;
					Gr->SetColor( RGB565CONVERT(r,g,b) );
					Gr->Line(0,i,win->width,i);
				}	
				//r=100,g=100,b=100;
				for( i=h2+48; i< win->height; i++ )
				{
					r+=1;g+=1;b+=1;
					Gr->SetColor( RGB565CONVERT(r,g,b) );
					Gr->Line(0,i,win->width,i);
				}	
			}
		}
		if(style==TEXT_BUTTON || style==ICON_BUTTON) //BMP가 지정이 안되어 있으면
		{		
			Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
			Gr->Line(sx, sy, sx, sy+win->height-1);			//좌측수직라인그리기		
			Gr->Line(sx, sy, sx+win->width-1, sy);			//상단수직라인		
			Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));	//우측수직라인
			Gr->Line(sx+win->width-1, sy+1, sx+win->width-1, sy+1+win->height);		
			Gr->Line(sx+1, sy+win->height-1, sx+1+win->width, sy+win->height-1);//아래백색라인
			Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
			Gr->Line(sx+1, sy+1, sx+1, sy+win->height-2);	//좌측회색라인		
			Gr->Line(sx+1, sy+1, sx+win->width-2, sy+1);	//위회색라인
		}
		if(style==BMP_BUTTON )	
		{
			Gr->BmpDisplay1(0, 0, win->TouchName);
			if(strlen(win->IconName))
			{				
				Gr->IconDisplay(0, 0, win->IconName, 0xffff);	
			}
		}
	}
	else
	{
		win->ToggleStatus = OFF;
		if(style!=BMP_BUTTON)
		{
			r=110,g=110,b=110;
			if(win->height <= 48)
			{
				for( i=win->height; i> 0; i-- )
				{
					r+=3;g+=3;b+=3;
					Gr->SetColor( RGB565CONVERT(r,g,b) );
					Gr->Line(0,i,win->width,i);
				}	
			}
			else
			{
				h1 = win->height - 48;
				h2 = h1 / 2;
				//하단라인을 그린다.
				for( i=win->height; i> win->height - h2; i-- )
				{
					r+=3;g+=3;b+=3;
					Gr->SetColor( RGB565CONVERT(r,g,b) );
					Gr->Line(0,i,win->width,i);
				}	
				
				for( i=win->height-h2; i> 48; i-- )
				{
					r+=3;g+=3;b+=3;
					Gr->SetColor( RGB565CONVERT(r,g,b) );
					Gr->Line(0,i,win->width,i);
				}	
				for( i=win->height-h2+48; i> 0; i-- )
				{
					r+=3;g+=3;b+=3;
					Gr->SetColor( RGB565CONVERT(r,g,b) );
					Gr->Line(0,i,win->width,i);
				}
			}
		}
		if(style==TEXT_BUTTON || style==ICON_BUTTON) //BMP가 지정이 안되어 있으면
		{
			Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));
			Gr->Line(sx, sy, sx, sy+win->height-1);		//좌측수직라인그리기
			Gr->Line(sx, sy, sx+win->width-1, sy); 	
			Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
			Gr->Line(sx+win->width-1, sy, sx+win->width-1, sy+win->height);	//우측수직라인
			Gr->Line(sx, sy+win->height-1, sx+win->width, sy+win->height-1);//아래흑색라인
			Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
			Gr->Line(sx+win->width-2, sy+1, sx+win->width-2, sy+win->height-2);//우측회색라인	
			Gr->Line(sx+1, sy+win->height-2, sx+win->width-2, sy+win->height-2);//아래회색라인
		}	
		else if(style==BMP_BUTTON)	
		{
			Gr->BmpDisplay1(0, 0, win->NormalName);
			if(strlen(win->IconName))
				Gr->IconDisplay(0, 0, win->IconName, 0xffff);
			
		}
		
	}
	if(strlen(win->Text))	
	{
#ifdef LOW_FONT_USE
		text_width = LowTextLen(win->Text, fArial12);
#endif		
		if(style==ICON_BUTTON)	
		{
			image_size.width=0;
			image_size.height=0;
			GetImage(win->NormalName, &image_size);
			
			sx = (win->width - (text_width+image_size.width)) / 2;
			sy = (win->height - image_size.height) / 2;
			Gr->IconDisplay(sx, sy, win->NormalName, 0xffff);		
			sx += image_size.width;
			sy = (win->height - BUTTON_FONT_HEIGHT) / 2;
		}
		else
		{
			sx = (win->width - text_width) / 2;
			sy = (win->height - BUTTON_FONT_HEIGHT) / 2;
		}

		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
        
		Gr->LowTextOut(sx, sy, win->Text, win->FontColor, fArial12);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	ButtonMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			i,r=9,g=52,b=128;
	int			text_width=0;
	int			style;
	BitmapSize  image_size= {0, 0};
	int			h1, h2;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	style = LOWORD(win->Param);

	
	Gr->FillRect(0, 0, 0, 0);
	
	if(style!=BMP_BUTTON)
	{
		r=110,g=110,b=110;
		if(win->height <= 48)
		{
			for( i=win->height; i> 0; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}	
		}
		else
		{
			h1 = win->height - 48;
			h2 = h1 / 2;
			//하단라인을 그린다.
			for( i=win->height; i> win->height - h2; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}	
			
			for( i=win->height-h2; i> 48; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}	
			for( i=win->height-h2+48; i> 0; i-- )
			{
				r+=3;g+=3;b+=3;
				Gr->SetColor( RGB565CONVERT(r,g,b) );
				Gr->Line(0,i,win->width,i);
			}
		}
	}
	if(style==TEXT_BUTTON || style==ICON_BUTTON) //BMP가 지정이 안되어 있으면
	{
		Gr->SetColor(RGB565CONVERT(0xff, 0xff, 0xff));
		Gr->Line(sx, sy, sx, sy+win->height-1);		//좌측수직라인그리기
		Gr->Line(sx, sy, sx+win->width-1, sy); 	
		Gr->SetColor(RGB565CONVERT(0x00, 0x00, 0x00));
		Gr->Line(sx+win->width-1, sy, sx+win->width-1, sy+win->height);	//우측수직라인
		Gr->Line(sx, sy+win->height-1, sx+win->width, sy+win->height-1);//아래흑색라인
		Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
		Gr->Line(sx+win->width-2, sy+1, sx+win->width-2, sy+win->height-2);//우측회색라인	
		Gr->Line(sx+1, sy+win->height-2, sx+win->width-2, sy+win->height-2);//아래회색라인
	}	
	else if(style==BMP_BUTTON)	
	{
		Gr->BmpDisplay1(0, 0, win->NormalName);
		if(strlen(win->IconName))		
			Gr->IconDisplay(0, 0, win->IconName, 0xffff);	
	}

	if(strlen(win->Text))
	{
#ifdef LOW_FONT_USE
		text_width = LowTextLen(win->Text, fArial12);
#endif		

		if(style==ICON_BUTTON)	
		{
			image_size.width=0;
			image_size.height=0;
			GetImage(win->NormalName, &image_size);
			
			sx = (win->width - (text_width+image_size.width)) / 2;
			sy = (win->height - image_size.height) / 2;
			Gr->IconDisplay(sx, sy, win->NormalName, 0xffff);		
			sx += image_size.width;
			sy = (win->height - BUTTON_FONT_HEIGHT) / 2;
		}
		else
		{
			sx = (win->width - text_width) / 2;
			sy = (win->height - BUTTON_FONT_HEIGHT) / 2;
		}

		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
		Gr->LowTextOut(sx, sy, win->Text, win->FontColor, fArial12);	
	}
	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 Button(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			int 	style,								// 버튼의 종류 BMP_BUTTON, ICON_BUTTON, TEXT_BUTTON
			char 	*normal_image, 						// 이미지버튼시 터치안된 이미지이름 or Icon name
			char 	*touch_image,						// 이미지버튼시 터치된 이미지이름
			char 	*icon_image,						// 
			int 	font_no,							// 버튼에 사용될 폰트번호
			char	*text,								// 버튼에 표시되는 문자열
			int		function,							// 토글버튼, 푸쉬버튼의 구분
			U32		id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) )
{
	PARAM	parameter;
	HANDLE	handle;
	U32		w_id;
	WidgetStruct wid_struct;	
	
	Windows		*parent_win;
	parent_win = GetWindow(NULL, ContainerHandle, "Button");
    if(parent_win==0)
    {
        Dprintf("Button::Alloc Error\n");
    }
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	parameter = MAKELONG(style, 0);
	
	wid_struct.FontColor = clBlack;

	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;	
	wid_struct.Toggle = function;
	wid_struct.ID = id;
	wid_struct.FontNo = font_no;

	strcpy(wid_struct.IconName, icon_image);
	
    handle=makeWindow(
				LCD,						// Display Section Position
				"Button",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,			// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				ButtonPaintEvent,			// PaintEvent
				ButtonMouseOnEvent,			// MouseOnEvent
				ButtonMouseOffEvent,		// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				ClickEvent,					// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				NULL,  						// ChangeEvent
                NULL,                       // KeyEvent
                NULL,                       // EncoderEvent
                ButtonFocusEvent,           // FocusEvent
                NULL,                       // CloseEvent
				parameter,					// PARAMETER
				normal_image,				// Normal Touch Screen or BMP
				touch_image,				// Touch Screen or ICON
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
