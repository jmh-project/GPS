/**
  ******************************************************************************
  * @file    		TouchCalibration.h
  * @author  	(주)자이언텍 기술지원팀
  * @version 	V1.0.0
  * @date    	19-December-2014
  * @brief   	Disk operating System
  *          		
  ******************************************************************************
  * @attention
  *
  * 이파일은 자이언텍 지적재산권이므로 소스를 유출해서는 안됩니다.
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc</center></h2>
  ******************************************************************************
  */

#ifndef __DOS_HEADER
#define __DOS_HEADER

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "Teeco_HW_Config.h"
#include "stm32f4xx.h"	 
#include "BSP_DS1302.h"	 
#include "Teeco_C_define.h"	 
#include "ff.h"
#include "DataLog.h"
/** @addtogroup TEECO_BootLoader
  * @{
  */
/** @addtogroup ResourceManager
  * @{
  */	 
/* Exported types ------------------------------------------------------------*/
#define	BLOCK_SIZE	512
#define	FILE_COPY		1
#define	FILE_DELETE	2
#define	FILE_LIST		3
	 
typedef struct __bitmapHeader
{
	uint16_t		Type;			//!< BM 이라는 문자저장 = 0x4d42
	uint32_t		FileSize;		//!< 파일의 전체사이즈
	uint16_t		Reserved;	//!< 사용하지 않는다.
	uint16_t		Reserved1;	//!< 사용하지 않는다.
	uint32_t		PixelOffset;	//!< 실질데이터의 시작주소를 Offset 으로 표시한다.(0x36)
	//!<
	uint32_t		HeaderSize;		//!< 헤더사이즈(최소 40Byte)
	uint32_t		Width;		//!< 그림데이터의 가로사이즈
	uint32_t		Height;		//!< 그림데이터의 세로사이즈
	uint16_t		Plane;		//!< 1로 설정한다.
	uint16_t		BitCount;	//!< 한픽셀당 비트수
	uint32_t		Compress;		//!< 압축방식
	uint32_t		BitmapSize;	//!< BMP Data만의 사이즈
	uint32_t		X_Res;		//!< 미터당 픽셀수 X
	uint32_t		Y_Res;	//!< 미터당 픽셀수 Y
	uint32_t		ColorMapEntry;	//!< 실질적으로사용될 컬러맵의 엔트리수
	uint32_t		ColorImportant;	//!< 주로사용되는 컬러수	
} __attribute__((packed)) __BMPHeader;
	 
/* Exported variable --------------------------------------------------------*/	
extern FATFS usb_fatfs;
extern FIL usb_file;

extern FATFS sd_fatfs;
extern FIL sd_file;

extern FATFS ram_fatfs;
extern FIL ram_file;

extern FATFS fatfs;
extern FIL file;
extern char CurrenrDir[64];
extern char CurrenrDisk[16];
extern char Disk_Kind[16];
extern BYTE Disk_Buffer[MEMORY_BUF_SIZE];   /* File copy buffer */	 
/* Exported constants --------------------------------------------------------*/	 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/	 
int 	Dir_files_Process(char* path, int func, int display);
int 	Dir_files(char* path, int func, int display);
FRESULT DirScan ( char *src_disk, char* path , char *dst_disk,  char *dst_path, int action);

FRESULT Dir_List(char *path);
FRESULT Dir_List1(void);
FRESULT ChangeDir(char *path);
FRESULT FileView(char *filename);
int 	GetLabel(void);
int 	Format(int disk);
int 	SetLabel(char *label);
int 	Rename(char *src, char *dst);
int 	Remove(char *name);
int 	CheckFile(char *name);
FRESULT Set_TimeStamp (
    char *obj,     /* Pointer to the file name */
    char *year1,
    char *month1,
    char *day1,
    char *hour1,
    char *min1,
    char *sec1
);
FRESULT scan_files (
    char* path        /* Start node to be scanned (also used as work area) */
);
void 	SD_Mount(void);
void 	RAM_Mount(void);
void 	RAM_UnMount(void);
int 	USB_Mount(void);
void 	USB_UnMount(void);
int 	Disk_Mount(char *disk);
int 	Disk_UnMount(char *disk);
int 	GetDirectory(char *cur_dir);
int 	DiskSpace(void);
int 	DiskCopy(char *srcdisk, char *src, char *dstdisk, char *dest);
int 	FileCopy(char *src, char *dest);
void 	FileErrorDisplay(FRESULT res);
//int 	BMPCreate(char *sx1, char *sy1, char *width1, char *height1, char *filename);	
int 	BMPCreateSD(char *sx1, char *sy1, char *width1, char *height1, char *filename);
int BMPCreateUSB(char *sx1, char *sy1, char *width1, char *height1, char *filename, U32 address);
void 	BitmapInfoDisplay(__BMPHeader *Header);
int 	DataToDisk(void *struct_data,  int data_size, char *filename);
int 	DiskToData(void *struct_data,  int data_size, char *filename);
int 	CopyToMemory(char *filename, U32 address);
int 	FileCheck(const char *disk, char *name, int display);
int 	MakeDir(char *disk, char *name);

#ifdef __cplusplus
}
#endif
/**
  * @}
  */

/**
  * @}
  */
#endif /* __DOS_HEADER */
/******************* (C) COPYRIGHT 2014 ZionTech *****END OF FILE****/
