//#include "stdafx.h"
#include "Ping.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

CPing::CPing(void)
{
	m_socketRaw		= INVALID_SOCKET;
	m_bPrintInfo	= false;
	m_ulTimestamp	= 0;
	m_nMaxTime		= 0 ;
	m_nMinTime		= 1000;
	Init();
}

CPing::CPing(int timeOut)
{
	m_socketRaw		= INVALID_SOCKET;
	m_bPrintInfo	= false;
	m_ulTimestamp	= 0;
	m_nMaxTime		= 0 ;
	m_nMinTime		= 1000;
	Init( timeOut);
}

CPing::~CPing(void)
{
	FInit();
}

bool CPing::Init( int timeOut )
{
	WSADATA wsaData = {0};
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
		return false;

	//创建发送套接字
	// 注：为了使用发送接收超时设置(即设置SO_RCVTIMEO, SO_SNDTIMEO)，
	m_socketRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (m_socketRaw == SOCKET_ERROR) 
	{		
		closesocket( m_socketRaw );
		return false;
	}
	// 设置一个有接收时限的套接字
	int nTimeout	= timeOut;
	int nResult		= setsockopt(m_socketRaw, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTimeout, sizeof(nTimeout));
	if(nResult == SOCKET_ERROR)
	{
		closesocket( m_socketRaw );
		return false;
	}
	//设置一个有发送时限的套接字
	nTimeout	= 100;
	nResult		= setsockopt(m_socketRaw, SOL_SOCKET, SO_SNDTIMEO, (char*)&nTimeout, sizeof(nTimeout));
	if(nResult == SOCKET_ERROR)
	{
		closesocket( m_socketRaw );
		return false;
	}
	return true;
}

bool CPing::FInit()
{
	WSACleanup();
	return true;
}


bool CPing::PingHelper( sockaddr_in& addrDsc, int nPackNum, int nDataSize)
{
	char szIcmpData[MAX_ICMP_PACKET] = {0};
	char szRecvBuff[MAX_ICMP_PACKET] = {0};
	ConstructIcmpMessage(szIcmpData, nDataSize);
	IcmpHeader* pIcmpHeader = reinterpret_cast<IcmpHeader*>(szIcmpData);
	int nPackSize	= nDataSize + sizeof(IcmpHeader);
	int nSeqNO		= 0;
	int nRet		= 0;
	int nRecvPacket = 0;
	nReceivedTime = 0;
	nReceivedNum = 0;

	sockaddr_in addrSrc = {0};

	for (int i = 0; i < nPackNum; ++i)
	{
		pIcmpHeader->i_cksum	= 0; // 先把ICMP的检验和置零
		pIcmpHeader->i_seq		= nSeqNO++; // 发送顺序加一
		pIcmpHeader->i_cksum	= CheckSum((USHORT*)szIcmpData, nPackSize);// 计算检验和
		m_ulTimestamp			= GetTickCount(); // 记录时间
		nRet = sendto(m_socketRaw, szIcmpData, nPackSize, 0, (sockaddr*)&addrDsc, sizeof(addrDsc));
		if (nRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT) // 发送超时
			{
				continue;
			}
			closesocket( m_socketRaw );
			return false;
		}

		int nSrcLen = sizeof(addrSrc);
		nRet		= recvfrom(m_socketRaw, szRecvBuff, sizeof(szRecvBuff), 0, (sockaddr*)&addrSrc, &nSrcLen);
		if (nRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT) // 接收超时
			{
				continue;
			}
			closesocket( m_socketRaw );
			return false;
		}

		if ( DecodeIpPacket( szRecvBuff, nRet, &addrSrc ) )
		{
			++nRecvPacket;
			++nReceivedNum;
		}
		
	}

	nReceivedTime = ( nReceivedTime + ( nPackNum - nRecvPacket ) * 1000 ) / nPackNum;

	if (m_bPrintInfo)
	{
		std::cout << std::endl << "Packets: Sent = " << nPackNum;
		std::cout << ", Received = " << nRecvPacket;
		std::cout << ", Lost = " << nPackNum-nRecvPacket;
		std::cout << " (" << (nPackNum-nRecvPacket)*100/nPackNum;
		std::cout << "% loss)" << std::endl;
	}

	closesocket( m_socketRaw );
	return nRecvPacket > 0;
}

