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
int	 DataLog_ToolBoxMessageTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//char	tbuf[20];
	
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(Gr==NULL) 															// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("DataLog_ToolBoxMessageTimerEvent:: Graphics Error\n");
		return(0);
	}	
	return 0;
}

int	 DataLog_ToolBoxItemPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "DataLog_ToolBoxItemPaintEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(win==NULL) 
	{
		Dprintf("DataLog_ToolBoxItemPaintEvent:: Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("DataLog_ToolBoxItemPaintEvent::GetGraphics Error\n");
		return(0);
	}	
	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	 DataLog_ToolBoxItemClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{	
	Windows 	*win = GetWindow(parent, handle, "DataLog_ToolBoxItemClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "DataLog_ToolBoxItemClickEvent");

	volatile unsigned int		color=0;
	volatile U32					id=0;
	volatile unsigned int		i=0;
	volatile unsigned int		itemCnt=0;
	//char			filename[30];
	//Dprintf("KeyPad NButton parent=%d Handle=%d Click = %s Mode=%d parentname=%s\n",win->Parent, win->Handle, win->NormalName, parent_win->Mode, parent_win->Text);
	if(win==NULL) 
	{
		Dprintf("DataLog_ToolBoxItemClickEvent:: Error\n");
		return(0);
	}
	if(parent_win == NULL)
	{
		Dprintf("DataLog_ToolBoxItemClickEvent:: Windows Error\n");
	}
	//SetWidgetText(parent_win->CallWidget_ID, win->Text);//종료시에 콜한 버튼에 데이터를 출력한다.
	
	/*
	if(win->ID == 48)//취소
	{
		CloseWindow(NULL, parent);
		return 1;
	}
    else if(win->ID == 0)
    {
        if(GetWidgetStatus(gv.handle2[win->ID]))RelayControl(RELAY_1_PORT,ON);
        else RelayControl(RELAY_1_PORT,OFF);
    }
    else if(win->ID == 4)
    {
        if(GetWidgetStatus(gv.handle2[win->ID]))RelayControl(RELAY_2_PORT,ON);
        else RelayControl(RELAY_2_PORT,OFF);
    }
    else if(win->ID == 8)
    {
        if(GetWidgetStatus(gv.handle2[win->ID]))RelayControl(RELAY_3_PORT,ON);
        else RelayControl(RELAY_3_PORT,OFF);
    }
    else if(win->ID == 12)
    {
        if(GetWidgetStatus(gv.handle2[win->ID]))RelayControl(RELAY_4_PORT,ON);
        else RelayControl(RELAY_4_PORT,OFF);
    }
    else if(win->ID == 16)
    {
        if(GetWidgetStatus(gv.handle2[win->ID]))RelayControl(RELAY_5_PORT,ON);
        else RelayControl(RELAY_5_PORT,OFF);
    }
    else if(win->ID == 20)
    {
        if(GetWidgetStatus(gv.handle2[win->ID]))RelayControl(RELAY_6_PORT,ON);
        else RelayControl(RELAY_6_PORT,OFF);
    }
    */
	return(1);
}
//MessageQClear
int	 DataLog_ToolBoxItemCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int		sx = 10;
	int		sy = 10;
    int     add_sx=50;
    int     width=243, height=49;
    int     add_sy=width+10;
	//int		color=clWhite;
	//int		textColor=clBlack;
	//int		i=0;
	//u32		id = 0;
	
	Windows 	*win = GetWindow(parent, handle,"DataLog_ReNameItemCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL)
	{
		Dprintf("DataLog_ReNameItemCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("DataLog_ReNameItemCreateEvent::GetGraphics Error\n");
		return(0);
	}	
	
	PaintDrawControl(OFF);
	Gr->SetColor(GRAY7);
	Gr->FillRect(0, 0, win->client_width+1, win->client_height);	
    //Label(handle, buttonX, buttonY+buttonSpace, buttonW, buttonH, "MODBUS",  clBlack);
	Label(handle, sx, sy, width, height, "RELAY",  clBlack);
	Label(handle, sx+add_sy, sy, width, height, "NONE",  clBlack);
	Label(handle, sx+add_sy+add_sy, sy, width, height, "NONE",  clBlack);
	Label(handle, sx+add_sy+add_sy+add_sy, sy, width, height, "NONE",  clBlack);
	
	sy+=add_sx;
	gv.handle2[0] = NButton(handle, sx, sy, width, height, 0, "RELAY 1", TOGGLE_BUTTON, 0, DataLog_ToolBoxItemClickEvent);
	gv.handle2[1] = NButton(handle, sx+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 1, DataLog_ToolBoxItemClickEvent);
	gv.handle2[2] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 2, DataLog_ToolBoxItemClickEvent);
	gv.handle2[3] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 3, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;
	gv.handle2[4] = NButton(handle, sx, sy, width, height, 0, "RELAY 2", TOGGLE_BUTTON, 4, DataLog_ToolBoxItemClickEvent);
	gv.handle2[5] = NButton(handle, sx+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 5, DataLog_ToolBoxItemClickEvent);
	gv.handle2[6] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 6, DataLog_ToolBoxItemClickEvent);
	gv.handle2[7] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 7, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;
	gv.handle2[8] = NButton(handle, sx, sy, width, height, 0, "RELAY 3", TOGGLE_BUTTON, 8, DataLog_ToolBoxItemClickEvent);
	gv.handle2[9] = NButton(handle, sx+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 9, DataLog_ToolBoxItemClickEvent);
	gv.handle2[10] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 10, DataLog_ToolBoxItemClickEvent);
	gv.handle2[11] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,0, "NONE", PUSH_BUTTON, 11, DataLog_ToolBoxItemClickEvent);
	
	sy+=add_sx;
	gv.handle2[12] = NButton(handle, sx, sy, width, height, 0, "RELAY 4", TOGGLE_BUTTON, 12, DataLog_ToolBoxItemClickEvent);
	gv.handle2[13] = NButton(handle, sx+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 13, DataLog_ToolBoxItemClickEvent);
	gv.handle2[14] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 14, DataLog_ToolBoxItemClickEvent);
	gv.handle2[15] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 15, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;
	gv.handle2[16] = NButton(handle, sx, sy, width, height,  0, "RELAY 5", TOGGLE_BUTTON, 16, DataLog_ToolBoxItemClickEvent);
	gv.handle2[17] = NButton(handle, sx+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 17, DataLog_ToolBoxItemClickEvent);
	gv.handle2[18] = NButton(handle, sx+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 18, DataLog_ToolBoxItemClickEvent);
	gv.handle2[19] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 19, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[20] = NButton(handle, sx, sy, width, height,  0, "RELAY 6", TOGGLE_BUTTON, 20, DataLog_ToolBoxItemClickEvent);
	gv.handle2[21] = NButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 21, DataLog_ToolBoxItemClickEvent);
	gv.handle2[22] = NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 22, DataLog_ToolBoxItemClickEvent);
	gv.handle2[23] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "NONE", PUSH_BUTTON, 23, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[24] = NButton(handle, sx, sy, width, height,  0, "NONE", PUSH_BUTTON, 24, DataLog_ToolBoxItemClickEvent);
	gv.handle2[25] = NButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 25, DataLog_ToolBoxItemClickEvent);
	gv.handle2[26] = NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 26, DataLog_ToolBoxItemClickEvent);
	gv.handle2[27] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 27, DataLog_ToolBoxItemClickEvent);
	
	sy+=add_sx;		   
	gv.handle2[28] = NButton(handle, sx, sy, width, height,  0, "NONE", PUSH_BUTTON, 28, DataLog_ToolBoxItemClickEvent);
	gv.handle2[29] = NButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 29, DataLog_ToolBoxItemClickEvent);
	gv.handle2[30] = NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 30, DataLog_ToolBoxItemClickEvent);
	gv.handle2[31] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 31, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[32] = NButton(handle, sx, sy, width, height,  0, "NONE", PUSH_BUTTON, 32, DataLog_ToolBoxItemClickEvent);
	gv.handle2[33] = NButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 33, DataLog_ToolBoxItemClickEvent);
	gv.handle2[34] = NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 34, DataLog_ToolBoxItemClickEvent);
	gv.handle2[35] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 35, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[36] = 	NButton(handle, sx, sy, width, height,  0, "NONE", PUSH_BUTTON, 36, DataLog_ToolBoxItemClickEvent);
	gv.handle2[37] = 	NButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 37, DataLog_ToolBoxItemClickEvent);
	gv.handle2[38] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 38, DataLog_ToolBoxItemClickEvent);
	gv.handle2[39] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 39, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[40] = 	NButton(handle, sx, sy, width, height,  0, "NONE", PUSH_BUTTON, 40, DataLog_ToolBoxItemClickEvent);
	gv.handle2[41] = 	NButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 41, DataLog_ToolBoxItemClickEvent);
	gv.handle2[42] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 42, DataLog_ToolBoxItemClickEvent);
	gv.handle2[43] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 43, DataLog_ToolBoxItemClickEvent);

	sy+=add_sx;		   
	gv.handle2[44] = 	NButton(handle, sx, sy, width, height,  0, "NONE", PUSH_BUTTON, 44, DataLog_ToolBoxItemClickEvent);
	gv.handle2[45] = 	NButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 45, DataLog_ToolBoxItemClickEvent);
	gv.handle2[46] = 	NButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 46, DataLog_ToolBoxItemClickEvent);
	gv.handle2[47] = 	NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 47, DataLog_ToolBoxItemClickEvent);
	
	sy+=add_sx;		   
    gv.handle2[48] = NButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, "HOME", PUSH_BUTTON, 48, DataLog_ToolBoxItemClickEvent);
	SetWidgetColor(gv.handle2[48],YELLOW,YELLOW);
	PaintDrawControl(ON);
	
	return(1);
}
//DataLog_ToolBoxItemKeypad(win->Parent, win->Handle,  NULL, (1024-780)/2, (LCD_Y-555)/2, 0, "", "이름변경");
void DataLog_ToolBoxItemKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title)
{
	WidgetStruct wid_struct;	

	wid_struct.FontNo = font_no;
	wid_struct.CallWidget_ID = id;

	makeWindow(
				LCD,
				"DataLog_ToolBoxItemKeypad",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				1024, 768,			// Size
				SHOW,					// Create after Show
				DataLog_ToolBoxItemCreateEvent,		// CreateEvent
				DataLog_ToolBoxItemPaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				DataLog_ToolBoxMessageTimerEvent,					// TimerEvent
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
