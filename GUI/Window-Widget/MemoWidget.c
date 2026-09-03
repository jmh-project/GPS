/******************************************************************************
*    	File name	: TextBox.c 
*		Description : TEECO GUI Window Control Widget [TextBox]
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
#include "DataLog.h"
/******************************************************************************
							Constant & Macros
******************************************************************************/

/* 
VRAM
#define     MEMOBOX_BUFFER      0x6ABF2000      //Size 135,656 Byte
#define     MEMOBOX_BUF_SIZE    135656
*/

#define     MEMOBOX_BUFFER      0x64691A00      //Size 135,656 Byte
#define     MEMOBOX_BUF_SIZE    131072

/******************************************************************************
							Private & Local Variables
******************************************************************************/
extern  volatile int gFontSize_X[10];
extern  volatile int gFontSize_Y[10];
char    MemoBoxBuf[100][100];

/******************************************************************************
							Function Prototype
******************************************************************************/

/******************************************************************************
							Example Program
******************************************************************************/
int	MemoBoxMouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
 	
	return(1);
}


int	MemoBoxMouseOffEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	return(1);
}

#if defined (YSI_ENABLE)
int	MemoBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");
    static int      sx = 0;
    static int      sy = 0;
    //int             style;
    int             i, len;
    static char     charbuf[100];                           //문자표시시 들어온데이터를 통합하는 버퍼이다.
    static int      char_count=0;                           //문자표시시 입력된문자의 수이다.
    int fontSize = 24;
    
    #ifdef DATALOG_7INCH
    int  colume = (win->height / fontSize)-1;   //현재윈도우에서 몇행까지 문자가 입력되는지를 계산한다.
    #endif
    #ifdef DATALOG_8INCH
    int  colume = (win->height / fontSize);
    #endif
    char        *text = (char *)Data;                       //문자열을 표시할때는 
    static int  col_start=0;                                //현재표시하고있는 행번호이다.
    char    onechar = (char )param;                         //한문자를 표시할때 사용하는 변수이다.
    
    //int text_width;
	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	Gr->FillRect(0, 0, 0, 0);	
    //Gr->SetFont(win->FontNo);
    //Gr->SetFontColor(win->FontColor);
    //Gr->SetFontSpaceGap(win->FontSpaceGap);
    //Gr->SetFontGap(win->FontGap);    
    /*
    //프롬프트 문자를 입력한다.
    if(char_count==0)
    {
        strcpy(charbuf, win->Text);
        char_count = strlen(charbuf);
    }
    */
    SetFontAliasing("ng_eh_24.sif");
    if(command==1)  //한문자씩입력하는게 보여져야한다.
    {
        if(onechar != '\n')    //표시할문자가 입력되면
        {
            charbuf[char_count++]= onechar;
            charbuf[char_count] = 0;
            
            strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.
            
            //배경색으로 현재라인을 지우고
            Gr->SetColor(win->BackColor);
            Gr->FillRect(sx, sy, win->width, fontSize);//gFontSize_Y[win->FontNo]);
            //문자열을 출력한다.
            //Gr->TextOut(sx, sy, charbuf);  

            TextLenAliasing(MemoBoxBuf[col_start], "ng_eh_24.sif");
            Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");
        }
        else        //행을바꾸는 코드가 입력되면
        {
            if(char_count == 0) //입력된문자 없이 개행코드만 입력된경우는 빈문자열(프롬프트)을 넣는다.
            {
                strcpy(MemoBoxBuf[col_start], "    ");
            }
            
            if(sy+fontSize < win->height) //행을 내리는데 화면의 행에 여유가 잇을경우
            {
                col_start++;                    //버퍼의 행을 증가시킨다.
                sy += fontSize; //다음문자의 위치를 1칸 아래로 내린다.
                char_count = 0;                 //다시문자를 처음부터 받기위해 0 으로
                
                /*
                //프롬프트 를 출력한다.
                strcpy(charbuf, win->Text);
                char_count = strlen(charbuf);
                strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.            
                //배경색으로 현재라인을 지우고
                Gr->SetColor(win->BackColor);
                Gr->FillRect(sx, sy, win->width, gFontSize_Y[win->FontNo]);
                //문자열을 출력한다.
                Gr->TextOut(sx, sy, charbuf);                  
                */
            }
            else    //행이 꽉차서 스크롤을 행하여야 할경우
            {            
                //전체 화면을 지운다.
                Gr->SetColor(win->BackColor);
                Gr->FillRect(0, 0, win->width, win->height);
                
                //입력되었던 전체문자열을 1라인씩 당긴다.
                for(i=0; i < colume; i++)
                {
                    strcpy(MemoBoxBuf[i], MemoBoxBuf[i+1]);
                }
                
                //스크롤된 문자열을 화면에 출력한다.
                sy = 0;
                for(i=0; i < colume; i++)
                {
                    //Gr->TextOut(sx, sy, MemoBoxBuf[i]);          
                    
                    //SetFontAliasing("ng_eh_24.sif");
                    TextLenAliasing(MemoBoxBuf[i], "ng_eh_24.sif");
                    Gr->TextOutAliasing(sx, sy, MemoBoxBuf[i], clYellow, 0, "ng_eh_24.sif");

                    sy += fontSize;//gFontSize_Y[win->FontNo];
                }
                //스크롤이 되기 시작하면 입력되는문자열은 항상마지막라인이기 때문에 
                col_start = colume;
                char_count = 0;
            }            
        }
    }
    else if(command==0)   //문자열을 표시한다.
    {
        if(text != NULL)
        {
            len = strlen(text);
            if(text[len-1] != '\n')
            {
                if(char_count == 0) //문자입력이 안되었고 처음일경우
                {
                    strcpy(charbuf, text);
                    char_count = strlen(charbuf);  //입력카운트를 기록한다.
                }
                else    //한문자라도 입력이 된경우라면
                {
                    strcat(charbuf, text);
                    char_count += strlen(text);
                }            
                strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.            

                //배경색으로 현재라인을 지우고
                Gr->SetColor(win->BackColor);
                Gr->FillRect(sx, sy, win->width, fontSize);//gFontSize_Y[win->FontNo]);
                //문자열을 출력한다.
                //Gr->TextOut(sx, sy, charbuf);    
                //SetFontAliasing("ng_eh_24.sif");
                TextLenAliasing(charbuf, "ng_eh_24.sif");
                Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");
            }
            else
            {
                text[len-1] = 0;

                if(char_count == 0) //문자입력이 안되었고 처음일경우
                {
                    strcpy(charbuf, text); //현재화면에 표시할 내용을 저장시켜둔다.            
                    strcpy(MemoBoxBuf[col_start], text); //현재화면에 표시할 내용을 저장시켜둔다.            
                }
                else    //한문자라도 입력이 된경우라면
                {
                    strcat(charbuf, text);
                    char_count += strlen(text);
                    strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.            
                }
                
                if(sy+fontSize < win->height) //행을 내리는데 화면의 행에 여유가 잇을경우
                {
                    //배경색으로 현재라인을 지우고
                    Gr->SetColor(win->BackColor);
                    Gr->FillRect(sx, sy, win->width, fontSize);
                    //문자열을 출력한다.
                    //Gr->TextOut(sx, sy, charbuf);  
                    //SetFontAliasing("ng_eh_24.sif");
                    TextLenAliasing(charbuf, "ng_eh_24.sif");
                    Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");

                    col_start++;                    //버퍼의 행을 증가시킨다.
                    sy += fontSize;//gFontSize_Y[win->FontNo]; //다음문자의 위치를 1칸 아래로 내린다.
                    char_count = 0;                 //다시문자를 처음부터 받기위해 0 으로                    
                }
                else    //행이 꽉차서 스크롤을 행하여야 할경우
                {            
                    //전체 화면을 지운다.
                    Gr->SetColor(win->BackColor);
                    Gr->FillRect(0, 0, win->width, win->height);
                    
                    //입력되었던 전체문자열을 1라인씩 당긴다.
                    for(i=0; i < col_start; i++)
                    {
                        strcpy(MemoBoxBuf[i], MemoBoxBuf[i+1]);
                    }
                    
                    //스크롤된 문자열을 화면에 출력한다.
                    sy = 0;
                    for(i=0; i < col_start; i++)
                    {
                        //Gr->TextOut(sx, sy, MemoBoxBuf[i]);          
                        //SetFontAliasing("ng_eh_24.sif");
                        TextLenAliasing(charbuf, "ng_eh_24.sif");
                        Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");
                        sy += fontSize;//gFontSize_Y[win->FontNo];
                    }
                    //스크롤이 되기 시작하면 입력되는문자열은 항상마지막라인이기 때문에 
                    col_start = colume;
                    char_count = 0;
                }            
            }
#if 0            
            if(char_count == 0) //문자입력이 안되었고 처음일경우
            {
                strcpy(charbuf, text);
                char_count = strlen(charbuf);  //입력카운트를 기록한다.
            }
            else    //한문자라도 입력이 된경우라면
            {
                strcat(charbuf, text);
                char_count += strlen(text);
            }            
            strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.            

            //배경색으로 현재라인을 지우고
            Gr->SetColor(win->BackColor);
            Gr->FillRect(sx, sy, win->width, fontSize);//gFontSize_Y[win->FontNo]);
            //문자열을 출력한다.
            //Gr->TextOut(sx, sy, charbuf);    
            SetFontAliasing("ng_eh_24.sif");
            text_width = TextLenAliasing(charbuf, "ng_eh_24.sif");
            Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");
            
#endif            
        }
    }
    else if(command==2)
    {
        char_count = 0;
        col_start = 0;
        sx = 0;
        sy = 0;
        //전체 화면을 지운다.
        Gr->SetColor(win->BackColor);
        Gr->FillRect(0, 0, win->width, win->height);        
    }
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	MemoBoxCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
	Gr->SetColor(win->BackColor);
	Gr->FillRect(0, 0, win->width, win->height);
    
	Gr->Draw(parent, handle, command, param, Data);	
	PaintDrawControl(ON);    
	return(1);
}


