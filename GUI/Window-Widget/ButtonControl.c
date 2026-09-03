/**
  ******************************************************************************
  * @file       ButtonControl.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월7일
  * @brief    이파일은  버튼위젯에대해 제어할수있는 모든함수의 집합이다.
                
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
/* Private typedef -----------------------------------------------------------*/
///< 위젯의 변경을 바로적용할지, 다음에 윈도우가 적용하게할지를 결정한다
#define		UPDATE		1	//!< 즉시변경한다.
#define		CHANGE	0	//!< 윈도우가 알아서 적용한다.

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void GraphicsPointerLoad(void);
extern void GraphicsPointerSave(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @fn        Button_SetText
  * @brief    이함수는  버튼의 문자열을 변경한다.
  * @date    2014년 12월7일  
  * @param    id: 		제어할버튼의 ID
  * @param    text:		변경할 문자열
  * @param   action	:	변경을 바로적용할지, 다음에 윈도우가 적용하게할지를 결정한다
  * 		@arg 	UPDATE->즉시변경한다.
  * 		@arg 	CHANGE->윈도우가 알아서 적용한다. CreateEvent에서 생성시에 사용한다.
  * @retval  true, false
  * @todo    
  */ 
void NInnerBoxDraw(Graphics *Gr, int sx, int sy, int width, int height);
int Button_SetText(U32 id, char *text, int action)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==0)
	{
		Dprintf("%s->%s Object Err Win=%04x\n", __FILE__, __FUNCTION__, win);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	//!< 윈도우의 텍스트길이는 64자를 넘을수없다.
	int len = strlen(text);
	if(len > 64)
	{
		Dprintf("%s->%s TextLen Over\n",  __FILE__, __FUNCTION__);		
		return(0);
	}
	GraphicsPointerSave();
	strcpy(win->Text, text);		
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, action, 0, 0);		
	GraphicsPointerLoad();
    return(1);		
}
/**
	char	buf[20];
	@brief Button_GetText(id, buf, sizeof(buf));
*/
int Button_GetText(U32 id, char *text, int max_len)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	Windows	*win = GetWindow(parent, handle, "");
	if(win==0)
	{
		Dprintf("%s->%s Object Err Win=%04x\n", __FILE__, __FUNCTION__, win);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	//!< 매개변수의 버퍼가 모자라서 텍스트가 넘치면 치명적인 에러가 발생할수 있다.
	//!< 방지하기 위해서는 리턴받을 문자길이의 최대치를 지정해줘야한다.
	int len = strlen(win->Text);
	if(len > max_len)
	{
		Dprintf("%s->%s Buffer Over\n",  __FILE__, __FUNCTION__);
		return(0);
	}
	else strcpy(text, win->Text);
	return(1);	
}

int Button_SetIcon(U32 id, char *icon_name, int action)
{
	HANDLE parent = LOWORD(id);
	HANDLE handle = HIWORD(id);
	
	Windows	*win = GetWindow(parent, handle,"");
	if(win==0)
	{
		Dprintf("%s->%s Object Err Win=%04x\n", __FILE__, __FUNCTION__, win);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}	
	//!< 윈도우의 아이콘이름의 길이는 32자를 넘을수없다.
	int len = strlen(icon_name);
	if(len > 32)
	{
		Dprintf("%s->%s IconName Len Over\n",  __FILE__, __FUNCTION__);		
		return(0);
	}	
	strcpy(win->IconName, icon_name);		
	if(win->PaintEvent != NULL) win->PaintEvent(win->Parent, win->Handle, action, 0, 0);		
	
    return(1);		
}

