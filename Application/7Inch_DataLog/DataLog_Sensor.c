/******************************************************************************
*       File name	: Sensor_Main.c 
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
    U32     gOldID = 0, gOldID2 = 0;
unsigned int       gInputCalChannel = 0;
unsigned int       gAD420CalChannel = 0;
unsigned int       gInputCalStart = OFF, gOutputCalStart = OFF;

/******************************************************************************
							Function Prototype
******************************************************************************/
int Info_Window(char *title, char *text);
U32 GroupPanel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text);
int DataLog_UartRateWindow(int 	sx, 
							int 	sy, 
							char 	*IconName,
							int 	id,
							int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
int DataLog_UartItemWindow(int 	sx, 
							int 	sy, 
							char 	*IconName,
							int 	id,
							int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
U32 SLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style);
int SensorInOutChannelConnect(void);
U32 Label(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int	fontcolor);
unsigned int    MainScreenCreate( HANDLE parent, HANDLE handle );
void DataLog_420SelectKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
/******************************************************************************
							Example Program
******************************************************************************/
                                     
// 키패드사용시 사용하는 콜백함수이다.
int  SensorMain_KeyPadEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	if(command==1) // Caller ID
	{
		char	buf[128];
        int i;
        for(i=0;i<128;i++)buf[i]=0;
		strcpy(buf, (char *)Data);
        Dprintf("KeyPadData = %s\n",buf);
	}
	return(1);
}
//이함수는 위젯의 키이벤트를 받기위해 사용한다.
void UART_AD420_Control( unsigned short value1, unsigned short value2 );

int  SensorMain_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle," ");
	Graphics 	*Gr = GetGraphics(NULL, parent, CLIENT_AREA); 	//그래픽영역을 얻는다.
    char        data[20];
    //int i=0;
    static int buttonMessage=0;
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    
	PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);	//dummy code	           
    if(win->ID>=1 && win->ID <= 5)//USART RATE
    {
        DataLog_UartRateWindow((LCD_X - 410)/2, (LCD_Y-156+52)/2, 0, gv.handle[win->ID], 0);
    }
    if(win->ID>=6 && win->ID <=9)
    {
        //선택된항목을화면에출력하고나온다.
        DataLog_UartItemWindow((LCD_X - 810)/2, (LCD_Y-572-52)/2, 0, gv.handle[win->ID], 0);
    }
	if(win->ID==10) {
		if(flash.Uart[USART_5]->SensorID == SENSOR_LAN) {
			flash.Uart[USART_5]->SensorID = SENSOR_NONE;
			strcpy(data,"DISABLE");
		} else {
			flash.Uart[USART_5]->SensorID = SENSOR_LAN;
			strcpy(data,"ENABLE");
		}
		SetWidgetText(gv.handle[10],data);
	}
#if defined (RAIN_STORAGE_CONTROL) || defined (RAIN_ENABLE)
	if(win->ID==77)
	{
		//flash.rsc.powerMeter
		KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "펄스값");
	}
#endif	
    if(win->ID == 90)//ALALOG INPUT CAL START
    {
        if(gInputCalStart == OFF)
        {
            gInputCalStart = ON;
           // SetWidgetStatus(gv.handle[win->ID],1);
        }
        else
        {
            gInputCalStart = OFF;
            if(buttonMessage==0)
            {
                Dprintf("전압저장\n");
                GetWidgetText(gv.handle[23],data);//ID
                strcpy(flash.Input[gInputCalChannel]->Name,data);
                GetWidgetText(gv.handle[24],data);//LOW MV
                flash.Input[gInputCalChannel]->CalLowmV = atof(data);
                GetWidgetText(gv.handle[25],data);//HIGH MV
                flash.Input[gInputCalChannel]->CalHighmV = atof(data);
                GetWidgetText(gv.handle[26],data);//ZERO
                flash.Input[gInputCalChannel]->CalLowLimit = atof(data);
                GetWidgetText(gv.handle[27],data);//SPAN
                flash.Input[gInputCalChannel]->CalHighLimit = atof(data);

                if(flash.Input[gInputCalChannel]->Name[0] == 'N'&& 
                    flash.Input[gInputCalChannel]->Name[1] == 'O' && 
                    flash.Input[gInputCalChannel]->Name[2] == 'N' &&
                    flash.Input[gInputCalChannel]->Name[3] == 'E')
                {
                    flash.Input[gInputCalChannel]->SensorStatus = 0;
                }
                else	flash.Input[gInputCalChannel]->SensorStatus = 1;
            }
            else//강수량
            {
#if defined (GREEN_ALGAE_ENABLE)				
                Dprintf("강수량저장\n");
                
                GetWidgetText(gv.handle[23],data);
                flash.pulsTimeMinSV = atoi(data);
                GetWidgetText(gv.handle[24],data);
                flash.pulsTimeSecSV = atoi(data);
                GetWidgetText(gv.handle[25],data);
                flash.pulsUnit = atof(data);
                GetWidgetText(gv.handle[26],data);
                flash.pulsUnit2 = atof(data);
                
                flash.pulsTimeSV= (flash.pulsTimeMinSV * 60) + flash.pulsTimeSecSV;
                Dprintf("flash.pulsTimeSV = %d\n",flash.pulsTimeSV);
                flash.pulsTimePV=0;
#endif				
            }
            DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
            //SetWidgetStatus(gv.handle[win->ID],0);
        }
    }
#ifdef DATALOG_7INCH    
    if(win->ID>=11 && win->ID <=20)//ANALOG INPUT CHANNEL SELECT
#endif
#ifdef DATALOG_8INCH    
    if(win->ID>=11 && win->ID <=18)//ANALOG INPUT CHANNEL SELECT
