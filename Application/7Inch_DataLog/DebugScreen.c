				
/******************************************************************************
*    	File name	: SmartLogStart.c 
*		Description : DongMun RH100 Control Program
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: 
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"
extern void SD_UnMount(void);
/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/

#if defined (YSI_ENABLE)
extern volatile U32		gSensorTimer_id;
extern volatile U32 	gTextOut[21];
extern U32				YsiDosScreen_id;
#else
volatile U32			gSensorTimer_id;
volatile U32 			gTextOut[21];
U32						YsiDosScreen_id;
#endif
int TransmitStringData( char *font, char channel) {
    if(channel==0)
        SENSOR_UART1_STR( font );//센서로 데이터를 송신한다.
    else if(channel==1)
        SENSOR_UART2_STR( font );//센서로 데이터를 송신한다.
    else if(channel==2)
        SENSOR_UART3_STR( font );//센서로 데이터를 송신한다.
    else if(channel==3)
        SENSOR_UART4_STR( font );//센서로 데이터를 송신한다.
    else if(channel==4)
        SENSOR_UART5_STR( font );//센서로 데이터를 송신한다.
    else if(channel==5)
        SENSOR_UART6_STR( font );//센서로 데이터를 송신한다.
    else if(channel==6)
        SENSOR_UART7_STR( font );//센서로 데이터를 송신한다.
    else if(channel==7)
        SENSOR_UART8_STR( font );//센서로 데이터를 송신한다.
    return 0;
}

#if defined (YSI_ENABLE)

#else 

void Delay(__IO uint32_t nTime);
volatile U32		gDosScreen_id1;
volatile U32		gDosScreen_id2;
volatile int    	gStartIdToggle;
int     			gLineCount;            

/******************************************************************************
							Function Prototype

	DebugScreenTimerEvent

	Dprintf("SmartLogStartTimerEvent::TimerEventid = %08x,%08x,%08x,%08x\n", parent,handle,command,param);
	sprintf(cbuf,"%7.3f",gv.ch2_pv2);
	SetWidgetText(gEdit2_id2, cbuf);
	한줄에 문자 50자가 들어간다.
******************************************************************************/
extern U32 MemoBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		backcolor, 
			int 	font_no,
			int		fontcolor
);


int	 DebugScreenTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
    char		cbuf[DOS_SCREEN_BUF_SIZE];
    int			cnt=0;


    Windows 	*win = GetWindow(parent, handle, "DebugScreenPaintEvent");				//windows 관련 정보를 얻는다.
    Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

    if(win==NULL) 											// 윈도우가 생성이 안된경우 
    {
        Dprintf("DebugScreenPaintEvent::Windows Error\n");
        return(0);
    }
    if(Gr==NULL) 												// 그래픽 할당을 못 받은 경우  
    {
        Dprintf("DebugScreenPaintEvent:: Graphics Error\n");
        return(0);
    }

    //DS1302TimeRead();
    char    buf[128];
    for(int i=0; i<128; i++) {
        buf[i]=0;
    }
