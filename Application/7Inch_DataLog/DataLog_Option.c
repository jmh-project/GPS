#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"

/******************************************************

    1. RTU 재부팅을 실행한다.

******************************************************/
extern volatile U32 W5500_IO_PROCESS;
void SystemRebootGo(void) {
	int waitCount=0;

    IOStopTimer(W5500_IO_PROCESS);
	DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
	//Delay(1000);
    //FillRect(0, 0, 1024, 768);	
	LCD_OFF();		

    EventMessage("시스템리부팅시작");
    
	NVIC_SystemReset();
	
	Dprintf("데이터저장완료\n");
	
	while(1)
	{
		Uart_SendByte7(0x02);
		Uart_SendByte7('P');
		Uart_SendByte7('O');
		Uart_SendByte7('W');
		Uart_SendByte7('E');
		Uart_SendByte7('R');
		Uart_SendByte7(0x03);
		Dprintf("명령에의한리셋\n");						
		if(waitCount>=10) {
			Dprintf("NVIC_SystemReset\n");
			NVIC_SystemReset();            
		} else {
			waitCount++;
			Delay(1000);
		}
	}
}
/****************************************************

	2. 설정시간이 되면 리셋이 된다.
		- DataLog_Hardware_IOProcess.c
		- 혹시나 화면이 깨지면 복원을 하기 위해서이다.
		- 25 가설정이 되면 동작하지 않는다.

****************************************************/
int RebootSetTime(void) {
    if(flash.autoRebootHourSv>=25) {
        //자동리셋을 적용하지 않는다.
    } else if(currentDate.hour==flash.autoRebootHourSv) {
        if(flash.lcdInitFlag==0 && currentDate.minute == 2) {
            flash.lcdInitFlag=1;
            EventMessage("지동리부팅실행");
            SystemRebootGo();
        }
    } else {
        if(flash.lcdInitFlag != 0) {     
            flash.lcdInitFlag=0;
            DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        }
    } 
    return 0;
}
/******************************************************

    3. 설정한 시간이되면 슬립모드로 넘어간다.

******************************************************/
extern volatile U16 	g_Color;
int EntrySleepMode(void) {
	if(flash.sleepModeFlag==1) {	
		if(flash.powerControl!=2) {
			gv.sleepModeTimerCntSecPv++;//1초마다 증가한다.
			//Dprintf("슬립모드 = %d\n",gv.sleepModeTimerCntSecPv);
			if(gv.sleepModeTimerCntSecPv >= 600) {//10분후에 저절력모드로 넘어간다.
			//if(gv.sleepModeTimerCntSecPv >= 10) {	
				flash.powerControl=2;// 화면을 터치하면 POWER 가 ON된다.
				EventMessage("슬립모드진입");
                LCD_OFF();
                //g_Color=clBlack;	
                //Rectangle(0, 0, 1024, 768);	
				
                SystemRebootGo();
			}
			if(gv.sleepModeTimerCntSecPv==(600-10)) {   
                
				DataLog_MessageBox_Window("절전모드", "절전모드 진입 10초전입니다.");
			}
		} else {
			gv.sleepModeTimerCntSecPv = 0;
		}
	}	
    return 0;
}

/******************************************************

    4. 슬립모드에서 깨어난다.

******************************************************/
void BSP_ScreenClear(void);
int SystemWakeUp(void) {
    if(gv.wakeUpFlag==1) { //gWakeUpFlag 터치용
        gv.wakeUpFlag=0;
        //Dprintf("SWITCH = %d\n",flash.powerControl);
        if(flash.powerControl == 2)
        {
            Dprintf("시스템 리부팅을 시작합니다.\n");
            flash.powerControl=0;
            GPU_PowerControl(flash.powerControl);
            Delay(10);
            TeecoBmpDMA_Config();
            BSP_SM718Init();
            SM718_2DInit();
            Init2D();
            BSP_ScreenClear();
            SetGraphicsPage(LCD_SCREEN);
            g_Color=clWhite;	
            LoadAliasingFont("ng_eh_24.sif");
            SetFontAliasing("ng_eh_24.sif");	
            TextOutAliasing(100, ((768-32)/2) - 30, "Wake up", clWhite, 0, "ng_eh_24.sif");	
            Rectangle(100, (768-32)/2, 824, 32);	
            LCD_ON();		
            Delay(10);
            FillRect(100, (768-32)/2, 824/3, 32);	
            Delay(10);
            FillRect(100, (768-32)/2, 824/2, 32);	
            Delay(10);            
            FillRect(100, (768-32)/2, 824, 32);	
            Delay(10);
            g_Color=clBlack;
			
            SystemRebootGo();
        }
    }
    return 0;
}


