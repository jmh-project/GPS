				
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
#include "DataLog.h"
/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/

#if defined (YSI_ENABLE)

void Delay(__IO uint32_t nTime);
volatile U32	gSensorTimer_id;
volatile U32	gDosScreen_id;
volatile U32 gTextOut[21];

/******************************************************************************
							Function Prototype

	SmartLogSensorChannelTimerEvent

	Dprintf("SmartLogStartTimerEvent::TimerEventid = %08x,%08x,%08x,%08x\n", parent,handle,command,param);
	sprintf(cbuf,"%7.3f",gv.ch2_pv2);
	SetWidgetText(gEdit2_id2, cbuf);
	한줄에 문자 50자가 들어간다.
******************************************************************************/
U32 MemoBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		backcolor, 
			int 	font_no,
			int		fontcolor
);


U32		YsiDosScreen_id;
int	 SmartLogSensorChannelTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	char		cbuf[DOS_SCREEN_BUF_SIZE], data[30];
	int			i = 0;
	int			k = 0;
	int			cnt=0;
	static int	lineCnt = 0;
    unsigned int length=45;
	
	
	Windows 	*win = GetWindow(parent, handle, "SmartLogSensorChannelPaintEvent");				//windows 관련 정보를 얻는다.
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	
	if(win==NULL) 											// 윈도우가 생성이 안된경우 
	{
		Dprintf("SmartLogSensorChannelPaintEvent::Windows Error\n");
		return(0);
	}
	if(Gr==NULL) 												// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("SmartLogSensorChannelPaintEvent:: Graphics Error\n");
		return(0);
	}
    
    //DS1302TimeRead();
    //sprintf(data,"20%02d/%02d/%02d %02d:%02d:%02d", RDate.year,RDate.month,RDate.date,RDate.hour, RDate.minute, RDate.sec);
    //SetWidgetText(gv.titleBarTimeId, data);//gv.titleBarTimeId
    
	for(k=0;k<DOS_SCREEN_BUF_SIZE;k++)cbuf[k]=0;
	if(gv.dosScreenFlag==ENABLE)//gv.dosScreenFlag 토글이 된다.
	{		
		gv.dosScreenFlag=DISABLE;
		cnt = 0;
		if(gv.dosCnt >= 1)//글자가 없으면 넘어간다.
		{
			for(i=0;i<gv.dosCnt;i++)//수신한 글자 수 만큼 출력한다. 
			{
				if(lineCnt >= length)//70자를 출력하고 문자열을 변경한다.
				{
					AddWidgetString(gDosScreen_id, cbuf);
					cnt = 0;
					lineCnt = 0;
					for(k=0;k<DOS_SCREEN_BUF_SIZE;k++)cbuf[k]=0;
					AddWidgetChar(gDosScreen_id, '\n');//줄바꿈명령
				}
				else if(gv.dosScreenData[i] == 0x0d)
				{
					AddWidgetString(gDosScreen_id, cbuf);
					cnt = 0;
					lineCnt = 0;
					for(k=0;k<DOS_SCREEN_BUF_SIZE;k++)cbuf[k]=0;
					AddWidgetChar(gDosScreen_id, '\n');//줄바꿈명령
				}
				else
				{
					lineCnt++;//한줄에 출력할 수 있는 폰트 수를 결정한다.
					cbuf[cnt++]=gv.dosScreenData[i];
                    cbuf[cnt]=0;
					//AddWidgetChar(gDosScreen_id, gv.dosScreenData[i]); //통신으로 수신한 데이터를 한문자를 출력한다.
				}
			}
			if(cnt>=1)//문자열의 길이가 70보다 작으면 여기서 출력한다.
				AddWidgetString(gDosScreen_id, cbuf);
			for(k=0;k<DOS_SCREEN_BUF_SIZE;k++) gv.dosScreenData[k]=0;
			gv.dosCnt=0;
		}
	}
	else if(gv.dosScreenFlag==DISABLE)
	{
		gv.dosScreenFlag=ENABLE;
		cnt = 0;
		if(gv.dosCnt2 >= 1) 
		{
			for(i=0;i<gv.dosCnt2;i++)
			{
				if(lineCnt >= length)//70자를 출력하고 문자열을 변경한다.
				{
					AddWidgetString(gDosScreen_id, cbuf);
					cnt = 0;
					lineCnt = 0;
					for(k=0;k<DOS_SCREEN_BUF_SIZE;k++)
						cbuf[k]=0;
					AddWidgetChar(gDosScreen_id, '\n');//줄바꿈명령
				}
				else if(gv.dosScreenData2[i] == 0x0d)//스트링 버퍼에 저장된 데이터를 엔터 명령을 만나면 출력한다.
				{
					AddWidgetString(gDosScreen_id, cbuf); //통신으로 수신한 데이터를 문자열로 출력한다.
					cnt=0;
					lineCnt = 0;
					for(k=0;k<DOS_SCREEN_BUF_SIZE;k++)
						cbuf[k]=0;
					AddWidgetChar(gDosScreen_id, '\n');//줄바꿈명령
				}
				else
				{
					lineCnt++;
					cbuf[cnt++]=gv.dosScreenData2[i];//수신한 데이터를 스트링 버퍼에 저장한다.
                    cbuf[cnt]=0;
					//gv.dosScreenData2[i]=0;
					//AddWidgetChar(gDosScreen_id, gv.dosScreenData2[i]); //통신으로 수신한 데이터를 한문자를 출력한다.
				}
			}
			if(cnt>=1)
				AddWidgetString(gDosScreen_id, cbuf); //현재 수신한 문자열의 길이가 70보다 작은 경우 여기서 출력한다.
			for(k=0;k<DOS_SCREEN_BUF_SIZE;k++)gv.dosScreenData2[k]=0;
			gv.dosCnt2=0;
		}
	}
	return 1;
}
int	 SmartLogSensorChannel_MouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			nCnt=0;
	Windows 	*win = GetWindow(parent, handle,"SmartLogSensorChannel_MouseOnEvent");

	//Dprintf("SmartLogStartTimerEvent::TimerEventid = %08x,%08x,%08x,%08x\n", parent,handle,command,param);
	//Dprintf("SmartLogConfig_Config_MouseOnEvent\n");
 	//Dprintf("SmartLogSensorChannel_MouseOnEvent :: BUTTON ID = %d\n",win->ID);
	if(win == NULL)
	{
		Dprintf("SmartLogSensorChannel_MouseOnEvent:: Windows Error\n");
		return(0);
	}
	switch(win->ID)
	{
		case KEY_A:
		{
			TransmitCharData('a', gv.nPara_UartChannel);
			Dprintf("A");
		}break;
		case KEY_B:
		{
			TransmitCharData('b', gv.nPara_UartChannel);
		}break;
		case KEY_C:
		{
			TransmitCharData('c', gv.nPara_UartChannel);
		}break;
		case KEY_D:
		{
			TransmitCharData('d', gv.nPara_UartChannel);
		}break;
		case KEY_E:
		{
			TransmitCharData('e', gv.nPara_UartChannel);
		}break;
		case KEY_F:
		{
			TransmitCharData('f', gv.nPara_UartChannel);
		}break;
		case KEY_G:
		{
			TransmitCharData('g', gv.nPara_UartChannel);
		}break;
		case KEY_H:
		{
			TransmitCharData('h', gv.nPara_UartChannel);
		}break;
		case KEY_I:
		{
			TransmitCharData('i', gv.nPara_UartChannel);
		}break;
		case KEY_J:
		{
			TransmitCharData('j', gv.nPara_UartChannel);
		}break;
		case KEY_K:
		{
			TransmitCharData('k', gv.nPara_UartChannel);
		}break;
		case KEY_L:
		{
			TransmitCharData('l', gv.nPara_UartChannel);
		}break;
		case KEY_M:
		{
			TransmitCharData('m', gv.nPara_UartChannel);
		}break;
		case KEY_N:
		{
			TransmitCharData('n', gv.nPara_UartChannel);
		}break;
		case KEY_O:
		{
			TransmitCharData('o', gv.nPara_UartChannel);
		}break;
		case KEY_P:
		{
			TransmitCharData('p', gv.nPara_UartChannel);
		}break;
		case KEY_Q:
		{
			TransmitCharData('q', gv.nPara_UartChannel);
		}break;
		case KEY_R:
		{
			TransmitCharData('r', gv.nPara_UartChannel);
		}break;
		case KEY_S:
		{
			TransmitCharData('s', gv.nPara_UartChannel);
		}break;
		case KEY_T:
		{
			TransmitCharData('t', gv.nPara_UartChannel);
		}break;
		case KEY_U:
		{
			TransmitCharData('u', gv.nPara_UartChannel);
		}break;
		case KEY_V:
		{
			TransmitCharData('v', gv.nPara_UartChannel);
		}break;
		case KEY_W:
		{
			TransmitCharData('w', gv.nPara_UartChannel);
		}break;
		case KEY_X:
		{
			TransmitCharData('x', gv.nPara_UartChannel);
		}break;
		case KEY_Y:
		{
			TransmitCharData('y', gv.nPara_UartChannel);
		}break;
		case KEY_Z:
		{
			TransmitCharData('z', gv.nPara_UartChannel);
		}break;
		case KEY_POINT:
		{
			TransmitCharData('.', gv.nPara_UartChannel);
		}break;
		case KEY__:
		{
			if(flash.Uart[USART_3]->SensorID == SENSOR_YSI)//위쯔 프로그램에서 동작하는 버튼이다.  
			{
				TransmitCharData('#', gv.nPara_UartChannel);
				TransmitCharData('S', gv.nPara_UartChannel);
				TransmitCharData(0x0d, gv.nPara_UartChannel);
				TransmitCharData(0x0a, gv.nPara_UartChannel);
			}
			if(flash.Uart[USART_1]->SensorID == SENSOR_YSI||flash.Uart[USART_2]->SensorID == SENSOR_YSI ||flash.Uart[USART_3]->SensorID == SENSOR_YSI)
			{
                TransmitCharData('/', gv.nPara_UartChannel);
			}
		}break;
		case KEY_1:
		{
			TransmitCharData('1', gv.nPara_UartChannel);
		}break;
		case KEY_2:
		{
			TransmitCharData('2', gv.nPara_UartChannel);
		}break;
		case KEY_3:
		{
			TransmitCharData('3', gv.nPara_UartChannel);
		}break;
		case KEY_4:
		{
			TransmitCharData('4', gv.nPara_UartChannel);
		}break;
		case KEY_5:
		{
			TransmitCharData('5', gv.nPara_UartChannel);
		}break;
		case KEY_6:
		{
			TransmitCharData('6', gv.nPara_UartChannel);
		}break;
		case KEY_7:
		{
			TransmitCharData('7', gv.nPara_UartChannel);
		}break;
		case KEY_8:
		{
			TransmitCharData('8', gv.nPara_UartChannel);
		}break;
		case KEY_9:
		{
			TransmitCharData('9', gv.nPara_UartChannel);
		}break;
		case KEY_0:
		{
			TransmitCharData('0', gv.nPara_UartChannel);
		}break;
		case KEY_PARA:
		{
			//gTimeTransmitFlag=0;
			gv.nFlag_Sensor_DataRead[gv.nPara_UartChannel]=ITEM_READ_MODE;//통신 프로토콜에서 ysi 측정 항목을 알아내는 모드로 넘어간다.
			
			for(nCnt=0;nCnt<400;nCnt++)
				gv.aItemReceiveBuffer[gv.nPara_UartChannel][nCnt]=0;//수신 버퍼 초기화 

			gv.nFlag_RecevieItemListRead=OFF;			
			gv.nPARA_Command_Start = ON;
			TransmitCharData(ESC, gv.nPara_UartChannel);
			TransmitCharData(ESC, gv.nPara_UartChannel);
			while(gv.nPARA_Command_Start==ON) {// 정해진 데이터를 수신대기 한다.
				
			}
			gv.nPARA_Command_Start = ON;

			TransmitCharData('p', gv.nPara_UartChannel);
			TransmitCharData('a', gv.nPara_UartChannel);
			TransmitCharData('r', gv.nPara_UartChannel);
			TransmitCharData('a', gv.nPara_UartChannel);
			TransmitCharData(ENTER, gv.nPara_UartChannel);

			while(gv.nPARA_Command_Start==ON) {
				
			}
			gv.nFlag_RecevieItemListRead=ON;//데이터 수집을 시작한다.
			gv.nFlag_Receive_Report_Start=OFF;
			Delay(100);

			fnSensor_PARAX_Command_Process( gv.nPara_UartChannel );//채널을 변경가능해야한다.
			
			gv.nReceiveItemNameBufCount=0;
			Delay(100);
			TransmitCharData(ENTER, gv.nPara_UartChannel);
			TransmitCharData('r', gv.nPara_UartChannel);
			TransmitCharData('u', gv.nPara_UartChannel);
			TransmitCharData('n', gv.nPara_UartChannel);
			TransmitCharData(ENTER, gv.nPara_UartChannel);
			gv.nFlag_Sensor_DataRead[gv.nPara_UartChannel]=DOS_MODE; // ysi 센서 설정모드로 넘어간다.
		}break;
        case KEY_RUN:
        {
			TransmitCharData('r', gv.nPara_UartChannel);
			TransmitCharData('u', gv.nPara_UartChannel);
			TransmitCharData('n', gv.nPara_UartChannel);
        }
        break;
		case KEY_ESC :
		{
			TransmitCharData(ESC, gv.nPara_UartChannel);
		}break;
		case KEY_ENT:
		{
			TransmitCharData(ENTER, gv.nPara_UartChannel);
		}break;
        case KEY_SPACE:
        {
            TransmitCharData(':', gv.nPara_UartChannel);
        }
        break;
		case KEY_HOME:
		{
			gv.nFlag_Sensor_DataRead[gv.nPara_UartChannel] = SCREEN_MODE;	//dos화면 출력을 정지한다.
            
            DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.dat");
            MessageQClear();	//기존에 저장된 터치 이벤트를 모두지운다.
            TimerQClear();
			StopTimer(gSensorTimer_id);
			CloseTimer(gSensorTimer_id);
			gv.OldScreen = 0;

			gv.tmsStatusCode[USART_1]=0;
			gv.tmsStatusCode[USART_2]=0;
			gv.tmsStatusCode[USART_3]=0;
			gv.tmsStatusCode[USART_4]=0;
			
            #ifdef MANUAL_SYSTEM_PROJECT
			if(7==MainScreenCreate(NULL, parent))
			{
				MainScreenCreate(NULL, parent);
			}
            #endif
		}break;
	}
	return 1;
}

