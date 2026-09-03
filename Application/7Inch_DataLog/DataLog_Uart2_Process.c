#include "TEECO_System.h"
#include "DataLog.h"
int Uart2ReceiveProcess(void) {	
    int comPort = USART_2;
    if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG) {
        char buf[64];
        sprintf(buf,"Debug Uart%d ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\r", comPort+1);
        TransmitStringData(buf, comPort);
    } 
    #if defined(BATTERY_ENABLE)	
    gv.requestBatteryInfoNo=TrancerParsing(0x01, comPort, gv.requestBatteryInfoNo);	
    TracerBatteryRequest(comPort, gv.requestBatteryInfoNo);//gv.requestBatteryInfoNo 데이터 수신 후 증가한다.    
	#endif
    return 0;
}
