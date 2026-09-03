/**
  ******************************************************************************
  * @file       	DOS.c
  * @author  	(주)자이언텍 기술개발지원팀
  * @version 	V0.1
  * @date    	2014년 12월19일
  * @brief    	DOS 함수
                
  *          
  * @attention
  * 이파일은 자이언텍 지적재산권이므로 타사에 소스를 제공해서는 안됩니다. 
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc.</center></h2>
  * @warning  
  * @verbatim
  *  Edit History
  * @endverbatim  
  ******************************************************************************  
  */
#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "TEECO_System.h"
#include "string.h"
#include "dos.h"
#include "MemoryManager.h"
#include "TEECO_Define.h"
#include "TEECO_C_Define.h"

	 
	 
#include "T_socket.h"
#include "T_dhcp.h"
#include "T_loopback.h"
#include "BSP_W5500.h"
	 
	 
	 
/* Private typedef -----------------------------------------------------------*/
#define	BLOCK_SIZE	512		//!< 디스크의 기본물리적사이즈
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS usb_fatfs;
FIL usb_file;

FATFS sd_fatfs;
FIL sd_file;

FATFS ram_fatfs;
FIL ram_file;

FATFS fatfs;
FIL file;

char CurrenrDir[64];
char CurrenrDisk[16];
char Disk_Kind[16];
//BYTE Disk_Buffer[4096] = {0, };   /* File copy buffer */
BYTE Disk_Buffer[MEMORY_BUF_SIZE] = {0, };   /* File copy buffer */

extern volatile U32	Graphics_Address;

/* Private function prototypes -----------------------------------------------*/
extern int File_Format(int *cnt);	//Nand 를 포맷하는함수
void File_Rename(char *s_filename, char *r_filename);
int File_Delete(char *filename);
int File_Find(char *filename);
SD_Error DiskInit(void);
int USBTest(void);
int FileCheck(const char *disk, char *name, int display);
/* Private functions ---------------------------------------------------------*/
#if 1
//이함수는 디렉토리의 파일명을 조사하는 용도로만 사용한다.	
FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;

	uint32_t	year;
	uint32_t month;
	uint32_t day, hour, min;
	uint32_t	total_size=0;
	int	total_file=0;
	int	total_dir=0;
	int	isdir=0;
	char	buf[64],font[70];

#if 0
	FRESULT  fr;
	char	src_file_name[128];
    FIL fsrc;
	UINT br;	
#endif	
//    int i;
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    strcpy(fno.fname,lfn);
    fno.fsize = sizeof lfn;
#endif

    res = f_opendir(&dir, path);                                /* Open the directory */
    if (res == FR_OK) 
    {
        int i = strlen(path);
        for (;;) 
        {
            res = f_readdir(&dir, &fno);                        /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;    /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;                   /* Ignore dot entry */
#if  _USE_LFN
            fn = *fno.fname ? fno.fname : fno.fname;
#else
            fn = fno.fname;
#endif
			year = fno.fdate >> 9;
			year += 1980;
			month = (fno.fdate >> 5) & 0x0f;
			day = fno.fdate & 0x1f;
			hour = (fno.ftime >> 11) & 0x1f;
			min = (fno.ftime >> 5) & 0x3f;
//			sec = (fno.ftime  & 0x1f) * 2;			
			
            if (fno.fattrib & AM_DIR) 
            {                    /* It is a directory */
				isdir=1;
				total_dir++;
                //Dprintf("[%24s]",fn); 
                
				//sprintf(&path[i], "/%s", fn);
                //res = scan_files(path);
                //if (res != FR_OK) break;
                //path[i] = 0;
            } 
            else 
            {                                       /* It is a file. */
                //Dprintf("%s/%s", path, fn);
#if 0
				//!< 다른함수를 호출할경우는 그함수는 마운트를 따로 하면 안된다.
				//!< +++++++++++++ 파일읽기 테스트 시작
				//!< 디렉토리의 파일을 읽기위해서는 다음과 같이 하면된다.
				/* Open source file on the drive 1 */
				sprintf(src_file_name,"1:%s", fn);
				fr = f_open(&fsrc, src_file_name, FA_OPEN_EXISTING | FA_READ);
				if(fr != FR_OK)
				{
					Dprintf("scan_files:Error Code=%04x\n", fr);
					FileErrorDisplay(fr);	
				}
				//for (;;) 
				{
					fr = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);  /* Read a chunk of source file */
					if (fr || br == 0) break; /* error or eof */
				}
				Dprintf("%02x %02x %02x %02x %02x %02x\n", Disk_Buffer[0], Disk_Buffer[1], Disk_Buffer[2], Disk_Buffer[3], Disk_Buffer[4], Disk_Buffer[5]);
				/* Close open files */
				f_close(&fsrc);			
				//!< -------------------------- 파일읽기 테스트 끝				
#endif			
				isdir=0;
				total_file++;
                //Dprintf("%24s", fn);
            }
			//Dprintf("%4d-%02d-%02d %02d:%02d\t" , year, month, day, hour, min); // fno.fsize);
			Dprintf("%4d-%02d-%02d %02d:%02d " , year, month, day, hour, min); // fno.fsize);
			//if(isdir==1) Dprintf("<DIR>\t\t%s\n", fn);
			if(isdir==1) {
				Dprintf("<DIR> %s\n", fn);
				//sprintf(font,"<DIR> %s\n",fn);
				
			}
			else 
			{
				IntToStringWon(fno.fsize, buf, sizeof(buf));	
				Dprintf("%14s  %s\n", buf, fn);	
				
				sprintf(font,"%s\n",fn);	
				DebugMessagePrintf(font);
				
				//sprintf(font,"%s",fn);
				//FileCheck(Disk_Kind,font,0);
			}
			total_size+=	fno.fsize;		
        }
		f_closedir(&dir);
    }
	IntToStringWon(total_size, buf, sizeof(buf));	
	Dprintf("\t%3d File\t%s byte\n", total_file, buf);
	Dprintf("\t%3d Directory\t", total_dir);
    return res;
}
#endif

#if 0
// path 에 주어진 디렉토리의 파일리스트를 표시한다.
FRESULT Dir_List(char *path)
{
	FRESULT res;
    char    buf[64];
	if((res=f_chdir(path))==FR_OK)
	{
		Dprintf("Dir_List::Path Dir name->%s\n", path);
		if(strlen(path) != 0)
			sprintf(buf,"%s%s", Disk_Kind, CurrenrDir);
		else
			sprintf(path,"%s/", Disk_Kind);	//!< 현재의 디렉토리를 표시하는방법 dir/
		scan_files(buf);	
	}
	else
	{
		Dprintf("Dir_List::Directory name ErrorCode=%d\n", res);
		FileErrorDisplay(res);							
	}
	return(FR_OK);
}
#endif

#if 0
/**
	@brief 리소스매니져에서 dir 을 입력하면 실행되는 함수이다.
*/
FRESULT Dir_List1(void)
{
	FRESULT res;
    char    buf[64];
	char path[32];
	
	sprintf(path,"%s/", Disk_Kind);	//!< 현재의 디렉토리를 표시하는방법 dir/
	if((res=f_chdir(path))==FR_OK)
	{
		Dprintf("Dir_List::Path Dir name:%s\n", path);
		sprintf(buf,"%s%s",Disk_Kind, CurrenrDir);
		scan_files(buf);	
	}
	else
	{
		Dprintf("Dir_List::Directory name ErrorCode=%d\n", res);
		FileErrorDisplay(res);							
	}	
	//!< 디스크의 공간을 표시한다.
    DWORD fre_clust, fre_sect;//, tot_sect;
	FATFS	*fs;	
	if((f_getfree(buf, &fre_clust, &fs))==FR_OK)
	{
		/* Get total sectors and free sectors */
		//tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 512 bytes/sector) */
		//Dprintf("\t%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect / 2, fre_sect / 2);		
		IntToStringWon((fre_sect / 2), buf, sizeof(buf));
		Dprintf("%s Kbyte available.\n",  buf);			
	}
	return(FR_OK);
}
#endif

