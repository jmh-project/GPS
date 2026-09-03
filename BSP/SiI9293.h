#define 		SET_BITS    	0xFF
#define 		CLEAR_BITS  	0x00

int StrToHex(char *data);

uint8_t SIL9293Read( uint8_t dev_addr, uint8_t reg_addr );
void SIL9293Write(  uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
void SII9293WriteBlock(  uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, int size);
void SII9293RegBitsSet ( uint8_t dev_addr, uint8_t reg_addr, uint8_t bitMask, int setBits );
void SII9293RegWriteWord(uint8_t dev_addr, uint8_t reg_addr, uint16_t value);
void SII9293RegModify ( uint8_t dev_addr, uint8_t reg_addr,  uint8_t mask, uint8_t value);

void SiI9293Init(void);
void SII9293_I2C_Init(void);
uint8_t SII9293Read( uint8_t dev_addr, uint8_t reg_addr );
void SII9293Write(  uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
void SII9134_I2C_Init(void);
uint8_t SII9134Read( uint8_t dev_addr, uint8_t reg_addr );
void SII9134Write(  uint8_t dev_addr, uint8_t reg_addr, uint8_t data);
void RXID_Read(void);

void init_rx(void);
void HDMI_Chip_Reset(void);
void Delay(__IO uint32_t nTime);
void SiI9134Init(void);
void init_tx(void);
void SiI91343Init(void);
void TXID_Read(void);
void SII9134_I2C_PortInit(void);


uint8_t SII9134Read( uint8_t dev_addr, uint8_t reg_addr );
void SII9134Write(  uint8_t dev_addr, uint8_t reg_addr, uint8_t data);

void SII9134RegModify ( uint8_t dev_addr, uint8_t reg_addr,  uint8_t mask, uint8_t value);
void SII9134RegBitsSet ( uint8_t dev_addr, uint8_t reg_addr, uint8_t bitMask, int setBits );
void SII9134WriteBlock(  uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, int size);
void SII9134RegWriteWord(uint8_t dev_addr, uint8_t reg_addr, uint16_t value);

void TXSystemStatus(void);
void TXSyncRead(void);

uint8_t SiI_TMDS_setup(uint8_t bVMode);
void TXMode_Setting(char *str);

void HS_Setting(char *data);
void VS_Setting(char *data);
void CLK_Setting(char *data);

void BrightSend(char *str);
void RedGammaSend(char *str, int display);
void GreenGammaSend(char *str, int display);
void BlueGammaSend(char *str, int display);
void AllSend(char *br, char *red, char *green, char *blue, int display);

void RXSystemStatus(void);
void RXHotPlug(char *data);

