/*
**  @file $RCSfile: shtable_shm.h,v $
**  general description of this module
**  $Id: shtable_shm.h,v 1.1.1.1 2011/01/05 10:59:29 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:29 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef SHTABLE_SHM_H

#define SHTABLE_SHM_H

#include "comm/shtable.h"

LPSHTABLE sht_create_shm(int iBucket, int iMax, int iUnit, unsigned int uiKey);

#endif /* SHTABLE_SHM_H */
