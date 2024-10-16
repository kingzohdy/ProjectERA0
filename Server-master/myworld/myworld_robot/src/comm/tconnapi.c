/*
**  @file $RCSfile: tconnapi.c,v $
**  general description of this module
**  $Id: tconnapi.c,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:28 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "pal/pal.h"
#include "tdr/tdr.h"
#include "tbus/tbus.h"
#include "tconnapi.h"


struct tagTConnapiHandle
{
	TFRAMEHEAD	stSend;
	TFRAMEHEAD	stRecv;
	int iBus;
	int iProc;
};

typedef struct tagTConnapiHandle	TCONNAPIHANDLE;
typedef struct tagTConnapiHandle	*LPTCONNAPIHANDLE;

extern unsigned char g_szMetalib_proto_ss[];

static LPTDRMETALIB gs_pstLib= NULL;
LPTDRMETA g_pstConnapiFrameHead = NULL;




int tconnapi_init(int iKey)
{
	char szKey[32] = {0};

	if(0!=iKey)
	{
             snprintf(szKey, sizeof(szKey), "%d", iKey);     
	}

	 return tconnapi_initialize(&szKey[0], 0);

		
}

int tconnapi_initialize(const char *a_pszGCIMKey, int a_iBusinessid)
{
	if (tbus_initialize(a_pszGCIMKey, a_iBusinessid) < 0)
	{
		return -1;
	}

	gs_pstLib				=	(LPTDRMETALIB) g_szMetalib_proto_ss;

	g_pstConnapiFrameHead	=       tdr_get_meta_by_name(gs_pstLib, "tframehead");

	if( g_pstConnapiFrameHead )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}


void tconnapi_fini(void)
{
	tbus_fini();
	return;
}

int tconnapi_create(int iProcID, int* piHandle)
{
	TCONNAPIHANDLE* pstHandle;

	pstHandle	=	(TCONNAPIHANDLE*) calloc(1, (int) sizeof(*pstHandle));

	if( !pstHandle )
	{
		return -1;
	}

	if( tbus_new(&pstHandle->iBus)<0 )
	{
		return -1;
	}

	pstHandle->iProc	=	iProcID;

	if( tbus_bind(pstHandle->iBus, (unsigned int)iProcID)<0 )
	{
		free(pstHandle);
		return -1;
	}

	if( piHandle )
	{
		*piHandle	=	(int)pstHandle;
	}

	return 0;
}

void tconnapi_free(int* piHandle)
{
	if( !piHandle )
	{
		return;
	}

	if( *piHandle )
	{
		free( (void*)*piHandle );
	}

	*piHandle	=	0;

	return;
}

int tconnapi_decode(const char* pszBuff, int iBuff, TFRAMEHEAD* pstHead, int* piHeadLen)
{
	TDRDATA stHost;
	TDRDATA stNet;

	stNet.pszBuff	=	(char*)pszBuff;
	stNet.iBuff		=	iBuff;

	stHost.pszBuff	=	(char*) pstHead;
	stHost.iBuff	=	sizeof(*pstHead);

	if( tdr_ntoh(g_pstConnapiFrameHead, &stHost, &stNet, 0)<0 )
	{
		errno	=	EBADF;
		return -1;
	}

	*piHeadLen	=	stNet.iBuff;

	return 0;
}

int tconnapi_encode(char* pszBuff, int* piBuff, TFRAMEHEAD* pstHead)
{
	TDRDATA stHost;
	TDRDATA stNet;

	stNet.pszBuff	=	(char*)pszBuff;
	stNet.iBuff	=	*piBuff;

	stHost.pszBuff	=	(char*) pstHead;
	stHost.iBuff	=	sizeof(*pstHead);

	if( tdr_hton(g_pstConnapiFrameHead, &stNet,&stHost,  0)<0 )
	{
		errno	=	EBADF;
		return -1;
	}

	*piBuff		=	stNet.iBuff;

	return 0;
}

int tconnapi_recv(int iHandle, int* piSrc, char* pszBuff, int* piBuff, TFRAMEHEAD* pstHead)
{
	TCONNAPIHANDLE* pstHandle;
	size_t iSize;
	char szPkg[TCONNAPI_MAX_BUFF];
	TDRDATA stHost;
	TDRDATA stNet;

	pstHandle	=	(TCONNAPIHANDLE*) iHandle;

	iSize	=	sizeof(szPkg);

	if( 0!=tbus_recv(pstHandle->iBus, piSrc, &pstHandle->iProc, szPkg, &iSize, 0) )
	{
		errno	=	EAGAIN;
		return  -1;
	}

	stHost.iBuff	=	(int)sizeof(pstHandle->stRecv);
	stHost.pszBuff	=	(char*)&pstHandle->stRecv;
	stNet.iBuff		=	iSize;
	stNet.pszBuff	=	szPkg;

	if( tdr_ntoh(g_pstConnapiFrameHead, &stHost, &stNet, 0)<0 )
	{
		errno	=	EBADF;
		return -1;
	}

	if( *piBuff < (int)iSize - stNet.iBuff )
	{
		*piBuff	=	iSize - stNet.iBuff;
		errno	=	ENOMEM;
		return -1;
	}

	*piBuff	=	iSize - stNet.iBuff;

	if( pstHead )
	{
		memcpy(pstHead, &pstHandle->stRecv, sizeof(*pstHead));
	}

	if( !pszBuff )
	{
		return -1;
	}

	memcpy(pszBuff, szPkg + stNet.iBuff, iSize - stNet.iBuff);

	memcpy(&pstHandle->stSend, &pstHandle->stRecv, sizeof(pstHandle->stSend));
	
	return 0;
}

int tconnapi_connect(int iHandle, int iDst)
{
	TCONNAPIHANDLE* pstHandle;

	pstHandle		=	(TCONNAPIHANDLE*) iHandle;

	return tbus_connect(pstHandle->iBus, iDst);
}

int tconnapi_send(int iHandle, int iDst, char* pszBuff, int iBuff, TFRAMEHEAD* pstHead)
{
	TCONNAPIHANDLE* pstHandle;
	int iRet;
	int iSrc;
	struct iovec astVecs[2];
	char szNetHead[sizeof(TFRAMEHEAD)*2];
	TDRDATA stNet;
	TDRDATA stHost;

	pstHandle		=	(TCONNAPIHANDLE*) iHandle;

	stNet.iBuff		=	(int)sizeof(szNetHead);
	stNet.pszBuff	=	szNetHead;

	if( pstHead )
	{
		stHost.iBuff	=	(int)sizeof(*pstHead);
		stHost.pszBuff	=	(char*)pstHead;
	}
	else
	{
		stHost.iBuff	=	(int)sizeof(pstHandle->stSend);
		stHost.pszBuff	=	(char*)&pstHandle->stSend;
	}

	if( tdr_hton(g_pstConnapiFrameHead, &stNet, &stHost, 0)<0 )
	{
		errno	=	EBADF;
		return -1;
	}

	astVecs[0].iov_len	=	stNet.iBuff;
	astVecs[0].iov_base	=	szNetHead;
	astVecs[1].iov_len	=	iBuff;
	astVecs[1].iov_base	=	pszBuff;

	iSrc	=	0;

	iRet	=	tbus_sendv(pstHandle->iBus, &iSrc, &iDst, astVecs, 2, 0);

	if( iRet<=0 )
	{
		return iRet;
	}

	return -1;
}

