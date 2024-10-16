/*
**  @file $RCSfile: tsocket.c,v $
**  general description of this module
**  $Id: tsocket.c,v 1.1.1.1 2011/01/05 10:59:28 jerry Exp $
**  @author $Author: jerry $
**  @date $Date: 2011/01/05 10:59:28 $
**  @version $Revision: 1.1.1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


/*
 * Copyright (c) 1983, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "pal/tos.h"
#include "pal/ttypes.h"
#include "pal/ttime.h"
#include "pal/tstdio.h"
#include "pal/tsocket.h"

#ifdef WIN32
#pragma warning(disable:4018)
#endif /* WIN32 */

#ifdef WIN32

int inet_aton(const char* cp, struct in_addr* inp)
{
	unsigned long ulAddr;

	ulAddr	=	inet_addr(cp);

	if( ulAddr!=0xffffffff )
	{
		inp->S_un.S_addr	=	ulAddr;
		return 0;
	}

	inp->S_un.S_addr	=	ulAddr;

	return -1;
}


/*
 * Internet network address interpretation routine.
 * The library routines call this routine to interpret
 * network numbers.
 */
in_addr_t inet_network(const char *cp)
{
	register in_addr_t val, base, n, i;
	register char c;
	in_addr_t parts[4], *pp = parts;
	int digit;

again:
	val = 0; base = 10; digit = 0;
	if (*cp == '0')
		digit = 1, base = 8, cp++;
	if (*cp == 'x' || *cp == 'X')
		base = 16, cp++;
	while ((c = *cp) != 0) {
		if (isdigit(c)) {
			if (base == 8 && (c == '8' || c == '9'))
				return (INADDR_NONE);
			val = (val * base) + (c - '0');
			cp++;
			digit = 1;
			continue;
		}
		if (base == 16 && isxdigit(c)) {
			val = (val << 4) + (tolower (c) + 10 - 'a');
			cp++;
			digit = 1;
			continue;
		}
		break;
	}
	if (!digit)
		return (INADDR_NONE);
	if (pp >= parts + 4 || val > 0xff)
		return (INADDR_NONE);
	if (*cp == '.') {
		*pp++ = val, cp++;
		goto again;
	}
	if (*cp && !isspace(*cp))
		return (INADDR_NONE);
	if (pp >= parts + 4 || val > 0xff)
		return (INADDR_NONE);
	*pp++ = val;
	n = pp - parts;
	for (val = 0, i = 0; i < n; i++) {
		val <<= 8;
		val |= parts[i] & 0xff;
	}
	return (val);
}


/*
 * Formulate an Internet address from network + host.  Used in
 * building addresses stored in the ifnet structure.
 */
struct in_addr inet_makeaddr(in_addr_t net, in_addr_t host)
{
	struct in_addr in;

	if (net < 128)
		in.s_addr = (net << IN_CLASSA_NSHIFT) | (host & IN_CLASSA_HOST);
	else if (net < 65536)
		in.s_addr = (net << IN_CLASSB_NSHIFT) | (host & IN_CLASSB_HOST);
	else if (net < 16777216L)
		in.s_addr = (net << IN_CLASSC_NSHIFT) | (host & IN_CLASSC_HOST);
	else
		in.s_addr = net | host;

	in.s_addr = htonl(in.s_addr);

	return in;
}



/*
 * Return the network number from an internet
 * address; handles class a/b/c network #'s.
 */
in_addr_t inet_netof(struct in_addr in)
{
	register unsigned long i = ntohl(in.s_addr);

	if (IN_CLASSA(i))
		return (((i)&IN_CLASSA_NET) >> IN_CLASSA_NSHIFT);
	else if (IN_CLASSB(i))
		return (((i)&IN_CLASSB_NET) >> IN_CLASSB_NSHIFT);
	else
		return (((i)&IN_CLASSC_NET) >> IN_CLASSC_NSHIFT);
}


/*
 * Return the local network address portion of an
 * internet address; handles class a/b/c network
 * number formats.
 */
