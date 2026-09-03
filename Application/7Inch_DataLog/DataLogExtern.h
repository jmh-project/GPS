#ifndef __DATALOG_EXTERN
#define __DATALOG_EXTERN

#ifdef __cplusplus
 extern "C" {
#endif
#include "DataLog.h"
#include "TEECO_C_Define.h"     

int  SmartLog_Uart1Two_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart1Four_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart1Six_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart1Eight_Window(HANDLE parent, HANDLE handle);

int  SmartLog_Uart2Two_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart2Four_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart2Six_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart2Eight_Window(HANDLE parent, HANDLE handle);

int  SmartLog_Uart3Two_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart3Four_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart3Six_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart3Eight_Window(HANDLE parent, HANDLE handle);

int  SmartLog_Uart4Two_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart4Four_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart4Six_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart4Eight_Window(HANDLE parent, HANDLE handle);

int  SmartLog_Uart5Two_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart5Four_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart5Six_Window(HANDLE parent, HANDLE handle);
int  SmartLog_Uart5Eight_Window(HANDLE parent, HANDLE handle);

int  SmartLog_ADFour_Window(HANDLE parent, HANDLE handle);
int  SmartLog_ADTwo_Window(HANDLE parent, HANDLE handle);
int  SmartLog_ADSix_Window(HANDLE parent, HANDLE handle);
int  SmartLog_ADEight_Window(HANDLE parent, HANDLE handle);
int  SmartLog_EX100_Window(HANDLE parent, HANDLE handle);
int  HOMOGENIZATION_Window(HANDLE parent, HANDLE handle);
int RainStorage_Window(HANDLE parent, HANDLE handle);

	 
int  SmartLog_Uart1Ten_Window(HANDLE parent, HANDLE handle);	 
int UartStatusChecking( unsigned int uartPort );	 
int SaveTheMeasurementWater(int time);	 
int GpsReceiveProcess(int ch);	 
void SensorLimitProcess(void);
int RelayControl( 
    unsigned char port1, unsigned int status1, 
    unsigned char port2, unsigned int status2,
    unsigned char port3, unsigned int status3,
    unsigned char port4, unsigned int status4,
    unsigned char port5, unsigned int status5,
    unsigned char port6, unsigned int status6,
	unsigned char port7, unsigned int status7
    );

void DataLog_SensorLimitItemKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);	 
void SystemPowerUp(void);
void LCD_ON(void);
void LCD_OFF(void);
void PowerControl(void);	 
void ScanDataRequest( int comPort );	 
unsigned int HMI_Gps_Format( char *name, float fdata, int status);	 
unsigned int HMI_item_FormatString( char *name, char *stringd, int status);	 
void HMI_DumpSaveFormat_WaterMonitoring( int dumpAutoDump );	 
int	UartPortInfo( int uart, char *data);
void AutomaticWaterSamplerRequest(int comPort);
void AutomaticWaterSamplerCommand(int comPort);	 
void AutomaticWaterSampler(int ch, BYTE ascii );
	 
int CopyRectVideo(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height, uint32_t addr);
int BmpToLan(char *sx1, char *sy1, char *width1, char *height1, char *filename, int slotNumber);	 
int SDtoLanTrandmit(void);	 
int EventMessageSave(int message);	 
void RequestDataHFTOL(void);	 
void RelayPowerControl(void);	 
void AlarmCreate(void);	 
int AD420mAInput(void);	 
int AD420mAOutput(void);
int Uart1ReceiveProcess(void);	 
int Uart2ReceiveProcess(void);
int Uart3ReceiveProcess(void);
int Uart4ReceiveProcess(void);
int Uart5ReceiveProcess(void);	 
	 
