#include <errno.h>
#include "tlog/tlog.h"
#include "tdr/tdr.h"
#include "tapp/tapp.h"
#include "proto_comm.h"
#include "op_log_meta.h"
#include "cs_net.h"
#include "pal/tnet.h"
#include "comm_misc.h"
#include "tlog_db_conf_desc.h"
#include "tlog/tlogbin.h"
#include "role_db_meta.h"
#include "entity.h"
#include <mysqld_error.h>


static char gs_szSqlTmp[1024000];
static char gs_szSql[1024000];
TDRDBMS g_stDBMS;

#define  MAJOR             1
#define  MINOR             1
#define  REV               0
#define  BUILD             20100721

#define MAX_MSG 64000*2
#define UNUSED( param ) ((void)(param))

#define CCH( arr ) (sizeof(arr)/sizeof(arr[0]))

struct taglog_dbsvrEnv
{
	TLOG_DBCONF *pstConf;
	TLOG_DBRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TDRDBHANDLE hDBHandle;
	LPTDRMETALIB pstDbLib;
	LPTDRMETA pstLogicLogMeta;
	LPTDRMETA pstOpLogMonPkgMeta;
	int iSocket;
	time_t tCheckDB;
	int iFrom;	// op log 源类型
	int iChatMonSocket;
	int iChatMonHeartBeartTime;
	int iOpLogMetaVersion;
	char ChatMonConnUrl[128];

	TAPPCTX *pstAppCtx;
	

	char szRecvBuffer[MAX_MSG];
	char szSendBuffer[MAX_MSG];
	
};

typedef struct taglog_dbsvrEnv		LOG_DBSVRENV;

static LOG_DBSVRENV gs_stEnv;
static TAPPCTX gs_stAppCtx;
static TLOG_DBRUN_STATUS gs_stStat;

extern unsigned char g_szMetalib_tlog_db[];
extern unsigned char g_szMetalib_op_log_meta[];

int log_create_index(char *pszTable, char *pszTableChg, TDRDBHANDLE hDBHandle,char *pszIndexName, char *pszIndex)
{
	TDRDATA stSql;
	char szIndexCreate[10240];
	TDRDBRESULTHANDLE hDBResult = 0;

	snprintf(szIndexCreate, sizeof(szIndexCreate), "create index %s_%s on %s (%s)", 
		pszTable, pszIndexName, pszTableChg, pszIndex);

	stSql.iBuff = strlen(szIndexCreate);
	stSql.pszBuff = &szIndexCreate[0];

	if (0 > tdr_query(&hDBResult, hDBHandle, &stSql))
	{
		return -1;
	}
	
	tdr_free_dbresult(&hDBResult);

	
	return 0;
}

int log_create_table(TDRDBHANDLE hDBHandle, LPTDRMETA pstMeta, char *pszTable, char *pszTableChg)
{
	char *pszTmp;
	char szTableCreate[102400];
	char szTableCreateTmp[102400];
	TDRDATA stSql;
	int iLen = 0;
	TDRDBRESULTHANDLE hDBResult = 0;
	
	stSql.iBuff = sizeof(szTableCreate);
	stSql.pszBuff = &szTableCreate[0];
	
	if (0 != tdr_create_table(&g_stDBMS, pstMeta, &stSql, 0))
	{
		return -1;
	}

	pszTmp = strstr(stSql.pszBuff, pszTable);
	if (NULL == pszTmp)
	{
		return -1;
	}

	iLen = pszTmp - stSql.pszBuff;
	memcpy(&szTableCreateTmp[0], stSql.pszBuff, iLen);
	strcpy(&szTableCreateTmp[0] + iLen, pszTableChg);
	iLen += strlen(pszTableChg);
	pszTmp += strlen(pszTable);

	if ((int)sizeof(szTableCreateTmp) - iLen < stSql.iBuff - (pszTmp - stSql.pszBuff))
	{
		return -1;
	}
	
	memcpy(szTableCreateTmp + iLen, pszTmp, stSql.iBuff - (pszTmp - stSql.pszBuff));

	stSql.iBuff = iLen + stSql.iBuff - (pszTmp - stSql.pszBuff);
	stSql.pszBuff = &szTableCreateTmp[0];

	if (0 > tdr_query(&hDBResult, hDBHandle, &stSql))
	{
		/*FILE *fp = fopen("./123","w");
		if (!fp)
			return -1;

		fwrite(stSql.pszBuff,stSql.iBuff, 1,fp);
		if (fp) fclose(fp);*/
		return -1;
	}
	
	tdr_free_dbresult(&hDBResult);

	if(0 == strcmp(pszTable, "LogAccLogin") )
	{
		log_create_index(pszTable, pszTableChg, hDBHandle, "Uin_Index" ,"Uin");
	}
	else if(0 == strcmp(pszTable, "LogRoleLogin") || 0==strcmp(pszTable, "LogRoleLogout"))
	{
		log_create_index(pszTable, pszTableChg, hDBHandle, "RoleID_Index" ,"RoleID");
	}
	else if (0 == strcmp(pszTable, "OpRoleLvUpSnapshot"))
	{
		log_create_index(pszTable, pszTableChg, hDBHandle, "RoleID_Index" ,"RoleInfo_RoleID");
	}
	else if (0 == strcmp(pszTable, "LogRoleTask"))
	{
		log_create_index(pszTable, pszTableChg, hDBHandle, "Task_Index" ,"LogData_TaskID");
	}
	else if (0 == strcmp(pszTable, "LogPworldTime"))
	{
		log_create_index(pszTable, pszTableChg, hDBHandle, "PworldID_Index" ,"PworldID");
	}
	else if (0 == strcmp(pszTable, "LogRoleEnterPworld"))
	{
		log_create_index(pszTable, pszTableChg, hDBHandle, "PworldID_Index" ,"PworldID");
	}
	else if (0 == strcmp(pszTable, "LogPlayerDie"))
	{
		log_create_index(pszTable, pszTableChg, hDBHandle, "KillerType_Index" ,"KillerType");
	}
	else if (0 == strcmp(pszTable, "LogMosterDie"))
	{
		log_create_index(pszTable, pszTableChg, hDBHandle, "MosterID_Index" ,"MosterID");
	}
	else if (0 == strcmp(pszTable, "LogRoleExchg"))
	{
	       log_create_index(pszTable, pszTableChg, hDBHandle, "RoleID_Index" ,"RoleID");
	}
	else if (0 == strcmp(pszTable, "LogRoleMail"))
	{
	       log_create_index(pszTable, pszTableChg, hDBHandle, "RoleID_Index" ,"RoleID");
	}
	else if (0 == strcmp(pszTable, "OpImpItemConsume") || 0 == strcmp(pszTable, "OpImpItemCreate"))
	{
	       log_create_index(pszTable, pszTableChg, hDBHandle, "ItemID_Index" ,"ItemID");
	}


	log_create_index(pszTable, pszTableChg, hDBHandle, "LogTime_Index" ,"LogTime");
	
	return 0;
}

int log_mem_to_db(TDRDBHANDLE hDBHandle, LPTDRMETA pstMeta, char* pszData, int iUnit)
{
	int i, iLen;
	char *pszTmp;
	char szTableNameTmp[TDR_NAME_LEN*3+10];
	char szTableName[TDR_NAME_LEN*3+10];
	char szSuffix[16];
	char *pszProxy="";
	//int iFlag = 0;
	
	
	time_t tTime;
	struct tm *ptmNow;
	int iRet = 0;
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;

	stDBObj.pstMeta = pstMeta;
	stDBObj.iVersion = tdr_get_meta_current_version(pstMeta);
	stDBObj.iObjSize = iUnit;
	stDBObj.pszObj = pszData;
	stSql.iBuff = sizeof(gs_szSql);
	stSql.pszBuff = &gs_szSql[0];

	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_INSERT, &stDBObj, NULL);
	if (0 != iRet)
	{
		printf("生成插入资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		printf("meta name %s\n", tdr_get_meta_name(pstMeta));
		return -1;
	}

	//修改表名
	pszTmp = stSql.pszBuff + 12;
	i = 0;
	while (*pszTmp != ' ' && i < (int)sizeof(szTableNameTmp)-1)
	{
		szTableNameTmp[i++] = *pszTmp;
		pszTmp++;
	}
	szTableNameTmp[i] = 0;
	tTime = time(NULL);
	ptmNow = localtime( &tTime );
	strftime(szSuffix, sizeof(szSuffix), "%y%m%d", ptmNow);

	switch(gs_stEnv.iFrom)
	{
	/*	
	case OP_LOG_PVE_AOTUTEAM:
		pszProxy = "_pve_autoteam";
		break;

	case OP_LOG_PVE_BATTLE:
		pszProxy = "_pve_battle";
		break;

	case OP_LOG_PVP:
		pszProxy = "_pvp";
		break;*/
		
	default:
		pszProxy = "";
		break;
	}

	snprintf(szTableName, sizeof(szTableName), "%s_%s%s", szTableNameTmp, szSuffix,pszProxy);
	szTableName[sizeof(szTableName) - 1] = 0;
	
	iLen = sprintf(gs_szSqlTmp, "INSERT INTO %s ", szTableName);
	if ((int)sizeof(gs_szSqlTmp) - iLen < stSql.iBuff - i -12)
	{
		return -1;
	}
	
	memcpy((char *)gs_szSqlTmp + iLen, pszTmp, stSql.iBuff - i -12);
		
	stSql.iBuff = iLen + stSql.iBuff - i -12;
	stSql.pszBuff = &gs_szSqlTmp[0];
		
	/*执行sql语句*/
	iRet = tdr_query(&hDBResult, hDBHandle, &stSql);
	tdr_free_dbresult(&hDBResult);

	if (0 != iRet)
	{
		int iDbmsErrNo = tdr_dbms_errno(hDBHandle);		
		
		// insert fail 
		// Table 'xxxxxxxx' doesn't exist
		if (ER_NO_SUCH_TABLE == iDbmsErrNo)
		{
			if (0 == log_create_table(hDBHandle, pstMeta, szTableNameTmp, szTableName))
			{
				iRet = tdr_query(&hDBResult, hDBHandle, &stSql);
				tdr_free_dbresult(&hDBResult);
			}
		}
		// Unknown column 'xx' in 'field list'
		else if(ER_BAD_FIELD_ERROR == iDbmsErrNo)
		{
			snprintf(szTableName, sizeof(szTableName), "%s_%s%s_%s", szTableNameTmp, szSuffix,pszProxy,"new");
			szTableName[sizeof(szTableName) - 1] = 0;
			if (log_create_table(hDBHandle, pstMeta, szTableNameTmp, szTableName) == 0)
			{
				iLen = sprintf(gs_szSqlTmp, "INSERT INTO %s ", szTableName);
				if ((int)sizeof(gs_szSqlTmp) - iLen < stSql.iBuff - i -12)
				{
					return -1;
				}
				
				memcpy((char *)gs_szSqlTmp + iLen, pszTmp, stSql.iBuff - i -12);
					
				stSql.iBuff = strlen(gs_szSqlTmp);
				stSql.pszBuff = &gs_szSqlTmp[0];
					
				/*执行sql语句*/
				iRet = tdr_query(&hDBResult, hDBHandle, &stSql);
				tdr_free_dbresult(&hDBResult);
			}
		}
	}
	
	if (0 != iRet)
	{
		/*printf("执行插入资源记录的SQL语句失败: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDBHandle));*/
		tlog_error(gs_stEnv.pstLogCat, 0, 0, "执行插入资源记录的SQL语句失败: %s dbms error:%s\n",
				tdr_error_string(iRet), tdr_dbms_error(hDBHandle));		

		gs_szSqlTmp[1024] = 0;
		//printf("sql:%s\n", gs_szSqlTmp);
		tlog_error(gs_stEnv.pstLogCat, 0, 0, "sql:%s\n", gs_szSqlTmp);
		return -1;
	}
	
	return 0;
}


