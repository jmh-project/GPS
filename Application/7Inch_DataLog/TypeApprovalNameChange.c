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
int	 TypeApprovalNameTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	//char	tbuf[20];
	
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(Gr==NULL) 															// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("DataLog_ReNameMessageTimerEvent:: Graphics Error\n");
		return(0);
	}	
	return 0;
}

int	 TypeApprovalNamePaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle, "TypeApprovalNamePaintEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.

	if(win==NULL) 
	{
		Dprintf("TypeApprovalNamePaintEvent:: Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("TypeApprovalNamePaintEvent::GetGraphics Error\n");
		return(0);
	}	
	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}
int	 TypeApprovalNameClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {	
	Windows 	*win = GetWindow(parent, handle, "TypeApprovalNameClickEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "TypeApprovalNameClickEvent");
    Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	
    volatile unsigned int		color=0;
	volatile U32					id=0;
	volatile unsigned int		i=0;
	volatile unsigned int		itemCnt=0;
    char buf[30];

	//Dprintf("KeyPad SButton parent=%d Handle=%d Click = %s Mode=%d parentname=%s\n",win->Parent, win->Handle, win->NormalName, parent_win->Mode, parent_win->Text);
	if(win==NULL) 
	{
		Dprintf("TypeApprovalNameClickEvent:: Error\n");
		return(0);
	}
	if(parent_win == NULL)
	{
		Dprintf("TypeApprovalNameClickEvent:: Windows Error\n");
	}
	//SetWidgetText(parent_win->CallWidget_ID, win->Text);//종료시에 콜한 버튼에 데이터를 출력한다.
	
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

	if(win->ID == 48) {//HOME BUTTON
		GetWidgetText(gv.handle2[1],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[0],buf);
		GetWidgetText(gv.handle2[5],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[1],buf);
		GetWidgetText(gv.handle2[9],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[2],buf);
		GetWidgetText(gv.handle2[13],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[3],buf);
		GetWidgetText(gv.handle2[17],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[4],buf);
		GetWidgetText(gv.handle2[21],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[5],buf);
		GetWidgetText(gv.handle2[25],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[6],buf);
		GetWidgetText(gv.handle2[29],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[7],buf);
		GetWidgetText(gv.handle2[33],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[8],buf);
		GetWidgetText(gv.handle2[37],buf);buf[5]=0;		
		strcpy(flash.tmsDataName[9],buf);
        DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
		CloseWindow(NULL, parent);
		return 1;
	} 
    else if(win->ID == 1) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
	}        
    else if(win->ID == 5) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    else if(win->ID == 9) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    else if(win->ID == 13) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    else if(win->ID == 17) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    else if(win->ID == 21) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    else if(win->ID == 25) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    else if(win->ID == 29) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    else if(win->ID == 33) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    else if(win->ID == 37) {
		KeyBoard(gv.handle2[win->ID], (1024-644)/2, (LCD_Y-394)/2, NULL,NULL);
    }        
    
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	
	return(1);
}
//MessageQClear
int	 TypeApprovalNameCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int		sx = 10;
	int		sy = 10;
    int     add_sx=50;
    int     width=243, height=49;
    int     add_sy=width+10;
    char    buf[64];
	//int		color=clWhite;
	//int		textColor=clBlack;
	//int		i=0;
	//u32		id = 0;
	
	Windows 	*win = GetWindow(parent, handle,"TypeApprovalNameCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(win==NULL)
	{
		Dprintf("TypeApprovalNameCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("TypeApprovalNameCreateEvent::GetGraphics Error\n");
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
	Label(handle, sx+add_sy, sy, width, height, "출력이름",  clBlack);
	Label(handle, sx+add_sy+add_sy, sy, width, height, "항목이름",  clBlack);
	Label(handle, sx+add_sy+add_sy+add_sy, sy, width, height, "출력이름",  clBlack);
	
	sy+=add_sx;
	gv.handle2[0] = SButton(handle, sx, sy, width, height, 0, flash.Uart[USART_1]->SetupName[0], PUSH_BUTTON, 0, TypeApprovalNameClickEvent);
	if(strcmp(flash.Uart[USART_1]->SetupName[0],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[0]);
	}
	else {
		strcpy(flash.tmsDataName[0],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[1] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 1, TypeApprovalNameClickEvent);
	gv.handle2[2] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[0], PUSH_BUTTON, 2, TypeApprovalNameClickEvent);
	gv.handle2[3] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[0], PUSH_BUTTON, 3, TypeApprovalNameClickEvent);

	sy+=add_sx;
	gv.handle2[4] = SButton(handle, sx, sy, width, height, 0, flash.Uart[USART_1]->SetupName[1], PUSH_BUTTON, 4, TypeApprovalNameClickEvent);
	if(strcmp(flash.Uart[USART_1]->SetupName[1],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[1]);
	}
	else {
		strcpy(flash.tmsDataName[1],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[5] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 5, TypeApprovalNameClickEvent);
	gv.handle2[6] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[1], PUSH_BUTTON, 6, TypeApprovalNameClickEvent);
	gv.handle2[7] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[1], PUSH_BUTTON, 7, TypeApprovalNameClickEvent);

	sy+=add_sx;
	gv.handle2[8] = SButton(handle, sx, sy, width, height, 0, flash.Uart[USART_1]->SetupName[2], PUSH_BUTTON, 8, TypeApprovalNameClickEvent);
    //sprintf(buf,"%s",flash.tmsDataName[2]);
	if(strcmp(flash.Uart[USART_1]->SetupName[2],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[2]);
	}
	else {
		strcpy(flash.tmsDataName[2],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[9] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 9, TypeApprovalNameClickEvent);
	gv.handle2[10] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[2], PUSH_BUTTON, 10, TypeApprovalNameClickEvent);
	gv.handle2[11] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,0, flash.Uart[USART_4]->SetupName[2], PUSH_BUTTON, 11, TypeApprovalNameClickEvent);
	
	sy+=add_sx;
	gv.handle2[12] = SButton(handle, sx, sy, width, height, 0, flash.Uart[USART_1]->SetupName[3], PUSH_BUTTON, 12, TypeApprovalNameClickEvent);
    //sprintf(buf,"%s",flash.tmsDataName[3]);
	if(strcmp(flash.Uart[USART_1]->SetupName[3],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[3]);
	}
	else {
		strcpy(flash.tmsDataName[3],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[13] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 13, TypeApprovalNameClickEvent);
	gv.handle2[14] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[3], PUSH_BUTTON, 14, TypeApprovalNameClickEvent);
	gv.handle2[15] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[3], PUSH_BUTTON, 15, TypeApprovalNameClickEvent);

	sy+=add_sx;
	gv.handle2[16] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[4], PUSH_BUTTON, 16, TypeApprovalNameClickEvent);
    //sprintf(buf,"%s",flash.tmsDataName[4]);
	if(strcmp(flash.Uart[USART_1]->SetupName[4],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[4]);
	}
	else {
		strcpy(flash.tmsDataName[4],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[17] = SButton(handle, sx+add_sy, sy, width, height, 0, buf, PUSH_BUTTON, 17, TypeApprovalNameClickEvent);
	gv.handle2[18] = SButton(handle, sx+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_3]->SetupName[4], PUSH_BUTTON, 18, TypeApprovalNameClickEvent);
	gv.handle2[19] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[4], PUSH_BUTTON, 19, TypeApprovalNameClickEvent);

	sy+=add_sx;		   
	gv.handle2[20] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[5], PUSH_BUTTON, 20, TypeApprovalNameClickEvent);
    //sprintf(buf,"%s",flash.tmsDataName[5]);
	if(strcmp(flash.Uart[USART_1]->SetupName[5],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[5]);
	}
	else {
		strcpy(flash.tmsDataName[5],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[21] = SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 21, TypeApprovalNameClickEvent);
	gv.handle2[22] = SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[5], PUSH_BUTTON, 22, TypeApprovalNameClickEvent);
	gv.handle2[23] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height, 0, flash.Uart[USART_4]->SetupName[5], PUSH_BUTTON, 23, TypeApprovalNameClickEvent);

	sy+=add_sx;		   
	gv.handle2[24] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[6], PUSH_BUTTON, 24, TypeApprovalNameClickEvent);
    //sprintf(buf,"%s",flash.tmsDataName[6]);
	if(strcmp(flash.Uart[USART_1]->SetupName[6],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[6]);
	}
	else {
		strcpy(flash.tmsDataName[6],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[25] = SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 25, TypeApprovalNameClickEvent);
	gv.handle2[26] = SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[6], PUSH_BUTTON, 26, TypeApprovalNameClickEvent);
	gv.handle2[27] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[6], PUSH_BUTTON, 27, TypeApprovalNameClickEvent);
	
	sy+=add_sx;		   
	gv.handle2[28] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[7], PUSH_BUTTON, 28, TypeApprovalNameClickEvent);
    //sprintf(buf,"%s",flash.tmsDataName[7]);
	if(strcmp(flash.Uart[USART_1]->SetupName[7],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[7]);
	}
	else {
		strcpy(flash.tmsDataName[7],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[29] = SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 29, TypeApprovalNameClickEvent);
	gv.handle2[30] = SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[7], PUSH_BUTTON, 30, TypeApprovalNameClickEvent);
	gv.handle2[31] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[7], PUSH_BUTTON, 31, TypeApprovalNameClickEvent);

	sy+=add_sx;		   
	gv.handle2[32] = SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[8], PUSH_BUTTON, 32, TypeApprovalNameClickEvent);
    //sprintf(buf,"%s",flash.tmsDataName[8]);
	if(strcmp(flash.Uart[USART_1]->SetupName[8],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[8]);
	}
	else {
		strcpy(flash.tmsDataName[8],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[33] = SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 33, TypeApprovalNameClickEvent);
	gv.handle2[34] = SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[8], PUSH_BUTTON, 34, TypeApprovalNameClickEvent);
	gv.handle2[35] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[8], PUSH_BUTTON, 35, TypeApprovalNameClickEvent);

	sy+=add_sx;		   
	gv.handle2[36] = 	SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[9], PUSH_BUTTON, 36, TypeApprovalNameClickEvent);
    //sprintf(buf,"%s",flash.tmsDataName[9]);
	if(strcmp(flash.Uart[USART_1]->SetupName[9],"NONE")) {
		sprintf(buf,"%s",flash.tmsDataName[9]);
	}
	else {
		strcpy(flash.tmsDataName[9],"NONE");
		strcpy(buf,"NONE");
	}
	gv.handle2[37] = 	SButton(handle, sx+add_sy, sy, width, height,  0, buf, PUSH_BUTTON, 37, TypeApprovalNameClickEvent);
	gv.handle2[38] = 	SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[9], PUSH_BUTTON, 38, TypeApprovalNameClickEvent);
	gv.handle2[39] = 	SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[9], PUSH_BUTTON, 39, TypeApprovalNameClickEvent);

	sy+=add_sx;		   
	gv.handle2[40] = 	SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[10], PUSH_BUTTON, 40, TypeApprovalNameClickEvent);
	gv.handle2[41] = 	SButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 41, TypeApprovalNameClickEvent);
	gv.handle2[42] = 	SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[10], PUSH_BUTTON, 42, TypeApprovalNameClickEvent);
	gv.handle2[43] = 	SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[10], PUSH_BUTTON, 43, TypeApprovalNameClickEvent);

	sy+=add_sx;		   
	gv.handle2[44] = 	SButton(handle, sx, sy, width, height,  0, flash.Uart[USART_1]->SetupName[11], PUSH_BUTTON, 44, TypeApprovalNameClickEvent);
	gv.handle2[45] = 	SButton(handle, sx+add_sy, sy, width, height,  0, "NONE", PUSH_BUTTON, 45, TypeApprovalNameClickEvent);
	gv.handle2[46] = 	SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_3]->SetupName[11], PUSH_BUTTON, 46, TypeApprovalNameClickEvent);
	gv.handle2[47] = 	SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, height,  0, flash.Uart[USART_4]->SetupName[11], PUSH_BUTTON, 47, TypeApprovalNameClickEvent);
	
	sy+=add_sx;		   
	//gv.handle2[49] = 	SButton(handle, sx+add_sy+add_sy, sy, width, height,  0, "SAVE", PUSH_BUTTON, 49, TypeApprovalNameClickEvent);
	//SetWidgetColor(gv.handle2[49],YELLOW,YELLOW);
    gv.handle2[48] = SButton(handle, sx+add_sy+add_sy+add_sy, sy, width, 60, "48G_Home_Icon2.bmp", "HOME", PUSH_BUTTON, 48, TypeApprovalNameClickEvent);
	SetWidgetColor(gv.handle2[48],YELLOW,YELLOW);
	PaintDrawControl(ON);
	
	return(1);
}

void TypeApprovalNameKeypad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title)
{
	WidgetStruct wid_struct;	

	wid_struct.FontNo = font_no;
	wid_struct.CallWidget_ID = id;

	makeWindow(
				LCD,
				"TypeApprovalNameKeypad",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				1024, 768,			// Size
				SHOW,					// Create after Show
				TypeApprovalNameCreateEvent,		// CreateEvent
				TypeApprovalNamePaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				TypeApprovalNameTimerEvent,					// TimerEvent
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

