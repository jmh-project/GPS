/******************************************************************************
*    	File name	: BSP_DS1302.c
*		Company		:
*		Version		:
*		Data		:
*		Author		:
*		Contact		:
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif


/******************************************************************************
							Include File
******************************************************************************/
#include "BSP_DS1302.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "Debug_Uart.h"
#include "DataLog.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

void Delay(__IO uint32_t nTime);

/******************************************************************************
							Constant & Macros
******************************************************************************/
#define TESTYEAR    (0x16)
#define TESTMONTH   (0x01)
#define TESTDATE    (0x01)
#define TESTDAY     (0x15)      //SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7   
#define TESTHOUR    (0x10)
#define TESTMIN     (0x10)
#define TESTSEC     (0x00)
/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
void RTC_Delay(void)
{
	for(int i=0; i< 5000; i++);
}	

void DS1302PortInit(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	

    //DS1302 Pin Setting
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOG, GPIO_Pin_7);      //IO Pin
    GPIO_ResetBits(GPIOF, GPIO_Pin_10);      //Clk Pin
    GPIO_ResetBits(GPIOF, GPIO_Pin_9);     //CS Pin
}

void DS1302DataPort_InMode(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	

    //DS1302 Pin Setting
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

void DS1302DataPort_OutMode(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	

    //DS1302 Pin Setting
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG , ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}


void DS1302Clock(void)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_10);     //CLK Pin
    RTC_Delay();// Delay(1);
    GPIO_ResetBits(GPIOF, GPIO_Pin_10);   //CLK Pin                   
    RTC_Delay();// Delay(1);
}

void DS1302Data(int s)
{
    if(s==1)
        GPIO_SetBits(GPIOG, GPIO_Pin_7);      //IO Pin
    else
        GPIO_ResetBits(GPIOG, GPIO_Pin_7);      //IO Pin
}

void DS1302Start(void)
{
    GPIO_SetBits(GPIOF, GPIO_Pin_9);     //CS Pin
    RTC_Delay();// Delay(1);
}

void DS1302Stop(void)
{
    GPIO_ResetBits(GPIOF, GPIO_Pin_9);     //CS Pin    
    RTC_Delay();// Delay(1);
}

void DS1302OutByte(unsigned char data)
{
    unsigned char    i, temp;

    if(data & 0x01)  { DS1302Data(1);  }
    else             { DS1302Data(0);  }
    RTC_Delay();// Delay(1);
    DS1302Clock();

    for(i=1; i<8; i++)
    {
        temp = data >> i;
        if(temp & 0x01)  { DS1302Data(1);  }
        else             { DS1302Data(0);  }
		RTC_Delay();// Delay(1);
        DS1302Clock();
    }
}

void DS1302WriteByte(unsigned char command, unsigned char data)
{
    DS1302Start();  //CS is High
    DS1302OutByte(command & 0xfe);
    DS1302OutByte(data);  
    DS1302Stop();   //CS is Low         
}

unsigned char DS1302InByte(void)
{
    unsigned char    c;
    unsigned char    i;
    c=0x00;
    
    DS1302DataPort_InMode();
	
    RTC_Delay();// Delay(1);
    for(i=0; i<8; i++)
    {
        if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7))
        {
            switch(i)
            {
                case 0: c |= 0x01; break;
                case 1: c |= 0x02; break;
                case 2: c |= 0x04; break;
                case 3: c |= 0x08; break;
                case 4: c |= 0x10; break;
                case 5: c |= 0x20; break;
                case 6: c |= 0x40; break;
                case 7: c |= 0x80; break;
            }
        }
        DS1302Clock();
    }       
    DS1302DataPort_OutMode();    
    RTC_Delay();// Delay(1);
    return (c);
}

unsigned char DS1302ReadByte(unsigned char command)
{
    unsigned char    i;
    DS1302Start();  //CS is High
    DS1302OutByte(command | 0x01);
    i=DS1302InByte();
    DS1302Stop();   //CS is Low      
    return (i);   
}

void DS1302Init(void)
{
    unsigned char    i;

	//DS1302PortInit();

    DS1302WriteByte(0x8E, 0x00);    
    DS1302WriteByte(0x90, 0xA5);
    
    i = DS1302ReadByte(0x80);
    i = i & 0x7f;
    DS1302WriteByte(0x80, i);
    
    i = DS1302ReadByte(0x84);
    i = i & 0x7f;       //24 시간제로 셋팅한다.
    DS1302WriteByte(0x84, i);    
}