void SaveMeasuredValues(void);	 
void UserSetTimeDataSaveing(void);	 
#ifdef GOSUNG
void RequestDataHFTOL(void);
#endif	 
void UartConnectErrorCheckingMode(void);	 
void StructCheckingMode(void);	 
#ifdef RELAY_SENSOR_ENABLE
void RelayPowerControl(void);
#endif
#ifdef INPUT_PORT_SELECT_1
void AlarmCreate(void);
#endif
void SleepModeRtcRead(void);
void LanCheckingMode(void);	 
int EntrySleepMode(void);	 
int RebootSetTime(void);	 
int GetWidgetStatus(U32 id);     
void PortInit_16C554(void);
void UartA_SendByte(U8 c);
int BOAT_DataLog_Save(void);
void PODOC_Status(unsigned int comPort,unsigned int address);
void PODOC_Request(unsigned int comPort,unsigned int address);
void PODOC_Command(unsigned int comPort,unsigned int address, unsigned int functionCode, unsigned int hMemory, unsigned int lMemory);
int DataToDisk(void *struct_data,  int data_size, char *filename);
int Remove(char *name);
int DiskToData(void *struct_data,  int data_size, char *filename);
U32 TitleBar(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		backcolor, 
			int		fontcolor,
			int		align,		//Left, Right, Center
			int		style,		//모양을 결정한다.
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );	 
U32 SButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			char	*IconName,								// 버튼에 표시되는 아이콘
			char	*text,								// 버튼에 표시되는 문자열
			int	function,							// 토글버튼, 푸쉬버튼의 구분				
			U32	id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
U32 NButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			char	*IconName,								// 버튼에 표시되는 아이콘
			char	*text,								// 버튼에 표시되는 문자열
			int	function,							// 토글버튼, 푸쉬버튼의 구분				
			U32	id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
			
            
int PONSEL_EC_DataReceive( int ascii, int channel );
void DS1302_SecSet(uint8_t date);
void DS1302_YearSet(uint8_t date);
void DS1302_MonthSet(uint8_t date);
void DS1302_DaySet(uint8_t date);
void DS1302_HourSet(uint8_t date);
void DS1302_MinSet(uint8_t date);



            
int	TitleBar_Create( HANDLE handle, int ch, char *buf );//핸들과 USART 채널 정보에 대한 데이터를 출력한다.
int DS1302TimeRead(void);
int SystemMain_Window(HANDLE parent, HANDLE handle);
int Uart_TransmitPort( unsigned int uartPort, unsigned char data );            
int TMS_Transmit(int channel);
int Average5MinData(void);
int Average60MinData(void);
int Average5MinDataCalculated( void );            
int Average60MinDataCalculated( void );    
int SDFileDelect( char *filename2);
int SD_FileWrite(void);
            int DataToDisk(void *struct_data,  int data_size, char *filename);