FRESULT ChangeDir(char *path)
{
	//FRESULT res;
    BYTE    buf[64];

	if(!strcmp(path,".."))
	{
		sprintf((char *)buf,"%s..", Disk_Kind);
		//f_chdir("1:..");
		f_chdir((TCHAR *)buf);
		GetDirectory(CurrenrDir);		
		//CurrenrDir[0] = 0;
	}
	else
	{
		sprintf((char *)buf,"%s%s", Disk_Kind, path);
		if(f_chdir((TCHAR *)buf)==FR_OK)
		{
			//strcpy((char *)CurrenrDir, path);
			GetDirectory(CurrenrDir);						
		}
		else
		{
			Dprintf("\t%s 디렉토리를 찾을수 없습니다.\n", path);
			//FileErrorDisplay(res);					
		}	
	}
	//Dprintf("ChangeDir::Change Dir name:%s\n", path);
	return(FR_OK);	
}

FRESULT FileView(char *filename)
{
	//FRESULT res;
    char    buf[64];
    char line[82]; 	/* Line buffer */	
	gv.usbSaveWaitFlag=1;
	sprintf(buf,"%s%s", Disk_Kind, filename);
    if(f_open(&sd_file,  buf,  FA_OPEN_EXISTING |  FA_READ ) == FR_OK) {
		/* Read all lines and display it */
		while (f_gets(line, sizeof(line), &sd_file)) {
			Dprintf(line);
			//DebugMessagePrintf(line);
			if(Dstatus()==1) {
				if(Dgetch()==0x1b) break;
			}
		}
		gv.usbSaveWaitFlag=0;
		/* Close open files */
		f_close(&sd_file);		
	}
	else
	{
		Dprintf("\t%s파일은 없는 파일입니다..\n", filename);
		//Dprintf("FileView::File not found\n");
		//FileErrorDisplay(res);	
	}	
	gv.usbSaveWaitFlag=0;
	return(FR_OK);		
}

void FileErrorDisplay(FRESULT res)
{
	switch(res)
	{
		case FR_DISK_ERR:					Dprintf(" (1) A hard error occurred in the low level disk I/O layer \n"); break;
		case FR_INT_ERR:					Dprintf(" (2) Assertion failed \n"); break;
		case FR_NOT_READY:				Dprintf(" (3) The physical drive cannot work \n"); break;
		case FR_NO_FILE:					Dprintf(" (4) Could not find the file \n"); break;
		case FR_NO_PATH:					Dprintf(" (5) Could not find the path \n"); break;
		case FR_INVALID_NAME:			Dprintf(" (6) The path name format is invalid \n"); break;
		case FR_DENIED:						Dprintf(" (7) Access denied due to prohibited access or directory full \n"); break;
		case FR_EXIST:						Dprintf(" (8) Access denied due to prohibited access \n"); break;
		case FR_INVALID_OBJECT:		Dprintf("  (9) The file/directory object is invalid \n"); break;
		case FR_WRITE_PROTECTED:	Dprintf("  (10) The physical drive is write protected \n"); break;
		case FR_INVALID_DRIVE:		Dprintf("  (11) The logical drive number is invalid \n"); break;
		case FR_NOT_ENABLED:			Dprintf("  (12) The volume has no work area \n"); break;
		case FR_NO_FILESYSTEM:		Dprintf("  (13) There is no valid FAT volume \n"); break;
		case FR_MKFS_ABORTED:		Dprintf("  (14) The f_mkfs() aborted due to any parameter error \n"); break;
		case FR_TIMEOUT:								Dprintf("  (15) Could not get a grant to access the volume within defined period \n"); break;
		case FR_LOCKED:								Dprintf("  (16) The operation is rejected according to the file sharing policy \n"); break;
		case FR_NOT_ENOUGH_CORE:				Dprintf(" * (17) LFN working buffer could not be allocated \n"); break;
		case FR_TOO_MANY_OPEN_FILES:		Dprintf("  (18) Number of open files > _FS_SHARE \n"); break;
		case FR_INVALID_PARAMETER:			Dprintf("  (19) Given parameter is invalid \n"); break;
	}
}

#if 0
//디스크의 라벨을 얻는다.
int GetLabel(void)
{
	FRESULT res;
	char	str[128];
	char	buf[128];
	
	sprintf(buf, "%s", Disk_Kind);
	if((res=f_getlabel(buf, str, 0))==FR_OK)
	{
		Dprintf("Disk Label:%s\n", str);
	}
	else
	{
		Dprintf("GetLabel::Error Code=%d\n", res);
		FileErrorDisplay(res);		
	}	
	return(1);
}
#endif

//디스크의 라벨을 갱신한다.
int SetLabel(char *label)
{
	FRESULT res;
	char	str[128];

	sprintf(str,"%s%s\n", Disk_Kind, label);
	if((res=f_setlabel(str))==FR_OK)
	{
		//Dprintf("Disk Label:%s\n", str);
	}
	else
	{
		Dprintf("SetLabel::Error Code=%d\n", res);
		FileErrorDisplay(res);
		return(0);
	}
	return(1);
}
void SD_UnMount(void);
void SD_Mount(void);
//디스크를 포맷한다.
int Format(int disk)
{
	FRESULT res;
	char	buf[32];
	//BYTE work[512];
	SD_Mount();
	if(disk==SD) 
	{
		strcpy(buf,"1:");
		//SD 가 삽입이 안되어 있으면 경고표시하고 탈출한다.
		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==1) 
		{
			Dprintf("SD Card가 없습니다.\n"); 
			return(0);
		}
	}
	else if(disk==USB) 
	{
		strcpy(buf,"0:");
		//USB가 마운트 되지 않았으면 경고하고 탈출한다.
		if(gUSB_MSC_Status==0)
		{
			Dprintf("USB 를 찾을수없습니다.\n");
			return(0);
		}
	}
	else if(disk==RAM) 
	{
		strcpy(buf,"2:");
	}
	if((res=f_mkfs(buf, FM_ANY,0, NULL, 0))==FR_OK)
	{
		Dprintf("%s diskformat success\n", disk);
	}
	else  
	{
		Dprintf("DiskFormat::Error Code=%d\n", res);
		FileErrorDisplay(res);	
		return(0);		
	}
	SD_UnMount();
	return(1);
}
//파일의 이름을 변경한다.
int Rename(char *src, char *dst)
{
	FRESULT res;
	char	old_name[128];
	char	new_name[128];
	//f_chdir("1:..");
	sprintf(old_name,"%s/%s", Disk_Kind, src);
	sprintf(new_name,"%s", dst);
	
	if((res = f_rename(old_name, new_name))==FR_OK)
	{
		//Dprintf("Rename::OK\n");
	}
	else
	{
		Dprintf("\t%s파일을 찾을수 없습니다.\n", src);
		FileErrorDisplay(res);
		return(0);
	}	
	return(1);
}

