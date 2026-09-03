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
int	 DataLog_DispItemTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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

int	 DataLog_DispItemPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "DataLog_DispItemPaintEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(win==NULL) 
	{
		Dprintf("DataLog_DispItemPaintEvent:: Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("DataLog_DispItemPaintEvent::GetGraphics Error\n");
		return(0);
	}	
	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	 DataLog_DispItemClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{	
	Windows 	*win = GetWindow(parent, handle, "DataLog_DispItemClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "DataLog_DispItemClickEvent");

	volatile unsigned int		color=0;
	volatile U32					id=0;
	volatile unsigned int		i=0;
	volatile unsigned int		itemCnt=0;
	char			filename[30];

	//Dprintf("KeyPad NButton parent=%d Handle=%d Click = %s Mode=%d parentname=%s\n",win->Parent, win->Handle, win->NormalName, parent_win->Mode, parent_win->Text);
	if(win==NULL) 
	{
		Dprintf("DataLog_DispItemClickEvent:: Error\n");
		return(0);
	}
	if(parent_win == NULL)
	{
		Dprintf("DataLog_DispItemClickEvent:: Windows Error\n");
	}
	//SetWidgetText(parent_win->CallWidget_ID, win->Text);//종료시에 콜한 버튼에 데이터를 출력한다.
	if(win->ID == 48)//취소
	{
		CloseWindow(NULL, parent);
		return 1;
	}
	if(win->ID == 49)//저장
	{
		GetWidgetText(gv.handle2[0],filename); strcpy(flash.Uart[USART_1]->SetupName[0],filename);
		GetWidgetText(gv.handle2[1],filename); strcpy(flash.Uart[USART_2]->SetupName[0],filename);
		GetWidgetText(gv.handle2[2],filename); strcpy(flash.Uart[USART_3]->SetupName[0],filename);
		GetWidgetText(gv.handle2[3],filename); strcpy(flash.Uart[USART_4]->SetupName[0],filename);

		GetWidgetText(gv.handle2[4],filename); strcpy(flash.Uart[USART_1]->SetupName[1],filename);
		GetWidgetText(gv.handle2[5],filename); strcpy(flash.Uart[USART_2]->SetupName[1],filename);
		GetWidgetText(gv.handle2[6],filename); strcpy(flash.Uart[USART_3]->SetupName[1],filename);
		GetWidgetText(gv.handle2[7],filename); strcpy(flash.Uart[USART_4]->SetupName[1],filename);

		GetWidgetText(gv.handle2[8],filename); strcpy(flash.Uart[USART_1]->SetupName[2],filename);
		GetWidgetText(gv.handle2[9],filename); strcpy(flash.Uart[USART_2]->SetupName[2],filename);
		GetWidgetText(gv.handle2[10],filename); strcpy(flash.Uart[USART_3]->SetupName[2],filename);
		GetWidgetText(gv.handle2[11],filename); strcpy(flash.Uart[USART_4]->SetupName[2],filename);

		GetWidgetText(gv.handle2[12],filename); strcpy(flash.Uart[USART_1]->SetupName[3],filename);
		GetWidgetText(gv.handle2[13],filename); strcpy(flash.Uart[USART_2]->SetupName[3],filename);
		GetWidgetText(gv.handle2[14],filename); strcpy(flash.Uart[USART_3]->SetupName[3],filename);
		GetWidgetText(gv.handle2[15],filename); strcpy(flash.Uart[USART_4]->SetupName[3],filename);

		GetWidgetText(gv.handle2[16],filename); strcpy(flash.Uart[USART_1]->SetupName[4],filename);
		GetWidgetText(gv.handle2[17],filename); strcpy(flash.Uart[USART_2]->SetupName[4],filename);
		GetWidgetText(gv.handle2[18],filename); strcpy(flash.Uart[USART_3]->SetupName[4],filename);
		GetWidgetText(gv.handle2[19],filename); strcpy(flash.Uart[USART_4]->SetupName[4],filename);

		GetWidgetText(gv.handle2[20],filename); strcpy(flash.Uart[USART_1]->SetupName[5],filename);
		GetWidgetText(gv.handle2[21],filename); strcpy(flash.Uart[USART_2]->SetupName[5],filename);
		GetWidgetText(gv.handle2[22],filename); strcpy(flash.Uart[USART_3]->SetupName[5],filename);
		GetWidgetText(gv.handle2[23],filename); strcpy(flash.Uart[USART_4]->SetupName[5],filename);

		GetWidgetText(gv.handle2[24],filename); strcpy(flash.Uart[USART_1]->SetupName[6],filename);
		GetWidgetText(gv.handle2[25],filename); strcpy(flash.Uart[USART_2]->SetupName[6],filename);
		GetWidgetText(gv.handle2[26],filename); strcpy(flash.Uart[USART_3]->SetupName[6],filename);
		GetWidgetText(gv.handle2[27],filename); strcpy(flash.Uart[USART_4]->SetupName[6],filename);

		GetWidgetText(gv.handle2[28],filename); strcpy(flash.Uart[USART_1]->SetupName[7],filename);
		GetWidgetText(gv.handle2[29],filename); strcpy(flash.Uart[USART_2]->SetupName[7],filename);
		GetWidgetText(gv.handle2[30],filename); strcpy(flash.Uart[USART_3]->SetupName[7],filename);
		GetWidgetText(gv.handle2[31],filename); strcpy(flash.Uart[USART_4]->SetupName[7],filename);

		GetWidgetText(gv.handle2[32],filename); strcpy(flash.Uart[USART_1]->SetupName[8],filename);
		GetWidgetText(gv.handle2[33],filename); strcpy(flash.Uart[USART_2]->SetupName[8],filename);
		GetWidgetText(gv.handle2[34],filename); strcpy(flash.Uart[USART_3]->SetupName[8],filename);
		GetWidgetText(gv.handle2[35],filename); strcpy(flash.Uart[USART_4]->SetupName[8],filename);

		GetWidgetText(gv.handle2[36],filename); strcpy(flash.Uart[USART_1]->SetupName[9],filename);
		GetWidgetText(gv.handle2[37],filename); strcpy(flash.Uart[USART_2]->SetupName[9],filename);
		GetWidgetText(gv.handle2[38],filename); strcpy(flash.Uart[USART_3]->SetupName[9],filename);
		GetWidgetText(gv.handle2[39],filename); strcpy(flash.Uart[USART_4]->SetupName[9],filename);

		GetWidgetText(gv.handle2[40],filename); strcpy(flash.Uart[USART_1]->SetupName[10],filename);
		GetWidgetText(gv.handle2[41],filename); strcpy(flash.Uart[USART_2]->SetupName[10],filename);
		GetWidgetText(gv.handle2[42],filename); strcpy(flash.Uart[USART_3]->SetupName[10],filename);
		GetWidgetText(gv.handle2[43],filename); strcpy(flash.Uart[USART_4]->SetupName[10],filename);

		GetWidgetText(gv.handle2[44],filename); strcpy(flash.Uart[USART_1]->SetupName[11],filename);
		GetWidgetText(gv.handle2[45],filename); strcpy(flash.Uart[USART_2]->SetupName[11],filename);
		GetWidgetText(gv.handle2[46],filename); strcpy(flash.Uart[USART_3]->SetupName[11],filename);
		GetWidgetText(gv.handle2[47],filename); strcpy(flash.Uart[USART_4]->SetupName[11],filename);
	
		//StructToNand("protocol.data", &flash, sizeof(__SmartLogdata));
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        
		CloseWindow(NULL, parent);
		return 1;
	}
	else
	{
		KeyBoard(gv.handle2[win->ID], (800-644)/2, (600-394)/2, NULL,NULL); 
	}
	
	return(1);
}
//MessageQClear
int	 DataLog_DispItemCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int		sx = 10;
	int		sy = 10;
    int     add_sx=40;
    int     width=243, height=39;
    int     add_sy=width+10;
	//int		color=clWhite;
	//int		textColor=clBlack;
	//int		i=0;
	//u32		id = 0;
	
	Windows 	*win = GetWindow(parent, handle,"DataLog_DispItemCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL)
	{
		Dprintf("DataLog_DispItemCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("DataLog_DispItemCreateEvent::GetGraphics Error\n");
		return(0);
	}	
	
	PaintDrawControl(OFF);
	Gr->SetColor(GRAY7);
	Gr->FillRect(0, 0, win->client_width+1, win->client_height);	

//Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "MODBUS",  clBlack);
//	Label(handle, sx, sy, width, height, "UART 1",  clBlack);
//	Label(handle, sx+add_sy, sy, width, height, "UART 2",  clBlack);
//	Label(handle, sx+add_sy+add_sy, sy, width, height, "UART 3",  clBlack);
//	Label(handle, sx+add_sy+add_sy+add_sy, sy, width, height, "UART 4",  clBlack);
	
	//sy+=add_sx;
	gv.handle2[0] = NButton(handle, sx, sy, width, height, 0, "Temperature 'C", PUSH_BUTTON, 0, DataLog_DispItemClickEvent);
	gv.handle2[1] = NButton(handle, sx+add_sy, sy, width, height, 0, "Temperature 'F", PUSH_BUTTON, 1, DataLog_DispItemClickEvent);
	gv.handle2[2] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "Temperature 'K", PUSH_BUTTON, 2, DataLog_DispItemClickEvent);
	gv.handle2[3] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "Conductivity mS/cm", PUSH_BUTTON, 3, DataLog_DispItemClickEvent);

	sy+=add_sx;
	gv.handle2[4] = NButton(handle, sx, sy, width, height, 0, "Conductivity uS/cm", PUSH_BUTTON, 4, DataLog_DispItemClickEvent);
	gv.handle2[5] = NButton(handle, sx+add_sy, sy, width, height, 0, "SpConductivity mS/cm", PUSH_BUTTON, 5, DataLog_DispItemClickEvent);
	gv.handle2[6] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "SpConductivity uS/cm", PUSH_BUTTON, 6, DataLog_DispItemClickEvent);
	gv.handle2[7] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "TDS g/L", PUSH_BUTTON, 7, DataLog_DispItemClickEvent);

	sy+=add_sx;
	gv.handle2[8] = NButton(handle, sx, sy, width, height, 0, "Salinity ppt", PUSH_BUTTON, 8, DataLog_DispItemClickEvent);
	gv.handle2[9] = NButton(handle, sx+add_sy, sy, width, height, 0, "pH mV", PUSH_BUTTON, 9, DataLog_DispItemClickEvent);
	gv.handle2[10] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "pH", PUSH_BUTTON, 10, DataLog_DispItemClickEvent);
	gv.handle2[11] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,0, "ORP mV", PUSH_BUTTON, 11, DataLog_DispItemClickEvent);
	
	sy+=add_sx;
	gv.handle2[12] = NButton(handle, sx, sy, width, height, 0, "Depth m", PUSH_BUTTON, 12, DataLog_DispItemClickEvent);
	gv.handle2[13] = NButton(handle, sx+add_sy, sy, width, height, 0, "Battery V", PUSH_BUTTON, 13, DataLog_DispItemClickEvent);
	gv.handle2[14] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "Turbidity NTU", PUSH_BUTTON, 14, DataLog_DispItemClickEvent);
	gv.handle2[15] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "NH3(Ammonia) mg/L", PUSH_BUTTON, 15, DataLog_DispItemClickEvent);

	sy+=add_sx;
	gv.handle2[16] = NButton(handle, sx, sy, width, height,  0, "NH4(Ammonium) mg/L", PUSH_BUTTON, 16, DataLog_DispItemClickEvent);
	gv.handle2[17] = NButton(handle, sx+add_sy, sy, width, height, 0, "TDS kg/L", PUSH_BUTTON, 17, DataLog_DispItemClickEvent);
	gv.handle2[18] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "NO3(Nitrate) mV", PUSH_BUTTON, 18, DataLog_DispItemClickEvent);
	gv.handle2[19] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "NO3(Nitrate) mg/L", PUSH_BUTTON, 19, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[20] = NButton(handle, sx, sy, width, height,  0, "TDS mg/L", PUSH_BUTTON, 20, DataLog_DispItemClickEvent);
	gv.handle2[21] = NButton(handle, sx+add_sy, sy, width, height,  0, "Chloride mg/L", PUSH_BUTTON, 21, DataLog_DispItemClickEvent);
	gv.handle2[22] = NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "Chloride mV", PUSH_BUTTON, 22, DataLog_DispItemClickEvent);
	gv.handle2[23] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "TSS mg/L", PUSH_BUTTON, 23, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[24] = NButton(handle, sx, sy, width, height,  0, "TSS g/L", PUSH_BUTTON, 24, DataLog_DispItemClickEvent);
	gv.handle2[25] = NButton(handle, sx+add_sy, sy, width, height,  0, "Chlorophyll ug/L", PUSH_BUTTON, 25, DataLog_DispItemClickEvent);
	gv.handle2[26] = NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "Chlorophyll RFU", PUSH_BUTTON, 26, DataLog_DispItemClickEvent);
	gv.handle2[27] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[6], PUSH_BUTTON, 27, DataLog_DispItemClickEvent);
	
	sy+=add_sx;		   
	gv.handle2[28] = NButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[7], PUSH_BUTTON, 28, DataLog_DispItemClickEvent);
	gv.handle2[29] = NButton(handle, sx+add_sy, sy, width, height,  0, flash.Uart[USART_2]->SetupName[7], PUSH_BUTTON, 29, DataLog_DispItemClickEvent);
	gv.handle2[30] = NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[7], PUSH_BUTTON, 30, DataLog_DispItemClickEvent);
	gv.handle2[31] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[7], PUSH_BUTTON, 31, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[32] = NButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[8], PUSH_BUTTON, 32, DataLog_DispItemClickEvent);
	gv.handle2[33] = NButton(handle, sx+add_sy, sy, width, height,  0, flash.Uart[USART_2]->SetupName[8], PUSH_BUTTON, 33, DataLog_DispItemClickEvent);
	gv.handle2[34] = NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[8], PUSH_BUTTON, 34, DataLog_DispItemClickEvent);
	gv.handle2[35] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[8], PUSH_BUTTON, 35, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[36] = 	NButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[9], PUSH_BUTTON, 36, DataLog_DispItemClickEvent);
	gv.handle2[37] = 	NButton(handle, sx+add_sy, sy, width, height,  0, flash.Uart[USART_2]->SetupName[9], PUSH_BUTTON, 37, DataLog_DispItemClickEvent);
	gv.handle2[38] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[9], PUSH_BUTTON, 38, DataLog_DispItemClickEvent);
	gv.handle2[39] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[9], PUSH_BUTTON, 39, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[40] = 	NButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[10], PUSH_BUTTON, 40, DataLog_DispItemClickEvent);
	gv.handle2[41] = 	NButton(handle, sx+add_sy, sy, width, height,  0, flash.Uart[USART_2]->SetupName[10], PUSH_BUTTON, 41, DataLog_DispItemClickEvent);
	gv.handle2[42] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[10], PUSH_BUTTON, 42, DataLog_DispItemClickEvent);
	gv.handle2[43] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[10], PUSH_BUTTON, 43, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[44] = 	NButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[11], PUSH_BUTTON, 44, DataLog_DispItemClickEvent);
	gv.handle2[45] = 	NButton(handle, sx+add_sy, sy, width, height,  0, flash.Uart[USART_2]->SetupName[11], PUSH_BUTTON, 45, DataLog_DispItemClickEvent);
	gv.handle2[46] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[11], PUSH_BUTTON, 46, DataLog_DispItemClickEvent);
	gv.handle2[47] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[11], PUSH_BUTTON, 47, DataLog_DispItemClickEvent);
	
    
	sy+=add_sx;		   
	gv.handle2[48] = 	NButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[11], PUSH_BUTTON, 44, DataLog_DispItemClickEvent);
	gv.handle2[49] = 	NButton(handle, sx+add_sy, sy, width, height,  0, flash.Uart[USART_2]->SetupName[11], PUSH_BUTTON, 45, DataLog_DispItemClickEvent);
	gv.handle2[50] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[11], PUSH_BUTTON, 46, DataLog_DispItemClickEvent);
	gv.handle2[51] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[11], PUSH_BUTTON, 47, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[52] = 	NButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[11], PUSH_BUTTON, 44, DataLog_DispItemClickEvent);
	gv.handle2[53] = 	NButton(handle, sx+add_sy, sy, width, height,  0, flash.Uart[USART_2]->SetupName[11], PUSH_BUTTON, 45, DataLog_DispItemClickEvent);
	gv.handle2[54] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[11], PUSH_BUTTON, 46, DataLog_DispItemClickEvent);
	gv.handle2[55] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[11], PUSH_BUTTON, 47, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[56] = 	NButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[11], PUSH_BUTTON, 44, DataLog_DispItemClickEvent);
	gv.handle2[57] = 	NButton(handle, sx+add_sy, sy, width, height,  0, flash.Uart[USART_2]->SetupName[11], PUSH_BUTTON, 45, DataLog_DispItemClickEvent);
	gv.handle2[58] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[11], PUSH_BUTTON, 46, DataLog_DispItemClickEvent);
	gv.handle2[59] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[11], PUSH_BUTTON, 47, DataLog_DispItemClickEvent);

	sy+=add_sx;		   
//	gv.handle2[49] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "SAVE", PUSH_BUTTON, 49, DataLog_DispItemClickEvent);
//	SetWidgetColor(gv.handle2[49],YELLOW,YELLOW);
//    gv.handle2[48] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "CANCEL", PUSH_BUTTON, 48, DataLog_DispItemClickEvent);
//	SetWidgetColor(gv.handle2[48],YELLOW,YELLOW);
	PaintDrawControl(ON);
	
	return(1);
}

void DataLog_DispItemKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title) {
	WidgetStruct wid_struct;	
	wid_struct.FontNo = font_no;
	wid_struct.CallWidget_ID = id;
	makeWindow(
				LCD,
				"DataLog_DispItemKeypad",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				1024, 768,			    // Size
				SHOW,					// Create after Show
				DataLog_DispItemCreateEvent,		// CreateEvent
				DataLog_DispItemPaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				DataLog_DispItemTimerEvent,					// TimerEvent
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
