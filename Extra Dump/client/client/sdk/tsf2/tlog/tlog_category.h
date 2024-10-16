/*
**  @file $RCSfile: tlog_category.h,v $
**  general description of this module
**  $Id: tlog_category.h,v 1.4 2008/11/12 10:09:28 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2008/11/12 10:09:28 $
**  @version $Revision: 1.4 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOG_CATEGORY_H
#define TLOG_CATEGORY_H

#include <stdarg.h>
#include <tlog/tlog_event.h>
#include <tlog/tlogfiledef.h>
#include <tlog/tlognetdef.h>
#include <tlog/tlogvecdef.h>
#include <tlog/tloganydef.h>
#include <tlog/tlogfilterdef.h>
#include <tlog/tlog_priority_def.h>
#include <tlog/tlog_category_def.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct tagTLogCategoryInst
{
	char szName[TLOG_NAME_LEN];
	struct tagTLogCategoryInst* pstParent;
	struct tagTLogCategoryInst* pstForward;
	TLOGCATEGORY* pstCat;
	int iInited;
	int iSeq;
	int* piPriorityHigh;
	int* piPriorityLow;
	TLOGANY stLogAny;
};

typedef struct tagTLogCategoryInst	TLOGCATEGORYINST;
typedef struct tagTLogCategoryInst	*LPTLOGCATEGORYINST;

int tlog_category_append(TLOGCATEGORYINST *a_pstCatInst, TLOGEVENT* a_pstEvt);

int tlog_category_log(TLOGCATEGORYINST *a_pstCatInst, TLOGEVENT* a_pstEvt);

int tlog_category_logv(TLOGCATEGORYINST *a_pstCatInst, TLOGEVENT* a_pstEvt, const char* a_pszFmt, ...);



#ifdef __cplusplus
}
#endif

#endif /* TLOG_CATEGORY_H */


