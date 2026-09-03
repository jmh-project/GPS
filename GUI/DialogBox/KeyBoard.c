/******************************************************************************
*    	File name	: KeyBoard.c 
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
#define     KB_A_ID     0
#define     KB_B_ID     1
#define     KB_C_ID     2
#define     KB_D_ID     3
#define     KB_E_ID     4
#define     KB_F_ID     5
#define     KB_G_ID     6
#define     KB_H_ID     7
#define     KB_I_ID     8
#define     KB_J_ID     9
#define     KB_K_ID     10
#define     KB_L_ID     11
#define     KB_M_ID     12
#define     KB_N_ID     13
#define     KB_O_ID     14
#define     KB_P_ID     15
#define     KB_Q_ID     16
#define     KB_R_ID     17
#define     KB_S_ID     18
#define     KB_T_ID     19
#define     KB_U_ID     20
#define     KB_V_ID     21
#define     KB_W_ID     22
#define     KB_X_ID     23
#define     KB_Y_ID     24
#define     KB_Z_ID     25

#define     KB_BS_ID    26
#define     KB_CAPS_ID  27
#define     KB_SP_ID    28
#define     KB_ENT_ID   29
#define     KB_CLOSE_ID 30

#define     KB_0_ID     31
#define     KB_1_ID     32
#define     KB_2_ID     33
#define     KB_3_ID     34
#define     KB_4_ID     35
#define     KB_5_ID     36
#define     KB_6_ID     37
#define     KB_7_ID     38
#define     KB_8_ID     39
#define     KB_9_ID     40

#define     KB_DOT_ID   41
#define     KB_MINUS_ID 42

#define     KEYBOARD_FONT   2

#define     KEYBOARD_NORMAL_SCREEN      USER_SCREEN17
#define     KEYBOARD_TOUCH_SCREEN       USER_SCREEN18

#define     KEYBOARD_WIDTH      644
#define     KEYBOARD_HEIGHT     394


#define		MAX_INPUT_DIGIT		20
/******************************************************************************
							Private & Local Variables
******************************************************************************/
volatile int KeyBoard_Caps_Flag=OFF;
extern volatile char	gKeypadBuf[32];
/******************************************************************************
							Function Prototype
******************************************************************************/
int	 KeyBoardPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data);
U32 STransTextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		align,		//Left, Right, Center
			int		attr,		//READ_ONLY, RW_MODE
			int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );


