

#include "zone_fairy.h"
#include "zone_clt.h"
#include "zone_package.h"
#include "zone_pet.h"
#include "zone_player.h"
#include "zone_move.h"
#include "zone_relation.h"
#include "zone_task.h"
#include "zone_act_status.h"
#include "zone_attr.h"
#include "zone_skill.h"
#include "zone_oplog.h"
#include "zone_map.h"
#include "zone_status.h"
#include "zone_err.h"

//#define ZEROREELID 5251119

FAIRYDEF *faity_find(ZONESVRENV* pstEnv, int iFairyID, int iLv)
{
//	FAIRYDEF *pstFairyDef;
	FAIRYDEF stFairyDef;
	int iEqu,iFairyIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stFairyDef.FairyID = iFairyID;
	stFairyDef.Level = iLv;
	iFairyIndex = bsearch_int(&stFairyDef, pstObj->sFairyDef, pstObj->iFairyDef, FAIRY_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (FAIRYDEF *)&pstObj->sFairyDef[iFairyIndex];
	}

	return NULL;
}

FAIRYSTARDEF *fairy_star_find(ZONESVRENV* pstEnv, int iFairyID, int iStar)
{
	FAIRYSTARDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.FairyID = iFairyID;
	stDef.Star = iStar;

	iIndex = bsearch_int(&stDef, pstObj->sFairyStarDef, pstObj->iFairyStarDef, FAIRY_STUFF_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (FAIRYSTARDEF *)&pstObj->sFairyStarDef[iIndex];
	}
	return NULL;
}

FAIRYSTARSTUFFDEF * fairy_star_stuff_find(ZONESVRENV* pstEnv, int iItemID)
{
	FAIRYSTARSTUFFDEF stDef;
	int iEqu,iIndex;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.ItemID = iItemID;
	iIndex = bsearch_int(&stDef, pstObj->sFairyStuffDef, pstObj->iFairyStuffDef, FAIRY_STAR_DATA, &iEqu);
	if (iEqu)
	{
		return (FAIRYSTARSTUFFDEF *)&pstObj->sFairyStuffDef[iIndex];
	}
	return NULL;
}

int fairy_chg_to_clt(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEFAIRY *pstRoleFairy)
{
	CSPKG stPak;
	CSFAIRYSVR *pstFairySvr = &stPak.Body.FairySvr;  
	int iLeft;

	pstFairySvr->FairyResType = FAIRY_RES_TYPE_DATA;
	pstFairySvr->FairySvrData.FairyData.FairyID = pstRoleFairy->FairyID;
	pstFairySvr->FairySvrData.FairyData.FairyLv = pstRoleFairy->FairyLevel;
	pstFairySvr->FairySvrData.FairyData.Stat = pstRoleFairy->Stat;
	pstFairySvr->FairySvrData.FairyData.WID = pstRoleFairy->WID;
	pstFairySvr->FairySvrData.FairyData.Grididx = pstRoleFairy->GridIdx;
	pstFairySvr->FairySvrData.FairyData.EvolveLeftTM = 0;
	if (pstRoleFairy->Stat & FAIRY_STAT_EVOLVE)
	{
		iLeft = (int)pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM - pstEnv->pstAppCtx->stCurr.tv_sec;
		if (iLeft < 0)
			iLeft = 0;
		pstFairySvr->FairySvrData.FairyData.EvolveLeftTM = iLeft;
	}
	pstFairySvr->FairySvrData.FairyData.Exp = pstRoleFairy->Exp;
	pstFairySvr->FairySvrData.FairyData.Star = pstRoleFairy->StarLvl;
	pstFairySvr->FairySvrData.FairyData.TraitID = pstRoleFairy->TraitID;
	pstFairySvr->FairySvrData.FairyData.IsKeyin = pstRoleFairy->IsKeyin;

	Z_CSHEAD_INIT(&stPak.Head, FAIRY_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPak);
	return 0;
}

// iFlag: 0--出征  1--收回 2--升级
int fairy_action(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy, int iFlag)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSROLEFAIRYACTION *pstRoleFairyAct = &pstActionRes->Data.FairyAct;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_FAIRY_VIEW;

	pstRoleFairyAct->Flag = iFlag;
	pstRoleFairyAct->ID = pstRoleFairy->FairyID;
	pstRoleFairyAct->LV = pstRoleFairy->FairyLevel;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int fairy_get_free_grid(Player *pstPlayer)
{
	char cGrid[FAIRY_MAX_NUM];
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	//FairyOnline *pstFairyOnline = &pstPlayer->stOnline.stFairyOnline;
	
	assert(pstFairyPkg->FairyMaxNum <= FAIRY_MAX_NUM);
	memset(cGrid, 0, sizeof(cGrid));

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].GridIdx >=0 &&
			pstFairyPkg->RoleFairys[i].GridIdx < pstFairyPkg->FairyMaxNum)
		{
			cGrid[(int)(pstFairyPkg->RoleFairys[i].GridIdx)] = 1;
		}
	}

	for (i=0; i<pstFairyPkg->FairyMaxNum; i++)
	{
		if (!cGrid[i])
		{
			return i;
		}
	}

	return -1;

	
}

// 收回精灵 删除技能
int fairy_del_skill(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEFAIRY *pstRoleFairy)
{
	int i,iSkillID;
	ZoneSkill *pstZoneSkill;
	FAIRYDEF *pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);

	if (!pstFairyDef)
	{
		return -1;
	}
	
	for (i=0; i<FAIRY_MAX_SKILL; i++)
	{
		iSkillID = pstFairyDef->FairySkillList[i].SkillID;
		if (iSkillID == 0)
		{
			break;
		}
		
		if (pstFairyDef->FairySkillList[i].SkillType == FAIRY_SKILL_TYPE_DIATHESE)
		{
			continue;
		}

		pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
		if (NULL == pstZoneSkill)
		{
			continue;
		}
		z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, iSkillID, 1);
	}

	return 0;
}

// 出征添加技能
int fairy_add_skill(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEFAIRY *pstRoleFairy)
{

	int i;
	SKILLDEF *pstSkillDef;
	ZoneSkill *pstZoneSkill;
	int iTmp,iSkillID,iSkillLV;

	FAIRYDEF *pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);

	if (!pstFairyDef)
	{
		return -1;
	}

	for (i=0; i<FAIRY_MAX_SKILL; i++)
	{
		iSkillID = pstFairyDef->FairySkillList[i].SkillID;
		iSkillLV = pstFairyDef->FairySkillList[i].SkillLevel;
		if (iSkillID == 0 || iSkillLV == 0)
		{
			break;
		}
		
		if (pstFairyDef->FairySkillList[i].SkillType == FAIRY_SKILL_TYPE_DIATHESE)
		{
			continue;
		}

		pstSkillDef = z_find_skill(pstEnv, iSkillID, iSkillLV, NULL);
		if (NULL == pstSkillDef)
		{
			continue;
		}

		if (0 > z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, iSkillID, iSkillLV, 0))
		{
			continue;
		}

		pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
		if (NULL == pstZoneSkill)
		{
			continue;
		}
	

		pstZoneSkill->ullPetWID = pstRoleFairy->WID;
		pstZoneSkill->stSkillData.CoolLeft = pstSkillDef->SelfCool;
		pstZoneSkill->stSkillData.CollectNum = 0;
		
		iTmp = pstZoneSkill->stSkillData.CoolLeft*100.0;
		TV_ADD_MS(pstZoneSkill->stCoolEnd, pstEnv->pstAppCtx->stCurr, iTmp);		
		
		z_skill_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstZoneSkill);
	}
	return 0;
}

