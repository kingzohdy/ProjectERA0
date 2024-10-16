/*
**  @file $RCSfile: tlognet.h,v $
**  general description of this module
**  $Id: tlognet.h,v 1.3 2012/07/27 05:09:20 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2012/07/27 05:09:20 $
**  @version $Revision: 1.3 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOGNET_H
#define TLOGNET_H

#include "tlog/tlogio.h"
#include "tlog/tlognetdef.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct tagTLogDevNetInst
{
	int s;
	int iType; 
	int iCurRetry;
};

typedef struct tagTLogDevNetInst	TLOGDEVNETINST;
typedef struct tagTLogDevNetInst	*LPTLOGDEVNETINST;

struct tagTLogNet
{
	TLOGDEVNET* pstDev;
	TLOGDEVNETINST stInst;
};

typedef struct tagTLogNet		TLOGNET;
typedef struct tagTLogNet		*LPTLOGNET;

int tlognet_init(TLOGNET* a_pstLogNet, TLOGDEVNET* a_pstDev);
int tlognet_write(TLOGNET* a_pstLogNet, const char* a_pszBuff, int a_iBuff);
int tlognet_fini(TLOGNET* a_pstLogNet);

int tlognet_writev(TLOGNET* a_pstLogNet, const TLOGIOVEC* a_pstIOVec, int a_iCount);

#ifdef __cplusplus
}
#endif

#endif /* TLOGNET_H */


