/*
**  @file $RCSfile: comm_misc.c,v $
**  general description of this module
**  $Id: comm_misc.c,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:28 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "comm_misc.h"
#include "pal/tresource.h"

int set_rlimit(unsigned int uiMaxConn)
{
	struct rlimit rlim;

	rlim.rlim_cur = rlim.rlim_max = uiMaxConn + 100; 
	if (setrlimit(RLIMIT_NOFILE, &rlim) < 0) {
		perror("setlimit");
		return -1;
	}
	getrlimit(RLIMIT_NOFILE, &rlim);
	printf("Max open file: %ld\n", rlim.rlim_cur);
	return 0;
}

int ss_han_create(unsigned char *pszLib, LPSSHANDLE *ppSSHandle)
{
	int iRet = 0;
	SSHANDLE *pstSSHandle = NULL;

	pstSSHandle = (SSHANDLE *)malloc(sizeof(SSHANDLE));
	if (!pstSSHandle)
	{
		return -1;
	}

	pstSSHandle->pstSSProtoLib = (LPTDRMETALIB)pszLib;
	
	if (0 == iRet)
	{
		pstSSHandle->pstSSProtoPkgMeta = tdr_get_meta_by_name(pstSSHandle->pstSSProtoLib, "SSPkg");
		if (!pstSSHandle->pstSSProtoPkgMeta)
		{
			iRet = -1;
		}
	}

	if (0 > iRet)
	{
		free(pstSSHandle);
	}
	else
	{
		*ppSSHandle = pstSSHandle;
	}
	
	return iRet;
}

int ss_han_destroy(LPSSHANDLE *ppSSHandle)
{
	free(*ppSSHandle);
	*ppSSHandle = NULL;
	
	return 0;
}

int ss_unpack(SSHANDLE  *pstSSHandler, char *pszBuf, int iBuf, SSPKG *pstPkg, int iVersion)
{
	TDRDATA stHost;
	TDRDATA stNet;
	
		
	stNet.pszBuff = pszBuf;
	stNet.iBuff = iBuf;
	stHost.pszBuff = (char *)pstPkg;
	stHost.iBuff = sizeof(SSPKG);

	return tdr_ntoh(pstSSHandler->pstSSProtoPkgMeta, &stHost, &stNet, iVersion);
}

int ss_pack(SSHANDLE  *pstSSHandler, TDRDATA *pstNet, SSPKG *pstPkg, int iVersion)
{
	TDRDATA stHost;

	stHost.iBuff = sizeof(SSPKG);
	stHost.pszBuff = (char *)pstPkg;

	pstPkg->Head.Ver = TDR_METALIB_PROTO_SS_VERSION;

	return tdr_hton(pstSSHandler->pstSSProtoPkgMeta,  pstNet, &stHost, iVersion);
}


