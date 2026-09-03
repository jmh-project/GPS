/******************************************************************************
*       File name	: TableWidget.c 
*		Description : TEECO GUI Window Control Widget [TableBox]
*		Company		: idnics co.,
*		Version		: 0.10 (2014년 10월24일 1차 배포함)
*		Version		: 0.11 
*		Contact		: www.ziontek.co.kr
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
// 위젯을만들때는 PaintDrawControl() 함수를 사용하면 안된다.
// 이파일은 위젯을작성할때 복사해서 사용해야하는 견본이다.
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#if 1

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define	CELL_MAX_STRING	64	//셀당 최대표시할수있는 문자의수

#define	INT_TYPE		0
#define	FLOAT_TYPE		1
#define	STRING_TYPE	2

#define	CELL_Y_HEIGHT	30		//그림파일일때는 라인빼고 공백길이만

/******************************************************************************
							Private & Local Variables
******************************************************************************/
static int *Cell_Info;	//[0] = X1_CELL_WIDTH
static int *Cell_Data;	//[0] = INT, STRING, FLOAT
static int *Cell_Align;//[0] = RIGHT_ALIGN

static int Display_Index=0;
static int Display_Cell_Cnt=0; //한화면에 표시하는 셀의열수
static int Cell_Colume_Cnt=0;
//static int Cell_Row_Cnt=0;
char		Table_FileName[64];
char		Table_Date[64];

//현재테이블의 최상위라인의 배열번호를 나타낸다.
static int 	current_line=0;

//현재 터치가 눌린 X, Y 좌표를 표시하는 전역변수이다.
static int 	Current_Cursor_X=0; //현재보이는 화면의 셀의 위치를 계산한다. 
static int 	Current_Cursor_Y=0;

//현재로드된 데이터의 길이를 나타낸다.
static int TableData_Count=0;

//위젯이 투명모드인지를 판단한다.
static int TransparencyMode=0;

extern Record_Data *UPS_Record;

/******************************************************************************
							Function Prototype
******************************************************************************/
int SDtoRecord(char *filename, char *date_time, int cnt, int kind);
extern void GraphicsPointerSave(void);
extern void GraphicsPointerLoad(void);
int Table_RecordDisplay(Windows *win, Graphics  *Gr, int col_no,  int array_no, int select_display);
/******************************************************************************
							Example Program
******************************************************************************/
//============================================================================================================
// 작성한 위젯의 데이터를 갱신하기 위해서는 아래와같은 함수를 만들어야한다.
// 전달할데이터의 구조에따라 매개변수는 필요한대로 지정한다.
// 예제에서는 PaintEvent를 호출하지만 프로그램의 흐름에따라 다른 이벤트를 호출해도 상관없다.
//============================================================================================================
int SetWidget_TableIndex(U32 id,  int row_cnt)
{	
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Error\n", __FUNCTION__);
		return(0);
	}
    
	GraphicsPointerSave();
	//Display_Index = row_cnt;
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, row_cnt, 0);
	
	GraphicsPointerLoad();
	
	return(1);		
}

//현재 로드된 데이터의 길이를 얻는다.
int GetWidget_TableDataCount(U32 id)
{	
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Error\n", __FUNCTION__);
		return(0);
	}
	return(TableData_Count);
}

int GetWidget_TableIndex(U32 id)
{	
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Error\n", __FUNCTION__);
		return(0);
	}
	return(Display_Index);
}


int SetWidget_TableI_Scroll(U32 id,  int row_cnt, int mode) //mode=2: UP, mode=3:DOWN
{	
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Error\n", __FUNCTION__);
		return(0);
	}
    
	GraphicsPointerSave();
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, mode, row_cnt, 0);
	
	GraphicsPointerLoad();
	
	return(1);		
}


