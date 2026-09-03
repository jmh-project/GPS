#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"
/*
*   ioboard 제어용
*/

int gComPortToggle=5;
int TransmitCharData( char font, char channel)
{
    if(channel==0)
        SENSOR_UART1( font );//센서로 데이터를 송신한다.
    else if(channel==1)
        SENSOR_UART2( font );//센서로 데이터를 송신한다.
    else if(channel==2)
        SENSOR_UART3( font );//센서로 데이터를 송신한다.
    else if(channel==3)
        SENSOR_UART4( font );//센서로 데이터를 송신한다.
    return 0;
}

int	 IOProcessHardwareCreate(HANDLE handle, int command, int param, void *Data) {
    U32		id;

    stIOProcess 	*io_process = FindIOProcess(handle);

    if(io_process==NULL) 
    {
        Dprintf("IOProcessHardwareCreate:: IO Precess not Found\n");
        return(0);
    }
    id = IOOpenTimer(handle, 1000);
    IOStartTimer(id);
    return(1);
}
void ConnectionStatus(void) {
    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)==0) {
        flash.connection[0]=1;//접점
        Dprintf("알람발생\n");
    } else {
        //flash.connection[0]=0; //데이터저장 후 클리어한다.
    }
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)==0) {
        flash.connection[1]=1;//접점
    } else {
        flash.connection[1]=0;
    }
}
#ifdef JSON_ENABLE
int ProgramUpdate(void) {
    if(gv.downloadFlag==1) {
        SD_ProgramUpdateFileWrite("1:TEECO.bin",gv.downloadDataSize);
        Dprintf("업데이트 파일 크기 = %d\n",gv.downloadDataSize);
        EventMessage("프로그램 업데이트를 시작합니다.");	
        SystemRebootGo();
    }	
	if(gv.json.nextSendCmdNo==5) { //펌웨어 업데이트 중에는 다른 기능이 실행 되지 않는다.
        return 1;
	}	
    return 0;
}
#endif
int EventMessageCheck(void) {   
    switch(gv.sm718WaitMessage) {//SM718관련이벤트가발생하면저장한다.
        case 0: break;
        case 1: EventMessage("SM718 Wait2D_Engine");		break;
        case 2: EventMessage("SM718 deWaitForNotBusy");		break;
        case 3: EventMessage("SM718 SM718_2D_Engine_Wait");	break;
        case 4: EventMessage("SM718 WaitForNotBusy");		break;
        case 5: EventMessage("SM718 WaitFor2DBusy");		break;
    }
    gv.sm718WaitMessage=0;

    if(gv.powerResetMessage==1) {		//IOBoard 로부터 수신한 메시지를 저장한다.
        gv.powerResetMessage=0;
        EventMessage("IOBOARD POWER RESET");
    }
    return 0;
}
int SystemAdministrator(void) {         //이부분을 수정하면 시스템에 치명적일 수 있다.        	
	gv.systemWatshTimerCnt=0;           //TimingDelay_Decrement
    GPIO_ToggleBits(GPIOB,GPIO_Pin_0);  //0. IOBoard 신호
    gv.lanCloseTimerCount[0]++;         //1. 랜 close 타임증가
	#ifdef JSON_ENABLE
    TcpIpDebug("LAN TEST");             //1. 줄바꿈은 사용하지 않는다.   
    if(ProgramUpdate()==1) {            //1. 랜 접속중 펌웨어업데이트 (JSON_ENABLE 시 사용가능)
        return 0;
    }
	#endif
    TouchChecking();                    //2. 터치 상태를 체크한다.
    EventMessageCheck();                //2-1 SM718 오류 체크용    
    DS1302TimeRead();                   //3. 시간을 읽어들인다.
    UartConnectErrorCheckingMode();	    //4. SENOSR 통신 단절시 에러시간이 증가한다. 통신중에 메시지가 출력된다.
    StructCheckingMode();               //5. 구조체 파일에 문제가 발생한 경우 리셋한다.
    
	SaveMesaurements();               //6. 5분 데이터 저장 
	//SaveTheMeasurement(1);				//6-1. 초단위로 데이터 저장 (인하대학교)
    
	EntrySleepMode();                   //7. 절전모드로 넘어간다.
    RebootSetTime();                    //8. 설정 시간이 되면 화면을 초기화한다.
    SystemWakeUp();                     //9. 슬립모드에서 깨어난다.
            
    if(gv.screenCreateFlag==1) {        //10.화면을 생성한다.
        char	buf[64];
        sprintf(buf,"%s_%02d%02d.bmp", "Image", currentDate.minute,currentDate.sec);
        strcpy(Disk_Kind,"1:");		
        BMPCreateSD("0", "0", "1024", "768", buf);	
        DataLog_MessageBox_Window("화면캡쳐", "화면캡처완료.");
        gv.screenCreateFlag=0;
    }    
	if(gv.buttonWatchTimerCount>61000) {
		Dprintf("gv.buttonWatchTimerCount=%d\n",gv.buttonWatchTimerCount);
	}
    return 0;
}
int	 IOProcessHardwareTimerEvent(HANDLE handle, int command, int param, void *Data) {
    stIOProcess *io_process = FindIOProcess(handle);
    if(io_process==NULL) {
        EventMessage("IO Precess not Found");
        Dprintf("IOProcessHardwareTimerEvent:: IO Precess not Found\n");
        return(0);
    }   
    SystemAdministrator();              //시스템 오동작시 RESET 명령을 실행한다.	
    Uart1ReceiveProcess();              //10. 수신한 통신데이터를 처리한다.
    Uart2ReceiveProcess();
    Uart3ReceiveProcess();
    Uart4ReceiveProcess();
    AD420mAOutput();                    //11. 4~20mA출력
    AD420mAInput();                     //12. 4~20mA입력   
    
    
    IoBoardControl();
    //ConnectionStatus();               //13. 접점상태읽기

    return (1);
}

