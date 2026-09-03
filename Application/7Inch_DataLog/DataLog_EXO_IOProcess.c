/******************************************************************************
*    	File name	: EXO_Process.c 
*		Description : DongMun RH100 Control Program
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: 
******************************************************************************/



/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "DataLog.h"

/***********************************************************************
*******
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/
/******************************************************************************
							Function Prototype
******************************************************************************/



static const unsigned short crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				EXO COMMAND LIST
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//EXO 전용
unsigned short CRCCCITT_RX(unsigned char *data, unsigned int length, unsigned short seed, unsigned short final)
{ 
    unsigned int count;
    unsigned int crc = seed;
    unsigned int temp;
    unsigned char buf = 0x7e;

    temp = (buf ^ (crc >> 8)) & 0xff;//시작신호 0x7E추가
    crc = crc16tab[temp] ^ (crc << 8);
    
    for (count = 0; count < length; ++count)
    {
        temp = (*data++ ^ (crc >> 8)) & 0xff;
        crc = crc16tab[temp] ^ (crc << 8);
    }

    return (unsigned short)(crc ^ final);
}
unsigned short CRCCCITT(unsigned char *data, unsigned int length, unsigned short seed, unsigned short final)
{ 
    unsigned int count;
    unsigned int crc = seed;
    unsigned int temp;

    for (count = 0; count < length; ++count)
    {
        temp = (*data++ ^ (crc >> 8)) & 0xff;
        crc = crc16tab[temp] ^ (crc << 8);
    }

    return (unsigned short)(crc ^ final);
}

