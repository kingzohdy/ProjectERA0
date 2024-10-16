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
#include "recv_report_conf_desc.h"
#include "version.h"
#include "comm_misc.h"
#include "apps/tormapi/torm_protocol_api.h"
#include "role_db_meta.h"

#define UNUSED( var ) ((void)var)
#define LOOP_NUM 100

struct tagrecv_reportEnv
{
	RECV_REPORTCONF *pstConf;
	RECV_REPORTRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	int iSocket;
	SSHANDLE *pstSSHan; 
	LPTDRMETALIB pstResLib;
	TDRDBHANDLE hResDBHandle;
	int iCheckDB;

	TORMPROTOCOLHANDLE hTorm;
	char szRecvBuffer[10240];
	char szSendBuffer[10240];
};

typedef struct tagrecv_reportEnv		RECV_REPORTENV;
typedef struct tagrecv_reportEnv		*LRECV_REPORTENV;


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_recv_report[];

RECV_REPORTENV gs_stEnv;
static RECV_REPORTRUN_STATUS gs_stStat;


extern unsigned char g_szMetalib_proto_ss[];


static int dir_msg(RECV_REPORTENV *pstEnv, char *stBuff, int iRecvNum, SSPKG *pstPkg)
{
	
	if (0 > ss_unpack(pstEnv->pstSSHan, stBuff, iRecvNum, pstPkg, 0))
	{
		return -1;
	}
	return 0;
}

