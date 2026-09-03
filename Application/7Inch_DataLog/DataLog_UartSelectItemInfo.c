#include "TEECO_System.h"
#include "T_socket.h"
#include "T_dhcp.h"
#include "T_loopback.h"
#include "DataLog.h"
#include "BSP_W5500.h"

extern volatile int EXO_Find_CommandChangeFlag[3]; 
extern void ULIK_Init(void);
int	UartPortInfo( int uart, char *data)
{
	unsigned int        cnt = 0;
    /*
        EXO가아니면 전부삭제한다.
    
    */
	if(0!=strcmp( data, "EXO"))
	{
        if(gv.ChangeNameFlag==1)//이름삭제
        {
            flash.Uart[uart]->SensorID     = 0;
            flash.Uart[uart]->SensorStatus  = 0;
            flash.Uart[uart]->ItemCnt       = 0;  
            for(cnt = 0;cnt < USART_SENSOR_MAX; cnt++)
            {
                strcpy(flash.Uart[uart]->SetupName[cnt], "NONE");
            }
        }
	}
	//USART_1 일 경우 탁도가 선택이 안되면  자동으로 OFF 된다.
	//if(USART_1 == uart)flash.TUB_Process_Control_Flag=OFF;
	if(0==strcmp( data, "ARGADV"))
	{
		flash.Uart[uart]->SensorID = SENSOR_ARGADV; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt	= 4;//출력되는 데인터 수

		strcpy(flash.Uart[uart]->SetupName[0],"DIR");
		strcpy(flash.Uart[uart]->SetupName[1],"VEL");
		strcpy(flash.Uart[uart]->SetupName[2],"VX");
		strcpy(flash.Uart[uart]->SetupName[3],"VY");
	}
	else if(0==strcmp(data,"ODO")) {
		#if defined(YSI_ODD_ENABLE)
		flash.Uart[uart]->SensorID = SENSOR_ODO;
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt = 2;//출력되는 데인터 수
        strcpy(flash.Uart[USART_1]->SetupName[0],"Temp 'C");
        strcpy(flash.Uart[USART_1]->SetupName[1],"ODO mg/L");
		#endif
		#if defined(YSI_ODOCT_ENABLE)
		flash.Uart[uart]->SensorID = SENSOR_ODO;
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt = 3;//출력되는 데인터 수
        strcpy(flash.Uart[uart]->SetupName[0],"Temp 'C");
        strcpy(flash.Uart[uart]->SetupName[1],"ODO mg/L");

        if(flash.odoct.spcondUnit==0) {
            strcpy(flash.Uart[uart]->SetupName[2],"SPCOND uS/cm");
        } else if(flash.odoct.spcondUnit==1){
            strcpy(flash.Uart[uart]->SetupName[2],"SPCOND mS/cm");
        } else {
            strcpy(flash.Uart[uart]->SetupName[2],"Salinity ppt");
        }
        
		//strcpy(flash.Uart[uart]->SetupName[3],"Salinity ppt");
		
        //strcpy(SmartLogFlash.Uart[USART_1]->SetupName[2],"Salinity ppt");
		//strcpy(flash.Uart[USART_1]->SetupName[3],"pH");
		#endif
        //strcpy(SmartLogFlash.Uart[uart]->SetupName[4],"Ref Temp'C");		
	}
	else if(0==strcmp( data, "STATUS" )) {
		flash.Uart[uart]->SensorID = SENSOR_STATUS;
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt = 5;//출력되는 데인터 수
        strcpy(flash.Uart[uart]->SetupName[0],"A펌프동작상태");
        strcpy(flash.Uart[uart]->SetupName[1],"B펌프동작상태");
        strcpy(flash.Uart[uart]->SetupName[2],"강우량접점값");
        strcpy(flash.Uart[uart]->SetupName[3],"강우량");		
        strcpy(flash.Uart[uart]->SetupName[4],"UPS동작상태");		
		strcpy(flash.Uart[uart]->SetupName[5],"강우량알람상태");		
	}
	else if(0==strcmp( data, "SAMPLER100"))
	{
		flash.Uart[uart]->SensorID = SENSOR_SAMPLER100;
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt	= 4;//출력되는 데인터 수
        strcpy(flash.Uart[uart]->SetupName[0],"동작상태");
        strcpy(flash.Uart[uart]->SetupName[1],"도어상태");
        strcpy(flash.Uart[uart]->SetupName[2],"온도");
        strcpy(flash.Uart[uart]->SetupName[3],"채수병위치");
		//측정값만 사용한다.
	}
	else if(0==strcmp( data, "PONSEL_DO"))
	{
		flash.Uart[uart]->SensorID = SENSOR_PONSEL_DO; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt	= 2;//출력되는 데인터 수
        strcpy(flash.Uart[uart]->SetupName[0],"DO mg/L");
        strcpy(flash.Uart[uart]->SetupName[1],"Temperature 'C");
	}
    else if(0==strcmp( data, "ECD_pH"))
    {
        flash.Uart[uart]->SensorID = SENSOR_ECD_PH; // ECD 사의 pH센서
        flash.Uart[uart]->SensorStatus = 1;
        flash.Uart[uart]->ItemCnt = 8;
        strcpy(flash.Uart[uart]->SetupName[0],"[ch1] pH");
        strcpy(flash.Uart[uart]->SetupName[1],"[ch1] Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[2],"[ch2] pH");
        strcpy(flash.Uart[uart]->SetupName[3],"[ch2] Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[4],"[ch3] pH");
        strcpy(flash.Uart[uart]->SetupName[5],"[ch3] Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[6],"[ch4] pH");
        strcpy(flash.Uart[uart]->SetupName[7],"[ch4] Temperature 'C");
		/*	
        flash.Uart[uart]->ItemCnt = 4;
        strcpy(flash.Uart[uart]->SetupName[0],"[ch1] pH");
        strcpy(flash.Uart[uart]->SetupName[1],"[ch1] Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[2],"[ch2] pH");
        strcpy(flash.Uart[uart]->SetupName[3],"[ch2] Temperature 'C");
		*/
	}
    else if(0==strcmp( data, "PH_COND"))
    {
		flash.Uart[uart]->SensorID = SENSOR_PONSEL_PH_COND; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt	= 3;//출력되는 데인터 수
        
        strcpy(flash.Uart[uart]->SetupName[0],"Temperture 'C");
        strcpy(flash.Uart[uart]->SetupName[1],"pH");
        strcpy(flash.Uart[uart]->SetupName[2],"COND mS/cm");
    }
	else if(0==strcmp( data, "PONSEL_EC"))
	{        
		#if 0
		flash.Uart[uart]->SensorID = SENSOR_PONSEL_EC; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt	= 6;//출력되는 데인터 수
			
        strcpy(flash.Uart[uart]->SetupName[0],"[CH1] COND mS/cm");
        strcpy(flash.Uart[uart]->SetupName[1],"[CH1] Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[2],"[CH2] COND mS/cm");
        strcpy(flash.Uart[uart]->SetupName[3],"[CH2] Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[4],"[CH3] COND mS/cm");
        strcpy(flash.Uart[uart]->SetupName[5],"[CH3] Temperature 'C");
		#endif
		flash.Uart[uart]->SensorID = SENSOR_PONSEL_EC; 	//외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;				//포트를 사용한다.
		flash.Uart[uart]->ItemCnt	= 3;				//출력되는 데이터 수
			
		strcpy(flash.Uart[USART_1]->SetupName[0],"TEMP 'C");
		strcpy(flash.Uart[USART_1]->SetupName[1],"COND mS/cm");
		strcpy(flash.Uart[USART_1]->SetupName[2],"Salinity ppt");
	}
	else if(0==strcmp( data, "PONSEL_PH"))
	{
        //Dprintf("flash.Uart[%d]->SensorID = %d\n",uart,flash.Uart[uart]->SensorID);
		flash.Uart[uart]->SensorID = SENSOR_PONSEL_PH; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;//포트를 사용한다.
		flash.Uart[uart]->ItemCnt	= 4;//출력되는 데인터 수
        
        strcpy(flash.Uart[uart]->SetupName[0],"[CH1]pH");
        strcpy(flash.Uart[uart]->SetupName[1],"[CH2]Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[2],"[CH2]pH");
        strcpy(flash.Uart[uart]->SetupName[3],"[CH2]Temperature 'C");
        /*
        strcpy(flash.Uart[uart]->SetupName[4],"[CH3] pH");
        strcpy(flash.Uart[uart]->SetupName[5],"[CH3] Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[6],"[CH4] pH");
        strcpy(flash.Uart[uart]->SetupName[7],"[CH4] Temperature 'C");
        strcpy(flash.Uart[uart]->SetupName[8],"[CH5] pH");
        strcpy(flash.Uart[uart]->SetupName[9],"[CH5] Temperature 'C");
        */
        //Dprintf("UART[%d] PH\n",uart);
	}
#ifdef DMSAMPLER_ENABLE
	else if(0==strcmp( data, "DMSampler")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_DMSAMPLER; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;
		flash.Uart[uart]->ItemCnt=8;
	}
#endif	
#ifdef  ENABLE_TUBIDITY    
	else if(0==strcmp( data, "TUBIDITY")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_TUBIDITY; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;
		flash.Uart[uart]->ItemCnt=1;//탁도값과 mV값을 출력한다.
		flash.TUB_Process_Control_Flag=2;//USART로 연결이 된다.
		if(gv.ChangeNameFlag==1)
		{
			strcpy(flash.Uart[uart]->SetupName[0],"TUBIDITY");
		}	
	}
#endif    
#ifdef GPS_ENABLE	
	else if(0==strcmp( data, "GPS"))
	{		
		flash.Uart[uart]->SensorID = SENSOR_GPS;
		flash.Uart[uart]->SensorStatus = 1;
		flash.Uart[uart]->ItemCnt = 2;
		strcpy(flash.Uart[uart]->SetupName[0],"위도");
		strcpy(flash.Uart[uart]->SetupName[1],"경도");
	}
#endif	
#ifdef EXO_ENABLE
	else if(0==strcmp( data, "EXO"))
	{
		flash.Uart[uart]->SensorID = SENSOR_EXO; //외부와 통신전용으로 사용한다.
		flash.Uart[USART_1]->SensorStatus = 1;
		EXO_Find_CommandChangeFlag[0]=OFF;
		EXO_Find_CommandChangeFlag[1]=OFF;
		EXO_Find_CommandChangeFlag[2]=OFF;
	}
#endif    
	else if(0==strcmp( data, "YSI-S")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_YSI; 
		flash.Uart[uart]->SensorStatus = 1;
        
	}
	else if(0==strcmp( data, "TN")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_TN; 
		flash.Uart[uart]->SensorStatus = 1;
		strcpy(flash.Uart[uart]->SetupName[0],"TON00");
		flash.Uart[uart]->ItemCnt = 1;
	}
	else if(0==strcmp( data, "TP")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_TP; 
		flash.Uart[uart]->SensorStatus = 1;

		strcpy(flash.Uart[uart]->SetupName[0],"TOP00");
		flash.Uart[uart]->ItemCnt = 1;

	}
	else if(0==strcmp( data, "Auto-W")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_AUTO_SAMPLER; 
		flash.Uart[uart]->SensorStatus = 1;

		strcpy(flash.Uart[uart]->SetupName[0],"SAM00");
		strcpy(flash.Uart[uart]->SetupName[1],"SAM01");
		strcpy(flash.Uart[uart]->SetupName[2],"SAM02");
		strcpy(flash.Uart[uart]->SetupName[3],"SAM03");

		flash.Uart[uart]->ItemCnt = 4;
	}
	else if(0==strcmp( data, "WIZ")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_WIZ; 
		flash.Uart[uart]->SensorStatus = 1;

		strcpy(flash.Uart[uart]->SetupName[0],"NO3");
		strcpy(flash.Uart[uart]->SetupName[1],"NO2");
		strcpy(flash.Uart[uart]->SetupName[2],"PO4");
		strcpy(flash.Uart[uart]->SetupName[3],"NH3");
		strcpy(flash.Uart[uart]->SetupName[4],"NO2SOD");
		strcpy(flash.Uart[uart]->SetupName[5],"NO3SOD");
		strcpy(flash.Uart[uart]->SetupName[6],"PO4SOD");
		strcpy(flash.Uart[uart]->SetupName[7],"NH3SOD");
		strcpy(flash.Uart[uart]->SetupName[8],"NO2FOD");
		strcpy(flash.Uart[uart]->SetupName[9],"NO3FOD");
		strcpy(flash.Uart[uart]->SetupName[10],"PO4FOD");
		strcpy(flash.Uart[uart]->SetupName[11],"NH3FOD");

		flash.Uart[uart]->ItemCnt = 4;//화면에 출력하는 갯수
	}
	else if(0==strcmp( data, "MODBUS"))
	{
		flash.Uart[uart]->SensorID = SENSOR_MODBUS; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data, "LAN"))
	{
		flash.Uart[uart]->SensorID = SENSOR_LAN; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data,"EX100-1"))
	{
		flash.Uart[uart]->SensorID = SENSOR_EX100_1; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data,"EX100-2"))
	{
		flash.Uart[uart]->SensorID = SENSOR_EX100_2; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data, "TMS"))
	{
		flash.Uart[uart]->SensorID = SENSOR_TMS; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data, "KECO"))
	{
		flash.Uart[uart]->SensorID = SENSOR_KECO; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data, "EXTTMS"))
	{
		flash.Uart[uart]->SensorID = SENSOR_EXTTMS; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data, "SEJONG"))
	{
		flash.Uart[uart]->SensorID = SENSOR_SEJONG; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data, "HF-TOL"))
	{
		flash.Uart[uart]->SensorID = SENSOR_HFTOL; //외부와 통신전용으로 사용한다.
		//별도의 화면에 값을 출력한다.
		flash.Uart[uart]->SensorStatus = 1;
		flash.Uart[uart]->ItemCnt = 1;
		strcpy(flash.Uart[uart]->SetupName[0],"HF-TOL");
	}
	else if(0==strcmp( data, "HF-CLX"))
	{
		flash.Uart[uart]->SensorID = SENSOR_HFCLX; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;
		//strcpy(flash.Uart[uart]->SetupName[0],"HF-CLX");
		strcpy(flash.Uart[uart]->SetupName[0],"CLX-EX mg/L");
		flash.Uart[uart]->ItemCnt = 1;
	}	
	else if(0==strcmp( data, "SONTEK"))
	{
		flash.Uart[uart]->SensorID = SENSOR_SONTEK; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;
		strcpy(flash.Uart[uart]->SetupName[0],"DIR");
		strcpy(flash.Uart[uart]->SetupName[1],"VEL");
		strcpy(flash.Uart[uart]->SetupName[2],"X");
		strcpy(flash.Uart[uart]->SetupName[3],"Y");
		strcpy(flash.Uart[uart]->SetupName[4],"DEP");
		//strcpy(flash.Uart[uart]->SetupName[5],"TEMP");
		
		flash.Uart[uart]->ItemCnt = 6;
	}
