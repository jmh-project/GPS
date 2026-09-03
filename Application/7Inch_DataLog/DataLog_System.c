/******************************************************************************
*       File name	: Sensor_Main.c 
*		Description 	: 7인치 데이터로거 시작윈도우
*		Company	: ziontech co.,
*		Version		: 0.10 (2015년 4월23일 Start)
*		Contact		: www.ziontek.co.kr
******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
// 이윈도우에서 사용할 터치 사운드의 정의
#define		TOUCH_SOUND			1	 

/******************************************************************************
							Private & Local Variables
******************************************************************************/
volatile static int play_timer=0;    

/******************************************************************************
							Function Prototype
******************************************************************************/
int Info_Window(char *title, char *text);
U32 GroupPanel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text);
int DataLog_UartRateWindow(int 	sx, 
							int 	sy, 
							char 	*IconName,
							int 	id,
							int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
int DataLog_UartItemWindow(int 	sx, 
							int 	sy, 
							char 	*IconName,
							int 	id,
							int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
U32 SLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style);
int SensorInOutChannelConnect(void);
U32 Label(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int	fontcolor);
int NInfo_Window(char *title, char *text);
unsigned int    MainScreenCreate( HANDLE parent, HANDLE handle );     
int Calendar_Window(void);
int PONSEL_DO_Window(void);
int PONSEL_EC_Window(void);
int UINH4_Window(void);                            
void SmartLogNysiScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
void CONDScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
int DataLog_MSeriesSelectWindow(int 	sx, 
int 	sy, 
char 	*IconName,
int 	id,
int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) ); //사용자가 이 윈도우에서 받을 콜백함수를 등록한다.
int DataLog_FillSetupWindow(int 	sx, 
							int 	sy, 
							char 	*IconName,
							int 	id,
							int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
int ECD_PH_Window(void);
void DataLog_TempFactroKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
void DataLog_420SelectKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
                            
void DataLog_DispItemKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title); 
void DataLog_ValuePointKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);  
void DepthScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
void SystemColorSet(void);
#ifdef SPECTRO_D_ENABLE
extern void ULIKScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);                            
#endif                            
                            
