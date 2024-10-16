/*
**  @file $RCSfile: tnet.c,v $
**  general description of this module
**  $Id: tnet.c,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:28 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#include "pal/tos.h"
#include "pal/tsocket.h"
#include "pal/tfile.h"
#include "pal/tnet.h"

#include <assert.h>


#ifdef WIN32

#pragma warning(disable:4996)

#endif /* WIN32 */

static TNETPROTODEF gs_astProtos[]	=	
{
	{TNET_PROTO_TCP, 	TNET_ID_TCP, 	1,	1},
	{TNET_PROTO_UDP, 	TNET_ID_UDP,	1,	1},
	{TNET_PROTO_UNIX, 	TNET_ID_UNIX, 	1,	1},
	{TNET_PROTO_PIPE, 	TNET_ID_PIPE, 	1,	1},
	{TNET_PROTO_FILE, 	TNET_ID_FILE, 	1,	1},
	{TNET_PROTO_SHM, 	TNET_ID_SHM, 	0,	1},
	{TNET_PROTO_THTTP, 	TNET_ID_THTTP, 	1,	1},
};

static TNETTYPEDEF gs_astTypes[] = 
{
	{TNET_TYPE_STREAM, 	SOCK_STREAM},
	{TNET_TYPE_DGRAM, 	SOCK_DGRAM},
	{TNET_TYPE_SEQPACKET, 	SOCK_SEQPACKET},
	{TNET_TYPE_RAW, 	SOCK_RAW},
	{TNET_TYPE_RDM, 	SOCK_RDM},
#ifndef WIN32
	{TNET_TYPE_PACKET, 	SOCK_PACKET},
#endif
};

int tnet_is_number(const char* a_pszHost)
{
	int iSize;

	iSize	=	strspn(a_pszHost, TNET_STR_NUMBER);

	if( iSize==(int)strlen(a_pszHost) )
		return 1;
	else
		return 0;
}

int tnet_str2inet(const char* a_pszAddr, struct sockaddr_in* a_pstIn)
{
	char szHost[TNET_ADDR_LEN];
#ifndef WIN32
	char szBuff[TNET_BUFF_LEN];
	struct hostent stHost;
	int iErr;
#endif /* WIN32 */
	struct hostent* pstRet=NULL;
	char* pszPort;
	unsigned short wPort;

	assert( a_pszAddr && a_pstIn);

	a_pstIn->sin_family	=	AF_INET;

	pszPort	=	strchr( a_pszAddr, TNET_CHAR_PORT );

	if( pszPort )
	{
		if( pszPort - a_pszAddr >= TNET_ADDR_LEN )
			return -1;

		memcpy(szHost, a_pszAddr, pszPort - a_pszAddr);
		szHost[pszPort - a_pszAddr]	=	'\0';

		pszPort++;
	}
	else
	{
		if( strlen(a_pszAddr) >= TNET_ADDR_LEN )
			return -1;

		strcpy(szHost, a_pszAddr);

		pszPort	=	NULL; 
	}

	if( pszPort )
		wPort	=	atoi(pszPort);
	else
		wPort	=	0;

	a_pstIn->sin_port	=	HTONS(wPort);

	if( tnet_is_number(szHost) ) /* it is a numbering address. */
	{
		a_pstIn->sin_addr.s_addr	=	inet_addr(szHost);	
	}
	else
	{
#ifdef WIN32
		pstRet	=	gethostbyname(szHost);
#else
		gethostbyname_r(szHost, &stHost, szBuff, TNET_BUFF_LEN, &pstRet, &iErr);
#endif
		if( NULL==pstRet )
			return -1;

		a_pstIn->sin_addr=	*(struct in_addr*) pstRet->h_addr_list[0];
	}
	
	return 0;
}

#ifndef WIN32
int tnet_str2unix(const char* a_pszAddr, struct sockaddr_un* a_pstUn)
{
	int iLen;

	assert( a_pszAddr && a_pstUn);

	a_pstUn->sun_family	=	AF_UNIX;

	iLen	=	strlen(a_pszAddr);

	if( iLen >= UNIX_PATH_MAX )
		return -1;

	memcpy(a_pstUn->sun_path, a_pszAddr, iLen);
	a_pstUn->sun_path[iLen]	=	'\0';

	return 0;
}
#endif /* WIN32 */

