/*
**  @file RCSfile
**  general description of this module
**  Id
**  @author Author
**  @date Date
**  @version Revision
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "cs_svr_conf_desc.h"
#include "version.h"
#include "cs_net.h"
#include "comm_misc.h"
#include "entity.h"
#include "tbus/tbus.h"
#include "tconnapi.h"
#include "proto_ss.h"
#include "comm/tmempool.h"
#include "zone_svr.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "zone_db.h"
#include "resdb_meta.h"


#define LOOP_NUM 100
#define MAX_MSG 64000*2

#if defined(UNUSED)
#	undef UNUSED
#	define UNUSED(arg) ((void)(arg))
#endif

#define MAX_ROLE_BACK_REQ 5

#pragma pack(1)

typedef struct RoleBackRecord
{
	int iTime;
	ZONEROLEBACKREQ stRoleBackReq;
}BACKRECORD;

struct tagcs_svrEnv
{
	CS_SVRCONF *pstConf;
	CS_SVRRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	unsigned long int iTconnapiHandle;
	LPNETHANDLE pstCSHan;
	LPSSHANDLE pstSSHan;

	LPTDRMETALIB pstProtoLib;
	LPTDRMETA pstProtoMeta;
	TORMPROTOCOLHANDLE hTorm;

	int iZoneSvrID;
	int iRoleDBID;
	int iMiscDBID;
	int iZoneCtrlID;
	int iCsConnID;
	unsigned short   unWorldID; 

	ZoneObj *pstZoneObj;
	int iShmID;
	LPSHTABLE pstShtNameMemID; 
	unsigned short  unGidSeq;

	char bIsAttath;
	char szRecvBuffer[MAX_MSG];
	char szSendBuffer[MAX_MSG];
};


typedef struct tagcs_asyn
{
	int Cmd;                            
   	int ConnIdx;                        
    	TIPINFO IPInfo;   

	/*char RoleName[MAXROLENAME];       
    	tdr_ulonglong RoleID;                         
	int iType;
	int iTime;*/
	int iLen;
	char Data[512];

}CS_ASYN;

#pragma pack() 

typedef struct tagcs_svrEnv		CS_SVRENV;
typedef struct tagcs_svrEnv		*LCS_SVRENV;


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_cs_svr[];
extern unsigned char g_szMetalib_proto_ss[];

CS_SVRENV gs_stEnv;
static CS_SVRRUN_STATUS gs_stStat;

#define BLOCK_ROLE_DB_UPDATE_RES 					10000	
#define SILENCE_ROLE_DB_UPDATE_RES 					10001
#define SET_ROLE_BASEINFO_DB_UPDATE_RES 			10002
#define SET_ROLE_DETAIL_DB_UPDATE_RES 				10003
#define SET_ROLE_SKILL_DB_UPDATE_RES 				10004
#define SET_ROLE_FITTING_DB_UPDATE_RES 				10005
#define SET_ROLE_ITEM_DB_UPDATE_RES 				10006
#define SET_ROLE_DIATHESIS_DB_UPDATA_RES 			10007
#define SET_ROLE_TASK_DB_UPDATA_RES 				10008
#define CHG_ROLE_POS_UP_RES 							10009
#define SET_ROLE_RIDE_DB_UPDATE_RES 				10010
#define SET_ROLE_QIYUE_DB_UPDATE_RES 				10011
#define ADD_ROLE_QIYUE_DB_UPDATE_RES  				10012   
#define DEL_ROLE_QIYUE_DB_UPDATE_RES 				10013
#define MODIFY_FAIRY_DB_UPDATE_RES 				10014
#define SET_ROLE_PET_DB_UPDATE_RES 				10015
#define ADD_ROLE_DESIGNATION_DB_UPDATE_RES 				10016
#define DEL_ROLE_DESIGNATION_DB_UPDATE_RES 				10017
#define SET_ROLE_RESPACT_DB_UPDATE_RES 				10018
#define DEL_ROLE_RESPACT_DB_UPDATE_RES 				10019
#define SET_ROLE_PERM_DB_UPDATE_RES 				10020
#define SET_ROLE_PACKAGE_DB_UPDATE_RES 				10021
#define ROLE_DROP_RESTORE_ROLEDATA_RES 				10022
#define ROLE_DROP_RESTORE_SELECT_RES 				10023
#define ROLE_DROP_UPDATE_RES 				10024
#define ROLE_BACK_RECORD_UP_RES 				10025
#define ROLE_HATCH_PET_UPDATE_RES 				10026
#define SET_SAFELOCK_UP_RES						10027
#define SET_LIFT_SKILL_UP_RES						10028
#define SET_SECOND_PWD_UP_RES						10029



SKILLDEF*cs_find_skilldef(CS_SVRENV*pstEnv, int iSkillID, int iSkillLevel);
FITTINGSDEF*cs_find_fittingdef(CS_SVRENV*pstEnv, int iFittingID, int iFittingLevel);
DIATHESISDEF*cs_find_diathesisdef(CS_SVRENV*pstEnv, int iDiathesisID, int iDiathesisLevel);
ITEMDEF *cs_find_itemdef(CS_SVRENV*pstEnv, int iItemID);
int check_rolegrid(CS_SVRENV*pstEnv,ITEMDEF *pstItemDef,ZONESETROLEITEMLISTREQ *pstSetItemReq);

static TASKDEF *cs_find_taskdef(CS_SVRENV *pstEnv, int iTaskID);

tdr_ulonglong cs_get_gid(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv);

static void cs_change_wid_grids(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                             ROLEGRID *pstGrids, int iNum);

static int cs_sendto_clt_pkg(CS_SVRENV *pstEnv, CS_ASYN *pstAsyn, 
	                           SSPKG *pstSSPkg);

static int cs_sendto_clt_pkg_x(CS_SVRENV *pstEnv, int iConnIdx, 
	                             const TIPINFO *pstIPInfo, SSPKG *pstSSPkg);
int cs_send_to_clt(CS_SVRENV*pstEnv, char* pszBuff, int iBuff,int iConnIdx, 
	                 const TIPINFO *pIpInfo);
void cs_init_sshead(SSPKGHEAD*pstHead,int iCmd);

int err_res(CS_SVRENV* pstEnv,int iConnIdx, const TIPINFO *pIpInfo, int iErrReason, int iCmd);

void *cs_find_def(void* pBase, unsigned int uiKey, int iUnit, int iUnitNum, int *piIdx)
{
	void *pDef;
	int iIndex;
	int iEqu;

	if (piIdx && *piIdx >= 0 && *piIdx < iUnitNum)
	{
		pDef = pBase + (*piIdx)*iUnit;
		if (*((unsigned int *)pDef) == uiKey)
		{
			return pDef;
		}
	}

	iIndex = bsearch_int(&uiKey, pBase, iUnitNum, iUnit, &iEqu);
	if (iEqu)
	{
		if (piIdx)
		{
			*piIdx = iIndex;
		}
		
		return  (pBase + iIndex*iUnit);
	}
	
	return NULL;
}

int out_line_res(CS_SVRENV* pstEnv,int iConnIdx, const TIPINFO *pIpInfo)
{
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	
	SSPKG stSSPkg;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);

	stSSPkg.Body.CsCtrL.Cmd = OUTLINE_RES;

	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff,iConnIdx,pIpInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -1;
	}
	return 0;
}

// 通过契约ID查找契约定义
QIYUEDEF * cs_find_qiyuedef( CS_SVRENV* pstEnv, int iQiYueID , int *piIdx)
{
	return (QIYUEDEF *)cs_find_def(pstEnv->pstZoneObj->sQiYueDef, iQiYueID, QIYUE_DEF_DATA, 
									pstEnv->pstZoneObj->iQiYueDef, piIdx);
}

static inline void cs_change_wid_items(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                     ROLEITEM *pstItems, int iNum)
{
	int i = iNum;
	while(i--)
	{
		pstItems[i].WID = cs_get_gid(pstAppCtx, pstEnv);
	}
}

static inline void cs_change_wid_arms(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                    ROLEARM *pstArms, int iNum)
{
	int i = iNum;
	while(i--)
	{
		pstArms[i].WID = cs_get_gid(pstAppCtx, pstEnv);
	}
}

/*
static inline void cs_change_wid_pets(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                    ROLEPET *pstPets, int iNum)
{
	int i = iNum;
	while(i--)
	{
		pstPets[i].WID = cs_get_gid(pstAppCtx, pstEnv);
	}
}
*/

static void cs_change_wid_grids(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                             ROLEGRID *pstGrids, int iNum)
{
	int i = iNum;
	while(i--)
	{
		switch(pstGrids[i].Type)
		{
		case GRID_ITEM_ARM:
			pstGrids[i].GridData.RoleArm.WID = cs_get_gid(pstAppCtx, pstEnv);
			break;

		case GRID_ITEM_ITEM:
			pstGrids[i].GridData.RoleItem.WID = cs_get_gid(pstAppCtx, pstEnv);
			break;
		}
	}
}

tdr_ulonglong cs_get_gid(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv)
{
	tdr_ulonglong ullGid = 0;
	short iTmp = -1;
	
	memcpy(&ullGid, &pstAppCtx->stCurr.tv_sec, sizeof(int));
	memcpy( ((char *)(&ullGid)+sizeof(int)), &iTmp, sizeof(short)); 
	memcpy( ((char *)(&ullGid)+sizeof(ullGid)-sizeof(pstEnv->unGidSeq)), &pstEnv->unGidSeq, sizeof(pstEnv->unGidSeq)); 
	pstEnv->unGidSeq++;
	
	return ullGid;
}

int cs_attach_zoneobj(CS_SVRENV* pstEnv)
{
	int iShmID;
	
	iShmID		=	shmget(pstEnv->pstConf->iResShmKey, sizeof(ZoneObj), 0666);
	if( iShmID<0 )
		return -1;

	if (pstEnv->pstZoneObj)
	{
		shmdt(pstEnv->pstZoneObj);
	}
	
	pstEnv->iShmID = iShmID;
	pstEnv->pstZoneObj =	(ZoneObj *)shmat(iShmID, NULL, 0);
	if(!pstEnv->pstZoneObj)
	{
		return -1;
	}

	if (OBJ_SHM_VER != pstEnv->pstZoneObj->iVersion)
	{
		return -1;
	}

	pstEnv->pstShtNameMemID = sht_attach(pstEnv->pstZoneObj->sNameSht, sizeof(pstEnv->pstZoneObj->sNameSht),
											PLAYER_BUCKET, MAX_PLAYER, sizeof(NameMemID));
	if (NULL == pstEnv->pstShtNameMemID)
	{
		return -1;
	}
	
	return 0;
}


int cs_svr_init(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv)
{
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (CS_SVRCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (CS_SVRRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}

	if (sizeof(ZONEGETROLEITEMLISTRES) >= CS_ASYN_DATA)
	{
		printf("macro:CS_ASYN_DATA=%d  too little %d\n", CS_ASYN_DATA, sizeof(ZONEGETROLEITEMLISTRES));
		return -1;
	}

	//计算关联进程的bus id
	GET_WORLD_ENTITY_ID(pstEnv->iRoleDBID, pstAppCtx->iId, FUNC_ROLE_DB);
	GET_WORLD_ENTITY_ID(pstEnv->iMiscDBID, pstAppCtx->iId, FUNC_MISC_DB);
	GET_WORLD_ENTITY_ID(pstEnv->iZoneCtrlID, pstAppCtx->iId, FUNC_ZONE_CTRL);
	GET_WORLD_ENTITY_ID(pstEnv->iCsConnID, pstAppCtx->iId, FUNC_CS_CONN);
	GET_WORLD_ENTITY_ID(pstEnv->iZoneSvrID, pstAppCtx->iId, FUNC_ZONE_SVR);
	GET_WORLD_ID(pstEnv->unWorldID, pstAppCtx->iId);

	if (0 > tconnapi_initialize(pstAppCtx->pszGCIMKey, pstAppCtx->iBusinessID))
	{
		printf("tbus gcim key %s init fail\n", pstAppCtx->pszGCIMKey) ;
		return -1;
	}
	
	if (0 > tconnapi_create(pstAppCtx->iId, &pstEnv->iTconnapiHandle))
	{
		printf("tbus id %s init fail\n", pstAppCtx->pszId);
		return -1;
	}

	pstEnv->bIsAttath = 1;
	pstEnv->pstZoneObj = NULL;
	if(0 > cs_attach_zoneobj(pstEnv))
	{
		pstEnv->bIsAttath = 0;
		perror("error:\n");
		printf("cs_attach_player fail\n");
		return -1;
	}
	
	if (0 > ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan))
	{
		printf("ss_han_create fail\n");
		return -1;
	}

	if (0 > torm_create_handle_from_dir(&pstEnv->hTorm, "../cfg/db_meta"))
	{
		printf("torm_create_handle fail\n");
		return -1;
	}


	printf("cs_svr start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "cs_svr start");

	return 0;
}

int NameMemIDHashCmp( const void * pv1 , const void *pv2 )
{
	NameMemID *pstNameMemID1 = (NameMemID *)pv1;
	NameMemID *pstNameMemID2 = (NameMemID *)pv2;

	return strcmp(pstNameMemID1->szName, pstNameMemID2->szName);
	UNUSED(pv1);	//add by paraunused.pl
	UNUSED(pv2);	//add by paraunused.pl
}

unsigned int NameMemIDHashCode(const void* pvData)
{
	NameMemID *pstNameMemID = (NameMemID *)pvData;
	
	return sht_get_code(pstNameMemID->szName);
	UNUSED(pvData);	//add by paraunused.pl
}

void cs_init_sshead(SSPKGHEAD*pstHead,int iCmd)
{
	pstHead->Cmd = iCmd;
	pstHead->HeadLen = sizeof(SSPKGHEAD);
	pstHead->Magic = PROTOCOL_MAGIC;
	pstHead->Ver = TDR_METALIB_PROTO_SS_VERSION;
}

int check_role_isonline_by_name(CS_SVRENV *pstEnv, char*pszRoleName)
{
	NameMemID stNameMemID;
	NameMemID *pstNameMemID;

	if(!pstEnv->bIsAttath)
	{
		return -1;
	}
	
	STRNCPY(stNameMemID.szName, pszRoleName, sizeof(stNameMemID.szName));
	pstNameMemID = sht_find(pstEnv->pstShtNameMemID, &stNameMemID, 
		                      NameMemIDHashCmp, NameMemIDHashCode);
	if (pstNameMemID)
	{
		return 0;
	}
	return -1;
}

void cs_init_torm_head(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv, TORMPKGHEAD *pstHead, int iCmd, char *pszMetaName, int  iUin)
{
	UNUSED(pstEnv);

	pstHead->HeadComm.InstID = pstAppCtx->iId;
	pstHead->HeadComm.Magic = TORM_MAGIC;
	pstHead->HeadCmd.Cmd = iCmd;

	if (pszMetaName)
	{
		STRNCPY(pstHead->HeadCmd.MetaNameVer.MetaName, pszMetaName, sizeof(pstHead->HeadCmd.MetaNameVer.MetaName));
	}
	pstHead->HeadCmd.MetaNameVer.MetaVer=TDR_METALIB_ROLE_DB_META_VERSION;
	pstHead->HeadApp.AppCmd = 0;
	pstHead->HeadApp.Seq = 0;
	pstHead->HeadApp.ObjID = iUin;
	pstHead->AsynCallBackDataLen = 0;
	
}

int cs_torm_send(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv, TORMPKG *pstPkg, int iDstID)
{
	TDRDATA stData;
	int iSrc;
	int iDst;

	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if(0 != torm_pack_msg(pstEnv->hTorm, &stData, pstPkg, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_pack_msg error");
		return -1;
	}
	
	iSrc = pstAppCtx->iId;
	iDst = iDstID;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, stData.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}
	else
	{
		return 0;
	}
}

void cs_init_asyn(ASYNREQ *pstAsyn,int iCmd,int iConnIdx,TIPINFO *pstIpInfo,char*szBuff,int iLen)
{
	pstAsyn->Cmd = iCmd;

	if (iConnIdx)
	{
		pstAsyn->ConnIdx = iConnIdx;
	}

	if (pstIpInfo)
	{
		memcpy(&pstAsyn->IPInfo,pstIpInfo,sizeof(TIPINFO));
	}
	
	if (!szBuff)
	{
		pstAsyn->DataLen = 0;
	}
	else
	{
		pstAsyn->DataLen = iLen;
		memcpy(pstAsyn->Data,szBuff,iLen);
	}
}

void cs_init_csasyn(CS_ASYN *pstAsyn,int iCmd,int iConnIdx,TIPINFO *pstIpInfo,char*szBuff,int iLen)
{
	pstAsyn->Cmd = iCmd;
	pstAsyn->ConnIdx = iConnIdx;
	memcpy(&pstAsyn->IPInfo,pstIpInfo,sizeof(TIPINFO));

	pstAsyn->iLen = 0;
	if (szBuff)
	{
		pstAsyn->iLen = iLen;
		memcpy(pstAsyn->Data,szBuff,iLen);
	}
}

int cs_package_maxnum_by_type(ROLEDATA *pstRoleData, int iListType)
{
	int iRet = 0;
	ROLEPACKAGE *pstPak = &pstRoleData->Package;

	switch(iListType)
	{
		case LIST_TYPE_STASH:
			iRet = pstPak->StashPackage.MaxNum;
			break;
		case LIST_TYPE_SUNDRIES:
			iRet = pstPak->SundriesPackage.MaxNum;
			break;
		case LIST_TYPE_STUFF:
			iRet = pstPak->StuffPackage.MaxNum;
			break;
		case LIST_TYPE_TASK:
			iRet = pstPak->TaskPackage.MaxNum;
			break;
		default:
			return 0;
			break;
	}
	
	return iRet;
}

int cs_db_select_by_roledata(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;
	ROLEDATA stRoleData;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	CS_ASYN stCsAsyn;
	SSCSCTRL *pstCsCtrl = &pstPkgReq->Body.CsCtrL;

	memset(&stRoleData, 0, sizeof(stRoleData));
	memset(&stPkg,0,sizeof(TORMPKG));

	if(pstCsCtrl->Cmd == BLOCKPLAYER_REQ)
	{	
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.BlockPlayerReq,sizeof(pstCsCtrl->CsCmd.BlockPlayerReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.BlockPlayerReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == CHG_ROLE_POS_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.ChgRolePosReq,sizeof(pstCsCtrl->CsCmd.ChgRolePosReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.ChgRolePosReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SILENCE_ROLE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SilenceRoleReq,sizeof(pstCsCtrl->CsCmd.SilenceRoleReq));

		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SilenceRoleReq.RoleName, sizeof(stRoleData.RoleName));
	}

	else if(pstCsCtrl->Cmd == GET_ROLE_TASK_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleTaskReq,sizeof(pstCsCtrl->CsCmd.GetRoleTaskReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleTaskReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == DEL_ROLE_TASK_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.DelRoleTaskReq,sizeof(pstCsCtrl->CsCmd.DelRoleTaskReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.DelRoleTaskReq.RoleName, sizeof(stRoleData.RoleName));
	}
	
	else if(pstCsCtrl->Cmd == GET_ROLE_DETAIL_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleDetailReq,sizeof(pstCsCtrl->CsCmd.GetRoleDetailReq));

		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleDetailReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_DETAIL_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
				(char*)&pstCsCtrl->CsCmd.SetRoleDetailReq,sizeof(pstCsCtrl->CsCmd.SetRoleDetailReq));	
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRoleDetailReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == GET_ROLE_SKILLINFO_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleSkillInfoReq,sizeof(pstCsCtrl->CsCmd.GetRoleSkillInfoReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleSkillInfoReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_SKILLINFO_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRoleSkillInfoReq,sizeof(pstCsCtrl->CsCmd.SetRoleSkillInfoReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRoleSkillInfoReq.RoleName, sizeof(stRoleData.RoleName));
	}
	/*else if(pstCsCtrl->Cmd == GET_ROLE_FITTING_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleFittingsReq,sizeof(pstCsCtrl->CsCmd.GetRoleFittingsReq));
		
		stRoleData.RoleID = pstCsCtrl->CsCmd.GetRoleFittingsReq.RoleID;
		stRoleData.Uin = pstCsCtrl->CsCmd.GetRoleFittingsReq.Uin;
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_FITTING_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRoleFittingsReq,sizeof(pstCsCtrl->CsCmd.SetRoleFittingsReq));

		stRoleData.RoleID = pstCsCtrl->CsCmd.SetRoleFittingsReq.RoleID;
		stRoleData.RoleID = pstCsCtrl->CsCmd.SetRoleFittingsReq.Uin;
	}*/
	else if(pstCsCtrl->Cmd == GET_ROLE_ITEMLIST_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleItemListReq,sizeof(pstCsCtrl->CsCmd.GetRoleItemListReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleItemListReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_ITEMLIST_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRoleItemListReq,sizeof(pstCsCtrl->CsCmd.SetRoleItemListReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRoleItemListReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == GET_ROLE_DIATHESIS_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleItemListReq,sizeof(pstCsCtrl->CsCmd.GetRoleItemListReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleItemListReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_DIATHESIS_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRoleDiathesisReq,sizeof(pstCsCtrl->CsCmd.SetRoleDiathesisReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRoleDiathesisReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == GET_ROLE_RIDE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleRideInfoReq,sizeof(pstCsCtrl->CsCmd.GetRoleRideInfoReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleRideInfoReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_RIDE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRoleRideInfoReq,sizeof(pstCsCtrl->CsCmd.SetRoleRideInfoReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRoleRideInfoReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == GET_ROLE_QIYUELIST_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleQiYueListReq,sizeof(pstCsCtrl->CsCmd.GetRoleQiYueListReq));

		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleQiYueListReq.RoleName, sizeof(stRoleData.RoleName));
	}	
	else if(pstCsCtrl->Cmd == SET_ROLE_QIYUE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRoleQiYueReq,sizeof(pstCsCtrl->CsCmd.SetRoleQiYueReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRoleQiYueReq.RoleName, sizeof(stRoleData.RoleName));
	}	
	else if(pstCsCtrl->Cmd == ADD_ROLE_QIYUE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.AddRoleQiYueReq,sizeof(pstCsCtrl->CsCmd.AddRoleQiYueReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.AddRoleQiYueReq.RoleName, sizeof(stRoleData.RoleName));
	}	
	else if(pstCsCtrl->Cmd == DEL_ROLE_QIYUE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.DelRoleQiYueReq,sizeof(pstCsCtrl->CsCmd.DelRoleQiYueReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.DelRoleQiYueReq.RoleName, sizeof(stRoleData.RoleName));
	}	
	else if (pstCsCtrl->Cmd == MODIFY_ROLE_FAIRY_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRoleFairyReq,sizeof(pstCsCtrl->CsCmd.SetRoleFairyReq));

		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRoleFairyReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == GET_ROLE_PET_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRolePetInfoReq,sizeof(pstCsCtrl->CsCmd.GetRolePetInfoReq));

		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRolePetInfoReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_PET_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRolePetInfoReq,sizeof(pstCsCtrl->CsCmd.SetRolePetInfoReq));

		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRolePetInfoReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == ADD_ROLE_DESIGNATION_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		            		   &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.AddRoleDesignationReq),
					   sizeof(pstCsCtrl->CsCmd.AddRoleDesignationReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.AddRoleDesignationReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == DEL_ROLE_DESIGNATION_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		               &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.DelRoleDesignationReq),
					   sizeof(pstCsCtrl->CsCmd.DelRoleDesignationReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.DelRoleDesignationReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == GET_ROLE_DESIGNATION_LIST_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		               	   &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.GetRoleDesignationListReq),
					   sizeof(pstCsCtrl->CsCmd.GetRoleDesignationListReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleDesignationListReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_RESPACT_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		               	   &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.SetRoleRespactReq),
					   sizeof(pstCsCtrl->CsCmd.SetRoleRespactReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRoleRespactReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == DEL_ROLE_RESPACT_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		               	   &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.DelRoleRespactReq),
					   sizeof(pstCsCtrl->CsCmd.DelRoleRespactReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.DelRoleRespactReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == GET_ROLE_RESPACT_LIST_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		               	    &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.GetRoleRespactListReq),
					   sizeof(pstCsCtrl->CsCmd.GetRoleRespactListReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRoleRespactListReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if (pstCsCtrl->Cmd == SET_ROLE_PERM_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		               	   &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.SetPermReq),
					   sizeof(pstCsCtrl->CsCmd.SetPermReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetPermReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if (pstCsCtrl->Cmd == GET_ROLE_FRIEND_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		               	   &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.FriendReq),
					   sizeof(pstCsCtrl->CsCmd.FriendReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.FriendReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if (pstCsCtrl->Cmd == GET_ROLE_BLACK_REQ)
	{
		cs_init_csasyn(&stCsAsyn, pstCsCtrl->Cmd, pstFrameHead->ConnIdx,
		               	   &pstFrameHead->ExtraInfo.IPInfo,
					   (char*)(&pstCsCtrl->CsCmd.BlackReq),
					   sizeof(pstCsCtrl->CsCmd.BlackReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.BlackReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == GET_ROLE_PACKAGE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRolePackageReq,sizeof(pstCsCtrl->CsCmd.GetRolePackageReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetRolePackageReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_ROLE_PACKAGE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetRolePackageReq,sizeof(pstCsCtrl->CsCmd.SetRolePackageReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetRolePackageReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if( pstCsCtrl->Cmd == GET_ROLE_PACKAGE_ITEM_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetPackageItemInfoReq,sizeof(pstCsCtrl->CsCmd.GetPackageItemInfoReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetPackageItemInfoReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if( pstCsCtrl->Cmd == ROLE_DROP_RESTORE_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.RoleDropRestoreReq,sizeof(pstCsCtrl->CsCmd.RoleDropRestoreReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.RoleDropRestoreReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == ROLE_BACK_RECORD_REQ)//回档
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.RoleBackReq,sizeof(pstCsCtrl->CsCmd.RoleBackReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.RoleBackReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if( pstCsCtrl->Cmd == ROLE_HATCH_PET_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.RoleHatchPetReq,sizeof(pstCsCtrl->CsCmd.RoleHatchPetReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.RoleHatchPetReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if (pstCsCtrl->Cmd == GET_SAFELOCK_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetSafeLockReq,sizeof(pstCsCtrl->CsCmd.GetSafeLockReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetSafeLockReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_SAFELOCK_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetSafeLockReq,sizeof(pstCsCtrl->CsCmd.SetSafeLockReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetSafeLockReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if (pstCsCtrl->Cmd == GET_LIFT_SKILL_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetLiftSkillReq,sizeof(pstCsCtrl->CsCmd.GetLiftSkillReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.GetLiftSkillReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if (pstCsCtrl->Cmd == SET_LIFT_SKILL_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetLiftSkillReq,sizeof(pstCsCtrl->CsCmd.SetLiftSkillReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetLiftSkillReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else if(pstCsCtrl->Cmd == SET_SECOND_PWD_REQ)
	{
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.SetSecondPwdReq,sizeof(pstCsCtrl->CsCmd.SetSecondPwdReq));
		strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.SetSecondPwdReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else
	{
		return -1;
	}

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, "RoleData", -1);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,sizeof(pstSelectReq->OperInfo.WhereDef),
				"where RoleName='%s'", stRoleData.RoleName);
	
	stPkg.Head.AsynCallBackDataLen = sizeof(stCsAsyn);
	memcpy(stPkg.Head.AsynCallBackData, &stCsAsyn, stPkg.Head.AsynCallBackDataLen);
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleData;
	stMetaData.iBuff = sizeof(stRoleData);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, 0))
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrl->Cmd);
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail  cmd=%d", pstCsCtrl->Cmd);
		return -1;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, &stPkg, pstEnv->iRoleDBID))
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrl->Cmd);
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_torm_send fail  cmd=%d", pstCsCtrl->Cmd);
		return -1;
	}

	
	return 0;
}

int cs_db_select_role_drop(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;
	ROLEDROP stRoleDrop;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	CS_ASYN stCsAsyn;
	SSCSCTRL *pstCsCtrl = &pstPkgReq->Body.CsCtrL;

	memset(&stRoleDrop, 0, sizeof(stRoleDrop));
	memset(&stPkg,0,sizeof(TORMPKG));

	if(pstCsCtrl->Cmd == GET_ROLE_DROP_REQ)
	{	
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetRoleDropReq,sizeof(pstCsCtrl->CsCmd.GetRoleDropReq));
		strncpy(stRoleDrop.RoleName, pstCsCtrl->CsCmd.GetRoleDropReq.RoleName, sizeof(stRoleDrop.RoleName));
	}
	else
	{
		return -1;
	}

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, "RoleDrop", -1);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,sizeof(pstSelectReq->OperInfo.WhereDef),
				"where RoleName='%s'", stRoleDrop.RoleName);
	
	stPkg.Head.AsynCallBackDataLen = sizeof(stCsAsyn);
	memcpy(stPkg.Head.AsynCallBackData, &stCsAsyn, stPkg.Head.AsynCallBackDataLen);
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleDrop;
	stMetaData.iBuff = sizeof(stRoleDrop);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, 0))
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrl->Cmd);
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail  cmd=%d", pstCsCtrl->Cmd);
		return -1;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, &stPkg, pstEnv->iMiscDBID))
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrl->Cmd);
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_torm_send fail  cmd=%d", pstCsCtrl->Cmd);
		return -1;
	}

	return 0;
}

int cs_db_role_update(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv, ROLEDATA*pstRoleData,CS_ASYN*pstAsyn)
{
	
	TORMPKG stPkg;
	TORMUPDATEREQ *pstUpdateReq = &stPkg.Body.UpdateReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRoleData;
	stMetaData.iBuff = sizeof(ROLEDATA);

	//memset(&stPkg.Head, 0, sizeof(stPkg.Head));
	//memset(&stPkg.Body, 0, sizeof(stPkg.Body));
	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, "RoleData", pstRoleData->Uin);

	if(pstAsyn->Cmd == BLOCKPLAYER_REQ)
	{
		pstAsyn->Cmd = BLOCK_ROLE_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == SILENCE_ROLE_REQ)
	{
		pstAsyn->Cmd = SILENCE_ROLE_DB_UPDATE_RES;
	}
	
	else if(pstAsyn->Cmd == SET_ROLE_DETAIL_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_DETAIL_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == SET_ROLE_SKILLINFO_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_SKILL_DB_UPDATE_RES;
	}
	/*else if(pstAsyn->Cmd == SET_ROLE_FITTING_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_FITTING_DB_UPDATE_RES;
	}*/
	else if(pstAsyn->Cmd == SET_ROLE_ITEMLIST_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_ITEM_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == SET_ROLE_DIATHESIS_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_DIATHESIS_DB_UPDATA_RES;
	}
	else if (pstAsyn->Cmd == DEL_ROLE_TASK_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_TASK_DB_UPDATA_RES;
	}
	else if (pstAsyn->Cmd == CHG_ROLE_POS_REQ)
	{
		pstAsyn->Cmd = CHG_ROLE_POS_UP_RES;
	}
	else if(pstAsyn->Cmd == SET_ROLE_RIDE_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_RIDE_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == SET_ROLE_PACKAGE_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_PACKAGE_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == SET_ROLE_QIYUE_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_QIYUE_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == ADD_ROLE_QIYUE_REQ)
	{
		pstAsyn->Cmd = ADD_ROLE_QIYUE_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == DEL_ROLE_QIYUE_REQ)
	{
		pstAsyn->Cmd = DEL_ROLE_QIYUE_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == MODIFY_ROLE_FAIRY_REQ)
	{
		pstAsyn->Cmd = MODIFY_FAIRY_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == SET_ROLE_PET_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_PET_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == ADD_ROLE_DESIGNATION_REQ)
	{
		pstAsyn->Cmd = ADD_ROLE_DESIGNATION_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == DEL_ROLE_DESIGNATION_REQ)
	{
		pstAsyn->Cmd = DEL_ROLE_DESIGNATION_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == SET_ROLE_RESPACT_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_RESPACT_DB_UPDATE_RES;
	}
	else if(pstAsyn->Cmd == DEL_ROLE_RESPACT_REQ)
	{
		pstAsyn->Cmd = DEL_ROLE_RESPACT_DB_UPDATE_RES;
	}
	else if (pstAsyn->Cmd == SET_ROLE_PERM_REQ)
	{
		pstAsyn->Cmd = SET_ROLE_PERM_DB_UPDATE_RES;
	}
	else if (pstAsyn->Cmd == ROLE_DROP_RESTORE_REQ)
	{
		pstAsyn->Cmd =  ROLE_DROP_RESTORE_ROLEDATA_RES;
	}
	else if (pstAsyn->Cmd == ROLE_BACK_RECORD_REQ)
	{
		pstAsyn->Cmd =  ROLE_BACK_RECORD_UP_RES;
	}
	else if (pstAsyn->Cmd == ROLE_HATCH_PET_REQ)
	{
		pstAsyn->Cmd =  ROLE_HATCH_PET_UPDATE_RES;
	}
	else if (pstAsyn->Cmd == SET_SAFELOCK_REQ)
	{
		pstAsyn->Cmd =  SET_SAFELOCK_UP_RES;
	}
	else if (pstAsyn->Cmd == SET_LIFT_SKILL_REQ)
	{
		pstAsyn->Cmd =  SET_LIFT_SKILL_UP_RES;
	}
	else if(pstAsyn->Cmd == SET_SECOND_PWD_REQ)
	{
		pstAsyn->Cmd = SET_SECOND_PWD_UP_RES;
	}
	else
	{
		return -1;
	}
	
	stPkg.Head.AsynCallBackDataLen = sizeof(*pstAsyn);
	memcpy(stPkg.Head.AsynCallBackData, pstAsyn, stPkg.Head.AsynCallBackDataLen);
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;
		
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, &stPkg, pstEnv->iRoleDBID))
	{
		return -1;
	}
	
	return 0;
}

int cs_modify_role_data(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead)
{
	int iDstID;
	int iSrcID;
	TDRDATA stData;
	SSCSCTRL *pstCsCtrlReq;
	char stBuff[sizeof(SSPKG)];
	SSPKG stZoneReq;
	char szRoleName[32];
	ASYNREQ *pstAsyn;
	pstAsyn = &stZoneReq.Body.CsCtrL.CsCmd.ZoneReq;
	
	pstCsCtrlReq = &pstPkgReq->Body.CsCtrL;
	if(pstCsCtrlReq->Cmd == BLOCKPLAYER_REQ)
	{
		strcpy(szRoleName, pstCsCtrlReq->CsCmd.BlockPlayerReq.RoleName);
	}
	else if(pstCsCtrlReq->Cmd == SILENCE_ROLE_REQ)
	{
		strcpy(szRoleName, pstCsCtrlReq->CsCmd.SilenceRoleReq.RoleName);
	}
	else
	{
		return -1;
	}
	
	if(check_role_isonline_by_name(pstEnv, szRoleName) == 0)
	{
		stData.iBuff = sizeof(stBuff);
		stData.pszBuff = stBuff;

		cs_init_sshead(&stZoneReq.Head, pstPkgReq->Head.Cmd);
		
		stZoneReq.Body.CsCtrL.Cmd = ZONE_REQ;
		if(pstCsCtrlReq->Cmd == BLOCKPLAYER_REQ)
		{
			cs_init_asyn(pstAsyn, pstCsCtrlReq->Cmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
						(char*)(&pstCsCtrlReq->CsCmd.BlockPlayerReq), sizeof(ZONEBLOCKPLAYERREQ));

		}
		else if(pstCsCtrlReq->Cmd == SILENCE_ROLE_REQ)
		{
			cs_init_asyn(pstAsyn, pstCsCtrlReq->Cmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
						(char*)(&pstCsCtrlReq->CsCmd.SilenceRoleReq), sizeof(ZONESILENCEROLEREQ));
		}
		else
		{
			return -1;
		}

		if (0 > ss_pack(pstEnv->pstSSHan, &stData, &stZoneReq, 0))
		{
			err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrlReq->Cmd);
			tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
			return -2;
		}

		iDstID = pstEnv->iZoneSvrID;
		iSrcID  = pstAppCtx->iId;
		if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, stData.pszBuff, stData.iBuff, 0))
		{
			err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrlReq->Cmd);
			tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
			return -2;
		}
		
	}
	else		// 角色不在线
	{
		if(cs_db_select_by_roledata(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead) < 0)
		{
			return -2;
		}
	}
	return 0;
}

int cs_unblock_player(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead)
{
	//const SSCSCTRL *pstCsCtrl = &pstPkgReq->Body.CsCtrL;

	if(cs_db_select_by_roledata(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead) < 0)
	{
		return -1;
	}
	return 0;
}

int cs_send_to_clt(CS_SVRENV*pstEnv, char* pszBuff, int iBuff,int iConnIdx, 
	                 const TIPINFO *pIpInfo)
{
	TFRAMEHEAD stFrameHead;
	memset(&stFrameHead, 0,sizeof(TFRAMEHEAD));
	stFrameHead.Cmd = TFRAMEHEAD_CMD_STOP;
	stFrameHead.TimeStampType = 0;
	stFrameHead.ID = -1;
	stFrameHead.ConnIdx = iConnIdx;
	memcpy(&stFrameHead.ExtraInfo.IPInfo,pIpInfo,sizeof(TIPINFO));
	stFrameHead.ExtraType = TFRAMEHEAD_EXTRA_IP;
	
	if(tconnapi_send(pstEnv->iTconnapiHandle, pstEnv->iCsConnID, pszBuff, iBuff, &stFrameHead) != 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tconnapi_send fail");
		return -1;
	}
	return 0;
}

