#include "TEECO_System.h"
#include "DataLog.h"

#if defined(PONSEL_EC)

int gPonEcCommandToggle=0;
int gCalRagneSet=0;
volatile int gCalColor;
volatile int gCleanFlag;
volatile int gDispCount;
volatile static int play_timer=0;

void PONSEL_EC_Status(unsigned int comPort,unsigned int address) {
    unsigned char crcBuf[256];
    unsigned short crc=0;
    unsigned int i=0;
    
    crcBuf[0]=address;
    crcBuf[1]=0x10;
    crcBuf[2]=0x00;
    crcBuf[3]=0x01;
    crcBuf[4]=0x00;
    crcBuf[5]=0x01;
    crcBuf[6]=0x02;
    crcBuf[7]=0x00;
    crcBuf[8]=0x1F;
    crc=crc16(crcBuf,9);
    crcBuf[9]=(crc&0xFF00)>>8;
    crcBuf[10]=crc&0x00FF;
    for(i=0;i<11;i++)
    {
        Uart_TransmitPort(comPort,crcBuf[i]);
    }
}
void PONSEL_EC_Request(unsigned int comPort,unsigned int address) {
    unsigned char crcBuf[256];
    unsigned short crc=0;
    unsigned int i=0;
    
    crcBuf[0]=address;
    crcBuf[1]=0x03;
    crcBuf[2]=0x00;
    crcBuf[3]=0x52;
    crcBuf[4]=0x00;
    crcBuf[5]=0x0B;
    crc=crc16(crcBuf,6);
    crcBuf[6]=(crc&0xFF00)>>8;
    crcBuf[7]=crc&0x00FF;
    for(i=0;i<8;i++)
    {
        Uart_TransmitPort(comPort,crcBuf[i]);
    }
}
void PONSEL_EC_Command(unsigned int comPort,unsigned int address, unsigned int functionCode, unsigned int hMemory, unsigned int lMemory) {
    unsigned char crcBuf[256];
    unsigned short crc=0;
    unsigned int i=0;
    
    crcBuf[0]=address;
    crcBuf[1]=functionCode;
    crcBuf[2]=(hMemory&0xFF00)>>8;
    crcBuf[3]=hMemory&0x00FF;
    crcBuf[4]=(lMemory&0xFF00)>>8;
    crcBuf[5]=lMemory&0x00FF;
    crc=crc16(crcBuf,6);
    crcBuf[6]=(crc&0xFF00)>>8;
    crcBuf[7]=crc&0x00FF;
    
    for(i=0;i<8;i++)
    {
        Uart_TransmitPort(comPort,crcBuf[i]);
    }
}
int PONSEL_EC_DataReceive( int ascii, int ch ) {
	int port=0;
	unsigned char		aCrcBuf[256];
	unsigned int		nReturnCrc16=0;
	static 	char		oldAscii=0;
    float   mV=0.0F;
    U32     *address;
    //U8      a, b, c, d;
    void    *addr1; 
	addr1 = &mV; 
	address = addr1;
	
    if((oldAscii == 0x0A || oldAscii == 0x0B || oldAscii == 0x0C || oldAscii == 0x0D || oldAscii == 0x0E || oldAscii == 0x0F || oldAscii == 0x10 || oldAscii == 0x11) && (ascii == 0x03 || ascii == 0x10))//측정치를 요청하는 경우
	{
		gv.nFlag_ReceiveStart[ch]=1;
		gv.nReceiveBufferCount[ch]=0;
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++] = oldAscii;
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++] = ascii;
	}
	else if( gv.nFlag_ReceiveStart[ch]==1) 
	{
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++] = ascii;
		if(flash.ponsel_ec.calStartFlag==1)//온도보정리턴값
        {
            
        }
		if(flash.ponsel_ec.calStartFlag==2||flash.ponsel_ec.calStartFlag==3)//전도도보정리턴값
        {
            if( 
                flash.ponsel_ec.calStep==8
                ||flash.ponsel_ec.calStep==13
                ||flash.ponsel_ec.calStep==15
                ||flash.ponsel_ec.calStep==16
                ||flash.ponsel_ec.calStep==17
                ||flash.ponsel_ec.calStep==19
                ||flash.ponsel_ec.calStep==20
                ||flash.ponsel_ec.calStep==21
                )
            {
                if(gv.nReceiveBufferCount[ch] == 9)//리턴값
                {
                    aCrcBuf[0]=gv.aItemReceiveBuffer[ch][0];//id = 0x0A
                    aCrcBuf[1]=gv.aItemReceiveBuffer[ch][1];//function code = 0x10
                    aCrcBuf[2]=gv.aItemReceiveBuffer[ch][2];//데이터 길이 = 0x00
                    aCrcBuf[3]=gv.aItemReceiveBuffer[ch][3];//valueHigh1 = 0x01
                    aCrcBuf[4]=gv.aItemReceiveBuffer[ch][4];//valueHigh2 = 0x00
                    aCrcBuf[5]=gv.aItemReceiveBuffer[ch][5];//valueLow1 = 0x01
                    aCrcBuf[6]=gv.aItemReceiveBuffer[ch][6];//valueLow1 = 0x01
                    
                    nReturnCrc16=(crc16(aCrcBuf,7));
                    aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
                    aCrcBuf[1]=(nReturnCrc16&0x00FF);
                    
                    if((aCrcBuf[0]==gv.aItemReceiveBuffer[ch][7])&&(aCrcBuf[1]==gv.aItemReceiveBuffer[ch][8]))
                    {
                        flash.ponsel_ec.calStep++;
                        gv.nFlag_ReceiveStart[ch]=0;
                        gv.nReceiveBufferCount[ch]=0;
                    }
                }                
            }
            if( flash.ponsel_ec.calStep==0
                ||flash.ponsel_ec.calStep==1
                ||flash.ponsel_ec.calStep==2
                ||flash.ponsel_ec.calStep==4
                ||flash.ponsel_ec.calStep==6
                ||flash.ponsel_ec.calStep==7
                ||flash.ponsel_ec.calStep==9
                ||flash.ponsel_ec.calStep==11
			    ||flash.ponsel_ec.calStep==12
			    ||flash.ponsel_ec.calStep==14
                ||flash.ponsel_ec.calStep==23
			    ||flash.ponsel_ec.calStep==24
				||flash.ponsel_ec.calStep==25
            )
            {
                if(gv.nReceiveBufferCount[ch] == 8)//리턴값
                {
                    aCrcBuf[0]=gv.aItemReceiveBuffer[ch][0];//id = 0x0A
                    aCrcBuf[1]=gv.aItemReceiveBuffer[ch][1];//function code = 0x10
                    aCrcBuf[2]=gv.aItemReceiveBuffer[ch][2];//데이터 길이 = 0x00
                    aCrcBuf[3]=gv.aItemReceiveBuffer[ch][3];//valueHigh1 = 0x01
                    aCrcBuf[4]=gv.aItemReceiveBuffer[ch][4];//valueHigh2 = 0x00
                    aCrcBuf[5]=gv.aItemReceiveBuffer[ch][5];//valueLow1 = 0x01
                    
                    nReturnCrc16=(crc16(aCrcBuf,6));
                    aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
                    aCrcBuf[1]=(nReturnCrc16&0x00FF);
                    
                    if((aCrcBuf[0]==gv.aItemReceiveBuffer[ch][6])&&(aCrcBuf[1]==gv.aItemReceiveBuffer[ch][7]))
                    {
                        if(flash.ponsel_ec.calStep==6)
                        {
                            flash.ponsel_ec.calStep=5;
                        }
                        else if(flash.ponsel_ec.calStep==11)
                        {
                            flash.ponsel_ec.calStep=10;
                        }
                        else 
                        {
                            if(flash.ponsel_ec.calStep==25)
                            {
									
                                //보정종료
                                //IOStartTimer(EC_PONSEL_IO_PROCESS);
                                
                                //SetWidgetText(gv.handle2[7], "보정시작");
                                
								flash.ponsel_ec.calSV[0] = 0.0f;
                                flash.ponsel_ec.calReceiveFlag=0;   // 명령수신상태
                                flash.ponsel_ec.calStep=0;          // 명령스텝
                                gDispCount=0;                               // 문자를 변경한다.
                                gCleanFlag=0;                               // 세척
                                //SetWidgetStatus(gv.handle2[1],ON);
                                //flash.ponsel_ec.channel=0;//현재채널
                                flash.ponsel_ec.calStartFlag=0;//보정시작플래그
                                //채널주소값고정
                                flash.ponsel_ec.modbusAddress[0]=0x0A;
                                flash.ponsel_ec.modbusAddress[1]=0x0B;
                                flash.ponsel_ec.modbusAddress[2]=0x0C;
                                flash.ponsel_ec.modbusAddress[3]=0x0D;
                                flash.ponsel_ec.modbusAddress[4]=0x0E;

                            }
							
							if(gPonEcCommandToggle==0 && flash.ponsel_ec.calStep==4) {
								flash.ponsel_ec.calStep=3;
								gPonEcCommandToggle=1;
							} else {
								flash.ponsel_ec.calStep++;
							}
							
							
                        }

                        gv.nFlag_ReceiveStart[ch]=0;
                        gv.nReceiveBufferCount[ch]=0;
                    }
                }
            }
           if( flash.ponsel_ec.calStep==3
                || flash.ponsel_ec.calStep==12
                || flash.ponsel_ec.calStep==16
                || flash.ponsel_ec.calStep==18
			    || flash.ponsel_ec.calStep==22
           )
           {
               if(gv.nReceiveBufferCount[ch] == 7)//리턴값
                {
                    aCrcBuf[0]=gv.aItemReceiveBuffer[ch][0];//id = 0x0A
                    aCrcBuf[1]=gv.aItemReceiveBuffer[ch][1];//function code = 0x10
                    aCrcBuf[2]=gv.aItemReceiveBuffer[ch][2];//데이터 길이 = 0x00
                    aCrcBuf[3]=gv.aItemReceiveBuffer[ch][3];//valueHigh1 = 0x01
                    aCrcBuf[4]=gv.aItemReceiveBuffer[ch][4];//valueHigh2 = 0x00
                    
                    nReturnCrc16=(crc16(aCrcBuf,5));
                    aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
                    aCrcBuf[1]=(nReturnCrc16&0x00FF);
                    if((aCrcBuf[0]==gv.aItemReceiveBuffer[ch][5])&&(aCrcBuf[1]==gv.aItemReceiveBuffer[ch][6]))
                    {
						
						flash.ponsel_ec.calStep++;
						
                        gv.nFlag_ReceiveStart[ch]=0;
                        gv.nReceiveBufferCount[ch]=0;
						/*
						if( flash.ponsel_ec.calStep==23) {
							//보정종료
							
							//SetWidgetText(gv.handle2[7], "보정시작");
							
							IOStartTimer(EC_PONSEL_IO_PROCESS);
							flash.ponsel_ec.calSV[0] = 0.0f;
							flash.ponsel_ec.calReceiveFlag=0;//명령수신상태
							flash.ponsel_ec.calStep=0;//명령스텝
							gDispCount=0;//문자를 변경한다.
							gCleanFlag=0;//세척
							//SetWidgetStatus(gv.handle2[1],ON);
							//flash.ponsel_ec.channel=0;//현재채널
							flash.ponsel_ec.calStartFlag=0;//보정시작플래그
							//채널주소값고정
							flash.ponsel_ec.modbusAddress[0]=0x0A;
							flash.ponsel_ec.modbusAddress[1]=0x0B;
							flash.ponsel_ec.modbusAddress[2]=0x0C;
							flash.ponsel_ec.modbusAddress[3]=0x0D;
							flash.ponsel_ec.modbusAddress[4]=0x0E;
                    		
							flash.ponsel_ec.calStep=26;
						}
						*/
                    }
                }
           }
		   /**
		   
			교정중 측정값을 요청한다.
		   
		   */
           if( flash.ponsel_ec.calStep==5 || flash.ponsel_ec.calStep==10)
           {
                //Dprintf("CAL DATA OUT\n");
                if(gv.nReceiveBufferCount[ch]==(gv.aItemReceiveBuffer[ch][2] + 5))//0A 03 00 00 4바이트는 길이에 포함이 안된다. 그래서 추가한다.
                {
                    aCrcBuf[0]=gv.aItemReceiveBuffer[ch][0];//id
                    aCrcBuf[1]=gv.aItemReceiveBuffer[ch][1];//function code
                    aCrcBuf[2]=gv.aItemReceiveBuffer[ch][2];//데이터 길이
                    aCrcBuf[3]=gv.aItemReceiveBuffer[ch][3];//0x00
                    aCrcBuf[4]=gv.aItemReceiveBuffer[ch][4];//0x00
                    //온도
                    aCrcBuf[5]=gv.aItemReceiveBuffer[ch][5];
                    aCrcBuf[6]=gv.aItemReceiveBuffer[ch][6];
                    aCrcBuf[7]=gv.aItemReceiveBuffer[ch][7];
                    aCrcBuf[8]=gv.aItemReceiveBuffer[ch][8];
                    //염도
                    aCrcBuf[9]=gv.aItemReceiveBuffer[ch][9];
                    aCrcBuf[10]=gv.aItemReceiveBuffer[ch][10];
                    aCrcBuf[11]=gv.aItemReceiveBuffer[ch][11];
                    aCrcBuf[12]=gv.aItemReceiveBuffer[ch][12];
                    //전도도
                    aCrcBuf[13]=gv.aItemReceiveBuffer[ch][13];
                    aCrcBuf[14]=gv.aItemReceiveBuffer[ch][14];
                    aCrcBuf[15]=gv.aItemReceiveBuffer[ch][15];
                    aCrcBuf[16]=gv.aItemReceiveBuffer[ch][16];
                    //COND NOT COMPENSED
                    aCrcBuf[17]=gv.aItemReceiveBuffer[ch][17];
                    aCrcBuf[18]=gv.aItemReceiveBuffer[ch][18];
                    aCrcBuf[19]=gv.aItemReceiveBuffer[ch][19];
                    aCrcBuf[20]=gv.aItemReceiveBuffer[ch][20];

                    aCrcBuf[21]=gv.aItemReceiveBuffer[ch][21];
                    aCrcBuf[22]=gv.aItemReceiveBuffer[ch][22];
                    aCrcBuf[23]=gv.aItemReceiveBuffer[ch][23];
                    aCrcBuf[24]=gv.aItemReceiveBuffer[ch][24];

                    nReturnCrc16=(crc16(aCrcBuf,25));
                    aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
                    aCrcBuf[1]=(nReturnCrc16&0x00FF);
                    
					
                    if((aCrcBuf[0]==gv.aItemReceiveBuffer[ch][25])&&(aCrcBuf[1]==gv.aItemReceiveBuffer[ch][26]))
                    {
                        if(gv.aItemReceiveBuffer[ch][0]==0x0A)
                        {
                            *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);
                            *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);
                            *address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);
							
                            if(flash.ponsel_ec.channel==0) {
                                if(mV < 1000) { 
                                    flash.ponsel_ec.PV = mV;
                                }
                            }
                        }
                        else if(gv.aItemReceiveBuffer[ch][0]==0x0B)
                        {
                            *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);
                            *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);
                            *address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);
							if(mV < 1000) {
								if(flash.ponsel_ec.channel==1)flash.ponsel_ec.PV = mV;
                            }
                        }
                        else if(gv.aItemReceiveBuffer[ch][0]==0x0C)
                        {
                            *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);
                            *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);
                            *address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);
							if(mV < 1000) {
								if(flash.ponsel_ec.channel==2)flash.ponsel_ec.PV = mV;
                            }
                        }
                        else if(gv.aItemReceiveBuffer[ch][0]==0x0D)
                        {
                            *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);
                            *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);
                            *address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);
							if(mV < 1000) {
								if(flash.ponsel_ec.channel==3)flash.ponsel_ec.PV = mV;
                            }
                        }
                        else if(gv.aItemReceiveBuffer[ch][0]==0x0E)
                        {
                            *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);
                            *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);
                            *address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);
							if(mV < 1000) {
								if(flash.ponsel_ec.channel==4)flash.ponsel_ec.PV = mV;
                            }
                        }
						
                    }
                    gv.nFlag_ReceiveStart[ch]=0;
                    gv.nReceiveBufferCount[ch]=0;
                    flash.ponsel_ec.calStep++;
                }               
           }
        }
		/**
		
			측정값을 요청한다.
		
		*/
        if(flash.ponsel_ec.calStartFlag==0)
        {
            if(gv.aItemReceiveBuffer[ch][2]==0x00)// 0A 10 00 01 00 01 51 72 //제어명령에 대한 리턴값
            {
                if(gv.nReceiveBufferCount[ch] == 8)//리턴값
                {
                    aCrcBuf[0]=gv.aItemReceiveBuffer[ch][0];//id = 0x0A
                    aCrcBuf[1]=gv.aItemReceiveBuffer[ch][1];//function code = 0x10
                    aCrcBuf[2]=gv.aItemReceiveBuffer[ch][2];//데이터 길이 = 0x00
                    aCrcBuf[3]=gv.aItemReceiveBuffer[ch][3];//valueHigh1 = 0x01
                    aCrcBuf[4]=gv.aItemReceiveBuffer[ch][4];//valueHigh2 = 0x00
                    aCrcBuf[5]=gv.aItemReceiveBuffer[ch][5];//valueLow1 = 0x01
                    
                    nReturnCrc16=(crc16(aCrcBuf,6));
                    aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
                    aCrcBuf[1]=(nReturnCrc16&0x00FF);
                    if((aCrcBuf[0]==gv.aItemReceiveBuffer[ch][6])&&(aCrcBuf[1]==gv.aItemReceiveBuffer[ch][7]))
                    {
                        //Uart_SendByte2('M');
                    }
                }
            }
            //측정값에 대한 리턴값
            else if(gv.nReceiveBufferCount[ch]==(gv.aItemReceiveBuffer[ch][2] + 5))//0A 03 00 00 4바이트는 길이에 포함이 안된다. 그래서 추가한다.
            {
                aCrcBuf[0]=gv.aItemReceiveBuffer[ch][0];//id
                aCrcBuf[1]=gv.aItemReceiveBuffer[ch][1];//function code
                aCrcBuf[2]=gv.aItemReceiveBuffer[ch][2];//데이터 길이
                aCrcBuf[3]=gv.aItemReceiveBuffer[ch][3];//0x00
                aCrcBuf[4]=gv.aItemReceiveBuffer[ch][4];//0x00
                //온도
                aCrcBuf[5]=gv.aItemReceiveBuffer[ch][5];
                aCrcBuf[6]=gv.aItemReceiveBuffer[ch][6];
                aCrcBuf[7]=gv.aItemReceiveBuffer[ch][7];
                aCrcBuf[8]=gv.aItemReceiveBuffer[ch][8];
                
				//전도도
                aCrcBuf[9]=gv.aItemReceiveBuffer[ch][9];
                aCrcBuf[10]=gv.aItemReceiveBuffer[ch][10];
                aCrcBuf[11]=gv.aItemReceiveBuffer[ch][11];
                aCrcBuf[12]=gv.aItemReceiveBuffer[ch][12];
                
				//염도
                aCrcBuf[13]=gv.aItemReceiveBuffer[ch][13];
                aCrcBuf[14]=gv.aItemReceiveBuffer[ch][14];
                aCrcBuf[15]=gv.aItemReceiveBuffer[ch][15];
                aCrcBuf[16]=gv.aItemReceiveBuffer[ch][16];
                
				//DO ppm
                aCrcBuf[17]=gv.aItemReceiveBuffer[ch][17];
                aCrcBuf[18]=gv.aItemReceiveBuffer[ch][18];
                aCrcBuf[19]=gv.aItemReceiveBuffer[ch][19];
                aCrcBuf[20]=gv.aItemReceiveBuffer[ch][20];

                aCrcBuf[21]=gv.aItemReceiveBuffer[ch][21];
                aCrcBuf[22]=gv.aItemReceiveBuffer[ch][22];
                aCrcBuf[23]=gv.aItemReceiveBuffer[ch][23];
                aCrcBuf[24]=gv.aItemReceiveBuffer[ch][24];

                nReturnCrc16=(crc16(aCrcBuf,25));
                aCrcBuf[0]=(nReturnCrc16>>8)&0x00FF;
                aCrcBuf[1]=(nReturnCrc16&0x00FF);
                
                if((aCrcBuf[0]==gv.aItemReceiveBuffer[ch][25])&&(aCrcBuf[1]==gv.aItemReceiveBuffer[ch][26]))
                {
                    if(gv.aItemReceiveBuffer[ch][0]==0x0A)
                    {
						port = 0;
                        *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);//온도
                        if(mV < 100) {
                            flash.ponsel_ec.tempSetPv[port] = mV;
                            flash.Uart[USART_1]->SensorPV[0] = mV;						
                        }
                        *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);//염분
                        if(mV < 100) {
                            flash.Uart[USART_1]->SensorPV[2] = mV;
						}
                        *address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);//전도도
						if(mV < 1000) {
							flash.Uart[USART_1]->SensorPV[1] = mV;
                            if(flash.ponsel_ec.channel==0)flash.ponsel_ec.PV = mV;
                        }
                    }
                    if(gv.aItemReceiveBuffer[ch][0]==0x0B)
                    {
						port = 1;
                        *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);
						if(mV < 1000) {
							flash.Uart[USART_1]->SensorPV[3] = mV;
						}
                        *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);
						if(mV < 1000) {
							flash.Uart[USART_1]->SensorPV[5] = mV;
						}
						*address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);
						if(mV < 1000) {
							flash.Uart[USART_1]->SensorPV[4] = mV * 1000;
							if(flash.ponsel_ec.channel==1)flash.ponsel_ec.PV = mV;
						}
                    }
                    if(gv.aItemReceiveBuffer[ch][0]==0x0C)
                    {
						port = 2;
                        *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);
						if(mV < 1000) {	
							flash.Uart[USART_1]->SensorPV[6] = mV;
						}
                        *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);
						if(mV<1000) {
							flash.Uart[USART_1]->SensorPV[8] = mV;
						}
						*address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);
						if(mV<1000) {
							flash.Uart[USART_1]->SensorPV[7] = mV * 1000;
							if(flash.ponsel_ec.channel==2)flash.ponsel_ec.PV = mV;
						}
                    }
                    if(gv.aItemReceiveBuffer[ch][0]==0x0D)
                    {
						port = 3;
                        *address = ((gv.aItemReceiveBuffer[ch][5] << 24) | (gv.aItemReceiveBuffer[ch][6] << 16) | (gv.aItemReceiveBuffer[ch][7] << 8) | gv.aItemReceiveBuffer[ch][8]);
						if(mV < 1000) {	
							flash.Uart[USART_1]->SensorPV[9] = mV;
						}
                        *address = ((gv.aItemReceiveBuffer[ch][13] << 24) | (gv.aItemReceiveBuffer[ch][14] << 16) | (gv.aItemReceiveBuffer[ch][15] << 8) | gv.aItemReceiveBuffer[ch][16]);
						if(mV < 1000) {
							flash.Uart[USART_1]->SensorPV[11] = mV;
						}
						*address = ((gv.aItemReceiveBuffer[ch][9] << 24) | (gv.aItemReceiveBuffer[ch][10] << 16) | (gv.aItemReceiveBuffer[ch][11] << 8) | gv.aItemReceiveBuffer[ch][12]);
						if(mV < 1000) {
							flash.Uart[USART_1]->SensorPV[10] = mV * 1000;
							if(flash.ponsel_ec.channel==3)flash.ponsel_ec.PV = mV;
						}
                    }
                    if(gv.aItemReceiveBuffer[ch][0]==0x0E)
                    {
                    }
                    gv.nFlag_ReceiveDataProcess[ch] = ENABLE;
                }
                gv.nFlag_ReceiveStart[ch]=0;
                gv.nReceiveBufferCount[ch]=0;
            }
        }
		if(gv.nReceiveBufferCount[ch] > 49)
		{
			gv.nFlag_ReceiveStart[ch]=0; 
			gv.nReceiveBufferCount[ch]=0;
			gv.nFlag_ReceiveDataProcess[ch] = DISABLE;
		}
	}
	oldAscii = ascii;
	return 1;
}
int	PONSEL_EC_KeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle," ");
	Graphics 	*Gr = GetGraphics(NULL, parent, CLIENT_AREA); 	//그래픽영역을 얻는다.
    char data[20];
    
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

	PaintDrawControl(OFF);
	Gr->FillRect(0, 0, 0, 0);	//dummy code	       
    
	
	
	
    //VoicePlay(TOUCH_SOUND);
    if(win->ID==2)//채널1
    {
        if(flash.ponsel_ec.channel==0) flash.ponsel_ec.channel=1;
        else if( flash.ponsel_ec.channel == 1) flash.ponsel_ec.channel=2;            
        else if( flash.ponsel_ec.channel == 2) flash.ponsel_ec.channel=3;            
        else if( flash.ponsel_ec.channel == 3) flash.ponsel_ec.channel=4;            
        else if( flash.ponsel_ec.channel == 4) flash.ponsel_ec.channel=0;            

        switch( flash.ponsel_ec.channel )
        {
            case 0: SetWidgetText(gv.handle2[2],"채널1");
                break;
            case 1: SetWidgetText(gv.handle2[2],"채널2");
                break;
            case 2: SetWidgetText(gv.handle2[2],"채널3");
                break;
            case 3: SetWidgetText(gv.handle2[2],"채널4");
                break;
            case 4: SetWidgetText(gv.handle2[2],"채널5");
                break;
        }
        #if defined (PONSEL_EC)
        flash.ponsel_ec.modbusAddress[0]=0x0A;
        flash.ponsel_ec.modbusAddress[1]=0x0B;
        flash.ponsel_ec.modbusAddress[2]=0x0C;
        flash.ponsel_ec.modbusAddress[3]=0x0D;
        flash.ponsel_ec.modbusAddress[4]=0x0E;
        #endif
    }
	else if(win->ID==3) {
		switch( gCalRagneSet ) {
			case 0:
				SetWidgetText(gv.handle2[3],"0/700mS/cm");
				gCalRagneSet=1;
				break;
			case 1:
				SetWidgetText(gv.handle2[3],"0/100mS/cm");
				gCalRagneSet=0;
				break;
			case 2:
				SetWidgetText(gv.handle2[3],"0/200000uS/cm");
				gCalRagneSet=3;
				break;
			case 3:
				SetWidgetText(gv.handle2[3],"0/200uS/cm");
				gCalRagneSet=0;
				break;
		}
		Dprintf("RANGE = %d\n", gCalRagneSet);
	}
    else if(win->ID==4)//세척시간설정
    {
        KeyPad(win->Parent, win->Handle,  gv.handle2[4], (1024-345)/2, (LCD_Y-415)/2, 1, "", "세척시간(분)");
    }
    else if(win->ID==5)//세척시작
    {
		flash.ponsel_ec.clearCountPV=flash.ponsel_ec.clearCountSV*60;
    }
    else if(win->ID==6)//표준버퍼
    {
        KeyPad(win->Parent, win->Handle,  gv.handle2[6], (1024-345)/2, (LCD_Y-415)/2, 1, "", "표준버퍼");
    }
    else if(win->ID==7)//전도도보정시작
    {
        if(flash.ponsel_ec.calStartFlag==0)
        {
            //IOStopTimer(EC_PONSEL_IO_PROCESS);
            
            GetWidgetText(gv.handle2[6],data);
            flash.ponsel_ec.calSV[0]=atof(data);
            Dprintf("calSV = %10.1f\n",flash.ponsel_ec.calSV[0]);
            
            SetWidgetStatus(gv.handle2[7],ON);
            SetWidgetText(gv.handle2[7], "제로교정중");
            
            gCalColor = clYellow;
            flash.ponsel_ec.calStartFlag=2;//보정모드로 이동한다.
            flash.ponsel_ec.calReceiveFlag=0;
            flash.ponsel_ec.calStep=0;
#ifdef PONSEL_PH_COND_ENABLE            
            ponsel_phcond_step = 0;//인터럽트에서 값수신
#endif			
        }
        else if(flash.ponsel_ec.calStartFlag==2)
        {
            flash.ponsel_ec.calStartFlag=3;
            flash.ponsel_ec.calStep=7;    
            SetWidgetText(gv.handle2[7], "스펜교정중");
        }
        else if(flash.ponsel_ec.calStartFlag==3)
        {
            flash.ponsel_ec.calStep=12;
            SetWidgetText(gv.handle2[7], "교정완료진행중");
        }
    }
    else if(win->ID==8)
    {
        flash.ponsel_ec.factory = 1;
        flash.ponsel_ec.factoryCount = 0;
        SetWidgetText(gv.handle2[8],"진행중");
    }
    else if(win->ID==9)//Close
    {
        flash.ponsel_ec.calStartFlag=0;
        flash.ponsel_ec.calReceiveFlag=0;
        flash.ponsel_ec.calStep=0;

        unsigned int nData=0;
        char data[100];
        GetWidgetText(gv.handle2[4],data);
        nData = atoi(data);
        flash.ponsel_ec.clearCountSV = nData;

		DataToDisk(&flash, sizeof(__SmartLogdata),"DongMoonFlashData.txt");
        
	    StopTimer(play_timer);  	
		TimerQClear();
		CloseWindow(0, parent);
    }
	PaintDrawControl(ON);
	Gr->Draw(parent, handle, command, param, Data);    
	return(1);
}
int	PONSEL_EC_TimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

