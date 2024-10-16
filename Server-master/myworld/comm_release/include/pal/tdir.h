/*
**  @file $RCSfile: tdir.h,v $
**  general description of this module
**  $Id: tdir.h,v 1.3 2012/07/27 05:09:02 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:09:02 $
**  @version $Revision: 1.3 $
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
