#ifndef MUD_SVR_H
#define MUD_SVR_H

#include "mud_svr_conf_desc.h"
#include "pal/pal.h"
#include "comm_misc.h"
#include "comm/shtable.h"
#include "cs_net.h"
#include "tlog/tlog.h"
#include "comm/tmempool.h"
#include "tapp/tapp.h"
#include "tbus/tbus.h"
#include "tormapi/torm_protocol.h"
#include "tormapi/torm_protocol_api.h"
#include "tormapi/torm_error.h"
#include "entity.h"

#include "black_white.h"
#include "resdb_meta.h"
#include "og_script.h"
//#include "mud_module.h"
#include <math.h>
#include "op_log_meta.h"

#define MUD_PLAYER_SHM_MAGIC 0x10000000
#define MUD_SHUTDOWN_NOTIFY_TIMER 10

/*if player struct change, increase MUD_PLAYER_SHM_VER*/
#define MUD_PLAYER_SHM_VER 0x1000

#define MUD_OBJ_SHM_VER 0x1000


#define  MAJOR             1
#define  MINOR             1
#define  REV               0
#define  BUILD             20090113
#define MAX_MSG 64000*2
#define MAX_CS_CMD 1024

#define LOOP_NUM 100

#define RES_DIR "../cfg/res"

//在线人数
#define MAX_MUD_PLAYER 4120
#define MAX_MUD_PLAYER_SOFT 4100

#define MUD_PLAYER_ONLINE_DATA 1024
//#define MUD_PLAYER_ROLE_DATA 50000
#define MUD_PLAYER_ROLE_DATA 18400
#define MUD_PLAYER_SHADOW_ROLE_DATA 1024

#define TRUE 1
#define FALSE 0


typedef struct tagmud_svrEnv		MUD_SVRENV;
typedef struct tagmud_svrEnv		*LMUD_SVRENV;

//聊天房间 
typedef struct
{
	 int iPlayerNum;
	 int aiPlayerIdx[MAX_CHAT_ROOM_PLAYER];	 //pool Idx
}ChatRoomInfo;

typedef struct
{
	 ChatRoomInfo astChatRoom[MAX_MUD_CHAT_ROOM];	
}MudChatRoom;

#define CHAT_ROOM_DATA sizeof(MudChatRoom)

typedef enum tagMudPlayerAccStatus
{
	MUD_PLAYER_STATUS_ACC_NOLOGIN = 0,
	MUD_PLAYER_STATUS_ACC_LOGIN_REQ,
	MUD_PLAYER_STATUS_ACC_LOGIN,
	MUD_PLAYER_STATUS_ROLE_LIST_REQ,
	MUD_PLAYER_STATUS_ROLE_LIST,
	MUD_PLAYER_STATUS_ROLE_LOGIN_REQ,
	MUD_PLAYER_STATUS_ROLE_LOGIN,
	MUD_PLAYER_STATUS_ROLE_LOGOUT_REQ,	
	MUD_PLAYER_STATUS_ROLE_LOGOUT,
	MUD_PLAYER_STATUS_ACC_LOGOUT_REQ,
	MUD_PLAYER_STATUS_ACC_LOGOUT,
}MUDPLAYERACCSTATUS;

#define MISC_FLAG_RELATION_ALL 0x01

// 单链表实现
typedef struct tagSListEntry
{
	int iNext; // 下个结点的索引
	int iValue;   // 本结点的值
}SListEntry;

typedef struct tagPlayerIdxEntries
{
	int iHead; // 头结点的索引
	SListEntry astEntries[MAX_MUD_PLAYER];
}PlayerIdxEntries;



// 缓存标志
typedef enum tagMudCacheFlag
{
	CACHE_CLEAN, // 干净无需回写
	CACHE_IMM, // 立即回写
	CACHE_DIRTY, // 脏需要回写
	CACHE_INEXIST, // 不存在申请删除
}MUDCACHEFLAG;

