/*
**  @file $RCSfile: tstdio.h,v $
**  general description of this module
**  $Id: tstdio.h,v 1.3 2012/07/27 05:09:06 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:09:06 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef TSTDIO_H
#define TSTDIO_H

#include <fcntl.h>
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>


#ifdef __cplusplus
extern "C"
{
#endif	/* __cplusplus */


/* PS Method Start */

#ifdef WIN32

	#define	L_ctermid				9
	#define L_cuserid				9

#ifndef TMP_MAX
	#define TMP_MAX					238328
#endif

	/* vc can process below func
	#define fileno(fp)				_fileno(fp)
	#define fdopen(fd, mode)		_fdopen(fd, mode)
	#define getw(fp)				_getw(fp)
	#define mktemp(sz)				_mktemp(sz)
	#define putw(w, fp)				_putw(w, fp)
	#define tempnam(dir, prefix)	_tempnam(dir, prefix)
	*/

	#define snprintf				_snprintf
	#define vsnprintf				_vsnprintf
	#define snwprintf				_snwprintf
	#define vsnwprintf				_vsnwprintf

	//#define fpurge(fp)				fflush(fp)
	int fpurge(FILE *stream);

	//#define __fpurge(fp)			fflush(fp)
	void  __fpurge(FILE *stream);

	#define sys_errlist				_sys_errlist
	#define sys_nerr				_sys_nerr

	void setbuffer(FILE *a_fpStream, char *a_pszBuf, size_t a_iSize);
    void setlinebuf(FILE *a_fpStream);
	
#else	/* WIN32 */
#endif	/* WIN32 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif	/* TSTDIO_H */
