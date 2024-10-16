#include "Ping.h"
#include <afxinet.h>
#include <process.h>


CPing::CPing(LPCSTR szHost, IPingSink *pPingSink, int nRetries) 
: m_dwLastTimeRequest(0), m_nRetries(nRetries), m_pPingSink(pPingSink)
{
	memset(m_szHost, 0, sizeof(m_szHost));
	strncpy(m_szHost, szHost, 128);
}

CPing::~CPing()
{
}

void CPing::Svc()					//启动类入口
{
	int nIntervel = Ping(m_nRetries);
	if (m_pPingSink != NULL)
	{
		m_pPingSink->OnResult(m_szHost, nIntervel);
	}
}

int CPing::Ping(UINT nRetries)
{
	int nTime = -1;
	int nTimeTotal = 0;

	SOCKET	  rawSocket;
	LPHOSTENT lpHost;
	UINT	  nLoop;
	int       nRet;
	struct    sockaddr_in saDest;
	struct    sockaddr_in saSrc;
//	DWORD	  dwTimeSent;
	DWORD	  dwElapsed;
	u_char    cTTL;


	// Create a Raw socket
	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (rawSocket == SOCKET_ERROR) 
	{		
		return nTime;
	}

	CString strServerName;
	CString strObject;		//查询对象名(http文件)
	INTERNET_PORT nPort;	//端口
	DWORD dwServiceType;	//服务类型
	AfxParseURL( //词法分析
		m_szHost, //被分析URL串
		dwServiceType, //服务类型，ftp，http等
		strServerName, //服务器名
		strObject, //URL中被查询对象
		nPort ); //URL指定的端口，可能为空
	
	// Setup destination socket address	
	saDest.sin_family = AF_INET;
	saDest.sin_port = 0;
	saDest.sin_addr.s_addr = inet_addr(strServerName);
	if (INADDR_NONE == saDest.sin_addr.s_addr)
	{
		// Lookup host
		lpHost = gethostbyname(strServerName);
		if (lpHost == NULL)
		{
			closesocket(rawSocket);
			return nTime;
		}
		
		saDest.sin_addr.s_addr = *((u_long FAR *) (lpHost->h_addr));
	}

	// Ping multiple times
	for (nLoop = 0; nLoop < nRetries; nLoop++)
	{
		// Send ICMP echo request
		SendEchoRequest(rawSocket, &saDest);

LOOP:
		//select等待1000ms
		nRet = WaitForEchoReply(rawSocket);
		if (nRet == SOCKET_ERROR)
		{			
			break;
		}

		if (nRet == 0)	//超时
		{			
			nTimeTotal += 1000;
		}
		else
		{
			// Receive reply
			RecvEchoReply(rawSocket, &saSrc, &cTTL);

			if (saDest.sin_addr.s_addr != saSrc.sin_addr.s_addr ||
				m_nReplyID != m_nReqID )
			{
				goto LOOP;
			}

			// Calculate elapsed time
			dwElapsed = GetTickCount() - m_dwLastTimeRequest;

			nTimeTotal += dwElapsed;
			Sleep(10);
		}
	} //for
	
	if (nLoop == nRetries)
	{
		nTime = nTimeTotal / nRetries;
	}

	nRet = closesocket(rawSocket);

	return nTime;
}

DWORD WINAPI CPing::Svc_( LPVOID lpParam)
{
	CPing* pBkProc =  (CPing*)lpParam;
	if (pBkProc == NULL)
	{
		return 1;
	}
	pBkProc->Svc();

	_endthread();
	return 0;
}

BOOL CPing::Open()				//启动处理过程
{
	DWORD dwThreadId = 0;
	DWORD dwThrdParam = 1; 
	m_hThread = CreateThread( 
		NULL,                        // default security attributes 
		0,                           // use default stack size  
		Svc_,						 // thread function 
		(void*)(this),                // argument to thread function 
		0,                           // use default creation flags 
		&dwThreadId);                // returns the thread identifier 

	if (!m_hThread) 
	{
		return FALSE;
	}
	CloseHandle(m_hThread);
	return TRUE;
}

int	CPing::SendEchoRequest(SOCKET soc, LPSOCKADDR_IN addr)
{
	static ECHOREQUEST echoReq;
	static u_short nId = 1;
	static u_short nSeq = 1;
	int nRet;
	
	// Fill in echo request
	echoReq.icmpHdr.Type		= ICMP_ECHOREQ;
	echoReq.icmpHdr.Code		= 0;
	echoReq.icmpHdr.Checksum	= 0;
	echoReq.icmpHdr.ID			= nId++;
	echoReq.icmpHdr.Seq			= nSeq++;
	m_nReqID					= echoReq.icmpHdr.ID;
	// Fill in some data to send
	for (nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = '0';
	
	// Save tick count when sent
	//echoReq.dwTime				= GetTickCount();
	
	m_dwLastTimeRequest = GetTickCount();

	// Put data in packet and compute checksum
	echoReq.icmpHdr.Checksum = in_cksum((u_short *)&echoReq, sizeof(ECHOREQUEST));
	
	// Send the echo request  								  
	nRet = sendto(soc,						/* socket */
		(LPSTR)&echoReq,			/* buffer */
		sizeof(ECHOREQUEST),
		0,							/* flags */
		(LPSOCKADDR)addr, /* destination */
		sizeof(SOCKADDR_IN));   /* address length */

	return (nRet);
}
int CPing::WaitForEchoReply(SOCKET s)
{
	struct timeval Timeout;
	fd_set readfds;
	
	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = 1;
    Timeout.tv_usec = 0;
	
	return(select(1, &readfds, NULL, NULL, &Timeout));
}

//
// Mike Muuss' in_cksum() function
// and his comments from the original
// ping program
//
// * Author -
// *	Mike Muuss
// *	U. S. Army Ballistic Research Laboratory
// *	December, 1983

/*
*			I N _ C K S U M
*
* Checksum routine for Internet Protocol family headers (C Version)
*
*/
u_short CPing::in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;
	
	/*
	*  Our algorithm is simple, using a 32 bit accumulator (sum),
	*  we add sequential 16 bit words to it, and at the end, fold
	*  back all the carry bits from the top 16 bits into the lower
	*  16 bits.
	*/
	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}
	
	/* mop up an odd byte, if necessary */
	if( nleft == 1 ) {
		u_short	u = 0;
		
		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}
	
	/*
	* add back carry outs from top 16 bits to low 16 bits
	*/
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}


void CPing::RecvEchoReply(SOCKET soc, LPSOCKADDR_IN addr, u_char *pTTL) 
{
	ECHOREPLY echoReply;
	int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);
	
	// Receive the echo reply	
	nRet = recvfrom(soc,					// socket
		(LPSTR)&echoReply,	// buffer
		sizeof(ECHOREPLY),	// size of buffer
		0,					// flags
		(LPSOCKADDR)addr,	// From address
		&nAddrLen);			// pointer to address len
	
	// return time sent and IP TTL
	*pTTL		= echoReply.ipHdr.TTL;
	m_nReplyID	= echoReply.echoRequest.icmpHdr.ID;
	//return (echoReply.echoRequest.dwTime); 
}

LPCSTR CPing::GetHostName()
{
	return m_szHost;
}