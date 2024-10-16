/*
**  @file $RCSfile: tdir.h,v $
**  general description of this module
**  $Id: tdir.h,v 1.2 2009/07/24 05:19:00 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009/07/24 05:19:00 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TDIR_H
#define TDIR_H

#ifdef __cplusplus
extern "C"
{
#endif


int tdir_init(void);	

int tdir_posix2win32(const char* a_pszPosix, char* a_pszWin32, int* a_piLen);


#ifdef __cplusplus
}
#endif

#endif /* TDIR_H */