int z_tlogdb_init(TAPPCTX *pstAppCtx, LOG_DBSVRENV *pstEnv)
{
	char szErrorMsg[2048]={0};
	int iWorldID = 0;
	char szWorldID[10] = {0};
	
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (TLOG_DBCONF *)pstAppCtx->stConfData.pszBuff;
	pstEnv->pstAppCtx = pstAppCtx;

	pstEnv->pstRunStat = (TLOG_DBRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}

	pstEnv->iSocket = tnet_listen(pstEnv->pstConf->szUri, 5);
	if(pstEnv->iSocket == -1)
	{
		printf("pstEnv->iSocket == %d\n",pstEnv->iSocket);
		return -1;
	}
	if (0 > tnet_set_nonblock(pstEnv->iSocket , 1))
	{
		perror("net listen set nonblock error");
		return -1;
	}

	if (0 > tdr_load_metalib(&pstEnv->pstDbLib, pstEnv->pstConf->szRoleDbMeta))
	{
		return -1;
	}
	
	pstEnv->pstLogicLogMeta = tdr_get_meta_by_name((LPTDRMETALIB) g_szMetalib_op_log_meta, "op_log_meta");
	pstEnv->pstOpLogMonPkgMeta = tdr_get_meta_by_name((LPTDRMETALIB) g_szMetalib_op_log_meta, "OpLogMonPkg");
	pstEnv->iOpLogMetaVersion = tdr_get_metalib_version((LPTDRMETALIB) g_szMetalib_op_log_meta);
	
	memset(&g_stDBMS, 0, sizeof(g_stDBMS));
	STRNCPY(g_stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(g_stDBMS.szDBMSName));
	if (pstEnv->pstConf->szLogDBName[0])
	{
		STRNCPY(g_stDBMS.szDBMSCurDatabaseName, pstEnv->pstConf->szLogDBName, sizeof(g_stDBMS.szDBMSCurDatabaseName));
	}
	else
	{
		GET_WORLD_ID(iWorldID, pstAppCtx->iId);
		sprintf(szWorldID, "world_%d", iWorldID);
		STRNCPY(g_stDBMS.szDBMSCurDatabaseName, szWorldID, sizeof(g_stDBMS.szDBMSCurDatabaseName));
	}
	STRNCPY(g_stDBMS.szDBMSConnectionInfo, pstEnv->pstConf->szLogDBConnect, sizeof(g_stDBMS.szDBMSConnectionInfo));	
	STRNCPY(g_stDBMS.szDBMSUser, pstEnv->pstConf ->szLogDBUser, sizeof(g_stDBMS.szDBMSUser));
	STRNCPY(g_stDBMS.szDBMSPassword, pstEnv->pstConf->szLogDBPasswd, sizeof(g_stDBMS.szDBMSPassword));
	g_stDBMS.iReconnectOpt = 1;	
	
	if (0 > tdr_open_dbhanlde(&pstEnv->hDBHandle, &g_stDBMS, &szErrorMsg[0]))
	{
		printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
		return -1;
	}
	pstEnv->tCheckDB = pstAppCtx->stCurr.tv_sec;
	pstEnv->iChatMonSocket = -1;
	
	tlog_info(pstEnv->pstLogCat, 0, 0, "tlog_db start");

	return 0;
}

int z_tlogdb_fini(TAPPCTX *pstAppCtx, LOG_DBSVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	
	tnet_close(pstEnv->iSocket);
	tdr_free_lib(&pstEnv->pstDbLib);
	tdr_close_dbhanlde(&pstEnv->hDBHandle);
	tlog_info(pstEnv->pstLogCat, 0, 0, "tlog_db finish");
	
	return 0;
}

int log_db_insert(TDRDBHANDLE hDBHandle, LPTDRMETA pstMeta, char* pszData, int iUnit)
{
	int iRet = 0;
	TDRDBOBJECT stDBObj;
	TDRDATA stSql;
	TDRDBRESULTHANDLE hDBResult = 0;
	char szSql[10240];

	stDBObj.pstMeta = pstMeta;
	stDBObj.iVersion = tdr_get_meta_current_version(pstMeta);
	stDBObj.iObjSize = iUnit;
	stDBObj.pszObj = pszData;
	stSql.iBuff = sizeof(szSql);
	stSql.pszBuff = &szSql[0];
	
	iRet = tdr_obj2sql(hDBHandle, &stSql, TDR_DBOP_INSERT, &stDBObj, NULL);
	if (0 != iRet)
	{
		/*printf("生成插入资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));*/

		tlog_error(gs_stEnv.pstLogCat, 0, 0, "生成插入资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return -1;
	}

	iRet = tdr_query(&hDBResult, hDBHandle, &stSql);
	tdr_free_dbresult(&hDBResult);
	
	if (0 != iRet)
	{
		return -1;
	}
	return 0;
}

static inline int rolearm_to_loggrid(LOGGRIDDATA *pstLogRoleGrid, ROLEARM *pstRoleArm)
{
	int i;
	memset(pstLogRoleGrid, 0, sizeof(*pstLogRoleGrid));
	if (0 == pstRoleArm->DefID)
	{
		return 0;
	}
	
	pstLogRoleGrid->Type = GRID_ITEM_ARM;
	pstLogRoleGrid->DefID = pstRoleArm->DefID;
	pstLogRoleGrid->InstFlag = pstRoleArm->InstFlag;
	pstLogRoleGrid->WID = pstRoleArm->WID;
	pstLogRoleGrid->Dur = pstRoleArm->Dur;
	pstLogRoleGrid->Num = pstRoleArm->Num;
	pstLogRoleGrid->ValidEnd = pstRoleArm->ValidEnd;
	pstLogRoleGrid->StarLv = pstRoleArm->StarLv;
	pstLogRoleGrid->FightLv = pstRoleArm->FightLv;
	pstLogRoleGrid->Flag = 0;

	pstLogRoleGrid->StarLv = pstRoleArm->StarLv;
	memcpy(pstLogRoleGrid->StarInfo, 
		pstRoleArm->StarInfo, 
		sizeof(pstRoleArm->StarInfo[0])*pstRoleArm->StarLv);

	pstLogRoleGrid->HoleNum = pstRoleArm->HoleNum;
	memcpy(pstLogRoleGrid->HoleGem, 
		pstRoleArm->HoleGem, 
		sizeof(pstRoleArm->HoleGem[0])*pstRoleArm->HoleNum);
	
	
	pstLogRoleGrid->BaseAttrNum = pstRoleArm->BaseAttrNum;
	for(i=0;i<pstRoleArm->BaseAttrNum;i++)
	{
		pstLogRoleGrid->BaseAttr[i].ResultID = pstRoleArm->BaseAttr[i].ResultID;
		pstLogRoleGrid->BaseAttr[i].Mul = pstRoleArm->BaseAttr[i].Mul;
	}
	
	pstLogRoleGrid->AddAttrNum = pstRoleArm->AddAttrNum;
	memcpy(pstLogRoleGrid->AddAttr, 
		pstRoleArm->AddAttr, 
		sizeof(pstRoleArm->AddAttr[0])*pstRoleArm->AddAttrNum);

	pstLogRoleGrid->WashAttrNum = pstRoleArm->WashAttrNum;
	memcpy(pstLogRoleGrid->WashAddAttr, 
		pstRoleArm->WashAddAttr, 
		sizeof(pstRoleArm->WashAddAttr[0])*pstRoleArm->WashAttrNum);
	
	pstLogRoleGrid->ExtendedFromDefID = pstRoleArm->ExtendedFromDefID;
	pstLogRoleGrid->KeYinAddAttrID = pstRoleArm->KeYinAddAttr.AddAttrID;

	return 0;
}

static inline int roleitem_to_loggrid(LOGGRIDDATA *pstLogRoleGrid, ROLEITEM *pstRoleItem, char cGridType)
{
	pstLogRoleGrid->Type = cGridType;
	pstLogRoleGrid->DefID = pstRoleItem->DefID;
	pstLogRoleGrid->Num = pstRoleItem->Num;
	pstLogRoleGrid->InstFlag = pstRoleItem->InstFlag;
	pstLogRoleGrid->WID = pstRoleItem->WID;
	pstLogRoleGrid->ValidEnd = pstRoleItem->ValidEnd;
	pstLogRoleGrid->Flag = pstRoleItem->Flag;
	pstLogRoleGrid->InstVal1 = pstRoleItem->InstVal1;
	pstLogRoleGrid->InstVal2 = pstRoleItem->InstVal2;
	pstLogRoleGrid->InstVal3 = pstRoleItem->InstVal3;

	return 0;
}

static inline int rolearm_to_exgrid(EXCHGGRIDDATA *pstLogRoleGrid, ROLEARM *pstRoleArm)
{
	//int i;
	memset(pstLogRoleGrid, 0, sizeof(*pstLogRoleGrid));
	if (0 == pstRoleArm->DefID)
	{
		return 0;
	}
	
	pstLogRoleGrid->Type = GRID_ITEM_ARM;
	pstLogRoleGrid->DefID = pstRoleArm->DefID;
	pstLogRoleGrid->InstFlag = pstRoleArm->InstFlag;
	pstLogRoleGrid->WID = pstRoleArm->WID;
	pstLogRoleGrid->Dur = pstRoleArm->Dur;
	pstLogRoleGrid->Num = pstRoleArm->Num;
	pstLogRoleGrid->ValidEnd = pstRoleArm->ValidEnd;
	pstLogRoleGrid->StarLv = pstRoleArm->StarLv;
	pstLogRoleGrid->Flag = 0;

	pstLogRoleGrid->StarLv = pstRoleArm->StarLv;
	pstLogRoleGrid->HoleNum = pstRoleArm->HoleNum;
	pstLogRoleGrid->BaseAttrNum = pstRoleArm->BaseAttrNum;
	pstLogRoleGrid->AddAttrNum = pstRoleArm->AddAttrNum;
	pstLogRoleGrid->WashAttrNum = pstRoleArm->WashAttrNum;
	pstLogRoleGrid->ExtendedFromDefID = pstRoleArm->ExtendedFromDefID;
	pstLogRoleGrid->KeYinAddAttrID = pstRoleArm->KeYinAddAttr.AddAttrID;

	return 0;
}

static inline int roleitem_to_exgrid(EXCHGGRIDDATA *pstLogRoleGrid, ROLEITEM *pstRoleItem, char cGridType)
{
	pstLogRoleGrid->Type = cGridType;
	pstLogRoleGrid->DefID = pstRoleItem->DefID;
	pstLogRoleGrid->Num = pstRoleItem->Num;
	pstLogRoleGrid->InstFlag = pstRoleItem->InstFlag;
	pstLogRoleGrid->WID = pstRoleItem->WID;
	pstLogRoleGrid->ValidEnd = pstRoleItem->ValidEnd;
	pstLogRoleGrid->Flag = pstRoleItem->Flag;
	pstLogRoleGrid->InstVal1 = pstRoleItem->InstVal1;
	pstLogRoleGrid->InstVal2 = pstRoleItem->InstVal2;
	pstLogRoleGrid->InstVal3 = pstRoleItem->InstVal3;

	return 0;
}

static inline int grid_to_loggrid( LOGGRIDDATA *pstLogGrid, ROLEGRID *pstGrid, int iGridNum)
{
	int i;
	ROLEGRID *pstRoleGrid = NULL;
		
	for (i=0; i<iGridNum; i++)
	{
		pstRoleGrid = pstGrid + i;

		if ( pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			rolearm_to_loggrid( &pstLogGrid[i], &pstRoleGrid->GridData.RoleArm);
		}
		else
		{
			roleitem_to_loggrid( &pstLogGrid[i], &pstRoleGrid->GridData.RoleItem, pstRoleGrid->Type);
		}
	}

	return 0;
}

static inline int grid_to_exgrid( EXCHGGRIDDATA *pstLogGrid, ROLEGRID *pstGrid, int iGridNum)
{
	int i;
	ROLEGRID *pstRoleGrid = NULL;
		
	for (i=0; i<iGridNum; i++)
	{
		pstRoleGrid = pstGrid + i;

		if ( pstRoleGrid->Type == GRID_ITEM_ARM)
		{
			rolearm_to_exgrid( &pstLogGrid[i], &pstRoleGrid->GridData.RoleArm);
		}
		else
		{
			roleitem_to_exgrid( &pstLogGrid[i], &pstRoleGrid->GridData.RoleItem, pstRoleGrid->Type);
		}
	}

	return 0;
}

static inline int pet_to_logpet(LOGROLEPETDATA*pstLogPets, ROLEPET *pstPets, int iPetNum)
{
	int i,j;
	ROLEPET *pstRolePet = NULL;

	for (i=0; i<iPetNum; i++)
	{
		pstRolePet = pstPets + i;

		pstLogPets[i].PetID = pstRolePet->PetID;
		pstLogPets[i].WID = pstRolePet->WID;
		pstLogPets[i].Exp = pstRolePet->Exp;
		pstLogPets[i].MaxHP = pstRolePet->MaxHP;
		pstLogPets[i].HP = pstRolePet->HP;
		pstLogPets[i].LoveVal = pstRolePet->LoveVal;
		pstLogPets[i].Level = pstRolePet->Level;
		pstLogPets[i].PetGenius = pstRolePet->PetGenius;
		pstLogPets[i].BreedNum = pstRolePet->BreedNum;
		pstLogPets[i].Hierarchy = pstRolePet->Hierarchy;
		pstLogPets[i].StrengthenLevel = pstRolePet->StrengthenLevel;
		pstLogPets[i].PhsicAtkGenius = pstRolePet->PhsicAtkGenius;
		pstLogPets[i].IceAtkGenius = pstRolePet->IceAtkGenius;
		pstLogPets[i].HPGenius = pstRolePet->HPGenius;
		pstLogPets[i].DefGenius = pstRolePet->DefGenius;
		pstLogPets[i].HitRateGenius = pstRolePet->HitRateGenius;
		pstLogPets[i].FleeGenius = pstRolePet->FleeGenius;
		pstLogPets[i].IceDefGenius = pstRolePet->IceDefGenius;
		for (j=0; j<MAX_PET_STRENGTHEN_LEVEL; j++)
		{
			pstLogPets[i].StrengthenInfo[j] = pstRolePet->StrengthenInfo[j];
		}
		pstLogPets[i].NextStrenLvl = pstRolePet->NextStrenLvl;
		pstLogPets[i].PetSkills.SkillGridNum = pstRolePet->PetSkills.SkillGridNum;
		for (j=0;j<pstRolePet->PetSkills.SkillGridNum && j<MAX_PET_SKILL_GRID; j++)
		{
			pstLogPets[i].PetSkills.SkillGrid[j] = pstRolePet->PetSkills.SkillGrid[j];
		}

		pstLogPets[i].Gender = pstRolePet->Gender;
		pstLogPets[i].NativeSkill = pstRolePet->NativeSkill;
	}

	return 0;
}

static inline int pet_to_expet(EXCHGPETDATA*pstLogPets, ROLEPET *pstPets, int iPetNum)
{
	int i,j;
	ROLEPET *pstRolePet = NULL;

	for (i=0; i<iPetNum; i++)
	{
		pstRolePet = pstPets + i;

		pstLogPets[i].PetID = pstRolePet->PetID;
		pstLogPets[i].WID = pstRolePet->WID;
		pstLogPets[i].Exp = pstRolePet->Exp;
		pstLogPets[i].MaxHP = pstRolePet->MaxHP;
		pstLogPets[i].Level = pstRolePet->Level;
		pstLogPets[i].PetGenius = pstRolePet->PetGenius;
		pstLogPets[i].BreedNum = pstRolePet->BreedNum;
		pstLogPets[i].Hierarchy = pstRolePet->Hierarchy;
		pstLogPets[i].StrengthenLevel = pstRolePet->StrengthenLevel;
		for (j=0; j<MAX_PET_STRENGTHEN_LEVEL; j++)
		{
			pstLogPets[i].StrengthenInfo[j] = pstRolePet->StrengthenInfo[j];
		}
	}

	return 0;
}
	

int role_skill_chg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLESKILLCHG *pstOpRoleSkill =  &pstOpLog->OpLog.RoleSkill;
	LOGROLESKILLCHG stRoleSkill;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleSkillChg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleSkillChg fail \n");
		return -1;
	}

	memset(&stRoleSkill, 0, sizeof(stRoleSkill));
	
	memcpy(&stRoleSkill.LogData, pstOpRoleSkill, sizeof(stRoleSkill.LogData));
	tdr_utctime_to_tdrdatetime (&stRoleSkill.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleSkill, sizeof(stRoleSkill));

	return 0;
}

