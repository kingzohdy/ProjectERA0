/*
**  @file $RCSfile: tdir.h,v $
**  general description of this module
**  $Id: tdir.h,v 1.1 2007/04/24 02:02:17 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2007/04/24 02:02:17 $
**  @version $Revision: 1.1 $
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
