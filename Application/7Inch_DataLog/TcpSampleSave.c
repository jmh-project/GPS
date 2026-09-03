#include "TEECO_System.h"
#include "stm32f4xx.h"
#include "Debug_Uart.h"
#include "windows.h"
#include "dos.h"
#include <rt_misc.h>
#include "DataLog.h"

U32 KIM_RAM_ADDRESS;
#ifdef  JSON_ENABLE
U32 JSON_RAM_ADDRESS;
#endif
extern FATFS sd_fatfs;
extern FIL sd_file;
extern char CurrenrDir[64];
extern char CurrenrDisk[16];
extern char Disk_Kind[16];
extern BYTE Disk_Buffer[MEMORY_BUF_SIZE];/* File copy buffer */
#ifdef KECO_PROTOCOL_ENABLE
extern int KECO_DUMA_FileCreate(int comPort);
#endif
#ifdef JSON_ENABLE
int Json_Dump_FileCreate(RealTime saveDate);
#endif

int Uart_TransmitPort( unsigned int uartPort, unsigned char data ) { 
    switch( uartPort )
    {
        case USART_1: SENSOR_UART1( data ); break;
        case USART_2: SENSOR_UART2( data ); break;
        case USART_3: SENSOR_UART3( data ); break;
        case USART_4: SENSOR_UART4( data ); break;
    }
    return 0;
}
int DUMP_DateChecking(RealTime startDatePvSv, RealTime endDateSv) {
    RealTime startDatePv = startDatePvSv;
    RealTime endDatePv = endDateSv;
	//startDatePv, endDate :: 덤프요청시간
	if(startDatePv.month==endDatePv.month && startDatePv.year==endDatePv.year) {	
		Dprintf("년월일이 같습니다.\n");
		return 0;//NAK 처리한다.
	} else {
		if(startDatePv.year < endDatePv.year) {//년이 변경되는 경우
			if(startDatePv.month >= 12) {
				startDatePv.year++;
				startDatePv.month=1;
                gv.dumpStartDate = startDatePv;
			}
			//Dprintf("year = %d month = %d\n",startDatePv.year,startDatePv.month);
			return 1;//재검색한다.
		} else if (startDatePv.year > endDatePv.year) {			
			startDatePv.year=endDatePv.year;
			startDatePv.month=endDatePv.month;
            gv.dumpStartDate = startDatePv;
			return 1;//재검색
		} else if(startDatePv.year == endDatePv.year) {//월이변경되는경우
			startDatePv.month++;
			if(startDatePv.month > endDatePv.month) {
				return 0;//검색실패
			} else {
                gv.dumpStartDate = startDatePv;
				return 1;//재검색
			}
		} else {
			return 0;
		} 
	}
}

int DataLogTransmitFileToSDCard(RealTime saveTime) {//정렬된 송신파일정보 저장
    char save_file_name[100];
    char buf[100];
    for(int i=0; i<100; i++) {
        save_file_name[i]=0;
        buf[i]=0;
    }    
    int bufSize =  strlen((char *)gv.dataLogBuf);
    if(bufSize > SMARTLOG_DATALOG_SIZE) {
        EventMessage("DUMP 파일 생성시 dataLogBuf Overflow 발생 초기화");
        for(int i=0; i<SMARTLOG_DATALOG_SIZE; i++) {
            gv.dataLogBuf[i]=0;
        }
    } 
    else {
        sprintf(save_file_name,"1:DataLogTransmitFile%02d%02d.txt",saveTime.year, saveTime.month);
		StringToSDCardWrite(save_file_name,bufSize);    
        Dprintf("DUMP 20%02d/%02d/%02d %02d:%02d:%02d   %s Size = %d\n",
        saveTime.year,saveTime.month,saveTime.date,saveTime.hour,saveTime.minute,0,save_file_name,bufSize);
        
        sprintf(buf,"20%02d/%02d/%02d %02d:%02d%s\n",
        saveTime.year,saveTime.month,saveTime.date,saveTime.hour,saveTime.minute,save_file_name);
        DebugMessagePrintf(buf);
    }
    return 0;
}
int DumpFileCreate(RealTime saveDate) {		//0. 파일 생성후 SDCRAD 에 저장    
    flash.dumpNewDate = saveDate;			//1. DATA 요청시 시간정보    
	#ifdef KECO_PROTOCOL_ENABLE
	if(flash.Uart[USART_4]->SensorID == SENSOR_KECO) {	
		KECO_DUMA_FileCreate(USART_1);		//2. 데이터 생성
		DataLogTransmitFileToSDCard(saveDate);	//3. 저장
	}
	else {
		Dprintf("TMS 덤프 파일 생성\n");
	}
    #endif
    #if defined TYPE_APPROVAL_ENABLE
	if(flash.Uart[USART_4]->SensorID == SENSOR_TMS) {
		TYPE_APPROVAL_DUMA_FileCreate(USART_1);
		DataLogTransmitFileToSDCard(saveDate);	//3. 저장
	}
	else {
		Dprintf("KECO 덤프 파일 생성\n");
	}
	#endif
	#ifdef JSON_ENABLE
        Json_Dump_FileCreate(saveDate);
		DataLogTransmitFileToSDCard(saveDate);	//3. 저장
    #endif
	
    return 0;
}