int recv_report_init(TAPPCTX *pstAppCtx, RECV_REPORTENV *pstEnv)
{
	TDRDBMS stDBMS;
	char szErrorMsg[2048]={0};
	
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (RECV_REPORTCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (RECV_REPORTRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
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

	if (0 > ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan))
	{
		printf("ss_han_create is fail\n");
		return -1;
	}

	if (0 > torm_create_handle_from_dir(&pstEnv->hTorm, pstEnv->pstConf->szDbMetaDir))
	{
		printf("torm_create_handle fail\n");
		return -1;
	}

	if (0 > tdr_load_metalib(&pstEnv->pstResLib, pstEnv->pstConf->szRoleDbMeta))
	{
		printf("load meta lib resdb_meta fail\n");
		return -1;
	}

	if (pstEnv->pstConf->iRecordType > 0)
	{
		memset(&stDBMS, 0, sizeof(stDBMS));
		STRNCPY(stDBMS.szDBMSName, TDR_DEFAULT_DBMS, sizeof(stDBMS.szDBMSName));
		STRNCPY(stDBMS.szDBMSCurDatabaseName, pstEnv->pstConf->szOnlineDBName, sizeof(stDBMS.szDBMSCurDatabaseName));
		STRNCPY(stDBMS.szDBMSConnectionInfo, pstEnv->pstConf->szOnlineDBConnect, sizeof(stDBMS.szDBMSConnectionInfo));	
		STRNCPY(stDBMS.szDBMSUser, pstEnv->pstConf ->szOnlineDBUser, sizeof(stDBMS.szDBMSUser));
		STRNCPY(stDBMS.szDBMSPassword, pstEnv->pstConf->szOnlineDBPasswd, sizeof(stDBMS.szDBMSPassword));
		stDBMS.iReconnectOpt = 1;	
		
		if (0 > tdr_open_dbhanlde(&pstEnv->hResDBHandle, &stDBMS, &szErrorMsg[0]))
		{
			printf("tdr_open_dbhanlde fail: %s", szErrorMsg);
			return -1;
		}

		pstEnv->iCheckDB = pstAppCtx->stCurr.tv_sec;
	}
	
	printf("recv_report start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "recv_report start, RecordType %d", pstEnv->pstConf->iRecordType);

	return 0;
}

static int write_reprot_to_file(RECV_REPORTENV *pstEnv,SSPKG *pstPkg)
{
	char stFileName[128];
	char stBuff[64];
	char szPachName[64];
	FILE *fp;
	struct tm *pstTm;
	int i,j;
	HOURDATA *pstHourData;
	SSREPORTDATAALL *pstRePort;
	SSDIRINFOHOUR *pstDirInfoHour;

	//printf("write data\n");
	
	if (pstPkg->Head.Cmd == RECORD_DIR_INFO)
	{
		pstRePort = &pstPkg->Body.RePortDataAll;

		if(pstRePort->time <= 0)
		{
			return -1;
		}	
		pstTm = localtime((time_t*)&pstRePort->time);
		
		snprintf(szPachName, sizeof(szPachName), "%s/world_stat", pstEnv->pstConf->szPath);
		if(access(szPachName, F_OK) < 0)//  文件夹不存在     创建
		{
			if(mkdir(szPachName, S_IWRITE|S_IREAD|S_IEXEC) < 0)
			{
				printf("mkdir fail\n");
				return -1;
			}
		}
	
		snprintf(szPachName,sizeof(szPachName), "%s/world_stat/%d-%d-%d", 
				pstEnv->pstConf->szPath,pstTm->tm_year+1900, pstTm->tm_mon+1, pstTm->tm_mday);
		if(access(szPachName, F_OK) < 0)
		{
			if(mkdir(szPachName, S_IWRITE|S_IREAD|S_IEXEC) < 0)
			{
				printf("mkdir fail\n");
				return -1;
			}
		}
		
		
		for (i=0; i<pstRePort->WorldNum; i++)
		{
			snprintf(stFileName,sizeof(stFileName),"%s/%d.txt", szPachName, pstRePort->RePort[i].WorldID);
			fp = fopen(stFileName,"w");
			for (j=0; j<pstRePort->RePort[i].DataNum; j++)
			{
				pstHourData = &pstRePort->RePort[i].DirHourData[j];
				snprintf(stBuff, sizeof(stBuff), "[Hour=%d    OnLineNum=%d]\n", pstHourData->Hour, pstHourData->PlayerNum);
				fwrite(stBuff, strlen(stBuff), 1, fp);
			}
			fclose(fp);
		}

	}
	else if(pstPkg->Head.Cmd == DIR_INFO_HOUR)
	{
		pstDirInfoHour = &pstPkg->Body.DirInfoHour;

		if(pstDirInfoHour->Time <= 0)
		{
			return -1;
		}
		pstTm = localtime((time_t*)&pstDirInfoHour->Time);
		
		snprintf(szPachName, sizeof(szPachName), "%s/total_stat", pstEnv->pstConf->szPath);
		if(access(szPachName, F_OK) < 0)
		{
			if(mkdir(szPachName, S_IWRITE|S_IREAD|S_IEXEC) < 0)
			{
				printf("mkdir fail\n");
				return -1;
			}
		}
		
		snprintf(szPachName,sizeof(szPachName), "%s/total_stat/%d-%d-%d", 
				pstEnv->pstConf->szPath,pstTm->tm_year+1900, pstTm->tm_mon+1, pstTm->tm_mday);
		if(access(szPachName, F_OK) < 0)
		{
			if(mkdir(szPachName, S_IWRITE|S_IREAD|S_IEXEC) < 0)
			{
				printf("mkdir fail\n");
				return -1;
			}
		}
		
		

		snprintf(stFileName,sizeof(stFileName),"%s/%d.txt",szPachName, pstDirInfoHour->Hour);
		fp = fopen(stFileName,"w");
		for (i=0; i<pstDirInfoHour->WorldNum; i++)
		{
			snprintf(stBuff, sizeof(stBuff), "[WorldID=%d   OnLineNum=%d]\n", 
				pstDirInfoHour->WorldInfoHour[i].WorldID, pstDirInfoHour->WorldInfoHour[i].OnLineNum);

			fwrite(stBuff, strlen(stBuff), 1, fp);
		}
		fclose(fp);
	}
	else
	{
		return -1;
	}

	
	return 0;
}

int dirinfo_to_db(TDRDBHANDLE hDBHandle, LPTDRMETA pstMeta, char* pszData, int iUnit)
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
		printf("生成插入资源记录的SQL语句失败: %s dbms error:%s\n",
			tdr_error_string(iRet), tdr_dbms_error(hDBHandle));
		return -1;
	}

	/*执行sql语句*/
	iRet = tdr_query(&hDBResult, hDBHandle, &stSql);
	tdr_free_dbresult(&hDBResult);
	if (0 != iRet)
	{
		//printf("执行Mysql语句:%s 失败",stSql);
		return -1;
	}
	return 0;
}

