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
#include "TEMPLE_ORI_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"

#define MAX_THREAD 256
#define LOCAL_BUS_ADDR_START 10
#define CHANNEL_SIZE 1024*256
#define MAX_MSG_SIZE 64000
#define CHANNEL_APPEND 128
//char szWatchMsg[MAX_MSG_SIZE - sizeof()];

typedef struct tagChannelAppend
{
	char sAppend[CHANNEL_APPEND];
}ChannelAppend;

typedef struct tagThreadMgr
{
	pthread_t tid;
	int iIdx;
	LPTBUSCHANNEL	pstChannel0;
	LPTBUSCHANNEL	pstChannel1;
	char cIsExit;
	char cIsValid;
}ThreadMgr;

struct tagTEMPLE_ORIEnv
{
	TEMPLE_BIGCONF *pstConf;
	TEMPLE_BIGRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
	TAPPCTX *pstAppCtx;
	pthread_mutex_t stLogMutex;
	int iThread;
	ThreadMgr astThread[MAX_THREAD];
};

typedef struct tagTEMPLE_ORIEnv		TEMPLE_BIGENV;
typedef struct tagTEMPLE_ORIEnv		*LTEMPLE_BIGENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_TEMPLE_ORI[];

TEMPLE_BIGENV gs_stEnv;
static TEMPLE_BIGRUN_STATUS gs_stStat;

typedef enum 
{
	MUTI_LOGLEVEL_MIN = 0,	
	MUTI_LOGLEVEL_TRACE,	
	MUTI_LOGLEVEL_DEBUG,	
	MUTI_LOGLEVEL_INFO,
	MUTI_LOGLEVEL_WARN,	
	MUTI_LOGLEVEL_ERROR,
	MUTI_LOGLEVEL_SEVERE,
}MUTILOGLEVEL;

