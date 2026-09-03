#include "Teeco_System.h"

//아래의 주석문은 파일을  작성할때 상단에 만들어 넣는다.
/**
        @file     	linked-list.c
        @date   	2014년 6월2일
        @auther   	SongBJ
        @brief      	윈도우자료구조체를 체인으로 연결하기위한 라이브러리
        @version    	0.01
	    @warning 	경고! 문서화       
	    @image 		html structure00.png, 문서에 그림을 포함합니다
        @attention
*/

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @defgroup ADC 
  * @brief ADC driver modules
  * @{
  */ 

//여기부터 코드를 작성하면 됩니다.


/**
  * @}
  */ 

/**
  * @}
  */


/**
        @mainpage 	Link-list Main Page
*/

/**
        ///중요 코드를 설명할때 시작 지점 가리킨다
        @code
        ///중요코드 설명할때 종료 지점 가리킨다
        @endcode
*/ 

/**
        @def MAX_BUF
        @brief 최대버퍼를 지정한다.
*/    

/** 
	    @defgroup ParentGroup 이것은 Parent Group 
	    @brief ParentGroup 에 대한 설명을 합니다. 
*/ 
/** 
	    @defgroup ChildGroup 이것은 Child Group 
	    @ingroup ParentGroup 
	    @brief ChildGroup 에 대한 설명을 합니다. 
*/ 

/** 
	    @ingroup ChildGroup 
	    @class sedna::AvatarSystem 
	    @brief sedna::AvatarSystem 에 대한 설명을 합니다. 
*/ 

#define     MAX_WIN     /**< 최대로만들 윈도우의 수 */

///시리얼포트로부터 한문자를 받아들인다.
extern int Dgetc(void);

//<!     @brief 예제용 구조체
//<!     @auther
struct {
    int     cnt;    /**< 인자갯수 */
    int     test;   //!< test용 변수
} stTest;

//아래의 주석문은 함수를 작성할때 만들어 넣는다.
/**
        @fn         int Linktest(void);
        @brief      윈도우자료구조체를 체인으로 연결하기위한 라이브러리
        @date       2014년 6월2일
        @param    진입변수없음
                @arg  멤버등의 구체적설명
                @arg  멤버등의 구체적설명
                @arg  멤버등의 구체적설명
        @return    성공유무를 나타낸다.         
        @retval     
        @see        ///참고할 함수나 페이지를 지정한다.
        @ref        ///링크를 건다.
	    @warning 경고! 문서화      
	    @todo        앞으로 할일을 문서화 합니다 
	    @image html structure00.png        
 @verbatim //문서에서 칸을친다.
 @endverbatim
*/
int Linktest(
    int  num,   
    int  cnt     /**< 만들갯수 */
)
{
	for(int i=0; i<100; i++)
	{
		//ADC_Config();
	}
    return(1);
}

struct input_event {
	uint16_t	type;
	uint16_t	code;
};



/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */
/** @defgroup ADC 
  * @brief ADC driver modules
  * @{
  */ 

/**
 @verbatim //문서에서 칸을친다.
    주로 동작설명을 할때 사용한다.
 @endverbatim
*/

int ADC_Config(void)
{
	return(10);	
}
/**
    @}
*/
/**
    @}
*/
