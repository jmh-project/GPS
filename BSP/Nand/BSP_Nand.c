/******************************************************************************
*    	File name	: BSP_NAND.c
*		Description : GPU Init Routine
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 06 / 14
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "BSP_Nand.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/
#ifdef PSRAM_USE
#define		NAND_MBR_ADDRESS_RESOURCE	    		0x64000000
#define		SOURCE_BMP_ADDR_RESOURCE				0x64020000
#define		CONV_BMP_ADDR_RESOURCE	    			0x643E0000
#define		DIR_ADDR_RESOURCE			    		0x645A2000	// 2MB Allocation
#endif
// 아래의 설정은 VRAM 을 사용할때 사용한다.
#ifdef VRAM_USE
#define		NAND_MBR_ADDRESS_RESOURCE				0x6B1B2000
#define		SOURCE_BMP_ADDR_RESOURCE		0x6ABF2000
#define		CONV_BMP_ADDR_RESOURCE					0x6AED2000
#define		DIR_ADDR_RESOURCE						0x6B7F2000	// 2MB Allocation
#endif

/******************************************************************************
							Private & Local Variables
******************************************************************************/
extern volatile U32	g_BadBlock[100];
extern volatile int	g_BadBlockCount;
extern volatile U8		FlashPageBuf[2048];	//Nand Flash Memory의 페이지를 조작하기위한 버퍼

/******************************************************************************
							Function Prototype
******************************************************************************/

#define		NAND_CLE		(*(volatile U8 *)0x60010000)	//A16:CLE
#define		NAND_ALE		(*(volatile U8 *)0x60020000)	//A17:ALE
#define		NAND_DATA		(*(volatile U8 *)0x60000000)	

//#define 	NAND_WAITRB()    {	while(!(GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_6 )));} 

void NAND_WAITRB(void)
{	
	int cnt=0;
	while(1)
	{
		if(GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_6 )==0) break;
		cnt++;
		if(cnt > 2000) 
		{
			Dprintf("NAND_WAITRB::Wait Err-1\n");
			break;
		}
	}
	cnt=0;
	while(1)
	{
		if(GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_6 )==1) break;
		cnt++;
		if(cnt > 2000) 
		{
			Dprintf("NAND_WAITRB::Wait Err-2\n");
			break;
		}
	}
		
	//while((GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_6 )));
	//while(!(GPIO_ReadInputDataBit( GPIOG, GPIO_Pin_6 )));
}

void BSP_NandReadID(NAND_IDTypeDef *NandID)
{
	NAND_CLE = 0x90;	
	NAND_ALE = 0x00;	
	NandID->Maker_ID =  NAND_DATA;
	NandID->Device_ID = NAND_DATA;
	NandID->Third_ID =  NAND_DATA;
	NandID->Fourth_ID = NAND_DATA;
}

void BSP_NandReadpage(U16 block, U16 page, U8 *buffer)
{
	//U8	data;
	U16	i;
	U32	blockPage;	
    blockPage=(block<<6) | page;
//Dprintf("blockpage = %08x\n", blockPage);	

	NAND_CLE = 0x00;	

	NAND_ALE = 0x00;
	NAND_ALE = 0x00;
	NAND_ALE = (blockPage&0xff);
	NAND_ALE = ((blockPage>>8)&0xff);
	NAND_ALE = ((blockPage>>16)&0x03);
	NAND_CLE = 0x30;	
	NAND_WAITRB();
	
    for(i=0;i<2048;i++)
    {
	   	*buffer++ = NAND_DATA;	// Read one page
	}
}

U16 BSP_NandWritepage(U16 block, U16 page, U8 *buffer)
{
	U8	data;
	U16	i;
	U32	blockPage;	
	int	loop=0;
	
    blockPage=(block<<6) | page;

	NAND_CLE = 0x80;	

	NAND_ALE = 0x00;
	NAND_ALE = 0x00;
	NAND_ALE = (blockPage&0xff);
	NAND_ALE = ((blockPage>>8)&0xff);
	NAND_ALE = ((blockPage>>16)&0x03);

    for(i=0;i<2048;i++)
    {
	   	NAND_DATA = *buffer++;
	}
	NAND_CLE = 0x10;
	//NAND_WAITRB();
    //여기서 무한루프 걸릴가능성이 있다. 타임아웃과 에어메시지를 출력하게 해야한다.
	while(1)
	{
		loop++;
		NAND_CLE = 0x70;
		data = NAND_DATA;
		if(data & 0x40) break;
		if(loop > 2000)
		{
			Dprintf("\t\t\tBSP_NandWritepage:: Loop Exit\n");
			break;
		}
	}

	NAND_CLE = 0x70;
	data = NAND_DATA;
	if(data & 0x01) 
    {
        Dprintf("BSP_NandWritepage:: [Error]block=%d page=%d\n", block, page);
        return(0);	// Error
    }
	else return(1);
}

U16 BSP_NandEraseBlock(U16 block)
{
	U8	data;
	int	loop=0;

	U32	blockPage = (block<<6);	

	NAND_CLE = 0x60;	

	NAND_ALE = (U8)(blockPage&0xff);
	NAND_ALE = (U8)((blockPage>>8)&0xff);
	NAND_ALE = (U8)((blockPage>>16)&0x03);

	NAND_CLE = 0xD0;

	//NAND_WAITRB();
    //여기서 무한루프 걸릴가능성이 있다. 타임아웃과 에러메시지를 출력하게 해야한다.
	while(1)
	{
		loop++;
		NAND_CLE = 0x70;
		data = NAND_DATA;
		if(data & 0x40) break;
		if(loop > 10000)
		{
			Dprintf("BSP_NandWritepage:: Loop Exit\n");
			break;
		}
		
	}
	NAND_CLE = 0x70;
	data = NAND_DATA;

	if(data & 0x01) 
    {
        Dprintf("BSP_NandEraseBlock:: block=%d\n", block);
        return(0);	// Error
    }
	else 
    {
        return(1);
    }
}

//주어진 블록이 배드블록인지를 검사하는 함수이다.
int IsBadBlock(int block)
{
	int		i;
	
	for(i=0; i<g_BadBlockCount; i++)
	{
		if(g_BadBlock[i]==block) return(1);
	}
	return(0);
}

#ifdef __cplusplus
}
#endif
