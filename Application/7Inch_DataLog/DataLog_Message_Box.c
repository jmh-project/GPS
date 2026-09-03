/******************************************************************************
*    	File name	: KeyPad.c 
*		Description : TEECO Project User Program File
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
#include "dos.h"
#include "DataLog.h"
/******************************************************************************
							Constant & Macros
******************************************************************************/
#define		MAX_INPUT_DIGIT		20
//Sound 효과음의 정의
#define		TOUCH_SOUND			1	 
/******************************************************************************
							Private & Local Variables
******************************************************************************/
static char DataLog_MessageBox_Text[128];
/******************************************************************************
							Function Prototype
******************************************************************************/
U32 gMessageTextId = 0;		
int Remove(char *name);
int DiskFormat(char  *disk);
extern int gIDChangeFlag;
static int	  DataLog_MessageBox_ClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "");
	Windows		*parent_win = FindContainerWindow(parent,  "");
	if(parent_win == NULL)
	{
		Dprintf("SConfirm_ClickEvent:: Windows Error\n");
	}
	int waitCount=0;
	if(win->ID==1) {//OK
		Dprintf("win->Text =%s\n",parent_win->Text);
		if(!strcmp(parent_win->Text,"Message"))
		{
			gv.ChangeNameFlag=ON;
		}
		else if(!strcmp(parent_win->Text,"공장초기화"))
		{
			SetWidgetText(gMessageTextId, "공장초기화 진행 중 입니다.");
            //SetWidgetText(gMessageTextId, "공장초기화 진행 중 입니다.");
			SD_Mount();
			Remove("DongMoonFlashData.txt");
            Remove("DongMoonFlashData2.txt");
			
			//Format(SD);			
			Delay(500);//저장안정시간
			while(1) {
				Uart_SendByte7(0x02);
				Uart_SendByte7('P');
				Uart_SendByte7('O');
				Uart_SendByte7('W');
				Uart_SendByte7('E');
				Uart_SendByte7('R');
				Uart_SendByte7(0x03);	
				if(waitCount>=5) {
					NVIC_SystemReset();
				} else {
					waitCount++;
					Delay(3000);
				} 
			}
		}
		else if(!strcmp(parent_win->Text,"안전모드"))
		{
			SD_Mount();
			Delay(1000);//저장안정시간
			while(1) {
				Uart_SendByte7(0x02);
				Uart_SendByte7('P');
				Uart_SendByte7('O');
				Uart_SendByte7('W');
				Uart_SendByte7('E');
				Uart_SendByte7('R');
				Uart_SendByte7(0x03);	
				if(waitCount>=5) {
					NVIC_SystemReset();
				} else {
					waitCount++;
					Delay(3000);
				} 
			}
		}
		else if(!strcmp(parent_win->Text,"절전모드"))
		{
		}
		else if(!strcmp(parent_win->Text,"ID변경"))
		{
            gIDChangeFlag=1;
            Dprintf("아이디 변경\n");
		}
		else if(!strcmp(parent_win->Text,"복원파일생성"))
		{
            EventMessage("복원파일생성\n");
            Remove("DongMoonFlashData2.txt");
            DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData2.txt");
		}
        else if(!strcmp(parent_win->Text,"업데이트")) {
            gv.usbProgramUpdateFlag=1;
        }
#ifdef  ENABLE_TUBIDITY        
		else if(!strcmp(parent_win->Text,"TUBIDITY"))
		{
			flash.TUB_Process_Control_Flag=ON;
		}
#endif
        else if(!strcmp(parent_win->Text,"메모리삭제"))
        {
			SetWidgetText(gMessageTextId, "메모리삭제 진행 중 입니다.");
			Remove("Average5MinData.csv");
			Remove("Average60MinData.csv");
			Remove("TimeSettingv.csv");          
#ifdef HOMOGENIZATION
			Remove("RealData.csv");          
			Remove("AverageData.csv");          
#endif            
        }
		gv.messageBoxDispFlag=0;
		CloseWindow(NULL, parent);
	}
	else if(win->ID==2) {//CANCEL
		if(!strcmp(parent_win->Text,"Message"))
		{
			gv.ChangeNameFlag=OFF;
		}
		else if(!strcmp(parent_win->Text,"ID변경"))
		{
            gIDChangeFlag=0;
            Dprintf("아이디 변경취소\n");
		}
        else if(!strcmp(parent_win->Text,"업데이트")) {
            gv.usbProgramUpdateFlag=0;
        }
#ifdef  ENABLE_TUBIDITY
		else if(!strcmp(parent_win->Text,"TUBIDITY"))
		{
			flash.TUB_Process_Control_Flag=OFF;
		}
#endif        
		else if(!strcmp(parent_win->Text,"POWER"))
		{
			flash.powerControl=0;//POWER ON MODE
		}
		else if(!strcmp(parent_win->Text,"절전모드"))
		{
			gv.sleepModeTimerCntSecPv=0;
			Dprintf("절전모드 타이머 초기화\n");
		}
		else if(!strcmp(parent_win->Text,"ERROR"))
		{
			Dprintf("LAN 에러체크 타이머 초기화\n");
		}
		gv.messageBoxDispFlag=0;
		CloseWindow(NULL, parent);
	}
    else if(win->ID==3) {
		gv.messageBoxDispFlag=0;
		CloseWindow(NULL, parent);
    }
	return(1);
}