int role_diathesis_chg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEDIATHESISCHG *pstOpRoleDiathesis =  &pstOpLog->OpLog.RoleDiathesis;
	LOGROLEDIATHESISCHG stRoleDiathesis;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleDiathesisChg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleDiathesis fail \n");
		return -1;
	}

	memset(&stRoleDiathesis, 0, sizeof(stRoleDiathesis));
	
	memcpy(&stRoleDiathesis.LogData, pstOpRoleDiathesis, sizeof(stRoleDiathesis.LogData));
	tdr_utctime_to_tdrdatetime (&stRoleDiathesis.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleDiathesis, sizeof(stRoleDiathesis));

	return 0;
}

int role_map_chg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEMAPCHG *pstOpRoleMapChg =  &pstOpLog->OpLog.RoleMap;
	LOGROLEMAPCHG stRoleMap;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleMapChg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleMapChg fail \n");
		return -1;
	}

	memset(&stRoleMap, 0, sizeof(stRoleMap));

	stRoleMap.Lvl = pstOpRoleMapChg->Lvl;
	stRoleMap.Uin = pstOpRoleMapChg->Uin;
	stRoleMap.RoleInfo = pstOpRoleMapChg->RoleInfo;
	stRoleMap.MapID = pstOpRoleMapChg->MapID;
	stRoleMap.Pos = pstOpRoleMapChg->Pos;
	stRoleMap.PreMapID = pstOpRoleMapChg->PreMapID;
	stRoleMap.PrePos = pstOpRoleMapChg->PrePos;
	STRNCPY( stRoleMap.LoginName, pstOpRoleMapChg->LoginName, CCH(stRoleMap.LoginName) );

	tdr_utctime_to_tdrdatetime (&stRoleMap.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleMap, sizeof(stRoleMap));

	return 0;
}

int role_enter_pworld_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEENTERPWORLD *pstOpRoleEnterPworld =  &pstOpLog->OpLog.RoleEnterPworld;
	LOGROLEENTERPWORLD stRoleEnterPworld;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleEnterPworld");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleEnterPworld fail \n");
		return -1;
	}

	memset(&stRoleEnterPworld, 0, sizeof(stRoleEnterPworld));

	stRoleEnterPworld.Uin = pstOpRoleEnterPworld->Uin;
	stRoleEnterPworld.RoleInfo = pstOpRoleEnterPworld->RoleInfo;
	stRoleEnterPworld.PworldID = pstOpRoleEnterPworld->PworldID;
	stRoleEnterPworld.RoleLevel = pstOpRoleEnterPworld->RoleLevel;
	
	STRNCPY( stRoleEnterPworld.LoginName, pstOpRoleEnterPworld->LoginName, CCH(stRoleEnterPworld.LoginName) );

	tdr_utctime_to_tdrdatetime (&stRoleEnterPworld.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleEnterPworld, sizeof(stRoleEnterPworld));

	return 0;
}

int role_qiyue_chg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEQIYUECHG *pstOpRoleQiYueChg =  &pstOpLog->OpLog.RoleQiYueChg;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpRoleQiYueChg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleQiYueChg fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpRoleQiYueChg->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpRoleQiYueChg, sizeof(OPROLEQIYUECHG));

	return 0;
}

int role_bangdingtianyuan_chg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPBUYSHOPPINGCHG *pstOpBuyShoppingChg =  &pstOpLog->OpLog.OpBuyShoppingChg;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpBuyShoppingChg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpBuyShoppingChg fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpBuyShoppingChg->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpBuyShoppingChg, sizeof(OPBUYSHOPPINGCHG));

	return 0;
}


int role_shopping_exchange_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPSHOPINGEXCHANGE *pstOpShopingExchange =  &pstOpLog->OpLog.OpShopingExchange;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpShopingExchange");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpShopingExchange fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpShopingExchange->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpShopingExchange, sizeof(OPSHOPINGEXCHANGE));

	return 0;
}


int role_get_mailgoods_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEGETMAILGOODS *pstCore = &pstOpLog->OpLog.GetMailGoods;
	LOGROLEGETMAILGOODS stDummy;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleGetMailGoods");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleRide fail \n");
		return -1;
	}

	memset(&stDummy, 0, sizeof(stDummy));

	stDummy.Uin = pstCore->Uin;
	stDummy.RoleInfo = pstCore->RoleInfo;
	stDummy.Money = pstCore->Money;
	stDummy.MailWID = pstCore->MailWID;
	stDummy.ItemNum = pstCore->ItemNum;
	grid_to_loggrid(stDummy.Items, pstCore->Items, pstCore->ItemNum);
	STRNCPY(stDummy.LoginName, pstCore->AccountLoginName, sizeof(stDummy.LoginName));
	tdr_utctime_to_tdrdatetime(&stDummy.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stDummy, sizeof(stDummy));
	return 0;
}

int role_fairy_chg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{ 
	LPTDRMETA pstMeta;
	OPFAIRY *pstRoleFairyChg =  &pstOpLog->OpLog.RoleFairyChg;
	LOGROLEFAIRYCHG stLogFairy;
		

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleFairyChg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleQiYueChg fail \n");
		return -1;
	}

	memset(&stLogFairy, 0, sizeof(stLogFairy));
	memcpy(&stLogFairy.LogData, pstRoleFairyChg, sizeof(stLogFairy.LogData));
	

	tdr_utctime_to_tdrdatetime (&stLogFairy.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLogFairy, sizeof(stLogFairy));

	return 0;
}

int role_ride_chg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{ 
	LPTDRMETA pstMeta;
	OPROLERIDE *pstRoleRideChg =  &pstOpLog->OpLog.RoleRideChg;
	LOGROLERIDECHG stLogRide;
		

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleRideChg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleRide fail \n");
		return -1;
	}

	memset(&stLogRide, 0, sizeof(stLogRide));
	memcpy(&stLogRide.LogData, pstRoleRideChg, sizeof(stLogRide.LogData));
	

	tdr_utctime_to_tdrdatetime (&stLogRide.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLogRide, sizeof(stLogRide));

	return 0;
}

int role_designation_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEDESIGNATION *pstRoleDsn = &pstOpLog->OpLog.RoleDesignationChg;
	LOGROLEDESIGNATION stLogDsn;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleDesignation");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleRide fail \n");
		return -1;
	}

	stLogDsn.LogData = *pstRoleDsn;
	tdr_utctime_to_tdrdatetime (&stLogDsn.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLogDsn, sizeof(stLogDsn));
	return 0;
}

int role_respact_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLERESPACT *pstRoleRespact = &pstOpLog->OpLog.RoleRespactChg;
	LOGROLERESPACT stLogRespact;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleRespact");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleRide fail \n");
		return -1;
	}

	stLogRespact.LogData = *pstRoleRespact;
	tdr_utctime_to_tdrdatetime (&stLogRespact.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLogRespact,
	              sizeof(stLogRespact));
	return 0;
}

int rolelogin_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLELOGIN *pstOpRoleLogin =  &pstOpLog->OpLog.RoleLogin;
	LOGROLELOGIN stRoleLogin;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleLogin");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleLogin fail \n");
		return -1;
	}

	memset(&stRoleLogin, 0, sizeof(stRoleLogin));
	
	memcpy(&stRoleLogin.LogData, pstOpRoleLogin, sizeof(stRoleLogin.LogData));
	tdr_utctime_to_tdrdatetime (&stRoleLogin.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleLogin, sizeof(stRoleLogin));

	return 0;
}

int rolelogout_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLELOGOUT *pstOpRoleLogout =  &pstOpLog->OpLog.RoleLogout;
	LOGROLELOGOUT stRoleLogout;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleLogout");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleLogout fail \n");
		return -1;
	}

	memset(&stRoleLogout, 0, sizeof(stRoleLogout));

	stRoleLogout.Uin = pstOpRoleLogout->Uin;
	stRoleLogout.RoleID = pstOpRoleLogout->RoleID;
	STRNCPY( stRoleLogout.RoleName, pstOpRoleLogout->RoleName, CCH(stRoleLogout.RoleName) );
	STRNCPY(stRoleLogout.AccountLoginName, pstOpRoleLogout->AccountLoginName, CCH(stRoleLogout.AccountLoginName));
	STRNCPY(stRoleLogout.IP, pstOpRoleLogout->IP, CCH(stRoleLogout.IP));
	stRoleLogout.Duration = pstOpRoleLogout->Duration;
	
	tdr_utctime_to_tdrdatetime (&stRoleLogout.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleLogout, sizeof(stRoleLogout));

	return 0;
}

