/*
**  @file $RCSfile: zone_trans.c,v $
**  general description of this module
**  $Id: zone_trans.c,v 1.32 2013/08/07 01:25:28 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/08/07 01:25:28 $
**  @version $Revision: 1.32 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_trans.h"
#include "zone_clt.h"
#include "zone_player.h"
#include "zone_act_status.h"
#include "zone_map.h"
#include "zone_package.h"
#include "zone_attr.h"
#include "zone_oplog.h"
#include "zone_city.h"
#include "zone_team.h"
#include "zone_vip.h"
#include "zone_err.h"
#include "zone_combat.h"

extern int g_team_follow_flag;

MAPTRANSDEF *z_find_trans(ZONESVRENV* pstEnv, int iListIndex,  int *piIdx)
{
	int i;
	MAPTRANSDEF *pstTrans;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	if (piIdx && *piIdx >=0  && *piIdx < pstObj->iMapTransDef)
	{
		pstTrans = (MAPTRANSDEF *)pstObj->sMapTransDef[*piIdx];
		if ((int)pstTrans->ListIndex == iListIndex)
		{
			return pstTrans;
		}
	}


	for (i = 0; i < pstObj->iMapTransDef; i++)
	{
		pstTrans = (MAPTRANSDEF *)pstObj->sMapTransDef[i];
		if ((int)pstTrans->ListIndex == iListIndex)
		{
			return pstTrans;
		}
	}

	
	return NULL;
}


int player_trans_select_check(ZONESVRENV* pstEnv, Player *pstPlayer,CSTRANSSELECT *pstTransSelect , 
									int *piNeedMoney, RESPOS *pstPos, int *piMapID, int iTip)
{
	int i;
	MAPTRANSDEF *pstTransDef = NULL;
	int iNeedMoney = 0;
		
	pstTransSelect->TransIDNum = min(pstTransSelect->TransIDNum,MAX_TRANS);

	for (i = 0; i < pstTransSelect->TransIDNum; i++)
	{
		pstTransDef = z_find_trans(pstEnv, pstTransSelect->TransID[i],NULL);
		if (NULL == pstTransDef)
		{
			return -1;
		}

		if (piMapID && pstTransDef->FromMapID != *piMapID)
		{
			return -1;
		}

		//条件检查
		if (NULL == star_get_result(pstEnv, pstPlayer, STAR_RESULT_TRANS))
		{
			if (pstTransDef->TransCondition != TRANS_CONDITION_NONE)
			{
				switch (pstTransDef->TransCondition)
				{
				case TRANS_CONDITION_WORLD_CITY_OWNER:
					if (!is_world_city_owner(pstEnv,pstPlayer))
					{
						if (iTip)
							z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_TRANS101);
						return -1;
					}
					break;
				default:
					break;
				}
			}
		}

		if (pstPlayer->stRoleData.Level < pstTransDef->NeedLevel)
		{
			if (iTip)
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2,ZONEERR_TRANS102, pstTransDef->NeedLevel);
			return -1;
		}

		iNeedMoney += pstTransDef->Money;

		if (piMapID)
		{
			*piMapID = pstTransDef->ToMapID;
		}
	}

	if (piNeedMoney)
	{
		*piNeedMoney = iNeedMoney;
	}
	
	if (pstTransDef)
	{
		if (pstPos)
		{	
			pstPos->X = pstTransDef->X;
			pstPos->Y = pstTransDef->Y;
		}

		if (piMapID)
		{
			*piMapID = pstTransDef->ToMapID;
		}
	}
	
	return 0;
}

int player_trans_select(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSTRANSSELECT *pstTransSelect = &pstCsPkg->Body.TransSelect;
	Npc *pstNpc;
//	MAPTRANSDEF *pstTransDef = NULL;	
	MapIndex* pstMapIdx = NULL;
	int iMapPos = pstPlayer->stOnline.iMapPos;
	int iMapID = pstPlayer->stRoleData.Map;
	RESPOS stPos;
	int iRet;
//	int i = 0;
	int iNeedMoney = 0;
	int iToMapID = 0;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	int iPos;

	if (pstTransSelect->NpcID == 0)
	{
		//检查道具
		if (0 > package_get_item(pstEnv, pstPlayer, pstTransSelect->UseItem.SrcListType, 
							pstTransSelect->UseItem.SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
		{
			return -1;
		}

		if (NULL == z_item_get_result(pstItemDef, RESULT_ITEM_TRANS))
		{
			return -1;
		}

		if (0 > z_player_goto_check(pstEnv, pstPlayer, 0 , NULL))
		{
			return -1;
		}
	}
	else
	{
		pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstTransSelect->NpcID);
		if (NULL == pstNpc)
		{
			return -1;
		}

		if( check_npc_ui(pstEnv, pstPlayer,UI_TYPE_MAP_TRANS) < 0)
		{
			return -1;
		}
	}

	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_TRANS))
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &iMapPos, iMapID);

	if (!pstMapIdx)
	{
		return -1;
	}

	if (is_refer_line_map(pstMapIdx))
	{

		iMapID = z_find_map_byline(pstMapIdx,1);

		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &iMapPos, iMapID);

		if (!pstMapIdx)
		{
			return -1;
		}
	}

	stPos.X = 0;
	stPos.Y = 0;
	iToMapID = pstMapIdx->stMapDef.MapID;
	if ( player_trans_select_check(pstEnv, pstPlayer,pstTransSelect ,&iNeedMoney,&stPos, &iToMapID, 1) < 0)
	{
		return -1;
	}

	if (stPos.X <= 0 || stPos.Y <= 0 || iToMapID <= 0)
		return -1;

	//检查金钱
	if (NULL == star_get_result(pstEnv, pstPlayer, STAR_RESULT_TRANS))
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iNeedMoney))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOMONEY3);
			return -1;
		}
	}
	
	pstPlayer->stOnline.stTransSelect = *pstTransSelect;
	g_team_follow_flag = 1;

	iRet = z_player_change_map(pstAppCtx, pstEnv, pstPlayer, iToMapID, &stPos, 
					RAND1(180), 0);
	g_team_follow_flag = 0;

	if (0 > iRet)
	{
		return -1;
	}

	if (pstTransSelect->NpcID == 0)
	{
		ITEMDEC stDec;

		iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, pstTransSelect->UseItem.SrcListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_USE);	
		//扣道具
		if (0 <= iRet)
		{
			stDec.ID = pstItemDef->ItemID;
			stDec.Num = 1;
			stDec.BindNum = iRet;
			z_role_item_dec_oplog(pstEnv,pstPlayer, &stDec,OP_ITEM_DEC_USE);
		}
	}

	//扣钱
	if (NULL == star_get_result(pstEnv, pstPlayer, STAR_RESULT_TRANS))
	{
		iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iNeedMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_TRANS, 0, iNeedMoney-iRet, iRet, "传送消耗");
	}
	

	{
		OPMAPTRANSINFO stData;
		stData.FromMapID = iMapID;
		stData.ToMapID = iToMapID;

		if (NULL == star_get_result(pstEnv, pstPlayer, STAR_RESULT_TRANS))
		{
			stData.Money = iNeedMoney;
		}
		else
		{
			stData.Money = 0;
		}
			
		z_map_trans_oplog(pstEnv,pstPlayer, &stData);
	}
	
	return 0;
	UNUSED(pstFrameHead);
}

int player_trans_openlist(ZONESVRENV* pstEnv, Player *pstPlayer, Npc *pstNpc)
{
	//MAPTRANSDEF *pstMapTransDef = NULL;	//comment by paraunused.pl
	CSPKG stPkg;	//comment by paraunused.pl
	CSTRANSLIST *pstTransList = &stPkg.Body.TransList;	//comment by paraunused.pl
	int iMapID = pstPlayer->stRoleData.Map;
	MapIndex* pstMapIdx = NULL;
	int iMapPos = pstPlayer->stOnline.iMapPos;
	//int i = 0;	//comment by paraunused.pl
//	int iIdx = 0;

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstNpc->stMon.stMap.iID, &pstNpc->stMon.stCurrPos, OP_DIST + 100)) 
	{
		
		return -1;
	}

	if (0 == z_player_act_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, RES_ACTION_DIALOG))
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &iMapPos, iMapID);

	if (!pstMapIdx)
	{
		return -1;
	}

	if (is_refer_line_map(pstMapIdx))
	{

		iMapID = z_find_map_byline(pstMapIdx,1);

		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &iMapPos, iMapID);

		if (!pstMapIdx)
		{
			return -1;
		}
	}

	player_ui(pstEnv,pstPlayer,pstNpc,UI_TYPE_MAP_TRANS);



	memset(pstTransList,0,sizeof(*pstTransList));

	pstTransList->NpcID = pstNpc->stMon.iID;
	pstTransList->FromMapID = iMapID;
	pstTransList->Count = 0;	
/*
	for(i = 0; i < MAX_TRANS; i++)
	{
		if (pstTransList->Count >= pstMapIdx->iMapTransCount)
		{
			break;
		}
		
		iIdx = i + pstMapIdx->iMapTransStart;

		if (iIdx >= pstEnv->pstZoneObj->iMapTransDef)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "MapTransDef index error");
			break;
		}
		
		pstMapTransDef = (MAPTRANSDEF *)pstEnv->pstZoneObj->sMapTransDef[iIdx];
		
		pstTransList->TransList[pstTransList->Count].ToMapID = pstMapTransDef->ToMapID;
		pstTransList->TransList[pstTransList->Count].TransID = pstMapTransDef->ListIndex;
		pstTransList->TransList[pstTransList->Count].Money = pstMapTransDef->Money;
		pstTransList->TransList[pstTransList->Count].Condition = pstMapTransDef->TransCondition;

		pstTransList->Count++;
		
	}
*/
	Z_CSHEAD_INIT(&stPkg.Head, TRANS_LIST);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	


	return 0;
}


int player_trans_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iToMap, int iTip)
{
	MAPTRANSDEF *pstTrans;
	int i;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	MapIndex *pstMapIdx;
	int iMap = iToMap;
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, NULL, iToMap);
	if(NULL == pstMapIdx)
	{
		return -1;
	}

	if (is_refer_line_map(pstMapIdx))
	{
		iMap = pstMapIdx->stMapDef.ReferMapID;	
	}
	
	
	for (i = 0; i < pstObj->iMapTransDef; i++)
	{
		pstTrans = (MAPTRANSDEF *)pstObj->sMapTransDef[i];
		if (pstTrans->ToMapID == iMap)
		{
			if (pstPlayer->stRoleData.Level < pstTrans->NeedLevel)
			{
				if (iTip)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_TRANS103	, pstTrans->NeedLevel);
				}	
				return -1;
			}
			break;
		}
	}
	return 0;
}

