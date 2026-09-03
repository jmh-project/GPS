/******************************************************************************
*    	File name	: WidgetCommon.c 
*		Description : TEECO GUI Window Control Widget [Common]
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
//	id = MAKELONG(ContainerHandle, handle);	//LOWORD->ContainerHandle, HIWORD->handle

//그래픽핸들의 상태를 임시저정후 다시 복귀시키는 함수이다.
volatile HANDLE g_Grapgics_Parent;
volatile HANDLE g_Grapgics_Handle;
volatile int g_Graphics_OffsetX;
volatile int g_Graphics_OffsetY;

void GraphicsPointerSave(void)
{
	g_Grapgics_Parent = gp_Graphics->Parent;	
	g_Grapgics_Handle = gp_Graphics->Handle;
	g_Graphics_OffsetX = gp_Graphics->Offset_X;
	g_Graphics_OffsetY = gp_Graphics->Offset_Y;
}

void GraphicsPointerLoad(void)
{
	gp_Graphics->Parent = g_Grapgics_Parent;
	gp_Graphics->Handle = g_Grapgics_Handle;
	gp_Graphics->Offset_X = g_Graphics_OffsetX;
	gp_Graphics->Offset_Y = g_Graphics_OffsetY;
}

int GetWidgetText(U32 id, char *text)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle, "");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	strcpy(text, win->Text);
	return(1);	
}

int SetWidgetText(U32 id, char *text)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
    
	GraphicsPointerSave();
	strcpy(win->Text, text);
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 12, 0);
	
	GraphicsPointerLoad();
	
    return(1);		
}

int AddWidgetString(U32 id, char *text)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}	
    if(win->PaintEvent != NULL) 
    {
        win->PaintEvent(win->Parent, win->Handle, 0, 0, (char *)text);	
    }

	return(1);		
}

int AddWidgetChar(U32 id, char text)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}	

    if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 1, text, 0);	

	return(1);		
}

int AddWidgetClear(U32 id)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}	

    if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 2, 0, 0);	

	return(1);		
}

int SetWidgetColor(U32 id, int backcolor, int fontcolor)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	win->BackColor = backcolor;
	win->FontColor = fontcolor;
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}

int SetWidgetCheck(U32 id, int check)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
//Dprintf("SetWidgetCheck::parent=%d handle=%d\n", win->Parent, win->Handle);	
	win->Param=check;
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	//GraphicsPointerLoad();
	
	return(1);		
}

int WidgetDisplay(U32 id, int check) //SHOW, HIDDEN
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	win->ShowStatus=check;
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	
	return(1);		
}


int WidgetRedraw(U32 id)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	
	return(1);		
}

int GetWidgetCheck(U32 id)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	if(win->Param==TRUE) return TRUE;
	else return FALSE;
}

int SetWidgetPosition(U32 id, int value)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	
	win->Position = value;
	
	if(win->PaintEvent != NULL)win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}

int GetWidgetPosition(U32 id)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	return (win->Position);
}

int SetWidgetImage(U32 id, char *image)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	
	strcpy(win->NormalName,image);
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}

int SetButtonWidgetImage(U32 id, char *normal_image, char *touch_image)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	
	strcpy(win->NormalName, normal_image);
	strcpy(win->TouchName, touch_image);
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}

int SetWidgetIcon(U32 id, char *iconname)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	
	strcpy(win->IconName,iconname);	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}


int SetWidgetFont(U32 id, char *font_name, int gap, int space, int font_color)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	
	//strcpy(win->FontName, font_name);
	win->FontGap = gap;
	win->FontSpaceGap = space;
	win->FontColor = font_color;
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}

int SetWidgetFontNo(U32 id, int font_no, int gap, int space, int font_color)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	
	win->FontNo = font_no;
	win->FontGap = gap;
	win->FontSpaceGap = space;
	win->FontColor = font_color;
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, 0, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}

//버튼의 눌린모양을 변경할때 사용한다.
int SetWidgetStatus(U32 id, int status)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, status, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}

//버튼의 눌린모양을 조회할때 사용한다.
int GetWidgetStatus(U32 id)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	
	return(win->ToggleStatus);	
}

//위젯의 포커스를 주거나 해제할때 사용한다. status=ON, OFF
int SetWidgetFocus(U32 id, int status)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}
	//GraphicsPointerSave();
	
	if(win->FocusEvent != NULL) win->FocusEvent(win->Parent, win->Handle, status, 0, 0);
	//GraphicsPointerLoad();
	return(1);		
}


//위젯등을 제작시 부모윈도우의 바탕영역의 일부를 가지고 와야할경우사용한다.
int GetWindowRectBmp(HANDLE parent, HANDLE handle, int sx, int sy, int width, int height, void *Data)
{
	//U16		*data = (U16 *)Data;
	Windows *win = GetWindow(parent, handle,"");
	if(win==NULL)
	{
		Dprintf("%s::Window Error\n", __FUNCTION__);
		return(0);
	}	
	if(parent!=NULL) //컨테이너윈도우일경우
	{
		
	}
	else	//위젯윈도우일 경우
	{
	}
	return(1);
}
#ifdef __cplusplus
}
#endif
