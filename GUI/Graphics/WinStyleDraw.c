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

#include "WinStyleDraw.h"

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/// 약간 튀어나온 패널을 그린다.
void OuterPanel_Draw(Windows *win, Graphics *Gr, int sx, int sy, int width, int height)
{
	Gr->SetColor(WHITE);
	Gr->Line(sx, sy, sx, sy+height-1);		//좌측수직라인그리기
	Gr->Line(sx, sy, sx+width-1, sy); 	//상단라인그리기
	
	Gr->SetColor(BLACK);
	Gr->Line(sx+width-1, sy, sx+width-1, sy+height);	//바깥쪽 우측수직라인
	Gr->Line(sx, sy+height-1, sx+width, sy+height-1);//바깥쪽 아래라인
	
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
	Gr->Line(sx+width-2, sy+1, sx+width-2, sy+height-2);//안쪽우측수직라인	
	Gr->Line(sx+1, sy+height-2, sx+width-2, sy+height-2);//안쪽아래라인	
}

/// 약간들어간 패널을 그린다.
void InnerPanel_Draw(Windows *win, Graphics *Gr, int sx, int sy, int width, int height)
{
	Gr->SetColor(BLACK);
	Gr->Line(sx, sy, sx, sy+height-1);		//좌측수직라인그리기
	Gr->Line(sx, sy, sx+width-1, sy); 		//상단라인그리기
	
	Gr->SetColor(WHITE);
	Gr->Line(sx+width-1, sy, sx+width-1, sy+height-1);	//바깥쪽 우측수직라인
	Gr->Line(sx, sy+height-1, sx+width-1, sy+height-1);//바깥쪽 아래라인
	
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));	
	Gr->Line(sx+width-2, sy+1, sx+width-2, sy+height-2);//안쪽우측수직라인	
	Gr->Line(sx+1, sy+height-2, sx+width-2, sy+height-2);//안쪽아래라인	

	Gr->Line(sx+1, sy+1, sx+1, sy+height-2);	//좌측 안쪽회색 수직라인		
	Gr->Line(sx+1, sy+1, sx+width-2, sy+1);		//상단 안쪽회색 수평라인
}

//깊이들어간 패널을 그린다.
void InnerBoxDraw(Graphics *Gr, int sx, int sy, int width, int height)
{
	//채우는색
	//Gr->SetColor(clWhite);		
	//Gr->FillRect(sx, sy, width, height);	

	//좌측, 상단라인
	Gr->SetColor(RGB565CONVERT(164, 164, 164));
	Gr->Rectangle(sx, sy, width, height);

	Gr->SetColor(RGB565CONVERT(147, 147, 147));
	Gr->Rectangle(sx+1, sy+1, width-2, height-2);

	Gr->SetColor(RGB565CONVERT(136, 136, 136));
	Gr->Rectangle(sx+2, sy+2, width-4, height-4);

	Gr->SetColor(RGB565CONVERT(130, 130, 130));
	Gr->Rectangle(sx+3, sy+3, width-6, height-6);
	//하단라인
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->HLine(sx, sy+height, width);

	Gr->SetColor(RGB565CONVERT(195, 195, 195));
	Gr->HLine(sx+1, sy+height-1, width-2);

	Gr->SetColor(RGB565CONVERT(190, 190, 190));
	Gr->HLine(sx+2, sy+height-2, width-4);
	
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->HLine(sx+3, sy+height-3, width-6);
	//우측라인
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->VLine(sx+width, sy+1, height-1);

	Gr->SetColor(RGB565CONVERT(195, 195, 195));
	Gr->VLine(sx+width-1, sy+2, height-4);

	Gr->SetColor(RGB565CONVERT(190, 190, 190));
	Gr->VLine(sx+width-2, sy+3, height-6);
	
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->VLine(sx+width-3, sy+4, height-8);	
}


void TextBoxStyle_Draw(Windows *win, Graphics *Gr, int sx, int sy, int width, int height)
{
	Gr->SetColor(WHITE);
	Gr->FillRect(sx, sy, width, height);

	//스타일을 그린다.
	Gr->SetColor(BLACK);
	Gr->Line(sx, sy, sx, sy+height-1);		//바깥쪽 좌측 수직라인그리기		
	Gr->Line(sx, sy, sx+width-1, sy);			//바깥쪽 상단 수평라인그리기		
	
	Gr->SetColor(WHITE);	
	Gr->Line(sx+width-1, sy+1, sx+width-1, sy+1+height);		//바깥쪽우측 수직라인
	Gr->Line(sx+1, sy+height-1, sx+1+width, sy+height-1);	//바깥쪽아래 수평라인
	
	Gr->SetColor(RGB565CONVERT(0x88, 0x88, 0x88));		
	Gr->Line(sx+1, sy+1, sx+1, sy+height-2);	//좌측 안쪽회색 수직라인		
	Gr->Line(sx+1, sy+1, sx+width-2, sy+1);		//상단 안쪽회색 수평라인
}

//다이아몬드스타일의 버튼을 그린다.
void DiamondButton(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->FillRect(sx, sy, win->width, win->height);	

	//좌측
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->VLine(sx, sy, height);
	Gr->SetColor(RGB565CONVERT(204, 204, 204));
	Gr->VLine(sx+1, sy+1, height-2);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx+2, sy+2, height-4);
	Gr->SetColor(RGB565CONVERT(209, 209, 209));
	Gr->VLine(sx+3, sy+3, height-6);
	Gr->SetColor(RGB565CONVERT(211, 211, 211));
	Gr->VLine(sx+4, sy+4, height-8);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->VLine(sx+5, sy+5, height-10);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx+6, sy+6, height-12);
	Gr->SetColor(RGB565CONVERT(206, 206, 206));
	Gr->VLine(sx+7, sy+7, height-14);
	Gr->SetColor(RGB565CONVERT(202, 202, 202));
	Gr->VLine(sx+8, sy+8, height-16);
	Gr->SetColor(RGB565CONVERT(197, 197, 197));
	Gr->VLine(sx+9, sy+9, height-18);
	//우측
	Gr->SetColor(RGB565CONVERT(64, 64, 64));
	Gr->VLine(sx+width, sy, height);
	Gr->SetColor(RGB565CONVERT(112, 112, 112));
	Gr->VLine(sx+width-1, sy+1, height-2);
	Gr->SetColor(RGB565CONVERT(130, 130, 130));
	Gr->VLine(sx+width-2, sy+2, height-4);
	Gr->SetColor(RGB565CONVERT(139, 139, 139));
	Gr->VLine(sx+width-3, sy+3, height-6);
	Gr->SetColor(RGB565CONVERT(147, 147, 147));
	Gr->VLine(sx+width-4, sy+4, height-8);
	Gr->SetColor(RGB565CONVERT(153, 153, 153));
	Gr->VLine(sx+width-5, sy+5, height-10);
	Gr->SetColor(RGB565CONVERT(157, 157, 157));
	Gr->VLine(sx+width-6, sy+6, height-12);
	Gr->SetColor(RGB565CONVERT(162, 162, 162));
	Gr->VLine(sx+width-7, sy+7, height-14);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+width-8, sy+8, height-16);
	Gr->SetColor(RGB565CONVERT(170, 170, 170));
	Gr->VLine(sx+width-9, sy+9, height-18);
	Gr->SetColor(RGB565CONVERT(174, 174, 174));
	Gr->VLine(sx+width-10, sy+10, height-20);
	Gr->SetColor(RGB565CONVERT(180, 180, 180));
	Gr->VLine(sx+width-11, sy+11, height-22);
	Gr->SetColor(RGB565CONVERT(186, 186, 186));
	Gr->VLine(sx+width-12, sy+12, height-24);
	//상단
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->HLine(sx+1, sy, width-4);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->HLine(sx+3, sy+1, width-8);
	//하단
	Gr->SetColor(RGB565CONVERT(133, 133, 133));
	Gr->HLine(sx+1, sy+height, width-4);
	Gr->SetColor(RGB565CONVERT(165, 165, 165));
	Gr->HLine(sx+3, sy+height-1, width-8);
	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
}

