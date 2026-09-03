/******************************************************************************
*       File name	: DataLog_Main.c 
*		Description 	: 7인치 데이터로거 시작윈도우
*		Company	: ziontech co.,
*		Version		: 0.10 (2015년 4월23일 Start)
*		Contact		: www.ziontek.co.kr
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"

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
int Info_Window(char *title, char *text);
int SensorMain_Window(HANDLE parent, HANDLE handle);
int SensorInOutChannelConnect(void);
unsigned int    MainScreenCreate( HANDLE parent, HANDLE handle );
void DataLog_VarInit(void);
int IOProcessHardwareControl(void);
int USB_IOProcess(void);
int EXO_IOProcess(void);
int INTELLITECT_IOProcess(void);

#ifdef REMOTE_TERMINAL_UNIT
void RpmSet(unsigned int uartPort, int rpm);
int BoatControl(void);
#endif

/******************************************************************************
							Example Program
******************************************************************************/
int DataLogMain_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	StopTimer(play_timer);
	CloseTimer(play_timer);
    gv.NextScreenCreate=1;
	if(1==SensorInOutChannelConnect())		  	//센서가 연결 되어 있는 경우 
	{
	 	//gv.NoSensorInfo = 1;				//센서 정보가 있는 경우 0이된다.
		MainScreenCreate(parent,handle);	//측정화면을 출력한다.
	}
	else 									 	//센서 정보 설정 화면으로 넘어간다.
	{
	 	//gv.NoSensorInfo = 0;				//센서정보가 없는 경우 1이된다.
        //Dprintf("SensorMain_Window\n");
	 	SensorMain_Window(parent,handle);//센서 정보가 없으므로 센서 설정화면으로 넘어간다.	
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
void IOProcess_Init(void) {
    SmartLog_Uart4Init();//USART 1
    SmartLog_Uart1Init();//USART 2
    SmartLog_Uart3Init();//USART 3
    SmartLog_Uart6Init();//USART 4
        
	IOProcessHardwareControl();	//모든 시스템에 적용된다.
	USB_IOProcess();			//주기적으로 USB를 체크한다
    #if defined(YSI_ODOCT_ENABLE) || defined(YSI_ODD_ENABLE)
    ODO_IOProcess();
    #endif
    if(flash.Uart[USART_5]->SensorID == SENSOR_LAN) {
        W5500_IOProcess();        
    }
	else {
		Dprintf("[DISABLE] W5500_IOProcess\n");
	}
    #ifdef EXO_ENABLE    
	EXO_IOProcess();	 
    #endif	    
}
int DataLogMain_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
    
	Gr->SetColor(BLACK);
	Gr->FillRect(0, 0, LCD_X, LCD_Y);

    IOProcess_Init();

	gv.NextScreenCreate=1;	    // 출력할 화면의 정보를 첫번째 부터 읽어들인다.
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	
    play_timer =  OpenTimer(parent, handle, 100);	//타이머를 여기서 등록한다.
    StartTimer(play_timer);  	
	return(1);
}

int DataLogMain_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	PaintDrawControl(ON);		
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int DataLogMain_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
int DataLogMain_Window(void)
{
	if(GetWindowName("DataLogMain_Window")) {
		Dprintf("DataLogMain_Window:: Windows already Create\n");
		return 0;		
	}	
	makeWindow(
				LCD,							// Display Section Position, 이윈도우가 어디에 표시되는가?
				"DataLogMain_Window",			// Windows Name, 이이름으로 이윈도우의 핸들을 찾을수 있다. 64자
				DESKTOP_WINDOW,					// Parent Window Handle
				CONTAINER_WINDOW,				// Windows Kind
				STYLE_NONE,						// Windows Style, TEECO_STYLE, XP_STYLE 두가지가 기본준비되어있다.
				0, 0, 							// Start Position
				WINDOW_WIDTH, WINDOW_HEIGHT,	// Size
				SHOW,							// Create after Show
				DataLogMain_CreateEvent,		// CreateEvent
				DataLogMain_PaintEvent,			// PaintEvent
				NULL,							// MouseOnEvent
				NULL,							// MouseOffEvent
				NULL,							// MouseMoveEvent
				NULL,							// GestureEvent
				NULL,							// ClickEvent, 콜백함수를 등록한다.(콜백함수는 어떤것도 등록가능하다)
				DataLogMain_TimerEvent,			// TimerEvent
				NULL,							// ControlEvent, 이윈도우를 제어하기위해 사용된다.
				NULL,							// ChangeEvent, 윈도우의 속성값이 
				NULL,							// KeyEvent
				NULL, 							// EncoderEvent, 시스템에 엔코더가 연결되오 있을때 발생된다.
				NULL, 							// FocusEvent,  마우스를 사용할때 커서가 위에 있으면 발생된다.
				DataLogMain_CloseEvent,			// Close Event, 윈도우가 닫힐때 발생한다.
				NULL,							// PARAMETER, window->Param에 저장된다.
				NULL,							// NormalName용 BMP 파일명, window->NormalName, 
				NULL,							// TouchName용 BMP파일명, window->TouchName
				NULL,							// Windows Text, win->Text 에 저장된다. 128Byte의 사이즈를 가지고 있다. 초과되지 않도록 한다.
				MODALLESS_WINDOW,				// window->Mode에 저장된다.
				NULL							// WidgetStruct 구조체이다. 속성이 모자랄때 추가하기위해 사용한다.
				);
	return(1);
}				

#ifdef __cplusplus
}
#endif
