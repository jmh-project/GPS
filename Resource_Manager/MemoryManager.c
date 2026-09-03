/**
  ******************************************************************************
  * @file       	MemoryManager.c
  * @author  	(주)자이언텍 기술개발지원팀
  * @version 	V0.1
  * @date    	2014년 12월19일
  * @brief    	이파일은  발효기콘트롤러로 사용할 메인화면프로그램이다.
                
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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "MemoryManager.h"
#include "TEECO_Define.h"
#include "TEECO_C_Define.h"
#include "String.h"
#include "dos.h"
#include "Debug_Uart.h"
#include "bsp_nand.h"

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile 		U8	FlashPageBuf[2048];	//Nand Flash Memory의 페이지를 조작하기위한 버퍼
volatile 		U32	g_BadBlock[100];
volatile int	g_BadBlockCount;
volatile int 	g_BMP_Dir_ClusterNo;
volatile int  gDMA_USE=0;   //하드웨어 설정에서 DMA를 사용하면 리소스매니져에서는 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @fn        	Memory_Dump
  * @brief    	이함수는  이윈도우 위젯의 키이벤트를 받기위해 사용한다.
  * @date    	2014년 12월1일  
  * @param    TEECO Windows standard Parameter
  * 		@arg parent 여기서는 데스크탑윈도우가아니고 버튼의 부모를 가지고온다.
  * @retval  	none
  * @todo    
  */
void Memory_Dump(char *address, char *size)
{
	int		addr = StrToHex(address);
	U8		bbuf[32];
	U16		wbuf[32];
	U32		dbuf[32];
	int		i, y;
	U8		ckey;

    Dprintf("\n");
	if(addr == -1) 	Dprintf("Dump Addr input error\n");
	else
	{
		while(1)
		{
			for(y=0; y<40; y++)
			{
				if(*size=='b')
				{
					for(i=0; i<16; i++)	bbuf[i] = *(__IO U8 *)addr++;	
					Dprintf("0x%08x:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\t", addr-16, bbuf[0],bbuf[1],bbuf[2],bbuf[3],bbuf[4],bbuf[5],bbuf[6],bbuf[7],bbuf[8],bbuf[9],bbuf[10],bbuf[11],bbuf[12],bbuf[13],bbuf[14],bbuf[15]);
					for(i=0; i<16; i++)
					{
						if(bbuf[i] < 0x20  || bbuf[i] > 0x7f) bbuf[i]='.';
					}						
					Dprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", bbuf[0],bbuf[1],bbuf[2],bbuf[3],bbuf[4],bbuf[5],bbuf[6],bbuf[7],bbuf[8],bbuf[9],bbuf[10],bbuf[11],bbuf[12],bbuf[13],bbuf[14],bbuf[15]);
				}
				else if(*size=='w')
				{
					for(i=0; i<8; i++)	{ wbuf[i] = *(__IO U16 *)addr; addr+=2; }
					Dprintf("0x%08x:%04x %04x %04x %04x %04x %04x %04x %04x\t", addr-16, wbuf[0],wbuf[1],wbuf[2],wbuf[3],wbuf[4],wbuf[5],wbuf[6],wbuf[7]);
					Dprintf("........\n");
				}
				else if(*size=='d')
				{
					for(i=0; i<4; i++)	{ dbuf[i] = *(__IO U32 *)addr;	addr+=4; }
					Dprintf("0x%08x:%08x %08x %08x %08x\t", addr-16, dbuf[0],dbuf[1],dbuf[2],dbuf[3]);
					Dprintf("........\n");
				}
			}
			ckey = Dgetch();
			if(ckey=='\n' | ckey=='\r') continue;
			else if(ckey==0x1b) break;
		}
	}
}

int Memory_Fill(char *address, char *size, char *fill_data, char *fill_len)
{
	U32		addr = StrToHex(address);
	U32		data = StrToHex(fill_data);
	U32		len =  StrToHex(fill_len);
	int		y;

	for(y=0; y<len; y++)
	{
		if(*size=='b')
		{
			*(__IO U8 *)addr++ = (U8)data;
		}
		else if(*size=='w')
		{
			*(__IO U16 *)addr = (U16)data;
			addr+=2;
		}
		else if(*size=='d')
		{
			*(__IO U32 *)addr = (U32)data;
			addr+=4;
		}
	}
	return(1);
}

