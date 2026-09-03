/******************************************************************************
*    	File name	: Message.c 
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
#include "DataLog.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
#ifndef PSRAM_USE
#define		MSG_VRAM_ADDRESS	0x6B1F2000	//메시지가 저장되는 시작 어드레스
#define		TIMER_VRAM_ADDRESS	0x6B212000	//타이머 메시지가 저장되는 시작 어드레스
#define		IOTIMER_VRAM_ADDRESS	0x6B232000	//IO 타이머 메시지가 저장되는 시작 어드레스
#endif

#ifdef PSRAM_USE
#define		MSG_VRAM_ADDRESS			0x645B1A00 //메시지가 저장되는 시작 어드레스
#define		TIMER_VRAM_ADDRESS		0x645B1A00+0x10000	//타이머 메시지가 저장되는 시작 어드레스
#define		IOTIMER_VRAM_ADDRESS	0x645B1A00+0x20000	//IO 타이머 메시지가 저장되는 시작 어드레스
#endif

/******************************************************************************
							Private & Local Variables
******************************************************************************/
volatile int gMsgWCount; 	//메시지 라이트 포인트
volatile int gMsgRCount;	//메시지 리드 포인트

volatile int gTimerWCount; 	//메시지 라이트 포인트
volatile int gTimerRCount;	//메시지 리드 포인트

volatile int gIOTimerWCount; 	//메시지 라이트 포인트
volatile int gIOTimerRCount;	//메시지 리드 포인트

volatile int gWinFocus;		//현재 포커스받은 윈도우 

MSG		*SystemMSG;			//메시지 시작포인터
MSG		*TimerMSG;			//메시지 시작포인터
MSG		*IOTimerMSG;		//메시지 시작포인터
/******************************************************************************
							Function Prototype
******************************************************************************/
void MessageQClear(void)
{
	U32		i;
	//gTimerRCount = 0;
	//gTimerWCount = 0;
	gMsgWCount = 0;
	gMsgRCount = 0;
	
	for(i=0; i< (4000 * sizeof(MSG)); i++)
	{
		*(U8 *)(MSG_VRAM_ADDRESS+i) = 0;
	}	
}

void TimerQClear(void)
{
	U32		i;
	//gTimerRCount = 0;
	//gTimerWCount = 0;
	gTimerWCount = 0;
	gTimerRCount = 0;
	
	for(i=0; i< (4000 * sizeof(MSG)); i++)
	{
		*(U8 *)(TIMER_VRAM_ADDRESS+i) = 0;
	}	
}

void SystemMSGInsert(MSG *message)
{
	static int first=0;
	U32		addr;
    
	if(first==0) //처음일경우는 메시지시작포인터에 어드레스를 할당한다.
	{
		first = 1;
		SystemMSG = (MSG *)MSG_VRAM_ADDRESS;
	}
	else
	{
		addr = MSG_VRAM_ADDRESS + (gMsgWCount * sizeof(MSG));	
		SystemMSG = (MSG *)addr;
	}
	SystemMSG->Parent = message->Parent;
	SystemMSG->Handle = message->Handle;
	SystemMSG->message = message->message;
	SystemMSG->wParam = message->wParam;
	SystemMSG->lParam = message->lParam;
//	SystemMSG->time = message->time; 		//시스템이 가동된시간이다.
	SystemMSG->pt = message->pt;		
//Dprintf("SystemMSGInsert::X=%d Y=%d\n", message->pt.x , message->pt.y);	
	gMsgWCount++;
	if(gMsgWCount > 4000)	//메시지버퍼가 4000을 초과하면 처음부터 다시 저장한다.
	{
		gMsgWCount = 0; first=0;
	}
}

void TimerMSGInsert(MSG *message)
{
	static int first=0;
	U32		addr;
	if(first==0) //처음일경우는 메시지시작포인터에 어드레스를 할당한다.
	{
		first = 1;
		TimerMSG = (MSG *)TIMER_VRAM_ADDRESS;
	}
	else
	{
		addr = TIMER_VRAM_ADDRESS + (gTimerWCount * sizeof(MSG));	
		TimerMSG = (MSG *)addr;
	}
	TimerMSG->Parent = message->Parent;
	TimerMSG->Handle = message->Handle;
	TimerMSG->message = message->message;
	TimerMSG->wParam = message->wParam;
	TimerMSG->lParam = message->lParam;
//	TimerMSG->time = message->time; 		//시스템이 가동된시간이다.
	TimerMSG->pt = message->pt;		
//Dprintf("TimerMSGInsert::X=%d Y=%d\n", message->pt.x , message->pt.y);	
	gTimerWCount++;
	if(gTimerWCount > 4000)	//메시지버퍼가 4000을 초과하면 처음부터 다시 저장한다.
	{
		gTimerWCount = 0; first=0;
	}
}

void IOTimerMSGInsert(MSG *message)
{
	static int first=0;
	U32		addr;
	if(first==0) //처음일경우는 메시지시작포인터에 어드레스를 할당한다.
	{
		first = 1;
		IOTimerMSG = (MSG *)IOTIMER_VRAM_ADDRESS;
	}
	else
	{
		addr = IOTIMER_VRAM_ADDRESS + (gIOTimerWCount * sizeof(MSG));	
		IOTimerMSG = (MSG *)addr;
	}
	IOTimerMSG->Parent = message->Parent;
	IOTimerMSG->Handle = message->Handle;
	IOTimerMSG->message = message->message;
	IOTimerMSG->wParam = message->wParam;
	IOTimerMSG->lParam = message->lParam;
//	IOTimerMSG->time = message->time; 		//시스템이 가동된시간이다.
	IOTimerMSG->pt = message->pt;		
	//Dprintf("TimerMSGInsert::X=%d Y=%d\n", message->pt.x , message->pt.y);	
	gIOTimerWCount++;
	if(gIOTimerWCount > 4000)	//메시지버퍼가 4000을 초과하면 처음부터 다시 저장한다.
	{
		gIOTimerWCount = 0; first=0;
	}

/*
	stIOProcess *io_process;
	
	//이코드는 인터럽트 루틴에서 직접 호출하게 테스트 했던 코드이다.
	io_process = FindIOProcess(message->Handle);			
	if(io_process==NULL)
	{
		Dprintf("DispatchMessage:: IOTIMER_EVENT->Handle Error hanlde=%d\n", message->Handle);
	}
	else if(io_process->TimerEvent != NULL) 
	{
		io_process->TimerEvent(message->Handle, message->wParam, 0, 0); 
	}
*/	
}


