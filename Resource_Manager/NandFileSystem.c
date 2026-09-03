/******************************************************************************
*    	File name	: NandFileSystem.c 
*		Description : nand data load Save Program
*		Company		: idnics co.,
*		Version		: 1.00
*		Contact		: www.ziontek.com
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "Memorymanager.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define		N_BLOCK_SIZE			131072

/******************************************************************************
							Private & Local Variables
******************************************************************************/
extern volatile U8		FlashPageBuf[2048];	//Nand Flash Memory의 페이지를 조작하기위한 버퍼

/******************************************************************************
							Function Prototype
******************************************************************************/
//void StructToNand(char *filename, void *struct_data, int size);
//int NandToStruct(char *filename, void *struct_data, int data_size);
int	SearchLastAddress(void);
int IsBadBlock(int block);

typedef struct __TestData
{
	volatile int		a;
	volatile int		b;
	volatile float	f_tes;
	volatile float	f_tes1;
	
	volatile float	f_tes2;
	volatile float	f_tes3;
	volatile float	f_tes4;
	volatile float	f_tes5;
} TestData;


void NandFileTest(void)
{
	int 	size = sizeof(TestData) ;
	TestData	test, verify;
	test.a = 100;
	test.b = 200;
	test.f_tes = 3.14f;
	
    //test 확인하기 위해 값을 지운다.
	verify.a = 0;
	verify.b = 0;
	verify.f_tes = 0.0f;
	
	Dprintf("\nTest:: a=%d b=%d f=%3.1f\n", test.a, test.b, test.f_tes);
	StructToNand("Testdata2.data", &test, size);
	
	NandToStruct("Testdata2.data", &verify, size);
	Dprintf("Test:: a=%d b=%d ftes=%3.1f\n", verify.a, verify.b, verify.f_tes);
}


//주어진 구조체의 데이터를 낸드에 저장한다.
int StructToNandNew(char *filename, void *struct_data, int size)
{
	char	buf[64];
	U32		src;
	int		i, y, page;
	int		need_block=0;			//Font 를 저장하기위해 필요로 하는 불록의 수
	int 	f_s, start_addr;		//nand list 빈공간의 시작주소
	int 	start_block;			//nand 에 저장하기위한 시작불록
	int		blk, mod;
	int		cnt=0;

	U8		*VRAM_FileBuf = ( U8 *)0x6ABF2000;
	U8 		*NandMBR = ( U8 *)NAND_MBR_ADDRESS+2048; //0x6B1B2000

	need_block = need_block;
	src = (U32)struct_data;
//Dprintf("StructToNandNew:: Data Size=%d\n", size);
	
	for(i=0; i < size; i++)
		*VRAM_FileBuf++ = *(U8 *)(src++);	
	
	blk = size / N_BLOCK_SIZE;
	mod = size % N_BLOCK_SIZE;

	if(blk==0) need_block = 1;
	else if(mod >=1)
	{
		need_block = blk +1;
	}
	start_addr = SearchLastAddress();
	
//Dprintf("StructToNandNew:: NandMBR Start Address=%d Need Block=%d \n", start_addr, need_block);
	
	if(start_addr==0) start_block=1;	//처음기록하는것이라면 시작블록은 1 이된다.
	else
	{
		NandMBR = (NandMBR + start_addr) - 64;  //마지막에 기록된 리소스를 찾는다.
		NandMBR += 46;	                        //리소스가 사용한 마지막 블록번호
		buf[0] = *NandMBR++;
		buf[1] = *NandMBR++;
		buf[2] = *NandMBR++;
		buf[3] = *NandMBR++;
		buf[4] = 0;

		start_block = atoi(buf) + 1;
	}

	//첫블록이 배드블록인지 찾아서 스킵하는 코드이다.
	while(1)
	{
		if(!IsBadBlock(start_block)) 
		{
			break;
		}
		else start_block++;
	}
	f_s = start_block;	//저장하기위해 시작블록을 임시저장
	
	//BSP_NandEraseBlock(start_block);
	VRAM_FileBuf = ( U8 *)0x6ABF2000;	//임시영역 3MB
	
	for(i=0; i<need_block; i++)
	{		
		BSP_NandEraseBlock(start_block);
		for(page=0; page <64; page++)
		{
			for(y=0; y<2048; y++)
			{	
				FlashPageBuf[y] = *VRAM_FileBuf++;
				cnt++;
				if(cnt >= size) 
				{
					page = 64;
					break;
				}
			}
			BSP_NandWritepage(start_block, page, ( U8 *)FlashPageBuf);
		}
		start_block++;
		while(1)
		{
			if(!IsBadBlock(start_block)) 
			{
				break;
			}
			else start_block++;
		}
	}
	NandMBR = ( U8 *)(NAND_MBR_ADDRESS+2048+start_addr);
//Dprintf("StructToNandNew::NandMBR = %p\n", NandMBR);
	for(i=0; i<64; i++) buf[i]=0xff;	//Buf Clear
	sprintf(buf,"ND%32s%8d%4d%4d",filename, size,  f_s, start_block-1);
//Dprintf("StructToNand::ND%32s%8d%4d%4d\n",filename, size,  f_s, start_block-1);
	for(i=0; i<64; i++)
	{
		*NandMBR++ = buf[i];
	}
	NandCasheSave(); //FAT 를 업데이트한다.
	return(1);
}  