int Memory_Compare(char *src_addr, char *dst_addr, char *size)
{
	uint32_t		src = StrToHex(src_addr);
	uint32_t		dst = StrToHex(dst_addr);
	uint32_t		len =  atoi(size);	
	
	for(int i=0; i<len; i++)
	{
		uint8_t	s_data = *(__IO U8 *)src++;
		uint8_t	d_data = *(__IO U8 *)dst++;
		if(s_data != d_data)
		{
			Dprintf("Compare Err: 0x%08p[%02x] 0x%08p[%02x]\n", src-1, s_data, dst-1, d_data);
		}
		if(Dstatus())
		{
			if(Dgetch()==0x1b) break;
		}
	}
	return(1);
}

volatile static int test=0;
int Memory_Edit(char *address, char *size)
{
	U32		addr = StrToHex(address);
	int		data32;
	int		data16;
	int		data8;
	int		DONE=0;

	if(*size == 'b' || *size == 'w' || *size == 'd'){ }
	else  	{ Dprintf("Edit Size input error\n"); return 0; } 	
   	
	while(!DONE)
	{
		if(*size=='b')	
		{
			data8 = *(__IO U8 *)addr;
			Dprintf("ADDR:0x%p[%02x]=", addr, data8);
			data8 = DgetIntNum();
			if(data8 == -1) break;
			*(__IO U8 *)addr = data8;
			addr++;
		}
		else if(*size=='w')	
		{
			data16 = *(__IO U16 *)addr;
			Dprintf("ADDR:0x%p[%04x]=", addr, data16);
			data16 = DgetIntNum();
			if(data16 == -1) break;
			*(__IO U16 *)addr = data16;
			addr+=2;
		}
		else if(*size=='d')	
		{
			data32 = *(__IO U32 *)addr;
			Dprintf("ADDR:0x%p[%08x]=", addr, data32);
			data32 = DgetIntNum();
			if(data32 == -1) break;
			*(__IO U32 *)addr = data32;
			addr+=4;
		}
	}
	return(1);
}

int Memory_Copy(char *src_addr, char *dest_addr, char *size, char *copy_len, char *opcode)
{
	U32		src =  StrToHex(src_addr);
	U32		dest = StrToHex(dest_addr);
	U32		len = StrToHex(copy_len);
	U32		src_data, dest_data;
	int		i;

	if(*size=='b')	
	{
		for(i=0; i<len; i++)
		{
			src_data = *(__IO U8 *)src;
			dest_data = *(__IO U8 *)dest;
			if(!strcmp(opcode, "and"))
			{
				src_data = (U8)src_data & (U8)dest_data;
				*(__IO U8 *)dest = (U8)src_data;
				src++; dest++;
			}
			else if(!strcmp(opcode, "or"))
			{
				src_data = (U8)src_data | (U8)dest_data;
				*(__IO U8 *)dest = (U8)src_data;
				src++; dest++;
			}
			else if(!strcmp(opcode, "xor"))
			{
				src_data = (U8)src_data ^ (U8)dest_data;
				*(__IO U8 *)dest = (U8)src_data;
				src++; dest++;
			}
			else if(!strcmp(opcode, "not"))
			{
				src_data = ~(U8)src_data;
				*(__IO U8 *)dest = (U8)src_data;
				src++; dest++;
			}
			else if(!strcmp(opcode, "over"))
			{
				*(__IO U8 *)dest = (U8)src_data;
				src++; dest++;
			}
			else
				Dprintf("opcode input error\n");
		}
		return 1;				
	}
	else if(*size=='w')
	{
		for(i=0; i<len; i++)
		{
			src_data = *(__IO U16 *)src;
			dest_data = *(__IO U16 *)dest;
			if(!strcmp(opcode, "and"))
			{
				src_data = (U16)src_data & (U16)dest_data;
				*(__IO U16 *)dest = (U16)src_data;
				src+=2; dest+=2;
			}
			else if(!strcmp(opcode, "or"))
			{
				src_data = (U16)src_data | (U16)dest_data;
				*(__IO U16 *)dest = (U16)src_data;
				src+=2; dest+=2;
			}
			else if(!strcmp(opcode, "xor"))
			{
				src_data = (U16)src_data ^ (U16)dest_data;
				*(__IO U16 *)dest = (U16)src_data;
				src+=2; dest+=2;
			}
			else if(!strcmp(opcode, "not"))
			{
				src_data = ~(U16)src_data;
				*(__IO U16 *)dest = (U16)src_data;
				src+=2; dest+=2;
			}
			else if(!strcmp(opcode, "over"))
			{
				*(__IO U16 *)dest = (U16)src_data;
				src+=2; dest+=2;
			}
			else
				Dprintf("opcode input error\n");
		}
		return 1;				
	}
	else if(*size=='d')
	{
		for(i=0; i<len; i++)
		{
			src_data = *(__IO U32 *)src;
			dest_data = *(__IO U32 *)dest;
			if(!strcmp(opcode, "and"))
			{
				src_data = (U32)src_data & (U32)dest_data;
				*(__IO U32 *)dest = (U32)src_data;
				src+=4; dest+=4;
			}
			else if(!strcmp(opcode, "or"))
			{
				src_data = (U32)src_data | (U32)dest_data;
				*(__IO U32 *)dest = (U32)src_data;
				src+=4; dest+=4;
			}
			else if(!strcmp(opcode, "xor"))
			{
				src_data = (U32)src_data ^ (U32)dest_data;
				*(__IO U32 *)dest = (U32)src_data;
				src+=4; dest+=4;
			}
			else if(!strcmp(opcode, "not"))
			{
				src_data = ~(U32)src_data;
				*(__IO U32 *)dest = (U32)src_data;
				src+=4; dest+=4;
			}
			else if(!strcmp(opcode, "over"))
			{
				*(__IO U32 *)dest = (U32)src_data;
				src+=4; dest+=4;
			}
			else
				Dprintf("opcode input error\n");
		}
		return 1;				
	}
	return(0);
}

