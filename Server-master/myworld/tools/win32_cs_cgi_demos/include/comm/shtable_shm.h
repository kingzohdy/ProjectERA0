/*
**  @file $RCSfile: shtable_shm.h,v $
**  general description of this module
**  $Id: shtable_shm.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef SHTABLE_SHM_H

#define SHTABLE_SHM_H

#include "comm/shtable.h"

LPSHTABLE sht_create_shm(int iBucket, int iMax, int iUnit, unsigned int uiKey);

#endif /* SHTABLE_SHM_H */
