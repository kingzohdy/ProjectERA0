

#include "zone_strongpoint.h"
#include "zone_clt.h"
#include "zone_clan.h"
#include "zone_map.h"
#include "zone_city.h"
#include "zone_attr.h"
#include "zone_status.h"
#include "zone_mail.h"
#include "zone_map.h"
#include "zone_oplog.h"
#include "zone_secondaryskill.h"
#include "libdirty.h"
#include "zone_err.h"
#include "zone_range.h"
#include "zone_unreal.h"
#include "zone_package.h"
#include "zone_machine.h"
#include "zone_combat.h"
#include "zone_clanboon.h"
#include "zone_err.h"

extern int g_iOptype;
extern int g_iOpsubtype;

#define SELECT_HOUR 12
#define FIGHT_HOUR 20
#define FIGHT_MIN 30

#define RANGE_TOP_NUM 300
#define STRONG_GLORY_TIME 10*60

static int g_iWeek[MAX_STRONG_POINT_WEEK]={2,4};

int strong_clt_enter_check(ZONESVRENV* pstEnv, Player *pstPlayer);
int strong_owner_chg(ZONESVRENV* pstEnv,STRONGPOINTONE *pstOne, int iOwnerID);
int strong_glory_close(ZONESVRENV* pstEnv, STRONGPOINTONE *pstOne, STRONGPOINTDEF *pstDef);

int strong_banner_reset(ZONESVRENV* pstEnv,  STRONGPOINTONE*pstOne);
static int issameday(time_t t1, time_t t2)
{
	struct tm curr1;
	struct tm curr2;
	
	localtime_r(&t1, &curr1);
	localtime_r(&t2, &curr2);

	if( curr1.tm_year==curr2.tm_year && curr1.tm_yday==curr2.tm_yday )
		return 1;

	return 0;
}

int strong_glory_chg(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;
	Npc *pstNpc;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;
	STRONGPOINTONE* pstOne;

	pstStrongSvr->Cmd = STRONG_SVR_OP_STRONGLORY;
	pstStrongSvr->Data.StrongGlory.Num = 0;
	
	for (i=0; i<pstInfo->Num; i++)
	{
		pstOne = &pstInfo->StrongPoints[i];
		pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstOne->StrongBanner.NpcID);
		if (!pstNpc)
			continue;
		pstStrongSvr->Data.StrongGlory.Glory[pstStrongSvr->Data.StrongGlory.Num].MapID = pstOne->MapID;
		pstStrongSvr->Data.StrongGlory.Glory[pstStrongSvr->Data.StrongGlory.Num].Open = pstOne->Glory.Open;
		pstStrongSvr->Data.StrongGlory.Glory[pstStrongSvr->Data.StrongGlory.Num].x = pstNpc->stMon.stCurrPos.X;
		pstStrongSvr->Data.StrongGlory.Glory[pstStrongSvr->Data.StrongGlory.Num].y = pstNpc->stMon.stCurrPos.Y;
		pstStrongSvr->Data.StrongGlory.Glory[pstStrongSvr->Data.StrongGlory.Num].StartTime = pstOne->Glory.Start;
		pstStrongSvr->Data.StrongGlory.Glory[pstStrongSvr->Data.StrongGlory.Num].EndTime =
														pstOne->Glory.Start + STRONG_GLORY_TIME;
		pstStrongSvr->Data.StrongGlory.Num ++;
	}

	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);

	if (pstPlayer)
	{
		return z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);
}

STRONGPOINTDEF* strong_def_find(ZONESVRENV* pstEnv, int iMapID)
{
	STRONGPOINTDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.MapID = iMapID;
	iIndex = bsearch_int(&stDef, pstObj->sStrongPointDef, pstObj->iStrongPointDef, STRONG_POINT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (STRONGPOINTDEF *)&pstObj->sStrongPointDef[iIndex];
	}
	return NULL;
}

BANNERDEF *strong_banner_def_find(ZONESVRENV* pstEnv, int iType, int iSubType)
{
	BANNERDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Type = iType;
	stDef.SubType = iSubType;

	iIndex = bsearch_int(&stDef, pstObj->sBannerDef, pstObj->iBannerDef, BANNER_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (BANNERDEF *)&pstObj->sBannerDef[iIndex];
	}
	return NULL;
}


int strong_banner_chg(ZONESVRENV* pstEnv, STRONGPOINTONE *pstOne, Monster *pstMon)
{
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;

	memset(pstStrongSvr, 0, sizeof(*pstStrongSvr));
	pstStrongSvr->Cmd = STRONG_SVR_OP_BANNER_CHG;

	pstStrongSvr->Data.BannerChg.MapID = pstOne->MapID;
	pstStrongSvr->Data.BannerChg.MemID = pstOne->StrongBanner.NpcID;
	pstStrongSvr->Data.BannerChg.Back = pstOne->StrongBanner.Back;
	pstStrongSvr->Data.BannerChg.Sign = pstOne->StrongBanner.Sign;
	pstStrongSvr->Data.BannerChg.Style = pstOne->StrongBanner.Style;

	pstStrongSvr->Data.BannerChg.Effect = 0;
	pstMon->cBannerEffect = 0;
	if (pstOne->Glory.Open)
	{
		pstStrongSvr->Data.BannerChg.Effect = 1;
		pstMon->cBannerEffect = 1;
	}
	
	STRNCPY(pstStrongSvr->Data.BannerChg.XuanYan, pstOne->StrongBanner.XuanYan, 
				sizeof(pstStrongSvr->Data.BannerChg.XuanYan));

	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);
	z_sendpkg_mon_area_view(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg);
	return 0;
}

int strong_enter_id_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iID)
{
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;

	pstStrongSvr->Cmd = STRONG_SVR_OP_STRONGID;
	pstStrongSvr->Data.StrongID.ID = iID;

	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int strong_new_banner(ZONESVRENV* pstEnv)
{
	int i;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	STRONGPOINTDEF *pstDef;
	ZONEIDX stIdx;
	MapIndex *pstMapIndex;
	MAPNPC stMapNpc;
	int iRet;
	Npc *pstNpc;
	ZONEMAPINST *pstMInst;
	
	for (i=0; i<pstInfo->Num; i++)
	{
		pstDef = strong_def_find(pstEnv,pstInfo->StrongPoints[i].MapID);
		if (!pstDef)
			continue;

		stIdx.iID = pstDef->MapID;
		stIdx.iPos  =-1;
		pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (!pstMInst)
		{
			continue;
		}

		pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMInst->iMapPos, pstMInst->iMapInstID);
		if (NULL == pstMapIndex)
		{
			continue;
		}

		memset(&stMapNpc, 0, sizeof(stMapNpc));
		stMapNpc.MonsterID = pstDef->BannerMon.MonID;
		stMapNpc.Pos.X = pstDef->BannerMon.Pos.X;
		stMapNpc.Pos.Y = pstDef->BannerMon.Pos.Y;
		stMapNpc.Dir = pstDef->BannerDir%180;
		STRNCPY(stMapNpc.Script, pstDef->Script, sizeof(stMapNpc.Script));

		iRet = z_in_new_pworld_npc(pstEnv->pstAppCtx, pstEnv, pstMapIndex, pstMInst, &stMapNpc,0);
		if (0 > iRet)
		{
			continue;
		}

		pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, iRet);
		if(pstNpc)
		{
			newnpc_add(pstEnv, pstMInst, pstNpc);
		}

		pstInfo->StrongPoints[i].StrongBanner.NpcID = iRet;

		//strong_banne_chg(pstEnv, &pstInfo->StrongPoints[i], &pstNpc->stMon);
	}

	return 0;
}

int strong_global_init(ZONESVRENV* pstEnv)
{
	int i,j;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	STRONGPOINTDEF *pstDef;
	
	// 正向检查,删除定义表中没有的
	for (i=pstInfo->Num-1; i>=0; i--)
	{	
		for (j=0;j<pstEnv->pstZoneObj->iStrongPointDef; j++)
		{
			pstDef = (STRONGPOINTDEF *)&pstEnv->pstZoneObj->sStrongPointDef[j];
			if (pstDef->MapID == pstInfo->StrongPoints[i].MapID)
			{
				break;
			}
		}

		if (j >= pstEnv->pstZoneObj->iStrongPointDef)
		{
			pstInfo->Num --;
			if (pstInfo->Num != i)
			{
				pstInfo->StrongPoints[i] = pstInfo->StrongPoints[pstInfo->Num];
			}
		}
	}

	
	// 反向检查
	for (i=0;i<pstEnv->pstZoneObj->iStrongPointDef; i++)
	{	
		pstDef = (STRONGPOINTDEF *)&pstEnv->pstZoneObj->sStrongPointDef[i];
		for (j=0; j<pstInfo->Num; j++)
		{
			if (pstDef->MapID == pstInfo->StrongPoints[j].MapID)
			{
				break;	
			}	
		}

		if (j>=pstInfo->Num && pstInfo->Num < MAX_STRONG_POINT)
		{
			memset(&pstInfo->StrongPoints[pstInfo->Num], 0, sizeof(pstInfo->StrongPoints[pstInfo->Num]));
			pstInfo->StrongPoints[pstInfo->Num].MapID = pstDef->MapID;
			pstInfo->Num ++;
		}
	}

	
	strong_new_banner(pstEnv);
	return 0;
}


int strong_del_pworld_info(ZONESVRENV* pstEnv, int iMapID)
{
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;
	STRONGPOINTONE *pstOne;

	for (i=0; i<pstInfo->Num; i++)
	{
		pstOne = &pstInfo->StrongPoints[i];
		if (pstOne->MapID == iMapID)
		{
			memset(&pstOne->Pworld,0, sizeof(pstOne->Pworld));
			break;
		}
	}
	return 0;
}

STRONGPOINTONE* strong_point_one_get(ZONESVRENV* pstEnv, int iMapID)
{
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].MapID == iMapID)
		{
			return &pstInfo->StrongPoints[i];
		}
	}
	return NULL;
}

int strong_clan_sale_get(ZONESVRENV* pstEnv, ZoneClan *pstClan, STRONGPOINTINFO *pstInfo)
{
	int i, j;
	int iMoney = 0;
	STRONGPOINTONE *pstOne;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	STRONGINFO *pstStrongInfo = &pstClan->stClan.ClanInner.StrongInfo;
	STRONGPOINTDEF *pstDef;

	for (i=0; i<pstStrongInfo->Num; i++)
	{
		if (!(issameday(iCurr, pstStrongInfo->SaleInfo[i].SaleTime+24*60*60)))
		{
			continue;
		}

		for (j=0; j<pstInfo->Num; j++)
		{
			pstOne = &pstInfo->StrongPoints[j];
			if (pstOne->Sale.ClanID == pstClan->stClan.ClanProf.Id)
			{
				break;
			}

			if (pstOne->AtkClanID == pstClan->stClan.ClanProf.Id)
			{
				break;
			}
		}

		if (j < pstInfo->Num)
		{
			continue;
		}

		pstDef = 	strong_def_find(pstEnv,pstStrongInfo->SaleInfo[i].SaleMap);
		if (pstDef && pstStrongInfo->SaleInfo[i].SaleMoney > 0)
		{
			player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
					ZONEERR_CLANBROAD268 ,
					pstDef->Name,pstStrongInfo->SaleInfo[i].SaleMoney);
		}
		
		iMoney += pstStrongInfo->SaleInfo[i].SaleMoney;
	}

	if (iMoney < 0)
	{
		iMoney = 0;
	}
	return iMoney;
}