int fairy_up_online_by_login(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	int i;

	FAIRYPACKAGE *pstpkg = &pstPlayer->stRoleData.Package.FairyPackage;

	for (i=0; i<pstpkg->FairyNum; i++)
	{
		if (pstpkg->RoleFairys[i].Stat & FAIRY_STAT_CAMP)
		{
			fairy_up_online(pstEnv,pstPlayer,&pstpkg->RoleFairys[i]);
			break;
		}
	}

	return 0;
}


// 下线自动停止炼化
int fairy_evolve_stop_by_logout(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	/*int i;

	FAIRYPACKAGE *pstpkg = &pstPlayer->stRoleData.Package.FairyPackage;

	for (i=0; i<pstpkg->FairyNum; i++)
	{
		if (pstpkg->RoleFairys[i].Stat == FAIRY_STAT_EVOLVE)
		{
			pstpkg->RoleFairys[i].Stat = FAIRY_STAT_EVOLVE_STOP;
			fairy_chg_to_clt(pstEnv, pstPlayer,&pstpkg->RoleFairys[i]);
			pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_EVOLVE_ING;
			break;
		}
	}*/

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
}

int fairy_up_online(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEFAIRY *pstRoleFairy)
{
	FairyOnline *pstFairyOnline = &pstPlayer->stOnline.stFairyOnline;
	int i,j,iSkillID,iSKillLV;
	FAIRYDEF *pstFairyDef;
	DIATHESISDEF *pstDiathDef;
	SKILLRESULT stResult;
	
	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
	{
		return -1;
	}
	
	//memset (pstFairyOnline, 0, sizeof(*pstFairyOnline));
	pstFairyOnline->iDiathNum = 0;
	pstFairyOnline->ullCampWID = pstRoleFairy->WID;
	pstFairyOnline->stFairyAttr.unPhyAtk = pstFairyDef->PhyAtk;
	pstFairyOnline->stFairyAttr.unMgcAtk = pstFairyDef->MgcAtk;
	pstFairyOnline->stFairyAttr.unBrkATk = pstFairyDef->BrkAtk;
	
	pstFairyOnline->stFairyAttr.unFireATk = pstFairyDef->FireAtk;
	pstFairyOnline->stFairyAttr.unThunderATk = pstFairyDef->ThunderAtk;
	pstFairyOnline->stFairyAttr.unLightATk = pstFairyDef->LightAtk;
	pstFairyOnline->stFairyAttr.unNightATk = pstFairyDef->NightAtk;
	pstFairyOnline->stFairyAttr.unWaterATk = pstFairyDef->WaterAtk;
	
	for (i=0; i<FAIRY_MAX_SKILL; i++)
	{
		if (pstFairyDef->FairySkillList[i].SkillType == FAIRY_SKILL_TYPE_DIATHESE)
		{	
			iSkillID = pstFairyDef->FairySkillList[i].SkillID;
			iSKillLV = pstFairyDef->FairySkillList[i].SkillLevel;

			pstDiathDef = z_find_diathesis(pstEnv, iSkillID, iSKillLV);
			if (!pstDiathDef)
			{
				continue;
			}
			pstFairyOnline->FairyDiathList[(int)(pstFairyOnline->iDiathNum)].DiathID = iSkillID;
			pstFairyOnline->FairyDiathList[(int)(pstFairyOnline->iDiathNum)].DiathLV = iSKillLV;
			pstFairyOnline->FairyDiathList[(int)(pstFairyOnline->iDiathNum)].CoolEnd = 0;

			// result1 --时间 秒  result2 --val
			for (j=0; j<MAX_DIATHESIS_RESULT; j++)
			{
				if (pstDiathDef->DiathesisResult[j].SkillResultID == 0)
				{
					break;
				}
				if (pstDiathDef->DiathesisResult[j].SkillResultID == RESULT_ADD_EXP_ONTIME ||
					pstDiathDef->DiathesisResult[j].SkillResultID == RESULT_ADD_XP_ONTIME)
				{
					pstFairyOnline->FairyDiathList[(int)(pstFairyOnline->iDiathNum)].CoolEnd = 
						stResult.SkillResultVal1+pstEnv->pstAppCtx->stCurr.tv_sec;
					pstPlayer->stRoleData.MiscInfo.Flag |= FAIRY_DIATH_TICK;
				}

				
				if (pstDiathDef->DiathesisResult[j].SkillResultID == RESULT_BEIHOULING)
				{
					pstPlayer->stRoleData.MiscInfo.Flag |= FAIRY_BEIHOULING;
					pstPlayer->stOnline.stBhlingOl.bBHlingRate = pstDiathDef->DiathesisResult[j].SkillResultVal1;
					pstPlayer->stOnline.stBhlingOl.bSkillLvl = pstDiathDef->DiathesisResult[j].SkillResultVal3;
					pstPlayer->stOnline.stBhlingOl.iSkillID = pstDiathDef->DiathesisResult[j].SkillResultVal2;
				}
			}
			
			pstFairyOnline->iDiathNum ++;
			
		}
	}

	
	return 0;
}

/*int get_gird_by_wid(Player *pstPlayer, tdr_ulonglong ullWID)
{
	int i;
	for (i=0; i<pstPlayer->stOnline.stFairyOnline.iNum; i++)
	{
		if (pstPlayer->stOnline.stFairyOnline.FairyOneOnline[i].WID == ullWID)
		{
			return pstPlayer->stOnline.stFairyOnline.FairyOneOnline[i].cGridIdx;
		}
	}

	return -1;
}*/

/*tdr_ulonglong get_wid_by_grid(Player *pstPlayer, int iGirdIdx)
{
	int i;
	for (i=0; i<pstPlayer->stOnline.stFairyOnline.iNum; i++)
	{
		if (pstPlayer->stOnline.stFairyOnline.FairyOneOnline[i].cGridIdx == iGirdIdx)
		{
			return pstPlayer->stOnline.stFairyOnline.FairyOneOnline[i].WID;
		}
	}

	return -1;
}*/



int fairy_camp_pre_brk(ZONESVRENV* pstEnv, Player *pstPlayer,tdr_ulonglong ullWID)
{
	CSPKG stPkg;
	CSFAIRYSVR *pstFairySvr = &stPkg.Body.FairySvr;  

	pstFairySvr->FairyResType = FAIRY_RES_TYPE_PRE_BRK;
	pstFairySvr->FairySvrData.WID = ullWID;

	Z_CSHEAD_INIT(&stPkg.Head, FAIRY_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int fairy_born_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iFairyID)
{
	int i;
	FAIRYDEF *pstFairyDef;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	pstFairyDef = faity_find(pstEnv, iFairyID, 1);
	if (!pstFairyDef)
	{
		return -1;
	}

	if (pstFairyPkg->FairyNum >= pstFairyPkg->FairyMaxNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY1);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY2);
		return -1;
	}

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].FairyID == iFairyID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY3);
			return -1;
		}
	}

	return 0;
}

