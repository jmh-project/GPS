#ifndef _GRAPHBOX_WIDGET
#define _GRAPHBOX_WIDGET

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "TEECO_C_Define.h"
	 
//만들어진 그래프의 X 인터벌을 변경한다.
int SetWidget_GraphInterval(int x);
	 
// 만들어진 그래프의 Y 최대값을 변경한다.
int SetWidget_GraphMaxY(float y);
	 
// 만들어진 그래프에 데이터를그리기위해 전송할때 사용하는 함수이다.
// 그래프의 위젯아이디와 표시할 데이터의 배열, 그리고 길이를 전달한다.
int SetWidget_GraphData(U32 id,  float *display_data, int data_len);
	 
U32 Graph(	HANDLE ContainerHandle, 				// 위젯윈도우이므로 컨테이너를지정해야한다.
			int 		sx, int sy, int width, int height, 	// 컨테이너안에 위젯의위치와 크기지정
			float	ymax,	// Y max 이값은 SetWidget으로 변경할수있다.
			int		x_inc );	// X축 증가값  이값은 SetWidget으로 변경할수있다.	 
#ifdef __cplusplus
}
#endif

#endif
