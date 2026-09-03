/**
  ******************************************************************************
  * @file       HLK_RM04_Wifi.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월31일 ~ 2015년 1월3일
  * @brief    이파일은  HLK_RM04 Wifi Module 에 대한 기능함수이다.
                
  *          
  * @attention
  * 이파일은 자이언텍 지적재산권이므로 타사에 소스를 제공해서는 안됩니다. 
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc.</center></h2>
  * @warning  
  * @verbatim
  *  Edit History
  * @endverbatim  
  ******************************************************************************  
  */
//!< 와이파이 모듈에 명령을지시할때는 뒤에 0x0d '\r'을 송신하면
//!< 와이파이 모듈은 0x0d 0x0a로 행을바꾸고 응답의 끝에도  0x0d 0x0a로 응답된다.

//!< 와이파이 모듈은 전원인가후 30초정도가 흐른후에 응답을한다.
//!< 하드웨어단자의 ES/RST핀을 100msec ~ 6sec 이내에 Low 로 하면 AT Command Mode로 진입하고
//!< 6초이상 누르면 공장설정모드로  설정된다.
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"

/* Private typedef -----------------------------------------------------------*/
#define			WIFI_Putc						Uart_SendByte1
#define			WIFI_Getc						Uart_Getch1
#define			WIFI_Printf						Uart_Printf1
#define			WIFI_Dstatus					Uart_Status1

//!< 와이파이모듈처리 함수의 에러를 구분한다.
#define			PARAMETER_MISTAKE		10
#define			FUNCTION_SUCESS			11

//USART1 의 데이터 타임아웃시간
#define			COMM_TIMEOUT			300

// default Tes time should be 100 ms
#define 			TES_TIME_IN_MS				100

// defualt Tpt time should be 10ms
#define 			TPT_TIME_IN_MS				10

//!< Network Mode의 구분
#define			WIFI_ETHERNET_MODE		1
#define			WIFI_CLIENT_MODE			2
#define			WIFI_AP_MODE				3

/**!< IP TYPE 의 구분 */
#define			DHCP							1
#define			STATIC							2

#define			NONE_PROTOCOL			0
#define			TCP_PROTOCOL				1
#define			UDP_PROTOCOL				2

//!< Mode 지정시사용
#define			NONE_MODE					0
#define			CLIENT_MODE				1
#define			SERVER_MODE				2

//!< Network Protocol 의 구분
#define			TCP_SERVER					1
#define			TCP_CLIENT					2
#define			UDP_SERVER					3
#define			UDP_CLIENT					4

//!< Server Enable Status
#define			AUTOENABLE					1
#define			OPEN							1
#define			CLOSE							0

//!< ENC TPYE 의구분(참조용)
#define			none 							1		//!< Open network
#define			wep_open 						2		//!< Wep encryption，open authentication method
#define			wep 								3		//!< Wep encryption，encryption authentication
#define			wpa_tkip 						4		//!< wpa tkip
#define			wpa_aes 						5		//!< wpa aes
#define			wpa2_tkip 						6		//!< wpa2 tkip
#define			wpa2_aes 						7		//!< wpa2 aes
#define			wpawpa2_tkip 				8		//!< wpa/wpa2 tkip

