#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"

extern int DumpFileCreate(RealTime saveDate);
/*
    5분평균데이터
*/
void MeasuredValueAveraging( int process ) {
    char buf[128];
    if(gv.averCnt5Min[USART_1]==0) { //1. 초기화.데이터가 저장된 후 초기화 된다.
        
        Dprintf("MeasuredValueAveraging :: 평균데이터 저장 및 초기화\n");
        for(int comPort=0; comPort<USART_MAX; comPort++) {
            for(int channel=0; channel<USART_SENSOR_MAX; channel++) {
                gv.averPV5Min[comPort][channel] = 0;
            }
        }
        gv.averCnt5Min[USART_1]=0;
		gv.averCnt5Min[USART_2]=0;
		gv.averCnt5Min[USART_3]=0;
		gv.averCnt5Min[USART_4]=0;
    }
    if(process==0) {        		//2. 누적           
        for(int comPort=0; comPort<USART_MAX; comPort++) {
            for(int channel=0; channel<USART_SENSOR_MAX; channel++) {
				if(gv.tmsStatusCode[comPort]==6) {
					gv.averPV5Min[comPort][channel] = 0;//통신 불량이 발생한 포트 데이터는 0으로 처리한다.
				}
				else if(gv.tmsStatusCode[comPort]==3) {
					gv.averPV5Min[comPort][channel] = 0;//교정중 일때 발생한 포트 데이터는 0으로 처리한다.
				}
				else {
					gv.averPV5Min[comPort][channel] += flash.Uart[comPort]->SensorPV[channel];
				}
            }
        }
		//동작불량이 발생하면 포인트가 증가하지 않는다.
        gv.averCnt5Min[USART_1]++;
		gv.averCnt5Min[USART_2]++;
		gv.averCnt5Min[USART_3]++;
		gv.averCnt5Min[USART_4]++;
    } else {                		//3. 평균 
        for(int comPort=0; comPort<USART_MAX; comPort++) {
            for(int channel=0; channel<USART_SENSOR_MAX; channel++) {                
                if(gv.averPV5Min[comPort][channel]!=0) { //측정값이 0이면 평균처리를 하지 않는다.
                    gv.averPV5Min[comPort][channel] /= gv.averCnt5Min[comPort];					
					
					Dprintf("MeasuredValueAveraging ::평균 누적수량[%d] USART[%d][%s] = %.2f\n"
					,gv.averCnt5Min[comPort],comPort,flash.Uart[comPort]->SetupName[channel],gv.averPV5Min[comPort][channel]);
                    sprintf(buf,"[5MIN]%s=%.2f\n",flash.Uart[comPort]->SetupName[channel],gv.averPV5Min[comPort][channel]);
                    DebugMessagePrintf(buf);        
                }
            }
        }
        gv.averCnt5Min[USART_1]=0;
		gv.averCnt5Min[USART_2]=0;
		gv.averCnt5Min[USART_3]=0;
		gv.averCnt5Min[USART_4]=0;
		MeasuredValue60MinAveraging(1); //60분 데이터 누적
    }
	#if defined (SAMAN_ENABLE)
	flash.Uart[USART_1]->averPV5Min[2]=(float)flash.pulsCountPv;//정수값은 카운트하지 않는다.
	#endif
}
/*
	60분평균데이터
*/
void MeasuredValue60MinAveraging(int process) {
	char buf[128];
	if(process==0) { //0. 초기화
        Dprintf("MeasuredValue60MinAveraging :: 평균데이터 저장 및 초기화\n");
        for(int comPort=0; comPort<USART_MAX; comPort++) {
            for(int channel=0; channel<USART_SENSOR_MAX; channel++) {
                gv.averPV60Min[comPort][channel] = 0;
            }
        }
        gv.averCnt60Min[USART_1]=0; //평균처리 횟수 3600개
		gv.averCnt60Min[USART_2]=0;
		gv.averCnt60Min[USART_3]=0;
		gv.averCnt60Min[USART_4]=0;		
		
		gv.tmsStatusCode60MinCalCnt[USART_1]=0;//교정횟수 카운트값
		gv.tmsStatusCode60MinCalCnt[USART_2]=0;
		gv.tmsStatusCode60MinCalCnt[USART_3]=0;
		gv.tmsStatusCode60MinCalCnt[USART_4]=0;

		gv.tmsStatusCode60minComErrCnt[USART_1]=0;//통신에러 카운트값
		gv.tmsStatusCode60minComErrCnt[USART_2]=0;
		gv.tmsStatusCode60minComErrCnt[USART_3]=0;
		gv.tmsStatusCode60minComErrCnt[USART_4]=0;
	}
	if(process==1) { //1. 누적
        for(int comPort=0; comPort<USART_MAX; comPort++) {
            for(int channel=0; channel<USART_SENSOR_MAX; channel++) {                
                if(gv.averPV5Min[comPort][channel]!=0) { 	//측정값이 0이면 평균처리를 하지 않는다.
                    if(gv.tmsStatusCode[comPort]==6) { 		//동작불량 일때는 데이터를 누적하지 않는다. 이전데이터만 유지한다.						
					}
					else if(gv.tmsStatusCode[comPort]==3) { //교정중 일때는 데이터를 누적하지 않는다. 이전데이터만 유지한다.						
					}
					else {
						gv.averPV60Min[comPort][channel] += gv.averPV5Min[comPort][channel]; //5분평균데이터를 60분평균데이터에 저장한다.
					}
					Dprintf("MeasuredValue60MinAveraging ::평균 누적수량[%d] USART[%d][%s] = %.2f\n"
					,gv.averCnt60Min[comPort],comPort,flash.Uart[comPort]->SetupName[channel],gv.averPV60Min[comPort][channel]);
                    sprintf(buf,"[60MIN]%s=%.2f\n",flash.Uart[comPort]->SetupName[channel],gv.averPV60Min[comPort][channel]);
                    DebugMessagePrintf(buf);        
                }
            }
        }
		if(gv.tmsStatusCode[USART_1]==0) { //정상 동작 코드 에서만 데이터 누적 횟수를 증가한다.
			gv.averCnt60Min[USART_1]++;
		}
		if(gv.tmsStatusCode[USART_2]==0) {
			gv.averCnt60Min[USART_2]++;
		}
		if(gv.tmsStatusCode[USART_3]==0) {
			gv.averCnt60Min[USART_3]++;
		}
		if(gv.tmsStatusCode[USART_4]==0) {
			gv.averCnt60Min[USART_4]++;
		}
	}
	if(process==2) { //2. 평균처리
        for(int comPort=0; comPort<USART_MAX; comPort++) {
            for(int channel=0; channel<USART_SENSOR_MAX; channel++) {                
                if(gv.averPV60Min[comPort][channel]!=0) { //측정값이 0이면 평균처리를 하지 않는다.
                    if(gv.tmsStatusCode60MinCalCnt[comPort]>=3) {//교정 횟수가 3회 이상이면 데이터를 0처리한다.
						gv.averPV60Min[comPort][channel]=0;
					} 
					else if(gv.tmsStatusCode60minComErrCnt[comPort]>=3) {//통신 불량이 3회 발생하면 측정값을 0으로 처리한다.
						gv.averPV60Min[comPort][channel]=0;
					}
					else {
						gv.averPV60Min[comPort][channel] /= gv.averCnt60Min[comPort];
					}
                    Dprintf("MeasuredValue60MinAveraging ::평균 누적수량[%d] USART[%d][%s] = %.2f\n"
					,gv.averCnt60Min[comPort],comPort,flash.Uart[comPort]->SetupName[channel],gv.averPV60Min[comPort][channel]);
                    sprintf(buf,"[60MIN]%s=%.2f\n",flash.Uart[comPort]->SetupName[channel],gv.averPV60Min[comPort][channel]);
                    DebugMessagePrintf(buf);        
                }
            }
        }
	}
}
void ModemPowerControl(int port, int status) {//데이터 저장 후 모뎀파워제어를 실행한다.
	#ifdef MODEM_POWER_CONTROL
	if(port>=0 && port<7) {
		if(status==ON) {
			if(flash.wakeUpTimeModem==0){
				flash.relay[port]=status;
				Dprintf("모뎀 항상 ON\n");
			}
			else if(flash.wakeUpTimeModem==30) {
				if(currentDate.minute==58) {
					flash.relay[port]=status;
					Dprintf("모뎀 30분 ON\n");
				}
				else if(currentDate.minute==28) {
					flash.relay[port]=status;
					Dprintf("모뎀 30분 ON\n");
				}
				else {
					if(gv.lanStatus[0]==0 && currentDate.minute == 35) {//7분동안 접속이안되는 경우 power off
						flash.relay[port]=OFF;
					}
					if(gv.lanStatus[0]==0 && currentDate.minute == 5) {
						flash.relay[port]=OFF;
					}
				}
			}
			else if(flash.wakeUpTimeModem==60) {
				if(currentDate.minute==58) {
					flash.relay[port]=status;
					Dprintf("모뎀 60분 ON\n");
				}
				else if(gv.lanStatus[0]==0 && currentDate.minute == 5) {
					flash.relay[port]=OFF;
				}
			}				
		}
		else {
			flash.relay[port]=status;
			Dprintf("모뎀 OFF\n");
		}
	}
	else {
		Dprintf("모뎀 제어 포트 설정 오류\n");
	}
	#endif
}
void SaveTimeProcess(void) {
   if(flash.onlyOneSaveFlag == 222) {
        Dprintf("SaveMesaurements :: 5분전시간저장\n");
        flash.onlyOneSaveFlag = 0;
        flash.saveMinute = currentDate.minute;
        //5분전시간을 저장한다.
		flash.dumpTimeSaveFlag=1;
        flash.gDataSave5MinFlag=0;	//5분데이터초기화
        flash.gDataSave60MinFlag=0; //60분데이터초기화
        flash.Save60TimeSaveFlag=0;        
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
    }
    if(flash.saveMinute != currentDate.minute) {
		flash.saveMinute = currentDate.minute;//다음 시간을 기약한다.	
		ModemPowerControl(RELAY_1, ON);//모뎀파워제어
        
		//flash.saveMinuteSv=1;
		
		if(flash.saveMinuteSv==1) {//1분데이터
            flash.gDataSave5MinFlag=1;
			if(currentDate.minute==0) {//60분 데이터 저장
				if(flash.Save60TimeSaveFlag == 1) { //60분데이터를 저장해야 하는 시점인가 확인한다.
					flash.gDataSave60MinFlag = 1;//60분데이터를 저장한다.
				}
				else {
					flash.save60Date = currentDate;//flash.save60Date 시간은 데이터가 출력 될 때 사용되는 시간
					flash.Save60TimeSaveFlag = 1;//다음 60분이되면 데이터를 저장해라
				}
			}
        }
        else if(flash.saveMinuteSv==5) {//5분데이터
            switch( currentDate.minute )
            {
                case 0:  //시간데이터에서 들어온다.
                {
                    flash.gDataSave5MinFlag = 1;
                    if(flash.Save60TimeSaveFlag == 1) { //60분데이터를 저장해야 하는 시점인가 확인한다.
                        flash.gDataSave60MinFlag = 1;//60분데이터를 저장한다.
                    }
                    else {
                        flash.save60Date = currentDate;//flash.save60Date 시간은 데이터가 출력 될 때 사용되는 시간
                        flash.Save60TimeSaveFlag = 1;//다음 60분이되면 데이터를 저장해라
                    }
                }
                break; //누적데이터
                case 5  : flash.gDataSave5MinFlag=1;    break;            
                case 10 : flash.gDataSave5MinFlag=1;	break;            
                case 15 : flash.gDataSave5MinFlag=1;	break;			
                case 20 : flash.gDataSave5MinFlag=1;	break;			
                case 25 : flash.gDataSave5MinFlag=1;	break;			
                case 30 : flash.gDataSave5MinFlag=1;	break;		
                case 35 : flash.gDataSave5MinFlag=1;	break;
                case 40 : flash.gDataSave5MinFlag=1;	break;
                case 45 : flash.gDataSave5MinFlag=1;	break; 
                case 50 : flash.gDataSave5MinFlag=1;	break;
                case 55 : flash.gDataSave5MinFlag=1;	break;
                default: break;
            }
        }
        else if(flash.saveMinuteSv==10) {//10분데이터
            switch( currentDate.minute )
            {
                case 0:  //시간데이터에서 들어온다.
                {
                    flash.gDataSave5MinFlag = 1;
                    if(flash.Save60TimeSaveFlag == 1) { //60분데이터를 저장해야 하는 시점인가 확인한다.
                        flash.gDataSave60MinFlag = 1;	//60분데이터를 저장한다.
                    }
                    else {
                        flash.save60Date = currentDate;//flash.save60Date 시간은 데이터가 출력 될 때 사용되는 시간
                        flash.Save60TimeSaveFlag = 1;//다음 60분이되면 데이터를 저장해라
                    }
                }
                break; //누적데이터
                case 10 : flash.gDataSave5MinFlag=1;	break;            
                case 20 : flash.gDataSave5MinFlag=1;	break;			
                case 30 : flash.gDataSave5MinFlag=1;	break;		
                case 40 : flash.gDataSave5MinFlag=1;	break;
                case 50 : flash.gDataSave5MinFlag=1;	break;
                default: break;
            }
        }
//		if(currentDate.minute==4||currentDate.minute==9||currentDate.minute==14||currentDate.minute==19||currentDate.minute==24
//			||currentDate.minute==29||currentDate.minute==34||currentDate.minute==39||currentDate.minute==44||currentDate.minute==49||currentDate.minute==54||currentDate.minute==59) {
//			gv.printOutStorageTimeFlag=1;//데이터저장대기시간을 출력한다.
//		} else {
//			gv.printOutStorageTimeFlag=0; 
//		}
    }    
}
/*
    5분데이터저장
*/
void SaveMesaurements(void) {
    SaveTimeProcess();                          	//1, 5, 10분주기 데이터저장 ( FLAG ) 
    MeasuredValueAveraging(0);                  	//5분데이터누적시킨다.
    if(flash.gDataSave5MinFlag==1)              	//5분데이터저장
    {
		flash.gDataSave5MinFlag=0;
        if(flash.dumpTimeSaveFlag == 0)         	//5분전 시간이 저장되어 있으면 데이터가 저장되기 시작한다.
        {			
            MeasuredValueAveraging(1);          	//누적된 측정치를 평균을 낸다.                        
			//DUMP 파일생성
			DumpFileCreate(flash.saveDate);     	//송신파일생성            
            
			SaveTheMeasurement(5);              	//5분데이터저장 
			flash.saveDate = currentDate;			//5분전시간을 저장한다. 매우중요
			
			if(gv.tmsStatusCode[USART_1]==6) {		
				gv.tmsStatusCode[USART_1]=0;			
				gv.tmsStatusCode60minComErrCnt[USART_1]++;	//1시간 데이터에 5분통신불량이 3회이상 발생하면 통신불량 데이터 처리한다.
				EventMessage("UART1 통신에러발생");
				DebugMessagePrintf("UART1 통신에러발생\n");
			}	
			if(gv.tmsStatusCode[USART_2]==6) {		
				gv.tmsStatusCode[USART_2]=0;
				gv.tmsStatusCode60minComErrCnt[USART_2]++;
				EventMessage("UART2 통신에러발생");
				DebugMessagePrintf("UART2 통신에러발생\n");
			}	
			if(gv.tmsStatusCode[USART_3]==6) {		
				gv.tmsStatusCode[USART_3]=0;
				gv.tmsStatusCode60minComErrCnt[USART_3]++;
				EventMessage("UART3 통신에러발생");
				DebugMessagePrintf("UART3 통신에러발생\n");
			}	
			if(gv.tmsStatusCode[USART_4]==6) {		
				gv.tmsStatusCode[USART_4]=0;
				gv.tmsStatusCode60minComErrCnt[USART_4]++;
				EventMessage("UART4 통신에러발생");
				DebugMessagePrintf("UART4 통신에러발생\n");
			}	
			if(gv.tmsStatusCode[USART_1]==3) {		
				gv.tmsStatusCode60MinCalCnt[USART_1]++;//3회이상 카운트 되면 교정 중으로 출력한다.
			}
			if(gv.tmsStatusCode[USART_2]==3) {
				gv.tmsStatusCode60MinCalCnt[USART_2]++;
			}
			if(gv.tmsStatusCode[USART_3]==3) {
				gv.tmsStatusCode60MinCalCnt[USART_3]++;
			}
			if(gv.tmsStatusCode[USART_4]==3) {
				gv.tmsStatusCode60MinCalCnt[USART_4]++;
			}
			if(flash.gDataSave60MinFlag==1) {			
				flash.gDataSave60MinFlag=0;				
				MeasuredValue60MinAveraging(2);			//60분 데이터 평균
				SaveTheMeasurement(60);					//60분 데이터 저장
				MeasuredValue60MinAveraging(0);			//60분 데이터 초기화
				flash.save60Date = currentDate;			//60분전시간을 저장한다. 매우중요
			}
        }
        else if(flash.dumpTimeSaveFlag == 1)		//5분전 시간을 저장 후 다시 들어오지 않는다.
        {
            flash.dumpTimeSaveFlag = 0;				//5분이 지나고 다음시간부터 데이터를 저장하기 시작한다.
            flash.gDataSave5MinFlag = 0;			//첫번째 데이터는 저장하지 않는다.
            flash.gDataSave60MinFlag = 0;			//60분데이터도 저장하지 않는다.
            flash.saveMinute = currentDate.minute;
			flash.saveDate = currentDate;			//5분전 데이터시간을 여기서 저장한다.

        }        
		DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
    }
}