int strong_send_mail(ZONESVRENV* pstEnv, ZoneClan *pstAtkClan, ZoneClan *pstDefClan,int iType, 
						STRONGPOINTONE *pstOne, STRONGPOINTDEF *pstDef)
{
	char szMsg[512];
	char szStrongName[64];
	int i;

	snprintf(szStrongName, sizeof(szStrongName), LJY_MAIL54, pstDef->Name);
	
	if (iType == 1)
	{
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL42,
			pstAtkClan->stClan.ClanProf.Name, pstOne->Sale.Money, pstDef->Name);
		
			player_sysmail_fast_send(pstEnv, pstAtkClan->stClan.ClanBrief.Leader.RoleName, szStrongName,
	 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);
			
			for (i=0; i<pstAtkClan->stClan.ClanBrief.AssistNum; i++)
			{
				player_sysmail_fast_send(pstEnv, pstAtkClan->stClan.ClanBrief.Assists[i].RoleName, szStrongName,
	 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);	
			}
		
	}
	else if (iType == 2)
	{
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL44,
			pstAtkClan->stClan.ClanProf.Name, pstOne->Sale.Money, pstDef->Name,
			pstDef->Name, pstDefClan->stClan.ClanProf.Name, pstDef->Name);

		player_sysmail_fast_send(pstEnv, pstAtkClan->stClan.ClanBrief.Leader.RoleName, szStrongName,
	 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);
		for (i=0; i<pstAtkClan->stClan.ClanBrief.AssistNum; i++)
		{
			player_sysmail_fast_send(pstEnv, pstAtkClan->stClan.ClanBrief.Assists[i].RoleName, szStrongName,
 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);	
		}
	}
	else if (iType == 3)
	{
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL45,
			pstDef->Name, pstDef->Name,pstOne->Sale.Money,pstDef->Name);
		
		player_sysmail_fast_send(pstEnv, pstDefClan->stClan.ClanBrief.Leader.RoleName, szStrongName,
	 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);
		for (i=0; i<pstDefClan->stClan.ClanBrief.AssistNum; i++)
		{
			player_sysmail_fast_send(pstEnv, pstDefClan->stClan.ClanBrief.Assists[i].RoleName, szStrongName,
 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);	
		}
	}
	else if(iType == 4)
	{
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL46,
			pstDefClan->stClan.ClanProf.Name,pstDef->Name);
		
		player_sysmail_fast_send(pstEnv, pstDefClan->stClan.ClanBrief.Leader.RoleName, szStrongName,
	 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);
		for (i=0; i<pstDefClan->stClan.ClanBrief.AssistNum; i++)
		{
			player_sysmail_fast_send(pstEnv, pstDefClan->stClan.ClanBrief.Assists[i].RoleName, szStrongName,
 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);	
		}
	}
	else if (iType == 5)
	{
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL47,
			pstAtkClan->stClan.ClanProf.Name,pstDef->Name);
		
		player_sysmail_fast_send(pstEnv, pstAtkClan->stClan.ClanBrief.Leader.RoleName, szStrongName,
	 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);
		for (i=0; i<pstAtkClan->stClan.ClanBrief.AssistNum; i++)
		{
			player_sysmail_fast_send(pstEnv, pstAtkClan->stClan.ClanBrief.Assists[i].RoleName, szStrongName,
 									szMsg,NULL, 0, 0,LJY_MAIL43, 0);	
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int strong_select_event(ZONESVRENV* pstEnv, STRONGPOINTONE *pstOne)
{
	ZoneClan *pstClan = NULL;
	ZoneClan *pstDefClan;
	STRONGPOINTDEF *pstDef;
	int iOcc = 0;
	
	pstOne->AtkClanID = 0;
	memset(&pstOne->Pworld, 0, sizeof(pstOne->Pworld));
	
	// 无人竞拍
	if (pstOne->Sale.ClanID <= 0)
	{
		return 0;
	}

	pstDef = strong_def_find(pstEnv,pstOne->MapID);
	if (!pstDef)
		return 0;

	pstClan = z_id_clan(pstEnv,pstOne->Sale.ClanID);
	if (!pstClan)
		return 0;
	
	if (pstOne->OwerID > 0)
	{
		pstDefClan = z_id_clan(pstEnv, pstOne->OwerID);
		if (!pstDefClan)
		{
			// 占领公会已经木有了
			/*player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
			"您所在公会竞拍%s成功,当前据点无人占领,您的公会现为占领公会!",
                          pstDef->Name);
			
			pstOne->AtkClanID = 0;
			strong_owner_chg(pstEnv, pstOne, pstOne->Sale.ClanID);
			strong_send_mail(pstEnv, pstClan, NULL,1, pstOne, pstDef);
			z_strong_oplog(pstEnv, pstOne->Sale.ClanID, pstOne->MapID);

			if (pstDef->Lvl == 3)
			{
				world_city_set_nextweek_open(pstEnv);
			}
			return 1;*/
			
		}
		else
		{
			/*player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
				ZONEERR_CLANBROAD269,
                          pstDef->Name);*/	
			pstOne->AtkClanID = pstOne->Sale.ClanID;
			strong_send_mail(pstEnv, pstClan, pstDefClan,2, pstOne, pstDef);
			strong_send_mail(pstEnv, pstClan, pstDefClan,3, pstOne, pstDef);
			pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2; // 立即更新
			iOcc = 1;
		}
	}
	

	//  无人占领
	if (!iOcc)
	{
		if (pstOne->Sale.SecondID <= 0)
		{
			player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
				ZONEERR_CLANBROAD270,
                          pstDef->Name);
	
			pstOne->AtkClanID = 0;
			strong_owner_chg(pstEnv, pstOne, pstOne->Sale.ClanID);
			strong_send_mail(pstEnv, pstClan, NULL,1, pstOne, pstDef);
			z_strong_oplog(pstEnv, pstOne->Sale.ClanID,  pstOne->MapID);

			if (pstDef->Lvl == 3)
			{
				world_city_set_nextweek_open(pstEnv);
			}	
		}
		else
		{
			ZoneClan *pstAtkClan;
			player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
				ZONEERR_CLANBROAD271,
                         pstDef->Name);

			strong_owner_chg(pstEnv, pstOne, pstOne->Sale.ClanID);
			z_strong_oplog(pstEnv, pstOne->Sale.ClanID, pstOne->MapID);
			if (pstDef->Lvl == 3)
			{
				world_city_set_nextweek_open(pstEnv);
			}


			pstOne->AtkClanID =  pstOne->Sale.SecondID;
			pstAtkClan = z_id_clan(pstEnv, pstOne->AtkClanID);
			if (pstAtkClan)
			{
				/*player_clan_msg_broadcast(pstEnv, pstAtkClan, SYS_CLAN, 
					ZONEERR_CLANBROAD272,
                         		pstDef->Name);*/
			}
			
			pstDefClan = z_id_clan(pstEnv, pstOne->OwerID);
			if (pstDefClan && pstAtkClan)
			{
				strong_send_mail(pstEnv, pstAtkClan, pstDefClan,4, pstOne, pstDef);
				strong_send_mail(pstEnv, pstAtkClan, pstDefClan,5, pstOne, pstDef);
			}
		}
		
		return 1;
	}

	return 0;
}


int strong_refresh_map_inst(ZONESVRENV* pstEnv,ZONEPWORLDINST *pstZInst,ZONEMAPINST* pstMInst, int StrongMapID)
{
	int i;
	STRONGPOINTDEF *pstDef;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	STRONGPOINTONE *pstOne = NULL;
	Monster *pstMon;
	ZoneClan *pstClan;
	MONSTERDEF *pstMonDef;


	pstDef = strong_def_find(pstEnv, StrongMapID);
	if (!pstDef)
	{
		return -1;
	}

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].MapID == StrongMapID )
		{
			pstOne = &pstInfo->StrongPoints[i];
			break;
		}
	}

	if (!pstOne)
	{
		return -1;
	}

	pstOne->Pworld.MonNum = 0;

	// 刷资源怪
	for (i=0; i<MAX_RES_MON; i++)
	{
		if (pstDef->ResMon[i].MonID == 0)
		{
			break;
		}

		if ((pstMon = new_city_build_normal_mon(pstEnv, pstMInst, pstDef->ResMon[i].MonID,
				RAND1(180), &pstDef->ResMon[i].Pos, NULL, 0)))
		{
			pstMonDef =  z_get_mon_def(pstEnv, pstMon);
			if (pstMonDef && pstMonDef->MiscType&MON_MISC_TYPE_STRONG_RES)
			{
				pstOne->Pworld.Mon[pstOne->Pworld.MonNum].Flag = 1;
			}
			else
			{
				pstOne->Pworld.Mon[pstOne->Pworld.MonNum].Flag = 0;
			}
			
			pstOne->Pworld.Mon[pstOne->Pworld.MonNum].MemID = pstMon->iID;
			pstOne->Pworld.Mon[pstOne->Pworld.MonNum].ClanID = 0;
			pstOne->Pworld.Mon[pstOne->Pworld.MonNum].X = pstDef->ResMon[i].Pos.X;
			pstOne->Pworld.Mon[pstOne->Pworld.MonNum].Y = pstDef->ResMon[i].Pos.Y;
			pstOne->Pworld.MonNum ++;
		}
	}

	// 刷复活点怪
	pstClan = z_id_clan(pstEnv,pstOne->OwerID);
	if (!pstClan)
		return -1;
	new_city_build_normal_mon(pstEnv, pstMInst, pstDef->ReviveMonID[0].MonID,
						RAND1(180), &pstDef->ReviveMonID[0].Pos, pstClan, 0);

	pstClan = z_id_clan(pstEnv,pstOne->AtkClanID);
	if (!pstClan)
		return -1;
	new_city_build_normal_mon(pstEnv, pstMInst, pstDef->ReviveMonID[1].MonID,
						RAND1(180), &pstDef->ReviveMonID[1].Pos, pstClan, 0);
	return 0;
}

STRONGPOINTONE* strong_one_find_by_clan(ZONESVRENV* pstEnv, ZoneClan *pstClan, int iAtk)
{
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;

	if (iAtk)
	{
		for (i=0; i<pstInfo->Num; i++)
		{
			if (pstInfo->StrongPoints[i].IsOpen == 0)
			{
				continue;
			}
			if (pstInfo->StrongPoints[i].AtkClanID == pstClan->stClan.ClanProf.Id)
			{
				return &pstInfo->StrongPoints[i];
			}
		}	
	}
	else
	{
		for (i=0; i<pstInfo->Num; i++)
		{
			if (pstInfo->StrongPoints[i].IsOpen == 0)
			{
				continue;
			}
			
			if (pstInfo->StrongPoints[i].OwerID == pstClan->stClan.ClanProf.Id)
			{
				return &pstInfo->StrongPoints[i];
			}
		}	
	}
	
	return NULL;
}

int strong_owner_chg(ZONESVRENV* pstEnv,STRONGPOINTONE *pstOne, int iOwnerID)
{
	ZoneClan *pstZoneClan;
	int iOld = pstOne->OwerID;
	
	pstOne->OwerID = iOwnerID;
	pstOne->OccTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2;

	pstZoneClan = z_id_clan(pstEnv, iOwnerID);
	if(pstZoneClan)
	{
		//更新时间
		clan_map_resource_time_update(pstEnv->pstAppCtx->stCurr.tv_sec, 
			&pstZoneClan->stClan.ClanCore.NextAddStrongPointResource);

		clan_boon_strong_chg_update(pstEnv,pstOne->MapID, pstZoneClan, 2);
	}

	// 记录公会福利事件
	pstZoneClan = z_id_clan(pstEnv, iOld);
	if (pstZoneClan)
	{
		clan_boon_strong_chg_update(pstEnv,pstOne->MapID, pstZoneClan, 1);	
	}
	return 0;
}

int strong_clan_destory(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	STRONGPOINTONE *pstOne;
	ZONEPWORLDINST *pstPInst;
	ZONEMAPINST *pstMInst;
	ZoneClan *pstOther;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iClanID = 0;

	pstOne = strong_one_find_by_clan(pstEnv, pstClan, 0);
	if (pstOne)
	{
		if (pstOne->Stat == STRONG_STAT_FIGHT)
		{
			pstPInst =  z_find_pworld_inst(pstEnv->pstMapObj, pstOne->Pworld.InstID);
			if (pstPInst)
			{
				pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPInst->astMapInstIdx[0]);
				if (!pstMInst)
					return -1;
				pstOther = z_id_clan( pstEnv, pstOne->AtkClanID);
				if (!pstOther)
					return -1;

				z_pworld_over(pstEnv,pstPInst);	
				pstPInst->iStrongEnd = iCurr+30;
				
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
						ZONEERR_BROADMAP316, 
						pstClan->stClan.ClanProf.Name, pstOther->stClan.ClanProf.Name);	

				iClanID = pstOne->AtkClanID;
			}
		}
		strong_owner_chg(pstEnv,pstOne, iClanID);
		strong_svr_strong_info( pstEnv, NULL, 0);
		strong_banner_reset(pstEnv,  pstOne);	
	}
	else
	{
		pstOne = strong_one_find_by_clan(pstEnv, pstClan, 1);	
		if (pstOne)
		{
			if (pstOne->Stat == STRONG_STAT_FIGHT)
			{
				pstPInst =  z_find_pworld_inst(pstEnv->pstMapObj, pstOne->Pworld.InstID);
				if (pstPInst)
				{
					pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPInst->astMapInstIdx[0]);
					if (!pstMInst)
						return -1;
					pstOther = z_id_clan( pstEnv, pstOne->OwerID);
					if (!pstOther)
						return -1;

					z_pworld_over(pstEnv,pstPInst);	
					pstPInst->iStrongEnd = iCurr+30;
					
					z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
							ZONEERR_BROADMAP316, 
							pstClan->stClan.ClanProf.Name, pstOther->stClan.ClanProf.Name);	
				}
			}
		}
	}


	return 0;
}

int strong_fight_event(ZONESVRENV* pstEnv, STRONGPOINTONE *pstOne)
{
	STRONGPOINTDEF *pstDef;
	ZONEPWORLDINST *pstPInst;
	ZONEIDX stIdx;
	ZONEPWORLD *pstPworld;
	ZONEPWORLDINST *pstZInst;
	ZoneClan *pstClan;
	
	pstZInst = z_find_pworld_inst(pstEnv->pstMapObj, pstOne->Pworld.InstID);
	if (pstZInst)
	{
		z_in_destroy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstZInst);
	}

	pstDef = strong_def_find(pstEnv,pstOne->MapID);
	if (!pstDef)
	{
		return -1;
	}

	if (pstOne->AtkClanID <= 0)
	{
		return -1;
	}

	pstClan = z_id_clan(pstEnv, pstOne->AtkClanID);
	if (!pstClan)
		return -1;

	if (pstOne->OwerID <= 0)
	{
		player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
				ZONEERR_CLANBROAD273,
                          pstDef->Name);

		strong_owner_chg(pstEnv,pstOne, pstOne->AtkClanID);
		pstOne->AtkClanID = 0;
		strong_svr_strong_info( pstEnv, NULL, 0);
		z_strong_oplog(pstEnv, pstOne->OwerID, pstOne->MapID);
		return 0;
	}

	// 创建副本	
	stIdx.iID = pstDef->PworldID;
	stIdx.iPos = -1;
	pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);
	if (pstPworld == NULL || pstPworld->stPworldDef.Type != PWORLD_TYPE_PLOY)
	{
		return -1;
	}

	pstPInst = z_new_ploy_pworld_inst(pstEnv->pstAppCtx, pstEnv, pstPworld, pstPworld->stPworldDef.Timeout*60,0);
	if (!pstPInst)
	{
		return -1;
	}
	pstOne->Pworld.InstID = pstPInst->iPworldInstID;

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, ZONEERR_CLANBROAD274,pstDef->Name);

	pstClan = z_id_clan(pstEnv, pstOne->OwerID);
	if (pstClan)
		player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, ZONEERR_CLANBROAD274,pstDef->Name);
	return 0;
}

int strong_player_is_in_pworld(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEPWORLD *pstZonePworld;
	
	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (pstZonePworld && pstZonePworld->stPworldDef.StrongMapID > 0)
	{
		return 1;
	}
	return 0;
}

int strong_war_one(ZONESVRENV* pstEnv, Player *pstPlayer,STRONGWARONE *pstOne)
{
	ZoneClan *pstClan;
	WarFieldOL *pstWarOL = &pstPlayer->stOnline.stWarOL;
//	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	pstClan = player_get_clan(pstEnv, pstPlayer);
	
	pstOne->AssitNum = pstPlayer->stRoleData.MiscInfo.MiscWar.Assit;
	pstOne->Career = pstPlayer->stRoleData.Career;
	pstOne->CountKill = pstPlayer->stRoleData.MiscInfo.MiscWar.Kill;
	pstOne->EnterTime = pstPlayer->stRoleData.MiscInfo.MiscWar.FightTime;
	pstOne->MaxContinueKill = pstWarOL->iKillNum;
	pstOne->ResGet = pstWarOL->iOre;
	pstOne->CurrContinueKill = pstWarOL->iContinueKill;
	pstOne->MapID = GET_MAPID(pstPlayer->stRoleData.Map);
	pstOne->Die = pstPlayer->stRoleData.MiscInfo.MiscWar.Die;
	pstOne->ClanWID = 0;
	if (pstClan)
		pstOne->ClanWID = pstClan->stClan.ClanProf.GID;
	strncpy(pstOne->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOne->RoleName));
	return 0;
}

