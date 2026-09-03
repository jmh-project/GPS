#ifndef BSP5500_H
#define BSP5500_H

#ifdef __cplusplus
 extern "C" {
#endif
#define	    YES	    1
#define      NO		0

#define	    ON	    1
#define      OFF	0

#define	    HIGH	1
#define      LOW	0

#define		COMMON_REG		0x00

#define		SOCKET0_REG		0x08
#define		SOCKET0_TXBUF		0x10
#define		SOCKET0_RXBUF		0x18

#define		SOCKET1_REG		0x28
#define		SOCKET1_TXBUF		0x30
#define		SOCKET1_RXBUF		0x38

#define		SOCKET2_REG		0x48
#define		SOCKET2_TXBUF		0x50
#define		SOCKET2_RXBUF		0x58

#define		SOCKET3_REG		0x68
#define		SOCKET3_TXBUF		0x70
#define		SOCKET3_RXBUF		0x78

#define		SOCKET4_REG		0x88
#define		SOCKET4_TXBUF		0x90
#define		SOCKET4_RXBUF		0x98

#define		SOCKET5_REG		0xA8
#define		SOCKET5_TXBUF		0xB0
#define		SOCKET5_RXBUF		0xB8

#define		SOCKET6_REG		0xC8
#define		SOCKET6_TXBUF		0xD0
#define		SOCKET6_RXBUF		0xD8

#define		SOCKET7_REG		0xE8
#define		SOCKET7_TXBUF		0xF0
#define		SOCKET7_RXBUF		0xF8

#define		READ_MODE			0x00
#define		WRITE_MODE		0x04

#define		VDM				0x00
#define		FDM1				0x01
#define		FDM2				0x02
#define		FDM3				0x03
	 
#include <stdint.h>
void BSP_W5500_Init(void);
void W5500_IntlevelTime(void);

void Write_W5500_Reg(uint16_t addr, uint8_t bank, uint8_t *data, int length);
void Read_W5500_Reg(uint16_t addr, uint8_t bank, uint8_t *data, int length);
//내부레지스터를 건들여 리셋명령을 실행한다.
void W5500_SoftReset(void);
//게이트웨이를 설정한다.
void W5500_Set_Gateway(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
void W5500_Get_Gateway(uint8_t *data);
//서브넷마스트를 설정한다.
void W5500_Set_SubnetMask(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
void W5500_Get_SubnetMask(uint8_t *data);
//MAC 어드레스를 설정한다.
void W5500_Set_MacAddr(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f);
void W5500_Get_MacAddr(uint8_t *data);
//Source IP를 설정한다.
void W5500_Set_IP(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
void W5500_Get_IP(uint8_t *data);

uint8_t LAN_SPI1_Send(uint8_t data);

extern volatile unsigned int gLanSendFlag;
extern volatile unsigned int gDumaSendFlag;
extern volatile unsigned int gAutoSendFlag;
extern unsigned char gLanReceiveBuf[100];
extern volatile unsigned int gReTransmit;
extern volatile unsigned int gTransmitCommand;



#define ACK_C   0x06     
#define EOT_C   0x04     
#define ENQ_C   0x05
#define NAK_C   0x15
	 
/***************************************
 * SOCKET NUMBER DEFINION for Examples *
 ***************************************/
#define SOCK_DHCP				0
#define MY_MAX_DHCP_RETRY	3
	 
extern void  wizchip_select(void);
extern void  wizchip_deselect(void);
extern void  wizchip_write(uint8_t wb);
extern uint8_t wizchip_read(void);							
extern void platform_init(void);
extern void network_init(void);
	 
extern volatile uint32_t msTicks; /* counts 1ms timeTicks */
extern uint32_t prevTick;
	 
int32_t loopback_tcps(uint8_t sn, uint8_t* buf, uint16_t port);

							

#ifdef __cplusplus
}
#endif

#endif
