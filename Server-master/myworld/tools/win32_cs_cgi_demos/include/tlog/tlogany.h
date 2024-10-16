/*
**  @file $RCSfile: tlogany.h,v $
**  general description of this module
**  $Id: tlogany.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOGANY_H
#define TLOGANY_H

#include "tlog/tlogio.h"
#include "tlog/tlogfiledef.h"
#include "tlog/tlognetdef.h"
#include "tlog/tlogvecdef.h"
#include "tlog/tloganydef.h"

#ifdef __cplusplus
extern "C"
{
#endif


union tagTLogDevSelectorInst
{
	TLOGFILE stFile;
	TLOGNET stNet;
	TLOGVEC stVec;
};

typedef union tagTLogDevSelectorInst 	TLOGDEVSELECTORINST;
typedef union tagTLogDevSelectorInst 	*LPTLOGDEVSELECTORINST;

struct tagTLogDevAnyInst
{
	int iType;
	TLOGDEVSELECTORINST stAny;
};

typedef struct tagTLogDevAnyInst	TLOGDEVANYINST;
typedef struct tagTLogDevAnyInst	*LPTLOGDEVANYINST;

struct tagTLogAny
{
	TLOGDEVANY* pstDev;
	TLOGDEVANYINST stInst;
};

typedef struct tagTLogAny		TLOGANY;
typedef struct tagTLogAny		*LPTLOGANY;

int tlogany_init(TLOGANY* a_pstLogAny, TLOGDEVANY* a_pstDev);
int tlogany_fini(TLOGANY* a_pstLogAny);

int tlogany_write(TLOGANY* a_pstLogAny, int a_iID, int a_iCls, const char* a_pszBuff, int a_iBuff);
int tlogany_writev(TLOGANY* a_pstLogAny, int a_iID, int a_iCls, const TLOGIOVEC* a_pstIOVec, int a_iCount);

#ifdef __cplusplus
}
#endif

#endif /* TLOGANY_H */


