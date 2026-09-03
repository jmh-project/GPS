/******************************************************************************
*    	File name	: WindowsTimer.c 
*		Description : TEECO Project Entry File
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

#include "Windows.h"

/******************************************************************************
							Description
******************************************************************************/
//윈도우타이머는 모두 64개만 생성될수있다.
//모든윈도우는 타이머를 가질수 있고 64개 이내에서 갯수의 제한은 없다.
//한윈도우에서 64개를 모두 사용할경우 다른 윈도우에서는 사용할수 없다.

//타이머를 가지고 있는 윈도우가 없어지면 타이머도 자동 소멸된다.

/******************************************************************************
							Constant & Macros
******************************************************************************/
#ifndef PSRAM_USE
#define	WINTIMER_MALLOC_START		0x6B3F2000 //VRAM
#endif

#ifdef PSRAM_USE
#define	WINTIMER_MALLOC_START		0x645ACA00 //PSRAM
#endif
/******************************************************************************
							Private & Local Variables
******************************************************************************/
//각각의 배열의 값은 타이머의 활성화 여부를 나타내고 있다.
//윈도우 매니져에서는 타이머의 전체 갯수를 관리한다.
volatile U8		WinTimerFlag[64];	

/******************************************************************************
							Function Prototype
******************************************************************************/

void WindowsTimerInit(void)
{
	int		i;
	
	for(i=0; i<64; i++)	//개별타이머 활성화 플래그 클리어
	{
		WinTimerFlag[i] = FALSE;
	}
	memset((U8 *)WINTIMER_MALLOC_START, 0x00, (sizeof(WinTimer) * 64));
	gp_WinManager->TimerCount = 0;
}

int OpenTimer(HANDLE parent, HANDLE handle, int interval)
{
	int			i;
	WinTimer 	*wintimer;
	U32			addr;

	for(i=0; i<64; i++)
	{
		if(WinTimerFlag[i] == 0) //타이머가 할당이 안되어 있으면
		{
			addr = WINTIMER_MALLOC_START + (i * sizeof(WinTimer));
//Dprintf("SetTimer:: addr=%08x\n", addr);
			wintimer = (WinTimer *)addr;
			wintimer->Parent = parent;
			wintimer->Handle = handle;
			wintimer->Interval = interval;

			wintimer->id = i;
			wintimer->Status=STOP;
			wintimer->Count=0;

			WinTimerFlag[i] = TRUE;
			gp_WinManager->TimerCount++;
//Dprintf("SetTimer::parent=%d handle=%d Interval=%d id=%d Total=%d\n", 
//			wintimer->Parent, wintimer->Handle, wintimer->Interval, wintimer->id, gp_WinManager->TimerCount);
			return(i);
		}
	}
	Dprintf("OpenTimer:: Timer Full Allocation Error\n");
	return(-1);
}

int StartTimer(int id)
{
	WinTimer *win_timer;
	U32 addr = WINTIMER_MALLOC_START + (id * sizeof(WinTimer));
	
	if(WinTimerFlag[id]==TRUE)
	{
		win_timer = (WinTimer *)addr;
		win_timer->Status=START;
		win_timer->Count=0;
		//Dprintf("StartTimer:: addr=%08x\n", addr);
	}	
	else
	{
		Dprintf("StartTimer:: id Not Found id=%d\n", id);
	}
	return(1);
}

int SetTimerInterval(int id, int interval)
{
	WinTimer *win_timer;
	U32 addr = WINTIMER_MALLOC_START + (id * sizeof(WinTimer));
	
	if(WinTimerFlag[id]==TRUE)
	{
		win_timer = (WinTimer *)addr;
//		win_timer->Status=START;
//		win_timer->Count=0;
        win_timer->Interval = interval;
		//Dprintf("StartTimer:: addr=%08x\n", addr);
	}	
	else
	{
		Dprintf("SetTimerInterval:: id Not Found id=%d\n", id);
	}
	return(1);
}



int StopTimer(int id)
{
	WinTimer *win_timer;
	U32 addr = WINTIMER_MALLOC_START + (id * sizeof(WinTimer));
	
	if(WinTimerFlag[id]==TRUE)
	{
		win_timer = (WinTimer *)addr;
		win_timer->Status=STOP;
		win_timer->Count=0;
		//WinTimerFlag[id] = 0; //이건 클로즈할때나 사용한다.
        
	}	
	else
	{
		Dprintf("StopTimer:: id Not Found id=%d\n", id);
	}
	return(1);
}

int CloseTimer(int id)
{
	if(id >= 0 && id <64)
	{
		gp_WinManager->TimerCount--;
		WinTimerFlag[id] = FALSE;
	}
	else
		Dprintf("CloseTimer:: id=%d mistypeing\n", id);
	return(1);
}

int WindowCloseAuto(HANDLE parent, HANDLE handle)
{
	int			i;
	WinTimer 	*wintimer;
	U32			addr;
	
	for(i=0; i<64; i++) //총64개의 타이머중에서
	{
		if(WinTimerFlag[i] == TRUE) //가동중인 타이머를 검색후
		{
			addr = WINTIMER_MALLOC_START + (i * sizeof(WinTimer));

			wintimer = (WinTimer *)addr; //그타이머의 포인터를 얻은후
			if(parent == wintimer->Parent && handle == wintimer->Handle) //클로즈되는 윈도우의 핸들과 같으면
			{
				WinTimerFlag[i] = FALSE; 		//디저블시키고
				gp_WinManager->TimerCount--; 	//전체 카운트를 감소시킨다.
			}
		}
	}
	return(1);
}

#ifdef __cplusplus
}
#endif
