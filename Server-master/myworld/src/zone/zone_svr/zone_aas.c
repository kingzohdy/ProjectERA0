/*
**  @file $RCSfile: zone_aas.c,v $
**  general description of this module
**  $Id: zone_aas.c,v 1.9 2013/01/12 09:40:46 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/01/12 09:40:46 $
**  @version $Revision: 1.9 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_clt.h"
#include "zone_player.h"
#include "comm_misc.h"

int GetToday(TAPPCTX* pstAppCtx)
{
    struct tm *ptmNow;
    time_t time_tNow = pstAppCtx->stCurr.tv_sec;
    char stoday[20];
    
    ptmNow = localtime( &time_tNow );
    strftime( stoday, 20, "%Y%m%d", ptmNow);
    return atoi(stoday);
}

int z_player_birthday_check(TAPPCTX* pstAppCtx, char *pstBirthDay)
{
	if (!pstBirthDay)
	{
		return 2;
	}

	
	if (strlen(pstBirthDay) != 8)
	{
		return 2;
	}

	{
		int iToDay = GetToday(pstAppCtx);
		int iBirth = atoi(pstBirthDay);
		
		if (iBirth +180000 > iToDay)
		{
			return 1;
		}
	}
	
	return 0;
}

//通知客户端身份补填成功
int player_add_id_card_succ(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSAASSVR *pstAasSvr = &stPkg.Body.AasSvr;

	//取消防沉迷
	pstPlayer->bAasFlag= 0;
	pstPlayer->bAasIncomeLimt= 0;
	
	pstAasSvr->Type = AAS_SVR_SUCC;
	Z_CSHEAD_INIT(&stPkg.Head, AAS_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//客户端弹出身份证补填
int player_aas_id_card_notify(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSAASSVR *pstAasSvr = &stPkg.Body.AasSvr;
	
	if(pstPlayer->bAasFlag != 2)
	{
		return 0;
	}

	pstAasSvr->Type = AAS_SVR_UI;
	Z_CSHEAD_INIT(&stPkg.Head, AAS_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

//登录防沉迷检查
int player_acc_login_aas_check(ZONESVRENV* pstEnv, Player *pstPlayer, char *pstBirthDay)
{
	pstPlayer->bAasFlag= 0;
	pstPlayer->bAasIncomeLimt= 0;

	if (pstEnv->pstConf->NoAas)
	{
		return 0;
	}

	pstPlayer->bAasFlag = z_player_birthday_check(pstEnv->pstAppCtx, pstBirthDay);
	if(pstPlayer->bAasFlag > 0)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "account %s birthday %s ", 
			pstPlayer->szAccount, pstBirthDay);
		
		memset(&pstPlayer->stAasTime, 0, sizeof(pstPlayer->stAasTime));
		pstPlayer->stAasTime.tActive = pstEnv->pstAppCtx->stCurr.tv_sec;
		
		send_to_aas(pstEnv, pstPlayer->stRoleData.Uin, AAS_GAME_START);
	}

	return 0;
}

int aas_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{
	CSAASCLT *pstAasClt = &pstCsPkg->Body.AasClt;

	switch(pstAasClt->Type)
	{
		//补填身份证
		case AAS_CLT_ADD_ID_INFO:
			z_ss_fill_id_card_req(pstEnv,pstPlayer,&pstAasClt->Data.IDInfo);
			break;
		default:
			break;
	}
	return 0;
}
