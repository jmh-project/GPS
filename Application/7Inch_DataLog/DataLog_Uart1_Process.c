#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"
int Uart1ReceiveProcess(void) {
    int comPort = USART_1;
    if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG) {
        char buf[64];
        sprintf(buf,"Debug Uart%d ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\r", comPort+1);
        TransmitStringData(buf, comPort);
    } 
    #ifdef SPECTRO_D_ENABLE
    else if(flash.Uart[comPort]->SensorID == SENSOR_SPECTRO_D) {
        ULIK_IOProcess(comPort);
    }
    #endif
    #if defined(SPECTRO_M_ENABLE)
	else if(flash.Uart[comPort]->SensorID == SENSOR_SPECTRO_D) {
        ULIK_MW_IOProcess(comPort);
	}
	#endif
	#if defined (PONSEL_EC)
	else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_EC) {
		static int ponselCount=0;
		if(flash.ponsel_ec.calStartFlag==0) { //교정중에는 측정값을 요청하지 않는다.
			switch( ponselCount )
			{
				case 0: PONSEL_EC_Status(comPort,0x0A); break;
				//case 1: PONSEL_EC_Status(comPort,0x0B); break;
				//case 2: PONSEL_EC_Status(comPort,0x0C); break;
				//case 3: PONSEL_EC_Status(comPort,0x0D); break;
				case 1: PONSEL_EC_Request(comPort,0x0A); break;
				//case 5: PONSEL_EC_Request(comPort,0x0B); break;
				//case 6: PONSEL_EC_Request(comPort,0x0C); break;
				//case 7: PONSEL_EC_Request(comPort,0x0D); break;
			}
			ponselCount++;
			if(ponselCount >= 2) ponselCount = 0;
		}
	}
	#endif
	#if defined (YSI_ENABLE)
	else if(gv.nFlag_Sensor_DataRead[comPort]==SCREEN_MODE) {       // 현재 상태가 SCREEN MODE 상태인가
		if(flash.Uart[comPort]->SensorID == SENSOR_YSI )
		{
			if(gv.nFlag_ReceiveDataProcess[comPort]==1)//데이터 수신을 완료한 경우 
			{
                gv.ysiResetStep[comPort] = 0;
				fnSensorReceiveValueProcess(comPort);//데이터 에러처리 루틴이 있다. 
				gv.nFlag_ReceiveDataProcess[comPort] = 0;//수신데이터 처리완료
				gv.comportConnectionError[comPort] = 0;//YSI 리셋타이머 초기화					
			}
			else
			{     
				if(gv.comportConnectionError[comPort] >= 45)// SmartLogFlash.ysiPowerControlTimerSV설정된 시간동안 데이터가 안들어오면 ysi 파워를 끈다.
				{
                    
                    switch(gv.ysiResetStep[comPort])
                    {
                        case 0: Uart_TransmitPort( comPort, 0x1B ); break;
                        case 1: Uart_TransmitPort( comPort, 0x1B ); break;
                        case 2: Uart_TransmitPort( comPort, 0x1B ); break;
                        case 3: Uart_TransmitPort( comPort, 'r' ); 
                                  Uart_TransmitPort( comPort, 'u' ); 
                                  Uart_TransmitPort( comPort, 'n' ); 
                                  Uart_TransmitPort( comPort, 0x0D ); 
                        break;
                    }
                    if(gv.ysiResetStep[comPort]==3)
                    {
                        gv.ysiResetStep[comPort] = 0;
                        gv.comportConnectionError[comPort] = 0;
                        //Dprintf("YSI POWER RESET[ USART_%d ]\n ",comPort +1);
                    }
                    else
                        gv.ysiResetStep[comPort]++;
				}
			}
		}
	}		
	#endif
    return 0;
}