//파일/디렉토리의 이름을 삭제한다.
void SD_UnMount(void);
int Remove(char *name)
{
	FRESULT res;
	char	file_name[128];
    
    SD_Mount();
	sprintf(file_name,"%s%s", Disk_Kind, name);
	if((res = f_unlink(file_name))==FR_OK)
	{
		Dprintf("Delete::OK %s delete\n", name);
	}
	else
	{
		Dprintf("Remove:Error Code=%04x %s\n", res, name); 
		//FileErrorDisplay(res);
        SD_UnMount();
		return(0);		
	}	
    SD_UnMount();
	return(1);
}

//!< 디렉토리를 생성한다. 
//!< disk -> "0: ",  "1:" , "2:"
int MakeDir(char *disk, char *name)
{
	//FRESULT res;
	char	file_name[128];
	
	sprintf(file_name,"%s%s", Disk_Kind, name);
	
	if(f_mkdir(file_name)==FR_OK)
	{
		//Dprintf("makedir::OK %s makedir\n", name);
	}
	else
	{
		Dprintf("\t %s/%s디렉토리를 만들수없습니다\n", disk, name);
		return(0);
		//FileErrorDisplay(res);		
	}	
	return(1);
}

//typedef struct {
//	DWORD	fsize;			/* File size */
//	WORD	fdate;			/* Last modified date */
//	WORD	ftime;			/* Last modified time */
//	BYTE	fattrib;			/* Attribute */
//	TCHAR	fname[13];		/* Short file name (8.3 format) */
//#if _USE_LFN
//	TCHAR*	lfname;			/* Pointer to the LFN buffer */
//	UINT 	lfsize;			/* Size of LFN buffer in TCHAR */
//#endif
//} FILINFO;

/*
	bit15:9  	Year from 1980 (0..127)
	bit8:5   	Month (1..12)
	bit4:0   	Day in month(1..31)
	
	bit15:11 	Hour (0..23)
	bit10:5  	Minute (0..59)
	bit4:0  	Second / 2 (0..29)
*/

//파일이나 서브디렉토리의 시간을 바꾼다.
FRESULT Set_TimeStamp (
    char *obj,     /* Pointer to the file name */
    char *year1,
    char *month1,
    char *day1,
    char *hour1,
    char *min1,
    char *sec1
)
{
	FRESULT res;
    FILINFO fno;
	char	file_name[128];
    
	int year = atoi(year1);
    int month = atoi(month1);
    int mday = atoi(day1);
    int hour = atoi(hour1);
    int min = atoi(min1);
    int sec = atoi(sec1);	
	
	sprintf(file_name,"%s%s", Disk_Kind, obj);
	
    fno.fdate = (WORD)(((year - 1980) * 512U) | month * 32U | mday);
    fno.ftime = (WORD)(hour * 2048U | min * 32U | sec / 2U);
	
	if((res = f_utime(file_name, &fno))==FR_OK)
	{
		Dprintf("Set_TimeStamp::Time Change OK\n");
	}
	else
	{
		Dprintf("Set_TimeStamp:Error Code=%04x\n", res);
		FileErrorDisplay(res);		
	}
	return(FR_OK);
}

//extern void DiskInfo(void);
void SD_Mount(void) {
    int errorCnt=0;
    for(errorCnt=0;errorCnt<10;errorCnt++)
    {
        if(DiskInit() != SD_OK)
        {
            Dprintf("SD_Mount::SD Card Error\n");
        }
        if ( f_mount(&sd_fatfs, "1:", 1) != FR_OK )       
        {
            Dprintf("SD_Mount::Cannot initialize File System. [%d]\n",errorCnt);        
        }
        else
        {
            if(errorCnt >0) Dprintf("[ERROR] SD connection error = %d\n",errorCnt);    
            strcpy(CurrenrDisk, "SD"); strcpy(Disk_Kind, "1:");
            break;
        }
    }
}

void SD_UnMount(void)
{
	f_mount(0, "1:", 0);        	
}

void RAM_Mount(void)
{
#if 0
	FRESULT  res;
    if ( f_mount(&ram_fatfs, "2:", 1) != FR_OK )       Dprintf("RAM_Mount::Cannot initialize File System.\n");        
    //if ( f_mount(RAM, &ram_fatfs ) != FR_OK )       Dprintf("RAM_Mount::Cannot initialize File System.\n");        
	if((res=f_mkfs(buf, FM_ANY,0, NULL, 0))==FR_OK)
	{
		//strcpy(CurrenrDisk, "RAM"); strcpy(Disk_Kind, "2:");  
		//f_chdrive(Disk_Kind); 
		//GetDirectory(CurrenrDir);		
	}
	else
	{
		Dprintf("RamDiskFormat::Error Code=%d\n", res);
		FileErrorDisplay(res);		

	}		
#endif
}

void RAM_UnMount(void)
{
	//f_mount(RAM, NULL); 	
	f_mount(0, "2:", 0);        		
}

int USB_Mount(void)
{
	int ret = USBTest();
	if(gUSB_MSC_Status==1 && ret==1)
	{
		if ( f_mount(&usb_fatfs, "0:", 1 ) != FR_OK )     Dprintf("USB_Mount::Cannot initialize File System.\n");        
		else 	
		{
			SD_Mount();
			Dprintf("USB Mount 가 성공했습니다\n");
		}
	}
	return(ret);
}

void USB_UnMount(void) {
	//f_mount(USB, NULL); 	
	f_mount(0, "0:", 0);        		
}

int Disk_Mount(char *disk)
{
	if(!strcmp(disk,"sd")) SD_Mount();
	else if(!strcmp(disk,"usb")) USB_Mount();
	else if(!strcmp(disk,"ram")) RAM_Mount();
	else if(!strcmp(disk,"nand")) NandMBRCashe();
	else { Dprintf("입력문자는 sd 나 usb 를 사용합니다.\n", disk); return 0; }
	return 1;
}

int Disk_UnMount(char *disk)
{
	if(!strcmp(disk,"sd")) SD_UnMount();
	else if(!strcmp(disk,"usb")) USB_UnMount();
	else { Dprintf("umount error %s is not use[sd or usb]\n", disk); return 0; }
	Dprintf("umount ok\n");
	return 1;
}

//현재 디렉토리를 반환한다.
int GetDirectory(char *cur_dir)
{
	FRESULT res;
	int 		i, cnt=0;
	char	file_name[64];

	if(strcmp(CurrenrDisk,"NAND")) //낸드가 아닐때만 진입한다.
	{	
		if((res = f_getcwd (file_name, 128))==FR_OK)
		{
			//Dprintf("GetDirectory::%s\n",file_name);
			for(i=3; i<strlen(file_name); i++)
			{
				cur_dir[cnt++] = file_name[i];
			}
			cur_dir[cnt]=0;
		}
		else
		{
			Dprintf("GetDirectory:Error Code=%04x\n", res);
			FileErrorDisplay(res);		
		}
		return(res);	
	}
	return(1);
}

//디스크공간조사
int DiskSpace(void)
{
	FRESULT res;
    DWORD fre_clust, fre_sect, tot_sect;
	FATFS	*fs;
	char	buf[64];

    /* Get volume information and free clusters of drive 1 */
    
	sprintf(buf, "%s", Disk_Kind);
	//if((res = f_getfree("1:", &fre_clust, &fs))==FR_OK)
	if((res = f_getfree(buf, &fre_clust, &fs))==FR_OK)
	{
		/* Get total sectors and free sectors */
		tot_sect = (fs->n_fatent - 2) * fs->csize;
		fre_sect = fre_clust * fs->csize;

		/* Print the free space (assuming 512 bytes/sector) */
		Dprintf("%10lu KiB total drive space.\n%10lu KiB available.\n", tot_sect / 2, fre_sect / 2);			
	}
	else
	{
		Dprintf("DiskSpace:Error Code=%04x\n", res);
		FileErrorDisplay(res);	
		return -1;		
	}
	return(1);
}

#if 0
int DiskCopy(char *srcdisk, char *src, char *dstdisk, char *dest)
{
	FRESULT fr;

	char	src_file_name[128];
	char	dst_file_name[128];
    FIL fsrc, fdst;	/* File objects */	
	UINT br, bw;         /* File read/write count */
	
	if(!strcmp(srcdisk, "sd:"))
	{
		sprintf(src_file_name,"1:%s", src);
	}
	else if(!strcmp(srcdisk, "usb:"))
	{
		sprintf(src_file_name,"0:%s", src);
	}
	else if(!strcmp(srcdisk, "ram:"))
	{
		sprintf(src_file_name,"2:%s", src);
	}

	if(!strcmp(dstdisk, "sd:"))
	{
		sprintf(dst_file_name,"1:%s", dest);
	}
	else if(!strcmp(dstdisk, "usb:"))
	{
		sprintf(dst_file_name,"0:%s", dest);
	}	
	else if(!strcmp(dstdisk, "ram:"))
	{
		sprintf(dst_file_name,"2:%s", dest);
	}	
	
    /* Open source file on the drive 1 */
    fr = f_open(&fsrc, src_file_name, FA_OPEN_EXISTING | FA_READ);
	if(fr != FR_OK)
	{
		Dprintf("FileCopy:Error Code=%04x\n", fr);
		FileErrorDisplay(fr);	
		return -1;		
	}	
    /* Create destination file on the drive 0 */
    fr = f_open(&fdst, dst_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if(fr != FR_OK)
	{
		Dprintf("FileCopy:Error Code=%04x\n", fr);
		FileErrorDisplay(fr);	
		return -1;		
	}
    /* Copy source to destination */
    for (;;) 
	{
        fr = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);  /* Read a chunk of source file */
        if (fr || br == 0) break; /* error or eof */
        fr = f_write(&fdst, Disk_Buffer, br, &bw);            /* Write it to the destination file */
        if (fr || bw < br) break; /* error or disk full */
    }

    /* Close open files */
    f_close(&fsrc);
    f_close(&fdst);
	Dprintf("FileCopy OK\n");
	return(1);
}
#endif

//!< 파일이나 디렉토리가 있는지를 검사한다.
//!< 파일이면 리턴3을 디렉토리이면 리턴 2를 에러면 0 을 리턴한다.
//!< disk=Disk_Kind, 
int FileCheck(const char *disk, char *name, int display)
{
	//FRESULT res;
	FILINFO fileinfo;
	uint32_t	year;
	uint32_t month;
	uint32_t day, hour, min, sec;

	char	file_name[64];
	sprintf(file_name,"%s%s", disk, name);
	
	if(( f_stat(file_name, &fileinfo))==FR_OK)
	{
		//Dprintf("FileCheck::file or directory exist\n");
		year = fileinfo.fdate >> 9;
		year += 1980;
		month = (fileinfo.fdate >> 5) & 0x0f;
		day = fileinfo.fdate & 0x1f;
		hour = (fileinfo.ftime >> 11) & 0x1f;
		min = (fileinfo.ftime >> 5) & 0x3f;
		sec = (fileinfo.ftime  & 0x1f) * 2;
		
		if(display==1)
		{
			if(fileinfo.fattrib == AM_DIR)		Dprintf("<DIR>");
			else Dprintf("<FILE>");
			Dprintf("%s:[%4d/%2d/%2d %02d:%02d.%02d] %d Byte\n", name, year, month, day, hour, min, sec, fileinfo.fsize);
		}		
		if(fileinfo.fattrib == AM_DIR)		return(2);
		else return(3);
	}
	else
	{
		//if(display==1)
			Dprintf("\t%s 파일을 찾을수 없습니다.\n", name);
		//Dprintf("FileCheck:Error Code=%04x\n", res);		
		//FileErrorDisplay(res);	
		return(0);		
	}	
	//return(0);
}

int FCopy(char *src, char *dest)
{
	FRESULT  fr;
	//FILINFO fileinfo;

    FIL 	fsrc, fdst;	/* File objects */	
	UINT br, bw;         /* File read/write count */
	
    /* Open source file on the drive 1 */
    fr = f_open(&fsrc, src, FA_OPEN_EXISTING | FA_READ);
	if(fr != FR_OK)
	{
		Dprintf("%s 파일은 존재하지 않습니다.\n", src);
		//FileErrorDisplay(fr);	
		return -1;		
	}	
    /* Create destination file on the drive 0 */
    fr = f_open(&fdst, dest, FA_CREATE_ALWAYS | FA_WRITE);
	if(fr != FR_OK)
	{
	    f_close(&fsrc);
		Dprintf("%s 파일을 생성할수 없습니다.\n", dest);
		//FileErrorDisplay(fr);	
		return -1;		
	}
    /* Copy source to destination */
    for (;;) 
	{
        fr = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);  /* Read a chunk of source file */
        if (fr || br == 0) break; /* error or eof */
		//Dprintf("%s::cnt=%d\n", __FUNCTION__, br);
        fr = f_write(&fdst, Disk_Buffer, br, &bw);            /* Write it to the destination file */
        if (fr || bw < br) break; /* error or disk full */
    }
    /* Close open files */
    f_close(&fsrc);
    f_close(&fdst);
	return(1);
}


// 이함수는 디렉토리의 파일명을 조사한후 처리한다.
// path-> 디렉토리 이름
// action-> 해야할 작업 (COPY, DEL,...)
// src_disk ->"1:" , "0:"

//디렉토리의 파일을 모두지울때는 다음과 같이 사용한다.(작동안됨)
//DirScan ( "1:", "Dirname" , NULL,  NULL, FILE_DELETE);

BYTE DirScan_Buffer[4096];   /* File copy buffer */

