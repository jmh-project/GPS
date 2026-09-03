/******************************************************************************
*    	File name	: main.c 
*		Description : TEECO Project Entry File
*		Company	:	ZionTech
*		Version		: 1.00
******************************************************************************/
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"

/*
    1. TOUCH 프로그램수정
    2. EXO 프로그램수정
    3. 
*/
#ifdef EXO_ENABLE
int EXO_IOProcess(void);
#endif

void IOProcess_Init(void);

int main(void)
{
    int		kind, Done=0;
	MSG 	Message, TimMessage, IOTimMessage;		
    
	TEECO_BSP_Init();   
	Windows_Init();
    
//    Dprintf("\n 펌웨어 업데이트 이력 \n");
//    Dprintf("1. 2023.07.08 EXO FIND 기능업데이트\n");
//    Dprintf("2. 2023.08.08 TCP/IP 펌웨어업데이트\n");
//    Dprintf("3. 2023.08.21 ULICK MW TYPE 센서적용\n");
//    Dprintf("4. 2023.08.24 RTU SM718 문제 발생시 자동복구기능추가 BUTTON, TOUCH\n");
//    Dprintf("5. 2023.08.24 저장파일을 TXT 형태로 저장한다.\n");
//    Dprintf("6. 2023.08.24 프로그램 업데이트 방법 변경 USB, SDCARD, LAN\n");
//    Dprintf("7. 2023.08.25 RAM SPEED 150\n");
//    Dprintf("8. 2023.08.28 IOBoard 리셋시 이력을 저장한다.\n");
//    Dprintf("9. 2023.08.28 ULIK D/M type 프로그램작업완료.\n");
//    Dprintf("10.2023.10.05 ODOCT 프로그램 업데이트 ODOCT 모델은 염분수동교정이안됨\n");
//    Dprintf("11.2023.10.13 EXO2 2CH 프로그램작업진행\n");
//	Dprintf("12.2023.11.22 USB 다운로드시 리셋되는 현상 수정\n");
//    Dprintf("13.2023.11.29 1분데이터 60분데이터 저장기능추가\n");
//	Dprintf("14.2023.11.29 140초 동안 터치동작이 없는 경우 시스템리셋\n");
//	Dprintf("15.2023.11.29 -9999999.9 ~ 99999999.9 범위를 벗어나는 경우 오류처리\n");
//	Dprintf("16.2023.12.01 Color 설정에서 기존컬러 복원\n");
//		
//	Dprintf("17. MEMORY 참조\n");
//	Dprintf("- PSRAM 64MBIT , 4M WORD * 16BIT DRAM ARRAY\n");
//	Dprintf("- SDRAM 16M * 16BIT = 256MBIT / 8 = 32MBYTE\n");
//	Dprintf("- SDRAM 512M * 16BIT = 8GBIT (SM718)\n");
//	Dprintf("- SCREEN PAGE1 = 0x6A280000 SCREEN PAGE14 = 0x6B600000\n");
//	
//	Dprintf("18.2024.01.04 TMS 프로토콜적용\n");
//	Dprintf("18-1. 통신불량처리시간초 300초(5분) 누적시 75초 이상 통신이 안되는 경우 통신불량 처리\n");
//	Dprintf("18-2. 통신불량처리시간초 3600초(60분) 누적시 5분데이터가 3회이상 통신이 안되는 경우 통신불량처리\n");
//	Dprintf("18-3. 통신교정처리는 모드진입시 무조건 5분데이터는 교정상태처리한다. 5분 1시간 공통적용\n");
//	Dprintf("18-4. pH, DO TMS 프로토콜 적용시 항목코드가 자동 적용된다. KECO 프로토콜 적용시 기본 DAT00, DAT01 순으로 출력되며 변경이 가능한다.\n");
//	Dprintf("18-5. 60분 데이터 저장시 5분 통신불량이 3회 발생하면 통신불량처리하고 측정값은 0으로 저장한다.\n");	
//	Dprintf("18-6. RTU 가 중간에 리셋이 되는 경우 모든 경우를 무시하고 데이터를 저장한다.\n");
//	
//	Dprintf("19. EXO FIND 기능 실행시 센서가 안정이 안된경우 메시지를 출력한다.\n");
//	Dprintf("20. 센서 연결 시 처리 시간과 요청 시간이 1대1 처리가 되도록 한다.\n");
//	
//	Dprintf("21. http://elm-chan.org/fsw/ff/archives.html 파일시스템 주소 http://elm-chan.org/fsw/ff/\n");
//	Dprintf("21-1. ff_wtoupper, ff_convert 별도 함수 선언하여 수정\n");
//	Dprintf("21-2. gv.usbSaveWaitFlag=1 이면 시스템 오류 체크 타이머가 동작하지 않는다.\n");
//	Dprintf("21-3. f_read는 바이너리 데이터를 읽을 때 주로 사용되며, 지정된 바이트 수만큼 읽어오기 때문에 줄 바꿈 문자 등을 고려하지 않습니다.\n");
//	Dprintf("21-4. f_gets는 텍스트 파일에서 한 줄을 읽어오기 때문에 줄 바꿈 문자 등을 고려하며, 문자열로 반환됩니다.\n");
//	
//	Dprintf("22-1. TEECO program 다운시 1분 후에 자동 리셋된다. 기존 모델은 2분이다.\n");
//	Dprintf("22-2. 1분 후 복구하는 이유는 5분 데이터를 꼭 저장하기 위함이다.\n");
//	Dprintf("22-3. 프로그램 다운로드 시간은 대략 35초 정도된다.\n");
//	Dprintf("22-4. 부팅타임 5초\n");

    Dprintf("1024, 768 (3928,3881) (73,127)\n");//터치교정값
    Dprintf("1024, 768 (3939,3870) (95,191)\n");//교정후값
	
	if(flash.powerControl == 0) {
		Window_Resource_Load();		//Font, ICON등을 로딩한다.
		USB_Host_Start();			//USB HOST Memory 기능사용시 활성화시킨다.        		        
        
		flash.StartVerify=1234;     //데이터복원기능구현 / system 메뉴에서 복구파일은 사용자가 설정한다.
        flash.autoRebootCount++;
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
		
		DataLogMain_Window();        
        Dprintf("재부팅횟수 = %d\n",flash.autoRebootCount);
        EventMessage("프로그램을 시작합니다");
	}
	else {
        EventMessage("슬립모드를 시작합니다");
        flash.StartVerify=1234;     //데이터복원기능구현
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        IOProcess_Init();
	}	
	gv.usbSaveWaitFlag=0;
	while (!Done) {
		kind = GetMessage(&Message, &TimMessage, &IOTimMessage); 	//메시지큐에서 메시지를 읽어들인다.
		if(kind) { 	
			DispatchMessage(&Message, &TimMessage, &IOTimMessage, kind);	//메시지큐에서 꺼낸 메시지를 WndProc 으로 전달한다.
        }
	}
	return(0);    
}