/******************************************************************************
							Example Program
******************************************************************************/
//이함수는 위젯의 키이벤트를 받기위해 사용한다.
#if defined (FARM_MANAGEMENT_ENABLE)
volatile int gScanCleanFlag=0;							
#endif	
int Ulik_MW_CODCalClear(int comPort, int id, int function, int addr1, int addr2);      
int colorCount=0;                            
int  SystemMain_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle," ");
	Graphics 	*Gr = GetGraphics(NULL, parent, CLIENT_AREA); 	//그래픽영역을 얻는다.
    
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);	//dummy code	  
	unsigned int nData = 0;
	unsigned char buf[128];
	//float fData=0;

    //서버
    if(win->ID == 1)//IP
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 2)//SUB
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 3)//GATE
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 4)//PORT
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 5)//MAC
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    //클라이언트
    else if(win->ID == 6)//IP
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 7)//SUB
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 8)//GATE
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 9)//PORT
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 10)//MAC
    {
        KeyBoard(gv.handle[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }
    else if(win->ID == 11)//시간설정
    {
        Calendar_Window();
    }
    else if(win->ID == 12)
    {
        KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "자동리부팅시간설정");
    }
    else if(win->ID == 13)
    {
		if(flash.sleepModeFlag==1) {
			DataLog_MessageBox_Window("절전모드", "절전모드 취소합니다.");
			flash.sleepModeFlag=0;
			SetWidgetText(gv.handle[win->ID],"실행");
			SetWidgetStatus(gv.handle[win->ID],0);
		} else {
			DataLog_MessageBox_Window("절전모드", "절전모드 실행합니다.");
			flash.sleepModeFlag=1;
			SetWidgetText(gv.handle[win->ID],"정지");
			SetWidgetStatus(gv.handle[win->ID],1);
		}
    }
    else if(win->ID == 14)
    {         
        DataLog_MessageBox_Window("업데이트", "프로그램 업데이트를 실행합니다.");
    }
    else if(win->ID == 15)//공장초기화
    {
        DataLog_MessageBox_Window("공장초기화", "RTU 설정값을 재설정해야 합니다.");
    }
    else if(win->ID == 17)
    {
        DataLog_MessageBox_Window("복원파일생성", "RTU 복원파일을 생성합니다.");
    }
    else if(win->ID == 18) 
    {
        DataLog_ValuePointKeypad(win->Parent, win->Handle,  NULL, 0, 0, 0, "", "소수 자릿수 설정");        
    }
    else if(win->ID == 19) 
    {
		#if defined(TYPE_APPROVAL_ENABLE)
			flash.saveMinuteSv=5;//형식승인 버젼은 5분데이터만 저장한다.
		#else
        if(flash.saveMinuteSv==1) {
            flash.saveMinuteSv=5;
        } 
        else if(flash.saveMinuteSv==5) {
            flash.saveMinuteSv=10;
        } else {
            flash.saveMinuteSv=1;
        }
		#endif
        sprintf((char *)buf,"%2d", flash.saveMinuteSv);
        SetWidgetText(gv.handle[win->ID],(char *)buf);
    }
    #ifdef EXO_ENABLE  
    else if(win->ID == 20) { SmartLogNysiScreen(win->Parent, win->Handle,  NULL, (1024-550)/2, (768-700)/2, FONT16, "", "EXO CALIBRATION"); } 
	else if(win->ID == 22) { 
		if(flash.EXO.SpcondmgmsEnable[USART_1] == 1)
		{
			flash.EXO.SpcondmgmsEnable[USART_1] = 0;
			SetWidgetText(gv.handle[win->ID],"uS/cm");
		}
		else
		{
			flash.EXO.SpcondmgmsEnable[USART_1] = 1;
			SetWidgetText(gv.handle[win->ID],"mS/cm"); 
		}
    }
	#if defined(TYPE_APPROVAL_ENABLE)
	#if defined(RTU_VER12_ENABLE)
	else if(win->ID == 24) {
		TypeApprovalFactorKeypad(win->Parent, win->Handle,  NULL, 0, 0, 0, "", "pH 측정상수");  
	}
	else if(win->ID == 26) {
		TypeApprovalFactorKeypad(win->Parent, win->Handle,  NULL, 0, 0, 0, "", "DO 측정상수");  
	}
	#endif
    else if(win->ID == 28) {
        TypeApprovalNameKeypad(win->Parent, win->Handle,  NULL, 0, 0, 0, "", "TMS 항목코드변경");  
    }
	#else 
    else if(win->ID == 24) { 
		if(flash.EXO.SpcondmgmsEnable[USART_2] == 1)
		{
			flash.EXO.SpcondmgmsEnable[USART_2] = 0;
			SetWidgetText(gv.handle[win->ID],"uS/cm");
		}
		else
		{
			flash.EXO.SpcondmgmsEnable[USART_2] = 1;
			SetWidgetText(gv.handle[win->ID],"mS/cm"); 
		}
    }
	#endif
    //else if(win->ID == 22) { DataLog_DispItemKeypad(win->Parent, win->Handle,  NULL, 0, 0, 0, "", "EXO 항목설정");  } 
    #endif
    #ifdef YSI_ODD_ENABLE
    else if(win->ID == 20) {
        ODO_Window();
    }
    else if(win->ID == 22) {
        ODO_CapInfoWindow();
    }
    #endif
    #ifdef YSI_ODOCT_ENABLE
    else if(win->ID == 20) {
        ODO_Window();
    }
    else if(win->ID == 22) {
        ODO_CapInfoWindow();
    }
    else if(win->ID == 24) {
        CT_Window();
    }
    else if(win->ID == 26) {
        if(flash.odoct.spcondUnit==0) {
            flash.odoct.spcondUnit=1;
            SetWidgetText(gv.handle[win->ID],"mS/cm");
        } else if(flash.odoct.spcondUnit==1){
            flash.odoct.spcondUnit=2;
            SetWidgetText(gv.handle[win->ID],"ppt");
        } else {
            flash.odoct.spcondUnit=0;
            SetWidgetText(gv.handle[win->ID],"uS/cm");
        }
        if(flash.odoct.spcondUnit==0) {
            strcpy(flash.Uart[USART_1]->SetupName[2],"SPCOND uS/cm");
        } else if(flash.odoct.spcondUnit==1){
            strcpy(flash.Uart[USART_1]->SetupName[2],"SPCOND mS/cm");
        } else {
            strcpy(flash.Uart[USART_1]->SetupName[2],"Salinity ppt");
        }
//        if(flash.odoct.spcondUnit==0) {
//            flash.odoct.spcondUnit=1;
//            SetWidgetText(gv.handle[win->ID],"mS/cm");
//        } else {
//            flash.odoct.spcondUnit=0;
//            SetWidgetText(gv.handle[win->ID],"uS/cm");
//        }
//        if(flash.odoct.spcondUnit==0) {
//            strcpy(flash.Uart[USART_1]->SetupName[2],"SPCOND uS/cm");
//        } else if(flash.odoct.spcondUnit==1){
//            strcpy(flash.Uart[USART_1]->SetupName[2],"SPCOND mS/cm");
//        }
    }
    #endif
    #ifdef SPECTRO_D_ENABLE
    else if(win->ID == 20) 
    { 
        Dprintf("ULIK COD CALMODE\n");
        gv.ulikCalItem=0; 
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    } 
    else if(win->ID == 22) 
    { 
        Dprintf("ULIK TOC CALMODE\n");
        gv.ulikCalItem=1; 
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    else if(win->ID == 24) 
    { 
        Dprintf("ULIK TUR STANDARD CALMODE\n");
        gv.ulikCalItem=2; 
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    else if(win->ID == 26) 
    { 
        Dprintf("ULIK COD STANDARD CALMODE\n");
        gv.ulikCalItem=3; 
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    else if(win->ID == 28) 
    { 
        Dprintf("ULIK TOC STANDARD CALMODE\n");
        gv.ulikCalItem=4; 
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    #endif
	#if defined (SAMAN_ENABLE)
    else if(win->ID == 20) {
        KeyPad(win->Parent, win->Handle,  gv.handle[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "적산산유량교정");
    }
	#endif
	#if defined (PONSEL_EC)
    else if(win->ID == 20) {
        PONSEL_EC_Window();
    }
	#endif
    #ifdef SPECTRO_M_ENABLE
    else if(win->ID == 20) {//COD
        Dprintf("ULIK MW [ COD ] CALMODE\n");
        gv.ulikCalItem=0;         
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    else if(win->ID == 22) {//TOC
        Dprintf("ULIK MW [ TOC ] CALMODE\n");
        gv.ulikCalItem=1;    
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    else if(win->ID == 24) {//NO3_N
        Dprintf("ULIK MW [ NO3 ] CALMODE\n");
        gv.ulikCalItem=2;         
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    else if(win->ID == 26) {//TUR
        Dprintf("ULIK MW [ TUR ] CALMODE\n");
        gv.ulikCalItem=3;    
        ULIKScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    #endif
    #ifdef CTD_ENABLE
    else if(win->ID == 28) {//COND
        Dprintf("COND CALMODE\n");
		gv.ulikCalItem=5;//전도도
        CONDScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    else if(win->ID == 30) {//Depth
        Dprintf("DEPTH CALMODE\n");
		gv.ulikCalItem=5;//전도도
        DepthScreen(win->Parent, win->Handle,  NULL, (1024-1000)/2, (768-730)/2, FONT16, "", "Calibration"); 
    }
    #endif
    else if(win->ID == 38) {
        flash.systemColorNo++;
        if(flash.systemColorNo>=8) {
            flash.systemColorNo=0;
        }
        SystemColorSet();
        sprintf((char *)buf,"%d", flash.systemColorNo);
        SetWidgetText(gv.handle[win->ID],(char *)buf);
    }
	else if(win->ID == 34) {
		#if defined(MODEM_POWER_CONTROL)
		//flash.wakeUpTimeModem
		if(flash.wakeUpTimeModem==0) {
			flash.wakeUpTimeModem = 30;
		}
		else if(flash.wakeUpTimeModem == 30) {
			flash.wakeUpTimeModem = 60;
		}
		else if(flash.wakeUpTimeModem == 60) {
			flash.wakeUpTimeModem = 0;
		}
        sprintf((char *)buf,"%d", flash.wakeUpTimeModem);
        SetWidgetText(gv.handle[win->ID],(char *)buf);
		#endif
	}
    else if(win->ID == 37) {//HOME
        char        data[30], fbuf[30];
        int             i = 0, addressCount = 0, count = 0;
        for( i = 0; i<30; i++) 
        {
            data[i] = 0;
            fbuf[i] = 0;
        }
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[1],fbuf);
        i = 0;
        addressCount = 0;
        for(count = 0; count<30; count++)
        {
            data[i] = fbuf[count];
            if(data[i] == '.' || data[i] == 0)
            {
                data[i] = 0;
                i=0;
                flash.serverIp[addressCount] = atoi(data);
                addressCount++;
                if(addressCount >= 5) break;
            }
            else i++;
        }
        //Dprintf("SIP = %d.%d.%d.%d\n",flash.serverIp[0],flash.serverIp[1],flash.serverIp[2],flash.serverIp[3]);
        
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[2],fbuf);			
        i = 0;
        addressCount = 0;
        for(count = 0; count<30; count++)
        {
            data[i] = fbuf[count];
            if(data[i] == '.' || data[i] == 0)
            {
                data[i] = 0;
                i=0;
                flash.serverMask[addressCount] = atoi(data);
                addressCount++;
                if(addressCount >= 5) break;
            }
            else i++;
        }
        //Dprintf("SMASK = %d.%d.%d.%d\n",flash.serverMask[0],flash.serverMask[1],flash.serverMask[2],flash.serverMask[3]);
        
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[3],fbuf);
        i = 0;
        addressCount = 0;
        for(count = 0; count<30; count++)
        {
            data[i] = fbuf[count];
            if(data[i] == '.' || data[i] == 0)
            {
                data[i] = 0;
                i=0;
                flash.serverGate[addressCount] = atoi(data);
                addressCount++;
                if(addressCount >= 5) break;
            }
            else i++;
        }
        //Dprintf("SGATE = %d.%d.%d.%d\n",flash.serverGate[0],flash.serverGate[1],flash.serverGate[2],flash.serverGate[3]);
        
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[4],fbuf);
        flash.serverPort[0] = atoi(fbuf);
        flash.serverPort[1] = flash.serverPort[0] + 1;
        
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[5],fbuf);
        i = 0;
        addressCount = 0;
        for(count = 0; count<30; count++)
        {
            data[i] = fbuf[count];
            if(data[i] == '.' || data[i] == 0)
            {
                data[i] = 0;
                i=0;
                flash.serverMac[addressCount] = atoi(data);
                addressCount++;
                if(addressCount >= 7) break;
            }
            else i++;
        }
        GetWidgetText(gv.handle[6],fbuf);//IP
        i = 0;
        addressCount = 0;
        for(count = 0; count<30; count++)
        {
            data[i] = fbuf[count];
            if(data[i] == '.' || data[i] == 0)
            {
                data[i] = 0;
                i=0;
                flash.clientIp[addressCount] = atoi(data);
                addressCount++;
                if(addressCount >= 5) break;
            }
            else i++;
        }
        
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[7],fbuf);			
        i = 0;
        addressCount = 0;
        for(count = 0; count<30; count++)
        {
            data[i] = fbuf[count];
            if(data[i] == '.' || data[i] == 0)
            {
                data[i] = 0;
                i=0;
                flash.clientMask[addressCount] = atoi(data);
                addressCount++;
                if(addressCount >= 5) break;
            }
            else i++;
        }
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[8],fbuf);			
        i = 0;
        addressCount = 0;
        for(count = 0; count<30; count++)
        {
            data[i] = fbuf[count];
            if(data[i] == '.' || data[i] == 0)
            {
                data[i] = 0;
                i=0;
                flash.clientGate[addressCount] = atoi(data);
                addressCount++;
                if(addressCount >= 5) break;
            }
            else i++;
        }
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[9],fbuf);
        flash.clientPort = atoi(fbuf);
        
        for(i=0;i<30;i++)fbuf[i]=0;
        GetWidgetText(gv.handle[10],fbuf);
        i = 0;
        addressCount = 0;
        for(count = 0; count<30; count++)
        {
            data[i] = fbuf[count];
            if(data[i] == '.' || data[i] == 0)
            {
                data[i] = 0;
                i=0;
                flash.clientMac[addressCount] = atoi(data);
                addressCount++;
                if(addressCount >= 7) break;
            }
            else i++;
        }        
        
        W5500_Init();


        //1. 자동리부팅시간
		GetWidgetText(gv.handle[12],data);
        nData = atoi(data);
        flash.autoRebootHourSv = nData;
		Dprintf("[SYSTEM] 자동리부팅 %d시\n", flash.autoRebootHourSv);
        //2. 절전모드
        //3. 공장초기화
        //4. 복원파일생성
        //5. 저장주기(분)
        //6. OPTION
		//6-1 모뎀동작주기
		#if defined(MODEM_POWER_CONTROL)
		Dprintf("[SYSTEM] 모뎀 동작 주기 %d 분\n",flash.wakeUpTimeModem);
		#endif
        Dprintf("[SYSTEM] 측정값 저장주기 = %d 분\n", flash.saveMinuteSv);
		#if defined (SAMAN_ENABLE)
		GetWidgetText(gv.handle[20],data);
        nData = atoi(data);
        if(nData != flash.pulsCountPv) {
			flash.pulsCountPv = nData;
			Dprintf("[SYSTEM] 적산유량 변경적용 %d\n",flash.pulsCountPv);
		} else {
			Dprintf("[SYSTEM] 적산유량 정상적용%d\n",flash.pulsCountPv);
		}
		#endif
		#ifdef EXO_ENABLE
		Dprintf("[SYSTEM] EXO 세정주기 = %d 초 \n", flash.EXO.WiperEnableTimeSV);
		#endif
        #if defined(TYPE_APPROVAL_ENABLE)
		PasswordInit();//비밀번호 초기화 재설정 후 진입해야 한다.
		#endif
		DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        #ifdef MANUAL_SYSTEM_PROJECT
        if(0==SensorInOutChannelConnect())		  //센서가 설정이 없는경우
        {
            SensorMain_Window(NULL, parent);
            Dprintf("SensorInOutChannelConnect\n");
        }
        else
        {
            gv.NewScreen = 0;//출력화면이 하나 인데 NEXT 버튼을 누르면 같은 화면이 중복 생성되는 에러를 막아준다.
            gv.OldScreen = 0;
            gv.tmsStatusCode[USART_1]=0;
            gv.tmsStatusCode[USART_2]=0;
            gv.tmsStatusCode[USART_3]=0;
            gv.tmsStatusCode[USART_4]=0;
            gv.NextScreenCreate=1;//첫번째 화면이 출력 된게한다. 첫번째 화면이란 USART 1의 센서가 연결되어있을때 화면이다.
            if(7 == MainScreenCreate(NULL, parent) )//7이 리턴되면 화면정보를 한번더 읽는다.화면이 순환형식으로 되어있어서 이다.
            {
                MainScreenCreate(NULL, parent);
            }
        }
        #endif
    }    
    PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);    
	return(1);
}
int SystemMain_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
    
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);//Dummy
    //DS1302TimeRead();
	gv.sleepModeTimerCntSecPv=0;//스립모드 진입타이머 초기화

    unsigned char    buf[128];
    for(int i=0; i<128; i++) {
        buf[i]=0;
    }
    sprintf((char *)buf,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
    SetWidgetText(gv.titleBarTimeId, (char *)buf);//시간출력
    
    if(gv.usbProgramUpdateFlag==1) {
        BSP_NandEraseBlock(4000);
        strcpy((char *)buf,"USBUPDAT");
        BSP_NandWritepage(4000,1,buf);
        SystemRebootGo();
    }
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int SystemMain_MouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;
	
	//마우스의 좌표를 읽을때 사용한다.
	int 		tx = LOWORD(param);
	int 		ty = HIWORD(param);	

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	Dprintf("SystemMain_MouseOnEvent::X=%4d Y=%4d\n", tx, ty);
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

	//사용자 프로그램 영역
	
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int SystemMain_MouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	//마우스의 좌표를 읽을때 사용한다.
	int 		tx = LOWORD(param);
	int 		ty = HIWORD(param);	

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	Dprintf("SystemMain_MouseOffEvent::X=%4d Y=%4d\n", tx, ty);
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//사용자 프로그램 영역
	{
		
	}
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int SystemMain_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
    Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}    	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//투명위젯을 사용하려면 Paint Event에서 할배경작업을 미리해야한다. 그림이 필요하면 그림먼저 로딩할것(*********************** 중요 *********)
	//또는 PPaintDrawControl(ON); ->Gr->Draw를 실행후 사용한다.
	
    /**
    *   LAN 설정
    */
	if(flash.systemColorNo==6) {
		Gr->SetColor(RGB565CONVERT(230,  230,  230));
	}
	else {
		Gr->SetColor(flash.systemBackGroundColor);
	}
	Gr->FillRect(0, 0, win->width-2, win->height-2);
    
    unsigned int groupSpace = 10;
    unsigned int titleBarH = 50+groupSpace;
	unsigned int buttonH = 56;
    unsigned int buttonW = 146;
    unsigned int buttonWW = 74;
    unsigned int buttonSpace = 4;

    unsigned int groupTitleBar = 60;
    unsigned int groupX = groupSpace;
    unsigned int groupY = titleBarH;
    unsigned int groupW = buttonW+buttonW + buttonSpace + buttonSpace + buttonSpace +buttonWW;
    unsigned int groupH = groupTitleBar+buttonH+4+buttonH+4+buttonH+4+buttonH+4+buttonH+4+buttonH+4+buttonH+4+buttonH+4+buttonH+4+buttonH+4 + buttonSpace;
    
    unsigned int buttonX = groupX + buttonSpace;
    unsigned int buttonY = groupY + groupTitleBar + buttonSpace;
    
    TitleBar(handle, 0, 0, LCD_X, 50, 1, "SYSTEM", clYellow, 1, RIGHT_ALIGN, 2, NULL);	
    char    dataBuf[30];
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH+4 , "LAN 설정");
    
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "S-IP",  clBlack);
    sprintf(dataBuf,"%d.%d.%d.%d",flash.serverIp[0],flash.serverIp[1],flash.serverIp[2],flash.serverIp[3]);
    gv.handle[1] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 1, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "S-SUB",  clBlack);
    sprintf(dataBuf,"%d.%d.%d.%d",flash.serverMask[0],flash.serverMask[1],flash.serverMask[2],flash.serverMask[3]);
    gv.handle[2] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 2, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "S-GATE",  clBlack);
    sprintf(dataBuf,"%d.%d.%d.%d",flash.serverGate[0],flash.serverGate[1],flash.serverGate[2],flash.serverGate[3]);
    gv.handle[3] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 3, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "S-PORT",  clBlack);
    sprintf(dataBuf,"%d", flash.serverPort[0]);
    gv.handle[4] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 4, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "S-MAC",  clBlack);
    sprintf(dataBuf,"%d.%d.%d.%d.%d.%d",flash.serverMac[0],flash.serverMac[1],flash.serverMac[2],flash.serverMac[3],flash.serverMac[4],flash.serverMac[5]);
    gv.handle[5] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 5, SystemMain_KeyEvent);   

    
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "C-IP",  clBlack);
    sprintf(dataBuf,"%d.%d.%d.%d",flash.clientIp[0],flash.clientIp[1],flash.clientIp[2],flash.clientIp[3]);
    gv.handle[6] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 6, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "C-SUB",  clBlack);
    sprintf(dataBuf,"%d.%d.%d.%d",flash.clientMask[0],flash.clientMask[1],flash.clientMask[2],flash.clientMask[3]);
    gv.handle[7] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 7, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "C-GATE",  clBlack);
    sprintf(dataBuf,"%d.%d.%d.%d",flash.clientGate[0],flash.clientGate[1],flash.clientGate[2],flash.clientGate[3]);
    gv.handle[8] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 8, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "C-PORT",  clBlack);
    sprintf(dataBuf,"%d", flash.clientPort);
    gv.handle[9] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 9, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "C-MAC",  clBlack);
    sprintf(dataBuf,"%d.%d.%d.%d.%d.%d",flash.clientMac[0],flash.clientMac[1],flash.clientMac[2],flash.clientMac[3],flash.clientMac[4],flash.clientMac[5]);
    gv.handle[10] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW + buttonWW, buttonH, 0, dataBuf,  PUSH_BUTTON, 10, SystemMain_KeyEvent);   

    /**
    *   하드웨어설정
    */
    groupSpace = 10;
    titleBarH = 50+groupSpace;
	buttonH = buttonH;
    buttonW = 146;
    buttonSpace = 4;

    groupTitleBar = 60;
    groupY = titleBarH;
    groupX = groupW + groupSpace + groupSpace;
    groupW = buttonW+buttonW + buttonSpace + buttonSpace + buttonSpace;
    groupH = groupTitleBar+buttonH+4+buttonH+4+buttonH+4+buttonH+4+ buttonSpace;
    
    buttonX = groupX + buttonSpace;
    buttonY = groupY + groupTitleBar + buttonSpace;
    
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH , "하드웨어");
    
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "시간",  clBlack);
    gv.handle[11] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 11, SystemMain_KeyEvent);   
    
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "자동리부팅(시)",  clBlack);
    sprintf(dataBuf,"%d", flash.autoRebootHourSv);
    gv.handle[12] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 12, SystemMain_KeyEvent);   
 
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "절전모드",  clBlack);
    sprintf(dataBuf,"%d", flash.autoRebootHourSv);
    gv.handle[13] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 13, SystemMain_KeyEvent);   
    if(flash.sleepModeFlag==1) {
		SetWidgetText(gv.handle[13],"정지");
		SetWidgetStatus(gv.handle[13],1);
	} else {
		SetWidgetText(gv.handle[13],"실행");
		SetWidgetStatus(gv.handle[13],0);
	}
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "업데이트",  clBlack);    
    gv.handle[14] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 14, SystemMain_KeyEvent);   

    groupSpace = 10;
	buttonH = buttonH;
    buttonW = 146;
    buttonSpace = 4;

    groupTitleBar = 60;
    groupY = titleBarH + groupH;
    groupX = groupW + groupSpace + groupSpace + 74;
    unsigned int next1groupX = groupX;
    groupW = buttonW+buttonW + buttonSpace + buttonSpace + buttonSpace;
    groupH = groupTitleBar+buttonH+4+buttonH+4+buttonH+4+buttonH+4+ buttonSpace+4+ buttonH;
    
    buttonX = groupX + buttonSpace;
    buttonY = groupY + groupTitleBar + buttonSpace;
    
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH , "초기화");

    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "공장초기화",  clBlack);
    gv.handle[15] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 15, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "복원파일생성",  clBlack);
    gv.handle[17] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 17, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "소수점자릿수",  clBlack);
    gv.handle[18] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 18, SystemMain_KeyEvent);   

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "저장주기(분)",  clBlack);
    sprintf(dataBuf,"%d",flash.saveMinuteSv);
    gv.handle[19] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 19, SystemMain_KeyEvent);   
    
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "COLOR",  clBlack);
    sprintf(dataBuf,"%d",flash.systemColorNo);
    gv.handle[38] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 38, SystemMain_KeyEvent);   
    
    
    /*************
    *   OPTION
    */
	buttonH = 48;
    groupSpace = 10;
    titleBarH = 50+groupSpace;
	buttonH = buttonH;
    buttonW = 146;
    buttonSpace = 4;

    groupTitleBar = 60;
    groupY = titleBarH;
    groupX = next1groupX + groupW + groupSpace;
    groupW = buttonW+buttonW + buttonSpace + buttonSpace + buttonSpace;
    groupH = groupTitleBar+buttonH+4+buttonH+4+buttonH+4+buttonH+4+ buttonSpace +4+buttonH +4+buttonH +4+buttonH +4+buttonH +4+buttonH+4+buttonH+4+buttonH;
    
    buttonX = groupX + buttonSpace;
    buttonY = groupY + groupTitleBar + buttonSpace;

