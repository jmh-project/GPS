/******************************************************************************
							Include File
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

#include "TEECO_System.h"
#ifdef W5500
#include "BSP_W5500.h"

/******************************************************************************
							Constant & Macros
******************************************************************************/


/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/
//W5500 Interrupt
/*
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{		
		EXTI_ClearITPendingBit(EXTI_Line1);        
    }
}
*/

//PC1 W5500 Interrupt
void EXTILine1_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Enable GPIOG clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Configure PG15 pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Connect EXTI Line1 to PB1 pin */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
	
	/* Configure EXTI Line1 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI15_10 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

//PA5: SPI1_CLK
//PA6: SPI1_MISO
//PA7: SPI1_MOSI
void LAN_SPI1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;    

    /* Peripheral Clock Enable -------------------------------------------------*/
    /* Enable the SPI clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    /* GPIOG Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /* SPI GPIO Configuration --------------------------------------------------*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;

    /* SPI SCK pin configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;   //PA5
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI  MOSI pin configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;  //PA7
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //MISO Pin
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;  //PA6
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* Connect SPI pins to AF5 */  
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

    /* SPI configuration -------------------------------------------------------*/
    SPI_I2S_DeInit(SPI1);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // [84MHZ / 128 = ]
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;

  	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  	SPI_Init(SPI1, &SPI_InitStructure);
    /* The Data transfer is performed in the SPI interrupt routine */
  	/* Enable the SPI peripheral */
  	SPI_Cmd(SPI1, ENABLE);  
}

uint8_t LAN_SPI1_Send(uint8_t data)
{
    uint8_t rdata;
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));
    SPI_I2S_SendData(SPI1, data);           
    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)));

    while(!(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)));                    
    rdata = SPI_I2S_ReceiveData(SPI1);   
    return(rdata);    
}

void LAN_SPI1_CS(int s)
{
    if(s==1)
    {
        //SPI_CS ->High
        GPIOA->BSRRL = GPIO_Pin_4;        
    }
    else
    {
        //SPI_CS ->Low
        GPIOA->BSRRH = GPIO_Pin_4;        
    }
}

void W5500_Reset(void)
{	
	GPIOC->BSRRH =GPIO_Pin_4;
	Delay(10);
	GPIOC->BSRRL =GPIO_Pin_4;
	Delay(100);
}	

