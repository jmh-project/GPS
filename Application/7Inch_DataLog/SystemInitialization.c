#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"

extern void SmartLog_Uart7Init(void);
extern void EXTILine15_Config(void);
extern void EXTILine3_Config(void);
extern void ParamaterInit(void);

void ExoInit(void) {		
#ifdef EXO_ENABLE    
    flash.EXO.item=0;
    flash.EXO.itemAdd=0;		
    flash.EXO.DataListCount[0]=0;
    flash.EXO.DataListCount[1]=0;
    flash.EXO.DataListCount[2]=0;		
    
    flash.Uart[USART_1]->ItemCnt=0;
    flash.Uart[USART_2]->ItemCnt=0;
    flash.Uart[USART_3]->ItemCnt=0;
    
    flash.EXO.UartPort=EXO_DATA_REQUEST_PORT;
    for( int i = 0; i < 3; i++) {
        flash.EXO.Sensor_PH[i][0]=0;
        flash.EXO.Sensor_ORP[i][0]=0;
        flash.EXO.Sensor_ODO[i][0]=0;
        flash.EXO.Sensor_COND[i][0]=0;
        flash.EXO.Sensor_TUB[i][0]=0;
        flash.EXO.Sensor_SOND[i][0]=0;
        flash.EXO.Sensor_CHLIB[i][0]=0;
        flash.EXO.Sensor_DEPTH[i][0]=0;
        flash.EXO.Sensor_WIPER[i][0]=0;
        flash.EXO.Sensor_FDOM[i][0]=0;
        
        flash.EXO.Sensor_PH_Status[i][0]=0;
        flash.EXO.Sensor_PHMV_Status[i][0]=0;
        flash.EXO.Sensor_ORP_Status[i][0]=0;
        flash.EXO.Sensor_ODO_SAT_Status[i][0]=0;
        flash.EXO.Sensor_ODO_MGL_Status[i][0]=0;
        flash.EXO.Sensor_COND_SP_Status[i][0]=0;
        flash.EXO.Sensor_COND_SAL_Status[i][0]=0;
        flash.EXO.Sensor_COND_TEMP_Status[i][0]=0;
        flash.EXO.Sensor_TUB_FNU_Status[i][0]=0;
        flash.EXO.Sensor_TUB_NTU_Status[i][0]=0;
        flash.EXO.Sensor_CHLIB_UGL_Status[i][0]=0;
        flash.EXO.Sensor_CHLIB_RFU_Status[i][0]=0;
        flash.EXO.Sensor_BGA_PCRFU_Status[i][0]=0;
        flash.EXO.Sensor_BGA_PCUGL_Status[i][0]=0;
        flash.EXO.Sensor_BGA_PERFU_Status[i][0]=0;
        flash.EXO.Sensor_BGA_PEUGL_Status[i][0]=0;
        flash.EXO.Sensor_DEPTH_Status[i][0]=0;
        flash.EXO.Sensor_FDOM_RFU_Status[i][0]=0;
        flash.EXO.Sensor_FDOM_QSU_Status[i][0]=0;
        flash.EXO.Sensor_CABLEPOWER_Status[i][0]=0;
        flash.EXO.Sensor_Battery_Status[i][0]=0;			
    }

    flash.EXO.ECCalbration_Buffer[0]=0;
    flash.EXO.ECCalbration_Buffer[1]=0;
    flash.EXO.ECCalbration_Buffer[2]=0;

    flash.EXO.CalStart[0]=0;
    flash.EXO.CalStart[1]=0;
    flash.EXO.CalStart[2]=0;

    gv.wiperEnableTimePV=0;

    flash.EXO.WiperEnableTimeSV=600;//세척주기 초기값으로 10분 간격으로 세척을 실행한다.

    flash.EXO.Sensor_PORT[0]=0;
    flash.EXO.Sensor_PORT[1]=0;
    flash.EXO.Sensor_PORT[2]=0;

    gv.wiperEnableFlag=0;
    gv.wiperWaitCountSec=0;		
    
#endif
}
void ULIK_Init(void) {
    flash.Uart[USART_1]->ItemCnt=0;
	#ifdef SPECTRO_M_ENABLE    
	flash.ulik.id=0x50;//OWS-MW-L10
    flash.ulik.cleanTimeSv=600;//sec
    flash.Uart[USART_1]->SensorID = SENSOR_SPECTRO_M;
    strcpy(flash.Uart[USART_1]->SetupName[0],"TEMP 'C");
    strcpy(flash.Uart[USART_1]->SetupName[1],"COD mg/L");
    strcpy(flash.Uart[USART_1]->SetupName[2],"TUR NTU");
    strcpy(flash.Uart[USART_1]->SetupName[3],"NO3_N mg/L");
    strcpy(flash.Uart[USART_1]->SetupName[4],"TOC mg/L");
    strcpy(flash.Uart[USART_1]->SetupName[5],"NONE");
    flash.Uart[USART_1]->Rate = 9600;
    flash.Uart[USART_1]->SensorStatus = 1;
    flash.Uart[USART_1]->ItemCnt += 5;
    flash.Uart[USART_1]->UartOr485Select=1;
    #endif
    #ifdef CTD_ENABLE
    strcpy(flash.Uart[USART_1]->SetupName[5],"COND uS/cm");
    strcpy(flash.Uart[USART_1]->SetupName[6],"DEPTH m");
    strcpy(flash.Uart[USART_1]->SetupName[7],"Pressure");
	flash.Uart[USART_1]->ItemCnt += 3;
    #endif
    #ifdef BATTERY_ENABLE    
    strcpy(flash.Uart[USART_1]->SetupName[8],"사용전압 V");
    strcpy(flash.Uart[USART_1]->SetupName[9],"사용전류 A");
    strcpy(flash.Uart[USART_1]->SetupName[10],"배터리잔량 %");
    strcpy(flash.Uart[USART_1]->SetupName[11],"배터리전압 V");
	flash.Uart[USART_1]->ItemCnt += 4;
    #endif
    #ifdef SPECTRO_D_ENABLE
    flash.ulik.id=0x60;//OWS-DW-L10
    flash.ulik.cleanTimeSv=600;//sec
    flash.Uart[USART_1]->SensorID = SENSOR_SPECTRO_D;
    strcpy(flash.Uart[USART_1]->SetupName[0],"COD mg/L");
    strcpy(flash.Uart[USART_1]->SetupName[1],"TUR NTU");                         
    strcpy(flash.Uart[USART_1]->SetupName[2],"TOC mg/L");
    strcpy(flash.Uart[USART_1]->SetupName[3],"TEMP 'C");
    flash.Uart[USART_1]->Rate = 9600;
    flash.Uart[USART_1]->SensorStatus = 1;
    flash.Uart[USART_1]->ItemCnt = 4;
    flash.Uart[USART_1]->UartOr485Select=1;
    #endif
	#ifdef ULIK_CTD_ENABLE
		flash.Uart[USART_2]->Rate = 115200;
		flash.Uart[USART_2]->UartOr485Select=1;
	#endif
}
void TcpIpSet(void) {
    flash.serverMac[0] = 0x00;            
    flash.serverMac[1] = 0x80;
    flash.serverMac[2] = 0xDC;
    flash.serverMac[3] = 0x00;
    flash.serverMac[4] = 0xAB;        
    flash.serverMac[5] = 0x10;    

    flash.clientMac[0] = 0x00;            
    flash.clientMac[1] = 0x80;
    flash.clientMac[2] = 0xDC;
    flash.clientMac[3] = 0x00;
    flash.clientMac[4] = 0xAB;   
    flash.clientMac[5] = 0x10;        

    for(int i=0; i<5 ;i++)
    {
        flash.serverIp[i]= 0;
        flash.serverGate[i]= 0;
        flash.serverMask[i]= 0;

        flash.clientIp[i]= 0;
        flash.clientGate[i]= 0;
        flash.clientMask[i]= 0;
    }
    flash.serverIp[0]=192;
    flash.serverIp[1]=168;
    flash.serverIp[2]=0;
    flash.serverIp[3]=123;

    flash.serverMask[0]=255;
    flash.serverMask[1]=255;
    flash.serverMask[2]=255;
    flash.serverMask[3]=0;

    flash.serverGate[0]=192;
    flash.serverGate[1]=168;
    flash.serverGate[2]=0;
    flash.serverGate[3]=1;
        
    flash.serverPort[0] = 5000;
    flash.serverPort[1] = 5001;
    flash.clientPort = 5000;    

    flash.Uart[USART_5]->SensorID = SENSOR_NONE;
}
void AD420ValueInit(void) {
    int i=0;
    for(i=0;i<OUTPUT_MAX_CHANNEL;i++)
    {
        flash.Output[i]->CalLowLimit = 0.0F;
        flash.Output[i]->CalHighLimit = 0.0F;
        flash.Output[i]->PV = 0.0F;
    }
}
void UartValueInit(void) {
    int i=0,k=0;
    for(i=0;i<USART_MAX;i++)
    {
        flash.Uart[i]->SensorID = 0;    
        flash.Uart[i]->Rate = 9600;
        flash.Uart[i]->SensorStatus = 0;
        flash.Uart[i]->ItemCnt = 0;
    }
    for(k=0;k<USART_MAX;k++)//UART PORT COUNT
    {
        for(i=0;i<USART_SENSOR_MAX;i++)//UART SENSOR COUNT
        {
            strcpy(flash.Uart[k]->SetupName[i],"NONE");
            flash.Uart[k]->SensorPV[i]=0;
        }
    }
	#if defined(TYPE_APPROVAL_ENABLE)
    for(i=0;i<10;i++) {
		strcpy(flash.tmsDataName[i],"NONE");
	}
	#endif
}
void InputOutputmAInit(void) {
    int i=0;
    for(i=0;i<INPUT_MAX_CHANNEL;i++)
    {
        flash.Input[i]->CalLowmV = 357.0F;
        flash.Input[i]->CalHighmV = 1792.0F;
        flash.Input[i]->CalLowLimit = 0.0F;
        flash.Input[i]->CalHighLimit = 100.0F;
        flash.Input[i]->SensorStatus = 0;
        flash.Input[i]->PV = 0.0F;
        strcpy(flash.Input[i]->Name,"NONE");
    }
    for(i=0;i<OUTPUT_MAX_CHANNEL;i++) 
    {
        strcpy(flash.Output[i]->Name,"NONE");
    }    
}
void FloatPointInit(void) {
    for(int i=0; i<FLOAT_POINT_MAX; i++) {
        flash.floatPoint[i]=2;
    }
}