int strong_war_one_notify(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;

	memset(pstStrongSvr, 0, sizeof(*pstStrongSvr));
	pstStrongSvr->Cmd = STRONG_SVR_OP_WAR_ONE;
	strong_war_one(pstEnv, pstPlayer,&pstStrongSvr->Data.WarOne);
	
	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}



int strong_player_die(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstKiller,int *piNixi)
{
	ZoneClan *pstClan;
	unsigned int iPosition;
	ZONEMAPINST *pstMInst;
	ZONEPWORLD *pstZonePworld;
	ZONEIDX stIdx;
	int i;
	Player *pstAssit;
	ZoneClan *pstKillClan;
	STRONGPOINTDEF *pstDef;
	MISCWAR *pstWar = &pstKiller->stRoleData.MiscInfo.MiscWar;
	
	
	if (!pstKiller)
		return -1;

	WarFieldOL *pstWarOL = &pstKiller->stOnline.stWarOL;
	
	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstZonePworld ||!strong_is_normal(pstZonePworld))
	{
		return -1;

	}

	pstDef = strong_def_find( pstEnv, pstZonePworld->stPworldDef.StrongMapID);
	if (!pstDef)
		return -1;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = -1;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMInst)
		return -1;

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}

	pstKillClan = player_get_clan(pstEnv, pstKiller);
	if (!pstKillClan)
	{
		return -1;
	}

	iPosition = player_clan_position(pstEnv, pstPlayer);
	if (iPosition >= CLAN_POS_VICE_PRESIDENT )
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,
				ZONEERR_BROADMAP311, 
			pstKiller->stRoleData.RoleName,pstClan->stClan.ClanProf.Name, 
			kClanPosName[iPosition], pstPlayer->stRoleData.RoleName);	
	}

	// 击杀方玩家
	pstWarOL->iContinueKill ++;  	// 连杀数
	if (pstWarOL->iContinueKill > pstWarOL->iKillNum)
	{
		pstWarOL->iKillNum  = pstWarOL->iContinueKill;    // 最大连杀数	
	}

//	pstWarOL->iCurrKill ++;		// 总杀人数
	pstWar->Kill ++;
	//pstWarOL->iDieNum = 0;		// 连死清零
	pstKiller->stOnline.iDieNum = 0;

	if (pstWarOL->iContinueKill == 100)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP305,
			pstKiller->stRoleData.RoleName, pstWarOL->iContinueKill);	
	}
	else if (pstWarOL->iContinueKill == 50)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP306,
			pstKiller->stRoleData.RoleName, pstWarOL->iContinueKill);	
	}
	else if (pstWarOL->iContinueKill == 20)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP307,
			pstKiller->stRoleData.RoleName, pstWarOL->iContinueKill);
	}
	else if(pstWarOL->iContinueKill == 10)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP308,
			pstKiller->stRoleData.RoleName, pstWarOL->iContinueKill);
	}
	else if (pstWarOL->iContinueKill == 5)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP309,
			pstKiller->stRoleData.RoleName, pstWarOL->iContinueKill);
	}
	else if (pstWarOL->iContinueKill == 3)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP310,
			pstKiller->stRoleData.RoleName, pstWarOL->iContinueKill);
	}
	
	if (pstPlayer->stOnline.stWarOL.iContinueKill > 2)
	{
		z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE,ZONEERR_BROADMAP312,
				pstKiller->stRoleData.RoleName, pstPlayer->stRoleData.RoleName,
				pstPlayer->stOnline.stWarOL.iContinueKill);
	}

	// 计算助攻
	for (i = 0; i < pstKiller->stOnline.iVAnimate; i++) 
	{
		pstAssit = z_id_player(pstEnv->pstAppCtx, pstEnv, pstKiller->stOnline.aiVAnimate[i]);
		if(NULL == pstAssit || pstAssit->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		if(CS_STAT_DEAD & pstAssit->stOnline.State)
		{
			continue;
		}

		pstClan = player_get_clan(pstEnv, pstAssit);
		if (!pstClan || pstClan->stClan.ClanProf.GID != pstKillClan->stClan.ClanProf.GID)
		{
			continue;
		}
		
//		pstAssit->stOnline.stWarOL.iAssistKill ++;
		pstAssit->stRoleData.MiscInfo.MiscWar.Assit ++;
	}

	// 杀人获取buff  todo:
	for (i=MAX_STRONG_KILL_BUFF -1; i>=0; i--)
	{
		if (pstDef->KillBuff[i].KillNum <= 0)
		{
			continue;
		}

		if (pstWarOL->iContinueKill == pstDef->KillBuff[i].KillNum)
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstKiller, pstDef->KillBuff[i].BuffID, 
									pstDef->KillBuff[i].BuffLvl, pstDef->KillBuff[i].BuffTime, 1, 0);
			break;
		}
	}
	

	// 被击杀玩家
	pstPlayer->stOnline.stWarOL.iDieSec ++; 		// 死亡累加
	pstPlayer->stOnline.stWarOL.iContinueKill = 0;	// 连杀清零
	pstPlayer->stRoleData.MiscInfo.MiscWar.Die ++;
	
	// 通知客户端个人信息 todo:
	strong_war_one_notify(pstEnv, pstKiller);
	strong_war_one_notify(pstEnv, pstPlayer);
	return 0;
}

int strong_war_award_one(ZONESVRENV* pstEnv, Player *pstPlayer, int iWinID, STRONGPOINTDEF *pstDef)
{
	int i;
	ROLEGRID stRoleGrid[MAX_STRONG_ITEM];
	int iNum = 0;
	STRONGAWARD *pstAward;       
//	int iTime =  pstEnv->pstAppCtx->stCurr.tv_sec;
	char szMsg[128];
	char szTital[32];
	LEVELUP *pstLvl;
	
	if (	pstPlayer->stRoleData.MiscInfo.MiscWar.Assit + pstPlayer->stRoleData.MiscInfo.MiscWar.Kill< 40 &&
		pstPlayer->stRoleData.MiscInfo.MiscWar.Die < 20 && 
		pstPlayer->stRoleData.MiscInfo.MiscWar.FightTime < 1200)
	{
		return 0;
	}

	snprintf(szTital, sizeof(szTital), LJY_MAIL55, pstDef->Name);
	pstLvl = z_get_levelup(pstEnv, pstPlayer->stRoleData.Level);
	if (!pstLvl)
		return 0;
	
	if (pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id == (unsigned int)iWinID)
	{
		pstAward = &pstDef->WinAward;
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL48,
		pstPlayer->stRoleData.RoleName, pstDef->Name);
	}
	else
	{
		pstAward = &pstDef->FailAward;
		snprintf(szMsg, sizeof(szMsg), LJY_MAIL49,
		pstPlayer->stRoleData.RoleName, pstDef->Name);
	}
	
	for (i=0; i<MAX_STRONG_ITEM; i++)
	{
		if (pstAward->Item[i].ItemID <= 0)
		{
			break;
		}

		if (item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid[iNum], pstAward->Item[i].ItemID) < 0)
		{
			stRoleGrid[i].GridData.RoleItem.Num = pstAward->Item[i].ItemNum;
			continue;
		}

		iNum ++;
	}

	if (iNum > 0)
	{
		player_sysmail_fast_send(pstEnv, pstPlayer->stRoleData.RoleName, szTital,szMsg,stRoleGrid, iNum, 0,NULL, 0);
	}

	for (i=0; i<MAX_STRONG_MONEY; i++)
	{
		if (pstAward->Money[i].Type <= 0)
		{
			break;
		}

		package_add_money( pstEnv, pstPlayer, pstAward->Money[i].Type, pstAward->Money[i].Num);
		if (pstAward->Money[i].Type == ATTR_ID_BULL)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_STRONGWAR, 0,  
				pstAward->Money[i].Num, 0, "据点战奖励");
		}
		else if (pstAward->Money[i].Type == ATTR_ID_MONEY)
		{
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_STRONGWAR, 0,  0, 
				pstAward->Money[i].Num, "据点战奖励");
		}
	}

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_EXP_NOLEVEL_LIMIT;
	player_add_exp(pstEnv, pstPlayer, pstLvl->ContributeExp*(1.0*pstAward->Exp/10000), 0);
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_EXP_NOLEVEL_LIMIT;
	return 0;
}

int strong_war_award(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPInst, int iWinID)
{
	STRONGPOINTDEF *pstDef;
	ZONEPWORLD *pstPworld;
	int i;
	Player *pstPlayer;

	pstPworld = z_find_pworld(pstEnv->pstMapObj, &pstPInst->stPworldIdx);
	if (NULL == pstPworld)
	{
		return -1;;
	}

	pstDef = strong_def_find(pstEnv, pstPworld->stPworldDef.StrongMapID);
	if (!pstDef)
	{
		return -1;
	}

	for (i=0; i<pstPInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPInst->aiPlayerID[i]);
		if (!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		strong_war_award_one(pstEnv, pstPlayer, iWinID, pstDef);
	}
	return 0;
}


int strong_war_all_notify(ZONESVRENV* pstEnv, ZONEPWORLDINST *pstPInst, int iEnd)
{
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;
	int i;
	Player *pstPlayer;
	Player *apstPlayer[STRONG_PLAYER_MAX];
	int iNum = 0;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iTime;
	
	memset(pstStrongSvr, 0, sizeof(*pstStrongSvr));
	pstStrongSvr->Cmd = STRONG_SVR_OP_WAR_ALL;

	for (i=0; i<pstPInst->iPlayer; i++)
	{
		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPInst->aiPlayerID[i]);
		if (!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			continue;
		}

		apstPlayer[iNum] = pstPlayer;
		iNum ++;

		iTime = iCurr-pstPlayer->stOnline.stWarOL.tFightTime;
		pstPlayer->stRoleData.MiscInfo.MiscWar.FightTime += iTime;
		pstPlayer->stOnline.stWarOL.tFightTime = iCurr;

		if (iEnd)
		{
			z_strong_fini_oplog( pstEnv, pstPlayer);
		}
	
		strong_war_one(pstEnv, pstPlayer,&pstStrongSvr->Data.WarAll.Ones[pstStrongSvr->Data.WarAll.Num]);
		pstStrongSvr->Data.WarAll.Num++;
		if (pstStrongSvr->Data.WarAll.Num >= STRONG_PLAYER_MAX)
		{
			break;
		}
	}

	if (iNum <= 0)
	{
		return 0;
	}
		
	pstStrongSvr->Data.WarAll.End = iEnd;
	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);
	z_cltmsg_limit_broadcast(pstEnv->pstAppCtx, pstEnv, apstPlayer,iNum, &stPkg);
	return 0;
}

int strong_exit_clear_buff(ZONESVRENV* pstEnv, Player *pstPlayer, ZONEPWORLD *pstPworld)
{
	STRONGPOINTDEF *pstDef;
	ZoneStatus *pstStat;
	int i;

	if (pstPworld->stPworldDef.StrongMapID <= 0 || pstPworld->stPworldDef.PolyType != PWORLD_POLY_STRONG)
		return 0;

	pstDef = strong_def_find( pstEnv, pstPworld->stPworldDef.StrongMapID);
	if (!pstDef)
		return 0;

	pstStat = z_player_find_status(pstPlayer, pstDef->DieBuff.BuffID);
	if (pstStat)
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDef->DieBuff.BuffID, 1, 0);
	}

	for (i=0; i<MAX_STRONG_KILL_BUFF; i++)
	{
		if (pstDef->KillBuff[i].BuffID <= 0)
		{
			break;
		}
		pstStat = z_player_find_status(pstPlayer, pstDef->KillBuff[i].BuffID);
		if (pstStat)
		{
			z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDef->KillBuff[i].BuffID, 1, 0);
		}	
	}
	return 0;
}

int strong_resmon_die(ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef, Player *pstKiller)
{
	STRONGPOINTONE* pstOne;
	ZONEPWORLD *pstZonePworld;
	ZoneClan *pstClan = NULL;
	unsigned int iClanID = 0;
	int i;
	ZONEMAPINST *pstMInst;
	Monster *pstMon1;
	STRONGPOINTDEF *pstDef;
	int iMonID = 0;
	
	if (pstMonDef->GetRes <= 0 )
	{
		return 0;
	}

	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMInst)
	{
		return 0;
	}

	pstZonePworld = z_find_pworld2(pstEnv, pstMon->stMap.iID);
	if (!pstZonePworld ||pstZonePworld->stPworldDef.StrongMapID <= 0)
	{
		return 0;
	}

	pstOne = strong_point_one_get(pstEnv, pstZonePworld->stPworldDef.StrongMapID);
	if (!pstOne)
	{
		return 0;
	}

	if (pstKiller)
	{
		iClanID = pstKiller->stRoleData.MiscInfo.ClanInfo.ClanId.Id;
	}
	else
	{
		for (i=0; i<pstOne->Pworld.MonNum; i++)
		{
			if (	pstOne->Pworld.Mon[i].MemID == pstMon->iID &&
				pstOne->Pworld.Mon[i].ClanID > 0)
			{
				iClanID = (pstOne->OwerID==pstOne->Pworld.Mon[i].ClanID?pstOne->AtkClanID:pstOne->OwerID);
				break;
			}
		}	
	}

	if (iClanID > 0)
	{
		pstClan = z_id_clan(pstEnv, iClanID);
		if (pstClan)
		{
			z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, ZONEERR_BROADMAP317, 
				pstClan->stClan.ClanProf.Name, pstMonDef->Name);
		}
	}
	
	for (i=0; i<pstOne->Pworld.MonNum; i++)
	{
		if (	pstOne->Pworld.Mon[i].MemID == pstMon->iID)
		{
			pstDef = strong_def_find(pstEnv, pstZonePworld->stPworldDef.StrongMapID);
			if (!pstDef)
			{
				return 0;
			}

			if (iClanID > 0)
			{
				if (iClanID == pstOne->AtkClanID)
				{
					iMonID = pstDef->ResMon[i].AtkMonID;
				}
				else 
				{
					iMonID = pstDef->ResMon[i].DefMonID;
				}
			}
			else
			{
				iMonID = pstDef->ResMon[i].MonID;
			}
		
			pstMon1 = new_city_build_normal_mon(pstEnv, pstMInst, iMonID,
										RAND1(180), &pstMon->stCurrPos, pstClan, 0);
			if (pstMon1)
			{
				pstOne->Pworld.Mon[i].MemID = pstMon1->iID;
				if (pstClan)
				{
					pstOne->Pworld.Mon[i].ClanID = iClanID;
					pstOne->Pworld.Mon[i].ClanWID = pstClan->stClan.ClanProf.GID;
				}
				else
				{
					pstOne->Pworld.Mon[i].ClanID = 0;
					pstOne->Pworld.Mon[i].ClanWID = 0;
				}
			
				pstOne->Pworld.Mon[i].X = pstMon->stCurrPos.X;
				pstOne->Pworld.Mon[i].Y = pstMon->stCurrPos.Y;
				// 通知客户端
				strong_svr_pworld_info(pstEnv, NULL, 0, pstMInst, pstZonePworld->stPworldDef.StrongMapID);
			}

			//z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
			//z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
			if (pstKiller)
			{
				pstKiller->stOnline.stWarOL.iOre ++;
			}
			break;
		}	
	}
	
	return 0;
}

