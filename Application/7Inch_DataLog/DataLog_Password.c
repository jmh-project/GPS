/******************************************************************************
*    	File name	: Password.c 
*		Description : TEECO Project User Program File
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: 
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
/******************************************************************************
							Constant & Macros
******************************************************************************/
#if defined(TYPE_APPROVAL_ENABLE)
/******************************************************************************
*    	File name	: PasswordKeyPad.c 
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
volatile char gPasswordKeyPad_String[128];
/******************************************************************************
							Function Prototype
******************************************************************************/

volatile static U32	k_btn1, k_btn2, k_btn3, k_btn4, k_btn5, k_btn6, k_btn7, k_btn8, k_btn9, k_btn0;
volatile static U32	k_btnplus, k_btnclr, k_btnback, k_btnent, k_btndot;
volatile static U32	k_textbox;
extern volatile char	gKeypadBuf[32];
extern volatile char   kb_temp_buf[32];
U32 STextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,		//24 는 24사이즈, 48, 64 3가지가 준비되어있다.
			char	*text,		//init Character
			int	fontcolor, 
			int	backcolor,
			int	align,		//Left, Right, Center
			int	attr,		//READ_ONLY, RW_MODE
			U32	id,
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );	
void VoicePlay(uint8_t bank);				

static int dot=0;
static int first=0;
int PasswordInit(void) {	
	for(int i=0; i<12; i++) {
		gv.password[i]='0';
	}
	return 0;
}				
static int	 KeypadClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	//    char    buf[32];
	int		len;
	
	Windows 	*win = GetWindow(parent, handle, "");
	Windows		*parent_win = FindContainerWindow(parent,  "");
	//Dprintf("PasswordKeyPad Button parent=%d Handle=%d Click = %s Mode=%d parentname=%s\n",win->Parent, win->Handle, win->NormalName, parent_win->Mode, parent_kb_temp_buf);

	if(parent_win == NULL)
	{
		Dprintf("KeypadClickEvent:: Windows Error\n");
	}
	
    //VoicePlay(TOUCH_SOUND);   	
	if(win->ID >= 0 && win->ID <=9)
	{
		if(win->ID == 0) 		strcpy((char *)kb_temp_buf,"0");
		else if(win->ID == 1) 	strcpy((char *)kb_temp_buf,"1");
		else if(win->ID == 2) 	strcpy((char *)kb_temp_buf,"2");
		else if(win->ID == 3) 	strcpy((char *)kb_temp_buf,"3");
		else if(win->ID == 4) 	strcpy((char *)kb_temp_buf,"4");
		else if(win->ID == 5) 	strcpy((char *)kb_temp_buf,"5");
		else if(win->ID == 6) 	strcpy((char *)kb_temp_buf,"6");
		else if(win->ID == 7) 	strcpy((char *)kb_temp_buf,"7");
		else if(win->ID == 8) 	strcpy((char *)kb_temp_buf,"8");
		else if(win->ID == 9) 	strcpy((char *)kb_temp_buf,"9");
		
		if(first==0)
		{
			first=1;
			strcpy((char *)gKeypadBuf, (char *)kb_temp_buf);
			strcpy((char *)gv.password, (char *)kb_temp_buf);
		}
		else
		{
			if(strlen((char *)gKeypadBuf) < 10) {
				strcat((char *)gKeypadBuf, (char *)kb_temp_buf);
				strcat((char *)gv.password, (char *)kb_temp_buf);
			}
		}
		SetWidgetText(k_textbox, (char *)gKeypadBuf);
	}
	else if(win->ID == 11) //CLEAR Key
	{
		first=0;
		dot=0;
		strcpy((char *)gKeypadBuf," ");
		SetWidgetText(k_textbox, (char *)gKeypadBuf);
		strcpy((char *)gv.password, (char *)gKeypadBuf);		
		gKeypadBuf[0] = 0;
		gv.password[0] = 0;
	}
	else if(win->ID==10) // Back Key
	{
		len = strlen((char *)gKeypadBuf);
		Dprintf("Back Key = %d\n",len);
		if(len != 0)
		{
			if(gKeypadBuf[len-1]=='.')
			{
				dot=0;
			}
			gKeypadBuf[len-1] = 0;
			SetWidgetText(k_textbox, (char *)gKeypadBuf);
			Dprintf("Back Key Data = %s\n",gKeypadBuf);
			strcpy((char *)gv.password, (char *)gKeypadBuf);
		}
	}
	else if(win->ID==12) // Dot Key
	{
		if(first==1)
		{
			if(dot==0)
			{
				dot=1;
				strcpy((char *)kb_temp_buf,".");
				strcat((char *)gKeypadBuf, (char *)kb_temp_buf);
				SetWidgetText(k_textbox, (char *)gKeypadBuf);				
			}
		}
	}
	else if(win->ID==14) //+/- Key
	{
		if(first==0)
		{
			first=1;
			strcpy((char *)kb_temp_buf,"-");			
			strcpy((char *)gKeypadBuf, (char *)kb_temp_buf);
			SetWidgetText(k_textbox, (char *)gKeypadBuf);	
		}
	}
	//엔터키를 누르면 클로즈된다.
	if(win->ID == 15)
	{
        if(first==0)
        {
           	//GetWidgetText(win->CallWidget_ID, buf);
            //SetWidgetText(parent_win->CallWidget_ID, buf);
            //Dprintf("PasswordKeyPad==0:: %s\n", buf);
        }
		else
        {
			if(parent_win->CallWidget_ID != 0)
			{
				SetWidgetText(parent_win->CallWidget_ID, (char *)gKeypadBuf);
			}
			strcpy((char *)gPasswordKeyPad_String, (char *)gKeypadBuf);
            //Dprintf("PasswordKeyPad==1:: %s===%s\n", (char *)gKeypadBuf, (char *)gPasswordKeyPad_String);
        }
		first=0;
		//Dprintf("PasswordKeyPadClose::Parent=%d handle=%d\n", 0, parent);     
		CloseWindow(NULL, parent);

		if(!strcmp(flash.password,gv.password)) {
			//Info_Window("PASSWORD","정상 처리되었습니다.");
			//DataLog_MessageBox_Window("PASSWORD", "정상 처리되었습니다.");	
			DataLog_Key_Event( gv.password_parent, gv.password_handle, gv.password_timer, gv.password_id);	
		} 
		else {
			//Info_Window("PASSWORD","잘못된 입력입니다.");
			DataLog_MessageBox_Window("PASSWORD", "잘못된 입력입니다.");
		}
	}
	return(1);
}

