/******************************************************************************
*    	File name	: BSP_SM718.c
*		Description : GPU Init Routine
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 06 / 04
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/

/******************************************************************************
							Include File
******************************************************************************/
#include "TEECO_System.h"
#include "WindowsGraphics.h"
#include "DataLog.h"

int gHValue=3050, gLValue=2582, gHClp=0, gLClp=70;
//int gHValue=4095, gLValue=4095, gHClp=0, gLClp=70;
/******************************************************************************
							Constant & Macros
******************************************************************************/
#define		GPIO_0		(U32)0x00000001
#define		GPIO_1		(U32)0x00000002
#define		GPIO_2		(U32)0x00000004
#define		GPIO_3		(U32)0x00000008
#define		GPIO_4		(U32)0x00000010
#define		GPIO_5		(U32)0x00000020
#define		GPIO_6		(U32)0x00000040
#define		GPIO_7		(U32)0x00000080
#define		GPIO_8		(U32)0x00000100
#define		GPIO_9		(U32)0x00000200
#define		GPIO_10		(U32)0x00000400
#define		GPIO_11		(U32)0x00000800
#define		GPIO_12		(U32)0x00001000
#define		GPIO_13		(U32)0x00002000
#define		GPIO_14		(U32)0x00004000
#define		GPIO_15		(U32)0x00008000
#define		GPIO_16		(U32)0x00010000
#define		GPIO_17		(U32)0x00020000
#define		GPIO_18		(U32)0x00040000
#define		GPIO_19		(U32)0x00080000
#define		GPIO_20		(U32)0x00100000
#define		GPIO_21		(U32)0x00200000
#define		GPIO_22		(U32)0x00400000
#define		GPIO_23		(U32)0x00800000
#define		GPIO_24		(U32)0x01000000
#define		GPIO_25		(U32)0x02000000
#define		GPIO_26		(U32)0x04000000
#define		GPIO_27		(U32)0x08000000
#define		GPIO_28		(U32)0x10000000
#define		GPIO_29		(U32)0x20000000
#define		GPIO_30		(U32)0x40000000
#define		GPIO_31		(U32)0x80000000

#define		GPIO_IN				0
#define		GPIO_OUT			1
/******************************************************************************
							Private & Local Variables
******************************************************************************/


/******************************************************************************
							Function Prototype
******************************************************************************/
void SM718Clk_Calc(double out_f);
#define		SYSTEM_CONTROL_2		*(volatile U16 *)0x6BE00002
#define		SYSTEM_CONTROL_0		*(volatile U16 *)0x6BE00000
#define		MISCELL_CONTROL_2		*(volatile U16 *)0x6BE00006
#define		MISCELL_CONTROL_0		*(volatile U16 *)0x6BE00004
void SM718_SystemConfig(void) {
	//SYSTEM_CONTROL PORT is Default Value
	MISCELL_CONTROL_2 =  (2 <<  9) |	// Memory Refresh Control 1: 16 X 100 , 2: 32 X 100
						 (1 <<  8) |	// Interrupt Signal Inverter
					     (0 <<  7) |	// PLL Clock Count Disable
						 (1 <<  4) |	// DAC Power Enable
						  0  ; 			// Clock Select (0: Crystal) MA9

	MISCELL_CONTROL_0 =  (1 << 14) | 	// DDR SDRAM Colume Size 512(0x01)
						 (2 << 12) |	// DDR SDRAM Size 32MB(0x01), 64MB(0x02)
						 (0 << 11) |	// tWTR
						 (0 << 10) |	// tWR
						 (0 << 9)  |	// tRP
						 (0 << 8)  |	// tRFC
						 (0 << 7)  |	// tRAS
						 (1 << 6)  |	// Local Memory Controller Reset(0), 1(Normal)
						 (1 << 5)  |	// Local Memory Remain in Active State Donot(1), 0(Active)
						 (1 << 4)  |	// CAS Latency 2.5CLK(0) 1: 3CLK
						 (0 << 3)  |	// DLL Enable
						 (0 << 2)  |	// SDRAM Low Driver Enable(0)
						 (0 << 1)  |	// Memory Bus 32Bit(0), 64Bit(1)
						  0;			// Embedded Momory Control Enable(0)
}


#define		GPIO_CONTROL_2			*(volatile U16 *)0x6BE0000A
#define		GPIO_CONTROL_0			*(volatile U16 *)0x6BE00008
void SM718_GPIOConfig(void) {
	GPIO_CONTROL_2 = 	 (1 << 15) |	// GPIO31 0:GPIO, 1:I2C-DATA
						 (1 << 14) |	// GPIO30 0:GPIO, 1:I2C-CLK
						 (0 << 13) | 	// GPIO29 0:GPIO
						 (0 << 12) | 	// GPIO28 0:GPIO
						 (0 << 11) | 	// GPIO27 0:GPIO
						 (0 << 10) | 	// GPIO26 0:GPIO
						 (0 <<  9) | 	// GPIO25 0:GPIO
						 (0 <<  8) | 	// GPIO24 0:GPIO
						 (0 <<  7) | 	// GPIO23 0:GPIO
						 (0 <<  6) | 	// GPIO22 0:GPIO
						 (0 <<  5) | 	// GPIO21 0:GPIO
						 (0 <<  4) | 	// GPIO20 0:GPIO
						 (0 <<  3) | 	// GPIO19 0:GPIO, 1:PWM2
						 (0 <<  2) | 	// GPIO18 0:GPIO, 1:PWM1
						 (1 <<  1) | 	// GPIO17 0:GPIO, 1:PWM0
						 (0 ) ; 		// GPIO16 0:GPIO or ZV-Port1 CLOCK

	GPIO_CONTROL_0 = 	 (0 << 15) |	// GPIO15 0:GPIO or ZV-Port[xx]
						 (0 << 14) |	// GPIO14 0:GPIO or ZV-Port[xx]
						 (0 << 13) |	// GPIO13 0:GPIO or ZV-Port[xx]
						 (0 << 12) |	// GPIO12 0:GPIO or ZV-Port[xx]
						 (0 << 11) |	// GPIO11 0:GPIO or ZV-Port[xx]
						 (0 << 10) |	// GPIO10 0:GPIO or ZV-Port[xx]
						 (0 <<  9) |	// GPIO9 0:GPIO or ZV-Port[xx]
						 (0 <<  8) |	// GPIO8 0:GPIO or ZV-Port[xx]
						 (0 <<  7) |	// GPIO7 0:GPIO or ZV-Port[xx]
						 (0 <<  6) |	// GPIO6 0:GPIO or ZV-Port[xx]
						 (0 <<  5) |	// GPIO5 0:GPIO or ZV-Port[xx]
						 (0 <<  4) |	// GPIO4 0:GPIO or ZV-Port[xx]
						 (0 <<  3) |	// GPIO3 0:GPIO or ZV-Port[xx]
						 (0 <<  2) |	// GPIO2 0:GPIO or ZV-Port[xx]
						 (0 <<  1) |	// GPIO1 0:GPIO or ZV-Port[xx]
						 (0 ) ;			// GPIO0 0:GPIO or ZV-Port[xx]							
}

#define		LMAC_CONTROL_2		*(volatile U16 *)0x6BE0000E
#define		LMAC_CONTROL_0		*(volatile U16 *)0x6BE0000C
void SM718_LocalMemory_Arbitration(void) {
	LMAC_CONTROL_2 = (0 << 12) | 	// Internal Memory Priority Fixed(0)
	                 (1 << 8)  |	// VGA FIFO Priority 7
					 (7 << 4)  |	// DMA FIFO Priority 6
					 6;				// ZV Port1 FIFO Priority 5

	LMAC_CONTROL_0 = (5 << 12) | 	// ZV Port0 FIFO Priority 3
	                 (3 << 8)  |	// VIDEO FIFO Priority 1
	                 (2 << 4)  |	// PANEL FIFO Priority 4
	                 (4);			// CRT FIFO Priority 2 	
}


#define		ARBITRATION_CONTROL_2		*(volatile U16 *)0x6BE00016
#define		ARBITRATION_CONTROL_0		*(volatile U16 *)0x6BE00014
void SM718_ArbitrationControl(void) {
	ARBITRATION_CONTROL_2 = (0 << 12) |		// Local Memory Priority (0)
	                        (3 << 4)  |		// Panel FIFO Priority (1)
							 1;				// ZV Port FIFO Priority (2)

	ARBITRATION_CONTROL_0 = (5 << 12) |		// Command FIFO Priority (5)
	                        (4 << 8)  |		// DMA FIFO Priority (4)
	                        (2 << 4)  |		// VIDEO FIFO Priority (3)
							 6;				// CRT Port FIFO Priority (6)
}