int rolenew_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLENEW *pstOpRoleNew =  &pstOpLog->OpLog.RoleNew;
	LOGROLENEW stRoleNew;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleNew");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleNew fail \n");
		return -1;
	}

	memset(&stRoleNew, 0, sizeof(stRoleNew));
	
	memcpy(&stRoleNew.LogData, pstOpRoleNew, sizeof(stRoleNew.LogData));
	tdr_utctime_to_tdrdatetime (&stRoleNew.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleNew, sizeof(stRoleNew));

	return 0;
}

int roleshop_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLESHOP *pstOpRoleShop =  &pstOpLog->OpLog.RoleShop;
	LOGROLESHOP stRoleShop;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleShop");
	if (!pstMeta)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tdr_get_meta_by_name LogRoleShop fail");
		return -1;
	}

	memset(&stRoleShop, 0, sizeof(stRoleShop));

	stRoleShop.Uin = pstOpRoleShop->Uin;
	stRoleShop.RoleID = pstOpRoleShop->RoleID;
	stRoleShop.ShopAct = pstOpRoleShop->ShopAct;
	STRNCPY( stRoleShop.LoginName, pstOpRoleShop->AccountLoginName, CCH(stRoleShop.LoginName) );
	STRNCPY( stRoleShop.RoleName, pstOpRoleShop->RoleName, CCH(stRoleShop.RoleName) );
	stRoleShop.OldShopData = pstOpRoleShop->OldShopData;
	stRoleShop.NowShopData = pstOpRoleShop->NowShopData;
	stRoleShop.ItemDec = pstOpRoleShop->ItemDec;
	stRoleShop.ClanConDec = pstOpRoleShop->ClanConDec;
	if (SHOP_ITEM_ITEM == pstOpRoleShop->ShopItemData.ItemType)
	{
		stRoleShop.GoodsData = pstOpRoleShop->ShopItemData.ItemData.GoodsData;
	}
	tdr_utctime_to_tdrdatetime (&stRoleShop.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleShop, sizeof(stRoleShop));

	return 0;
}

int roleexchg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEEXCHG *pstOpRoleExchg =  &pstOpLog->OpLog.RoleExchg;
	LOGROLEEXCHG stRoleExchg;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleExchg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleExchg fail \n");
		return -1;
	}

	memset(&stRoleExchg, 0, sizeof(stRoleExchg));

	stRoleExchg.EventLogWID = pstOpRoleExchg->EventLogWID;
	stRoleExchg.Uin = pstOpRoleExchg->Uin;
	stRoleExchg.ExchgObjUin = pstOpRoleExchg->ExchgObjUin;
	
	stRoleExchg.RoleID = pstOpRoleExchg->RoleID;
	stRoleExchg.ExchgObjRoleID = pstOpRoleExchg->ExchgObjRoleID;
	
	STRNCPY( stRoleExchg.RoleName, pstOpRoleExchg->RoleName, CCH(stRoleExchg.RoleName) );
	STRNCPY( stRoleExchg.ExchgObjName, pstOpRoleExchg->ExchgObjName, CCH(stRoleExchg.ExchgObjName) );

	STRNCPY( stRoleExchg.LoginName, pstOpRoleExchg->LoginName, CCH(stRoleExchg.LoginName) );
	STRNCPY( stRoleExchg.ExchgObjLoginName, pstOpRoleExchg->ExchgObjLoginName, CCH(stRoleExchg.ExchgObjLoginName) );
	
	stRoleExchg.ExchgIn.Money = pstOpRoleExchg->ExchgIn.Money;
	stRoleExchg.ExchgIn.ItemNum = pstOpRoleExchg->ExchgIn.ItemNum;
	stRoleExchg.ExchgIn.PetNum = pstOpRoleExchg->ExchgIn.PetNum;
	grid_to_exgrid( stRoleExchg.ExchgIn.RoleGrid, pstOpRoleExchg->ExchgIn.RoleGrid, stRoleExchg.ExchgIn.ItemNum);
	pet_to_expet( stRoleExchg.ExchgIn.RolePet, pstOpRoleExchg->ExchgIn.RolePet, stRoleExchg.ExchgIn.PetNum);

	stRoleExchg.ExchgOut.Money = pstOpRoleExchg->ExchgOut.Money;
	stRoleExchg.ExchgOut.ItemNum = pstOpRoleExchg->ExchgOut.ItemNum;
	stRoleExchg.ExchgOut.PetNum = pstOpRoleExchg->ExchgOut.PetNum;
	grid_to_exgrid( stRoleExchg.ExchgOut.RoleGrid, pstOpRoleExchg->ExchgOut.RoleGrid, stRoleExchg.ExchgOut.ItemNum);
	pet_to_expet( stRoleExchg.ExchgOut.RolePet, pstOpRoleExchg->ExchgOut.RolePet, stRoleExchg.ExchgOut.PetNum);
		
	tdr_utctime_to_tdrdatetime (&stRoleExchg.LogTime, iSec);
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleExchg, sizeof(stRoleExchg));

	return 0;
}

static inline int stallitem_to_logitem( LOGSTALLITEM *pstLogItems, OPSTALLITEM *pstItems, int iItemNum)
{
	int i;

	for (i=0; i<iItemNum; i++)
	{
		pstLogItems[i].Price = pstItems[i].Price;
		pstLogItems[i].ItemID = pstItems[i].RoleGrid.GridData.RoleItem.DefID;
		pstLogItems[i].WID = pstItems[i].RoleGrid.GridData.RoleItem.WID;
		pstLogItems[i].Num = pstItems[i].RoleGrid.GridData.RoleItem.Num;
	}

	return 0;
}

static inline int stallpet_to_logpet( LOGSTALLPET *pstLogPets, OPSTALLPET *pstPets, int iPetNum)
{
	int i;
	for (i=0; i<iPetNum; i++)
	{
		pstLogPets[i].Price = pstPets[i].Price;
		pstLogPets[i].WID = pstPets[i].WID;
		pstLogPets[i].PetLv = pstPets[i].PetLv;
		pstLogPets[i].PetID = pstPets[i].PetID;
		//pet_to_logpet( &pstLogPets[i].RolePet, &pstPets[i].RolePet, 1);
	}

	return 0;
}

int rolestall_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLESTALL *pstOpRoleStall =  &pstOpLog->OpLog.RoleStall;
	LOGROLESTALL stRoleStall;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleStall");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleStall fail \n");
		return -1;
	}

	memset(&stRoleStall, 0, sizeof(stRoleStall));

	stRoleStall.StallFlag = pstOpRoleStall->StallFlag;
	stRoleStall.Uin = pstOpRoleStall->Uin;
	stRoleStall.StallRoleID = pstOpRoleStall->StallRoleID;
	STRNCPY( stRoleStall.StallOwner, pstOpRoleStall->StallOwner, CCH(stRoleStall.StallOwner) );
	STRNCPY( stRoleStall.LoginName, pstOpRoleStall->AccountLoginName, CCH(stRoleStall.LoginName) );
	tdr_utctime_to_tdrdatetime (&stRoleStall.LogTime, iSec);
	
	stRoleStall.ItemNum = pstOpRoleStall->ItemNum;
	stallitem_to_logitem( stRoleStall.StallItem, pstOpRoleStall->StallItem, stRoleStall.ItemNum);

	stRoleStall.PetNum= pstOpRoleStall->PetNum;
	stallpet_to_logpet( stRoleStall.StallPet, pstOpRoleStall->StallPet, stRoleStall.PetNum);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleStall, sizeof(stRoleStall));

	return 0;
}

int rolestallbuy_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLESTALLBUY *pstOpRoleStallBuy  =  &pstOpLog->OpLog.RoleStallBuy;
	LOGROLESTALLBUY stRoleStallBuy;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleStallBuy");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleStallBuy fail \n");
		return -1;
	}

	memset(&stRoleStallBuy, 0, sizeof(stRoleStallBuy));

	stRoleStallBuy.StallUin = pstOpRoleStallBuy->StallUin;
	stRoleStallBuy.Uin = pstOpRoleStallBuy->Uin;
	
	stRoleStallBuy.StallRoleID = pstOpRoleStallBuy->StallRoleID;
	stRoleStallBuy.RoleID = pstOpRoleStallBuy->RoleID;

	STRNCPY( stRoleStallBuy.StallOwner, pstOpRoleStallBuy->StallOwner, CCH(stRoleStallBuy.StallOwner) );
	STRNCPY( stRoleStallBuy.RoleName, pstOpRoleStallBuy->RoleName, CCH(stRoleStallBuy.RoleName) );

	STRNCPY( stRoleStallBuy.OwnerLoginName, pstOpRoleStallBuy->OwnerLoginName, CCH(stRoleStallBuy.OwnerLoginName) );
	STRNCPY( stRoleStallBuy.BuyLoginName, pstOpRoleStallBuy->BuyLoginName, CCH(stRoleStallBuy.BuyLoginName) );
	
	tdr_utctime_to_tdrdatetime (&stRoleStallBuy.LogTime, iSec);
	
	stRoleStallBuy.Price = pstOpRoleStallBuy->Price;
	stRoleStallBuy.Type = pstOpRoleStallBuy->Type;
	if (STALL_TYPE_ITEM == stRoleStallBuy.Type)
	{
		grid_to_loggrid( &stRoleStallBuy.RoleGrid, &pstOpRoleStallBuy->BuyData.RoleGrid, 1);
	}
	else if (STALL_TYPE_PET == stRoleStallBuy.Type)
	{
		pet_to_logpet( &stRoleStallBuy.RolePet, &pstOpRoleStallBuy->BuyData.RolePet, 1);
	}
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleStallBuy, sizeof(stRoleStallBuy));

	return 0;
}

int rolemail_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEMAIL *pstOpRoleMail =  &pstOpLog->OpLog.RoleMail;
	LOGROLEMAIL stRoleMail;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleMail");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleMail fail \n");
		return -1;
	}

	memset(&stRoleMail, 0, sizeof(stRoleMail));

	stRoleMail.Uin = pstOpRoleMail->Uin;
	stRoleMail.RoleID = pstOpRoleMail->RoleID;
	STRNCPY( stRoleMail.LoginName, pstOpRoleMail->AccountLoginName, CCH(stRoleMail.LoginName) );
	STRNCPY( stRoleMail.RoleName, pstOpRoleMail->RoleName, CCH(stRoleMail.RoleName) );
	stRoleMail.Operation = pstOpRoleMail->Operation;
	
	stRoleMail.RoleMailData.WID = pstOpRoleMail->RoleMailData.WID;
	stRoleMail.RoleMailData.Flags = pstOpRoleMail->RoleMailData.Flags;
	stRoleMail.RoleMailData.ItemNum = pstOpRoleMail->RoleMailData.ItemNum ;
	stRoleMail.RoleMailData.Money = pstOpRoleMail->RoleMailData.Money;
	
	grid_to_loggrid( stRoleMail.RoleMailData.Items, pstOpRoleMail->RoleMailData.Items, stRoleMail.RoleMailData.ItemNum);

	STRNCPY( stRoleMail.RoleMailData.Send, pstOpRoleMail->RoleMailData.Send, CCH(stRoleMail.RoleMailData.Send) );
	STRNCPY( stRoleMail.RoleMailData.Title, pstOpRoleMail->RoleMailData.Title, CCH(stRoleMail.RoleMailData.Title) );
	STRNCPY( stRoleMail.RoleMailData.To, pstOpRoleMail->RoleMailData.To, CCH(stRoleMail.RoleMailData.To) );
	
	tdr_utctime_to_tdrdatetime (&stRoleMail.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleMail, sizeof(stRoleMail));

	return 0;
}

