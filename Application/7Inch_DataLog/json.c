#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"

#ifdef JSON_ENABLE
int DUMP_SDCardTORAM(RealTime startDate, RealTime endDate);
int DUMP_DateChecking(RealTime startDateSv, RealTime endDateSv);
int DUMP_SDCardTORAM(RealTime startTime, RealTime endTime);
int JSON_DUMP_Searchforsendingfiles(RealTime startDateSv, RealTime endDateSv);

void Json_TxBufClear(void) { //버퍼초기화    
    for(int i=0; i<SMARTLOG_DATALOG_SIZE; i++) {
        gv.transmitBuf[i]=0;
    }        
}
void Json_firmware_send_ack(void) {
	Json_TxBufClear();
	sprintf((char *)gv.transmitBuf,"[{\"group\":\"%03d\",\"site\":\"%03d\",\"device\":\"%03d\",\"cmd_type\":\"firmware_send_ack\"}]",
	flash.json.groupSv,flash.json.siteSv,flash.json.diviceSv);
}

void Json_Format_Sending(char *string) {
    Json_TxBufClear();
	sprintf((char *)gv.transmitBuf,
        "[{\"group\":\"%03d\",\"site\":\"%03d\",\"device\":\"%03d\",\"cmd_type\":\"%s\"}]",
        flash.json.groupSv,flash.json.siteSv,flash.json.diviceSv,string);
}
void Json_rtu_parameterSendProcess(void) {
	char buf[128];
    for(int i=0; i<128; i++) {
        buf[i]=0;
    }
    Json_TxBufClear();
	sprintf((char *)gv.transmitBuf,"[{\"group\":\"%03d\",\"site\":\"%03d\",\"device\":\"%03d\",\"cmd_type\":\"server_set\"},",
	flash.json.groupSv,flash.json.siteSv,flash.json.diviceSv);
	//SERVER IP
	sprintf(buf,"{\"serverIp\":\"%d.%d.%d.%d\",",flash.serverIp[0],flash.serverIp[1],flash.serverIp[2],flash.serverIp[3]);
	strcat((char *)gv.transmitBuf,buf);
	//SERVER PORT
	sprintf(buf,"\"serverPort\":\"%d\",",flash.serverPort[0]);
	strcat((char *)gv.transmitBuf,buf);
	//CLIENT IP
	sprintf(buf,"\"clientIp\":\"%d.%d.%d.%d\",",flash.clientIp[0],flash.clientIp[1],flash.clientIp[2],flash.clientIp[3]);
	strcat((char *)gv.transmitBuf,buf);
	//CLIENT PORT
	sprintf(buf,"\"clientPort\":\"%d\"}]",flash.clientPort);//즉시측정수심설정값
	strcat((char *)gv.transmitBuf,buf);
}
int Json_IpSet(int ch) {                                //SERVER / CLIENT IP SET
	char buf[10];
	int ip1, ip2, ip3, ip4;
	int iteam=0;
	int cnt=0;
	int w5500InitFLag=0;
	Dprintf("RX SERVER IP = %s\n",gv.json.serverIpPv);	
    Dprintf("RX CLIENT IP = %s\n",gv.json.serverIpPv);	
	if(ch==0) {	//server
		for(int i=0; i<32; i++) {
			if(gv.json.serverIpPv[i]!='.') {
				buf[cnt++]=gv.json.serverIpPv[i];
				buf[cnt]=0;
			} else {
				if(iteam==0) {
					ip1 = atoi(buf);
					if(flash.serverIp[0]!=ip1) { 
						flash.serverIp[0]=ip1;	
						w5500InitFLag=1;
					}
				} else if(iteam==1) {
					ip2 = atoi(buf);
					if(flash.serverIp[1]!=ip2) {
						flash.serverIp[1]=ip2;
						w5500InitFLag=1;
					}
					
				} else if(iteam==2) {
					ip3 = atoi(buf);
					if(flash.serverIp[2]!=ip3) {
						flash.serverIp[2]=ip3;
						w5500InitFLag=1;
					}
				} else if(iteam==3) {
					ip4 = atoi(buf);
					if(flash.serverIp[3]!=ip4) {
						flash.serverIp[3]=ip4;
						w5500InitFLag=1;
					}
					if(w5500InitFLag==1) { //ip 변경이 없는 경우 초기화 하지 않는다.
						flash.serverGate[0]=flash.serverIp[0];
						flash.serverGate[1]=flash.serverIp[1];
						flash.serverGate[2]=flash.serverIp[2];
						flash.serverGate[3]=1;
						W5500_Init();
						Dprintf("SET SERVER IP = %d.%d.%d.%d\n",flash.serverIp[0],flash.serverIp[1],flash.serverIp[2],flash.serverIp[3]);
					}
					return 0;
				}
				cnt=0;
				iteam++;
				for(int c; c<10; c++) {
					buf[c]=0;
				}
			}
		}
	} else if(ch==1) { //client
		for(int i=0; i<32; i++) {
			if(gv.json.clientIpPv[i]!='.') {
				buf[cnt++]=gv.json.clientIpPv[i];
				buf[cnt]=0;
			} else {
				if(iteam==0) {
					ip1 = atoi(buf);
					if(flash.clientIp[0]!=ip1) {
						flash.clientIp[0]=ip1;
						w5500InitFLag=1;
					}
				} else if(iteam==1) {
					ip2 = atoi(buf);
					if(flash.clientIp[1]!=ip2) {
						flash.clientIp[1]=ip2;
						w5500InitFLag=1;
					}
				} else if(iteam==2) {
					ip3 = atoi(buf);
					if(flash.clientIp[2]!=ip3) {
						flash.clientIp[2]=ip3;
						w5500InitFLag=1;
					}
				} else if(iteam==3) {
					ip4 = atoi(buf);
					if(flash.clientIp[3]!=ip4) {						
						flash.clientIp[3]=ip4;
						w5500InitFLag=1;
					}
					if(w5500InitFLag==1) {
						flash.clientGate[0]=flash.clientIp[0];
						flash.clientGate[1]=flash.clientIp[1];
						flash.clientGate[2]=flash.clientIp[2];
						flash.clientGate[3]=1;
						//W5500_Init();						
						Dprintf("SET CLIENT IP = %d.%d.%d.%d\n",flash.clientIp[0],flash.clientIp[1],flash.clientIp[2],flash.clientIp[3]);
					}
					return 0;
				}
				cnt=0;
				iteam++;
				for(int c; c<10; c++) {
					buf[c]=0;
				}
			}
		}
	}
    return 0;
}
int Json_TimeSet(int address) {                         //시간설정
	unsigned char Data[10];
	int key_value=0;
	uint8_t 	data;

	Data[0] = gv.json.valueList[address][0];    
	Data[1] = gv.json.valueList[address][1];
	Data[2] = gv.json.valueList[address][2];    
	Data[3] = gv.json.valueList[address][3];
	Data[4] = 0;
	key_value = atoi((char *)Data);
	if(key_value < 2000 || key_value > 2099)
	{
		Dprintf("Range Error", "Year Input Error\n");
        return -1;
	}
	else
	{
		data = IntToBCD(key_value-2000);
		DS1302_YearSet(data);
	}	
	
	Data[0] = gv.json.valueList[address][5];    
	Data[1] = gv.json.valueList[address][6];
	Data[2] = 0;
	key_value = atoi((char *)Data);
	if(key_value < 1 || key_value > 12)
	{
		Dprintf("Range Error", "Month Input Error\n");
        return -1;
	}
	else
	{
		data = IntToBCD(key_value);
		DS1302_MonthSet(data);
	}	
     

	Data[0] = gv.json.valueList[address][8];    
	Data[1] = gv.json.valueList[address][9];
	Data[2] = 0;
	key_value = atoi((char *)Data);
	if(key_value < 1 || key_value > 31)
	{
		Dprintf("Range Error", "Day Input Error\n");
        return -1;
	}
	else
	{
		data = IntToBCD(key_value);
		DS1302_DaySet(data);
	}	

	Data[0] = gv.json.valueList[address][11];    
	Data[1] = gv.json.valueList[address][12];
	Data[2] = 0;
	key_value = atoi((char *)Data);
	if(key_value < 0 || key_value > 24)
	{
		Dprintf("Range Error", "Hour Input Error\n");
        return -1;
	}
	else
	{
		data = IntToBCD(key_value);
		DS1302_HourSet(data);
	}	

	Data[0] = gv.json.valueList[address][14];    
	Data[1] = gv.json.valueList[address][15];
	Data[2] = 0;
	key_value = atoi((char *)Data);
	if(key_value < 0 || key_value > 59)
	{
		Dprintf("Range Error", "Minute Input Error\n");
        return -1;
	}
	else
	{
		data = IntToBCD(key_value);
		DS1302_MinSet(data);
	}	
	Data[0] = gv.json.valueList[address][17];    
	Data[1] = gv.json.valueList[address][18];
	Data[2] = 0;
	key_value = atoi((char *)Data);
	if(key_value < 0 || key_value > 59)
	{
		Dprintf("Range Error", "Second Input Error\n");
        return -1;
	}
	else
	{
		data = IntToBCD(key_value);
		DS1302_SecSet(data);
	}	
    return 0;
}
void ReceiveDumpTimeProcess(int time, int address) {    //덤프요청 시작, 종료시간
	char date_buf[20];
	if(time==0) { //start time
		date_buf[0] = gv.json.valueList[address][2];    
		date_buf[1] = gv.json.valueList[address][3];
		date_buf[2] = 0;
		gv.dumpStartDate.year = atoi(date_buf);
		
		date_buf[0] = gv.json.valueList[address][5];    
		date_buf[1] = gv.json.valueList[address][6];
		date_buf[2] = 0;
		gv.dumpStartDate.month = atoi(date_buf);           

		date_buf[0] = gv.json.valueList[address][8];    
		date_buf[1] = gv.json.valueList[address][9];
		date_buf[2] = 0;
		gv.dumpStartDate.date = atoi(date_buf);           

		date_buf[0] = gv.json.valueList[address][11];    
		date_buf[1] = gv.json.valueList[address][12];
		date_buf[2] = 0;
		gv.dumpStartDate.hour = atoi(date_buf);           

		date_buf[0] = gv.json.valueList[address][14];    
		date_buf[1] = gv.json.valueList[address][15];
		date_buf[2] = 0;
		gv.dumpStartDate.minute = atoi(date_buf);           
	} else if(time==1) {
		//종료시간
		date_buf[0] = gv.json.valueList[address][2];    
		date_buf[1] = gv.json.valueList[address][3];
		date_buf[2] = 0;
		gv.dumpEndDate.year = atoi(date_buf);
		
		date_buf[0] = gv.json.valueList[address][5];    
		date_buf[1] = gv.json.valueList[address][6];
		date_buf[2] = 0;
		gv.dumpEndDate.month = atoi(date_buf);                

		date_buf[0] = gv.json.valueList[address][8];    
		date_buf[1] = gv.json.valueList[address][9]; 
		date_buf[2] = 0;
		gv.dumpEndDate.date = atoi(date_buf);                

		date_buf[0] = gv.json.valueList[address][11];    
		date_buf[1] = gv.json.valueList[address][12];
		date_buf[2] = 0;
		gv.dumpEndDate.hour = atoi(date_buf);                

		date_buf[0] = gv.json.valueList[address][14];    
		date_buf[1] = gv.json.valueList[address][15];
		date_buf[2] = 0;
		gv.dumpEndDate.minute = atoi(date_buf);    
	}
}
int Json_Parsing(int size) {
	char commandBuf[32];
	char valueBuf[32];
	int addrCnt=0;
	int itemCnt=0;
	//int value=0;
	int step=0;//전문 분석 스텝
	
	for(int i=0; i<32; i++) {
        commandBuf[i]=0;
        valueBuf[i]=0;
    }
	for(int bufCnt=0; bufCnt<size; bufCnt++) {
		if(gv.transmitBuf[bufCnt]==0x22) {
			if(step==0){ 			
				step=1;
				bufCnt++;
			} else if(step==1) {
				strcpy(gv.json.cmdList[itemCnt],commandBuf);
				addrCnt=0;
				step=2;
			} else if(step==2) {
				step=3;
				bufCnt++;
			} else if(step==3) {
				strcpy(gv.json.valueList[itemCnt],valueBuf);
				if (!strcmp("group",gv.json.cmdList[itemCnt])) {                    //지역 ID 001~999                    
					flash.json.groupSv = atoi(gv.json.valueList[itemCnt]);
				} else if (!strcmp("site",gv.json.cmdList[itemCnt])) {              //저수지 ID 001~999
					flash.json.siteSv = atoi(gv.json.valueList[itemCnt]);			
				} else if (!strcmp("device",gv.json.cmdList[itemCnt])) {            //RTU장비 ID 001~999
					flash.json.diviceSv = atoi(gv.json.valueList[itemCnt]);
				} else if (!strcmp("serverIp",gv.json.cmdList[itemCnt])) {          //SERVER IP SET
					strcpy(gv.json.serverIpPv,gv.json.valueList[itemCnt]);
					strcat(gv.json.serverIpPv,".");
					Json_IpSet(0);//0이면 SERVER IP SET//1이면 CLIENT IP SET
				} else if (!strcmp("serverPort",gv.json.cmdList[itemCnt])) {        //SERVER PORT SET
					gv.json.serverPortPv = atoi(gv.json.valueList[itemCnt]);
					flash.serverPort[0] = gv.json.serverPortPv;
				} else if (!strcmp("clientIp",gv.json.cmdList[itemCnt])) {          //SERVER IP SET
					strcpy(gv.json.clientIpPv,gv.json.valueList[itemCnt]);
					strcat(gv.json.clientIpPv,".");
					Json_IpSet(1);//0이면 SERVER IP SET//1이면 CLIENT IP SET
				} else if (!strcmp("clientPort",gv.json.cmdList[itemCnt])) {        //SERVER PORT SET
					gv.json.clientPortPv = atoi(gv.json.valueList[itemCnt]);
					flash.clientPort = gv.json.clientPortPv;
				} else if (!strcmp("rtu_time_set",gv.json.valueList[itemCnt])) {    //RTU 설정정보
					Json_TimeSet(itemCnt);
				} else if (!strcmp("",gv.json.valueList[itemCnt])) {				//RTU 설정정보

				} else if (!strcmp("deviceVersion",gv.json.cmdList[itemCnt])) {	    //펌웨어버젼 20201010					
					strcpy(flash.json.deviceVersionSv,gv.json.valueList[itemCnt]);
				} else if (!strcmp("rtu_info",gv.json.cmdList[itemCnt])) {			//명령형태
				} else if (!strcmp("server_ack",gv.json.cmdList[itemCnt])) {		//서버 ACK
				} else if (!strcmp("server_nak",gv.json.cmdList[itemCnt])) {		//서버 NAK
				} else if (!strcmp("rtu_set",gv.json.valueList[itemCnt])) {			//LAN 서버 파라메터 전송
					gv.json.nextSendCmdNo=3;
				} else if (!strcmp("time_set",gv.json.cmdList[itemCnt])) {			//서버 파라메터 전송 시간정보설정
					Json_TimeSet(itemCnt);
				} else if (!strcmp("server_set_complit",gv.json.valueList[itemCnt])) {	//서버 파라메터 전송 완료
				} else if (!strcmp("rtu_set_complit",gv.json.valueList[itemCnt])) {	    //서버 파라메터 전송 완료
				} else if (!strcmp("request_dump_data",gv.json.valueList[itemCnt])) {	//서버 덤프요청
					gv.json.nextSendCmdNo=1;
				} else if (!strcmp("start_time",gv.json.cmdList[itemCnt])) {		//서버 덤프시작시간
					ReceiveDumpTimeProcess(0, itemCnt);
				} else if (!strcmp("end_time",gv.json.cmdList[itemCnt])) {			//서버 덤프종료시간
					ReceiveDumpTimeProcess(1, itemCnt);
				} else if (!strcmp("server_eot",gv.json.valueList[itemCnt])) {		//송신종료
					gv.json.nextSendCmdNo=10;
				} else if (!strcmp("firmware_update",gv.json.valueList[itemCnt])) {	//펌웨어업데이트
					gv.json.nextSendCmdNo=4;
					gv.downloadFlag=0;
					gv.downloadDataSize=0;
				} else if (!strcmp("firmware_size",gv.json.cmdList[itemCnt])) {	    //펌웨어사이즈
					gv.json.firmwareSize=atoi(gv.json.valueList[itemCnt]);
				} else if (!strcmp("server_enq",gv.json.valueList[itemCnt])) {	    //서버가 접속했다.
					gv.json.nextSendCmdNo=2;
				} else if (!strcmp("rtu_debug",gv.json.valueList[itemCnt])) {	    //RTU로 부터 DEBUG 데이터를 수신한다.
					gv.json.nextSendCmdNo=25;
                    gv.jsonTcpDebugFlag=1;            
                }
                Dprintf("[%d]명령어 = %s [%d]설정값 = %s\n",itemCnt,gv.json.cmdList[itemCnt],itemCnt,gv.json.valueList[itemCnt]);
                itemCnt++;
                addrCnt=0;
                step=0;
            }			
        }            
		if(gv.transmitBuf[bufCnt]==']') { //분석완료 후 gv.json.nextSendCmdNo 정보를 이용하여 송신 절차를 진행한다.
			switch(gv.json.nextSendCmdNo) {
				case 1: {
					if(RESULT_OK==JSON_DUMP_Searchforsendingfiles(gv.dumpStartDate,gv.dumpEndDate)) {//파일읽기
						Json_Format_Sending("response_dump_data");//측정값 송신전에 헤더정보
						JSON_RAM_ADDRESS = SDREAD_RAM;
						gv.json.sendingStep=3;//송신스텝		
						gv.json.sendingCount=0;//송신카운트				
					} else {
                        EventMessage("[ ERR ] 파일 쓰기\n");
						gv.json.nextSendCmdNo=11;
						Dprintf("파일이없습니다.\n");
						Json_Format_Sending("dump_nak");
					}
				}break;
				case 2:	{//ENQ 응답 송신데이터 생성  
					Json_Format_Sending("rtu_info");
				}break;
				case 3:	{//서버 to RTU 파라메터 설정
					Json_rtu_parameterSendProcess();
				}break;	
				case 4: {//펌웨어업데이트
					Json_Format_Sending("firmware_update_ack");
				}
				break;
				case 6: {//EOT 송신
					Json_Format_Sending("rtu_eot");
				}
				break;
				case 10: { //송수신완료
						
				}
				break;
				case 11: { //DUMP 데이터
					
				}
				break;
				case 24: { //RTU RESET
					Json_Format_Sending("rtu_reset_ack");
					SystemRebootGo();
					
				}break;
                case 25: { //RTU DEBUG MODE
                    Json_Format_Sending("rtu_reset_ack");
                }break;
			}	
			return 0;
		}
		if(step==1) {
			commandBuf[addrCnt++]=gv.transmitBuf[bufCnt];
			commandBuf[addrCnt]=0;
		}
		if(step==3) {
			valueBuf[addrCnt++]=gv.transmitBuf[bufCnt];
			valueBuf[addrCnt]=0;
		}	
	}
	return 0;    
}
void FloatPoint_JsonFormatProcess(int ch, int comPort,  char *data) {       
    if(flash.floatPoint[ch]==0) {
        sprintf(data,",\"data%02d\":\"%.0f\"",ch,gv.averPV5Min[comPort][ch]); 
    }
    else if(flash.floatPoint[ch]==1) {
        sprintf(data,",\"data%02d\":\"%.1f\"",ch,gv.averPV5Min[comPort][ch]); 
    }
    else if(flash.floatPoint[ch]==2) {
        sprintf(data,",\"data%02d\":\"%.2f\"",ch,gv.averPV5Min[comPort][ch]); 
    }
    else if(flash.floatPoint[ch]==3) {
        sprintf(data,",\"data%02d\":\"%.3f\"",ch,gv.averPV5Min[comPort][ch]); 
    }
    else if(flash.floatPoint[ch]==4) {
        sprintf(data,",\"data%02d\":\"%.4f\"",ch,gv.averPV5Min[comPort][ch]); 
    } 
    else {
        sprintf(data,",\"data%02d\":\"%.0f\"",ch,gv.averPV5Min[comPort][ch]); 
    }
}
int Json_Dump_FileCreate(RealTime saveDate) {
    unsigned int saveDateInt=0;
    int comPort = USART_1;
    char sbuf[128];
    for(int i=0;i<128;i++)sbuf[i]=0;
	for(int i=0;i<SMARTLOG_DATALOG_SIZE;i++) {
		gv.dataLogBuf[i]=0;
	}
    //검색정보
    saveDateInt = (saveDate.year*599040) + (saveDate.month*46080) + (saveDate.date*1440) + (saveDate.hour*60) + (saveDate.minute*1);
    sprintf(sbuf,"*%010d",saveDateInt);
    strcat((char *)gv.dataLogBuf,sbuf);
    //현재시간
    sprintf(sbuf,"{\"date\":\"20%02d-%02d-%02d %02d:%02d:%02d\"",saveDate.year,saveDate.month,saveDate.date,saveDate.hour,saveDate.minute,0);
    strcat((char *)gv.dataLogBuf,sbuf);
    
    
    FloatPoint_JsonFormatProcess(0, comPort, sbuf);//TEMP
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(1, comPort, sbuf);//COD
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(2, comPort, sbuf);//TUR
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(3, comPort, sbuf);//NO3
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(4, comPort, sbuf);//TOC
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(5, comPort, sbuf);//COND
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(6, comPort, sbuf);//DEPTH
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(7, comPort, sbuf);//PRESSURE
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(8, comPort, sbuf);//사용전압
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(9, comPort, sbuf);//사용전류
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(10, comPort, sbuf);//배터리잔량
    strcat((char *)gv.dataLogBuf,sbuf);
    FloatPoint_JsonFormatProcess(11, comPort, sbuf);//배터리전압
    strcat((char *)gv.dataLogBuf,sbuf);

    sbuf[0]='}';
    sbuf[1]=',';
    sbuf[2]=0x0D;
    sbuf[3]=0;
    strcat((char *)gv.dataLogBuf,sbuf);
    return 0;
}
int JSON_DUMP_Searchforsendingfiles(RealTime startDateSv, RealTime endDateSv) {
    RealTime startDatePv=startDateSv;
    RealTime endDatePv=endDateSv;
    unsigned int findDumpStartTime=0, findDumpEndTime=0, newFindDumpTime=0;
    int status=0;
    char tbuf[32];
    for(int i=0; i<32; i++) {
        tbuf[i]=0;
    }
	findDumpStartTime=(startDatePv.year*599040) + (startDatePv.month*46080) + (startDatePv.date*1440) + (startDatePv.hour*60) + (startDatePv.minute*1);
	findDumpEndTime=(endDatePv.year*599040) + (endDatePv.month*46080) + (endDatePv.date*1440) + (endDatePv.hour*60) + (endDatePv.minute*1);
	KIM_RAM_ADDRESS = SDREAD_RAM;
	JSON_RAM_ADDRESS = SDREAD_RAM;
	
    gv.json.sendingSize=0;
	
	status=DUMP_SDCardTORAM(startDatePv, endDatePv);
    
	if(status != RESULT_OK) { //파일이 존재하지 않는 경우 처리
		Dprintf("재검색을 시작한다.\n");
		for(int i=1; i<13; i++) { //12개월을 체크한다.
			if(DUMP_DateChecking(startDatePv,endDatePv) == 1) { //일 을 업데이트 하고 재검색한다.
				startDatePv = gv.dumpStartDate;//gv.dumpStartDate 덤프요청시작시간원본
				Dprintf("재검색 정보 YEAR = %d MONTH = %d\n",startDatePv.year,startDatePv.month);
				status=DUMP_SDCardTORAM(startDatePv, endDatePv);
				if(status==RESULT_FILE_OPEN_ERROR) {
					Dprintf("[ REERR ] 파일 열기\n");
                    EventMessage("[ REERR] 파일 열기 오류\n");
				} else if(status==RESULT_SD_MOUNT_ERROR) {
					Dprintf("[ REERR ] SDCARD MOUNT\n");
                    EventMessage("[ REERR ] SDCARD MOUNT ERROR\n");
				} else if(status==RESULT_FILE_WRITE_ERROR) {
					Dprintf("[ REERR ] 파일 쓰기\n");
                    EventMessage("[ REERR ] 파일 쓰기 오류\n");
				} else {
					Dprintf("[RE]정상 적으로 파일을 읽었습니다.\n");
					break;
				}
			} else { //에러처리
				if(status==RESULT_FILE_OPEN_ERROR) {
					Dprintf("[ ERR ] 파일 열기\n");
                    EventMessage("[ ERR ] 파일 열기\n");
					return RESULT_SD_ERROR;
				} else if(status==RESULT_SD_MOUNT_ERROR) {
                    EventMessage("[ ERR ] 파일 열기\n");
					Dprintf("[ ERR ] 파일 열기\n");
					return RESULT_SD_ERROR;
				} else if(status==RESULT_FILE_WRITE_ERROR) {
					Dprintf("[ ERR ] 파일 쓰기\n");
                    EventMessage("[ ERR ] 파일 쓰기\n");
					return RESULT_SD_ERROR;
				} else {
					Dprintf("정상 적으로 파일을 읽었습니다.\n");
				}
			}//error
		}//for
	}
    for(int i=0; i < gv.openFileSize; i++)
    {
        if(*(U8 *)KIM_RAM_ADDRESS == '*')
        {
			KIM_RAM_ADDRESS++;
			tbuf[0]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[1]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[2]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[3]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[4]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[5]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[6]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[7]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[8]=*(unsigned char *)KIM_RAM_ADDRESS;
			KIM_RAM_ADDRESS++;
			tbuf[9]=*(unsigned char *)KIM_RAM_ADDRESS;
			tbuf[10]=0;

			newFindDumpTime=atoi(tbuf);
			if(findDumpStartTime <= newFindDumpTime && findDumpEndTime >= newFindDumpTime) {
				while(1) {
					KIM_RAM_ADDRESS++;					
					if(*(U8 *)KIM_RAM_ADDRESS==0x0D) {
						break;
					} else {
						*(U8 *)JSON_RAM_ADDRESS = *(U8 *)KIM_RAM_ADDRESS;                        
						JSON_RAM_ADDRESS++;
						gv.json.sendingSize++;
					}
				}
			}
			if(findDumpEndTime <= newFindDumpTime) {
				if(gv.json.sendingSize != 0) {
					JSON_RAM_ADDRESS--;				
					*(U8 *)JSON_RAM_ADDRESS=']';
					return RESULT_OK;
				} else {					
					return RESULT_NOFILE;
				}
			}
        }        
		KIM_RAM_ADDRESS++;
    }
	if(findDumpEndTime>=newFindDumpTime) {
        if(gv.json.sendingSize != 0) {
            JSON_RAM_ADDRESS--;				
            *(U8 *)JSON_RAM_ADDRESS=']';
            return RESULT_OK;
        } else {					
            return RESULT_NOFILE;
        }
	}
	Dprintf("파일읽기종료\n");
    return RESULT_NOFILE;//데이터가 저장이 안된것으로 본다.
}

#endif