void BSP_W5500_Init(void)
{
    GPIO_InitTypeDef   GPIO_InitStructure;
    /* GPIOG Peripheral clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIOA->BSRRL =GPIO_Pin_4;	//CS High		

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	LAN_SPI1_Config();		//SPI
	//EXTILine1_Config();
	W5500_Reset	();
}

/*
void W5500_RW_Test(void)
{
	uint8_t	data=0;
	uint8_t	data1=0;

	LAN_SPI1_CS(0);
	//Address Out
	LAN_SPI1_Send(0x00);
	LAN_SPI1_Send(0x18);
	//Control Out: RWB = 1, OM=00
	LAN_SPI1_Send(COMMON_REG | WRITE_MODE | VDM);
	LAN_SPI1_Send(0x57);
	LAN_SPI1_Send(0x89);
	LAN_SPI1_CS(1);
	

	LAN_SPI1_CS(0);
	//Address Out
	LAN_SPI1_Send(0x00);
	LAN_SPI1_Send(0x18);

	LAN_SPI1_Send(COMMON_REG | READ_MODE | VDM);
	data = LAN_SPI1_Send(0x00);
	data1 = LAN_SPI1_Send(0x00);
	LAN_SPI1_CS(1);
	
	Dprintf("Data = %02x %02x\n", data, data1);
}
*/

void WriteSingle_W5500_Reg(uint16_t addr, uint8_t bank, uint8_t *data, int length)
{
	LAN_SPI1_CS(0);
	//Address Out
	LAN_SPI1_Send((uint8_t)((addr >> 8) & 0x00ff));
	LAN_SPI1_Send((uint8_t)(addr & 0x00ff));	
	LAN_SPI1_Send(bank | WRITE_MODE | VDM);
	for(int i=0; i<length; i++)
	{
		LAN_SPI1_Send(data[i]);
	}
	LAN_SPI1_CS(1);
}

void Write_W5500_Reg(uint16_t addr, uint8_t bank, uint8_t *data, int length)
{
	LAN_SPI1_CS(0);
	//Address Out
	LAN_SPI1_Send((uint8_t)((addr >> 8) & 0x00ff));
	LAN_SPI1_Send((uint8_t)(addr & 0x00ff));	
	LAN_SPI1_Send(bank | WRITE_MODE | VDM);
	for(int i=0; i<length; i++)
	{
		LAN_SPI1_Send(data[i]);
	}
	LAN_SPI1_CS(1);
}

void Read_W5500_Reg(uint16_t addr, uint8_t bank, uint8_t *data, int length)
{
	LAN_SPI1_CS(0);
	//Address Out
	LAN_SPI1_Send((uint8_t)((addr >> 8) & 0x00ff));
	LAN_SPI1_Send((uint8_t)(addr & 0x00ff));	
	LAN_SPI1_Send(bank | READ_MODE | VDM);
	for(int i=0; i<length; i++)
	{
		data[i] = LAN_SPI1_Send(0x00);
	}
	LAN_SPI1_CS(1);
}

//내부레지스터를 건들여 리셋명령을 실행한다.
void W5500_SoftReset(void)
{
	uint8_t	data[12];
	data[0] = 0x80;
	Write_W5500_Reg(0x0000, COMMON_REG, data, 1);
}

//인터럽트 연속대기시간설정
void W5500_IntlevelTime(void)
{
	uint8_t	data[12];
	data[0] = 0x00;
	data[1] = 0x04;
	Write_W5500_Reg(0x0013, COMMON_REG, data, 2);
}


//게이트웨이를 설정한다.
void W5500_Set_Gateway(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	uint8_t	data[12];
	data[0] = a; data[1] = b; data[2] = c; data[3] = d;
	Write_W5500_Reg(0x0001, COMMON_REG, data, 4);	
}

void W5500_Get_Gateway(uint8_t *data)
{
	Read_W5500_Reg(0x0001, COMMON_REG, data, 4);		
}

//서브넷마스트를 설정한다.
void W5500_Set_SubnetMask(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	uint8_t	data[12];
	data[0] = a; data[1] = b; data[2] = c; data[3] = d;
	Write_W5500_Reg(0x0005, COMMON_REG, data, 4);	
}

void W5500_Get_SubnetMask(uint8_t *data)
{
	Read_W5500_Reg(0x0005, COMMON_REG, data, 4);		
}

//MAC 어드레스를 설정한다.
void W5500_Set_MacAddr(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f)
{
	uint8_t	data[12];
	data[0] = a; data[1] = b; data[2] = c; data[3] = d; data[4] = e; data[5] = f; 
	Write_W5500_Reg(0x0009, COMMON_REG, data, 6);	
}

void W5500_Get_MacAddr(uint8_t *data)
{
	Read_W5500_Reg(0x0009, COMMON_REG, data, 6);		
}

//Source IP를 설정한다.
void W5500_Set_IP(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	uint8_t	data[12];
	data[0] = a; data[1] = b; data[2] = c; data[3] = d;
	Write_W5500_Reg(0x000F, COMMON_REG, data, 4);	
}

void W5500_Get_IP(uint8_t *data)
{
	Read_W5500_Reg(0x000F, COMMON_REG, data, 4);		
}

/*
void W5500_Test_Reg_RW(void)
{
	uint16_t	addr;
	uint8_t	data[32];

	Dprintf("Addr Input:");
	addr = DgetIntNum();
	Dprintf("\n Data:");
	data[0] = DgetIntNum();
	Dprintf("\nADDR=%04x Data=%02x\n", addr, data[0]);
	Write_W5500_Reg(addr, COMMON_REG, data, 1);
	Read_W5500_Reg(addr, COMMON_REG, data, 1);
	Dprintf("Verify=%02x\n", data[0]);
}
*/

void LAN_W5500_Test(void)
{
//	uint16_t	addr;
	uint8_t	data[32];
	Dprintf("LAN W5500 Test Start.......\n");
	
	BSP_W5500_Init();
	W5500_SoftReset();
	Delay(10);
	
	W5500_IntlevelTime();
	
	W5500_Set_Gateway(192, 168, 0, 1);
	W5500_Get_Gateway(data);
	Dprintf("Gateway = %d.%d.%d.%d\n", data[0], data[1], data[2], data[3]);

	W5500_Set_SubnetMask(255, 255, 0, 0);
	W5500_Get_SubnetMask(data);
	Dprintf("SubnetMask = %d.%d.%d.%d\n", data[0], data[1], data[2], data[3]);
	
	W5500_Set_MacAddr(0x00, 0xd8, 0xa9, 0xf7, 0x23, 0x11);
	W5500_Get_MacAddr(data);
	Dprintf("MAC Addr = %02x.%02x.%02x.%02x.%02x.%02x\n", data[0], data[1], data[2], data[3], data[4], data[5]);
	
	W5500_Set_IP(192, 168, 0, 11);
	W5500_Get_IP(data);
	Dprintf("Source IP = %d.%d.%d.%d\n", data[0], data[1], data[2], data[3]);
	
	Read_W5500_Reg(0x0039, COMMON_REG, data, 1);
	Dprintf("Chip Version=%d %02x\n", data[0], data[0]);
	while(1)
	{
	}
}
#endif
#ifdef __cplusplus
}
#endif