//	U32 address = SDREAD_RAM; //저장된 데이터
//	char logBuf[1000];
//	for(int i=0; i<1000; i++) {
//		logBuf[i]=(*(U8 *)address++);
//	}
//	DebugMessagePrintf(logBuf);
//	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
    sprintf(buf,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
    SetWidgetText(gv.titleBarTimeId, buf);//시간출력
    if(gv.dosScreenFlag==ENABLE) {//gv.dosScreenFlag 토글이 된다.
        //gv.dosScreenFlag=DISABLE;
        cnt = 0;
        if(gv.dosCnt >= 1)//수신한 데이터의 문자열 갯수 Ysi_ReadData_Dos 에서 증가한다.
        {
            for(int i=0;i<gv.dosCnt;i++)//수신한 글자 수 만큼 출력한다. 
            {
                if(cnt == 45)//32 출력하고 그 이상이면 줄바꿈 명령을 실행한다.
                {
                    if(gStartIdToggle==0) {
                        cbuf[++cnt]=gv.dosScreenData[i];
                        cbuf[++cnt]=gv.dosScreenData[i];
                        cbuf[++cnt]=0;
                        AddWidgetString(gDosScreen_id1, cbuf);
                        AddWidgetChar(gDosScreen_id1,0x0A);
                    } 
                    else {
                        cbuf[++cnt]=gv.dosScreenData[i];
                        cbuf[++cnt]=gv.dosScreenData[i];
                        cbuf[++cnt]=0;
                        AddWidgetString(gDosScreen_id2, cbuf);
                        AddWidgetChar(gDosScreen_id2,0x0A);
                    }
                    cnt = 0;
                }
                else if(gv.dosScreenData[i] == 0x0A) //줄바꿈 명령이 있는 경우 바로 실행한다.
                {     
                    if(gStartIdToggle==0) {
                        AddWidgetString(gDosScreen_id1, cbuf);
                        AddWidgetChar(gDosScreen_id1,0x0A);
                    } 
                    else {
                        AddWidgetString(gDosScreen_id2, cbuf);
                        AddWidgetChar(gDosScreen_id2,0x0A);
                    }
                    cnt = 0;
                }
                else
                {
                    cbuf[cnt++]=gv.dosScreenData[i];
                    cbuf[cnt]=0;
                }
            }
            //for(int k=0;k<DOS_SCREEN_BUF_SIZE;k++) gv.dosScreenData[k]=0;
            gv.dosCnt=0;
        }
    }
    else if(gv.dosScreenFlag==DISABLE) {
        //gv.dosScreenFlag=ENABLE;
        cnt = 0;
        if(gv.dosCnt2 >= 1) 
        {
            for(int i=0;i<gv.dosCnt2;i++)
            {
                if(cnt == 45)
                {
                    if(gStartIdToggle==0) {
                        cbuf[++cnt]=gv.dosScreenData[i];
                        cbuf[++cnt]=gv.dosScreenData[i];
                        cbuf[++cnt]=0;
                        AddWidgetString(gDosScreen_id1, cbuf);
                        AddWidgetChar(gDosScreen_id1,0x0A);
                    } 
                    else {
                        cbuf[++cnt]=gv.dosScreenData[i];
                        cbuf[++cnt]=gv.dosScreenData[i];
                        cbuf[++cnt]=0;
                        AddWidgetString(gDosScreen_id2, cbuf);
                        AddWidgetChar(gDosScreen_id2,0x0A);
                    }
                    cnt = 0;
                    gLineCount++;
                }
                else if(gv.dosScreenData2[i] == 0x0A)//스트링 버퍼에 저장된 데이터를 엔터 명령을 만나면 출력한다.
                {
                    if(gStartIdToggle==0) {
                        AddWidgetString(gDosScreen_id1, cbuf);
                        AddWidgetChar(gDosScreen_id1,0x0A);
                    } 
                    else {
                        AddWidgetString(gDosScreen_id2, cbuf);
                        AddWidgetChar(gDosScreen_id2,0x0A);
                    }
                    cnt=0;
                    gLineCount++;
                }
                else
                {
                    cbuf[cnt++]=gv.dosScreenData2[i];//수신한 데이터를 스트링 버퍼에 저장한다.
                    cbuf[cnt]=0;
                }
            }
            if(gLineCount>=28) {
                if(gStartIdToggle==0) {
                    gStartIdToggle=1;
                    AddWidgetClear(gDosScreen_id2);
                } 
                else {
                    gStartIdToggle=0;
                    AddWidgetClear(gDosScreen_id1);
                }
                gLineCount=0;
            }
            //for(int k=0;k<DOS_SCREEN_BUF_SIZE;k++)gv.dosScreenData2[k]=0;
            gv.dosCnt2=0;
        }
    }

    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
    return 1;
}
int	 DebugScreen_MouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
    Windows *win = GetWindow(parent, handle,"DebugScreen_MouseOnEvent");

    if(win == NULL)
    {
        Dprintf("DebugScreen_MouseOnEvent:: Windows Error\n");
        return(0);
    }
	if(win->ID==10) {
		gv.debugFlag=0;
		Dprintf("DebugScreenFunction :: Close\n");		
		MessageQClear();	//기존에 저장된 터치 이벤트를 모두지운다.
		TimerQClear();
		StopTimer(gSensorTimer_id);
		CloseTimer(gSensorTimer_id);
		gv.OldScreen = 0;
		gv.tmsStatusCode[USART_1]=0;
		gv.tmsStatusCode[USART_2]=0;
		gv.tmsStatusCode[USART_3]=0;
		gv.tmsStatusCode[USART_4]=0;
		
		#if defined(TYPE_APPROVAL_ENABLE)
		PasswordInit();
		#endif
		DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
		#ifdef MANUAL_SYSTEM_PROJECT
		{
			gv.NewScreen = 0;//출력화면이 하나 인데 NEXT 버튼을 누르면 같은 화면이 중복 생성되는 에러를 막아준다.
			gv.OldScreen = 0;
//			gv.tmsStatusCode[USART_1]=0;
//			gv.tmsStatusCode[USART_2]=0;
//			gv.tmsStatusCode[USART_3]=0;
//			gv.tmsStatusCode[USART_4]=0;
			gv.NextScreenCreate=1;//첫번째 화면이 출력 된게한다. 첫번째 화면이란 USART 1의 센서가 연결되어있을때 화면이다.
			if(7==MainScreenCreate(NULL, parent))
			{
				MainScreenCreate(NULL, parent);
			}
		}
		#endif
    }
    return 1;
}

