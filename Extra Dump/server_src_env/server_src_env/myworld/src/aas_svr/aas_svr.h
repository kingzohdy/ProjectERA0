#ifndef AAS_SVR_H
#define AAS_SVR_H


#define  MAJOR             1
#define  MINOR             1
#define  REV               0
#define  BUILD             20090113

#define OBJ_SHM_VER 0x0001

#define LOOP_NUM 100
#define MAX_MSG 64000*2

#define UNUSED( param ) ((void)(param))

#define MEMPOOL_SIZE(max, unit) sizeof(TMEMPOOL) - 1 + max*TMEMPOOL_ROUND_UNIT(unit + sizeof(TMEMBLOCK) - 1)

#define Z_SSHEAD_INIT(pstHead, unCmd) {(pstHead)->Cmd = unCmd;(pstHead)->Magic = PROTOCOL_MAGIC;}

//��������Ϣ
#define MAX_AAS_PLAYER 1000000			//�����Ե����ʺ�����
#define AAS_BUCKET MAX_AAS_PLAYER*3

#define GAME_INST_DATA 32
#define MAX_AAS_GAME_INST       MAX_AAS_PLAYER      

#define AAS_TIME_OUT 720	//������ʱ

#define AAS_OFFLINE_TIME 18000

typedef struct tagAasSvrInfo
{
	int SvrID;
	time_t tActive;
}AasSvrInfo;

typedef struct tagAasOnlineInst
{
	ListEntry stEntry;
	AasSvrInfo stSvrInfo;
	char sRes[GAME_INST_DATA - sizeof(ListEntry) - sizeof(AasSvrInfo)];
}AasOnlineInst;

typedef struct tagAasHash
{
	int iUin;
	int iInstList;			//������������½ʵ��
	time_t tOnline;
	time_t tOffline;
	time_t tActive;
}AasHash;

typedef struct tagZoneObj
{
	int iVersion;
	char sAasInst[MEMPOOL_SIZE(MAX_AAS_GAME_INST, GAME_INST_DATA)];
	char sAasSht[SHT_SIZE(AAS_BUCKET, MAX_AAS_PLAYER, sizeof(AasHash))];
}ZoneObj;

struct tagaas_svrEnv
{
	AAS_SVRCONF *pstConf;
	AAS_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	int iZoneObjShmID;
	ZoneObj *pstZoneObj;
	TMEMPOOL* pstAasPool; // �������ڴ��
	LPSHTABLE pstShtAas;
	int iTconnapiHandle;
	SSHANDLE  *pstSSHan;
	int iRegionID;

	//char sAasInst[MEMPOOL_SIZE(MAX_AAS_GAME_INST, GAME_INST_DATA)];
	//char sAasSht[SHT_SIZE(AAS_BUCKET, MAX_AAS_PLAYER, sizeof(AasHash))];
	
	char szRecvBuffer[MAX_MSG]; /*���������Ϣ������*/
	char szSendBuffer[MAX_MSG]; /*���淢����Ϣ������*/
	
};

typedef struct tagaas_svrEnv		AAS_SVRENV;
typedef struct tagaas_svrEnv		*LAAS_SVRENV;


#endif 
