#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"


int EventMessage( char *message ) {
    for(int i=0;i<SMARTLOG_DATALOG_SIZE;i++) {
        gv.dataLogBuf[i]=0;
    }
    DS1302TimeRead();
    sprintf((char *)gv.dataLogBuf,"20%02d/%02d/%02d %02d:%02d:%02d,", currentDate.year,  currentDate.month, currentDate.date, currentDate.hour,  currentDate.minute, currentDate.sec);
    strcat((char *)gv.dataLogBuf, message);
    strcat((char *)gv.dataLogBuf,",\n");
    int bufSize = strlen((char *)gv.dataLogBuf);
    
	char eventFileName[64];
	sprintf(eventFileName,"1:EventMessage%02d%02d.csv",currentDate.year,  currentDate.month);
    SD_StructFileWrite(eventFileName, bufSize);
    Dprintf("[ 20%02d/%02d/%02d %02d:%02d:%02d ]   %s Size = %d [Á¤º¸] %s\n",
    currentDate.year,currentDate.month,currentDate.date,currentDate.hour,currentDate.minute,currentDate.sec,eventFileName,bufSize,gv.dataLogBuf);
    return (1);
}