void NandDump(char *nand_block, char *nand_page)
{
	U16		block = StrToHex(nand_block);
	U16		page = StrToHex(nand_page);
	U8		ckey;
	int		cnt=0;
	int		i, y;
	U8		mbuf[16];
	U32		addr=0;

	BSP_NandReadpage(block, page, (U8 *)FlashPageBuf);
	cnt = 0;
	while(1)
	{
		for(y=0; y<128; y++)
		{
			for(i=0; i<16; i++)	mbuf[i] = FlashPageBuf[cnt++];	
			Dprintf("B=%4d P=%2d 0x%04x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\t", block, page, addr, mbuf[0],mbuf[1],mbuf[2],mbuf[3],mbuf[4],mbuf[5],mbuf[6],mbuf[7],mbuf[8],mbuf[9],mbuf[10],mbuf[11],mbuf[12],mbuf[13],mbuf[14],mbuf[15]);
			for(i=0; i<16; i++)
			{
				if(mbuf[i] < 0x20  || mbuf[i] > 0x7f) mbuf[i]='.';
			}						
			Dprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", mbuf[0],mbuf[1],mbuf[2],mbuf[3],mbuf[4],mbuf[5],mbuf[6],mbuf[7],mbuf[8],mbuf[9],mbuf[10],mbuf[11],mbuf[12],mbuf[13],mbuf[14],mbuf[15]);
			addr += 16;
		}
		page++;
		if(page==64) { block++; page=0;}
		addr=0;
		cnt=0;
		BSP_NandReadpage(block, page, (U8 *)FlashPageBuf);
		ckey = Dgetch();
		if(ckey=='\n' | ckey=='\r') continue;
		else if(ckey==0x1b) break;	
	}
}

void Nand_Erase(char *nand_block)
{
	int		block;
	int		i;

	if(!strcmp((const char *)nand_block, "all"))
	{
		for(i=0; i<4096; i++)
		{
			if(!BSP_NandEraseBlock(i))	
			{
				Dprintf("%d Block Erase Error[BAD BLOCK]\n", i);
			}
		}
		Dprintf("Nand All Erase Complete\n");
	}
	else
	{
		block = StrToHex(nand_block);
		if(!BSP_NandEraseBlock(block))
		{
			Dprintf("%d Block Erase Error\n", block);
		}
		else 
		{
			Dprintf("%d Block Erase OK\n", block);
		}
	}
}

//Nand에 파일을 저장하고 나면 데이터는 저장이 되지만 0번블록의 빈번한 쓰기를 막기위해
//미리 0블록을 메모리에 캐쉬해서 사용하고 있다. 
//저장이 끝나면 이함수를 불러서 FAT를 갱신해야한다.
void File_NandSave(void)
{
	NandCasheSave();
}

