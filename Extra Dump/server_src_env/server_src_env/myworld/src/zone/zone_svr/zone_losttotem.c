

#include "zone_losttotem.h"
#include "zone_player.h"
#include "zone_map.h"
#include "zone_npc.h"
#include "zone_package.h"
#include "zone_attr.h"
#include "zone_move.h"
#include "zone_arm.h"
#include "zone_syncrelation.h"
#include "zone_clt.h"
#include "zone_ronglu.h"
#include "zone_err.h"

int player_is_in_lostmap(Player *pstPlayer)
{
	int iMap = GET_MAPID(pstPlayer->stRoleData.Map);

	if (iMap == 3211 || iMap == 3212 || iMap == 3213 || iMap == 3214 || iMap == 3215 || iMap==3216)
	{
		return 1;
	}
	return 0;
}

int losttotem_use_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, ITEMDEF *pstItemDef)
{
	tdr_longlong ullExp = 0;
	LOSTTOTEMINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.LostTotemInfo;
//	Monster *pstMon;
	// 经验满
	if (pstRoleItem->InstVal2 >= pstRoleItem->InstVal1)
	{
		ullExp = pstRoleItem->InstVal1;
		z_player_addexp_test(pstEnv, pstPlayer, &ullExp);
		if(0 >= ullExp)
		{
			return -1;
		}

		if (pstInfo->Num >= 5)
		{
			z_send_sysmsg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_LOSTTOTEM_301);
			return -1;
		}
	}
	else
	{
		if (!player_is_in_lostmap(pstPlayer))
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_LOSTTOTEM_302);
			return -1;
		}	

		

		/*pstMon = z_id_monster(pstEnv->pstAppCtx,  pstEnv, pstInfo->MonID);
		if (pstMon && strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName) == 0)
		{
			return -1;
		}*/
	}
	return 0;
}

Monster* losttotem_new_mon(ZONESVRENV* pstEnv, Player *pstPlayer,  int iMonID, int iLeftTime, int iUpLimit, int iCurr)
{
//	Monster *pstMon; 
	Monster stMonster;
	MONSTERDEF *pstMonDef;
	Monster *pstFind;
	RESPOS stPos;

	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	MapIndex *pstMapIdx;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
	{
		return NULL;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		return NULL;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.tRefresh	=	pstEnv->pstAppCtx->stCurr.tv_sec;

	// 这里用的memid，登录记得恢复一下
	stMonster.iOwnerID = pstPlayer->iMemID;
	mon_set_pkinfo(&stMonster, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);
	snprintf(stMonster.szOwner,sizeof(stMonster.szOwner),"%s",pstPlayer->stRoleData.RoleName);
	stMonster.szOwner[sizeof(stMonster.szOwner) - 1] = 0;
	
	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_LOST_TOTEM)
	{
		return NULL;
	}
	
	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 400, 400, &stPos);

	stMonster.bDirection = RAND1(180);
	stMonster.stInitPos = stPos;
	pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
	if( !pstFind )
		return NULL;

	pstFind->iCommType = MON_UNION_TYPE_LOSTTOTEM;
	pstFind->stCommUnion.stLostTotem.iPower = iCurr;
	pstFind->stCommUnion.stLostTotem.iMaxPower = iUpLimit;

	pstFind->cFirstBorn = 1;
	if(iLeftTime > 0)
	{
		pstFind->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + iLeftTime;
	}
	if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind, NULL))
	{
		tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
		return NULL;
	}
	return pstFind;
}

int losttotem_use(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEITEM *pstRoleItem, ITEMDEF *pstItemDef)
{
	LOSTTOTEMINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.LostTotemInfo;
	Monster *pstMon; 
	
	if (pstRoleItem->InstVal2 >= pstRoleItem->InstVal1)
	{
		player_add_exp(pstEnv, pstPlayer, pstRoleItem->InstVal1, 0);

		// 删除todo  ,此物品填了使用不消失
		package_dec(pstEnv->pstAppCtx,pstEnv,pstPlayer,pstRoleItem->DefID, 
						1, LIST_TYPE_SUNDRIES, &pstRoleItem->DefPos,PACKAGE_DEC_UNBIND, 0);

		pstInfo->Num++;
	}
	else
	{
		Monster *pstMon1;
		pstMon1 = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstInfo->MonID);
		if (pstMon1 && strcmp(pstMon1->szOwner, pstPlayer->stRoleData.RoleName) == 0)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon1);
			z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon1);

			pstInfo->ItemWid = 0;
			pstInfo->MonID = 0;
		}

		// 召唤totem
		pstMon = losttotem_new_mon(pstEnv,pstPlayer, pstItemDef->Result[0].ResultVal3, 
									pstItemDef->Result[0].ResultVal1, pstItemDef->Result[0].ResultVal2, 
									pstRoleItem->InstVal2);
		if (!pstMon)
			return -1;
	
		pstInfo->ItemWid = pstRoleItem->WID;
		pstInfo->MonID = pstMon->iID;

		player_losttotem_active_notify(pstEnv, pstPlayer, 1, pstInfo->ItemWid);
	}
	
	return 0;
}

Monster* player_find_lost_totem(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	LOSTTOTEMINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.LostTotemInfo;
	Monster *pstMon;
	
	if (!player_is_in_lostmap(pstPlayer))
		return NULL;

	if (pstInfo->MonID <= 0 ||pstInfo->ItemWid <= 0)
	{
		return NULL;
	}
	
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstInfo->MonID);
	if (!pstMon || strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName) )
	{
		return NULL;
	}

	return pstMon;
}

