/******************************************************************************
*    	File name	: BootLoader.h 
*		Description : Hardware Configuration File
*		Company		: idnics co.,
*		Version		: 1.00
*		Data		: 2012 / 06 / 19
*		Author		: BJ Song
*		Contact		: Naver TEECO Cafe
******************************************************************************/
#ifndef TEECO_BOOT_PROFILE_H
    #define TEECO_BOOT_PROFILE_H

#ifdef __cplusplus
 extern "C" {
#endif
extern char	current_dir[64];

int 	StrToHex(char *data);
int 	StringGetLine(char *src_buf, char *getbuf,  int getbuf_size, int startpos, int *read_cnt);
int 	IntToStringWon(int value, char *str, int array_size);
int 	StringReverse(char *str);
int 	StringSlashParsing(char *buf);
void 	StringTrim(char *str);	 
int 	Words_Separator(const char *src, char *firstname, char *secondname, char sep);
int 	StringSearch(const char *src, char *compare);
int 	StringSeparator(const char *src, char *buf, char start, char end);
int 	inet_pton4(char *ip, uint8_t *sbuf);
int 	inet_ntop4(char *ip, uint8_t *sbuf);
#ifdef __cplusplus
}
#endif

#endif // TEECO_BOOT_PROFILE_H

