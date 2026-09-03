#include "TEECO_System.h"
#include "DataLog.h"
#include "time.h"

#if defined(YSI_ODOCT_ENABLE) || defined(YSI_ODD_ENABLE)
int odo_RequestMeasure(int id, int addr, int len, int comPort);
U32 ODO_PROCESS;
extern time_t baseTime;
extern int gNewID;
extern int gCapUpdateStep;
int ODO_IOPrepareEvent2(HANDLE handle, int command, int param, void *Data) {
	stIOProcess 	*io_process = FindIOProcess(handle);
	if(io_process==NULL) 
	{
		Dprintf("IOPrepareEvent:: IO Precess not Found\n");
		return(0);
	}
	ODO_PROCESS = IOOpenTimer(handle, 500);//mS
	IOStartTimer(ODO_PROCESS);
    Dprintf("ODO IOProcess Start\n");
	return(1);
}
int ODO_IOTimerEvent2(HANDLE handle, int command, int param, void *Data) { 
    stIOProcess 	*io_process = FindIOProcess(handle);
	if(io_process==NULL) 
	{
		Dprintf("IOTimerEvent:: IO Precess not Found\n");
		return(0);
	}
    if(gv.request_DataFlag==ON) {  
        odo_RequestMeasure( gv.modbusId, 0x0000, 0x0016, USART_1);//ODO / CT 측정값 요청 
        //Dprintf("odo_RequestMeasure :: ID = %02x\n", gv.modbusId);
    }
    return 0;
}
int ODO_IOProcess(void) {
    Dprintf("[ENABLE] ODO_IOProcess\n");
	makeIOProcess("ODO", ODO_IOPrepareEvent2, ODO_IOTimerEvent2);
	return(1);	
}
int odo_Status(int id, int addr, int len, int comPort) {
    unsigned char functionCode = 0x03;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hilen = (len & 0xFF00)>>8;
    unsigned char lolen = (len & 0x00FF);
    unsigned char crcBuf[20];
	unsigned int  count=0;
	unsigned char buf[2];
	unsigned int  nReturnCrc16=0;

    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hilen;
    crcBuf[count++]=lolen;
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hilen, comPort);
    TransmitCharData(lolen, comPort);
    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
int odo_Cap_Coefficient(int id, int addr, int len, int comPort) {
    
    unsigned char functionCode = 0x03;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hilen = (len & 0xFF00)>>8;
    unsigned char lolen = (len & 0x00FF);
    unsigned char crcBuf[20];
	unsigned int  count=0;
	unsigned char buf[2];
	unsigned int  nReturnCrc16=0;

    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hilen;
    crcBuf[count++]=lolen;
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hilen, comPort);
    TransmitCharData(lolen, comPort);
    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
