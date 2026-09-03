#include "Teeco_System.h"
#include "SiI9293.h"

//#define		ON		1
//#define		OFF		0
#define     MAXIN_DOSKEY        20

#define		DEV_ADDR					0x64
#define		DEV_ADDR_WR			0x64
#define		DEV_ADDR_RD			0x65

//#define		SDA			GPIO_Pin_9
//#define		SCL			GPIO_Pin_6

#define		SCL_High()	GPIOB->BSRRL = SCL
#define		SCL_Low()	GPIOB->BSRRH = SCL
#define		SDA_High()	GPIOB->BSRRL = SDA
#define		SDA_Low()	GPIOB->BSRRH = SDA

static char  LineCommand[MAXIN_DOSKEY][128];

int StrToHex(char *data);
void HDMI_Shell(void);
void HDMI_Chip_Reset(void);

void HDMI_PortInit(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    /* GPIOE Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD , ENABLE);

	//RX CEC Input
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

	//RX Int
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//RX Chip Reset
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	GPIOC->BSRRL =GPIO_Pin_2;	//RX Reset High
}

void HDMI_Chip_Reset(void)
{
	GPIOC->BSRRH =GPIO_Pin_2;	//RX Reset Low
	Delay(10);
	GPIOC->BSRRL =GPIO_Pin_2;	//RX Reset High
}

void HPDTest(void)
{
	for(int addr=0; addr < 0x1A; addr++)
	{
		uint8_t data = SII9293Read(0x64, addr);
		Dprintf("ADDR:%02x ->%02x\n", addr, data);
	}
}

void Sil9293Test_Help(void)
{
	Dprintf("\tShell 명령\n");
	Dprintf("\t> rd------[ rx_read 0x64 0x00 ], 칩의 레지스터를 읽는명령\n");
	Dprintf("\t> wr-----[ rx_write 0x64 0x00 0x11 ], 칩의 레지스터에 쓰는명령\n");
	Dprintf("\t> id---------[ rxid ] Sil9293 ID Read\n");
	Dprintf("\t> sts-------[ rx_sts ] SiI9293 HDMI Signal Detect Check & Setting Information\n");
	Dprintf("\t> hpd-------[ rx_hpd 0 or 1 ] Sil9293 HPD Signal\n");
    Dprintf("-----------------------------------------------------------------\n");	
	Dprintf("\t> vs-----------[ vs + or - ] Vsync Polarity\n");
	Dprintf("\t> hs-----------[ hs + or - ] Hsync Polarity\n");
	Dprintf("\t> clk----------[ clk + or - ] clk Polarity\n");
    Dprintf("-----------------------------------------------------------------\n");	
	Dprintf("\t> help\n");
	Dprintf("\t> reset\n");		
}

void EDID_Test(void);

int Command_Parsing(char *Buf)
{
	int		i, j;
	int		buf_find=0;
	int		argc_cnt=0;
	int		array_cnt=0;
	int		space_cnt=0;
	char		arg[10][32];		// 16개 짜리가 10개 있다.5:Row, 16:Colume
	int		len= strlen(Buf);	// 사용자가 입력한 명령의 길이를 계산한다.
//	int 		ret=0;

	if(*Buf==' ' || Buf[len-1] == ' ') 
	{
		//명령의 첫문자 & Last Cjaracter 공백이면 입력에러로 처리한다.
		Dprintf("명령의처음이나 끝에 공백이 있으면 안됩니다.");
		return(0);	//First character is space
	}	

	// 입력된 문자열중에서 공백하나를 기준으로 명령을 골라낸다.
	for(i=0; i<strlen(Buf); i++)
	{
		if(Buf[i] == ' ')
		{
			if(buf_find==0)
			{
				//Dprintf("1. Space find i=%d argc_cnt=%d array_cnt=%d,\n", i, argc_cnt, array_cnt);
	   			for(j=array_cnt; j < i; j++)
				{
					arg[argc_cnt][j-array_cnt] = Buf[j];
				}
				if(argc_cnt >= 10)
				{
					Dprintf("Too many Parameter input ");
					return(0);
				}
				arg[argc_cnt][j-array_cnt] = 0;
				buf_find=1;		// Space Twice Protected
				argc_cnt++;		// Parameter Count
				array_cnt = i+1;
				space_cnt++;
				//Dprintf("space find=%s\n", arg[argc_cnt]); 

			}
			else
			{
				//공백이 2개면 입력에러로 처리한다.
				Dprintf("공백이 2개이상 입력되었습니다. ");
				return(0);	//if spcae twice writen then Error 
			}
		}
		else
		{
			buf_find=0;
		}
	}
	// Last Command Buffer Read
	// 파라메터가 1개라도 검출되면 마지막의 파라메터를 취한다.
	if(argc_cnt >= 1)
	{
		for(j=array_cnt; j < i; j++)
		{
			arg[argc_cnt][j-array_cnt] = Buf[j];
		}
		arg[argc_cnt][j-array_cnt] = 0;
		argc_cnt++;	
	}
	if(argc_cnt==0)		 	// command parameter 1
	{
		if(!strcmp(Buf,"help")  || !strcmp(Buf,"?") ){ Sil9293Test_Help(); return 1; }
		else if(!strcmp(Buf,"reset"))	{NVIC_SystemReset(); return(0); }
		else if(!strcmp(Buf,"id"))	{RXID_Read();	return(1); }
		else if(!strcmp(Buf,"sts"))	{RXSystemStatus();	return(1); }
		else if(!strcmp(Buf,"r"))	{HDMI_Chip_Reset();	return(1); }
		else if(!strcmp(Buf,"e"))	{EDID_Test();	return(1); }
	}
	else if(argc_cnt==2)
	{
		if(!strcmp(arg[0],"hpd")) { 	RXHotPlug(arg[1]);	return(1);}
		else if(!strcmp(arg[0],"hs")) { 	HS_Setting(arg[1]);	return(1);	}
		else if(!strcmp(arg[0],"vs")) { 	VS_Setting(arg[1]);	return(1);   }
		else if(!strcmp(arg[0],"clk")) { 	CLK_Setting(arg[1]);	return(1);	}
	}
	else if(argc_cnt==3)
	{
		if(!strcmp(arg[0],"rd")) 
		{ 
			uint8_t dev_addr = StrToHex(arg[1]);
			uint8_t dev_reg_addr = StrToHex(arg[2]);
			//arg[1]을 사용한다.
			Dprintf("ADDR->%02x:%02x=[%02x]\n", dev_addr, dev_reg_addr, SII9293Read(dev_addr, dev_reg_addr));
			return 1; 
		}
	}
	else if(argc_cnt==4)
	{
		if(!strcmp(arg[0],"wr")) 
		{ 
			uint8_t dev_addr = StrToHex(arg[1]);
			uint8_t dev_reg_addr = StrToHex(arg[2]);			
			uint8_t data = StrToHex(arg[3]);
			SII9293Write(dev_addr, dev_reg_addr, data);			
			Dprintf("Verify ADDR->%02x:%02x=[%02x]\n", dev_addr, dev_reg_addr, SII9293Read(dev_addr, dev_reg_addr));
			return 1; 
		}
	}
	return(0);
}


void HDMI_Shell(void)
{
	int		c_len, i;
	int		ret;
    char   	sd; 
    int     	com_count=0;       //현재저장된 명령열 수 
    int     	line_count =0;      //Doskey용 카운트
    int     	in_count=0;         //현재 라인에 사용자가 입력하는 문자수    
	char		sbuf[128];
	
	sbuf[0] = 0;
	Dprintf("\n\r+---------------------------------------------------------------+\n");
	Dprintf("|                  TEECO shell  Manager V 2.0                   |\n");
    Dprintf("+---------------------------------------------------------------+\n");
    
	Dprintf("shell>");	
    for(;;)
    {
        if(Dstatus())
        {
            sd = Dgetch();
            if(sd==0x1b) //DOS Key 기능의 구현
            {
                sd = Dgetch();
                if(sd==0x5b)
                {
                    sd = Dgetch();
                    if(sd==0x41)
                    {
                        //Dprintf("com_count=%d line_count=%d in_count=%d\n", com_count, line_count, in_count);
                        if(com_count !=0)
                        {
                            in_count = 0;
                            line_count--;
                            if(line_count < 0) 
                            {
                                line_count = com_count-1;
                            }
                            if(line_count>=0)
                            {          
                                Dprintf("\r                                                                                                 ");
								Dprintf("\nshell>");								
                                strcpy(sbuf, LineCommand[line_count]);
                                in_count = strlen(sbuf);
                            }
                        }
                        else //아무의미없이 도스키가 눌리면
                        {
                            in_count=0;                 //입력 문자열의버퍼를 초기화한다.
                            sbuf[0] = 0;
                        }
                    } 
                    else  if(sd==0x42)
                    {
                        if(com_count !=0)
                        {
                            if(line_count < com_count)
                            {
                                line_count++;
                                if(line_count >= com_count) 
                                {
                                    line_count = 0;
                                }
                                if(line_count>=0)
                                {          
                                    Dprintf("\r                                                                                                 ");									
									Dprintf("\nshell>");								
                                    strcpy(sbuf, LineCommand[line_count]);
                                    in_count = strlen(sbuf);
                                }
                            }
                        }
                        else
                        {
                            in_count=0;                 //입력 문자열의버퍼를 초기화한다.
                            sbuf[0] = 0;
                        }                        
                    }
                }
            }
            else if(sd==0x0d)
            {
                c_len = strlen(sbuf);
                if(c_len!=0)
                {
                    strcpy(LineCommand[com_count++], sbuf); //입력된 문자열을 버퍼에 저장하고 버퍼카운트를 증가시킨다.
                    line_count = com_count;                        //Doskey 시작값을 마지막 입력값으로 설정한다.
                    
                    if(com_count >= MAXIN_DOSKEY) 
                    {
                        for(i=0; i<MAXIN_DOSKEY-1; i++)
                        {
                            strcpy(LineCommand[i],LineCommand[i+1]);
                        }
                        com_count=99;
                        strcpy(LineCommand[com_count], sbuf);
                    }                    
                    if(!strcmp(sbuf, "cls"))
                    {
                        com_count = line_count = in_count=0;
                        sbuf[0] = 0;
                        for(i=0; i<40; i++) Dputc('\n');
                    }
                    else
                    {
						Dprintf("\n");
                        ret = Command_Parsing(sbuf);
                        in_count=0;
                        if(ret==0)
                        {
                            Dprintf("%s 는 실행할수있는 명령이 아닙니다.\n", sbuf);
							sbuf[0] = 0;
                        }
                        else if(ret==2) break;
						sbuf[0] = 0;
                    }
					Dprintf("\nshell>");
                }   
                else
                {
					Dprintf("\nshell>");					
                    in_count=0;                 //입력 문자열의버퍼를 초기화한다.
                    sbuf[0] = 0;
                }
            }
            else if(sd=='\b')
            {
                if(in_count != 0)               //입력된 문자가 하나라도 있어야 동작
                {
                    Dprintf("\b \b");         //화면의 1문자를 지우고
                    sbuf[--in_count] = 0;     //문자열의 마지막문자를 지운다.
                }
            } 
            else if(sd >=' ' && sd <=0x7f) //문자가 입력이되면
            {
                Dputc(sd);                      //화면에 출력하고
                sbuf[in_count++] = sd;      //버퍼에 저장한다.
                sbuf[in_count] = 0;           //문자열의 구성을 위해 NULL 추가  
            }
        }
    }
}

void SII9293Write(  uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
{
    I2C_Start();
    I2C_Clock_Data(dev_addr);           

    I2C_ACK();                      

    I2C_Clock_Data(reg_addr);        

    I2C_ACK();                      

    I2C_Clock_Data(data);           

    I2C_ACK();                      

    I2C_Stop();                     
}

void SII9293WriteBlock(  uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, int size)
{
	for(int i=0; i< size; i++)
	{
		SII9293Write(dev_addr, reg_addr++, data[i]);
	}
}

uint8_t SII9293Read( uint8_t dev_addr, uint8_t reg_addr )
{
    u8 data=0x00;

    I2C_Start();
    I2C_Clock_Data(dev_addr);           
    I2C_ACK();                      

    I2C_Clock_Data(reg_addr);
    I2C_ACK();                      

	I2C_Start();

    I2C_Clock_Data(dev_addr+1);
    I2C_ACK();                      

    I2CSDAPort_Input();
	I2C_Delay();

	if(GPIO_ReadInputDataBit(GPIOB, SDA)) data |= 0x80;
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

void SII9293RegBitsSet ( uint8_t dev_addr, uint8_t reg_addr, uint8_t bitMask, int setBits )
{
    uint8_t aByte;

    aByte = SII9293Read( dev_addr, reg_addr);
    aByte = (setBits) ? (aByte | bitMask) : (aByte & ~bitMask);
    SII9293Write( dev_addr, reg_addr, aByte );
}

void SII9293RegWriteWord(uint8_t dev_addr, uint8_t reg_addr, uint16_t value)
{
	uint8_t buffer[2];
	buffer[0] = (uint8_t) value;
	buffer[1] = (uint8_t) (value >> 8);
	SII9293WriteBlock(dev_addr, reg_addr, buffer, 2);
}

void SII9293RegModify ( uint8_t dev_addr, uint8_t reg_addr,  uint8_t mask, uint8_t value)
{
    uint8_t aByte;

    aByte = SII9293Read( dev_addr, reg_addr);
    aByte &= (~mask);                       // first clear all bits in mask
    aByte |= (mask & value);                // then set bits from value
    SII9293Write( dev_addr, reg_addr, aByte );
}

//0x74 : HDCP Operation (0~ 0xFF)
//0x68:  Audio Clock Generation (0x00 ~ 0x18)
//0x68:  Audio Out (0x26 ~ 0x39)
//0x68: Power Down Control(0x3c ~ 0x3F)
//0x64: Power Down Control(0x08[0])
//0x68: Packet Contents and Control(0x40 ~ 0xFF)
//0x80: CEC Bus Interface Control(0x00 ~ 0x0C)
//0xD0: GPIO Control
//0xD0: Receiver Control(0x6C ~ 0x70)
//0xC0: MHL and CBUS Control (0x00 ~ 0xFF)

//SIL9293칩의 ID를 읽는 함수
void RXID_Read(void)
{
	uint8_t	vendor_lo=0;
	uint8_t	vendor_hi=0;
	uint8_t	device_lo=0;
	uint8_t	device_hi=0;
	uint16_t	vendor_id=0, device_id=0;
	
	vendor_lo = SII9293Read(0x64, 0x00);
	vendor_hi = SII9293Read(0x64, 0x01);

	//Dprintf("VID_LO 6x64::0x00 = %02x\n", vendor_lo);
	device_lo = SII9293Read(0x64, 0x02);
	device_hi = SII9293Read(0x64, 0x03);

	vendor_id = (vendor_hi << 8 ) | vendor_lo;
	device_id = (device_hi << 8 ) | device_lo;
	Dprintf("\tRX Chip VENDOR ID = %04x DEVICE ID= %04x\n", vendor_id, device_id);	
}

void RXSystemStatus(void)
{
	uint8_t	status=0;
	status = SII9293Read(0x64, 0x06);
	Dprintf("SiI9293 HDMI Signal Detect Check & Setting Information\n");
	Dprintf("------------------------------------------------------------------------\n");
	if(status & 0x10) Dprintf("\tPCLK_STABLE=1\n");
	else  Dprintf("\tPCLK_STABLE=0\n");
	if(status&0x08) Dprintf("\tHDMI+5V detect\n");
	else Dprintf("\tHDMI+5V not detect\n");
	if(status&0x04) Dprintf("\tVsysnc detect\n");
	else Dprintf("\tVsync not detect\n");
	if(status&0x02) Dprintf("\tClock detect\n");
	else Dprintf("\tno clock detect\n");
	if(status&0x01) Dprintf("\tDE clock detect\n");
	else Dprintf("\tNo DE clock detect\n");
	//
	status = SII9293Read(0x64, 0x08);
	if(status & 0x04)	Dprintf("\n\tVideo Bus Mode=24/30/36 Bit Mode\n");
	else Dprintf("\tVideo Bus Mode=12/15/18-bit mode (maximum 75 MHz)\n");
	if(status & 0x02) Dprintf("\tOut Clock Edge = Invert the output clock\n");
	else Dprintf("\tOut Clock Edge = Normal output clock\n");
	//
	status = SII9293Read(0x64, 0x48);
	if(status&0x80) Dprintf("\tVSync = Invert the VSYNC signal\n");
	else Dprintf("\tVSync = Do not invert the VSYNC signal\n");

	if(status&0x40) Dprintf("\tHSync = Invert the HSYNC signal\n");
	else Dprintf("\tHSync = Do not invert the HSYNC signal\n");
	
	if(status&0x02) Dprintf("\tBITM = 1\n");
	else Dprintf("\tBITM = 0\n");	
	Dprintf("------------------------------------------------------------------------\n");
}

//HDMI +5V 가 검지 되었는지를 조사한다.
int PowerCheck(void)
{
	uint8_t	status=0;
	status = SII9293Read(0x64, 0x06);
	if(status & 0x08)	return(1);
	else return(0);
}

void RXHotPlug(char *data)
{
	uint8_t cntl = StrToHex(data);
	SII9293Write(0x64, 0x0b, cntl);
	if(cntl ==0) Dprintf("\tRX HPD Pin Low Set\n");
	else if(cntl==1)  Dprintf("\tRX HPD Pin High Set\n");
}


void HS_Setting(char *str)
{
	uint8_t data = SII9293Read(0x64, 0x48);
	if(*str == '+') 
	{
		data &= ~ 0x40;
		Dprintf("Hsync Clock =  Do not invert the HSYNC signal Set\n");
	}
	else if(*str == '-') 
	{
		data |= 0x40;
		Dprintf("Hsync Clock =  Invert the HSYNC signal Set\n");
	}
	SII9293Write(0x64, 0x48, data);		
}

void VS_Setting(char *str)
{
	uint8_t data = SII9293Read(0x64, 0x48);
	if(*str == '+') 
	{
		data &= ~ 0x80;
		Dprintf("Vsync Clock =  Do not invert the VSYNC signal Set\n");
	}
	else if(*str == '-') 
	{
		data |= 0x80;
		Dprintf("Vsync Clock =  Invert the VSYNC signal Set\n");
	}
	SII9293Write(0x64, 0x48, data);		
}


void CLK_Setting(char *str)
{
	uint8_t data = SII9293Read(0x64, 0x08);
	if(*str == '+') 
	{
		data &= ~ 0x02;
		Dprintf("Output Clock =  Normal output clock Set\n");
	}
	else if(*str == '-') 
	{
		data |= 0x02;
		Dprintf("Output Clock =  Invert the output clock Set\n");
	}
	SII9293Write(0x64, 0x08, data);		
}

void 	EDID_BSP_I2C_PortInit(void);
void ITU656Setting(void);

void  SII9293_Test(void)
{
	//RX Reset Port, Interrupt Port, CEC Port Init
	HDMI_PortInit();
	// SII9293을 리셋한다.
	HDMI_Chip_Reset();
	Delay(200);	
	
	Dprintf("HPD Enable\n");
	SII9293Write(0x64, 0x0b, 0x01);
	
	Dprintf("RXID Read Start\n");
	RXID_Read();
	Dprintf("SiI9293 Chip Init\n");
	//init_rx();	
	ITU656Setting();
	
	HDMI_Shell();	
}







