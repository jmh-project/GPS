/******************************************************************************
*    	File name	: TransParencyTextBox.c 
*		Description : TEECO GUI Window Control Widget [TransParencyTextBox]
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: www.ziontek.co.kr
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
							Constant & Macros
******************************************************************************/


/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

/******************************************************************************
							Example Program
******************************************************************************/
/*


*/

#define	FONT_Y_SIZE	16

//총표시 문자길이
static char WinText[512];

//버튼의 눌린모양을 변경할때 사용한다.
int SetWidgetShow(U32 id, int status)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle," ");
	if(win==NULL)
	{
		Dprintf("SetWidgetStatus::Window Error\n");
		return(0);
	}
	if(status==ON)
	{
		win->ShowStatus = ON;
	}
	else
	{
		win->ShowStatus = OFF;
	}
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, status, 0, 0);

	return(1);		
}

//버튼의 눌린모양을 변경할때 사용한다.
int SetWidgetMultiText(U32 id, char *text)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle," ");
	if(win==NULL)
	{
		Dprintf("SetWidgetMultiText::Window Error\n");
		return(0);
	}
	strcpy(WinText, text);
	if(win->PaintEvent != NULL) 
	{
		win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
		return(1);
	}
	return(-1);		
}


int	MultiTextBoxCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	int			sx=0, sy=0;
	int			page=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	//부모윈도우의 바탕색을 칠한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
		
    return(1);
}

int	MultiTextBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	char			*sbuf;
	char			cdata;
	int			len, i, sx=0, sy=0;
	int			page=0;
	int			line;
//	int			han_flag=0, han_ok=0;

	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);
	//부모윈도우의 바탕색을 칠한다.
	switch(parent)
	{
		case 1: page = CONTAINER_WINDOW1_SCREEN; break;
		case 2: page = CONTAINER_WINDOW2_SCREEN; break;
		case 3: page = CONTAINER_WINDOW3_SCREEN; break;
		case 4: page = CONTAINER_WINDOW4_SCREEN; break;
		case 5: page = CONTAINER_WINDOW5_SCREEN; break;
	}
	Gr->BitBlt(page, win->abs_sx, win->abs_sy, sx, sy, win->width, win->height);
	
	if(strlen(WinText))
	{
		Gr->SetFont(win->FontNo);
		Gr->SetHangulFont(37);

		Gr->SetFontGap(win->FontGap);
		Gr->SetFontSpaceGap(win->FontSpaceGap);
		Gr->SetFontColor(win->FontColor);
		
		len = strlen(WinText);
		sbuf = (char *)malloc(len);

		sx=5;
		sy=2;	//멀티라인이기때문에 고정처리한다.
		line=0;
		
		for(i=0; i<len; i++) //전체문자길이중에서
		{			
			cdata = WinText[i];
			sbuf[line++]=cdata;			
			if(cdata=='\n')
			{
				sbuf[line-1] = 0;
				Gr->TextOut(sx, sy, sbuf);					
				sy+=FONT_Y_SIZE;
				line=0;
			}				
		}
		free(sbuf);
		//Dprintf("\tDebug::%s %s()->%4d:: %s\n", __FILE__, __FUNCTION__, __LINE__, win->Text);
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


U32 MultiTextBox(HANDLE ContainerHandle, 
			int 		sx, int sy, int width, int height, 
			int 		font_no,
			char		*text		//init Character
			)

{
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle,"");
	
	if(GetWindowName("")) 
	{
		Dprintf("MultiTextBox::Only One\n");
		return 0;		
	}	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	
	wid_struct.FontColor = WHITE;

	wid_struct.FontNo = font_no;
	wid_struct.FontGap = 4;
	wid_struct.FontSpaceGap = 7;	
	
	if(strlen(text) >= 512)
	{
		//Dprintf("MultiTextBox:: 문자열이 512를 넘어간 에러발생\n");
		return(0);
	}
	strcpy(WinText, text);
	
	handle=makeWindow(
				LCD,						// Display Section Position
				"MultiTextBox",				// Windows Name
				ContainerHandle,				// Parent Window Handle
				WIDGET_WINDOW,			// Windows Kind
				STYLE_NONE,				// Windows Style
				sx, sy, 						// Start Position
				width, height,					// Size
				HIDDEN,						// Create after Show
				MultiTextBoxCreateEvent,		// CreateEvent
				MultiTextBoxPaintEvent,		// PaintEvent
				NULL,						// MouseOnEvent
				NULL,						// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				NULL,						// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				NULL,						// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				0,						// PARAMETER
				NULL,						// Back Image
				NULL,						// 
				text,							// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	//Dprintf("TextBox::id = %08x\n", id);	
	return(id);
}				
#ifdef __cplusplus
}
#endif