int	 SmartLogSensorChannelMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//마우스의 좌표를 구하는방법
	int 		tx = LOWORD(param);
	int 		ty = HIWORD(param);	
	
	Dprintf("SmartLogSensorChannelMouseOnEvent:: X=%d Y=%d\n", tx, ty);
	return 0;
}

int	 SmartLogSensorChannelPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "SmartLogTwoChannelPaintEvent");				//windows 관련 정보를 얻는다.
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL) 											// 윈도우가 생성이 안된경우 
	{
		Dprintf("SmartLogTwoChannelPaintEvent::Windows Error\n");
		return(0);
	}
	if(Gr==NULL) 												// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("SmartLogTwoChannelPaintEvent:: Graphics Error\n");
		return(0);
	}	
	Gr->FillRect(0, 0, 0, 0);										// dummy code, 아직버그는 발견못했음, 초기에 반드시 해주어야한다.
#ifdef DATALOG_8INCH    
	gDosScreen_id = MemoBox(handle, 15, 54, 642, 694, BLACK, DOS_SCREEN_FONT, YELLOW);//DOS 화면을 만든다.
#endif    
#ifdef DATALOG_7INCH
    gDosScreen_id = MemoBox(handle, 15, 50, 577-15 + 80, 583-55, BLACK, DOS_SCREEN_FONT, YELLOW);
