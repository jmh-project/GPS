/* Includes --------------------------------------------------------------------*/
#ifdef __cplusplus
 extern "C" {
#endif
#include "TEECO_System.h"

/* Private Compile Condition -------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/ 

/* Private define ------------------------------------------------------------*/ 
#define		Sprintf		Uart_SendString2
#define		Dprintf		Uart_Printf2
#define		Dgetch		Uart_Getch2
#define		Dstatus 	Uart_Status2
#define		DgetIntNum	Uart_GetIntNum2
#define		DgetString	Uart_GetString2
#define		Dputc		Uart_SendByte2

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
int 	UserMain(void);
void 	Uart_SendByte1(char Data);
void 	Uart_SendByte2(char Data);
void 	Uart_SendByte3(char Data);
void 	Uart_SendByte4(char Data);
void 	Uart_SendByte6(char Data);

void 	Uart_SendString1(char *pt);
void 	Uart_SendString2(char *pt);
void 	Uart_SendString3(char *pt);
void 	Uart_SendString4(char *pt);
void 	Uart_SendString6(char *pt);

void 	Uart_Printf1(char *fmt,...);
void 	Uart_Printf2(char *fmt,...);
void 	Uart_Printf3(char *fmt,...);
void 	Uart_Printf4(char *fmt,...);
void 	Uart_Printf6(char *fmt,...);

u8 		Uart_Getch1(void);
u8 		Uart_Getch2(void);
u8 		Uart_Getch3(void);
u8 		Uart_Getch4(void);
u8 		Uart_Getch6(void);

void 	Uart_GetString1(char *string);
void 	Uart_GetString2(char *string);
void 	Uart_GetString3(char *string);
void 	Uart_GetString4(char *string);
void 	Uart_GetString6(char *string);

u8 		Uart_Status1(void);
u8 		Uart_Status2(void);
u8 		Uart_Status3(void);
u8 		Uart_Status4(void);
u8 		Uart_Status6(void);

int 	Uart_GetIntNum1(void);
int 	Uart_GetIntNum2(void);
int 	Uart_GetIntNum3(void);
int 	Uart_GetIntNum4(void);
int 	Uart_GetIntNum6(void);

/* Private functions ---------------------------------------------------------*/
void Uart_SendByte1(char Data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, (u8) Data);
}
void Uart_SendByte2(char Data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_SendData(USART2, (u8) Data);
}
void Uart_SendByte7(char Data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(UART7, USART_FLAG_TC) == RESET);
	USART_SendData(UART7, (u8) Data);
}

int sendchar(int Data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_SendData(USART2, (u8) Data);
    return(Data);
}

void Uart_SendByte3(char Data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	USART_SendData(USART3, (u8) Data);
}									 
void Uart_SendByte4(char Data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
	USART_SendData(UART4, (u8) Data);
}
void Uart_SendByte5(char Data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
	USART_SendData(UART5, (u8) Data);
}
void Uart_SendByte6(char Data)
{
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
	USART_SendData(USART6, (u8) Data);
}

void Uart_SendString1(char *pt)
{
    while(*pt)	Uart_SendByte1(*pt++);
}
void Uart_SendString2(char *pt)
{
    while(*pt)	Uart_SendByte2(*pt++);    
    /*
    while(*pt)	
    {
        if(*pt=='\n')  
        {
            Uart_SendByte2(*pt++);
            Uart_SendByte2('\r');            
        }
        else
            Uart_SendByte2(*pt++);
    }
    */
}

void Uart_SendString3(char *pt)
{
    while(*pt)	Uart_SendByte3(*pt++);
}							  
void Uart_SendString4(char *pt)
{
    while(*pt)	Uart_SendByte4(*pt++);
}
void Uart_SendString5(char *pt)
{
    while(*pt)	Uart_SendByte5(*pt++);
}
void Uart_SendString6(char *pt)
{
    while(*pt)	Uart_SendByte6(*pt++);
}
void Uart_SendString7(char *pt)
{
    while(*pt)	Uart_SendByte7(*pt++);
}
void Uart_Printf1(char *fmt,...)
{
    va_list ap;
    char string[512];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString1(string);
    va_end(ap);
}
int Ysi_ReadData_Dos( char ascii );
void DebugMessagePrintf(char *pt)
{
    if(gv.debugFlag==1) {
        while(*pt)	Ysi_ReadData_Dos(*pt++);
    }
}

