#ifndef _BSP_DS1302
#define _BSP_DS1302

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "TEECO_C_Define.h"

//Real Time
typedef struct Rtime {
	int		year;
	int		month;
	int		date;
	int		hour;
	int		minute;
	int		sec;
}TimeS;

typedef struct {
	volatile int	Year;
	volatile int	Month;
	volatile int	Day;
	volatile int	WeekEnd;
} Date;

typedef struct {
	volatile int	Hour;
	volatile int	Minute;
	volatile int	Second;
} Time;
     
unsigned char DS1302ReadByte(unsigned char command);
void DS1302WriteByte(unsigned char command, unsigned char data);
void DS1302InitSet(void);
void DS1302_YearSet(uint8_t date);
void DS1302_MonthSet(uint8_t date);
void DS1302_DaySet(uint8_t date);
void DS1302_HourSet(uint8_t date);
void DS1302_MinSet(uint8_t date);
void DS1302_SecSet(uint8_t date);
int DS1302TimeRead(void);
#ifdef __cplusplus
}
#endif

#endif