FRESULT DirScan ( char *src_disk, char* path , char *dst_disk,  char *dst_path, int action)
{
    FRESULT 	res;
	FRESULT  	fr;
    FILINFO 		fno;
    FIL 			fsrc, fdst;
	UINT 		br, bw;	
    DIR 			dir;

	int			total_file=0;
	char			src_file_name[64];
	//char			dst_file_name[64];

    char *fn;   /* This function is assuming non-Unicode cfg. */
    static TCHAR lfn[_MAX_LFN + 1];
    //fno.fname = lfn;
	strcpy(fno.fname,lfn);
    fno.fsize = sizeof lfn;
	

	//Dprintf("DirScan->%s %s %s %s\n", src_disk, path, dst_disk, dst_path);
    res = f_opendir(&dir, path);                                /* Open the directory */
    if (res == FR_OK) 
    {
        int i = strlen(path);
        for (;;) 
        {
            res = f_readdir(&dir, &fno);                        /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;    /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;                   /* Ignore dot entry */
            fn = *fno.fname ? fno.fname : fno.fname;
			
            if (fno.fattrib & AM_DIR) 
            { 
            } 
            else 
            { 
				if(action == FILE_DELETE)
				{
					sprintf(src_file_name,"%s/%s/%s", src_disk, path, fn);
					
					if((res = f_unlink(src_file_name))==FR_OK)
					{
						Dprintf("Delete::OK %s delete\n", src_file_name);
					}	
					else
					{
						Dprintf("%s delete Error\n", src_file_name);
					}
				}
				else if(action==FILE_COPY)
				{
					//sprintf(src_file_name,"%s/%s/%s", src_disk, path, fn);
					//sprintf(dst_file_name,"%s/%s/%s", dst_disk,  dst_path, fn);
					//FCopy(src_file_name, dst_file_name);
					//Dprintf("\t%s copied\n", src_file_name);					
					
					//Dprintf("SRC--->%s\n", src_file_name);
					sprintf(src_file_name,"%s/%s/%s", src_disk, path, fn);
					fr = f_open(&fsrc, src_file_name, FA_OPEN_EXISTING | FA_READ);
					if(fr != FR_OK)
					{
						Dprintf("%s 파일열기에  실패했습니다.\n", src_file_name);
						return FR_INT_ERR;		
					}	
					sprintf(src_file_name,"%s/%s/%s", dst_disk,  dst_path, fn);
					//Dprintf("DST--->%s\n", src_file_name);
					fr = f_open(&fdst, src_file_name, FA_CREATE_ALWAYS | FA_WRITE);
					if(fr != FR_OK)
					{
						f_close(&fsrc);
						Dprintf("%s 파일을 생성할수 없습니다.\n", src_file_name);
						return FR_INT_ERR;		
					}										
					//Dprintf("File Copy Start\n");
					int Loop=0;
					for (;;) 
					{
						fr = f_read(&fsrc, DirScan_Buffer, sizeof DirScan_Buffer, &br); 
						//Dprintf("read=%d", br);
						Loop++;
						if (fr || br == 0) break;
						fr = f_write(&fdst, DirScan_Buffer, br, &bw); 
						if (fr || bw < br) break;
						//Dprintf("wr=%d ", br);
						Loop++;
					}
					Dprintf("\t%s copied\n", fn);
					f_close(&fsrc);			
					f_close(&fdst);						
				}
				total_file++;
            }
        }
		f_closedir(&dir);
		 if(action==FILE_COPY) 	Dprintf("%d files copied\n", total_file);
		 else if(action==FILE_DELETE) 	Dprintf("%d files delete\n", total_file);
    }
	else
	{
		Dprintf("DirScan::Directory Open Error\n");
	}
    return res;
}

//이함수는 디렉토리의 파일명을 지우는 용도로만 사용한다.	
FRESULT scan_files_delete (
    char* path        /* Start node to be scanned (also used as work area) */
)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;

	char	src_file_name[64];
	
    char *fn;   /* This function is assuming non-Unicode cfg. */
#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    //fno.fname = lfn;
	strcpy(fno.fname,lfn);
    fno.fsize = sizeof lfn;
#endif

    res = f_opendir(&dir, path);                                /* Open the directory */
    if (res == FR_OK) 
    {
        int i = strlen(path);
        for (;;) 
        {
            res = f_readdir(&dir, &fno);                        /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;    /* Break on error or end of dir */
            if (fno.fname[0] == '.') continue;                   /* Ignore dot entry */
#if  _USE_LFN
            fn = *fno.fname ? fno.fname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) 
            { 
            } 
            else 
            { 
                Dprintf("%s/DataLog/%s", path, fn);
				sprintf(src_file_name,"1:%s", fn);
				if((res = f_unlink(src_file_name))==FR_OK)
				{
					Dprintf("Delete::OK %s delete\n", src_file_name);
				}
				else
				{
					Dprintf("Delete Error=%d\n", res);
				}
	
				//Remove(src_file_name);
            }
        }
		f_closedir(&dir);
    }
    return res;
}


//parh가 주어지면 주어진 디렉토리의 파일을 모두지운다.
//사용법: DeleteDirFile("DataLog");
FRESULT DeleteDirFile(char *path)
{
    char    	buf[64];
	//char		str[64];
    //DWORD fre_clust, fre_sect;//, tot_sect;
	//FATFS	*fs;
	int 		is_slash=0;		//서브디렉토리를 지정했는지를 검사한다.

    if ( f_mount(&sd_fatfs, "1:", 1) != FR_OK )       { Dprintf("DeleteDirFile::Cannot initialize File System.\n");   return(FR_INT_ERR); }
	else
	{
		//Dprintf("DataLog_Save::SD mount ok\n");
		strcpy(CurrenrDisk, "SD"); strcpy(Disk_Kind, "1:");  
    }		
	if(strlen(path) != 0)	
	{
		sprintf(buf,"%s%s", Disk_Kind,  path);	//디렉토리가 지정이되어있으면
		for(int i=0; i<strlen(path); i++)
		{
			if(path[i] =='/') is_slash++;
		}		
	}
	//디렉토리를 체인지한다.
	if(f_chdir(buf)==FR_OK)
	{
		Dprintf("DeleteDirFile:: Set directory name:%s\n", buf);
			
		//!< 현재디렉토리를 얻고 표시한다.		
		//f_getcwd (str, 128);	
		//Dprintf("DeleteDirFile:: Read Directory:%s<-->%s\n", str, CurrenrDir);
		
		//!< 이미 디렉토리는 체인지 됬으므로 "1:" 만 가지고 간다.
		sprintf(buf,"%s%s", Disk_Kind, "");
		scan_files_delete(buf);			
	}
	else Dprintf("\t%s는 디렉토리이름이 아닙니다.\n", buf);
    /* Unregister work area prior to discard it */
	f_mount(0, Disk_Kind, 0); 	
	return(FR_INT_ERR);
}