static int	  DataLog_MessageBox_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	int currentTimeBackColor, currentTimeFontColor; 
	Gr->FillRect(0, 0, 0, 0);//Dummy
	PaintDrawControl(OFF);
	if(flash.systemColorNo!=6) {		
		Gr->SetColor(RGB565CONVERT(0,  0,  0));
		Gr->FillRect(0, 0, win->width-2, win->height-2);
		Gr->SetColor(clWhite);
		Gr->Rectangle(0, 0, win->width-2, win->height-2);
		currentTimeBackColor=clBlack;
		currentTimeFontColor=clWhite;
	}
	else {
		Gr->SetColor(RGB565CONVERT(230,  230,  230));
		Gr->FillRect(0, 0, win->width-2, win->height-2);
		currentTimeBackColor=RGB565CONVERT(230,  230,  230);
		currentTimeFontColor=clBlack;
	}

	//TitleBar(handle, 0, 0, 500-2, 50, 1, win->Text, clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	//BIcon(handle, 20, 50, 48, 48 , "48G_Warning_Icon1.bmp", RGB565CONVERT(230,  230,  230));

	gMessageTextId = SLabel(handle, 0, 50, 500-10, 48, DataLog_MessageBox_Text, currentTimeFontColor, currentTimeBackColor,  0);
	U32 btnHandle  = SButton(handle, 500 - 20 - 200, 180, 200, 54, " ", "OK",  PUSH_BUTTON, 1, DataLog_MessageBox_ClickEvent);
	
	if((!strcmp(DataLog_MessageBox_Text,"절전모드 취소합니다.")) || (!strcmp(DataLog_MessageBox_Text,"절전모드 실행합니다."))) {
		Dprintf("Cancel 버튼 삭제\n");
	} else if( !strcmp(DataLog_MessageBox_Text,"RTU 복원파일을 생성합니다.")) {
        Dprintf("Cancel 버튼 삭제\n");
    } else if( !strcmp(DataLog_MessageBox_Text,"대기중교정")) {
        Dprintf("Cancel 버튼 삭제\n");
    } else if( !strcmp(DataLog_MessageBox_Text,"방향을 선택하세요.")) {
        SetWidgetText(btnHandle,"오른쪽");
        SButton(handle, 20, 180, 200, 54, " ", "왼쪽",  PUSH_BUTTON, 2, DataLog_MessageBox_ClickEvent);
        SButton(handle, 500-50, 1, 50, 48, " ", "X",  PUSH_BUTTON, 3, DataLog_MessageBox_ClickEvent);
    } else if( !strcmp(DataLog_MessageBox_Text,"프로그램 업데이트를 실행합니다.")) {        
        SButton(handle, 20, 180, 200, 54, " ", "Cancel",  PUSH_BUTTON, 2, DataLog_MessageBox_ClickEvent);
    } else if((!strcmp(DataLog_MessageBox_Text,"정상 처리되었습니다.")) || (!strcmp(DataLog_MessageBox_Text,"잘못된 입력입니다."))) {
		Dprintf("Cancel 버튼 삭제\n");
	} else if( !strcmp(DataLog_MessageBox_Text,"TMS 항목코드가 잘 못 입력되었습니다.")) {
		Dprintf("Cancel 버튼 삭제\n");
	} else {
		SButton(handle, 20, 180, 200, 54, " ", "Cancel",  PUSH_BUTTON, 2, DataLog_MessageBox_ClickEvent);
	}
	
	
	if(!strcmp(DataLog_MessageBox_Text,"절전모드 실행합니다.")) {
        SLabel(handle, 0, 50+50, 499, 48, "600초후 실행 / 교정중에는 실행안됨", currentTimeFontColor, currentTimeBackColor,  0);
    }
	PaintDrawControl(ON);
	return(1);
}

static int	  DataLog_MessageBox_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data){
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);	
	PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int DataLog_MessageBox_Window(char *title, char *text) {
	if(gv.messageBoxDispFlag==0) {               
        if(flash.powerControl==2) {//저전력 모드에서는 동작하지 않는다.
            return 0;
        }
        else {            
            gv.messageBoxDispFlag=1; 
        }
	}
    else {
        Dprintf("메시지박스가 생성됨\n");
        return 0;//메시지박스 중복출력을 막는다.
    }
	
	if(GetWindowName("DataLog_MessageBox_Window")) 
	{
		Dprintf("DataLog_MessageBox_Window:: Windows already Create\n");
		return 0;		
	}		
	int sx = (LCD_X - 500 ) / 2;
	int sy = (LCD_Y - 300 ) / 2;
	strcpy(DataLog_MessageBox_Text, text);
	makeWindow(
				LCD,
				"DataLog_MessageBox_Window",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				500, 300,				// Size
				SHOW,					// Create after Show
				DataLog_MessageBox_CreateEvent,		// CreateEvent
				DataLog_MessageBox_PaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				NULL,					// TimerEvent
				NULL,  					// ControlEvent
				NULL,  					// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				NULL,					// PARAMETER
				NULL,					// Normal Touch Screen
				0,						// Touch Screen
				title,					// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	return(1);
}
#ifdef __cplusplus
}
#endif