ROLEFAIRY * fairy_born(ZONESVRENV* pstEnv, Player *pstPlayer,int iFairyID)
{
	FAIRYDEF *pstFairyDef;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	ROLEFAIRY *pstRoleFairy;

	

	if (pstFairyPkg->FairyNum >= pstFairyPkg->FairyMaxNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY4);
		return NULL;
	}
	
	pstFairyDef = faity_find(pstEnv, iFairyID, 1);
	if (!pstFairyDef)
	{
		return NULL;
	}

	pstRoleFairy = &pstFairyPkg->RoleFairys[pstFairyPkg->FairyNum];

	memset(pstRoleFairy, 0, sizeof(*pstRoleFairy));
	pstRoleFairy->FairyID = pstFairyDef->FairyID;
	pstRoleFairy->FairyLevel = 1;
	pstRoleFairy->Stat |= FAIRY_STAT_BACK;
	pstRoleFairy->StarLvl = 1;
	pstRoleFairy->TraitID = pstFairyDef->TraitID;
	pstRoleFairy->WID = z_get_gid( pstEnv->pstAppCtx, pstEnv );
	pstRoleFairy->GridIdx = fairy_get_free_grid(pstPlayer);
	if (pstRoleFairy->GridIdx < 0)
	{
		return NULL;
	}

	pstFairyPkg->FairyNum ++;
	
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY5, pstFairyDef->Name);
	
	z_role_fairy_oplog(pstEnv,pstPlayer,pstRoleFairy, OP_FAIRY_TYPE_BORN,1,NULL,0);
	return pstRoleFairy;
	
}

ROLEFAIRY *get_pack_fairy(Player *pstPlayer,tdr_ulonglong ullWID)
{
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	
	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].WID == ullWID)
		{
			return &pstFairyPkg->RoleFairys[i];
		}
	}

	return NULL;
}


/*
1.	在同一时间下，每个角色只能同时召唤出一只精灵（如果不收回就召唤另一个精灵则替换为新召唤的精灵，原召唤的精灵系统自动收回）；
2.	死亡状态不可召唤；（系统提示：当前状态无法召唤）
3.	战斗状态不可召唤；（系统提示：当前状态无法召唤）
4.	使用技能时不可召唤；（系统提示：当前状态无法召唤）
5.	移动中不可召唤；（系统提示：当前状态无法召唤）
*/

int fairy_camp_check(ZONESVRENV* pstEnv,Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID)
{
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	ZONEIDX stIdx;
	ROLEFAIRY *pstRoleFairy;
	FAIRYDEF *pstFairyDef;
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEPWORLD *pstZonePworld;

	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy )
	{
		return -1;
	}

	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
	{
		return -1;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{
		pstZonePworldInst = z_find_pworldinst2(pstEnv, pstPlayer->stRoleData.Map);
		if (NULL == pstZonePworldInst)
		{
			return -1;
		}

		pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstZonePworldInst->stPworldIdx);
		if (NULL == pstZonePworld)
		{
			return -1;
		}

		if (pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_FAIRY_F)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY6);
			return -1;
		}

	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
		return -1;
	}

	if (pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_NO_FAIRY_F)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY7);
		return -1;
	}
	
	/*if (pstPlayer->stOnline.State & (CS_STAT_DEAD ))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "死亡状态不能召唤精灵");
		return -1;
	}

	if (pstPlayer->stOnline.State & (CS_STAT_FIGHT))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "战斗状态不能召唤精灵");
		return -1;
	}*/

	if (pstRoleFairy->Stat & FAIRY_STAT_CAMP)
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY8,pstFairyDef->Name);
		return -1;
	}

	/*if (pstRoleFairy->Stat & FAIRY_STAT_EVOLVE)
	{	
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "(%s)精灵正在炼化状态中,无法出征",pstFairyDef->Name);
		return -1;
	}*/

	return 0;
	UNUSED(iGridIdx);	//add by paraunused.pl
}

int fairy_camp_pre(ZONESVRENV* pstEnv, Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID)
{
	FairyOnline *pstFairyOnline = &pstPlayer->stOnline.stFairyOnline;

	// 如果有精灵吟唱中 打断前面的吟唱
	if ((pstPlayer->stOnline.State & CS_STAT_FAIRY_CAMPAIGN_PRE) &&
		pstFairyOnline->ullWID > 0)
	{
		if (pstFairyOnline->ullWID == ullWID)
		{
			return 0;
		}
		
		fairy_camp_pre_brk(pstEnv, pstPlayer,pstFairyOnline->ullWID);
		pstPlayer->stOnline.State &= ~CS_STAT_FAIRY_CAMPAIGN_PRE;
		pstFairyOnline->ullWID = 0;
		pstFairyOnline->uiCampPreTime = 0;
	}

	// 检查禁止召唤状态
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NO_BECKON))
	{
		fairy_camp_pre_brk(pstEnv, pstPlayer,ullWID);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY9);
		return -1;
	}
	
	if (fairy_camp_check(pstEnv,pstPlayer,iGridIdx, ullWID) < 0)
	{
		fairy_camp_pre_brk(pstEnv, pstPlayer,ullWID);
		return -1;
	}

	pstPlayer->stOnline.State |= CS_STAT_FAIRY_CAMPAIGN_PRE;
	pstFairyOnline->ullWID = ullWID;
	pstFairyOnline->uiCampPreTime = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
	
}

int set_fairy_camp(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY*pstRoleFairy)
{
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	// 精灵出征清空XP
	fairy_camp_clean_xp(pstEnv, pstPlayer);

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_CAMP)
		{
			pstFairyPkg->RoleFairys[i].Stat &= ~FAIRY_STAT_CAMP;
			pstFairyPkg->RoleFairys[i].Stat |=  FAIRY_STAT_BACK;
			fairy_del_skill(pstEnv,pstPlayer,&pstFairyPkg->RoleFairys[i]);
			fairy_chg_to_clt(pstEnv, pstPlayer,&pstFairyPkg->RoleFairys[i]);
			fairy_action(pstEnv, pstPlayer, &pstFairyPkg->RoleFairys[i], 1);
			pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_DIATH_TICK;
			pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_BEIHOULING;
			break;
		}
	}

	pstRoleFairy->Stat |= FAIRY_STAT_CAMP;
	pstRoleFairy->Stat &= ~FAIRY_STAT_BACK;
	fairy_add_skill(pstEnv,pstPlayer,pstRoleFairy);
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
	fairy_action(pstEnv, pstPlayer, pstRoleFairy, 0);

	fairy_up_online(pstEnv,pstPlayer,pstRoleFairy);
	
	// 计算数值
	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	
	return 0;
}

int set_fairy_drop(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy)
{
	if (!pstRoleFairy)
	{
		return -1;
	}
	
	if (pstRoleFairy->Stat & FAIRY_STAT_CAMP)
	{
		fairy_del_skill(pstEnv,pstPlayer,pstRoleFairy);
	
		pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_DIATH_TICK;
		pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_BEIHOULING;
	}
	
	pstRoleFairy->Stat |= FAIRY_STAT_DROP;
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
	fairy_action(pstEnv, pstPlayer, pstRoleFairy, 1);// 

	return 0;
}

int fairy_camp_clean_xp(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	
	CSPKG stPkg;
	CSPLAYERATTRVAL *pstChg = &stPkg.Body.AttrChg;

	if (pstPlayer->stRoleData.RoleDetail.XP != 0)
	{
		pstPlayer->stRoleData.RoleDetail.XP = 0;
		pstChg->AttrNum = 1;
		pstChg->AttrList[0].AttrID = ATTR_ID_XP;
		pstChg->AttrList[0].Value.XP = 0;

		Z_CSHEAD_INIT(&stPkg.Head, ATTR_CHG);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}

	return 0;
}

int fairy_add_xp_by_sec(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	if (pstPlayer->stOnline.State& (CS_STAT_DEAD))
	{
		return 0;
	}

	if (fairy_get_camp(pstEnv, pstPlayer) == NULL)
	{
		return 0;
	}

	player_add_xp(pstEnv, pstPlayer, 2, 0,1);
	return 0;
}

