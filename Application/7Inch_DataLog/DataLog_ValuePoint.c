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
int	 DataLog_ValuePointTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//char	tbuf[20];
	
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(Gr==NULL) 															// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("DataLog_ReNameMessageTimerEvent:: Graphics Error\n");
		return(0);
	}	
	return 0;
}

int	 DataLog_ValuePointPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "DataLog_ValuePointPaintEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(win==NULL) 
	{
		Dprintf("DataLog_ValuePointPaintEvent:: Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("DataLog_ValuePointPaintEvent::GetGraphics Error\n");
		return(0);
	}	
	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
int	 DataLog_ValuePointClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{	
	Windows 	*win = GetWindow(parent, handle, "DataLog_ValuePointClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "DataLog_ValuePointClickEvent");
    Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	
    volatile unsigned int		color=0;
	volatile U32					id=0;
	volatile unsigned int		i=0;
	volatile unsigned int		itemCnt=0;
    char buf[30];
    char addr = 0;

	//Dprintf("KeyPad SButton parent=%d Handle=%d Click = %s Mode=%d parentname=%s\n",win->Parent, win->Handle, win->NormalName, parent_win->Mode, parent_win->Text);
	if(win==NULL) 
	{
		Dprintf("DataLog_ValuePointClickEvent:: Error\n");
		return(0);
	}
	if(parent_win == NULL)
	{
		Dprintf("DataLog_ValuePointClickEvent:: Windows Error\n");
	}
	//SetWidgetText(parent_win->CallWidget_ID, win->Text);//종료시에 콜한 버튼에 데이터를 출력한다.
	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

	if(win->ID == 48) {//취소
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
		CloseWindow(NULL, parent);
		return 1;
	} else if(win->ID == 49) { //저장
        
		CloseWindow(NULL, parent);
		return 1;
	} 
    else if(win->ID == 1) {
        addr=0;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 5) {
        addr=1;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 9) {
        addr=2;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 13) {
        addr=3;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 17) {
        addr=4;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 21) {
        addr=5;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 25) {
        addr=6;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 29) {
        addr=7;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 33) {
        addr=8;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 37) {
        addr=9;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 41) {
        addr=10;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    else if(win->ID == 45) {
        addr=11;
        flash.floatPoint[addr]++;
        if(flash.floatPoint[addr]>=5) {
            flash.floatPoint[addr]=0;
        }
        sprintf(buf,"%d",flash.floatPoint[addr]);
        SetWidgetText(gv.handle2[win->ID], buf);
    }        
    
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	
	return(1);
}
//MessageQClear
int	 DataLog_ValuePointCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int		sx = 10;
	int		sy = 10;
    int     add_sx=50;
    int     width=243, height=49;
    int     add_sy=width+10;
    char    buf[10];
	//int		color=clWhite;
	//int		textColor=clBlack;
	//int		i=0;
	//u32		id = 0;
	
	Windows 	*win = GetWindow(parent, handle,"DataLog_ValuePointCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL)
	{
		Dprintf("DataLog_ValuePointCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("DataLog_ValuePointCreateEvent::GetGraphics Error\n");
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
    //Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "MODBUS",  clBlack);
	Label(handle, sx, sy, width, height, "항목이름",  clBlack);
	Label(handle, sx+add_sy, sy, width, height, "출력범위",  clBlack);
	Label(handle, sx+add_sy+add_sy, sy, width, height, "항목이름",  clBlack);
	Label(handle, sx+add_sy+add_sy+add_sy, sy, width, height, "출력범위",  clBlack);
	
	sy+=add_sx;
	gv.handle2[0] = SButton(handle, sx, sy, width, height, 0, flash.Uart[USART_1]->SetupName[0], PUSH_BUTTON, 0, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[0]);
	gv.handle2[1] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 1, DataLog_ValuePointClickEvent);
	gv.handle2[2] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[0], PUSH_BUTTON, 2, DataLog_ValuePointClickEvent);
	gv.handle2[3] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[0], PUSH_BUTTON, 3, DataLog_ValuePointClickEvent);

	sy+=add_sx;
	gv.handle2[4] = SButton(handle, sx, sy, width, height, 0, flash.Uart[USART_1]->SetupName[1], PUSH_BUTTON, 4, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[1]);
	gv.handle2[5] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 5, DataLog_ValuePointClickEvent);
	gv.handle2[6] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[1], PUSH_BUTTON, 6, DataLog_ValuePointClickEvent);
	gv.handle2[7] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[1], PUSH_BUTTON, 7, DataLog_ValuePointClickEvent);

	sy+=add_sx;
	gv.handle2[8] = SButton(handle, sx, sy, width, height, 0, flash.Uart[USART_1]->SetupName[2], PUSH_BUTTON, 8, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[2]);
	gv.handle2[9] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 9, DataLog_ValuePointClickEvent);
	gv.handle2[10] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[2], PUSH_BUTTON, 10, DataLog_ValuePointClickEvent);
	gv.handle2[11] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,0, flash.Uart[USART_4]->SetupName[2], PUSH_BUTTON, 11, DataLog_ValuePointClickEvent);
	
	sy+=add_sx;
	gv.handle2[12] = SButton(handle, sx, sy, width, height, 0, flash.Uart[USART_1]->SetupName[3], PUSH_BUTTON, 12, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[3]);
	gv.handle2[13] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 13, DataLog_ValuePointClickEvent);
	gv.handle2[14] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[3], PUSH_BUTTON, 14, DataLog_ValuePointClickEvent);
	gv.handle2[15] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[3], PUSH_BUTTON, 15, DataLog_ValuePointClickEvent);

	sy+=add_sx;
	gv.handle2[16] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[4], PUSH_BUTTON, 16, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[4]);
	gv.handle2[17] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 17, DataLog_ValuePointClickEvent);
	gv.handle2[18] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[4], PUSH_BUTTON, 18, DataLog_ValuePointClickEvent);
	gv.handle2[19] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[4], PUSH_BUTTON, 19, DataLog_ValuePointClickEvent);

	sy+=add_sx;		   
	gv.handle2[20] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[5], PUSH_BUTTON, 20, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[5]);
	gv.handle2[21] = SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 21, DataLog_ValuePointClickEvent);
	gv.handle2[22] = SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[5], PUSH_BUTTON, 22, DataLog_ValuePointClickEvent);
	gv.handle2[23] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[5], PUSH_BUTTON, 23, DataLog_ValuePointClickEvent);

	sy+=add_sx;		   
	gv.handle2[24] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[6], PUSH_BUTTON, 24, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[6]);
	gv.handle2[25] = SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 25, DataLog_ValuePointClickEvent);
	gv.handle2[26] = SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[6], PUSH_BUTTON, 26, DataLog_ValuePointClickEvent);
	gv.handle2[27] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[6], PUSH_BUTTON, 27, DataLog_ValuePointClickEvent);
	
	sy+=add_sx;		   
	gv.handle2[28] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[7], PUSH_BUTTON, 28, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[7]);
	gv.handle2[29] = SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 29, DataLog_ValuePointClickEvent);
	gv.handle2[30] = SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[7], PUSH_BUTTON, 30, DataLog_ValuePointClickEvent);
	gv.handle2[31] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[7], PUSH_BUTTON, 31, DataLog_ValuePointClickEvent);

	sy+=add_sx;		   
	gv.handle2[32] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[8], PUSH_BUTTON, 32, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[8]);
	gv.handle2[33] = SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 33, DataLog_ValuePointClickEvent);
	gv.handle2[34] = SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[8], PUSH_BUTTON, 34, DataLog_ValuePointClickEvent);
	gv.handle2[35] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[8], PUSH_BUTTON, 35, DataLog_ValuePointClickEvent);

	sy+=add_sx;		   
	gv.handle2[36] = 	SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[9], PUSH_BUTTON, 36, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[9]);
	gv.handle2[37] = 	SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 37, DataLog_ValuePointClickEvent);
	gv.handle2[38] = 	SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[9], PUSH_BUTTON, 38, DataLog_ValuePointClickEvent);
	gv.handle2[39] = 	SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[9], PUSH_BUTTON, 39, DataLog_ValuePointClickEvent);

	sy+=add_sx;		   
	gv.handle2[40] = 	SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[10], PUSH_BUTTON, 40, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[10]);
	gv.handle2[41] = 	SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 41, DataLog_ValuePointClickEvent);
	gv.handle2[42] = 	SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[10], PUSH_BUTTON, 42, DataLog_ValuePointClickEvent);
	gv.handle2[43] = 	SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[10], PUSH_BUTTON, 43, DataLog_ValuePointClickEvent);

	sy+=add_sx;		   
	gv.handle2[44] = 	SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[11], PUSH_BUTTON, 44, DataLog_ValuePointClickEvent);
    sprintf(buf,"%d",flash.floatPoint[11]);
	gv.handle2[45] = 	SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 45, DataLog_ValuePointClickEvent);
	gv.handle2[46] = 	SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[11], PUSH_BUTTON, 46, DataLog_ValuePointClickEvent);
	gv.handle2[47] = 	SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[11], PUSH_BUTTON, 47, DataLog_ValuePointClickEvent);
	
	sy+=add_sx;		   
	//gv.handle2[49] = 	SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "SAVE", PUSH_BUTTON, 49, DataLog_ValuePointClickEvent);
	//SetWidgetColor(gv.handle2[49],YELLOW,YELLOW);
    gv.handle2[48] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, 60, "48G_Home_Icon2.bmp", "HOME", PUSH_BUTTON, 48, DataLog_ValuePointClickEvent);
	SetWidgetColor(gv.handle2[48],YELLOW,YELLOW);
	PaintDrawControl(ON);
	
	return(1);
}

void DataLog_ValuePointKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title)
{
	WidgetStruct wid_struct;	

	wid_struct.FontNo = font_no;
	wid_struct.CallWidget_ID = id;

	makeWindow(
				LCD,
				"DataLog_ValuePointKeypad",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				1024, 768,			// Size
				SHOW,					// Create after Show
				DataLog_ValuePointCreateEvent,		// CreateEvent
				DataLog_ValuePointPaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				DataLog_ValuePointTimerEvent,					// TimerEvent
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