#endif
    {
        if(gInputCalStart == OFF)
        {
            if(buttonMessage==1)
            {
                SetWidgetText(gv.handle[50],"항목");
                SetWidgetText(gv.handle[51],"최소전압");
                SetWidgetText(gv.handle[52],"최대전압");
                SetWidgetText(gv.handle[53],"최소값");
                SetWidgetText(gv.handle[54],"최대값");
                buttonMessage=0;
            }
            SetWidgetStatus(gv.handle[gOldID],0);
            SetWidgetStatus(gv.handle[win->ID],1);
            gOldID = win->ID;
            gInputCalChannel = win->ID - 11;
            SetWidgetText(gv.handle[23], flash.Input[gInputCalChannel]->Name);
            sprintf(data,"%7.1f",flash.Input[gInputCalChannel]->CalLowmV);
            SetWidgetText(gv.handle[24], data);
            sprintf(data,"%7.1f",flash.Input[gInputCalChannel]->CalHighmV);
            SetWidgetText(gv.handle[25], data);
            sprintf(data,"%7.1f",flash.Input[gInputCalChannel]->CalLowLimit);
            SetWidgetText(gv.handle[26], data);
            sprintf(data,"%7.1f",flash.Input[gInputCalChannel]->CalHighLimit);
            SetWidgetText(gv.handle[27], data);
            Dprintf("InputCalChannel = %d\n",gInputCalChannel);
        }
    }
    if(win->ID>=23 && win->ID <=27)//ANALOG INPUT NAME, LOWMV, HIGHMV, ZERO, SPAN
    {
        if(buttonMessage==0)
        {
            if(gInputCalStart == ON)
            {
                switch( win->ID )
                {
#ifdef MANUAL_SYSTEM_PROJECT
                    case 23: KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL); break;
#endif                    
                    case 24: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "최소전압"); break;
                    case 25: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "최대전압"); break;
                    case 26: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "최소값"); break;
                    case 27: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "최대값"); break;
                }
            }
        }
        else
        {
            if(gInputCalStart == ON)
            {
                switch( win->ID )
                {
                    case 23: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "누적시간(분)"); break;
                    case 24: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "누적시간(초)"); break;
                    case 25: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "펄스당[mm]"); break;
                    case 26: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415+50)/2, 1, "", "단위면적"); break;                       
                }
            }
        }
    }
	//강수량
    if(win->ID==21)
    {				
#if defined (RAIN_STORAGE_CONTROL) || defined (RAIN_ENABLE) ||	defined (GREEN_ALGAE_ENABLE)        
		Dprintf("강수량선택\n");
        if(gInputCalStart == OFF)
        {
            buttonMessage=1;
            SetWidgetStatus(gv.handle[gOldID],0);
            SetWidgetStatus(gv.handle[win->ID],1);
            gOldID = win->ID;

            SetWidgetText(gv.handle[50],"누적시간(분)");
            SetWidgetText(gv.handle[51],"누적시간(초)");
            SetWidgetText(gv.handle[52],"클럭당[mm]");
            SetWidgetText(gv.handle[53],"단위면적");
            SetWidgetText(gv.handle[54],"");
            
            sprintf(data,"%10d",flash.pulsTimeMinSV);
            SetWidgetText(gv.handle[23], data);
            sprintf(data,"%10d",flash.pulsTimeSecSV);
            SetWidgetText(gv.handle[24], data);
            sprintf(data,"%10.2f",flash.pulsUnit);
            SetWidgetText(gv.handle[25], data);
            sprintf(data,"%10.2f",flash.pulsUnit2);
            SetWidgetText(gv.handle[26], data);
            //sprintf(data,"%7.1f",flash.Input[gInputCalChannel]->CalHighLimit);
            SetWidgetText(gv.handle[27], "");
            //Dprintf("InputCalChannel = %d\n",gInputCalChannel);
           
        }
#endif		
    }
/***************************

    4 ~20mA OUT 설정

***************************/    
    if(win->ID == 91)//ALALOG OUTPUT CAL START
    {
        if(gOutputCalStart == OFF)
        {
            gOutputCalStart = ON;
            gv.cal420Toggle = 2;
            //SetWidgetStatus(gv.handle[win->ID],1);
        }
        else
        {
            gv.cal420Toggle = OFF;
            gOutputCalStart = OFF;
            GetWidgetText(gv.handle[40],data);//ID
            strcpy(flash.Output[gAD420CalChannel]->Name,data);
            GetWidgetText(gv.handle[41],data);//LOW MV
            flash.Output[gAD420CalChannel]->CalLowLimit = atof(data);
            GetWidgetText(gv.handle[42],data);//HIGH MV     
            flash.Output[gAD420CalChannel]->CalHighLimit = atof(data);
            
            DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        }
    }
    
#ifdef DATALOG_7INCH    
    if(win->ID>=28 && win->ID <= 31)//ANALOG AD420 CHANNEL SELECT
#endif    
#ifdef DATALOG_8INCH    
    if(win->ID>=28 && win->ID <= 37)//ANALOG AD420 CHANNEL SELECT
