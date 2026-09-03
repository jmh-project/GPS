#include "TEECO_System.h"
#include "DataLog.h"
#include "time.h"

int gIDChangeFlag=0;
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
int odo_SlaveAddressGet(int id, int addr, int setAddress, int comPort);
int odo_SlaveAddressSet(int id, int addr, int setAddress, int comPort);
int odo_ZeroCal(int id, int addr, int num, unsigned char cnt, int item, int comPort );
int odo_FactoryReset(int id, int addr, int len, int comPort);
int odo_mgLCal(int id, int addr, int num, unsigned char cnt, int item, int comPort );
int odo_AirCal(int id, int addr, int num, unsigned char cnt, int item, int comPort );
int odo_SalinitySet(int id, int addr, int num, unsigned char cnt, int item, int comPort );
int odo_RequestMeasure(int id, int addr, int len, int comPort);
int odo_Status(int id, int addr, int len, int comPort);
int gNextPointStart = 0;
int gCalStartFlag = 0;
int gChangeID=1;
int gNewID=0;
int gZeroCalStartFlag=0;
int gSpanCalStartFlag=0;
int gPercentCalStartFlag=0;
int gSalCalStrartFlag=0;
int gGodoCalStartFlag=0;
extern int gCapUpdateStep;
time_t baseTime;

