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

//#include "zone_svr.h"
#include "zone_svr_conf_desc.h"
#include "tapp/tapp.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "zone_ctrl_conf_desc.h"
#include "version.h"
#include "entity.h"
#include "comm_misc.h"
#include "proto_comm.h"
#include "proto_ss.h"
#include "tbus/tbus.h"
#define UNUSED( var ) ((void)var)


#define MAX_CMD_LEN 2048

struct tagzone_ctrlEnv
{
	ZONE_CTRLCONF *pstConf;
	ZONE_CTRLRUN_CUMULATE *pstRunCumu;
	ZONE_CTRLRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	char szCmd[MAX_CMD_LEN];
	char sBuff[sizeof(SSPKG)];
	SSPKG stPkg;
	int iZoneSvrID;
	SSHANDLE *pstSSHan;
	LPTDRMETA pstMeta;
};

typedef struct tagzone_ctrlEnv		ZONE_CTRLENV;
typedef struct tagzone_ctrlEnv		*LZONE_CTRLENV;

#define Z_SSHEAD_INIT(pstHead, unCmd) {(pstHead)->Cmd = unCmd;(pstHead)->Magic = PROTOCOL_MAGIC;}


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_zone_ctrl[];
extern unsigned char g_szMetalib_proto_ss[];
extern unsigned char g_szMetalib_zone_svr[];


ZONE_CTRLENV gs_stEnv;
static ZONE_CTRLRUN_CUMULATE gs_stCumu;
static ZONE_CTRLRUN_STATUS gs_stStat;