#define		INTMASK_CONTROL_2		*(volatile U16 *)0x6BE0002A
#define		INTMASK_CONTROL_0		*(volatile U16 *)0x6BE00028
void SM718_InterruptMask(void) {
	INTMASK_CONTROL_2 = (0 << 15) |		// GPIO31 Interrupt Disable(0)
	                    (0 << 14) | 	// GPIO30 Interrupt Disable(0)
	                    (0 << 13) | 	// GPIO29 Interrupt Disable(0)
	                    (0 << 12) | 	// GPIO28 Interrupt Disable(0)
	                    (0 << 11) | 	// GPIO27 Interrupt Disable(0)
	                    (0 << 10) | 	// GPIO26 Interrupt Disable(0)
	                    (0 << 9); 		// GPIO25 Interrupt Disable(0)

	INTMASK_CONTROL_0 = (0 << 12) |		// I2C Interrupt Disable(0)
	                    (0 << 11) | 	// PWM Interrupt Disable(0)
	                    (0 << 9) | 		// DMA1 Interrupt Disable(0)
	                    (0 << 8) | 		// PCI Interrupt Disable(0)
	                    (0 << 7) | 		// SSP1 Interrupt Disable(0)
	                    (0 << 6) | 		// SSP0 Interrupt Disable(0)
	                    (0 << 5) | 		// 2D Interrupt Disable(0)
	                    (0 << 4) | 		// ZV1 Interrupt Disable(0)
	                    (0 << 3) | 		// ZV0 Interrupt Disable(0)
	                    (0 << 2) | 		// CRT vertical Interrupt Disable(0)
	                    (0 << 1) | 		// Panel vertical Interrupt Disable(0)
	                    (0)  ; 			// VGA Vertical Interrupt Disable(0)
}

#define		POWERMODE_CONTROL		*(volatile U16 *)0x6BE0004C
#define		POWERMODE_0_CONTROL		*(volatile U16 *)0x6BE00044
#define		POWERMODE_1_CONTROL		*(volatile U16 *)0x6BE00048
void SM718_PowerModeControl(void) {
	POWERMODE_0_CONTROL = (1 << 14) | 	// MCLK Select Divided by 4(0x01)
	                      (1 << 12) | 	// M2XCLK Select Divided by 2(0x01)	
						  (1 << 10) |	// VGA Clock Control Enable
						  (1 << 9)  |	// PWM Clock Control Enable
						  (1 << 8)  |	// I2C Clock Control Enable
						  (0 << 7)  |	// SSP Clock Control Disble
						  (1 << 6)  |	// GPIO Clock Control Enable
						  (1 << 5)  |	// ZVPORT Clock Control Enable
						  (1 << 4)  |	// Color Space Clock Control Enable
						  (1 << 3)  |	// 2D Engine Clock Control Enable
						  (1 << 2)  |	// Display Control Clock Control Enable
						  (1 << 1)  |	// Local Memory Control Clock Control Enable
						  (1 ) ;		// DMA Clock Control Enable

	POWERMODE_1_CONTROL = (1 << 14) | 	// MCLK Select Divided by 4(0x01)
	                      (1 << 12) | 	// M2XCLK Select Divided by 2(0x01)	
						  (1 << 10) |	// VGA Clock Control Enable
						  (1 << 9)  |	// PWM Clock Control Enable
						  (1 << 8)  |	// I2C Clock Control Enable
						  (0 << 7)  |	// SSP Clock Control Disble
						  (1 << 6)  |	// GPIO Clock Control Enable
						  (1 << 5)  |	// ZVPORT Clock Control Enable
						  (1 << 4)  |	// Color Space Clock Control Enable
						  (1 << 3)  |	// 2D Engine Clock Control Enable
						  (1 << 2)  |	// Display Control Clock Control Enable
						  (1 << 1)  |	// Local Memory Control Clock Control Enable
						  (1 ) ;		// DMA Clock Control Enable
	
	POWERMODE_CONTROL =   (1 << 3)	| 	// Osillator Input Control Enable
	                      (1 << 2)  |	// ACPI Control Disable
						   0;			// Power Mode is 0
}