//cp src dest
int FileCopy(char *src, char *dest)
{
	FRESULT  fr;
	FILINFO fileinfo;

	char	src_file_name[32];
	char	dst_file_name[32];

	char	src_name[32];
	char	dst_name[32];
    FIL 	fsrc, fdst;	/* File objects */	
	UINT br, bw;         /* File read/write count */
	
	char	src_disk_name[16];
	char	dst_disk_name[16];

	char	src_disk[16];
	char	dst_disk[16];
	
	int src_ret = Words_Separator(src,  src_disk_name, src_file_name, ':' );
	if(src_ret==1) strcat(src_disk_name,":");
	int dst_ret = Words_Separator(dest,  dst_disk_name, dst_file_name, ':' );
	if(dst_ret==1) strcat(dst_disk_name,":");

	if(src_ret==1)	//디스크지정자가 하나라도 있으면
	{
		//Dprintf("FileCopy:: src_ret->1, diskname=%s filename=%s\n", src_disk_name, src_file_name);
		if(!strcmp(src_disk_name, "sd:"))			{ sprintf(src_name,"1:%s", src_file_name); strcpy(src_disk, "1:"); }
		else if(!strcmp(src_disk_name, "usb:")) 	{ sprintf(src_name,"0:%s", src_file_name);  strcpy(src_disk, "0:"); }
		else if(!strcmp(src_disk_name, "ram:"))		{ sprintf(src_name,"2:%s", src_file_name);  strcpy(src_disk, "2:"); }
		else return 0;
	}
	else
	{
		strcpy(src_disk, Disk_Kind);
		sprintf(src_name,"%s%s", Disk_Kind, src_file_name);
	}
	if(dst_ret==1)	//디스크지정자가 하나라도 있으면
	{
		//Dprintf("FileCopy:: dst_ret->1, diskname=%s filename=%s\n", dst_disk_name, dst_file_name);
		if(!strcmp(dst_disk_name, "sd:"))		{ sprintf(dst_name,"1:%s", dst_file_name);  strcpy(dst_disk, "1:"); }
		else if(!strcmp(dst_disk_name, "usb:"))	{ sprintf(dst_name,"0:%s", dst_file_name);   strcpy(dst_disk, "0:"); }
		else if(!strcmp(dst_disk_name, "ram:"))	{ sprintf(dst_name,"2:%s", dst_file_name);   strcpy(dst_disk, "2:"); }
		else return 0;
	}
	else
	{
		strcpy(dst_disk, Disk_Kind);
		sprintf(dst_name,"%s%s", Disk_Kind, dst_file_name);
	}	
	
	//Dprintf("%s %s %s %s\n", src_disk, dst_disk, src_file_name, dst_file_name);
	if(( f_stat(src_name, &fileinfo))==FR_OK)
	{
		if(fileinfo.fattrib == AM_DIR)	 
		{
			//Dprintf("DIR->%s\n", dst_name);
			if(f_mkdir(dst_name)==FR_OK)			
			{
				//복사할 디스크에 디렉토리가 만들어 졌으므로 복사를 시작한다.
				DirScan(src_disk, src_file_name, dst_disk, dst_file_name, FILE_COPY);
				return(1);
			}
			else
			{
				Dprintf("\t복사할 디스크에 %s 디렉토리를 생성할수 없습니다.\n", dst_name);
				return(0);
			}
		}
	}
	//!< 파일이면 정상복사시작
	
    /* Open source file on the drive 1 */
    fr = f_open(&fsrc, src_name, FA_OPEN_EXISTING | FA_READ);
	if(fr != FR_OK)
	{
		Dprintf("%s 파일은 존재하지 않습니다.\n", src_name);
		//FileErrorDisplay(fr);	
		return -1;		
	}	
    /* Create destination file on the drive 0 */
    fr = f_open(&fdst, dst_name, FA_CREATE_ALWAYS | FA_WRITE);
	if(fr != FR_OK)
	{
	    f_close(&fsrc);
		Dprintf("%s 파일을 생성할수 없습니다.\n", dst_name);
		//FileErrorDisplay(fr);	
		return -1;		
	}
    /* Copy source to destination */
    for (;;) 
	{
        fr = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);  /* Read a chunk of source file */
        if (fr || br == 0) break; /* error or eof */
		//Dprintf("%s::cnt=%d\n", __FUNCTION__, br);
        fr = f_write(&fdst, Disk_Buffer, br, &bw);            /* Write it to the destination file */
        if (fr || bw < br) break; /* error or disk full */
    }

    /* Close open files */
    f_close(&fsrc);
    f_close(&fdst);
	//Dprintf("\t1 file (s) copied.\n");
	Dprintf("\t1개 파일이 복사되었습니다.\n");
	return(1);
}
//주어진좌표의 화면에서 비트맵을 생성해서 파일로저장한다.
void ScreenSave(char *filename, int xs, int ys);
int BMPCreateSD(char *sx1, char *sy1, char *width1, char *height1, char *filename) 
{
	__BMPHeader	Header;
	
	FRESULT  fr;
	char	src_file_name[128];
	//U32	address = Graphics_Address;	//현재보이는 화면의 어드레스를 얻는다.
	
	int	startx = atoi(sx1);
	int	starty = atoi(sy1);
	int	sx=0, sy=0;
	int	width = atoi(width1);
	int	height = atoi(height1);
	U32	address = GRAM_BASE;//+LCD_ALPHA_CONTAINER_ADDR;	//현재보이는 화면의 어드레스를 얻는다.
	//U32	address = (1024*768*20);	//프로그램이 다운된다.
	int	offset=0;
	//int	count=0;
	U16	color;
//    FIL fsrc;	/* File objects */	
	UINT br=0, bw=0;         /* File read/write count */	
	
    //Graphics_Address = GRAM_BASE+(width*height*20);
    //g_GPU_addr = GRAM_BASE+(width*height*20);   
	//ScreenFillRect(0, 0, LCD_X, LCD_Y, clRed);	

    
	//!< Bitmap Header를 작성한다.
	Header.Type = 0x4d42;
	Header.FileSize = 0x36 + (width*height*3);
	Header.Reserved = 0;
	Header.Reserved1 = 0;
	Header.PixelOffset = 0x36;
	Header.HeaderSize = 40;
	Header.Width=width;
	Header.Height=height;
	Header.Plane=1;
	Header.BitCount=24;
	Header.Compress = 0x00;
	Header.BitmapSize = (width*height*3);
	Header.X_Res = 0;//2834;
	Header.Y_Res = 0;//2834;	
	Header.ColorMapEntry=0;
	Header.ColorImportant=0;

	if ( f_mount(&sd_fatfs, "1:", 1) != FR_OK )       { Dprintf("BMPCreateSD::Cannot initialize File System.\n");   return(0); }
	else
	{
		//Dprintf("DataLog_Save::SD mount ok\n");
		strcpy(CurrenrDisk, "SD"); strcpy(Disk_Kind, "1:");  
	}		

	sprintf(src_file_name,"%s%s", Disk_Kind, filename);

    //fr = f_open(&fsrc, src_file_name,  FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
    if(f_open(&sd_file,  src_file_name,  FA_CREATE_ALWAYS | FA_WRITE | FA_READ ) != FR_OK)    	
	{
		Dprintf("BMPCreateSD::File Open Error=%s\n", src_file_name);
		FileErrorDisplay(fr);	
		return -1;		
	}	

	//!< 헤더를 저장하기위해 디스크라이트용 버퍼에 복사한다.
	U32	src = (U32)&Header;
	for(int i=0; i<sizeof(__BMPHeader); i++)
	{
		Disk_Buffer[offset] = *(U8 *)src++;
		//Dprintf("%x",Disk_Buffer[offset]);
		offset++;
	}
		
	//!< 마지막라인부터 저장한다.
	for(sy=starty+height-1; sy>= starty; sy--)
	{
		for(sx=startx; sx< startx+width; sx++)
		{
			color = *(U16 *)(address + (((sy * LCD_X) +sx) *2));
			Disk_Buffer[offset++] = (color  << 3) & 0xF8;	//Blue Color
			if(offset == _MAX_SS)
			{
				fr = f_write(&sd_file, (BYTE *)Disk_Buffer, offset, &bw); 
				//count++;
				if (fr || bw < br) 
				{
					Dprintf("BMPCreate::Write Error-->1 br=%d bw=%d\n", br, bw);
					FileErrorDisplay(fr);	
					f_close(&sd_file);
					return(0);
				}			
				offset=0;
			}
			Disk_Buffer[offset++] = (color  >> 3) & 0xFC;		//Green Color
			if(offset == _MAX_SS)
			{
				fr = f_write(&sd_file, (BYTE *)Disk_Buffer, offset, &bw); 
				//count++;
				if (fr || bw < br) 
				{
					Dprintf("BMPCreate::Write Error-->2 br=%d bw=%d\n", br, bw);
					FileErrorDisplay(fr);	
					f_close(&sd_file);
					return(0);
				}			
				offset=0;
			}
			Disk_Buffer[offset++] = (color  >> 8) & 0xF8;		//Red Color
			if(offset == _MAX_SS) //4096
			{
				fr = f_write(&sd_file, (BYTE *)Disk_Buffer, offset, &bw); 
				//count++;
				if (fr || bw < br) 
				{
					Dprintf("BMPCreate::Write Error-->3 br=%d bw=%d\n", br, bw);
					FileErrorDisplay(fr);	
					f_close(&sd_file);
					return(0);
				}			
				offset=0;
			}		
		}
	}
	if(offset !=0)
		fr = f_write(&sd_file, (BYTE *)Disk_Buffer, offset, &bw); 
    f_close(&sd_file);
    /* Unregister work area prior to discard it */
	f_mount(0, Disk_Kind, 0); 	
	Dprintf("SD Screen Capture OK = %s\n", src_file_name);
	return(1);
}
int BmpToLan(char *sx1, char *sy1, char *width1, char *height1, char *filename, int slotNumber)
{
	__BMPHeader	Header;
	//FRESULT  fr;
	//char	src_file_name[128];

	int	startx = atoi(sx1);
	int	starty = atoi(sy1);
	int	sx=0, sy=0;
	int	width = atoi(width1);
	int	height = atoi(height1);
	U32	address = 0x6A280000;//GRAM_BASE+LCD_ALPHA_CONTAINER_ADDR;
	U32 memAddre =0x6B480000;
	//int	i=0;
	int	offset=0;
	U16	color;
	//UINT br=0, bw=0;         /* File read/write count */	
	
	//!< Bitmap Header를 작성한다.
	Header.Type = 0x4d42;
	Header.FileSize = 0x36 + (width*height*3);
	Header.Reserved = 0;
	Header.Reserved1 = 0;
	Header.PixelOffset = 0x36;
	Header.HeaderSize = 40;
	Header.Width=width;
	Header.Height=height;
	Header.Plane=1;
	Header.BitCount=24;
	Header.Compress = 0x00;
	Header.BitmapSize = (width*height*3);
	Header.X_Res = 0;
	Header.Y_Res = 0;	
	Header.ColorMapEntry=0;
	Header.ColorImportant=0;


	int maxSize = 0;
	BYTE bColor = 0;

	//!< 헤더를 저장하기위해 디스크라이트용 버퍼에 복사한다.
	U32	src = (U32)&Header;
	for(int i=0; i<sizeof(__BMPHeader); i++)
	{
		//Disk_Buffer[offset++] = *(U8 *)src++;
		*(U8 *)memAddre++ = *(U8 *)src++;
		offset++;
	}
	//!< 마지막라인부터 저장한다.
	for(sy=starty+height-1; sy>= starty; sy--)
	{
		for(sx=startx; sx< startx+width; sx++)
		{
			color = *(U16 *)(address + (((sy * LCD_X) +sx) *2));
			
			//Disk_Buffer[offset++] = (color  << 3) & 0xF8;	//Blue Color
			bColor = (color  << 3) & 0xF8;	//Blue Color
			if(bColor == 0x00) bColor =0x01;
			
			//Disk_Buffer[offset++] = bColor;
			*(U8 *)memAddre++ = bColor;
			offset++;
			if(offset == 8000)
			{
				memAddre = 0x6B480000;
				send(slotNumber, (unsigned char *)memAddre, offset);
				//maxSize += offset;
				//for(i=0;i<offset;i++)
				//	Dputc(Disk_Buffer[i]);
				offset=0;
			}
			//Disk_Buffer[offset++] = (color  >> 3) & 0xFC;		//Green Color
			bColor = (color  >> 3) & 0xFC;		//Green Color
			if(bColor == 0x00) bColor =0x01;
			
			//Disk_Buffer[offset++] = bColor;
			*(U8 *)memAddre++ = bColor;
			offset++;
			if(offset == 8000)
			{
				memAddre = 0x6B480000;
				send(slotNumber, (unsigned char *)memAddre, offset);
				//maxSize += offset;
				//for(i=0;i<offset;i++)
				//	Dputc(Disk_Buffer[i]);
				offset=0;
			}
			//Disk_Buffer[offset++] = (color  >> 8) & 0xF8;		//Red Color
			bColor = (color  >> 8) & 0xF8;		//Red Color
			if(bColor == 0x00) bColor =0x01;
			
			//Disk_Buffer[offset++] = bColor;
			*(U8 *)memAddre++ = bColor;
			offset++;
			if(offset == 8000) //4096
			{
				memAddre =0x6B480000;
				send(slotNumber, (unsigned char *)memAddre, offset);
				//maxSize += offset;
				//for(i=0;i<offset;i++)
				//	Dputc(Disk_Buffer[i]);
				offset=0;
			}		
		}
	}
	if(offset !=0) {
		send(slotNumber, Disk_Buffer,offset);
		maxSize += offset;
		//Dprintf("\n\n");
		//for(i=0;i<offset;i++)
		//	Dprintf("%02x ",Disk_Buffer[i]);
		//Dprintf("\n\n");
	}
	//Dprintf("IMAGE SIZE = %d\n", maxSize);
	return(1);
}
int BMPCreateUSB(char *sx1, char *sy1, char *width1, char *height1, char *filename, U32 address)
{
	__BMPHeader	Header;
	FRESULT  fr;
	char	src_file_name[128];
	//U32	address = Graphics_Address;	//현재보이는 화면의 어드레스를 얻는다.
	//U32	address = 0x68000000;	//현재보이는 화면의 어드레스를 얻는다.
	int	startx = atoi(sx1);
	int	starty = atoi(sy1);
	int	sx, sy;
	int	width = atoi(width1);
	int	height = atoi(height1);
	
	int	offset=0;
	//int	count=0;
	U16	color;
    //FIL fsrc;	/* File objects */	
	UINT br, bw;         /* File read/write count */	
	
	//!< Bitmap Header를 작성한다.
	Header.Type = 0x4d42;
	Header.FileSize = 0x36 + (width*height*3);
	Header.Reserved = 0;
	Header.Reserved1 = 0;
	Header.PixelOffset = 0x36;
	Header.HeaderSize = 40;
	Header.Width=width;
	Header.Height=height;
	Header.Plane=1;
	Header.BitCount=24;
	Header.Compress = 0x00;
	Header.BitmapSize = (width*height*3);
	Header.X_Res = 2834;
	Header.Y_Res = 2834;	
	Header.ColorMapEntry=0;
	Header.ColorImportant=0;

	if ( f_mount(&usb_fatfs, "0:", 1) != FR_OK )       { Dprintf("BMPCreateUSB::Cannot initialize File System.\n");   return(0); }
	else
	{
		//Dprintf("DataLog_Save::SD mount ok\n");
		strcpy(CurrenrDisk, "USB"); strcpy(Disk_Kind, "0:");  
	}		

	sprintf(src_file_name,"%s%s", Disk_Kind, filename);

    //fr = f_open(&fsrc, src_file_name,  FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
    if(f_open(&usb_file,  src_file_name,  FA_CREATE_ALWAYS | FA_WRITE | FA_READ ) != FR_OK)    	
	{
		Dprintf("BMPCreateUSB::File Open Error=%s\n", src_file_name);
		FileErrorDisplay(fr);	
		return -1;		
	}	

	//!< 헤더를 저장하기위해 디스크라이트용 버퍼에 복사한다.
	U32	src = (U32)&Header;
	for(int i=0; i<sizeof(__BMPHeader); i++)
	{
		Disk_Buffer[offset++] = *(U8 *)src++;
	}
		
	//!< 마지막라인부터 저장한다.
	for(sy=starty+height-1; sy>= starty; sy--)
	{
		for(sx=startx; sx< startx+width; sx++)
		{
			color = *(U16 *)(address + (((sy * LCD_X) +sx) *2));
			Disk_Buffer[offset++] = (color  << 3) & 0xF8;	//Blue Color
			if(offset == _MAX_SS)
			{
				fr = f_write(&usb_file, (BYTE *)Disk_Buffer, offset, &bw); 
				//count++;
				if (fr || bw < br) 
				{
					Dprintf("BMPCreate::Write Error-->1 br=%d bw=%d\n", br, bw);
					FileErrorDisplay(fr);	
					f_close(&usb_file);
					return(0);
				}			
				offset=0;
			}
			Disk_Buffer[offset++] = (color  >> 3) & 0xFC;		//Green Color
			if(offset == _MAX_SS)
			{
				fr = f_write(&usb_file, (BYTE *)Disk_Buffer, offset, &bw); 
				//count++;
				if (fr || bw < br) 
				{
					Dprintf("BMPCreate::Write Error-->2 br=%d bw=%d\n", br, bw);
					FileErrorDisplay(fr);	
					f_close(&usb_file);
					return(0);
				}			
				offset=0;
			}
			Disk_Buffer[offset++] = (color  >> 8) & 0xF8;		//Red Color
			if(offset == _MAX_SS) //4096
			{
				fr = f_write(&usb_file, (BYTE *)Disk_Buffer, offset, &bw); 
				//count++;
				if (fr || bw < br) 
				{
					Dprintf("BMPCreate::Write Error-->3 br=%d bw=%d\n", br, bw);
					FileErrorDisplay(fr);	
					f_close(&usb_file);
					return(0);
				}			
				offset=0;
			}		
		}
	}
	if(offset !=0) {
		fr = f_write(&usb_file, (BYTE *)Disk_Buffer, offset, &bw); 
    }
    f_close(&usb_file);
	f_mount(0, Disk_Kind, 0); 	
	Dprintf("USB Screen Capture OK = %s\n", src_file_name);
	return(1);
}