int rolearm_machining_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEARMMACHINING *pstOpRoleMachining =  &pstOpLog->OpLog.RoleArmMachining;
	LOGROLEARMMACHINING stRoleMachining;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleArmMachining");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleArmMachining fail \n");
		return -1;
	}

	memset(&stRoleMachining, 0, sizeof(stRoleMachining));

	stRoleMachining.Uin = pstOpRoleMachining->Uin;
	stRoleMachining.RoleID = pstOpRoleMachining->RoleID;
	STRNCPY(stRoleMachining.RoleName, pstOpRoleMachining->RoleName, CCH(stRoleMachining.RoleName) );
	STRNCPY(stRoleMachining.LoginName, pstOpRoleMachining->AccountLoginName, CCH(stRoleMachining.LoginName) );
	stRoleMachining.MachiningActType = pstOpRoleMachining->MachiningActType;
	stRoleMachining.ExpendData = pstOpRoleMachining->ExpendData;
	stRoleMachining.SuccFlag = pstOpRoleMachining->SuccFlag;
	memcpy(stRoleMachining.ObtainItem, pstOpRoleMachining->ObtainItem, sizeof(stRoleMachining.ObtainItem));
	
	rolearm_to_loggrid( &stRoleMachining.ArmData.NewRoleArm, &pstOpRoleMachining->ArmData.NewRoleArm);
	rolearm_to_loggrid( &stRoleMachining.ArmData.OldRoleArm, &pstOpRoleMachining->ArmData.OldRoleArm);	
	
	tdr_utctime_to_tdrdatetime (&stRoleMachining.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleMachining, sizeof(stRoleMachining));

	return 0;
}

int mon_die_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMOSTERDIE *pstOpMonDie =  &pstOpLog->OpLog.MonsterDie;
	LOGMOSTERDIE stMonDie;

	// pstEnv->pstConf->MonSubLimit等于0表示所有怪都会写入，等于1表示微怪不会写入
	if (pstOpMonDie->SubType < pstEnv->pstConf->MonSubLimit)
	{
		return -1;
	}

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogMosterDie");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogMosterDie fail \n");
		return -1;
	}

	memset(&stMonDie, 0, sizeof(stMonDie));

	stMonDie.EventLogWID = pstOpMonDie->EventLogWID;
	stMonDie.MosterID = pstOpMonDie->MosterID;
	stMonDie.KillerType = pstOpMonDie->KillerType;
	stMonDie.SubType = pstOpMonDie->SubType;
	stMonDie.PlayerUin = pstOpMonDie->PlayerUin;
	stMonDie.PlayerLoginName[0] = 0;
	if (pstOpMonDie->PlayerLoginName[0] != 0)
	{
		STRNCPY(stMonDie.PlayerLoginName, pstOpMonDie->PlayerLoginName, CCH(stMonDie.PlayerLoginName) );
	}
	
	switch (stMonDie.KillerType)
	{
		case KILLER_PLAYER:
			stMonDie.PlayerInfo = pstOpMonDie->KillerInfo.PlayerInfo;
			break;
		case KILLER_MONSTER:
			stMonDie.MonsterInfo = pstOpMonDie->KillerInfo.MonsterInfo;
			break;
		case KILLER_PET:
			stMonDie.PetInfo = pstOpMonDie->KillerInfo.PetInfo;
			break;
		default:
			break;
	}

	stMonDie.MapID = pstOpMonDie->MapID;
	stMonDie.Pos = pstOpMonDie->Pos;
	stMonDie.PetID = pstOpMonDie->PetID;
	stMonDie.LifeTime = pstOpMonDie->LifeTime;
	
	tdr_utctime_to_tdrdatetime (&stMonDie.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stMonDie, sizeof(stMonDie));

	return 0;
}

int role_die_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPPLAYERDIE *pstOpRoleDie =  &pstOpLog->OpLog.RoleDie;
	LOGPLAYERDIE stRoleDie;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogPlayerDie");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogPlayerDie fail \n");
		return -1;
	}

	memset(&stRoleDie, 0, sizeof(stRoleDie));

	stRoleDie.Career = pstOpRoleDie->Career;
	stRoleDie.KillerCareer = pstOpRoleDie->KillerCareer;
	stRoleDie.EventLogWID = pstOpRoleDie->EventLogWID;
	stRoleDie.PlayerUin = pstOpRoleDie->PlayerUin;
	stRoleDie.PlayerInfo = pstOpRoleDie->PlayerInfo;
	stRoleDie.KillerType = pstOpRoleDie->KillerType;
	STRNCPY(stRoleDie.PlayerLoginName, pstOpRoleDie->PlayerLoginName, CCH(stRoleDie.PlayerLoginName) );

	stRoleDie.KillerUin = pstOpRoleDie->KillerUin;
	stRoleDie.KillerLoginName[0] = 0;
	if (pstOpRoleDie->KillerLoginName[0] != 0)
	{
		STRNCPY(stRoleDie.KillerLoginName, pstOpRoleDie->KillerLoginName, CCH(stRoleDie.KillerLoginName) );
	}
	
	switch (stRoleDie.KillerType)
	{
		case KILLER_PLAYER:
			stRoleDie.KillerPlayerInfo = pstOpRoleDie->KillerInfo.PlayerInfo;
			break;
		case KILLER_MONSTER:
			stRoleDie.KillerMonsterInfo = pstOpRoleDie->KillerInfo.MonsterInfo;
			break;
		case KILLER_PET:
			stRoleDie.KillerPetInfo = pstOpRoleDie->KillerInfo.PetInfo;
			break;
		default:
			break;
	}

	stRoleDie.MapID = pstOpRoleDie->MapID;
	stRoleDie.Pos = pstOpRoleDie->Pos;
	stRoleDie.PlayerLevel = pstOpRoleDie->PlayerLevel;
	stRoleDie.Money = pstOpRoleDie->Money;
	stRoleDie.Exp = pstOpRoleDie->Exp;
	stRoleDie.PetID = pstOpRoleDie->PetID;
	stRoleDie.KillerLv = pstOpRoleDie->KillerLv;
		
	tdr_utctime_to_tdrdatetime (&stRoleDie.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleDie, sizeof(stRoleDie));

	return 0;
}

int pet_attr_chg_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPPETATTRCHG *pstOpPetChg =  &pstOpLog->OpLog.PetAttrChg;
	LOGPETATTRCHG stPetChg;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogPetAttrChg");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogPetAttrChg fail \n");
		return -1;
	}

	memset(&stPetChg, 0, sizeof(stPetChg));

	stPetChg.OwnerUin = pstOpPetChg->OwnerUin;
	stPetChg.OwnerInfo = pstOpPetChg->OwnerInfo;
	stPetChg.PetActType = pstOpPetChg->PetActType;
	stPetChg.ActObjID = pstOpPetChg->ActObjID;
	stPetChg.UseMoney = pstOpPetChg->UseMoney;
	stPetChg.ItemDec = pstOpPetChg->UseItem;
	
	pet_to_logpet(&stPetChg.PetInfo, &pstOpPetChg->Pet, 1);
	STRNCPY(stPetChg.OwnerLoginName, pstOpPetChg->OwnerLoginName, CCH(stPetChg.OwnerLoginName) );
	
	tdr_utctime_to_tdrdatetime (&stPetChg.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stPetChg, sizeof(stPetChg));

	return 0;
}

int pick_item_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEPICKITEM *pstOpPickItem =  &pstOpLog->OpLog.RolePickItem;
	LOGROLEPICKITEM stPickItem;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRolePickItem");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRolePickItem fail \n");
		return -1;
	}

	memset(&stPickItem, 0, sizeof(stPickItem));

	stPickItem.Uin = pstOpPickItem->Uin;
	stPickItem.RoleInfo = pstOpPickItem->RoleInfo;
	STRNCPY(stPickItem.LoginName, pstOpPickItem->LoginName, CCH(stPickItem.LoginName) );
	stPickItem.How = pstOpPickItem->How;
	stPickItem.CareerLimit = pstOpPickItem->CareerLimit;
	stPickItem.X = pstOpPickItem->X;
	stPickItem.Y= pstOpPickItem->Y;
	stPickItem.Map= pstOpPickItem->Map;
	stPickItem.RoleCareer = pstOpPickItem->RoleCareer;

	if (pstOpPickItem->Items.Type == GRID_ITEM_ARM)
	{
		rolearm_to_loggrid(&stPickItem.Items, &pstOpPickItem->Items.GridData.RoleArm);
	}
	else
	{
		roleitem_to_loggrid(&stPickItem.Items, &pstOpPickItem->Items.GridData.RoleItem, pstOpPickItem->Items.Type);
	}
		
	grid_to_loggrid(&stPickItem.Items, &pstOpPickItem->Items, 1);
	tdr_utctime_to_tdrdatetime (&stPickItem.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stPickItem, sizeof(stPickItem));

	return 0;
}

int drop_item_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPBOOTYDROP *pstOpBootyDrop =  &pstOpLog->OpLog.RoleBootyDrop;
	LOGROLEDROPITEM stDropItem;

	if (pstOpBootyDrop->ItemImportant < pstEnv->pstConf->DropItemLimit)
	{
		return -1;
	}

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleDropItem");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleDropItem fail \n");
		return -1;
	}

	memset(&stDropItem, 0, sizeof(stDropItem));

	stDropItem.EventLogWID = pstOpBootyDrop->EventLogWID;
	stDropItem.DropMap = pstOpBootyDrop->MapID;
	stDropItem.DropPos = pstOpBootyDrop->Pos;
	stDropItem.DropType = pstOpBootyDrop->DropType;
	stDropItem.FromID = pstOpBootyDrop->FromID;
	stDropItem.FromType = pstOpBootyDrop->FromType;
	STRNCPY(stDropItem.FromName, pstOpBootyDrop->FromName, CCH(stDropItem.FromName) );
	
	if ( pstOpBootyDrop->RoleGrid.Type == GRID_ITEM_ARM)
	{
		rolearm_to_loggrid( &stDropItem.LogDropItem, &pstOpBootyDrop->RoleGrid.GridData.RoleArm);
	}
	else
	{
		roleitem_to_loggrid( &stDropItem.LogDropItem, &pstOpBootyDrop->RoleGrid.GridData.RoleItem, 
							pstOpBootyDrop->RoleGrid.Type);
	}
	
	tdr_utctime_to_tdrdatetime (&stDropItem.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stDropItem, sizeof(stDropItem));

	return 0;
}


int role_task_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLETASK *pstOpRoleTask =  &pstOpLog->OpLog.RoleTask;
	LOGROLETASK stRoleTask;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleTask");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleTask fail \n");
		return -1;
	}

	memset(&stRoleTask, 0, sizeof(stRoleTask));
	
	memcpy(&stRoleTask.LogData, pstOpRoleTask, sizeof(stRoleTask.LogData));
	tdr_utctime_to_tdrdatetime (&stRoleTask.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleTask, sizeof(stRoleTask));

	return 0;
}

int clan_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLECLAN *pstOpRoleClan =  &pstOpLog->OpLog.RoleClan;
	LOGROLECLAN stRoleClan;

	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleClan");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleClan fail \n");
		return -1;
	}

	memset(&stRoleClan, 0, sizeof(stRoleClan));
	
	memcpy(&stRoleClan.LogData, pstOpRoleClan, sizeof(stRoleClan.LogData));
	tdr_utctime_to_tdrdatetime (&stRoleClan.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleClan, sizeof(stRoleClan));

	return 0;
}

int role_del_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEDEL *pstOpRoleDel =  &pstOpLog->OpLog.RoleDel;
	LOGROLEDEL stRoleDel;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleDel");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleDel fail \n");
		return -1;
	}

	memset(&stRoleDel, 0, sizeof(stRoleDel));
	
	memcpy(&stRoleDel.LogData, pstOpRoleDel, sizeof(stRoleDel.LogData));
	tdr_utctime_to_tdrdatetime (&stRoleDel.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleDel, sizeof(stRoleDel));

	return 0;
}

