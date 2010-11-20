/*-------------------------------------------*/
/* Integer type definitions for FatFs module */
/*-------------------------------------------*/
#ifndef _INTEGER

#if 0
#include <windows.h>
#else

/* These types must be 16-bit, 32-bit or larger integer */
#define INT 	int
#define UINT	unsigned int

/* These types must be 8-bit integer */
#define CHAR	signed char
#define UCHAR	unsigned char
#define BYTE	unsigned char

/* These types must be 16-bit integer */
#define SHORT	short
#define USHORT	unsigned short
#define WORD	unsigned short
#define WCHAR	unsigned short

/* These types must be 32-bit integer */
#define LONG	typedef long
#define ULONG	unsigned long
#define DWORD	unsigned long

/* Boolean type */
typedef enum { FALSE = 0, TRUE } BOOL;

#endif

#define _INTEGER
#endif