#define		PRIMARY_DISPLAY_CONTROL		*(volatile U32 *)0x6BE0005C
#define		SECONDARY_DISPLAY_CONTROL	*(volatile U32 *)0x6BE00060
//PCLK, HSYNC, VSYNC 클럭의 결정
void SM718_DisplayControl(void) {
#ifdef LCD_SAMSUNG_LMS700KF06	
	PRIMARY_DISPLAY_CONTROL =   (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0:Div1
	                            (3 << 12) | 	// Divided 1
								(9 << 8)  |		// PLL N: 0~15
								 124;				// PLL M: 0~255

	SECONDARY_DISPLAY_CONTROL = (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0: Div1
	                            (3 << 12) | 	// Divided 1
								(9 << 8)  |		// PLL N: 0~15
								 124;				// PLL M: 0~255
#endif

#ifdef LCD_5Inch
	PRIMARY_DISPLAY_CONTROL =   (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0:Div1
	                            (3 << 12) | 	// Divided 1
								(12 << 8)  |		// PLL N: 0~15
								 223;				// PLL M: 0~255

	SECONDARY_DISPLAY_CONTROL = (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0: Div1
	                            (3 << 12) | 	// Divided 1
								(12 << 8)  |		// PLL N: 0~15
								 223;				// PLL M: 0~255
#endif
	
#ifdef LCD_TAIWAN_CLAA080	
	PRIMARY_DISPLAY_CONTROL =   (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0:Div1
	                            (2 << 12) | 	// Divided 1
								(13 << 8)  |		// PLL N: 0~15
								 139;				// PLL M: 0~255

	SECONDARY_DISPLAY_CONTROL = (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0: Div1
	                            (2 << 12) | 	// Divided 1
								(13 << 8)  |		// PLL N: 0~15
								 139;				// PLL M: 0~255
#endif

#ifdef LCD_TS8080
	PRIMARY_DISPLAY_CONTROL =   (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0:Div1
	                            (1 << 12) | 	// Divided 1
								(15 << 8)  |		// PLL N: 0~15
								 143;				// PLL M: 0~255

	SECONDARY_DISPLAY_CONTROL = (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0: Div1
	                            (1 << 12) | 	// Divided 1
								(15 << 8)  |		// PLL N: 0~15
								 143;				// PLL M: 0~255
#endif	
#ifdef LCD_1024
	PRIMARY_DISPLAY_CONTROL =   (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0:Div1
	                            (1 << 12) | 	// Divided 1
								(15 << 8)  |		// PLL N: 0~15
								 143;				// PLL M: 0~255

	SECONDARY_DISPLAY_CONTROL = (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0: Div1
	                            (1 << 12) | 	// Divided 1
								(15 << 8)  |		// PLL N: 0~15
								 143;				// PLL M: 0~255
#endif	
#ifdef LCD1024X600
	PRIMARY_DISPLAY_CONTROL =   (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0:Div1
	                            (1 << 12) | 	// Divided 1
								(15 << 8)  |		// PLL N: 0~15
								 143;				// PLL M: 0~255

	SECONDARY_DISPLAY_CONTROL = (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0: Div1
	                            (1 << 12) | 	// Divided 1
								(15 << 8)  |		// PLL N: 0~15
								 143;				// PLL M: 0~255
#endif	
#ifdef LCD17INCH
	PRIMARY_DISPLAY_CONTROL =   (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0:Div1
	                            (1 << 12) | 	// Divided 1
								(15 << 8)  |		// PLL N: 0~15
								 143;				// PLL M: 0~255

	SECONDARY_DISPLAY_CONTROL = (0 << 18) |		// PLL No Bypass
								(1 << 17) | 	// PLL Power ON
								(0 << 16) |		// Crystal Input								 	
								(0 << 14) |		// PLL POST Divider 0: Div1
	                            (1 << 12) | 	// Divided 1
								(15 << 8)  |		// PLL N: 0~15
								 143;				// PLL M: 0~255
#endif	
}

#define		MXCLKPLL_CONTROL_2	*(volatile U16 *)0x6BE00072
#define		MXCLKPLL_CONTROL_0	*(volatile U16 *)0x6BE00070
#define		MXCLKPLL_CONTROL	*(volatile U32 *)0x6BE00070
void SM718_MXCLKControl(void) {    
	MXCLKPLL_CONTROL_2 = (0 << 2)  | 	// PLL No Bypass(0)
	                     (1 << 1)  |	// PLL Power ON	
						  0;			// PLL CLK Select(0:XTal)
	MXCLKPLL_CONTROL_0 = (0 << 12) | 	// PLL OD Divided by 1, 2, 4, 8(0, 1, 2, 3)
						 (4 << 8)  |	// N value(0~15)
                          92 //150
	;			//90, 110 , V4.01, 4.02===> 110 130
/*
	MXCLKPLL_CONTROL =  (0 << 18) | 	// PLL No Byapass
						(1 << 17) |		// PLL Power ON
						(0 << 16) | 	// PLL CLK Select
						(0 << 12) |
	                    (4 << 8)  |
		                130;
*/	
}

#define		HOST_CONTROL_2	*(volatile U16 *)0x6BE00076
#define		HOST_CONTROL_0	*(volatile U16 *)0x6BE00074
void SM718_HostControl(void) {
	HOST_CONTROL_0 = (1 << 15) |		// Host 16Bit Mode, GPIO23 Bit
					 (0 << 14) |		// Internal PLL, GPIO21
					 (0 << 7)  |		// Little Endian
					 (0 << 6)  |		// Ready LOW Active
					 (1 << 5)  |		// MMIO is 0x3E0_0000
					 2;					// XScale BUS , GPIO 19, 18, 17 Set
}

#define		SYSTEMMEMORY_CONTROL_2	*(volatile U16 *)0x6BE0007A
#define		SYSTEMMEMORY_CONTROL_0	*(volatile U16 *)0x6BE00078
void SM718_MemoryControl(void) {
	SYSTEMMEMORY_CONTROL_2 = (0 << 4) | 	//
							 (0 << 1) |		// System Memory Burst Length Selection
							 (1 ) ;			// 1: CAS Latency 3CLK 0:2CLK

	SYSTEMMEMORY_CONTROL_0 = (0 << 13) |	// Bus Hold Time Selection 
							 (0 << 11) | 	// Colume Size 512:1, 1024:0, 256:2
							 (4 <<  8) |	// Size 64MB(4), 32M(5)
							 (1 <<  7) |	// Delay 7Clock
							 (0 <<  6) |	// 2CLK Delay
							 (0 <<  5) |	// Bank Select(4Bank)	
							 (1 << 4)  |	// System Memory Reset(0)
							 (0 << 1)  |	// Delay 0
							  0;
}

#define		SM718_PWM0_2		*(U16 *)0x6BE10022
#define		SM718_PWM0_0		*(U16 *)0x6BE10020
#define		SM718_PWM0		    *(U32 *)0x6BE10020
void SM718_PWMSetting(void) {
	U32		data;
//	U16		hi, lo;

//기존데이터	
/*
    data = (830 << 20) | 	// High Count , 830, 186(A type)
	       (20  << 8)  | 	// Low Count  , 20, 186(A type)
		   (8 << 4) 	|	// 96M / 4096 = 23.438KHZ
		   1;				// PWM Enable
*/    
//변경데이터	
    data = (830 << 20) | 	// High Count , 830, 186(A type)
	       (20  << 8)  | 	// Low Count  , 20, 186(A type)
		   (8 << 4) 	|	// 96M / 4096 = 23.438KHZ
		   1;				// PWM Enable
	
    /*
    hi = (U16)((data >> 16) & 0xffff);
	lo = (U16)(data  & 0xffff);
	SM718_PWM0_2 = hi;
	SM718_PWM0_0 = lo;
    */    
    
	SM718_PWM0 = data;
}


void LCD_PWMSetting(int high, int low, int div) { //high=830, low=20m div=8
	U32		data;
	//U16		hi, lo;

	
    data = (high << 20) | 	// High Count , 830, 186(A type)
	       (low  << 8)  | 	// Low Count  , 20, 186(A type)
		   (div << 4) 	|	// 96M / 4096 = 23.438KHZ
		   1;				// PWM Enable
	
    /*
    hi = (U16)((data >> 16) & 0xffff);
	lo = (U16)(data  & 0xffff);
	SM718_PWM0_2 = hi;
	SM718_PWM0_0 = lo;
    */    
	SM718_PWM0 = data;
}


/* 640 x 480  [4:3] */
// { 800, 640, 656, 96, NEG, 525, 480, 490, 2, NEG, 25175000, 31469, 60, NEG},
// { 840, 640, 656, 64, NEG, 500, 480, 481, 3, NEG, 31500000, 37500, 75, NEG},
// { 832, 640, 696, 56, NEG, 509, 480, 481, 3, NEG, 36000000, 43269, 85, NEG},

/* 800 X 600 */
// {1056, 800, 840,128, POS, 628, 600, 601, 4, POS, 40000000, 37879, 60, NEG},
// {1056, 800, 816, 80, POS, 625, 600, 601, 3, POS, 49500000, 46875, 75, NEG},
// {1048, 800, 832, 64, POS, 631, 600, 601, 3, POS, 56250000, 53674, 85, NEG},

/* 1024 x 768  [4:3] */
// {1344,1024,1048,136, NEG, 806, 768, 771, 6, NEG, 65000000, 48363, 60, NEG},
// {1312,1024,1040, 96, POS, 800, 768, 769, 3, POS, 78750000, 60023, 75, NEG},
// {1376,1024,1072, 96, POS, 808, 768, 769, 3, POS, 94500000, 68677, 85, NEG},

/* 1152 x 864  [4:3] -- Widescreen eXtended Graphics Array */
// {1600,1152,1216,128, POS, 900, 864, 865, 3, POS,108000000, 67500, 75, NEG},

/* 1280 x 1024 [5:4] */
// {1688,1280,1328,112, POS,1066,1024,1025, 3, POS,108000000, 63981, 60, NEG},
// {1688,1280,1296,144, POS,1066,1024,1025, 3, POS,135000000, 79976, 75, NEG},
// {1728,1280,1344,160, POS,1072,1024,1025, 3, POS,157500000, 91146, 85, NEG},

/* 1360 x 768 [16:9] */
// {1792,1280,1424,112, POS, 795, 768, 771, 6, POS, 85500000, 47712, 60, NEG},
 //800X480
// { 973, 800, 822, 56, POS, 524, 480, 490, 2, NEG, 30600000, 31449, 60, NEG},
#define	DISPLAY_MODE			MODE_800600

#ifdef LCD_TAIWAN_CLAA080
// 8Inch LCD
#define	CRT_HORIZONTAL_MAX		1016
#define CRT_WIDTH				800
#define	CRT_HSYNC_START			840		//줄이면 화면이 우측으로간다.
#define CRT_HSYNC_WIDTH			128

#define CRT_HEIGHT_TATAL		628
#define CRT_HEIGHT				600
#define CRT_VSYNC_START			601
#define CRT_VSYNC_WIDTH			4
#endif


#ifdef LCD_SAMSUNG_LMS700KF06
//7Inch LCD
#define	CRT_HORIZONTAL_MAX		830
#define CRT_WIDTH				800
#define	CRT_HSYNC_START			816		//줄이면 화면이 우측으로간다.
#define CRT_HSYNC_WIDTH			6

#define CRT_HEIGHT_TATAL		495
#define CRT_HEIGHT				480
#define CRT_VSYNC_START			487
#define CRT_VSYNC_WIDTH			2
#endif



#ifdef LCD_5Inch
//7Inch LCD
#define	CRT_HORIZONTAL_MAX		1056
#define CRT_WIDTH				800
#define	CRT_HSYNC_START			1000		//줄이면 화면이 우측으로간다.
#define CRT_HSYNC_WIDTH			10

#define CRT_HEIGHT_TATAL		525
#define CRT_HEIGHT				480
#define CRT_VSYNC_START			490
#define CRT_VSYNC_WIDTH			12
#endif

#ifdef LCD_TS8080
#define	CRT_HORIZONTAL_MAX		1344
#define CRT_WIDTH				LCD_X        // 
#define	CRT_HSYNC_START			1250		//줄이면 화면이 우측으로간다.
#define CRT_HSYNC_WIDTH			90

#define CRT_HEIGHT_TATAL		845
#define CRT_HEIGHT				LCD_Y
#define CRT_VSYNC_START			820
#define CRT_VSYNC_WIDTH			10
#endif

#ifdef LCD_1024
#define	CRT_HORIZONTAL_MAX		1344
#define CRT_WIDTH				1024        // 
#define	CRT_HSYNC_START			1250		//줄이면 화면이 우측으로간다.
#define CRT_HSYNC_WIDTH			90

#define CRT_HEIGHT_TATAL		845
#define CRT_HEIGHT				768
#define CRT_VSYNC_START			820
#define CRT_VSYNC_WIDTH			10
#endif

#ifdef LCD1024X600
#define	CRT_HORIZONTAL_MAX		1344
#define CRT_WIDTH				1024        // 
#define	CRT_HSYNC_START			1250		//줄이면 화면이 우측으로간다.
#define CRT_HSYNC_WIDTH			90

#define CRT_HEIGHT_TATAL		635
#define CRT_HEIGHT				600
#define CRT_VSYNC_START			620
#define CRT_VSYNC_WIDTH			10
#endif

#ifdef LCD17INCH
#define	CRT_HORIZONTAL_MAX		1390
#define CRT_WIDTH				LCD_X        // 
#define	CRT_HSYNC_START			1290		//줄이면 화면이 우측으로간다.
#define CRT_HSYNC_WIDTH			90

#define CRT_HEIGHT_TATAL		1066
#define CRT_HEIGHT				LCD_Y	//1024
#define CRT_VSYNC_START			1050
#define CRT_VSYNC_WIDTH			10
#endif

#define	LCD_DISPLAY_CONTROL			*(volatile U32 *)0x6be80000

#define	LCD_DISPLAY_CONTROL_2		*(volatile U16 *)0x6be80002
#define	LCD_DISPLAY_CONTROL_0		*(volatile U16 *)0x6be80000

#define	LCD_FB_ADDRESS_2			*(volatile U16 *)0x6be8000e
#define	LCD_FB_ADDRESS_0			*(volatile U16 *)0x6be8000c

#define	LCD_OFFSET_2				*(volatile U16 *)0x6be80012
#define	LCD_OFFSET_0				*(volatile U16 *)0x6be80010

#define	LCD_X_WIDTH_2				*(volatile U16 *)0x6be80016
#define	LCD_X_WIDTH_0				*(volatile U16 *)0x6be80014

#define	LCD_Y_HEIGHT_2				*(volatile U16 *)0x6be8001A
#define	LCD_Y_HEIGHT_0				*(volatile U16 *)0x6be80018

#define	LCD_TL_LOCATION_2			*(volatile U16 *)0x6be8001e
#define	LCD_TL_LOCATION_0			*(volatile U16 *)0x6be8001c

#define	LCD_BR_LOCATION_2			*(volatile U16 *)0x6be80022
#define	LCD_BR_LOCATION_0			*(volatile U16 *)0x6be80020

#define	LCD_H_TOTAL_2				*(volatile U16 *)0x6be80026
#define	LCD_H_TOTAL_0				*(volatile U16 *)0x6be80024

#define	LCD_HSYNC_2					*(volatile U16 *)0x6be8002A
#define	LCD_HSYNC_0					*(volatile U16 *)0x6be80028

#define	LCD_V_TOTAL_2				*(volatile U16 *)0x6be8002E
#define	LCD_V_TOTAL_0				*(volatile U16 *)0x6be8002c

#define	LCD_V_SYNC_2				*(volatile U16 *)0x6be80032
#define	LCD_V_SYNC_0				*(volatile U16 *)0x6be80030

void LCD_ON(void)
{
	U32	 	pData;

	pData = 			  (0 << 28) |	// Panel Data
						  (1 << 27) | 	// FPEN High Drive
	                      (1 << 26) | 	// VBIAS Out Pin
	                      (1 << 25) | 	// Panel Control Signal and Data Line Enable
	                      (1 << 24) | 	// FPVDDEN High Drive

	                      (1 << 19) | 	// Dual Digital Display Enable=1
	                      
	                      (0 << 18) | 	// Double Pixel Out
	                      (1 << 16) | 	// Fifo Level
	                      
	                      (0 << 14) | 	// Clock Phase High=0
	                      (1 << 13) | 	// Vertical Sync Low
	                      (1 << 12) | 	// H Sync Low
	                      
	                      (0 << 10 ) | 	// Lock Panel Timming

	                      (0 << 9 ) | 	// Disable Color Key
	                      (1 << 8) |  	// Timing Enable
	                      
	                      (0 << 7) |   	// Vertical Panning Down
	                      (0 << 6) |   	// Vertical Paning Dsiable
	                      (0 << 5) |  	// H Panning Right
	                      (0 << 4) |  	// H Panning Disable
	                      (0 << 3) |  	// Disable Gamma Coontrol
	                      
	                      (1 << 2) |  	// Enable Panle Graphivs
	                       1  ;  	  	//16Bit RGB

   	LCD_DISPLAY_CONTROL = pData; 
}
void LCD_OFF(void)
{
	U32	 	pData;

	pData = 			  (0 << 28) |	// Panel Data
						  (1 << 27) | 	// FPEN High Drive
	                      (1 << 26) | 	// VBIAS Out Pin
	                      (1 << 25) | 	// Panel Control Signal and Data Line Enable
	                      (1 << 24) | 	// FPVDDEN High Drive

	                      (1 << 19) | 	// Dual Digital Display Enable=1
	                      
	                      (0 << 18) | 	// Double Pixel Out
	                      (1 << 16) | 	// Fifo Level
	                      
	                      (0 << 14) | 	// Clock Phase High=0
	                      (1 << 13) | 	// Vertical Sync Low
	                      (1 << 12) | 	// H Sync Low
	                      
	                      (0 << 10 ) | 	// Lock Panel Timming

	                      (0 << 9 ) | 	// Disable Color Key
	                      (1 << 8) |  	// Timing Enable
	                      
	                      (0 << 7) |   	// Vertical Panning Down
	                      (0 << 6) |   	// Vertical Paning Dsiable
	                      (0 << 5) |  	// H Panning Right
	                      (0 << 4) |  	// H Panning Disable
	                      (0 << 3) |  	// Disable Gamma Coontrol
	                      
	                      (0 << 2) |  	// Enable Panle Graphivs
	                       1  ;  	  	//16Bit RGB

   	LCD_DISPLAY_CONTROL = pData; 
}


#define	LCD_FB_ADDRESS			*(volatile U32 *)0x6be8000c
U32 GetGraphicsAddr(int Page_No);
void SM718_LCDScreenChange(int Page_No)
{
	U32	 	pData, addr;
	addr = GetGraphicsAddr(Page_No);

	pData = 		 (0 << 31) | 	// 0:No Flip Pending 1:Flip Pending
				     (0 << 27) | 	// 0:Local Memory 1:External Memory
				     (addr ) ; 		// Bit25~4(Frame Buffer Address)

	LCD_FB_ADDRESS = pData; 
	SetGraphicsPage(Page_No);
}

void SM718_LCDInit(void)
{
	U16		hi, lo;
	U32	 	pData;

	pData = 		 (0 << 31) | 	// 0:No Flip Pending 1:Flip Pending
				     (0 << 27) | 	// 0:Local Memory 1:External Memory
				     (0 ) ; 		// Bit25~4(Frame Buffer Address)
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_FB_ADDRESS_2 = hi;
	LCD_FB_ADDRESS_0 = lo; 

	pData = 	((CRT_WIDTH /8) << 20)| //Bit29~20
				((CRT_WIDTH /8) << 4);	//Bit13~ 4
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_OFFSET_2 = hi;
	LCD_OFFSET_0 = lo;	

	pData = (CRT_WIDTH << 16) | 0;
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_X_WIDTH_2 = hi; 
	LCD_X_WIDTH_0 = lo;

	pData = (CRT_HEIGHT << 16) | 0;
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_Y_HEIGHT_2 = hi; 
	LCD_Y_HEIGHT_0 = lo;

	pData = ( 0 << 16 ) | 0;
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_TL_LOCATION_2 = hi; 
	LCD_TL_LOCATION_0 = lo;

	pData = ((CRT_HEIGHT-1) << 16) | (CRT_WIDTH-1);
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_BR_LOCATION_2 = hi; 
	LCD_BR_LOCATION_0 = lo; 

	pData = (CRT_HORIZONTAL_MAX << 16) | //수평최대값
	                       CRT_WIDTH-1 ;					//수평실제값
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_H_TOTAL_2 = hi; 
	LCD_H_TOTAL_0 = lo; 

	pData = (CRT_HSYNC_WIDTH << 15) |		//수평동기펄스폭
	                      CRT_HSYNC_START;				//수평동기 시작위치
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_HSYNC_2 = hi; 
	LCD_HSYNC_0 = lo; 
	
	pData = (CRT_HEIGHT_TATAL << 16) | 	//수직최대라인수
	                     CRT_HEIGHT-1;	//수직실제값
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_V_TOTAL_2 = hi; 
	LCD_V_TOTAL_0 = lo; 
	
	pData = (CRT_VSYNC_WIDTH << 16) | 	//수직동기펄스폭
	                    CRT_VSYNC_START;			//수직동기 시작위치
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	LCD_V_SYNC_2 = hi;
	LCD_V_SYNC_0 = lo;

	pData = 		  (0 << 28) |	// Panel Data
						  (1 << 27) | 	// FPEN High Drive
	                      (1 << 26) | 	// VBIAS Out Pin
	                      (1 << 25) | 	// Panel Control Signal and Data Line Enable
	                      (1 << 24) | 	// FPVDDEN High Drive

	                      (1 << 19) | 	// Dual Digital Display Enable=1
	                      
	                      (0 << 18) | 	// Double Pixel Out
	                      (1 << 16) | 	// Fifo Level
	                      
	                      (0 << 14) | 	// Clock Phase High=0
	                      (1 << 13) | 	// Vertical Sync Low
	                      (1 << 12) | 	// H Sync Low
	                      
	                      (0 << 10 ) | 	// Lock Panel Timming

	                      (0 << 9 ) | 	// Disable Color Key
	                      (1 << 8) |  	// Timing Enable
	                      
	                      (0 << 7) |   	// Vertical Panning Down
	                      (0 << 6) |   	// Vertical Paning Dsiable
	                      (0 << 5) |  	// H Panning Right
	                      (0 << 4) |  	// H Panning Disable
	                      (0 << 3) |  	// Disable Gamma Coontrol
	                      
	                      (0 << 2) |  	// Enable Panle Graphivs
	                       1  ;  	  	//16Bit RGB
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	LCD_DISPLAY_CONTROL_2 = hi; 
   	LCD_DISPLAY_CONTROL_0 = lo; 
}


void SM718_LCDClear(U16 color)
{
	U32		address=0x68000000;
	int		i;
	
	for(i=0; i<(LCD_X*LCD_Y); i++)
	{
		*(U16  *)address= color;
		address+=2;
	}
}


#define	CRT_DISPLAY_CONTROL_2		*(volatile U16 *)0x6be80202
#define	CRT_DISPLAY_CONTROL_0		*(volatile U16 *)0x6be80200

#define	CRT_FB_ADDRESS_2			*(volatile U16 *)0x6be80206
#define	CRT_FB_ADDRESS_0			*(volatile U16 *)0x6be80204

#define	CRT_OFFSET_2				*(volatile U16 *)0x6be8020A
#define	CRT_OFFSET_0				*(volatile U16 *)0x6be80208

#define	CRT_H_TOTAL_2				*(volatile U16 *)0x6be8020E
#define	CRT_H_TOTAL_0				*(volatile U16 *)0x6be8020C

#define	CRT_HSYNC_2					*(volatile U16 *)0x6be80212
#define	CRT_HSYNC_0					*(volatile U16 *)0x6be80210

#define	CRT_V_TOTAL_2				*(volatile U16 *)0x6be80216
#define	CRT_V_TOTAL_0				*(volatile U16 *)0x6be80214

#define	CRT_V_SYNC_2				*(volatile U16 *)0x6be8021A
#define	CRT_V_SYNC_0				*(volatile U16 *)0x6be80218

#define	CRT_TL_LOCATION_2			*(volatile U16 *)0x6be80282
#define	CRT_TL_LOCATION_0			*(volatile U16 *)0x6be80280

#define	CRT_BR_LOCATION_2			*(volatile U16 *)0x6be80286
#define	CRT_BR_LOCATION_0			*(volatile U16 *)0x6be80284


void SM718_CRTInit(void)
{
	U16		hi, lo;
	U32	 	pData;

	pData = 		 (0 << 31) | 	// 0:No Flip Pending 1:Flip Pending
				     (0 << 27) | 	// 0:Local Memory 1:External Memory
				     (0 ) ; 		// Bit25~4(Frame Buffer Address)
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	CRT_FB_ADDRESS_2 = hi;
	CRT_FB_ADDRESS_0 = lo; 

	pData = 	((CRT_WIDTH /8) << 20)| //Bit29~20
				((CRT_WIDTH /8) << 4);	//Bit13~ 4
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	CRT_OFFSET_2 = hi;
	CRT_OFFSET_0 = lo;	

	pData = ( 0 << 16 ) | 0;
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	CRT_TL_LOCATION_2 = hi; 
	CRT_TL_LOCATION_0 = lo;

	pData = ((CRT_HEIGHT-1) << 16) | (CRT_WIDTH-1);
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	CRT_BR_LOCATION_2 = hi; 
	CRT_BR_LOCATION_0 = lo; 

	pData = (CRT_HORIZONTAL_MAX << 16) | //수평최대값
	                       CRT_WIDTH-1 ;					//수평실제값
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	CRT_H_TOTAL_2 = hi; 
	CRT_H_TOTAL_0 = lo; 

	pData = (CRT_HSYNC_WIDTH << 15) |		//수평동기펄스폭
	                      CRT_HSYNC_START;				//수평동기 시작위치
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	CRT_HSYNC_2 = hi; 
	CRT_HSYNC_0 = lo; 
	
	pData = (CRT_HEIGHT_TATAL << 16) | 	//수직최대라인수
	                     CRT_HEIGHT-1;	//수직실제값
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	CRT_V_TOTAL_2 = hi; 
	CRT_V_TOTAL_0 = lo; 
	
	pData = (CRT_VSYNC_WIDTH << 16) | 	//수직동기펄스폭
	                    CRT_VSYNC_START;			//수직동기 시작위치
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
	CRT_V_SYNC_2 = hi;
	CRT_V_SYNC_0 = lo;

	pData = 			  
	                      (0 << 26) | 	// 0:VGA Data SHIFT Enable
	                      (0 << 24) | 	// 0:H Auto Center Disable
	                      (1 << 23) | 	// 
	                      (0 << 22) | 	// 
	                      (0 << 21) | 	// 
	                      (0 << 20) | 	// 

	                      (1 << 10) | 	// 0: Panel Data
	                      (1 << 16) | 	// Fifo Level
	                      
	                      (0 << 14) | 	// Clock Phase High=0
	                      (1 << 13) | 	// Vertical Sync Low
	                      (1 << 12) | 	// H Sync Low
	                      
	                      (0 << 10 ) | 	// 0:CRT will show pixels
	                      (1 << 8) |  	// Timing Enable
	                      
	                      (0 << 3) |  	// Disable Gamma Coontrol
	                      (1 << 2) |  	// Enable Panle Graphivs
	                       1  ;  	  	//16Bit RGB
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	CRT_DISPLAY_CONTROL_2 = hi; 
   	CRT_DISPLAY_CONTROL_0 = lo; 
	//-----------------------------------------------------
	pData = 			  (0 << 28) |	// Panel Data
						  (1 << 27) | 	// FPEN High Drive
	                      (1 << 26) | 	// VBIAS Out Pin
	                      (1 << 25) | 	// Panel Control Signal and Data Line Enable
	                      (1 << 24) | 	// FPVDDEN High Drive

	                      (1 << 19) | 	// Dual Digital Display Enable=1
	                      
	                      (0 << 18) | 	// Double Pixel Out
	                      (1 << 16) | 	// Fifo Level
	                      
	                      (0 << 14) | 	// Clock Phase High=0
	                      (1 << 13) | 	// Vertical Sync Low
	                      (1 << 12) | 	// H Sync Low
	                      
	                      (0 << 10 ) | 	// Lock Panel Timming

	                      (0 << 9 ) | 	// Disable Color Key
	                      (1 << 8) |  	// Timing Enable
	                      
	                      (0 << 7) |   	// Vertical Panning Down
	                      (0 << 6) |   	// Vertical Paning Dsiable
	                      (0 << 5) |  	// H Panning Right
	                      (0 << 4) |  	// H Panning Disable
	                      (0 << 3) |  	// Disable Gamma Coontrol
	                      
	                      (1 << 2) |  	// Enable Panle Graphivs
	                       1  ;  	  	// 16Bit RGB
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	LCD_DISPLAY_CONTROL_2 = hi; 
   	LCD_DISPLAY_CONTROL_0 = lo; 

}


void SM718_AlphaClear(U16 color)
{
	U32		address=(0x68000000 + (LCD_X*LCD_Y*2));
	int		i;
	
	for(i=0; i<(LCD_X*LCD_Y); i++)
	{
		*(U16  *)address= color;
		address+=2;
	}
	/*
	address=(0x68000000 + (800*600*2));
	for(i=0; i<((LCD_X*LCD_Y)/2); i++)
	{
		*(U16  *)address= color;
		address+=2;
	}
	*/
}

#define		ALPHA_DISPLAY_CONTROL_2		*(volatile U16 *)0x6be80102
#define		ALPHA_DISPLAY_CONTROL_0		*(volatile U16 *)0x6be80100

#define		ALPHA_FRAME_ADDRESS_2		*(volatile U16 *)0x6be80106
#define		ALPHA_FRAME_ADDRESS_0		*(volatile U16 *)0x6be80104

#define		ALPHA_WIDTH_OFFSET_2		*(volatile U16 *)0x6be8010a
#define		ALPHA_WIDTH_OFFSET_0		*(volatile U16 *)0x6be80108

#define		ALPHA_TL_POSITION_2			*(volatile U16 *)0x6be8010e
#define		ALPHA_TL_POSITION_0			*(volatile U16 *)0x6be8010C

#define		ALPHA_BR_POSITION_2			*(volatile U16 *)0x6be80112
#define		ALPHA_BR_POSITION_0			*(volatile U16 *)0x6be80110

#define		ALPHA_CROMAKEY_2			*(volatile U16 *)0x6be80116
#define		ALPHA_CROMAKEY_0			*(volatile U16 *)0x6be80114

#define		ALPHA_LOOKUP_TABLE			0x6be80118

#define		ALPHA_DISPLAY_CONTROL		*(volatile U32 *)0x6be80100
#define		ALPHA_FRAME_ADDRESS			*(volatile U32 *)0x6be80104
#define		ALPHA_WIDTH_OFFSET			*(volatile U32 *)0x6be80108
#define		ALPHA_TL_POSITION			*(volatile U32 *)0x6be8010C
#define		ALPHA_BR_POSITION			*(volatile U32 *)0x6be80110
#define		ALPHA_CROMAKEY				*(volatile U32 *)0x6be80114

void SM718_AlphaInit(void)
{
	U32		pData;
	U16		hi, lo;

	//---------------------------------------------------------------------------
	pData = 	(0 << 31) |			
				(0 << 27) |	 //Local Memory
                (LCD_X*LCD_Y*2);
	
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	ALPHA_FRAME_ADDRESS_2 = hi; 
   	ALPHA_FRAME_ADDRESS_0 = lo; 

	//---------------------------------------------------------------------------
	pData = 	((CRT_WIDTH /8) << 20)| //Bit29~20
				((CRT_WIDTH /8) << 4);	//Bit13~ 4
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	ALPHA_WIDTH_OFFSET_2 = hi; 
   	ALPHA_WIDTH_OFFSET_0 = lo; 
	
	//---------------------------------------------------------------------------
	pData = ( 0 << 16 )| 0;
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	ALPHA_TL_POSITION_2 = hi; 
   	ALPHA_TL_POSITION_0 = lo; 

	//---------------------------------------------------------------------------
	pData = ((CRT_HEIGHT-1) << 16) | (CRT_WIDTH-1);
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	ALPHA_BR_POSITION_2 = hi; 
   	ALPHA_BR_POSITION_0 = lo; 
	
	//---------------------------------------------------------------------------
	pData=			0x0;
	hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	ALPHA_CROMAKEY_2 = hi; 
   	ALPHA_CROMAKEY_0 = lo; 

	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+2) = 0x8000;
	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0) = 0x0000;

	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+6) = 0x0400;
	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+4) = 0x8400;

	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x0a) = 0x0010;
	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+8) = 0x1010;

	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x0e) = 0x0410;
	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0xC) = 0xb618;

	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x12) = 0x38e7;
	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x10) = 0xf800;

	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x16) = 0x07e0;
	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x14) = 0xffe0;

	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x1A) = 0x001f;
	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x18) = 0xf81f;

	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x1e) = 0x07ff;
	*(volatile U16 *)(ALPHA_LOOKUP_TABLE+0x1c) = 0x0fff;

	//---------------------------------------------------------------------------
	pData = 	(1 << 28)  |		//투명도를 사용한다.
		        (10 << 24)  |		//투명도 1이 제일 투명하고 15가 뚜렷하다.
		        (1 << 16)  |
		        (0 << 4)   |
		        (1 << 3)   | 
		        (0 << 2)   |
		         1;
   	ALPHA_DISPLAY_CONTROL = pData; 
}

