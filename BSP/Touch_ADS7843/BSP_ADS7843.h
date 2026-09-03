#ifndef _BSP_ADS7843
#define _BSP_ADS7843

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "TEECO_C_Define.h"

typedef struct _TouchPosition
{
	int	sx, sy;
	int ex, ey;
}stTouchPosition;

#define	HIGH	1
#define LOW		0
#define	XPOINT	1
#define YPOINT	0

void BSP_ADS7843_TouchRead( void );
void BSP_ADS7843_PortInit( void );
void BSP_ADS7843_TouchRead( void );

#ifdef __cplusplus
}
#endif

#endif