#endif    
    {
        if(gOutputCalStart==OFF)
        {
            SetWidgetStatus(gv.handle[gOldID2],0);
            SetWidgetStatus(gv.handle[win->ID],1);
            gOldID2 = win->ID;
            
            gAD420CalChannel = win->ID - 28;
            
            SetWidgetText(gv.handle[40], flash.Output[gAD420CalChannel]->Name);
            sprintf(data,"%7.1f",flash.Output[gAD420CalChannel]->CalLowLimit); 
            SetWidgetText(gv.handle[41], data);
            sprintf(data,"%7.1f",flash.Output[gAD420CalChannel]->CalHighLimit);
            SetWidgetText(gv.handle[42], data);
            
            Dprintf("OutputCalChannel = %d\n",gAD420CalChannel);
        }
    }
    if(win->ID>=40 && win->ID <=43)//ANALOG AD420 NAME, LOWMV, HIGHMV, ZERO, SPAN
    {
        if(gOutputCalStart==ON)
        {
            switch( win->ID )
            {
                case 40: 
				{
					DataLog_420SelectKeypad(win->Parent, win->Handle,  NULL, 0, 0, 0, "", "4~20mA 출력항목선택");        
					//KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL); 										
					break;
				}
                case 41: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "Low mV"); break;
                case 42: KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "High mV"); break;
                case 43: 
                {
                    //4~20mA OUT
                    if( gv.cal420Toggle == 2)
                    {
                        gv.cal420Toggle = 3;
                        SetWidgetText(gv.handle[win->ID], "4mA");
#ifdef DATALOG_7INCH                        
                        IntAD420Control( 0, 0, 0, 0 );
#endif                        
#ifdef DATALOG_8INCH                        
						gv.ad420Ch1mA=0;
						gv.ad420Ch2mA=0;
#endif
                    }
                    else
                    {
                        gv.cal420Toggle = 2;
                        SetWidgetText(gv.handle[win->ID], "20mA");
#ifdef DATALOG_7INCH                        
                        IntAD420Control( 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF );
#endif                        
#ifdef DATALOG_8INCH                        
						gv.ad420Ch1mA=0xFFFF;
						gv.ad420Ch2mA=0xFFFF;
#endif                        

					}
                }
                break;
            }
        }
    }
	if(win->ID == 65) {
		if(flash.Uart[USART_1]->UartOr485Select==0) {
			flash.Uart[USART_1]->UartOr485Select=1;
			strcpy(data,"RS485");
			SetWidgetText(gv.handle[win->ID], data);
		} else {
			flash.Uart[USART_1]->UartOr485Select=0;
			strcpy(data,"RS232");
			SetWidgetText(gv.handle[win->ID], data);
		}		
	}
	if(win->ID == 66) {
		if(flash.Uart[USART_2]->UartOr485Select==0) {
			flash.Uart[USART_2]->UartOr485Select=1;
			strcpy(data,"RS485");
			SetWidgetText(gv.handle[win->ID], data);
		} else {
			flash.Uart[USART_2]->UartOr485Select=0;
			strcpy(data,"RS232");
			SetWidgetText(gv.handle[win->ID], data);
		}		
	}
	if(win->ID == 67) {
		if(flash.Uart[USART_3]->UartOr485Select==0) {
			flash.Uart[USART_3]->UartOr485Select=1;
			strcpy(data,"RS485");
			SetWidgetText(gv.handle[win->ID], data);
		} else {
			flash.Uart[USART_3]->UartOr485Select=0;
			strcpy(data,"RS232");
			SetWidgetText(gv.handle[win->ID], data);
		}		
	}
	if(win->ID == 68) {
		if(flash.Uart[USART_4]->UartOr485Select==0) {
			flash.Uart[USART_4]->UartOr485Select=1;
			strcpy(data,"RS485");
			SetWidgetText(gv.handle[win->ID], data);
		} else {
			flash.Uart[USART_4]->UartOr485Select=0;
			strcpy(data,"RS232");
			SetWidgetText(gv.handle[win->ID], data);
		}		
	}
    if(win->ID == 44)//HOME BUTTON
    {
        for(int i=0; i<20; i++) {//초기화
            data[i]=0;
        }
        gv.cal420Toggle = OFF;//420mA 보정시에 사용
        buttonMessage=0;//강수량 관련 변수
		{   //통신 보레이트 설정   
            GetWidgetText(gv.handle[1], data);           
            flash.Uart[USART_1]->Rate=atoi(data);
            GetWidgetText(gv.handle[2], data);
            flash.Uart[USART_2]->Rate=atoi(data);
            GetWidgetText(gv.handle[3], data);
            flash.Uart[USART_3]->Rate=atoi(data); 
            GetWidgetText(gv.handle[4], data);
            flash.Uart[USART_4]->Rate=atoi(data);
            //GetWidgetText(gv.handle[5], data);
            //flash.Uart[USART_5]->Rate=atoi(data);
            
			SmartLog_Uart4Init();//USART 1
            SmartLog_Uart1Init();//USART 2
            SmartLog_Uart3Init();//USART 3
            SmartLog_Uart6Init();//USART 4
		}
		{   //통신 센서선택
            GetWidgetText(gv.handle[6],data);
            UartPortInfo(USART_1,data);
            
            GetWidgetText(gv.handle[7],data);
            UartPortInfo(USART_2,data);
            
            GetWidgetText(gv.handle[8],data);
            UartPortInfo(USART_3,data);
            
            GetWidgetText(gv.handle[9],data);
            UartPortInfo(USART_4,data);

            if(gv.jsonTcpDebugFlag==0) {
                if(flash.Uart[USART_5]->SensorID==SENSOR_LAN) {
                    W5500_Init();
                }
            }      
		}		
        //4~20mA 전류가 출력된다. OFF가 아니면 보정모드이다.
        gv.cal420Toggle =  OFF;

		//gv.tmsStatusCode[USART_1]=0;
		//gv.tmsStatusCode[USART_2]=0;
		//gv.tmsStatusCode[USART_3]=0;
		//gv.tmsStatusCode[USART_4]=0;
		
		#if defined(TYPE_APPROVAL_ENABLE)
		PasswordInit();
		#endif
        //데이터저장
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
#ifdef MANUAL_SYSTEM_PROJECT    
        if(0==SensorInOutChannelConnect())		  //센서가 설정이 없는경우
        {
            SystemMain_Window(NULL, parent);
            Dprintf("SensorInOutChannelConnect\n");
        }
        else
        {
            gv.NewScreen = 0;//출력화면이 하나 인데 NEXT 버튼을 누르면 같은 화면이 중복 생성되는 에러를 막아준다.
            gv.OldScreen = 0;
            gv.NextScreenCreate=1;//첫번째 화면이 출력 된게한다. 첫번째 화면이란 USART 1의 센서가 연결되어있을때 화면이다.
            if(7 == MainScreenCreate(NULL, parent) )//7이 리턴되면 화면정보를 한번더 읽는다.화면이 순환형식으로 되어있어서 이다.
            {
                MainScreenCreate(NULL, parent);
            }
            //Dprintf("UART[%d] PH\n",flash.Uart[0]->SensorID);
            //Dprintf("UART[%d] PH\n",flash.Uart[1]->SensorID);
        }
#endif
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
int SensorMain_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
    
    char    buf[128];
    for(int i=0; i<128; i++) {
        buf[i]=0;
    }
      
    sprintf(buf,"%7.1f",flash.Input[gInputCalChannel]->mV); 
    buf[7]=0;
    SetWidgetText(gv.handle[62],buf);

    sprintf(buf,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
    SetWidgetText(gv.titleBarTimeId, buf);//시간출력
    
	gv.sleepModeTimerCntSecPv=0;//스립모드 진입타이머 초기화
	
	SetWidgetText(gv.handle[40], flash.Output[gAD420CalChannel]->Name);
	
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
#ifdef  DATALOG_8INCH
int SensorMain_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
    Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
                                                                     
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	
	//gv.tmsStatusCode[USART_1]=3;
	//gv.tmsStatusCode[USART_2]=3;
	//gv.tmsStatusCode[USART_3]=3;
	//gv.tmsStatusCode[USART_4]=3;

	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//투명위젯을 사용하려면 Paint Event에서 할배경작업을 미리해야한다. 그림이 필요하면 그림먼저 로딩할것(*********************** 중요 *********)
	//또는 PPaintDrawControl(ON); ->Gr->Draw를 실행후 사용한다.
	
	if(flash.systemColorNo==6) {
		Gr->SetColor(RGB565CONVERT(230,  230,  230));
	}
	else {
		Gr->SetColor(flash.systemBackGroundColor);
	}
	Gr->FillRect(0, 0, win->width-2, win->height-2);

    
    
	//Gr->BmpLoad(USER_SCREEN1, 0, 0, "WaterCell_Main_2.bmp");		    //평소의 화면
	//Gr->BmpLoad(USER_SCREEN2, 0, 0, "WaterCell_Main_2_t.bmp");		//터치화면(효과화면)
    //Gr->BitBlt(USER_SCREEN1, 0, 0, 0, 51, 1024, 640);
	gInputCalStart = OFF;       //INPUT CALSTART
    gOutputCalStart = OFF;      //OUTPUT CAL START
    
    unsigned int groupSpace = 25;
    unsigned int titleBarH = 50+groupSpace;
	unsigned int buttonH = 60;
    unsigned int buttonW = 100;

    unsigned int groupX = 3;
    unsigned int groupY = titleBarH;
    unsigned int groupW = buttonW+buttonW+buttonW+ 6;
    unsigned int groupH = buttonH+buttonH+4;
    
    unsigned int buttonX = groupX;
    unsigned int buttonY = 0;
    
    unsigned int buttonSpace = 2;
	char buf[30];
    char            sensorNameBuffer[USART_MAX][20];
    char            usartRateBuffer[USART_MAX][20];
    unsigned int count1=0, count2 = 0;
    unsigned int comPort = 0;

	/**
	*	화면을 만들지 않고 제어만 실행한다.
	*/
	#if defined (WINCH_ENABLE)
	#if defined (FARM_MANAGEMENT_ENABLE)
	flash.Uart[USART_2]->SensorID = SENSOR_WINCH;
	#endif
	#endif


    for(count1=0; count1 < USART_MAX; count1++) 
    {
        for(count2=0; count2 < 20; count2++) 
        {
            sensorNameBuffer[count1][count2] = 0;
            usartRateBuffer[count1][count2] = 0;
        }
    }
	
	//Dprintf("flash.Uart[USART_5]->SensorID=%d\n",flash.Uart[USART_5]->SensorID);
    for(comPort=0; comPort < USART_MAX; comPort++) {   //선택된 항목 출력
        sprintf(usartRateBuffer[comPort],"%d",flash.Uart[comPort]->Rate);
        
        if(flash.Uart[comPort]->SensorID == SENSOR_YSI)strcpy(sensorNameBuffer[comPort],"YSI-S");
        else if(flash.Uart[comPort]->SensorID == SENSOR_TN)strcpy(sensorNameBuffer[comPort],"TN");
        else if(flash.Uart[comPort]->SensorID == SENSOR_TP)strcpy(sensorNameBuffer[comPort],"TP");
        else if(flash.Uart[comPort]->SensorID == SENSOR_AUTO_SAMPLER)strcpy(sensorNameBuffer[comPort],"Auto-W");
        else if(flash.Uart[comPort]->SensorID == SENSOR_WIZ) strcpy(sensorNameBuffer[comPort],"WIZ");
        else if(flash.Uart[comPort]->SensorID == SENSOR_MODBUS) strcpy(sensorNameBuffer[comPort],"MODBUS"); 
        else if(flash.Uart[comPort]->SensorID == SENSOR_TMS) strcpy(sensorNameBuffer[comPort],"TMS");
        else if(flash.Uart[comPort]->SensorID == SENSOR_EXO) 
		{
			strcpy(sensorNameBuffer[comPort],"EXO");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
		}
        else if(flash.Uart[comPort]->SensorID == SENSOR_HFTOL) strcpy(sensorNameBuffer[comPort],"HF-TOL");
        else if(flash.Uart[comPort]->SensorID == SENSOR_HFCLX) strcpy(sensorNameBuffer[comPort],"HF-CLX");
        #ifdef SONTEK_ENABLE
        else if(flash.Uart[comPort]->SensorID == SENSOR_SONTEK) strcpy(sensorNameBuffer[comPort],"SONTEK");
        #endif
        #ifdef ADP_SONTEK_ENABLE	
        else if(flash.Uart[comPort]->SensorID == SENSOR_ADPSONTEK) strcpy(sensorNameBuffer[comPort],"ADP");
        #endif
        else if(flash.Uart[comPort]->SensorID == SENSOR_SEJONG) strcpy(sensorNameBuffer[comPort],"SEJONG");
        else if(flash.Uart[comPort]->SensorID == SENSOR_SCAN) strcpy(sensorNameBuffer[comPort],"SCAN");
        else if(flash.Uart[comPort]->SensorID == SENSOR_EX100_1) strcpy(sensorNameBuffer[comPort],"EX100-1");
        else if(flash.Uart[comPort]->SensorID == SENSOR_EX100_2) strcpy(sensorNameBuffer[comPort],"EX100-2");
        else if(flash.Uart[comPort]->SensorID == ANALOG_OUT) strcpy(sensorNameBuffer[comPort],"AO");
        else if(flash.Uart[comPort]->SensorID == SENSOR_EXTTMS) strcpy(sensorNameBuffer[comPort],"EXTTMS");
        else if(flash.Uart[comPort]->SensorID == SENSOR_INTELL) strcpy(sensorNameBuffer[comPort],"INTELL");//USART 1에만 사용할 수 있다.
        else if(flash.Uart[comPort]->SensorID == SENSOR_FLOW) strcpy(sensorNameBuffer[comPort],"FLOW");
        else if(flash.Uart[comPort]->SensorID == SENSOR_TUBIDITY) strcpy(sensorNameBuffer[comPort],"TUBIDITY");//USART 1에만 사용할 수 있다.
        else if(flash.Uart[comPort]->SensorID == SENSOR_UITNH4)strcpy(sensorNameBuffer[comPort],"UITNH4");
        else if(flash.Uart[comPort]->SensorID == SENSOR_LAN) strcpy(sensorNameBuffer[comPort],"LAN");
        else if(flash.Uart[comPort]->SensorID == SENSOR_WINCH) 
        {
            strcpy(sensorNameBuffer[comPort],"WINCH");
            flash.Uart[comPort]->SensorID=SENSOR_WINCH;
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_DO)
        {
            strcpy(sensorNameBuffer[comPort],"PONSEL_DO");
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_ARGADV)
        {
            strcpy(sensorNameBuffer[comPort],"ARGADV");
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_EC)
        {
            strcpy(sensorNameBuffer[comPort],"PONSEL_EC");
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_PH)
        {
            strcpy(sensorNameBuffer[comPort],"PONSEL_PH");
            Dprintf("UART[%d] PONSEL_PH\n",comPort);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_PH_COND)
        {
            strcpy(sensorNameBuffer[comPort],"PH_COND");
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_RAIN)
        {
            strcpy(sensorNameBuffer[comPort],"RAIN");
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_ECD_PH)
        {
            strcpy(sensorNameBuffer[comPort],"ECD_pH");
        }
        #ifdef M_SERIES
        else if(flash.Uart[comPort]->SensorID == SENSOR_MSERIES)
        {
            strcpy(sensorNameBuffer[comPort],"Mseries");
        }
        #endif
        #ifdef AUTO_SAMPLER
        else if(flash.Uart[comPort]->SensorID == SENSOR_AUTOSAMPLER)
        {
            strcpy(sensorNameBuffer[comPort],"AutoSampler");
        }
        #endif
		#ifdef DMSAMPLER_ENABLE
        else if(flash.Uart[comPort]->SensorID == SENSOR_DMSAMPLER)
        {
            strcpy(sensorNameBuffer[comPort],"DMSampler");
        }
		#endif
        else if(flash.Uart[comPort]->SensorID == SENSOR_WS501)
        {
            strcpy(sensorNameBuffer[comPort],"WS501");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_SONAR)
        {
            strcpy(sensorNameBuffer[comPort],"SONAR");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
		#ifdef GPS_ENABLE
        else if(flash.Uart[comPort]->SensorID == SENSOR_GPS)
        {
            strcpy(sensorNameBuffer[comPort],"GPS");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
		#endif
		else if(flash.Uart[comPort]->SensorID == SENSOR_NEP500)
		{
            strcpy(sensorNameBuffer[comPort],"NEP500");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
		}
		else if(flash.Uart[comPort]->SensorID == SENSOR_SAMPLER100)
		{
            strcpy(sensorNameBuffer[comPort],"SAMPLER100");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
		}
		else if(flash.Uart[comPort]->SensorID == SENSOR_STATUS)
		{
            strcpy(sensorNameBuffer[comPort],"STATUS");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
		}
		else if(flash.Uart[comPort]->SensorID == SENSOR_KECO)
		{
            strcpy(sensorNameBuffer[comPort],"KECO");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
		}
        else if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG) 
        {
            strcpy(sensorNameBuffer[comPort],"DEBUG");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_LDI) 
        {
            strcpy(sensorNameBuffer[comPort],"LDI");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_ODO) 
        {
            strcpy(sensorNameBuffer[comPort],"ODO");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_SPECTRO_D) 
        {
            strcpy(sensorNameBuffer[comPort],"SPECTRO-D");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_SPECTRO_M) 
        {
            strcpy(sensorNameBuffer[comPort],"SPECTRO-M");
			Dprintf(" UART[%d] NAME = %s\n",comPort+1,sensorNameBuffer[comPort]);
        }
        else 
        {
            flash.Uart[comPort]->SensorID = 0;
            strcpy(sensorNameBuffer[comPort],"NONE");
        }
    }


    //TitleBar(handle, 0, 0, LCD_X, 50, 1, "SENSOR", clYellow, 1, RIGHT_ALIGN, 2, NULL);	
 
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW+23+10+10+16, groupH , "USART1");
    
    buttonX = groupX + buttonSpace;
    buttonY = groupY + buttonH +buttonSpace;
    gv.handle[1] = SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, usartRateBuffer[USART_1],  PUSH_BUTTON, 1, SensorMain_KeyEvent);   
	if(flash.Uart[USART_1]->UartOr485Select==0) {
		strcpy(buf,"RS232");
	} else {
		strcpy(buf,"RS485");
	}		
	gv.handle[65] = SButton(handle, buttonX+buttonW, buttonY+buttonSpace, buttonW, buttonH, 0, buf,  PUSH_BUTTON, 65, SensorMain_KeyEvent); 
    gv.handle[6] = SButton(handle, buttonX+buttonW+buttonW+buttonSpace, buttonY+buttonSpace, buttonW+23+10+10+16, buttonH, 0, sensorNameBuffer[USART_1],  PUSH_BUTTON, 6, SensorMain_KeyEvent);   
    
    
    groupY = groupY + 4 + groupH;
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW+23+10+10+16, groupH , "USART2");
    buttonX = groupX + buttonSpace;
    buttonY = groupY + buttonH +buttonSpace;
    gv.handle[2] = SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, usartRateBuffer[USART_2],  PUSH_BUTTON, 2, SensorMain_KeyEvent);   
	if(flash.Uart[USART_2]->UartOr485Select==0) {
		strcpy(buf,"RS232");
	} else {
		strcpy(buf,"RS485");
	}		
	gv.handle[66] = SButton(handle, buttonX+buttonW, buttonY+buttonSpace, buttonW, buttonH, 0, buf,  PUSH_BUTTON, 66, SensorMain_KeyEvent);   
    gv.handle[7] = SButton(handle, buttonX+buttonW+buttonW+buttonSpace, buttonY+buttonSpace, buttonW+23+10+10+16, buttonH, 0, sensorNameBuffer[USART_2],  PUSH_BUTTON, 7, SensorMain_KeyEvent);   

    groupY = groupY + 4 + groupH;
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW+23+10+10+16, groupH , "USART3");
    buttonX = groupX + buttonSpace;
    buttonY = groupY + buttonH +buttonSpace;
    gv.handle[3] = SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, usartRateBuffer[USART_3],  PUSH_BUTTON, 3, SensorMain_KeyEvent);   
	if(flash.Uart[USART_3]->UartOr485Select==0) {
		strcpy(buf,"RS232");
	} else {
		strcpy(buf,"RS485");
	}		
	gv.handle[67] = SButton(handle, buttonX+buttonW, buttonY+buttonSpace, buttonW, buttonH, 0, buf,  PUSH_BUTTON, 67, SensorMain_KeyEvent);   
    gv.handle[8] = SButton(handle, buttonX+buttonW+buttonW+buttonSpace, buttonY+buttonSpace, buttonW+23+10+10+16, buttonH, 0, sensorNameBuffer[USART_3],  PUSH_BUTTON, 8, SensorMain_KeyEvent);   

    groupY = groupY + 4 + groupH;
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW+23+10+10+16, groupH , "USART4");
    buttonX = groupX + buttonSpace;
    buttonY = groupY + buttonH +buttonSpace;
	
    gv.handle[4] = SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, usartRateBuffer[USART_4],  PUSH_BUTTON, 4, SensorMain_KeyEvent);   
	if(flash.Uart[USART_4]->UartOr485Select==0) {
		strcpy(buf,"RS232");
	} else {
		strcpy(buf,"RS485");
	}		
	gv.handle[68] = SButton(handle, buttonX+buttonW, buttonY+buttonSpace, buttonW, buttonH, 0, buf,  PUSH_BUTTON, 68, SensorMain_KeyEvent);   
    gv.handle[9] = SButton(handle, buttonX+buttonW+buttonW+buttonSpace, buttonY+buttonSpace, buttonW+23+10+10+16, buttonH, 0, sensorNameBuffer[USART_4],  PUSH_BUTTON, 9, SensorMain_KeyEvent);   
    	
	buttonW = 150;
	groupW = buttonW+buttonW+6;	
	groupY = groupY + 4 + groupH;
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW+23+10+10+16, groupH , "External communication");
    buttonX = groupX + buttonSpace;
    buttonY = groupY + buttonH +buttonSpace;
    gv.handle[5] = SButton(handle, buttonX, buttonY+buttonSpace, buttonW+11+5+5+8, buttonH, 0, "LAN",  PUSH_BUTTON, 5, 0);   
	
	//Dprintf("flash.Uart[USART_5]->SensorID=%d\n",flash.Uart[USART_5]->SensorID);
	if(flash.Uart[USART_5]->SensorID == SENSOR_LAN) {
		strcpy(buf,"ENABLE");
	} else {
		strcpy(buf,"DISABLE");
	}
    gv.handle[10] = SButton(handle, buttonX+buttonW+buttonSpace+11+5+5+8, buttonY+buttonSpace, buttonW+11+5+5+8, buttonH, 0, buf,  PUSH_BUTTON, 10, SensorMain_KeyEvent);   
  
    titleBarH = 50+groupSpace;
	buttonH = buttonH;
    buttonW = 150;

    groupX = buttonW+buttonW + 6 + groupSpace + groupSpace + 23 + 10;
    groupY = titleBarH;
    groupW = buttonW+buttonW + 6;
    groupH = buttonH+buttonH+4 + buttonH+4  + buttonH+4  + buttonH+4  + buttonH+4 + buttonH+4+ buttonH+4+ buttonH+4+ buttonH+4;
    
    buttonX = groupX;
    buttonY = 0;
    
    buttonSpace = 2;