in_addr_t inet_lnaof(struct in_addr in)
{
	register i = ntohl(in.s_addr);

	if (IN_CLASSA(i))
		return ((i)&IN_CLASSA_HOST);
	else if (IN_CLASSB(i))
		return ((i)&IN_CLASSB_HOST);
	else
		return ((i)&IN_CLASSC_HOST);
}


#endif /* WIN32 */

int tsocket_init(int a_iMajor, int a_iMinor)
{
#ifdef WIN32
	WSADATA stWsa;
	int iRet;
	WORD wVer;

	wVer	=	MAKEWORD(a_iMajor, a_iMinor);

	iRet	=	WSAStartup(wVer, &stWsa);

	return iRet;

#else /* WIN32 */
	return 0;
#endif /* WIN32 */
}

int tsocket_fini(void)
{
#ifdef WIN32
	WSACleanup();
	return 0;
#else /* WIN32 */
	return 0;
#endif /* WIN32 */
}

int tsocket_open(int a_iAf, int a_iType, int a_iProto)
{
	return socket(a_iAf, a_iType, a_iProto);
}

int tsocket_open_tcp(void)
{
	return tsocket_open(PF_INET, SOCK_STREAM, 0);
}

int tsocket_open_udp(void)
{
	return tsocket_open(PF_INET, SOCK_DGRAM, 0);
}

int tsocket_open_unix(int a_iType)
{
	return tsocket_open(PF_UNIX, a_iType, 0);
}

int tsocket_reuse_addr(int a_iSock, int a_iIsReuseAddr)
{
	return setsockopt(a_iSock, SOL_SOCKET, SO_REUSEADDR, (char*)&a_iIsReuseAddr, sizeof(a_iIsReuseAddr));
}

int tsocket_set_nonblock(int a_iSock, int a_iIsNonblocking)
{
#ifdef WIN32
	unsigned long lParam;

	lParam	=	a_iIsNonblocking;

	return ioctlsocket(a_iSock, FIONBIO, &lParam);

#else
	int iFlags;
	
	iFlags = fcntl(a_iSock, F_GETFL, 0);

	if( a_iIsNonblocking )
		iFlags	|=	O_NONBLOCK | O_ASYNC;
	else
		iFlags	&=	~(O_NONBLOCK | O_ASYNC);

	return fcntl(a_iSock, F_SETFL, iFlags);
#endif
}

int tsocket_listen(int a_iSock, struct sockaddr* a_pstAddr, int a_iLen, int a_iBacklog)
{
	if( bind(a_iSock, a_pstAddr, a_iLen)<0 )
		return -1;

	return listen(a_iSock, a_iBacklog);
}

int tsocket_start_connect(int a_iSock, struct sockaddr* a_pstAddr, int a_iLen)
{
	int iErr;

	if( 0==connect(a_iSock, a_pstAddr, a_iLen) )
		return 0;

#ifdef WIN32
	iErr	=	WSAGetLastError(); 
	
	if( WSAEISCONN==iErr )
		return 0;
	else if( WSAEWOULDBLOCK==iErr || WSAEALREADY==iErr )
		return TSOCKET_INPROGRESS;
	else
		return -1;
#else
	iErr	=	errno;

	if( EISCONN==iErr )
		return 0;
	else if( EINPROGRESS==iErr || EALREADY==iErr )
		return TSOCKET_INPROGRESS;
	else
		return -1;
#endif
}

int tsocket_check_connect(int a_iSock, int a_iTimeout)
{	
	fd_set wrfs;
	struct timeval tv;
	int iRet;

#ifndef WIN32
	int iError;
	socklen_t iLen;
#else /* WIN32 */
	fd_set exceptfs;
#endif /* WIN32 */
	
	FD_ZERO(&wrfs);
	FD_SET(a_iSock, &wrfs);

	MS_TO_TV(tv, a_iTimeout);

#ifdef WIN32
	
	FD_ZERO(&exceptfs);
	FD_SET(a_iSock, &exceptfs);
	
	if( TSOCKET_WAIT_INFINITE==a_iTimeout )
		iRet    =       select(a_iSock+1, NULL, &wrfs, &exceptfs, NULL);
	else
		iRet    =       select(a_iSock+1, NULL, &wrfs, &exceptfs, &tv);
	
	if( iRet<0 )
		return -1;
	else if( 0==iRet )
		return TSOCKET_TIMEOUT;
	
	if( FD_ISSET(a_iSock, &exceptfs) )
		return -1;
	else
		return 0;
	
#else
	if( TSOCKET_WAIT_INFINITE==a_iTimeout )
		iRet    =       select(a_iSock+1, NULL, &wrfs, NULL, NULL);
	else
		iRet    =       select(a_iSock+1, NULL, &wrfs, NULL, &tv);
	
	if( iRet<0 )
		return -1;
	else if( 0==iRet )
		return TSOCKET_TIMEOUT;
	
	iError  =       0;
	iLen    =       sizeof(iLen);
	
	getsockopt(a_iSock, SOL_SOCKET, SO_ERROR, (char*)&iError, &iLen);
	
	if( 0==iError )
	    return 0;
	else
	    return -1;
#endif
}

