/******************************************************************************
*    	File name	: main.c 
*		Description : TEECO Project Entry File
*		Company	:	ZionTech
*		Version		: 1.00
******************************************************************************/
/******************************************************************************
							Include File
******************************************************************************/
#include 		"stm32f4xx.h"
#include 		"cmsis_os.h"
#include 		"Debug_Uart.h"
#include 		"windows.h"
#include 		"task.h"
#include 		"dos.h"


/******************************************************************************
							Function Prototype
******************************************************************************/
extern int  		Main_Window(int sx, int sy);
extern "C" void TEECO_BSP_Init(void);
extern volatile int		OS_START;
extern "C" {
	int main(void);
	int CPPTest(void);
	int GraphTest_Window(void);
}

int main(void)
{
	OS_START=0;		
	
	int		kind, Done=0;
	MSG 	Message, TimMessage, IOTimMessage;		
	TEECO_BSP_Init();
	Dprintf("BSP Init\n");
	//CPPTest();
	//char	buf1[32];
	//char	buf2[32];
	//char	buf3[32];
	//StringCommaSeparator(">at+net_wanip=?192.168.23.56,255.255.254.12,190.234.56.43\n\r", buf1, buf2, buf3);
	//HLK_RM04_Test();

	Windows_Init();         	   	// GUI 를 사용하기위한 준비작업이다.
	//Window_Resource_Load();	// Font, ICON등을 로딩한다.
	//USB_Host_Start();			// USB HOST Memory 기능사용시 활성화시킨다.
	
	//Main_Window(0, 0);
	Dprintf("Table Test Program Start\n");
	/*
	while(1)
	{
		Dprintf("Test Program\n");
		Delay(1000);
	}
	*/
	CPPTest();
	//stl_test();
	//TableTest_Window();
	GraphTest_Window();
	while (!Done)
	{
		//ScreenCheck();
		kind = GetMessage(&Message, &TimMessage, &IOTimMessage); 	//메시지큐에서 메시지를 읽어들인다.
		if(kind) 	
			DispatchMessage(&Message, &TimMessage, &IOTimMessage, kind);	//메시지큐에서 꺼낸 메시지를 WndProc 으로 전달한다.
	}
	return(0);    
}