//LCD Alpha Screen 을 ON/OFF 하는 함수이다.
void LCD_AlphaScreen(int s)
{
	U32		pData = ALPHA_DISPLAY_CONTROL;
	if(s==ON)
	{
		pData |= 0x00000004;		
	}
	else
	{
		pData &= ~0x00000004;
	}
	ALPHA_DISPLAY_CONTROL = pData;
}

void Lcd_AlphaTransparency(int value)
{
	U32		val = value;
	U32		pData = ALPHA_DISPLAY_CONTROL;
	val <<= 24;
	val &= 0x0f000000;
	pData &= ~0x0f000000;
	pData = pData | val;
	ALPHA_DISPLAY_CONTROL = pData;	
    //Dprintf("Lcd_AlphaTransparency:: pData = %08x\n", pData);    
}

#define		VIDEO_CONTROL_2			*(volatile U16 *)0x6be80042
#define		VIDEO_CONTROL_0			*(volatile U16 *)0x6be80040

#define		VIDEO_FB0_ADDRESS_2		*(volatile U16 *)0x6be80046
#define		VIDEO_FB0_ADDRESS_0		*(volatile U16 *)0x6be80044

#define		VIDEO_FRB_WIDTH_2		*(volatile U16 *)0x6be8004a
#define		VIDEO_FRB_WIDTH_0		*(volatile U16 *)0x6be80048

