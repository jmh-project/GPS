#ifdef __cplusplus
 extern "C" {
#endif
/* Includes --------------------------------------------------------------------*/

/* Private Compile Condition -------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/ 

/* Private define ------------------------------------------------------------*/ 
#define		Sprintf		Uart_SendString2
#define		Dprintf 	Uart_Printf2//WAIT
#define		Dgetint		Uart_GetIntNum2
#define		Dgetch		Uart_Getch2
#define		Dstatus 	Uart_Status2
#define		DgetIntNum	Uart_GetIntNum2
#define		DgetString	Uart_GetString2
#define		Dputc		Uart_SendByte2

#define		TPrintf		Uart_Printf2	 
    
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void DebugMessagePrintf(char *pt);
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
void 	Uart_Printf7(char *fmt,...);
void 	TeecoUart_Printf2(int debugarea, char *fmt,...);

unsigned char 		Uart_Getch1(void);
unsigned char 		Uart_Getch2(void);
unsigned char 		Uart_Getch3(void);
unsigned char 		Uart_Getch4(void);
unsigned char 		Uart_Getch6(void);

void 	Uart_GetString1(char *string);
void 	Uart_GetString2(char *string);
void 	Uart_GetString3(char *string);
void 	Uart_GetString4(char *string);
void 	Uart_GetString6(char *string);

int 	Uart_GetIntNum1(void);
int 	Uart_GetIntNum2(void);
int 	Uart_GetIntNum3(void);
int 	Uart_GetIntNum4(void);
int 	Uart_GetIntNum6(void);

#ifndef u8
#define u8	unsigned char
#endif
#ifndef u32
#define u32	unsigned long
#endif

u8 Uart_Status1(void);
u8 Uart_Status2(void);
u8 Uart_Status3(void);
u8 Uart_Status4(void);


void HexDump  (u8 *addr, u32 len);

/* Private functions ---------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif
