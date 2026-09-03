/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    19-March-2012
  * @brief   This file includes the usb host library user callbacks
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "usbh_usr.h"
#include "dos.h"
#include "ff.h"       /* FATFS */
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "DataLog.h"     
#include "Debug_Uart.h"

extern volatile int    gUSB_MSC_Status;
extern char CurrenrDir[64];
extern char CurrenrDisk[16];
extern char Disk_Kind[16];

/** @addtogroup USBH_USER
* @{
*/

/** @addtogroup USBH_MSC_DEMO_USER_CALLBACKS
* @{
*/

/** @defgroup USBH_USR 
* @brief    This file includes the usb host stack user callbacks
* @{
*/ 

/** @defgroup USBH_USR_Private_TypesDefinitions
* @{
*/ 
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Defines
* @{
*/ 
#define IMAGE_BUFFER_SIZE    512
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Macros
* @{
*/ 
extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Variables
* @{
*/ 
uint8_t USBH_USR_ApplicationState = USH_USR_FS_INIT;
uint8_t filenameString[15]  = {0};


uint8_t Image_Buf[IMAGE_BUFFER_SIZE];
uint8_t line_idx = 0;   

/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */

USBH_Usr_cb_TypeDef USR_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
    
};

/**
* @}
*/

/** @defgroup USBH_USR_Private_Constants
* @{
*/ 
/*--------------- LCD Messages ---------------*/
const uint8_t MSG_HOST_INIT[]        = "> Host Library Initialized\n";
const uint8_t MSG_DEV_ATTACHED[]     = "> Device Attached \n";
const uint8_t MSG_DEV_DISCONNECTED[] = "> Device Disconnected\n";
const uint8_t MSG_DEV_ENUMERATED[]   = "> Enumeration completed \n";
const uint8_t MSG_DEV_HIGHSPEED[]    = "> High speed device detected\n";
const uint8_t MSG_DEV_FULLSPEED[]    = "> Full speed device detected\n";
const uint8_t MSG_DEV_LOWSPEED[]     = "> Low speed device detected\n";
const uint8_t MSG_DEV_ERROR[]        = "> Device fault \n";

const uint8_t MSG_MSC_CLASS[]        = "> Mass storage device connected\n";
const uint8_t MSG_HID_CLASS[]        = "> HID device connected\n";
const uint8_t MSG_DISK_SIZE[]        = "> Size of the disk in MBytes: \n";
const uint8_t MSG_LUN[]              = "> LUN Available in the device:\n";
const uint8_t MSG_ROOT_CONT[]        = "> Exploring disk flash ...\n";
const uint8_t MSG_WR_PROTECT[]       = "> The disk is write protected\n";
const uint8_t MSG_UNREC_ERROR[]      = "> UNRECOVERED ERROR STATE\n";

/**
* @}
*/


/** @defgroup USBH_USR_Private_FunctionPrototypes
* @{
*/
//static uint8_t Explore_Disk (char* path , uint8_t recu_level);
//static uint8_t Image_Browser (char* path);
//static void     Show_Image(void);
//static void     Toggle_Leds(void);
/**
* @}
*/ 


/** @defgroup USBH_USR_Private_Functions
* @{
*/ 

#define		LCD_LOG_SetHeader	Dprintf


/**
* @brief  USBH_USR_Init 
*         Displays the message on LCD for host lib initialization
* @param  None
* @retval None
*/
void USBH_USR_Init(void)
{
	static uint8_t startup = 0;  
  
	if(startup == 0 )
	{
		startup = 1;
    
    
		//Dprintf("LCD Init\n");
		Dprintf("\n> USB Host library started.\n"); 
		Dprintf ("     STATUS::USB Host Library v2.1.0\n" );
	}
}

/**
* @brief  USBH_USR_DeviceAttached 
*         Displays the message on LCD on device attached
* @param  None
* @retval None
*/
void USBH_USR_DeviceAttached(void)
{
    //LCD_UsrLog((void *)MSG_DEV_ATTACHED); // "> Device Attached \n";
}


/**
* @brief  USBH_USR_UnrecoveredError
* @param  None
* @retval None
*/
void USBH_USR_UnrecoveredError (void)
{
  
    /* Set default screen color*/ 
    //LCD_ErrLog((void *)MSG_UNREC_ERROR); 
}


/**
* @brief  USBH_DisconnectEvent
*         Device disconnect event
* @param  None
* @retval Staus
*/
void USBH_USR_DeviceDisconnected (void)
{
  
  //LCD_LOG_ClearTextZone();
  
  //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                      ");
  //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "                                      ");  
  
  /* Set default screen color*/
  //LCD_ErrLog((void *)MSG_DEV_DISCONNECTED);
}
/**
* @brief  USBH_USR_ResetUSBDevice 
* @param  None
* @retval None
*/
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
}


/**
* @brief  USBH_USR_DeviceSpeedDetected 
*         Displays the message on LCD for device speed
* @param  Device speed
* @retval None
*/
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
    //LCD_UsrLog((void *)MSG_DEV_HIGHSPEED);
  }  
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
    //LCD_UsrLog((void *)MSG_DEV_FULLSPEED);
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
    //LCD_UsrLog((void *)MSG_DEV_LOWSPEED);
  }
  else
  {
    //LCD_UsrLog((void *)MSG_DEV_ERROR);
  }
}

