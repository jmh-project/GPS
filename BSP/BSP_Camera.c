#include "TEECO_System.h"
//#include "Cell01_Struct.h"
#if 0
#define VIDEO_CAMERA_VIEW_X     800
#define VIDEO_CAMERA_VIEW_Y     600

void OC5642_Sensor_RAW_Setting( void );
void OC5642_Processor_RAW_Setting( void );
void VGA_640_480_PreView( void );
void QVGA_320_240_PreView( void );
void SVGA_PreView( void );
void QCIG_PreView( void );
void  CIF_PreView( void );
void QSXGA_to_vga640_480( void );
void QSXGA_to_sxga1280_960( void );
void QSXGA_to_QVGA320_240( void );
void QSXGA_to_qxga2048_1536( void );
void QSXGA_to_uxga1600_1200( void );
void QSXGA_to_xga1024_768( void );
void QSXGA_Capture( void );
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//#define VERIFY_WRITTEN_DATA           //Read에서 베리파이를 할지여부를 지정한다.
#define CAMERA_FLAG_TIMEOUT             ((uint32_t)0x1000)
#define CAMERA_LONG_TIMEOUT             ((uint32_t)(300 * CAMERA_FLAG_TIMEOUT))

#define DCMI_TIMEOUT_MAX                CAMERA_LONG_TIMEOUT

/* The 7 bits Codec address (sent through I2C interface) */
#define CAMERA_CODEC_ADDRESS             0x78
#define CAMERA_I2C_SPEED                     30000 //Hz

__IO uint32_t  CAMERATimeout = CAMERA_LONG_TIMEOUT; 

uint32_t Camera_ReadRegister(uint16_t RegisterAddr);
uint32_t Camera_WriteRegister(uint16_t RegisterAddr, uint8_t RegisterValue);

unsigned short int gConversionData[100][100];
extern volatile U32	Graphics_Address;
extern volatile U16 	g_Color;
int Memory_Bitblt2(uint32_t src_addr, uint32_t dest_addr, uint32_t sx, uint32_t sy, uint32_t xlen,uint32_t ylen, uint32_t srcWidth, uint32_t destWidth );
volatile int	gStartXH, gStartXL, gStartYH,gStartYL, gAddPoint,gxSize,gySize;
void High_Resolution_Video_720P( void );
void OV5642_Setting(void);

void Camera_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;
  
    /* Enable I2C GPIO clocks */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    /* CODEC_I2C SCL and SDA pins configuration -------------------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);     
    /* Connect pins to I2C peripheral */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);  
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);  
    
    /* CODEC_I2C peripheral configuration */
    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x33;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = CAMERA_I2C_SPEED;

    I2C_Init(I2C1, &I2C_InitStructure);    
    /* Enable the I2C peripheral */
    I2C_Cmd(I2C1, ENABLE);  
}

uint32_t Camera_TIMEOUT_UserCallback(void)
{
    Dprintf("CAMERA I2C TIMEOUT CALLBACK\n");
    while (1);
}


uint32_t Camera_WriteRegister(uint16_t RegisterAddr, uint8_t RegisterValue)
{
    uint32_t result = 0;
    uint8_t  reg_addr_hi=(uint8_t)((RegisterAddr>>8) & 0x00ff); 
    uint8_t  reg_addr_lo=(uint8_t)(RegisterAddr & 0x00ff); 

/*
    CAMERATimeout = CAMERA_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
    {
        if((CAMERATimeout--) == 0) 
        {
            Dprintf("Camera_WriteRegister::#1\n");
            return Camera_TIMEOUT_UserCallback();
        }
    }
*/    
    /* Start the config sequence */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((CAMERATimeout--) == 0)         
        {
            Dprintf("Camera_WriteRegister::#2\n");
            return Camera_TIMEOUT_UserCallback();
        }
    }
  
    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(I2C1, CAMERA_CODEC_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((CAMERATimeout--) == 0)         
        {
            Dprintf("Camera_WriteRegister::#3\n");
            return Camera_TIMEOUT_UserCallback();
        }
    }

    /* Transmit the first address for write operation */
    I2C_SendData(I2C1, reg_addr_hi);
    /* Test on EV8 and clear it */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if((CAMERATimeout--) == 0)         
        {
            Dprintf("Camera_WriteRegister::#4\n");
            return Camera_TIMEOUT_UserCallback();
        }
    }

    /* Transmit the first address for write operation */
    I2C_SendData(I2C1, reg_addr_lo);
    /* Test on EV8 and clear it */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    {
        if((CAMERATimeout--) == 0)         
        {
            Dprintf("Camera_WriteRegister::#5\n");
            return Camera_TIMEOUT_UserCallback();
        }
    }
    
    /* Prepare the register value to be sent */
    I2C_SendData(I2C1, RegisterValue);
  
    /*!< Wait till all data have been physically transferred on the bus */
    CAMERATimeout = CAMERA_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF))   //I2C_FLAG_BTF::Byte transfer finished flag
    {
        if((CAMERATimeout--) == 0)         
        {
            Dprintf("Camera_WriteRegister::#6\n");
            return Camera_TIMEOUT_UserCallback();
        }
    }
  
    /* End the configuration sequence */
    I2C_GenerateSTOP(I2C1, ENABLE);  
  