int cs_db_role_list(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv, SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead)
{
	CS_ASYN stAsyn;
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;
	ROLELISTINFO stRoleListInfo;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	int iUin = pstPkgReq->Body.CsCtrL.CsCmd.RoleListReq.Uin;
	SSCSCTRL *pstCsCtrl = &pstPkgReq->Body.CsCtrL;

	
	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, "RoleListInfo", iUin);
	cs_init_csasyn(&stAsyn, pstCsCtrl->Cmd,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
				(char*)&pstCsCtrl->CsCmd.RoleListReq, sizeof(ZONEROLELISTREQ));
	
	stPkg.Head.AsynCallBackDataLen = sizeof(stAsyn);
	memcpy(stPkg.Head.AsynCallBackData, &stAsyn, stPkg.Head.AsynCallBackDataLen);
	
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where Uin=%d", iUin);

	//因为Uin是分表因子，所以这个字段还是要赋值
	stRoleListInfo.Uin = iUin;
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleListInfo;
	stMetaData.iBuff = sizeof(stRoleListInfo);
	if (0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, 0))
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrl->Cmd);
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg err");
		return -1;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, &stPkg, pstEnv->iRoleDBID))
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrl->Cmd);
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_torm_send err");
		return -1;
	}
	return 0;
}

int cs_zone_svr_up_uin(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv, char * pszBuff, int iBuff, int iCmd)
{
	int iDstID;
	int iSrcID;
	SSPKG stZoneReq;
	ASYNREQ *pstAsyn;
	pstAsyn = &stZoneReq.Body.CsCtrL.CsCmd.ZoneReq;
	TDRDATA stData;
	char stBuff[sizeof(SSPKG)];
	

	stData.iBuff = sizeof(stBuff);
	stData.pszBuff = stBuff;

	cs_init_sshead(&stZoneReq.Head, CS_CTRL);
	
	stZoneReq.Body.CsCtrL.Cmd = ZONE_REQ;

	if (iCmd == SET_ROLE_DETAIL_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, 0, NULL,pszBuff, iBuff);
	}

	if (0 > ss_pack(pstEnv->pstSSHan, &stData, &stZoneReq, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	iDstID = pstEnv->iZoneSvrID;
	iSrcID  = pstAppCtx->iId;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, stData.pszBuff, stData.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail DstID=%d SrcID=%d Cmd=%d", iDstID, iSrcID, iCmd);
		return -1;
	}
	return 0;
}


int cs_zone_svr_asyn(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead,int iCmd)
{
	int iDstID;
	int iSrcID;
	TDRDATA stData;
	SSCSCTRL *pstCsCtrlReq;
	char stBuff[sizeof(SSPKG)];
	SSPKG stZoneReq;
	//char szRoleName[32];
	ASYNREQ *pstAsyn;
	pstAsyn = &stZoneReq.Body.CsCtrL.CsCmd.ZoneReq;
	
	pstCsCtrlReq = &pstPkgReq->Body.CsCtrL;

	stData.iBuff = sizeof(stBuff);
	stData.pszBuff = stBuff;

	cs_init_sshead(&stZoneReq.Head, CS_CTRL);
	
	stZoneReq.Body.CsCtrL.Cmd = ZONE_REQ;

	if(iCmd == GET_ROLE_DETAIL_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRoleDetailReq), sizeof(ZONEGETROLEDETAILREQ));

	}
	else if (iCmd == CHG_ROLE_POS_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.ChgRolePosReq), sizeof(ZONECHGROLEPOSREQ));
	}
	
	else if (iCmd == GET_ROLE_SKILLINFO_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRoleSkillInfoReq), sizeof(ZONEGETROLESKILLINFOREQ));
	}
	else if (iCmd == GET_ROLE_ITEMLIST_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRoleItemListReq), sizeof(ZONEGETROLEITEMLISTREQ));
	}
	else if (iCmd == BLOCKPLAYER_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.BlockPlayerReq), sizeof(ZONEBLOCKPLAYERREQ));
	}
	else if (iCmd == SILENCE_ROLE_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.SilenceRoleReq), sizeof(ZONESILENCEROLEREQ));
	}
	/*else if (iCmd == GET_ROLE_FITTING_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRoleFittingsReq), sizeof(ZONEGETROLEFITTINGSREQ));
	}*/
	else if (iCmd == GET_ROLE_DIATHESIS_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRoleDiathesisReq), sizeof(ZONEGETROLEDIATHESISREQ));
	}
	else if (iCmd == GET_ROLE_TASK_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRoleTaskReq), sizeof(ZONEGETROLETASKREQ));
	}
	else if (iCmd == CHG_ROLE_NAME_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.ChgRoleNameReq), sizeof(ZONECHGROLENAMEREQ));
	}
	else if (iCmd == GET_ROLE_RIDE_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRoleRideInfoReq), sizeof(ZONEGETROLERIDEINFOREQ));
	}
	else if( iCmd ==  GET_ROLE_QIYUELIST_REQ )
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRoleQiYueListReq), sizeof(GETROLEQIYUELISTREQ));
	}
	else if( iCmd ==  SET_ROLE_QIYUE_REQ )
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.SetRoleQiYueReq), sizeof(SETROLEQIYUEREQ));
	}
	else if( iCmd ==  ADD_ROLE_QIYUE_REQ )
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.AddRoleQiYueReq), sizeof(ADDROLEQIYUEREQ));
	}
	else if( iCmd ==  DEL_ROLE_QIYUE_REQ )
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.DelRoleQiYueReq), sizeof(DELROLEQIYUEREQ));
	}
	else if(iCmd == GET_ROLE_PET_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRolePetInfoReq), sizeof(ZONEGETROLEPETINFOREQ));
	}
	else if(iCmd == ADD_ROLE_DESIGNATION_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             &pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.AddRoleDesignationReq),
					 sizeof(pstCsCtrlReq->CsCmd.AddRoleDesignationReq));
	}
	else if(iCmd == DEL_ROLE_DESIGNATION_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             &pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.DelRoleDesignationReq),
					 sizeof(pstCsCtrlReq->CsCmd.DelRoleDesignationReq));
	}
	else if(iCmd == GET_ROLE_DESIGNATION_LIST_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             &pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.GetRoleDesignationListReq),
					 sizeof(pstCsCtrlReq->CsCmd.GetRoleDesignationListReq));
	}
	else if(iCmd == SET_ROLE_RESPACT_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             &pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.SetRoleRespactReq),
					 sizeof(pstCsCtrlReq->CsCmd.SetRoleRespactReq));
	}
	else if(iCmd == DEL_ROLE_RESPACT_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             		&pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.DelRoleRespactReq),
					 sizeof(pstCsCtrlReq->CsCmd.DelRoleRespactReq));
	}
	else if(iCmd == GET_ROLE_RESPACT_LIST_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		            		 &pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.GetRoleRespactListReq),
					 sizeof(pstCsCtrlReq->CsCmd.GetRoleRespactListReq));
	}

	else if(iCmd == SET_ROLE_LOGOUT_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             		&pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.KickOffRoleReq),
					 sizeof(pstCsCtrlReq->CsCmd.KickOffRoleReq));
	}
	else if (iCmd == DEL_ROLE_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             		&pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.DelRoleReq),
					 sizeof(pstCsCtrlReq->CsCmd.DelRoleReq));
	}
	else if (iCmd == GET_ROLE_FRIEND_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             		&pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.FriendReq),
					 sizeof(pstCsCtrlReq->CsCmd.FriendReq));
	}
	else if (iCmd == GET_ROLE_BLACK_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             		&pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.BlackReq),
					 sizeof(pstCsCtrlReq->CsCmd.BlackReq));
	}
	else if (iCmd == IP_LIMIT_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             		&pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.IPLimitReq),
					 sizeof(pstCsCtrlReq->CsCmd.IPLimitReq));
	}
	else if (iCmd == SET_ROLE_PERM_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             		&pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.SetPermReq),
					 sizeof(pstCsCtrlReq->CsCmd.SetPermReq));
	}
	else if (iCmd == SET_ACCOUNT_UIN_FLAG_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx,
		             		&pstFrameHead->ExtraInfo.IPInfo,
					 (char*)(&pstCsCtrlReq->CsCmd.SetAccountUinFlagReq),
					 sizeof(pstCsCtrlReq->CsCmd.SetAccountUinFlagReq));

	}
	else if (iCmd == GET_ROLE_PACKAGE_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetRolePackageReq), sizeof(ZONEGETROLEPACKAGEREQ));
	}
	else if( iCmd == GET_ROLE_PACKAGE_ITEM_REQ )
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetPackageItemInfoReq), sizeof(ZONEGETPACKAGEITEMINFOREQ));
	}
	else if (iCmd == GET_LIFT_SKILL_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.GetLiftSkillReq), sizeof(GETLIFTSKILLREQ));	
	}
	else if (iCmd == FORCE_MSG_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.ForceMsgReq), sizeof(FORCEMSGREQ));	
	}
	else if (iCmd == CTL_SHOP_CGI_REQ)
	{
		cs_init_asyn(pstAsyn, iCmd, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
					(char*)(&pstCsCtrlReq->CsCmd.ShopCgiReq), sizeof(SHOPCGIREQ));
	}
	else 
	{
		return -1;
	}

	if (0 > ss_pack(pstEnv->pstSSHan, &stData, &stZoneReq, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,iCmd);
		return -1;
	}

	iDstID = pstEnv->iZoneSvrID;
	iSrcID  = pstAppCtx->iId;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, stData.pszBuff, stData.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail DstID=%d SrcID=%d Cmd=%d", iDstID, iSrcID, iCmd);
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,iCmd);
		return -1;
	}
	return 0;
}

int cs_zone_svr_chg_pos(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv,int iMap,int iUin, const char* pszName)
{
	int iDstID;
	int iSrcID;
	TDRDATA stData;
	SSCSCTRL *pstCsCtrlReq;
	char stBuff[sizeof(SSPKG)];
	SSPKG stPkg;

	pstCsCtrlReq = &stPkg.Body.CsCtrL;

	stData.iBuff = sizeof(stBuff);
	stData.pszBuff = stBuff;

	cs_init_sshead(&stPkg.Head, CS_CTRL);
	stPkg.Body.CsCtrL.Cmd = CHG_POS_UP_CACHE_REQ;
	pstCsCtrlReq->CsCmd.ChgPosUpCacheReq.Map = iMap;
	pstCsCtrlReq->CsCmd.ChgPosUpCacheReq.Uin = iUin;
	STRNCPY(pstCsCtrlReq->CsCmd.ChgPosUpCacheReq.RoleName, 
		pszName, sizeof(pstCsCtrlReq->CsCmd.ChgPosUpCacheReq.RoleName));

	if (0 > ss_pack(pstEnv->pstSSHan, &stData, &stPkg, 0))
	{
		return -1;
	}

	iDstID = pstEnv->iZoneSvrID;
	iSrcID  = pstAppCtx->iId;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, stData.pszBuff, stData.iBuff, 0))
	{
		return -1;
	}
	return 0;
}


int err_res(CS_SVRENV* pstEnv,int iConnIdx, const TIPINFO *pIpInfo, int iErrReason, int iCmd)
{
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	
	SSPKG stSSPkg;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);

	stSSPkg.Body.CsCtrL.Cmd = ERR_RES;
	stSSPkg.Body.CsCtrL.CsCmd.ErrRes.ErrCmd = iCmd;
	stSSPkg.Body.CsCtrL.CsCmd.ErrRes.ErrReason = iErrReason;

	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff,iConnIdx,pIpInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -1;
	}
	return 0;
}

int cs_role_drop_info(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead, char*szRoleName)
{
	//SSCSCTRL *pstCsCtrlReq = &pstPkgReq->Body.CsCtrL;

	char bIsOnline = 0;

	// 不在线访问数据库,在线去zone_svr
	if (pstEnv->bIsAttath &&  check_role_isonline_by_name(pstEnv, szRoleName) == 0 )
	{
		bIsOnline = 1;
	}

	/*
	if (bIsOnline )
	{
		// 要求角色不在线
		if (	pstCsCtrlReq->Cmd == ROLE_DROP_BACK_REQ)
		{
			err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_ONLINE,pstCsCtrlReq->Cmd );
			return -2;
		}
	}
	*/
	
	if(cs_db_select_role_drop(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead) < 0)
	{
		return -1;
	}
	
	return 0;
}

int cs_role_info(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead, char*szRoleName)
{
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	SSCSCTRL *pstCsCtrlReq = &pstPkgReq->Body.CsCtrL;
	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;

	char bIsOnline = 0;

	// 不在线访问数据库,在线去zone_svr
	if (pstEnv->bIsAttath &&  check_role_isonline_by_name(pstEnv, szRoleName) == 0 )
	{
		bIsOnline = 1;
	}

	if (!bIsOnline )
	{
		if(cs_db_select_by_roledata(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead) < 0)
		{
			return -1;
		}
	}
	else
	{
		// 要求角色不在线
		if (	pstCsCtrlReq->Cmd == SET_ROLE_DETAIL_REQ ||
		     	pstCsCtrlReq->Cmd == SET_ROLE_SKILLINFO_REQ ||
		       pstCsCtrlReq->Cmd == SET_ROLE_DIATHESIS_REQ ||
		       pstCsCtrlReq->Cmd == SET_ROLE_ITEMLIST_REQ||
		       pstCsCtrlReq->Cmd == DEL_ROLE_TASK_REQ ||
		       pstCsCtrlReq->Cmd == SET_ROLE_RIDE_REQ ||
		       pstCsCtrlReq->Cmd == SET_ROLE_PACKAGE_REQ ||
		       pstCsCtrlReq->Cmd == SET_ROLE_PET_REQ ||
		       pstCsCtrlReq->Cmd == MODIFY_ROLE_FAIRY_REQ ||
		       pstCsCtrlReq->Cmd == ROLE_DROP_RESTORE_REQ ||
		       pstCsCtrlReq->Cmd == DEL_ROLE_REQ ||
		       pstCsCtrlReq->Cmd == ROLE_BACK_RECORD_REQ ||
		       pstCsCtrlReq->Cmd == ROLE_HATCH_PET_REQ ||
		       pstCsCtrlReq->Cmd == SET_SAFELOCK_REQ ||
		       pstCsCtrlReq->Cmd == GET_SAFELOCK_REQ ||
		       pstCsCtrlReq->Cmd == SET_LIFT_SKILL_REQ ||
		       pstCsCtrlReq->Cmd == SET_SECOND_PWD_REQ)
		{
			err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_ONLINE,pstCsCtrlReq->Cmd );
			return -2;
		}
		
		if (cs_zone_svr_asyn(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead,pstCsCtrlReq->Cmd) < 0)
		{
			return -1;
		}
	}

	return 0;
}

int cs_get_role_info_select_db(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv,SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;
	ROLEDATA stRoleData;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	ASYNREQ stAsyn;
	SSCSCTRL *pstCsCtrl = &pstPkgReq->Body.CsCtrL;
	
	memset(&stRoleData, 0, sizeof(stRoleData));
	memset(&stPkg,0,sizeof(TORMPKG));
	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, "RoleData", -1);

	cs_init_asyn(&stAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
					(char*)&pstCsCtrl->CsCmd.GetRoleDetailReq,sizeof(ZONEGETROLEDETAILREQ));
	
	stRoleData.RoleID = pstCsCtrl->CsCmd.GetRoleDetailReq.RoleID;

	stPkg.Head.AsynCallBackDataLen = sizeof(stAsyn);
	memcpy(stPkg.Head.AsynCallBackData, &stAsyn, stPkg.Head.AsynCallBackDataLen);
	
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;

	snprintf(pstSelectReq->OperInfo.WhereDef, sizeof(pstSelectReq->OperInfo.WhereDef),"where RoleID=%llu", stRoleData.RoleID);
	
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 1;

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleData;
	stMetaData.iBuff = sizeof(stRoleData);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, &stPkg, pstEnv->iRoleDBID))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_torm_send fail");
		return -1;
	}
	
	return 0;
}

static int cs_db_send_req(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                        TORMPKG *pstTormPkg, const void *pvData, int iDataLen,
	                        int iDst)
{
	int iRet = 0;
	TDRDATA stNetData;
	TDRDATA stMetaData;

	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pvData;
	stMetaData.iBuff = iDataLen;

	iRet = torm_make_req_msg(pstEnv->hTorm, &stNetData, pstTormPkg, &stMetaData, 0);
	if(iRet != 0)
	{
		//printf("torm_make_req_msg fail, reason : %s\n", torm_error_string(iRet));
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail, reason : %s\n",
			         torm_error_string(iRet));
		return -1;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, pstTormPkg, iDst))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_torm_send fail");
		return -1;
	}
	
	return 0;
}

static int cs_sendto_clt_pkg(CS_SVRENV *pstEnv, CS_ASYN *pstAsyn, SSPKG *pstSSPkg)
{
	TDRDATA stNet;
	//char szBuff[sizeof(SSPKG)];

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	
	if(ss_pack(pstEnv->pstSSHan, &stNet, pstSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff, pstAsyn->ConnIdx, 
		                &pstAsyn->IPInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -1;
	}
	return 0;
}

static int cs_sendto_zsvr_pkg(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                            SSPKG *pstSSPkg)
{
	int iDstID = 0, iSrcID = 0;
	TDRDATA stNet;

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	
	if (0 > ss_pack(pstEnv->pstSSHan, &stNet, pstSSPkg, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	iDstID = pstEnv->iZoneSvrID;
	iSrcID  = pstAppCtx->iId;
	if (0 > tbus_send(pstAppCtx->iBus, &iSrcID, &iDstID, stNet.pszBuff, stNet.iBuff, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
		return -1;
	}
	
	return 0;
}

static void cs_set_asyn(TORMPKGHEAD *pstTormHead, int iMatchedCmd, 
	                      TFRAMEHEAD *pstFH, const void *pvData, int iDataLen)
{
	ASYNREQ stAsyn;
	cs_init_asyn(&stAsyn, iMatchedCmd, pstFH->ConnIdx, 
		           &pstFH->ExtraInfo.IPInfo, (char *)pvData, iDataLen);

	pstTormHead->AsynCallBackDataLen += sizeof(stAsyn.Cmd);
	pstTormHead->AsynCallBackDataLen += sizeof(stAsyn.ConnIdx);
	pstTormHead->AsynCallBackDataLen += sizeof(stAsyn.IPInfo);
	if(pvData && iDataLen > 0)
	{
		pstTormHead->AsynCallBackDataLen += sizeof(stAsyn.DataLen);
		pstTormHead->AsynCallBackDataLen += iDataLen;
	}
	memcpy(pstTormHead->AsynCallBackData, &stAsyn, pstTormHead->AsynCallBackDataLen);
}

static int cs_z_update_rolelist_cache_req(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                                        int iUin)
{
	SSPKG stSSPkg;

	if(!pstEnv->bIsAttath)
	{
		return -1;
	}

	stSSPkg.Body.CsCtrL.CsCmd.UpdateRoleListCacheReq.Uin = iUin;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = UPDATE_ROLELIST_CACHE_REQ;
	return cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
}

// 获取角色的邮件列表
static int cs_db_role_maillist(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                             TFRAMEHEAD *pstFrameHead,
	                             const ZONEGETMAILLISTREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	MAILENTRY stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "MailEntry", -1);
	cs_set_asyn(&stPkg.Head, GET_ROLE_MAIL_LIST_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OffSet = pstReq->Index * MAIL_MAX_NUM;
	pstDBReq->Limit = MAIL_MAX_NUM;
	snprintf(pstDBReq->OperInfo.WhereDef, sizeof(pstDBReq->OperInfo.WhereDef),
		     "where Recv = \'%s\'", pstReq->RoleName);

	bzero(&stDummy, sizeof(stDummy));
	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, GET_ROLE_MAIL_LIST_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_role_maillist_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                 CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	int i = 0;
	SSPKG stSSPkg;
	ZONEGETMAILLISTRES *pstCtlRes = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleMailListRes;
	TORMSELECTRES *pstRes = &pstPkg->Body.SelectRes;
	MAILENTRY stDummy;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		goto error;
	}
	
	pstCtlRes->ResNum = pstRes->SelectResult.ResultNum;
	i = pstRes->SelectResult.ResultNum;
	while(i--)
	{
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stDummy, sizeof(stDummy)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
			goto error;
		}
		pstCtlRes->MailHeads[i] = stDummy.Mail.Head;
	}

final:
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_MAIL_LIST_RES;
	return cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);

error:
	pstCtlRes->ResNum = 0;
	goto final;
	return 0;
}

// 获取一封邮件的详细数据
static int cs_db_role_maildata(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                             TFRAMEHEAD *pstFrameHead, 
	                             const ZONEGETMAILREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	MAILENTRY stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		                "MailEntry", -1);
	cs_set_asyn(&stPkg.Head, GET_ROLE_MAIL_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->Limit = 1;
	pstDBReq->OffSet = 0;
	snprintf(pstDBReq->OperInfo.WhereDef,
		       sizeof(pstDBReq->OperInfo.WhereDef),
		       "where WID = %llu and Recv = \'%s\'",
		       pstReq->MailID,
		       pstReq->RoleName);

	bzero(&stDummy, sizeof(stDummy));
	stDummy.Mail.Head.WID = pstReq->MailID;

	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		                    pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,GET_ROLE_MAIL_REQ);
		return -1;
	}
	return 0;

}

static int cs_db_role_maildata_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                 CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	SSPKG stSSPkg;
	ZONEGETMAILRES *pstCtlRes = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleMailRes;
	TORMSELECTRES *pstRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		goto error;
	}
	
	pstCtlRes->Found = pstRes->SelectResult.ResultNum > 0 ? 1 : 0;
	if(pstCtlRes->Found)
	{
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&pstCtlRes->MailData, 
			                     sizeof(pstCtlRes->MailData)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
			goto error;
		}
	}

final:
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_MAIL_RES;
	return cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);

error:
	pstCtlRes->Found = 0;
	goto final;
	return 0;
}

// 恢复被删除的高级角色
static int cs_db_restore_role(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                          TFRAMEHEAD *pstFrameHead, int iUin,
	                          tdr_ulonglong llRoleID)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;
	ROLEDELINFO stDummy;

	bzero(&stDummy, sizeof(stDummy));
	stDummy.RoleID = llRoleID;
	stDummy.DelTime = 0;
	stDummy.CtrlFlag = iUin; //借个地方,下面要置0

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
		              "RoleDelInfo", -1);
	cs_set_asyn(&stPkg.Head, RESTORE_ROLE_REQ, pstFrameHead, &stDummy,
	            sizeof(stDummy));


	stDummy.CtrlFlag = 0; //借个地方,下面要置0

	
	
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 0;

	return cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
	                      pstEnv->iRoleDBID);
}

static int cs_reload_server_req(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                     SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead,
                                     const ZONERELOADSERVERREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLRELOADSERVERREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlReloadServerReq;
	
	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_RELOAD_SERVER_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);

	return iRet;

	UNUSED(pstPkgReq);
}

static int cs_db_will_restore_role(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                               TFRAMEHEAD *pstFrameHead, 
	                               const SSRESTOREROLEREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	ROLEDATA stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "RoleData", -1);
	cs_set_asyn(&stPkg.Head, RESTORE_ROLE_REQ | 0x80000000, pstFrameHead, NULL,
	            0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->Limit = 1;
	pstDBReq->OffSet = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	bzero(&stDummy, sizeof(stDummy));
	stDummy.RoleID = pstReq->RoleID;

	return cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		                  pstEnv->iRoleDBID);
}

static int cs_db_will_restore_role_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                   CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ROLEDATA stData;
	TFRAMEHEAD stDummy;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;
	SSRESTOREROLERES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.RestoreRoleRes;

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		goto fatal;
	}

	if(pstDBRes->SelectResult.ResultNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "nothing be selected");
		goto fatal;
	}

	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stData, sizeof(stData)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		goto fatal;
	}

	stDummy.ConnIdx = pstAsyn->ConnIdx;
	stDummy.ExtraInfo.IPInfo = pstAsyn->IPInfo;

	iRet = cs_db_restore_role(pstAppCtx, pstEnv, &stDummy, stData.Uin,
	                          stData.RoleID);
	pstRes->Succeeded = 1;

final:
	return iRet;

fatal:
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = RESTORE_ROLE_RES;
	pstRes->Succeeded = 0;
	iRet = cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);
	goto final;
	return 0;
}

static int cs_db_restore_role_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                              CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	SSPKG stSSPkg;

	UNUSED(pstAppCtx);
	
	if(TORM_ERR_IS_ERROR(pstPkg->Body.UpdateRes.ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		stSSPkg.Body.CsCtrL.CsCmd.RestoreRoleRes.Succeeded = 0;
	}
	else
	{
		stSSPkg.Body.CsCtrL.CsCmd.RestoreRoleRes.Succeeded = 1;
	}

	if(stSSPkg.Body.CsCtrL.CsCmd.RestoreRoleRes.Succeeded)
	{
		int iUin;
		TORMPKG stPkg;
		TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;
		NAMEIDTYPE stNameIDType;
		ROLEDELINFO stDummy;
		
		memcpy(&stDummy, pstAsyn->Data, sizeof(stDummy));
		iUin = stDummy.CtrlFlag;//
		
		cs_z_update_rolelist_cache_req(pstAppCtx, pstEnv, iUin);

		cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
		              "NameIDType", iUin);

		pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_NORET;
		pstDBReq->OperInfo.WhereDef[0] = 0;
		pstDBReq->DoInsert = 0;

		stNameIDType.GID = stDummy.RoleID;
		stNameIDType.NameType = 1;
		

		cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stNameIDType, sizeof(stNameIDType),
	                      pstEnv->iMiscDBID);
	}

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = RESTORE_ROLE_RES;
	return cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);
}

static int cs_db_del_role_mail(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                             TFRAMEHEAD *pstFrameHead,
	                             const ZONEDELMAILREQ *pstReq)
{
	TORMPKG stPkg;
	TORMDELETEREQ *pstDBReq = &stPkg.Body.DeleteReq;
	MAILENTRY stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_DELETE_REQ, 
		                "MailEntry", -1);
	cs_set_asyn(&stPkg.Head, DEL_ROLE_MAIL_REQ, pstFrameHead, NULL, 0);
	
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstDBReq->OperInfo.WhereDef,
		       sizeof(pstDBReq->OperInfo.WhereDef),
		       "where WID = %llu and Recv = \'%s\'",
		       pstReq->MailID,
		       pstReq->RoleName);

	bzero(&stDummy, sizeof(stDummy));
	stDummy.Mail.Head.WID = pstReq->MailID;
	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		                    pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,DEL_ROLE_MAIL_REQ);
		return -1;
	}
	return 0;

}

static int cs_db_del_role_mail_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                 CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	SSPKG stSSPkg;

	UNUSED(pstAppCtx);
	
	if(TORM_ERR_IS_ERROR(pstPkg->Body.DeleteRes.ErrNo))
	{
		stSSPkg.Body.CsCtrL.CsCmd.DelMailRes.Succeeded = 0;
	}
	else
	{
		stSSPkg.Body.CsCtrL.CsCmd.DelMailRes.Succeeded = 1;
	}

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = DEL_ROLE_MAIL_RES;
	return cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);
}

static int cs_z_del_role_mail_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
			                            const ZONECTLDELMAILRES *pstCtlRes)
{
	SSPKG stSSPkg;
	ZONEDELMAILRES *pstSSRes = &stSSPkg.Body.CsCtrL.CsCmd.DelMailRes;
	CS_ASYN stDummy;

	*pstSSRes = pstCtlRes->Core;
	if(!pstSSRes->Succeeded)
	{
		TFRAMEHEAD stFHDummy;

		stFHDummy.ConnIdx = pstCtlRes->Extra.ConnIdx;
		stFHDummy.ExtraInfo.IPInfo = pstCtlRes->Extra.IPInfo;
		
		return cs_db_del_role_mail(pstAppCtx, pstEnv, &stFHDummy, 
			                         &pstCtlRes->ReqCopy);
	}
	
	stDummy.ConnIdx = pstCtlRes->Extra.ConnIdx;
	stDummy.IPInfo = pstCtlRes->Extra.IPInfo;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = DEL_ROLE_MAIL_RES;
	return cs_sendto_clt_pkg(pstEnv, &stDummy, &stSSPkg);
}

// 删除一封邮件
static int cs_role_del_mail(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                          TFRAMEHEAD *pstFrameHead,
	                          const ZONEDELMAILREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLDELMAILREQ *pstSSReq = &stSSPkg.Body.CsCtrL.CsCmd.CtlDelMailReq;
	int iRet = 0;
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_del_role_mail(pstAppCtx, pstEnv, pstFrameHead, pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DEL_ROLE_MAIL_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	
out:
	return iRet;
}

static int cs_db_ins_role_mail(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                             TFRAMEHEAD *pstFrameHead,
	                             const ZONESENDMAILREQ *pstReq)
{
	int i = 0;
	TORMPKG stPkg;
	TORMINSERTREQ *pstDBReq = &stPkg.Body.InsertReq;
	MAILENTRY stDummy;
	MAILHEAD *pstHead = &stDummy.Mail.Head;
	MAILDETAIL *pstDetail = &stDummy.Mail.Detail;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_INSERT_REQ, 
		                "MailEntry", -1);
	cs_set_asyn(&stPkg.Head, SEND_MAIL_REQ, pstFrameHead, NULL, 0);
	
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	bzero(&stDummy, sizeof(stDummy));
	pstHead->WID = cs_get_gid(pstAppCtx, pstEnv);
	pstHead->Time = pstAppCtx->stCurr.tv_sec;
	pstHead->Flags = MAIL_FLAG_SYSTEM;
	STRNCPY(pstHead->Recv, pstReq->To, sizeof(pstHead->Recv));
	STRNCPY(pstHead->Send, pstReq->Send, sizeof(pstHead->Send));
	STRNCPY(pstHead->Title, pstReq->Title, sizeof(pstHead->Title));
	pstDetail->Gold = pstReq->Money;
	pstDetail->ItemNum = pstReq->ItemNum;
	STRNCPY(pstDetail->Text, pstReq->Text, sizeof(pstDetail->Text));
	i = pstReq->ItemNum;
	while(i--)
	{
		pstDetail->Items[i] = pstReq->Items[i];
		pstDetail->Items[i].GridData.RoleArm.WID = cs_get_gid(pstAppCtx, pstEnv);
	}

	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		                    pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,SEND_MAIL_REQ);
		return -1;
	}
	return 0;
	
}

static int cs_db_ins_role_mail_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                 CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	SSPKG stSSPkg;

	UNUSED(pstAppCtx);
	
	if(TORM_ERR_IS_ERROR(pstPkg->Body.InsertRes.ErrNo))
	{
		stSSPkg.Body.CsCtrL.CsCmd.SendMailRes.Succeeded = 0;
	}
	else
	{
		stSSPkg.Body.CsCtrL.CsCmd.SendMailRes.Succeeded = 1;
	}

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = SEND_MAIL_RES;
	return cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);
}

static int cs_update_role_for_copying(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                                    TFRAMEHEAD *pstFH, const ROLEDATA *pstData)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
		                "RoleData", pstData->Uin);
	cs_set_asyn(&stPkg.Head, COPY_ROLE_RES, pstFH, &pstData->Uin,
		          sizeof(pstData->Uin));
	
	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;
	pstDBReq->DoInsert = 0;

	return cs_db_send_req(pstAppCtx, pstEnv, &stPkg, pstData, sizeof(*pstData),
		                    pstEnv->iRoleDBID);
}

static int cs_db_update_role_for_copying_res(TAPPCTX *pstAppCtx, 
	                                           CS_SVRENV *pstEnv, 
	                                           CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	SSPKG stSSPkg;
	//const TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;
	UNUSED(pstAppCtx);
	
	if(TORM_ERR_IS_ERROR(pstPkg->Body.InsertRes.ErrNo))
	{
		stSSPkg.Body.CsCtrL.CsCmd.CopyRoleRes.Succeeded = 0;
	}
	else
	{
		stSSPkg.Body.CsCtrL.CsCmd.CopyRoleRes.Succeeded = 1;
	}

	if(stSSPkg.Body.CsCtrL.CsCmd.CopyRoleRes.Succeeded)
	{
		int iUin = 0;
		memcpy(&iUin, pstAsyn->Data, sizeof(iUin));
		cs_z_update_rolelist_cache_req(pstAppCtx, pstEnv, iUin);
	}

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = COPY_ROLE_RES;
	return cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);
}

static void cs_change_role_all_wid(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                 ROLEDATA *pstRoleData)
{
	cs_change_wid_items(pstAppCtx, pstEnv,
	                    pstRoleData->Package.AmmPackage.RoleItems,
		                pstRoleData->Package.AmmPackage.Num);
	cs_change_wid_grids(pstAppCtx, pstEnv, 
		                pstRoleData->Package.StashPackage.RoleGrids,
		                pstRoleData->Package.StashPackage.Num);
	cs_change_wid_items(pstAppCtx, pstEnv, 
		                pstRoleData->Package.StuffPackage.RoleItems,
		                pstRoleData->Package.StuffPackage.Num);
	cs_change_wid_grids(pstAppCtx, pstEnv, 
		                pstRoleData->Package.SundriesPackage.RoleGrids,
		                pstRoleData->Package.SundriesPackage.Num);
	cs_change_wid_items(pstAppCtx, pstEnv, 
		                pstRoleData->Package.TaskPackage.RoleItems,
		                pstRoleData->Package.TaskPackage.Num);
	cs_change_wid_arms(pstAppCtx, pstEnv, pstRoleData->Wear.RoleArms,
		               pstRoleData->Wear.ArmNum);
}

static int cs_db_select_role_for_copying_res(TAPPCTX *pstAppCtx, 
	                                           CS_SVRENV *pstEnv, 
	                                           CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	int iRet = 0;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;
	ZONECOPYROLEREQ *pstReq = (ZONECOPYROLEREQ *)pstAsyn->Data;
	SSPKG stSSPkg;
	ROLEDATA stData;
	TFRAMEHEAD stDummy;
	ZONECOPYROLERES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.CopyRoleRes;

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo) || pstDBRes->SelectResult.ResultNum <= 0)
	{
		pstRes->Succeeded = CS_ERR_SYS;
		goto fatal;
	}

	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stData, sizeof(stData)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		pstRes->Succeeded = CS_ERR_SYS;
		goto fatal;
	}

	stDummy.ConnIdx = pstAsyn->ConnIdx;
	stDummy.ExtraInfo.IPInfo = pstAsyn->IPInfo;

	stData.RoleID = pstReq->DstRoleID;
	STRNCPY(stData.RoleName, pstReq->DstRoleName, sizeof(stData.RoleName));
	cs_change_role_all_wid(pstAppCtx, pstEnv, &stData);
	
	iRet = cs_update_role_for_copying(pstAppCtx, pstEnv, &stDummy, &stData);
	if(iRet < 0)
	{
		pstRes->Succeeded = CS_ERR_SYS;
		goto fatal;
	}

	pstRes->Succeeded = CS_ERR_SUCCEEDED;

final:
	return iRet;

fatal:
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = COPY_ROLE_RES;
	iRet = cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);
	goto final;
	return 0;
}


static int cs_sendto_clt_pkg_x(CS_SVRENV *pstEnv, int iConnIdx, 
	                             const TIPINFO *pstIPInfo, SSPKG *pstSSPkg)
{
	TDRDATA stNet;

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	
	if(ss_pack(pstEnv->pstSSHan, &stNet, pstSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff, iConnIdx, pstIPInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -1;
	}
	return 0;
}

static int cs_z_send_role_mail_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                             const ZONECTLSENDMAILRES *pstCtlRes)
{
	SSPKG stSSPkg;
	ZONESENDMAILRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.SendMailRes;

	UNUSED(pstAppCtx);
	
	*pstRes = pstCtlRes->Core;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = SEND_MAIL_RES;
	return cs_sendto_clt_pkg_x(pstEnv, pstCtlRes->Extra.ConnIdx, 
		                         &pstCtlRes->Extra.IPInfo, &stSSPkg);
}

static int cs_z_block_account_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                            const ZONECTLBLOCKACCOUNTRES *pstCtlRes)
{
	SSPKG stSSPkg;
	ZONEBLOCKACCOUNTRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.BlockAccountRes;

	UNUSED(pstAppCtx);
	
	*pstRes = pstCtlRes->Core;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = BLOCK_ACCOUNT_RES;
	return cs_sendto_clt_pkg_x(pstEnv, pstCtlRes->Extra.ConnIdx, 
		                         &pstCtlRes->Extra.IPInfo, &stSSPkg);
}

static int cs_z_silence_account_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                              const ZONECTLSILENCEACCOUNTRES *pstCtlRes)
{
	SSPKG stSSPkg;
	ZONESILENCEACCOUNTRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.SilenceAccountRes;

	UNUSED(pstAppCtx);
	
	*pstRes = pstCtlRes->Core;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = SILENCE_ACCOUNT_RES;
	return cs_sendto_clt_pkg_x(pstEnv, pstCtlRes->Extra.ConnIdx, 
		                         &pstCtlRes->Extra.IPInfo, &stSSPkg);
}

static int cs_z_get_bulletin_list_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                         const ZONECTLGETBULLETINLISTRES *pstCtlRes)
{
	int i = 0;
	SSPKG stSSPkg;
	ZONEGETBULLETINLISTRES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.GetBulletinListRes;

	UNUSED(pstAppCtx);
	
	i = pstCtlRes->Core.Num;
	pstRes->Num = pstCtlRes->Core.Num;
	while(i--)
	{
		BULLETINENTRY *pstDst = &pstRes->BulletinArray[i].Core;
		const BULLETINENTRY *pstSrc = &pstCtlRes->Core.BulletinArray[i].Core;
		
		STRNCPY(pstDst->Text, pstSrc->Text, sizeof(pstDst->Text));
		pstDst->ID = pstSrc->ID;
		pstDst->RollInterval = pstSrc->RollInterval;

		pstRes->BulletinArray[i].ActiveTime = 
			pstCtlRes->Core.BulletinArray[i].ActiveTime;
		pstRes->BulletinArray[i].ActiveTimeStart = pstCtlRes->Core.BulletinArray[i].ActiveTimeStart;
	}

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = GET_BULLETIN_LIST_RES;
	return cs_sendto_clt_pkg_x(pstEnv, pstCtlRes->Extra.ConnIdx, 
		                       &pstCtlRes->Extra.IPInfo, &stSSPkg);
}