int odo_Cap_CoefficientSet(int id, int addr, int num, int cnt, int comPort) {
    
    unsigned char functionCode = 0x10;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hilen = (num & 0xFF00)>>8;
    unsigned char lolen = (num & 0x00FF);
    unsigned char crcBuf[128];
	unsigned int  count=0;
    unsigned int  count2=0;
	unsigned char buf[2];
	unsigned int  nReturnCrc16=0;

    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hilen;
    crcBuf[count++]=lolen;
    crcBuf[count++]=cnt;
    
    crcBuf[count++]=(gv.K1 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K1 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K1 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K1 & 0x000000FF;
    
    crcBuf[count++]=(gv.K2 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K2 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K2 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K2 & 0x000000FF;

    crcBuf[count++]=(gv.K3 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K3 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K3 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K3 & 0x000000FF;

    crcBuf[count++]=(gv.K4 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K4 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K4 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K4 & 0x000000FF;

    crcBuf[count++]=(gv.K5 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K5 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K5 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K5 & 0x000000FF;

    crcBuf[count++]=(gv.K6 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K6 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K6 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K6 & 0x000000FF;

    crcBuf[count++]=(gv.K7 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K7 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K7 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K7 & 0x000000FF;

    crcBuf[count++]=(gv.KC >> 8) & 0x000000FF;
    crcBuf[count++]=gv.KC & 0x000000FF;

    crcBuf[count++]=0;
    crcBuf[count++]=0;
    crcBuf[count++]=0;
    crcBuf[count++]=0;
    
    nReturnCrc16=(crc16(crcBuf,count));//CRC16계산
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    count=0;
    crcBuf[count++]=(gv.K1 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K1 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K1 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K1 & 0x000000FF;
    
    crcBuf[count++]=(gv.K2 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K2 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K2 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K2 & 0x000000FF;

    crcBuf[count++]=(gv.K3 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K3 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K3 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K3 & 0x000000FF;

    crcBuf[count++]=(gv.K4 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K4 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K4 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K4 & 0x000000FF;

    crcBuf[count++]=(gv.K5 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K5 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K5 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K5 & 0x000000FF;

    crcBuf[count++]=(gv.K6 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K6 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K6 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K6 & 0x000000FF;

    crcBuf[count++]=(gv.K7 >> 24) & 0x000000FF;
    crcBuf[count++]=(gv.K7 >> 16) & 0x000000FF;
    crcBuf[count++]=(gv.K7 >> 8) & 0x000000FF;
    crcBuf[count++]=gv.K7 & 0x000000FF;

    crcBuf[count++]=(gv.KC >> 8) & 0x000000FF;
    crcBuf[count++]=gv.KC & 0x000000FF;

    crcBuf[count++]=0;
    crcBuf[count++]=0;
    crcBuf[count++]=0;
    crcBuf[count++]=0;
   
    //송신
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hilen, comPort);
    TransmitCharData(lolen, comPort);
    TransmitCharData(cnt, comPort);
    
    TransmitCharData(crcBuf[0], comPort);
    TransmitCharData(crcBuf[1], comPort);
    TransmitCharData(crcBuf[2], comPort);
    TransmitCharData(crcBuf[3], comPort);

    TransmitCharData(crcBuf[4], comPort);
    TransmitCharData(crcBuf[5], comPort);
    TransmitCharData(crcBuf[6], comPort);
    TransmitCharData(crcBuf[7], comPort);

    TransmitCharData(crcBuf[8], comPort);
    TransmitCharData(crcBuf[9], comPort);
    TransmitCharData(crcBuf[10], comPort);
    TransmitCharData(crcBuf[11], comPort);

    TransmitCharData(crcBuf[12], comPort);
    TransmitCharData(crcBuf[13], comPort);
    TransmitCharData(crcBuf[14], comPort);
    TransmitCharData(crcBuf[15], comPort);
    
    TransmitCharData(crcBuf[16], comPort);
    TransmitCharData(crcBuf[17], comPort);
    TransmitCharData(crcBuf[18], comPort);
    TransmitCharData(crcBuf[19], comPort);

    TransmitCharData(crcBuf[20], comPort);
    TransmitCharData(crcBuf[21], comPort);
    TransmitCharData(crcBuf[22], comPort);
    TransmitCharData(crcBuf[23], comPort);

    TransmitCharData(crcBuf[24], comPort);
    TransmitCharData(crcBuf[25], comPort);
    TransmitCharData(crcBuf[26], comPort);
    TransmitCharData(crcBuf[27], comPort);

    TransmitCharData(crcBuf[28], comPort);
    TransmitCharData(crcBuf[29], comPort);

    TransmitCharData(crcBuf[30], comPort);
    TransmitCharData(crcBuf[31], comPort);
    TransmitCharData(crcBuf[32], comPort);
    TransmitCharData(crcBuf[33], comPort);

    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);

    Dprintf("CAP1 = %02x %02x %02x %02x\n",crcBuf[0],crcBuf[1],crcBuf[2],crcBuf[3]);
    Dprintf("CAP2 = %02x %02x\n",crcBuf[28],crcBuf[29]);

    return 0;
}

int odo_SlaveAddressGet(int id, int addr, int setAddress, int comPort) {
    unsigned char functionCode = 0x03;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hinum = (setAddress & 0xFF00)>>8;
    unsigned char lonum = (setAddress & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;
    unsigned char   byte4[4];
    
    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hinum;
    crcBuf[count++]=lonum;
    
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hinum, comPort);
    TransmitCharData(lonum, comPort);
    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
int odo_SalinitySet(int id, int addr, int num, unsigned char cnt, int item, int comPort ) {
    unsigned char functionCode = 0x10;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hinum = (num & 0xFF00)>>8;
    unsigned char lonum = (num & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;
    unsigned char   byte4[4];
    
    float	fdata;
    U32		*address;
    void	*addr1;
    U8		a=0, b=0, c=0, d=0, e=0;
    address = addr1 = &fdata;
    
    if(item==1) {
        Dprintf("설정된 염분값 = %.2f\n",flash.odoct.salPv);
        fdata=flash.odoct.salPv;   //염분보상적용
        a=(*address>>24) & 0xFF;
        b=(*address>>16) & 0xFF;
        c=(*address>>8)  & 0xFF;
        d=*address 	     & 0xFF;
        byte4[0]=a;
        byte4[1]=b;
        byte4[2]=c;
        byte4[3]=d;
    } else if(item==2) {          
        fdata=flash.odoct.barometerPv;   //고도보상적용
        Dprintf("고도보상값 = %.1f\n",flash.odoct.barometerPv);
        a=(*address>>24) & 0xFF;
        b=(*address>>16) & 0xFF;
        c=(*address>>8)  & 0xFF;
        d=*address 	     & 0xFF;
        byte4[0]=a;
        byte4[1]=b;
        byte4[2]=c;
        byte4[3]=d;
    } else if(item==3) {          
        fdata=flash.odoct.tempCalPv;   //온도보상
        a=(*address>>24) & 0xFF;
        b=(*address>>16) & 0xFF;
        c=(*address>>8)  & 0xFF;
        d=*address 	     & 0xFF;
        byte4[0]=a;
        byte4[1]=b;
        byte4[2]=c;
        byte4[3]=d;
    }
    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hinum;
    crcBuf[count++]=lonum;
    crcBuf[count++]=cnt;
    crcBuf[count++]=byte4[0];
    crcBuf[count++]=byte4[1];
    crcBuf[count++]=byte4[2];
    crcBuf[count++]=byte4[3];
    
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    //기본코드
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hinum, comPort);
    TransmitCharData(lonum, comPort);
    TransmitCharData(cnt, comPort);
    //교정값
    TransmitCharData(byte4[0], comPort);
    TransmitCharData(byte4[1], comPort);
    TransmitCharData(byte4[2], comPort);
    TransmitCharData(byte4[3], comPort);
    //검정코드
    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
int odo_SlaveAddressSet(int id, int addr, int setAddress, int comPort) {
    unsigned char functionCode = 0x06;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hinum = (setAddress & 0xFF00)>>8;
    unsigned char lonum = (setAddress & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;
    unsigned char   byte4[4];
    
    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hinum;
    crcBuf[count++]=lonum;
    
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hinum, comPort);
    TransmitCharData(lonum, comPort);
    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
int odo_ZeroCal(int id, int addr, int num, unsigned char cnt, int item, int comPort ) {//ZERO 교정
    unsigned char functionCode = 0x10;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hinum = (num & 0xFF00)>>8;
    unsigned char lonum = (num & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;
    unsigned char   byte4[4];
    
    switch(item) {
        case 1:
            byte4[0]=(baseTime >> 24) & 0x000000FF;
            byte4[1]=(baseTime >> 16) & 0x000000FF;
            byte4[2]=(baseTime >> 8) & 0x000000FF;
            byte4[3]=baseTime & 0x000000FF;
            break;
    }

    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hinum;
    crcBuf[count++]=lonum;
    crcBuf[count++]=cnt;
    crcBuf[count++]=byte4[0];
    crcBuf[count++]=byte4[1];
    crcBuf[count++]=byte4[2];
    crcBuf[count++]=byte4[3];
    
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hinum, comPort);
    TransmitCharData(lonum, comPort);
    TransmitCharData(cnt, comPort);
    
    TransmitCharData(byte4[0], comPort);
    TransmitCharData(byte4[1], comPort);
    TransmitCharData(byte4[2], comPort);
    TransmitCharData(byte4[3], comPort);
    
    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
//Percent Saturation 교정
//대기중에서 보정시 염분보정이 안된다.
int odo_AirCal(int id, int addr, int num, unsigned char cnt, int item, int comPort ) {//mg/L 교정
    unsigned char functionCode = 0x10;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hinum = (num & 0xFF00)>>8;
    unsigned char lonum = (num & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;
    unsigned char   byte8[10];
    
    float	fdata;
    U32		*address;
    void	*addr1;
    U8		a=0, b=0, c=0, d=0, e=0;
    address = addr1 = &fdata;
    
    fdata=flash.odoct.barometerPv; //대기압
    a=(*address>>24) & 0xFF;
    b=(*address>>16) & 0xFF;
    c=(*address>>8)  & 0xFF;
    d=*address 	     & 0xFF;

    byte8[0]=(baseTime >> 24) & 0x000000FF;
    byte8[1]=(baseTime >> 16) & 0x000000FF;
    byte8[2]=(baseTime >> 8) & 0x000000FF;
    byte8[3]=baseTime & 0x000000FF;

    byte8[4]=a;
    byte8[5]=b;
    byte8[6]=c;
    byte8[7]=d;

    count=0;
    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hinum;
    crcBuf[count++]=lonum;
    crcBuf[count++]=cnt;
    //교정일.설정값
    crcBuf[count++]=byte8[0];
    crcBuf[count++]=byte8[1];
    crcBuf[count++]=byte8[2];
    crcBuf[count++]=byte8[3];
    //대기압.설정값
    crcBuf[count++]=byte8[4];
    crcBuf[count++]=byte8[5];
    crcBuf[count++]=byte8[6];
    crcBuf[count++]=byte8[7];
    
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    gv.nFlag_ReceiveStart[comPort]=0;
    gv.nReceiveBufferCount[comPort]=0;
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hinum, comPort);
    TransmitCharData(lonum, comPort);
    TransmitCharData(cnt, comPort);
    
    TransmitCharData(byte8[0], comPort);
    TransmitCharData(byte8[1], comPort);
    TransmitCharData(byte8[2], comPort);
    TransmitCharData(byte8[3], comPort);

    TransmitCharData(byte8[4], comPort);
    TransmitCharData(byte8[5], comPort);
    TransmitCharData(byte8[6], comPort);
    TransmitCharData(byte8[7], comPort);

    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    Dprintf("[odo_AirCal] Barometric pressure = %.1f 교정명령송신완료\n", fdata);
    return 0;
}
int odo_mgLCal(int id, int addr, int num, unsigned char cnt, int item, int comPort ) {
    unsigned char functionCode = 0x10;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hinum = (num & 0xFF00)>>8;
    unsigned char lonum = (num & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;
    unsigned char   byte12[12];
    
    float	fdata;
    U32		*address;
    void	*addr1;
    U8		a=0, b=0, c=0, d=0, e=0;
    address = addr1 = &fdata;
    

    switch(item) {
        case 1:
            
            byte12[0]=(baseTime >> 24) & 0x000000FF;
            byte12[1]=(baseTime >> 16) & 0x000000FF;
            byte12[2]=(baseTime >> 8) & 0x000000FF;
            byte12[3]=baseTime & 0x000000FF;
        
            fdata=flash.odoct.odoCalPv;   //교정값
            a=(*address>>24) & 0xFF;
            b=(*address>>16) & 0xFF;
            c=(*address>>8)  & 0xFF;
            d=*address 	     & 0xFF;

            byte12[4]=a;
            byte12[5]=b;
            byte12[6]=c;
            byte12[7]=d;

            fdata=0.0;      //염분
            a=(*address>>24) & 0xFF;
            b=(*address>>16) & 0xFF;
            c=(*address>>8)  & 0xFF;
            d=*address 	     & 0xFF;

            byte12[8]=a;
            byte12[9]=b;
            byte12[10]=c;
            byte12[11]=d;
            break;
    }
    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hinum;
    crcBuf[count++]=lonum;
    crcBuf[count++]=cnt;
    //교정일
    crcBuf[count++]=byte12[0];
    crcBuf[count++]=byte12[1];
    crcBuf[count++]=byte12[2];
    crcBuf[count++]=byte12[3];
    //교정값
    crcBuf[count++]=byte12[4];
    crcBuf[count++]=byte12[5];
    crcBuf[count++]=byte12[6];
    crcBuf[count++]=byte12[7];
    //염분값
    crcBuf[count++]=byte12[8];
    crcBuf[count++]=byte12[9];
    crcBuf[count++]=byte12[10];
    crcBuf[count++]=byte12[11];
    
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hinum, comPort);
    TransmitCharData(lonum, comPort);
    TransmitCharData(cnt, comPort);
    
    TransmitCharData(byte12[0], comPort);
    TransmitCharData(byte12[1], comPort);
    TransmitCharData(byte12[2], comPort);
    TransmitCharData(byte12[3], comPort);

    TransmitCharData(byte12[4], comPort);
    TransmitCharData(byte12[5], comPort);
    TransmitCharData(byte12[6], comPort);
    TransmitCharData(byte12[7], comPort);

    TransmitCharData(byte12[8], comPort);
    TransmitCharData(byte12[9], comPort);
    TransmitCharData(byte12[10], comPort);
    TransmitCharData(byte12[11], comPort);

    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
int cond_mgLCal(int id, int addr, int num, unsigned char cnt, int item, int comPort ) {//spcond 교정
    unsigned char functionCode = 0x10;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hinum = (num & 0xFF00)>>8;
    unsigned char lonum = (num & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;
    unsigned char   byte12[12];
    
    float	fdata;
    U32		*address;
    void	*addr1;
    U8		a=0, b=0, c=0, d=0, e=0;
    address = addr1 = &fdata;

    byte12[0]=(baseTime >> 24) & 0x000000FF;
    byte12[1]=(baseTime >> 16) & 0x000000FF;
    byte12[2]=(baseTime >> 8) & 0x000000FF;
    byte12[3]=baseTime & 0x000000FF;
    
    if(item==0) {
        fdata=flash.odoct.condCalPv;   //교정값.전도도
        a=(*address>>24) & 0xFF;
        b=(*address>>16) & 0xFF;
        c=(*address>>8)  & 0xFF;
        d=*address 	     & 0xFF;
    } else if(item==1) {
        fdata=flash.odoct.saltCalPv;   //교정값.염분값
        a=(*address>>24) & 0xFF;
        b=(*address>>16) & 0xFF;
        c=(*address>>8)  & 0xFF;
        d=*address 	     & 0xFF;
    }
    byte12[4]=a;
    byte12[5]=b;
    byte12[6]=c;
    byte12[7]=d;

    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hinum;
    crcBuf[count++]=lonum;
    crcBuf[count++]=cnt;
    //교정일
    crcBuf[count++]=byte12[0];
    crcBuf[count++]=byte12[1];
    crcBuf[count++]=byte12[2];
    crcBuf[count++]=byte12[3];
    //교정값
    crcBuf[count++]=byte12[4];
    crcBuf[count++]=byte12[5];
    crcBuf[count++]=byte12[6];
    crcBuf[count++]=byte12[7];
    
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hinum, comPort);
    TransmitCharData(lonum, comPort);
    TransmitCharData(cnt, comPort);
    
    TransmitCharData(byte12[0], comPort);
    TransmitCharData(byte12[1], comPort);
    TransmitCharData(byte12[2], comPort);
    TransmitCharData(byte12[3], comPort);

    TransmitCharData(byte12[4], comPort);
    TransmitCharData(byte12[5], comPort);
    TransmitCharData(byte12[6], comPort);
    TransmitCharData(byte12[7], comPort);

    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
int odo_ReceiveData(int comPort, unsigned char ascii) {//데이터수신
    unsigned char		aCrcBuf[50];
	unsigned int		nReturnCrc16=0;
	static 	char		oldAscii=0;
    float	fdata;
    U32		*address;
    void	*addr1;
    U8		a=0, b=0, c=0, d=0, e=0;
    address = addr1 = &fdata;
	if((oldAscii == gv.modbusId) && (ascii == 0x04 || ascii == 0x03 || ascii == 0x10))
	{
		gv.nFlag_ReceiveStart[comPort]=1;
		gv.nReceiveBufferCount[comPort]=0;
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = oldAscii;
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = ascii;
	}
	else if(gv.nFlag_ReceiveStart[comPort]==1)        
	{
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++] = ascii;
        //gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]] = 0;
		if((gv.aItemReceiveBuffer[comPort][2]+5)==gv.nReceiveBufferCount[comPort])
		{
			nReturnCrc16=(crc16(gv.aItemReceiveBuffer[comPort],gv.nReceiveBufferCount[comPort]-2));
			aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
			aCrcBuf[1]=(nReturnCrc16&0x00FF);
			
			if((aCrcBuf[0]==gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]-2])&&(aCrcBuf[1]==gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]-1]))
			{
                if(gv.aItemReceiveBuffer[comPort][1]==0x04) {
                    //gv.nFlag_ReceiveDataProcess[comPort] = ENABLE;
                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][3] << 24)
                    | (gv.aItemReceiveBuffer[comPort][4] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][5] << 8)
                    | gv.aItemReceiveBuffer[comPort][6]);
                    
                    gv.satPv[gv.odo_ArrayAddress]=fdata;//saturation
                    //flash.Uart[USART_1]->SensorPV[0]=fdata;
                    
                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][7] << 24)
                    | (gv.aItemReceiveBuffer[comPort][8] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][9] << 8)
                    | gv.aItemReceiveBuffer[comPort][10]);
                    
                    gv.odoPv[gv.odo_ArrayAddress]=fdata;//odo mg/L
                    flash.Uart[USART_1]->SensorPV[1]=fdata;
                    
                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][11] << 24)
                    | (gv.aItemReceiveBuffer[comPort][12] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][13] << 8)
                    | gv.aItemReceiveBuffer[comPort][14]);
                    
                    gv.baroPv[gv.odo_ArrayAddress]=fdata;//Barometer
                   // flash.Uart[USART_1]->SensorPV[2]=fdata;
                    
                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][15] << 24)
                    | (gv.aItemReceiveBuffer[comPort][16] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][17] << 8)
                    | gv.aItemReceiveBuffer[comPort][18]);
                    
                    gv.tempPv[gv.odo_ArrayAddress]=fdata;//Temp 'c
                    flash.Uart[USART_1]->SensorPV[0]=fdata;
                    
                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][19] << 24)
                    | (gv.aItemReceiveBuffer[comPort][20] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][21] << 8)
                    | gv.aItemReceiveBuffer[comPort][22]); //ref temp

                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][23] << 24)
                    | (gv.aItemReceiveBuffer[comPort][24] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][25] << 8)
                    | gv.aItemReceiveBuffer[comPort][26]); //time since boot

                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][27] << 24)
                    | (gv.aItemReceiveBuffer[comPort][28] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][29] << 8)
                    | gv.aItemReceiveBuffer[comPort][30]); //Conductivity (us/cm)
                     

                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][31] << 24)
                    | (gv.aItemReceiveBuffer[comPort][32] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][33] << 8)
                    | gv.aItemReceiveBuffer[comPort][34]); //Specific Conductivity (us/cm)
                    gv.spcondPv[gv.odo_ArrayAddress]=fdata;
                    
                    if(flash.odoct.spcondUnit==0) {
                        flash.Uart[USART_1]->SensorPV[2]=fdata;
                    } else if(flash.odoct.spcondUnit==1){
                        flash.Uart[USART_1]->SensorPV[2]=fdata/1000.0;
                    }
                    
                    
                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][35] << 24)
                    | (gv.aItemReceiveBuffer[comPort][36] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][37] << 8)
                    | gv.aItemReceiveBuffer[comPort][38]); //Salinity (ppt) [3:0]
                    gv.salinityPv[gv.odo_ArrayAddress]=fdata;
                    //SmartLogFlash.Uart[USART_1]->SensorPV[2]=fdata;
                    if(flash.odoct.spcondUnit==2) {
                        flash.Uart[USART_1]->SensorPV[2]=fdata;
                    }                    
                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][39] << 24)
                    | (gv.aItemReceiveBuffer[comPort][40] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][41] << 8)
                    | gv.aItemReceiveBuffer[comPort][42]); //Conductivity nLF (us/cm) [3:0]
                    
                    
                    *address = 
                    ((gv.aItemReceiveBuffer[comPort][43] << 24)
                    | (gv.aItemReceiveBuffer[comPort][44] << 16) 
                    | (gv.aItemReceiveBuffer[comPort][45] << 8)
                    | gv.aItemReceiveBuffer[comPort][46]); //Total Dissolved Solids (mg/L) [3:0]
                    gv.tdsPv[gv.odo_ArrayAddress]=fdata;
                    
                } else if(gv.aItemReceiveBuffer[comPort][1]==0x03) {//ID변경값수신
                    if(gCapUpdateStep==0) {
                        gNewID=gv.aItemReceiveBuffer[comPort][4];
                    } else if(gCapUpdateStep==1) {//데이터요청명령을 내리면 수신
//                      for(int i=0; i<gv.aItemReceiveBuffer[comPort]Cnt; i++) {
//                           Dprintf("%02X ", gv.aItemReceiveBuffer[comPort][i]);
//                      }                          
//                    01 03 20 
//                    C0 0D E6 73 
//                    42 04 8E CD 
//                    40 BB AA E4 
//                    3C 7C F6 94 
//                    81 51 91 43 
//                    3C 80 D8 02 
//                    B8 66 AF CD 
//                    00 98 
//                    00 00 48 85 
                    gv.K1=((gv.aItemReceiveBuffer[comPort][3] << 24)
                            | (gv.aItemReceiveBuffer[comPort][4] << 16) 
                            | (gv.aItemReceiveBuffer[comPort][5] << 8)
                            | gv.aItemReceiveBuffer[comPort][6]);
                    gv.K2=((gv.aItemReceiveBuffer[comPort][7] << 24)
                            | (gv.aItemReceiveBuffer[comPort][8] << 16) 
                            | (gv.aItemReceiveBuffer[comPort][9] << 8)
                            | gv.aItemReceiveBuffer[comPort][10]);
                    gv.K3=((gv.aItemReceiveBuffer[comPort][11] << 24)
                            | (gv.aItemReceiveBuffer[comPort][12] << 16) 
                            | (gv.aItemReceiveBuffer[comPort][13] << 8)
                            | gv.aItemReceiveBuffer[comPort][14]);
                    gv.K4=((gv.aItemReceiveBuffer[comPort][15] << 24)
                            | (gv.aItemReceiveBuffer[comPort][16] << 16) 
                            | (gv.aItemReceiveBuffer[comPort][17] << 8)
                            | gv.aItemReceiveBuffer[comPort][18]);
                    gv.K5=((gv.aItemReceiveBuffer[comPort][19] << 24)
                            | (gv.aItemReceiveBuffer[comPort][20] << 16) 
                            | (gv.aItemReceiveBuffer[comPort][21] << 8)
                            | gv.aItemReceiveBuffer[comPort][22]);
                    gv.K6=((gv.aItemReceiveBuffer[comPort][23] << 24)
                            | (gv.aItemReceiveBuffer[comPort][24] << 16) 
                            | (gv.aItemReceiveBuffer[comPort][25] << 8)
                            | gv.aItemReceiveBuffer[comPort][26]);
                    gv.K7=((gv.aItemReceiveBuffer[comPort][27] << 24)
                            | (gv.aItemReceiveBuffer[comPort][28] << 16) 
                            | (gv.aItemReceiveBuffer[comPort][29] << 8)
                            | gv.aItemReceiveBuffer[comPort][30]);
                    gv.KC=((gv.aItemReceiveBuffer[comPort][31]<<8)|gv.aItemReceiveBuffer[comPort][32]);

                        gCapUpdateStep=2;
                    } else if(gCapUpdateStep==4) {
                        gCapUpdateStep=0;
                        gv.qcScoreTime=((gv.aItemReceiveBuffer[comPort][3] << 24)
                                | (gv.aItemReceiveBuffer[comPort][4] << 16) 
                                | (gv.aItemReceiveBuffer[comPort][5] << 8)
                                | gv.aItemReceiveBuffer[comPort][6]);
                        gv.qcScoreStatus=((gv.aItemReceiveBuffer[comPort][7]<<8)|gv.aItemReceiveBuffer[comPort][8]);
                        //Dprintf("gv.qcScoreTime = %d, gv.qcScoreStatus = %d\n",gv.qcScoreTime,gv.qcScoreStatus);
                    }
                }
                
            } else {
                Dprintf("[error] crc16\n");//측정겂관련명령만 수신한다.
            }
            gv.nFlag_ReceiveStart[comPort]=0;
            gv.nReceiveBufferCount[comPort]=0;
		}
		if(gv.nReceiveBufferCount[comPort] > 49)
		{
            Dprintf("[error] overflow");
            gv.nFlag_ReceiveStart[comPort]=0;
            gv.nReceiveBufferCount[comPort]=0;
            gv.nFlag_ReceiveDataProcess[comPort]=DISABLE;
		}
	}
    if(gv.nReceiveBufferCount[comPort] > 49)
    {
        Dprintf("[error] overflow");
        gv.nFlag_ReceiveStart[comPort]=0;
        gv.nReceiveBufferCount[comPort]=0;
        gv.nFlag_ReceiveDataProcess[comPort]=DISABLE;
    }
	oldAscii = ascii;
    return 0;
}
int odo_FactoryReset(int id, int addr, int len, int comPort) {//교정초기화
    unsigned char functionCode = 0x06;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hilen = (len & 0xFF00)>>8;
    unsigned char lolen = (len & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;

    crcBuf[count++]=id;//device address
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hilen;
    crcBuf[count++]=lolen;
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hilen, comPort);
    TransmitCharData(lolen, comPort);
    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    return 0;
}
int odo_RequestMeasure(int id, int addr, int len, int comPort) {//측정값요청
    unsigned char functionCode = 0x04;
    unsigned char hiAddr = (addr & 0xFF00)>>8;
    unsigned char loAddr = (addr & 0x00FF);
    unsigned char hilen = (len & 0xFF00)>>8;
    unsigned char lolen = (len & 0x00FF);

    unsigned char	crcBuf[20];
	unsigned int	count=0;
	unsigned char	buf[2];
	unsigned int 	nReturnCrc16=0;

    crcBuf[count++]=id;
    crcBuf[count++]=functionCode;
    crcBuf[count++]=hiAddr;
    crcBuf[count++]=loAddr;
    crcBuf[count++]=hilen;
    crcBuf[count++]=lolen;
    nReturnCrc16=(crc16(crcBuf,count));
	buf[0]=(nReturnCrc16>>8)&0x00FF;
	buf[1]=(nReturnCrc16&0x00FF);
    
    TransmitCharData(id, comPort); 
    TransmitCharData(functionCode, comPort);
    TransmitCharData(hiAddr, comPort);
    TransmitCharData(loAddr, comPort);
    TransmitCharData(hilen, comPort);
    TransmitCharData(lolen, comPort);
    TransmitCharData(buf[0], comPort);
    TransmitCharData(buf[1], comPort);
    
    //Dprintf("CHECK %02x %02x\n",buf[0],buf[1]);
    return 0;
}

#endif