int IOProcessHardwareControl(void) {
    Dprintf("[ENABLE] IOProcessHardwareControl\n");
	makeIOProcess("HARDWARE", IOProcessHardwareCreate, IOProcessHardwareTimerEvent);
	return (0);
}
void IoBoardControl(void) {
    char buf1[50],buf2[50];

    sprintf(buf1,"%05d",gv.ad420Ch1mA);
    sprintf(buf2,"%05d",gv.ad420Ch2mA);

    Uart_SendByte7(0x02);
        
    Uart_SendByte7('A');    //코드 :: 4~20mA 출력 0~65535
    Uart_SendByte7(buf1[0]);//채널 1
    Uart_SendByte7(buf1[1]);
    Uart_SendByte7(buf1[2]);
    Uart_SendByte7(buf1[3]);
    Uart_SendByte7(buf1[4]);

    Uart_SendByte7(buf2[0]);//채널 2
    Uart_SendByte7(buf2[1]);
    Uart_SendByte7(buf2[2]);
    Uart_SendByte7(buf2[3]);
    Uart_SendByte7(buf2[4]);

    if(flash.Uart[USART_1]->UartOr485Select==RS232_ENABLE)	{
        Uart_SendByte7('0');//RS232
    } else {
        Uart_SendByte7('1');//RS484
    }
    if(flash.Uart[USART_2]->UartOr485Select==RS232_ENABLE)	{
        Uart_SendByte7('0');
    } else {
        Uart_SendByte7('1');
    }
    if(flash.Uart[USART_3]->UartOr485Select==RS232_ENABLE)	{
        Uart_SendByte7('0');
    } else {
        Uart_SendByte7('1');
    }
    if(flash.Uart[USART_4]->UartOr485Select==RS232_ENABLE)	{
        Uart_SendByte7('0');
    } else {
        Uart_SendByte7('1');
    }

    if(flash.Uart5Or485Select==RS232_ENABLE)	{
        Uart_SendByte7('0');
    } else {
        Uart_SendByte7('1');
    }
    if(flash.Uart6Or485Select==RS232_ENABLE)	{ 
        Uart_SendByte7('0');
    } else {
        Uart_SendByte7('1');
    }
    if(flash.Uart7Or485Select==RS232_ENABLE)	{
        Uart_SendByte7('0');
    } else {
        Uart_SendByte7('1');
    }
    if(flash.Uart8Or485Select==RS232_ENABLE)	{
        Uart_SendByte7('0');
    } else {
        Uart_SendByte7('1');
    }
    //relay control
    if(flash.relay[0]==ON) GPIO_SetBits(GPIOB, GPIO_Pin_7);//cpu 에서제어 relay 1
    else GPIO_ResetBits(GPIOB, GPIO_Pin_7);
    if(flash.relay[1]==ON) GPIO_SetBits(GPIOB, GPIO_Pin_8);//cpu 에서제어 relay 2
    else GPIO_ResetBits(GPIOB, GPIO_Pin_8);

    if(flash.relay[2]==ON)Uart_SendByte7('1');//ioboard 에서제어 relay 3
    else Uart_SendByte7('0');
    if(flash.relay[3]==ON)Uart_SendByte7('1');//ioboard 에서제어 relay 4
    else Uart_SendByte7('0');
    if(flash.relay[4]==ON)Uart_SendByte7('1');//ioboard 에서제어 relay 5
    else Uart_SendByte7('0');
    if(flash.relay[5]==ON)Uart_SendByte7('1');//ioboard 에서제어 relay 6
    else Uart_SendByte7('0');
    if(flash.relay[6]==ON)Uart_SendByte7('1');//ioboard 에서제어 relay 7
    else Uart_SendByte7('0');

    /**
    *	측정하고하는 항목을 선택한다.
    *	1. GPS
    *	2. SCAN
    */
    if(flash.Uart[USART_4]->SensorID == SENSOR_DEBUG) {
        Uart_SendByte7(0x33); //연결된 센서 선택    
        Uart_SendByte7(0x33); //연결된 센서 선택    
        Uart_SendByte7(0x33); //연결된 센서 선택    
        Uart_SendByte7(0x33); //연결된 센서 선택    
        
        if(gComPortToggle==5) {
            Uart_SendByte7('D');//명령코드 배열주소 29
            Uart_SendByte7('2');//길이 명령코드에서 종료신호까지 
            Uart_SendByte7('1');//길이        
            Uart_SendByte7('5');//통신포트  
            gComPortToggle=6;            
            Uart_Printf7("0123456789 UART5"); //최대송신길이 128byte
            Uart_SendByte7(0x0D);//종료
        } else if(gComPortToggle==6) {
            Uart_SendByte7('D');//명령코드 배열주소 29
            Uart_SendByte7('1');//길이 명령코드에서 종료신호까지
            Uart_SendByte7('5');//길이        
            Uart_SendByte7('6');//통신포트  
            gComPortToggle=7;            
            Uart_Printf7("0123456789"); //최대송신길이 99byte
            Uart_SendByte7(0x0D);//종료
        } else if(gComPortToggle==7) {
            Uart_SendByte7('D');//명령코드 배열주소 29
            Uart_SendByte7('1');//길이 명령코드에서 종료신호까지
            Uart_SendByte7('5');//길이        
            Uart_SendByte7('7');//통신포트  
            gComPortToggle=8;            
            Uart_Printf7("0123456789"); //최대송신길이 99byte
            Uart_SendByte7(0x0D);//종료
        } else if(gComPortToggle==8) {
            Uart_SendByte7('D');//명령코드 배열주소 29
            Uart_SendByte7('1');//길이 명령코드에서 종료신호까지
            Uart_SendByte7('5');//길이        
            Uart_SendByte7('8');//통신포트  
            gComPortToggle=5;            
            Uart_Printf7("0123456789"); //최대송신길이 99byte
            Uart_SendByte7(0x0D);//종료
        }
    } 
	else {
        Uart_SendByte7(flash.Uart5ItemSelect+0x30); //연결된 센서 선택
        Uart_SendByte7(flash.Uart6ItemSelect+0x30); //연결된 센서 선택
        Uart_SendByte7(flash.Uart7ItemSelect+0x30); //연결된 센서 선택
        Uart_SendByte7(flash.Uart8ItemSelect+0x30); //연결된 센서 선택
        Uart_SendByte7(0x0D);
    }
    /*
    *   ioboard 에 송신완료 후 리턴값이 올라온다.
    */
}
