/******************************************************************************
*    	File name	: IOProcess.c 
*		Description : TEECO Project Entry File
*		Company		: idnics co.,
*		Version		: 1.00
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
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
//VRAM 사용시
#ifdef VRAM_USE
#define	IOTIMER_MALLOC_START		0x6B3FA000
#define     IO_PROCESS1     (0x6B803200)
#define     IO_PROCESS2     (0x6B803400)
#define     IO_PROCESS3     (0x6B803600)
#define     IO_PROCESS4     (0x6B803800)
#define     IO_PROCESS5     (0x6B803A00)
#endif

//PSRAM 사용시
#ifdef PSRAM_USE
#define	IOTIMER_MALLOC_START		0x645A4A00
#define     IO_PROCESS1     				(0x645A4000)
#define     IO_PROCESS2     				(0x645A4000+0x200)
#define     IO_PROCESS3     				(0x645A4000+0x400)
#define     IO_PROCESS4     				(0x645A4000+0x600)
#define     IO_PROCESS5     				(0x645A4000+0x800)
#endif

/******************************************************************************
							Private & Local Variables
******************************************************************************/
//각각의 배열의 값은 타이머의 활성화 여부를 나타내고 있다.
//윈도우 매니져에서는 타이머의 전체 갯수를 관리한다.
//gp_WinManager->IOTimerCount
volatile U8		IOTimerFlag[64];	

//IO Process는 다섯개로 제한한다. 이프로세서가 많다고 좋은것도 아니고 5개면
//시스템제어부분이 많다고 해도 구분하기 충분하기때문이다.
stIOProcess		*gp_IOProcess_1;
stIOProcess		*gp_IOProcess_2;
stIOProcess		*gp_IOProcess_3;
stIOProcess		*gp_IOProcess_4;
stIOProcess		*gp_IOProcess_5;

#if defined MEDIA_USB_KEY
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;
#endif

volatile int    gUSB_MSC_Status=0;
volatile int 	gUSB_MSC_StatusDump=0;
/******************************************************************************
							Function Prototype
******************************************************************************/
//Windows_Init 에서 호출하여 초기화한다.
void IOProcessInit(void)
{
	//IO Process 시작메모리를 할당 받는다.
	gp_IOProcess_1 = (stIOProcess *)IO_PROCESS1; 	//내부메모리를 사용하기위해 삭제
	memset(gp_IOProcess_1, 0, sizeof(stIOProcess));

	gp_IOProcess_2 = (stIOProcess *)IO_PROCESS2; 	//내부메모리를 사용하기위해 삭제
	memset(gp_IOProcess_2, 0, sizeof(stIOProcess));

	gp_IOProcess_3 = (stIOProcess *)IO_PROCESS3; 	//내부메모리를 사용하기위해 삭제
	memset(gp_IOProcess_3, 0, sizeof(stIOProcess));

	gp_IOProcess_4 = (stIOProcess *)IO_PROCESS4; 	//내부메모리를 사용하기위해 삭제
	memset(gp_IOProcess_4, 0, sizeof(stIOProcess));

	gp_IOProcess_5 = (stIOProcess *)IO_PROCESS5; 	//내부메모리를 사용하기위해 삭제
	memset(gp_IOProcess_5, 0, sizeof(stIOProcess));
    
    gUSB_MSC_Status = 0;
}

//Windows_Init 에서 호출하여 초기화한다.
void IOTimerInit(void)
{
	int		i;
	
	for(i=0; i<64; i++)	//개별타이머 활성화 플래그 클리어
	{
		IOTimerFlag[i] = FALSE;
	}
    //내부메모리를 사용하기위해 삭제
	memset((U8 *)IOTIMER_MALLOC_START, 0x00, (sizeof(IOTimer) * 64));
	gp_WinManager->IOTimerCount = 0;
}

int IOOpenTimer(HANDLE handle, int interval)
{
	int			i;
	IOTimer 	*iotimer;
	U32			addr;
	
	for(i=0; i<64; i++)
	{
		if(IOTimerFlag[i] == 0)
		{
			addr = IOTIMER_MALLOC_START + (i * sizeof(IOTimer));
			//Dprintf("SetTimer:: addr=%08x\n", addr);
			iotimer = (IOTimer *)addr;
			iotimer->Handle = handle;
			iotimer->Interval = interval;

			iotimer->id = i;
			iotimer->Status=STOP;
			iotimer->Count=0;

			IOTimerFlag[i] = TRUE;
			gp_WinManager->IOTimerCount++;
			//Dprintf("SetTimer::parent=%d handle=%d Interval=%d id=%d Total=%d\n", 
			//wintimer->Parent, wintimer->Handle, wintimer->Interval, wintimer->id, gp_WinManager->TimerCount);
			return(i);
		}
	}
	Dprintf("IOOpenTimer:: Timer Allocation Error\n");
	return(-1);
}

