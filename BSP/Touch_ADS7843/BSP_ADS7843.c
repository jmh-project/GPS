/******************************************************************************
*    	File name	: BSP_ADS7843.c
*		Company		:
*		Version		:
*		Data		:
*		Author		:
*		Contact		:
******************************************************************************/



/******************************************************************************
							Include File
******************************************************************************/
#include "Teeco_System.h"
#include "BSP_ADS7843.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "Debug_Uart.h"
//#include "TouchCalibration.h"

volatile int	g_XMIN;		// X축 보정시좌표 최소점 mV
volatile int 	g_XMAX;		// X축 보정시좌표 최대점 mV
volatile int	g_YMIN;		// Y축 보정시좌표 최소점 mV
volatile int	g_YMAX;		// Y축 보정시좌표 최대점 mV

volatile int 	gTouchX, gTouchY, gPress;
volatile int	g_TouchCalStatus;
volatile int 	g_TouchAverX[5];
volatile int 	g_TouchAverY[5];	 
/* Exported constants --------------------------------------------------------*/	 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/	 
void Touch_Cal(void);
void Touch_Test(void);	 

void Delay(__IO uint32_t nTime);

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define		TOUCH_DELAY		1000
/******************************************************************************
							Private & Local Variables
******************************************************************************/
extern volatile int 	gTouchX, gTouchY;

/******************************************************************************
							Function Prototype
******************************************************************************/
void BSP_ADS7843_TouchRead( void );
U16 BSP_ADS7843_ReadTouchPoint( U32 Point );