static int cs_z_add_bulletin_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                           const ZONECTLADDBULLETINRES *pstCtlRes)
{
	SSPKG stSSPkg;
	ZONEADDBULLETINRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.AddBulletinRes;

	UNUSED(pstAppCtx);

	*pstRes = pstCtlRes->Core;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = ADD_BULLETIN_RES;
	return cs_sendto_clt_pkg_x(pstEnv, pstCtlRes->Extra.ConnIdx, 
		                         &pstCtlRes->Extra.IPInfo, &stSSPkg);
}

static int cs_z_del_bulletin_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                           const ZONECTLDELBULLETINRES *pstCtlRes)
{
	SSPKG stSSPkg;
	ZONEDELBULLETINRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.DelBulletinRes;

	UNUSED(pstAppCtx);

	*pstRes = pstCtlRes->Core;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = DEL_BULLETIN_RES;
	return cs_sendto_clt_pkg_x(pstEnv, pstCtlRes->Extra.ConnIdx, 
		                         &pstCtlRes->Extra.IPInfo, &stSSPkg);
}

// 发送一封系统邮件给角色
static int cs_role_send_sys_mail(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
	                             TFRAMEHEAD *pstFrameHead,
	                             const ZONESENDMAILREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLSENDMAILREQ *pstSSReq = &stSSPkg.Body.CsCtrL.CsCmd.CtlSendMailReq;
	int iRet = 0;
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_ins_role_mail(pstAppCtx, pstEnv, pstFrameHead, pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_SEND_MAIL_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	
out:
	return iRet;
}

// 踢帐号下线
static int cs_block_account(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                            TFRAMEHEAD *pstFH, const ZONEBLOCKACCOUNTREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLBLOCKACCOUNTREQ *pstSSReq = &stSSPkg.Body.CsCtrL.CsCmd.CtlBlockAccountReq;
	ZONEBLOCKACCOUNTRES *pstSSRes = &stSSPkg.Body.CsCtrL.CsCmd.BlockAccountRes;
	int iRet = 0;
	
	if(!pstEnv->bIsAttath)
	{
		goto fatal;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFH->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFH->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_BLOCK_ACCOUNT_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	if(iRet < 0)
	{
		goto fatal;
	}

final:
	return iRet;
	
fatal:
	pstSSRes->Succeeded = 0;
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = BLOCK_ACCOUNT_RES;
	iRet = cs_sendto_clt_pkg_x(pstEnv, pstFH->ConnIdx, &pstFH->ExtraInfo.IPInfo,
		                         &stSSPkg);
	goto final;
	return 0;
}

// 禁言帐号
static int cs_silence_account(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                              TFRAMEHEAD *pstFH, 
                              const ZONESILENCEACCOUNTREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLSILENCEACCOUNTREQ *pstSSReq = &stSSPkg.Body.CsCtrL.CsCmd.CtlSilenceAccountReq;
	ZONESILENCEACCOUNTRES *pstSSRes = &stSSPkg.Body.CsCtrL.CsCmd.SilenceAccountRes;
	int iRet = 0;
	
	if(!pstEnv->bIsAttath)
	{
		goto fatal;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFH->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFH->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_SILENCE_ACCOUNT_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	if(iRet < 0)
	{
		goto fatal;
	}

final:
	return iRet;
	
fatal:
	pstSSRes->Succeeded = 0;
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = SILENCE_ACCOUNT_RES;
	iRet = cs_sendto_clt_pkg_x(pstEnv, pstFH->ConnIdx, &pstFH->ExtraInfo.IPInfo,
		                         &stSSPkg);
	goto final;
	return 0;
}

// 复制角色
static int cs_select_role_for_copying(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
                                      TFRAMEHEAD *pstFH, 
                                      const ZONECOPYROLEREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	ROLEDATA stDummy;
	SSPKG stSSPkg;
	ZONECOPYROLERES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.CopyRoleRes;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "RoleData", -1);
	cs_set_asyn(&stPkg.Head, COPY_ROLE_REQ, pstFH, pstReq, sizeof(*pstReq));

	// 双方都必须不在线
	if(!check_role_isonline_by_name(pstEnv, (char *)pstReq->DstRoleName) ||
	   !check_role_isonline_by_name(pstEnv, (char *)pstReq->SrcRoleName))
	{
		goto fatal;
	}

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->Limit = 1;
	pstDBReq->OffSet = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	bzero(&stDummy, sizeof(stDummy));
	stDummy.RoleID = pstReq->SrcRoleID;

	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		                    pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFH->ConnIdx, &pstFH->ExtraInfo.IPInfo, ERR_TDR,COPY_ROLE_REQ);
		return -1;
	}
	return 0;

fatal:
	pstRes->Succeeded = CS_ERR_ONLINE;
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = COPY_ROLE_RES;
	cs_sendto_clt_pkg(pstEnv, (CS_ASYN *)stPkg.Head.AsynCallBackData, &stSSPkg);
	return -1;
}

// 获取公告列表
static int cs_get_bulletin_list(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
                                TFRAMEHEAD *pstFH, 
                                const ZONEGETBULLETINLISTREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLGETBULLETINLISTREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlGetBulletinListReq;
	ZONEGETBULLETINLISTRES *pstSSRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.GetBulletinListRes;
	int iRet = 0;
	
	if(!pstEnv->bIsAttath)
	{
		goto fatal;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFH->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFH->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_GET_BULLETIN_LIST_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	if(iRet < 0)
	{
		goto fatal;
	}

final:
	return iRet;
	
fatal:
	pstSSRes->Num = 0;
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = GET_BULLETIN_LIST_RES;
	iRet = cs_sendto_clt_pkg_x(pstEnv, pstFH->ConnIdx, &pstFH->ExtraInfo.IPInfo,
		                         &stSSPkg);
	goto final;
	return 0;
}

// 添加一条公告
static int cs_add_bulletin(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
                           TFRAMEHEAD *pstFH, 
                           const ZONEADDBULLETINREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLADDBULLETINREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlAddBulletinReq;
	ZONEADDBULLETINRES *pstSSRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.AddBulletinRes;
	int iRet = 0;
	
	if(!pstEnv->bIsAttath)
	{
		goto fatal;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFH->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFH->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_ADD_BULLETIN_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	if(iRet < 0)
	{
		goto fatal;
	}

final:
	return iRet;
	
fatal:
	pstSSRes->Succeeded = 0;
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = ADD_BULLETIN_RES;
	iRet = cs_sendto_clt_pkg_x(pstEnv, pstFH->ConnIdx, &pstFH->ExtraInfo.IPInfo,
		                         &stSSPkg);
	goto final;
	return 0;
}

// 删除一条公告
static int cs_del_bulletin(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
                           TFRAMEHEAD *pstFH, 
                           const ZONEDELBULLETINREQ *pstReq)
{
	SSPKG stSSPkg;
	ZONECTLDELBULLETINREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlDelBulletinReq;
	ZONEDELBULLETINRES *pstSSRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.DelBulletinRes;
	int iRet = 0;
	
	if(!pstEnv->bIsAttath)
	{
		goto fatal;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFH->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFH->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DEL_BULLETIN_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	if(iRet < 0)
	{
		goto fatal;
	}

final:
	return iRet;
	
fatal:
	pstSSRes->Succeeded = 0;
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = DEL_BULLETIN_RES;
	iRet = cs_sendto_clt_pkg_x(pstEnv, pstFH->ConnIdx, &pstFH->ExtraInfo.IPInfo,
		                         &stSSPkg);
	goto final;
	return 0;
}

static int cs_create_role_item(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
                               TFRAMEHEAD *pstFH, 
                               const ZONECREATEITEMREQ *pstReq)
{
	((void)(pstAppCtx));
	((void)(pstEnv));
	((void)(pstFH));
	((void)(pstReq));
	return -1;
}

int cs_qiyue_req(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead, char*szRoleName)
{
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	SSCSCTRL *pstCsCtrlReq = &pstPkgReq->Body.CsCtrL;
	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;

	char bIsOnline = 0;

	// 不在线访问数据库,在线去zone_svr
	if (pstEnv->bIsAttath &&  check_role_isonline_by_name(pstEnv, szRoleName) == 0 )
	{
		bIsOnline = 1;
	}

	if (!bIsOnline)
	{
		if(cs_db_select_by_roledata(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "cs_db_select_by_roledata fail");
			return -1;
		}
	}
	else
	{
		if (   pstCsCtrlReq->Cmd == SET_ROLE_QIYUE_REQ ||
		       pstCsCtrlReq->Cmd == ADD_ROLE_QIYUE_REQ ||
		       pstCsCtrlReq->Cmd == DEL_ROLE_QIYUE_REQ )
		{
			err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_ONLINE,pstCsCtrlReq->Cmd);
			//out_line_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo);
			return -2;
		}
		
		return cs_zone_svr_asyn(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead,pstCsCtrlReq->Cmd);
	}

	return 0;
}

int cs_kick_off	(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead)
{
	SSCSCTRL *pstCsCtrlReq;
	pstCsCtrlReq = &pstPkgReq->Body.CsCtrL;

	// 在线检查
	if (check_role_isonline_by_name(pstEnv, pstCsCtrlReq->CsCmd.KickOffRoleReq.RoleName) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_NO,pstCsCtrlReq->Cmd);
		return -1;
	}

	if (pstEnv->bIsAttath)
	{
		return cs_zone_svr_asyn(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead,pstCsCtrlReq->Cmd);
	}
	else
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_SYS,pstCsCtrlReq->Cmd);
		return -1;
	}

	

	return 0;
}


// 发往zone_svr 操作
int cs_del_role(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead)
{
	SSCSCTRL *pstCsCtrlReq;
	pstCsCtrlReq = &pstPkgReq->Body.CsCtrL;

	(void)pstAppCtx;

	// 在线检查
	if (check_role_isonline_by_name(pstEnv, pstCsCtrlReq->CsCmd.KickOffRoleReq.RoleName) == 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_ONLINE,pstCsCtrlReq->Cmd);
		//out_line_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo);
		return -1;
	}

	return 0;
}

int cs_del_role_req(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead, char*szRoleName)
{
	SSCSCTRL *pstCsCtrlReq = &pstPkgReq->Body.CsCtrL;
	char bIsOnline = 0;

	if (pstEnv->bIsAttath &&  check_role_isonline_by_name(pstEnv, szRoleName) == 0 )
	{
		bIsOnline = 1;
	}

	if (bIsOnline)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_ONLINE,pstCsCtrlReq->Cmd);
		return -2;
	}
	else
	{
		return cs_zone_svr_asyn(pstAppCtx,pstEnv,pstPkgReq,pstFrameHead,pstCsCtrlReq->Cmd);
	}

}

static int cs_db_clan_all_info(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                               TFRAMEHEAD *pstFrameHead,
                               const ZONEGETCLANALLINFOREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	MAILENTRY stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, GET_CLAN_ALL_INFO_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 1;
	snprintf(pstDBReq->OperInfo.WhereDef, sizeof(pstDBReq->OperInfo.WhereDef),
		     "WHERE Name = \'%s\'", pstReq->ClanName);

	bzero(&stDummy, sizeof(stDummy));
	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, GET_CLAN_ALL_INFO_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_clan_all_info_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                               CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	SSPKG stSSPkg;
	ZONEGETCLANALLINFORES *pstRes = 
		&stSSPkg.Body.CsCtrL.CsCmd.GetClanAllInfoRes;
	TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;
	CLAN stDummy;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		goto error;
	}

	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stDummy, sizeof(stDummy))
	   < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		goto error;
	}

	pstRes->ClanActiveTime  = stDummy.ClanActiveTime;
	pstRes->ClanBrief       = stDummy.ClanBrief;
	pstRes->ClanCity        = stDummy.ClanCity;
	pstRes->ClanCore        = stDummy.ClanCore;
	pstRes->ClanDestroyTime = stDummy.ClanDestroyTime;
	pstRes->ClanInner       = stDummy.ClanInner;
	pstRes->ClanProf        = stDummy.ClanProf;
	pstRes->Succeeded       = 1;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = GET_CLAN_ALL_INFO_RES;
	return cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);

error:
	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
	        GET_CLAN_ALL_INFO_REQ);
	return 0;
}

static int cs_z_clan_all_info_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                      const ZONECTLGETCLANALLINFORES *pstCtlRes)
{
	SSPKG stSSPkg;

	UNUSED(pstAppCtx);

	if(!pstCtlRes->Core.Succeeded)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "can not find clan[%s]",
		           pstCtlRes->Core.ClanProf.Name);

		err_res(pstEnv, pstCtlRes->Extra.ConnIdx, &pstCtlRes->Extra.IPInfo,
		        ERR_SYS, GET_CLAN_ALL_INFO_RES);
		return -1;
	}

	stSSPkg.Body.CsCtrL.CsCmd.GetClanAllInfoRes = pstCtlRes->Core;

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = GET_CLAN_ALL_INFO_RES;
	return cs_sendto_clt_pkg_x(pstEnv, pstCtlRes->Extra.ConnIdx,
	                           &pstCtlRes->Extra.IPInfo, &stSSPkg);
}

static int cs_get_clan_all_info(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead,
                                const ZONEGETCLANALLINFOREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLGETCLANALLINFOREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlGetClanAllInfoReq;

	UNUSED(pstPkgReq);
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_clan_all_info(pstAppCtx, pstEnv, pstFrameHead, pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_GET_CLAN_ALL_INFO_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	
out:
	return iRet;
}

static int cs_db_del_clan_member(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                 TFRAMEHEAD *pstFrameHead, CLAN *pstClan,
                                 const char *pszRoleName)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;

	unsigned int i = 0, j = 0;

	if(strcmp(pszRoleName, pstClan->ClanBrief.Leader.RoleName) == 0)
	{
		// 不能删除会长
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "can not delete leader[%s] from clan[%s]",
		           pszRoleName, pstClan->ClanProf.Name);
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_ARGS, DEL_CLAN_MEMBER_REQ);
		return -1;
	}
	
	for(i = 0; i < pstClan->ClanCore.MemNum; ++i)
	{
		if(strcmp(pszRoleName,
		          pstClan->ClanCore.ClanMem[i].RoleMember.RoleName) == 0)
		{
			break;
		}
	}
	if(i >= pstClan->ClanCore.MemNum)
	{
		// 没有找到指定的成员
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "can not find any member in clan[%s]",
		           pstClan->ClanProf.Name);
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_ARGS, DEL_CLAN_MEMBER_REQ);
		return -1;
	}
	if(i != pstClan->ClanCore.MemNum - 1)
	{
		// 因为公会成员是有序排列的,因此删除后也要保持有序
		memmove(pstClan->ClanCore.ClanMem + i,
		        pstClan->ClanCore.ClanMem + i + 1,
		        sizeof(pstClan->ClanCore.ClanMem[0]) *
		        (pstClan->ClanCore.MemNum - i - 1));
	}

	// 更新副会长信息
	j = pstClan->ClanBrief.AssistNum;
	while(j--)
	{
		if(strcmp(pszRoleName,
		          pstClan->ClanBrief.Assists[j].RoleName) == 0)
		{
			if(j != (unsigned)(pstClan->ClanBrief.AssistNum - 1))
			{
				memcpy(pstClan->ClanBrief.Assists + j,
				       pstClan->ClanBrief.Assists +
				       pstClan->ClanBrief.AssistNum - 1,
				       sizeof(pstClan->ClanBrief.Assists[0]));
			}

			--pstClan->ClanBrief.AssistNum;
			break;
		}
	}

	// 更新官员信息
	/*
	j = pstClan->ClanBrief.OfficerNum;
	while(j--)
	{
		if(strcmp(pszRoleName,
		          pstClan->ClanBrief.Officers[j].RoleName) == 0)
		{
			if(j != (unsigned)(pstClan->ClanBrief.OfficerNum - 1))
			{
				memcpy(pstClan->ClanBrief.Officers + j,
				       pstClan->ClanBrief.Officers +
				       pstClan->ClanBrief.OfficerNum - 1,
				       sizeof(pstClan->ClanBrief.Officers[0]));
			}

			--pstClan->ClanBrief.OfficerNum;
			break;
		}
	}
	*/

	// 更新成员数
	--pstClan->ClanCore.MemNum;
	--pstClan->ClanProf.MemNum;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, DEL_CLAN_MEMBER_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, pstClan, sizeof(*pstClan),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, DEL_CLAN_MEMBER_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_del_clan_member_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                 CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	const TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
		        DEL_CLAN_MEMBER_REQ);
		return -1;
	}

	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_NO,
	        DEL_CLAN_MEMBER_REQ);	
	return 0;
}

static int cs_db_will_del_clan_member(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                 TFRAMEHEAD *pstFrameHead,
                                 const ZONEDELCLANMEMBERREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	MAILENTRY stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, DEL_CLAN_MEMBER_REQ | 0x80000000, pstFrameHead,
	            NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 1;
	snprintf(pstDBReq->OperInfo.WhereDef, sizeof(pstDBReq->OperInfo.WhereDef),
		     "WHERE Name = \'%s\'", pstReq->ClanName);

	bzero(&stDummy, sizeof(stDummy));
	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, DEL_CLAN_MEMBER_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_will_del_clan_member_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                      CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	int iRet = 0;
	CLAN stData;
	TFRAMEHEAD stDummy;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		goto error;
	}

	if(pstDBRes->SelectResult.ResultNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "nothing be selected");
		goto error;
	}

	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stData, sizeof(stData)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		goto error;
	}

	stDummy.ConnIdx = pstAsyn->ConnIdx;
	stDummy.ExtraInfo.IPInfo = pstAsyn->IPInfo;

	iRet = cs_db_del_clan_member(pstAppCtx, pstEnv, &stDummy, &stData,
	                             pstAsyn->Data);
	return iRet;

error:
	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
	        DEL_CLAN_MEMBER_REQ);
	return 0;
}

static int cs_z_del_clan_member_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                        const ZONECTLDELCLANMEMBERRES *pstCtlRes)
{
	int iRet = 0;

	UNUSED(pstAppCtx);

	if(!pstCtlRes->Core.Succeeded)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone_svr: delete clan member fail!");
	}
	
	iRet = pstCtlRes->Core.Succeeded ? ERR_NO : ERR_SYS;
	err_res(pstEnv, pstCtlRes->Extra.ConnIdx, &pstCtlRes->Extra.IPInfo, iRet,
	        DEL_CLAN_MEMBER_REQ);
	return 0;
}

static int cs_del_clan_member(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                              SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead,
                              const ZONEDELCLANMEMBERREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLDELCLANMEMBERREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlDelClanMemberReq;

	UNUSED(pstPkgReq);
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_will_del_clan_member(pstAppCtx, pstEnv, pstFrameHead,
		                                  pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DEL_CLAN_MEMBER_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	
out:
	return iRet;
}

static int cs_db_disband_clan(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                              TFRAMEHEAD *pstFrameHead,
                              const ZONEDISBANDCLANREQ *pstReq)
{
	TORMPKG stPkg;
	TORMDELETEREQ *pstDBReq = &stPkg.Body.DeleteReq;
	MAILENTRY stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_DELETE_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, DISBAND_CLAN_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstDBReq->OperInfo.WhereDef, sizeof(pstDBReq->OperInfo.WhereDef),
		     "WHERE Name = \'%s\'", pstReq->ClanName);

	bzero(&stDummy, sizeof(stDummy));
	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, DISBAND_CLAN_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_disband_clan_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                              CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	TORMDELETERES *pstDBRes = &pstPkg->Body.DeleteRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
		        DISBAND_CLAN_REQ);
		return -1;
	}

	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_NO,
	        DISBAND_CLAN_REQ);
	return 0;	
}

static int cs_z_disband_clan_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                     const ZONECTLDISBANDCLANRES *pstCtlRes)
{
	int iRet = 0;

	UNUSED(pstAppCtx);

	if(!pstCtlRes->Core.Succeeded)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "zone_svr: disband clan fail!");
	}
	iRet = pstCtlRes->Core.Succeeded ? ERR_NO : ERR_SYS;
	err_res(pstEnv, pstCtlRes->Extra.ConnIdx, &pstCtlRes->Extra.IPInfo,
	        iRet, DISBAND_CLAN_REQ);
	return 0;
}

static int cs_disband_clan(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                           SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead,
                           const ZONEDISBANDCLANREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLDISBANDCLANREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlDisbandClanReq;

	UNUSED(pstPkgReq);
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_disband_clan(pstAppCtx, pstEnv, pstFrameHead, pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DISBAND_CLAN_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	
out:
	return iRet;
}

static int cs_db_clear_clan_diplomatism(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                        TFRAMEHEAD *pstFrameHead, CLAN *pstClan)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;

	UNUSED(pstAppCtx);

	// 清除公会的外交关系
	pstClan->ClanInner.ClanEnemyNum  = 0;
	bzero(pstClan->ClanInner.ClanEnemys,
	      sizeof(pstClan->ClanInner.ClanEnemys));
	pstClan->ClanInner.ClanFriendNum = 0;
	bzero(pstClan->ClanInner.ClanFriends,
	      sizeof(pstClan->ClanInner.ClanFriends));

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, CLEAR_CLAN_DIPLOMATISM_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, pstClan, sizeof(*pstClan),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, CLEAR_CLAN_DIPLOMATISM_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_clear_clan_diplomatism_res(TAPPCTX *pstAppCtx,
                                            CS_SVRENV *pstEnv, CS_ASYN *pstAsyn,
                                            TORMPKG *pstPkg)
{
	const TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
		        CLEAR_CLAN_DIPLOMATISM_REQ);
		return -1;
	}

	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_NO,
	        CLEAR_CLAN_DIPLOMATISM_REQ);
	return 0;
}

static int cs_db_will_clear_clan_diplomatism(TAPPCTX *pstAppCtx,
                                             CS_SVRENV *pstEnv,
                                             TFRAMEHEAD *pstFrameHead,
                                      const ZONECLEARCLANDIPLOMATISMREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	MAILENTRY stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, CLEAR_CLAN_DIPLOMATISM_REQ | 0x80000000, pstFrameHead,
	            NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 1;
	snprintf(pstDBReq->OperInfo.WhereDef, sizeof(pstDBReq->OperInfo.WhereDef),
		     "WHERE Name = \'%s\'", pstReq->ClanName);

	bzero(&stDummy, sizeof(stDummy));
	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, CLEAR_CLAN_DIPLOMATISM_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_will_clear_clan_diplomatism_res(TAPPCTX *pstAppCtx,
                                                 CS_SVRENV *pstEnv,
                                                 CS_ASYN *pstAsyn,
                                                 TORMPKG *pstPkg)
{
	int iRet = 0;
	CLAN stData;
	TFRAMEHEAD stDummy;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		goto error;
	}

	if(pstDBRes->SelectResult.ResultNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "nothing be selected");
		goto error;
	}

	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stData, sizeof(stData)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		goto error;
	}

	stDummy.ConnIdx = pstAsyn->ConnIdx;
	stDummy.ExtraInfo.IPInfo = pstAsyn->IPInfo;

	iRet = cs_db_clear_clan_diplomatism(pstAppCtx, pstEnv, &stDummy, &stData);
	return iRet;

error:
	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
	        CLEAR_CLAN_DIPLOMATISM_REQ);
	return 0;
}

static int cs_z_clear_clan_diplomatism_res(TAPPCTX *pstAppCtx,
                                           CS_SVRENV *pstEnv,
			                    const ZONECTLCLEARCLANDIPLOMATISMRES *pstCtlRes)
{
	int iRet = 0;

	UNUSED(pstAppCtx);

	if(!pstCtlRes->Core.Succeeded)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone_svr: clear clan diplomatism fail!");
	}
	iRet = pstCtlRes->Core.Succeeded ? ERR_NO : ERR_SYS;
	err_res(pstEnv, pstCtlRes->Extra.ConnIdx, &pstCtlRes->Extra.IPInfo, iRet,
	        CLEAR_CLAN_DIPLOMATISM_REQ);
	return 0;
}

static int cs_clear_clan_diplomatism(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                     SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead,
                                     const ZONECLEARCLANDIPLOMATISMREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLCLEARCLANDIPLOMATISMREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlClearClanDiplomatismReq;

	UNUSED(pstPkgReq);
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_will_clear_clan_diplomatism(pstAppCtx, pstEnv,
		                                         pstFrameHead, pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_CLEAR_CLAN_DIPLOMATISM_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	
out:
	return iRet;
}

static int cs_db_get_clan_list(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                               SSPKG *pstPkg, TFRAMEHEAD *pstFrameHead,
                               ZONEFINDCLANREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	CLAN stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, FIND_CLAN_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OffSet = pstReq->Index * CLAN_MAX_RES;
	pstDBReq->Limit = CLAN_MAX_RES;
	snprintf(pstDBReq->OperInfo.WhereDef, sizeof(pstDBReq->OperInfo.WhereDef),
		     "WHERE Name LIKE \'%s\'", pstReq->Pattern);

	bzero(&stDummy, sizeof(stDummy));
	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, FIND_CLAN_REQ);
		return -1;
	}
	return 0;
	UNUSED(pstPkg);	//add by paraunused.pl
}

static int cs_db_get_clan_list_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                               CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	int i = 0;
	SSPKG stSSPkg;
	ZONEFINDCLANRES *pstCtlRes = &stSSPkg.Body.CsCtrL.CsCmd.FindClanRes;
	TORMSELECTRES *pstRes = &pstPkg->Body.SelectRes;
	CLAN stDummy;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstRes->ErrNo))
	{
		goto error;
	}
	
	pstCtlRes->ResNum = pstRes->SelectResult.ResultNum;
	i = pstRes->SelectResult.ResultNum;
	while(i--)
	{
		if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stDummy, sizeof(stDummy)) < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
			goto error;
		}
		STRNCPY(pstCtlRes->ClanArray[i].Name, stDummy.ClanProf.Name,
		        sizeof(pstCtlRes->ClanArray[i].Name));
	}

final:
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = FIND_CLAN_RES;
	return cs_sendto_clt_pkg(pstEnv, pstAsyn, &stSSPkg);

error:
	pstCtlRes->ResNum = 0;
	goto final;
	return 0;
}

static int cs_z_exchange_clan_leader_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                    const ZONECTLEXCHANGECLANLEADERRES *pstCtlRes)
{
	int iRet = 0;

	UNUSED(pstAppCtx);

	if(!pstCtlRes->Core.Succeeded)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone_svr: clear clan diplomatism fail!");
	}
	iRet = pstCtlRes->Core.Succeeded ? ERR_NO : ERR_SYS;
	err_res(pstEnv, pstCtlRes->Extra.ConnIdx, &pstCtlRes->Extra.IPInfo, iRet,
	        EXCHANGE_CLAN_LEADER_REQ);
	return 0;
}

static int cs_z_set_clan_base_attr_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                    const ZONECTLSETCLANBASEATTRRES *pstCtlRes)
{
	int iRet = 0;

	UNUSED(pstAppCtx);

	if(!pstCtlRes->Core.Succeeded)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone_svr: set clan base attr fail!");
	}
	iRet = pstCtlRes->Core.Succeeded ? ERR_NO : ERR_SYS;
	err_res(pstEnv, pstCtlRes->Extra.ConnIdx, &pstCtlRes->Extra.IPInfo, iRet,
	        SET_CLAN_BASE_ATTR_RES);
	return 0;
}

static int cs_db_will_set_clan_base_attr(TAPPCTX *pstAppCtx,
                                             CS_SVRENV *pstEnv,
                                             TFRAMEHEAD *pstFrameHead,
                                      const ZONESETCLANBASEATTRREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	MAILENTRY stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, SET_CLAN_BASE_ATTR_REQ | 0x80000000, pstFrameHead,
	            pstReq, sizeof(*pstReq));

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 1;
	snprintf(pstDBReq->OperInfo.WhereDef, sizeof(pstDBReq->OperInfo.WhereDef),
		     "WHERE Name = '%s'", pstReq->ClanName);

	bzero(&stDummy, sizeof(stDummy));
	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, SET_CLAN_BASE_ATTR_REQ);
		return -1;
	}
	return 0;
}

static int cs_set_clan_base_attr_req(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                     SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead,
                                     const ZONESETCLANBASEATTRREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLSETCLANBASEATTRREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CtlSetClanBaseAttrReq;
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_will_set_clan_base_attr(pstAppCtx, pstEnv,
		                                         pstFrameHead, pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_SET_CLAN_BASE_ATTR_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);
	
out:
	return iRet;
	UNUSED(pstPkgReq);
}

static int cs_db_set_clan_base_attr(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                      TFRAMEHEAD *pstFrameHead, CLAN *pstClan,
                                      const char *pszData)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;
	ZONESETCLANBASEATTRREQ* pstReq = (ZONESETCLANBASEATTRREQ*)pszData;
	
	//各种设置
	pstClan->ClanProf.Money = pstReq->Money;
	pstClan->ClanProf.StableValue = pstReq->Stable;
	pstClan->ClanProf.TechPoint = pstReq->Tech;
	pstClan->ClanProf.ActiveValue = pstReq->Active;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, SET_CLAN_BASE_ATTR_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, pstClan, sizeof(*pstClan),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, SET_CLAN_BASE_ATTR_REQ);
		return -1;
	}
	return 0;
}


static int cs_db_will_set_clan_base_attr_res(TAPPCTX *pstAppCtx,
                                               CS_SVRENV *pstEnv, 
	                                           CS_ASYN *pstAsyn,
	                                           TORMPKG *pstPkg)
{
	int iRet = 0;
	CLAN stData;
	TFRAMEHEAD stDummy;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		goto error;
	}

	if(pstDBRes->SelectResult.ResultNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "nothing be selected");
		goto error;
	}

	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stData, sizeof(stData)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		goto error;
	}

	stDummy.ConnIdx = pstAsyn->ConnIdx;
	stDummy.ExtraInfo.IPInfo = pstAsyn->IPInfo;

	iRet = cs_db_set_clan_base_attr(pstAppCtx, pstEnv, &stDummy, &stData,
	                                  pstAsyn->Data);
	return iRet;

error:
	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
	        SET_CLAN_BASE_ATTR_REQ);
	return 0;
}

static int cs_db_set_clan_base_attr_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                      CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	const TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
		        SET_CLAN_BASE_ATTR_REQ);
		return -1;
	}

	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_NO,
	        SET_CLAN_BASE_ATTR_REQ);
	return 0;
	UNUSED(pstAppCtx);
}



static int cs_db_exchange_clan_leader(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                      TFRAMEHEAD *pstFrameHead, CLAN *pstClan,
                                      const char *pszRoleName)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;
	CLANMEM *pstLeader = NULL, *pstNewLeader = NULL;
	unsigned int i = 0;

	if(strcmp(pszRoleName, pstClan->ClanBrief.Leader.RoleName) == 0)
	{
		// 不能自己把职位转让给自己
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "can not exchange leader[%s] from clan[%s]",
		           pstClan->ClanBrief.Leader.RoleName,
		           pstClan->ClanProf.Name);
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_ARGS, EXCHANGE_CLAN_LEADER_REQ);
		return -1;
	}
	
	for(i = 0; i < pstClan->ClanCore.MemNum; ++i)
	{
		if(strcmp(pszRoleName,
		          pstClan->ClanCore.ClanMem[i].RoleMember.RoleName) == 0)
		{
			pstNewLeader = pstClan->ClanCore.ClanMem + i;
		}
		else if(strcmp(pstClan->ClanBrief.Leader.RoleName,
		               pstClan->ClanCore.ClanMem[i].RoleMember.RoleName) == 0)
		{
			pstLeader = pstClan->ClanCore.ClanMem + i;
		}
		if(pstLeader && pstNewLeader)
		{
			break;
		}
	}
	if(!pstNewLeader || !pstLeader)
	{
		// 没有找到指定的成员
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "can not find member[%s] in clan[%s]",
		           pszRoleName,
		           pstClan->ClanProf.Name);
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_ARGS, EXCHANGE_CLAN_LEADER_REQ);
		return -1;
	}

	// 交换会长的职位
	pstLeader->Position = pstNewLeader->Position;
	pstNewLeader->Position = CLAN_POS_PRESIDENT;
	pstClan->ClanBrief.Leader = pstNewLeader->RoleMember;

	// 更新官员信息
	pstClan->ClanBrief.AssistNum = 0;
	//pstClan->ClanBrief.OfficerNum = 0;
	for(i = 0; i < pstClan->ClanCore.MemNum; ++i)
	{
		const CLANMEM *pstMem = pstClan->ClanCore.ClanMem + i;
		switch(pstMem->Position)
		{
		case CLAN_POS_VICE_PRESIDENT:
			pstClan->ClanBrief.Assists[pstClan->ClanBrief.AssistNum++] =
				pstMem->RoleMember;
			break;

		//case CLAN_POS_OFFICER:
		//	pstClan->ClanBrief.Officers[pstClan->ClanBrief.OfficerNum++] =
		//		pstMem->RoleMember;
		//	break;
		}
	}

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, EXCHANGE_CLAN_LEADER_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	if (cs_db_send_req(pstAppCtx, pstEnv, &stPkg, pstClan, sizeof(*pstClan),
		               pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, EXCHANGE_CLAN_LEADER_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_exchange_clan_leader_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                      CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	const TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
		        EXCHANGE_CLAN_LEADER_REQ);
		return -1;
	}

	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_NO,
	        EXCHANGE_CLAN_LEADER_REQ);
	return 0;
}

static int cs_db_will_exchange_clan_leader(TAPPCTX *pstAppCtx,
                                           CS_SVRENV *pstEnv,
                                           TFRAMEHEAD *pstFrameHead,
                                           ZONEEXCHANGECLANLEADERREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	CLAN stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, EXCHANGE_CLAN_LEADER_REQ | 0x80000000,
	            pstFrameHead, pstReq->NewLeaderRoleName,
	            strlen(pstReq->NewLeaderRoleName) + 1);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 1;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	bzero(&stDummy, sizeof(stDummy));
	STRNCPY(stDummy.ClanProf.Name, pstReq->ClanName,
	        sizeof(stDummy.ClanProf.Name));
	if(cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		              pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, EXCHANGE_CLAN_LEADER_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_will_exchange_clan_leader_res(TAPPCTX *pstAppCtx,
                                               CS_SVRENV *pstEnv, 
	                                           CS_ASYN *pstAsyn,
	                                           TORMPKG *pstPkg)
{
	int iRet = 0;
	CLAN stData;
	TFRAMEHEAD stDummy;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		goto error;
	}

	if(pstDBRes->SelectResult.ResultNum <= 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "nothing be selected");
		goto error;
	}

	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stData, sizeof(stData)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		goto error;
	}

	stDummy.ConnIdx = pstAsyn->ConnIdx;
	stDummy.ExtraInfo.IPInfo = pstAsyn->IPInfo;

	iRet = cs_db_exchange_clan_leader(pstAppCtx, pstEnv, &stDummy, &stData,
	                                  pstAsyn->Data);
	return iRet;

error:
	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
	        EXCHANGE_CLAN_LEADER_REQ);
	return 0;
}

static int cs_exchange_clan_leader(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                   SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead,
                                   ZONEEXCHANGECLANLEADERREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLEXCHANGECLANLEADERREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CltExchangeClanLeaderReq;

	UNUSED(pstPkgReq);
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_will_exchange_clan_leader(pstAppCtx, pstEnv, pstFrameHead,
		                                       pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_EXCHANGE_CLAN_LEADER_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);

out:
	return iRet;
}

static int cs_z_del_clan_diplomatism_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                      const ZONECTLDELCLANDIPLOMATISMRES *pstCtlRes)
{
	int iRet = 0;

	UNUSED(pstAppCtx);

	if(!pstCtlRes->Core.Succeeded)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone_svr: clear clan diplomatism fail!");
	}
	iRet = pstCtlRes->Core.Succeeded ? ERR_NO : ERR_SYS;
	err_res(pstEnv, pstCtlRes->Extra.ConnIdx, &pstCtlRes->Extra.IPInfo, iRet,
	        DEL_CLAN_DIPLOMATISM_REQ);
	return 0;
}

static int del_clan_diplomatism(const char *pszName, CLAN *pstClan)
{
	int i = pstClan->ClanInner.ClanFriendNum;
	int iFlag = 0;
	while(i--)
	{
		if(strcmp(pszName, pstClan->ClanInner.ClanFriends[i].ClanId.Name) == 0)
		{
			if(i != pstClan->ClanInner.ClanFriendNum - 1)
			{
				memmove(pstClan->ClanInner.ClanFriends + i,
				        pstClan->ClanInner.ClanFriends +
				        pstClan->ClanInner.ClanFriendNum - 1,
				        sizeof(pstClan->ClanInner.ClanFriends[0]));
			}
			++iFlag;
			break;
		}
	}

	i = pstClan->ClanInner.ClanEnemyNum;
	while(i--)
	{
		if(strcmp(pszName, pstClan->ClanInner.ClanEnemys[i].ClanId.Name) == 0)
		{
			if(i != pstClan->ClanInner.ClanFriendNum - 1)
			{
				memmove(pstClan->ClanInner.ClanEnemys + i,
				        pstClan->ClanInner.ClanEnemys +
				        pstClan->ClanInner.ClanEnemyNum - 1,
				        sizeof(pstClan->ClanInner.ClanEnemys[0]));
			}
			++iFlag;
			break;
		}
	}

	return iFlag == 2 ? 0 : -1;
}