static int	 PasswordKeyPadCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	//char	buf[64];
	int		sx = 10;
	int		sy = 50;

	gKeypadBuf[0] = 0;
	
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	//Dprintf("PasswordKeyPadCreateEvent::Start\n");
    
	PaintDrawControl(OFF);
	
	Gr->SetColor(GRAY7);
	Gr->FillRect(0, 0, win->client_width, win->client_height);	
//	if(win->CallWidget_ID != 0)
//		GetWidgetText(win->CallWidget_ID, buf);
//	else buf[0] = 0;	
	for(int i=0; i<10; i++) {
		gv.password[i]='1';
		gKeypadBuf[i]='1';
//		buf[i]='1';
	}
//	buf[10]=0;
	gv.password[10]=0;
	gKeypadBuf[10]=0;
	//k_textbox = TextBox(handle, sx, 5, 316, 32, 2, buf, YELLOW, BLACK, RIGHT_ALIGN, RW_MODE, NULL);
	k_textbox = STextBox(handle, sx, 5, 316, 32, 24, "**********", clBlack, clWhite, RIGHT_ALIGN, READ_ONLY, 1, NULL);	//RW_MODE
	/*
	k_btn7 = Button(handle, sx, sy, 76, 77, BMP_BUTTON, "KeyButton_N7.bmp", "KeyButton_T7.bmp", "", 0, "", PUSH_BUTTON, 7, KeypadClickEvent);
	k_btn8 = Button(handle, sx+80, sy, 76, 77, BMP_BUTTON, "KeyButton_N8.bmp", "KeyButton_T8.bmp", "",  0, "", PUSH_BUTTON, 8, KeypadClickEvent);
	k_btn9 = Button(handle, sx+80+80, sy, 76, 77, BMP_BUTTON, "KeyButton_N9.bmp", "KeyButton_T9.bmp", "",  0, "", PUSH_BUTTON, 9, KeypadClickEvent);
	k_btnback = Button(handle, sx+80+80+80, sy, 76, 77, BMP_BUTTON, "KeyButton_Nback.bmp", "KeyButton_Tback.bmp", "",  0, "", PUSH_BUTTON, 10, KeypadClickEvent);

	k_btn4 = Button(handle, sx, sy+80, 76, 77, BMP_BUTTON, "KeyButton_N4.bmp", "KeyButton_T4.bmp", "",  0, "", PUSH_BUTTON, 4, KeypadClickEvent);
	k_btn5 = Button(handle, sx+80, sy+80, 76, 77, BMP_BUTTON, "KeyButton_N5.bmp", "KeyButton_T5.bmp",  "", 0, "", PUSH_BUTTON, 5, KeypadClickEvent);
	k_btn6 = Button(handle, sx+80+80, sy+80, 76, 77, BMP_BUTTON, "KeyButton_N6.bmp", "KeyButton_T6.bmp", "",  0, "", PUSH_BUTTON, 6, KeypadClickEvent);
	k_btnclr = Button(handle, sx+80+80+80, sy+80, 76, 77, BMP_BUTTON, "KeyButton_Nclr.bmp", "KeyButton_Tclr.bmp", "",  0, "", PUSH_BUTTON, 11, KeypadClickEvent);

	k_btn1 = Button(handle, sx, sy+80+80, 76, 77, BMP_BUTTON, "KeyButton_N1.bmp", "KeyButton_T1.bmp", "",  0, "", PUSH_BUTTON, 1, KeypadClickEvent);
	k_btn2 = Button(handle, sx+80, sy+80+80, 76, 77, BMP_BUTTON, "KeyButton_N2.bmp", "KeyButton_T2.bmp", "",  0, "", PUSH_BUTTON, 2, KeypadClickEvent);
	k_btn3 = Button(handle, sx+80+80, sy+80+80, 76, 77, BMP_BUTTON, "KeyButton_N3.bmp", "KeyButton_T3.bmp", "",  0, "", PUSH_BUTTON, 3, KeypadClickEvent);
	
	k_btndot = Button(handle, sx, sy+80+80+80, 76, 77, BMP_BUTTON, "KeyButton_Ndot.bmp", "KeyButton_Tdot.bmp", "",  0, "", PUSH_BUTTON, 12, KeypadClickEvent);
	k_btn0 = Button(handle, sx+80, sy+80+80+80, 76, 77, BMP_BUTTON, "KeyButton_N0.bmp", "KeyButton_T0.bmp", "",  0, "", PUSH_BUTTON, 0, KeypadClickEvent);
	k_btnplus = Button(handle, sx+80+80, sy+80+80+80, 76, 77, BMP_BUTTON, "KeyButton_Nplus.bmp", "KeyButton_Tplus.bmp", "",  0, "", PUSH_BUTTON, 14, KeypadClickEvent);
	
	k_btnent = Button(handle, sx+80+80+80, sy+80+80+80, 76, 77, BMP_BUTTON, "KeyButton_Nent.bmp", "KeyButton_Tent.bmp", "",  0, "", PUSH_BUTTON, 15, KeypadClickEvent);
	*/
    
    k_btn7 = NButton(handle, sx, sy, 76, 77, "", "7",  PUSH_BUTTON, 7, KeypadClickEvent);  
    k_btn8 = NButton(handle, sx+80, sy, 76, 77, "", "8",  PUSH_BUTTON, 8, KeypadClickEvent);
    k_btn9 = NButton(handle, sx+80+80, sy, 76, 77, "", "9",  PUSH_BUTTON, 9, KeypadClickEvent);
    k_btnback = NButton(handle, sx+80+80+80, sy, 76, 77, "", "BACK",  PUSH_BUTTON, 10, KeypadClickEvent);

    k_btn4 = NButton(handle, sx, sy+80, 76, 77, "", "4",  PUSH_BUTTON, 4, KeypadClickEvent);  
    k_btn5 = NButton(handle, sx+80, sy+80, 76, 77, "", "5",  PUSH_BUTTON, 5, KeypadClickEvent);
    k_btn6 = NButton(handle, sx+80+80, sy+80, 76, 77, "", "6",  PUSH_BUTTON, 6, KeypadClickEvent);
    k_btnclr= NButton(handle, sx+80+80+80, sy+80, 76, 77, "", "CLR",  PUSH_BUTTON, 11, KeypadClickEvent);

    k_btn1 = NButton(handle, sx, sy+80+80, 76, 77, "", "1",  PUSH_BUTTON, 1, KeypadClickEvent);  
    k_btn2 = NButton(handle, sx+80, sy+80+80, 76, 77, "", "2",  PUSH_BUTTON, 2, KeypadClickEvent);
    k_btn3 = NButton(handle, sx+80+80, sy+80+80, 76, 77, "", "3",  PUSH_BUTTON, 3, KeypadClickEvent);
    
    k_btndot = NButton(handle, sx, sy+80+80+80, 76, 77, "", ".",  PUSH_BUTTON, 12, KeypadClickEvent);  
    k_btn0 = NButton(handle, sx+80, sy+80+80+80, 76, 77, "", "0",  PUSH_BUTTON, 0, KeypadClickEvent);
    k_btnplus = NButton(handle, sx+80+80, sy+80+80+80, 76, 77, "", "+/-",  PUSH_BUTTON, 14, KeypadClickEvent);

    k_btnent = NButton(handle, sx+80+80+80, sy+80+80+80, 76, 77, "", "ENT",  PUSH_BUTTON, 15, KeypadClickEvent);
	
    //Dprintf("PasswordKeyPad:: Mode=%d parentname=%s\n",win->Mode, win->Text);
	dot=0;
	first=0;
	PaintDrawControl(ON);
	
	return(1);
}

static int	 PasswordKeyPadPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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

void PasswordKeyPad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, HANDLE timer, char *title)
{
	WidgetStruct wid_struct;	
	wid_struct.FontNo = font_no;
	gv.password_timer=timer;
	gv.password_id=id;
	gv.password_parent=parent;
	gv.password_handle=handle;
	makeWindow(
				LCD,
				"KEY_PAD",				// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				345, 415,				// Size
				SHOW,					// Create after Show
				PasswordKeyPadCreateEvent,		// CreateEvent
				PasswordKeyPadPaintEvent,		// PaintEvent
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
				GRAY7,					// PARAMETER
				NULL,					// Normal Touch Screen
				0,						// Touch Screen
				title,					// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
				
	//ShowWindow(NULL, mainHandle);
}
#ifdef __cplusplus
}
#endif
#endif


