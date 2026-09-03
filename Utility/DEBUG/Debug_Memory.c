/******************************************************************************
*    	File name	: Debug_Memory.c
*		Description : Memory test Code
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 05 / 30
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/


/******************************************************************************
							Private & Local Variables
******************************************************************************/


/******************************************************************************
							Function Prototype
******************************************************************************/

void SAA7113H_Setting(void);
/******************************************************************************
		함수원형	: 	TIM2_IRQHandler();
		설    명	:	음성 기준시간 발생용 타이머 인터럽트 처리루틴
		전달인자 	: 	없음 
		리턴값 		:	Time Base Interrupt 
		수정이력 	:	없음 
		작성일시 	:	12/05/31
		사용레벨 	:	LowLevelDriver, OS, SYSTEM, BOOT, User
		전역변수    :   VRAM 할당영역있음
		Note        :   
******************************************************************************/

void MemoryTest_Help(void)
{
	Dprintf("************************************************\n");
	Dprintf("*             Memory Test Function             *\n");
	Dprintf("************************************************\n");
	Dprintf("R. Read Page Memory\n");
	Dprintf("F. Fill Memory\n");
	Dprintf("E. Edit Memory\n");
	Dprintf("s. 7113 Setting\n");
	Dprintf("c. SM718 VSYNC Read\n");

	Dprintf("ESC. Exit\n");
}

#define		CAPTURE_CONTROL_2		*(volatile U16 *)0x6be90002
#define		CAPTURE_CONTROL_0		*(volatile U16 *)0x6be90000

/******************************************************************************
		함수원형	: 	TIM2_IRQHandler();
		설    명	:	음성 기준시간 발생용 타이머 인터럽트 처리루틴
		전달인자 	: 	없음 
		리턴값 		:	Time Base Interrupt 
		수정이력 	:	없음 
		작성일시 	:	12/05/31
		사용레벨 	:	LowLevelDriver, OS, SYSTEM, BOOT, User
		전역변수    :   VRAM 할당영역있음
		Note        :   
******************************************************************************/

void MemoryTest(void)
{
	U8		ckey;
	U32		addr;
	int		i, y;
	U8		mbuf[32];
	U16		data16;
	U32		len;
	
	//SRAM_Init();		
	MemoryTest_Help();
	while(1)
	{
		ckey = Dstatus();
		if(ckey!=RESET)
		{
			ckey = Dgetch();
			if(ckey=='r' | ckey=='R')
			{
				Dprintf("Input Read Address:");
				addr = DgetIntNum();
				Dprintf("\nRead Dump Address:%x\n", addr);

				while(1)
				{
					for(y=0; y<40; y++)
					{
						for(i=0; i<16; i++)	mbuf[i] = *(U8 *)addr++;	
						Dprintf("0x%08x:%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\t", addr-16, mbuf[0],mbuf[1],mbuf[2],mbuf[3],mbuf[4],mbuf[5],mbuf[6],mbuf[7],mbuf[8],mbuf[9],mbuf[10],mbuf[11],mbuf[12],mbuf[13],mbuf[14],mbuf[15]);
						for(i=0; i<16; i++)
						{
							if(mbuf[i] < 0x20  || mbuf[i] > 0x7f) mbuf[i]='.';
						}						
						Dprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", mbuf[0],mbuf[1],mbuf[2],mbuf[3],mbuf[4],mbuf[5],mbuf[6],mbuf[7],mbuf[8],mbuf[9],mbuf[10],mbuf[11],mbuf[12],mbuf[13],mbuf[14],mbuf[15]);
					}
					ckey = Dgetch();
					if(ckey=='\n' | ckey=='\r') continue;
					else if(ckey==0x1b) break;
				}
			}
			else if(ckey=='F' | ckey=='f')
			{
				Dprintf("Memory Fill Start Address:");
				addr = DgetIntNum();
				Dprintf("\nMemory Fill Data:");
				data16 = DgetIntNum();
				Dprintf("\nMemory Fill Size:");
				len = DgetIntNum();

				for(i=0; i<len; i++)  
				{
					*(U16 *)addr = data16;
					addr+=2;
				}
			}
			else if(ckey=='E' | ckey=='e')
			{
				Dprintf("Memory Edit Address:");
				addr = DgetIntNum();
				Dprintf("\nEdit Data:");
				data16 = DgetIntNum();

				*(U16 *)addr = data16;
			}
			else if(ckey==0x1b)
			{
				break;
			}
			else if(ckey=='s')
			{
#ifdef SAA7113H
				//SAA7113H_Setting();
				SAA7113HTest();
#endif                
			}
			else if(ckey=='c')
			{
				Dprintf("Data=%04x\n", CAPTURE_CONTROL_2);
			}
			MemoryTest_Help();
		}		
	}
}