int tnet_decode(char* a_pszDst, int* a_piDst, const char* a_pszSrc, int a_iSrc)
{
	assert( a_pszDst && a_piDst && a_pszSrc && a_iSrc );

	return 0;
}

int tnet_encode(char* a_pszDst, int* a_piDst, const char* a_pszSrc, int a_iSrc)
{
	assert( a_pszDst && a_piDst && a_pszSrc && a_iSrc );

	return 0;
}

const TNETPROTODEF* tnet_find_proto(const char* a_pszProto)
{
	int i;
	
	assert( a_pszProto);
	
	for(i=0; i<(int)(sizeof(gs_astProtos)/sizeof(TNETPROTODEF)); i++)
	{
		if( 0==strcmp(gs_astProtos[i].pszName, a_pszProto ) )
			return gs_astProtos + i;
	}
	
	return NULL;
}

const TNETTYPEDEF* tnet_find_type(const char* a_pszType)
{
	int i;
	
	assert( a_pszType );
	
	for(i=0; i<(int)(sizeof(gs_astTypes)/sizeof(TNETTYPEDEF)); i++)
	{
		if( 0==strcmp(gs_astTypes[i].pszType, a_pszType ) )
			return gs_astTypes + i;
	}
	
	return NULL;
}

const TNETARG* tnet_find_arg(TNETOPT* a_pstOpt, const char* a_pszName)
{
	int i;
	
	assert( a_pstOpt && a_pszName );
	
	for(i=0; i<a_pstOpt->iArgs; i++)
	{
		if( 0==strcmp(a_pstOpt->args[i].szName, a_pszName) )
			return a_pstOpt->args + i;
	}
	
	return NULL;
}

int tnet_get_arg(TNETOPT* a_pstOpt, const char* a_pszArg)
{
	int iLen;
	int iName;
	int iValue;
	char* pszNext;
	char* pszValue;
	
	assert( a_pstOpt );
	
	if( !a_pszArg )
		return 0;
		
	a_pstOpt->iArgs	=	0;
		
	do
	{
		pszNext	=	strchr(a_pszArg, TNET_CHAR_SEP);
		
		if( pszNext )
		{
			iLen	=	pszNext - a_pszArg;
			pszNext++;
		}
		else
		{
			iLen	=	strlen(a_pszArg);
			pszNext	=	NULL;
		}
		
		if( iLen<=0 )
			break;
		
		pszValue=	memchr(a_pszArg, TNET_CHAR_EQ, iLen);
		
		if( pszValue )
		{
			iName	=	pszValue - a_pszArg;
			iValue	=	iLen - iName - 1;
			
			pszValue++;
		}
		else
		{
			iName	=	iLen;
			iValue	=	0;
			
			pszValue	=	NULL;
		}
		
		if( iName>=TNET_NAME_LEN || iValue>=TNET_VALUE_LEN )
			return -1;
			
		if( iName > 0 )
		{
			memcpy( a_pstOpt->args[a_pstOpt->iArgs].szName, a_pszArg, iName);
			a_pstOpt->args[a_pstOpt->iArgs].szName[iName]	=	0;
		}
		else
			a_pstOpt->args[a_pstOpt->iArgs].szName[0]	=	0;
		
		if( iValue > 0 )
		{
			memcpy( a_pstOpt->args[a_pstOpt->iArgs].szValue, pszValue, iValue);
			a_pstOpt->args[a_pstOpt->iArgs].szValue[iValue]	=	0;
		}
		else
			a_pstOpt->args[a_pstOpt->iArgs].szValue[0]	=	0;
		
		a_pszArg	=	pszNext;
		
		a_pstOpt->iArgs++;
	}
	while(a_pszArg);

	return 0;
}