void RTCTest(void)
{
//    char    buf[50];
    unsigned char min, sec;
    static  unsigned char oldsec=100;
//    static unsigned char toggle=0;
    
	DS1302Init();
	Dprintf("DS1302 RTC Test\n");
    while(1)
    {
        min = DS1302ReadByte(0x83);    
        sec = DS1302ReadByte(0x81);
        if(oldsec == 100) oldsec=sec;
        else
        {
            if(oldsec != sec)
            {
                Dprintf("%02x:%02x",min,sec);
                oldsec = sec;
            }
        }    
    }
}
int DS1302TimeRead(void)
{
    char    fbuf[10];
    for(int i=0; i<10; i++) {
        fbuf[i]=0;
    }
    
    currentDate.sec = DS1302ReadByte(0x81) & 0x7f;
    currentDate.minute = DS1302ReadByte(0x83);
    currentDate.hour = DS1302ReadByte(0x85) & 0x3f;
    currentDate.date = DS1302ReadByte(0x87);
    currentDate.month = DS1302ReadByte(0x89);
    currentDate.year = DS1302ReadByte(0x8D);

    sprintf(fbuf,"%02x",currentDate.year);  fbuf[3]=0;
	currentDate.year=atoi(fbuf);		
	sprintf(fbuf,"%02x",currentDate.month); fbuf[3]=0;
	currentDate.month=atoi(fbuf);
	sprintf(fbuf,"%02x",currentDate.date);  fbuf[3]=0;
	currentDate.date=atoi(fbuf);		
	sprintf(fbuf,"%02x",currentDate.hour);  fbuf[3]=0;
	currentDate.hour=atoi(fbuf);		
	sprintf(fbuf,"%02x",currentDate.minute);fbuf[3]=0;
	currentDate.minute=atoi(fbuf);	
	sprintf(fbuf,"%02x",currentDate.sec);   fbuf[3]=0;  
	currentDate.sec=atoi(fbuf);
    
	//Dprintf("%4x.%02x.%02x  %02x:%02x:%02x", Real.year, Real.month, Real.date, Real.hour, Real.minute, Real.sec);	
	return(1);    
}

//!< 초기에 한번은 호출해줘야한다.
//!< 이내용은 리소스매니져에서 추가하도록한다.
void DS1302InitSet(void)
{
	U8 time;
	
	DS1302Init();
	
    DS1302WriteByte(0x80, TESTSEC);
    DS1302WriteByte(0x82, TESTMIN);
    
    time =     DS1302ReadByte(0x85);
    time = (time & 0x80) | TESTHOUR;
    
    DS1302WriteByte(0x84, time);
    DS1302WriteByte(0x86, TESTDATE);
    DS1302WriteByte(0x88, TESTMONTH);
    DS1302WriteByte(0x8C, TESTYEAR);
	Dprintf("RTCinit OK:%02x/%02x/%02x %02x:%02x:%02x\n", TESTYEAR, TESTMONTH, TESTDATE, TESTHOUR, TESTMIN,TESTSEC);
}

void DS1302_YearSet(uint8_t date)
{
	DS1302Init();	
    DS1302WriteByte(0x8C, date);
}
void DS1302_MonthSet(uint8_t date)
{
	DS1302Init();	
    DS1302WriteByte(0x88, date);
}
void DS1302_DaySet(uint8_t date)
{
	DS1302Init();	
    DS1302WriteByte(0x86, date);
}
void DS1302_HourSet(uint8_t date)
{
	U8	time;
	DS1302Init();	
    time =     DS1302ReadByte(0x85);
    time = (time & 0x80) | date;	
    DS1302WriteByte(0x84, date);
}

void DS1302_MinSet(uint8_t date)
{
	DS1302Init();	
    DS1302WriteByte(0x82, date);
}

void DS1302_SecSet(uint8_t date)
{
	DS1302Init();	
    DS1302WriteByte(0x80, date);
}

void DS1302ToRtc(void)
{
}

void DS1302Display(void)
{
	TimeS	Time;

	Time.sec = DS1302ReadByte(0x81) & 0x7f;
	Time.minute = DS1302ReadByte(0x83);
	Time.hour = DS1302ReadByte(0x85) & 0x3f;
	Time.date = DS1302ReadByte(0x87);
	Time.month = DS1302ReadByte(0x89);
	Time.year = DS1302ReadByte(0x8D);
	
	Dprintf("%04x/%02x/%02x %02x:%02x.%02x\n", Time.year+0x2000, Time.month, Time.date, Time.hour, Time.minute, Time.sec);
}

#ifdef __cplusplus
}
#endif

