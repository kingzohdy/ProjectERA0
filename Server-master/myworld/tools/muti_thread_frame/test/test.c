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
#include "test_conf_desc.h"
#include "version.h"
#include "tbus/tbus.h"

#define MAX_THREAD 256
#define LOCAL_BUS_ADDR_START 10
#define CHANNEL_SIZE 1024*256
#define MAX_MSG_SIZE 64000
#define CHANNEL_APPEND 128
#define MSG_LEN 1024
//char szWatchMsg[MAX_MSG_SIZE - sizeof()];

struct tagTestEnv
{
	TESTCONF *pstConf;
	TAPPCTX *pstAppCtx;
	int iRandomFd;
	char szSendBuf[MSG_LEN];
	char szRecvBuf[MSG_LEN];
};

typedef struct tagTestEnv		TESTENV;
typedef struct tagTestEnv		*LPTESTENV;

static TAPPCTX gs_stAppCtx;
extern unsigned char g_szMetalib_test[];

TESTENV gs_stEnv;



int test_init(TAPPCTX *pstAppCtx, TESTENV *pstEnv)
{	
	if (NULL == pstAppCtx->stConfData.pszBuff || 0 == pstAppCtx->stConfData.iMeta 
		|| NULL == pstAppCtx->pszConfFile )
	{
		printf( "initializing failed." );
		return -1;
	}
	pstEnv->pstConf = (TESTCONF *)pstAppCtx->stConfData.pszBuff;

	pstEnv->pstAppCtx = pstAppCtx;

	if( ( pstEnv->iRandomFd = open( "/dev/urandom", O_RDONLY ) ) == -1 )
	{
		printf( "open \"/dev/urandom\" failed." );
		return -1;
	}

	//recv bus left msg
	while(1)
	{
		int iSrc;
		int iDst;
		int iLen;

		iDst = pstAppCtx->iId;
		iSrc = TBUS_ADDR_ALL;
		iLen = sizeof(pstEnv->szRecvBuf);
		if (0 != tbus_recv(pstAppCtx->iBus, &iSrc, &iDst, pstEnv->szRecvBuf,(size_t*)&iLen, 0))
			break;
	}

	printf( "initializing ok.\n" );
	return 0;
}

int test_data_transfer(TAPPCTX *pstAppCtx, TESTENV *pstEnv)
{
	int fBusy;
	int iRet;
	int iSrc;
	int iDst;
	int iLen;

	fBusy = 0;
	iSrc = pstAppCtx->iId;
	iDst = 0;
	iLen = sizeof(pstEnv->szSendBuf);

	if( read( pstEnv->iRandomFd, pstEnv->szSendBuf, iLen ) != iLen )
	{
		printf( "generate random data failed!\n" );
		return 0;
	}

	iRet = tbus_send( pstAppCtx->iBus, &iSrc, &iDst, pstEnv->szSendBuf, iLen, 0 );
	if( iRet < 0 )
	{
		printf("send error.ret:%d\n", iRet );
		return 0;
	}

	printf("send : %d bytes.\n", iLen );

  usleep(100*1000);  

	fBusy = 1;
	iSrc = TBUS_ADDR_ALL;
	iDst = pstAppCtx->iId;
	iLen = sizeof(pstEnv->szRecvBuf);
	iRet = tbus_recv(pstAppCtx->iBus, &iSrc, &iDst, pstEnv->szRecvBuf, (size_t*)&iLen, 0);
	if( iRet == 0 )
	{
		printf( "recv : %d bytes. ", iLen );

		if( memcmp( pstEnv->szSendBuf, pstEnv->szRecvBuf, sizeof(pstEnv->szSendBuf) ) == 0 )
			printf("data checked ok.\n");
		else
			printf("data checked failed.\n");
	}
	return fBusy;
}

int test_proc(TAPPCTX *pstAppCtx, TESTENV *pstEnv)
{
	int fBusy=0;
	
	//如果进程还没不是退出状态，转发bus的消息到channel
	if (!tapp_is_exit())
	{
		fBusy = test_data_transfer( pstAppCtx, pstEnv );
	}

	if( !fBusy )
	{
		printf("finished.\n");
		exit(-1);
		return -1;	
	}
	
	return 0;
}

int test_reload(TAPPCTX *pstAppCtx, TESTENV *pstEnv)
{
	TESTCONF   *pstPreConf;
	
	pstPreConf = (TESTCONF   *)pstAppCtx->stConfPrepareData.pszBuff;
	
	printf("test reload\n");
	return 0;
}


int test_fini(TAPPCTX *pstAppCtx, TESTENV *pstEnv)
{
	close( pstEnv->iRandomFd );
	pstEnv->iRandomFd = -1;
	return 0;
}

int test_tick(TAPPCTX *pstAppCtx, TESTENV *pstEnv)
{
	return 0;
}

int test_stop(TAPPCTX *pstAppCtx, TESTENV *pstEnv)
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

	gs_stAppCtx.pfnInit	=	(PFNTAPPFUNC)test_init;
	
	gs_stAppCtx.iLib = (int)g_szMetalib_test;
	gs_stAppCtx.stConfData.pszMetaName = "testconf";
	gs_stAppCtx.uiVersion = TAPP_MAKE_VERSION(MAJOR, MINOR, REV, BUILD);
		
	iRet	=	tapp_def_init(&gs_stAppCtx, pvArg);
	if( iRet<0 )
	{
		printf("Error: app Initialization failed.\n");
		return iRet;
	}

	gs_stAppCtx.pfnFini	=	(PFNTAPPFUNC)test_fini;
	gs_stAppCtx.pfnProc	=	(PFNTAPPFUNC)test_proc;
	gs_stAppCtx.pfnTick	=	(PFNTAPPFUNC)test_tick;
	gs_stAppCtx.pfnReload	=	(PFNTAPPFUNC)test_reload;
	gs_stAppCtx.pfnStop  =   (PFNTAPPFUNC)test_stop;
	
	iRet	=	tapp_def_mainloop(&gs_stAppCtx, pvArg);

	tapp_def_fini(&gs_stAppCtx, pvArg);
	
	return iRet;
}