int SetWidget_TableName(U32 id,  char *filename, char *date, int row_cnt)
{	
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Error\n", __FUNCTION__);
		return(0);
	}
    //Dprintf("SetWidget_TableName:: Entry\n");
	GraphicsPointerSave();
	strcpy(Table_FileName, filename);
	strcpy(Table_Date, date);
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 1, row_cnt, 0);
	
	GraphicsPointerLoad();
    //Dprintf("SetWidget_TableName:: Exit\n");
	
	return(1);		
}


//============================================================================================================
// TimerEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
//============================================================================================================
int	Table_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			page=0;
	int			sx=0, sy=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
	
	if(TransparencyMode==YES)
	{
		//부모윈도우의 바탕색을 칠한다.투명처리용 위젯을 만들때 사용한다.
		switch(parent)
		{
			case 1: page = CONTAINER_WINDOW1_SCREEN; break;
			case 2: page = CONTAINER_WINDOW2_SCREEN; break;
			case 3: page = CONTAINER_WINDOW3_SCREEN; break;
			case 4: page = CONTAINER_WINDOW4_SCREEN; break;
			case 5: page = CONTAINER_WINDOW5_SCREEN; break;
		}
		Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	}
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

void Table_Cell_Display(Record_Data data, int index)
{
	switch(index)
	{
		case 0:  Dprintf("%s \n", data.Mode);  break;
		case 1:  Dprintf("%s \n", data.Date); break;
		case 2:  Dprintf("%6.1f \n", data.AC_IN_Volt); break;
		case 3:  Dprintf("%6.1f \n", data.AC_IN_Freq); break;
		case 4:  Dprintf("%6.1f \n", data.PV_IN_Volt); break;
		case 5:  Dprintf("%5d \n", data.AC_APP_Pow); break;
		case 6:  Dprintf("%5d \n", data.AC_Active_Pow); break;
		case 7:  Dprintf("%6.1f \n", data.Battery_Volt); break;
		case 8:  Dprintf("%5d \n", data.Battery_Cap); break;
		case 9:  Dprintf("%5d \n", data.Chargin_Current); break;
		case 10: Dprintf("%6.1f \n", data.AC_OUT_Volt); break;
		case 11: Dprintf("%6.1f \n", data.AC_OUT_Freq); break;
	}
}

//============================================================================================================
// MouseOnEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
//============================================================================================================
int	Table_MouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
//	int			page=0;
//	int			sx=0, sy=0;
	int			line=0;
	//마우스의 좌표를 읽을때 사용한다.
	int 		tx = LOWORD(param);
	int 		ty = HIWORD(param);	
	int		ycell_pos, xcell_pos;
	int		cell_index = Display_Index;
	
	if( TableData_Count ==0 ) return(0);
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		//Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
	//사용자 프로그램 영역
	//현재좌표가 테이블의 어떤셀을 가르키는지 연산한다.
	ycell_pos = ty / CELL_Y_HEIGHT; //셀의 높이
	//Dprintf("current_line=%d\n", current_line);
	
	if(tx >= 0 &&  tx < Cell_Info[cell_index]) 	xcell_pos = 0 + Display_Index;
	else if(tx >=  Cell_Info[cell_index] &&  tx < (Cell_Info[cell_index] + Cell_Info[cell_index+1]) ) 	xcell_pos = 1 + Display_Index;
	else if(tx >=  Cell_Info[cell_index+1] &&  tx < (Cell_Info[cell_index] + Cell_Info[cell_index+1] + Cell_Info[cell_index+2] )) 	xcell_pos = 2 + Display_Index;
	else if(tx >=  Cell_Info[cell_index+2] &&  tx < (Cell_Info[cell_index] + Cell_Info[cell_index+1] + Cell_Info[cell_index+2]  + Cell_Info[cell_index+3] )) 	xcell_pos = 3 + Display_Index;
	else if(tx >=  Cell_Info[cell_index+3] &&  tx < (Cell_Info[cell_index] + Cell_Info[cell_index+1] + Cell_Info[cell_index+2]  + Cell_Info[cell_index+3]  + Cell_Info[cell_index+4] ) )	xcell_pos = 4 + Display_Index;
	else if(tx >=  Cell_Info[cell_index+4] &&  tx < (Cell_Info[cell_index] + Cell_Info[cell_index+1] + Cell_Info[cell_index+2]  + Cell_Info[cell_index+3]  + Cell_Info[cell_index+4]  + Cell_Info[cell_index+5] ) )	xcell_pos = 5 + Display_Index;
	
	line = ycell_pos;
	
	Current_Cursor_Y = line + current_line;
	Current_Cursor_X = xcell_pos;
	//Dprintf("xcell=%d DisplayIndex=%d\n", xcell_pos, Display_Index);
	if(Current_Cursor_Y < TableData_Count) //빈공백을 가르키지 않으면 처리한다. 빈공백에 마캉되는것을 방지하기위함
	{
		//int Table_RecordDisplay(Windows *win, Graphics  *Gr, int col_no,  int array_no, int select_display)
		if( win->PaintEvent != 0) win->PaintEvent(win->Parent, win->Handle, 0, Display_Index, 0);	
		Table_RecordDisplay(win, Gr, line, current_line+line, YES);			
	}	
	//Dprintf("Table_MouseOnEvent:: CELL[%d, %d]=", Current_Cursor_X, Current_Cursor_Y );
	Table_Cell_Display(UPS_Record[Current_Cursor_Y], Current_Cursor_X);

	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//============================================================================================================
// MouseOffEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
//============================================================================================================
int	Table_MouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			page=0;
	int			sx=0, sy=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
	if(TransparencyMode==YES)
	{
		//부모윈도우의 바탕색을 칠한다.투명처리용 위젯을 만들때 사용한다.
		switch(parent)
		{
			case 1: page = CONTAINER_WINDOW1_SCREEN; break;
			case 2: page = CONTAINER_WINDOW2_SCREEN; break;
			case 3: page = CONTAINER_WINDOW3_SCREEN; break;
			case 4: page = CONTAINER_WINDOW4_SCREEN; break;
			case 5: page = CONTAINER_WINDOW5_SCREEN; break;
		}
		Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	}
	//사용자 프로그램 영역
	{
		
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//============================================================================================================
// CreateEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
// 위젯이 처음 만들어질때만 호출된다. 초기화, 폰트로딩, 시간이 걸리는 그림파일 로딩등 의 작업을 한다.
//============================================================================================================
int	Table_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
//	U32		timerID=0;
	int			page=0;
	int			sx=0, sy=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Gr->FillRect(0, 0, 0, 0);//Dummy
	if(TransparencyMode==YES)
	{
		//부모윈도우의 바탕색을 칠한다.투명처리용 위젯을 만들때 사용한다.
		switch(parent)
		{
			case 1: page = CONTAINER_WINDOW1_SCREEN; break;
			case 2: page = CONTAINER_WINDOW2_SCREEN; break;
			case 3: page = CONTAINER_WINDOW3_SCREEN; break;
			case 4: page = CONTAINER_WINDOW4_SCREEN; break;
			case 5: page = CONTAINER_WINDOW5_SCREEN; break;
		}
		Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	}	
	//사용자 프로그램 영역
	{
		Table_FileName[0]=0;
		Table_Date[0]=0;
		win->FontColor = clBlack;
		//timerID =  OpenTimer(parent, handle, 1000);	
		//StartTimer(timerID);
	}
	Gr->Draw(parent, handle, command, param, Data);
    return(1);
}
/*

	Dprintf("%s %s %6.1f %6.1f %6.1f %5d %5d %6.1f %5d %5d %6.1f %6.1f\n", \
		data.Mode, 
		data.Date, 
		data.AC_IN_Volt, \f
		data.AC_IN_Freq,\f
		data.PV_IN_Volt,\f
		data.AC_APP_Pow,\d
		data.AC_Active_Pow,\d
		data.Battery_Volt,\f
		data.Battery_Cap,\d
		data.Chargin_Current,\d
		data.AC_OUT_Volt,\f
		data.AC_OUT_Freq);f
*/
int RightTrim_String(char *str)
{
	int	len=strlen(str);
	char	buf[len];
	int	cnt=0;
	int	start=0, i;
	
	if(len==0) return(0);

	for(i=0; i<len; i++) //주어진 총문자를 검색하면서
	{
		if(str[i] == ' ' && start==0) //공백이면 통과고 문자가시작되면 처리하면 안된다.
		{			
		}
		else
		{
			start=1;
			buf[cnt++] = str[i];
		}
	}
	buf[cnt]=0;
	for(i=0; i<cnt; i++)
		str[i] = buf[i];
	str[i]=0;
	return(1);
}

