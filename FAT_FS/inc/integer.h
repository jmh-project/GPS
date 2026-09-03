/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/

#ifndef _FF_INTEGER
#define _FF_INTEGER

#ifdef _WIN32	/* FatFs development platform */

#include <windows.h>
#include <tchar.h>
typedef unsigned __int64 QWORD;


#else			/* Embedded platform */

/* These types MUST be 16-bit or 32-bit */
typedef int				INT;


/* These types MUST be 16-bit */
typedef short			SHORT;
typedef unsigned short	WCHAR;

/* These types MUST be 32-bit */

#ifndef		LONG
#define		LONG			long
#endif

typedef unsigned long	ULONG;

#ifndef		UINT
#define		UINT			unsigned int
#endif

#ifndef 	BYTE
#define		BYTE	unsigned char
#endif
#ifndef 	WORD
#define		WORD	unsigned short int
#endif

#ifndef		DWORD
#define		DWORD	unsigned long int
#endif


/* This type MUST be 64-bit (Remove this for ANSI C (C89) compatibility) */
typedef unsigned long long QWORD;

#endif

#endif
