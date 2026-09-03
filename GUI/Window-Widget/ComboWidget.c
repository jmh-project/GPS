/**
  ******************************************************************************
  * @file       ComboWidget.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월8일
  * @brief    그동안 티코는 복합위젯을 만들기가 번거로웠지만 이소스로 인해 복합위젯이 구현된다.
  *				 메모리는 128개 까지 구현해놓았다.
  *          
  * @attention
  * 이파일은 자이언텍 지적재산권이므로 타사에 소스를 제공해서는 안됩니다. 
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc.</center></h2>
  * @warning  
  * @verbatim
  *  Edit History
  * @endverbatim  
  ******************************************************************************  
  */
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"
#include "WinStyleDraw.h"

/* Private typedef -----------------------------------------------------------*/
#define	COMBO_SIZE		0x2000	//!< 8Kbyte 를 할당한다.
#define	COMBO_ADDRESS1		(0x64751A00+(COMBO_SIZE * 0))			/**< PSRAM */
#define	COMBO_ADDRESS2		(0x64751A00+(COMBO_SIZE * 1))			/**< PSRAM */
#define	COMBO_ADDRESS3		(0x64751A00+(COMBO_SIZE * 2))			/**< PSRAM */
#define	COMBO_ADDRESS4		(0x64751A00+(COMBO_SIZE * 3))			/**< PSRAM */
#define	COMBO_ADDRESS5		(0x64751A00+(COMBO_SIZE * 4))			/**< PSRAM */
#define	COMBO_ADDRESS6		(0x64751A00+(COMBO_SIZE * 5))			/**< PSRAM */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//!< 생성되는 콤보위젯의  ID를 생성하기위해 사용한다.
U32	g_ComboHandle_No=0;
//!< 컨테이너 윈도우마다 생성된 콤보윈도우의 수를 관리한다. [1] ~ [5]까지 사용, 0은 사용하지 않는다.
int 	ComboCount[6];	
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//!< 
//!< @brief	Combo 위젯용 구조체의정의
typedef struct
{
	U32	id;				/**< 할당된 콤보의 ID */
	HANDLE Parent;	/**< 이콤보가 소속되는 컨테이너 윈도우 부모핸들*/
	HANDLE Handle;	/**< 이콤보가 소속되는 컨테이너 윈도우 핸들*/
	int	ShowStatus;
	int	sx, sy;				//!< 콤보윈도우의 시작위치(부모윈도우에대한 논리좌표이다)
	U32	widget_id1;
	U32	widget_id2;
	U32	widget_id3;
	U32	Group_ID;
	
	int	(*CreateEvent)(HANDLE, HANDLE ,  int , int );
    int	(*PaintEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);	
	int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);
	int	(*MouseOffEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);
	int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);
	int	(*TimerEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);
	int	(*CloseEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);
	void   		*PreviousWindow;		
	void   		*NextWindow;	
} ComboWidget;

//!< 각각의 컨테이너 윈도우마다 콤보구조체를 하나씩 갖는다.
ComboWidget		*gpComboWidget1;
ComboWidget		*gpComboWidget2;
ComboWidget		*gpComboWidget3;
ComboWidget		*gpComboWidget4;
ComboWidget		*gpComboWidget5;

//!< 각 콤보마다 할당될 어드레스이다.
volatile U32	g_Combo1_malloc_address;
volatile U32	g_Combo2_malloc_address;
volatile U32	g_Combo3_malloc_address;
volatile U32	g_Combo4_malloc_address;
volatile U32	g_Combo5_malloc_address;