int tsocket_connect(int a_iSock, struct sockaddr* a_pstAddr, int a_iLen, int a_iTimeout)
{	
	int iErr, iErr1;

	iErr	=	tsocket_start_connect(a_iSock, a_pstAddr, a_iLen);

	if( 0==iErr )
		return 0;
	else if( -1==iErr )
		return -1;

	iErr1	=	tsocket_check_connect(a_iSock, a_iTimeout);
	
	if( 0!=iErr1 )
		return -1;
	else
		return 0;
}

int tsocket_accept(int a_iSock, struct sockaddr* a_pstAddr, int* a_piLen, int a_iTimeout)
{	
	fd_set rdfs;
	struct timeval tv;
	int iRet;
	
	FD_ZERO(&rdfs);
	FD_SET(a_iSock, &rdfs);

	MS_TO_TV(tv, a_iTimeout);

	if( TSOCKET_WAIT_INFINITE==a_iTimeout )
		iRet    =       select(a_iSock+1, &rdfs, NULL, NULL, NULL);
	else
		iRet    =       select(a_iSock+1, &rdfs, NULL, NULL, &tv);
	
	if( iRet<=0 )
		return -1;

	return accept(a_iSock, a_pstAddr, (socklen_t*)a_piLen);
}

int tsocket_close(int a_iSock)
{
#ifdef WIN32
	return closesocket(a_iSock);
#else
	return close(a_iSock);
#endif
}

int tsocket_send(int a_iSock, const char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	int iRet;
	struct timeval tv;
	struct timeval* pstTV;

	fd_set  fds;

	FD_ZERO(&fds);
	FD_SET(a_iSock, &fds);
	
	if( a_iTimeout )
	{
		pstTV	=	&tv;

		if( TSOCKET_WAIT_INFINITE==a_iTimeout )
			pstTV	=	NULL;
		else
			MS_TO_TV(tv, a_iTimeout);

		iRet		=	select( a_iSock+1, NULL, &fds, NULL, pstTV );

		if( iRet<0 )
			return -1;

		if( 0==iRet )
			return 0;
	}
		
	iRet	=	send(a_iSock, a_pszBuff, a_iLen, 0);

	if( iRet<0 )
	{
		if( SOCKETEWOULDBLOCK==socketerrno || SOCKETEAGAIN==socketerrno )
		{
			if( a_iTimeout )
				usleep(200000);

			return 0;
		}

		return -1;
	}
	else if( 0==iRet )
		return -1;
	else
		return iRet;
}

int tsocket_sendall(int a_iSock, const char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	int iTotal;
	int iRet;
	struct timeval tv;
	struct timeval* pstTV;

	fd_set  fds;

	FD_ZERO(&fds);
	FD_SET(a_iSock, &fds);
	
	iTotal	=	0;
	pstTV	=	&tv;

	if( TSOCKET_WAIT_INFINITE==a_iTimeout )
		MS_TO_TV(tv, 500);
	else
		MS_TO_TV(tv, a_iTimeout);

	while( iTotal<a_iLen )
	{
		iRet		=	select( a_iSock+1, NULL, &fds, NULL, pstTV );

		if( iRet<0 )
			return -1;

		if( 0==iRet && TSOCKET_WAIT_INFINITE!=a_iTimeout )
			return iTotal;
		
		iRet	=	send(a_iSock, a_pszBuff + iTotal, a_iLen - iTotal, 0);

		if( iRet<0 )
		{
			//perror("x:");
			if( SOCKETEWOULDBLOCK==socketerrno || SOCKETEAGAIN==socketerrno )
			{
				usleep(200000);

				continue;
			}

			return -1;
		}


		if( 0==iRet )
		{
			if( 0==iTotal )
				return -1;
			else
				return iTotal;
		}
		
		iTotal +=	iRet;
	}

	return iTotal;
}