static int cs_db_del_clan_diplomatism(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                      TFRAMEHEAD *pstFrameHead, CLAN *pstClan1,
                                      CLAN *pstClan2)
{
	TORMPKG stPkg;
	TORMUPDATEREQ *pstDBReq = &stPkg.Body.UpdateReq;

	// 互相删除外交关系
	if(del_clan_diplomatism(pstClan1->ClanProf.Name, pstClan2) < 0 ||
	   del_clan_diplomatism(pstClan2->ClanProf.Name, pstClan1) < 0)
	{
		err_res(pstEnv, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_ARGS, DEL_CLAN_DIPLOMATISM_REQ);
		return -1;
	}

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, 
		              "Clan", -1);
	cs_set_asyn(&stPkg.Head, DEL_CLAN_DIPLOMATISM_REQ, pstFrameHead, NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OperInfo.WhereDef[0] = 0;

	if(cs_db_send_req(pstAppCtx, pstEnv, &stPkg, pstClan1, sizeof(*pstClan1),
		               pstEnv->iMiscDBID) < 0 ||
	   cs_db_send_req(pstAppCtx, pstEnv, &stPkg, pstClan2, sizeof(*pstClan2),
			          pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv, pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, DEL_CLAN_DIPLOMATISM_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_del_clan_diplomatism_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, 
	                                      CS_ASYN *pstAsyn, TORMPKG *pstPkg)
{
	const TORMUPDATERES *pstDBRes = &pstPkg->Body.UpdateRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "update failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
		        DEL_CLAN_DIPLOMATISM_REQ);
		return -1;
	}

	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_NO,
	        DEL_CLAN_DIPLOMATISM_REQ);
	return 0;
}

static int cs_db_will_del_clan_diplomatism(TAPPCTX *pstAppCtx,
                                           CS_SVRENV *pstEnv,
                                           TFRAMEHEAD *pstFrameHead,
                                           ZONEDELCLANDIPLOMATISMREQ *pstReq)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstDBReq = &stPkg.Body.SelectReq;
	CLAN stDummy;

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ,
	                  "Clan", -1);
	cs_set_asyn(&stPkg.Head, DEL_CLAN_DIPLOMATISM_REQ | 0x80000000, pstFrameHead,
	            NULL, 0);

	pstDBReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDBReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstDBReq->OffSet = 0;
	pstDBReq->Limit = 2;
	snprintf(pstDBReq->OperInfo.WhereDef, sizeof(pstDBReq->OperInfo.WhereDef),
	         "WHERE Name='%s' OR Name='%s'",
	         pstReq->ClanName, pstReq->TargetName);

	bzero(&stDummy, sizeof(stDummy));
	if(cs_db_send_req(pstAppCtx, pstEnv, &stPkg, &stDummy, sizeof(stDummy),
		              pstEnv->iMiscDBID) < 0)
	{
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo,
		        ERR_TDR, DEL_CLAN_DIPLOMATISM_REQ);
		return -1;
	}
	return 0;
}

static int cs_db_will_del_clan_diplomatism_res(TAPPCTX *pstAppCtx,
                                               CS_SVRENV *pstEnv, 
	                                           CS_ASYN *pstAsyn,
	                                           TORMPKG *pstPkg)
{
	int iRet = 0;
	CLAN stData1, stData2;
	TFRAMEHEAD stDummy;
	const TORMSELECTRES *pstDBRes = &pstPkg->Body.SelectRes;

	UNUSED(pstAppCtx);

	if(TORM_ERR_IS_ERROR(pstDBRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select failed. reason : %s\n", 
			       torm_error_string(pstPkg->Body.UpdateRes.ErrNo));
		goto error;
	}

	if(pstDBRes->SelectResult.ResultNum != 2)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "need 2 clan be selected");
		goto error;
	}

	if(torm_fetch_one_meta(pstEnv->hTorm, (char *)&stData1,
	                       sizeof(stData1)) < 0 ||
	   torm_fetch_one_meta(pstEnv->hTorm, (char *)&stData2,
			               sizeof(stData2)) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		goto error;
	}

	stDummy.ConnIdx = pstAsyn->ConnIdx;
	stDummy.ExtraInfo.IPInfo = pstAsyn->IPInfo;

	iRet = cs_db_del_clan_diplomatism(pstAppCtx, pstEnv, &stDummy, &stData1,
	                                  &stData2);
	return iRet;

error:
	err_res(pstEnv, pstAsyn->ConnIdx, &pstAsyn->IPInfo, ERR_SYS,
	        DEL_CLAN_DIPLOMATISM_REQ);
	return 0;
}

static int cs_del_clan_diplomatism(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
                                   SSPKG *pstPkgReq, TFRAMEHEAD *pstFrameHead,
                                   ZONEDELCLANDIPLOMATISMREQ *pstReq)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONECTLDELCLANDIPLOMATISMREQ *pstSSReq = 
		&stSSPkg.Body.CsCtrL.CsCmd.CltDelClanDiplomatismReq;

	UNUSED(pstPkgReq);
	
	if(!pstEnv->bIsAttath)
	{
		iRet = cs_db_will_del_clan_diplomatism(pstAppCtx, pstEnv, pstFrameHead,
		                                       pstReq);
		goto out;
	}

	pstSSReq->Core = *pstReq;
	pstSSReq->Extra.ConnIdx = pstFrameHead->ConnIdx;
	pstSSReq->Extra.IPInfo = pstFrameHead->ExtraInfo.IPInfo;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = CTL_DEL_CLAN_DIPLOMATISM_REQ;
	iRet = cs_sendto_zsvr_pkg(pstAppCtx, pstEnv, &stSSPkg);

out:
	return iRet;
}

int cs_clan_select(TAPPCTX* pstAppCtx,CS_SVRENV* pstEnv,SSPKG *pstPkgReq,TFRAMEHEAD *pstFrameHead)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;
	//ROLEDATA stRoleData;
	CLAN stClan;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	CS_ASYN stCsAsyn;
	SSCSCTRL *pstCsCtrl = &pstPkgReq->Body.CsCtrL;

	memset(&stClan, 0, sizeof(stClan));
	memset(&stPkg,0,sizeof(TORMPKG));

	if(pstCsCtrl->Cmd == GET_CLAN_INFO_REQ)
	{	
		cs_init_csasyn(&stCsAsyn,pstCsCtrl->Cmd,pstFrameHead->ConnIdx,&pstFrameHead->ExtraInfo.IPInfo,
			(char*)&pstCsCtrl->CsCmd.GetClanInfoReq,sizeof(pstCsCtrl->CsCmd.GetClanInfoReq));
		//strncpy(stRoleData.RoleName, pstCsCtrl->CsCmd.BlockPlayerReq.RoleName, sizeof(stRoleData.RoleName));
	}
	else
	{
		return -1;
	}

	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, "Clan", -1);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,sizeof(pstSelectReq->OperInfo.WhereDef),
				"where Name='%s'", pstCsCtrl->CsCmd.GetClanInfoReq.ClanName);
	
	stPkg.Head.AsynCallBackDataLen = sizeof(stCsAsyn);
	memcpy(stPkg.Head.AsynCallBackData, &stCsAsyn, stPkg.Head.AsynCallBackDataLen);
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stClan;
	stMetaData.iBuff = sizeof(stClan);

	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrl->Cmd);
		return -1;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, &stPkg, pstEnv->iRoleDBID))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_torm_send fail");
		err_res(pstEnv,pstFrameHead->ConnIdx, &pstFrameHead->ExtraInfo.IPInfo, ERR_TDR,pstCsCtrl->Cmd);
		return -1;
	}

	return 0;
}

static int back_record_get_date(const char *pszDate, char* szYear, char *szMonth)
{
	//2011-07-01 15:32:08
	const char *p = pszDate;
	strncpy(szYear,p+2,2);
	szYear[2] = 0;
	strncpy(szMonth,p+5,2);
	szMonth[2] = 0;
	return 0;
}

int get_role_sanpshot(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv,const char* pszRoleName, const char*pszDate,
						LOGROLESNAPSHOT *pstSanpshot)
{
	TDRDBMS stDBMS;
	TDRDBHANDLE hResDBHandle;
	TDRDBRESULTHANDLE hDBResult = 0;
	char szSql[10240];
	int iNumRows;
	TDRDATA stSql;
	LPTDRMETALIB pstResLib;
	//LOGROLESNAPSHOT *pstSanpshot, stSanpshot;
	//int iErr;	//comment by paraunused.pl
	char szYear[16],szMonth[16];
	int iRet;
	
	memset(&stDBMS, 0, sizeof(stDBMS));
	STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
	STRNCPY(stDBMS.szDBMSCurDatabaseName, pstEnv->pstConf->szSzLogDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
	STRNCPY(stDBMS.szDBMSConnectionInfo, pstEnv->pstConf->szSzLogDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
	STRNCPY(stDBMS.szDBMSUser, pstEnv->pstConf->szSzLogDBUser, sizeof(stDBMS.szDBMSUser));
	STRNCPY(stDBMS.szDBMSPassword, pstEnv->pstConf->szSzLogDBPasswd, sizeof(stDBMS.szDBMSPassword));
	stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&hResDBHandle, &stDBMS, NULL))
	{
		iRet = CS_ERR_SYS;
		goto final;
	}

	if (0 > tdr_load_metalib(&pstResLib, "../../cfg/db_meta/role_db_meta.tdr") && 
		0 > tdr_load_metalib(&pstResLib, "../cfg/db_meta/role_db_meta.tdr"))
	{
		iRet = CS_ERR_SYS;
		goto final;
	}

	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];

	TDRDBOBJECT stDBObj;
	memset (&stDBObj, 0, sizeof(stDBObj));
	stDBObj.pstMeta = tdr_get_meta_by_name(pstResLib, "LogRoleSnapshot");
	if (!stDBObj.pstMeta)
	{
		iRet = CS_ERR_SYS;
		goto final;
	}	
	stDBObj.iVersion = tdr_get_meta_current_version(stDBObj.pstMeta);
	stDBObj.iObjSize = sizeof(*pstSanpshot);
	stDBObj.pszObj = (char*)pstSanpshot;

	back_record_get_date(pszDate, szYear, szMonth);
	memset(szSql, 0, sizeof(szSql));
	snprintf(szSql, sizeof(szSql),
			"select * from LogRoleSnapshot_%s%s where RoleName='%s' AND LogTime='%s'",
			szYear,szMonth,pszRoleName,pszDate);
	stSql.iBuff = strlen(szSql);
	stSql.pszBuff = &szSql[0];
	if(tdr_query(&hDBResult, hResDBHandle, &stSql) != 0)
	{
		iRet =  CS_ERR_SYS;
		goto final;
	}

	iNumRows = tdr_num_rows(hDBResult);
	if(iNumRows != 1)
	{
		iRet =  CS_ERR_SYS;
		goto final;
	}

	
	if (tdr_fetch_row(hDBResult, &stDBObj) != 0)
	{
		iRet =  CS_ERR_SYS;
		goto final;
	}
		
	iRet = 0;
	//pstSanpshot = (LOGROLESNAPSHOT *)stDBObj.pszObj;

final:	
	if(hDBResult)
		tdr_free_dbresult(&hDBResult);
	return iRet;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int cs_clt_msg(TAPPCTX* pstAppCtx, CS_SVRENV* pstEnv, char *pszMsg, int iMsgLen, TFRAMEHEAD *pstFrameHead)
{
	int iRet = 0;
	SSPKG stPkgReq;
	SSCSCTRL *pstCsCtrlReq;
	pstCsCtrlReq = &stPkgReq.Body.CsCtrL;
	
	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &stPkgReq, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack fail");
		return -1;
	}
	if(stPkgReq.Head.Cmd != CS_CTRL)
	{
		return -1;
	}

	tlog_info_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstSSHan->pstSSProtoPkgMeta, 
					&stPkgReq, sizeof(stPkgReq), 0);

	switch(pstCsCtrlReq->Cmd)
	{
	case CTL_SHOP_CGI_REQ:
		iRet= cs_zone_svr_asyn(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead,pstCsCtrlReq->Cmd);
		break;
	case FORCE_MSG_REQ:
		iRet= cs_zone_svr_asyn(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead,pstCsCtrlReq->Cmd);
		break;
	case SET_LIFT_SKILL_REQ:
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetLiftSkillReq.RoleName);
		break;
	case GET_LIFT_SKILL_REQ:
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetLiftSkillReq.RoleName);
		break;
	case GET_SAFELOCK_REQ:
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetSafeLockReq.RoleName);
		break;
	case SET_SAFELOCK_REQ:
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetSafeLockReq.RoleName);
		break;
	case ROLE_BACK_RECORD_REQ://回档
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.RoleBackReq.RoleName);
		break;
	case SET_ACCOUNT_UIN_FLAG_REQ:
		iRet = cs_zone_svr_asyn(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead,pstCsCtrlReq->Cmd);
		break;
	
	case IP_LIMIT_REQ:
		iRet = cs_zone_svr_asyn(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead,pstCsCtrlReq->Cmd);
		break;
	case GET_ROLE_BLACK_REQ:
		
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.BlackReq.RoleName);
		break;
	case GET_ROLE_FRIEND_REQ:
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.FriendReq.RoleName);
		
		break;
	case GET_CLAN_INFO_REQ:
		iRet = cs_clan_select(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead);
		break;
	case SET_ROLE_PERM_REQ:
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetPermReq.RoleName);
		break;
	case DEL_ROLE_REQ:
		return cs_del_role_req(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.DelRoleReq.RoleName);
		break;

	case SET_ROLE_LOGOUT_REQ:								// 踢角色
		return cs_kick_off(pstAppCtx, pstEnv, &stPkgReq,pstFrameHead);
		
		break;
	case BLOCKPLAYER_REQ:
		if(pstCsCtrlReq->CsCmd.BlockPlayerReq.Type == 0)		// 封角色
		{
			iRet = cs_modify_role_data(pstAppCtx, pstEnv, &stPkgReq,pstFrameHead);
		}
		else	 if(pstCsCtrlReq->CsCmd.BlockPlayerReq.Type == 1)	// 解封角色
		{
			iRet = cs_unblock_player(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead);
		}
		else
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "BLOCKPLAYER_REQ Type err");
			return -1;
		}
		
		break;
	
	case SILENCE_ROLE_REQ:			// 禁言某个玩家
		if(cs_modify_role_data(pstAppCtx, pstEnv, &stPkgReq,pstFrameHead) < 0)
		{
			return -1;
		}
		
		break;
	case ROLE_LIST_REQ:
		if (cs_db_role_list(pstAppCtx, pstEnv, &stPkgReq,pstFrameHead)<0)
		{
			return -1;
		}
		break;
	case CHG_ROLE_POS_REQ:			// 跳转角色
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.ChgRolePosReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	
	case GET_ROLE_DETAIL_REQ:		// 获取玩家详细数据
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRoleDetailReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case SET_ROLE_DETAIL_REQ:		// 设置玩家详细信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRoleDetailReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case GET_ROLE_SKILLINFO_REQ:	// 获取玩家技能信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRoleSkillInfoReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case SET_ROLE_SKILLINFO_REQ:	// 设置玩家技能
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRoleSkillInfoReq.RoleName) < 0)
		{
			return -1;
		}
		break;

	case GET_ROLE_ITEMLIST_REQ:		// 获取玩家道具信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRoleItemListReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case SET_ROLE_ITEMLIST_REQ:		// 设置玩家道具
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRoleItemListReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case GET_ROLE_DIATHESIS_REQ:	// 获取玩家素质信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRoleDiathesisReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case SET_ROLE_DIATHESIS_REQ:	// 设置玩家素质信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRoleDiathesisReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case GET_ROLE_TASK_REQ:		// 获取角色任务信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRoleTaskReq.RoleName) < 0)
		{
			return -1;
		}
		break;						// 删除角色任务
	case DEL_ROLE_TASK_REQ:
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.DelRoleTaskReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case GET_ROLE_PACKAGE_REQ:	// 获取玩家背包格数
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRolePackageReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case SET_ROLE_PACKAGE_REQ:	// 设置玩家背包格数
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRolePackageReq.RoleName) < 0)
		{
			return -1;
		}
	case GET_ROLE_RIDE_REQ:	// 获取玩家骑乘信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRoleRideInfoReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case SET_ROLE_RIDE_REQ:	// 设置玩家骑乘
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRoleRideInfoReq.RoleName) < 0)
		{
			return -1;
		}
	case GET_ROLE_QIYUELIST_REQ:	//获取契约列表
		if(cs_qiyue_req(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRoleQiYueListReq.RoleName) < 0 )
		{
			return -1;
		}
		break;
	case SET_ROLE_QIYUE_REQ:	//设置当前使用的契约
		if(cs_qiyue_req(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRoleQiYueReq.RoleName) < 0 )
		{
			return -1;
		}
		break;	
	case ADD_ROLE_QIYUE_REQ:	//加契约
		if(cs_qiyue_req(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.AddRoleQiYueReq.RoleName) < 0 )
		{
			return -1;
		}
		break;	
	case DEL_ROLE_QIYUE_REQ:	//删除契约
		if(cs_qiyue_req(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.DelRoleQiYueReq.RoleName) < 0 )
		{
			return -1;
		}
		break;

	case MODIFY_ROLE_FAIRY_REQ:
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRoleFairyReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case GET_ROLE_PET_REQ:	// 获取玩家宠物信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetRolePetInfoReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case SET_ROLE_PET_REQ:	// 设置玩家宠物
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetRolePetInfoReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case CHG_ROLE_NAME_REQ: // 修改角色名
		if (pstEnv->bIsAttath)
		{
			return cs_zone_svr_asyn(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead,
			                        pstCsCtrlReq->Cmd);
		}
		else
		{
			return -1;
		}
		break;

	case GET_ROLE_MAIL_LIST_REQ: // 获取角色的邮件列表
		iRet = cs_db_role_maillist(pstAppCtx, pstEnv, pstFrameHead,
			                         &pstCsCtrlReq->CsCmd.GetRoleMailListReq);
		break;

	case GET_ROLE_MAIL_REQ: // 获取一封邮件的详细数据
		iRet = cs_db_role_maildata(pstAppCtx, pstEnv, pstFrameHead,
			                         &pstCsCtrlReq->CsCmd.GetRoleMailReq);
		break;
		
	case RESTORE_ROLE_REQ: // 恢复被删除的高级角色
		iRet = cs_db_will_restore_role(pstAppCtx, pstEnv, pstFrameHead, 
		                               &pstCsCtrlReq->CsCmd.RestoreRoleReq);
		break;

	case DEL_ROLE_MAIL_REQ: // 删除一封邮件
		iRet = cs_role_del_mail(pstAppCtx, pstEnv, pstFrameHead,
			                      &pstCsCtrlReq->CsCmd.DelMailReq);
		break;

	case SEND_MAIL_REQ: // 发送一封系统邮件给角色
		iRet = cs_role_send_sys_mail(pstAppCtx, pstEnv, pstFrameHead,
			                           &pstCsCtrlReq->CsCmd.SendMailReq);
		break;

	case BLOCK_ACCOUNT_REQ: // 踢帐号下线
		iRet = cs_block_account(pstAppCtx, pstEnv, pstFrameHead,
			                      &pstCsCtrlReq->CsCmd.BlockAccountReq);
		break;

	case SILENCE_ACCOUNT_REQ: // 禁言帐号
		iRet = cs_silence_account(pstAppCtx, pstEnv, pstFrameHead,
			                        &pstCsCtrlReq->CsCmd.SilenceAccountReq);
		break;

	case COPY_ROLE_REQ: // 复制角色
		iRet = cs_select_role_for_copying(pstAppCtx, pstEnv, pstFrameHead, 
			                              &pstCsCtrlReq->CsCmd.CopyRoleReq);
		break;

	case GET_BULLETIN_LIST_REQ: // 获取公告列表
		iRet = cs_get_bulletin_list(pstAppCtx, pstEnv, pstFrameHead, 
			                          &pstCsCtrlReq->CsCmd.GetBulletinListReq);
		break;

	case ADD_BULLETIN_REQ: // 添加一条公告信息
		iRet = cs_add_bulletin(pstAppCtx, pstEnv, pstFrameHead,
			                     &pstCsCtrlReq->CsCmd.AddBulletinReq);
		break;

	case DEL_BULLETIN_REQ: // 删除一条公告信息
		iRet = cs_del_bulletin(pstAppCtx, pstEnv, pstFrameHead,
			                     &pstCsCtrlReq->CsCmd.DelBulletinReq);
		break;

	case CREATE_ITEM_REQ: // 创建物品
		iRet = cs_create_role_item(pstAppCtx, pstEnv, pstFrameHead,
			                         &pstCsCtrlReq->CsCmd.CreateRoleItemReq);
		break;

	case ADD_ROLE_DESIGNATION_REQ: // 添加一个称号
		iRet = cs_role_info(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                    pstCsCtrlReq->CsCmd.AddRoleDesignationReq.RoleName);
		break;

	case DEL_ROLE_DESIGNATION_REQ: // 删除一个称号
		iRet = cs_role_info(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                    pstCsCtrlReq->CsCmd.DelRoleDesignationReq.RoleName);
		break;

	case GET_ROLE_DESIGNATION_LIST_REQ: // 列出称号列表
		iRet = cs_role_info(pstAppCtx,pstEnv, &stPkgReq, pstFrameHead,
		                pstCsCtrlReq->CsCmd.GetRoleDesignationListReq.RoleName);
		break;

	case SET_ROLE_RESPACT_REQ: // 设置/添加声望
		iRet = cs_role_info(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                    pstCsCtrlReq->CsCmd.SetRoleRespactReq.RoleName);
		break;

	case DEL_ROLE_RESPACT_REQ: // 删除一个声望数据
		iRet = cs_role_info(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                    pstCsCtrlReq->CsCmd.DelRoleRespactReq.RoleName);
		break;

	case GET_ROLE_RESPACT_LIST_REQ: // 列出声望列表
		iRet = cs_role_info(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                    pstCsCtrlReq->CsCmd.GetRoleRespactListReq.RoleName);
		break;

	case GET_CLAN_ALL_INFO_REQ: // 获取指定公会的全部信息
		iRet = cs_get_clan_all_info(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                            &pstCsCtrlReq->CsCmd.GetClanAllInfoReq);
		break;

	case DEL_CLAN_MEMBER_REQ: // 从指定公会中删除一个指定的成员
		iRet = cs_del_clan_member(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                          &pstCsCtrlReq->CsCmd.DelClanMemberReq);
		break;

	case DISBAND_CLAN_REQ: // 解散一个指定的公会
		iRet = cs_disband_clan(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                       &pstCsCtrlReq->CsCmd.DisbandClanReq);
		break;

	case CLEAR_CLAN_DIPLOMATISM_REQ: // 清除一个指定公会的外交关系
		iRet = cs_clear_clan_diplomatism(pstAppCtx, pstEnv, &stPkgReq,
		                                 pstFrameHead,
		                          &pstCsCtrlReq->CsCmd.ClearClanDiplomatismReq);
		break;
	case FIND_CLAN_REQ: // 模糊查询公会
		iRet = cs_db_get_clan_list(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                           &pstCsCtrlReq->CsCmd.FindClanReq);
		break;
	case DEL_CLAN_DIPLOMATISM_REQ: // 删除指定的公会关系
		iRet = cs_del_clan_diplomatism(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead,
		                            &pstCsCtrlReq->CsCmd.DelClanDiplomatismReq);
		break;
	case EXCHANGE_CLAN_LEADER_REQ: // 将公会会长与某一成员交换
		iRet = cs_exchange_clan_leader(pstAppCtx, pstEnv, &stPkgReq,
		                               pstFrameHead,
		                            &pstCsCtrlReq->CsCmd.ExchangeClanLeaderReq);
		break;
	case GET_ROLE_PACKAGE_ITEM_REQ:	// 获取玩家指定背包物品信息
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.GetPackageItemInfoReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case GET_ROLE_DROP_REQ:
		if(cs_role_drop_info(pstAppCtx, pstEnv, &stPkgReq, pstFrameHead, pstCsCtrlReq->CsCmd.GetRoleDropReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case ROLE_DROP_RESTORE_REQ:
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.RoleDropRestoreReq.RoleName) < 0)
		{
			return -1;
		}
		break;
	case ROLE_HATCH_PET_REQ:
		if(cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.RoleHatchPetReq.RoleName) < 0)
		{
			return -1;
		}
		break;
		
	case SET_CLAN_BASE_ATTR_REQ: // 设置一个公会的基本资源,金,定,繁,科
		iRet = cs_set_clan_base_attr_req(pstAppCtx, pstEnv, &stPkgReq,
		                                 pstFrameHead,
		                          &pstCsCtrlReq->CsCmd.SetClanBaseAttrReq);
		break;

	case RELOAD_SERVER_REQ: // 指定服务器reload
		iRet = cs_reload_server_req(pstAppCtx, pstEnv, &stPkgReq,
		                                 pstFrameHead,
		                          &pstCsCtrlReq->CsCmd.ReloadServerReq);
		break;
	case SET_SECOND_PWD_REQ:
		iRet = cs_role_info(pstAppCtx,pstEnv,&stPkgReq,pstFrameHead, pstCsCtrlReq->CsCmd.SetSecondPwdReq.RoleName);
		break;
	default:
		break;
	}
	
	return iRet;
}


void roledata_to_baseinfo(ROLEDATA*pstRoleData, ZONEROLEBASEINFO*pstBaseInfo)
{
	if(!pstBaseInfo || !pstRoleData)
	{
		return;
	}
	pstBaseInfo->BlockEnd = pstRoleData->BlockEnd;
	pstBaseInfo->Career = pstRoleData->Career;
	pstBaseInfo->CreateTime = pstRoleData->CreateTime;
	pstBaseInfo->Face = pstRoleData->Face;
	pstBaseInfo->Gender = pstRoleData->Gender;
	pstBaseInfo->Hair = pstRoleData->Hair;
	pstBaseInfo->Head = pstRoleData->Head;
	pstBaseInfo->LastLogin = pstRoleData->LastLogin;
	pstBaseInfo->LastLogout = pstRoleData->LastLogout;
	pstBaseInfo->Level = pstRoleData->Level;
	pstBaseInfo->Map = pstRoleData->Map;
	pstBaseInfo->Pos = pstRoleData->Pos;
	pstBaseInfo->RoleID = pstRoleData->RoleID;
	strcpy(pstBaseInfo->RoleName, pstRoleData->RoleName);
	pstBaseInfo->SilenceEnd = pstRoleData->SilenceEnd;
	pstBaseInfo->Uin = pstRoleData->Uin;
	
}

void baseinfo_to_roledata(ROLEDATA*pstRoleData, ZONEROLEBASEINFO*pstBaseInfo)
{
	if(!pstBaseInfo || !pstRoleData)
	{
		return;
	}
	pstRoleData->BlockEnd = pstBaseInfo->BlockEnd ;
	pstRoleData->Career = pstBaseInfo->Career;
	pstRoleData->CreateTime = pstBaseInfo->CreateTime;
	pstRoleData->Face = pstBaseInfo->Face;
	pstRoleData->Gender = pstBaseInfo->Gender;
	pstRoleData->Hair = pstBaseInfo->Hair;
	pstRoleData->Head = pstBaseInfo->Head;
	pstRoleData->LastLogin = pstBaseInfo->LastLogin;
	pstRoleData->LastLogout = pstBaseInfo->LastLogout;
	pstRoleData->Level = pstBaseInfo->Level;
	pstRoleData->Map = pstBaseInfo->Map;
	pstRoleData->Pos = pstBaseInfo->Pos;
	//pstRoleData->RoleID = pstBaseInfo->RoleID;
	//strcpy(pstRoleData->RoleName, pstBaseInfo->RoleName);
	pstRoleData->SilenceEnd = pstBaseInfo->SilenceEnd;
	//pstRoleData->Uin = pstBaseInfo->Uin;
}

int cs_package_item_info_by_type(ROLEDATA *pstRoleData, int iListType,  SSGETPACKAGEITEMINFO *pstGetPackageItemInfo)
{
	ROLEPACKAGE *pstPak = &pstRoleData->Package;

	switch(iListType)
	{
		case LIST_TYPE_STASH:
			pstGetPackageItemInfo->StashPackage =  pstPak->StashPackage;
			break;
		case LIST_TYPE_SUNDRIES:
			pstGetPackageItemInfo->SundriesPackage =pstPak->SundriesPackage;
			break;
		case LIST_TYPE_STUFF:
			pstGetPackageItemInfo->StuffPackage = pstPak->StuffPackage;
			break;
		case LIST_TYPE_TASK:
			pstGetPackageItemInfo->TaskPackage = pstPak->TaskPackage;
			break;
		case LIST_TYPE_SHOPPING:
			pstGetPackageItemInfo->ShoppingPackage = pstPak->ShoppingPackage;
			break;
		default:
			return 0;
			break;
	}
	
	return 0;
}

int cs_get_role_info_to_clt(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn,TORMPKG* pstPkg)
{
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	
	ROLEDATA stRoleData;
	//int iRet = 0;
	SSPKG stSSPkg;

	SKILLINFO *pstSkillInfo = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleSkillInfoRes.RoleSkillInfo;
	//FITTINGSINFO *pstFittingfInfo = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleFittingsRes.RoleFittingsInfo;
	ROLEPACKAGE *pstPack = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleItemListRes.ZoneRolePackage;
	ROLEWEAR *pstWear = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleItemListRes.ZoneRoleWear;
	DIATHESISINFO *pstDiathesis = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleDiathesisRes.ZoneDiathesisInfo;
	//Player *pstPlayer;

	ZONEGETROLEDETAILRES *pstGetRoleDetailRes = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleDetailRes;
	ROLEDETAIL *pstRoleDetail = &pstGetRoleDetailRes->RoleDetailInfo;
	TASKINFO *pstTaskInfo = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleTaskRes.TaskInfo;
	ROLERIDELIST *pstRideInfo = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleRideInfoRes.RoleRideInfo;
	QIYUEINFO *pstQiyueInfo = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleQiYueListRes.QiYueInfo;
	PETPACKAGE *pstPetInfo = &stSSPkg.Body.CsCtrL.CsCmd.GetRolePetInfoRes.RolePetInfo;
	ZONEGETROLEPACKAGERES *pstGetRolePackRes = &stSSPkg.Body.CsCtrL.CsCmd.GetRolePackageRes;
	ZONEGETPACKAGEITEMINFORES *pstGetPackageItemInfoRes = &stSSPkg.Body.CsCtrL.CsCmd.GetPackageItemInfoRes;

	ZONEGETFIRENDRES *pstFriendRes =  &stSSPkg.Body.CsCtrL.CsCmd.FirendRes;
	ZONEGETBLACKRES *pstBlackRes =  &stSSPkg.Body.CsCtrL.CsCmd.BlackRes;
	GETSAFELOCKRES *pstGetSafeRes = &stSSPkg.Body.CsCtrL.CsCmd.GetSafeLockRes;

	
	if (TORM_ERR_IS_ERROR(pstPkg->Body.SelectRes.ErrNo))
	{
		return -1;
	}
	if (1 != pstPkg->Body.SelectRes.SelectResult.ResultNum)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleData, sizeof(stRoleData)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		return -1;
	}

	memset(&stSSPkg, 0, sizeof(stSSPkg));
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);

	
	if(GET_ROLE_DETAIL_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_DETAIL_RES;
	
		memcpy(pstRoleDetail,&stRoleData.RoleDetail,sizeof(ROLEDETAIL));
		pstRoleDetail->StashMoney = stRoleData.Package.StashPackage.Money;
		pstGetRoleDetailRes->StashMoney = stRoleData.Package.StashPackage.Money;
		pstGetRoleDetailRes->StashBull = stRoleData.Package.StashPackage.Bull;
		//pstRoleDetail->Bull = stRoleData.Package.StashPackage.Bull;
		
		pstGetRoleDetailRes->IsOnline = 0;
		pstGetRoleDetailRes->X = stRoleData.Pos.X;
		pstGetRoleDetailRes->Y = stRoleData.Pos.Y;
		pstGetRoleDetailRes->MapID = stRoleData.Map;
		pstGetRoleDetailRes->Perm = stRoleData.Perm;
		pstGetRoleDetailRes->CreateTime = stRoleData.CreateTime;
		pstGetRoleDetailRes->DelTime = stRoleData.DelTime;
		pstGetRoleDetailRes->Uin = stRoleData.Uin;
		pstGetRoleDetailRes->RoleID = stRoleData.RoleID;
		pstGetRoleDetailRes->Level = stRoleData.Level;
		pstGetRoleDetailRes->Career = stRoleData.Career;
		pstGetRoleDetailRes->Gender = stRoleData.Gender;
		pstGetRoleDetailRes->ClanID = stRoleData.MiscInfo.ClanInfo.ClanId.Id;

		pstGetRoleDetailRes->BlockStart = stRoleData.BlockStart;
		pstGetRoleDetailRes->SilenceStart = stRoleData.SilenceStart;

		pstGetRoleDetailRes->PreLoginTime = stRoleData.LastLogin;
		pstGetRoleDetailRes->LastLogoutTime = stRoleData.LastLogout;
		
		STRNCPY(pstGetRoleDetailRes->ClanName, stRoleData.MiscInfo.ClanInfo.ClanId.Name,
				sizeof(pstGetRoleDetailRes->ClanName));
		STRNCPY(pstGetRoleDetailRes->RoleName, stRoleData.RoleName, sizeof(pstGetRoleDetailRes->RoleName));

		pstGetRoleDetailRes->AttrVal = stRoleData.MiscInfo.AttrVal;
		pstGetRoleDetailRes->GeniusInfo = stRoleData.MiscInfo.GeniusInfo;

		if (stRoleData.BlockEnd > 0)
		{
			if (stRoleData.BlockEnd - pstAppCtx->stCurr.tv_sec > 0)
			{
				pstGetRoleDetailRes->Block = stRoleData.BlockEnd - pstAppCtx->stCurr.tv_sec ;
			}
			else
			{
				pstGetRoleDetailRes->Block = 0;
			}
		}
		else
		{
			pstGetRoleDetailRes->Block = stRoleData.BlockEnd;
		}
		
		if (stRoleData.SilenceEnd > 0)
		{
			if (stRoleData.SilenceEnd - pstAppCtx->stCurr.tv_sec > 0)
			{
				pstGetRoleDetailRes->Silence = stRoleData.SilenceEnd - pstAppCtx->stCurr.tv_sec;
			}
			else 
			{
				pstGetRoleDetailRes->Silence = 0;
			}
		}
		else
		{
			pstGetRoleDetailRes->Silence = stRoleData.SilenceEnd;
		}
		
	
	}
	else if(GET_ROLE_SKILLINFO_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_SKILLINFO_RES;
		
		memcpy(pstSkillInfo,&stRoleData.SkillInfo,sizeof(SKILLINFO));
		
	}
	/*else if(GET_ROLE_FITTING_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_FITTING_RES;
		
		memcpy(pstFittingfInfo, &stRoleData.FittingsInfo, sizeof(FITTINGSINFO));
		
	}*/
	else if(GET_ROLE_ITEMLIST_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_ITEMLIST_RES;
	
		memcpy(pstPack,&stRoleData.Package,sizeof(ROLEPACKAGE));
		memcpy(pstWear,&stRoleData.Wear,sizeof(ROLEWEAR));
		
	}
	else if(GET_ROLE_DIATHESIS_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_DIATHESIS_RES;
		
		memcpy(pstDiathesis, &stRoleData.DiathesisInfo, sizeof(DIATHESISINFO));
		
	}
	else if (GET_ROLE_TASK_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_TASK_RES;
		memcpy(pstTaskInfo, &stRoleData.MiscInfo.TaskInfo, sizeof(TASKINFO));
	}
	else if(GET_ROLE_RIDE_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_RIDE_RES;
		memcpy(pstRideInfo,&stRoleData.MiscInfo.RideList,sizeof(ROLERIDELIST));
	}
	else if(GET_ROLE_PACKAGE_REQ == pstAsyn->Cmd)
	{
		ZONEGETROLEPACKAGEREQ stReq;

		if(pstAsyn->iLen != sizeof(ZONEGETROLEPACKAGEREQ))
		{
			return -1;
		}
		
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);
		
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_PACKAGE_RES;

		pstGetRolePackRes->ZoneRolePackageData.ListType = stReq.ListType;
		pstGetRolePackRes->ZoneRolePackageData.MaxNum = cs_package_maxnum_by_type(&stRoleData, stReq.ListType);
	}
	else if(GET_ROLE_QIYUELIST_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_QIYUELIST_RES;
		memcpy(pstQiyueInfo,&stRoleData.Package.QiYueInfo,sizeof(QIYUEINFO));
	}	
	else if(GET_ROLE_PET_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_PET_RES;
		memcpy(pstPetInfo,&stRoleData.Package.PetPackage,sizeof(PETPACKAGE));
	}
	else if (GET_ROLE_FRIEND_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_FRIEND_RES;
		pstFriendRes->RoleBuddy = stRoleData.Buddy;
	}
	else if (GET_ROLE_BLACK_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_BLACK_RES;
		pstBlackRes->BlackNum = stRoleData.MiscInfo.BlackNum;
		pstBlackRes->EnemyNum = stRoleData.MiscInfo.Enemy.EnemyNum;
		memcpy(pstBlackRes->Blacks,stRoleData.MiscInfo.BlackMember, sizeof(pstBlackRes->Blacks));
		memcpy(pstBlackRes->Enemys,stRoleData.MiscInfo.Enemy.Enemys, sizeof(pstBlackRes->Enemys));
	}
	else if(GET_ROLE_PACKAGE_ITEM_REQ == pstAsyn->Cmd)
	{
		ZONEGETPACKAGEITEMINFOREQ stReq;

		if(pstAsyn->iLen != sizeof(ZONEGETPACKAGEITEMINFOREQ))
		{
			return -1;
		}
		
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);
	
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_PACKAGE_ITEM_RES;
		pstGetPackageItemInfoRes->ListType = stReq.ListType;
		cs_package_item_info_by_type(&stRoleData, stReq.ListType,&pstGetPackageItemInfoRes->GetPackageItemInfo);
	}
	else if (GET_SAFELOCK_REQ == pstAsyn->Cmd)
	{
		stSSPkg.Body.CsCtrL.Cmd = GET_SAFELOCK_RES;
		strncpy(pstGetSafeRes->RoleName, stRoleData.RoleName, sizeof(pstGetSafeRes->RoleName));
		pstGetSafeRes->SafeLockPassWd[0] = 0;
		if (stRoleData.MiscInfo.LockPassWD[0] != 0)
		{
			strncpy(pstGetSafeRes->SafeLockPassWd, 
				stRoleData.MiscInfo.LockPassWD,
				sizeof(pstGetSafeRes->SafeLockPassWd));
		}
	}
	else if (GET_LIFT_SKILL_REQ == pstAsyn->Cmd)
	{
		GETLIFTSKILLRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.GetLiftSkillRes;

		stSSPkg.Body.CsCtrL.Cmd = GET_LIFT_SKILL_RES;
		strncpy(pstRes->RoleName, stRoleData.RoleName, sizeof(pstRes->RoleName));
		pstRes->LiftSkillInfo = stRoleData.MiscInfo.SecondarySkillMiscInfo;
	}
	else
	{
		return -1;
	}

	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -2;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff,pstAsyn->ConnIdx,&pstAsyn->IPInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -2;
	}
	
	return 0;
}