//이함수는 태스크중에서 가장많이 호출되는 함수이므로 시스템의 체크사항 점검은 이루틴에서 한다.
int GetMessage(MSG *lpMsg, MSG *timerMsg, MSG *iotimerMsg)
{
	MSG		*msg;
	U32		addr;
	int		ret1=0, ret2=0, ret3=0;
	//static int soundFlag=0;

	//사운드의 재생이 끝나면 인터럽트를 막아야한다. Once Try Edit
	
	if(gMsgWCount != gMsgRCount)
	{
		addr = MSG_VRAM_ADDRESS + (gMsgRCount * sizeof(MSG));	
		msg = (MSG *)addr;

		lpMsg->Parent = msg->Parent;	//Device ID
		lpMsg->Handle = msg->Handle;	//Not Use
		lpMsg->message = msg->message;	//Message Command
		lpMsg->wParam = msg->wParam;	//Not Use
	   	lpMsg->lParam = msg->lParam;	//Not Use
		//lpMsg->time = msg->time;		//System Time
		lpMsg->pt.x = msg->pt.x;		//Touch X
		lpMsg->pt.y = msg->pt.y;		//Touch Y
//Dprintf("GetMessage::Message=%04x X=%d Y=%d\n", lpMsg->message, lpMsg->pt.x , lpMsg->pt.y);	
		gMsgRCount++;
		if(gMsgRCount > 4000) //메시지 리드가 4000을 초과하면 처음부터 시작한다.
		{
			gMsgRCount=0;
		}
		ret1 = 0x01;
	}
	//윈도우용 타이머
	if(gTimerWCount != gTimerRCount)
	{
		addr = TIMER_VRAM_ADDRESS + (gTimerRCount * sizeof(MSG));	
		msg = (MSG *)addr;

		timerMsg->Parent = msg->Parent;	//Device ID
		timerMsg->Handle = msg->Handle;	//Not Use
		timerMsg->message = msg->message;	//Message Command
		timerMsg->wParam = msg->wParam;	//Not Use
	   	timerMsg->lParam = msg->lParam;	//Not Use
		//timerMsg->time = msg->time;		//System Time
		timerMsg->pt.x = msg->pt.x;		//Touch X
		timerMsg->pt.y = msg->pt.y;		//Touch Y
//Dprintf("GetMessage::Timer=%04x ID=%d\n", timerMsg->message, timerMsg->wParam);	
		gTimerRCount++;
		if(gTimerRCount > 4000) //메시지 리드가 4000을 초과하면 처음부터 시작한다.
		{
			gTimerRCount=0;
		}
		ret2 = 0x02;
	}
	//윈도우용 타이머
	if(gIOTimerWCount != gIOTimerRCount)
	{
		addr = IOTIMER_VRAM_ADDRESS + (gIOTimerRCount * sizeof(MSG));	
		msg = (MSG *)addr;

		iotimerMsg->Handle = msg->Handle;	//Device ID
		iotimerMsg->message = msg->message;	//Message Command
		iotimerMsg->wParam = msg->wParam;	//Not Use
	   	iotimerMsg->lParam = msg->lParam;	//Not Use
		//timerMsg->time = msg->time;		//System Time
		iotimerMsg->pt.x = msg->pt.x;		//Touch X
		iotimerMsg->pt.y = msg->pt.y;		//Touch Y
		//Dprintf("GetMessage::Timer=%04x ID=%d\n", timerMsg->message, timerMsg->wParam);	
		gIOTimerRCount++;
		if(gIOTimerRCount > 4000) //메시지 리드가 4000을 초과하면 처음부터 시작한다.
		{
			gIOTimerRCount=0;
		}
		ret3 = 0x04;
	}
	
	return(ret1 | ret2 | ret3);
}


/******************************************************************************
		Function name 			: DispatchMessage
		Function prototype 		: int DispatchMessage(MSG *lpMsg)
		Behavior description	: 윈도우의 시스템메시지분석함수이다. 
								  여기서 컨테이너 윈도우로 전달한다.
		Input parameter			: Handle:Windows Handle, 
		Output parameter 		: None
		Return parameter		: 0
		Required preconditions	: None
		Called functions		: 
		Revision 				: V0.1 
		Date 					: 2012/07/16
		Edit Level				: idnics
******************************************************************************/
extern int Encoder_Parsing(MSG *lpMsg);
extern volatile int 	gTouchX, gTouchY, gPress;
int DispatchMessage(MSG *lpMsg, MSG *timerMsg,  MSG *iotimerMsg, int kind)
{
	Windows		*window;
	stIOProcess *io_process;
	
	if(kind & 0x01)
	{
		switch(lpMsg->Parent)
		{
			case TOUCH_DEVICE:		
            { 
				if(gv.messageBoxDispFlag==0) {
                    if(gv.systemWatchTimerFlag==1) {//화면 감시용 버튼이 자동으로 눌리면 슬립모드 진행 타이머를 초기화 하지 않는다.
                    }
                    else {
                        gv.sleepModeTimerCntSecPv=0;//슬립모드 진입타이머 초기화
                        gv.wakeUpFlag=1;            //슬립모드에서 깨어난다.
                    }
                    gv.systemWatchTimerFlag=0;
				}
                Touch_Parsing(lpMsg); 
                //gv.screenWatchTimerCount=0;//그래픽 처리 이벤트 타이머가 초기화 안되면 리셋된다.    
                
                //Dprintf("EVENT1\n");
				/*
				static int	file_cnt=0;
				if(gTouchX < 100 && gTouchY < 100)
				{
					char	buf[64];
					Dprintf("SCREEN SAVE START\n");
					sprintf(buf,"%s_%d.bmp", "SCREEN_", file_cnt++);
					//strcpy(Disk_Kind,"1:");	//USB Select
					BMPCreateSD("0", "0", "1024", "768", buf);	
					
					Dprintf("SCREEN COPY\n ");
					
				}
				*/
            }
            break;
			case KEYPAD_DEVICE:		KeyPad_Parsing(lpMsg); break;
			case ENCODER_DEVICE:	Encoder_Parsing(lpMsg); break;
			default:	break;
		}
	}
	if(kind & 0x02)
	{
		//타이머 이벤트 전송
		if(timerMsg->message == TIMER_EVENT)	
		{
			if(timerMsg->Parent==NULL)	window = FindContainerWindow(timerMsg->Handle,"DispatchMessage");
			else 	window = FindWidgetChain(timerMsg->Parent, timerMsg->Handle);
			if(window==NULL)
			{
				//Dprintf("DispatchMessage:: TIMER_EVENT->Handle Error parent=%d hanlde=%d\n", timerMsg->Parent, timerMsg->Handle);
				return(0);
			}
			if(window->TimerEvent != NULL) 
			{
				window->TimerEvent(timerMsg->Parent, timerMsg->Handle, timerMsg->wParam, 0, 0); 
			}
		}
	}
	if(kind & 0x04)
	{
		// IO 타이머 이벤트 전송
		if(iotimerMsg->message == TIMER_EVENT)	
		{
			io_process = FindIOProcess(iotimerMsg->Handle);			
			if(io_process==NULL)
			{
				Dprintf("DispatchMessage:: IOTIMER_EVENT->Handle Error hanlde=%d\n", timerMsg->Handle);
				return(0);
			}
			if(io_process->TimerEvent != NULL) 
			{
				io_process->TimerEvent(iotimerMsg->Handle, iotimerMsg->wParam, 0, 0); 
				return(1);
			}
		}
	}	
	return(0);
}