typedef struct __Wifi
{
	int		atmode;										//!< 모듈이 전송모드인지 AT 모드인지를 나타낸다.
	int		stable_time;								//!< 모듈은 30초이상 경과해야 응답을한다.

	int		network_mode;							//!< Client, AP , Ethernet 인지의 구분	
	int		ip_type;										//!< DHCP 인지 Static 인지를 구분한다.
	int		dhcp_server;								//!< AP Mode에서 DHCP서버를 사용할지를 지정
	
	uint8_t	ip[4];
	uint8_t	subnetmask[4];
	uint8_t	gateway[4];
	uint8_t	dns_1[4];
	uint8_t	dns_2[4];
	uint16_t	port;
	
	int		network_protocol;						//!< TCP / UDP 의 구분, Server, Client 로 4가지의 구분
	int		timeout;										//!< Netwotk Time의 지정
	int		tcp_auto;
	uint8_t	remote_ip[4];
	uint16_t	remote_port;
	
	char		enc_type[32];									//!< 암호화방법
	char		ssid[64];										//!< SSID
	char		password[64];								//!< 비밀번호
	int		channel;										//!< 공유기의 채널번호
	
	int		serial_framming_length;
	int		serial_framming_timeout;
	int		rts;
	
	void		(*SerialInit)(void);							//!< 모듈에 연결할 통신포트를 설정한다.
	int		(*ATModeEntry)(void);					//!< AT Mode로 진입하는 함수이다. atmode를 수정한다.
	void		(*Serial_TX_Int_Enable)(void);			//!< 통신포트의 송신인터럽트를 허가한다.
	void		(*Serial_TX_Int_Disable)(void);		//!< 통신포트의 송신인터럽트를 금지한다.
	void		(*Serial_RX_Int_Enable)(void);			//!< 통신포트의 수신인터럽트를 허가한다.
	void		(*Serial_RX_Int_Disable)(void);		//!< 통신포트의 수신인터럽트를 금지한다.
	void		(*Putc)(char sd);
	uint8_t	(*Getc)(void);
	void 		(*Printf)(char *fmt,...);
	uint8_t	(*Status)(void);
	
}__attribute__((packed)) WIFI_HLK_RM04;

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern volatile int  g_Wifi_Time;
extern volatile int  g_Wifi_TimeOut;

char	WIFI_COMM_BUF[1024];		//!< 인터럽트 수신버퍼
int	wifi_com_count=0;				//!< 인터럽트 수신카운트	
int	wifi_command_parsing=0;		//!< 분석시작플래그

/* Private function prototypes -----------------------------------------------*/
void 	Uart1_Config(void);
int 	ATModeEntry(void);
void 	Serial_TX_Int_Disable(void);
void 	Serial_TX_Int_Enable(void);
void 	Serial_RX_Int_Disable(void);
void 	Serial_RX_Int_Enable(void);

/* Private functions ---------------------------------------------------------*/
WIFI_HLK_RM04	WIFI_RM04 = 
{
	.atmode = { 0 },
	.stable_time = { 0 },
	.network_mode = { WIFI_CLIENT_MODE  },
	.ip_type = { DHCP },
	.ip = { 192, 168, 0, 10 },
	.subnetmask = {255, 255, 255, 0 },
	.gateway = { 192, 168, 0, 1 }, 
	.dns_1 = { 0, 0, 0, 0 },
	.dns_2 = { 0, 0, 0, 0 },
	.port = { 5678 },
	.network_protocol = { TCP_CLIENT },
	.remote_ip = { 0, 0, 0, 0 },
	.remote_port = { 5678 },
	.enc_type = { "none" },
	.ssid = {"iptime je"},
	.password = {"12345678"},
	.channel = 3,
	.serial_framming_length = 64,
	.serial_framming_timeout = 0,
	
	.SerialInit = Uart1_Config,
	.ATModeEntry = ATModeEntry,
	.Serial_TX_Int_Enable = Serial_TX_Int_Enable,
	.Serial_TX_Int_Disable = Serial_TX_Int_Disable,
	.Serial_RX_Int_Enable = Serial_RX_Int_Enable,
	.Serial_RX_Int_Disable = Serial_RX_Int_Disable,
	.Putc = WIFI_Putc,
	.Getc = WIFI_Getc,
	.Printf = WIFI_Printf,
	.Status = WIFI_Dstatus
};
WIFI_HLK_RM04	*pWIFI_RM04 = &WIFI_RM04;

/******************************************************************************
	@brief	와이파이 통신포트의 송신인터럽트를 금지시킨다.
*******************************************************************************/
void Serial_TX_Int_Disable(void)
{
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
}

/******************************************************************************
	@brief	와이파이 통신포트의 송신인터럽트를 허가시킨다.
*******************************************************************************/
void Serial_TX_Int_Enable(void)
{
	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
}

