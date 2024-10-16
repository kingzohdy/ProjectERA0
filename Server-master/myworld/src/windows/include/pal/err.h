/*
**  @file $RCSfile: err.h,v $
**  general description of this module
**  $Id: err.h,v 1.2 2009/07/24 05:19:00 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009/07/24 05:19:00 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef ERR_H
#define ERR_H

#include <sys/cdefs.h>
#include <stdarg.h>

#ifdef __cpluscplus
extern "C"
{
#endif

void warn (const char *fmt, ...);
void warnx (const char *fmt, ...);

void errx (int eval, const char *fmt, ...);

void vwarn (const char *fmt, va_list ap);
void vwarnx (const char *fmt, va_list ap);

void verr (int eval, const char *fmt, va_list ap);
void verrx (int eval, const char *fmt, va_list ap);

#ifdef __cplusplus
}
#endif

#endif /* ERR_H */