#if defined (TYPE_APPROVAL_ENABLE)    
    GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH + 32 , "측정상수코드/교정");
#else	
	GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH + 32 , "OPTION");
#endif
    
#ifdef EXO_ENABLE	 
#if defined (TYPE_APPROVAL_ENABLE)    
	//EXO 교정함수	
	Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "EXO",  clBlack);
	gv.handle[20] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "교정실행",  PUSH_BUTTON, 20, SystemMain_KeyEvent);	
    
	buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "EC1.UNIT",  clBlack);
	if(flash.EXO.SpcondmgmsEnable[USART_1] == 1) {
		strcpy(dataBuf,"mS/cm");
	}
	else {
		strcpy(dataBuf,"uS/cm");
	}    
	gv.handle[22] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 22, SystemMain_KeyEvent);	

	#if defined (RTU_VER12_ENABLE)
	
	buttonY = buttonY + buttonH + buttonSpace;
	Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "pH 측정상수",  clBlack);	
	gv.handle[24] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 24, SystemMain_KeyEvent);	

	buttonY = buttonY + buttonH + buttonSpace;
	Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "DO 측정상수",  clBlack);	
	gv.handle[26] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 26, SystemMain_KeyEvent);	

	#endif
	buttonY = buttonY + buttonH + buttonSpace;
	Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "TMS 항목",  clBlack);	
	gv.handle[28] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "코드변경",  PUSH_BUTTON, 28, SystemMain_KeyEvent);	