/******************************************************************************
	@brief	와이파이 통신포트의 수신인터럽트를 금지시킨다.
*******************************************************************************/
void Serial_RX_Int_Disable(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
}

/******************************************************************************
	@brief	와이파이 통신포트의 수신인터럽트를 허가시킨다.
*******************************************************************************/
void Serial_RX_Int_Enable(void)
{
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

/******************************************************************************
	@brief 	HLK_RM04 와이파이 모듈을 AT Command 모드로 설정한다.
*******************************************************************************/
int ATModeEntry(void)
{
	if(pWIFI_RM04->stable_time==0)
	{
		Dprintf("ATModeEntry:: wait about 30 seconds, After starting. %d\n", g_Wifi_Time);
		return(0);
	}
	Delay(500);
	pWIFI_RM04->Putc('+'); 	
	Delay(TPT_TIME_IN_MS*2);
	pWIFI_RM04->Putc('+'); 	 
	Delay(TPT_TIME_IN_MS*2);
	pWIFI_RM04->Putc('+');
	
	Delay(500);
	
	pWIFI_RM04->Putc(0x1b); 	
	Delay(TPT_TIME_IN_MS*2);
	pWIFI_RM04->Putc(0x1b); 	
	Delay(TPT_TIME_IN_MS*2);
	pWIFI_RM04->Putc(0x1b);
	Delay(TPT_TIME_IN_MS*2);
	
	pWIFI_RM04->Putc(0x0d);		//!< \r Code
	pWIFI_RM04->Putc(0x0d);		//!< \r Code
	Delay(100);
	if(pWIFI_RM04->Status()==1) 	WIFI_RM04.atmode = 1;
	else  	WIFI_RM04.atmode = 0;
	return (1);
}

/******************************************************************************
	@brief 	HLK_RM04 와이파이 모듈수신 인터럽트서비스루틴
*******************************************************************************/
void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
		g_Wifi_TimeOut=0;
        char data = (USART_ReceiveData(USART1) & 0x7F);
		if(pWIFI_RM04->atmode==1)
		{
			if(wifi_com_count < 1024)
				WIFI_COMM_BUF[wifi_com_count++] = data;		
			else
				wifi_com_count=0;
		}
		else
			Dputc(data);
    }
    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    {   
        /* Write one byte to the transmit data register */
        //if(TxCounter == NbrOfDataToTransfer)
        {
            /* Disable the EVAL_COM1 Transmit interrupt */
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
        }
    }	
}

// 와이파이의 파라메터를 설정한다.
int Set_WirelessParameter(char *ssid, char *enc_type, char *password)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		strcpy(pWIFI_RM04->ssid, ssid);
		strcpy(pWIFI_RM04->enc_type, enc_type);
		strcpy(pWIFI_RM04->password, password);
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+wifi_conf=%s,%s,%s\r", ssid, enc_type, password);
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_WirelessParameter:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_WirelessParameter:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_WirelessParameter:: not ATCommand mode\n");
	return(0);	
}

// 와이파이 모듈을 AT command모드로 진입시킨다.
int Set_ATCommnadMode(void)
{
	if(pWIFI_RM04->atmode==0) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		pWIFI_RM04->Serial_RX_Int_Disable();		//인터럽트를 금지시킨다.
		if(pWIFI_RM04->ATModeEntry()==1)
		{
			wifi_com_count=0;	//!< 통신카운터 클리어
			if(pWIFI_RM04->atmode==1)  Dprintf("Set_ATCommnadMode::AT Command Mode Entry sucess=%d\n", g_Wifi_Time); 
			else Dprintf("Set_ATCommnadMode::AT Command Mode Entry fail!\n");
		}
		pWIFI_RM04->Serial_RX_Int_Enable();		//인터럽트를 허가한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		if(pWIFI_RM04->atmode==1) return(1);
	}
	else
		Dprintf("Set_ATCommnadMode:: not ATCommand mode\n");
	return(0);

}

