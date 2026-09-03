#include "TEECO_System.h"
#include "DataLog.h"

#ifdef SPECTRO_M_ENABLE
U32 ULIK_TimerID;
//U32 gv.handle2[10];
extern void FloatPoint_Process(int ch, int comPort,  char *data);
extern void ULIKScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
extern int ULIK_MessageBox_Window(char *title, char *text);
int ulikCleanCount=0;//minute count
int gWaitMessageFlag=OFF;
int ulikZeroCalibrationFlag=OFF;
int	 ULIKScreenClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {	
	Windows 	*win = GetWindow(parent, handle, "ULIKScreenClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "ULIKScreenClickEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(parent_win == NULL) {
		Dprintf("ULIKScreenClickEvent:: Windows Error\n");
	}
	if(Gr==NULL) {
		Dprintf("ULIKScreenClickEvent::GetGraphics Error\n");
		return(0);
	}	
	return(1);
}
int	 ULIKScreenEditButtonEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle, "ULIKScreenClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "ULIKScreenClickEvent");
	if(parent_win == NULL)
	{
		Dprintf("ULIKScreenClickEvent:: Windows Error\n");
	}
	char pBuf[20];
    for(int i=0; i<20; i++) {
        pBuf[i]=0;
    }
    
    if(win->ID==3) {        //[BUTTON].교정값설정
        gv.ulikMessageStep=1;
        if(gv.ulikCalItem==0) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "COD");
        }
        else if(gv.ulikCalItem==1) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "TOC");
        }
        else if(gv.ulikCalItem==2) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "NO3_N");
        }
        else if(gv.ulikCalItem==3) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "TUR");
        }
    }
	else if(win->ID==4)  {
        if(gv.ulikCalItem==0) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "COD");
        }
        else if(gv.ulikCalItem==1) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "TOC");
        }
        else if(gv.ulikCalItem==2) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "NO3_N");
        }
        else if(gv.ulikCalItem==3) {
            KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "TUR");
        }
	}
    else if(win->ID==12) {  //[BUTTON].Clean start
        Dprintf("세정시작\n");
        Ulik_MW_WindowCleaning(USART_1,flash.ulik.id,0x0F);
        SetWidgetStatus(gv.handle2[12],ON);
        ulikCleanCount=20;//sec
    }
    else if(win->ID==13) {  //[BUTTON].comfirm        
        GetWidgetText(gv.handle2[3],pBuf);//항목별 교정여부  
        float fdata = atof(pBuf);        
        if(gv.ulikCalStep==0 && fdata > 0.0f) { //1점 보정 이상이면
            if(gv.ulikCalItem==0) {
                ULIK_MessageBox_Window("COD","0점부터 교정을 진행해야 합니다.");                
            }
            else if(gv.ulikCalItem==1) {
                ULIK_MessageBox_Window("TOC","0점부터 교정을 진행해야 합니다.");
            }
            else if(gv.ulikCalItem==2) {
                ULIK_MessageBox_Window("NO3_N","0점부터 교정을 진행해야 합니다.");
            }
            else if(gv.ulikCalItem==3) {
                ULIK_MessageBox_Window("TUR","0점부터 교정을 진행해야 합니다.");
            }
        }        
        else if(gv.ulikCalStep >= 5) {
            if(gv.ulikCalItem==0) {
                ULIK_MessageBox_Window("COD","최대 5점까지 교정이 가능합니다.");
            }
            else if(gv.ulikCalItem==1) {
                ULIK_MessageBox_Window("TOC","최대 5점까지 교정이 가능합니다.");
            }
            else if(gv.ulikCalItem==2) {
                ULIK_MessageBox_Window("NO3_N","최대 5점까지 교정이 가능합니다.");
            }
            else if(gv.ulikCalItem==3) {
                ULIK_MessageBox_Window("TUR","최대 5점까지 교정이 가능합니다.");
            }
        }
        else {
            GetWidgetText(gv.handle2[3],pBuf);
            float fdata = atof(pBuf);            
            if(fdata >= 0 && gv.ulikMessageStep==1) {    
                SetWidgetStatus(gv.handle2[13],ON);
                //GetWidgetText(gv.handle2[3],pBuf);
                //float fdata = atof(pBuf);
                Dprintf("교정값 = %.3f\n",fdata);
                if(gv.ulikCalItem==0) {
                    ULIK_MessageBox_Window("COD","교정 값을 적용합니다.");
                }
                else if(gv.ulikCalItem==1) {
                    ULIK_MessageBox_Window("TOC","교정 값을 적용합니다.");
                }
                else if(gv.ulikCalItem==2) {
                    ULIK_MessageBox_Window("NO3_N","교정 값을 적용합니다.");
                }
                else if(gv.ulikCalItem==3) {
                    ULIK_MessageBox_Window("TUR","교정 값을 적용합니다.");
                }
            } else {
                if(gv.ulikCalItem==0) {
                    ULIK_MessageBox_Window("COD","COD 값을 설정해야 합니다.");
                }
                else if(gv.ulikCalItem==1) {
                    ULIK_MessageBox_Window("TOC","TOC 값을 설정해야 합니다.");
                }
                else if(gv.ulikCalItem==2) {
                    ULIK_MessageBox_Window("NO3_N","NO3_N 값을 설정해야 합니다.");
                }
                else if(gv.ulikCalItem==3) {
                    ULIK_MessageBox_Window("TUR","TUR 값을 설정해야 합니다.");
                }
            }
        }
    }
    else if(win->ID==15) {  //ClearAllCalib 
        if(gv.ulikCalItem==0) {
            ULIK_MessageBox_Window("COD","모든 교정 값을 초기화합니다.");
        }
        else if(gv.ulikCalItem==1) {
            ULIK_MessageBox_Window("TOC","모든 교정 값을 초기화합니다.");
        }
        else if(gv.ulikCalItem==2) {
            ULIK_MessageBox_Window("NO3_N","모든 교정 값을 초기화합니다.");
        }
        else if(gv.ulikCalItem==3) {
            ULIK_MessageBox_Window("TUR","모든 교정 값을 초기화합니다.");
        }
    }
    else if(win->ID==19) {	//세척주기 sec
        KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "세척주기(초)");
    }
    else if(win->ID==16) {	//세정주기
        GetWidgetText(gv.handle2[19],pBuf);
        if(pBuf[0]!=0x20) {
            flash.ulik.cleanTimeSv=atoi(pBuf);
        }
        Dprintf("센서세정시간 %d sec\n",flash.ulik.cleanTimeSv);
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        CloseWindow(NULL, parent);
    }
    else if(win->ID==36) {
        ULIK_MessageBox_Window("Zero calibration","공통 적용 됩니다.");
    }
	return (1);
}   
int	 ULIKScreenTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win  = GetWindow(parent, handle, "ULIKScreenTimerEvent");				//windows 관련 정보를 얻는다.
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL) {
		Dprintf("ULIKScreenTimerEvent::Windows Error\n");
		return(0);
	}
	if(Gr==NULL) {
		Dprintf("ULIKScreenTimerEvent:: Graphics Error\n");
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
    int addressH=0;
    int addressL=0;
    //교정명령송신 STEP
    if(gv.ulikCalCmdSendNo==1) {            
        
		GetWidgetText(gv.handle2[3],pBuf);//사용자가 설정한 교정 설정값
        float fdata = atof(pBuf);
		
        if(gv.ulikCalItem==0) { //COD.교정값  
            fdata += 0.0001f;
            if(gv.ulikCalStep==0) {
                addressH=0x01;
                addressL=0x06;
            } else if(gv.ulikCalStep==1) {
                addressH=0x01;
                addressL=0x0A;
            } else if(gv.ulikCalStep==2) {
                addressH=0x01;
                addressL=0x0E;
            } else if(gv.ulikCalStep==3) {
                addressH=0x01;
                addressL=0x12;
            } else if(gv.ulikCalStep==4) {
                addressH=0x01;
                addressL=0x16;
            }
            Ulik_MW_COD_CalParaSet(comPort, flash.ulik.id, fdata, addressH, addressL);
            gv.ulikCalCmdSendNo=4;
        }
        else if(gv.ulikCalItem==1) {//TOC.교정값
            fdata += 0.0001f;
            if(gv.ulikCalStep==0) {
                addressH=0x01;
                addressL=0x42;
            } else if(gv.ulikCalStep==1) {
                addressH=0x01;
                addressL=0x46;
            } else if(gv.ulikCalStep==2) {
                addressH=0x01;
                addressL=0x4A;
            } else if(gv.ulikCalStep==3) {
                addressH=0x01;
                addressL=0x4E;
            } else if(gv.ulikCalStep==4) {
                addressH=0x01;
                addressL=0x52;
            }
            Ulik_MW_TOC_CalParaSet(comPort, flash.ulik.id, fdata, addressH, addressL);
            gv.ulikCalCmdSendNo=4;//교정파라메터송신 다음단계
        }
        else if(gv.ulikCalItem==2) {//NO3_N.교정값
            fdata += 0.0001f;
            if(gv.ulikCalStep==0) {
                addressH=0x01;
                addressL=0x2E;
            } else if(gv.ulikCalStep==1) {
                addressH=0x01;
                addressL=0x32;
            } else if(gv.ulikCalStep==2) {
                addressH=0x01;
                addressL=0x36;
            } else if(gv.ulikCalStep==3) {
                addressH=0x01;
                addressL=0x3A;
            } else if(gv.ulikCalStep==4) {
                addressH=0x01;
                addressL=0x3E;
            }
            Ulik_MW_NO3_CalParaSet(comPort, flash.ulik.id, fdata, addressH, addressL);
            gv.ulikCalCmdSendNo=4;
        }
        else if(gv.ulikCalItem==3) {//TUR . 교정값
            fdata += 0.0001f;
            if(gv.ulikCalStep==0) {
                addressH=0x01;
                addressL=0x1A;
            } else if(gv.ulikCalStep==1) {
                addressH=0x01;
                addressL=0x1E;
            } else if(gv.ulikCalStep==2) {
                addressH=0x01;
                addressL=0x22;
            } else if(gv.ulikCalStep==3) {
                addressH=0x01;
                addressL=0x26;
            } else if(gv.ulikCalStep==4) {
                addressH=0x01;
                addressL=0x2A;
            }
            Ulik_MW_TUR_CalParaSet(comPort, flash.ulik.id, fdata, addressH, addressL);
            gv.ulikCalCmdSendNo=4;
        }
        Dprintf("교정(설정 %.3f)명령 송신완료\n",fdata);              
    }    
    else if(gv.ulikCalCmdSendNo==3) {//교정값 삭제명령
        gv.ulikCalCmdSendNo=0;
        if(gv.ulikCalItem==0) {
            Ulik_MW_COD_CalParaClear(comPort, flash.ulik.id);
            Dprintf("COD 교정 데이터 전부 삭제한다.\n");
        }
        else if(gv.ulikCalItem==1) {
            Ulik_MW_TOC_CalParaClear(comPort, flash.ulik.id);
            Dprintf("TOC 교정 데이터 전부 삭제한다.\n");
        }
        else if(gv.ulikCalItem==2) {
            Ulik_MW_NO3_CalParaClear(comPort, flash.ulik.id);
            Dprintf("NO3_N 교정 데이터 전부 삭제한다.\n");
        }
        else if(gv.ulikCalItem==3) {
            Ulik_MW_TUR_CalParaClear(comPort, flash.ulik.id);
            Dprintf("TUR 교정 데이터 전부 삭제한다.\n");
        }
        Dprintf("교정(삭제)명령 송신완료\n");
    }
    else if(gv.ulikCalCmdSendNo==4) {//COD.org 교정값 송신  
        
		GetWidgetText(gv.handle2[4],pBuf);  
        float fdata = atof(pBuf);
        
		if(gv.ulikCalItem==0) {
            if(gv.ulikCalStep==0) {
                addressH=0x01;
                addressL=0x04;
            } else if(gv.ulikCalStep==1) {
                addressH=0x01;
                addressL=0x08;
            } else if(gv.ulikCalStep==2) {
                addressH=0x01;
                addressL=0x0C;
            } else if(gv.ulikCalStep==3) {
                addressH=0x01;
                addressL=0x10;
            } else if(gv.ulikCalStep==4) {
                addressH=0x01;
                addressL=0x14;
            }
            Ulik_MW_COD_CalParaSet(comPort, flash.ulik.id, fdata, addressH, addressL);
        }
        else if(gv.ulikCalItem==1) {//TOC org
            if(gv.ulikCalStep==0) {
                addressH=0x01;
                addressL=0x40;
            } else if(gv.ulikCalStep==1) {
                addressH=0x01;
                addressL=0x44;
            } else if(gv.ulikCalStep==2) {
                addressH=0x01;
                addressL=0x48;
            } else if(gv.ulikCalStep==3) {
                addressH=0x01;
                addressL=0x4C;
            } else if(gv.ulikCalStep==4) {
                addressH=0x01;
                addressL=0x50;
            }
            Ulik_MW_TOC_CalParaSet(comPort, flash.ulik.id, fdata, addressH, addressL);
        }
        else if(gv.ulikCalItem==2) {//NO3_N org
            if(gv.ulikCalStep==0) {
                addressH=0x01;
                addressL=0x2C;
            } else if(gv.ulikCalStep==1) {
                addressH=0x01;
                addressL=0x30;
            } else if(gv.ulikCalStep==2) {
                addressH=0x01;
                addressL=0x34;
            } else if(gv.ulikCalStep==3) {
                addressH=0x01;
                addressL=0x38;
            } else if(gv.ulikCalStep==4) {
                addressH=0x01;
                addressL=0x3C;
            }
            Ulik_MW_NO3_CalParaSet(comPort, flash.ulik.id, fdata, addressH, addressL);
        }
        else if(gv.ulikCalItem==3) {//TUR org
            if(gv.ulikCalStep==0) {
                addressH=0x01;
                addressL=0x18;
            } else if(gv.ulikCalStep==1) {
                addressH=0x01;
                addressL=0x1C;
            } else if(gv.ulikCalStep==2) {
                addressH=0x01;
                addressL=0x20;
            } else if(gv.ulikCalStep==3) {
                addressH=0x01;
                addressL=0x24;
            } else if(gv.ulikCalStep==4) {
                addressH=0x01;
                addressL=0x28;
            }
            Ulik_MW_TUR_CalParaSet(comPort, flash.ulik.id, fdata, addressH, addressL);
        }
        Dprintf("교정(설정 %.3f)명령 송신완료\n",fdata);        
        gv.ulikCalCmdSendNo=0;
        SetWidgetStatus(gv.handle2[13],OFF);//BUTTON IMAGE CHANGE
    }
    else if(gv.ulikCalCmdSendNo==5) {
        GetWidgetText(gv.handle2[5],pBuf);
        float fdata = atof(pBuf);
        if(gv.ulikCalItem==0) {
            //Ulik_TURsensor(comPort, flash.ulik.id, fdata);
        }
        else if(gv.ulikCalItem==1) {
            //Ulik_TURsensor(comPort, flash.ulik.id, fdata);
        }
        Dprintf("교정(설정 %.3f)명령 송신완료\n",fdata);        
        gv.ulikCalCmdSendNo=6;//교정파라메터송신 다음단계        
    }
    else if(gv.ulikCalCmdSendNo==6) {
        if(gv.ulikCalItem==0) {
            //Ulik_CODcalibCtrl(comPort,flash.ulik.id,0x06,0);
        }
        else if(gv.ulikCalItem==1) {
            //Ulik_TOCcalibCtrl(comPort,flash.ulik.id,0x06,0);
        }
        Dprintf("교정완료명령송신\n");
        gv.ulikCalCmdSendNo=0;
        SetWidgetStatus(gv.handle2[13],OFF);
    }
    else if(gv.ulikCalCmdSendNo==7) {//교정취소
        gv.ulikCalCmdSendNo=0;
        SetWidgetStatus(gv.handle2[13],OFF);
        sprintf(pBuf,"%.3f", 0.0);
        SetWidgetText(gv.handle2[3],pBuf);
    }
    
    //세정명령 이미지처리
    if(ulikCleanCount > 0) {
        ulikCleanCount--;
        if(ulikCleanCount==1) {
            Dprintf("자동세정정지\n");
            //Ulik_WindowAutoCleaningDisable(USART_1,flash.ulik.id,0x0F);
        }
    } else {
        SetWidgetStatus(gv.handle2[12],OFF);
    }
    
    //COD
    if(gv.ulikCalItem==0) {
        sprintf(pBuf,"%10.3f", gv.COD_Cr);    
        pBuf[10]=0;
        SetWidgetText(gv.handle2[0],pBuf);  //COD    
        sprintf(pBuf,"%10.3f", gv.COD_KHP);
        pBuf[10]=0;
        SetWidgetText(gv.handle2[1],pBuf);  //CODorg    
        //sprintf(pBuf,"%10.3f", gv.TUR);
        //pBuf[10]=0;
        //SetWidgetText(gv.handle2[2],pBuf);  //TUR
        //교정값중에 자동으로 입력되는 값    
//        sprintf(pBuf,"%10.3f", gv.COD_KHP);
//        pBuf[10]=0;
//        SetWidgetText(gv.handle2[4],pBuf);  //COD_KHP    
        //sprintf(pBuf,"%10.3f", gv.TUR);
        //pBuf[10]=0;
        //SetWidgetText(gv.handle2[5],pBuf);  //TUR
    }
    //TOC
    else if(gv.ulikCalItem==1) {
        sprintf(pBuf,"%10.3f", gv.TOCstd);    
        pBuf[10]=0;
        SetWidgetText(gv.handle2[0],pBuf);  //TOC  
        sprintf(pBuf,"%10.3f", gv.TOCreal);
        pBuf[10]=0;
        SetWidgetText(gv.handle2[1],pBuf);  //TOCstd
        //sprintf(pBuf,"%10.3f", gv.TUR);
        //pBuf[10]=0;
        //SetWidgetText(gv.handle2[2],pBuf);  //TUR
        //교정값중에 자동으로 입력되는 값    
//        sprintf(pBuf,"%10.3f", gv.TOCreal);
//        pBuf[10]=0;
//        SetWidgetText(gv.handle2[4],pBuf);  //TOCstd    
        //sprintf(pBuf,"%10.3f", gv.TUR);
        //pBuf[10]=0;
        //SetWidgetText(gv.handle2[5],pBuf);  //TUR
    }
    //NO3_N
    else if(gv.ulikCalItem==2) {
        sprintf(pBuf,"%10.3f", gv.NO3);    
        pBuf[10]=0;
        SetWidgetText(gv.handle2[0],pBuf);  
        sprintf(pBuf,"%10.3f", gv.NO3org);
        pBuf[10]=0;
        SetWidgetText(gv.handle2[1],pBuf);  
        //sprintf(pBuf,"%10.3f", gv.TUR);
        //pBuf[10]=0;
        //SetWidgetText(gv.handle2[2],pBuf);
        //교정값중에 자동으로 입력되는 값    
//        sprintf(pBuf,"%10.3f", gv.NO3org);
//        pBuf[10]=0;
//        SetWidgetText(gv.handle2[4],pBuf);  
        //sprintf(pBuf,"%10.3f", gv.TUR);
        //pBuf[10]=0;
        //SetWidgetText(gv.handle2[5],pBuf);
    }
    //TUR
    else if(gv.ulikCalItem==3) {
        sprintf(pBuf,"%10.3f", gv.TUR);    
        pBuf[10]=0;
        SetWidgetText(gv.handle2[0],pBuf);  //TOC  
        sprintf(pBuf,"%10.3f", gv.TURorg);
        pBuf[10]=0;
        SetWidgetText(gv.handle2[1],pBuf);  //TOCstd
        //sprintf(pBuf,"%10.3f", gv.TUR);
        //pBuf[10]=0;
        //SetWidgetText(gv.handle2[2],pBuf);  //TUR
        //교정값중에 자동으로 입력되는 값    
//        sprintf(pBuf,"%10.3f", gv.TURorg);
//        pBuf[10]=0;
//        SetWidgetText(gv.handle2[4],pBuf);  //TOCstd    
        //sprintf(pBuf,"%10.3f", gv.TUR);
        //pBuf[10]=0;
        //SetWidgetText(gv.handle2[5],pBuf);  //TUR
    }
    
    
    //교정값을 화면에 출력한다.
    paraAddress = gv.ulikCalStep-1;

    switch(paraAddress) {
        case 0: paraAddress = 0;    break;
        case 1: paraAddress = 0;    break;
        case 2: paraAddress = 1;    break;
        case 3: paraAddress = 2;    break;
        case 4: paraAddress = 3;    break;
        case 5: paraAddress = 4;    break;
        default : paraAddress = 0;  break;
    }
    if(gv.ulikCalItem==0) {
        sprintf(pBuf,"%d-point calibration value", paraAddress+1);
        SetWidgetText(gv.handle2[21],pBuf);
        SetWidgetText(gv.handle2[30],"COD");
        SetWidgetText(gv.handle2[31],"CODorg");

        sprintf(pBuf,"%d-point calibration value", paraAddress+2);
        SetWidgetText(gv.handle2[22],pBuf);
        SetWidgetText(gv.handle2[33],"COD");
        SetWidgetText(gv.handle2[34],"CODorg");
    } 
    else if(gv.ulikCalItem==1) {
        sprintf(pBuf,"%d-point calibration value", paraAddress+1);
        SetWidgetText(gv.handle2[21],pBuf);
        SetWidgetText(gv.handle2[30],"TOC");
        SetWidgetText(gv.handle2[31],"TOCorg");

        sprintf(pBuf,"%d-point calibration value", paraAddress+2);
        SetWidgetText(gv.handle2[22],pBuf);
        SetWidgetText(gv.handle2[33],"TOC");
        SetWidgetText(gv.handle2[34],"TOCorg");
    }
    else if(gv.ulikCalItem==2) {
        sprintf(pBuf,"%d-point calibration value", paraAddress+1);
        SetWidgetText(gv.handle2[21],pBuf);
        SetWidgetText(gv.handle2[30],"NO3_N");
        SetWidgetText(gv.handle2[31],"NO3_Norg");

        sprintf(pBuf,"%d-point calibration value", paraAddress+2);
        SetWidgetText(gv.handle2[22],pBuf);
        SetWidgetText(gv.handle2[33],"NO3_N");
        SetWidgetText(gv.handle2[34],"NO3_Norg");
    }
    else if(gv.ulikCalItem==3) {
        sprintf(pBuf,"%d-point calibration value", paraAddress+1);
        SetWidgetText(gv.handle2[21],pBuf);
        SetWidgetText(gv.handle2[30],"TUR");
        SetWidgetText(gv.handle2[31],"TURorg");

        sprintf(pBuf,"%d-point calibration value", paraAddress+2);
        SetWidgetText(gv.handle2[22],pBuf);
        SetWidgetText(gv.handle2[33],"TUR");
        SetWidgetText(gv.handle2[34],"TURorg");
    }
    
    if(gv.ulikCalItem==0 || gv.ulikCalItem==1 || gv.ulikCalItem==2 || gv.ulikCalItem==3) {
        //1-point calibration value    
        sprintf(pBuf,"%10.3f", gv.paraValue1[paraAddress]);    
        pBuf[10]=0;
        SetWidgetText(gv.handle2[7],pBuf);  //교정값
        sprintf(pBuf,"%10.3f", gv.paraValue2[paraAddress]);
        pBuf[10]=0;
        SetWidgetText(gv.handle2[6],pBuf);  //측정값
        
        //2-point calibration value
        paraAddress += 1;
        sprintf(pBuf,"%10.3f", gv.paraValue1[paraAddress]);    
        pBuf[10]=0;
        SetWidgetText(gv.handle2[10],pBuf);
        sprintf(pBuf,"%10.3f", gv.paraValue2[paraAddress]);
        pBuf[10]=0;
        SetWidgetText(gv.handle2[9],pBuf); 
    } 
    if(gv.ulikMessageStep==1) {
        SetWidgetText(gv.handle2[17],"2. 측정값이 안정되면 Comfirm 터치 후 교정을 완료합니다.");
    } 
    else {
        SetWidgetText(gv.handle2[17],"1. KEY BOARD ICON 터치 후 교정 중인 버퍼나 샘플 값을 입력합니다.");
    }
    
    //교정스텝출력
    sprintf(pBuf,"CalStep.%d",gv.ulikCalStep);
    SetWidgetText(gv.handle2[18],pBuf);
    
    if(ulikZeroCalibrationFlag==ON) {
        Dprintf("교정 명령을 송신한다.");
        //Ulik_ZeroCalibration(comPort,flash.ulik.id,0x0F);
        ulikZeroCalibrationFlag=0;
    }
    
	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return 1;
}
int	 ULIKScreenCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle,"SmartLogSontekConfigCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL) {
		Dprintf("ULIKScreenCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL){
		Dprintf("ULIKScreenCreateEvent::GetGraphics Error\n");
		return(0);
	}	
	PaintDrawControl(OFF);
    Gr->FillRect(0, 0, 0, 0);//Dummy
	Gr->SetColor(flash.systemBackGroundColor);
	Gr->FillRect(0, 0, win->width, win->height);
	
    //unsigned int labelBackColor = RGB565CONVERT(36, 36, 36);  //RGB565CONVERT(191, 191, 191);//화면떨리는현상발생
    //unsigned int labelFontColor = clYellow;

    char pBuf[120];
    int gwidth=240;
    int width=240-3, height=50, side=5;
	int	sx = 10;
	int	sy = 60 + height + side;
    
    for(int i=0; i<120; i++) {
        pBuf[i]=0;
    }
    sprintf(pBuf,"%10.3f", 0.0);
    pBuf[10]=0;

    gv.ulikMessageStep=0;   //스텝별로 교정 메시지가 출력된다.
    ulikCleanCount=0;       //세정시간
    //COD CALIBRATION SCREEN
    if(gv.ulikCalItem==0) {
        //좌측화면    
        GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "측정값");
        
        Label(handle, sx, sy, width, height, "COD",  clBlack);
        gv.handle2[0]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  NULL, 0, NULL); 
        
        Label(handle, sx, sy+height+side, width, height, "CODorg",  clBlack);
        gv.handle2[1]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  NULL, 1, NULL); 
        
        GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "교정값(사용자설정)");
        
        Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "COD",  clBlack);
        gv.handle2[3]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  PUSH_BUTTON, 3, ULIKScreenEditButtonEvent); //COD 사용자 설정값
        
        Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "CODorg",  clBlack);
        gv.handle2[4]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  SELECT_BUTTON, 4, ULIKScreenEditButtonEvent);  //CODorg 자동설정값
        
        //우측화면
        sx = sx+width+width+side+side+side; 
        gv.handle2[21]=GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "1-point calibration value");
        gv.handle2[30]=Label(handle, sx, sy, width, height, "COD",  clBlack);
        gv.handle2[6]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  SELECT_BUTTON, 6, NULL); 
        gv.handle2[31]=Label(handle, sx, sy+height+side, width, height, "CODorg",  clBlack);
        gv.handle2[7]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 7, NULL); 
        
        //gv.handle2[32]=Label(handle, sx, sy+height+side+height+side, width, height, "CODreal",  clBlack);
        //gv.handle2[8]=SButton(handle, sx+width+side, sy+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 8, NULL); 
        
        gv.handle2[22]=GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "2-point calibration value");
        gv.handle2[33]=Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "COD",  clBlack);
        gv.handle2[9]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 9, ULIKScreenEditButtonEvent); 
        gv.handle2[34]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "CODorg",  clBlack);
        gv.handle2[10]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 10, ULIKScreenEditButtonEvent); 
        //gv.handle2[35]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side, width, height, "CODreal",  clBlack);
        //gv.handle2[11]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 11, ULIKScreenEditButtonEvent); 
        
        //BUTTON
        sx=10;
        gv.handle2[12]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_TestBottle_Icon6.bmp", "     Clean start",  SELECT_BUTTON, 12, ULIKScreenEditButtonEvent);
        gv.handle2[13]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Save_Icon1.bmp", "Comfirm",  SELECT_BUTTON, 13, ULIKScreenEditButtonEvent); 
        
        //gv.handle2[14]=SButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear Last Calib",  PUSH_BUTTON, 14, ULIKScreenEditButtonEvent); 
        gv.handle2[15]=NButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear All Calib",  PUSH_BUTTON, 15, ULIKScreenEditButtonEvent); 
        gv.handle2[16]=NButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Home_Icon2.bmp", "EXIT",  PUSH_BUTTON, 16, ULIKScreenEditButtonEvent); 
        //Zero Calibration
        //gv.handle2[36]=SButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Retry_Icon2.bmp", "Zero Cal",  PUSH_BUTTON, 36, ULIKScreenEditButtonEvent); 


        //교정메시지출력
        gv.handle2[17]=SButton(handle, 5, 10, 1000-15, height, 0, 
        "1. KEY BOARD ICON 터치 후 교정 중인 버퍼나 샘플 값을 입력합니다.",  PUSH_BUTTON, 18, NULL); 
        //교정스텝출력
        sprintf(pBuf,"CalStep.%d",gv.ulikCalStep);
        gv.handle2[18]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, " ", pBuf,  SELECT_BUTTON, 18, ULIKScreenEditButtonEvent); 
        //세정시간설정
        sprintf(pBuf,"     Clean sec %d",flash.ulik.cleanTimeSv);
        gv.handle2[19]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_TestBottle_Icon6.bmp", pBuf,  PUSH_BUTTON, 19, ULIKScreenEditButtonEvent);
    } 
    //TOC CALIBRATION SCREEN
    else if(gv.ulikCalItem==1) {
        //좌측화면    
        GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "측정값");
        Label(handle, sx, sy, width, height, "TOC",  clBlack);
        gv.handle2[0]=NButton(handle, sx+width+side, sy, width, height, 0, pBuf,  NULL, 0, NULL); 
        Label(handle, sx, sy+height+side, width, height, "TOCorg",  clBlack);
        gv.handle2[1]=NButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  NULL, 1, NULL); 
        
        
        GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "교정값(사용자설정)");
        Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "TOC",  clBlack);
        gv.handle2[3]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  PUSH_BUTTON, 3, ULIKScreenEditButtonEvent); 
        Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "TOCorg",  clBlack);
        gv.handle2[4]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  SELECT_BUTTON, 4, ULIKScreenEditButtonEvent); 
        
        //우측화면
        sx = sx+width+width+side+side+side; 
        gv.handle2[21]=GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "1-point calibration value");
        gv.handle2[30]=Label(handle, sx, sy, width, height, "TOC",  clBlack);
        gv.handle2[6]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  SELECT_BUTTON, 6, NULL); 
        gv.handle2[31]=Label(handle, sx, sy+height+side, width, height, "TURorg",  clBlack);
        gv.handle2[7]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 7, NULL); 
        
        gv.handle2[22]=GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "2-point calibration value");
        gv.handle2[33]=Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "TOC",  clBlack);
        gv.handle2[9]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 9, ULIKScreenEditButtonEvent); 
        gv.handle2[34]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "TOCorg",  clBlack);
        gv.handle2[10]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 10, ULIKScreenEditButtonEvent); 
        
        //BUTTON
        sx=10;
        gv.handle2[12]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_TestBottle_Icon6.bmp", "     Clean start",  SELECT_BUTTON, 12, ULIKScreenEditButtonEvent);
        gv.handle2[13]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Save_Icon1.bmp", "Comfirm",  SELECT_BUTTON, 13, ULIKScreenEditButtonEvent); 
        
        //gv.handle2[14]=SButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear Last Calib",  PUSH_BUTTON, 14, ULIKScreenEditButtonEvent); 
        gv.handle2[15]=NButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear All Calib",  PUSH_BUTTON, 15, ULIKScreenEditButtonEvent); 
        gv.handle2[16]=NButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Home_Icon2.bmp", "EXIT",  PUSH_BUTTON, 16, ULIKScreenEditButtonEvent); 

        //교정메시지출력
        gv.handle2[17]=SButton(handle, 5, 10, 1000-15, height, 0, 
        "1. KEY BOARD ICON 터치 후 교정 중인 버퍼나 샘플값을 입력합니다.",  PUSH_BUTTON, 18, NULL); 
        //교정스텝출력
        sprintf(pBuf,"CalStep.%d",gv.ulikCalStep);
        gv.handle2[18]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, " ", pBuf,  SELECT_BUTTON, 18, ULIKScreenEditButtonEvent); 
        //세정시간설정
        sprintf(pBuf,"     Clean sec %d",flash.ulik.cleanTimeSv);
        gv.handle2[19]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_TestBottle_Icon6.bmp", pBuf,  PUSH_BUTTON, 19, ULIKScreenEditButtonEvent);
    } 
    //NO3 CALIBRATION SCREEN
    else if(gv.ulikCalItem==2) {
        //좌측화면    
        GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "측정값");
        Label(handle, sx, sy, width, height, "NO3_N",  clBlack);
        gv.handle2[0]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  NULL, 0, NULL); 
        Label(handle, sx, sy+height+side, width, height, "NO3_Norg",  clBlack);
        gv.handle2[1]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  NULL, 1, NULL); 
        
        GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "교정값(사용자설정)");
        Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "NO3_N",  clBlack);
        gv.handle2[3]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  PUSH_BUTTON, 3, ULIKScreenEditButtonEvent); 
        Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "NO3_Norg",  clBlack);
        gv.handle2[4]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  SELECT_BUTTON, 4, ULIKScreenEditButtonEvent); 
        
        //우측화면
        sx = sx+width+width+side+side+side; 
        gv.handle2[21]=GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "1-point calibration value");
        gv.handle2[30]=Label(handle, sx, sy, width, height, "NO3_N",  clBlack);
        gv.handle2[6]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  SELECT_BUTTON, 6, NULL); 
        gv.handle2[31]=Label(handle, sx, sy+height+side, width, height, "NO3_Norg",  clBlack);
        gv.handle2[7]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 7, NULL); 
        
        gv.handle2[22]=GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "2-point calibration value");
        gv.handle2[33]=Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "NO3_N",  clBlack);
        gv.handle2[9]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 9, ULIKScreenEditButtonEvent); 
        gv.handle2[34]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "NO3_Norg",  clBlack);
        gv.handle2[10]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 10, ULIKScreenEditButtonEvent); 
        
        //BUTTON
        sx=10;
        gv.handle2[12]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_TestBottle_Icon6.bmp", "     Clean start",  SELECT_BUTTON, 12, ULIKScreenEditButtonEvent);
        gv.handle2[13]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Save_Icon1.bmp", "Comfirm",  SELECT_BUTTON, 13, ULIKScreenEditButtonEvent); 
        
        gv.handle2[15]=NButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear All Calib",  PUSH_BUTTON, 15, ULIKScreenEditButtonEvent); 
        gv.handle2[16]=NButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Home_Icon2.bmp", "EXIT",  PUSH_BUTTON, 16, ULIKScreenEditButtonEvent); 

        //Zero Calibration
        //gv.handle2[36]=SButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Retry_Icon2.bmp", "Zero Cal",  PUSH_BUTTON, 36, ULIKScreenEditButtonEvent); 

        //교정메시지출력
        gv.handle2[17]=SButton(handle, 5, 10, 1000-15, height, 0, 
        "1. KEY BOARD ICON 터치 후 교정 중인 버퍼나 샘플 값을 입력합니다.",  PUSH_BUTTON, 18, NULL); 
        //교정스텝출력
        sprintf(pBuf,"CalStep.%d",gv.ulikCalStep);
        gv.handle2[18]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, " ", pBuf,  SELECT_BUTTON, 18, ULIKScreenEditButtonEvent); 
        //세정시간설정
        sprintf(pBuf,"     Clean sec %d",flash.ulik.cleanTimeSv);
        gv.handle2[19]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_TestBottle_Icon6.bmp", pBuf,  PUSH_BUTTON, 19, ULIKScreenEditButtonEvent);
        
    }
    //TUR CALIBRATION SCREEN
    else if(gv.ulikCalItem==3) {
        //좌측화면    
        GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "측정값");
        Label(handle, sx, sy, width, height, "TUR",  clBlack);
        gv.handle2[0]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  NULL, 0, NULL); 
        Label(handle, sx, sy+height+side, width, height, "TURorg",  clBlack);
        gv.handle2[1]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  NULL, 1, NULL); 
        
        GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "교정값(사용자설정)");
        Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "TUR",  clBlack);
        gv.handle2[3]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  PUSH_BUTTON, 3, ULIKScreenEditButtonEvent); 
        Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "TURorg",  clBlack);
        gv.handle2[4]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  SELECT_BUTTON, 4, ULIKScreenEditButtonEvent); 
        
        //우측화면
        sx = sx+width+width+side+side+side; 
        gv.handle2[21]=GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "1-point calibration value");
        gv.handle2[30]=Label(handle, sx, sy, width, height, "TUR",  clBlack);
        gv.handle2[6]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  SELECT_BUTTON, 6, NULL); 
        gv.handle2[31]=Label(handle, sx, sy+height+side, width, height, "TURorg",  clBlack);
        gv.handle2[7]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 7, NULL); 
        
        gv.handle2[22]=GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "2-point calibration value");
        gv.handle2[33]=Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "TUR",  clBlack);
        gv.handle2[9]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 9, ULIKScreenEditButtonEvent); 
        gv.handle2[34]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "TURorg",  clBlack);
        gv.handle2[10]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 10, ULIKScreenEditButtonEvent); 
        
        //BUTTON
        sx=10;
        gv.handle2[12]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_TestBottle_Icon6.bmp", "     Clean start",  SELECT_BUTTON, 12, ULIKScreenEditButtonEvent);
        gv.handle2[13]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Save_Icon1.bmp", "Comfirm",  SELECT_BUTTON, 13, ULIKScreenEditButtonEvent); 
        
        gv.handle2[15]=NButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear All Calib",  PUSH_BUTTON, 15, ULIKScreenEditButtonEvent); 
        gv.handle2[16]=NButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Home_Icon2.bmp", "EXIT",  PUSH_BUTTON, 16, ULIKScreenEditButtonEvent); 

        //Zero Calibration
        //gv.handle2[36]=SButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Retry_Icon2.bmp", "Zero Cal",  PUSH_BUTTON, 36, ULIKScreenEditButtonEvent); 

        //교정메시지출력
        gv.handle2[17]=SButton(handle, 5, 10, 1000-15, height, 0, 
        "1. KEY BOARD ICON 터치 후 교정 중인 버퍼나 샘플 값을 입력합니다.",  PUSH_BUTTON, 18, NULL); 
        //교정스텝출력
        sprintf(pBuf,"CalStep.%d",gv.ulikCalStep);
        gv.handle2[18]=NButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, " ", pBuf,  SELECT_BUTTON, 18, ULIKScreenEditButtonEvent); 
        //세정시간설정
        sprintf(pBuf,"     Clean sec %d",flash.ulik.cleanTimeSv);
        gv.handle2[19]=NButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_TestBottle_Icon6.bmp", pBuf,  PUSH_BUTTON, 19, ULIKScreenEditButtonEvent);
       
    }    
    //여분
    else if(gv.ulikCalItem==4) {
        //좌측화면    
        GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "측정값");
        
        Label(handle, sx, sy, width, height, "TOCstd",  clBlack);
        gv.handle2[0]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  NULL, 0, NULL); 
        
        Label(handle, sx, sy+height+side, width, height, "NONE",  clBlack);
        gv.handle2[1]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  NULL, 1, NULL); 
        
        Label(handle, sx, sy+height+side+height+side, width, height, "NONE",  clBlack);
        gv.handle2[2]=SButton(handle, sx+width+side, sy+height+side+height+side, width, height, 0, pBuf,  NULL, 2,NULL);         
        
        GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "교정값(사용자설정)");
        
        Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "TOCstd",  clBlack);
        gv.handle2[3]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  PUSH_BUTTON, 3, ULIKScreenEditButtonEvent); 
        
        Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "NONE",  clBlack);
        gv.handle2[4]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, "48G_Keyboard_Icon1.bmp", pBuf,  SELECT_BUTTON, 4, ULIKScreenEditButtonEvent); 
        
        Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side, width, height, "NONE",  clBlack);
        gv.handle2[5]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 5, ULIKScreenEditButtonEvent); 
        
        //우측화면
        sx = sx+width+width+side+side+side; 
        gv.handle2[21]=GroupPanel(handle, sx-5, sy-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "1-point calibration value");
        gv.handle2[30]=Label(handle, sx, sy, width, height,  "TOCstd_org",  clBlack);
        gv.handle2[6]=SButton(handle, sx+width+side, sy, width, height, 0, pBuf,  SELECT_BUTTON, 6, NULL); 
        gv.handle2[31]=Label(handle, sx, sy+height+side, width, height, "TOCstd",  clBlack);
        gv.handle2[7]=SButton(handle, sx+width+side, sy+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 7, NULL); 
        gv.handle2[32]=Label(handle, sx, sy+height+side+height+side, width, height, "NONE",  clBlack);
        gv.handle2[8]=SButton(handle, sx+width+side, sy+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 8, NULL); 
        
        gv.handle2[22]=GroupPanel(handle, sx-5, sy+height+side+height+side+height+side+height+side-height, gwidth+side+gwidth+side, height+side+height+side+height+side+height+side, "NONE");
        gv.handle2[33]=Label(handle, sx, sy+height+side+height+side+height+side+height+side, width, height, "NONE",  clBlack);
        gv.handle2[9]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 9, ULIKScreenEditButtonEvent); 
        gv.handle2[34]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side, width, height, "NONE",  clBlack);
        gv.handle2[10]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 10, ULIKScreenEditButtonEvent); 
        gv.handle2[35]=Label(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side, width, height, "NONE",  clBlack);
        gv.handle2[11]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side, width, height, 0, pBuf,  SELECT_BUTTON, 11, ULIKScreenEditButtonEvent); 
        
        //BUTTON
        sx=10;
        gv.handle2[12]=SButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_TestBottle_Icon6.bmp", "     Clean start",  SELECT_BUTTON, 12, ULIKScreenEditButtonEvent);

        gv.handle2[13]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Save_Icon1.bmp", "Comfirm",  SELECT_BUTTON, 13, ULIKScreenEditButtonEvent); 
        
        gv.handle2[14]=SButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear Calib",  PUSH_BUTTON, 14, ULIKScreenEditButtonEvent); 
        //WidgetDisplay(gv.handle2[14],HIDDEN);
        gv.handle2[15]=SButton(handle, sx+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Chip_Icon1.bmp", "     Clear All Calib",  PUSH_BUTTON, 15, ULIKScreenEditButtonEvent); 
        WidgetDisplay(gv.handle2[15],HIDDEN);
        gv.handle2[16]=SButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_Home_Icon2.bmp", "EXIT",  PUSH_BUTTON, 16, ULIKScreenEditButtonEvent); 

        //Zero Calibration
        gv.handle2[36]=SButton(handle, sx+width+side+width+side+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side, width, 60, "48G_Retry_Icon2.bmp", "Zero Cal",  PUSH_BUTTON, 36, ULIKScreenEditButtonEvent); 

        //교정메시지출력
        gv.handle2[17]=SButton(handle, 5, 10, 1000-15, height, 0, 
        "1. KEY BOARD ICON 터치 후 교정 중인 버퍼나 샘플 값을 입력합니다.",  PUSH_BUTTON, 18, NULL); 
        //교정스텝출력
        sprintf(pBuf,"CalStep.%d",gv.ulikCalStep);
        gv.handle2[18]=SButton(handle, sx+width+side, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, " ", pBuf,  SELECT_BUTTON, 18, ULIKScreenEditButtonEvent); 
        WidgetDisplay(gv.handle2[18],HIDDEN);
        //세정시간설정
        sprintf(pBuf,"     Clean sec %d",flash.ulik.cleanTimeSv);
        gv.handle2[19]=SButton(handle, sx, sy+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+height+side+side+side, width, 60, "48G_TestBottle_Icon6.bmp", pBuf,  PUSH_BUTTON, 19, ULIKScreenEditButtonEvent);
        
    }        
    ULIK_TimerID=OpenTimer(parent, handle, 1000);
	StartTimer(ULIK_TimerID);
	PaintDrawControl(ON);	
	return(1);
}
int	 ULIKScreenPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
void ULIKScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title) {
	WidgetStruct wid_struct;	
	wid_struct.FontNo = font_no;
	//wid_struct.CallWidget_ID = id;
	makeWindow(
				LCD,
				"ULIKScreen",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				STYLE_NONE,				// Windows Style
				sx, sy, 				// Start Position
				1000, 730,			// Size
				SHOW,					// Create after Show
				ULIKScreenCreateEvent,		// CreateEvent
				ULIKScreenPaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				ULIKScreenTimerEvent,					// TimerEvent
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

