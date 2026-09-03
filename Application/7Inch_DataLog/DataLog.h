#ifndef __DATALOG_HEADER
#define __DATALOG_HEADER

#ifdef __cplusplus
 extern "C" {
#endif
#include "Teeco_HW_Config.h"
#include "stm32f4xx.h"	 
#include "DataLogDefine.h"
#include "DataLogExtern.h"
     
#define		MEMORY_BUF_SIZE 4096
/*****************************************************************************
     
     시스템 실행 명령
     
*****************************************************************************/     
#define   DATALOG_8INCH
#define	  AUTO_SCREEN_TIMER           10 //자동으로 화면이 전환 되는 시간 10초    
#define   SDREAD_RAM                  0xC0000000//0x6A280000 //USER_SCREEN5_ADDR//0xC0000000
#define   MANUAL_SYSTEM_PROJECT     
#define   CONNECTION_WAITING_TIME     20 //600SEC 접속대기시간 600SEC 이후 TCP/IP 접속해지     

//금호마린테크	 
//#define YSI_ODOCT_ENABLE    //ODOCT
//#define MODBUS_ENABLE

//배혁 수자원공사
//#define YSI_ENABLE	 
//#define MODBUS_ENABLE
	 
//거제도 20221113 권순환
//#define PONSEL_EC	 
	 
//삼안 엔드레스하우저, 4~20mA 입력 2채널, 
//#define SAMAN_ENABLE 

//ODO는 SENSOR메뉴에서  ODO를 선택하면 ODOCT 도 같이 적용된다.
//#define YSI_ODD_ENABLE      //ODO     
//#define YSI_ODOCT_ENABLE    //ODOCT

//대청호수질관리소 EXO 2채널
//#define DAECHEONGHO_SUGILGWANLISO 
//#define MODBUS_ENABLE        
//#define EXO_ENABLE
//#define _Ammoninum //적용한다.

//형식승인시작===================================================================================
#define RTUV3_ENABLE //<RTUV3.0>
//#define RTU_VER12_ENABLE

#define EXO_ENABLE
#define _Ammoninum //선택적용한다.
//#define _FDOM_RFU
//#define _FDOM_QSU

#define KECO_PROTOCOL_ENABLE //환경공단 프로토콜
#define TYPE_APPROVAL_ENABLE //형식승인 TMS
//형식승인종료===================================================================================

//#define JSON_ENABLE
//#define SPECTRO_D_ENABLE

///ULIK + CTD + BATTERY
//#define SPECTRO_M_ENABLE
//#define CTD_ENABLE
//#define BATTERY_ENABLE
//#define ULIK_CTD_ENABLE //2개의 센서가 통합된 모델이다.
//#define MODEM_POWER_CONTROL


extern  U32 KIM_RAM_ADDRESS;
extern  BYTE Disk_Buffer[MEMORY_BUF_SIZE];
#ifdef  JSON_ENABLE
extern  U32 JSON_RAM_ADDRESS;
#endif
/*********************************************************************************************

    시간이 저장되는 구조체

*********************************************************************************************/
typedef struct __Rtime {
	int		year;
	int		month;
	int		date;
	int		hour;
	int		minute;
	int		sec;
} RealTime;
extern RealTime currentDate;
/*********************************************************************************************

    JSON

*********************************************************************************************/
#ifdef JSON_ENABLE
typedef struct __Json {         //Flash에 저장된다.
	int groupSv;				//지역
	int siteSv;				    //저수지
	int diviceSv;				//RTU ID
	char deviceVersionSv[32];    
} JsonFlashVal;

typedef struct __JsonGlobalVal {//전역변수로 활용된다.
	char cmdList[50][20];	//명령어
	char valueList[50][20];		//제어값
	int commandCode;				//명령코드
	int groupPv;				//지역
	int sitePv;				//저수지
	int divicePv;				//RTU ID
	char deviceVersionPv[32];
	char serverIpPv[32];	
	int serverPortPv;
	char clientIpPv[32];
	int clientPortPv;
    int rxByteSize;
    int nextSendCmdNo;
    int sendingSize;        //측정값 송신크기
    int sendingStep;        //측정값 송신스텝
    int sendingCount;     //측정값 송신횟수 (1000바이트씩송신)
    int oneSendingSizeCount;
    unsigned int firmwareSize;    
} JsonGlobalVal;
#endif
/*********************************************************************************************

    ULIK SENSOR

*********************************************************************************************/
#if defined(SPECTRO_D_ENABLE) || defined(SPECTRO_M_ENABLE)
typedef struct __ULIK {
    int id;//device id
    int cleanTimeSv;//세정주기설정 minute    
} UlikFlash;
#endif
/*********************************************************************************************

    ULIK SENSOR

*********************************************************************************************/
#if defined(CTD_ENABLE)
typedef struct __CTD {
    int id;//device id
    int calCount;//cond 교정횟수
    int depthCalCount;//depth 교정횟수
    float condParaValue1[6];//교정값을 저장한다.
    float condParaValue2[6];//교정값을 저장한다.
    float depthParaValue1[6];//교정값을 저장한다.
    float depthParaValue2[6];//교정값을 저장한다.
} CTD;
#endif
/*********************************************************************************************

    PONSEL EC

*********************************************************************************************/
#if defined(PONSEL_EC)
typedef struct _sn_ponsel_ec
{
    volatile unsigned char modbusAddress[5];
    volatile unsigned int   channel;
    volatile float temperaturSV[5];
    volatile float calSV[5];
    volatile float PV;    
    volatile int calStartFlag;
    volatile int calReceiveFlag;
    volatile int calStep;
    volatile int factory;
    volatile int factoryCount;
    volatile int clearCountSV;
    volatile int clearCountPV;
    volatile int clearWaitCountPV;
    volatile int clearWaitCountSV;
    volatile int clearWaitCountFlag;
	
	//온도팩터설정
	volatile float tempFactor[8];
	volatile float tempXy[8];
	volatile float tempSetPv[8];
}__sn_ponsel_ec;
#endif
/*********************************************************************************************

    ODO CT SENSOR

*********************************************************************************************/
#if defined( YSI_ODD_ENABLE )||defined(YSI_ODOCT_ENABLE)
typedef struct __ODOCT {
    float barometerPv;//mmHg
    float salPv;//염분
    float odoCalPv;//교정값
    
    float condCalPv;//전도도교정값
    float saltCalPv;//염분교정값
    float tempCalPv;//전도도온도보상
    
    int spcondUnit;//0 : uS / 1 : mS
    
    int odo_MaxCh;
} ODOCT;
#endif
/*********************************************************************************************

    Input(4~20mA) 관련 자료구조

*********************************************************************************************/
#define     INPUT_MAX_CHANNEL   8
typedef struct __Input
{
	char				Name[24];
	volatile float	    CalLowmV;
	volatile float	    CalHighmV;
	volatile float	    CalLowLimit;
	volatile float	    CalHighLimit;
	volatile int		SensorStatus;
    volatile float      mV;
	volatile float	    PV;
}__InputStruct[INPUT_MAX_CHANNEL];
/*********************************************************************************************

EXO 신형

*********************************************************************************************/
#ifdef EXO_ENABLE
#define EXO_DATA_REQUEST_PORT 3
#define EXO_MAX_CH 3
#define EXO_MAX_ITEM 70
typedef struct _EXO {
	volatile float	            ECCalbration_Buffer[EXO_MAX_CH];	
	float	                    PV[EXO_MAX_CH];//현재값
	float	                    SV[EXO_MAX_CH];//진해값 
	float	                    temp[EXO_MAX_CH];//전도도가 연결되면 현재온도
	volatile int	            CalPoint;//보정 포인트
	volatile int	            CalStart[EXO_MAX_CH];//보정시작 FLAG
	volatile int	            CalStartStep[EXO_MAX_CH];//보정명령 그룹선택
	volatile int	            CalCommand;//보정명령 그룹에서 스텝실행 명령
	volatile unsigned char      CalHexaItemCode[EXO_MAX_CH];//보정시 아이템 id가저장이 된다.
	volatile int	            CalItem;//보정항목을 선택할때 사용하는 변수이다.
	//volatile unsigned int	DeviceId[3];
	//volatile unsigned int	SensorPortName[10];
	int	    UartPort;//CONFIG 화면에서 사용되는 UART포트를 저장한다.
	//volatile unsigned int	SelectItemFlag[3][20];
	//volatile float				Value[20];
	//volatile unsigned int	SensorPortNumber[10];
	volatile unsigned char	    EXO_CalAddress;
	volatile unsigned char	    EXO_Address[EXO_MAX_CH][4];// EXO 센서 ADDRESS 이다.
	volatile float              CalSaltPV[EXO_MAX_CH];
	//volatile unsigned int	EXO_Sensor_SerialNumber[3][20];//사용자가 설정한 센서 시리얼넘버로 센서에서 올라온값과 비교하여 센서정보를 알아낸다
	volatile float				EXO_Data[EXO_MAX_CH][EXO_MAX_ITEM];//측정치 최대 70여개 수신된다.
	volatile unsigned char	    SensorAddress[EXO_MAX_CH][8][4];//센서 어드레스 정보가 저장된다.
	volatile unsigned char	    imsiSensorAddress[EXO_MAX_CH][8][4];//센서 어드레스 정보가 저장된다.
	volatile int				item;//통신으로 수신한 측정치를 구분할때 아이템이 사용된다.
	volatile int				itemAdd;//아이템수에 따라 측정포인트를 구한다.
	volatile int				DataListCount[EXO_MAX_CH];//수신데이터 최대길이
	
    volatile int				Sensor_PH[EXO_MAX_CH][1];		//포트넘버 
	volatile int				Sensor_ORP[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_ODO[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_COND[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_TUB[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_SOND[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_CHLIB[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_DEPTH[EXO_MAX_CH][1];//포트넘버
	volatile int				Sensor_WIPER[EXO_MAX_CH][1];//포트넘버
	volatile int				Sensor_FDOM[EXO_MAX_CH][1];//포트넘버
    volatile int                Sensor_Ammoninum[EXO_MAX_CH][1]; //포트넘버	

	volatile int				Sensor_PH_Status[EXO_MAX_CH][1];		//포트넘버 
	volatile int				Sensor_PHMV_Status[EXO_MAX_CH][1];		//포트넘버 
	volatile int				Sensor_ORP_Status[EXO_MAX_CH][1];		//포트넘버 
	volatile int				Sensor_ODO_SAT_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_ODO_MGL_Status[EXO_MAX_CH][1];	//포트넘버
	//volatile int				Sensor_COND_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_COND_SP_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_COND_SAL_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_COND_TEMP_Status[EXO_MAX_CH][1];
	volatile int				Sensor_TUB_FNU_Status[EXO_MAX_CH][1];	//포트넘버
//	volatile int				Sensor_TUB_RAW_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_TUB_NTU_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_CHLIB_UGL_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_CHLIB_RFU_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_BGA_PCRFU_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_BGA_PCUGL_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_BGA_PERFU_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_BGA_PEUGL_Status[EXO_MAX_CH][1];	//포트넘버
	volatile int				Sensor_DEPTH_Status[EXO_MAX_CH][1];//포트넘버
    volatile int				Sensor_Ammoninum_Status[EXO_MAX_CH][1];      //포트넘버
	
	volatile int				Sensor_TSS_MGL_Status[EXO_MAX_CH][1];//포트넘버
	volatile int				Sensor_FDOM_RFU_Status[EXO_MAX_CH][1];//포트넘버
	volatile int				Sensor_FDOM_QSU_Status[EXO_MAX_CH][1];//포트넘버
	volatile int				Sensor_CABLEPOWER_Status[EXO_MAX_CH][1];//포트넘버
    volatile int                Sensor_Battery_Status[EXO_MAX_CH][1];

	volatile int				WiperEnableTimeSV;
	volatile int				Sensor_PORT[EXO_MAX_CH];//현재측정중인포트
	
	volatile float				CalTemp[EXO_MAX_CH];
	volatile int 				SpcondmgmsEnable[EXO_MAX_CH];
	volatile int 				exoCalMode;
}__EXO;
#endif
/*********************************************************************************************

Output 관련 자료구조

*********************************************************************************************/
#define OUTPUT_MAX_CHANNEL  3
typedef struct __Output {
	char			    Name[16];
	volatile float	    CalLowLimit;
	volatile float	    CalHighLimit;
	volatile float	    PV;
}__OutputStruct[OUTPUT_MAX_CHANNEL];
/*********************************************************************************************

Sensor 관련 자료구조

*********************************************************************************************/
//#define USART_SENSOR_MAX    30
#define USART_SENSOR_MAX    15
#define USART_MAX   5
typedef struct __Uart {			                		// UART
	char			    SetupName[USART_SENSOR_MAX][24];//항목별이름
	volatile int		SensorID;                   	//어떤센서가 물려 있는가.					
	volatile int		Rate;			                //통신속도 
	volatile int		SensorStatus;		        	//접속상태 를 체크하여 데이터 출력 여부를 결정한다.
	volatile int		ItemCnt;			            //측정항목수 
	volatile int		UartOr485Select;				//0 RS232 1 RS485
	volatile float		SensorPV[USART_SENSOR_MAX];		//센서로들오는 값 
}__SensorStruct[USART_MAX];
/*********************************************************************************************

FLASH 자료구조

*********************************************************************************************/
#define FLOAT_POINT_MAX 24
typedef struct __smartLog
{
    volatile int StartVerify;
    volatile int systemStatus;//공장초기화가 되어 있는가?
	volatile int ScreenCnt[6];
	volatile int sleepModeFlag;//슬립모드 적용여부를 판단한다.
	volatile int powerControl;
    volatile int relay[10];
    volatile int connection[10];
    volatile int autoRebootCount; //자동리부팅횟수
    volatile int autoRebootHourSv;//설정된 시간이 되면 리부팅한다.
    
    int systemColorNo;
    int systemColor;
    int systemFontColor;
    int systemGroupPanelColor;
    int systemSetupPanelFontColor;
    int systemBackGroundColor;
    
    int serverMac[10];
    int serverIp[10];
    int serverGate[10];
    int serverMask[10];
    int serverPort[2];

    int clientMac[10];
    int clientIp[10];
    int clientGate[10];
    int clientMask[10];
    int clientPort;
    
	int Uart5Or485Select;
	int Uart5ItemSelect;	
	int Uart6Or485Select;
	int Uart6ItemSelect;
	int Uart7Or485Select;
	int Uart7ItemSelect;
	int Uart8Or485Select;
	int Uart8ItemSelect;
	
	int onlyOneSaveFlag;
    int saveMinute;
    int dumpTimeSaveFlag;
    int gDataSave5MinFlag;
    int gDataSave60MinFlag;
    int Save60TimeSaveFlag;
    
    int lcdInitFlag;
    char floatPoint[FLOAT_POINT_MAX];
    int saveMinuteSv;//저장주기 설정값 


	__SensorStruct Uart[USART_MAX];
	__InputStruct Input[INPUT_MAX_CHANNEL];
	__OutputStruct Output[OUTPUT_MAX_CHANNEL];

    RealTime saveDate;//5분데이터 저장시간
    RealTime dumpNewDate;//최근 덤프데이터 송신시간
    RealTime save60Date;//60분데이터 저장시간
    
    #ifdef YSI_ENABLE
 	volatile int	nFlag_SelectItem_Temp[5];
	volatile int	nSelectItem_Temp_Unit[5];
	
	volatile int	nFlag_SelectItem_SpCond[5];
	volatile int	nFlag_SelectItem_SpCond_Unit[5];
	
	volatile int	nFlag_SelectItem_Cond[5];
	volatile int	nFlag_SelectItem_Cond_Unit[5];
	
	volatile int	nFlag_SelectItem_Resist[5];
	volatile int	nFlag_SelectItem_Resist_Unit[5];
	
	volatile int	nFlag_SelectItem_TDS[5];
	volatile int	nFlag_SelectItem_TDS_Unit[5];
	
	volatile int	nFlag_SelectItem_Sal[5];
	volatile int	nFlag_SelectItem_Sal_Unit[5];
	
	volatile int	nFlag_SelectItem_DOSat[5];
	volatile int	nFlag_SelectItem_DOSat_Unit[5];
	
	volatile int	nFlag_SelectItem_DO[5];
	volatile int	nFlag_SelectItem_DO_Unit[5];
	
	volatile int	nFlag_SelectItem_DOChrg[5];
	volatile int	nFlag_SelectItem_DOChrg_Unit[5];
	
	volatile int	nFlag_SelectItem_DOSatLocal[5];
	volatile int	nFlag_SelectItem_DOSatLocal_Unit[5];

	volatile int	nFlag_SelectItem_PressAbs[5];
	volatile int	nSelectItem_PressAbs_Unit[5];

	volatile int	nFlag_SelectItem_PressPsir[5];
	volatile int	nSelectItem_PressPsir_Unit[5];

	volatile int	nFlag_SelectItem_PressGau[5];
	volatile int	nSelectItem_PressGau_Unit[5];

	volatile int	nFlag_SelectItem_PressPsi[5];
	volatile int	nSelectItem_PressPsi_Unit[5];
	
	volatile int	nFlag_SelectItem_Depth[5];
	volatile int    nSelectItem_Depth_Unit[5];
	
	volatile int	nFlag_SelectItem_pH[5];
	volatile int	nFlag_SelectItem_pH_Unit[5];
	
	volatile int 	nFlag_SelectItem_pHmV[5];

	volatile int 	nFlag_SelectItem_ORP[5];
	
	volatile int	nFlag_SelectItem_Ammonium[5];
	volatile int	nFlag_SelectItem_Ammonium_Uint[5];

	volatile int	nFlag_SelectItem_Ammonia[5];
	volatile int	nFlag_SelectItem_Ammonia_Uint[5];

	volatile int	nFlag_SelectItem_Nitrate[5];
	volatile int	nFlag_SelectItem_Nitrate_Uint[5];

	volatile int	nFlag_SelectItem_Chloride[5];
	volatile int	nFlag_SelectItem_Chloride_Uint[5];

	volatile int	nFlag_SelectItem_PAR1[5];
	volatile int	nFlag_SelectItem_PAR1_Uint[5];

	volatile int	nFlag_SelectItem_PAR2[5];
	volatile int	nFlag_SelectItem_PAR2_Uint[5];

	volatile int	nFlag_SelectItem_Turbidity6026[5];
	volatile int	nFlag_SelectItem_Turbidity6026_Uint[5];
	
	volatile int	nFlag_SelectItem_Chlorophyll[5];
	volatile int	nFlag_SelectItem_Chlorophyll_Uint[5];

	volatile int	nFlag_SelectItem_Fluorescence[5];
	volatile int	nFlag_SelectItem_Fluorescence_Uint[5];


	volatile int	nFlag_SelectItem_Rhodamine[5];
	volatile int	nFlag_SelectItem_Rhodamine_Uint[5];

	volatile int	nFlag_SelectItem_Turbidity6136[5];
	volatile int	nFlag_SelectItem_Turbidity6136_Uint[5];

	volatile int 	nFlag_SelectItem_RhodaminWT[5];

	volatile int	nFlag_SelectItem_ODOSet[5];

	volatile int	nFlag_SelectItem_ODO[5];
	
	volatile int	nFlag_SelectItem_BGAPC[5];
	volatile int	nFlag_SelectItem_BGAPC_Uint[5];


	volatile int	nFlag_SelectItem_BGAPE[5];
	volatile int	nFlag_SelectItem_BGAPE_Uint[5];

	volatile int	nFlag_SelectItem_BGAPCRFU[5];
	volatile int	nFlag_SelectItem_BGAPCRFU_Uint[5];

	volatile int	nFlag_SelectItem_CHL[5];
	volatile int	nFlag_SelectItem_CHL_Uint[5];

	volatile int	nFlag_SelectItem_Bat[5];
	
	volatile int	nFlag_SelectItem_Ground[5];

	volatile int	nFlag_SelectItem_FullScale[5];
	
	volatile int 	ysiSensorNameChange;
    #endif	

    #ifdef EXO_ENABLE
	__EXO EXO;
    #endif
    #ifdef JSON_ENABLE
    JsonFlashVal json; 
    #endif
    #if defined(SPECTRO_D_ENABLE) || defined(SPECTRO_M_ENABLE)
    UlikFlash ulik;
    #endif
    #if defined(CTD_ENABLE)
    CTD ctd;
    #endif
    #if defined(MODEM_POWER_CONTROL)
	int wakeUpTimeModem;
	#endif
    #if defined (YSI_ODD_ENABLE)||defined(YSI_ODOCT_ENABLE)
    ODOCT odoct;
    #endif
    #if defined (SAMAN_ENABLE)
	int pulsCountPv;
	int timerCnt200ms;
	#endif
	#if defined(PONSEL_EC)
	__sn_ponsel_ec ponsel_ec;
	#endif
	#if defined(TYPE_APPROVAL_ENABLE)
	float codeMSIG[2];//검출신호
	float codeMTM1[2];//수온
	//float codeMTM2;//측정조온도
	//float codeMSAM;//희석비율
	float codeZERO[2];//영점교정액농도
	float codeSPAN[2];//스팬굥정액농도
	//float codeSLOP;//교정기울기
	//float codeICPT;//교정절펀
	float codeFACT[2];//보정기울기
	float codeOFST[2];//보정절편
	float codeMAXR[2];//측정범위
	char password[12];//비밀번호
	//float codeMv1;//mV LOW
	//float codeMv2;//mV HIGH
	char tmsDataName[10][6];
	#endif
    volatile int    EndVerify;
}__SmartLogdata;

extern __SmartLogdata flash;

/*********************************************************************************************

GLOBAL 변수

*********************************************************************************************/
#if defined (YSI_ENABLE)
#define     DOS_SCREEN_BUF_SIZE     600
#else
#define     DOS_SCREEN_BUF_SIZE     800
#endif
#define     LAN_RXBUF_SIZE          512
#define		SMARTLOG_DATALOG_SIZE   4500
#define     USART_BUFFER_MAXSIZE    400
typedef struct __global
{
    volatile int StartVerify; 	
    volatile int usbReadErrorFlag;//USB읽기에러 발생시 ENABLE 된다.
    volatile int wakeUpFlag;//슬립모드 해제용	
    //volatile int screenWatchTimerCount;
    volatile int buttonWatchTimerCount;// button on 이벤트시 타이머 시작 button off 이벤트 발생시 타이머 종료
    volatile int usbProgramUpdateFlag;
    volatile int sm718WaitMessage;//에러가 발생하면 
	//volatile int ClearDataFlag;//YSI 데이터 수신시 필요없는 데이터 수신거부시 사용한다.
	//volatile int screenCreateFlag;
    volatile int powerResetMessage;//ioboard 에서 수신하면 메시지를 저장한다.
    volatile int systemWatshTimerCnt;//시스템 감시용 타이머
    volatile int systemWatchTimerFlag;    
    
 	volatile int	nReceiveItemNameBufCount;
	volatile int	nCount_Receive_Report;
	volatile int	nFlag_Receive_Item_Find;
	volatile int	nFlag_RecevieItemListRead;
	volatile int	nPARA_Command_Start;
	volatile int	nFlag_Sensor_DataRead[8];//Uart 채널이 화면이 현재 어디에 속하는가?
	volatile int	nFlag_Receive_Report_Start;
	volatile int	nPara_UartChannel;   //para 모드에서 통신 채널을 선택한다.

    //통신용버퍼
	int 			nReceiveBufferCount[USART_MAX];//데이터 수신버퍼	
	int				nFlag_ReceiveStart[USART_MAX];//데이터 수신시작											
	unsigned char   aItemReceiveBuffer[USART_MAX][USART_BUFFER_MAXSIZE];
	int				nFlag_ReceiveDataProcess[USART_MAX]; //데이터를 수신 완료 플레그 
    int             exoRequestWait;//데이터요청을 잠시대기한다.
	volatile int	NextScreenCreate;
	volatile int	NewScreen;//같은 화면이 반복생성되는 것을 막는다.
	volatile int	OldScreen;
	volatile int	ChangeNameFlag;
	volatile int	comportConnectionError[USART_MAX];
	volatile int	tmsStatusCode[USART_MAX];			//5분간격 TMS 상태코드
	volatile int	tmsStatusCode60minComErrCnt[USART_MAX];	//5분간격 통신불량횟수
	volatile int 	tmsStatusCode60MinCalCnt[USART_MAX];//5분간격 교정횟수
	
	volatile int			usb_reconnectFlag;
	
	volatile float			tms_pHmV;	
//	volatile float			tms_DOPV;
//	volatile float			tms_TempPV;
//	volatile float			tms_SPCondPV;
//	volatile float			tms_TubidityPV;
	
	volatile int			ReceiveDataLength[3];
	volatile int			dumpTimeSaveFlag;//5분전데이터를 저장하기 위함

    U32					handle[70];
	U32					handle2[70];
    volatile int        cal420Toggle;//AD420 교정시에 사용한다.
    U32                 titleBarLogId;
    U32                 titleBarTimeId;
    U32                 titelBarId;
    char                titleBuf[128];
    
    volatile unsigned int            autoScreen;
    volatile unsigned int            autoScreenTimer;
    float         					 value420Output[USART_SENSOR_MAX];
    volatile unsigned int            ysiResetStep[5];
    
    //volatile unsigned int DA_AddressData;
    
    volatile unsigned int lanCloseTimerCount[2];
    volatile unsigned int lanDataTransmitSize;
    volatile unsigned int lanSendFlag[2];
    unsigned char lanRxBuf[2][LAN_RXBUF_SIZE];
    volatile int lanStatus[2];//0. 대기중 1. 접속중
    volatile unsigned int usbSaveWaitFlag;
	
	volatile unsigned int sleepModeTimerCntSecPv;
	
    volatile int dumpStartTimePv;
    RealTime dumpStartDate;//60분데이터 저장시간
	volatile int dumpEndTimePv;
    RealTime dumpEndDate;//60분데이터 저장시간
    volatile int kimRamAddress;//덤프 데이터 송신중 주소값
    volatile int dumpSendStatus;//SD카드 데이터를 다시 읽는다.
    volatile int dumpMode;//덤프 데이터 송신 도중 다른 데이터 요청이 들어온경우

	//volatile int newTransmitMode;
	//volatile int reSdCardReadingFlag;

	//int clientConnectFlag;

	//volatile int sleepModeLanOut;
	
	volatile int touchErrorCount;//터치가 연속적으로 눌리는 경우 터치 에러메시지를 출력한다.
	volatile int touchReceiveErrorCount;//TOUCH 보드에서 상태값이 송신이 안되는 경우 리셋한다.
    volatile int touchRebootFlag;
	volatile int messageBoxDispFlag;
	volatile int UpdateAddress;
	volatile int sendDataSize;
	volatile int downloadFlag;
	volatile int downloadDataSize;
	
	volatile int ad420Ch1mA;//4~20mA 출력값 0~65535
	volatile int ad420Ch2mA;
	
    volatile int debugFlag;//LOGDATA 버튼을 누르면 활성화된다. COMMAND 화면에 DEBUG 데이터가 출력된다.
    volatile int screenCreateFlag;
	//volatile float ioBoardPv[8];
	//volatile float OldIoBoardPv;
	
	//volatile int readErrorFileFlag;//파일읽기에 실패하면 해당파일을 삭제한다.
	//volatile int sleepExoDataReceiveFlag;//EXO 데이터 읽는시점
	
    volatile int averCnt5Min[USART_MAX];                           //누적데이터 카운트 값
	volatile int averCnt60Min[USART_MAX];
    volatile float averPV5Min[USART_MAX][USART_SENSOR_MAX];	//5분평균데이터
	volatile float averPV60Min[USART_MAX][USART_SENSOR_MAX];	//60분평균데이터
    //volatile int printOutStorageTimeFlag;   
	unsigned char		dataLogBuf[SMARTLOG_DATALOG_SIZE];
    unsigned char		transmitBuf[SMARTLOG_DATALOG_SIZE];
	char				timeBuf[32];    

    unsigned int openFileSize;
    unsigned int openFileSizeCnt;//파일 읽어낸 사이즈를 기억한다.
    //unsigned int returnAckCheck;//데이터 송신 후 ACK 신호대기
	char			dosScreenData[DOS_SCREEN_BUF_SIZE]; //DEBUG 화면구성
	char			dosScreenData2[DOS_SCREEN_BUF_SIZE];
	volatile int	dosCnt;
	volatile int	dosCnt2;
	volatile int	dosScreenFlag;
	volatile int	dosScreenFlag2;

    volatile unsigned char serialComStatusToggle[10];//통신용 상태 LED 동작용8
	
	int jsonTcpDebugFlag;

    #ifdef JSON_ENABLE
        JsonGlobalVal json;    
    #endif
    #ifdef EXO_ENABLE
    volatile unsigned int	exo_count;
	volatile float			BarometerPV[3];
    volatile int            exoRxLength[5];
    volatile int exoCalModeRxFlag;
	volatile unsigned char	EXOTimeCode[4];
    volatile unsigned char  exoOldData[5];
    int             exoBufferCount[USART_MAX];
	int exoPortFindCnt[3];//센서인식작업시 첫번째 포트 부터 찾기위해 전역변수 활용
	int findDelayTimeCnt;//전원이 리셋되면 최초 ?초간은 대기한다. 센서 초기화가 완료될 때까지 기다린다.
	int wiperWaitCountmSec;
	volatile int				wiperEnableFlag;//40초 대기 Flag이다.
	volatile unsigned int	    wiperWaitCountSec;//40초 대기 count를 실행한다.
	volatile int				wiperEnableTimePV;
    #endif
    //TMS
    #ifdef KECO_PROTOCOL_ENABLE
    unsigned int    tmsReSendCount;
    unsigned int    tmsDumpStepCnt;//2이면 재송신 
	#if defined(TYPE_APPROVAL_ENABLE)
	char password[12];
	int password_timer;
	int password_id;
	int password_parent;
	int password_handle;
	#endif
	#endif
    #if defined(SPECTRO_D_ENABLE) || defined(SPECTRO_M_ENABLE)
    int ulikLength[3];//통신길이
    int ulikFunctionCode;
    int ulikOldAscii;
    int ulikToggleParaMeasRequestFlag;//명령을 바꿔가면서 송신한다. 1. 데이터요청 2. 설정값요청
    int ulikCalItem;//0.COD, 1.TOC, 2.TUR
    int ulikCalStep;//교정진행 스텝이다.
    int ulikMessageStep;//메시지 출력 순서
    int ulikCleanTimerCount;//세정시간타이머증가 10sec
    int ulikCalCmdSendNo;//교정값 송신
    int ulikReceivedItemNo;
    
    float WaterT;
    float COD_KHP;
    float TUR;
    float TOCstd;
    float COD_Cr;
    float TOCreal;
    float NO3;
    float NO3org;
    float TURorg;
    //parameter 
    float paraValue1[23]; //COD_KHPsensor
    float paraValue2[23]; //TURsensor
    float paraValue3[23]; //CODreal        

    int ctd_ulik_toggle; //측정값 번갈아 가며 요청한다.    
    int ctd_cal_mode; //ctd 교정모드인 경우
    int ctd_cal_step;//ctd 교정 스텝
    
    float ctdPv[10];//ctd 센서 측정값
    int ctdCalClearStep;//교정초기화과정
    #endif
    #if defined (YSI_ODD_ENABLE)||defined(YSI_ODOCT_ENABLE)
		int     modbusId;
        int     odo_ArrayAddress;
        int     odo_mode;
        float   satPv[10];
        float   odoPv[10];
        float   tempPv[10];
        float   baroPv[10];
    
        float   condPv[10];
        float   spcondPv[10];
        float   salinityPv[10];
        float   tdsPv[10];
    
        unsigned int K1;
        unsigned int K2;
        unsigned int K3;
        unsigned int K4;
        unsigned int K5;
        unsigned int K6;
        unsigned int K7;
        unsigned int KC;
        
        int request_DataFlag;
        unsigned int qcScoreTime;
        int qcScoreStatus;
    #endif
    #ifdef BATTERY_ENABLE
        int requestBatteryInfoNo;//배터리 정보요청순서
    #endif
    volatile int EndVerify;
}__globalVal;
extern __globalVal gv;   


#ifdef __cplusplus
}
#endif

#endif
/******************* (C) COPYRIGHT 2014 ZionTech *****END OF FILE****/