#endif    
	gv.dosScreenFlag = ENABLE;
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
/******************************************************************************
	SmartLogStartCreateEvent
	설명 :  센서 접속 여부를 판단한다.
******************************************************************************/
//U32 MemoryButton(HANDLE ContainerHandle, int sx, int sy, int width, int height, int NormalPage, int TouchPage, int function, int id, int (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *))
//int FileButton(HANDLE ContainerHandle, int sx, int sy, int width, int height, char *NormalName, char *TouchName, int (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *))
//MemoryButton(handle, 563, 287, 165, 164, USER_SCREEN1, USER_SCREEN2, PUSH_BUTTON, 1, SmartXSelfKeyEvent); 

#ifdef DATALOG_7INCH
int	 SmartLogSensorChannelCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	char	cbuf[30];
	//int		backColor=WHITE;
	//int		fontColor=BLUE;
	//int		fontType=FONT16;
	//int		textBoxType = READ_ONLY;
	//int		fontPoint=CENTER_ALIGN;
	//static int	onePlay=0;	

	//U32		UserScreen = USER_SCREEN5;
	//U32		UserScreen_Touch = USER_SCREEN6;
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(Gr==NULL) 												// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("SmartLogTwoChannelPaintEvent:: Graphics Error\n");
		return(0);
	}	

	//gTimeTransmitFlag=0;//시간설정시에 사용한다.
	
	PaintDrawControl(OFF);//Create Event 발생 후에 Paint Event를 발생하게 한다.	
	/*
    if(onePlay==0)
	{
		BmpLoad(UserScreen, 0, 0, "SmartLog_Sensor.bmp");// image
		BmpLoad(UserScreen_Touch, 0, 0, "SmartLog_Sensor_t.bmp");// Touch image
		onePlay=1;
	}
	Gr->BitBlt(UserScreen, 0, 0, 0, 0, 800, 600);
	*/
	

