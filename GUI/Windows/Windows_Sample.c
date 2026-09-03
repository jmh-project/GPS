/******************************************************************************
*       File name	: Windows_Sample.c 
*		Description 	: TEECO GUI Window Control Widget [TransParencyTextBox]
*		Company	: idnics co.,
*		Version		: 0.10 (2014년 5월28일 1차 배포함)
*		Version		: 0.11 (2014년 5월28일 :compile Error로인해 수정배포
*		Version		: 0.2 (2015년 3월9일 : 수정배포
*		Contact		: www.ziontek.co.kr
******************************************************************************/
// 이파일은 윈도우를 작성할때 복사해서 사용해야하는 견본이다.
#ifdef __cplusplus
 extern "C" {
#endif
 
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

#if 0
/******************************************************************************
							Constant & Macros
******************************************************************************/
// 이윈도우에서 사용할 터치 사운드의 정의
#define		TOUCH_SOUND			1	 

/******************************************************************************
							Private & Local Variables
******************************************************************************/
volatile static int play_timer=0;
/******************************************************************************
							Function Prototype
******************************************************************************/

/******************************************************************************
							Example Program
******************************************************************************/
// 키패드사용시 사용하는 콜백함수이다.
int  Sample_KeyPadEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	if(command==1) // Caller ID
	{
		char	buf[128];
		strcpy(buf, (char *)Data);
	}
}

//이함수는 위젯의 키이벤트를 받기위해 사용한다.
int  Sample_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle," ");
	Graphics 	*Gr = GetGraphics(NULL, parent, CLIENT_AREA); 	//그래픽영역을 얻는다.


	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);	//dummy code	       
    
    if(win->ID==1)
    {
		VoicePlay(TOUCH_SOUND);
		//SKeyPad(0, parent, NULL, (LCD_X-345) / 2 , (LCD_Y - 415) / 2, clBlack, "", "Year Input", win->ID, Sample_KeyPadEvent);		
    }
    else if(win->ID==2)
    {
    }
    else if(win->ID==3)
    {
    }
    else if(win->ID==4)
    {
    }

	//키가 눌렸을때 키의 속성을 바꿀때 사용하는방법
    /*
	if(command==KEY_ON)	  SetWidgetFocus(test_btn_id, 1);
    else 	  SetWidgetFocus(test_btn_id, 0);
	*/
	
	PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);    
	return(1);
}