#define		VIDEO_FB0_LASTADDR_2	*(volatile U16 *)0x6be8004e
#define		VIDEO_FB0_LASTADDR_0	*(volatile U16 *)0x6be8004c

#define		VIDEO_TL_LOCATION_2		*(volatile U16 *)0x6be80052
#define		VIDEO_TL_LOCATION_0		*(volatile U16 *)0x6be80050

#define		VIDEO_BR_LOCATION_2		*(volatile U16 *)0x6be80056
#define		VIDEO_BR_LOCATION_0		*(volatile U16 *)0x6be80054

#define		VIDEO_SCALE_REG_2		*(volatile U16 *)0x6be8005a
#define		VIDEO_SCALE_REG_0		*(volatile U16 *)0x6be80058

#define		VIDEO_INIT_SCALE_2		*(volatile U16 *)0x6be8005e
#define		VIDEO_INIT_SCALE_0		*(volatile U16 *)0x6be8005c

#define		VIDEO_YUV_2				*(volatile U16 *)0x6be80062
#define		VIDEO_YUV_0				*(volatile U16 *)0x6be80060

#define		VIDEO_FB1_ADDRESS_2		*(volatile U16 *)0x6be80066
#define		VIDEO_FB1_ADDRESS_0		*(volatile U16 *)0x6be80064

