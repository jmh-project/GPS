/**
  ******************************************************************************
  * @file       WinStyleDraw.c
  * @author  (주)자이언텍 기술개발지원팀
  * @version V0.1
  * @date    2014년 12월6일
  * @brief    이파일은  윈도우내부에서 모양을 내기위한 함수들의 집합파일이다.
                
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
//#define		COLOR_ADD		0
#define		COLOR_ADD		230-191
//좌우가 볼록한 패널모양을 그릴때 사용한다. 윈도우용이다.
void WindowPanel(Windows *win, Graphics *Gr, int sx, int sy, int width, int height, int outline)
{
	//int	width = win->width-1;
	//int	height = win->height-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(191+COLOR_ADD, 191+COLOR_ADD, 191+COLOR_ADD));
	Gr->FillRect(sx, sy, width, height);	

	//좌측
	Gr->SetColor(RGB565CONVERT(187+COLOR_ADD, 187+COLOR_ADD, 187+COLOR_ADD));
	Gr->VLine(sx, sy, height);
	Gr->SetColor(RGB565CONVERT(204+COLOR_ADD, 204+COLOR_ADD, 204+COLOR_ADD));
	Gr->VLine(sx+1, sy, height);
	Gr->SetColor(RGB565CONVERT(208+COLOR_ADD, 208+COLOR_ADD, 208+COLOR_ADD));
	Gr->VLine(sx+2, sy, height);
	Gr->SetColor(RGB565CONVERT(209+COLOR_ADD, 209+COLOR_ADD, 209+COLOR_ADD));
	Gr->VLine(sx+3, sy, height);
	Gr->SetColor(RGB565CONVERT(211+COLOR_ADD, 211+COLOR_ADD, 211+COLOR_ADD));
	Gr->VLine(sx+4, sy, height);
	Gr->SetColor(RGB565CONVERT(210+COLOR_ADD, 210+COLOR_ADD, 210+COLOR_ADD));
	Gr->VLine(sx+5, sy, height);
	Gr->SetColor(RGB565CONVERT(208+COLOR_ADD, 208+COLOR_ADD, 208+COLOR_ADD));
	Gr->VLine(sx+6, sy, height);
	Gr->SetColor(RGB565CONVERT(206+COLOR_ADD, 206+COLOR_ADD, 206+COLOR_ADD));
	Gr->VLine(sx+7, sy, height);
	Gr->SetColor(RGB565CONVERT(202+COLOR_ADD, 202+COLOR_ADD, 202+COLOR_ADD));
	Gr->VLine(sx+8, sy, height);
	Gr->SetColor(RGB565CONVERT(197+COLOR_ADD, 197+COLOR_ADD, 197+COLOR_ADD));
	Gr->VLine(sx+9, sy, height);
	//우측
	Gr->SetColor(RGB565CONVERT(64+COLOR_ADD, 64+COLOR_ADD, 64+COLOR_ADD));
	Gr->VLine(sx+width, sy, height);
	Gr->SetColor(RGB565CONVERT(112+COLOR_ADD, 112+COLOR_ADD, 112+COLOR_ADD));
	Gr->VLine(sx+width-1, sy, height);
	Gr->SetColor(RGB565CONVERT(130+COLOR_ADD, 130+COLOR_ADD, 130+COLOR_ADD));
	Gr->VLine(sx+width-2, sy, height);
	Gr->SetColor(RGB565CONVERT(139+COLOR_ADD, 139+COLOR_ADD, 139+COLOR_ADD));
	Gr->VLine(sx+width-3, sy, height);
	Gr->SetColor(RGB565CONVERT(147+COLOR_ADD, 147+COLOR_ADD, 147+COLOR_ADD));
	Gr->VLine(sx+width-4, sy, height);
	Gr->SetColor(RGB565CONVERT(153+COLOR_ADD, 153+COLOR_ADD, 153+COLOR_ADD));
	Gr->VLine(sx+width-5, sy, height);
	Gr->SetColor(RGB565CONVERT(157+COLOR_ADD, 157+COLOR_ADD, 157+COLOR_ADD));
	Gr->VLine(sx+width-6, sy, height);
	Gr->SetColor(RGB565CONVERT(162+COLOR_ADD, 162+COLOR_ADD, 162+COLOR_ADD));
	Gr->VLine(sx+width-7, sy, height);
	Gr->SetColor(RGB565CONVERT(166+COLOR_ADD, 166+COLOR_ADD, 166+COLOR_ADD));
	Gr->VLine(sx+width-8, sy, height);
	Gr->SetColor(RGB565CONVERT(170+COLOR_ADD, 170+COLOR_ADD, 170+COLOR_ADD));
	Gr->VLine(sx+width-9, sy, height);
	Gr->SetColor(RGB565CONVERT(174+COLOR_ADD, 174+COLOR_ADD, 174+COLOR_ADD));
	Gr->VLine(sx+width-10, sy, height);
	Gr->SetColor(RGB565CONVERT(180+COLOR_ADD, 180+COLOR_ADD, 180+COLOR_ADD));
	Gr->VLine(sx+width-11, sy, height);
	Gr->SetColor(RGB565CONVERT(186+COLOR_ADD, 186+COLOR_ADD, 186+COLOR_ADD));
	Gr->VLine(sx+width-12, sy, height);
	//상단
	Gr->SetColor(RGB565CONVERT(208+COLOR_ADD, 208+COLOR_ADD, 208+COLOR_ADD));
	Gr->HLine(sx+1, sy, width-4);
	Gr->SetColor(RGB565CONVERT(210+COLOR_ADD, 210+COLOR_ADD, 210+COLOR_ADD));
	Gr->HLine(sx+3, sy+1, width-8);
	//하단
	Gr->SetColor(RGB565CONVERT(133+COLOR_ADD, 133+COLOR_ADD, 133+COLOR_ADD));
	Gr->HLine(sx+1, sy+height, width-4);
	Gr->SetColor(RGB565CONVERT(165+COLOR_ADD, 165+COLOR_ADD, 165+COLOR_ADD));
	Gr->HLine(sx+3, sy+height-1, width-8);	
	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
	//IconOut48G(10, 10, "48G_Tool_Icon1.bmp");
}
