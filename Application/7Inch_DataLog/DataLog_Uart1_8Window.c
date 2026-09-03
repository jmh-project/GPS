#ifdef __cplusplus
 extern "C" {
#endif
#include "TEECO_System.h"
#include "DataLog.h"
volatile static int play_timer=0;

int  SmartLog_Uart1Eight_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	#if defined(TYPE_APPROVAL_ENABLE) && defined(RTU_VER12_ENABLE)
	if((win->ID==1||win->ID==2||win->ID==3||win->ID==4)&&(strcmp(flash.password,gv.password))) {
		PasswordKeyPad(win->Parent, win->Handle,  win->ID, (1024-345)/2, (LCD_Y-415)/2, 1, play_timer, "PASSWORD");
	}
	else {
		DataLog_Key_Event( parent, handle, play_timer, win->ID);
	}
	#else
		DataLog_Key_Event( parent, handle, play_timer, win->ID);
	#endif
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
void ScreenFillRect(int sx, int sy, int width, int height, int color);
int LowTextOut(int sx, int sy, const char *s, int fontcolor, int font);
int  SmartLog_Uart1Eight_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		return(0);
	}
//    static int toggle=0;
	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

//    if(toggle==0) {
//        toggle=1;
//        SetGraphicsPage(LCD_ALPHA_SCREEN);        
//        ScreenFillRect(0, 0, LCD_X, LCD_Y/2, clBlue);
//        LowTextOut(100, ((768-32)/2) -60, "BOOTLOADER VER 1.110", clRed, fTimeRoma16);
//    } 
//    else if(toggle==1) {
//        toggle=2;
//        SetGraphicsPage(LCD_ALPHA_SCREEN);
//        ScreenFillRect(0, 0, LCD_X, LCD_Y/3, clYellow);
//    }
//    else if(toggle==2) {
//        toggle=3;
//        SetGraphicsPage(LCD_ALPHA_SCREEN);
//        ScreenFillRect(0, 0, LCD_X, LCD_Y/4, clRed);
//    }
//    else if(toggle==3) {
//        toggle=0;
//        SetGraphicsPage(LCD_ALPHA_SCREEN);
//        ScreenFillRect(0, 0, LCD_X, LCD_Y/5, clGreen);
//    }
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
            DataLog_Data_Display( USART_1, 8 );
        }
    }
    else
        DataLog_Data_Display( USART_1, 8 );
    
  //통신상태를 이미지로 출력한다.  
//    if(gv.serialComStatusToggle[USART_1]==1) {
//        SetWidgetIcon(gv.handle[30],"48G_LED_Blue_Icon1.bmp");//통신 수신
//    } else if(gv.serialComStatusToggle[USART_1]==0) {
//        SetWidgetIcon(gv.handle[30],"48G_LED_Yellow_Icon1.bmp");//통신 요청
//    } else {
//        SetWidgetIcon(gv.handle[30],"48G_LED_Red_Icon1.bmp");//통신 불량
//    }
    
    //Dprintf("TIME ID = %08x\n",gv.titleBarLogId);

    PaintDrawControl(ON);	
    
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
void FloatPoint_Process(int ch, int comPort,  char *data);
int  SmartLog_Uart1Eight_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	Gr->FillRect(0, 0, 0, 0);//Dummy
	if(flash.systemColorNo==6) {
		Gr->SetColor(RGB565CONVERT(200,  200,  200));
	}
	else {
		Gr->SetColor(clBlack);
	}
	Gr->FillRect(0, 0, LCD_X, LCD_Y);

    //STextBox( handle, 10, 60, 500, 200, 1, "1234.0", WHITE, BLACK, CENTER_ALIGN, READ_ONLY,NULL, NULL);
    
    char    pBuf[120];
    for(int i=0; i<120; i++) {
        pBuf[i]=0;
    }
    #ifdef DATALOG_7INCH
    unsigned int groupSpace = 5;
    #endif
    #ifdef DATALOG_8INCH
    unsigned int groupSpace = 10;
    #endif
    
    //unsigned int groupSpace = 5;
    
    unsigned int    comPort = USART_1;
    //unsigned int    channel = 0;
    //unsigned int space = 2;
    unsigned int titleBarY = 59;//50
    
    unsigned int groupTitleBar = 40;
    unsigned int groupX = 8;
    unsigned int groupY = groupSpace+titleBarY;
    unsigned int groupW = 500;
    unsigned int groupH = 149;//115
        
    unsigned int textX = groupX;
    unsigned int textY = groupY+ groupTitleBar;
    unsigned int textW = groupW;
    unsigned int textH = groupH-groupTitleBar;
        
    unsigned int labelBackColor = flash.systemColor;  //RGB565CONVERT(191, 191, 191);
    unsigned int labelFontColor = flash.systemFontColor;
    
    	
    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Uart[comPort]->SetupName[0]);
    FloatPoint_Process(0,comPort,pBuf);
    pBuf[10]=0;
    gv.handle[0] = NLabel(handle, textX, textY, textW, textH,pBuf,labelFontColor,labelBackColor,3);
    
    groupX = groupX + 10 + groupW;
    textX = groupX;
    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Uart[comPort]->SetupName[1]);

    FloatPoint_Process(1,comPort,pBuf);
    pBuf[10]=0;
    gv.handle[1] = NLabel(handle, textX, textY, textW, textH,pBuf,labelFontColor,labelBackColor,3);

    groupX = 8;
    groupY = groupY + groupH + groupSpace;
    textX = groupX;
    textY = groupY + groupTitleBar;
    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Uart[comPort]->SetupName[2]);

    FloatPoint_Process(2,comPort,pBuf);
    gv.handle[2] = NLabel(handle, textX, textY, textW, textH,pBuf,labelFontColor,labelBackColor,3);

    groupX = groupX + 10 + groupW;
    textX = groupX;
    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Uart[comPort]->SetupName[3]);
    FloatPoint_Process(3,comPort,pBuf);
    gv.handle[3] = NLabel(handle, textX, textY, textW, textH,pBuf,labelFontColor,labelBackColor,3);

    groupX = 8;
    groupY = groupY + groupH + groupSpace;
    textX = groupX;
    textY = groupY + groupTitleBar;
       
 
    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Uart[comPort]->SetupName[4]);
    FloatPoint_Process(4,comPort,pBuf);
    gv.handle[4] = NLabel(handle, textX, textY, textW, textH,pBuf,labelFontColor,labelBackColor,3);

    groupX = groupX + 10 + groupW;
    textX = groupX;
    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Uart[comPort]->SetupName[5]);
    FloatPoint_Process(5,comPort,pBuf);
    pBuf[10]=0;
    gv.handle[5] = NLabel(handle, textX, textY, textW, textH,pBuf,labelFontColor,labelBackColor,3);

    groupX = 8;
    groupY = groupY + groupH + groupSpace;
    textX = groupX;
    textY = groupY + groupTitleBar;

    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Uart[comPort]->SetupName[6]);
    FloatPoint_Process(6,comPort,pBuf);
    gv.handle[6] = NLabel(handle, textX, textY, textW, textH,pBuf,labelFontColor,labelBackColor,3);

    groupX = groupX + 10 + groupW;
    textX = groupX;
    NGroupPanel(handle, groupX, groupY, groupW, groupH, flash.Uart[comPort]->SetupName[7]);
    FloatPoint_Process(7,comPort,pBuf);
    gv.handle[7] = NLabel(handle, textX, textY, textW, textH,pBuf,labelFontColor,labelBackColor,3);
    
    TitleBar_MenuButton_Create(handle,comPort,SmartLog_Uart1Eight_KeyEvent);

    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	//타이머를 여기서 등록한다.
	play_timer =  OpenTimer(parent, handle, 300);	
    StartTimer(play_timer);  	
    gv.autoScreenTimer = 0;    
	return(1);
}