void File_NandSpace(void)
{
	char	buf[64];
	int		last_addr;
	int 	end_block;
	
	U8 		*NandMBR = (U8 *)NAND_MBR_ADDRESS_RESOURCE+2048; //0x6B1B2000
	
	last_addr = SearchLastAddress();

	NandMBR = (NandMBR + last_addr) - 64;
	NandMBR += 46;	//BMP End Block 
	buf[0] = *NandMBR++;
	buf[1] = *NandMBR++;
	buf[2] = *NandMBR++;
	buf[3] = *NandMBR++;
	buf[4] = 0;
	end_block = atoi(buf) + 1;

	IntToStringWon( ((4096-end_block) * 131072), buf, sizeof(buf)  ) ;
	Dprintf("\tnand spaceblock=%d  %s byte available\n", 4096-end_block, buf);
}

//Nand 의 0블록을 VRAM에 복사한다. 속도개선을 위해 캐시하는작업
void NandMBRCashe(void)
{
	char	buf[10];
	int		i;
	U8 		*NandMBR = ( U8 *)NAND_MBR_ADDRESS_RESOURCE;
	
	for(i=0; i<64; i++)
	{
		BSP_NandReadpage(0, i, NandMBR);
		NandMBR += 2048;
	}
	//Dprintf("Szie=%08x\n", NandMBR);
	NandMBR = (U8 *)NAND_MBR_ADDRESS_RESOURCE;
	buf[0] = *NandMBR++;
	buf[1] = *NandMBR++;
	buf[2] = 0;
	g_BadBlockCount = atoi(buf);
	
	for(i=0; i<g_BadBlockCount;i++)
	{
		buf[0] = *NandMBR++;
		buf[1] = *NandMBR++;
		buf[2] = *NandMBR++;
		buf[3] = *NandMBR++;
		buf[4] = 0;	
		g_BadBlock[i] = atoi(buf);
		//Dprintf("Bad Block=%d\n",g_BadBlock[i]); 
	}
}

//조작된 메모리의 Nand Cashe 를 Nand의 0블록에 저장하는 루틴
void NandCasheSave(void)
{
	int		y, page=0;
	U8		*VRAM_FileBuf = (U8 *)NAND_MBR_ADDRESS_RESOURCE;
	
	BSP_NandEraseBlock(0);
	for(page=0; page<64; page++)
	{
		for(y=0; y<2048; y++)
		{	
			FlashPageBuf[y] = *VRAM_FileBuf++;
		}
		BSP_NandWritepage(0, page, (U8 *)FlashPageBuf);
	}
}

//Nand MBR 에서 비어있는 주소를 찾는다.
int	SearchLastAddress(void)
{
	int		i=0;
	U8		buf[64];
	U8 		*NandMBR = (U8 *)(NAND_MBR_ADDRESS_RESOURCE+2048); // Page 1부터 시작
	
	for(;;)
	{
		buf[0] = *NandMBR++;
		buf[1] = *NandMBR++;
		if(buf[0]==0xff && buf[1]==0xff) 
		{
			//Dprintf("Start Address=%d\n", i);
			return(i);
		}
		else
		{
			//Dprintf("%02x %02x %p\n", buf[0], buf[1], NandMBR);
		}
		NandMBR+=62;
		i+=64;
		if(i>=131072) break;
	}
	return(0);	
}

