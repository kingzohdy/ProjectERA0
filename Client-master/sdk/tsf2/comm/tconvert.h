/*
**  @file $RCSfile: tconvert.h,v $
**  general description of this module
**  $Id: tconvert.h,v 1.1 2008/07/29 10:40:42 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2008/07/29 10:40:42 $
**  @version $Revision: 1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TCONVERT_H

#define TCONVERT_H

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

long tconvert_get_bytesl(const char *a_szStr, char** a_ppszEnd, int a_iRadix);
long tconvert_get_daysl(const char *a_szStr, char** a_ppszEnd, int a_iRadix);

#ifdef __cplusplus
#endif /* __cplusplus */

#endif /* TCONVERT_H */
