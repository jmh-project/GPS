#include "TEECO_System.h"
#include "DataLog.h"

#if defined (YSI_ODD_ENABLE)||defined(YSI_ODOCT_ENABLE)

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
int odo_Cap_Coefficient(int id, int addr, int len, int comPort);
int odo_Cap_CoefficientSet(int id, int addr, int num, int cnt, int comPort);
int odo_RequestMeasure(int id, int addr, int len, int comPort);
extern int gChangeID;
extern int gNewID;
extern int gIDChangeFlag;
int gCapUpdateStep=0;
int gWriteFlag=0;
int gReadFlag=0;
unsigned int ODO_StringChangeInit2Byte(char *buf) {
    U8      a=0, b=0, c=0, d=0, f=0;
    
    f=buf[0];
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    a = (f & 0x0F) << 4;
    f = buf[1];    
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    f = (f & 0x0F);
    a |= f;

    return a;
}
unsigned int ODO_StringChangeInit4Byte(char *buf) {
    U8      a=0, b=0, c=0, d=0, f=0;
    
    f=buf[0];
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    a = (f & 0x0F) << 4;
    f = buf[1];    
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    f = (f & 0x0F);
    a |= f;

    f=buf[2];
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    b = (f & 0x0F) << 4;
    f = buf[3];    
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    f = (f & 0x0F);
    b |= f;
    
    f=buf[4];
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    c = (f & 0x0F) << 4;
    f = buf[5];    
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    f = (f & 0x0F);
    c |= f;
    
    f=buf[6];
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    d = (f & 0x0F) << 4;
    f = buf[7];    
    if(f>=0x30&&f<=0x39) {
        f = f - 0x30;
    } else {
        switch( f ) {
            case 'A': 	f=10; break;
            case 'B': 	f=11; break;
            case 'C': 	f=12; break;
            case 'D': 	f=13; break;
            case 'E': 	f=14; break;
            case 'F': 	f=15; break;
        }
    }
    f = (f & 0x0F);
    d |= f;
    
    return ((a << 24) | (b << 16) | (c << 8) | d);
}


