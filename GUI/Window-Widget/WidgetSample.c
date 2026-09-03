/******************************************************************************
*       File name	: WidgetSample.c 
*		Description : TEECO GUI Window Control Widget [TransParencyTextBox]
*		Company		: idnics co.,
*		Version		: 0.10 (2014년 5월28일 1차 배포함)
*		Version		: 0.11 (2014년 5월28일 :compile Error로인해 수정배포
*		Version		: 0.2 (2015년 3월9일 :수정배포
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


/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
extern void GraphicsPointerSave(void);
extern void GraphicsPointerLoad(void);

/******************************************************************************
							Example Program
******************************************************************************/

// 작성한 위젯의 데이터를 갱신하기 위해서는 아래와같은 함수를 만들어야한다.
// 전달할데이터의 구조에따라 매개변수는 필요한대로 지정한다.
// 예제에서는 PaintEvent를 호출하지만 프로그램의 흐름에따라 다른 이벤트를 호출해도 상관없다.
int SetWidgetData(U32 id, void *Data, int cnt)
{	
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle," ");
	if(win==NULL)
	{
		Dprintf("SetWidgetData::Window Error\n");
		return(0);
	}
    
	GraphicsPointerSave();
	strcpy(win->Text, (char *)Data);
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, cnt, Data);
	
	GraphicsPointerLoad();
	
  return(1);		
}

int TransParencyName_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int TransParencyName_MouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int TransParencyName_MouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//위젯에서는 PaintDrawControl()을 사용하면 안된다.
int TransParencyName_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	//사용자 프로그램 영역
	{
		//timerID =  OpenTimer(parent, handle, 1000);	
		//StartTimer(timerID);
	}
	Gr->Draw(parent, handle, command, param, Data);
    return(1);
}

//위젯에서는 PaintDrawControl()을 사용하면 안된다.
int TransParencyName_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//위젯에서는 PaintDrawControl()을 사용하면 안된다.
//매개변수는 필요에의해 추가가 가능하지만 매개변수를 어디에 대입할지는 관리자와 협의해야한다.
U32 TransParencyName_Window(HANDLE ContainerHandle, //위젯윈도우이므로 컨테이너를지정해야한다.
			int 	sx, int sy, int width, int height, 			//컨테이너안에 위젯의위치와 크기지정
			int 	font_no,																//사용할폰트
			char	*text,																	//init Character
			int	align,																	//Left, Right, Center
			int	attr,																		//READ_ONLY, RW_MODE
			int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *))
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	//!< 반드시 지워야한다.
	memset(&wid_struct, 0, sizeof(WidgetStruct));	
	
	parent_win = GetWindow(NULL, ContainerHandle,"");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.FontColor = BLACK;
	wid_struct.FontNo = font_no;
	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;	
	wid_struct.Attr = attr;	
	//wid_struct.ID = id;	//생성되는 윈도우마다 가지는 구조체의 ID
	//strcpy(wid_struct.IconName, IconName);
	//wid_struct.Toggle = function;	 //키의 토글상태의 정의
	handle=makeWindow(
				LCD,												// Display Section Position
				"TransParencyName_Window",			// Windows Name, 64자
				ContainerHandle,							// Parent Window Handle
				WIDGET_WINDOW,							// Windows Kind
				STYLE_NONE,									// Windows Style
				sx, sy, 											// Start Position
				width, height,									// Size
				SHOW,											// Create after Show
				TransParencyName_CreateEvent,		// CreateEvent
				TransParencyName_PaintEvent,			// PaintEvent
				TransParencyName_MouseOnEvent,	// MouseOnEvent
				TransParencyName_MouseOffEvent,	// MouseOffEvent
				NULL,											// MouseMoveEvent
				NULL,											// GestureEvent
				ClickEvent,										// ClickEvent
				TransParencyName_TimerEvent,			// TimerEvent
				NULL,  											// ControlEvent
				NULL,											// ChangeEvent
				NULL,											// KeyEvent
				NULL,											// EncoderEvent, 시스템에 엔코더가 연결되오 있을때 발생된다.
				NULL,											// FocusEvent,  마우스를 사용할때 커서가 위에 있으면 발생된다.
				NULL,											// Close Event, 윈도우가 닫힐때 발생한다.
				align,												// PARAMETER, window->Param에 저장된다.
				NULL,											// NormalName용 BMP 파일명, window->NormalName, 
				NULL,											// TouchName용 BMP파일명, window->TouchName
				text,												// Windows Text, win->Text 에 저장된다. 128Byte의 사이즈를 가지고 있다. 초과되지 않도록 한다.
				MODALLESS_WINDOW,
				&wid_struct									// WidgetStruct 구조체이다. 속성이 모자랄때 추가하기위해 사용한다.
				);
	id = MAKELONG(ContainerHandle, handle);	// LOWORD->ContainerHandle, HIWORD->handle
	//Dprintf("TransParencyName_Window::id = %08x\n", id);	
	return(id);
}				
#ifdef __cplusplus
}
#endif