#ifdef EXO_ENABLE
extern void EXO_ComRequestValueInit(int UartSelect);
volatile U32 EXO_IO_PROCESS;
extern volatile U32 EXO_CalApplyID;
int	EXO_Calibration_Command(unsigned int comPort, unsigned int destAddress, unsigned int sourceAddress, 
unsigned char title,unsigned char none, unsigned char item, unsigned char commandList, unsigned char length1, unsigned char length2, unsigned char *data)
{
	unsigned short 		crc=0;
	int						size=0, cnt=0;
	unsigned char 		uBuf[30];
	int						i=0;
    volatile int        len2=length2;
    volatile int        len1=length1;
    EXO_ComRequestValueInit(comPort);
    
	for(cnt=0;cnt<30;cnt++) uBuf[cnt]=0;
	
	uBuf[0]=0x7E;
	uBuf[1]=0x02;
	
	gv.exo_count+=0x01;
	if(gv.exo_count >= 0xFF) gv.exo_count=0x01; //데이터 전송시 카운트를 한다.
 	uBuf[2]=gv.exo_count;
	//ADDRESS
	if(COMMON_ID==destAddress)
	{
		uBuf[3]=0x00;uBuf[4]=0x00;uBuf[5]=0x00;uBuf[6]=0x00;
	}
	else if(COMMON_CALIBRATION_ID==destAddress)
	{
		uBuf[3]=flash.EXO.SensorAddress[comPort][flash.EXO.EXO_CalAddress][0];
		uBuf[4]=flash.EXO.SensorAddress[comPort][flash.EXO.EXO_CalAddress][1];
		uBuf[5]=flash.EXO.SensorAddress[comPort][flash.EXO.EXO_CalAddress][2];
		uBuf[6]=flash.EXO.SensorAddress[comPort][flash.EXO.EXO_CalAddress][3];  
		/*
		uBuf[3]=0x71;
		uBuf[4]=0x21;
		uBuf[5]=0x96;
		uBuf[6]=0x56;
		*/
	}
	else if(destAddress==40)
	{
		uBuf[3]=flash.EXO.EXO_Address[comPort][0];
		uBuf[4]=flash.EXO.EXO_Address[comPort][1];
		uBuf[5]=flash.EXO.EXO_Address[comPort][2];
		uBuf[6]=flash.EXO.EXO_Address[comPort][3];
	}
	else //센서정보를 읽어온다.
	{
		uBuf[3]=flash.EXO.SensorAddress[comPort][destAddress-1][0];
		uBuf[4]=flash.EXO.SensorAddress[comPort][destAddress-1][1];
		uBuf[5]=flash.EXO.SensorAddress[comPort][destAddress-1][2];
		uBuf[6]=flash.EXO.SensorAddress[comPort][destAddress-1][3]; 
        
        flash.EXO.imsiSensorAddress[comPort][destAddress-1][0]=flash.EXO.SensorAddress[comPort][destAddress-1][0];
        flash.EXO.imsiSensorAddress[comPort][destAddress-1][1]=flash.EXO.SensorAddress[comPort][destAddress-1][1];
        flash.EXO.imsiSensorAddress[comPort][destAddress-1][2]=flash.EXO.SensorAddress[comPort][destAddress-1][2];
        flash.EXO.imsiSensorAddress[comPort][destAddress-1][3]=flash.EXO.SensorAddress[comPort][destAddress-1][3];
        
        Dprintf("Add = %02x %02x %02x %02x ======>", uBuf[3],uBuf[4],uBuf[5],uBuf[6]);
	}
	switch(sourceAddress)
	{
		case SONDE_ID:		uBuf[7]=0xFF;uBuf[8]=0xB6;uBuf[9]=0xC6;uBuf[10]=0xA9; break;//SONDE
	}
	//COMMAND
	uBuf[11]=title;
	uBuf[12]=none;
	uBuf[13]=item;
	uBuf[14]=commandList;
	//LENGTH
	uBuf[15]=len1;
	uBuf[16]=len2;
	//DATA
	if(uBuf[15]!=0 || uBuf[16]!=0)
	{
		len2 = len2+17;//21
		size = len2;
		for(i=17;i<len2;i++) uBuf[i]=*data++;
	}
	else size = 17;
	crc=CRCCCITT(uBuf,size,0xffff,0);
	
	if(comPort==USART_1)
	{
		//Dprintf("EXO 1 OUT\n");
		//시작
		SENSOR_UART1(uBuf[0]); 
		SENSOR_UART1(uBuf[1]);
		SENSOR_UART1(uBuf[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART1(uBuf[3]);
		SENSOR_UART1(uBuf[4]);
		SENSOR_UART1(uBuf[5]);
		SENSOR_UART1(uBuf[6]);
		//DESTINATION ADDRESS (고정)
		SENSOR_UART1(uBuf[7]);
		SENSOR_UART1(uBuf[8]);
		SENSOR_UART1(uBuf[9]);
		SENSOR_UART1(uBuf[10]);
		//COMMAND
		SENSOR_UART1(uBuf[11]);//명령 타이틀
		SENSOR_UART1(uBuf[12]);//예약
		SENSOR_UART1(uBuf[13]);//센서종류
		SENSOR_UART1(uBuf[14]);//명령
		//LENGTH
		SENSOR_UART1(uBuf[15]);
		SENSOR_UART1(uBuf[16]);
		//DATA
		if(uBuf[15]!=0 || uBuf[16]!=0) 
		{
			for(i=17;i<len2;i++) SENSOR_UART1(uBuf[i]);
		}
		//CRC16
		SENSOR_UART1((crc & 0xFF00)>>8);
		SENSOR_UART1(crc & 0x00FF);
	}
	if(comPort==USART_2)
	{
		//시작
		SENSOR_UART2(uBuf[0]); 
		SENSOR_UART2(uBuf[1]);
		SENSOR_UART2(uBuf[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART2(uBuf[3]);
		SENSOR_UART2(uBuf[4]);
		SENSOR_UART2(uBuf[5]);
		SENSOR_UART2(uBuf[6]);
		//DESTINATION ADDRESS (고정)
		SENSOR_UART2(uBuf[7]);
		SENSOR_UART2(uBuf[8]);
		SENSOR_UART2(uBuf[9]);
		SENSOR_UART2(uBuf[10]);
		//COMMAND
		SENSOR_UART2(uBuf[11]);//명령 타이틀
		SENSOR_UART2(uBuf[12]);//예약
		SENSOR_UART2(uBuf[13]);//센서종류
		SENSOR_UART2(uBuf[14]);//명령
		//LENGTH
		SENSOR_UART2(uBuf[15]);
		SENSOR_UART2(uBuf[16]);
		//DATA
		if(uBuf[15]!=0 || uBuf[16]!=0) 
		{
			for(i=17;i<len2;i++) SENSOR_UART2(uBuf[i]);
		}
		//CRC16
		SENSOR_UART2((crc & 0xFF00)>>8);
		SENSOR_UART2(crc & 0x00FF);
	}
	if(comPort==USART_3)
	{
		//시작
		SENSOR_UART3(uBuf[0]); 
		SENSOR_UART3(uBuf[1]);
		SENSOR_UART3(uBuf[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART3(uBuf[3]);
		SENSOR_UART3(uBuf[4]);
		SENSOR_UART3(uBuf[5]);
		SENSOR_UART3(uBuf[6]);
		//DESTINATION ADDRESS (고정)
		SENSOR_UART3(uBuf[7]);
		SENSOR_UART3(uBuf[8]);
		SENSOR_UART3(uBuf[9]);
		SENSOR_UART3(uBuf[10]);
		//COMMAND
		SENSOR_UART3(uBuf[11]);//명령 타이틀
		SENSOR_UART3(uBuf[12]);//예약
		SENSOR_UART3(uBuf[13]);//센서종류
		SENSOR_UART3(uBuf[14]);//명령
		//LENGTH
		SENSOR_UART3(uBuf[15]);
		SENSOR_UART3(uBuf[16]);
		//DATA
		if(uBuf[15]!=0 || uBuf[16]!=0) 
		{
			for(i=17;i<len2;i++) SENSOR_UART3(uBuf[i]);
		}
		//CRC16
		SENSOR_UART3((crc & 0xFF00)>>8);
		SENSOR_UART3(crc & 0x00FF);
	}
	return 0;
}
//7E 02 0A 71 21 91 81 FF B6 C6 A9 20 00 00 15 00 02 00 00 DF 67
//7E 02 23 00 00 00 00 FF B6 C6 A9 20 00 00 22 00 00 82 24 
//
volatile int EXO_Find_CommandChangeFlag[3];
int EXO_SondeIDFind(int comPort)
{
	unsigned short 	crc=0;
	int					size=0;//, cnt=0;
	unsigned char EXO_SONDE_INFO[]		={0x7E, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x99, 0x99, 0x99, 0x99, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00};
		
	gv.exo_count+=0x01;
	if(gv.exo_count >= 0xFF) 
	{
		gv.exo_count=0x01; //데이터 전송시 카운트를 한다.
	}
	EXO_SONDE_INFO[2]=gv.exo_count;
	size = sizeof(EXO_SONDE_INFO);
	crc=CRCCCITT(EXO_SONDE_INFO,size,0xffff,0);
	
	if(comPort==USART_1)
	{
		//시작
		SENSOR_UART1(EXO_SONDE_INFO[0]); 
		SENSOR_UART1(EXO_SONDE_INFO[1]);
		SENSOR_UART1(EXO_SONDE_INFO[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART1(EXO_SONDE_INFO[3]);
		SENSOR_UART1(EXO_SONDE_INFO[4]);
		SENSOR_UART1(EXO_SONDE_INFO[5]);
		SENSOR_UART1(EXO_SONDE_INFO[6]);
		//DESTINATION ADDRESS (고정)
		SENSOR_UART1(EXO_SONDE_INFO[7]);
		SENSOR_UART1(EXO_SONDE_INFO[8]);
		SENSOR_UART1(EXO_SONDE_INFO[9]);
		SENSOR_UART1(EXO_SONDE_INFO[10]);
		//COMMAND
		SENSOR_UART1(EXO_SONDE_INFO[11]);//명령 타이틀
		SENSOR_UART1(EXO_SONDE_INFO[12]);//예약
		SENSOR_UART1(EXO_SONDE_INFO[13]);//센서종류
		SENSOR_UART1(EXO_SONDE_INFO[14]);//명령
		//LENGTH
		SENSOR_UART1(EXO_SONDE_INFO[15]);
		SENSOR_UART1(EXO_SONDE_INFO[16]);
		//CRC16
		SENSOR_UART1((crc & 0xFF00)>>8);
		SENSOR_UART1(crc & 0x00FF);
	}
	if(comPort==USART_2)
	{
		//시작
		SENSOR_UART2(EXO_SONDE_INFO[0]); 
		SENSOR_UART2(EXO_SONDE_INFO[1]);
		SENSOR_UART2(EXO_SONDE_INFO[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART2(EXO_SONDE_INFO[3]);
		SENSOR_UART2(EXO_SONDE_INFO[4]);
		SENSOR_UART2(EXO_SONDE_INFO[5]);
		SENSOR_UART2(EXO_SONDE_INFO[6]);
		//DESTINATION ADDRESS (고정)
		SENSOR_UART2(EXO_SONDE_INFO[7]);
		SENSOR_UART2(EXO_SONDE_INFO[8]);
		SENSOR_UART2(EXO_SONDE_INFO[9]);
		SENSOR_UART2(EXO_SONDE_INFO[10]);
		//COMMAND
		SENSOR_UART2(EXO_SONDE_INFO[11]);//명령 타이틀
		SENSOR_UART2(EXO_SONDE_INFO[12]);//예약
		SENSOR_UART2(EXO_SONDE_INFO[13]);//센서종류
		SENSOR_UART2(EXO_SONDE_INFO[14]);//명령
		//LENGTH
		SENSOR_UART2(EXO_SONDE_INFO[15]);
		SENSOR_UART2(EXO_SONDE_INFO[16]);
		//CRC16
		SENSOR_UART2((crc & 0xFF00)>>8);
		SENSOR_UART2(crc & 0x00FF);
	}
	if(comPort==USART_3)
	{
		//시작
		SENSOR_UART3(EXO_SONDE_INFO[0]); 
		SENSOR_UART3(EXO_SONDE_INFO[1]);
		SENSOR_UART3(EXO_SONDE_INFO[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART3(EXO_SONDE_INFO[3]);
		SENSOR_UART3(EXO_SONDE_INFO[4]);
		SENSOR_UART3(EXO_SONDE_INFO[5]);
		SENSOR_UART3(EXO_SONDE_INFO[6]);
		//DESTINATION ADDRESS (고정)
		SENSOR_UART3(EXO_SONDE_INFO[7]);
		SENSOR_UART3(EXO_SONDE_INFO[8]);
		SENSOR_UART3(EXO_SONDE_INFO[9]);
		SENSOR_UART3(EXO_SONDE_INFO[10]);
		//COMMAND
		SENSOR_UART3(EXO_SONDE_INFO[11]);//명령 타이틀
		SENSOR_UART3(EXO_SONDE_INFO[12]);//예약
		SENSOR_UART3(EXO_SONDE_INFO[13]);//센서종류
		SENSOR_UART3(EXO_SONDE_INFO[14]);//명령
		//LENGTH
		SENSOR_UART3(EXO_SONDE_INFO[15]);
		SENSOR_UART3(EXO_SONDE_INFO[16]);
		//CRC16
		SENSOR_UART3((crc & 0xFF00)>>8);
		SENSOR_UART3(crc & 0x00FF);
	}
	return 0;	
}
int	EXO_SensorFind(int comPort, int hexaSensorPort)
{
	unsigned short 	crc=0;
	int					size=0;//, cnt=0;
	unsigned char EXO_SENSOR_INFO[]		={0x7E, 0x02, 0x00, 0x51, 0x31, 0x89, 0x27, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x00, 0x15, 0x00, 0x02, 0x00, 0x00};
		
	EXO_SENSOR_INFO[3]=flash.EXO.EXO_Address[comPort][0];
	EXO_SENSOR_INFO[4]=flash.EXO.EXO_Address[comPort][1];
	EXO_SENSOR_INFO[5]=flash.EXO.EXO_Address[comPort][2];
	EXO_SENSOR_INFO[6]=flash.EXO.EXO_Address[comPort][3];
		
	gv.exo_count+=0x01;
	if(gv.exo_count >= 0xFF) 
	{
		gv.exo_count=0x01; //데이터 전송시 카운트를 한다
	}
	EXO_SENSOR_INFO[2]=gv.exo_count;
	EXO_SENSOR_INFO[18]=hexaSensorPort;//HEXA ADDRESS
    
    //Dprintf("PORT %d ",hexaSensorPort);
	
    size = sizeof(EXO_SENSOR_INFO);
	crc=CRCCCITT(EXO_SENSOR_INFO,size,0xffff,0);
	
	if(comPort==USART_1)
	{
		//시작
		SENSOR_UART1(EXO_SENSOR_INFO[0]); 
		SENSOR_UART1(EXO_SENSOR_INFO[1]);
		SENSOR_UART1(EXO_SENSOR_INFO[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART1(EXO_SENSOR_INFO[3]);
		SENSOR_UART1(EXO_SENSOR_INFO[4]);
		SENSOR_UART1(EXO_SENSOR_INFO[5]);
		SENSOR_UART1(EXO_SENSOR_INFO[6]);
		//DESTINATION ADDRESS (고정)
		SENSOR_UART1(EXO_SENSOR_INFO[7]);
		SENSOR_UART1(EXO_SENSOR_INFO[8]);
		SENSOR_UART1(EXO_SENSOR_INFO[9]);
		SENSOR_UART1(EXO_SENSOR_INFO[10]);
		//COMMAND
		SENSOR_UART1(EXO_SENSOR_INFO[11]);//명령 타이틀
		SENSOR_UART1(EXO_SENSOR_INFO[12]);//예약
		SENSOR_UART1(EXO_SENSOR_INFO[13]);//센서종류
		SENSOR_UART1(EXO_SENSOR_INFO[14]);//명령
		//LENGTH
		SENSOR_UART1(EXO_SENSOR_INFO[15]);
		SENSOR_UART1(EXO_SENSOR_INFO[16]);
		//DATA = SONDE 포트넘버가 들어간다. 포트에 연결된 센서 정보가 올라온다.
		SENSOR_UART1(EXO_SENSOR_INFO[17]);
		SENSOR_UART1(EXO_SENSOR_INFO[18]);
		//CRC16
		SENSOR_UART1((crc & 0xFF00)>>8);
		SENSOR_UART1(crc & 0x00FF);
	}
	if(comPort==USART_2)
	{
		//시작
		SENSOR_UART2(EXO_SENSOR_INFO[0]); 
		SENSOR_UART2(EXO_SENSOR_INFO[1]);
		SENSOR_UART2(EXO_SENSOR_INFO[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART2(EXO_SENSOR_INFO[3]);
		SENSOR_UART2(EXO_SENSOR_INFO[4]);
		SENSOR_UART2(EXO_SENSOR_INFO[5]);
		SENSOR_UART2(EXO_SENSOR_INFO[6]);
		//DESTINATION ADDRESS (고정)
		SENSOR_UART2(EXO_SENSOR_INFO[7]);
		SENSOR_UART2(EXO_SENSOR_INFO[8]);
		SENSOR_UART2(EXO_SENSOR_INFO[9]);
		SENSOR_UART2(EXO_SENSOR_INFO[10]);
		//COMMAND
		SENSOR_UART2(EXO_SENSOR_INFO[11]);//명령 타이틀
		SENSOR_UART2(EXO_SENSOR_INFO[12]);//예약
		SENSOR_UART2(EXO_SENSOR_INFO[13]);//센서종류
		SENSOR_UART2(EXO_SENSOR_INFO[14]);//명령
		//LENGTH
		SENSOR_UART2(EXO_SENSOR_INFO[15]);
		SENSOR_UART2(EXO_SENSOR_INFO[16]);
		//DATA = SONDE 포트넘버가 들어간다. 포트에 연결된 센서 정보가 올라온다.
		SENSOR_UART2(EXO_SENSOR_INFO[17]);
		SENSOR_UART2(EXO_SENSOR_INFO[18]);
		//CRC16
		SENSOR_UART2((crc & 0xFF00)>>8);
		SENSOR_UART2(crc & 0x00FF);
	}	
	if(comPort==USART_3)
	{
		//시작
		SENSOR_UART3(EXO_SENSOR_INFO[0]); 
		SENSOR_UART3(EXO_SENSOR_INFO[1]);
		SENSOR_UART3(EXO_SENSOR_INFO[2]);
		//SOURCE ADDRESS (고정)
		SENSOR_UART3(EXO_SENSOR_INFO[3]);
		SENSOR_UART3(EXO_SENSOR_INFO[4]);
		SENSOR_UART3(EXO_SENSOR_INFO[5]);
		SENSOR_UART3(EXO_SENSOR_INFO[6]);
		//DESTINATIO ADDRESS (고정)
		SENSOR_UART3(EXO_SENSOR_INFO[7]);
		SENSOR_UART3(EXO_SENSOR_INFO[8]);
		SENSOR_UART3(EXO_SENSOR_INFO[9]);
		SENSOR_UART3(EXO_SENSOR_INFO[10]);
		//COMMAND
		SENSOR_UART3(EXO_SENSOR_INFO[11]);//명령 타이틀
		SENSOR_UART3(EXO_SENSOR_INFO[12]);//예약
		SENSOR_UART3(EXO_SENSOR_INFO[13]);//센서종류
		SENSOR_UART3(EXO_SENSOR_INFO[14]);//명령
		//LENGTH
		SENSOR_UART3(EXO_SENSOR_INFO[15]);
		SENSOR_UART3(EXO_SENSOR_INFO[16]);
		//DATA = SONDE 포트넘버가 들어간다. 포트에 연결된 센서 정보가 올라온다.
		SENSOR_UART3(EXO_SENSOR_INFO[17]);
		SENSOR_UART3(EXO_SENSOR_INFO[18]);
		//CRC16
		SENSOR_UART3((crc & 0xFF00)>>8);
		SENSOR_UART3(crc & 0x00FF);
	}	
	return 0;
}
int Calibration_EC(int UartPort, float calBuffer)
{
	int UartSelect=UartPort;
	int		i=0;
	char	sd=0;
	int		onlyoneFlag=0;
	unsigned char	dataBuf[20];
    float   			mV=calBuffer;
    U32     			*address;
    //U8      			a, b, c, d;
    void    			*addr1; 
	unsigned int		itemCount=0;
	unsigned int		listNumber=0;
	addr1 = &mV;  
    address = addr1;
    //address = &mV;  
	
	//12번째 데이터
	while(1)
	{
		
		if(gv.nFlag_ReceiveDataProcess[UartSelect]==ON)
		{
			onlyoneFlag=0;
			for(i=0;i<gv.nReceiveBufferCount[UartSelect];i++)//DEBUG
			{
				if(gv.aItemReceiveBuffer[UartSelect][13]==0x19)//모든 측정값을 요청하면 1바이트를 쉬프트하여 처리한다.
				{
					if(onlyoneFlag==0)
					{
						onlyoneFlag=1;
						itemCount=0;
						i++;
					}
					mV=0.0f;
					*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 24); 	i++;
					*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 16);		i++; 
					*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 8); 		i++;
					*address |= gv.aItemReceiveBuffer[UartSelect][i];
					//Dprintf("%08x            %7.3f\n",*address, mV);
					if(itemCount==12)
					{
						flash.Uart[0]->SensorPV[listNumber]=mV;
					}
					itemCount++; 
				}
				else if(	  	gv.aItemReceiveBuffer[UartSelect][13]==0x15 // ROUTE TABLE ENTRY( 센서정보 )
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x17 // 해당원시데이터값
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x09 // 보정유형
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x05 // 보정계수
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x04 // 보정시간
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x03 // 보정시작명령
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x02 // 보정버퍼설정
				  )
				{
					if(gv.aItemReceiveBuffer[UartSelect][13]==0x15)// SONDE 에 연결된 센서 주소와 포트 번화를 읽어온다.
					{
						Dprintf("ADDRESS = ");
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][16]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][17]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][18]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][19]);
						Uart_SendByte2(0x0d);
						Dprintf("PORT = ");
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][20]);
						break;
					}
					else if(gv.aItemReceiveBuffer[UartSelect][13]==0x04)// 보정시작시간
					{
						Dprintf("EC CALIBRATION TIME\n");
						break;
					}
					else if(gv.aItemReceiveBuffer[UartSelect][13]==0x03)// 보정시작
					{
						Dprintf("EC CALIBRATION START \n");
						break;
					}
					else if(gv.aItemReceiveBuffer[UartSelect][13]==0x02)// 보정버퍼설정
					{
						Dprintf("EC BUFFER SET \n");
						break;
					}
					else if(gv.aItemReceiveBuffer[UartSelect][13]==0x14)// 보정완료
					{
						Dprintf("EC CALIBRATION END\n");
						break;
					}
				}
				else
				{
					Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][i]);
				}
			}
			Uart_SendByte2(0x0d);
			Uart_SendByte2(0x0d);
			gv.nFlag_ReceiveDataProcess[UartSelect]=OFF;//통신데이터를 여기서처리한다.
			gv.nReceiveBufferCount[UartSelect]=0;			
		}
		if(Dstatus())
		{
			sd=Dgetch();
			for(i=0;i<20;i++) dataBuf[i]=0;
			switch(sd)
			{
				case '0'://보정시간설정
				{
					EXO_Calibration_Command(UartSelect, EC_ID, SONDE_ID, 0x20, 0x00, 0x18, 0x04, 0x00, 0x00, dataBuf);
				}				
				break;
				case '1'://보정시작
				{
					EXO_Calibration_Command(UartSelect, EC_ID, SONDE_ID, 0x30, 0x00, 0x18, 0x03, 0x00, 0x00, dataBuf);
				}				
				break;
				case '2'://보정값설정
				{
					mV=calBuffer;
					dataBuf[0]= (*address>>24)&0xFF;     
					dataBuf[1]= (*address>>16)&0xFF; 
					dataBuf[2]= (*address>>8)&0xFF;  
					dataBuf[3]= *address & 0xFF;        				
					EXO_Calibration_Command(UartSelect, EC_ID, SONDE_ID, 0x30, 0x00, 0x18, 0x02, 0x00, 0x04, dataBuf);
				}				
				break;
				case '3'://보정종료시간
				{
					dataBuf[0]=0x51;
					dataBuf[1]=0xC4;
					dataBuf[2]=0x62;
					dataBuf[3]=0x18;
					EXO_Calibration_Command(UartSelect, EC_ID, SONDE_ID, 0x30, 0x00, 0x18, 0x04, 0x00, 0x04, dataBuf);
				}
				break;
				case '4'://보정종료
				{
					EXO_Calibration_Command(UartSelect, EC_ID, SONDE_ID, 0x20, 0x00, 0x18, 0x14, 0x00, 0x00, dataBuf);
				}
				break;
				case '5'://모든측정치요청
				{
					EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, dataBuf);
				}
				break;
			}
		}
		if(g_Timer_msec>1500)
		{
			g_Timer_msec=0;
			for(i=0;i<20;i++) dataBuf[i]=0;
			EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, dataBuf);
		}
	}
}
int Calibration_DEPTH(int UartPort, float calBuffer)
{
	int UartSelect=UartPort;
	int		i=0;
	char	sd=0;
	int		onlyoneFlag=0;
	unsigned char	dataBuf[20];
    float   			mV=calBuffer;
    U32     			*address;
    //U8      			a, b, c, d;
    void    			*addr1; 
    addr1 = &mV;
    address = addr1;
    //address = &mV;      
	//22번째데이터
	while(1)
	{
		
		if(gv.nFlag_ReceiveDataProcess[UartSelect]==ON)
		{
			onlyoneFlag=0;
			for(i=0;i<gv.nReceiveBufferCount[UartSelect];i++)//DEBUG
			{
				if(gv.aItemReceiveBuffer[UartSelect][13]==0x19)//모든 측정값을 요청하면 1바이트를 쉬프트하여 처리한다.
				{
					if(onlyoneFlag==0)
					{
						onlyoneFlag=1;
						i++;
					}
					mV=0.0f;
					*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 24); 	i++;
					*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 16);		i++; 
					*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 8); 		i++;
					*address |= gv.aItemReceiveBuffer[UartSelect][i];
					Dprintf("%08x            %7.3f\n",*address, mV);
				}
				else if(	  	gv.aItemReceiveBuffer[UartSelect][13]==0x15 // ROUTE TABLE ENTRY( 센서정보 )
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x17 // 해당원시데이터값
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x09 // 보정유형
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x05 // 보정계수
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x04 // 보정시간
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x03 // 보정시작명령
							||	gv.aItemReceiveBuffer[UartSelect][13]==0x02 // 보정버퍼설정
				  )
				{
					if(gv.aItemReceiveBuffer[UartSelect][13]==0x15)// SONDE 에 연결된 센서 주소와 포트 번화를 읽어온다.
					{
						Dprintf("DEPTH ADDRESS = ");
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][16]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][17]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][18]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][19]);
						Uart_SendByte2(0x0d);
						Dprintf("PORT = ");
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][20]);
						break;
					}
					else if(gv.aItemReceiveBuffer[UartSelect][13]==0x04)// 보정시작시간
					{
						Dprintf("DEPTH CALIBRATION TIME\n");
						break;
					}
					else if(gv.aItemReceiveBuffer[UartSelect][13]==0x03)// 보정시작
					{
						Dprintf("DEPTH CALIBRATION START \n");
						break;
					}
					else if(gv.aItemReceiveBuffer[UartSelect][13]==0x02)// 보정버퍼설정
					{
						Dprintf("DEPTH BUFFER SET \n");
						break;
					}
					else if(gv.aItemReceiveBuffer[UartSelect][13]==0x14)// 보정완료
					{
						Dprintf("DEPTH CALIBRATION END\n");
						break;
					}
				}
				else
				{
					Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][i]);
				}
			}
			Uart_SendByte2(0x0d);
			Uart_SendByte2(0x0d);
			gv.nFlag_ReceiveDataProcess[UartSelect]=OFF;//통신데이터를 여기서처리한다.
			gv.nReceiveBufferCount[UartSelect]=0;			
		}
		if(Dstatus())
		{
			sd=Dgetch();
			for(i=0;i<20;i++) dataBuf[i]=0;
			switch(sd)
			{
				case '0'://보정시간설정
				{
					EXO_Calibration_Command(UartSelect, DEPTH_ID, SONDE_ID, 0x20, 0x00, 0x24, 0x04, 0x00, 0x00, dataBuf);
				}				
				break;
				case '1'://보정시작
				{
					EXO_Calibration_Command(UartSelect, DEPTH_ID, SONDE_ID, 0x30, 0x00, 0x24, 0x03, 0x00, 0x00, dataBuf);
				}				
				break;
				case '2'://보정값설정
				{
					mV=calBuffer;
					dataBuf[0]= (*address>>24)&0xFF;     
					dataBuf[1]= (*address>>16)&0xFF; 
					dataBuf[2]= (*address>>8)&0xFF;  
					dataBuf[3]= *address & 0xFF;        				
					EXO_Calibration_Command(UartSelect, DEPTH_ID, SONDE_ID, 0x30, 0x00, 0x24, 0x02, 0x00, 0x04, dataBuf);
				}				
				break;
				case '3'://보정종료시간
				{
					dataBuf[0]=0x51;
					dataBuf[1]=0xC4;
					dataBuf[2]=0x62;
					dataBuf[3]=0x18;
					EXO_Calibration_Command(UartSelect, DEPTH_ID, SONDE_ID, 0x30, 0x00, 0x24, 0x04, 0x00, 0x04, dataBuf);
				}
				break;
				case '4'://보정종료
				{
					EXO_Calibration_Command(UartSelect, DEPTH_ID, SONDE_ID, 0x20, 0x00, 0x24, 0x14, 0x00, 0x00, dataBuf);
				}
				break;
				case '5'://모든측정치요청
				{
					EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, dataBuf);
				}
				break;
			}
		}
		if(g_Timer_msec>1500)
		{
			g_Timer_msec=0;
			for(i=0;i<20;i++) dataBuf[i]=0;
			EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, dataBuf);
		}
	}
}