typedef struct
{
	char chVer; 
	char chCmd;
	char chExtraType;                    
	char chTimeStampType;                
    int iID;                            
    int iConnIdx;                       
    TFRAMEHEADDATA stExtraInfo;                    
    TTIMESTAMPDATA stTimeStamp;                    
}MudConnectInfo;

typedef struct tagMudUinCache 
{
	int  iUin;	 /*uin of player*/
	int  iIdx;	/*index of player in memory pool*/
	int  iOnline;
	time_t tLastLogin;
	char sRes[2];
	char cGetRoleList;
	unsigned char ucRoleNum;
	ROLELISTINFO astRoleListInfo[ROLE_MAX_NUM];
}MudUinCache;


typedef struct
{
	char szName[ROLE_NAME_LEN];	
	int iIdx;
} Name2PlayerIdx;

// 公会成员缓存数据
typedef struct tagClanMemberCache
{
	int iClanID;      // 公会ID
	int iMemberNum;   // 现在的成员数量
	int iMemberEntry; // 成员链表入口
}ClanMemberCache;

//
// 精简版RoleData
//
typedef struct tagMudRoleData
{
	int iUin;
	tdr_ulonglong ullRoleID;
	char szRoleName[ROLE_NAME_LEN];
	int iPerm; //权限

	// 时间记录
	long tBlockEnd;
	long tSilenceEnd;
	long tCreateTime;
	long tLastLogin;
	long tLastLogout;

	// 外观属性
	int iGender;
	int iCareer;
	int iLevel;
	int iHead;
	int iFace;
	int iHair;
	ROLEAVATARPROF stAvatar; // 阿凡达

	// 所有物
	ROLEWEAR stWear; // 身上穿戴

	// 关系
	int iFriendNum;
	int iFriendMax;
	ROLEBUDDYMEMBER astFriends[MAX_BUDDY_FRIEND];
	int iBadGuyNum; // 黑名单坏人人数
	ROLEENEMYMEMBER astBadGuys[MAX_BLACK_NUM]; // 黑名单中的坏人
	CLANID stClanInfo; // 所属公会
	
	//属性
	CSPLAYERATTRVAL AttrVal;
	CSPLAYERATTRVAL BaseAttrVal;

	//心情
	char Emotion[EMOTION_LEN];            	/* 心情签名 */
	uint EmotionAlterTime;	

	int iZoneRemainExp;
	int RoleMiscFlag;                    	/*  位标志,装备是否显示问题 */
	int iTakeMudExp;	//zone获取的mud离线经验
	
	
}MudRoleData;

// 角色挂机专用信息
typedef struct tagMudShadowData
{
	int iCacheFlag;
	SHADOWACCDATA stCore;
}MudShadowData;

//角色在线信息
typedef struct {
	int State;
	long long llTimeErrAdjust;
	int iTimeAdjustNum;
	TTIMEVAL stConnectTime;
	time_t tChatCommCoolEnd;
	time_t tChatWorldCoolEnd;
	time_t tRoleLogin;
	time_t tRoleLogout;
	unsigned char bConnLost; //conn连接是否断开
	unsigned char bChatroom;	//房间 
	char sRes[2];
	int iSecCount;
	int iMiscFlag;
}MudPlayerOnline;

typedef struct tagMudPlayer
{
	int iMagic;
	MudPlayerOnline stOnline; 
	char sRes1[MUD_PLAYER_ONLINE_DATA - sizeof(MudPlayerOnline)];
	int iUinCachePos;
	int iIdx; /*idx in player mempool*/
	unsigned short unCltProVer;	/*the cs protocol version of client*/
	char eStatus;
	char sRes2;
	int iRoleLogoutReason;
	MudConnectInfo stConnectInfo;
	time_t tActive;
	char szAccount[ACCOUNT_LOGIN_NAME_LEN+5];
	int iAccSilenceEnd; // 帐号禁言结束时间
	int iCacheFlag; // 缓存标志
	MudRoleData stRoleData; // 精简版角色数据
	char sRes3[MUD_PLAYER_ROLE_DATA - sizeof(MudRoleData)];
	MudShadowData stShadowData; // 挂机专用数据
	char sRes4[MUD_PLAYER_SHADOW_ROLE_DATA - sizeof(MudShadowData)];
	int iZoneOfflineExp;//模拟计算的zone离线经验
	int iMaxOfflineExp;//最大可积累的离线经验
	int iChatSilenceFlag;//聊天频道屏蔽标记
	char szAddFriendName[NAME_LEN];
	int iBuddyDirty;//buddy信息需要回写
	char aMacAdd[MAC_ADDR_LEN];//mac 地址
	time_t tLastTickSec;
	int iOnlineMudExp;	//mud 在线获得的补偿经验
	int iAccMaxLevel; //帐号中最大的角色等级
	int iContributeExp;//最大等级对应的ContributeExp
	ACCSHARE stAccShare; //帐号共享的数据
	int iInvalidTime; //一段时间内无法获得离线经验
	
}MudPlayer;