int Encoder_Parsing(MSG *lpMsg)
{
    static PARAM old_key_value;
//	Windows	*win;
	Windows	*temp_window;
	//Graphics 	*Gr;
//	int		active_win_cnt;
	int		select_zorder=0;

	Windows	*window_Zorder1=NULL;
	Windows	*window_Zorder2=NULL;
	Windows	*window_Zorder3=NULL;
	Windows	*window_Zorder4=NULL;
	Windows	*window_Zorder5=NULL;
	
	int		win_cnt = GetWindowCount();	//총컨테이너 윈도우의 수를 구한다.
	int		hidden_cnt = GetHiddenWindowCount("Encoder_Parsing");
	int		i;
    
    select_zorder = select_zorder;
	window_Zorder2 = window_Zorder2;
    window_Zorder3 = window_Zorder3;
    window_Zorder4 = window_Zorder4;
    window_Zorder5 = window_Zorder5;
    
	PARAM	lParam=0;
	lParam =lParam;
	//Dprintf("Global Touch x=%d y=%d\n", lpMsg->pt.x, lpMsg->pt.y);
	//윈도우의 Z순서를 정렬한다.
	for(i=1; i <=5; i++)	// 컨테이너윈도우 5개를 검사하는데
	{
		temp_window = FindContainerWindow(i, "Encoder_Parsing");
		if(temp_window != NULL)
		{
			if(gp_WinManager->ContainerWin[i]==true) // 해당윈도우가 메모리에 있으면
			{
				if(temp_window->Zorder==1 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder1 = FindContainerChain(i,"Encoder_Parsing");
					//Dprintf("Touch_Parsing::Zorder1_Windows=%d\n", i);
				}
				if(temp_window->Zorder==2 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder2 = FindContainerChain(i,"Encoder_Parsing");
					//Dprintf("Touch_Parsing::Zorder2_Windows=%d\n", i);
				}
				if(temp_window->Zorder==3 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder3 = FindContainerChain(i, "Encoder_Parsing");
					//Dprintf("PaintDraw::Zorder3_Windows=%d\n", i);
				}
				if(temp_window->Zorder==4 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder4 = FindContainerChain(i, "Encoder_Parsing");
					//Dprintf("Touch_Parsing::Zorder4_Windows=%d\n", i);
				}
				if(temp_window->Zorder==5 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder5 = FindContainerChain(i, "Encoder_Parsing");
					//Dprintf("Touch_Parsing::Zorder5_Windows=%d\n", i);
				}
			} //윈도우의 Z순서대로 정렬한다.
		}
	}

	//엔코더의 번호를 구분한다.
    lParam = MAKELONG(lpMsg->lParam, lpMsg->wParam); 
	//Dprintf("%08x %d %d \n", lParam, lpMsg->lParam, lpMsg->wParam);
	
	//active_win_cnt =  win_cnt-hidden_cnt;		
	if(lpMsg->message==KEY_ON || lpMsg->message==KEY_ROT)
	{
		//키패드가 떠있으면 제일순위로 할당된다.
		if(gp_WinManager->InputWindow_No != 0)
		{
			temp_window = FindContainerWindow(gp_WinManager->InputWindow_No, "Encoder_Parsing");
			if(temp_window != NULL)
			{
                //lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
                old_key_value = lpMsg->wParam;
                gWinFocus = temp_window->Handle;
                if(temp_window->EncoderEvent != NULL)
                {
                    temp_window->EncoderEvent(temp_window->Parent, temp_window->Handle, lpMsg->message, lParam, NULL);
                }
                return(1);
            }
		}
		else if(gp_WinManager->VideoAlphaContainerWin==true)
		{
            //lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
            old_key_value = lpMsg->wParam;
            gWinFocus = gp_VideoAlpha_ContainerWindow->Handle;
            if(gp_VideoAlpha_ContainerWindow->EncoderEvent != NULL)
            {
                gp_VideoAlpha_ContainerWindow->EncoderEvent(gp_VideoAlpha_ContainerWindow->Parent, gp_VideoAlpha_ContainerWindow->Handle, lpMsg->message, lParam, NULL);
            }
            return(1);
		}
		else if(gp_WinManager->LcdAlphaContainerWin==true && gp_WinManager->LcdAlpha_Touch==ON)
		{
            //lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
            old_key_value = lpMsg->wParam;
            gWinFocus = gp_LcdAlpha_ContainerWindow->Handle;
            if(gp_LcdAlpha_ContainerWindow->EncoderEvent != NULL)
            {
                gp_LcdAlpha_ContainerWindow->EncoderEvent(gp_LcdAlpha_ContainerWindow->Parent, gp_LcdAlpha_ContainerWindow->Handle, lpMsg->message, lParam, NULL);
            }
            return(1);
		}
        else if(window_Zorder1->ShowStatus==SHOW)
        {
            //lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
            old_key_value = lpMsg->wParam;
            gWinFocus = window_Zorder1->Handle;
            if(window_Zorder1->EncoderEvent != NULL)
            {
                window_Zorder1->EncoderEvent(window_Zorder1->Parent, window_Zorder1->Handle, lpMsg->message, lParam, NULL);
            }
            return(1);            
        }
	}
	else if(lpMsg->message==KEY_OFF)
	{
        //Dprintf("Touch_Parsing::TOUCH_OFF Message Processing\n");
		if(gWinFocus== -1) return(0);
		temp_window = FindContainerWindow(gWinFocus, "Encoder_Parsing");

        //lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
		if(temp_window->EncoderEvent != NULL)
		{
			temp_window->EncoderEvent(temp_window->Parent, temp_window->Handle, KEY_OFF, old_key_value, NULL);
		}
		gWinFocus = -1;
	}

	return(1);
}