U32 MemoBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		backcolor, 
			int 	font_no,
			int		fontcolor
)
{
	//PARAM	parameter;    
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "MemoBox");
    
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	wid_struct.FontNo = font_no;

	wid_struct.FontGap = 0;
	wid_struct.FontSpaceGap =0;	

	//parameter = MAKELONG(style, 0);

	handle=makeWindow(
				LCD,						// Display Section Position
				"MemoBox",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				MemoBoxCreateEvent,			// CreateEvent
				MemoBoxPaintEvent,			// PaintEvent
				MemoBoxMouseOnEvent,		// MouseOnEvent
				MemoBoxMouseOffEvent,		// MouseOffEvent
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
				NULL,					    // PARAMETER
				NULL,						// Back Image
				NULL,						// 
				NULL,						// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	//Dprintf("MemoBox::id = %08x\n", id);	
	return(id);
}				



#endif

#ifndef YSI_ENABLE
char charbuf[LAN_RXBUF_SIZE];
int	MemoBoxPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");
    static int      sx = 0;
    static int      sy = 0;
    //int             style;
    int             i, len;
    //static char     charbuf[100];                           //문자표시시 들어온데이터를 통합하는 버퍼이다.
    static int      char_count=0;                           //문자표시시 입력된문자의 수이다.
    int fontSize = 24;
    
    #ifdef DATALOG_7INCH
    int  colume = (win->height / fontSize)-1;   //현재윈도우에서 몇행까지 문자가 입력되는지를 계산한다.
    #endif
    #ifdef DATALOG_8INCH
    int  colume = (win->height / fontSize);
    #endif
    char        *text = (char *)Data;                       //문자열을 표시할때는 
    static int  col_start=0;                                //현재표시하고있는 행번호이다.
    char    onechar = (char )param;                         //한문자를 표시할때 사용하는 변수이다.
    
    
    for(int i=0; i<LAN_RXBUF_SIZE; i++) {
        charbuf[i]=0;
    }
    //int text_width;
	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);	
    //Gr->SetFont(win->FontNo);
    //Gr->SetFontColor(win->FontColor);
    //Gr->SetFontSpaceGap(win->FontSpaceGap);
    //Gr->SetFontGap(win->FontGap);    
    /*
    //프롬프트 문자를 입력한다.
    if(char_count==0)
    {
        strcpy(charbuf, win->Text);
        char_count = strlen(charbuf);
    }
    */
    SetFontAliasing("ng_eh_24.sif");
    if(command==1)  //한문자씩입력하는게 보여져야한다.
    {
        if(onechar != '\n')    //표시할문자가 입력되면
        {
            charbuf[char_count++]= onechar;
            charbuf[char_count] = 0;
            
            strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.
            
            //배경색으로 현재라인을 지우고
            Gr->SetColor(win->BackColor);
            Gr->FillRect(sx, sy, win->width, fontSize);//gFontSize_Y[win->FontNo]);
            //문자열을 출력한다.
            //Gr->TextOut(sx, sy, charbuf);  

            TextLenAliasing(MemoBoxBuf[col_start], "ng_eh_24.sif");
            Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");
        }
        else        //행을바꾸는 코드가 입력되면
        {
            if(char_count == 0) //입력된문자 없이 개행코드만 입력된경우는 빈문자열(프롬프트)을 넣는다.
            {
                strcpy(MemoBoxBuf[col_start], "    ");
            }
            
            if(sy+fontSize < win->height) //행을 내리는데 화면의 행에 여유가 잇을경우
            {
                col_start++;                    //버퍼의 행을 증가시킨다.
                sy += fontSize; //다음문자의 위치를 1칸 아래로 내린다.
                char_count = 0;                 //다시문자를 처음부터 받기위해 0 으로
                
                /*
                //프롬프트 를 출력한다.
                strcpy(charbuf, win->Text);
                char_count = strlen(charbuf);
                strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.            
                //배경색으로 현재라인을 지우고
                Gr->SetColor(win->BackColor);
                Gr->FillRect(sx, sy, win->width, gFontSize_Y[win->FontNo]);
                //문자열을 출력한다.
                Gr->TextOut(sx, sy, charbuf);                  
                */
            }
            else    //행이 꽉차서 스크롤을 행하여야 할경우
            {            
                //전체 화면을 지운다.
                Gr->SetColor(win->BackColor);
                Gr->FillRect(0, 0, win->width, win->height);
                
                //입력되었던 전체문자열을 1라인씩 당긴다.
                for(i=0; i < colume; i++)
                {
                    strcpy(MemoBoxBuf[i], MemoBoxBuf[i+1]);
                }
                
                //스크롤된 문자열을 화면에 출력한다.
                sy = 0;
                for(i=0; i < colume; i++)
                {
                    //Gr->TextOut(sx, sy, MemoBoxBuf[i]);          
                    
                    //SetFontAliasing("ng_eh_24.sif");
                    TextLenAliasing(MemoBoxBuf[i], "ng_eh_24.sif");
                    Gr->TextOutAliasing(sx, sy, MemoBoxBuf[i], clYellow, 0, "ng_eh_24.sif");

                    sy += fontSize;//gFontSize_Y[win->FontNo];
                }
                //스크롤이 되기 시작하면 입력되는문자열은 항상마지막라인이기 때문에 
                col_start = colume;
                char_count = 0;
            }            
        }
    }
    else if(command==0)   //문자열을 표시한다.
    {
        if(text != NULL)
        {
            len = strlen(text);
            if(text[len-1] != '\n')
            {
                if(char_count == 0) //문자입력이 안되었고 처음일경우
                {
                    strcpy(charbuf, text);
                    char_count = strlen(charbuf);  //입력카운트를 기록한다.
                }
                else    //한문자라도 입력이 된경우라면
                {
                    strcat(charbuf, text);
                    char_count += strlen(text);
                }            
                strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.            

                //배경색으로 현재라인을 지우고
                Gr->SetColor(win->BackColor);
                Gr->FillRect(sx, sy, win->width, fontSize);//gFontSize_Y[win->FontNo]);
                //문자열을 출력한다.
                //Gr->TextOut(sx, sy, charbuf);    
                //SetFontAliasing("ng_eh_24.sif");
                TextLenAliasing(charbuf, "ng_eh_24.sif");
                Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");
            }
            else
            {
                text[len-1] = 0;

                if(char_count == 0) //문자입력이 안되었고 처음일경우
                {
                    strcpy(charbuf, text); //현재화면에 표시할 내용을 저장시켜둔다.            
                    strcpy(MemoBoxBuf[col_start], text); //현재화면에 표시할 내용을 저장시켜둔다.            
                }
                else    //한문자라도 입력이 된경우라면
                {
                    strcat(charbuf, text);
                    char_count += strlen(text);
                    strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.            
                }
                
                if(sy+fontSize < win->height) //행을 내리는데 화면의 행에 여유가 잇을경우
                {
                    //배경색으로 현재라인을 지우고
                    Gr->SetColor(win->BackColor);
                    Gr->FillRect(sx, sy, win->width, fontSize);
                    //문자열을 출력한다.
                    //Gr->TextOut(sx, sy, charbuf);  
                    //SetFontAliasing("ng_eh_24.sif");
                    TextLenAliasing(charbuf, "ng_eh_24.sif");
                    Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");

                    col_start++;                    //버퍼의 행을 증가시킨다.
                    sy += fontSize;//gFontSize_Y[win->FontNo]; //다음문자의 위치를 1칸 아래로 내린다.
                    char_count = 0;                 //다시문자를 처음부터 받기위해 0 으로                    
                }
                else    //행이 꽉차서 스크롤을 행하여야 할경우
                {            
                    //전체 화면을 지운다.
                    Gr->SetColor(win->BackColor);
                    Gr->FillRect(0, 0, win->width, win->height);
                    
                    //입력되었던 전체문자열을 1라인씩 당긴다.
                    for(i=0; i < col_start; i++)
                    {
                        strcpy(MemoBoxBuf[i], MemoBoxBuf[i+1]);
                    }
                    
                    //스크롤된 문자열을 화면에 출력한다.
                    sy = 0;
                    for(i=0; i < col_start; i++)
                    {
                        //Gr->TextOut(sx, sy, MemoBoxBuf[i]);          
                        //SetFontAliasing("ng_eh_24.sif");
                        TextLenAliasing(charbuf, "ng_eh_24.sif");
                        Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");
                        sy += fontSize;//gFontSize_Y[win->FontNo];
                    }
                    //스크롤이 되기 시작하면 입력되는문자열은 항상마지막라인이기 때문에 
                    col_start = colume;
                    char_count = 0;
                }            
            }
#if 0            
            if(char_count == 0) //문자입력이 안되었고 처음일경우
            {
                strcpy(charbuf, text);
                char_count = strlen(charbuf);  //입력카운트를 기록한다.
            }
            else    //한문자라도 입력이 된경우라면
            {
                strcat(charbuf, text);
                char_count += strlen(text);
            }            
            strcpy(MemoBoxBuf[col_start], charbuf); //현재화면에 표시할 내용을 저장시켜둔다.            

            //배경색으로 현재라인을 지우고
            Gr->SetColor(win->BackColor);
            Gr->FillRect(sx, sy, win->width, fontSize);//gFontSize_Y[win->FontNo]);
            //문자열을 출력한다.
            //Gr->TextOut(sx, sy, charbuf);    
            SetFontAliasing("ng_eh_24.sif");
            text_width = TextLenAliasing(charbuf, "ng_eh_24.sif");
            Gr->TextOutAliasing(sx, sy, charbuf, clYellow, 0, "ng_eh_24.sif");
            
#endif            
        }
    }
    else if(command==2)
    {
        char_count = 0;
        col_start = 0;
        sx = 0;
        sy = 0;
        //전체 화면을 지운다.
        Gr->SetColor(win->BackColor);
        Gr->FillRect(0, 0, win->width, win->height);        
    }
    PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}