int DUMP_SDCardTORAM(RealTime startTime, RealTime endTime) {//덤프 데이터를 RAM에 읽어들인다.
	FRESULT	fr;
	char src_file_name[128];
    char save_file_name[128];
    FIL fsrc;	    /* File objects */	
	UINT br;         /* File read/write count */	
    U32 address = SDREAD_RAM;
	int sdInitCountPv=0,sdInitCountSv=20;
	int i;
	//int	ret= -1;
    int file_size=0;
    
	int startYear = startTime.year;
	int startMonth = startTime.month;
	int endYear = endTime.year;
	int endMonth = endTime.month;
	
	int loop=0;
	
	int year=startYear;
	int month=startMonth;
	int loopOutFlag=0;
	
    for(i=0;i<128;i++) {
        src_file_name[i]=0;
        save_file_name[i]=0;
    }
	int returnMessage=0;
	/**
	*	SDCARD MOUNT
	*	MOUNT 가 안되는 경우 20회 까지 재 실행 한다.
	*/
	while(1) {
		if( f_mount(&sd_fatfs, "1:", 1) != FR_OK ) {
			Dprintf("ERR. Mount = %d\n",sdInitCountPv);
			sdInitCountPv++;
			if(sdInitCountPv > sdInitCountSv) {
				Dprintf("ERR. SD RESTART [ %s ]\n", src_file_name);
				f_mount(0, Disk_Kind, 0);
				return RESULT_SD_MOUNT_ERROR;
			}
		}
		else break;
	}
    
    sprintf(save_file_name,"DataLogTransmitFile%02d%02d.txt",startTime.year, startTime.month);        
	sprintf(src_file_name,"%s%s", Disk_Kind, save_file_name);
	Dprintf("오픈파일이름 = %s\n",src_file_name);

	fr = f_open(&fsrc, src_file_name, FA_OPEN_EXISTING | FA_READ);
	if(fr != FR_OK) {
		Dprintf("[File Open Error] %s\n",src_file_name);
		returnMessage=RESULT_FILE_OPEN_ERROR;
	} else {	
		gv.openFileSize = f_size(&fsrc);//첫번째 읽어들인 데이터의 파일 크기
		file_size=0;
		for (;;) 
		{
			fr = f_read(&fsrc, Disk_Buffer, _MAX_SS, &br);/* Read a chunk of source file */
			if (fr || br == 0)  {break; }/* error or eof */
			for(int i=0; i<br; i++)
			{
				(*(U8 *)address++) = Disk_Buffer[i];
			}
			file_size += br;
		}   
		if(br != _MAX_SS)
		{
			for(int i=0; i<br; i++)
			{
				(*(U8 *)address++) = Disk_Buffer[i];
			}
			file_size += br;
		}
		returnMessage=RESULT_OK;
	}
	f_close(&fsrc);
	
	if(startMonth!=endMonth) {
		loop=1;
	} else {
		loop=0;
	}
	
	while(loop) {
		month++;
		if(month==13) {
			year++;
			month=1;
		}
		if(month==endMonth && year==endYear) {
			loopOutFlag=1;//endMonth 까지 데이를 읽고 리턴한다.
		}
		
        sprintf(save_file_name,"DataLogTransmitFile%02d%02d.txt",year, month);        
		sprintf(src_file_name,"%s%s", Disk_Kind, save_file_name);
		Dprintf("오픈파일이름 = %s\n",src_file_name);
        
		fr = f_open(&fsrc, src_file_name, FA_OPEN_EXISTING | FA_READ);
		if(fr != FR_OK) {
			Dprintf("[File Open Error] %s\n",src_file_name);
		} else {	
			gv.openFileSize += f_size(&fsrc);//첫번째 읽어들인 데이터의 파일 크기(2023.01.31 수정)
			file_size=0;
			for (;;) 
			{
				fr = f_read(&fsrc, Disk_Buffer, _MAX_SS, &br);/* Read a chunk of source file */
				if (fr || br == 0)  {break; }/* error or eof */
				for(int i=0; i<br; i++)
				{
					(*(U8 *)address++) = Disk_Buffer[i];
				}
				file_size += br;
			}   
			if(br != _MAX_SS)
			{
				for(int i=0; i<br; i++)
				{
					(*(U8 *)address++) = Disk_Buffer[i];
				}
				file_size += br;
			}
			returnMessage=RESULT_OK;
		}
		f_close(&fsrc);
		
		if(loopOutFlag==1){
			loop=0;
		}
	}	
	//SDCARD 마운트해제
    f_mount(0, Disk_Kind, 0);
	
	return returnMessage;
}

