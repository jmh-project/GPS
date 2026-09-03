#ifdef __cplusplus
 extern "C" {
#endif

#include "TEECO_System.h"
#include "Windows.h"
#include "dos.h"
#include "DataLog.h"
     
#if defined (SPECTRO_D_ENABLE) || defined (SPECTRO_M_ENABLE)
char ULIK_MessageBox_Text[128];
char ULIK_Message_Item[128];
extern int ulikZeroCalibrationFlag;
int ULIK_MessageBox_ClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "");
	Windows		*parent_win = FindContainerWindow(parent,  "");
	if(parent_win == NULL)
	{
		Dprintf("SConfirm_ClickEvent:: Windows Error\n");
	}
	if(win->ID==1) {//OK
        if(strcmp(ULIK_Message_Item,"COD")==0) {            
            if(strcmp(ULIK_MessageBox_Text,"교정 값을 적용합니다.")==0) {
                gv.ulikMessageStep=0;   //출력 메시지가 전환된다.
                gv.ulikCalCmdSendNo=1;  //교정명령을 송신한다.
                Dprintf("ULIK COD 설정값 송신 :: 교정값을 적용합니다.\n");
            }
            else if(strcmp(ULIK_MessageBox_Text,"마지막 교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=2;
                Dprintf("ULIK COD 설정값 송신 :: 마지막 교정 값을 초기화합니다.\n");
                Dprintf("ULIK 마지막 교정 포인트 = %d\n",gv.ulikCalStep);
            }
            else if(strcmp(ULIK_MessageBox_Text,"모든 교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=3;
                gv.ulikCalStep=0;
                gv.ulikMessageStep=0;
                Dprintf("ULIK COD 설정값 송신 :: 모든 교정 값을 초기화합니다.\n");
            }
        }
        else if(strcmp(ULIK_Message_Item,"TOC")==0) {            
            if(strcmp(ULIK_MessageBox_Text,"교정 값을 적용합니다.")==0) {
                gv.ulikMessageStep=0;   //출력 메시지가 전환된다.
                gv.ulikCalCmdSendNo=1;  //교정명령을 송신한다.
                Dprintf("ULIK TOC 설정값 송신 :: 교정값을 적용합니다.\n");
            }
            else if(strcmp(ULIK_MessageBox_Text,"마지막 교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=2;
                Dprintf("ULIK TOC 설정값 송신 :: 마지막 교정 값을 초기화합니다.\n");
                Dprintf("ULIK 마지막 교정 포인트 = %d\n",gv.ulikCalStep);
            }
            else if(strcmp(ULIK_MessageBox_Text,"모든 교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=3;
                gv.ulikCalStep=0;
                gv.ulikMessageStep=0;
                Dprintf("ULIK TOC 설정값 송신 :: 모든 교정 값을 초기화합니다.\n");
            }
        }
        else if(strcmp(ULIK_Message_Item,"NO3_N")==0) {            
            if(strcmp(ULIK_MessageBox_Text,"교정 값을 적용합니다.")==0) {
                gv.ulikMessageStep=0;   //출력 메시지가 전환된다.
                gv.ulikCalCmdSendNo=1;  //교정명령을 송신한다.
                Dprintf("ULIK NO3_N 설정값 송신 :: 교정값을 적용합니다.\n");
            }
            else if(strcmp(ULIK_MessageBox_Text,"교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=2;
                Dprintf("ULIK NO3_N 설정값 송신 :: 마지막 교정 값을 초기화합니다.\n");
                Dprintf("ULIK 마지막 교정 포인트 = %d\n",gv.ulikCalStep);
            }
            else if(strcmp(ULIK_MessageBox_Text,"모든 교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=3;
                gv.ulikCalStep=0;
                gv.ulikMessageStep=0;
                Dprintf("ULIK NO3_N 설정값 송신 :: 모든 교정 값을 초기화합니다.\n");
            }
        }
        else if(strcmp(ULIK_Message_Item,"TUR")==0) {            
            if(strcmp(ULIK_MessageBox_Text,"교정 값을 적용합니다.")==0) {
                gv.ulikMessageStep=0;   //출력 메시지가 전환된다.
                gv.ulikCalCmdSendNo=1;  //교정명령을 송신한다.
                Dprintf("ULIK TUR 설정값 송신 :: 교정값을 적용합니다.\n");
            }
            else if(strcmp(ULIK_MessageBox_Text,"교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=2;
                Dprintf("ULIK TUR 설정값 송신 :: 마지막 교정 값을 초기화합니다.\n");
                Dprintf("ULIK 마지막 교정 포인트 = %d\n",gv.ulikCalStep);
            }
            else if(strcmp(ULIK_MessageBox_Text,"모든 교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=3;
                gv.ulikCalStep=0;
                gv.ulikMessageStep=0;
                Dprintf("ULIK TUR 설정값 송신 :: 모든 교정 값을 초기화합니다.\n");
            }
        }
        else if(strcmp(ULIK_Message_Item,"COND")==0) {
            if(strcmp(ULIK_MessageBox_Text,"교정 값을 적용합니다.")==0) {
                gv.ulikMessageStep=0;   //출력 메시지가 전환된다.
                gv.ulikCalCmdSendNo=1;  //교정명령을 송신한다.
            }
            else if(strcmp(ULIK_MessageBox_Text,"모든 교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=3;
                gv.ctdCalClearStep=0;
                Dprintf("CTD COND 설정값 송신 :: 모든 교정 값을 초기화합니다.\n");
            }
        }
        else if(strcmp(ULIK_Message_Item,"Depth")==0) {
            if(strcmp(ULIK_MessageBox_Text,"교정 값을 적용합니다.")==0) {
                gv.ulikMessageStep=0;   //출력 메시지가 전환된다.
                gv.ulikCalCmdSendNo=1;  //교정명령을 송신한다.
            }
            else if(strcmp(ULIK_MessageBox_Text,"모든 교정 값을 초기화합니다.")==0) {
                gv.ulikCalCmdSendNo=3;
                gv.ctdCalClearStep=0;
                Dprintf("CTD COND 설정값 송신 :: 모든 교정 값을 초기화합니다.\n");
            }
        }
        if(strcmp(ULIK_Message_Item,"Zero calibration")==0) {
            ulikZeroCalibrationFlag=ON;//교정 명령을 송신한다.
        }
		CloseWindow(NULL, parent);
	}
    else if(win->ID==2) {//Cancel
        if(strcmp(ULIK_Message_Item,"COD")==0) {
            gv.ulikMessageStep=0;
            gv.ulikCalCmdSendNo=7;
        }
        else if(strcmp(ULIK_Message_Item,"TOC")==0) {
            gv.ulikMessageStep=0;
            gv.ulikCalCmdSendNo=7;
        }
        else if(strcmp(ULIK_Message_Item,"NO3_N")==0) {
            gv.ulikMessageStep=0;
            gv.ulikCalCmdSendNo=7;
        }
        else if(strcmp(ULIK_Message_Item,"TUR")==0) {
            gv.ulikMessageStep=0;
            gv.ulikCalCmdSendNo=7;
        }
        Dprintf("ULIK 설정값 송신 취소\n");
		CloseWindow(NULL, parent);
    }
	return(1);
}
int ULIK_MessageBox_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	PaintDrawControl(OFF);
	Gr->SetColor(RGB565CONVERT(230,  230,  230));
	Gr->FillRect(0, 0, win->width-2, win->height-2);
	TitleBar(handle, 0, 0, LCD_X, 50, 1, win->Text, clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	SLabel(handle, 0, 50+50, 499, 48, ULIK_MessageBox_Text, clBlack, RGB565CONVERT(191, 191, 191),  0);
    SButton(handle, 500 - 20 - 200, 180+50, 200, 54, " ", "OK",  PUSH_BUTTON, 1, ULIK_MessageBox_ClickEvent);
    SButton(handle, 20, 180+50, 200, 54, " ", "Cancel",  PUSH_BUTTON, 2, ULIK_MessageBox_ClickEvent);
	
    PaintDrawControl(ON);
	return(1);
}
int ULIK_MessageBox_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data){
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	Gr->FillRect(0, 0, 0, 0);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int ULIK_MessageBox_Window(char *title, char *text){
	if(gv.messageBoxDispFlag==1) return 0;//메시지박스 중복출력을 막는다.
	
	
	if(GetWindowName("ULIK_MessageBox_Window")) 
	{
		Dprintf("ULIK_MessageBox_Window:: Windows already Create\n");
		return 0;		
	}		
	int sx = (LCD_X - 500 ) / 2;
	int sy = (LCD_Y - 300 ) / 2;
	strcpy(ULIK_MessageBox_Text, text);
    strcpy(ULIK_Message_Item,title);
	makeWindow(
				LCD,
				"ULIK_MessageBox_Window",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				STYLE_NONE,				// Windows Style
				sx, sy, 				// Start Position
				500, 300,				// Size
				SHOW,					// Create after Show
				ULIK_MessageBox_CreateEvent,		// CreateEvent
				ULIK_MessageBox_PaintEvent,		// PaintEvent
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
				NULL,						// Touch Screen
				title,					// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	return(1);
}

#endif

#ifdef __cplusplus
}
#endif