/**
* @brief  USBH_USR_Device_DescAvailable 
*         Displays the message on LCD for device descriptor
* @param  device descriptor
* @retval None
*/
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;  
  
  Dprintf("VID : %04Xh\n" , (uint32_t)(*hs).idVendor); 
  Dprintf("PID : %04Xh\n" , (uint32_t)(*hs).idProduct); 
}

/**
* @brief  USBH_USR_DeviceAddressAssigned 
*         USB device is successfully assigned the Address 
* @param  None
* @retval None
*/
void USBH_USR_DeviceAddressAssigned(void)
{
  
}


/**
* @brief  USBH_USR_Conf_Desc 
*         Displays the message on LCD for configuration descriptor
* @param  Configuration descriptor
* @retval None
*/
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
  USBH_InterfaceDesc_TypeDef *id;
  
  id = itfDesc;  
  
  if((*id).bInterfaceClass  == 0x08)
  {
    //LCD_UsrLog((void *)MSG_MSC_CLASS);
  }
  else if((*id).bInterfaceClass  == 0x03)
  {
    //LCD_UsrLog((void *)MSG_HID_CLASS);
  }    
}

/**
* @brief  USBH_USR_Manufacturer_String 
*         Displays the message on LCD for Manufacturer String 
* @param  Manufacturer String 
* @retval None
*/
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
  //LCD_UsrLog("Manufacturer : %s\n", (char *)ManufacturerString);
}

/**
* @brief  USBH_USR_Product_String 
*         Displays the message on LCD for Product String
* @param  Product String
* @retval None
*/
void USBH_USR_Product_String(void *ProductString)
{
  //LCD_UsrLog("Product : %s\n", (char *)ProductString);  
}

/**
* @brief  USBH_USR_SerialNum_String 
*         Displays the message on LCD for SerialNum_String 
* @param  SerialNum_String 
* @retval None
*/
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  //LCD_UsrLog( "Serial Number : %s\n", (char *)SerialNumString);    
} 



/**
* @brief  EnumerationDone 
*         User response request is displayed to ask application jump to class
* @param  None
* @retval None
*/
void USBH_USR_EnumerationDone(void)
{
  
  /* Enumeration complete */
  //LCD_UsrLog((void *)MSG_DEV_ENUMERATED);
  
  //LCD_SetTextColor(Green);
  //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "To see the root content of the disk : " );
  //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key...                       ");
  //LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
  //Dprintf("To see the root content of the disk : \n" );
  Dprintf("Press Key...\n");  
} 


/**
* @brief  USBH_USR_DeviceNotSupported
*         Device is not supported
* @param  None
* @retval None
*/
void USBH_USR_DeviceNotSupported(void)
{
  //LCD_ErrLog ("> Device not supported."); 
}  


