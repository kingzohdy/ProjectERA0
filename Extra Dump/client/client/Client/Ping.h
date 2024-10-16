#pragma once
#include <windows.h> 

#define ICMP_ECHO 8  // 定义回显报文代码
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8  // 最小8字节ICMP包
#define MAX_ICMP_PACKET 1024 // 最大ICMP大小

#pragma pack(1)
// IP头首部结构
struct IpHeader
{
	unsigned int h_len:4;		// 首部长度
	unsigned int version:4;		// IP版本
	unsigned char tos;			// 服务类型
	unsigned short total_len;	// 包总长度
	unsigned short ident;		// 标识符
	unsigned short frag_and_flags; // 标志
	unsigned char ttl;			// 生存周期
	unsigned char proto;		// protocol (TCP, UDP etc) 协议类型
	unsigned short checksum;	// IP检验和
	unsigned int sourceIP;		// 源地址IP
	unsigned int destIP;		// 目的地址IP
};

// ICMP 首部结构
struct IcmpHeader
{
	BYTE i_type;		// 类型
	BYTE i_code;		// 代码类型
	USHORT i_cksum;		// 检验和
	USHORT i_id;		// 地址
	USHORT i_seq;		// 发送顺序
						// 增加一个时间戳
//	ULONG timestamp;
};


class CPing
{
public:
	CPing(void);
	CPing( int timeOut );
	~CPing(void);

	bool CanPing(const char* pDscAddr);
	void Ping(const char* pDscAddr, int nPackNum = 2, int nPackSize = 32);
	void Ping(sockaddr_in& pDscAddr, int nPackNum = 2, int nPackSize = 32);
public:
	int nReceivedTime;
	int nReceivedNum;
	int m_nMaxTime;
	int m_nMinTime;

private:
	bool Init( int timeOut = 300 );
	bool FInit();
	bool ConstructIcmpMessage( char* pDataBuff, int nDataSize );
	USHORT CheckSum( USHORT* pBuff, int nSize );
	bool DecodeIpPacket( char* pBuff, int nBuffSize, sockaddr_in* pAddr );
	bool PingHelper( const char* pDscAddr, int nPackNum = 4, int nPackSize = 32 );
	bool PingHelper( sockaddr_in& addrDsc, int nPackNum = 4, int nDataSize = 32 );

private:
	SOCKET	m_socketRaw;
	bool	m_bPrintInfo;
	ULONG	m_ulTimestamp;
	
};

