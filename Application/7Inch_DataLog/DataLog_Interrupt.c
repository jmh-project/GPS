#include "TEECO_System.h"
#include "DataLog.h"

unsigned short CRCCCITT_RX(unsigned char *data, unsigned int length, unsigned short seed, unsigned short final);

void Flow_Receive( char ascii, int channel );
volatile unsigned int gIntellitectStatusFlag;
volatile float	gOffsetData;
extern volatile unsigned int	gLimitCount;
volatile U8 g_data;

const unsigned char auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 

const unsigned char auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,  
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;
unsigned short crc16(unsigned char *puchMsg, unsigned short usDataLen) 
{ 
	unsigned char uchCRCHi = 0xFF ; 
	unsigned char uchCRCLo = 0xFF ; 
	U32 uIndex ; 
	while (usDataLen--)
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ;  
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex]; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return (uchCRCHi << 8 | uchCRCLo) ; 
}
//1B 5B 32 4A 
extern volatile U32	gDosScreen_id;
void DOS_ScreenDisp(void);
#if defined (YSI_ENABLE)
int ClearDataFlag=0;
int Ysi_ReadData_Dos( char ascii )//¼ö½Å·çÆ¾ USART1, USART2, USART3, USART4, USART5 °øÅëÀ¸·Î »ç¿ëÇÑ´Ù.
{
	//int 	i;
    static int lineClearCount=0,lineClearFlag=0;
    if(ascii ==';')lineClearFlag=1;
    if(lineClearFlag==1)//";1H"¹®ÀÚ¸¦ »èÁ¦ÇÑ´Ù.
    {
        lineClearCount++;
        if(lineClearCount >= 4)
        {
            lineClearCount=0;
            lineClearFlag=0;
        }
        //else Dprintf("%c",ascii);
    }
    if(lineClearCount != 0) return 0;
    
	if(ascii <= 0x1F) //[2J[1;1H---------------Run setup------------µ¥ÀÌÅÍ¸¦ »èÁ¦ÇÏ±â À§ÇØ¼­ÀÌ´Ù.
	{
		if(ascii == 0x0A)
		{
		}
		else if(ascii == 0x0d)
		{
		}
		else 
		{
			//Dprintf("[%02x]",ascii);
			ClearDataFlag=1;
		}
	}
	if(ClearDataFlag==1)
	{
		if(ascii=='H' || ascii==0x08 || ascii==0x31) 
		{
				ClearDataFlag = 0;
				//gv.dosScreenData[gv.dosCnt++]=0x0d;
		}
		else
		{
			//Dprintf("<%02x>",ascii);
		}
	}
	else if( gv.dosScreenFlag==ENABLE )
	{
		if( gv.dosCnt < DOS_SCREEN_BUF_SIZE)//¹®ÀÚ¿­ÀÇ ÃÖ´ë±æÀÌ°¡ 200ÀÌ´Ù.
		{
			if(ascii==0x0a)
			{
				
			}
			else
			{
				gv.dosScreenData[gv.dosCnt++] = ascii;
			}
		}
		else
		{
			gv.dosCnt=0;
		}
	}
	else if( gv.dosScreenFlag==DISABLE )
	{
		if( gv.dosCnt2 < DOS_SCREEN_BUF_SIZE)//¹®ÀÚ¿­ÀÇ ÃÖ´ë±æÀÌ°¡ 200ÀÌ´Ù.
		{
			if(ascii==0x0a)
			{
				
			}
			else
			{
				gv.dosScreenData2[gv.dosCnt2++] = ascii;
			}
		}
		else
		{
			gv.dosCnt2 = 0;
		}
	}
	//Uart_SendByte2(ascii);
    return 0;
}
#else 
int Ysi_ReadData_Dos( char ascii )//¼ö½Å·çÆ¾ USART1, USART2, USART3, USART4, USART5 °øÅëÀ¸·Î »ç¿ëÇÑ´Ù.
{
    if( gv.dosScreenFlag==ENABLE )        
	{
		if( gv.dosCnt < DOS_SCREEN_BUF_SIZE) {
            gv.dosScreenData[gv.dosCnt++] = ascii;
		} 
        else {
            gv.dosScreenFlag=DISABLE;
        }
	}
	else if( gv.dosScreenFlag==DISABLE )
	{
		if( gv.dosCnt2 < DOS_SCREEN_BUF_SIZE) {
            gv.dosScreenData2[gv.dosCnt2++] = ascii;
		} 
        else {
            gv.dosScreenFlag=ENABLE;
        }
	}
    return 0;
}
#endif
int TMSDataReceive( char ascii, int channel )
{
    return (1);
}
void Ysi_Item_Read(int ch, char ascii )
{
	static char	oldData=0;
	if(gv.nPARA_Command_Start==ON)// 1.ON »óÅÂ¿¡¼­ ¾Æ·¡ ¸í·ÉÀÌ ¼ö½Å ´ë±â ÇÑ´Ù.
	{
		if(ascii=='#')					
		{
			gv.nPARA_Command_Start=OFF;
		}
		if((ascii=='a')&&(oldData=='r'))
		{
			gv.nPARA_Command_Start=OFF;
		}
	}
	if(gv.nFlag_RecevieItemListRead == ON)//¼¾¼­ Á¤º¸¸¦ ¼öÁýÇÑ´Ù.
	{
		if((ascii==0x20)||(ascii==0x0D)||(ascii==0x0A)||((ascii>=0x30)&&(ascii<=0x39)))
		{
			if(ascii==0x0A)
			{
				gv.nFlag_Receive_Report_Start=ON;
				gv.nReceiveItemNameBufCount=0;
			}
			else if(gv.nFlag_Receive_Report_Start==ON)//µ¥ÀÌÅÍ ¼ö½Å½ÃÀÛ 
			{
				if(ascii==0x0D)
				{
					gv.nFlag_Receive_Report_Start=OFF;
					gv.nFlag_RecevieItemListRead=OFF;//¼¾¼­ Á¤º¸¸¦ ¼ö½Å¿Ï·áÇÏå¸?
				}
				else
				{
					gv.aItemReceiveBuffer[ch][gv.nReceiveItemNameBufCount++]=ascii;
				}
			}
		}
    }
	oldData = ascii;
}
int Ysi_UartData_ReceiveProcess( char ascii, char ch )
{
    if(gv.nFlag_ReceiveDataProcess[ch] == 1)//µ¥ÀÌÅÍ¸¦ Ã³¸®ÁßÀÏ‹š´Â ¹ö¸°´Ù.
    {
        return 0;
    }
	//³ëÀÌÁî·Î ÆÇ´ÜµÈ µ¥ÀÌÅÍ¸¦ ¼ö½ÅÇÑ °æ¿ì µ¥ÀÌÅÍ¸¦ ¸ðµÎ ¹ö¸°´Ù.
	//Uart_SendByte2(ascii);
	if(ascii == 0x0A || ascii == 0x0D || ascii == 0x20 || ascii == 0x3A || ascii == 0x2E || ascii == 0x2F || ascii == 0x2D)
	{
	}
	else if( ascii >= 0x30 && ascii <= 0x39 )
	{
	}
	else
	{
		//³ëÀÌÁî·Î ÆÇ´Ü
		gv.nReceiveBufferCount[ch]=0; 
		gv.nFlag_ReceiveStart[ch]=OFF;
		gv.nFlag_ReceiveDataProcess[ch]=OFF;

		//Uart_SendByte2(ascii);

		return 0;
	}
	
	if(ascii==0x0A)//µ¥ÀÌÅÍ ¼ö½Å ½ÃÀÛ 
	{
		//Uart_SendByte2('A');
		gv.nFlag_ReceiveStart[ch]	=ON;			//µ¥ÀÌÅÍ ¼ö½Å´ë±â 
		gv.nReceiveBufferCount[ch]	=0;		//µ¥ÀÌÅÍ ¹öÆÛ ÁÖ¼Ò ÃÊ±âÈ­ 
	}
	else if(gv.nFlag_ReceiveStart[ch]==ON)
	{
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++]=ascii;	//µ¥ÀÌÅÍ ¼öÁý 
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]]=NULL;		//¹®ÀÚ¿­ÀÇ ³¡¿¡´Â  NULL À» ²À ³Ö¾î ÁØ´Ù.
		
		if(ascii==0x0D)//µ¥ÀÌÅÍ ¼ö½Å¿Ï·á 
		{		
			//Uart_SendByte2('O');
			gv.nFlag_ReceiveDataProcess[ch]=ON; 		//µ¥ÀÌÅÍ¸¦ ¼ö½Å ¿Ï·á ÇÃ·¹±× 
			gv.nFlag_ReceiveStart[ch]=OFF;		
			gv.ReceiveDataLength[ch]=gv.nReceiveBufferCount[ch];		
			//gv.nReceiveBufferCount[ch]=0;			//µ¥ÀÌÅÍ ¼ö½Å¿Ï·á½Ã¿¡ Ä«¿îÆ®°ªÀ» °¡Áö°í µ¥ÀÌÅÍ ±æÀÌ¸¦ ºñ±³ÇÑ´Ù.
		}
		if( gv.nReceiveBufferCount[ch] >= 399 ) 		//ÀúÀå¿µ¿ªÀÌ ¿À¹ÙÇÏ¸é º¯¼öµéÀ» ÃÊ±âÈ­ 
		{
			//Uart_SendByte2('1');
			gv.nReceiveBufferCount[ch]=0;
			gv.nFlag_ReceiveStart[ch]=OFF;
			gv.nFlag_ReceiveDataProcess[ch]=OFF;			
		}
	}
	else 
	{
		//Uart_SendByte2('2');		
		gv.nReceiveBufferCount[ch]=0; 
		gv.nFlag_ReceiveStart[ch]=OFF;
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
	}
	
	if(gv.nReceiveBufferCount[ch]>=399)
	{
		//Uart_SendByte2('3');
		gv.nFlag_ReceiveStart[ch]=OFF;
		gv.nReceiveBufferCount[ch]=0;
		gv.nFlag_ReceiveDataProcess[ch]=0;
	}                                                                               
	return 0;
}                                                                                                               
#ifdef EXO_ENABLE
void EXO_UartData_ReceiveProcess( unsigned char ascii, char ch ) {         
    unsigned char crcBuf[2], crcData[2];
    unsigned short crc=0;
        
    //gv.nFlag_ReceiveStart[ch]´Â µ¥ÀÌÅÍ ¼Û½Å½Ã¿¡ Å¬¸®¾î µÈ´Ù.
	if(ascii==0x02 && gv.exoOldData[ch] == 0x7E)//µ¥ÀÌÅÍ ¼ö½Å ½ÃÀÛ 
	{
		gv.nFlag_ReceiveStart[ch]=1;//µ¥ÀÌÅÍ ¼ö½Å´ë±â 
		gv.nReceiveBufferCount[ch]=0;//µ¥ÀÌÅÍ ¹öÆÛ ÁÖ¼Ò ÃÊ±âÈ­
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++]=ascii;
		gv.exoRxLength[ch]=0;
        gv.exoBufferCount[ch]=0;
        gv.nFlag_ReceiveDataProcess[ch]=0;
		//Dputc('R');
	}
	else if(gv.nFlag_ReceiveStart[ch]==1)
	{
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++]=ascii;//µ¥ÀÌÅÍ ¼öÁý
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]]=0;	//¹®ÀÚ¿­ÀÇ ³¡¿¡´Â  NULL À» ²À ³Ö¾î ÁØ´Ù.
        
		if(gv.nReceiveBufferCount[ch]==15) { //Length
			gv.exoRxLength[ch] = (gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]-1])<<8;
		}			
		if(gv.nReceiveBufferCount[ch]==16) {
			gv.exoRxLength[ch] = gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]-1];
			gv.exoRxLength[ch] +=gv.nReceiveBufferCount[ch];//Áö±Ý±îÁö µ¥ÀÌÅÍÀÇ ±æÀÌ¸¦ ´õÇÑ´Ù.
			gv.exoRxLength[ch] += 2;//crc16 2byte ±îÁö ±æÀÌ·Î °è»êÇÑ´Ù.
		}
        if( gv.nReceiveBufferCount[ch] >= (USART_BUFFER_MAXSIZE-10) ) {//ÀúÀå¿µ¿ªÀÌ ¿À¹ÙÇÏ¸é º¯¼öµéÀ» ÃÊ±âÈ­ 
            gv.exoRxLength[ch]=0;
			gv.nReceiveBufferCount[ch]=0;
			//gv.nFlag_ReceiveStart[ch]=OFF;
			gv.nFlag_ReceiveDataProcess[ch]=4;//overFlow
		}
		else if(gv.exoRxLength[ch] == gv.nReceiveBufferCount[ch]) {//µ¥ÀÌÅÍ ¼ö½Å¿Ï·á
			crcData[0]=gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]-2];
			crcData[1]=gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]-1]; 
			crc=CRCCCITT_RX(gv.aItemReceiveBuffer[ch],gv.exoRxLength[ch]-2,0xffff,0);
			crcBuf[0] = ((crc & 0xFF00)>>8);
			crcBuf[1] = crc & 0x00FF;
			if(crcBuf[0]==crcData[0] && crcBuf[1]==crcData[1]){
				gv.nFlag_ReceiveDataProcess[ch]=ON;					//µ¥ÀÌÅÍ Á¤»ó¼ö½Å µ¥ÀÌÅÍ Ã³¸®ÇÑ´Ù.
                gv.exoBufferCount[ch]=gv.nReceiveBufferCount[ch];	//¼ö½Åµ¥ÀÌÅÍÀÇ ±æÀÌ¸¦ º°µµ·Î ÀúÀåÇÑ´Ù.
                gv.nFlag_ReceiveStart[ch]=0;
				//Dputc('K');
			}
			else {
				gv.nFlag_ReceiveDataProcess[ch]=3;//crc error
                gv.nFlag_ReceiveDataProcess[ch]=0;
                gv.nReceiveBufferCount[ch]=0;
                gv.exoBufferCount[ch]=0;
				gv.exoRxLength[ch]=0;
                gv.nFlag_ReceiveStart[ch]=0;
			}
			//gv.nFlag_ReceiveStart[ch]=OFF;		
		}
	}
	if(gv.nReceiveBufferCount[ch] >= (USART_BUFFER_MAXSIZE-10))
	{
        gv.exoRxLength[ch]=0;
		//gv.nFlag_ReceiveStart[ch]=OFF;
		gv.nReceiveBufferCount[ch]=0;
		gv.nFlag_ReceiveDataProcess[ch]=5;//overFlow	
        gv.nFlag_ReceiveStart[ch]=0;
	}
	gv.exoOldData[ch] = ascii;
}
#endif
void EXTI9_5ADC_int( void ) {
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line5);
	}	
}