int KeyPad_Parsing(MSG *lpMsg)
{
    static PARAM old_key_value;
	PARAM	lParam=0;
//	Windows	*win;
	Windows	*temp_window;
	//Graphics 	*Gr;
//	int		active_win_cnt;
	int		select_zorder=0;

	Windows	*window_Zorder1=NULL;
	Windows	*window_Zorder2=NULL;
	Windows	*window_Zorder3=NULL;
	Windows	*window_Zorder4=NULL;
	Windows	*window_Zorder5=NULL;
	
	
	int		win_cnt = GetWindowCount();	//총컨테이너 윈도우의 수를 구한다.
	int		hidden_cnt = GetHiddenWindowCount("KeyPad_Parsing");
	int		i;

    
    select_zorder = select_zorder;
    window_Zorder2 = window_Zorder2;
    window_Zorder3 = window_Zorder3;
    window_Zorder4 = window_Zorder4;
    window_Zorder5 = window_Zorder5;
    lParam = lParam;
	//Dprintf("Global Touch x=%d y=%d\n", lpMsg->pt.x, lpMsg->pt.y);
	//윈도우의 Z순서를 정렬한다.
	for(i=1; i <=5; i++)	// 컨테이너윈도우 5개를 검사하는데
	{
		temp_window = FindContainerWindow(i, "KeyPad_Parsing");
		if(temp_window != NULL)
		{
			if(gp_WinManager->ContainerWin[i]==true) // 해당윈도우가 메모리에 있으면
			{
				if(temp_window->Zorder==1 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder1 = FindContainerChain(i,"KeyPad_Parsing");
					//Dprintf("Touch_Parsing::Zorder1_Windows=%d\n", i);
				}
				if(temp_window->Zorder==2 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder2 = FindContainerChain(i,"KeyPad_Parsing");
					//Dprintf("Touch_Parsing::Zorder2_Windows=%d\n", i);
				}
				if(temp_window->Zorder==3 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder3 = FindContainerChain(i, "KeyPad_Parsing");
					//Dprintf("PaintDraw::Zorder3_Windows=%d\n", i);
				}
				if(temp_window->Zorder==4 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder4 = FindContainerChain(i, "KeyPad_Parsing");
					//Dprintf("Touch_Parsing::Zorder4_Windows=%d\n", i);
				}
				if(temp_window->Zorder==5 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder5 = FindContainerChain(i, "KeyPad_Parsing");
					//Dprintf("Touch_Parsing::Zorder5_Windows=%d\n", i);
				}
			} //윈도우의 Z순서대로 정렬한다.
		}
	}

	//active_win_cnt =  win_cnt-hidden_cnt;		
	if(lpMsg->message==KEY_ON)
	{
		//키패드가 떠있으면 제일순위로 할당된다.
		if(gp_WinManager->InputWindow_No != 0)
		{
			temp_window = FindContainerWindow(gp_WinManager->InputWindow_No, "KeyPad_Parsing");
			if(temp_window != NULL)
			{
                lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
                old_key_value = lpMsg->wParam;
                gWinFocus = temp_window->Handle;
                if(temp_window->KeyEvent != NULL)
                {
                    temp_window->KeyEvent(temp_window->Parent, temp_window->Handle, KEY_ON, lpMsg->wParam, NULL);
                }
                return(1);
            }
		}
		else if(gp_WinManager->VideoAlphaContainerWin==true)
		{
            lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
            old_key_value = lpMsg->wParam;
            gWinFocus = gp_VideoAlpha_ContainerWindow->Handle;
            if(gp_VideoAlpha_ContainerWindow->KeyEvent != NULL)
            {
                gp_VideoAlpha_ContainerWindow->KeyEvent(gp_VideoAlpha_ContainerWindow->Parent, gp_VideoAlpha_ContainerWindow->Handle, KEY_ON, lpMsg->wParam, NULL);
            }
            return(1);
		}
		else if(gp_WinManager->LcdAlphaContainerWin==true && gp_WinManager->LcdAlpha_Touch==ON)
		{
            lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
            old_key_value = lpMsg->wParam;
            gWinFocus = gp_LcdAlpha_ContainerWindow->Handle;
            if(gp_LcdAlpha_ContainerWindow->KeyEvent != NULL)
            {
                gp_LcdAlpha_ContainerWindow->KeyEvent(gp_LcdAlpha_ContainerWindow->Parent, gp_LcdAlpha_ContainerWindow->Handle, KEY_ON, lpMsg->wParam, NULL);
            }
            return(1);
		}
        else if(window_Zorder1->ShowStatus==SHOW)
        {
            lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
            old_key_value = lpMsg->wParam;
            gWinFocus = window_Zorder1->Handle;
            if(window_Zorder1->KeyEvent != NULL)
            {
                window_Zorder1->KeyEvent(window_Zorder1->Parent, window_Zorder1->Handle, KEY_ON, lpMsg->wParam, NULL);
            }
            return(1);            
        }
	}
	else if(lpMsg->message==KEY_OFF)
	{
        //Dprintf("Touch_Parsing::TOUCH_OFF Message Processing\n");
		if(gWinFocus== -1) return(0);
		temp_window = FindContainerWindow(gWinFocus, "KeyPad_Parsing");

        lParam = MAKELONG(0, 0);    //키패드이므로 좌표필요없음
		if(temp_window->KeyEvent != NULL)
		{
			temp_window->KeyEvent(temp_window->Parent, temp_window->Handle, KEY_OFF, old_key_value, NULL);
		}
		gWinFocus = -1;
	}
	return(1);
}
//volatile int gTouchManagerFlag=0;
int Touch_Parsing(MSG *lpMsg)
{
	PARAM	lParam;
//	Windows	*win;
	Windows	*temp_window;
	//Graphics 	*Gr;
//	int		active_win_cnt;
	int		select_zorder=0;
    static char     winname[64];

	Windows	*window_Zorder1=NULL;
	Windows	*window_Zorder2=NULL;
	Windows	*window_Zorder3=NULL;
	Windows	*window_Zorder4=NULL;
	Windows	*window_Zorder5=NULL;
		
	int		win_cnt = GetWindowCount();	//총컨테이너 윈도우의 수를 구한다.
	int		hidden_cnt = GetHiddenWindowCount("Touch_Parsing");
	int		i;
    
    select_zorder = select_zorder;
    window_Zorder3 = window_Zorder3;
    window_Zorder4 = window_Zorder4;
    window_Zorder5 = window_Zorder5;
    
	//Dprintf("Global Touch x=%d y=%d\n", lpMsg->pt.x, lpMsg->pt.y);
	//윈도우의 Z순서를 정렬한다.
	for(i=1; i <=5; i++)	// 컨테이너윈도우 5개를 검사하는데
	{
		temp_window = FindContainerWindow(i, "Touch_Parsing");
		if(temp_window != NULL)
		{
			if(gp_WinManager->ContainerWin[i]==true) // 해당윈도우가 메모리에 있으면
			{
				if(temp_window->Zorder==1 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder1 = FindContainerChain(i,"Touch_Parsing");
					//Dprintf("Touch_Parsing::Zorder1_Windows=%d\n", i);
				}
				if(temp_window->Zorder==2 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder2 = FindContainerChain(i,"Touch_Parsing");
					//Dprintf("Touch_Parsing::Zorder2_Windows=%d\n", i);
				}
				if(temp_window->Zorder==3 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder3 = FindContainerChain(i, "Touch_Parsing");
					//Dprintf("PaintDraw::Zorder3_Windows=%d\n", i);
				}
				if(temp_window->Zorder==4 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder4 = FindContainerChain(i, "Touch_Parsing");
					//Dprintf("Touch_Parsing::Zorder4_Windows=%d\n", i);
				}
				if(temp_window->Zorder==5 && temp_window->ShowStatus==SHOW) 
				{
					window_Zorder5 = FindContainerChain(i, "Touch_Parsing");
					//Dprintf("Touch_Parsing::Zorder5_Windows=%d\n", i);
				}
			} //윈도우의 Z순서대로 정렬한다.
		}
	}

	//active_win_cnt =  win_cnt-hidden_cnt;
		
	if(lpMsg->message==TOUCH_ON)
	{
		//키패드가 떠있으면 제일순위로 할당된다.
		if(gp_WinManager->InputWindow_No != 0 )
		{
			temp_window = FindContainerWindow(gp_WinManager->InputWindow_No, "Touch_Parsing");
            //strcpy(winname, temp_window->Name);
			if(temp_window != NULL)
			{
				//터치좌표가 윈도우 안에 있는가?		
				if(temp_window->ShowStatus==SHOW && lpMsg->pt.x  >= temp_window->abs_sx &&  lpMsg->pt.y  >= temp_window->abs_sy && lpMsg->pt.x  <= temp_window->abs_ex && lpMsg->pt.y  <= temp_window->abs_ey)
				{
					//Dprintf("Touch_Parsing::TOUCH_ON Screen Point Match1\n");
					//좌표변환해야한다.
					lpMsg->pt.x = lpMsg->pt.x - temp_window->abs_sx; 
					lpMsg->pt.y = lpMsg->pt.y - temp_window->abs_sy; 
					lParam = MAKELONG(lpMsg->pt.x, lpMsg->pt.y);
					gWinFocus = temp_window->Handle;
					if(temp_window->MainFunction != NULL) //MainFunction은 makeWindow에서 할당해준다.
					{
						temp_window->MainFunction(temp_window->Parent, temp_window->Handle, TOUCH_ON, lParam, NULL);
					}
					return(1);
				}
				//입력윈도우가 히든상태이면  최상위윈도우(Z2)안에 있는가?		
				else if(window_Zorder2->ShowStatus==SHOW && lpMsg->pt.x  >= window_Zorder2->abs_sx &&  lpMsg->pt.y  >= window_Zorder2->abs_sy && lpMsg->pt.x  <= window_Zorder2->abs_ex && lpMsg->pt.y  <= window_Zorder2->abs_ey)
				{
					Dprintf("Touch_Parsing::TOUCH_ON Screen Point Match2\n");
					//좌표변환해야한다.
					lpMsg->pt.x = lpMsg->pt.x - window_Zorder2->abs_sx; 
					lpMsg->pt.y = lpMsg->pt.y - window_Zorder2->abs_sy; 
					lParam = MAKELONG(lpMsg->pt.x, lpMsg->pt.y);
					gWinFocus = window_Zorder2->Handle;
					if(window_Zorder2->MainFunction != NULL)
					{
						window_Zorder2->MainFunction(window_Zorder2->Parent, window_Zorder2->Handle, TOUCH_ON, lParam, NULL);
					}
				}				
			}
		}
		if(gp_WinManager->VideoAlphaContainerWin==true)
		{
			//터치좌표가 비디오윈도우 안에 있는가?		
			if(gp_VideoAlpha_ContainerWindow->ShowStatus==SHOW && lpMsg->pt.x  >= gp_VideoAlpha_ContainerWindow->abs_sx &&  lpMsg->pt.y  >= gp_VideoAlpha_ContainerWindow->abs_sy && lpMsg->pt.x  <= gp_VideoAlpha_ContainerWindow->abs_ex && lpMsg->pt.y  <= gp_VideoAlpha_ContainerWindow->abs_ey)
			{
				//Dprintf("Touch_Parsing::TOUCH_ON Screen Point Match\n");
				//좌표변환해야한다.
				lpMsg->pt.x = lpMsg->pt.x - gp_VideoAlpha_ContainerWindow->abs_sx; 
				lpMsg->pt.y = lpMsg->pt.y - gp_VideoAlpha_ContainerWindow->abs_sy; 
				lParam = MAKELONG(lpMsg->pt.x, lpMsg->pt.y);
				gWinFocus = gp_VideoAlpha_ContainerWindow->Handle;
                //strcpy(winname, gp_VideoAlpha_ContainerWindow->Name);
                
				if(gp_VideoAlpha_ContainerWindow->MainFunction != NULL)
				{
					gp_VideoAlpha_ContainerWindow->MainFunction(gp_VideoAlpha_ContainerWindow->Parent, gp_VideoAlpha_ContainerWindow->Handle, TOUCH_ON, lParam, NULL);
				}
				return(1);
			}
		}
		if(gp_WinManager->LcdAlphaContainerWin==true && gp_WinManager->LcdAlpha_Touch==ON)
		{
			//터치좌표가 LCD 알파윈도우 안에 있는가?				
			if(gp_LcdAlpha_ContainerWindow->ShowStatus==SHOW && lpMsg->pt.x  >= gp_LcdAlpha_ContainerWindow->abs_sx &&  lpMsg->pt.y  >= gp_LcdAlpha_ContainerWindow->abs_sy && lpMsg->pt.x  <= gp_LcdAlpha_ContainerWindow->abs_ex && lpMsg->pt.y  <= gp_LcdAlpha_ContainerWindow->abs_ey)
			{
				//Dprintf("Touch_Parsing::TOUCH_ON Screen Point Match\n");
				//좌표변환해야한다.
				lpMsg->pt.x = lpMsg->pt.x - gp_LcdAlpha_ContainerWindow->abs_sx; 
				lpMsg->pt.y = lpMsg->pt.y - gp_LcdAlpha_ContainerWindow->abs_sy; 
				lParam = MAKELONG(lpMsg->pt.x, lpMsg->pt.y);
				gWinFocus = gp_LcdAlpha_ContainerWindow->Handle;
                //strcpy(winname, gp_LcdAlpha_ContainerWindow->Name);                
				if(gp_LcdAlpha_ContainerWindow->MainFunction != NULL)
				{
					gp_LcdAlpha_ContainerWindow->MainFunction(gp_LcdAlpha_ContainerWindow->Parent, gp_LcdAlpha_ContainerWindow->Handle, TOUCH_ON, lParam, NULL);
				}
				return(1);
			}
		}
		//터치좌표가 최상위윈도우(Z1)안에 있는가?		
		if(window_Zorder1->ShowStatus==SHOW && lpMsg->pt.x  >= window_Zorder1->abs_sx &&  lpMsg->pt.y  >= window_Zorder1->abs_sy && lpMsg->pt.x  <= window_Zorder1->abs_ex && lpMsg->pt.y  <= window_Zorder1->abs_ey)
		{
			//gTouchManagerFlag=1;//터치가눌리면다음동작을체크한다.
			//Dprintf("gTouchManagerFlag=%d\n",gTouchManagerFlag);
			//Dprintf("Touch_Parsing::TOUCH_ON Screen Point Match3\n");
			//좌표변환해야한다.
			lpMsg->pt.x = lpMsg->pt.x - window_Zorder1->abs_sx; 
			lpMsg->pt.y = lpMsg->pt.y - window_Zorder1->abs_sy; 
			lParam = MAKELONG(lpMsg->pt.x, lpMsg->pt.y);
			gWinFocus = window_Zorder1->Handle;
            strcpy(winname, window_Zorder1->Name);       
			//Dprintf("TOUCH ON:: %s\n", winname);            
            
			if(window_Zorder1->MainFunction != NULL)
			{
				//Dprintf("wincow create\n");
				window_Zorder1->MainFunction(window_Zorder1->Parent, window_Zorder1->Handle, TOUCH_ON, lParam, NULL);
			} 
		}
		//다른 컨테이너윈도우와 겹치는지를 확인후 겹친 윈도우 최상위로 올린다.
		//모달윈도우(또는 입력윈도우)가 없을경우만 해당한다.
        /*
		else if(gp_WinManager->ModalWindow_No ==0 && gp_WinManager->InputWindow_No==0)
		{
			//Dprintf("Touch_Parsing:: Other Window Press\n");
			
			if((lpMsg->pt.x  >= window_Zorder2->abs_sx &&  lpMsg->pt.y  >= window_Zorder2->abs_sy && lpMsg->pt.x  <= window_Zorder2->abs_ex && lpMsg->pt.y  <= window_Zorder2->abs_ey) &&  window_Zorder2->ShowStatus==SHOW)
			{
				select_zorder = 2;
				gWinFocus = window_Zorder2->Handle;
		//Dprintf("Touch_Parsing::%s Window Select zorder=%d\n", window_Zorder2->Name, select_zorder);
			}
			else if((lpMsg->pt.x  >= window_Zorder3->abs_sx &&  lpMsg->pt.y  >= window_Zorder3->abs_sy && lpMsg->pt.x  <= window_Zorder3->abs_ex && lpMsg->pt.y  <= window_Zorder3->abs_ey) &&  window_Zorder3->ShowStatus==SHOW)
			{
				select_zorder = 3;
				gWinFocus = window_Zorder3->Handle;
		//Dprintf("Touch_Parsing::%s Window Select zorder=%d\n", window_Zorder2->Name, select_zorder);
			}
			else if((lpMsg->pt.x  >= window_Zorder4->abs_sx &&  lpMsg->pt.y  >= window_Zorder4->abs_sy && lpMsg->pt.x  <= window_Zorder4->abs_ex && lpMsg->pt.y  <= window_Zorder4->abs_ey) &&  window_Zorder4->ShowStatus==SHOW)
			{
				select_zorder = 4;
				gWinFocus = window_Zorder4->Handle;
		//Dprintf("Touch_Parsing::%s Window Select zorder=%d\n", window_Zorder2->Name, select_zorder);
			}
			else if((lpMsg->pt.x  >= window_Zorder5->abs_sx &&  lpMsg->pt.y  >= window_Zorder5->abs_sy && lpMsg->pt.x  <= window_Zorder5->abs_ex && lpMsg->pt.y  <= window_Zorder5->abs_ey) &&  window_Zorder5->ShowStatus==SHOW)
			{
				select_zorder = 5;
				gWinFocus = window_Zorder5->Handle;
		//Dprintf("Touch_Parsing::%s Window Select zorder=%d\n", window_Zorder2->Name, select_zorder);
			}

			if(select_zorder != 0)
			{			
				for(i=1; i<=5; i++)
				{
					win = FindContainerWindow(i, "Touch_Parsing");
					if(win != NULL)
					{
						if(win->ShowStatus==SHOW && win->Zorder < select_zorder)  
						{
							win->Zorder++;
						}
						else if(win->ShowStatus==SHOW && win->Zorder == select_zorder)  
						{
							win->Zorder=1;
							gWinFocus = -1;
						}
					}
				}
				//선택된 윈도우의 PaintDraw 를 호출해야한다.
				if(select_zorder==2)  win = window_Zorder2;
				else if(select_zorder==3)  win = window_Zorder3;
				else if(select_zorder==4)  win = window_Zorder4;
				else if(select_zorder==5)  win = window_Zorder5;

				Gr = GetGraphics(win->Parent, win->Handle, WINDOW_AREA); 	//그래픽영역을 얻는다.
				if(win->Style==XP_STYLE) win->StyleDrawEvent(win->Parent, win->Handle, 0, 0, win->Text);
				Gr->Draw(win->Parent, win->Handle, 0, 0, 0);

				//win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
				
				//Dprintf("Select WinName=%s win->Zorder=%d select_Zorder=%d\n", win->Name, win->Zorder, select_zorder);
			}			
		}
        */
	}
	else if(lpMsg->message==TOUCH_MOVE)
	{
		if(gWinFocus == -1) {
			return(0);
		}
		else
		{
			temp_window = FindContainerWindow(gWinFocus, "Touch_Parsing");
			//좌표변환해야한다.
			lpMsg->pt.x = lpMsg->pt.x - temp_window->abs_sx; 
			lpMsg->pt.y = lpMsg->pt.y - temp_window->abs_sy; 
			lParam = MAKELONG(lpMsg->pt.x, lpMsg->pt.y);
			if(temp_window->MainFunction != NULL)
			{
				temp_window->MainFunction(temp_window->Parent, temp_window->Handle, TOUCH_MOVE, lParam, NULL);
			} 
		}
	}
	else if(lpMsg->message==TOUCH_OFF)
	{
		//Dprintf("Touch_Parsing::TOUCH_OFF Message Processing\n");
		if(gWinFocus== -1) {			
			Dprintf("Touch_Parsing OFF EVENT :: 생성되지 않은 윈도우입니다.\n");
			return(0);
		}
		temp_window = FindContainerWindow(gWinFocus, "Touch_Parsing");
		//좌표변환해야한다.
		lpMsg->pt.x = lpMsg->pt.x - temp_window->abs_sx; 
		lpMsg->pt.y = lpMsg->pt.y - temp_window->abs_sy; 
		lParam = MAKELONG(lpMsg->pt.x, lpMsg->pt.y);
//Dprintf("TOUCH OFF--1:: %s\n", winname);            

		if(temp_window->MainFunction != NULL)
		{
//Dprintf("TOUCH OFF--2:: %s  Z=%d\n", temp_window->Name, temp_window->Zorder);            
//Dprintf("TOUCH OFF--3:: %d\n", GetWindowByName(winname));            
            
            //마우스가 OFF 될때 혹시 그윈도우가 Close 됬을때를 대비하기위함이다.
            //if(!strcmp(winname, temp_window->Name ))
            if(GetWindowByName(winname) != 0)
            {
                temp_window->MainFunction(temp_window->Parent, temp_window->Handle, TOUCH_OFF, lParam, NULL);
            } else {
				Dprintf("Touch_Parsing OFF EVENT :: 이벤트가 발생하지 않는 윈도우입니다.");
			}
		}
		gWinFocus = -1;

		/*
		//터치좌표가 최상위윈도우(Z1)안에 있는가?		
		if(lpMsg->pt.x  >= window_Zorder1->abs_sx &&  lpMsg->pt.y  >= window_Zorder1->abs_sy && lpMsg->pt.x  <= window_Zorder1->abs_ex && lpMsg->pt.y  <= window_Zorder1->abs_ey)
		{
			lParam = MAKELONG(lpMsg->pt.x, lpMsg->pt.y);
			//좌표변환해야한다.
		 	temp_window.ClickEvent(temp_window.Parent, temp_window.Handle, WM_CLICK, lParam, NULL);
		}
		*/
	}
	return(1);
}