int zone_ctrl_init(TAPPCTX *pstAppCtx, ZONE_CTRLENV *pstEnv)
{
	LPTDRMETALIB pstLib;
	
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (ZONE_CTRLCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunCumu = (ZONE_CTRLRUN_CUMULATE *)pstAppCtx->stRunDataCumu.pszBuff;
	if (NULL == pstEnv->pstRunCumu)
	{
		pstEnv->pstRunCumu = &gs_stCumu;
	}
	
	pstEnv->pstRunStat = (ZONE_CTRLRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	pstLib = (LPTDRMETALIB)g_szMetalib_proto_ss;	
	pstEnv->pstMeta = tdr_get_meta_by_name(pstLib, "SSPkg");
	if (NULL == pstEnv->pstMeta)
	{
		return -1;
	}

	GET_WORLD_ENTITY_ID(pstEnv->iZoneSvrID, pstAppCtx->iId, FUNC_ZONE_SVR);
	if (0 > ss_han_create(g_szMetalib_proto_ss, &pstEnv->pstSSHan))
	{
		return -1;
	}
	
	//printf("zone_ctrl start\n");
	//tlog_info(pstEnv->pstLogCat, 0, 0, "zone_ctrl start");

	return 0;
}

int zone_ctrl_parse_cmd(TAPPCTX *pstAppCtx, ZONE_CTRLENV *pstEnv)
{
	char *pch;
	SSCTRLCMD *pstCmd;
	int iArg = 0;
	

	Z_SSHEAD_INIT(&pstEnv->stPkg.Head, ZONE_CTRL);
	pstCmd	= &pstEnv->stPkg.Body.ZoneCtrl.CtrlCmd;

	
	if (NULL != (pch = strstr(pstEnv->szCmd, "test")))
	{
		pstEnv->stPkg.Body.ZoneCtrl.Cmd = TEST_REQ;
		pstCmd->TestReq.Res = 0;
	}
	else if (NULL != (pch = strstr(pstEnv->szCmd, "shutdown")))
	{
		pch += strlen("shutdown") + 1;
		if (isdigit(*pch))
		{
			iArg = atoi(pch);
		}
		else
		{
			iArg = 60;
		}

		pstEnv->stPkg.Body.ZoneCtrl.Cmd = SHUTDOWN_REQ;
		pstCmd->ShutDownReq.Time = iArg;
	}
	else if(strcmp(pstEnv->szCmd,"shmdel") == 0)
	{
		ZONE_SVRCONF stConf;
		LPTDRMETALIB pstLib;
		TDRDATA stHost;
		LPTDRMETA pstMeta;
		char stCmd[256];

		stHost.iBuff = sizeof(stConf);
		stHost.pszBuff = (char *)&stConf;
		pstLib = (LPTDRMETALIB)&g_szMetalib_zone_svr[0];
		pstMeta = tdr_get_meta_by_name(pstLib, "zone_svrconf");
		if(NULL == pstMeta)
		{
			return -1;
		}
		if( tdr_input_file(pstMeta, &stHost, "../cfg/zone_svr.xml", 0, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME) < 0)
		{
			return -1;
		}              
		snprintf(stCmd, sizeof(stCmd), "ipcrm -M %d", stConf.PlayerShmKey);
		system(stCmd);

		snprintf(stCmd, sizeof(stCmd), "ipcrm -M %d", stConf.UinCacheShmKey);
		system(stCmd);

		snprintf(stCmd, sizeof(stCmd), "ipcrm -M %d", stConf.ResShmKey);
		system(stCmd);

		snprintf(stCmd, sizeof(stCmd), "ipcrm -M %d", stConf.MapShmKey);
		system(stCmd);

		exit(0);
		
	}
	else if (NULL != (pch = strstr(pstEnv->szCmd, "loadscript")))
	{
		ZONELOADSCRIPTREQ *psLoadScriptReq = &pstEnv->stPkg.Body.ZoneCtrl.CtrlCmd.LoadScriptReq;
		
		pch += strlen("loadscript") + 1;

		if (*pch!='\0' )
		{
			psLoadScriptReq->Type = LOADSCRIPT_ONE;
			snprintf(psLoadScriptReq->Name, sizeof(psLoadScriptReq->Name), "../cfg/script/%s.mac", pch);
		}
		else
		{
			psLoadScriptReq->Type = LOADSCRIPT_ALL;
			psLoadScriptReq->Name[0] = 0;
		}		
		
		pstEnv->stPkg.Body.ZoneCtrl.Cmd = LOADSCRIPT_REQ;
	}
	else if(NULL != (pch = strstr(pstEnv->szCmd, "reload")))
	{
		ZONERELOADREQ *pstReLoadReq = &pstEnv->stPkg.Body.ZoneCtrl.CtrlCmd.ReLoadReq;

		pstReLoadReq->Res = 0;
		pstEnv->stPkg.Body.ZoneCtrl.Cmd = RELOAD_REQ;
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int zone_ctrl_proc(TAPPCTX *pstAppCtx, ZONE_CTRLENV *pstEnv)
{
	int iBusy=0;
	int iSrc;
	int iDst;
	TDRDATA stData;
	int i;
	SSPKG stSSRes;
	
	if (pstEnv->szCmd[0] == 0)
	{
		exit(-1);
	}
	else
	{
		//先把残余的消息收空
		for (;;)
		{
			iSrc = TBUS_ADDR_ALL;
	       	 iDst = TBUS_ADDR_ALL;
			stData.iBuff = sizeof(pstEnv->sBuff);
			stData.pszBuff = pstEnv->sBuff;
			if (0 > tbus_recv(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, (size_t *)&stData.iBuff, 0))
			{
				break;
			}
		}

		printf("get cmd=%s\n", pstEnv->szCmd);
		tlog_info(pstEnv->pstLogCat, 0, 0, "get cmd=%s\n", pstEnv->szCmd);

		if (0 > zone_ctrl_parse_cmd(pstAppCtx, pstEnv))
		{
			printf("parse cmd=%s fail\n", pstEnv->szCmd);
			tlog_error(pstEnv->pstLogCat, 0, 0, "parse cmd=%s fail", pstEnv->szCmd);
			exit(-1);
		}

		stData.iBuff = sizeof(pstEnv->sBuff);
		stData.pszBuff = pstEnv->sBuff;

		if (0 > ss_pack(pstEnv->pstSSHan, &stData, &pstEnv->stPkg, 0))
		{
			printf("ss_pack fail\n");
			tlog_error(pstEnv->pstLogCat, 0, 0, "ss_pack fail");
			exit(-1);
		}

		iDst = pstEnv->iZoneSvrID;
		iSrc = pstAppCtx->iId;
		if (0 > tbus_send(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, stData.iBuff, 0))
		{
			printf("tbus_send fail\n");
			tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_send fail");
			exit(-1);
		}

		usleep(10*1000);
		for (i=0; i<20; i++)
		{
			iSrc = TBUS_ADDR_ALL;
	        	iDst = TBUS_ADDR_ALL;
			stData.iBuff = sizeof(pstEnv->sBuff);
			stData.pszBuff = pstEnv->sBuff;
			if (0 == tbus_recv(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, (size_t *)&stData.iBuff, 0))
			{
				if (0 == ss_unpack(pstEnv->pstSSHan, stData.pszBuff, stData.iBuff, &stSSRes, 0))
				{
					tlog_info_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstMeta, &stSSRes, sizeof(stSSRes), 0);
				}
				exit(0);
			}

			usleep(10*1000);
		}

		printf("tbus_recv fail\n");
		tlog_error(pstEnv->pstLogCat, 0, 0, "tbus_recv fail");
		exit(-1);
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

int zone_ctrl_reload(TAPPCTX *pstAppCtx, ZONE_CTRLENV *pstEnv)
{
	ZONE_CTRLCONF   *pstPreConf;
	
	pstPreConf = (ZONE_CTRLCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("zone_ctrl reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "zone_ctrl reload");

	return 0;
}

int zone_ctrl_fini(TAPPCTX *pstAppCtx, ZONE_CTRLENV *pstEnv)
{
	ss_han_destroy(&pstEnv->pstSSHan);
	
	printf("zone_ctrl finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "zone_ctrl finish");
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int zone_ctrl_tick(TAPPCTX *pstAppCtx, ZONE_CTRLENV *pstEnv)
{
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstEnv);	//add by paraunused.pl
}

int zone_ctrl_stop(TAPPCTX *pstAppCtx, ZONE_CTRLENV *pstEnv)
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
	int i;
	char *pch;
	
	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)zone_ctrl_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_zone_ctrl;
	gs_stAppCtx.stConfData.pszMetaName = "zone_ctrlconf";
	gs_stAppCtx.pszLogFile = "../log/zone_ctrl";
	gs_stAppCtx.pszConfFile = "../cfg/zone_ctrl.xml";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
	gs_stAppCtx.iNoLoadConf = 1;
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	for (i=1; i<argc; i++)
	{
		if (NULL != (pch = strstr(argv[i], "--cmd=")))
		{
			pch += strlen("--cmd=");
			STRNCPY(gs_stEnv.szCmd, pch, sizeof(gs_stEnv.szCmd));
		}
	}

	if (gs_stEnv.szCmd[0] == 0)
	{
		printf("no cmd\n");
		return -1;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)zone_ctrl_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)zone_ctrl_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)NULL;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)NULL;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)NULL;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