/**********************
//채널 설정을 하지 않은 경우에는 실행 되지 않는다.

gv.NextScreenCreate == 1 이면 -1 하여 Uart1 채널이 된다.
gv.NextScreenCreate == 2 이면 -1 하여 Uart1 채널이 된다.
gv.NextScreenCreate == 3 이면 -1 하여 Uart1 채널이 된다.
gv.NextScreenCreate == 4 이면 -1 하여 Uart1 채널이 된다.
gv.NextScreenCreate == 5 이면 -1 하여 Uart1 채널이 된다.

**********************/
	 //도스화면에 출력할 데이터 통신 포트를 설정한다.
	
	if(gv.NextScreenCreate == 2)//Uart1 
	{
		gv.nPara_UartChannel = USART_1;
	}
	if(gv.NextScreenCreate == 3)//Uart2 
	{
		gv.nPara_UartChannel = USART_2;
	}
	if(gv.NextScreenCreate == 4)//Uart3 
	{
		gv.nPara_UartChannel = USART_3;
	}
	if(gv.NextScreenCreate == 5)//Uart4 
	{
		gv.nPara_UartChannel = USART_4;
	}
	if(gv.NextScreenCreate == 6)//Uart5 
	{
		gv.nPara_UartChannel = USART_5;
	}
	gv.nFlag_Sensor_DataRead[gv.nPara_UartChannel]=DOS_MODE; //도스 화면에 데이터를 출력한다.
	Dprintf("gv.nPara_UartChannel = %d\n",gv.nPara_UartChannel);

	
    //TitleBar_Create( handle, gv.nPara_UartChannel );//타이틀바에 들어갈 내용을 출력한다.
    //YsiDosScreen_id = MemoBox(handle, 22, 57, 570-22, 574-57, BLACK, 2, YELLOW, SCROLL_MODE);