int send_to_chat_mon_service(LOG_DBSVRENV *pstEnv,int iCmd,OPLOGMONDATA *pstBody)
{
	OPLOGMONPKG stPkg;
	TDRDATA stHost;
	TDRDATA stNet;
	int iRet = 0;
	int iNow = gs_stAppCtx.stCurr.tv_sec;

	if (pstEnv->ChatMonConnUrl[0] == 0)
	{
		return 0;
	}

	if (iNow - pstEnv->iChatMonHeartBeartTime >= 30)
	{
		return 0;
	}

	stPkg.Body = *pstBody;

	stPkg.Head.Cmd = iCmd;
	stPkg.Head.Magic = PROTOCOL_MAGIC; 
	stPkg.Head.Ver = pstEnv->iOpLogMetaVersion;
	stPkg.Head.HeadLen = sizeof(stPkg.Head);

	stHost.iBuff = sizeof(stPkg);
	stHost.pszBuff = (char *)&stPkg;

	stNet.pszBuff = pstEnv->szSendBuffer;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);

	iRet = tdr_hton(pstEnv->pstOpLogMonPkgMeta, &stNet, &stHost,stPkg.Head.Ver);

	if (iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tdr_hton fail");
		return -1;
	}
	
	iRet = tnet_send(pstEnv->iChatMonSocket,stNet.pszBuff, stNet.iBuff, 0);
	
	if (iRet < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "tnet_send fail:%s",strerror(errno));

		tnet_close(pstEnv->iChatMonSocket);
		pstEnv->iChatMonSocket = -1;
		
		pstEnv->iChatMonSocket = tnet_connect(pstEnv->ChatMonConnUrl, 1000);
		if (pstEnv->iChatMonSocket < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tnet_connect fail, url=%s", 
						pstEnv->ChatMonConnUrl);
			return -1;
		}
		else
		{		
			tnet_set_nonblock(pstEnv->iChatMonSocket, 1);
			tnet_set_sendbuff(pstEnv->iChatMonSocket, 20480);

			iRet = tnet_send(pstEnv->iChatMonSocket,stNet.pszBuff, stNet.iBuff, 0);
	
			if (iRet < 0)
			{
				tnet_close(pstEnv->iChatMonSocket);
				pstEnv->iChatMonSocket = -1;
				tlog_error(pstEnv->pstLogCat, 0, 0, "tnet_send fail");
				return -1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}

int role_chat_to_chat_mon_service(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog)
{
	OPLOGMONDATA stBody;

	stBody.OpLogData = *pstOpLog;
	
	send_to_chat_mon_service(pstEnv,OP_LOG_MON_DATA,&stBody);
	
	return 0;
}

int role_chat_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLECHAT *pstOpRoleChat =  &pstOpLog->OpLog.RoleChat;
	LOGROLECHAT stRoleChat;	
	int iNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	  
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleChat");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleChat fail \n");
		return -1;
	}

	memset(&stRoleChat, 0, sizeof(stRoleChat));

	stRoleChat.Uin = pstOpRoleChat->Uin;
	STRNCPY( stRoleChat.LoginName, pstOpRoleChat->LoginName, CCH(stRoleChat.LoginName) );
	stRoleChat.RoleInfo = pstOpRoleChat->RoleInfo;
	stRoleChat.ChatType = pstOpRoleChat->ChatType;

	STRNCPY( stRoleChat.ToName, pstOpRoleChat->ToName, CCH(stRoleChat.ToName) );
	STRNCPY( stRoleChat.Content, pstOpRoleChat->Content, CCH(stRoleChat.Content) );
	
	tdr_utctime_to_tdrdatetime (&stRoleChat.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRoleChat, sizeof(stRoleChat));

	if (pstEnv->ChatMonConnUrl[0]  && 
		iNow - pstEnv->iChatMonHeartBeartTime < 30)
	{
		role_chat_to_chat_mon_service(pstEnv,pstOpLog);
	}	
	
	return 0;
}

int role_pet_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEPET *pstOpRolePet =  &pstOpLog->OpLog.RolePet;
	LOGROLEPET stRolePet;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRolePet");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRolePet fail \n");
		return -1;
	}

	memset(&stRolePet, 0, sizeof(stRolePet));

	stRolePet.EventLogWID = pstOpRolePet->EventLogWID;
	stRolePet.Uin = pstOpRolePet->Uin;
	stRolePet.RoleInfo = pstOpRolePet->RoleInfo;
	stRolePet.Event = pstOpRolePet->Event;
	stRolePet.GetItem = pstOpRolePet->GetItem;
	stRolePet.ItemDec = pstOpRolePet->ItemDec;
	stRolePet.Money = pstOpRolePet->Money;
	stRolePet.PetStrenInfo = pstOpRolePet->PetStrenInfo;
	
	STRNCPY( stRolePet.LoginName, pstOpRolePet->LoginName, CCH(stRolePet.LoginName) );

	if (OP_PET_LVUP != stRolePet.Event)
	{
		pet_to_logpet( &stRolePet.PetInfo, &pstOpRolePet->PetInfo.BronPet, 1);
	}
	else
	{
		pet_to_logpet( &stRolePet.PetInfo, &pstOpRolePet->PetInfo.LvUpPet.CurrentPet, 1);
	}
	
	
	tdr_utctime_to_tdrdatetime (&stRolePet.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stRolePet, sizeof(stRolePet));

	return 0;
}


int role_gmcmd_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPGMCMD *pstGmCmd = &pstOpLog->OpLog.GMCmd;
	LOGROLEGMCMD stGMCmd;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleGmCmd");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleGmCmd fail \n");
		return -1;
	}

	memcpy(&stGMCmd.LogData, pstGmCmd, sizeof(OPGMCMD));
	tdr_utctime_to_tdrdatetime (&stGMCmd.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stGMCmd, sizeof(stGMCmd));

	return 0;
}


int role_snapshot_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLESNAPSHOT *pstSnapshot = &pstOpLog->OpLog.OpRoleSnapshot;
	LOGROLESNAPSHOT stSnapshot;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleSnapshot");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleSnapshot fail \n");
		return -1;
	}

	memcpy(&stSnapshot.LogData, pstSnapshot, sizeof(OPROLESNAPSHOT));
	tdr_utctime_to_tdrdatetime (&stSnapshot.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stSnapshot, sizeof(stSnapshot));

	return 0;
}

int role_trade_info_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPTRADEINFO *pstOpTradeInfo =  &pstOpLog->OpLog.OpTradeInfo;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpTradeInfo");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpTradeInfo fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpTradeInfo->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpTradeInfo, sizeof(OPTRADEINFO));

	return 0;
}

int role_lvup_snapshot_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog,
                             int iSec)
{
	OPROLELVUPSNAPSHOT *pstOpLvUp = &pstOpLog->OpLog.RoleLvUpSnapshot;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "OpRoleLvUpSnapshot");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleLvUpSnapshot fail\n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime(&pstOpLvUp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpLvUp,
	              sizeof(*pstOpLvUp));
	return 0;
}

int role_use_xp_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{	
	LOGUSEXP stLogUseXp;
	OPUSEXPSKILL *pstOpUseXpSKill= &pstOpLog->OpLog.OpUseXpSKill;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogUseXp");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogUseXp fail\n");
		return -1;
	}

	memcpy(&stLogUseXp.LogData, pstOpUseXpSKill, sizeof(stLogUseXp.LogData));
	tdr_utctime_to_tdrdatetime (&stLogUseXp.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLogUseXp, sizeof(stLogUseXp));
	return 0;
}

int rolt_gift_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGGIFT stLogGift;
	OPGIFT *pstGift  = &pstOpLog->OpLog.OpGift;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogGift");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogGift fail\n");
		return -1;
	}

	memcpy(&stLogGift.LogData, pstGift, sizeof(stLogGift.LogData));
	tdr_utctime_to_tdrdatetime (&stLogGift.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLogGift, sizeof(stLogGift));
	return 0;
}

int rolt_media_gift_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGMEDIAGIFT stLogGift;
	OPMEDIAGIFT *pstGift  = &pstOpLog->OpLog.OpMediaGift;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogMediaGift");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogMediaGift fail\n");
		return -1;
	}

	memcpy(&stLogGift.LogData, pstGift, sizeof(stLogGift.LogData));
	tdr_utctime_to_tdrdatetime (&stLogGift.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLogGift, sizeof(stLogGift));
	return 0;
}

int rolt_ltr_gift_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGLTRGIFT stLogGift;
	OPLTRGIFT *pstGift  = &pstOpLog->OpLog.OpLtrGift;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogLtrGift");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogLtrGift fail\n");
		return -1;
	}

	memcpy(&stLogGift.LogData, pstGift, sizeof(stLogGift.LogData));
	tdr_utctime_to_tdrdatetime (&stLogGift.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLogGift, sizeof(stLogGift));
	return 0;
}

int clan_active_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPCLANACTIVE *pstOpClanActive =  &pstOpLog->OpLog.OpClanActive;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpClanActive");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpClanActive fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpClanActive->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpClanActive, sizeof(*pstOpClanActive));
	return 0;
}

int daily_goal_award_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPDAILYGOALAWARD *pstOpDailyGoalAward =  &pstOpLog->OpLog.OpDailyGoalAward;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpDailyGoalAward");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpDailyGoalAward fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpDailyGoalAward->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpDailyGoalAward, sizeof(*pstOpDailyGoalAward));
	return 0;
}

int bindgold_consume(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPBINDGOLDCONSUME *pstOp =  &pstOpLog->OpLog.OpBindGoldConsume;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpBindGoldConsume");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpBindGoldConsume fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int bindgold_create(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPBINDGOLDCREATE *pstOp =  &pstOpLog->OpLog.OpBindGoldCreate;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpBindGoldCreate");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpBindGoldCreate fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int money_create(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMONEYCREATE *pstOp =  &pstOpLog->OpLog.OpMoneyCreate;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpMoneyCreate");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpMoneyCreate fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int money_consume(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMONEYCONSUME *pstOp =  &pstOpLog->OpLog.OpMoneyConsume;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpMoneyConsume");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpMoneyConsume fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int impitem_create(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPIMPITEMCREATE *pstOp =  &pstOpLog->OpLog.OpImpItemCreate;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpImpItemCreate");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpImpItemCreate fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int impitem_consume(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPIMPITEMCONSUME *pstOp =  &pstOpLog->OpLog.OpImpItemConsume;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpImpItemConsume");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpImpItemConsume fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int clanprop_create(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPCLANPROPERTYCREATE *pstOp =  &pstOpLog->OpLog.OpClanPropertyCreate;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpClanPropertyCreate");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpClanPropertyCreate fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int clanprop_consume(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPCLANPROPERTYCONSUME *pstOp =  &pstOpLog->OpLog.OpClanPropertyConsume;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpClanPropertyConsume");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpClanPropertyConsume fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int contri_create(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPCONTRICREATE *pstOp =  &pstOpLog->OpLog.OpContriCreate;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpContriCreate");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpContriCreate fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int contri_consume(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPCONTRICONSUME *pstOp =  &pstOpLog->OpLog.OpContriConsume;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpContriConsume");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpContriConsume fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}


int role_secondary_skill_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGROLESECONDARYSKILL stLog;
	OPROLESECONDARYSKILL *pstSecSkill  = &pstOpLog->OpLog.RoleSecondarySkill;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogRoleSecondarySkill");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleSecondarySkill fail\n");
		return -1;
	}

	memcpy(&stLog.LogData, pstSecSkill, sizeof(stLog.LogData));
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}


int role_acc_login_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGACCLOGIN stLog;
	OPACCLOGIN *pstAccLogin = &pstOpLog->OpLog.OpAccLogin;

	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogAccLogin");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogAccLogin fail\n");
		return -1;
	}

	memcpy(&stLog.LogData, pstAccLogin, sizeof(stLog.LogData));
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_acc_logout_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGACCLOGOUT stLog;
	OPACCLOGOUT*pstAccLogout = &pstOpLog->OpLog.OpAccLogout;

	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogAccLogout");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogAccLogout fail\n");
		return -1;
	}

	memcpy(&stLog.LogData, pstAccLogout, sizeof(stLog.LogData));
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_secskill_recipe_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGROLESECSKILLRECIPE stLog;
	OPROLESECSKILLRECIPE *pstRecipe = &pstOpLog->OpLog.RoleSecSkillRecipe;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogRoleSecSkillRecipe");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogGift fail\n");
		return -1;
	}

	memcpy(&stLog.LogData, pstRecipe, sizeof(stLog.LogData));
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int pworld_time_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGPWORLDTIME stLog;
	OPPWORLDTIME *pstOp  = &pstOpLog->OpLog.OpPworldTime;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogPworldTime");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogPworldTime fail\n");
		return -1;
	}

	memcpy(&stLog.LogData, pstOp, sizeof(stLog.LogData));
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int mud_acc_login_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMUDACCLOGIN *pstOpMudAccLogin =  &pstOpLog->OpLog.OpMudAccLogin;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpMudAccLogin");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpMudAccLogin fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpMudAccLogin->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpMudAccLogin, sizeof(*pstOpMudAccLogin));
	return 0;
}