#else
	Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "EXO",  clBlack);
	gv.handle[20] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 20, SystemMain_KeyEvent);	
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "EC1.UNIT",  clBlack);
	if(flash.EXO.SpcondmgmsEnable[USART_1] == 1) {
		strcpy(dataBuf,"mS/cm");
	}
	else {
		strcpy(dataBuf,"uS/cm");
	}    
	gv.handle[22] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 22, SystemMain_KeyEvent);	
    if(flash.Uart[USART_2]->SensorID == SENSOR_EXO) {
        buttonY = buttonY + buttonH + buttonSpace;
        Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "EC2.UNIT",  clBlack);
        if(flash.EXO.SpcondmgmsEnable[USART_2] == 1) {
            strcpy(dataBuf,"mS/cm");
        }
        else {
            strcpy(dataBuf,"uS/cm");
        }
        
        gv.handle[24] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 24, SystemMain_KeyEvent);	
    }
#endif
#endif
#ifdef YSI_ODD_ENABLE
//	GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH + 32 , "OPTION");

    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "ODO CAL",  clBlack);
	gv.handle[20] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 20, SystemMain_KeyEvent);	

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "ODO CAP",  clBlack);
	gv.handle[22] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 22, SystemMain_KeyEvent);	
//    buttonY = buttonY + buttonH + buttonSpace;
//    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "pH",  clBlack);
//	gv.handle[28] = NButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 28, SystemMain_KeyEvent);	

