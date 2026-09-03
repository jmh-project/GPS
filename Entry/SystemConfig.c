
#include "Teeco_System.h"
#include "stm32f4xx.h"
#include "HD44780.h"
extern char gBuffer1[2048];
extern int gCount1;
extern int gStart1;
extern int gProcess1;    

extern char gBuffer3[2048];
extern int gCount3;
extern int gStart3;
extern int gProcess3;    

extern char gBuffer4[2048];
extern int gCount4;
extern int gStart4;
extern int gProcess4;    

extern char gBuffer5[2048];
extern int gCount5;
extern int gStart5;
extern int gProcess5;    

extern char gBuffer6[2048];
extern int gCount6;
extern int gStart6;
extern int gProcess6;    


extern double gmV[220];
extern double gMinmV[120];
extern double gMaxmV[120];
extern char gStatus[120];
extern char gName[120][20];

extern void ProcessGage(void);
extern int gComplete1,gComplete2,gComplete3,gComplete4; 
void DataRequest(void)
{
    int i = 0;
    Dprintf("KEY TRANSMIT\n");

    while(1)
    {
        
        Uart_SendByte3('<');
        Uart_SendByte3('0');
        Uart_SendByte3('0');
        Uart_SendByte3('0');
        Uart_SendByte3('7');
        Uart_SendByte3(',');
        Uart_SendByte3('0');
        Uart_SendByte3('1');
        Uart_SendByte3(',');
        Uart_SendByte3('2');
        Uart_SendByte3('1');
        Uart_SendByte3('2');
        Uart_SendByte3(',');

        for(i=0; i<106; i++) 
        {   
            Uart_Printf3("%5.4f,%5.4f,",gMinmV[i],gMaxmV[i]);
        }
        Uart_SendByte3('>');
        while(gProcess3==0)
        {
            ProcessGage();
        }
        gComplete1 = 1;
        Dprintf("TRANSMIT CHANNEL1\n");        

        Uart_SendByte4('<');
        Uart_SendByte4('0');
        Uart_SendByte4('0');
        Uart_SendByte4('0');
        Uart_SendByte4('7');
        Uart_SendByte4(',');
        Uart_SendByte4('0');
        Uart_SendByte4('1');
        Uart_SendByte4(',');
        Uart_SendByte4('2');
        Uart_SendByte4('1');
        Uart_SendByte4('2');
        Uart_SendByte4(',');

        for(i=0; i<106; i++) 
        {   
            Uart_Printf4("%5.4f,%5.4f,",gMinmV[i],gMaxmV[i]);
        }
        Uart_SendByte4('>');

        while(gProcess4==0)
        {
            ProcessGage();
        }
        gComplete2 = 1;
        Dprintf("TRANSMIT CHANNEL2\n");

        Uart_SendByte5('<');
        Uart_SendByte5('0');
        Uart_SendByte5('0');
        Uart_SendByte5('0');
        Uart_SendByte5('7');
        Uart_SendByte5(',');
        Uart_SendByte5('0');
        Uart_SendByte5('1');
        Uart_SendByte5(',');
        Uart_SendByte5('2');
        Uart_SendByte5('1');
        Uart_SendByte5('2');
        Uart_SendByte5(',');

        for(i=0; i<106; i++) 
        {   
            Uart_Printf5("%5.4f,%5.4f,",gMinmV[i],gMaxmV[i]);
        }
        Uart_SendByte5('>');

        while(gProcess5==0)
        {
            ProcessGage();
        }
        gComplete3 = 1;
        Dprintf("TRANSMIT CHANNEL3\n");
        
        Uart_SendByte6('<');
        Uart_SendByte6('0');
        Uart_SendByte6('0');
        Uart_SendByte6('0');
        Uart_SendByte6('7');
        Uart_SendByte6(',');
        Uart_SendByte6('0');
        Uart_SendByte6('1');
        Uart_SendByte6(',');
        Uart_SendByte6('2');
        Uart_SendByte6('1');
        Uart_SendByte6('2');
        Uart_SendByte6(',');

        for(i=0; i<106; i++) 
        {   
            Uart_Printf6("%5.4f,%5.4f,",gMinmV[i],gMaxmV[i]);
        }
        Uart_SendByte6('>');         

        while(gProcess6==0)
        {
            ProcessGage();
        }
        gComplete4 = 1;
        Dprintf("TRANSMIT CHANNEL4\n");





        break;
    }
}
 void BuzzerOut(int sound)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    Delay(sound);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}
