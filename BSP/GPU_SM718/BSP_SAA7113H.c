/******************************************************************************
*    	File name	: BSP_SAA7113H.c
*		Description : Video Decoder Init Routine
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 06 / 08
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
#define SAA7113H
#ifdef SAA7113H
/******************************************************************************
							Constant & Macros
******************************************************************************/
//#define		SDA			GPIO_Pin_9
//#define		SCL			GPIO_Pin_6
//
//#define		SCL_High()	GPIOB->BSRRL = SCL
//#define		SCL_Low()	GPIOB->BSRRH = SCL
//#define		SDA_High()	GPIOB->BSRRL = SDA
//#define		SDA_Low()	GPIOB->BSRRH = SDA

/******************************************************************************
							Private & Local Variables
******************************************************************************/


/******************************************************************************
							Function Prototype
******************************************************************************/
void BSP_I2C_PortInit(void);
void EEPROMWrite( U16 addr, U8 data);
U8 EEPROMRead( U16 addr );

void VDPWrite( U16 addr, U8 data)
{
    I2C_Start(); 
    I2C_Clock_Data(0x4a);
    I2C_ACK();
    I2C_Clock_Data(addr);
    I2C_ACK();                      
    I2C_Clock_Data(data);           
	I2C_ACK();                      
    I2C_Stop();                     
}

U8 VDPRead( U8 addr )
{
    u8 data=0x00;

    I2C_Start();
    I2C_Clock_Data(0x4a);           
    I2C_ACK();                      
    I2C_Clock_Data(addr);
    I2C_ACK();                      
	I2C_Start();
    I2C_Clock_Data(0x4b);           
    I2C_ACK();                      
    I2CSDAPort_Input();

	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x80;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x40;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x20;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x10;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x08;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x04;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x02;
	I2C_Clk();
	if(GPIO_ReadInputDataBit(GPIOB,SDA)) data |= 0x01;
	I2C_Clk();

    I2C_Stop();
    return ( data );
}

#define SAA7113H_NR_REGISTER	99