#define		VIDEO_FB1_LASTADDR_2	*(volatile U16 *)0x6be8006a
#define		VIDEO_FB1_LASTADDR_0	*(volatile U16 *)0x6be80068
//32Bit
#define		VIDEO_CONTROL			*(volatile U32 *)0x6be80040
#define		VIDEO_FB0_ADDRESS		*(volatile U32 *)0x6be80044
#define		VIDEO_FRB_WIDTH			*(volatile U32 *)0x6be80048
#define		VIDEO_FB0_LASTADDR		*(volatile U32 *)0x6be8004c
#define		VIDEO_TL_LOCATION		*(volatile U32 *)0x6be80050
#define		VIDEO_BR_LOCATION		*(volatile U32 *)0x6be80054
#define		VIDEO_SCALE_REG			*(volatile U32 *)0x6be80058
#define		VIDEO_INIT_SCALE		*(volatile U32 *)0x6be8005c
#define		VIDEO_YUV				*(volatile U32 *)0x6be80060
#define		VIDEO_FB1_ADDRESS		*(volatile U32 *)0x6be80064
#define		VIDEO_FB1_LASTADDR		*(volatile U32 *)0x6be80068

#define		VIDEO_X		1024//LCD_X//1600//640//701//1280 	//680
#define		VIDEO_Y		654//LCD_Y//1200//480//501//1024 	//470

#define		VIDEO_SX	0
#define		VIDEO_SY	50