#define MUD_PLAYER_DATA MUD_PLAYER_ONLINE_DATA+MUD_PLAYER_ROLE_DATA+MUD_PLAYER_SHADOW_ROLE_DATA+1024+512
char szWatchPlayer[MUD_PLAYER_DATA - sizeof(MudPlayer)];

#define MUD_UIN_CACHE MAX_MUD_PLAYER*10

typedef struct
{
	int iShmVersion;
	char sData[1];
}MudPlayerShm;

typedef int (*CmdHandle)(MUD_SVRENV*, MudPlayer*, TFRAMEHEAD*, CSPKG*);

typedef struct
{
	char cCtrlFlag;
	char cStateLimit;
	short nRes;
	CmdHandle pfnCmdHandle;
}MudCmdMap;


#define NAME2PLAYERIDX_BUCKET (MAX_MUD_PLAYER * 3)

#define CLAN_MEMBER_CACHE_BUCKET (CLAN_MAX_NUM_HARD * 3)

#define MAX_LEVEL 120
#define LEVELUP_DEF_DATA 384
char sWatchLevelUpData[LEVELUP_DEF_DATA-sizeof(LEVELUP)];

#define MAX_OFFLINE_EXP_PARAMETER 4
#define OFFLINE_EXP_PARAMETER_DATA 32
char sWatchOfflineExpParameterData[OFFLINE_EXP_PARAMETER_DATA - sizeof(OFFLINEEXPPARAMETER)];

typedef struct {
	char szLoginName[ACCOUNT_LOGIN_NAME_LEN+5];
	int iID;
} LoginName2MemID;

typedef struct tagMudObj
{
	int iVersion;

	//升级效果表
	time_t tLevelUpDef;
	int iLevelUpDef;
	char sLevelUpDef[MAX_LEVEL][LEVELUP_DEF_DATA];

	//聊天房间
	char sMudChatRoom[CHAT_ROOM_DATA];

	// Name -> Player Pool Index 哈希表
	char sName2PlayerIdxSht[SHT_SIZE(NAME2PLAYERIDX_BUCKET, MAX_MUD_PLAYER, sizeof(Name2PlayerIdx))];
	char sMudL2PSht[SHT_SIZE(NAME2PLAYERIDX_BUCKET, MAX_MUD_PLAYER, sizeof(LoginName2MemID))];

	// 公会成员缓存
	char sClanMemberCacheSht[SHT_SIZE(CLAN_MEMBER_CACHE_BUCKET, CLAN_MAX_NUM_HARD, sizeof(ClanMemberCache))];

	// Player Index的结点数据
	char sPlayerIdxEntries[sizeof(PlayerIdxEntries)];

	//离线补偿经验公式表
	time_t tOfflineExpParameter;
	int iOfflineExpParameter;
	char sOfflineExpParameter[MAX_OFFLINE_EXP_PARAMETER][OFFLINE_EXP_PARAMETER_DATA];

	int iClanNum;
	CLANID astClanID[CLAN_MAX_NUM_HARD];

}MudObj;

#define DIR_REPORT_TIMER 15