void Uart_Printf2(char *fmt,...)
{
    va_list ap;
    char string[512];
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString2(string); 
    va_end(ap);
}
#ifdef JSON_ENABLE
void TcpIpDebug(char *fmt,...) {	
    if(gv.jsonTcpDebugFlag==1) {
        va_list ap;
        char string[512];
        va_start(ap,fmt);
        vsprintf(string,fmt,ap);
        send(0, (unsigned char *)string, strlen(string));
        va_end(ap);
    }    
}
#endif
void TeecoUart_Printf2(int debug_area, char *fmt,...)
{
    va_list ap;
    char string[512];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString2(string);
    va_end(ap);
}

void Uart_Printf3(char *fmt,...)
{
    va_list ap;
    char string[512];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString3(string);
    va_end(ap);
}

void Uart_Printf4(char *fmt,...)
{
    va_list ap;
    char string[512];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString4(string);
    va_end(ap);
}
void Uart_Printf5(char *fmt,...)
{
    va_list ap;
    char string[512];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString5(string);
    va_end(ap);
}

void Uart_Printf6(char *fmt,...)
{
    va_list ap;
    char string[256];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString6(string);
    va_end(ap);
}
void Uart_Printf7(char *fmt,...)
{
    va_list ap;
    char string[512];

    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    Uart_SendString7(string);
    va_end(ap);
}

u8 Uart_Getch1(void)
{
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return(USART_ReceiveData(USART1));    
}

u8 Uart_Getch2(void)
{
	while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
    return(USART_ReceiveData(USART2));    
}

u8 Uart_Getch3(void)
{
	while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
    return(USART_ReceiveData(USART3));    
}
u8 Uart_Getch4(void)
{
	while(USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET);
    return(USART_ReceiveData(UART4));    
}
u8 Uart_Getch6(void)
{
	while(USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == RESET);
    return(USART_ReceiveData(USART6));    
}

u8 Uart_Status1(void)
{
	return(USART_GetFlagStatus(USART1, USART_FLAG_RXNE));
}
u8 Uart_Status2(void)
{
	return(USART_GetFlagStatus(USART2, USART_FLAG_RXNE));
}
u8 Uart_Status3(void)
{
	return(USART_GetFlagStatus(USART3, USART_FLAG_RXNE));
}
u8 Uart_Status4(void)
{
	return(USART_GetFlagStatus(UART4, USART_FLAG_RXNE));
}
u8 Uart_Status6(void)
{
	return(USART_GetFlagStatus(USART6, USART_FLAG_RXNE));
}



void Uart_GetString1(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart_Getch1())!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart_Printf1("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;
            Uart_SendByte1(c);
        }
    }
    *string='\0';
    Uart_SendByte1('\n');
}

void Uart_GetString2(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart_Getch2())!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart_Printf2("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;
            Uart_SendByte2(c);
        }
    }
    *string='\0';
    Uart_SendByte2('\n');
}

void Uart_GetString3(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart_Getch3())!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart_Printf3("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;
            Uart_SendByte3(c);
        }
    }
    *string='\0';
    Uart_SendByte3('\n');
}

void Uart_GetString4(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart_Getch4())!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart_Printf4("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;
            Uart_SendByte4(c);
        }
    }
    *string='\0';
    Uart_SendByte4('\n');
}

void Uart_GetString6(char *string)
{
    char *string2 = string;
    char c;
    while((c = Uart_Getch6())!='\r')
    {
        if(c=='\b')
        {
            if( (int)string2 < (int)string )
            {
                Uart_Printf6("\b \b");
                string--;
            }
        }
        else 
        {
            *string++ = c;
            Uart_SendByte6(c);
        }
    }
    *string='\0';
    Uart_SendByte6('\n');
}