//    buttonY = buttonY + buttonH + buttonSpace;
//    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "최대채널수",  clBlack);
//    sprintf(dataBuf,"%d",flash.odoct.odo_MaxCh);
//	gv.handle[30] = NButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 30, SystemMain_KeyEvent);	

#endif
#ifdef YSI_ODOCT_ENABLE
//	GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH + 32 , "OPTION");

    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "ODO CAL",  clBlack);
	gv.handle[20] = NButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 20, SystemMain_KeyEvent);	

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "ODO CAP",  clBlack);
	gv.handle[22] = NButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 22, SystemMain_KeyEvent);	

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "COND CAL",  clBlack);
	gv.handle[24] = NButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 24, SystemMain_KeyEvent);	

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "COND UNIT",  clBlack);
    if(flash.odoct.spcondUnit==0) {
        strcpy(dataBuf,"uS/cm");
    } else if(flash.odoct.spcondUnit==1){
        strcpy(dataBuf,"mS/cm");
    } else {
        strcpy(dataBuf,"ppt");
    }
	gv.handle[26] = NButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 26, SystemMain_KeyEvent);	
#endif
#ifdef SPECTRO_D_ENABLE
//	GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH + 32 , "OPTION");

    Label(handle, buttonX, buttonY+buttonSpace, buttonW+buttonW+buttonSpace, buttonH, "SAMPLE WATER 교정",  clBlack);
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "COD",  clBlack);
	gv.handle[20] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 20, SystemMain_KeyEvent);	
	
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "TOC",  clBlack);
	gv.handle[22] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 22, SystemMain_KeyEvent);	

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW+buttonW+buttonSpace, buttonH, "STANDARD BUFFER 교정",  clBlack);
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "TUR",  clBlack);
	gv.handle[24] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 24, SystemMain_KeyEvent);	
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "COD_KHP",  clBlack);
	gv.handle[26] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 26, SystemMain_KeyEvent);	
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "TOCstd",  clBlack);
	gv.handle[28] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 28, SystemMain_KeyEvent);	
