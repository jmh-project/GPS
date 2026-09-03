/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif
#include "diskio.h"
#include "stm32f4xx.h"
#include "ffconf.h"
#include "stm324xg_eval_sdio_sd.h"
#include "usb_conf.h"
#include "usbh_msc_core.h"
#include "TEECO_C_Define.h"
#include "TEECO_Define.h"
#include "Debug_Uart.h"
#include "dos.h"

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                     USB_Host;


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                  */
/*-----------------------------------------------------------------------*/

static volatile DSTATUS Stat = STA_NOINIT;	/* Disk status */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    SD_Error res = SD_OK;
    
    if(drv==SD)
    {
    //Dprintf("disk_initialize::drv=%d\n", drv);
        /* Configure the NVIC Preemption Priority Bits */
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

        NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure); 

        res =  SD_Init(); 

        if(res == SD_OK)
        {
            res = (SD_Error)0x0;
        } 
        return ((DSTATUS)res);
    }
    else if(drv==USB)
    {
        if(HCD_IsDeviceConnected(&USB_OTG_Core))
        {  
            Stat &= ~STA_NOINIT;
        }    
        return Stat;  
    }
	else if(drv==RAM)
	{
		//BSP 에서 램을 초기화하기때문에 할필요가 없다.
        Stat &= ~STA_NOINIT;
        return Stat;  		
	}
    return ((DSTATUS)res);
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0) */
)
{
//Dprintf("disk_status::drv=%d\n", drv);
	if (drv>=3) return STA_NOINIT;		/* Supports only single drive */
	return 0;
	/*
	if(drv==RAM)
	{
		Stat = STA_NOINIT;  
		Stat &= ~STA_NOINIT;
		return Stat;		
	}
	*/
}


/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	        /* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
    SD_Error status = SD_OK;
	uint8_t	sts=0;
    //BYTE status = USBH_MSC_OK;
    
    if(drv==SD)
    {
        //int s;
    //Dprintf("disk_read::drv=%d\n", drv);

        //s = sizeof(buff) / 512;
        SD_ReadMultiBlocks(buff, sector << 9, 512, count);

        /* Check if the Transfer is finished */
        status =  SD_WaitReadOperation();
        while(SD_GetStatus() != SD_TRANSFER_OK);

        if (status == SD_OK)
        {
            return RES_OK;
        }
        else
        {
            return RES_ERROR;
        }
    }
    else if(drv==USB)
    {
        //Dprintf(" disk_read::drv=%d\n", drv);   
        //if (drv || !count) return RES_PARERR;
        if (Stat & STA_NOINIT) return RES_NOTRDY;

        if(HCD_IsDeviceConnected(&USB_OTG_Core))
        {  
            do
            {
                sts = USBH_MSC_Read10(&USB_OTG_Core, buff, sector, (uint32_t)(512 * count));
                USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);

                if(!HCD_IsDeviceConnected(&USB_OTG_Core))
                { 
                    return RES_ERROR;
                }      
            }
            while(sts == USBH_MSC_BUSY );
        }

        if(status == USBH_MSC_OK)   return RES_OK;
        return RES_ERROR;        
    }
	else if(drv==RAM)
	{
		uint32_t *pSrcBuffer = (uint32_t *)buff;
		uint32_t BufferSize = (BLOCK_SIZE * count)/4; 
		uint32_t *pSdramAddress = (uint32_t *) (RAM_DEVICE_ADDR + (sector * BLOCK_SIZE)); 
		  
		for(; BufferSize != 0; BufferSize--)
		{
			*pSrcBuffer++ = *(__IO uint32_t *)pSdramAddress++;                
		}   
		return RES_OK;
	}
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/
/* The FatFs module will issue multiple sector transfer request
/  (count > 1) to the disk I/O layer. The disk function should process
/  the multiple sector transfer properly Do. not translate it into
/  multiple single sector transfers to the media, or the data read/write
/  performance may be drasticaly decreased. */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
    BYTE status = USBH_MSC_OK;
    //SD_Error status = SD_OK;
    
    if(drv==SD)
    {
    //Dprintf("disk_write::drv=%d\n", drv);

        SD_WriteMultiBlocks((BYTE *)buff, sector << 9, 512, count);

        /* Check if the Transfer is finished */
        status = SD_WaitWriteOperation();
        while(SD_GetStatus() != SD_TRANSFER_OK);     

        if (status == SD_OK)
        {
            return RES_OK;
        }
        else
        {
            return RES_ERROR;
        }
    }
    else if(drv==USB)
    {
        //Dprintf(" disk_write::drv=%d\n", drv);     
        //if (drv || !count) return RES_PARERR;
        if (Stat & STA_NOINIT) return RES_NOTRDY;
        if (Stat & STA_PROTECT) return RES_WRPRT;

        if(HCD_IsDeviceConnected(&USB_OTG_Core))
        {  
            do
            {
                status = USBH_MSC_Write10(&USB_OTG_Core,(BYTE*)buff,sector,512 * count);
                USBH_MSC_HandleBOTXfer(&USB_OTG_Core, &USB_Host);

                if(!HCD_IsDeviceConnected(&USB_OTG_Core))
                { 
                return RES_ERROR;
                }
            }
            while(status == USBH_MSC_BUSY );
        }

        if(status == USBH_MSC_OK)    return RES_OK;
        return RES_ERROR;
    }
	else if(drv==RAM)
	{
		uint32_t *pDstBuffer = (uint32_t *)buff;
		uint32_t BufferSize = (BLOCK_SIZE * count)/4 + count; 
		uint32_t *pSramAddress = (uint32_t *) (RAM_DEVICE_ADDR + (sector * BLOCK_SIZE)); 

		for(; BufferSize != 0; BufferSize--)
		{
			*(__IO uint32_t *)pSramAddress++ = *pDstBuffer++;    
		}   
		return RES_OK;		
	}
    return RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
extern SD_CardInfo SDCardInfo;
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    DRESULT res = RES_OK;
//Dprintf("disk_ioctl::drv=%d\n", drv);
    if(drv>=3) return RES_PARERR;
    
    switch (ctrl) 
    {
        case CTRL_SYNC :		/* Make sure that no pending write process */    
            res = RES_OK;
        break;
        
        case GET_SECTOR_COUNT :	  // Get number of sectors on the disk (DWORD)
            if(drv==SD)
            {
				SD_GetCardInfo(&SDCardInfo);
				*(DWORD*)buff = SDCardInfo.CardCapacity / BLOCK_SIZE;
				res = RES_OK;
            }
            else if(drv==USB)
            {
                *(DWORD*)buff = (DWORD) USBH_MSC_Param.MSCapacity;                
            }
			else if(drv==RAM)
			{
				*(DWORD*)buff = RAM_DEVICE_SIZE / BLOCK_SIZE;				
			}
            res = RES_OK;
        break;

        case GET_SECTOR_SIZE :	  // Get R/W sector size (WORD) 
            *(WORD*)buff = BLOCK_SIZE;
            res = RES_OK;
        break;

        case GET_BLOCK_SIZE :	    // Get erase block size in unit of sector (DWORD)
            if(drv==SD)       *(DWORD*)buff = BLOCK_SIZE;
            else if(drv==USB) *(DWORD*)buff = BLOCK_SIZE;
            else if(drv==RAM) *(DWORD*)buff = BLOCK_SIZE;
            res = RES_OK;
        break;
        default:
            res = RES_PARERR;        break;
    }
    return res;
}
#ifdef __cplusplus
}
#endif