int EXO_Test2(void) {
	int		UartSelect=USART_2;
	int		i=0;
	char	sd=0;
	unsigned char	dataBuf[20];
	
	Dprintf("EXO TEST PROGRAM\n");
	for(i=0;i<20;i++) dataBuf[i]=0;
	while(1)
	{
		if(gv.nFlag_ReceiveDataProcess[UartSelect]==ON)
		{
			for(i=0;i<gv.nReceiveBufferCount[UartSelect];i++)//DEBUG
			{
				if(	  	gv.aItemReceiveBuffer[UartSelect][13]==0x15 // ROUTE TABLE ENTRY( 센서정보 )
					||	gv.aItemReceiveBuffer[UartSelect][13]==0x17 // 해당원시데이터값
					||	gv.aItemReceiveBuffer[UartSelect][13]==0x09 // 보정유형
					||	gv.aItemReceiveBuffer[UartSelect][13]==0x05 // 보정계수
				  )
				{
					if(gv.aItemReceiveBuffer[UartSelect][13]==0x15)// SONDE 에 연결된 센서 주소와 포트 번화를 읽어온다.
					{
						Dprintf("ADDRESS = ");
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][16]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][17]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][18]);
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][19]);
						Uart_SendByte2(0x0d);
						Dprintf("PORT = ");
						Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][20]);
						break;
					}
				}
				else
				{
					Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][i]);
				}
			}
			Uart_SendByte2(0x0d);
			Uart_SendByte2(0x0d);
			gv.nFlag_ReceiveDataProcess[UartSelect]=OFF;//통신데이터를 여기서처리한다.
			gv.nReceiveBufferCount[UartSelect]=0;			
		}
		if(Dstatus())
		{
			sd=Dgetch();
			switch(sd)
			{
				//FIND PORT	
				case '1':	EXO_SensorFind(USART_2, 0x00);break;
				case '2':	EXO_SensorFind(USART_2, 0x01);break;
				case '3':	EXO_SensorFind(USART_2, 0x02);break;
				case '4':	EXO_SensorFind(USART_2, 0x03);break;
				case '5':	EXO_SensorFind(USART_2, 0x04);break;
				
				case 'a':// STATUS
											EXO_Calibration_Command(USART_2, 0, 0, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, dataBuf);
				break;
				
				/*
				case 'b':// COND ON	
											dataBuf[0]=0x01;
											EXO_Calibration_Command(USART_2, 1, 0, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, dataBuf);
				break;
				case 'c':// END ON	
											dataBuf[0]=0x01;
											EXO_Calibration_Command(USART_2, 5, 0, 0x30, 0x00, 0x00, 0x03, 0x00, 0x01, dataBuf);
				break;
				case 'd':// CAL START	
											EXO_Calibration_Command(USART_2, 1, 0, 0x30, 0x00, 0x18, 0x03, 0x00, 0x00, dataBuf);
											Delay(2000);
											dataBuf[0]=0x00;
											EXO_Calibration_Command(USART_2, 1, 0, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, dataBuf);
				break;
				case 'e':// 임시데이터	
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x12, 0x00, 0x00, dataBuf);
				break;
				case 'f':// 해당원시데이터값	
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x17, 0x00, 0x00, dataBuf);
				break;
				case 'g':// 해당원시데이터값	
											dataBuf[0] = 0x42;dataBuf[1] = 0xF0;dataBuf[2] = 0x00;dataBuf[3] = 0x00;
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x17, 0x00, 0x00, dataBuf);
				break;
				case 'h':// 보정유효성
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x09, 0x00, 0x00, dataBuf);
				break;
				case 'i':// 보정계수
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x05, 0x00, 0x00, dataBuf);
				break;
				case 'j':// 보정완료
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x14, 0x00, 0x00, dataBuf);//0x14
											Delay(2000);
											dataBuf[0]=0x00;
											EXO_Calibration_Command(USART_2, 1, 0, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, dataBuf);//COND OFF
											Delay(2000);
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x12, 0x00, 0x00, dataBuf);//임시데이터
											Delay(2000);
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x17, 0x00, 0x00, dataBuf);//해당원시데이터값
											Delay(2000);
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x0E, 0x00, 0x00, 0x00, dataBuf);//온도데이터
											Delay(2000);
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x05, 0x00, 0x00, dataBuf);//보정계수
											Delay(2000);
											dataBuf[0]=0x00;
											EXO_Calibration_Command(USART_2, 1, 0, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, dataBuf);//COND OFF
											Delay(2000);
											dataBuf[0]=0x00;
											EXO_Calibration_Command(USART_2, 5, 0, 0x30, 0x00, 0x00, 0x03, 0x00, 0x01, dataBuf);
				break;
				case 'k'://전도도값
											EXO_Calibration_Command(USART_2, 1, 0, 0x20, 0x00, 0x18, 0x14, 0x00, 0x00, dataBuf);//0x14
				break;
				*/
			}
		}
	}
}

