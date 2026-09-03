/******************************************************************************
*       File name	: Windows_Sample.c 
*		Description 	: TEECO GUI Window Control Widget [TransParencyTextBox]
*		Company	: idnics co.,
*		Version		: 0.10 (2014년 5월28일 1차 배포함)
*		Version		: 0.11 (2014년 5월28일 :compile Error로인해 수정배포
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
#include "BSP_DS1302.h"
	 
/******************************************************************************
							Constant & Macros
******************************************************************************/
#define		WINDOW_WIDTH		LCD_X
#define		WINDOW_HEIGHT		LCD_Y

/******************************************************************************
							Private & Local Variables
******************************************************************************/
volatile static int play_timer;
static U32	time_textbox=0;
U32	year_btn=0;
U32	mon_btn=0;
U32	day_btn=0;
U32	hour_btn=0;
U32	min_btn=0;
U32	sec_btn=0;
U32	init_btn=0;

TimeS	Real;
/******************************************************************************
							Function Prototype
******************************************************************************/
void DS1302InitSet(void);			
uint8_t IntToBCD(int data) {
	char	buf[32];
	sprintf(buf,"%02d", data);
	uint8_t  ret= ((buf[0] << 4) & 0xf0) | ( buf[1] & 0x0f);
	return(ret);
}	
			
/******************************************************************************
							Example Program
******************************************************************************/
int	 Calendar_Event(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	int 		key_value;
	uint8_t 	data;
	
	//Dprintf("command=%d %s\n", command, (char *)Data);
	if(command==1) //year Key
	{
		key_value = atoi((char *)Data);
		if(key_value < 2000 || key_value > 2099)
		{
			Info_Window("Range Error", "Year Input Error");
		}
		else
		{
			data = IntToBCD(key_value-2000);
			DS1302_YearSet(data);
		}	
	}
	else if(command==2) 
	{
		key_value = atoi((char *)Data);
		if(key_value < 1 || key_value > 12)
		{
			Info_Window("Range Error", "Month Input Error");
		}
		else
		{
			data = IntToBCD(key_value);
			DS1302_MonthSet(data);
		}	
	}
	else if(command==3)
	{
		key_value = atoi((char *)Data);
		if(key_value < 1 || key_value > 31)
		{
			Info_Window("Range Error", "Day Input Error");
		}
		else
		{
			data = IntToBCD(key_value);
			DS1302_DaySet(data);
		}	
	}
	else if(command==4)
	{
		key_value = atoi((char *)Data);
		if(key_value < 0 || key_value > 24)
		{
			Info_Window("Range Error", "Hour Input Error");
		}
		else
		{
			data = IntToBCD(key_value);
			DS1302_HourSet(data);
		}	
	}
	else if(command==5)
	{
		key_value = atoi((char *)Data);
		if(key_value < 0 || key_value > 59)
		{
			Info_Window("Range Error", "Minute Input Error");
		}
		else
		{
			data = IntToBCD(key_value);
			DS1302_MinSet(data);
		}	
	}
	else if(command==6)
	{
		key_value = atoi((char *)Data);
		if(key_value < 0 || key_value > 59)
		{
			Info_Window("Range Error", "Second Input Error");
		}
		else
		{
			data = IntToBCD(key_value);
			DS1302_SecSet(data);
		}	
	}
	return(1);
}

//이함수는 위젯의 키이벤트를 받기위해 사용한다.
int	 Calendar_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
    
    if(win->ID==1)	//Year Set
    {
		SKeyPad(0, parent, NULL, (LCD_X-345) / 2 , (LCD_Y - 415) / 2, clBlack, "", "Year Input", win->ID, Calendar_Event);
    }
	else if(win->ID==2) //month
	{
		SKeyPad(0, parent, NULL, (LCD_X-345) / 2 , (LCD_Y - 415) / 2, clBlack, "", "Month Input", win->ID, Calendar_Event);		
	}
	else if(win->ID==3) //day
	{
		SKeyPad(0, parent, NULL, (LCD_X-345) / 2 , (LCD_Y - 415) / 2, clBlack, "", "Day Input", win->ID, Calendar_Event);		
	}
	else if(win->ID==4) //hour
	{
		SKeyPad(0, parent, NULL, (LCD_X-345) / 2 , (LCD_Y - 415) / 2, clBlack, "", "Hour Input", win->ID, Calendar_Event);		
	}
	else if(win->ID==5) //minute
	{
		SKeyPad(0, parent, NULL, (LCD_X-345) / 2 , (LCD_Y - 415) / 2, clBlack, "", "Minute Input", win->ID, Calendar_Event);		
	}
	else if(win->ID==6) //sec
	{
		SKeyPad(0, parent, NULL, (LCD_X-345) / 2 , (LCD_Y - 415) / 2, clBlack, "", "Second Input", win->ID, Calendar_Event);		
	}
	else if(win->ID==7)
	{
		DS1302InitSet();
	}
		
    else if(win->ID==30)
    {
	    StopTimer(play_timer);  	
		TimerQClear();
		CloseWindow(0, parent);
    }
	PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);    
	return(1);
}