int	 DebugScreenMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
    //마우스의 좌표를 구하는방법
    int 		tx = LOWORD(param);
    int 		ty = HIWORD(param);	
    Dprintf("DebugScreenMouseOnEvent:: X=%d Y=%d\n", tx, ty);
    return 0;
}
int	 DebugScreenPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
    Windows 	*win = GetWindow(parent, handle, "SmartLogTwoChannelPaintEvent");				//windows 관련 정보를 얻는다.
    Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
    if(win==NULL) // 윈도우가 생성이 안된경우 
    {
        Dprintf("SmartLogTwoChannelPaintEvent::Windows Error\n");
        return(0);
    }
    if(Gr==NULL) // 그래픽 할당을 못 받은 경우  
    {
        Dprintf("SmartLogTwoChannelPaintEvent:: Graphics Error\n");
        return(0);
    }	
    Gr->FillRect(0, 0, 0, 0); // dummy code, 아직버그는 발견못했음, 초기에 반드시 해주어야한다.
    gDosScreen_id1 = MemoBox(handle, 10, 60, (1004/2)-5, 694, BLACK, DOS_SCREEN_FONT, YELLOW);//DOS 화면을 만든다.
    AddWidgetClear(gDosScreen_id1);

    gDosScreen_id2 = MemoBox(handle, (1004/2)+10, 60, (1004/2), 694, BLACK, DOS_SCREEN_FONT, YELLOW);//DOS 화면을 만든다.
    AddWidgetClear(gDosScreen_id2);
	
//	DebugMessagePrintf("SDCard file list.\n");

	char path[128];
	SD_Mount();
	sprintf(path,"%s/", Disk_Kind);	//!< 현재의 디렉토리를 표시하는방법 dir/
	scan_files(path);
	//FileView("DumpTransmitFile2204.txt");
	SD_UnMount();

	
    gv.dosScreenFlag = 0;
    Gr->Draw(parent, handle, command, param, Data);	
    return(1);
}
int	 DebugScreenCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
    Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
    if(Gr==NULL)    // 그래픽 할당을 못 받은 경우  
    {
        Dprintf("SmartLogTwoChannelPaintEvent:: Graphics Error\n");
        return(0);
    }
	Dprintf("DebugScreenCreateEvent :: 디버그 메뉴에서는 교정중 명령이 실행안된다.\n");
	