//좌우가 볼록한 패널을 그릴때 사용한다. 위젯용이다.
void LeftRightPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->FillRect(sx, sy, win->width, win->height);	

	//좌측
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->VLine(sx, sy, height);
	Gr->SetColor(RGB565CONVERT(204, 204, 204));
	Gr->VLine(sx+1, sy, height);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx+2, sy, height);
	Gr->SetColor(RGB565CONVERT(209, 209, 209));
	Gr->VLine(sx+3, sy, height);
	Gr->SetColor(RGB565CONVERT(211, 211, 211));
	Gr->VLine(sx+4, sy, height);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->VLine(sx+5, sy, height);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx+6, sy, height);
	Gr->SetColor(RGB565CONVERT(206, 206, 206));
	Gr->VLine(sx+7, sy, height);
	Gr->SetColor(RGB565CONVERT(202, 202, 202));
	Gr->VLine(sx+8, sy, height);
	Gr->SetColor(RGB565CONVERT(197, 197, 197));
	Gr->VLine(sx+9, sy, height);
	//우측
	Gr->SetColor(RGB565CONVERT(64, 64, 64));
	Gr->VLine(sx+width, sy, height);
	Gr->SetColor(RGB565CONVERT(112, 112, 112));
	Gr->VLine(sx+width-1, sy, height);
	Gr->SetColor(RGB565CONVERT(130, 130, 130));
	Gr->VLine(sx+width-2, sy, height);
	Gr->SetColor(RGB565CONVERT(139, 139, 139));
	Gr->VLine(sx+width-3, sy, height);
	Gr->SetColor(RGB565CONVERT(147, 147, 147));
	Gr->VLine(sx+width-4, sy, height);
	Gr->SetColor(RGB565CONVERT(153, 153, 153));
	Gr->VLine(sx+width-5, sy, height);
	Gr->SetColor(RGB565CONVERT(157, 157, 157));
	Gr->VLine(sx+width-6, sy, height);
	Gr->SetColor(RGB565CONVERT(162, 162, 162));
	Gr->VLine(sx+width-7, sy, height);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+width-8, sy, height);
	Gr->SetColor(RGB565CONVERT(170, 170, 170));
	Gr->VLine(sx+width-9, sy, height);
	Gr->SetColor(RGB565CONVERT(174, 174, 174));
	Gr->VLine(sx+width-10, sy, height);
	Gr->SetColor(RGB565CONVERT(180, 180, 180));
	Gr->VLine(sx+width-11, sy, height);
	Gr->SetColor(RGB565CONVERT(186, 186, 186));
	Gr->VLine(sx+width-12, sy, height);
	//상단
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->HLine(sx+1, sy, width-4);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->HLine(sx+3, sy+1, width-8);
	//하단
	Gr->SetColor(RGB565CONVERT(133, 133, 133));
	Gr->HLine(sx+1, sy+height, width-4);
	Gr->SetColor(RGB565CONVERT(165, 165, 165));
	Gr->HLine(sx+3, sy+height-1, width-8);	
	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
	//IconOut48G(10, 10, "48G_Tool_Icon1.bmp");
}



//상하가 볼록한 패널을 그릴때 사용한다.
void UpDownPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->FillRect(sx, sy, win->width, win->height);	

	//상단
	Gr->SetColor(RGB565CONVERT(187, 187, 187));
	Gr->HLine(sx, sy, width);
	Gr->SetColor(RGB565CONVERT(204, 204, 204));
	Gr->HLine(sx, sy+1, width);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->HLine(sx, sy+2, width);
	Gr->SetColor(RGB565CONVERT(209, 209, 209));
	Gr->HLine(sx, sy+3, width);
	Gr->SetColor(RGB565CONVERT(211, 211, 211));
	Gr->HLine(sx, sy+4, width);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->HLine(sx, sy+5, width);
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->HLine(sx, sy+6, width);
	Gr->SetColor(RGB565CONVERT(206, 206, 206));
	Gr->HLine(sx, sy+7, width);
	Gr->SetColor(RGB565CONVERT(202, 202, 202));
	Gr->HLine(sx, sy+8, width);
	Gr->SetColor(RGB565CONVERT(197, 197, 197));
	Gr->HLine(sx, sy+9, width);
	//하단
	Gr->SetColor(RGB565CONVERT(64, 64, 64));
	Gr->HLine(sx, sy+height, width);
	Gr->SetColor(RGB565CONVERT(112, 112, 112));
	Gr->HLine(sx, sy+height-1, width);
	Gr->SetColor(RGB565CONVERT(130, 130, 130));
	Gr->HLine(sx, sy+height-2, width);
	Gr->SetColor(RGB565CONVERT(139, 139, 139));
	Gr->HLine(sx, sy+height-3, width);
	Gr->SetColor(RGB565CONVERT(147, 147, 147));
	Gr->HLine(sx, sy+height-4, width);
	Gr->SetColor(RGB565CONVERT(153, 153, 153));
	Gr->HLine(sx, sy+height-5, width);
	Gr->SetColor(RGB565CONVERT(157, 157, 157));
	Gr->HLine(sx, sy+height-6, width);
	Gr->SetColor(RGB565CONVERT(162, 162, 162));
	Gr->HLine(sx, sy+height-7, width);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->HLine(sx, sy+height-8, width);
	Gr->SetColor(RGB565CONVERT(170, 170, 170));
	Gr->HLine(sx, sy+height-9, width);
	Gr->SetColor(RGB565CONVERT(174, 174, 174));
	Gr->HLine(sx, sy+height-10, width);
	Gr->SetColor(RGB565CONVERT(180, 180, 180));
	Gr->HLine(sx, sy+height-11, width);
	Gr->SetColor(RGB565CONVERT(186, 186, 186));
	Gr->HLine(sx, sy+height-12, width);
	//좌측
	Gr->SetColor(RGB565CONVERT(208, 208, 208));
	Gr->VLine(sx, sy+1, height-4);
	Gr->SetColor(RGB565CONVERT(210, 210, 210));
	Gr->VLine(sx+1, sy+3, height-8);
	//우측
	Gr->SetColor(RGB565CONVERT(133, 133, 133));
	Gr->VLine(sx+width, sy+1, height-4);
	Gr->SetColor(RGB565CONVERT(165, 165, 165));
	Gr->VLine(sx+width-1, sy+3, height-8);	
	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
}