#ifdef MANUAL_SYSTEM_PROJECT
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH , "ANALOG INPUT");
#endif
#ifdef REMOTE_TERMINAL_UNIT
	GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH , "ANALOG INPUT");    
#endif	
#if 1
    buttonX = groupX + buttonSpace;
    buttonY = groupY + buttonH +buttonSpace +buttonSpace;
    buttonW = 148;

    //SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH , 0, "128.1",  PUSH_BUTTON, 1, 0);   
    //gv.handle[62]=SLabel(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "0.0",  clBlack);
    gv.handle[62]=SLabel(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "0.0",clYellow,clBlack,1);
    gv.handle[60]=SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CAL START",  TOGGLE_BUTTON, 90, SensorMain_KeyEvent);   
    //buttonY = buttonY + buttonH;
    
    buttonW = 74;
    buttonY = buttonY + buttonH + buttonSpace+ buttonSpace+ buttonSpace+ buttonSpace;
    gv.handle[11] = SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, "CH1",  PUSH_BUTTON, 11, SensorMain_KeyEvent); 
    gOldID = 11;
    gInputCalChannel = 0;
    SetWidgetStatus(gv.handle[11],1);
    gv.handle[12] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH2",  PUSH_BUTTON, 12, SensorMain_KeyEvent);   
    gv.handle[13] = SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH3",  PUSH_BUTTON, 13, SensorMain_KeyEvent);   
    gv.handle[14] = SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH4",  PUSH_BUTTON, 14, SensorMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    gv.handle[15] = SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, "CH5",  PUSH_BUTTON, 15, SensorMain_KeyEvent);   
    gv.handle[16] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH6",  PUSH_BUTTON, 16, SensorMain_KeyEvent);   
    gv.handle[17] = SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH7",  PUSH_BUTTON, 17, SensorMain_KeyEvent);   
    gv.handle[18] = SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH8",  PUSH_BUTTON, 18, SensorMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    gv.handle[19] = SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, "CH9",  PUSH_BUTTON, 19, SensorMain_KeyEvent);   
    gv.handle[20] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH10",  PUSH_BUTTON, 20, SensorMain_KeyEvent);   
    
    gv.handle[21] = SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW+buttonW, buttonH, 0, "강수량",  PUSH_BUTTON, 21, SensorMain_KeyEvent);   
    //gv.handle[21] = SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH11",  PUSH_BUTTON, 21, 0);   
    //gv.handle[22] = SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH12",  PUSH_BUTTON, 22, 0);   

    char    dataBuf[16];
    
    buttonW = 148;
    buttonY = buttonY + buttonH + buttonSpace + 7;
    //U32 SLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style)
    //Label(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int	fontcolor)
    //SLabel(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "NAME",  clBlack, RGB565CONVERT(191, 191, 191),1);   
    gv.handle[50] =Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "항목",  clBlack);
    gv.handle[23] = SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, flash.Input[0]->Name,  PUSH_BUTTON, 23, SensorMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    //SLabel(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "Low mV",  clBlack, RGB565CONVERT(191, 191, 191),1);   
    gv.handle[51] =Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "최소전압",  clBlack);
    sprintf( dataBuf,"%7.1f",flash.Input[gInputCalChannel]->CalLowmV);
    gv.handle[24] = SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 24, SensorMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    //SLabel(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "High mV",  clBlack, RGB565CONVERT(191, 191, 191),1);   
    gv.handle[52] =Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "최대전압",  clBlack);
    sprintf( dataBuf,"%7.1f",flash.Input[gInputCalChannel]->CalHighmV);
    gv.handle[25] = SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 25, SensorMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    //SLabel(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "Zero",  clBlack, RGB565CONVERT(191, 191, 191),1);   
    gv.handle[53] =Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "최소값",  clBlack);
    sprintf( dataBuf,"%7.1f",flash.Input[gInputCalChannel]->CalLowLimit);
    gv.handle[26] = SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 26, SensorMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    //SLabel(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "Span",  clBlack, RGB565CONVERT(191, 191, 191),1);   
    gv.handle[54] =Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "최대값",  clBlack);
    sprintf( dataBuf,"%7.1f",flash.Input[gInputCalChannel]->CalHighLimit);
    gv.handle[27] = SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 27, SensorMain_KeyEvent);   