void TouchChecking(void) {
    //int waitCount=0;
    gv.touchReceiveErrorCount++;
    if(gv.touchReceiveErrorCount>240) {//4분간 터치 데이터가 송신이 안되면 초기화 된다.
        gv.touchReceiveErrorCount=0;
        EventMessage("터치점검진행");
        SystemRebootGo();
    }
    if(gv.touchRebootFlag==1) {
        gv.touchRebootFlag=0;
        EventMessage("STM32L070 재실행됨");
    }
}
void StructCheckingMode(void) {
    //int waitCount = 0;
    if(flash.StartVerify != 1234 || flash.EndVerify != 5678) {
        if(flash.powerControl!=2) {	
            DataLog_MessageBox_Window("ERROR1", "자료구조파일에 문제가발생 초기화합니다.");
        }
        Dprintf("flash.StartVerify = %d flash.EndVerify = %d\n",flash.StartVerify,flash.EndVerify);
        EventMessage("구조체파일검사 flash");
        SystemRebootGo();
    }    
    if(gv.StartVerify != 1234 || gv.EndVerify != 5678) {
        if(flash.powerControl!=2) {	
            DataLog_MessageBox_Window("ERROR2", "자료구조파일에 문제가발생 초기화합니다.");
        }
        EventMessage("구조체파일검사 gv");
        SystemRebootGo();
    }    
}

void UartConnectErrorCheckingMode(void) {//5분 데이터 처리용으로 사용한다.    
    if(flash.Uart[USART_1]->SensorID != SENSOR_NONE) {
		gv.comportConnectionError[USART_1]++;
        
		if(gv.comportConnectionError[USART_1]>75) {//75%이상이면 데이터 손실로본다.		   
            gv.serialComStatusToggle[USART_1]=3;	//LED 이미지 출력시 사용한다.
			gv.comportConnectionError[USART_1]=0;
			gv.tmsStatusCode[USART_1]=6;			//데이터저장후 0이된다.
            

            
			for(int i=0;i<USART_SENSOR_MAX;i++) {	//EXO 통신에러 발생시 0처리한다.
				flash.Uart[USART_1]->SensorPV[i]=0;
			}
			#ifdef EXO_ENABLE
			if(flash.Uart[USART_1]->SensorID == SENSOR_EXO) {
				for(int i=0;i<EXO_MAX_ITEM;i++) {//EXO 측정값 출력수가 최대 70개로 본다.
					flash.EXO.EXO_Data[USART_1][i]=0;//MAX 70 CHANNEL
				}
			}
			#endif
		} 
	}
	else {
		gv.comportConnectionError[USART_1]=0;
	}
	if(flash.Uart[USART_2]->SensorID != SENSOR_NONE) {
		gv.comportConnectionError[USART_2]++;
		if(gv.comportConnectionError[USART_2]>75) {
			gv.comportConnectionError[USART_2]=0;
			gv.tmsStatusCode[USART_2]=6;
		}
	}
	else {
		gv.comportConnectionError[USART_2]=0;
	}
	if(flash.Uart[USART_3]->SensorID != SENSOR_NONE) {
		gv.comportConnectionError[USART_3]++;
		if(gv.comportConnectionError[USART_3]>75) {
			gv.comportConnectionError[USART_3]=0;
			gv.tmsStatusCode[USART_3]=6;
		}
	}
	else {
		gv.comportConnectionError[USART_3]=0;
	}
	if(flash.Uart[USART_4]->SensorID != SENSOR_NONE) {
		gv.comportConnectionError[USART_4]++;
		if(gv.comportConnectionError[USART_4]>60) {
			gv.comportConnectionError[USART_4]=0;
			gv.tmsStatusCode[USART_4]=6;
		}
	}
	else {
		gv.comportConnectionError[USART_4]=0;
	}
}

