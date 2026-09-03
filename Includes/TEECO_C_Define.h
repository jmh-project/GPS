//#ifdef TEECO_CTYPE
//#define TEECO_CTYPE

//#ifdef __cplusplus
// extern "C" {
//#endif

//------ TEECO 의 C의 자료형 -------
#ifndef 	U32
#define 	U32 	unsigned long int
#endif

#ifndef 	u32
#define 	u32 	unsigned long int
#endif

#define 	U16 	unsigned short

//#define 	S32 	int

#define 	S16 	short int
#define 	U8  	unsigned char
#define		S8  	char

#ifndef 	BYTE
#define		BYTE	unsigned char
#endif

#ifndef 	WORD
#define		WORD	unsigned short int
#endif

#ifndef		DWORD
#define		DWORD	unsigned long int
#endif

#ifndef		LONG
#define		LONG			unsigned long int
#endif

#ifndef		UINT
#define		UINT			unsigned int
#endif

#define		HANDLE			int
#define		COLOR			unsigned short int //16Bit Color
#define		BOOL			unsigned long int
#define		WCOMMAND		unsigned long int
#define		PARAM			unsigned long int
#define		WPARAM			unsigned long int
#define		LPARAM			unsigned long int
#define		ADDRESS			unsigned long int
/*
#ifndef 	size_t
#define		size_t			unsigned long int
#endif
*/


//#ifdef __cplusplus
//}
//#endif

//#endif