// 와이파이 모듈을 클라이언트, AP, Ethernet 모드로 설정한다.
int Set_NetworkMode(int mode)
{
	if(pWIFI_RM04->atmode==1) 	//!< ATMode 일때만 진입한다.
	{		
		wifi_com_count=0;	//!< 통신카운터 클리어
		if(mode==WIFI_ETHERNET_MODE)	pWIFI_RM04->Printf("at+netmode=1\r");
		else if(mode==WIFI_CLIENT_MODE)	pWIFI_RM04->Printf("at+netmode=2\r");
		else if(mode==WIFI_AP_MODE)	pWIFI_RM04->Printf("at+netmode=3\r");
		else
		{
			Dprintf("Set_NetworkMode::parameter error mode=%d\n", mode);
			return(0);
		}
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);

		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_NetworkMode:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_NetworkMode:: Fail\n");
			return(0);			
		}
	}
	else
		Dprintf("Set_NetworkMode:: not ATCommand mode\n");
	return(0);
}

// 와이파이의 통신채널을 설정한다.
int Set_Channel(int ch)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		if(ch < 0 && ch > 14) 
		{
			Dprintf("Set_Channel::over range ch=%d\n", ch);
			return(0);
		}
		//!< 구조체에 대입한다.
		pWIFI_RM04->channel = ch;
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+Channel=%d\r", ch);
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);

		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_Channel:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_Channel:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_Channel:: not ATCommand mode\n");
	return(0);	
}

//!< 와이파이의 IP 타입을 설정한다.
//!< 매개변수는 STATIC , DHCP 둘둥의 하나
//!< 성공하면 1, 실패하면 0을 리턴한다.
int Set_IPType(int type)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->ip_type = type;
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		if(type==DHCP) pWIFI_RM04->Printf("at+dhcpc=1\r");
		else if(type==STATIC) pWIFI_RM04->Printf("at+dhcpc=0\r");
		else
		{
			Dprintf("Set_IPType::parameter error\n");
		}
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_IPType:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_IPType:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_IPType:: not ATCommand mode\n");
	return(0);	
}

//!< 와이파이의 IP 를 설정한다.
//!< 매개변수는 자신의 IP, SUBnetmask, Gateway를 입력한다.
//!< 성공하면 1, 실패하면 0을 리턴한다.
int Set_IP(char *ip, char *subnetmask, char *gateway)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		inet_pton4(ip, pWIFI_RM04->ip);
		inet_pton4(subnetmask, pWIFI_RM04->subnetmask);
		inet_pton4(gateway, pWIFI_RM04->gateway);
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+net_ip=%s,%s,%s\r", ip, subnetmask, gateway);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_IP:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_IP:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_IP:: not ATCommand mode\n");
	return(0);	
}

int Set_DNS(char *dns1, char *dns2)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		inet_pton4(dns1, pWIFI_RM04->dns_1);
		inet_pton4(dns2, pWIFI_RM04->dns_2);
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+net_dns=%s,%s\r", dns1, dns2);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_DNS:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_DNS:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_DNS:: not ATCommand mode\n");
	return(0);	
}

//status : OPEN, CLOSE
int Set_DHCPServer(int status)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->dhcp_server = status;
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+dhcpd=%d\r", status);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_DHCPServer:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_DHCPServer:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_DHCPServer:: not ATCommand mode\n");
	return(0);	
}

//!< 설정한 상태를 저장한다.
int Set_ConfigSave(void)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+net_commit=1\r");
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_ConfigSave:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_ConfigSave:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_ConfigSave:: not ATCommand mode\n");
	return(0);	
}

