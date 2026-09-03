/* Includes ------------------------------------------------------------------*/
#include "Teeco_System.h"
#include "SiI9293.h"
void ITU656Setting(void);

// ACR Control Register3 Bit Define
#define RX_CTS_THRESHOLD 							0x50 				// bits 3...6
#define RX_M__ACR_CTRL3__MCLK_LOOPBACK	0x04

const uint8_t default_sval_regs[4] = {0x0F, 0x20, 0x00, 0x00};

// Interrupt Control Register Bit Define 
#define RX_M__INT_CTRL__POLARITY					0x02
#define RX_M__INT_CTRL__OPEN_DRAIN				0x04

// HDMI Audio/Video MUTE Register
#define RX_M__AUDP_MUTE__AUDIO_MUTE		0x02
#define RX_M__AUDP_MUTE__VIDEO_MUTE		0x01

// Video Control 2 Register Bit Define
#define RX_M__VID_CTRL2__HJITTER_EN				0x01

// ECC Control Register
#define RX_M__ECC_CTRL__CAPTURE_CNT			0x01
#define RX_C__ECC0_HDCP_THRES_VALUE			0x0B40 // recommended value

//System Control Register 1
#define BIT_POWER_DOWN_ALL						0x01

//Software Reset Register
#define BIT_SWRST                       					0x01
//====================================================
// 이함수는 SiI에서 제공하는 F/W Source를 보고 똑같이 구현한 SiI9293칩 제어용 함수
//====================================================
void init_rx(void)
{
	// Hot Plug Control Register,  Enable Hotplug
	SII9293Write(0x64, 0x0b, 0x01);
	
	// System Power Down 3 Register
	SII9293Write(0x68, 0x3D, 0x01); //normal operation
	
	/*
	//Audio Out I2S Control Register 2
	SII9293Write(0x68, 0x27, 0x01);
	// Audio Out I2S Control Register 1
	SII9293Write(0x68, 0x26, 0x40);
	// Audio Out Control Register
	SII9293Write(0x68, 0x29, 0x04);
	*/
	
	/*
	//오디오를 사용하지 않으므로 Don't care
	//Audio Clock Recovery (ACR) Registers
	//ACR Control Register3,  Enable internal loopback, 
	SII9293Write(0x68, 0x18, RX_CTS_THRESHOLD | RX_M__ACR_CTRL3__MCLK_LOOPBACK);
	
	//오디오를 사용하지 않으므로 Don't care
	// init threshold for PLL unlock interrupt, 0x13 ~ 0x16까지 설정
	SII9293WriteBlock(0x68, 0x13, (uint8_t *)default_sval_regs, sizeof(default_sval_regs));
	*/
	
	// System Control Register 1
	// 비디오버스폭과 데이터의 에지를 설정한다.
	// 출력픽셀클록은 나누지 않는다. :00
	// 입력픽셀클록 복제안함: 00
	// 24 Bit Mode 를 사용한다, : 0x04
	//출력클록은 rising edge
	// Power Down Mode지정->설정시에는 PWDN으로 한후 차후 복귀시키는것으로 판단

	SII9293Write(0x64, 0x08, 0x04 | 0x01);		//0x01 -> Normal Mode, 0x02를 설정하면 Inverter Clock
	
	/*
	// HDMI Audio Preamble Criteria Register
	// 0x06->Default Value
	SII9293Write(0x68, 0x35, 0x06);
	*/
	/*
	// HDMI Audio HDCP Enable Criteria Register
	// 0x0C->Default Value
	SII9293Write(0x68, 0x38, 0x0C);
	*/
	
	// set interrupt pin configuration: negative polarity(Low Active), open drain
	// Interrupt Control Register, 0x64::0x79
	// Default Value ->0x06
	SII9293Write(0x64, 0x79, RX_M__INT_CTRL__POLARITY | RX_M__INT_CTRL__OPEN_DRAIN);
	
	// HDMI Audio/Video MUTE Register
	// AudioVideo Mute ON	-> 일시동작금지를 설정하는 이유??? (칩설정이 끝난후 풀거라생각함)
	SII9293Write(0x68, 0x37, RX_M__AUDP_MUTE__AUDIO_MUTE |  RX_M__AUDP_MUTE__VIDEO_MUTE);
	
	// Video Control 2 Register
	// Disable AVC Fix (Default)
	// Sync polarity is negative 로 디폴트값임
	// Disable Hsync jitter filter
	SII9293RegBitsSet(0x64, 0x57, RX_M__VID_CTRL2__HJITTER_EN, OFF); // 080115 according to SiI-CM-1053-0.70
	//SII9293Write(0x64, 0x57, 0x07);
	
	// Auto Audio and Video Control Register
	// 자동으로 오디오, 비디오처리하는 기능을 막는다.
	SII9293Write(0x64, 0xB5, 0x00);
	
	// ECC BCH Threshold Register
	// set BCH threshold and reset BCH counter	
	// Default Value:0x01
	SII9293Write(0x64, 0xBC, 0x02);
	
	// ECC Control Register
	// Capture and clear BCH T4 errors.	
	SII9293Write(0x64, 0xBB, RX_M__ECC_CTRL__CAPTURE_CNT);
	
	// set HDCP Error Threshold
	SII9293RegWriteWord(0x64, 0xC5, RX_C__ECC0_HDCP_THRES_VALUE);
	
	// Video Channel PCLK Count Base Register
	SII9293Write(0x64, 0x69,  0xFF);	
	
	// Video Processing Registers
	SII9293Write(0x64, 0x48, 
						(0 << 7 ) |				// 0:Do not invert the VSYNC signal 1: Invert the VSYNC signal
						(0 << 6) | 				// 0 : Do not invert the Hsync signal,  1:Invert the Hsync signal
						(1 << 2) | 				// 0: BT.601, 1: BT.709
						(0 << 1) |				// 0: Decoded pixel values are 8 bits wide, either RGB or YCbCr,  1:Decoded pixel values are YCbCr 4:2:2 and wider than 8 bits
						1							// 0: BT.601, 1: BT.709
							);	

	// Video Mode 2 Register
	SII9293Write(0x64, 0x49, 
		(0 << 6 ) |				// Dither Mode Selection. 0:Dither to 8 bits, 1:Dither to 10 bits, 2:Dither to 12 bits
		(0 << 5) |				// 0: Field 0 is even and field 1 is odd, 1:Field 0 is odd and field 1 is even
		(0 << 3) |				// 0: Disable range scaling for color-space converter, 1:RGB data range scaling from 16 ? 235 to 0 ? 255
		(0 << 2) |				// Enable YCbCr-to-RGB color-space converter.   0:Bypass, 1:Convert YCbCr to RGB
		(1 << 1) |				// 0: Output color space is RGB,  1: 1 ? Output color space is YCbCr
		0);							// Enable range clip 0 ? 255 to 16 ? 235 (RGB and Y) and 0 ? 255 to 16 ?240 (CbCr).  0:Disable, 1:Enable


	// Video Mode 1 Register
	SII9293Write(0x64, 0x4A, 
		(1 << 7 ) |				// Insert SYNC (SAV/EAV) code into ITU.656 data stream. 0:Disable, 1:Enable
		(1 << 6) |				// Multiplexed YC 4:2:2 into a single channel.  0:Disable, 1:Enable
		(1 << 5) |				// 10 bits to 8 bits Data Dither. 0:Disable, 1:Enable
		(0 << 4) |				// Scale RGB-to-YCbCr Range. 0:Bypass, 1:Compress 0 ~ 255 to 16 ~ 235
		(1 << 3) |				// Convert RGB-to-YCbCr Color-space. 0:Bypass, 1:Convert RGB to YCbCr
		(0 << 2) |				// Upsample.  0:Bypass, 1:4:2:2 to 4:4:4 upsampler
		(1 << 1) |				// Downsample. 0:Bypass, 1:4:4:4 to 4:2:2 downsampler
		0);							//

	//System Power Down 2 Register, ODCLK, HS, VS, DE Enable
	SII9293Write(0x68, 0x3E, 0xC7);		//{0x68}[0x3E] = 0xC5 // bit1 1'b0 Power down the VSYNC,HSYNC,DE signal

	//Auto Audio and Video Control Register, Enable AVC
	SII9293Write(0x64, 0xB5, 0x04);		//{0x64}[0xB5] =  0x04 

	SII9293Write(0x64, 0x5F, 0xC0);		//{0x64}[0x5F] = 0xC0  //video output format  digital YCbCr output 
	SII9293Write(0x64, 0x81, 0x00);		//{0x64}[0x81] =  0x22        // hiden register로서 clk x2를 해주는 것입니다.

	
	// Receiver Control Register 1
	SII9293Write(0xD0, 0x6C,  ( 0x01 |  0x02 | 0x04 |0x08 | 0x10 | 0x20 ) ); 
	// Receiver Control Register 4
	SII9293Write(0xD0, 0x6F, 0x22);	
	
	// Receiver Control Register 5
	// TMDS Termination Control Register
	// 50ohm, 48.6ohm, Enable this for Rx core to take
	SII9293Write(0xD0, 0x70, 0x0 | 0x40 | 0x80 );

	// Send normal video data
	SII9293RegBitsSet(0x68, 0x37, RX_M__AUDP_MUTE__VIDEO_MUTE, OFF);
	
	// Power Down모드해제, Normal Mode로 설정한다.
	SII9293RegModify(0x64, 0x08, BIT_POWER_DOWN_ALL, SET_BITS);	
}

