/******************************************************************************
							Include File
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#include "TEECO_System.h"
#ifdef W5500
#include "T_socket.h"
#include "T_dhcp.h"
#include "T_loopback.h"
#include "DataLog.h"
#include "BSP_W5500.h"

	 
wiz_NetInfo gWIZNETINFO = { .mac = {0x01, 0x08, 0xdc,0x00, 0xab, 0xcd},
                            .ip = {192, 168, 0, 7},
                            .sn = {255,255,0,0},
                            .gw = {192, 168, 0, 1},
                            .dns = {0,0,0,0},
                            .dhcp = NETINFO_STATIC }; //NETINFO_STATIC , NETINFO_DHCP	 
volatile uint32_t msTicks; /* counts 1ms timeTicks */
uint32_t prevTick;


							
void platform_init(void)
{
//	uint16_t	addr;
//	uint8_t	data[32];
	//Dprintf("platform_init::W5500 BSP Init\n");
	
	BSP_W5500_Init();	//Port Init
	W5500_SoftReset();
	Delay(10);
	
	W5500_IntlevelTime();	
}
/******************************************************************************
 * @brief  Network Init
 * Intialize the network information to be used in WIZCHIP
 *****************************************************************************/
void network_init(void)
{
//	wiz_NetInfo netinfo;
	
	// Set Network information from netinfo structure
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);

#ifdef _MAIN_DEBUG_
	// Get Network information
	ctlnetwork(CN_GET_NETINFO, (void*)&netinfo);

	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);

	//if(netinfo.dhcp == NETINFO_DHCP) Dprintf("\r\n=== %s NET CONF : DHCP ===\r\n",(char*)tmpstr);
	//else Dprintf("\r\n%s NET CONFr\n",(char*)tmpstr);

	Dprintf("MAC: %d:%d:%d:%d:%d:%d\r\n",netinfo.mac[0],netinfo.mac[1],netinfo.mac[2],
    netinfo.mac[3],netinfo.mac[4],netinfo.mac[5]);
	Dprintf("SIP: %d.%d.%d.%d\r\n", netinfo.ip[0],netinfo.ip[1],netinfo.ip[2],netinfo.ip[3]);
	Dprintf("GAR: %d.%d.%d.%d\r\n", netinfo.gw[0],netinfo.gw[1],netinfo.gw[2],netinfo.gw[3]);
	Dprintf("SUB: %d.%d.%d.%d\r\n", netinfo.sn[0],netinfo.sn[1],netinfo.sn[2],netinfo.sn[3]);
	Dprintf("DNS: %d.%d.%d.%d\r\n", netinfo.dns[0],netinfo.dns[1],netinfo.dns[2],netinfo.dns[3]);
	//Dprintf("===========================\r\n");
#endif
}

/*******************************************************
 * @ brief Call back for ip assing & ip update from DHCP
 *******************************************************/
void my_ip_assign(void)
{
	Dprintf("	my_ip_assign::entry\n");
	
	getIPfromDHCP(gWIZNETINFO.ip);
	getGWfromDHCP(gWIZNETINFO.gw);
	getSNfromDHCP(gWIZNETINFO.sn);
	getDNSfromDHCP(gWIZNETINFO.dns);
	gWIZNETINFO.dhcp = NETINFO_DHCP;
	
	Dprintf("	my_ip_assign::entry---1\n");
	/* Network initialization */
	network_init();      // apply from dhcp
#ifdef _MAIN_DEBUG_
	Dprintf("DHCP LEASED TIME : %ld Sec.\r\n", getDHCPLeasetime());
#endif
	Dprintf("	my_ip_assign::end\n");
}

/************************************
 * @ brief Call back for ip Conflict
 ************************************/
void my_ip_conflict(void)
{
#ifdef _MAIN_DEBUG_
	Dprintf("CONFLICT IP from DHCP\r\n");
#endif
   //halt or reset or any...
   while(1); // this example is halt.
}

//W5500 SPI CS Select Enable
void  wizchip_select(void)
{
	GPIOA->BSRRH = GPIO_Pin_4; 
}

//W5500 SPI CS Select Disable
void  wizchip_deselect(void)
{
	GPIOA->BSRRL = GPIO_Pin_4; 
}