//!< 명령을 분석할때 사용할수 있는 방법을 예시한다.
int WIFI_Parsing(void)
{
	//!< AT Command 모드일때 명령을날리고 타임아웃이 지나면  응답을 분석한다.
	if(pWIFI_RM04->atmode==1 && g_Wifi_Time>100 && wifi_command_parsing==1)
	{			
		WIFI_COMM_BUF[wifi_com_count]=0;
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("ok search ok\n");
		}
		Dprintf("%s cnt=%d", WIFI_COMM_BUF, wifi_com_count);
		char	buf[128];
		int cnt=StringSeparator(WIFI_COMM_BUF, buf, 0x0a, 0x0d);
		
		for(int i=0; i<cnt; i++)
			Dprintf("%02x ", buf[i]);
		Dprintf("\nString=%s\n", buf);
		
		wifi_com_count=0;
		wifi_command_parsing=0;
		return(1);
	}			
	return(0);
}

int Set_RemoteIP(char *ip)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		inet_pton4(ip, pWIFI_RM04->remote_ip);
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+remoteip=%s\r", ip);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_RemoteIP:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_RemoteIP:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_RemoteIP:: not ATCommand mode\n");
	return(0);	
}

int Set_RemotePort(uint16_t port)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->remote_port = port;
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+remoteport=%d\r", port);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_RemotePort:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_RemotePort:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_RemotePort:: not ATCommand mode\n");
	return(0);	
}
/**
pro 는 다음중 한가지를 사용한다.
#define			NONE_PROTOCOL			0
#define			TCP_PROTOCOL				1
#define			UDP_PROTOCOL				2
*/
int Set_RemoteProtocol(int pro)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->network_protocol = pro;
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		if(pro == NONE_PROTOCOL)
			pWIFI_RM04->Printf("at+remotepro=none\r");
		else if(pro == TCP_PROTOCOL)
			pWIFI_RM04->Printf("at+remotepro=tcp\r");
		else if(pro == UDP_PROTOCOL)
			pWIFI_RM04->Printf("at+remotepro=udp\r");
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_RemoteProtocol:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_RemoteProtocol:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_RemoteProtocol:: not ATCommand mode\n");
	return(0);	
}

int Set_NetworkTimeOut(int time)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->timeout = time;
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+timeout=%d\r", time);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_NetworkTimeOut:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_NetworkTimeOut:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_NetworkTimeOut:: not ATCommand mode\n");
	return(0);	
}

//!< The conversion mode setting
//!< Mode 지정시사용
/**
#define			NONE_MODE					0
#define			CLIENT_MODE				1
#define			SERVER_MODE				2
*/
int Set_Mode(int mode)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->timeout = mode;
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		if(mode==NONE_MODE) pWIFI_RM04->Printf("at+mode=none\r");
		else if(mode==CLIENT_MODE) pWIFI_RM04->Printf("at+mode=client\r");
		else if(mode==SERVER_MODE) pWIFI_RM04->Printf("at+mode=server\r");
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_Mode:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_Mode:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_Mode:: not ATCommand mode\n");
	return(0);	
}

int Set_UartPackLen(int len)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->serial_framming_length = len;
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+uartpacklen=%d\r", len);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_UartPackLen:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_UartPackLen:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_UartPackLen:: not ATCommand mode\n");
	return(0);	
}

int Set_UartPackTimeOut(int time)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->serial_framming_timeout = time;
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+uartpacktimeout=%d\r", time);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_UartPackTimeOut:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_UartPackTimeOut:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_UartPackTimeOut:: not ATCommand mode\n");
	return(0);	
}

//!< TCP Protocol이 끊어졌을때 재접속을 할지의 여부를 지정한다.
/**
#define			AUTOENABLE					1
#define			CLOSE							0
*/
int Set_TCPAuto(int status)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->tcp_auto = status;
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+tcp_auto=%d\r", status);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_TCPAuto:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_TCPAuto:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_TCPAuto:: not ATCommand mode\n");
	return(0);	
}

int Set_RTS(int status)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 구조체에 대입한다.
		pWIFI_RM04->rts = status;
		
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("at+RTS=%d\r", status);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		wifi_com_count=0;	//!< 통신카운터 클리어
		//!< 수신된 문자열중 ok 가 있는지 검색한다.
		if(StringSearch(WIFI_COMM_BUF, "ok")==1)
		{
			Dprintf("Set_RTS:: OK\n");
			return(1);
		}		
		else
		{
			Dprintf("Set_RTS:: Fail\n");
			return(0);			
		}		
	}	
	else
		Dprintf("Set_RTS:: not ATCommand mode\n");
	return(0);	
}



