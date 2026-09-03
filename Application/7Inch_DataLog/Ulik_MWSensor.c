#include "TEECO_System.h"
#include "DataLog.h"

#ifdef SPECTRO_M_ENABLE
int gUlik_CleanWaitFlag=0;
int gUlik_CleanTimerPv=0;
int ULIK_MW_Request(int comPort, int id, int function, int addr1, int addr2, int reg1, int reg2) {
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    gv.nFlag_ReceiveStart[comPort]=0;
    gv.ulikFunctionCode=function;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, addr1);
    Uart_TransmitPort(comPort, addr2);
    Uart_TransmitPort(comPort, reg1);
    Uart_TransmitPort(comPort, reg2);
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;
    aCrcBuf[2]=addr1;
    aCrcBuf[3]=addr2;
    aCrcBuf[4]=reg1;
    aCrcBuf[5]=reg2;
        
    
	nReturnCrc16=(crc16(aCrcBuf,6));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    
    return 0;
}

int Ulik_MW_WindowCleaning(int comPort, int id, int function) { //세척명령
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;
    aCrcBuf[2]=0x00;
    aCrcBuf[3]=0x00;
    aCrcBuf[4]=0x00;
    aCrcBuf[5]=0x10;
    aCrcBuf[6]=0x02;
    aCrcBuf[7]=0x15;
    aCrcBuf[8]=0x01;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x10);
    
    Uart_TransmitPort(comPort, 0x02);
    Uart_TransmitPort(comPort, 0x15);
    Uart_TransmitPort(comPort, 0x01);
    
    gv.ulikFunctionCode=function;
    
	nReturnCrc16=(crc16(aCrcBuf,9));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;
}
int Ulik_MW_COD_ParaSave(int comPort, int id) {//교정값을 저장한다.
	unsigned char aCrcBuf[16];	
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=0x0F;
    aCrcBuf[2]=0x00;//address
    aCrcBuf[3]=0x00;
    aCrcBuf[4]=0x00;//register
    aCrcBuf[5]=0x10;
    aCrcBuf[6]=0x02;//길이
    aCrcBuf[7]=0x11;//설정값
    aCrcBuf[8]=0x01;
    aCrcBuf[9]=0xDE;
    aCrcBuf[10]=0x23;
    
    Uart_TransmitPort(comPort, aCrcBuf[0]); 
    Uart_TransmitPort(comPort, aCrcBuf[1]);
    Uart_TransmitPort(comPort, aCrcBuf[2]);
    Uart_TransmitPort(comPort, aCrcBuf[3]);
    Uart_TransmitPort(comPort, aCrcBuf[4]);
    Uart_TransmitPort(comPort, aCrcBuf[5]);
    Uart_TransmitPort(comPort, aCrcBuf[6]);
    Uart_TransmitPort(comPort, aCrcBuf[7]);
    Uart_TransmitPort(comPort, aCrcBuf[8]);
    Uart_TransmitPort(comPort, aCrcBuf[9]);
    Uart_TransmitPort(comPort, aCrcBuf[10]);
    
    gv.ulikFunctionCode=aCrcBuf[1];//Function
    
    return 0;    
}
int Ulik_MW_COD_CalParaClear(int comPort, int id) {//교정데이터를 전부삭제한다.
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    int function = 0x06;
    int addr1 = 0x80, addr2 = 0x0F;
    int reg1 = 0xFF, reg2 = 0xFF;
    
    
    gv.nFlag_ReceiveStart[comPort]=0;
    gv.ulikFunctionCode=function;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, addr1);
    Uart_TransmitPort(comPort, addr2);
    Uart_TransmitPort(comPort, reg1);
    Uart_TransmitPort(comPort, reg2);
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;
    aCrcBuf[2]=addr1;
    aCrcBuf[3]=addr2;
    aCrcBuf[4]=reg1;
    aCrcBuf[5]=reg2;
        
    
	nReturnCrc16=(crc16(aCrcBuf,6));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;    
}
int Ulik_MW_TOC_CalParaClear(int comPort, int id) {//교정데이터를 전부삭제한다.
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    int function = 0x06;
    int addr1 = 0x80, addr2 = 0x0F;
    int reg1 = 0xFF, reg2 = 0xFC;
    
    
    gv.nFlag_ReceiveStart[comPort]=0;
    gv.ulikFunctionCode=function;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, addr1);
    Uart_TransmitPort(comPort, addr2);
    Uart_TransmitPort(comPort, reg1);
    Uart_TransmitPort(comPort, reg2);
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;
    aCrcBuf[2]=addr1;
    aCrcBuf[3]=addr2;
    aCrcBuf[4]=reg1;
    aCrcBuf[5]=reg2;
        
    
	nReturnCrc16=(crc16(aCrcBuf,6));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;    
}
int Ulik_MW_NO3_CalParaClear(int comPort, int id) {//교정데이터를 전부삭제한다.
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    int function = 0x06;
    int addr1 = 0x80, addr2 = 0x0F;
    int reg1 = 0xFF, reg2 = 0xFD;
    
    
    gv.nFlag_ReceiveStart[comPort]=0;
    gv.ulikFunctionCode=function;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, addr1);
    Uart_TransmitPort(comPort, addr2);
    Uart_TransmitPort(comPort, reg1);
    Uart_TransmitPort(comPort, reg2);
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;
    aCrcBuf[2]=addr1;
    aCrcBuf[3]=addr2;
    aCrcBuf[4]=reg1;
    aCrcBuf[5]=reg2;
        
    
	nReturnCrc16=(crc16(aCrcBuf,6));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;    
}
int Ulik_MW_TUR_CalParaClear(int comPort, int id) {//교정데이터를 전부삭제한다.
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    int function = 0x06;
    int addr1 = 0x80, addr2 = 0x0F;
    int reg1 = 0xFF, reg2 = 0xFE;
    
    
    gv.nFlag_ReceiveStart[comPort]=0;
    gv.ulikFunctionCode=function;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, addr1);
    Uart_TransmitPort(comPort, addr2);
    Uart_TransmitPort(comPort, reg1);
    Uart_TransmitPort(comPort, reg2);
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;
    aCrcBuf[2]=addr1;
    aCrcBuf[3]=addr2;
    aCrcBuf[4]=reg1;
    aCrcBuf[5]=reg2;
        
    
	nReturnCrc16=(crc16(aCrcBuf,6));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;    
}
int Ulik_MW_COD_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr) {//COD 표준용액 교정
    float   			mV=value;
    U32     			*address;
    U8      			a, b, c, d;
    void    			*addr1; 
    addr1 = &mV;
    address = addr1;
    a= (*address>>24)&0xFF;     //MSB
    b= (*address>>16)&0xFF; 
    c= (*address>>8)&0xFF;  
    d= *address & 0xFF;         //LSB
    
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=0x10;//Function. multiple write
    aCrcBuf[2]=hiAddr;//address
    aCrcBuf[3]=loAddr;
    aCrcBuf[4]=0x00;//register
    aCrcBuf[5]=0x02;
    aCrcBuf[6]=0x04;//길이
    aCrcBuf[7]=c;//설정값
    aCrcBuf[8]=d;
    aCrcBuf[9]=a;
    aCrcBuf[10]=b;
    
    Uart_TransmitPort(comPort, aCrcBuf[0]); 
    Uart_TransmitPort(comPort, aCrcBuf[1]);
    Uart_TransmitPort(comPort, aCrcBuf[2]);
    Uart_TransmitPort(comPort, aCrcBuf[3]);
    Uart_TransmitPort(comPort, aCrcBuf[4]);
    Uart_TransmitPort(comPort, aCrcBuf[5]);
    Uart_TransmitPort(comPort, aCrcBuf[6]);
    Uart_TransmitPort(comPort, aCrcBuf[7]);
    Uart_TransmitPort(comPort, aCrcBuf[8]);
    Uart_TransmitPort(comPort, aCrcBuf[9]);
    Uart_TransmitPort(comPort, aCrcBuf[10]);
    
    gv.ulikFunctionCode=aCrcBuf[1];//Function

	nReturnCrc16=(crc16(aCrcBuf,11));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    
    return 0;    
}
int Ulik_MW_TOC_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr) {//NO3 표준용액 교정
    float   			mV=value;
    U32     			*address;
    U8      			a, b, c, d;
    void    			*addr1; 
    addr1 = &mV;
    address = addr1;
    a= (*address>>24)&0xFF;     //MSB
    b= (*address>>16)&0xFF; 
    c= (*address>>8)&0xFF;  
    d= *address & 0xFF;         //LSB
    
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=0x10;//Function. multiple write
    aCrcBuf[2]=hiAddr;//address
    aCrcBuf[3]=loAddr;
    aCrcBuf[4]=0x00;//register
    aCrcBuf[5]=0x02;
    aCrcBuf[6]=0x04;//길이
    aCrcBuf[7]=c;//설정값
    aCrcBuf[8]=d;
    aCrcBuf[9]=a;
    aCrcBuf[10]=b;
    
    Uart_TransmitPort(comPort, aCrcBuf[0]); 
    Uart_TransmitPort(comPort, aCrcBuf[1]);
    Uart_TransmitPort(comPort, aCrcBuf[2]);
    Uart_TransmitPort(comPort, aCrcBuf[3]);
    Uart_TransmitPort(comPort, aCrcBuf[4]);
    Uart_TransmitPort(comPort, aCrcBuf[5]);
    Uart_TransmitPort(comPort, aCrcBuf[6]);
    Uart_TransmitPort(comPort, aCrcBuf[7]);
    Uart_TransmitPort(comPort, aCrcBuf[8]);
    Uart_TransmitPort(comPort, aCrcBuf[9]);
    Uart_TransmitPort(comPort, aCrcBuf[10]);
    
    gv.ulikFunctionCode=aCrcBuf[1];//Function

	nReturnCrc16=(crc16(aCrcBuf,11));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    
    return 0;    
}
int Ulik_MW_NO3_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr) {//NO3 표준용액 교정
    float   			mV=value;
    U32     			*address;
    U8      			a, b, c, d;
    void    			*addr1; 
    addr1 = &mV;
    address = addr1;
    a= (*address>>24)&0xFF;     //MSB
    b= (*address>>16)&0xFF; 
    c= (*address>>8)&0xFF;  
    d= *address & 0xFF;         //LSB
    
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=0x10;//Function. multiple write
    aCrcBuf[2]=hiAddr;//address
    aCrcBuf[3]=loAddr;
    aCrcBuf[4]=0x00;//register
    aCrcBuf[5]=0x02;
    aCrcBuf[6]=0x04;//길이
    aCrcBuf[7]=c;//설정값
    aCrcBuf[8]=d;
    aCrcBuf[9]=a;
    aCrcBuf[10]=b;
    
    Uart_TransmitPort(comPort, aCrcBuf[0]); 
    Uart_TransmitPort(comPort, aCrcBuf[1]);
    Uart_TransmitPort(comPort, aCrcBuf[2]);
    Uart_TransmitPort(comPort, aCrcBuf[3]);
    Uart_TransmitPort(comPort, aCrcBuf[4]);
    Uart_TransmitPort(comPort, aCrcBuf[5]);
    Uart_TransmitPort(comPort, aCrcBuf[6]);
    Uart_TransmitPort(comPort, aCrcBuf[7]);
    Uart_TransmitPort(comPort, aCrcBuf[8]);
    Uart_TransmitPort(comPort, aCrcBuf[9]);
    Uart_TransmitPort(comPort, aCrcBuf[10]);
    
    gv.ulikFunctionCode=aCrcBuf[1];//Function

	nReturnCrc16=(crc16(aCrcBuf,11));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    
    return 0;    
}
int Ulik_MW_TUR_CalParaSet(int comPort, int id, float value, unsigned char hiAddr, unsigned char loAddr) {//NO3 표준용액 교정
    float   			mV=value;
    U32     			*address;
    U8      			a, b, c, d;
    void    			*addr1; 
    addr1 = &mV;
    address = addr1;
    a= (*address>>24)&0xFF;     //MSB
    b= (*address>>16)&0xFF; 
    c= (*address>>8)&0xFF;  
    d= *address & 0xFF;         //LSB
    
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=0x10;//Function. multiple write
    aCrcBuf[2]=hiAddr;//address
    aCrcBuf[3]=loAddr;
    aCrcBuf[4]=0x00;//register
    aCrcBuf[5]=0x02;
    aCrcBuf[6]=0x04;//길이
    aCrcBuf[7]=c;//설정값
    aCrcBuf[8]=d;
    aCrcBuf[9]=a;
    aCrcBuf[10]=b;
    
    Uart_TransmitPort(comPort, aCrcBuf[0]); 
    Uart_TransmitPort(comPort, aCrcBuf[1]);
    Uart_TransmitPort(comPort, aCrcBuf[2]);
    Uart_TransmitPort(comPort, aCrcBuf[3]);
    Uart_TransmitPort(comPort, aCrcBuf[4]);
    Uart_TransmitPort(comPort, aCrcBuf[5]);
    Uart_TransmitPort(comPort, aCrcBuf[6]);
    Uart_TransmitPort(comPort, aCrcBuf[7]);
    Uart_TransmitPort(comPort, aCrcBuf[8]);
    Uart_TransmitPort(comPort, aCrcBuf[9]);
    Uart_TransmitPort(comPort, aCrcBuf[10]);
    
    gv.ulikFunctionCode=aCrcBuf[1];//Function

	nReturnCrc16=(crc16(aCrcBuf,11));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    
    return 0;    
}
int Ulik_MW_CODCalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_MW_Request(comPort, id, function, 0x01, 0x04, 0x00, 0x28);
    return 0;//WQP0.COD
}
int Ulik_MW_TOCCalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_MW_Request(comPort, id, function, 0x01, 0x40, 0x00, 0x28);
    return 0;//WQP3.TOC
}
int Ulik_MW_NO3CalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_MW_Request(comPort, id, function, 0x01, 0x2C, 0x00, 0x28);
    return 0;//WQP2.NOR
}
int Ulik_MW_TURCalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_MW_Request(comPort, id, function, 0x01, 0x18, 0x00, 0x28);
    return 0;//WQP1.TUR
}

