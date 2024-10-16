/*
**  @file $RCSfile: tlog_layout.h,v $
**  general description of this module
**  $Id: tlog_layout.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef TLOG_LAYOUT_H
#define TLOG_LAYOUT_H

#include "tlog/tlog_event.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define TLOG_LAYOUT_DEF_FORMAT  "[%d] %m"

int tlog_layout_format(tlog_event_t* a_pstEvt, const char* a_pszFmt);

#ifdef __cplusplus
}
#endif

#endif /* TLOG_LAYOUT_H */