int set_skill_info(CS_SVRENV*pstEnv,ROLEDATA *pstRoleData,SKILLINFO *pstSkillInfo, ZONESETROLESKILLINFOREQ *pstReq)
{
	int i;
	int iPos;
	SKILLDATA *pstSkillData = NULL;
	SKILLDEF *pstSkillDef;// = cs_find_skilldef(pstEnv,pstReq->ZoneSkillData.SkillID,pstReq->ZoneSkillData.SkillLevel);

	if(pstSkillInfo == NULL ||pstReq == NULL ||!pstRoleData)
	{
		return -1;
	}

	pstSkillDef = cs_find_skilldef(pstEnv,pstReq->ZoneSkillData.SkillID,pstReq->ZoneSkillData.SkillLevel);
	if (!pstSkillDef)
	{
		return -4;
	}

	//if ( CAREER_NON != pstSkillDef->Learn.Career && pstRoleData->Career != pstSkillDef->Learn.Career) 
	if ( (CAREER_NON != pstSkillDef->Learn.Career && (pstRoleData->Career&0x0F) != (pstSkillDef->Learn.Career&0x0F)) ||
		 (CAREER_NON != (pstSkillDef->Learn.Career&0xF0) && (pstRoleData->Career&0xF0) != (pstSkillDef->Learn.Career&0xF0)) )
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "此技能与你职业不符");
		return -4;
	}

	
	if(pstSkillInfo->SkillNum > MAX_ROLE_SKILL /*|| (int)pstSkillInfo->SkillNum < 0*/)
	{
		return -1;
	}
	for(i=0; i<(int)pstSkillInfo->SkillNum; i++)
	{
		if(pstSkillInfo->SkillList[i].SkillID == pstReq->ZoneSkillData.SkillID)
		{
			pstSkillData = &pstSkillInfo->SkillList[i];
			iPos = i;
			break;
		}
	}
	
	if(pstReq->Type == ROLE_SKILL_ADD)
	{
		if(pstSkillData || pstSkillInfo->SkillNum == MAX_ROLE_SKILL)
		{
			return -1;
		}
		memset( ((char*)&(pstReq->ZoneSkillData))+offsetof(SKILLDATA, Res), 0,sizeof(SKILLDATA) - offsetof(SKILLDATA, Res) );
		memcpy(pstSkillInfo->SkillList + pstSkillInfo->SkillNum,&pstReq->ZoneSkillData, sizeof(SKILLDATA));
		//pstSkillInfo->SkillList[pstSkillInfo->SkillNum].FittingsMap.FittingsMapRowNum = pstSkillDef->FittingsGridRowNum;
		pstSkillInfo->SkillNum ++;
	}
	else if(pstReq->Type == ROLE_SKILL_DEL)
	{
		if(pstSkillData == NULL ||pstSkillInfo->SkillNum == 0)
		{
			return -1;
		}

		pstSkillInfo->SkillNum --;
		if (iPos != pstSkillInfo->SkillNum)
		{
			memcpy(pstSkillInfo->SkillList + iPos,pstSkillInfo->SkillList + pstSkillInfo->SkillNum, sizeof(SKILLDATA));
		}
	}
	else if(pstReq->Type == ROLE_SKILL_MODIFY)
	{
		if(pstSkillData == NULL)
		{
			return -1;
		}

		pstSkillData = pstSkillInfo->SkillList + iPos;
		pstSkillData->SkillLevel = pstReq->ZoneSkillData.SkillLevel;
		//pstSkillData->FittingsMap.FittingsMapRowNum = pstSkillDef->FittingsGridRowNum;
		//memcpy(pstSkillInfo->SkillList + iPos,&pstReq->ZoneSkillData, sizeof(SKILLDATA));
	}
	else 
	{
		return -1;
	}
	return 0;
}

RIDEDEF *cs_find_ridedef(CS_SVRENV*pstEnv, int iRideID, int iRideLevel)
{
	RIDEDEF  stRideDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stRideDef.RideID= iRideID;
	stRideDef.Level = iRideLevel;
	iIndex = bsearch_int(&stRideDef, pstObj->sRideDef, pstObj->iRideDef, RIDE_DEF_DATA , &iEqu);
	if (iEqu)
	{
		return (RIDEDEF *)pstObj->sRideDef[iIndex];
	}
	
	return NULL;	
}

PETLEVELDEF *cs_find_petleveldef(CS_SVRENV*pstEnv, int iLevel)
{
	PETLEVELDEF  stPetLevelDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stPetLevelDef.Level = iLevel;
	iIndex = bsearch_int(&stPetLevelDef, pstObj->sPetLevelDef, pstObj->iPetLevelDef, PETLEVEL_DATA , &iEqu);
	if (iEqu)
	{
		return (PETLEVELDEF *)pstObj->sPetLevelDef[iIndex];
	}
	
	return NULL;	
}

PETDEF *cs_find_petdef(CS_SVRENV*pstEnv, int iPetID)
{
	PETDEF  stPetDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stPetDef.PetID= iPetID;
	iIndex = bsearch_int(&stPetDef, pstObj->sPetDef, pstObj->iPetDef, PET_DATA , &iEqu);
	if (iEqu)
	{
		return (PETDEF *)pstObj->sPetDef[iIndex];
	}
	
	return NULL;	
}

int set_pet_info(CS_SVRENV*pstEnv,PETPACKAGE *pstPetPackage, ZONESETROLEPETINFOREQ *pstReq)
{
	int i;
	int iPos;
	ROLEPET *pstRolePet = NULL;
	PETDEF *pstPetDef;

	if(pstPetPackage == NULL ||pstReq == NULL)
	{
		return -1;
	}

	pstPetDef = cs_find_petdef(pstEnv,pstReq->ZoneRolePetData.PetID);
	//if (!pstPetDef)
	//{
	//	return -1;
	//}
	
	if(pstPetPackage->MaxNum > MAX_PET_PACKAGE)
	{
		return -1;
	}
	
	for(i=0; i<(int)pstPetPackage->Num; i++)
	{
		if(pstPetPackage->RolePets[i].WID == pstReq->ZoneRolePetData.WID)
		{
			pstRolePet = &pstPetPackage->RolePets[i];
			iPos = i;
			break;
		}
	}
	
	if(pstReq->Type == ROLE_PET_ADD)
	{
		if(pstRolePet || pstPetPackage->Num >= pstPetPackage->MaxNum ||pstPetPackage->Num >= MAX_PET_PACKAGE)
		{
			return -1;
		}
		
		memcpy(&pstPetPackage->RolePets[pstPetPackage->Num], &pstReq->ZoneRolePetData, sizeof(ROLEPET));
		pstPetPackage->Num++;
	}
	else if(pstReq->Type == ROLE_PET_DEL)
	{
		if(pstRolePet == NULL ||pstPetPackage->Num == 0)
		{
			return -1;
		}

		pstPetPackage->Num--;
		if (iPos != pstPetPackage->Num)
		{
			memcpy(&pstPetPackage->RolePets[iPos], &pstPetPackage->RolePets[pstPetPackage->Num], sizeof(ROLEPET));
		}
	}
	else if(pstReq->Type == ROLE_PET_MODIFY)
	{
		if(pstRolePet == NULL)
		{
			return -1;
		}

		memcpy(&pstPetPackage->RolePets[iPos], &pstReq->ZoneRolePetData, sizeof(ROLEPET));
	}
	else 
	{
		return -1;
	}
	
	return 0;
}

static inline DESIGNATIONENTRY* find_role_designation(
	ROLEDESIGNATIONS *pstInfo,
	int iID)
{
	int i = pstInfo->Num;
	while(i--)
	{
		if((int)pstInfo->Designations[i].ID == iID)
		{
			return &pstInfo->Designations[i];
		}
	}
	return NULL;
}

static int add_role_designation_req(CS_SVRENV *pstEnv, CS_ASYN *pstAsyn,
                                    ROLEDESIGNATIONS *pstInfo,
                                    const ZONEADDROLEDESIGNATIONREQ *pstReq)
{
	int iRet = 0;
	SSPKG stPkg;
	DESIGNATIONENTRY *pstEntry = find_role_designation(pstInfo,
	                                                   pstReq->DesignationID);
	if(pstEntry || pstInfo->Num >= MAX_ROLE_DESIGNATION)
	{
		goto error;
	}

	pstEntry = &pstInfo->Designations[pstInfo->Num++];
	pstEntry->ID = pstReq->DesignationID;
	pstEntry->LastUseTime = 0;

	
final:
	return iRet;
	
error:
	iRet = -1;
	cs_init_sshead(&stPkg.Head, CS_CTRL);
	stPkg.Body.CsCtrL.Cmd = ADD_ROLE_DESIGNATION_RES;
	stPkg.Body.CsCtrL.CsCmd.AddRoleDesignationRes.Succeeded = 0;
	cs_sendto_clt_pkg(pstEnv, pstAsyn, &stPkg);
	goto final;
	return 0;
}

static int del_role_designation_req(CS_SVRENV *pstEnv, CS_ASYN *pstAsyn,
                                    ROLEDESIGNATIONS *pstInfo,
                                    const ZONEDELROLEDESIGNATIONREQ *pstReq)
{
	int iRet = 0, iIdx = 0;
	SSPKG stPkg;
	DESIGNATIONENTRY *pstEntry = find_role_designation(pstInfo,
	                                                   pstReq->DesignationID);
	if(!pstEntry)
	{
		goto error;
	}

	iIdx = (int)(pstEntry - pstInfo->Designations);
	if(iIdx < (int)pstInfo->Num - 1)
	{
		memcpy(pstEntry, &pstInfo->Designations[pstInfo->Num - 1],
		       sizeof(*pstEntry));
	}

	if((int)pstInfo->CurrentDesignationID == pstReq->DesignationID)
	{
		pstInfo->CurrentDesignationID = 0;
	}

	pstInfo->Num--;

final:
	return iRet;

error:
	iRet = -1;
	cs_init_sshead(&stPkg.Head, CS_CTRL);
	stPkg.Body.CsCtrL.Cmd = DEL_ROLE_DESIGNATION_RES;
	stPkg.Body.CsCtrL.CsCmd.DelRoleDesignationRes.Succeeded = 0;
	cs_sendto_clt_pkg(pstEnv, pstAsyn, &stPkg);
	goto final;
	return 0;
}

static int get_role_designation_list_req(
	CS_SVRENV *pstEnv,
	CS_ASYN *pstAsyn,
	ROLEDESIGNATIONS *pstInfo,
	const ZONEGETROLEDESIGNATIONLISTREQ *pstReq)
{
	SSPKG stPkg;
	ZONEGETROLEDESIGNATIONLISTRES *pstRes = 
		&stPkg.Body.CsCtrL.CsCmd.GetRoleDesignationListRes;

	((void)pstReq);

	pstRes->Succeeded = 1;
 	pstRes->Designations = *pstInfo;

	cs_init_sshead(&stPkg.Head, CS_CTRL);
	stPkg.Body.CsCtrL.Cmd = GET_ROLE_DESIGNATION_LIST_RES;
	cs_sendto_clt_pkg(pstEnv, pstAsyn, &stPkg);
	return 0;
}

static inline RESPACTENTRY* find_role_respact(RESPACTINFO *pstInfo, int iID)
{
	int i = pstInfo->RespactPartyNum;
	while(i--)
	{
		if((int)pstInfo->Respact[i].ID == iID)
		{
			return &pstInfo->Respact[i];
		}
	}
	return NULL;
}

static int set_role_respact_req(CS_SVRENV *pstEnv, CS_ASYN *pstAsyn,
                                RESPACTINFO *pstInfo,
                                const ZONESETROLERESPACTREQ *pstReq)
{
	int iRet = 0;
	SSPKG stPkg;
	RESPACTENTRY *pstEntry = find_role_respact(pstInfo, pstReq->RespactPartyID);

	if(pstEntry)
	{
		pstEntry->Value = pstReq->RespactValue;
	}
	else
	{
		if(pstInfo->RespactPartyNum >= MAX_RESPACT_PARTY_NUM)
		{
			goto error;
		}
		pstEntry = &pstInfo->Respact[pstInfo->RespactPartyNum++];
		pstEntry->ID = pstReq->RespactPartyID;
		pstEntry->Value = pstReq->RespactValue;
	}

final:
	return iRet;

error:
	iRet = -1;
	cs_init_sshead(&stPkg.Head, CS_CTRL);
	stPkg.Body.CsCtrL.Cmd = SET_ROLE_RESPACT_RES;
	stPkg.Body.CsCtrL.CsCmd.SetRoleRespactRes.Succeeded = 0;
	cs_sendto_clt_pkg(pstEnv, pstAsyn, &stPkg);
	goto final;
	return 0;
}

static int del_role_respact_req(CS_SVRENV *pstEnv, CS_ASYN *pstAsyn,
                                RESPACTINFO *pstInfo,
                                const ZONESETROLERESPACTREQ *pstReq)
{
	int iRet = 0, iIdx = 0;
	SSPKG stPkg;
	RESPACTENTRY *pstEntry = find_role_respact(pstInfo, pstReq->RespactPartyID);

	if(!pstEntry)
	{
		goto error;
	}

	iIdx = (int)(pstEntry - pstInfo->Respact);
	if(iIdx < pstInfo->RespactPartyNum - 1)
	{
		memcpy(pstEntry, &pstInfo->Respact[pstInfo->RespactPartyNum - 1],
		       sizeof(*pstEntry));
	}

final:
	return iRet;

error:
	iRet = -1;
	cs_init_sshead(&stPkg.Head, CS_CTRL);
	stPkg.Body.CsCtrL.Cmd = DEL_ROLE_RESPACT_RES;
	stPkg.Body.CsCtrL.CsCmd.DelRoleRespactRes.Succeeded = 0;
	cs_sendto_clt_pkg(pstEnv, pstAsyn, &stPkg);
	goto final;
	return 0;
}

static int get_role_respact_list_req(CS_SVRENV *pstEnv, CS_ASYN *pstAsyn,
                                     RESPACTINFO *pstInfo,
                                     const ZONEGETROLERESPACTLISTREQ *pstReq)
{
	SSPKG stPkg;
	ZONEGETROLERESPACTLISTRES *pstRes = 
		&stPkg.Body.CsCtrL.CsCmd.GetRoleRespactListRes;

	((void)pstReq);

	pstRes->Succeeded = 1;
 	pstRes->RespactInfo = *pstInfo;

	cs_init_sshead(&stPkg.Head, CS_CTRL);
	stPkg.Body.CsCtrL.Cmd = GET_ROLE_RESPACT_LIST_RES;
	cs_sendto_clt_pkg(pstEnv, pstAsyn, &stPkg);
	return 0;
}

int cs_package_enlarge(ROLEDATA *pstRoleData, ZONESETROLEPACKAGEREQ *pstReq)
{
	int iOld;
	int iTotal;
	ROLEPACKAGE *pstPak = &pstRoleData->Package;

	if (pstReq->Num <= 0)
	{
		return -1;
	}
	
	switch(pstReq->ListType)
	{
		case LIST_TYPE_STUFF:
			iOld = pstPak->StuffPackage.MaxNum;
			if (iOld+pstReq->Num>MAX_STUFF_PACKAGE)
			{
				pstPak->StuffPackage.MaxNum = MAX_STUFF_PACKAGE;
			}
			else
			{
				pstPak->StuffPackage.MaxNum = iOld+pstReq->Num;
			}

			iTotal = pstPak->StuffPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->StuffPackage.RoleItems+iOld, 0, sizeof(pstPak->StuffPackage.RoleItems[0])*(iTotal-iOld));
			}
			break;
		case LIST_TYPE_TASK:
			iOld = pstPak->TaskPackage.MaxNum;
			if (iOld+pstReq->Num>MAX_TASK_PACKAGE)
			{
				pstPak->TaskPackage.MaxNum = MAX_TASK_PACKAGE;
			}
			else
			{
				pstPak->TaskPackage.MaxNum = iOld+pstReq->Num;
			}

			iTotal = pstPak->TaskPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->TaskPackage.RoleItems+iOld, 0, sizeof(pstPak->TaskPackage.RoleItems[0])*(iTotal-iOld));
			}
			break;
		case LIST_TYPE_SUNDRIES:
			iOld = pstPak->SundriesPackage.MaxNum;
			if (iOld+pstReq->Num>MAX_SUNDRIES_PACKAGE)
			{
				pstPak->SundriesPackage.MaxNum = MAX_SUNDRIES_PACKAGE;
			}
			else
			{
				pstPak->SundriesPackage.MaxNum = iOld+pstReq->Num;
			}

			iTotal = pstPak->SundriesPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->SundriesPackage.RoleGrids+iOld, 0, sizeof(pstPak->SundriesPackage.RoleGrids[0])*(iTotal-iOld));
			}
			break;
		case LIST_TYPE_STASH:
			iOld = pstPak->StashPackage.MaxNum;
			if (iOld+pstReq->Num>MAX_STASH_PACKAGE)
			{
				pstPak->StashPackage.MaxNum = MAX_STASH_PACKAGE;
			}
			else
			{
				pstPak->StashPackage.MaxNum = iOld+pstReq->Num;
			}

			iTotal = pstPak->StashPackage.MaxNum ;
			if (iTotal > iOld)
			{
				memset(pstPak->StashPackage.RoleGrids+iOld, 0, sizeof(pstPak->StashPackage.RoleGrids[0])*(iTotal-iOld));
			}
			break;
		default:
			return -1;
			break;
	}
		
	return 0;
}

int set_ride_info(CS_SVRENV*pstEnv,ROLERIDELIST *pstRideInfo, ZONESETROLERIDEINFOREQ *pstReq)
{
	int i;
	int iPos;
	RIDEGRID *pstRideData = NULL;
	RIDEDEF *pstRideDef;

	if(pstRideInfo == NULL ||pstReq == NULL)
	{
		return -1;
	}

	pstRideDef = cs_find_ridedef(pstEnv,pstReq->ZoneRoleRideData.ItemID, pstReq->ZoneRoleRideData.Level);
	if (!pstRideDef)
	{
		return -1;
	}
	
	if(pstRideInfo->MaxNum > MAX_RIDE_GRID)
	{
		return -1;
	}
	
	for(i=0; i<(int)pstRideInfo->Num; i++)
	{
		if(pstRideInfo->RideGrids[i].ItemID == pstReq->ZoneRoleRideData.ItemID)
		{
			pstRideData = &pstRideInfo->RideGrids[i];
			iPos = i;
			break;
		}
	}
	
	if(pstReq->Type == ROLE_RIDE_ADD)
	{
		if(pstRideData || pstRideInfo->MaxNum == MAX_RIDE_GRID)
		{
			return -1;
		}
		
		memcpy(&pstRideInfo->RideGrids[pstRideInfo->Num], &pstReq->ZoneRoleRideData, sizeof(RIDEGRID));
		pstRideInfo->Num++;
	}
	else if(pstReq->Type == ROLE_RIDE_DEL)
	{
		if(pstRideData == NULL ||pstRideInfo->Num == 0)
		{
			return -1;
		}

		

		pstRideInfo->Num--;
		if (iPos != pstRideInfo->Num)
		{
			memcpy(&pstRideInfo->RideGrids[iPos], &pstRideInfo->RideGrids[pstRideInfo->Num], sizeof(RIDEGRID));
		}
	}
	else if(pstReq->Type == ROLE_RIDE_MODIFY)
	{
		if(pstRideInfo == NULL)
		{
			return -1;
		}

		memcpy(&pstRideInfo->RideGrids[iPos], &pstReq->ZoneRoleRideData, sizeof(RIDEGRID));
	}
	else 
	{
		return -1;
	}
	
	return 0;
}

/*
int set_fitting_info(CS_SVRENV*pstEnv,FITTINGSINFO *pstFitInfo, ZONESETROLEFITTINGSREQ *pstSetFitReq)
{
	int i;
	int iPos;
	FITTINGSDATA *pstFitDef = NULL;
	FITTINGSDEF *pstFittingdef;
	
	
	if(pstFitInfo == NULL || pstSetFitReq == NULL)
	{
		return -1;
	}
	
	pstFittingdef = cs_find_fittingdef(pstEnv, pstSetFitReq->ZoneFittingData.FittingsID, pstSetFitReq->ZoneFittingData.FittingsLevel);
	if (!pstFittingdef)
	{
		return -1;
	}
	
	if( pstFitInfo->FittingsNum > MAX_ROLE_FITTINGS)
	{
		return -1;
	}
	for(i=0;i<(int)pstFitInfo->FittingsNum;i++)
	{
		if(pstFitInfo->FittingsList[i].FittingsID == pstSetFitReq->ZoneFittingData.FittingsID)
		{
			pstFitDef = &pstFitInfo->FittingsList[i];
			iPos = i;
			break;
		}
	}
	if(pstSetFitReq->Type == ROLE_FITTING_ADD)
	{
		if(pstFitDef || pstFitInfo->FittingsNum == MAX_ROLE_FITTINGS)
		{
			return -1;
		}
		if(pstSetFitReq->ZoneFittingData.FittingsLevel < 1)
		{
			return -1;
		}
		memcpy(pstFitInfo->FittingsList+pstFitInfo->FittingsNum,&pstSetFitReq->ZoneFittingData,sizeof(FITTINGSDATA));
		pstFitInfo->FittingsNum++;
	}
	else if(pstSetFitReq->Type == ROLE_FITTING_DEL)
	{
		if(pstFitInfo->FittingsNum == 0 || !pstFitDef)
		{
			return -1;
		}

		pstFitInfo->FittingsNum --;
		if(iPos != (int)pstFitInfo->FittingsNum)
		{
			memcpy(pstFitInfo->FittingsList+iPos, pstFitInfo->FittingsList+pstFitInfo->FittingsNum, sizeof(FITTINGSDATA));
		}
	}
	else if(pstSetFitReq->Type == ROLE_FITTING_MODIFY)
	{
		if(!pstFitDef)
		{
			return -1;
		}
		memcpy(pstFitInfo->FittingsList+iPos, &pstSetFitReq->ZoneFittingData, sizeof(FITTINGSDATA));
	}
	else 
	{
		return -1;
	}
	
	return 0;
}
*/
ITEMDEF *cs_find_itemdef(CS_SVRENV*pstEnv, int iItemID)
{
	ITEMDEF  stItem;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stItem.ItemID = iItemID;
	iIndex = bsearch_int(&stItem, pstObj->sItemDef, pstObj->iItemDef, ITEM_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (ITEMDEF *)pstObj->sItemDef[iIndex];
	}
	
	return NULL;	
}

FAIRYDEF *cs_find_fairydef(CS_SVRENV*pstEnv, int iFairyID, int iFairyLv)
{
	FAIRYDEF stFairyDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stFairyDef.FairyID = iFairyID;
	stFairyDef.Level = iFairyLv;
	iIndex = bsearch_int(&stFairyDef, pstObj->sFairyDef, pstObj->iFairyDef, FAIRY_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (FAIRYDEF *)pstObj->sFairyDef[iIndex];
	}
	
	
	return NULL;
}

SKILLDEF*cs_find_skilldef(CS_SVRENV*pstEnv, int iSkillID, int iSkillLevel)
{
	SKILLDEF  stSkillDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stSkillDef.SkillID = iSkillID;
	stSkillDef.Level = iSkillLevel;
	iIndex = bsearch_int(&stSkillDef, pstObj->sSkillDef, pstObj->iSkillDef, SKILL_DATA , &iEqu);
	if (iEqu)
	{
		return (SKILLDEF *)pstObj->sSkillDef[iIndex];
	}
	
	return NULL;	
}

DIATHESISDEF*cs_find_diathesisdef(CS_SVRENV*pstEnv, int iDiathesisID, int iDiathesisLevel)
{
	DIATHESISDEF stDiathsisDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stDiathsisDef.DiathesisID = iDiathesisID;
	stDiathsisDef.Level = iDiathesisLevel;
	
	iIndex = bsearch_int(&stDiathsisDef, pstObj->sDiathesisDef, pstObj->iDiathesisDef, DIATHESIS_DATA , &iEqu);
	if (iEqu)
	{
		return (DIATHESISDEF *)pstObj->sDiathesisDef[iIndex];
	}
	
	return NULL;	
}

FITTINGSDEF*cs_find_fittingdef(CS_SVRENV*pstEnv, int iFittingID, int iFittingLevel)
{
	FITTINGSDEF stFitting;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stFitting.FittingsID = iFittingID;
	stFitting.Level = iFittingLevel;
	stFitting.GroupID = 1;
	
	iIndex = bsearch_int(&stFitting, pstObj->sFittingsDef, pstObj->iFittingsDef, FITTINGS_DATA , &iEqu);
	if (iEqu)
	{
		return (FITTINGSDEF *)pstObj->sFittingsDef[iIndex];
	}
	
	return NULL;	
}


int find_wear_item_by_grididx(ROLEWEAR *pstRoleWear,ZONESETROLEITEMLISTREQ *pstSetItemReq,ROLEITEM *pstRoleItem,int *pIpos)
{
	int i;
	int iType = pstSetItemReq->PackType;
	int iFindIndx = pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.GridIdx;
	*pIpos = -1;

	if(iType == LIST_TYPE_WEAR)
	{
		for(i=0; i<pstRoleWear->ArmNum; i++)
		{
			if(pstRoleWear->RoleArms[i].GridIdx == iFindIndx 
				&&pstRoleWear->RoleArms[i].WID == pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID)
			{
				memcpy(pstRoleItem,&pstRoleWear->RoleArms[i],sizeof(ROLEITEM));
				*pIpos = i;
				break;
			}
		}
	}
	else 
	{
		return -1;
	}
	return 0;
}

int find_pack_item_by_grididx(ROLEPACKAGE *pstPackge,ZONESETROLEITEMLISTREQ *pstSetItemReq,ROLEITEM *pstRoleItem,int *pIpos)
{
	int i;
	int iType = pstSetItemReq->PackType;
	int iFindIndx = pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.GridIdx;
	*pIpos = -1;
	
	if(iType == LIST_TYPE_STASH)
	{
		for(i=0;i<pstPackge->StashPackage.Num; i++)
		{
			if(pstPackge->StashPackage.RoleGrids[i].GridData.RoleItem.GridIdx == iFindIndx 
				&& pstPackge->StashPackage.RoleGrids[i].GridData.RoleItem.WID == pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID)
			{
				memcpy(pstRoleItem,&pstPackge->StashPackage.RoleGrids[i].GridData.RoleItem,sizeof(ROLEITEM));
				*pIpos = i;
				break;
			}
		}
	}
	else if(iType == LIST_TYPE_SUNDRIES)
	{
		for(i=0; i<pstPackge->SundriesPackage.Num; i++)
		{
			if(pstPackge->SundriesPackage.RoleGrids[i].GridData.RoleItem.GridIdx == iFindIndx
				&&pstPackge->SundriesPackage.RoleGrids[i].GridData.RoleItem.WID == pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID)
			{
				memcpy(pstRoleItem,&pstPackge->SundriesPackage.RoleGrids[i].GridData.RoleItem,sizeof(ROLEITEM));
				*pIpos = i;
				break;
			}
		}
	}
	else if(iType == LIST_TYPE_STUFF)
	{
		for(i=0; i<pstPackge->StuffPackage.Num; i++)
		{
			if(pstPackge->StuffPackage.RoleItems[i].GridIdx == iFindIndx
				&&pstPackge->StuffPackage.RoleItems[i].WID == pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID)
			{
				memcpy(pstRoleItem,&pstPackge->StuffPackage.RoleItems[i],sizeof(ROLEITEM));
				*pIpos = i;
				break;
			}
		}
	}
	else if(iType == LIST_TYPE_TASK)
	{
		for(i=0; i<pstPackge->TaskPackage.Num; i++)
		{
			if(pstPackge->TaskPackage.RoleItems[i].GridIdx == iFindIndx
				&&pstPackge->TaskPackage.RoleItems[i].WID == pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID)
			{
				memcpy(pstRoleItem,&pstPackge->TaskPackage.RoleItems[i],sizeof(ROLEITEM));
				*pIpos = i;
				break;
			}
		}
	}
	else
	{
		return -1;
	}
	return 0;
}


int set_item_del(ROLEPACKAGE *pstPackge,ROLEWEAR *pstWear,ZONESETROLEITEMLISTREQ *pstSetItemReq)
{
	int iNum = pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num;
	ROLEITEM stRoleItem;
	int iPos = -1;

	if(pstSetItemReq->PackType == LIST_TYPE_WEAR)
	{
		if(find_wear_item_by_grididx(pstWear,pstSetItemReq,&stRoleItem,&iPos) < 0)
		{
			return -1;
		}
	}
	else
	{
		if(find_pack_item_by_grididx(pstPackge,pstSetItemReq,&stRoleItem,&iPos) < 0)
		{
			return -1;
		}
	}
	
	if(iPos == -1)
	{
		return -1;
	}
	if(stRoleItem.Num < iNum)
	{
		return -1;
	}

	if(pstSetItemReq->PackType == LIST_TYPE_STASH)
	{
		if(stRoleItem.Num > iNum)
		{
			//stRoleItem.Num -= iNum;
			pstPackge->StashPackage.RoleGrids[iPos].GridData.RoleItem.Num -= iNum;
		}
		else if(stRoleItem.Num == iNum)
		{
			pstPackge->StashPackage.Num --;
			if (iPos != pstPackge->StashPackage.Num)
			{
				memcpy(&pstPackge->StashPackage.RoleGrids[iPos],
					     &pstPackge->StashPackage.RoleGrids[pstPackge->StashPackage.Num],
					     sizeof(ROLEGRID));
			}
			
		}
		else
		{
			return -1;
		}
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_SUNDRIES)
	{
		if(stRoleItem.Num > iNum)
		{
			//stRoleItem.Num -= iNum;
			pstPackge->SundriesPackage.RoleGrids[iPos].GridData.RoleItem.Num -= iNum;
		}
		else if(stRoleItem.Num == iNum)
		{
			pstPackge->SundriesPackage.Num --;
			if (iPos != pstPackge->SundriesPackage.Num)
			{
				memcpy(&pstPackge->SundriesPackage.RoleGrids[iPos],
					     &pstPackge->SundriesPackage.RoleGrids[pstPackge->SundriesPackage.Num],
					     sizeof(ROLEGRID));
			}
			
		}
		else
		{
			return -1;
		}
	}
	
	else if(pstSetItemReq->PackType == LIST_TYPE_TASK)
	{
		if(stRoleItem.Num > iNum)
		{
			//stRoleItem.Num -= iNum;
			pstPackge->TaskPackage.RoleItems[iPos].Num -= iNum;
		}
		else if(stRoleItem.Num == iNum)
		{
			pstPackge->TaskPackage.Num --;
			if (iPos != pstPackge->TaskPackage.Num)
			{
				memcpy(&pstPackge->TaskPackage.RoleItems[iPos],
					     &pstPackge->TaskPackage.RoleItems[pstPackge->TaskPackage.Num],
					     sizeof(ROLEITEM));
			}
			
		}
		else
		{
			return -1;
		}
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_STUFF)
	{
		if(stRoleItem.Num > iNum)
		{
			//stRoleItem.Num -= iNum;
			pstPackge->StuffPackage.RoleItems[iPos].Num -= iNum;
		}
		else if(stRoleItem.Num == iNum)
		{
			pstPackge->StuffPackage.Num --;
			if (iPos != pstPackge->StuffPackage.Num)
			{
				memcpy(&pstPackge->StuffPackage.RoleItems[iPos],
					     &pstPackge->StuffPackage.RoleItems[pstPackge->StuffPackage.Num],
					     sizeof(ROLEITEM));
			}
			
		}
		else
		{
			return -1;
		}
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_WEAR)
	{
		/*
		if(stRoleItem.Num > iNum)
		{
			stRoleItem.Num -= iNum;
		}
		else if(stRoleItem.Num == iNum)
		{
			pstWear->ArmNum --;
			if (iPos != pstWear->ArmNum)
			{
				memcpy(&pstWear->RoleArms[iPos],&pstWear->RoleArms[pstWear->ArmNum],sizeof(ROLEARM));
			}
		}
		else
		{
			return -1;
		}
		*/
		pstWear->ArmNum --;
		if (iPos != pstWear->ArmNum)
		{
			memcpy(&pstWear->RoleArms[iPos],&pstWear->RoleArms[pstWear->ArmNum],sizeof(ROLEARM));
		}
	}
	else
	{
		return -1;
	}
		
	return 0;
}

int set_item_modify(CS_SVRENV*pstEnv,ROLEPACKAGE *pstPackge,ROLEWEAR *pstWear,ZONESETROLEITEMLISTREQ *pstSetItemReq)
{
	ROLEITEM stRoleItem;
	int iPos = -1;
	ITEMDEF *pstItemDef = NULL;

	if(pstSetItemReq->PackType == LIST_TYPE_WEAR)
	{
		if(find_wear_item_by_grididx(pstWear, pstSetItemReq, &stRoleItem, &iPos) < 0)
		{
			return -1;
		}
	}
	else
	{
		if(find_pack_item_by_grididx(pstPackge,pstSetItemReq,&stRoleItem,&iPos) < 0)
		{
			return -1;
		}
	}
	
	if(iPos == -1)
	{
		return -1;
	}

	pstItemDef = 	cs_find_itemdef(pstEnv, pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.DefID);
	if(pstItemDef == NULL)
	{
		return -1;
	}

	if (check_rolegrid(pstEnv,pstItemDef,pstSetItemReq) < 0)
	{
		return -1;
	}
	

	if(pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num > pstItemDef->MaxPile ||
		pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num <= 0)
	{
		return -1;
	}

	if(pstSetItemReq->PackType == LIST_TYPE_STASH)
	{
		memcpy(&pstPackge->StashPackage.RoleGrids[iPos],&pstSetItemReq->ZoneRoleGrid,sizeof(ROLEGRID));
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_SUNDRIES)
	{
		memcpy(&pstPackge->SundriesPackage.RoleGrids[iPos],&pstSetItemReq->ZoneRoleGrid,sizeof(ROLEGRID));
	}
	
	else if(pstSetItemReq->PackType == LIST_TYPE_STUFF)
	{
		memcpy(&pstPackge->StuffPackage.RoleItems[iPos],&pstSetItemReq->ZoneRoleGrid.GridData,sizeof(ROLEITEM));
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_TASK)
	{
		memcpy(&pstPackge->TaskPackage.RoleItems[iPos],&pstSetItemReq->ZoneRoleGrid.GridData,sizeof(ROLEITEM));
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_WEAR)
	{
		memcpy(&pstWear->RoleArms[iPos],&pstSetItemReq->ZoneRoleGrid.GridData,sizeof(ROLEARM));
	}
	else 
	{
		return -1;
	}
	return 0;
}

int add_item_need_grid_num(int iReqNum,int iMaxPile)
{
	/*if(iReqNum%iMaxPile != 0)
	{
		return iReqNum/iMaxPile + 1;
	}
	else 
	{
		return iReqNum/iMaxPile;
	}*/

	if (iReqNum <= iMaxPile)
	{
		return 1;
	}
	else
	{
		if(iReqNum%iMaxPile != 0)
		{
			return  iReqNum/iMaxPile + 1;
		}
		else
		{
			return iReqNum/iMaxPile;
		}
	}
}

int cs_get_free_grididx_by_rolegrid(ROLEGRID *pstPackageRoleGrid, 
	                                  int iNum, int iMaxNum)
{
	int i;
	int iGrid;
	char sGrid[MAX_PACKAGE_GRID];
	ROLEGRID* pstGrid;

	assert(iMaxNum <= MAX_PACKAGE_GRID);

	memset(sGrid, 0, iMaxNum);
	for(i=0; i<iNum; i++)
	{
		pstGrid = pstPackageRoleGrid+i;
		if(pstGrid->Type == GRID_ITEM_ITEM)
		{
			iGrid = pstGrid->GridData.RoleItem.GridIdx;
		}
		else if(pstGrid->Type == GRID_ITEM_ARM)
		{
			iGrid = pstGrid->GridData.RoleArm.GridIdx;
		}
		else
		{
			return -1;
		}
		
		if (iGrid >= 0 && iGrid < iMaxNum)
		{
			sGrid[iGrid] = 1;
		}
	}

	for (i=0; i<iMaxNum; i++)
	{
		if (!sGrid[i])
			return i;
	}

	return -1;
}