//!< 이함수는 Windows_Init() 에서 호출한다.
void ComboWidget_Init(void)
{
	//!< 콤보구조체의 메모리를 할당하고 초기화시킨다.
	gpComboWidget1 = (ComboWidget *)COMBO_ADDRESS1;	/**< CONTAINER_WINDOWS 1 용 */
	memset(gpComboWidget1, 0, sizeof(ComboWidget));
	gpComboWidget2 = (ComboWidget *)COMBO_ADDRESS2;	/**< CONTAINER_WINDOWS 2 용 */
	memset(gpComboWidget2, 0, sizeof(ComboWidget));
	gpComboWidget3 = (ComboWidget *)COMBO_ADDRESS3;	/**< CONTAINER_WINDOWS 3 용 */
	memset(gpComboWidget3, 0, sizeof(ComboWidget));
	gpComboWidget4 = (ComboWidget *)COMBO_ADDRESS4;	/**< CONTAINER_WINDOWS 4 용 */
	memset(gpComboWidget4, 0, sizeof(ComboWidget));
	gpComboWidget5 = (ComboWidget *)COMBO_ADDRESS5;	/**< CONTAINER_WINDOWS 5 용 */
	memset(gpComboWidget5, 0, sizeof(ComboWidget));	

	//!< 메모리를 할당받을 초기값을 지정해준다.
	g_Combo1_malloc_address = COMBO_ADDRESS1;
	g_Combo2_malloc_address = COMBO_ADDRESS2;
	g_Combo3_malloc_address = COMBO_ADDRESS3;
	g_Combo4_malloc_address = COMBO_ADDRESS4;
	g_Combo5_malloc_address = COMBO_ADDRESS5;

	/*
	Dprintf("%s:: \n", __FUNCTION__);
	Dprintf("\t gpComboWidget1=%08x\n", gpComboWidget1);
	Dprintf("\t gpComboWidget2=%08x\n", gpComboWidget2);
	Dprintf("\t gpComboWidget3=%08x\n", gpComboWidget3);
	Dprintf("\t gpComboWidget4=%08x\n", gpComboWidget4);
	Dprintf("\t gpComboWidget5=%08x\n", gpComboWidget5);
	*/
	
	//!< 컨테이너 윈도우마다 생성된 콤보윈도우의 수를 관리한다. [1] ~ [5]까지 사용, 0은 사용하지 않는다.
	for(int i=0; i<6; i++)
		ComboCount[i] = 0;
}

//!< 컨테이너 윈도우가 닫힐때는 반드시 호출해주어야한다.
//!< 이함수는 CloseWindow 에서 자동호출되게 추가한다.
//!< window_no::handle 을 가지고들어간다.
void Free_Combo(int window_no)
{	
	switch (window_no)
	{
		case 1:	 g_Combo1_malloc_address = COMBO_ADDRESS1; ComboCount[window_no] = 0; memset((ComboWidget*)g_Combo1_malloc_address, 0, sizeof(ComboWidget));	break;
		case 2:	 g_Combo2_malloc_address = COMBO_ADDRESS2; ComboCount[window_no] = 0; memset((ComboWidget*)g_Combo2_malloc_address, 0, sizeof(ComboWidget));	break;
		case 3:	 g_Combo3_malloc_address = COMBO_ADDRESS3; ComboCount[window_no] = 0; memset((ComboWidget*)g_Combo3_malloc_address, 0, sizeof(ComboWidget));	break;
		case 4:	 g_Combo4_malloc_address = COMBO_ADDRESS4; ComboCount[window_no] = 0; memset((ComboWidget*)g_Combo4_malloc_address, 0, sizeof(ComboWidget));	break;
		case 5:	 g_Combo5_malloc_address = COMBO_ADDRESS5; ComboCount[window_no] = 0; memset((ComboWidget*)g_Combo5_malloc_address, 0, sizeof(ComboWidget));	break;
	}
}
//!<			ComboWidget *combo = (ComboWidget *)Malloc_Combo(ParentHandle, sizeof(ComboWidget));
//!< 이함수에는 문제가 있는데 할당을 무한대로 받을경우 시스템이 다운된다.(수정함)
U32	Malloc_Combo(int window_no, size_t size)
{
	U32	address=0;
	if(window_no==1) 			address=(g_Combo1_malloc_address += size); 			
	else if(window_no==2) 	address=(g_Combo2_malloc_address += size); 			
	else if(window_no==3) 	address=(g_Combo3_malloc_address += size); 			
	else if(window_no==4) 	address=(g_Combo4_malloc_address += size); 			
	else if(window_no==5) 	address=(g_Combo5_malloc_address += size); 
	else Dprintf("%s:: Windows No Error =%d\n", __FUNCTION__, window_no);
	///
	if(g_Combo1_malloc_address >= COMBO_ADDRESS2) { Dprintf("%s :: Over Combo1 Error\n", __FUNCTION__);  address=0; }
	if(g_Combo2_malloc_address >= COMBO_ADDRESS3) { Dprintf("%s :: Over Combo2  Error\n", __FUNCTION__);  address=0; }
	if(g_Combo3_malloc_address >= COMBO_ADDRESS4) { Dprintf("%s :: Over Combo3  Error\n", __FUNCTION__);  address=0; }
	if(g_Combo4_malloc_address >= COMBO_ADDRESS5) { Dprintf("%s :: Over Combo4  Error\n", __FUNCTION__);  address=0; }
	if(g_Combo5_malloc_address >= COMBO_ADDRESS6) { Dprintf("%s :: Over Combo5  Error\n", __FUNCTION__);  address=0; }
	return(address);	
}