int write_report_to_db(RECV_REPORTENV *pstEnv,SSPKG *pstPkg)
{
	int i;
	SSDIRINFODB *pstDirInfoDB = &pstPkg->Body.DirInfoDB;
	LPTDRMETA pstMeta = NULL;
	TDRDATA stHost;
	ONLINERECORD stDirOnline;
	
	if (pstPkg->Head.Cmd != DIR_INFO_DB)
	{
		return -1;
	}
	
	if (pstDirInfoDB->WorldNum <= 0)
	{
		return -1;
	}

	pstMeta = tdr_get_meta_by_name(pstEnv->pstResLib, "OnlineReCord");
	if (!pstMeta)
	{
		printf("tdr_get_meta_by_name is fail \n");
		return -1;
	}
	
	for (i=0; i<pstDirInfoDB->WorldNum; i++)
	{
		stDirOnline.OnlineNum 	= pstDirInfoDB->DirInfo[i].OnlineNum;
		stDirOnline.WorldID 	= pstDirInfoDB->DirInfo[i].WorldID;
		stDirOnline.RegionID 	= pstDirInfoDB->DirInfo[i].RegionID;
		stDirOnline.MudOnlineNum = pstDirInfoDB->DirInfo[i].MudOnlineNum;
		strncpy(stDirOnline.MapInfo,pstDirInfoDB->DirInfo[i].MapInfo,sizeof(stDirOnline.MapInfo));
		tdr_utctime_to_tdrdatetime (&stDirOnline.Time, pstDirInfoDB->DirInfo[i].Time);
		
		stHost.pszBuff 	= (char*)&stDirOnline;
		stHost.iBuff 		= sizeof(stDirOnline);
		if (dirinfo_to_db(pstEnv->hResDBHandle, pstMeta, stHost.pszBuff, stHost.iBuff) < 0)
		{
			return -1;
		}
	}
	
	//printf("write db succ\n");
	return 0;
}

int recv_report_proc(TAPPCTX *pstAppCtx, RECV_REPORTENV *pstEnv)
{
	int iBusy = 0;
	//int iLoopNum = 0;
	int iRecv;
	struct sockaddr stCliAddr;
	socklen_t iCliAddrLen = sizeof(stCliAddr);
	
	SSPKG stPkg;

	//while(LOOP_NUM > iLoopNum)
	{
		//iLoopNum ++;
		iRecv = recvfrom(pstEnv->iSocket, pstEnv->szRecvBuffer, sizeof(pstEnv->szRecvBuffer), 0, &stCliAddr, &iCliAddrLen);

		if(iRecv > 0)
		{
			iBusy = 1;
			if(dir_msg(pstEnv,pstEnv->szRecvBuffer,iRecv,&stPkg) < 0)
			{
				return -1;
			}

			tlog_info(pstEnv->pstLogCat, 0, 0, "recv report msg, RecordType %d", pstEnv->pstConf->iRecordType);

			if (pstEnv->pstConf->iRecordType == 0)
			{
				write_reprot_to_file(pstEnv,&stPkg);
			}
			else if (pstEnv->pstConf->iRecordType == 1)
			{
				write_report_to_db(pstEnv,&stPkg);
			}
			else if (pstEnv->pstConf->iRecordType == 2)
			{
				write_reprot_to_file(pstEnv,&stPkg);
				write_report_to_db(pstEnv,&stPkg);
			}
			else
			{
				return -1;
			}
			
			
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
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int recv_report_reload(TAPPCTX *pstAppCtx, RECV_REPORTENV *pstEnv)
{
	RECV_REPORTCONF   *pstPreConf;
	
	pstPreConf = (RECV_REPORTCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("recv_report reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "recv_report reload");

	return 0;
}

int recv_report_fini(TAPPCTX *pstAppCtx, RECV_REPORTENV *pstEnv)
{
	tnet_close(pstEnv->iSocket);
	ss_han_destroy(&pstEnv->pstSSHan);
	tdr_close_dbhanlde(&pstEnv->hResDBHandle);
	printf("recv_report finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "recv_report finish");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int recv_report_tick(TAPPCTX *pstAppCtx, RECV_REPORTENV *pstEnv)
{
	if (pstEnv->pstConf->iRecordType > 0 && pstAppCtx->stCurr.tv_sec-pstEnv->iCheckDB > 30)
	{
		tdr_keep_dbmsconnection(pstEnv->hResDBHandle);
		pstEnv->iCheckDB = pstAppCtx->stCurr.tv_sec;
	}
	return 0;
}

int recv_report_stop(TAPPCTX *pstAppCtx, RECV_REPORTENV *pstEnv)
{
	int iReady=0;
	
	iReady = 1;
	if (iReady)
	{
		return -1;
	}
	else
	{
		return 0;
	}
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)recv_report_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_recv_report;
	gs_stAppCtx.stConfData.pszMetaName = "recv_reportconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)recv_report_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)recv_report_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)recv_report_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)recv_report_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)recv_report_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