#define MutiLog(iLevel, szFormat, args...) \
		do{\
			pthread_mutex_lock(&gs_stEnv.stLogMutex); \
			switch(iLevel)\
			{\
			case MUTI_LOGLEVEL_TRACE:\
				tlog_trace(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_DEBUG:\
				tlog_debug(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_INFO:\
				tlog_info(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_WARN:\
				tlog_warn(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_ERROR:\
				tlog_error(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			case MUTI_LOGLEVEL_SEVERE:\
				tlog_fatal(gs_stEnv.pstLogCat, 0, 0, szFormat, ##args);\
				break;\
			default:\
				break;\
			}\
			pthread_mutex_unlock(&gs_stEnv.stLogMutex); \
		  }while(0)

void* thread_proc(void* pvArg)
{
	int iRet;
	ThreadMgr *pstThread;
	char szMsg[MAX_MSG_SIZE];
	int iLen;
	int iIdle = 0;

	pstThread = (ThreadMgr *)pvArg;

	while(1)
	{
		if( pstThread->cIsExit )
			break;

		iLen = sizeof(szMsg);
		iRet = tbus_channel_recv(pstThread->pstChannel1, szMsg, &iLen, 0);
		if (0 != iRet)
		{
			if ((int)TBUS_ERR_CHANNEL_EMPTY == iRet)
			{
				iIdle++;
				if (iIdle > gs_stAppCtx.iIdleCount)
				{
					iIdle = 0;
					usleep(gs_stAppCtx.iIdleSleep*1000);
				}
			}
			else
			{
				MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error Ret %d", iRet);
			}
			
			continue;
		}

		iIdle = 0;
		
		if (iLen <= (int)sizeof(ChannelAppend))
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error iLen %d", iLen);
			continue;
		}

		//消息处理并返回，返回消息打包在&szMsg[sizeof(ChannelAppend)]中，然后返回szMsg, Len 为消息长度+ sizeof(ChannelAppend)
		iRet = tbus_channel_send(pstThread->pstChannel1, szMsg, iLen, 0);
		if (0 != iRet)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_send error Ret %d", iRet);
			continue;
		}
	}

	return (void*)0;
}

int thread_init_one(TEMPLE_BIGENV *pstEnv, ThreadMgr *pstThread, int iIdx)
{
	int iKey;
	
	if (0 == pstEnv->pstConf->ChannelKeyStart)
	{
		iKey = ftok(pstEnv->pstAppCtx->pszPidFile, iIdx);
	}
	else
	{
		iKey = pstEnv->pstConf->ChannelKeyStart + iIdx;
	}

	pstThread->iIdx = iIdx;
	if (0 > tbus_open_channel(&pstThread->pstChannel0,  pstEnv->pstAppCtx->iId, LOCAL_BUS_ADDR_START+iIdx,  iKey, CHANNEL_SIZE))
	{
		return -1;
	}

	if (0 > tbus_open_channel(&pstThread->pstChannel1, LOCAL_BUS_ADDR_START+iIdx, pstEnv->pstAppCtx->iId,   iKey, CHANNEL_SIZE))
	{
		return -1;
	}

	pstThread->cIsValid = 1;
	
	if( pthread_create(&pstThread->tid, NULL, thread_proc, pstThread)<0 )
	{
		return -1;
	}

	return 0;
}

int thread_init(TEMPLE_BIGENV *pstEnv)
{
	int i;

	pstEnv->iThread = pstEnv->pstConf->MutiThreadNum;
	if (0 == pstEnv->iThread ||MAX_THREAD < pstEnv->iThread)
	{
		pstEnv->iThread = MAX_THREAD;
	}

	for (i=0; i<pstEnv->iThread; i++)
	{
		if (0 > thread_init_one(pstEnv, &pstEnv->astThread[i], i))
		{
			return -1;
		}
	}
	
	return 0;
}

int TEMPLE_ORI_init(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		return -1;
	}
	pstEnv->pstConf = (TEMPLE_BIGCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstRunStat = (TEMPLE_BIGRUN_STATUS *)pstAppCtx->stRunDataStatus.pszBuff;
	if (NULL == pstEnv->pstRunStat)
	{
		pstEnv->pstRunStat = &gs_stStat;
	}

	pstEnv->pstAppCtx = pstAppCtx;

	pthread_mutex_init(&pstEnv->stLogMutex, NULL);
	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	

	if (0 > thread_init(pstEnv))
	{
		printf("thread_init fail\n");
		return -1;	
	}
	
	printf("TEMPLE_ORI start\n");
	MutiLog(MUTI_LOGLEVEL_INFO, "TEMPLE_ORI start");

	return 0;
}

int store_appendinfo(TAPPCTX *pstAppCtx, ChannelAppend *pstChannelAppend)
{
	int iLen;

	iLen = sizeof(pstChannelAppend->sAppend);
	
	return tbus_get_pkg_header(pstAppCtx->iBus, pstChannelAppend->sAppend, &iLen);
}

int restore_appendinfo(TAPPCTX *pstAppCtx, ChannelAppend *pstChannelAppend)
{
	int iLen;

	iLen = sizeof(pstChannelAppend->sAppend);
	
	return tbus_set_pkg_header(pstAppCtx->iBus, pstChannelAppend->sAppend, iLen);
}

int proc_multi_bus2channel(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	int i;
	int iBusy;
	int iRet;
	int iSrc;
	int iDst;
	TDRDATA stData;
	char szMsg[MAX_MSG_SIZE];
	static int iRobin = 0;

	iBusy = 0;
	for (i=0; i<pstEnv->iThread; i++)
	{
		iSrc = TBUS_ADDR_ALL;
		iDst = pstEnv->pstAppCtx->iId;
		stData.iBuff = sizeof(szMsg) - sizeof(ChannelAppend);
		stData.pszBuff = (char *)szMsg + sizeof(ChannelAppend);
		
		iRet = tbus_recv(pstAppCtx->iBus, &iSrc, &iDst, stData.pszBuff, (size_t *)&stData.iBuff, 0);
		if (0 == iRet)
		{
			iBusy = 1;
			
			if (0 != store_appendinfo(pstAppCtx, (ChannelAppend *)szMsg))
			{
				MutiLog(MUTI_LOGLEVEL_ERROR, "store_appendinfo error");
			}
			
			stData.iBuff += sizeof(ChannelAppend);
			stData.pszBuff = (char *)szMsg;
			
			//根据策略选择一个线程,  暂时使用robin-round
			iRet = tbus_channel_send(pstEnv->astThread[(iRobin++) % pstEnv->iThread].pstChannel0, 
									stData.pszBuff, stData.iBuff, 0);
			if (0 != iRet)
			{
				MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_send error ret %d", iRet);
			}
		}
		else
		{
			break;
		}
	}
	
	return iBusy;	
}

int channel2bus_one(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv, ThreadMgr *pstThread)
{
	int iLen;
	int iRet;
	char szMsg[MAX_MSG_SIZE];
	int iSrc, iDst;
	int iBusy = 0;

	iLen = sizeof(szMsg);
	iRet = tbus_channel_recv(pstThread->pstChannel0, szMsg, &iLen, 0);
	if (0 == iRet)
	{
		iBusy = 1;

		if (iLen <= (int)sizeof(ChannelAppend))
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv len %d", iLen);
			return iBusy;
		}

		if (0 != restore_appendinfo(pstAppCtx, (ChannelAppend *)szMsg))
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "restore_appendinfo error");
			return iBusy;
		}

		iSrc = pstAppCtx->iId;
		iDst = 0;
				
		iRet = tbus_backward(pstAppCtx->iBus, &iSrc, &iDst, &szMsg[sizeof(ChannelAppend)], iLen-sizeof(ChannelAppend), 0);

		if (0 != iRet)
		{
			MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_backward error ret %d", iRet);
		}
	}
	else if ((int)TBUS_ERR_CHANNEL_EMPTY != iRet)
	{
		MutiLog(MUTI_LOGLEVEL_ERROR, "tbus_channel_recv error ret %d", iRet);
	}
		
	return iBusy;	
}

int proc_multi_channel2bus(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	int i;
	int iBusy = 0;

	for (i=0; i<pstEnv->iThread; i++)
	{
		iBusy = channel2bus_one(pstAppCtx, pstEnv, &pstEnv->astThread[i]);
	}

	return iBusy;
}

int TEMPLE_ORI_proc(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	int iBusy=0;
	
	//如果进程还没不是退出状态，转发bus的消息到channel
	if (!tapp_is_exit())
	{
		iBusy = proc_multi_bus2channel(pstAppCtx, pstEnv);
	}

	/*读取内部Channel消息转发到bus*/
	iBusy = proc_multi_channel2bus(pstAppCtx, pstEnv);

	if (iBusy)
		return 0;
	else
		return -1;
}

int TEMPLE_ORI_reload(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	TEMPLE_BIGCONF   *pstPreConf;
	
	pstPreConf = (TEMPLE_BIGCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("TEMPLE_ORI reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "TEMPLE_ORI reload");

	return 0;
}

int channel2bus_one_recvall(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv, ThreadMgr* pstThread)
{
	while(channel2bus_one(pstAppCtx, pstEnv, pstThread))
	{
		;
	}

	return 0;
}

int TEMPLE_ORI_fini(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	int i;
	void* pvRet;
	ThreadMgr* pstThread;

	assert(pstAppCtx);

	for(i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
			pstThread->cIsExit	=	1;
	}

	for(i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];

		if( pstThread->cIsValid )
		{
			channel2bus_one_recvall(pstAppCtx, pstEnv, pstThread);	
			pthread_join(pstThread->tid, &pvRet);
			channel2bus_one_recvall(pstAppCtx, pstEnv, pstThread);	
		}
	}

	for (i=0; i<pstEnv->iThread; i++)
	{
		pstThread	=	&pstEnv->astThread[i];
		tbus_close_channel(&pstThread->pstChannel0);
		tbus_close_channel(&pstThread->pstChannel1);
	}

	printf("TEMPLE_ORI stop\n");
	MutiLog(MUTI_LOGLEVEL_INFO, "TEMPLE_ORI stop");

	pthread_mutex_destroy(&pstEnv->stLogMutex);

	return 0;
}

int TEMPLE_ORI_tick(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	return 0;
}

int TEMPLE_ORI_stop(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
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
}

int main(int argc, char* argv[])
{
	int iRet;
	void* pvArg	=	&gs_stEnv;

	memset(&gs_stAppCtx, 0, sizeof(gs_stAppCtx));
	memset(&gs_stEnv, 0, sizeof(gs_stEnv));

	gs_stAppCtx.argc	=	argc;
	gs_stAppCtx.argv	=	argv;

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)TEMPLE_ORI_init;
	
	gs_stAppCtx.iLib = (unsigned long int)g_szMetalib_TEMPLE_ORI;
	gs_stAppCtx.stConfData.pszMetaName = "TEMPLE_ORIconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)TEMPLE_ORI_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)TEMPLE_ORI_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)TEMPLE_ORI_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)TEMPLE_ORI_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)TEMPLE_ORI_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