int  SmartLog_Uart1Eight_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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

int  SmartLog_Uart1Eight_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
int  SmartLog_Uart1Eight_Window(HANDLE parent, HANDLE handle)
{
	if(GetWindowName(" SmartLog_Uart1Eight_Window")) 
	{
		Dprintf(" SmartLog_Uart1Eight_Window:: Windows already Create\n");
		return 0;		
	}	
	makeWindow
    (
        LCD,								// Display Section Position, 이윈도우가 어디에 표시되는가?
        " SmartLog_Uart1Eight_Window",			// Windows Name, 이이름으로 이윈도우의 핸들을 찾을수 있다. 64자
        DESKTOP_WINDOW,		// Parent Window Handle
        CONTAINER_WINDOW,	// Windows Kind
        STYLE_NONE,					// Windows Style, TEECO_STYLE, XP_STYLE 두가지가 기본준비되어있다.
        0,0, 							// Start Position
        WINDOW_WIDTH, WINDOW_HEIGHT,				// Size
        SHOW,							// Create after Show
        SmartLog_Uart1Eight_CreateEvent,		// CreateEvent
        SmartLog_Uart1Eight_PaintEvent,			// PaintEvent
        NULL,	// MouseOnEvent
        NULL,	// MouseOffEvent
        NULL,							// MouseMoveEvent
        NULL,							// GestureEvent
        NULL,						// ClickEvent, 콜백함수를 등록한다.(콜백함수는 어떤것도 등록가능하다)
        SmartLog_Uart1Eight_TimerEvent,			// TimerEvent
        NULL,		// ControlEvent, 이윈도우를 제어하기위해 사용된다.
        NULL,		// ChangeEvent, 윈도우의 속성값이 
        NULL,			// KeyEvent
        NULL, 		// EncoderEvent, 시스템에 엔코더가 연결되오 있을때 발생된다.
        NULL, 							// FocusEvent,  마우스를 사용할때 커서가 위에 있으면 발생된다.
        SmartLog_Uart1Eight_CloseEvent,			// Close Event, 윈도우가 닫힐때 발생한다.
        NULL,							// PARAMETER, window->Param에 저장된다.
        NULL,							// NormalName용 BMP 파일명, window->NormalName, 
        NULL,							// TouchName용 BMP파일명, window->TouchName
        NULL,							// Windows Text, win->Text 에 저장된다. 128Byte의 사이즈를 가지고 있다. 초과되지 않도록 한다.
        MODALLESS_WINDOW,	// window->Mode에 저장된다.
        NULL					// WidgetStruct 구조체이다. 속성이 모자랄때 추가하기위해 사용한다.
    );
    if(parent==NULL && handle==NULL)//백그라운드 윈도우를 삭제한다.
    {
        Dprintf(" SmartLogTwoChannel :: MainWindows Create \n");
    }
    else 
    {
        //Dprintf("CloseWindows parent = %d handle = %d\n", parent, handle);
        CloseWindow(parent, handle);	
    }
	return(1);
}				
#ifdef __cplusplus
}
#endif