// config 구조체를 읽어 구성 정보를 설정한다.
    
    unsigned int groupX = 0;
    unsigned int groupY = 0;
    unsigned int groupW = 1024;
    unsigned int groupH = 600;
    GroupPanel(handle, groupX, groupY, groupW, groupH , "YSI CALIBRATION");
    DS1302TimeRead();
    sprintf(cbuf,"20%02d/%02d/%02d %02d:%02d:%02d", RDate.year,RDate.month,RDate.date,RDate.hour, RDate.minute, RDate.sec);
    gv.titleBarTimeId = SLabel(handle, 1024-230, 1, 220, 37,cbuf,clBlack,RGB565CONVERT(191, 191, 191),1);

    unsigned int buttonSpace = 6;
    unsigned int startX = 598 + 80;

    unsigned int buttonX = startX;
    unsigned int buttonY = 44;
    unsigned int buttonW = 80;
    unsigned int buttonH = 44;
    
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"A",PUSH_BUTTON,KEY_A,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"B",PUSH_BUTTON,KEY_B,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"C",PUSH_BUTTON,KEY_C,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"D",PUSH_BUTTON,KEY_D,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"E",PUSH_BUTTON,KEY_E,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"F",PUSH_BUTTON,KEY_F,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"G",PUSH_BUTTON,KEY_G,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"H",PUSH_BUTTON,KEY_H,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"I",PUSH_BUTTON,KEY_I,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"J",PUSH_BUTTON,KEY_J,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"K",PUSH_BUTTON,KEY_K,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"L",PUSH_BUTTON,KEY_L,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"M",PUSH_BUTTON,KEY_M,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"N",PUSH_BUTTON,KEY_N,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"O",PUSH_BUTTON,KEY_O,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"P",PUSH_BUTTON,KEY_P,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"Q",PUSH_BUTTON,KEY_Q,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"R",PUSH_BUTTON,KEY_R,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"S",PUSH_BUTTON,KEY_S,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"T",PUSH_BUTTON,KEY_T,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"U",PUSH_BUTTON,KEY_U,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"V",PUSH_BUTTON,KEY_V,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"W",PUSH_BUTTON,KEY_W,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"X",PUSH_BUTTON,KEY_X,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"Y",PUSH_BUTTON,KEY_Y,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"Z",PUSH_BUTTON,KEY_Z,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,".",PUSH_BUTTON,KEY_POINT,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"/",PUSH_BUTTON,KEY__,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"1",PUSH_BUTTON,KEY_1,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"2",PUSH_BUTTON,KEY_2,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"3",PUSH_BUTTON,KEY_3,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"4",PUSH_BUTTON,KEY_4,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"5",PUSH_BUTTON,KEY_5,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"6",PUSH_BUTTON,KEY_6,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"7",PUSH_BUTTON,KEY_7,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"8",PUSH_BUTTON,KEY_8,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"9",PUSH_BUTTON,KEY_9,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"0",PUSH_BUTTON,KEY_0,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,":",PUSH_BUTTON,KEY_SPACE,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"PARA",PUSH_BUTTON,KEY_PARA,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"ESC",PUSH_BUTTON,KEY_ESC,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"RUN",PUSH_BUTTON,KEY_RUN,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"ENTER",PUSH_BUTTON,KEY_ENT,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"HOME",PUSH_BUTTON,KEY_HOME,SmartLogSensorChannel_MouseOnEvent);

    //측정치 업데이트 주기를 설정한다.
	gSensorTimer_id = OpenTimer(parent, handle, 100);
	StartTimer(gSensorTimer_id);
	PaintDrawControl(ON);//Create Event 발생 후에 Paint Event를 발생하게 한다.	
	return(1);
}
#endif //SmartLogSensorChannelCreateEvent
#ifdef DATALOG_8INCH
int	 SmartLogSensorChannelCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	char	cbuf[30];
	//int		backColor=WHITE;
	//int		fontColor=BLUE;
	//int		fontType=FONT16;
	//int		textBoxType = READ_ONLY;
	//int		fontPoint=CENTER_ALIGN;
	//static int	onePlay=0;	

	//U32		UserScreen = USER_SCREEN5;
	//U32		UserScreen_Touch = USER_SCREEN6;
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(Gr==NULL) 												// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("SmartLogTwoChannelPaintEvent:: Graphics Error\n");
		return(0);
	}	
    gv.tmsStatusCode[USART_1]=3;
    gv.tmsStatusCode[USART_2]=3;
    gv.tmsStatusCode[USART_3]=3;
    gv.tmsStatusCode[USART_4]=3;

	//gTimeTransmitFlag=0;//시간설정시에 사용한다.
	
	PaintDrawControl(OFF);//Create Event 발생 후에 Paint Event를 발생하게 한다.	

	Gr->FillRect(0, 0, 0, 0);//Dummy
	//투명위젯을 사용하려면 Paint Event에서 할배경작업을 미리해야한다. 그림이 필요하면 그림먼저 로딩할것(*********************** 중요 *********)
	//또는 PPaintDrawControl(ON); ->Gr->Draw를 실행후 사용한다.
	Gr->SetColor(RGB565CONVERT(200,  200,  200));
	Gr->FillRect(0, 0, LCD_X, LCD_Y);


	/*
    if(onePlay==0)
	{
		BmpLoad(UserScreen, 0, 0, "SmartLog_Sensor.bmp");// image
		BmpLoad(UserScreen_Touch, 0, 0, "SmartLog_Sensor_t.bmp");// Touch image
		onePlay=1;
	}
	Gr->BitBlt(UserScreen, 0, 0, 0, 0, 800, 600);
	*/
	