int	ODO_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
    if(win->ID==2) {//ID 변경
        gv.odo_ArrayAddress++;
        if(gv.odo_ArrayAddress >= 9) {
            gv.odo_ArrayAddress = 0;
        }
        sprintf((char *)buf32,"아이디.%d",gv.odo_ArrayAddress+1);//배열주소  
        gv.modbusId=gv.odo_ArrayAddress+1;//센서주소
        SetWidgetText(gv.handle2[win->ID], (char *)buf32);
        Dprintf("MODBUS DeviceAddress = 0x%02x\n",gv.modbusId);
    } else if(win->ID==3) {//교정초기화
        odo_FactoryReset(gv.modbusId, 0x0200, 0x0001, USART_1);
        Dprintf("교정초기화\n");
    } else if(win->ID==5) {//제로교정
        if(gCalStartFlag==0) {
            gCalStartFlag = 1;
            SetWidgetStatus(gv.handle2[win->ID],ON);
        } else if(gCalStartFlag==1) {
            gCalStartFlag=0;
            GetWidgetText(gv.handle2[6],(char *)buf32);
            flash.odoct.odoCalPv=atof((char *)buf32);
            GetWidgetText(gv.handle2[10],(char *)buf32);
            flash.odoct.salPv=atof((char *)buf32);
            GetWidgetText(gv.handle2[11],(char *)buf32);
            flash.odoct.barometerPv=atof((char *)buf32);
            gZeroCalStartFlag=1;
            //odo_ZeroCal(gv.modbusId, 0x0220, 0x0002, 0x04, 1, USART_1);
            SetWidgetStatus(gv.handle2[win->ID],OFF);
            Dprintf("ZERO 교정시작\n");
        }
    } else if(win->ID==6) {//포화설정값
        KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "포화설정값");
        Dprintf("교정값(mg/L 설정)\n");
    } else if(win->ID==7) {//포화교정
        if(gCalStartFlag==0) {
            GetWidgetText(gv.handle2[6],(char *)buf32);
            flash.odoct.odoCalPv=atof((char *)buf32);
            if(flash.odoct.odoCalPv<=0) {
                DataLog_MessageBox_Window("포화교정", "포화설정값, 설정해야합니다.");
            } else {
                gCalStartFlag = 2;
                SetWidgetStatus(gv.handle2[win->ID],ON);
            }
        } else if(gCalStartFlag==2) {
            gCalStartFlag=0;
            GetWidgetText(gv.handle2[6],(char *)buf32);
            flash.odoct.odoCalPv=atof((char *)buf32);
            Dprintf("mg/L Calibration Value = %.2f\n",flash.odoct.odoCalPv);
            GetWidgetText(gv.handle2[10],(char *)buf32);
            flash.odoct.salPv=atof((char *)buf32);
            Dprintf("Salinity = %.1f\n",flash.odoct.salPv);
            GetWidgetText(gv.handle2[11],(char *)buf32);
            flash.odoct.barometerPv=atof((char *)buf32);
            Dprintf("Baro mmHg = %.1f\n",flash.odoct.barometerPv);
            gSpanCalStartFlag=1;
            //odo_mgLCal(gv.modbusId, 0x0240, 0x0006, 0x0C, 1, USART_1);
            SetWidgetStatus(gv.handle2[win->ID],OFF);
            Dprintf("SPAN 교정시작\n");
        }
    } else if(win->ID==9) {//대기중교정 명령송신
        GetWidgetText(gv.handle2[11],(char *)buf32);
        flash.odoct.barometerPv=atof((char *)buf32);
        Dprintf("Baro mmHg = %.1f\n",flash.odoct.barometerPv);
        if(gCalStartFlag==0) {             
            if(flash.odoct.barometerPv<=0) {
                DataLog_MessageBox_Window("대기중교정", "대기압(mmHg) 설정해야합니다. 760mmHg");
            } else {
                gCalStartFlag = 3;
                SetWidgetStatus(gv.handle2[win->ID],ON);
				Dprintf("대기중 교정시작\n");
            }
        } else if(gCalStartFlag==3) {
            gCalStartFlag=0;
            GetWidgetText(gv.handle2[6],(char *)buf32);
            flash.odoct.odoCalPv=atof((char *)buf32);
            Dprintf("mg/L Calibration Value = %.2f\n",flash.odoct.odoCalPv);
            GetWidgetText(gv.handle2[10],(char *)buf32);
            flash.odoct.salPv=atof((char *)buf32);
            Dprintf("Salinity = %.1f\n",flash.odoct.salPv);
            GetWidgetText(gv.handle2[11],(char *)buf32);
            flash.odoct.barometerPv=atof((char *)buf32);
            Dprintf("Baro mmHg = %.1f\n",flash.odoct.barometerPv);
            gPercentCalStartFlag=1;
            //odo_AirCal(gv.modbusId, 0x0230, 0x0004, 0x08, 1, USART_1);
            SetWidgetStatus(gv.handle2[win->ID],OFF);
            Dprintf("Percent Saturation 교정시작\n");
        }
    } else if(win->ID==10) {
        KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "교정값");
        Dprintf("염분(ppt 설정)\n");
    } else if(win->ID==11) {
        KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "mmHg");
        Dprintf("기압(mmHg 설정)\n");
    } else if(win->ID==13) {//close
        gZeroCalStartFlag=0;
        gSpanCalStartFlag=0;
        gPercentCalStartFlag=0;
        gSalCalStrartFlag=0;
        gGodoCalStartFlag=0;
        
        GetWidgetText(gv.handle2[6],(char *)buf32);
        flash.odoct.odoCalPv=atof((char *)buf32);
        GetWidgetText(gv.handle2[10],(char *)buf32);
        flash.odoct.salPv=atof((char *)buf32);
        GetWidgetText(gv.handle2[11],(char *)buf32);
        flash.odoct.barometerPv=atof((char *)buf32);
		
        gv.odo_mode=0;//측정모드       
        gv.request_DataFlag=ON;//IOPROCESS START         
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        
        Dprintf("ODO 교정 데이터 저장\n");
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
    } else if(win->ID==16) {                                            //염분교정적용        
        GetWidgetText(gv.handle2[10],(char *)buf32);
        flash.odoct.salPv=atof((char *)buf32);
        gSalCalStrartFlag=1;
        //odo_SalinitySet(gv.modbusId, 0x400, 0x0002, 0x04, 1, USART_1);//염분적용
        Dprintf("염분보정실행 = %.1f\n",flash.odoct.salPv);
    } else if(win->ID==17) {
        GetWidgetText(gv.handle2[11],(char *)buf32);
        flash.odoct.barometerPv=atof((char *)buf32);
        gGodoCalStartFlag=1;
        //odo_SalinitySet(gv.modbusId, 0x404, 0x0002, 0x04, 2, USART_1);//고도적용
        Dprintf("대기보정실행 = %.1f\n",flash.odoct.barometerPv);
    }
	PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);    
	return(1);
}
int GetTimeT(int year, int month, int day, int hour, int minute, int second) {  
    struct tm t = {0};  
    t.tm_year = year - 1900;  
    t.tm_mon = month - 1;  
    t.tm_mday = day;  
    t.tm_hour = hour;  
    t.tm_min = minute;  
    t.tm_sec = second;  
  
    return mktime(&t);  
}  
int gOdoToggle=0;
int	ODO_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");
    
    
    //unsigned int comPort = USART_1;
    //unsigned int address = 0;
    //unsigned char sumBuf[50];
    //char crcBuf[2];
    //unsigned short crc=0;
	char data64[64];
    //float fData=0.0;
    
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
    SetWidgetText(gv.handle2[17], data64);
    
