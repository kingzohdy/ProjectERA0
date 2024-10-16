/*
**  @file $RCSfile: tlogfilter.h,v $
**  general description of this module
**  $Id: tlogfilter.h,v 1.1 2008/08/05 07:15:26 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2008/08/05 07:15:26 $
**  @version $Revision: 1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOGFILTER_H
#define TLOGFILTER_H

#include "tlog/tlogfilterdef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define TLOGFILTER_MATCH_INT(match, f, id)			\
{	\
	if( (f)->nMod )		\
	{	\
		if( (f)->nModCount>0 && ( ((id) % (f)->nMod)<(f)->nModStart || ((id) % (f)->nMod) - (f)->nModStart>=(f)->nModCount ) ) \
			match	=	0;	\
		else	\
			match	=	1;	\
	}	\
	else	\
		match	=	1;	\
	\
	if( match && (f)->iCount>0 && ( (id)<(f)->iStart || (id)-(f)->iStart>=(f)->iCount ) )	\
		match	=	0;	\
}

int tlogfilter_match(TLOGFILTER* pstFilter, int iID, int iCls);
int tlogfilter_match_vec(TLOGFILTERVEC* pstFilterVec, int iID, int iCls);

#ifdef __cplusplus
}
#endif

#endif /* TLOGFILTER_H */


