/*
**  @file $RCSfile: zone_vip.c,v $
**  general description of this module
**  $Id: zone_vip.c,v 1.23 2013/05/16 02:01:26 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/05/16 02:01:26 $
**  @version $Revision: 1.23 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#include "zone_vip.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_package.h"
#include "zone_player.h"
#include "zone_oplog.h"
#include "zone_shop.h"
#include "zone_move.h"
#include "zone_err.h"

STARSTONEDEF *z_find_stonevip_def(ZONESVRENV *pstEnv, int ID)
{
	return (STARSTONEDEF *)z_find_def(pstEnv->pstZoneObj->sStarStoneDef, ID, MAX_STARSTONE_DEF_DATA, 
									pstEnv->pstZoneObj->iStarStoneDef , NULL);
}

STARFUNCDEF *z_find_starfunc_def(ZONESVRENV *pstEnv, int ID)
{
	return (STARFUNCDEF *)z_find_def(pstEnv->pstZoneObj->sStarFuncDef, ID, MAX_STARFUNC_DEF_DATA, 
									pstEnv->pstZoneObj->iStarFuncDef , NULL);
}

//开钻石
int star_stone_open(ZONESVRENV *pstEnv, VIPDATA *pstVipData,int iID,int iTime, int iGold)
{
	int i = 0;
	unsigned int uiTmp = 0;
	
	for(i=0;i<pstVipData->StoneNum;i++)
	{
		if( iID == pstVipData->StarStoneInfo[i].ID)
		{
			uiTmp = pstVipData->StarStoneInfo[i].EndTime + iTime;
			if( uiTmp >= MAX_END_TIME)
			{
				return -1;
			}
			pstVipData->StarStoneInfo[i].EndTime += iTime;

			pstVipData->CashGodCoin += iGold;
			return 0;
		}
	}
	
	if(pstVipData->StoneNum >= MAX_STONE_NUM)
	{
		return -1;
	}

	pstVipData->StarStoneInfo[pstVipData->StoneNum].ID = iID;
	pstVipData->StarStoneInfo[pstVipData->StoneNum].EndTime = pstEnv->pstAppCtx->stCurr.tv_sec+iTime;
	pstVipData->StoneNum++;
	pstVipData->CashGodCoin += iGold;

	return 0;
}

int star_stardata_action(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_VIP_INFO;
	pstActionRes->Data.VipInfo.VipFlag = pstPlayer->stRoleData.MiscInfo.VipData.VipFlag;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int star_stardata_chg(ZONESVRENV *pstEnv, Player *pstPlayer, int iAction)
{
	CSPKG stPkgRes;
	CSSTARDATARES *pstStarDataRes = &stPkgRes.Body.StarDataSvr.StarDataSvr.StarDataRes;
	VIPDATA *pstVipData = &pstPlayer->stRoleData.MiscInfo.VipData;

	stPkgRes.Body.StarDataSvr.Type = STARDATA_SYNC_RES;
	pstVipData->VipFlag = star_get_vipflag(pstEnv, pstPlayer);
	pstStarDataRes->VipData = *pstVipData;

	Z_CSHEAD_INIT(&stPkgRes.Head, STARDATA_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

	if (iAction)
	{
		star_stardata_action(pstEnv, pstPlayer);
	}
	
	return 0;
}

//核实到期的钻
int star_stone_check_endtime(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int iChg = 0;
	int i = 0;
	VIPDATA *pstVipData = &pstPlayer->stRoleData.MiscInfo.VipData;

	if(pstVipData->StoneNum<=0)
	{
		return -1;
	}
	for(i=pstVipData->StoneNum-1;i>=0;i--)
	{
		if(pstEnv->pstAppCtx->stCurr.tv_sec > pstVipData->StarStoneInfo[i].EndTime)
		{
			iChg++;
			//到期了
			pstVipData->StoneNum--;
			if (i != pstVipData->StoneNum)
			{
				memmove(&pstVipData->StarStoneInfo[i], &pstVipData->StarStoneInfo[i+1], 
							(pstVipData->StoneNum - i) * sizeof(pstVipData->StarStoneInfo[0]));
			}
		}
	}
	
	if(iChg)
	{
		star_stardata_chg(pstEnv, pstPlayer, 1);
	}

	return 0;
}

int star_get_vipflag(ZONESVRENV *pstEnv, Player *pstPlayer)
{
	int i;
	int iVipFlag = 0;
	VIPDATA *pstVipData = &pstPlayer->stRoleData.MiscInfo.VipData;

	for (i=0; i<pstVipData->StoneNum; i++)
	{
		iVipFlag += (int)pow(2.0, pstVipData->StarStoneInfo[i].ID);
	}

	return iVipFlag;
}

int star_stone_open_req(ZONESVRENV *pstEnv, Player *pstPlayer,CSSTARSTONEOPENREQ *pstStarStoneOpenReq)
{
	STARSTONEDEF *pstStarStoneDef;
	CSPKG stPkgRes;
	CSSTARSTONEOPENRES *pstStarStoneOpenRes = &stPkgRes.Body.StarDataSvr.StarDataSvr.StarStoneOpenRes;
	int iTime = 0;
	int iGodCoin=0;
	VIPDATA *pstVipData = &pstPlayer->stRoleData.MiscInfo.VipData;

	memset(pstStarStoneOpenRes,0,sizeof(CSSTARSTONEOPENRES));

	stPkgRes.Body.StarDataSvr.Type = STARDATA_OPEN_STONE_RES;
	pstStarStoneDef = z_find_stonevip_def(pstEnv, pstStarStoneOpenReq->StarStoneID);
	if(!pstStarStoneDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_VIP1);
		pstStarStoneOpenRes->Result = 1;
		goto error;
	}

	if(pstStarStoneOpenReq->OpenTimeType == 1)
	{
		iGodCoin = pstStarStoneDef->OpenGodCoin[0];
		iTime = 7 * 24 * 60 * 60;
	}
	else if(pstStarStoneOpenReq->OpenTimeType == 2)
	{
		iGodCoin = pstStarStoneDef->OpenGodCoin[1];
		iTime = 30 * 24 * 60 * 60;
	}
	else if(pstStarStoneOpenReq->OpenTimeType == 3)
	{
		iGodCoin = pstStarStoneDef->OpenGodCoin[2];
		iTime = 180 * 24 * 60 * 60;
	}
	else
	{
		return -1;
	}

	if (0 >= iGodCoin)
	{
		return -1;
	}

	/* 金币检查 */
	if(package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, iGodCoin ) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_VIP2, MONEY_TYPE_GODCOIN_NAME);
		pstStarStoneOpenRes->Result = 1;
		goto error;
	}

	if( 0 > star_stone_open(pstEnv, &pstPlayer->stRoleData.MiscInfo.VipData,pstStarStoneDef->ID,iTime, iGodCoin))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_VIP3,pstStarStoneDef->Name);
		pstStarStoneOpenRes->Result = 1;
		goto error;
	}

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN,iGodCoin);
	godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_STAR,pstStarStoneDef->ID,0,0,
					iGodCoin,"开通特权徽章");
	pstStarStoneOpenRes->Result = 0;

	pstVipData->VipFlag = star_get_vipflag(pstEnv, pstPlayer);

	star_stardata_action(pstEnv, pstPlayer);	
	