volatile U32	kb_btn_id[43];
volatile U32	kb_textbox;
volatile char   kb_temp_buf[32];
int	 KeyBoardClickEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//char	buf[32];
	//static int first=0;  
    int         i, len;
	Windows 	*win = GetWindow(parent, handle, "");
	Windows		*parent_win = FindContainerWindow(parent,  "");
	Graphics 	*Gr;
	
    //Dprintf("KeyPad Button parent=%d Handle=%d \n",win->Parent, win->Handle);

	if(parent_win == NULL)
	{
		Dprintf("KeypadClickEvent:: Windows Error\n");
	}

    if(win->ID != KB_BS_ID && win->ID != KB_CAPS_ID && win->ID != KB_ENT_ID && win->ID != KB_CLOSE_ID)
    {
        if(KeyBoard_Caps_Flag==OFF)
        {
            if(win->ID == KB_A_ID) strcpy((char *)kb_temp_buf,"A");
            else if(win->ID == KB_B_ID) strcpy((char *)kb_temp_buf,"B");
            else if(win->ID == KB_C_ID) strcpy((char *)kb_temp_buf,"C");
            else if(win->ID == KB_D_ID) strcpy((char *)kb_temp_buf,"D");
            else if(win->ID == KB_E_ID) strcpy((char *)kb_temp_buf,"E");
            else if(win->ID == KB_F_ID) strcpy((char *)kb_temp_buf,"F");
            else if(win->ID == KB_G_ID) strcpy((char *)kb_temp_buf,"G");
            else if(win->ID == KB_H_ID) strcpy((char *)kb_temp_buf,"H");
            else if(win->ID == KB_I_ID) strcpy((char *)kb_temp_buf,"I");
            else if(win->ID == KB_J_ID) strcpy((char *)kb_temp_buf,"J");
            else if(win->ID == KB_K_ID) strcpy((char *)kb_temp_buf,"K");
            else if(win->ID == KB_L_ID) strcpy((char *)kb_temp_buf,"L");
            else if(win->ID == KB_M_ID) strcpy((char *)kb_temp_buf,"M");
            else if(win->ID == KB_N_ID) strcpy((char *)kb_temp_buf,"N");
            else if(win->ID == KB_O_ID) strcpy((char *)kb_temp_buf,"O");
            else if(win->ID == KB_P_ID) strcpy((char *)kb_temp_buf,"P");
            else if(win->ID == KB_Q_ID) strcpy((char *)kb_temp_buf,"Q");
            else if(win->ID == KB_R_ID) strcpy((char *)kb_temp_buf,"R");
            else if(win->ID == KB_S_ID) strcpy((char *)kb_temp_buf,"S");
            else if(win->ID == KB_T_ID) strcpy((char *)kb_temp_buf,"T");
            else if(win->ID == KB_U_ID) strcpy((char *)kb_temp_buf,"U");
            else if(win->ID == KB_V_ID) strcpy((char *)kb_temp_buf,"V");
            else if(win->ID == KB_W_ID) strcpy((char *)kb_temp_buf,"W");
            else if(win->ID == KB_X_ID) strcpy((char *)kb_temp_buf,"X");
            else if(win->ID == KB_Y_ID) strcpy((char *)kb_temp_buf,"Y");
            else if(win->ID == KB_Z_ID) strcpy((char *)kb_temp_buf,"Z");
        }
        else
        {
            if(win->ID == KB_A_ID) strcpy((char *)kb_temp_buf,"a");
            else if(win->ID == KB_B_ID) strcpy((char *)kb_temp_buf,"b");
            else if(win->ID == KB_C_ID) strcpy((char *)kb_temp_buf,"c");
            else if(win->ID == KB_D_ID) strcpy((char *)kb_temp_buf,"d");
            else if(win->ID == KB_E_ID) strcpy((char *)kb_temp_buf,"e");
            else if(win->ID == KB_F_ID) strcpy((char *)kb_temp_buf,"f");
            else if(win->ID == KB_G_ID) strcpy((char *)kb_temp_buf,"g");
            else if(win->ID == KB_H_ID) strcpy((char *)kb_temp_buf,"h");
            else if(win->ID == KB_I_ID) strcpy((char *)kb_temp_buf,"i");
            else if(win->ID == KB_J_ID) strcpy((char *)kb_temp_buf,"j");
            else if(win->ID == KB_K_ID) strcpy((char *)kb_temp_buf,"k");
            else if(win->ID == KB_L_ID) strcpy((char *)kb_temp_buf,"l");
            else if(win->ID == KB_M_ID) strcpy((char *)kb_temp_buf,"m");
            else if(win->ID == KB_N_ID) strcpy((char *)kb_temp_buf,"n");
            else if(win->ID == KB_O_ID) strcpy((char *)kb_temp_buf,"o");
            else if(win->ID == KB_P_ID) strcpy((char *)kb_temp_buf,"p");
            else if(win->ID == KB_Q_ID) strcpy((char *)kb_temp_buf,"q");
            else if(win->ID == KB_R_ID) strcpy((char *)kb_temp_buf,"r");
            else if(win->ID == KB_S_ID) strcpy((char *)kb_temp_buf,"s");
            else if(win->ID == KB_T_ID) strcpy((char *)kb_temp_buf,"t");
            else if(win->ID == KB_U_ID) strcpy((char *)kb_temp_buf,"u");
            else if(win->ID == KB_V_ID) strcpy((char *)kb_temp_buf,"v");
            else if(win->ID == KB_W_ID) strcpy((char *)kb_temp_buf,"w");
            else if(win->ID == KB_X_ID) strcpy((char *)kb_temp_buf,"x");
            else if(win->ID == KB_Y_ID) strcpy((char *)kb_temp_buf,"y");
            else if(win->ID == KB_Z_ID) strcpy((char *)kb_temp_buf,"z");
        }
        
        if(win->ID == KB_SP_ID) 
        {
            strcpy((char *)kb_temp_buf," ");
        }
        else if(win->ID == KB_0_ID) strcpy((char *)kb_temp_buf,"0");
        else if(win->ID == KB_1_ID) strcpy((char *)kb_temp_buf,"1");
        else if(win->ID == KB_2_ID) strcpy((char *)kb_temp_buf,"2");
        else if(win->ID == KB_3_ID) strcpy((char *)kb_temp_buf,"3");
        else if(win->ID == KB_4_ID) strcpy((char *)kb_temp_buf,"4");
        else if(win->ID == KB_5_ID) strcpy((char *)kb_temp_buf,"5");
        else if(win->ID == KB_6_ID) strcpy((char *)kb_temp_buf,"6");
        else if(win->ID == KB_7_ID) strcpy((char *)kb_temp_buf,"7");
        else if(win->ID == KB_8_ID) strcpy((char *)kb_temp_buf,"8");
        else if(win->ID == KB_9_ID) strcpy((char *)kb_temp_buf,"9");
        else if(win->ID == KB_DOT_ID) strcpy((char *)kb_temp_buf,".");
        else if(win->ID == KB_MINUS_ID) strcpy((char *)kb_temp_buf,"/");
		/*
        if(first==0)
        {
            first=1;
            strcpy((char *)gKeypadBuf, (char *)kb_temp_buf);
        }
        else
        {
            if(strlen((char *)gKeypadBuf) < MAX_INPUT_DIGIT)
                strcat((char *)gKeypadBuf, (char *)kb_temp_buf);
        }
		*/
		if(strlen((char *)gKeypadBuf) < MAX_INPUT_DIGIT) {
			strcat((char *)gKeypadBuf, (char *)kb_temp_buf);
		}
        SetWidgetText(kb_textbox, (char *)gKeypadBuf);    
    }
	if(win->ID==KB_BS_ID) // Back Key
	{
		len = strlen((char *)gKeypadBuf);
        if(len <= 0) 
        {
            len = 0;
        }
		if(len != 0)
		{
			gKeypadBuf[len-1] = 0;
			SetWidgetText(kb_textbox, (char *)gKeypadBuf);
		}
	}    
    
	if(win->ID == KB_CLOSE_ID)
	{
		CloseWindow(NULL, parent);
	}
    
	if(win->ID == KB_ENT_ID)
	{
        SetWidgetText(parent_win->CallWidget_ID, (char *)gKeypadBuf);
		CloseWindow(NULL, parent);    
    }    
    
	if(win->ID == KB_CAPS_ID)
	{
        Gr = GetGraphics(NULL, parent, CLIENT_AREA);

        PaintDrawControl(OFF);
        if(Gr==NULL)
        {
            Dprintf("KeyBoardClickEvent::GetGraphics Error\n");
            return(0);
        }        
       	Gr->FillRect(0, 0, 0, 0);
        if(KeyBoard_Caps_Flag==ON)
        {
            KeyBoard_Caps_Flag=OFF;
            BmpLoad(KEYBOARD_NORMAL_SCREEN, 0, 0, "KeyBoard_Upper_off.bmp");
            BmpLoad(KEYBOARD_TOUCH_SCREEN,  0, 0, "KeyBoard_Upper_on.bmp");  
        }
        else
        {
            KeyBoard_Caps_Flag=ON;
            BmpLoad(KEYBOARD_NORMAL_SCREEN, 0, 0, "KeyBoard_Lower_off.bmp");
            BmpLoad(KEYBOARD_TOUCH_SCREEN,  0, 0, "KeyBoard_Lower_on.bmp");              
        }
        Gr->BitBlt(KEYBOARD_NORMAL_SCREEN, 0, 0, 0, 0, KEYBOARD_WIDTH-1, KEYBOARD_HEIGHT-1);	
        
        for(i=0; i<26; i++)
        {
            WidgetRedraw(kb_btn_id[i]);
        }
        //for Test
        //Gr->BitBlt(1, 0, 0, 0, 0, 200, 200);	        
        //Gr->SetColor(YELLOW);
        //Gr->FillRect(10, 10, 30, 30);
        
        PaintDrawControl(ON);      
        
        KeyBoardPaintEvent(NULL, parent, 0, 0, NULL);        
	}    
	return(1);
}