int	Calendar_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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

    Real.sec = DS1302ReadByte(0x81) & 0x7f;
    Real.minute = DS1302ReadByte(0x83);
    Real.hour = DS1302ReadByte(0x85) & 0x3f;
    Real.date = DS1302ReadByte(0x87);
    Real.month = DS1302ReadByte(0x89);
    Real.year = DS1302ReadByte(0x8D);
	
	char	buf[128];
	sprintf(buf, "%4x.%02x.%02x  %02x:%02x:%02x", Real.year+0x2000, Real.month, Real.date, Real.hour, Real.minute, Real.sec);	
	SetWidgetText(time_textbox, buf);
    
	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	Calendar_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
	int currentTimeBackColor, currentTimeFontColor; 
	if(flash.systemColorNo!=6) {
		Gr->SetColor(clBlack);
		currentTimeBackColor=clBlack;
		currentTimeFontColor=clWhite;
	}
	else {
		Gr->SetColor(RGB565CONVERT(230,  230,  230));
		currentTimeBackColor=RGB565CONVERT(230,  230,  230);
		currentTimeFontColor=clBlack;
	}

	Gr->FillRect(0, 0, win->width-2, win->height-2);

	//타이틀바를 그린다.
	TitleBar(handle, 0, 0, win->width, 50, 1, "Calendar Set", clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	
	//디스플레이용 바탕패널을 그린다.
	//SystemPanel(handle, 20, 70, 800-40, 130, 1, "", clBlack, 1, RIGHT_ALIGN, 3, NULL);	
	
	//측정값 디스플레이용 텍스트박스
	SLabel(handle, 300, 80, 200, 30, "Current Time", currentTimeFontColor, currentTimeBackColor,  1);
	time_textbox = STextBox(handle, 60, 80+30, 680, 70, 48, "", clBlack, clWhite, CENTER_ALIGN, READ_ONLY, 0, NULL);	//RW_MODE

	//버튼을 만든다.
	year_btn = SButton(handle, 30, 220, 200, 64, "48G_Calendar_Icon1.bmp", "Year",  PUSH_BUTTON, 1, Calendar_KeyEvent);			
	mon_btn = SButton(handle, 30, 220+80, 200, 64, "48G_Calendar_Icon1.bmp", "Month",  PUSH_BUTTON, 2, Calendar_KeyEvent);			
	day_btn = SButton(handle, 30, 220+80+80, 200, 64, "48G_Calendar_Icon1.bmp", "Day",  PUSH_BUTTON, 3, Calendar_KeyEvent);			

	hour_btn = SButton(handle, 300, 220, 200, 64, "48G_Clock_Icon1.bmp", "Hour",  PUSH_BUTTON, 4, Calendar_KeyEvent);			
	min_btn = SButton(handle, 300, 220+80, 200, 64, "48G_Clock_Icon1.bmp", "Minute",  PUSH_BUTTON, 5, Calendar_KeyEvent);			
	sec_btn = SButton(handle, 300, 220+80+80, 200, 64, "48G_Clock_Icon1.bmp", "Second",  PUSH_BUTTON, 6, Calendar_KeyEvent);			

	init_btn = SButton(handle, 550, 220, 200, 64, "48G_Config_Icon1.bmp", "RTC Init",  PUSH_BUTTON, 7, Calendar_KeyEvent);			
	SLabel(handle, 550, 220+70, 200, 30, "2016/01/01  10:10", currentTimeFontColor, currentTimeBackColor,  0);
	
	//Close Button
	SButton(handle, 550, 220+80+80, 200, 64, "48G_LogOut_Icon1.bmp", "Close",  PUSH_BUTTON, 30, Calendar_KeyEvent);			
	
	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	play_timer =  OpenTimer(parent, handle, 500);	
    StartTimer(play_timer);  	
    
	return(1);
}

int	Calendar_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
	
	//Gr->SetColor(RGB565CONVERT(230,  230,  230));
	//Gr->FillRect(0, 0, win->width-2, win->height-2);
	
	PaintDrawControl(ON);		
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	Calendar_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
	int Done=1;
	while(Done) //위젯윈도우를 모두찾아 CloseEvent를 전송한다.
	{
		if(win->CloseEvent != NULL) win->CloseEvent(0, 0, 0, 0, 0);
		if(win->NextWindow ==NULL) return(NULL);
		win = (Windows *)win->NextWindow;
	}	
	return(1);
}

int Calendar_Window(void) {
	if(GetWindowName("Calendar_Window")) 
	{
		Dprintf("Calendar_Window:: Windows already Create\n");
		return 0;		
	}	
	int sx = (LCD_X - 800) / 2;
	int sy = (LCD_Y - 500) / 2;	
	makeWindow(
				LCD,					// Display Section Position
				"Calendar_Window",		// Windows Name
				DESKTOP_WINDOW,		// Parent Window Handle
				CONTAINER_WINDOW,	// Windows Kind
				STYLE_NONE,			// Windows Style, TEECO_STYLE
				sx, sy, 			// Start Position
				800, 500,				// Size
				SHOW,					// Create after Show
				Calendar_CreateEvent,		// CreateEvent
				Calendar_PaintEvent,		// PaintEvent
				NULL,	// MouseOnEvent
				NULL,	// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				Calendar_TimerEvent,		// TimerEvent
				NULL,  					// ControlEvent
				NULL,					// ChangeEvent
				NULL,
				NULL,
				NULL,
				Calendar_CloseEvent,
				NULL,					// PARAMETER
				NULL,					// Back Image
				NULL,					// 
				NULL,					// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	return(1);
}				

#ifdef __cplusplus
}
#endif