/******************************************************************************
		설명 : 		ADS7843 IOPORT 초기화 
		전달인자 : 	없음  
		리턴값 :	없음 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
void BSP_ADS7843_PortInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);				
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);				

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	//T-CLK, T-CS,
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOF, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_10;	//T-IRQ, T-BUSY
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOF, &GPIO_InitStructure);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//T-DIN
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_ResetBits( GPIOF, GPIO_Pin_7 );		//CLK Pin Default Low
	BSP_ADS7843_ReadTouchPoint(XPOINT);
	BSP_ADS7843_ReadTouchPoint(XPOINT);
	//EXTILine6_Config();													
}
/******************************************************************************
		설명 :		AD7843 전용 Delay 정확한 값은 아니다.
		전달인자 :	카운터 값
		리턴값 :	없음 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
void BSP_ADS7843_ForDelay( U32 Count )
{
	while( Count-- );
}
/******************************************************************************
		설명 : 		ADS7843 CS PIN
		전달인자 :	ON/OFF 제어값 
		리턴값 :	없음 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
void BSP_ADS7843_CS( U32 Status)
{
 	if(Status == 1) GPIO_SetBits( GPIOF, GPIO_Pin_8 );
	else 			GPIO_ResetBits( GPIOF, GPIO_Pin_8 );		
}
/******************************************************************************
		설명 :		ADS7843 CLOCK PIN 
		전달인자 :	없음 
		리턴값 :	없음 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
void BSP_ADS7843_CLK( void )
{
 	GPIO_SetBits( GPIOF, GPIO_Pin_7 );
	BSP_ADS7843_ForDelay( TOUCH_DELAY );
	GPIO_ResetBits( GPIOF, GPIO_Pin_7 );		
}
/******************************************************************************
		설명 : 		ADS7843 DOUT PIN
		전달인자 :	ON/OFF 제어값 
		리턴값 :	NONE
		수정이력 :	NONE
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
void BSP_ADS7843_DOUT( U32 Status)
{
 	if(Status == 1) GPIO_SetBits( GPIOF, GPIO_Pin_9 );
	else 			GPIO_ResetBits( GPIOF, GPIO_Pin_9 );		
}
/******************************************************************************
		설명 :		ADS7843 DIN PIN 
		전달인자 :	없음
		리턴값 :	포트 레벨을 리턴한다.
		수정이력 :	없음
		작성일시 : 	12/05/22
		Note : 
******************************************************************************/
U32 BSP_ADS7843_DIN( void )
{
	if(GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_0 )) 	return 1;
	else 											return 0;	
}
/******************************************************************************
		설명 :		ADS7843 BUSY PIN 
		전달인자 :	없음 
		리턴값 :  	포트 레벨을 리턴한다.
		수정이력 : 	없음
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
U32 BSP_ADS7843_BUSY( void )
{
	if(GPIO_ReadInputDataBit( GPIOF, GPIO_Pin_10 ))	return 1;
	else 											return 0;	
}
/******************************************************************************
		설명 :		ADS7843 IRQ PIN 
		전달인자 :	없음 
		리턴값 :	포트 레벨을 리턴한다. 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 		
******************************************************************************/
U32 BSP_ADS7843_IRQ( void )
{
	if(GPIO_ReadInputDataBit( GPIOF, GPIO_Pin_6 )) 	return 1;
	else 											return 0;	
}
/******************************************************************************
		설명 : 		ADS7843 터치값을 읽어 들인다.
		전달인자 :	xpoint, ypoint 선택한다.
		리턴값 : 	전달 인자에 따른 좌표값 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 
******************************************************************************/
U16 BSP_ADS7843_ReadTouchPoint( U32 Point )
{
	U16	data = 0;
	int	loop=0;

	BSP_ADS7843_CS( LOW );
	BSP_ADS7843_ForDelay(TOUCH_DELAY);

	BSP_ADS7843_DOUT( HIGH );//Start signal
	BSP_ADS7843_CLK();	
	if( Point == XPOINT )
	{
		BSP_ADS7843_DOUT( HIGH );	//A2
		BSP_ADS7843_CLK();	
		BSP_ADS7843_DOUT( LOW );	//A1
		BSP_ADS7843_CLK();	
		BSP_ADS7843_DOUT( HIGH );	//A0
		BSP_ADS7843_CLK();	

		BSP_ADS7843_DOUT( LOW );	// LO:12Bit  HI:8Bit
		BSP_ADS7843_CLK();			// Clock Gen.5
		BSP_ADS7843_DOUT( LOW );	// LO:DFR  HI:SFR
		BSP_ADS7843_CLK();			// Clock Gen.6
		BSP_ADS7843_DOUT( LOW );	// POWER DOWN MODE
		BSP_ADS7843_CLK();			// Clock Gen.7
		BSP_ADS7843_DOUT( LOW );	// POWER DOWN MODE
		BSP_ADS7843_CLK();			// Clock Gen.8
	}
	else
	{
		BSP_ADS7843_DOUT( LOW );	//A2
		BSP_ADS7843_CLK();	
		BSP_ADS7843_DOUT( LOW );	//A1
		BSP_ADS7843_CLK();	
		BSP_ADS7843_DOUT( HIGH );	//A0
		BSP_ADS7843_CLK();	

		BSP_ADS7843_DOUT( LOW );	// LO:12Bit  HI:8Bit
		BSP_ADS7843_CLK();			// Clock Gen.5
		BSP_ADS7843_DOUT( HIGH );	// LO:DFR  HI:SFR
		BSP_ADS7843_CLK();			// Clock Gen.6
		BSP_ADS7843_DOUT( LOW );	// POWER DOWN MODE
		BSP_ADS7843_CLK();			// Clock Gen.7
		BSP_ADS7843_DOUT( LOW );	// POWER DOWN MODE
		BSP_ADS7843_CLK();			// Clock Gen.8
	}
	while(1)//터치 에러발생시 타이머로 루프를 빠져나가야 한다.
	{
		loop++;
   		if( BSP_ADS7843_BUSY() )
		{
		 	BSP_ADS7843_CLK();
			break;
		}
		if(loop > 10000) 
		{
			Dprintf("Touch Chip BUSY Error\n");
			break;
		}
	}
 	if(	BSP_ADS7843_DIN() )	data |= 0x0800;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0400;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0200;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0100;
	BSP_ADS7843_CLK();

	if(	BSP_ADS7843_DIN() )	data |= 0x0080;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0040;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0020;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0010;
	BSP_ADS7843_CLK();

	if(	BSP_ADS7843_DIN() )	data |= 0x0008;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0004;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0002;
	BSP_ADS7843_CLK();
	if(	BSP_ADS7843_DIN() )	data |= 0x0001;
	BSP_ADS7843_CLK();
	
	BSP_ADS7843_CLK();
	BSP_ADS7843_CLK();
	BSP_ADS7843_CLK();

	BSP_ADS7843_CS( HIGH );
	return ( data );
}
/******************************************************************************
		설명 :		터치 상태를 체크하여 터치가 되면 터치값을 읽는다. 
		전달인자 :	없음
		리턴값 :	터치값이 전역 구조체에 저장된다. 
		수정이력 :	없음 
		작성일시 :	12/05/22
		Note : 		
******************************************************************************/
int BSP_ADS7843_Read( void )
{
// 	U32		gx, gy;
	static int toggle=1;
	
	if( BSP_ADS7843_IRQ()==0 )
	{
		gTouchY = BSP_ADS7843_ReadTouchPoint( YPOINT );
	 	gTouchX = BSP_ADS7843_ReadTouchPoint( XPOINT );
		if(toggle==0)	toggle = 1;
	}
	else if( BSP_ADS7843_IRQ() )
	{
		if(toggle==1)  toggle = 0;
	}
	Dprintf("X=%d Y=%d\n", gTouchX, gTouchY);
	//Delay(100);
	return(0);
}