int fairy_camp(ZONESVRENV* pstEnv, Player *pstPlayer, int iGridIdx, tdr_ulonglong ullWID)
{
	FairyOnline *pstFairyOnline = &pstPlayer->stOnline.stFairyOnline;
	ROLEFAIRY *pstRoleFairy = NULL;
	FAIRYDEF *pstFairyDef;
	ROLEPET *pstRolePet;
	
	if (!(pstPlayer->stOnline.State & CS_STAT_FAIRY_CAMPAIGN_PRE))
	{
		goto finish;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstFairyOnline->uiCampPreTime + 1 
		< FAIRY_CAMP_PRE_TIME) 
	{
		goto finish;
	}

	if (pstFairyOnline->ullWID != ullWID)
	{
		goto finish;
	}

	if (fairy_camp_check(pstEnv,pstPlayer,iGridIdx, ullWID) < 0)
	{
		goto finish;
	}
	
	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy )
	{
		goto finish;
	}

	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID,pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
	{
		goto finish;
	}

	set_fairy_camp(pstEnv, pstPlayer, pstRoleFairy);
	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY17,pstFairyDef->Name);

	// 通灵
	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if (pstRolePet)
	{
		pet_update_online(pstEnv, pstPlayer, pstRolePet);
	}

	// 精灵出征追踪
	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_STATUS_FAIRYCAMP_TRACE)
	{
		task_fairy_trace(pstEnv, pstPlayer, pstRoleFairy, TASK_OBJ_FAIRY_CAMP);
		//task_player_trace(pstEnv, pstPlayer, TASK_OBJ_FAIRY_CAMP);
	}
	
	goto finish;

finish:
	pstFairyOnline->uiCampPreTime = 0;
	pstFairyOnline->ullWID = 0;
	pstPlayer->stOnline.State &= ~CS_STAT_FAIRY_CAMPAIGN_PRE;
	return 0;
	
}


int fairy_evolve_stuff_count(FAIRYDEF *pstFairyDef, EVOLVESTUFF *pstStuff)
{
	/*int i,j;

	memset (pstStuff, 0, sizeof(EVOLVESTUFF)*EVOLVE_STAFF_SORT);
	for (i=0; i<EVOLVE_STAFF_SORT; i++)
	{
		if (pstFairyDef->EvolveStuffList[i].ItemID == 0 ||
			pstFairyDef->EvolveStuffList[i].ItemNum == 0)
		{
			break;
		}

		if (i==0)
		{
			pstStuff[0].ItemID = pstFairyDef->EvolveStuffList[i].ItemID;
			pstStuff[0].ItemNum = pstFairyDef->EvolveStuffList[i].ItemNum;
			continue;
		}

		for (j=0; j<EVOLVE_STAFF_SORT; j++)
		{
			if (pstStuff[j].ItemID == 0)
			{
				pstStuff[j].ItemID = pstFairyDef->EvolveStuffList[i].ItemID;
				pstStuff[j].ItemNum = pstFairyDef->EvolveStuffList[i].ItemNum;
				break;
			}
			if (pstFairyDef->EvolveStuffList[i].ItemID == pstStuff[j].ItemID)
			{
				pstStuff[j].ItemNum += pstFairyDef->EvolveStuffList[i].ItemNum;
				break;
			}
		}
		
	}*/
	return 0;	
	UNUSED(pstFairyDef);	//add by paraunused.pl
	UNUSED(pstStuff);	//add by paraunused.pl
}

int fairy_evolve_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID)
{
	int i;
	ROLEFAIRY *pstRoleFairy;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	FAIRYDEF *pstFairyDef;
//	EVOLVESTUFF stStuff[EVOLVE_STAFF_SORT];


	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy )
	{
		return -1;
	}

	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel) ;
	if (!pstFairyDef)
	{
		return -1;
	}

	if (faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel + 1) == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY18,pstFairyDef->Name);
		return -1;
	}

	if (pstPlayer->stRoleData.Level < pstFairyDef->EvolveLevelLimit)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY19,pstFairyDef->EvolveLevelLimit);
		return -1;
	}

	if (pstPlayer->stOnline.State & (CS_STAT_DEAD |CS_STAT_FIGHT))
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "炼化失败,角色当前状态无法炼化");
		//return -1;
	}

	if (pstRoleFairy->Stat & FAIRY_STAT_CAMP)
	{
		//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "(%s)精灵正在出征,无法炼化",pstFairyDef->Name);
		//return -1;
	}

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_EVOLVE)
		{
			FAIRYDEF *pstDef = faity_find(pstEnv, pstFairyPkg->RoleFairys[i].FairyID, pstFairyPkg->RoleFairys[i].FairyLevel);
			if(pstDef)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY20,pstDef->Name);
			}
			return -1;
		}
	}

	if (package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstFairyDef->UseMoney) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY21, pstFairyDef->Name, pstFairyDef->UseMoney, MONEY_TYPE_MONEY_NAME);
		return -1;
	}

	if (pstPlayer->stRoleData.RoleDetail.Exp < pstFairyDef->UseExp)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY22,pstFairyDef->Name,pstFairyDef->UseExp);
		return -1;
	}
	/*if ((unsigned int)pstRoleFairy->Exp < pstFairyDef->UseExp)
	{
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "炼化(%s)精灵需要注入%d经验",pstFairyDef->Name,pstFairyDef->UseExp);
		return -1;
	}*/
	
	return 0;
	UNUSED(iGridIdx);	//add by paraunused.pl
}


ROLEFAIRY * get_evolve_fairy(Player *pstPlayer)
{
	int i;
	ROLEFAIRY *pstRoleFairys = pstPlayer->stRoleData.Package.FairyPackage.RoleFairys;
	ROLEFAIRY *pstRoleFairy;

	for (i=0; i<pstPlayer->stRoleData.Package.FairyPackage.FairyNum; i++)
	{
		pstRoleFairy = pstRoleFairys + i;
	
		if (pstRoleFairy->Stat & FAIRY_STAT_EVOLVE)
		{
			return pstRoleFairy;
		}	
	}
	
	return NULL;
}

int fairy_evolve_use_item(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEFAIRY *pstRoleFairy = get_evolve_fairy(pstPlayer);
	unsigned int iDiffTm;

	if (!pstRoleFairy)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY23);
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, DEC_EVOLVE_TIME_ITEMID, 1, -1, NULL,PACKAGE_DEC_NONE) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY24);
		return -1;
	}

	if (pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM > (unsigned int)pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		iDiffTm = pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM - pstEnv->pstAppCtx->stCurr.tv_sec;
	}
	else
	{
		iDiffTm = 0;
	}

	if (iDiffTm > DEC_EVOLVE_TIME)
	{
		pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM -= DEC_EVOLVE_TIME;
	}
	else
	{
		pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	pstRoleFairy->EvolveLeftTM = pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM - pstEnv->pstAppCtx->stCurr.tv_sec;
	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, DEC_EVOLVE_TIME_ITEMID, 1, 
				-1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_FAIRYEVOLVE);
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_FAIRY25, DEC_EVOLVE_TIME/60);
	return 0;
}