unsigned char TMS_MeterStatus( unsigned int uartPort, unsigned char sum);
unsigned int WinchReceiveDataProcess( unsigned int comport );
unsigned int WinchReceive( unsigned int comport, unsigned char ascii );
void WinchControl( unsigned int uartPort, unsigned int command, float depth );
unsigned int StepDataReceiveConverter( unsigned int channel);
unsigned int StepControl( unsigned int uartPort, unsigned int step );
int Sontek_DataReceiveConverter(int usart, int channel);
int Sontek_DataReceive( int ascii, int ch);
void AD420Control( unsigned short nValue, int channel);
int	fn420mAOutput( float lowlimit, float highlimit, float fValue, int channel, int UartChannel );
int Smapler_DepthTransmit( unsigned int comPort, unsigned int depthData );
unsigned int Sampler_Data_Request( unsigned int comPort );
int SensorDataErrorCheck(int ch);
unsigned int RS232_Receive( unsigned char ascii, unsigned int channel );
int HFTOL_ReceiveConverter(int usart, int channel);
int	SensorCommand_HFTOL(int command,int usart);
int HFCLX_ReceiveConverter(int usart, int channel);
int	SensorCommand_HFCLX(int command,int usart);
unsigned short CRCCCITT(unsigned char *data, unsigned int length, unsigned short seed, unsigned short final);
int EXO_SondeIDFind(int comPort);
int	EXO_SensorFind(int comPort, int hexaSensorPort);
int Calibration_EC(int UartPort, float calBuffer);
int EXO_Test2(void);
int	EXO_Transmit(unsigned int comport,  int command);
void EXO_Test(void);
int	EXO_Receive_Process(unsigned int UartSelect);
int fnSensor_PARAX_Command_Process( int ch );
void SmartLogUSBMessage(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
 int USB_Control(HANDLE parent, HANDLE handle);
int DataLog_MessageBox_Window(char *title, char *text);
void DataLog_ReNameItemKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
int	Int420mAOutput( void );
float ADCReading( void );
void InputReading( void );
void Ex100_Receive(int channel, unsigned char ascii);
int DataLog_Data_Display(unsigned int comPort, unsigned int channel);
int TransmitCharData( char font, char channel);
int SCan_DataReceive( int ascii, int channel );
int DiskToData(void *struct_data,  int data_size, char *filename);
int SensorMain_Window(HANDLE parent, HANDLE handle);
void SmartLog_Uart4Init(void);
void SmartLog_Uart3Init(void);
void SmartLog_Uart6Init(void);
void SmartLog_Uart1Init(void);
void IoportInit( void );
void UPS_USB_Power(int s);
U32 SLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style);
int SmartLogExo_ItemDisplayProcess(int UartSelect);
int SensorCommand_Intellitect(unsigned int address);
int SaveTheMeasurement( unsigned int time );
int fnSensorReceiveValueProcess(int ch);
int Scan_ReceiveConverter(int usart, int channel);
int	SmartLogEX100Control1(float pv, int	uart);
int	SmartLogEX100Control2(float	pv, int	uart);
int Sampler_ReceiveConverter(int usart);

int BusanDumpDataTransmit( unsigned int time, unsigned int timeFormat, unsigned int output);
int RS232_DataTransmit( unsigned int comPort );
//SD_Error DiskInit(void);
unsigned short crc16(unsigned char *puchMsg, unsigned short usDataLen) ;
int RS232_DataTransmit( unsigned int comPort );
int	EXO_Calibration_Command(unsigned int comPort, unsigned int destAddress, unsigned int sourceAddress, 
unsigned char title,unsigned char none, unsigned char item, unsigned char commandList, unsigned char length1, unsigned char length2, unsigned char *data);
void SmartLogEXOItemKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
unsigned int    MainScreenCreate( HANDLE parent, HANDLE handle );
U32 GroupPanel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text);
int Info_Window(char *title, char *text);
void SKeyPad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int seq, 
	char *value, char *title, int mode, int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
void TimerQClear(void);
U32 Label(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int	fontcolor);
#ifdef DATALOG_7INCH
void IntAD420Control(unsigned short value1, unsigned short value2, unsigned short value3, unsigned short value4);
#endif
#ifdef DATALOG_8INCH
void IntAD420Control(unsigned short value1, unsigned short value2);
#endif
int SDToData(char *filename, void *s, int size);
int Ysi_ReadData_Dos( char ascii );

