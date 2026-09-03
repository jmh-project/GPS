/******************************************************************************
*    	File name	: KeyPad.c 
*		Description : TEECO Project User Program File
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: Naver TEECO Cafe
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/


/******************************************************************************
							Function Prototype
******************************************************************************/

#if defined(TYPE_APPROVAL_ENABLE)
char gCodeTitleName[32];
float gCodeFdata[6];
int	 TypeApprovalFactorTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(Gr==NULL) 															// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("DataLog_ReNameMessageTimerEvent:: Graphics Error\n");
		return(0);
	}	
	return 0;
}

int	 TypeApprovalFactorPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle, "TypeApprovalFactorPaintEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(win==NULL) 
	{
		Dprintf("TypeApprovalFactorPaintEvent:: Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("TypeApprovalFactorPaintEvent::GetGraphics Error\n");
		return(0);
	}	
	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
int	 TypeApprovalFactorClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {	
	Windows 	*win = GetWindow(parent, handle, "TypeApprovalFactorClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "TypeApprovalFactorClickEvent");
    Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	
    volatile unsigned int		color=0;
	volatile U32					id=0;
	volatile unsigned int		i=0;
	volatile unsigned int		itemCnt=0;
    char buf[30];

	if(win==NULL) {
		Dprintf("TypeApprovalFactorClickEvent:: Error\n");
		return(0);
	}
	if(parent_win == NULL){
		Dprintf("TypeApprovalFactorClickEvent:: Windows Error\n");
	}
	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

    if(win->ID == 3) {//보정기울기
		KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "보정기울기");
	}        
    else if(win->ID == 4) {//보정절편
		KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "보정절편");
    }        
    else if(win->ID == 5) {//최대측정범위
		KeyPad(win->Parent, win->Handle,  gv.handle2[win->ID], (1024-345)/2, (LCD_Y-415)/2, 1, "", "최대측정범위");
    }        
	else if(win->ID == 6) {//HOME BUTTON
		if(!strcmp(gCodeTitleName,"pH 측정상수")) {
			GetWidgetText(gv.handle2[3],buf);
			flash.codeFACT[0]=atof(buf);
			GetWidgetText(gv.handle2[4],buf);
			flash.codeOFST[0]=atof(buf);
			GetWidgetText(gv.handle2[5],buf);
			flash.codeMAXR[0]=atof(buf);
		} else if(!strcmp(gCodeTitleName,"DO 측정상수")) {
			GetWidgetText(gv.handle2[3],buf);
			flash.codeFACT[1]=atof(buf);
			GetWidgetText(gv.handle2[4],buf);
			flash.codeOFST[1]=atof(buf);
			GetWidgetText(gv.handle2[5],buf);
			flash.codeMAXR[1]=atof(buf);
		}
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
		CloseWindow(NULL, parent);
		return 1;
	} 
    
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	
	return(1);
}
int	 TypeApprovalFactorCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	int		sx = 10;
	int		sy = 10;
    int     add_sx=50;
    int     width=243, height=49;
    int     add_sy=width+10;
    char    buf[64];
	
	Windows 	*win = GetWindow(parent, handle,"TypeApprovalFactorCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL)
	{
		Dprintf("TypeApprovalFactorCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("TypeApprovalFactorCreateEvent::GetGraphics Error\n");
		return(0);
	}	
	
	PaintDrawControl(OFF);
	if(flash.systemColorNo!=6) {		
		Gr->SetColor(clBlack);
	} 
	else {
		Gr->SetColor(RGB565CONVERT(230,  230,  230));
	}
	
	Gr->FillRect(0, 0, win->client_width+1, win->client_height);	
    Label(handle, sx, sy, width, height, "명칭",  clBlack);
	Label(handle, sx+add_sy, sy, width, height, "설정값",  clBlack);

	if(!strcmp(gCodeTitleName,"pH 측정상수")) {
		gCodeFdata[0]=flash.Uart[USART_1]->SensorPV[0];//온도
		gCodeFdata[1]=flash.codeZERO[0];
		gCodeFdata[2]=flash.codeSPAN[0];
		gCodeFdata[3]=flash.codeFACT[0];
		gCodeFdata[4]=flash.codeOFST[0];
		gCodeFdata[5]=flash.codeMAXR[0];
	} else if(!strcmp(gCodeTitleName,"DO 측정상수")) {
		gCodeFdata[0]=flash.Uart[USART_1]->SensorPV[0];//온도
		gCodeFdata[1]=flash.codeZERO[1];
		gCodeFdata[2]=flash.codeSPAN[1];
		gCodeFdata[3]=flash.codeFACT[1];
		gCodeFdata[4]=flash.codeOFST[1];
		gCodeFdata[5]=flash.codeMAXR[1];
	}
	
	sy+=add_sx;
	Label(handle, sx, sy, width, height, "측정조온도",  clBlack); 
	sprintf(buf,"%.2f",gCodeFdata[0]);
	gv.handle2[0] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 0, NULL);

	sy+=add_sx;
	Label(handle, sx, sy, width, height, "영점교정액",  clBlack); 
	sprintf(buf,"%.2f",gCodeFdata[1]);
	gv.handle2[1] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 1, NULL);

	sy+=add_sx;
	Label(handle, sx, sy, width, height, "스펜교정액",  clBlack); 
	sprintf(buf,"%.2f",gCodeFdata[2]);
	gv.handle2[2] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 2, NULL);
	
	sy+=add_sx;
	Label(handle, sx, sy, width, height, "보정기울기",  clBlack); 
	sprintf(buf,"%.2f",gCodeFdata[3]);
	gv.handle2[3] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 3, TypeApprovalFactorClickEvent);

	sy+=add_sx;
	Label(handle, sx, sy, width, height, "보정절편",  clBlack); 
	sprintf(buf,"%.2f",gCodeFdata[4]);
	gv.handle2[4] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 4, TypeApprovalFactorClickEvent);

	sy+=add_sx;
	Label(handle, sx, sy, width, height, "최대측정범위",  clBlack); 
	sprintf(buf,"%.2f",gCodeFdata[5]);
	gv.handle2[5] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 5, TypeApprovalFactorClickEvent);
	
	sy+=add_sx;		   
    gv.handle2[6] = SButton(handle, sx+add_sy, sy+5, width, height+5, "48G_Home_Icon2.bmp", "EXIT", PUSH_BUTTON, 6, TypeApprovalFactorClickEvent);
	PaintDrawControl(ON);
	
	return(1);
}

void TypeApprovalFactorKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title)
{
	WidgetStruct wid_struct;	

	wid_struct.FontNo = font_no;
	wid_struct.CallWidget_ID = id;
	
	strcpy(gCodeTitleName,title);
	makeWindow(
				LCD,
				"TypeApprovalFactorKeypad",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				(1024-520)/2, (768-475)/2, 				// Start Position
				520, 475,			// Size
				SHOW,					// Create after Show
				TypeApprovalFactorCreateEvent,		// CreateEvent
				TypeApprovalFactorPaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				TypeApprovalFactorTimerEvent,					// TimerEvent
				NULL,  					// ControlEvent
				NULL,  					// ChangeEvent
				NULL,
				NULL,
				NULL,
				NULL,
				GRAY7,					// PARAMETER
				value,					// Normal Touch Screen
				0,						// Touch Screen
				title,					// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
				
	//ShowWindow(NULL, mainHandle);
}
#endif