int DUMP_SDCardTOBuffer(RealTime startTime, RealTime endTime) {//RAM 에 읽어들인다. 시작시간 종료시간
    int status=0;
    KIM_RAM_ADDRESS = SDREAD_RAM;
    Dprintf("SDCARD 에서 데이터을 모두 읽어온다.\n");
    status=DUMP_SDCardTORAM( startTime, endTime);
    if(status==RESULT_FILE_OPEN_ERROR) {
        Dprintf("[ ERR ] 파일 열기\n");
        return RESULT_SD_ERROR;
    } else if(status==RESULT_SD_MOUNT_ERROR) {
        Dprintf("[ ERR ] SDCARD MOUNT\n");
        return RESULT_SD_ERROR;
    } else if(status==RESULT_FILE_WRITE_ERROR) {
        Dprintf("[ ERR ] 파일 쓰기\n");
        return RESULT_SD_ERROR;
    } else {
        Dprintf("정상 적으로 파일을 읽었습니다.\n");
    }
    //Dprintf("오픈파일크기 = %d\n", gv.openFileSize);
    return RESULT_OK;
}
int DUMP_BufferTOSend(RealTime startTime, RealTime endTime, int dumpMode) { //DUMP 데이터 송신
	unsigned int intDatePv=0;	
	unsigned int intDateSv=0;	
    
    char tbuf[20];
    for(int i=0;i<SMARTLOG_DATALOG_SIZE;i++) {
        gv.transmitBuf[i]=0;
    }
    if(dumpMode==2) {//덤프 데이터 송신 시 1회 실행
        gv.dumpStartTimePv=(startTime.year*599040) + (startTime.month*46080) + (startTime.date*1440) + (startTime.hour*60) + (startTime.minute*1);
        gv.dumpEndTimePv=(endTime.year*599040) + (endTime.month*46080) + (endTime.date*1440) + (endTime.hour*60) + (endTime.minute*1);
        gv.openFileSizeCnt=0;
    }
    if(dumpMode==3) {
        //DUMP 데이터 송신 중
    }
    if(dumpMode==1) {//DATA5MIN_SEND
        intDateSv=(startTime.year*599040) + (startTime.month*46080) + (startTime.date*1440) + (startTime.hour*60) + (startTime.minute*1);
        //endDateSv=(endTime.year*599040) + (endTime.month*46080) + (endTime.date*1440) + (endTime.hour*60) + (endTime.minute*1);
        gv.openFileSizeCnt=0;
    }
    for( ; gv.openFileSizeCnt < gv.openFileSize;)//전역변수로 gOpenFileSize 현재 읽어들인 파일의 크기이다.
    {
        if(*(U8 *)KIM_RAM_ADDRESS == '*')
        {
            for(int i=0; i<10; i++) {
                KIM_RAM_ADDRESS++;
                gv.openFileSizeCnt++;
                tbuf[i]=*(unsigned char *)KIM_RAM_ADDRESS;
            }
            tbuf[10]=0;    
			intDatePv=atoi(tbuf);
            if(dumpMode==1) {//5분 평균데이터저장
                if(intDatePv==intDateSv) {
                    int cnt=0;
                    while(1) {
                        KIM_RAM_ADDRESS++;
                        gv.openFileSizeCnt++;
                        gv.transmitBuf[cnt++]=*(U8 *)KIM_RAM_ADDRESS;
                        //Dputc(*(U8 *)KIM_RAM_ADDRESS);
                        if(*(U8 *)KIM_RAM_ADDRESS==0x0D) {
                            gv.lanDataTransmitSize=cnt;
                            return RESULT_OK;
                        }
                    }
                }
            } else if(dumpMode==2||dumpMode==3) {//누락 데이터 송신(DUMP)
                if(intDatePv>=gv.dumpStartTimePv && intDatePv <= gv.dumpEndTimePv) {
                    int cnt=0;
                    while(1) {
                        KIM_RAM_ADDRESS++;
                        gv.openFileSizeCnt++;
                        gv.transmitBuf[cnt++]=*(U8 *)KIM_RAM_ADDRESS;
                        //Dputc(*(U8 *)KIM_RAM_ADDRESS);
                        if(*(U8 *)KIM_RAM_ADDRESS==0x0D) {
                            gv.lanDataTransmitSize=cnt;
                            gv.dumpStartTimePv=intDatePv;//현재 송신하는 데이터의 시간값 ( 주소가 증가 했다고 본다. )
                            return RESULT_OK;
                        }
                    }
                }
            }
        } else {
            KIM_RAM_ADDRESS++;
            gv.openFileSizeCnt++;
        }            
    }
    Dprintf("NO FILE\n");
    return RESULT_NOFILE;//데이터가 저장이 안된것으로 본다.
}