void UART4_IRQHandler(void)//USART 1
{
    char        ascii;
    int         comPort = USART_1;

    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        ascii=USART_ReceiveData(UART4);
        gv.comportConnectionError[USART_1] = 0;     //ÃøÁ¤°ª¿¡ ÀÀ´äÀÌ ÀÖ´Â °æ¿ì Ä¿³Ø¼Ç¿¡·¯Ã³¸® Å¸ÀÌ¸Ó°¡ ÃÊ±âÈ­ µÈ´Ù.		
        if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG) {
            gv.comportConnectionError[comPort]=0;   //Å¸ÀÌ¸Ó°¡ Áõ°¡ÇÏ¸é »óÅÂ ÄÚµå ¿¡·¯¸¦ Ãâ·ÂÇÑ´Ù.	
            Ysi_ReadData_Dos(ascii);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_EXO) {
            #ifdef EXO_ENABLE
            EXO_UartData_ReceiveProcess( ascii, comPort);
            #endif
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_SPECTRO_D) {
            #ifdef SPECTRO_D_ENABLE
            Ulik_Receive( ascii, comPort);            
            #endif
            #if defined(SPECTRO_M_ENABLE) || defined (CTD_ENABLE)
            Ulik_MW_Receive( ascii, comPort);
            #endif
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_ODO) {
            #if defined (YSI_ODD_ENABLE) || defined (YSI_ODOCT_ENABLE)
            odo_ReceiveData(comPort, ascii);
            #endif
        }
		else if(flash.Uart[comPort]->SensorID == SENSOR_PONSEL_EC) {
			#if defined (PONSEL_EC)
			PONSEL_EC_DataReceive(ascii, comPort);
			#endif
		}
		else if(flash.Uart[comPort]->SensorID == SENSOR_YSI) {
			if(gv.nFlag_Sensor_DataRead[comPort]==DOS_MODE) {
				Ysi_ReadData_Dos(ascii);
			}
			else if(gv.nFlag_Sensor_DataRead[comPort]==ITEM_READ_MODE) {
				Ysi_Item_Read(comPort,ascii);//PARA µ¥ÀÌÅÍ ÀúÀå½Ã Ã¤³ÎÀÌ º¯°æµÇ¾î ÀúÀåµÈ´Ù.
			}
			else {
				Ysi_UartData_ReceiveProcess( ascii, comPort );
			}
		}
    }
}
/****************************************************************************************************************************************************************************************

UART1 RX INTERRUPT USART 2

****************************************************************************************************************************************************************************************/
void USART1_IRQHandler(void)	 //USART 2
{
    char	ascii;
    int		comPort = USART_2;

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        ascii=USART_ReceiveData(USART1);
        gv.comportConnectionError[USART_2] = 0;//ÃøÁ¤°ª¿¡ ÀÀ´äÀÌ ÀÖ´Â °æ¿ì Ä¿³Ø¼Ç¿¡·¯Ã³¸® Å¸ÀÌ¸Ó°¡ ÃÊ±âÈ­ µÈ´Ù.		
        if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG) {
            gv.comportConnectionError[comPort]=0;//Å¸ÀÌ¸Ó°¡ Áõ°¡ÇÏ¸é »óÅÂ ÄÚµå ¿¡·¯¸¦ Ãâ·ÂÇÑ´Ù.	
            Ysi_ReadData_Dos(ascii);
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_EXO) {
            #ifdef EXO_ENABLE
            EXO_UartData_ReceiveProcess( ascii, comPort);
            #endif
        }
        #if defined(BATTERY_ENABLE) 
        TrancerReveive(0x01,comPort,ascii);
        #endif
    }
}
/****************************************************************************************************************************************************************************************

UART3 RX INTERRUPT USART 3

****************************************************************************************************************************************************************************************/
void USART3_IRQHandler(void)	 //USART 3
{
    char		ascii;
    int			comPort = USART_3;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        ascii=USART_ReceiveData(USART3);
        gv.comportConnectionError[USART_3] = 0;//ÃøÁ¤°ª¿¡ ÀÀ´äÀÌ ÀÖ´Â °æ¿ì Ä¿³Ø¼Ç¿¡·¯Ã³¸® Å¸ÀÌ¸Ó°¡ ÃÊ±âÈ­ µÈ´Ù.		
        if(gv.nFlag_Sensor_DataRead[comPort]==DOS_MODE)//DOS È­¸é¿¡ µ¥ÀÌÅÍ¸¦ Ãâ·ÂÇÑ´Ù.
        {
            if(flash.Uart[comPort]->SensorID == SENSOR_YSI)
            {
                gv.comportConnectionError[comPort]=0;//Å¸ÀÌ¸Ó°¡ Áõ°¡ÇÏ¸é »óÅÂ ÄÚµå ¿¡·¯¸¦ Ãâ·ÂÇÑ´Ù.	
                Ysi_ReadData_Dos(ascii);
            }
            if(flash.Uart[comPort]->SensorID == SENSOR_DEBUG)
            {
                gv.comportConnectionError[comPort]=0;//Å¸ÀÌ¸Ó°¡ Áõ°¡ÇÏ¸é »óÅÂ ÄÚµå ¿¡·¯¸¦ Ãâ·ÂÇÑ´Ù.	
                Ysi_ReadData_Dos(ascii);
            }
        }
        else if(gv.nFlag_Sensor_DataRead[comPort]==SCREEN_MODE)//µ¥ÀÌÅÍ Ãâ·ÂÈ­¸é ¸ðµå¿¡¼­  
        {
            if(flash.Uart[comPort]->SensorID == SENSOR_YSI )
            {
                gv.comportConnectionError[comPort]=0;//Å¸ÀÌ¸Ó°¡ Áõ°¡ÇÏ¸é »óÅÂ ÄÚµå ¿¡·¯¸¦ Ãâ·ÂÇÑ´Ù.	
                Ysi_UartData_ReceiveProcess( ascii, comPort );
            }
            else if(flash.Uart[comPort]->SensorID == SENSOR_EXO)
            {
                #ifdef EXO_ENABLE
                EXO_UartData_ReceiveProcess( ascii, comPort);
                #endif
            }
        }
        else if(gv.nFlag_Sensor_DataRead[comPort]==ITEM_READ_MODE)//YSI ¼¾¼­ Á¤º¸¸¦ ÀÐ¾î µéÀÌ´Â ¸ðµå¿¡¼­ 
        {
            if(flash.Uart[comPort]->SensorID == SENSOR_YSI )
            {
                Ysi_Item_Read(comPort,ascii);//PARA µ¥ÀÌÅÍ ÀúÀå½Ã Ã¤³ÎÀÌ º¯°æµÇ¾î ÀúÀåµÈ´Ù.
            }
        }
    }
}
/****************************************************************************************************************************************************************************************

    UART6 RX INTERRUPT USART 4

****************************************************************************************************************************************************************************************/
#ifdef KECO_PROTOCOL_ENABLE
extern int KECO_DataReceive( char ascii, int comPort );
#endif
void USART6_IRQHandler(void)	 //USART 4
{
    char		ascii;
    int			comPort = USART_4;
	char		buf=0;
	
	buf=ascii; ascii=buf;
	
    if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) 
    {        
        ascii = USART_ReceiveData(USART6);
        gv.comportConnectionError[USART_4] = 0;//ÃøÁ¤°ª¿¡ ÀÀ´äÀÌ ÀÖ´Â °æ¿ì Ä¿³Ø¼Ç¿¡·¯Ã³¸® Å¸ÀÌ¸Ó°¡ ÃÊ±âÈ­ µÈ´Ù.		
		
        if(flash.Uart[comPort]->SensorID == SENSOR_EXO)
        {
            #ifdef EXO_ENABLE
            EXO_UartData_ReceiveProcess( ascii, comPort);
            #endif
        }
        else if(flash.Uart[comPort]->SensorID == SENSOR_KECO) {
            #ifdef KECO_PROTOCOL_ENABLE
            KECO_DataReceive(ascii, comPort);            
            #endif
		}
		else if(flash.Uart[comPort]->SensorID == SENSOR_TMS) {//Çü½Ä½ÂÀÎ
			#if defined(TYPE_APPROVAL_ENABLE)
			TYPE_APPROVAL_DataReceive(ascii, comPort);
			#endif
        }
        else if(flash.Uart[comPort]->SensorID==SENSOR_MODBUS) {
            #ifdef MODBUS_ENABLE
            ReceiveModbus(ascii,comPort);
            #endif
        }
    }
}
/****************************************************************************************************************************************************************************************

    UART7 RX INTERRUPT IOBOARD COM

****************************************************************************************************************************************************************************************/
void IOBoard_ReceiveProcess( unsigned char ascii, char ch )
{    
    int addCnt=0;
    char dBuf[128];
    
    //Dputc(ascii);
	if(ascii==0x02)
	{
		gv.nFlag_ReceiveStart[ch]=1;
		gv.nReceiveBufferCount[ch]=0;
	}
	else if(gv.nFlag_ReceiveStart[ch]==1)
	{
		gv.aItemReceiveBuffer[ch][gv.nReceiveBufferCount[ch]++]=ascii;//µ¥ÀÌÅÍ ¼öÁý		
		if(ascii==0x03)
		{
            if(gv.aItemReceiveBuffer[ch][addCnt++]=='P') {
                if(gv.aItemReceiveBuffer[ch][addCnt++]=='1') {
                    gv.powerResetMessage=1;
                }
            }
            /**
            *   Á¢Á¡ÀÔ·Â
            */
			if(gv.aItemReceiveBuffer[ch][addCnt++]=='I')//Á¢Á¡ÀÔ·Â
            {                
                flash.connection[2] = gv.aItemReceiveBuffer[ch][addCnt++] - 0x30;
                flash.connection[3] = gv.aItemReceiveBuffer[ch][addCnt++] - 0x30;
                flash.connection[4] = gv.aItemReceiveBuffer[ch][addCnt++] - 0x30;
                flash.connection[5] = gv.aItemReceiveBuffer[ch][addCnt++] - 0x30;
                flash.connection[6] = gv.aItemReceiveBuffer[ch][addCnt++] - 0x30;
                flash.connection[7] = gv.aItemReceiveBuffer[ch][addCnt++] - 0x30;
            }
            else Dprintf("INPUT ERROR [%02x][%02x]\n",gv.aItemReceiveBuffer[ch][0],gv.aItemReceiveBuffer[ch][1]);
            if(gv.aItemReceiveBuffer[ch][addCnt++]=='M')//4~20mA ÀÔ·Â 8 channel
            {
                for(int i=0;i<8;i++)
                {
                    dBuf[0]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[1]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[2]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[3]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[4]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[5]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[6]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[7]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[8]=gv.aItemReceiveBuffer[ch][addCnt++];
                    dBuf[9]=gv.aItemReceiveBuffer[ch][addCnt++];
                    flash.Input[i]->mV = atof(dBuf);
                }
            }
            else Dprintf("INPUT420 ERROR\n");
            if(flash.Uart[USART_4]->SensorID == SENSOR_DEBUG) {//USART_4 °¡ DEBUG ÀÌ¸é 5678¸ðµÎ µð¹ö±× ¸ðµå°¡ µÈ´Ù.
                if(gv.aItemReceiveBuffer[ch][addCnt++]=='D') {
                    dBuf[0]='D';
                    for(int i=1; i<128; i++) {//ÃÖ´ë¼ö½Åµ¥ÀÌÅÍ 128°³
                        dBuf[i]=gv.aItemReceiveBuffer[ch][addCnt++];
                        if(dBuf[i]==0x03) {
                            dBuf[i]=0;
                            break;
                        }
                    }
                    DebugMessageOut(dBuf);
                } else {
                    //Dprintf("DEBUG ERROR\n");
                }
            }
            gv.nFlag_ReceiveStart[ch]=0;//µ¥ÀÌÅÍ ¼ö½Å´ë±â 
            gv.nReceiveBufferCount[ch]=0;//µ¥ÀÌÅÍ ¹öÆÛ ÁÖ¼Ò ÃÊ±âÈ­
		}			
		if( gv.nReceiveBufferCount[ch] >= 399 ) //ÀúÀå¿µ¿ªÀÌ ¿À¹ÙÇÏ¸é º¯¼öµéÀ» ÃÊ±âÈ­ 
		{
			gv.nReceiveBufferCount[ch]=0;
			gv.nFlag_ReceiveStart[ch]=OFF;
			gv.nFlag_ReceiveDataProcess[ch]=OFF;
		}
	}
	else 
	{
		gv.nReceiveBufferCount[ch]=0; 
		gv.nFlag_ReceiveStart[ch]=OFF;
		gv.nFlag_ReceiveDataProcess[ch]=OFF;
	}
	if(gv.nReceiveBufferCount[ch]>=399)
	{
		gv.nFlag_ReceiveStart[ch]=OFF;
		gv.nReceiveBufferCount[ch]=0;
		gv.nFlag_ReceiveDataProcess[ch]=0;
	}
}
void UART7_IRQHandler(void)	 //IOBOARD ¿Í Åë½ÅÇÑ´Ù.
{
    char ascii=0;
	if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)
	{
		ascii=USART_ReceiveData(UART7);
        IOBoard_ReceiveProcess(ascii, USART_5);
    }
}
