/*
**  @file $RCSfile: ttypes.h,v $
**  general description of this module
**  $Id: ttypes.h,v 1.1.1.1 2011/01/05 10:59:29 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:29 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TTYPES_H

#define TTYPES_H

#include <stddef.h>
#include <time.h>
#include <sys/types.h>

#ifdef WIN32
	#include <wchar.h>

	#include <limits.h>

	#if _MSC_VER >= 1400
		#include <crtdefs.h>
	#else
		typedef int intptr_t;
		typedef unsigned long ULONG_PTR;
		typedef ULONG_PTR *LPULONG_PTR;
		typedef ULONG_PTR *PULONG_PTR;
		typedef ULONG_PTR DWORD_PTR;
		typedef ULONG_PTR *LPDWORD_PTR;
		typedef ULONG_PTR *PDWORD_PTR;
	#endif

#else /* WIN32 */
#endif	/* WIN32 */

/* PS Method. */

#ifdef WIN32

	typedef __int64 	longlong;
	typedef unsigned __int64 ulonglong;

	typedef int socklen_t;
	typedef int ssize_t;
	typedef long suseconds_t;

	typedef __int64 off64_t;
	typedef long off_t;

	#ifndef __int8_t_defined
		# define __int8_t_defined
		typedef signed char		int8_t;
		typedef short int		int16_t;
		typedef int				int32_t;

		#if _MSC_VER >= 1300
			typedef long long 	int64_t;
		#else /* _MSC_VER */
			typedef __int64		int64_t;
		#endif /* _MSC_VER */
	#endif


	/* Unsigned.  */
	typedef unsigned char		uint8_t;
	typedef unsigned char		u_int8_t;

	typedef unsigned short int	uint16_t;
	typedef unsigned short int	u_int16_t;

	#ifndef __uint32_t_defined
		typedef unsigned int	uint32_t;
		typedef unsigned int	u_int32_t;
		# define __uint32_t_defined
	#endif


	#if _MSC_VER >= 1300
		typedef unsigned long long 	uint64_t;
		typedef unsigned long long 	u_int64_t;
	#else /* _MSC_VER */
		typedef unsigned __int64	uint64_t;
		typedef unsigned __int64	u_int64_t;
	#endif /* _MSC_VER */

	typedef int ptrdiff_t;

	typedef int mode_t;

	typedef int key_t;

	typedef int pid_t;
	typedef int uid_t;
	typedef int gid_t;

#else /* WIN32 */

	/* call invention. */
	#define CALLBACK	__stdcall
	#define WINAPI		__stdcall

	/* basic macros. */
	#define CONST 		const
	#define VOID		void

	/* basic data type definition. */
	typedef long long longlong;
	typedef unsigned long long ulonglong;