int Average5MinDataReading(RealTime startTime, RealTime endTime) {//덤프 데이터를 RAM에 읽어들인다.
	FRESULT	fr;
	char src_file_name[128];
    char save_file_name[128];
    FIL fsrc;	    /* File objects */	
	UINT br;         /* File read/write count */	
    U32 address = SDREAD_RAM;
	int sdInitCountPv=0,sdInitCountSv=20;
	int i;
	//int	ret= -1;
    int file_size=0;
    
	int startYear = startTime.year;
	int startMonth = startTime.month;
	int endYear = endTime.year;
	int endMonth = endTime.month;
	
	int loop=0;
	
	int year=startYear;
	int month=startMonth;
	int loopOutFlag=0;
	
    for(i=0;i<128;i++) {
        src_file_name[i]=0;
        save_file_name[i]=0;
    }
	int returnMessage=0;
	/**
	*	SDCARD MOUNT
	*	MOUNT 가 안되는 경우 20회 까지 재 실행 한다.
	*/
	while(1) {
		if( f_mount(&sd_fatfs, "1:", 1) != FR_OK ) {
			Dprintf("ERR. Mount = %d\n",sdInitCountPv);
			sdInitCountPv++;
			if(sdInitCountPv > sdInitCountSv) {
				Dprintf("ERR. SD RESTART [ %s ]\n", src_file_name);
				f_mount(0, Disk_Kind, 0);
				return RESULT_SD_MOUNT_ERROR;
			}
		}
		else break;
	}
    sprintf(save_file_name,"AverageMinData%02d%02d.csv",startTime.year, startTime.month);        
	sprintf(src_file_name,"%s%s", Disk_Kind, save_file_name);
	Dprintf("오픈파일이름 = %s\n",src_file_name);

	fr = f_open(&fsrc, src_file_name, FA_OPEN_EXISTING | FA_READ);
	if(fr != FR_OK) {
		Dprintf("[File Open Error] %s\n",src_file_name);
		returnMessage=RESULT_FILE_OPEN_ERROR;
	} else {	
		gv.openFileSize = f_size(&fsrc);//첫번째 읽어들인 데이터의 파일 크기
		file_size=0;
		for (;;) 
		{
			fr = f_read(&fsrc, Disk_Buffer, 512, &br);/* Read a chunk of source file */
			if (fr || br == 0)  {break; }/* error or eof */
			for(int i=0; i<br; i++)
			{
				(*(U8 *)address++) = Disk_Buffer[i];
			}
			file_size += br;
		}   
		if(br != 512)
		{
			for(int i=0; i<br; i++)
			{
				(*(U8 *)address++) = Disk_Buffer[i];
			}
			file_size += br;
		}
		returnMessage=RESULT_OK;
	}
	f_close(&fsrc);
	
	if(startMonth!=endMonth) {
		loop=1;
	} else {
		loop=0;
	}
	
	while(loop) {
		month++;
		if(month==13) {
			year++;
			month=1;
		}
		if(month==endMonth && year==endYear) {
			loopOutFlag=1;//endMonth 까지 데이를 읽고 리턴한다.
		}
		
		sprintf(save_file_name,"AverageMinData%02d%02d.csv",year, month);
		
		sprintf(src_file_name,"%s%s", Disk_Kind, save_file_name);
		Dprintf("오픈파일이름 = %s\n",src_file_name);
        
		fr = f_open(&fsrc, src_file_name, FA_OPEN_EXISTING | FA_READ);
		if(fr != FR_OK) {
			Dprintf("[File Open Error] %s\n",src_file_name);
		} else {	
			gv.openFileSize += f_size(&fsrc);//첫번째 읽어들인 데이터의 파일 크기(2023.01.31 수정)
			file_size=0;
			for (;;) 
			{
				fr = f_read(&fsrc, Disk_Buffer, 512, &br);/* Read a chunk of source file */
				if (fr || br == 0)  {break; }/* error or eof */
				for(int i=0; i<br; i++)
				{
					(*(U8 *)address++) = Disk_Buffer[i];
				}
				file_size += br;
			}   
			if(br != 512)
			{
				for(int i=0; i<br; i++)
				{
					(*(U8 *)address++) = Disk_Buffer[i];
				}
				file_size += br;
			}
			returnMessage=RESULT_OK;
		}
		f_close(&fsrc);
		
		if(loopOutFlag==1){
			loop=0;
		}
	}	
	//SDCARD 마운트해제
    f_mount(0, Disk_Kind, 0);
	
	return returnMessage;
}
