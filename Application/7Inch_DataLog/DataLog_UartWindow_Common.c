#ifdef __cplusplus
 extern "C" {
#endif
#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"


int ValidMeasurement(float value, float minValue, float maxValue) {
	return (value >= minValue && value <= maxValue);	
}

#if defined (DAECHEONGHO_SUGILGWANLISO)
void DAECHEONGHO_FloatPoint_Display(int ch, int comPort) {
    char data[16];
    for(int i=0; i<16; i++) {
        data[i]=0;
    }
    int chChange;
    int chAddress;
    if(comPort==USART_1) {
        chChange = ch;
        chAddress = ch;
    } 
    else if(comPort==USART_2) {
        chChange = ch-6;
        chAddress = ch;
    }
    if(flash.floatPoint[chChange]==0) {
        sprintf(data,"%10.0f",flash.Uart[comPort]->SensorPV[chChange]); data[10]=0;
    }
    else if(flash.floatPoint[chChange]==1) {
        sprintf(data,"%10.1f",flash.Uart[comPort]->SensorPV[chChange]); data[10]=0;
    }
    else if(flash.floatPoint[chChange]==2) {
        sprintf(data,"%10.2f",flash.Uart[comPort]->SensorPV[chChange]); data[10]=0;
    }
    else if(flash.floatPoint[chChange]==3) {
        sprintf(data,"%10.3f",flash.Uart[comPort]->SensorPV[chChange]); data[10]=0;
    }
    else if(flash.floatPoint[chChange]==4) {
        sprintf(data,"%10.4f",flash.Uart[comPort]->SensorPV[chChange]); data[10]=0;
    } 
    else {
        Dprintf("데이터 출력 주소오류\n");
    }
    SetWidgetText(gv.handle[chAddress], data);
}
#endif
void FloatPoint_ADCDInputDisplay(int ch, float fdata) {
    char data[16];
    for(int i=0; i<16; i++) {
        data[i]=0;
    }
	int comPort = 0;//
	if(ValidMeasurement(gv.averPV5Min[comPort][ch],-9999999.9f,99999999.9)) {
		if(flash.floatPoint[ch]==0) {
			sprintf(data,"%10.0f",fdata); data[10]=0;
		}
		else if(flash.floatPoint[ch]==1) {
			sprintf(data,"%10.1f",fdata); data[10]=0;
		}
		else if(flash.floatPoint[ch]==2) {
			sprintf(data,"%10.2f",fdata); data[10]=0;
		}
		else if(flash.floatPoint[ch]==3) {
			sprintf(data,"%10.3f",fdata); data[10]=0;
		}
		else if(flash.floatPoint[ch]==4) {
			sprintf(data,"%10.4f",fdata); data[10]=0;
		} 
		else {
			Dprintf("데이터 출력 주소오류");
		}
		SetWidgetText(gv.handle[ch], data);
	}
	else {
		char stringBuf[64];
		sprintf(stringBuf,"ADC.%d %s 측정값 출력오류",ch+1,flash.Input[ch]->Name);
		EventMessage(stringBuf);
		strcpy(data,"0.0");
	}
}
void FloatPoint_ADInputAverageProcess(int ch, int comPort,  char *data) {       
	if(ValidMeasurement(gv.averPV5Min[comPort][ch],-9999999.9f,99999999.9)) {
		if(flash.floatPoint[ch]==0) {
			sprintf(data,"%s,%10.0f,",flash.Input[ch]->Name,gv.averPV5Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==1) {
			sprintf(data,"%s,%10.1f,",flash.Input[ch]->Name,gv.averPV5Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==2) {
			sprintf(data,"%s,%10.2f,",flash.Input[ch]->Name,gv.averPV5Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==3) {
			sprintf(data,"%s,%10.3f,",flash.Input[ch]->Name,gv.averPV5Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==4) {
			sprintf(data,"%s,%10.4f,",flash.Input[ch]->Name,gv.averPV5Min[comPort][ch]); 
		} 
		else {
			Dprintf("데이터 출력 주소오류");
		}
	}
	else {
		char stringBuf[64];
		sprintf(stringBuf,"ADC.%d %s 측정값 출력오류",ch+1,flash.Input[ch]->Name);
		EventMessage(stringBuf);
		strcpy(data,"0.0");
	}
}

void FloatPoint_Display(int ch, int comPort) {
    char data[16];
    for(int i=0; i<16; i++) {
        data[i]=0;
    }
	if(ValidMeasurement(flash.Uart[comPort]->SensorPV[ch],-9999999.9f,99999999.9f)) {
		if(flash.floatPoint[ch]==0) {
			sprintf(data,"%10.0f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		}
		else if(flash.floatPoint[ch]==1) {
			sprintf(data,"%10.1f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		}
		else if(flash.floatPoint[ch]==2) {
			sprintf(data,"%10.2f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		}
		else if(flash.floatPoint[ch]==3) {
			sprintf(data,"%10.3f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		}
		else if(flash.floatPoint[ch]==4) {
			sprintf(data,"%10.4f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		} 
		else {
			Dprintf("데이터 출력 주소오류\n");
		}
		SetWidgetText(gv.handle[ch], data);
	}
	else {
		char stringBuf[64];
		sprintf(stringBuf,"UART.%d %s 측정값 출력오류\n",comPort+1,flash.Uart[comPort]->SetupName[ch]);
		EventMessage(stringBuf);
	}
}
void FloatPoint_Process(int ch, int comPort,  char *data) { 
	if(ValidMeasurement(flash.Uart[comPort]->SensorPV[ch],-9999999.9f,99999999.9)) {
		if(flash.floatPoint[ch]==0) {
			sprintf(data,"%10.0f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		}
		else if(flash.floatPoint[ch]==1) {
			sprintf(data,"%10.1f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		}
		else if(flash.floatPoint[ch]==2) {
			sprintf(data,"%10.2f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		}
		else if(flash.floatPoint[ch]==3) {
			sprintf(data,"%10.3f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		}
		else if(flash.floatPoint[ch]==4) {
			sprintf(data,"%10.4f",flash.Uart[comPort]->SensorPV[ch]); data[10]=0;
		} 
		else {
			Dprintf("데이터 출력 주소오류");
		}
	}
	else {
		char stringBuf[64];
		sprintf(stringBuf,"UART.%d %s 측정값 출력오류\n",comPort+1,flash.Uart[comPort]->SetupName[ch]);
		EventMessage(stringBuf);
		strcpy(data,"0.0");
	}
}
void FloatPoint_AverageProcess(int ch, int comPort,  char *data) { //5분평균데이터 저장시 유효성검사
	if(ValidMeasurement(gv.averPV5Min[comPort][ch],-9999999.9f,99999999.9)) {
		if(flash.floatPoint[ch]==0) {
			sprintf(data,"%s,%10.0f,",flash.Uart[comPort]->SetupName[ch],gv.averPV5Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==1) {
			sprintf(data,"%s,%10.1f,",flash.Uart[comPort]->SetupName[ch],gv.averPV5Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==2) {
			sprintf(data,"%s,%10.2f,",flash.Uart[comPort]->SetupName[ch],gv.averPV5Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==3) {
			sprintf(data,"%s,%10.3f,",flash.Uart[comPort]->SetupName[ch],gv.averPV5Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==4) {
			sprintf(data,"%s,%10.4f,",flash.Uart[comPort]->SetupName[ch],gv.averPV5Min[comPort][ch]); 
		} 
		else {
			Dprintf("데이터 출력 주소오류\n");
		}
	} 
	else {
		char stringBuf[64];
		sprintf(stringBuf,"UART.%d %s 측정값 출력오류\n",comPort+1,flash.Uart[comPort]->SetupName[ch]);
		EventMessage(stringBuf);
	}
}
void TMSStatusCode(int ch,int comPort, char *data) {
	sprintf(data,"상태코드,%02d,",gv.tmsStatusCode[comPort]);
}
void TMSStatusCode60Min(int ch, int comPort, char *data) {
	if(gv.tmsStatusCode60minComErrCnt[comPort] >= 3) {//5분마다 통신불량 증가 횟수 3회이상이면 통신불량 처리한다.
		sprintf(data,"상태코드,%02d,",6); //통신불량
	}
	else if(gv.tmsStatusCode60MinCalCnt[comPort] >= 3) {
		sprintf(data,"상태코드,%02d,",3); //교정진행.5분마타 카운트 1 이면 한시간이면 3개 이상 증가하면 데이터를 0처리한다.
	}
	else {
		sprintf(data,"상태코드,%02d,",0); //정상측정중
	}
}
void FloatPoint_60MinAverageProcess(int ch, int comPort,  char *data) {       
	if(ValidMeasurement(gv.averPV60Min[comPort][ch],-9999999.9f,99999999.9)) {
		if(flash.floatPoint[ch]==0) {
			sprintf(data,"%s,%10.0f,",flash.Uart[comPort]->SetupName[ch],gv.averPV60Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==1) {
			sprintf(data,"%s,%10.1f,",flash.Uart[comPort]->SetupName[ch],gv.averPV60Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==2) {
			sprintf(data,"%s,%10.2f,",flash.Uart[comPort]->SetupName[ch],gv.averPV60Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==3) {
			sprintf(data,"%s,%10.3f,",flash.Uart[comPort]->SetupName[ch],gv.averPV60Min[comPort][ch]); 
		}
		else if(flash.floatPoint[ch]==4) {
			sprintf(data,"%s,%10.4f,",flash.Uart[comPort]->SetupName[ch],gv.averPV60Min[comPort][ch]); 
		} 
		else {
			Dprintf("데이터 출력 주소오류\n");
		}
	} 
	else {
		char stringBuf[64];
		sprintf(stringBuf,"UART.%d %s 측정값 출력오류\n",comPort+1,flash.Uart[comPort]->SetupName[ch]);
		EventMessage(stringBuf);
	}
}
void FloatPoint_RealProcess(int ch, int comPort,  char *data) {   
	if(ValidMeasurement(flash.Uart[comPort]->SensorPV[ch],-9999999.9f,99999999.9)) {	
		if(flash.floatPoint[ch]==0) {
			sprintf(data,"%s,%10.0f,",flash.Uart[comPort]->SetupName[ch],flash.Uart[comPort]->SensorPV[ch]); 
		}
		else if(flash.floatPoint[ch]==1) {
			sprintf(data,"%s,%10.1f,",flash.Uart[comPort]->SetupName[ch],flash.Uart[comPort]->SensorPV[ch]); 
		}
		else if(flash.floatPoint[ch]==2) {
			sprintf(data,"%s,%10.2f,",flash.Uart[comPort]->SetupName[ch],flash.Uart[comPort]->SensorPV[ch]); 
		}
		else if(flash.floatPoint[ch]==3) {
			sprintf(data,"%s,%10.3f,",flash.Uart[comPort]->SetupName[ch],flash.Uart[comPort]->SensorPV[ch]); 
		}
		else if(flash.floatPoint[ch]==4) {
			sprintf(data,"%s,%10.4f,",flash.Uart[comPort]->SetupName[ch],flash.Uart[comPort]->SensorPV[ch]); 
		} 
		else {
			Dprintf("데이터 출력 주소오류");
		}
	}
	else {
		char stringBuf[64];
		sprintf(stringBuf,"UART.%d %s 측정값 출력오류",comPort+1,flash.Uart[comPort]->SetupName[ch]);
		EventMessage(stringBuf);
		strcpy(data,"0.0");
	}
}

int DataLog_Data_Display(unsigned int comPort, unsigned int channel) {
    char data[256];
    for(int i=0; i<256; i++) {
        data[i]=0;
    }
    #ifdef JSON_ENABLE
    if(gv.json.nextSendCmdNo==5) {
        DataLog_MessageBox_Window("펌웨어업데이트", "자동 업데이트 진행 중입니다.");
    }
    #endif
    sprintf(data,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
    SetWidgetText(gv.titleBarTimeId, data);//시간출력
    if(gv.jsonTcpDebugFlag==1) {
        SetWidgetText(gv.titleBarLogId,"프로그래머가 제어 중 입니다.");
    }
    else if(gv.touchErrorCount > 1000) {
        SetWidgetText(gv.titleBarLogId,"터치가 눌려있습니다."); //터치오류출력
    }
    else if(gv.comportConnectionError[USART_1]>=25) {
        SetWidgetText(gv.titleBarLogId,"동작불량 (UART1)");
    }
    else if(gv.comportConnectionError[USART_2]>=25) {
        SetWidgetText(gv.titleBarLogId,"동작불량 (UART2)");
    }
    else if(gv.comportConnectionError[USART_3]>=25) {
        SetWidgetText(gv.titleBarLogId,"동작불량 (UART3)");
    }
//    else if(gv.comportConnectionError[USART_4]>=25) {
//        SetWidgetText(gv.titleBarLogId,"동작불량 (UART4)");
//    }
    #ifdef EXO_ENABLE
    else if(gv.wiperEnableFlag==ON) {
        sprintf(data,"30초 동안 세척합니다. 진행시간 : %02d", gv.wiperWaitCountSec);
        SetWidgetText(gv.titleBarLogId,data);
    } 
    #endif
    else if(flash.powerControl!=2&&flash.sleepModeFlag==1) {
        sprintf(data, "절전모드 진입[600초] %d 초",gv.sleepModeTimerCntSecPv);
        SetWidgetText(gv.titleBarLogId,data);
    } 
    else if(gv.lanStatus[0]==1) {
        SetWidgetText(gv.titleBarLogId,gv.titleBuf);
    } 
    else {
		#if defined (RTUV3_ENABLE)
		strcpy(gv.titleBuf,"< RTU V3.0 >"); 	//매우중요               
		#endif
		#if defined (YSI_ODD_ENABLE)
		strcpy(gv.titleBuf,"< RTU V4.0 >"); 	//매우중요               
		#endif
		#if defined (YSI_ODOCT_ENABLE)
		strcpy(gv.titleBuf,"< RTU V5.0 >");		//매우중요              
		#endif
		#if defined (RTU_VER12_ENABLE)
		strcpy(gv.titleBuf,"240207108008");
		#endif
		#if defined (SPECTRO_D_ENABLE)
		strcpy(gv.titleBuf,"< RTU V3.0 >");
		#endif
		#if defined (SPECTRO_M_ENABLE)
		strcpy(gv.titleBuf,"< RTU V3.0 >");
		#endif
        SetWidgetText(gv.titleBarLogId,gv.titleBuf);
    }
    
	
	//데이터 출력
	if(comPort==ADC_DATA_DISPLAY) {
		#if defined (SAMAN_ENABLE)
		for(int i=0; i<channel; i++) {
			//데이터 저장시 평균처리 사용한다.			
			if(i==2) {
				flash.Uart[USART_1]->SensorPV[i]=(float)flash.pulsCountPv;
				FloatPoint_ADCDInputDisplay(i, (float)flash.pulsCountPv);
			} 
			else {
				flash.Uart[USART_1]->SensorPV[i]=flash.Input[i]->PV;	
				FloatPoint_ADCDInputDisplay(i, flash.Input[i]->PV);
				
			}
			//Dprintf("PV[%d] = %.2f\n",i,flash.Uart[USART_1]->SensorPV[i]);
			
		}
		#endif
	}
    else if(channel==2) {
        FloatPoint_Display(0, comPort); 
        FloatPoint_Display(1, comPort); 
    }
    else if(channel==4) {		
        FloatPoint_Display(0, comPort); 
        FloatPoint_Display(1, comPort); 
        FloatPoint_Display(2, comPort); 
        FloatPoint_Display(3, comPort); 
    }
    else if(channel==6) {
		
		//flash.Uart[USART_1]->SensorPV[0]=1000000000.0f;
		
        FloatPoint_Display(0, comPort); 
        FloatPoint_Display(1, comPort); 
        FloatPoint_Display(2, comPort); 
        FloatPoint_Display(3, comPort); 
        FloatPoint_Display(4, comPort); 
        FloatPoint_Display(5, comPort); 
    }
    else if(channel==8) {
        FloatPoint_Display(0, comPort); 
        FloatPoint_Display(1, comPort); 
        FloatPoint_Display(2, comPort); 
        FloatPoint_Display(3, comPort); 
        FloatPoint_Display(4, comPort); 
        FloatPoint_Display(5, comPort); 
        FloatPoint_Display(6, comPort); 
        FloatPoint_Display(7, comPort); 
    }
    else if(channel==12) {
        #if defined (ULIK_CTD_ENABLE)
        FloatPoint_Display(0, comPort); 
        FloatPoint_Display(1, comPort); 
        FloatPoint_Display(2, comPort); 
        FloatPoint_Display(3, comPort); 
        FloatPoint_Display(4, comPort); 
        FloatPoint_Display(5, comPort); 
        FloatPoint_Display(6, comPort); 
        FloatPoint_Display(7, comPort); 
        FloatPoint_Display(8, comPort); 
        FloatPoint_Display(9, comPort); 
        FloatPoint_Display(10, comPort); 
        FloatPoint_Display(11, comPort); 
        #endif
        #if defined (DAECHEONGHO_SUGILGWANLISO)
        comPort=USART_1;
        DAECHEONGHO_FloatPoint_Display(0, comPort); 
        DAECHEONGHO_FloatPoint_Display(1, comPort); 
        DAECHEONGHO_FloatPoint_Display(2, comPort); 
        DAECHEONGHO_FloatPoint_Display(3, comPort); 
        DAECHEONGHO_FloatPoint_Display(4, comPort); 
        DAECHEONGHO_FloatPoint_Display(5, comPort); 
        comPort=USART_2;
        DAECHEONGHO_FloatPoint_Display(6, comPort); 
        DAECHEONGHO_FloatPoint_Display(7, comPort); 
        DAECHEONGHO_FloatPoint_Display(8, comPort); 
        DAECHEONGHO_FloatPoint_Display(9, comPort); 
        DAECHEONGHO_FloatPoint_Display(10, comPort); 
        DAECHEONGHO_FloatPoint_Display(11, comPort); 
        #endif
    }
    return 0;
}
extern int DebugScreenFunction(HANDLE parent, HANDLE handle);
extern int SmartLogSensorChannelFunction(HANDLE parent, HANDLE handle);
extern volatile char	gKeypadBuf[32];
int DataLog_Key_Event( HANDLE parent, HANDLE handle, HANDLE timer, HANDLE id ) { //화면전환
	switch( id ){
        case 1: {						
            StopTimer(timer);
            CloseTimer(timer);
            SensorMain_Window(0,parent); 					//SENSOR
        }
        break;
        case 2: {
            StopTimer(timer);
            CloseTimer(timer);			
//			char path[128];
//			SD_Mount();
//			sprintf(path,"%s/", Disk_Kind);	//!< 현재의 디렉토리를 표시하는방법 dir/
//			scan_files(path);
////			FileView("DumpTransmitFile2204.txt");
//			SD_UnMount();
			
			#if defined (YSI_ENABLE)				
				SmartLogSensorChannelFunction(0, parent); 	//6600 센서교정
			#else
				DebugScreenFunction(0,parent); 				//DEBUG
			#endif
        }
        break;
        case 3: {
            StopTimer(timer);
            CloseTimer(timer);
            SystemMain_Window(0,parent); 					//SYSTEM
        }
        break;
        case 4: {
            USB_Control(parent , handle); 					//USB popup winsow
        }
        break;
        case 5: {
			
        }
        break;
        case 6: {
            if(7==MainScreenCreate(NULL,parent)) 			//7을 리턴하면 제실행 한다.
            {
                MainScreenCreate(NULL,parent);	
            }
        }
        break;
        case 7: {
            
        }
        break;
        case 69:{
            //Dprintf("IMAGE CHECKING\n");
        }break;
    }
    return 1;
}

#ifdef __cplusplus
}
#endif