/**********************
//채널 설정을 하지 않은 경우에는 실행 되지 않는다.

gv.NextScreenCreate == 1 이면 -1 하여 Uart1 채널이 된다.
gv.NextScreenCreate == 2 이면 -1 하여 Uart1 채널이 된다.
gv.NextScreenCreate == 3 이면 -1 하여 Uart1 채널이 된다.
gv.NextScreenCreate == 4 이면 -1 하여 Uart1 채널이 된다.
gv.NextScreenCreate == 5 이면 -1 하여 Uart1 채널이 된다.

**********************/
	 //도스화면에 출력할 데이터 통신 포트를 설정한다.
	
	if(gv.NextScreenCreate == 2)//Uart1 
	{
		gv.nPara_UartChannel = USART_1;
	}
	if(gv.NextScreenCreate == 3)//Uart2 
	{
		gv.nPara_UartChannel = USART_2;
	}
	if(gv.NextScreenCreate == 4)//Uart3 
	{
		gv.nPara_UartChannel = USART_3;
	}
	if(gv.NextScreenCreate == 5)//Uart4 
	{
		gv.nPara_UartChannel = USART_4;
	}
	if(gv.NextScreenCreate == 6)//Uart5 
	{
		gv.nPara_UartChannel = USART_5;
	}
	gv.nFlag_Sensor_DataRead[gv.nPara_UartChannel]=DOS_MODE; //도스 화면에 데이터를 출력한다.
	Dprintf("gv.nPara_UartChannel = %d\n",gv.nPara_UartChannel);

	
    //TitleBar_Create( handle, gv.nPara_UartChannel );//타이틀바에 들어갈 내용을 출력한다.
    //YsiDosScreen_id = MemoBox(handle, 22, 57, 570-22, 574-57, BLACK, 2, YELLOW, SCROLL_MODE);