int IOStartTimer(int id)
{
	IOTimer *io_timer;
	U32 addr = IOTIMER_MALLOC_START + (id * sizeof(IOTimer));
	
	if(IOTimerFlag[id]==TRUE)
	{
		io_timer = (IOTimer *)addr;
		io_timer->Status=START;
		io_timer->Count=0;
		//Dprintf("StartTimer:: addr=%08x\n", addr);
	}	
	else
	{
		Dprintf("IOStartTimer:: id Not Found id=%d\n", id);
	}
	return(1);
}

//생성되어있는 타이머의 주기값을 바꿀때 사용한다.
int IOSetTimerInterval(int id, int interval)
{
	IOTimer *io_timer;
	U32 addr = IOTIMER_MALLOC_START + (id * sizeof(IOTimer));
	
	if(IOTimerFlag[id]==TRUE)
	{
		io_timer = (IOTimer *)addr;
//		io_timer->Status=START;
//		io_timer->Count=0;
		io_timer->Interval=interval;
	}	
	else
	{
		Dprintf("IOStartTimer:: id Not Found id=%d\n", id);
	}
	return(1);
}

//생성된타이머의 동작을 일시중지 시킬때 사용한다.
int IOStopTimer(int id)
{
	IOTimer *io_timer;
	U32 addr = IOTIMER_MALLOC_START + (id * sizeof(IOTimer));
	
	if(IOTimerFlag[id]==TRUE)
	{
		io_timer = (IOTimer *)addr;
		io_timer->Status=STOP;
		io_timer->Count=0;
	}	
	else
	{
		Dprintf("IOStopTimer:: id Not Found id=%d\n", id);
	}
	return(1);
}

int IOCloseTimer(int id)
{
	if(id >= 0 && id <64)
	{
		gp_WinManager->IOTimerCount--;
		IOTimerFlag[id] = FALSE;
	}
	else
		Dprintf("IOCloseTimer:: id=%d mistypeing\n", id);
	return(1);
}

int IOTimerAutoClose(HANDLE handle)
{
	int			i;
	IOTimer 	*iotimer;
	U32			addr;
	
	for(i=0; i<64; i++) //총64개의 타이머중에서
	{
		if(IOTimerFlag[i] == TRUE) //가동중인 타이머를 검색후
		{
			addr = IOTIMER_MALLOC_START + (i * sizeof(IOTimer));

			iotimer = (IOTimer *)addr; //그타이머의 포인터를 얻은후
			if(handle == iotimer->Handle) //클로즈되는 윈도우의 핸들과 같으면
			{
				IOTimerFlag[i] = FALSE; 		//디저블시키고
				gp_WinManager->IOTimerCount--; 	//전체 카운트를 감소시킨다.
			}
		}
	}
	return(1);
}

//주어진 핸들의 프로세스포인터를 리턴한다.
//이함수는 무조건 포이터를 리턴한다. makeIOProcess 에서 사용한다.
stIOProcess *FindIOProcessPointer(HANDLE handle)
{
	stIOProcess *io_process = NULL;

	switch(handle)
	{
		case 1: 	
				io_process = gp_IOProcess_1;	
		break;
		case 2: 	
				io_process = gp_IOProcess_2;	
		break;
		case 3: 	
				io_process = gp_IOProcess_3;	
		break;
		case 4: 	
				io_process = gp_IOProcess_4;	
		break;
		case 5: 	
				io_process = gp_IOProcess_5;	
		break;
		default:	
		break;
	}

	if(io_process== NULL) 
	{
		Dprintf("FindIOProcessPointer::Invalid Handle=%d\n", handle);
		return (NULL);
	}
	return(io_process);
}


//주어진 핸들의 프로세스포인터를 리턴한다.
stIOProcess *FindIOProcess(HANDLE handle)
{
	stIOProcess *io_process = NULL;

	switch(handle)
	{
		case 1: 	
				io_process = gp_IOProcess_1;	
		break;
		case 2: 	
				io_process = gp_IOProcess_2;	
		break;
		case 3: 	
				io_process = gp_IOProcess_3;	
		break;
		case 4: 	
				io_process = gp_IOProcess_4;	
		break;
		case 5: 	
				io_process = gp_IOProcess_5;	
		break;
		default:	
		break;
	}

	if(io_process== NULL) 
	{
		Dprintf("FindIOProcess::Invalid Handle=%d\n", handle);
		return (NULL);
	}
	//할당된 IO 가 생성이 안된것이라면 NULL
	if(io_process->Status==0) return (NULL);
	else return(io_process);
}

//프로세스를 만들때 비워있는 공간을 할당받기위해 사용한다.
//0이리턴되면 프로세스가 모두 사용중이므로 만들수 없다.
HANDLE GetIOProcessHandle(void)
{
	if(gp_WinManager->IOProcess[1] == false) return(1);
	if(gp_WinManager->IOProcess[2] == false) return(2);
	if(gp_WinManager->IOProcess[3] == false) return(3);
	if(gp_WinManager->IOProcess[4] == false) return(4);
	if(gp_WinManager->IOProcess[5] == false) return(5);
	return(0);
}