int strong_get_stat(ZONESVRENV* pstEnv, int iMapID)
{
	STRONGPOINTONE* pstOne;

	pstOne = strong_point_one_get(pstEnv, iMapID);
	if (!pstOne)
	{
		return -1;
	}

	return pstOne->Stat;
}

int strong_open_in(ZONESVRENV* pstEnv, STRONGPOINTONE *pstOne, STRONGPOINTDEF *pstStrongDef)
{
	pstOne->IsOpen = STRONG_OPEN;
	//strong_svr_strong_info(pstEnv, NULL, 0);
	pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2;

	z_sys_strf_broadcast(pstEnv, SYS_SVR, 
		ZONEERR_BROAD345, pstStrongDef->Name);
	return 0;
}

int strong_check_open(ZONESVRENV* pstEnv)
{
	RangeOneMem * pstOne;
	int i;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	STRONGPOINTDEF *pstStrongDef;
	int iTopNum;
	
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;
	memset(pstStrongSvr, 0, sizeof(*pstStrongSvr));
	pstStrongSvr->Cmd = STRONG_SVR_OP_STRONG_ACTIVE;

	pstOne = range_find_onemem(pstEnv,RANGE_MAIN_SINGLE, RANGE_MAIN_SINGLE_SUB_LEVEL);
	if (!pstOne)
		return 0;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].IsOpen == STRONG_CLOSE)
		{
			pstStrongDef = strong_def_find(pstEnv, pstInfo->StrongPoints[i].MapID);
			if (!pstStrongDef )
				continue;

			if (pstStrongDef->Lvl == 1)
			{
				iTopNum = pstEnv->pstConf->StrongOpen1;
				if (iTopNum <= 0 || iTopNum > RANGE_ONE_NUM)
				{
					iTopNum = RANGE_TOP_NUM;
				}

				if (pstOne->stLastRange.iNum < iTopNum)
					continue;
				
				if (pstOne->stLastRange.astRangeEntry[iTopNum-1].stRange.RangeData.Level >= 50)
				{
					strong_open_in(pstEnv, &pstInfo->StrongPoints[i], pstStrongDef);
					pstStrongSvr->Data.Active.MapIDs[pstStrongSvr->Data.Active.Num] = 
														pstInfo->StrongPoints[i].MapID;
					pstStrongSvr->Data.Active.Num ++;
				}
			}
			else if (pstStrongDef->Lvl == 2)
			{
				iTopNum = pstEnv->pstConf->StrongOpen2;
				if (iTopNum <= 0 || iTopNum > RANGE_ONE_NUM)
				{
					iTopNum = RANGE_TOP_NUM;
				}

				if (pstOne->stLastRange.iNum < iTopNum)
					continue;
				
				if (pstOne->stLastRange.astRangeEntry[iTopNum-1].stRange.RangeData.Level >= 60)
				{
					strong_open_in(pstEnv, &pstInfo->StrongPoints[i], pstStrongDef);
					pstStrongSvr->Data.Active.MapIDs[pstStrongSvr->Data.Active.Num] = 
														pstInfo->StrongPoints[i].MapID;
					pstStrongSvr->Data.Active.Num ++;
				}
			}
			else if (pstStrongDef->Lvl == 3)
			{
				iTopNum = pstEnv->pstConf->StrongOpen3;
				if (iTopNum <= 0 || iTopNum > RANGE_ONE_NUM)
				{
					iTopNum = RANGE_TOP_NUM;
				}

				if (pstOne->stLastRange.iNum < iTopNum)
					continue;
				
				if (pstOne->stLastRange.astRangeEntry[iTopNum-1].stRange.RangeData.Level >= 70)
				{
					strong_open_in(pstEnv, &pstInfo->StrongPoints[i], pstStrongDef);
					pstStrongSvr->Data.Active.MapIDs[pstStrongSvr->Data.Active.Num] = 
														pstInfo->StrongPoints[i].MapID;
					pstStrongSvr->Data.Active.Num ++;
				}
			}
			else
			{
				continue;
			}
		}
	}

	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);

	if (pstStrongSvr->Data.Active.Num > 0)
	{
		z_cltmsg_broadcast(pstEnv->pstAppCtx,pstEnv, &stPkg);
	}
	
	return 0;
}


int strong_glory_close_by_clan_destory(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	int i;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	STRONGPOINTONE *pstOne = NULL;
	STRONGPOINTDEF *pstDef;
	Npc *pstNpc;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].OwerID != pstClan->stClan.ClanProf.Id)
		{
			continue;
		}

		if (pstInfo->StrongPoints[i].Glory.Open == 1)
		{
			pstOne = &pstInfo->StrongPoints[i];
		}
	}

	if (!pstOne)
		return 0;

	pstDef = strong_def_find(pstEnv, pstOne->MapID);
	if (!pstDef)
		return 0;

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstOne->StrongBanner.NpcID);
	if (!pstNpc)
		return 0;

	pstOne->Glory.Open = 0;	
	strong_glory_chg(pstEnv,NULL);
	strong_banner_chg(pstEnv, pstOne, &pstNpc->stMon);
	
	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
				ZONEERR_CLANBROAD275,
             			pstClan->stClan.ClanProf.Name);
	z_sys_strf_broadcast(pstEnv, SYS_MIDDLE2, 
			ZONEERR_BROAD346,
			pstClan->stClan.ClanProf.Name);
	
	return 0;
}

int strong_glory_close(ZONESVRENV* pstEnv, STRONGPOINTONE *pstOne, STRONGPOINTDEF *pstDef)
{
	ZoneClan *pstClan;
	int iChg = 0;
	Npc *pstNpc;

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstOne->StrongBanner.NpcID);
	if (!pstNpc)
		return 0;
	
	if (pstOne->Glory.Open)
	{
		if (pstOne->Glory.Start + STRONG_GLORY_TIME < (unsigned int)pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstOne->Glory.Open = 0;	
			// 通知客户端 todo	
			strong_glory_chg(pstEnv,NULL);
			strong_banner_chg(pstEnv, pstOne, &pstNpc->stMon);

			pstClan = z_id_clan( pstEnv, pstOne->Glory.ID);
			if (!pstClan)
			{
				return 0;
			}
			
			z_sys_strf_broadcast(pstEnv, SYS_MIDDLE2, 
				ZONEERR_BROAD346,
				pstClan->stClan.ClanProf.Name);
			player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
							ZONEERR_CLANBROAD275,
	                         			pstClan->stClan.ClanProf.Name);
		}
		else
		{
			// 据点换人了
			if (pstOne->Glory.ID != pstOne->OwerID)
			{
				iChg = 1;	
			}

			// 公会没了
			pstClan = z_id_clan( pstEnv, pstOne->Glory.ID);
			if (!pstClan )
			{
				iChg = 1;	
			}

			if (iChg)
			{
				pstOne->Glory.Open = 0;	
				strong_glory_chg(pstEnv,NULL);
				strong_banner_chg(pstEnv, pstOne, &pstNpc->stMon);
				if (pstClan)
				{
					player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
							ZONEERR_CLANBROAD275,
	                         			pstClan->stClan.ClanProf.Name);
					z_sys_strf_broadcast(pstEnv, SYS_MIDDLE2, 
							ZONEERR_BROAD346,
							pstClan->stClan.ClanProf.Name);
				}
			}
		}
	}
	return 0;
}

// 每3秒检查一次
int strong_tick(ZONESVRENV* pstEnv)
{
	int i,j;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	STRONGPOINTDEF *pstDef;
	struct tm stNow;
	time_t tNow = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iWeek,iNextWeek;
	int iHour;
	STRONGPOINTONE *pstOne;
	int iNotify = 0;
	int iBackMoney = 0;
	ZoneClan *pstTmpClan;
	int iMoney;
	time_t tOpen;  // 开搞时间
	
	
	localtime_r(&tNow, &stNow);
	iWeek = stNow.tm_wday;
	if (iWeek == 0)
		iWeek = 7;
	iHour = stNow.tm_hour;
	
	stNow.tm_hour = FIGHT_HOUR;
	stNow.tm_min = FIGHT_MIN;
	tOpen = mktime(&stNow);

	for (i=0; i<pstInfo->Num; i++)
	{
		iNextWeek = -1;
		pstOne = &pstInfo->StrongPoints[i];

		if (pstOne->IsOpen == STRONG_CLOSE)
			continue;
		
		pstDef = 	strong_def_find(pstEnv,pstOne->MapID);
		if (!pstDef)
			continue;

		// 关闭战旗荣耀
		strong_glory_close(pstEnv, pstOne, pstDef);

		for (j=0; j<MAX_STRONG_POINT_WEEK; j++)
		{
			if (iWeek <= g_iWeek[j])
			{
				iNextWeek = g_iWeek[j];
				break;
			}
		}

		if (iNextWeek == -1)
			iNextWeek = g_iWeek[0];

		// 暗拍时间
		if (iWeek+1 == iNextWeek)
		{
			if (pstOne->Stat != STRONG_STAT_SALE)
			{
				memset(&pstOne->Sale, 0, sizeof(pstOne->Sale));
				pstOne->Stat = STRONG_STAT_SALE;
			}
		}
		else if (iWeek == iNextWeek)
		{
			if (iHour < SELECT_HOUR)
			{
				if (pstOne->Stat != STRONG_STAT_TONGJI)
				{
					pstOne->Stat = STRONG_STAT_TONGJI;
				}
			}
			else 
			{
				if (tNow < tOpen)
				{
					if (	pstOne->Stat != STRONG_STAT_SELECT )
					{
						if (pstEnv->pstClanObj->stClanObjInfo.iClanStat != CLAN_STAT_FINI)
						{
							return 0;
						}
						
						if (strong_select_event(pstEnv, pstOne))
						{
							iNotify = 1;
						}
						pstOne->Stat = STRONG_STAT_SELECT;
						iBackMoney = 1;
					}
				}
				else
				{
					if (tNow < tOpen+60*60)
					{
						if (pstOne->Stat == STRONG_STAT_SELECT)
						{
							strong_fight_event(pstEnv, pstOne);
							pstOne->Stat = STRONG_STAT_FIGHT;
						}

						// 副本结束了 
						if (pstOne->Pworld.InstID <= 0)
						{
							if (pstOne->Stat != STRONG_STAT_IDLE)
							{
								pstOne->Stat = STRONG_STAT_IDLE;
							}	
						}
					}
					else
					{
						// 副本实例还在不搞
						if (pstOne->Pworld.InstID > 0)
						{
							continue;
						}
						
						if (pstOne->Stat != STRONG_STAT_IDLE)
						{
							pstOne->Stat = STRONG_STAT_IDLE;
						}
					}
				}
			}
		}
		else
		{
			if (pstOne->Stat != STRONG_STAT_IDLE)
			{
				pstOne->Stat = STRONG_STAT_IDLE;
			}
		}
	}

	// 占领据点,全服广播
	if (iNotify)
	{
		strong_svr_strong_info(pstEnv, NULL, 0);
	}

	// 退钱
	if (iBackMoney)
	{
		g_iOptype = OP_OPTYPE_STRONG_SALE;
		for(i = 0; i < pstEnv->pstClanObj->iClanNum; ++i) 
		{
			pstTmpClan = &pstEnv->pstClanObj->astZoneClan[i];
			iMoney = strong_clan_sale_get(pstEnv,pstTmpClan, pstInfo);
			if (iMoney <= 0)
				continue;
			clan_property_modify(pstEnv, pstTmpClan, CLAN_PROPERTY_MONEY, iMoney, 0);
		}
		g_iOptype = 0;
	}
	return 0;
}


// 角色登录时发:据点占领信息
// 据点属主变化时全服广播
int strong_svr_strong_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iSingle)
{
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;
	int i;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	ZoneClan *pstClan;

	memset(pstStrongSvr, 0, sizeof(*pstStrongSvr));
	pstStrongSvr->Cmd = STRONG_SVR_OP_STRONG_INFO;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].IsOpen)
		{
			pstStrongSvr->Data.StrongPointInfo.StrongInfos[pstStrongSvr->Data.StrongPointInfo.Num].IsOpen = 1;
		}
		
		pstClan = z_id_clan(pstEnv,pstInfo->StrongPoints[i].OwerID);
		pstStrongSvr->Data.StrongPointInfo.StrongInfos[pstStrongSvr->Data.StrongPointInfo.Num].MapID = 
																	pstInfo->StrongPoints[i].MapID;
		if (pstClan)
		{
			pstStrongSvr->Data.StrongPointInfo.StrongInfos[pstStrongSvr->Data.StrongPointInfo.Num].ClanWid 
																	= pstClan->stClan.ClanProf.GID;
		}

		pstStrongSvr->Data.StrongPointInfo.Num ++;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);
	if (iSingle)
	{
		if (pstPlayer)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}	
	}
	else
	{
		z_cltmsg_broadcast(pstEnv->pstAppCtx,pstEnv, &stPkg);
	}
	
	return 0;
}


// 进入副本通知
// 有变化时地图广播
int strong_svr_pworld_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iSingle, ZONEMAPINST *pstMInst, int iStrongMapID)
{
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;
//	int i;
//	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	STRONGPOINTONE *pstOne;
	ZoneClan *pstClan;

	memset(pstStrongSvr, 0, sizeof(*pstStrongSvr));
	pstStrongSvr->Cmd = STRONG_SVR_OP_PWORLD_INFO;


	pstOne =  strong_point_one_get(pstEnv, iStrongMapID);
	if (!pstOne)
		return -1;
	memcpy(pstStrongSvr->Data.StrongPworldInfo.Mon, pstOne->Pworld.Mon, 
							sizeof(pstStrongSvr->Data.StrongPworldInfo.Mon));
	pstStrongSvr->Data.StrongPworldInfo.MonNum = pstOne->Pworld.MonNum;
	pstStrongSvr->Data.StrongPworldInfo.AtkRes = pstOne->Pworld.AtkRes;
	pstStrongSvr->Data.StrongPworldInfo.DefRes = pstOne->Pworld.DefRes;
	
	pstClan = z_id_clan(pstEnv, pstOne->OwerID);
	if (pstClan)
	{
		pstStrongSvr->Data.StrongPworldInfo.DefClanWid = pstClan->stClan.ClanProf.GID;
	}

	pstClan = z_id_clan(pstEnv, pstOne->AtkClanID);
	if (pstClan)
	{
		pstStrongSvr->Data.StrongPworldInfo.AtkClanWid = pstClan->stClan.ClanProf.GID;
	}

	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);
	if (iSingle)
	{
		if (pstPlayer)
		{
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		}
	}
	else
	{
		if (pstMInst)
		{
			map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMInst, &stPkg);
		}
	}
	return 0;
}



