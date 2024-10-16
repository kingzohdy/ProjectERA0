/*
**  @file $RCSfile: tlog_layout.h,v $
**  general description of this module
**  $Id: tlog_layout.h,v 1.3 2008/08/06 02:30:58 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2008/08/06 02:30:58 $
**  @version $Revision: 1.3 $
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


