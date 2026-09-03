#include "TEECO_System.h"
#include "DataLog.h"

#ifdef MODBUS_ENABLE
int ReceiveModbus( int ascii, int comPort ) {
	unsigned int		nReturnCrc16=0;
	static 	char		oldAscii=0;
    unsigned char       buf[2];
    
	if((oldAscii == 0x01) && (ascii == 0x04)) {
		gv.nFlag_ReceiveStart[comPort]=1;
		gv.nReceiveBufferCount[comPort]=0;
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = oldAscii;
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = ascii;
	}
	else if(gv.nFlag_ReceiveStart[comPort]==1) {
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = ascii;
        
		if(gv.nReceiveBufferCount[comPort]==8) {
			nReturnCrc16=(crc16(gv.aItemReceiveBuffer[comPort],6));
			buf[0]=(nReturnCrc16>>8)&0x00FF;
			buf[1]=(nReturnCrc16&0x00FF);
			
			if((buf[0]==gv.aItemReceiveBuffer[comPort][6])&&(buf[1]==gv.aItemReceiveBuffer[comPort][7])) {
				gv.nFlag_ReceiveDataProcess[comPort] = ENABLE;
			} else {
                Dprintf("MODBUS CRC 16 ERROR\n");
            }
			gv.nFlag_ReceiveStart[comPort]=0;
			gv.nReceiveBufferCount[comPort]=0;
		}
		if(gv.nReceiveBufferCount[comPort] > 49) {
			gv.nFlag_ReceiveStart[comPort]=0;
			gv.nReceiveBufferCount[comPort]=0;
			gv.nFlag_ReceiveDataProcess[comPort]=DISABLE;
		}
	}
	oldAscii = ascii;
	return 1;
}
int SendModbus(unsigned int comPort) {
	unsigned char aCrcBuf[500];	
	unsigned int nCrcCount=0;
	unsigned int nReturnCrc16;
    float pv;
    U32 *address;
    void *addr1; 
    addr1 = &pv;
    address = addr1;

	Uart_TransmitPort(comPort, 0x01);                                   aCrcBuf[nCrcCount++] = 0x01;
	Uart_TransmitPort(comPort, 0x04);                                   aCrcBuf[nCrcCount++] = 0x04;
	Uart_TransmitPort(comPort, gv.aItemReceiveBuffer[comPort][5]*2);    aCrcBuf[nCrcCount++] = gv.aItemReceiveBuffer[comPort][5]*2;

	for(int uartChannel=0;uartChannel<USART_MAX;uartChannel++) {
		if(flash.Uart[uartChannel]->SensorStatus==ENABLE) {
			for(int itemCnt=0;itemCnt<USART_SENSOR_MAX;itemCnt++) {
				if( strcmp(flash.Uart[uartChannel]->SetupName[itemCnt],"NONE")!=0) {
                    pv=flash.Uart[uartChannel]->SensorPV[itemCnt];
                    aCrcBuf[nCrcCount++]=(*address>>8)&0xFF;		
                    Uart_TransmitPort(comPort, (*address>>8)&0xFF);
                    aCrcBuf[nCrcCount++]=*address&0xFF;			
                    Uart_TransmitPort(comPort, *address&0xFF);
                    aCrcBuf[nCrcCount++]=(*address>>24)&0xFF;	
                    Uart_TransmitPort(comPort, (*address>>24)&0xFF);
                    aCrcBuf[nCrcCount++]=(*address>>16)&0xFF;	
                    Uart_TransmitPort(comPort, (*address>>16)&0xFF);
				}
				else itemCnt=USART_SENSOR_MAX;
			}
		}
	}
	nReturnCrc16=(crc16(aCrcBuf,nCrcCount));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);
	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    if(nCrcCount >= 500) Dprintf("[ ERROR ] SendModbus = Range over = %d\n",nCrcCount);
	return (1);	
}
#endif
