/*
**  @file $RCSfile: zone_apex.c,v $
**  general description of this module
**  $Id: zone_apex.c,v 1.12 2013/06/18 02:29:46 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/06/18 02:29:46 $
**  @version $Revision: 1.12 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_apex.h"
#include "zone_clt.h"
#include "zone_player.h"
#include "zone_zh_name.h"

int apex_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	SSPKG stSSPkg;
	SSAPEX *pstSSApex = &stSSPkg.Body.Apex;
	APEXDATA *pstApexData = &pstCsPkg->Body.ApexClt;

	if (NULL == pstPlayer )
	{
		return -1;
	}

	if (pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
	}
	else
	{
		if (0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_APEXON))
		{
			return -1;
		}
			
		pstSSApex->Uin = pstPlayer->stRoleData.Uin;
		pstSSApex->ApexMsgType = APEX_MSG_DATA;
		pstSSApex->ApexMsg.ApexData.DataLen = pstApexData->DataLen;
		if (pstSSApex->ApexMsg.ApexData.DataLen > MAX_APEX_DATA ||
			pstSSApex->ApexMsg.ApexData.DataLen <= 0)
		{
			return 0;
		}
		
		memcpy(pstSSApex->ApexMsg.ApexData.Data, pstApexData->Data, pstApexData->DataLen);

		Z_SSHEAD_INIT(&stSSPkg.Head, SS_APEX);
		z_ssmsg_send(pstEnv, &stSSPkg, pstEnv->iMudSvrID);	
	}
		
	return 0;
}

int z_ss_apex_get_res(ZONESVRENV* pstEnv, SSPKG *pstPkg)
{
	CSPKG stCSPkg;
	APEXDATA *pstApexSvr = &stCSPkg.Body.ApexSvr;
	SSAPEX* pstRes = &pstPkg->Body.Apex;
	Player* pstPlayer = player_get_by_uin(pstEnv, pstRes->Uin);

	if (NULL == pstPlayer)
	{
		return 0;
	}
	
	switch(pstRes->ApexMsgType)
	{
		case APEX_MSG_DATA:
			if (0 == (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_APEXON))
			{
				return -1;
			}
			
			pstApexSvr->DataLen = pstRes->ApexMsg.ApexData.DataLen;
			if (pstApexSvr->DataLen <= 0 || pstApexSvr->DataLen > MAX_APEX_DATA)
			{
				return -1;
			}

			memcpy(pstApexSvr->Data, pstRes->ApexMsg.ApexData.Data, pstApexSvr->DataLen);

			Z_CSHEAD_INIT(&stCSPkg.Head, APEX_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stCSPkg);
			break;
		case APEX_MSG_KICK:
			tlog_info(pstEnv->pstLogCat, 0, 0, "player uin %d apex kick", pstPlayer->stRoleData.Uin);
			pstPlayer->stRoleData.BlockEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 10;
			pstPlayer->stRoleData.BlockStart = pstEnv->pstAppCtx->stCurr.tv_sec;

			player_delay_kickoff(pstEnv->pstAppCtx, pstEnv, pstPlayer, LJY_KICK1);
			break;
		default:
			break;
	}
	
	return 0;
}

