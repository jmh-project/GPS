/**
  ******************************************************************************
  * @file       	StringClass.c
  * @author  	(주)자이언텍 기술개발지원팀
  * @version 	V0.1
  * @date    	2014년 12월19일
  * @brief    	이파일은  문자열처리에관한 함수의 집합이다.
                
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
#include "StringClass.h"
#include "Debug_Uart.h"
#include <stdio.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/

	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
StringClass _String = 
{
	.Copy = stringcopy,
};

StringClass *String = &_String;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

int stringcopy(char *dst, const char *src)
{
	int	len=strlen(src);
	Dprintf("len = %d\n", len);
	for(int i=0; i<len; i++) 
	{
		*dst = *src;
		Dputc(*dst);
		dst++; src++;
	}
	return(1);
}

#if 0
void	MyTestFunc(void)
{
	char	testbuf[64];
	String->Copy(testbuf, "Test String");
	Dprintf("\nbuf=%s\n", buf);
}
#endif
#ifdef __cplusplus
}
#endif