#ifndef PAL_NO_WIN_TYPES
	typedef CONST void	*LPCVOID;
	typedef CONST void	*PCVOID;

	typedef int HANDLE;
	typedef HANDLE *LPHANDLE;
	typedef HANDLE *PHANDLE;

	typedef HANDLE SOCKET;
	typedef SOCKET *LPSOCKET;
	typedef SOCKET *PSOCKET;

	typedef ulonglong ULONGLONG;
	typedef longlong LONGLONG;

	typedef ULONGLONG *LPULONGLONG;
	typedef ULONGLONG *PULONGLONG;

	typedef LONGLONG *LPLONGLONG;
	typedef LONGLONG *PLONGLONG;

	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned long DWORD;
	typedef unsigned int DWORD32;
	typedef ULONGLONG DWORD64;
	typedef ULONGLONG DWORDLONG;
	typedef int BOOL;
	typedef BYTE BOOLEAN;

	typedef BYTE *LPBYTE;
	typedef BYTE *PBYTE;
	typedef WORD *LPWORD;
	typedef WORD *PWORD;
	typedef DWORD *LPDWORD;
	typedef DWORD *PDWORD;
	typedef DWORD32 *LPDWORD32;
	typedef DWORD32 *PDWORD32;
	typedef DWORD64 *LPDWORD64;
	typedef DWORD64 *PDWORD64;
	typedef DWORDLONG *LPDWORDLONG;
	typedef DWORDLONG *PDWORDLONG;
	typedef BOOL *LPBOOL;
	typedef BOOL *PBOOL;
	typedef BOOLEAN *LPBOOLEAN;
	typedef BOOLEAN *PBOOLEAN;

	/* char/string definition. */
	typedef char CHAR;
	typedef unsigned char UCHAR;
	typedef wchar_t	WCHAR;

	typedef CHAR* LPCHAR;
	typedef CHAR* PCHAR;

	typedef UCHAR* LPUCHAR;
	typedef UCHAR* PUCHAR;

	typedef WCHAR* LPWCHAR;
	typedef WCHAR* PWCHAR;

	#ifdef UNICODE
		typedef WCHAR TCHAR;
	#else
		typedef char TCHAR;
	#endif

	typedef TCHAR TBYTE;

	typedef TCHAR *LPTCHAR;
	typedef TCHAR *PTCHAR;

	typedef TBYTE *LPTBYTE;
	typedef TBYTE *PTBYTE;

	typedef CONST CHAR *LPCSTR;
	typedef CONST WCHAR *LPCWSTR;
		  
	typedef CHAR *LPSTR;
	typedef WCHAR *LPWSTR;

	#ifdef UNICODE
		typedef LPCWSTR LPCTSTR; 
	#else
		typedef LPCSTR LPCTSTR;
	#endif

	#ifdef UNICODE
		typedef LPWSTR LPTSTR;
	#else
		typedef LPSTR LPTSTR;
	#endif

	typedef CONST CHAR	*PCSTR;
	typedef CONST WCHAR *PCWSTR;
		  
	typedef CHAR *PSTR;
	typedef WCHAR *PWSTR;

	#ifdef UNICODE
		typedef PCWSTR PCTSTR; 
	#else
		typedef PCSTR PCTSTR;
	#endif

	#ifdef UNICODE
		typedef PWSTR PTSTR;
	#else
		typedef PSTR PTSTR;
	#endif

	typedef DWORD COLORREF;
	typedef COLORREF *LPCOLORREF;
	typedef COLORREF *PCOLORREF;

	typedef WORD ATOM;

	typedef short SHORT;
	typedef unsigned short USHORT;

	typedef SHORT *LPSHORT;
	typedef SHORT *PSHORT;
	typedef USHORT *LPUSHORT;
	typedef USHORT *PUSHORT;


	/* int/int ptr */
	typedef int INT;
	typedef signed int INT32;
	typedef LONGLONG INT64;

	typedef INT *LPINT;
	typedef INT *PINT;
	typedef INT32 *LPINT32;
	typedef INT *PINT32;
	typedef INT64 *LPINT64;
	typedef INT *PINT64;

	typedef unsigned int UINT;
	typedef unsigned int UINT32;
	typedef ULONGLONG UINT64;

	typedef UINT *LPUINT;
	typedef UINT *PUINT;
	typedef UINT32 *LPUINT32;
	typedef UINT32 *PUINT32;
	typedef UINT64 *LPUINT64;
	typedef UINT64 *PUINT64;

	typedef long LONG;
	typedef int LONG32;
	typedef LONGLONG LONG64;

	typedef LONG *LPLONG;
	typedef LONG *PLONG;
	typedef LONG32 *LPLONG32;
	typedef LONG32 *PLONG32;
	typedef LONG64 *LPLONG64;
	typedef LONG64 *PLONG64;

	typedef unsigned long ULONG;
	typedef unsigned int ULONG32;
	typedef unsigned long long ULONG64;

	typedef ULONG *LPULONG;
	typedef ULONG *PULONG;
	typedef ULONG32 *LPULONG32;
	typedef ULONG32 *PULONG32;
	typedef ULONG64 *LPULONG64;
	typedef ULONG64 *PULONG64;


	#define POINTER_32

	#define POINTER_64

	typedef float		FLOAT;

	typedef WORD		LANGID;
	typedef DWORD		LCID;
	typedef DWORD		LCTYPE;
	typedef DWORD		LGRPID;
 
	typedef LONGLONG	USN;

	#ifdef _WIN64
		typedef int HALF_PTR;
	#else
		typedef short HALF_PTR;
	#endif

	#ifdef _WIN64
		typedef unsigned int UHALF_PTR;
	#else
		typedef unsigned short UHALF_PTR;
	#endif

	typedef HALF_PTR *LPHALF_PTR;
	typedef HALF_PTR *PHALF_PTR;
	typedef UHALF_PTR *LPUHALF_PTR;
	typedef UHALF_PTR *PUHALF_PTR;

	#if defined(_WIN64) 
		typedef long long INT_PTR; 
	#else 
		typedef int INT_PTR;
	#endif

	typedef INT_PTR *LPINT_PTR;
	typedef INT_PTR *PINT_PTR;

	#if defined(_WIN64) 
		typedef unsigned long long UINT_PTR; 
	#else 
		typedef unsigned int UINT_PTR;
	#endif

	typedef UINT_PTR *LPUINT_PTR;
	typedef UINT_PTR *PUINT_PTR;

	#if defined(_WIN64)
		typedef long long LONG_PTR; 
	#else
		typedef long LONG_PTR;
	#endif

	typedef LONG_PTR *LPLONG_PTR;
	typedef LONG_PTR *PLONG_PTR;

	#if defined(_WIN64)
		typedef unsigned long long ULONG_PTR;
	#else
		typedef unsigned long ULONG_PTR;
	#endif

	typedef ULONG_PTR *LPULONG_PTR;
	typedef ULONG_PTR *PULONG_PTR;

	typedef ULONG_PTR DWORD_PTR;
	typedef ULONG_PTR *LPDWORD_PTR;
	typedef ULONG_PTR *PDWORD_PTR;

	typedef ULONG_PTR SIZE_T;
	typedef LONG_PTR SSIZE_T;

	typedef LONG_PTR	LPARAM;
	typedef UINT_PTR	WPARAM;
#endif /* PAL_NO_WIN_TYPES */

#endif /* WIN32 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TTYPES_H */