//	    float pv;
//    U32 *address;
//    void *addr1; 
//    addr1 = &pv;
//    address = addr1;
	
	
    float   mV=0.0F;
    U32     *addr;
    U8      a, b, c, d;
    void    *addr1; 
	addr1 = &mV;        
    addr = addr1;
    //포트설정에 의해 자동으로 설정되어야한다.
    unsigned int comPort = USART_1;
    
	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	
	char	buf[128];	
	//SetWidgetText(time_textbox, buf);
    if(flash.ponsel_ec.calStartFlag != 0)
    {
        SetWidgetText(gv.handle2[10],"교정 진행중");
        sprintf(buf,"%10.1fmS/cm",flash.ponsel_ec.PV);
        SetWidgetColor(gv.handle2[0],clBlack,gCalColor);
        SetWidgetText(gv.handle2[0],buf);
        gDispCount=0;
    }
    else
    {
        if(gDispCount<9)
        {
            SetWidgetText(gv.handle2[10],"교정 완료");
        }
        else
        {
			if(flash.ponsel_ec.clearWaitCountFlag==ON) {
				SetWidgetText(gv.handle2[10],"세척중");
			} else {
				SetWidgetText(gv.handle2[10],"교정 대기중");
			}
        }
        gDispCount++;
        sprintf(buf,"%10.1fmS/cm",flash.ponsel_ec.PV);
        SetWidgetColor(gv.handle2[0],clBlack,gCalColor);
        SetWidgetText(gv.handle2[0],buf);
    }
    
    if(flash.ponsel_ec.calStartFlag==2 || flash.ponsel_ec.calStartFlag == 3)//전도도보정시작
    {
        unsigned char sumBuf[50];
        char crcBuf[2];
        unsigned short crc=0;
        int address=0;
		
        switch( flash.ponsel_ec.calStep )
        {
            case 0://보정시작명령
            {
                //COMMAND 0C 10 01 4C 00 02 04 00 00 00 00 C1 F6          
                //RECEIVE 0C 10 01 4C 00 02 80 FE    
                Dprintf("CH=%d MODBUS = %02x\n",flash.ponsel_ec.channel,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                gPonEcCommandToggle=0;
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
				gPonEcCommandToggle=0;
            }
            break;
            case 1:
            {
                //COMMAND 0C 10 00 4C 00 01 02 00 01 31 0C       
                //RECEIVE 0C 10 00 4C 00 01 C1 03                                                  
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 2:
            {
                //COMMAND 0C 10 00 AA 00 01 02 00 19 27 00                   
                //RECEIVE 0C 10 00 AA 00 01 20 F4                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0xAA);sumBuf[address++] = 0xAA;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x19);sumBuf[address++] = 0x19;//측정값 누적횟수
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 3:
            {
                //COMMAND 0C 03 00 A6 00 01 65 34                  
                //RECEIVE 0C 03 02 00 00 95 85                                                                   
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0xA6);sumBuf[address++] = 0xA6;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 4:
            {
                //COMMAND 0C 10 00 A6 00 01 02 00 00 E6 06           
                //RECEIVE 0C 10 00 A6 00 01 E0 F7                                                                                    
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0xA6);sumBuf[address++] = 0xA6;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01; Uart_TransmitPort(comPort,0x0F);sumBuf[address++] = 0x0F;break;
					case 1: Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02; Uart_TransmitPort(comPort,0x0F);sumBuf[address++] = 0x0F;break;
					case 2: Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03; Uart_TransmitPort(comPort,0x0F);sumBuf[address++] = 0x0F;break;
					case 3: Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04; Uart_TransmitPort(comPort,0x0F);sumBuf[address++] = 0x0F;break;
				}                
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 5: //측정값 요청 명령
            {
                //COMMAND 0C 03 00 52 00 0B A4 C1       
                //RECEIVE 0C 03 16 00 00 41 C8 54 1F 42 C6 FB 1D 41 03 49 52 41 03 49 52 00 00 00 00 2A E1   
                //Dprintf("comPort = %d Address = %02x\n",comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);               
				Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
				Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
				Uart_TransmitPort(comPort,0x52);sumBuf[address++] = 0x52;
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
				Uart_TransmitPort(comPort,0x0B);sumBuf[address++] = 0x0B;
				crc=crc16(sumBuf,address);
				crcBuf[0]=(crc&0xFF00)>>8;
				crcBuf[1]=crc&0x00FF;
				Uart_TransmitPort(comPort,crcBuf[0]);
				Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 6: //측정값 업데이트하고 다시 측정값 요청 명령 스텝 5로 이동한다.
            {
                //COMMAND 0C 10 00 01 00 01 02 00 1F BE D9        
                //RECEIVE 0C 03 16 00 00 41 C8 54 1F 42 C6 FB 1D 41 03 49 52 41 03 49 52 00 00 00 00 2A E1                                                                                 
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
				Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01; //A6 인경우 교정이 안된다.
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x1F);sumBuf[address++] = 0x1F;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;                      
            case 7: //NEXT 버튼을 누른경우
            {
                //COMMAND 0C 10 02 0A 00 02 04 42 C8 00 00 C4 9A     
                //RECEIVE 0C 10 02 0A 00 02 61 6F                
//제로보정                
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04; break; //200uS 
					case 1: Uart_TransmitPort(comPort,0x08);sumBuf[address++] = 0x08; break; //2000uS
					case 2: Uart_TransmitPort(comPort,0x0C);sumBuf[address++] = 0x0C; break; //20000uS
					case 3: Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10; break; //200000uS
				}
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04;
                
                //전도도버퍼값(0점보정)
                Uart_TransmitPort(comPort,0);sumBuf[address++] = 0;
                Uart_TransmitPort(comPort,0);sumBuf[address++] = 0;
                Uart_TransmitPort(comPort,0);sumBuf[address++] = 0;
                Uart_TransmitPort(comPort,0);sumBuf[address++] = 0;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 8:
            {
                //COMMAND 0C 10 01 4C 00 02 04 00 00 00 20 C0 2E
                //RECEIVE 0C 10 01 4C 00 02 80 FE                                                                                
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 9: //제로보정완료 명령를 대기한다.
            {
                //COMMAND 0C 10 01 4C 00 02 04 00 00 00 20 C0 2E
                //RECEIVE 0C 10 01 4C 00 02 80 FE                                                                                
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04;

				//Dprintf("전도도 교정값 %10.2f  hex %02x %02x %02x %02x\n",flash.ponsel_ec.calSV[0], a, b, c, d);

                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                
//				switch( gCalRagneSet ) {
//					case 0: Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00; break; //2000uS
//					case 1: Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10; break; //2000uS
//					case 2: Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20; break; //20000uS
//					case 3: Uart_TransmitPort(comPort,0x40);sumBuf[address++] = 0x40; break; //200000uS
//				}
                
				crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 10: //측정값 요청 명령
            {
                //COMMAND 0C 03 00 52 00 0B A4 C1       
                //RECEIVE 0C 03 16 00 00 41 C8 54 1F 42 C6 FB 1D 41 03 49 52 41 03 49 52 00 00 00 00 2A E1                                                                                 
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x52);sumBuf[address++] = 0x52;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x0B);sumBuf[address++] = 0x0B;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]); 
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 11: //측정값 업데이트하고 다시 측정값 요청 명령 스텝 10로 이동한다.
            {
                //COMMAND 0C 10 00 01 00 01 02 00 1F BE D9        
                //RECEIVE 0C 03 16 00 00 41 C8 54 1F 42 C6 FB 1D 41 03 49 52 41 03 49 52 00 00 00 00 2A E1                                                                                 
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x1F);sumBuf[address++] = 0x1F;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;                      
            case 12: //2포이트 보정을 완료한다.
            {
                //COMMAND 0C 10 02 0A 00 02 04 42 C8 00 00 C4 9A     
                //RECEIVE 0C 10 02 0A 00 02 61 6F                                                                                 
                //전도도버퍼값
                mV=flash.ponsel_ec.calSV[0];
               
                a= (*addr>>24)&0xFF;     //MSB
                b= (*addr>>16)&0xFF; 
                c= (*addr>>8)&0xFF;  
                d= *addr & 0xFF;         //LSB
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x06);sumBuf[address++] = 0x06; break; //2000uS
					case 1: Uart_TransmitPort(comPort,0x0A);sumBuf[address++] = 0x0A; break; //2000uS
					case 2: Uart_TransmitPort(comPort,0x0E);sumBuf[address++] = 0x0E; break; //20000uS
					case 3: Uart_TransmitPort(comPort,0x12);sumBuf[address++] = 0x12; break; //200000uS
				}
				
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04;

                Uart_TransmitPort(comPort,a);sumBuf[address++] = a;
                Uart_TransmitPort(comPort,b);sumBuf[address++] = b;
                Uart_TransmitPort(comPort,c);sumBuf[address++] = c;
                Uart_TransmitPort(comPort,d);sumBuf[address++] = d;

				crc=crc16(sumBuf,address);
				
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 13:
            {
                //COMMAND 0C 03 02 4C 00 02 05 79    
                //RECEIVE 0C 03 04 00 00 00 00 26 F3                                                                                                                        
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 14: //제로보정완료 명령를 대기한다.
			{
                //COMMAND 0C 10 01 4C 00 02 04 00 00 00 20 C0 2E
                //RECEIVE 0C 10 01 4C 00 02 80 FE                                                                                
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04;
				
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                
				
                
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;Uart_TransmitPort(comPort,0x0C);sumBuf[address++] = 0x0C; break; //2000uS
					case 1: Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;Uart_TransmitPort(comPort,0x30);sumBuf[address++] = 0x30; break; //2000uS
					case 2: Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;Uart_TransmitPort(comPort,0xC0);sumBuf[address++] = 0xC0; break; //20000uS
					case 3: Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00; break; //200000uS
				}
				
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 15:
            {
                //COMMAND 0C 03 02 52 00 02 65 7F        
                //RECEIVE 0C 03 04 40 30 BB 40 40 3C                                                                                                                                                                                                                   
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04;break;
					case 1: Uart_TransmitPort(comPort,0x08);sumBuf[address++] = 0x08;break;
					case 2: Uart_TransmitPort(comPort,0x0C);sumBuf[address++] = 0x0C;break;
					case 3: Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;break;
				}
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 16:
            {
                //COMMAND 0C 03 02 2E 00 02 A4 A7        
                //RECEIVE 0C 03 04 43 19 D6 A0 BC A8                                                                                                                                                                                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                
				
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;break;
					case 1: Uart_TransmitPort(comPort,0x50);sumBuf[address++] = 0x50;break;
					case 2: Uart_TransmitPort(comPort,0x54);sumBuf[address++] = 0x54;break;
					case 3: Uart_TransmitPort(comPort,0x58);sumBuf[address++] = 0x58;break;
				}
				
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 17:
            {
                //COMMAND 0C 03 02 71 00 01 D4 B4         
                //RECEIVE 0C 03 04 43 19 D6 A0 BC A8                                                                                                                                                                                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x28);sumBuf[address++] = 0x28;break;
					case 1: Uart_TransmitPort(comPort,0x2C);sumBuf[address++] = 0x2C;break;
					case 2: Uart_TransmitPort(comPort,0x30);sumBuf[address++] = 0x30;break;
					case 3: Uart_TransmitPort(comPort,0x34);sumBuf[address++] = 0x34;break;
				}				
                
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 18:
            {
                //COMMAND 0C 03 02 71 00 01 D4 B4         
                //RECEIVE 0C 03 04 43 19 D6 A0 BC A8                                                                                                                                                                                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
				                
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x6E);sumBuf[address++] = 0x6E;break;
					case 1: Uart_TransmitPort(comPort,0x70);sumBuf[address++] = 0x70;break;
					case 2: Uart_TransmitPort(comPort,0x72);sumBuf[address++] = 0x72;break;
					case 3: Uart_TransmitPort(comPort,0x74);sumBuf[address++] = 0x74;break;
				}				
                
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 19:
            {
                //COMMAND 0C 03 02 71 00 01 D4 B4         
                //RECEIVE 0C 03 04 43 19 D6 A0 BC A8                                                                                                                                                                                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
				
                
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x06);sumBuf[address++] = 0x06;break;
					case 1: Uart_TransmitPort(comPort,0x0A);sumBuf[address++] = 0x0A;break;
					case 2: Uart_TransmitPort(comPort,0x0E);sumBuf[address++] = 0x0E;break;
					case 3: Uart_TransmitPort(comPort,0x12);sumBuf[address++] = 0x12;break;
				}				
                
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 20:
            {
                //COMMAND 0C 03 02 71 00 01 D4 B4         
                //RECEIVE 0C 03 04 43 19 D6 A0 BC A8                                                                                                                                                                                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                
				
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x4E);sumBuf[address++] = 0x4E;break;
					case 1: Uart_TransmitPort(comPort,0x52);sumBuf[address++] = 0x52;break;
					case 2: Uart_TransmitPort(comPort,0x56);sumBuf[address++] = 0x56;break;
					case 3: Uart_TransmitPort(comPort,0x5A);sumBuf[address++] = 0x5A;break;
				}				
                
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 21:
            {
                //COMMAND 0C 03 02 71 00 01 D4 B4         
                //RECEIVE 0C 03 04 43 19 D6 A0 BC A8                                                                                                                                                                                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
				
                
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x2A);sumBuf[address++] = 0x2A;break;
					case 1: Uart_TransmitPort(comPort,0x2E);sumBuf[address++] = 0x2E;break;
					case 2: Uart_TransmitPort(comPort,0x32);sumBuf[address++] = 0x32;break;
					case 3: Uart_TransmitPort(comPort,0x36);sumBuf[address++] = 0x36;break;
				}				
				
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 22:
            {
                //COMMAND 0C 03 02 71 00 01 D4 B4         
                //RECEIVE 0C 03 04 43 19 D6 A0 BC A8                                                                                                                                                                                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
				
                
				switch( gCalRagneSet ) {
					case 0: Uart_TransmitPort(comPort,0x6F);sumBuf[address++] = 0x6F;break;
					case 1: Uart_TransmitPort(comPort,0x71);sumBuf[address++] = 0x71;break;
					case 2: Uart_TransmitPort(comPort,0x73);sumBuf[address++] = 0x73;break;
					case 3: Uart_TransmitPort(comPort,0x75);sumBuf[address++] = 0x75;break;
				}				
                
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
				SetWidgetText(gv.handle2[7], "교정시작");
            }
            break;
			
            case 23: //종료커멘드
            {
                //COMMAND 0C 03 02 71 00 01 D4 B4         
                //RECEIVE 0C 03 04 43 19 D6 A0 BC A8                                                                                                                                                                                                                                       
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x64);sumBuf[address++] = 0x64;//사용자 ID 중요
                Uart_TransmitPort(comPort,0x64);sumBuf[address++] = 0x64;
                Uart_TransmitPort(comPort,0x64);sumBuf[address++] = 0x64;
				
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                
                Uart_TransmitPort(comPort,0x32);sumBuf[address++] = 0x32;//2
                Uart_TransmitPort(comPort,0x31);sumBuf[address++] = 0x31;//1
                Uart_TransmitPort(comPort,0x31);sumBuf[address++] = 0x31;//1
                Uart_TransmitPort(comPort,0x33);sumBuf[address++] = 0x33;//3
                Uart_TransmitPort(comPort,0x31);sumBuf[address++] = 0x31;//1
                Uart_TransmitPort(comPort,0x37);sumBuf[address++] = 0x37;//7
                Uart_TransmitPort(comPort,0x30);sumBuf[address++] = 0x30;//0
                Uart_TransmitPort(comPort,0x37);sumBuf[address++] = 0x37;//7
                Uart_TransmitPort(comPort,0x32);sumBuf[address++] = 0x32;//2
                Uart_TransmitPort(comPort,0x30);sumBuf[address++] = 0x30;//0
                Uart_TransmitPort(comPort,0x32);sumBuf[address++] = 0x32;//2
                Uart_TransmitPort(comPort,0x31);sumBuf[address++] = 0x31;//1
                
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;
                Uart_TransmitPort(comPort,0x20);sumBuf[address++] = 0x20;

                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
                
                flash.ponsel_ec.calStep=24;
            }
            break;
            case 24:
            {
                //COMMAND 0C 10 01 4C 00 02 04 00 00 00 00 C1 F6       
                //RECEIVE 0C 10 01 4C 00 02 80 FE                                                                                                                                                                                                                                                               
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04;
                
				Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                
				crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
            case 25:
            {
                //COMMAND 0C 10 01 4C 00 02 04 00 00 00 00 C1 F6       
                //RECEIVE 0C 10 01 4C 00 02 80 FE                                                                                                                                                                                                                                                               
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x4C);sumBuf[address++] = 0x4C;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }
            break;
			
        }
        Dprintf("STEP = %d RANGE = %d\n",flash.ponsel_ec.calStep,gCalRagneSet);
    }
    
	
    
    //초기화
    unsigned int address = 0;
    unsigned char sumBuf[50];
    char crcBuf[2];
    unsigned short crc=0;
    
    if(flash.ponsel_ec.factory == 1)
    {
        Dprintf("FACTORY STEP = %d\n",flash.ponsel_ec.factoryCount);
        switch( flash.ponsel_ec.factoryCount )
        {
            case 0:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 1:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                Uart_TransmitPort(comPort,0x06);sumBuf[address++] = 0x06;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 2:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 3:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x04);sumBuf[address++] = 0x04;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 4:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x05);sumBuf[address++] = 0x05;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 5:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x06);sumBuf[address++] = 0x06;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 6:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x07);sumBuf[address++] = 0x07;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 7:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x08);sumBuf[address++] = 0x08;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 8:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x09);sumBuf[address++] = 0x09;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 9:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x0A);sumBuf[address++] = 0x0A;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 10:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x0B);sumBuf[address++] = 0x0B;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 11:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x0C);sumBuf[address++] = 0x0C;
                Uart_TransmitPort(comPort,0x8E);sumBuf[address++] = 0x8E;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x10);sumBuf[address++] = 0x10;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 12:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0xD6);sumBuf[address++] = 0xD6;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 13:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0xC2);sumBuf[address++] = 0xC2;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 14:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0xD8);sumBuf[address++] = 0xD8;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x02);sumBuf[address++] = 0x02;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
            case 15:
            {
                Uart_TransmitPort(comPort,flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel]);
                sumBuf[address++] = flash.ponsel_ec.modbusAddress[flash.ponsel_ec.channel];
                Uart_TransmitPort(comPort,0x03);sumBuf[address++] = 0x03;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0xC3);sumBuf[address++] = 0xC3;
                Uart_TransmitPort(comPort,0x00);sumBuf[address++] = 0x00;
                Uart_TransmitPort(comPort,0x01);sumBuf[address++] = 0x01;
                crc=crc16(sumBuf,address);
                crcBuf[0]=(crc&0xFF00)>>8;
                crcBuf[1]=crc&0x00FF;
                Uart_TransmitPort(comPort,crcBuf[0]);
                Uart_TransmitPort(comPort,crcBuf[1]);
            }break;
        }
        
        
        flash.ponsel_ec.factoryCount++;
        if(flash.ponsel_ec.factoryCount == 16)
        {
            flash.ponsel_ec.factory = 0;
            flash.ponsel_ec.factoryCount = 0;
            SetWidgetText(gv.handle2[8],"시작");
        }    
    
    }
    PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	PONSEL_EC_CreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}
	char data[256];
	U32 fontColor = clWhite, fontBackColor = clBlack;
    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy

	Gr->SetColor(clBlack);
	Gr->FillRect(0, 0, win->width-2, win->height-2);

    gCalColor = clWhite;//보정시작전컬러
	//타이틀바를 그린다.
	TitleBar(handle, 0, 0, win->width, 50, 1, "SH-PCTZB-0628 (전도도센서)", clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	
	//디스플레이용 바탕패널을 그린다.
	//SystemPanel(handle, 20, 70, 800-40, 130, 1, "", clYellow, 1, RIGHT_ALIGN, 3, NULL);	
	
	//측정값 디스플레이용 텍스트박스
	gv.handle2[10]=SLabel(handle, 300, 80, 200, 30, "교정 대기중", fontColor,fontBackColor,1);
	sprintf(data,"%10.1fmS/cm",0.0);
	gv.handle2[0] = STextBox(handle, 60, 80+30, 680, 70, 48, data, clWhite, clBlack, CENTER_ALIGN, READ_ONLY, 0, NULL);	//RW_MODE

	//버튼을 만든다.
    //채널선택버튼
    flash.ponsel_ec.calSV[0] = 50.0f;
    flash.ponsel_ec.calReceiveFlag=0;//명령수신상태
    flash.ponsel_ec.calStep=0;//명령스텝
    gDispCount=10;//문자를 변경한다.
    gCleanFlag=0;//세척
    //SetWidgetStatus(gv.handle2[1],ON);
    flash.ponsel_ec.channel=0;//현재채널
    flash.ponsel_ec.calStartFlag=0;//보정시작플래그
    //채널주소값고정
    
    #if defined (PONSEL_EC)
    flash.ponsel_ec.modbusAddress[0]=0x0A;
    flash.ponsel_ec.modbusAddress[1]=0x0B;
    flash.ponsel_ec.modbusAddress[2]=0x0C;
    flash.ponsel_ec.modbusAddress[3]=0x0D;
    flash.ponsel_ec.modbusAddress[4]=0x0E;
    #endif
    
    SLabel(handle, 30, 220, 200, 64, "채널변경 / RANGE",fontColor,fontBackColor,1);
    //gv.handle2[1] = NButton(handle,  30, 220, 200, 64, "48G_Search_Icon2.bmp", "채널1",  PUSH_BUTTON, 1, PONSEL_EC_KeyEvent);	
    gv.handle2[2] = NButton(handle, 310, 220, 200, 64, " ", "채널1",  PUSH_BUTTON, 2, PONSEL_EC_KeyEvent);		
    gv.handle2[3] = NButton(handle, 550, 220, 200, 64, " ", " ",  PUSH_BUTTON, 3, PONSEL_EC_KeyEvent);	
	gCalRagneSet=0;
	switch( gCalRagneSet ) {
		case 0:
			SetWidgetText(gv.handle2[3],"0/100mS/cm");
			break;
		case 1:
			SetWidgetText(gv.handle2[3],"0/700mS/cm");
			break;
		case 2:
			SetWidgetText(gv.handle2[3],"0/20000uS/cm");
			break;
		case 3:
			SetWidgetText(gv.handle2[3],"0/200000uS/cm");
			break;
	}
    //gv.handle2[4] = NButton(handle, 550, 220, 200, 64, "48G_Search_Icon2.bmp", "채널4",  PUSH_BUTTON, 4, PONSEL_EC_KeyEvent);		
    
    //보정항목이름출력
    SLabel(handle, 30, 220+80, 200, 64, "센서세척주기(분)",fontColor,fontBackColor,1);
    SLabel(handle, 10, 220+80+80, 240, 64, "교정 전 스펜설정 mS/cm",fontColor,fontBackColor,1);
    SLabel(handle, 30, 220+80+80+80, 200, 64, "교정초기화",fontColor,fontBackColor,1);
    
	//보정설정값
    
    sprintf(data,"%d",flash.ponsel_ec.clearCountSV);
	gv.handle2[4] = NButton(handle, 310, 220+80, 200, 64, " ", data,  PUSH_BUTTON, 4, PONSEL_EC_KeyEvent);			
    gv.handle2[5] = SButton(handle, 550, 220+80, 200, 64, " ", "세척시작",  PUSH_BUTTON, 5, PONSEL_EC_KeyEvent);
    //sprintf(data,"%5.1f'C",flash.ponsel_ec.oxygenSV[0]);
    sprintf(data,"%10.2f",flash.ponsel_ec.calSV[0]);
	gv.handle2[6] = NButton(handle, 310, 220+80+80, 200, 64, " ", data,  PUSH_BUTTON, 6, PONSEL_EC_KeyEvent);			
    gv.handle2[8] = NButton(handle, 310, 220+80+80+80, 200, 64, " ", "시작",  PUSH_BUTTON, 8, PONSEL_EC_KeyEvent);
	//보정시작버튼
				
	gv.handle2[7] = SButton(handle, 550, 220+80+80, 200, 64, " ", "교정시작",  PUSH_BUTTON, 7, PONSEL_EC_KeyEvent);			
	//Close Button
	SButton(handle, 550, 220+80+80+80, 200, 64, " ", "EXIT",  PUSH_BUTTON, 9, PONSEL_EC_KeyEvent);			
	
	PaintDrawControl(ON);	
	Gr->Draw(parent, handle, command, param, Data);
    
	play_timer =  OpenTimer(parent, handle, 500);	
    StartTimer(play_timer);  	
    
	return(1);
}

