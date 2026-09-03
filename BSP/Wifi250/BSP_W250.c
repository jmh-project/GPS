/******************************************************************************
							Include File
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#include "TEECO_System.h"

#ifdef WIZFI250_USE
/* Private define ------------------------------------------------------------*/ 
#define		Wprintf		Uart_Printf1
#define		Wgetch		Uart_Getch1
#define		Wstatus 	    Uart_Status1
#define		WgetIntNum	Uart_GetIntNum1
#define		WgetString	Uart_GetString1
#define		Wputc		Uart_SendByte1
//=====================================================
#define      COM_MAX_LEN    512
#define      WIFI_TIMEOUT     100 


//---------------- Wifi 상태를 정의한다.------------------
#define     WIFI_STANDBY               0
#define     WIFI_OK                      1
#define     WIFI_ERROR                2
#define     INVALD_INPUT            3
#define     INVALD_SOCKET_ID      4
#define     ERR_WIFI                   5
#define     ERR_MODE                6
#define     CONNECT                 7
#define     DISCONNECT             8
#define     LINK_UP                   9
#define     LINK_DOWN              10
#define     SYSTEM_RESET            11
#define     NOT_ACK                   12


volatile char   RxBuffer[COM_MAX_LEN];
volatile int     RxCounter=0;
volatile char   ParsingBuffer[COM_MAX_LEN];
volatile int     ParsingCounter=0;

volatile int     gParsing=0;

extern volatile int  g_Wifi_Time;
volatile int  g_Wifi_Time_Check;
volatile int  g_Wifi_Status;
volatile int  g_Wifi_Command;
volatile int  g_Wifi_Group_Command;

//=====================================================
void Wifi_Uart1_Config(void)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
  
    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
    /* Enable USART clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
    /* Connect USART pins to AF7 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
  
    /* Configure USART Tx and Rx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Enable the USART OverSampling by 8 */
    //USART_OverSampling8Cmd(UART4, ENABLE);  //오버샘플링할때만 사용한다.

    /* USARTx configuration ----------------------------------------------------*/
    /* USARTx configured as follow:
        - BaudRate = 5250000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 8
		     is: (USART APB Clock / 8) 
			 Example: 
			    - (USART3 APB1 Clock / 8) = (42 MHz / 8) = 5250000 baud
			    - (USART1 APB2 Clock / 8) = (84 MHz / 8) = 10500000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 16
		     is: (USART APB Clock / 16) 
			 Example: (USART3 APB1 Clock / 16) = (42 MHz / 16) = 2625000 baud
			 Example: (USART1 APB2 Clock / 16) = (84 MHz / 16) = 5250000 baud
        - Word Length = 8 Bits
        - one Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
    */ 
    USART_InitStructure.USART_BaudRate = 115200;    //5250000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
  
    /* NVIC configuration */
    /* Configure the Priority Group to 2 bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
    
    /* Enable USART */
    USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)
{
    int       i;
    char    data;
    static int start_flag=0;
    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        // Read one byte from the receive data register 
        data = (USART_ReceiveData(USART1) & 0x7F);
        //Dputc(data);
        if(data=='[')
        {
            start_flag=1;
        }
        if(start_flag==1)
        {
            RxBuffer[RxCounter++] = data;
            if(RxCounter == COM_MAX_LEN)
            {
                RxCounter = 0;
            }
            if(data==']')
            {
                for(i=0; i<RxCounter; i++)
                {
                    ParsingBuffer[i] = RxBuffer[i];
                }
                ParsingCounter = RxCounter;
                gParsing=1;
                start_flag=0;
                RxCounter=0;                
            }            
        }
    }
}

void TimeOutStart(void)
{
    g_Wifi_Time = 0;
    g_Wifi_Time_Check=1;
}

#define     COM_TERMINAL_CHECK      1
#define     COM_STANDBY                 2
#define     COM_JOIN                      3
#define     COM_SERVER                      4