void  wizchip_write(uint8_t wb)
{
	LAN_SPI1_Send(wb);
}

uint8_t wizchip_read(void)
{
	return LAN_SPI1_Send(0xff);
}
  
void W5500_Init(void) {	
	Dprintf("W5500_INIT\n");
	//unsigned int txFlag = 0;
	//unsigned int errorTimer = 0;	
	//uint8_t tmp;
	uint8_t memsize[2][8] = { {8,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1}};
	//uint8_t my_dhcp_retry = 0;
    //unsigned int damCode = 0;
		
    platform_init(); //HW Port init
 	
	gWIZNETINFO.mac[0] = flash.serverMac[0];	
	gWIZNETINFO.mac[1] = flash.serverMac[1];
	gWIZNETINFO.mac[2] = flash.serverMac[2];
	gWIZNETINFO.mac[3] = flash.serverMac[3];
	gWIZNETINFO.mac[4] = flash.serverMac[4];
    gWIZNETINFO.mac[5] = flash.serverMac[5];

	gWIZNETINFO.ip[0] = flash.serverIp[0];
	gWIZNETINFO.ip[1] = flash.serverIp[1];
	gWIZNETINFO.ip[2] = flash.serverIp[2]; 
	gWIZNETINFO.ip[3] = flash.serverIp[3];

		
	Dprintf("IP   %d.%d.%d.%d\n",flash.serverIp[0],flash.serverIp[1],flash.serverIp[2],flash.serverIp[3]);	
		
	gWIZNETINFO.gw[0] = flash.serverGate[0];
	gWIZNETINFO.gw[1] = flash.serverGate[1];
	gWIZNETINFO.gw[2] = flash.serverGate[2];
	gWIZNETINFO.gw[3] = flash.serverGate[3];

	Dprintf("GATE %d.%d.%d.%d\n",flash.serverGate[0],flash.serverGate[1],flash.serverGate[2],flash.serverGate[3]);	
    Dprintf("PORT %d\n",flash.serverPort[0]);

	gWIZNETINFO.dns[0] = 0;
	gWIZNETINFO.dns[1] = 0;
	gWIZNETINFO.dns[2] = 0;
	gWIZNETINFO.dns[3] = 0;
	
	gWIZNETINFO.sn[0] = flash.serverMask[0];
	gWIZNETINFO.sn[1] = flash.serverMask[1];
	gWIZNETINFO.sn[2] = flash.serverMask[2];
	gWIZNETINFO.sn[3] = flash.serverMask[3];
	
	reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect); //개별함수 수정할것
    /* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);//개별함수 수정할것
    
	//Dprintf("Chip Init\n");
	/* wizchip initialize*/
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
    {
		Dprintf("WIZCHIP Initialized fail.\n");
		while(1);
    }		
	//Dprintf("Status Check\n");
#if 0
    /* PHY link status check */
    do
    {
		if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
			Dprintf("Unknown PHY Link stauts.\r\n");
		if(errorTimer >= 10000)
		{
			Dprintf("[W5500_Init]/* PHY link status check */\n");
			break;
		}
		else errorTimer++;
		
    }while(tmp == PHY_LINK_OFF);	