error:
	pstStarStoneOpenRes->StarStoneID = pstStarStoneOpenReq->StarStoneID;
	pstStarStoneOpenRes->VipData = pstPlayer->stRoleData.MiscInfo.VipData;
	Z_CSHEAD_INIT(&stPkgRes.Head, STARDATA_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	
	return -1;
}

int player_star_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSSTARDATACLT *pstStarDataClt= &pstCsPkg->Body.StarDataClt;
	UNUSED(pstFrameHead);	
	UNUSED(pstAppCtx);	
	
	switch(pstStarDataClt->Type)
	{
		case STARDATA_OPEN_STONE_REQ:
			star_stone_open_req( pstEnv,pstPlayer,&pstStarDataClt->StarDataClt.StarStoneOpenReq);
			break;
		default:
			break;
	}
	
	return 0; 
}

int star_check_item_result(ZONESVRENV *pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	STARSTONEDEF *pstStarStoneDef;
	VIPDATA stVipData;

	stVipData = pstPlayer->stRoleData.MiscInfo.VipData;

	pstStarStoneDef = z_find_stonevip_def(pstEnv, pstResult->ResultVal1);
	if (NULL == pstStarStoneDef)
	{
		return -1;
	}

	if (0 > star_stone_open(pstEnv, &stVipData, pstResult->ResultVal1, pstResult->ResultVal2*3600, 0))
	{
		return -1;
	}
	
	return 0;
}

int star_item_result(ZONESVRENV *pstEnv, Player *pstPlayer, RESULT *pstResult)
{
	star_stone_open(pstEnv, &pstPlayer->stRoleData.MiscInfo.VipData, pstResult->ResultVal1, pstResult->ResultVal2*3600, 0);

	star_stardata_chg(pstEnv, pstPlayer, 1);
	
	return 0;
}

RESULT *star_get_result(ZONESVRENV *pstEnv, Player *pstPlayer, int iResultID)
{
	int i,j;
	STARSTONEDEF *pstStarStoneDef;
	STARFUNCDEF *pstStarFuncDef;
	VIPDATA *pstVipData = &pstPlayer->stRoleData.MiscInfo.VipData;

	for (i=0; i<pstVipData->StoneNum; i++)
	{
		pstStarStoneDef = z_find_stonevip_def(pstEnv, pstVipData->StarStoneInfo[i].ID);
		if (NULL == pstStarStoneDef)
		{
			continue;
		}

		for (j=0; j<MAX_STONE_FUNC; j++)
		{
			if (pstStarStoneDef->StarFuncList[j] == 0)
			{
				break;
			}

			pstStarFuncDef = z_find_starfunc_def(pstEnv, pstStarStoneDef->StarFuncList[j]);
			if (pstStarFuncDef && pstStarFuncDef->StarResult.ResultID == iResultID)
			{
				return &pstStarFuncDef->StarResult;
			}
		}
	}

	return NULL;
}

