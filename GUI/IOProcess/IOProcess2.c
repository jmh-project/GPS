/******************************************************************************
*    	File name	: IOProcess.c 
*		Description : TEECO Project Entry File
*		Company		: idnics co.,
*		Version		: 1.00
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"
/******************************************************************************
							Description
******************************************************************************/
//IO 타이머는 모두 64개만 생성될수있다.
//IO Process는 타이머를 가질수 있고 64개 이내에서 갯수의 제한은 없다.
//타이머를 가지고 있는 IO Process가 없어지면 타이머도 자동 소멸된다.

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define	IOTIMER_MALLOC_START		0x6B3FA000

/******************************************************************************
							Private & Local Variables
******************************************************************************/
//각각의 배열의 값은 타이머의 활성화 여부를 나타내고 있다.
//윈도우 매니져에서는 타이머의 전체 갯수를 관리한다.
//gp_WinManager->IOTimerCount
extern volatile U8		IOTimerFlag[64];	

//IO Process는 다섯개로 제한한다. 이프로세서가 많다고 좋은것도 아니고 5개면
//시스템제어부분이 많다고 해도 구분하기 충분하기때문이다.
extern stIOProcess		*gp_IOProcess_1;
extern stIOProcess		*gp_IOProcess_2;
extern stIOProcess		*gp_IOProcess_3;
extern stIOProcess		*gp_IOProcess_4;
extern stIOProcess		*gp_IOProcess_5;

#if defined MEDIA_USB_KEY
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;
#endif
/******************************************************************************
							Function Prototype
******************************************************************************/

int	 IOPrepareEvent(HANDLE handle, int command, int param, void *Data)
{
	int		id, id1;
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOPrepareEvent:: IO Precess not Found\n");
		return(0);
	}
	id = IOOpenTimer(handle, 10);
	//id1 = IOOpenTimer(handle, 800);

	IOStartTimer(id);
	//IOStartTimer(id1);
	return(1);
}

int	 IOTimerEvent(HANDLE handle, int command, int param, void *Data)
{
	//static int cnt=0;
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOTimerEvent:: IO Precess not Found\n");
		return(0);
	}
	//Dprintf("IO Process Timer Event ID=%d\n", command);
	USBH_Process(&USB_OTG_Core, &USB_Host);
	//EncoderProcess();	
	return(1);
}

int IOProcessTest(void)
{
	makeIOProcess("Test", IOPrepareEvent, IOTimerEvent);
	return(1);
}