int	 KeyBoardCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
    char    buf[64];
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
    
    KeyBoard_Caps_Flag=OFF;
    
	PaintDrawControl(OFF);

    //초기표시하는 문자는 대문자로 고정한다.
    BmpLoad(KEYBOARD_NORMAL_SCREEN, 0, 0, "KeyBoard_Upper_off.bmp");
	BmpLoad(KEYBOARD_TOUCH_SCREEN,  0, 0, "KeyBoard_Upper_on.bmp");

   	Gr->BitBlt(KEYBOARD_NORMAL_SCREEN, 0, 0, 0, 0, KEYBOARD_WIDTH-1, KEYBOARD_HEIGHT-1);	

    GetWidgetText(win->CallWidget_ID, buf);
    strcpy((char *)gKeypadBuf, buf);

    
    kb_textbox = STransTextBox(handle, 25, 22, 590, 37, 1, buf, RIGHT_ALIGN, READ_ONLY, NULL);	    
    
	kb_btn_id[KB_A_ID] =  MemoryButton(handle, 18, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_A_ID, KeyBoardClickEvent);
	kb_btn_id[KB_B_ID] =  MemoryButton(handle, 78, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_B_ID, KeyBoardClickEvent);
	kb_btn_id[KB_C_ID] =  MemoryButton(handle, 137, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_C_ID, KeyBoardClickEvent);
	kb_btn_id[KB_D_ID] =  MemoryButton(handle, 196, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_D_ID, KeyBoardClickEvent);
	kb_btn_id[KB_E_ID] =  MemoryButton(handle, 256, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_E_ID, KeyBoardClickEvent);
	kb_btn_id[KB_F_ID] =  MemoryButton(handle, 315, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_F_ID, KeyBoardClickEvent);
	kb_btn_id[KB_G_ID] =  MemoryButton(handle, 374, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_G_ID, KeyBoardClickEvent);
    kb_btn_id[KB_7_ID] =  MemoryButton(handle, 456, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_7_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_8_ID] =  MemoryButton(handle, 515, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_8_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_9_ID] =  MemoryButton(handle, 574, 75, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_9_ID, KeyBoardClickEvent);
                    
	kb_btn_id[KB_H_ID] =  MemoryButton(handle, 18, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_H_ID, KeyBoardClickEvent);
	kb_btn_id[KB_I_ID] =  MemoryButton(handle, 78, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_I_ID, KeyBoardClickEvent);
	kb_btn_id[KB_J_ID] =  MemoryButton(handle, 137, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_J_ID, KeyBoardClickEvent);
	kb_btn_id[KB_K_ID] =  MemoryButton(handle, 196, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_K_ID, KeyBoardClickEvent);
	kb_btn_id[KB_L_ID] =  MemoryButton(handle, 256, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_L_ID, KeyBoardClickEvent);
	kb_btn_id[KB_M_ID] =  MemoryButton(handle, 315, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_M_ID, KeyBoardClickEvent);
	kb_btn_id[KB_N_ID] =  MemoryButton(handle, 374, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_N_ID, KeyBoardClickEvent);
    kb_btn_id[KB_4_ID] =  MemoryButton(handle, 456, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_4_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_5_ID] =  MemoryButton(handle, 515, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_5_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_6_ID] =  MemoryButton(handle, 574, 134, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_6_ID, KeyBoardClickEvent);
                    
	kb_btn_id[KB_O_ID] =  MemoryButton(handle, 18, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_O_ID, KeyBoardClickEvent);
	kb_btn_id[KB_P_ID] =  MemoryButton(handle, 78, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_P_ID, KeyBoardClickEvent);
	kb_btn_id[KB_Q_ID] =  MemoryButton(handle, 137, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_Q_ID, KeyBoardClickEvent);
	kb_btn_id[KB_R_ID] =  MemoryButton(handle, 196, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_R_ID, KeyBoardClickEvent);
	kb_btn_id[KB_S_ID] =  MemoryButton(handle, 256, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_S_ID, KeyBoardClickEvent);
	kb_btn_id[KB_T_ID] =  MemoryButton(handle, 315, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_T_ID, KeyBoardClickEvent);
	kb_btn_id[KB_U_ID] =  MemoryButton(handle, 374, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_U_ID, KeyBoardClickEvent);
    kb_btn_id[KB_1_ID] =  MemoryButton(handle, 456, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_1_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_2_ID] =  MemoryButton(handle, 515, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_2_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_3_ID] =  MemoryButton(handle, 574, 194, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_3_ID, KeyBoardClickEvent);
                    
	kb_btn_id[KB_V_ID] =  MemoryButton(handle, 18, 253, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_V_ID, KeyBoardClickEvent);
	kb_btn_id[KB_W_ID] =  MemoryButton(handle, 78, 253, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_W_ID, KeyBoardClickEvent);
	kb_btn_id[KB_X_ID] =  MemoryButton(handle, 137, 253, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_X_ID, KeyBoardClickEvent);
	kb_btn_id[KB_Y_ID] =  MemoryButton(handle, 196, 253, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_Y_ID, KeyBoardClickEvent);
	kb_btn_id[KB_Z_ID] =  MemoryButton(handle, 256, 253, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_Z_ID, KeyBoardClickEvent);
	kb_btn_id[KB_BS_ID] =  MemoryButton(handle, 315, 253, 110, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_BS_ID, KeyBoardClickEvent);

    kb_btn_id[KB_0_ID] =  MemoryButton(handle, 456, 253, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_0_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_DOT_ID] =  MemoryButton(handle, 515, 253, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_DOT_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_MINUS_ID] =  MemoryButton(handle, 574, 253, 50, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_MINUS_ID, KeyBoardClickEvent);
                    
    kb_btn_id[KB_CAPS_ID] =  MemoryButton(handle, 19, 311, 110, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, TOGGLE_BUTTON, KB_CAPS_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_SP_ID] =  MemoryButton(handle, 137, 311, 168, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_SP_ID, KeyBoardClickEvent);	
    kb_btn_id[KB_ENT_ID] =  MemoryButton(handle, 315, 311, 110, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_ENT_ID, KeyBoardClickEvent);
    kb_btn_id[KB_CLOSE_ID] =  MemoryButton(handle, 456, 311, 168, 50, KEYBOARD_FONT, "",  CENTER_ALIGN,
					KEYBOARD_NORMAL_SCREEN, KEYBOARD_TOUCH_SCREEN, PUSH_BUTTON, KB_CLOSE_ID, KeyBoardClickEvent);
                    
	PaintDrawControl(ON);
	return(1);
}

int	 KeyBoardPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
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

int KeyBoard(U32 id, int sx, int sy, char *value, char *title)
{
	//HANDLE mainHandle;
	WidgetStruct wid_struct;

	if(GetWindowName("")) 
	{
		Dprintf("KeyBoard:: Windows already Create\n");
		return 0;		
	}
	
    //키보드윈도우는 입력윈도우이다. 이윈도우를 호출한 위젯의 아이디를 저장한다.
	wid_struct.CallWidget_ID = id;
	//초기에 표시할값은 매개변수의 char *value 이며 *NormalName에 할당된다.	
	makeWindow(
				LCD,
				"Key_Board",			// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				STYLE_NONE,				// Windows Style
				sx, sy, 				// Start Position
				KEYBOARD_WIDTH, KEYBOARD_HEIGHT,				// Size
				SHOW,					// Create after Show
				KeyBoardCreateEvent,	// CreateEvent
				KeyBoardPaintEvent,		// PaintEvent
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
				value,					// Normal Touch Screen
				0,						// Touch Screen
				title,					// Windows Text
				INPUT_WINDOW,
				&wid_struct
				);
	//ShowWindow(NULL, mainHandle);
	return(1);
}
#ifdef __cplusplus
}
#endif