/******************************************************************************
		Function name 			: ContainerMSGParsing
		Function prototype 		: int	ContainerMSGParsing(
										HANDLE handle, WCOMMAND command, 
										PARAM param, void *Data);
		Behavior description	: 윈도우의 메시지분석함수이다. 공통적으로 사용한다.
		Input parameter			: Handle:Windows Handle, 
		Output parameter 		: None
		Return parameter		: 0
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
int	ContainerMSGParsing(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows *win = FindContainerWindow(handle,"ContainerMSGParsing");	//컨테이너 윈도우포인터를 가지고온다.
	Windows *widget;
	PARAM	lParam;
	static int Container_Widget_Focus = -1;
	static int Container_Handle = -1;

	int tx = LOWORD(param);
	int ty = HIWORD(param);
	if(win == NULL)
	{
		Dprintf("ContainerMSGParsing:: Window Error\n");
		return(0);
	}
//Dprintf("\tContainerMSGParsing:: tx=%d ty=%d\n", tx, ty);
	//처음에 터치온된 윈도우를 반드시 기억해야한다.
	switch(command)
	{
		case WM_CREATE:		if(win->CreateEvent!=NULL) win->CreateEvent(parent, handle, command, param, Data); break;
		case WM_CLOSE:		if(win->CloseEvent!=NULL) win->CloseEvent(parent, handle, command, param, Data); break;
		case WM_PAINT:		if(win->PaintEvent!=NULL) win->PaintEvent(parent, handle, command, param, Data); break;
		case WM_MOUSEON:	if(win->MouseOnEvent!=NULL) win->MouseOnEvent(parent, handle, command, param, Data); break;
		case WM_MOUSEOFF:	if(win->MouseOffEvent!=NULL) win->MouseOffEvent(parent, handle, command, param, Data); break;
		case WM_MOUSEMOVE:	if(win->MouseMoveEvent!=NULL) win->MouseMoveEvent(parent, handle, command, param, Data); break;
		case WM_TIMER:		if(win->TimerEvent!=NULL) win->TimerEvent(parent, handle, command, param, Data); break;
		case WM_CLICK:		if(win->ClickEvent!=NULL) win->ClickEvent(parent, handle, command, param, Data); break;
		case WM_GESTURE:	if(win->GestureEvent!=NULL) win->GestureEvent(parent, handle, command, param, Data); break;
		case WM_HIDDEN:		if(win->HiddenEvent!=NULL) win->HiddenEvent(parent, handle, command, param, Data); break;
		//case TOUCH_ON:
		//case TOUCH_MOVE:
		//case TOUCH_OFF:
		default: break;
	}

	//컨테이너의 경우는 parent는 0 이고 handle이 윈도우번호이다.
	/*
	switch(handle)
	{
		case 1: 	widget = gp_WidgetWindow_1;			break;
		case 2: 	widget = gp_WidgetWindow_2;			break;
		case 3: 	widget = gp_WidgetWindow_3;			break;
		case 4: 	widget = gp_WidgetWindow_4;			break;
		case 5: 	widget = gp_WidgetWindow_5;			break;
		case 6: 	widget = gp_LcdAlphaWidgetWindow;	break;
		case 7: 	widget = gp_VideoAlphaWidgetWindow;	break;
		default:	Dprintf("ContainerMSGParsing::Windows_no mistyping\n"); break;
	}
	*/

	widget = FindNullWindowChain(handle);