/**
	@brief 비트맵구조체의 정보를 확인하는 함수이다(개발용)
*/
void BitmapInfoDisplay(__BMPHeader *Header)
{
	Dprintf("Header.Type = %04x\n", Header->Type);
	Dprintf("Header.FileSize=%d\n", 	Header->FileSize);
	Dprintf("Header.PixelOffset =%4x\n", Header->PixelOffset);
	Dprintf("Header.HeaderSize=%d\n",	Header->HeaderSize);
	Dprintf("Header.Width=%d\n",	Header->Width);
	Dprintf("Header.Height=%d\n",	Header->Height);
	Dprintf("Header.Plane=%d\n",	Header->Plane);
	Dprintf("Header.BitCount=%d\n",	Header->BitCount);
	Dprintf("Header.BitmapSize=%d\n",	Header->BitmapSize);
}

int RamToDisk(char *filename, uint32_t addr,  uint32_t size)
{
	FRESULT  	fr;
	char		src_file_name[128];
    FIL  		fdst;	/* File objects */	
	UINT 		bw;   /* File write count */
	uint32_t	address = addr; 
	uint32_t	len =  size;
	uint32_t	cnt=0;
	int 		ret=1;
    int         i=0;

    for(i=0;i<128;i++) src_file_name[i]=0;
    
    SD_Mount();    
	sprintf(src_file_name,"%s%s", Disk_Kind, filename);
    fr = f_open(&fdst, src_file_name, FA_CREATE_ALWAYS | FA_WRITE);
	if(fr != FR_OK)
	{
		Dprintf("RamToDisk:Error Code=%04x  %s \n", fr, src_file_name);
		return 5;		
	}	 
	for(int i=0; i<len; i++)
	{
		Disk_Buffer[cnt++] =  *(__IO U8 *)address++;
		if(cnt==_MAX_SS)
		{
			fr = f_write(&fdst, Disk_Buffer, cnt, &bw);
			if (fr || bw < cnt) { ret = 2; break; }
			cnt=0;
		}
	}
	if(cnt != _MAX_SS)
	{
		fr = f_write(&fdst, Disk_Buffer, cnt, &bw); 
		if (fr || bw < cnt)  
		{
			ret = 2;
		}
	}    
    f_close(&fdst);
	f_mount(0, Disk_Kind, 0); 	
	return(ret);	
}

