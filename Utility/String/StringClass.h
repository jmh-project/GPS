/**
  ******************************************************************************
  * @file    		StringClass.h
  * @author  	(주)자이언텍 기술지원팀
  * @version 	V1.0.0
  * @date    	19-December-2014
  * @brief   	문자열처리 클래스의 구현
  *          		
  ******************************************************************************
  * @attention
  *
  * 이파일은 자이언텍 지적재산권이므로 소스를 유출해서는 안됩니다.
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc</center></h2>
  ******************************************************************************
  */

#ifndef __STRING_CLASS
#define __STRING_CLASS

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "Teeco_HW_Config.h"
#include "stm32f4xx.h"	 

/** @addtogroup TEECO_BootLoader
  * @{
  */
/** @addtogroup ResourceManager
  * @{
  */	 
/* Exported types ------------------------------------------------------------*/
typedef struct {
	char	*string;
	int	(*Copy)(char *dst, const char *src);
} StringClass;	 
/* Exported variable --------------------------------------------------------*/	 
/* Exported constants --------------------------------------------------------*/	 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/	 
int stringcopy(char *dst, const char *src);
#ifdef __cplusplus
}
#endif
/**
  * @}
  */

/**
  * @}
  */
#endif /* __STRING_CLASS */
/******************* (C) COPYRIGHT 2014 ZionTech *****END OF FILE****/
