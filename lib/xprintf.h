/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2012     */
/*------------------------------------------------------------------------*/

#ifndef _STRFUNC
#define _STRFUNC

#ifdef __cplusplus
extern "C"
{
#endif

#if !defined(XPRINTF_USE_FLOAT)
    #define XPRINTF_USE_FLOAT      1   /* 1: Use floating point */
#endif
#if !defined(XPRINTF_USE_XFUNC_OUT)
    #define XPRINTF_USE_XFUNC_OUT	1	/* 1: Use output functions */
#endif
#define	_CR_CRLF		1	    /* 1: Convert \n ==> \r\n in the output char */
#define	_USE_LONGLONG	1	    /* 1: Enable long long integer in type "ll". */
#define	_LONGLONG_t		long long	/* Platform dependent long long integer type */

#if !defined(XPRINTF_USE_XFUNC_IN)
    #define XPRINTF_USE_XFUNC_IN	1	/* 1: Use input function */
#endif
#define	_LINE_ECHO		1	    /* 1: Echo back input chars in xgets function */


#if XPRINTF_USE_XFUNC_OUT
#define xdev_out(func) xfunc_out = (void(*)(unsigned char))(func)
extern void (*xfunc_out)(unsigned char);
void xputc (char c);
void xfputc (void (*func)(unsigned char), char c);
void xputs (const char* str);
void xfputs (void (*func)(unsigned char), const char* str);
void xprintf (const char* fmt, ...);
void xsprintf (char* buff, const char* fmt, ...);
void xfprintf (void (*func)(unsigned char), const char*	fmt, ...);
void put_dump (const void* buff, unsigned long addr, int len, int width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)
#endif

#if XPRINTF_USE_XFUNC_IN
#define xdev_in(func) xfunc_in = (unsigned char(*)(void))(func)
extern unsigned char (*xfunc_in)(void);
int xgets (char* buff, int len);
int xfgets (unsigned char (*func)(void), char* buff, int len);
int xatoi (char** str, long* res);
#endif

#ifdef __cplusplus
}
#endif

#endif
