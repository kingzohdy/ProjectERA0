#pragma once
#include <windows.h> 

#define ICMP_ECHO 8  // ������Ա��Ĵ���
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8  // ��С8�ֽ�ICMP��
#define MAX_ICMP_PACKET 1024 // ���ICMP��С

#pragma pack(1)
// IPͷ�ײ��ṹ
struct IpHeader
{
	unsigned int h_len:4;		// �ײ�����
	unsigned int version:4;		// IP�汾
	unsigned char tos;			// ��������
	unsigned short total_len;	// ���ܳ���
	unsigned short ident;		// ��ʶ��
	unsigned short frag_and_flags; // ��־
	unsigned char ttl;			// ��������
	unsigned char proto;		// protocol (TCP, UDP etc) Э������
	unsigned short checksum;	// IP�����
	unsigned int sourceIP;		// Դ��ַIP
	unsigned int destIP;		// Ŀ�ĵ�ַIP
};

// ICMP �ײ��ṹ
struct IcmpHeader
{
	BYTE i_type;		// ����
	BYTE i_code;		// ��������
	USHORT i_cksum;		// �����
	USHORT i_id;		// ��ַ
	USHORT i_seq;		// ����˳��
						// ����һ��ʱ���
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