/**
	@brief 콤보구조체중에서 NextWindow가 NULL 인 체인을 찾는다.
*/
ComboWidget *FindNullComboChain(int win_no)
{
	int		count=0;
	ComboWidget *win=NULL;

	//Dprintf("%s:: win_no=%d %d\n", __FUNCTION__, win_no);
	switch(win_no)
	{
		case 1: 	win = gpComboWidget1;	break;
		case 2: 	win = gpComboWidget2;	break;
		case 3: 	win = gpComboWidget3;	break;
		case 4: 	win = gpComboWidget4;	break;
		case 5: 	win = gpComboWidget5;	break; //
		default:	Dprintf("%s::Windows_no mistyping = %d\n", __FUNCTION__, win); return(0);
	}

	//Dprintf("%s:: win->NextWindow = %08x\n", __FUNCTION__, win->NextWindow);
	int Done=1;
	while(Done)
	{
		if(win->NextWindow ==NULL) return(win);
		else
		{
			win = (ComboWidget *)win->NextWindow;
			count++;
			if(count > 128) return(NULL);
		}
	}
	return(0);
}

/**
	@brief 콤보윈도우 구조체에서 해당되는 구조체를 찾는다.
	@parameter  Parent : 컨테이너 윈도우의 핸들이다.
	@parameter  handle : 콤보윈도우의  핸들이다.
*/
ComboWidget *FindComboChain(HANDLE parent, HANDLE handle)
{
	ComboWidget *win=NULL;

	switch(parent)
	{
		case 1: 	win = gpComboWidget1;	break;
		case 2: 	win = gpComboWidget2;	break;
		case 3: 	win = gpComboWidget3;	break;
		case 4: 	win = gpComboWidget4;	break;
		case 5: 	win = gpComboWidget5;	break; //
		default:	Dprintf("%s::Windows_no mistyping = %d\n", __FUNCTION__, win); return(0);
	}

	if(win== NULL) 
	{
		Dprintf("%s::Invalid Handle\n", __FUNCTION__);
		return (NULL);
	}
	while(1)
	{
		if(win->Handle==handle) return(win);
		if(win->NextWindow ==NULL) return(NULL);
		win = (ComboWidget *)win->NextWindow;
	}
}