U32 NGroupPanel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text);
void SD_Mount(void);
int W5500_Server(int slot);
void ADP_DataReceive( unsigned char ascii );
int ADP_DataProcess( unsigned char comPort );
void Uart_SendByte7(char Data);
int DUMA_TimeAdd( int time );
int DUMP_TimeAdd( int time );
int DUMP_AUTOTimeAdd( int time );
void Uart1_Config(void);
void Uart2_Config(void);
void Uart3_Config(void);
void Uart4_Config(void);
void DataLog_VarInit(void);
void W5500_Init(void);
uint8_t IntToBCD(int data);
int StringToSDCardWrite(char *filename, int size);
U32 STextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,		//24 는 24사이즈, 48, 64 3가지가 준비되어있다.
			char	*text,		//init Character
			int	fontcolor, 
			int	backcolor,
			int	align,		//Left, Right, Center
			int	attr,		//READ_ONLY, RW_MODE
			U32	id,
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
unsigned short crc16(unsigned char *puchMsg, unsigned short usDataLen);
int DataLog_Key_Event( HANDLE parent, HANDLE handle, HANDLE timer, HANDLE id );            
int Argadv_Process( int usart );
int Argadv_Receive( int ch, int ascii );
void ARGADV_DumpSaveFormat( int dumpAutoDump );
unsigned int DUMP_Header_Format(int itemCnt,int dumpautodump);            
unsigned int DUMP_item_Format( char *name, float fdata, int status);      
void UART_AD420_Control( unsigned short value1, unsigned short value2 );  
U32 NLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int fontcolor, int backcolor, int style);
int SD_StructFileWrite(char *filename, int size);            
int SmartLogSensorChannelFunction(HANDLE parent, HANDLE handle);            
void Rain_Relay_Control(void);
void RainStorage_Process(void);
int DO_PONSEL_IOProcess(void);     
int PONSEL_DO_Receive( int ascii, int channel ); 
void PONSEL_DO_Request(unsigned int comPort,unsigned int address);   
void PONSEL_DO_Status(unsigned int comPort,unsigned int address);     
void PONSEL_DO_Command(unsigned int comPort,unsigned int address, unsigned int functionCode, unsigned int hMemory, unsigned int lMemory);            
void PONSEL_SAL_Cal(int comPort, int address);
void PONSEL_Pressure_Cal(int comPort, int address);
            
int SaveTheMeasurement_Incheon( unsigned int time );
void SaveMesaurements(void);
void SystemRebootGo(void);
int SystemWakeUp(void);
void TouchChecking(void);
int TMS_TransmitFormatSave(int channel);
void HMI_DumpSaveFormat( int dumpAutoDump );
#ifdef WS501_ENABLE
void WinterCommand(int commandList);
int  WinterDataProcess( int comPort );
int SonarDataProcess( int comPort );
#endif
#if defined( PONSEL_PH_COND_ENABLE ) || defined(PONSEL_PH_ENABLE)
unsigned int ponsel_phcond_step=0;
#endif
#ifdef HMI_5MIN_AUTO_DUMP_LAN_TRANSMIT
extern unsigned int HMI_Header_Format(int itemCnt, int dumpautodump);
#endif
#ifdef AUTO_SAMPLER
volatile int gAutoDataRequest = 0;
extern float gPv1[35];
extern float gPv2[35];
extern float gPv3[35];
extern float gPv4[35];
extern float gPv5[35];
extern float gPv6[35];
extern float gPv7[35];
extern float gPv8[35];
extern float gPv9[35]; 
extern int gPvCnt;
extern void AutoSamplerProcess(void);
extern int FloatCompare(const void *key, const void *element);