int tnet_get_opt(TNETOPT* a_pstOpt, const char* a_pszUri)
{
	const char* pszProto;
	const char* pszArg;
	int iLen;

	assert( a_pstOpt && a_pszUri );

	pszProto	=	strstr(a_pszUri, TNET_STR_PROTO);

	if( pszProto )
	{
		if( pszProto - a_pszUri >= TNET_PROTO_LEN || pszProto == a_pszUri )
			return -1;

		memcpy(a_pstOpt->szProto, a_pszUri, pszProto - a_pszUri);

		a_pstOpt->szProto[pszProto - a_pszUri]	=	'\0';

		a_pszUri	=	pszProto + strlen(TNET_STR_PROTO);
	}
	else
	{
		strcpy(a_pstOpt->szProto, TNET_DEF_PROTO);
	}

	pszArg		=	strchr(a_pszUri, TNET_CHAR_ARG);
	
	if( pszArg )
		iLen	=	pszArg - a_pszUri;
	else
		iLen	=	strlen(a_pszUri);
	
	if( iLen>=TNET_ADDR_LEN )
		return -1;

	a_pstOpt->iArgs	=	0;

	if( iLen>0 )
	{
		memcpy(a_pstOpt->szAddr, a_pszUri, iLen);
		a_pstOpt->szAddr[iLen]	=	0;
	}
	else
	{
		a_pstOpt->szAddr[0]	=	0;
	}
	
	if( pszArg )
	{
		pszArg++;
		
		return tnet_get_arg(a_pstOpt, pszArg);
	}

	return 0;
}

int tnet_set_nonblock(int a_iSock, int a_iIsNonblock)
{
	return tsocket_set_nonblock(a_iSock, a_iIsNonblock);
}

int tnet_set_sendbuff(int a_iSock, int a_iSize)
{
	return tsocket_set_sendbuff(a_iSock, a_iSize);
}

int tnet_set_recvbuff(int a_iSock, int a_iSize)
{
	return tsocket_set_recvbuff(a_iSock, a_iSize);
}

int tnet_close(int a_iSock)
{
	return tsocket_close(a_iSock);
}

int tnet_open_by_proto(TNETOPT* a_pstOpt, const TNETPROTODEF* a_pstProto)
{
	int iType;
	const TNETARG* pstArg;
	const TNETTYPEDEF* pstType;

	assert( a_pstOpt && a_pstProto );

	switch( a_pstProto->iID )
	{
	case TNET_ID_UDP:
		return tsocket_open_udp();

	case TNET_ID_THTTP:
	case TNET_ID_TCP:
		return tsocket_open_tcp();

	case TNET_ID_UNIX:
		iType	=	SOCK_STREAM;
		
		pstArg	=	tnet_find_arg(a_pstOpt, TNET_ARG_TYPE);

		if( pstArg )
		{
			pstType	=	tnet_find_type(pstArg->szValue);

			if( pstType )
				iType	=	pstType->iType;	
		}

		return tsocket_open_unix(SOCK_STREAM);

	case TNET_ID_PIPE:
	case TNET_ID_SHM:
	case TNET_ID_FILE:
		return -1;
	default:
		break;
	}

	return -1;
}

int tnet_open(const char* a_pszUri)
{
	TNETOPT stOpt;
	const TNETPROTODEF* pstProto;

	if( -1==tnet_get_opt(&stOpt, a_pszUri) )
		return -1;

	pstProto	=	tnet_find_proto(stOpt.szProto);

	if( NULL==pstProto )
		return -1;

	return tnet_open_by_proto(&stOpt, pstProto);
}