#endif
#ifdef SPECTRO_M_ENABLE
//	GroupPanel(handle, groupX, groupY+buttonSpace, groupW, groupH + 32 , "OPTION");
	
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "COD",  clBlack);
	gv.handle[20] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "교정실행",  PUSH_BUTTON, 20, SystemMain_KeyEvent);	
	
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "TOC",  clBlack);
	gv.handle[22] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "교정실행",  PUSH_BUTTON, 22, SystemMain_KeyEvent);	

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "NO3",  clBlack);
	gv.handle[24] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "교정실행",  PUSH_BUTTON, 24, SystemMain_KeyEvent);	

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "TUR",  clBlack);
	gv.handle[26] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "교정실행",  PUSH_BUTTON, 26, SystemMain_KeyEvent);	
#ifdef CTD_ENABLE
    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "COND",  clBlack);
	gv.handle[28] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "교정실행",  PUSH_BUTTON, 28, SystemMain_KeyEvent);	

    buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "DEPTH",  clBlack);
	gv.handle[30] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "교정실행",  PUSH_BUTTON, 30, SystemMain_KeyEvent);	
	
	buttonY = buttonY + buttonH + buttonSpace;
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "모뎀주기.분",  clBlack);
	sprintf(dataBuf,"%d", flash.wakeUpTimeModem);
	gv.handle[34] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 34, SystemMain_KeyEvent);	