int	MemoBoxCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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
 	
	Gr->FillRect(0, 0, 0, 0);
	Gr->SetColor(win->BackColor);
	Gr->FillRect(0, 0, win->width, win->height);
    
	Gr->Draw(parent, handle, command, param, Data);	
	PaintDrawControl(ON);    
	return(1);
}


U32 MemoBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		backcolor, 
			int 	font_no,
			int		fontcolor
)
{
	//PARAM	parameter;    
	HANDLE		handle;
	U32			id;
	Windows		*parent_win;
	WidgetStruct wid_struct;
	parent_win = GetWindow(NULL, ContainerHandle, "MemoBox");
    
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	
	wid_struct.BackColor = backcolor;
	wid_struct.FontColor = fontcolor;
	wid_struct.FontNo = font_no;

	wid_struct.FontGap = 0;
	wid_struct.FontSpaceGap =0;	

	//parameter = MAKELONG(style, 0);

	handle=makeWindow(
				LCD,						// Display Section Position
				"MemoBox",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				MemoBoxCreateEvent,			// CreateEvent
				MemoBoxPaintEvent,			// PaintEvent
				MemoBoxMouseOnEvent,		// MouseOnEvent
				MemoBoxMouseOffEvent,		// MouseOffEvent
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
				NULL,					    // PARAMETER
				NULL,						// Back Image
				NULL,						// 
				NULL,						// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
	id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	//Dprintf("MemoBox::id = %08x\n", id);	
	return(id);
}				
#endif
#ifdef __cplusplus
}
#endif
