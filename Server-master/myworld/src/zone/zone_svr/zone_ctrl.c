/*
**  @file $RCSfile: zone_ctrl.c,v $
**  general description of this module
**  $Id: zone_ctrl.c,v 1.6 2013/01/12 07:50:10 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/01/12 07:50:10 $
**  @version $Revision: 1.6 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_ctrl.h"
#include "zone_script.h"

int z_ctrl_msg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, char *pszMsg, int iMsgLen)
{
	int iSrc;
	int iDst;
	SSZONECTRL *pstZoneCtrlReq;
	SSZONECTRL *pstZoneCtrlRes;
	TDRDATA stNet;
	SSPKG *pstSSRes = &pstEnv->stSSRes;
	int iRet;

	if (0 > ss_unpack(pstEnv->pstSSHan, pszMsg, iMsgLen, &pstEnv->stSSReq, 0))
	{
		return -1;
	}
			
	tlog_debug_dr(pstEnv->pstLogCat, 0, 0, pstEnv->pstSSHan->pstSSProtoPkgMeta, 
					&pstEnv->stSSReq, sizeof(pstEnv->stSSReq), 0);

	if (ZONE_CTRL != pstEnv->stSSReq.Head.Cmd)
	{
		return -1;
	}

	pstZoneCtrlReq = &pstEnv->stSSReq.Body.ZoneCtrl;

	tlog_info(pstEnv->pstLogCat, 0, 0, "recv zone ctrl cmd=%d", pstZoneCtrlReq->Cmd);

	Z_SSHEAD_INIT(&pstSSRes->Head, ZONE_CTRL);
	pstZoneCtrlRes = &pstSSRes->Body.ZoneCtrl;
	switch(pstZoneCtrlReq->Cmd)
	{
	case TEST_REQ:
		pstZoneCtrlRes->Cmd = TEST_RES;
		pstZoneCtrlRes->CtrlCmd.TestRes.Result = 0;
		break;
	case SHUTDOWN_REQ:
		pstZoneCtrlRes->Cmd = SHUTDOWN_RES;
		pstZoneCtrlRes->CtrlCmd.ShutDownRes.Result = 0;
		pstEnv->cShutdown = 1; // 转入关闭服务器第1阶段
		pstEnv->cClearShm = 1;
		pstEnv->ucShutdownCount = pstZoneCtrlReq->CtrlCmd.ShutDownReq.Time/SHUTDOWN_NOTIFY_TIMER;
		pstEnv->tShutdownStart = pstAppCtx->stCurr.tv_sec;
		tos_send_signal( pstAppCtx->pszPidFile, pstAppCtx->pszApp, SIGUSR1, NULL );
		break;
	case LOADSCRIPT_REQ:
		if (pstZoneCtrlReq->CtrlCmd.LoadScriptReq.Type == LOADSCRIPT_ONE)
		{
			iRet = load_one_script(pstEnv, pstZoneCtrlRes->CtrlCmd.LoadScriptReq.Name);
		}
		else
		{
			iRet = load_script_dir(pstEnv);
		}
		pstZoneCtrlRes->Cmd = LOADSCRIPT_RES;
		pstZoneCtrlRes->CtrlCmd.LoadScriptRes.Result = iRet;
		break;
	case RELOAD_REQ:
		iRet = z_reload_self();
		pstZoneCtrlRes->Cmd = RELOAD_RES;
		pstZoneCtrlRes->CtrlCmd.ReLoadRes.Result = iRet;
		break;
	default:
		break;
	}

	stNet.pszBuff = pstEnv->szSendBuffer;
	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	if (0 > ss_pack(pstEnv->pstSSHan, &stNet, pstSSRes, 0))
	{
		return 0;
	}

	iSrc = pstAppCtx->iId;
	iDst = 0;
	tbus_backward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0);

	return 0;
}

