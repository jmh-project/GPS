#ifndef __DATALOG_DEFINE
#define __DATALOG_DEFINE

#ifdef __cplusplus
 extern "C" {
#endif
#define READY_MODE 0
#define REAL_FILE_CREATE 1
#define DUMP_FILE_CREATE 2
#define AUTO_DUMP_FILE_CREATE 3

#define RS485_ENABLE 1
#define RS232_ENABLE 0	 
	 
#define STANDBY     0
#define DATA5MIN_SEND	1
#define ACK_SEND	2
#define	EOT_SEND	3
#define DUMP_SEND   4     
#define REAL_SEND   5     
#define PARA_SEND   6     
     
#define READY_MODE 0

#define RESULT_ERROR	1
#define RESULT_OK		2
#define RESULT_NOFILE	3
#define RESULT_FILE_OPEN_ERROR 4
#define RESULT_SD_MOUNT_ERROR 5
#define RESULT_FILE_WRITE_ERROR 6
#define RESULT_SD_ERROR 7
#define RESULT_READ_END	8
	 
	 
#define 	ACR				0x0d
#define 	ALF				0x0a	 	 


#define		ERROR_TIME	10000

#define		SAVE_MIN5			2
#define		SAVE_MIN60		3
#define		SAVE_FREE			1


//EXO DEFINE
#define	COMMON_ID					200
#define	COMMON_CALIBRATION_ID		100
#define	SONDE_ID						0
#define	EC_ID							1
#define DEPTH_ID						2
#define	PH_ID							3
#define	ODO_ID						4
#define	TURBIDITY_ID					5
#define	NONE_ID						6

#define 	ADC_DATA_DISPLAY			100		


#define	EXO_PORT1					1
#define	EXO_PORT2					2
#define	EXO_PORT3					3
#define	EXO_PORT4					4
#define	EXO_PORT5					5
#define	EXO_PORT6					6
#define	EXO_PORT7					7
#define	EXO_PORT8					8

#define 	EXO_PH						0
#define		EXO_ODO						1
#define		EXO_COND						2
#define		EXO_TURBIDITY_FNU			3
#define		EXO_CHLOROPHYLL			4
#define		EXO_FDOM						5
#define     EXO_AMMONINUM               6
#define		EXO_NOSELECT				100


#define		REQUEST_CMD		0

#define		CHANNEL 		8
#define		ITEM_LENGTH		20

#define		DUMA_COMMAND 20

#define		TMS_DATA	3//실시간데이터요청
#define		TMS_RDAT	11//순간측정자료요청
#define		TMS_DUMP	2//저장자료요청
#define		TMS_RCHK	12//원격검색
#define		TMS_CCHK	13//교정값검색
#define		TMS_TCHK	14//교정값송신
#define 	TMS_SETT	1//시간변경
#define 	TMS_SETP	4//비밀번호변경
#define		TMS_RSET	5//자료수집기초기화
#define 	TMS_INFO	6//부가정보요청
#define 	TMS_ASKP	7//비밀번호요청
#define 	TMS_TRAP	8//비밀번호송신
#define 	TMS_CVER	9//버전정보검색
#define 	TMS_TVER	10//버전정보송신
#define		TMS_SAMP	15//채취실행

#define 	KECO_SETT	4
#define		KECO_DUMA	5
#define		KECO_DATA	6
#define		KECO_RESPONSE	7
#define		KECO_DUMA_RESPONSE 8
#define 	KECO_RESET   9
#define 	KECO_PASS   10

#define		CH1					0
#define		CH2					1
#define		CH3					2
#define		CH4					3

#define RELAY_1_PORT		0
#define RELAY_2_PORT		1
#define RELAY_3_PORT		2
#define RELAY_4_PORT		3
#define RELAY_5_PORT		4
#define RELAY_6_PORT		5
#define RELAY_7_PORT		6


#define AD420_LATCH			(0x10)
#define AD420_CLOCK			(0x20)		 
#define AD420_DATA			(0x40)
#define AD420_DELAY			20
#define AD420_MA_OUT		(0x04)
#define	AD420_CLEAR			(0x08);

#define	AD420_CH1			0
#define	AD420_CH2			1
#define	AD420_CH3			2
#define	AD420_CH4			3
#define	AD420_CH5			4
#define	AD420_CH6			5
#define	AD420_CH7			6
#define	AD420_CH8			7
#define	AD420_CH9			8
#define	AD420_CH10			9				

#define		INOUT_BTN_ID	1
#define		CONFIG_BTN_ID	2
#define		SENSOR_BTN_ID	3
#define 	CAL_BTN_ID		4
#define 	USB_BTN_ID		5
#define		NEXT_BTN_ID		6

#define		LENGTH			25
#define 	START_LENGTH	50
#define		END_LINE		50
#define		BACKSPACE		0x08
#define		LINE_FEED		0x0A
#define 	ENTER			0x0D
#define		ESC 			0x1B
#define		SPACE			0x20
#define		BUF_SIZE		200

#define 	FLASH_SAVE_BLOCK	3800
// FONT DEFINE
#define		FONT16					0
#define 	DOS_SCREEN_FONT			4
#define		FONT64					29

#define		DOS_MODE				1
#define  	SCREEN_MODE			2
#define		ITEM_READ_MODE			3


//interrupt 순위설정 
#define 	AD7135_PRIORITY			3					


// SOUND DEFINE
#define 	SOUND_TOUCH_ONE		1

// DATA OUT FORMAT DEFINE
#define	FLOAT_FORMAT1	"%1.1f"
#define	ITEM_FORMAT		"%s" 
#define	VALUE_DISP_TIME_MS	500

#define	USART_1		0
#define	USART_2		1
#define	USART_3		2
#define	USART_4		3
#define	USART_5		4
#define	USART_6		5
#define	USART_7		6
#define	USART_8		7


#define		IO_PORT_COMMAND Uart_SendByte1

#define		SENSOR_UART1	Uart_SendByte4
#define     SENSOR_UART1_STR Uart_Printf4
#define 	SENSOR_UART2	Uart_SendByte1
#define     SENSOR_UART2_STR Uart_Printf1
#define		SENSOR_UART3	Uart_SendByte3
#define     SENSOR_UART3_STR Uart_Printf3
#define		SENSOR_UART4	Uart_SendByte6
#define     SENSOR_UART4_STR Uart_Printf6

#define     SENSOR_UART5 Uart_Printf7
#define     SENSOR_UART6 Uart_Printf7
#define     SENSOR_UART7 Uart_Printf7
#define     SENSOR_UART8 Uart_Printf7

#define     SENSOR_UART5_STR Uart_Printf7
#define     SENSOR_UART6_STR Uart_Printf7
#define     SENSOR_UART7_STR Uart_Printf7
#define     SENSOR_UART8_STR Uart_Printf7

//AD 채널을 선택한다.
#define INPUT_1		0
#define INPUT_2		1
#define INPUT_3		2
#define INPUT_4		3
#define INPUT_5		4
#define INPUT_6		5
#define INPUT_7		6
#define INPUT_8		7
#define INPUT_9		8
#define INPUT_10		9

//OUTPUT 채널
#define OUTPUT_1	0
#define OUTPUT_2	1
#define OUTPUT_3	2
#define OUTPUT_4	3
#define OUTPUT_5	4
#define OUTPUT_6	5
#define OUTPUT_7	6
#define OUTPUT_8	7
#define OUTPUT_9	8
#define OUTPUT_10	9


#define EXT_MODBUS	1
#define EXT_TMS		2
#define EXT_NORMAL	3
#define EXT_LAN		4


#define	SENSOR_NONE					0
#define	SENSOR_YSI					1
#define	SENSOR_TN					2
#define	SENSOR_TP					3
#define	SENSOR_AUTO_SAMPLER		4 //자동채수기
#define	SENSOR_WIZ					5
#define SENSOR_MODBUS				6
#define SENSOR_TMS					7
#define	SENSOR_EXO					8
#define	SENSOR_HFTOL				9	
#define	SENSOR_HFCLX				10
#define	SENSOR_SONTEK				11
#define	SENSOR_SEJONG				12
#define SENSOR_NORMAL				13
#define	SENSOR_SCAN					14
#define	SENSOR_EX100_1				15	
#define	SENSOR_EX100_2				16	
#define	ANALOG_OUT					17
#define	SENSOR_EXTTMS				18
#define	SENSOR_INTELL				19
#define	SENSOR_TUBIDITY				20
#define	SENSOR_WINCH				21
#define	SENSOR_SAMPLER				22
#define	SENSOR_RS232					23
#define	SENSOR_ADPSONTEK			24
#define SENSOR_BUSAN				25
#define BUSAN_DUMP_560MIN			26
#define	BUSAN_DUMP_5MIN			27
#define	BUSAN_DUMP_60MIN			28
#define	BUSAN_TIME_SETTING			29
#define	BUSAN_COM1					30
#define SENSOR_FLOW                 31
#define	SENSOR_LAN					32
#define	SENSOR_UITNH4				33
#define SENSOR_PONSEL_DO                    34
#define SENSOR_PONSEL_EC                    35
#define SENSOR_PONSEL_PH                    36
#define SENSOR_PONSEL_PH_COND           37
#define SENSOR_ARGADV                       38
#define SENSOR_RAIN                             39
#define SENSOR_ECD_PH                             40
#define SENSOR_MSERIES                             41
#define SENSOR_AUTOSAMPLER              42
#define SENSOR_CLX_EX	43
#define SENSOR_WS501	44
#define SENSOR_SONAR	45
#define SENSOR_GPS 46
#define SENSOR_DMSAMPLER 47
#define SENSOR_NEP500	48
#define SENSOR_SAMPLER100 49
#define SENSOR_STATUS 50
#define SENSOR_KECO 51
#define SENSOR_ECT400 52
#define SENSOR_DEBUG 53
#define SENSOR_LDI 54
#define SENSOR_SPECTRO_D 55
#define SENSOR_ODO 56
#define SENSOR_CTD 57
#define SENSOR_SPECTRO_M 58

#define	SENSOR_BOD					5
#define	SENSOR_COD					6
#define	SENSOR_ECOLI					7//대장균 
#define	SENSOR_WATER_LEVEL		8 //수위 

#define	DUMP_DATA				1	
#define	AUTO_DUMP_DATA		2
#define	DUMA_DATA				3
#define	DUMP_DATA60				4
#define	AUTO_DUMP_DATA60		5

#define	SMARTLOG_CONFIG_ADDRESS				USER_SCREEN1
#define	SMARTLOG_CONFIG_TOUCH_ADDRESS	USER_SCREEN2
#define	SMARTLOG_INOUT_ADDRESS				USER_SCREEN3
#define	SMARTLOG_INOUT_TOUCH_ADDRESS		USER_SCREEN4

#define		TEMPERATURE			(6668-1)
#define		FLOW					(6686-1)
#define		BAR					(6692-1)
#define		PH						(6698-1)
#define		ORP					(6704-1)
#define		CONDUCTIVITY		(6710-1)
#define		TURBIDITY				(6716-1)
#define		COLOUR				(6722-1)
#define		CHLORINE1				(6728-1)
#define		CHLORINE2				(6734-1)
#define		MONO_CHL			(6740-1)
#define		DISSOLVED_OXYGEN	(6746-1)


#define     CAN_USART1           1000
#define     CAN_USART2           2000
#define     CAN_USART3           3000
#define     CAN_USART4           4000
#define     CAN_USART5           5000

#define		CAN_AD420			6000
#define     CAN_IN_STATUS       7000
#define     CAN_IN_MA            8000
#define     CAN_EXIT_MA          8100    
#define     CAN_RELAY           9000
#define		CAN_YSI_POWER_OFF	9100
#define		CAN_FLOW				9200//남강댐
#define		CAN_FLOW1				9201//군위댐1
#define		CAN_FLOW2				9202//군위댐2
#define		CAN_FLOW3				9203//군위댐3
#define		CAN_DAM_CODE			9204
#define		CAN_ARRAY_LAN		0	

            
#define WINCH_DOWN  0
#define WINCH_UP 1
#define WINCH_HOME 2
#define WINCH_STATUS 3
#define WINCH_INIT 4
#define WINCH_LOW_END_POINT_RETURN 5
#define WINCH_START_POINT_INIT 6
#define WINCH_LOW_END_POINT_INIT 7
#define WINCH_DOWN_RELAY 8
#define WINCH_UP_RELAY 9
#define WINCH_DOWN_UPSPEED 10
#define WINCH_UP_UPSPEED 11

#define RELAY_1     0 
#define RELAY_2     1 
#define RELAY_3     2 
#define RELAY_4     3 
#define RELAY_5     4 
#define RELAY_6     5 

#define RELAY_7     6   //초우배출
#define RELAY_8     7   //우수공급 
#define RELAY_9     8   //시상수공급
#define RELAY_10   9   
#define RELAY_11   10 //집수조(OPEN) 
#define RELAY_12   11 //집수조(CLOSE)
#define RELAY_13   12 
#define RELAY_14   13   

#define INPUT_1     0   //유량계
#define INPUT_2     1   //전력량 펄스입력
#define INPUT_3     2 
#define INPUT_4     3 
#define INPUT_5     4
#define INPUT_6     5

#define INPUT_7     6   //초우배출(열림)
#define INPUT_8     7   //우수공급(열림) 
#define INPUT_9     8   //시상수공급(열림)
#define INPUT_10    9   //초우레벨
#define INPUT_11   10   //집수조(OPEN 1)
#define INPUT_12   11   //집수조(OPEN 2)
#define INPUT_13   12   //집수조(CLOSE 1)
#define INPUT_14   13   //집수조(CLOSE 2)
#define INPUT_15   14   //빗물센서
#define INPUT_16   15   //AUTO/MANUAL
#define INPUT_17   16   //초우배출(닫힘)
#define INPUT_18   17   //우수공급(닫힘)
#define INPUT_19   18   //시상수공급(닫힘)
#define INPUT_20   19   //집수조에러
#define INPUT_21   20	 //강우량	
#define INPUT_22   21
#define INPUT_23   22
#define INPUT_24   23
#define INPUT_25   24
#define INPUT_26   25
#define INPUT_27   26
#define INPUT_28   27
#define INPUT_29   28
#define INPUT_30   29

#define ONLY_ONE_PROCESS	222


#define ATM_WRITE		0x00
#define EC_WRITE		0x01
#define DEPTH_WRITE		0x02

#define SLEEP_POWER_ENABLE 2
#define MAIN_POWER_ENABLE 1


#define	KEY_A	1
#define	KEY_B	2
#define	KEY_C	3
#define	KEY_D	4
#define	KEY_E	5
#define	KEY_F	6
#define	KEY_G	7
#define	KEY_H	8
#define	KEY_I	9
#define	KEY_J	10
#define	KEY_K	11
#define	KEY_L	12
#define	KEY_M	13
#define	KEY_N	14
#define	KEY_O	15
#define	KEY_P	16
#define	KEY_Q	17
#define	KEY_R	18
#define	KEY_S	19
#define	KEY_T	20
#define	KEY_U	21
#define	KEY_V	22
#define	KEY_W	23
#define	KEY_X	24
#define	KEY_Y	25
#define	KEY_Z	26
#define	KEY_POINT	27
#define	KEY__		28
#define	KEY_1	29
#define	KEY_2	30
#define	KEY_3	31
#define	KEY_4	32
#define	KEY_5	33
#define	KEY_6	34
#define	KEY_7	35
#define	KEY_8	36
#define	KEY_9	37
#define	KEY_0	38
#define	KEY_PARA 39
#define	KEY_ESC 40
#define	KEY_ENT 41
#define	KEY_HOME 42
#define KEY_NONE 43
#define KEY_SS  44
#define KEY_RUN 45
#define KEY_SPACE 46

#ifdef __cplusplus
}
#endif

#endif