#endif
	
    /* Network initialization */
	//Dprintf("Network initialization\n");
    network_init(); // Static netinfo setting
	//Dprintf("getSn_TXBUF_SIZE = %d\n", getSn_TXBUF_SIZE(0));
	//Dprintf("getSn_RXBUF_SIZE = %d\n", getSn_RXBUF_SIZE(0));
}
#ifdef __cplusplus
}
#endif
#endif  //W5500
extern volatile U32	gDosScreen_id;
int AddWidgetString(U32 id, char *text);
void DebugMessageOut(char *buf) {
    if(gv.debugFlag==1) {//화면이 활성화되면
        int length  = strlen(buf);
        for(int i=0; i<length; i++) {
            Ysi_ReadData_Dos(*buf++);
        }
        Ysi_ReadData_Dos(0x0D);
        Ysi_ReadData_Dos(0x0A);
    } 
    else {
        //Dprintf("DEBUG :: %s\n", buf);
    }
}
int ModBusTcpIpSending(int slot) {
    float   mV=0.0F;
    U32     *addr;
    U8      a, b, c, d;
    void    *addr1; 
    addr1 = &mV;
    addr = addr1;
    int point = 0;
    // 1. 트랙잭션 아이디 , 마스터로 부터 수신한다.  2byte
    gv.transmitBuf[point++] = gv.lanRxBuf[slot][0];
    gv.transmitBuf[point++] = gv.lanRxBuf[slot][1];
    // 2. 프로토콜 아이디 0x0000 값으로 고정한다. 2byte
    gv.transmitBuf[point++] = 0x00;
    gv.transmitBuf[point++] = 0x00;
    // 3. 길이 length 이후부터 해당 프레임의 마지막까지의 길이 3byte
    gv.transmitBuf[point++] = 0x00;
    gv.transmitBuf[point++] = 0x1B;
    // 4. 유니트 아이디 1byte
    gv.transmitBuf[point++] = 0x01;//MODBUS ID
    // 5. 함수코드 1byte
    gv.transmitBuf[point++] = 0x04;//FUNCTION CODE
    
	gv.transmitBuf[point++] = 0x18;//측정값 6 * 4 = 24 = 0x18
    // 6. 데이터 Nbyte
    for(int i = 0; i < 6; i++)
    {
        //flash.Uart[USART_1]->SensorPV[i] = 100 +i;
        mV=flash.Uart[USART_1]->SensorPV[i];
                            
        a= (*addr>>24)&0xFF;     //MSB
        b= (*addr>>16)&0xFF; 
        c= (*addr>>8)&0xFF;  
        d= *addr & 0xFF;         //LSB
        
        gv.transmitBuf[point++] = a;
        gv.transmitBuf[point++] = b;
        gv.transmitBuf[point++] = c;
        gv.transmitBuf[point++] = d;
    }
    return point;
}
int W5500_Server(int slot) {//SERVER MODBUS TCP
    int32_t ret;
    uint16_t size = 0;
    uint8_t slotNumber=slot;
    int     sec = 15;//1분간 접속이 없는
    
    if(gv.lanCloseTimerCount[slotNumber]>=sec && gv.lanStatus[slotNumber]==1) { //IOProcessHardwareTimerEvent 에서 증가한다.
        //접속상태에서 sec 가 흐르면 접속을 해지한다.
        //송수신중에 sec 를 초기화한다.
        gv.lanStatus[slotNumber]=0;
        gv.lanCloseTimerCount[slotNumber]=0;
        close(slotNumber);
        DebugMessagePrintf("TCP SERVER :: CLOSE");
    }
    switch(getSn_SR(slotNumber))
    {
        case SOCK_ESTABLISHED :
        {
            if(getSn_IR(slotNumber) & Sn_IR_CON)
            {
                DebugMessagePrintf("TCP SERVER :: CONNECTED\n");
                setSn_IR(slotNumber,Sn_IR_CON);
                gv.lanStatus[slotNumber]=1;//접속됨
                gv.lanCloseTimerCount[slotNumber]=0;//1. 초기화
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //                          데이터 수신
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////            
            if((size = getSn_RX_RSR(slotNumber)) > 0)
            {
                gv.lanCloseTimerCount[slotNumber]=0;//2. 초기화
                for(int i=0; i<LAN_RXBUF_SIZE; i++) {
                    gv.lanRxBuf[slotNumber][i]=0;
                }
                if(size >= LAN_RXBUF_SIZE)//데이터 수신 크기 체크 
                {
                    Dprintf("TCP SERVER :: RECEIVE SIZE OVER ERROR = %d\n",size);
                    size = LAN_RXBUF_SIZE;//데이터길이 400이 넘어가면 400바이트 까지만 송신한다. (에러)
                }
                ret = recv(slotNumber,gv.lanRxBuf[slotNumber], size);
                if(ret <= 0) //ERROR 체크
                {
                    DebugMessagePrintf("TCP SERVER :: RECEIVE ERROR\n");
                    return ret;
                }
                DebugMessagePrintf("TCP SERVER :: RECEIVE OK\n");   
                //Dprintf("TCP SERVER :: RECEIVE OK\n");                
                gv.lanSendFlag[slotNumber]=1;
            }
            if(gv.lanSendFlag[slotNumber]==1) {
                gv.lanSendFlag[slotNumber]=0;
                sprintf(gv.titleBuf,"TCP/IP 20%02d/%02d/%02d %02d:%02d:%02d"
                , currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
                ret = send(slotNumber, gv.transmitBuf,ModBusTcpIpSending(slotNumber));
                if(ret < 0)
                {
                    DebugMessagePrintf("TCP SERVER :: TRANSMIT ERROR\n");
                } else {
                    DebugMessagePrintf("TCP SERVER :: SEND OK\n");
                }                    
            }
        }
        break;
        case SOCK_CLOSE_WAIT :
        {
            if((ret=disconnect(slotNumber)) != SOCK_OK) 
            {
                DebugMessagePrintf("TCP CLIENT :: CLOSE ERROR\n");
                return ret;
            }
            gv.lanStatus[slotNumber]=0; //접속상태해지
            gv.lanCloseTimerCount[slotNumber]=0; //접속해지타이머값
            DebugMessagePrintf("TCP CLIENT :: CLOSE");
        }
        break;
        case SOCK_INIT : 
        {
            if( (ret = listen(slotNumber)) != SOCK_OK) 
            {
                DebugMessagePrintf("TCP SERVER :: LISTEN ERROR\n");
                return ret;
            }
            DebugMessagePrintf("TCP SERVER :: LISTEN\n");
        }
        break;
        case SOCK_CLOSED:
        {
            if((ret=socket(slotNumber,Sn_MR_TCP,flash.serverPort[slotNumber],0x00)) != slotNumber)
            {
                DebugMessagePrintf("TCP SERVER :: SOCKET ERROR\n");
                return ret;
            }
            DebugMessagePrintf("TCP SERVER :: SOCKET CREATE\n");
        }
        break;
        default: break;
    }
    return 0;
}

#ifdef JSON_ENABLE
extern void Json_firmware_send_ack(void);
extern void Json_TxBufClear(void);
extern int Json_Parsing(int size);
int W5500_JsonServer(int slot) {
    int32_t ret=0;
    uint16_t size = 0;
    uint8_t slotNumber=slot;
    //CONNECTION_WAITING_TIME = 600SED
    if(gv.jsonTcpDebugFlag==0) { //디버그 명령을 수신하면 Close 하면 0 이된다.
        if(gv.lanCloseTimerCount[slotNumber]>=CONNECTION_WAITING_TIME && gv.lanStatus[slotNumber]==1) { //IOProcessHardwareTimerEvent 에서 증가한다.
            //접속상태에서 sec 가 흐르면 접속을 해지한다.
            //송수신중에 sec 를 초기화한다.
            gv.lanStatus[slotNumber]=0;
            gv.lanCloseTimerCount[slotNumber]=0;
            gv.lanStatus[slotNumber]=0; //접속상태해지
            gv.lanCloseTimerCount[slotNumber]=0; //접속해지타이머값
            gv.jsonTcpDebugFlag=0;
            gv.json.nextSendCmdNo=0;
            close(slotNumber);
            Dprintf("TCP SERVER :: CLOSE TIME OUT\n");
            DebugMessagePrintf("TCP SERVER :: CLOSE TIME OUT\n");
			ModemPowerControl(RELAY_1, OFF);
        }
    } else {
        
    }
    switch(getSn_SR(slotNumber))
    {
        case SOCK_ESTABLISHED :
        {
            if(getSn_IR(slotNumber) & Sn_IR_CON)
            {
                Dprintf("TCP SERVER :: CONNECTED\n");
                DebugMessagePrintf("TCP SERVER :: CONNECTED\n");
                setSn_IR(slotNumber,Sn_IR_CON);
                gv.lanStatus[slotNumber]=1;         //접속됨
                sprintf(gv.titleBuf,"TCP/IP 20%02d/%02d/%02d %02d:%02d:%02d"
                , currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
                gv.lanCloseTimerCount[slotNumber]=0;
            }
            if((size = getSn_RX_RSR(slotNumber)) > 0)
            {
                gv.lanCloseTimerCount[slotNumber]=0;
                Json_TxBufClear();
                ret = recv(slotNumber,gv.transmitBuf, size);
                if(ret <= 0) //ERROR 체크
                {
                    Dprintf("TCP SERVER :: RECEIVE ERROR\n");
                    return ret;
                }
                DebugMessagePrintf("TCP SERVER :: RECEIVE OK\n");   
                
                if(gv.json.nextSendCmdNo != 5) { //펌웨어업데이트 진행이 아니면 분석을 진행한다.
                    //Dprintf("TCP SERVER = %s\n",gv.transmitBuf);
                    Json_Parsing(size);
                } else {    
                    //PROGRAM UPDATE
					gv.downloadDataSize+=ret;				
					for(int i=0; i<ret; i++ ) {
						*(unsigned char *) gv.UpdateAddress = gv.transmitBuf[i];
						gv.UpdateAddress++;
					}				
					if(gv.downloadDataSize==gv.json.firmwareSize) {
						Dprintf("업데이트파일 사이즈 = %d addr = %08x\n", gv.downloadDataSize, (unsigned char *)gv.UpdateAddress);						
						Json_firmware_send_ack();
						send(slotNumber, gv.transmitBuf,strlen((char *)gv.transmitBuf));
						gv.downloadFlag=1;
						gv.json.firmwareSize=0;
                        gv.lanStatus[slotNumber]=0;
						gv.json.nextSendCmdNo=0;//수신종료                        
						//close(slotNumber);
					} 
                }
            }
            switch(gv.json.nextSendCmdNo) {
                case 0: {
                    
                }break;
                case 1: {   //DUMP 데이터송신
                            if(gv.json.sendingStep==3) {        //스텝1단계 헤더정보를 송신한다.   
                                Json_TxBufClear();                                
                                sprintf((char *)gv.transmitBuf,
                                "[{\"group\":\"%03d\",\"site\":\"%03d\",\"device\":\"%03d\",\"cmd_type\":\"%s\"},",
                                flash.json.groupSv,flash.json.siteSv,flash.json.diviceSv,"response_dump_data");
                                
                                send(slotNumber, gv.transmitBuf,strlen((char *)gv.transmitBuf));
                                
                                gv.json.sendingStep=2;
                                //Dprintf("송신파일크기[%d] = %s \n",gv.json.sendingSize,gv.transmitBuf);
                            } else if(gv.json.sendingStep==2) { //스텝2단계 1000바이트 씩 나누어서 데이터를 송신한다.
                                gv.json.oneSendingSizeCount=0;//1000바이트카운트
                                for(; gv.json.sendingCount < gv.json.sendingSize; gv.json.sendingCount++) {
                                    gv.transmitBuf[gv.json.oneSendingSizeCount] = *(unsigned char *)(JSON_RAM_ADDRESS);
                                    if(*(unsigned char *)(JSON_RAM_ADDRESS)!=']') {
                                        JSON_RAM_ADDRESS++;
                                        gv.json.oneSendingSizeCount++;								
                                        if(gv.json.oneSendingSizeCount==1000) {
                                            gv.lanCloseTimerCount[slotNumber]=0;	//Lan Close time 초기화                                            
                                            break;
                                        }
                                    } else {
                                        break;
                                    }
                                }			
                                if(gv.json.oneSendingSizeCount<1000) {
                                    gv.json.sendingStep=0;
                                    //Dprintf("마지막 길이 = %d\n",gv.json.oneSendingSizeCount);
                                    send(slotNumber, gv.transmitBuf,gv.json.oneSendingSizeCount+1);//DATA
                                    Dprintf("TCP SERVER :: TRANSMIT OK\n");
                                    DebugMessagePrintf("TCP SERVER :: TRANSMIT OK\n");
                                } else {
                                    send(slotNumber, gv.transmitBuf,gv.json.oneSendingSizeCount);//DATA
                                    Dprintf("TCP SERVER :: TRANSMIT OK\n");
                                    DebugMessagePrintf("TCP SERVER :: TRANSMIT OK\n");   
                                }						
                            }
                }break;
				case 2: { 	//ENQ 응답 송신데이터 송신
                            //Dprintf("ENQ 응답\n");
							send(slotNumber, gv.transmitBuf,strlen((char *)gv.transmitBuf));
                            Dprintf("TCP SERVER :: TRANSMIT OK\n");
                            DebugMessagePrintf("TCP SERVER :: TRANSMIT OK\n");
							gv.json.nextSendCmdNo=0;//송신완료
				}break;
                case 3: {   //서버 to RTU 파라메터 설정 응답
							send(slotNumber, gv.transmitBuf,strlen((char *)gv.transmitBuf));
                            Dprintf("TCP SERVER :: TRANSMIT OK\n");
                            DebugMessagePrintf("TCP SERVER :: TRANSMIT OK\n");
							gv.json.nextSendCmdNo=0;//송신완료
                }break;
                case 4: {   //펌웨어 업데이트 명령수신      
                            Dprintf("펌웨어업데이트명령수신\n");
							send(slotNumber, gv.transmitBuf,strlen((char *)gv.transmitBuf));
                            Dprintf("TCP SERVER :: TRANSMIT OK\n");
                            DebugMessagePrintf("TCP SERVER :: TRANSMIT OK\n");
							gv.json.nextSendCmdNo=5;
							gv.downloadDataSize=0;
							gv.UpdateAddress=SDREAD_RAM;
                }break;
                case 6: {
							send(slotNumber, gv.transmitBuf,strlen((char *)gv.transmitBuf));
                            Dprintf("TCP SERVER :: TRANSMIT OK\n");
                            DebugMessagePrintf("TCP SERVER :: TRANSMIT OK\n");
							gv.json.nextSendCmdNo=0;//송신완료
                }break;
                case 10: {  //SERVER CLOSE
                            gv.json.sendingStep=0;
                            gv.json.nextSendCmdNo=0;
                            gv.lanStatus[slotNumber]=0;
                            Dprintf("서버접속해제\n");
                            close(slotNumber);
                }break;
                case 11: {  //dump 데이터 없음
							send(slotNumber, gv.transmitBuf,strlen((char *)gv.transmitBuf));
                            Dprintf("TCP SERVER :: TRANSMIT OK\n");
                            DebugMessagePrintf("TCP SERVER :: TRANSMIT OK\n");
							gv.json.nextSendCmdNo=0;//송신완료
                }break;
                case 25: {
							send(slotNumber, gv.transmitBuf,strlen((char *)gv.transmitBuf));
                            Dprintf("TCP SERVER :: TRANSMIT OK\n");
                            DebugMessagePrintf("TCP SERVER :: TRANSMIT OK\n");
							gv.json.nextSendCmdNo=0;//송신완료
                }
                default : break;
            }
        } 
        break;
        case SOCK_CLOSE_WAIT :
        {
            if((ret=disconnect(slotNumber)) != SOCK_OK) 
            {
                Dprintf("TCP CLIENT :: CLOSE ERROR\n");
                return ret;
            }
            //Dprintf("TCP CLIENT :: CLOSE\n");
            gv.lanStatus[slotNumber]=0; //접속상태해지
            gv.lanCloseTimerCount[slotNumber]=0; //접속해지타이머값
            gv.jsonTcpDebugFlag=0;
            gv.json.nextSendCmdNo=0;
            Dprintf("TCP CLIENT :: CLOSE\n");
            DebugMessagePrintf("TCP CLIENT :: CLOSE\n");
			ModemPowerControl(RELAY_1, OFF);
        }
        break;
        case SOCK_INIT : 
        {
            if( (ret = listen(slotNumber)) != SOCK_OK) 
            {
                Dprintf("TCP SERVER :: LISTEN ERROR\n");
                return ret;
            }
            Dprintf("TCP SERVER :: LISTEN\n");
            DebugMessagePrintf("TCP SERVER :: LISTEN\n");
        }
        break;
        case SOCK_CLOSED:
        {
            if((ret=socket(slotNumber,Sn_MR_TCP,flash.serverPort[slotNumber],0x00)) != slotNumber)
            {
                Dprintf("TCP SERVER :: SOCKET ERROR\n");
                return ret;
            }
            Dprintf("TCP SERVER :: SOCKET CREATE\n");
            DebugMessagePrintf("TCP SERVER :: SOCKET CREATE\n");
        }
        break;
        default: break;
    }
    return 0;
}
#endif