int cs_get_free_grididx_by_roleitem(const ROLEITEM *pstItems, int iNum,
	                                  int iMaxNum)
{
	int i;
	int iGrid;
	char sGrid[MAX_PACKAGE_GRID];
	const ROLEITEM *pstCurr = pstItems;

	assert(iMaxNum <= MAX_PACKAGE_GRID);

	memset(sGrid, 0, iMaxNum * sizeof(sGrid[0]));
	i = iNum;
	while(i--)
	{
		iGrid = pstCurr->GridIdx;
		
		if(iGrid >= 0 && iGrid < iMaxNum)
		{
			sGrid[iGrid] = 1;
		}
		pstCurr++;
	}

	for(i = 0; i < iMaxNum; i++)
	{
		if (!sGrid[i])
			return i;
	}

	return -1;
}

int set_item_add(TAPPCTX*pstAppCtx,CS_SVRENV*pstEnv,ROLEPACKAGE *pstPackge,ZONESETROLEITEMLISTREQ *pstSetItemReq)
{
	int iLeftGrid = 0;
	ITEMDEF *pstItemDef = NULL;
	int iTotalNum = pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num;
	int iOldGrid = pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.GridIdx;
	//tdr_ulonglong ullWID = pstSetItemReq->ZoneRoleGrid.GridData.RoleArm.WID;
	int iNeedGrid;
	int iLeftNum;
	int iGrid;
	

	if(pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num <= 0)
	{
		return -1;
	}
	
	pstItemDef = 	cs_find_itemdef(pstEnv, pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.DefID);
	if(pstItemDef == NULL)
	{
		return -1;
	}

	if(check_rolegrid(pstEnv,pstItemDef,pstSetItemReq) < 0)
	{
		return -1;
	}

	if (pstItemDef->MaxPile == 0)
	{
		pstItemDef->MaxPile = 1;
	}

	if(pstSetItemReq->PackType == LIST_TYPE_STASH)
	{
		iLeftGrid = pstPackge->StashPackage.MaxNum - pstPackge->StashPackage.Num;
		iNeedGrid = add_item_need_grid_num(iTotalNum,pstItemDef->MaxPile);
		if(iLeftGrid < iNeedGrid)
		{
			return -1;
		}

		iLeftNum = iTotalNum;
		while(iLeftNum > 0)
		{
			if(iLeftNum > pstItemDef->MaxPile)
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = pstItemDef->MaxPile;
			}
			else
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = iLeftNum;
			}
			iLeftNum -= pstItemDef->MaxPile;

			iGrid = cs_get_free_grididx_by_rolegrid(pstPackge->StashPackage.RoleGrids,pstPackge->StashPackage.Num,
				pstPackge->StashPackage.MaxNum);
		
			if(iGrid != -1)
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.GridIdx = iGrid;
			}
			else
			{
				return -1;
			}
			pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID = cs_get_gid(pstAppCtx,pstEnv);
			if (pstSetItemReq->ZoneRoleGrid.Type == GRID_ITEM_ARM)
			{
				ROLEARM *pstRoleArm = &pstSetItemReq->ZoneRoleGrid.GridData.RoleArm;
					
				pstRoleArm->Dur = pstItemDef->ArmDur.MaxDur*DUR_UNIT;
				if (pstItemDef->ValidTime > 0)
				{
					pstRoleArm->ValidEnd = pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;
				}
				pstRoleArm->StarLv = pstItemDef->ItemStar;
				if (pstItemDef->ArmBind == ITEM_GET_BIND)
				{
					pstRoleArm->InstFlag |= INST_ITEM_BIND;
				}
			}
			else if (pstSetItemReq->ZoneRoleGrid.Type == GRID_ITEM_ITEM)
			{
				ROLEITEM *pstRoleItem = &pstSetItemReq->ZoneRoleGrid.GridData.RoleItem;

				//有使用次数的道具
				if(pstItemDef->UseMaxDur)
				{
					pstRoleItem->InstVal1 = pstItemDef->UseMaxDur;
				}
				
				if (pstItemDef->ValidTime > 0)
				{
					pstRoleItem->ValidEnd = pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;
				}

				if (pstItemDef->ArmBind == ITEM_GET_BIND)
				{
					pstRoleItem->InstFlag |= INST_ITEM_BIND;
				}
			}

			memcpy(&pstPackge->StashPackage.RoleGrids[pstPackge->StashPackage.Num],&pstSetItemReq->ZoneRoleGrid,sizeof(ROLEGRID));

			pstPackge->StashPackage.Num ++;
		}
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_SUNDRIES)
	{
		iLeftGrid = pstPackge->SundriesPackage.MaxNum - pstPackge->SundriesPackage.Num;
		iNeedGrid = add_item_need_grid_num(iTotalNum,pstItemDef->MaxPile);
		if(iLeftGrid < iNeedGrid)
		{
			return -1;
		}

		iLeftNum = iTotalNum;
		while(iLeftNum > 0)
		{
			if(iLeftNum > pstItemDef->MaxPile)
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = pstItemDef->MaxPile;
			}
			else
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = iLeftNum;
			}
			iLeftNum -= pstItemDef->MaxPile;

			iGrid = cs_get_free_grididx_by_rolegrid(pstPackge->SundriesPackage.RoleGrids,pstPackge->SundriesPackage.Num,
				pstPackge->SundriesPackage.MaxNum);
			if(iGrid != -1)
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.GridIdx = iGrid;
			}
			else
			{
				return -1;
			}

			pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID = cs_get_gid(pstAppCtx,pstEnv);
			if (pstSetItemReq->ZoneRoleGrid.Type == GRID_ITEM_ARM)
			{
				ROLEARM *pstRoleArm = &pstSetItemReq->ZoneRoleGrid.GridData.RoleArm;
					
				pstRoleArm->Dur = pstItemDef->ArmDur.MaxDur*DUR_UNIT;
				if (pstItemDef->ValidTime > 0)
				{
					pstRoleArm->ValidEnd = pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;
				}
				pstRoleArm->StarLv = pstItemDef->ItemStar;
				if (pstItemDef->ArmBind == ITEM_GET_BIND)
				{
					pstRoleArm->InstFlag |= INST_ITEM_BIND;
				}
			}
			else if (pstSetItemReq->ZoneRoleGrid.Type == GRID_ITEM_ITEM)
			{
				ROLEITEM *pstRoleItem = &pstSetItemReq->ZoneRoleGrid.GridData.RoleItem;

				//有使用次数的道具
				if(pstItemDef->UseMaxDur)
				{
					pstRoleItem->InstVal1 = pstItemDef->UseMaxDur;
				}
				
				if (pstItemDef->ValidTime > 0)
				{
					pstRoleItem->ValidEnd = pstAppCtx->stCurr.tv_sec + pstItemDef->ValidTime*3600;
				}

				if (pstItemDef->ArmBind == ITEM_GET_BIND)
				{
					pstRoleItem->InstFlag |= INST_ITEM_BIND;
				}
			}

			memcpy(&pstPackge->SundriesPackage.RoleGrids[pstPackge->SundriesPackage.Num],&pstSetItemReq->ZoneRoleGrid,sizeof(ROLEGRID));
			
			pstPackge->SundriesPackage.Num ++;
		}
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_TASK)
	{
		TASKPACKAGE *pstSubPkg = &pstPackge->TaskPackage;
		
		iLeftGrid = pstSubPkg->MaxNum - pstSubPkg->Num;
		iNeedGrid = add_item_need_grid_num(iTotalNum, pstItemDef->MaxPile);
		if(iLeftGrid < iNeedGrid)
		{
			return -1;
		}

		iLeftNum = iTotalNum;
		while(iLeftNum > 0)
		{
			if(iLeftNum > pstItemDef->MaxPile)
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = pstItemDef->MaxPile;
			}
			else
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = iLeftNum;
			}
			iLeftNum -= pstItemDef->MaxPile;

			iGrid = cs_get_free_grididx_by_roleitem(pstSubPkg->RoleItems,
				                                      pstSubPkg->Num,
				                                      pstSubPkg->MaxNum);
			if(iGrid < 0)
			{
				return -1;
			}
			pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.GridIdx = iGrid;
			pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID = 
				cs_get_gid(pstAppCtx, pstEnv);
			memcpy(&pstSubPkg->RoleItems[pstSubPkg->Num],
				     &pstSetItemReq->ZoneRoleGrid.GridData.RoleItem, sizeof(ROLEITEM));
			
			pstSubPkg->Num++;
		}
	}
	else if(pstSetItemReq->PackType == LIST_TYPE_STUFF)
	{
		STUFFPACKAGE *pstSubPkg = &pstPackge->StuffPackage;
		
		iLeftGrid = pstSubPkg->MaxNum - pstSubPkg->Num;
		iNeedGrid = add_item_need_grid_num(iTotalNum, pstItemDef->MaxPile);
		if(iLeftGrid < iNeedGrid)
		{
			return -1;
		}

		iLeftNum = iTotalNum;
		while(iLeftNum > 0)
		{
			if(iLeftNum > pstItemDef->MaxPile)
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = pstItemDef->MaxPile;
			}
			else
			{
				pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = iLeftNum;
			}
			iLeftNum -= pstItemDef->MaxPile;

			iGrid = cs_get_free_grididx_by_roleitem(pstSubPkg->RoleItems,
				                                      pstSubPkg->Num,
				                                      pstSubPkg->MaxNum);
			if(iGrid < 0)
			{
				return -1;
			}
			pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.GridIdx = iGrid;
			pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.WID = 
				cs_get_gid(pstAppCtx, pstEnv);
			memcpy(&pstSubPkg->RoleItems[pstSubPkg->Num],
				     &pstSetItemReq->ZoneRoleGrid.GridData.RoleItem, sizeof(ROLEITEM));
			
			pstSubPkg->Num++;
		}
	}
	else 
	{
		return -1;
	}
	pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.Num = iTotalNum;
	pstSetItemReq->ZoneRoleGrid.GridData.RoleItem.GridIdx = iOldGrid;
	return 0;
}

int check_rolegrid(CS_SVRENV*pstEnv, ITEMDEF *pstItemDef, 
	                 ZONESETROLEITEMLISTREQ *pstSetItemReq)
{
	ROLEGRID *pstGrid;
	ROLEARM *pstArm;

	UNUSED(pstEnv);

	if (!pstItemDef ||!pstSetItemReq)
	{
		return -1;
	}

	pstGrid = &pstSetItemReq->ZoneRoleGrid;
	pstArm = &pstGrid->GridData.RoleArm;
	
	if (pstGrid->Type == GRID_ITEM_ARM)
	{
		if (pstArm->StarLv > MAX_ARM_STAR_LEVEL ||
			(pstItemDef->StarLimit != 0 && pstArm->StarLv >= pstItemDef->StarLimit)||
			pstArm->HoleNum > MAX_ARM_HOLE ||
			pstArm->FightLv > MAX_ARM_FIGHT_LEVEL ||
			pstArm->FightLv > pstArm->StarLv)
		{
			return -1;
		}
	}

	switch(pstItemDef->ItemType)
	{
	case ITEM_TYPE_ARM:
	case ITEM_TYPE_ITEM:
		if(pstSetItemReq->PackType != LIST_TYPE_SUNDRIES && pstSetItemReq->PackType != LIST_TYPE_STASH)
		{
			return -1;
		}
		break;

	case ITEM_TYPE_STUFF:
		if(pstSetItemReq->PackType != LIST_TYPE_STUFF)
		{
			return -1;
		}
		break;
		
	case ITEM_TYPE_TASK:
		if(pstSetItemReq->PackType != LIST_TYPE_TASK)
		{
			return -1;
		}
		break;

	case ITEM_TYPE_AMM:
		if(pstSetItemReq->PackType != LIST_TYPE_AMM)
		{
			return -1;
		}
		break;

	default:
		return -1;
		break;
	}

	return 0;
}

int set_item_info(TAPPCTX*pstAppCtx,CS_SVRENV*pstEnv,ROLEPACKAGE *pstPackge,ROLEWEAR *pstWear,ZONESETROLEITEMLISTREQ *pstSetItemReq)
{
	if(!pstPackge || !pstSetItemReq ||!pstWear)
	{
		return -1;
	}

	if(pstSetItemReq->OPType == ROLE_ITEM_ADD)
	{
		if(set_item_add(pstAppCtx, pstEnv,pstPackge,pstSetItemReq) < 0)
		{
			return -1;
		}
	}
	else if(pstSetItemReq->OPType == ROLE_ITEM_DEL)
	{
		if(set_item_del(pstPackge,pstWear,pstSetItemReq) < 0)
		{
			return -1;
		}
	}
	else if(pstSetItemReq->OPType == ROLE_ITEM_MODIFY)
	{
		if( set_item_modify(pstEnv,pstPackge,pstWear,pstSetItemReq) < 0)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int find_diathesis_by_id(DIATHESISINFO *pstDiathesis,int iDiathesisID,DIATHESISDATA *pstData,int *iPos)
{
	int i;
	
	if(pstDiathesis == NULL)
	{
		return -1;
	}

	*iPos = -1;
	for(i=0;i<(int)pstDiathesis->DiathesisNum;i++)
	{
		if(pstDiathesis->DiathesisList[i].DiathesisID == iDiathesisID)
		{
			memcpy(pstData,&pstDiathesis->DiathesisList[i],sizeof(DIATHESISDATA));
			*iPos = i;
			break;
		}
	}
	return 0;
}

static TASKRUNONE *role_running_task(ROLEDATA *pstRoleData, unsigned int uTaskID)
{
	TASKINFO *pstTaskInfo = &pstRoleData->MiscInfo.TaskInfo;
	int i = pstTaskInfo->TaskRun.TaskRunNum;

	while(i--)
	{
		if(pstTaskInfo->TaskRun.TaskRuns[i].TaskID == uTaskID)
		{
			return pstTaskInfo->TaskRun.TaskRuns + i;
		}
	}
	return NULL;
}

static void role_del_running_task(ROLEDATA *pstRoleData, unsigned int uTaskID)
{
	TASKINFO *pstTaskInfo = &pstRoleData->MiscInfo.TaskInfo;
	int i = pstTaskInfo->TaskRun.TaskRunNum;

	while(i--)
	{
		if(pstTaskInfo->TaskRun.TaskRuns[i].TaskID == uTaskID)
		{
			pstTaskInfo->TaskRun.TaskRunNum--;
			if(i != pstTaskInfo->TaskRun.TaskRunNum)
			{
				pstTaskInfo->TaskRun.TaskRuns[i] = 
					pstTaskInfo->TaskRun.TaskRuns[pstTaskInfo->TaskRun.TaskRunNum];
			}
			break;
		}
	}
}

static void role_del_taskcycle(ROLEDATA *pstRoleData, unsigned int uTaskID)
{
	TASKRECYCLE *pstTaskCycleRun = &pstRoleData->MiscInfo.TaskInfo.TaskRecycle;
	int i = pstTaskCycleRun->TaskRecycleNum;

	while(i--)
	{
		if(pstTaskCycleRun->TaskRecycles[i].TaskRecycleID == uTaskID)
		{
			pstTaskCycleRun->TaskRecycleNum--;
			if (i != pstTaskCycleRun->TaskRecycleNum)
			{
				pstTaskCycleRun->TaskRecycles[i] = 
					pstTaskCycleRun->TaskRecycles[pstTaskCycleRun->TaskRecycleNum];
			}
			break;
		}
	}
}

static int del_task_item(TASKDEF *pstTaskDef, ROLEPACKAGE* pstRolePkg)
{
	int i;
	TASKPACKAGE *pstTaskPkg = &pstRolePkg->TaskPackage;
	ROLEITEM *pstRoleItem;
	int iItemNum;
	
	if (pstTaskDef->TaskItem <= 0)
	{
		return 0;
	}

	iItemNum = 1;
	if (pstTaskDef->TaskItemNum > 1)
	{
		iItemNum = pstTaskDef->TaskItemNum;
	}

	for (i=pstTaskPkg->Num-1; i >= 0; i--)
	{
		pstRoleItem = &pstTaskPkg->RoleItems[i];
		if (pstRoleItem->DefID == (int)pstTaskDef->TaskItem)
		{
			if (pstRoleItem->Num == iItemNum)
			{
				// 删除物品
				pstTaskPkg->Num --;
				if (i != pstTaskPkg->Num)
				{
					memmove(pstRoleItem,&pstTaskPkg->RoleItems[i+1], 
							   sizeof(*pstRoleItem) * (pstTaskPkg->Num - i));
				}
				break;
				
			}
			else if (pstRoleItem->Num > iItemNum)
			{
				pstRoleItem->Num -= iItemNum;
			}
			else 
			{
				// 删除完后找下一个
				pstTaskPkg->Num --;
				if (i != pstTaskPkg->Num)
				{
					memmove(pstRoleItem,&pstTaskPkg->RoleItems[i+1], 
							   sizeof(*pstRoleItem) * (pstTaskPkg->Num - i));
				}
				iItemNum -= pstRoleItem->Num;
			}
		}
	}

	return 0;
}

int del_role_task_by_id(CS_SVRENV*pstEnv, ROLEDATA *pstRoleData,
	                      unsigned int uiTaskID)
{
	TASKRUNONE *pstTaskRun = NULL;
	TASKDEF *pstTaskDef = NULL;
	int iTaskRecycleID;

	if(uiTaskID == BIRTH_PLAYER_TASK)
	{
		// 初始任务，不可放弃
		return -1;
	}

	pstTaskRun = role_running_task(pstRoleData, uiTaskID);
	if(!pstTaskRun)
	{
		return -1;
	}

	iTaskRecycleID = pstTaskRun->CycleID;

	pstTaskDef = cs_find_taskdef(pstEnv, uiTaskID);
	if(!pstTaskDef)
	{
		return -1;
	}

	del_task_item(pstTaskDef, &pstRoleData->Package);

	role_del_running_task(pstRoleData, uiTaskID);
	if (iTaskRecycleID > 0)
	{
		role_del_taskcycle(pstRoleData, iTaskRecycleID);
	}
	return 0;
}

int set_diathesis_info(CS_SVRENV*pstEnv,DIATHESISINFO *pstDiathesis, ZONESETROLEDIATHESISREQ *pstSetDiathesisReq)
{
	int iOpType ;
	DIATHESISDATA stDiathesisData;
	int iPos = -1;
	DIATHESISDEF *pstDiathsisDef;
	
	if(pstDiathesis == NULL || pstSetDiathesisReq == NULL)
	{
		return -1;
	}

	pstDiathsisDef = cs_find_diathesisdef(pstEnv, pstSetDiathesisReq->ZoneDiathesisData.DiathesisID,
										 pstSetDiathesisReq->ZoneDiathesisData.DiathesisLevel);
	if (!pstDiathsisDef)
	{
		return 0;
	}

	iOpType = pstSetDiathesisReq->OpType;
	if(find_diathesis_by_id(pstDiathesis,pstSetDiathesisReq->ZoneDiathesisData.DiathesisID,&stDiathesisData,&iPos) < 0)
	{
		return -1;
	}

	if(iOpType == ROLE_DIATHESIS_ADD)
	{
		if(iPos != -1)
		{
			return -1;
		}

		memcpy(&pstDiathesis->DiathesisList[pstDiathesis->DiathesisNum],&pstSetDiathesisReq->ZoneDiathesisData,sizeof(DIATHESISDATA));
		pstDiathesis->DiathesisNum ++;
	}
	else if(iOpType == ROLE_DIATHESIS_DEL)
	{
		if(iPos == -1)
		{
			return -1;
		}

		pstDiathesis->DiathesisNum --;
		if(iPos != (int)pstDiathesis->DiathesisNum )
		{
			memcpy(&pstDiathesis->DiathesisList[iPos],&pstDiathesis->DiathesisList[pstDiathesis->DiathesisNum],sizeof(DIATHESISDATA));
		}
	}
	else if(iOpType == ROLE_DIATHESIS_MODIFY)
	{
		if(iPos == -1)
		{
			return -1;
		}

		// 检查参数
		memcpy(&pstDiathesis->DiathesisList[iPos],&pstSetDiathesisReq->ZoneDiathesisData,sizeof(DIATHESISDATA));
	}
	else
	{
		return -1;
	}
	return 0;
}
int set_role_qiyue_req(CS_SVRENV*pstEnv,ROLEDATA *pstRoleData, SETROLEQIYUEREQ *pstReq)
{
	UNUSED(pstEnv);
	
	QIYUEINFO *pstQiYueInfo = NULL;
	int iEqual = 0;
	int iIndex = 0;
	
	pstQiYueInfo = &pstRoleData->Package.QiYueInfo;

	iIndex = bsearch_int(&pstReq->QiYueID, pstQiYueInfo->QiYueList, (int)pstQiYueInfo->QiYueNum, sizeof(int), &iEqual);
	if( !iEqual )
	{
		/* 没找到自己有这个契约*/
		return -1;
	}
	/* 找到了就设置下 */
	pstQiYueInfo->CurrQiYueID = pstReq->QiYueID;
		
	return 0;
}

int add_role_qiyue_req(CS_SVRENV*pstEnv,ROLEDATA *pstRoleData, ADDROLEQIYUEREQ *pstReq)
{
	QIYUEINFO *pstQiYueInfo = NULL;
	int iIndex = 0;
	int iEqual = 0;
	QIYUEDEF *pstQiYueDef = NULL;

	pstQiYueInfo = &pstRoleData->Package.QiYueInfo;
	
	pstQiYueDef = cs_find_qiyuedef( pstEnv, pstReq->QiYueID, NULL);
	if( !pstQiYueDef )
	{
		return 0;
	}
	
	iIndex = bsearch_int(&pstReq->QiYueID, pstQiYueInfo->QiYueList, pstQiYueInfo->QiYueNum, sizeof(int), &iEqual);
	if( iEqual )
	{
		return -1;
	}

	if( pstQiYueInfo->QiYueNum >= QIYUE_MAX_NUM )
	{
		return -1;
	}

	if ( !binsert_int(&pstReq->QiYueID,pstQiYueInfo->QiYueList, (int *)&pstQiYueInfo->QiYueNum, sizeof(int), 1) )
	{
		 return -1;
	}
		
	return 0;
}
int del_role_qiyue_req(CS_SVRENV*pstEnv,ROLEDATA *pstRoleData, DELROLEQIYUEREQ *pstReq)
{
	UNUSED(pstEnv);
	QIYUEINFO *pstQiYueInfo = NULL;
	
	pstQiYueInfo = &pstRoleData->Package.QiYueInfo;

	if (!bdelete_int(&pstReq->QiYueID, pstQiYueInfo->QiYueList,(int *)&pstQiYueInfo->QiYueNum, sizeof(int)))
	{
		return -1;
	}

	if( pstReq->QiYueID == pstQiYueInfo->CurrQiYueID )
	{
		pstQiYueInfo->CurrQiYueID = 0;
	}
		
	return 0;

}

static int get_fairy_by_id(FAIRYPACKAGE *pstFairyPak, int iFairyID)
{
	int i;

	for (i=0; i<pstFairyPak->FairyNum; i++)
	{
		if (pstFairyPak->RoleFairys[i].FairyID == iFairyID)
		{
			return i;
		}
	}

	return -1;
}

static int cs_get_free_grid_by_fairy(FAIRYPACKAGE *pstFairyPkg)
{

	char cGrid[FAIRY_MAX_NUM];
	int i;
	//FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	//FairyOnline *pstFairyOnline = &pstPlayer->stOnline.stFairyOnline;
	
	assert(pstFairyPkg->FairyMaxNum <= FAIRY_MAX_NUM);
	memset(cGrid, 0, sizeof(cGrid));

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].GridIdx >=0 &&
			pstFairyPkg->RoleFairys[i].GridIdx < pstFairyPkg->FairyMaxNum)
		{
			cGrid[(int)(pstFairyPkg->RoleFairys[i].GridIdx)] = 1;
		}
	}

	for (i=0; i<pstFairyPkg->FairyMaxNum; i++)
	{
		if (!cGrid[i])
		{
			return i;
		}
	}

	return -1;
}

int set_role_fairy(TAPPCTX*pstAppCtx,CS_SVRENV*pstEnv, ROLEDATA *pstRoleData, ZONESETROLEFAIRYREQ *pstSetFairyReq)
{
	int iIdx;
	FAIRYPACKAGE *pstFairyPak = &pstRoleData->Package.FairyPackage;
	FAIRYDEF *pstFairyDef;
	ROLEFAIRY *pstRoleFairy;

	if (pstSetFairyReq->OpType == ROLE_FAIRY_DEL)
	{
		iIdx = get_fairy_by_id(pstFairyPak, pstSetFairyReq->FairyID);
		if (-1 == iIdx)
		{
			return -1;
		}

		pstFairyPak->FairyNum --;
		if (iIdx != pstFairyPak->FairyNum)
		{
			memcpy(&pstFairyPak->RoleFairys[pstFairyPak->FairyNum], &pstFairyPak->RoleFairys[iIdx], 
				sizeof(pstFairyPak->RoleFairys[0]));
		}
	}
	else if (pstSetFairyReq->OpType == ROLE_FAIRY_ADD)
	{
		iIdx = get_fairy_by_id(pstFairyPak, pstSetFairyReq->FairyID);
		if (iIdx >= 0 && pstFairyPak->FairyNum >= pstFairyPak->FairyMaxNum)
		{
			return -1;
		}

		pstFairyDef = cs_find_fairydef(pstEnv, pstSetFairyReq->FairyID, pstSetFairyReq->FairyLV);
		if (!pstFairyDef)
		{
			return -1;
		}

		pstRoleFairy = &pstFairyPak->RoleFairys[pstFairyPak->FairyNum];
		pstRoleFairy->FairyID = pstSetFairyReq->FairyID;
		pstRoleFairy->FairyLevel = pstSetFairyReq->FairyLV;
		pstRoleFairy->EvolveLeftTM = 0;
		pstRoleFairy->WID = cs_get_gid(pstAppCtx,pstEnv);
		pstRoleFairy->Stat |= FAIRY_STAT_BACK;
		pstRoleFairy->GridIdx = cs_get_free_grid_by_fairy(pstFairyPak);

		pstFairyPak->FairyNum ++;
		
	}
	else if(pstSetFairyReq->OpType == ROLE_FAIRY_MODIFY)
	{
		iIdx = get_fairy_by_id(pstFairyPak, pstSetFairyReq->FairyID);
		if (-1 == iIdx)
		{
			return -1;
		}

		if (pstFairyPak->RoleFairys[iIdx].FairyLevel == pstSetFairyReq->FairyLV)
		{
			return -1;
		}

		pstFairyDef = cs_find_fairydef(pstEnv, pstSetFairyReq->FairyID, pstSetFairyReq->FairyLV);
		if (!pstFairyDef)
		{
			return -1;
		}

		pstFairyPak->RoleFairys[iIdx].FairyLevel = pstSetFairyReq->FairyLV;

		
		
	}
	else
	{
		return -1;
	}
	return 0;
}

int role_drop_restore_grid(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,ROLEDATA *pstRoleData, ROLEGRID *pstGrid)
{
	int iLeftGrid, iNeedGrid;
	int iLeftNum, iGrid;
	int iTotalNum = pstGrid->GridData.RoleItem.Num;
	ITEMDEF *pstItemDef;
	ROLEPACKAGE *pstPackge = &pstRoleData->Package;

	pstItemDef = cs_find_itemdef(pstEnv, pstGrid->GridData.RoleItem.DefID);
	if(pstItemDef == NULL)
	{
		return -1;
	}

	switch(pstItemDef->ItemType)
	{
		case ITEM_TYPE_ARM:
		case ITEM_TYPE_ITEM:
			iLeftGrid = pstPackge->StashPackage.MaxNum - pstPackge->StashPackage.Num;
			iNeedGrid = add_item_need_grid_num(iTotalNum,pstItemDef->MaxPile);
			if(iLeftGrid < iNeedGrid)
			{
				return -1;
			}

			iLeftNum = iTotalNum;
			while(iLeftNum > 0)
			{
				if(iLeftNum > pstItemDef->MaxPile)
				{
					pstGrid->GridData.RoleItem.Num = pstItemDef->MaxPile;
				}
				else
				{
					pstGrid->GridData.RoleItem.Num = iLeftNum;
				}
				iLeftNum -= pstItemDef->MaxPile;

				iGrid = cs_get_free_grididx_by_rolegrid(pstPackge->SundriesPackage.RoleGrids,pstPackge->SundriesPackage.Num,
					pstPackge->SundriesPackage.MaxNum);
				if(iGrid < 0)
				{
					return -1;
				}

				pstGrid->GridData.RoleItem.GridIdx = iGrid;
				//pstGrid->GridData.RoleItem.WID = cs_get_gid(pstAppCtx,pstEnv);
				memcpy(&pstPackge->SundriesPackage.RoleGrids[pstPackge->SundriesPackage.Num], pstGrid,sizeof(ROLEGRID));
				
				pstPackge->SundriesPackage.Num ++;
			}
			break;

		case ITEM_TYPE_STUFF:
			{
				STUFFPACKAGE *pstSubPkg = &pstPackge->StuffPackage;
			
				iLeftGrid = pstSubPkg->MaxNum - pstSubPkg->Num;
				iNeedGrid = add_item_need_grid_num(iTotalNum, pstItemDef->MaxPile);
				if(iLeftGrid < iNeedGrid)
				{
					return -1;
				}

				iLeftNum = iTotalNum;
				while(iLeftNum > 0)
				{
					if(iLeftNum > pstItemDef->MaxPile)
					{
						pstGrid->GridData.RoleItem.Num = pstItemDef->MaxPile;
					}
					else
					{
						pstGrid->GridData.RoleItem.Num = iLeftNum;
					}
					iLeftNum -= pstItemDef->MaxPile;

					iGrid = cs_get_free_grididx_by_roleitem(pstSubPkg->RoleItems,
						                                      pstSubPkg->Num,
						                                      pstSubPkg->MaxNum);
					if(iGrid < 0)
					{
						return -1;
					}
					pstGrid->GridData.RoleItem.GridIdx = iGrid;
					//pstGrid->GridData.RoleItem.WID = cs_get_gid(pstAppCtx, pstEnv);
					memcpy(&pstSubPkg->RoleItems[pstSubPkg->Num],
						     &pstGrid->GridData.RoleItem, sizeof(ROLEITEM));
					
					pstSubPkg->Num++;
				}
			}
			break;
		case ITEM_TYPE_TASK:
			{
				TASKPACKAGE *pstSubPkg = &pstPackge->TaskPackage;
			
				iLeftGrid = pstSubPkg->MaxNum - pstSubPkg->Num;
				iNeedGrid = add_item_need_grid_num(iTotalNum, pstItemDef->MaxPile);
				if(iLeftGrid < iNeedGrid)
				{
					return -1;
				}

				iLeftNum = iTotalNum;
				while(iLeftNum > 0)
				{
					if(iLeftNum > pstItemDef->MaxPile)
					{
						pstGrid->GridData.RoleItem.Num = pstItemDef->MaxPile;
					}
					else
					{
						pstGrid->GridData.RoleItem.Num = iLeftNum;
					}
					iLeftNum -= pstItemDef->MaxPile;

					iGrid = cs_get_free_grididx_by_roleitem(pstSubPkg->RoleItems,
						                                      pstSubPkg->Num,
						                                      pstSubPkg->MaxNum);
					if(iGrid < 0)
					{
						return -1;
					}
					pstGrid->GridData.RoleItem.GridIdx = iGrid;
					//pstGrid->GridData.RoleItem.WID = cs_get_gid(pstAppCtx, pstEnv);
					memcpy(&pstSubPkg->RoleItems[pstSubPkg->Num],
						     &pstGrid->GridData.RoleItem, sizeof(ROLEITEM));
					
					pstSubPkg->Num++;
				}
			}
		default:
			return -1;
			break;
	}

	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int role_drop_restore(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,ROLEDATA *pstRoleData, ROLEDROPRESTOREREQ *pstReq)
{
	switch(pstReq->Type)
	{
		case ROLE_DROP_GRID:
			return role_drop_restore_grid(pstAppCtx, pstEnv, pstRoleData, &pstReq->RestoreInfo.Grid);
			break;
		case ROLE_DROP_PET:
			if (pstRoleData->Package.PetPackage.Num >= pstRoleData->Package.PetPackage.MaxNum)
			{
				return -1;
			}
			pstRoleData->Package.PetPackage.RolePets[pstRoleData->Package.PetPackage.Num++]
				= pstReq->RestoreInfo.Pet;
			break;
		default:
			return -1;
			break;
	}
		
	return 0;
}

//数值 +- 10% 范围内随即
int cs_get_ran_genius(int iGeniusVal)
{
	int iMinVal, iMaxVal;
	int iVal;

	iMinVal = iGeniusVal*0.9;
	iMaxVal = iGeniusVal*1.1;

	if (iMaxVal > iMinVal)
	{
		iVal = iMinVal + RAND1(iMaxVal-iMinVal);
	}
	else
	{
		iVal = iGeniusVal;
	}

	return iVal;
}

//创建宠物各项资质成长
int cs_pet_all_genius_create(PETDEF *pstPetDef, ROLEPET *pstRolePet)
{
	PETGENIUS *pstGenius;
	
	if(pstRolePet->PetGenius >= MAX_PET_GENIUS)
	{
		return -1;
	}

	pstGenius = &pstPetDef->Genius[pstRolePet->PetGenius];

	pstRolePet->PhsicAtkGenius = cs_get_ran_genius(pstGenius->PhsicAtkGenius);
	pstRolePet->IceAtkGenius = cs_get_ran_genius(pstGenius->IceAtkGenius);
	pstRolePet->HPGenius = cs_get_ran_genius(pstGenius->HPGenius);
	pstRolePet->DefGenius = cs_get_ran_genius(pstGenius->DefGenius);
	pstRolePet->HitRateGenius = cs_get_ran_genius(pstGenius->HitRateGenius);
	pstRolePet->FleeGenius = cs_get_ran_genius(pstGenius->FleeGenius);

	return 0;
}

int cs_new_pet_data(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv, ROLEPET *pstRolePet)
{
	PETDEF *pstPetDef;
	PETLEVELDEF *pstPetLevelDef;
	
	pstPetDef = cs_find_petdef(pstEnv, pstRolePet->PetID);
	if (!pstPetDef)
	{
		return -1;
	}

	pstRolePet->PetID = pstPetDef->PetID;
	STRNCPY(pstRolePet->Name, pstPetDef->Name, sizeof(pstRolePet->Name));
	pstRolePet->WID =cs_get_gid(pstAppCtx, pstEnv);
	pstRolePet->LoveVal = BORN_PET_LOVE_VAL;
	
	if(pstRolePet->Level <= 0)
	{
		pstRolePet->Level = 1;
	}

	//检查下有效等级
	pstPetLevelDef = cs_find_petleveldef(pstEnv, pstRolePet->Level);
	if (!pstPetDef)
	{
		return -1;
	}
	
	pstRolePet->TraitID =0;
	pstRolePet->Gender = ((int )RAND1(2) )+ 1;
	
	if (0 > cs_pet_all_genius_create(pstPetDef, pstRolePet))
	{
		return -1;
	}

	//获得隐藏技能
	if (pstPetDef->NativeSkill.SkillID != 0 && 
		((int)RAND1(10000)) < pstPetDef->NativeSkill.Rate*100)
	{
		DIATHESISDEF *pstDiaDef;

		pstDiaDef = cs_find_diathesisdef(pstEnv, pstPetDef->NativeSkill.SkillID, 1);
		if (pstDiaDef)
		{
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillID = pstDiaDef->DiathesisID;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillLevel = 1;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].GridType = pstDiaDef->PetDiathesisType;

			pstRolePet->PetSkills.SkillGridNum++;
		}
	}
	else 
	{
		//获得技能栏
		if (pstPetDef->BornSkillGrid)
		{
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillID = 0;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillLevel = 0;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].GridType = pstPetDef->BornSkillGrid;
			pstRolePet->PetSkills.SkillGridNum++;
		}
	}

	//HP
	pstRolePet->MaxHP  = pstPetLevelDef->HP*(pstRolePet->HPGenius*(80+pstRolePet->StrengthenLevel*2)/100.0)/1000.0;
	pstRolePet->HP = pstRolePet->MaxHP;
	
	return 0;
}

int cs_hatch_pet(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,ROLEDATA *pstRoleData, ZONEROLEHATCHPETREQ *pstReq)
{
	ROLEPET stPet;
	PETPACKAGE	 *pstPetPackage = &pstRoleData->Package.PetPackage;
	 	
	if (pstRoleData->Package.PetPackage.Num >= pstRoleData->Package.PetPackage.MaxNum)
	{
		return -1;
	}

	memset(&stPet, 0, sizeof(stPet));
	stPet.PetID = pstReq->PetInfo.PetID;
	stPet.Level = pstReq->PetInfo.PetLevel;
	stPet.PetGenius = pstReq->PetInfo.PetGenius;
	stPet.StrengthenLevel = pstReq->PetInfo.StrengthenLevel;

	if(0> cs_new_pet_data(pstAppCtx, pstEnv, &stPet))
	{
		return -1;
	}
	
	pstPetPackage->RolePets[pstPetPackage->Num++] = stPet;
		
	return 0;
}

