/*
**  @file $RCSfile: tlog_priority.h,v $
**  general description of this module
**  $Id: tlog_priority.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
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