static U8 init_regs[SAA7113H_NR_REGISTER] =
{
  /* Register 0x00: chip version (read only)			       */
  /* Default: 0x11: (not used since read-only register)		       */
  0x00,

  /* Register 0x01: increment delay				       */
  /* Default: 0x08: IDEL=0b1000					       */
  0x08,

  /* Register 0x02: analog input control 1			       */
  /* Default: 0xC3: FUSE1, FUSE0				       */
  0xC3,

  /* Register 0x03: analog input control 2			       */
  /* Default: 0x33: VBSL, WPOFF, GAI28, GAI18			       */
  0x33,

  /* Register 0x04: analog input control 3			       */
  /* Default: 0x00						       */
  0, //0x00,

  /* Register 0x05: analog input control 4			       */
  /* Default: 0x00						       */
  0, //0x00,

  /* Register 0x06: horizontal sync start			       */
  /* Default: 0xE9: HSB=0b11101001				       */
  0xE9,

  /* Register 0x07: horizontal sync stop			       */
  /* Default: 0x0D: HSS=0b00001101				       */
  0x0D, //0xE0,

  /* Register 0x08: sync control				       */
  /* Default: 0x98: AUFD, HTC1, HTC0				       */
  /* FUCK: originla default: 0x98, */
  0x98,

  /* Register 0x09: luminance control				       */
  /* Default: 0x01: APER0					       */
  0x01,

  /* Register 0x0A: luminance brightness			       */
  /* Default: 0x80: BRIG=0b10000000				       */
  0x80,

  /* Register 0x0B: luminance contrast				       */
  /* Default: 0x47: CONT=0b01000111				       */
  0x47,

  /* Register 0x0C: chrominance saturation			       */
  /* Default: 0x40: SATN=0b01000000				       */
  0x40,

  /* Register 0x0D: chrominance hue control			       */
  /* Default: 0x00: HUEC=0x00000000				       */
  0x00,

  /* Register 0x0E: chrominance control				       */
  /* Default: 0x01: CHBW0					       */
  /* 0x01, changed NTSC-M -> NTSC-N */
  0x01,

  /* Register 0x0F: chrominance gain control			       */
  /* Default: 0x2a: CGAIN=0b0101010				       */
  0x2a, /* FUCK read: 0x0f */

  /* Register 0x10: format/delay control			       */
  /* Default: 0x00: YDEL=0b000					       */
  0x00,

  /* Register 0x11: output control 1*/
  /* Default: 0x0C: OEYC					       */
  /* 0x0C, original default value */
  0x0c,

  /* Register 0x12: output control 2				       */
  /* Default: 0x01: RTSE0=0b0001				       */
  /* 0x01, original default value */
  0x01,

  /* Register 0x13: output control 3				       */
  /* Default: 0x00: AOSL=0b00					       */
  0x00,

  /* Register 0x14: reserved					       */
  0x00,

  /* Register 0x15: VGATE start					       */
  /* Default: 0x00: VSTAlo=0b00000000				       */
  0x00,

  /* Register 0x16: VGATE stop					       */
  /* Default: 0x00: VSTOlo=0b00000000				       */
  0x00,

  /* Register 0x17: MSBs for VGATE control			       */
  /* Default: 0x00: VSTO8=0b0 VSTA8=0b0				       */
  0x00,

  /* Register 0x18: reserved					       */
  0x00,

  /* Register 0x19: reserved					       */
  0x00,

  /* Register 0x1A: reserved					       */
  0x00,

  /* Register 0x1B: reserved					       */
  0x00,

  /* Register 0x1C: reserved					       */
  0x00,

  /* Register 0x1D: reserved					       */
  0x00,

  /* Register 0x1E: reserved					       */
  0x00,

  /* Register 0x1F: decoder status byte				       */
  /* Default: 0x00: (read-only)					       */
  0x00,

  /* Register 0x20: reserved					       */
  0x00,

  /* Register 0x21: reserved					       */
  0x00,

  /* Register 0x22: reserved					       */
  0x00,

  /* Register 0x23: reserved					       */
  0x00,

  /* Register 0x24: reserved					       */
  0x00,

  /* Register 0x25: reserved					       */
  0x00,

  /* Register 0x26: reserved					       */
  0x00,

  /* Register 0x27: reserved					       */
  0x00,

  /* Register 0x28: reserved					       */
  0x00,

  /* Register 0x29: reserved					       */
  0x00,

  /* Register 0x2A: reserved					       */
  0x00,

  /* Register 0x2B: reserved					       */
  0x00,

  /* Register 0x2C: reserved					       */
  0x00,

  /* Register 0x2D: reserved					       */
  0x00,

  /* Register 0x2E: reserved					       */
  0x00,

  /* Register 0x2F: reserved					       */
  0x00,

  /* Register 0x30: reserved					       */
  0x00,

  /* Register 0x31: reserved					       */
  0x00,

  /* Register 0x32: reserved					       */
  0x00,

  /* Register 0x33: reserved					       */
  0x00,

  /* Register 0x34: reserved					       */
  0x00,

  /* Register 0x35: reserved					       */
  0x00,

  /* Register 0x36: reserved					       */
  0x00,

  /* Register 0x37: reserved					       */
  0x00,

  /* Register 0x38: reserved					       */
  0x00,

  /* Register 0x39: reserved					       */
  0x00,

  /* Register 0x3A: reserved					       */
  0x00,

  /* Register 0x3B: reserved					       */
  0x00,

  /* Register 0x3C: reserved					       */
  0x00,

  /* Register 0x3D: reserved					       */
  0x00,

  /* Register 0x3E: reserved					       */
  0x00,

  /* Register 0x3F: reserved					       */
  0x00,

  /* Register 0x40: slicer control 1				       */
  /* Default: 0x02: FCE ???					       */
  0x02,

  /* Register 0x41: line control register 2			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x42: line control register 3			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x43: line control register 4			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x44: line control register 5			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x45: line control register 6			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x46: line control register 7			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x47: line control register 8			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x48: line control register 9			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x49: line control register 10			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x4A: line control register 11			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x4B: line control register 12			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x4C: line control register 13			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x4D: line control register 14			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x4E: line control register 15			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x4F: line control register 16			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x50: line control register 17			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x51: line control register 18			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x52: line control register 19			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x53: line control register 20			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x54: line control register 21			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x55: line control register 22			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x56: line control register 23			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x57: line control register 24			       */
  /* Default: 0xff:   TO BE ADJUSTED				       */
  0xff,

  /* Register 0x58: programmable framing code			       */
  /* Default: 0x00: FC=0b00000000				       */
  0x00,

  /* Register 0x59: horizontal offset for slicer		       */
  /* Default: 0x54: HOFFlo=0b01010100 (see reg 0x5B)		       */
  0x54,

  /* Register 0x5A: vertical offset for slicer			       */
  /* Default: 0x07: VOFFlo=0b00000111 (see reg 0x5B)		       */
  0x07,

  /* Register 0x5B: held offset and MSBs for horiz. and vert. offset   */
  /* Default: 0x83: FOFF, HOFFhi= 0b011				       */
  0x83,

  /* Register 0x5C: reserved					       */
  0x00,

  /* Register 0x5D: reserved					       */
  0x00,

  /* Register 0x5E: sliced data identification code		       */
  /* Default: 0x00: SDID=0b000000				       */
  0x00,

  /* Register 0x5F: reserved					       */
  0x00,

  /* Register 0x60: slicer status byte 1			       */
  /* Default: 0x00: (unused: read-only)				       */
  0x00,

  /* Register 0x61: slicer status byte 2			       */
  /* Default: 0x00: (unused: read-only)				       */
  0x00, /* FUCK read: 0x0e */

  /* Register 0x62: slicer status byte 3			       */
  /* Default: 0x00: (unused: read-only)				       */
  0x00, /* FUCK read: 0x0f */
};