#define		COMMAND		1
#define		QUERY				2
#define		ALL					3

int Set_UserCommand(char *command, int display, int mode, char start, char end)
{
	if(pWIFI_RM04->atmode==1) 	//!< atmode 에 진입되어있지 않을때 시도한다.
	{
		//!< 모듈에 전송한다.
		wifi_com_count=0;	//!< 통신카운터 클리어
		pWIFI_RM04->Printf("%s\r", command);
		
		//!< 타임아웃동안 대기한후
		g_Wifi_TimeOut=0;
		while(g_Wifi_TimeOut <COMM_TIMEOUT);
		//!< 문자열처리를 위해 널문자삽입
		WIFI_COMM_BUF[wifi_com_count]=0;		
		//디버깅을 위해 수신된 문자를 출력한다.
		if(display==1)
		{
			Dprintf("\tbuf=");
			for(int i=0; i<wifi_com_count; i++)
			{
				if(WIFI_COMM_BUF[i] >= 0x20 && WIFI_COMM_BUF[i] <=0x7f) Dputc(WIFI_COMM_BUF[i]);
				else Dprintf("[%02x] ", WIFI_COMM_BUF[i]);
			}
			Dprintf("\n");
		}
		wifi_com_count=0;	//!< 통신카운터 클리어
		if(mode==COMMAND || mode==ALL)
		{
			//!< 수신된 문자열중 ok 가 있는지 검색한다.
			if(StringSearch(WIFI_COMM_BUF, "ok")==1)
			{
				Dprintf("\tSet_UserCommand:: OK\n");
				//return(1);
			}		
			else
			{
				Dprintf("\tSet_UserCommand:: Fail\n");
				//return(0);			
			}		
		}
		if(mode==QUERY || mode==ALL)
		{
			char	buf[128];
			int cnt=StringSeparator(WIFI_COMM_BUF, buf, 0x0a, 0x0d);
			//Dputc('\t');
			//for(int i=0; i<cnt; i++)	Dprintf("%02x ", buf[i]);
			Dprintf("\n\tQuery String[len=%d] %s\n", cnt, buf);		
			if(cnt != 0)	return(1);
			return(0);
		}
	}	
	else
		Dprintf("\tSet_UserCommand:: not ATCommand mode\n");
	return(0);	
}

void HLK_RM04_Setting(void)
{
	while(!Set_NetworkMode(WIFI_CLIENT_MODE)); //WIFI_AP_MODE, WIFI_CLIENT_MODE
	while(!Set_WirelessParameter("iptime je", "wpa2_aes", "thddmswp"));
	while(!Set_Channel(12));
	while(!Set_IPType(STATIC)); //STATIC, DHCP
	while(!Set_IP("192.168.0.12", "255.255.0.0", "192.168.0.1"));
	while(!Set_DNS("1.214.68.2", "61.41.153.2"));

	while(!Set_RemoteIP("192.168.0.2"));
	while(!Set_RemotePort(5678));
	while(!Set_RemoteProtocol(TCP_PROTOCOL));
	while(!Set_NetworkTimeOut(0));	//!< 0이면 타임아웃이 지정되지 않는다.
	while(!Set_Mode(CLIENT_MODE)); //CLIENT_MODE, SERVER_MODE
	while(!Set_TCPAuto(AUTOENABLE));	//!< 자동접속할지의 여부를 지정한다. CLOSE, AUTOENABLE
	
	while(!Set_UartPackLen(64));		//bit
	while(!Set_UartPackTimeOut(10)); //!< msec 
	while(!Set_RTS(0));
	
	//Set_ConfigSave();		//!< 위의상태를 저장한다. 저장시 시간이 걸리기때문에 끝났는지를 체크한다.
	Set_UserCommand("at+save=1", 0, COMMAND, 0x0a, 0x0d);	//!< Restart serial transformation service
	//Set_UserCommand("at+reconn=1", 0, COMMAND, 0x0a, 0x0d);	//!< Restart serial transformation service
	pWIFI_RM04->atmode=0;
}

