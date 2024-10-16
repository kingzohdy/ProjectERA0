/*
**  @file $RCSfile: tfork.h,v $
**  general description of this module
**  $Id: tfork.h,v 1.2 2009/07/24 05:19:00 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009/07/24 05:19:00 $
**  @version $Revision: 1.2 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TFORK_H

#define TFORK_H

#include "pal/ttypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WIN32

pid_t fork(void);

#endif

#ifdef __cplusplus
}
#endif


#endif /* TFORK_H */