#ifdef      SCAN_ENABLE    
	else if(0==strcmp( data,"SCAN"))
	{
		flash.Uart[uart]->SensorID = SENSOR_SCAN; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 1;
		if(flash.Scan.ItemCount==0)flash.Scan.ItemCount=1;
		else flash.Uart[uart]->ItemCnt = flash.Scan.ItemCount;
	}
#endif    
    #ifdef	ADP_SONTEK_ENABLE    
	else if(0==strcmp( data,"ADP"))
	{
        //Dprintf("SENSOR[%d] = ADP SONTEK[%d]\n",uart,flash.Uart[uart]->SensorID );
        
		flash.Uart[uart]->SensorID 		= SENSOR_ADPSONTEK;
		flash.Uart[uart]->SensorStatus 	= 1;//0이면데이터를 저장하지 않는다.
		//flash.Uart[uart]->ItemCnt 		= 8;

		strcpy(flash.Uart[uart]->SetupName[0],"DIR1");
		strcpy(flash.Uart[uart]->SetupName[1],"VEL1");
		strcpy(flash.Uart[uart]->SetupName[2],"DIR2");
		strcpy(flash.Uart[uart]->SetupName[3],"VEL2");
		strcpy(flash.Uart[uart]->SetupName[4],"DIR3");
		strcpy(flash.Uart[uart]->SetupName[5],"VEL3");
		strcpy(flash.Uart[uart]->SetupName[6],"DIR4");
		strcpy(flash.Uart[uart]->SetupName[7],"VEL4");
	}
    else if(0==strcmp( data, "RAIN"))
    {
		flash.Uart[uart]->SensorID 		= SENSOR_RAIN;
		flash.Uart[uart]->SensorStatus 	= 1;//0이면데이터를 저장하지 않는다.
		flash.Uart[uart]->ItemCnt 		= 1;
    }
	#endif
	else if(0==strcmp( data,"AO"))
	{
		flash.Uart[uart]->SensorID = ANALOG_OUT; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
	}
	else if(0==strcmp( data,"INTELL"))
	{
		flash.Uart[uart]->SensorID 		= SENSOR_INTELL; //외부와 통신전용으로 사용한다.
		flash.Uart[uart]->SensorStatus 	= 1;
	}
	else if(0==strcmp( data,"WINCH"))
	{
		flash.Uart[uart]->SensorID = SENSOR_WINCH;
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt	= 0;
	}
	else if(0==strcmp( data,"SAMPLER"))
	{
		flash.Uart[uart]->SensorID 			= SENSOR_SAMPLER;
		flash.Uart[uart]->SensorStatus 	= 1;
		flash.Uart[uart]->ItemCnt 			= 3;

		strcpy(flash.Uart[uart]->SetupName[0],"STEMP");		
		strcpy(flash.Uart[uart]->SetupName[1],"Position");		
		strcpy(flash.Uart[uart]->SetupName[2],"Status");		
	}
	else if(0==strcmp( data, "UITNH4"))//암모니아
	{
		flash.Uart[uart]->SensorID 			= SENSOR_UITNH4;
		flash.Uart[uart]->SensorStatus 	    = 1;
		flash.Uart[uart]->ItemCnt 			= 1;

		strcpy(flash.Uart[uart]->SetupName[0],"UITNH4");		
	}