/**
  * @fn        	CopyToMemory
  * @brief    	이함수는  SD 의 파일을 메모리에 복사한다.
  * @date    	2014년 12월13일  
  * @param   	disk :SD 또는 USB
  * @param   	filename :SD 에 있는 실행파일명
  * @param   	address: 복사할 메모리 주소
  * @retval  	true, false
  * @todo    
  */ 
void SD_Mount(void);
int CopyToMemory(char *filename, U32 address)
{
	FRESULT	fr;
	char		src_file_name[128];
    FIL 		fsrc;
	UINT 		br;
	int			ret= -1;
    int         i=0;
    
    for(i=0;i<128;i++) src_file_name[i]=0;
    
    SD_Mount();
	sprintf(src_file_name,"%s%s", Disk_Kind, filename);
    fr = f_open(&fsrc, src_file_name, FA_OPEN_EXISTING | FA_READ);
	if(fr != FR_OK)
	{
		Dprintf("Err%s->%s\n", __FILE__, __FUNCTION__);
        f_mount(0, Disk_Kind, 0);    
		return (ret);		
	}	
	int file_size=0;
    for (;;) 
	{
        fr = f_read(&fsrc, Disk_Buffer, sizeof Disk_Buffer, &br);
        if (fr || br == 0)  {ret = 0; break; }
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
		ret = 0;
	}
    
    f_close(&fsrc);
    f_mount(0, Disk_Kind, 0);  
	if(ret == -1) return(ret);
	else return(file_size);
}

/**
 *	@brief 구조체를 지정된 디스크에 지정된이름으로 저장하는 함수이다.
 *	@parameter 	disk-> SD, USB, RAM, NAND 를 지정한다.
 *	@parameter	data-> 구조체포인터이다.
 *	@parameter	data_size->구조체의사이즈이다, sizeof(구조체타입)
 *	@parameter	filename->저장할파일명이다.
 *	@retval			
 *						0->disk 구분자 error
 *						1->Good
 *						2->파일기록에러
 *						3->데이터사이즈 에러
 *						4->disk를 찾을수 없다.
 *						5->파일열기에러
 *	@todo   f_mount(SD, &sd_fatfs )	//0.1 F/S 사용시
 *	@todo 	f_mount(SD, NULL); 			//0.09 F/S 사용시
*/
#ifdef PSRAM 
#define	FREE_RAM	0x6475DA00
#else
#define	FREE_RAM	0x6BD80000
#endif
int DataToDisk(void *struct_data,  int data_size, char *filename)
{
	uint32_t	src = (U32)struct_data;
	int ret = RamToDisk(filename, src, data_size);
	return(ret);
}
int DiskToData(void *struct_data,  int data_size, char *filename)
{
	uint32_t	src = (U32)struct_data;
	int	file_size = CopyToMemory(filename, src);
    return(file_size);
}


typedef struct {
	int	no;
	int	SV;
	int	PV;
} DataLog;

#if 0
//!< 구조체를 디스크에 읽고 쓰는 함수를 테스트한 함수이다.
void	MyTestFunc(void)
{
	Dprintf("DiskToData & DataToDisk Test Function\n");
	DataLog		srcData = {0, 10, 20};
	int ret = DataToDisk(&srcData, sizeof(DataLog), "DataSave.bin");
	Dprintf("DataSave Result = %d\n", ret);
	srcData.no = 0;
	srcData.PV = 0;
	srcData.SV = 0;
	ret = DiskToData(&srcData, sizeof(DataLog), "DataSave.bin");
	Dprintf("DataLoad Result = %d\n", ret);
	Dprintf("Data = %d %d %d\n", srcData.no, srcData.SV, srcData.PV);
}
#endif
#ifdef __cplusplus
}
#endif