//Dprintf("\tContainerMSGParsing:: handle=%d widget=%08x\n", handle, widget);
	
	if(command==TOUCH_ON)
	{
//Dprintf("\tContainerMSGParsing:: TOUCH-ON---1\n");
		if(gp_WinManager->WidgetCount[handle] !=0) //위젯이 존재하는경우
		{
			while(1)
			{
				if(widget->ShowStatus==SHOW)
				{
					if(tx  >= widget->sx &&  ty  >= widget->sy && tx  <=  widget->sx+ widget->width && ty <=  widget->sy + widget->height)
					{
						tx = tx - widget->sx; 
						ty = ty - widget->sy; 
						lParam = MAKELONG(tx, ty);

					 	if(widget->MainFunction != NULL)
						{
							Container_Handle = handle;
							Container_Widget_Focus = widget->Handle; //TOUCH ON메시지를 받은 위젯의 핸들을 기억한다.
							//Dprintf("\tContainerMSGParsing:: Send WidgetFunction parent=%d handle=%d\n", widget->Parent,widget->Handle);
							//Dprintf("W:BackUp Handle parent=%d handle=%d\n", Container_Handle, Container_Widget_Focus);
							widget->MainFunction( widget->Parent,  widget->Handle, TOUCH_ON, lParam, NULL);
						}
						return(1);
					}				
				}
				if(widget->PreviousWindow ==NULL) break;
				widget = (Windows *)widget->PreviousWindow;
			}
		}
		Container_Handle = parent;
		Container_Widget_Focus = handle; //TOUCH ON메시지를 받은 위젯의 핸들을 기억한다.
		//Dprintf("C:BackUp Handle parent=%d handle=%d\n", Container_Handle, Container_Widget_Focus);
//Dprintf("\tContainerMSGParsing:: TOUCH-ON---2\n");
		
		if(win->MouseOnEvent != NULL) win->MouseOnEvent(parent, handle, command, param, Data);
	}
	else if	(command==TOUCH_MOVE)
	{
		if(Container_Handle==NULL) //전에눌린윈도우가 컨테이너일경우
		{
			win = FindContainerWindow(Container_Widget_Focus, "ContainerMSGParsing");
			if(win->MouseMoveEvent != NULL) win->MouseMoveEvent(Container_Handle, Container_Widget_Focus, command, param, Data);
		}
		else
		{
			widget = FindWidgetChain(Container_Handle, Container_Widget_Focus);
			tx = tx - widget->sx; 
			ty = ty - widget->sy; 
			lParam = MAKELONG(tx, ty);	
			if(widget->MainFunction	!= NULL)
				widget->MainFunction( widget->Parent,  widget->Handle, TOUCH_MOVE, lParam, NULL);
		}
	}
	else if(command==TOUCH_OFF)
	{
		if(Container_Handle==NULL) //전에눌린윈도우가 컨테이너일경우
		{
			win = FindContainerWindow(Container_Widget_Focus, "ContainerMSGParsing");
			if(win->MouseOffEvent != NULL) win->MouseOffEvent(Container_Handle, Container_Widget_Focus, command, param, Data);
		}
		else
		{
			widget = FindWidgetChain(Container_Handle, Container_Widget_Focus);
			tx = tx - widget->sx; 
			ty = ty - widget->sy; 
			lParam = MAKELONG(tx, ty);			
			if(widget->MainFunction	!= NULL)
				widget->MainFunction( widget->Parent,  widget->Handle, TOUCH_OFF, lParam, NULL);		
		}
		Container_Widget_Focus = -1;
		Container_Handle = -1;
	}
	return(0);
}


