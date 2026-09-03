/******************************************************************************
    file name: NandFileSystem.h
    who: ziontek inc. by Song BJ
    version: 0.1
    create: 2012 02 27
    description: fat32 header file
******************************************************************************/

#ifndef __NANDFAT32HIGH_FILESYSTEM
#define __NANDFAT32HIGH_FILESYSTEM

#ifdef __cplusplus
 extern "C" {
#endif

//주어진 구조체의 데이터를 낸드에 저장한다.
int StructToNandNew(char *filename, void *struct_data, int size);	 
int NandToStruct(char *filename, void *struct_data, int data_size);	 
int StructToNand(char *filename, void *struct_data, int size);

#ifdef __cplusplus
}
#endif
#endif