#ifdef VERIFY_WRITTEN_DATA
    /* Verify that the data has been correctly written */  
    result = (Camera_ReadRegister(RegisterAddr) == RegisterValue)? 0:1;
#endif /* VERIFY_WRITTENDATA */

    /* Return the verifying value: 0 (Passed) or 1 (Failed) */
    return result;  
}

uint32_t Camera_ReadRegister(uint16_t RegisterAddr)
{
    uint32_t result = 0;
    uint8_t  reg_addr_hi=(uint8_t)((RegisterAddr>>8) & 0x00ff); 
    uint8_t  reg_addr_lo=(uint8_t)(RegisterAddr & 0x00ff); 

    /*I2C 버스가 사용중일때는 기다린다. */
/*
    CAMERATimeout = CAMERA_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)) //I2C BUS가 대기중인지 체크한다.
    {
        if((CAMERATimeout--) == 0) 
        {
            Dprintf("Camera_ReadRegister:: I2C BUS Busy\n");
            return Camera_TIMEOUT_UserCallback();
        }
    }
*/  
    /* I2C Start Signal을 출력한다. */
    I2C_GenerateSTART(I2C1, ENABLE);

    /* Test on EV5 and clear it */
    //I2C Mode가 마스터모드로 설정되었는지를 체크한다.
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((CAMERATimeout--) == 0) return Camera_TIMEOUT_UserCallback();
    }
  
    //I2C 디바이스의 어드레스를 출력한다.
    /* Transmit the slave address and enable writing operation */
    I2C_Send7bitAddress(I2C1, CAMERA_CODEC_ADDRESS, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((CAMERATimeout--) == 0) return Camera_TIMEOUT_UserCallback();
    }

    /* Transmit the register address to be read */
    I2C_SendData(I2C1, reg_addr_hi);
    /* Test on EV8 and clear it */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET) //I2C_FLAG_BTF::Byte transfer finished flag
    {
        if((CAMERATimeout--) == 0) return Camera_TIMEOUT_UserCallback();
    }

    /* Transmit the register address to be read */
    I2C_SendData(I2C1, reg_addr_lo);
    /* Test on EV8 and clear it */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) == RESET) //I2C_FLAG_BTF::Byte transfer finished flag
    {
        if((CAMERATimeout--) == 0) return Camera_TIMEOUT_UserCallback();
    }
    
    /*!< Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C1, ENABLE);
  
    /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((CAMERATimeout--) == 0) return Camera_TIMEOUT_UserCallback();
    } 
  
    /*!< Send Codec address for read */
    I2C_Send7bitAddress(I2C1, CAMERA_CODEC_ADDRESS, I2C_Direction_Receiver);  
  
    /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
    {
        if((CAMERATimeout--) == 0) return Camera_TIMEOUT_UserCallback();
    }     
  
    /*!< Disable Acknowledgment */
    I2C_AcknowledgeConfig(I2C1, DISABLE);   
  
    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)I2C1->SR2;
  
    /*!< Send STOP Condition */
    I2C_GenerateSTOP(I2C1, ENABLE);
  
    /* Wait for the byte to be received */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET)
    {
        if((CAMERATimeout--) == 0) return Camera_TIMEOUT_UserCallback();
    }
  
    /*!< Read the byte received from the Codec */
    result = I2C_ReceiveData(I2C1);
  
    /* Wait to make sure that STOP flag has been cleared */
    CAMERATimeout = CAMERA_FLAG_TIMEOUT;
    while(I2C1->CR1 & I2C_CR1_STOP)
    {
        if((CAMERATimeout--) == 0) return Camera_TIMEOUT_UserCallback();
    }  
  
    /*!< Re-Enable Acknowledgment to be ready for another reception */
    I2C_AcknowledgeConfig(I2C1, ENABLE);  
  
    /* Clear AF flag for next communication */
    I2C_ClearFlag(I2C1, I2C_FLAG_AF); 
  
    /* Return the byte read from Codec */
    return result;
}