// 脚本
int strong_pworld_res_add(ZONESVRENV* pstEnv, int iMapID)
{
	STRONGPOINTONE* pstOne;
	Monster *pstMon;
	int i;
	MONSTERDEF *pstMonDef;
	int iChg=0;
	ZONEMAPINST *pstMInst = NULL;
	ZONEPWORLDINST *pstPInst = NULL;
	STRONGPOINTDEF *pstDef;

	pstOne = strong_point_one_get( pstEnv, iMapID);
	if (!pstOne)
	{
		return -1;
	}

	pstDef = strong_def_find( pstEnv, iMapID);
	if (!pstDef)
	{
		return -1;
	}

	for (i=0; i<pstOne->Pworld.MonNum; i++)
	{
		if (pstOne->Pworld.Mon[i].ClanID <= 0)
		{
			continue;
		}	

		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstOne->Pworld.Mon[i].MemID);
		if (!pstMon)
		{
			continue;
		}

		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if (!pstMonDef)
		{
			continue;
		}

		
		if (pstOne->Pworld.Mon[i].ClanID == pstOne->OwerID)
		{
			pstOne->Pworld.DefRes += pstMonDef->GetRes;
			if (pstOne->Pworld.DefRes > pstDef->ResLimit)
				pstOne->Pworld.DefRes = pstDef->ResLimit;
			
			iChg = 1;
		}
		else if (pstOne->Pworld.Mon[i].ClanID == pstOne->AtkClanID) 
		{
			pstOne->Pworld.AtkRes += pstMonDef->GetRes;
			if (pstOne->Pworld.AtkRes > pstDef->ResLimit)
				pstOne->Pworld.AtkRes = pstDef->ResLimit;
			
			iChg = 1;
		}

		if (pstMInst == NULL)
		{
			pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
		}
	}

	if (iChg && pstMInst)
	{
		pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMInst->iPworldInstID);
		if (!pstPInst  || pstPInst->iStrongEnd > 0 || pstPInst->bOver)
			return 0;
		
		strong_svr_pworld_info(pstEnv, NULL, 0, pstMInst, iMapID);
	}
	return 0;
}

int strong_award_check_time(ZONESVRENV* pstEnv, ZoneClan *pstClan, Player *pstPlayer)
{
	int iJoin = 0;
	int i;

	for (i=0; i<(int)pstClan->stClan.ClanCore.MemNum; i++)
	{
		if (pstClan->stClan.ClanCore.ClanMem[i].RoleMember.RoleID == pstPlayer->stRoleData.RoleID)
		{
			iJoin = pstClan->stClan.ClanCore.ClanMem[i].JoinTime;
			break;
		}
	}

	if(!iJoin)
	{
		return -1;
	}

	if (iJoin+12*60*60 > pstEnv->pstAppCtx->stCurr.tv_sec )
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE, ZONEERR_STRONG203);
		return -1;
	}
	return 0;
}

ROLESTRONGINFO * strong_role_get_info(Player *pstPlayer, int iStrongID)
{
	int i;
	ROLESTRONGINFO1 *pstInfo = &pstPlayer->stRoleData.MiscInfo.RoleStrongInfo1;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->RoleStrongInfo[i].StrongID == iStrongID)
		{
			return &pstInfo->RoleStrongInfo[i];
		}
	}

	if (pstInfo->Num >= MAX_STRONG_POINT)
	{
		return NULL;
	}

	memset(&pstInfo->RoleStrongInfo[pstInfo->Num], 0, sizeof(pstInfo->RoleStrongInfo[pstInfo->Num]));
	pstInfo->RoleStrongInfo[pstInfo->Num].StrongID = iStrongID;

	return &pstInfo->RoleStrongInfo[pstInfo->Num++];
}

CLANGETINFO* strong_clan_get_info(ZoneClan *pstClan, int iStrongID)
{
	int i;
	STRONGINFO *pstInfo = &pstClan->stClan.ClanInner.StrongInfo;
	
	for (i=0; i<pstInfo->StrongNum; i++)
	{
		if (pstInfo->StrongGetInfo[i].StrongID == iStrongID)
		{
			return &pstInfo->StrongGetInfo[i];
		}
	}

	if (pstInfo->StrongNum >= MAX_STRONG_POINT)
	{
		return NULL;
	}
	pstInfo->StrongGetInfo[pstInfo->StrongNum].GetTime = 0;
	pstInfo->StrongGetInfo[pstInfo->StrongNum].StrongID = iStrongID;

	return &pstInfo->StrongGetInfo[pstInfo->StrongNum++];
}

// 脚本
// iType: 1-公会资金 2-经验3-buff  4-公会贡献度
int strong_player_award(ZONESVRENV* pstEnv,Player *pstPlayer, int iType, int iMapID)
{
#if 0
	STRONGPOINTONE* pstOne;
	ZoneClan *pstClan;
	int iPosition;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	STRONGPOINTDEF *pstDef;
	STATUSDEF *pstStatDef;
	ROLESTRONGINFO *pstRoleStrong;
	CLANGETINFO *pstClanInfo;
	int iMap = pstPlayer->stRoleData.Map;

	pstOne = strong_point_one_get(pstEnv, iMapID);
	if (!pstOne)
		return -1;

	pstRoleStrong = strong_role_get_info(pstPlayer, iMapID);
	if (!pstRoleStrong)
	{
		return -1;
	}

	pstClan = player_get_clan( pstEnv,  pstPlayer);
	if (!pstClan )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG204);
		return -1;
	}

	pstClanInfo = strong_clan_get_info(pstClan, iMapID);
	if (!pstClanInfo)
		return -1;

	if (pstClan->stClan.ClanProf.Id != pstOne->OwerID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG204);
		return -1;
	}

	if (pstOne->OccTime > 0)
	{
		if (IsSameDay(iCurr , pstOne->OccTime))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
					ZONEERR_STRONG205);
			return -1;
		}
	}

	if ( strong_award_check_time(pstEnv, pstClan, pstPlayer) < 0)
	{
		return -1;
	}

	pstDef = strong_def_find(pstEnv, iMapID);
	if (!pstDef)
		return -1;

	iPosition = player_clan_position(pstEnv, pstPlayer);

	if (iType == 1)
	{
		if (iPosition != CLAN_POS_PRESIDENT)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
								ZONEERR_STRONG206);
			return -1;
		}

		if (pstClanInfo->GetTime > 0)
		{
			if (IsSameDay(iCurr , pstClanInfo->GetTime))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
								ZONEERR_STRONG207);
				return -1;
			}
		}
		
		if (package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstDef->ClanMoney) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOMORE_MONEY3);
			return -1;	
		}

		if (pstDef->ClanMoney > 0)
		{
			package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstDef->ClanMoney);
			z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_STRONGPOINT, pstDef->MapID,  0, pstDef->ClanMoney, "据点占领");
		}
		
		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE,
			           	ZONEERR_STRONG208, pstDef->ClanMoney);
		pstClanInfo->GetTime = iCurr;
	}
	/*else if (iType == 2)
	{
		if (!pstLevelUP)
		{
			return -1;
		}
		
		if (IsSameDay(iCurr ,pstPlayer->stRoleData.MiscInfo.RoleStrongInfo.GetExpTime))
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
				"您今天已经领取过占领经验!");
			return -1;
		}

		if (pstDef->Lvl == 1)
		{
			iExp = 0.8 *pstLevelUP->ContributeExp /100;
		}
		else if (pstDef->Lvl == 2)
		{
			iExp = 1.2 *pstLevelUP->ContributeExp /100;
		}
		else if (pstDef->Lvl == 3)
		{
			iExp = 2.0 *pstLevelUP->ContributeExp/100 ;
		}
		else
		{
			return -1;
		}
		
		player_add_exp(pstEnv,pstPlayer,iExp,0);
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
				"占领经验领取成功，您增加了%d点经验!", iExp);
		pstPlayer->stRoleData.MiscInfo.RoleStrongInfo.GetExpTime = iCurr;
	}*/
	else if (iType == 3)
	{
		pstStatDef = z_find_status(pstEnv, pstDef->BuffID,pstDef->BuffLvl);
		if (!pstStatDef)
			return -1;

		if (GET_PWORLDID(iMap) == 300)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG209);
			return -1;
		}	
		
		if (pstRoleStrong->GetBuffTime > 0)
		{
			if (IsSameDay(iCurr ,pstRoleStrong->GetBuffTime))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE, 
					ZONEERR_STRONG210,pstStatDef->Name);
				return -1;
			}
		}
		
		z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDef->BuffID, 
							pstDef->BuffLvl, pstDef->BuffTime, 1, 0);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG211, pstStatDef->Name);
		pstRoleStrong->GetBuffTime = iCurr;
	}
	else if (iType == 4)
	{
		if (pstRoleStrong->GetContriTime > 0)
		{
			if (IsSameDay(iCurr ,pstRoleStrong->GetContriTime))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
					ZONEERR_STRONG212);
				return -1;
			}
		}
		clan_resource_inc(pstEnv, pstClan, pstPlayer, CLAN_OWNED_CONTRIBUT,pstDef->ClanContri);
		pstRoleStrong->GetContriTime = iCurr;
	}
	else
	{
		return -1;
	}
#endif
	return 0;
}

int  strong_pworld_tick(ZONESVRENV* pstEnv, ZONEPWORLD *pstZonePworld, ZONEPWORLDINST *pstPInst, int iFlag)
{
	STRONGPOINTONE *pstOne;
	STRONGPOINTDEF *pstDef;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ZoneClan *pstClan;
	ZONEMAPINST *pstMInst;
	int iRes;
	
	if (!strong_is_normal(pstZonePworld))
	{
		return 0;
	}

	if (pstPInst->bOver  == 1)
	{
		return 0;
	}

	pstOne =  strong_point_one_get(pstEnv, pstZonePworld->stPworldDef.StrongMapID);
	if (!pstOne)
	{
		return 0;
	}

	pstDef = strong_def_find(pstEnv,  pstZonePworld->stPworldDef.StrongMapID);
	if (!pstDef)
	{
		return 0;
	}

	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPInst->astMapInstIdx[0]);
	if (!pstMInst)
	{
		return 0;
	}

	strong_svr_pworld_info(pstEnv, NULL, 0, pstMInst, pstZonePworld->stPworldDef.StrongMapID);

	if (iFlag)
	{
		if (iCurr + 5*60 >= pstPInst->tEnd)
		{
			if (!(pstPInst->iStrongFlag& 1))
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, 
						ZONEERR_BROADMAP318);
				pstPInst->iStrongFlag|= 1;		
			}
		}
		else if (iCurr + 10*60 >= pstPInst->tEnd)
		{
			if (!(pstPInst->iStrongFlag& 2))
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, 
						ZONEERR_BROADMAP319);
				pstPInst->iStrongFlag|= 2;		
			}
		}
		else if (iCurr + 20*60 >= pstPInst->tEnd)
		{
			if (!(pstPInst->iStrongFlag& 4))
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_MIDDLE, 
						ZONEERR_BROADMAP320);
				pstPInst->iStrongFlag|= 4;		
			}
		}
		
		if (	pstOne->Pworld.AtkRes >= pstDef->ResLimit || 
			pstOne->Pworld.DefRes >= pstDef->ResLimit)
		{
			z_pworld_over(pstEnv,pstPInst);	
			pstPInst->iStrongEnd = iCurr+30;

			if (pstOne->Pworld.DefRes >= pstDef->ResLimit)
			{
				// 防守成功
				pstClan = z_id_clan( pstEnv, pstOne->OwerID);
				if(pstClan)
				{
					clan_boon_strong_chg_update(pstEnv,pstOne->MapID, pstClan, 0);
				}
			}
			else
			{
				pstClan = z_id_clan( pstEnv, pstOne->AtkClanID);
				strong_owner_chg(pstEnv, pstOne, pstOne->AtkClanID);
				// 全服广播,据点属主变化 todo:
				strong_svr_strong_info( pstEnv, NULL, 0);
			}

			if (pstClan)
			{
				z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
					ZONEERR_BROADMAP321, 
					pstClan->stClan.ClanProf.Name);
				strong_war_award(pstEnv, pstPInst, pstClan->stClan.ClanProf.Id);
				strong_war_all_notify(pstEnv, pstPInst, 1);
				z_strong_oplog(pstEnv, pstOne->OwerID,  pstOne->MapID);
			}
			
		}
	}
	else
	{
		z_pworld_over(pstEnv,pstPInst);	
		pstPInst->iStrongEnd = iCurr+30;
		// 各种提示 todo
		if (pstOne->Pworld.AtkRes > pstOne->Pworld.DefRes)
		{
			pstClan = z_id_clan( pstEnv, pstOne->AtkClanID);
			strong_owner_chg(pstEnv, pstOne, pstOne->AtkClanID);
			iRes = pstOne->Pworld.AtkRes;
			// 全服广播,据点属主变化 todo:
			strong_svr_strong_info( pstEnv, NULL, 0);
		}
		else
		{
			// 防守成功
			pstClan = z_id_clan( pstEnv, pstOne->OwerID);
			if(pstClan)
			{
				clan_boon_strong_chg_update(pstEnv,pstOne->MapID, pstClan, 0);
			}
			iRes = pstOne->Pworld.DefRes;
		}

		if (pstClan)
		{
			z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
				ZONEERR_BROADMAP322, 
				pstClan->stClan.ClanProf.Name,iRes);

			strong_war_award(pstEnv, pstPInst, pstClan->stClan.ClanProf.Id);
			strong_war_all_notify(pstEnv, pstPInst, 1);
			z_strong_oplog(pstEnv, pstOne->OwerID,  pstOne->MapID);
		}
	}
	return 0;
}


