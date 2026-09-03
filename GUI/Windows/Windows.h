#ifdef __cplusplus
 extern "C" {
#endif
    
#ifndef WINDOWS_HEADER
    #define WINDOWS_HEADER

#include "Teeco_System.h"
#include "WindowsGraphics.h"
 
//=============================================================================
//          하드웨어의 정의를 여기서한다.
//=============================================================================
#define		SCREEN_SX			0
#define		SCREEN_SY			0
#define		SCREEN_WIDTH		800
#define		SCREEN_HEIGHT		600
#define		SCREEN_BITFERCOLOR	16
#define		FRAME_BUFFER		0x68000000

#define		DMA_ENABLE			1
#define		DOUBLE_BUFFER		1
#define		DOUBLE_BUFFER_AREA	1
#define		ENGINE_2D			0
#define		CLIP_ENABLE			1

#define		NO_DISPLAY			0
#define		DISPLAY				1

#define		SHOW				1
#define		HIDDEN				0

#define		MODALLESS_WINDOW	0
#define		MODAL_WINDOW		1
#define		INPUT_WINDOW		2

#define		LCD					0
#define		LCD_ALPHA			1
#define		VIDEO_ALPHA		2
#define		VIDEO			    3

#define		LCD_ALPHA_HANDLE	6
#define		VIDEO_ALPHA_HANDLE	7

//=============================================================================
//          윈도우용 C TYPE 을 정의
//=============================================================================

#define 	MAKEWORD(a,b)	(((a << 16) & 0xffff0000) | (b & 0x0000ffff))
#define 	MAKELONG(lo,hi)   ((LONG)(((WORD)((DWORD)(lo) & 0xffff)) | ((DWORD)((WORD)((DWORD)(hi) & 0xffff))) << 16))
#define 	LOWORD(l)       ((WORD)((DWORD)(l) & 0xffff))
#define 	HIWORD(l)       ((WORD)((DWORD)(l) >> 16))
//#define 	LOBYTE(w)       ((BYTE)((DWORD)(w) & 0xff))
//#define 	HIBYTE(w)       ((BYTE)((DWORD)(w) >> 8))

#define     XCENTER(x)      ((LCD_X-x) / 2)
#define     YCENTER(y)      ((LCD_Y-y) / 2)

#define		INVALID_DATA	-1
#define		CLIENT_AREA		1		// GetDC에서 어떤영역을 얻을지를 구분한다.
#define		WINDOW_AREA		2

//=============================================================================
//          위젯에 관련된 정의문
//=============================================================================
//버튼의 종류를 구분한다.
#define		BMP_BUTTON			1
#define		ICON_BUTTON			2
#define		TEXT_BUTTON			3

#define		PUSH_BUTTON			0
#define		TOGGLE_BUTTON		1
#define		SELECT_BUTTON		2

///< 위젯의 변경을 바로적용할지, 다음에 윈도우가 적용하게할지를 결정한다
#define		UPDATE		1	//!< 즉시변경한다.
#define		CHANGE	0	//!< 윈도우가 알아서 적용한다.

//버튼을 만들때 아이콘과 텍스트를 수직배치, 수평배치를 결정한다.
#define		HORIZONTAL			1
#define		VERTICAL			2

//위젯을 만들때 문자열의 정렬방법이다.
#define		RIGHT_ALIGN			1
#define		LEFT_ALIGN			2
#define		CENTER_ALIGN		3

//
#define       AND_ROP           1
#define       OR_ROP             2

#define		BASIC_STYLE			1
#define		PANEL_STYLE			2
//패널등의 윈도우의 에지모양을 결정한다.
#define		IN_EDGE				1
#define		OUT_EDGE			2

//텍스트박스관계된 속성
#define		READ_ONLY			0
#define		RW_MODE				1

//Key Pad
#define		KEYPAD_NORMAL_MODE		0
#define		KEYPAD_SEQ_MODE			1

//MemoBox
#define     PAGE_MODE           0
#define     SCROLL_MODE         1
//=============================================================================
//          윈도우사운드관련
//=============================================================================
#define		WINDOWS_SOUND1			1
#define		WINDOWS_SOUND2			2
#define		WINDOWS_SOUND3			3

//=============================================================================
//          윈도우의 종류를 구분한다.
//=============================================================================
#define		DESKTOP_WINDOW		0
#define		CONTAINER_WINDOW		1
#define		WIDGET_WINDOW		2

//=============================================================================
//          윈도우의 테두리모양을 정의한다.
//=============================================================================
typedef enum
{	
	WS_NONE=1,		//윈도우의 테두리를 아무것도 그리지 않는다.
	WS_WINDOWS_XP,	//XP 같은 타이틀바를 갖는다.
	WS_KUBUNTU,		//쿠분투같은 타이틀바를 갖는다.
} WindowStyle;

#define		STYLE_NONE		0
#define		XP_STYLE		1
#define		TEECO_STYLE		2

#define		WS_LINE_WIDTH	4
#define		WS_TITLE_HEIGHT	32+8
#define		WS_BACK_COLOR	GRAY3
//=============================================================================
//          윈도간의 통신을 위한 명령을 정의한다.
//=============================================================================
#define		WM_FUNCTION		0x0010		//0
#define		WM_CREATE		0x0001		//1
#define		WM_PAINT		0x0002		//2
#define		WM_MOVE			0x0003		//3
#define		WM_CLICK		0x0005		//4
#define		WM_DOUBLECLICK	0x0006		//5
#define		WM_MOUSEON		0x0007		//6
#define		WM_MOUSEOFF		0x0008		//7
#define		WM_MOUSEMOVE	0x0009		//8
#define		WM_GESTURE		0x000A		//9
#define		WM_HIDDEN		0x000B		//10
#define		WM_SHOW			0x000C		//11
#define		WM_TIMER		0x000D		//12
#define		WM_KEYPAD		0x000E		//13
#define		WM_ENCODER		0x000F		//14
#define		WM_CHANGE		0x0010		//15
#define		WM_CONTROL		0x0011		//16
#define		WM_CLOSE		0x0012		//17
#define		TOUCH_ON		0x0013		//17
#define		TOUCH_OFF		0x0014		//17
#define		TOUCH_MOVE		0x0015		//17
#define		TIMER_EVENT		0x0016		//17

#define		KEY_ON		    0x0017		//17
#define		KEY_OFF		    0x0018		//17
#define		KEY_ROT		    0x0019		//17

//======================================================
// WM_CHANGE 에대해 무엇이 바뀌었는지를 알수있게하는 코드
#define		TEXT_CHANGE			1
#define		FORMSIZE_CHANGE		2



//=============================================================================
//          시스템디바이스  통신을 위한 명령을 정의한다.
//=============================================================================
#define		TOUCH_DEVICE		0x1000
#define		KEYPAD_DEVICE		0x2000
#define		ENCODER_DEVICE		0x3000
#define		TIMER_DEVICE		0x4000
#define		INTERRUPT_DEVICE	0x5000


//=============================================================================
//          이벤트의 연결을 위한 함수구분정의
//=============================================================================
typedef enum
{
	MainFunction,		// 1
	CreateEvent,		// 2
	CloseEvent,			// 3
	PaintEvent,			// 4
	TimerEvent,			// 5
	ClickEvent,		   	// 6
	DoubleClickEvent,	// 7
	MouseOnEvent,	   	// 8
	MouseOffEvent,	   	// 9
	MouseMoveEvent,	   	// 10
	GestureEvent,	  	// 11
	ShowEvent,		  	// 12
	ControlEvent,	 	// 13
	MoveEvent,			// 14
	ResizeEvent,		// 15
	KeyEvent,		    // 16
	EncoderEvent,		// 17
	ChangeEvent,		// 18
	HiddenEvent			// 19
} EventType;

//=============================================================================
//       	윈도우메시지를 관리하는 구조체이다.
//=============================================================================
//     윈도우관련 구조체
typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT;

typedef struct tagMSG {
    HANDLE      Parent;
	HANDLE      Handle;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    //DWORD       time;
    POINT       pt;
} MSG;

//=============================================================================
//              윈도우를 그리기위해 DC 를 얻으면 생기는 구조체
//=============================================================================
#define		CLIENT_AREA		1
#define		WINDOW_AREA		2

typedef struct Func__
{
    int     (*SystemTimerVector)(void);
}FuncTable;

extern FuncTable stFuncTable;

typedef struct __bitmapsize
{
	int width, height;
} BitmapSize;

typedef struct __Rect
{
	int		sx, sy;
	int		ex, ey;
} Rect;

typedef struct __Handle
{
	HANDLE	Parent;
	HANDLE	Handle;
} st_Handle;

typedef struct __WinTimer
{
	HANDLE	Parent;
	HANDLE	Handle;
	int		Status;
	int		id;
	int		Interval;
	int		Count;
} WinTimer;

typedef struct __IOTimer
{
	HANDLE	Handle;
	int		Status;
	int		id;
	int		Interval;
	int		Count;
} IOTimer;


typedef struct __Font
{
	int 	Gap;
	int		SpaceGap;
	char	Font[32];
	U16		Color;
} Font;


typedef struct __WidgetStruct
{
	int 	BackColor;
	int		Position;
	int		BarColor;
	int		Style;
	int		Attr;		//속성
	
	int		Toggle;
	int		ID;
	char	IconName[32];
	int		FontNo;
	int		FontColor;
	int		FontGap;
	int		FontSpaceGap;
	U32		CallWidget_ID;
	int		TextAlignMode;    
} WidgetStruct;

//=============================================================================
//          윈도우의 속성을 정의한다.
//=============================================================================
typedef struct __Windows
{
	int			Kind;				//이 윈도우의 종류 (컨테이너, 위젯)
	HANDLE		Parent;				//이 윈도우의 부모핸들
	HANDLE		Handle;				//이 윈도우의 핸들
	int			Mode;				//MODAL_WINDOW, MODALLESS_WINDOW

	int			sx, sy;				//윈도우의 시작위치(부모윈도우에대한 논리좌표이다)
	int			width, height;		//윈도우의 크기

	int			abs_sx, abs_sy;		//그래픽 프레임버퍼에대한 물리적인 화면좌표
	int			abs_ex, abs_ey;		
    int			client_abs_sx, client_abs_sy;
    int			client_abs_ex, client_abs_ey;
    int			client_sx, client_sy;
    int			client_width, client_height;

	int			LineTo_X;			//각각의 윈도우에서 LineTo, MoveTo를 쓰는 포인터이다.
	int			LineTo_Y;	

	int			ChildCnt;			//이윈도우가 차일드를 몇개 가지고 있는지 
	int			Zorder;				//이윈도우가 부모윈도우에 대한 Z 순서(높으면 제일위에보인다)
	int			Focus;				//현재 이윈도에 포커스가 있으면 1

	int			BackColor;
	int			FontColor;
	int			FontNo;				//메모리에 저장되어 있는 테이블 번호
	int			FontGap;
	int			FontSpaceGap;
	char			IconName[32];
	int			Position;
	int			BarColor;
	int			WidgetStyle;
	int			Toggle;				
	int			ToggleStatus;
	int			ID;             //WidgetStruct
	U32			CallWidget_ID; //WidgetStruct
	int			TextAlignMode;//WidgetStruct

//	int			ShowEnable;			//부모가 보이게 되면 그려라는 명령
	int			ShowStatus;			//현재 화면에 보이고있는지 상태를 나타낸다.
	int			Style;				//Border Style: XP_STYPE
	char			NormalName[32];	//Icon 이름으로도 사용한다.
	char			TouchName[32];
	char			Text[128];
	char			Name[32];

	int			Attr;		//속성
 
	int	 		(*StyleDrawEvent)(HANDLE , HANDLE , WCOMMAND , PARAM , void *);
    int			(*MainFunction)(HANDLE, HANDLE, WCOMMAND, PARAM, void *); 		//0
	int			(*CreateEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		//1
    int			(*PaintEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);			//2
    int			(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		//3
    int			(*MouseOffEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		//4
    int			(*MouseMoveEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		//5
	int			(*GestureEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		//6
    int			(*MoveEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);			//7
    int			(*ResizeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		//8
    int			(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);			//9
    int			(*DoubleClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);	//10
    int			(*KeyEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		    //11
    int			(*EncoderEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);	 	//12
    int			(*TimerEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		 	//13
    int			(*ShowEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);     	 	//14
    int			(*HiddenEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);	 	//15
	int			(*ControlEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);	 	//16
	int			(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);	 	//17
	int			(*CloseEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);		 	//18
	int	        (*FocusEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *);	        //19
    
	PARAM		Param;
	void   		*PreviousWindow;		
	void   		*NextWindow;		
    
} Windows;

typedef struct __WindowsMag
{
	int		ContainerWin[6];		// 컨테이너 윈도우 5개중 메모리에 있는 상태 
	int		LcdAlphaContainerWin;		// 컨테이너 윈도우 5개중 메모리에 있는 상태 
	int		VideoAlphaContainerWin;		// 컨테이너 윈도우 5개중 메모리에 있는 상태 

	int 	WidgetCount[6];			// 현재생성된 컨테이너윈도우의 위젯윈도우의 수
	int 	LcdAlphaWidgetCount;			// 현재생성된 컨테이너윈도우의 위젯윈도우의 수
	int 	VideoAlphaWidgetCount;			// 현재생성된 컨테이너윈도우의 위젯윈도우의 수

	int		TimerCount;				// 현재 생성된 타이머의 수
	int		IOTimerCount;			// 현재 생성된 I/O 타이머의 수
	int		IOProcess[6];			// 생성된 프로세스수

	int		Handle;
	int 	FocusWindow;			// 현재 포커스를 받은윈도우의 부모핸들.
	int 	FocusChild;				// 현재 포커스를 받은윈도우의 위젯핸들.
	int		ModalWindow_No;			// 현재 모달로 지정된 윈도우의 번호 (1~5)
	int		InputWindow_No;			// 입력으로 설정된 윈도우의 번호(1~5)
	int		LcdAlpha_On;			// LCD ALPHA 의 상태.
	int		Video_On;				// VIDEO 의 상태
	int		VideoAlpha_On;			// VIDEO ALPHA 의 상태
	int		Input_Show;				// INPUT_WINDOW 가 보이는 상태
	int 	Modal_Show;				// MODAL_WINDOW 가 보이는상태
    int     LcdAlpha_Touch;         // 알파영역이 터치가 되는지의 여부를 결정한다.
} WindowsManager;

typedef struct __Graphics
{
	HANDLE	Parent;
	HANDLE	Handle;

	int		Offset_X;
	int		Offset_Y;
	int		Page;

	void	(*SetGraphicsPage)(int); 
	int		(*SetClip)(U16, int, int, int, int, int);	//int SetClipping(U16 enable, int left, int top, int right, int bottom);
	int 	(*LoadFont)(int, char *);
	void	(*SetFontGap)(int);
	void	(*SetFontSpaceGap)(int);
	void	(*SetColor)(U16); 							//void SetColor(U16 color); 
	void	(*SetFontColor)(U16); 							//void SetColor(U16 color); 
	void	(*SetBrush)(int); 							//void SetBrush(U16 Style); 
	void	(*SetFont)(int);  						//void SetFont(FontType fontstyle);
	void	(*SetFontStartLine)(int);  						//void SetFont(FontType fontstyle);
	void	(*SetHangulFont)(int);  						//void SetFont(FontType fontstyle);
	void	(*FontSort)(int);
	void	(*Plot)(U16, U16);							//void Plot(U16 sx, U16 sy, U16 color);
	//void 	(*BmpPlot)(U16, U16, U16); 					//void BmpPlot(U16 x, U16 y, U16 color);
	void	(*Line)(int, int, int, int);  				//void Line(U16 sx, U16 sy, U16 len, U16 color);
	void	(*MoveTo)(U16, U16); 						//void LineTo(U16 endx, U16 endy);
	void	(*LineTo)(U16, U16); 						//void LineTo(U16 endx, U16 endy);
	void	(*Arc)(U16, U16, U16, U16, U16); 			//void Arc(U16 sx, U16 sy, U16 radius, U16 start, U16 end);
	void	(*Elipse)(U16, U16, U16, U16);				//void Elipse(U16 sx, U16 sy, U16 a, U16 b);
	void	(*Rectangle)(U16, U16, U16, U16);  			//void Rectangle(U16 sx, U16, sy, U16 width, U16 height);
	void	(*RoundRect)(U16, U16, U16, U16);  			//void Rectangle(U16 sx, U16, sy, U16 width, U16 height);
	void	(*FillElipse)(U16, U16, U16, U16);			//void FillElipse(U16 sx, U16 sy, U16 a, U16 b);
	void	(*FillRect)(int, int, int, int);  			//void FillRect(U16 sx, U16, sy, U16 width, U16 height);
	void	(*HLine)(int, int, int);  					//void HLine(U16 sx, U16 sy, U16 len);
	void 	(*Hline)(U16 x1, U16 x2, U16 y);			//void Hline(U16 x1, U16 x2, U16 y)
	void	(*VLine)(int, int, int);   					//void VLine(U16 sx, U16 sy, U16 len);
	void 	(*CrossLine)(U16 x, U16 y, U16 len);		//void CrossLine(U16 x, U16 y, U16 len);
	void 	(*VLine2D)(int y1, int y2, int x);			//void VLine2D(int y1, int y2, int x);
	void 	(*HLine2D)(int x1, int x2, int y);			//void HLine2D(int x1, int x2, int y);
	void 	(*Line2D)(int x1, int y1, int x2, int y2); 			//void Line2D(int x1, int y1, int x2, int y2);
	void 	(*FillRect2D)(int xs, int ys, int xe, int ye); 		//void FillRect2D(int xs, int ys, int xe, int ye);
	void	(*BitBlt)(int, U16, U16, U16, U16, U16, U16); 	//void PaintBitBlt(int page, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy)
    void   (*FontBitBlt)(int page, int CompPage, int ref_page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy);
    void   (*FontBitBltOne)(int page, int CompPage, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy);
	void 	(*BitBltRop)(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy, int mode, U8 rop);
	void 	(*Rotate)(U32, U32, U16, U16, U16, U16, U16, U16); 	//void Rotate(U32 src, U32 dest, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy)
	int 	(*BmpDisplay)(U16, U16, char *);					//void BmpLoad(int page, U16 sx, U16 sy, char *name);
	int 	(*BmpDisplay1)(U16, U16, char *);					//void BmpLoad(int page, U16 sx, U16 sy, char *name);
	int 	(*BmpLoad)(int , U16 , U16 , char *);	
	void 	(*IconDisplay)(U16, U16, char *, U16);					//void BmpLoad(int page, U16 sx, U16 sy, char *name);
	int	    (*CTextOut)(int sx, int sy, const char *s); 	//void CTextOut(U16 sx, U16 sy, const char *s);
	int	    (*TextOut)(int sx, int sy, const char *s); 	//void TextOut(U16 sx, U16 sy, const char *s);
	int	    (*LowTextOut)(int sx, int sy, const char *s, int fontcolor, int font); 	//void TextOut(U16 sx, U16 sy, const char *s);
	int	    (*TextOutAliasing)(int sx, int sy, char *str, uint16_t color, uint16_t back_color, char *font);
    void 	(*ScreenClear)(int page);
	//void 	(*HangulOut)(int sx, int sy, const char *s);
	int 	(*Draw)(HANDLE, HANDLE, U32, U32, void *); 			//void	Draw(HANDLE parent, HANDLE handle, U32 command, U32 para, void *data);
} Graphics;

extern Windows	*gp_ContainerWindow_1;
extern Windows	*gp_ContainerWindow_2;
extern Windows	*gp_ContainerWindow_3;
extern Windows	*gp_ContainerWindow_4;
extern Windows	*gp_ContainerWindow_5;
extern Windows	*gp_LcdAlpha_ContainerWindow;
extern Windows	*gp_VideoAlpha_ContainerWindow;

extern Windows	*gp_WidgetWindow_1;			//위젯윈도우의 시작포인터
extern Windows	*gp_WidgetWindow_2;
extern Windows	*gp_WidgetWindow_3;
extern Windows	*gp_WidgetWindow_4;
extern Windows	*gp_WidgetWindow_5;
extern Windows	*gp_LcdAlphaWidgetWindow;
extern Windows	*gp_VideoAlphaWidgetWindow;

extern WindowsManager	*gp_WinManager;

extern volatile U32	g_Widget1_malloc_address;
extern volatile U32	g_Widget2_malloc_address;
extern volatile U32	g_Widget3_malloc_address;
extern volatile U32	g_Widget4_malloc_address;
extern volatile U32	g_Widget5_malloc_address;
extern volatile U32	g_LcdAlphaWidget_malloc_address;
extern volatile U32	g_VideoAlphaWidget_malloc_address;

extern Graphics		*gp_Graphics;
extern volatile u16	g_TextColor;
extern volatile int	g_FontNo;
extern volatile int g_TextStartLine;

/******************************************************************************
							Windows Function Prototype
******************************************************************************/
void 	Windows_Init(void);
U32		Malloc(int win_no, size_t size);	// Video Memory Use memory Allocation
Windows *FindContainerWindow(HANDLE handle, char *func);
Windows *FindWindowChain(HANDLE parent, HANDLE handle);
Windows *FindWidgetChain(HANDLE parent, HANDLE handle);
Windows *FindContainerChain(int handle, char *func);
int 	CloseWindow(HANDLE parent, HANDLE handle);
void 	Free(int window_no);
HANDLE 	GetWindowHandle(void);
int 	GetWindowCount(void);
int 	ConnectEventFunc(int kind, HANDLE parent, HANDLE handle, EventType event, int (*func)(HANDLE, HANDLE, WCOMMAND, PARAM, void *));
int     GetMessage(MSG *lpMsg, MSG *timerMsg, MSG *iotimerMsg);
int     DispatchMessage(MSG *lpMsg, MSG *timerMsg,  MSG *iotimerMsg, int kind);
int		CommonMSGParsing(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data);
Graphics *GetGraphics(HANDLE parent, HANDLE handle, int area);
int     GetHiddenWindowCount(char *func);
int 	PaintDraw(HANDLE parent, HANDLE handle, U32 command, U32 para, void *data);
Windows *GetZorderWindow(int zorder);
void 	WindowBitBlt(Windows *container_winow, int no);
void 	GraphicsInit(void);
int		ContainerMSGParsing(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data);
Windows *GetWindowByname(char *win_name);
Windows *GetWindowByMode(int mode);
int 	ShowWindow(HANDLE parent, HANDLE handle);
int 	HiddenWindow(HANDLE parent, HANDLE handle);
int 	ResizeWindow(HANDLE parent, HANDLE handle, int width, int height);
int 	MoveWindow(HANDLE parent, HANDLE handle, int dx, int dy);
Windows *GetWindow(HANDLE parent, HANDLE handle, char *func);
void    LcdAlphaTouchControl(int s);
Windows *GetWindowByName(char *name);
int 	makeWindow(	int screen, char *name, 					//Window Name
				HANDLE ParentHandle, int kind, 	//CONTAINER_WINDOW, WIDGET_WINDOW
				int style, 						//XP_STYLE, TEECO_STYLE, STYLE_NONE
				int sx, int sy, int width, int height, int	show,
				int	(*CreateEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		//1
			    int	(*PaintEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),			//2
			    int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		//3
			    int	(*MouseOffEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		//4
			    int	(*MouseMoveEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		//5
				int	(*GestureEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		//6
			    int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),			//7
			    int	(*TimerEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),		 	//8
				int	(*ControlEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	 	//9
				int	(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	 	//10
                int	(*KeyEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	        //11
                int	(*EncoderEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	    //12
                int	(*FocusEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	        //13
                int	(*CloseEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *),	        //14
				PARAM param,
				char	*NormalName,	//메모리버튼용
				char	*TouchName,		//메모리버튼용
				char	*text,	//속성
				int		mode,
				WidgetStruct *wid_struct
				);
int SendMessage(HANDLE src_parent, HANDLE src_handle, HANDLE dest_parent, HANDLE dest_handle, WCOMMAND message, PARAM param);
extern Windows	*thisWindow;	//이벤트를 받은 함수에서 자기자신의 윈도우정보를 바로 알기위해
								// 이벤트를 호출하기전에 여기에 대입해놓는다.
Graphics *GetGraphicsExt(HANDLE parent, HANDLE handle, int area, U32 address);

/****************************** For WinTimer *********************************/
void 		TimerProcess(void);
int 		GetTimerCount(void);
HANDLE 		CreateTimer(int interval, int (*TimerFunc)(HANDLE ,UINT, WPARAM, LPARAM));
int 		StartTimer(HANDLE handle);
int 		StopTimer(HANDLE handle);
int         SetTimerInterval(int id, int interval);
int 		CloseTimer(HANDLE handle);
void 		WinTimerInit(void);
/****************************************************************************/
void 	SystemMSGInsert(MSG *message);
extern 	volatile int gMsgWCount;
extern 	volatile int gMsgRCount;
extern 	volatile U32 gSystemTime;
extern 	volatile S16 gTouch;
extern 	MSG		*SystemMSG;			//메시지 시작포인터
extern  volatile int gWinFocus;		//현재 포커스받은 윈도우 
int 	Touch_Parsing(MSG *lpMsg);
int     KeyPad_Parsing(MSG *lpMsg);
U32 	GetGraphicsAddr(int Page_No);
int		WidgetMSGParsing(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data);
void 	WindowsTimerInit(void);
int 	TimerEvent_Parsing(MSG *lpMsg);
int 	SetWindowText(HANDLE src_parent, HANDLE src_handle, HANDLE dest_parent, HANDLE dest_handle, char *text);
void 	SetFontColor(U16 color);
void 	WinRect2D(U16 sx, U16 sy, U16 width, U16 height);
void 	WinRoundRect2D(U16 sx, U16 sy, U16 width, U16 height);
void 	IconDisplay(U16 sx, U16 sy, char *filename, U16 pass_color);
int 	PaintBmpDisplay(U16 sx, U16 sy, char *filename);
void 	PaintBitBltRop(int page, U16 sx, U16 sy, U16 dx1, U16 dy1, U16 dimx, U16 dimy, int mode, U8 rop);
void 	BitBltRop(int srcPage, int destPage, U16 sx, U16 sy, U16 dx, U16 dy, U16 dimx, U16 dimy, int mode, U8 rop);
void 	ScreenClear(int page);
int 	GetTextWidth(const char *text);
int 	GetTextHeight(int no);
void	GetImage(char *filename, BitmapSize *bmp);
extern const char Hangul_m[2400][32];
void 	HangulOut(int sx, int sy, const char *s);
void    SetFontStartLine(int start);
int WidgetDisplay(U32 id, int check); //SHOW, HIDDEN
/***************Widget Library********************************************/
int ComboFunctionCall(HANDLE handle, int function);

int Button_SetText(U32 id, char *text, int action);
int Button_SetIcon(U32 id, char *icon_name, int action);
int Button_GetText(U32 id, char *text, int max_len);

U32 MemoryButton(HANDLE ContainerHandle, int sx, int sy, int width, int height, 
			int font_no, char *text, int align, int NormalPage, int TouchPage, int function, int id, 
			int (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *));
int FileButton(HANDLE ContainerHandle, int sx, int sy, int width, int height, char *NormalName, char *TouchName, int (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *));

U32 SystemButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			char	*text,								// 버튼에 표시되는 문자열
			U32		id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
			
U32 RoundButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			char	*text,								// 버튼에 표시되는 문자열
			U32		id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );			

U32 RoundRectButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			char	*text,								// 버튼에 표시되는 문자열
			U32		id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
			
U32 Button(	HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	style,		//BMP_BUTTON, ICON_BUTTON, TEXT_BUTTON
			char 	*normal_image, 	//or Icon name
			char 	*touch_image,
			char 	*icon_image,			
			int 	font_no,
			char	*text,
			int		function,	
			U32		id,
			int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );

U32 RButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			int 	style,								// 버튼의 종류 BMP_BUTTON, ICON_BUTTON, TEXT_BUTTON
			char 	*normal_image, 						// 이미지버튼시 터치안된 이미지이름 or Icon name
			char 	*touch_image,						// 이미지버튼시 터치된 이미지이름
			char 	*icon_image,						// 
			int 	font_no,							// 버튼에 사용될 폰트번호
			char	*text,								// 버튼에 표시되는 문자열
			int		function,							// 토글버튼, 푸쉬버튼의 구분
			int		id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
U32 PowerButton(	HANDLE ContainerHandle, 					// 이버튼이 소속되는 콘테이너 윈도우의 핸들
			int 	sx, int sy, int width, int height, 	// 버튼의 시작위치와 사이즈
			char	*text,								// 버튼에 표시되는 문자열
			U32		id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
            
U32 TextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		backcolor, 
			int 	fontcolor,
			int		align,		//Left, Right, Center
			int		attr,		//READ_ONLY, RW_MODE			
			int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );			

U32 RadioButton(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		font_no,
			char	*text,		//init Character
			int		backcolor, 
			int		fontcolor,
			int 	check,
            int     call_id,
			int		(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
			
U32 CheckBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	style,
			int		font_no,
			char	*text,		//init Character
			int		backcolor, 
			int		fontcolor,
			int 	check,
			int		(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );			
	
U32 LineBar(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		backcolor);

U32 ProgressBar(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		font_no,
			int		backcolor,
			int		barcolor,
			int 	value
);

U32 TransParencyTextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		align,		//Left, Right, Center
			int		attr,		//READ_ONLY, RW_MODE						
			int	(*MouseOnEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );


U32 ImageBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			char	*image );
			
U32 Panel(	HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	style,		//IN_EDGE, OUT_EDGE
			char	*image);

U32 ScrollBar(HANDLE ContainerHandle, int style, int sx, int sy, int width, int height, 
			int backcolor, 
			int position,
			int		(*ChangeEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
			

U32 GroupBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int		fontcolor, 
			int		backcolor, 
			int		font_no, 
			char *Text);
            
U32 FontLabel(HANDLE ContainerHandle, 
                int 	sx, int sy,         //컨테이너에서의 시작위치
                int width, int height,    //위젯의크기  
                char	*text,		        //표시할문자열
                int		align,		        //정렬모드 RIGHT_ALIGN, , 
                uint16_t    font_x_size, //폰트의 X 사이즈
                uint16_t    font_y_size, //폰트의 Y 사이즈
                uint16_t    font_screen, //폰트가 로딩된 페이지
                uint8_t     *font_width_array, //폰트의 폭을조정하기위한 배열
                uint8_t     font_col_no,  //폰트가 몇행으로 구성되어있는가?
                uint8_t     font_col1_start_code, //1번째행의 시작코드 ex) ' '
                uint16_t    font_start_y1,
                uint8_t     font_col2_start_code, //2번째행의 시작코드 ex) '@'
                uint16_t    font_start_y2,
                uint8_t     font_col3_start_code, //3번째행의 시작코드    
                uint16_t    font_start_y3,
                uint8_t     rop_mode       //AND, OR
			);	
                
//폰트를 디자인할때는 0x20 ~ 부터 순서대로 디자인되어야 한다.
U32 FontNumLabel(HANDLE ContainerHandle, 
                int 	sx, int sy,         //컨테이너에서의 시작위치
                int width, int height,    //위젯의크기  
                char	*text,		        //표시할문자열
                int		align,		        //정렬모드 RIGHT_ALIGN, , 
                uint16_t    font_x_size, //폰트의 X 사이즈
                uint16_t    font_y_size, //폰트의 Y 사이즈
                uint16_t    font_screen, //폰트가 로딩된 페이지
                uint16_t    font_start_y, //스크린에서의 폰트시작위치
                uint16_t    font_dot_width, //스크린에서의 폰트시작위치
                uint8_t     rop_mode,       //AND, OR
				int	    (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *)				
			);     
U32 IconBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			char	*IconName,
			char	*Text,
			int	id,									// 크릭이벤트에 가지고들어갈 자신의 ID지정
			int	(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
			
U32 ClearTextLabel(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			char	*text,		//init Character
			int		align);		//Left, Right, Center
				
U32 SystemTextBox(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int	backcolor, 
			int	fontcolor,
			int	align,		//Left, Right, Center
			int	attr,		//READ_ONLY, RW_MODE
			U32	id,
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );		

U32 SystemPanel(HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	font_no,
			char	*text,		//init Character
			int		backcolor, 
			int		fontcolor,
			int		align,		//Left, Right, Center
			int		attr,		//READ_ONLY, RW_MODE
			int		(*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) );
			
U32 Graph(	HANDLE ContainerHandle, 				// 위젯윈도우이므로 컨테이너를지정해야한다.
			int 		sx, int sy, int width, int height, 	// 컨테이너안에 위젯의위치와 크기지정
			float	ymax,	// Y max 이값은 SetWidget으로 변경할수있다.
			int		x_inc );	// X축 증가값  이값은 SetWidget으로 변경할수있다.			
int SetWidgetText(U32 id, char *text);
int SetWidgetFocus(U32 id, int status);
int AddWidgetString(U32 id, char *text);
int AddWidgetChar(U32 id, char text);
int AddWidgetClear(U32 id);
int WidgetRedraw(U32 id);

int GetWidgetText(U32 id, char *text);
int SetWidgetColor(U32 id, int backcolor, int fontcolor);
int SetWidgetCheck(U32 id, int check);
int GetWidgetCheck(U32 id);
int GetWidgetPosition(U32 id);
int SetWidgetPosition(U32 id, int value);
int SetWidgetImage(U32 id, char *image);
int SetWidgetFont(U32 id, char *font_name, int gap, int space, int font_color);
int SetWidgetStatus(U32 id, int status);
int SetButtonWidgetImage(U32 id, char *normal_image, char *touch_image);

extern volatile int g_Timer_msec;
extern volatile int g_Timer_msec;
extern volatile int gWinTimerCount;
extern volatile U8	WinTimerFlag[64];	
int OpenTimer(HANDLE parent, HANDLE handle, int interval);
int StartTimer(int id);
int StopTimer(int id);
int CloseTimer(int id);
int WindowCloseAuto(HANDLE parent, HANDLE handle);
extern volatile int gTimerWCount; 	//메시지 라이트 포인트
extern volatile int gTimerRCount;	//메시지 리드 포인트
void TimerMSGInsert(MSG *message);
void Window_Resource_Load(void);
int GetWindowHeight(HANDLE parent, HANDLE handle);
int GetWindowZorder(HANDLE parent, HANDLE handle);

Windows *FindNullWindowChain(int win_no);
void MessageQClear(void);
int GetWindowName(char *name);
void PaintDrawControl(int s);
void KeyPad_Repeat(int s);

//**********************************************************************************
// USB 용
extern volatile int     gUSB_MSC_Status;
// KeyPad 용
extern volatile int     g_KeyPad_Repeat;
// 엔코더용
extern volatile int     gPosition;

extern volatile uint8_t gROP2;
/*
typedef enum 
{
    DISCONNECTED=0;
    INTER_PROCESSING;
    FILE_WRITE_STANDBY;    
} USB_Status;
*/

//*********************************************************************************/
//Windows 용 사운드 관련
extern __IO U32 gSoundCnt, gSoundCnt1, gSoundCnt2, gSoundCnt3;
extern __IO U32 gSoundAddr,gSoundAddr1, gSoundAddr2, gSoundAddr3 ; //  = (0x6ABF2000+44);
extern __IO U32 gSoundSize, gSoundSize1, gSoundSize2, gSoundSize3;
//*********************************************************************************/
//Windows 용 DialogBox 관련
void KeyPad(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title);
int KeyBoard(U32 id, int sx, int sy, char *value, char *title);
U32 LineTwoLabel(HANDLE ContainerHandle, int sx, int sy, int width, int height , char *text, int	fontcolor, char *text2);

#endif

#ifdef __cplusplus
  }
#endif
