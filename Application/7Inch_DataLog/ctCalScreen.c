#include "TEECO_System.h"
#include "DataLog.h"
#include "time.h"
#if defined (YSI_ODD_ENABLE) || defined (YSI_ODOCT_ENABLE)

volatile static int play_timer=0;

#define		WINDOW_WIDTH		LCD_X
#define		WINDOW_HEIGHT		LCD_Y

/******************************************************************************
							Private & Local Variables
******************************************************************************/
/******************************************************************************
							Function Prototype
******************************************************************************/
/******************************************************************************
							Example Program
******************************************************************************/
//이함수는 위젯의 키이벤트를 받기위해 사용한다.
int odo_SlaveAddressGet(int id, int addr, int setAddress, int comPort);
int odo_SlaveAddressSet(int id, int addr, int setAddress, int comPort);
int odo_ZeroCal(int id, int addr, int num, unsigned char cnt, int item, int comPort );
int odo_FactoryReset(int id, int addr, int len, int comPort);
int odo_mgLCal(int id, int addr, int num, unsigned char cnt, int item, int comPort );
int odo_AirCal(int id, int addr, int num, unsigned char cnt, int item, int comPort );
int cond_mgLCal(int id, int addr, int num, unsigned char cnt, int item, int comPort );
int odo_SalinitySet(int id, int addr, int num, unsigned char cnt, int item, int comPort ); 
int odo_RequestMeasure(int id, int addr, int len, int comPort);
int GetTimeT(int year, int month, int day, int hour, int minute, int second);
int odo_Status(int id, int addr, int len, int comPort);
extern int gNextPointStart;
extern int gCalStartFlag;
extern int gChangeID;
extern int gNewID;
extern int gIDChangeFlag;
extern time_t baseTime;
extern int gOdoToggle;
extern int gCapUpdateStep;