int strong_make_sale_info(ZONESVRENV* pstEnv,Player *pstPlayer, int iMapID, int iNpcID, ZoneClan *pstClan, 
							STRONGSALEINFO *pstSaleInfo, STRONGPOINTONE* pstOne)
{
	STRONGINFO *pstStrongInfo;
	int i;
	int iSelf = 0;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ZoneClan *pstAtkClan;
	CLANSALEINFO *pstClanSaleInfo=NULL;
	
	pstSaleInfo->SaleStat = pstOne->Stat;
	pstSaleInfo->NpcID = iNpcID;
	pstSaleInfo->MapID = iMapID;
	if (pstClan)
	{
		pstStrongInfo = &pstClan->stClan.ClanInner.StrongInfo;
		for (i=0; i<pstStrongInfo->Num; i++)
		{
			if (pstStrongInfo->SaleInfo[i].SaleMap == iMapID &&
				pstOne->Sale.ClanID > 0)
			{
				pstClanSaleInfo = &pstStrongInfo->SaleInfo[i];
				break;
			}
		}

		if (pstClanSaleInfo)
		{
			if (pstOne->Stat == STRONG_STAT_SALE)
			{
				if (issameday(iCurr, pstClanSaleInfo->SaleTime))
				{
					iSelf = 1;
				}
			}
			else if (	pstOne->Stat == STRONG_STAT_TONGJI ||
					pstOne->Stat == STRONG_STAT_SELECT ||
					pstOne->Stat == STRONG_STAT_FIGHT)
			{
				if (issameday(iCurr, pstClanSaleInfo->SaleTime+60*60*24))
				{
					iSelf = 1;
				}
			}

			if (iSelf)
			{
				pstSaleInfo->ClanJoin = 1;
				pstSaleInfo->ClanMoney = pstClanSaleInfo->SaleMoney;
				pstSaleInfo->ClanPostion = player_clan_position(pstEnv, pstPlayer);
			}
		}
	}

	if (pstOne->Stat >= STRONG_STAT_SELECT)
	{
		pstAtkClan = z_id_clan( pstEnv, pstOne->AtkClanID);
		if (pstAtkClan)
		{
			pstSaleInfo->AtkClanMoney = pstOne->Sale.Money;
			pstSaleInfo->AtkClanWid = pstAtkClan->stClan.ClanProf.GID;
		}
	}
	return 0;
}

// 脚本
int strong_svr_sale_info(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID, int iNpcID)
{
	STRONGPOINTONE* pstInfo;
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;
	ZoneClan *pstClan;

	memset(pstStrongSvr, 0, sizeof(*pstStrongSvr));
	pstInfo = strong_point_one_get(pstEnv, iMapID);
	if (!pstInfo)
	{
		return -1;
	}

	pstClan  = player_get_clan(pstEnv, pstPlayer);
	
	pstStrongSvr->Cmd = STRONG_SVR_OP_SALE_INFO;
	strong_make_sale_info(pstEnv,pstPlayer,iMapID, iNpcID, pstClan, 
							&pstStrongSvr->Data.SaleInfo, pstInfo);

	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int strong_sale_update(ZONESVRENV* pstEnv, STRONGPOINTONE *pstOne, int iSaleClanID, int iSaleMoney)
{
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (iSaleMoney <= pstOne->Sale.Money && iSaleMoney <=  pstOne->Sale.SecondMoney)
	{
		return 0;
	}
	
	if (iSaleMoney > pstOne->Sale.Money)
	{
		pstOne->Sale.SecondID = pstOne->Sale.ClanID;
		pstOne->Sale.SecondMoney = pstOne->Sale.Money;
		pstOne->Sale.SecondTime = pstOne->Sale.Time;

		pstOne->Sale.ClanID = iSaleClanID;
		pstOne->Sale.Money = iSaleMoney;
		pstOne->Sale.Time = iCurr;
	}
	else if (iSaleMoney > pstOne->Sale.SecondMoney)
	{
		pstOne->Sale.SecondID = iSaleClanID;
		pstOne->Sale.SecondMoney = iSaleMoney;
		pstOne->Sale.SecondTime = iCurr;
	}
	else
	{
		return 0;
	}

	return 1;
}


int strong_get_occu_num(ZONESVRENV* pstEnv, unsigned int iClanID)
{
	int iNum = 0;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].OwerID == iClanID)
		{
			iNum++;
		}
	}
	return iNum;
}

int strong_max_level_occu_money(ZONESVRENV* pstEnv, unsigned int iClanID)
{
	int iMoney = 0;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;
	STRONGPOINTDEF *pstDef;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].OwerID == iClanID)
		{
			pstDef = strong_def_find( pstEnv, pstInfo->StrongPoints[i].MapID);
			if (!pstDef)
			{
				continue;
			}

			if (pstDef->ClanMoney > iMoney)
			{
				iMoney = pstDef->ClanMoney;
			}
		}
	}	

	return iMoney;
}

int strong_pworld_is_open_by_clan(ZONESVRENV* pstEnv, ZoneClan *pstClan)
{
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (	pstInfo->StrongPoints[i].OwerID == pstClan->stClan.ClanProf.Id ||
			pstInfo->StrongPoints[i].AtkClanID == pstClan->stClan.ClanProf.Id)
		{
			if (pstInfo->StrongPoints[i].Stat == STRONG_STAT_FIGHT)
			{
				return 1;
			}
		}
	}
	return 0;
}

int strong_clt_sale( ZONESVRENV* pstEnv, Player *pstPlayer, STRONGSALE *pstSale)
{
	ZoneClan *pstClan;
	STRONGPOINTONE *pstOne;
	STRONGPOINTONE *pstOne1;
	int iClanPosition;
	STRONGPOINTDEF *pstDef;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int i;
	CLANSALEINFO *pstInfo = NULL;
	STRONGINFO *pstStrongInfo;
	CLANLEVELDEF *pstClanLvlDef;

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return 0;
	}

	pstOne =  strong_point_one_get(pstEnv, pstSale->MapID);
	if (!pstOne || pstOne->IsOpen == STRONG_CLOSE)
		return 0;

	iClanPosition = player_clan_position(pstEnv, pstPlayer);
	if (	iClanPosition != CLAN_POS_VICE_PRESIDENT &&
		iClanPosition != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG213);
		return 0;
	}

	if (pstOne->OwerID == pstClan->stClan.ClanProf.Id)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG214);
		return 0;
	}

	if (pstOne->Stat != STRONG_STAT_SALE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG215);
		return 0;
	}

	pstStrongInfo = &pstClan->stClan.ClanInner.StrongInfo;
	
	pstClanLvlDef = z_clan_level_def(pstEnv,pstClan->stClan.ClanProf.Level);
	if (!pstClanLvlDef)
	 	return 0;

	

	// 2013-03-20 delete
	/*if (strong_get_by_player(pstEnv,  pstPlayer))
	{
		z_sys_strf( pstEnv,  pstPlayer, SYS_MIDDLE, 
				"您的公会占有据点,不能参加其他据点竞拍!");
		return 0;
	}*/

	
	for (i=pstStrongInfo->Num -1; i>=0; i--)
	{
		if (!(issameday(iCurr, pstStrongInfo->SaleInfo[i].SaleTime)))
		{
			pstStrongInfo->Num --;
			if (i != pstStrongInfo->Num)
			{
				pstStrongInfo->SaleInfo[i] = pstStrongInfo->SaleInfo[pstStrongInfo->Num];
			}
			continue;
		}	

		pstOne1 = strong_point_one_get(pstEnv, pstStrongInfo->SaleInfo[i].SaleMap);
		if (pstOne1->Sale.ClanID <= 0)
		{
			pstStrongInfo->Num --;
			if (i != pstStrongInfo->Num)
			{
				pstStrongInfo->SaleInfo[i] = pstStrongInfo->SaleInfo[pstStrongInfo->Num];
			}
			continue;
		}
	}

	if (pstStrongInfo->Num > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_STRONG216);
		return 0;
	}

	if (	strong_get_occu_num(pstEnv, pstClan->stClan.ClanProf.Id) >= 
		pstClan->stClan.ClanProf.Level+( pstClan->stClan.ClanCore.ClanGroup.UseItemGroup?1:0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_STRONG242);
		return 0;
	}

/*	 if (pstStrongInfo->Num+strong_get_occu_num(pstEnv, pstClan->stClan.ClanProf.Id) >= pstClanLvlDef->OccuNum)
	 {
	 	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG239, pstClan->stClan.ClanProf.Level, pstClanLvlDef->OccuNum);
		return 0;
	 }*/

	if (pstClan->stClan.ClanInner.StrongInfo.Num < MAX_STRONG_POINT)
	{	
		pstInfo = &pstStrongInfo->SaleInfo[pstStrongInfo->Num];
		pstStrongInfo->Num ++;
	}
	else
	{
		return 0;
	}
	
	pstDef = strong_def_find( pstEnv, pstSale->MapID);
	if (!pstDef)
	{
		return 0;
	}

	if (pstDef->BaseUse > pstSale->Money)
	{
		return 0;
	}

	if (clan_property_modify_test(pstEnv, pstClan, CLAN_PROPERTY_MONEY, 
			                     pstSale->Money, 1) < 0)
	{
		return 0;
	}

	g_iOptype = OP_OPTYPE_STRONG_SALE;
	if (clan_property_modify(pstEnv, pstClan, CLAN_PROPERTY_MONEY, 
			                     pstSale->Money, 1) < 0)
	{
		g_iOptype = 0;
		return 0;
	}

	g_iOptype = 0;

	pstInfo->SaleMap = pstSale->MapID;
	pstInfo->SaleMoney = pstSale->Money;
	pstInfo->SaleTime = iCurr;

	// 2013-03-20 modify
	if (strong_sale_update(pstEnv, pstOne,pstClan->stClan.ClanProf.Id,pstSale->Money))
	{
		pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2; // 立即更新
	}
	// 只记录出钱最多的.
	/*if (pstOne->Sale.Money < pstSale->Money)
	{
		pstOne->Sale.ClanID = pstClan->stClan.ClanProf.Id;
		pstOne->Sale.Money = pstSale->Money;
		pstOne->Sale.Time = iCurr;
		pstEnv->pstGlobalObj->stGlobalObjInfo.iDirty = 2; // 立即更新
	}*/
	// modify end

	strong_svr_sale_info(pstEnv, pstPlayer, pstSale->MapID, 0);
	z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv,  pstPlayer, SYS_MIDDLE, 
					ZONEERR_STRONG217);
	return 0;
}

// 在副本中退出公会
/*int strong_pworld_player_exit_clan(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if(strong_player_is_in_pworld(pstEnv, pstPlayer))
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "非公会成员无法参加据点战!");
		player_pworld_exit_chgmap(pstEnv, pstPlayer);
	}
	return 0;
}*/

// 强行退出据点地图
int strong_pworld_force_exit(ZONESVRENV* pstEnv, Player *pstPlayer, char *pstTip)
{
	if(strong_player_is_in_pworld(pstEnv, pstPlayer))
	{
		if (pstTip)
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, pstTip);
		}
		player_pworld_exit_chgmap(pstEnv, pstPlayer);
	}
	return 0;
}

int strong_get_map_pos(ZONESVRENV* pstEnv, int iMapID, MapIndex *pstMapIdx,
							RESPOS *pstPos, Player *pstPlayer, int iReVive)
{
	ZoneClan *pstClan;
	STRONGPOINTONE* pstOne;

	pstOne = strong_point_one_get(pstEnv, iMapID);
	if (!pstOne)
	{
		return -1;
	}

	pstClan = player_get_clan(pstEnv,pstPlayer);
	if (!pstClan)
	{
		return -1;
	}

	// 防守方pos
	if (pstClan->stClan.ClanProf.Id == pstOne->OwerID)
	{
		if (iReVive)
		{
			*pstPos = pstMapIdx->stMapDef.RevivePs[0];
		}
		else
		{
			*pstPos = pstMapIdx->stMapDef.TransPos[0];
		}
		
	}
	// 进攻方pos
	else if (pstClan->stClan.ClanProf.Id == pstOne->AtkClanID)
	{
		if (iReVive)
		{
			*pstPos = pstMapIdx->stMapDef.RevivePs[1];
		}
		else
		{
			*pstPos = pstMapIdx->stMapDef.TransPos[1];
		}
	}
	else
	{
		return -1;
	}
	return 0;
}

int strong_enter_pworld(ZONESVRENV* pstEnv, Player *pstPlayer,  STRONGENTER *pstEnter)
{
//	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	STRONGPOINTONE *pstOne;
	ZONEPWORLDINST *pstPInst;
	ZoneClan *pstClan;
	ZONEPWORLD *pstZonePworld;
	ROLESTRONGINFO1 *pstInfo = &pstPlayer->stRoleData.MiscInfo.RoleStrongInfo1;
	STRONGPOINTDEF *pstDef;

	if ( strong_clt_enter_check( pstEnv,pstPlayer) < 0)
			return -1;
			
	pstOne =  strong_point_one_get(pstEnv, pstEnter->MapID);
	if (!pstOne)
		return -1;

	if (pstOne->Pworld.InstID <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2 ,ZONEERR_STRONG219);
		return -1;
	}

	pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstOne->Pworld.InstID);
	if (!pstPInst)
	{
		return -1; 
	}

	if (pstInfo->StrongID > 0)
	{
		pstDef = strong_def_find(pstEnv,pstInfo->StrongID);
		if (!pstDef)
		{
			return -1;
		}
		
		if (pstInfo->StrongID != pstEnter->MapID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2 ,
								ZONEERR_STRONG220,pstDef->Name);
			return -1;
		}
	}

	pstClan = player_get_clan(pstEnv,  pstPlayer);
	if (!pstClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2 , ZONEERR_STRONG221);
		return -1;
	}
	
	if (pstClan->stClan.ClanProf.Id != pstOne->OwerID &&
		pstClan->stClan.ClanProf.Id != pstOne->AtkClanID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2 , ZONEERR_STRONG222);
		return -1;
	}

	if(!is_in_strong_group_player(pstClan, pstPlayer, pstEnter->MapID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2 , ZONEERR_STRONG223);
		return -1;
	}

	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPInst->stPworldIdx);
	if (!pstZonePworld)
		return -1;
	
	z_player_enter_pworld_map(pstEnv, pstPlayer, pstPInst, pstZonePworld);
	pstPlayer->stOnline.iDieNum = 0;
	strong_war_one_notify(pstEnv, pstPlayer);

	pstPlayer->stRoleData.MiscInfo.RoleStrongInfo1.StrongID = pstEnter->MapID;
	strong_enter_id_chg( pstEnv, pstPlayer, pstEnter->MapID);
	return 0;
}