//선이굵은 패널을 그릴때 사용한다.
void OuterPanelDraw(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(RGB565CONVERT(181, 181, 181));
	Gr->FillRect(sx, sy, win->width, win->height);	
	//포인트
	/* 별차이가 없어보여 주석처리한다.
	Gr->SetColor(RGB565CONVERT(204, 204, 204));
	Gr->Plot(sx, sy);
	Gr->SetColor(RGB565CONVERT(207, 207, 207));
	Gr->Plot(sx+1, sy+1);
	Gr->SetColor(RGB565CONVERT(205, 205, 205));
	Gr->Plot(sx+2, sy+2);
	Gr->SetColor(RGB565CONVERT(196, 196, 196));
	Gr->Plot(sx+3, sy+3);
	*/
	//좌측
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->VLine(sx, sy, height);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+1, sy, height);
	Gr->SetColor(RGB565CONVERT(201, 201, 201));
	Gr->VLine(sx+2, sy, height);
	Gr->SetColor(RGB565CONVERT(195, 195, 195));
	Gr->VLine(sx+3, sy, height);
	//상단
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->HLine(sx+3, sy, width-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+2, sy+1, width-2);
	Gr->SetColor(RGB565CONVERT(201, 201, 201));
	Gr->HLine(sx+2, sy+2, width-2);
	Gr->SetColor(RGB565CONVERT(195, 195, 195));
	Gr->HLine(sx+1, sy+3, width-3);
	//우측
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->VLine(sx+width, sy, height);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->VLine(sx+width-1, sy+1, height);
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->VLine(sx+width-2, sy+2, height);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+width-3, sy+3, height);
	//하단
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->HLine(sx, sy+height, width);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->HLine(sx+1, sy+height-1, width-2);	
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->HLine(sx+2, sy+height-2, width-4);	
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->HLine(sx+3, sy+height-3, width-6);	
	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
}

