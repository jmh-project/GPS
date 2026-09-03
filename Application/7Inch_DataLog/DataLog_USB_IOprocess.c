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
volatile U32 USB_IO_PROCESS = 0;

extern FATFS fatfs;
extern FIL file;
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;
extern volatile FATFS sd_fatfs;
extern volatile FIL sd_file;
extern void USBH_Init(USB_OTG_CORE_HANDLE *pdev,
               USB_OTG_CORE_ID_TypeDef coreID,
               USBH_HOST *phost,               
               USBH_Class_cb_TypeDef *class_cb, 
               USBH_Usr_cb_TypeDef *usr_cb);

volatile U32		id_USBTimer;
volatile int		gWaitUsbCnt;
volatile int		gUSB_File_NameCnt;
#define     SD      1
#define     USB     0
extern U32 gMessageTextId;	
U32 gMessageTextId2;	
U32 gMessageTextId3;	
extern BYTE Disk_Buffer[MEMORY_BUF_SIZE]; 
char gFileName[64];
volatile int gUsbResetFlag = 0;
int gYear, gMonth;
int	 USB_IOPrepareEvent2(HANDLE handle, int command, int param, void *Data)
{
	//int		id;
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("IOPrepareEvent:: IO Precess not Found\n");
		return(0);
	}
	USB_IO_PROCESS = IOOpenTimer(handle, 10);
    
	//IOStartTimer(USB_IO_PROCESS);
	return(1);
}


int	 USB_IOTimerEvent2(HANDLE handle, int command, int param, void *Data)  
{
	stIOProcess 	*io_process = FindIOProcess(handle);
	if(io_process==NULL) 
	{
		Dprintf("IOTimerEvent:: IO Precess not Found\n");
		return(0);
	}
    //Uart_SendByte2('.');
	USBH_Process(&USB_OTG_Core, &USB_Host);
	return(1);
}

