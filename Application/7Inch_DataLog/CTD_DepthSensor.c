#include "TEECO_System.h"
#include "DataLog.h"
#if defined(CTD_ENABLE)
U32 DEPTH_TimerID;
extern void FloatPoint_Process(int ch, int comPort,  char *data);
int	 DepthScreenClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {	
	Windows 	*win = GetWindow(parent, handle, "DepthScreenClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "DepthScreenClickEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(parent_win == NULL) {
		Dprintf("DepthScreenClickEvent:: Windows Error\n");
	}
	if(Gr==NULL) {
		Dprintf("DepthScreenClickEvent::GetGraphics Error\n");
		return(0);
	}	
	return(1);
}
int	 DepthScreenEditButtonEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle, "DepthScreenClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "DepthScreenClickEvent");
	if(parent_win == NULL) {
		Dprintf("DepthScreenClickEvent:: Windows Error\n");
	}
	char pBuf[20];
    for(int i=0; i<20; i++) {
        pBuf[i]=0;
    }
    
    if(win->ID==3) {        //[BUTTON].교정값설정
        gv.ulikMessageStep=1;
        if(gv.ulikCalItem==5) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "Depth m");
        }
    }
    else if(win->ID==13) {  //[BUTTON].comfirm        
        GetWidgetText(gv.handle2[3],pBuf);//항목별 교정여부  
        float fdata = atof(pBuf);        
//        if(flash.ctd.depthCalCount==0 && fdata > 0.0f) { //1점 보정 이상이면
//            if(gv.ulikCalItem==5) {
//                ULIK_MessageBox_Window("COND","0점부터 교정을 진행해야 합니다.");                
//            }
//        }        
        if(flash.ctd.depthCalCount >= 2) {
            if(gv.ulikCalItem==5) {
                ULIK_MessageBox_Window("Depth","최대 2점까지 교정이 가능합니다.");
            }
        }
        else {
            GetWidgetText(gv.handle2[3],pBuf);
            float fdata = atof(pBuf);            
            if(fdata >= 0 && gv.ulikMessageStep==1) {    
                SetWidgetStatus(gv.handle2[13],ON);
                Dprintf("교정값 = %.3f\n",fdata);
                if(gv.ulikCalItem==5) {
                    ULIK_MessageBox_Window("Depth","교정 값을 적용합니다.");
                }
            } else {
                if(gv.ulikCalItem==5) {
                    ULIK_MessageBox_Window("Depth","교정값을 설정해야 합니다.");
                }
            }
        }
    }
    else if(win->ID==15) {//ClearAllCalib 
        if(gv.ulikCalItem==5) {
            ULIK_MessageBox_Window("Depth","모든 교정 값을 초기화합니다.");
        }
    }
    else if(win->ID==16) {//EXIT
        gv.ulikCalItem=0;
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        CloseWindow(NULL, parent);
    }
    else if(win->ID==36) {
        ULIK_MessageBox_Window("Zero calibration","공통 적용 됩니다.");
    }
	return (1);
}   
int	 DepthScreenTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win  = GetWindow(parent, handle, "DepthScreenTimerEvent");				//windows 관련 정보를 얻는다.
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL) {
		Dprintf("DepthScreenTimerEvent::Windows Error\n");
		return(0);
	}
	if(Gr==NULL) {
		Dprintf("DepthScreenTimerEvent:: Graphics Error\n");
		return(0);
	}	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	int paraAddress=0;
    char pBuf[32];
    int comPort=USART_1;
    for(int i=0; i<32; i++) {
        pBuf[i]=0;
    }    
    //int addressH=0;
    //int addressL=0;
    //교정명령송신 STEP
    if(gv.ulikCalCmdSendNo==1) {            
        GetWidgetText(gv.handle2[3],pBuf);//사용자가 설정한 교정 설정값
        float fdata = atof(pBuf);
        if(gv.ulikCalItem==5) { //Depth.교정값  
            if(flash.ctd.depthCalCount==0) {
                CTD_Write(comPort, flash.ctd.id, 0x80, 0x0A, DEPTH_WRITE, fdata);//1point
                flash.ctd.depthParaValue1[0]=fdata;
                flash.ctd.depthParaValue2[0]=gv.ctdPv[7];
                flash.ctd.depthCalCount=1;//교정횟수
            }
            else if(flash.ctd.depthCalCount==1) {
                CTD_Write(comPort, flash.ctd.id, 0x80, 0x0C, DEPTH_WRITE, fdata);//2point
                flash.ctd.depthParaValue1[1]=fdata;
                flash.ctd.depthParaValue2[1]=gv.ctdPv[7];
                flash.ctd.depthCalCount=2;
            }
        }            
        gv.ulikCalCmdSendNo=0;
        SetWidgetStatus(gv.handle2[13],OFF);//BUTTON IMAGE CHANGE
    }
    else if(gv.ulikCalCmdSendNo==3) {//교정초기화
        if(gv.ulikCalItem==5) {
            if(gv.ctdCalClearStep==0) {
                CTD_Write(comPort, flash.ctd.id, 0x80, 0x0A, DEPTH_WRITE, -1.0);//1point
                gv.ctdCalClearStep=1;
                SetWidgetStatus(gv.handle2[15],ON);
            } 
            else if(gv.ctdCalClearStep==1) {
                CTD_Write(comPort, flash.ctd.id, 0x80, 0x0C, DEPTH_WRITE, -1.0);//2point
                gv.ctdCalClearStep=0;
                gv.ulikCalCmdSendNo=0;
                SetWidgetStatus(gv.handle2[15],OFF);
                flash.ctd.depthCalCount=0;
                
                flash.ctd.depthParaValue1[0]=0.0;
                flash.ctd.depthParaValue2[0]=0.0;
                flash.ctd.depthParaValue1[1]=0.0;
                flash.ctd.depthParaValue2[1]=0.0;
                flash.ctd.depthParaValue1[2]=0.0;
                flash.ctd.depthParaValue2[2]=0.0;
            }
        }
    }
    //Depth
    sprintf(pBuf,"%10.2f", gv.ctdPv[6]);    
    pBuf[10]=0;
    SetWidgetText(gv.handle2[0],pBuf);  //Depth    
    sprintf(pBuf,"%10.2f", gv.ctdPv[7]);
    pBuf[10]=0;
    SetWidgetText(gv.handle2[1],pBuf);  //Pressure
    //sprintf(pBuf,"%10.2f", gv.ctdPv[3]);
    //pBuf[10]=0;
    //SetWidgetText(gv.handle2[2],pBuf);  //SAL

    sprintf(pBuf,"%10.2f", gv.ctdPv[7]);
    pBuf[10]=0;
    SetWidgetText(gv.handle2[4],pBuf);  //Pressure    
    
    //교정값을 화면에 출력한다.    
    if(flash.ctd.depthCalCount==0) {
        paraAddress = 0;
    }    
    else if(flash.ctd.depthCalCount==1) {
        paraAddress = 0;
    }
    else if(flash.ctd.depthCalCount==2) {
        paraAddress = 0;
    }
    else if(flash.ctd.depthCalCount==3) {
        paraAddress = 1;
    }
    if(gv.ulikCalItem==5) {
        sprintf(pBuf,"%d-point calibration value", paraAddress+1);
        SetWidgetText(gv.handle2[21],pBuf);
        SetWidgetText(gv.handle2[30],"Depth m");
        SetWidgetText(gv.handle2[31],"Pressure");

        sprintf(pBuf,"%d-point calibration value", paraAddress+2);
        SetWidgetText(gv.handle2[22],pBuf);
        SetWidgetText(gv.handle2[33],"Depth m");
        SetWidgetText(gv.handle2[34],"Pressure");
    } 
    
    if(gv.ulikCalItem==5) {
        //1-point calibration value    
        sprintf(pBuf,"%10.2f", flash.ctd.depthParaValue1[paraAddress]);    
        pBuf[10]=0;
        SetWidgetText(gv.handle2[6],pBuf);  //교정값
        sprintf(pBuf,"%10.2f", flash.ctd.depthParaValue2[paraAddress]);
        pBuf[10]=0;
        SetWidgetText(gv.handle2[7],pBuf);  //측정값
        
        //2-point calibration value
        paraAddress += 1;
        sprintf(pBuf,"%10.2f", flash.ctd.depthParaValue1[paraAddress]);    
        pBuf[10]=0;
        SetWidgetText(gv.handle2[9],pBuf);
        sprintf(pBuf,"%10.2f", flash.ctd.depthParaValue2[paraAddress]);
        pBuf[10]=0;
        SetWidgetText(gv.handle2[10],pBuf); 
    } 
    if(gv.ulikMessageStep==1) {
        SetWidgetText(gv.handle2[17],"2. 측정값이 안정되면 Comfirm 터치 후 교정을 완료합니다.");
    } 
    else {
        SetWidgetText(gv.handle2[17],"1. KEY BOARD ICON 터치 후 교정 중인 버퍼나 샘플 값을 입력합니다.");
    }
    
    //교정스텝출력
    sprintf(pBuf,"CalStep.%d",flash.ctd.depthCalCount);
    SetWidgetText(gv.handle2[18],pBuf);
    
	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return 1;
}
int	 DepthScreenCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle,"SmartLogSontekConfigCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL) {
		Dprintf("DepthScreenCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL){
		Dprintf("DepthScreenCreateEvent::GetGraphics Error\n");
		return(0);
	}
    
	
	PaintDrawControl(OFF);
    Gr->FillRect(0, 0, 0, 0);//Dummy
	Gr->SetColor(flash.systemBackGroundColor);
	Gr->FillRect(0, 0, LCD_X, LCD_Y);

    //unsigned int labelBackColor = RGB565CONVERT(36, 36, 36);  //RGB565CONVERT(191, 191, 191);
    //unsigned int labelFontColor = clYellow;

    char pBuf[120];
    int gwidth=240;
    int width=240-3, height=50, side=5;
	int	sx = 10;
	int	sy = 60 + height + side;
    
    for(int i=0; i<120; i++) {
        pBuf[i]=0;
    }
    sprintf(pBuf,"%.2f", 0.0);
    pBuf[10]=0;

    //좌측화면    
    GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "측정값");
    
    Label(handle, sx, sy, width, height, "Depth m",  clBlack);
    gv.handle2[0]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  NULL, 0, NULL); 
    
    Label(handle, sx, sy+height+side, width, height, "Pressure",  clBlack);
    gv.handle2[1]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  NULL, 1, NULL); 

    //Label(handle, sx, sy+height+side+height+side, width, height, "SAL ppt",  clBlack);
    //gv.handle2[2]=SButton(handle, sx+width+side, sy+height+side+height+side, width, height, 0, pBuf,  NULL, 1, NULL); 
    
    GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "교정값(사용자설정)");
    
    Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "Depth m",  clBlack);
    gv.handle2[3]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  PUSH_BUTTON, 3, DepthScreenEditButtonEvent); //COD 사용자 설정값
    
    Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "Pressure",  clBlack);
    gv.handle2[4]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 4, DepthScreenEditButtonEvent);  //CODorg 자동설정값
    
    //우측화면
    sx = sx+width+width+side+side+side; 
    gv.handle2[21]=GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "1-point calibration value");
    gv.handle2[30]=Label(handle, sx, sy, width, height, "Depth m",  clBlack);
    gv.handle2[6]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  SELECT_BUTTON, 6, NULL); 
    gv.handle2[31]=Label(handle, sx, sy+height+side, width, height, "Pressure",  clBlack);
    gv.handle2[7]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 7, NULL); 
    
    //gv.handle2[32]=Label(handle, sx, sy+height+side+height+side, width, height, "CODreal",  clBlack);
    //gv.handle2[8]=SButton(handle, sx+width+side, sy+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 8, NULL); 
    
    gv.handle2[22]=GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "2-point calibration value");
    gv.handle2[33]=Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "Depth m",  clBlack);
    gv.handle2[9]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 9, DepthScreenEditButtonEvent); 
    gv.handle2[34]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "Pressure",  clBlack);
    gv.handle2[10]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 10, DepthScreenEditButtonEvent); 
    //gv.handle2[35]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side, width, height, "CODreal",  clBlack);
    //gv.handle2[11]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 11, DepthScreenEditButtonEvent); 
    
    //BUTTON
    sx=10;
    //gv.handle2[12]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_TestBottle_Icon6.bmp", "     Clean start",  SELECT_BUTTON, 12, DepthScreenEditButtonEvent);
    gv.handle2[13]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Save_Icon1.bmp", "Comfirm",  SELECT_BUTTON, 13, DepthScreenEditButtonEvent); 
    
    //gv.handle2[14]=SButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear Last Calib",  PUSH_BUTTON, 14, DepthScreenEditButtonEvent); 
    gv.handle2[15]=NButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear All Calib",  SELECT_BUTTON, 15, DepthScreenEditButtonEvent); 
    gv.handle2[16]=NButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Home_Icon2.bmp", "EXIT",  PUSH_BUTTON, 16, DepthScreenEditButtonEvent); 
    //Zero Calibration
    //gv.handle2[36]=SButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Retry_Icon2.bmp", "Zero Cal",  PUSH_BUTTON, 36, DepthScreenEditButtonEvent); 


    //교정메시지출력
    gv.handle2[17]=SButton(handle, 5, 10, 1000-15, height, 0, 
    "1. KEY BOARD ICON 터치 후 교정 중인 버퍼나 샘플 값을 입력합니다.",  PUSH_BUTTON, 18, NULL); 
    //교정스텝출력
    sprintf(pBuf,"CalStep.%d",flash.ctd.depthCalCount);
    gv.handle2[18]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, " ", pBuf,  SELECT_BUTTON, 18, DepthScreenEditButtonEvent); 
    //세정시간설정
    //sprintf(pBuf,"     Clean sec %d",flash.ulik.cleanTimeSv);
    //gv.handle2[19]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_TestBottle_Icon6.bmp", pBuf,  PUSH_BUTTON, 19, DepthScreenEditButtonEvent);
 
 
    DEPTH_TimerID=OpenTimer(parent, handle, 1000);
	StartTimer(DEPTH_TimerID);
	PaintDrawControl(ON);	
	return(1);
}
int	 DepthScreenPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle, "SmartLogSontekConfigPaintEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(win==NULL) 
	{
		Dprintf("SmartLogSontekConfigPaintEvent:: Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("SmartLogSontekConfigPaintEvent::GetGraphics Error\n");
		return(0);
	}	
	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
void DepthScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title) {
	WidgetStruct wid_struct;	
	wid_struct.FontNo = font_no;
	//wid_struct.CallWidget_ID = id;
	makeWindow(
				LCD,
				"DepthScreen",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				STYLE_NONE,				// Windows Style
				sx, sy, 				// Start Position
				1000, 730,			// Size
				SHOW,					// Create after Show
				DepthScreenCreateEvent,		// CreateEvent
				DepthScreenPaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				DepthScreenTimerEvent,					// TimerEvent
				NULL,  					// ControlEvent
				NULL,  					// ChangeEvent
				NULL,
				NULL,
				NULL,
				NULL,
				GRAY7,					// PARAMETER
				value,					// Normal Touch Screen
				0,						// Touch Screen
				title,					// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
				
	//ShowWindow(NULL, mainHandle);
}
#endif