//선이굵은 패널을 이용해 탭을그릴때 사용한다. (수평탭)
void TabPanelDrawColume(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	int	tab1_width = 70;
	int	tab1_height = 30;

	int	tab2_width = 70;
	int	tab2_height = 30;
	
	int	tab3_width = 70;
	int	tab3_height = 30;
	int	back_color = RGB565CONVERT(181, 181, 181);
	
	int	tab1_sx = sx;
	int	tab2_sx = sx;
	int	tab3_sx = sx;
	int	tab1_sy = sy;
	int	tab2_sy = sy;
	int	tab3_sy = sy;
	int	active=3;	//TAB 을 액티브시킨다.
	
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(back_color);
	Gr->FillRect(sx, sy, win->width, win->height);	
	//===========================================BODY	
	//본체좌측
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx, sy+tab1_height, height);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+1, sy+tab1_height, height);
	Gr->SetColor(RGB565CONVERT(201, 201, 201));
	Gr->VLine(sx+2, sy+tab1_height, height);
	Gr->SetColor(RGB565CONVERT(195, 195, 195));
	Gr->VLine(sx+3, sy+tab1_height, height);
	
	//상단
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->HLine(sx+3, sy+tab1_height, width-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+1, sy+tab1_height+1, width-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx, sy+tab1_height+2, width-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->HLine(sx, sy+tab1_height+3, width-3);
	//우측
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->VLine(sx+width, sy+tab1_height, height);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->VLine(sx+width-1, sy+tab1_height+1, height);
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->VLine(sx+width-2, sy+tab1_height+2, height);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+width-3, sy+tab1_height+3, height);
	
	//제일끝하단
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->HLine(sx, sy+height, width);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->HLine(sx+1, sy+height-1, width-2);	
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->HLine(sx+2, sy+height-2, width-4);	
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->HLine(sx+3, sy+height-3, width-6);	

	//제일끝하단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(171, 171, 171));	
	Gr->Plot(sx, sy+height-3);
	Gr->SetColor(RGB565CONVERT(187, 187, 187));	
	Gr->Plot(sx+1, sy+height-3);
	Gr->SetColor(RGB565CONVERT(184, 184, 184));	
	Gr->Plot(sx+2, sy+height-3);
	Gr->SetColor(RGB565CONVERT(178, 178, 178));	
	Gr->Plot(sx+3, sy+height-3);

	Gr->SetColor(RGB565CONVERT(142, 142, 142));	
	Gr->Plot(sx, sy+height-2);
	Gr->SetColor(RGB565CONVERT(174, 174, 174));	
	Gr->Plot(sx+1, sy+height-2);
	Gr->SetColor(RGB565CONVERT(170, 170, 170));	
	Gr->Plot(sx+2, sy+height-2);
	Gr->SetColor(RGB565CONVERT(164, 164, 164));	
	Gr->Plot(sx+3, sy+height-2);

	Gr->SetColor(RGB565CONVERT(125, 125, 125));	
	Gr->Plot(sx+1, sy+height-1);
	Gr->SetColor(RGB565CONVERT(153, 153, 153));	
	Gr->Plot(sx+2, sy+height-1);
	Gr->SetColor(RGB565CONVERT(147, 147, 147));	
	Gr->Plot(sx+3, sy+height-1);

	Gr->SetColor(RGB565CONVERT(99, 99, 99));	
	Gr->Plot(sx+3, sy+height);
	//===========================================TAB1
	sx = tab1_sx;
	sy = tab1_sy;
	int space=0;

	if(active==1)
	{
		//TAB1 살리기	
		Gr->SetColor(back_color);
		Gr->FillRect(sx+4, sy, tab1_width-4, tab1_height+10);	
	}
	else
	{
		//TAB1 높이줄이기
		sy+=2;
		tab1_height-=2;
		space=5;
		Gr->SetColor(back_color);
		Gr->FillRect(sx, sy, tab1_width, tab1_height);		
	}	

	//탭상단 탭번호 1 
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+space+3, sy, tab1_width-space-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+space+1, sy+1, tab1_width-space-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+space, sy+2, tab1_width-space-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->HLine(sx+space, sy+3, tab1_width-space-3);	
	
	//맨위의 탭좌측 탭번호 1
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+space, sy+2, tab1_height-4);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+space+1, sy+1, tab1_height-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+space+2, sy+1, tab1_height-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+space+3, sy, tab1_height);
	
	//맨위의 탭우측 탭번호 1
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->VLine(sx+tab1_width, sy, tab1_height);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->VLine(sx+tab1_width-1, sy+1, tab1_height-1);
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->VLine(sx+tab1_width-2, sy+2, tab1_height-2);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+tab1_width-3, sy+3, tab1_height-3);
	
	//맨위 탭좌상단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(198, 198, 198));	
	Gr->Plot(sx+space+3, sy);
	
	Gr->SetColor(RGB565CONVERT(206, 206, 206));	
	Gr->Plot(sx+space+1, sy+1);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+space+2, sy+1);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+space+3, sy+1);
	
	Gr->SetColor(RGB565CONVERT(194, 194, 194));	
	Gr->Plot(sx+space, sy+2);
	Gr->SetColor(RGB565CONVERT(225, 225, 225));	
	Gr->Plot(sx+space+1, sy+2);
	Gr->SetColor(RGB565CONVERT(249, 249, 249));	
	Gr->Plot(sx+space+2, sy+2);
	Gr->SetColor(RGB565CONVERT(223, 223, 223));	
	Gr->Plot(sx+space+3, sy+2);
	
	Gr->SetColor(RGB565CONVERT(199, 199, 199));	
	Gr->Plot(sx+space, sy+3);
	Gr->SetColor(RGB565CONVERT(214, 214, 214));	
	Gr->Plot(sx+space+1, sy+3);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+space+2, sy+3);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+space+3, sy+3);	
	//===========================================TAB2

	sx = tab2_sx+1;
	sy = tab2_sy;

	if(active==2)
	{
		//TAB2 살리기	
		Gr->SetColor(back_color);
		Gr->FillRect(sx+tab1_width+4, sy, tab2_width-4, tab2_height+10);	
	}
	else
	{
		//TAB1 높이줄이기
		sy+=2;
		tab2_height-=2;
		Gr->SetColor(back_color);
		Gr->FillRect(sx+tab1_width+1, sy, tab2_width, tab2_height);		
	}	

	//탭상단 탭번호 2 
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+tab1_width+3, sy, tab2_width-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+tab1_width+1, sy+1, tab2_width-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+tab1_width, sy+2, tab2_width-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->HLine(sx+tab1_width, sy+3, tab2_width-3);	

	//맨위의 탭좌측 탭번호 2
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+tab1_width, sy+2, tab2_height-4-space);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+tab1_width+1, sy+1, tab2_height-2-space);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+tab1_width+2, sy+1, tab2_height-2-space);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+tab1_width+3, sy, tab2_height-space);
	
	//탭우측 탭번호 2
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->VLine(sx+tab1_width+tab2_width, sy, tab2_height);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->VLine(sx+tab1_width+tab2_width-1, sy+1, tab2_height-1);
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->VLine(sx+tab1_width+tab2_width-2, sy+2, tab2_height-2);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+tab1_width+tab2_width-3, sy+3, tab2_height-3);

	
	//맨위 탭좌상단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(198, 198, 198));	
	Gr->Plot(sx+tab1_width+3, sy);
	
	Gr->SetColor(RGB565CONVERT(206, 206, 206));	
	Gr->Plot(sx+tab1_width+1, sy+1);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+tab1_width+2, sy+1);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+tab1_width+3, sy+1);
	
	Gr->SetColor(RGB565CONVERT(194, 194, 194));	
	Gr->Plot(sx+tab1_width, sy+2);
	Gr->SetColor(RGB565CONVERT(225, 225, 225));	
	Gr->Plot(sx+tab1_width+1, sy+2);
	Gr->SetColor(RGB565CONVERT(249, 249, 249));	
	Gr->Plot(sx+tab1_width+2, sy+2);
	Gr->SetColor(RGB565CONVERT(223, 223, 223));	
	Gr->Plot(sx+tab1_width+3, sy+2);
	
	Gr->SetColor(RGB565CONVERT(199, 199, 199));	
	Gr->Plot(sx+tab1_width, sy+3);
	Gr->SetColor(RGB565CONVERT(214, 214, 214));	
	Gr->Plot(sx+tab1_width+1, sy+3);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+tab1_width+2, sy+3);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+tab1_width+3, sy+3);
	//===========================================TAB3

	sx = tab3_sx+2;
	sy = tab3_sy;

	if(active==3)
	{
		//TAB3 살리기	
		Gr->SetColor(back_color);
		Gr->FillRect(sx+tab1_width+tab2_width+4, sy, tab3_width-4, tab3_height+10);	
	}
	else
	{
		//TAB3 높이줄이기
		sy+=2;
		tab3_height-=2;
		Gr->SetColor(back_color);
		Gr->FillRect(sx+tab1_width+tab2_width+1, sy, tab3_width, tab3_height);		
	}	

	//탭상단 탭번호 3 
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+tab1_width+tab2_width+3, sy, tab3_width-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+tab1_width+tab2_width+1, sy+1, tab3_width-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+tab1_width+tab2_width, sy+2, tab3_width-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->HLine(sx+tab1_width+tab2_width, sy+3, tab3_width-3);	

	//맨위의 탭좌측 탭번호 3
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+tab1_width+tab2_width, sy+2, tab3_height-4);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+tab1_width+tab2_width+1, sy+1, tab3_height-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+tab1_width+tab2_width+2, sy+1, tab3_height-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+tab1_width+tab2_width+3, sy, tab3_height);
	
	//탭우측 탭번호 3
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->VLine(sx+tab1_width+tab2_width+tab3_width, sy, tab3_height);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->VLine(sx+tab1_width+tab2_width+tab3_width-1, sy+1, tab3_height-1);
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->VLine(sx+tab1_width+tab2_width+tab3_width-2, sy+2, tab3_height-2);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+tab1_width+tab2_width+tab3_width-3, sy+3, tab3_height-3);

	
	//맨위 탭좌상단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(198, 198, 198));	
	Gr->Plot(sx+tab1_width+tab2_width+3, sy);
	
	Gr->SetColor(RGB565CONVERT(206, 206, 206));	
	Gr->Plot(sx+tab1_width+tab2_width+1, sy+1);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+tab1_width+tab2_width+2, sy+1);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+tab1_width+tab2_width+3, sy+1);
	
	Gr->SetColor(RGB565CONVERT(194, 194, 194));	
	Gr->Plot(sx+tab1_width+tab2_width, sy+2);
	Gr->SetColor(RGB565CONVERT(225, 225, 225));	
	Gr->Plot(sx+tab1_width+tab2_width+1, sy+2);
	Gr->SetColor(RGB565CONVERT(249, 249, 249));	
	Gr->Plot(sx+tab1_width+tab2_width+2, sy+2);
	Gr->SetColor(RGB565CONVERT(223, 223, 223));	
	Gr->Plot(sx+tab1_width+tab2_width+3, sy+2);
	
	Gr->SetColor(RGB565CONVERT(199, 199, 199));	
	Gr->Plot(sx+tab1_width+tab2_width, sy+3);
	Gr->SetColor(RGB565CONVERT(214, 214, 214));	
	Gr->Plot(sx+tab1_width+tab2_width+1, sy+3);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+tab1_width+tab2_width+2, sy+3);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+tab1_width+tab2_width+3, sy+3);
	
	SetFontAliasing("ng_eh_24.sif");
	Gr->TextOutAliasing(sx, sy, "Tab1", clBlack, 0, "ng_eh_24.sif");	
}