U32	makeCombo(
		HANDLE ParentHandle,
		int sx, int sy,
		int	(*CreateEvent)(HANDLE, HANDLE, int, int),
		int	(*PaintEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	
		int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),
		int	(*MouseOffEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),
		int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),
		int	(*TimerEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),
		int	(*CloseEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *)
		)
{
	//Dprintf("%s::Handle = %d\n", __FUNCTION__, ParentHandle);
	//!< 콤보위젯을 생성하기위해 메모리를 할당받는다.
	ComboWidget *combo = (ComboWidget *)Malloc_Combo(ParentHandle, sizeof(ComboWidget));		//!< handle 은 컨테이너의 핸들이다.
	memset(combo, 0, sizeof(ComboWidget));	

	//Dprintf("%s:: \n", __FUNCTION__);
	//Dprintf("\t Malloc combo=%08x\n", combo);	
	
	//부모윈도우의 콤보위젯중 마지막위젯을 찾는다.
	ComboWidget *LastWindow = FindNullComboChain(ParentHandle);
	//Dprintf("\t Last combo=%08x\n", LastWindow);	
	
	if(combo != LastWindow)//처음만들어지는 콤보가 아니라면 그전콤보의 다음을 지금것으로 연결			
	{
		LastWindow->NextWindow = combo;
		combo->PreviousWindow = LastWindow; //지금만들어진 윈도우의 전 윈도우를 등록한다.
	}
	else	//처음만들어지는 경우일때는
	{
		combo->PreviousWindow = NULL;
	}		
	combo->NextWindow = (void *)NULL;	//지금만들어진 윈도우의 다음은 없으므로 NULL 지정	
	
	combo->Parent = ParentHandle;
	combo->Handle = g_ComboHandle_No++;	
	combo->sx = sx;
	combo->sy = sy;
	combo->CreateEvent = CreateEvent;
	combo->PaintEvent = PaintEvent;
	combo->MouseOnEvent = MouseOnEvent;
	combo->MouseOffEvent = MouseOffEvent;
	combo->ClickEvent = ClickEvent;
	combo->TimerEvent = TimerEvent;
	combo->CloseEvent = CloseEvent;
	combo->ShowStatus = SHOW;
	ComboCount[ParentHandle]++;
	//!< 생성될때 CreateEvent 는 호출한다.
	if(combo->CreateEvent != NULL) combo->CreateEvent(ParentHandle, combo->Handle, sx, sy);
	return(combo->Handle);	
}

int ComboFunctionCall(HANDLE handle, int function)
{
	ComboWidget	*combo;
	
	//Dprintf("%s::handle=%d  ComboCount[handle]=%d\n", __FUNCTION__, handle, ComboCount[handle]);	
	switch(handle)
	{
		case 1: 	combo = gpComboWidget1;	break;
		case 2: 	combo = gpComboWidget2;	break;
		case 3: 	combo = gpComboWidget3;	break;
		case 4: 	combo = gpComboWidget4;	break;
		case 5: 	combo = gpComboWidget5;	break; //
		default:	Dprintf("%s::Windows_no mistyping\n", __FUNCTION__); return(0);
	}	
	if(ComboCount[handle] != 0) //해당컨테이너 의 콤보위젯이 존재하면
	{
		while(1)
		{
			//!< 함수를 호출한다.
			if(function == WM_CREATE)	
			{
				if(combo->CreateEvent != NULL) combo->CreateEvent(handle, combo->Handle, combo->sx,  combo->sy);
			}
			if(function == WM_CLOSE)	
			{
				if(combo->CloseEvent != NULL) combo->CloseEvent(0, handle, 0, 0, 0);
			}
			if(function == WM_PAINT)	
			{
				if(combo->PaintEvent != NULL) combo->PaintEvent(0, handle, 0, 0, 0);
			}
			if(function == WM_CLICK)	
			{
				if(combo->ClickEvent != NULL) combo->ClickEvent(0, handle, 0, 0, 0);
			}
			if(function == WM_MOUSEON)	
			{
				if(combo->MouseOnEvent != NULL) combo->MouseOnEvent(0, handle, 0, 0, 0);
			}
			if(function == WM_MOUSEOFF)	
			{
				if(combo->MouseOffEvent != NULL) combo->MouseOffEvent(0, handle, 0, 0, 0);
			}
			if(function == WM_TIMER)	
			{
				if(combo->TimerEvent != NULL) combo->TimerEvent(0, handle, 0, 0, 0);
			}
			
			if(combo->NextWindow == NULL) break;
			combo = (ComboWidget *)combo->NextWindow;
		}				
	}
	return(1);
}

