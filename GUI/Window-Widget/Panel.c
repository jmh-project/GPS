/**
  ******************************************************************************
  * @file       Panel.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월8일
  * @brief    이파일은  윈도우치장용 패널위젯이다.
                
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
#define		WINDOW_WIDTH		LCD_X
#define		WINDOW_HEIGHT		LCD_Y
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int	PanelPaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");
	int			sx=0, sy=0;
	int			style;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	style = win->Param;
	//내부의 색을칠한다.
	Gr->SetColor(SYSTEMGRAY);	
	Gr->FillRect(sx, sy, win->width, win->height);
	
	if(strlen(win->NormalName))
	{
		Gr->BmpDisplay(0, 0, win->NormalName);
	}
	
	if(style==OUT_EDGE)
	{
		Gr->SetColor(WHITE);
		Gr->Line(sx, sy, sx, sy+win->height-1);		//좌측수직라인그리기
		Gr->Line(sx, sy, sx+win->width-1, sy); 	
		Gr->SetColor(BLACK);
		Gr->Line(sx+win->width-1, sy, sx+win->width-1, sy+win->height);	//우측수직라인
		Gr->Line(sx, sy+win->height-1, sx+win->width, sy+win->height-1);//아래흑색라인
		Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
		Gr->Line(sx+win->width-2, sy+1, sx+win->width-2, sy+win->height-2);//우측회색라인	
		Gr->Line(sx+1, sy+win->height-2, sx+win->width-2, sy+win->height-2);//아래회색라인
	}
	else if(style==IN_EDGE)
	{
		//스타일을 그린다.
		Gr->SetColor(BLACK);
		Gr->Line(sx, sy, sx, sy+win->height-1);			//좌측수직라인그리기		
		Gr->Line(sx, sy, sx+win->width-1, sy);			//상단수직라인		
		Gr->SetColor(WHITE);	//우측수직라인
		Gr->Line(sx+win->width-1, sy+1, sx+win->width-1, sy+1+win->height);		
		Gr->Line(sx+1, sy+win->height-1, sx+1+win->width, sy+win->height-1);//아래백색라인
		Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
		Gr->Line(sx+1, sy+1, sx+1, sy+win->height-2);	//좌측회색라인		
		Gr->Line(sx+1, sy+1, sx+win->width-2, sy+1);	//위회색라인
	}
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

U32 Panel(	HANDLE ContainerHandle, 
			int 	sx, int sy, int width, int height, 
			int 	style,		//IN_EDGE, OUT_EDGE
			char	*image)

{
	HANDLE	handle;
	U32		id;
	
	Windows		*parent_win;
	parent_win = GetWindow(NULL, ContainerHandle, "Panel");
	
	sx = sx + (parent_win->client_sx - parent_win->sx );
	sy = sy + (parent_win->client_sy - parent_win->sy );
	handle=makeWindow(
				LCD,						// Display Section Position
				"Panel",					// Windows Name
				ContainerHandle,			// Parent Window Handle
				WIDGET_WINDOW,				// Windows Kind
				STYLE_NONE,					// Windows Style
				sx, sy, 					// Start Position
				width, height,				// Size
				SHOW,						// Create after Show
				NULL,						// CreateEvent
				PanelPaintEvent,			// PaintEvent
				NULL,						// MouseOnEvent
				NULL,						// MouseOffEvent
				NULL,						// MouseMoveEvent
				NULL,						// GestureEvent
				NULL,						// ClickEvent
				NULL,						// TimerEvent
				NULL,  						// ControlEvent
				NULL,  						// ChangeEvent
                NULL,
                NULL,
                NULL,
                NULL,
				style,							// PARAMETER
				image,						// Normal Touch Screen or BMP
				NULL,						// Touch Screen or ICON
				NULL,						// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle
	return(id);				
}				
#ifdef __cplusplus
}
#endif