//주어진 구조체의 데이터를 낸드에 저장한다.
int StructToNand(char *filename, void *struct_data, int size)
{
	char	buf[64];
	U32		src;
	int		i, y, page;
	int		need_block;				//Font 를 저장하기위해 필요로 하는 불록의 수
	int 	f_s=0;					//nand list 빈공간의 시작주소
	int 	start_block=0, end_block=0;			//nand 에 저장하기위한 시작불록
	int		blk=0;
	int		cnt=0;
	st_NandFAT NandFat;

	U8		*VRAM_FileBuf = ( U8 *)0x6BD80000; //( U8 *)0x6ABF2000; 사용자영역이어서 바꿈
	U8 		*NandMBR = ( U8 *)NAND_MBR_ADDRESS+2048; //0x6B1B2000

	f_s = f_s;
	cnt = cnt;
	
	src = (U32)struct_data;
	
	//구조체데이터를 메모리에 옮긴다.
	for(i=0; i < size; i++)
		*VRAM_FileBuf++ = *(U8 *)(src++);	
	
	//기존에 파일이 존재하는지 검사하여 그정보를 가지고 온다.
	if(SearchNandFile(filename, &NandFat))
	{
		start_block = NandFat.start_blk;
		end_block = NandFat.end_blk;
//Dprintf("StructToNand[old]::File Found start_blk=%d end_block=%d\n", start_block, end_block);
	}
	else 
	{
		//신규파일이므로 새롭게 생성한다.
//Dprintf("StructToNand::File not Found=%s New Create\n", filename);
		StructToNandNew(filename, struct_data, size);
		return(1);
	}	

	blk = (end_block - start_block);
	if(blk==0) need_block = 1;
	else need_block = blk;
//Dprintf("StructToNand[old]::need Block=%d\n", need_block);	
	//첫블록이 배드블록인지 찾아서 스킵하는 코드이다.
	while(1)
	{
		if(!IsBadBlock(start_block)) 
		{
			break;
		}
		else start_block++;
	}
	f_s = start_block;	//저장하기위해 시작블록을 임시저장
	
//	BSP_NandEraseBlock(start_block);
	VRAM_FileBuf = ( U8 *)0x6BD80000;	
	
	for(i=0; i<need_block; i++)
	{		
		BSP_NandEraseBlock(start_block);
		for(page=0; page <64; page++)
		{
			for(y=0; y<2048; y++)
			{	
				FlashPageBuf[y] = *VRAM_FileBuf++;
			}
			BSP_NandWritepage(start_block, page, ( U8 *)FlashPageBuf);
		}
		start_block++;
		while(1)
		{
			if(!IsBadBlock(start_block)) 
			{
				break;
			}
			else start_block++;
		}
	}
	//FAT 를 업데이트할 필요가 없다, 기존의 파일정보는 유지하고 데이터만 바꾼것이다.(이전)
    //FAT 를 업데이트해야한다. 프로그래머가 항목을 추가하여 사이즈가 바뀔경우가 있다.
	
    NandMBR = ( U8 *)(NAND_MBR_ADDRESS+2048+NandFat.Position); //이파일의 시작주소 2048은 Page=1이라는 뜻임
//Dprintf("StructToNand::NandMBR = %p Position=%d\n", NandMBR, NandFat.Position);
    
	for(i=0; i<64; i++) buf[i]=0xff;	//Buf Clear
	sprintf(buf,"ND%32s%8d%4d%4d",filename, size,  f_s, start_block-1);
//Dprintf("StructToNand::ND%32s%8d%4d%4d\n",filename, size,  f_s, start_block-1);
	for(i=0; i<64; i++)
	{
		*NandMBR++ = buf[i];
	}
	NandCasheSave(); //FAT 를 업데이트한다.
    
	return(1);
}  

