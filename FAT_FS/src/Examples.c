/******************************************************************************
    Filename:       Examples.c
    Author:         Song BJ
    Version:        nothing
    Date:           2013 / 05 / 06
    Description:    Chan's File System Test Source
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#define     VERSION_STRING      "20130506_V0.1"
/******************************************************************************
                Include File
******************************************************************************/
#include "TEECO_System.h"
#include "ff.h"

/******************************************************************************
                Private typedef
******************************************************************************/

/******************************************************************************
                Private define
******************************************************************************/

/******************************************************************************
                Private variable
******************************************************************************/
extern  FATFS sd_fatfs;
extern  FIL sd_file;

DWORD acc_size;				/* Work register for fs command */
WORD acc_files, acc_dirs;
FILINFO Finfo;
#if _USE_LFN
char Lfname[512];
#endif

DIR Dir;					/* Directory object */
BYTE Buff[4*1024] __attribute__ ((aligned (4))) ;		/* Working buffer */
UINT Timer;		/* Performance timer (1kHz increment) */


DWORD AccSize;				/* Work register for fs command */
WORD AccFiles, AccDirs;

FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
);

#if 0
static
FRESULT scan_files1 (
	char* path		/* Pointer to the path name working buffer */
)
{
	DIR dirs;
	FRESULT res;
	int i;
	char *fn;
    Dprintf("scan_files1::%s\n", path);

	if ((res = f_opendir(&dirs, path)) == FR_OK) {
		i = strlen(path);
		while (((res = f_readdir(&dirs, &Finfo)) == FR_OK) && Finfo.fname[0]) {
			if (_FS_RPATH && Finfo.fname[0] == '.') continue;
#if _USE_LFN
			fn = *Finfo.lfname ? Finfo.lfname : Finfo.fname;
#else
			fn = Finfo.fname;
#endif
			if (Finfo.fattrib & AM_DIR) {
				AccDirs++;
				path[i] = '/'; strcpy(path+i+1, fn);
				res = scan_files1(path);
				path[i] = '\0';
				if (res != FR_OK) break;
			} 
            else 
            {
                Dprintf("%s/%s\n", path, fn);
				AccFiles++;
				AccSize += Finfo.fsize;
			}
		}
	}

	return res;
}
#endif
#ifdef __cplusplus
}
#endif



