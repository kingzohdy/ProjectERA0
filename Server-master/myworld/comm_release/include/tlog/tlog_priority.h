/*
**  @file $RCSfile: tlog_priority.h,v $
**  general description of this module
**  $Id: tlog_priority.h,v 1.3 2012/07/27 05:09:19 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:09:19 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOG_PRIORITY_H
#define TLOG_PRIORITY_H

#ifdef __cplusplus
extern "C"
{
#endif

const char* tlog_priority_to_string(int a_iPrio);
int tlog_priority_to_int(const char* a_pszName);

#ifdef __cplusplus
}
#endif

#endif /* TLOG_PRIORITY_H */