int TouchRead(void)
{
	return(1);
}
#ifdef ADS7843
int BSP_TouchRead( void )
{
	//static int aver_cnt=0;
	int		xdata, ydata;
	int		averx, avery;
//	int		read_flag=0;

	if( BSP_ADS7843_IRQ()==0 )
	{
#ifdef LCD_SAMSUNG_LMS700KF06        
		ydata = BSP_ADS7843_ReadTouchPoint( YPOINT );
	 	xdata = BSP_ADS7843_ReadTouchPoint( XPOINT );
#else
		xdata = BSP_ADS7843_ReadTouchPoint( YPOINT );
	 	ydata = BSP_ADS7843_ReadTouchPoint( XPOINT );
        
#endif        
		averx=xdata; avery=ydata;

		if(g_TouchCalStatus==YES)
		{
			gTouchX = LCD_X * ((float)(averx-g_XMIN) / (float)(g_XMAX-g_XMIN));
			gTouchY = LCD_Y * ((float)(avery-g_YMIN) / (float)(g_YMAX-g_YMIN));	
			//Dprintf("CAL::LCD_X=%d LCD_Y=%d<xmin=%d xmax=%d ymin=%d ymax=%d > x=%d y=%d  TX=%d TY=%d ", LCD_X, LCD_Y, g_XMIN, g_XMAX, g_YMIN, g_XMAX, averx, avery, gTouchX, gTouchY);
					
			if(gTouchX > LCD_X || gTouchX < 0) 	gTouchY=gTouchX=0;
			if(gTouchY > LCD_Y || gTouchY < 0) 	gTouchX=gTouchY=0;
			gPress=ON;
			//Dprintf("CAL--2::X=%d Y=%d   ", gTouchX, gTouchY);
		}
		else
		{
			gTouchX = averx; gTouchY = avery; 
			gPress=ON;
			//Dprintf("NOCAL::X=%d Y=%d   ", gTouchX, gTouchY);
		}
	}
	else
	{
		gPress=OFF;
	}
	return(0);
}
#endif

#ifdef TSC2003
int BSP_TouchRead( void )
{
	return(0);
}
#endif


void TouchConversion(int xdata, int ydata)
{
	if(g_TouchCalStatus==YES)
	{
		gTouchX = LCD_X * ((float)(xdata-g_XMIN) / (float)(g_XMAX-g_XMIN));
		gTouchY = LCD_Y * ((float)(ydata-g_YMIN) / (float)(g_YMAX-g_YMIN));	
		
				
		if(gTouchX > LCD_X || gTouchX < 0) 	gTouchY=gTouchX=0;
		if(gTouchY > LCD_Y || gTouchY < 0) 	gTouchX=gTouchY=0;
        //Dprintf("TOUCH ::X=%d Y=%d   \n", gTouchX, gTouchY);
	}
	else
	{
		gTouchX = xdata; gTouchY = ydata; 
		Dprintf("터치교정불량::X=%d Y=%d   \n", gTouchX, gTouchY);
	}
}