int Ulik_MW_ReqMeas(int comPort, int id, int function) {//측정값 요청    
    ULIK_MW_Request(comPort, id, function, 0x90, 0x06, 0x00, 0x2C);
    return 0;
}
int Ulik_MW_ParameterParsing(int comPort) {	//교정값 수신 분석
    unsigned char buf[10];
    float   mV=0.0F;
    U32     *address;
    void    *addr1=0; 
	addr1 = &mV;  
    address = addr1;
    int cnt=3;
    int ulikCalStep=0;
    
    
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    gv.paraValue1[0]=mV;
    //Dprintf("[1.PARAMETER ORG] = %.4f\n",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);    
    gv.paraValue2[0]=mV;
    
    //Dprintf("[1.PARAMETER    ] = %.4f\n",mV);
    if(mV > 0) {
        ulikCalStep++;
    }
    
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    //Dprintf("[2.PARAMETER ORG] = %.2f\n",mV);
    gv.paraValue1[1]=mV;
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    //Dprintf("[2.PARAMETER    ] = %.2f\n",mV);
    gv.paraValue2[1]=mV;
    if(mV > 0) {
        ulikCalStep++;
    }

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    //Dprintf("[3.PARAMETER ORG] = %.2f\n",mV);
    gv.paraValue1[2]=mV;
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    //Dprintf("[3.PARAMETER    ] = %.2f\n",mV);
    gv.paraValue2[2]=mV;
    if(mV > 0) {
        ulikCalStep++;
    }

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    //Dprintf("[4.PARAMETER ORG] = %.2f\n",mV);
    gv.paraValue1[3]=mV;
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    //Dprintf("[4.PARAMETER    ] = %.2f\n",mV);
    gv.paraValue2[3]=mV;
    if(mV > 0) {
        ulikCalStep++;
    }

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    //Dprintf("[5.PARAMETER ORG] = %.2f\n",mV);
    gv.paraValue1[4]=mV;
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);
    //Dprintf("[5.PARAMETER    ] = %.2f\n",mV);
    gv.paraValue2[4]=mV;
    if(mV > 0) {
        ulikCalStep++;
    }
    gv.ulikCalStep=ulikCalStep;
    return 0;
}
int Ulik_MW_Parsing(int comPort) {			//데이터 분석
    unsigned char buf[10];
    float   mV=0.0F;
    U32     *address;
    void    *addr1=0; 
	addr1 = &mV;  
    address = addr1;
    int cnt=3;
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TEMP
    flash.Uart[comPort]->SensorPV[0] = mV;    
//Dprintf("WaterT = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//COD
    flash.Uart[comPort]->SensorPV[1] = mV;
    gv.COD_Cr=mV;
//Dprintf("[WQP0]COD = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TUR 
    flash.Uart[comPort]->SensorPV[2] = mV;
    gv.TUR=mV;
//Dprintf("[WQP1]TUR = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Nitrate
    flash.Uart[comPort]->SensorPV[3] = mV;
    gv.NO3=mV;
//Dprintf("[WQP2]Nitrate = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TOC
    flash.Uart[comPort]->SensorPV[4] = mV;
    gv.TOCstd=mV;
//Dprintf("[WQP3]TOC = %.2f  \n",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Default
//Dprintf("[WQP4]Default = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Default
//Dprintf("[WQP5]Default = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Default
//Dprintf("[WQP6]Default = %.2f  ",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Default
//Dprintf("[WQP7]Default = %.2f  \n",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//COD_org
//Dprintf("[WQP0_org]COD = %.2f  ",mV);
    gv.COD_KHP=mV;
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TUR
    gv.TURorg = mV;
//Dprintf("[WQP1_org]TUR = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//NITRATE
    gv.NO3org=mV;
//Dprintf("[WQP2_org]NITRATE = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TOC
    gv.TOCreal=mV;
//Dprintf("[WQP3_org]TOC = %.2f  \n",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Default
//Dprintf("[WQP4_org]Default = %.2f  ",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Default
//Dprintf("[WQP5_org]Default = %.2f  ",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Default
//Dprintf("[WQP6_org]Default = %.2f  ",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Default
//("[WQP7_org]Default = %.2f \n",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//SAC1 흡광도계수
//Dprintf("[SAC1]흡광도계수 = %.2f  ",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//SAC2 흡광도계수
//Dprintf("[SAC2]흡광도계수 = %.2f  ",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//SAC3 흡광도계수
//Dprintf("[SAC3]흡광도계수 = %.2f  ",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//SAC4 흡광도계수
//Dprintf("[SAC4]흡광도계수 = %.2f  ",mV);

    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//Temp
//Dprintf("[Temp_org]Temp = %.2f\n",mV);
return 0;
}
int ULIK_MW_IOProcess(int comPort) {        
	if(gv.tmsStatusCode[USART_1]!=3) {//교정중에는 자동 세정을 진행하지 않는다. 타이머도 증가하지 않는다.
		if(flash.ulik.cleanTimeSv > 0) {//0이면 세척을 하지 않는다.
			if(flash.ulik.cleanTimeSv <= gv.ulikCleanTimerCount) {
				gv.ulikCleanTimerCount=0;
				Ulik_MW_WindowCleaning(USART_1,flash.ulik.id,0x0F);
				//Dprintf("ULIK 세척명령 실행\n");
				gUlik_CleanWaitFlag=1;
				gUlik_CleanTimerPv=0;
				return 0;
			}
			else {
				gv.ulikCleanTimerCount++;//세적시간증가
				//Dprintf("세척시간 (%04d) %04d\n",flash.ulik.cleanTimeSv,gv.ulikCleanTimerCount);
			}
		}
		if(gUlik_CleanWaitFlag==1) {
			gUlik_CleanTimerPv++;
			if(gUlik_CleanTimerPv>=20) {//20초세척
				gUlik_CleanWaitFlag=0;
				gUlik_CleanTimerPv=0;
			} 
			else {
				return 0; //세척중에는 명령을 송신하지 않는다.
			}
		}
	}
	else {
		gUlik_CleanWaitFlag=0;
		gUlik_CleanTimerPv=0;
		gv.ulikCleanTimerCount=0;
	}
    
	if(gv.nFlag_ReceiveDataProcess[comPort]==1) {//데이터 수신시
        gv.nFlag_ReceiveDataProcess[comPort]=0;
        //Dprintf("데이터 정상수신\n");
        if(gv.ulikReceivedItemNo==0) {		//ULIK 측정값 분석
            Ulik_MW_Parsing(comPort);
            //Dprintf("ULIK 측정값 분석\n");
        }
        else if(gv.ulikReceivedItemNo==1) {	//ULIK 교정값 분석
            Ulik_MW_ParameterParsing(comPort);
            //Dprintf("ULIK 파라메터 분석\n");
        }
        else if(gv.ulikReceivedItemNo==3) {	//CTD 측정값 분석
			#if defined (CTD_ENABLE)
            CTD_Parsing(comPort);
			#endif			
            //Dprintf("CTD 파라메터 분석\n");
        }
    }
    else if(gv.nFlag_ReceiveDataProcess[comPort]==3) {
        gv.nFlag_ReceiveDataProcess[comPort]=0;
        Dprintf("ULICK CRC ERROR\n");
    }
    else if(gv.nFlag_ReceiveDataProcess[comPort]==4) {
        gv.nFlag_ReceiveDataProcess[comPort]=0;
        Dprintf("ULICK OVERFLOW ERROR\n");
    }
    else {            
        if(gv.ulikCalCmdSendNo==0) {//데이터 송신시
            if(gv.ulikToggleParaMeasRequestFlag==0) {
                if(gv.tmsStatusCode[USART_1]==3) {//교정모드에서만 파라메터 데이터를 요청한다.
                    gv.ulikToggleParaMeasRequestFlag=1;
                    gv.ulikReceivedItemNo=0;
                }
                else {
                    gv.ulikReceivedItemNo=0;
                }
                //CTD 와 ULIK 센서 데이터를 번갈아가며 요청한다.
                if(gv.ctd_ulik_toggle==0) {
                    #if defined(CTD_ENABLE) 			//CTD를 사용하는경우
//                    if(gv.tmsStatusCode[USART_1]!=3) {	//ULIK 교정중에는 CTD 값을 요청하지 않는다.
                        gv.ctd_ulik_toggle=1;	
//                    } 
                    #endif
//                    if(gv.ulikCalItem!=5) {
                        Ulik_MW_ReqMeas(comPort,flash.ulik.id, 0x03);//측정값요청 ULIK
//                        Dprintf("ULIK 데이터 요청 코드 = %d gv.ctd_ulik_toggle = %d\n",gv.ulikReceivedItemNo,gv.ctd_ulik_toggle);
//                    }
                } 
                else if(gv.ctd_ulik_toggle==1) { 		//ULIK 데이터를 수신하면 1이된다.
                    #if defined(CTD_ENABLE) 			//CTD를 사용하는경우
                    gv.ctd_ulik_toggle=0;
                    gv.ulikReceivedItemNo=3;
                    
					if(gv.ulikCalCmdSendNo==0) {		//교정명령 송신시에는 요청하지 않는다.
                        CTD_Read(flash.ctd.id, comPort);
                        //Dprintf("CTD 데이터 요청 코드 = %d gv.ctd_ulik_toggle = %d\n",gv.ulikReceivedItemNo,gv.ctd_ulik_toggle);
                    }
                    #endif
                }
            }
            else { //교정값요청
                gv.ulikToggleParaMeasRequestFlag=0;
                gv.ulikReceivedItemNo=1;
                if(gv.ulikCalItem==0) {					
                    Ulik_MW_CODCalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    //Dprintf("ULIK 파라메터 요청 코드 = %d gv.ctd_ulik_toggle = %d\n",gv.ulikReceivedItemNo,gv.ctd_ulik_toggle);
                }
                else if(gv.ulikCalItem==1){
                    Ulik_MW_TOCCalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    //Dprintf("ULIK 파라메터 요청 코드 = %d\n",gv.ulikReceivedItemNo);
                }
                else if(gv.ulikCalItem==2) {
                    Ulik_MW_NO3CalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    //Dprintf("ULIK 파라메터 요청 코드 = %d\n",gv.ulikReceivedItemNo);
                }
                else if(gv.ulikCalItem==3) {
                    Ulik_MW_TURCalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    //Dprintf("ULIK 파라메터 요청 코드 = %d\n",gv.ulikReceivedItemNo);
                }
                else if(gv.ulikCalItem==5) { //전도도 측정값 요청
                    #if defined(CTD_ENABLE) //CTD를 사용하는경우
                    gv.ulikReceivedItemNo=3;
                    CTD_Read(0x01, comPort);
                    //Dprintf("CTD 교정 중 데이터 요청 코드 = %d\n",gv.ulikReceivedItemNo, gv.ctd_ulik_toggle);
                    #endif
                }
            }
        }
        else {
            //Dprintf("교정 명령 송신 중에는 다른 명령이 송신되지 않는다.\n");
        }
    }    
    return 0;
}
int Ulik_MW_Receive(int ascii, int ch ) { 	//데이터 수신
    unsigned char aCrcBuf[2], aCcData[2];
    unsigned int		nReturnCrc16=0;
    int id=0, functionCode=0;    
    #if defined(CTD_ENABLE)
    if(gv.ctd_ulik_toggle==1 || gv.ctd_ulik_toggle==3) {//ULIK
        id=flash.ulik.id;
        functionCode=gv.ulikFunctionCode;
        
    } 
    else { //CTD
        id=flash.ctd.id;
        functionCode=0x03;
    }
    #else 
        id=flash.ulik.id;
        functionCode=gv.ulikFunctionCode;
    #endif
    
	if(gv.ulikOldAscii==id && functionCode == ascii && gv.nFlag_ReceiveStart[ch] == 0)//데이터 수신 시작 
	{
		gv.nFlag_ReceiveStart[ch]=1;//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;//데이터 버퍼 주소 초기화
        gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++]=id;
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++]=ascii;
        gv.nFlag_ReceiveDataProcess[ch]=0;
        gv.ulikLength[ch]=0;        
	}
	else if(gv.nFlag_ReceiveStart[ch]==1)
	{
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++]=ascii;//데이터 수집
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]]=NULL;	//문자열의 끝에는  NULL 을 꼭 넣어 준다.
        
		if(gv.nReceiveBufferCount[ch]==3) { //데이터 길이
            gv.ulikLength[ch] = gv.aItemReceiveBuffer[ch][2];
            gv.ulikLength[ch] += 5;//id,function.length crc code            
		}			
        if( gv.nReceiveBufferCount[ch] >= (USART_BUFFER_MAXSIZE-10) ) {//저장영역이 오바하면 변수들을 초기화 
            gv.ulikLength[ch]=0;
			gv.nReceiveBufferCount[ch]=0;
			gv.nFlag_ReceiveDataProcess[ch]=4;//overFlow
		}
		else if(gv.ulikLength[ch] == gv.nReceiveBufferCount[ch]) {//데이터 수신완료 
			aCcData[0]=gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]-2];
			aCcData[1]=gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]-1];

			nReturnCrc16=(crc16(gv.aItemReceiveBuffer[ch],(gv.ulikLength[ch]-2)));
			aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
			aCrcBuf[1]=(nReturnCrc16&0x00FF);
                        
			if(aCrcBuf[0]==aCcData[0] && aCrcBuf[1]==aCcData[1]){
				gv.nFlag_ReceiveDataProcess[ch]=1;//데이터 정상수신 데이터 처리한다.
                if(gv.tmsStatusCode[ch]==6) {
                    gv.tmsStatusCode[ch]=0;
                }
			}
			else {
                if(gv.ctd_ulik_toggle==1) {
                    Dprintf("[CRC ERR ULIK] %02x %02x == %02x %02x\n",aCcData[0],aCcData[1],aCrcBuf[0],aCrcBuf[1]);
                }
                else {
                    Dprintf("[CRC ERR CTD] %02x %02x == %02x %02x\n",aCcData[0],aCcData[1],aCrcBuf[0],aCrcBuf[1]);
                }
                for( int i=0; i<gv.nReceiveBufferCount[ch]; i++) {
                    //Dprintf("%02x ",gv.aItemReceiveBuffer[ch][i]);
                }
				gv.nFlag_ReceiveDataProcess[ch]=3;//crc error
                gv.nReceiveBufferCount[ch]=0;
			}
			//gv.nFlag_ReceiveStart[ch]=OFF;		
		}
	}
	if(gv.nReceiveBufferCount[ch] >= (USART_BUFFER_MAXSIZE-10))
	{
		//gv.nFlag_ReceiveStart[ch]=OFF;
		gv.nReceiveBufferCount[ch]=0;
		gv.nFlag_ReceiveDataProcess[ch]=5;//overFlow	
	}
	gv.ulikOldAscii = ascii;
    return 0;
}
#endif