int fairy_evolve(ZONESVRENV* pstEnv, Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID)
{
//	int i;
	ROLEFAIRY *pstRoleFairy;
	FAIRYDEF *pstFairyDef;
	int iRet;
	
	if (fairy_evolve_check(pstEnv, pstPlayer,iGridIdx, ullWID) < 0)
	{
		return -1;
	}

	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy )
	{
		return -1;
	}


	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel) ;
	if (!pstFairyDef)
	{
		return -1;
	}

	if (pstRoleFairy->Stat & FAIRY_STAT_CAMP)
	{
		//fairy_back_in(pstEnv, pstPlayer, pstRoleFairy);
	}

	pstRoleFairy->Stat |= FAIRY_STAT_EVOLVE;
	pstRoleFairy->EvolveLeftTM = pstFairyDef->EvolveTime * 60;
	pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM = pstEnv->pstAppCtx->stCurr.tv_sec + pstRoleFairy->EvolveLeftTM;
	pstRoleFairy->Exp = 0;
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);

	//pstPlayer->stRoleData.MiscInfo.CampFyID = 0;
	//pstPlayer->stRoleData.MiscInfo.CampFyXP = 0;

	
	pstPlayer->stRoleData.MiscInfo.Flag |= FAIRY_EVOLVE_ING;

	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstFairyDef->UseMoney);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_FAIRYEVOLVE, pstRoleFairy->FairyID, pstFairyDef->UseMoney-iRet, iRet, "精灵进化");
	player_dec_exp(pstEnv, pstPlayer, pstFairyDef->UseExp);

	z_role_fairy_oplog(pstEnv,pstPlayer,pstRoleFairy, OP_FAIRY_TYPE_UP_START,
						pstRoleFairy->StarLvl,NULL, pstFairyDef->UseMoney);

	return 0;
}

int fairy_evolve_stop(ZONESVRENV* pstEnv, Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID)
{
	/*ROLEFAIRY *pstRoleFairy;

	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy )
	{
		return -1;
	}

	if (pstPlayer->stOnline.State & (CS_STAT_DEAD |CS_STAT_FIGHT))
	{
		return -1;
	}
	
	if (pstRoleFairy->Stat != FAIRY_STAT_EVOLVE)
	{
		return -1;
	}

	pstRoleFairy->Stat = FAIRY_STAT_EVOLVE_STOP;
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
	pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_EVOLVE_ING;*/
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
	UNUSED(pstPlayer);	//add by paraunused.pl
	UNUSED(iGridIdx);	//add by paraunused.pl
	UNUSED(ullWID);	//add by paraunused.pl
}

ROLEFAIRY* get_pack_fairy_by_id(Player *pstPlayer, int iFairyID)
{
	ROLEFAIRY *pstRoleFairy = NULL;
	int i;

	for(i=0; i<pstPlayer->stRoleData.Package.FairyPackage.FairyNum; i++)
	{
		pstRoleFairy = &pstPlayer->stRoleData.Package.FairyPackage.RoleFairys[i];
		if (pstRoleFairy->FairyID == iFairyID)
		{
			return pstRoleFairy;
		}
	}

	return NULL;
	
}

int fairy_back_in(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy)
{
	pstRoleFairy->Stat |= FAIRY_STAT_BACK;
	pstRoleFairy->Stat &= ~FAIRY_STAT_CAMP;
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
	fairy_action(pstEnv, pstPlayer, pstRoleFairy, 1);
	fairy_del_skill(pstEnv,pstPlayer,pstRoleFairy);
	
	pstPlayer->stOnline.stFairyOnline.ullCampWID = 0;
	pstPlayer->stOnline.stFairyOnline.iDiathNum = 0;
	pstPlayer->stOnline.stFairyOnline.stFairyAttr.unBrkATk = 0;
	pstPlayer->stOnline.stFairyOnline.stFairyAttr.unMgcAtk = 0;
	pstPlayer->stOnline.stFairyOnline.stFairyAttr.unPhyAtk = 0;

	pstPlayer->stOnline.stFairyOnline.stFairyAttr.unFireATk = 0;
	pstPlayer->stOnline.stFairyOnline.stFairyAttr.unThunderATk = 0;
	pstPlayer->stOnline.stFairyOnline.stFairyAttr.unLightATk = 0;
	pstPlayer->stOnline.stFairyOnline.stFairyAttr.unNightATk = 0;
	pstPlayer->stOnline.stFairyOnline.stFairyAttr.unWaterATk = 0;
	
	pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_DIATH_TICK;
	pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_BEIHOULING;

	// 计算数值
	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);

       // 收回时清空XP
	fairy_camp_clean_xp(pstEnv, pstPlayer);
	return 0;
}

int fairy_back(ZONESVRENV* pstEnv, Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID)
{
	ROLEFAIRY *pstRoleFairy;
	FAIRYDEF *pstFairyDef;
	ROLEPET *pstRolePet;

	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy )
	{
		return -1;
	}

	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
	{
		return -1;
	}

	if (!(pstRoleFairy->Stat & FAIRY_STAT_CAMP))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY26, pstFairyDef->Name);
		return -1;
	}

	if (pstPlayer->stOnline.State & (CS_STAT_DEAD |CS_STAT_FIGHT))
	{
		//z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "角色当前状态不能召回精灵");
		//return -1;
	}

	fairy_back_in(pstEnv, pstPlayer, pstRoleFairy);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY27,pstFairyDef->Name);

	// 通灵
	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if (pstRolePet)
	{
		pet_update_online(pstEnv, pstPlayer, pstRolePet);
	}
	return 0;
	UNUSED(iGridIdx);	//add by paraunused.pl
}

int fairy_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy)
{
	FAIRYDEF *pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel +1);

	if (!pstFairyDef)
	{
		return -1;
	}

	pstRoleFairy->FairyLevel ++;
	pstRoleFairy->EvolveLeftTM = 0;
	pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM = 0;
	pstRoleFairy->Stat &= ~FAIRY_STAT_EVOLVE;

	//处于出征状态的精灵需要立即更新
	if(pstRoleFairy->Stat & FAIRY_STAT_CAMP)
	{	
		//清楚之前的技能
		fairy_del_skill(pstEnv,pstPlayer,pstRoleFairy);
		
		//升级后的技能、属性更新
		fairy_add_skill(pstEnv, pstPlayer, pstRoleFairy);
		fairy_up_online(pstEnv, pstPlayer, pstRoleFairy);
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	fairy_action(pstEnv, pstPlayer, pstRoleFairy, 2);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY28, 
					pstFairyDef->Name, pstRoleFairy->FairyLevel);

	//z_sys_strf(pstEnv, pstPlayer, SYS_COMBAT,"%s的等级提升到了%d级", 
	//			pstFairyDef->Name, pstRoleFairy->FairyLevel);
	
	z_role_fairy_oplog(pstEnv,pstPlayer,pstRoleFairy, OP_FAIRY_TYPE_UP_END,
						pstRoleFairy->StarLvl, NULL, 0);

	
	//成长之路
	{
		ACHIEVEMENTCOND stCond;
	
		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_FAIRY_LEVEL;
		stCond.Val1 = pstRoleFairy->FairyLevel;
		stCond.Val2 = pstRoleFairy->FairyID;
	
		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}
	return 0;
}