/**
* @brief  USBH_USR_UserInput
*         User Action for application state entry
* @param  None
* @retval USBH_USR_Status : User response for key button
*/
USBH_USR_Status USBH_USR_UserInput(void)
{
	USBH_USR_Status usbh_usr_status;
  
	usbh_usr_status = USBH_USR_NO_RESP;  
  
	//Dprintf("User Input Wait...\n");
	//Dgetch();
  /*Key B3 is in polling mode to detect user action */
  
	//if(STM_EVAL_PBGetState(Button_KEY) == RESET) 
	    
	usbh_usr_status = USBH_USR_RESP_OK;    
	 
	return usbh_usr_status;
}  

/**
* @brief  USBH_USR_OverCurrentDetected
*         Over Current Detected on VBUS
* @param  None
* @retval Staus
*/
void USBH_USR_OverCurrentDetected (void)
{
  //LCD_ErrLog ("Overcurrent detected.");
}

//사용자는 이함수의 예제를 보고 파일읽고 쓰기를 작성한다.
#if 0
int USB_FileWrite(void)
{
    //WCHAR   filename[64];
    char   filename[64];
    FRESULT res;
    //uint8_t writeTextBuff[] = "STM32 Connectivity line Host Demo application using FAT_FS   ";
    uint8_t writeTextappendBuff[] = "\nFile Append test123   ";
    uint16_t bytesWritten, bytesToWrite;
    
    Dprintf("USB_FileWrite Entry---1\n");
    //if ( f_mount( USB, &usb_fatfs ) != FR_OK )  //f_mount(&usb_fatfs, Disk_Kind, 0 )
    if ( f_mount(&usb_fatfs, Disk_Kind, 0 ) != FR_OK )  //
    {
        /* efs initialisation fails*/
        Dprintf("USB_FileWrite::Cannot initialize File System.\n");
        return(-1);
    }
    Dprintf("USB_FileWrite Entry---2\n");
    
    //새로운 파일을 생성할때 사용한다.
    filename[0] = '0';
    filename[1] = ':';
    filename[2] = 'T';
    filename[3] = 'E';
    filename[4] = 'S';
    filename[5] = 'T';
    filename[6] = '0';
    filename[7] = '3';
    filename[8] = 0;
    Dprintf("USB_FileWrite Entry---3\n");
    
    //usb_fatfs.drv = 0;
    //usb_file.fs->drv=0;
    Dprintf("USB_FileWrite Entry---4\n");
    
    //Dprintf("USB_FileWrite::drive=%d drive=%d\n", usb_fatfs.drv, usb_file.fs->drv);
    
    if(f_open(&usb_file, filename, FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
    //기존에 존재하는 파일을 열고 추가할때 사용한다.
    //if(f_open(&file, (XCHAR *)"0:TestProgram.dat",FA_OPEN_EXISTING | FA_WRITE) == FR_OK)
    { 
        /* Write buffer to file */
        bytesToWrite = sizeof(writeTextappendBuff); 

        Dprintf("File Size=%d\n", usb_file.fsize);
        f_lseek(&usb_file, usb_file.fsize); 

        res= f_write (&usb_file, writeTextappendBuff, bytesToWrite, (void *)&bytesWritten);   
        if((bytesWritten == 0) || (res != FR_OK)) /*EOF or Error*/
        {
            //LCD_ErrLog("> TestProgram CANNOT be writen.\n");
        }
        else
        {
            //LCD_UsrLog("> 'TestProgram' file created\n");
        }
  
        f_close(&usb_file);
        //f_mount(USB, NULL); 
		f_mount(0, Disk_Kind, 0);   		
    }    
    else
    {
        Dprintf (">File not Create\n");
        return(0);
    }
    return(0);    
}
#endif
/********************************************************************************
*	3-1
*	CSV 파일 저장
*	.데이터를 이어서 저장한다.
*/
int SD_StructFileWrite(char *filename, int size) {
	FRESULT         fr;
    FIL  			fdst;	/* File objects */	
	UINT 		    bw;   /* File write count */
	uint32_t		len =  size;
	uint32_t		cnt=0;
	int 			ret=1;
	int				errorFlag = 0, errorCount = 0;
    uint32_t		fSize=0;
	while(1)
	{
        if ( f_mount( &sd_fatfs, "1:", 1 ) != FR_OK )//1 은 SD 
        {
            Dprintf("[SD] mount error %s\n",filename);
            errorFlag = 1;
        }
        else
        {
            fr = f_open(&fdst, filename, FA_OPEN_ALWAYS | FA_WRITE  | FA_READ);
			fSize = f_size(&fdst);
			Dprintf("NAME %s OPEN.SIZE = %d  WRITE.SIZE = %d NEXT OPEN SIZE = %d\n",filename,fSize, len, fSize+len);
            if(fr != FR_OK)
            {
                Dprintf("OPEN ERROR\n");
                errorFlag=1;
            }
            else
            {                
                //ret=f_lseek(&fdst, fdst.fsize);   
				ret = f_lseek(&fdst, f_size(&fdst));	
                if( ret != 0) Dprintf("[Error]f_lseek = %d\n",ret);                
                for(int i=0; i<len; i++)
                {
                    Disk_Buffer[cnt++] = gv.dataLogBuf[i];
                    
                    if(cnt==_MAX_SS)
                    {
                        fr = f_write(&fdst, Disk_Buffer, cnt, &bw);            //Write it to the destination file
                        if (fr || bw < cnt) 
                        { 
							gv.usbReadErrorFlag=1;
                            Dprintf("File Write Error1\n");
                            break; 
                        }//error or disk full
                        cnt=0;
                    }
                }
                if(cnt != _MAX_SS)
                {
                    fr = f_write(&fdst, Disk_Buffer, cnt, &bw);            /* Write it to the destination file */
                    if (fr || bw < cnt)  
                    {
                        Dprintf("File Write Error2\n");
						gv.usbReadErrorFlag=1;
                    }
                }    
                f_close(&fdst); /* Close open files */
                /* Unregister work area prior to discard it */
                f_mount(0, "1:", 0); 	                
                return 0;
            }
        }
		if(errorFlag == 1) 
		{
			errorFlag = 0;
			errorCount++;
			Dprintf("SD ErrorCount = %d\n",errorCount);
		}
		else
			return (0); //정상동작인 경우 여기서 정지한다.  
		
		if(errorCount > 10) 
		{
			Dprintf("SD WRITE ERROR\n");
			gv.usbReadErrorFlag=1;
			return 0;
		}
	}
}
/********************************************************************************
*	3-2
*	프로그램 업데이트 파일 쓰기
*	데이터를 이어서 저장한다.
*/
int SD_ProgramUpdateFileWrite(char *filename, int size) {
	FRESULT   fr;
    FIL  			fdst;	/* File objects */	
	UINT 		bw;   /* File write count */
	uint32_t		len =  size;
	uint32_t		cnt=0;
	int 			ret=1;
	int				errorFlag = 0, errorCount = 0;
    gv.UpdateAddress = SDREAD_RAM;//업데이트 파일이 저장되어있는 시작 주소
	while(1)
	{
        if ( f_mount( &sd_fatfs, "1:", 1 ) != FR_OK )//1 은 SD 
        {
            Dprintf("[SD] mount error %s\n",filename);
            errorFlag = 1;
        }
        else
        {
            fr = f_open(&fdst, filename, FA_OPEN_ALWAYS | FA_WRITE  | FA_READ);
            if(fr != FR_OK)
            {
                Dprintf("OPEN ERROR\n");
                errorFlag=1;
            }
            else
            {                
                //ret=f_lseek(&fdst, fdst.fsize);                                                                                
				ret = f_lseek(&fdst, f_size(&fdst));	
                if( ret != 0) Dprintf("[Error]f_lseek = %d\n",ret);                
                for(int i=0; i<len; i++)
                {
                    Disk_Buffer[cnt++] = *(unsigned char *) gv.UpdateAddress++;
                    
                    if(cnt==_MAX_SS)
                    {
                        fr = f_write(&fdst, Disk_Buffer, cnt, &bw);            //Write it to the destination file
                        if (fr || bw < cnt) 
                        { 
							gv.usbReadErrorFlag=1;
                            Dprintf("File Write Error1\n");
                            break; 
                        }//error or disk full
                        cnt=0;
                    }
                }
                if(cnt != _MAX_SS)
                {
                    fr = f_write(&fdst, Disk_Buffer, cnt, &bw);            /* Write it to the destination file */
                    if (fr || bw < cnt)  
                    {
                        Dprintf("File Write Error2\n");
						gv.usbReadErrorFlag=1;
                    }
                }    
                f_close(&fdst); /* Close open files */
                /* Unregister work area prior to discard it */
                f_mount(0, "1:", 0); 	                
                return 0;
            }
        }
		if(errorFlag == 1) 
		{
			errorFlag = 0;
			
			errorCount++;
			Dprintf("SD ErrorCount = %d\n",errorCount);
		}
		else
			return (0); //정상동작인 경우 여기서 정지한다.  
		
		if(errorCount > 10) //10동작후 에러처리한다.
		{
			Dprintf("SD WRITE ERROR\n");
			gv.usbReadErrorFlag=1;
			return 0;
		}
	}
}
/********************************************************************************
*	3-3
*	DUMP 데이터 저장, 에러 플래그 동작, 4096바이트 이상 저장하지 않는다.
*	데이터를 이어서 저장한다.
*/
int FileStatusCheck(char *filename) {
	FRESULT	fr;
	FILINFO fno;
	int errorFlag = 0, errorCount = 0;
    
	while(1)
	{
        if ( f_mount( &sd_fatfs, "1:", 1 ) != FR_OK )//1 은 SD 
        {
            Dprintf("[SD] mount error %s\n",filename);
            errorFlag = 1;
        }
        else
        {
			fr = f_stat(filename, &fno);
			f_mount(0, "1:", 0); 	
			if(fr == FR_OK) {
				//정상파일
				Dprintf("정상파일\n");
				return FR_OK;
			} else if(fr == FR_NO_FILE) {
				//파일이 없는 경우
				Dprintf("파일이 없는 경우\n");
				return FR_NO_FILE;
			} else {				
				//기타 오류 처리
				Dprintf("기타 오류 처리\n");
				return FR_DISK_ERR;
			}
			//return fr;
        }
		if(errorFlag == 1) 
		{
			errorFlag = 0;
			errorCount++;
			Dprintf("SD ErrorCount = %d\n",errorCount);
		}
		if(errorCount > 10) 
		{
			Dprintf("SD WRITE ERROR\n");
			gv.usbReadErrorFlag=1;
			return 0;
		}
	}
	//return 0;
}
int StringToSDCardWrite(char *filename, int size) {
	FRESULT	fr;
    FIL  			fdst;	/* File objects */		
	UINT 			bw;   /* File write count */
	uint32_t		len =  size;
	uint32_t		cnt=0;
	int 			ret=1;
	int				errorFlag = 0, errorCount = 0;
    uint32_t		fSize=0;
	
	while(1)
	{
        if ( f_mount( &sd_fatfs, "1:", 1 ) != FR_OK )//1 은 SD 
        {
            Dprintf("[SD] mount error %s\n",filename);
            errorFlag = 1;
        }
        else
        {
            fr = f_open(&fdst, filename, FA_OPEN_ALWAYS | FA_WRITE  | FA_READ);
			fSize = f_size(&fdst);
			Dprintf("NAME %s OPEN.SIZE = %d  WRITE.SIZE = %d NEXT OPEN SIZE = %d\n",filename,fSize, len, fSize+len);
            if(fr != FR_OK)
            {
                Dprintf("OPEN ERROR\n");
                errorFlag=1;
            }
            else
            {                
				ret = f_lseek(&fdst, f_size(&fdst));	
                if( ret != 0) Dprintf("[Error]f_lseek = %d\n",ret);                
                for(int i=0; i<len; i++)
                {
                    Disk_Buffer[cnt++] = gv.dataLogBuf[i];
                    
                    if(cnt==_MAX_SS)//_MAX_SS = 512 
                    {
                        fr = f_write(&fdst, Disk_Buffer, cnt, &bw);            //Write it to the destination file
                        if (fr || bw < cnt) 
                        { 
                            Dprintf("File Write Error1\n");
							gv.usbReadErrorFlag=1;
                            break; 
                        }//error or disk full
                        cnt=0;
                    }
                }
                if(cnt != _MAX_SS)
                {
                    fr = f_write(&fdst, Disk_Buffer, cnt, &bw);            /* Write it to the destination file */
                    if (fr || bw < cnt)  
                    {
                        Dprintf("File Write Error2\n");
						gv.usbReadErrorFlag=1;
                    }
                }    
                f_close(&fdst); /* Close open files */
                /* Unregister work area prior to discard it */
                f_mount(0, "1:", 0); 	
                return 0;
            }
        }
		if(errorFlag == 1) 
		{
			errorFlag = 0;
			errorCount++;
			Dprintf("SD ErrorCount = %d\n",errorCount);
		}
		else
			return (0); //정상동작인 경우 여기서 정지한다.  
		
		if(errorCount > 10) 
		{
			Dprintf("SD WRITE ERROR\n");
			gv.usbReadErrorFlag=1;
			return 0;
		}
	}
}
extern volatile int gUSB_MSC_StatusDump;
int USBH_USR_MSC_Application(void)
{
    //FRESULT res;
    //uint8_t writeTextBuff[] = "STM32 Connectivity line Host Demo application using FAT_FS   ";
    //uint16_t bytesWritten, bytesToWrite;
  
    switch(USBH_USR_ApplicationState)
    {
        case USH_USR_FS_INIT: 
    
        /* Initialises the File System*/
        //if ( f_mount(USB, &usb_fatfs ) != FR_OK )
        if ( f_mount(&usb_fatfs, Disk_Kind, 0 ) != FR_OK )  
        {
            /* efs initialisation fails*/
            Dprintf("USBH_USR_MSC_Application::Cannot initialize File System.\n");
            return(-1);
        }
        Dprintf(">USBH_USR_MSC_Application:: File System initialized.\n");
        Dprintf(">USBH_USR_MSC_Application:: Disk capacity : %ld Bytes\n", USBH_MSC_Param.MSCapacity * USBH_MSC_Param.MSPageLength); 
    
        if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
        {
            //LCD_ErrLog((void *)MSG_WR_PROTECT);
            Dprintf("USBH_USR_MSC_Application::Write Protect\n");
        }    
        USBH_USR_ApplicationState = USH_USR_FS_WRITEFILE;
    break;
    
    case USH_USR_FS_WRITEFILE:
        USB_OTG_BSP_mDelay(100);
    
        if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
        {      
            Dprintf ( "> Disk flash is write protected \n");
            USBH_USR_ApplicationState = USH_USR_FS_DRAW;
            break;
        }
        //Dprintf("USB Write File Standby\n");
        gUSB_MSC_Status = 1;    //이변수가 1일때 파일읽기 쓰기를 할수있다.
		gUSB_MSC_StatusDump = 1;
        //Dgetch();
#if 0    
        f_mount(0, &fatfs);
    
        if(f_open(&file, "0:STM32.TXT",FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
        { 
            /* Write buffer to file */
            bytesToWrite = sizeof(writeTextBuff); 
            res= f_write (&file, writeTextBuff, bytesToWrite, (void *)&bytesWritten);   
      
            if((bytesWritten == 0) || (res != FR_OK)) /*EOF or Error*/
            {
                //LCD_ErrLog("> STM32.TXT CANNOT be writen.\n");
            }
            else
            {
                //LCD_UsrLog("> 'STM32.TXT' file created\n");
            }
      
            f_close(&file);
            f_mount(0, NULL); 
        }    
        else
        {
            //LCD_UsrLog ("> STM32.TXT created in the disk\n");
        }
#endif        
        USBH_USR_ApplicationState = USH_USR_FS_WRITEFILE;    
        break;    
    
        default: break;
    }
    return(0);
}

#if 0 //원본소스이다.
int USBH_USR_MSC_Application(void)
{
    FRESULT res;
    uint8_t writeTextBuff[] = "STM32 Connectivity line Host Demo application using FAT_FS   ";
    uint16_t bytesWritten, bytesToWrite;
  
    switch(USBH_USR_ApplicationState)
    {
        case USH_USR_FS_INIT: 
    
        /* Initialises the File System*/
        if ( f_mount( 0, &fatfs ) != FR_OK ) 
        {
            /* efs initialisation fails*/
            //LCD_ErrLog("> Cannot initialize File System.\n");
            return(-1);
        }
        //LCD_UsrLog("> File System initialized.\n");
        //LCD_UsrLog("> Disk capacity : %d Bytes\n", USBH_MSC_Param.MSCapacity * USBH_MSC_Param.MSPageLength); 
    
        if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
        {
            //LCD_ErrLog((void *)MSG_WR_PROTECT);
        }
    
        USBH_USR_ApplicationState = USH_USR_FS_READLIST;
    break;
    
    case USH_USR_FS_READLIST:    
        //LCD_UsrLog((void *)MSG_ROOT_CONT);
        Explore_Disk("0:/", 1);
        line_idx = 0;   
        USBH_USR_ApplicationState = USH_USR_FS_WRITEFILE;    
    break;
    
    case USH_USR_FS_WRITEFILE:
    
        //LCD_SetTextColor(Green);
        //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
        //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key to write file");
        //LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
        Dprintf("Press Key to write file\n");
        USB_OTG_BSP_mDelay(100);
    
        /*Key B3 in polling*/
        /*
        while((HCD_IsDeviceConnected(&USB_OTG_Core)) &&  (STM_EVAL_PBGetState (BUTTON_KEY) == SET))          
        {
            Toggle_Leds();
        }
        */
        Dprintf("Key B3 in polling\n");
        Dgetch();
        /* Writes a text file, STM32.TXT in the disk*/
        LCD_UsrLog("> Writing File to disk flash ...\n");
        if(USBH_MSC_Param.MSWriteProtect == DISK_WRITE_PROTECTED)
        {      
            //LCD_ErrLog ( "> Disk flash is write protected \n");
            USBH_USR_ApplicationState = USH_USR_FS_DRAW;
            break;
        }
    
        /* Register work area for logical drives */
        f_mount(0, &fatfs);
    
        if(f_open(&file, "0:STM32.TXT",FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
        { 
            /* Write buffer to file */
            bytesToWrite = sizeof(writeTextBuff); 
            res= f_write (&file, writeTextBuff, bytesToWrite, (void *)&bytesWritten);   
      
            if((bytesWritten == 0) || (res != FR_OK)) /*EOF or Error*/
            {
                //LCD_ErrLog("> STM32.TXT CANNOT be writen.\n");
            }
            else
            {
                //LCD_UsrLog("> 'STM32.TXT' file created\n");
            }
      
            /*close file and filesystem*/
            f_close(&file);
            f_mount(0, NULL); 
        }    
        else
        {
            //LCD_UsrLog ("> STM32.TXT created in the disk\n");
        }
        USBH_USR_ApplicationState = USH_USR_FS_DRAW; 
    
        //LCD_SetTextColor(Green);
        //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
        //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "To start Image slide show Press Key.");
        //LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
        Dprintf("To start Image slide show Press Key.\n");  
    break;
    
    case USH_USR_FS_DRAW:    
        Dprintf("Key Press Standby\n");
        /*Key B3 in polling*/
        /*
        while((HCD_IsDeviceConnected(&USB_OTG_Core)) && (STM_EVAL_PBGetState (BUTTON_KEY) == SET))
        {
            Toggle_Leds();
        }
        */
        Dgetch();
        while(HCD_IsDeviceConnected(&USB_OTG_Core))
        {
            if ( f_mount( 0, &fatfs ) != FR_OK ) 
            {
                /* fat_fs initialisation fails*/
                return(-1);
            }
            return Image_Browser("0:/");
        }
    break;
    
        default: break;
    }
    return(0);
}
#endif

/**
* @brief  Explore_Disk 
*         Displays disk content
* @param  path: pointer to root path
* @retval None
*/
#if 0
static uint8_t Explore_Disk (char* path , uint8_t recu_level)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;
    char tmp[14];
  
    res = f_opendir(&dir, path);
    if (res == FR_OK) 
    {
        while(HCD_IsDeviceConnected(&USB_OTG_Core)) 
        {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) 
            {
                break;
            }
            if (fno.fname[0] == '.')
            {
                continue;
            }

            fn = fno.fname;
            strcpy(tmp, fn); 

            line_idx++;
            if(line_idx > 9)
            {
                line_idx = 0;
                //LCD_SetTextColor(Green);
                //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 42, "                                              ");
                //LCD_DisplayStringLine( LCD_PIXEL_HEIGHT - 30, "Press Key to continue...");
                //LCD_SetTextColor(LCD_LOG_DEFAULT_COLOR); 
                
                Dprintf("Press Key to continue...\n");
        
                /*Key B3 in polling*/
                /*
                while((HCD_IsDeviceConnected(&USB_OTG_Core)) && (STM_EVAL_PBGetState (BUTTON_KEY) == SET))
                {
                    Toggle_Leds();          
                }
                */
                Dgetch();
            } 
      
            if(recu_level == 1)
            {
                //LCD_DbgLog("   |__");
            }
            else if(recu_level == 2)
            {
                //LCD_DbgLog("   |   |__");
            }
            if((fno.fattrib & AM_MASK) == AM_DIR)
            {
                strcat(tmp, "\n"); 
                //LCD_UsrLog((void *)tmp);
            }
            else
            {
                strcat(tmp, "\n"); 
                //LCD_DbgLog((void *)tmp);
            }

            if(((fno.fattrib & AM_MASK) == AM_DIR)&&(recu_level == 1))
            {
                Explore_Disk(fn, 2);
            }
        }
    }
    return res;
}
#endif 

#if 0
static uint8_t Image_Browser (char* path)
{
  FRESULT res;
  uint8_t ret = 1;
  FILINFO fno;
  DIR dir;
  char *fn;
  
  res = f_opendir(&dir, path);
  if (res == FR_OK) {
    
    for (;;) {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0) break;
      if (fno.fname[0] == '.') continue;

      fn = fno.fname;
 
      if (fno.fattrib & AM_DIR) 
      {
        continue;
      } 
      else 
      {
        if((strstr(fn, "bmp")) || (strstr(fn, "BMP")))
        {
          res = f_open(&file, fn, FA_OPEN_EXISTING | FA_READ);
          Dprintf("Show Image\n");
			Show_Image();
          USB_OTG_BSP_mDelay(100);
          ret = 0;
          while((HCD_IsDeviceConnected(&USB_OTG_Core)) && \
            (STM_EVAL_PBGetState (BUTTON_KEY) == SET))
          {
            Toggle_Leds();
          }
          f_close(&file);
          
        }
      }
    }  
  }
  
  #ifdef USE_USB_OTG_HS 
  LCD_LOG_SetHeader(" USB OTG HS MSC Host");
#else
  LCD_LOG_SetHeader(" USB OTG FS MSC Host");
#endif
  LCD_LOG_SetFooter ("     USB Host Library v2.1.0" );
  LCD_UsrLog("> Disk capacity : %ld Bytes\n", USBH_MSC_Param.MSCapacity * \
      USBH_MSC_Param.MSPageLength); 
  USBH_USR_ApplicationState = USH_USR_FS_READLIST;

  return ret;
 }
#endif

 /**
* @brief  Show_Image 
*         Displays BMP image
* @param  None
* @retval None
*/
 #if 0
static void Show_Image(void)
{
  
  uint16_t i = 0;
  uint16_t numOfReadBytes = 0;
  FRESULT res; 
  
  //LCD_SetDisplayWindow(239, 319, 240, 320);
  //LCD_WriteReg(R3, 0x1008);
  //LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  
  /* Bypass Bitmap header */ 
  f_lseek (&file, 54);
  
  while (HCD_IsDeviceConnected(&USB_OTG_Core))
  {
    res = f_read(&file, Image_Buf, IMAGE_BUFFER_SIZE, (void *)&numOfReadBytes);
    if((numOfReadBytes == 0) || (res != FR_OK)) /*EOF or Error*/
    {
      break; 
    }
    for(i = 0 ; i < IMAGE_BUFFER_SIZE; i+= 2)
    {
      //LCD_WriteRAM(Image_Buf[i+1] << 8 | Image_Buf[i]); 
    } 
  }  
}
#endif

/**
* @brief  USBH_USR_DeInit
*         Deint User state and associated variables
* @param  None
* @retval None
*/
void USBH_USR_DeInit(void)
{
  USBH_USR_ApplicationState = USH_USR_FS_INIT;
}


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/**
* @}
*/
#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

