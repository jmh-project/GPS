/******************************************************************************
*       File name	: GraphBox.c 
*		Description : TEECO GUI Window Control Widget [GraphBox]
*		Company		: idnics co.,
*		Version		: 0.10 (2014년 10월28일 1차 배포함)
*		Version		: 0.11 
*		Contact		: www.ziontek.co.kr
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
// 위젯을만들때는 PaintDrawControl() 함수를 사용하면 안된다.
// 이파일은 위젯을작성할때 복사해서 사용해야하는 견본이다.
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
/******************************************************************************
							Constant & Macros
******************************************************************************/
//투명위젯을 만들때 사용한다.
#define		TRANCYPARENCY

/******************************************************************************
							Private & Local Variables
******************************************************************************/
static float	Graph_Y_Max;		// 주어진사이즈에서 Y의 최대값
static int		Graph_X_Inc;			// X 증가값
//static float	*Graph_Data;

/******************************************************************************
							Function Prototype
******************************************************************************/
extern void GraphicsPointerSave(void);
extern void GraphicsPointerLoad(void);
/******************************************************************************
							Example Program
******************************************************************************/
//=============================================================================
// 작성한 위젯의 데이터를 갱신하기 위해서는 아래와같은 함수를 만들어야한다.
// 전달할데이터의 구조에따라 매개변수는 필요한대로 지정한다.
// 예제에서는 PaintEvent를 호출하지만 프로그램의 흐름에따라 다른 이벤트를 호출해도 상관없다.
//=============================================================================
//만들어진 그래프의 X 인터벌을 변경한다.
int SetWidget_GraphInterval(int x)
{
	if(x > 0)	Graph_X_Inc = x;
	else
	{
		Dprintf("SetWidget_GraphInterval::value is minus = %d\n", x);
		return(0);
	}
	return(1);
}
// 만들어진 그래프의 Y 최대값을 변경한다.
int SetWidget_GraphMaxY(float y)
{
	if(y > 0)	Graph_Y_Max = y;
	else
	{
		Dprintf("SetWidget_GraphMaxY::value is minus = %f\n", y);
		return(0);
	}
	return(1);
}

// 만들어진 그래프에 데이터를그리기위해 전송할때 사용하는 함수이다.
// 그래프의 위젯아이디와 표시할 데이터의 배열, 그리고 길이를 전달한다.
int SetWidget_GraphData(U32 id,  float *display_data, int data_len)
{	
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    
	GraphicsPointerSave();
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, data_len, display_data);
	
	GraphicsPointerLoad();
	
	return(1);		
}


// 만들어진 그래프에 데이터를그리기위해 전송할때 사용하는 함수이다.
// 그래프의 위젯아이디와 표시할 데이터의 배열, 그리고 길이를 전달한다.
int SetWidget_GraphDataFit(U32 id,  float *display_data, int data_len)
{	
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	int	old_inc_x = Graph_X_Inc;
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    
	GraphicsPointerSave();
	
	//표시할데이터가 윈도우사이즈보다 크면 압축평균을 낸다.
	if(data_len > win->width)
	{
	}
	else //표시할데이터가 윈도우사이즈안에 들어온다.
	{
		Graph_X_Inc = win->width / data_len;
	}
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, data_len, display_data);
	
	GraphicsPointerLoad();
	
	Graph_X_Inc = old_inc_x;
	
	return(1);		
}


//=============================================================================
// TimerEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
//=============================================================================
int	Graph_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			page=0;
	int			sx=0, sy=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
#ifdef TRANCYPARENCY	
	//부모윈도우의 바탕색을 칠한다.투명처리용 위젯을 만들때 사용한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
#endif	
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//=============================================================================
// MouseOnEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
//=============================================================================
int	Graph_MouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			page=0;
	int			sx=0, sy=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
#ifdef TRANCYPARENCY	
	//부모윈도우의 바탕색을 칠한다.투명처리용 위젯을 만들때 사용한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
#endif	
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//=============================================================================
// MouseOffEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
//=============================================================================
int	Graph_MouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			page=0;
	int			sx=0, sy=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
#ifdef TRANCYPARENCY	
	//부모윈도우의 바탕색을 칠한다.투명처리용 위젯을 만들때 사용한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
#endif	
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//=============================================================================
// CreateEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
// 위젯이 처음 만들어질때만 호출된다. 초기화, 폰트로딩, 시간이 걸리는 그림파일 로딩등 
// 의 작업을 한다.
//=============================================================================
int	Graph_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//U32		timerID=0;
	int			page=0;
	int			sx=0, sy=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
