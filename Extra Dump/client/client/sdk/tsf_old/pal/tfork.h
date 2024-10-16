/*
**  @file $RCSfile: tfork.h,v $
**  general description of this module
**  $Id: tfork.h,v 1.2 2007/05/29 08:29:30 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2007/05/29 08:29:30 $
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
