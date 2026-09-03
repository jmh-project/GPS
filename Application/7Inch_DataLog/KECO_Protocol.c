
#include "TEECO_System.h"
#include "DataLog.h"

#ifdef KECO_PROTOCOL_ENABLE
extern  U32 gOpenFileSize;
extern U32 RE_TRAMSMIT_ADDRESS;
extern int DataLogTransmitFileToSDCard(RealTime saveTime);
extern int DUMP_SDCardTORAM(RealTime startTime, RealTime endTime);
extern int Uart_TransmitPort( unsigned int uartPort, unsigned char data );

int KECO_DataReceive( char ascii, int comPort ) {//KECO 데이터수신
	char date_buf[16];
	if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_RESPONSE) {
		if(ascii==0x06) {									//데이터 송수신 완료
			if(gv.tmsDumpStepCnt!=0) {					//남은 덤프 데이터를 송신한다.
				gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA_RESPONSE;
			}				
		}
		if(ascii==0x15) { 							        //데이터 재송신을 3회까지 실시한다.
			gv.tmsReSendCount++;
			if(gv.tmsReSendCount>=3) {
				gv.tmsReSendCount=0;
				gv.nFlag_ReceiveDataProcess[comPort]=0;
			}
		}
	}
	if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_DUMA_RESPONSE) {
		if(ascii==0x06) {
			gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA;
			gv.tmsReSendCount=0;								//재송신카운트				
		} else if(ascii==0x15) {
			gv.tmsDumpStepCnt=4;								//재송신한다. (램주소가 업데이트안된다.)
			gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA;
			gv.tmsReSendCount++;
			if(gv.tmsReSendCount>=3) {
				gv.tmsReSendCount=0;
                gv.tmsDumpStepCnt=0;
				gv.nFlag_ReceiveDataProcess[comPort]=0;
			}
		}
	}
	if(ascii==0x02)
	{
		gv.nFlag_ReceiveStart[comPort]=1;
		gv.nReceiveBufferCount[comPort]=0;
	}
	else if(gv.nFlag_ReceiveStart[comPort]==1)
	{
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++]=ascii;
		if(ascii==0x0d) {     
			//덤프요청
                 if(gv.aItemReceiveBuffer[comPort][0]=='D' && gv.aItemReceiveBuffer[comPort][1]=='U' && gv.aItemReceiveBuffer[comPort][2]=='M' && gv.aItemReceiveBuffer[comPort][3]=='A') {
				gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA;

				date_buf[0] =gv.aItemReceiveBuffer[comPort][6];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][7];
				date_buf[2] = 0;
				gv.dumpStartDate.year = atoi(date_buf);
				
				date_buf[0] =gv.aItemReceiveBuffer[comPort][8];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][9];
				date_buf[2] = 0;
				gv.dumpStartDate.month = atoi(date_buf);           

				date_buf[0] =gv.aItemReceiveBuffer[comPort][10];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][11];
				date_buf[2] = 0;
				gv.dumpStartDate.date = atoi(date_buf);           

				date_buf[0] =gv.aItemReceiveBuffer[comPort][12];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][13];
				date_buf[2] = 0;
				gv.dumpStartDate.hour = atoi(date_buf);           

				date_buf[0] =gv.aItemReceiveBuffer[comPort][14];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][15];
				date_buf[2] = 0;
				gv.dumpStartDate.minute = atoi(date_buf);           

				Dprintf("덤프요청시작시간 %d/%d/%d %d:%d:%d\n"
				,gv.dumpStartDate.year
				,gv.dumpStartDate.month
				,gv.dumpStartDate.date
				,gv.dumpStartDate.hour
				,gv.dumpStartDate.minute
				,0
				);

				//종료시간
				date_buf[0] =gv.aItemReceiveBuffer[comPort][20];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][21];
				date_buf[2] = 0;
				gv.dumpEndDate.year = atoi(date_buf);
				
				date_buf[0] =gv.aItemReceiveBuffer[comPort][22];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][23];
				date_buf[2] = 0;
				gv.dumpEndDate.month = atoi(date_buf);                

				date_buf[0] =gv.aItemReceiveBuffer[comPort][24];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][25];
				date_buf[2] = 0;
				gv.dumpEndDate.date = atoi(date_buf);                

				date_buf[0] =gv.aItemReceiveBuffer[comPort][26];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][27];
				date_buf[2] = 0;
				gv.dumpEndDate.hour = atoi(date_buf);                

				date_buf[0] =gv.aItemReceiveBuffer[comPort][28];    
				date_buf[1] =gv.aItemReceiveBuffer[comPort][29];
				date_buf[2] = 0;
				gv.dumpEndDate.minute = atoi(date_buf);

				Dprintf("덤프요청종료시간 %d/%d/%d %d:%d:%d\n"
				,gv.dumpEndDate.year
				,gv.dumpEndDate.month
				,gv.dumpEndDate.date
				,gv.dumpEndDate.hour
				,gv.dumpEndDate.minute
				,0
				);
				gv.tmsDumpStepCnt=2;//최초1회송신용
				gv.tmsReSendCount=0;
			}
			//데이터요청
			else if(gv.aItemReceiveBuffer[comPort][0]=='D' && gv.aItemReceiveBuffer[comPort][1]=='A' && gv.aItemReceiveBuffer[comPort][2]=='T' && gv.aItemReceiveBuffer[comPort][3]=='A') {
				gv.nFlag_ReceiveDataProcess[comPort] = KECO_DATA;
			}
			//시간변경
			else if(gv.aItemReceiveBuffer[comPort][0]=='S' && gv.aItemReceiveBuffer[comPort][1]=='E' && gv.aItemReceiveBuffer[comPort][2]=='T' && gv.aItemReceiveBuffer[comPort][3]=='T') {
				unsigned char Data[10];
				int key_value=0;
				uint8_t 	data;
				
				Data[0] =gv.aItemReceiveBuffer[comPort][4];
				Data[1] =gv.aItemReceiveBuffer[comPort][5];
				Data[2] =gv.aItemReceiveBuffer[comPort][6];
				Data[3] =gv.aItemReceiveBuffer[comPort][7];
				Data[4] = 0;
				
				key_value = atoi((char *)Data);
				if(key_value < 2000 || key_value > 2099)
				{
					//Info_Window("Range Error", "Year Input Error");
				}
				else
				{
					data = IntToBCD(key_value-2000);
					DS1302_YearSet(data);
				}	
				Data[0] =gv.aItemReceiveBuffer[comPort][8];
				Data[1] =gv.aItemReceiveBuffer[comPort][9];
				Data[2] = 0;
				key_value = atoi((char *)Data);
				if(key_value < 1 || key_value > 12)
				{
					//Info_Window("Range Error", "Month Input Error");
				}
				else
				{
					data = IntToBCD(key_value);
					DS1302_MonthSet(data);
				}	
				Data[0] =gv.aItemReceiveBuffer[comPort][10];
				Data[1] =gv.aItemReceiveBuffer[comPort][11];
				Data[2] = 0;
				key_value = atoi((char *)Data);
				if(key_value < 1 || key_value > 31)
				{
					//Info_Window("Range Error", "Day Input Error");
				}
				else
				{
					data = IntToBCD(key_value);
					DS1302_DaySet(data);
				}	
				Data[0] =gv.aItemReceiveBuffer[comPort][12];
				Data[1] =gv.aItemReceiveBuffer[comPort][13];
				Data[2] = 0;
				key_value = atoi((char *)Data);
				if(key_value < 0 || key_value > 24)
				{
					//Info_Window("Range Error", "Hour Input Error");
				}
				else
				{
					data = IntToBCD(key_value);
					DS1302_HourSet(data);
				}	
				Data[0] =gv.aItemReceiveBuffer[comPort][14];
				Data[1] =gv.aItemReceiveBuffer[comPort][15];
				Data[2] = 0;
				key_value = atoi((char *)Data);
				if(key_value < 0 || key_value > 59)
				{
					//Info_Window("Range Error", "Minute Input Error");
				}
				else
				{
					data = IntToBCD(key_value);
					DS1302_MinSet(data);
				}	
				Data[0] =gv.aItemReceiveBuffer[comPort][16];
				Data[1] =gv.aItemReceiveBuffer[comPort][17];
				Data[2] = 0;
				key_value = atoi((char *)Data);
				if(key_value < 0 || key_value > 59)
				{
					//Info_Window("Range Error", "Second Input Error");
				}
				else
				{
					data = IntToBCD(key_value);
					DS1302_SecSet(data);
				}
                Uart_TransmitPort( comPort, 0x06);//eot send                
			}
			//비밀번호변경
			else if(gv.aItemReceiveBuffer[comPort][0]=='S' && gv.aItemReceiveBuffer[comPort][1]=='E' && gv.aItemReceiveBuffer[comPort][2]=='T' && gv.aItemReceiveBuffer[comPort][3]=='P') {
					flash.password[0]=gv.aItemReceiveBuffer[comPort][4];
					flash.password[1]=gv.aItemReceiveBuffer[comPort][5];
					flash.password[2]=gv.aItemReceiveBuffer[comPort][6];
					flash.password[3]=gv.aItemReceiveBuffer[comPort][7];
					flash.password[4]=gv.aItemReceiveBuffer[comPort][8];
					flash.password[5]=gv.aItemReceiveBuffer[comPort][9];
					flash.password[6]=gv.aItemReceiveBuffer[comPort][10];
					flash.password[7]=gv.aItemReceiveBuffer[comPort][11];
					flash.password[8]=gv.aItemReceiveBuffer[comPort][12];
					flash.password[9]=gv.aItemReceiveBuffer[comPort][13];
			}
			//자료 수집기 리셋
			else if(gv.aItemReceiveBuffer[comPort][0]=='R' && gv.aItemReceiveBuffer[comPort][1]=='S' && gv.aItemReceiveBuffer[comPort][2]=='E' && gv.aItemReceiveBuffer[comPort][3]=='T') {
				gv.nFlag_ReceiveDataProcess[comPort] = KECO_RESET;
			}
			gv.nFlag_ReceiveStart[comPort]=0;
			gv.nReceiveBufferCount[comPort]=0;            
		}
		else
		{
			if(gv.nReceiveBufferCount[comPort]>200)
			{
				gv.nFlag_ReceiveDataProcess[comPort]=0;
				gv.nFlag_ReceiveStart[comPort]=0;
				gv.nReceiveBufferCount[comPort]=0;
			}
		}
	}
    return (1);
}
unsigned char KECO_Status( unsigned int uartPort, unsigned char sum) { //KECO 상태값
	char		sum1=sum;
	if(gv.tmsStatusCode[uartPort]==6)
	{
		Uart_TransmitPort(USART_4, '0'); //동작불량 
		Uart_TransmitPort(USART_4, '6');
		sum1+='0';
		sum1+='6';
	}
	else
	{
		if(gv.tmsStatusCode[uartPort]==3)
		{
			Uart_TransmitPort(USART_4, '0'); //교정중
			Uart_TransmitPort(USART_4, '3');
			sum1+='0';
			sum1+='3';
		}
		else 
		{
			Uart_TransmitPort(USART_4, '0'); //정상  
			Uart_TransmitPort(USART_4, '0');
			sum1+='0';
			sum1+='0';
		}
	}
	return (sum1);
}
int KECO_ProtocolSending(int comPort) {//KECO 순간측정자료송신
	S8 			        abuf[30];
	unsigned int		cSum=0;
	char		    	sumbuf[5];
	int					c=0;//, i=0;
	int					itemCount=0;//송신 아이템 수
	int				    itemNo=0;//송신 순서
	//float		    	fdata=0.0f;
    itemCount=itemNo;
	
	//1. 문장의시작 1byte
    Uart_TransmitPort( comPort, 0x02);	
	//2. 명령어 4byte
	Uart_TransmitPort( comPort, 'D');	
    Uart_TransmitPort( comPort, 'A');
    Uart_TransmitPort( comPort, 'T');
    Uart_TransmitPort( comPort, 'A');
	cSum = 0x02;
	cSum += 'D'+'A'+'T'+'A';
	//3. 측정일시 14byte
	sprintf(abuf,"20%02d%02d%02d%02d%02d%02d"
    ,currentDate.year,currentDate.month,currentDate.date,currentDate.hour,currentDate.minute,currentDate.sec);
	Uart_TransmitPort( comPort,  abuf[0] );
	cSum+=abuf[0];
	Uart_TransmitPort( comPort,  abuf[1] );
	cSum+=abuf[1];
	Uart_TransmitPort( comPort,  abuf[2] );
	cSum+=abuf[2];
	Uart_TransmitPort( comPort,  abuf[3] );
	cSum+=abuf[3];
	Uart_TransmitPort( comPort,  abuf[4] );
	cSum+=abuf[4];
	Uart_TransmitPort( comPort,  abuf[5] );
	cSum+=abuf[5];
	Uart_TransmitPort( comPort,  abuf[6] );
	cSum+=abuf[6];
	Uart_TransmitPort( comPort,  abuf[7] );
	cSum+=abuf[7];
	Uart_TransmitPort( comPort,  abuf[8] );
	cSum+=abuf[8];
	Uart_TransmitPort( comPort,  abuf[9] );
	cSum+=abuf[9];
	Uart_TransmitPort( comPort,  abuf[10] );
	cSum+=abuf[10];
	Uart_TransmitPort( comPort,  abuf[11] );
	cSum+=abuf[11];
	Uart_TransmitPort( comPort,  abuf[12] );
	cSum+=abuf[12];
	Uart_TransmitPort( comPort,  abuf[13] );
	cSum+=abuf[13];	
	//4. 항목수 2byte
    itemCount=0;
	itemNo=0;
	itemCount = flash.Uart[USART_1]->ItemCnt;
	sprintf(sumbuf,"%02d", itemCount);
	Uart_TransmitPort( comPort, sumbuf[0]);
	Uart_TransmitPort( comPort, sumbuf[1]);
	cSum+=sumbuf[0];
	cSum+=sumbuf[1];	

	for(int i=0; i<itemCount; i++) {
		//5-1. 항목코드 5byte TEM00
		Uart_TransmitPort( comPort, flash.tmsDataName[i][0]);cSum+=flash.tmsDataName[i][0];
		Uart_TransmitPort( comPort, flash.tmsDataName[i][1]);cSum+=flash.tmsDataName[i][1];
		Uart_TransmitPort( comPort, flash.tmsDataName[i][2]);cSum+=flash.tmsDataName[i][2];
		Uart_TransmitPort( comPort, flash.tmsDataName[i][3]);cSum+=flash.tmsDataName[i][3];
		Uart_TransmitPort( comPort, flash.tmsDataName[i][4]);cSum+=flash.tmsDataName[i][4];
		//5-2. 측정값
		if(flash.floatPoint[i]==0) {
			sprintf(abuf,"%-10.0f",flash.Uart[USART_1]->SensorPV[i]);
		}
		else if(flash.floatPoint[i]==1) {
			sprintf(abuf,"%-10.1f",flash.Uart[USART_1]->SensorPV[i]);
		}
		else if(flash.floatPoint[i]==2) {
			sprintf(abuf,"%-10.2f",flash.Uart[USART_1]->SensorPV[i]);
		}
		else if(flash.floatPoint[i]==3) {
			sprintf(abuf,"%-10.3f",flash.Uart[USART_1]->SensorPV[i]);
		}
		else if(flash.floatPoint[i]==4) {
			sprintf(abuf,"%-10.4f",flash.Uart[USART_1]->SensorPV[i]);
		}
		for(c=0;c<10;c++) {
			Uart_TransmitPort( comPort, abuf[c]); 
			cSum += abuf[c];
		}
		//5-3. 상태정보
		cSum=KECO_Status( USART_1, cSum);
		//Factor    
		Uart_TransmitPort( comPort, '0');cSum+='0';	
		Uart_TransmitPort( comPort, '0');cSum+='0';		
	}
	#if 0
	//5-1. 항목코드 5byte TEM00
	Uart_TransmitPort( comPort, flash.tmsDataName[0][0]);cSum+=flash.tmsDataName[0][0];
	Uart_TransmitPort( comPort, flash.tmsDataName[0][1]);cSum+=flash.tmsDataName[0][1];
	Uart_TransmitPort( comPort, flash.tmsDataName[0][2]);cSum+=flash.tmsDataName[0][2];
	Uart_TransmitPort( comPort, flash.tmsDataName[0][3]);cSum+=flash.tmsDataName[0][3];
	Uart_TransmitPort( comPort, flash.tmsDataName[0][4]);cSum+=flash.tmsDataName[0][4];
	//5-2. 측정값
	sprintf(abuf,"%10.3f",flash.Uart[USART_1]->SensorPV[0]);
	//sprintf(abuf,"%10.3f",1.0);
	for(c=0;c<10;c++) {
		Uart_TransmitPort( comPort, abuf[c]); 
		cSum += abuf[c];
	}
	//5-3. 상태정보
	cSum=KECO_Status( USART_1, cSum);
	
    //Factor    
	Uart_TransmitPort( comPort, '0');cSum+='0';	
	Uart_TransmitPort( comPort, '0');cSum+='0';		

	//항목코드 CON00
	Uart_TransmitPort( comPort, flash.tmsDataName[1][0]);cSum+=flash.tmsDataName[1][0];
	Uart_TransmitPort( comPort, flash.tmsDataName[1][1]);cSum+=flash.tmsDataName[1][1];
	Uart_TransmitPort( comPort, flash.tmsDataName[1][2]);cSum+=flash.tmsDataName[1][2];
	Uart_TransmitPort( comPort, flash.tmsDataName[1][3]);cSum+=flash.tmsDataName[1][3];
	Uart_TransmitPort( comPort, flash.tmsDataName[1][4]);cSum+=flash.tmsDataName[1][4];
	//측정값
	sprintf(abuf,"%10.3f",flash.Uart[USART_1]->SensorPV[1]);
	//sprintf(abuf,"%10.3f",2.0);
	for(c=0;c<10;c++) {
		Uart_TransmitPort( comPort, abuf[c]); 
		cSum += abuf[c];
	}
	//상태정보
	cSum=KECO_Status( USART_1, cSum);
    //Factor    
	Uart_TransmitPort( comPort, '0');cSum+='0';	
	Uart_TransmitPort( comPort, '0');cSum+='0';		

	
	//항목코드 DOW00
	Uart_TransmitPort( comPort, flash.tmsDataName[2][0]);cSum+=flash.tmsDataName[2][0];
	Uart_TransmitPort( comPort, flash.tmsDataName[2][1]);cSum+=flash.tmsDataName[2][1];
	Uart_TransmitPort( comPort, flash.tmsDataName[2][2]);cSum+=flash.tmsDataName[2][2];
	Uart_TransmitPort( comPort, flash.tmsDataName[2][3]);cSum+=flash.tmsDataName[2][3];
	Uart_TransmitPort( comPort, flash.tmsDataName[2][4]);cSum+=flash.tmsDataName[2][4];
	//측정값
	sprintf(abuf,"%10.3f",flash.Uart[USART_1]->SensorPV[2]);
	//sprintf(abuf,"%10.3f",3.0);
	for(c=0;c<10;c++) {
		Uart_TransmitPort( comPort, abuf[c]); 
		cSum += abuf[c];
	}
	//상태정보
	cSum=KECO_Status( USART_1, cSum);
    //Factor    
	Uart_TransmitPort( comPort, '0');cSum+='0';	
	Uart_TransmitPort( comPort, '0');cSum+='0';		

	//항목코드 PHY00
	Uart_TransmitPort( comPort, flash.tmsDataName[3][0]);cSum+=flash.tmsDataName[3][0];
	Uart_TransmitPort( comPort, flash.tmsDataName[3][1]);cSum+=flash.tmsDataName[3][1];
	Uart_TransmitPort( comPort, flash.tmsDataName[3][2]);cSum+=flash.tmsDataName[3][2];
	Uart_TransmitPort( comPort, flash.tmsDataName[3][3]);cSum+=flash.tmsDataName[3][3];
	Uart_TransmitPort( comPort, flash.tmsDataName[3][4]);cSum+=flash.tmsDataName[3][4];
	//측정값
	sprintf(abuf,"%10.3f",flash.Uart[USART_1]->SensorPV[3]);
	//sprintf(abuf,"%10.3f",4.0);

	for(c=0;c<10;c++) {
		Uart_TransmitPort( comPort, abuf[c]); 
		cSum += abuf[c];
	}
	//상태정보
	cSum=KECO_Status( USART_1, cSum);
    //Factor    
	Uart_TransmitPort( comPort, '0');cSum+='0';	
	Uart_TransmitPort( comPort, '0');cSum+='0';		

	
	//항목코드 SUS00
	Uart_TransmitPort( comPort, flash.tmsDataName[4][0]);cSum+=flash.tmsDataName[4][0];
	Uart_TransmitPort( comPort, flash.tmsDataName[4][1]);cSum+=flash.tmsDataName[4][1];
	Uart_TransmitPort( comPort, flash.tmsDataName[4][2]);cSum+=flash.tmsDataName[4][2];
	Uart_TransmitPort( comPort, flash.tmsDataName[4][3]);cSum+=flash.tmsDataName[4][3];
	Uart_TransmitPort( comPort, flash.tmsDataName[4][4]);cSum+=flash.tmsDataName[4][4];
	//측정값
	sprintf(abuf,"%10.3f",flash.Uart[USART_1]->SensorPV[4]);
	//sprintf(abuf,"%10.3f",5.0);
	
	for(c=0;c<10;c++) {
		Uart_TransmitPort( comPort, abuf[c]); 
		cSum += abuf[c];
	}
	//상태정보
	cSum=KECO_Status( comPort, cSum);
    //Factor    
	Uart_TransmitPort( comPort, '0');cSum+='0';	
	Uart_TransmitPort( comPort, '0');cSum+='0';		
	
	#endif
	for(c=0;c<50;c++) {		//부가정보
		Uart_TransmitPort( comPort, 0x20);
		cSum+=0x20;
	}	
	Uart_TransmitPort( comPort, 0x03);				//ETX
	cSum+=0x03;

	Uart_TransmitPort( comPort, (cSum&0xF0)>>4|0x30);
	Uart_TransmitPort( comPort, (cSum&0x0F)|0x30);

	Uart_TransmitPort( comPort, 0x0d);
	return TRUE;
}
int KECO_DUMA_FileCreate(int comPort) {//5분 덤프데이터저장 //덤프요청 DUMA2020092816500020200928165500
	char 	abuf[32];
	unsigned int 	cSum=0;
	char		    sumbuf[5];
	int				c=0, i=0;
	int				nBufferCount=0;
	int				itemCount=0;//송신 아이템 수
	int	            itemNo=0;//송신 순서
	//float		    fdata=0.0f;
	int				address=0;
	unsigned int 	saveDateInt=0;

    nBufferCount=itemNo;
    nBufferCount=nBufferCount;
    
	for(i=0;i<SMARTLOG_DATALOG_SIZE;i++) {
		gv.dataLogBuf[i]=0;
	}

    for(i=0;i<32;i++)abuf[i]=0;
	/**
	*	측정값을 찾기위한 설정값
	*/
	gv.dataLogBuf[address++]='*';	
    saveDateInt = (flash.saveDate.year*599040) + (flash.saveDate.month*46080) + (flash.saveDate.date*1440) + (flash.saveDate.hour*60) + (flash.saveDate.minute*1);
	sprintf(abuf,"%010d",saveDateInt);
	gv.dataLogBuf[address++] = abuf[0];
	gv.dataLogBuf[address++] = abuf[1];
	gv.dataLogBuf[address++] = abuf[2];
	gv.dataLogBuf[address++] = abuf[3];
	gv.dataLogBuf[address++] = abuf[4];
	gv.dataLogBuf[address++] = abuf[5];
	gv.dataLogBuf[address++] = abuf[6];
	gv.dataLogBuf[address++] = abuf[7];
	gv.dataLogBuf[address++] = abuf[8];
	gv.dataLogBuf[address++] = abuf[9];
	
	gv.dataLogBuf[address++] = 0x02;
	gv.dataLogBuf[address++] = 'D';
	gv.dataLogBuf[address++] = 'U';
	gv.dataLogBuf[address++] = 'M';
	gv.dataLogBuf[address++] = 'P';
	
	/**
	*	명령어
	*/
	for(i=0;i<32;i++)abuf[i]=0;	
	cSum = 0x02;
	cSum += 'D'+'U'+'M'+'P';	
	nBufferCount = 0;//송신데이터 길이를 계산한다.

	sprintf(abuf,"20%02d%02d%02d%02d%02d%02d",flash.saveDate.year,flash.saveDate.month,flash.saveDate.date,flash.saveDate.hour,flash.saveDate.minute,0);
	
	gv.dataLogBuf[address++] = abuf[0];
	gv.dataLogBuf[address++] = abuf[1];
	gv.dataLogBuf[address++] = abuf[2];
	gv.dataLogBuf[address++] = abuf[3];

	gv.dataLogBuf[address++] = abuf[4];
	gv.dataLogBuf[address++] = abuf[5];

	gv.dataLogBuf[address++] = abuf[6];
	gv.dataLogBuf[address++] = abuf[7];

	gv.dataLogBuf[address++] = abuf[8];
	gv.dataLogBuf[address++] = abuf[9];

	gv.dataLogBuf[address++] = abuf[10];
	gv.dataLogBuf[address++] = abuf[11];

	gv.dataLogBuf[address++] = abuf[12];
	gv.dataLogBuf[address++] = abuf[13];

	cSum+=abuf[0];
	cSum+=abuf[1];
	cSum+=abuf[2];
	cSum+=abuf[3];
	cSum+=abuf[4];
	cSum+=abuf[5];
	cSum+=abuf[6];
	cSum+=abuf[7];
	cSum+=abuf[8];
	cSum+=abuf[9];
	cSum+=abuf[10];
	cSum+=abuf[11];
	cSum+=abuf[12];
	cSum+=abuf[13];	

	/**
	*	측정항목 수
	*/
    itemCount=0;
	itemNo=0;
	itemCount = flash.Uart[USART_1]->ItemCnt;//가변된다
	sprintf(sumbuf,"%02d", itemCount);
	gv.dataLogBuf[address++] = sumbuf[0];
	gv.dataLogBuf[address++] = sumbuf[1];
	cSum+=sumbuf[0];
	cSum+=sumbuf[1];
	
	for(int i=0; i<itemCount; i++) {
		gv.dataLogBuf[address++] = flash.tmsDataName[i][0];cSum+=flash.tmsDataName[i][0];
		gv.dataLogBuf[address++] = flash.tmsDataName[i][1];cSum+=flash.tmsDataName[i][1];
		gv.dataLogBuf[address++] = flash.tmsDataName[i][2];cSum+=flash.tmsDataName[i][2];	
		gv.dataLogBuf[address++] = flash.tmsDataName[i][3];cSum+=flash.tmsDataName[i][3];
		gv.dataLogBuf[address++] = flash.tmsDataName[i][4];cSum+=flash.tmsDataName[i][4];	
		//측정값
		if(flash.floatPoint[i]==0) {
			sprintf(abuf,"%-10.0f",gv.averPV5Min[comPort][i]);//5분데이터
		}
		else if(flash.floatPoint[i]==1) {
			sprintf(abuf,"%-10.1f",gv.averPV5Min[comPort][i]);//5분데이터
		}
		else if(flash.floatPoint[i]==2) {
			sprintf(abuf,"%-10.2f",gv.averPV5Min[comPort][i]);//5분데이터
		}
		else if(flash.floatPoint[i]==3) {
			sprintf(abuf,"%-10.3f",gv.averPV5Min[comPort][i]);//5분데이터
		}
		else if(flash.floatPoint[i]==4) {
			sprintf(abuf,"%-10.4f",gv.averPV5Min[comPort][i]);//5분데이터
		}
		abuf[10]=0;    
		for(c=0;c<10;c++) {
			gv.dataLogBuf[address++]=abuf[c];
			cSum += abuf[c];
		}
		//상태정보
		if(gv.tmsStatusCode[comPort]==6)
		{
			gv.dataLogBuf[address++] ='0'; //동작불량 
			gv.dataLogBuf[address++] ='6';
			cSum+='0';
			cSum+='6';
		}
		else
		{
			if(gv.tmsStatusCode[comPort]==3)
			{
				gv.dataLogBuf[address++] ='0'; //교정중
				gv.dataLogBuf[address++] ='3';
				cSum+='0';
				cSum+='3';
			}
			else 
			{
				gv.dataLogBuf[address++] ='0'; //정상  
				gv.dataLogBuf[address++] ='0';
				cSum+='0';
				cSum+='0';
			}
		}
		//FACTOR 값
		gv.dataLogBuf[address++] ='0'; 
		gv.dataLogBuf[address++] ='0';
		cSum+='0';
		cSum+='0';	
	}
#if 0	
	/**
	*	항목코드
	*/
	gv.dataLogBuf[address++] = 'T';cSum+='T';
	gv.dataLogBuf[address++] = 'E';cSum+='E';
	gv.dataLogBuf[address++] = 'M';cSum+='M';	
	gv.dataLogBuf[address++] = '0';cSum+='0';
	gv.dataLogBuf[address++] = '0';cSum+='0';	
	//측정값
	sprintf(abuf,"%10.3f",gv.averPV5Min[comPort][0]);
    abuf[10]=0;    
    for(c=0;c<10;c++) {
		gv.dataLogBuf[address++]=abuf[c];
		cSum += abuf[c];
	}
	//상태정보
	if(gv.tmsStatusCode[comPort]==6)
	{
		gv.dataLogBuf[address++] ='0'; //동작불량 
		gv.dataLogBuf[address++] ='6';
		cSum+='0';
		cSum+='6';
	}
	else
	{
		if(gv.tmsStatusCode[comPort]==3)
		{
			gv.dataLogBuf[address++] ='0'; //교정중
			gv.dataLogBuf[address++] ='3';
			cSum+='0';
			cSum+='3';
		}
		else 
		{
			gv.dataLogBuf[address++] ='0'; //정상  
			gv.dataLogBuf[address++] ='0';
			cSum+='0';
			cSum+='0';
		}
	}
    //FACTOR 값
    gv.dataLogBuf[address++] ='0'; 
    gv.dataLogBuf[address++] ='0';
    cSum+='0';
    cSum+='0';
	//항목코드 
	gv.dataLogBuf[address++] = 'C';cSum+='C';
	gv.dataLogBuf[address++] = 'O';cSum+='O';
	gv.dataLogBuf[address++] = 'N';cSum+='N';	
	gv.dataLogBuf[address++] = '0';cSum+='0';
	gv.dataLogBuf[address++] = '0';cSum+='0';
	
	//측정값
	sprintf(abuf,"%10.3f",gv.averPV5Min[comPort][1]);
    abuf[10]=0;    
    for(c=0;c<10;c++) {
		gv.dataLogBuf[address++]=abuf[c];
		cSum += abuf[c];
	}
	//상태정보
	if(gv.tmsStatusCode[comPort]==6)
	{
		gv.dataLogBuf[address++] ='0'; //동작불량 
		gv.dataLogBuf[address++] ='6';
		cSum+='0';
		cSum+='6';
	}
	else
	{
		if(gv.tmsStatusCode[comPort]==3)
		{
			gv.dataLogBuf[address++] ='0'; //교정중
			gv.dataLogBuf[address++] ='3';
			cSum+='0';
			cSum+='3';
		}
		else 
		{
			gv.dataLogBuf[address++] ='0'; //정상  
			gv.dataLogBuf[address++] ='0';
			cSum+='0';
			cSum+='0';
		}
	}
    //FACTOR 값
    gv.dataLogBuf[address++] ='0'; 
    gv.dataLogBuf[address++] ='0';
    cSum+='0';
    cSum+='0';

	//항목코드 
	gv.dataLogBuf[address++] = 'D';cSum+='D';
	gv.dataLogBuf[address++] = 'O';cSum+='O';
	gv.dataLogBuf[address++] = 'W';cSum+='W';	
	gv.dataLogBuf[address++] = '0';cSum+='0';
	gv.dataLogBuf[address++] = '0';cSum+='0';
	
	//측정값
	sprintf(abuf,"%10.3f",gv.averPV5Min[comPort][2]);
    abuf[10]=0;    
    for(c=0;c<10;c++) {
		gv.dataLogBuf[address++]=abuf[c];
		cSum += abuf[c];
	}
	//상태정보
	if(gv.tmsStatusCode[comPort]==6)
	{
		gv.dataLogBuf[address++] ='0'; //동작불량 
		gv.dataLogBuf[address++] ='6';
		cSum+='0';
		cSum+='6';
	}
	else
	{
		if(gv.tmsStatusCode[comPort]==3)
		{
			gv.dataLogBuf[address++] ='0'; //교정중
			gv.dataLogBuf[address++] ='3';
			cSum+='0';
			cSum+='3';
		}
		else 
		{
			gv.dataLogBuf[address++] ='0'; //정상  
			gv.dataLogBuf[address++] ='0';
			cSum+='0';
			cSum+='0';
		}
	}
    //FACTOR 값
    gv.dataLogBuf[address++] ='0'; 
    gv.dataLogBuf[address++] ='0';
    cSum+='0';
    cSum+='0';
    
	//항목코드 
	gv.dataLogBuf[address++] = 'P';cSum+='P';
	gv.dataLogBuf[address++] = 'H';cSum+='H';
	gv.dataLogBuf[address++] = 'Y';cSum+='Y';	
	gv.dataLogBuf[address++] = '0';cSum+='0';
	gv.dataLogBuf[address++] = '0';cSum+='0';
	
	//측정값
	sprintf(abuf,"%10.3f",gv.averPV5Min[comPort][3]);
    abuf[10]=0;    
    for(c=0;c<10;c++) {
		gv.dataLogBuf[address++]=abuf[c];
		cSum += abuf[c];
	}
	//상태정보
	if(gv.tmsStatusCode[comPort]==6)
	{
		gv.dataLogBuf[address++] ='0'; //동작불량 
		gv.dataLogBuf[address++] ='6';
		cSum+='0';
		cSum+='6';
	}
	else
	{
		if(gv.tmsStatusCode[comPort]==3)
		{
			gv.dataLogBuf[address++] ='0'; //교정중
			gv.dataLogBuf[address++] ='3';
			cSum+='0';
			cSum+='3';
		}
		else 
		{
			gv.dataLogBuf[address++] ='0'; //정상  
			gv.dataLogBuf[address++] ='0';
			cSum+='0';
			cSum+='0';
		}
	}
    //FACTOR 값
    gv.dataLogBuf[address++] ='0'; 
    gv.dataLogBuf[address++] ='0';
    cSum+='0';
    cSum+='0';
	//항목코드 
	gv.dataLogBuf[address++] = 'S';cSum+='S';
	gv.dataLogBuf[address++] = 'U';cSum+='U';
	gv.dataLogBuf[address++] = 'S';cSum+='S';	
	gv.dataLogBuf[address++] = '0';cSum+='0';
	gv.dataLogBuf[address++] = '0';cSum+='0';
	
	//측정값
	sprintf(abuf,"%10.3f",gv.averPV5Min[comPort][4]);
    abuf[10]=0;    
    for(c=0;c<10;c++) {
		gv.dataLogBuf[address++]=abuf[c];
		cSum += abuf[c];
	}
	//상태정보
	if(gv.tmsStatusCode[comPort]==6)
	{
		gv.dataLogBuf[address++] ='0'; //동작불량 
		gv.dataLogBuf[address++] ='6';
		cSum+='0';
		cSum+='6';
	}
	else
	{
		if(gv.tmsStatusCode[comPort]==3)
		{
			gv.dataLogBuf[address++] ='0'; //교정중
			gv.dataLogBuf[address++] ='3';
			cSum+='0';
			cSum+='3';
		}
		else 
		{
			gv.dataLogBuf[address++] ='0'; //정상  
			gv.dataLogBuf[address++] ='0';
			cSum+='0';
			cSum+='0';
		}
	}
    //FACTOR 값
    gv.dataLogBuf[address++] ='0'; 
    gv.dataLogBuf[address++] ='0';
    cSum+='0';
    cSum+='0';
#endif	
	for(c=0;c<50;c++)		//부가정보
	{
		gv.dataLogBuf[address++]=0x20;
		cSum+=0x20;
	}	
	gv.dataLogBuf[address++]=0x03;				//ETX
	cSum+=0x03;

	gv.dataLogBuf[address++]=(cSum&0xF0)>>4|0x30;
	gv.dataLogBuf[address++]=(cSum&0x0F)|0x30;

	gv.dataLogBuf[address++]=0x0d;
	
	return TRUE;
}
#endif