int	PONSEL_EC_PaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle," ");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s Object Err Win=%04x Gr=%04x\n", __FUNCTION__, win, Gr);
		//!< 객체를 얻지못하면 이벤트를 처리할수 없다.
		return(0);
	}

    PaintDrawControl(OFF);	
	Gr->FillRect(0, 0, 0, 0);//Dummy
	PaintDrawControl(ON);		
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	PONSEL_EC_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	//윈도우가 닫힐때 필요한처리를 한다.
	Windows *win=NULL;

	switch(handle) //컨테이너윈도우 
	{
		case 1: 	win = gp_WidgetWindow_1;	break;
		case 2: 	win = gp_WidgetWindow_2;	break;
		case 3: 	win = gp_WidgetWindow_3;	break;
		case 4: 	win = gp_WidgetWindow_4;	break;
		case 5: 	win = gp_WidgetWindow_5;	break;
		case 6: 	win = gp_LcdAlphaWidgetWindow;	break; //
		case 7: 	win = gp_VideoAlphaWidgetWindow;	break; //
		default:	Dprintf("FindWidgetChain::Windows_no mistyping\n"); return(0);
	}

	if(win== NULL) 
	{
		Dprintf("TabelTest_CloseEvent::Invalid Handle\n");
		return (NULL);
	}
	int Done=1;
	while(Done) //위젯윈도우를 모두찾아 CloseEvent를 전송한다.
	{
		if(win->CloseEvent != NULL) win->CloseEvent(0, 0, 0, 0, 0);
		if(win->NextWindow ==NULL) return(NULL);
		win = (Windows *)win->NextWindow;
	}	
	return(1);
}

