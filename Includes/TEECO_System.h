#ifndef XMIOCM_HEADER
#define XMIOCM_HEADER

#ifdef __cplusplus
 extern "C" {
#endif

//!< 기본적인 C 표준함수	 
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include "TEECO_HW_Config.h"
#include "stm324xg_eval_sdio_sd.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_it.h"

#include "TEECO_C_Define.h"
#include "TEECO_Define.h"
//#include "stm32f4xx.h"
	 
#include "TEECO_Color.h"
#include "Windows.h"
#include "IOProcess.h"

#include "String.h"
#include "Debug_Uart.h"
#include "BSP_Init.h"
#include "BSP_ADS7843.h"
#include "BSP_Port_i2C.h"

#include "BSP_EEProm.h"
#include "BSP_SM718.h"
#include "BSP_SAA7113H.h"
#include "BSP_NAND.h"
#include "NandFileSystem.h"

#include "DataLogExtern.h"

#define		public
#define		private
#define		protected	



#ifdef MEDIA_USB_KEY
	#include "usb_hcd_int.h"
	#include "usbh_usr.h"
	#include "usbh_core.h"
	#include "usbh_msc_core.h"
#endif
#ifdef __cplusplus
}
#endif

#endif
