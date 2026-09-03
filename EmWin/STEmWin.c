//#include "TEECO_System.h"
#include "Debug_Uart.h"
#include "GUI.h"
#include "WM.h"

volatile  int Arr_Test[800*600];

void 	STemWinPorting(void)
{
	//GUI_Initialized = 1;	
	/* Activate the use of memory device feature */
	WM_SetCreateFlags(WM_CF_MEMDEV);	
	Dprintf("GUI_Init Entry\n");
	GUI_Init();
	Dprintf("GUI_Init Entry Out\n");
	
	GUI_Clear();
	
	GUI_DispStringAt("1.Hello world!", 0, 0);
	GUI_SetFont(&GUI_Font20_1);
	GUI_DispStringAt("2.Hello world!", 0, 100);
	while(1);	
}
