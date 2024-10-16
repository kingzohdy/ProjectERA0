/*
**  @file $RCSfile: tlogfile.h,v $
**  general description of this module
**  $Id: tlogfile.h,v 1.1.1.1 2011/01/04 11:02:51 niko Exp $
**  @author $Author: niko $
**  @date $Date: 2011/01/04 11:02:51 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOGFILE_H

#define TLOGFILE_H

#include <stdlib.h>
#include <stdio.h>
#include "tlog/tlogio.h"
#include "tlog/tlogfiledef.h"

#define TLOGFILE_PATH_LEN	(TLOGFILE_PATTERN_LEN + 64)

#ifdef __cplusplus
extern "C"
{
#endif


struct tagTLogDevFileInst
{
	FILE* fp;
	char* pszBuff;		/* if filebuff is set, here is file buffer pointer */
	time_t tLastMsg;
	int fDirty;
	time_t tLastSync;
	int iCurRotate;
	time_t tCreate;	/* the time for create file. */
	int iTail;
	int iRes;
	char szPath[TLOGFILE_PATH_LEN];
};

typedef struct tagTLogDevFileInst	TLOGDEVFILEINST;
typedef struct tagTLogDevFileInst	*LPTLOGDEVFILEINST;

struct tagTLogFile
{
	TLOGDEVFILE* pstDev;
	TLOGDEVFILEINST stInst;
};

typedef struct tagTLogFile			TLOGFILE;
typedef struct tagTLogFile			*LPTLOGFILE;


int tlogfile_init(TLOGFILE* a_pstLogFile, TLOGDEVFILE* a_pstDev);
int tlogfile_write(TLOGFILE* a_pstLogFile, const char* a_pszBuff, int a_iBuff);
int tlogfile_fini(TLOGFILE* a_pstLogFile);

int tlogfile_sync(TLOGFILE* a_pstLogFile);
int tlogfile_writev(TLOGFILE* a_pstLogFile, const TLOGIOVEC* a_pstIOVec, int a_iCount);

#ifdef __cplusplus
}
#endif

#endif /* TLOGFILE_H */
