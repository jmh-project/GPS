

/******************************************************************************
*    	File name	: KeyPad.c 
*		Description : TEECO Project User Program File
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: Naver TEECO Cafe
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/

#include "TEECO_System.h"
#include "DataLog.h"
#ifdef EXO_ENABLE
/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/
/******************************************************************************
							Function Prototype
******************************************************************************/
//void SmartLogEXOItemKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);

volatile U32 	EXO_HiperID;
volatile U32	EXO_CalID[10];
volatile U32	EXO_TimerID;
volatile U32 	EXO_CalITem_ID;
volatile U32  EXO_CalStartID=0, EXO_CalApplyID=0, EXO_CalPoint1ID=0, EXO_CalPoint2ID=0, EXO_CalPoint3ID=0, EXO_CalMessageID=0,EXO_CalExit=0;
volatile U32	EXO_CalComPortID[5];
volatile int	WiperChange;
volatile U32	EXO_FactorID;

//#define ODO_SAT_CAL

#if ODO_SAT_CAL
volatile int gDO1ChannelChange = 0,gDO2ChannelChange = 0,gDO3ChannelChange = 0;
#endif
int	 SmartLogNysiScreenEditButtonEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {	
	Windows 	*win = GetWindow(parent, handle, "SmartLogNysiScreenClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "SmartLogNysiScreenClickEvent");
	if(parent_win == NULL)
	{
		Dprintf("SmartLogNysiScreenClickEvent:: Windows Error\n");
	}
	return (1);
}   
int gWaitMessageFlag=OFF;
int	 SmartLogNysiScreenTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	char		cbuf[20];

	//Dprintf("SmartLogStartTimerEvent::TimerEventid = %08x,%08x,%08x,%08x\n", parent,handle,command,param);
	Windows 	*win  = GetWindow(parent, handle, "SmartLogNysiScreenTimerEvent");				//windows 관련 정보를 얻는다.
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	
	if(win==NULL) 											// 윈도우가 생성이 안된경우 
	{
		Dprintf("SmartLogNysiScreenTimerEvent::Windows Error\n");
		return(0);
	}
	if(Gr==NULL) 												// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("SmartLogNysiScreenTimerEvent:: Graphics Error\n");
		return(0);
	}	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
    
	//Dprintf("교정채널 = %d\n",flash.EXO.UartPort);
	
    //DS1302TimeRead();
    
	gv.wiperEnableTimePV=0;//와이터가 보정중에는 자동으로 동작하지 않도록 타이머값을 초기화한다.
	//USART 포트 설정이 되어 있는 경우 
	if(flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)
	{
		//측정값
		sprintf(cbuf,"%.3f",flash.Uart[flash.EXO.UartPort]->SensorPV[flash.EXO.CalItem]);
		SetWidgetText(EXO_CalID[4], cbuf);		
		//Dprintf("교정전도도 = %.3f\n",flash.Uart[flash.EXO.UartPort]->SensorPV[flash.EXO.CalItem]);		
		//PANDING
		if(flash.EXO.CalHexaItemCode[flash.EXO.UartPort] ==0x18) {
            float fdata = flash.EXO.SV[flash.EXO.UartPort] / 1000;
			sprintf(cbuf,"%.3f",fdata);
		} else {
			sprintf(cbuf,"%.3f",flash.EXO.SV[flash.EXO.UartPort]);
		}
		SetWidgetText(EXO_CalID[5], cbuf);
		//TEMPERATURE
		sprintf(cbuf,"%.3f",flash.EXO.temp[flash.EXO.UartPort]);
		SetWidgetText(EXO_CalID[6], cbuf);
		//CALPOINT
		//sprintf(cbuf,"%10d",flash.EXO.CalPoint);
		//SetWidgetText(EXO_CalID[7], cbuf);
		/*	
		if(WiperChange==0)
		{
			sprintf(cbuf,"%d",flash.EXO.WiperEnableTimeSV[flash.EXO.UartPort]);
			SetWidgetText(EXO_HiperID, cbuf);
		}
		else
		{
			GetWidgetText(EXO_HiperID, cbuf);
			flash.EXO.WiperEnableTimeSV[flash.EXO.UartPort] = atoi(cbuf);
		}
		*/
		if(flash.EXO.CalStart[flash.EXO.UartPort]==4)//종료버튼의 이미지를 전환한다.
		{
			if(gWaitMessageFlag == ON) {
				gWaitMessageFlag=OFF;
				SetWidgetText(EXO_CalID[7],"교정이 완료 되었습니다.");
			}
			flash.EXO.CalStartStep[flash.EXO.UartPort]=0;
			flash.EXO.CalCommand=0;
			SetWidgetColor(EXO_CalExit,clBlue,clWhite);	// SetWidgetColor(EXO_CalExit,clBlue, clRed);	
			SetWidgetText(EXO_CalExit,"EXIT");			
		}
	}
	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				CALIBRATION ID SELECT