//    if(gv.qcScoreStatus==0x00) {    
//        SetWidgetText(gv.handle2[18], "Best");
//    } else if(gv.qcScoreStatus==0x01) {    
//        SetWidgetText(gv.handle2[18], "OK");
//    } else if(gv.qcScoreStatus==0x02) {    
//        SetWidgetText(gv.handle2[18], "Bad");
//    } else {
//        SetWidgetText(gv.handle2[18], "Reading");
//    }
    //Dprintf("CAL TIME = %d / %s\n",gv.qcScoreTime, data64);

    sprintf(data64,"%.2f'C / %.2fmg/L / %.1f%c",gv.tempPv[gv.odo_ArrayAddress],gv.odoPv[gv.odo_ArrayAddress],gv.satPv[gv.odo_ArrayAddress],0x25);
    //strcat(data64,"%");
    SetWidgetText(gv.handle2[0],data64);
    
    sprintf(data64,"%d",gNewID);
    SetWidgetText(gv.handle2[14],data64);

    if(gIDChangeFlag==1) {
        gIDChangeFlag=0;
        Dprintf("ID 설정변경송신");
        odo_SlaveAddressSet(gv.modbusId,0x0000,gChangeID,USART_1);//ID 설정변경
    }
    else if(gZeroCalStartFlag==1) {
        gZeroCalStartFlag=0;
        Dprintf("제로교정명령송신\n");
        odo_ZeroCal(gv.modbusId, 0x0220, 0x0002, 0x04, 1, USART_1);//ZERO 교정
    }
    else if(gSpanCalStartFlag==1) {
        gSpanCalStartFlag=0;
        odo_mgLCal(gv.modbusId, 0x0240, 0x0006, 0x0C, 1, USART_1);//mg/L교정
    }
    else if(gPercentCalStartFlag==1) {
        gPercentCalStartFlag=0;
        odo_AirCal(gv.modbusId, 0x0230, 0x0004, 0x08, 1, USART_1);
        Dprintf("포화교정명령송신\n");
    }
    if(gSalCalStrartFlag==1) {
        gSalCalStrartFlag=0;
        odo_SalinitySet(gv.modbusId, 0x400, 0x0002, 0x04, 1, USART_1);//염분적용
        Dprintf("염분적용 명령 송신\n");
    }
    else if(gGodoCalStartFlag==1) {
        gGodoCalStartFlag=0;
        odo_SalinitySet(gv.modbusId, 0x404, 0x0002, 0x04, 2, USART_1);//고도적용
        Dprintf("고도교정 명령 송신\n");
    }
    else if(gv.request_DataFlag==OFF) {
        if(gCalStartFlag==0) {
            if(gOdoToggle==0) {
                gOdoToggle=1;
                //Dprintf("QC Score\n");
                gCapUpdateStep=4;
                odo_Status(gv.modbusId, 0x0210, 0x0003, USART_1);
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

int	ODO_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
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
	//Gr->SetColor(flash.systemBackGroundColor);
	//Gr->FillRect(0, 0, win->width, win->height);
	
	//타이틀바를 그린다.
	TitleBar(handle, 0, 0, win->width, 50, 1, "ODO 교정", clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	
	//디스플레이용 바탕패널을 그린다.
	//SystemPanel(handle, 20, 70, 800-40, 130, 1, "", clYellow, 1, RIGHT_ALIGN, 3, NULL);	

    int sy = 210, sy2 = 61;
    int hight = 54;
    int width = 300;

    //항목리스트
    SLabel(handle, 10, sy, width, hight, "아이디전환/공장초기화",fontColor,fontBackColor,1);
    SLabel(handle, 10, sy+sy2, width, hight, "제로 / 포화 교정",fontColor,fontBackColor,1);
    SLabel(handle, 10, sy+sy2+sy2, width, hight, "포화설정값 / 염분설정값",fontColor,fontBackColor,1);
    
    SLabel(handle, 10, sy+sy2+sy2+sy2, width, hight, "대기압(mmHg) / 대기중교정",fontColor,fontBackColor,1);
	SLabel(handle, 10, sy+sy2+sy2+sy2+sy2, width, hight, "아이디읽기 / 아이디설정",fontColor,fontBackColor,1);
    
	SLabel(handle, 30, sy+sy2+sy2+sy2+sy2+sy2, width, hight, "센서아이디 변경 / 교정시간",fontColor,fontBackColor,1);
    
    SLabel(handle, 30, sy+sy2+sy2+sy2+sy2+sy2+sy2, width, hight, "염분 교정값",fontColor,fontBackColor,1);

    SLabel(handle, 30, sy+sy2+sy2+sy2+sy2+sy2+sy2+sy2, width+180, hight, "ODO/CT복합센서는 염분교정값이 자동적용됩니다.",fontColor,fontBackColor,1);

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
    gv.handle2[2] = NButton(handle, 310, sy, 200, hight, " ", data,  PUSH_BUTTON, 2, ODO_KeyEvent);                
    gv.handle2[3] = NButton(handle, 550, sy, 200, hight, " ", "초기화",  PUSH_BUTTON, 3, ODO_KeyEvent);		
    
    //sprintf(data,"%d",0);
	//gv.handle2[4] = SButton(handle, 310, sy+80, 200, hight, " ", data,  PUSH_BUTTON, 4, ODO_KeyEvent);			
    gv.handle2[5] = NButton(handle, 310, sy+sy2, 200, hight, " ", "제로교정",  SELECT_BUTTON, 5, ODO_KeyEvent);	    
    
	sprintf(data,"%.2f",flash.odoct.odoCalPv);
    gv.handle2[6] = NButton(handle, 310, sy+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 6, ODO_KeyEvent);			
    gv.handle2[7] = NButton(handle, 550, sy+sy2, 200, hight, " ", "포화교정",  SELECT_BUTTON, 7, ODO_KeyEvent);			
    
    //gv.handle2[8] = SButton(handle, 310, sy+80+80+80, 200, hight, " ", data,  PUSH_BUTTON, 8, ODO_KeyEvent);
	gv.handle2[9] = NButton(handle, 550, sy+sy2+sy2+sy2, 200, hight, " ", "대기중교정",  SELECT_BUTTON, 9, ODO_KeyEvent);				
	sprintf(data,"%.1f",flash.odoct.salPv);    
    gv.handle2[10] = NButton(handle, 550, sy+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 10, ODO_KeyEvent);
    sprintf(data,"%.1f",flash.odoct.barometerPv);
	gv.handle2[11] = NButton(handle, 310, sy+sy2+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 11, ODO_KeyEvent);				
	gv.handle2[12] = NButton(handle, 310, sy+sy2+sy2+sy2+sy2+sy2, 200, hight, " ", "아이디변경실행",  PUSH_BUTTON, 12, ODO_KeyEvent);				
    
    
    NButton(handle, 550, sy+sy2+sy2+sy2+sy2+sy2+sy2+sy2, 200, hight, " ", "EXIT",  PUSH_BUTTON, 13, ODO_KeyEvent);			


    sprintf(data,"%d",gNewID);
	gv.handle2[14] = NButton(handle, 310, sy+sy2+sy2+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 14, ODO_KeyEvent);				
    sprintf(data,"%d",gChangeID);
    gv.handle2[15] = NButton(handle, 550, sy+sy2+sy2+sy2+sy2, 200, hight, " ", data,  PUSH_BUTTON, 15, ODO_KeyEvent);				
    gv.handle2[16] = NButton(handle, 550, sy+sy2+sy2+sy2+sy2+sy2+sy2, 200, hight, " ", "염분교정적용",  PUSH_BUTTON, 16, ODO_KeyEvent);
    
    gv.handle2[17] = NButton(handle, 550, sy+sy2+sy2+sy2+sy2+sy2, 200, hight, " ", " ",  PUSH_BUTTON, 17, 0);//교정일자				
    //gv.handle2[18] = SButton(handle, 310, sy+sy2+sy2+sy2+sy2+sy2+sy2, 200, hight, " ", " ",  PUSH_BUTTON, 18, 0);//QC Score				


	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	play_timer =  OpenTimer(parent, handle, 1000);	
    StartTimer(play_timer);  	    
	return(1);
}
int	ODO_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
//	Gr->FillRect(0, 0, 0, 0);//Dummy	
//	Gr->SetColor(RGB565CONVERT(230,  230,  230));
//	Gr->FillRect(0, 0, win->width-2, win->height-2);	
	PaintDrawControl(ON);		
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
int	ODO_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
int ODO_Window(void)
{
	if(GetWindowName("ODO_Window")) 
	{
		Dprintf("ODO_Window:: Windows already Create\n");
		return 0;		
	}	
	int sx = (LCD_X - 800) / 2;
	int sy = (LCD_Y - 710) / 2;	
	makeWindow(
				LCD,					// Display Section Position
				"ODO_Window",		// Windows Name
				DESKTOP_WINDOW,		// Parent Window Handle
				CONTAINER_WINDOW,	// Windows Kind
				STYLE_NONE,			// Windows Style, TEECO_STYLE
				sx, sy, 			// Start Position
				800, 710,				// Size
				SHOW,					// Create after Show
				ODO_CreateEvent,		// CreateEvent
				ODO_PaintEvent,		// PaintEvent
				NULL,	// MouseOnEvent
				NULL,	// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				ODO_TimerEvent,		// TimerEvent
				NULL,  					// ControlEvent
				NULL,					// ChangeEvent
				NULL,
				NULL,
				NULL,
				ODO_CloseEvent,
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
