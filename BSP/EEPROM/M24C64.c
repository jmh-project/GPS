#include "Xmicom.h"

//*************************************************************************************************************
// 포트 제어 24c512
//*************************************************************************************************************
#define		SDA			GPIO_Pin_9
#define		SCL			GPIO_Pin_6

void FM24c256_Init(void)
{
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SDA | SCL;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIOB->BSRRL = SDA | SCL;

}
 void SCLPort_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = SDA | SCL;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}
void SCLPort_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SDAPort_Input(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void SDAPort_Output(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


#define   DELAY   Delay_for

#define		EPROM_SDA	GPIO_Pin_6	
#define     EPROM_SCL	GPIO_Pin_5

void Delay_for( unsigned long int time )
{
    time *= 20;
    while( time-- );
}
void EEP_Start(void) //SCL이 H 일때  SDA를 H 에서 L 로 변화 하는 것을 스타트 컨디션 이라고 한다.
{
    SCLPort_Output();
    SDAPort_Output();

    GPIO_SetBits(GPIOB,EPROM_SDA);
    GPIO_SetBits(GPIOB,EPROM_SCL);
    DELAY( 1 );

    GPIO_ResetBits(GPIOB,EPROM_SDA);
    DELAY( 3 );

    GPIO_ResetBits(GPIOB,EPROM_SCL);
    DELAY( 1 );
}
void EEP_Stop(void)// SCL이 H 일때 SDA를 L에서 H로 변화하는 것을 스톱 컨디션 이라고 한다.
{
    SCLPort_Output();
    SDAPort_Output();

    GPIO_ResetBits(GPIOB,EPROM_SDA);
    GPIO_ResetBits(GPIOB,EPROM_SCL);
    GPIO_SetBits(GPIOB,EPROM_SCL);
    DELAY( 2 );
    GPIO_SetBits(GPIOB,EPROM_SDA);
    DELAY( 2 );
}
void EEP_Clock_Data( unsigned char data)
{
    int i;

    for(i=0;i<8;i++)
    {
       	if(data & 0x80) GPIO_SetBits(GPIOB,EPROM_SDA); // SCL이 L 일때 SDA( 데이터 ) 를 변경해야만 한다. //SCL 이  H 일때 SDA( 데이터 ) 를 변경하는 것은 의미가없다.
       	else  GPIO_ResetBits(GPIOB,EPROM_SDA);             // 최상위 비트부터 데이터를 1비트씩 8비트를 전송한다.
        DELAY( 1 );
        GPIO_SetBits(GPIOB,EPROM_SCL);
        DELAY( 4 );
        GPIO_ResetBits(GPIOB,EPROM_SCL);
        DELAY( 1 );
        data <<= 1;               //비트 쉬프트
    }//SCL.L //여기서 8개클럭을 마무리하고
}
void EEP_ACK(void)
{
    SDAPort_Input();
    GPIO_SetBits(GPIOB,EPROM_SCL);//9번째 클럭
    DELAY(3);
    //while(!(P3IN & 0x02)); //SDA 핀이 H 를 유지할때 까지 기다린다.
    while(GPIO_ReadInputDataBit(GPIOB,EPROM_SDA)); //SDA 핀이 L 를 유지할때 까지 기다린다.
    GPIO_ResetBits(GPIOB,EPROM_SCL);
    //SCLPort_Input();
    //while((P3IN & 0x01)==0x00);//슬레이브 송신기가 컨트롤 바이트를 수신하고 나서 데이터 바이트를 송신할때 까지 처리시간이 필요할 경우 슬레이브 송신기는 SCL을 L로 유지한다.

    //SCLPort_Output();
    SDAPort_Output();
    //GPIO_ResetBits(GPIOB,GPIO_Pin_13);

}
void EEP_Write( unsigned int addr, unsigned char data)
{
    EEP_Start();
    EEP_Clock_Data(0xa0);           //Device address & R/w bit is low

    EEP_ACK();                      ////ACK.Signal 을 받는다.

    EEP_Clock_Data(addr>>8);        //EEPROM.High.Address.Send	

    EEP_ACK();                      ////ACK.Signal 을 받는다.

    EEP_Clock_Data(addr);           //EEPROM.Low.Address.Send	

	EEP_ACK();                      ////ACK.Signal 을 받는다.

    EEP_Clock_Data(data);           //Data.Send	

    EEP_ACK();                      ////ACK.Signal 을 받는다.

    EEP_Stop();                     //STOP
}

unsigned char EEP_Read( unsigned int addr )
{
    int i=0;
    unsigned char data=0x00;
    unsigned char sda_input=0x00;

    EEP_Start();
    EEP_Clock_Data(0xa0);           //Device address & R/w bit is low

    EEP_ACK();                      ////ACK.Signal 을 받는다.

    EEP_Clock_Data(addr>>8);        //EEPROM.High.Address.Send	

    EEP_ACK();                      ////ACK.Signal 을 받는다.

    EEP_Clock_Data(addr);           //EEPROM.Low.Address.Send	

    EEP_ACK();                      ////ACK.Signal 을 받는다.

	EEP_Start();

    EEP_Clock_Data(0xa1);           //Device address & R/w bit is READ

    EEP_ACK();                      ////ACK.Signal 을 받는다.

    SDAPort_Input();
    //DELAY( 1 );
    for(i=0;i<8;i++)
    {
        //sda_input = P3IN & 0x02;
        if(GPIO_ReadInputDataBit(GPIOB,EPROM_SDA))
        {
            data |= 0x01;
        }
        if(i<7)data <<= 1;

        GPIO_SetBits(GPIOB,EPROM_SCL);
        DELAY( 1 );
        GPIO_ResetBits(GPIOB,EPROM_SCL);
        DELAY( 1 );
    }

    EEP_Stop();
    return ( data );
}
int AT24C512_DataToWrite(void *s, int size, u16 mem_addr)
{
	u32		addr;
	u16		f_addr = mem_addr;
	int		leng;
 
	addr =(u32)s;		//시작주소
	leng=size;
	Uart_Printf1("Mem size=%d\n", size);
	//Uart_SendByte0('1');
	while(leng--)		//쓰기.
	{		
		EEP_Write(f_addr++,  *(u8 *)(addr++));
		Delay(10);		//지연 시간이 중요하다 이시간이 짧으면 중간에 빼먹고 저장하는 일들이 발생된다. 현재지연시간=4.7msec이다.
		//Uart_SendByte1('1');
	}

	addr = (u32)s;
	leng=size;
	f_addr = mem_addr;
	/*
	while(leng--)
	{
	//Uart_SendByte0('2');
		if(*(u8 *)(addr++) != EEP_Read(f_addr++) )
		{
			return(0);
		}
		Delay(1);		//지연 시간이 중요하다 이시간이 짧으면 중간에 빼먹고 저장하는 일들이 발생된다.
	}
	*/
	//Uart_SendByte0('3');

	return (1);
}

int AT24C512_DataToRead(void *s, int size, u16 mem_addr)
{
	u32		addr;
	u16		f_addr = mem_addr;

	addr = (u32)s;
	
	while(size--)
	{
		*(u8 *)(addr++) = EEP_Read(f_addr++);
		Delay(1);		//지연 시간이 중요하다 이시간이 짧으면 중간에 빼먹고 저장하는 일들이 발생된다.
	}	
	//Uart_Printf0("EndAddress=%d\n",f_addr);
	return (1);
}