void I2C1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStruct;
 
 
    /* GPIOB clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
   
    /* I2C1 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
 
    /* Connect I2C1 pins to AF4 ************************************************/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);  
   
    /* Configure I2C1 GPIOs *****************************************************/  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   
    GPIO_Init(GPIOB, &GPIO_InitStructure);
 
    /* Configure I2C1 ***********************************************************/  
    /* I2C DeInit */   
    I2C_DeInit(I2C1);
   
    /* Set the I2C structure parameters */
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = CAMERA_I2C_SPEED;
   
    /* Initialize the I2C peripheral w/ selected parameters */
    I2C_Init(I2C1, &I2C_InitStruct);
 
    /* Enable the I2C peripheral */
    I2C_Cmd(I2C1, ENABLE);
}

void CameraTest_Help(void)
{
    Dprintf("=====================================\n");
    Dprintf("w. OV5642 Register Write\n");
    Dprintf("r.  OV5642 Register Read\n");
    Dprintf("=====================================\n");
	Dprintf("c.  Memory Add Xpoint\n");
	Dprintf("x. MemoryAdd Ypoint\n");
    Dprintf("=====================================\n");	
	Dprintf("p. VIDEO ON/OFF Toggle\n");
    Dprintf("=====================================\n");
	Dprintf("v. VGA_640_480_PreView\n");
	Dprintf("1. SVGA PREVIEW\n");
	Dprintf("2. QCIF PREVIEW\n");
	Dprintf("3. CIF PREVIEW\n");
    Dprintf("=====================================\n");
	Dprintf("o. OC5642_Processor_RAW_Setting\n");
	Dprintf("4. QSXGA_to_vga_640_480\n");
	Dprintf("6. QSXGA_to_sxga1280_960\n");
	Dprintf("7. QSXGA_to_QVGA320_240\n");
	Dprintf("8. QSXGA_to_qxga2048_1536\n");
	Dprintf("9. QSXGA_to_uxga1600_1200\n");
	Dprintf("0. QSXGA_to_xga1024_768\n");
    Dprintf("=====================================\n");
}
void SM718_CameraScreenClear(U32 addr, U16 data)
{
	int		i;
	
	for(i=0; i<(VIDEO_CAMERA_VIEW_X*VIDEO_CAMERA_VIEW_Y); i++)
	{
		*(U16  *)addr= data;
		addr+=2;
	}
}

void LCD_AlphaScreen(int s);
void SM718_VideoAlphaClear(U16 color);
void LCD_AlphaScreen(int s);
void VideoAlphaScreen(int s);
void SM718_AlphaClear(U16 color);
void Video_AlphaTransparency(int value);
void SM718_VideoMode(int sx, int sy, int width, int height, int enable);
void SM718_CaptureMode(int width, int height);

