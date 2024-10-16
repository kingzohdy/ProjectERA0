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

struct tagTEMPLE_ORIEnv
{
	TEMPLE_BIGCONF *pstConf;
	TEMPLE_BIGRUN_STATUS   *pstRunStat;
	TLOGCATEGORYINST* pstLogCat;
};

typedef struct tagTEMPLE_ORIEnv		TEMPLE_BIGENV;
typedef struct tagTEMPLE_ORIEnv		*LTEMPLE_BIGENV;


static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_TEMPLE_ORI[];

TEMPLE_BIGENV gs_stEnv;
static TEMPLE_BIGRUN_STATUS gs_stStat;

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

	if (0 > tapp_get_category(NULL, (void *)(&pstEnv->pstLogCat)))	
	{		
		printf("tapp_get_category run fail\n");
		return -1;	
	}	
	
	printf("TEMPLE_ORI start\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "TEMPLE_ORI start");

	return 0;
}

int TEMPLE_ORI_proc(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	int iBusy=0;
	
	if (iBusy)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int TEMPLE_ORI_reload(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{
	TEMPLE_BIGCONF   *pstPreConf;
	
	pstPreConf = (TEMPLE_BIGCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("TEMPLE_ORI reload\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "TEMPLE_ORI reload");

	return 0;
}

int TEMPLE_ORI_fini(TAPPCTX *pstAppCtx, TEMPLE_BIGENV *pstEnv)
{

	printf("TEMPLE_ORI finish\n");
	tlog_info(pstEnv->pstLogCat, 0, 0, "TEMPLE_ORI finish");
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