bool CPing::PingHelper(const char* pDscAddr, int nPackNum , int nDataSize )
{
	nReceivedTime	= 0;
	nReceivedNum	= 0;
	if (pDscAddr == NULL || nPackNum <= 0)
		return false;

	// 进行域名解析
	sockaddr_in addrDsc = {0};

	/*hostent* pHostent = gethostbyaddr(pDscAddr, strlen(pDscAddr), AF_INET);
	if (pHostent != NULL)
	{
		addrDsc.sin_family = AF_INET;
		addrDsc.sin_addr.S_un.S_addr = inet_addr(pDscAddr);
	}
	else
	{
		pHostent = gethostbyname(pDscAddr);
		if (pHostent == NULL)
			return false;

		addrDsc.sin_family = pHostent->h_addrtype;
		memcpy(&(addrDsc.sin_addr), pHostent->h_addr, pHostent->h_length);
	}*/
	addrDsc.sin_family = AF_INET;
	addrDsc.sin_addr.S_un.S_addr = inet_addr(pDscAddr);
	if (m_bPrintInfo)
	{
		std::cout << "Pinging " << inet_ntoa(addrDsc.sin_addr);
		std::cout << " with " << nDataSize << " bytes of data:" << std::endl << std::endl;
	}
	return PingHelper( addrDsc, nPackNum, nDataSize );
}

bool CPing::ConstructIcmpMessage(char* pDataBuff, int nDataSize)
{
	if (pDataBuff == NULL)
		return false;

	IcmpHeader* pIcmpHeader = (IcmpHeader*)pDataBuff;
	pIcmpHeader->i_type		= ICMP_ECHO;     // 设置回显报文
	pIcmpHeader->i_code		= 0;
	pIcmpHeader->i_id		= (USHORT)GetCurrentProcessId();// 获取当前进程ID
	pIcmpHeader->i_cksum	= 0;       // 初始化检验和为0
	pIcmpHeader->i_seq		= 0;       // 顺序符

	char* pDataPart			= pDataBuff + sizeof(IcmpHeader);  // 给数据区赋地址
	memset(pDataPart, '0', nDataSize);      // 把数据区初始化

	return true;
}

// 计算校验和
USHORT CPing::CheckSum(USHORT* pBuff, int nSize)
{
	unsigned long ulChkSum = 0;			// 给检验和置0
	while(nSize > 1)
	{
		ulChkSum += *pBuff++;			// 把所有IP数据报的数据都加到一起
		nSize -= sizeof(USHORT);		// 每次计算两个十二位二进制和 （每次减两个字节）
	}
	if(nSize != 0)
	{
		ulChkSum += *(UCHAR*)pBuff;		// 判断当只剩下一个字节没有加到检验和时在这里加上
	}
	ulChkSum = (ulChkSum >> 16) + (ulChkSum & 0XFFFF); // 先按32bit计算二进制和得到cksum，然后把高位低位相加
														//得到16bit的和。
	ulChkSum += (ulChkSum >>16);		// 把高16位和低16位相加溢出的进位再加到校验和中

	return (USHORT)(~ulChkSum);			// 取反得到检验和
}

bool CPing::DecodeIpPacket( char* pBuff, int nBuffSize, sockaddr_in* pAddr )
{
	if (pBuff == NULL || nBuffSize <= 0)
		return false;

	IpHeader* pIpHeader				= (IpHeader*)pBuff;
	unsigned short usIpHeaderLen	= (pIpHeader->h_len) * 4 ; //计算IP首部的长度
	IcmpHeader* pIcmpHeader			= (IcmpHeader*)(pBuff + usIpHeaderLen);
	//判断是否是回显应答报文
	if ( pIcmpHeader->i_type != ICMP_ECHOREPLY )
		return false;

	// 判断是否是当前进程的IP地址，并确认是此次的回显报文。
	if ( pIcmpHeader->i_id != (USHORT)GetCurrentProcessId() )
		return false;

	if (nBuffSize < usIpHeaderLen + ICMP_MIN)
		return false;

	if (m_bPrintInfo)
	{
		std::cout << "Reply from " << inet_ntoa(pAddr->sin_addr);
		std::cout << ": bytes="<< nBuffSize-usIpHeaderLen-ICMP_MIN;
		std::cout << " time=" << GetTickCount()- m_ulTimestamp;
		std::cout << "ms TTL=" << pIpHeader->ttl << std::endl;
	}
	int time = GetTickCount()- m_ulTimestamp;
	m_nMaxTime = ( time > m_nMaxTime )? time : m_nMaxTime;
	m_nMinTime = ( time < m_nMinTime )? time : m_nMinTime;

	nReceivedTime +=  time;
	return true;
}

bool CPing::CanPing(const char* pDscAddr)
{
	m_bPrintInfo = false;
	return PingHelper(pDscAddr);
}

void CPing::Ping(const char* pDscAddr, int nPackNum , int nPackSize )
{
	m_bPrintInfo = false;
	PingHelper(pDscAddr, nPackNum, nPackSize);
}

void CPing::Ping(sockaddr_in& pDscAddr, int nPackNum, int nPackSize )
{
	m_bPrintInfo = false;
	PingHelper(pDscAddr, nPackNum, nPackSize);
}