int	PONSEL_EC_MouseOnEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Graphics 	*Gr= GetGraphics(parent, handle, CLIENT_AREA);
	Windows 	*win = GetWindow(parent, handle,"");;

	if(win==0 || Gr==0)
	{
		Dprintf("%s->%s Object Err Win=%04x Gr=%04x\n", __FILE__, __FUNCTION__, win, Gr);
		return(0);
	}
 	//Dummy
	Gr->FillRect(0, 0, 0, 0);	
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int PONSEL_EC_Window(void) {
	if(GetWindowName("PONSEL_EC_Window")) 
	{
		Dprintf("PONSEL_EC_Window:: Windows already Create\n");
		return 0;		
	}	
	int sx = (LCD_X - 800) / 2;
	int sy = (LCD_Y - 550) / 2;	
	makeWindow(
				LCD,					// Display Section Position
				"PONSEL_EC_Window",		// Windows Name
				DESKTOP_WINDOW,		// Parent Window Handle
				CONTAINER_WINDOW,	// Windows Kind
				STYLE_NONE,			// Windows Style, TEECO_STYLE
				sx, sy, 			// Start Position
				800, 550,				// Size
				SHOW,					// Create after Show
				PONSEL_EC_CreateEvent,		// CreateEvent
				PONSEL_EC_PaintEvent,		// PaintEvent
				PONSEL_EC_MouseOnEvent,	// MouseOnEvent
				NULL,	// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				PONSEL_EC_TimerEvent,		// TimerEvent
				NULL,  					// ControlEvent
				NULL,					// ChangeEvent
				NULL,
				NULL,
				NULL,
				PONSEL_EC_CloseEvent,
				NULL,					// PARAMETER
				NULL,					// Back Image
				NULL,					// 
				NULL,					// Windows Text
				MODALLESS_WINDOW,
				NULL
				);
	return(1);
}
#endif