//    gv.tmsStatusCode[USART_1]=3;
//    gv.tmsStatusCode[USART_2]=3;
//    gv.tmsStatusCode[USART_3]=3;
//    gv.tmsStatusCode[USART_4]=3;

    if(flash.Uart[USART_1]->SensorID == SENSOR_DEBUG) {
        gv.nFlag_Sensor_DataRead[USART_1]=DOS_MODE;
    }
    if(flash.Uart[USART_2]->SensorID == SENSOR_DEBUG) {
        gv.nFlag_Sensor_DataRead[USART_2]=DOS_MODE;
    }
    if(flash.Uart[USART_3]->SensorID == SENSOR_DEBUG) {
        gv.nFlag_Sensor_DataRead[USART_3]=DOS_MODE;
    }
    if(flash.Uart[USART_4]->SensorID == SENSOR_DEBUG) {
        gv.nFlag_Sensor_DataRead[USART_4]=DOS_MODE;
    }

    PaintDrawControl(OFF);//Create Event 발생 후에 Paint Event를 발생하게 한다.	

    Gr->FillRect(0, 0, 0, 0);//Dummy
    Gr->SetColor(RGB565CONVERT(36,  36,  36));
    Gr->FillRect(0, 0, LCD_X, LCD_Y);

     //도스화면에 출력할 데이터 통신 포트를 설정한다.

    if(gv.NextScreenCreate == 2)//Uart1 
    {
        gv.nPara_UartChannel = USART_1;
    }

    TitleBar_MenuButton_Create(handle,12,DebugScreen_MouseOnEvent);
    gv.handle2[15]=NButton(handle, 2, 0, 300-2, 60, "48G_Home_Icon2.bmp", "HOME",  PUSH_BUTTON, 10, DebugScreen_MouseOnEvent);     
      
    PaintDrawControl(ON);//Create Event 발생 후에 Paint Event를 발생하게 한다.	
    Gr->Draw(parent, handle, command, param, Data);
    //측정치 업데이트 주기를 설정한다.
    gSensorTimer_id = OpenTimer(parent, handle, 10);
    StartTimer(gSensorTimer_id);
    gv.debugFlag=1;
    return(1);
}
#define		WINDOW_WIDTH		LCD_X
#define		WINDOW_HEIGHT		LCD_Y
int DebugScreenFunction(HANDLE parent, HANDLE handle) {
    Dprintf("DebugScreenFunction :: Create\n");
	if(GetWindowName("DebugScreen")) 
	{
		Dprintf("DebugScreen:: Windows already Create\n");
		return 0;		
	}	
	MessageQClear();	//기존에 저장된 터치 이벤트를 모두지운다.
    TimerQClear();
	
	makeWindow
	(
		LCD,
		"DebugScreen",		// Windows Name
		DESKTOP_WINDOW,			// Parent Window Handle
		CONTAINER_WINDOW,		// Windows Kind
		STYLE_NONE,				// Windows Style
		0, 0, 					// Start Position
		LCD_X, LCD_Y,				// Size
		SHOW,					// Create after Show
		DebugScreenCreateEvent,	// CreateEvent
		DebugScreenPaintEvent,	// PaintEvent
		DebugScreenMouseOnEvent,	// MouseOnEvent
		NULL,					// MouseOffEvent
		NULL,					// MouseMoveEvent
		NULL,					// GestureEvent
		NULL,					// ClickEvent
		DebugScreenTimerEvent,	// TimerEvent
		NULL,  					// ControlEvent
		NULL,  					// ChangeEvent
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,					// PARAMETER
		0,						// Normal Touch Screen
		0,						// Touch Screen
		"MainWindow",			// Windows Text
		MODALLESS_WINDOW,
		NULL
	);
	//Dprintf("SmartXMain::Handle=%d\n", mainHandle);	
	if(parent==NULL && handle==NULL)//백그라운드 윈도우를 삭제한다.
	{
		Dprintf(" DebugScreen :: MainWindows Create \n");
	}
	else 
	{
		//Dprintf("CloseWindows parent = %d handle = %d\n", parent, handle);
		CloseWindow(parent, handle);	
	}
	return(1);	
}
#endif
