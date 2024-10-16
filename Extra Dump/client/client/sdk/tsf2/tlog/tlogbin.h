/*
**  @file $RCSfile: tlogbin.h,v $
**  general description of this module
**  $Id: tlogbin.h,v 1.1 2008/08/05 07:15:26 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2008/08/05 07:15:26 $
**  @version $Revision: 1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOGBIN_H
#define TLOGBIN_H

#include "tlog/tlogbindef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TLOGBIN_INIT_HEAD(pstHead, seq, id, cls, type, version, bodylen, pstTm)	\
	do																				\
	{																				\
		(pstHead)->chVer		=	TLOGBIN_VER;									\
		(pstHead)->chMagic		=	TLOGBIN_MAGIC;									\
		(pstHead)->chHeadLen	=	sizeof(TLOGBINHEAD);							\
		(pstHead)->chCmd		=	0;												\
		(pstHead)->iType		=	type;											\
		(pstHead)->iSeq			=	seq;											\
		(pstHead)->iID			=	id;											\
		(pstHead)->iCls			=	cls;											\
		(pstHead)->iBodyVer		=	version;										\
		(pstHead)->iBodyLen		=	bodylen;										\
		if( pstTm )																	\
		{																			\
			(pstHead)->stTime.iSec	=	(pstTm)->tv_sec;							\
			(pstHead)->stTime.iUsec	=	(pstTm)->tv_usec;							\
		}																			\
		else																		\
		{																			\
			gettimeofday((struct timeval*)&(pstHead)->stTime, NULL);				\
		}																			\
		(pstHead)->iCheckSum	=	0;												\
	}																				\
	while(0)


int tlogbin_hton_head(char* a_pszBuff, int a_iBuff, const TLOGBINHEAD* a_pstHead);
int tlogbin_ntoh_head(TLOGBINHEAD* a_pstHead, const char* a_pszBuff, int a_iBuff);

int tlogbin_get_pkgsize(const char* a_pszBuff, int a_iBuff, int* piHead);

int tlogbin_make_pkg(char* a_pszPkg, int a_iPkg, TLOGBINHEAD* a_pstHead, const char* a_pszBody, int a_iBody);
int tlogbin_get_head(TLOGBINHEAD* a_pstHead, const char* a_pszBuff, int a_iBuff, int* piPkg);


int tlogbin_filter(TLOGBINHEAD* a_pstHead, TLOGBINFILTER* a_pstFilter, const char* a_pszExt);

#ifdef __cplusplus
}
#endif

#endif /* TLOGBIN_H */