/// 여기서부터는 콤보를 구성하는 에제이다.

int Combo_UpDown_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;
	char	buf[32];
	
	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	
	int group_id = LOWORD(win->ID);
	int key_id = HIWORD(win->ID);
	
	HANDLE pa, ha;
	pa = (group_id >> 8) & 0x00ff;
	ha = group_id  & 0x00ff;
	
	ComboWidget *combo = FindComboChain(pa, ha);
	
	//Dprintf("GroupID = %04x KeyID = %04x ComboID = %08x\n", group_id, key_id, combo->Group_ID);
	//!< 해당윈도우마다 모두찾아서 호출해야한다.
	
	g_Timer_msec=0;
	Dprintf("%s Start Time=%d\n", __FUNCTION__, g_Timer_msec);
	if(key_id==1)
	{
		if(GetWidgetText(combo->widget_id1, buf))
		{
			float	no = atof(buf);
			no += 0.1f;
			sprintf(buf,"%5.1f", no);
			SetWidgetText(combo->widget_id1, buf);
		}
	}
	Dprintf("%s End Time=%d\n", __FUNCTION__, g_Timer_msec);
	return(1);
}
//!< Parent : 컨테이너 윈도우의 핸들이다.
//!< handle : 콤보윈도우의  핸들이다.
int Combo_UpDown_CreateEvent(HANDLE Parent, HANDLE handle, int sx, int sy)
{	
	ComboWidget * combo = FindComboChain(Parent, handle);
	if(combo == NULL)
	{
		Dprintf("%s->%s Combo Object Err \n", __FILE__, __FUNCTION__);
		return(0);
	}
	//Dprintf("%s:Entry -> handle=%08x sx=%d sy=%d\n", __FUNCTION__, handle, sx, sy);
	//!< MAKELONG(1, 2) : Key ID=1, Group ID=2
	Panel(Parent, sx, sy, 300, 120, OUT_EDGE, "");
	combo->widget_id1 = TextBox(Parent, sx+5, sy+5, 120, 32, 1, "100.0", clWhite, clBlack, RIGHT_ALIGN, NULL, NULL);		
	
	//!< 그룹아이디를 생성해서 구조체에 대입해줘야한다.
	uint32_t	group;
	group = ((Parent << 8) & 0xff00) |  handle;
	combo->Group_ID = group;
	
	combo->widget_id2 = SystemButton(Parent, sx+5, sy+45, 126, 60, "UP",  MAKELONG(group, 1), Combo_UpDown_KeyEvent);	
	combo->widget_id3 = SystemButton(Parent, sx+5+126+5, sy+45, 126, 60, "DOWN",  MAKELONG(group, 2), Combo_UpDown_KeyEvent);	
	Button_SetIcon(combo->widget_id2, "48G_Up_Icon1.bmp", CHANGE);	
	Button_SetIcon(combo->widget_id3, "48G_Down_Icon1.bmp", CHANGE);	
	return(1);
}

int Combo_UpDown_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
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

int Combo_UpDown(HANDLE ContainerHandle, int sx, int sy)
{
	U32		w_id;
	HANDLE	handle;
	//Dprintf("%s::Entry Handle=%d\n", __FUNCTION__, ContainerHandle);
	
	handle = makeCombo(
		ContainerHandle,
		sx, sy,
		Combo_UpDown_CreateEvent,
		NULL,
		NULL,
		NULL,
		Combo_UpDown_KeyEvent,
		NULL,
		Combo_UpDown_CloseEvent
		);	
	w_id = MAKELONG(ContainerHandle, handle);		//!< LOWORD->ContainerHandle, HIWORD->handle
	//Dprintf("%s::End ID=%08x\n", __FUNCTION__, w_id);
	return(w_id);	
}
#ifdef __cplusplus
}
#endif
