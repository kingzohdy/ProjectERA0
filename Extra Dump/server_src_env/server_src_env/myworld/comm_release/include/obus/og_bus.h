
#ifndef _OG_BUS_H
#define _OG_BUS_H

#include "oi_log.h"
#include "og_channel.h"

#define BUS_VERSION	0

#define BUS_TRACE 01
#define BUS_SYN 02

//心跳保持包
#define BUS_KEEP 04

#define BUS_PROC	0
#define BUS_RELAY_SERVER	1
#define BUS_RELAY_CLIENT	2

#define MAX_BUS_PKG_DATA 65536
#define MAX_ATTACH_CHANNEL 1000

#define BUS_PKG_START		0
#define BUS_PKG_INPROC		1
#define BUS_PKG_STOP		2
#define BUS_PKG_REDIR		3

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned char cVer;
	unsigned char cHeadLen;
	char sCheckSum[2];
	char sBodyLen[4];
	char sDst[4];
	char sSrc[4];
	char sSeq[4];
	char sFlag[2];
	unsigned char cRoute;
	char cReserve;
} BusPkgHead;

struct tagConnInfo
{
	int iPkgCount;
	int iByteCount;
	int iPkgTotal;
	int iByteTotal;
	int iSeq;
	int iSeqValid;
	
	int iTry;
	time_t tStart;

	int iPkgValid;
	int iIsHead;
	int iPos;
	int iSend;
	
	BusPkgHead stHead;
};

typedef struct tagConnInfo		CONNINFO;
typedef struct tagConnInfo		*LPCONNINFO;

typedef struct {
	BusPkgHead stHead;
	char sData[MAX_BUS_PKG_DATA];
} BusPkg;

typedef struct {
	int iDst;
	Channel stChannel;
} AttachChannel;

typedef struct {
	int iProcID;
	BusPkg stRecvPkg;
	BusPkg stSendPkg;
	BusPkgHead *pstHead;
	char *sData;
	int iSrc;
	int iSize;
	int iSide;
	LogFile stLog;
	int iAttach;
	AttachChannel astAttach[MAX_ATTACH_CHANNEL];
	int iLast;
} BusInterface;

typedef struct {
	int iGetLen;
	char sBuf[sizeof(BusPkg)];
} BusRecvBuf;

typedef struct {
	int iKey;
	int iUp;
	int iUpRelay;
	char sUpRelayType[20];
	int iUpSize;
	int iDown;
	int iDownRelay;
	char sDownRelayType[20];
	int iDownSize;
} ChannelConfig;

typedef struct {
	char sLogPath[80];
} BusLog;

typedef struct {
	ChannelConfig stChnCfg;
	BusLog stLog;
} BusConfig;

int BusGetLog(void);
int BusSetLog(int iLog);

int BusConnect(BusInterface *pstBus, char *sFile, int iProcID);
int BusConnectDir(BusInterface *pstBus, char *sDir, int iProcID, char* sSuffix);
int BusSend(BusInterface *pstBus, int iDst, char *sData, int iSize, short shFlag);
int BusForward(BusInterface *pstBus, int iDst, char *sData, int iSize, short shFlag);
int BusBackward(BusInterface *pstBus, char *sData, int iSize, short shFlag);
int BusDeliver(BusInterface *pstBus, int iDst, char *sData, int iSize, short shFlag);
int BusRecv(BusInterface *pstBus);
int GetBackDst(BusInterface *pstBus);

int BusRealRecvChannel(BusInterface *pstBus, int iChannel, char cType);

int GetChannelIdxByEntity( BusInterface *pstBus , int iDst) ;
int BusRecvSpec( BusInterface *pstBus , int iDst ) ;

unsigned short BusHeadLen(BusPkgHead* pstHead);

#ifdef __cplusplus
}
#endif

#endif