int strong_limit_check(ZONESVRENV* pstEnv,Player *pstPlayer, int iMapID, ZoneClan *pstClan)
{
	unsigned int iPosition;
	STRONGPOINTONE *pstOne;

	pstOne =  strong_point_one_get(pstEnv, pstPlayer->stRoleData.Map);
	if (!pstOne)
	{	
		return -1;
	}

	if (!pstClan || pstOne->OwerID != pstClan->stClan.ClanProf.Id)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG224);
		return -1;
	}

	iPosition = player_clan_position(pstEnv, pstPlayer);
	if (	iPosition != CLAN_POS_PRESIDENT &&
		iPosition != CLAN_POS_VICE_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG225);
		return -1;
	}

	return 0;
}

int strong_open_set_banner_ui(ZONESVRENV* pstEnv,Player *pstPlayer, Npc* pstNpc)
{
	ZoneClan *pstClan;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (strong_limit_check(pstEnv,pstPlayer, pstPlayer->stRoleData.Map, pstClan) < 0)
	{
		return -1;
	}

	open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, UI_TYPE_SET_BANNER);
	return 0;
}

int strong_open_set_banner_xuanyan_ui(ZONESVRENV* pstEnv,Player *pstPlayer, Npc* pstNpc)
{
	ZoneClan *pstClan;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (strong_limit_check(pstEnv,pstPlayer, pstPlayer->stRoleData.Map, pstClan) < 0)
	{
		return -1;
	}
	open_ui(pstEnv, pstPlayer, pstNpc->stMon.iID, UI_TYPE_SET_BANNER_XUANYAN);
	return 0;
}

int strong_make_banner_view( ZONESVRENV* pstEnv, CSMONSTER *pstCSMon, Monster *pstVMon)
{
	STRONGPOINTONE* pstOne;
	MONSTERDEF *pstMonDef = z_get_mon_def(pstEnv, pstVMon);
	
	memset(&pstCSMon->BannerInfo, 0, sizeof(pstCSMon->BannerInfo));
	if (!pstMonDef )
	{
		return 0;
	}

	if (!(pstMonDef->MiscType & MON_MISC_TYPE_BANNER))
	{
		return 0;
	}

	pstOne = strong_point_one_get(pstEnv, pstVMon->stMap.iID);
	if (!pstOne)
	{
		return 0;
	}

	pstCSMon->BannerInfo.Back = pstOne->StrongBanner.Back;
	pstCSMon->BannerInfo.Sign = pstOne->StrongBanner.Sign;
	pstCSMon->BannerInfo.Style = pstOne->StrongBanner.Style;
	pstCSMon->BannerInfo.GloryEffect = pstVMon->cBannerEffect;
	STRNCPY(pstCSMon->BannerInfo.XuanYan, pstOne->StrongBanner.XuanYan, sizeof(pstCSMon->BannerInfo.XuanYan));
	return 0;
}

int strong_set_banner(ZONESVRENV* pstEnv, Player *pstPlayer, STRONGSETBANNER*pstBanner)
{
	ZoneClan *pstClan;
	unsigned int iPosition;
	BANNERDEF *pstDef;
	int iClanMoney = 0;
	int TechLvl;
	STRONGPOINTONE *pstOne;
	Npc *pstMon;

	pstOne =  strong_point_one_get(pstEnv,pstBanner->MapID);
	if (!pstOne || pstOne->StrongBanner.NpcID <= 0 || pstOne->StrongBanner.NpcID != pstBanner->NpcID)
	{
		return -1;
	}
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan || pstOne->OwerID != pstClan->stClan.ClanProf.Id)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG224);
		return -1;
	}

	iPosition = player_clan_position(pstEnv, pstPlayer);
	if (	iPosition != CLAN_POS_PRESIDENT &&
		iPosition != CLAN_POS_VICE_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG225);
		return -1;
	}

	pstMon = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstOne->StrongBanner.NpcID);
	if (!pstMon)
	{
		return -1;
	}
	
	if (pstBanner->Style)
	{
		pstDef = strong_banner_def_find(pstEnv, BANNER_TYPE_STYLE, pstBanner->Style);
		if (!pstDef)
			return -1;
		iClanMoney += pstDef->ClanMoney;

		TechLvl = pstClan->stClan.ClanAdvance.Tech.TechLevel[CLAN_TECH_CATEGORY_SHAPE];
		if (pstDef->TechLimit > 0 && TechLvl < pstDef->TechLimit )
		{
			goto tech;
		}
	}

	if (pstBanner->Sign)
	{
		pstDef = strong_banner_def_find(pstEnv, BANNER_TYPE_SIGN, pstBanner->Sign);
		if (!pstDef)
			return -1;
		iClanMoney += pstDef->ClanMoney;

		TechLvl = pstClan->stClan.ClanAdvance.Tech.TechLevel[CLAN_TECH_CATEGORY_SHADING];
		if (pstDef->TechLimit > 0 && TechLvl < pstDef->TechLimit)
		{
			goto tech;
		}
	}

	if (pstBanner->Back)
	{
		pstDef = strong_banner_def_find(pstEnv, BANNER_TYPE_BACKGROUP, pstBanner->Back);
		if (!pstDef)
			return -1;
		iClanMoney += pstDef->ClanMoney;

		TechLvl = pstClan->stClan.ClanAdvance.Tech.TechLevel[CLAN_TECH_CATEGORY_MARK];
		if (pstDef->TechLimit > 0 && TechLvl < pstDef->TechLimit )
		{
			goto tech;
		}
	}

	if (iClanMoney > 0)
	{
		if (clan_property_modify(pstEnv, pstClan, CLAN_PROPERTY_MONEY, iClanMoney, 1) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_STRONG226);
			return -1;
		}
	}

	pstOne->StrongBanner.Back = pstBanner->Back;
	pstOne->StrongBanner.Sign = pstBanner->Sign;
	pstOne->StrongBanner.Style = pstBanner->Style;

	// 通知区域内玩家
	strong_banner_chg(pstEnv, pstOne, &pstMon->stMon);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_STRONG227);
	return 0;
tech:
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_STRONG228);
	return 0;
}


int strong_set_banner_xuanyan(ZONESVRENV* pstEnv, Player *pstPlayer, STRONGBANNERXUANYAN *pstXuanYan)
{
	ZoneClan *pstClan;
	unsigned int iPosition;
	STRONGPOINTONE *pstOne;
	Npc *pstNpc;
	
	pstOne =  strong_point_one_get(pstEnv, pstXuanYan->MapID);
	if (!pstOne)
	{	
		return -1;
	}

	pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstOne->StrongBanner.NpcID);
	if (!pstNpc)
	{
		return -1;
	}

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan || pstOne->OwerID != pstClan->stClan.ClanProf.Id)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG224);
		return -1;
	}

	iPosition = player_clan_position(pstEnv, pstPlayer);
	if (	iPosition != CLAN_POS_PRESIDENT &&
		iPosition != CLAN_POS_VICE_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG225);
		return -1;
	}

	if (strcmp(pstOne->StrongBanner.XuanYan, pstXuanYan->XuanYan) == 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_STRONG229);
		return -1;
	}

	if (pstXuanYan->XuanYan[0]!=0)
	{
		if ((CheckDirty(pstXuanYan->XuanYan, 0x1f) & 0x1f ) )
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIRTY);
			return -1;
		}
	}
	
	STRNCPY(pstOne->StrongBanner.XuanYan, pstXuanYan->XuanYan, sizeof(pstOne->StrongBanner.XuanYan));
	// 通知区域内玩家
	strong_banner_chg(pstEnv, pstOne, &pstNpc->stMon);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_STRONG230);
	return 0;
}


int strong_clt_info_all(ZONESVRENV* pstEnv, Player *pstPlayer, STRONGINFOALLREQ *pstReq)
{
	CSPKG stPkg;
	STRONGSVR *pstStrongSvr  = &stPkg.Body.StrongSvr;
	STRONGALLINFO *pstAll = &pstStrongSvr->Data.AllInfo;
	int i;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	ZoneClan *pstClan;
	ZoneClan *pstOwner;
	WORLDCITY *pstWorldCity = &pstEnv->pstGlobalObj->stGlobal.Misc.WorldCity;

	pstClan = player_get_clan(pstEnv, pstPlayer);	
	memset(pstAll, 0, sizeof(*pstAll));

	for(i=0; i<pstInfo->Num; i++)
	{
		strong_make_sale_info(pstEnv,pstPlayer, pstInfo->StrongPoints[i].MapID, 0, pstClan, 
						&pstAll->AllInfoOnes[pstAll->Num].SaleInfo, &pstInfo->StrongPoints[i]);
	
		if (pstInfo->StrongPoints[i].IsOpen)
		{
			pstAll->AllInfoOnes[pstAll->Num].OccupInfo.IsOpen = 1;
		}
		
		pstAll->AllInfoOnes[pstAll->Num].OccupInfo.MapID = pstInfo->StrongPoints[i].MapID;	

		if (pstInfo->StrongPoints[i].OwerID > 0)
		{
			pstOwner = z_id_clan(pstEnv, pstInfo->StrongPoints[i].OwerID);
			if (pstOwner)
			{
				pstAll->AllInfoOnes[pstAll->Num].OccupInfo.ClanWid = pstOwner->stClan.ClanProf.GID;
			}
		}
		
		pstAll->Num ++;
	}

	pstAll->CityStat = pstWorldCity->CityCore.Stat;
	pstStrongSvr->Cmd = STRONG_SVR_OP_ALL_INFO;

	Z_CSHEAD_INIT(&stPkg.Head, STRONG_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);	
	return 0;
}

int strong_player_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	STRONGINFOALLREQ stReq;
	strong_clt_info_all(pstEnv, pstPlayer, &stReq);
	return 0;
}


int strong_player_award_money(ZONESVRENV* pstEnv,Player *pstPlayer, ZoneClan* pstZoneClan)
{
	int iPosition;
	int iMoney = 0;
	int iOucc = 0;
//	CLANGETINFO *pstClanInfo;

	iPosition = player_clan_position(pstEnv, pstPlayer);
	if (iPosition != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG206);
		return -1;
	}

	if (IsSameDay(pstZoneClan->stClan.ClanCore.LastClanPayTime,
			pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_CLAN264);
		return -1;	
	}

	// 时间城固定10w
	if (is_world_city_master(pstEnv,pstPlayer))
	{
		iMoney += 100000;
		iOucc += 1;
	}
	else
	{
		iMoney += strong_max_level_occu_money(pstEnv, pstZoneClan->stClan.ClanProf.Id);
	}

	iOucc += strong_get_occu_num(pstEnv,  pstZoneClan->stClan.ClanProf.Id);
	iMoney += (iOucc-1)*10000;

	if (iMoney <= 0)
	{
		return -1;
	}

	if (0 >  package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney))
	{			
		return -1;
	}

	package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, iMoney);
	z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_CLANPAY, 0,  0, iMoney, "公会占领资金");
	pstZoneClan->stClan.ClanCore.LastClanPayTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE,ZONEERR_STRONG208, iMoney);
	return 0;
}


int strong_player_award_1(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ZoneClan* pstZoneClan = player_get_clan(pstEnv,pstPlayer);
	CLANMEM* pstMem = NULL;
	
	if (!pstZoneClan)
	{
		return -1;
	}

	pstMem = clan_id_mem(pstZoneClan, pstPlayer->stRoleData.RoleID,
	                         &pstPlayer->stOnline.iClanMemIdx);
	if (!pstMem )
	{
		return -1;
	}
	
	if (pstMem->JoinTime + 2 * 86400 >= pstEnv->pstAppCtx->stCurr.tv_sec )
	{
		//return -1;
	}
	
	strong_player_award_money(pstEnv,pstPlayer, pstZoneClan);
	return 0;
}


int strong_clt_award(ZONESVRENV* pstEnv, Player *pstPlayer, STRONGAWARDREQ*pstReq)
{
	
	strong_player_award_1(pstEnv,pstPlayer);
	/*if (!pstReq->StrongID)
	{
		player_clan_pay(pstEnv, pstPlayer, CLAN_PAY_MONEY,NULL);
	}
	else
	{
		strong_player_award(pstEnv,pstPlayer,pstReq->Type, pstReq->StrongID);	
	}*/
	return 0;
}


int strong_clt_enter_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iRet;	
	ITEMDEF *pstItemDef;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	iRet = team_trans_check_one(pstEnv,pstPlayer);
	team_trans_check_tip(pstEnv, pstPlayer,1, iRet, NULL);
	if (iRet < 0)
		return -1;

	if (pstPlayer->stOnline.State &CS_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG231);
		return -1;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG232);
		return -1;	
	}


	if (0 < pstRoleTmpSys->TmpItemID)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (pstItemDef && pstItemDef->ItemTmp.Type == ITEM_TMP_MACHINE)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG201);
			return -1;
		}
	}

	if (is_player_ride_on_machine(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG202);
		return -1;
	}

	if (pstPlayer->stOnline.State &(CS_STAT_RIDE_ON |CS_STAT_FLY | CS_STAT_HITCH_RIDE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG202);
		return -1;
	}

	//末世巅峰准备地图中不能传送
	if (pstPlayer->stRoleData.Map == SHZ_READY_MAP_ID || pstPlayer->stRoleData.Map == ZHZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == JNZ_READY_MAP_ID || pstPlayer->stRoleData.Map == LHS_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == LSZ_READY_MAP_ID || pstPlayer->stRoleData.Map == HMZ_READY_MAP_ID ||
		pstPlayer->stRoleData.Map == SYZ_READY_MAP_ID || pstPlayer->stRoleData.Map == YLS_READY_MAP_ID )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG38);
		return -1;
	}

	return 0;
}