int USB_IOProcess(void)
{
    Dprintf("[ENABLE] USB_IOProcess\n");
	makeIOProcess("USB", USB_IOPrepareEvent2, USB_IOTimerEvent2);
	return(1);	
}
int	 SmartLogUSBMessageTimerEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	//char	tbuf[20];
	
	Graphics 	*Gr   = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	if(Gr==NULL) 													// 그래픽 할당을 못 받은 경우  
	{
		Dprintf("SmartLogUSBMessageTimerEvent:: Graphics Error\n");
		return(0);
	}	
	gv.sleepModeTimerCntSecPv=0;
	if(gUSB_MSC_Status == 1)
	{
		SetWidgetText(gMessageTextId,"[ 다운로드 ] 버튼을 누르면 다운로드를 시작합니다.");
		CloseTimer(id_USBTimer);
	}
	else
	{
		if(gWaitUsbCnt==0) SetWidgetText(gMessageTextId,"USB 접속 대기중 입니다.");
		//SetWidgetText(gMessageTextId,tbuf);
		if(gWaitUsbCnt >= 10) SetWidgetText(gMessageTextId,"HOME 버튼을 누르고 다시 시도해 주세요");
		gWaitUsbCnt++;
	}	
	return 0;
}
int SDtoLanTrandmit(void) {
	char		fbuf[30];
	//int			filenameCount=0;
	//int			dispCount=0;
	unsigned int	maxSize=0;
	//unsigned int	i=0;
	uint32_t			ScreenPage=SDREAD_RAM;
	
	FATFS fs[2];         	/* Work area (file system object) for logical drives */
    FIL fsrc;//, fdst;      	/* file objects */
    FRESULT res;         	/* FatFs function common result code */
    UINT br=0;//, bw=0;		/* File read/write count */

	f_mount( &fs[1], "1:", SD );
	while(1)
	{
		sprintf(fbuf,"1:WaterImage.bmp");
		res = f_open(&fsrc, fbuf, FA_OPEN_EXISTING | FA_READ);
		if(res!=FR_OK)
		{
			f_mount(0, "1:", 0); 
			return (0);
		}
		else
		{
			maxSize = 0;
			//Dprintf("WaterImage.bmp size = %d\n", fsrc.fsize);
			while(1)
			{
				res = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);     /* Read a chunk of src file */
				if (res || br == 0) //error or eof
				{
					Dprintf(" 2 res = %d br = %d\n",res, br);
					break; 
				}		
				for( int i=0; i<br; i++ )
				{
					*(unsigned char *) ScreenPage = Disk_Buffer[i];
					Dprintf("%c%c",Disk_Buffer[i]);
					ScreenPage++;
				}	
				if(br < 4096)
				{
					break;
				}
				maxSize++;
			}
			f_close(&fsrc);
			//f_close(&fdst);
			break;
		}
	}	
	return 0;
}
volatile  int			intervaltime=0;
int	 SmartLogUSBMessageKeyEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {	
	char fbuf[128];
	//int	filenameCount=0;
	unsigned int i=0;
	Windows 	*win = GetWindow(parent, handle, "SmartLogUSBMessageKeyEvent");
	Windows		*parent_win = FindContainerWindow(parent,  "SmartLogUSBMessageKeyEvent");
	
	FATFS fs[2];         	/* Work area (file system object) for logical drives */
    FIL fsrc, fdst;      	/* file objects */
    FRESULT res;         	/* FatFs function common result code */
    UINT br=0, bw=0;		/* File read/write count */

	if(parent_win == NULL) {
		Dprintf("SmartLogUSBMessageKeyEvent:: Windows Error\n");
	}
	if(win->ID == 3)
	{
		for(i=0;i<64;i++)
		{
			gFileName[i] = 0;
		}
		gMonth++;
		if(gMonth > 12)
		{
			gMonth = 1;
			gYear++;
			if(gYear > 99) gYear = 0;
		}
		sprintf(gFileName,"20%02d년 %02d월 데이터",gYear, gMonth);
		SetWidgetText(gMessageTextId2,gFileName);
        sprintf(gFileName,"%02d년 %02d월 %s",gYear, gMonth,"/ 월 단위로 다운로드진행");
        SetWidgetText(gMessageTextId3,gFileName);
	}
	else if(win->ID == 4)
	{
		for(i=0;i<60;i++)
		{
			gFileName[i] = 0;
		}
		gMonth--;
		if(gMonth == 0) 
		{
			gMonth = 12;
			gYear--;
			if(gYear < 0) gYear = 99;
		}
		sprintf(gFileName,"20%02d년 %02d월 데이터",gYear, gMonth);
		SetWidgetText(gMessageTextId2,gFileName);
        sprintf(gFileName,"%02d년 %02d월 %s",gYear, gMonth,"/ 월 단위로 다운로드진행");
        SetWidgetText(gMessageTextId3,gFileName);
	}
	else if(win->ID == 1)//YES BUTTON
	{
		IOStopTimer(USB_IO_PROCESS);
        
		if(gUSB_MSC_Status==1)
		{
			SetWidgetText(gMessageTextId,"다운로드 중 입니다.");			
            /***************************************************************************************************************			
                        
                        TimeSettingv.csv
						//Close	
						f_mount(0, "0:", 0); 
						f_mount(0, "1:", 0); 
	
            ***************************************************************************************************************/			
			f_mount( &fs[0], "0:", USB );
			f_mount( &fs[1], "1:", SD );
			Dprintf("USB 파일 다운로드 시작\n");
			
			#if 1 //이벤트데이터저장
			while(1)
			{
				//DumpTransmitFile2204.txt
				//strcpy(gFileName,"1:DumpTransmitFile2204.txt");
				sprintf(gFileName,"1:EventMessage%02d%02d.csv",gYear, gMonth);
				res = f_open(&fsrc, gFileName, FA_OPEN_EXISTING | FA_READ);//원본파일오픈
				if(res!=FR_OK)
				{
					//SetWidgetText(gMessageTextId,"이벤트 데이터 파일이 생성되지 않았습니다.");
					Dprintf("이벤트 데이터 파일이 생성되지 않았습니다.\n");
					break;
				}
				else
				{
                    sprintf(fbuf,"0:EVENT_20%02d%02d%02d%02d%02d%02d.csv",currentDate.year,  currentDate.month, currentDate.date, currentDate.hour,  currentDate.minute, currentDate.sec);
					//strcpy(fbuf,"0:DumpTransmitFile2204.txt");
					res = f_open(&fdst, fbuf, FA_OPEN_ALWAYS | FA_WRITE);		
//					while (f_gets((char *)Disk_Buffer, sizeof(Disk_Buffer), &fsrc)) {
//						res = f_write(&fdst, Disk_Buffer, sizeof(Disk_Buffer), &bw);               /* Write it to the dst file */
//					}
					while(1)
					{
						res = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);     /* Read a chunk of src file */
						if (res || br == 0) //error or eof
						{
							Dprintf("EVENT : res=%d br = %d\n", res, br);
							SetWidgetText(gMessageTextId,"이벤트 파일 읽기 에러 발생.");
							Delay(1000);
							break; 
						}				
						res = f_write(&fdst, Disk_Buffer, br, &bw);               /* Write it to the dst file */
						if (res || bw < br) //error or disk full
						{
							SetWidgetText(gMessageTextId,"이벤트 파일 쓰기 에러 발생.");
							Delay(1000);
							break; /* error or disk full */
						}
						if(bw < 4096)
						{
							break;
						} 
					}
					f_close(&fsrc);
					f_close(&fdst);
					break;
				}
			}
			#endif
			#if 1 //5분데이터저장
			while(1)
			{
				sprintf(gFileName,"1:AverageMinData%02d%02d.csv",gYear, gMonth);
				res = f_open(&fsrc, gFileName, FA_OPEN_EXISTING | FA_READ);
				if(res!=FR_OK)
				{
					//SetWidgetText(gMessageTextId,"5분 데이터 파일이 생성되지 않았습니다.");
					Dprintf("5분 데이터 파일이 생성되지 않았습니다.\n");
					break;
				}
				else 
				{
                    sprintf(fbuf,"0:AverageMinData_20%02d%02d%02d%02d%02d%02d.csv",currentDate.year,  currentDate.month, currentDate.date, currentDate.hour,  currentDate.minute, currentDate.sec);
                    res = f_open(&fdst, fbuf, FA_OPEN_EXISTING | FA_WRITE);                     
                    if(res==FR_NO_FILE)//파일이 존재하지 않는 경우 생성하여 저장한다.
                    {
                        res = f_open(&fdst, fbuf, FA_OPEN_ALWAYS | FA_WRITE);
                    }
					while(1)
					{
						res = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);     /* Read a chunk of src file */
						if (res || br == 0) //error or eof
						{
							SetWidgetText(gMessageTextId,"5분데이터 파일 읽기 에러 발생.");
							Dprintf("5분데이터 파일 읽기 에러 발생.\n");
							Delay(1000);
							break; 
						}				
						res = f_write(&fdst, Disk_Buffer, br, &bw);               /* Write it to the dst file */
						if (res || bw < br) //error or disk full
						{
							SetWidgetText(gMessageTextId,"5분데이터 파일 쓰기 에러 발생.");
							Dprintf("5분데이터 파일 쓰기 에러 발생.\n");
							Delay(1000);
							break; /* error or disk full */
						}
						if(bw < 4096)
						{
							break;
						} 
					}
					f_close(&fsrc);
					f_close(&fdst);
					break;
				}
			}
			#endif
			#if 1 // 60분 데이터 저장
			while(1)
			{
				sprintf(gFileName,"1:AverageHourData%02d%02d.csv",gYear, gMonth);
				res = f_open(&fsrc, gFileName, FA_OPEN_EXISTING | FA_READ);
				if(res!=FR_OK)
				{
					SetWidgetText(gMessageTextId,"60분 데이터 파일이 생성되지 않았습니다.");
					Dprintf("60분 데이터 파일이 생성되지 않았습니다.\n");
					break;
				}
				else
				{
                    sprintf(fbuf,"0:AverageHourData_20%02d%02d%02d%02d%02d%02d.csv",currentDate.year,  currentDate.month, currentDate.date, currentDate.hour,  currentDate.minute, currentDate.sec);
                    res = f_open(&fdst, fbuf, FA_OPEN_EXISTING | FA_WRITE);                     
                    if(res==FR_NO_FILE)//파일이 존재하지 않는 경우 생성하여 저장한다.
                    {
                        res = f_open(&fdst, fbuf, FA_OPEN_ALWAYS | FA_WRITE);
                    }
					while(1)
					{
						res = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);     /* Read a chunk of src file */
						if (res || br == 0) //error or eof
						{
							SetWidgetText(gMessageTextId,"60분데이터 파일 읽기 에러 발생.");
							Dprintf("60분데이터 파일 읽기 에러 발생.\n");
							Delay(1000);
							break; 
						}				
						res = f_write(&fdst, Disk_Buffer, br, &bw);               /* Write it to the dst file */
						if (res || bw < br) //error or disk full
						{
							SetWidgetText(gMessageTextId,"60분데이터 파일 쓰기 에러 발생.");
							Dprintf("60분데이터 파일 쓰기 에러 발생.\n");
							Delay(1000);
							break; /* error or disk full */
						}
						if(bw < 4096)
						{
							break;
						}
					}
					f_close(&fsrc);
					f_close(&fdst);
					break;
				}
			}
			#endif
			#if 1 // 덤프파일 데이터
			while(1)
			{
				sprintf(gFileName,"1:DataLogTransmitFile%02d%02d.txt",gYear, gMonth);
				res = f_open(&fsrc, gFileName, FA_OPEN_EXISTING | FA_READ);
				if(res!=FR_OK)
				{
					SetWidgetText(gMessageTextId,"DUMP 데이터 파일이 생성되지 않았습니다.");
					Dprintf("DUMP 데이터 파일이 생성되지 않았습니다 (%s).\n",gFileName);
					break;
				}
				else
				{
                    sprintf(fbuf,"0:DataLogTransmitFile_20%02d%02d%02d%02d%02d%02d.txt",currentDate.year,  currentDate.month, currentDate.date, currentDate.hour,  currentDate.minute, currentDate.sec);
                    res = f_open(&fdst, fbuf, FA_OPEN_EXISTING | FA_WRITE);                     
                    if(res==FR_NO_FILE)//파일이 존재하지 않는 경우 생성하여 저장한다.
                    {
                        res = f_open(&fdst, fbuf, FA_OPEN_ALWAYS | FA_WRITE);
                    }				
					while(1)
					{
						res = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);     /* Read a chunk of src file */
						if (res || br == 0) //error or eof
						{
							SetWidgetText(gMessageTextId,"DUMP 데이터 파일 읽기 에러 발생.");
							Dprintf("DUMP 데이터 파일 읽기 에러 발생.\n");
							Delay(1000);
							break; 
						}				
						res = f_write(&fdst, Disk_Buffer, br, &bw);               /* Write it to the dst file */
						if (res || bw < br) //error or disk full
						{
							SetWidgetText(gMessageTextId,"DUMP 데이터 파일 쓰기 에러 발생.");
							Dprintf("DUMP 데이터 파일 쓰기 에러 발생.\n");
							Delay(1000);
							break; /* error or disk full */
						}
						if(bw < 4096)
						{
							break;
						}
					}
					f_close(&fsrc);
					f_close(&fdst);
					break;
				}
			}
			#endif

			
            
			//최종적으로 여기서 파일 마운트를 해제한다.
            /* Unregister work area prior to discard it */
			//if(saveFlag==1)
			{
				SetWidgetText(gMessageTextId,"작업 완료.");
			}