struct tagmud_svrEnv
{
	TAPPCTX *pstAppCtx;
	MUD_SVRCONF *pstConf;
	MUD_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	NETHANDLE *pstNetHan;
	TORMPROTOCOLHANDLE hTorm;
	MudObj *pstMudObj;
	int iRoleDBProtoVer;
	int iMiscDBProtoVer;
	int iMudConnID;
	int iRoleDBID;
	int iMiscDBID;
	int iRegionGateID;
	int iZoneSvrID;
	int iMudCtrlID;
	unsigned long int iTconnapiHandle;
	int iUinCacheWashoutPos;
	int iTormSeq;
	TMEMPOOL* pstPlayerPool;
	MudPlayerShm *pstPlayerShm;
	LPSHTABLE pstShtUinCache;
	LPSHTABLE pstShtName2PlayerIdx;
	LPSHTABLE pstMudShtL2P;
	LPSHTABLE pstShtClanMemberCache;
	PlayerIdxEntries *pstPlayerIdxEntries;
	int iUinCacheShmID;
	int iPlayerShmID;
	int iObjShmID;
	int iUpdateCount;
	int iUpdatePos;
	int iBenchCount;
	int iBenchPos;
	int iOpenGM;
	int iGetZoneGm;
	MudCmdMap astCmdMap[MAX_CS_CMD];
	
	SSHANDLE  *pstSSHan;
	int iDirSvrSock;
	int iDirBakSock;
	int iConnIP;
	unsigned short unPort;
	char cShutdown;
	unsigned char ucShutdownCount;
	TFRAMEHEAD stFrameHeadTemp;

	CSPKG stCSReq;
	CSPKG stCSRes;
	SSPKG stSSReq;
	SSPKG stSSRes;
	TORMPKG stOrmPkg;
	BlackWhiteHandle *pstBlackWhiteH;
	MudChatRoom *pstMudChatRoom;
	time_t tDirReport;
	time_t tShutdownStart;
	unsigned short   unWorldID; //根据id命名规则得到的worldid，参加entity.h
	unsigned char ucRegionID;
	char cClearShm;

	char szRecvBuffer[MAX_MSG]; /*保存接收消息缓冲区*/
	char szSendBuffer[MAX_MSG]; /*保存发送消息缓冲区*/

	//运营日志
	TLOGCATEGORYINST* pstOpLogCat;	
	TLOGCONF stOpLogConf;	
	TLOGCTX stOpLogCtx;	
	LPTDRMETA pstLogicLogMeta;
};

//
// 导出ENV结构
//
extern MUD_SVRENV gs_stEnv;

static inline LMUD_SVRENV this_env() { return &gs_stEnv; }

static inline const TFRAMEHEAD* last_frame()
{ return &gs_stEnv.stFrameHeadTemp; }

MudRoleData* role_to_mud(const ROLEDATA *pstFrom, MudRoleData *pstTo);

ROLEDATA* mud_to_role(const MudRoleData *pstFrom, ROLEDATA *pstTo);

// 公会成员缓存查找
const ClanMemberCache* clan_mem_cached_find(const CLANID *pstClanID);

// 加入公会成员缓存
int clan_mem_cached_add(const MudPlayer *pstPlayer);

// 删除公会成员缓存
int clan_mem_cached_del(const MudPlayer *pstPlayer);

// Env
#define THIS_ENV (this_env())

// 当前时间(秒)
#define CURR (this_env()->pstAppCtx->stCurr.tv_sec)

// 快速获取角色所在的房间
static inline ChatRoomInfo* fast_get_room(MudPlayer *pstPlayer)
{
	assert(pstPlayer->eStatus == MUD_PLAYER_STATUS_ROLE_LOGIN);
	return this_env()->pstMudChatRoom->astChatRoom +
		   (int)pstPlayer->stOnline.bChatroom;
}

//
// 一些有用的宏
//
#ifndef CCH
#define CCH(arr) (sizeof(arr)/sizeof(arr[0]))
#endif // CCH

#ifndef UNUSED
#define UNUSED(arg) ((void)arg)
#endif // UNUSED

int m_oplog_send(OP_LOG_META *pstOpLog);


#endif // MUD_SVR_H
