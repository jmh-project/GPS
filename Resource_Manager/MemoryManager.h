/**
  ******************************************************************************
  * @file    		MemoryManager.h
  * @author  	(주)자이언텍 기술지원팀
  * @version 	V1.0.0
  * @date    	19-December-2014
  * @brief   	메모리조작관련 부트로더용 함수들
  *          		
  ******************************************************************************
  * @attention
  *
  * 이파일은 자이언텍 지적재산권이므로 소스를 유출해서는 안됩니다.
  * <h2><center>&copy; COPYRIGHT 2014 ZionTech Inc</center></h2>
  ******************************************************************************
  */

#ifndef __MEMORY_MANAGER
#define __MEMORY_MANAGER

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "Teeco_HW_Config.h"
#include "stm32f4xx.h"	 
#include "Teeco_c_Define.h"
/** @addtogroup TEECO_BootLoader
  * @{
  */
/* Exported types ------------------------------------------------------------*/
typedef struct __NandFat
{
	int		Position;
	int		x_size, y_size;
	int		start_blk;
	int		end_blk;
    int     	Size;
    char     	FontKind[4];
} st_NandFAT;
	 
/* Exported variable --------------------------------------------------------*/	 
extern volatile 		U8	FlashPageBuf[2048];	//Nand Flash Memory의 페이지를 조작하기위한 버퍼
extern volatile 		U32	g_BadBlock[100];
extern volatile int	g_BadBlockCount;
extern volatile int 	g_BMP_Dir_ClusterNo;
extern volatile int  	gDMA_USE;
/* Exported constants --------------------------------------------------------*/	 
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/	 
int 	Memory_Fill(char *address, char *size, char *fill_data, char *fill_len);
int 	Memory_Edit(char *address, char *size);
void 	Memory_Dump(char *address, char *size);
int 	Memory_Copy(char *src_addr, char *dest_addr, char *size, char *copy_len, char *opcode);
int 	Memory_Compare(char *src_addr, char *dst_addr, char *size);
void 	File_NandSpace(void);
void 	File_NandSave(void);
void 	Nand_Erase(char *nand_block);
void 	NandDump(char *nand_block, char *nand_page);
int 	SearchNandFile(char *filename, st_NandFAT *Fat);
int	SearchLastAddress(void);
void 	NandCasheSave(void);
void 	NandMBRCashe(void);
/** @addtogroup ResourceManager
  * @{
  */	 
	 
#ifdef __cplusplus
}
#endif
/**
  * @}
  */

/**
  * @}
  */
#endif /* __MEMORY_MANAGER */
/******************* (C) COPYRIGHT 2014 ZionTech *****END OF FILE****/