#define	BUTTON_FONT_HEIGHT	24
#define	ICON_TEXT_INTERVAL	5
int ButtonTextDraw(Windows *win, Graphics *Gr, int icon_size)
{
	int	sx, sy;
	//!< 글씨를 쓰기위한 루틴, 문자가 있을때만 출력하게한다.
	if(strlen(win->Text))		
	{
		//int text_width = Get_CFontText_Width(20, win->Text);
		SetFontAliasing("ng_eh_24.sif");
		int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
		
		text_width += icon_size;
		sx = (win->width - text_width) / 2;
		sy = (win->height - BUTTON_FONT_HEIGHT) / 2;

        sx -= 5;//김대만
        
		sx += icon_size;
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
        //RGB565CONVERT(230, 230, 230)
		Gr->TextOutAliasing(sx+ICON_TEXT_INTERVAL, sy, win->Text, flash.systemFontColor, 0, "ng_eh_24.sif");
		//SystemFontSet(SYSTEMGRAY, SYSTEM_FONT_20);
		//Gr->CTextOut(sx+ICON_TEXT_INTERVAL, sy, win->Text);
	}		
	return(1);
}
int LableTextDraw(Windows *win, Graphics *Gr, int icon_size)
{
	int	sx, sy;
	//!< 글씨를 쓰기위한 루틴, 문자가 있을때만 출력하게한다.
	if(strlen(win->Text))		
	{
		//int text_width = Get_CFontText_Width(20, win->Text);
		SetFontAliasing("ng_eh_24.sif");
		int text_width = TextLenAliasing(win->Text, "ng_eh_24.sif");
		
		text_width += icon_size;
		sx = (win->width - text_width) / 2;
		sy = (win->height - BUTTON_FONT_HEIGHT) / 2;

        sx -= 5;//김대만
        
		sx += icon_size;
		if(sx < 0) sx = 0;
		if(sy < 0) sy = 0;
        //RGB565CONVERT(230, 230, 230)
		Gr->TextOutAliasing(sx+ICON_TEXT_INTERVAL, sy, win->Text, clWhite, 0, "ng_eh_24.sif");
		//SystemFontSet(SYSTEMGRAY, SYSTEM_FONT_20);
		//Gr->CTextOut(sx+ICON_TEXT_INTERVAL, sy, win->Text);
	}		
	return(1);
}
int ButtonIconDraw(Windows *win)
{
	char	icon_buf[32];

	int	icon_size=0;
	int	sy_icon = 0;
	int	sx_icon = 2;
	int 	text_width;
	
	//text_width = Get_CFontText_Width(20, win->Text);
	//!< 아이콘이름이 지정이 되어있으면 아이콘을 그린다.
	if(strlen(win->IconName)) 
	{
		//Dprintf("%s:: Iconname=%s\n", __FUNCTION__, win->IconName);
		//int is_icon = 1;
		//!< 아이콘 파일명의 첫글자를 보고 사이즈를 유츨한다.
		strncpy(icon_buf, win->IconName, 3);		// 32_. 48_, 64_, 32G, 48G, 64G
		icon_buf[3] = 0;
		//Dprintf("%s:: iconName=%s Header=%s\n", __FUNCTION__, win->IconName, icon_buf);
		//!< 아이콘이 들어갈영역이 되어야만 아이콘을 그린다.
		if(win->height > icon_size)
		{
			if(!(strcmp(icon_buf, "32_")))	
			{ 
				icon_size = 32;    
				text_width = text_width + icon_size;  
				sx_icon = (win->width - text_width) / 2;
				sy_icon = (win->height - icon_size) / 2;   	
				IconOut32W(sx_icon, sy_icon, win->IconName); 
			}
			else if(!(strcmp(icon_buf, "48_")))	
			{ 
				icon_size = 48;	
				text_width = text_width + icon_size;  
				sx_icon = (win->width - text_width) / 2;
				sy_icon = (win->height - icon_size) / 2;   
				IconOut48W(sx_icon, sy_icon+1, win->IconName); 
			}
#ifdef LCD_TS8080
			else if(!(strcmp(icon_buf, "64_")))	
			{ 
				icon_size = 64;	
				text_width = text_width + icon_size;  
				sx_icon = (win->width - text_width) / 2;
				sy_icon = (win->height - icon_size) / 2;   
				IconOut64W(sx_icon, sy_icon, win->IconName); 
			}
#endif			
			else if(!(strcmp(icon_buf, "32G")))	
			{ 
				icon_size = 32;	
				text_width = text_width + icon_size;  
				sx_icon = (win->width - text_width) / 2;
				sy_icon = (win->height - icon_size) / 2;   
				IconOut32G(sx_icon, sy_icon, win->IconName); 
			}
			else if(!(strcmp(icon_buf, "48G")))	
			{ 
				icon_size = 48;	
				text_width = text_width + icon_size;  
				sx_icon = (win->width - text_width) / 2;
				sy_icon = (win->height - icon_size) / 2;   
				sx_icon = 7;
				IconOut48G(sx_icon, sy_icon, win->IconName); 
			}
#ifdef LCD_TS8080
			else if(!(strcmp(icon_buf, "64G")))	
			{ 
				icon_size = 64;	
				text_width = text_width + icon_size;  
				sx_icon = (win->width - text_width) / 2;
				sy_icon = (win->height - icon_size) / 2;   
				IconOut64G(sx_icon, sy_icon, win->IconName); 
			}		
			//Dprintf("iconsize=%d sx=%d sy=%d \n", icon_size, sx_icon, sy_icon);	
#endif		
		}
		else		
		{
			Dprintf("%s:: ICON Draw Size Small\n", __FUNCTION__);
			icon_size=0;
		}
	}
	else 
	{
		icon_size=0;
	}
	return(icon_size);
}
#ifdef __cplusplus
}
#endif