//선이굵은 패널을 이용해 탭을그릴때 사용한다. (수직탭)
void TabPanelDrawRow(Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	int	tab1_width = 30;
	int	tab1_height = 70;

	int	tab2_width = 30;
	int	tab2_height = 50;
	
	int	tab3_width = 30;
	int	tab3_height = 70;
	int	back_color = RGB565CONVERT(181, 181, 181);
	
	int	tab1_sx = sx;
	int	tab2_sx = sx;
	int	tab3_sx = sx;
	int	tab1_sy = sy;
	int	tab2_sy = sy;
	int	tab3_sy = sy;
	int	active=3;	//TAB1 을 액티브시킨다.
	
	// System Color로  사각영역을 그린다(채운다).
	Gr->SetColor(back_color);
	Gr->FillRect(sx, sy, win->width, win->height);	
	//===========================================BODY	
	//본체좌측
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+tab1_width, sy, height);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+tab1_width+1, sy, height);
	Gr->SetColor(RGB565CONVERT(201, 201, 201));
	Gr->VLine(sx+tab1_width+2, sy, height);
	Gr->SetColor(RGB565CONVERT(195, 195, 195));
	Gr->VLine(sx+tab1_width+3, sy, height);
	
	//상단
	Gr->SetColor(RGB565CONVERT(191, 191, 191));
	Gr->HLine(sx+3, sy, width-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+1, sy+1, width-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx, sy+2, width-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->HLine(sx, sy+3, width-3);
	//우측
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->VLine(sx+width, sy, height);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->VLine(sx+width-1, sy+1, height);
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->VLine(sx+width-2, sy+2, height);
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->VLine(sx+width-3, sy+3, height);
	
	//제일끝하단
	Gr->SetColor(RGB565CONVERT(100, 100, 100));
	Gr->HLine(sx+tab1_width, sy+height, width);
	Gr->SetColor(RGB565CONVERT(137, 137, 137));
	Gr->HLine(sx+tab1_width+1, sy+height-1, width-2);	
	Gr->SetColor(RGB565CONVERT(151, 151, 151));
	Gr->HLine(sx+tab1_width+2, sy+height-2, width-4);	
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->HLine(sx+tab1_width+3, sy+height-3, width-6);	

	//제일끝하단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(171, 171, 171));	
	Gr->Plot(sx+tab1_width, sy+height-3);
	Gr->SetColor(RGB565CONVERT(187, 187, 187));	
	Gr->Plot(sx+tab1_width+1, sy+height-3);
	Gr->SetColor(RGB565CONVERT(184, 184, 184));	
	Gr->Plot(sx+tab1_width+2, sy+height-3);
	Gr->SetColor(RGB565CONVERT(178, 178, 178));	
	Gr->Plot(sx+tab1_width+3, sy+height-3);

	Gr->SetColor(RGB565CONVERT(142, 142, 142));	
	Gr->Plot(sx+tab1_width, sy+height-2);
	Gr->SetColor(RGB565CONVERT(174, 174, 174));	
	Gr->Plot(sx+tab1_width+1, sy+height-2);
	Gr->SetColor(RGB565CONVERT(170, 170, 170));	
	Gr->Plot(sx+tab1_width+2, sy+height-2);
	Gr->SetColor(RGB565CONVERT(164, 164, 164));	
	Gr->Plot(sx+tab1_width+3, sy+height-2);

	Gr->SetColor(RGB565CONVERT(125, 125, 125));	
	Gr->Plot(sx+tab1_width+1, sy+height-1);
	Gr->SetColor(RGB565CONVERT(153, 153, 153));	
	Gr->Plot(sx+tab1_width+2, sy+height-1);
	Gr->SetColor(RGB565CONVERT(147, 147, 147));	
	Gr->Plot(sx+tab1_width+3, sy+height-1);

	Gr->SetColor(RGB565CONVERT(99, 99, 99));	
	Gr->Plot(sx+tab1_width+3, sy+height);
	//===========================================TAB1
	sx = tab1_sx;
	sy = tab1_sy;
	int space=0;

	if(active==1)
	{
		//TAB1 살리기	
		Gr->SetColor(back_color);
		Gr->FillRect(sx, sy+4, tab1_width+10, tab1_height-4);	
	}
	else
	{
		//TAB1 높이줄이기
		sx+=2;
		tab1_width-=2;
		space=5;
		Gr->SetColor(back_color);
		Gr->FillRect(sx, sy, tab1_width, tab1_height);		
	}	

	//맨위의 탭상단 탭번호 1  액티브되어있을때의 화면이다.
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+3, sy+space, tab1_width-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+1, sy+space+1, tab1_width-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx, sy+space+2, tab1_width-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->HLine(sx, sy+space+3, tab1_width-3);	
	
	//맨위의 탭좌측 탭번호 1
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx, sy+space+2, tab1_height-4-space);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+1, sy+space+1, tab1_height-2-space);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+2, sy+space+1, tab1_height-2-space);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+3, sy+space, tab1_height-space);
	//맨위의 탭하단 탭번호 1
	Gr->SetColor(RGB565CONVERT(168, 168, 168));
	Gr->HLine(sx, sy+tab1_height-3, tab1_width);
	Gr->SetColor(RGB565CONVERT(155, 155, 155));
	Gr->HLine(sx, sy+tab1_height-2, tab1_width);	
	Gr->SetColor(RGB565CONVERT(140, 140, 140));
	Gr->HLine(sx+1, sy+tab1_height-1, tab1_width-1);	
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->HLine(sx+3, sy+tab1_height, tab1_width-3);	
	
	
	//맨위 탭좌상단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(198, 198, 198));	
	Gr->Plot(sx+3, sy+space);
	
	Gr->SetColor(RGB565CONVERT(206, 206, 206));	
	Gr->Plot(sx+1, sy+space+1);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+2, sy+space+1);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+3, sy+space+1);
	
	Gr->SetColor(RGB565CONVERT(194, 194, 194));	
	Gr->Plot(sx, sy+space+2);
	Gr->SetColor(RGB565CONVERT(225, 225, 225));	
	Gr->Plot(sx+1, sy+space+2);
	Gr->SetColor(RGB565CONVERT(249, 249, 249));	
	Gr->Plot(sx+2, sy+space+2);
	Gr->SetColor(RGB565CONVERT(223, 223, 223));	
	Gr->Plot(sx+3, sy+space+2);
	
	Gr->SetColor(RGB565CONVERT(199, 199, 199));	
	Gr->Plot(sx, sy+space+3);
	Gr->SetColor(RGB565CONVERT(214, 214, 214));	
	Gr->Plot(sx+1, sy+space+3);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+2, sy+space+3);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+3, sy+space+3);
	
	//맨위 탭좌하단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(171, 171, 171));	
	Gr->Plot(sx, sy+height-3);
	Gr->SetColor(RGB565CONVERT(187, 187, 187));	
	Gr->Plot(sx+1, sy+height-3);
	Gr->SetColor(RGB565CONVERT(184, 184, 184));	
	Gr->Plot(sx+2, sy+height-3);
	Gr->SetColor(RGB565CONVERT(178, 178, 178));	
	Gr->Plot(sx+3, sy+height-3);

	Gr->SetColor(RGB565CONVERT(142, 142, 142));	
	Gr->Plot(sx, sy+height-2);
	Gr->SetColor(RGB565CONVERT(174, 174, 174));	
	Gr->Plot(sx+1, sy+height-2);
	Gr->SetColor(RGB565CONVERT(170, 170, 170));	
	Gr->Plot(sx+2, sy+height-2);
	Gr->SetColor(RGB565CONVERT(164, 164, 164));	
	Gr->Plot(sx+3, sy+height-2);

	Gr->SetColor(RGB565CONVERT(125, 125, 125));	
	Gr->Plot(sx+1, sy+height-1);
	Gr->SetColor(RGB565CONVERT(153, 153, 153));	
	Gr->Plot(sx+2, sy+height-1);
	Gr->SetColor(RGB565CONVERT(147, 147, 147));	
	Gr->Plot(sx+3, sy+height-1);

	Gr->SetColor(RGB565CONVERT(99, 99, 99));	
	Gr->Plot(sx+3, sy+height);
	
	//===========================================TAB2
	sx = tab2_sx;
	sy = tab2_sy;
	if(active==2)
	{	
		//TAB2 살리기
		Gr->SetColor(back_color);
		Gr->FillRect(sx, sy+tab1_height+4, tab2_width+10, tab2_height-4);
	}
	else
	{
		//TAB2 높이줄이기
		sx+=2;
		tab2_width-=2;
	}	
	
	//두번째 탭상단 탭번호 2
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+3, sy+tab1_height, tab2_width-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+1, sy+tab1_height+1, tab2_width-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx, sy+tab1_height+2, tab2_width-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->HLine(sx, sy+tab1_height+3, tab2_width-3);
	
	//두번째 탭좌측 탭번호 2
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx, sy+tab1_height+2, tab2_height-4);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+1, sy+tab1_height+1, tab2_height-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+2, sy+tab1_height+1, tab2_height-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+3, sy+tab1_height, tab2_height);
	//두번째 탭하단 탭번호 2
	Gr->SetColor(RGB565CONVERT(168, 168, 168));
	Gr->HLine(sx, sy+tab1_height+tab2_height-3, tab2_width);
	Gr->SetColor(RGB565CONVERT(155, 155, 155));
	Gr->HLine(sx, sy+tab1_height+tab2_height-2, tab2_width);	
	Gr->SetColor(RGB565CONVERT(140, 140, 140));
	Gr->HLine(sx+1, sy+tab1_height+tab2_height-1, tab2_width-1);	
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->HLine(sx+3, sy+tab1_height+tab2_height, tab2_width-3);	
	
	//두번째 탭좌상단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(198, 198, 198));	
	Gr->Plot(sx+3, sy+tab1_height);
	
	Gr->SetColor(RGB565CONVERT(206, 206, 206));	
	Gr->Plot(sx+1, sy+tab1_height+1);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+2, sy+tab1_height+1);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+3, sy+tab1_height+1);
	
	Gr->SetColor(RGB565CONVERT(194, 194, 194));	
	Gr->Plot(sx, sy+tab1_height+2);
	Gr->SetColor(RGB565CONVERT(225, 225, 225));	
	Gr->Plot(sx+1, sy+tab1_height+2);
	Gr->SetColor(RGB565CONVERT(249, 249, 249));	
	Gr->Plot(sx+2, sy+tab1_height+2);
	Gr->SetColor(RGB565CONVERT(223, 223, 223));	
	Gr->Plot(sx+3, sy+tab1_height+2);
	
	Gr->SetColor(RGB565CONVERT(199, 199, 199));	
	Gr->Plot(sx, sy+tab1_height+3);
	Gr->SetColor(RGB565CONVERT(214, 214, 214));	
	Gr->Plot(sx+1, sy+tab1_height+3);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+2, sy+tab1_height+3);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+3, sy+tab1_height+3);

	//===========================================TAB3
	sx = tab3_sx;
	sy = tab3_sy;
	if(active==3)
	{	
		//TAB3 살리기
		Gr->SetColor(back_color);
		Gr->FillRect(sx, sy+tab1_height+tab2_height+4, tab3_width+10, tab3_height-4);
	}
	else
	{
		//TAB3 높이줄이기
		sx+=2;
		tab3_width-=2;
	}
	//세번째 탭상단 탭번호 3
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+3, sy+tab1_height+tab2_height, tab3_width-3);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx+1, sy+tab1_height+tab2_height+1, tab3_width-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->HLine(sx, sy+tab1_height+tab2_height+2, tab3_width-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->HLine(sx, sy+tab1_height+tab2_height+3, tab3_width-3);

	//세번째 탭좌측 탭번호 3
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx, sy+tab2_height+tab1_height+2, tab3_height-4);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+1, sy+tab2_height+tab1_height+1, tab3_height-2);
	Gr->SetColor(RGB565CONVERT(200, 200, 200));
	Gr->VLine(sx+2, sy+tab2_height+tab1_height+1, tab3_height-2);
	Gr->SetColor(RGB565CONVERT(194, 194, 194));
	Gr->VLine(sx+3, sy+tab2_height+tab1_height, tab3_height);
	//세번째 탭하단 탭번호 3
	Gr->SetColor(RGB565CONVERT(168, 168, 168));
	Gr->HLine(sx, sy+tab1_height+tab2_height+tab3_height-3, tab3_width);
	Gr->SetColor(RGB565CONVERT(155, 155, 155));
	Gr->HLine(sx, sy+tab1_height+tab2_height+tab3_height-2, tab3_width);	
	Gr->SetColor(RGB565CONVERT(140, 140, 140));
	Gr->HLine(sx+1, sy+tab1_height+tab2_height+tab3_height-1, tab3_width-1);	
	Gr->SetColor(RGB565CONVERT(166, 166, 166));
	Gr->HLine(sx+3, sy+tab1_height+tab2_height+tab3_height, tab3_width-3);	
	//세번째 탭좌상단 안티앨리어싱처리
	Gr->SetColor(RGB565CONVERT(198, 198, 198));	
	Gr->Plot(sx+3, sy+tab1_height+tab2_height);
	
	Gr->SetColor(RGB565CONVERT(206, 206, 206));	
	Gr->Plot(sx+1, sy+tab1_height+tab2_height+1);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+2, sy+tab1_height+tab2_height+1);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+3, sy+tab1_height+tab2_height+1);
	
	Gr->SetColor(RGB565CONVERT(194, 194, 194));	
	Gr->Plot(sx, sy+tab1_height+tab2_height+2);
	Gr->SetColor(RGB565CONVERT(225, 225, 225));	
	Gr->Plot(sx+1, sy+tab1_height+tab2_height+2);
	Gr->SetColor(RGB565CONVERT(249, 249, 249));	
	Gr->Plot(sx+2, sy+tab1_height+tab2_height+2);
	Gr->SetColor(RGB565CONVERT(223, 223, 223));	
	Gr->Plot(sx+3, sy+tab1_height+tab2_height+2);
	
	Gr->SetColor(RGB565CONVERT(199, 199, 199));	
	Gr->Plot(sx, sy+tab1_height+tab2_height+3);
	Gr->SetColor(RGB565CONVERT(214, 214, 214));	
	Gr->Plot(sx+1, sy+tab1_height+tab2_height+3);
	Gr->SetColor(RGB565CONVERT(224, 224, 224));	
	Gr->Plot(sx+2, sy+tab1_height+tab2_height+3);
	Gr->SetColor(RGB565CONVERT(213, 213, 213));	
	Gr->Plot(sx+3, sy+tab1_height+tab2_height+3);

	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
}

