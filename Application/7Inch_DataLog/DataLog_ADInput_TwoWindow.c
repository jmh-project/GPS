/******************************************************************************
*       File name	: Windows_ SmartLog_ADTwo.c 
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
#include "DataLog.h"
#if 1
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
U32 STextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,		//24 는 24사이즈, 48, 64 3가지가 준비되어있다.
			char	*text,		//init Character
			int	fontcolor, 
			int	backcolor,
			int	align,		//Left, Right, Center
			int	attr,		//READ_ONLY, RW_MODE
			U32	id,
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
U32 NGroupPanel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text);
U32 STransTextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		align,		//Left, Right, Center
			int		attr,		//READ_ONLY, RW_MODE
			int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
U32 SLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style);
U32 NLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style);        
int DataLog_Data_Display(unsigned int comPort, unsigned int channel); 
int DataLog_Key_Event( HANDLE parent, HANDLE handle, HANDLE timer, HANDLE id );     
/******************************************************************************
							Example Program
******************************************************************************/
int  SmartLog_ADTwo_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

    DataLog_Key_Event( parent, handle, play_timer, win->ID);

    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
int  SmartLog_ADTwo_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
    
    //100 ADC 데이터를 출력하라는 숫자
    if(gv.autoScreen == ON)
    {
        if(gv.autoScreenTimer == AUTO_SCREEN_TIMER)
        {
            StopTimer(play_timer);
            CloseTimer(play_timer);
            DataLog_Key_Event( parent, handle, play_timer, 7);
        }
        else 
        {
            gv.autoScreenTimer++;
            DataLog_Data_Display( ADC_DATA_DISPLAY, 2 );
        }
    }
    else
        DataLog_Data_Display( ADC_DATA_DISPLAY, 2 );
    
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
int  SmartLog_ADTwo_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	Gr->SetColor(RGB565CONVERT(200,  200,  200));
	Gr->FillRect(0, 0, LCD_X, LCD_Y);

    //STextBox( handle, 10, 60, 500, 200, 1, "1234.0", WHITE, BLACK, CENTER_ALIGN, READ_ONLY,NULL, NULL);

    unsigned int groupSpace = 143+84;
    
    unsigned int titleBarY = 40;
    unsigned int groupX = 8;
    unsigned int groupY = groupSpace+titleBarY;
    unsigned int groupW = 500;
    unsigned int groupH = 200;
    
    unsigned int textX = groupX; 
    unsigned int textY = groupY+ titleBarY;
    unsigned int textW = groupW;
    unsigned int textH = groupH-titleBarY;
    
    
    unsigned int labelBackColor = RGB565CONVERT(191, 191, 191);
    unsigned int labelFontColor = clBlack;
    char    data[20];
    
    
    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Input[0]->Name);
	
    sprintf(data,"%10.2f",flash.Input[0]->PV);
    gv.handle[0] = NLabel(handle, textX, textY, textW, textH,data,labelFontColor,labelBackColor,3);
    
    groupX = groupX + 10 + groupW;
    textX = groupX;

#ifdef 	INPUT_PORT_SELECT_1
	NGroupPanel(handle, groupX, groupY, groupW, groupH, "ALARM");
    sprintf(data,"%d",flash.relay[0]);
    gv.handle[1] = NLabel(handle, textX, textY, textW, textH,data,labelFontColor,labelBackColor,1);
#endif  
#ifdef 	NORMAL_PORT
	NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Input[1]->Name);
    sprintf(data,"%10.2f",flash.Input[1]->PV);
    gv.handle[1] = NLabel(handle, textX, textY, textW, textH,data,labelFontColor,labelBackColor,3);