#ifdef TRANCYPARENCY	
	//부모윈도우의 바탕색을 칠한다.투명처리용 위젯을 만들때 사용한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
#endif	
	win->FontColor = clRed;
	//사용자 프로그램 영역
	{
		//timerID =  OpenTimer(parent, handle, 1000);	
		//StartTimer(timerID);
	}
	Gr->Draw(parent, handle, command, param, Data);
    return(1);
}

//=============================================================================
// PaintEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
//=============================================================================
int	Graph_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			page=0, i;
	int			sx=0, sy=0;
	float			factor, dis_data;
	static int 	first=0;
	int			dis_x;
	float			*dis_buf = (float *)Data; //표시해야할 데이터의 배열이다.
	int			old_x, old_y;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
#ifdef TRANCYPARENCY	
	//부모윈도우의 바탕색을 칠한다. 투명처리용 위젯을 만들때 사용한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);	
#endif
	
#ifndef TRANCYPARENCY		
	Gr->SetColor(clBlue);
	Gr->FillRect(0, 0, win->width, win->height);//Dummy	
#endif
	Gr->SetColor(win->FontColor);
	if(first==0)
	{
		first=1;
		Gr->SetColor(clBlack);
	}
	else
	{
		//기울기를 구한다.
		factor = ((float)(win->height-1) / Graph_Y_Max) * -1.00f; 
		
		//param 은 표시해야할 데이터의 길이
		for(i=0; i<param; i++)
		{
			//입력된데이터를 그래프에 맞는 값으로 변경한다.
			dis_data = (factor * dis_buf[i]) + (float)win->height;
			if(i<2)
			{
				Gr->MoveTo(dis_x=0, (int)dis_data);
				old_x = dis_x; old_y = (int)dis_data;
			}
			else
			{		
				//굵게 그리기위해 3번을 그린다.
				Gr->LineTo(dis_x, (int)dis_data); 	// Line 1
				
				Gr->MoveTo(old_x, old_y-1);
				Gr->LineTo(dis_x, (int)dis_data-1);	// Line 2
				
				Gr->MoveTo(old_x, old_y+1);
				Gr->LineTo(dis_x, (int)dis_data+1);	// Line 3
				
				old_x = dis_x; old_y = (int)dis_data;

				//Dprintf("%2d %5.1f %5.1f\n", i, dis_buf[i], dis_data);
			}
			dis_x += Graph_X_Inc;
		}
	}	
	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//=============================================================================
// 컨테이너가 닫히면서 컨테이너에서 보내오는 위젯클로즈 이벤트이다.
// 위젯자체의 데이터기능을 활성화할려면 핸들로 ID 를 구분하여야 한다.
//=============================================================================

int	 Graph_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	//윈도우가 닫힐때 필요한처리를 한다.

	return(1);
}

U32 Graph(	HANDLE ContainerHandle, 				// 위젯윈도우이므로 컨테이너를지정해야한다.
			int 		sx, int sy, int width, int height, 	// 컨테이너안에 위젯의위치와 크기지정
			float	ymax,	// Y max 이값은 SetWidget으로 변경할수있다.
			int		x_inc )	// X축 증가값  이값은 SetWidget으로 변경할수있다.
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;		// makeWindow에 매개변수가 늘어나는것을 방지하기위해 구조체를 사용한다.
	
	parent_win = GetWindow(NULL, ContainerHandle,"Graph");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	Graph_Y_Max = ymax;
	Graph_X_Inc = x_inc;
	
	handle=makeWindow(
				LCD,				// Display Section Position
				"Graph",				// Windows Name
				ContainerHandle,		// Parent Window Handle
				WIDGET_WINDOW,	// Windows Kind
				STYLE_NONE,		// Windows Style
				sx, sy, 				// Start Position
				width, height,			// Size
				SHOW,				// Create after Show
				Graph_CreateEvent,	// CreateEvent
				Graph_PaintEvent,		// PaintEvent
				NULL,				// MouseOnEvent
				NULL,				// MouseOffEvent
				NULL,				// MouseMoveEvent
				NULL,				// GestureEvent
				NULL,				// ClickEvent
				Graph_TimerEvent,	// TimerEvent
				NULL,  				// ControlEvent
				NULL,				// ChangeEvent
                NULL,           	// KeyEvent(Button)
                NULL,               // EncoderEvent
                NULL,               // FocusEvent
                Graph_CloseEvent,	// CloseEvent
				NULL,				// PARAMETER
				NULL,				// Back Image
				NULL,				// 
				NULL,				// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	id = MAKELONG(ContainerHandle, handle);		//LOWORD->ContainerHandle, HIWORD->handle
	return(id);
}				
#ifdef __cplusplus
}
#endif