int tnet_listen(const char* a_pszUri, int a_iBacklog)
{	
	TNETOPT stOpt;
	TNETADDR stAddr;
	const TNETARG* pstArg;
	const TNETPROTODEF* pstProto;
	int s;
	int fReuseAddr;

	if( -1==tnet_get_opt(&stOpt, a_pszUri) )
		return -1;

	pstProto	=	tnet_find_proto(stOpt.szProto);

	if( NULL==pstProto )
		return -1;

	s	=	tnet_open_by_proto(&stOpt, pstProto);

	if( -1==s )
		return -1;

	pstArg	=	tnet_find_arg(&stOpt, TNET_ARG_REUSE);

	if( pstArg )
		fReuseAddr	=	atoi(pstArg->szValue);
	else
		fReuseAddr	=	0;	

	if( fReuseAddr )
		tsocket_reuse_addr(s, fReuseAddr);

	switch(pstProto->iID)
	{
	case TNET_ID_THTTP:
	case TNET_ID_TCP:
		if( tnet_str2inet(stOpt.szAddr, &stAddr.stIn) < 0 )
			break;
		
		if( tsocket_listen(s, (struct sockaddr*)&stAddr.stIn, sizeof(stAddr.stIn), a_iBacklog)<0 )
			break;
		else
			return s;

		break;

	case TNET_ID_UDP:
		if( tnet_str2inet(stOpt.szAddr, &stAddr.stIn) < 0 )
			break;
		
		if( bind(s, (struct sockaddr*)&stAddr.stIn, sizeof(stAddr.stIn))<0 )
			break;
		else
			return s;

		break;

#ifndef WIN32
	case TNET_ID_UNIX:
		if( tnet_str2unix(stOpt.szAddr, &stAddr.stUn) < 0 )
			break;

		unlink(stOpt.szAddr);
		
		if( tsocket_listen(s, (struct sockaddr*)&stAddr.stUn, sizeof(stAddr.stUn), a_iBacklog)<0 )
			break;
		else
			return s;

		break;

#endif /* WIN32 */

	default:
		break;
	}

	tnet_close(s);

	return -1;
}

int tnet_connect(const char* a_pszUri, int a_iTimeout)
{	
	TNETOPT stOpt;
	TNETADDR stAddr;
	const TNETPROTODEF* pstProto;
	int s;

	if( -1==tnet_get_opt(&stOpt, a_pszUri) )
		return -1;


	pstProto	=	tnet_find_proto(stOpt.szProto);

	if( NULL==pstProto )
		return -1;

	s	=	tnet_open_by_proto(&stOpt, pstProto);

	if( -1==s )
		return -1;

	tnet_set_nonblock(s, 1);

	switch(pstProto->iID)
	{
	case TNET_ID_TCP:
	case TNET_ID_UDP:
		if( tnet_str2inet(stOpt.szAddr, &stAddr.stIn) < 0 )
			break;
		
		if( tsocket_connect(s, (struct sockaddr*)&stAddr.stIn, sizeof(stAddr.stIn), a_iTimeout)<0 )
			break;
		else
			return s;

		break;

#ifndef WIN32
	case TNET_ID_UNIX:
		if( tnet_str2unix(stOpt.szAddr, &stAddr.stUn) < 0 )
			break;
		
		if( tsocket_connect(s, (struct sockaddr*)&stAddr.stUn, sizeof(stAddr.stUn), a_iTimeout)<0 )
			break;
		else
			return s;

		break;
#endif /* WIN32 */

	default:
		break;
	}

	tnet_close(s);

	return -1;
}

int tnet_accept(int a_iSock, struct sockaddr* a_pstAddr, int* a_piLen, int a_iTimeout)
{
	return tsocket_accept(a_iSock, a_pstAddr, a_piLen, a_iTimeout);
}

int tnet_send(int a_iSock, const char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	return tsocket_send(a_iSock, a_pszBuff, a_iLen, a_iTimeout);
}

int tnet_recv(int a_iSock, char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	return tsocket_recv(a_iSock, a_pszBuff, a_iLen, a_iTimeout);
}

int tnet_sendall(int a_iSock, const char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	return tsocket_sendall(a_iSock, a_pszBuff, a_iLen, a_iTimeout);
}

int tnet_recvall(int a_iSock, char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	return tsocket_recvall(a_iSock, a_pszBuff, a_iLen, a_iTimeout);
}

int tnet_write(int a_iSock, const char* a_pszBuff, int a_iLen)
{
	return tfwrite(a_iSock, a_pszBuff, a_iLen);
}

int tnet_read(int a_iSock, char* a_pszBuff, int a_iLen)
{
	return tfread(a_iSock, a_pszBuff, a_iLen);
}