int strong_clt_enter(ZONESVRENV* pstEnv, Player *pstPlayer,STRONGENTERREQ *pstReq)
{
	if (!pstReq->StrongID)
	{
		MapIndex *pstMapIdx;
		ZONEMAPINST *pstMapInst;
		ZONEIDX stIdx;
		RESPOS stPos;

		if (GET_MAPID(pstPlayer->stRoleData.Map) == WORLD_CITY_MAPID)
		{
			return 0;
		}
		
		if ( strong_clt_enter_check( pstEnv,pstPlayer) < 0)
			return 0;
		
		if (!is_in_siege_city_fast(pstEnv))
		{
			return 0;
		}	

		stIdx.iID = WORLD_CITY_MAPID;
		stIdx.iPos = -1;
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);;
		if (NULL == pstMapInst)
		{
			return 0;
		}

		pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (NULL == pstMapIdx)
		{
			return 0;
		}

		if (is_world_city_owner(pstEnv, pstPlayer))
		{
			stPos = pstMapIdx->stMapDef.TransPos[0];
		}
		else
		{
			stPos = pstMapIdx->stMapDef.TransPos[1];
		}
		z_player_change_map(pstEnv->pstAppCtx, pstEnv, pstPlayer, WORLD_CITY_MAPID, &stPos, RAND1(180), 0);

	}
	else
	{
		STRONGENTER stEnter;
		stEnter.MapID = pstReq->StrongID;
		strong_enter_pworld(pstEnv, pstPlayer,  &stEnter);	

		
	}
	return 0;
}

int strong_clt_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	STRONGCLT *pstStrongClt = &pstCsPkg->Body.StrongClt;

	if(INST_MODE_ZONE_SVR != pstEnv->iInstMode )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}
	
	 switch(pstStrongClt->Cmd)
	 {
	case STRONG_CLT_OP_SALE:
		return strong_clt_sale( pstEnv, pstPlayer, &pstStrongClt->Data.Sale);
		break;
	case STRONG_CLT_OP_SET_BANNER:
		return strong_set_banner(pstEnv, pstPlayer, &pstStrongClt->Data.SetBanner);
		break;
	case STRONG_CLT_OP_SET_BANNER_XUANYAN:
		return strong_set_banner_xuanyan(pstEnv, pstPlayer, &pstStrongClt->Data.SetXuanyan);
		break;
	case STRONG_CLT_OP_INFO_ALL:
		return strong_clt_info_all( pstEnv, pstPlayer, &pstStrongClt->Data.InfoAllReq);
		break;
	case STRONG_CLT_OP_AWARD:
		return strong_clt_award(pstEnv, pstPlayer, &pstStrongClt->Data.AwardReq);
		break;
	case STRONG_CLT_OP_ENTER:
		return strong_clt_enter( pstEnv, pstPlayer,&pstStrongClt->Data.EnterReq);
		break;
	 }
	return 0;
}

int strong_is_open(ZONESVRENV* pstEnv, int iMapID)
{
	STRONGPOINTONE* pstOne;

	pstOne = strong_point_one_get(pstEnv, iMapID);
	if (pstOne)
	{
		return pstOne->IsOpen;
	}
	return 0;
}

int strong_banner_reset(ZONESVRENV* pstEnv,  STRONGPOINTONE*pstOne)
{
	Npc *pstNpc;
	pstOne->StrongBanner.Back = 0;
	pstOne->StrongBanner.Sign = 0;
	pstOne->StrongBanner.Style = 0;
	pstOne->StrongBanner.XuanYan[0] = 0;

	pstNpc = z_id_npc(pstEnv->pstAppCtx,pstEnv, pstOne->StrongBanner.NpcID);
	if (pstNpc)
		strong_banner_chg(pstEnv, pstOne, &pstNpc->stMon);
	return 0;
}

int strong_cancel(ZONESVRENV* pstEnv, Player * pstPlayer, int iMapID)
{
	STRONGPOINTONE* pstOne;
	ZoneClan *pstClan;
	int iClanPosition;
	STRONGPOINTDEF *pstDef;
	ZONEPWORLDINST *pstPInst;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ZONEMAPINST *pstMInst;
	ZoneClan *pstAtkClan;
	int iClanID=0;
	
	pstClan = player_get_clan(pstEnv,  pstPlayer);
	if (!pstClan)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,pstPlayer, SYS_MIDDLE, ZONEERR_NO_POWER_OP);
		return 0;
	}

	iClanPosition = player_clan_position(pstEnv, pstPlayer);
	if (iClanPosition != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_NO_POWER_OP);
		return 0;
	}

	pstOne = strong_point_one_get(pstEnv, iMapID);
	if (!pstOne)
	{
		return 0;
	}

	if (pstOne->OwerID != pstClan->stClan.ClanProf.Id)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_NO_POWER_OP);
		return 0;
	}

	if (pstOne->Stat == STRONG_STAT_FIGHT)
	{
		pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstOne->Pworld.InstID);		
		if (pstPInst)
		{
			pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPInst->astMapInstIdx[0]);
			if (!pstMInst)
				return 0;

			pstAtkClan = z_id_clan(pstEnv, pstOne->AtkClanID);
			if (!pstAtkClan)
				return 0;
	
			z_pworld_over(pstEnv,pstPInst);	
			pstPInst->iStrongEnd = iCurr+30;
			
			z_sys_strf_map_broadcast(pstEnv, pstMInst,SYS_SVR, 
						ZONEERR_BROADMAP323,
						pstClan->stClan.ClanProf.Name, pstAtkClan->stClan.ClanProf.Name);

			iClanID = pstOne->AtkClanID;

			strong_war_award(pstEnv, pstPInst,iClanID);
			strong_war_all_notify(pstEnv, pstPInst, 1);
		}

	}

	strong_owner_chg(pstEnv, pstOne, iClanID);
	strong_svr_strong_info( pstEnv, NULL, 0);
	strong_banner_reset(pstEnv,  pstOne);

	pstDef = strong_def_find( pstEnv, iMapID);
	if (pstDef)
	{
		player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN, 
				ZONEERR_CLANBROAD276,pstDef->Name);
	}
	
	return 0;
}

STRONGPOINTONE* strong_get_by_player(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int i;
	ZoneClan *pstClan;

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return NULL;

	
	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].OwerID == pstClan->stClan.ClanProf.Id)
		{
			return &pstInfo->StrongPoints[i];
		}
	}
	
	return NULL;
}

int strong_clear_all(ZONESVRENV* pstEnv)
{
	int i;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	int iMapID;
	int iBanner;
	ZoneClan *pstTmpClan;

	for (i=0; i<pstInfo->Num; i++)
	{
		iMapID = pstInfo->StrongPoints[i].MapID;
		iBanner = pstInfo->StrongPoints[i].StrongBanner.NpcID;
		memset(&pstInfo->StrongPoints[i], 0, sizeof(pstInfo->StrongPoints[i]));
		pstInfo->StrongPoints[i].MapID = iMapID;
		pstInfo->StrongPoints[i].StrongBanner.NpcID = iBanner;

		strong_banner_reset( pstEnv,  &pstInfo->StrongPoints[i]);
	}

	strong_svr_strong_info(pstEnv, NULL, 0);

	for(i = 0; i < pstEnv->pstClanObj->iClanNum; ++i) 
	{
		pstTmpClan = &pstEnv->pstClanObj->astZoneClan[i];
		if (pstTmpClan)
		{
			memset(&pstTmpClan->stClan.ClanInner.StrongInfo, 0 , 
					sizeof(pstTmpClan->stClan.ClanInner.StrongInfo));
		}
	}
	return 0;
}

int strong_set_owner(ZONESVRENV* pstEnv,Player *pstPlayer, int iMapID)
{
	STRONGPOINTONE* pstOne;
	ZoneClan *pstClan;

	pstClan = player_get_clan( pstEnv, pstPlayer);
	if (!pstClan)
	{
		return 0;
	}
	
	pstOne = strong_point_one_get(pstEnv,iMapID);
	if (!pstOne || !pstOne->IsOpen)
	{
		return 0;
	}

	strong_owner_chg(pstEnv,pstOne, pstClan->stClan.ClanProf.Id);
	//pstOne->OwerID = pstClan->stClan.ClanProf.Id;
	strong_svr_strong_info(pstEnv, NULL, 0);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_STRONG233);
	
	return 0;
}

int strong_clear(ZONESVRENV* pstEnv, Player *pstPlayer, int iMapID)
{
	STRONGPOINTONE* pstOne;
	ZoneClan *pstClan;
	STRONGINFO *pstStrongInfo;
	int i;
	unsigned int uiOwerID;

	pstOne = strong_point_one_get(pstEnv,iMapID);
	if (!pstOne)
	{
		return 0;
	}

	uiOwerID = pstOne->OwerID;
	memset (pstOne, 0, sizeof(*pstOne));
	pstOne->MapID = iMapID;
	pstOne->OwerID = uiOwerID;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return 0;

	
	pstStrongInfo = &pstClan->stClan.ClanInner.StrongInfo;
	for (i=pstStrongInfo->Num-1; i>=0; i--)
	{
		if (pstStrongInfo->SaleInfo[i].SaleMap > 0)
		{
			pstStrongInfo->Num --;
			if (i != pstStrongInfo->Num)
			{
				pstStrongInfo->SaleInfo[i] = pstStrongInfo->SaleInfo[pstStrongInfo->Num];
			}
			break;
		}
	}
	return 0;
}


// 判断一个据点是否开启了荣誉
int strong_glory_is_open(ZONESVRENV* pstEnv, STRONGPOINTONE* pstOne)
{
	return pstOne->Glory.Open;
}


//玩家 公会是否有据点开启了荣誉
int strong_glory_clan_is_open(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneClan *pstClan;
	int i;
	STRONGPOINTINFO *pstInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.StrongPointInfo;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return 0;

	for (i=0; i<pstInfo->Num; i++)
	{
		if (pstInfo->StrongPoints[i].OwerID != pstClan->stClan.ClanProf.Id)
		{
			continue;
		}

		if (pstInfo->StrongPoints[i].Glory.Open == 1)
		{
			return pstInfo->StrongPoints[i].MapID;
		}
	}
	return 0;
}

// 判断某个玩家是不是某个据点成员
int strong_player_is_owner(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap)
{
	ZoneClan *pstClan;
	STRONGPOINTONE* pstOne;
	
	pstOne = strong_point_one_get(pstEnv,iMap);
	if (!pstOne)
	{
		return 0;
	}

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return 0;

	return pstOne->OwerID == pstClan->stClan.ClanProf.Id?1:0;
}




int strong_grory_set_open(ZONESVRENV* pstEnv, STRONGPOINTONE* pstOne)
{
	pstOne->Glory.Start = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstOne->Glory.Open = 1;
	pstOne->Glory.ID = pstOne->OwerID;
	return 0;
}


int strong_glory_open(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap)
{
	STRONGPOINTONE* pstOne;
	time_t  t = pstEnv->pstAppCtx->stCurr.tv_sec;
	struct tm *pstTm;
	STRONGPOINTDEF *pstDef = NULL;
	int iOpenMap = 0;
	ZoneClan *pstClan;
	int iClanPosition;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
		return -1;

	iClanPosition = player_clan_position(pstEnv, pstPlayer);
	if (	iClanPosition != CLAN_POS_VICE_PRESIDENT &&
		iClanPosition != CLAN_POS_PRESIDENT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, 
							ZONEERR_STRONG234);
		return -1;
	}
	
	pstOne = strong_point_one_get(pstEnv,iMap);
	if (!pstOne)
	{
		return -1;
	}

	if (pstOne->OwerID != pstClan->stClan.ClanProf.Id)
	{
		return -1;
	}

	if (pstOne->OccTime > 0)
	{
		if (IsSameDay(t , pstOne->OccTime))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
					ZONEERR_STRONG235);
			return -1;
		}
	}

	if (IsSameDay(pstOne->Glory.Start, t))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG236);
		return -1;
	}

	if ((iOpenMap = strong_glory_clan_is_open(pstEnv, pstPlayer)) > 0)
	{
		pstDef = strong_def_find(pstEnv, iOpenMap);
		if (pstDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG237,pstDef->Name);	
		}
		pstDef = NULL;
		return -1;
	}

	pstTm = localtime(&t);
	if (pstTm->tm_hour >= 0 && pstTm->tm_hour <= 5)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_STRONG238);
		return -1;
	}

	strong_grory_set_open( pstEnv, pstOne);

	// 公会广播战旗荣耀开始
	strong_glory_chg(pstEnv,NULL);

	pstDef = strong_def_find(pstEnv, iMap);
	if (pstDef)
	{
		CSPKG stPkg;
		POSTIONLINK stPostionLink;
		Npc *pstNpc;

		pstNpc = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstOne->StrongBanner.NpcID);
		if (!pstNpc)
			return 0;

		strong_banner_chg(pstEnv, pstOne,&pstNpc->stMon);
		
		stPostionLink.MapID = GET_MAPID(pstNpc->stMon.stMap.iID);
		stPostionLink.X = pstNpc->stMon.stCurrPos.X;
		stPostionLink.Y = pstNpc->stMon.stCurrPos.Y;

		z_fill_link_msg(&stPkg, SYS_SVR, CHAT_LINK_POSTION, (void *)&stPostionLink, ZONEERR_LINK311,
			pstClan->stClan.ClanProf.Name);
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkg);

		z_fill_link_msg(&stPkg, SYS_CLAN, CHAT_LINK_POSTION, (void *)&stPostionLink, ZONEERR_LINK312, pstPlayer->stRoleData.RoleName);
		player_clan_broadcast(pstEnv, pstClan, &stPkg, 0);

	}
	return 0;
}

int strong_glory_player_login(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	strong_glory_chg(pstEnv, pstPlayer);
	return 0;
}

int strong_glory_player_tick(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	STRONGPOINTONE* pstOne;
	Npc *pstNpc;
	LEVELUP *pstLvlUp;
	
	if (pstPlayer->stRoleData.Level < 10)
	{
		return 0;
	}
	
	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		return 0;
	}

	pstOne = strong_point_one_get(pstEnv,pstPlayer->stRoleData.Map);
	if (!pstOne)
	{
		return 0;
	}

	if (!pstOne->Glory.Open)
	{
		return 0;
	}

	pstNpc = z_id_npc(pstEnv->pstAppCtx,  pstEnv, pstOne->StrongBanner.NpcID);
	if (!pstNpc)
	{
		return 0;
	}

	if (z_distance(&pstPlayer->stRoleData.Pos, &pstNpc->stMon.stCurrPos) > 1200)
	{
		return 0;	
	}

	pstLvlUp = (LEVELUP *)pstEnv->pstZoneObj->sLevelUpDef[pstPlayer->stRoleData.Level-1];
	player_add_exp(pstEnv, pstPlayer, pstLvlUp->ContributeExp/10000.0*5, 0);
	return 0;
}

//  普通据点
int strong_is_normal(ZONEPWORLD *pstPworld)
{
	if (	pstPworld && 
		pstPworld->stPworldDef.StrongMapID > 0 && 
		pstPworld->stPworldDef.PolyType == PWORLD_POLY_STRONG)
	{
		return 1;
	}
	return 0;
}