int mud_acc_logout_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMUDACCLOGOUT *pstOpMudAccLogout =  &pstOpLog->OpLog.OpMudAccLogout;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpMudAccLogout");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpMudAccLogout fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpMudAccLogout->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpMudAccLogout, sizeof(*pstOpMudAccLogout));
	return 0;
}

int mud_role_login_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMUDROLELOGIN *pstOpMudRoleLogin =  &pstOpLog->OpLog.OpMudRoleLogin;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpMudRoleLogin");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpMudRoleLogin fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpMudRoleLogin->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpMudRoleLogin, sizeof(*pstOpMudRoleLogin));
	return 0;
}

int mud_role_logout_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMUDROLELOGOUT *pstOpMudRoleLogout =  &pstOpLog->OpLog.OpMudRoleLogout;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpMudRoleLogout");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpMudRoleLogout fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpMudRoleLogout->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpMudRoleLogout, sizeof(*pstOpMudRoleLogout));
	return 0;
}

int mud_take_exp_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMUDTAKEEXP *pstOpMudTakeExp =  &pstOpLog->OpLog.OpMudTakeExp;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpMudTakeExp");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpMudTakeExp fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpMudTakeExp->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpMudTakeExp, sizeof(*pstOpMudTakeExp));
	return 0;
}

int role_item_dec_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEITEMDEC *pstOpRoleItemDec =  &pstOpLog->OpLog.OpRoleItemDec;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpRoleItemDec");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleItemDec fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpRoleItemDec->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpRoleItemDec, sizeof(*pstOpRoleItemDec));
	return 0;
}

int role_item_drop_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEITEMDROP *pstOpRoleItemDrop =  &pstOpLog->OpLog.OpRoleItemDrop;

	LOGROLEITEMDROP stLog;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleItemDrop");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpRoleItemDrop fail \n");
		return -1;
	}
	
	memset(&stLog,0,sizeof(stLog));

	stLog.Uin = pstOpRoleItemDrop->Uin;
	stLog.RoleInfo = pstOpRoleItemDrop->RoleInfo;
	strncpy(stLog.Pass9Account,pstOpRoleItemDrop->Pass9Account,sizeof(stLog.Pass9Account));
	grid_to_loggrid(&stLog.GridData,&pstOpRoleItemDrop->GridData,1);

	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_die_drop_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEDIEDROP *pstOpRoleDieDrop = &pstOpLog->OpLog.OpRoleDieDrop;
	//int i;

	LOGROLEDIEDROP stLog;
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRoleDieDrop");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleDieDrop fail \n");
		return -1;
	}
	
	memset(&stLog,0,sizeof(stLog));

	stLog.EventLogWID = pstOpRoleDieDrop->EventLogWID;
	stLog.Uin = pstOpRoleDieDrop->Uin;
	stLog.RoleInfo = pstOpRoleDieDrop->RoleInfo;
	STRNCPY(stLog.LoginName, pstOpRoleDieDrop->LoginName, sizeof(stLog.LoginName));
	stLog.DropNum = pstOpRoleDieDrop->DropNum;
	//for (i=0; i<pstOpRoleDieDrop->DropNum; i++)
	{
		grid_to_loggrid(stLog.DropItem, pstOpRoleDieDrop->DropItem, pstOpRoleDieDrop->DropNum);
	}
	
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_task_item_award_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPTASKITEMAWARD *pstOp = &pstOpLog->OpLog.OpTaskItemAward;
	LOGTASKAWARDITEM stLog;
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogTaskAwardItem");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogTaskAwardItem  fail \n");
		return -1;
	}

	memset(&stLog,0,sizeof(stLog));

	stLog.EventLogWID = pstOp->EventLogWID;
	stLog.TaskID = pstOp->TaskID;
	stLog.RoleInfo = pstOp->RoleInfo;
	stLog.AwardNum = pstOp->AwardNum;
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);
	grid_to_loggrid(stLog.AwardItem, pstOp->AwardItem, pstOp->AwardNum);
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_pworld_award_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEPWORLDAWARD *pstOpRolePworldAward =  &pstOpLog->OpLog.OpRolePworldAward;
	LOGROLEPWORLDAWARD stLog;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRolePworldAward");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRolePworldAward fail \n");
		return -1;
	}
	
	memset(&stLog,0,sizeof(stLog));

	stLog.EventLogWID = pstOpRolePworldAward->EventLogWID;
	stLog.Uin = pstOpRolePworldAward->Uin;
	stLog.RoleInfo = pstOpRolePworldAward->RoleInfo;
	strncpy(stLog.Pass9Account, pstOpRolePworldAward->LoginName,sizeof(stLog.Pass9Account));

	stLog.PworldID = pstOpRolePworldAward->PworldID;
	stLog.Exp = pstOpRolePworldAward->Exp;
	stLog.Money = pstOpRolePworldAward->Money;

	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_pworld_card_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPROLEPWORLDCARD *pstOpRolePworldAward =  &pstOpLog->OpLog.OpRolePworldCard;
	LOGROLEPWORLDCARD stLog;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogRolePworldCard");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRolePworldCard fail \n");
		return -1;
	}
	
	memset(&stLog,0,sizeof(stLog));

	stLog.EventLogWID = pstOpRolePworldAward->EventLogWID;
	stLog.Uin = pstOpRolePworldAward->Uin;
	stLog.RoleInfo = pstOpRolePworldAward->RoleInfo;
	strncpy(stLog.Pass9Account, pstOpRolePworldAward->LoginName,sizeof(stLog.Pass9Account));

	stLog.PworldID = pstOpRolePworldAward->PworldID;
	stLog.Exp = pstOpRolePworldAward->Exp;
	stLog.Money = pstOpRolePworldAward->Money;
	grid_to_loggrid(&stLog.Item, &pstOpRolePworldAward->Item, 1);

	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_battle_award_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGROLEBATTLEAWARD stLog;
	OPROLEBATTLEAWARD *pstRoleBattleAward  = &pstOpLog->OpLog.OpRoleBattleAward;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogRoleBattleAward");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleBattleAward fail\n");
		return -1;
	}

	memcpy(&stLog.LogData, pstRoleBattleAward, sizeof(stLog.LogData));
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_recipe_make_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGROLERECIPEMAKE stLog;
	OPROLERECIPEMAKE *pstRecipeMake  = &pstOpLog->OpLog.OpRoleRecipeMake;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogRoleRecipeMake");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleRecipeMake fail\n");
		return -1;
	}

	memset(&stLog,0,sizeof(stLog));
	stLog.Uin = pstRecipeMake->Uin;
	stLog.RoleInfo = pstRecipeMake->RoleInfo;
	strncpy(stLog.Pass9Account, pstRecipeMake->Pass9Account, sizeof(stLog.Pass9Account));

	stLog.SkillType = pstRecipeMake->SkillType;
	stLog.SkillLevel = pstRecipeMake->SkillLevel;
	stLog.RecipeID = pstRecipeMake->RecipeID;
	memcpy(&stLog.DecItem[0], &pstRecipeMake->MakeInfo.DecItem[0], sizeof(stLog.DecItem[0])*MAX_RECIPE_STUFF);
	stLog.GetItem = pstRecipeMake->MakeInfo.GetItem;
	grid_to_loggrid(&stLog.ResolveItem, &pstRecipeMake->MakeInfo.ResolveItem, 1);

	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_pet_catch_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGROLEPETCATCH stLog;
	OPROLEPETCATCH *pstPetCatch  = &pstOpLog->OpLog.OpRolePetCatch;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogRolePetCatch");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRolePetCatch fail\n");
		return -1;
	}

	memcpy(&stLog.LogData, pstPetCatch, sizeof(stLog.LogData));
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int role_fengyin_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGROLEFENGYIN stLog;
	OPROLEARMFENGYIN *pstOp = &pstOpLog->OpLog.OpRoleArmFengyin;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogRoleFengyin");
	ROLEGRID stGrid;

	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogRoleFengyin fail\n");
		return -1;
	}

	memset(&stLog, 0, sizeof(stLog));
	stLog.UnFengyinUse = pstOp->Use;
	stLog.Type= pstOp->Type;
	stLog.RoleInfo = pstOp->RoleInfo;
	stLog.FengyinInfo = pstOp->FengyinInfo;
	stGrid.Type = GRID_ITEM_ARM;
	stGrid.GridData.RoleArm = pstOp->RoleArm;
	grid_to_loggrid(&stLog.LogArm, &stGrid, 1);
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int map_trans_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPMAPTRANS *pstOpMapTrans =  &pstOpLog->OpLog.OpMapTrans;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpMapTrans");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpMapTrans fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpMapTrans->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpMapTrans, sizeof(*pstOpMapTrans));
	return 0;
}


int strong_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPSTRONGINFO *pstStrong = &pstOpLog->OpLog.OpStrongInfo;
	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpStrongInfo");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpStrongInfo fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstStrong->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstStrong, sizeof(*pstStrong));
	return 0;
}

int open_box_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPOPENBOX *pstOpOpenBox = &pstOpLog->OpLog.OpOpenBox;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpOpenBox");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpOpenBox fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpOpenBox->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpOpenBox, sizeof(*pstOpOpenBox));
	return 0;
}

int cityfini_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPCITYFINI *pstCity = &pstOpLog->OpLog.OpCityFini;
	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpCityFini");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpCityFini fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstCity->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstCity, sizeof(*pstCity));
	return 0;
}

int strongfini_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPSTRONGFINI *pstStrong = &pstOpLog->OpLog.OpStrongFini;
	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpStrongFini");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpStrongFini fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstStrong->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstStrong, sizeof(*pstStrong));
	return 0;
}

int spanstrong_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPSPANSTRONG *pstStrong = &pstOpLog->OpLog.OpSpanStrong;
	
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpSpanStrong");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpSpanStrong fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstStrong->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstStrong, sizeof(*pstStrong));
	return 0;
}

int home_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPHOME *pstOpHome = &pstOpLog->OpLog.OpHome;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpHome");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpHome fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpHome->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpHome, sizeof(*pstOpHome));
	return 0;
}

int lucky_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPLUCKY *pstOpLucky =  &pstOpLog->OpLog.OpLucky;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpLucky");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpLucky fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOpLucky->LogTime, iSec);
	
	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOpLucky, sizeof(*pstOpLucky));
	return 0;
}

int genius_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPGENUIS *pstOp =  &pstOpLog->OpLog.OpGenuis;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpGenuis");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpGenuis fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int spar_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPSPAR *pstOp =  &pstOpLog->OpLog.OpSpar;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpSpar");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpSpar fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int boon_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPBOON*pstOp =  &pstOpLog->OpLog.OpBoon;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpBoon");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpBoon fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int boon_cond_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPBOONCOND*pstOp =  &pstOpLog->OpLog.OpBoonCond;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpBoonCond");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpBoonCond fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int battle_money_use_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPBATTLEMONEYUSE *pstOp = &pstOpLog->OpLog.OpBattleMoneyUse;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpBattleMoneyUse");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpBattleMoneyUse fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}

int store_buy_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPSTORE*pstOp =  &pstOpLog->OpLog.OpStore;
	LOGSTORE stLog;

	memset(&stLog, 0, sizeof(stLog));
	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "LogStore");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogStore fail \n");
		return -1;
	}

	stLog.Type = pstOp->Type;
	stLog.StoreOwnerRoleID = pstOp->StoreOwnerRoleID;
	stLog.StoreID = pstOp->StoreID;
	stLog.BuyRoleID = pstOp->BuyRoleID;
	if (pstOp->BuyName[0] != '\0')
	{
		strncpy(stLog.BuyName, pstOp->BuyName, sizeof(stLog.BuyName));
	}

	stLog.CurrMoney = pstOp->CurrMoney;
	stLog.Money = pstOp->Money;
	stLog.Num = pstOp->Num;
	stLog.PreMoney = pstOp->PreMoney;

	strncpy(stLog.StoreName, pstOp->StoreName, sizeof(stLog.StoreName));
	strncpy(stLog.StoreOwnerName, pstOp->StoreOwnerName, sizeof(stLog.StoreOwnerName));

	if (pstOp->RoleGrid.GridData.RoleItem.DefID > 0)
	{
		grid_to_exgrid( &stLog.Grid, &pstOp->RoleGrid, 1);
	}

	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int script_add_exp_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LPTDRMETA pstMeta;
	OPSCRIPTADDEXP*pstOp =  &pstOpLog->OpLog.OpScriptAddExp;

	pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib, "OpScriptAddExp");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name OpScriptAddExp fail \n");
		return -1;
	}

	tdr_utctime_to_tdrdatetime (&pstOp->LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)pstOp, sizeof(*pstOp));
	return 0;
}



