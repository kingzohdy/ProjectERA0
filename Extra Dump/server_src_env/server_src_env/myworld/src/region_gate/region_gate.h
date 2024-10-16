#ifndef REGION_GATE_H
#define REGION_GATE_H

#define REGION_OBJ_SHM_VER  0x00001

#define MAX_MSG 64000*2

/* ������world��busid */
#define MAX_REGION_WORLD_NUM  1000

#define MAX_REGION_UIN_NUM  100000

#define UIN_ONLINE_BUCKET MAX_REGION_UIN_NUM*3
#define UIN_WORLD_INST_DATA  32
#define MAX_UIN_WORLD_INST MAX_REGION_UIN_NUM

#define UIN_ONLINE_TIME_OUT 720	//������ʱ

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

// ���ս��������ݽṹ����:

// һ��zone_svr��һ��ս���������Ŷ���ϢժҪ
typedef struct tagPworldQueue
{
	int iZoneBusID;  // bus_id
	int iPlayerNum;  // ���ڶ����е��������
	time_t tInvalid; // ��Чʱ��,�����ʱ��֮ǰ��������Ч״̬
	time_t tAlive;   // ���ʱ��,�����ʱ��֮ǰ�Ķ��ж�����"���"״̬
}PworldQueue;

// һ��ս��������״̬��Ϣ
typedef struct tagPworldStat
{
	int iPworldDefID;  // ս����������ID
	int iMaxPworldNum; // ��󸱱�����
	int iMaxPlayerNum; // ս�����������������
	int iCampNum;      // ս����������Ӫ��
	int iQueueNum;     // �������ս�������Ķ�������
	PworldQueue astRequestQueue[MAX_REGION_WORLD_NUM]; // ������ϢժҪ
}PworldStat;

// ȫ��ս��������״̬��Ϣ
typedef struct tagGlobalPworlds
{
	int iPworldDefNum; // ս��������������
	PworldStat astPworldStat[MAX_BATTLE_KIND_OF_PWORLD_NUM]; // ״̬��Ϣ
	int iItemSpace;
}GlobalPworlds;

// һ��ս����������״̬��Ϣ
typedef struct tagBattleStat
{
	int iBattleBusID; // ս����������bus_id
	int iPworldNum;   // ս����������ǰ��������
	int iPlayerNum;   // ս����������ǰ��������
	time_t tAlive;    // ���ʱ��,�����ʱ��֮ǰ��ս��������������"���"״̬
}BattleStat;

// ȫ��ս����������״̬��Ϣ
typedef struct tagGlobalBattles
{
	int iBattleNum; // ս������������
	BattleStat astBattleStat[MAX_BATTLE_SVR_NUM]; // ״̬��Ϣ
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
	GlobalPworlds *pstGlobalPworlds; // ȫ��ս���Ŷ�״̬
	GlobalBattles *pstGlobalBattles; // ȫ��ս��������״̬
	int iLastSrc; // ��¼���һ���ܵ����ݵ�bus_id
	char cClearShm;
	
	char szRecvBuffer[MAX_MSG]; /*���������Ϣ������*/
	char szSendBuffer[MAX_MSG]; /*���淢����Ϣ������*/
};

typedef struct tagregion_gateEnv		REGION_GATEENV;
typedef struct tagregion_gateEnv		*LREGION_GATEENV;

#endif/* REGION_GATE_H */