#endif

#endif
#if defined (SAMAN_ENABLE)
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "적산유량교정",  clBlack);
	sprintf(dataBuf,"%d",flash.pulsCountPv);
	gv.handle[20] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, dataBuf,  PUSH_BUTTON, 20, SystemMain_KeyEvent);	
#endif
#if defined (PONSEL_EC)    
    Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "전도도교정",  clBlack);	
	gv.handle[20] = SButton(handle, buttonX+buttonW+buttonSpace, buttonY+buttonSpace, buttonW, buttonH, 0, "실행",  PUSH_BUTTON, 20, SystemMain_KeyEvent);	
#endif
	if(flash.systemColorNo!=6) {
		NButton(handle, 0+180+170+170+170+170, LCD_Y-65, 164, 64, "48G_Home_Icon2.bmp", "HOME",  PUSH_BUTTON, 37, SystemMain_KeyEvent);
	}
	else {
		SButton(handle, 0+180+170+170+170+170, LCD_Y-65, 164, 64, "48G_Home_Icon2.bmp", "HOME",  PUSH_BUTTON, 37, SystemMain_KeyEvent);
	}

	TitleBar_MenuButton_Create(handle,11,SystemMain_KeyEvent);
    
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    play_timer =  OpenTimer(parent, handle, 1000);	
    StartTimer(play_timer);  	
	return(1);
}