//				1. 보정 아이템이 바뀔때마다 보정변수를 초기화한다..
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int	 SmartLogNysiScreenClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle, "SmartLogNysiScreenClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "SmartLogNysiScreenClickEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	//char buf[32];
	//unsigned int			i=0;
	char		data[128];
	if(parent_win == NULL)
	{
		Dprintf("SmartLogNysiScreenClickEvent:: Windows Error\n");
	}
	if(Gr==NULL)
	{
		Dprintf("SmartLogNysiScreenClickEvent::GetGraphics Error\n");
		return(0);
	}	
	///////////////////////////////////////////////////////////////
	//		 ITEM CHANGE
	///////////////////////////////////////////////////////////////
	if(win->ID==1 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)
	{
		SetWidgetText(EXO_CalID[7],"START 버튼을 누르고 교정을 시작합니다.");
		
		flash.EXO.SV[0]=0;//설정값이 초기화된다.
		flash.EXO.SV[1]=0;//설정값이 초기화된다.
		flash.EXO.SV[2]=0;//설정값이 초기화된다.
		
		flash.EXO.CalPoint=0;//보정포인트 클리어
        
		flash.EXO.CalItem++;//보정 아이템을 전환한다.
		if(flash.EXO.CalItem >= flash.Uart[flash.EXO.UartPort]->ItemCnt) //설정되어 있는 아이템 수를 넘어가면 처음으로 넘어간다.
		{
			flash.EXO.CalItem=0;
		}
        /**
        *   측정 항목중에 ODO mg/L 가 있는 경우
        *   ODO Sat 로 보정을 진행하게 한다.
        */
#if ODO_SAT_CAL
        if(0==strcmp(flash.Uart[flash.EXO.UartPort]->SetupName[flash.EXO.CalItem] ,"ODO mg/L")) 
        {
            Dprintf("교정용으로 항목을 변경한다. ODO mg/L to ODO Sat UART = %d PortNo = %d\n",flash.EXO.UartPort,flash.EXO.Sensor_ODO[flash.EXO.UartPort][0]);
            if(flash.EXO.UartPort==USART_1) {
				gDO1ChannelChange = ON;
			}
			if(flash.EXO.UartPort==USART_2) {
				gDO2ChannelChange = ON;
			}
			if(flash.EXO.UartPort==USART_3) {
				gDO3ChannelChange = ON;
			}
            strcpy(flash.Uart[flash.EXO.UartPort]->SetupName[flash.EXO.CalItem], "ODO Sat");
			flash.EXO.Sensor_ODO_SAT_Status[flash.EXO.UartPort][0] = ON;
			flash.EXO.Sensor_ODO_MGL_Status[flash.EXO.UartPort][0] = OFF;
        }
#endif		
		SetWidgetText(EXO_CalITem_ID,flash.Uart[flash.EXO.UartPort]->SetupName[flash.EXO.CalItem]);//설정된 아이템 명 출력
		if(!strcmp(flash.Uart[flash.EXO.UartPort]->SetupName[flash.EXO.CalItem],"pH")) {
			flash.EXO.SV[0]=7.0;
			flash.EXO.SV[1]=4.0;
			SetWidgetText(EXO_CalID[0],"7.000");
			SetWidgetText(EXO_CalID[1],"4.000");
		}
		else {
			SetWidgetText(EXO_CalID[0],"0.000");
			SetWidgetText(EXO_CalID[1],"0.000");
		}
		Dprintf("교정항목 = %s\n",flash.Uart[flash.EXO.UartPort]->SetupName[flash.EXO.CalItem]);
	}
	///////////////////////////////////////////////////////////////
	//		 FACTORY
	///////////////////////////////////////////////////////////////
	else if(win->ID==2 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT) {
        Dprintf("FACTORY\n");
		GetWidgetText(EXO_CalITem_ID,data);
		SetWidgetColor(EXO_FactorID,clBlue,clRed);	
		flash.EXO.SV[flash.EXO.UartPort]=0;
		if(0==strcmp(data,"SPCOND mS/cm"))
		{
			EventMessage("[SYSTEM] SPCOND mS/cm 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x18;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_COND[flash.EXO.UartPort][0]-1;
            Dprintf("FACTORY :: SPCOND mS/cm\n");
            Dprintf("FACTORY :: PORT = %d\n",flash.EXO.UartPort);
            Dprintf("FACTORY :: ADDRESS = %d\n",flash.EXO.EXO_CalAddress);
			Dprintf("SPCOND_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
            //SPCOND_CalAddress = 0 49 43 00 05
		}
		else if(0==strcmp(data,"SAL ppt"))
		{
			EventMessage("[SYSTEM] SAL ppt 초기화실행");	
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x13;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_COND[flash.EXO.UartPort][0]-1;
            Dprintf("FACTORY :: SPCOND mS/cm\n");
            Dprintf("FACTORY :: PORT = %d\n",flash.EXO.UartPort);
            Dprintf("FACTORY :: ADDRESS = %d\n",flash.EXO.EXO_CalAddress);
			Dprintf("SPCOND_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
            //SPCOND_CalAddress = 0 49 43 00 05
		}
		else if(0==strcmp(data,"DEPTH m")) 
		{
			EventMessage("[SYSTEM] DEPTH m 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x24;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_DEPTH[flash.EXO.UartPort][0]-1;
		}
		else if(0==strcmp(data,"ODO Sat")) 
		{
			EventMessage("[SYSTEM] ODO Sat 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x09;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_ODO[flash.EXO.UartPort][0]-1;
            Dprintf("FACTORY :: ODO Sat\n");
            Dprintf("FACTORY :: PORT = %d\n",flash.EXO.UartPort);
            Dprintf("FACTORY :: ADDRESS = %d\n",flash.EXO.EXO_CalAddress);
		}
		else if(0==strcmp(data,"ODO mg/L")) 
		{
			EventMessage("[SYSTEM] ODO mg/L 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x0A;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_ODO[flash.EXO.UartPort][0]-1;
            Dprintf("FACTORY :: ODO mg/L\n");
            Dprintf("FACTORY :: PORT = %d\n",flash.EXO.UartPort);
            Dprintf("FACTORY :: ADDRESS = %d\n",flash.EXO.EXO_CalAddress);
			#if defined(TYPE_APPROVAL_ENABLE)
			flash.codeZERO[1]=0;
			flash.codeSPAN[1]=0;
			Dprintf("DO 교정값 초기화\n");
			#endif
		}
		else if(0==strcmp(data,"FDOM RFU")) 
		{
			EventMessage("[SYSTEM] FDOM RFU 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x2E;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_FDOM[flash.EXO.UartPort][0]-1;
			Dprintf("FDOM RFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"FDOM QSU")) 
		{
			EventMessage("[SYSTEM] FDOM QSU 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x37;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_FDOM[flash.EXO.UartPort][0]-1;
			Dprintf("FDOM QFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"pH"))
		{
			EventMessage("[SYSTEM] pH 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort]=0x03;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_PH[flash.EXO.UartPort][0]-1;
			
			Dprintf("pH_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
			#if defined(TYPE_APPROVAL_ENABLE)
			flash.codeZERO[0]=0;
			flash.codeSPAN[0]=0;
			Dprintf("pH 교정값 초기화\n");
			#endif
		}			
		else if(0==strcmp(data,"ORP mV"))
		{
			EventMessage("[SYSTEM] ORP mV 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort]=0x06;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_PH[flash.EXO.UartPort][0]-1;
			
			Dprintf("pH_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}			
		else if(0==strcmp(data,"CHLOROPHYLL ug/L"))
		{
			EventMessage("[SYSTEM] CHLOROPHYLL ug/L 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x1A;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_CHLIB[flash.EXO.UartPort][0]-1;
			
			Dprintf("CHLIB_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"BGAPC ug/L"))
		{
			EventMessage("[SYSTEM] BGAPC ug/L 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x3A;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_CHLIB[flash.EXO.UartPort][0]-1;
			
			Dprintf("CHLIB_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		//else if(0==strcmp(data,"TUBIDITY FNU"))
		else if(0==strcmp(data,"TURBIDITY NTU") || 0==strcmp(data,"TURBIDITY FNU"))
		{
			EventMessage("[SYSTEM] TURBIDITY NTU 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x15;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_TUB[flash.EXO.UartPort][0]-1;
			
			Dprintf("CHLIB_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"NH4+"))
		{
			EventMessage("[SYSTEM] NH4+ 초기화실행");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x44;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_Ammoninum[flash.EXO.UartPort][0]-1;
			
			Dprintf("Ammoninum CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		//초기화 명령을 송신한다.
		EXO_Calibration_Command(flash.EXO.UartPort, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[flash.EXO.UartPort], 0x06, 0x00, 0x00, NULL);
//7E 02 60 82 21 8B 32 FF B6 C6 A9 30 00 09 06 00 00 DA 28 
//7E 02 60 FF B6 C6 A9 82 21 8B 32 00 00 09 06 00 00 32 
		SetWidgetColor(EXO_FactorID,clBlue,clWhite);	
	}
	///////////////////////////////////////////////////////////////////////////////
	//		보정시작 버튼
	//		1. 100 이라는 숫자는 통신 설정이 되었는가 를 의미한다.
	///////////////////////////////////////////////////////////////////////////////
	else if(win->ID==20 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT) {
		//SetWidgetText(EXO_CalID[7],"1P APPLY 버튼은 교정완료 입니다.");
		SetWidgetText(EXO_CalID[7],"PANDING 데이터 확인 후 1P APPLY 터치~");
		
		GetWidgetText(EXO_CalITem_ID,data);
		Dprintf("START BUTTON CalItem = %s\n",data);
		
		SetWidgetText(EXO_CalExit,"COMPLETE");
		
		{//센서가 연결되어 있는 포트의 넘버를 알아온다.
		if(0==strcmp(data,"SPCOND mS/cm"))
		{
			EventMessage("[SYSTEM] SPCOND mS/cm 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x18;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_COND[flash.EXO.UartPort][0]-1;
			Dprintf("SPCOND RFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
            //b2 21 99 cb
		}
		else if(0==strcmp(data,"SAL ppt"))
		{
			EventMessage("[SYSTEM] SAL ppt 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x13;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_COND[flash.EXO.UartPort][0]-1;
			Dprintf("SPCOND RFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
            //b2 21 99 cb
		}
		else if(0==strcmp(data,"DEPTH m")) 
		{
			EventMessage("[SYSTEM] DEPTH m 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x24;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_DEPTH[flash.EXO.UartPort][0]-1;
			Dprintf("DEPTH RFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"ODO Sat")) 
		{
			EventMessage("[SYSTEM] ODO Sat 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x09;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_ODO[flash.EXO.UartPort][0]-1;
			Dprintf("ODD Sat Hexa Code = 0x%02x\n",flash.EXO.CalHexaItemCode[flash.EXO.UartPort]);
			Dprintf("ODO Sat RFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"ODO mg/L"))
		{
			EventMessage("[SYSTEM] ODO mg/L 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x0A;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_ODO[flash.EXO.UartPort][0]-1;
			Dprintf("ODOmg/L RFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"FDOM RFU")) 
		{
			EventMessage("[SYSTEM] FDOM RFU 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x2E;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_FDOM[flash.EXO.UartPort][0]-1;
			Dprintf("FDOM RFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"FDOM QSU")) 
		{
			EventMessage("[SYSTEM] FDOM QSU 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x37;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_FDOM[flash.EXO.UartPort][0]-1;
			Dprintf("FDOM RFU_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"pH"))
		{
			EventMessage("[SYSTEM] pH 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x03;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_PH[flash.EXO.UartPort][0]-1;
			
			Dprintf("pH_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}			
        else if(0==strcmp(data,"ORP mV"))
        {
			EventMessage("[SYSTEM] ORP mV 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x06;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_PH[flash.EXO.UartPort][0]-1;
			
			Dprintf("pH_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
        }
		else if(0==strcmp(data,"CHLOROPHYLL ug/L"))
		{
			EventMessage("[SYSTEM] CHLOROPHYLL ug/L 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x1A;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_CHLIB[flash.EXO.UartPort][0]-1;
			
			Dprintf("CHLIB_CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		else if(0==strcmp(data,"BGAPC ug/L"))
		{
			EventMessage("[SYSTEM] BGAPC ug/L 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x3A;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_CHLIB[flash.EXO.UartPort][0]-1;
			
			Dprintf("BGAPC CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		//else if(0==strcmp(data,"TUBIDITY FNU"))
        else if(0==strcmp(data,"TURBIDITY NTU"))
		{
			EventMessage("[SYSTEM] TURBIDITY NTU 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x15;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_TUB[flash.EXO.UartPort][0]-1;
			
			Dprintf("TURBIDITY FNU CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
        else if(0==strcmp(data,"NH4+"))
		{
			EventMessage("[SYSTEM] NH4+ 교정시작");
			flash.EXO.CalHexaItemCode[flash.EXO.UartPort] =0x44;//보정ID
			flash.EXO.EXO_CalAddress  =flash.EXO.Sensor_Ammoninum[flash.EXO.UartPort][0]-1;
			
			Dprintf("NH4+ CalAddress = %d %02x %02x %02x %02x \n"
			,flash.EXO.EXO_CalAddress
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][0]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][1]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][2]
			,flash.EXO.SensorAddress[flash.EXO.UartPort][flash.EXO.EXO_CalAddress][3]
			);
		}
		}
        //////////////////////////////////////////////
		//  설정된 보정버퍼값을 읽어들인다.
		//////////////////////////////////////////////
		
		GetWidgetText(EXO_CalID[0],data);
		flash.EXO.ECCalbration_Buffer[0]=atof(data);
		if(flash.EXO.CalHexaItemCode[flash.EXO.UartPort] ==0x18)//전도도 보정시  mS/cm 단위를 환산해 준다.
		{
			flash.EXO.ECCalbration_Buffer[0] *= 1000.0f;
			Dprintf("SPCOND BUFFER = %f\n",flash.EXO.ECCalbration_Buffer[0]);
		}
		GetWidgetText(EXO_CalID[1],data);
		flash.EXO.ECCalbration_Buffer[1]=atof(data);
		GetWidgetText(EXO_CalID[2],data);
		flash.EXO.ECCalbration_Buffer[2]=atof(data);
		flash.EXO.CalPoint=0;									//전도도는 1point 고정이다.
		flash.EXO.CalStart[flash.EXO.UartPort]=ON;              //보정을 시작하는 명령이다. EXO에 데이터를 요청하는 명령이 달라진다.
		flash.EXO.CalStartStep[flash.EXO.UartPort]=1;			//보정스텝
		flash.EXO.CalCommand=0;									//명령 리스트 카운트값 
		SetWidgetColor(EXO_CalStartID,clBlue, clRed);			//버튼이미지 효과를 만든다.	
		SetWidgetColor(EXO_CalPoint1ID,clBlue, clWhite);	
		SetWidgetColor(EXO_CalPoint2ID,clBlue, clWhite);	
		SetWidgetColor(EXO_CalPoint3ID,clBlue, clWhite);	
				
		Dprintf("버퍼1 = %.2f 버퍼2 = %.2f 버퍼3 = %.2f\n",flash.EXO.ECCalbration_Buffer[0],flash.EXO.ECCalbration_Buffer[1],flash.EXO.ECCalbration_Buffer[2]);		
		//Dprintf("EXO_CAL_START[1 point] = USART[%d]\n",flash.EXO.UartPort);//보정시 포트가 바뀌지 않는 현상이 발생하여 테스용으로 만든다.
	}
	///////////////////////////////////////////////////////////////////////////////
	//		1point 보정 값을 송신한다.
	//		1차 보정이 완료되면 flash.EXO.CalStart[flash.EXO.UartPort]=ON 된다.
	///////////////////////////////////////////////////////////////////////////////
	else if(win->ID==21 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT && flash.EXO.CalStart[flash.EXO.UartPort]==ON)
	{
		Dprintf("첫번째 교정완료\n");
		#if defined(TYPE_APPROVAL_ENABLE) 
		if(flash.EXO.CalHexaItemCode[flash.EXO.UartPort]==0x03) {		//pH
			flash.codeZERO[0]=flash.EXO.ECCalbration_Buffer[0];
			//Dprintf("pH codeZERO = %.2f\n",flash.codeZERO[0]);
		}
		else if(flash.EXO.CalHexaItemCode[flash.EXO.UartPort]==0x0A) {	//DO			
			flash.codeZERO[1]=0;
			flash.codeSPAN[1]=flash.EXO.ECCalbration_Buffer[0];
			//Dprintf("DO codeZERO = %.2f\n",flash.codeZERO[1]);
		}
		 
		Dprintf("형식\n");		
		#endif	
		SetWidgetColor(EXO_CalStartID,clBlue, clWhite);							// 버튼이미지 효과를 만든다.
		SetWidgetColor(EXO_CalPoint1ID,clBlue, clRed);							// 1point start
		SetWidgetColor(EXO_CalPoint2ID,clBlue, clWhite);						// 2point wait
		SetWidgetColor(EXO_CalPoint3ID,clBlue, clWhite);						// 3point wait
		flash.EXO.CalPoint=1;											// 보정 카운트
		flash.EXO.CalCommand=0;											// 명령 리스트 카운트값 
		
		flash.EXO.CalStartStep[flash.EXO.UartPort]=2;			// 보정스텝
		
		Dprintf("EXO_CAL_START[2 point] = USART[%d]\n",flash.EXO.UartPort);//보정시 포트가 바뀌지 않는 현상이 발생하여 테스용으로 만든다.
		
		SetWidgetText(EXO_CalID[7],"1점 보정인경우 COMPLETE 터치합니다.");
	}
	///////////////////////////////////////////////////////////////////////////////
	//		2point 보정 값을 송신한다.
	///////////////////////////////////////////////////////////////////////////////
	else if(win->ID==22 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT&& flash.EXO.CalPoint==1)
	{
		Dprintf("두번째 교정완료\n");
		#if defined(TYPE_APPROVAL_ENABLE) 
		if(flash.EXO.CalHexaItemCode[flash.EXO.UartPort]==0x03) {//pH
			flash.codeSPAN[0]=flash.EXO.ECCalbration_Buffer[1];
		}
		else if(flash.EXO.CalHexaItemCode[flash.EXO.UartPort]==0x0A) {//DO
			if(flash.EXO.ECCalbration_Buffer[0]!=0.0f) {
				flash.codeZERO[1]=flash.EXO.ECCalbration_Buffer[0];
			} else {
				flash.codeZERO[1]=0;
			}
			flash.codeSPAN[1]=flash.EXO.ECCalbration_Buffer[1];
		}
		//Dprintf("형식승인 pH 2차교정버퍼값\n");
		#endif	
		SetWidgetColor(EXO_CalStartID,clBlue, clWhite);							//버튼이미지 효과를 만든다.
		SetWidgetColor(EXO_CalPoint1ID,clBlue, clWhite);	
		SetWidgetColor(EXO_CalPoint2ID,clBlue, clRed);	
		SetWidgetColor(EXO_CalPoint3ID,clBlue, clWhite);	
		flash.EXO.CalPoint=2;													//보정 카운트
		flash.EXO.CalCommand=0;											//명령 리스트 카운트값 
		flash.EXO.CalStartStep[flash.EXO.UartPort]=3;											//보정스텝
		Dprintf("EXO_CAL_START[3 point] = USART[%d]\n",flash.EXO.UartPort);//보정시 포트가 바뀌지 않는 현상이 발생하여 테스용으로 만든다.
		SetWidgetText(EXO_CalID[7],"2점 보정인경우 COMPLETE 터치합니다.");
	}	
	///////////////////////////////////////////////////////////////////////////////
	//		3point 보정 값을 송신한다.
	///////////////////////////////////////////////////////////////////////////////
	else if(win->ID==23 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT&& flash.EXO.CalPoint==2)
	{
		Dprintf("세번째 교정완료\n");
		SetWidgetColor(EXO_CalStartID,clBlue, clWhite);							//버튼이미지 효과를 만든다.
		SetWidgetColor(EXO_CalPoint1ID,clBlue, clWhite);	
		SetWidgetColor(EXO_CalPoint2ID,clBlue, clWhite);	
		SetWidgetColor(EXO_CalPoint3ID,clBlue, clRed);	
		flash.EXO.CalPoint=3;
		flash.EXO.CalCommand=0;											//명령 리스트 카운트값 
		flash.EXO.CalStartStep[flash.EXO.UartPort]=4;											//보정스텝
		SetWidgetText(EXO_CalID[7],"COMPLETE 터치합니다.");
	}	
	///////////////////////////////////////////////////////////////////////////////
	//		보정종료버튼 // EXIT 겸용
	///////////////////////////////////////////////////////////////////////////////
	else if(win->ID==24 )
	{
		Dprintf("EXIT\n");

		if(flash.EXO.CalPoint==1||flash.EXO.CalPoint==2||flash.EXO.CalPoint==3)
		{
			flash.EXO.CalStartStep[flash.EXO.UartPort]=5;					//보정스텝
			flash.EXO.CalPoint=0;											//증가해야 하는 항목이 있고 종료해야 하는 항목이 있다.
			flash.EXO.CalCommand=0;											//명령 리스트 카운트값 
			
			SetWidgetColor(EXO_CalStartID,clBlue, clWhite);					//버튼이미지 효과를 만든다.
			SetWidgetColor(EXO_CalPoint1ID,clBlue, clWhite);	
			SetWidgetColor(EXO_CalPoint2ID,clBlue, clWhite);	
			SetWidgetColor(EXO_CalPoint3ID,clBlue, clWhite);	
			SetWidgetColor(EXO_CalExit,clBlue, clRed);	
			
			SetWidgetText(EXO_CalID[7],"잠시만 기다려 주세요.");
			gWaitMessageFlag = ON;
			#if defined(TYPE_APPROVAL_ENABLE)
//			flash.codeMv1=gv.codeMv1;
//			flash.codeMv2=gv.codeMv2;
 			#endif
		}
		else
		{
			StopTimer(EXO_TimerID);
			CloseTimer(EXO_TimerID);
			/**
			*	교정식이 바뀐다.
			*/
#if ODO_SAT_CAL			
            if(gDO1ChannelChange == 1)
            {
                Dprintf("ODO Sat to ODO mg/L\n");
                strcpy(flash.Uart[USART_1]->SetupName[flash.EXO.CalItem], "ODO mg/L");
                flash.EXO.Sensor_ODO_SAT_Status[USART_1][0] = OFF;
                flash.EXO.Sensor_ODO_MGL_Status[USART_1][0] = ON;
            }
            if(gDO2ChannelChange == 1)
            {
                Dprintf("ODO Sat to ODO mg/L\n");
                strcpy(flash.Uart[USART_2]->SetupName[flash.EXO.CalItem], "ODO mg/L");
                flash.EXO.Sensor_ODO_SAT_Status[USART_2][0] = OFF;
                flash.EXO.Sensor_ODO_MGL_Status[USART_2][0] = ON;
            }
            if(gDO3ChannelChange == 1)
            {
                Dprintf("ODO Sat to ODO mg/L\n");
                strcpy(flash.Uart[USART_3]->SetupName[flash.EXO.CalItem], "ODO mg/L");
                flash.EXO.Sensor_ODO_SAT_Status[USART_3][0] = OFF;
                flash.EXO.Sensor_ODO_MGL_Status[USART_3][0] = ON;
            }
#endif
			GetWidgetText(EXO_HiperID,data);
			
			flash.EXO.WiperEnableTimeSV=atoi(data);
			
			Dprintf("EXO 세척시간 = %d SEC 간격\n",flash.EXO.WiperEnableTimeSV);
			
			gv.wiperEnableTimePV=0;
			
			flash.EXO.ECCalbration_Buffer[0]=0.0F;
			flash.EXO.ECCalbration_Buffer[1]=0.0F;
			flash.EXO.ECCalbration_Buffer[2]=0.0F;
			flash.EXO.PV[0]=0.0F;
			flash.EXO.PV[1]=0.0F;
			flash.EXO.PV[2]=0.0F;
			flash.EXO.SV[0]=0.0F;
			flash.EXO.SV[1]=0.0F;
			flash.EXO.SV[2]=0.0F;
			flash.EXO.temp[0]=0.0F;
			flash.EXO.temp[1]=0.0F;
			flash.EXO.temp[2]=0.0F;
			flash.EXO.CalPoint=1;//전도도는 1point 고정이다.
			flash.EXO.CalCommand=0;// 명령 리스트 카운트값 
			flash.EXO.CalStartStep[0]=0;//보정스텝 [flash.EXO.UartPort]
			flash.EXO.CalStartStep[1]=0;
			flash.EXO.CalStartStep[2]=0;
			
			flash.EXO.CalStart[flash.EXO.UartPort]=OFF;//보정을 시작하는 명령이다. EXO에 데이터를 요청하는 명령이 달라진다.
			
			flash.EXO.UartPort=EXO_DATA_REQUEST_PORT;

            //통신 상태값을 0x03으로 송신한다.
			Dprintf("[SYSTEM] TMS 상태코드 값을 교정모드에서 측정모드로 전환한다.\n");
            gv.tmsStatusCode[USART_1] = 0x00;
			gv.tmsStatusCode[USART_2] = 0x00;
			gv.tmsStatusCode[USART_3] = 0x00;
			gv.tmsStatusCode[USART_4] = 0x00;
			
			if(!(strcmp(flash.Uart[flash.EXO.UartPort]->SetupName[1],"SPCOND mS/cm"))) {
				if(flash.EXO.SpcondmgmsEnable[flash.EXO.UartPort]==0) {
					sprintf(flash.Uart[flash.EXO.UartPort]->SetupName[1],"%s","SPCOND uS/cm");
				} else {
					sprintf(flash.Uart[flash.EXO.UartPort]->SetupName[1],"%s","SPCOND mS/cm");
				}
			} else {
				Dprintf("EXO 전도도 단위가 선택되지 않는다.");
			}
            #if defined(DAECHEONGHO_SUGILGWANLISO)//대청호 수질관리소에 적용된다.
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
			
			flash.EXO.exoCalMode=0;//교정모드종료
			
            DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
						
			CloseWindow(NULL, parent);
		}//EXO_CalExit
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//		보정채널 변경시에는 모든값을 초기화 한다.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	else if(win->ID==6)//UART1 채널로 변경
	{	
		flash.EXO.UartPort=USART_1;
        if(flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]==ON) {
            strcpy( flash.Uart[USART_1]->SetupName[1] ,"SPCOND mS/cm");
        }
		if(flash.Uart[0]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[0],clBlue, clRed);		
		if(flash.Uart[1]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[1],clBlue, clWhite);
		if(flash.Uart[2]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[2],clBlue, clWhite);
		SetWidgetText(EXO_CalITem_ID,flash.Uart[flash.EXO.UartPort]->SetupName[flash.EXO.CalItem]);//포트 id
		
		flash.EXO.CalStart[flash.EXO.UartPort]=OFF;

		flash.EXO.ECCalbration_Buffer[0]=0.0F;
		flash.EXO.ECCalbration_Buffer[1]=0.0F;
		flash.EXO.ECCalbration_Buffer[2]=0.0F;
		
		flash.EXO.PV[0]=0.0F;
		flash.EXO.PV[1]=0.0F;
		flash.EXO.PV[2]=0.0F;
		flash.EXO.SV[0]=0.0F;
		flash.EXO.SV[1]=0.0F;
		flash.EXO.SV[2]=0.0F;
		flash.EXO.temp[0]=0.0f;
		flash.EXO.temp[1]=0.0f;
		flash.EXO.temp[2]=0.0f;

		flash.EXO.CalPoint=0;//전도도는 1point 고정이다.	
		
		flash.EXO.CalCommand=0;
		flash.EXO.CalItem=0;
		flash.EXO.CalHexaItemCode[0]=0;	
		flash.EXO.CalHexaItemCode[1]=0;	
		flash.EXO.CalHexaItemCode[2]=0;	

		SetWidgetText(EXO_CalID[7],"CHANGE 버튼을 누르고 항목을 설정합니다.");
		
	}
	else if(win->ID==7)//UART2 채널로 변경
	{
		flash.EXO.UartPort=USART_2;
        if(flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]==ON) {
            strcpy( flash.Uart[USART_2]->SetupName[1] ,"SPCOND mS/cm");
        }
		if(flash.Uart[0]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[0],clBlue, clWhite);		
		if(flash.Uart[1]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[1],clBlue, clRed);
		if(flash.Uart[2]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[2],clBlue, clWhite);
		SetWidgetText(EXO_CalITem_ID,flash.Uart[flash.EXO.UartPort]->SetupName[flash.EXO.CalItem]);//포트 id
		flash.EXO.CalStart[flash.EXO.UartPort]=OFF;

		flash.EXO.ECCalbration_Buffer[0]=0.0F;
		flash.EXO.ECCalbration_Buffer[1]=0.0F;
		flash.EXO.ECCalbration_Buffer[2]=0.0F;
		
		flash.EXO.PV[0]=0.0F;
		flash.EXO.PV[1]=0.0F;
		flash.EXO.PV[2]=0.0F;
		flash.EXO.SV[0]=0.0F;
		flash.EXO.SV[1]=0.0F;
		flash.EXO.SV[2]=0.0F;
		flash.EXO.temp[0]=0.0f;
		flash.EXO.temp[1]=0.0f;
		flash.EXO.temp[2]=0.0f;

		flash.EXO.CalPoint=0;//전도도는 1point 고정이다.	
		
		flash.EXO.CalCommand=0;
		flash.EXO.CalItem=0;
		flash.EXO.CalHexaItemCode[0]=0;	
		flash.EXO.CalHexaItemCode[1]=0;	
		flash.EXO.CalHexaItemCode[2]=0;	
		
		SetWidgetText(EXO_CalID[7],"CHANGE 버튼을 누르고 항목을 설정합니다.");		
	}
	
	else if(win->ID==8)//UART3 채널로 변경
	{
		flash.EXO.UartPort=USART_3;
        if(flash.EXO.Sensor_COND_SP_Status[flash.EXO.UartPort][0]==ON) {
            strcpy( flash.Uart[USART_3]->SetupName[1] ,"SPCOND mS/cm");
        }
		if(flash.Uart[0]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[0],clBlue, clWhite);		
		if(flash.Uart[1]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[1],clBlue, clWhite);
		if(flash.Uart[2]->SensorID == SENSOR_EXO)SetWidgetColor(EXO_CalComPortID[2],clBlue, clRed);
		SetWidgetText(EXO_CalITem_ID,flash.Uart[flash.EXO.UartPort]->SetupName[flash.EXO.CalItem]);//포트 id
		
		flash.EXO.CalStart[flash.EXO.UartPort]=OFF;

		flash.EXO.ECCalbration_Buffer[0]=0.0F;
		flash.EXO.ECCalbration_Buffer[1]=0.0F;
		flash.EXO.ECCalbration_Buffer[2]=0.0F;
		
		flash.EXO.PV[0]=0.0F;
		flash.EXO.PV[1]=0.0F;
		flash.EXO.PV[2]=0.0F;
		flash.EXO.SV[0]=0.0F;
		flash.EXO.SV[1]=0.0F;
		flash.EXO.SV[2]=0.0F;
		flash.EXO.temp[0]=0.0f;
		flash.EXO.temp[1]=0.0f;
		flash.EXO.temp[2]=0.0f;

		flash.EXO.CalPoint=0;//전도도는 1point 고정이다.	
		
		flash.EXO.CalCommand=0;
		flash.EXO.CalItem=0;
		flash.EXO.CalHexaItemCode[0]=0;	
		flash.EXO.CalHexaItemCode[1]=0;	
		flash.EXO.CalHexaItemCode[2]=0;	

		SetWidgetText(EXO_CalID[7],"CHANGE 버튼을 누르고 항목을 설정합니다.");
	}
	
	else if(win->ID==10) //와이퍼 동작 시간 설정
	{
		WiperChange=1;
		KeyPad(win->Parent, win->Handle, EXO_HiperID, (1024-345)/2, (768-415)/2, FONT16, "", "Time SEC"); 
	}
	else if(win->ID==12)//와이퍼 동작
	{
		EventMessage("[SYSTEM] 와이퍼 동작실행");
		gv.wiperEnableFlag=ON;//보정 중에는 와이퍼를 수동으로 동작 시킨다. 이때 측정치는 올라오지 않는다. 30초대기 Flag 이다.
		EXO_Calibration_Command(flash.EXO.UartPort, 40, SONDE_ID, 0x30, 0x00, 0x01, 0xc9, 0x00, 0x00, NULL);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//				보정아이템 설정
//	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if(win->ID==9 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)// EXO 출력 ITEM 선택
	{
		/**
		*	교정식이 바뀐다.
		*/
#if ODO_SAT_CAL
		if(gDO1ChannelChange == ON)
		{
			Dprintf("ODO Sat to ODO mg/L\n");
			strcpy(flash.Uart[USART_1]->SetupName[flash.EXO.CalItem], "ODO mg/L");
			flash.EXO.Sensor_ODO_SAT_Status[USART_1][0] = OFF;
			flash.EXO.Sensor_ODO_MGL_Status[USART_1][0] = ON;
		}
		if(gDO2ChannelChange == ON)
		{
			Dprintf("ODO Sat to ODO mg/L\n");
			strcpy(flash.Uart[USART_2]->SetupName[flash.EXO.CalItem], "ODO mg/L");
			flash.EXO.Sensor_ODO_SAT_Status[USART_2][0] = OFF;
			flash.EXO.Sensor_ODO_MGL_Status[USART_2][0] = ON;
		}
		if(gDO3ChannelChange == ON)
		{
			Dprintf("ODO Sat to ODO mg/L\n");
			strcpy(flash.Uart[USART_3]->SetupName[flash.EXO.CalItem], "ODO mg/L");
			flash.EXO.Sensor_ODO_SAT_Status[USART_3][0] = OFF;
			flash.EXO.Sensor_ODO_MGL_Status[USART_3][0] = ON;
		}
#endif		
		if(gv.findDelayTimeCnt>20000) {
			SmartLogEXOItemKeypad(win->Parent, win->Handle,  NULL, (1024-940)/2, (768-700)/2, FONT16, "", "EXO");//860, 602	
		} else {
			sprintf(data,"센서가 초기화 중입니다. %d 초후 다시 실행해 주세요.",(20-(gv.findDelayTimeCnt/1000)));
			SetWidgetText(EXO_CalID[7],data);
		}
	}
	else if(win->ID==30 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT) {
		if(flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)KeyPad(win->Parent, win->Handle,  EXO_CalID[0], (800-345)/2, (600-415)/2, FONT16, "", "CALIBRATION BUFFER 1");
	}
	else if(win->ID==31 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT) {
		if(flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)KeyPad(win->Parent, win->Handle,  EXO_CalID[1], (800-345)/2, (600-415)/2, FONT16, "", "CALIBRATION BUFFER 2");
	}
	else if(win->ID==32 && flash.EXO.UartPort<EXO_DATA_REQUEST_PORT) {
		if(flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)KeyPad(win->Parent, win->Handle,  EXO_CalID[2], (800-345)/2, (600-415)/2, FONT16, "", "CALIBRATION BUFFER 3");
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//					EXIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	else if(win->ID==24)//EXIT
	{
		StopTimer(EXO_TimerID);
		CloseTimer(EXO_TimerID);
		GetWidgetText(EXO_HiperID,data);
		flash.EXO.WiperEnableTimeSV[flash.EXO.UartPort]=atoi(data);
		gv.wiperEnableTimePV[flash.EXO.UartPort]=0;
		flash.EXO.ECCalbration_Buffer[0]=0.0F;
		flash.EXO.ECCalbration_Buffer[1]=0.0F;
		flash.EXO.ECCalbration_Buffer[2]=0.0F;	
		flash.EXO.PV=0.0F;
		flash.EXO.SV=0.0F;
		flash.EXO.temp=0.0F;
		flash.EXO.CalPoint=1;//전도도는 1point 고정이다.
		flash.EXO.CalCommand=0;// 명령 리스트 카운트값 
		flash.EXO.CalStartStep=0;//보정스텝
		flash.EXO.CalStart=OFF;//보정을 시작하는 명령이다. EXO에 데이터를 요청하는 명령이 달라진다.
		StructToNand("SmartLogConfig.data", &SmartLogFlash, sizeof(__SmartLogdata));
		CloseWindow(NULL, parent);
	}
*/

	return(1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				CALIBRATION BUFFER SET
//				1. 보정용액을 설정한다.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int	 SmartLogEXOCalBuffer1Event(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{	
	Windows 	*win = GetWindow(parent, handle, "SmartLogEXOCalBuffer1Event");
	Windows		*parent_win = FindContainerWindow(parent,  "SmartLogEXOCalBuffer1Event");
	//unsigned char	fbuf[10];
	if(parent_win == NULL)Dprintf("SmartLogEXOCalBuffer1Event:: Windows Error\n");	
	if(flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)KeyPad(win->Parent, win->Handle,  EXO_CalID[0], (800-345)/2, (600-415)/2, FONT16, "", "CALIBRATION BUFFER 1");
	return(1);
}
int	 SmartLogEXOCalBuffer2Event(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{	
	Windows 	*win = GetWindow(parent, handle, "SmartLogEXOCalBuffer2Event");
	Windows		*parent_win = FindContainerWindow(parent,  "SmartLogEXOCalBuffer2Event");
	if(parent_win == NULL)Dprintf("SmartLogEXOCalBuffer2Event:: Windows Error\n");
	if(flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)KeyPad(win->Parent, win->Handle,  EXO_CalID[1], (800-345)/2, (600-415)/2, FONT16, "", "CALIBRATION BUFFER 2");
	return(1);
} 
int	 SmartLogEXOCalBuffer3Event(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{	
	Windows 	*win = GetWindow(parent, handle, "SmartLogEXOCalBuffer3Event");
	Windows		*parent_win = FindContainerWindow(parent,  "SmartLogEXOCalBuffer3Event");
	if(parent_win == NULL)Dprintf("SmartLogEXOCalBuffer3Event:: Windows Error\n");
	if(flash.EXO.UartPort<EXO_DATA_REQUEST_PORT)KeyPad(win->Parent, win->Handle,  EXO_CalID[2], (800-345)/2, (600-415)/2, FONT16, "", "CALIBRATION BUFFER 3");
	return(1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				CALIBRATION SCREEN CREATE
//				1. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int	 SmartLogNysiScreenCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//char	buf[64];
	char	fbuf[20];
	int		sx = 10;
	//int		sy = 50;
	float	fdata1=0.0f,fdata2=0.0f,fdata3=0.0f;
	U32		id=0;
	
	Windows 	*win = GetWindow(parent, handle,"SmartLogSontekConfigCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	
	if(win==NULL) {
		Dprintf("SmartLogNysiScreenCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL) {
		Dprintf("SmartLogNysiScreenCreateEvent::GetGraphics Error\n");
		return(0);
	}	
	PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);//Dummy
	
	Dprintf("[SYSTEM] TMS 상태코드 값을 교정모드로 전환한다.\n");
	EventMessage("[SYSTEM] EXO 교정실행");
	gv.tmsStatusCode[USART_1]=3;
	gv.tmsStatusCode[USART_2]=3;
	gv.tmsStatusCode[USART_3]=3;
	gv.tmsStatusCode[USART_4]=3;

	
	if(flash.systemColorNo!=6) {		
		Gr->SetColor(clBlack);
	} 
	else {
		Gr->SetColor(RGB565CONVERT(230,  230,  230));
	}
	Gr->FillRect(0, 0, win->client_width+1, win->client_height);	//PANEL CREATE
	
    gv.tmsStatusCode[USART_1] = 3;
    
	flash.EXO.exoCalMode=1;//EXO 교정모드
    	
    /**
    *   DO mg/L 측정시 보정을 DO Sat 로 진행 할 수 있게 한다.
    */
	#if ODO_SAT_CAL	
    gDO1ChannelChange = 0;
	gDO2ChannelChange = 0;
	gDO3ChannelChange = 0;
	#endif
	flash.EXO.UartPort=EXO_DATA_REQUEST_PORT;// 1. 보정 메뉴들이 동작하지 않는다. 2. 선택되지 않은 포트는 동작하지 않는다.( 보정시 충돌을 방지 한다.
	
	flash.EXO.CalStart[0]=OFF;//UART1
	flash.EXO.CalStart[1]=OFF;//UART2
	flash.EXO.CalStart[2]=OFF;//UART2

	flash.EXO.ECCalbration_Buffer[0]=0.0F;
	flash.EXO.ECCalbration_Buffer[1]=0.0F;
	flash.EXO.ECCalbration_Buffer[2]=0.0F;

	//출력 측정치 초기화
	flash.EXO.PV[0]=0.0F;
	flash.EXO.PV[1]=0.0F;
	flash.EXO.PV[2]=0.0F;
	
	flash.EXO.SV[0]=0.0F;
	flash.EXO.SV[1]=0.0F;
	flash.EXO.SV[2]=0.0F;
	
	flash.EXO.temp[0]=0.0f;
	flash.EXO.temp[1]=0.0f;
	flash.EXO.temp[2]=0.0f;

	flash.EXO.CalPoint=0;//전도도는 1point 고정이다.	
	
	flash.EXO.CalCommand=0;
	flash.EXO.CalItem=0;
	flash.EXO.CalHexaItemCode[0]=0;
	flash.EXO.CalHexaItemCode[1]=0;
	flash.EXO.CalHexaItemCode[2]=0;
	
	fdata1=flash.EXO.ECCalbration_Buffer[0];
	fdata2=flash.EXO.ECCalbration_Buffer[1];
	fdata3=flash.EXO.ECCalbration_Buffer[2];

//FINISH
	WiperChange=0;	
	
	EXO_CalITem_ID = TextBox(handle, sx, 10, 90+90+50, 50, 2,"NONE", clYellow, clBlack, CENTER_ALIGN, READ_ONLY, NULL);
				   SetWidgetColor(EXO_CalITem_ID,clBlack,clYellow);
	
	
	id 	= Button(handle, sx+100+100+50+25, 10, 90+15, 50, TEXT_BUTTON, "Button_76x76_off.bmp", "Button_76x76_on.bmp", "",  FONT16, "CHANGE", PUSH_BUTTON, 1, SmartLogNysiScreenClickEvent);
				  SetWidgetColor(id,clBlue,clWhite);	
	EXO_FactorID 	= Button(handle, sx+100+100+100+15+50+25, 10, 90+15, 50, TEXT_BUTTON, "Button_76x76_off.bmp", "Button_76x76_on.bmp", "",  FONT16, "FACTOR", PUSH_BUTTON, 2, SmartLogNysiScreenClickEvent);
				  SetWidgetColor(EXO_FactorID,clBlue,clWhite);	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//			각 항목에 따른 설정값 선택항목에 따라서 변경된다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Label(handle, sx, 10+50+10, 90+90+50, 50, "1P Buffer",  clBlack); 
				   sprintf(fbuf,"%.3f",fdata1);
				   fbuf[10]=0;
	//EXO_CalID[0] = TextBox(handle, sx+100+100+50, 10+50+10, 90+90+10+30+50 , 50, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, RW_MODE, SmartLogEXOCalBuffer1Event);
	EXO_CalID[0] = SButton(handle, sx+100+100+50, 10+50+10, 90+90+10+30+50, 50, 0, fbuf,  PUSH_BUTTON, 30, SmartLogNysiScreenClickEvent); 

	SetWidgetColor(EXO_CalID[0],clBlue,clWhite);
				   
    Label(handle, sx, 10+50+50+10, 90+90+50, 50, "2P Buffer",  clBlack);
				   sprintf(fbuf,"%.3f",fdata2);
				   fbuf[10]=0;
	//EXO_CalID[1] = TextBox(handle, sx+100+100+50, 10+50+50+10, 90+90+10+30+50, 50, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, RW_MODE, SmartLogEXOCalBuffer2Event);
	EXO_CalID[1] = SButton(handle, sx+100+100+50, 10+50+50+10, 90+90+10+30+50, 50, 0, fbuf,  PUSH_BUTTON, 31, SmartLogNysiScreenClickEvent); 
				   SetWidgetColor(EXO_CalID[1],clBlue,clWhite);
	
    Label(handle, sx, 10+50+50+50+10, 90+90+50, 50, "3P Buffer",  clBlack);
				   sprintf(fbuf,"%.3f",fdata3);
				   fbuf[10]=0;
	//EXO_CalID[2] = TextBox(handle, sx+100+100+50, 10+50+50+50+10, 90+90+10+30+50 , 50, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, RW_MODE, SmartLogEXOCalBuffer3Event);
	EXO_CalID[2] = SButton(handle, sx+100+100+50, 10+50+50+50+10, 90+90+10+30+50, 50, 0, fbuf,  PUSH_BUTTON, 32, SmartLogNysiScreenClickEvent); 
				   SetWidgetColor(EXO_CalID[2],clBlue,clWhite);		   
	
	//EXO_CalMessageID=TransParencyTextBox(handle, sx, 5+42+42+42+42, 90+90+10+90+90+10, 32, 2,  "STANDBY", CENTER_ALIGN, READ_ONLY, NULL);
	//			   SetWidgetColor(EXO_CalMessageID,clWhite,clRed);
	//EXO_CalID[3] = TextBox(handle, sx+100+100, 5+42+42+42+42, 90+90+10, 32, 2, "0.00", RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
				   //SetWidgetColor(EXO_CalITem_ID,clWhite,clBlack);
	
	
	//id=TransParencyTextBox(handle, sx, 5+42+42+42+42, 90+90, 32, 2,  "CURRENT(PRE)", CENTER_ALIGN, READ_ONLY, NULL);
    Label(handle, sx, 10+50+50+50+50+25, 90+90+50, 50, "CURRENT(PRE)",  clBlack);
				   //SetWidgetColor(id,clWhite,clBlack);
				   sprintf(fbuf,"%.3f",flash.EXO.PV[flash.EXO.UartPort]);
				   fbuf[10]=0;				   
	//EXO_CalID[4] = TextBox(handle, sx+100+100+50, 10+50+50+50+50+25, 90+90+10+30+50, 50, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
	EXO_CalID[4] = SButton(handle, sx+100+100+50, 10+50+50+50+50+25, 90+90+10+30+50, 50, 0, fbuf,  SELECT_BUTTON, 1, NULL); 
				   SetWidgetColor(EXO_CalID[4],clWhite,clBlack);

	//id=TransParencyTextBox(handle, sx, 5+42+42+42+42+42, 90+90, 32, 2,  "PENDING(POST)", CENTER_ALIGN, READ_ONLY, NULL);
    id = Label(handle, sx, 10+50+50+50+50+50+25, 90+90+50, 50, "PENDING(POST)",  clRed);
	SetWidgetColor(id,clYellow,clBlack);
				   sprintf(fbuf,"%.3f",flash.EXO.SV[flash.EXO.UartPort]);
				   fbuf[10]=0;				   
	//EXO_CalID[5] = TextBox(handle, sx+100+100+50, 10+50+50+50+50+50+25, 90+90+10+30+50, 50, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
	EXO_CalID[5] = SButton(handle, sx+100+100+50, 10+50+50+50+50+50+25, 90+90+10+30+50, 50, 0, fbuf,  SELECT_BUTTON, 1, NULL); 
					SetWidgetColor(EXO_CalID[5],clWhite,clBlack);

	//id=TransParencyTextBox(handle, sx, 5+42+42+42+42+42+42, 90+90, 32, 2,  "TEMP C", CENTER_ALIGN, READ_ONLY, NULL);
    Label(handle, sx, 10+50+50+50+50+50+50+25, 90+90+50, 50, "TEMP 'C",  clBlack);
				   //SetWidgetColor(id,clWhite,clBlack);
				   sprintf(fbuf,"%.3f",flash.EXO.temp[flash.EXO.UartPort]);
				   fbuf[10]=0;
	//EXO_CalID[6] = TextBox(handle, sx+100+100+50, 10+50+50+50+50+50+50+25, 90+90+10+30+50, 50, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
	EXO_CalID[6] = SButton(handle, sx+100+100+50, 10+50+50+50+50+50+50+25, 90+90+10+30+50, 50, 0, fbuf,  SELECT_BUTTON, 1, NULL); 
				   SetWidgetColor(EXO_CalID[6],clWhite,clBlack);


	//EXO_CalID[9]=TransParencyTextBox(handle, sx, 5+42+42+42+42+42+42+42, 90+90, 32, 2,  "CAL POINTS", CENTER_ALIGN, READ_ONLY, NULL);
	/*
    Label(handle, sx, 5+42+42+42+42+42+42+42, 90+90, 32, "CAL POINTS",  clBlack);
				   //SetWidgetColor(EXO_CalID[9],clWhite,clBlack);
				   sprintf(fbuf,"%10d",flash.EXO.CalPoint);
				   fbuf[10]=0;
				   //Dprintf("SmartLogNysiScreenCreateEvent :: flash.EXO.CalPoint = %d\n",flash.EXO.CalPoint);
	EXO_CalID[7] = TextBox(handle, sx+100+100, 5+42+42+42+42+42+42+42, 90+90+10, 32, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
				   SetWidgetColor(EXO_CalID[7],clWhite,clBlack);
	*/	
	//화면에 출력할 항목을 선택한다.
	EXO_CalID[3]= Button(handle, sx, 10+50+50+50+50+50+50+50+50, 130, 50, TEXT_BUTTON, " ", " ", "", FONT16, "FIND", PUSH_BUTTON, 9, SmartLogNysiScreenClickEvent);
				SetWidgetColor(EXO_CalID[3],clBlue,clWhite);

	id	= Button(handle, sx+150+130+50, 10+50+50+50+50+50+50+50+50, 140+50, 50, TEXT_BUTTON, " ", " ", "", FONT16, "WIPER START", PUSH_BUTTON, 12, SmartLogNysiScreenClickEvent);
				  SetWidgetColor(id,clBlue,clWhite);

	id	= Button(handle, sx+140, 10+50+50+50+50+50+50+50+50, 50, 50, TEXT_BUTTON, " ", " ", "", FONT16, "SEC", PUSH_BUTTON, 10, SmartLogNysiScreenClickEvent);
	SetWidgetColor(id,clBlue,clWhite);
	
	sprintf(fbuf,"%d", flash.EXO.WiperEnableTimeSV);
	EXO_HiperID = SButton(handle, sx+140+60, 10+50+50+50+50+50+50+50+50, 65+50, 50, 0, fbuf,  PUSH_BUTTON, 10, SmartLogNysiScreenClickEvent);   
	//EXO_HiperID = TextBox(handle, sx+140+60, 10+50+50+50+50+50+50+50+50, 65+50, 50, 2, fbuf, RGB565CONVERT(191, 191, 191), WHITE, CENTER_ALIGN, READ_ONLY, NULL);
				   SetWidgetColor(EXO_HiperID,clWhite,clBlack);
	//
	// 통신포트가 설정이 되어 있으면 ENABLE 된다.
	//
	if(flash.Uart[0]->SensorID == SENSOR_EXO)
	{
		strcpy( flash.Uart[USART_1]->SetupName[1] ,"SPCOND mS/cm");
		EXO_CalComPortID[0]	= Button(handle, sx, 379+7+50+30, 130, 50, TEXT_BUTTON, " ", " ", "", FONT16, "UART1", PUSH_BUTTON, 6, SmartLogNysiScreenClickEvent);
		SetWidgetColor(EXO_CalComPortID[0],clBlue,clWhite);	
	}
	if(flash.Uart[1]->SensorID == SENSOR_EXO)
	{
		strcpy( flash.Uart[USART_2]->SetupName[1] ,"SPCOND mS/cm");
		EXO_CalComPortID[1]	= Button(handle, sx+140, 379+7+50+30, 130, 50, TEXT_BUTTON, " ", " ", "", FONT16, "UART2", PUSH_BUTTON, 7, SmartLogNysiScreenClickEvent);
		SetWidgetColor(EXO_CalComPortID[1],clBlue,clWhite);
	}
	
	if(flash.Uart[2]->SensorID == SENSOR_EXO) {
		strcpy( flash.Uart[USART_3]->SetupName[1] ,"SPCOND mS/cm");
		EXO_CalComPortID[2]	= Button(handle, sx+140+140, 379+7+50+30, 130, 50, TEXT_BUTTON, " ", " ", "", FONT16, "UART3", PUSH_BUTTON, 8, SmartLogNysiScreenClickEvent);
		SetWidgetColor(EXO_CalComPortID[2],clBlue,clWhite);	
	}
	EXO_CalID[7] = SButton(handle, sx, 379+7+50+30+50+10, 520, 70, 0, " ",  SELECT_BUTTON, 7, 0);
	SetWidgetText(EXO_CalID[7],"통신중인 포트를 선택합니다.");
	
	
	//보정진행버튼
	EXO_CalStartID	= Button(handle, sx, 379+7+7+50+60+100, 75+25, 50, TEXT_BUTTON, " ", " ", "", FONT16, "START", PUSH_BUTTON, 20, SmartLogNysiScreenClickEvent);
	SetWidgetColor(EXO_CalStartID,clBlue,clWhite);		
	EXO_CalPoint1ID = Button(handle, sx+80+25, 379+7+7+50+60+100, 75+25, 50, TEXT_BUTTON, " ", " ", "", FONT16, "1P APPLY", PUSH_BUTTON, 21, SmartLogNysiScreenClickEvent);
	SetWidgetColor(EXO_CalPoint1ID,clBlue,clWhite);		
	EXO_CalPoint2ID = Button(handle, sx+80+80+25+25, 379+7+7+50+60+100, 75+25, 50, TEXT_BUTTON, " ", " ", "", FONT16, "2P APPLY", PUSH_BUTTON, 22, SmartLogNysiScreenClickEvent);
	SetWidgetColor(EXO_CalPoint2ID,clBlue,clWhite);		
	EXO_CalPoint3ID	= Button(handle, sx+80+80+25+25+80+25, 379+7+7+50+60+100, 75+25, 50, TEXT_BUTTON, " ", " ", "",  FONT16, "3P APPLY", PUSH_BUTTON, 23, SmartLogNysiScreenClickEvent);
	SetWidgetColor(EXO_CalPoint3ID,clBlue,clWhite);	
	EXO_CalExit    = Button(handle, sx+80+80+25+25+80+25+80+25, 379+7+7+50+60+100, 100, 50, TEXT_BUTTON, " ", " ", "",  FONT16, "EXIT", PUSH_BUTTON, 24, SmartLogNysiScreenClickEvent);
	SetWidgetColor(EXO_CalExit,clBlue,clWhite);	
	
	
	//Gr->Draw(parent, handle, command, param, Data);	
	EXO_TimerID=OpenTimer(parent, handle, VALUE_DISP_TIME_MS);
	StartTimer(EXO_TimerID);
	PaintDrawControl(ON);
	return(1);
}

int	 SmartLogNysiScreenPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
void SmartLogNysiScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title) {
	WidgetStruct wid_struct;	

	wid_struct.FontNo = font_no;
	//wid_struct.CallWidget_ID = id;

	makeWindow(
				LCD,
				"SmartLogNysiScreen",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				450+100, 700,			// Size
				SHOW,					// Create after Show
				SmartLogNysiScreenCreateEvent,		// CreateEvent
				SmartLogNysiScreenPaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				SmartLogNysiScreenTimerEvent,					// TimerEvent
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

