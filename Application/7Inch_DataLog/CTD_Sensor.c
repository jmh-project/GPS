#include "TEECO_System.h"
#include "DataLog.h"
#if defined(CTD_ENABLE)
void CTD_Read(int deviceId, int comPort) {
    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;

    gv.nFlag_ReceiveStart[comPort]=0;       //통신수신관련초기화
    
    crcBuf[count++]=deviceId;
    crcBuf[count++]=0x03;
    crcBuf[count++]=0x90;
    crcBuf[count++]=0x00;
    crcBuf[count++]=0x00;
    crcBuf[count++]=0x12;

    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(deviceId, comPort);    //device id
    TransmitCharData(0x03, comPort);        //function code
    TransmitCharData(0x90, comPort);        //address
    TransmitCharData(0x00, comPort);
    TransmitCharData(0x00, comPort);        //data no
    TransmitCharData(0x12, comPort);    
    TransmitCharData(buf[0], comPort);    
    TransmitCharData(buf[1], comPort);    
}
//CTD_Write(RS485_1,protocol.deviceId,DEPTH_WRITE,flash.calDepthBuffer);
void CTD_Write(int comPort, int id, int addsh, int addsl, unsigned char cmd, float value) {
	int cnt = 0;
    unsigned char	crcBuf[20];
	unsigned int 	nReturnCrc16=0;
    
    float pv = 0.0;
    U32 *address;
    void *addr1; 
    addr1 = &pv;
    address = addr1;
	
    if(value == -1) {
        pv=-1;
    }
    else {
        if(cmd==EC_WRITE) {
            pv=value;
        }
        else {
            pv=value;
        }
    }
    
	crcBuf[cnt++]=id;//id
	crcBuf[cnt++]=0x10;//function code
	switch(cmd)
	{
		case ATM_WRITE:
			break;
		case EC_WRITE:
            crcBuf[cnt++]=addsh;//address
            crcBuf[cnt++]=addsl;
            crcBuf[cnt++]=0x00;//reg cnt
            crcBuf[cnt++]=0x02;
			Dprintf("COND CAL uS = %.1f\n",pv);
			break;
		case DEPTH_WRITE:
            crcBuf[cnt++]=addsh;//address
            crcBuf[cnt++]=addsl;
            crcBuf[cnt++]=0x00;//reg cnt
            crcBuf[cnt++]=0x02;
			Dprintf("DEPTH CAL uS = %.1f\n",pv);
			break;
	}
	crcBuf[cnt++]=0x04;//데이터길이
	if(pv==-1) {
        crcBuf[cnt++]=0xBF;
        crcBuf[cnt++]=0x80;
        crcBuf[cnt++]=0x00;
        crcBuf[cnt++]=0x00;
    }
    else {     
        crcBuf[cnt++]=(*address>>24)&0xFF;
        crcBuf[cnt++]=(*address>>16)&0xFF;//교정값
        crcBuf[cnt++]=(*address>>8)&0xFF;
        crcBuf[cnt++]=(*address&0xFF);
    }
    nReturnCrc16=(crc16(crcBuf,cnt));
	crcBuf[cnt++]=(nReturnCrc16>>8)&0x00FF;
	crcBuf[cnt++]=(nReturnCrc16&0x00FF);
    for(int i=0; i<cnt; i++) {
        TransmitCharData(crcBuf[i], comPort);   
        //Dprintf("%02x ",crcBuf[i]);
    }
}
void CTD_Parsing(int comPort) {
	char buf[64];
	//int bufCnt=0;
	//int cnt = 0;
	int itemCnt=0;
	//int byteNo=0;
	//float fValue;
	float   mV=0.0;
    
    U32 *address;
    void *addr1; 
    addr1 = &mV;
    address = addr1;

    itemCnt=3;
    /*
        수온
    */
    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];

    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);     
    gv.ctdPv[0]=mV;
    /*
        전도도 mS -> uS 전환
    */
    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];  
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];		
    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]); 
    gv.ctdPv[1]=mV;
    //Dprintf("전도도(25) = %.2f\n",mV);
    
    
    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];		
    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]); 
    //flash.Uart[comPort]->SensorPV[5]=mV;
    gv.ctdPv[2]=mV;
    flash.Uart[comPort]->SensorPV[5]=mV;
    //Dprintf("전도도 = %.2f\n",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];		

    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]); 
    gv.ctdPv[3]=mV;
    //Dprintf("염분 = %.2f\n",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];		

    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]); 
    gv.ctdPv[4]=mV;
    //Dprintf("저항 = %.2f\n",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];		
    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]); 
    gv.ctdPv[5]=mV;
    //Dprintf("TDS = %.2f\n",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];		
    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);
    gv.ctdPv[6]=mV;
    //Dprintf("DEPTH = %.2f\n",mV);
    flash.Uart[comPort]->SensorPV[6]=mV;
    
    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];		
    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);
    gv.ctdPv[7]=mV;
    flash.Uart[comPort]->SensorPV[7]=mV;
    //Dprintf("BARO(Depth) = %.2f\n",mV);
    
    buf[0]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][itemCnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][itemCnt++];		
    *address = ((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);
    
    gv.ctdPv[8]=mV;
    //Dprintf("BARO(내부) = %.2f\n",mV);
}
#endif