int KeyRead(void)
{
    int sound = 100;
    if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)==0)
    {
        BuzzerOut(sound);
        while(1)
        {
            if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_0)==1)
            {
                return 1;
            }
        }
    }
    if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)==0)
    {
        BuzzerOut(sound);
        while(1)
        {
            if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)==1)
            {
                return 2;
            }
        }
    }
    if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)==0)
    {
        BuzzerOut(sound);
        while(1)
        {
            if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2)==1)
            {
                return 3;
            }
        }
    }
    if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)==0)
    {
        BuzzerOut(sound);
        while(1)
        {
            if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)==1)
            {
                return 4;
            }
        }
    }
    if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)==0)
    {
        BuzzerOut(sound);
        while(1)
        {
            if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4)==1)
            {
                return 5;
            }
        }
    }
    if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5)==0)
    {
        BuzzerOut(sound);
        while(1)
        {
            if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5)==1)
            {
                return 6;
            }
        }
    }
    return 0;
}    

void NameConfig(void)
{
    int address = 0;

    strcpy(gName[address++],"VANA 1E   ");
    strcpy(gName[address++],"SIG_IN1 3H");
    strcpy(gName[address++],"SIG_IN2 2H");
    strcpy(gName[address++],"REFIN 2J  ");
    strcpy(gName[address++],"VPAD 1G   ");
    strcpy(gName[address++],"VANA 1E   ");
    strcpy(gName[address++],"SIG_IN1 3H");
    strcpy(gName[address++],"SIG_IN2 2H");
    strcpy(gName[address++],"REFIN 2J  ");
    strcpy(gName[address++],"VPAD 1G   ");
    strcpy(gName[address++],"VTDO 8J   ");
    strcpy(gName[address++],"VRFEM 7J  ");
    strcpy(gName[address++],"VECID 11H ");
    strcpy(gName[address++],"VTDO 8J   ");
    strcpy(gName[address++],"VRFEM 7J  ");
    strcpy(gName[address++],"VECID 11H ");
    strcpy(gName[address++],"PLL_FLT 8H");
    strcpy(gName[address++],"PLL_FLT 8H");
    strcpy(gName[address++],"TST 12H   ");
    strcpy(gName[address++],"I2C_SEL0 6");
    strcpy(gName[address++],"I2C_SEL1 4");
    strcpy(gName[address++],"SCE 7H    ");
    strcpy(gName[address++],"RSTN 12G  ");
    strcpy(gName[address++],"SCL 10H   ");
    strcpy(gName[address++],"SDA 9H    ");
    strcpy(gName[address++],"MCLK 7H   ");
    strcpy(gName[address++],"VSYNC_IN 8");
    strcpy(gName[address++],"HSYNC_IN 7");
    strcpy(gName[address++],"GPIO0 2C  ");
    strcpy(gName[address++],"GPIO1 10D ");
    strcpy(gName[address++],"GPIO2 10E ");
    strcpy(gName[address++],"GPIO3 10F ");
    strcpy(gName[address++],"GPIO4 11G ");
    strcpy(gName[address++],"GPIO5 10G ");
    strcpy(gName[address++],"TST 12H   ");
    strcpy(gName[address++],"I2C_SEL0 6");
    strcpy(gName[address++],"I2C_SEL1 4");
    strcpy(gName[address++],"SCE 7H    ");
    strcpy(gName[address++],"RSTN 12G  ");
    strcpy(gName[address++],"SCL 10H   ");
    strcpy(gName[address++],"SDA 9H    ");
    strcpy(gName[address++],"MCLK 7H   ");
    strcpy(gName[address++],"VSYNC_IN 8");
    strcpy(gName[address++],"HSYNC_IN 7");
    strcpy(gName[address++],"GPIO0 2C  ");
    strcpy(gName[address++],"GPIO1 10D ");
    strcpy(gName[address++],"GPIO2 10E ");
    strcpy(gName[address++],"GPIO3 10F ");
    strcpy(gName[address++],"GPIO4 11G ");
    strcpy(gName[address++],"GPIO5 10G ");
    strcpy(gName[address++],"M_DNDATA0 ");
    strcpy(gName[address++],"M_DNDATA1 ");
    strcpy(gName[address++],"M_DNDATA2 ");
    strcpy(gName[address++],"M_DNDATA3 ");
    strcpy(gName[address++],"M_DPDATA0 ");
    strcpy(gName[address++],"M_DPDATA 1");
    strcpy(gName[address++],"M_DPDATA 2");
    strcpy(gName[address++],"M_DPDATA 3");
    strcpy(gName[address++],"M_DNCLK 10");
    strcpy(gName[address++],"M_DPCLK 9C");
    strcpy(gName[address++],"M_DNDATA0 ");
    strcpy(gName[address++],"M_DNDATA1 ");
    strcpy(gName[address++],"M_DNDATA2 ");
    strcpy(gName[address++],"M_DNDATA3 ");
    strcpy(gName[address++],"M_DPDATA0 ");
    strcpy(gName[address++],"M_DPDATA1 ");
    strcpy(gName[address++],"M_DPDATA2 ");
    strcpy(gName[address++],"M_DPDATA3 ");
    strcpy(gName[address++],"M_DNCLK 10");
    strcpy(gName[address++],"M_DPCLK 9C");
    strcpy(gName[address++],"DATAN0 1C ");
    strcpy(gName[address++],"DATAN1 2A ");
    strcpy(gName[address++],"DATAN2 4B ");
    strcpy(gName[address++],"DATAN3 4A ");
    strcpy(gName[address++],"DATAN4 6C ");
    strcpy(gName[address++],"DATAN5 5A ");
    strcpy(gName[address++],"DATAN6 6B ");
    strcpy(gName[address++],"DATAN7 7A ");
    strcpy(gName[address++],"DATAP0 1B ");
    strcpy(gName[address++],"DATAP1 2B ");
    strcpy(gName[address++],"DATAP2 3B ");
    strcpy(gName[address++],"DATAP3 3A ");
    strcpy(gName[address++],"DATAP4 5C ");
    strcpy(gName[address++],"DATAP5 5B ");
    strcpy(gName[address++],"DATAP6 6A ");
    strcpy(gName[address++],"DATAP7 7B ");
    strcpy(gName[address++],"CLKN0 3C  ");
    strcpy(gName[address++],"CLKP0 2C  ");
    strcpy(gName[address++],"DATAN0 1C ");
    strcpy(gName[address++],"DATAN1 2A ");
    strcpy(gName[address++],"DATAN2 4B ");
    strcpy(gName[address++],"DATAN3 4A ");
    strcpy(gName[address++],"DATAN4 6C ");
    strcpy(gName[address++],"DATAN5 5A ");
    strcpy(gName[address++],"DATAN6 6B ");
    strcpy(gName[address++],"DATAN7 7A ");
    strcpy(gName[address++],"DATAP0 1B ");
    strcpy(gName[address++],"DATAP1 2B ");
    strcpy(gName[address++],"DATAP2 3B ");
    strcpy(gName[address++],"DATAP3 3A ");
    strcpy(gName[address++],"DATAP4 5C ");
    strcpy(gName[address++],"DATAP5 5B ");
    strcpy(gName[address++],"DATAP6 6A ");
    strcpy(gName[address++],"DATAP7 7B ");
    strcpy(gName[address++],"CLKN0 3C  ");
    strcpy(gName[address++],"CLKP0 2C  ");
    strcpy(gName[address++],"NONE      ");
    strcpy(gName[address++],"NONE      ");
}
void HD44780_GotoXY(unsigned char x, unsigned char y);
void HD44780_PutStr(char *str);
void ProcessGage(void)
{
    HD44780_GotoXY(0,3);
    
    if(g_Timer_msec < 750)
    {
        HD44780_PutStr(">");
    }
    else if(g_Timer_msec < 750*2)
    {
        HD44780_PutStr(">>");
    }
    else if(g_Timer_msec < 750*3)
    {
        HD44780_PutStr(">>>");
    }
    else if(g_Timer_msec < 750*4)
    {
        HD44780_PutStr(">>>>");
    }
    else if(g_Timer_msec < 750*5)
    {
        HD44780_PutStr(">>>>>");
    }
    else if(g_Timer_msec < 750*6)
    {
        HD44780_PutStr(">>>>>>");
    }
    else if(g_Timer_msec < 750*7)
    {
        HD44780_PutStr(">>>>>>>");
    }
    else if(g_Timer_msec < 750*8)
    {
        HD44780_PutStr(">>>>>>>>");
    }
    else if(g_Timer_msec < 750*9)
    {
        HD44780_PutStr(">>>>>>>>>");
    }
    else if(g_Timer_msec < 750*10)
    {
        HD44780_PutStr(">>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*11)
    {
        HD44780_PutStr(">>>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*12)
    {
        HD44780_PutStr(">>>>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*13)
    {
        HD44780_PutStr(">>>>>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*14)
    {
        HD44780_PutStr(">>>>>>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*15)
    {
        HD44780_PutStr(">>>>>>>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*16)
    {
        HD44780_PutStr(">>>>>>>>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*17)
    {
        HD44780_PutStr(">>>>>>>>>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*18)
    {
        HD44780_PutStr(">>>>>>>>>>>>>>>>>>");
    }
    else if(g_Timer_msec < 750*19)
    {
        HD44780_PutStr(">>>>>>>>>>>>>>>>>>>");
    }
    else if(g_Timer_msec >=20000)
    {
        HD44780_PutStr("Error Restart.      ");
    }
}