int fairy_diath_tick(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	FairyOnline *pstFairyOnline = &pstPlayer->stOnline.stFairyOnline;
	int i,j, iDiathID, iDiathLV;
	
	DIATHESISDEF *pstDiathDef;
//	SKILLRESULT stResult;

	if (!(pstPlayer->stRoleData.MiscInfo.Flag & FAIRY_DIATH_TICK))
	{
		return 0;
	}
	
	for (i=0; i<pstFairyOnline->iDiathNum; i++)
	{
		iDiathID = pstFairyOnline->FairyDiathList[i].DiathID;
		iDiathLV = pstFairyOnline->FairyDiathList[i].DiathLV;
		pstDiathDef = z_find_diathesis(pstEnv, iDiathID, iDiathLV);
		if (!pstDiathDef)
		{
			continue;
		}

		for (j=0; j<MAX_DIATHESIS_RESULT; j++)
		{
			if (pstDiathDef->DiathesisResult[j].SkillResultID == 0)
			{
				break;
			}
			if (pstDiathDef->DiathesisResult[j].SkillResultID == RESULT_ADD_EXP_ONTIME)
			{
				if (pstFairyOnline->FairyDiathList[i].CoolEnd >0 &&
					pstEnv->pstAppCtx->stCurr.tv_sec >= (int)pstFairyOnline->FairyDiathList[i].CoolEnd)
				{
					pstFairyOnline->FairyDiathList[i].CoolEnd = 
						pstEnv->pstAppCtx->stCurr.tv_sec +pstDiathDef->DiathesisResult[j].SkillResultVal1;
					
					player_add_exp(pstEnv, pstPlayer, pstDiathDef->DiathesisResult[j].SkillResultVal2, 0);
				}
				break;
			}

			if (pstDiathDef->DiathesisResult[j].SkillResultID == RESULT_ADD_XP_ONTIME)
			{
				if (pstFairyOnline->FairyDiathList[i].CoolEnd >0 &&
					pstEnv->pstAppCtx->stCurr.tv_sec >= (int)pstFairyOnline->FairyDiathList[i].CoolEnd)
				{
					pstFairyOnline->FairyDiathList[i].CoolEnd = 
						pstEnv->pstAppCtx->stCurr.tv_sec +pstDiathDef->DiathesisResult[j].SkillResultVal1;
					
					player_add_xp(pstEnv, pstPlayer, pstDiathDef->DiathesisResult[j].SkillResultVal2, 0,1);
				}
				break;
			}
		}

		
	}
	
	
	
	return 0;
}

int fairy_evolve_tick(ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	if (!(pstPlayer->stRoleData.MiscInfo.Flag & FAIRY_EVOLVE_ING))
	{
		return 0;
	}

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_EVOLVE )
		{
			if (pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM <= (unsigned int)pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				fairy_level_up(pstEnv, pstPlayer, &pstFairyPkg->RoleFairys[i]);
				fairy_chg_to_clt(pstEnv, pstPlayer,&pstFairyPkg->RoleFairys[i]);
				pstPlayer->stRoleData.MiscInfo.Flag &= ~FAIRY_EVOLVE_ING;
				//z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,"炼化成功");

				// 精灵等级追踪
				task_fairy_trace(pstEnv, pstPlayer, &pstFairyPkg->RoleFairys[i], TASK_OBJ_FAIRY_LV);
				//task_player_trace(pstEnv, pstPlayer, TASK_OBJ_FAIRY_LV);
				return 0;
			}
		}
	}
	return 0;
}

// 客户端主动打断吟唱
int fairy_camp_pre_brk_req(ZONESVRENV* pstEnv, Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID)
{
	ROLEFAIRY *pstRoleFairy;

	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy )
	{
		return -1;
	}

	if (!(pstPlayer->stOnline.State & CS_STAT_FAIRY_CAMPAIGN_PRE) ||
		pstPlayer->stOnline.stFairyOnline.ullWID != ullWID)
	{
		return -1;
	}

	pstPlayer->stOnline.stFairyOnline.uiCampPreTime = 0;
	pstPlayer->stOnline.stFairyOnline.ullWID = 0;
	pstPlayer->stOnline.State &= ~CS_STAT_FAIRY_CAMPAIGN_PRE;
	fairy_camp_pre_brk(pstEnv, pstPlayer, ullWID);
	return 0;
	UNUSED(iGridIdx);	//add by paraunused.pl
}

int fairy_view(ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEFAIRYVIEW *pstRoleFairyView)
{
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	pstRoleFairyView->FairyID = 0;

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_CAMP)
		{
			pstRoleFairyView->FairyID = pstFairyPkg->RoleFairys[i].FairyID;
			pstRoleFairyView->FairyLV = pstFairyPkg->RoleFairys[i].FairyLevel;
			break;
		}
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int fairy_evolve_logout(ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	int i;
	int iLeft;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	if (!(pstPlayer->stRoleData.MiscInfo.Flag & FAIRY_EVOLVE_ING))
	{
		return 0;
	}

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_EVOLVE )
		{
			iLeft = (int)pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM - pstEnv->pstAppCtx->stCurr.tv_sec;

			if (iLeft < 0)
			{
				iLeft = 0;
			}
			pstFairyPkg->RoleFairys[i].EvolveLeftTM = iLeft;
			
			break;
		}
			
	}
	return 0;
}


int fairy_evolve_update_time(ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	if (!(pstPlayer->stRoleData.MiscInfo.Flag & FAIRY_EVOLVE_ING))
	{
		return 0;
	}

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_EVOLVE )
		{
			
			pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM = 
					pstEnv->pstAppCtx->stCurr.tv_sec + pstFairyPkg->RoleFairys[i].EvolveLeftTM;
			
			break;
		}
			
	}
	return 0;
}


int fairy_evolve_update_lift_time(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;

	if (!(pstPlayer->stRoleData.MiscInfo.Flag & FAIRY_EVOLVE_ING))
	{
		return 0;
	}

	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_EVOLVE )
		{
			pstFairyPkg->RoleFairys[i].EvolveLeftTM = 0;
			if ((int)pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM>pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				pstFairyPkg->RoleFairys[i].EvolveLeftTM = 
							pstPlayer->stRoleData.MiscInfo.FairyEvolveEndTM-
							pstEnv->pstAppCtx->stCurr.tv_sec;
			}
			break;
		}
	}
	return 0;
}


int fairy_into_exp_check(ZONESVRENV* pstEnv, Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID, int iIntoExp)
{
	ROLEFAIRY *pstRoleFairy;
	FAIRYDEF *pstFairyDef;

	pstRoleFairy = get_pack_fairy(pstPlayer, ullWID);
	if (!pstRoleFairy)
	{
		return -1;
	}

	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
	{
		return -1;
	}

	if (pstRoleFairy->Stat & FAIRY_STAT_EVOLVE)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY29, pstFairyDef->Name);
		return -1;
	}

	
	if (iIntoExp <= 0 )
	{
		return -1;
	}
	
	if ((pstRoleFairy->Exp + iIntoExp) > (int)pstFairyDef->UseExp)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY30);
		return -1;
	}

	if (pstPlayer->stRoleData.RoleDetail.Exp < iIntoExp)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY31);
		return -1;
	}
	
	return 0;
	UNUSED(iGridIdx);	//add by paraunused.pl
}

int fairy_into_exp(ZONESVRENV* pstEnv, Player *pstPlayer,int iGridIdx, tdr_ulonglong ullWID, int iIntoExp)
{
	return 0;
	
	/*ROLEFAIRY *pstRoleFairy;
	FAIRYDEF *pstFairyDef;
	
	if (fairy_into_exp_check(pstEnv,pstPlayer,iGridIdx, ullWID,iIntoExp) < 0)
	{
		return -1;
	}

	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy )
	{
		return -1;
	}

	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel) ;
	if (!pstFairyDef)
	{
		return -1;
	}

	pstRoleFairy->Exp += iIntoExp;
	player_dec_exp(pstEnv, pstPlayer, iIntoExp);
	
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);

	z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE,"注入炼化经验成功");
	return 0;*/
	UNUSED(pstEnv);
	UNUSED(pstPlayer);
	UNUSED(&iGridIdx);
	UNUSED(&ullWID);
	UNUSED(&iIntoExp);
	
}


