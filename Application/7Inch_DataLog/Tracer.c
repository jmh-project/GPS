#include "TEECO_System.h"
#include "DataLog.h"
#if defined(BATTERY_ENABLE)
void TracerBatteryRequest(int comPort, int info) {
	unsigned char aCrcBuf[32];	
	//unsigned int nCrcCount=0;
	unsigned int nReturnCrc16;
    if(info==0) {        //사용전압 :: LOAD VOLTAGE
		aCrcBuf[0]=0x01;
		aCrcBuf[1]=0x04;
		aCrcBuf[2]=0x31;
		aCrcBuf[3]=0x0C;
		aCrcBuf[4]=0x00;
		aCrcBuf[5]=0x02;
//        Uart_TransmitPort(comPort, 0x01);
//        Uart_TransmitPort(comPort, 0x04);
//        Uart_TransmitPort(comPort, 0x31);
//        Uart_TransmitPort(comPort, 0x0C);
//        Uart_TransmitPort(comPort, 0x00);
//        Uart_TransmitPort(comPort, 0x02);
//        Uart_TransmitPort(comPort, 0xBF);
//        Uart_TransmitPort(comPort, 0x34);
    }
    else if(info==1) {   //사용전류
		aCrcBuf[0]=0x01;
		aCrcBuf[1]=0x04;
		aCrcBuf[2]=0x31;
		aCrcBuf[3]=0x0D;
		aCrcBuf[4]=0x00;
		aCrcBuf[5]=0x02;
//        Uart_TransmitPort(comPort, 0x01);
//        Uart_TransmitPort(comPort, 0x04);
//        Uart_TransmitPort(comPort, 0x31);
//        Uart_TransmitPort(comPort, 0x0D);//0X0E
//        Uart_TransmitPort(comPort, 0x00);
//        Uart_TransmitPort(comPort, 0x02);
//        Uart_TransmitPort(comPort, 0x1E);
//        Uart_TransmitPort(comPort, 0xF4);
    }
    else if(info==2) {   //배터리잔량
		aCrcBuf[0]=0x01;
		aCrcBuf[1]=0x04;
		aCrcBuf[2]=0x31;
		aCrcBuf[3]=0x1A;
		aCrcBuf[4]=0x00;
		aCrcBuf[5]=0x02;
//        Uart_TransmitPort(comPort, 0x01);
//        Uart_TransmitPort(comPort, 0x04);
//        Uart_TransmitPort(comPort, 0x31);
//        Uart_TransmitPort(comPort, 0x1A);
//        Uart_TransmitPort(comPort, 0x00);
//        Uart_TransmitPort(comPort, 0x02);
//        Uart_TransmitPort(comPort, 0x5E);
//        Uart_TransmitPort(comPort, 0xF0);
    }
    else if(info==3) {   //배터리전압
		aCrcBuf[0]=0x01;
		aCrcBuf[1]=0x04;
		aCrcBuf[2]=0x33;
		aCrcBuf[3]=0x1A;
		aCrcBuf[4]=0x00;
		aCrcBuf[5]=0x02;
//        Uart_TransmitPort(comPort, 0x01);
//        Uart_TransmitPort(comPort, 0x04);
//        Uart_TransmitPort(comPort, 0x33);
//        Uart_TransmitPort(comPort, 0x1A);
//        Uart_TransmitPort(comPort, 0x00);
//        Uart_TransmitPort(comPort, 0x02);
//        Uart_TransmitPort(comPort, 0x5F);
//        Uart_TransmitPort(comPort, 0x48);
    }		
	for(int i=0; i<6; i++) {
		Uart_TransmitPort(comPort, aCrcBuf[i]);
	}
	nReturnCrc16=(crc16(aCrcBuf,6));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);
	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
//	Dprintf("SEND %d\n",info);
}
int TrancerParsing(int id, int comPort, int info) {
    int value=0;
    if(gv.nFlag_ReceiveDataProcess[comPort]==ENABLE) {
        gv.nFlag_ReceiveDataProcess[comPort]=DISABLE;
        gv.comportConnectionError[comPort]=0;//통신에러삭제
        if(info==0) {            
			value = (gv.aItemReceiveBuffer[comPort][3] << 8) | gv.aItemReceiveBuffer[comPort][4];
            if(value != 0) {
				flash.Uart[USART_1]->SensorPV[8] = (float)value/100.0f;
			}
            info=1;
        }
        else if(info==1) {
            value = (gv.aItemReceiveBuffer[comPort][3] << 8) | gv.aItemReceiveBuffer[comPort][4];
			if(value != 0) {
				flash.Uart[USART_1]->SensorPV[9]= (float)value/100.0f;
			}
            info=2;
        }
        else if(info==2) {
            value = (gv.aItemReceiveBuffer[comPort][3] << 8) | gv.aItemReceiveBuffer[comPort][4];
			if(value != 0) {
				flash.Uart[USART_1]->SensorPV[10]= (float)value;
			}
            info=3;
        }
        else if(info==3) {
            value = (gv.aItemReceiveBuffer[comPort][3] << 8) | gv.aItemReceiveBuffer[comPort][4];
            if(value != 0) {
				flash.Uart[USART_1]->SensorPV[11]=(float)value/100.0;
			}
            info=0;
        }
		//Dprintf("RX %d PV = %.2f\n",info,flash.Uart[USART_1]->SensorPV[8+info]);
    }
    return info;
}
int TrancerReveive(int id, int comPort, int ascii) {
	unsigned int		nReturnCrc16=0;
	static 	char		oldAscii=0;
    unsigned char       buf[2];
    
	if((oldAscii == id) && (ascii == 0x04)) {
		gv.nFlag_ReceiveStart[comPort]=1;
		gv.nReceiveBufferCount[comPort]=0;
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = oldAscii;
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = ascii;
	}
	else if(gv.nFlag_ReceiveStart[comPort]==1) {
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = ascii;
        
		if(gv.nReceiveBufferCount[comPort]==9) {
			nReturnCrc16=(crc16(gv.aItemReceiveBuffer[comPort],7));
			buf[0]=(nReturnCrc16>>8)&0x00FF;
			buf[1]=(nReturnCrc16&0x00FF);
			
			if((buf[0]==gv.aItemReceiveBuffer[comPort][7])&&(buf[1]==gv.aItemReceiveBuffer[comPort][8])) {
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
#endif

