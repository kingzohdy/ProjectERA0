// TaskPing.h: interface for the CTaskPing class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxsock.h>

#define REQ_DATASIZE 32		// Echo Request Data size
#define ICMP_ECHOREPLY	0
#define ICMP_ECHOREQ	8

#pragma pack(1)

// ICMP Header - RFC 792
typedef struct tagICMPHDR
{
	u_char	Type;			// Type
	u_char	Code;			// Code
	u_short	Checksum;		// Checksum
	u_short	ID;				// Identification
	u_short	Seq;			// Sequence
}ICMPHDR, *PICMPHDR;

// ICMP Echo Request
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
//	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;

// IP Header -- RFC 791
typedef struct tagIPHDR
{
	u_char  VIHL;			// Version and IHL
	u_char	TOS;			// Type Of Service
	short	TotLen;			// Total Length
	short	ID;				// Identification
	short	FlagOff;		// Flags and Fragment Offset
	u_char	TTL;			// Time To Live
	u_char	Protocol;		// Protocol
	u_short	Checksum;		// Checksum
	struct	in_addr iaSrc;	// Internet Address - Source
	struct	in_addr iaDst;	// Internet Address - Destination
}IPHDR, *PIPHDR;

// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;

#pragma pack()

struct IPingSink
{
	virtual void OnResult(LPCSTR szHost, DWORD dwIntervel) = NULL;
};

class CPing
{
public:
	CPing(LPCSTR szHost, IPingSink *pPingSink, int nRetries = 3);	
	virtual ~CPing();

	BOOL Open();
protected:
	static DWORD WINAPI Svc_( LPVOID lpParam);
	void Svc();					//启动类入口
	LPCSTR GetHostName();
private:
	int		Ping(UINT nRetries = 2);
	int		SendEchoRequest(SOCKET soc, LPSOCKADDR_IN addr);
	int		WaitForEchoReply(SOCKET soc);
	void	RecvEchoReply(SOCKET soc, LPSOCKADDR_IN addr, u_char * cTTL);
	u_short in_cksum(u_short *addr, int len);
protected:
	char		m_szHost[128];

	int			m_nRetries;
	DWORD		m_dwLastTimeRequest;
	IPingSink *	m_pPingSink;
	int			m_nReqID;
	int			m_nReplyID;
	HANDLE		m_hThread;	//线程
};