unsigned char EXO_STANDBY[]				={0x7E, 0x02, 0x56, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char EXO_ALL_DATA[]				={0x7e, 0x02, 0x69, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00};

unsigned char EXO_DATALIST[]				={0x7E, 0x02, 0x07, 0x71, 0x21, 0x91, 0x81, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x00, 0x21, 0x00, 0x00};

unsigned char EXO_DATACOND[]			={0x7E, 0x02, 0x4E, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x18, 0x12, 0x00, 0x00};
unsigned char EXO_DATA_TEMP[]			={0x7E, 0x02, 0x43, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x0E, 0x00, 0x00, 0x00};
unsigned char EXO_CAL_STATUS[]			={0x7E, 0x02, 0x44, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x18, 0x13, 0x00, 0x00};
unsigned char EXO_CAL_BUFFER[]			={0x7E, 0x02, 0x82, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x18, 0x02, 0x00, 0x04, 0x43, 0x02, 0x00, 0x00};

unsigned char EXO_DIS_ENVIRONMENTAL1[]	={0x7E, 0x02, 0x3C, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x01};
unsigned char EXO_DIS_ENVIRONMENTAL2[]	={0x7E, 0x02, 0x3C, 0x71, 0x21, 0x94, 0x5C, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x01};
unsigned char EXO_DIS_ENVIRONMENTAL3[]	={0x7E, 0x02, 0x3C, 0x71, 0x21, 0x91, 0x9F, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x01};
unsigned char EXO_DIS_ENVIRONMENTAL4[]	={0x7E, 0x02, 0x3C, 0x61, 0x21, 0x90, 0x50, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x01};
unsigned char EXO_DIS_ENVIRONMENTAL5[]	={0x7E, 0x02, 0x3C, 0x61, 0x21, 0x8E, 0x07, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x01};

unsigned char EXO_DIS_ENVIRONMENTAL10[]	={0x7E, 0x02, 0x3C, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x00};
unsigned char EXO_DIS_ENVIRONMENTAL20[]	={0x7E, 0x02, 0x3C, 0x71, 0x21, 0x94, 0x5C, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x00};
unsigned char EXO_DIS_ENVIRONMENTAL30[]	={0x7E, 0x02, 0x3C, 0x71, 0x21, 0x91, 0x9F, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x00};
unsigned char EXO_DIS_ENVIRONMENTAL40[]	={0x7E, 0x02, 0x3C, 0x61, 0x21, 0x90, 0x50, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x00};
unsigned char EXO_DIS_ENVIRONMENTAL50[]	={0x7E, 0x02, 0x3C, 0x61, 0x21, 0x8E, 0x07, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, 0x00};

unsigned char EXO_RAW_DATA[]					={0x7E, 0x02, 0x42, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x18, 0x17, 0x00, 0x00};
unsigned char EXO_SENSOR_RANGE[]				={0x7E, 0x02, 0x46, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x18, 0x10, 0x00, 0x00};

unsigned char EXO_CAL_COEFFICIENTS[]			={0x7E, 0x02, 0x48, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x18, 0x05, 0x00, 0x00};
unsigned char EXO_CELL_CONSTANT[]				={0x7E, 0x02, 0x4A, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x11, 0xEC, 0x00, 0x00 };
unsigned char EXO_CAL_POINT[]					={0x7E, 0x02, 0x45, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x18, 0x15, 0x00, 0x00};

unsigned char EXO_REBUILD[]						={0x7E, 0x02, 0x4A, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x18, 0x14, 0x00, 0x00};	

unsigned char EXO_CONFIG[]						={0x7E, 0x02, 0x0B, 0x71, 0x21, 0x91, 0x81, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x01, 0xFD, 0x00, 0x00};
unsigned char EXO_EC_ROUTE_TABLE_ENTRY[]		={0x7E, 0x02, 0x0A, 0x71, 0x21, 0x91, 0x81, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x00, 0x15, 0x00, 0x02, 0x00, 0x00};
unsigned char EXO_EC_ALL_DEVICE_INFORMATION[]={0x7E, 0x02, 0x0C, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00};
unsigned char EXO_EC_STATUS[] = 						  {0x7E, 0x02, 0x0D, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00};

unsigned char EXO_CALSTART[]=						{0x7E, 0x02, 0x9E, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x18, 0x03, 0x00, 0x00};

unsigned char	EXO_BUFFER_NUMBER[]			={0x7E, 0x02, 0xDD, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x18, 0x07, 0x00, 0x01, 0x00};

unsigned char	EXO_CAL_TIME1[]			={0x7E, 0x02, 0x79, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6 ,0xA9, 0x20, 0x00, 0x18, 0x04, 0x00, 0x00};
unsigned char EXO_CAL_TIME2[]		={0x7E, 0x02, 0x31, 0x71, 0x21, 0x96, 0x56, 0xFF, 0xB6, 0xC6, 0xA9, 0x30, 0x00, 0x18, 0x04, 0x00, 0x04, 0x51, 0xC4, 0x62, 0x18};

int	EXO_Transmit(unsigned int comport,  int command) {
	int		cnt=0;
	unsigned short crc;
	int		size;
    float   			mV=100;
    U32     			*address;
    U8      			a, b, c, d;
    void    			*addr1; 
	//unsigned char 	sd=0;

    addr1 = &mV;
    address = addr1;

	
    a= (*address>>24)&0xFF;     //MSB
    b= (*address>>16)&0xFF; 
    c= (*address>>8)&0xFF;  
    d= *address & 0xFF;         //LSB

	gv.exo_count+=0x01;
	if(gv.exo_count >= 0xFF) gv.exo_count=0x01;//COMMAND COUNT
	
	switch(command)
	{
		case 0:
		{
			size = sizeof(EXO_ALL_DATA);
			EXO_ALL_DATA[2]=gv.exo_count;
			crc=CRCCCITT(EXO_ALL_DATA,size,0xffff,0);
		}break;
		case 1:
		{
			size = sizeof(EXO_DATALIST);
			EXO_DATALIST[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DATALIST,size,0xffff,0);
		}break;
		case 2:
		{
			size = sizeof(EXO_DATACOND);
			EXO_DATACOND[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DATACOND,size,0xffff,0);
		}break;
		case 3:
		{
			size = sizeof(EXO_STANDBY);
			EXO_STANDBY[2]=gv.exo_count;
			crc=CRCCCITT(EXO_STANDBY,size,0xffff,0);
		}break;
		case 4:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL1);
			EXO_DIS_ENVIRONMENTAL1[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL1,size,0xffff,0);
		}break;
		case 5:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL2);
			EXO_DIS_ENVIRONMENTAL2[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL2,size,0xffff,0);
		}break;
		case 6:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL3);
			EXO_DIS_ENVIRONMENTAL3[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL3,size,0xffff,0);
		}break;
		case 7:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL4);
			EXO_DIS_ENVIRONMENTAL4[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL4,size,0xffff,0);
		}break;
		case 8:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL5);
			EXO_DIS_ENVIRONMENTAL5[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL5,size,0xffff,0);
		}break;
		case 9:
		{
			size = sizeof(EXO_DATA_TEMP);
			EXO_DATA_TEMP[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DATA_TEMP,size,0xffff,0);
		}break;
		case 10:
		{
			size = sizeof(EXO_CAL_STATUS);
			EXO_CAL_STATUS[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CAL_STATUS,size,0xffff,0);
		}break;
		case 11:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL10);
			EXO_DIS_ENVIRONMENTAL10[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL10,size,0xffff,0);
		}break;
		case 12:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL20);
			EXO_DIS_ENVIRONMENTAL20[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL20,size,0xffff,0);
		}break;
		case 13:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL30);
			EXO_DIS_ENVIRONMENTAL30[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL30,size,0xffff,0);
		}break;
		case 14:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL40);
			EXO_DIS_ENVIRONMENTAL40[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL40,size,0xffff,0);
		}break;
		case 15:
		{
			size = sizeof(EXO_DIS_ENVIRONMENTAL50);
			EXO_DIS_ENVIRONMENTAL50[2]=gv.exo_count;
			crc=CRCCCITT(EXO_DIS_ENVIRONMENTAL50,size,0xffff,0);
		}break;
		case 16:
		{
			EXO_CAL_BUFFER[17]=a;
			EXO_CAL_BUFFER[18]=b;
			EXO_CAL_BUFFER[19]=c;
			EXO_CAL_BUFFER[20]=d;
			size = sizeof(EXO_CAL_BUFFER);
			EXO_CAL_BUFFER[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CAL_BUFFER,size,0xffff,0);
		}break;
		case 17:
		{
			size = sizeof(EXO_RAW_DATA);
			EXO_RAW_DATA[2]=gv.exo_count;
			crc=CRCCCITT(EXO_RAW_DATA,size,0xffff,0);
		}break;
		case 18:
		{			
			size = sizeof(EXO_SENSOR_RANGE);
			EXO_SENSOR_RANGE[2]=gv.exo_count;
			crc=CRCCCITT(EXO_SENSOR_RANGE,size,0xffff,0);
		}break;
		case 19:
		{
			size = sizeof(EXO_CAL_COEFFICIENTS);
			EXO_CAL_COEFFICIENTS[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CAL_COEFFICIENTS,size,0xffff,0);
		}break;
		case 20:
		{			
			size = sizeof(EXO_CELL_CONSTANT);
			EXO_CELL_CONSTANT[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CELL_CONSTANT,size,0xffff,0);
		}break;
		case 21:
		{
			size = sizeof(EXO_CAL_POINT);
			EXO_CAL_POINT[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CAL_POINT,size,0xffff,0);
		}break;
		case 22:
		{
			size = sizeof(EXO_REBUILD);
			EXO_REBUILD[2]=gv.exo_count;
			crc=CRCCCITT(EXO_REBUILD,size,0xffff,0);
		}break;
		case 23:
		{
			size = sizeof(EXO_CONFIG);
			EXO_CONFIG[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CONFIG,size,0xffff,0);
		}break;
		case 24://EXO_EC_ROUTE_TABLE_ENTRY
		{
			size = sizeof(EXO_EC_ROUTE_TABLE_ENTRY);
			EXO_EC_ROUTE_TABLE_ENTRY[2]=gv.exo_count;
			crc=CRCCCITT(EXO_EC_ROUTE_TABLE_ENTRY,size,0xffff,0);
		}break;
		case 25://EXO_EC_ALL_DEVICE_INFORMATION
		{
			size = sizeof(EXO_EC_ALL_DEVICE_INFORMATION);
			EXO_EC_ALL_DEVICE_INFORMATION[2]=gv.exo_count;
			crc=CRCCCITT(EXO_EC_ALL_DEVICE_INFORMATION,size,0xffff,0);
		}break;
		case 26://EXO_EC_STATUS
		{
			size = sizeof(EXO_EC_STATUS);
			EXO_EC_STATUS[2]=gv.exo_count;
			crc=CRCCCITT(EXO_EC_STATUS,size,0xffff,0);
		}break;
		case 27://EXO_CALSTART
		{
			size = sizeof(EXO_CALSTART);
			EXO_CALSTART[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CALSTART,size,0xffff,0);
		}break;
		case 28:
		{
			size=sizeof(EXO_BUFFER_NUMBER);
			EXO_BUFFER_NUMBER[2]=gv.exo_count;
			crc=CRCCCITT(EXO_BUFFER_NUMBER,size,0xffff,0);
		}break;
		case 29:
		{
			size=sizeof(EXO_CAL_TIME1);
			EXO_CAL_TIME1[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CAL_TIME1,size,0xffff,0);
		}break;
		
		case 30:
		{
			size=sizeof(EXO_CAL_TIME2);
			EXO_CAL_TIME2[2]=gv.exo_count;
			crc=CRCCCITT(EXO_CAL_TIME2,size,0xffff,0);
		}break;
	}
	
	for(cnt=0;cnt<size;cnt++)
	{
		if(cnt==2)//데이터 송신 카운트값
		{
			if(comport == USART_1)SENSOR_UART1(gv.exo_count);
			else if(comport == USART_2)SENSOR_UART2(gv.exo_count);
			else if(comport == USART_3)SENSOR_UART3(gv.exo_count);
		}
		else
		{
			if(comport == USART_1)
			{
				switch(command)
				{
					case 0: SENSOR_UART1(EXO_ALL_DATA[cnt]); break;
					case 1: SENSOR_UART1(EXO_DATALIST[cnt]); break;
					case 2: SENSOR_UART1(EXO_DATACOND[cnt]); break;
					case 3: SENSOR_UART1(EXO_STANDBY[cnt]); break;
					case 4: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL1[cnt]); break;
					case 5: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL2[cnt]); break;
					case 6: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL3[cnt]); break;
					case 7: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL4[cnt]); break;
					case 8: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL5[cnt]); break;
					case 9: SENSOR_UART1(EXO_DATA_TEMP[cnt]); break;
					case 10: SENSOR_UART1(EXO_CAL_STATUS[cnt]); break;
					case 11: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL10[cnt]); break;
					case 12: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL20[cnt]); break;
					case 13: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL30[cnt]); break;
					case 14: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL40[cnt]); break;
					case 15: SENSOR_UART1(EXO_DIS_ENVIRONMENTAL50[cnt]); break;
					case 16: SENSOR_UART1(EXO_CAL_BUFFER[cnt]); break;
					case 17: SENSOR_UART1(EXO_RAW_DATA[cnt]); break;
					case 18: SENSOR_UART1(EXO_SENSOR_RANGE[cnt]); break;	
					case 19: SENSOR_UART1(EXO_CAL_COEFFICIENTS[cnt]); break;	
					case 20: SENSOR_UART1(EXO_CELL_CONSTANT[cnt]); break;
					case 21: SENSOR_UART1(EXO_CAL_POINT[cnt]); break;
					case 22: SENSOR_UART1(EXO_REBUILD[cnt]); break;
				}
			}
			else if(comport == USART_2)
			{
				switch(command)
				{
					case 0: SENSOR_UART2(EXO_ALL_DATA[cnt]); break;
					case 1: SENSOR_UART2(EXO_DATALIST[cnt]); break;
					case 2: SENSOR_UART2(EXO_DATACOND[cnt]); break;
					case 3: SENSOR_UART2(EXO_STANDBY[cnt]); break;
					case 4: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL1[cnt]); break;
					case 5: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL2[cnt]); break;
					case 6: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL3[cnt]); break;
					case 7: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL4[cnt]); break;
					case 8: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL5[cnt]); break;
					case 9: SENSOR_UART2(EXO_DATA_TEMP[cnt]); break;
					case 10: SENSOR_UART2(EXO_CAL_STATUS[cnt]); break;
					case 11: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL10[cnt]); break;
					case 12: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL20[cnt]); break;
					case 13: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL30[cnt]); break;
					case 14: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL40[cnt]); break;
					case 15: SENSOR_UART2(EXO_DIS_ENVIRONMENTAL50[cnt]); break;
					case 16: SENSOR_UART2(EXO_CAL_BUFFER[cnt]); break;
					case 17: SENSOR_UART2(EXO_RAW_DATA[cnt]); break;
					case 18: SENSOR_UART2(EXO_SENSOR_RANGE[cnt]); break;
					case 19: SENSOR_UART2(EXO_CAL_COEFFICIENTS[cnt]); break;	
					case 20: SENSOR_UART2(EXO_CELL_CONSTANT[cnt]); break;
					case 21: SENSOR_UART2(EXO_CAL_POINT[cnt]); break;
					case 22: SENSOR_UART2(EXO_REBUILD[cnt]); break;
					case 23: SENSOR_UART2(EXO_CONFIG[cnt]);break;
					case 24: SENSOR_UART2(EXO_EC_ROUTE_TABLE_ENTRY[cnt]);break;
					case 25: SENSOR_UART2(EXO_EC_ALL_DEVICE_INFORMATION[cnt]);break;
					case 26: SENSOR_UART2(EXO_EC_STATUS[cnt]);break;
					case 27: SENSOR_UART2(EXO_CALSTART[cnt]);break;
					case 28: SENSOR_UART2(EXO_BUFFER_NUMBER[cnt]);break;
					case 29: SENSOR_UART2(EXO_CAL_TIME1[cnt]);break;
					case 30: SENSOR_UART2(EXO_CAL_TIME2[cnt]);break;
				}
			}
			else if(comport == USART_3)
			{
				switch(command)
				{
					case 0: SENSOR_UART3(EXO_ALL_DATA[cnt]); break;
					case 1: SENSOR_UART3(EXO_DATALIST[cnt]); break;
					case 2: SENSOR_UART3(EXO_DATACOND[cnt]); break;
					case 3: SENSOR_UART3(EXO_STANDBY[cnt]); break;
					case 4: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL1[cnt]); break;
					case 5: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL2[cnt]); break;
					case 6: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL3[cnt]); break;
					case 7: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL4[cnt]); break;
					case 8: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL5[cnt]); break;
					case 9: SENSOR_UART3(EXO_DATA_TEMP[cnt]); break;
					case 10: SENSOR_UART3(EXO_CAL_STATUS[cnt]); break;
					case 11: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL10[cnt]); break;
					case 12: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL20[cnt]); break;
					case 13: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL30[cnt]); break;
					case 14: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL40[cnt]); break;
					case 15: SENSOR_UART3(EXO_DIS_ENVIRONMENTAL50[cnt]); break;
					case 16: SENSOR_UART3(EXO_CAL_BUFFER[cnt]); break;
					case 17: SENSOR_UART3(EXO_RAW_DATA[cnt]); break;
					case 18: SENSOR_UART3(EXO_SENSOR_RANGE[cnt]); break;
					case 19: SENSOR_UART3(EXO_CAL_COEFFICIENTS[cnt]); break;	
					case 20: SENSOR_UART3(EXO_CELL_CONSTANT[cnt]); break;
					case 21: SENSOR_UART3(EXO_CAL_POINT[cnt]); break;
					case 22: SENSOR_UART3(EXO_REBUILD[cnt]); break;
				}
			}
		}
	}
	if(comport == USART_1)
	{
		SENSOR_UART1((crc & 0xFF00)>>8);
		SENSOR_UART1(crc & 0x00FF);
	}
	else if(comport == USART_2)
	{
		SENSOR_UART2((crc & 0xFF00)>>8);
		SENSOR_UART2(crc & 0x00FF);
	}
	else if(comport == USART_3)
	{
		SENSOR_UART3((crc & 0xFF00)>>8);
		SENSOR_UART3(crc & 0x00FF);
	}
	return 0;
}


void EXO_Test(void) {
    float   			mV=130.0;
    U32     			*address;
    //U8      			a, b, c, d;
    void    			*addr1; 
	unsigned char 	sd=0;
	int					i=0;
	//int					count=0;
	int					UartSelect=USART_2;
	int					onlyoneFlag=0;
    addr1 = &mV;
    address = addr1;
	
    Dprintf("EXO PROGRAM START\n");
    /*
	*address = (0x3E<< 24) | (0xCD << 16) | (0xA4 << 8) | 0x04;  //1.2345를 분해한값    
    Dprintf("2------> %08x %f\n", *address, mV);
	*/
	
	while(1)
	{
		onlyoneFlag=0;
		if(gv.nFlag_ReceiveDataProcess[UartSelect]==ON)
		{
			for(i=0;i<gv.nReceiveBufferCount[UartSelect];i++)//DEBUG
			{
				if(	  gv.aItemReceiveBuffer[UartSelect][13]==0x12 //측정값
					|| gv.aItemReceiveBuffer[UartSelect][13]==0x17 //원시데이터
					|| gv.aItemReceiveBuffer[UartSelect][12]==0x0E //온도
					|| gv.aItemReceiveBuffer[UartSelect][13]==0x10 //측정범위
					|| gv.aItemReceiveBuffer[UartSelect][13]==0x05 //보정계수
					|| gv.aItemReceiveBuffer[UartSelect][13]==0xEC //셀상수
					|| gv.aItemReceiveBuffer[UartSelect][13]==0x19
				  )
				{
					mV=0;
					if(i >=16 )
					{
						if(gv.aItemReceiveBuffer[UartSelect][13]==0x19)//모든 측정값을 요청하면 1바이트를 쉬프트하여 처리한다.
						{
							if(onlyoneFlag==0)
							{
								onlyoneFlag=1;
								i++;
							}
						}
						*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 24); 	i++;
						*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 16);		i++; 
						*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 8); 		i++;
						*address |= gv.aItemReceiveBuffer[UartSelect][i];
						Dprintf("%08x            %7.3f\n",*address, mV);
					}
				}
				else
				{
					Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][i]);
				}
			}
			Uart_SendByte2(0x0d);
			Uart_SendByte2(0x0d);
			gv.nFlag_ReceiveDataProcess[UartSelect]=OFF;//통신데이터를 여기서처리한다.
			gv.nReceiveBufferCount[UartSelect]=0;			
		}
		
		if(Dstatus())
		{
			sd=Dgetch();
			switch(sd)
			{
				case '1':
				{
					EXO_Transmit(USART_2, 3);//Standby
				}break;
				case '2':
				{
					EXO_Transmit(USART_2, 11);
				}break;
				case '4':
				{
					EXO_Transmit(USART_2, 5);
				}break;
				case '5':
				{
					EXO_Transmit(USART_2, 7);
				}break;
				case '6':
				{
					EXO_Transmit(USART_2, 8);
				}break;				
				case '7'://보정값을 설정한다.
				{
					EXO_Transmit(USART_2, 16);
				}break;
				case '8'://전도도 측정값을 요청한다.
				{
					EXO_Transmit(USART_2, 2);			
				}break;
				case '9':
				{
					EXO_Transmit(USART_2, 11);
				}break;
				case '0':
				{
					EXO_Transmit(USART_2, 12);
				}break;
				case 'a':
				{
					EXO_Transmit(USART_2, 13);
				}break;
				case 'b':
				{
					EXO_Transmit(USART_2, 14);
				}break;
				case 'c':
				{
					EXO_Transmit(USART_2, 15);
				}break;
				case 'd'://원시데이터
				{
					EXO_Transmit(USART_2, 17);
				}break;
				case 'e'://온도데이터
				{
					EXO_Transmit(USART_2, 9);
				}break;
				case 'f'://센서레인지
				{
					EXO_Transmit(USART_2, 18);
				}break;
				case 'h'://보정상태
				{
					EXO_Transmit(USART_2, 10);
				}break;
				case 'i'://셀상수 
				{
					EXO_Transmit(USART_2, 20);
				}break;
				case 'j'://보정포인트
				{
					EXO_Transmit(USART_2, 21);
				}break;
				case 'k':// 현재는 보정완료 명령으로 본다.
				{
					EXO_Transmit(USART_2, 22);
				}break;
				case 'l'://모든 측정치를 요청한다.
				{
					EXO_Transmit(USART_2, 0);
				}break;
				case 'm'://ALL DATA LIST
				{
					EXO_Transmit(USART_2, 1);
				}break;
				case 'n': //EXO_CONFIG
				{
					EXO_Transmit(USART_2,23);
				}break;
				case 'o': //EXO_EC_ROUTE_TABLE_ENTRY
				{
					EXO_Transmit(USART_2,24);
				}break;
				case 'p': //EXO_EC_ALL_DEVICE_INFORMATION
				{
					EXO_Transmit(USART_2,25);
				}break;
				case 'q'://EXO_EC_STATU
				{
					EXO_Transmit(USART_2,26);
				}break;
				case 't'://버퍼넘버선택
				{
					EXO_Transmit(USART_2,28);
		
				}break;
				case 'u'://보정시작시간
				{
					EXO_Transmit(USART_2,29);
		
				}break;
				case 'v'://보정종료시간
				{
					EXO_Transmit(USART_2,30);
		
				}break;
				case 'g'://보정계수
				{
					EXO_Transmit(USART_2, 19);
				}break;
				case 'r'://CAL START
				{
					EXO_Transmit(USART_2,27);
	
				}break;
				case 's':
				{
					EXO_Transmit(USART_2, 4);//전도도센서 OFF
				}break;
				case '3':
				{
					EXO_Transmit(USART_2, 11);//전도도센서 ON //346
				}break;
				
			}
		}
		
		if(g_Timer_msec>1500)
		{
			g_Timer_msec=0;
			EXO_Transmit(USART_2, 3);
			/*
			switch(count)
			{
				case 0: EXO_Transmit(USART_2, 11);break;
				case 1: EXO_Transmit(USART_2, 2);break;
				case 2: EXO_Transmit(USART_2, 17);break;
				case 3: EXO_Transmit(USART_2, 9);break;
				case 4: EXO_Transmit(USART_2, 10);break;
			}
			count++;
			if(count==5)count=0;
			*/
		}
		
		
	}
}