/*static int role_back_record(TAPPCTX*pstAppCtx, 
							CS_SVRENV*pstEnv,
							ROLEDATA *pstRoleData, 
							ZONEROLEBACKREQ *pstBackReq)
{
	pstRoleData->AvatarProf = pstBackReq->RoleData.AvatarProf;
	pstRoleData->DiathesisInfo = pstBackReq->RoleData.DiathesisInfo;
	pstRoleData->MiscInfo = pstBackReq->RoleData.MiscInfo;
	pstRoleData->Package = pstBackReq->RoleData.Package;
	pstRoleData->RoleDetail = pstBackReq->RoleData.RoleDetail;
	pstRoleData->SkillInfo = pstBackReq->RoleData.SkillInfo;
	pstRoleData->StatusInfo = pstBackReq->RoleData.StatusInfo;
	pstRoleData->Wear= pstBackReq->RoleData.Wear;
	return 0;
}*/


SECONDARYSKILLDEF  * cs_find_secondary_skill_def(CS_SVRENV *pstEnv, unsigned short iSkillType, unsigned short iLevel)
{
	int iIndex;
	int iEqu;
	SECONDARYSKILLDEF stSkill;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stSkill.SkillType = iSkillType;
	stSkill.Level = iLevel;
	iIndex = bsearch_int(&stSkill, pstObj->sSecondarySkillDef, pstObj->iSecondarySkillDef, SECONDARY_SKILL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SECONDARYSKILLDEF *)pstObj->sSecondarySkillDef[iIndex];
	}

	return NULL;
}

int cs_set_role_lift_skill(CS_SVRENV *pstEnv,ROLEDATA *pstRoleData, SETLIFTSKILLREQ *pstReq)
{
	int i;
	 SECONDARYSKILLMISCINFO *pstInfo = &pstRoleData->MiscInfo.SecondarySkillMiscInfo;

	if (pstReq->SkillOpType == SETLIFTSKILL_OP_DEL)
	{
		for (i=pstInfo->SkillNum-1; i>=0; i--)
		{
			if (pstReq->SkillType == pstInfo->SkillInfo[i].SkillType)
			{
				pstInfo->SkillNum -- ;
				if (pstInfo->SkillNum != i)
				{
					memcpy(&pstInfo->SkillInfo[i], &pstInfo->SkillInfo[pstInfo->SkillNum], sizeof(pstInfo->SkillInfo[0]));
				}
				break;
			}
		}

		if (i<0)
		{
			return -1;
		}
	}
	else if (pstReq->SkillOpType ==SETLIFTSKILL_OP_MOD)
	{
		for (i=0; i<pstInfo->SkillNum; i++)
		{
			if (pstReq->SkillType == pstInfo->SkillInfo[i].SkillType)
			{
				char cFlag;
				if (cs_find_secondary_skill_def(pstEnv, pstReq->SkillType,pstReq->SkillLvl))
				{
					pstInfo->SkillInfo[i].SkillLevel = pstReq->SkillLvl;
				}
				else
				{
					return -1;
				}
				pstInfo->SkillInfo[i].SkillVal = pstReq->SkillVal;
				if (pstReq->RecipeOpType >0 && pstReq->RecipeID >= MAX_SECONDARY_SKILL_RECIPE*8)
				{
					return -1;
				}

				cFlag = 1 << (pstReq->RecipeID%8); 
				if (pstReq->RecipeOpType == RECIPE_OP_DEL)
				{
					pstInfo->SkillInfo[i].RecipeInfo[pstReq->RecipeID/8] &=~cFlag;
				}
				else if (pstReq->RecipeOpType == RECIPE_OP_ADD)
				{
					pstInfo->SkillInfo[i].RecipeInfo[pstReq->RecipeID/8] |= cFlag;
				}
				
				break;
			}
		}

		if (i>= pstInfo->SkillNum)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int cs_db_select_res(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn,TORMPKG* pstPkg)
{
	unsigned int uiGodCoin;
	ZONEBLOCKPLAYERREQ stBlockReq;
	ZONESILENCEROLEREQ stSilenceReq;
	//ZONESETROLEBASEINFOREQ stSetBsInfoReq;
	ZONESETROLEDETAILREQ stSetDetailReq;
	ZONESETROLESKILLINFOREQ stSetSkillInfoReq;
	//ZONESETROLEFITTINGSREQ stSetFittingReq;
	ZONESETROLEITEMLISTREQ stSetItemReq;
	ZONESETROLEDIATHESISREQ stSetDiathesisReq;
	ZONEDELROLETASKREQ stDelTaskReq;
	ZONECHGROLEPOSREQ stChgRolePosReq;
	ZONESETROLERIDEINFOREQ stSetRideInfoReq;
	ZONESETROLEPETINFOREQ stSetPetInfoReq;
	ZONESETROLEPACKAGEREQ stSetPackageReq;
	
	ROLEDATA stRoleData;
	int iRet = 0;
	
	if (TORM_ERR_IS_ERROR(pstPkg->Body.SelectRes.ErrNo))
	{
		return -1;
	}
	if (1 != pstPkg->Body.SelectRes.SelectResult.ResultNum)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleData, sizeof(stRoleData)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		return -1;
	}

	if(pstAsyn->Cmd == BLOCKPLAYER_REQ)
	{
		memcpy(&stBlockReq, pstAsyn->Data,pstAsyn->iLen);
		if(stBlockReq.Type == 0)
		{
			stRoleData.BlockEnd = (stBlockReq.Time <0) ? -1: stBlockReq.Time + pstAppCtx->stCurr.tv_sec;
			stRoleData.BlockStart = pstAppCtx->stCurr.tv_sec;
		}
		else if(stBlockReq.Type == 1)
		{
			stRoleData.BlockEnd = 0;
			stRoleData.BlockStart = 0;
		}
		else
		{
			return -1;
		}
	}
	else if (pstAsyn->Cmd == CHG_ROLE_POS_REQ)
	{
		if(pstAsyn->iLen != sizeof(stChgRolePosReq))
		{
			return -1;
		}
		memcpy(&stChgRolePosReq, pstAsyn->Data,pstAsyn->iLen);

		stRoleData.Map = stChgRolePosReq.Map;
		stRoleData.Pos.X = stChgRolePosReq.X;
		stRoleData.Pos.Y = stChgRolePosReq.Y;

		stChgRolePosReq.Uin = stRoleData.Uin;
		memcpy(pstAsyn->Data, &stChgRolePosReq,pstAsyn->iLen);
	}
	else if(pstAsyn->Cmd == SILENCE_ROLE_REQ)
	{
		memcpy(&stSilenceReq, pstAsyn->Data,pstAsyn->iLen);
		stRoleData.SilenceEnd = ((stSilenceReq.Time < 0) ? -1:pstAppCtx->stCurr.tv_sec + stSilenceReq.Time);
		if (stSilenceReq.Time == 0)
		{
			stRoleData.SilenceStart = 0;
		}
		else 
		{
			stRoleData.SilenceStart = pstAppCtx->stCurr.tv_sec;
		}
		
	}

	else if(pstAsyn->Cmd == SET_ROLE_DETAIL_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEDETAILREQ))
		{
			return -1;
		}

		memcpy(&stSetDetailReq, pstAsyn->Data, pstAsyn->iLen);
		if (check_role_isonline_by_name(pstEnv, stSetDetailReq.RoleName) == 0)
		{
			return -3;
		}

		//不允许修改金券
		uiGodCoin = stRoleData.RoleDetail.GodCoin;
		stRoleData.RoleDetail 	= stSetDetailReq.RoleDetailInfo;
		stRoleData.RoleDetail.GodCoin = uiGodCoin;
		stRoleData.Map 		= stSetDetailReq.MapID;
		stRoleData.Pos.X		= stSetDetailReq.X;
		stRoleData.Pos.Y 		= stSetDetailReq.Y;
		stRoleData.Perm 		= stSetDetailReq.Perm;
		stRoleData.BlockEnd 	= stSetDetailReq.BolckTime;
		stRoleData.SilenceEnd 	= stSetDetailReq.SilenceTime;

		if (stSetDetailReq.Level > MAX_LEVEL)
			stSetDetailReq.Level = MAX_LEVEL;
		stRoleData.Level 		= stSetDetailReq.Level;
		
		
		// uin和roleid赋值    更新uincach要用
		stSetDetailReq.Uin		= stRoleData.Uin;
		stSetDetailReq.RoleID  	= stRoleData.RoleID;
		memcpy(pstAsyn->Data, &stSetDetailReq, pstAsyn->iLen);
	}
	else if(pstAsyn->Cmd == SET_ROLE_SKILLINFO_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLESKILLINFOREQ))
		{
			return -1;
		}
		memcpy(&stSetSkillInfoReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stSetSkillInfoReq.RoleName) == 0)
		{
			return -3;
		}

		iRet = set_skill_info(pstEnv,&stRoleData,&stRoleData.SkillInfo, &stSetSkillInfoReq);
		if (iRet < 0)
			return iRet;
		//if(set_skill_info(pstEnv,&stRoleData.SkillInfo, &stSetSkillInfoReq) < 0)
		//{
		//	return -1;
		//}
	}
	/*else if(pstAsyn->Cmd == SET_ROLE_FITTING_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEFITTINGSREQ))
		{
			return -1;
		}
		memcpy(&stSetFittingReq, pstAsyn->Data, pstAsyn->iLen);
		if( set_fitting_info(pstEnv,&stRoleData.FittingsInfo, &stSetFittingReq) < 0)
		{
			return -1;
		}
	}*/

	else if(pstAsyn->Cmd == SET_ROLE_DIATHESIS_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEDIATHESISREQ))
		{
			return -1;
		}
		memcpy(&stSetDiathesisReq, pstAsyn->Data, pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stSetDiathesisReq.RoleName) == 0)
		{
			return -3;
		}
		
		if(set_diathesis_info(pstEnv,&stRoleData.DiathesisInfo, &stSetDiathesisReq) < 0)
		{
			return -1;
		}
	}
	else if (pstAsyn->Cmd == DEL_ROLE_TASK_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONEDELROLETASKREQ))
		{
			return -1;
		}
		memcpy(&stDelTaskReq, pstAsyn->Data, pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stDelTaskReq.RoleName) == 0)
		{
			return -3;
		}
		
		if(del_role_task_by_id(pstEnv, &stRoleData, stDelTaskReq.TaskID) < 0)
		{
			return -1;
		}
	}
	
	else if(pstAsyn->Cmd == SET_ROLE_ITEMLIST_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEITEMLISTREQ))
		{
			return -1;
		}
		memcpy(&stSetItemReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stSetItemReq.RoleName) == 0)
		{
			return -3;
		}
		
		if(set_item_info(pstAppCtx, pstEnv,&stRoleData.Package,&stRoleData.Wear, &stSetItemReq) < 0)
		{
			return -1;
		}
	}
	else if(pstAsyn->Cmd == SET_ROLE_RIDE_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLERIDEINFOREQ))
		{
			return -1;
		}
		memcpy(&stSetRideInfoReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stSetRideInfoReq.RoleName) == 0)
		{
			return -3;
		}
		
		if(set_ride_info(pstEnv,&stRoleData.MiscInfo.RideList, &stSetRideInfoReq) < 0)
		{
			return -1;
		}
		else
		{
			if (	stSetRideInfoReq.Type == ROLE_RIDE_DEL && 
				stRoleData.MiscInfo.RideItemID == stSetRideInfoReq.ZoneRoleRideData.ItemID)
			{
				stRoleData.MiscInfo.Flag &= ~ROLE_LOGOUT_RIDE;
				stRoleData.MiscInfo.Flag &= ~ROLE_LOGOUT_FLY;
				stRoleData.MiscInfo.RideItemID = 0;
				stRoleData.MiscInfo.RideLevel = 0;
			}
		}
	}
	else if(pstAsyn->Cmd == SET_ROLE_PACKAGE_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEPACKAGEREQ))
		{
			return -1;
		}
		memcpy(&stSetPackageReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stSetPackageReq.RoleName) == 0)
		{
			return -3;
		}
		
		if(cs_package_enlarge(&stRoleData, &stSetPackageReq) < 0)
		{
			return -1;
		}
	}
	else if(pstAsyn->Cmd == SET_ROLE_QIYUE_REQ)
	{
		SETROLEQIYUEREQ stSetRoleQiYueReq;
		if(pstAsyn->iLen != sizeof(SETROLEQIYUEREQ))
		{
			return -1;
		}
		memcpy(&stSetRoleQiYueReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stSetRoleQiYueReq.RoleName) == 0)
		{
			return -3;
		}
		
		if(set_role_qiyue_req(pstEnv,&stRoleData, &stSetRoleQiYueReq) < 0)
		{
			return -1;
		}
	}	
	else if(pstAsyn->Cmd == ADD_ROLE_QIYUE_REQ)
	{
		ADDROLEQIYUEREQ stAddRoleQiYueReq;
		if(pstAsyn->iLen != sizeof(ADDROLEQIYUEREQ))
		{
			return -1;
		}
		memcpy(&stAddRoleQiYueReq,pstAsyn->Data,pstAsyn->iLen);
		if (check_role_isonline_by_name(pstEnv, stAddRoleQiYueReq.RoleName) == 0)
		{
			return -3;
		}
		
		if(add_role_qiyue_req(pstEnv,&stRoleData, &stAddRoleQiYueReq) < 0)
		{
			return -1;
		}
	}	
	else if(pstAsyn->Cmd == DEL_ROLE_QIYUE_REQ)
	{
		DELROLEQIYUEREQ stDelRoleQiYueReq;
		if(pstAsyn->iLen != sizeof(DELROLEQIYUEREQ))
		{
			return -1;
		}
		memcpy(&stDelRoleQiYueReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stDelRoleQiYueReq.RoleName) == 0)
		{
			return -3;
		}
		
		if(del_role_qiyue_req(pstEnv,&stRoleData, &stDelRoleQiYueReq) < 0)
		{
			return -1;
		}
	}	
	else if (pstAsyn->Cmd == MODIFY_ROLE_FAIRY_REQ)
	{
		ZONESETROLEFAIRYREQ stSetFairyReq;
		if(pstAsyn->iLen != sizeof(ZONESETROLEFAIRYREQ))
		{
			return -1;
		}

		memcpy(&stSetFairyReq, pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stSetFairyReq.RoleName) == 0)
		{
			return -3;
		}
		
		if (set_role_fairy(pstAppCtx,pstEnv,&stRoleData,&stSetFairyReq) < 0)
		{
			return -1;
		}
	}
	else if(pstAsyn->Cmd == SET_ROLE_PET_REQ)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEPETINFOREQ))
		{
			return -1;
		}
		memcpy(&stSetPetInfoReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stSetPetInfoReq.RoleName) == 0)
		{
			return -3;
		}
			
		if(set_pet_info(pstEnv,&stRoleData.Package.PetPackage, &stSetPetInfoReq) < 0)
		{
			return -1;
		}
	}
	else if(pstAsyn->Cmd == ADD_ROLE_DESIGNATION_REQ)
	{
		ZONEADDROLEDESIGNATIONREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		if(add_role_designation_req(pstEnv, pstAsyn,
		                            &stRoleData.MiscInfo.DesignationInfo,
		                            &stReq) < 0)
		{
			return -1;
		}
	}
	else if(pstAsyn->Cmd == DEL_ROLE_DESIGNATION_REQ)
	{
		ZONEDELROLEDESIGNATIONREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		if(del_role_designation_req(pstEnv, pstAsyn,
		                            &stRoleData.MiscInfo.DesignationInfo,
		                            &stReq) < 0)
		{
			return -1;
		}
	}
	else if(pstAsyn->Cmd == GET_ROLE_DESIGNATION_LIST_REQ)
	{
		ZONEGETROLEDESIGNATIONLISTREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		return get_role_designation_list_req(
			pstEnv,
			pstAsyn,
			&stRoleData.MiscInfo.DesignationInfo,
			&stReq); // 不要再update鸟
	}
	else if(pstAsyn->Cmd == SET_ROLE_RESPACT_REQ)
	{
		ZONESETROLERESPACTREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		if(set_role_respact_req(pstEnv, pstAsyn,
		                        &stRoleData.MiscInfo.RespactInfo,
		                        &stReq) < 0)
		{
			return -1;
		}
	}
	else if(pstAsyn->Cmd == DEL_ROLE_RESPACT_REQ)
	{
		ZONESETROLERESPACTREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		if(del_role_respact_req(pstEnv, pstAsyn,
		                        &stRoleData.MiscInfo.RespactInfo,
		                        &stReq) < 0)
		{
			return -1;
		}
	}
	else if(pstAsyn->Cmd == GET_ROLE_RESPACT_LIST_REQ)
	{
		ZONEGETROLERESPACTLISTREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		return get_role_respact_list_req(pstEnv, pstAsyn,
		                                 &stRoleData.MiscInfo.RespactInfo,
		                                 &stReq);
	}
	else if(pstAsyn->Cmd == SET_ROLE_PERM_REQ)
	{
		ZONESETROLEPERMREQ stReq;
		if (pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);
		if (stRoleData.Perm != stReq.Perm )
		{
			stRoleData.Perm = stReq.Perm;
		}
		else
		{
			return -1;
		}
		
	}
	else if(pstAsyn->Cmd == ROLE_DROP_RESTORE_REQ)
	{
		ROLEDROPRESTOREREQ stDropRestoreReq;
		
		if(pstAsyn->iLen != sizeof(stDropRestoreReq))
		{
			return -1;
		}
		memcpy(&stDropRestoreReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stDropRestoreReq.RoleName) == 0)
		{
			return -3;
		}

		if(0> role_drop_restore(pstAppCtx, pstEnv, &stRoleData, &stDropRestoreReq))
		{
			return -1;
		}
	}
	else if (pstAsyn->Cmd == ROLE_BACK_RECORD_REQ)
	{
		ZONEROLEBACKREQ stReq;
		LOGROLESNAPSHOT *pstSanpshot = (LOGROLESNAPSHOT *)malloc(sizeof(LOGROLESNAPSHOT));
		if (!pstSanpshot)
			return -1;

		memcpy(&stReq,pstAsyn->Data,sizeof(stReq));
		
		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		if (get_role_sanpshot(pstAppCtx,pstEnv,stReq.RoleName, stReq.BackData,pstSanpshot) < 0)
		{
			return -1;
		}
		stRoleData = pstSanpshot->LogData.LogRoleData;

		if (pstSanpshot)
			free(pstSanpshot);
	}
	else if(pstAsyn->Cmd == ROLE_HATCH_PET_REQ)
	{
		ZONEROLEHATCHPETREQ stHatchReq;
		
		if(pstAsyn->iLen != sizeof(stHatchReq))
		{
			return -1;
		}
		memcpy(&stHatchReq,pstAsyn->Data,pstAsyn->iLen);

		if (check_role_isonline_by_name(pstEnv, stHatchReq.RoleName) == 0)
		{
			return -3;
		}

		if(0> cs_hatch_pet(pstAppCtx, pstEnv, &stRoleData, &stHatchReq))
		{
			return -1;
		}
	}
	
	else if (pstAsyn->Cmd == SET_SAFELOCK_REQ)
	{
		SETSAFELOCKREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
			return -1;

		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		if (stRoleData.Level < SAFELOCK_LEVEL)
			return -1;
		if (stReq.SafeLockPassWd[0] == 0)
			return -1;

		strncpy(stRoleData.MiscInfo.LockPassWD,stReq.SafeLockPassWd, sizeof(stRoleData.MiscInfo.LockPassWD));

		
	}
	else if (pstAsyn->Cmd == SET_SECOND_PWD_REQ)   //lzk
	{
		SETSECONDPWDREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
			return -1;

		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		//if (stReq.SecondPassWd[0] == 0)
		//	return -1;

		strncpy(stRoleData.SecondPwd,stReq.SecondPassWd, sizeof(stRoleData.SecondPwd));	
	}
	else if (pstAsyn->Cmd == SET_LIFT_SKILL_REQ)
	{
		SETLIFTSKILLREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
			return -1;
		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		if (check_role_isonline_by_name(pstEnv, stReq.RoleName) == 0)
		{
			return -3;
		}

		if ( cs_set_role_lift_skill(pstEnv,&stRoleData, &stReq) < 0)
		{
			return -4;
		}
		
	}
	else
	{
		return -1;
	}
	
	if(cs_db_role_update(pstAppCtx, pstEnv, &stRoleData,pstAsyn) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_db_role_update fail");
		return -2;
	}		
	return  0;
}

int cs_role_drop_info_update(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn, ROLEDROP *pstRoleDrop, ROLEDROPRESTOREREQ *pstReq)
{
	int i;
	TORMPKG stPkg;
	TORMUPDATEREQ *pstUpdateReq = &stPkg.Body.UpdateReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	
	switch(pstReq->Type)
	{
		case ROLE_DROP_GRID:
			for(i=0; i< pstRoleDrop->DropData.GridNum; i++)
			{
				if(pstRoleDrop->DropData.DropGrids[i].Grid.GridData.RoleItem.WID == 
					pstReq->RestoreInfo.Grid.GridData.RoleItem.WID)
				{
					pstRoleDrop->DropData.DropGrids[i].RestoreFlag = 1;
				}
			}
			break;
		case ROLE_DROP_PET:
			for(i=0; i< pstRoleDrop->DropData.PetNum; i++)
			{
				if(pstRoleDrop->DropData.DropPets[i].Pet.WID == pstReq->RestoreInfo.Pet.WID)
				{
					pstRoleDrop->DropData.DropPets[i].RestoreFlag = 1;
				}
			}
			break;
		default:
			return -1;
			break;
	}

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pstRoleDrop;
	stMetaData.iBuff = sizeof(ROLEDATA);

	//memset(&stPkg.Head, 0, sizeof(stPkg.Head));
	//memset(&stPkg.Body, 0, sizeof(stPkg.Body));
	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_UPDATE_REQ, "RoleDrop", pstRoleDrop->Uin);

	if(pstAsyn->Cmd == ROLE_DROP_RESTORE_SELECT_RES)
	{
		pstAsyn->Cmd = ROLE_DROP_UPDATE_RES;
	}
	else
	{
		return -1;
	}
	
	stPkg.Head.AsynCallBackDataLen = sizeof(*pstAsyn);
	memcpy(stPkg.Head.AsynCallBackData, pstAsyn, stPkg.Head.AsynCallBackDataLen);
	
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstUpdateReq->OperInfo.WhereDef[0] = 0;
	pstUpdateReq->DoInsert = 0;
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -1;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, &stPkg, pstEnv->iMiscDBID))
	{
		return -1;
	}
	
	return 0;
}

int cs_role_drop_select_res(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn,TORMPKG* pstPkg)
{
	ROLEDROP stRoleDrop;
	SSPKG stSSPkg;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);

	memset(&stRoleDrop, 0, sizeof(stRoleDrop));
	
	if (pstPkg->Body.SelectRes.SelectResult.ResultNum == 1)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleDrop, sizeof(stRoleDrop)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
			return -1;
		}
	}

	if (pstAsyn->Cmd == GET_ROLE_DROP_REQ)
	{
		ZONEGETROLEDROPREQ stReq;
		ZONEGETROLEDROPRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.GetRoleDropRes;
		
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);
		
		
		stSSPkg.Body.CsCtrL.Cmd = GET_ROLE_DROP_RES;
		memcpy (&pstRes->RoleName, &stReq.RoleName, sizeof(pstRes->RoleName));
		pstRes->DropData = stRoleDrop.DropData;
	}
	else if(pstAsyn->Cmd == ROLE_DROP_RESTORE_SELECT_RES)
	{
		ROLEDROPRESTOREREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);
			
		//更新
		if (pstPkg->Body.SelectRes.SelectResult.ResultNum == 1)
		{
			
			return cs_role_drop_info_update(pstAppCtx, pstEnv, pstAsyn, &stRoleDrop, &stReq);
		}
		//无记录则无需更新标记
		else
		{
			ROLEDROPRESTORERES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.RoleDropRestoreRes;
			
			stSSPkg.Body.CsCtrL.Cmd = ROLE_DROP_RESTORE_RES;
			memcpy (&pstRes->RoleName, &stReq.RoleName, sizeof(pstRes->RoleName));
		}
		
	}

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);

	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -2;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff,pstAsyn->ConnIdx,&pstAsyn->IPInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -2;
	}

	return 0;
}

int cs_role_drop_update_res(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn,TORMPKG* pstPkg)
{
	UNUSED(pstAppCtx);
	SSPKG stSSPkg;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;
	//int iResult;

	memset(&stSSPkg, 0,sizeof(stSSPkg));
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	if (TORM_ERR_IS_ERROR(pstPkg->Body.UpdateRes.ErrNo) )
	{
		return   -1;				// 不成功
	}
	
	if(pstAsyn->Cmd == ROLE_DROP_UPDATE_RES)
	{
		ROLEDROPRESTOREREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		
		memcpy(&stReq,pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = ROLE_DROP_RESTORE_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.RoleDropRestoreRes.RoleName, stReq.RoleName);
	}
	else
	{
		return -1;
	}
	

	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -2;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff,pstAsyn->ConnIdx,&pstAsyn->IPInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -2;
	}

	return 0;
}


int cs_db_update_res(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn,TORMPKG* pstPkg)
{
	ZONEBLOCKPLAYERREQ stBlockReq;
	ZONESILENCEROLEREQ stSilenceReq;
	//ZONESETROLEBASEINFOREQ stSetBsInfoReq;
	ZONESETROLEDETAILREQ stSetDetailReq;
	ZONESETROLESKILLINFOREQ stSetSkillReq;
	//ZONESETROLEFITTINGSREQ stSetFitReq;
	ZONESETROLEITEMLISTREQ stSetItemReq;
	ZONESETROLEDIATHESISREQ stSetDiathesisReq;
	ZONEDELROLETASKREQ stDelTaskReq;
	ZONECHGROLEPOSREQ stChgRolePosReq;
	ZONESETROLERIDEINFOREQ stSetRideReq;
	ZONESETROLEPETINFOREQ stSetPetReq;
	ZONESETROLEPACKAGEREQ stSetPackageReq;

	UNUSED(pstAppCtx);
	
	SSPKG stSSPkg;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;
	//int iResult;

	memset(&stSSPkg, 0,sizeof(stSSPkg));
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	if (TORM_ERR_IS_ERROR(pstPkg->Body.UpdateRes.ErrNo) )
	{
		return   -1;				// 不成功
	}
	
	if(pstAsyn->Cmd == BLOCK_ROLE_DB_UPDATE_RES)
	{
		memcpy(&stBlockReq, pstAsyn->Data, pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = BLOCKPLAYER_RES;
		stSSPkg.Body.CsCtrL.CsCmd.BlockPlayerRes.Type = stBlockReq.Type;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.BlockPlayerRes.RoleName, stBlockReq.RoleName);
	}
	else if(pstAsyn->Cmd == CHG_ROLE_POS_UP_RES)
	{
		memcpy(&stChgRolePosReq, pstAsyn->Data, pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = CHG_ROLE_POS_RES;
		strncpy(stSSPkg.Body.CsCtrL.CsCmd.ChgRolePosRes.RoleName, stChgRolePosReq.RoleName, 
				sizeof(stSSPkg.Body.CsCtrL.CsCmd.ChgRolePosRes.RoleName));

		// 通知zone_svr 更新Uincach
		cs_zone_svr_chg_pos(pstAppCtx, pstEnv,stChgRolePosReq.Map,
								stChgRolePosReq.Uin, stChgRolePosReq.RoleName);
		
	}
	else if(pstAsyn->Cmd == SILENCE_ROLE_DB_UPDATE_RES)
	{
		memcpy(&stSilenceReq, pstAsyn->Data, pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SILENCE_ROLE_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SilenceRoleRes.RoleName,stSilenceReq.RoleName);
	}
	
	else if(pstAsyn->Cmd == SET_ROLE_DETAIL_DB_UPDATE_RES)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEDETAILREQ))
		{
			return -1;
		}
		memcpy(&stSetDetailReq,pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_DETAIL_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRoleDetailRes.RoleName,stSetDetailReq.RoleName);

		// 设置完角色数据 去zone_svr更新uincach
		cs_zone_svr_up_uin(pstAppCtx,pstEnv, (char*)&stSetDetailReq, sizeof(stSetDetailReq), SET_ROLE_DETAIL_REQ);
		
	}
	else if(pstAsyn->Cmd == SET_ROLE_SKILL_DB_UPDATE_RES)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLESKILLINFOREQ))
		{
			return -1;
		}
		memcpy(&stSetSkillReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_SKILLINFO_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRoleSkillInfoRes.RoleName, stSetSkillReq.RoleName);
	}
	/*else if(pstAsyn->Cmd == SET_ROLE_FITTING_DB_UPDATE_RES)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEFITTINGSREQ))
		{
			return -1;
		}
		memcpy(&stSetFitReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_FITTING_RES;
	
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRoleFittingsRes.RoleName,stSetFitReq.RoleName);
	}*/

	else if(pstAsyn->Cmd == SET_ROLE_DIATHESIS_DB_UPDATA_RES)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEDIATHESISREQ))
		{
			return -1;
		}
		memcpy(&stSetDiathesisReq,pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_DIATHESIS_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRoleDiathesisRes.RoleName, stSetDiathesisReq.RoleName);
	}
	else if(pstAsyn->Cmd == SET_ROLE_TASK_DB_UPDATA_RES)
	{
		if(pstAsyn->iLen != sizeof(stDelTaskReq))
		{
			return -1;
		}
		memcpy(&stDelTaskReq,pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = DEL_ROLE_TASK_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.DelRoleTaskRes.RoleName, stDelTaskReq.RoleName);
	}
	else if(pstAsyn->Cmd == SET_ROLE_ITEM_DB_UPDATE_RES)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEITEMLISTREQ))
		{
			return -1;
		}
		memcpy(&stSetItemReq,pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_ITEMLIST_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRoleItemListRes.RoleName, stSetItemReq.RoleName);
	}
	else if(pstAsyn->Cmd == SET_ROLE_RIDE_DB_UPDATE_RES)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLERIDEINFOREQ))
		{
			return -1;
		}
		memcpy(&stSetRideReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_RIDE_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRoleRideInfoRes.RoleName, stSetRideReq.RoleName);
	}
	else if(pstAsyn->Cmd == SET_ROLE_PACKAGE_DB_UPDATE_RES)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEPACKAGEREQ))
		{
			return -1;
		}
		memcpy(&stSetPackageReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_PACKAGE_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRolePackageRes.RoleName, stSetPackageReq.RoleName);
	}
	else if(pstAsyn->Cmd == SET_ROLE_QIYUE_DB_UPDATE_RES)
	{
		SETROLEQIYUEREQ stSetRoleQiyueReq;
		if(pstAsyn->iLen != sizeof(SETROLEQIYUEREQ))
		{
			return -1;
		}
		memcpy(&stSetRoleQiyueReq, pstAsyn->Data,sizeof(stSetRoleQiyueReq));
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_QIYUE_RES;
		stSSPkg.Body.CsCtrL.CsCmd.SetRoleQiYueRes.Result = 0;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRoleQiYueRes.RoleName, stSetRoleQiyueReq.RoleName);
	}
	else if(pstAsyn->Cmd == ADD_ROLE_QIYUE_DB_UPDATE_RES)
	{
		ADDROLEQIYUEREQ stAddRoleQiyueReq;
		if(pstAsyn->iLen != sizeof(ADDROLEQIYUEREQ))
		{
			return -1;
		}
		memcpy(&stAddRoleQiyueReq, pstAsyn->Data,sizeof(stAddRoleQiyueReq));
		stSSPkg.Body.CsCtrL.Cmd = ADD_ROLE_QIYUE_RES;
		stSSPkg.Body.CsCtrL.CsCmd.AddRoleQiYueRes.Result = 0;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.AddRoleQiYueRes.RoleName, stAddRoleQiyueReq.RoleName);
	}
	else if(pstAsyn->Cmd == DEL_ROLE_QIYUE_DB_UPDATE_RES)
	{
		DELROLEQIYUEREQ stDelRoleQiyueReq;
		if(pstAsyn->iLen != sizeof(DELROLEQIYUEREQ))
		{
			return -1;
		}
		memcpy(&stDelRoleQiyueReq, pstAsyn->Data,sizeof(stDelRoleQiyueReq));
		stSSPkg.Body.CsCtrL.Cmd = DEL_ROLE_QIYUE_RES;
		stSSPkg.Body.CsCtrL.CsCmd.DelRoleQiYueRes.Result = 0;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.DelRoleQiYueRes.RoleName, stDelRoleQiyueReq.RoleName);
	}	
	else if (pstAsyn->Cmd == MODIFY_FAIRY_DB_UPDATE_RES)
	{
		ZONESETROLEFAIRYREQ stRoleFairyReq;
		if (pstAsyn->iLen != sizeof(ZONESETROLEFAIRYREQ))
		{
			return -1;
		}

		memcpy(&stRoleFairyReq,  pstAsyn->Data, pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = MODIFY_ROLE_FAIRY_RES;
		stSSPkg.Body.CsCtrL.CsCmd.SetRoleFairyRes.OpType = stRoleFairyReq.OpType;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRoleFairyRes.RoleName, stRoleFairyReq.RoleName);
	}
	else if(pstAsyn->Cmd == SET_ROLE_PET_DB_UPDATE_RES)
	{
		if(pstAsyn->iLen != sizeof(ZONESETROLEPETINFOREQ))
		{
			return -1;
		}
		memcpy(&stSetPetReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_PET_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetRolePetInfoRes.RoleName, stSetPetReq.RoleName);
	}
	else if(pstAsyn->Cmd == ADD_ROLE_DESIGNATION_DB_UPDATE_RES)
	{
		stSSPkg.Body.CsCtrL.Cmd = ADD_ROLE_DESIGNATION_RES;
		stSSPkg.Body.CsCtrL.CsCmd.AddRoleDesignationRes.Succeeded = 1;
	}
	else if(pstAsyn->Cmd == DEL_ROLE_DESIGNATION_DB_UPDATE_RES)
	{
		stSSPkg.Body.CsCtrL.Cmd = DEL_ROLE_DESIGNATION_RES;
		stSSPkg.Body.CsCtrL.CsCmd.DelRoleDesignationRes.Succeeded = 1;
	}
	else if(pstAsyn->Cmd == SET_ROLE_RESPACT_DB_UPDATE_RES)
	{
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_RESPACT_RES;
		stSSPkg.Body.CsCtrL.CsCmd.SetRoleRespactRes.Succeeded = 1;
	}
	else if(pstAsyn->Cmd == DEL_ROLE_RESPACT_DB_UPDATE_RES)
	{
		stSSPkg.Body.CsCtrL.Cmd = DEL_ROLE_RESPACT_RES;
		stSSPkg.Body.CsCtrL.CsCmd.DelRoleRespactRes.Succeeded = 1;
	}
	else if (pstAsyn->Cmd == SET_ROLE_PERM_DB_UPDATE_RES)
	{
		ZONESETROLEPERMREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_ROLE_PERM_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetPermRes.RoleName, stReq.RoleName);	
	}
	else if(ROLE_BACK_RECORD_UP_RES == pstAsyn->Cmd )
	{
		ZONEROLEBACKREQ stReq;
		if (pstAsyn->iLen != sizeof(stReq))
			return -1;
		
		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = ROLE_BACK_RECORD_RES;
		strncpy(stSSPkg.Body.CsCtrL.CsCmd.RoleBackRes.RoleName, stReq.RoleName,
				sizeof(stSSPkg.Body.CsCtrL.CsCmd.RoleBackRes.RoleName));	
	}
	else if (pstAsyn->Cmd == ROLE_HATCH_PET_UPDATE_RES)
	{
		ZONEROLEHATCHPETREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		
		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = ROLE_HATCH_PET_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.RoleHatchPetRes.RoleName, stReq.RoleName);	
	}
	else if (SET_SAFELOCK_UP_RES == pstAsyn->Cmd)
	{
		SETSAFELOCKREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_SAFELOCK_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetSafeLockRes.RoleName, stReq.RoleName);	
	}
	else if (SET_SECOND_PWD_UP_RES == pstAsyn->Cmd) //lzk
	{
		SETSECONDPWDREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_SECOND_PWD_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetSecondPwdRes.RoleName, stReq.RoleName);	
	}
	else if(SET_LIFT_SKILL_UP_RES ==  pstAsyn->Cmd)
	{
		SETLIFTSKILLREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
		stSSPkg.Body.CsCtrL.Cmd = SET_LIFT_SKILL_RES;
		strcpy(stSSPkg.Body.CsCtrL.CsCmd.SetLiftSkillRes.RoleName, stReq.RoleName);	
	}
	else
	{
		return -1;
	}
	

	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -2;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff,pstAsyn->ConnIdx,&pstAsyn->IPInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -2;
	}

	
	
	return 0;
}

int cs_role_drop_restore_res(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn,TORMPKG* pstPkg)
{
	TORMPKG stPkg;
	TORMSELECTREQ *pstSelectReq = &stPkg.Body.SelectReq;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	ROLEDROP stRoleDrop;
	UNUSED(pstAppCtx);
	
	if (TORM_ERR_IS_ERROR(pstPkg->Body.UpdateRes.ErrNo) )
	{
		return   -1;				// 不成功
	}

	memset(&stRoleDrop, 0, sizeof(stRoleDrop));
	
	cs_init_torm_head(pstAppCtx, pstEnv, &stPkg.Head, TORM_SELECT_REQ, "RoleDrop", -1);
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	
	if(pstAsyn->Cmd == ROLE_DROP_RESTORE_ROLEDATA_RES)
	{
		ROLEDROPRESTOREREQ stReq;
		if(pstAsyn->iLen != sizeof(stReq))
		{
			return -1;
		}
		memcpy(&stReq, pstAsyn->Data,pstAsyn->iLen);
	
		pstAsyn->Cmd = ROLE_DROP_RESTORE_SELECT_RES;
		snprintf(pstSelectReq->OperInfo.WhereDef,sizeof(pstSelectReq->OperInfo.WhereDef),
				"where RoleName='%s'", stReq.RoleName);
	}
	else
	{
		return -1;
	}

	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)&stRoleDrop;
	stMetaData.iBuff = sizeof(stRoleDrop);
	
	stPkg.Head.AsynCallBackDataLen = sizeof(*pstAsyn);
	memcpy(stPkg.Head.AsynCallBackData, pstAsyn, stPkg.Head.AsynCallBackDataLen);
	
	if(0 != torm_make_req_msg(pstEnv->hTorm, &stNetData, &stPkg, &stMetaData, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail");
		return -2;
	}

	if (0 > cs_torm_send(pstAppCtx, pstEnv, &stPkg, pstEnv->iMiscDBID))
	{
		return -2;
	}
	
	return 0;
}