int gSalCalFlag=0;
int gCondCalFlag=0;
int gTempCalFlag=0;
int	CT_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle," ");
	Graphics 	*Gr = GetGraphics(NULL, parent, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    unsigned char buf32[32];
    for(int i=0; i<32; i++) {
        buf32[i]=0;
    }
	PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);	//dummy code	           
    if(win->ID==2) {
        gv.odo_ArrayAddress++;
        if(gv.odo_ArrayAddress >= 9) {
            gv.odo_ArrayAddress = 0;
        }
        sprintf((char *)buf32,"아이디.%d",gv.odo_ArrayAddress+1);//배열주소  
        gv.modbusId=gv.odo_ArrayAddress+1;//센서주소
        SetWidgetText(gv.handle2[win->ID], (char *)buf32);
        Dprintf("MODBUS DeviceAddress = 0x%02x\n",gv.modbusId);
    } else if(win->ID==3) {
        odo_FactoryReset(gv.modbusId, 0x0300, 0x0001, USART_1);
        Dprintf("교정초기화\n");
    } else if(win->ID==5) {
        if(gCalStartFlag==0) {
            gCalStartFlag = 1;
            SetWidgetStatus(gv.handle2[win->ID],ON);
        } else if(gCalStartFlag==1) {
            gCalStartFlag=0;
            GetWidgetText(gv.handle2[6],(char *)buf32);
            flash.odoct.saltCalPv=atof((char *)buf32);
            gSalCalFlag=1;
            //cond_mgLCal(gv.modbusId, 0x0330, 0x0004, 0x08, 1, USART_1);
            SetWidgetStatus(gv.handle2[win->ID],OFF);
            Dprintf("전도도염분 교정시작 = %.1f\n",flash.odoct.saltCalPv);
        }
    } else if(win->ID==6) {
        KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "교정값");
        Dprintf("염분교정값설정\n");
    } else if(win->ID==7) {
        if(gCalStartFlag==0) {
            gCalStartFlag = 2;
            SetWidgetStatus(gv.handle2[win->ID],ON);
        } else if(gCalStartFlag==2) {
            gCalStartFlag=0;
            GetWidgetText(gv.handle2[10],(char *)buf32);
            flash.odoct.condCalPv=atof((char *)buf32);
            gCondCalFlag=1;
            //cond_mgLCal(gv.modbusId, 0x0340, 0x0004, 0x08, 0, USART_1);
            SetWidgetStatus(gv.handle2[win->ID],OFF);
            Dprintf("전도도교정값 = %.1f\n",flash.odoct.condCalPv);
        }
    } else if(win->ID==9) {
        GetWidgetText(gv.handle2[11],(char *)buf32);
        flash.odoct.tempCalPv=atof((char *)buf32);
        gTempCalFlag=1;
        //odo_SalinitySet(gv.modbusId, 0x502, 0x0002, 0x04, 3, USART_1);
        Dprintf("전도도온도보상적용 = %.1f\n",flash.odoct.tempCalPv);
    } else if(win->ID==10) {
        KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "전도도교정값");
        Dprintf("전도도교정값설정\n");
    } else if(win->ID==11) {
        KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "온도보상");
        Dprintf("온도도보상\n");
    } else if(win->ID==13) {//close
        GetWidgetText(gv.handle2[6],(char *)buf32);
        flash.odoct.saltCalPv=atof((char *)buf32);
        
        GetWidgetText(gv.handle2[10],(char *)buf32);
        flash.odoct.condCalPv=atof((char *)buf32);
        
        GetWidgetText(gv.handle2[11],(char *)buf32);
        flash.odoct.tempCalPv=atof((char *)buf32);
		
        gv.odo_mode=0;//측정모드
        gv.request_DataFlag=ON;

        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        
        Dprintf("전도도 교정 데이터 저장\n");
        StopTimer(play_timer);  	        
        TimerQClear();
        CloseWindow(0, parent);
    } else if(win->ID==12) {//현재 ID 변경
        if(gNewID==0) {
            DataLog_MessageBox_Window("경고", "현재아이디 확인 후 변경이 가능합니다.");
        } else if(gNewID == gChangeID) {
            DataLog_MessageBox_Window("경고", "변경하고자하는 아이디가 같다.");
        } else {
            DataLog_MessageBox_Window("ID변경", "센서아이디변경");
        }
    } else if(win->ID==14) {//현재 ID 요청
        odo_SlaveAddressGet(gv.modbusId,0x0000,0x0001,USART_1);
    } else if(win->ID==15) {//ID 설정
        gChangeID++;
        if(gChangeID>=10) {
            gChangeID=1;
        }        
        sprintf((char *)buf32,"%d",gChangeID);
        SetWidgetText(gv.handle2[15],(char *)buf32);
    }

	PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);    
	return(1);
}
int	CT_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");
    
    unsigned int comPort = USART_1;
    unsigned int address = 0;
    unsigned char sumBuf[50];
    char crcBuf[2];
    unsigned short crc=0;
	char data64[64];
    float fData=0.0;
    
    if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

    for(int i=0; i<64; i++) {
        data64[i]=0;
    }
    sprintf(data64,"%.2f'C / %.1fus/cm / %.1fppt",gv.tempPv[gv.odo_ArrayAddress],gv.spcondPv[gv.odo_ArrayAddress],gv.salinityPv[gv.odo_ArrayAddress]);
    SetWidgetText(gv.handle2[0],data64);
    
    sprintf(data64,"%d",gNewID);
    SetWidgetText(gv.handle2[14],data64);

    //RTU 시간    
    int year=currentDate.year+2000, month=currentDate.month, day=currentDate.date, hour=currentDate.hour, minute=currentDate.minute, sec=currentDate.sec;   
    int date = -1 ;  
    
    date =  (year * 10000) + (month * 100) + day;   //20211206;
    //Dprintf("Date = %d\n", date);
    year = date / 10000 ;  
    month = ( date - year*10000 ) / 100 ;   
    day = date - ( year * 10000 + month * 100 )  ;     
    baseTime =GetTimeT(year,month,day,hour,minute,sec);
    //Dprintf("DATE = %d\n",baseTime);
    
    struct tm *tm;
    tm = localtime(&baseTime); 
    //Dprintf("BaseTime = %d\n",ctime(&baseTime));
    //Dprintf("year = %d\n",tm->tm_year + 1900);
    //Dprintf("momth = %d\n",tm->tm_mon+1);
    //Dprintf("day = %d\n",tm->tm_mday);
    //Dprintf("hour = %d\n",tm->tm_hour);
    //Dprintf("min = %d\n",tm->tm_min);
    //Dprintf("sec = %d\n",tm->tm_sec);

    tm = localtime(&gv.qcScoreTime); 
    if((tm->tm_year + 1900)==1970) {
        sprintf(data64,"%04d/%02d/%02d %02d:%02d",0, 0, 0, 0, 0);
    } else {
        sprintf(data64,"%04d/%02d/%02d %02d:%02d",tm->tm_year + 1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min);
    }
    SetWidgetText(gv.handle2[16], data64);    
    
    
    if(gIDChangeFlag==1) {
        gIDChangeFlag=0;
        odo_SlaveAddressSet(gv.modbusId,0x0000,gChangeID,USART_1);
    }
    else if(gSalCalFlag==1) {
        gSalCalFlag=0;
        cond_mgLCal(gv.modbusId, 0x0330, 0x0004, 0x08, 1, USART_1);
    }    
    else if(gCondCalFlag==1) {
        gCondCalFlag=0;
        cond_mgLCal(gv.modbusId, 0x0340, 0x0004, 0x08, 0, USART_1);
    }        
    else if(gTempCalFlag==1) {
        gTempCalFlag=0;
        odo_SalinitySet(gv.modbusId, 0x502, 0x0002, 0x04, 3, USART_1);
    }        
    else if(gv.request_DataFlag==OFF) {
        if(gCalStartFlag==0) {
            if(gOdoToggle==0) {
                gOdoToggle=1;
                //Dprintf("QC Score\n");
                gCapUpdateStep=4;
                odo_Status(gv.modbusId, 0x0310, 0x0003, USART_1);
            } else {
                gOdoToggle=0;
                odo_RequestMeasure( gv.modbusId, 0x0000, 0x0016, USART_1);//ODO / CT 측정값 요청    
                //Dprintf("Data\n");
            }
        } else {
           odo_RequestMeasure( gv.modbusId, 0x0000, 0x0016, USART_1);//ODO / CT 측정값 요청    
        }
    }

    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	CT_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    /**
    *   gApplyCalCount = 보정적용카운트값
    */
    //IOStopTimer(DO_PONSEL_PROCESS);
    U32 fontColor = clWhite, fontBackColor = clBlack;   
    gv.odo_mode=1;//교정모드
    gv.odo_ArrayAddress=0;
    gv.modbusId=gv.odo_ArrayAddress+1;
    gCalStartFlag=0;
    gv.request_DataFlag=OFF;
	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

	//Gr->SetColor(RGB565CONVERT(230,  230,  230));
	Gr->SetColor(clBlack);
	Gr->FillRect(0, 0, win->width-2, win->height-2);
	//타이틀바를 그린다.
	TitleBar(handle, 0, 0, win->width, 50, 1, "CT 교정", clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	
	//디스플레이용 바탕패널을 그린다.
	//SystemPanel(handle, 20, 70, 800-40, 130, 1, "", clYellow, 1, RIGHT_ALIGN, 3, NULL);	


    int sy = 220, sy2 = 70;
    int hight = 60;
    int width = 300;
    
    //항목리스트
    SLabel(handle, 10, sy, width, hight, "아이디전환/공장초기화",fontColor,fontBackColor,1);
    SLabel(handle, 10, sy+sy2, width, hight, "표준버퍼 설정후 교정시작",fontColor,fontBackColor,1);
    SLabel(handle, 10, sy+sy2+sy2, width, hight, "염분설정값 / 전도도설정값",fontColor,fontBackColor,1);
    
    SLabel(handle, 10, sy+sy2+sy2+sy2, width, hight, "온도보상 / 온도보상적용",fontColor,fontBackColor,1);
	SLabel(handle, 10, sy+sy2+sy2+sy2+sy2, width, hight, "아이디읽기 / 아이디설정",fontColor,fontBackColor,1);
    
	SLabel(handle, 30, sy+sy2+sy2+sy2+sy2+sy2, width, hight, "센서아이디 변경",fontColor,fontBackColor,1);
    
    char data[256];
    for(int i=0; i<250; i++) {
        data[i]=0;
    }
	//sprintf(data,"%10.2f%",0.0);
	gv.handle2[0] = STextBox(handle, 60, 80+30, 680, 70, 48, "  ", clWhite, clBlack, CENTER_ALIGN, READ_ONLY, 0, NULL);
	//측정값 디스플레이용 텍스트박스
	gv.handle2[1]=SLabel(handle, 300, 80, 200, 30, "측정값", clWhite, RGB565CONVERT(10, 10, 10),  1);


    gv.modbusId=gv.odo_ArrayAddress+1;
    sprintf(data,"아이디.%d",gv.odo_ArrayAddress+1);
    gv.handle2[2] = SButton(handle, 310, sy, 200, hight, " ", data,  PUSH_BUTTON, 2, CT_KeyEvent);                
    gv.handle2[3] = SButton(handle, 550, sy, 200, hight, " ", "초기화",  PUSH_BUTTON, 3, CT_KeyEvent);		
    
    //sprintf(data,"%d",0);
	//gv.handle2[4] = SButton(handle, 310, 220+80, width, hight, " ", data,  PUSH_BUTTON, 4, CT_KeyEvent);			
    gv.handle2[5] = SButton(handle, 310, sy+sy2, 200, hight, " ", "염분교정",  SELECT_BUTTON, 5, CT_KeyEvent);	    
    
	sprintf(data,"%.2f",flash.odoct.saltCalPv);
    gv.handle2[6] = SButton(handle, 310, sy+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 6, CT_KeyEvent);			
    gv.handle2[7] = SButton(handle, 550, sy+sy2, 200, hight, " ", "전도도교정",  PUSH_BUTTON, 7, CT_KeyEvent);			
    
    //gv.handle2[8] = SButton(handle, 310, 220+80+80+80, 200, hight, " ", data,  PUSH_BUTTON, 8, CT_KeyEvent);
	gv.handle2[9] = SButton(handle, 550, sy+sy2+sy2+sy2, 200, hight, " ", "온도보상적용",  PUSH_BUTTON, 9, CT_KeyEvent);				
	sprintf(data,"%.1f",flash.odoct.condCalPv);    
    gv.handle2[10] = SButton(handle, 550, sy+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 10, CT_KeyEvent);
    sprintf(data,"%.1f",flash.odoct.tempCalPv);
	gv.handle2[11] = SButton(handle, 310, sy+sy2+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 11, CT_KeyEvent);				

    gv.handle2[12] = SButton(handle, 310, sy+sy2+sy2+sy2+sy2+sy2, 200, hight, " ", "아이디변경실행",  PUSH_BUTTON, 12, CT_KeyEvent);				
    
    gv.handle2[16]=SButton(handle, 550, sy+sy2+sy2+sy2+sy2+sy2, 200, hight, " ", " ",  PUSH_BUTTON, 16, 0);			
    
    SButton(handle, 550, sy+sy2+sy2+sy2+sy2+sy2+sy2, 200, hight, " ", "EXIT",  PUSH_BUTTON, 13, CT_KeyEvent);			

    sprintf(data,"%d",gNewID);
	gv.handle2[14] = SButton(handle, 310, sy+sy2+sy2+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 14, CT_KeyEvent);				
    sprintf(data,"%d",gChangeID);
    gv.handle2[15] = SButton(handle, 550, sy+sy2+sy2+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 15, CT_KeyEvent);				


	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	play_timer =  OpenTimer(parent, handle, 1000);	
    StartTimer(play_timer);  	    
	return(1);
}
int	CT_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	//Gr->FillRect(0, 0, 0, 0);//Dummy	
	//Gr->SetColor(RGB565CONVERT(230,  230,  230));
	//Gr->FillRect(0, 0, win->width-2, win->height-2);	
	PaintDrawControl(ON);		
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
int	CT_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
	int Done=1;
	while(Done) //위젯윈도우를 모두찾아 CloseEvent를 전송한다.
	{
		if(win->CloseEvent != NULL) win->CloseEvent(0, 0, 0, 0, 0);
		if(win->NextWindow ==NULL) return(NULL);
		win = (Windows *)win->NextWindow;
	}	
	return(1);
}
int CT_Window(void)
{
	if(GetWindowName("CT_Window")) 
	{
		Dprintf("CT_Window:: Windows already Create\n");
		return 0;		
	}	
	int sx = (LCD_X - 800) / 2;
	int sy = (LCD_Y - 710) / 2;	
	makeWindow(
				LCD,					// Display Section Position
				"CT_Window",		// Windows Name
				DESKTOP_WINDOW,		// Parent Window Handle
				CONTAINER_WINDOW,	// Windows Kind
				STYLE_NONE,			// Windows Style, TEECO_STYLE
				sx, sy, 			// Start Position
				800, 710,				// Size
				SHOW,					// Create after Show
				CT_CreateEvent,		// CreateEvent
				CT_PaintEvent,		// PaintEvent
				NULL,	// MouseOnEvent
				NULL,	// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				CT_TimerEvent,		// TimerEvent
				NULL,  					// ControlEvent
				NULL,					// ChangeEvent
				NULL,
				NULL,
				NULL,
				CT_CloseEvent,
				NULL,					// PARAMETER
				NULL,					// Back Image
				NULL,					// 
				NULL,					// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	return(1);
}			
#endif
