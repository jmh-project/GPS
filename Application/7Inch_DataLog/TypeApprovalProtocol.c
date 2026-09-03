#include "TEECO_System.h"
#include "DataLog.h"

#if defined(TYPE_APPROVAL_ENABLE)//형식승인프로토콜

void ExopHSlope(float mV) {
//	float a,b,pv;
//	float zero = flash.codeZERO;
//	float span = flash.codeSPAN;
//	float iZero= flash.codeMv1;
//	float iSpan= flash.codeMv2;
//	float source=mV;
//	
//	if(flash.codeZERO>0&&flash.codeSPAN>0&&flash.codeFACT==0&&flash.codeOFST==0) {
//		a = (iSpan - iZero) / ( zero - span);
//		b = iZero + (a * zero);
//		pv = (b - source) / a;
//		
//		flash.codeSLOP = a;
//		flash.codeICPT = b;
//		Dprintf("pH1 = %.2f mV = %.2f 기울기 = %.3f 절편 = %.3f\n",pv,source,a,b);
//	}
//	else if(flash.codeZERO>0&&flash.codeSPAN>0&&(flash.codeFACT!=0||flash.codeOFST!=0)) {
//		a = flash.codeSLOP + flash.codeFACT;
//		b = flash.codeICPT + flash.codeOFST;
//		pv = (b - source) / a;
//		Dprintf("pH2 = %.2f mV = %.2f 기울기 = %.3f 절편 = %.3f\n",pv,source,a,b);
//	}
}
extern volatile int 	gUSB_MSC_StatusDump;
void TYPE_APPROVAL_FactorSave(char *id, float value) {
	S8 			        abuf[128];
	int uartCount=0;
	int usbStatus=0;
	int	tcpStatus=0;
	for(int i=0; i<128; i++) {
		abuf[i]=0;
	}
	if(id[0]=='A'&&id[1]=='U'&&id[2]=='X'&&id[3]=='I') {
		//FACTOR AUXI		
		if(flash.Uart[USART_1]->SensorID!=SENSOR_NONE) {
			uartCount++;		
		}
		if(flash.Uart[USART_2]->SensorID!=SENSOR_NONE) {
			uartCount++;		
		}
		if(flash.Uart[USART_3]->SensorID!=SENSOR_NONE) {
			uartCount++;		
		}
		if(flash.Uart[USART_4]->SensorID!=SENSOR_NONE) {
			uartCount++;		
		}
		if(gUSB_MSC_StatusDump==1) {			//USB사용시 접속기록이 저장된다.
			gUSB_MSC_StatusDump=0;
			usbStatus=1;
		}
		else {
			usbStatus=0;
		}
		if(flash.Uart[USART_5]->SensorID==SENSOR_LAN) {
			tcpStatus=1;
		}			
		else {
			tcpStatus=0;
		}
		sprintf(abuf,"AUXI,R%dU%dT%dE0  ,PASS,%s,",uartCount,usbStatus,tcpStatus,flash.password);
		strcat((char *)gv.dataLogBuf,abuf);
	}
	else {
		sprintf(abuf,"%s,%-10.3f,",id,value);	
		strcat((char *)gv.dataLogBuf,abuf);
	}
}
int TYPE_APPROVAL_DataReceive( char ascii, int comPort ) { //TMS 형식승인 수신처리
	char date_buf[16];
	if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_RESPONSE) {
		if(ascii==0x06) {									//데이터 송수신 완료
			if(gv.tmsDumpStepCnt!=0) {						//남은 덤프 데이터를 송신한다. 데이터가 없는 경우 0
				gv.nFlag_ReceiveDataProcess[comPort]=KECO_DUMA_RESPONSE;
			}				
		}
		if(ascii==0x15) { 							        //데이터 재송신을 3회까지 실시한다.
			gv.tmsReSendCount++;							//재송신 횟수증가
			if(gv.tmsReSendCount>=3) {						//재송신 3회이상
				gv.tmsReSendCount=0;						
				gv.nFlag_ReceiveDataProcess[comPort]=0;		//재송신 정지
			}
		}
	}
	if(gv.nFlag_ReceiveDataProcess[comPort]==KECO_DUMA_RESPONSE) {
		if(ascii==0x06) {										//송신데이터 정상수신
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
	if(ascii==0x02)	{
		gv.nFlag_ReceiveStart[comPort]=1;
		gv.nReceiveBufferCount[comPort]=0;
	}
	else if(gv.nFlag_ReceiveStart[comPort]==1) 	{
		gv.aItemReceiveBuffer[comPort][gv.nReceiveBufferCount[comPort]++]=ascii;
		if(ascii==0x0d) {//체크섬 처리는 하지 않는다.
			//덤프 데이터 요청
            if(gv.aItemReceiveBuffer[comPort][0]=='D' && gv.aItemReceiveBuffer[comPort][1]=='U' && gv.aItemReceiveBuffer[comPort][2]=='M' && gv.aItemReceiveBuffer[comPort][3]=='P') {
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
				gv.tmsDumpStepCnt=2;//최초1회송신용, SDCARD 에 저장된 덤프데이터 정보를 전부 읽어들인다.
				gv.tmsReSendCount=0;
			}
			//실시간데이터요청
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
				gv.nFlag_ReceiveDataProcess[comPort] = KECO_SETT;
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
				gv.nFlag_ReceiveDataProcess[comPort] = KECO_PASS;
			}
			//측정기기리셋
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
void TYPE_APPROVAL_PasswordResponse(int comPort) {
	unsigned int cSum=0;
	char abuf[32];
	Uart_TransmitPort(comPort, 0x02);  cSum = 0x02;
	Uart_TransmitPort(comPort, 'S');   cSum += 'S';
	Uart_TransmitPort(comPort, 'E');   cSum += 'E';
	Uart_TransmitPort(comPort, 'T');   cSum += 'T';
	Uart_TransmitPort(comPort, 'P');   cSum += 'P';
	
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
	for(int i=0; i<10; i++) {
		Uart_TransmitPort(comPort, flash.password[i]);   cSum += flash.password[i];
	}
	Uart_TransmitPort( comPort, 0x03);				//ETX
	cSum+=0x03;

	Uart_TransmitPort( comPort, (cSum&0xF0)>>4|0x30);
	Uart_TransmitPort( comPort, (cSum&0x0F)|0x30);

	Uart_TransmitPort( comPort, 0x0d);	
}
unsigned char TYPE_APPROVAL_Status( unsigned int uartPort, unsigned int sum) { //KECO 상태값
	unsigned int		sum1=sum;
	if(gv.tmsStatusCode[uartPort]==6 || gv.comportConnectionError[uartPort]>10)
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

unsigned int TYPE_APPROVAL_SendFactor(int comPort, char *id, float value) {
	unsigned int 		cSum=0;
	S8 			        abuf[30];
	Uart_TransmitPort( comPort, id[0]);cSum+=id[0];
	Uart_TransmitPort( comPort, id[1]);cSum+=id[1];
	Uart_TransmitPort( comPort, id[2]);cSum+=id[2];	
	Uart_TransmitPort( comPort, id[3]);cSum+=id[3];	
	//측정값	
	sprintf(abuf,"%-10.3f",value);//좌측정렬
	for(int i=0;i<10;i++) {
		Uart_TransmitPort( comPort, abuf[i]); 
		cSum += abuf[i];
	}	
	return cSum;
}
int TYPE_APPROVAL_ProtocolSending(int comPort) {//KECO 순간측정자료송신
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
	itemCount = 1;
	sprintf(sumbuf,"%02d", itemCount);
	Uart_TransmitPort( comPort, sumbuf[0]);
	Uart_TransmitPort( comPort, sumbuf[1]);
	cSum+=sumbuf[0];
	cSum+=sumbuf[1];	
	//5-1. 항목코드 5byte

	//항목코드 EXO 항목 2번째가 pH인가 DO인가를 판단하여 TMS 항목이름이자동으로 설정된다.	
	if(flash.Uart[USART_1]->SetupName[1][0]=='p'&&flash.Uart[USART_1]->SetupName[1][1]=='H') {
		flash.tmsDataName[1][0]='P';	
		flash.tmsDataName[1][1]='H';
		flash.tmsDataName[1][2]='Y';
		flash.tmsDataName[1][3]='0';
		flash.tmsDataName[1][4]='0';
	}
	else if(flash.Uart[USART_1]->SetupName[1][0]=='O'&&flash.Uart[USART_1]->SetupName[1][1]=='D') {
		flash.tmsDataName[1][0]='D';	
		flash.tmsDataName[1][1]='O';
		flash.tmsDataName[1][2]='W';
		flash.tmsDataName[1][3]='0';
		flash.tmsDataName[1][4]='0';
	}
	Uart_TransmitPort( comPort, flash.tmsDataName[1][0]);cSum+=flash.tmsDataName[1][0];
	Uart_TransmitPort( comPort, flash.tmsDataName[1][1]);cSum+=flash.tmsDataName[1][1];
	Uart_TransmitPort( comPort, flash.tmsDataName[1][2]);cSum+=flash.tmsDataName[1][2];	
	Uart_TransmitPort( comPort, flash.tmsDataName[1][3]);cSum+=flash.tmsDataName[1][3];	
	Uart_TransmitPort( comPort, flash.tmsDataName[1][4]);cSum+=flash.tmsDataName[1][4];	
	
	
	if(flash.tmsDataName[1][0]=='P'&&flash.tmsDataName[1][1]=='H'&&flash.tmsDataName[1][2]=='Y') {//측정항목이 pH 인경우
		//측정값
		if(flash.floatPoint[1]==0) {
			sprintf(abuf,"%-10.0f",flash.Uart[USART_1]->SensorPV[1]);
		}
		else if(flash.floatPoint[1]==1) {
			sprintf(abuf,"%-10.1f",flash.Uart[USART_1]->SensorPV[1]);
		}
		else if(flash.floatPoint[1]==2) {
			sprintf(abuf,"%-10.2f",flash.Uart[USART_1]->SensorPV[1]);
		}
		else if(flash.floatPoint[1]==3) {
			sprintf(abuf,"%-10.3f",flash.Uart[USART_1]->SensorPV[1]);
		}
		else if(flash.floatPoint[1]==4) {
			sprintf(abuf,"%-10.4f",flash.Uart[USART_1]->SensorPV[1]);
		}
		for(c=0;c<10;c++) {
			Uart_TransmitPort( comPort, abuf[c]); 
			cSum += abuf[c];
		}
		//상태정보
		cSum=TYPE_APPROVAL_Status( USART_1, cSum);
		//측정상수코드 12개    
		Uart_TransmitPort( comPort, '0');cSum+='0';	
		Uart_TransmitPort( comPort, '9');cSum+='9';		

		flash.codeMSIG[0]=gv.tms_pHmV;
		cSum += TYPE_APPROVAL_SendFactor(comPort,"MSIG",gv.tms_pHmV);//측정 mV
		
		//항목코드	
		Uart_TransmitPort( comPort, 'M');cSum+='M';
		Uart_TransmitPort( comPort, 'T');cSum+='T';
		Uart_TransmitPort( comPort, 'M');cSum+='M';
		Uart_TransmitPort( comPort, '1');cSum+='1';
		if(flash.floatPoint[0]==0) {
			sprintf(abuf,"%-10.0f",flash.Uart[USART_1]->SensorPV[0]);
		}
		else if(flash.floatPoint[0]==1) {
			sprintf(abuf,"%-10.1f",flash.Uart[USART_1]->SensorPV[0]);
		}
		else if(flash.floatPoint[0]==2) {
			sprintf(abuf,"%-10.2f",flash.Uart[USART_1]->SensorPV[0]);
		}
		else if(flash.floatPoint[0]==3) {
			sprintf(abuf,"%-10.3f",flash.Uart[USART_1]->SensorPV[0]);
		}
		else if(flash.floatPoint[0]==4) {
			sprintf(abuf,"%-10.4f",flash.Uart[USART_1]->SensorPV[0]);
		}
		for(c=0;c<10;c++) {
			Uart_TransmitPort( comPort, abuf[c]); 
			cSum += abuf[c];
		}
	//	flash.codeMTM1=flash.Uart[USART_1]->SensorPV[0];
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"MTM1",flash.codeMTM1);//측정온도

	//	flash.codeMTM2=flash.Uart[USART_1]->SensorPV[0];
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"MTM2",flash.codeMTM2);
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"MSAM",flash.codeMSAM);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"ZERO",flash.codeZERO[0]);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"SPAN",flash.codeSPAN[0]);
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"SLOP",flash.codeSLOP);
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"ICPT",flash.codeICPT);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"FACT",flash.codeFACT[0]);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"OFST",flash.codeOFST[0]);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"MAXR",flash.codeMAXR[0]);
	}
	else if(flash.tmsDataName[1][0]=='D'&&flash.tmsDataName[1][1]=='O'&&flash.tmsDataName[1][2]=='W') {//측정항목이 DO 인경우
//		//측정값
//		if(flash.floatPoint[1]==0) {
//			sprintf(abuf,"%-10.0f",flash.Uart[USART_1]->SensorPV[1]);
//		}
//		else if(flash.floatPoint[1]==1) {
//			sprintf(abuf,"%-10.1f",flash.Uart[USART_1]->SensorPV[1]);
//		}
//		else if(flash.floatPoint[1]==2) {
//			sprintf(abuf,"%-10.2f",flash.Uart[USART_1]->SensorPV[1]);
//		}
//		else if(flash.floatPoint[1]==3) {
//			sprintf(abuf,"%-10.3f",flash.Uart[USART_1]->SensorPV[1]);
//		}
//		else if(flash.floatPoint[1]==4) {
//			sprintf(abuf,"%-10.4f",flash.Uart[USART_1]->SensorPV[1]);
//		}
//		for(c=0;c<10;c++) {
//			Uart_TransmitPort( comPort, abuf[c]); 
//			cSum += abuf[c];
//		}
//		//상태정보
//		cSum=TYPE_APPROVAL_Status( USART_1, cSum);
//		//측정상수코드 12개    
//		Uart_TransmitPort( comPort, '0');cSum+='0';	
//		Uart_TransmitPort( comPort, '1');cSum+='1';		
		//측정값
		if(flash.floatPoint[1]==0) {
			sprintf(abuf,"%-10.0f",flash.Uart[USART_1]->SensorPV[1]);
		}
		else if(flash.floatPoint[1]==1) {
			sprintf(abuf,"%-10.1f",flash.Uart[USART_1]->SensorPV[1]);
		}
		else if(flash.floatPoint[1]==2) {
			sprintf(abuf,"%-10.2f",flash.Uart[USART_1]->SensorPV[1]);
		}
		else if(flash.floatPoint[1]==3) {
			sprintf(abuf,"%-10.3f",flash.Uart[USART_1]->SensorPV[1]);
		}
		else if(flash.floatPoint[1]==4) {
			sprintf(abuf,"%-10.4f",flash.Uart[USART_1]->SensorPV[1]);
		}
		for(c=0;c<10;c++) {
			Uart_TransmitPort( comPort, abuf[c]); 
			cSum += abuf[c];
		}
		//상태정보
		cSum=TYPE_APPROVAL_Status( USART_1, cSum);
		//측정상수코드 12개    
		Uart_TransmitPort( comPort, '0');cSum+='0';	
		Uart_TransmitPort( comPort, '9');cSum+='9';		

		flash.codeMSIG[1]=0;
		cSum += TYPE_APPROVAL_SendFactor(comPort,"MSIG",flash.codeMSIG[1]);//측정 mV
		
		//항목코드	
		Uart_TransmitPort( comPort, 'M');cSum+='M';
		Uart_TransmitPort( comPort, 'T');cSum+='T';
		Uart_TransmitPort( comPort, 'M');cSum+='M';
		Uart_TransmitPort( comPort, '1');cSum+='1';
		if(flash.floatPoint[0]==0) {
			sprintf(abuf,"%-10.0f",flash.Uart[USART_1]->SensorPV[0]);
		}
		else if(flash.floatPoint[0]==1) {
			sprintf(abuf,"%-10.1f",flash.Uart[USART_1]->SensorPV[0]);
		}
		else if(flash.floatPoint[0]==2) {
			sprintf(abuf,"%-10.2f",flash.Uart[USART_1]->SensorPV[0]);
		}
		else if(flash.floatPoint[0]==3) {
			sprintf(abuf,"%-10.3f",flash.Uart[USART_1]->SensorPV[0]);
		}
		else if(flash.floatPoint[0]==4) {
			sprintf(abuf,"%-10.4f",flash.Uart[USART_1]->SensorPV[0]);
		}
		for(c=0;c<10;c++) {
			Uart_TransmitPort( comPort, abuf[c]); 
			cSum += abuf[c];
		}
	//	flash.codeMTM1=flash.Uart[USART_1]->SensorPV[0];
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"MTM1",flash.codeMTM1);//측정온도

	//	flash.codeMTM2=flash.Uart[USART_1]->SensorPV[0];
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"MTM2",flash.codeMTM2);
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"MSAM",flash.codeMSAM);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"ZERO",flash.codeZERO[1]);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"SPAN",flash.codeSPAN[1]);
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"SLOP",flash.codeSLOP);
	//	cSum += TYPE_APPROVAL_SendFactor(comPort,"ICPT",flash.codeICPT);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"FACT",flash.codeFACT[1]);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"OFST",flash.codeOFST[1]);
		cSum += TYPE_APPROVAL_SendFactor(comPort,"MAXR",flash.codeMAXR[1]);
		
	}
	
	Uart_TransmitPort( comPort, 'A');cSum+='A';
	Uart_TransmitPort( comPort, 'U');cSum+='U';
	Uart_TransmitPort( comPort, 'X');cSum+='X';	
	Uart_TransmitPort( comPort, 'I');cSum+='I';	

	Uart_TransmitPort( comPort, 'R');cSum+='R';//시리얼장치
	int uartCount=0;
	if(flash.Uart[USART_1]->SensorID!=SENSOR_NONE) {
		uartCount++;		
	}
	if(flash.Uart[USART_2]->SensorID!=SENSOR_NONE) {
		uartCount++;		
	}
	if(flash.Uart[USART_3]->SensorID!=SENSOR_NONE) {
		uartCount++;		
	}
	if(flash.Uart[USART_4]->SensorID!=SENSOR_NONE) {
		uartCount++;		
	}
	char countBuf[10];
	sprintf(countBuf,"%d",uartCount);
	countBuf[1]=0;
	Uart_TransmitPort( comPort, countBuf[0]);cSum+=countBuf[0];
	
	Uart_TransmitPort( comPort, 'U');cSum+='U';//USB
	if(gUSB_MSC_Status==1) {
		Uart_TransmitPort( comPort, '1');cSum+='1';//USB 연결됨	
	} 
	else {
		Uart_TransmitPort( comPort, '0');cSum+='0';//USB 대기중	
	}
	Uart_TransmitPort( comPort, 'T');cSum+='T';//TCP/IP	
	if(flash.Uart[USART_5]->SensorID==SENSOR_LAN) {
		Uart_TransmitPort( comPort, '1');cSum+='1';
	}
	else {
		Uart_TransmitPort( comPort, '0');cSum+='0';
	}
	
	
	Uart_TransmitPort( comPort, 'E');cSum+='E';//다른장비	
	Uart_TransmitPort( comPort, '0');cSum+='0';	
	Uart_TransmitPort( comPort, ' ');cSum+=' ';//나머지 2바이트는 공백처리(장비입출력장치접속검지)
	Uart_TransmitPort( comPort, ' ');cSum+=' ';
	
	Uart_TransmitPort( comPort, 'P');cSum+='P';
	Uart_TransmitPort( comPort, 'A');cSum+='A';
	Uart_TransmitPort( comPort, 'S');cSum+='S';	
	Uart_TransmitPort( comPort, 'S');cSum+='S';	
	
	for(int i=0; i<10; i++) {
		Uart_TransmitPort( comPort, flash.password[i]);
		cSum+=flash.password[i];
	}
	for(c=0;c<50;c++)		//부가정보 50byte 중에 14byte는 비번정보
	{
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
int TYPE_APPROVAL_DUMA_FileCreate(int comPort) {//5분 덤프데이터저장 //덤프요청 DUMA2020092816500020200928165500
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
	itemCount = 1;
	sprintf(sumbuf,"%02d", itemCount);
	gv.dataLogBuf[address++] = sumbuf[0];
	gv.dataLogBuf[address++] = sumbuf[1];
	cSum+=sumbuf[0];
	cSum+=sumbuf[1];
	/**
	*	항목코드
	*/
	gv.dataLogBuf[address++] = flash.tmsDataName[1][0];cSum+=flash.tmsDataName[1][0];
	gv.dataLogBuf[address++] = flash.tmsDataName[1][1];cSum+=flash.tmsDataName[1][1];
	gv.dataLogBuf[address++] = flash.tmsDataName[1][2];cSum+=flash.tmsDataName[1][2];	
	gv.dataLogBuf[address++] = flash.tmsDataName[1][3];cSum+=flash.tmsDataName[1][3];
	gv.dataLogBuf[address++] = flash.tmsDataName[1][4];cSum+=flash.tmsDataName[1][4];	
	
	if(flash.tmsDataName[1][0]=='P'&&flash.tmsDataName[1][1]=='H'&&flash.tmsDataName[1][2]=='Y') {//측정항목이 pH 인경우	
		//측정값
		if(flash.floatPoint[1]==0) {
			sprintf(abuf,"%-10.0f",gv.averPV5Min[comPort][1]);
		}
		else if(flash.floatPoint[1]==1) {
			sprintf(abuf,"%-10.1f",gv.averPV5Min[comPort][1]);
		}
		else if(flash.floatPoint[1]==2) {
			sprintf(abuf,"%-10.2f",gv.averPV5Min[comPort][1]);
		}
		else if(flash.floatPoint[1]==3) {
			sprintf(abuf,"%-10.3f",gv.averPV5Min[comPort][1]);
		}
		else if(flash.floatPoint[1]==4) {
			sprintf(abuf,"%-10.4f",gv.averPV5Min[comPort][1]);
		}
		
		abuf[10]=0;    
		for(c=0;c<10;c++) {
			gv.dataLogBuf[address++]=abuf[c];
			cSum += abuf[c];
		}
		//상태정보
		if(gv.tmsStatusCode[comPort]==6) {
			gv.dataLogBuf[address++] ='0'; //동작불량 
			gv.dataLogBuf[address++] ='6';
			cSum+='0';
			cSum+='6';
		}
		else {
			if(gv.tmsStatusCode[comPort]==3) {
				gv.dataLogBuf[address++] ='0'; //교정중
				gv.dataLogBuf[address++] ='3';
				cSum+='0';
				cSum+='3';
			}
			else {
				gv.dataLogBuf[address++] ='0'; //정상  
				gv.dataLogBuf[address++] ='0';
				cSum+='0';
				cSum+='0';
			}
		}
		//FACTOR 값
		gv.dataLogBuf[address++] ='0'; 
		gv.dataLogBuf[address++] ='9';
		cSum+='0';
		cSum+='9';
		
		//FACTOR MSIG
		gv.dataLogBuf[address++]='M'; cSum+='M';
		gv.dataLogBuf[address++]='S'; cSum+='S';
		gv.dataLogBuf[address++]='I'; cSum+='I';
		gv.dataLogBuf[address++]='G'; cSum+='G';
		//측정값
		sprintf(abuf,"%-10.3f",gv.tms_pHmV);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
		
		//FACTOR MTM1
		gv.dataLogBuf[address++]='M'; cSum+='M';
		gv.dataLogBuf[address++]='T'; cSum+='T';
		gv.dataLogBuf[address++]='M'; cSum+='M';
		gv.dataLogBuf[address++]='1'; cSum+='1';
		//측정값
		flash.codeMTM1[0]=gv.averPV5Min[comPort][0];
		if(flash.floatPoint[0]==0) {
			sprintf(abuf,"%-10.0f",flash.codeMTM1[0]);//좌측정렬
		}
		else if(flash.floatPoint[0]==1) {
			sprintf(abuf,"%-10.1f",flash.codeMTM1[0]);//좌측정렬
		}
		else if(flash.floatPoint[0]==2) {
			sprintf(abuf,"%-10.2f",flash.codeMTM1[0]);//좌측정렬
		}
		else if(flash.floatPoint[0]==3) {
			sprintf(abuf,"%-10.3f",flash.codeMTM1[0]);//좌측정렬
		}
		else if(flash.floatPoint[0]==4) {
			sprintf(abuf,"%-10.4f",flash.codeMTM1[0]);//좌측정렬
		}
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
	//	//FACTOR MTM2
	//	gv.dataLogBuf[address++]='M'; cSum+='M';
	//	gv.dataLogBuf[address++]='T'; cSum+='T';
	//	gv.dataLogBuf[address++]='M'; cSum+='M';
	//	gv.dataLogBuf[address++]='2'; cSum+='2';
	//	//측정값
	//	flash.codeMTM2=flash.Uart[USART_1]->SensorPV[0];
	//	sprintf(abuf,"%-10.3f",flash.codeMTM2);//좌측정렬
	//	for(int i=0;i<10;i++) {
	//		gv.dataLogBuf[address++]=abuf[i];
	//		cSum += abuf[i];
	//	}	
	//	//FACTOR MSAM
	//	gv.dataLogBuf[address++]='M'; cSum+='M';
	//	gv.dataLogBuf[address++]='S'; cSum+='S';
	//	gv.dataLogBuf[address++]='A'; cSum+='A';
	//	gv.dataLogBuf[address++]='M'; cSum+='M';
	//	//측정값
	//	sprintf(abuf,"%-10.3f",flash.codeMSAM);//좌측정렬
	//	for(int i=0;i<10;i++) {
	//		gv.dataLogBuf[address++]=abuf[i];
	//		cSum += abuf[i];
	//	}	
		//FACTOR ZERO
		gv.dataLogBuf[address++]='Z'; cSum+='Z';
		gv.dataLogBuf[address++]='E'; cSum+='E';
		gv.dataLogBuf[address++]='R'; cSum+='R';
		gv.dataLogBuf[address++]='O'; cSum+='O';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeZERO[0]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
		//FACTOR SPAN
		gv.dataLogBuf[address++]='S'; cSum+='S';
		gv.dataLogBuf[address++]='P'; cSum+='P';
		gv.dataLogBuf[address++]='A'; cSum+='A';
		gv.dataLogBuf[address++]='N'; cSum+='N';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeSPAN[0]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
	//	//FACTOR SLOP
	//	gv.dataLogBuf[address++]='S'; cSum+='S';
	//	gv.dataLogBuf[address++]='L'; cSum+='L';
	//	gv.dataLogBuf[address++]='O'; cSum+='O';
	//	gv.dataLogBuf[address++]='P'; cSum+='P';
	//	//측정값
	//	sprintf(abuf,"%-10.3f",flash.codeSLOP);//좌측정렬
	//	for(int i=0;i<10;i++) {
	//		gv.dataLogBuf[address++]=abuf[i];
	//		cSum += abuf[i];
	//	}	
	//	//FACTOR ICPT
	//	gv.dataLogBuf[address++]='I'; cSum+='I';
	//	gv.dataLogBuf[address++]='C'; cSum+='C';
	//	gv.dataLogBuf[address++]='P'; cSum+='P';
	//	gv.dataLogBuf[address++]='T'; cSum+='T';
	//	//측정값
	//	sprintf(abuf,"%-10.3f",flash.codeICPT);//좌측정렬
	//	for(int i=0;i<10;i++) {
	//		gv.dataLogBuf[address++]=abuf[i];
	//		cSum += abuf[i];
	//	}	
		//FACTOR FACT
		gv.dataLogBuf[address++]='F'; cSum+='F';
		gv.dataLogBuf[address++]='A'; cSum+='A';
		gv.dataLogBuf[address++]='C'; cSum+='C';
		gv.dataLogBuf[address++]='T'; cSum+='T';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeFACT[0]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
		//FACTOR OFST
		gv.dataLogBuf[address++]='O'; cSum+='O';
		gv.dataLogBuf[address++]='F'; cSum+='F';
		gv.dataLogBuf[address++]='S'; cSum+='S';
		gv.dataLogBuf[address++]='T'; cSum+='T';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeOFST[0]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
		//FACTOR MAXR
		gv.dataLogBuf[address++]='M'; cSum+='M';
		gv.dataLogBuf[address++]='A'; cSum+='A';
		gv.dataLogBuf[address++]='X'; cSum+='X';
		gv.dataLogBuf[address++]='R'; cSum+='R';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeMAXR[0]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}		
	} 
	else if(flash.tmsDataName[1][0]=='D'&&flash.tmsDataName[1][1]=='O'&&flash.tmsDataName[1][2]=='W') {//측정항목이 DO 인경우
		//측정값
//		if(flash.floatPoint[1]==0) {
//			sprintf(abuf,"%-10.0f",gv.averPV5Min[comPort][1]);
//		}
//		else if(flash.floatPoint[1]==1) {
//			sprintf(abuf,"%-10.1f",gv.averPV5Min[comPort][1]);
//		}
//		else if(flash.floatPoint[1]==2) {
//			sprintf(abuf,"%-10.2f",gv.averPV5Min[comPort][1]);
//		}
//		else if(flash.floatPoint[1]==3) {
//			sprintf(abuf,"%-10.3f",gv.averPV5Min[comPort][1]);
//		}
//		else if(flash.floatPoint[1]==4) {
//			sprintf(abuf,"%-10.4f",gv.averPV5Min[comPort][1]);
//		}
//		abuf[10]=0;    
//		for(c=0;c<10;c++) {
//			gv.dataLogBuf[address++]=abuf[c];
//			cSum += abuf[c];
//		}
//		//상태정보
//		if(gv.tmsStatusCode[comPort]==6) {
//			gv.dataLogBuf[address++] ='0'; //동작불량 
//			gv.dataLogBuf[address++] ='6';
//			cSum+='0';
//			cSum+='6';
//		}
//		else {
//			if(gv.tmsStatusCode[comPort]==3) {
//				gv.dataLogBuf[address++] ='0'; //교정중
//				gv.dataLogBuf[address++] ='3';
//				cSum+='0';
//				cSum+='3';
//			}
//			else {
//				gv.dataLogBuf[address++] ='0'; //정상  
//				gv.dataLogBuf[address++] ='0';
//				cSum+='0';
//				cSum+='0';
//			}
//		}
//		//FACTOR 값
//		gv.dataLogBuf[address++] ='0'; 
//		gv.dataLogBuf[address++] ='1';
//		cSum+='0';
//		cSum+='1';	
		//측정값
		if(flash.floatPoint[1]==0) {
			sprintf(abuf,"%-10.0f",gv.averPV5Min[comPort][1]);
		}
		else if(flash.floatPoint[1]==1) {
			sprintf(abuf,"%-10.1f",gv.averPV5Min[comPort][1]);
		}
		else if(flash.floatPoint[1]==2) {
			sprintf(abuf,"%-10.2f",gv.averPV5Min[comPort][1]);
		}
		else if(flash.floatPoint[1]==3) {
			sprintf(abuf,"%-10.3f",gv.averPV5Min[comPort][1]);
		}
		else if(flash.floatPoint[1]==4) {
			sprintf(abuf,"%-10.4f",gv.averPV5Min[comPort][1]);
		}
		
		abuf[10]=0;    
		for(c=0;c<10;c++) {
			gv.dataLogBuf[address++]=abuf[c];
			cSum += abuf[c];
		}
		//상태정보
		if(gv.tmsStatusCode[comPort]==6) {
			gv.dataLogBuf[address++] ='0'; //동작불량 
			gv.dataLogBuf[address++] ='6';
			cSum+='0';
			cSum+='6';
		}
		else {
			if(gv.tmsStatusCode[comPort]==3) {
				gv.dataLogBuf[address++] ='0'; //교정중
				gv.dataLogBuf[address++] ='3';
				cSum+='0';
				cSum+='3';
			}
			else {
				gv.dataLogBuf[address++] ='0'; //정상  
				gv.dataLogBuf[address++] ='0';
				cSum+='0';
				cSum+='0';
			}
		}
		//FACTOR 값
		gv.dataLogBuf[address++] ='0'; 
		gv.dataLogBuf[address++] ='9';
		cSum+='0';
		cSum+='9';
		
		//FACTOR MSIG
		gv.dataLogBuf[address++]='M'; cSum+='M';
		gv.dataLogBuf[address++]='S'; cSum+='S';
		gv.dataLogBuf[address++]='I'; cSum+='I';
		gv.dataLogBuf[address++]='G'; cSum+='G';
		//측정값
		sprintf(abuf,"%-10.3f",0.0);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
		
		//FACTOR MTM1
		gv.dataLogBuf[address++]='M'; cSum+='M';
		gv.dataLogBuf[address++]='T'; cSum+='T';
		gv.dataLogBuf[address++]='M'; cSum+='M';
		gv.dataLogBuf[address++]='1'; cSum+='1';
		//측정값
		flash.codeMTM1[1]=gv.averPV5Min[comPort][0];
		if(flash.floatPoint[0]==0) {
			sprintf(abuf,"%-10.0f",flash.codeMTM1[1]);//좌측정렬
		}
		else if(flash.floatPoint[0]==1) {
			sprintf(abuf,"%-10.1f",flash.codeMTM1[1]);//좌측정렬
		}
		else if(flash.floatPoint[0]==2) {
			sprintf(abuf,"%-10.2f",flash.codeMTM1[1]);//좌측정렬
		}
		else if(flash.floatPoint[0]==3) {
			sprintf(abuf,"%-10.3f",flash.codeMTM1[1]);//좌측정렬
		}
		else if(flash.floatPoint[0]==4) {
			sprintf(abuf,"%-10.4f",flash.codeMTM1[1]);//좌측정렬
		}
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
	//	//FACTOR MTM2
	//	gv.dataLogBuf[address++]='M'; cSum+='M';
	//	gv.dataLogBuf[address++]='T'; cSum+='T';
	//	gv.dataLogBuf[address++]='M'; cSum+='M';
	//	gv.dataLogBuf[address++]='2'; cSum+='2';
	//	//측정값
	//	flash.codeMTM2=flash.Uart[USART_1]->SensorPV[0];
	//	sprintf(abuf,"%-10.3f",flash.codeMTM2);//좌측정렬
	//	for(int i=0;i<10;i++) {
	//		gv.dataLogBuf[address++]=abuf[i];
	//		cSum += abuf[i];
	//	}	
	//	//FACTOR MSAM
	//	gv.dataLogBuf[address++]='M'; cSum+='M';
	//	gv.dataLogBuf[address++]='S'; cSum+='S';
	//	gv.dataLogBuf[address++]='A'; cSum+='A';
	//	gv.dataLogBuf[address++]='M'; cSum+='M';
	//	//측정값
	//	sprintf(abuf,"%-10.3f",flash.codeMSAM);//좌측정렬
	//	for(int i=0;i<10;i++) {
	//		gv.dataLogBuf[address++]=abuf[i];
	//		cSum += abuf[i];
	//	}	
		//FACTOR ZERO
		gv.dataLogBuf[address++]='Z'; cSum+='Z';
		gv.dataLogBuf[address++]='E'; cSum+='E';
		gv.dataLogBuf[address++]='R'; cSum+='R';
		gv.dataLogBuf[address++]='O'; cSum+='O';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeZERO[1]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
		//FACTOR SPAN
		gv.dataLogBuf[address++]='S'; cSum+='S';
		gv.dataLogBuf[address++]='P'; cSum+='P';
		gv.dataLogBuf[address++]='A'; cSum+='A';
		gv.dataLogBuf[address++]='N'; cSum+='N';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeSPAN[1]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
	//	//FACTOR SLOP
	//	gv.dataLogBuf[address++]='S'; cSum+='S';
	//	gv.dataLogBuf[address++]='L'; cSum+='L';
	//	gv.dataLogBuf[address++]='O'; cSum+='O';
	//	gv.dataLogBuf[address++]='P'; cSum+='P';
	//	//측정값
	//	sprintf(abuf,"%-10.3f",flash.codeSLOP);//좌측정렬
	//	for(int i=0;i<10;i++) {
	//		gv.dataLogBuf[address++]=abuf[i];
	//		cSum += abuf[i];
	//	}	
	//	//FACTOR ICPT
	//	gv.dataLogBuf[address++]='I'; cSum+='I';
	//	gv.dataLogBuf[address++]='C'; cSum+='C';
	//	gv.dataLogBuf[address++]='P'; cSum+='P';
	//	gv.dataLogBuf[address++]='T'; cSum+='T';
	//	//측정값
	//	sprintf(abuf,"%-10.3f",flash.codeICPT);//좌측정렬
	//	for(int i=0;i<10;i++) {
	//		gv.dataLogBuf[address++]=abuf[i];
	//		cSum += abuf[i];
	//	}	
		//FACTOR FACT
		gv.dataLogBuf[address++]='F'; cSum+='F';
		gv.dataLogBuf[address++]='A'; cSum+='A';
		gv.dataLogBuf[address++]='C'; cSum+='C';
		gv.dataLogBuf[address++]='T'; cSum+='T';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeFACT[1]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
		//FACTOR OFST
		gv.dataLogBuf[address++]='O'; cSum+='O';
		gv.dataLogBuf[address++]='F'; cSum+='F';
		gv.dataLogBuf[address++]='S'; cSum+='S';
		gv.dataLogBuf[address++]='T'; cSum+='T';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeOFST[1]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}	
		//FACTOR MAXR
		gv.dataLogBuf[address++]='M'; cSum+='M';
		gv.dataLogBuf[address++]='A'; cSum+='A';
		gv.dataLogBuf[address++]='X'; cSum+='X';
		gv.dataLogBuf[address++]='R'; cSum+='R';
		//측정값
		sprintf(abuf,"%-10.3f",flash.codeMAXR[1]);//좌측정렬
		for(int i=0;i<10;i++) {
			gv.dataLogBuf[address++]=abuf[i];
			cSum += abuf[i];
		}
	}
	else {
		DataLog_MessageBox_Window("항목코드", "TMS 항목코드가 잘 못 입력되었습니다.");
	}
	//FACTOR AUXI
	gv.dataLogBuf[address++]='A'; cSum+='A';
	gv.dataLogBuf[address++]='U'; cSum+='U';
	gv.dataLogBuf[address++]='X'; cSum+='X';
	gv.dataLogBuf[address++]='I'; cSum+='I';
	
	gv.dataLogBuf[address++]='R'; cSum+='R';//시리얼장치
	
	int uartCount=0;
	if(flash.Uart[USART_1]->SensorID!=SENSOR_NONE) {
		uartCount++;		
	}
	if(flash.Uart[USART_2]->SensorID!=SENSOR_NONE) {
		uartCount++;		
	}
	if(flash.Uart[USART_3]->SensorID!=SENSOR_NONE) {
		uartCount++;		
	}
	if(flash.Uart[USART_4]->SensorID!=SENSOR_NONE) {
		uartCount++;		
	}
	char countBuf[10];
	sprintf(countBuf,"%d",uartCount);
	gv.dataLogBuf[address++]=countBuf[0]; cSum+=countBuf[0];//사용수고정
	
	gv.dataLogBuf[address++]='U'; cSum+='U';//USB장치 
	if(gUSB_MSC_StatusDump==1) {			//USB사용시 접속기록이 저장된다.
		gUSB_MSC_StatusDump=0;
		gv.dataLogBuf[address++]='1'; cSum+='1';//USB 접속중
	}
	else {
		gv.dataLogBuf[address++]='0'; cSum+='0';
	}
	gv.dataLogBuf[address++]='T'; cSum+='T';
	if(flash.Uart[USART_5]->SensorID==SENSOR_LAN) {
		gv.dataLogBuf[address++]='1'; cSum+='1';
	}
	else {
		gv.dataLogBuf[address++]='0'; cSum+='0';
	}
	gv.dataLogBuf[address++]='E'; cSum+='E';
	gv.dataLogBuf[address++]='0'; cSum+='0';
	gv.dataLogBuf[address++]=' '; cSum+=' ';
	gv.dataLogBuf[address++]=' '; cSum+=' ';
	//부가정보 : 비밀번호
	gv.dataLogBuf[address++]='P'; cSum+='P';
	gv.dataLogBuf[address++]='A'; cSum+='A';
	gv.dataLogBuf[address++]='S'; cSum+='S';
	gv.dataLogBuf[address++]='S'; cSum+='S';

	for(int i=0; i<10; i++) {
		gv.dataLogBuf[address++]=flash.password[i];
		cSum+=flash.password[i];
	}
	for(c=0;c<50;c++)		//부가정보 50byte 중에 14byte는 비번정보
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