#endif	


    char    pBuf[30];
    TitleBar_Create(handle,6,pBuf);
    TitleBar(handle, 0, 0, LCD_X, 50, 1, pBuf, clYellow, 1, RIGHT_ALIGN, 3, NULL);	
    
    //U32 SLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style)
    gv.titleBarLogId = SLabel(handle, (1024-345) / 2, 5, 345, 40,gv.titleBuf,clBlack,RGB565CONVERT(230, 230, 230),1);
    //DS1302TimeRead();
    sprintf(pBuf,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
    gv.titleBarTimeId = SLabel(handle, 1024-230, 5, 220, 40,pBuf,clBlack,RGB565CONVERT(230, 230, 230),1);
    SButton(handle, 0, LCD_Y-65, 180, 64, "48G_Config_Icon2.bmp", "SENSOR",  PUSH_BUTTON, 1, SmartLog_ADTwo_KeyEvent);
    SButton(handle, 0+180, LCD_Y-65, 170, 64, "48G_TestBottle_Icon1.bmp", "CAL",  PUSH_BUTTON, 2, SmartLog_ADTwo_KeyEvent);
    SButton(handle, 0+180+170, LCD_Y-65, 170, 64, "48G_Network_Icon1.bmp", "SYSTEM",  PUSH_BUTTON, 3, SmartLog_ADTwo_KeyEvent);
    SButton(handle, 0+180+170+170, LCD_Y-65, 170, 64, "48G_USB_Icon1.bmp", "USB",  PUSH_BUTTON, 4, SmartLog_ADTwo_KeyEvent);
	gv.handle[50]=SButton(handle, 0+180+170+170+170, LCD_Y-65, 170, 64, "48G_Shutdown_Icon1.bmp", "안전리셋",  PUSH_BUTTON, 5, SmartLog_ADTwo_KeyEvent);
	SButton(handle, 0+180+170+170+170+170, LCD_Y-65, 164, 64, "48G_Display_Icon1.bmp", "NEXT",  PUSH_BUTTON, 6, SmartLog_ADTwo_KeyEvent);
	gv.autoScreenTimer = 0;
	
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	//타이머를 여기서 등록한다.
	play_timer =  OpenTimer(parent, handle, 1000);	
    StartTimer(play_timer);  	
    
	return(1);
}

int  SmartLog_ADTwo_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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

int  SmartLog_ADTwo_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
int  SmartLog_ADTwo_Window(HANDLE parent, HANDLE handle)
{
	if(GetWindowName(" SmartLog_ADTwo_Window")) 
	{
		Dprintf(" SmartLog_ADTwo_Window:: Windows already Create\n");
		return 0;		
	}	
	makeWindow
    (
        LCD,								// Display Section Position, 이윈도우가 어디에 표시되는가?
        " SmartLog_ADTwo_Window",			// Windows Name, 이이름으로 이윈도우의 핸들을 찾을수 있다. 64자
        DESKTOP_WINDOW,		// Parent Window Handle
        CONTAINER_WINDOW,	// Windows Kind
        STYLE_NONE,					// Windows Style, TEECO_STYLE, XP_STYLE 두가지가 기본준비되어있다.
        0,0, 							// Start Position
        WINDOW_WIDTH, WINDOW_HEIGHT,				// Size
        SHOW,							// Create after Show
        SmartLog_ADTwo_CreateEvent,		// CreateEvent
        SmartLog_ADTwo_PaintEvent,			// PaintEvent
        NULL,	// MouseOnEvent
        NULL,	// MouseOffEvent
        NULL,							// MouseMoveEvent
        NULL,							// GestureEvent
        NULL,						// ClickEvent, 콜백함수를 등록한다.(콜백함수는 어떤것도 등록가능하다)
        SmartLog_ADTwo_TimerEvent,			// TimerEvent
        NULL,		// ControlEvent, 이윈도우를 제어하기위해 사용된다.
        NULL,		// ChangeEvent, 윈도우의 속성값이 
        NULL,			// KeyEvent
        NULL, 		// EncoderEvent, 시스템에 엔코더가 연결되오 있을때 발생된다.
        NULL, 							// FocusEvent,  마우스를 사용할때 커서가 위에 있으면 발생된다.
        SmartLog_ADTwo_CloseEvent,			// Close Event, 윈도우가 닫힐때 발생한다.
        NULL,							// PARAMETER, window->Param에 저장된다.
        NULL,							// NormalName용 BMP 파일명, window->NormalName, 
        NULL,							// TouchName용 BMP파일명, window->TouchName
        NULL,							// Windows Text, win->Text 에 저장된다. 128Byte의 사이즈를 가지고 있다. 초과되지 않도록 한다.
        MODALLESS_WINDOW,	// window->Mode에 저장된다.
        NULL					// WidgetStruct 구조체이다. 속성이 모자랄때 추가하기위해 사용한다.
    );
    if(parent==NULL && handle==NULL)//백그라운드 윈도우를 삭제한다.
    {
        Dprintf(" SmartLog_ADTwo_Window :: MainWindows Create \n");
    }
    else 
    {
        //Dprintf("CloseWindows parent = %d handle = %d\n", parent, handle);
        CloseWindow(parent, handle);	
    }
	return(1);
}				
#endif

#ifdef __cplusplus
}
#endif
