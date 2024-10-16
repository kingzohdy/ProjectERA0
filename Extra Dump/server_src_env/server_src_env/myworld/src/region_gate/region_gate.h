#ifndef REGION_GATE_H
#define REGION_GATE_H

#define REGION_OBJ_SHM_VER  0x00001

#define MAX_MSG 64000*2

/* 用来存world的busid */
#define MAX_REGION_WORLD_NUM  1000

#define MAX_REGION_UIN_NUM  100000

#define UIN_ONLINE_BUCKET MAX_REGION_UIN_NUM*3
#define UIN_WORLD_INST_DATA  32
#define MAX_UIN_WORLD_INST MAX_REGION_UIN_NUM

#define UIN_ONLINE_TIME_OUT 720	//心跳超时

#define MEMPOOL_SIZE(max, unit) sizeof(TMEMPOOL) - 1 + max*TMEMPOOL_ROUND_UNIT(unit + sizeof(TMEMBLOCK) - 1)

#define Z_SSHEAD_INIT(pstHead, unCmd) {(pstHead)->Cmd = unCmd;(pstHead)->Magic = PROTOCOL_MAGIC;}

typedef enum tagUinOnlineStatus
{
	ZONE_SVR_LOGIN = 1,
	MUD_SVR_LOGIN,
}UINONLINESTATUS;

typedef struct tagWorldOnlineInfo
{
	int iSvrID;
	int fStatus;
	time_t tActive;
}WorldOnlineInfo;

typedef struct tagUinOnlineInst
{
	ListEntry stEntry;
	WorldOnlineInfo stSvrInfo;
	char sRes[UIN_WORLD_INST_DATA - sizeof(ListEntry) - sizeof(WorldOnlineInfo)];
}UinOnlineInst;

typedef struct tagUinOnlineHash
{
	int iUin;
	int iInstList;
}UinOnlineHash;

// 跨服战场相关数据结构定义:

// 一个zone_svr的一个战场副本的排队信息摘要
typedef struct tagPworldQueue
{
	int iZoneBusID;  // bus_id
	int iPlayerNum;  // 处于队列中的玩家人数
	time_t tInvalid; // 无效时间,在这个时间之前都处于无效状态
	time_t tAlive;   // 存活时间,在这个时间之前的队列都处于"存活"状态
}PworldQueue;

// 一个战场副本的状态信息
typedef struct tagPworldStat
{
	int iPworldDefID;  // 战场副本定义ID
	int iMaxPworldNum; // 最大副本数量
	int iMaxPlayerNum; // 战场副本定义的总人数
	int iCampNum;      // 战场副本的阵营数
	int iQueueNum;     // 处于这个战场副本的队列数量
	PworldQueue astRequestQueue[MAX_REGION_WORLD_NUM]; // 队列信息摘要
}PworldStat;

// 全局战场副本的状态信息
typedef struct tagGlobalPworlds
{
	int iPworldDefNum; // 战场副本定义数量
	PworldStat astPworldStat[MAX_BATTLE_KIND_OF_PWORLD_NUM]; // 状态信息
	int iItemSpace;
}GlobalPworlds;

// 一个战场服务器的状态信息
typedef struct tagBattleStat
{
	int iBattleBusID; // 战场服务器的bus_id
	int iPworldNum;   // 战场服务器当前副本数量
	int iPlayerNum;   // 战场服务器当前在线人数
	time_t tAlive;    // 存活时间,在这个时间之前的战场服务器都处于"存活"状态
}BattleStat;

// 全局战场服务器的状态信息
typedef struct tagGlobalBattles
{
	int iBattleNum; // 战场服务器数量
	BattleStat astBattleStat[MAX_BATTLE_SVR_NUM]; // 状态信息
}GlobalBattles;

typedef struct tagRegionObj
{
	int iVersion;
	char sUinOnlineInst[MEMPOOL_SIZE(MAX_UIN_WORLD_INST, UIN_WORLD_INST_DATA)];
	char sUinOnlineSht[SHT_SIZE(UIN_ONLINE_BUCKET, MAX_REGION_UIN_NUM, sizeof(UinOnlineHash))];
	char sGlobalPworlds[sizeof(GlobalPworlds)];
	char sGlobalBattles[sizeof(GlobalBattles)];
}RegionObj;

struct tagregion_gateEnv
{
	REGION_GATECONF *pstConf;
	REGION_GATERUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	RegionObj *pstObj;
	int iAasSvrID;
	int iInformerID;
	int iServiceID;
	int iBillID;
	int iZoneBusIDNum;
	int aiZoneBusID[MAX_REGION_WORLD_NUM];
	LPSSHANDLE pstSSHan;
	int iObjShmID;
	LPSHTABLE pstShtUin;
	TMEMPOOL* pstUinPool;
	GlobalPworlds *pstGlobalPworlds; // 全局战场排队状态
	GlobalBattles *pstGlobalBattles; // 全局战场服务器状态
	int iLastSrc; // 记录最后一次受到数据的bus_id
	char cClearShm;
	
	char szRecvBuffer[MAX_MSG]; /*保存接收消息缓冲区*/
	char szSendBuffer[MAX_MSG]; /*保存发送消息缓冲区*/
};

typedef struct tagregion_gateEnv		REGION_GATEENV;
typedef struct tagregion_gateEnv		*LREGION_GATEENV;

#endif/* REGION_GATE_H */