ROLEITEM *player_find_item_by_totem(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	LOSTTOTEMINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.LostTotemInfo;
	int i;
	SUNDRIESPACKAGE *pstSundPkg = &pstPlayer->stRoleData.Package.SundriesPackage;

	if (!player_find_lost_totem( pstEnv,pstPlayer))
	{
		return NULL;
	}

	for(i=0; i<pstSundPkg->Num; i++)
	{
		if (pstSundPkg->RoleGrids[i].GridData.RoleItem.WID == pstInfo->ItemWid)
		{
			return &pstSundPkg->RoleGrids[i].GridData.RoleItem;
		}
	}
	return NULL;
}

int losttotem_free(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	Monster *pstMon;
	LOSTTOTEMINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.LostTotemInfo;

	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstInfo->MonID);
	if (!pstMon || strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName))
	{
		return 0;
	}

	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
	z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);

	pstInfo->ItemWid = 0;
	pstInfo->MonID = 0;
	return 0;
}

int losttotem_leave_map(ZONESVRENV* pstEnv, Monster *pstMon)
{
	Player *pstPlayer;
	if (pstMon->bSubType != MON_SUB_TYPE_LOST_TOTEM)
	{
		return 0;
	}

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv,pstMon->szOwner);
	if (!pstPlayer)
		return 0;

	if (pstPlayer->stRoleData.MiscInfo.LostTotemInfo.MonID == pstMon->iID)
	{
		player_losttotem_active_notify(pstEnv, pstPlayer, 0, pstPlayer->stRoleData.MiscInfo.LostTotemInfo.ItemWid);	
	}

	return 0;
}

int lostmap_mon_die(ZONESVRENV* pstEnv,Player *pstPlayer, MONSTERDEF *pstMonDef, Monster *pstMon)
{
	ROLEITEM *pstRoleItem;
	ITEMDEF *pstItemDef;
	Monster *pstLost;

	pstLost = player_find_lost_totem( pstEnv,pstPlayer);
	if (!pstLost)
	{
		return 0;
	}
	
	if (z_distance(&pstMon->stCurrPos, &pstLost->stCurrPos) > 4000 || 
		pstMon->stMap.iID != pstLost->stMap.iID)
	{
		return 0;
	}

	pstRoleItem = player_find_item_by_totem(pstEnv,pstPlayer);
	if (!pstRoleItem)
		return 0;

	if (pstRoleItem->InstVal2 >= pstRoleItem->InstVal1)
	{
		return 0;
	}

	pstRoleItem->InstVal2 += pstMonDef->Booty.Exp*5.0f;
	if (pstRoleItem->InstVal2 > pstRoleItem->InstVal1)
	{
		pstRoleItem->InstVal2 = pstRoleItem->InstVal1;
		pstRoleItem->InstFlag  &= ~INST_ITEM_BIND;
	}

	pstLost->stCommUnion.stLostTotem.iPower = pstRoleItem->InstVal2;

	// 道具变化
	item_one_grid_update_ex(pstEnv,  pstPlayer, pstRoleItem, LIST_TYPE_SUNDRIES);

	// 借用熔炉协议通知
	ronglu_killnum_chg(pstEnv, pstPlayer, pstRoleItem->InstVal2, pstLost, pstMon, 1);

	if (pstRoleItem->InstVal2 == pstRoleItem->InstVal1)
	{
		pstItemDef = z_find_itemdef(pstEnv,pstRoleItem->DefID);
		if (pstItemDef)
		{
			player_add_school_val( pstEnv, pstPlayer, pstItemDef->Result[0].ResultVal4);
		}
		losttotem_free(pstEnv,pstPlayer);
	}
	return 0;
}


int player_losttotem_active_notify(ZONESVRENV* pstEnv, Player* pstPlayer, int iActive, tdr_ulonglong ullWid)
{
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstRes = &stPkg.Body.PlayerMiscSvr;
	LOSTTOTEMACTIVE *pstActive = &pstRes->PlayerMiscSvrData.TotemActive;
	pstRes->PlayerMiscType = PLAYER_TOTEM_ACTIVE;

	pstActive->Active = iActive;
	pstActive->WID = ullWid;

	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int losttotem_player_login_check(ZONESVRENV *pstEnv, Player *pstPlayer)
{	
	Monster *pstMonster; 
	int iID = pstPlayer->stRoleData.MiscInfo.LostTotemInfo.MonID;

	if (iID <= 0) return 0;
	
	pstMonster = z_id_monster(pstEnv->pstAppCtx,pstEnv, iID);
	if (	!pstMonster || 
		strcmp(pstMonster->szOwner, pstPlayer->stRoleData.RoleName))
	{
		pstPlayer->stRoleData.MiscInfo.LostTotemInfo.MonID = 0;
		pstPlayer->stRoleData.MiscInfo.LostTotemInfo.ItemWid = 0;
		return 0;
	}
	
	mon_set_pkinfo(pstMonster, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);
	return 0;
}

int losttotem_pkg_move_limit( ZONESVRENV *pstEnv, Player *pstPlayer, int iSrcList, int iDstList, ROLEITEM *pstRoleItem)
{
	LOSTTOTEMINFO *pstInfo = &pstPlayer->stRoleData.MiscInfo.LostTotemInfo;
	Monster *pstMon;
	
	if (pstRoleItem->WID != pstInfo->ItemWid)
	{
		return 0;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstInfo->MonID);
	if (!pstMon || strcmp(pstMon->szOwner, pstPlayer->stRoleData.RoleName))
	{
		return 0;
	}

	if (!(pstRoleItem->InstFlag & INST_ITEM_BIND))
	{
		return 0;
	}

	// 从杂物包丢弃 或者放到仓库
	if ((iDstList == -1 || iDstList == LIST_TYPE_STASH) && iSrcList == LIST_TYPE_SUNDRIES)
	{
		return 1;
	}
	return 0;
}