// YC8_2_YC8@480p
void ITU656Setting(void)
{
	Dprintf("ITU656 Setting Start\n");
	//24Bit Mode, PowerDown Release->Normal Mode
	SII9293Write(0x64, 0x08, 0x05);		//{0x64}[0x08] =  0x05
	
	//Auto Audio FIFO Reset -> ???
	SII9293Write(0x64, 0x07, 0x40);		//{0x64}[0x07] =  0x40
	
	//Enable TMDS Core0, Select DDC0, DDC Delay Enable
	SII9293Write(0x64, 0x09, 0x91);		//{0x64}[0x09] =  0x91
	//HPD = 1;
	SII9293Write(0x64, 0x0B, 0x01);		//{0x64}[0x0B] =  0x01
	//System Power Down 3 Register Normal AUDIO, Normal operation
	SII9293Write(0x68, 0x3D, 0x01);		//{0x68}[0x3D] =  0x01 

	//Audio Out I2S Control Register 2
	SII9293Write(0x68, 0x27, 0xF9);		//{0x68}[0x27] =  0xf9
	//Audio Out I2S Control Register 1, Sample edge is falling
	SII9293Write(0x68, 0x26, 0x40);		//{0x68}[0x26] =  0x40
	//Audio Out Control Register
	SII9293Write(0x68, 0x29, 0x04);		//{0x68}[0x29] =  0x04
	
	//Software Reset Register, Reset all internal logic, except the register interface, acr, hdcp, aac.
	SII9293Write(0x64, 0x05, 0x01);		//{0x64}[0x05] =  0x01
	SII9293Write(0x64, 0x05, 0x00);		//{0x64}[0x05] =  0x00 
	
	//System Power Down 2 Register, ODCLK, HS, VS, DE Enable
	SII9293Write(0x68, 0x3E, 0xC7);		//{0x68}[0x3E] = 0xC5 // bit1 1'b0 Power down the VSYNC,HSYNC,DE signal

	//Video Control Register, BT709 Mode
	SII9293Write(0x64, 0x48, 0x01);		//{0x64}[0x48] = 0x01
	//Video Mode 2 Register, Output color space is YCbCr
	SII9293Write(0x64, 0x49, 0x02);		//{0x64}[0x49] = 0x02  //video mode  output color space YCbCr
	
	//Insert SYNC (SAV/EAV) code into ITU.656 data stream.
	//Multiplexed YC 4:2:2 into a single channel
	//10 bits to 8 bits Data Dither.
	//Convert RGB to YCbCr
	//4:4:4 to 4:2:2 downsampler
	SII9293Write(0x64, 0x4A, 0xEA);		//{0x64}[0x4A] = 0xEA  //video mode select
	
	//Auto Output Format Register
	//Digital 8-bit 4:2:2 YCbCr output
	SII9293Write(0x64, 0x5F, 0xC0);		//{0x64}[0x5F] = 0xC0  //video output format  digital YCbCr output 
	//SII9293Write(0x64, 0x81, 0x22);		//{0x64}[0x81] =  0x22        // hiden register로서 clk x2를 해주는 것입니다.
	//SII9293Write(0x64, 0x5F, 0xF0);		//{0x64}[0x5F] =  0xF0

	//Auto Audio and Video Control Register, Enable AVC
	SII9293Write(0x64, 0xB5, 0x04);		//{0x64}[0xB5] =  0x04 

	// My Test=================================

	//=======================================

	// Receiver Control Register 1, PLL is ON, Basic bias circuits of PLL are ON, Channel 0 is ON, Channel 1 and Channel 2 are OFF
	// Fully ON
	SII9293Write(0xD0, 0x6C, 0x3F);		//{0xD0} [0x6c] = 0x3f
	// Receiver Control Register 5
	SII9293Write(0xD0, 0x70, 0xC8);		//{0xD0} [0x70] = 0xc8  //these two registers need to set in es0.1 chip
}





