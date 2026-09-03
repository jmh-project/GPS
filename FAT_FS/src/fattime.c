#ifdef __cplusplus
 extern "C" {
#endif
#include "integer.h"
#include "fattime.h"
#include "BSP_DS1302.h"
//#include <stdio.h>
//#include <stdlib.h>
extern  int sprintf(char *s,  ...);
extern  int atoi(const char * str);

//#include "rtc.h" //RPi

/*
	bit31:25   	Year from 1980 (0..127)
	bit24:21   	Month (1..12)
	bit20:16   	Day in month(1..31)
	bit15:11  	Hour (0..23)
	bit10:5   	Minute (0..59)
	bit4:0  	    Second / 2 (0..29)

	The get_fattime() function must return any valid time even if the system does not support a real time clock. 
	If a zero is returned, the file will not have a valid time. This fucntion is not required in read only configuration.

	bit15:9   	Year from 1980 (0..127)
	bit8:5   	Month (1..12)
	bit4:0   	Day in month(1..31)
	
	bit15:11  	Hour (0..23)
	bit10:5   	Minute (0..59)
	bit4:0  	    Second / 2 (0..29)

*/

DWORD get_fattime (void)
{
    char		buf[32];
	TimeS	Time;
	uint32_t	year, mon, day, hour, min, sec;

	Time.sec = DS1302ReadByte(0x81) & 0x7f;
	Time.minute = DS1302ReadByte(0x83);
	Time.hour = DS1302ReadByte(0x85) & 0x3f;
	Time.date = DS1302ReadByte(0x87);
	Time.month = DS1302ReadByte(0x89);
	Time.year = DS1302ReadByte(0x8D);
	
	sprintf(buf,"%04x", Time.year+0x2000);
	year = atoi(buf);
	sprintf(buf,"%02x", Time.month);
	mon = atoi(buf);
	sprintf(buf,"%02x", Time.date);
	day = atoi(buf);
	sprintf(buf,"%02x", Time.hour);
	hour = atoi(buf);
	sprintf(buf,"%02x", Time.minute);
	min = atoi(buf);
	sprintf(buf,"%02x", Time.sec);
	sec = atoi(buf);
	
	return	((year-1980) << 25)	      // Year = 2006
			| (mon << 21)	      // Month = Feb
			| (day << 16)	      // Day = 9
			| (hour << 11)	      // Hour = 22
			| (min << 5)	      // Min = 30
			| (sec >> 1)	      // Sec = 0
			;
}
#ifdef __cplusplus
}
#endif

