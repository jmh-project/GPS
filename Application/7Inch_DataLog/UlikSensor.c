#include "TEECO_System.h"
#include "DataLog.h"

#ifdef SPECTRO_D_ENABLE
int ULIK_Request(int comPort, int id, int function, int addr1, int addr2, int reg1, int reg2) {
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    gv.nFlag_ReceiveStart[comPort]=0;
    
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
    
    gv.ulikFunctionCode=function;
    
	nReturnCrc16=(crc16(aCrcBuf,6));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    //Dprintf("flash.ulik.id = %02x CRC16 = %02x %02x\n",flash.ulik.id,aCrcBuf[0], aCrcBuf[1]);
    return 0;
}
int Ulik_HoldingRegisterTable_Reading(int comPort, int id) { //센서정보를 요청한다.
    ULIK_Request(comPort, id, 0x03, 0x80, 0x00, 0x00, 0x22);
    return 0;
}

int Ulik_WindowCleaning(int comPort, int id, int function) { //세척명령
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
int Ulik_WindowAutoCleaningDisable(int comPort, int id, int function) {
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
    aCrcBuf[8]=0x03;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x10);
    
    Uart_TransmitPort(comPort, 0x02);
    Uart_TransmitPort(comPort, 0x15);
    Uart_TransmitPort(comPort, 0x03);
    
    gv.ulikFunctionCode=function;
    
	nReturnCrc16=(crc16(aCrcBuf,9));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;
}
int Ulik_Restart(int comPort, int id, int function) { //Restart command
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;
    aCrcBuf[2]=0x00;
    aCrcBuf[3]=0x00;
    aCrcBuf[4]=0x00;
    aCrcBuf[5]=0x10;
    aCrcBuf[6]=0x02;
    aCrcBuf[7]=0x10;
    aCrcBuf[8]=0x01;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x10);
    
    Uart_TransmitPort(comPort, 0x02);
    Uart_TransmitPort(comPort, 0x10);
    Uart_TransmitPort(comPort, 0x01);
    
    gv.ulikFunctionCode=function;
    
	nReturnCrc16=(crc16(aCrcBuf,9));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;
}
int Ulik_SaveF(int comPort, int id, int function) { //Save parameter command
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;
    aCrcBuf[2]=0x00;
    aCrcBuf[3]=0x00;
    aCrcBuf[4]=0x00;
    aCrcBuf[5]=0x10;
    aCrcBuf[6]=0x02;
    aCrcBuf[7]=0x11;
    aCrcBuf[8]=0x01;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x10);
    
    Uart_TransmitPort(comPort, 0x02);
    Uart_TransmitPort(comPort, 0x11);
    Uart_TransmitPort(comPort, 0x01);
    
    gv.ulikFunctionCode=function;
    
	nReturnCrc16=(crc16(aCrcBuf,9));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;
}
int Ulik_ZeroCalibration(int comPort, int id, int function) { //Zero Calibration command
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=function;//0x0F
    aCrcBuf[2]=0x00;
    aCrcBuf[3]=0x00;
    aCrcBuf[4]=0x00;
    aCrcBuf[5]=0x10;
    aCrcBuf[6]=0x02;
    aCrcBuf[7]=0x14;
    aCrcBuf[8]=0x01;
    
    Uart_TransmitPort(comPort, id); 
    Uart_TransmitPort(comPort, function);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x00);
    Uart_TransmitPort(comPort, 0x10);
    
    Uart_TransmitPort(comPort, 0x02);
    Uart_TransmitPort(comPort, 0x14);
    Uart_TransmitPort(comPort, 0x01);
    
    gv.ulikFunctionCode=function;
    
	nReturnCrc16=(crc16(aCrcBuf,9));
	aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
	aCrcBuf[1]=(nReturnCrc16&0x00FF);

	Uart_TransmitPort(comPort, aCrcBuf[0]);
	Uart_TransmitPort(comPort, aCrcBuf[1]);
    return 0;
}
int Ulik_ReqMeas(int comPort, int id, int function) {//측정값 요청    
    ULIK_Request(comPort, id, function, 0x80, 0x10, 0x00, 0x0C);
    return 0;
}
/**********************************************************************************************
    COD Water sample 교정명령
***********************************************************************************************/
int Ulik_CODCalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_Request(comPort, id, function, 0x01, 0x86, 0x00, 0x4E);
    return 0;
}
int Ulik_CODcalibCtrl(int comPort, int id, int function, int menu) {//교정적용 및 취소
    switch( menu )//function 0x06
    {
        case 0: ULIK_Request(comPort, id, function, 0x90, 0x16, 0x00, 0x01); break;//Comfirm Calib    :: 교정적용
        case 1: ULIK_Request(comPort, id, function, 0x90, 0x16, 0xFF, 0xFF); break;//Clear last Calib :: 마지막 교정 초기화
        case 2: ULIK_Request(comPort, id, function, 0x90, 0x16, 0xFF, 0x9C); break;//Clear All Calib  :: 모든 교정 초기화
    }    
    return 0;
}
int Ulik_CODReal(int comPort, int id, float value) { // CODReal 설정값을 레지스터에 쓰기
    //60 10 90 10 00 02 04 00 00 41 C8 95 9E 
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x10;
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
int Ulik_COD_KHPsensor(int comPort, int id, float value) { // COD_KHPsensor 설정값을 레지스터에 쓰기
    //60 10 90 0A 00 02 04 00 00 41 C8 14 ED
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x0A;
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
int Ulik_TURsensor(int comPort, int id, float value) { // TURsensor 설정값을 레지스터에 쓰기
    //60 10 90 0C 00 02 04 00 00 41 C8 94 C7 
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x0C;
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
/**********************************************************************************************
    TOC Water sample 교정명령
***********************************************************************************************/
int Ulik_TOCCalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_Request(comPort, id, function, 0x03, 0x86, 0x00, 0x4E);
    return 0;
}
int Ulik_TOCcalibCtrl(int comPort, int id, int function, int menu) {//교정적용 및 취소
    switch( menu )//function 0x06
    {
        case 0: ULIK_Request(comPort, id, function, 0x90, 0x17, 0x00, 0x01); break;//Comfirm Calib    :: 교정적용
        case 1: ULIK_Request(comPort, id, function, 0x90, 0x17, 0xFF, 0xFF); break;//Clear last Calib :: 마지막 교정 초기화
        case 2: ULIK_Request(comPort, id, function, 0x90, 0x17, 0xFF, 0x9C); break;//Clear All Calib  :: 모든 교정 초기화
    }
    
    return 0;
}
int Ulik_TOCReal(int comPort, int id, float value) { // TOCReal 설정값을 레지스터에 쓰기
    //60 10 90 10 00 02 04 00 00 41 C8 95 9E 
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x12;
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
int Ulik_TOCstd_sensor(int comPort, int id, float value) { // TOCstd_sensor 설정값을 레지스터에 쓰기
    //60 10 90 0A 00 02 04 00 00 41 C8 14 ED
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x0E;
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

/**********************************************************************************************
    STANDARD CALIBRATION COD TOC TUR TEMP
***********************************************************************************************/
int Ulik_COD_StdCalibration(int comPort, int id, float value) {//COD 표준용액 교정
    //60 10 90 00 00 02 04 00 00 41 C8 94 92  //25.0 set command
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x00;
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
int Ulik_COD_KPHCalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_Request(comPort, id, function, 0x01, 0x80, 0x00, 0x04);
    return 0;
}
int Ulik_COD_KPHCalibrationClear(int comPort, int id) {//COD 교정값 초기화
    //60 10 90 00 00 02 04 00 00 BF 80 D4 C4 //
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x00;
    aCrcBuf[4]=0x00;//register
    aCrcBuf[5]=0x02;
    aCrcBuf[6]=0x04;//길이
    aCrcBuf[7]=0x00;//설정값
    aCrcBuf[8]=0x00;
    aCrcBuf[9]=0xBF;
    aCrcBuf[10]=0x80;
    
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

int Ulik_TOC_StdCalibration(int comPort, int id, float value) {//TOC 표준용액 교정
    //60 10 90 04 00 02 04 00 00 41 C8 95 61  //25.0 set command
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x04;
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
int Ulik_TOC_StdCalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_Request(comPort, id, function, 0x03, 0x80, 0x00, 0x04);
    return 0;
}

int Ulik_TOC_StdCalibrationClear(int comPort, int id) {//COD 교정값 초기화
    //60 10 90 00 00 02 04 00 00 BF 80 D4 C4 //
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x04;
    aCrcBuf[4]=0x00;//register
    aCrcBuf[5]=0x02;
    aCrcBuf[6]=0x04;//길이
    aCrcBuf[7]=0x00;//설정값
    aCrcBuf[8]=0x00;
    aCrcBuf[9]=0xBF;
    aCrcBuf[10]=0x80;
    
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

int Ulik_TEMP_StdCalibration(int comPort, int id, float value) {//TEMP 표준용액 교정
    //60 10 90 06 00 02 04 00 00 41 C8 14 B8  //25.0 set command
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x06;
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


int Ulik_TUR_StdCalibration(int comPort, int id, float value) {//탁도 표준용액 교정
    //60 10 90 02 00 02 04 00 00 41 C8 15 4B //25.0 set command
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
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x02;
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
int Ulik_TUR_CalibrationClear(int comPort, int id) {//탁도 교정값 초기화
    //60 10 90 02 00 02 04 00 00 BF 80 55 1D //
	unsigned char aCrcBuf[16];	
	unsigned int nReturnCrc16;
    
    aCrcBuf[0]=id;
    aCrcBuf[1]=0x10;//Function
    aCrcBuf[2]=0x90;//address
    aCrcBuf[3]=0x02;
    aCrcBuf[4]=0x00;//register
    aCrcBuf[5]=0x02;
    aCrcBuf[6]=0x04;//길이
    aCrcBuf[7]=0x00;//설정값
    aCrcBuf[8]=0x00;
    aCrcBuf[9]=0xBF;
    aCrcBuf[10]=0x80;
    
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
int Ulik_TURCalDataStorageRegisterTable(int comPort, int id, int function) {//교정값이 저장된 레지스터 데이터를 요청한다.
    ULIK_Request(comPort, id, function, 0x02, 0x80, 0x00, 0x04);
    return 0;
}

//Holding register table
/*
60 : ID
03 : Function Code 
44 : Length
01 25 : ProVerSN
00 11 : DevSN 
01 B3 : SoftwareSN
00 60 : ModBusID 
25 80 : BaudRate 
00 00 0D 2A : SysOnTs 
00 00 : Year 
07 E7 : Month
00 06 : Date
00 14 : Hour 
00 0E : Min
00 0D : Sec
00 2D 63 68 : PowVolt
41 C9 80 00 : WaterT
80 00 41 D1 : COD_KHP
86 E7 41 8E : TUR
D2 65 41 DD : TOCstd 
0B 0A 40 E4 : COD_Cr 
48 28 41 4E : TOCreal 
B3 27 : RunFlagL 
41 B1 : RunFlagH
60 00 : BrushPos
00 00 : BrushPos1 
07 D9 : BrushPos2 
08 00 02 EE  : M_PeriodTs
00 01 : CWD_Times
63 BE : CRC CODE
*/
int Ulik_Parsing(int comPort) {//측정값 분석
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
    gv.WaterT=mV;
    flash.Uart[comPort]->SensorPV[3] = mV;
Dprintf("WaterT = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//COD_KHP
    gv.COD_KHP=mV;
Dprintf("COD_KHP = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TUR 
    gv.TUR = mV;
    flash.Uart[comPort]->SensorPV[1] = mV;
Dprintf("TUR = %.2f  ",mV);
    flash.Uart[comPort]->SensorPV[1]=mV;
    flash.Uart[comPort]->SensorPV[2]=mV;
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TOCstd
    gv.TOCstd=mV;
Dprintf("TOCstd = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//COD_Cr
    gv.COD_Cr=mV;
    flash.Uart[comPort]->SensorPV[0] = mV;
Dprintf("COD_Cr = %.2f  ",mV);
    buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
    buf[4]=0;
    *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TOCreal
    gv.TOCreal=mV;
    flash.Uart[comPort]->SensorPV[2] = mV;
Dprintf("TOCreal = %.2f\n",mV);
    return 0;
}

int Ulik_ParameterParsing(int comPort) {//Parameter parsing
    unsigned char buf[10];
    float   mV=0.0F;
    U32     *address;
    void    *addr1=0; 
	addr1 = &mV;  
    address = addr1;
    int cnt=3;
    //최대 교정범위 5
    int ulikCalStep=0;
    for(int i=0; i<6; i++) { 
        buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[4]=0;
        *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//COD_KHPsensor
        gv.paraValue1[i]=mV;
//Dprintf("[%02d]PARA = %010.3f",i,mV);
        buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[4]=0;
        *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TURsensor
        gv.paraValue2[i]=mV;
//Dprintf("[%02d]PARA = %010.3f",i,mV);
        buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[4]=0;
        *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//CODreal 
        gv.paraValue3[i]=mV;
        if(gv.ulikCalItem==0) { //CODreal 0보다크면 교정이 진행 된 것으로 본다.
            if(mV > 0) {
                ulikCalStep++;
            }
        }
        else if(gv.ulikCalItem==1) {
            if(mV > 0) {
                ulikCalStep++;
            }
        }
//Dprintf("[%d]PARA = %010.3f\n",i,mV);
        i++;

        buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[4]=0;
        *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//COD_KHPsensor
        gv.paraValue1[i]=mV;
//Dprintf("[%02d]PARA = %010.3f",i,mV);
        buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[4]=0;
        *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//TURsensor
        gv.paraValue2[i]=mV;
//Dprintf("[%02d]PARA = %010.3f",i,mV);
        buf[0]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[1]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[2]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[3]=gv.aItemReceiveBuffer[comPort][cnt++];
        buf[4]=0;
        *address = ((buf[2] << 24) | (buf[3] << 16) | (buf[0] << 8) | buf[1]);//CODreal 
        gv.paraValue3[i]=mV;
        if(gv.ulikCalItem==0) { //CODreal 0보다크면 교정이 진행 된 것으로 본다.
            if(mV > 0) {
                ulikCalStep++;
            }
        }
        else if(gv.ulikCalItem==1) {
            if(mV > 0) {
                ulikCalStep++;
            }
        }
//Dprintf("[%d]PARA2 = %010.3f\n",i,mV);
    }
//Dprintf("\n");
    gv.ulikCalStep=ulikCalStep;        
    return 0;
}
int Ulik_Receive( int ascii, int ch ) {
    unsigned char aCrcBuf[2], aCcData[2];
    unsigned int		nReturnCrc16=0;
    
    //Dputc(ascii);    

    if(gv.ulikOldAscii==flash.ulik.id && gv.ulikFunctionCode == ascii && gv.nFlag_ReceiveStart[ch] == 0)//데이터 수신 시작 
	{
		gv.nFlag_ReceiveStart[ch]=1;//데이터 수신대기 
		gv.nReceiveBufferCount[ch]=0;//데이터 버퍼 주소 초기화
        gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++]=flash.ulik.id;
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
int gUlik_CleanWaitFlag=0;
int gUlik_CleanTimerPv=0;
int ULIK_IOProcess(int comPort) {
    if(flash.ulik.cleanTimeSv > 0) {
        if(flash.ulik.cleanTimeSv <= gv.ulikCleanTimerCount) {
            gv.ulikCleanTimerCount=0;
            Ulik_WindowCleaning(USART_1,flash.ulik.id,0x0F);
            gUlik_CleanWaitFlag=1;
            gUlik_CleanTimerPv=0;
            return 0;
        }
        else {
            gv.ulikCleanTimerCount++;//세적시간증가
        }
    }
    if(gUlik_CleanWaitFlag==1) {
        gUlik_CleanTimerPv++;
        if(gUlik_CleanTimerPv>=10) {
            gUlik_CleanWaitFlag=0;
            gUlik_CleanTimerPv=0;
        } 
        else {
            return 0;
        }
    }
    
    
    if(gv.nFlag_ReceiveDataProcess[comPort]==1) {
        gv.nFlag_ReceiveDataProcess[comPort]=0;
        
        if(gv.ulikToggleParaMeasRequestFlag==1) {//측정값 분석
            Ulik_Parsing(comPort);
        }
        else if(gv.ulikToggleParaMeasRequestFlag==0) {//교정값 분석
            Ulik_ParameterParsing(comPort);
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
        if(gv.tmsStatusCode[comPort]==3) {//교정모드인경우    
            if(gv.ulikCalCmdSendNo==0) {
                if(gv.ulikToggleParaMeasRequestFlag==0) {
                    gv.ulikToggleParaMeasRequestFlag=1;
                    Ulik_ReqMeas(comPort,flash.ulik.id, 0x03);
                }
                else {
                    gv.ulikToggleParaMeasRequestFlag=0;
                    if(gv.ulikCalItem==0) {
                        Ulik_CODCalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    }
                    else if(gv.ulikCalItem==1){
                        Ulik_TOCCalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    }
                    else if(gv.ulikCalItem==2) {
                        Ulik_TURCalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    }
                    else if(gv.ulikCalItem==3) {
                        Ulik_COD_KPHCalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    }
                    else if(gv.ulikCalItem==4) {
                        Ulik_TOC_StdCalDataStorageRegisterTable(comPort,flash.ulik.id,0x03);
                    }
                }
            }
        }
        else {
            gv.ulikToggleParaMeasRequestFlag=1;
            Ulik_ReqMeas(comPort,flash.ulik.id, 0x03);
        }
    }    
    return 0;
}

#endif