int tsocket_recv(int a_iSock, char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	int iRet;
	struct timeval tv;
	struct timeval* pstTV;

	fd_set  fds;

	FD_ZERO(&fds);
	FD_SET(a_iSock, &fds);
	
	if( a_iTimeout )
	{
		pstTV	=	&tv;

		if( TSOCKET_WAIT_INFINITE==a_iTimeout )
			pstTV	=	NULL;
		else
			MS_TO_TV(tv, a_iTimeout);

		iRet		=	select( a_iSock+1, &fds, NULL, NULL, pstTV );

		if( iRet<0 )
		{
			if (SOCKETINTR == socketerrno)	
			{
				return 0;
			}
			
			return -1;
		}

		if( 0==iRet )
			return 0;
	}

		
	iRet	=	recv(a_iSock, a_pszBuff, a_iLen, 0);

	if( iRet<0 )
	{
		if( SOCKETEWOULDBLOCK==socketerrno || SOCKETEAGAIN==socketerrno )
		{
			if( a_iTimeout )
				usleep(100000);

			return 0;
		}

		return -1;
	}
	else if( 0==iRet )
		return -1;
	else	
		return iRet;
}

int tsocket_recvall(int a_iSock, char* a_pszBuff, int a_iLen, int a_iTimeout)
{
	int iTotal;
	int iRet;
	struct timeval tv;
	struct timeval* pstTV;

	fd_set  fds;

	FD_ZERO(&fds);
	FD_SET(a_iSock, &fds);
	
	iTotal	=	0;
	pstTV	=	&tv;

	if( TSOCKET_WAIT_INFINITE==a_iTimeout )
		pstTV	=	NULL;
	else
		MS_TO_TV(tv, a_iTimeout);

	while( iTotal<a_iLen )
	{
		iRet		=	select( a_iSock+1, &fds, NULL, NULL, pstTV );

		if( iRet<0 )
		{
			if (SOCKETINTR == socketerrno)	
			{
				return iTotal;
			}
			
			return -1;
		}

		if( 0==iRet )
			return iTotal;

		
		iRet	=	recv(a_iSock, a_pszBuff + iTotal, a_iLen - iTotal, 0);

		if( iRet<0 )
		{
			if( SOCKETEWOULDBLOCK==socketerrno || SOCKETEAGAIN==socketerrno )
			{
				tos_usleep(100000);
				continue;
			}

			return -1;
		}


		if( 0==iRet )
		{
			if( 0==iTotal )
				return -1;
			else
				return iTotal;
		}
		
		iTotal +=	iRet;
	}

	return iTotal;
}


int tsocket_set_recvbuff(int a_iSock, int a_iSize)
{
	return setsockopt(a_iSock, SOL_SOCKET, SO_RCVBUF, (void*)&a_iSize, sizeof(a_iSize));
}

int tsocket_set_sendbuff(int a_iSock, int a_iSize)
{
	return setsockopt(a_iSock, SOL_SOCKET, SO_SNDBUF, (void*)&a_iSize, sizeof(a_iSize));
}

int tsocket_get_type(int a_iSock, int* a_piType)
{
	socklen_t iSize;

	iSize	=	sizeof(*a_piType);
	return getsockopt(a_iSock, SOL_SOCKET, SO_TYPE, (void*)a_piType, &iSize);
}

int tsocket_get_acceptconn(int a_iSock, int* a_piAcceptConn)
{
	socklen_t iSize;

	iSize	=	sizeof(*a_piAcceptConn);
	return getsockopt(a_iSock, SOL_SOCKET, SO_ACCEPTCONN, (void*)a_piAcceptConn, &iSize);
}


