
#ifndef _OG_CHANNEL_H
#define _OG_CHANNEL_H

#define CHANNEL_SIDE_UP	01
#define CHANNEL_SIDE_DOWN	02
#define CHANNEL_CREATE	04

#define ERR_CHANNEL_PKG	-1
#define ERR_CHANNEL_FULL	-2
#define ERR_CHANNEL_EMPTY	-3
#define ERR_CHANNEL_CONFUSE	-4
#define ERR_CHANNEL_AUTH	-5
#define ERR_CHANNEL_RECV	-6

#define CHANNEL_ZONE_CONN_UP_SIZE	5242880
#define CHANNEL_ZONE_CONN_DOWN_SIZE	5242880
#define CHANNEL_ZONE_CTRL_UP_SIZE	102400
#define CHANNEL_ZONE_CTRL_DOWN_SIZE	10240
#define CHANNEL_ZONE_DB_UP_SIZE	1048576
#define CHANNEL_ZONE_DB_DOWN_SIZE	1048576

typedef struct {
	int iSize;
	int iHead;
	int iTail;
	int iSeq;
	int iLog;
	int iGet;
	int iRecvSeq;
	char sLogFile[80];
	char cAuth;
	char cGStart;
	char cPStart;
	char sReserve[512 - sizeof(int)*7 - 83];
} ChannelVar;

typedef struct {
	int iUp;
	int iDown;
	int iUpRelay;
	int iDownRelay;
	ChannelVar stUp;
	ChannelVar stDown;
} ChannelHead;

typedef struct {
	int iSide;
	ChannelHead	*pstHead;
	char *sUp;
	char *sDown;
} Channel;

/*
typedef struct {
	ChannelHead	stHead;
	char sUp[CHANNEL_ZONE_CONN_UP_SIZE];
	char sDown[CHANNEL_ZONE_CONN_DOWN_SIZE];
} ChannelZoneConnect;

typedef struct {
	ChannelHead	stHead;
	char sUp[CHANNEL_ZONE_CTRL_UP_SIZE];
	char sDown[CHANNEL_ZONE_CTRL_DOWN_SIZE];
} ChannelZoneCtrl;

typedef struct {
	ChannelHead	stHead;
	char sUp[CHANNEL_ZONE_DB_UP_SIZE];
	char sDown[CHANNEL_ZONE_DB_DOWN_SIZE];
} ChannelZoneDB;

*/

ChannelVar *ChannelVarGet(Channel* pstChannel);
ChannelVar *ChannelVarPush(Channel* pstChannel);

int ChannelAuthPos(Channel* pstChannel, int* piPos);

#endif