// config 구조체를 읽어 구성 정보를 설정한다.
    
    //unsigned int groupX = 0;
    //unsigned int groupY = 0;
    //unsigned int groupW = 1024;
    //unsigned int groupH = 768;
    char    pBuf[30];
	for( int i; i<30; i++) {
		pBuf[i] = 0;
	}
    //NGroupPanel(handle, groupX, groupY, groupW, groupH , "YSI CALIBRATION");
    
	TitleBar(handle, 0, 0, LCD_X, 50, 1, "YSI CALIBRATION", clYellow, 1, RIGHT_ALIGN, 3, NULL);	
    DS1302TimeRead();
    //sprintf(cbuf,"20%02d/%02d/%02d %02d:%02d:%02d", RDate.year,RDate.month,RDate.date,RDate.hour, RDate.minute, RDate.sec);
    //gv.titleBarTimeId = SLabel(handle, 1024-230, 5, 220, 40,cbuf,clBlack,RGB565CONVERT(230, 230, 230),1);
    gv.titleBarLogId = SLabel(handle, (1024-345) / 2, 5, 345, 40,gv.titleBuf,clBlack,RGB565CONVERT(230, 230, 230),1);
    unsigned int buttonSpace = 6;
    unsigned int startX = 598 + 80;

    unsigned int buttonX = startX;
    unsigned int buttonY = 54;
    unsigned int buttonW = 80;
    unsigned int buttonH = 58;
    
	#if 1
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"A",PUSH_BUTTON,KEY_A,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"B",PUSH_BUTTON,KEY_B,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"C",PUSH_BUTTON,KEY_C,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"D",PUSH_BUTTON,KEY_D,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"E",PUSH_BUTTON,KEY_E,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"F",PUSH_BUTTON,KEY_F,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"G",PUSH_BUTTON,KEY_G,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"H",PUSH_BUTTON,KEY_H,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"I",PUSH_BUTTON,KEY_I,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"J",PUSH_BUTTON,KEY_J,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"K",PUSH_BUTTON,KEY_K,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"L",PUSH_BUTTON,KEY_L,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"M",PUSH_BUTTON,KEY_M,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"N",PUSH_BUTTON,KEY_N,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"O",PUSH_BUTTON,KEY_O,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"P",PUSH_BUTTON,KEY_P,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"Q",PUSH_BUTTON,KEY_Q,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"R",PUSH_BUTTON,KEY_R,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"S",PUSH_BUTTON,KEY_S,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"T",PUSH_BUTTON,KEY_T,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"U",PUSH_BUTTON,KEY_U,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"V",PUSH_BUTTON,KEY_V,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"W",PUSH_BUTTON,KEY_W,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"X",PUSH_BUTTON,KEY_X,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"Y",PUSH_BUTTON,KEY_Y,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"Z",PUSH_BUTTON,KEY_Z,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,".",PUSH_BUTTON,KEY_POINT,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"/",PUSH_BUTTON,KEY__,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"1",PUSH_BUTTON,KEY_1,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"2",PUSH_BUTTON,KEY_2,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"3",PUSH_BUTTON,KEY_3,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"4",PUSH_BUTTON,KEY_4,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"5",PUSH_BUTTON,KEY_5,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"6",PUSH_BUTTON,KEY_6,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"7",PUSH_BUTTON,KEY_7,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"8",PUSH_BUTTON,KEY_8,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"9",PUSH_BUTTON,KEY_9,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"0",PUSH_BUTTON,KEY_0,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,":",PUSH_BUTTON,KEY_SPACE,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"PARA",PUSH_BUTTON,KEY_PARA,SmartLogSensorChannel_MouseOnEvent);

    buttonX = startX;
    buttonY = buttonY + buttonH + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"ESC",PUSH_BUTTON,KEY_ESC,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"RUN",PUSH_BUTTON,KEY_RUN,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"ENTER",PUSH_BUTTON,KEY_ENT,SmartLogSensorChannel_MouseOnEvent);
    buttonX = buttonX + buttonW + buttonSpace;
    NButton(handle,buttonX,buttonY,buttonW,buttonH,NULL,"HOME",PUSH_BUTTON,KEY_HOME,SmartLogSensorChannel_MouseOnEvent);
	#endif //BUTTON IMAGE CLOSE
	PaintDrawControl(ON);//Create Event 발생 후에 Paint Event를 발생하게 한다.	
	Gr->Draw(parent, handle, command, param, Data);
    //측정치 업데이트 주기를 설정한다.
	gSensorTimer_id = OpenTimer(parent, handle, 10);
	StartTimer(gSensorTimer_id);
	
	return(1);
}
#endif //SmartLogSensorChannelCreateEvent
#define		WINDOW_WIDTH		LCD_X
#define		WINDOW_HEIGHT		LCD_Y