int NandToStruct(char *filename, void *struct_data, int data_size)
{
	U32		src;
	int		start_block;
	int		end_block;
	int		y, i;
	//int		xs; 
	int		page;
	int		size=0;
	int		cnt=0;
	st_NandFAT NandFat;

	U8		*VRAM_FileBuf = (U8 *)0x6BD80000; //( U8 *)0x6ABF2000; 사용자영역이어서 바꿈
//Dprintf("NandToStruct::Data size=%d\n", data_size);

	size= size;
	if(SearchNandFile(filename, &NandFat))
	{
		size = NandFat.x_size;
		start_block = NandFat.start_blk;
		end_block = NandFat.end_blk;
//Dprintf("NandToStruct::File Found size=%d start_blk=%d end_blk=%d\n", size, start_block, end_block);
		for(i=start_block; i<=end_block; i++)
		{
			for(page=0; page <64; page++)
			{
				BSP_NandReadpage(start_block, page, (U8 *)FlashPageBuf);
				for(y=0; y<2048; y++)
				{	
					*VRAM_FileBuf++ = FlashPageBuf[y]; 
					cnt++;
					if(cnt >= data_size)
					{
						page=64;
						break;
					}
				}
			}
			start_block++;
			while(1)
			{
				if(!IsBadBlock(start_block)) 
				{
					break;
				}
				else start_block++;
			}
		}
		
		VRAM_FileBuf = (U8 *)0x6BD80000;
		src = (U32)struct_data;
		for(i=0; i < data_size; i++)
		{
			*(U8 *)(src++) = *VRAM_FileBuf++;
		}		
	}
	else 
	{
		Dprintf("NandToStruct::File not Found=%s\n", filename);
        return(0);
	}
	return(1);	
}

/*
    Dprintf("NandFile System Test\n");
    while(1)
    {
        sd = Dgetch();
        if(sd=='1')
        {
            Dprintf("Struct init\n");
            for(i=0; i<100; i++)
            {
                stFloatTest.Value[i] = (test+=1.0);
            }
            
        }
        else if(sd=='2')
        {
            Dprintf("NandFile Create\n");
            StructToNand("SongTestStruct4.dat", &stFloatTest, sizeof(stTest));
        }
        else if(sd=='3')
        {
            Dprintf("Nand Data Verify\n");
            NandToStruct("SongTestStruct4.dat", &stFloatTest, sizeof(stTest));
            
            for(i=0; i<100; i++)
            {
                Dprintf("Data=%3.1f  ", stFloatTest.Value[i]);
            }
            
        }
    }

*/
#ifdef __cplusplus
}
#endif
