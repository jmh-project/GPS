/******************************************************************************
*    	File name	: main.c 
*		Description : Entry main File
*		Company		: idnics co.,
*		Version		: 1.00
*		Date		: 2012 / 06 / 19
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifdef __cplusplus
 extern "C" {
#endif

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"

 
/******************************************************************************
							Constant & Macros
******************************************************************************/

/******************************************************************************
							Private & Local Variables
******************************************************************************/

/******************************************************************************
							Function Prototype
******************************************************************************/

     //I2C 엔진을 사용하지 않고 Port로 I2C 를 구현할때 사용하고자 만들었다.
void BSP_I2C_PortInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  	GPIO_InitStructure.GPIO_Pin = SDA | SCL;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIOB->BSRRL = SDA | SCL;
}
#if 1
void I2C_Delay(void)
{
	int s=20;
	while(s--);
	//Delay(1);
}


//I2C Data Port는 입력과 출력이 되어야하므로 입력으로 전환 시킬때 사용하는 함수이다.
void I2CSDAPort_Input(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin =  SDA;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

//I2C Data Port는 입력과 출력이 되어야하므로 출력으로 전환 시킬때 사용하는 함수이다.
void I2CSDAPort_Output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  	GPIO_InitStructure.GPIO_Pin =  SDA;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//I2C Start 시그널발생
void I2C_Start(void)
{
    I2CSDAPort_Output();
	SDA_High();
	SCL_High();	
	I2C_Delay();

	SDA_Low();	I2C_Delay();	I2C_Delay();
	SCL_Low();	I2C_Delay();
}

//I2C Stop 시그널발생
void I2C_Stop(void)
{
    I2CSDAPort_Output();
	SDA_Low();		
	SCL_Low();
			
	SCL_High();	I2C_Delay(); I2C_Delay();
	SDA_High();	I2C_Delay(); I2C_Delay();
}

void I2C_Clk(void)
{
	SCL_High();	I2C_Delay();
	SCL_Low();	I2C_Delay();
}

void I2C_Clock_Data( u8 data)
{
   	if(data & 0x80) GPIO_SetBits(GPIOB, SDA);
   	else  GPIO_ResetBits(GPIOB, SDA);
	I2C_Delay();
	I2C_Clk();
   	if(data & 0x40) GPIO_SetBits(GPIOB, SDA);
   	else  GPIO_ResetBits(GPIOB, SDA);
	I2C_Delay();
	I2C_Clk();
   	if(data & 0x20) GPIO_SetBits(GPIOB, SDA);
   	else  GPIO_ResetBits(GPIOB, SDA);
	I2C_Delay();
	I2C_Clk();
   	if(data & 0x10) GPIO_SetBits(GPIOB, SDA);
   	else  GPIO_ResetBits(GPIOB, SDA);
	I2C_Delay();
	I2C_Clk();
   	if(data & 0x08) GPIO_SetBits(GPIOB, SDA);
   	else  GPIO_ResetBits(GPIOB, SDA);
	I2C_Delay();
	I2C_Clk();
   	if(data & 0x04) GPIO_SetBits(GPIOB, SDA);
   	else  GPIO_ResetBits(GPIOB, SDA);
	I2C_Delay();
	I2C_Clk();
   	if(data & 0x02) GPIO_SetBits(GPIOB, SDA);
   	else  GPIO_ResetBits(GPIOB, SDA);
	I2C_Delay();
	I2C_Clk();
   	if(data & 0x01) GPIO_SetBits(GPIOB, SDA);
   	else  GPIO_ResetBits(GPIOB, SDA);
	I2C_Delay();
	I2C_Clk();
}

void I2C_ACK(void)
{
    I2CSDAPort_Input();
    
	I2C_Delay();
    
    GPIO_SetBits(GPIOB, SCL);//9번째 클럭
	
    I2C_Delay();
	
    I2C_Delay();

    while(GPIO_ReadInputDataBit(GPIOB, SDA));
	
    GPIO_ResetBits(GPIOB, SCL);

    I2CSDAPort_Output();
}

void I2C_ACK_M(void)
{
}
#endif
#ifdef __cplusplus
}
#endif