#endif    


    titleBarH = 50+groupSpace;
	buttonH = buttonH;
    buttonW = 150;

    groupX = buttonW+buttonW + 6 + groupSpace + buttonW+buttonW + 6 + groupSpace + groupSpace + 23+5;
    groupY = titleBarH;
    groupW = buttonW+buttonW + 6;
    groupH = buttonH+buttonH+4 + buttonH+4  + buttonH+4  + buttonH+4  + buttonH+4 + buttonH+4+ buttonH+4+ buttonH+4+ buttonH+4;
    //groupH -= 10;
    
    buttonX = groupX;
    buttonY = 0;
    
    buttonSpace = 2;

    GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH , "4~20mA 출력설정");
    buttonX = groupX + buttonSpace;
    buttonY = groupY + buttonH +buttonSpace;
    buttonW = 74;

    gv.handle[28]=SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, "CH1",  TOGGLE_BUTTON, 28, SensorMain_KeyEvent);   
    gOldID2 = 28;
    gAD420CalChannel = 0;
    SetWidgetStatus(gv.handle[28],1);
    
    gv.handle[29]=SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH2",  PUSH_BUTTON, 29, SensorMain_KeyEvent);   
    gv.handle[30]=SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH3",  SELECT_BUTTON, 30, 0);   
    gv.handle[31]=SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH4",  SELECT_BUTTON, 31, 0);   

    buttonY = buttonY + buttonH + buttonSpace;
    gv.handle[32]=SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, "CH5",  SELECT_BUTTON, 32, 0);   
    gv.handle[33]=SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH6",  SELECT_BUTTON, 33, 0);   
    gv.handle[34]=SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH7",  SELECT_BUTTON, 34, 0);   
    gv.handle[35]=SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH8",  SELECT_BUTTON, 35, 0);   

    buttonY = buttonY + buttonH + buttonSpace;
    gv.handle[36]=SButton(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, 0, "CH9",  SELECT_BUTTON, 36, 0);   
    gv.handle[37]=SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH10",  SELECT_BUTTON, 37, 0);   
    gv.handle[38]=SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH11",  SELECT_BUTTON, 38, 0);   
    gv.handle[39]=SButton(handle, buttonX+buttonW+buttonSpace +buttonW+buttonSpace +buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "CH12",  SELECT_BUTTON, 39, 0);   

    buttonW = 150;
    buttonY = buttonY + buttonH + buttonSpace + 7;
  
    gv.handle[61]=SButton(handle, buttonX , buttonY+buttonSpace, buttonW * 2, buttonH, 0, "CAL START",  TOGGLE_BUTTON, 91, SensorMain_KeyEvent);   
    
    buttonW = 148;
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "Name",  clBlack);  
    gv.handle[40]=SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, flash.Output[gAD420CalChannel]->Name,  PUSH_BUTTON, 40, SensorMain_KeyEvent);   
  
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "Zero",  clBlack);  
    sprintf( dataBuf,"%7.1f",flash.Output[gAD420CalChannel]->CalLowLimit);
    gv.handle[41]=SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 41, SensorMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "Span",  clBlack);  
    sprintf( dataBuf,"%7.1f",flash.Output[gAD420CalChannel]->CalHighLimit);
    gv.handle[42]=SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 42, SensorMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "4~20mA",  clBlack);  
    gv.handle[43]=SButton(handle, buttonX+buttonW+buttonSpace+buttonSpace-1, buttonY+buttonSpace, buttonW, buttonH, 0, "CALOUT",  PUSH_BUTTON, 43, SensorMain_KeyEvent);   

	if(flash.systemColorNo!=6) {
		NButton(handle, 0+180+170+170+170+170, LCD_Y-65, 164, 64, "48G_Home_Icon2.bmp", "HOME",  PUSH_BUTTON, 44, SensorMain_KeyEvent);
	}
	else {
		SButton(handle, 0+180+170+170+170+170, LCD_Y-65, 164, 64, "48G_Home_Icon2.bmp", "HOME",  PUSH_BUTTON, 44, SensorMain_KeyEvent);
	}


    TitleBar_MenuButton_Create(handle,10,SensorMain_KeyEvent);


    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
    play_timer =  OpenTimer(parent, handle, 1000);	
    StartTimer(play_timer);  	
	return(1);
}
#endif
int SensorMain_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	//static int start=0;
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
    /*
	//사용자 프로그램 영역
	if(start==0) //메인윈도우 같이 한번생성후 계속사용할때, 1회만 처리할내용
	{
		start=1;
	}	
	Gr->SetColor(clBlack);
	Gr->FillRect(0, 0, LCD_X, LCD_Y);
    */
	//SetFontAliasing("ng128_e.sif");	
	//TextOutAliasing(10, 100, "-1.234", clWhite, 0, "ng128_e.sif");		

	
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