int world_city_siege_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	LOGWORLDCITYSIEGE stLog;
	OPWORLDCITYSIEGE *psWorldCitySiege  = &pstOpLog->OpLog.OpWorldCitySiege;
	LPTDRMETA pstMeta = tdr_get_meta_by_name(pstEnv->pstDbLib,
	                                         "LogWorldCitySiege");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name LogWorldCitySiege fail\n");
		return -1;
	}

	memcpy(&stLog.LogData, psWorldCitySiege, sizeof(stLog.LogData));
	tdr_utctime_to_tdrdatetime (&stLog.LogTime, iSec);

	log_mem_to_db(pstEnv->hDBHandle, pstMeta, (char *)&stLog, sizeof(stLog));
	return 0;
}

int heart_beat_to_chat_mon(LOG_DBSVRENV *pstEnv,OPCHATMONHEARTBEATREQ *pstReq)
{
	OPLOGMONDATA stBody; 
	MONHEARTBEAT *pstRes = &stBody.HeartBeatRes;
	int iRet = 0;
		
	pstRes->ID = pstReq->ID;
	pstRes->Seq = pstReq->Seq;
	pstRes->Version = pstEnv->iOpLogMetaVersion;
	STRNCPY(pstRes->AppID,pstEnv->pstAppCtx->pszId,sizeof(pstRes->AppID));

	iRet = send_to_chat_mon_service(pstEnv,OP_LOG_MON_HEART_BEAT_RES, &stBody);
	
	return iRet;
}

int chat_mon_heart_beat_req_to_db(LOG_DBSVRENV *pstEnv, OP_LOG_META* pstOpLog, int iSec)
{
	OPCHATMONHEARTBEATREQ *pstReq =  &pstOpLog->OpLog.OpChatMonHeartBeatReq;
	int iNow = gs_stAppCtx.stCurr.tv_sec;

	STRNCPY(pstEnv->ChatMonConnUrl,pstReq->ConnUrl,sizeof(pstEnv->ChatMonConnUrl));

	if (pstEnv->iChatMonSocket < 0)
	{
		pstEnv->iChatMonSocket = tnet_connect(pstEnv->ChatMonConnUrl, 1000);
		if (pstEnv->iChatMonSocket < 0)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tnet_connect fail, url=%s", 
						pstEnv->ChatMonConnUrl);
			return -1;
		}
		else
		{
			pstEnv->iChatMonHeartBeartTime = iNow;			
			tnet_set_nonblock(pstEnv->iChatMonSocket, 1);
			tnet_set_sendbuff(pstEnv->iChatMonSocket, 20480);
			
			heart_beat_to_chat_mon(pstEnv,pstReq);
		}
	}
	else
	{
		pstEnv->iChatMonHeartBeartTime = iNow;			
		heart_beat_to_chat_mon(pstEnv,pstReq);
	}

	return 0;
	UNUSED(iSec);	//add by paraunused.pl
}

int tlog_dbsvr_msg(TAPPCTX *pstAppCtx, LOG_DBSVRENV *pstEnv, TLOGBINHEAD *pstTlogHead, char * pszRecvBuffer)
{
	TDRDATA stHost;	
	TDRDATA stNet;
	OP_LOG_META stOpLog;
	int i;

	UNUSED(pstAppCtx);
	
	stNet.iBuff = pstTlogHead->iBodyLen;
	stNet.pszBuff = pszRecvBuffer + pstTlogHead->chHeadLen;
	stHost.iBuff = sizeof(OP_LOG_META);
	stHost.pszBuff = (char *)&stOpLog;	
	
	if (0 > tdr_ntoh(pstEnv->pstLogicLogMeta, &stHost, &stNet, pstTlogHead->iBodyVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "op_log_meta ntoh fail, len=%d", stNet.iBuff);
		return -1;
	}	

	for (i=0; i<pstEnv->pstConf->Count; i++)
	{
		if (pstEnv->pstConf->LimitCmd[i].Opcmd == stOpLog.OpLogType)
		{
			return 0;
		}
	}

	pstEnv->iFrom = stOpLog.OpLogFrom;

	switch (stOpLog.OpLogType)
	{
	case OP_LOG_ROLELOGIN:
		return rolelogin_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLENEW:
		return rolenew_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLELOGOUT:
		return rolelogout_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLESHOP:
		return roleshop_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLEMAIL:
		return rolemail_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_EXCHG:
		return roleexchg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_STALL:
		return rolestall_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_STALL_BUY:
		return rolestallbuy_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_ARM_MACHINING:
		return rolearm_machining_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_MONSTER_DIE:
		return mon_die_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_PICK_ITEM:
		return pick_item_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_TASK:
		return role_task_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_CHAT:
		return role_chat_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_CLAN:
		return clan_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_PET:
		return role_pet_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_DEL:
		return role_del_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;                   
	case OP_LOG_ROLE_DIE:
		return role_die_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_PET_ATTR_CHG:
		return pet_attr_chg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_MAP_CHG:
		return role_map_chg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_SKILL:
		return role_skill_chg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_DIATHESIS:
		return role_diathesis_chg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_QIYUE:
		return role_qiyue_chg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_FAIRY:
		return role_fairy_chg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_RIDE:
		return role_ride_chg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_DESIGNATION:
		return role_designation_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_RESPACT:
		return role_respact_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_BOOTY_DROP:
		return drop_item_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_GM_CMD:
		return role_gmcmd_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_SNAPSHOT:
		return role_snapshot_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_USE_BANGDINGTIANYUAN:
		return role_bangdingtianyuan_chg_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_GET_MAILGOODS:
		return role_get_mailgoods_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_TRADE_INFO:
		return role_trade_info_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_LVUP_SNAPSHOT:
		return role_lvup_snapshot_to_db(pstEnv, &stOpLog,
		                                pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_ENTER_PWORLD:
		return role_enter_pworld_to_db( pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_USE_XP_SKILL:
		return role_use_xp_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_GIFT:
		return rolt_gift_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_PWORLD_TIME:
		return pworld_time_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_SECONDARY_SKILL:
		return role_secondary_skill_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_SECONDARY_SKILL_RECIPE:
		return role_secskill_recipe_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ACC_LOGIN:
		return role_acc_login_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_ACC_LOGOUT:
		return role_acc_logout_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_SHOPPING_EXCHANGE:
		role_shopping_exchange_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_MUD_ACC_LOGIN:
		return mud_acc_login_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_MUD_ACC_LOGOUT:
		return mud_acc_logout_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_MUD_ROLE_LOGIN:
		return mud_role_login_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_MUD_ROLE_LOGOUT:
		return mud_role_logout_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_MUD_TAKE_EXP:
		return mud_take_exp_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
		
	case OP_LOG_ROLE_ITEM_DEC:
		return role_item_dec_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_ROLE_ITEM_DROP:
		return role_item_drop_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_ROLE_DIE_DROP:
		return role_die_drop_to_db(pstEnv, &stOpLog,  pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_TASK_AWARD_ITEM:
		return role_task_item_award_to_db(pstEnv, &stOpLog,  pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_PWORLD_AWARD:
		return role_pworld_award_to_db(pstEnv, &stOpLog,  pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_PWORLD_CARD:
		return role_pworld_card_to_db(pstEnv, &stOpLog,  pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_BATTLE_AWARD:
		return role_battle_award_to_db(pstEnv, &stOpLog,  pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_RECIPE_MAKE:
		return role_recipe_make_to_db(pstEnv, &stOpLog,  pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_ROLE_PET_CATCH:
		return role_pet_catch_to_db(pstEnv, &stOpLog,  pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_MAP_TRANS:
		return map_trans_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_FENGYIN:
		return role_fengyin_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_LUCKY:
		return lucky_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_WORLD_CITY_SIEGE:
		return world_city_siege_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_MEDIA_GIFT:
		return rolt_media_gift_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_LTR_GIFT:
		return rolt_ltr_gift_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_CLAN_ACTIVE:
		return clan_active_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_DAILY_GOAL_AWARD:
		return daily_goal_award_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_BINDGOLD_CONSUME:
		bindgold_consume(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_BINDGOLD_CREATE:
		bindgold_create(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_MONEY_CONSUME:
		money_consume(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_MONEY_CREATE:
		money_create(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_IMPITEM_CONSUME:
		impitem_consume(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_IMPITEM_CREATE:
		impitem_create(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;	
	case OP_LOG_CLANPROP_CONSUME:
		clanprop_consume(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_CLANPROP_CREATE:
		clanprop_create(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;	
	case OP_LOG_CONTRI_CONSUME:
		contri_consume(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_CONTRI_CREATE:
		contri_create(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;		
	case OP_LOG_STRONG:
		strong_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;

	case OP_LOG_CHAT_MON_HEART_BEAT_REQ:
		return chat_mon_heart_beat_req_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_GENIUS:
		genius_to_db(pstEnv,  &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_SPAR:
		spar_to_db(pstEnv,  &stOpLog, pstTlogHead->stTime.iSec);
		break;	
	case OP_LOG_CLANBOON:
		boon_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_SCRIPT_ADD_EXP:
		script_add_exp_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_BOON_COND:
		boon_cond_to_db(pstEnv,  &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_STORE:
		store_buy_to_db(pstEnv,  &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_BATTLE_MONEY_USE:
		battle_money_use_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_STRONG_FINI:
		strongfini_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_SPANSTRONG:
		spanstrong_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;	
	case OP_LOG_CITY_FINI:
		cityfini_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_OPEN_BOX:
		open_box_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	case OP_LOG_HOME:
		home_to_db(pstEnv, &stOpLog, pstTlogHead->stTime.iSec);
		break;
	default:
		return 0;
		break;
	}

	return 0;
}

int z_tlogdb_proc(TAPPCTX *pstAppCtx, LOG_DBSVRENV *pstEnv)
{
	int iRecv = 0;
	int iBusy = 0;
	struct sockaddr stCliAddr;
	socklen_t iCliAddrLen;
	TLOGBINHEAD stTlogHead;

	UNUSED(pstAppCtx);
	
	iCliAddrLen = sizeof(stCliAddr);
	iRecv = recvfrom(pstEnv->iSocket, pstEnv->szRecvBuffer, sizeof(pstEnv->szRecvBuffer), 0, &stCliAddr, &iCliAddrLen);
	if (0 < iRecv)
	{	
		iBusy = 1;
		
		if (0 > tlogbin_ntoh_head(&stTlogHead, pstEnv->szRecvBuffer, iRecv))
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "tlogbin ntoh fail, len=%d", iRecv);
			return -1;
		}
		
		tlog_dbsvr_msg(pstAppCtx, pstEnv, &stTlogHead, pstEnv->szRecvBuffer);
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

int z_tlogdb_tick(TAPPCTX *pstAppCtx, LOG_DBSVRENV *pstEnv)
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	if ( pstAppCtx->stCurr.tv_sec-pstEnv->tCheckDB > 30)
	{
		tdr_keep_dbmsconnection(pstEnv->hDBHandle);
		pstEnv->tCheckDB = pstAppCtx->stCurr.tv_sec;
	}
	
	return 0;
}

int z_tlogdb_reload(TAPPCTX *pstAppCtx, LOG_DBSVRENV *pstEnv)
{
	TLOG_DBCONF   *pstPreConf;
	
	pstPreConf = (TLOG_DBCONF   *)pstAppCtx->stConfPrepareData.pszBuff;

	memcpy(pstEnv->pstConf, pstPreConf, sizeof(TLOG_DBCONF));

	pstEnv->pstConf->DropItemLimit = pstPreConf->DropItemLimit;
	pstEnv->pstConf->MonSubLimit = pstPreConf->MonSubLimit;
	
	printf("tlog_db reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "tlog_db reload");

	return 0;
}

int z_tlogdb_stop(TAPPCTX *pstAppCtx, LOG_DBSVRENV *pstEnv)
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

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)z_tlogdb_init;


	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_tlog_db;
	gs_stAppCtx.stConfData.pszMetaName = "tlog_dbconf";

    	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
	
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	
	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)z_tlogdb_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)z_tlogdb_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)z_tlogdb_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)z_tlogdb_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)z_tlogdb_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);

	return iRet;
}