void SM718_VideoInit(void)
{
	uint32_t		pData;
	uint16_t		hi, lo;

	pData = (0 << 31) | 	//No Flip
	         (0 << 27) | 	//Local Memory
	        //((VIDEO_X*VIDEO_Y*20));//VIDEO_SCREEN_ADDR
			LCD_ALPHA_CONTAINER_ADDR;

	hi = (uint16_t)((pData >> 16) & 0xffff);
	lo = (uint16_t)(pData & 0xffff);

    VIDEO_FB0_ADDRESS_2 = hi;
   	VIDEO_FB0_ADDRESS_0 = lo;
	
	pData	= ((VIDEO_X / 8) << 20) |		//Width, ??? ??????? 8? ???.
	          ((VIDEO_X / 8) << 4)  |		//Offset
	          (0 << 16) |
			   0;

	hi = (uint16_t)((pData >> 16) & 0xffff);
	lo = (uint16_t)(pData & 0xffff);

    VIDEO_FRB_WIDTH_2 = hi;
   	VIDEO_FRB_WIDTH_0 = lo;


	pData = (0 << 27) |	//Local Memory
			LCD_ALPHA_CONTAINER_ADDR + (VIDEO_X * VIDEO_Y * 2);
			//((VIDEO_X*VIDEO_Y)*20)+(VIDEO_X*VIDEO_Y*2 );//VIDEO_SCREEN_ADDR

	hi = (uint16_t)((pData >> 16) & 0xffff);
	lo = (uint16_t)(pData & 0xffff);


    VIDEO_FB0_LASTADDR_2 = hi;
   	VIDEO_FB0_LASTADDR_0 = lo;


	pData = (VIDEO_SY << 16) | 		//T
	         VIDEO_SX;				//L
	hi = (uint16_t)((pData >> 16) & 0xffff);
	lo = (uint16_t)(pData & 0xffff);

	VIDEO_TL_LOCATION_2 = hi;
   	VIDEO_TL_LOCATION_0 = lo;


	pData = ((VIDEO_Y+VIDEO_SY-1) << 16) |  //B
	         (VIDEO_X+VIDEO_SX-1);			//R
	hi = (uint16_t)((pData >> 16) & 0xffff);
	lo = (uint16_t)(pData & 0xffff);


   	VIDEO_BR_LOCATION_2 = hi;
   	VIDEO_BR_LOCATION_0 = lo;

	//??? ?? ?? 0??? ????? 1:1 ? ??.
   	//HDMI
   	hi=0;
	lo=0;
	//DVD FULL IMGAE
	//hi = 1255;
	//lo = 2014;
	//DVD
	hi = gHValue;
	lo = gLValue;

    //hi = 4095;
	//lo = 4095;

	VIDEO_SCALE_REG_2 = hi;
   	VIDEO_SCALE_REG_0 = lo;

   	VIDEO_INIT_SCALE_2 = hi;
   	VIDEO_INIT_SCALE_0 = lo;

	pData = 0x00ededed;
	hi = (uint16_t)((pData >> 16) & 0xffff);
	lo = (uint16_t)(pData & 0xffff);
   	VIDEO_YUV_2 = hi;
   	VIDEO_YUV_0 = lo;

    //(LCD_X*LCD_Y*4)+(VIDEO_X*VIDEO_Y*2)  //FB0

#if 0 //??????.

	pData =  (0 << 31) | 		//No Flip
                (0 << 27) | 		//Local Memory
	        (VIDEO_X*VIDEO_Y*6);
            //(LCD_X*LCD_Y*4)+(VIDEO_X*VIDEO_Y*2);
	hi = (uint16_t)((pData >> 16) & 0xffff);
	lo = (uint16_t)(pData & 0xffff);
   	VIDEO_FB1_ADDRESS_2 = hi;
   	VIDEO_FB1_ADDRESS_0 = lo;
   	//Dprintf("Video Frame Buf1 Start=%08x\n",((LCD_X*LCD_Y*4)+(VIDEO_X*VIDEO_Y*2)) + 0x68000000);


	pData = (0 << 27) |	//Local Memory
	                     (0 << 26) |	//CS0 Select
	                     //((LCD_X*LCD_Y*6)+(VIDEO_X*VIDEO_Y*2));
                        (VIDEO_X*VIDEO_Y*4)+(VIDEO_X*VIDEO_Y*4)-1;
	hi = (uint16_t)((pData >> 16) & 0xffff);
	lo = (uint16_t)(pData & 0xffff);
   	VIDEO_FB1_LASTADDR_2 = hi;
   	VIDEO_FB1_LASTADDR_0 = lo;
   	//Dprintf("Video Frame Buf1 Last=%08x\n",((LCD_X*LCD_Y*4)+(VIDEO_X*VIDEO_Y*4))-1 + 0x68000000);
#endif
	//Video FB Edge Control Register ? ????? ?? ????

	pData = 		(1 << 18) | 		//Line Buffer 1:??????, 0:????
					(2 << 16) |	//FIFO1
	                (1 << 14) | 	//1:Video Frame as Capture Frame Buffer Enable default:1
	                (0 << 13) |	//1:Enable Doubling Buffer
	                (0 << 12) | 	//1:UYVY 0:YUYV
	                (0 << 11) | 	//0:Vertical Scale Factor 1/2 Disable
	                (0 << 10) | 	//0:Horizontal Scale Factor 1/2 Disable
	                (0 << 9)  | 	//1:Enable Vertical Interpolation
	                (0 << 8)  | 	//1:Enable H Interpolation
	                (1 << 2)  |	//1:Enable Video Plane
	                 1;			// Video Plan Format -> 1:16BIT-RGB MODE 3:16Bit YUYV Mode

	VIDEO_CONTROL =  pData;
}

void VideoScreen(int s)
{
	U32		pData = VIDEO_CONTROL;
	if(s==ON)
	{
		pData |= 0x00000004;
	}
	else
	{
		pData &= ~0x00000004;
	}	
	VIDEO_CONTROL =  pData;
}

#define		CAPTURE_CONTROL		*(volatile U32 *)0x6be90000
#define		CAPTURE_CONTROL_2		*(volatile U16 *)0x6be90002
#define		CAPTURE_CONTROL_0		*(volatile U16 *)0x6be90000

#define		CAPTURE_CLIPPING		*(volatile U32 *)0x6be90004
#define		CAPTURE_CLIPPING_2		*(volatile U16 *)0x6be90006
#define		CAPTURE_CLIPPING_0		*(volatile U16 *)0x6be90004

#define		CAPTURE_SIZE			*(volatile U32 *)0x6be90008
#define		CAPTURE_SIZE_2			*(volatile U16 *)0x6be9000a
#define		CAPTURE_SIZE_0			*(volatile U16 *)0x6be90008

#define		CAPTURE_BUFFR0		    *(volatile U32 *)0x6be9000c
#define		CAPTURE_BUFFR0_2		*(volatile U16 *)0x6be9000e
#define		CAPTURE_BUFFR0_0		*(volatile U16 *)0x6be9000c

#define		CAPTURE_BUFFR1		*(volatile U32 *)0x6be90010
#define		CAPTURE_BUFFR1_2		*(volatile U16 *)0x6be90012
#define		CAPTURE_BUFFR1_0		*(volatile U16 *)0x6be90010

#define		CAPTURE_BUFFR_OFF		*(volatile U32 *)0x6be90014
#define		CAPTURE_BUFFR_OFF_2		*(volatile U16 *)0x6be90016
#define		CAPTURE_BUFFR_OFF_0		*(volatile U16 *)0x6be90014

#define		CAPTURE_FIFO			*(volatile U32 *)0x6be90018
#define		CAPTURE_FIFO_2			*(volatile U16 *)0x6be9001a
#define		CAPTURE_FIFO_0			*(volatile U16 *)0x6be90018

#define		CVIDEO_X		VIDEO_X//768//1600//1024//640//701//1280 	//680
#define		CVIDEO_Y		VIDEO_Y//494//1200//768//480//501//1024 	//470

int gCaptureX=500, gCaptureY=300;
void SM718_CaptureInit(void)
{
	uint32_t		pData;
//	U16		hi, lo;

	//HDMI?????
	pData = ( 0 << 16) | 0;//????????
	//DVD????
	//pData = ( 15 << 16) | 6;//???????? ??? ?? ???? ??

	pData = ( gHClp << 16) | gLClp;

	//pData = ( 4 << 16) | (48-10);//???????? ??? ?? ???? ??

	CAPTURE_CLIPPING = pData;

	pData = (CVIDEO_Y << 16) |  CVIDEO_X;

    CAPTURE_SIZE = pData;

	//Capture ????? ??? ????? ??.
	pData = 	 (0 << 31) |	//
			 (0 << 27) |	//Local Memory
			 //(0 << 26) |	//CS0 Select
			 //((VIDEO_X*VIDEO_Y*20));
			 LCD_ALPHA_CONTAINER_ADDR;
                    
    CAPTURE_BUFFR0 = pData;


    pData = (VIDEO_X / 8 )<< 4;
	//pData = (VIDEO_X )<< 4;
    CAPTURE_BUFFR_OFF = pData;

    pData = 0x04;
   	CAPTURE_FIFO = pData;

	pData = 		  (0 << 19) |	//0: Do Not Delay HREF 1:Delay HREF by one clock
					  (0 << 18) |	//1:Enable H Averraging
	                  (0 << 17) |	//1:Enable V 1/2 Shrink
	                  (0 << 16) |	//1:Enable H 1/2 Shrink
	                  (0 << 15) |	//Field Detect Methode 1:Falling edge of VSYNC 1:Rising
	                  (0 << 14) |	//Vsync Polarity 0:Active Hi 1:Active Low
	                  (0 << 13) |	//Hsync Polarity 0:Active Hi 1:Active Low
	                  (0 << 12) |	//Clock Polarity 0:Active Hi 1:Active Low
	                  (0 << 11) |	//1:Enable UV Swap  (??? ??? ????)
	                  (0 << 10) |	//1:Enable Byte Swap
	                  (1 << 9)  |	//1:8Bit Capture, 0:16Bit Capture
	                  (0 << 8)  |	//1:RGB Capture Format, 0:YUV
	                  (1 << 7)  |	//1:Enable Field Swap (??? ???? ????)
	                  (1 << 6)  |	//1:Enable Interaced Data Capturing in Weave
	                  (0 << 5)  |	//1:Enable Interaced Data Capturing in Bob//????
	                  (0 << 4)  |	//1:Enable Double Buffering
	                  (0 << 3)  |	//0:Continuous Capture
	                  (1 << 2)  |	//1:Enable YUV to RGB Color Conversion
	                  (1 << 1)  |	//1:Enable ITU-656 Input
	                   1; 		    //1:Capture Enable
	/*
    hi = (U16)((pData >> 16) & 0xffff);
	lo = (U16)(pData & 0xffff);
   	CAPTURE_CONTROL_2 = hi;
   	CAPTURE_CONTROL_0 = lo;
    */
    CAPTURE_CONTROL = pData;
}

