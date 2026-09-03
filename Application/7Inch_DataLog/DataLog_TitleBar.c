#include "TEECO_System.h"
#include "T_socket.h"
#include "T_dhcp.h"
#include "T_loopback.h"
#include "DataLog.h"
#include "BSP_W5500.h"

int	TitleBar_Create( HANDLE handle, int ch, char *buf )//핸들과 USART 채널 정보에 대한 데이터를 출력한다.
{
	//현재 화면의 정보를 출력한다.
    if(ch==ADC_DATA_DISPLAY) {
		sprintf(buf,"%s","ADCInput");
	}
	else if(ch==10) {
        sprintf(buf,"%s","SENSOR");
    }
    else if(ch==11) {
        sprintf(buf,"%s","SYSTEM");
    }
    else if(ch==12) {
        sprintf(buf,"%s","TCP/IP DEBUG");
    }
	else if(flash.Uart[ch]->SensorID == SENSOR_YSI)//YSI센서 
	{
		sprintf(buf,"YSI/USART %d",ch + 1);                    
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_DEBUG)
	{
		sprintf(buf,"DEBUG/USART %d",ch + 1);                    
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_ARGADV)
	{
		sprintf(buf,"ARGADV/USART %d",ch + 1);                    
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_ADPSONTEK)
	{
		sprintf(buf,"ADP/USART %d",ch + 1);                    
	}	
	else if(flash.Uart[ch]->SensorID == SENSOR_UITNH4)
	{
		sprintf(buf,"UITNH4/USART %d",ch + 1);                    
	}	
	else if(flash.Uart[ch]->SensorID == SENSOR_TN)//TN
	{
		sprintf(buf,"TN/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_TP)//TP
	{
		sprintf(buf,"TP/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_WIZ)//WIZ
	{
		sprintf(buf,"WIZ/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_AUTO_SAMPLER)//AUTO_SAMPLER
	{
		sprintf(buf,"AUTO_SAM/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_EXO)//SENSOR_EXO
	{
		sprintf(buf,"EXO / USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_HFTOL)//SENSOR_HFTOL
	{
		sprintf(buf,"HF-TOL/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_HFCLX)//SENSOR_HFCLX
	{
		//sprintf(buf,"HF-CLX/USART %d",ch + 1);
		sprintf(buf,"CLX-EX/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_SONTEK)//SENSOR_SONTEK
	{
		sprintf(buf,"SONTEK/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_SCAN)//SCAN_SONTEK
	{
		sprintf(buf,"SCAN/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_WINCH)
	{
		sprintf(buf,"WINCH/USART %d",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_SAMPLER)
	{
		sprintf(buf,"SAMPLER/USART %d",ch + 1);
	}
	/*
	else if(flash.Nitrogen_Control==ON)
	{
		sprintf(buf,"NITROGEN");
		gWizTimeDisp=0;
	}
	*/
	else if(flash.Uart[ch]->SensorID == SENSOR_INTELL)
	{
		sprintf(buf,"INTELLITECT");
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_FLOW)
	{
		sprintf(buf,"FLOW");
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_PONSEL_DO)
	{
		#ifdef NORMAL_PORT
			sprintf(buf,"PONSEL_DO / USART_3");
		#endif
		#ifdef MUJU_SEWER
			sprintf(buf,"무주하수처리장");
		#endif
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_PONSEL_EC)
	{
		sprintf(buf,"PONSEL_EC/USART %d", ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_PONSEL_PH)
	{
		sprintf(buf,"PONSEL_pH");
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_ECD_PH)
	{
		sprintf(buf,"ECD pH");
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_PONSEL_PH_COND)
	{
		sprintf(buf,"PONSEL pH COND");
	}
    else if(flash.Uart[ch]->SensorID == SENSOR_AUTOSAMPLER)
    {
        sprintf(buf,"이설형지류지천수질모니터링");
    }
    else if(flash.Uart[ch]->SensorID == SENSOR_GPS)
    {
        sprintf(buf,"GPS / USART_%d", ch + 1);
    }
    #ifdef M_SERIES
	else if(flash.Uart[ch]->SensorID == SENSOR_MSERIES)
	{
		sprintf(buf,"Mseries");
		gWizTimeDisp=0;
	}
    #endif
	else if(flash.Uart[ch]->SensorID == SENSOR_NEP500)
	{
		sprintf(buf,"USART_%d / NEP500",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_SAMPLER100)
	{
		sprintf(buf,"USART_%d / AUTOSAMPLER",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_STATUS)
	{
		sprintf(buf,"USART_%d / 펌프상태,강우량",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_LDI)
	{
		sprintf(buf,"USART_%d / LDI",ch + 1);
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_ODO)
	{
		#if defined(YSI_ODD_ENABLE)
		sprintf(buf,"USART_%d / ODO",ch + 1);
		#endif
		#if defined(YSI_ODOCT_ENABLE)
		sprintf(buf,"USART_%d / ODOCT",ch + 1);
		#endif
		
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_SPECTRO_D)
	{
        #ifdef SPECTRO_D_ENABLE
		sprintf(buf,"USART_%d / SPECTRO-D",ch + 1);
        #endif
	}
	else if(flash.Uart[ch]->SensorID == SENSOR_SPECTRO_M) {
        #ifdef SPECTRO_M_ENABLE
		sprintf(buf,"USART_%d / SPECTOR-M",ch + 1);
        #endif
	}
	else 
	{
		Dprintf("TITLEBAR NAME UART_%d ID = %d\n", ch, flash.Uart[ch]->SensorID);
		sprintf(buf,"NONE");
	}
	return 1;	
}