int Sample_EncoderEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	int 		enc_no = (int)LOWORD(param);
	int 		enc_value = (int)HIWORD(param);	
	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

	Dprintf("command = %02x enc_no=%d enc_value=%d\n", command, enc_no, enc_value);

    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Sample_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//사용자 프로그램 영역
	if(command==1)	// command 로 타이머의 ID를 구분한다.
	{
		
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Sample_ChangeEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//사용자 프로그램 영역
	if(command==1)	// command 로 타이머의 ID를 구분한다.
	{
		
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Sample_ControlEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//사용자 프로그램 영역
	if(command==1)	// command 로 타이머의 ID를 구분한다.
	{
		
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Sample_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//사용자 프로그램 영역
	if(command==1)	// command 로 타이머의 ID를 구분한다.
	{
		
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Sample_MouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
	
	//마우스의 좌표를 읽을때 사용한다.
	int 		tx = LOWORD(param);
	int 		ty = HIWORD(param);	

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	Dprintf("Sample_MouseOnEvent::X=%4d Y=%4d\n", tx, ty);
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

	//사용자 프로그램 영역
    //win = GetWindow(0, parent, "NIBPMainKeyEvent");
    //win = GetWindowByName("NIBPMain");	
	
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Sample_MouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//마우스의 좌표를 읽을때 사용한다.
	int 		tx = LOWORD(param);
	int 		ty = HIWORD(param);	

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	Dprintf("Sample_MouseOffEvent::X=%4d Y=%4d\n", tx, ty);
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//사용자 프로그램 영역
	{
		
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Sample_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//투명위젯을 사용하려면 Paint Event에서 할배경작업을 미리해야한다. 그림이 필요하면 그림먼저 로딩할것(*********************** 중요 *********)
	//또는 PPaintDrawControl(ON); ->Gr->Draw를 실행후 사용한다.
	Gr->SetColor(RGB565CONVERT(230,  230,  230));
	Gr->FillRect(0, 0, win->width-2, win->height-2);

	Gr->BmpLoad(USER_SCREEN1, 0, 0, "WaterCell_Main_2.bmp");		//평소의 화면
	Gr->BmpLoad(USER_SCREEN2, 0, 0, "WaterCell_Main_2_t.bmp");		//터치화면(효과화면)
    Gr->BitBlt(USER_SCREEN1, 0, 0, 0, 51, 1024, 640);
	
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	//타이머를 여기서 등록한다.
	//play_timer =  OpenTimer(parent, handle, 500);	
    //StartTimer(play_timer);  	
    
	return(1);
}

int Sample_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	static int start=0;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//사용자 프로그램 영역
    //Gr->BmpDisplay(0, 0, "Kim_image.bmp");    
	if(start==0) //메인윈도우 같이 한번생성후 계속사용할때, 1회만 처리할내용
	{
		Gr->SetColor(RGB565CONVERT(230, 230, 230));
		Gr->FillRect(0, 700, LCD_X, 68);
		start=1;
	}	
	Gr->SetColor(clBlue);
	Gr->FillRect(0, 0, LCD_X, LCD_Y);
	
	//타이머이벤트등에서  배경을 갱신하기위한 사용법, 호출시에 commad 에 값을넣고 호출한다.
	//콘트롤이나 체인지 이벤트를 사용하면 더 깔끔할수도 있다.
	if(command==1)
	{
	}
	else if(command==2)
	{
	}	
	PaintDrawControl(ON);		
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int Sample_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//윈도우가 닫힐때 필요한처리를 한다.
	Windows *win=NULL;

	switch(handle) //컨테이너윈도우 
	{
		case 1: 	win = gp_WidgetWindow_1;	break;
		case 2: 	win = gp_WidgetWindow_2;	break;
		case 3: 	win = gp_WidgetWindow_3;	break;
		case 4: 	win = gp_WidgetWindow_4;	break;
		case 5: 	win = gp_WidgetWindow_5;	break;
		case 6: 	win = gp_LcdAlphaWidgetWindow;	break; //
		case 7: 	win = gp_VideoAlphaWidgetWindow;	break; //
		default:	Dprintf("FindWidgetChain::Windows_no mistyping\n"); return(0);
	}

	if(win== NULL) 
	{
		Dprintf("TabelTest_CloseEvent::Invalid Handle\n");
		return (NULL);
	}
	// 이윈도우에서 malloc 사용시 여기서 free 해주어야한다.
	
	int Done=1;
	while(Done) //위젯윈도우를 모두찾아 CloseEvent를 전송한다.
	{
		if(win->CloseEvent != NULL) win->CloseEvent(0, 0, 0, 0, 0);
		if(win->NextWindow ==NULL) return(NULL);
		win = (Windows *)win->NextWindow;
	}	
	return(1);
}

#define		WINDOW_WIDTH		LCD_X
#define		WINDOW_HEIGHT		LCD_Y

// 이윈도우의 기능이 많을경우 콜백함수를 여러게 등록해서 사용하면 작업을 분산시킬수 있다.
// 백그라운드 윈도우로 동작하고 이벤트나 데이터 관리후 필요에따라 전달할경우에 사용한다.
// GUI 없이도 태스킹 프로그램에 적용하는것을 고려해서 의논할것
int Sample_Window(int 	sx, 
							int 	sy, 
							char 	*IconName,
							int 	id,
							int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) ) //사용자가 이 윈도우에서 받을 콜백함수를 등록한다.
{
	if(GetWindowName("Sample_Window")) 
	{
		Dprintf("Sample_Window:: Windows already Create\n");
		return 0;		
	}	
	/*
	//화면의 중앙에 표시할때 사용한다.
	int sx = (LCD_X - WINDOW_WIDTH) / 2;
	int sy = (LCD_Y - WINDOW_HEIGHT) / 2;
	*/
	WidgetStruct 	wid_struct;				//위젯용이지만 윈도우에서도 일부 사용한다.
	//!< 반드시 지워야한다.
	memset(&wid_struct, 0, sizeof(WidgetStruct));
	// 아이콘이름을 가지고 가는 방법
	strcpy(wid_struct.IconName, Iconname);
	// ID 를 등록하는 방법
	wid_struct.CallWidget_ID = id;		// window->CallWidget_ID 에 대입된다.
	//
	// void LCD_AlphaScreen(int s);
	// void Lcd_AlphaTransparency(int value);
	makeWindow(
				LCD,								// Display Section Position, 이윈도우가 어디에 표시되는가?
				"Sample_Window",			// Windows Name, 이이름으로 이윈도우의 핸들을 찾을수 있다. 64자
				DESKTOP_WINDOW,		// Parent Window Handle
				CONTAINER_WINDOW,	// Windows Kind
				STYLE_NONE,					// Windows Style, TEECO_STYLE, XP_STYLE 두가지가 기본준비되어있다.
				sx, sy, 							// Start Position
				WINDOW_WIDTH, WINDOW_HEIGHT,				// Size
				SHOW,							// Create after Show
				Sample_CreateEvent,		// CreateEvent
				Sample_PaintEvent,			// PaintEvent
				Sample_MouseOnEvent,	// MouseOnEvent
				Sample_MouseOffEvent,	// MouseOffEvent
				NULL,							// MouseMoveEvent
				NULL,							// GestureEvent
				ClickEvent,						// ClickEvent, 콜백함수를 등록한다.(콜백함수는 어떤것도 등록가능하다)
				Sample_TimerEvent,			// TimerEvent
				Sample_ControlEvent,		// ControlEvent, 이윈도우를 제어하기위해 사용된다.
				Sample_ChangeEvent,		// ChangeEvent, 윈도우의 속성값이 
				Sample_KeyEvent,			// KeyEvent
				Sample_EncoderEvent, 		// EncoderEvent, 시스템에 엔코더가 연결되오 있을때 발생된다.
				NULL, 							// FocusEvent,  마우스를 사용할때 커서가 위에 있으면 발생된다.
				Sample_CloseEvent,			// Close Event, 윈도우가 닫힐때 발생한다.
				NULL,							// PARAMETER, window->Param에 저장된다.
				NULL,							// NormalName용 BMP 파일명, window->NormalName, 
				NULL,							// TouchName용 BMP파일명, window->TouchName
				NULL,							// Windows Text, win->Text 에 저장된다. 128Byte의 사이즈를 가지고 있다. 초과되지 않도록 한다.
				MODALLESS_WINDOW,	// window->Mode에 저장된다.
				&wid_struct					// WidgetStruct 구조체이다. 속성이 모자랄때 추가하기위해 사용한다.
				);
	return(1);
}				
#endif

#ifdef __cplusplus
}
#endif