//현재 만들어진 프로세스의 수를 리턴한다.
int GetIOProcessCount(void)
{
	int		cnt=0;
	if(gp_WinManager->IOProcess[1] == true) cnt++;
	if(gp_WinManager->IOProcess[2] == true) cnt++;
	if(gp_WinManager->IOProcess[3] == true) cnt++;
	if(gp_WinManager->IOProcess[4] == true) cnt++;
	if(gp_WinManager->IOProcess[5] == true) cnt++;
	return(cnt);
}

//주어진 핸들의 프로세스를 리턴한다.
stIOProcess *GetIOProcess(HANDLE handle)
{
	stIOProcess	*io_process=NULL;
	
	io_process = FindIOProcess(handle);
	
	if(io_process==NULL)
	{
		Dprintf("GetIOProcess::process not found handle=%d\n", handle);
		return(NULL);
	}
	return(io_process);
}

//프로세스에는 이름이 할당되는데 이름으로 프로세스 포인터를 찾을때 사용한다.
stIOProcess *GetIOProcessByName(char *name)
{
	int		i;
	int		cnt = GetIOProcessCount();
	stIOProcess	*io_process=NULL;

	if(cnt==0) return(NULL);

	for(i=1; i<=cnt; i++)
	{
		io_process = GetIOProcess(i);
		if(!strcmp(name,io_process->Name))
		{
			return(io_process);
		}
	}
	return(NULL);	
}


HANDLE makeIOProcess(char *Name, 
		int	(*PrepareEvent)(HANDLE , int , int , void *),
		int	(*TimerEvent)(HANDLE , int , int , void *) )
{
	HANDLE	handle;
	//내부에서 사용하기위해 임시포인터
	stIOProcess	*io_process=NULL;
	
    //프로세스의 핸들을 할당받는다. 1 ~5 까지 사용할수있는데 안쓰는 번호를 할당 받게된다.
	handle = GetIOProcessHandle();
	if(handle==0)
	{
		Dprintf("makeIOProcess::Don't make IO Process [Handle]\n");
		return(0);
	}
    //핸들에 해당하는 정의된 메모리 영역을 가지고 온다.
    //gp_IOProcess_1 ~ 5
	io_process = FindIOProcessPointer(handle);
	if(io_process==NULL)
	{
		Dprintf("makeIOProcess::Don't make IO Process [Pointer]\n");
		return(0);		
	}
    gp_WinManager->IOProcess[handle] = true;
    //지정받은 메모리 영역에 필요한 데이터를 기재한다.
	//make Process
	strcpy(io_process->Name, Name);
	io_process->TimerEvent = TimerEvent;
	io_process->PrepareEvent = PrepareEvent;
	io_process->Handle = handle;
	io_process->Status = OK;
	
    //프로세스가 생성되는 시점에서 처음콜되는 함수 Create 와 같다.
    io_process->PrepareEvent(handle, 0, 0, 0);
 
	return(handle);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int	 IOPrepareEvent(HANDLE handle, int command, int param, void *Data)
{
	int		id;
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOPrepareEvent:: IO Precess not Found\n");
		return(0);
	}
	id = IOOpenTimer(handle, 1000);
    Dprintf("IO Timer ID= %d\n", id);
	IOStartTimer(id);
	return(1);
}
int	 IOPrepareEvent1(HANDLE handle, int command, int param, void *Data)
{
	int		id;
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOPrepareEvent1:: IO Precess not Found\n");
		return(0);
	}
	id = IOOpenTimer(handle, 1000);
	IOStartTimer(id);
	return(1);
}
int	 IOPrepareEvent2(HANDLE handle, int command, int param, void *Data)
{
	int		id;
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOPrepareEvent2:: IO Precess not Found\n");
		return(0);
	}
	id = IOOpenTimer(handle, 1000);
	IOStartTimer(id);
	return(1);
}
int	 IOTimerEvent(HANDLE handle, int command, int param, void *Data)
{
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOTimerEvent:: IO Precess not Found\n");
		return(0);
	}
    Dprintf("\n IOTimerEvent Occur\n");
	//USBH_Process(&USB_OTG_Core, &USB_Host);
	return(1);
}

int	 IOTimerEvent1(HANDLE handle, int command, int param, void *Data)
{
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOTimerEvent1:: IO Precess not Found\n");
		return(0);
	}
	//USBH_Process(&USB_OTG_Core, &USB_Host);
    Dprintf("\n IOTimerEvent1 Occur\n");    
	return(1);
}

int	 IOTimerEvent2(HANDLE handle, int command, int param, void *Data)
{
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOTimerEvent1:: IO Precess not Found\n");
		return(0);
	}
	//USBH_Process(&USB_OTG_Core, &USB_Host);
    Dprintf("\n IOTimerEvent2 Occur\n");    
	return(1);
}

int IOProcessTest(void)
{
	makeIOProcess("Test", IOPrepareEvent, IOTimerEvent);
	makeIOProcess("Test1", IOPrepareEvent1, IOTimerEvent1);
	makeIOProcess("Test2", IOPrepareEvent2, IOTimerEvent2);
	return(1);
}
#ifdef __cplusplus
}
#endif