int cs_role_list_db_select_res(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn,TORMPKG* pstPkg)
{
	ZONEROLELISTREQ stListReq;
	int i;
	ROLELISTINFO stRoleListInfo;
	SSPKG stSSPkg;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;
	ZONEROLELISTRES *pstRoleListRes = &stSSPkg.Body.CsCtrL.CsCmd.RoleListRes;

	UNUSED(pstAppCtx);

	memset (pstRoleListRes, 0, sizeof(*pstRoleListRes));
	memcpy(&stListReq, pstAsyn->Data, pstAsyn->iLen);
	if (TORM_ERR_IS_ERROR(pstPkg->Body.SelectRes.ErrNo))
	{
		return -1;
	}
	if (pstPkg->Body.SelectRes.SelectResult.ResultNum < 0 )
	{
		return -1;
	}

	if (pstPkg->Body.SelectRes.SelectResult.ResultNum > CS_ROLE_LIST_NUM)
	{
		pstPkg->Body.SelectRes.SelectResult.ResultNum = CS_ROLE_LIST_NUM;
	}
	
	for(i=0; i<pstPkg->Body.SelectRes.SelectResult.ResultNum;i++)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleListInfo, sizeof(ROLELISTINFO)))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
			return -1;
		}

		if (pstRoleListRes->Uin == 0)
		{
			pstRoleListRes->Uin = stRoleListInfo.Uin;
		}

		if (stRoleListInfo.DelTime > 0)
		{
			pstRoleListRes->RoleList[i].IsDelete = 1;
		}
		pstRoleListRes->RoleList[i].RoleID = stRoleListInfo.RoleID;
		strncpy(pstRoleListRes->RoleList[i].RoleName, stRoleListInfo.RoleName, sizeof(pstRoleListRes->RoleList[i].RoleName));
		
	}

	pstRoleListRes->RoleNum = i;
	
	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	
	stSSPkg.Body.CsCtrL.Cmd = ROLE_LIST_RES;
	
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -2;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff,pstAsyn->ConnIdx,&pstAsyn->IPInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -2;
	}
	return 0;
}

int cs_clan_select_res(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv,CS_ASYN*pstAsyn,TORMPKG* pstPkg)
{
	CLAN stClan;
	SSPKG stSSPkg;
	ZONEGETCLANINFORES *pstGetClanInfoRes = &stSSPkg.Body.CsCtrL.CsCmd.GetClanInfoRes;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);

	(void)pstAppCtx;	

	if (TORM_ERR_IS_ERROR(pstPkg->Body.SelectRes.ErrNo))
	{
		printf("%s\n", torm_error_string(pstPkg->Body.SelectRes.ErrNo));
		return -1;
	}
	if (1 != pstPkg->Body.SelectRes.SelectResult.ResultNum)
	{
		return -1;
	}

	if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stClan, sizeof(stClan)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_fetch_one_meta fail");
		return -1;
	}

	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	
	if (pstAsyn->Cmd == GET_CLAN_INFO_REQ)
	{
		if (pstAsyn->iLen != sizeof(ZONEGETCLANINFOREQ))
		{
			return -1;
		}
		stSSPkg.Body.CsCtrL.Cmd = GET_CLAN_INFO_RES;
		memcpy (&pstGetClanInfoRes->ClanProf, &stClan.ClanProf, sizeof(stClan.ClanProf));
	}
	else
	{
		return -1;
	}

	if(ss_pack(pstEnv->pstSSHan, &stNet, &stSSPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -2;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff, stNet.iBuff,pstAsyn->ConnIdx,&pstAsyn->IPInfo) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "cs_send_to_clt fail");
		return -2;
	}

	return 0;
}

int cs_db_msg(TAPPCTX*pstAppCtx, CS_SVRENV*pstEnv, char *pszMsg, int iMsgLen)
{
	TDRDATA stData;
	TORMPKG stPkg;
	int iRet;
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	//ASYNREQ stAsyn;
	CS_ASYN stCsAsyn;
	
	stNet.iBuff = sizeof(szBuff);
	stNet.pszBuff = szBuff;

	stData.pszBuff = pszMsg;
	stData.iBuff = iMsgLen;
	iRet =  torm_attach_res_msg(pstEnv->hTorm, &stData, &stPkg, 0);
	if (0 >iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_attach_res_msg error:%s, iProtoVer=%d", torm_error_string(iRet), 0);
		return -1;
	}

	if(stPkg.Head.AsynCallBackDataLen <= 0)
	{
		return -1;
	}
	memcpy(&stCsAsyn, stPkg.Head.AsynCallBackData, stPkg.Head.AsynCallBackDataLen);

	tlog_info(pstEnv->pstLogCat, 0, 0, "get msg from db asyn cmd=%d", stCsAsyn.Cmd);
	
	switch(stCsAsyn.Cmd)
	{
	case BLOCKPLAYER_REQ:
	case SILENCE_ROLE_REQ: 
	case SET_ROLE_DETAIL_REQ:
	case SET_ROLE_SKILLINFO_REQ:
	//case SET_ROLE_FITTING_REQ:
	case SET_ROLE_ITEMLIST_REQ:
	case SET_ROLE_DIATHESIS_REQ:
	case DEL_ROLE_TASK_REQ:
	case CHG_ROLE_POS_REQ:
	case SET_ROLE_RIDE_REQ:
	case SET_ROLE_PACKAGE_REQ:
       case SET_ROLE_QIYUE_REQ:
	case ADD_ROLE_QIYUE_REQ:
	case DEL_ROLE_QIYUE_REQ:
	case MODIFY_ROLE_FAIRY_REQ:
	case SET_ROLE_PET_REQ:
	case ADD_ROLE_DESIGNATION_REQ:
	case DEL_ROLE_DESIGNATION_REQ:
	case GET_ROLE_DESIGNATION_LIST_REQ:
	case SET_ROLE_RESPACT_REQ:
	case DEL_ROLE_RESPACT_REQ:
	case GET_ROLE_RESPACT_LIST_REQ:
	case SET_ROLE_PERM_REQ:
	case ROLE_BACK_RECORD_REQ:
	case ROLE_DROP_RESTORE_REQ:
	case ROLE_HATCH_PET_REQ:
	case SET_SAFELOCK_REQ:
	case SET_LIFT_SKILL_REQ:
	case SET_SECOND_PWD_REQ:
		iRet = cs_db_select_res(pstAppCtx, pstEnv,&stCsAsyn,&stPkg);
		if (iRet == -3)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_ONLINE, stCsAsyn.Cmd);
		}
		else if (iRet == -1)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_SYS, stCsAsyn.Cmd);
		}
		else if (iRet == -2)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_TDR, stCsAsyn.Cmd);
		}

		else if (iRet == -4)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_ARGS, stCsAsyn.Cmd);
		}
		break;

	case BLOCK_ROLE_DB_UPDATE_RES:
	case SILENCE_ROLE_DB_UPDATE_RES:
	case SET_ROLE_DETAIL_DB_UPDATE_RES:
	case SET_ROLE_SKILL_DB_UPDATE_RES:
	case SET_ROLE_FITTING_DB_UPDATE_RES:
	case SET_ROLE_ITEM_DB_UPDATE_RES:
	case SET_ROLE_DIATHESIS_DB_UPDATA_RES:
	case SET_ROLE_TASK_DB_UPDATA_RES:
	case CHG_ROLE_POS_UP_RES:
	case SET_ROLE_RIDE_DB_UPDATE_RES:
	case SET_ROLE_PACKAGE_DB_UPDATE_RES:
	case SET_ROLE_QIYUE_DB_UPDATE_RES:
	case ADD_ROLE_QIYUE_DB_UPDATE_RES:
	case DEL_ROLE_QIYUE_DB_UPDATE_RES:
	case MODIFY_FAIRY_DB_UPDATE_RES:
	case SET_ROLE_PET_DB_UPDATE_RES:
	case SET_ROLE_PERM_DB_UPDATE_RES:
	case ADD_ROLE_DESIGNATION_DB_UPDATE_RES:
	case DEL_ROLE_DESIGNATION_DB_UPDATE_RES:
	case SET_ROLE_RESPACT_DB_UPDATE_RES:
	case DEL_ROLE_RESPACT_DB_UPDATE_RES:
	case ROLE_BACK_RECORD_UP_RES:
	case ROLE_HATCH_PET_UPDATE_RES:
	case SET_SAFELOCK_UP_RES:
	case SET_LIFT_SKILL_UP_RES:
	case SET_SECOND_PWD_UP_RES:
		iRet = cs_db_update_res(pstAppCtx, pstEnv,&stCsAsyn,&stPkg);
		if (iRet == -1)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_SYS, stCsAsyn.Cmd);
		}
		else if (iRet == -2)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_TDR, stCsAsyn.Cmd);
		}
		break;
	case ROLE_DROP_RESTORE_ROLEDATA_RES:
		iRet = cs_role_drop_restore_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		if (iRet == -1)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_SYS, stCsAsyn.Cmd);
		}
		else if (iRet == -2)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_TDR, stCsAsyn.Cmd);
		}
		break;
	case ROLE_DROP_RESTORE_SELECT_RES:
	case GET_ROLE_DROP_REQ:
		iRet = cs_role_drop_select_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		if (iRet == -1)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_SYS, stCsAsyn.Cmd);
		}
		else if (iRet == -2)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_TDR, stCsAsyn.Cmd);
		}
		break;
	case ROLE_DROP_UPDATE_RES:
		iRet = cs_role_drop_update_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		if (iRet == -1)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_SYS, stCsAsyn.Cmd);
		}
		else if (iRet == -2)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_TDR, stCsAsyn.Cmd);
		}
		break;
	case GET_ROLE_DETAIL_REQ:
	case GET_ROLE_SKILLINFO_REQ:
	//case GET_ROLE_FITTING_REQ:
	case GET_ROLE_ITEMLIST_REQ:
	case GET_ROLE_DIATHESIS_REQ:
	case GET_ROLE_TASK_REQ:
	case GET_ROLE_RIDE_REQ:
	case GET_ROLE_PACKAGE_REQ:
	case GET_ROLE_PET_REQ:
       case GET_ROLE_QIYUELIST_REQ:
	case GET_ROLE_FRIEND_REQ:
	case GET_ROLE_BLACK_REQ:
	case GET_ROLE_PACKAGE_ITEM_REQ:
	case GET_SAFELOCK_REQ:
	case GET_LIFT_SKILL_REQ:
		iRet = cs_get_role_info_to_clt(pstAppCtx,pstEnv,&stCsAsyn,&stPkg);
		if (iRet == -1)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_SYS, stCsAsyn.Cmd);
		}
		else if (iRet == -2)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_TDR, stCsAsyn.Cmd);
		} 
		break;
	
	case ROLE_LIST_REQ:
		iRet = cs_role_list_db_select_res(pstAppCtx,pstEnv,&stCsAsyn,&stPkg);
		if (iRet == -1)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_SYS, stCsAsyn.Cmd);
		}
		else if (iRet == -2)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_TDR, stCsAsyn.Cmd);
		} 
		break;

	case GET_CLAN_ALL_INFO_REQ:
		iRet = cs_db_clan_all_info_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case DEL_CLAN_MEMBER_REQ | 0x80000000:
		iRet = cs_db_will_del_clan_member_res(pstAppCtx, pstEnv, &stCsAsyn,
		                                      &stPkg);
		break;

	case DEL_CLAN_MEMBER_REQ:
		iRet = cs_db_del_clan_member_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case DISBAND_CLAN_REQ:
		iRet = cs_db_disband_clan_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case CLEAR_CLAN_DIPLOMATISM_REQ | 0x80000000:
		iRet = cs_db_will_clear_clan_diplomatism_res(pstAppCtx, pstEnv,
		                                             &stCsAsyn, &stPkg);
		break;

	case CLEAR_CLAN_DIPLOMATISM_REQ:
		iRet = cs_db_clear_clan_diplomatism_res(pstAppCtx, pstEnv, &stCsAsyn,
		                                        &stPkg);
		break;

	case RESTORE_ROLE_REQ | 0x80000000:
		iRet = cs_db_will_restore_role_res(pstAppCtx, pstEnv, &stCsAsyn,
		                                   &stPkg);
		break;

	case RESTORE_ROLE_REQ:
		iRet = cs_db_restore_role_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case GET_ROLE_MAIL_LIST_REQ:
		iRet = cs_db_role_maillist_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case GET_ROLE_MAIL_REQ:
		iRet = cs_db_role_maildata_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case DEL_ROLE_MAIL_REQ:
		iRet = cs_db_del_role_mail_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case SEND_MAIL_REQ:
		iRet = cs_db_ins_role_mail_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case COPY_ROLE_REQ:
		iRet = cs_db_select_role_for_copying_res(pstAppCtx, pstEnv, &stCsAsyn, 
			                                       &stPkg);
		break;

	case COPY_ROLE_RES:
		iRet = cs_db_update_role_for_copying_res(pstAppCtx, pstEnv, &stCsAsyn, 
			                                       &stPkg);
		break;

	case GET_CLAN_INFO_REQ:
		iRet =  cs_clan_select_res(pstAppCtx, pstEnv,&stCsAsyn,&stPkg);
		if (iRet == -1)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_SYS, stCsAsyn.Cmd);
		}
		else if (iRet == -2)
		{
			err_res(pstEnv, stCsAsyn.ConnIdx, &stCsAsyn.IPInfo, ERR_TDR, stCsAsyn.Cmd);
		} 
		break;

	case FIND_CLAN_REQ:
		iRet = cs_db_get_clan_list_res(pstAppCtx, pstEnv, &stCsAsyn, &stPkg);
		break;

	case DEL_CLAN_DIPLOMATISM_REQ | 0x80000000:
		iRet = cs_db_will_del_clan_diplomatism_res(pstAppCtx, pstEnv,
		                                             &stCsAsyn, &stPkg);
		break;

	case DEL_CLAN_DIPLOMATISM_REQ:
		iRet = cs_db_del_clan_diplomatism_res(pstAppCtx, pstEnv, &stCsAsyn,
		                                        &stPkg);
		break;

	case EXCHANGE_CLAN_LEADER_REQ | 0x80000000:
		iRet = cs_db_will_exchange_clan_leader_res(pstAppCtx, pstEnv, &stCsAsyn,
		                                           &stPkg);
		break;

	case EXCHANGE_CLAN_LEADER_REQ:
		iRet = cs_db_exchange_clan_leader_res(pstAppCtx, pstEnv, &stCsAsyn,
		                                      &stPkg);
		break;

	case SET_CLAN_BASE_ATTR_REQ | 0x80000000:
		iRet = cs_db_will_set_clan_base_attr_res(pstAppCtx, pstEnv, &stCsAsyn,
		                                           &stPkg);
		break;

	case SET_CLAN_BASE_ATTR_REQ:
		iRet = cs_db_set_clan_base_attr_res(pstAppCtx, pstEnv, &stCsAsyn,
		                                      &stPkg);
		break;
		
	default:
		iRet = -1;
		break;
	}

	return iRet;
}


static int cs_z_reload_server_res(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv,
			                    const ZONECTLRELOADSERVERRES *pstCtlRes)
{
	int iRet = 0;
	SSPKG stSSPkg;
	ZONERELOADSERVERRES *pstRes = &stSSPkg.Body.CsCtrL.CsCmd.ReloadServerRes;

	UNUSED(pstAppCtx);


	if(!pstCtlRes->Core.Succeeded)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,
		           "zone_svr: set clan base attr fail!");
	}
	iRet = pstCtlRes->Core.Succeeded ? ERR_NO : ERR_SYS;
	pstRes->Succeeded = pstCtlRes->Core.Succeeded;


	cs_init_sshead(&stSSPkg.Head, CS_CTRL);
	stSSPkg.Body.CsCtrL.Cmd = RELOAD_SERVER_RES;
	return cs_sendto_clt_pkg_x(pstEnv, pstCtlRes->Extra.ConnIdx,
				&pstCtlRes->Extra.IPInfo, &stSSPkg);
}

int cs_zone_svr_msg(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv, char*pszMsg,int iLen)
{
	SSPKG stZoneRes;
	ZONEBLOCKPLAYERREQ stBlockReq;
	ZONESILENCEROLEREQ stSilenceReq;

	
	SSPKG stCsConn;
	ZONEGETROLEDETAILRES *pstGetRoleInfoRes = &stCsConn.Body.CsCtrL.CsCmd.GetRoleDetailRes;
	ZONEGETROLESKILLINFORES *pstGetRoleSkillRes = &stCsConn.Body.CsCtrL.CsCmd.GetRoleSkillInfoRes;
	ZONEGETROLEDIATHESISRES *pstGetDiathsisRes = &stCsConn.Body.CsCtrL.CsCmd.GetRoleDiathesisRes;
	ZONEGETROLETASKRES *pstGetTaskRes = &stCsConn.Body.CsCtrL.CsCmd.GetRoleTaskRes;
	//ZONEGETROLEFITTINGSRES *pstGetFittingRes = &stCsConn.Body.CsCtrL.CsCmd.GetRoleFittingsRes;
	ZONEGETROLEITEMLISTRES *pstGetItemRes = &stCsConn.Body.CsCtrL.CsCmd.GetRoleItemListRes;
	ZONECHGROLENAMERES *pstChgNameRes = &stCsConn.Body.CsCtrL.CsCmd.ChgRoleNameRes;
	ZONECHGROLEPOSRES *pstChgPosRes = &stCsConn.Body.CsCtrL.CsCmd.ChgRolePosRes;
	ZONEGETROLERIDEINFORES *pstGetRoleRideRes = &stCsConn.Body.CsCtrL.CsCmd.GetRoleRideInfoRes;
	GETROLEQIYUELISTRES *pstGetRoleQiYueListRes = &stCsConn.Body.CsCtrL.CsCmd.GetRoleQiYueListRes;
	ZONEGETROLEPETINFORES *pstGetRolePetRes = &stCsConn.Body.CsCtrL.CsCmd.GetRolePetInfoRes;
	ZONEGETROLEPACKAGERES *pstGetRolePackageRes = &stCsConn.Body.CsCtrL.CsCmd.GetRolePackageRes;
	ZONEGETPACKAGEITEMINFORES *pstGetPackageItemInfoRes = &stCsConn.Body.CsCtrL.CsCmd.GetPackageItemInfoRes;
	
	TDRDATA stNet;
	char szBuff[sizeof(SSPKG)];
	stNet.pszBuff = szBuff;
	stNet.iBuff = sizeof(szBuff);
	
	if(ss_unpack(pstEnv->pstSSHan, pszMsg, iLen, &stZoneRes, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_unpack");
		return -1;
	}

	tlog_info(pstEnv->pstLogCat, 0, 0, "get zone svr msg cmd=%d ", 
	          stZoneRes.Body.CsCtrL.Cmd);

	switch(stZoneRes.Body.CsCtrL.Cmd)
	{
	case ZONE_RES:
		tlog_info(pstEnv->pstLogCat, 0, 0, "get zone svr msg asyn cmd=%d ", 
				  stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd);
		break;
		
	case CTL_DEL_ROLE_MAIL_RES:
		return cs_z_del_role_mail_res(pstAppCtx, pstEnv, 
			                        &stZoneRes.Body.CsCtrL.CsCmd.CtlDelMailRes);
		break;

	case CTL_SEND_MAIL_RES:
		return cs_z_send_role_mail_res(pstAppCtx, pstEnv, 
			                       &stZoneRes.Body.CsCtrL.CsCmd.CtlSendMailRes);
		break;

	case CTL_BLOCK_ACCOUNT_RES:
		return cs_z_block_account_res(pstAppCtx, pstEnv,
			                   &stZoneRes.Body.CsCtrL.CsCmd.CtlBlockAccountRes);
		break;

	case CTL_SILENCE_ACCOUNT_RES:
		return cs_z_silence_account_res(pstAppCtx, pstEnv,
			                 &stZoneRes.Body.CsCtrL.CsCmd.CtlSilenceAccountRes);
		break;

	case CTL_GET_BULLETIN_LIST_RES:
		return cs_z_get_bulletin_list_res(pstAppCtx, pstEnv,
			                &stZoneRes.Body.CsCtrL.CsCmd.CtlGetBulletinListRes);
		break;

	case CTL_ADD_BULLETIN_RES:
		return cs_z_add_bulletin_res(pstAppCtx, pstEnv,
			                    &stZoneRes.Body.CsCtrL.CsCmd.CtlAddBulletinRes);
		break;

	case CTL_DEL_BULLETIN_RES:
		return cs_z_del_bulletin_res(pstAppCtx, pstEnv,
			                    &stZoneRes.Body.CsCtrL.CsCmd.CtlDelBulletinRes);
		break;

	case CTL_GET_CLAN_ALL_INFO_RES:
		return cs_z_clan_all_info_res(pstAppCtx, pstEnv,
		                     &stZoneRes.Body.CsCtrL.CsCmd.CtlGetClanAllInfoRes);
		break;

	case CTL_DEL_CLAN_MEMBER_RES:
		return cs_z_del_clan_member_res(pstAppCtx, pstEnv,
		                      &stZoneRes.Body.CsCtrL.CsCmd.CtlDelClanMemberRes);
		break;

	case CTL_DISBAND_CLAN_RES:
		return cs_z_disband_clan_res(pstAppCtx, pstEnv,
		                        &stZoneRes.Body.CsCtrL.CsCmd.CtlDisbandClanRes);
		break;

	case CTL_CLEAR_CLAN_DIPLOMATISM_RES:
		return cs_z_clear_clan_diplomatism_res(pstAppCtx, pstEnv,
		               &stZoneRes.Body.CsCtrL.CsCmd.CtlClearClanDiplomatismRes);
		break;

	case CTL_DEL_CLAN_DIPLOMATISM_RES:
		return cs_z_del_clan_diplomatism_res(pstAppCtx, pstEnv,
		                 &stZoneRes.Body.CsCtrL.CsCmd.CltDelClanDiplomatismRes);
		break;

	case CTL_EXCHANGE_CLAN_LEADER_RES:
		return cs_z_exchange_clan_leader_res(pstAppCtx, pstEnv,
		                 &stZoneRes.Body.CsCtrL.CsCmd.CltExchangeClanLeaderRes);
		break;
		
	case CTL_SET_CLAN_BASE_ATTR_RES:
		return cs_z_set_clan_base_attr_res(pstAppCtx,pstEnv, 
						&stZoneRes.Body.CsCtrL.CsCmd.CtlSetClanBaseAttrRes);
		break;

	case CTL_RELOAD_SERVER_RES:
		return cs_z_reload_server_res(pstAppCtx,pstEnv, 
						&stZoneRes.Body.CsCtrL.CsCmd.CtlReloadServerRes);
		break;
	default:
		return -1;
		break;
	}
	
	if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == BLOCKPLAYER_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEBLOCKPLAYERREQ))
		{
			return -1;
		}
		memcpy(&stBlockReq, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(ZONEBLOCKPLAYERREQ));
		stCsConn.Body.CsCtrL.Cmd = BLOCKPLAYER_RES;
		stCsConn.Body.CsCtrL.CsCmd.BlockPlayerRes.Type = stBlockReq.Type;
		strcpy(stCsConn.Body.CsCtrL.CsCmd.BlockPlayerRes.RoleName,stBlockReq.RoleName);
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == SILENCE_ROLE_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONESILENCEROLEREQ))
		{
			return -1;
		}
		memcpy(&stSilenceReq, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(ZONESILENCEROLEREQ));
		stCsConn.Body.CsCtrL.Cmd = SILENCE_ROLE_RES;
		strcpy(stCsConn.Body.CsCtrL.CsCmd.SilenceRoleRes.RoleName, stSilenceReq.RoleName);
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_DETAIL_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEGETROLEDETAILRES))
		{
			return -1;
		}

		memcpy(pstGetRoleInfoRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetRoleInfoRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_DETAIL_RES;
		
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_SKILLINFO_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEGETROLESKILLINFORES))
		{
			return -1;
		}

		memcpy(pstGetRoleSkillRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetRoleSkillRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_SKILLINFO_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_ITEMLIST_REQ)
	{
		memcpy(pstGetItemRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetItemRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_ITEMLIST_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_DIATHESIS_REQ)
	{
		memcpy(pstGetDiathsisRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetDiathsisRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_DIATHESIS_RES;
	}
	/*else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_FITTING_REQ)
	{
		memcpy(pstGetFittingRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetFittingRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_FITTING_RES;
	}*/
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_TASK_REQ)
	{
		memcpy(pstGetTaskRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetTaskRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_TASK_RES;
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == CHG_ROLE_NAME_REQ)
	{
		memcpy(pstChgNameRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstChgNameRes));
		stCsConn.Body.CsCtrL.Cmd = CHG_ROLE_NAME_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == CHG_ROLE_POS_REQ)
	{
		memcpy(pstChgPosRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstChgPosRes));
		stCsConn.Body.CsCtrL.Cmd = CHG_ROLE_POS_RES;
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_PACKAGE_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEGETROLEPACKAGERES))
		{
			return -1;
		}

		memcpy(pstGetRolePackageRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetRolePackageRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_PACKAGE_RES;
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_RIDE_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEGETROLERIDEINFORES))
		{
			return -1;
		}

		memcpy(pstGetRoleRideRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetRoleRideRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_RIDE_RES;
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_QIYUELIST_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(GETROLEQIYUELISTRES))
		{
			return -1;
		}

		memcpy(pstGetRoleQiYueListRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetRoleQiYueListRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_QIYUELIST_RES;
	}	
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_PET_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEGETROLEPETINFORES))
		{
			return -1;
		}

		memcpy(pstGetRolePetRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstGetRolePetRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_PET_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == ADD_ROLE_DESIGNATION_REQ)
	{
		ZONEADDROLEDESIGNATIONRES *pstRes = 
			&stCsConn.Body.CsCtrL.CsCmd.AddRoleDesignationRes;

		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen !=
		   sizeof(*pstRes))
		{
			return -1;
		}

		memcpy(pstRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,
		       sizeof(*pstRes));
		stCsConn.Body.CsCtrL.Cmd = ADD_ROLE_DESIGNATION_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == DEL_ROLE_DESIGNATION_REQ)
	{
		ZONEDELROLEDESIGNATIONRES *pstRes = 
			&stCsConn.Body.CsCtrL.CsCmd.DelRoleDesignationRes;

		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen !=
		   sizeof(*pstRes))
		{
			return -1;
		}

		memcpy(pstRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,
		       sizeof(*pstRes));
		stCsConn.Body.CsCtrL.Cmd = DEL_ROLE_DESIGNATION_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_DESIGNATION_LIST_REQ)
	{
		ZONEGETROLEDESIGNATIONLISTRES *pstRes = 
			&stCsConn.Body.CsCtrL.CsCmd.GetRoleDesignationListRes;

		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen !=
		   sizeof(*pstRes))
		{
			return -1;
		}

		memcpy(pstRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,
		       sizeof(*pstRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_DESIGNATION_LIST_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == SET_ROLE_RESPACT_REQ)
	{
		ZONESETROLERESPACTRES *pstRes = 
			&stCsConn.Body.CsCtrL.CsCmd.SetRoleRespactRes;

		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen !=
		   sizeof(*pstRes))
		{
			return -1;
		}

		memcpy(pstRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,
		       sizeof(*pstRes));
		stCsConn.Body.CsCtrL.Cmd = SET_ROLE_RESPACT_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == DEL_ROLE_RESPACT_REQ)
	{
		ZONEDELROLERESPACTRES *pstRes = 
			&stCsConn.Body.CsCtrL.CsCmd.DelRoleRespactRes;

		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen !=
		   sizeof(*pstRes))
		{
			return -1;
		}

		memcpy(pstRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,
		       sizeof(*pstRes));
		stCsConn.Body.CsCtrL.Cmd = DEL_ROLE_RESPACT_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_RESPACT_LIST_REQ)
	{
		ZONEGETROLERESPACTLISTRES *pstRes = 
			&stCsConn.Body.CsCtrL.CsCmd.GetRoleRespactListRes;

		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen !=
		   sizeof(*pstRes))
		{
			return -1;
		}

		memcpy(pstRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,
		       sizeof(*pstRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_RESPACT_LIST_RES;
	}

	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == SET_ROLE_LOGOUT_REQ)
	{
		ZONEROLELOGOUTREQ stRoleLogoutReq;
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEROLELOGOUTREQ))
		{
			return -1;
		}
		memcpy(&stRoleLogoutReq, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(stRoleLogoutReq));
		stCsConn.Body.CsCtrL.Cmd = SET_ROLE_LOGOUT_RES;
		strcpy(stCsConn.Body.CsCtrL.CsCmd.KickOffRoleRes.RoleName,stRoleLogoutReq.RoleName);
	}

	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == DEL_ROLE_REQ)
	{
		ZONEDELROLEREQ stDelRole;
		memcpy(&stDelRole,stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(stDelRole));
		stCsConn.Body.CsCtrL.Cmd = DEL_ROLE_RES;
		strcpy(stCsConn.Body.CsCtrL.CsCmd.DelRoleRes.RoleName,stDelRole.RoleName);
	}

	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_FRIEND_REQ)
	{
		ZONEGETFIRENDRES *pstRes = &stCsConn.Body.CsCtrL.CsCmd.FirendRes;
		
		if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(*pstRes))
		{
			return -1;
		}

		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_FRIEND_RES;
		memcpy (pstRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data, sizeof(*pstRes));
	}

	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_BLACK_REQ)
	{
		ZONEGETBLACKRES *pstRes = &stCsConn.Body.CsCtrL.CsCmd.BlackRes;
		
		if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(*pstRes))
		{
			return -1;
		}

		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_BLACK_RES;
		memcpy (pstRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data, sizeof(*pstRes));
	}

	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == SET_ROLE_PERM_REQ)
	{
		ZONESETROLEPERMREQ stReq;
		if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(stReq))
		{
			return -1;
		}

		memcpy(&stReq, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(stReq));
		stCsConn.Body.CsCtrL.Cmd = SET_ROLE_PERM_RES;
		strcpy(stCsConn.Body.CsCtrL.CsCmd.SetPermRes.RoleName ,stReq.RoleName);
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == ERR_RES)
	{
		ERRRES stErrRes;
		if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(stErrRes))
		{
			return -1;
		}

		memcpy(&stErrRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(stErrRes));
		
		err_res(pstEnv,stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.ConnIdx, 
				&stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.IPInfo,stErrRes.ErrReason, stErrRes.ErrCmd);
		
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_ROLE_PACKAGE_ITEM_REQ)
	{
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(ZONEGETPACKAGEITEMINFORES))
		{
			return -1;
		}

		memcpy(pstGetPackageItemInfoRes, stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,
					sizeof(*pstGetPackageItemInfoRes));
		stCsConn.Body.CsCtrL.Cmd = GET_ROLE_PACKAGE_ITEM_RES;
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == GET_LIFT_SKILL_REQ)
	{
		GETLIFTSKILLRES *pstRes = &stCsConn.Body.CsCtrL.CsCmd.GetLiftSkillRes;
		
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(GETLIFTSKILLRES))
		{
			return -1;
		}

		memcpy(pstRes,stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstRes));
		stCsConn.Body.CsCtrL.Cmd = GET_LIFT_SKILL_RES;
	}
	else if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == FORCE_MSG_REQ)
	{
		FORCEMSGRES *pstRes = &stCsConn.Body.CsCtrL.CsCmd.ForceMsgRes;
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(FORCEMSGREQ))
		{
			return -1;
		}
		stCsConn.Body.CsCtrL.Cmd = FORCE_MSG_RES;
		pstRes->Succ = 0;
	}
	else if (stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Cmd == CTL_SHOP_CGI_REQ)
	{
		SHOPCGIRES *pstRes = &stCsConn.Body.CsCtrL.CsCmd.ShopCgiRes;
		if(stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.DataLen != sizeof(SHOPCGIRES))
		{
			return -1;
		}

		memcpy(pstRes,stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.Data,sizeof(*pstRes));
		stCsConn.Body.CsCtrL.Cmd = CTL_SHOP_CGI_RES;
	}
	
	else
	{
		return -1;
	}

	cs_init_sshead(&stCsConn.Head, stZoneRes.Head.Cmd);
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stCsConn, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
		return -1;
	}

	if(cs_send_to_clt(pstEnv,stNet.pszBuff,stNet.iBuff,stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.ConnIdx,
						&stZoneRes.Body.CsCtrL.CsCmd.ZoneRes.AsynReq.IPInfo) < 0)
	{
		return -1;
	}

	return 0;
}

int cs_svr_proc(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv)
{
	int iBusy=0;
	int iLoopNum = 0;
	TBUSADDR iMsgDst ;
	TBUSADDR iMsgSrc ;
	int iMsgLen;
	int iRet = 0;
	char *pszMsg;
	TFRAMEHEAD stFrameHead;
	int iFrameHeadLen ;
	
	while (LOOP_NUM > iLoopNum)
	{
		iLoopNum++;
		
		iMsgDst = pstAppCtx->iId;
		iMsgSrc = TBUS_ADDR_ALL;	
		iMsgLen = sizeof(pstEnv->szRecvBuffer);
		pszMsg = &pstEnv->szRecvBuffer[0];
		if ( TBUS_SUCCESS != tbus_recv(pstAppCtx->iBus, &iMsgSrc, &iMsgDst, (void *)pszMsg, (size_t *)&iMsgLen, 0) )
		{
			break;
		}
		iBusy = 1;

		if( iMsgSrc == pstEnv->iCsConnID)
		{
			iFrameHeadLen = sizeof(stFrameHead);
			iRet = tconnapi_decode(pszMsg, iMsgLen, &stFrameHead, &iFrameHeadLen);
			if (0 != iRet)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "tconnapi_decode failed, iRet0x%x, Msglen:%d", iRet, iMsgLen);
				continue;
			}

			pszMsg += iFrameHeadLen;
			iMsgLen -= iFrameHeadLen;
			iRet = cs_clt_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen, &stFrameHead);
		}
		else if(iMsgSrc == pstEnv->iRoleDBID || iMsgSrc == pstEnv->iMiscDBID)
		{
			iRet = cs_db_msg(pstAppCtx, pstEnv, pszMsg, iMsgLen);
		}
		else if( iMsgSrc == pstEnv->iZoneSvrID)
		{
			iRet = cs_zone_svr_msg(pstAppCtx, pstEnv, pszMsg,iMsgLen);
		}
		else
		{

		}
		

	}
	
	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int cs_svr_reload(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv)
{
	CS_SVRCONF   *pstPreConf;
	
	pstPreConf = (CS_SVRCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("cs_svr reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "cs_svr reload");

	return 0;
}

int cs_svr_fini(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv)
{
	UNUSED(pstAppCtx);

	if (pstEnv->pstZoneObj)
	{
		shmdt(pstEnv->pstZoneObj);
	}

	printf("cs_svr finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "cs_svr finish");
	return 0;
}

int cs_svr_tick(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv)
{
	struct shmid_ds stShmStat;
	int iShmID;
	//int i;

	if (0 == pstEnv->bIsAttath)
	{
		iShmID		=	shmget(pstEnv->pstConf->iResShmKey, sizeof(ZoneObj), 0666);
		if( iShmID<0 )
		{
			return 0;
		}
	}
	else
	{
		iShmID = pstEnv->iShmID;
	}
	
	if ( 0 != shmctl(iShmID, IPC_STAT, &stShmStat))
	{
		return 0;
	}

	if (0 == pstEnv->bIsAttath)
	{
		//重新attach
		if (0 == cs_attach_zoneobj(pstEnv))
		{
			pstEnv->bIsAttath = 1;		
						
			tlog_info(pstEnv->pstLogCat, 0, 0, "attach shm again");
			
			return 0;
		}
	}
	else
	{
		if ((int)stShmStat.shm_nattch < 2)
		{
			if (pstEnv->pstZoneObj)
			{
				shmdt(pstEnv->pstZoneObj);
			}
			
			pstEnv->bIsAttath = 0;

			tlog_info(pstEnv->pstLogCat, 0, 0, "detach shm");
			return 0;
		}
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int cs_svr_stop(TAPPCTX *pstAppCtx, CS_SVRENV *pstEnv)
{
	int iReady=0;

	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	
	iReady = 1;
	if (iReady)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)cs_svr_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_cs_svr;
	gs_stAppCtx.stConfData.pszMetaName = "cs_svrconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)cs_svr_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)cs_svr_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)cs_svr_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)cs_svr_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)cs_svr_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}

static TASKDEF *cs_find_taskdef(CS_SVRENV *pstEnv, int iTaskID)
{
	TASKDEF stTaskDef;
	int iIndex;
	int iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (!pstObj)
	{
		return NULL;
	}

	stTaskDef.ID = iTaskID;
	iIndex = bsearch_int(&stTaskDef, pstObj->sTaskDef, pstObj->iTaskDef, TASK_DATA,
		                   &iEqu);

	return iEqu ? (TASKDEF *)pstObj->sTaskDef[iIndex] : NULL;
}