int	ODO_CapInfoKeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
    } else if(win->ID==3) {//EXIT
        gv.odo_mode=0;//측정모드    
        gv.request_DataFlag=ON;        
        
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        
        Dprintf("ODO 교정 데이터 저장\n");
        StopTimer(play_timer);  	        
        TimerQClear();
        CloseWindow(0, parent);        
    } else if(win->ID==4) {
        KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    } else if(win->ID==5) {
        KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    } else if(win->ID==6) {
        KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    } else if(win->ID==7) {
        KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    } else if(win->ID==8) {
        KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    } else if(win->ID==9) {
        KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    } else if(win->ID==10) {
        KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    } else if(win->ID==11) {
        KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    } else if(win->ID==12) {
        gCapUpdateStep=1;
        gReadFlag=1;
        //odo_Cap_Coefficient(gv.modbusId,0x100,0x0011,USART_1);
        Dprintf("데이터읽기\n");
    } else if(win->ID==13) {
        GetWidgetText(gv.handle2[4],(char *)buf32);
        gv.K1=ODO_StringChangeInit4Byte((char *)buf32);
        Dprintf("K1 = %08x\n",gv.K1);    
        GetWidgetText(gv.handle2[5],(char *)buf32);
        gv.K2=ODO_StringChangeInit4Byte((char *)buf32);
        Dprintf("K2 = %08x\n",gv.K2);    
        GetWidgetText(gv.handle2[6],(char *)buf32);
        gv.K3=ODO_StringChangeInit4Byte((char *)buf32);
        Dprintf("K3 = %08x\n",gv.K3);    
        GetWidgetText(gv.handle2[7],(char *)buf32);
        gv.K4=ODO_StringChangeInit4Byte((char *)buf32);
        Dprintf("K4 = %08x\n",gv.K4);    
        GetWidgetText(gv.handle2[8],(char *)buf32);
        gv.K5=ODO_StringChangeInit4Byte((char *)buf32);
        Dprintf("K5 = %08x\n",gv.K5);    
        GetWidgetText(gv.handle2[9],(char *)buf32);
        gv.K6=ODO_StringChangeInit4Byte((char *)buf32);
        Dprintf("K6 = %08x\n",gv.K6);    
        GetWidgetText(gv.handle2[10],(char *)buf32);
        gv.K7=ODO_StringChangeInit4Byte((char *)buf32);
        Dprintf("K7 = %08x\n",gv.K7);    
        GetWidgetText(gv.handle2[11],(char *)buf32);
        gv.KC=ODO_StringChangeInit2Byte((char *)buf32);
        Dprintf("KC = %04x\n",gv.KC);    
        gWriteFlag=1;
        //odo_Cap_CoefficientSet(gv.modbusId,0x100,0x0011,0x22,USART_1);
        
        Dprintf("데이터쓰기\n");
    }
    
	PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);    
	return(1);
}
int	ODO_CapInfoTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");
       
    unsigned int comPort = USART_1;
    unsigned int address = 0;
    unsigned char sumBuf[50]={0};
    char crcBuf[2]={0};
    unsigned short crc=0;
	char data64[64]={0};
    float fData=0.0;
    
    if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
    
    sprintf(data64,"%.2f'C / %.2fmg/L / %.1f%C",gv.tempPv[gv.odo_ArrayAddress],gv.odoPv[gv.odo_ArrayAddress],gv.satPv[gv.odo_ArrayAddress],0x25);    
    SetWidgetText(gv.handle2[0],data64);
    
    if(gCapUpdateStep==2){
        sprintf(data64,"%08X",gv.K1);
        data64[8]=0;
        SetWidgetText(gv.handle2[4], data64);
        sprintf(data64,"%08X",gv.K2);
        data64[8]=0;
        SetWidgetText(gv.handle2[5], data64);
        sprintf(data64,"%08X",gv.K3);
        data64[8]=0;
        SetWidgetText(gv.handle2[6], data64);
        sprintf(data64,"%08X",gv.K4);
        data64[8]=0;
        SetWidgetText(gv.handle2[7], data64);
        sprintf(data64,"%08X",gv.K5);
        data64[8]=0;
        SetWidgetText(gv.handle2[8], data64);
        sprintf(data64,"%08X",gv.K6);
        data64[8]=0;
        SetWidgetText(gv.handle2[9], data64);
        sprintf(data64,"%08X",gv.K7);
        data64[8]=0;
        SetWidgetText(gv.handle2[10], data64);
        sprintf(data64,"%02X",gv.KC);
        data64[2]=0;
        SetWidgetText(gv.handle2[11], data64);
        gCapUpdateStep=0;
    }
    if(gWriteFlag==1) {
        gWriteFlag=0;
        odo_Cap_CoefficientSet(gv.modbusId,0x100,0x0011,0x22,USART_1);
    }
    else if(gReadFlag==1) {
        gReadFlag=0;
        odo_Cap_Coefficient(gv.modbusId,0x100,0x0011,USART_1);
    }
    else if(gv.request_DataFlag==OFF) {
        odo_RequestMeasure( gv.modbusId, 0x0000, 0x0016, USART_1);//ODO / CT 측정값 요청    
    }
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	ODO_CapInfoCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    //IOStopTimer(DO_PONSEL_PROCESS);
       
    gv.odo_mode=1;//교정모드
    gv.odo_ArrayAddress=0;
    gv.modbusId=gv.odo_ArrayAddress+1;
    gv.request_DataFlag=OFF;
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

	Gr->SetColor(clBlack);
	Gr->FillRect(0, 0, win->width-2, win->height-2);
	//타이틀바를 그린다.
	TitleBar(handle, 0, 0, win->width, 50, 1, "ODO Cap Replacement", clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	
	//디스플레이용 바탕패널을 그린다.
	//SystemPanel(handle, 20, 70, 800-40, 130, 1, "", clYellow, 1, RIGHT_ALIGN, 3, NULL);	

    //항목리스트
    SLabel(handle, 10, 220, 200+40, 64, "아이디전환",clWhite,clBlack,1);
    SLabel(handle, 10, 220+80, 200+40+40, 64, "CAP K1 / CAP K2",clWhite,clBlack,1);
    SLabel(handle, 10, 220+80+80, 200+40+40, 64, "CAP K3 / CAP K4",clWhite,clBlack,1);
    SLabel(handle, 10, 220+80+80+80, 200+40+40, 64, "CAP K5 / CAP K6",clWhite,clBlack,1);
	SLabel(handle, 10, 220+80+80+80+80, 200+40+40, 64, "CAP K7 / CAP K8",clWhite,clBlack,1);
    
	//SLabel(handle, 30, 220+80+80+80+80+80, 200, 64, "센서아이디 변경",clBlack,RGB565CONVERT(230, 230, 230),1);
    
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
    gv.handle2[2] = NButton(handle, 310, 220, 200, 64, " ", data,  PUSH_BUTTON, 2, ODO_CapInfoKeyEvent);                
    gv.handle2[3] = NButton(handle, 550, 220, 200, 64, " ", "EXIT",  PUSH_BUTTON, 3, ODO_CapInfoKeyEvent);		
    
    sprintf(data,"%d",0);
	gv.handle2[4] = NButton(handle, 310, 220+80, 200, 64, " ", data,  PUSH_BUTTON, 4, ODO_CapInfoKeyEvent);			
    gv.handle2[5] = NButton(handle, 550, 220+80, 200, 64, " ", data,  PUSH_BUTTON, 5, ODO_CapInfoKeyEvent);	    
    
    gv.handle2[6] = NButton(handle, 310, 220+80+80, 200, 64, " ", data,  PUSH_BUTTON, 6, ODO_CapInfoKeyEvent);			
    gv.handle2[7] = NButton(handle, 550, 220+80+80, 200, 64, " ", data,  PUSH_BUTTON, 7, ODO_CapInfoKeyEvent);			
    
    gv.handle2[8] = NButton(handle, 310, 220+80+80+80, 200, 64, " ", data,  PUSH_BUTTON, 8, ODO_CapInfoKeyEvent);
	gv.handle2[9] = NButton(handle, 550, 220+80+80+80, 200, 64, " ", data,  PUSH_BUTTON, 9, ODO_CapInfoKeyEvent);				
    
    gv.handle2[10] = NButton(handle, 310, 220+80+80+80+80, 200, 64, " ", data,  PUSH_BUTTON, 10, ODO_CapInfoKeyEvent);
	gv.handle2[11] = NButton(handle, 550, 220+80+80+80+80, 200, 64, " ", data,  PUSH_BUTTON, 11, ODO_CapInfoKeyEvent);				
    
	gv.handle2[12] = NButton(handle, 310, 220+80+80+80+80+80, 200, 64, " ", "읽기",  PUSH_BUTTON, 12, ODO_CapInfoKeyEvent);				
    gv.handle2[13] = NButton(handle, 550, 220+80+80+80+80+80, 200, 64, " ", "쓰기",  PUSH_BUTTON, 13, ODO_CapInfoKeyEvent);				

	//gv.handle2[14] = SButton(handle, 310, 220+80+80+80+80+80+80, 200, 64, " ", "읽기",  PUSH_BUTTON, 14, ODO_CapInfoKeyEvent);				
    //gv.handle2[15] = SButton(handle, 550, 220+80+80+80+80+80+80, 200, 64, " ", "EXIT",  PUSH_BUTTON, 15, ODO_CapInfoKeyEvent);				

	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	play_timer =  OpenTimer(parent, handle, 1000);	
    StartTimer(play_timer);  	    
	return(1);
}
int	ODO_CapInfoPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
int	ODO_CapInfoCloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
int ODO_CapInfoWindow(void)
{
	if(GetWindowName("ODO_CapInfoWindow")) 
	{
		Dprintf("ODO_CapInfoWindow:: Windows already Create\n");
		return 0;		
	}	
	int sx = (LCD_X - 800) / 2;
	int sy = (LCD_Y - 710) / 2;	
	makeWindow(
				LCD,					// Display Section Position
				"ODO_CapInfoWindow",		// Windows Name
				DESKTOP_WINDOW,		// Parent Window Handle
				CONTAINER_WINDOW,	// Windows Kind
				STYLE_NONE,			// Windows Style, TEECO_STYLE
				sx, sy, 			// Start Position
				800, 710,				// Size
				SHOW,					// Create after Show
				ODO_CapInfoCreateEvent,		// CreateEvent
				ODO_CapInfoPaintEvent,		// PaintEvent
				NULL,	// MouseOnEvent
				NULL,	// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				ODO_CapInfoTimerEvent,		// TimerEvent
				NULL,  					// ControlEvent
				NULL,					// ChangeEvent
				NULL,
				NULL,
				NULL,
				ODO_CapInfoCloseEvent,
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
