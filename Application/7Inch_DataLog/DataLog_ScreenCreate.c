/******************************************************************************
*       File name	: DataLog_Main.c 
*		Description 	: 7인치 데이터로거 시작윈도우
*		Company	: ziontech co.,
*		Version		: 0.10 (2015년 4월23일 Start)
*		Contact		: www.ziontek.co.kr
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "Windows.h"
#include "DataLog.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
/******************************************************************************
							Private & Local Variables
******************************************************************************/
/******************************************************************************
							Function Prototype
******************************************************************************/
/******************************************************************************
							Example Program
******************************************************************************/
int  SmartLog_Uart12Screen_Window(HANDLE parent, HANDLE handle);
#ifdef AUTO_SAMPLER
int  SmartLog_AutoSampler_Window(HANDLE parent, HANDLE handle);
#endif
unsigned int MainScreenCreate( HANDLE parent, HANDLE handle ) {
	unsigned    int     comPort=0;
    unsigned    int     itemCount = 0;
	/**********************

	UART1 USART2 USART3 USART4 USART 5 센서 정보를 읽어들인다.

	**********************/
    //Dprintf("MSERIES  ITEMCNT = %d\n",flash.ScreenCnt[0]);
	
	
    for(itemCount=0; itemCount < 5; itemCount++)
    {
        comPort = itemCount;
        //Dprintf("UART[%d] [%d]]\n",comPort,flash.Uart[comPort]->SensorID);
        //Dprintf("MainScreenCreate :: Comport = %d ItemCnt = %d,%d\n",comPort,flash.ScreenCnt[comPort],flash.Uart[comPort]->ItemCnt);
        flash.ScreenCnt[comPort] = flash.Uart[comPort]->ItemCnt;//윈도우에 출력할 측정화면의 수를 초기화하고 아래 루틴을 통해 알아낸다.        
        if(flash.Uart[comPort]->SensorStatus == 1)
        {
            if(flash.Uart[comPort]->SensorID == SENSOR_YSI)                 {Dprintf("MainScreenCreate :: SENSOR_YSI\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_EXO)           {Dprintf("MainScreenCreate :: SENSOR_EXO\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_ADPSONTEK)      {Dprintf("MainScreenCreate :: SENSOR_ADPSONTEK\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}} 
            else if(flash.Uart[comPort]->SensorID == SENSOR_TN)             {Dprintf("MainScreenCreate :: SENSOR_TN\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_TP)             {Dprintf("MainScreenCreate :: SENSOR_TP\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_AUTO_SAMPLER)   {Dprintf("MainScreenCreate :: SENSOR_AUTO_SAMPLER\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_WIZ)            {Dprintf("MainScreenCreate :: SENSOR_WIZ\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
			else if(flash.Uart[comPort]->SensorID == SENSOR_HFTOL)          {Dprintf("MainScreenCreate :: SENSOR_HFTOL\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_UITNH4)         {Dprintf("MainScreenCreate :: SENSOR_UITNH4\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_HFCLX)          {Dprintf("MainScreenCreate :: SENSOR_HFCLX\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_PH_COND) {Dprintf("MainScreenCreate :: SENSOR_PONSEL_PH_COND\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_DO)      {Dprintf("MainScreenCreate :: SENSOR_PONSEL_DO\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_EC)      {Dprintf("MainScreenCreate :: SENSOR_PONSEL_EC\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_PH)      {Dprintf("MainScreenCreate :: SENSOR_PONSEL_PH\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_ECD_PH)         {Dprintf("MainScreenCreate :: SENSOR_ECD_PH\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}    
            else if(flash.Uart[comPort]->SensorID == SENSOR_SCAN)           {Dprintf("MainScreenCreate :: SENSOR_SCAN\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_ARGADV)         {Dprintf("MainScreenCreate :: SENSOR_ARGADV\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_RAIN)           {Dprintf("MainScreenCreate :: SENSOR_RAIN\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID == SENSOR_INTELL)         {Dprintf("MainScreenCreate :: SENSOR_INTELL\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
            else if(flash.Uart[comPort]->SensorID==SENSOR_TUBIDITY)         {Dprintf("MainScreenCreate :: SENSOR_TUBIDITY\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
			else if(flash.Uart[comPort]->SensorID == SENSOR_WS501)          {Dprintf("MainScreenCreate :: SENSOR_WS501\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
			else if(flash.Uart[comPort]->SensorID == SENSOR_SONAR)          {Dprintf("MainScreenCreate :: SENSOR_SONAR\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
			else if(flash.Uart[comPort]->SensorID == SENSOR_WINCH)          {Dprintf("MainScreenCreate :: SENSOR_WINCH\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}} 
			else if(flash.Uart[comPort]->SensorID == SENSOR_NEP500)         {Dprintf("MainScreenCreate :: SENSOR_NEP500\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
			else if(flash.Uart[comPort]->SensorID == SENSOR_ECT400)         {Dprintf("MainScreenCreate :: SENSOR_ECT400\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}} 
			else if(flash.Uart[comPort]->SensorID == SENSOR_SAMPLER100)     {Dprintf("MainScreenCreate :: SENSOR_SAMPLER100\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}}
			else if(flash.Uart[comPort]->SensorID == SENSOR_STATUS)         {Dprintf("MainScreenCreate :: SENSOR_STATUS\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}} 
			else if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG)          {Dprintf("MainScreenCreate :: SENSOR_DEBUG\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}} 
			else if(flash.Uart[comPort]->SensorID == SENSOR_LDI)            {Dprintf("MainScreenCreate :: SENSOR_LDI\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}} 
			else if(flash.Uart[comPort]->SensorID == SENSOR_SPECTRO_D)      {Dprintf("MainScreenCreate :: SENSOR_SPECTRO_D\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=5;}} 
			else if(flash.Uart[comPort]->SensorID == SENSOR_ODO)            {Dprintf("MainScreenCreate :: SENSOR_ODO\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=2;}} 
			else if(flash.Uart[comPort]->SensorID == SENSOR_SPECTRO_M)      {Dprintf("MainScreenCreate :: SENSOR_SPECTRO_D\n");if(flash.ScreenCnt[comPort]==0){flash.ScreenCnt[comPort]=flash.Uart[USART_1]->ItemCnt;}} 
			else 
            {
                flash.ScreenCnt[comPort]=0;									
                flash.Uart[comPort]->SensorID = SENSOR_NONE;
            }			
            flash.systemStatus=ON;//센서가 설정 됨  
        }
        else
        {
            flash.ScreenCnt[comPort]=0;//생성하지 않는다.
            if(flash.Uart[comPort]->SensorID == SENSOR_KECO || flash.Uart[comPort]->SensorID == SENSOR_LAN || flash.Uart[comPort]->SensorID==SENSOR_MODBUS || flash.Uart[comPort]->SensorID==SENSOR_TMS|| flash.Uart[comPort]->SensorID==SENSOR_EXTTMS)
            {
            }
            else if(flash.Uart[comPort]->SensorID==ANALOG_OUT)
            {
            }
            else
            {
                flash.Uart[comPort]->SensorID = SENSOR_NONE;	//설정된 센서가 없다.
            }
        }
        
        //Dprintf("flash.ScreenCnt[%d] = %d Sensor IdCnt = %d \n",comPort,flash.ScreenCnt[comPort], flash.Uart[comPort]->SensorID);
        /*
        if(comPort == USART_1)      comPort = USART_2;
        else if(comPort == USART_2) comPort = USART_3;
        else if(comPort == USART_3) comPort = USART_4;
        else if(comPort == USART_4) comPort = USART_5;
        */
    }
    /**********************

    INPUT 센서 정보 

    **********************/
	flash.ScreenCnt[5] = 0;     
    for( itemCount =0; itemCount < 10; itemCount++ )//input 10 channel 검사
    {
        if(flash.Input[itemCount]->SensorStatus==ON)
        {
            flash.ScreenCnt[5]++;
            flash.systemStatus=ON;//센서가 설정 됨  
            
        }
    }
	if(flash.systemStatus==OFF)//센서 정보가 없는 경우 
	{
        Dprintf("flash.systemStatus = %d\n");
		return 1;
	}
	/******************************************************************************************************************************************************

	읽어들인 센서정보를 가지고 화면을 생성한다.

	******************************************************************************************************************************************************/
    if(gv.NextScreenCreate==1) {//USART 1 화면을 구성한다.
		if(flash.Uart[0]->SensorStatus == 1)//센서정보가 있는 경우	
		{
			if(flash.ScreenCnt[0] == 1)//측정화면수가 1이면
			{
				gv.NewScreen = 1;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart1Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else if(flash.ScreenCnt[0] == 2)//측정화면수가 2이면
			{
				gv.NewScreen = 2;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart1Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else if(flash.ScreenCnt[0] == 3)//측정화면수가 3이면
			{
				gv.NewScreen = 3;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart1Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else if(flash.ScreenCnt[0] == 4)//측정화면수가 4이면
			{
				gv.NewScreen = 4;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart1Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else if(flash.ScreenCnt[0] == 5)//측정화면수가 5이면
			{
				gv.NewScreen = 5;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart1Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else if(flash.ScreenCnt[0] == 6)//측정화면수가 6이면
			{ 	
				gv.NewScreen = 6;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart1Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else if(flash.ScreenCnt[0] == 7)//측정화면수가 7이면
			{
				gv.NewScreen = 7;
				if(gv.NewScreen != gv.OldScreen)
				{                    
                    SmartLog_Uart1Eight_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else if(flash.ScreenCnt[0] == 8)//측정화면수가 8이면
			{
				gv.NewScreen = 8;
				if(gv.NewScreen != gv.OldScreen)
				{
                    SmartLog_Uart1Eight_Window(parent, handle);                    
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
            //측정화면수가 10이면
            //화면8개생성시화면의구성시필요한변수를사용한다.
			else if(flash.ScreenCnt[0] == 10)
			{
				gv.NewScreen = 8;
				if(gv.NewScreen != gv.OldScreen)
				{
					//SmartLog_Uart1Ten_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else if(flash.ScreenCnt[0] == 12)
			{
				gv.NewScreen = 8;
				if(gv.NewScreen != gv.OldScreen)
				{
                    SmartLog_Uart12Screen_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=2;
					return 0;
				}
				else
					gv.NextScreenCreate=2;
			}
			else 
			{
			 	gv.NextScreenCreate=2;
			}
		}
		else
			gv.NextScreenCreate = 2;
	}
	if(gv.NextScreenCreate==2) {//USART 2 화면을 구성한다.
		if(flash.Uart[1]->SensorStatus == 1)
		{
			if(flash.ScreenCnt[1] == 1)
			{
				gv.NewScreen = 9;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart2Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=3;
					return 0;
				}
				else
					gv.NextScreenCreate=3;
			}
			else if(flash.ScreenCnt[1] == 2)
			{
				gv.NewScreen = 10;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart2Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=3;
					return 0;
				}
				else
					gv.NextScreenCreate=3;
			}
			else if(flash.ScreenCnt[1] == 3)
			{
				gv.NewScreen = 11;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart2Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=3;
					return 0;
				}
				else
					gv.NextScreenCreate=3;
			}
			else if(flash.ScreenCnt[1] == 4)
			{
				gv.NewScreen = 12;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart2Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=3;
					return 0;
				}
				else
					gv.NextScreenCreate=3;
			}
			else if(flash.ScreenCnt[1] == 5)
			{
				gv.NewScreen = 13;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart2Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=3;
					return 0;
				}
				else
					gv.NextScreenCreate=3;
			}
			else if(flash.ScreenCnt[1] == 6)
			{
				gv.NewScreen = 14;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart2Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=3;
					return 0;
				}
				else
					gv.NextScreenCreate=3;
			}
			else if(flash.ScreenCnt[1] == 7)
			{
				gv.NewScreen = 15;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart2Eight_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=3;
					return 0;
				}
				else
					gv.NextScreenCreate=3;
			}
			else if(flash.ScreenCnt[1] == 8)
			{
				gv.NewScreen = 16;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart2Eight_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=3;
					return 0;
				}
				else
					gv.NextScreenCreate=3;
			}
			else
			{
				gv.NextScreenCreate=3;
			}
		}
		else
			gv.NextScreenCreate = 3;
	}
	if(gv.NextScreenCreate==3) {//USART 3 화면을 구성한다.
		if(flash.Uart[2]->SensorStatus == 1)
		{
			if(flash.ScreenCnt[2] == 1)
			{
				gv.NewScreen = 17;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart3Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=4;
					return 0;
				}
				else
					gv.NextScreenCreate=4;
			}
			else if(flash.ScreenCnt[2] == 2)
			{
				gv.NewScreen = 18;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart3Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=4;
					return 0;
				}
				else
					gv.NextScreenCreate=4;
			}
			else if(flash.ScreenCnt[2] == 3)
			{
				gv.NewScreen = 19;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart3Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=4;
					return 0;
				}
				else
					gv.NextScreenCreate=4;
			}
			else if(flash.ScreenCnt[2] == 4)
			{
				gv.NewScreen = 20;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart3Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=4;
					return 0;
				}
				else
					gv.NextScreenCreate=4;
			}
			else if(flash.ScreenCnt[2] == 5)
			{
				gv.NewScreen = 21;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart3Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=4;
					return 0;
				}
				else
					gv.NextScreenCreate=4;
			}
			else if(flash.ScreenCnt[2] == 6)
			{
				gv.NewScreen = 22;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart3Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=4;
					return 0;
				}
				else
					gv.NextScreenCreate=4;
			}
			else if(flash.ScreenCnt[2] == 7)
			{
				gv.NewScreen = 23;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart3Eight_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=4;
					return 0;
				}
				else
					gv.NextScreenCreate=4;
			}
			else if(flash.ScreenCnt[2] == 8)
			{
				gv.NewScreen = 24;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart3Eight_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=4;
					return 0;
				}
				else
					gv.NextScreenCreate=4;
			}
			else
			{
				gv.NextScreenCreate=4;
			}
		}
		else
			gv.NextScreenCreate = 4;
	}
	if(gv.NextScreenCreate==4) {//USART 4 화면을 구성한다.
		if(flash.Uart[3]->SensorStatus == 1)
		{
			if(flash.ScreenCnt[3] == 1)
			{
				gv.NewScreen = 25;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart4Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=5;
					return 0;
				}
				else
					gv.NextScreenCreate=5;
			}
			else if(flash.ScreenCnt[3] == 2)
			{
				gv.NewScreen = 26;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart4Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=5;
					return 0;
				}
				else
					gv.NextScreenCreate=5;
			}
			else if(flash.ScreenCnt[3] == 3)
			{
				gv.NewScreen = 27;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart4Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=5;
					return 0;
				}
				else
					gv.NextScreenCreate=5;
			}
			else if(flash.ScreenCnt[3] == 4)
			{
				gv.NewScreen = 28;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart4Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=5;
					return 0;
				}
				else
					gv.NextScreenCreate=5;
			}
			else if(flash.ScreenCnt[3] == 5)
			{
				gv.NewScreen = 29;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart4Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=5;
					return 0;
				}
				else
					gv.NextScreenCreate=5;
			}
			else if(flash.ScreenCnt[3] == 6)
			{
				gv.NewScreen = 30;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart4Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=5;
					return 0;
				}
				else
					gv.NextScreenCreate=5;
			}
			else if(flash.ScreenCnt[3] == 7)
			{
				gv.NewScreen = 31;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart4Eight_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=5;
					return 0;
				}
				else
					gv.NextScreenCreate=5;
			}
			else if(flash.ScreenCnt[3] == 8)
			{
				gv.NewScreen = 32;
				if(gv.NewScreen != gv.OldScreen)
				{
#ifdef DMSAMPLER_ENABLE
					SmartLog_Uart4Eight_Window(parent, handle);
#endif					
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=5;
					return 0;
				}
				else
					gv.NextScreenCreate=5;
			}
		}
		else
			gv.NextScreenCreate = 5;
	}
	if(gv.NextScreenCreate==5) {//USART 5 화면을 구성한다.
		if(flash.Uart[4]->SensorStatus == 1)
		{
			if(flash.ScreenCnt[4] == 1)
			{
				gv.NewScreen = 33;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart5Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=6;
					return 0;
				}
				else
					gv.NextScreenCreate=6;
			}
			else if(flash.ScreenCnt[4] == 2)
			{
				gv.NewScreen = 34;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart5Two_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=6;
					return 0;
				}
				else
					gv.NextScreenCreate=6;
			}
			else if(flash.ScreenCnt[4] == 3)
			{
				gv.NewScreen = 35;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart5Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=6;
					return 0;
				}
				else
					gv.NextScreenCreate=6;
			}
			else if(flash.ScreenCnt[4] == 4)
			{
				gv.NewScreen = 36;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart5Four_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=6;
					return 0;
				}
				else
					gv.NextScreenCreate=6;
			}
			else if(flash.ScreenCnt[4] == 5)
			{
				gv.NewScreen = 37;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart5Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=6;
					return 0;
				}
				else
					gv.NextScreenCreate=6;
			}
			else if(flash.ScreenCnt[4] == 6)
			{
				gv.NewScreen = 38;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart5Six_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=6;
					return 0;
				}
				else
					gv.NextScreenCreate=6;
			}
			else if(flash.ScreenCnt[4] == 7)
			{
				gv.NewScreen = 39;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart5Eight_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=6;
					return 0;
				}
				else
					gv.NextScreenCreate=6;
			}
			else if(flash.ScreenCnt[4] == 8)
			{
				gv.NewScreen = 40;
				if(gv.NewScreen != gv.OldScreen)
				{
					SmartLog_Uart5Eight_Window(parent, handle);
					gv.OldScreen = gv.NewScreen;
					gv.NextScreenCreate=6;
					return 0;
				}
				else
					gv.NextScreenCreate=6;
			}
		}
		else
			gv.NextScreenCreate = 6;
	}

	//ADC INPUT SCREEN
	if(gv.NextScreenCreate==6)
	{
        Dprintf("AD SCREEN CREATE =%d \n",flash.ScreenCnt[5]);
		if(flash.ScreenCnt[5] == 1)
		{
			gv.NewScreen = 41;
			if(gv.NewScreen != gv.OldScreen)
			{

#ifdef  ENABLE_TUBIDITY                
				if(flash.TUB_Process_Control_Flag==ON)//AD 입력을 받아 그래프로 데이터를 출력한다.
				{
					//SmartLog_ADInput_Graph_Window(parent, handle);
                    Dprintf("SmartLog_ADInput_Graph_Window\n");
				}
				else
				{
                    Dprintf("SmartLog_ADTwo_Window\n");
					SmartLog_ADTwo_Window(parent, handle);
				}
#endif                
                SmartLog_ADTwo_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
		else if(flash.ScreenCnt[5] == 2)
		{
			gv.NewScreen = 42;
			if(gv.NewScreen != gv.OldScreen)
			{
				/*
				if(flash.Nitrogen_Control==ON)
				{
					SmartLog_ADSix_Window(parent, handle);
				}
				else
				{
					SmartLog_ADTwo_Window(parent, handle);
				}
				*/
				SmartLog_ADTwo_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
		else if(flash.ScreenCnt[5] == 3)
		{
			gv.NewScreen = 43;
			if(gv.NewScreen != gv.OldScreen)
			{
				SmartLog_ADFour_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
		else if(flash.ScreenCnt[5] == 4)
		{
			gv.NewScreen = 44;
			if(gv.NewScreen != gv.OldScreen)
			{
				SmartLog_ADFour_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
		else if(flash.ScreenCnt[5] == 5)
		{
			gv.NewScreen = 45;
			if(gv.NewScreen != gv.OldScreen)
			{
				SmartLog_ADSix_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
		else if(flash.ScreenCnt[5] == 6)
		{
			gv.NewScreen = 46;
			if(gv.NewScreen != gv.OldScreen)
			{
				SmartLog_ADSix_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
		else if(flash.ScreenCnt[5] == 7)
		{
			gv.NewScreen = 47;
			if(gv.NewScreen != gv.OldScreen)
			{
				SmartLog_ADEight_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
		else if(flash.ScreenCnt[5] == 8)
		{
			gv.NewScreen = 48;
			if(gv.NewScreen != gv.OldScreen)
			{
				SmartLog_ADEight_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		EX100 화면구성
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef  EX100_ENABLE
		else if(flash.Ex100.status==ON)
		{
			gv.NewScreen = 49;
			if(gv.NewScreen != gv.OldScreen)
			{
				SmartLog_EX100_Window(parent, handle);
				gv.OldScreen = gv.NewScreen;
				gv.NextScreenCreate=1;
				return 0;
			}
			else
				gv.NextScreenCreate=1;
		}
#endif        
		else
			gv.NextScreenCreate = 1;
	}
	return 7;
}


//센서 접속 여부를 판단하여 리턴한다.
int SensorInOutChannelConnect(void) {
    unsigned int    i = 0;
/****************

UART INPUT CHECK

****************/
    for(i = 0; i<5; i++)
    {
        if(flash.Uart[i]->SensorStatus==ON)
        {                       
            return 1;
        }
    }
/**************

AD420mA INPUT CHECK

**************/
    for(i = 0; i<10; i++)
    {
        if(flash.Input[i]->SensorStatus==ON)
        {
            return 1;
        }
    }
	return 0;
}

int TitleBar_MenuButton_Create( U32 handle, int comPort, int (*ClickEvent)(HANDLE, HANDLE, WCOMMAND, PARAM, void *) ) {
    char pBuf[128];
    TitleBar_Create(handle,comPort,pBuf);    
    TitleBar(handle, 0, 0, LCD_X, 60, 1, pBuf, clYellow, 1, RIGHT_ALIGN, 3, NULL);	
    
	if(flash.systemColorNo!=6) {
		if(comPort==10) {//SENSOR
			NButton(handle, 0, 0, 300+2, 60, "48G_TestBottle_Icon1.bmp", pBuf,  SELECT_BUTTON, 67, 0);    
		}
		else if(comPort==11) {//SYSTEM
			NButton(handle, 0, 0, 300+2, 60, "48G_Tool_Icon3.bmp", pBuf,  SELECT_BUTTON, 67, 0);    
		}
		else if(comPort==12) {//DEBUG
			//SButton(handle, 2, 0, 300-2, 50, " ", pBuf,  SELECT_BUTTON, 67, 0);    
		}
		else {
			NButton(handle, 0, 0, 300+2, 60, 0, pBuf,  SELECT_BUTTON, 67, 0); //UART   
		}
		
		if(comPort!=12) {
			gv.titleBarLogId = NButton(handle, 300, 0, 424, 60, " ", gv.titleBuf,  PUSH_BUTTON, 69, ClickEvent); //VER4.0
			sprintf(pBuf,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
			//48G_Calendar_Icon1.bmp
			gv.titleBarTimeId = NButton(handle, 1024-300, 0, 300, 60, 0, pBuf,  SELECT_BUTTON, 68, 0);
		} else {
			gv.titleBarLogId = NButton(handle, 300, 0, 424, 60, " ", gv.titleBuf,  PUSH_BUTTON, 69, ClickEvent);//VER4.0
			sprintf(pBuf,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
			gv.titleBarTimeId = NButton(handle, 1024-300, 0, 300, 60, " ", pBuf,  SELECT_BUTTON, 68, 0);
		}
		if(comPort!=10 && comPort!=11  && comPort!=12) { //SENSOR, SYSTEM 모드에서는 버튼을 생성하지 않는다.
			//Button(handle,  0, LCD_Y-65, 256, 64, ICON_BUTTON, " ", " ","48G_TestBottle_Icon1.bmp", FONT64, "SENSOR", PUSH_BUTTON, 1, ClickEvent);
			NButton(handle, 0, LCD_Y-65, 256, 64, "48G_TestBottle_Icon1.bmp", "SENSOR",  PUSH_BUTTON, 1, ClickEvent);
			NButton(handle, 0+256, LCD_Y-65, 256, 64, "48G_Tool_Icon3.bmp", "SYSTEM",  PUSH_BUTTON, 3, ClickEvent);
			NButton(handle, 0+256+256, LCD_Y-65, 256, 64, "48G_USB_Icon1.bmp", "USB",  PUSH_BUTTON, 4, ClickEvent);
			#if defined(YSI_ENABLE)
			NButton(handle, 0+256+256+256, LCD_Y-65, 256, 64, "48G_Command_Icon1.bmp", "CAL",  PUSH_BUTTON, 2, ClickEvent);
			#else
			NButton(handle, 0+256+256+256, LCD_Y-65, 256, 64, "48G_Command_Icon1.bmp", "DEBUG",  PUSH_BUTTON, 2, ClickEvent);
			#endif
		} 
		
	} 
	else {
		if(comPort==10) {//SENSOR
			NButton(handle, 0, 0, 300+2, 60, "48G_TestBottle_Icon1.bmp", pBuf,  SELECT_BUTTON, 67, 0);    
		}
		else if(comPort==11) {//SYSTEM
			NButton(handle, 0, 0, 300+2, 60, "48G_Tool_Icon3.bmp", pBuf,  SELECT_BUTTON, 67, 0);    
		}
		else if(comPort==12) {//DEBUG

		}
		else {
			NButton(handle, 0, 0, 300+2, 60, 0, pBuf,  SELECT_BUTTON, 67, 0); //UART   
		}
		
		if(comPort!=12) {
			gv.titleBarLogId = NButton(handle, 300+1, 0, 424, 60, " ", gv.titleBuf,  PUSH_BUTTON, 69, ClickEvent); //VER4.0
			sprintf(pBuf,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
			gv.titleBarTimeId = NButton(handle, 1024-300, 0, 300, 60, 0, pBuf,  SELECT_BUTTON, 68, 0);
		} else {
			gv.titleBarLogId = NButton(handle, 300+1, 0, 424, 60, " ", gv.titleBuf,  PUSH_BUTTON, 69, ClickEvent);//VER4.0
			sprintf(pBuf,"20%02d/%02d/%02d %02d:%02d:%02d", currentDate.year,currentDate.month,currentDate.date,currentDate.hour, currentDate.minute, currentDate.sec);
			gv.titleBarTimeId = NButton(handle, 1024-300, 0, 300, 60, " ", pBuf,  SELECT_BUTTON, 68, 0);
		}
		if(comPort!=10 && comPort!=11  && comPort!=12) { //SENSOR, SYSTEM 모드에서는 버튼을 생성하지 않는다.
			SButton(handle, 0, LCD_Y-65, 256, 64, "48G_TestBottle_Icon1.bmp", "SENSOR",  PUSH_BUTTON, 1, ClickEvent);
			SButton(handle, 0+256, LCD_Y-65, 256, 64, "48G_Tool_Icon3.bmp", "SYSTEM",  PUSH_BUTTON, 3, ClickEvent);
			SButton(handle, 0+256+256, LCD_Y-65, 256, 64, "48G_USB_Icon1.bmp", "USB",  PUSH_BUTTON, 4, ClickEvent);
			#if defined(YSI_ENABLE)
			SButton(handle, 0+256+256+256, LCD_Y-65, 256, 64, "48G_Command_Icon1.bmp", "CAL",  PUSH_BUTTON, 2, SmartLog_Uart1Four_KeyEvent);
			#else
			SButton(handle, 0+256+256+256, LCD_Y-65, 256, 64, "48G_Command_Icon1.bmp", "DEBUG",  PUSH_BUTTON, 2, ClickEvent);			
			#endif
		} 
	}
    return 0;
}