void OptionInit(void) {
    flash.saveMinuteSv=5;//데이터 저장주기
}
void ODOCT_Init(void) {
    #if defined(YSI_ODOCT_ENABLE) || defined(YSI_ODD_ENABLE)
        flash.odoct.barometerPv=760.0;
        flash.odoct.odoCalPv=8.30;
        flash.odoct.salPv=0;
    #endif
}
void CTD_Init(void) {
    #if defined(CTD_ENABLE)
    flash.ctd.id=0x01;
    #endif
}
void YSI_Init(void) {
	#if defined (YSI_ENABLE)
	flash.ysiSensorNameChange=ON;//SS mg/L 출력하게 한다.
	gv.jsonTcpDebugFlag=0;
	gv.debugFlag=0;
	#endif
}
void ModemPowerInit(void) {
	#if defined(MODEM_POWER_CONTROL)
	flash.wakeUpTimeModem=60;//초기설정값 1시간			
	#endif
}
void EXO_Set(void) {
    #if defined(EXO_ENABLE)
	gv.findDelayTimeCnt=0;

    #if defined(DAECHEONGHO_SUGILGWANLISO)
    flash.Uart[USART_1]->SensorID = SENSOR_EXO;
    flash.Uart[USART_1]->Rate = 115200;
    flash.Uart[USART_1]->SensorStatus = 1;
    flash.Uart[USART_1]->ItemCnt = 12;
    flash.Uart[USART_1]->UartOr485Select=1;    

    flash.Uart[USART_2]->SensorID = SENSOR_EXO;
    flash.Uart[USART_2]->Rate = 115200;
    flash.Uart[USART_2]->SensorStatus = 1;
    flash.Uart[USART_2]->ItemCnt = 6;
    flash.Uart[USART_2]->UartOr485Select=1;    
    
    flash.Uart[USART_4]->SensorID = SENSOR_MODBUS;
    flash.Uart[USART_4]->Rate = 9600;
    flash.Uart[USART_4]->UartOr485Select=1;    
    #endif
    #endif
}
void TYPE_APPROVAL_Init(void) {
	#if defined(TYPE_APPROVAL_ENABLE)
	flash.codeMSIG[0]=0;		//검출신호
	flash.codeMTM1[0]=0.0;	//수온
//	flash.codeMTM2=25.0;	//측정조온도
//	flash.codeMSAM=1.0;		//희석비율
	flash.codeZERO[0]=0.0;		//영점교정액농도
	flash.codeSPAN[0]=0.0;		//스팬교정액농도
//	flash.codeSLOP=0;		//교정기울기
//	flash.codeICPT=0;		//교정절편
	flash.codeFACT[0]=1.0;		//보정기울기
	flash.codeOFST[0]=0.0;		//보정절편
	flash.codeMAXR[0]=14.0;	//측정범위
	flash.codeMSIG[1]=0;		//검출신호
	flash.codeMTM1[1]=0.0;	//수온
//	flash.codeMTM2=25.0;	//측정조온도
//	flash.codeMSAM=1.0;		//희석비율
	flash.codeZERO[1]=0.0;		//영점교정액농도
	flash.codeSPAN[1]=0.0;		//스팬교정액농도
//	flash.codeSLOP=0;		//교정기울기
//	flash.codeICPT=0;		//교정절편
	flash.codeFACT[1]=1.0;		//보정기울기
	flash.codeOFST[1]=0.0;		//보정절편
	flash.codeMAXR[1]=20.0;	//측정범위
	for(int i=0; i<10; i++) { //비번초기화
		flash.password[i]='1';
	}
	flash.password[10]=0;
	flash.password[11]=0;
	flash.saveMinuteSv=5;//형식승인 버젼은 5분데이터만 저장한다.
	
	strcpy(flash.tmsDataName[0],"DAT00");
	strcpy(flash.tmsDataName[1],"DAT01");
	strcpy(flash.tmsDataName[2],"DAT02");
	strcpy(flash.tmsDataName[3],"DAT03");
	strcpy(flash.tmsDataName[4],"DAT04");
	strcpy(flash.tmsDataName[5],"DAT05");
	strcpy(flash.tmsDataName[6],"DAT06");
	strcpy(flash.tmsDataName[7],"DAT07");
	strcpy(flash.tmsDataName[8],"DAT08");
	strcpy(flash.tmsDataName[9],"DAT09");
	
	#endif
}
void DataLog_VarInit(void) {
    SmartLog_Uart7Init();   //IOBOARD 와 통신용
    IoportInit();
    EXTILine15_Config();	//INPUT 1	//PB15 	유량계
    EXTILine3_Config();		//INPUT 2	//PD3 	전력량계 펄스값	
    //EXTILine7_Config();	//PB7 	    강우량 펄스값		

    strcpy(gv.timeBuf,"2000/00/00 00:00:00");//YSI6600 측정시간
	gv.StartVerify=1234;
	gv.EndVerify=5678;	

    DiskToData(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");//데이터복원 점검       
    if(flash.StartVerify==1) { 
        Remove("DongMoonFlashData.txt");
		
        DiskToData(&flash, sizeof(__SmartLogdata),"DongMoonFlashData2.txt");
        
		EventMessage("데이터복원기능실행");
        
		flash.StartVerify=1234;
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
    }
    
	if(flash.StartVerify!=1234 || flash.EndVerify!=5678) {
		
        EventMessage("자료구조 초기화 ( flash )");
        
        flash.StartVerify=1234;
		flash.EndVerify=5678;
		flash.powerControl=DISABLE;     //SLEEP MODE DISABLE
        flash.sleepModeFlag=DISABLE;    //절전모드 적용여부
        flash.autoRebootHourSv=25;      //자동리부팅취소
        flash.onlyOneSaveFlag=222;      //5분전데이터생성용
        flash.systemColorNo=7;          //테마 컬러
        
        AD420ValueInit();
        UartValueInit();
        InputOutputmAInit();
        
        ExoInit();
        TcpIpSet();
        FloatPointInit();
        OptionInit();
        ODOCT_Init();
        CTD_Init();
		ModemPowerInit();
		TYPE_APPROVAL_Init();			//형식승인관련초기화
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
	}
	/*
	
	프로그램 실행시 별도로 초기화가 필요한 부분은 여기서 초기화 한다.
	
	*/
    gv.touchReceiveErrorCount = 0;
    gv.nFlag_Sensor_DataRead[USART_1]=SCREEN_MODE;
	gv.nFlag_Sensor_DataRead[USART_2]=SCREEN_MODE;
	gv.nFlag_Sensor_DataRead[USART_3]=SCREEN_MODE;
	gv.nFlag_Sensor_DataRead[USART_4]=SCREEN_MODE;
	gv.nFlag_Sensor_DataRead[USART_5]=SCREEN_MODE;
	DS1302TimeRead();   
	#if defined(RTUV3_ENABLE)
	strcpy(gv.titleBuf,"< RTU V3.0 >"); 	//매우중요               
	#endif
	#if defined(YSI_ODD_ENABLE)
	strcpy(gv.titleBuf,"< RTU V4.0 >"); 	//매우중요               
	#endif
	#if defined(YSI_ODOCT_ENABLE)
	strcpy(gv.titleBuf,"< RTU V5.0 >");		//매우중요              
	#endif
	#if defined(SPECTRO_D_ENABLE)
	strcpy(gv.titleBuf,"< RTU V3.0 >");
	#endif
	
	gv.autoScreen = OFF;        //자동으로 화면을 전환한다.
    gv.usbSaveWaitFlag=0;
		
	ParamaterInit();
	
    flash.StartVerify=1;
    DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
    /**
    *   flash 파일이 정상적이지 않은경우 시스템이 리부팅된다. 
    *   flash.StrartVerify==1 이면 복원기능이 실행된다.
    */

}
void SystemColorSet(void) {//color 변경
	switch(flash.systemColorNo) {
        case 0:
            flash.systemColor = clDarkRed;
            flash.systemFontColor = clWhite;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = clDarkRed;//RGB565CONVERT(191, 191, 191);
            break;
        case 1:
            flash.systemColor = clDarkGreen;
            flash.systemFontColor = clWhite;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = clDarkGreen;//RGB565CONVERT(191, 191, 191);
            break;
        case 2:
            flash.systemColor = clDarkYellow;
            flash.systemFontColor = clWhite;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = clDarkYellow;//RGB565CONVERT(191, 191, 191);
            break;
        case 3:
            flash.systemColor = clDarkBlue;
            flash.systemFontColor = clWhite;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = clDarkBlue;//RGB565CONVERT(191, 191, 191);
            break;
        case 4:
            flash.systemColor = GRAY5;
            flash.systemFontColor = clWhite;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = GRAY5;//RGB565CONVERT(191, 191, 191);
            break;
        case 5:
            flash.systemColor = clDarkCyan;
            flash.systemFontColor = clWhite;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = clDarkCyan;//RGB565CONVERT(191, 191, 191);
            break;
        case 6:
            flash.systemColor = RGB565CONVERT(191, 191, 191);
            flash.systemFontColor = clBlack;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = RGB565CONVERT(191, 191, 191);
            break;
        case 7:
            flash.systemColor = RGB565CONVERT(2, (52+0), (77+10));
            flash.systemFontColor = clWhite;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = clBlack;//RGB565CONVERT(191, 191, 191);
            break;
        default :
            flash.systemColor = RGB565CONVERT(2, (52+0), (77+10));
            flash.systemFontColor = clWhite;
            flash.systemGroupPanelColor = clBlack;
            flash.systemSetupPanelFontColor = clWhite;
            flash.systemBackGroundColor = clBlack;//RGB565CONVERT(191, 191, 191);
            break;
    }
}
void ParamaterInit(void) {
    SystemColorSet();
    #ifdef EXO_ENABLE
    EXO_Set();    
    #endif
    #ifdef SPECTRO_D_ENABLE
    ULIK_Init();
    #endif
    #ifdef SPECTRO_M_ENABLE
    ULIK_Init();
    #endif
    #ifdef CTD_ENABLE
    CTD_Init();
    #endif
    #if defined(YSI_ODOCT_ENABLE) || defined(YSI_ODD_ENABLE)
        gv.modbusId=0x01;		//시작모드버스아이디
        gv.request_DataFlag=ON;	//데이터요청
    #endif
	#if defined (SAMAN_ENABLE)
		strcpy(flash.Input[0]->Name,"순시유량 m3/h");//4~20mA
		strcpy(flash.Input[1]->Name,"수위 cm");	//4~20mA
		strcpy(flash.Input[2]->Name,"적산유량 m3");//puls 공식 200mS
		flash.Input[0]->SensorStatus = 1;
		flash.Input[1]->SensorStatus = 1;
		flash.Input[2]->SensorStatus = 1;
	#endif
	#if defined (PONSEL_EC)
		flash.Uart[USART_1]->SensorID = SENSOR_PONSEL_EC;
		strcpy(flash.Uart[USART_1]->SetupName[0],"TEMP 'C");
		strcpy(flash.Uart[USART_1]->SetupName[1],"COND mS/cm");
		strcpy(flash.Uart[USART_1]->SetupName[2],"Salinity ppt");
		flash.Uart[USART_1]->Rate = 9600;
		flash.Uart[USART_1]->SensorStatus = 1;
		flash.Uart[USART_1]->ItemCnt = 3;
		flash.Uart[USART_1]->UartOr485Select=1;
		//교정관련 변수초기화
		flash.ponsel_ec.calStartFlag=0;
        flash.ponsel_ec.calReceiveFlag=0;
        flash.ponsel_ec.calStep=0;
		
	#endif
	#if defined (YSI_ENABLE)
		YSI_Init();
	#endif
	
	Dprintf("ParamaterInit :: 실행\n");
}

void Bubble_sort(float list[], int n){
	int i, j;
	float	temp;

	for(i=n-1; i>0; i--){
			// 0 ~ (i-1)까지 반복
			for(j=0; j<i; j++){
				// j번째와 j+1번째의 요소가 크기 순이 아니면 교환
				if(list[j]<list[j+1]){
				temp = list[j];
				list[j] = list[j+1];
				list[j+1] = temp;
			}
		}
	}
}



