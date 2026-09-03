/******************************************************************************
*    	File name	: Daol_Lcd.c 
*		Description : TEECO Project Entry File
*		Company		: idnics co.,
*		Version		: 1.00
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

#include "usbh_core.h"
#include "usbh_usr.h"
#include "usbh_msc_core.h"
#include "HD44780.h"

#define		LCD_RS_H	GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define		LCD_RS_L		GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define		LCD_RW_H	GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define		LCD_RW_L	GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define		LCD_EN_H	GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define		LCD_EN_L	GPIO_ResetBits(GPIOA, GPIO_Pin_4)
void CLCD_Delay( unsigned int cnt )
{
	
    //Delay(cnt);
    cnt *= 1500;
	while( cnt-- );
}

void DataPort( unsigned short int  port )
{
	unsigned short int output=0;
	
	if( port & 0x01 ) 	GPIO_SetBits(GPIOG, GPIO_Pin_0);
	else					GPIO_ResetBits(GPIOG, GPIO_Pin_0);

	if( port & 0x02 ) 	GPIO_SetBits(GPIOG, GPIO_Pin_1);
	else					GPIO_ResetBits(GPIOG, GPIO_Pin_1);

	if( port & 0x04 ) 	GPIO_SetBits(GPIOG, GPIO_Pin_2);
	else					GPIO_ResetBits(GPIOG, GPIO_Pin_2);

	if( port & 0x08 ) 	GPIO_SetBits(GPIOG, GPIO_Pin_3);
	else					GPIO_ResetBits(GPIOG, GPIO_Pin_3);


	if( port & 0x10 ) 	GPIO_SetBits(GPIOG, GPIO_Pin_4);
	else					GPIO_ResetBits(GPIOG, GPIO_Pin_4);

	if( port & 0x20 )	GPIO_SetBits(GPIOG, GPIO_Pin_5);
	else					GPIO_ResetBits(GPIOG, GPIO_Pin_5);

	if( port & 0x40 ) 	GPIO_SetBits(GPIOG, GPIO_Pin_6);
	else					GPIO_ResetBits(GPIOG, GPIO_Pin_6);

	if( port & 0x80 ) 	GPIO_SetBits(GPIOG, GPIO_Pin_7);
	else					GPIO_ResetBits(GPIOG, GPIO_Pin_7);


}

void BusyCheck( void )
{
	CLCD_Delay(10);
}


void Write_Cmd( unsigned char font )
{
	BusyCheck();
	LCD_RS_L;
	LCD_RW_L;
	DataPort( font );
	
	LCD_EN_L;
	CLCD_Delay(5);
	LCD_EN_H;
	CLCD_Delay(5);
}

void Write_Data( unsigned char font )
{
	BusyCheck();
	LCD_RS_H;
	LCD_RW_L;
	DataPort( font );
	LCD_EN_L;
	CLCD_Delay(5);
	LCD_EN_H;
	CLCD_Delay(5);
}
void Write_String(char *str)
{
	int i=0;
    for(i=0;i<4;i++)
    {
        Write_Data( *(str+i) );
    }
}
void LCD_Init( void )
{
	int	i=0;

	LCD_RS_L;
	LCD_RW_L;
	LCD_EN_L;

    Delay(10);
	DataPort( HD44780_CMD_RESET );
    Delay(10);
	DataPort( HD44780_CMD_RESET );
    Delay(10);
	DataPort( HD44780_CMD_RESET );
    Delay(10);

	LCD_EN_L;
	CLCD_Delay(5);
	LCD_EN_H;
	CLCD_Delay(5);
	
	CLCD_Delay(10);
	Write_Cmd( HD44780_CMD_FUNCTION | HD44780_FUNC_BUS_8BIT | HD44780_FUNC_LINES_2 | HD44780_FUNC_FONT_5x8);//8bit, 2line, 5x7dot
	CLCD_Delay(41);
	Write_Cmd( HD44780_CMD_ENTRY_MODE | HD44780_ENTRY_ADDR_INC | HD44780_ENTRY_SHIFT_CURS);
	CLCD_Delay(41);
	Write_Cmd( HD44780_CMD_DISPLAY | HD44780_DISP_ON | HD44780_DISP_CURS_OFF | HD44780_DISP_BLINK_OFF);
	CLCD_Delay(41);
	Write_Cmd( HD44780_CMD_SHIFT | HD44780_SHIFT_RIGHT | HD44780_SHIFT_CURSOR);
	CLCD_Delay(41);
	Write_Cmd( HD44780_CMD_CLEAR );
	CLCD_Delay(41);
	Write_Cmd( HD44780_CMD_RETURN_HOME );
	CLCD_Delay(41);
}

void LCD_Help( void )
{
	Dprintf("\n---------------- LCD ---------------\n");
	Dprintf("1. Init\n");
	Dprintf("2. LINE1 WRITE\n");
	Dprintf("3. LINE2 WRITE\n");

}
void LCD_Display( char *str1, char *str2, char *str3, char *str4 )
{    
    //Dprintf("%s",str1);
    Write_String(str1);
    //Write_String(str3);
    //Write_String(str2);                                                                  
   // Write_String(str4);    
}
void HD44780_GotoXY(unsigned char x, unsigned char y)
{
    unsigned char copy_y=0;

    if(x > (20-1))
        x = 0;

    if(y > (4-1))
        y = 0;

    switch(y)
    {
        case 0:  copy_y = 0x80; break;
        case 1:  copy_y = 0xc0; break;
        case 2:  copy_y = 0x94; break;
        case 3:  copy_y = 0xd4; break;
    }
    Write_Cmd(x + copy_y);
}
void HD44780_PutChar(unsigned char c)
{
	Write_Data(c & 0xff);
}

void HD44780_PutStr(char *str)
{
	__IO unsigned int i=0;

	do
	{
		HD44780_PutChar(str[i]);
		i++;
	}while(str[i]!='\0');
}
void LCDTest( void )
{
	unsigned char ascii=0;
	
	char buf1[30];
	char buf2[30];
	char buf3[30];
	char buf4[30];
    int i = 0;
    for(i=0;i<30;i++)
    {
        buf1[i] = 0;
        buf2[i] = 0;
        buf3[i] = 0;                                              
        buf4[i] = 0;                     
    }
	LCD_Help();
	while(1)
	{
		ascii = Uart_Getch2();
		if(ascii == '1')
		{
            HD44780_GotoXY(1,0);
		}
		else if( ascii == '2')
		{
            HD44780_GotoXY(0,1);
		}
		else if( ascii == '3')
		{
            HD44780_GotoXY(0,2);
		}
		else if( ascii == '4')
		{
            HD44780_GotoXY(0,3);
		}
        else if(ascii == '5')
        {
            HD44780_PutStr("1. GOOD");
            //Write_Data('B');
            //Write_Data('1');
            //Write_Data('D');
        }
        else if(ascii =='6')
        {
            HD44780_PutChar(0x20);
        }
		LCD_Help();
	}
}



