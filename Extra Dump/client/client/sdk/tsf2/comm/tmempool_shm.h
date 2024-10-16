/*
**  @file $RCSfile: tmempool_shm.h,v $
**  general description of this module
**  $Id: tmempool_shm.h,v 1.1.1.1 2008/05/28 07:34:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008/05/28 07:34:59 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TMEMPOOL_SHM_H
#define TMEMPOOL_SHM_H

#include "comm/tmempool.h"

#ifdef __cplusplus
extern "C"
{
#endif

int tmempool_new_shm(TMEMPOOL** ppstPool, unsigned int uiKey, int iMax, int iUnit);

#ifdef __cplusplus
}
#endif

#endif  /* TMEMPOOL_SHM_H */