int SystemMain_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	//static int start=0;
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
    /*
	//사용자 프로그램 영역
	if(start==0) //메인윈도우 같이 한번생성후 계속사용할때, 1회만 처리할내용
	{
		start=1;
	}	
	Gr->SetColor(clBlack);
	Gr->FillRect(0, 0, LCD_X, LCD_Y);
    */
	//SetFontAliasing("ng128_e.sif");	
	//TextOutAliasing(10, 100, "-1.234", clWhite, 0, "ng128_e.sif");		

	
	//타이머이벤트등에서  배경을 갱신하기위한 사용법, 호출시에 commad 에 값을넣고 호출한다.
	//콘트롤이나 체인지 이벤트를 사용하면 더 깔끔할수도 있다.
	if(command==1)
	{
	}
	else if(command==2)
	{
	}	
	PaintDrawControl(ON);		
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int SystemMain_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	//윈도우가 닫힐때 필요한처리를 한다.
	Windows *win=NULL;

	switch(handle) //컨테이너윈도우 
	{
		case 1: 	win = gp_WidgetWindow_1;	break;
		case 2: 	win = gp_WidgetWindow_2;	break;
		case 3: 	win = gp_WidgetWindow_3;	break;
		case 4: 	win = gp_WidgetWindow_4;	break;
		case 5: 	win = gp_WidgetWindow_5;	break;
		case 6: 	win = gp_LcdAlphaWidgetWindow;	break; //
		case 7: 	win = gp_VideoAlphaWidgetWindow;	break; //
		default:	Dprintf("FindWidgetChain::Windows_no mistyping\n"); return(0);
	}

	if(win== NULL) 
	{
		Dprintf("TabelTest_CloseEvent::Invalid Handle\n");
		return (NULL);
	}
	// 이윈도우에서 malloc 사용시 여기서 free 해주어야한다.
	
	int Done=1;
	while(Done) //위젯윈도우를 모두찾아 CloseEvent를 전송한다.
	{
		if(win->CloseEvent != NULL) win->CloseEvent(0, 0, 0, 0, 0);
		if(win->NextWindow ==NULL) return(NULL);
		win = (Windows *)win->NextWindow;
	}	
	return(1);
}

#define		WINDOW_WIDTH		LCD_X
#define		WINDOW_HEIGHT		LCD_Y
// 이윈도우의 기능이 많을경우 콜백함수를 여러게 등록해서 사용하면 작업을 분산시킬수 있다.
// 백그라운드 윈도우로 동작하고 이벤트나 데이터 관리후 필요에따라 전달할경우에 사용한다.
// GUI 없이도 태스킹 프로그램에 적용하는것을 고려해서 의논할것
int SystemMain_Window(HANDLE parent, HANDLE handle) {
	if(GetWindowName("SystemMain_Window")) 
	{
		Dprintf("SystemMain_Window:: Windows already Create\n");
		return 0;		
	}
	makeWindow(
				LCD,							// Display Section Position, 이윈도우가 어디에 표시되는가?
				"SystemMain_Window",			// Windows Name, 이이름으로 이윈도우의 핸들을 찾을수 있다. 64자
				DESKTOP_WINDOW,					// Parent Window Handle
				CONTAINER_WINDOW,				// Windows Kind
				STYLE_NONE,						// Windows Style, TEECO_STYLE, XP_STYLE 두가지가 기본준비되어있다.
				0, 0, 							// Start Position
				WINDOW_WIDTH, WINDOW_HEIGHT,	// Size
				SHOW,							// Create after Show
				SystemMain_CreateEvent,			// CreateEvent
				SystemMain_PaintEvent,			// PaintEvent
				NULL,							// MouseOnEvent
				NULL,							// MouseOffEvent
				NULL,							// MouseMoveEvent
				NULL,							// GestureEvent
				NULL,							// ClickEvent, 콜백함수를 등록한다.(콜백함수는 어떤것도 등록가능하다)
				SystemMain_TimerEvent,			// TimerEvent
				NULL,							// ControlEvent, 이윈도우를 제어하기위해 사용된다.
				NULL,							// ChangeEvent, 윈도우의 속성값이 
				NULL,							// KeyEvent
				NULL, 							// EncoderEvent, 시스템에 엔코더가 연결되오 있을때 발생된다.
				NULL, 							// FocusEvent,  마우스를 사용할때 커서가 위에 있으면 발생된다.
				SystemMain_CloseEvent,			// Close Event, 윈도우가 닫힐때 발생한다.
				NULL,							// PARAMETER, window->Param에 저장된다.
				NULL,							// NormalName용 BMP 파일명, window->NormalName, 
				NULL,							// TouchName용 BMP파일명, window->TouchName
				NULL,							// Windows Text, win->Text 에 저장된다. 128Byte의 사이즈를 가지고 있다. 초과되지 않도록 한다.
				MODALLESS_WINDOW,	// window->Mode에 저장된다.
				NULL					// WidgetStruct 구조체이다. 속성이 모자랄때 추가하기위해 사용한다.
				);
    if(parent==NULL && handle==NULL)//백그라운드 윈도우를 삭제한다.
    {
        Dprintf(" SystemMain_Window :: MainWindows Create \n");
    }
    else 
    {
        //Dprintf("CloseWindows parent = %d handle = %d\n", parent, handle);
        CloseWindow(parent, handle);	
    }
    return(1);
}				

#ifdef __cplusplus
}
#endif
