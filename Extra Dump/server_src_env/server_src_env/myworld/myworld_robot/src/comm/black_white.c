/*
**  @file $RCSfile: black_white.c,v $
**  general description of this module
**  $Id: black_white.c,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:28 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "black_white.h"
#include "comm/tini.h"

TINISECTIONDESC astBlackAddrSectionDesc[] =
{
	{"BlackEntry" , TINI_SECTION_ARRAY , offsetof(ADDRLIST, astAddr) ,
		{
			{"IP" , TINI_IP , offsetof(AddrEntry , iAddr) , "0", 0, 0 },
			{"Mask" , TINI_IP , offsetof(AddrEntry , iAddrMask), "0", 0, 0 } ,

		} , offsetof(ADDRLIST , iAddrNum) , sizeof(AddrEntry) ,MAX_ADDR_NUM

	} ,

} ;

TINISECTIONDESC astWhitekAddrSectionDesc[] =
{
	{"WhiteEntry" , TINI_SECTION_ARRAY , offsetof(ADDRLIST, astAddr) ,
		{
			{"IP" , TINI_IP , offsetof(AddrEntry , iAddr) , "0", 0, 0 },
			{"Mask" , TINI_IP , offsetof(AddrEntry , iAddrMask), "0", 0, 0 } ,

		}, offsetof(ADDRLIST , iAddrNum) , sizeof(AddrEntry) ,MAX_ADDR_NUM

	} ,

} ;

int black_white_init(char *pszCfgPath, BlackWhiteHandle **pstHandle)
{

	BlackWhiteHandle *pstTmp;

	pstTmp = (BlackWhiteHandle *)malloc(sizeof(BlackWhiteHandle));
	if (NULL == pstTmp)
	{
		return -1;
	}

	if (0 > tini_get_ini(pszCfgPath , (void *)&pstTmp->stBlack , astBlackAddrSectionDesc))
	{
		free(pstTmp);
		return -1;
	}

	if (0 > tini_get_ini(pszCfgPath , (void *)&pstTmp->stWhite, astWhitekAddrSectionDesc))
	{
		free(pstTmp);
		return -1;
	}

	*pstHandle = pstTmp;
	
	return 0;
}

int black_white_check(BlackWhiteHandle *pstHandle, int iIP)
{
	int i;
	int iTmpAddr;

	if (NULL == pstHandle)
	{
		return 0;
	}

	// check white address list ,first
	for( i = 0  ; i < pstHandle->stWhite.iAddrNum; i++ )
	{
		iTmpAddr = iIP & pstHandle->stWhite.astAddr[i].iAddrMask;
		if( iTmpAddr == pstHandle->stWhite.astAddr[i].iAddr)
		{
			return 0 ;
		}
	}

	// then , check black address list
	for( i = 0  ; i < pstHandle->stBlack.iAddrNum ; i++ )
	{
		/* if IP & Net Mask  = 0.0.0.0 , then block all */
		if( 0 == pstHandle->stBlack.astAddr[i].iAddrMask && 0 == pstHandle->stBlack.astAddr[i].iAddr )
		{
			return -1;
		}
		iTmpAddr = iIP & pstHandle->stBlack.astAddr[i].iAddrMask ;
		if( iTmpAddr == pstHandle->stBlack.astAddr[i].iAddr)
		{
			return -1 ;
		}
	}

	return 0 ;
}

int black_white_fini(BlackWhiteHandle **ppstHandle)
{
	free(*ppstHandle);
	*ppstHandle = NULL;
	
	return 0;
}