//밝은회색용
int LightGray_LeftColor[] = { 
	RGB565CONVERT(138, 138, 138),
	RGB565CONVERT(155, 155, 155),
	RGB565CONVERT(162, 162, 162),
	RGB565CONVERT(164, 164, 164),
	RGB565CONVERT(167, 167, 167),
	RGB565CONVERT(180, 180, 180),
	RGB565CONVERT(195, 195, 195),
	RGB565CONVERT(208, 208, 208),
	RGB565CONVERT(208, 208, 208),
	RGB565CONVERT(205, 205, 205)
};
int LightGray_UpColor[] = { 
	RGB565CONVERT(194, 194, 194),
	RGB565CONVERT(208, 208, 208),
	RGB565CONVERT(210, 210, 210),
	RGB565CONVERT(211, 211, 211),
	RGB565CONVERT(213, 213, 213),
	RGB565CONVERT(223, 223, 223),
	RGB565CONVERT(237, 237, 237),
	RGB565CONVERT(248, 248, 248),
	RGB565CONVERT(235, 235, 235),
	RGB565CONVERT(204, 204, 204)
};	

int LightGray_LightColor[] = { 
	RGB565CONVERT(69, 69, 69),
	RGB565CONVERT(102, 102, 102),
	RGB565CONVERT(112, 112, 112),
	RGB565CONVERT(114, 114, 114),
	RGB565CONVERT(118, 118, 118),
	RGB565CONVERT(138, 138, 138),
	RGB565CONVERT(161, 161, 161),
	RGB565CONVERT(183, 183, 183),
	RGB565CONVERT(193, 193, 193),
	RGB565CONVERT(201, 201, 201)
};	
int LightGray_BottomColor[] = { 
	RGB565CONVERT(141, 141, 141),
	RGB565CONVERT(102, 102, 102),
	RGB565CONVERT(101, 101, 101),
	RGB565CONVERT(105, 105, 105),
	RGB565CONVERT(111, 111, 111),
	RGB565CONVERT(133, 133, 133),
	RGB565CONVERT(158, 158, 158),
	RGB565CONVERT(181, 181, 181),
	RGB565CONVERT(192, 192, 192),
	RGB565CONVERT(200, 200, 200)
};	