/******************************************************************************
		Function name 			: WidgetMSGParsing
		Function prototype 		: int	WidgetMSGParsing(
										HANDLE handle, WCOMMAND command, 
										PARAM param, void *Data);
		Behavior description	: 윈도우의 메시지분석함수이다. 공통적으로 사용한다.
		Input parameter			: Handle:Windows Handle, 
		Output parameter 		: None
		Return parameter		: 0
		Required preconditions	: None
		Called functions		: MakeWinidow()
		Revision 				: V0.1 
		Date 					: 2012/07/10
		Edit Level				: idnics
******************************************************************************/
int	WidgetMSGParsing(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows *win = FindWidgetChain(parent, handle);	//위젯 윈도우포인터를 가지고온다.

	//PARAM	lParam;
	static int Container1_Widget_Focus = -1;

	//int tx = LOWORD(param);
	//int ty = HIWORD(param);

	Container1_Widget_Focus = Container1_Widget_Focus;
	if(win->ShowStatus == HIDDEN) return(0);
//Dprintf("\tWidgetMSGParsing:: tx=%d ty=%d\n", tx, ty);	
	switch(command)
	{
		case WM_CREATE:		if(win->CreateEvent!=NULL)   win->CreateEvent(parent, handle, command, param, Data); break;
		case WM_CLOSE:		if(win->CloseEvent!=NULL)  win->CloseEvent(parent, handle, command, param, Data); break;
		case WM_PAINT:		if(win->PaintEvent!=NULL)  win->PaintEvent(parent, handle, command, param, Data); break;
		case WM_MOUSEON:	if(win->MouseOnEvent!=NULL)  win->MouseOnEvent(parent, handle, command, param, Data); break;
		case WM_MOUSEOFF:	if(win->MouseOffEvent!=NULL) win->MouseOffEvent(parent, handle, command, param, Data); break;
		case WM_MOUSEMOVE:	if(win->MouseMoveEvent!=NULL)  win->MouseMoveEvent(parent, handle, command, param, Data); break;
		case WM_TIMER:		if(win->TimerEvent!=NULL)  win->TimerEvent(parent, handle, command, param, Data); break;
		case WM_CLICK:		if(win->ClickEvent!=NULL)  win->ClickEvent(parent, handle, command, param, Data); break;
		case WM_GESTURE:	if(win->GestureEvent!=NULL)  win->GestureEvent(parent, handle, command, param, Data); break;
		case WM_HIDDEN:		if(win->HiddenEvent!=NULL)  win->HiddenEvent(parent, handle, command, param, Data); break;
		default: break;
	}

	if(command==TOUCH_ON)
	{
		Container1_Widget_Focus = 1;
		if(win->MouseOnEvent != NULL)
			win->MouseOnEvent(parent, handle, command, param, Data);
		//즉시동작을 이루기위해 마우스온만되도 클릭이벤트가 발생하게 하였다.
		/*
		if(tx >= 0 && ty >= 0 && tx <= win->width && ty <= win->height)
		{
			if(win->ClickEvent != NULL)
				win->ClickEvent(parent, handle, command, param, Data);
		}		
		*/
	}
	else if	(command==TOUCH_MOVE)
	{
		if(win->MouseMoveEvent != NULL)
			win->MouseMoveEvent(parent, handle, command, param, Data);
	}
	else if(command==TOUCH_OFF)
	{
		if(win->MouseOffEvent != NULL)
			win->MouseOffEvent(parent, handle, command, param, Data);
		
		//노이즈로 인해 클릭이벤트가 발생안하는경우가 있어서 좌표비교는 생략한다.
		if(win->ClickEvent != NULL)
			win->ClickEvent(parent, handle, command, param, Data);
		/*
		if(tx >= 0 && ty >= 0 && tx <= win->width && ty <= win->height)
		{
			if(win->ClickEvent != NULL)
				win->ClickEvent(parent, handle, command, param, Data);
		}
		*/
		
	}
	return(0);
}