//이름이 주어지면 NandFAT 에서 일치하는 파일이름을 찾은후 정보를 리턴한다.
//찾는 이름이 없으면 0 을 리턴한다.
int SearchNandFile(char *filename, st_NandFAT *Fat)
{
	U8		*VRAM_NandFAT = (U8 *)(NAND_MBR_ADDRESS_RESOURCE+2048); //2048부터 페이지 1이고, 여기부터 FAT시작영역이다.
	char	name_buf[33];
	char	filename_buf[33];
	char	buf[10];
	int		i, y, last_addr;
    
	//Nand MBR 에서 비어있는 주소를 찾는다. 검색할 마지막 어드레스를 지정한다.
    //buf[0], buf[1] 은 NandFile의 종류이다. 이내용이 ff 일때 리스트의 끝이므로 그것을 찾는다.
	last_addr =	SearchLastAddress();
	sprintf(filename_buf,"%32s", filename);

	for(i=0; i < last_addr; i+=64)
	{
		VRAM_NandFAT = (U8 *)(NAND_MBR_ADDRESS_RESOURCE+2048);
		buf[0] = *(VRAM_NandFAT+i);
		buf[1] = *(VRAM_NandFAT+i+1);
		buf[2] = 0;

		for(y=0; y<32; y++)  
        { 
            name_buf[y] = *(VRAM_NandFAT+i+2);  
            VRAM_NandFAT++; 
        }

		name_buf[y] = 0;
		//Dprintf("SearchNandFile::kind=%s srcname=%s comparename=%s\n", 	buf, filename_buf, name_buf);	
		if(!strcmp(name_buf, filename_buf ))
		{
			if (!strcmp(buf, "NH" )) //"NF%32s%4d%4d%4d%4d%6d"
			{
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->x_size = atoi(buf);			
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->y_size = atoi(buf);			
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->start_blk = atoi(buf);			
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->end_blk = atoi(buf);
				Fat->Position = i;	
                
                //한글의경우는 사이즈를 가지고 간다.
                for(y=0; y<8; y++)
                {
                    buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  
                } 
                buf[y]=0;
                
                Fat->Size = atoi(buf);			
                
				//Dprintf("SearchNandFile::Hangul-> Name=%s xs=%d ys=%d start=%d end=%d Size=%d\n", filename, Fat->x_size, Fat->y_size, Fat->start_blk, Fat->end_blk, Fat->Size);
				return(1);
			}
			else if (!strcmp(buf, "NB" ) || !strcmp(buf, "NF" ) || !strcmp(buf, "NA" ) || !strcmp(buf, "NU" ) ||!strcmp(buf, "NE" ) )
			{
				strcpy(Fat->FontKind, buf);

				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->x_size = atoi(buf);	
				//test = atoi(buf);	
				//Dprintf("Fname=%s Bmp XS=%d ADDR=%08x %02x %02x %02x %02x %02x\n", filename, test, VRAM_NandFAT+i+2, buf[0], buf[1],buf[2],buf[3],buf[4]);                
				
                for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->y_size = atoi(buf);			
				//test = atoi(buf);			
				//Dprintf("Fname=%s Bmp YS=%d ADDR=%08x %02x %02x %02x %02x %02x\n", filename, test, VRAM_NandFAT+i+2, buf[0], buf[1],buf[2],buf[3],buf[4]);                
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->start_blk = atoi(buf);			
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->end_blk = atoi(buf);
				Fat->Position = i;	
                
                //MBR 의 처음에 기록된 사이즈가 손상이 되었을경우 두번째 기재된 값을 사용한다.
                //이코드는 폰트는 적용되지 않는다.
                if(Fat->x_size == 0 || Fat->y_size == 0)
                {
                    for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
                    Fat->x_size = atoi(buf);			
                    for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
                    Fat->y_size = atoi(buf);			                    
                }
                for(y=0; y<8; y++)
                {
                    buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  
                } 
                buf[y]=0;
                
                Fat->Size = atoi(buf);			
				
				//Dprintf("SearchNandFile::Name=%s xs=%d ys=%d start=%d end=%d\n", filename, Fat->x_size, Fat->y_size, Fat->start_blk, Fat->end_blk);
				return(1);
			}
			else if(!strcmp(buf, "NS" ))
			{
				for(y=0; y<7; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->x_size = atoi(buf);			
				for(y=0; y<5; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->y_size = atoi(buf);			
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->start_blk = atoi(buf);			
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } buf[y]=0;
				Fat->end_blk = atoi(buf);
				Fat->Position = i;	
				return(1);		
			}
			else if(!strcmp(buf, "ND" ))
			{
				for(y=0; y<8; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } 
                buf[y]=0;
				Fat->x_size = atoi(buf);			
				
                for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } 
                buf[y]=0;				
                Fat->start_blk = atoi(buf);			
                
				for(y=0; y<4; y++)  { buf[y] = *(VRAM_NandFAT+i+2); VRAM_NandFAT++;  } 
                buf[y]=0;
				Fat->end_blk = atoi(buf);
                
				Fat->Position = i;	
				//Dprintf("SearchNandFile::Name=%s size=%d start_blk=%d end_blk=%d Position=%d\n", filename, Fat->x_size, Fat->start_blk, Fat->end_blk, i);
				return(1);		
			}
		}
	}
	return(0);
}
#ifdef __cplusplus
}
#endif