//밝은블루용
int LightBlue_LeftColor[] = { 
	RGB565CONVERT(101, 161, 183),
	RGB565CONVERT(113, 183, 207),
	RGB565CONVERT(118, 191, 217),
	RGB565CONVERT(119, 193, 219),
	RGB565CONVERT(122, 197, 223),
	RGB565CONVERT(132, 212, 235),
	RGB565CONVERT(144, 229, 249),
	RGB565CONVERT(154, 244, 255),
	RGB565CONVERT(154, 244, 255),
	RGB565CONVERT(151, 241, 255)
};

int LightBlue_UpColor[] = { 
	RGB565CONVERT(144, 229, 247),
	RGB565CONVERT(152, 245, 255),
	RGB565CONVERT(153, 248, 255),
	RGB565CONVERT(154, 249, 255),
	RGB565CONVERT(156, 251, 255),
	RGB565CONVERT(167, 255, 255),
	RGB565CONVERT(181, 255, 255),
	RGB565CONVERT(192, 255, 255),
	RGB565CONVERT(178, 255, 255),
	RGB565CONVERT(159, 248, 255)
};	

int LightBlue_LightColor[] = { 
	RGB565CONVERT(51, 82, 93),
	RGB565CONVERT(74, 120, 136),
	RGB565CONVERT(81, 132, 148),
	RGB565CONVERT(83, 134, 153),
	RGB565CONVERT(86, 140, 159),
	RGB565CONVERT(101, 163, 185),
	RGB565CONVERT(117, 190, 216),
	RGB565CONVERT(133, 216, 245),
	RGB565CONVERT(253, 227, 141),
	RGB565CONVERT(147, 236, 255)
};	
int LightBlue_BottomColor[] = { 
	RGB565CONVERT(109, 160, 178),
	RGB565CONVERT(77, 118, 133),
	RGB565CONVERT(76, 118, 132),
	RGB565CONVERT(79, 123, 138),
	RGB565CONVERT(84, 130, 146),
	RGB565CONVERT(99, 156, 175),
	RGB565CONVERT(117, 185, 209),
	RGB565CONVERT(134, 213, 241),
	RGB565CONVERT(142, 226, 252),
	RGB565CONVERT(147, 235, 255)
};	