FAIRYSTUFFRATE * fairy_stuff_rate_get(FAIRYSTARSTUFFDEF *pstStuffDef, int iStar)
{
	int i,j;
	
	for (i=pstStuffDef->MinStar; i<=pstStuffDef->MaxStar; i++)
	{
		if (iStar == i)
		{
			for (j=0; j<FAIRY_MAX_STAR; j++)
			{
				if (	pstStuffDef->StuffRate[j].Rate == 0 || 
					pstStuffDef->StuffRate[j].Star == 0)
				{
					return NULL;
				}

				if (pstStuffDef->StuffRate[j].Star == i)
				{
					return &pstStuffDef->StuffRate[j];
				}
			}
		}
	}
	return NULL;
}

int fairy_star_up_check_stuff(ZONESVRENV* pstEnv, Player *pstPlayer,
						FAIRYSTARDEF *pstFairyStarDef,FAIRYSTARSTUFF *pstCltStuff)
{
	FAIRYSTARSTUFF *pstStarUpStuff = &pstFairyStarDef->StarUpStuff;

	// 主材料

	if (pstCltStuff->MainStuffNum <= 0)
	{
		return -1;
	}
	
	if (	pstStarUpStuff->MainStuffID != pstCltStuff->MainStuffID ||
		pstCltStuff->MainStuffNum < pstStarUpStuff->MainStuffNum)
	{
		return -1;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
		pstStarUpStuff->MainStuffID, pstCltStuff->MainStuffNum, -1,NULL,PACKAGE_DEC_NONE) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY32);
		return -1;
	}

	if ( 	pstStarUpStuff->Money > 0 &&
		package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstStarUpStuff->Money) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY33);
		return -1;
	}

	return 0;
}

int fairy_star_up_in(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEFAIRY *pstRoleFairy,
						FAIRYSTARDEF *pstFairyStarDef,int iStuffRate,FAIRYSTARSTUFF *pstCltStuff,FAIRYDEF *pstFairyDef)
{
	int iRate = iStuffRate;
	int iRand;
	int iLvl = pstRoleFairy->StarLvl;
	ITEMDEC stDec;
	int iBind = 0;
	int iRet;

	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstCltStuff->MainStuffID, pstCltStuff->MainStuffNum, 
						-1, NULL,PACKAGE_DEC_NONE))
	{
		return -1;
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, pstCltStuff->Money))
	{
		return -1;
	}

	iRand = RAND1(10000)+1;

	// 品质升级成功
	if (iRand < iRate)
	{
		pstRoleFairy->StarLvl++;
		fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY34,
					pstFairyDef->Name,pstRoleFairy->StarLvl);

		//强化后，完成相关任务
		task_fairy_star_trace(pstEnv, pstPlayer,pstRoleFairy->StarLvl);

		//成长之路
		{
			ACHIEVEMENTCOND stCond;

			memset(&stCond,0,sizeof(stCond));
			
			stCond.Type = COND_FAIRY_STRENGTHEN;
			stCond.Val1 = pstRoleFairy->StarLvl;
			stCond.Val2 = pstRoleFairy->FairyID;

			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
	}
	// 失败
	else
	{
		iRand = RAND1(10000);
		// 回退
		if (	iRand < pstFairyStarDef->StarBackRate * 100  &&
			pstRoleFairy->StarLvl > 1)
		{
			pstRoleFairy->StarLvl--;
			fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY35,
						pstFairyDef->Name,pstRoleFairy->StarLvl);
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY36, pstFairyDef->Name);
		}
	}

	iBind = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,pstCltStuff->MainStuffID, pstCltStuff->MainStuffNum, 
						-1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_FAIRYSTAR);
	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstCltStuff->Money);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_FAIRYSTAR, pstRoleFairy->FairyID, pstCltStuff->Money-iRet, iRet, "精灵强化");

	itemdec_fill(&stDec,pstCltStuff->MainStuffID,pstCltStuff->MainStuffNum, iBind);
	z_role_fairy_oplog(pstEnv,pstPlayer,pstRoleFairy, OP_FAIRY_TYPE_STAR_UP,iLvl,&stDec, pstCltStuff->Money);
	return 0;
}

int fairy_star_up(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEFAIRY *pstRoleFairy,FAIRYSTARSTUFF *pstCltStuff)
{
	FAIRYSTARDEF *pstFairyStarDef;
//	FAIRYSTARSTUFFDEF *pstStuffDef;
//	FAIRYSTUFFRATE *pstStuffRate;
//	int iLvl = 0;
	
	FAIRYDEF *pstFairyDef =  faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
		return -1;

#if 0
	if (pstPlayer->stOnline.State & (CS_STAT_DEAD ))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "您当前处于死亡状态，无法聚灵");
		return -1;
	}

	if (pstPlayer->stOnline.State & (CS_STAT_FIGHT))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, "您当前处于战斗状态，无法聚灵");
		return -1;
	}

	if (pstRoleFairy->Stat & FAIRY_STAT_CAMP)
	{	
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, "该精灵当前处于出征状态，请先收回后再聚灵");
		return -1;
	}
#endif

	if (pstPlayer->stOnline.State & (CS_STAT_STALL))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY10);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY11);
		return -1;
	}

	if (pstRoleFairy->StarLvl == FAIRY_MAX_STAR)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY12);
		return -1;
	}

	if (pstRoleFairy->StarLvl == pstFairyDef->MaxStar)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY13);
		return -1;
	}

	pstFairyStarDef = fairy_star_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->StarLvl);
	if (!pstFairyStarDef)
		return -1;

	if (pstCltStuff->MainStuffID !=pstFairyStarDef->StarUpStuff.MainStuffID ||
		pstCltStuff->MainStuffNum < pstFairyStarDef->StarUpStuff.MainStuffNum ||
		pstCltStuff->Money < pstFairyStarDef->StarUpStuff.Money)
	{
		return -1;
	}

//	iLvl = pstRoleFairy->StarLvl;
	fairy_star_up_in(pstEnv, pstPlayer,pstRoleFairy,pstFairyStarDef,pstFairyStarDef->StarSuccRate,pstCltStuff,pstFairyDef);

	// 调整数值
	if (pstRoleFairy->Stat & FAIRY_STAT_CAMP)
	{
		ROLEPET *pstPet = pet_get_camp(pstEnv, pstPlayer);
		if (pstPet)
		{
			pet_update_online(pstEnv, pstPlayer, pstPet);
		}
	}
		
//	z_role_fairy_oplog(pstEnv,pstPlayer,pstRoleFairy, OP_FAIRY_TYPE_STAR_UP,iLvl);
	return 0;
}

int fairy_star_up_req(ZONESVRENV* pstEnv, Player *pstPlayer, CSFAIRYCLT *pstFairyClt)
{
	FAIRYSTARSTUFF *pstCltStuff = &pstFairyClt->StarUpStuff;
	ROLEFAIRY *pstRoleFairy = get_pack_fairy(pstPlayer,pstFairyClt->WID);
	if (!pstRoleFairy )
		return -1;

	if (fairy_star_up(pstEnv, pstPlayer, pstRoleFairy,pstCltStuff) < 0)
		return -1;

	return 0;
}