int SendMessage(HANDLE src_parent, HANDLE src_handle, HANDLE dest_parent, HANDLE dest_handle, WCOMMAND message, PARAM param)
{
	Windows	*window;
	PARAM		send_param;
	int 		id=0;

	send_param   = MAKELONG(src_parent, src_handle);
	
	if(dest_parent==NULL)	//Container Window
	{
		window = FindContainerWindow(dest_handle, "SendMessage");
	}
	else 				//Widget Window
	{
		window = FindWidgetChain(dest_parent, dest_handle);
	}
	if(window==NULL)
	{
		Dprintf("SendMessage:: Handle Error parent=%d hanlde=%d\n", dest_parent, dest_handle);
		return(0);
	}
	switch(message)
	{
		case WM_TIMER: 		
		{
			id = param;		
			if(window->TimerEvent != NULL) window->TimerEvent(dest_parent, dest_parent, id, send_param, NULL); 
			break;
		}
		case WM_PAINT: 		if(window->PaintEvent != NULL) window->PaintEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		case WM_MOVE: 		if(window->MoveEvent != NULL) window->MoveEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		case WM_CLICK: 		if(window->ClickEvent != NULL) window->ClickEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		case WM_MOUSEON: 	if(window->MouseOnEvent != NULL) window->MouseOnEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		case WM_MOUSEOFF: 	if(window->MouseOffEvent != NULL) window->MouseOffEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		case WM_MOUSEMOVE: 	if(window->MouseMoveEvent != NULL) window->MouseMoveEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		case WM_CHANGE: 	if(window->ChangeEvent != NULL) window->ChangeEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		case WM_CONTROL: 	if(window->ControlEvent != NULL) window->ControlEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		case WM_CLOSE: 		if(window->CloseEvent != NULL) window->CloseEvent(dest_parent, dest_parent, id, send_param, NULL); break;
		default: break;
	}
	return(1);
}

int SetWindowText(HANDLE src_parent, HANDLE src_handle, HANDLE dest_parent, HANDLE dest_handle, char *text)
{
	Windows	*window;

	if(dest_parent==NULL) window = FindContainerWindow(dest_handle, "SetWindowText");
	else  window = FindWidgetChain(dest_parent, dest_handle);
	if(window==NULL)
	{
		Dprintf("SetWindowText:: Handle Error parent=%d hanlde=%d\n", dest_parent, dest_handle);
		return(0);
	}
	strcpy(window->Text, text);
	if(!SendMessage(src_parent, src_handle, dest_parent, dest_handle, WM_CHANGE, TEXT_CHANGE))
	{
		Dprintf("SetWindowText:: Send Message Error\n");
	}
	return(1);
}

#ifdef __cplusplus
}
#endif