void SAA7113H_Setting(void)
{
	U8		num;
//	U8		data;

	//BSP_I2C_PortInit(); //BSP_Init()에서 초기화 했음


	for(num=0; num<SAA7113H_NR_REGISTER; num++)
	{
		VDPWrite(num, init_regs[num]);
		//WriteSAA7111(0x48, num, init_regs[num]);
	} 
}

void SAA7113HTest_Help(void)
{
	Dprintf("***********************************************\n\r");
	Dprintf("              SAA7113H TEST\n\r");
	Dprintf("***********************************************\n\r");
	Dprintf("1.EEPROM Write\n");
	Dprintf("2.EEPROM Read\n");
	Dprintf("r.SAA7113H Read\n");
	Dprintf("w.SAA7113H Write\n");
	Dprintf("s.SAA7113H Setting\n"); 
	Dprintf("c. BMP SD Copy\n");
}
extern int gHValue, gLValue, gHClp, gLClp;
int BMPCreateSD(char *sx1, char *sy1, char *width1, char *height1, char *filename) ;
extern char CurrenrDir[64];
extern char CurrenrDisk[16];
extern char Disk_Kind[16];
extern BYTE Disk_Buffer[MEMORY_BUF_SIZE] ;
extern int gCaptureX, gCaptureY;