int Uart_GetIntNum1(void)
{
    char str[30];
    char *string = str;
    int base     = 10;
    int minus    = 0;
    int result   = 0;
    int lastIndex;    
    int i;
    
    Uart_GetString1(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}

int Uart_GetIntNum2(void)
{
    char str[30];
    char *string = str;
    int base     = 10;
    int minus    = 0;
    int result   = 0;
    int lastIndex;    
    int i;
    
    Uart_GetString2(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}

int Uart_GetIntNum3(void)
{
    char str[30];
    char *string = str;
    int base     = 10;
    int minus    = 0;
    int result   = 0;
    int lastIndex;    
    int i;
    
    Uart_GetString3(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}

int Uart_GetIntNum4(void)
{
    char str[30];
    char *string = str;
    int base     = 10;
    int minus    = 0;
    int result   = 0;
    int lastIndex;    
    int i;
    
    Uart_GetString4(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}

int Uart_GetIntNum6(void)
{
    char str[30];
    char *string = str;
    int base     = 10;
    int minus    = 0;
    int result   = 0;
    int lastIndex;    
    int i;
    
    Uart_GetString6(string);
    
    if(string[0]=='-')
    {
        minus = 1;
        string++;
    }
    
    if(string[0]=='0' && (string[1]=='x' || string[1]=='X'))
    {
        base    = 16;
        string += 2;
    }
    
    lastIndex = strlen(string) - 1;
    
    if(lastIndex<0)
        return -1;
    
    if(string[lastIndex]=='h' || string[lastIndex]=='H' )
    {
        base = 16;
        string[lastIndex] = 0;
        lastIndex--;
    }

    if(base==10)
    {
        result = atoi(string);
        result = minus ? (-1*result):result;
    }
    else
    {
        for(i=0;i<=lastIndex;i++)
        {
            if(isalpha(string[i]))
            {
                if(isupper(string[i]))
                    result = (result<<4) + string[i] - 'A' + 10;
                else
                    result = (result<<4) + string[i] - 'a' + 10;
            }
            else
                result = (result<<4) + string[i] - '0';
        }
        result = minus ? (-1*result):result;
    }
    return result;
}



void HexDump  (u8 *addr, u32 len)
{
 	u8		*s=addr, *endPtr=(u8*)((u32)addr+len);
 	u32		i, remainder=len%16;
 		
 	Dprintf("\n Offset        Hex Value                                      Ascii value\n");
 	
 	// print out 16 byte blocks.
 	while (s+16<=endPtr)
	{ 		
 		// offset 출력.
 		Dprintf("0x%08lx  ", (long)(s-addr));
 		
 		// 16 bytes 단위로 내용 출력.
 		for (i=0; i<16; i++)
		{
 			Dprintf("%02x ", s[i]);
 		}
 		Dprintf(" ");
 		
 		for (i=0; i<16; i++)
		{
 			if (s[i]>=32 && s[i]<=125) Dprintf("%c", s[i]);
 			else Dprintf(".");
 		}
 		s += 16;
 		Dprintf("\n");
 	}
 	
 	// Print out remainder.
 	if (remainder){
 		
 		// offset 출력.
 		Dprintf("0x%08lx  ", (long)(s-addr));
 		
 		// 16 bytes 단위로 출력하고 남은 것 출력.
 		for (i=0; i<remainder; i++)
		{
 			Dprintf("%02x ", s[i]);
 		}
 		for (i=0; i<(16-remainder); i++)
		{
 			Dprintf("   ");
 		}
 
 		Dprintf(" ");
 		for (i=0; i<remainder; i++)
		{
 			if (s[i]>=32 && s[i]<=125) Dprintf("%c", s[i]);
 			else	Dprintf(".");
 		}
 		for (i=0; i<(16-remainder); i++)
		{
 			Dprintf(" ");
 		}
 		Dprintf("\n");
 	}
 	return;
}	// HexDump.

#ifdef __cplusplus
  }
#endif