int SensorMain_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
int SensorMain_Window(HANDLE parent, HANDLE handle) {
	if(GetWindowName("SensorMain_Window")) 
	{
		Dprintf("SensorMain_Window:: Windows already Create\n");
		return 0;		
	}	
	makeWindow(
				LCD,								// Display Section Position, 이윈도우가 어디에 표시되는가?
				"SensorMain_Window",			// Windows Name, 이이름으로 이윈도우의 핸들을 찾을수 있다. 64자
				DESKTOP_WINDOW,		// Parent Window Handle
				CONTAINER_WINDOW,	// Windows Kind
				STYLE_NONE,					// Windows Style, TEECO_STYLE, XP_STYLE 두가지가 기본준비되어있다.
				0, 0, 							// Start Position
				WINDOW_WIDTH, WINDOW_HEIGHT,				// Size
				SHOW,							// Create after Show
				SensorMain_CreateEvent,		// CreateEvent
				SensorMain_PaintEvent,			// PaintEvent
				NULL,	// MouseOnEvent
				NULL,	// MouseOffEvent
				NULL,							// MouseMoveEvent
				NULL,							// GestureEvent
				NULL,						// ClickEvent, 콜백함수를 등록한다.(콜백함수는 어떤것도 등록가능하다)
				SensorMain_TimerEvent,			// TimerEvent
				NULL,		// ControlEvent, 이윈도우를 제어하기위해 사용된다.
				NULL,		// ChangeEvent, 윈도우의 속성값이 
				NULL,			// KeyEvent
				NULL, 		// EncoderEvent, 시스템에 엔코더가 연결되오 있을때 발생된다.
				NULL, 							// FocusEvent,  마우스를 사용할때 커서가 위에 있으면 발생된다.
				SensorMain_CloseEvent,			// Close Event, 윈도우가 닫힐때 발생한다.
				NULL,							// PARAMETER, window->Param에 저장된다.
				NULL,							// NormalName용 BMP 파일명, window->NormalName, 
				NULL,							// TouchName용 BMP파일명, window->TouchName
				NULL,							// Windows Text, win->Text 에 저장된다. 128Byte의 사이즈를 가지고 있다. 초과되지 않도록 한다.
				MODALLESS_WINDOW,	// window->Mode에 저장된다.
				NULL					// WidgetStruct 구조체이다. 속성이 모자랄때 추가하기위해 사용한다.
				);
    if(parent==NULL && handle==NULL)//백그라운드 윈도우를 삭제한다.
    {
        Dprintf(" SensorMain_Window :: MainWindows Create \n");
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