int SmartLogSensorChannelFunction(HANDLE parent, HANDLE handle)
{
	if(GetWindowName("SmartLogSensorChannel")) 
	{
		Dprintf("SmartLogSensorChannel:: Windows already Create\n");
		return 0;		
	}	
	MessageQClear();	//기존에 저장된 터치 이벤트를 모두지운다.
    TimerQClear();
	
	makeWindow
	(
		LCD,
		"SmartLogSensorChannel",		// Windows Name
		DESKTOP_WINDOW,			// Parent Window Handle
		CONTAINER_WINDOW,		// Windows Kind
		STYLE_NONE,				// Windows Style
		0, 0, 					// Start Position
		LCD_X, LCD_Y,				// Size
		SHOW,					// Create after Show
		SmartLogSensorChannelCreateEvent,	// CreateEvent
		SmartLogSensorChannelPaintEvent,	// PaintEvent
		SmartLogSensorChannelMouseOnEvent,	// MouseOnEvent
		NULL,					// MouseOffEvent
		NULL,					// MouseMoveEvent
		NULL,					// GestureEvent
		NULL,					// ClickEvent
		SmartLogSensorChannelTimerEvent,	// TimerEvent
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
		Dprintf(" SmartLogSensorChannel :: MainWindows Create \n");
	}
	else 
	{
		//Dprintf("CloseWindows parent = %d handle = %d\n", parent, handle);
		CloseWindow(parent, handle);	
	}
	return(1);
	
}

#endif