volatile unsigned int	onlyoneFlag;
volatile int EXO_Find_CommandChangeFlag[3];
extern volatile int gMessageSensorError;
extern volatile int gExoErrorCnt;
extern volatile int gExoDataReceiveErrorFlag;
void EXO_ComRequestValueInit(int UartSelect) { 
    gv.exoRxLength[UartSelect]=0;
    gv.exoBufferCount[UartSelect]=0;
    gv.nReceiveBufferCount[UartSelect]=0;
    gv.nFlag_ReceiveStart[UartSelect]=OFF;
    gv.nFlag_ReceiveDataProcess[UartSelect]=OFF;//여기서 클리어 하면 상태메시지 출력이 안된다.			
    for(int i=0;i<USART_BUFFER_MAXSIZE;i++) {
        gv.aItemReceiveBuffer[UartSelect][i]=0;	
    }
}
int	EXO_Receive_Process(unsigned int UartSelect)
{
	float mV=0;                                                                     
	U32 *address;
	//U8 a, b, c, d;
	void *addr1; 
	int onlyoneFlag=0;
	int	itemCount=0;
	unsigned char dataBuf[30];
    int i;
    int length=0;
	
	
    addr1 = &mV;
    address = addr1;

	if(gv.findDelayTimeCnt < 20000) {//1ms 마다 증가(타이머에서)
		return 0; //센서가 안정이 안된것으로 본다.
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//
	//		수신데이터를 처리한다.
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
	if(flash.EXO.CalStart[USART_1]==ON || flash.EXO.CalStart[USART_2]==ON  || flash.EXO.CalStart[USART_3]==ON) { //교정중에도 다른 센서 측정값은 요청한다.
		if(flash.EXO.CalStart[UartSelect] == OFF) {
			EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); 
			return 0;
		}
	}
	//세척명령
	if(flash.EXO.WiperEnableTimeSV > 0)//0보다 커야지만 동작을한다.
	{
		gv.wiperWaitCountmSec++;
		if(gv.wiperWaitCountmSec>=2) {//1sec 만들기 / EXO_IOPrepareEvent(500); 500이 변경되면 수정 되어야 한다.
			gv.wiperEnableTimePV++;//세척 대기시간이 증가한다.
			gv.wiperWaitCountSec++;//세척 진행시간이 증가한다.
			gv.wiperWaitCountmSec=0;
		}
	}
	if(flash.EXO.WiperEnableTimeSV < gv.wiperEnableTimePV)
	{
		gv.wiperEnableTimePV=0;
		EXO_ComRequestValueInit(UartSelect);
		if(flash.Uart[USART_1]->SensorID == SENSOR_EXO) EXO_Calibration_Command(USART_1, 40, SONDE_ID, 0x30, 0x00, 0x01, 0xc9, 0x00, 0x00, NULL);
		if(flash.Uart[USART_2]->SensorID == SENSOR_EXO) EXO_Calibration_Command(USART_2, 40, SONDE_ID, 0x30, 0x00, 0x01, 0xc9, 0x00, 0x00, NULL);
		if(flash.Uart[USART_3]->SensorID == SENSOR_EXO) EXO_Calibration_Command(USART_3, 40, SONDE_ID, 0x30, 0x00, 0x01, 0xc9, 0x00, 0x00, NULL);
		gv.wiperEnableFlag=ON;	//세척진행
		gv.wiperWaitCountSec=0; 	//세척진행시간초기화
		Dprintf("Wiper Control Start\n");
		DebugMessagePrintf("Wiper Control Start\n");
		return 0;
	}
	if(gv.wiperEnableFlag==ON)	//Wiper 가 동작중에는 데이터를 수신하지 않는다.
	{					
		gv.wiperEnableTimePV=0;	//동작중에는 대기 시간이 초기화 된다.
		if(gv.wiperWaitCountSec >= 30)//30초전에 동작이 완료된다.
		{
			Dprintf("Wiper Control End\n");
			DebugMessagePrintf("Wiper Control End\n");
			gv.wiperEnableFlag=OFF;//자동세정종료
			gv.wiperWaitCountSec=0;//동작을 완료하면 시간을 초기화한다.
		} else {
			//세척 중 에는 Depth 값만 업데이트 한다.
		}            
	}


	if(gv.nFlag_ReceiveDataProcess[UartSelect]==1) {
		gv.nFlag_ReceiveDataProcess[UartSelect]=0;
        //Dprintf("EXO USART_%d OK\n",UartSelect+1);
        gv.serialComStatusToggle[UartSelect] = 1;   //화면 좌측상단 LED ON/OFF 용
		gv.comportConnectionError[UartSelect] = 0;  //데이터 정상 수신시 통신에러 타이머를 초기화 한다.
		itemCount=0;//수신한 측정치값에 카운트를 적용한다.
        if(gv.exoBufferCount[UartSelect] != gv.nReceiveBufferCount[UartSelect]) {
            Dprintf("EXO UART[%d] 길이에러  %d, %d\n",UartSelect ,gv.exoBufferCount[UartSelect], gv.nReceiveBufferCount[UartSelect]);
            EXO_ComRequestValueInit(UartSelect);
            return 0;
        }       
        if(gv.exoBufferCount[UartSelect] >= (USART_BUFFER_MAXSIZE-10)) {
            Dprintf("EXO UART[%d] OVERFLOW = %d\n",gv.exoBufferCount[UartSelect]);
            EXO_ComRequestValueInit(UartSelect);
            return 0;
        }
        length = gv.exoBufferCount[UartSelect];//gv.exoBufferCount 값이 변경될 수 있다.
        //Dprintf("%d[%02x]",length,gv.aItemReceiveBuffer[UartSelect][13]);
        for(i=0; i<length; i++)//수신데이터처리
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
			//	모든 측정값을 요청하면 1바이트를 쉬프트하여 처리한다.
			//	1. 0x7E 명령을 처리하지 않는다.
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if(gv.aItemReceiveBuffer[UartSelect][13]==0x22)//13번째 버퍼에 13이들어오면 모든데이터가 올라온것으로본다.
			{
				if(onlyoneFlag==0)
				{
					onlyoneFlag=1;
					itemCount=0;
					i++;
				}
				mV=0.0f;
				*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 24); 	i++;
				*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 16);	i++; 
				*address |= (gv.aItemReceiveBuffer[UartSelect][i] << 8); 	i++;
				*address |= gv.aItemReceiveBuffer[UartSelect][i];
				
				if(mV < -10000.0f || mV >1000000)//범위내의 데이터만 사용한다.
				{
					flash.EXO.EXO_Data[UartSelect][itemCount]=0.0f;
				}
				else
				{
					flash.EXO.EXO_Data[UartSelect][itemCount]=mV; //정상적인 값을 전역버퍼에 저장한다.
				}
/*
1. 모든데이터를 출력한다.
2. 이것을 참고하여 데이터 출력 순서를 결정한다.				
*/
//Dprintf("[%02d][%02d] % 10.3f\n",UartSelect,itemCount, flash.EXO.EXO_Data[UartSelect][itemCount]);
				itemCount++; 
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
			//	제어 관련 명령인 경우
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if(	  	gv.aItemReceiveBuffer[UartSelect][13]==0x15 // ROUTE TABLE ENTRY( 센서정보 )
						||	gv.aItemReceiveBuffer[UartSelect][13]==0x17 // 해당원시데이터값
						||	gv.aItemReceiveBuffer[UartSelect][13]==0x09 // 보정유형
						||	gv.aItemReceiveBuffer[UartSelect][13]==0x05 // 보정계수
						||	gv.aItemReceiveBuffer[UartSelect][13]==0x04 // 보정시간
						||	gv.aItemReceiveBuffer[UartSelect][13]==0x03 // 보정시작명령
						||	gv.aItemReceiveBuffer[UartSelect][13]==0x02 // 보정버퍼설정
						||	gv.aItemReceiveBuffer[UartSelect][13]==0x12 // 보정버퍼설정
						||  gv.aItemReceiveBuffer[UartSelect][15]==0x26 // SONDE 주소
						||	gv.aItemReceiveBuffer[UartSelect][13]==0x01 // 포트정보
						||  gv.aItemReceiveBuffer[UartSelect][12]==0x0E 
			)
			{
				if(gv.aItemReceiveBuffer[UartSelect][2]==0x99&&gv.aItemReceiveBuffer[UartSelect][3]==0x99&&gv.aItemReceiveBuffer[UartSelect][4]==0x99&&gv.aItemReceiveBuffer[UartSelect][5]==0x99&&gv.aItemReceiveBuffer[UartSelect][15]==0x26)//SONDE ADDRESS
				{
					flash.EXO.EXO_Address[UartSelect][0]=gv.aItemReceiveBuffer[UartSelect][42];
					flash.EXO.EXO_Address[UartSelect][1]=gv.aItemReceiveBuffer[UartSelect][43];
					flash.EXO.EXO_Address[UartSelect][2]=gv.aItemReceiveBuffer[UartSelect][44];
					flash.EXO.EXO_Address[UartSelect][3]=gv.aItemReceiveBuffer[UartSelect][45];
					//Dprintf("SONDE ADDRESS = %02x %02x %02x %02x\n",gv.aItemReceiveBuffer[UartSelect][42],gv.aItemReceiveBuffer[UartSelect][43],gv.aItemReceiveBuffer[UartSelect][44],gv.aItemReceiveBuffer[UartSelect][45]);
					break;
				}
				else if(gv.aItemReceiveBuffer[UartSelect][12]==0x0E)
				{
					mV=0.0f;
					*address |= (gv.aItemReceiveBuffer[UartSelect][16] << 24);
					*address |= (gv.aItemReceiveBuffer[UartSelect][17] << 16);
					*address |= (gv.aItemReceiveBuffer[UartSelect][18] << 8);
					*address |= gv.aItemReceiveBuffer[UartSelect][19];
					flash.EXO.CalTemp[UartSelect]=mV;//전도도 측정치
					//Dprintf("CalTemp  = %f\n",mV);
					break;
				}
                /**************************************************************************************
                *
                *   포트정보
                *
                ***************************************************************************************/
				else if(gv.aItemReceiveBuffer[UartSelect][13]==0x01)
				{
                    //Dprintf("포트정보 = %d",gv.aItemReceiveBuffer[UartSelect][19]);
					if(gv.aItemReceiveBuffer[UartSelect][19]==0x01)
					{
                        if(flash.EXO.Sensor_PORT[UartSelect]==5) {
                            Dprintf("PORT[%d] Ammoninum\n",flash.EXO.Sensor_PORT[UartSelect]);
                            flash.EXO.Sensor_Ammoninum[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
                        } else {
                            Dprintf("PORT[%d] PH\n",flash.EXO.Sensor_PORT[UartSelect]);
                            //if(flash.EXO.Sensor_PH_Status[UartSelect][0]==ON||flash.EXO.Sensor_PHMV_Status[UartSelect][0]==ON)
                            flash.EXO.Sensor_PH[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
                        }
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x02)
					{
						Dprintf("PORT[%d] ODO\n",flash.EXO.Sensor_PORT[UartSelect]);
						//if( flash.EXO.Sensor_ODO_SAT_Status[UartSelect][0]==ON || flash.EXO.Sensor_ODO_MGL_Status[UartSelect][0]==ON)
						flash.EXO.Sensor_ODO[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x03)
					{
						Dprintf("PORT[%d] COND\n",flash.EXO.Sensor_PORT[UartSelect]);
						//if(flash.EXO.Sensor_COND_TEMP_Status[UartSelect][0]==ON||flash.EXO.Sensor_COND_SP_Status[UartSelect][0]==ON||flash.EXO.Sensor_COND_SAL_Status[UartSelect][0]==ON)
						flash.EXO.Sensor_COND[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x04)
					{
						Dprintf("PORT[%d] TUB\n",flash.EXO.Sensor_PORT[UartSelect]);
						//if(flash.EXO.Sensor_TUB_FNU_Status[UartSelect][0]==ON||flash.EXO.Sensor_TUB_NTU_Status[UartSelect][0]==ON)
						flash.EXO.Sensor_TUB[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x05)
					{
						Dprintf("PORT[%d] SOND\n",flash.EXO.Sensor_PORT[UartSelect]);
						flash.EXO.Sensor_SOND[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x06)
					{
						Dprintf("PORT[%d] CHLIB\n",flash.EXO.Sensor_PORT[UartSelect]);
						/*
						if(	flash.EXO.Sensor_CHLIB_UGL_Status[UartSelect][0]==ON||
							flash.EXO.Sensor_CHLIB_RFU_Status[UartSelect][0]==ON||
							flash.EXO.Sensor_BGA_PCRFU_Status[UartSelect][0]==ON||
							flash.EXO.Sensor_BGA_PCUGL_Status[UartSelect][0]==ON||
							flash.EXO.Sensor_BGA_PERFU_Status[UartSelect][0]==ON||
							flash.EXO.Sensor_BGA_PEUGL_Status[UartSelect][0]==ON
						)
						*/
						flash.EXO.Sensor_CHLIB[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x07)
					{
						Dprintf("PORT[%d] DEPTH\n",flash.EXO.Sensor_PORT[UartSelect]);
						//if(flash.EXO.Sensor_DEPTH_Status[UartSelect][0]==ON)
						flash.EXO.Sensor_DEPTH[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x08)
					{
						Dprintf("PORT[%d] WIPER\n",flash.EXO.Sensor_PORT[UartSelect]);
						flash.EXO.Sensor_WIPER[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
						break;	
					}
				   else if(gv.aItemReceiveBuffer[UartSelect][19]==0x09)
					{
						Dprintf("PORT[%d] NO\n",flash.EXO.Sensor_PORT[UartSelect]);
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x0a)
					{
						Dprintf("PORT[%d] NO\n",flash.EXO.Sensor_PORT);
						break;	
					}
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x0b)
					{
						Dprintf("PORT[%d] FDOM\n",flash.EXO.Sensor_PORT[UartSelect]);
						//if(flash.EXO.Sensor_FDOM_RFU_Status[UartSelect][0]==ON||flash.EXO.Sensor_FDOM_QSU_Status[UartSelect][0]==ON)
						flash.EXO.Sensor_FDOM[UartSelect][0]=flash.EXO.Sensor_PORT[UartSelect];
						break;	
					} 
					else if(gv.aItemReceiveBuffer[UartSelect][19]==0x14)
					{
						Dprintf("PORT[%d] NitraLED\n",flash.EXO.Sensor_PORT[UartSelect]);
						break;
					}
				}
				else if(gv.aItemReceiveBuffer[UartSelect][13]==0x15)// SONDE 에 연결된 센서 주소와 포트 번호를 읽어온다.
				{
					if(gv.aItemReceiveBuffer[UartSelect][16]==0 && gv.aItemReceiveBuffer[UartSelect][17]==0 && gv.aItemReceiveBuffer[UartSelect][18]==0)
                    {
                        //Dprintf("ERROR\n");
                    }
                    else
                    {
                        Dprintf("ADDR[%d] = ",gv.aItemReceiveBuffer[UartSelect][24]);
                        Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][16]);
                        Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][17]);
                        Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][18]);
                        Dprintf("%02x ",gv.aItemReceiveBuffer[UartSelect][19]);
                        switch(gv.aItemReceiveBuffer[UartSelect][24]) //포트ID
                        {
                            case 0x1: 
                                        flash.EXO.SensorAddress[UartSelect][0][0]=gv.aItemReceiveBuffer[UartSelect][16];
                                        flash.EXO.SensorAddress[UartSelect][0][1]=gv.aItemReceiveBuffer[UartSelect][17];
                                        flash.EXO.SensorAddress[UartSelect][0][2]=gv.aItemReceiveBuffer[UartSelect][18];
                                        flash.EXO.SensorAddress[UartSelect][0][3]=gv.aItemReceiveBuffer[UartSelect][19];
                            break;
                            case 0x2: 
                                        flash.EXO.SensorAddress[UartSelect][1][0]=gv.aItemReceiveBuffer[UartSelect][16];
                                        flash.EXO.SensorAddress[UartSelect][1][1]=gv.aItemReceiveBuffer[UartSelect][17];
                                        flash.EXO.SensorAddress[UartSelect][1][2]=gv.aItemReceiveBuffer[UartSelect][18];
                                        flash.EXO.SensorAddress[UartSelect][1][3]=gv.aItemReceiveBuffer[UartSelect][19];
                            break;
                            case 0x3: 
                                        flash.EXO.SensorAddress[UartSelect][2][0]=gv.aItemReceiveBuffer[UartSelect][16];
                                        flash.EXO.SensorAddress[UartSelect][2][1]=gv.aItemReceiveBuffer[UartSelect][17];
                                        flash.EXO.SensorAddress[UartSelect][2][2]=gv.aItemReceiveBuffer[UartSelect][18];
                                        flash.EXO.SensorAddress[UartSelect][2][3]=gv.aItemReceiveBuffer[UartSelect][19];
                            break;
                            case 0x4: 
                                        flash.EXO.SensorAddress[UartSelect][3][0]=gv.aItemReceiveBuffer[UartSelect][16];
                                        flash.EXO.SensorAddress[UartSelect][3][1]=gv.aItemReceiveBuffer[UartSelect][17];
                                        flash.EXO.SensorAddress[UartSelect][3][2]=gv.aItemReceiveBuffer[UartSelect][18];
                                        flash.EXO.SensorAddress[UartSelect][3][3]=gv.aItemReceiveBuffer[UartSelect][19];
                            break;
                            case 0x5: 
                                        flash.EXO.SensorAddress[UartSelect][4][0]=gv.aItemReceiveBuffer[UartSelect][16];
                                        flash.EXO.SensorAddress[UartSelect][4][1]=gv.aItemReceiveBuffer[UartSelect][17];
                                        flash.EXO.SensorAddress[UartSelect][4][2]=gv.aItemReceiveBuffer[UartSelect][18];
                                        flash.EXO.SensorAddress[UartSelect][4][3]=gv.aItemReceiveBuffer[UartSelect][19];
                            break;
                            case 0x6: 
                                        flash.EXO.SensorAddress[UartSelect][5][0]=gv.aItemReceiveBuffer[UartSelect][16];
                                        flash.EXO.SensorAddress[UartSelect][5][1]=gv.aItemReceiveBuffer[UartSelect][17];
                                        flash.EXO.SensorAddress[UartSelect][5][2]=gv.aItemReceiveBuffer[UartSelect][18];
                                        flash.EXO.SensorAddress[UartSelect][5][3]=gv.aItemReceiveBuffer[UartSelect][19];
                            break;
                            case 0x7: 
                                        flash.EXO.SensorAddress[UartSelect][6][0]=gv.aItemReceiveBuffer[UartSelect][16];
                                        flash.EXO.SensorAddress[UartSelect][6][1]=gv.aItemReceiveBuffer[UartSelect][17];
                                        flash.EXO.SensorAddress[UartSelect][6][2]=gv.aItemReceiveBuffer[UartSelect][18];
                                        flash.EXO.SensorAddress[UartSelect][6][3]=gv.aItemReceiveBuffer[UartSelect][19];
                            break;
                            case 0xc:
                                        flash.EXO.SensorAddress[UartSelect][7][0]=gv.aItemReceiveBuffer[UartSelect][16];
                                        flash.EXO.SensorAddress[UartSelect][7][1]=gv.aItemReceiveBuffer[UartSelect][17];
                                        flash.EXO.SensorAddress[UartSelect][7][2]=gv.aItemReceiveBuffer[UartSelect][18];
                                        flash.EXO.SensorAddress[UartSelect][7][3]=gv.aItemReceiveBuffer[UartSelect][19];
                            break;
                            default : Dprintf("추가된 주소값 %02x \n",gv.aItemReceiveBuffer[UartSelect][24]);
                                break;
                        }
                    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
//		데이터를 출력한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    SmartLogExo_ItemDisplayProcess(UartSelect);//수신한 데이터를 화면에 출력한다.
                    break;							
				}
				else if(gv.aItemReceiveBuffer[UartSelect][13]==0x12)//PANDING
				{
					mV=0.0f;
					*address |= (gv.aItemReceiveBuffer[UartSelect][16] << 24); 	
					*address |= (gv.aItemReceiveBuffer[UartSelect][17] << 16);		 
					*address |= (gv.aItemReceiveBuffer[UartSelect][18] << 8); 		
					*address |= gv.aItemReceiveBuffer[UartSelect][19];
					flash.EXO.SV[UartSelect]=mV;
					Dprintf("PENDING[CH=%d]  = %f\n",UartSelect,mV);
					break;
				}
				else if(gv.aItemReceiveBuffer[UartSelect][13]==0x04)// 보정시작시간
				{
					gv.EXOTimeCode[0]=gv.aItemReceiveBuffer[UartSelect][16];
					gv.EXOTimeCode[1]=gv.aItemReceiveBuffer[UartSelect][17];
					gv.EXOTimeCode[2]=gv.aItemReceiveBuffer[UartSelect][18];
					gv.EXOTimeCode[3]=gv.aItemReceiveBuffer[UartSelect][19];							
					//Dprintf("CALIBRATION TIME CODE = %02x %02x %02x %02x \n",gv.EXOTimeCode[0],gv.EXOTimeCode[1],gv.EXOTimeCode[2],gv.EXOTimeCode[3]);
					break;
				}
				else if(gv.aItemReceiveBuffer[UartSelect][13]==0x03)// 보정시작
				{
					Dprintf("CALIBRATION START \n");
					break;
				}
				else if(gv.aItemReceiveBuffer[UartSelect][13]==0x02)// 보정버퍼설정
				{
					mV=0.0f;
					*address |= (gv.aItemReceiveBuffer[UartSelect][16] << 24); 	
					*address |= (gv.aItemReceiveBuffer[UartSelect][17] << 16);		 
					*address |= (gv.aItemReceiveBuffer[UartSelect][18] << 8); 		
					*address |= gv.aItemReceiveBuffer[UartSelect][19];
					Dprintf("BUFFER SET %7.2f\n",mV);
					mV=0.0f;
					if(gv.aItemReceiveBuffer[UartSelect][15]==0x08)
					{
						*address |= (gv.aItemReceiveBuffer[UartSelect][20] << 24); 	
						*address |= (gv.aItemReceiveBuffer[UartSelect][21] << 16);		 
						*address |= (gv.aItemReceiveBuffer[UartSelect][22] << 8); 		
						*address |= gv.aItemReceiveBuffer[UartSelect][23];
						//Dprintf("TEMP SET %7.2f\n",mV);
					}
					break;
				}
				else if(gv.aItemReceiveBuffer[UartSelect][13]==0x14)// 보정완료
				{
					Dprintf("CALIBRATION END\n");
					break;
				}
				else if(gv.aItemReceiveBuffer[UartSelect][13]==0x05)//DO Barometer mmHg
				{
					if(flash.EXO.CalHexaItemCode[UartSelect]==0x09)
					{
						mV=0.0f;
						*address = (gv.aItemReceiveBuffer[UartSelect][24]<< 24) | (gv.aItemReceiveBuffer[UartSelect][25] << 16) | (gv.aItemReceiveBuffer[UartSelect][26] << 8) | gv.aItemReceiveBuffer[UartSelect][27];  //1.2345를 분해한값    
						//mV =760.0;
						Dprintf("Barometer[CH=%d] = %f\n",UartSelect, mV);
						gv.BarometerPV[UartSelect]=mV;
					}
					break;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
			//		상태값이 리턴되면 HEXA 값으로 출력한다.
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else 
			{
//Dprintf("%02x[%d][%02x] ",gv.aItemReceiveBuffer[UartSelect][i],i,gv.aItemReceiveBuffer[UartSelect][13]);
                //Dprintf("EXO COMMAND ERROR\n");
                break;
			}
//Dputc('_');
		}		
		if(itemCount>0)
		{
			flash.EXO.DataListCount[UartSelect]=itemCount;//최대출력크기를 알아본다.
            //Dprintf("EXO itemCount = %d\n", itemCount);
//Dputc('D');
		SmartLogExo_ItemDisplayProcess(UartSelect);
//Dputc('O');
		}
        EXO_ComRequestValueInit(UartSelect);
//Dputc('E');
//Dputc(0x0a);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////					
	//		보정,  측정치요청 에러처리 등을 일을한다.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
	else if(gv.nFlag_ReceiveDataProcess[UartSelect]==0) {
		for(i=0;i<30;i++) dataBuf[i]=0;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		//
		//
		//
		//
		//		측정치를 화면에 출력한다.
		//
		//
		//
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//Dprintf("flash.EXO.UartPort = %d\n",flash.EXO.UartPort);
//        Dputc('D');
//		SmartLogExo_ItemDisplayProcess(UartSelect);
//        Dputc('O');
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		보정시작이 되면 해당 커멘드를 송신하기 시작한다.
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(flash.EXO.CalStart[flash.EXO.UartPort]==ON)
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//								보정 시작 스텝
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if(flash.EXO.CalStartStep[flash.EXO.UartPort]==1)
			{
				//Dprintf("flash.EXO.CalCommand[%02x] = %d\n",flash.EXO.CalHexaItemCode ,flash.EXO.CalCommand);
				//보정 진행 중에 항상 콜한다.
				switch( flash.EXO.CalCommand)
				{
					case 0: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, dataBuf); 
					}break;//STATUS
					case 1: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x04, 0x00, 0x00, dataBuf);
					}break;//TIME
					case 2: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x03, 0x00, 0x00, dataBuf);
					}
					break;//START CAL
					//case 4: 		EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode, 0x16, 0x00, 0x00, dataBuf);break;
					case 4: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, dataBuf);
					}break;//ALL
					case 5: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x12, 0x00, 0x00, dataBuf);
					}break;//PENDING
					case 6: 
					{
						if(flash.EXO.CalHexaItemCode[UartSelect]==0x09)EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x05, 0x00, 0x00, dataBuf);
					}break;//ODO 인경우에만 바로메터 값이 적용된다.
					case 7: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x17, 0x00, 0x00, dataBuf);		
					}break;
					case 8: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x13, 0x00, 0x00, dataBuf);
					}break;
					
					//case 10:	 EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode, 0x15, 0x00, 0x00, dataBuf);		break;
					//case 11:	 EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode, 0x10, 0x00, 0x00, dataBuf);		break;
					//case 12:	 dataBuf[0]=0x00;EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, 0x00, 0x23, 0x00, 0x01, dataBuf);		break;
				}
				flash.EXO.CalCommand++;
				if(flash.EXO.CalCommand >= 9) //반복적으로 값을 요청한다.
				{
					flash.EXO.CalCommand=5;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//								1차 보정 시작
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if(flash.EXO.CalStartStep[flash.EXO.UartPort]==2)
			{
				switch( flash.EXO.CalCommand)
				{
					case 0: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, 0x0E, 0x00, 0x00, 0x00, dataBuf); //온도값 
					}break;
					case 1: 
					{
						if(flash.EXO.CalHexaItemCode[UartSelect]==0x03 || flash.EXO.CalHexaItemCode[UartSelect]==0x09  || flash.EXO.CalHexaItemCode[UartSelect]==0x0A)
						{
							dataBuf[0]=0x01;
						}
						else 
						{
							dataBuf[0]=0x00;
						}
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x07, 0x00, 0x01, dataBuf); //보정버퍼선택
					}break;
                    /*
                        1차보정값적용
                    */
					case 2:
					{
						//온도를 사용하지 않는 모델
						//0x18 전도도  0x24 뎁스 0x15 탁도 0x06 ORP 0x13 SAL
						if(flash.EXO.CalHexaItemCode[UartSelect]==0x18 || flash.EXO.CalHexaItemCode[UartSelect]==0x24 || flash.EXO.CalHexaItemCode[UartSelect]==0x15  || flash.EXO.CalHexaItemCode[UartSelect]==0x06 || flash.EXO.CalHexaItemCode[UartSelect]==0x13) 
						{
							mV=flash.EXO.ECCalbration_Buffer[0];//첫번째 보정 버퍼값
							//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x04, dataBuf);
						}
						else if(flash.EXO.CalHexaItemCode[UartSelect]==0x03)//pH보정
						{
							if(flash.EXO.ECCalbration_Buffer[0] >0.0f)
							{
								mV=flash.EXO.ECCalbration_Buffer[0];//첫번째 보정 버퍼값
								Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[0]= (*address>>24)&0xFF;     
								dataBuf[1]= (*address>>16)&0xFF; 
								dataBuf[2]= (*address>>8)&0xFF;  
								dataBuf[3]= *address & 0xFF;        				
								mV=flash.EXO.CalTemp[UartSelect];//온도값
								Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[4]= (*address>>24)&0xFF;     
								dataBuf[5]= (*address>>16)&0xFF; 
								dataBuf[6]= (*address>>8)&0xFF;  
								dataBuf[7]= *address & 0xFF;        				
								EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x08, dataBuf);
							}
							/*
							else if(flash.EXO.ECCalbration_Buffer[0] >0.0f && flash.EXO.ECCalbration_Buffer[1] > 0.0f && flash.EXO.ECCalbration_Buffer[2] > 0.0f)
							{
								mV=flash.EXO.ECCalbration_Buffer[0];//첫번째 보정 버퍼값
								Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[0]= (*address>>24)&0xFF;     
								dataBuf[1]= (*address>>16)&0xFF; 
								dataBuf[2]= (*address>>8)&0xFF;  
								dataBuf[3]= *address & 0xFF;        				
								mV=flash.EXO.ECCalbration_Buffer[1];//첫번째 보정 버퍼값
								Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[4]= (*address>>24)&0xFF;     
								dataBuf[5]= (*address>>16)&0xFF; 
								dataBuf[6]= (*address>>8)&0xFF;  
								dataBuf[7]= *address & 0xFF;        				
								mV=flash.EXO.ECCalbration_Buffer[2];//첫번째 보정 버퍼값
								Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[8]= (*address>>24)&0xFF;     
								dataBuf[9]= (*address>>16)&0xFF; 
								dataBuf[10]= (*address>>8)&0xFF;  
								dataBuf[11]= *address & 0xFF;        				
								EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x0C, dataBuf);
							}
							*/
						}
                        else if(flash.EXO.CalHexaItemCode[UartSelect]==0x44)//NH4+
                        {
                            mV=flash.EXO.ECCalbration_Buffer[0];//첫번째 보정 버퍼값
                            Dprintf("1P BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
                            dataBuf[0]= (*address>>24)&0xFF;     
                            dataBuf[1]= (*address>>16)&0xFF; 
                            dataBuf[2]= (*address>>8)&0xFF;  
                            dataBuf[3]= *address & 0xFF;        				
                            mV=flash.EXO.CalTemp[UartSelect];//온도값
                            Dprintf("온도[%d] = %10.3f\n",UartSelect,mV);
                            dataBuf[4]= (*address>>24)&0xFF;     
                            dataBuf[5]= (*address>>16)&0xFF; 
                            dataBuf[6]= (*address>>8)&0xFF;  
                            dataBuf[7]= *address & 0xFF;        				
                            mV=flash.EXO.CalSaltPV[UartSelect];//염분값
                            Dprintf("염분값[%d] = %10.3f\n",UartSelect,mV);
                            dataBuf[8]= (*address>>24)&0xFF;     
                            dataBuf[9]= (*address>>16)&0xFF; 
                            dataBuf[10]= (*address>>8)&0xFF;  
                            dataBuf[11]= *address & 0xFF;        				
                            EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x0C, dataBuf);
                        }
						else if(flash.EXO.CalHexaItemCode[UartSelect]==0x09)//ODO Sat보정의 경우 3가지값이 설정된다.
						{
							mV=flash.EXO.ECCalbration_Buffer[0];
							Dprintf("BUFFER1[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;        				
							mV= flash.EXO.CalTemp[UartSelect];//온도값 
							Dprintf("TEMP[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[4]= (*address>>24)&0xFF;     
							dataBuf[5]= (*address>>16)&0xFF; 
							dataBuf[6]= (*address>>8)&0xFF;  
							dataBuf[7]= *address & 0xFF;        				
							mV=gv.BarometerPV[UartSelect];
							Dprintf("gv.BarometerPV = %10.3f\n",mV);
							dataBuf[8]= (*address>>24)&0xFF;     
							dataBuf[9]= (*address>>16)&0xFF; 
							dataBuf[10]= (*address>>8)&0xFF;  
							dataBuf[11]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x0C, dataBuf);
						}
						else if(flash.EXO.CalHexaItemCode[UartSelect]==0x0A)//ODO mg/L보정의 경우 3가지값이 설정된다.
						{
							mV=flash.EXO.ECCalbration_Buffer[0];
							//Dprintf("BUFFER1[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;        				
							mV= flash.EXO.CalTemp[UartSelect];//온도값 
							//Dprintf("TEMP[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[4]= (*address>>24)&0xFF;     
							dataBuf[5]= (*address>>16)&0xFF; 
							dataBuf[6]= (*address>>8)&0xFF;  
							dataBuf[7]= *address & 0xFF;        				
							mV= flash.EXO.CalSaltPV[UartSelect];//SALT 값
							//Dprintf("BUFFER2[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[8]= (*address>>24)&0xFF;     
							dataBuf[9]= (*address>>16)&0xFF; 
							dataBuf[10]= (*address>>8)&0xFF;  
							dataBuf[11]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x0C, dataBuf);
						}                        
						//온도를 사용하는 모델
						else
						{
							mV=flash.EXO.ECCalbration_Buffer[0];//첫번째 버퍼값
							//Dprintf("COMMON BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;   
							
							mV= flash.EXO.CalTemp[UartSelect];//온도값 
							//Dprintf("COMMON TEMP[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[4]= (*address>>24)&0xFF;     
							dataBuf[5]= (*address>>16)&0xFF; 
							dataBuf[6]= (*address>>8)&0xFF;  
							dataBuf[7]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x08, dataBuf);
						}
					}break;
					//순환루틴
					case 3: EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, dataBuf);break;//ALL
					case 4: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x12, 0x00, 0x00, dataBuf);     break;//PENDING
					case 5: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x17, 0x00, 0x00, dataBuf);		break;
					case 6: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x13, 0x00, 0x00, dataBuf);		break;
				}
				//Dprintf("flash.EXO.CalHexaItemCode = 0x%02x\n",flash.EXO.CalHexaItemCode);
				flash.EXO.CalCommand++;
				if(flash.EXO.CalCommand >= 7) //반복적으로 값을 요청한다.
				{
					flash.EXO.CalCommand=3;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//								2차 보정 시작
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if(flash.EXO.CalStartStep[flash.EXO.UartPort]==3)//2점 보정버퍼값을 송신한다.		
			{
				switch( flash.EXO.CalCommand)
				{
					case 0: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, 0x0E, 0x00, 0x00, 0x00, dataBuf); //온도값 
					}break;
					case 1: //보정버퍼설정
					{
						if(flash.EXO.CalHexaItemCode[UartSelect]==0x03 || flash.EXO.CalHexaItemCode[UartSelect]==0x09)                                                                                                                                                                             
						{
							dataBuf[0]=0x02;
						}                                                                                                                                                                                                                   
						else dataBuf[0]=0x01;
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x07, 0x00, 0x01, dataBuf);//보정버퍼선택         
					}break;
					case 2:
					{
						//온도를 사용하지 않는 모델
						if(flash.EXO.CalHexaItemCode[UartSelect]==0x18 || flash.EXO.CalHexaItemCode[UartSelect]==0x24 || flash.EXO.CalHexaItemCode[UartSelect]==0x15) 
						{
							mV=flash.EXO.ECCalbration_Buffer[1];//첫번째 보정 버퍼값
							//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);                    
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x04, dataBuf);
						}
						else if(flash.EXO.CalHexaItemCode[UartSelect]==0x03)//pH보정
						{
							if(flash.EXO.ECCalbration_Buffer[1] > 0.0f)
							{
								mV=flash.EXO.ECCalbration_Buffer[1];//첫번째 보정 버퍼값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[0]= (*address>>24)&0xFF;     
								dataBuf[1]= (*address>>16)&0xFF; 
								dataBuf[2]= (*address>>8)&0xFF;  
								dataBuf[3]= *address & 0xFF;        				
								mV=flash.EXO.CalTemp[UartSelect];//온도값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[4]= (*address>>24)&0xFF;     
								dataBuf[5]= (*address>>16)&0xFF; 
								dataBuf[6]= (*address>>8)&0xFF;  
								dataBuf[7]= *address & 0xFF;        				
								EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x08, dataBuf);
							}
							/*
							else if(flash.EXO.ECCalbration_Buffer[0] >0.0f && flash.EXO.ECCalbration_Buffer[1] > 0.0f && flash.EXO.ECCalbration_Buffer[2] > 0.0f)
							{
								mV=flash.EXO.ECCalbration_Buffer[0];//첫번째 보정 버퍼값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[0]= (*address>>24)&0xFF;     
								dataBuf[1]= (*address>>16)&0xFF; 
								dataBuf[2]= (*address>>8)&0xFF;  
								dataBuf[3]= *address & 0xFF;        				
								mV=flash.EXO.ECCalbration_Buffer[1];//첫번째 보정 버퍼값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[4]= (*address>>24)&0xFF;     
								dataBuf[5]= (*address>>16)&0xFF; 
								dataBuf[6]= (*address>>8)&0xFF;  
								dataBuf[7]= *address & 0xFF;        				
								mV=flash.EXO.ECCalbration_Buffer[2];//첫번째 보정 버퍼값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[8]= (*address>>24)&0xFF;     
								dataBuf[9]= (*address>>16)&0xFF; 
								dataBuf[10]= (*address>>8)&0xFF;  
								dataBuf[11]= *address & 0xFF;        				
								EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x0C, dataBuf);
							}
							*/
						}
                        else if(flash.EXO.CalHexaItemCode[UartSelect]==0x44)//NH4+
                        {
                            mV=flash.EXO.ECCalbration_Buffer[1];//첫번째 보정 버퍼값
                            Dprintf("1P BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
                            dataBuf[0]= (*address>>24)&0xFF;     
                            dataBuf[1]= (*address>>16)&0xFF; 
                            dataBuf[2]= (*address>>8)&0xFF;  
                            dataBuf[3]= *address & 0xFF;        				
                            mV=flash.EXO.CalTemp[UartSelect];//온도값
                            Dprintf("온도[%d] = %10.3f\n",UartSelect,mV);
                            dataBuf[4]= (*address>>24)&0xFF;     
                            dataBuf[5]= (*address>>16)&0xFF; 
                            dataBuf[6]= (*address>>8)&0xFF;  
                            dataBuf[7]= *address & 0xFF;        				
                            mV=flash.EXO.CalSaltPV[UartSelect];//염분값
                            Dprintf("염분값[%d] = %10.3f\n",UartSelect,mV);
                            dataBuf[8]= (*address>>24)&0xFF;     
                            dataBuf[9]= (*address>>16)&0xFF; 
                            dataBuf[10]= (*address>>8)&0xFF;  
                            dataBuf[11]= *address & 0xFF;        				
                            EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x0C, dataBuf);
                        }                        
						else if(flash.EXO.CalHexaItemCode[UartSelect]==0x0A)//ODO mg/L보정의 경우 3가지값이 설정된다.
						{
							mV=flash.EXO.ECCalbration_Buffer[1];
							//Dprintf("BUFFER1[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;        				
							mV= flash.EXO.CalTemp[UartSelect];//온도값 
							//Dprintf("TEMP[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[4]= (*address>>24)&0xFF;     
							dataBuf[5]= (*address>>16)&0xFF; 
							dataBuf[6]= (*address>>8)&0xFF;  
							dataBuf[7]= *address & 0xFF;        				
							mV= flash.EXO.CalSaltPV[UartSelect];//SALT 값
							//Dprintf("BUFFER2[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[8]= (*address>>24)&0xFF;     
							dataBuf[9]= (*address>>16)&0xFF; 
							dataBuf[10]= (*address>>8)&0xFF;  
							dataBuf[11]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x0C, dataBuf);
						}						
						//온도를 사용하는 모델
						else
						{
							mV=flash.EXO.ECCalbration_Buffer[1];//첫번째 버퍼값                                          
							//Dprintf("COMMON BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;        				
							mV= flash.EXO.CalTemp[UartSelect];//온도값 
							//Dprintf("COMMON TEMP[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[4]= (*address>>24)&0xFF;     
							dataBuf[5]= (*address>>16)&0xFF; 
							dataBuf[6]= (*address>>8)&0xFF;  
							dataBuf[7]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x08, dataBuf);
						}
					}break;
					//순환루틴
					case 3: EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, dataBuf);break;//ALL
					case 4: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x12, 0x00, 0x00, dataBuf);break;//PENDING
					case 5: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x17, 0x00, 0x00, dataBuf);break;
					case 6: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x13, 0x00, 0x00, dataBuf);break;
				}
				//Dprintf("flash.EXO.CalHexaItemCode = 0x%02x\n",flash.EXO.CalHexaItemCode);
				flash.EXO.CalCommand++;
				if(flash.EXO.CalCommand >= 7) //반복적으로 값을 요청한다.
				{
					flash.EXO.CalCommand=3;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//								3차 보정 시작
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if(flash.EXO.CalStartStep[flash.EXO.UartPort]==4)//3점 보정버퍼값을 송신한다.		
			{
				switch( flash.EXO.CalCommand)
				{
					case 0: 
					{
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, 0x0E, 0x00, 0x00, 0x00, dataBuf); //온도값 
					}break;
					case 1: 
					{
						if(flash.EXO.CalHexaItemCode[UartSelect]==0x03)
						{
							dataBuf[0]=0x03;
						}
						else dataBuf[0]=0x02;
						EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x07, 0x00, 0x01, dataBuf);//보정버퍼선택
					}break;
					case 2:
					{
						//온도를 사용하지 않는 모델
						if(flash.EXO.CalHexaItemCode[UartSelect]==0x18 || flash.EXO.CalHexaItemCode[UartSelect]==0x24 || flash.EXO.CalHexaItemCode[UartSelect]==0x15) 
						{
							mV=flash.EXO.ECCalbration_Buffer[2];//첫번째 보정 버퍼값
							//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x04, dataBuf);
						}
						else if(flash.EXO.CalHexaItemCode[UartSelect]==0x03)//pH보정
						{
							if(flash.EXO.ECCalbration_Buffer[2] > 0.0f)
							{
								mV=flash.EXO.ECCalbration_Buffer[2];//첫번째 보정 버퍼값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[0]= (*address>>24)&0xFF;     
								dataBuf[1]= (*address>>16)&0xFF; 
								dataBuf[2]= (*address>>8)&0xFF;  
								dataBuf[3]= *address & 0xFF;        				
								mV=flash.EXO.CalTemp[UartSelect];//온도값 
								//Dprintf("TEMP[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[4]= (*address>>24)&0xFF;     
								dataBuf[5]= (*address>>16)&0xFF; 
								dataBuf[6]= (*address>>8)&0xFF;  
								dataBuf[7]= *address & 0xFF;        				
								EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x08, dataBuf);
							}
							/*
							else if(flash.EXO.ECCalbration_Buffer[0] >0.0f && flash.EXO.ECCalbration_Buffer[1] > 0.0f && flash.EXO.ECCalbration_Buffer[2] > 0.0f)
							{
								mV=flash.EXO.ECCalbration_Buffer[0];//첫번째 보정 버퍼값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[0]= (*address>>24)&0xFF;     
								dataBuf[1]= (*address>>16)&0xFF; 
								dataBuf[2]= (*address>>8)&0xFF;  
								dataBuf[3]= *address & 0xFF;        				
								mV=flash.EXO.ECCalbration_Buffer[1];//첫번째 보정 버퍼값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[4]= (*address>>24)&0xFF;     
								dataBuf[5]= (*address>>16)&0xFF; 
								dataBuf[6]= (*address>>8)&0xFF;  
								dataBuf[7]= *address & 0xFF;        				
								mV=flash.EXO.ECCalbration_Buffer[2];//첫번째 보정 버퍼값
								//Dprintf("BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
								dataBuf[8]= (*address>>24)&0xFF;     
								dataBuf[9]= (*address>>16)&0xFF; 
								dataBuf[10]= (*address>>8)&0xFF;  
								dataBuf[11]= *address & 0xFF;        				
								EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x0C, dataBuf);
							}
							*/
						}
						//온도를 사용하는 모델
						else
						{
							mV=flash.EXO.ECCalbration_Buffer[2];//첫번째 버퍼값
							//Dprintf("COMMON BUFFER[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[0]= (*address>>24)&0xFF;     
							dataBuf[1]= (*address>>16)&0xFF; 
							dataBuf[2]= (*address>>8)&0xFF;  
							dataBuf[3]= *address & 0xFF;        				
							mV= flash.EXO.CalTemp[UartSelect];//온도값 
							//Dprintf("COMMON TEMP[%d] = %10.3f\n",flash.EXO.CalPoint-1,mV);
							dataBuf[4]= (*address>>24)&0xFF;     
							dataBuf[5]= (*address>>16)&0xFF; 
							dataBuf[6]= (*address>>8)&0xFF;  
							dataBuf[7]= *address & 0xFF;        				
							EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x02, 0x00, 0x08, dataBuf);
						}
					}break;
					//순환루틴 
					case 3: EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, dataBuf);break;//ALL
					case 4: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x12, 0x00, 0x00, dataBuf);break;//PENDING
					case 5: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x17, 0x00, 0x00, dataBuf);break;
					case 6: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x13, 0x00, 0x00, dataBuf);break;
				}
				//Dprintf("flash.EXO.CalHexaItemCode = 0x%02x\n",flash.EXO.CalHexaItemCode);
				flash.EXO.CalCommand++;
				if(flash.EXO.CalCommand >= 7) //반복적으로 값을 요청한다.
				{
					flash.EXO.CalCommand=3;
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//								보정 완료 스텝
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			else if(flash.EXO.CalStartStep[flash.EXO.UartPort]==5)	
			{
				if(flash.EXO.CalHexaItemCode[UartSelect]==0x09)//ODO 보정전용
				{
					if(flash.EXO.CalCommand==5)
					{
						dataBuf[0]=gv.EXOTimeCode[0];
						dataBuf[1]=gv.EXOTimeCode[1];
						dataBuf[2]=gv.EXOTimeCode[2];
						dataBuf[3]=gv.EXOTimeCode[3];
						dataBuf[4]=0;
						//Dprintf("CALIBRATION END TIME CODE = %02x %02x %02x %02x \n",gv.EXOTimeCode[0],gv.EXOTimeCode[1],gv.EXOTimeCode[2],gv.EXOTimeCode[3]);
					}
					switch(flash.EXO.CalCommand)
					{
						case 0: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x0D, 0x00, 0x00, dataBuf);		break;
						case 1: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x09, 0x00, 0x00, dataBuf);		break;
						case 2: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x12, 0x00, 0x00, dataBuf);		break;
						case 3: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x17, 0x00, 0x00, dataBuf);		break;
						case 4: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x13, 0x00, 0x00, dataBuf);		break;
						case 5: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x04, 0x00, 0x04, dataBuf);		break;
						case 6: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x05, 0x00, 0x00, dataBuf);		break;
						case 7: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x14, 0x00, 0x00, dataBuf);		break;
						case 8: EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, dataBuf);break;//ALL
					}
					flash.EXO.CalCommand++;
					if(flash.EXO.CalCommand>=9)
					{
						flash.EXO.CalStartStep[flash.EXO.UartPort]=0;
						flash.EXO.CalCommand=0;
						flash.EXO.CalStart[flash.EXO.UartPort]=4;//보정을 완료한다. 
						//SetWidgetColor(EXO_CalApplyID,clBlue, clWhite);	
					}					
				}
				else if(flash.EXO.CalHexaItemCode[UartSelect]==0x2E || flash.EXO.CalHexaItemCode[UartSelect]==0x37)//FDOM, RFU, QSU 보정전용
				{
					if(flash.EXO.CalCommand==5)
					{
						dataBuf[0]=gv.EXOTimeCode[0];
						dataBuf[1]=gv.EXOTimeCode[1]; 
						dataBuf[2]=gv.EXOTimeCode[2];
						dataBuf[3]=gv.EXOTimeCode[3];
						dataBuf[4]=0;
						//Dprintf(" FDOM CALIBRATION END TIME CODE = %02x %02x %02x %02x \n",gv.EXOTimeCode[0],gv.EXOTimeCode[1],gv.EXOTimeCode[2],gv.EXOTimeCode[3]);
					}
					switch(flash.EXO.CalCommand)
					{
						//임시 보정계수
						case 0: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x0D, 0x00, 0x00, dataBuf);		break;
						//보정유효성
						case 1: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x09, 0x00, 0x00, dataBuf);		break;
						//임시데이터
						case 2: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x12, 0x00, 0x00, dataBuf);		break;
						//해당 원시 데이터값
						case 3: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x17, 0x00, 0x00, dataBuf);		break;
						//보정상태
						case 4: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x13, 0x00, 0x00, dataBuf);		break;
						//보정시간
						case 5: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x04, 0x00, 0x04, dataBuf);		break;
						//보정계수
						case 6: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x05, 0x00, 0x00, dataBuf);		break;
						case 7: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x25, 0x00, 0x00, dataBuf);break;
						//저장	
						case 8: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x14, 0x00, 0x00, dataBuf);		break;
					}
					flash.EXO.CalCommand++;
					if(flash.EXO.CalCommand>=9)//보정을 종료한다.
					{
						//Dprintf("CALIBRATION END STEP\n");
						flash.EXO.CalStartStep[flash.EXO.UartPort]	=0;
						flash.EXO.CalCommand	=0;
						flash.EXO.CalStart[flash.EXO.UartPort]		=4;//보정을 완료한다. 
						//SetWidgetColor(EXO_CalApplyID,clBlue, clWhite);	
					}					
				}
				else
				{
					if(flash.EXO.CalCommand==2)
					{
						dataBuf[0]=gv.EXOTimeCode[0];
						dataBuf[1]=gv.EXOTimeCode[1];
						dataBuf[2]=gv.EXOTimeCode[2];
						dataBuf[3]=gv.EXOTimeCode[3];
						dataBuf[4]=0;
						//Dprintf("CALIBRATION END TIME CODE = %02x %02x %02x %02x \n",gv.EXOTimeCode[0],gv.EXOTimeCode[1],gv.EXOTimeCode[2],gv.EXOTimeCode[3]);
					}
					switch(flash.EXO.CalCommand)
					{
						case 0: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x0D, 0x00, 0x00, dataBuf);	break;
						case 1: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x09, 0x00, 0x00, dataBuf);	break;
						case 2: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x30, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x04, 0x00, 0x04, dataBuf);break;
						case 3: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x05, 0x00, 0x00, dataBuf);break;
						case 4: EXO_Calibration_Command(UartSelect, COMMON_CALIBRATION_ID, SONDE_ID, 0x20, 0x00, flash.EXO.CalHexaItemCode[UartSelect], 0x14, 0x00, 0x00, dataBuf);break;
						case 5: EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, dataBuf);break;//ALL
					}
					flash.EXO.CalCommand++;
					if(flash.EXO.CalCommand>=6)
					{
						flash.EXO.CalStartStep[flash.EXO.UartPort]=0;
						flash.EXO.CalCommand=0;
						flash.EXO.CalStart[flash.EXO.UartPort]=4;//보정을 완료한다. 
						//SetWidgetColor(EXO_CalApplyID,clBlue, clWhite);	
					}
				}
			}
		}	
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//		보정 모드에서만 대기 하는 경우
		//		1. 포트 정보를 요청한다.
		//		2. 측정치를 요청한다.
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else {
			//1. 포트 정보를 요청한다.
			//Dprintf("EXO_Find_CommandChangeFlag[UartSelect] = %d\n",EXO_Find_CommandChangeFlag[UartSelect]);
			if(EXO_Find_CommandChangeFlag[UartSelect]==ON)
			{
				//Dprintf("gToggle[%d] = %d\n",UartSelect,gToggle[UartSelect]);
				switch( gv.exoPortFindCnt[UartSelect] )
				{
					//case 0: 	EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); break;
					case 1:	    EXO_SondeIDFind(UartSelect);break;
					
                    case 2:	    EXO_SensorFind(UartSelect, 0x00);break;//PORT1
					case 3:	    EXO_Calibration_Command(UartSelect, EXO_PORT1, SONDE_ID, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00);flash.EXO.Sensor_PORT[UartSelect]=EXO_PORT1;break;//어떤센서인지를 알아본다.
					//case 4: 	EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); break;
					
                    case 4:	    EXO_SensorFind(UartSelect, 0x01);break;//PORT2
					case 5:	    EXO_Calibration_Command(UartSelect, EXO_PORT2, SONDE_ID, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00);flash.EXO.Sensor_PORT[UartSelect]=EXO_PORT2;break;//어떤센서인지를 알아본다.
					//case 7: 	EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); break;
					
                    case 6:	    EXO_SensorFind(UartSelect, 0x02);break;//PORT3
					case 7:	    EXO_Calibration_Command(UartSelect, EXO_PORT3, SONDE_ID, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00);flash.EXO.Sensor_PORT[UartSelect]=EXO_PORT3;break;//어떤센서인지를 알아본다.
					//case 10: 	EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); break;
					
                    case 8:	EXO_SensorFind(UartSelect, 0x03);break;//PORT4
					case 9:	EXO_Calibration_Command(UartSelect, EXO_PORT4, SONDE_ID, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00);flash.EXO.Sensor_PORT[UartSelect]=EXO_PORT4;break;//어떤센서인지를 알아본다.
					//case 13: 	EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); break;
					
                    case 10:	EXO_SensorFind(UartSelect, 0x04);break;//PORT5
					case 11:	EXO_Calibration_Command(UartSelect, EXO_PORT5, SONDE_ID, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00);flash.EXO.Sensor_PORT[UartSelect]=EXO_PORT5;break;//어떤센서인지를 알아본다.
					//case 17: 	EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); break;
					
                    case 12:	EXO_SensorFind(UartSelect, 0x05);break;//PORT6
					case 13:	EXO_Calibration_Command(UartSelect, EXO_PORT6, SONDE_ID, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00);flash.EXO.Sensor_PORT[UartSelect]=EXO_PORT6;break;//어떤센서인지를 알아본다.
					//case 20: 	EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); break;
					
                    case 14:	EXO_SensorFind(UartSelect, 0x06);break;//PORT7
					case 15:	EXO_Calibration_Command(UartSelect, EXO_PORT7, SONDE_ID, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00);flash.EXO.Sensor_PORT[UartSelect]=EXO_PORT7;break;//어떤센서인지를 알아본다.
					
                    case 16:	EXO_SensorFind(UartSelect, 0x07);break;//PORT8
					case 17:	EXO_Calibration_Command(UartSelect, EXO_PORT8, SONDE_ID, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00);flash.EXO.Sensor_PORT[UartSelect]=EXO_PORT8;break;//어떤센서인지를 알아본다.
				}
				gv.exoPortFindCnt[UartSelect]++;
				if(gv.exoPortFindCnt[UartSelect]>=18) {
					gv.exoPortFindCnt[UartSelect]=0;
				}
			}
			else
			{
				if(gv.serialComStatusToggle[UartSelect]!=3) {//측정값 요청한다.
					gv.serialComStatusToggle[UartSelect]=0; 
				}
				//Dprintf("송신 = %d\n", gv.exoBufferCount[UartSelect]);
//Dputc('Q');
                EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); 
				//EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); 
				//7E 02 00 A1 81 8C 24 99 99 99 99 20 00 01 D0 00 00 4D 4B : 데이터요청
				//EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x01, 0xD0, 0x00, 0x00, 0x00); 
				//7E 02 00 99 99 99 99 A1 81 8C 24 00 00 01 D0 00 24 41 AE 6E 40 37 F2 82 60 B8 E9 A2 00 7F FF FF FF 7F FF FF FF 7F FF FF FF 00 02 EA 8F 00 03 20 6D 7F FF FF FF C7 59 //응답코드
				//2. 측정치를 요청한다.
/*				
				switch( gToggle[UartSelect] )
				{
					case 0: EXO_Calibration_Command(UartSelect, COMMON_ID, SONDE_ID, 0x20, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00); 
					break;
					case 1:	EXO_SondeIDFind(UartSelect);
					break;
				}
				gToggle[UartSelect]++;
				if(gToggle[UartSelect]>=2)gToggle[UartSelect]=0; //측정값 만 요청한다.
				
				//gToggle[UartSelect]=0;
				//Dprintf("EXO USART[%d] count=%d\n",UartSelect,gToggle[UartSelect]);
*/				
				//Dprintf("EXO[%d] 측정값 요청\n",UartSelect);
			}
		}
	}
    
    if(gv.nFlag_ReceiveDataProcess[UartSelect]==3) {
        gv.nFlag_ReceiveDataProcess[UartSelect]=0;
        Dprintf("UART[%d] CRC ERROR LENGTH %d, %d\n",UartSelect ,gv.exoBufferCount[UartSelect], gv.nReceiveBufferCount[UartSelect]);
    } 
    else if(gv.nFlag_ReceiveDataProcess[UartSelect]==4) {
        gv.nFlag_ReceiveDataProcess[UartSelect]=0;
        Dprintf("EXO OVERFLOW 1 ERROR\n");
    } 
    else if(gv.nFlag_ReceiveDataProcess[UartSelect]==5) {
        gv.nFlag_ReceiveDataProcess[UartSelect]=0;
        Dprintf("EXO OVERFLOW 2 ERROR\n");
    } 
    else {
        gv.nFlag_ReceiveDataProcess[UartSelect]=0;
    }
	return (1);
}
int	 EXO_IOPrepareEvent(HANDLE handle, int command, int param, void *Data) {
	stIOProcess 	*io_process = FindIOProcess(handle);
	if(io_process==NULL) 
	{
		Dprintf("IOPrepareEvent:: IO Precess not Found\n");
		return(0);
	}
	EXO_IO_PROCESS = IOOpenTimer(handle, 500);  //읽는 시간을 줄이면 전체적인 시간이 변경되어야 한다.
	IOStartTimer(EXO_IO_PROCESS);               //DataLog_EXO_ItemSelect 에서 활성화된다.
	return(1);
}

int	 EXO_IOTimerEvent(HANDLE handle, int command, int param, void *Data) {
	stIOProcess 	*io_process = FindIOProcess(handle);
	if(io_process==NULL) {
		Dprintf("IOTimerEvent:: IO Precess not Found\n");
		return(0);
	}
		
    if(flash.Uart[USART_1]->SensorID == SENSOR_EXO) {		
		EXO_Receive_Process(USART_1);
    }
    if(flash.Uart[USART_2]->SensorID == SENSOR_EXO) {
		EXO_Receive_Process(USART_2);
    }
    if(flash.Uart[USART_3]->SensorID == SENSOR_EXO) {
		EXO_Receive_Process(USART_3);
    }
    return 0;
}

int EXO_IOProcess(void) {
    Dprintf("[ENABLE] EXO_IOProcess\n");
	makeIOProcess("EXO", EXO_IOPrepareEvent, EXO_IOTimerEvent);
	return(1);
}
#endif

