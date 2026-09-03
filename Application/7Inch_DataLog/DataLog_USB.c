
/******************************************************************************
*    	File name	: SmartLogStart.c 
*		Description : DongMun RH100 Control Program
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: 
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"
#include "diskio.h"		/* Declarations of low level disk I/O functions */
#include "usbh_usr.h"
#include "ff.h"       /* FATFS */
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "Debug_Uart.h"
#include "usbh_core.h"


//unsigned char 		Disk_Buffer[4096];
/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/
/******************************************************************************
							Function Prototype
******************************************************************************/
extern void BusanTransmitData(unsigned int time, unsigned char *data);
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;
extern volatile FATFS sd_fatfs;
extern volatile FIL sd_file;
extern void USBH_Init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID,
               USBH_HOST *phost,               
               USBH_Class_cb_TypeDef *class_cb, 
               USBH_Usr_cb_TypeDef *usr_cb);

//#define _MAX_SS    4096  /* 512, 1024, 2048 or 4096 */ ffconf.h 파일에서 수정한다.
int SaveTheMeasurement( unsigned int time )
{
	char pBuf[128];
    char strName[64];

	for(int i=0;i<SMARTLOG_DATALOG_SIZE;i++) {
		gv.dataLogBuf[i]=0;
	}
    for(int i=0;i<128;i++)pBuf[i]=0;
    
    if(time == 1) {//실시간 데이터
    	sprintf((char *)gv.dataLogBuf,"DATE,20%02d/%02d/%02d %02d:%02d:%02d,", 
        currentDate.year,  
        currentDate.month, 
        currentDate.date, 
        currentDate.hour,  
        currentDate.minute, 
        currentDate.sec);
        for(int i=0;i<128;i++)pBuf[i]=0;
        for(int comPort=0; comPort<USART_MAX; comPort++) {
            for(int channel=0; channel<flash.ScreenCnt[comPort]; channel++) {
                if( strcmp(flash.Uart[comPort]->SetupName[channel],"NONE") != 0) {
                    FloatPoint_RealProcess(channel,comPort,pBuf);            
                    strcat((char *)gv.dataLogBuf,pBuf);
					TMSStatusCode(channel,comPort,pBuf);
					strcat((char *)gv.dataLogBuf,pBuf);
                }
            }
        }
    }
    else if(time==5) {//5분 데이터
        sprintf((char *)gv.dataLogBuf,"DATE,20%02d/%02d/%02d %02d:%02d:%02d,", flash.saveDate.year,  flash.saveDate.month, flash.saveDate.date, flash.saveDate.hour,  flash.saveDate.minute, 0);
		for(int comPort=0; comPort<USART_MAX; comPort++) {//USART
            for(int channel=0; channel<flash.ScreenCnt[comPort]; channel++) {
                if( strcmp(flash.Uart[comPort]->SetupName[channel],"NONE") != 0) {
                    FloatPoint_AverageProcess(channel,comPort,pBuf);            					
                    strcat((char *)gv.dataLogBuf,pBuf);
					TMSStatusCode(channel,comPort,pBuf);
					strcat((char *)gv.dataLogBuf,pBuf);
                }
            }
        }		
//		for(int channel=0; channel<8; channel++) { //ADCInput
//			if(strcmp(flash.Input[channel]->Name,"NONE") != 0) {
//				FloatPoint_ADInputAverageProcess(channel,USART_1,pBuf);
//				strcat((char *)gv.dataLogBuf,pBuf);
//			}
//		}
		#if defined(TYPE_APPROVAL_ENABLE)//형식승인 FACTOR 값저장
		if(flash.tmsDataName[1][0]=='P'&&flash.tmsDataName[1][1]=='H'&&flash.tmsDataName[1][2]=='Y') {
			TYPE_APPROVAL_FactorSave("MSIG",gv.tms_pHmV);
			TYPE_APPROVAL_FactorSave("MTM1",gv.averPV5Min[USART_1][0]);
			TYPE_APPROVAL_FactorSave("ZERO",flash.codeZERO[0]);
			TYPE_APPROVAL_FactorSave("SPAN",flash.codeSPAN[0]);
			TYPE_APPROVAL_FactorSave("FACT",flash.codeFACT[0]);
			TYPE_APPROVAL_FactorSave("OFST",flash.codeOFST[0]);
			TYPE_APPROVAL_FactorSave("MAXR",flash.codeMAXR[0]);
			TYPE_APPROVAL_FactorSave("AUXI",0);
		}
		else if(flash.tmsDataName[1][0]=='D'&&flash.tmsDataName[1][1]=='O'&&flash.tmsDataName[1][2]=='W') {
			//TYPE_APPROVAL_FactorSave("AUXI",0);
			TYPE_APPROVAL_FactorSave("MSIG",0);
			TYPE_APPROVAL_FactorSave("MTM1",gv.averPV5Min[USART_1][0]);
			TYPE_APPROVAL_FactorSave("ZERO",flash.codeZERO[1]);
			TYPE_APPROVAL_FactorSave("SPAN",flash.codeSPAN[1]);
			TYPE_APPROVAL_FactorSave("FACT",flash.codeFACT[1]);
			TYPE_APPROVAL_FactorSave("OFST",flash.codeOFST[1]);
			TYPE_APPROVAL_FactorSave("MAXR",flash.codeMAXR[1]);
			TYPE_APPROVAL_FactorSave("AUXI",0);
		}
		#endif
    }
    else if(time==60) {//60분 데이터
		
        sprintf((char *)gv.dataLogBuf,"DATE,20%02d/%02d/%02d %02d:%02d:%02d,", flash.save60Date.year,  flash.save60Date.month, flash.save60Date.date, flash.save60Date.hour, 0, 0);
        for(int comPort=0; comPort<USART_MAX; comPort++) {
            for(int channel=0; channel<flash.ScreenCnt[comPort]; channel++) {
                if( strcmp(flash.Uart[comPort]->SetupName[channel],"NONE") != 0) {
                    FloatPoint_60MinAverageProcess(channel,comPort,pBuf);            
                    strcat((char *)gv.dataLogBuf,pBuf);
					TMSStatusCode60Min(channel,comPort,pBuf);
					strcat((char *)gv.dataLogBuf,pBuf);
                }
            }
        }
		#if defined(TYPE_APPROVAL_ENABLE)//형식승인 FACTOR 값저장
		if(flash.tmsDataName[1][0]=='P'&&flash.tmsDataName[1][1]=='H'&&flash.tmsDataName[1][2]=='Y') {
			TYPE_APPROVAL_FactorSave("MSIG",gv.tms_pHmV);
			TYPE_APPROVAL_FactorSave("MTM1",flash.Uart[USART_1]->SensorPV[0]);
			TYPE_APPROVAL_FactorSave("ZERO",flash.codeZERO[0]);
			TYPE_APPROVAL_FactorSave("SPAN",flash.codeSPAN[0]);
			TYPE_APPROVAL_FactorSave("FACT",flash.codeFACT[0]);
			TYPE_APPROVAL_FactorSave("OFST",flash.codeOFST[0]);
			TYPE_APPROVAL_FactorSave("MAXR",flash.codeMAXR[0]);
			TYPE_APPROVAL_FactorSave("AUXI",0);
		}
		else if(flash.tmsDataName[1][0]=='D'&&flash.tmsDataName[1][1]=='O'&&flash.tmsDataName[1][2]=='W') {
			//TYPE_APPROVAL_FactorSave("AUXI",0);
			TYPE_APPROVAL_FactorSave("MSIG",0);
			TYPE_APPROVAL_FactorSave("MTM1",flash.Uart[USART_1]->SensorPV[0]);
			TYPE_APPROVAL_FactorSave("ZERO",flash.codeZERO[1]);
			TYPE_APPROVAL_FactorSave("SPAN",flash.codeSPAN[1]);
			TYPE_APPROVAL_FactorSave("FACT",flash.codeFACT[1]);
			TYPE_APPROVAL_FactorSave("OFST",flash.codeOFST[1]);
			TYPE_APPROVAL_FactorSave("MAXR",flash.codeMAXR[1]);
			TYPE_APPROVAL_FactorSave("AUXI",0);
		}
		#endif
		//60분 데이터 누적변수 초기화
    }	
    
    pBuf[0]=0x0d;
    pBuf[1]=0;
    strcat((char *)gv.dataLogBuf,pBuf);
    int bufSize = strlen((char *)gv.dataLogBuf);
    for(int i=0; i<64;i++) {
        strName[i]=0;
    }
	if(time==1) {
        sprintf(strName,"1:AverageMinData%02d%02d.csv",currentDate.year,currentDate.month);
		SD_StructFileWrite(strName, bufSize);
		Dprintf("%dsec 20%02d/%02d/%02d %02d:%02d:%02d  %s Size = %d\n",(time * 60),
		currentDate.year,currentDate.month,currentDate.date,currentDate.hour,currentDate.minute,currentDate.sec,strName,bufSize);
		sprintf(pBuf,"20%02d/%02d/%02d %02d:%02d:%02d%s\n",
		currentDate.year,currentDate.month,currentDate.date,currentDate.hour,currentDate.minute,currentDate.sec, strName);
		DebugMessagePrintf(pBuf);        

	}
    else if(time==5) {
		//int fileStatus=FileStatusCheck(strName);
		//Dprintf("파일상태 = %d\n",fileStatus);		
        sprintf(strName,"1:AverageMinData%02d%02d.csv",flash.saveDate.year,flash.saveDate.month);
		SD_StructFileWrite(strName, bufSize);
		Dprintf("%dsec 20%02d/%02d/%02d %02d:%02d:%02d  %s Size = %d\n",(time * 60),
		flash.saveDate.year,flash.saveDate.month,flash.saveDate.date,flash.saveDate.hour,flash.saveDate.minute,0,strName,bufSize);
		sprintf(pBuf,"20%02d/%02d/%02d %02d:%02d%s\n",
		flash.saveDate.year,flash.saveDate.month,flash.saveDate.date,flash.saveDate.hour,flash.saveDate.minute,strName);
		DebugMessagePrintf(pBuf);        
    }
    else if(time==60) {
        sprintf(strName,"1:AverageHourData%02d%02d.csv",flash.save60Date.year,flash.save60Date.month);
        SD_StructFileWrite(strName, bufSize);
        Dprintf("HOUR 20%02d/%02d/%02d %02d:%02d:%02d   %s Size = %d\n",
        flash.save60Date.year,flash.save60Date.month,flash.save60Date.date,flash.save60Date.hour,flash.save60Date.minute,0,strName,bufSize);
		sprintf(pBuf,"20%02d/%02d/%02d %02d:%02d%s\n",
		flash.save60Date.year,flash.save60Date.month,flash.save60Date.date,flash.save60Date.hour,flash.save60Date.minute,strName);
		DebugMessagePrintf(pBuf);        
        
		flash.save60Date = currentDate;
    }
    
    if(gv.usbReadErrorFlag==1) {
        gv.usbReadErrorFlag=0;
        EventMessage("SDCARD 데이터 저장에러");
    } else {
        //Dprintf("[SaveTheMeasurement] = %s\n",gv.dataLogBuf);
    }
	return (0);
}