ROLEFAIRY *fairy_get_camp(ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	int i;
	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	
	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].Stat & FAIRY_STAT_CAMP)
		{
			return &pstFairyPkg->RoleFairys[i];
		}
	}

	return NULL;
	UNUSED(pstEnv);
}

int fairy_is_tongling(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (!pet_get_camp(pstEnv, pstPlayer))
		return 0;

	if (!fairy_get_camp(pstEnv, pstPlayer))
		return 0;
	return 1;
}

int fairy_tongling_up_attr(ZONESVRENV* pstEnv, Player *pstPlayer, PetFightAttrOnline *pstPetFightAttrOnline, PetExpOnline *pstPetExpOnline)
{
	ROLEFAIRY *pstRoleFairy;
	FAIRYSTARDEF *pstFairyStarDef;

	pstRoleFairy = fairy_get_camp( pstEnv, pstPlayer);
	if (!pstRoleFairy)
		return -1;

	pstFairyStarDef = fairy_star_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->StarLvl);
	if (!pstFairyStarDef)
		return -1;

	// 宠物经验乘修正
	//pstPlayer->stOnline.stFitOneline.unPetExpRate+=pstFairyStarDef->PetExpMul;
	//pstPlayer->stOnline.stDyn.stHealChg.unPetExpMul += pstFairyStarDef->PetExpMul;
	if (pstPetExpOnline)
	{
		pstPetExpOnline->nExpMul += pstFairyStarDef->PetExpMul;
	}

	// 元素攻击
	if (pstFairyStarDef->AttrType == FAIRY_ATTR_FIRE)
	{
		pstPetFightAttrOnline->stAtkChg.unFireBaseArmAtk += pstFairyStarDef->Atk;
	}
	else if(pstFairyStarDef->AttrType == FAIRY_ATTR_LIGHT)
	{
		pstPetFightAttrOnline->stAtkChg.unLightBaseArmAtk += pstFairyStarDef->Atk;
	}
	else if(pstFairyStarDef->AttrType == FAIRY_ATTR_NIGHT)
	{
		pstPetFightAttrOnline->stAtkChg.unNightBaseArmAtk += pstFairyStarDef->Atk;
	}
	else if(pstFairyStarDef->AttrType == FAIRY_ATTR_THUNDER)
	{
		pstPetFightAttrOnline->stAtkChg.unThunderBaseArmAtk += pstFairyStarDef->Atk;
	}
	else if(pstFairyStarDef->AttrType == FAIRY_ATTR_WATER)
	{
		pstPetFightAttrOnline->stAtkChg.unWaterBaseArmAtk += pstFairyStarDef->Atk;
	}
	else
	{
		return -1;
	}

	
	return 0;
}


int fairy_clean_keyin_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;

	FAIRYPACKAGE *pstFairyPkg = &pstPlayer->stRoleData.Package.FairyPackage;
	
	for (i=0; i<pstFairyPkg->FairyNum; i++)
	{
		if (pstFairyPkg->RoleFairys[i].IsKeyin > 0)
		{
			pstFairyPkg->RoleFairys[i].IsKeyin = 0;
			fairy_chg_to_clt(pstEnv, pstPlayer,&pstFairyPkg->RoleFairys[i]);
		}
	}

	return 0;
}

int fairy_keyin_req(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong ullWID)
{
	ROLEFAIRY *pstRoleFairy;
	FAIRYDEF *pstFairyDef;
	int iRand;
	ROLEGRID stRoleGrid;
	int iBind = 0;
	ITEMDEC stDec;

	pstRoleFairy = get_pack_fairy(pstPlayer,ullWID);
	if (!pstRoleFairy)
		return -1;

	pstFairyDef = faity_find(pstEnv, pstRoleFairy->FairyID, pstRoleFairy->FairyLevel);
	if (!pstFairyDef)
		return -1;

	if (pstRoleFairy->IsKeyin > 0)
	{
		return -1;
	}
	
	if (pstFairyDef->PNum <= 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY14);
		return -1;
	}

	if ((int)pstPlayer->stRoleData.RoleDetail.XP < pstFairyDef->KeyinUseXp)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_FAIRY15,pstFairyDef->KeyinUseXp);
		return -1;
	}

	if (pstFairyDef->ZeroReelID > 0)
	{
		if (package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
								ZEROREELID, pstFairyDef->ZeroReelID, -1,NULL,PACKAGE_DEC_NONE) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_FAIRY16);
			return -1;
		}
	}

	iRand = RAND1(pstFairyDef->PNum);
	if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstFairyDef->PItems[iRand]))
	{
		if (package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0) == -1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOSPACE);
			return -1;
		}			
	}
	else
	{
		return -1;
	}

	if (pstFairyDef->ZeroReelID > 0)
	{
		iBind = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, ZEROREELID, pstFairyDef->ZeroReelID, 
					-1,NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_FAIRYKEYIN);
	}
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0, IMPITEM_OPTYPE_FAIRYKEYIN);

	player_add_xp(pstEnv, pstPlayer, pstFairyDef->KeyinUseXp, 1,1);

	pstRoleFairy->IsKeyin = 1;
	fairy_chg_to_clt(pstEnv, pstPlayer,pstRoleFairy);

	itemdec_fill(&stDec,ZEROREELID, pstFairyDef->ZeroReelID, iBind);
	z_role_fairy_oplog(pstEnv,pstPlayer,pstRoleFairy, OP_FAIRY_TYPE_KEYIN,pstRoleFairy->StarLvl,&stDec, 0);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_FAIRY_MARK;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}
	return 0;
}

int player_fairy_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	
	CSFAIRYCLT *pstFairyClt = &pstCsPkg->Body.FairyClt;
	UNUSED(pstFrameHead);

	if (pstFairyClt->OpType == OP_CAMP_PRE)
	{
		fairy_camp_pre(pstEnv, pstPlayer,pstFairyClt->Grididx, pstFairyClt->WID);
	}
	else if(pstFairyClt->OpType == OP_CAMP)
	{
		fairy_camp(pstEnv, pstPlayer,pstFairyClt->Grididx, pstFairyClt->WID);
	}
	else if(pstFairyClt->OpType == OP_EVOLVE)
	{
		fairy_evolve(pstEnv, pstPlayer,pstFairyClt->Grididx, pstFairyClt->WID);
	}
	else if(pstFairyClt->OpType == OP_BACK)
	{
		fairy_back(pstEnv, pstPlayer,pstFairyClt->Grididx, pstFairyClt->WID);
	}
	else if(pstFairyClt->OpType == OP_DROP)
	{
		
	}
	else if(pstFairyClt->OpType == OP_EVOLVE_STOP)
	{
		
	}
	else if(pstFairyClt->OpType == OP_CAMP_BRK)
	{
		fairy_camp_pre_brk_req(pstEnv, pstPlayer,pstFairyClt->Grididx, pstFairyClt->WID);
	}
	else if (pstFairyClt->OpType == OP_INTO_EXP)
	{
		fairy_into_exp(pstEnv, pstPlayer,pstFairyClt->Grididx, pstFairyClt->WID, pstFairyClt->IntoExp);
	}
	else if (pstFairyClt->OpType == OP_STAR_UP)
	{
		fairy_star_up_req(pstEnv, pstPlayer, pstFairyClt);
	}
	else if(pstFairyClt->OpType == OP_KEYIN)
	{
		fairy_keyin_req(pstEnv, pstPlayer, pstFairyClt->WID);
	}
	else if(pstFairyClt->OpType == OP_DEC_EVOLVE_TIME)
	{
		fairy_evolve_use_item(pstEnv, pstPlayer);
	}
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}
