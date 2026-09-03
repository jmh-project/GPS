#ifndef _BSP_ENCODER
#define _BSP_ENCODER

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "TEECO_C_Define.h"

void BSP_Encoder_Portinit(void);
extern U32	Encoder_Puls;
#ifdef __cplusplus
}
#endif

#endif