void Wifi_Test(void)
{
    char    sd;
    char    buf[128];
    int      i;
    int      process_no=0;

    Wifi_Uart1_Config();        //Wifi용 통신포트의 초기화
    g_Wifi_Status = WIFI_STANDBY;
    g_Wifi_Group_Command = COM_STANDBY;
    
    Dprintf("Wifi250 Test Start________________\n");
    while(1)
    {
        //테스트용 명령전달
        if(Dstatus())
        {
            sd = Dgetch();
            if(sd=='a')
            {
                Dprintf("Server Connect Start------->\n");
                g_Wifi_Group_Command = COM_SERVER;
                process_no = 0;                
            }
            else if(sd=='1') //터미널체크 명령인 "AT" 를 송신한다.
            {
                Dprintf("AT Command Out------->");
                Wprintf("AT\r");
                g_Wifi_Command=COM_TERMINAL_CHECK; //현재 와이파이 모듈에 터미널 체크명령을 송신했음을 기록한다.
                TimeOutStart(); //와이파이의 응답시간제한을 위하여
            } 
            else if(sd=='2')
            {
                Dprintf("AT+WSET=0,Song,,5------->");                        
                g_Wifi_Status=WIFI_STANDBY; 
                Wprintf("AT+WSET=0,Song,,5\r"); 
                g_Wifi_Command=COM_TERMINAL_CHECK; 
                TimeOutStart();              
            }
            else if(sd=='3')
            {
                Dprintf("AT+WSEC=0,wpa,74477447------->");
                g_Wifi_Status=WIFI_STANDBY; 
                Wprintf("AT+WSEC=0,wpa,74477447\r"); 
                g_Wifi_Command=COM_TERMINAL_CHECK; 
                TimeOutStart();             
            }
            else if(sd=='4')
            {
                Dprintf("AT+WNET=1------->"); 
                g_Wifi_Status=WIFI_STANDBY; 
                Wprintf("AT+WNET=1\r"); 
                g_Wifi_Command=COM_TERMINAL_CHECK; 
                TimeOutStart(); 
            }       
            else if(sd=='5')
            {
                Dprintf("AT+WJOIN------->"); 
                g_Wifi_Status=WIFI_STANDBY; 
                Wprintf("AT+WJOIN\r"); 
                g_Wifi_Command=COM_TERMINAL_CHECK; 
                TimeOutStart();             
            }
            else if(sd=='6')
            {
                Dprintf("AT+SCON=SO,TCN,1.11.202.83,10011,10011,1------->"); 
                g_Wifi_Status=WIFI_STANDBY; 
                Wprintf("AT+SCON=SO,TCN,1.11.202.83,10011,10011,1\r"); 
                g_Wifi_Command=COM_TERMINAL_CHECK; 
                TimeOutStart();             
            }
            else if(sd=='c')
            {
                Dprintf("Wifi Disconnect------->\n");
                Wprintf("+++");
                g_Wifi_Status=WIFI_STANDBY; 
                g_Wifi_Command=COM_TERMINAL_CHECK; 
            }
            else if(sd=='d')
            {
                Dprintf("Wifi Data Mode------->\n");
                Wprintf("AT+SDATA\r");
                g_Wifi_Status=WIFI_STANDBY; 
                g_Wifi_Command=COM_TERMINAL_CHECK; 
            }
            else if(sd=='j')
            {
                Dprintf("Wifi GateWay Connect Start------->\n");
                g_Wifi_Group_Command = COM_JOIN;
                process_no = 0;
            }
            else if(sd=='s')
            {
                Dprintf("Server Data Transmit\n");
                Wprintf("POST /fcs.rpc HTTP/1.0\r\n");
                Wprintf("Host: 1.11.202.83:10011\r\n");
                Wprintf("Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n");
                strcpy(buf, "{\"jsonrpc\":\"2.0\", \"method\":\"AuthMeasureMemberByCard\", \"params\": { \"UID\":\"C6041FBD\", \"machineSN\":\"ME0000001\", \"macAddr\":\"000102030405\", \"ipAddr\":\"192.168.100.100\" }, \"id\":1 }");
                Wprintf("Content-length: %d\r\n\r\n", strlen(buf));
                Wprintf("%s", buf);
            }
        }
        //연속동작을 위한 시컨스제어
        if(g_Wifi_Group_Command==COM_SERVER)
        {
            switch(process_no)
            {
                case 0:
                {
                    Dprintf("\tServer Connect Try------->");
                    g_Wifi_Status=WIFI_STANDBY;             
                    Wprintf("AT+SCON=SO,TCN,1.11.202.83,10011,10011,1\r"); 
                    g_Wifi_Command=COM_TERMINAL_CHECK; 
                    process_no++;
                    TimeOutStart();
                    break;
                }           
                case 1:
                {
                    if(g_Wifi_Status == CONNECT )//|| g_Wifi_Status == WIFI_OK)
                    {
                        Dprintf("Server Data Transmit\n");
                        Wprintf("POST /fcs.rpc HTTP/1.0\r\n");
                        Wprintf("Host: 1.11.202.83:10011\r\n");
                        Wprintf("Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n");
                        strcpy(buf, "{\"jsonrpc\":\"2.0\", \"method\":\"AuthMeasureMemberByCard\", \"params\": { \"UID\":\"C6041FBD\", \"machineSN\":\"ME0000001\", \"macAddr\":\"000102030405\", \"ipAddr\":\"192.168.100.100\" }, \"id\":1 }");
                        Wprintf("Content-length: %d\r\n\r\n", strlen(buf));
                        Wprintf("%s", buf);       
                        process_no++;                        
                    }
                    if(g_Wifi_Time_Check==1)
                    {
                        if(g_Wifi_Time > 500)  //100msec 동안 응답이 없으면
                        {         
                            process_no =0;
                            Dprintf("\t\tTime Out\n");
                        }
                    }
                    break;
                }
                case 3:
                {
                    if(g_Wifi_Status == DISCONNECT)
                    {      
                        Dprintf("Server Data Transmit OK\n");
                        g_Wifi_Group_Command = COM_STANDBY;
                    }       
                    break;
                }
            }
        }
        /*
        if(g_Wifi_Group_Command==COM_JOIN)
        {
            switch(process_no)
            {
                case 0: 
                {
                    Dprintf("\tTerminal Check------->");
                    g_Wifi_Status=WIFI_STANDBY; 
                    Wprintf("AT\r"); 
                    g_Wifi_Command=COM_TERMINAL_CHECK; 
                    TimeOutStart(); 
                    process_no++; 
                    break; //Terminal Check
                }
                case 1:
                {
                    if(g_Wifi_Status==WIFI_OK)
                    {
                        Dprintf("\tAT+WSET=0,Song,,5------->");                        
                        g_Wifi_Status=WIFI_STANDBY; 
                        Wprintf("AT+WSET=0,Song,,5\r"); 
                        g_Wifi_Command=COM_TERMINAL_CHECK; 
                        TimeOutStart(); 
                        process_no++; 
                    }
                    break;
                }
                case 2:
                {
                    if(g_Wifi_Status==WIFI_OK)
                    {
                        Dprintf("\tAT+WSEC=0,wpa,74477447\r");
                        g_Wifi_Status=WIFI_STANDBY; 
                        Wprintf("AT+WSEC=0,wpa,74477447\r"); 
                        g_Wifi_Command=COM_TERMINAL_CHECK; 
                        TimeOutStart(); 
                        process_no++; 
                    }
                    break;
                }
                case 3:
                {
                    if(g_Wifi_Status==WIFI_OK)
                    {
                        Dprintf("\tAT+WNET=1\r"); 
                        g_Wifi_Status=WIFI_STANDBY; 
                        Wprintf("AT+WNET=1\r"); 
                        g_Wifi_Command=COM_TERMINAL_CHECK; 
                        TimeOutStart(); 
                        process_no++; 
                    }
                    break;
                }
                case 4:
                {
                    if(g_Wifi_Status==WIFI_OK)
                    {
                        Dprintf("\tAT+WJOIN\r"); 
                        g_Wifi_Status=WIFI_STANDBY; 
                        Wprintf("AT+WJOIN\r"); 
                        g_Wifi_Command=COM_TERMINAL_CHECK; 
                        TimeOutStart(); 
                        process_no++; 
                    }
                    break;
                }
                case 5:
                {
                    if(g_Wifi_Status==LINK_UP)
                    {
                        Dprintf("Link Up\n");                        
                        process_no++; 
                    }
                    break;
                }
                case 6:
                {
                    if(g_Wifi_Status==WIFI_OK)
                    {
                        Dprintf("JOIN OK\n");
                        g_Wifi_Group_Command = COM_STANDBY;
                    }
                    break;
                }
            }
        }
        */
        //모듈이 응답시간안에 응답하지 않을경우 처리
        /*
        if(g_Wifi_Time_Check==1)
        {
            if(g_Wifi_Time > WIFI_TIMEOUT)  //100msec 동안 응답이 없으면
            {
                gParsing=0;
                RxCounter=0;
                g_Wifi_Time_Check=0;
                g_Wifi_Status = NOT_ACK; //현재와이파이 모듈의 상태는 응답없음상태이다.
                g_Wifi_Command = COM_STANDBY; //다음명령을 받기위해 
                Dprintf("Wifi Ack TimeOut\n");
            }
        }
        */
        //모듈이 응답했을경우 응답분석
        if(gParsing==1)
        {
            for(i=0; i<ParsingCounter; i++)
            {
                buf[i] = ParsingBuffer[i];
            }
            buf[i]=0;
            
            //Dprintf("Parsing String=%s\n", buf);
            if(!strcmp(buf,"[OK]"))
            {
                g_Wifi_Status = WIFI_OK;
                Dprintf("OK Command Received\n");                
            }
            else if(!strcmp(buf,"[ERROR]"))
            {
                g_Wifi_Status = WIFI_ERROR;
                Dprintf("ERROR Command Received\n");                
            }
            else if(!strcmp(buf,"[ERROR: INVALID INPUT]"))
            {
                g_Wifi_Status = INVALD_INPUT;
                Dprintf("ERROR: INVALID INPUT Command Received\n");                
            }
            else if(!strcmp(buf,"[ERROR: INVALID SCID]"))
            {
                g_Wifi_Status = INVALD_SOCKET_ID;
                Dprintf("ERROR: INVALID Socket ID Command Received\n");                
            }
            else if(!strcmp(buf,"[ERROR:Wifi Status]"))
            {
                g_Wifi_Status = ERR_WIFI;
                Dprintf("ERROR:Wifi Command Received\n");                
            }
            else if(!strcmp(buf,"[ERROR:Mode Status]"))
            {
                g_Wifi_Status = ERR_MODE;
                Dprintf("ERROR:Mode Command Received\n");                
            }
            else if(!strcmp(buf,"[CONNECT 0]"))
            {
                g_Wifi_Status = CONNECT;
                Dprintf("CONNECT Command\n");                
            }
            else if(!strcmp(buf,"[DISCONNECT 0]"))
            {
                g_Wifi_Status = DISCONNECT;
                Dprintf("DISCONNECT Command Received\n");                
            }
            else if(!strcmp(buf,"[Link-Up Event]"))
            {
                g_Wifi_Status = LINK_UP;
                Dprintf("Link-Up Command\n");                
            }
            else if(!strcmp(buf,"[Link-Down Event]"))
            {
                g_Wifi_Status = LINK_DOWN;
                Dprintf("Link-Down Command Received\n");                
            }
            else if(!strcmp(buf,"[Reset Event]"))
            {
                g_Wifi_Status = SYSTEM_RESET;
                Dprintf("Reset Command Received\n");                
            }
            
            if(g_Wifi_Command==COM_TERMINAL_CHECK)
            {
                if(g_Wifi_Status==WIFI_OK)
                {
                    //g_Wifi_Command=COM_STANDBY;
                }
            }
            gParsing=0;
        }
    }    
}
#endif

#ifdef __cplusplus
}
#endif