//			else if(saveFlag==0)
//			{
//				SetWidgetText(gMessageTextId,"파일 이름을 다시 설정하세요.");
//			}
            f_mount(0, "0:", 0); 
            f_mount(0, "1:", 0); 
			return 0;
		}
		else
		{
            gv.usbSaveWaitFlag=0;
			#if defined(TYPE_APPROVAL_ENABLE)
			PasswordInit();
			#endif
            GPIO_ResetBits(GPIOB, GPIO_Pin_2);
			//Dprintf("SmartLogFlash.IntervalTimeSec = %d, %d\n",SmartLogFlash.IntervalTimeSec,intervaltime);
			//StopTimer(id_USBTimer);
			//IOStopTimer(USB_IO_PROCESS);
			CloseWindow(NULL, parent);//다운로드 완료후 자동으로 WINDOW 를 CLOSE 한다. 이때 경보음을 울려준다.
		}
	}
	if(win->ID == 2)//NO BUTTON
	{
        TimerQClear();//여기서 이것을 사용하면 부모윈도우 타이머까지 동작을 안한다.
        gv.usbSaveWaitFlag=0;
        //Dprintf("SmartLogFlash.IntervalTimeSec = %d\n",SmartLogFlash.IntervalTimeSec);
		//USBH_Process::	HOST_IDLE
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		//Dprintf("SmartLogUSBMessageKeyEvent :: IOOpenTimer = %d\n ", USB_IO_PROCESS);
		//StopTimer(id_USBTimer);
		//IOStopTimer(USB_IO_PROCESS);
		//Dprintf("2[STRUCT] ERROR :: gv.StartVerify = %d gv.EndVerify = %d\n",gv.StartVerify,gv.EndVerify);
		#if defined(TYPE_APPROVAL_ENABLE)
		PasswordInit();
		#endif
		CloseWindow(NULL, parent);//[ KEY ]동작에의한 WINDOWS CLOSE는 NULL 과 PARENT 인자를 가지고 CALL한다.
		//Dprintf("3[STRUCT] ERROR :: gv.StartVerify = %d gv.EndVerify = %d\n",gv.StartVerify,gv.EndVerify);
	}
	return(1); 
}
char        gMessageUSB[30];	
extern volatile int gUSB_Select;
void USB_Host_Start(void);
int	 SmartLogUSBMessageCreateEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
	Windows 	*win = GetWindow(parent, handle,"KeyPadCreateEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA); 	//그래픽영역을 얻는다.
	
	if(win==NULL)
	{
		Dprintf("SmartLogKeyPadCreateEvent:: Windows Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("SmartLogKeyPadCreateEvent::GetGraphics Error\n");
		return(0);
	}	
	PaintDrawControl(OFF);
    unsigned int labelBackColor = flash.systemColor;
    unsigned int labelFontColor = flash.systemFontColor;
	if(flash.systemColorNo==6) {
		Gr->SetColor(RGB565CONVERT(200,  200,  200));
	}
	else {
		Gr->SetColor(clBlack);
	}
	Gr->FillRect(0, 0, win->width, win->height);

	//gv.touchManagerFlag=0;
	gWaitUsbCnt=0;
	for(int i=0; i<30; i++) {
		gFileName[i] = 0;
	}
	//TitleBar(handle, 0, 0, LCD_X, 50, 1, win->Text, clYellow, 1, RIGHT_ALIGN, 2, NULL);	
	
	//BIcon(handle, 20, 50, 48, 48 , "48G_Warning_Icon1.bmp", RGB565CONVERT(230,  230,  230));
	gMessageTextId = SLabel(handle, 0, 50, 500-10, 48, gMessageUSB, labelFontColor, labelBackColor,  0);
    
    
	gYear = currentDate.year;
	gMonth = currentDate.month;
	sprintf(gFileName,"20%02d년 %02d월 데이터",gYear, gMonth);
	gMessageTextId2 = SLabel(handle, 0, 50+48, 500-10, 60, gFileName, labelFontColor, labelBackColor,  0);
    
	//sprintf(gFileName,"%02d년 %02d월 %s",gYear, gMonth,"/ 월 단위로 다운로드진행");
    gMessageTextId3 = SLabel(handle, 0, 50+48+48, 500-10, 60, "월 단위로 다운로드가 가능합니다.", labelFontColor, labelBackColor,  0);
	
	SButton(handle, 20, 180+40, 200, 60, "48G_Up_Icon1.bmp", "월변경",  PUSH_BUTTON, 3, SmartLogUSBMessageKeyEvent);
	SButton(handle, 500 - 20 - 200, 180+40, 200, 60, "48G_Down_Icon1.bmp", "월변경",  PUSH_BUTTON, 4, SmartLogUSBMessageKeyEvent);

	SButton(handle, 20, 180+10+100, 200, 60, "48G_USB_Icon1.bmp", "다운로드",  PUSH_BUTTON, 1, SmartLogUSBMessageKeyEvent);

	SButton(handle, 500-20-200, 180+10+100, 200, 60, "48G_Home_Icon2.bmp", "HOME",  PUSH_BUTTON, 2, SmartLogUSBMessageKeyEvent);
	PaintDrawControl(ON);

   //USB 초기화 
	gUSB_MSC_Status=0;
	gUSB_File_NameCnt=0;
	gWaitUsbCnt=0;
    
    if(gUsbResetFlag==1)
    {
        USB_Host_Start();
        gUsbResetFlag = 0;
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_2);
        Delay(1000);
        GPIO_SetBits(GPIOB, GPIO_Pin_2);
        Delay(1000);
    }
    
    IOStartTimer(USB_IO_PROCESS);	//USB 탐색 
    
	id_USBTimer=OpenTimer(parent, handle, 1000);		// 1초간격으로 메시지를 출력한다.
	StartTimer(id_USBTimer);
	PaintDrawControl(ON);
	gv.usbSaveWaitFlag=1;
	return(1);
}

int	 SmartLogUSBMessagePaintEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data)
{
	Windows 	*win = GetWindow(parent, handle, "SmartLogUSBMessagePaintEvent");
	Graphics 	*Gr = GetGraphics(parent, handle, CLIENT_AREA);

	if(win==NULL) 
	{
		Dprintf("SmartLogUSBMessagePaintEvent:: Error\n");
		return(0);
	}
	if(Gr==NULL)
	{
		Dprintf("SmartLogUSBMessagePaintEvent::GetGraphics Error\n");
		return(0);
	}	
	
	Gr->FillRect(0, 0, 0, 0);
	Gr->Draw(parent, handle, command, param, Data);	
	return(1);
}

int	 SmartLogUSBMessage_CloseEvent(HANDLE parent, HANDLE handle, WCOMMAND command, PARAM param, void *Data) {
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
		Dprintf("DataLogSetup_CloseEvent::Invalid Handle\n");
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

void SmartLogUSBMessage(HANDLE parent, HANDLE handle, U32 id, int sx, int sy, int font_no, char *value, char *title) {
	if(GetWindowName("USB")) 
	{
		Dprintf("USB:: Windows already Create\n");
	} else {
	WidgetStruct wid_struct;	
	wid_struct.FontNo = font_no;
	//wid_struct.CallWidget_ID = id;
    strcpy(gMessageUSB, value);
	makeWindow(
				LCD,
				"USB",	// Windows Name
				DESKTOP_WINDOW,			// Parent Window Handle
				CONTAINER_WINDOW,		// Windows Kind
				XP_STYLE,				// Windows Style
				sx, sy, 				// Start Position
				500, 420,				// Size
				SHOW,					// Create after Show
				SmartLogUSBMessageCreateEvent,		// CreateEvent
				SmartLogUSBMessagePaintEvent,		// PaintEvent
				NULL,					// MouseOnEvent
				NULL,					// MouseOffEvent
				NULL,					// MouseMoveEvent
				NULL,					// GestureEvent
				NULL,					// ClickEvent
				SmartLogUSBMessageTimerEvent,					// TimerEvent
				NULL,  					// ControlEvent
				NULL,  					// ChangeEvent
				NULL,
				NULL,
				NULL,
				SmartLogUSBMessage_CloseEvent,
				GRAY7,					// PARAMETER
				value,					// Normal Touch Screen
				0,						// Touch Screen
				title,					// Windows Text
				MODALLESS_WINDOW,
				&wid_struct
				);
			}
}
int USB_Control(HANDLE parent, HANDLE handle)
{
 	SmartLogUSBMessage(parent, handle,  NULL, (LCD_X-500)/2, (LCD_Y-420)/2, FONT16, "", "SDCard to USB");		
	return 0;
}