#define	RIGHT_OFFSET	8
// col_no로 주어진 행위치에 array_no의 배열(레코드)의 내용을 디스플레이한다.
// 현재는 UPS 모니터용으로 되어있지만 이함수만 바꿔서 다른용도로 사용할수있다.
int Table_RecordDisplay(Windows *win, Graphics  *Gr, int col_no,  int array_no, int select_display)
{
	int		sx=0, sy=0;	
	char		sbuf[CELL_MAX_STRING];
	int		y_index = 0; //길이만큼 옵셋을 정하기위한 변수
	int		cell=0;
	int		len=0;
//	int		cell_len=0;
	int		dx=0;
	int		xtemp=0;
	
	/*
	Cell_Info[1] = X1_WIDTH;	
	Cell_Data[0] = X1_DATA_TYPE;
	Display_Index;
	Cell_Colume_Cnt;
	Cell_Row_Cnt;
	*/
	// 디스플레이할 Y좌표를 구한다.
	sy = col_no * CELL_Y_HEIGHT + y_index;	
	cell = Display_Index;
	
	//선택된 행이 블록표시되게한다.
	if(select_display==YES)
	{
		int ux=0;
		Gr->SetColor(clYellow);
		Gr->FillRect(sx, col_no * CELL_Y_HEIGHT, win->width-10, CELL_Y_HEIGHT-10);
		Gr->SetColor(clCyan);
		
		if(Current_Cursor_X >= Display_Index)
			xtemp = Current_Cursor_X- Display_Index;
		switch(xtemp)
		{
			case 0:  ux = 0;  	Gr->FillRect(ux, col_no * CELL_Y_HEIGHT, Cell_Info[0], CELL_Y_HEIGHT-10); 	break;
			case 1:  ux = Cell_Info[0]+1;  	
				Gr->FillRect(ux, col_no * CELL_Y_HEIGHT, Cell_Info[1]-12, CELL_Y_HEIGHT-10); 	break;
			case 2:  ux = Cell_Info[0] + Cell_Info[1] +1;  	
				Gr->FillRect(ux, col_no * CELL_Y_HEIGHT, Cell_Info[2]-12, CELL_Y_HEIGHT-10); 	break;
			case 3:  ux = Cell_Info[0] + Cell_Info[1] + Cell_Info[2]+1;  	
				Gr->FillRect(ux, col_no * CELL_Y_HEIGHT, Cell_Info[3]-12, CELL_Y_HEIGHT-10); 	break;
			case 4:  ux = Cell_Info[0] + Cell_Info[1] + Cell_Info[2]+ Cell_Info[3]+1;  	
				Gr->FillRect(ux, col_no * CELL_Y_HEIGHT, Cell_Info[4]-12, CELL_Y_HEIGHT-10); 	break;
			case 5:  ux = Cell_Info[0] + Cell_Info[1] + Cell_Info[2] + Cell_Info[3] + Cell_Info[4]+1;  	
				Gr->FillRect(ux, col_no * CELL_Y_HEIGHT, Cell_Info[5]-12, CELL_Y_HEIGHT-10); 	break;
		}		
	}
	// 레코드의 항목중 먼저표시할 인덱스를 구분한다.
	switch(Display_Index) 
	{
		case 0: 
		{
			if(Cell_Data[cell]==STRING_TYPE)
			{
				strcpy(sbuf, UPS_Record[array_no].Mode);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  	dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;

				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==1) break; 				
			}
			else
				Dprintf("0.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell]), cell;
		}
		case 1: 
		{
			if(Cell_Data[cell]==STRING_TYPE)
			{
				strcpy(sbuf, UPS_Record[array_no].Date);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  	dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;

				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==2) break; 				
			}
			else
				Dprintf("1.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 2: 
		{
			if(Cell_Data[cell]==FLOAT_TYPE)
			{
				sprintf(sbuf, "%6.1f", UPS_Record[array_no].AC_IN_Volt);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;

				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==3) break; 				
			}
			else
				Dprintf("2.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 3: 
		{
			if(Cell_Data[cell]==FLOAT_TYPE)
			{
				sprintf(sbuf, "%6.1f", UPS_Record[array_no].AC_IN_Freq);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  	dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==4) break; 				
			}
			else
				Dprintf("3.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 4: 
		{
			if(Cell_Data[cell]==FLOAT_TYPE)
			{
				sprintf(sbuf, "%6.1f", UPS_Record[array_no].PV_IN_Volt);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  					dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==5) break; 				
			}
			else
				Dprintf("4.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 5: 
		{
			if(Cell_Data[cell]==INT_TYPE)
			{
				sprintf(sbuf, "%5d", UPS_Record[array_no].AC_APP_Pow);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  				dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==6) break; 				
			}
			else
				Dprintf("5.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 6: 
		{
			if(Cell_Data[cell]==INT_TYPE)
			{
				sprintf(sbuf, "%5d", UPS_Record[array_no].AC_Active_Pow);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  		dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==7) break; 				
			}
			else
				Dprintf("6.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 7: 
		{
			if(Cell_Data[cell]==FLOAT_TYPE)
			{
				sprintf(sbuf, "%6.1f", UPS_Record[array_no].Battery_Volt);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  		dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==8) break; 				
			}
			else
				Dprintf("7.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 8: 
		{
			if(Cell_Data[cell]==INT_TYPE)
			{
				sprintf(sbuf, "%5d", UPS_Record[array_no].Battery_Cap);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  			dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==9) break; 				
			}
			else
				Dprintf("8.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 9: 
		{
			if(Cell_Data[cell]==INT_TYPE)
			{
				sprintf(sbuf, "%5d", UPS_Record[array_no].Chargin_Current);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  		dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==10) break; 				
			}
			else
				Dprintf("9.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 10: 
		{
			if(Cell_Data[cell]==FLOAT_TYPE)
			{
				sprintf(sbuf, "%6.1f", UPS_Record[array_no].AC_OUT_Volt);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  	dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==11) break; 				
			}
			else
				Dprintf("10.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
		case 11: 
		{
			if(Cell_Data[cell]==FLOAT_TYPE)
			{
				sprintf(sbuf, "%6.1f", UPS_Record[array_no].AC_OUT_Freq);
				RightTrim_String(sbuf);
				len = LowTextLen(sbuf, fArial12);	
				if(Cell_Align[cell] == LEFT_ALIGN) dx = 3;
				else if(Cell_Align[cell] == CENTER_ALIGN) dx =  ( Cell_Info[cell] - len) / 2;
				else if(Cell_Align[cell] == RIGHT_ALIGN)  		dx = (Cell_Info[cell] - len)-RIGHT_OFFSET;
				dx += sx;
				Gr->LowTextOut(dx, sy, sbuf, win->FontColor, fArial12);
				sx += Cell_Info[cell];
				cell++;
				//한화면에 표시하는 열수가 같으면 탈출한다.
				if(Display_Cell_Cnt==12) break; 				
			}
			else
				Dprintf("11.Table_RecordDisplay::TYPE Error=%d  Cnt=%d \n", Cell_Data[cell], cell);
		}
	}
	return(1);
}

//============================================================================================================
// PaintEvent
// 위젯에서는 PaintDrawControl()을 사용하면 안된다.
//============================================================================================================
int	Table_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			page=0, i;
	int			sx=0, sy=0;
//	char			sbuf[CELL_MAX_STRING];
	static 		int first=0;
	static 		int	retcnt=0;
	
	static 		int first_line=0;
//	static 		int last_line=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	//Dprintf("Table_PaintEvent:: Entry\n");
	if(TransparencyMode==YES)
	{
		//부모윈도우의 바탕색을 칠한다.투명처리용 위젯을 만들때 사용한다.
		switch(parent)
		{
			case 1: page = CONTAINER_WINDOW1_SCREEN; break;
			case 2: page = CONTAINER_WINDOW2_SCREEN; break;
			case 3: page = CONTAINER_WINDOW3_SCREEN; break;
			case 4: page = CONTAINER_WINDOW4_SCREEN; break;
			case 5: page = CONTAINER_WINDOW5_SCREEN; break;
		}
		Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	}		
	else
	{
		Gr->SetColor(clYellow);
		Gr->FillRect(0, 0, win->width, win->height);//Dummy
	}
	if(first==0)
	{
		//Display_Index = param;
		first=1;
	}
	else
	{
		//디스플레이할 열번호를 갱신한다.
		Display_Index = param;
		
		if(command==1) //파일을 읽는모드
		{
			if(UPS_Record != NULL) free(UPS_Record);			
			//SD에서 cnt만큼의 레코드를 읽어들여 UPS_Record[]에 저장한다.
			retcnt = SDtoRecord(Table_FileName, Table_Date, 60, 1);
			if(retcnt) //시간단위검색
			{
				/*
				//내용을 디버거로 출력한다.
				for(int i=0; i<retcnt; i++)
					DisplayStruct_Index(UPS_Record[i], Display_Index);	//UART Display	
				Dprintf("SDtoStruct Data View Total=%d\n", retcnt);
				*/
				TableData_Count = retcnt;
			}
			else
			{
				Dprintf("Test_SDtoRecord::Error or Date not Found\n");
			}	
			if(retcnt < Cell_Colume_Cnt)
			{
				for(i=0; i<retcnt; i++)
					Table_RecordDisplay(win, Gr, i, i, NO);
				first_line = 0;
				current_line = first_line;
			}
			else
			{
				for(i=0; i<Cell_Colume_Cnt; i++)
					Table_RecordDisplay(win, Gr, i, i, NO);
				first_line = 0;
				current_line = first_line;
			}
		}
		else if(command==0) //읽어들인 구조체에서 디스플레이의 변경
		{
			//현재 1번줄의 데이터번호가 전체데이터수보다 작을때만 표시한다. 
			if(current_line < retcnt)
			{
				if(current_line >= retcnt)	current_line -= Cell_Colume_Cnt;
				//표시할라인이 테이블라인수 보다 작을때
				if((retcnt - current_line) < Cell_Colume_Cnt)
				{
					for(i=current_line; i<current_line+(retcnt - current_line); i++)
					{
						if(Current_Cursor_Y == i+current_line)
							Table_RecordDisplay(win, Gr, i-current_line, i, YES);			
						else
							Table_RecordDisplay(win, Gr, i-current_line, i, NO);			
					}
				}
				else
				{
					for(i=current_line; i<current_line+Cell_Colume_Cnt; i++)
					{
						if(Current_Cursor_Y == i+current_line)
							Table_RecordDisplay(win, Gr, i-current_line, i, YES);			
						else
							Table_RecordDisplay(win, Gr, i-current_line, i, NO);					
					}
				}
			}
			else //없더라도 표시해야한다.
			{
				for(i=0; i<Cell_Colume_Cnt; i++)
				{
					if(Current_Cursor_Y == i+current_line)
						Table_RecordDisplay(win, Gr, i, current_line+i, YES);				
					else
						Table_RecordDisplay(win, Gr, i, current_line+i, NO);				
				}
			}	
		}
		else if(command==2) //UP command
		{
			if(current_line==0)
			{
				//최상위이다. 없더라도 데이터는 갱신표시해야한다.
				//Dprintf("Table:: Top Line=%d\n", current_line);
				if(retcnt < Cell_Colume_Cnt)
				{
					for(i=0; i<retcnt; i++)
					{
						if(Current_Cursor_Y == i+current_line)
							Table_RecordDisplay(win, Gr, i, i, YES);
						else
							Table_RecordDisplay(win, Gr, i, i, NO);
					}
				}
				else
				{
					for(i=0; i<Cell_Colume_Cnt; i++)
					{
						if(Current_Cursor_Y == i+current_line)
							Table_RecordDisplay(win, Gr, i, i, YES);
						else
							Table_RecordDisplay(win, Gr, i, i, NO);
					}
				}				
			}
			else
			{
				if(current_line > 0)
				{
					current_line -= Cell_Colume_Cnt;
					for(i=current_line; i<current_line+Cell_Colume_Cnt; i++)
					{
						if(Current_Cursor_Y == i+current_line)
							Table_RecordDisplay(win, Gr, i-current_line, i, YES);			
						else
							Table_RecordDisplay(win, Gr, i-current_line, i, NO);			
					}
					//Dprintf("Table:: page UP-1 Command=%d  index=%d\n", current_line , Display_Index);
				}
				else				
				{
					//Dprintf("Table:: page UP-2 Command=%d Index=%d\n", current_line, Display_Index);
				}
			}
		}
		else if(command==3) //DOWN command
		{
			//현재 1번줄의 데이터번호가 전체데이터수보다 작을때만 표시한다. 
			if(current_line < retcnt)
			{
				current_line += Cell_Colume_Cnt;
				if(current_line >= retcnt)
				{
					current_line -= Cell_Colume_Cnt;
				}
				
				//표시할라인이 테이블라인수 보다 작을때
				if((retcnt - current_line) < Cell_Colume_Cnt)
				{
					//다운키를 계속누르면 표시를 반복한다.(수정사항)
					//current_line += (retcnt - current_line);
					for(i=current_line; i<current_line+(retcnt - current_line); i++)
					{
						if(Current_Cursor_Y == i+current_line)
							Table_RecordDisplay(win, Gr, i-current_line, i, YES);			
						else
							Table_RecordDisplay(win, Gr, i-current_line, i, NO);			
					}
					//Dprintf("Table:: 1-1.page DN Command=%d\n", current_line);
				}
				else
				{
					for(i=current_line; i<current_line+Cell_Colume_Cnt; i++)
					{
						if(Current_Cursor_Y == i+current_line)
							Table_RecordDisplay(win, Gr, i-current_line, i, YES);			
						else
							Table_RecordDisplay(win, Gr, i-current_line, i, NO);			
					}
					//Dprintf("Table:: 1-2.page DN Command=%d\n", current_line);
				}
			}
			else //없더라도 표시해야한다.
			{
			}
		}
	}	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

//============================================================================================================
// 컨테이너가 닫히면서 컨테이너에서 보내오는 위젯클로즈 이벤트이다.
// 위젯자체의 데이터기능을 활성화할려면 핸들로 ID 를 구분하여야 한다.
//============================================================================================================

int	 Tabel_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	Dprintf("Tabel_CloseEvent::Close\n");
	//윈도우가 닫힐때 필요한처리를 한다.
	if(UPS_Record != NULL) free(UPS_Record);
	free(Cell_Data);
	free(Cell_Info);
	free(Cell_Align);
	
	TableData_Count = 0;
	TransparencyMode = 0;
	Display_Cell_Cnt = 0;
	return(1);
}

// 매개변수는 필요에의해 추가가 가능하지만 매개변수를 어디에 대입할지는 관리자와 협의해야한다.
// 또는 이파일을 복사해서 수정해서 사용하는것은 상관이 없다.
// 테이블의 폰트는 시스템폰트로 고정을 시키기로 한다.
U32 Table(HANDLE ContainerHandle, 				// 위젯윈도우이므로 컨테이너를지정해야한다.
			int 	sx, int sy, int width, int height, 	// 컨테이너안에 위젯의위치와 크기지정
			int 	col_cnt, int row_cnt,				// 테이블의 배열수 X, Y
			int	*cell_info,						//cell[0]=x1_width ...
			int  	*cell_data, 					//cell[0]의 데이터타입을 지정한다.FLOAT, STRING등
			int  	*cell_align, 				//cell[0]의 문자정렬모드를 지정하는 배열
			int  	display_cell_cnt,		//현재윈도우에보이는 셀의 가로수가 몇개인지 입력한다.
			int	transparency	//위젯을 투명위젯인지의 구분을 한다.
				)
{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;		// makeWindow에 매개변수가 늘어나는것을 방지하기위해 구조체를 사용한다.
	
	parent_win = GetWindow(NULL, ContainerHandle,"");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	//매개변수로 들어온 셀정보를 테이블전용의 전역변수에 대입한다.
	Cell_Info = (int *)malloc((sizeof(int) * row_cnt) );
	if(Cell_Info==0)
	{
		Dprintf("Table::Can not Memory alocation\n");
		return(0);
	}
	memcpy( (int *)Cell_Info, (int *)cell_info, row_cnt*4);

	Cell_Data = (int *)malloc(sizeof(int) * row_cnt);
	if(Cell_Data==0)
	{
		Dprintf("Table::Can not Memory alocation\n");
		return(0);
	}
	memcpy( (int *)Cell_Data, (int *)cell_data, row_cnt*4);

	Cell_Align = (int *)malloc(sizeof(int) * row_cnt);
	if(Cell_Align==0)
	{
		Dprintf("Table::Can not Memory alocation\n");
		return(0);
	}
	memcpy( (int *)Cell_Align, (int *)cell_align, row_cnt*4);

	//Display_Index = display_cell_cnt;
	TransparencyMode = transparency;
	Display_Cell_Cnt = display_cell_cnt; //현재화면에 표시되는 열의 셀수
	
	Cell_Colume_Cnt = col_cnt;
//	Cell_Row_Cnt = row_cnt;
	
	handle=makeWindow(
				LCD,					// Display Section Position
				"Table",				// Windows Name
				ContainerHandle,		// Parent Window Handle
				WIDGET_WINDOW,	// Windows Kind
				STYLE_NONE,			// Windows Style
				sx, sy, 					// Start Position
				width, height,			// Size
				SHOW,					// Create after Show
				Table_CreateEvent,	// CreateEvent
				Table_PaintEvent,		// PaintEvent
				Table_MouseOnEvent,	// MouseOnEvent
				NULL,// MouseOffEvent
				NULL,					// MouseMoveEvent 가등록되면 선택된 셀이 표시된다.
				NULL,					// GestureEvent
				NULL,			// ClickEvent
				Table_TimerEvent,	// TimerEvent
				NULL,  				// ControlEvent
				NULL,					// ChangeEvent
                NULL,           		// KeyEvent(Button)
                NULL,                 	// EncoderEvent
                NULL,                 	// FocusEvent
                Tabel_CloseEvent,		// CloseEvent
				NULL,					// PARAMETER
				NULL,					// Back Image
				NULL,					// 
				NULL,					// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	id = MAKELONG(ContainerHandle, handle);		//LOWORD->ContainerHandle, HIWORD->handle
	return(id);
}				
#endif

#ifdef __cplusplus
}
#endif

