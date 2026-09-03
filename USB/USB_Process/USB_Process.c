/******************************************************************************
							Include File
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
#include "TEECO_System.h"
//#include "stm324xg_eval_audio_codec.h"

//Host 사용시 사용
#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"

//Device 사용시사용
//#include "usbd_msc_core.h"
//#include "usbd_usr.h"
//#include "usbd_desc.h"
//#include "usb_conf.h"

//HID 사용시사용
//#include "usbh_hid_core.h"
/******************************************************************************
							Constant & Macros
******************************************************************************/
extern  USBH_Usr_cb_TypeDef USR_Callbacks;

extern __IO U32 gSoundExtAddr;
extern __IO U32 gSoundExtSize;
extern __IO uint32_t nowPlay;
/******************************************************************************
							Private & Local Variables
******************************************************************************/
__ALIGN_BEGIN USB_OTG_CORE_HANDLE      USB_OTG_Core __ALIGN_END;
__ALIGN_BEGIN USBH_HOST                USB_Host __ALIGN_END;

//__ALIGN_BEGIN   USB_OTG_CORE_HANDLE     USB_OTG_Core_dev __ALIGN_END ;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE     USB_OTG_dev __ALIGN_END ;


#if defined MEDIA_USB_KEY
USB_OTG_CORE_HANDLE          USB_OTG_Core;
USBH_HOST                    	USB_Host;
#endif

volatile int gUSB_IO_Timer_ID=0;

extern  FATFS usb_fatfs;
extern  FIL usb_file;

extern  FATFS sd_fatfs;
extern  FIL sd_file;

extern  FATFS fatfs;
extern  FIL file;

extern BYTE Disk_Buffer[MEMORY_BUF_SIZE];   /* File copy buffer */
/******************************************************************************
							Function Prototype
******************************************************************************/

/******************************************************************************
							Main Program Entry
******************************************************************************/
extern  int gUSB_Select;

void USB_Lib_Load(void)
{
    gUSB_Select = HOST;    
    USBH_Init(&USB_OTG_Core, 
				USB_OTG_FS_CORE_ID,
				&USB_Host,
				&USBH_MSC_cb, 
				&USR_cb);    
}


//!< 이함수는 부트로더에서도 사용하기때문에 컴파일옵션에서 중립을 취한다.
int USBTest(void)
{
	static int err_cnt=0;
	
    Dprintf(">USB 마운트를 시작합니다.\n");	
	if(err_cnt > 2)
	{
		Dprintf(">USB Error Gen and Power OFF->On Retry\n");
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);  //USB Power Off	
		Delay(500);
		GPIO_SetBits(GPIOB, GPIO_Pin_2); //USB Power ON 	  
		err_cnt=0;
	}
	g_Timer_msec = 0;
	int Done=1;
	gUSB_MSC_Status=0;
	while(Done)
	{
		USBH_Process(&USB_OTG_Core, &USB_Host);
		if(gUSB_MSC_Status==1)
		{
			//Dprintf("USB Standby\n");
			return(1);
		} 	
		Delay(10);
		if(Dstatus())
		{
			if(Dgetch() == 0x1b) return(2);
		}
		if(g_Timer_msec > 5000)
		{
			Dprintf(">USB 를 찾을수 없습니다.(타임아웃에러)\n");
			err_cnt++;
			return(0);
		}		
	}
	return(1);
}


int	 USB_IOTimerEvent(HANDLE handle, int command, int param, void *Data)
{
//	static int loop=0;
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("USB_IOTimerEvent:: IO Precess not Found\n");
		return(0);
	}
	USBH_Process(&USB_OTG_Core, &USB_Host);
	
	/*
	// 타이머동작확인시 사용한다.
	loop++;
	if(loop > 50)
	{
		Dprintf("USB_IOTimerEvent::occur\n");
		loop=0;
	}
	*/
	if(gUSB_MSC_Status==1)
	{
		//Dprintf("USB Standby\n");
		//USB_FileWrite();		
	}  	
	return(1);
}


int	 USB_IOPrepareEvent(HANDLE handle, int command, int param, void *Data)
{
	//int		id;
	stIOProcess 	*io_process = FindIOProcess(handle);

	if(io_process==NULL) 
	{
		Dprintf("USB_IOPrepareEvent:: IO Precess not Found\n");
		return(0);
	}
	gUSB_IO_Timer_ID = IOOpenTimer(handle, 10);
	//IOStartTimer(id);
	return(1);
}
extern void USB_Select_Power_PortInit(void);
void USB_Host_Start(void)
{
	Dprintf("HOST Process Start\n");
	USB_Select_Power_PortInit();
    gUSB_Select = HOST;    
    USBH_Init(&USB_OTG_Core, 
				USB_OTG_FS_CORE_ID,
				&USB_Host,
				&USBH_MSC_cb, 
				&USR_cb);    
    Dprintf("HOST Process End\n");
	
	makeIOProcess("USBHost", USB_IOPrepareEvent, USB_IOTimerEvent);
}

int SDtoUSB(char *src, char dest)
{
    FRESULT fr;          /* FatFs function common result code */
    UINT br, bw;         /* File read/write count */


    /* Register work area for each logical drive */
    if ( f_mount(&sd_fatfs, "1:", 1) != FR_OK )   
    //if ( f_mount(SD, &sd_fatfs ) != FR_OK ) 
    {
        /* efs initialisation fails*/
        Dprintf("SDtoUSB::Cannot initialize File System.\n");
        return(-1);
    }
    if ( f_mount(&usb_fatfs, "0:", 1) != FR_OK )   
	//if ( f_mount(USB, &usb_fatfs ) != FR_OK ) 
    {
        /* efs initialisation fails*/
        Dprintf("SDtoUSB::Cannot initialize File System.\n");
        return(-1);
    }

    /* Open source file on the drive 1 */
    fr = f_open(&sd_file, "0:file.bin", FA_OPEN_EXISTING | FA_READ);
    if (fr) return (int)fr;

    /* Create destination file on the drive 0 */
    fr = f_open(&usb_file, "1:file.bin", FA_CREATE_ALWAYS | FA_WRITE);
    if (fr) return (int)fr;

    /* Copy source to destination */
    for (;;) {
        fr = f_read(&sd_file, Disk_Buffer, sizeof Disk_Buffer, &br);  /* Read a chunk of source file */
        if (fr || br == 0) break; /* error or eof */
        fr = f_write(&usb_file, Disk_Buffer, br, &bw);            /* Write it to the destination file */
        if (fr || bw < br) break; /* error or disk full */
    }

    /* Close open files */
    f_close(&sd_file);
    f_close(&usb_file);

    /* Unregister work area prior to discard it */
	//f_mount(0, NULL); 	
	f_mount(0, "0:", 0);        	
	//f_mount(1, NULL);	
	f_mount(0, "1:", 0);        	
	return(1);
}
#ifdef __cplusplus
}
#endif