// 와이파이 모듈이 전원인가후 30초가 지나면 플래그를 체크한다.
// 자동으로 ATCommand 모드로 진입한다.
int WIFI_TimeCheck(void)
{
	if(pWIFI_RM04->stable_time == 0) //전원인가후 30초가 경과된것을 체크한다.
	{
		if(g_Wifi_Time > 30000)
		{
			pWIFI_RM04->stable_time=1;
			//Set_ATCommnadMode();
			//HLK_RM04_Setting();
			return(1);
		}
	}	
	return(0);
}


void HLK_RM04_Init(void)
{
	wifi_com_count=0;
	pWIFI_RM04->SerialInit();						//!< 통신포트를 초기화시킨다.
	pWIFI_RM04->Serial_RX_Int_Enable();		//!< 통신포트의 수신인터럽트를 허가시킨다.
	pWIFI_RM04->Serial_TX_Int_Disable();		//!< 통신포트의 송신 인터럽트를 금지시킨다.
}

int UserInput(void)
{
	char	sbuf[128];
	int	cnt=0;
	pWIFI_RM04->atmode=1;
	while(1)		
	{
		if(Dstatus())
		{			
			char sd = Dgetch();
			if(sd==0x0d)
			{
				if(cnt!=0)
				{
					Dputc('\n');
					sbuf[cnt]=0;
					//Dprintf(">%s\n", sbuf);
					Set_UserCommand(sbuf, 1, ALL, 0x0a, 0x0d);
					cnt=0;
				}
				else Dputc('\n');
			}
			else if(sd==0x1b)
			{
				if(cnt==0) return(1);
				for(int i=0; i<cnt; i++) Dputc('\b');
				for(int i=0; i<cnt; i++) Dputc(' ');
				for(int i=0; i<cnt; i++) Dputc('\b');
				cnt=0;
			}
			else if(sd==' ')
			{
				if(cnt==0)
				{
					Set_ATCommnadMode();					
				}
			}
			else
			{
				if(cnt >= 128) cnt=0;
				sbuf[cnt++] = sd;
				Dputc(sd);
			}
		}
	}	
}

void HLK_RM04_Test(void)
{
//	char	sbuf[128];
//	int	cnt=0;
	HLK_RM04_Init();
	
	Dprintf("HLK_RM04 Wifi Module AT Command Test \n");
	while(1)		
	{
		if(Dstatus())
		{			
			char sd = Dgetch();
			if(sd=='1') 	Set_ATCommnadMode();
			else if(sd=='2') HLK_RM04_Setting();
			else if(sd=='3') UserInput();
			else if(sd=='4') pWIFI_RM04->atmode=0;
			else pWIFI_RM04->Putc(sd);

			/*
			if(sd==0x0d)
			{
				if(cnt!=0)
				{
					Dputc('\n');
					sbuf[cnt]=0;
					//Dprintf(">%s\n", sbuf);
					Set_UserCommand(sbuf, 1, ALL, 0x0a, 0x0d);
					cnt=0;
				}
				else Dputc('\n');
			}
			else if(sd==0x1b)
			{
				for(int i=0; i<cnt; i++) Dputc('\b');
				for(int i=0; i<cnt; i++) Dputc(' ');
				for(int i=0; i<cnt; i++) Dputc('\b');
				cnt=0;
			}
			else if(sd==' ')
			{
				if(cnt==0)
				{
					Set_ATCommnadMode();					
				}
			}
			else
			{
				if(cnt >= 128) cnt=0;
				sbuf[cnt++] = sd;
				Dputc(sd);
			}
			*/
		}
		WIFI_TimeCheck();	//!< 와이파이 모듈이 전원인가후 30초가 지났는지를 검사한다.
	}	
}
#ifdef __cplusplus
}
#endif

