#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"
/*
	TMS 테스트 코드
	
*/
int Uart4ReceiveProcess(void) {
	int comPort = USART_4;
    #ifdef KECO_PROTOCOL_ENABLE
	#if defined(TYPE_APPROVAL_ENABLE) 
		//ExopHSlope(gv.tms_pHmV);//교정식을 적용하지 않는다.
	#endif
    if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG) {
        char buf[64];
        sprintf(buf,"Debug Uart%d ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\r", comPort+1);
        TransmitStringData(buf, comPort);
    } 
    else if(flash.Uart[comPort]->SensorID == SENSOR_KECO) {			//일반송신
		if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_DATA) {		//실시간 데이터
			KECO_ProtocolSending(comPort);
			gv.nFlag_ReceiveDataProcess[comPort]=KECO_RESPONSE;
		} 
        else if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_DUMA) {	//덤프 데이터
            if(gv.tmsDumpStepCnt==2) {//1회실행
				EventMessage("KECO:덤프 데이터 송신요청");
                DUMP_SDCardTOBuffer(gv.dumpStartDate,gv.dumpEndDate);
            }
            if(gv.tmsDumpStepCnt==4) {//재송신
                for( int i=0; i<gv.lanDataTransmitSize; i++) { 			//측정값 재송신
                    Uart_TransmitPort(comPort, gv.transmitBuf[i]);
                }
                gv.tmsDumpStepCnt=3;
                gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA_RESPONSE;//응답
            }
            else {
                int status=DUMP_BufferTOSend(gv.dumpStartDate,gv.dumpEndDate,gv.tmsDumpStepCnt);	//데이터를 검색하여 송신까지한다.
                gv.tmsDumpStepCnt=3;
                gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA_RESPONSE;
                if(status==RESULT_NOFILE) {
                    gv.nFlag_ReceiveDataProcess[comPort]=0;
                    Uart_TransmitPort( comPort, 0x04);//eot send
                    gv.tmsDumpStepCnt=0;
                    Dprintf("송신완료\n");
                } 
                else {
                    for( int i=0; i<gv.lanDataTransmitSize; i++) { //측정값 재송신
                        Uart_TransmitPort(comPort, gv.transmitBuf[i]);
                    }
                }
            }
		} 
		else if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_RESET) {
			gv.nFlag_ReceiveDataProcess[comPort]=0;
			EventMessage("KECO:시스템리셋실행");
			SystemRebootGo();
		}
    }
	else if(flash.Uart[comPort]->SensorID == SENSOR_TMS) {					//형식승인
		#if defined(TYPE_APPROVAL_ENABLE)
		if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_DATA) {				//실시간 데이터 송신
			TYPE_APPROVAL_ProtocolSending(comPort);
			gv.nFlag_ReceiveDataProcess[comPort]=KECO_RESPONSE;
		} 
        else if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_DUMA) {			//덤프 데이터 송신 모드 진입
            if(gv.tmsDumpStepCnt==2) {										//1회실행 :: SDCARD 데이터를 읽어온다.
				EventMessage("TMS:덤프 데이터 송신요청");
                DUMP_SDCardTOBuffer(gv.dumpStartDate,gv.dumpEndDate);		
            }			
            if(gv.tmsDumpStepCnt==4) {										//재송신
                for( int i=0; i<gv.lanDataTransmitSize; i++) {				 
                    Uart_TransmitPort(comPort, gv.transmitBuf[i]);
                }
                gv.tmsDumpStepCnt=3;
                gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA_RESPONSE;	//응답 대기 코드
            }
            else {
                int status=DUMP_BufferTOSend(gv.dumpStartDate,gv.dumpEndDate,gv.tmsDumpStepCnt);	//덤프 데이터 송신
                gv.tmsDumpStepCnt=3;			 													//덤프 데이터 송신 후 응답대기 코드
                gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA_RESPONSE;
                if(status==RESULT_NOFILE) {															//덤프 데이터가 없는 경우 송신 종료 0x04 송신
                    gv.nFlag_ReceiveDataProcess[comPort]=0;
                    Uart_TransmitPort( comPort, 0x04);//eot send
                    gv.tmsDumpStepCnt=0;
                    Dprintf("송신완료\n");
                } 
                else {
                    for( int i=0; i<gv.lanDataTransmitSize; i++) { 									//덤프 데이터 송신
                        Uart_TransmitPort(comPort, gv.transmitBuf[i]);
                    }
                }
            }
		}
		else if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_RESET) {
			gv.nFlag_ReceiveDataProcess[comPort]=0;
			EventMessage("TMS:시스템리셋실행");
			SystemRebootGo();
		}
		else if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_SETT) {
			gv.nFlag_ReceiveDataProcess[comPort]=0;
			EventMessage("TMS:시스템시간이 변경되였습니다.");
		}
		else if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_PASS) {
			gv.nFlag_ReceiveDataProcess[comPort]=0;
			EventMessage("TMS:비밀번호가 변경되였습니다.");
			DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
			//비밀번호 변경후 응답 데이터를 송신한다.
			TYPE_APPROVAL_PasswordResponse(comPort);
		}
		#endif
	}
    #endif
    #ifdef MODBUS_ENABLE
    if(flash.Uart[comPort]->SensorID==SENSOR_MODBUS) {
        if(gv.nFlag_ReceiveDataProcess[comPort]==ENABLE) {
            SendModbus(comPort);
            gv.nFlag_ReceiveDataProcess[comPort]=DISABLE;
        }
    }
    #endif
    return 0;
}