void SystemTextBoxDraw(int  press, Windows *win, Graphics *Gr, int sx, int sy);
//선이겁나게 굵은 패널을 그릴때 사용한다.(파워포인트에 있는모양)
void PowerPanelDraw(int press, Windows *win, Graphics *Gr, int sx, int sy, int outline)
{
	int	width = win->width-1;
	int	height = win->height-1;
	int 	*left_color;
	int 	*up_color;
	int 	*light_color;
	int 	*bottom_color;
	
	if(win->BackColor==GRAY || win->BackColor==clWhite || win->BackColor==clYellow)
	{
		left_color = LightGray_LeftColor;
		up_color = LightGray_UpColor;
		light_color = LightGray_LightColor;
		bottom_color = LightGray_BottomColor;
		Gr->SetColor(RGB565CONVERT(204, 204, 204));
	}
	else if(win->BackColor==BLUE)
	{
		left_color = LightBlue_LeftColor;
		up_color = LightBlue_UpColor;
		light_color = LightBlue_LightColor;
		bottom_color = LightBlue_BottomColor;
		Gr->SetColor(RGB565CONVERT(150, 239, 255));
	}	
	Gr->FillRect(sx, sy, win->width, win->height);	
	if(press==OFF)
	{	
		// 좌측
		Gr->SetColor(left_color[0]);
		Gr->VLine(sx, sy, height);
		Gr->SetColor(left_color[1]);
		Gr->VLine(sx+1, sy+1, height-2);
		Gr->SetColor(left_color[2]);
		Gr->VLine(sx+2, sy+2, height-4);
		Gr->SetColor(left_color[3]);
		Gr->VLine(sx+3, sy+3, height-6);
		Gr->SetColor(left_color[4]);
		Gr->VLine(sx+3, sy+4, height-8);
		Gr->SetColor(left_color[5]);
		Gr->VLine(sx+3, sy+5, height-10);
		Gr->SetColor(left_color[6]);
		Gr->VLine(sx+3, sy+6, height-12);
		Gr->SetColor(left_color[7]);
		Gr->VLine(sx+3, sy+7, height-14);
		Gr->SetColor(left_color[8]);
		Gr->VLine(sx+3, sy+8, height-16);
		Gr->SetColor(left_color[9]);
		Gr->VLine(sx+3, sy+9, height-18);

		//상단
		Gr->SetColor(up_color[0]);
		Gr->HLine(sx, sy, width);
		Gr->SetColor(up_color[1]);
		Gr->HLine(sx+1, sy+1, width-2);
		Gr->SetColor(up_color[2]);
		Gr->HLine(sx+2, sy+2, width-4);
		Gr->SetColor(up_color[3]);
		Gr->HLine(sx+3, sy+3, width-6);
		Gr->SetColor(up_color[4]);
		Gr->HLine(sx+4, sy+4, width-8);
		Gr->SetColor(up_color[5]);
		Gr->HLine(sx+5, sy+5, width-10);
		Gr->SetColor(up_color[6]);
		Gr->HLine(sx+6, sy+6, width-12);
		Gr->SetColor(up_color[7]);
		Gr->HLine(sx+7, sy+7, width-14);
		Gr->SetColor(up_color[8]);
		Gr->HLine(sx+8, sy+8, width-16);
		Gr->SetColor(up_color[9]);
		Gr->HLine(sx+9, sy+9, width-18);

		//우측
		Gr->SetColor(light_color[0]);
		Gr->VLine(sx+width, sy, height);
		Gr->SetColor(light_color[1]);
		Gr->VLine(sx+width-1, sy+1, height-2);
		Gr->SetColor(light_color[2]);
		Gr->VLine(sx+width-2, sy+2, height-4);
		Gr->SetColor(light_color[3]);
		Gr->VLine(sx+width-3, sy+3, height-6);
		Gr->SetColor(light_color[4]);
		Gr->VLine(sx+width-4, sy+4, height-8);
		Gr->SetColor(light_color[5]);
		Gr->VLine(sx+width-5, sy+5, height-10);
		Gr->SetColor(light_color[6]);
		Gr->VLine(sx+width-6, sy+6, height-12);
		Gr->SetColor(light_color[7]);
		Gr->VLine(sx+width-7, sy+7, height-14);
		Gr->SetColor(light_color[8]);
		Gr->VLine(sx+width-8, sy+8, height-16);
		Gr->SetColor(light_color[9]);
		Gr->VLine(sx+width-9, sy+9, height-18);

		//하단
		Gr->SetColor(bottom_color[0]);
		Gr->HLine(sx, sy+height, width);
		Gr->SetColor(bottom_color[1]);
		Gr->HLine(sx+1, sy+height-1, width-2);	
		Gr->SetColor(bottom_color[2]);
		Gr->HLine(sx+2, sy+height-2, width-4);	
		Gr->SetColor(bottom_color[3]);
		Gr->HLine(sx+3, sy+height-3, width-6);	
		Gr->SetColor(bottom_color[4]);
		Gr->HLine(sx+4, sy+height-4, width-8);	
		Gr->SetColor(bottom_color[5]);
		Gr->HLine(sx+5, sy+height-5, width-10);	
		Gr->SetColor(bottom_color[6]);
		Gr->HLine(sx+6, sy+height-6, width-12);	
		Gr->SetColor(bottom_color[7]);
		Gr->HLine(sx+7, sy+height-7, width-14);	
		Gr->SetColor(bottom_color[8]);
		Gr->HLine(sx+8, sy+height-8, width-16);	
		Gr->SetColor(bottom_color[9]);
		Gr->HLine(sx+9, sy+height-9, width-18);	
	}
	else
	{
		SystemTextBoxDraw(ON, win, Gr, sx, sy);
	}
	
	if(outline==1)
	{
		Gr->SetColor(clBlack);
		Gr->Rectangle(sx, sy, width, height);			
	}
}


#ifdef __cplusplus
}
#endif