#ifdef M_SERIES
	else if(0==strcmp( data, "Mseries")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_MSERIES; 
		flash.Uart[uart]->SensorStatus = 1;
        //Dprintf("SENSOR_MSERIES\n");
	}
#endif    
#ifdef AUTO_SAMPLER    
	else if(0==strcmp( data, "AutoSampler")) 
	{
		flash.Uart[uart]->SensorID = SENSOR_AUTOSAMPLER; 
		flash.Uart[uart]->SensorStatus = 1;
        flash.Uart[uart]->ItemCnt = 7;  //화면을 새로 구성한다.
        //Dprintf("SENSOR_AUTOSAMPLER\n");
	}
#endif    
#ifdef REMOTE_TERMINAL_UNIT
	else if(0==strcmp(data, "WS501"))
	{
		flash.Uart[uart]->SensorID = SENSOR_WS501; 
		flash.Uart[uart]->SensorStatus = 1;
        flash.Uart[uart]->ItemCnt = 6;  
	}
	else if(0==strcmp(data,"SONAR"))
	{
		flash.Uart[uart]->SensorID = SENSOR_SONAR;
		flash.Uart[uart]->SensorStatus = 1;
        flash.Uart[uart]->ItemCnt = 6;  
	}
#endif //#ifdef REMOTE_TERMINAL_UNIT	
    else if(0==strcmp( data, "NONE"))
    {
        //Dprintf("UART[%d] NO SENSOR\n", uart);
		flash.Uart[uart]->SensorID = 0;
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0; 
    }
	else if(0==strcmp( data,"NEP500"))
	{
		flash.Uart[uart]->SensorID 		= SENSOR_NEP500;
		flash.Uart[uart]->SensorStatus 	= 1;
		flash.Uart[uart]->ItemCnt 		= 2;
		strcpy(flash.Uart[USART_1]->SetupName[0],"[CH1] TUBIDITY NTU");		
		strcpy(flash.Uart[USART_1]->SetupName[1],"[CH2] TUBIDITY NTU");		
	}
    else if(0==strcmp(data, "DEBUG")) {
        flash.Uart[uart]->SensorID 		= SENSOR_DEBUG;
        flash.Uart[uart]->SensorStatus 	= 1;
        flash.Uart[uart]->ItemCnt 		= 2;
    }
    else if(0==strcmp(data,"LDI")) {
        flash.Uart[uart]->SensorID      = SENSOR_LDI;
        flash.Uart[uart]->SensorStatus 	= 1;
        flash.Uart[uart]->ItemCnt 		= 2;
        strcpy(flash.Uart[uart]->SetupName[0], "측정지점 0m");
        strcpy(flash.Uart[uart]->SetupName[1], "측정지점 0.2m");
        strcpy(flash.Uart[uart]->SetupName[2], "측정지점 0.4m");
        strcpy(flash.Uart[uart]->SetupName[3], "측정지점 0.6m");
        strcpy(flash.Uart[uart]->SetupName[4], "측정지점 0.8m");
        strcpy(flash.Uart[uart]->SetupName[5], "측정지점 1.0m");
        strcpy(flash.Uart[uart]->SetupName[6], "측정지점 1.2m");
        strcpy(flash.Uart[uart]->SetupName[7], "측정지점 1.4m");
        strcpy(flash.Uart[uart]->SetupName[8], "측정지점 1.6m");
        strcpy(flash.Uart[uart]->SetupName[9], "측정지점 1.8m");
    }
    else if(0==strcmp(data, "SPECTRO-D")) {

        ULIK_Init();
    }
    else if(0==strcmp(data, "SPECTRO-M")) {

        ULIK_Init();
    }
    else
    {
        //Dprintf("UART[%d] NO SENSOR\n", uart);
		flash.Uart[uart]->SensorID = 0;
		flash.Uart[uart]->SensorStatus = 0;
		flash.Uart[uart]->ItemCnt = 0;
    }
    Dprintf("SENSOR CH[%d] %s\n",uart ,data);
	return (1);
}      
