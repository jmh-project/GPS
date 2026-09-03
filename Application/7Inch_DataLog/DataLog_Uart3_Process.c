#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"

int Uart3ReceiveProcess(void) {
    int comPort = USART_3;
    char buf[64];
    if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG) {
        sprintf(buf,"Debug Uart%d ABCDEFGHIJKLMNOPQRSTUVWXYZ\n\r", comPort+1);
        TransmitStringData(buf, comPort);
    } 
    return 0;
}