int CopyRectVideo(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height, uint32_t addr)
{
	uint32_t x = 0;
	uint32_t y = 0;
	uint32_t ScreenPage=addr;
	
	//uint32_t ScreenPage2=0x68000000;
	
	uint32_t SrcAddr = 0;
	//uint32_t SrcAddr2 = 0;
	
	uint16_t (*VRAM_FileBuf)[1024];
	
	VRAM_FileBuf = (uint16_t(*)[1024])(LCD_ALPHA_CONTAINER_ADDR + 0x68000000);
	
	for(y=sy; y<=height; y++) 
	{
		SrcAddr = ScreenPage + (((( sy + y ) * 1024 ) + sx ) * 2 );
		
		//SrcAddr2 = ScreenPage2 + (((( sy + y ) * 1024 ) + sx ) * 2 );
		
		for(x=sx; x<=width; x++)
		{
			*(uint16_t *) (SrcAddr) = VRAM_FileBuf[y][x];
			SrcAddr += 2; 

			//*(uint16_t *) (SrcAddr2) = VRAM_FileBuf[y][x];
			//SrcAddr2 += 2; 
		}		
	}
	return 0; 
}
int Memory_Bitblt(char *src_addr, char *dest_addr,  char *xlen,  char *ylen )
{
	U32		src =  StrToHex(src_addr);
	U32		dest = StrToHex(dest_addr);
	U32		width = StrToHex(xlen);
	U32		height = StrToHex(ylen);
    
    U32     src_first = src;
	U16		src_data;
	
    int		    x, y;

    for(y=0; y<height; y++)
    {
//Dprintf("No=%d SRC=%08x DEST=%08x width=%d height=%d\n", y, src, dest, width, height);    
        src = (y * ( 1024 * 2 )) + src_first;
        dest = (y * ( 1024 * 2 )) + 0x68000000;

        for(x=0; x<width; x++)
        {
            src_data = *(__IO U16 *)src;
            *(__IO U16 *)dest = src_data;
//Dprintf("No=%d SRC=%08x DEST=%08x data=%04x\n", x, src, dest, src_data);    
            src+=2; dest+=2;
        }
    }     
	return (0);
}
void SM718_VideoInit(void);
void SM718_CaptureInit(void);
void SAA7113HTest(void)
{
	U8		ckey;
	U32		addr;
	U8		data;
	
	Dprintf("SAA7113 TEST\n");
	BSP_I2C_PortInit(); //BSP_Init()에서 초기화 했음
	SAA7113HTest_Help();
	while(1)
	{
		ckey = Dgetch();
		if(ckey=='1')
		{
			Dprintf("EEPROM Write Address:");
			addr = DgetIntNum();
			Dprintf("\n\rData:");
			data = DgetIntNum();
			EEPROMWrite((U16)addr, data);
		}
		else if(ckey=='2')
		{
			Dprintf("EEPROM Read Address:");
			addr = DgetIntNum();
			data = EEPROMRead((U16)addr);
			Dprintf("\n\rRead Data:%x\n", data);
		}
		else if(ckey=='w')
		{
			Dprintf("7113 Write Address:");
			addr = DgetIntNum();
			Dprintf("\n\rData:");
			data = DgetIntNum();
			VDPWrite((U8)addr, data);
		}
		else if(ckey=='r')
		{
			Dprintf("7113 Read Address:");
			addr = DgetIntNum();
			data = VDPRead((U8)addr);
			Dprintf("\n\rRead Data:%x\n", data);
		}
		else if(ckey=='s')
		{
            Dprintf("SAA7113H_Setting\n\r");
			SAA7113H_Setting();
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(0);
		}
		else if(ckey=='3')
		{
			Dprintf("gHValue=%d\n\r",gHValue);
			gHValue++;
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(1);
		}
		else if(ckey=='4')
		{
			Dprintf("gHValue=%d\n\r",gHValue);
			gHValue--;
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(1);
		}
		else if(ckey=='5')
		{
			Dprintf("gLValue=%d\n\r",gLValue);
			gLValue++;
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(1);
		}
		else if(ckey=='6')
		{
			Dprintf("gLValue=%d\n\r",gLValue);
			gLValue--;
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(1);
		}
        else if(ckey=='7')
        {
			//gHClp=0, gLClp
            Dprintf("gCaptureX = %d\n\r",gHClp);
            gHClp++;
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(1);
        }
        else if(ckey=='8')
        {
            Dprintf("gCaptureX = %d\n\r",gHClp);
            gHClp--;
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(1);
        }
        else if(ckey=='9')
        {
            Dprintf("gCaptureY = %d\n\r",gLClp);
            gLClp++;
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(1);
        }
        else if(ckey=='0')
        {
            Dprintf("gCaptureY = %d\n\r",gLClp);
            gLClp--;
			SM718_VideoInit();
			SM718_CaptureInit();
			VDPWrite(0x02, 0xC3);//TUNNER
			VideoScreen(1);
        }
        else if(ckey=='c')
        {
            //strcpy(Disk_Kind,"1:");	//USB Select
			Dprintf("BMP CREATE SD START\n");
            //BMPCreateSD("0", "10", "640", "480", "WaterImage.bmp");

			//USER_SCREEN5_ADDR			
			VideoScreen(0);		
			//BMPCreateSD("0", "10", "640", "480", "WaterImage.bmp");	
			BmpToLan("0", "10", "640", "480", "WaterImage.bmp",0);	
			Dprintf("BMP CREATE SD END\n");
        }
		else if(ckey=='d')
		{
			VideoScreen(0);		
			Dprintf("CopyRectVideo START\n");
			CopyRectVideo( 0, 0, 1024, 664,0x68000000);
			Dprintf("CopyRectVideo END\n");
		}
		else if(ckey==0x1b)
		{
			Dprintf("SDCARD READING\n");
			SDtoLanTrandmit();
		}
	}
}

#endif //SAA7113H
#ifdef __cplusplus
}
#endif