void Camera_Test(void)
{
    char    		sd;
	uint16_t		addr;
	uint8_t		data;
	unsigned int		toggle=0;
	int	alpha=7;
	
	//Cell.imageXPointValue=0;
	//Cell.imageYPointValue=0;


	SM718_VideoAlphaClear(clBlack);
	BmpLoad(VIDEO_ALPHA_SCREEN, 0, 0, "backimage.bmp");	
	
	//VideoAlphaScreen(ON);
	VideoScreen(ON);	
	Dprintf("=====================\n");
	while(1)
	{
		char sd = Dgetch();
		if(sd=='1') 
		{
			VideoAlphaScreen(ON);	
			Dprintf("Video Alpha Screen ON\n");
		}
		else if(sd=='2') 
		{
			VideoAlphaScreen(OFF);	
			Dprintf("Video Alpha Screen OFF\n");
		}
		else if(sd=='u')
		{
			Video_AlphaTransparency(alpha++);
			if(alpha > 15) alpha=15;
			Dprintf("Video Alpha Val=%d\n", alpha);
		}
		else if(sd=='d')
		{
			Video_AlphaTransparency(alpha--);
			if(alpha < 0) alpha=0;
			Dprintf("Video Alpha Val=%d\n", alpha);
		}
		else if(sd=='8')
		{
			SM718_CaptureMode(1024, 768);
			SM718_VideoMode((LCD_X-1024)/2, (LCD_Y-768)/2, 1024, 768, 1);
			Dprintf("1024X768 Mode\n");
		}
		else if(sd=='9')
		{
			SM718_CaptureMode(1280, 1024);
			SM718_VideoMode((LCD_X-1280)/2, (LCD_Y-1024)/2, 1280, 1024, 1);
			Dprintf("1280X1024 Mode\n");
		}
	}

	Camera_IIC_Init();  //I2C Engine Init
    CameraTest_Help();
	
	OC5642_Processor_RAW_Setting();	
	QSXGA_to_sxga1280_960();
	//SVGA_PreView();
	//VGA_640_480_PreView();

    while(1)
    {
        sd = Dgetch();
		
        if(sd=='w') //OV5642 의 레지스터에 값을 기록한다.
        {
            Dprintf("OV5642 WR Addr:");
            addr = DgetIntNum();
            Dprintf("\nOV5642 data:");
            data = DgetIntNum();
            Camera_WriteRegister(addr, data);
            data = Camera_ReadRegister(addr);
            Dprintf("------------------------------->Read Verify:%02x\n", data);
            
        }
        else if(sd=='r') //OV5642 레지스터값을 읽어낸다.
        {
            Dprintf("OV5642 RD Addr:");
            addr = DgetIntNum();
            data = Camera_ReadRegister(addr);
            Dprintf("------------------------------>%02x\n", data);
        }
		else if(sd=='c')//메모리 카피
		{
			//Cell.imageXPointValue+=100;
			//Memory_Bitblt3(VIDEO_SCREEN_ADDR, LCD_SCREEN_ADDR,  Cell.imageXPointValue, Cell.imageYPointValue, 1000, 700, 0, 0, VIDEO_CAMERA_VIEW_X,  LCD_X );
			//Dprintf("StartXpoint = %d StartYpoint = %d\n", Cell.imageXPointValue, Cell.imageYPointValue);
		}
		else if(sd=='x')//메모리 카피
		{
			//Cell.imageYPointValue+=100;
			//Memory_Bitblt3(VIDEO_SCREEN_ADDR, LCD_SCREEN_ADDR,  Cell.imageXPointValue, Cell.imageYPointValue, 1000, 700, 0, 0, VIDEO_CAMERA_VIEW_X,  LCD_X );
			//Dprintf("StartXpoint = %d StartYpoint = %d\n", Cell.imageXPointValue, Cell.imageYPointValue);
		}		
		else if(sd == 'p')
		{
			//Cell.imageXPointValue=0;
			//Cell.imageYPointValue=0;
			if(toggle==0)
			{
				VideoScreen(ON);
				Dprintf("Video ON\n");
				toggle=1;
			}
			else 
			{
				VideoScreen(OFF);
				Dprintf("Video OFF\n");
				toggle=0;
			}
		}
		else if(sd == 'v')
		{
			VGA_640_480_PreView();		
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if(sd == '1')//SVGA PREVIEW
		{
			SVGA_PreView();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if(sd=='2')//QCIF PREVIEW
		{
			QCIG_PreView();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if(sd=='3')//CIF PREVIEW
		{			
			CIF_PreView();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if(sd=='4')//QSXGA_to_vga640_480
		{
			QSXGA_to_vga640_480();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if(sd=='6')//QSXGA_to_sxga1280_960
		{
			QSXGA_to_sxga1280_960();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if( sd=='7')//QSXGA_to_QVGA320_240
		{
			QSXGA_to_QVGA320_240();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if( sd == '8')//QSXGA_to_qxga2048_1536
		{
			QSXGA_to_qxga2048_1536();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if( sd == '9')//QSXGA_to_uxga1600_1200
		{
			QSXGA_to_uxga1600_1200();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if(sd=='0')//QSXGA_to_xga1024_768
		{
			QSXGA_to_xga1024_768();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else if(sd=='o')
		{
			Dprintf("OC5642_Processor_RAW_Setting\n");
			OC5642_Processor_RAW_Setting();
			//SM718_CameraScreenClear(VIDEO_SCREEN_ADDR, clBlack);	
		}
		else
        {
            CameraTest_Help();
        }		
    }    
}

#endif