#define		VIDEO_ALPHA_DISPLAY_CONTROL		*(volatile U32 *)0x6be80080
#define		VIDEO_ALPHA_FRAME_ADDRESS		*(volatile U32 *)0x6be80084
#define		VIDEO_ALPHA_WIDTH_OFFSET		*(volatile U32 *)0x6be80088
#define		VIDEO_ALPHA_LAST_ADDRESS		*(volatile U32 *)0x6be8008C
#define		VIDEO_ALPHA_TL_POSITION			*(volatile U32 *)0x6be80090
#define		VIDEO_ALPHA_BR_POSITION			*(volatile U32 *)0x6be80094
#define		VIDEO_ALPHA_SCALE				*(volatile U32 *)0x6be80098
#define		VIDEO_ALPHA_INIT_SCALE			*(volatile U32 *)0x6be8009C
#define		VIDEO_ALPHA_CROMAKEY			*(volatile U32 *)0x6be800A0
#define		VIDEO_ALPHA_LOOKUP_TABLE			0x6be800A4

void SM718_VideoAlphaInit(void)
{
	VIDEO_ALPHA_FRAME_ADDRESS = (0 << 31) |				//
					 			(0 << 27) |				//Local Memory
	                			(0 << 26) |				//CS0 Select
                     			(LCD_X*LCD_Y*20);
	
	VIDEO_ALPHA_LAST_ADDRESS = 	
					 			(0 << 27) |				//Local Memory
	                			(0 << 26) |				//CS0 Select
                     			((LCD_X*LCD_Y*20)+(VIDEO_X*VIDEO_Y*2));


	VIDEO_ALPHA_WIDTH_OFFSET= 	((VIDEO_X/8) << 20) |		//Width
	                  			((VIDEO_X/8) << 4);			//Offset
                     			

	VIDEO_ALPHA_TL_POSITION = 	(VIDEO_SY << 16) | 			//T
	                    		VIDEO_SX;					//L

	VIDEO_ALPHA_BR_POSITION = 	((VIDEO_Y+VIDEO_SY-1) << 16) | 	//T
	                    		(VIDEO_X+VIDEO_SX-1);			//L

	VIDEO_ALPHA_CROMAKEY=	0;

	VIDEO_ALPHA_SCALE = 0;
	VIDEO_ALPHA_INIT_SCALE = 0;

	*(volatile U32 *)(VIDEO_ALPHA_LOOKUP_TABLE+0) = 0x80000000;
	*(volatile U32 *)(VIDEO_ALPHA_LOOKUP_TABLE+4) = 0x04008400;
	*(volatile U32 *)(VIDEO_ALPHA_LOOKUP_TABLE+8) = 0x00101010;
	*(volatile U32 *)(VIDEO_ALPHA_LOOKUP_TABLE+0xC) = 0x0410b618;
	*(volatile U32 *)(VIDEO_ALPHA_LOOKUP_TABLE+0x10) = 0x38e7f800;
	*(volatile U32 *)(VIDEO_ALPHA_LOOKUP_TABLE+0x14) = 0x07e0ffe0;
	*(volatile U32 *)(VIDEO_ALPHA_LOOKUP_TABLE+0x18) = 0x001ff81f;
	*(volatile U32 *)(VIDEO_ALPHA_LOOKUP_TABLE+0x1c) = 0x07ffffff;

	VIDEO_ALPHA_DISPLAY_CONTROL = (1 << 28) | //투명도를 사용한다.
		                    (15 << 24) |					//투명도 1이 제일 투명하고 15가 뚜렷하다.
		                    (1 << 16) |
		                    (0 << 11) |
		                    (0 << 10) |
		                    (0 << 9) |
		                    (0 << 8) |
		                    (0 << 4) |
		                    (1 << 3) |
		                    (0 << 2) |		//Video alpha OFF:0
		                    1;
	
}

void VideoAlphaScreen(int s)
{
	U32		pData = VIDEO_ALPHA_DISPLAY_CONTROL;
	if(s==ON)
	{
		pData |= 0x00000004;
	}
	else
	{
		pData &= ~0x00000004;
	}	
	VIDEO_ALPHA_DISPLAY_CONTROL =  pData;
}

int BootLogoDisplay(void);
extern volatile U32	g_GPU_addr;
void ScreenFillRect(int sx, int sy, int width, int height, int color)
{
	U32		data;
	u16		rop = 0x8000 | 0x0c;	//0x0c:ROP2    

	int		xs = sx ;
	int		ys = sy;
	Wait2D_Engine();

	POKE_32(DE_WINDOW_SOURCE_BASE     , g_GPU_addr);  //init DE_WINDOW_SOURCE_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_WINDOW_DESTINATION_BASE, g_GPU_addr);  //init DE_WINDOW_DESTINATION_BASE to 0 so that it is on-screen memory start from address 0
    POKE_32(DE_FOREGROUND , color);

	data = (xs << 16) | ys;
    //POKE_32(DE_SOURCE, data);
    POKE_32(DE_DESTINATION, data);

	data = (width << 16) | height;
    POKE_32(DE_DIMENSION  , data);

    //POKE_32(DE_PITCH, LCD_X << 16 | LCD_X);
    //POKE_32(DE_COLOR_COMPARE, RED);
    //POKE_32(DE_COLOR_COMPARE_MASK, 0xffffffff);	//1이면 해당되는 비트는 비교하게된다.
    POKE_32(DE_CONTROL, ((0x80030000) | rop));
	//Dprintf("WinFillRect2D::End\n");
    //WaitForNotBusy();
}

void BSP_ScreenClear(void)
{
    //2D 명령으로 메모리클리어하는 속도를 증가시킨다.    
	SetGraphicsPage(LCD_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	SetGraphicsPage(CONTAINER_WINDOW1_SCREEN);

	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	SetGraphicsPage(CONTAINER_WINDOW2_SCREEN);
	
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	SetGraphicsPage(CONTAINER_WINDOW3_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	
	SetGraphicsPage(CONTAINER_WINDOW4_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	
	SetGraphicsPage(CONTAINER_WINDOW5_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	

	SetGraphicsPage(CONTAINER_WIDGET1_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	SetGraphicsPage(CONTAINER_WIDGET2_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	SetGraphicsPage(CONTAINER_WIDGET3_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	SetGraphicsPage(CONTAINER_WIDGET4_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	
	SetGraphicsPage(CONTAINER_WIDGET5_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);	

	SetGraphicsPage(DOUBLE_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);			

	SetGraphicsPage(LCD_ALPHA_CONTAINER_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);		
	SetGraphicsPage(LCD_ALPHA_WIDGET_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);		
	SetGraphicsPage(LCD_ALPHA_DOUBLE_SCREEN);
	ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlack);		

    //테스트
    //SetGraphicsPage(LCD_ALPHA_SCREEN);
    //ScreenFillRect(0, 0, LCD_X, LCD_Y, clBlue);
    //Lcd_AlphaTransparency(15);
    //LCD_AlphaScreen(ON);

}

void BSP_SM718Init(void)
{
	SM718_MXCLKControl();//Dprintf("SM718_MXCLKControl\n");
	SM718_HostControl();//Dprintf("SM718_HostControl\n");
	SM718_MemoryControl();//Dprintf("SM718_MemoryControl\n");
	SM718_PowerModeControl();//Dprintf("SM718_PowerModeControl\n");
	SM718_SystemConfig();//Dprintf("SM718_SystemConfig\n");
	SM718_GPIOConfig();//Dprintf("SM718_GPIOConfig\n");

	SM718_LocalMemory_Arbitration();//Dprintf("SM718_LocalMemory_Arbitration\n");
	SM718_ArbitrationControl();//Dprintf("SM718_ArbitrationControl\n");
	SM718_InterruptMask();//Dprintf("SM718_InterruptMask\n");
	SM718_DisplayControl();//Dprintf("SM718_DisplayControl\n");
	//SM718_PWMSetting();
	
    SM718_LCDInit();//Dprintf("SM718_LCDInit\n");	
	//SM718_CRTInit();
	//SM718_AlphaInit();
	//SM718_VideoAlphaInit();
	/*< 비디오를 사용하기위해서는 아래두개의 주석을 해제해야한다. */
	//SM718_VideoInit();
	//SM718_CaptureInit();
	SM718_PWMSetting();//Dprintf("SM718_PWMSetting\n");
	//SM718Clk_Calc(68.14);	//!< CRT Monitor 의 주파수를 구할때 사용한다.
	//while(1);
}

#if 0
//!< 모니터의 주파수를 자동계산하기위한 함수이다.
//!< 수평최대픽셀 * 수직최대픽셀 * 수직주파수 = out_f
//!< dot clock을 스코프로 확인한다.
const int OD_VALUE[5] = {0, 1, 2, 4, 8};
void SM718Clk_Calc(double out_f)
{
	int		M, N, OD;
	double	value;
	
	Dprintf("SM718 PLL Start\n");
	for(M=1; M<256; M++)
	{
		for(N=1; N<16; N++)
		{
			for(OD=1; OD<=4; OD++)
			{
				value = (14.318f * (double)M) / (double)N / (double)OD_VALUE[OD];
				if(value > out_f-0.11 && value < out_f+0.11)
				{
					Dprintf("SM718 F=%3.3f PLL M=%d N=%d OD=%d\n", value, M, N, OD-1);
				}					
			}
		}		
	}
	Dprintf("SM718 PLL End\n");	
}
#endif