#endif
int TitleBar_MenuButton_Create( U32 handle, int comPort, int (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );


void DebugMessageOut(char *buf);
void TeecoBmpDMA_Config(void);
void SM718_2DInit(void);
int TestFunction(void);
void GPU_PowerControl(unsigned int status);
int IOProcessHardwareControl(void);
int DataLogMain_Window(void);
void USB_Host_Start(void);
int EventMessage( char *message );
int W5500_IOProcess(void);
void IoBoardControl(void);			
void DataLog_VarInit(void);			
int TransmitStringData( char *font, char channel);


#ifdef MODBUS_ENABLE
int SendModbus(unsigned int comPort);
int ReceiveModbus( int ascii, int comPort );
extern void FloatPoint_RealProcess(int ch, int comPort,  char *data);
#endif
#ifdef SPECTRO_D_ENABLE
extern int ULIK_Request(int comPort, int id, int function, int addr1, int addr2, int reg1, int reg2);
extern int Ulik_HoldingRegisterTable_Reading(int comPort, int id);
extern int Ulik_WindowCleaning(int comPort, int id, int function);
extern int Ulik_Restart(int comPort, int id, int function);
extern int Ulik_SaveF(int comPort, int id, int function);
extern int Ulik_ZeroCalibration(int comPort, int id, int function);
extern int Ulik_ReqMeas(int comPort, int id, int function);
extern int Ulik_CODCalDataStorageRegisterTable(int comPort, int id, int function);
extern int Ulik_CODcalibCtrl(int comPort, int id, int function, int menu);
extern int Ulik_CODReal(int comPort, int id, float value);
extern int Ulik_COD_KHPsensor(int comPort, int id, float value);
extern int Ulik_TURsensor(int comPort, int id, float value);
extern int Ulik_TOCCalDataStorageRegisterTable(int comPort, int id, int function);
extern int Ulik_TOCcalibCtrl(int comPort, int id, int function, int menu);
extern int Ulik_TOCReal(int comPort, int id, float value);
extern int Ulik_TOCstd_sensor(int comPort, int id, float value);
extern int Ulik_Parsing(int comPort);
extern int Ulik_ParameterParsing(int comPort);
extern int Ulik_Receive( int ascii, int ch );

extern int Ulik_TUR_StdCalibration(int comPort, int id, float value);
extern int Ulik_TURCalDataStorageRegisterTable(int comPort, int id, int function);
extern int Ulik_TUR_CalibrationClear(int comPort, int id);

extern int Ulik_COD_StdCalibration(int comPort, int id, float value);
extern int Ulik_COD_KPHCalDataStorageRegisterTable(int comPort, int id, int function);
extern int Ulik_COD_KPHCalibrationClear(int comPort, int id);

extern int Ulik_TOC_StdCalibration(int comPort, int id, float value);
extern int Ulik_TOC_StdCalDataStorageRegisterTable(int comPort, int id, int function);
extern int Ulik_TOC_StdCalibrationClear(int comPort, int id);

extern int Ulik_TEMP_StdCalibration(int comPort, int id, float value);
extern int Ulik_WindowAutoCleaningDisable(int comPort, int id, int function);
extern void ULIKScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
extern void ULIK_Init(void);
#endif
#ifdef SPECTRO_M_ENABLE
extern int ULIK_MW_Request(int comPort, int id, int function, int addr1, int addr2, int reg1, int reg2);
extern int Ulik_MW_ReqMeas(int comPort, int id, int function);    
extern int Ulik_MW_Parsing(int comPort);
extern int ULIK_MW_IOProcess(int comPort);
extern int Ulik_MW_Receive(int ascii, int ch );
extern int Ulik_MW_COD_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr);
extern int Ulik_MW_COD_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr);
extern void ULIKScreen(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
extern int Ulik_MW_COD_CalParaClear(int comPort, int id);
extern int Ulik_MW_COD_ParaSave(int comPort, int id);
extern int Ulik_MW_WindowCleaning(int comPort, int id, int function);
extern int Ulik_MW_TOC_CalParaClear(int comPort, int id);
extern int Ulik_MW_TUR_CalParaClear(int comPort, int id);
extern int Ulik_MW_NO3_CalParaClear(int comPort, int id);
extern int Ulik_MW_TOC_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr);
extern int Ulik_MW_NO3_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr);
extern int Ulik_MW_TUR_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr);
extern int Ulik_MW_TOCCalDataStorageRegisterTable(int comPort, int id, int function);
extern int Ulik_MW_TUR_CalParaClear(int comPort, int id);
extern int Ulik_MW_NO3_CalParaClear(int comPort, int id);
extern int Ulik_MW_NO3CalDataStorageRegisterTable(int comPort, int id, int function);
extern int Ulik_MW_TURCalDataStorageRegisterTable(int comPort, int id, int function);
extern void ULIK_Init(void);
extern int ULIK_MessageBox_Window(char *title, char *text);
#endif
#if defined(YSI_ODOCT_ENABLE) || defined(YSI_ODD_ENABLE)
extern int ODO_IOProcess(void);
extern int ODO_Window(void);
extern int ODO_CapInfoWindow(void);
extern int odo_ReceiveData(int comPort, unsigned char ascii);
extern int CT_Window(void);
#endif

