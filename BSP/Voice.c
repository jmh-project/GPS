/******************************************************************************
							Include File
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif
#include "TEECO_System.h"

//!< GPIO 로 SPI를 구현하였다.
//보이스칩을 굽기위해서는 모든포트가 입력이 되어야한다.

void Voice_Port_All_In(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
//    DAC_InitTypeDef  DAC_InitStructure;

    /* GPIOG Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG, ENABLE);
    //==============================================================================
    //PC4:AUDIO_RESET
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //PA8:MISO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //PG11:MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
	
    //PB5: CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	//PF8: CLK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void Voice_Port_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
//    DAC_InitTypeDef  DAC_InitStructure;

    /* GPIOG Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOG, ENABLE);
    //==============================================================================
    //PC4:AUDIO_RESET
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIOC->BSRRL = GPIO_Pin_4;	//Reset High

    //PA8:MISO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //PG11:MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIOG->BSRRH = GPIO_Pin_11;	//DATA Out Low
	
    //PB5: CS
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIOB->BSRRL = GPIO_Pin_5;		//CS High

    //PF8: CLK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIOF->BSRRH = GPIO_Pin_8;	//CLK Low
}

void VoiceReset()
{
    GPIOC->BSRRH = GPIO_Pin_4;        
    Delay(1);
    GPIOC->BSRRL = GPIO_Pin_4;        
}

void Voice_SPI_CS(int s)
{
    if(s==1)
    {
        //SPI_CS ->High
        GPIOB->BSRRL = GPIO_Pin_5;        
    }
    else
    {
        //SPI_CS ->Low
        GPIOB->BSRRH = GPIO_Pin_5;        
    }	
}

void Voice_SPI_Clk(void)
{
	GPIOF->BSRRL = GPIO_Pin_8; //High
	for(int i=0; i<100; i++);
	GPIOF->BSRRH = GPIO_Pin_8; //Low
	for(int i=0; i<100; i++);
}

void Voice_SPI_Data(int s)
{
	if(s==1)
		GPIOG->BSRRL = GPIO_Pin_11; //High
	else
		GPIOG->BSRRH = GPIO_Pin_11; //Low
}

uint8_t Voice_SPI_Send(uint8_t data)
{

	if(data  & 0x80) Voice_SPI_Data(1);
	else Voice_SPI_Data(0);
	Voice_SPI_Clk();
	if(data  & 0x40) Voice_SPI_Data(1);
	else Voice_SPI_Data(0);
	Voice_SPI_Clk();
	if(data  & 0x20) Voice_SPI_Data(1);
	else Voice_SPI_Data(0);
	Voice_SPI_Clk();
	if(data  & 0x10) Voice_SPI_Data(1);
	else Voice_SPI_Data(0);
	Voice_SPI_Clk();
	if(data  & 0x08) Voice_SPI_Data(1);
	else Voice_SPI_Data(0);
	Voice_SPI_Clk();
	if(data  & 0x04) Voice_SPI_Data(1);
	else Voice_SPI_Data(0);
	Voice_SPI_Clk();
	if(data  & 0x02) Voice_SPI_Data(1);
	else Voice_SPI_Data(0);
	Voice_SPI_Clk();
	if(data  & 0x01) Voice_SPI_Data(1);
	else Voice_SPI_Data(0);
	Voice_SPI_Clk();
	
	return(1);
}

void VoiceDelay(__IO uint32_t nTime) //1 ->100uSec
{ 

	//1 = 100us
	//2 = 202us
	//3 = 304us
	//4 = 404us
	//5 = 504us
	//6 = 604us 
	//7 = 700us
	//8 = 800us
	//9 = 900us
	//10 = 1010us
	int i;
	int count = 4240 * nTime;
	
	for(i=0; i <count; i++);	
}

void VoiceChipReset(void)
{
	GPIOC->BSRRH =GPIO_Pin_4; //Low
	Delay(1);
	GPIOC->BSRRL =GPIO_Pin_4; //High
}

#define     DELAY500    9

void Voice_Init(void)
{
    //D Command
     Voice_SPI_CS(0);      VoiceDelay(1);   uint8_t data1 = Voice_SPI_Send(0x44);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x15); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x00);    VoiceDelay(DELAY500);   Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //K Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x4B);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0xFE); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0xE1);    VoiceDelay(DELAY500);   Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //O Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x4F);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x84); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0xFF);    VoiceDelay(DELAY500);    Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //V Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x56);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x06); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x00);    VoiceDelay(DELAY500);   Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //M Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x4D);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x7F); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x00);    VoiceDelay(DELAY500);    Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //N Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x4E);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x7F); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x00);    VoiceDelay(DELAY500);   Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //O Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x4F);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x08); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x02);    VoiceDelay(DELAY500);    Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //O Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x4F);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0xD9); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x02);    VoiceDelay(DELAY500);    Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //C Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x43);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x05); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x00);    VoiceDelay(DELAY500);   Voice_SPI_CS(1);    VoiceDelay(DELAY500);
    //C Command
     Voice_SPI_CS(0);      VoiceDelay(1);   data1 = Voice_SPI_Send(0x48);    VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x01); VoiceDelay(DELAY500);   data1 = Voice_SPI_Send(0x00);    VoiceDelay(DELAY500);   Voice_SPI_CS(1);    VoiceDelay(DELAY500);
}

void VoicePlay(uint8_t bank)
{
    Voice_SPI_CS(0);      
    VoiceDelay(1);   
    uint8_t data1 = Voice_SPI_Send(0x50);    
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(bank); 
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(0x00);    
    VoiceDelay(DELAY500);   
    Voice_SPI_CS(1);    
}

//현재 재생 중인 음성을 정지합니다.
void SpeexVoiceStop()
{
    Voice_SPI_CS(0);      
    VoiceDelay(1);   
    uint8_t  data1 = Voice_SPI_Send(0x53);    
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(0x01); 
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(0x00);    
    VoiceDelay(DELAY500);   
    Voice_SPI_CS(1);       
}

//현재 재생 중인 음성을 정지합니다.
void ADPCMVoiceStop()
{
    Voice_SPI_CS(0);      
    VoiceDelay(1);   
    uint8_t data1 = Voice_SPI_Send(0x53);    
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(0x02); 
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(0x00);    
    VoiceDelay(DELAY500);   
    Voice_SPI_CS(1);       
}

//현재 재생 중 이거나 예약된 모든 음성 재생을 취소합니다.
void VoiceStop(void)
{
    Voice_SPI_CS(0);      
    VoiceDelay(1);   
    uint8_t data1 = Voice_SPI_Send(0x58);    
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(0x00); 
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(0x00);    
    VoiceDelay(DELAY500);   
    Voice_SPI_CS(1);       
}

//볼륨을 조정한다.
void VoiceVolume(uint8_t vol)
{
	if(vol > 128) vol = 128;
    Voice_SPI_CS(0);      
    VoiceDelay(1);   
    uint8_t data1 = Voice_SPI_Send(0x4E);    
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(vol); 
    VoiceDelay(DELAY500);   
    data1 = Voice_SPI_Send(0x00);    
    VoiceDelay(DELAY500);   
    Voice_SPI_CS(1);       
}

void VoiceTest_Help(void)
{
	Dprintf("=========Voice Chip Test==========\n");
	Dprintf("1. Audio Play\n");
	Dprintf("2. Audio Stop\n");
	
}

void VoiceTest(void)
{
	char	sd;
	int	num=0;
	
	Voice_Port_All_In();
	Dprintf("Sound Data Writing\n");
	
	Voice_Port_Init();
	VoiceChipReset();
	Delay(10);
	Voice_Init();

	VoiceTest_Help();
	
	while(1)
	{
		sd = Dgetch();
		if(sd==' ')
		{
			VoicePlay(8);
			Dprintf("Play Sound\n");
		}
		else if(sd=='1')
		{
			Dprintf("\n재생할 뱅크번호를 입력하시오:");
			num = DgetIntNum();
			VoicePlay(num);
			Dprintf("\n");
		}
		else if(sd=='2')
		{
			Dprintf("Audio Stop Command\n");
			VoiceStop();
		}
		else
			VoiceTest_Help();
	}	    
}

#ifdef __cplusplus
}
#endif