#if defined(CTD_ENABLE)
extern void CTD_Read(int deviceId, int comPort);
void CTD_Write(int comPort, int id, int addsh, int addsl, unsigned char cmd, float value);
extern void CTD_Parsing(int comPort);
#endif
extern void ModemPowerControl(int port, int status);
extern int SD_ProgramUpdateFileWrite(char *filename, int size);
extern void FloatPoint_Process(int ch, int comPort,  char *data);
extern int32_t send(uint8_t sn, uint8_t * buf, uint16_t len);
extern void TcpIpDebug(char *fmt,...);
extern  int SystemAdministrator(void);
#ifdef SPECTRO_D_ENABLE
int ULIK_IOProcess(int comPort);
#endif
#if defined(BATTERY_ENABLE)
extern void TracerBatteryRequest(int comPort, int info);
extern int TrancerReveive(int id, int comPort, int ascii);
extern int TrancerParsing(int id, int comPort, int info);
#endif
#if defined (DAECHEONGHO_SUGILGWANLISO)
extern void DAECHEONGHO_FloatPoint_Display(int ch, int comPort);
#endif
extern void FloatPoint_ADInputAverageProcess(int ch, int comPort,  char *data);
extern void FloatPoint_AverageProcess(int ch, int comPort,  char *data);
extern void FloatPoint_RealProcess(int ch, int comPort,  char *data);
extern void FloatPoint_60MinAverageProcess(int ch, int comPort,  char *data);
extern void MeasuredValue60MinAveraging(int process);
extern int ValidMeasurement(float value, float minValue, float maxValue);

extern int FileStatusCheck(char *filename);
extern void ExopHSlope(float mV);
#if defined (PONSEL_EC)
extern int PONSEL_EC_Window(void);
extern int PONSEL_EC_DataReceive( int ascii, int ch );
extern void PONSEL_EC_Status(unsigned int comPort,unsigned int address);
extern void PONSEL_EC_Request(unsigned int comPort,unsigned int address);
extern void PONSEL_EC_Command(unsigned int comPort,unsigned int address, unsigned int functionCode, unsigned int hMemory, unsigned int lMemory);
#endif
#ifdef KECO_PROTOCOL_ENABLE
extern int KECO_ProtocolSending(int comPort);
extern int DUMP_BufferTOSend(RealTime startTime, RealTime endTime, int dumpMode);
extern int DUMP_SDCardTOBuffer(RealTime startTime, RealTime endTime);
#endif
#if defined(TYPE_APPROVAL_ENABLE)
extern int TYPE_APPROVAL_ProtocolSending(int comPort);
extern int TYPE_APPROVAL_DataReceive( char ascii, int comPort );
extern unsigned int TYPE_APPROVAL_SendFactor(int comPort, char *id, float value);
extern int TYPE_APPROVAL_DUMA_FileCreate(int comPort);
extern int TYPE_APPROVAL_DataReceive( char ascii, int comPort );
extern void TypeApprovalNameKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
extern void PasswordKeyPad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, HANDLE timer, char *title);
extern int PasswordInit(void);
extern void TYPE_APPROVAL_FactorSave(char *id, float value);
extern void TYPE_APPROVAL_PasswordResponse(int comPort);
extern void TypeApprovalFactorKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
#endif
extern void TMSStatusCode(int ch,int comPort, char *data);
extern void TMSStatusCode60Min(int ch, int comPort, char *data);
#ifdef __cplusplus
}
#endif

#endif

