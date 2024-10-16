/*
**  @file $RCSfile: zone_pet.c,v $
**  general description of this module
**  $Id: zone_pet.c,v 1.512 2014/05/29 13:10:59 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/05/29 13:10:59 $
**  @version $Revision: 1.512 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_pet.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_skill.h"
#include "zone_move.h"
#include "zone_combat.h"
#include "mapmask.h"
#include "zone_map.h"
#include "zone_package.h"
#include "libdirty.h"
#include "zone_oplog.h"
#include "zone_status.h"
#include "zone_db.h"
#include "proto_comm.h"
#include "zone_task.h"
#include "zone_range.h"
#include "zone_player.h"
#include "zone_stall.h"
#include "zone_battle.h"
#include "zone_safelock.h"
#include "zone_machine.h"
#include "zone_arm.h"
#include "zone_shop.h"
#include "zone_span.h"
#include "zone_err.h"
#include "zone_zh_name.h"
#include "zone_clan_pet.h"

#define PLAYER_PET_HETI_LEVEL 60
#define PET_S_LAST 1.36f
#define PET_MAX_WENBAO 100

int pet_heti_start(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEPET *pstRolePet, int iUpAttr);
int pet_heti_end(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEPET *pstRolePet, int iUpAttr);
ROLEPET * pet_find_heti(Player *pstPlayer);
int pet_strengthen_val_check(ROLEPET *pstRolePet);

PETDEF *z_find_pet_def(ZONESVRENV *pstEnv, int iPetID, int *pIidx)
{
	return (PETDEF *)z_find_def(pstEnv->pstZoneObj->sPetDef, iPetID, PET_DATA, 
									pstEnv->pstZoneObj->iPetDef, pIidx);
}

PETGENIUSDEF *z_find_pet_pinzhi_def(ZONESVRENV *pstEnv, int iPinZhi, int *pIidx)
{
	return (PETGENIUSDEF *)z_find_def(pstEnv->pstZoneObj->sPetGeniusDef, iPinZhi, PET_GENIUS_DEF_DATA, 
									pstEnv->pstZoneObj->iPetGeniusDef, pIidx);
}

PETWASHGENIUSDEF *z_find_pet_wash_genius_def(ZONESVRENV *pstEnv, int iImportant, int iPinZhi)
{
	PETWASHGENIUSDEF stDef;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Important = iImportant;
	stDef.PinZhi = iPinZhi;

	iIndex = bsearch_int(&stDef, pstObj->sPetWashGeniusDef, pstObj->iPetWashGeniusDef, PET_WASH_GENIUS_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETWASHGENIUSDEF *)pstObj->sPetWashGeniusDef[iIndex];
	}

	return NULL;
}

PETLEVELDEF* z_find_pet_level_def(ZONESVRENV *pstEnv, int iLevel, int *pIidx)
{
	return (PETLEVELDEF *)z_find_def(pstEnv->pstZoneObj->sPetLevelDef, iLevel, PETLEVEL_DATA, 
									pstEnv->pstZoneObj->iPetLevelDef, pIidx);
}

PETTRAITDEF *z_find_pet_trait_def(ZONESVRENV *pstEnv, int iPetTraitID, int iStar)
{
	PETTRAITDEF stDef;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.TraitID = iPetTraitID;
	stDef.Star = iStar;

	iIndex = bsearch_int(&stDef, pstObj->sPetTraitDef, pstObj->iPetTraitDef, PET_TRAIT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETTRAITDEF *)pstObj->sPetTraitDef[iIndex];
	}

	return NULL;
}

PETSTRENGTHENDEF *z_find_pet_strengthen_def(ZONESVRENV *pstEnv, int iLevel)
{
	PETSTRENGTHENDEF  stStrengthen;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stStrengthen.Level= iLevel;
	iIndex = bsearch_int(&stStrengthen, pstObj->sPetStrengthenDef, pstObj->iPetStrengthenDef, PET_STRENGTHEN_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETSTRENGTHENDEF *)pstObj->sPetStrengthenDef[iIndex];
	}
	
	return NULL;	
}

PETRESOLVEDEF *z_find_pet_resolve_def(ZONESVRENV *pstEnv, int iResolveType, int iVal)
{
	PETRESOLVEDEF stResolve;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stResolve.ResolveType = iResolveType;
	stResolve.Val= iVal;
	
	iIndex = bsearch_int(&stResolve, pstObj->sPetResolveDef, pstObj->iPetResolveDef, PET_RESOLVE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETRESOLVEDEF *)pstObj->sPetResolveDef[iIndex];
	}
	
	return NULL;	
}

PETSKILLGRIDDEF *z_find_pet_skillgrid_def(ZONESVRENV *pstEnv, int iSkillGrid)
{
	PETSKILLGRIDDEF stSkillGrid;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stSkillGrid.SkillGrid = iSkillGrid;
	iIndex = bsearch_int(&stSkillGrid, pstObj->sPetSkillGridDef, pstObj->iPetSkillGridDef, PET_SKILL_GRID_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETSKILLGRIDDEF *)pstObj->sPetSkillGridDef[iIndex];
	}
	
	return NULL;	
}

PETFUSEDEF *z_find_pet_fuse_def(ZONESVRENV *pstEnv, int iGenius1, int iGenius2)
{
	PETFUSEDEF stFuse;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stFuse.Genius1 = iGenius1;
	stFuse.Genius2 = iGenius2;
	
	iIndex = bsearch_int(&stFuse, pstObj->sPetFuseDef, pstObj->iPetFuseDef, PET_FUSE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETFUSEDEF *)pstObj->sPetFuseDef[iIndex];
	}
	
	return NULL;	
}

int pet_fight_val(ZONESVRENV* pstEnv, ROLEPET *pstRolePet, PETDEF *pstPetDef)
{
	int iPhsicAtk;
	int iHP, iFlee, iHitRate;
	int iDef, iIceDef;
	PETLEVELDEF *pstPetLevelDef;
	int iStrengthenVal = pet_strengthen_val_get(pstRolePet);

	pstPetLevelDef = z_find_pet_level_def(pstEnv, pstRolePet->Level, NULL);
	if(NULL == pstPetLevelDef)
	{
		return -1;
	}

	//物理攻击
	iPhsicAtk = pstPetLevelDef->PhsicAtk*(pstRolePet->PhsicAtkGenius*(100+iStrengthenVal)/100.0)/1000.0;
	//魔法防御
	iIceDef = pstPetLevelDef->IceDef*(pstRolePet->IceDefGenius*(100+iStrengthenVal)/100.0)/1000.0;
	//物理防御
	iDef = pstPetLevelDef->PhsicDef*(pstRolePet->DefGenius*(100+iStrengthenVal)/100.0)/1000.0;
	//闪避
	iFlee = pstPetLevelDef->Flee*(0.8+pstRolePet->FleeGenius*(100+iStrengthenVal)/100.0/5000.0);
	//命中 
	iHitRate = pstPetLevelDef->HitRate*(0.8+pstRolePet->HitRateGenius*(100+iStrengthenVal)/100.0/5000.0);	
	//血量
	iHP = pstPetLevelDef->HP*(pstRolePet->HPGenius*(100+iStrengthenVal)/100.0)/1000.0;
	
	return ((iPhsicAtk*15) + iHP + (iIceDef*2.5) + (iDef*2.5) + (iHitRate*4) + (iFlee*5))/10;

	UNUSED(pstPetDef);
}


int pet_shi_get_free_fightpos(PetOnline *pstPetOnline)
{
	int i;
	char sMap[PET_MAX_FIGHT_SHI];

	memset(sMap, 0, sizeof(sMap));
	for (i=0; i<pstPetOnline->bFightShiNum; i++)
	{
		if (pstPetOnline->astFightShi[i].iXieDaiPos >=0 && pstPetOnline->astFightShi[i].iXieDaiPos < PET_MAX_FIGHT_SHI)
		sMap[(int)pstPetOnline->astFightShi[i].iXieDaiPos] = 1;
	}

	for (i=0; i<PET_MAX_FIGHT_SHI; i++)
	{
		if (0 == sMap[i])
		{
			return i;
		}
	}

	return -1;
}

void pet_load_online_data(ZONESVRENV *pstEnv, RolePetOnline *pstRolePetOnline, ROLEPET *pstRolePet, PETDEF *pstPetDef)
{
	pstRolePetOnline->ullWID = pstRolePet->WID;
	pstRolePetOnline->unDefID = pstRolePet->PetID;
	pstRolePetOnline->iDefIdx = -1;
	pstRolePetOnline->bPetType = pstPetDef->Type;
	pstRolePetOnline->cPackageIdx = -1;
		
	return;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int  get_pet_shi_xiedaipos(ZONESVRENV *pstEnv, Player * pstPlayer, ROLEPET* pstRolePet)
{
	PETDEF *pstPetDef;
	PetFightShiOnline *pstPetFightShiOnline;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return -1;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		pstPetFightShiOnline = (PetFightShiOnline *) pet_find_online(pstPlayer, pstRolePet);
		if (pstPetFightShiOnline)
		{
			return pstPetFightShiOnline->iXieDaiPos;
		}
	}

	return -1;
}

int  get_pet_tishen(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	int iTishen = 0;
	STATUSRESULT stResult;

	iTishen = pstRolePet->FightAttr.YongQi;
		
	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		z_get_pet_status_result(pstEnv, pstPlayer, pstRolePet, STATUS_RESULT_PET_TISHEN, &stResult);
		iTishen += stResult.ResultVal1;
	}	
	
	if(iTishen < 0)
	{
		iTishen = 0;
	}

	return iTishen;
}

RolePetOnline *pet_insert_online(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	PETDEF *pstPetDef;
	RolePetOnline *pstRolePetOnline;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return NULL;
	}

	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		
		if (pstPetOnline->bFightShiNum >= PET_MAX_FIGHT_SHI)	
		{
			return NULL;
		}
		
		memset(&pstPetOnline->astFightShi[pstPetOnline->bFightShiNum], 0, sizeof(pstPetOnline->astFightShi[0]));

		pstPetOnline->astFightShi[pstPetOnline->bFightShiNum].iXieDaiPos = pet_shi_get_free_fightpos(pstPetOnline);
		STRNCPY(pstPetOnline->astFightShi[pstPetOnline->bFightShiNum].szName, pstRolePet->Name, 
					sizeof(pstPetOnline->astFightShi[0].szName));
		pstRolePetOnline = &pstPetOnline->astFightShi[pstPetOnline->bFightShiNum++].stRolePetOnline;
	}
	else
	{
		if (pstPetOnline->bBackNum >= (int)(sizeof(pstPetOnline->astBack)/sizeof(pstPetOnline->astBack[0])))
		{
			return NULL;
		}

		memset(&pstPetOnline->astBack[pstPetOnline->bBackNum], 0, sizeof(pstPetOnline->astBack[0]));

		pstRolePetOnline = &pstPetOnline->astBack[pstPetOnline->bBackNum++].stRolePetOnline;
	}

	pet_load_online_data(pstEnv, pstRolePetOnline, pstRolePet, pstPetDef);
	
	return pstRolePetOnline;
}

PetFightShiOnline *pet_find_fight_online(Player *pstPlayer, ROLEPET *pstRolePet)
{
	int i;
	RolePetOnline *pstRolePetOnline;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	
	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		for (i=0; i<pstPetOnline->bFightShiNum; i++)
		{
			pstRolePetOnline = &pstPetOnline->astFightShi[i].stRolePetOnline;
			
			if (pstRolePetOnline->ullWID == pstRolePet->WID)
			{
				return &pstPetOnline->astFightShi[i];
			}
		}
	}

	return NULL;
}

RolePetOnline *pet_find_online(Player *pstPlayer, ROLEPET *pstRolePet)
{
	int i;
	RolePetOnline *pstRolePetOnline;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	
	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		for (i=0; i<pstPetOnline->bFightShiNum; i++)
		{
			pstRolePetOnline = &pstPetOnline->astFightShi[i].stRolePetOnline;
			
			if (pstRolePetOnline->ullWID == pstRolePet->WID)
			{
				return pstRolePetOnline;
			}
		}
	}
	else
	{
		for (i=0; i<pstPetOnline->bBackNum; i++)
		{
			pstRolePetOnline = &pstPetOnline->astBack[i].stRolePetOnline;
			
			if (pstRolePetOnline->ullWID == pstRolePet->WID)
			{
				return pstRolePetOnline;
			}
		}
	}
		
	return NULL;
}

int pet_update_online(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	RolePetOnline *pstRolePetOnline;
		
	pstRolePetOnline = pet_find_online(pstPlayer, pstRolePet);
	if (NULL == pstRolePetOnline)
	{
		pstRolePetOnline = pet_insert_online(pstEnv, pstPlayer, pstRolePet);
		if (NULL == pstRolePetOnline)
		{
			return -1;
		}
	}

	pet_attr_update(pstEnv, pstPlayer, pstRolePet, pstRolePetOnline);

	return 0;
}

PetTiShenOnline *pet_find_tishenonline_byonline(Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline)
{
	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		return &((PetFightShiOnline *)pstRolePetOnline)->stPetTiShenOnline; 
	}
	else
	{
		return &((PetBackOnline *)pstRolePetOnline)->stPetTiShenOnline;
	}
	
	return NULL;
	UNUSED(pstPlayer);	//add by paraunused.pl
}

PetPowerOnline *pet_find_poweronline_byonline(Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline)
{
	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		return &((PetFightShiOnline *)pstRolePetOnline)->stPetPowerOnline; 
	}
	else
	{
		return &((PetBackOnline *)pstRolePetOnline)->stPetPowerOnline;
	}
	
	return NULL;
	UNUSED(pstPlayer);	//add by paraunused.pl
}

PetFightAttrOnline *pet_find_fightattronline_byonline(Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline)
{
	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		return &((PetFightShiOnline *)pstRolePetOnline)->stPetFightAttrOnline; 
	}
	else
	{
		return &((PetBackOnline *)pstRolePetOnline)->stPetFightAttrOnline; 
	}
		
	return NULL;
	UNUSED(pstPlayer);	//add by paraunused.pl
}

PetFightAttrOnline *pet_find_fightattronline(Player *pstPlayer, ROLEPET *pstRolePet)
{
	RolePetOnline *pstRolePetOnline;

	pstRolePetOnline = pet_find_online(pstPlayer, pstRolePet);
	if (NULL == pstRolePetOnline)
	{
		return NULL;
	}

	return pet_find_fightattronline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
}
	
PetPowerOnline *pet_find_poweronline(Player *pstPlayer, ROLEPET *pstRolePet)
{
	RolePetOnline *pstRolePetOnline;
	
	pstRolePetOnline = pet_find_online(pstPlayer, pstRolePet);
	if (NULL == pstRolePetOnline)
	{
		return NULL;
	}

	return pet_find_poweronline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
}

PetExpOnline *pet_find_exponline_byonline(Player *pstPlayer, ROLEPET *pstRolePet, RolePetOnline *pstRolePetOnline)
{
	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		return &((PetFightShiOnline *)pstRolePetOnline)->stPetExpOnline; 
	}
	else
	{
		return &((PetBackOnline *)pstRolePetOnline)->stPetExpOnline; 
	}
	
	return NULL;
	UNUSED(pstPlayer);	//add by paraunused.pl
}

PetExpOnline *pet_find_exponline(Player *pstPlayer, ROLEPET *pstRolePet)
{
	RolePetOnline *pstRolePetOnline;
	
	pstRolePetOnline = pet_find_online(pstPlayer, pstRolePet);
	if (NULL == pstRolePetOnline)
	{
		return NULL;
	}

	return pet_find_exponline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
}

int pet_level_up_action(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	int iXieDaiPos;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	iXieDaiPos = get_pet_shi_xiedaipos(pstEnv, pstPlayer, pstRolePet);
	if (iXieDaiPos < 0)
	{
		return -1;
	}

	pstActionRes->ID =  pstPlayer->iMemID+(iXieDaiPos+1)*PET_ID_BASE;
	pstActionRes->ActionID = ACTION_LEVEL;

	pstActionRes->Data.Level = pstRolePet->Level;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int pet_hp_action(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iOldHP)
{
	int iXieDaiPos;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	iXieDaiPos = get_pet_shi_xiedaipos(pstEnv, pstPlayer, pstRolePet);
	if (iXieDaiPos < 0)
	{
		return -1;
	}

	memset(&pstActionRes->Data.HPChg, 0, sizeof(pstActionRes->Data.HPChg));
	
	pstActionRes->ID =  pstPlayer->iMemID+(iXieDaiPos+1)*PET_ID_BASE;
	pstActionRes->ActionID = ACTION_HP_CHG;

	pstActionRes->Data.HPChg.ChgHP = pstRolePet->HP - iOldHP;
	pstActionRes->Data.HPChg.CurrHP = pstRolePet->HP;
	pstActionRes->Data.HPChg.CurrMaxHP = pstRolePet->MaxHP;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int pet_add_power(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEPET *pstRolePet,  int iPower)
{
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETATTRCHG *pstPetChgData = &pstPetSvr->PetSvrData.AttrChg;
	unsigned int iOldPower;
	PETDEF *pstPetDef;
	long long llPower;
	
	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	llPower = iOldPower = pstRolePet->HP;
	llPower += iPower;
	if (llPower > pstRolePet->MaxHP)
	{
		pstRolePet->HP = pstRolePet->MaxHP;
	}
	else if (llPower < 0)
	{
		pstRolePet->HP = 0;
	}
	else
	{
		pstRolePet->HP = llPower;
	}

	if (iOldPower == pstRolePet->HP)
	{
		return -1;
	}

	pet_hp_action(pstEnv, pstPlayer, pstRolePet, iOldPower);
	
	pstPetSvr->PetSvrType = PET_ATTR_CHG;
	
	pstPetChgData->Wid = pstRolePet->WID;
	pstPetChgData->Type = PET_CHG_HP;

	pstPetChgData->AttrValue.ChgHP.HP = pstRolePet->HP;
	pstPetChgData->AttrValue.ChgHP.XieDaiPos = get_pet_shi_xiedaipos(pstEnv, pstPlayer, pstRolePet);

	//if (pstRolePet->Power == 0 && pstPetDef->Type == PET_TYPE_SHIWEI)
	//{
	//	pet_set_stat(pstEnv, pstPlayer, pstRolePet, 0);
	//}
	
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

ROLEPET *z_find_role_pet(Player *pstPlayer, tdr_ulonglong ullWID, int *piIdx)
{
	int i;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	
	if (ullWID <= 0)
		return NULL;

	for (i = 0; i < pstPetPackage->Num; i++)
	{
		if  (ullWID == pstPetPackage->RolePets[i].WID )

		{
			if (piIdx) 	*piIdx = i;
			
			return (ROLEPET *)&pstPetPackage->RolePets[i];
		}
	}	
	return NULL;	
}

int pet_xiedai_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, PETDEF *pstPetDef, int iStat)
{
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

	if ((int)pstRolePet->Level - (int)pstPlayer->stRoleData.Level > PET_LEVEL_LIMIT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
				ZONEERR_PET161, pstRolePet->Name);
		return -1;
	}

	if (PET_STAT_FIGHT == iStat && pstRolePet->LoveVal < PET_FIGHT_LOVEVAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
				ZONEERR_PET101);
		return -1;
	}

	if (0 != pstPetDef->FightLevel && pstPlayer->stRoleData.Level < pstPetDef->FightLevel)
	{

		z_send_sys_str_msg(pstEnv->pstAppCtx,  pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET102, 
				pstRolePet->Name, pstPetDef->FightLevel);
					
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	
	if (pstPetOnline->bFightShiNum >= PET_MAX_FIGHT_SHI)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PET103);
		return -1;
	}
	
	return 0;
}

int pet_del_online(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	int i;
	RolePetOnline *pstRolePetOnline;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	
	if (pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		for (i=0; i<pstPetOnline->bFightShiNum; i++)
		{
			pstRolePetOnline = &pstPetOnline->astFightShi[i].stRolePetOnline;
			
			if (pstRolePetOnline->ullWID == pstRolePet->WID)
			{
				pstPetOnline->bFightShiNum--;
				if (i != pstPetOnline->bFightShiNum)
				{
					pstPetOnline->astFightShi[i] = pstPetOnline->astFightShi[pstPetOnline->bFightShiNum];
				}
				
				return 0;
			}
		}

	}
	else
	{
		for (i=0; i<pstPetOnline->bBackNum; i++)
		{
			pstRolePetOnline = &pstPetOnline->astBack[i].stRolePetOnline;
			
			if (pstRolePetOnline->ullWID == pstRolePet->WID)
			{
				pstPetOnline->bBackNum--;
				if (i != pstPetOnline->bBackNum)
				{
					pstPetOnline->astBack[i] = pstPetOnline->astBack[pstPetOnline->bBackNum];
				}
			}
		}
	}
	
	return -1;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int pet_shi_skill_addone(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iSkillID, int iSkillLevel, int fNormalSkill)
{
	int iTmp;
	PetFightShiOnline *pstPetFightShiOnline;
	PetZoneSkill *pstPetZoneSkill;
	SKILLDEF *pstSkillDef;
	DIATHESISDEF *pstDiathesisDef;

	pstPetFightShiOnline = (PetFightShiOnline *)pet_find_online(pstPlayer, pstRolePet);
	if (NULL == pstPetFightShiOnline)
	{
		return -1;
	}

	if (pstPetFightShiOnline->bSkillNum >= MAX_PET_SKILL)
	{
		return -1;
	}

	pstPetZoneSkill = &pstPetFightShiOnline->astPetZoneSkills[pstPetFightShiOnline->bSkillNum];
	pstPetZoneSkill->SkillID = iSkillID;
	pstPetZoneSkill->SkillLevel = iSkillLevel;
	
	//出生技能
	if (fNormalSkill)
	{
		pstSkillDef = z_find_skill_idx(pstEnv, pstPetZoneSkill->SkillID, pstPetZoneSkill->SkillLevel , &pstPetZoneSkill->iSkillDefIdx);
		if (NULL == pstSkillDef)
		{
			return -1;
		}
		
		iTmp = pstSkillDef->SelfCool*100.0;
		pstPetZoneSkill->iSkillType = PET_SKILL_ATK_NORMAL;
	}
	//攻击类素质
	else
	{
		SKILLRESULT *pstResult;
		
		pstDiathesisDef = z_find_diathesis(pstEnv, pstPetZoneSkill->SkillID, pstPetZoneSkill->SkillLevel);
		if(NULL == pstDiathesisDef)
		{
			return -1;
		}

		pstResult = z_diathesis_def_get_result(pstDiathesisDef, RESULT_PET_ADD_SKILL);
		if(NULL == pstResult)
		{
			return -1;
		}

		pstSkillDef = z_find_skill_idx(pstEnv, pstResult->SkillResultVal1, pstResult->SkillResultVal2, &pstPetZoneSkill->iSkillDefIdx);
		if (NULL == pstSkillDef)
		{
			return -1;
		}
		
		iTmp = pstSkillDef->SelfCool*100.0;

		pstPetZoneSkill->iSkillType = pstResult->SkillResultVal3;
	}

	TV_ADD_MS(pstPetZoneSkill->stCoolEnd, pstEnv->pstAppCtx->stCurr, iTmp);	

	pstPetFightShiOnline->bSkillNum++;
	
	return 0;
}

int pet_update_skill_sort(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	int i, j;
	PetFightShiOnline *pstPetFightShiOnline;
	DIATHESISDEF *pstDiathesisDef;
	PetZoneSkill *pstPetZoneSkill;
	int iTmp;
	
	if (0 == (PET_STAT_FIGHT & pstRolePet->StatFlag))
	{
		return -1;
	}
	 
	pstPetFightShiOnline = (PetFightShiOnline *)pet_find_online(pstPlayer, pstRolePet);
	if (NULL == pstPetFightShiOnline)
	{
		return -1;
	}

	//直接排序
	for (i=0; i<pstPetFightShiOnline->bSkillNum; i++)
	{
		pstPetZoneSkill = &pstPetFightShiOnline->astPetZoneSkills[i];

		pstDiathesisDef = z_find_diathesis(pstEnv, pstPetZoneSkill->SkillID, pstPetZoneSkill->SkillLevel);
		if(NULL == pstDiathesisDef)
		{
			return -1;
		}
		
		iTmp = pstDiathesisDef->SelfCool;
		
		for (j=i; j<pstPetFightShiOnline->bSkillNum; j++)
		{

			pstDiathesisDef = z_find_diathesis(pstEnv, pstPetFightShiOnline->astPetZoneSkills[j].SkillID, 
												pstPetFightShiOnline->astPetZoneSkills[j].SkillLevel);
			if(NULL == pstDiathesisDef)
			{
				return -1;
			}
			
			if (iTmp < pstDiathesisDef->SelfCool)
			{
				PetZoneSkill stTmpSkill;

				memcpy(&stTmpSkill, pstPetZoneSkill, sizeof(stTmpSkill));
				memcpy(pstPetZoneSkill, &pstPetFightShiOnline->astPetZoneSkills[j], sizeof(pstPetZoneSkill));
				memcpy(&pstPetFightShiOnline->astPetZoneSkills[j], &stTmpSkill, sizeof(pstPetFightShiOnline->astPetZoneSkills[j]));
			}
		}	
	}
	
	return 0;
}

int pet_update_skill(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, PETDEF *pstPetDef)
{
	int i;
	
	//宠物本身就有的技能
	pet_shi_skill_addone(pstEnv, pstPlayer, pstRolePet, pstPetDef->NormalSkill, 1, 1);

	//宠物素质
	for(i = 0; i< pstRolePet->PetSkills.SkillGridNum; i++)
	{
		//攻击类的技能
		//if (pstRolePet->PetSkills.SkillGrid[i].GridType != PET_SKILL_ATK)
		//{
		//	continue;
		//}
		
		if (pstRolePet->PetSkills.SkillGrid[i].SkillID == 0)
		{
			continue;
		}

		pet_shi_skill_addone(pstEnv, pstPlayer, pstRolePet, pstRolePet->PetSkills.SkillGrid[i].SkillID, pstRolePet->PetSkills.SkillGrid[i].SkillLevel, 0);
	}

	pet_update_skill_sort(pstEnv, pstPlayer, pstRolePet);
	
	return 0;
}

int pet_fight_addskill(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, PETSKILLGRID *pstPetSkillOne)
{
        PETDEF *pstPetDef;

	if (0 == (PET_STAT_FIGHT & pstRolePet->StatFlag))
	{
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return -1;
	}

	/*
	//非 攻击类的技能
	if (pstPetSkillOne->GridType != PET_SKILL_ATK)
	{
		return -1;
	}
	*/

	if (0 > pet_shi_skill_addone(pstEnv, pstPlayer, pstRolePet, pstPetSkillOne->SkillID, pstPetSkillOne->SkillLevel, 0))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pet  add skill fail");
		return -1;
	}

	pet_update_skill_sort(pstEnv, pstPlayer, pstRolePet);
	
	return 0;
}

int pet_fight_skill_upone(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, PETSKILLGRID*pstPetSkillOne)
{
	int i;
	PetFightShiOnline *pstPetFightShiOnline;

	pstPetFightShiOnline = (PetFightShiOnline *)pet_find_online(pstPlayer, pstRolePet);
	if (NULL == pstPetFightShiOnline)
	{
	        return -1;
	}

	for (i=0; i<pstPetFightShiOnline->bSkillNum; i++)
	{
	        if (pstPetFightShiOnline->astPetZoneSkills[i].SkillID == pstPetSkillOne->SkillID)
	        {
	                pstPetFightShiOnline->astPetZoneSkills[i].SkillLevel= pstPetSkillOne->SkillLevel;
	                return 0;
	        }
	}

	return -1;
	UNUSED(pstEnv);	//add by paraunused.pl
}

static void pet_fill_diathesis_view(const ROLEPET *pstPet, CSPETVIEW *pstView)
{
	int i = 0;
	const PETSKILL *pstSkill = &pstPet->PetSkills;

	pstView->DiathesisNum = 0;
	for(i = 0; i < pstSkill->SkillGridNum; ++i)
	{
		if(pstSkill->SkillGrid[i].SkillID)
		{
			pstView->Diathesis[(int)pstView->DiathesisNum].ID = 
				pstSkill->SkillGrid[i].SkillID;
			pstView->Diathesis[(int)pstView->DiathesisNum].Level =
				pstSkill->SkillGrid[i].SkillLevel;
			++pstView->DiathesisNum;
		}
	}
}

int pet_view(ZONESVRENV* pstEnv, Player *pstPlayer, CSROLEPETVIEW *pstRolePetView)
{
	int i;
	int j;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	ROLEPET *pstRolePet;
	PetFightShiOnline *pstPetFightOnline;
	CSPETVIEW *pstOnePetView;

	pstRolePetView->PetNum = 0;
	for (i=0; i<pstPetOnline->bFightShiNum; i++)
	{
		pstPetFightOnline =  &pstPetOnline->astFightShi[i];

		pstRolePetView->PetView[pstRolePetView->PetNum].Zhanchong = 0;
		pstRolePetView->PetView[pstRolePetView->PetNum].HetiFlag = 0;
		if ( pstPetOnline->astFightShi[i].stRolePetOnline.ullWID == pstPlayer->stOnline.stHeti.PetWID)
		{
			pstRolePetView->PetView[pstRolePetView->PetNum].HetiFlag = 1;
		}

		//pstRolePetView->PetView[pstRolePetView->PetNum].HetiFlag = pstPlayer->stOnline.stHeti.iPetWID >0?1:0;//只有一个出征的宠物
		pstRolePetView->PetView[pstRolePetView->PetNum].PetID = pstPetOnline->astFightShi[i].stRolePetOnline.unDefID;
		pstRolePetView->PetView[pstRolePetView->PetNum].PetWID = pstPetOnline->astFightShi[i].stRolePetOnline.ullWID;
		pstRolePetView->PetView[pstRolePetView->PetNum].XieDaiPos = pstPetOnline->astFightShi[i].iXieDaiPos;
		STRNCPY(pstRolePetView->PetView[pstRolePetView->PetNum].PetName, pstPetOnline->astFightShi[i].szName, 
					sizeof(pstRolePetView->PetView[0].PetName));
		pstRolePet = z_find_role_pet(pstPlayer, pstPetOnline->astFightShi[i].stRolePetOnline.ullWID, NULL);
		if (pstRolePet)
		{
			pstRolePetView->PetView[pstRolePetView->PetNum].MaxPower = pstRolePet->MaxHP;
			pstRolePetView->PetView[pstRolePetView->PetNum].Power = pstRolePet->HP;
			pstRolePetView->PetView[pstRolePetView->PetNum].Level = pstRolePet->Level;
			pstRolePetView->PetView[pstRolePetView->PetNum].StrengthenLevel = pstRolePet->StrengthenLevel;
			pstRolePetView->PetView[pstRolePetView->PetNum].Passive = pstRolePet->Passive;
			pstRolePetView->PetView[pstRolePetView->PetNum].Genius = pstRolePet->PetGenius;
			pet_fill_diathesis_view(pstRolePet, pstRolePetView->PetView + pstRolePetView->PetNum);

			
			if (pstRolePet->MiscFlag & PET_MISC_FLAG_COMBAT)
			{
				pstRolePetView->PetView[pstRolePetView->PetNum].Zhanchong = 1;
			}

			pstRolePetView->PetView[pstRolePetView->PetNum].GeniusSkillID = pstRolePet->NativeSkill.SkillID;
			pstRolePetView->PetView[pstRolePetView->PetNum].GeniusSkillLvl = pstRolePet->NativeSkill.SkillLevel;
		}
		else
		{
			pstRolePetView->PetView[pstRolePetView->PetNum].MaxPower = 0;
			pstRolePetView->PetView[pstRolePetView->PetNum].Power = 0;
			pstRolePetView->PetView[pstRolePetView->PetNum].StrengthenLevel = 0;
			pstRolePetView->PetView[pstRolePetView->PetNum].DiathesisNum = 0;
			pstRolePetView->PetView[pstRolePetView->PetNum].Passive = 0;
			pstRolePetView->PetView[pstRolePetView->PetNum].Genius = 0;
			pstRolePetView->PetView[pstRolePetView->PetNum].GeniusSkillID = 0;
			pstRolePetView->PetView[pstRolePetView->PetNum].GeniusSkillLvl = 0;
		}

		pstOnePetView = &pstRolePetView->PetView[pstRolePetView->PetNum];
		pstOnePetView->GoodStatusNum = pstPetFightOnline->stStatusInfo.bGoodStatusNum;
		
		for(j=0; j<pstPetFightOnline->stStatusInfo.bGoodStatusNum; j++)
		{
			pstOnePetView->GoodStatusList[j].StatusID = pstPetFightOnline->stStatusInfo.astGoodStatus[j].unStatusID;
			pstOnePetView->GoodStatusList[j].Level = pstPetFightOnline->stStatusInfo.astGoodStatus[j].bLevel;
			pstOnePetView->GoodStatusList[j].Overlay = pstPetFightOnline->stStatusInfo.astGoodStatus[j].unOverlay;
			pstOnePetView->GoodStatusList[j].ApplyID = pstPetFightOnline->stStatusInfo.astGoodStatus[j].iApplyID;
			pstOnePetView->GoodStatusList[j].ApplyRelation= pstPetFightOnline->stStatusInfo.astGoodStatus[j].cApplyRelation;			
			SET_STATUS_TIME(pstOnePetView->GoodStatusList[j].Time,  pstPetFightOnline->stStatusInfo.astGoodStatus[j].stEnd, pstEnv->pstAppCtx->stCurr);
		}
		
		pstRolePetView->PetNum++;
	}

	return 0;
}

int pet_player_view_action(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSROLEPETVIEW *pstRolePetView = &pstActionRes->Data.PetView;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_PET_VIEW;
	
	pet_view(pstEnv, pstPlayer, pstRolePetView);
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int pet_stat_chg(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, PETDEF *pstPetDef, int iOldStat)
{
	CSPKG stPkg;
	CSPETSVR *pstPetSvr  = &stPkg.Body.PetSvr;
	CSPETATTRCHG *pstPetChgData = &pstPetSvr->PetSvrData.AttrChg;

	pstPetSvr->PetSvrType = PET_ATTR_CHG;

	pstPetChgData->Type = PET_CHG_STAT;
	pstPetChgData->Wid = pstRolePet->WID;
	pstPetChgData->AttrValue.ChgStatFlag =  pstRolePet->StatFlag;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	if ((PET_STAT_FIGHT & iOldStat) || (PET_STAT_FIGHT & pstRolePet->StatFlag))
	{
		pet_player_view_action(pstEnv, pstPlayer);
	}

	return 0;
	UNUSED(pstPetDef);	//add by paraunused.pl
}

int pet_set_stat(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iStat)
{
	PETDEF *pstPetDef;
	 int iOldStat = pstRolePet->StatFlag;
	
	if ((0 == iStat && 0 == (PET_STAT_FIGHT & pstRolePet->StatFlag)) ||
		(0 != iStat && (iStat & pstRolePet->StatFlag)))
	{
		return 0;
	}

	if (PET_STAT_FIGHT == iStat && (pstRolePet->MiscFlag&PET_MISC_FLAG_JIYANG))
	{
		return 0;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return -1;
	}

	if (PET_STAT_FIGHT == iStat  && 0 > pet_xiedai_check(pstEnv, pstPlayer, pstRolePet, pstPetDef, iStat))
	{
		return -1;
	}

	pet_del_online(pstEnv, pstPlayer, pstRolePet);

	pstRolePet->MiscFlag &= ~PET_MISC_FLAG_HETI;
	pstRolePet->StatFlag &= ~(PET_STAT_FIGHT|PET_STAT_SEEFIGHT);
	if (0 != iStat)
	{
		pstRolePet->StatFlag |= iStat;
	}

	pet_update_online(pstEnv, pstPlayer, pstRolePet);

	if (PET_STAT_FIGHT == iStat)
	{
		if (0 > pet_update_skill(pstEnv, pstPlayer, pstRolePet, pstPetDef))
		{
			return -1;
		}

		pstPlayer->stOnline.iPetFgtTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	}

	pet_stat_chg(pstEnv, pstPlayer, pstRolePet, pstPetDef, iOldStat);

	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	return 0;
}

int open_pet_clt_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->ID = iNpcID;
	pstActionRes->Data.UI.UIType = UI_TYPE_PET;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

//召回出征宠物
int z_player_backup_pet(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ROLEPET *pstRolePet;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

	if (pstPetOnline->bFightShiNum > 0)
	{
		pstPlayer->stOnline.stPetOnline.llPetWidBackup = pstPetOnline->astFightShi[0].stRolePetOnline.ullWID;
		//pstPlayer->stOnline.llPetWidBackup = pstPetOnline->astFightShi[0].stRolePetOnline.ullWID;
	}

	for(i=0; i <pstPetOnline->bFightShiNum; i++)
	{
		pstRolePet = z_find_role_pet(pstPlayer, pstPetOnline->astFightShi[i].stRolePetOnline.ullWID, NULL);
		if (pstRolePet)
		{
			pet_set_stat(pstEnv, pstPlayer, pstRolePet, 0);
		}
	}

	return 0;
}

int z_player_restore_pet(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPET *pstRolePet;

	if (is_player_ride_on_machine(pstPlayer))
	{
		return -1;
	}
	
	if (0 != pstPlayer->stOnline.stPetOnline.llPetWidBackup)
	{
		pstRolePet = z_find_role_pet(pstPlayer,pstPlayer->stOnline.stPetOnline.llPetWidBackup, NULL);
		if (pstRolePet)
		{
			pet_set_stat(pstEnv, pstPlayer, pstRolePet, PET_STAT_FIGHT);
		}
	}

	pstPlayer->stOnline.stPetOnline.llPetWidBackup = 0;
	
	return 0;
}

int z_player_restore_pet_nowmap(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst;
	MapIndex *pstMapIdx;
	ZONEPWORLDINST *pstPworldInst;
	ZONEPWORLD *pstZonePworld ;
	
	stIdx.iID	=	pstPlayer->stRoleData.Map;
	stIdx.iPos	=	pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst)
		return -1;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstPlayer->stRoleData.Map);
	if (!pstMapIdx)
		return -1;
	
	if (pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_PET_FIGHT)
	{
		return -1;
	}

	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (pstPworldInst)
	{
	      pstZonePworld= z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	      if (pstZonePworld && (pstZonePworld->stPworldDef.CtrlFlag & PWORLD_NO_PET_FIGHT))  
	      {
		  	return -1;
	      }
	}

	z_player_restore_pet(pstEnv, pstPlayer);

	return 0;
}

int pet_campaign_pre_check(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETACT *pstPetAct)
{
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	ZONEIDX stIdx;
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef = NULL;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

	if(is_player_ride_on_machine(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET104);
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
	{	
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld && 
			(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_PET_FIGHT))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET105);
			z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
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

	if (pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_NO_PET_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET106);
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	// 检查禁止召唤状态
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NO_BECKON))
	{
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET107);
		return -1;
	}
	
	pstRolePet = z_find_role_pet(pstPlayer, pstPetAct->PetWid, NULL);
	if (NULL == pstRolePet || (PET_STAT_DEAD & pstRolePet->StatFlag))
	{
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)	
	{
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	if (0 != pstPetDef->FightLevel && pstPlayer->stRoleData.Level < pstPetDef->FightLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
							ZONEERR_PET108, pstRolePet->Name, pstPetDef->FightLevel);
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	if ((int)pstRolePet->Level - (int)pstPlayer->stRoleData.Level > PET_LEVEL_LIMIT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
				ZONEERR_PET109, pstRolePet->Name);
			
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	if (pstRolePet->LoveVal < PET_FIGHT_LOVEVAL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
							ZONEERR_PET110);
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}
	
	if (pstPetOnline->bFightShiNum >= PET_MAX_FIGHT_SHI)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PET111);
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}

	if (is_pet_heti(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PET112);
		z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		return -1;
	}
	
	return 0;
}

int pet_action_campaignpre(ZONESVRENV* pstEnv, Player *pstPlayer,  ROLEPET * pstRolePet)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ActionID = ACTION_PET_CAMPAIGN_PRE;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.PetCampaignPre.PetID = pstRolePet->WID;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	return 0;
}

int pet_clt_act(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETACT *pstPetAct)
{
	ROLEPET *pstRolePet;
	int iStat;
	ROLEPET *pstHetiPet;

	if (CS_STAT_DEAD & pstPlayer->stOnline.State)
	{
		return -1;
	}

	// 宠物出征必须非上架

	switch(pstPetAct->ActTpye)
	{
		case PET_ACT_CAMPAIGN:
		case PET_ACT_BACKUP:
			pstRolePet = z_find_role_pet(pstPlayer, pstPetAct->PetWid, NULL);
			if (NULL == pstRolePet || (PET_STAT_DEAD & pstRolePet->StatFlag))
			{
				return -1;
			}

			if (pstPetAct->ActTpye ==PET_ACT_CAMPAIGN &&
				pet_is_up(pstPlayer, pstRolePet->WID))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET162);
				return -1;
			}

			pstHetiPet = pet_find_heti(pstPlayer);
			if (pstHetiPet)
			{
				pet_heti_end(pstEnv,pstPlayer,pstHetiPet, 0);	
			}
			
			if (PET_ACT_BACKUP == pstPetAct->ActTpye)
			{
				iStat = 0;
				
			}
			else
			{
				
				iStat = PET_STAT_FIGHT;
				if (!(pstPlayer->stOnline.State & CS_STAT_CAMPAIGN_PRE))
				{
					return -1;
				}
			
				if (pstEnv->pstAppCtx->stCurr.tv_sec  - pstPlayer->stOnline.iPetFightPreTime + 1 < PET_CAMPAIGN_PRE_TIME) 
				{
					tlog_debug(pstEnv->pstLogCat, 0, 0, "fight pre fail");
					return -1;
				}
			
				pstPlayer->stOnline.State &= ~CS_STAT_CAMPAIGN_PRE;

				pstPlayer->stRoleData.MiscInfo.FightPetWID = pstRolePet->WID;
			}
			
			pet_set_stat(pstEnv, pstPlayer, pstRolePet, iStat);
			break;
		case PET_ACT_CAMPAIGN_PRE:
			if (0 > pet_campaign_pre_check(pstEnv, pstPlayer, pstPetAct))
			{
				return -1;
			}

			pstRolePet = z_find_role_pet(pstPlayer, pstPetAct->PetWid, NULL);
			if(pstRolePet == NULL)
			{
				return -1;
			}
			
			if (pet_is_up(pstPlayer, pstRolePet->WID))
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET162);
				return -1;
			}
			
			pstPlayer->stOnline.State |= CS_STAT_CAMPAIGN_PRE;
			pstPlayer->stOnline.iPetFightPreTime = pstEnv->pstAppCtx->stCurr.tv_sec;

			//广播动作
			pet_action_campaignpre(pstEnv, pstPlayer, pstRolePet);
			
			break;
		case PET_ACT_CAMPAIGN_BRK:
			z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

PETSKILLGRID *find_pet_skill( ROLEPET * pstRolePet , unsigned short unSkillID, int *piIdx)
{
	int i;
	PETSKILL *pstPetSkill = &pstRolePet->PetSkills;
	
	for (i = 0 ; i< pstPetSkill->SkillGridNum; i++)
	{
		if ( pstPetSkill->SkillGrid[i].SkillID == unSkillID )
		{
			if (piIdx) 	*piIdx = i;
			return (PETSKILLGRID *)&pstPetSkill->SkillGrid[i];
		}
	}

	return NULL;
	
}

int pet_skill_learn_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, 
							DIATHESISDEF *pstDiathesisDef, CSPETSKILLLEARN *pstCSPetSkillLearn, int *piPos)
 {
 	int i;
// 	SKILLLEARN *pstSkillLearn = &pstDiathesisDef->Learn;

	for(i = 0; i<pstRolePet->PetSkills.SkillGridNum; i++)
	{
		if(	pstRolePet->PetSkills.SkillGrid[i].SkillID == pstDiathesisDef->DiathesisID ||
			pstRolePet->PetSkills.SkillGrid[i].SkillID == pstDiathesisDef->GroupSkill)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PET163, pstRolePet->Name, pstDiathesisDef->Name);
			return -1;
		}

		// 学会了此技能的高级或者低级技能
		//if (pstRolePet->PetSkills.SkillGrid[i].SkillID == pstDiathesisDef->GroupSkill)
		//{
		//	return -1;
		//}
	}
	
	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstCSPetSkillLearn->StuffData.ItemID, 1, 
							pstCSPetSkillLearn->StuffData.ListType, piPos,PACKAGE_DEC_NONE))
	{
		return -1;
	}

	/*if (pstSkillLearn->MoneyAttrID >= ATTR_ID_MONEY && pstSkillLearn->MoneyAttrID <= ATTR_ID_BULL)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstSkillLearn->MoneyAttrID, pstSkillLearn->MoneyVal))
		{
			z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "银币不够!");
			return -1;
		}
	}*/
	
	return 0;
 }

int pet_skill_up_check(ZONESVRENV* pstEnv, Player *pstPlayer, PETSKILLGRID*pstPetSkillOne, DIATHESISDEF *pstDiathesisDef, 
						CSPETSKILLUP *psCStPetSkillUp, ITEMDEF *pstItemDef, int *piPos)
 {
	RESULT *pstResult;
	SKILLLEARN *pstSkillLearn = &pstDiathesisDef->Learn;

	pstResult = z_item_get_result(pstItemDef, RESULT_PET_SKILL_UP_STUFF);
	if(NULL == pstResult)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET113);
		return -1;
	}

	if(pstResult->ResultVal1 != pstDiathesisDef->PetDiathesisGradeType)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET113);
		return -1;
	}
	
	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							psCStPetSkillUp->StuffData.ItemID, 1, 
							psCStPetSkillUp->StuffData.ListType, piPos,PACKAGE_DEC_NONE))
	{
		return -1;
	}

	if (pstSkillLearn->MoneyAttrID >= ATTR_ID_MONEY && pstSkillLearn->MoneyAttrID <= ATTR_ID_BULL)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstSkillLearn->MoneyAttrID, pstSkillLearn->MoneyVal))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
			return -1;
		}
	}
	
	return 0;
	UNUSED(pstPetSkillOne);	//add by paraunused.pl
 }

int pet_skill_learn_consume(ZONESVRENV* pstEnv, Player* pstPlayer, 
								SKILLLEARN *pstSkillLearn, CSPETSKILLLEARN *pstCSPetSkillLearn, 
								int *piPos, USEMONEY *pstUseMoney, ITEMDEC *pstDec)
{
	int iRet = 0;

	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
						pstCSPetSkillLearn->StuffData.ItemID, 1, 
						pstCSPetSkillLearn->StuffData.ListType, piPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PETSKILL);
	if (iRet < 0)
	{
		return -1;
	}

	itemdec_fill(pstDec, pstCSPetSkillLearn->StuffData.ItemID, 1, iRet);
	
	/*if (package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstSkillLearn->MoneyVal) < 0)
	{
		return -1;
	}
	
	pstUseMoney->UseMoney_Money = pstSkillLearn->MoneyVal;*/
	return 0;
	UNUSED(pstSkillLearn);	//add by paraunused.pl
	UNUSED(pstUseMoney);	//add by paraunused.pl
}

int pet_skill_up_consume(ZONESVRENV* pstEnv, Player* pstPlayer, SKILLLEARN *pstSkillLearn, 
							CSPETSKILLUP *pstCSPetSkillUp, int *piPos)
{
	int iRet;
	
	if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
						pstCSPetSkillUp->StuffData.ItemID, 1, 
						pstCSPetSkillUp->StuffData.ListType, piPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PETSKILL))
	{
		return -1;
	}

	if (pstSkillLearn->MoneyAttrID >= ATTR_ID_MONEY && pstSkillLearn->MoneyAttrID <= ATTR_ID_BULL)
	{
		iRet = package_dec_money(pstEnv, pstPlayer, pstSkillLearn->MoneyAttrID, pstSkillLearn->MoneyVal) ;
		if (iRet < 0)
		{
			return -1;
		}

		if (pstSkillLearn->MoneyAttrID == ATTR_ID_MONEY)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETSKILL, pstCSPetSkillUp->SkillID, 0, pstSkillLearn->MoneyVal, "宠物技能");
		}
		else if (pstSkillLearn->MoneyAttrID == ATTR_ID_BULL)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETSKILL, pstCSPetSkillUp->SkillID, pstSkillLearn->MoneyVal-iRet, iRet, "宠物技能");
		}
	}
	
	return 0;
}

PETRESOLVEGETITEM *pet_resolve_get_item(PETRESOLVEDEF *pstPetResolveDef)
{
	int i;
	int iRand, iRate;

	iRate = 0;
	iRand = RAND1(100);
	
	for(i=0; i< MAX_PET_RESOLVE_ITEM; i++)
	{
		iRate += pstPetResolveDef->GetItem[i].Rate;

		if(iRand < iRate)
		{
			return &pstPetResolveDef->GetItem[i];
		}
	}

	return NULL;
}

int pet_resolve_add_item(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, ITEMDEC *pstLogGetItem)
{
	PETRESOLVEDEF *pstPetResolveDef;
	PETRESOLVEDEF *pstPetResolveDef2;
	PETRESOLVEGETITEM *pstGetItem;
	ROLEGRID stRoleGrid;
	PETDEF *pstPetDef;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return -1;
	}

	//品质分解
	pstPetResolveDef = z_find_pet_resolve_def(pstEnv, PET_RESOLVE_TYPE_PINZHI, pstRolePet->PetGenius+1);

	//等级分解
	pstPetResolveDef2 = z_find_pet_resolve_def(pstEnv, PET_RESOLVE_TYPE_LEVEL, pstPetDef->FightLevel/10);

	if(pstPetResolveDef)
	{
		if(pstPetResolveDef2)
		{
			// 随即取一个分解产物类型
			int iRnd = RAND1(2);
			
			pstPetResolveDef =  iRnd ? pstPetResolveDef2 : pstPetResolveDef;
		}
	}
	else
	{
		pstPetResolveDef = pstPetResolveDef2;
	}

	//没产物的直接放生
	if(NULL == pstPetResolveDef)
	{
		return 0;
	}

	pstGetItem = pet_resolve_get_item(pstPetResolveDef);
	if(NULL == pstGetItem || pstGetItem->ItemID <= 0)
	{
		return 0;
	}

	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstGetItem->ItemID))
	{
		return -1;
	}

	stRoleGrid.GridData.RoleItem.Num = pstGetItem->Num;
	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
	{	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, ZONEERR_PET114);
		return -1;
	}

	pstLogGetItem->ID = pstGetItem->ItemID;
	pstLogGetItem->Num = pstGetItem->Num;
	pstLogGetItem->BindNum = 0;
	if(stRoleGrid.GridData.RoleItem.InstFlag & INST_ITEM_BIND)
	{
		pstLogGetItem->BindNum = pstGetItem->Num;
	}
	
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid,  1,0, IMPITEM_OPTYPE_PET_RESOLVE);

	return 0;
}

int pet_del(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong PetWid, int fOplog, ITEMDEC *pstGetItem) 
{
	int iIdx;
	ROLEPET *pstRolePet;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	
	pstRolePet = z_find_role_pet(pstPlayer, PetWid, &iIdx);
	if (!pstRolePet)
	{
		return -1;
	}

	if(pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		pet_set_stat(pstEnv, pstPlayer, pstRolePet, 0);
	}

	range_delete(pstEnv, RANGE_MAIN_PET, RANGE_MAIN_PET_SUB_FIGHT, pstRolePet->WID);
	
	pet_del_online(pstEnv, pstPlayer, pstRolePet);

	if(fOplog)
	{	
		if(fOplog == 3)
		{
			z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_EAT_DEL, NULL, NULL, NULL,NULL);
		}
		else if(fOplog == 2)
		{
			z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_FUSE_DEL, pstGetItem, NULL, NULL,NULL);
		}
		else
		{
			z_oplog_event_begin(pstEnv);
			z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_RELEASE, pstGetItem, NULL, NULL,NULL);
		}

		z_role_drop_record(pstEnv, pstPlayer, ROLE_DROP_PET, pstRolePet);
	}
	
	pstPetSvr->PetSvrType = PET_SVR_DEL;
	pstPetSvr->PetSvrData.Del.Wid = pstRolePet->WID;
	pstPetSvr->PetSvrData.Del.Flag = 0;
	
	if(fOplog == 3)
	{
		pstPetSvr->PetSvrData.Del.Flag = 1;
	}

	pstPetPackage->Num--;
	if (iIdx != pstPetPackage->Num)
	{
		memmove(&pstPetPackage->RolePets[iIdx], &pstPetPackage->RolePets[iIdx+1], (pstPetPackage->Num - iIdx)*sizeof(ROLEPET));
	}

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

//宠物丢弃
int pet_drop(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong PetWid) 
{
	ROLEPET *pstRolePet;
	ITEMDEC stGetItem;

	memset(&stGetItem, 0, sizeof(stGetItem));
	
	pstRolePet = z_find_role_pet(pstPlayer, PetWid, NULL);
	if(NULL == pstRolePet)
	{
		return -1;
	}

	if(pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		return -1;
	}

	if(player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if(0 > pet_resolve_add_item(pstEnv, pstPlayer, pstRolePet, &stGetItem))
	{
		return -1;
	}

	pet_del(pstEnv, pstPlayer, PetWid, 1, &stGetItem);
	
	return 0 ;
}

int pet_update_to_clt(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet)
{
	PETDEF *pstPetDef;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}
	
	pstPetSvr->PetSvrType = PET_INFO_CHG;
	memcpy(&pstPetSvr->PetSvrData.PetInfo, pstRolePet, sizeof(ROLEPET));

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int pet_fight_attr_to_clt(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet)
{
	PETDEF *pstPetDef;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	pstPetSvr->PetSvrType = PET_SVR_FIGHT_ATTR;
	pstPetSvr->PetSvrData.PetFightAttr.PetWID = pstRolePet->WID;
	memcpy(&pstPetSvr->PetSvrData.PetFightAttr.Attr, &pstRolePet->FightAttr, sizeof(pstPetSvr->PetSvrData.PetFightAttr.Attr));

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int pet_add_loveval_notify(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iOldLoveVal)
{
	//宠物天赋,最大忠诚度
	int iMaxLoveVal = MAX_PET_LOVE_VAL;
	
	pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_MAX_LOVE_VAL_ADD_MAP, &iMaxLoveVal,pstRolePet);
	if(pstRolePet->LoveVal == iMaxLoveVal)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET164, pstRolePet->Name);
	}	
	else if(pstRolePet->LoveVal > iOldLoveVal)
	{
		int iAddLove;

		iAddLove = (pstRolePet->LoveVal -iOldLoveVal)/100;
		if(!iAddLove)
		{
			iAddLove =1;
		}
		
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET165, 
					pstRolePet->Name, iAddLove);
	}

	return 0;
}

int pet_loveval_check(ZONESVRENV *pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, int iOldLoveVal)
{
	if (pstRolePet->LoveVal < PET_FIGHT_LOVEVAL && (pstRolePet->StatFlag & PET_STAT_FIGHT) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET115, pstRolePet->Name);
		pet_set_stat(pstEnv, pstPlayer, pstRolePet, 0);

		return 0;
	}
	
	if (iOldLoveVal >= 3100 && pstRolePet->LoveVal < 3100)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET116, pstRolePet->Name);
	}
	else if (iOldLoveVal >= 4000 && pstRolePet->LoveVal < 4000)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET117, pstRolePet->Name);
	}
	else if (iOldLoveVal >= 5000 && pstRolePet->LoveVal < 5000)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET118, pstRolePet->Name);
	}
	else
	{	
		return 0;
	}

	return 0;
}

//更新宠物忠诚度
int pet_love_val_chg(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet, int iLoveVal)
{
	int iOldLoveVal;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETATTRCHG *pstAttrChg = &pstPetSvr->PetSvrData.AttrChg;
	int iMaxLoveVal = MAX_PET_LOVE_VAL;

	iOldLoveVal = pstRolePet->LoveVal;

	//宠物天赋,最大忠诚度
	pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_MAX_LOVE_VAL_ADD_MAP, &iMaxLoveVal,pstRolePet);
	if (iLoveVal + pstRolePet->LoveVal >=  iMaxLoveVal)
	{
		pstRolePet->LoveVal = iMaxLoveVal;
	}
	else if (iLoveVal + pstRolePet->LoveVal <= 0)
	{
		pstRolePet->LoveVal = 0;
	}
	else
	{
		pstRolePet->LoveVal += iLoveVal;
	}

	//提醒
	if (iLoveVal < 0)
	{
		pet_loveval_check(pstEnv, pstPlayer, pstRolePet, iOldLoveVal);
	}
	else
	{
		pet_add_loveval_notify(pstEnv, pstPlayer, pstRolePet, iOldLoveVal);
	}

	if (	pstRolePet->LoveVal < 50*PET_LOVE_VAL_UNIT &&
		pstRolePet->MiscFlag &PET_MISC_FLAG_HETI)
	{
		pet_heti_in(pstEnv, pstPlayer,pstRolePet, 0);
	}
	
	if (iOldLoveVal/PET_LOVE_VAL_UNIT != pstRolePet->LoveVal/PET_LOVE_VAL_UNIT)
	{
		pstPetSvr->PetSvrType = PET_ATTR_CHG;

		pstAttrChg->Type =  PET_CHG_LOVE_VAL;
		pstAttrChg->Wid = pstRolePet->WID;
		pstAttrChg->AttrValue.ChgLoveVal = pstRolePet->LoveVal;

		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	
	return 0;
}

int get_pet_skill_num(ROLEPET *pstRolePet)
{
	int i;
	int iSkillNum = 0;

	for(i=0; i<pstRolePet->PetSkills.SkillGridNum; i++)
	{
		if(pstRolePet->PetSkills.SkillGrid[i].SkillID)
		{
			iSkillNum++;
		}
	}
	
	return iSkillNum;
}

PETSKILLGRID *pet_get_learn_skill_grid(ZONESVRENV* pstEnv, ROLEPET *pstRolePet)
{
	int i;
	int iRate, iRand;
	PETSKILLGRIDDEF *pstSkillGridDef;
	int SkillNum;

	SkillNum = get_pet_skill_num(pstRolePet)+1;
	if(SkillNum > pstRolePet->PetSkills.SkillGridNum)
	{
		SkillNum = pstRolePet->PetSkills.SkillGridNum;
	}

	pstSkillGridDef = z_find_pet_skillgrid_def(pstEnv, SkillNum);
	if(NULL == pstSkillGridDef)
	{
		return NULL;
	}

	iRate = 0;
	iRand = RAND1(100);
	
	for(i=0; i<MAX_PET_SKILL_GRID; i++)
	{
		if(i >= pstRolePet->PetSkills.SkillGridNum)
		{
			break;
		}
		
		iRate += pstSkillGridDef->GridRate[i];
		if(iRand < iRate)
		{
			return &pstRolePet->PetSkills.SkillGrid[i];
		}
	}
	
	return NULL;
}

int pet_skill_learn(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETSKILLLEARN*pstPetSkillLearn)
{
	DIATHESISDEF *pstDiathesisDef;
	ROLEPET *pstRolePet;
	PETSKILLGRID*pstPetSkillOne;
	ITEMDEF *pstItemDef;
	RESULT *pstResult;
	ROLEITEM *pstRoleItem;
	int iPos = -1;
	//技能消耗
	USEMONEY stUseMoney;
	ITEMDEC stDec;

	if (pstPlayer->stOnline.State& CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET166);
		return -1;
	}

	if (pstPlayer->stOnline.State& CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET167);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET168);
		return -1;	
	}

	pstRolePet = z_find_role_pet(pstPlayer, pstPetSkillLearn->PetWid, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	if(pstRolePet->Level < 20)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET119);
		return -1;
	}
	
	/*if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						"您的宠物处于出战状态，不能进行技能学习操作。");
		return -1;
	}*/
	
	if (0 > package_get_item(pstEnv, pstPlayer, 
							pstPetSkillLearn->StuffData.ListType, pstPetSkillLearn->StuffData.GridIdx, 
							&pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;
	}

	pstResult = z_item_get_result(pstItemDef, RESULT_PET_SKILL_BOOK);
	if(NULL == pstResult)
	{
		return -1;
	}
	
	pstDiathesisDef = z_find_diathesis(pstEnv, pstResult->ResultVal1, 1);
	if (	NULL == pstDiathesisDef )
	{
		return -1;
	}

	// 只有特技和技能书才能学习
	// modify by jerry
	if (	!(pstDiathesisDef->PetSkillType &
		(PET_LEARN_SKILL_TYPE_BOOK|PET_LEARN_SKILL_TYPE_TEJI)))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET120);
		return -1;
	}
	
	if (pstDiathesisDef->PetSkillType &PET_LEARN_SKILL_TYPE_TEJI)
	{
		PETDEF *pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
		if (!pstPetDef)
		{
			return -1;
		}

		if (pstDiathesisDef->DiathesisID != pstPetDef->TejiID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET121);
			return -1;
		}
	}
	// modify end
	
	pstPetSkillOne = pet_get_learn_skill_grid(pstEnv, pstRolePet);
	if(NULL == pstPetSkillOne)
	{
		return -1;
	}
	
	if (0 > pet_skill_learn_check(pstEnv, pstPlayer, pstRolePet, pstDiathesisDef, pstPetSkillLearn, &iPos))
	{
		return -1;
	}
	

	memset(&stUseMoney, 0, sizeof(stUseMoney));
	memset(&stDec, 0, sizeof(stDec));
	if (0 > pet_skill_learn_consume(pstEnv, pstPlayer, &pstDiathesisDef->Learn, 
									pstPetSkillLearn, &iPos,&stUseMoney,&stDec))
	{
		return -1;
	}
	
	pstPetSkillOne->SkillID = pstDiathesisDef->DiathesisID;
	pstPetSkillOne->SkillLevel = 1;
	pstPetSkillOne->GridType = pstDiathesisDef->PetDiathesisType;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET169, pstRolePet->Name, pstDiathesisDef->Name);

	//更新技能
	if (pstRolePet->StatFlag & PET_STAT_FIGHT )
	{
		pet_fight_addskill (pstEnv,pstPlayer, pstRolePet, pstPetSkillOne);
	}

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	// 合体的更新下数值
	if (is_pet_heti(pstPlayer))
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}
	
	task_pet_trace(pstEnv, pstPlayer, pstRolePet, TASK_OBJ_PET_SKILL);

	pet_svr_act(pstEnv, pstPlayer, PET_SVR_ACT_SKILL_LEARN);
	
	z_pet_attr_chg_oplog(pstEnv, pstPlayer, pstRolePet, OP_PET_LEARN_SKILL, 
						pstPetSkillOne->SkillID,&stDec,&stUseMoney);
	
	return 0;	
}


#if 0
//技能升级
int pet_skill_up(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETSKILLUP *pstPetSkillUp)
{
	DIATHESISDEF *pstDiathesisDef;
	ROLEPET *pstRolePet;
	PETSKILLGRID*pstPetSkillOne;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	int iPos = -1;

	pstRolePet = z_find_role_pet(pstPlayer, pstPetSkillUp->PetWid, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						"您的宠物处于出战状态，不能进行技能升级操作。");
		return -1;
	}

	//选的哪个格子
	if (pstPetSkillUp->SkillGridIdx >= pstRolePet->PetSkills.SkillGridNum ||
		pstPetSkillUp->SkillGridIdx >= MAX_PET_SKILL_GRID)
	{
		return -1;
	}

	//格子信息与协议匹配
	pstPetSkillOne = &pstRolePet->PetSkills.SkillGrid[pstPetSkillUp->SkillGridIdx];
	if (pstPetSkillOne->SkillID ==0 || pstPetSkillOne->SkillID != pstPetSkillUp->SkillID)
	{
		return -1;
	}
	
	//技能实际上是素质
	pstPetSkillOne = find_pet_skill(pstRolePet, pstPetSkillUp->SkillID, NULL);
	if (NULL == pstPetSkillOne)
	{
		return -1;
	}
	
	pstDiathesisDef = z_find_diathesis(pstEnv, pstPetSkillUp->SkillID, pstPetSkillOne->SkillLevel+1);
	if (NULL == pstDiathesisDef)
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						"该技能已达到等级上限");
		return -1;
	}

	if (0 > package_get_item(pstEnv, pstPlayer, 
							pstPetSkillUp->StuffData.ListType, pstPetSkillUp->StuffData.GridIdx, 
							&pstItemDef, &pstRoleItem, &iPos))
	{
		z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "请放入正确的材料");
		return -1;
	}
	
	if (0 > pet_skill_up_check(pstEnv, pstPlayer, pstPetSkillOne, pstDiathesisDef, pstPetSkillUp, pstItemDef, &iPos))
	{
		return -1;
	}
	
	//技能消耗
	if (0 > pet_skill_up_consume(pstEnv, pstPlayer, &pstDiathesisDef->Learn, pstPetSkillUp, &iPos))
	{
		return -1;
	}	
	
	pstPetSkillOne->SkillLevel++;

	z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE, 
			"宠物[%s]学会了%s%d级", pstRolePet->Name, pstDiathesisDef->Name, pstPetSkillOne->SkillLevel);
	
	//更新技能
	if (pstRolePet->StatFlag & PET_STAT_FIGHT )
	{
		pet_fight_skill_upone(pstEnv, pstPlayer, pstRolePet, pstPetSkillOne);
	}

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	task_pet_trace(pstEnv, pstPlayer, pstRolePet, TASK_OBJ_PET_SKILL);

	pet_svr_act(pstEnv, pstPlayer, PET_SVR_ACT_SKILL_UP);
	
	z_pet_attr_chg_oplog(pstEnv, pstPlayer, pstRolePet, OP_PET_LEARN_SKILL, pstPetSkillUp->SkillID);
	
	return 0;	
}

#endif


int pet_clt_skill_up(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETSKILLUPDATE *pstSkillUp)
{
	ROLEPET *pstRolePet;
	DIATHESISDEF *pstDiathesisDef;
	int iSkillLevel;
	int iUse;
	int iSkillID;
	ITEMDEC stDec;
	USEMONEY stUseMoney;
	int iRet;
	SKILLUPRATE *pstUp;
	int iRate;
	int iChg = 0;

	if (pstPlayer->stOnline.State& CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET166);
		return -1;
	}

	if (pstPlayer->stOnline.State& CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET167);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET168);
		return -1;	
	}

	pstRolePet = z_find_role_pet(pstPlayer, pstSkillUp->PetWid, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PET122);
		return -1;
	}

	if (	pstSkillUp->Idx >= pstRolePet->PetSkills.SkillGridNum ||
		pstSkillUp->Idx >= MAX_PET_SKILL_GRID)
	{
		return -1;
	}

	iSkillID = pstRolePet->PetSkills.SkillGrid[(int)pstSkillUp->Idx].SkillID;
	if (	iSkillID <= 0)
	{
		return -1;
	}

	iSkillLevel = pstRolePet->PetSkills.SkillGrid[(int)pstSkillUp->Idx].SkillLevel;
	/*if (iSkillLevel == 3)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
										ZONEERR_PET123);
		return -1;	
	}*/

	if (z_find_diathesis(pstEnv, iSkillID, iSkillLevel+1) == NULL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET123);
		return -1;
	}

	pstDiathesisDef = z_find_diathesis(pstEnv, iSkillID, iSkillLevel);
	if (NULL == pstDiathesisDef)
	{
		return -1;
	}

	
	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							PET_SKILL_UP_ITEMID, iSkillLevel, 
							LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE))
	{
		ITEMDEF *pstItemDef = z_find_itemdef( pstEnv, PET_SKILL_UP_ITEMID);
		if (pstItemDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET124, pstItemDef->Name);
		}
		return -1;
	}

	iUse = (iSkillLevel+1)*1000;	
	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iUse))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET170);
		return -1;
	}

	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					PET_SKILL_UP_ITEMID, iSkillLevel, 
					LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PETSKILL);
	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iUse);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETSKILL, pstRolePet->PetID, iUse-iRet, iRet, "宠物技能");
	memset(&stUseMoney, 0, sizeof(stUseMoney));
	memset(&stDec, 0, sizeof(stDec));
	stUseMoney.UseMoney_Money = iUse;
	itemdec_fill(&stDec, PET_SKILL_UP_ITEMID,iSkillLevel,iRet);
	
	pstUp = &pstDiathesisDef->SkillUpRate;
	iRate = RAND1(pstUp->Up+pstUp->Down+pstUp->NoChg);

	// 成功
	if (iRate < pstUp->Up)
	{
		pstRolePet->PetSkills.SkillGrid[(int)pstSkillUp->Idx].SkillLevel ++;
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
				ZONEERR_PET171, 
				pstRolePet->Name, pstDiathesisDef->Name,iSkillLevel+1);
		iChg = 1;
	}
	else
	{
		// 倒退
		if (iRate < pstUp->Up+pstUp->Down)
		{
			if (iSkillLevel > 1)
			{
				pstRolePet->PetSkills.SkillGrid[(int)pstSkillUp->Idx].SkillLevel --;
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET172, 
							pstRolePet->Name, pstDiathesisDef->Name);
				iChg = 2;
			}
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET173);
		}
	}
	
	if (iChg)
	{
		pet_update_online(pstEnv, pstPlayer, pstRolePet);
		pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
		if(iChg == 1)
			pet_svr_act(pstEnv, pstPlayer, PET_SVR_ACT_SKILL_UP);
	}
	
	// todo:log
	z_pet_attr_chg_oplog(pstEnv, pstPlayer, pstRolePet, OP_PET_SKILL_UP, iSkillID,&stDec,&stUseMoney);
	return 0;
}


int pet_change_name(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETCHGNAME *pstChgName)
{
	ROLEPET *pstRolePet ;
	PETDEF *pstPetDef;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETATTRCHG *pstPetChgData = &pstPetSvr->PetSvrData.AttrChg;

	pstRolePet = z_find_role_pet(pstPlayer,pstChgName->PetWid, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return -1;
	}

	if ((0 == pstEnv->pstConf->CountryVer && CheckCharacter(pstChgName->PetName)) || (CheckDirty(pstChgName->PetName, 0x1f) & 0x1f ) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PET125);
		return -1;
	}

	if (strcmp(pstChgName->PetName, pstRolePet->Name) == 0)
	{
		return -1;
	}
	else
	{
		STRNCPY(pstRolePet->Name, pstChgName->PetName, sizeof(pstRolePet->Name));
	}

	pstPetSvr->PetSvrType = PET_ATTR_CHG;
	
	pstPetChgData->Wid = pstRolePet->WID;
	pstPetChgData->Type= PET_CHG_NAME;
	memcpy(pstPetChgData->AttrValue.ChgName, pstRolePet->Name, sizeof(pstRolePet->Name));

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		PetFightShiOnline *pstPetFightShiOnline;
		
		pstPetFightShiOnline = (PetFightShiOnline *)pet_find_online(pstPlayer, pstRolePet);
		if (pstPetFightShiOnline)
		{
			STRNCPY(pstPetFightShiOnline->szName, pstRolePet->Name, sizeof(pstPetFightShiOnline->szName));
			pet_player_view_action(pstEnv, pstPlayer);
		}
	}

	/*range_pet_set(pstEnv, pstPlayer, pstRolePet, pstPetDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstRolePet, pstPetDef));*/

	return 0;
}

int pet_strengthen_stuff_check(ZONESVRENV* pstEnv, Player *pstPlayer, 
	PETSTRENGTHENDEF *pstPetStrengthenDef)
{
	ITEMDEF *pstItemDef;
	int iItemNum;
	
	pstItemDef = z_find_itemdef(pstEnv, pstPetStrengthenDef->UseItemID);
	if(NULL == pstItemDef)
	{
		return -1;
	}

	iItemNum = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, pstPetStrengthenDef->UseItemID, -1, 0,PACKAGE_DEC_NONE);
	if(iItemNum < pstPetStrengthenDef->UseItemNum)
	{
		return -1;
	}

	return 0;
}

int pet_strengthen_check(ZONESVRENV* pstEnv, Player *pstPlayer, 
	PETSTRENGTHENDEF *pstPetStrengthenDef)
{
	if (pstPlayer->stOnline.State& CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET167);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET168);
		return -1;	
	}
	
	if(0 > pet_strengthen_stuff_check(pstEnv, pstPlayer, pstPetStrengthenDef))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET174);
		return -1;
	}
	
	if (pstPetStrengthenDef->MoneyType >= ATTR_ID_MONEY && pstPetStrengthenDef->MoneyType <= ATTR_ID_BULL)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstPetStrengthenDef->MoneyType, pstPetStrengthenDef->MoneyVal))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
			return -1;
		}
	}

	/*
	 if (pstCsStrengthen->SafeData.ItemID)
        {
                if (0 > package_get_item(pstEnv, pstPlayer,
                                                                pstCsStrengthen->SafeData.ListType, pstCsStrengthen->SafeData.GridIdx,
                                                                &pstItemDef, &pstRoleItem, &iPos))
                {
                        return -1;
                }

               
                if( NULL == z_item_get_result(pstItemDef, RESULT_PET_STRENGTHEN_SAFE))
                {
                        return -1;
                }

                if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer,
                                                                pstCsStrengthen->SafeData.ItemID, 1,
                                                                pstCsStrengthen->SafeData.ListType, &iPos))
                {
                        return -1;
                }
        }
	*/
	
	return 0;
}

int pet_strengthen_stuff_dec(ZONESVRENV * pstEnv, Player * pstPlayer, 
	PETSTRENGTHENDEF * pstPetStrengthenDef, ITEMDEC *pstItemDec)
{
	ITEMDEF *pstItemDef;
	int iBind = 0;
	
	pstItemDef = z_find_itemdef(pstEnv, pstPetStrengthenDef->UseItemID);
	if(NULL == pstItemDef)
	{
		return -1;
	}

	iBind =  package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
				pstPetStrengthenDef->UseItemID, pstPetStrengthenDef->UseItemNum, 
				-1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PETSTRENG);
	if (0 > iBind)
	{
		return -1;
	}

	itemdec_fill(pstItemDec, pstPetStrengthenDef->UseItemID, pstPetStrengthenDef->UseItemNum, iBind);
	
	return 0;
}

int pet_strengthen_consume(ZONESVRENV* pstEnv, Player *pstPlayer, PETSTRENGTHENDEF *pstPetStrengthenDef, ITEMDEC *pstItemDec, ROLEPET *pstRolePet)
{	
	int iRet;
	
	if(0 > pet_strengthen_stuff_dec(pstEnv, pstPlayer, pstPetStrengthenDef, pstItemDec))
	{
		return -1;
	}
	
	if (pstPetStrengthenDef->MoneyType >= ATTR_ID_MONEY && pstPetStrengthenDef->MoneyType <= ATTR_ID_BULL)
	{
		iRet = package_dec_money(pstEnv, pstPlayer, pstPetStrengthenDef->MoneyType, pstPetStrengthenDef->MoneyVal);
		if (iRet < 0)
		{
			return -1;
		}

		if (pstPetStrengthenDef->MoneyType == ATTR_ID_MONEY)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETSTREN, pstRolePet->PetID, 0, pstPetStrengthenDef->MoneyVal, "宠物强化");
		}
		else if (pstPetStrengthenDef->MoneyType == ATTR_ID_BULL)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETSTREN, pstRolePet->PetID, pstPetStrengthenDef->MoneyVal-iRet, iRet, "宠物强化");
		}
	}

	

	/*
	 if (pstCsStrengthen->SafeData.ItemID)
        {
                if (0 > package_get_item(pstEnv, pstPlayer,
                                                                pstCsStrengthen->SafeData.ListType, pstCsStrengthen->SafeData.GridIdx,
                                                                &pstItemDef, &pstRoleItem, &iPos))
                {
                        return -1;
                }

                if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer,
                                                        pstCsStrengthen->SafeData.ItemID, 1,
                                                        pstCsStrengthen->SafeData.ListType, &iPos))
                {
                        return -1;
                }
        }
	*/
	
	return 0;
}

int pet_strengthen_perfect_num_get(ROLEPET *pstRolePet)
{
	int i, iCount = 0;;

	for(i=0; i<pstRolePet->StrengthenLevel; i++)
	{
		if(pstRolePet->StrengthenInfo[i] == MAX_PET_STRENGTHEN_RATE_LIST)
		{
			iCount++;
		}
	}
	
	return iCount;
}

int pet_strengthen_mul_get(ROLEPET *pstRolePet, PETSTRENGTHENDEF *pstPetStrengthenDef)
{
	int i;
	int iRand, iRate;

	if(pstRolePet->StrengthenCount >= pstPetStrengthenDef->PerfectNum)
	{
		pstRolePet->StrengthenCount -= pstPetStrengthenDef->PerfectNum;
		return MAX_PET_STRENGTHEN_RATE_LIST-1;
	}
	else if(pstRolePet->StrengthenCount >= 0 || abs(pstRolePet->StrengthenCount) > pstPetStrengthenDef->PerfectNum)
	{
		if(RAND1(10000) < pstPetStrengthenDef->PerfectRate)
		{
			pstRolePet->StrengthenCount -= pstPetStrengthenDef->PerfectNum;
			return MAX_PET_STRENGTHEN_RATE_LIST-1;
		}
	}
	
	iRate = 0;
	iRand = RAND1(10000);
	for(i=0; i<MAX_PET_STRENGTHEN_RATE_LIST; i++)
	{
		if(pstPetStrengthenDef->LevelRate[i] <= 0 )
		{
			continue;
		}

		iRate += pstPetStrengthenDef->LevelRate[i];
		if(iRand < iRate)
		{
			return i;
		}
	}
	
	return -1;
}

int pet_strengthen(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETSTRENGTHEN *pstCsStrengthen)
{
	int iMul;
//	char szMsg[256];
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;
	PETSTRENGTHENDEF *pstPetStrengthenDef;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	ITEMDEC stItemDec;
	USEMONEY stUse;
	//int iPetLvl;

	pstRolePet = z_find_role_pet(pstPlayer,pstCsStrengthen->PetWid, NULL);
	if (NULL == pstRolePet)
	{
		goto _error;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PET126);
		goto _error;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		goto _error;
	}

	if (pstCsStrengthen->StrenLvl  >  pstRolePet->StrengthenLevel +1)
	{
		goto _error;
	}
	
	pstPetStrengthenDef = z_find_pet_strengthen_def(pstEnv, pstCsStrengthen->StrenLvl);
	if (NULL == pstPetStrengthenDef)
	{
		goto _error;
	}
	
	if(0 > pet_strengthen_check(pstEnv, pstPlayer, pstPetStrengthenDef))
	{
		goto _error;
	}

	if ( 0 > pet_strengthen_consume(pstEnv, pstPlayer, pstPetStrengthenDef, &stItemDec, pstRolePet))
	{
		goto _error;
	}

	pstRolePet->StrengthenCount += pstPetStrengthenDef->UseItemNum;
	iMul = pet_strengthen_mul_get(pstRolePet, pstPetStrengthenDef);
	if(0 > iMul)
	{
		goto _error;
	}

	pstPlayer->stOnline.stPetStrengthenUIInfo.ullPetWid = pstRolePet->WID;
	pstPlayer->stOnline.stPetStrengthenUIInfo.iAddVal = iMul+1;
	pstPlayer->stOnline.stPetStrengthenUIInfo.cReStrenLvl = pstCsStrengthen->StrenLvl;
	//pstPlayer->stOnline.stPetStrengthenUIInfo.iBindNum = stItemDec.BindNum;
		
	pstPetSvr->PetSvrType = PET_SVR_STRENGTHEN;
	pstPetSvr->PetSvrData.Strengthen.SuccFlag = pstPlayer->stOnline.stPetStrengthenUIInfo.iAddVal;
	pstPetSvr->PetSvrData.Strengthen.Wid = pstRolePet->WID;
	pstPetSvr->PetSvrData.Strengthen.StrenLvl = pstCsStrengthen->StrenLvl ;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	z_oplog_event_begin(pstEnv);

	bzero(&stUse, sizeof(stUse));
	stUse.UseMoney_Money = pstPetStrengthenDef->MoneyVal;
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_STRENGTHEN_CONSUME, NULL, 
						&stItemDec, &stUse,NULL);

	return 0;
	
_error:

	pstPetSvr->PetSvrType = PET_SVR_STRENGTHEN;
	pstPetSvr->PetSvrData.Strengthen.SuccFlag = 0;
	pstPetSvr->PetSvrData.Strengthen.Wid = pstRolePet->WID;
	pstPetSvr->PetSvrData.Strengthen.StrenLvl = pstCsStrengthen->StrenLvl ;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
	
	/*
	//成功
	if((int )RAND1(10000) < iRate*100 )
	{
		pstRolePet->StrengthenLevel++;

		pet_update_online(pstEnv, pstPlayer, pstRolePet);
		pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

		pstPetSvr->PetSvrData.Strengthen.SuccFlag = 1;

		if(pstRolePet->StrengthenLevel >= 8)
		{
			z_sys_strf_broadcast(pstEnv, pstPlayer, SYS_MIDDLE, 
				"%s 经过不懈努力，终于成功将宠物%s 强化到+%d",
				pstPlayer->stRoleData.RoleName, pstRolePet->Name, pstRolePet->StrengthenLevel);
		}
	}
	//强化失败
	else
	{
		pstPetSvr->PetSvrData.Strengthen.SuccFlag = 0;
	}

	pstPetSvr->PetSvrType = PET_SVR_STRENGTHEN;
	pstPetSvr->PetSvrData.Strengthen.Wid = pstRolePet->WID;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_STRENGTHEN, NULL);

	range_pet_set(pstEnv, pstPlayer, pstRolePet, pstPetDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstRolePet, pstPetDef));

	return 0;
	*/
}

int pet_wash_skill_grid_check(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETWASHSKILLGRID *pstCsWashSkill, 
								ITEMDEF *pstItemDef, int *piPos)
{
	if ( NULL == z_item_get_result(pstItemDef, RESULT_PET_WASH_SKILL_GRID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PET113);
		return -1;
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, PET_WASH_SKILL_GRID_MONEY))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
		return -1;
	}

	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstCsWashSkill->StuffData.ItemID, 1, 
							pstCsWashSkill->StuffData.ListType, piPos,PACKAGE_DEC_NONE))
	{
		return -1;
	}

	return 0;
}

int pet_wash_skill_grid(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETWASHSKILLGRID *pstCsWashSkill)
{
	//char szMsg[256];
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;
	int iSkillType;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	int iPos = -1;
	int iRet;
	
	pstRolePet = z_find_role_pet(pstPlayer,pstCsWashSkill->PetWid, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PET182);
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return -1;
	}
	
	if (pstCsWashSkill->SkillGridIdx >= pstRolePet->PetSkills.SkillGridNum)
	{
		return -1;
	}

	if (0 > package_get_item(pstEnv, pstPlayer, 
							pstCsWashSkill->StuffData.ListType, pstCsWashSkill->StuffData.GridIdx, 
							&pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;
	}
	
	if (0 > pet_wash_skill_grid_check(pstEnv, pstPlayer, pstCsWashSkill, pstItemDef, &iPos))
	{
		return -1;
	}

	if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
						pstCsWashSkill->StuffData.ItemID, 1, 
						pstCsWashSkill->StuffData.ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PETSKILLGRID))
	{
		return -1;
	}

	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, PET_WASH_SKILL_GRID_MONEY);
	if (0 > iRet)
	{
		return -1;
	}
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PET_WASHSKILL, pstRolePet->PetID, PET_WASH_SKILL_GRID_MONEY-iRet, iRet, "宠物技能栏");

	iSkillType = get_ran_skillgrid_type(pstRolePet);
	if (iSkillType)
	{
		pstRolePet->PetSkills.SkillGrid[pstCsWashSkill->SkillGridIdx].GridType = iSkillType;
		pstRolePet->PetSkills.SkillGrid[pstCsWashSkill->SkillGridIdx].SkillID = 0;
		pstRolePet->PetSkills.SkillGrid[pstCsWashSkill->SkillGridIdx].SkillLevel = 0;

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
			ZONEERR_PET175, pstRolePet->Name);

		pet_update_online(pstEnv, pstPlayer, pstRolePet);
		pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

		pet_svr_act(pstEnv, pstPlayer, PET_SVR_ACT_WASH_GRID);
	}
	
	return 0;
}

int pet_watch(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETCLTWATCH *pstCSWash)
{
	Player *pstWatchPlayer;
	PetFightShiOnline *pstPetFightShiOnline;
	ROLEPET *pstRolePet;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETWATCHINFO *pstWatchInfo = &pstPetSvr->PetSvrData.WatchInfo;

	pstWatchPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstCSWash->MemID%PET_ID_BASE);
	if (NULL == pstWatchPlayer)
	{
		return -1;
	}

	pstPetFightShiOnline = pet_get_shipet_by_xiedaipos(pstWatchPlayer, (pstCSWash->MemID/PET_ID_BASE)-1);
	if (NULL == pstPetFightShiOnline)
	{
		return -1;
	}

	pstRolePet = z_find_role_pet(pstWatchPlayer, pstPetFightShiOnline->stRolePetOnline.ullWID, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	pstPetSvr->PetSvrType = PET_SVR_WATCH;
	STRNCPY(pstWatchInfo->RoleName, 
				pstWatchPlayer->stRoleData.RoleName, 
				CCH(pstWatchInfo->RoleName));
	
	memcpy(&pstWatchInfo->Pet, pstRolePet, sizeof(pstWatchInfo->Pet));

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

// 切换主动/被动模式
 //int pet_set_passive(ZONESVRENV *pstEnv, Player *pstPlayer, char fPassive)PETCHGMODE ChgMode
int pet_set_passive(ZONESVRENV *pstEnv, Player *pstPlayer, PETCHGMODE *pstChgMode)
{
	int i = 0;
	//PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	//int fHasSetup = 0;
	ROLEPET *pstRolePet;
	
	for (i=0; i<pstPlayer->stRoleData.Package.PetPackage.Num; i++)
	{
		pstRolePet = &pstPlayer->stRoleData.Package.PetPackage.RolePets[i];
		if (pstRolePet->WID == pstChgMode->PetWID)
		{
			pstRolePet->Passive = pstChgMode->Passive;
			z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			pet_player_view_action(pstEnv, pstPlayer);
			break;
		}
	}
	/*for(i = 0; i < pstPetOnline->bFightShiNum; ++i)
	{
		ROLEPET *pstRolePet = z_find_role_pet(
			pstPlayer,
			pstPetOnline->astFightShi[i].stRolePetOnline.ullWID,
			NULL);
		if(pstRolePet)
		{
			pstRolePet->Passive = fPassive;
			fHasSetup = 1;
		}
	}
	if(fHasSetup)
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		pet_player_view_action(pstEnv, pstPlayer);
	}*/
	return 0;
}

int get_pet_fuse_rate(ROLEPET *pstRolePet)
{
	if(pstRolePet->PetGenius == PET_GENIUS_B)
	{
		return 50;
	}
	else if(pstRolePet->PetGenius == PET_GENIUS_A)
	{
		return 70;
	}
	else if(pstRolePet->PetGenius == PET_GENIUS_S)
	{

		return 90;
	}
	else
	{
		return 0;
	}

	return 0;
}

int get_pet_fuse_genius(ZONESVRENV* pstEnv ,ROLEPET *pstRolePet, ROLEPET *pstOtherRolePet)
{
	int i;
	int iRate, iRand;
	PETFUSEDEF *pstFuseDef;

	pstFuseDef = z_find_pet_fuse_def(pstEnv, pstRolePet->PetGenius+1, pstOtherRolePet->PetGenius+1);
	if(NULL == pstFuseDef)
	{
		return -1;
	}

	iRate = 0;
	iRand = RAND1(100);
	for(i=0; i<MAX_PET_GENIUS; i++)
	{
		if(pstFuseDef->FuseGenius[i] == 0)
		{
			continue;
		}

		iRate += pstFuseDef->FuseGenius[i];
		if(iRand < iRate)
		{
			return i;
		}
	}
	
	return -1;
}

int pet_fuse_check(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOtherPlayer)
{
	LEVELUP *pstLevelUp;
	ROLEPET *pstRolePet, *pstOtherRolePet;
	PETDEF *pstPetDef, *pstBabyPetDef;
	int iMoney;
	
	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if (player_check_safelock(pstEnv, pstOtherPlayer) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET127);
		return -1;
	}

	if( INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		if(!is_together_world(pstEnv, pstPlayer, pstOtherPlayer))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_PET128);
			return -1;
		}
	}

	if (pstOtherPlayer->stOnline.bWaitRoleLogout)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET129);
		return -1;
	}

	if (pstOtherPlayer->stOnline.State & CS_STAT_DEAD )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET130);
		return -1;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if (pstOtherPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, 0);
	}

	if (0 > z_check_op_dist(pstEnv, pstPlayer, pstOtherPlayer->stRoleData.Map, &pstOtherPlayer->stRoleData.Pos, 1000 + 100))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET131);
		return -1;
	}

	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if (NULL == pstRolePet)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET132);
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (NULL == pstPetDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET133);
		return -1;
	}

	pstBabyPetDef = z_find_pet_def(pstEnv, pstPetDef->BabyPetID, NULL);
	if (NULL == pstBabyPetDef)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET133);
		return -1;
	}

	if(pstRolePet->PetGenius < PET_GENIUS_B)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET134);
		return -1;
	}

	if(pstRolePet->Hierarchy > 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET135);
		return -1;
	}

	pstLevelUp = z_get_levelup(pstEnv, pstRolePet->Level);
	if (!pstLevelUp)
	{
		return -1;
	}

	if(pstRolePet->Level < 50)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET136);
		return -1;
	}

	iMoney = (pstLevelUp->ContributeMoney/2)/100*100;

	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
		return -1;
	}

	pstOtherRolePet = pet_get_camp(pstEnv, pstOtherPlayer);
	if (NULL == pstOtherRolePet)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET137);
		return -1;
	}

	if(pstOtherRolePet->PetGenius < PET_GENIUS_B)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET138);
		return -1;
	}

	if(pstOtherRolePet->Hierarchy > 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET139);
		return -1;
	}

	if(pstRolePet->Level < 50)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET140);
		return -1;
	}

	pstLevelUp = z_get_levelup(pstEnv, pstOtherRolePet->Level);
	if (!pstLevelUp)
	{
		return -1;
	}

	iMoney = (pstLevelUp->ContributeMoney/2)/100*100;

	if (0 > package_dec_money_test(pstEnv, pstOtherPlayer, ATTR_ID_BULL, iMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
		return -1;
	}

	if(pstOtherRolePet->PetID != pstRolePet->PetID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET141);
		return -1;
	}

	return 0;
}

int pet_fuse_invite(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETFUSEINVITE *pstCSInvite)
{
	Player *pstOtherPlayer;
	PetFightShiOnline *pstPetFightShiOnline;
	ROLEPET *pstOtherRolePet;
	ROLEPET *pstRolePet;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;

	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if(NULL == pstRolePet)
	{
		return -1;
	}

	pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstCSInvite->MemID%PET_ID_BASE);
	if (NULL == pstOtherPlayer)
	{
		return -1;
	}

	pstPetFightShiOnline = pet_get_shipet_by_xiedaipos(pstOtherPlayer, (pstCSInvite->MemID/PET_ID_BASE)-1);
	if (NULL == pstPetFightShiOnline)
	{
		return -1;
	}

	pstOtherRolePet = z_find_role_pet(pstOtherPlayer, pstPetFightShiOnline->stRolePetOnline.ullWID, NULL);
	if (NULL == pstOtherRolePet)
	{
		return -1;
	}

	if (pstPlayer == pstOtherPlayer)
	{
		return -1;
	}

	if (pstPlayer->stOnline.stPetFuseInfo.bStat != PET_FUSE_NO )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET142);
		return -1;
	}

	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstPlayer->stOnline.stPetFuseInfo.iInViteTime < 10)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET143);
		return -1;
	}

	if (pstOtherPlayer->stOnline.stPetFuseInfo.bStat != PET_FUSE_NO )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET144);
		return -1;
	}
	
	if (pstEnv->pstAppCtx->stCurr.tv_sec - pstOtherPlayer->stOnline.stPetFuseInfo.iInViteTime < 10)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET145);
		return -1;
	}

	if(0 > pet_fuse_check(pstEnv, pstPlayer, pstOtherPlayer))
	{
		return -1;
	}
	
	pstOtherPlayer->stOnline.stPetFuseInfo.iInViteTime = pstEnv->pstAppCtx->stCurr.tv_sec + 10;
	pstOtherPlayer->stOnline.stPetFuseInfo.iMemID = pstPlayer->iMemID;

	pstPetSvr->PetSvrType = PET_SVR_FUSE_INVITE;
	pstPetSvr->PetSvrData.FuseInvite.MemID = pstPlayer->iMemID;
	pstPetSvr->PetSvrData.FuseInvite.Pet = *pstRolePet;
	STRNCPY(pstPetSvr->PetSvrData.FuseInvite.RoleName, pstPlayer->stRoleData.RoleName, CCH(pstPetSvr->PetSvrData.FuseInvite.RoleName));

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);

	return 0;
}

int pet_fuse_reject(ZONESVRENV* pstEnv, Player *pstPlayer,  int iMemID)
{
	Player *pstOtherPlayer;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;

	pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iMemID);
	if (NULL == pstOtherPlayer)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stPetFuseInfo.iMemID == pstOtherPlayer->iMemID)
	{
		pstPlayer->stOnline.stPetFuseInfo.iInViteTime = 0;
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstOtherPlayer, SYS_MIDDLE2, ZONEERR_PET176, pstPlayer->stRoleData.RoleName);

	pstPetSvr->PetSvrType = PET_SVR_FUSE_REJECT;
	pstPetSvr->PetSvrData.FuseReject.MemID = pstPlayer->iMemID;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);

	return 0;
}


int pet_fuse_cancel(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	Player *pstOtherPlayer;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;

	if (pstPlayer->stOnline.stPetFuseInfo.bStat == PET_FUSE_NO )
	{
		return -1;
	}

	pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stPetFuseInfo.iMemID);
	if (NULL == pstOtherPlayer)
	{
		return -1;
	}
	
	if(pstOtherPlayer->stOnline.stPetFuseInfo.bStat == PET_FUSE_NO )
	{
		return -1;
	}
	
	if(pstPlayer->stOnline.stPetFuseInfo.bStat == PET_FUSE_READY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET177);
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstOtherPlayer, SYS_MIDDLE2, ZONEERR_PET177);
	}

	memset(&pstPlayer->stOnline.stPetFuseInfo, 0, sizeof(pstPlayer->stOnline.stPetFuseInfo));
	memset(&pstOtherPlayer->stOnline.stPetFuseInfo, 0, sizeof(pstOtherPlayer->stOnline.stPetFuseInfo));
	
	pstPetSvr->PetSvrType = PET_SVR_FUSE_CANCEL;
	pstPetSvr->PetSvrData.FuseCancel.MemID = pstPlayer->iMemID;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);

	return 0;
}

int pet_fuse_accept(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETFUSEACCEPT  *pstCSAccept)
{
	Player *pstOtherPlayer;
	ROLEPET *pstRolePet;
	ROLEPET *pstOtherRolePet;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;

	pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstCSAccept->MemID);
	if (NULL == pstOtherPlayer)
	{
		return -1;
	}

	if (pstPlayer == pstOtherPlayer || pstPlayer->stOnline.stPetFuseInfo.iMemID != pstOtherPlayer->iMemID)
	{
		return -1;
	}
	
	pstOtherPlayer->stOnline.stPetFuseInfo.iInViteTime= 0;
	if(0 > pet_fuse_check(pstEnv, pstPlayer, pstOtherPlayer))
	{
		return -1;
	}

	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	pstOtherRolePet = pet_get_camp(pstEnv, pstOtherPlayer);
	if (NULL == pstOtherRolePet)
	{
		return -1;
	}

	pstPlayer->stOnline.stPetFuseInfo.bStat = PET_FUSE_READY;

	//锁定宠物
	pstPlayer->stOnline.stPetFuseInfo.ullPetWid = pstRolePet->WID;
	pstPlayer->stOnline.stPetFuseInfo.ullOtherPetWid= pstOtherRolePet->WID;
	pstPlayer->stOnline.stPetFuseInfo.iMemID = pstOtherPlayer->iMemID;

	pstOtherPlayer->stOnline.stPetFuseInfo.bStat = PET_FUSE_READY;
		
	//锁定宠物
	pstOtherPlayer->stOnline.stPetFuseInfo.ullPetWid = pstOtherRolePet->WID;
	pstOtherPlayer->stOnline.stPetFuseInfo.ullOtherPetWid= pstRolePet->WID;
	pstOtherPlayer->stOnline.stPetFuseInfo.iMemID = pstPlayer->iMemID;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	pstPetSvr->PetSvrType = PET_SVR_FUSE_UI;

	//发送双方宠物信息各至对方
	pstPetSvr->PetSvrData.FuseUI.Pet = *pstOtherRolePet;
	STRNCPY(pstPetSvr->PetSvrData.FuseUI.RoleName, pstOtherPlayer->stRoleData.RoleName, CCH(pstPetSvr->PetSvrData.FuseUI.RoleName));
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);


	pstPetSvr->PetSvrData.FuseUI.Pet = *pstRolePet;
	STRNCPY(pstPetSvr->PetSvrData.FuseUI.RoleName, pstPlayer->stRoleData.RoleName, CCH(pstPetSvr->PetSvrData.FuseUI.RoleName));
	z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);
	
	return 0;
}


// 出生获得指定天赋
int pet_native_skill(ZONESVRENV* pstEnv, PETDEF *pstPetDef, ROLEPET *pstRolePet)
{
	int iRand = RAND1(10000);
	DIATHESISDEF *pstDiaDef;
	//PETNATIVESKILLDEF *pstPetNativeDef;
	
	if (iRand >= pstPetDef->NativeSkill.Rate)
	{
		return -1;
	}
	
	pstDiaDef = z_find_diathesis(pstEnv, pstPetDef->NativeSkill.SkillID, 1);
	if (NULL == pstDiaDef)
	{
		return -1;
	}

	pstRolePet->NativeSkill.SkillID = pstDiaDef->DiathesisID;
	pstRolePet->NativeSkill.SkillLevel = pstDiaDef->Level;
	pstRolePet->NativeSkill.GridType = pstDiaDef->PetDiathesisType;
	
	return 0;
}

int pet_native_add_money(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPET *pstRolePet;
	DIATHESISDEF *pstDiathesisDef;
	int i;

	pstRolePet = pet_find_fight(pstPlayer);
	if (!pstRolePet)
	{
		return 0;
	}

	pstDiathesisDef = z_find_diathesis(pstEnv, pstRolePet->NativeSkill.SkillID, pstRolePet->NativeSkill.SkillLevel);
	if(!pstDiathesisDef)	
	{
		return 0;
	}

	for (i=0; i<(int)(sizeof(pstDiathesisDef->DiathesisResult)/sizeof(pstDiathesisDef->DiathesisResult[0])); i++)
	{
		if (pstDiathesisDef->DiathesisResult[i].SkillResultID == RESULT_PET_OWNER_BEACH_MONEY)
		{
			package_add_money(pstEnv, pstPlayer, pstDiathesisDef->DiathesisResult[i].SkillResultVal1,
										pstDiathesisDef->DiathesisResult[i].SkillResultVal2);
		}
	}
	return 0;
}

int pet_native_skill_result(ZONESVRENV* pstEnv, Player *pstPlayer,int iType, int* pValue,ROLEPET *pstRolePet)
{
	PETNATIVEINFO* pstNativeInfo = NULL;
	PetFightAttrOnline stPetFightOnline;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

	if (!pValue)
	{
		return -1;
	}

	if (pstPetOnline->bFightShiNum > 0)
	{
		pstNativeInfo = &pstPetOnline->astFightShi[0].stPetFightAttrOnline.stNativeInfo;
		if (pstRolePet && pstPetOnline->astFightShi[0].stRolePetOnline.ullWID != pstRolePet->WID)
		{
			pstNativeInfo = NULL;
		}		
	}
	
	if (pstRolePet && NULL == pstNativeInfo)
	{
		memset(&stPetFightOnline,0,sizeof(stPetFightOnline));
		pstNativeInfo = &stPetFightOnline.stNativeInfo;
		if(pstRolePet->NativeSkill.SkillID > 0 && pstRolePet->NativeSkill.GridType == PET_SKILL_SELF_BUFF)
		{
			pet_attr_diathesis(pstEnv, pstPlayer,&pstRolePet->NativeSkill, NULL, NULL, &stPetFightOnline, NULL);
		}	
		
		//宠物爱心等级
		{
			PETLOVELEVELDEF* pstLoveLevelDef = z_find_pet_love_level_def(pstEnv,pstRolePet->LoveLevel);
			if (pstLoveLevelDef && pstLoveLevelDef->DiathesisInfo.SkillID &&
				pstLoveLevelDef->DiathesisInfo.SkillLevel)
			{
				pet_attr_diathesis(pstEnv, pstPlayer,&pstLoveLevelDef->DiathesisInfo, NULL, NULL, &stPetFightOnline, NULL);
			}
		}
	}
	
	if (!pstNativeInfo || 0 == (pstNativeInfo->iFlag & iType))
	{		
		return -1;
	}
	switch(iType)
	{
	case RESULT_NATIVE_EXP_ADD_MAP:
		(*pValue) += pstNativeInfo->sExpAdd/10000.0f * (*pValue);
		break;
	case RESULT_NATIVE_MAX_LOVE_VAL_ADD_MAP:
		(*pValue) += pstNativeInfo->sMaxLoveValAdd;
		break;
	case RESULT_NATIVE_ITEM_HP_ADD_MAP:
		(*pValue) += pstNativeInfo->sItemHpAdd/10000.0f * (*pValue);
		break;
	case RESULT_NATIVE_OWNER_DIE_EXP_MAP:
		(*pValue) -= pstNativeInfo->sOwnerDieExp/10000.0f * (*pValue);
		if ((*pValue) < 0)
		{
			(*pValue) = 0;
		}
		break;
	case RESULT_NATIVE_OWNER_DIE_MONEY_MAP:
		(*pValue) -= pstNativeInfo->sOwnerDieMoney/10000.0f * (*pValue);
		if ((*pValue) < 0)
		{
			(*pValue) = 0;
		}
		break;
	case RESULT_NATIVE_XP_ADD_MAP:
		(*pValue) += pstNativeInfo->sXpAdd/10000.0f * (*pValue);
		break;
	case RESULT_NATIVE_REPAIR_COST_MAP:
		(*pValue) -= pstNativeInfo->sRepairCost/10000.0f * (*pValue);
		if ((*pValue) < 0)
		{
			(*pValue) = 0;
		}
		break;
	case RESULT_NATIVE_LOVE_VAL_DIE_MAP:
		(*pValue) = 0;
		break;
	default:
		break;
	}
	return 0;
}

int pet_born_update(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet)
{
	CSPKG stPkg;
	CSPETSVR *pstPetSvr  = &stPkg.Body.PetSvr;
	CSPETATTRCHG *pstPetChgData= &pstPetSvr->PetSvrData.AttrChg;

	//宠物更新各项属性
	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	pstRolePet->HP = pstRolePet->MaxHP;

	pstPetSvr->PetSvrType = PET_ATTR_CHG;
	pstPetChgData->Type = PET_CHG_HP;
	pstPetChgData->Wid = pstRolePet->WID;
	pstPetChgData->AttrValue.ChgHP.HP = pstRolePet->HP;
	pstPetChgData->AttrValue.ChgHP.XieDaiPos = -1;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
	
}

int pet_fuse_skill_data(ZONESVRENV* pstEnv, PETDEF *pstPetDef, PETSKILL *pstPetSkill, ROLEPET *pstRolePet)
{
	int i;
	PETSKILLGRIDDEF *pstSkillGridDef;

	for(i=0; i<pstPetSkill->SkillGridNum; i++)
	{
		if(pstPetSkill->SkillGrid[i].SkillID == 0)
		{
			continue;
		}
		
		pstSkillGridDef = z_find_pet_skillgrid_def(pstEnv, i+1);
		if(NULL == pstSkillGridDef)
		{
			continue;
		}
		
		if(RAND1(100) < pstSkillGridDef->TransferRate)
		{
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillID = pstPetSkill->SkillGrid[i].SkillID;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillLevel = pstPetSkill->SkillGrid[i].SkillLevel;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].GridType = pstPetSkill->SkillGrid[i].GridType;
			pstRolePet->PetSkills.SkillGridNum++;
		}
	}
	
	if (pstPetDef->BornSkillGrid && pstRolePet->PetSkills.SkillGridNum == 0)
	{
		pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillID = 0;
		pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillLevel = 0;
		pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].GridType = 0;
		pstRolePet->PetSkills.SkillGridNum++;
	}
	
	return 0;
}

//数值  110% - 115%范围内随即
int pet_fuse_get_ran_genius(int iGeniusVal)
{
	int iMinVal, iMaxVal;
	int iVal;

	iMinVal = iGeniusVal*1.1;
	iMaxVal = iGeniusVal*1.15;

	if (iMaxVal > iMinVal)
	{
		iVal = iMinVal + RAND1(iMaxVal-iMinVal);
	}
	else
	{
		iVal = iGeniusVal;
	}

	return iVal;
}

int pet_fuse_genius_data(ZONESVRENV* pstEnv, PETDEF *pstPetDef, ROLEPET *pstRolePet, int iGenius, int iGeniusAdd)
{
	
	PETIMPUPATTRDEF *pstDef = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp, pstPetDef->PetAttrType);
	if (!pstDef)
		return -1;

	PETGENIUS *pstGenius = &pstDef->Genius[iGenius];
	
	//新宠物资质
	pstRolePet->PetGenius = iGenius;
	pstRolePet->PhsicAtkGenius = pet_fuse_get_ran_genius(pstGenius->PhsicAtkGenius)+iGeniusAdd;
	pstRolePet->IceAtkGenius = pet_fuse_get_ran_genius(pstGenius->IceAtkGenius)+iGeniusAdd;
	pstRolePet->HPGenius = pet_fuse_get_ran_genius(pstGenius->HPGenius)+iGeniusAdd;
	pstRolePet->DefGenius = pet_fuse_get_ran_genius(pstGenius->DefGenius)+iGeniusAdd;
	pstRolePet->HitRateGenius = pet_fuse_get_ran_genius(pstGenius->HitRateGenius)+iGeniusAdd;
	pstRolePet->FleeGenius = pet_fuse_get_ran_genius(pstGenius->FleeGenius)+iGeniusAdd;
	pstRolePet->IceDefGenius = pet_fuse_get_ran_genius(pstGenius->IceDefGenius)+iGeniusAdd;

	return 0;
}

int pet_fuse_baby_data(ZONESVRENV* pstEnv, PETDEF *pstPetDef, ROLEPET *pstRolePet, 
	int iGenius, int iGeniusAdd, PETSKILL *pstOldSkill)
{
	PETLEVELDEF *pstPetLevelDef;
	
	pstPetLevelDef = z_find_pet_level_def(pstEnv, 1, NULL);
	if (NULL == pstPetDef)
	{
		return -1;
	}

	//基本属性
	pstRolePet->PetID = pstPetDef->PetID;
	STRNCPY(pstRolePet->Name, pstPetDef->Name, sizeof(pstRolePet->Name));
	pstRolePet->WID =z_get_gid(pstEnv->pstAppCtx, pstEnv);
	pstRolePet->LoveVal = BORN_PET_LOVE_VAL;
	pstRolePet->Level = 1;
	pstRolePet->NextStrenLvl = 1;

	//第二代宠物
	pstRolePet->Hierarchy = 2;

	//获得资质
	pet_fuse_genius_data(pstEnv,pstPetDef, pstRolePet, iGenius, iGeniusAdd);
	
	//获得天赋
	pet_native_skill(pstEnv, pstPetDef, pstRolePet);

	//技能继承
	pet_fuse_skill_data(pstEnv, pstPetDef, pstOldSkill, pstRolePet);

	return 0;
}

ROLEPET* pet_fuse_baby(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRoleOldPet, int iGenius, int iGeniusAdd)
{
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef, *pstBabyPetDef;
	PETSKILL stPetSkill;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;	

	pstPetDef = z_find_pet_def(pstEnv, pstRoleOldPet->PetID, NULL);
	if (NULL == pstPetDef)
	{
		//error
		return NULL;
	}

	pstBabyPetDef = z_find_pet_def(pstEnv, pstPetDef->BabyPetID, NULL);
	if(NULL == pstBabyPetDef)
	{
		return NULL;
	}

	//继承
	stPetSkill = pstRoleOldPet->PetSkills;
	
	//清除老的
	if(0 > pet_del(pstEnv, pstPlayer, pstRoleOldPet->WID, 2, NULL))
	{
		//error
		return NULL;
	}

	pstRolePet = &pstPetPackage->RolePets[pstPetPackage->Num];
	memset(pstRolePet, 0, sizeof(*pstRolePet));
	
	if(0 > pet_fuse_baby_data(pstEnv, pstBabyPetDef, pstRolePet,iGenius, iGeniusAdd, &stPetSkill))
	{	
		//error
		return NULL;
	}
	pstPetPackage->Num++;

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PET178, pstRolePet->Name);

	//通知客户端
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	
	//更新属性信息
	pet_born_update(pstEnv, pstPlayer, pstRolePet);

	range_pet_set(pstEnv, pstPlayer, pstRolePet, pstBabyPetDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstRolePet, pstBabyPetDef));

		//成长之路
	{
		ACHIEVEMENTCOND stCond;
	
		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_PET_GET;
		stCond.Val1 = 0;
	
		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);

		if (pstPetDef->Type == PET_TYPE_ABERRANCE)
		{
			stCond.Val1 = 1;
	
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}

		if (pstRolePet && pstRolePet->PetGenius == PET_GENIUS_S)
		{
			stCond.Val1 = 2;
	
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
		
	}

	return pstRolePet;
}

/*
int pet_fuse_process(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOtherPlayer, CSPETFUSESUCC *pstCSFuseSucc,
	ROLEPET *pstRolePet, ROLEPET *pstOtherRolePet)
{
	int iRate;
	int iGenius;
	int iGeniusAdd =0;
	LEVELUP *pstLevelUp;
	ROLEPET *pstBaby, *pstOtherBaby;
	int iMoney;
	int iLevel;
	USEMONEY stUse;
	

	pstLevelUp = z_get_levelup(pstEnv, pstRolePet->Level);
	if (!pstLevelUp)
	{
		return -1;
	}

	iMoney = (pstLevelUp->ContributeMoney/2)/100*100;
	
	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, iMoney))
	{
		return -1;
	}

	pstLevelUp = z_get_levelup(pstEnv, pstOtherRolePet->Level);
	if (!pstLevelUp)
	{
		return -1;
	}

	iMoney = (pstLevelUp->ContributeMoney/2)/100*100;
	
	if (0 > package_dec_money(pstEnv, pstOtherPlayer, ATTR_ID_BULL, iMoney))
	{
		return -1;
	}

	iRate = (get_pet_fuse_rate(pstRolePet)+get_pet_fuse_rate(pstOtherRolePet))/2;
	if(iRate  <= RAND1(100) )
	{
		return -1;
	}

	iGenius = get_pet_fuse_genius(pstEnv, pstRolePet, pstOtherRolePet);
	if(iGenius < 0 || iGenius >= MAX_PET_GENIUS)
	{
		return -1;
	}

	z_oplog_event_begin(pstEnv);

	//平均等级
	iLevel = (pstOtherRolePet->Level+pstRolePet->Level)/2;

	//第四次修改 2011-11-24
	//power(lvn,3)/12000 + power(An,3)/18+150点 两宠物平均等级,宠物强化等级是一对一
	iGeniusAdd = pow(iLevel, 3)/12000;
	iGeniusAdd += pow(pstRolePet->StrengthenLevel, 3)/18;
	iGeniusAdd += pow(pet_strengthen_val_get(pstRolePet), 3)/1200;
	iGeniusAdd += 150;

	bzero(&stUse, sizeof(stUse));
	stUse.UseMoney_Money = iMoney;
	pstBaby = pet_fuse_baby(pstEnv, pstPlayer,  pstRolePet, iGenius, iGeniusAdd);
	if(pstBaby)
	{
		//运营日志
		z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_FUSE, NULL, NULL, &stUse,NULL);
		
		pstCSFuseSucc->Baby.MemID = pstPlayer->iMemID;
		pstCSFuseSucc->Baby.Pet = *pstBaby;
	}

	//power(lvn,3)/12000 + power(An,3)/18+150点 两宠物平均等级,宠物强化等级是一对一
	iGeniusAdd = pow(iLevel, 3)/12000;
	iGeniusAdd += pow(pstOtherRolePet->StrengthenLevel, 3)/18;
	iGeniusAdd += pow(pet_strengthen_val_get(pstOtherRolePet), 3)/1200;
	iGeniusAdd += 150;
	
	pstOtherBaby = pet_fuse_baby(pstEnv, pstOtherPlayer,  pstOtherRolePet, iGenius, iGeniusAdd);
	if(pstOtherBaby)
	{
		//运营日志
		z_role_pet_oplog( pstEnv, pstOtherPlayer, pstOtherRolePet, NULL, OP_PET_FUSE, NULL, NULL, &stUse,NULL);
		
		pstCSFuseSucc->Baby.OtherMemID = pstOtherPlayer->iMemID;
		pstCSFuseSucc->Baby.OtherPet = *pstOtherBaby;
	}
	
	return 0;
}*/

/*
int pet_fuse_deal(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETFUSEDEAL *pstCSDeal)
{
	Player *pstOtherPlayer;
	ROLEPET *pstRolePet;
	ROLEPET *pstOtherRolePet;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	UNUSED(pstCSDeal);

	pstOtherPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stPetFuseInfo.iMemID);
	if (NULL == pstOtherPlayer || pstOtherPlayer->stOnline.stPetFuseInfo.iMemID != pstPlayer->iMemID)
	{
		goto _error;
	}

	if (pstOtherPlayer->stOnline.stPetFuseInfo.bStat == PET_FUSE_NO ||
		pstPlayer->stOnline.stPetFuseInfo.bStat == PET_FUSE_NO)
	{
		goto _error;
	}

	if(0 > pet_fuse_check(pstEnv, pstPlayer, pstOtherPlayer))
	{
		goto _error;
	}

	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if (NULL == pstRolePet || pstRolePet->WID != pstPlayer->stOnline.stPetFuseInfo.ullPetWid)
	{
		goto _error;
	}

	pstOtherRolePet = pet_get_camp(pstEnv, pstOtherPlayer);
	if (NULL == pstOtherRolePet || pstOtherRolePet->WID != pstPlayer->stOnline.stPetFuseInfo.ullOtherPetWid)
	{
		goto _error;
	}

	pstPlayer->stOnline.stPetFuseInfo.bStat = PET_FUSE_DEAL;
		
	pstPetSvr->PetSvrType = PET_SVR_FUSE_DEAL;
	pstPetSvr->PetSvrData.FuseDeal.MemID = pstPlayer->iMemID;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	if(pstOtherPlayer->stOnline.stPetFuseInfo.bStat != PET_FUSE_DEAL)
	{
		return 0;
	}

	//成功
	if(0 == pet_fuse_process(pstEnv, pstPlayer, pstOtherPlayer, &pstPetSvr->PetSvrData.FuseSucc, pstRolePet, pstOtherRolePet))
	{
		//成功之后就只能关闭UI了
		memset(&pstPlayer->stOnline.stPetFuseInfo, 0, sizeof(pstPlayer->stOnline.stPetFuseInfo));
		memset(&pstOtherPlayer->stOnline.stPetFuseInfo, 0, sizeof(pstOtherPlayer->stOnline.stPetFuseInfo));

		//成功
		pstPetSvr->PetSvrType = PET_SVR_FUSE_SUCC;
		pstPetSvr->PetSvrData.FuseSucc.Succ = 1;

		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);
	}
	//失败
	else
	{
		//失败了可以再来
		pstPlayer->stOnline.stPetFuseInfo.bStat = PET_FUSE_READY;
		pstOtherPlayer->stOnline.stPetFuseInfo.bStat = PET_FUSE_READY;

		//失败
		pstPetSvr->PetSvrType = PET_SVR_FUSE_SUCC;
		pstPetSvr->PetSvrData.FuseSucc.Succ = 0;

		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);
	}

	return 0;
	
_error:

	memset(&pstPlayer->stOnline.stPetFuseInfo, 0, sizeof(pstPlayer->stOnline.stPetFuseInfo));
	
	pstPetSvr->PetSvrType = PET_SVR_FUSE_CANCEL;
	pstPetSvr->PetSvrData.FuseCancel.MemID = pstPlayer->iMemID;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	if(pstOtherPlayer)
	{
		memset(&pstOtherPlayer->stOnline.stPetFuseInfo, 0, sizeof(pstOtherPlayer->stOnline.stPetFuseInfo));

		pstPetSvr->PetSvrType = PET_SVR_FUSE_CANCEL;
		pstPetSvr->PetSvrData.FuseCancel.MemID = pstOtherPlayer->iMemID;

		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstOtherPlayer, &stPkg);
	}
	
	return 0;
}*/

/*
int pet_add_skill_grid(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETADDSKILLGRID *pstCSSkill)
{
	ROLEPET *pstRolePet = NULL;
	PETSKILL *pstPetSkills;    
	PETSKILLGRIDDEF *pstSkillGridDef;

	pstRolePet = z_find_role_pet(pstPlayer, pstCSSkill->PetWid, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	if(pstRolePet->PetSkills.SkillGridNum >= MAX_PET_SKILL_GRID)
	{
		return -1;
	}

	pstSkillGridDef = z_find_pet_skillgrid_def(pstEnv, pstRolePet->PetSkills.SkillGridNum+1);
	if(NULL == pstSkillGridDef)
	{
		return -1;
	}
	
	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_BULL, pstSkillGridDef->Money))
	{
		return -1;
	}

	pstPetSkills = &pstRolePet->PetSkills;
	pstPetSkills->SkillGrid[pstPetSkills->SkillGridNum].SkillID = 0;
	pstPetSkills->SkillGrid[pstPetSkills->SkillGridNum].SkillLevel = 0;
	pstPetSkills->SkillGrid[pstPetSkills->SkillGridNum].GridType = 0;
	pstPetSkills->SkillGridNum++;

	z_sys_strf( pstEnv, pstPlayer, SYS_MIDDLE, 
			"宠物[%s]获得了新的技能栏位 ", pstRolePet->Name);

	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	pet_svr_act(pstEnv, pstPlayer, PET_SVR_ACT_ADD_SKILLGRID);
	
	return 0;
}*/

int pet_strengthen_save(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETSTRENGTHENSAVE *pstCSSave)
{
	PETDEF *pstPetDef;
	ROLEPET *pstRolePet = NULL;
	int iTip = 0;
	PETSTRENINFO stPetStrenInfo;
	
	if(pstPlayer->stOnline.stPetStrengthenUIInfo.ullPetWid != pstCSSave->PetWid)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stPetStrengthenUIInfo.iAddVal != pstCSSave->Val)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stPetStrengthenUIInfo.cReStrenLvl != pstCSSave->StrenLvl)
	{
		return -1;
	}

	pstRolePet = z_find_role_pet(pstPlayer, pstCSSave->PetWid, NULL);
	if (NULL == pstRolePet || pstRolePet->PetID != pstCSSave->PetID)
	{
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (NULL == pstPetDef)
	{
		return -1;
	}

	stPetStrenInfo.Star = pstCSSave->StrenLvl;
	if (pstCSSave->StrenLvl > pstRolePet->StrengthenLevel )
	{
		if (pstRolePet->StrengthenLevel >= MAX_PET_STRENGTHEN_LEVEL)
			return -1;

		stPetStrenInfo.PreVal = pstRolePet->StrengthenInfo[pstRolePet->StrengthenLevel];
		pstRolePet->StrengthenInfo[pstRolePet->StrengthenLevel] = pstCSSave->Val;
		pstRolePet->StrengthenLevel++;
		iTip = 1;
	}
	else
	{
		stPetStrenInfo.PreVal = pstRolePet->StrengthenInfo[pstCSSave->StrenLvl  -1];
		pstRolePet->StrengthenInfo[pstCSSave->StrenLvl -1] = pstCSSave->Val;
	}

	stPetStrenInfo.LastVal = pstCSSave->Val;
	//pstRolePet->NextStrenLvl  = pstRolePet->StrengthenLevel +1;
	//if (pstRolePet->NextStrenLvl  > MAX_PET_STRENGTHEN_LEVEL)
	//{
	//	pstRolePet->NextStrenLvl = 0;
	//}

	/*
	//占绑即绑
	if(pstPlayer->stOnline.stPetStrengthenUIInfo.iBindNum > 0)
	{
		// 不离不弃
		if (pstEnv->pstZoneObj->iPetLoveLevelDef)
		{
			pstRolePet->LoveLevel = pstEnv->pstZoneObj->iPetLoveLevelDef-1;
		}
	}
	*/

	memset(&pstPlayer->stOnline.stPetStrengthenUIInfo, 0, sizeof(pstPlayer->stOnline.stPetStrengthenUIInfo));

	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	if(pstCSSave->Val == 10)
	{
		z_sys_strf_broadcast(pstEnv, SYS_SHOW, 
			ZONEERR_BROAD331,
			pstPlayer->stRoleData.RoleName, pstRolePet->Name);
	}
	
	task_pet_trace(pstEnv, pstPlayer, pstRolePet, TASK_OBJ_PET_STRENGTHEN);

	z_oplog_event_begin(pstEnv);

	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_STRENGTHEN, NULL, NULL, NULL,&stPetStrenInfo);
	range_pet_set(pstEnv, pstPlayer, pstRolePet, pstPetDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstRolePet, pstPetDef));
	
	return 0;
}

int pet_restrengthen(ZONESVRENV* pstEnv, Player *pstPlayer,CSPETRESTREN *pstPetReStren)
{
	ROLEPET *pstRolePet;
	int iRet;
	ITEMDEC stDec;

	pstRolePet = z_find_role_pet(pstPlayer,pstPetReStren->PetWid, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	if (	pstPetReStren->StrenLvl > pstRolePet->StrengthenLevel || 
		pstPetReStren->StrenLvl <= 0 || 
		pstPetReStren->StrenLvl > MAX_PET_STRENGTHEN_LEVEL)
	{
		return -1;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PET146);
		return -1;
	}

	if (pstRolePet->StrengthenInfo[(int)(pstPetReStren->StrenLvl-1)] == 10)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET147);
		return -1;
	}


	if (	pstRolePet->NextStrenLvl != 0 &&
		pstRolePet->NextStrenLvl <= pstRolePet->StrengthenLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET148);
		return -1;
	}


	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, RESTREN_ITEMID, 1, -1, 
		NULL, PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PETSTRENG);
	if (iRet < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_NOITEM);
		return -1;
	}

	pstRolePet->NextStrenLvl = pstPetReStren->StrenLvl;
	pet_update_to_clt(pstEnv,pstPlayer, pstRolePet);

	task_pet_trace(pstEnv, pstPlayer, pstRolePet, TASK_OBJ_PET_RESET_STRENGTHEN);

	z_oplog_event_begin(pstEnv);
	itemdec_fill(&stDec, RESTREN_ITEMID, 1, iRet);
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_STRENGTHEN_REDO, NULL, &stDec,NULL,NULL);
	return 0;
}

int pet_clt_lock_skill(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETLOCKSKILL *pstLock)
{
	ROLEPET *pstRolePet;
	PETSKILL *pstPetSkill; 
	int i;
	int iIdx = -1;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstLock->PetWid, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	pstPetSkill = &pstRolePet->PetSkills;
	if (pstPetSkill->SkillGridNum <= pstLock->Idx)
	{
		return -1;
	}

	if (pstLock->Lock)
	{
		if (pstRolePet->LockNum >= MAX_LOCK_SKILL_GRID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET179,
						MAX_LOCK_SKILL_GRID);
			return -1;
		}

		if (pstRolePet->LockNum != 0)
		{
			for (i=0; i<pstRolePet->LockNum; i++)
			{
				if (pstRolePet->LockIdxs[i] == pstLock->Idx)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET180);
					return -1;
				}
			}
		}
		
		// 只有第一次锁的时候才记录消耗类型
		if (!(pstRolePet->MiscFlag & (PET_MISC_FLAG_USE_JINBI |PET_MISC_FLAG_USE_JINQUAN)))
		{
			if (pstLock->UseType == 0)
			{
				pstRolePet->MiscFlag |= PET_MISC_FLAG_USE_JINBI;
			}
			else
			{
				pstRolePet->MiscFlag |= PET_MISC_FLAG_USE_JINQUAN;
			}
		}

		pstRolePet->LockIdxs[pstRolePet->LockNum ++] = pstLock->Idx;
		
		pet_update_to_clt(pstEnv,pstPlayer, pstRolePet);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET181);
	}
	else
	{
		if (pstRolePet->LockNum <= 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET183);
			return -1;
		}

		iIdx = -1;
		for (i=0; i<pstRolePet->LockNum; i++)
		{
			if (pstRolePet->LockIdxs[i] == pstLock->Idx)
			{
				iIdx = i;
				break;
			}
		}

		if (iIdx < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET184);
			return -1;
		}
		pstRolePet->LockNum -- ;
		if (pstRolePet->LockNum != iIdx)
		{
			pstRolePet->LockIdxs[iIdx]	= pstRolePet->LockIdxs[pstRolePet->LockNum];
		}

		if (pstRolePet->LockNum == 0)
		{
			pstRolePet->MiscFlag &= ~(PET_MISC_FLAG_USE_JINBI |PET_MISC_FLAG_USE_JINQUAN);
		}

		pet_update_to_clt(pstEnv,pstPlayer, pstRolePet);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET185);
	}

	//todo:log
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_LOCK_SKILL, NULL, NULL, NULL,NULL);
	return 0;
}


int pet_get_rand_skill_one(int iNum, PetRndSkill *pastRanSkills)
{
	int i;
	int iCountRate = 0;
	int iRate;

	for (i=0; i<iNum; i++)
	{
		iCountRate += pastRanSkills[i].iRate;
	}

	iRate = RAND1(iCountRate);
	iCountRate = 0;

	for (i=0; i<iNum; i++)
	{
		iCountRate += pastRanSkills[i].iRate;
		if (iRate < iCountRate)
		{
			return i;
		}
	}
	return -1;
}

int pet_rand_skill_in(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEPET *pstRolePet,
					PETSKILLGRID *pstGrid, int *pNum,PetRndSkill *pastRanSkills)
{
	int iRand;
	int i;
	int iSkillID;
	DIATHESISDEF *pstDiaDef;
	
	//iRand = RAND1(*pNum);
	iRand = pet_get_rand_skill_one(*pNum, pastRanSkills);
	if (iRand < 0)
		iRand = RAND1(*pNum);

	iSkillID = pastRanSkills[iRand].iID;
	pstGrid->SkillID = iSkillID;
	pstGrid->SkillLevel =  pastRanSkills[iRand].iLevel;
	pstDiaDef = z_find_diathesis(pstEnv, pstGrid->SkillID,pstGrid->SkillLevel);
	if (NULL == pstDiaDef)
	{
		return -1;
	}
	pstGrid->GridType = pstDiaDef->PetDiathesisType;

	if (pstPlayer && pstRolePet)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer,SYS_MIDDLE2, 
					ZONEERR_PET186,
					pstRolePet->Name, pstDiaDef->Name);
	}

	// 删除被选数组中的技能
	for (i=*pNum; i>=0; i--)
	{
		if (	pastRanSkills[i].iID == iSkillID ||
			pastRanSkills[i].iGroupID == iSkillID)
		{
			(*pNum) --;
			if (*pNum != i)
			{
				pastRanSkills[i] = pastRanSkills[*pNum];
			}
		}
	}
	return 0;	
}

int pet_get_all_skill(ZONESVRENV* pstEnv,ROLEPET *pstRolePet,int *piRandNum, PetRndSkill *pastRndSkill)
{
	int i,j;
	for (i=0; i<pstEnv->iPetRndSkill; i++)
	{
		for (j=0; j<pstRolePet->PetSkills.SkillGridNum; j++)
		{
			if (pstRolePet->PetSkills.SkillGrid[j].SkillID == pstEnv->astPetRndSkill[i].iID ||
				pstRolePet->PetSkills.SkillGrid[j].SkillID == pstEnv->astPetRndSkill[i].iGroupID)
			{
				break;
			}
		}

		if (*piRandNum >= PET_RND_SKILL_MAX)
		{
			break;
		}
		
		if (j>=pstRolePet->PetSkills.SkillGridNum)
		{
			pastRndSkill[*piRandNum].iLevel = pstEnv->astPetRndSkill[i].iLevel;
			pastRndSkill[*piRandNum].iID = pstEnv->astPetRndSkill[i].iID;
			pastRndSkill[*piRandNum].iGroupID = pstEnv->astPetRndSkill[i].iGroupID;
			pastRndSkill[*piRandNum].iRate = pstEnv->astPetRndSkill[i].iRate;
			(*piRandNum) ++;
		}
	}

	if (*piRandNum < 5) 	// 连5个候选技能都没有 报错。。
	{
		return -1;
	}

	return 0;
}

int pet_clt_reset_skill(ZONESVRENV* pstEnv, Player *pstPlayer,  CSPETSKILLRESET *pstReset)
{
	int i,j;
	int iLock;
	int iRandNum = 0;
	PetRndSkill astRndSkill[PET_RND_SKILL_MAX];
	ROLEPET *pstRolePet;
	ROLEPET stOldPet;
	int iReset;
	int iUseType;
	int iUse;
	USEDEF *pstUseDef1;
	USEDEF *pstUseDef2;
	USEMONEY stUse;
	ITEMDEC stDec;
	char *pszTip;
	int iRet;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	pstPetSvr->PetSvrType = PET_SVR_RESET_SKILL;
	pstPetSvr->PetSvrData.ResetSkillSucc.Wid = pstReset->PetWid;
	
	if (pstPlayer->stOnline.State& CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET166);
		goto _Fail;
	}

	if (pstPlayer->stOnline.State& CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET167);
		goto _Fail;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET168);
		goto _Fail;
	}

	pstRolePet = z_find_role_pet(pstPlayer, pstReset->PetWid, NULL);
	if (!pstRolePet || pstRolePet->PetSkills.SkillGridNum <= 0)
	{
		goto _Fail;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PET149);
		goto _Fail;
	}

	stOldPet = *pstRolePet;

	iReset = (int)pstRolePet->PetSkills.SkillGridNum - (int)pstRolePet->LockNum;
	if (iReset <= 0 )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET150);
		goto _Fail;
	}

	// 服务器内部错误
	if (	pstRolePet->LockNum > 0 &&
		!(pstRolePet->MiscFlag & (PET_MISC_FLAG_USE_JINBI |PET_MISC_FLAG_USE_JINQUAN)))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "pet_clt_reset_skill:check pstRolePet->LockNum error");
		goto _Fail;
	}

	bzero(&stUse, sizeof(stUse));
	bzero(&stDec, sizeof(stDec));
	// 有锁定格子要消耗金币or金券
	if (pstRolePet->LockNum > 0)
	{
		pstUseDef1 = z_find_usedef(pstEnv,USE_ID_PETSKILL_LOCK);
		if (!pstUseDef1)
		{
			goto _Fail;
		}
	
		if (pstRolePet->MiscFlag & PET_MISC_FLAG_USE_JINBI)
		{
			iUseType = ATTR_ID_BINDGODCOIN;	
			stUse.UseMoney_JinBi = iUse = pstUseDef1->UseJinbi*pow(4,pstRolePet->LockNum-1);
			pszTip = MONEY_TYPE_BIND_GODCOIN_NAME;
		}
		else if (pstRolePet->MiscFlag & PET_MISC_FLAG_USE_JINQUAN)
		{
			iUseType = ATTR_ID_GODCOIN;	
			stUse.UseMoney_JinQuan = iUse = pstUseDef1->UseJinquan*pow(4,pstRolePet->LockNum-1);
			pszTip = MONEY_TYPE_GODCOIN_NAME;
		}
		else
		{
			goto _Fail;
		}

		/*
		锁定一个技能格时消耗为X*1
		锁定两个技能格时消耗为X*4
		锁定三个技能格时消耗为X*16
		*/
	}
	else
	{
		pstUseDef2 = z_find_usedef(pstEnv,USE_ID_PETSKILL_RESET);
		if (!pstUseDef2)
		{
			goto _Fail;
		}

		stUse.UseMoney_Money = iUse = pstUseDef2->UseMoney;
		iUseType = ATTR_ID_BULL;	
		pszTip = MONEY_TYPE_MONEY_NAME;
	}

	if (package_dec_money_test(pstEnv, pstPlayer, iUseType, iUse) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET187,pszTip);
		goto _Fail;
	}

	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							PET_SKILL_RESET_ID, 1, 
							LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE))
	{
		ITEMDEF *pstDef = z_find_itemdef( pstEnv, PET_SKILL_RESET_ID);
		if (pstDef)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET188,pstDef->Name);
		}
		goto _Fail;
	}
	

	// 获取所有可随机的技能
	if (pet_get_all_skill(pstEnv,pstRolePet,&iRandNum, astRndSkill) < 0)
	{
		goto _Fail;
	}

	for (i=0; i<pstRolePet->PetSkills.SkillGridNum; i++)
	{
		iLock = 0;
		for (j=0; j<pstRolePet->LockNum; j++)
		{
			if (pstRolePet->LockIdxs[j] == i)
			{
				iLock = 1;
				break;
			}
		}	

		// 被锁了就不搞了
		if (iLock)
		{
			continue;
		}

		pet_rand_skill_in(pstEnv,NULL,NULL,&pstRolePet->PetSkills.SkillGrid[i], &iRandNum,astRndSkill);
	}

	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					PET_SKILL_RESET_ID, 1, 
					LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PET_RESETSKILL);
	itemdec_fill(&stDec, PET_SKILL_RESET_ID, 1, iRet);

	iRet = package_dec_money(pstEnv, pstPlayer, iUseType, iUse);
	if (ATTR_ID_GODCOIN == iUseType)
	{
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_PETSKILL,pstRolePet->PetID,0,0,
					iUse,"宠物重置技能");
	}
	else if (ATTR_ID_BINDGODCOIN == iUseType)
	{
		z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_PETSKILL,pstRolePet->PetID,
					iUse,"宠物重置技能");
	}
	else if (ATTR_ID_BULL == iUseType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETRESETSKILL, pstRolePet->PetID, iUse-iRet, iRet, "宠物重置技能");
	}
	else if (ATTR_ID_MONEY == iUseType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETRESETSKILL, 
				pstRolePet->PetID, 0, iUse, "宠物重置技能");
	}
	
	pet_update_to_clt(pstEnv,pstPlayer, pstRolePet);		// 宠物变化通知客户端
	pet_update_online(pstEnv, pstPlayer, pstRolePet);		// 属性变化通知客户端
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET189);

	//todo:log
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, &stOldPet, OP_PET_RESET_SKILL, NULL, &stDec, &stUse,NULL);
	pstPetSvr->PetSvrData.ResetSkillSucc.Succ = 1;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
		
	_Fail:
		pstPetSvr->PetSvrData.ResetSkillSucc.Succ = 0;
		Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		return -1;
}

int pet_clt_heti_end(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPET *pstRolePet;

	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if (!pstRolePet)
		return -1;

	return pet_heti_in(pstEnv, pstPlayer,pstRolePet, 0);
}

int pet_get_combat_num(Player *pstPlayer)
{
	int i;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	int iNum = 0;

	for (i=0; i<pstPetPackage->Num; i++)
	{
		if (pstPetPackage->RolePets[i].MiscFlag&PET_MISC_FLAG_COMBAT)
		{
			iNum++;
		}	
	}
	return iNum;
}

int pet_step_up(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETSTEPUP*pstStepUp)
{
	ROLEPET *pstRolePet;
	int iUseNum = 0;
	ROLEPET *pstCamp;
	int iSucc = 0;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	tdr_ulonglong ullWid = 0;
	ITEMDEC stItemDec;
	USEDEF *pstUseDef;
	int iRet;
	PETDEF *pstPetDef;

	pstRolePet = z_find_role_pet(pstPlayer, pstStepUp->PetWid, NULL);
	if (!pstRolePet)
		goto final;

	ullWid = pstStepUp->PetWid;
	if (pstRolePet->MiscFlag&PET_MISC_FLAG_COMBAT)
	{
		goto final;
	}

	if (pstRolePet->Level < 60)
	{
		goto final;
	}

	if (pstRolePet->PetGenius < PET_GENIUS_B)
	{
		goto final;
	}

	pstUseDef = z_find_usedef(pstEnv, USE_ID_PET_STEP_UP);
	if (!pstUseDef)
	{
		goto final;
	}

	if ( 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstUseDef->UseMoney) )
	{
		return -1;
	}
	iUseNum =  (pet_get_combat_num(pstPlayer)+1)*10;

	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, PET_STEP_UP_USE_ID, 
				iUseNum, -1, NULL,PACKAGE_DEC_NONE))
	{
		goto final;
	}

	pstRolePet ->MiscFlag |= PET_MISC_FLAG_COMBAT;

	if (pstEnv->pstZoneObj->iPetLoveLevelDef )
	{
		pstPetDef = z_find_pet_def(pstEnv,pstRolePet->PetID, NULL);
		if (pstPetDef)
		{
			pet_love_level_up_in(pstEnv, pstPlayer, pstRolePet,
			pstEnv->pstZoneObj->iPetLoveLevelDef-1-pstRolePet->LoveLevel,pstPetDef,0);
		}
		else
		{
			pstRolePet->LoveLevel = pstEnv->pstZoneObj->iPetLoveLevelDef-1;
		}
	}
	
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	stItemDec.ID = PET_STEP_UP_USE_ID;
	stItemDec.Num = iUseNum;
	stItemDec.BindNum = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, PET_STEP_UP_USE_ID, 
				iUseNum, -1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PET_STEP_UP);
	iRet = package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstUseDef->UseMoney);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PET_STEPUP, 
				pstRolePet->PetID, 0, pstUseDef->UseMoney, "宠物进阶");

	iSucc = 1;

	pstCamp = pet_get_camp(pstEnv, pstPlayer);
	if (	pstCamp && 
		(pstCamp->MiscFlag&PET_MISC_FLAG_COMBAT))
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);	// 重新计算数值
	}

	z_oplog_event_begin(pstEnv);
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_STEP_UP, NULL, &stItemDec, NULL, NULL);

final:

	pstPetSvr->PetSvrType = PET_SVR_STEP_UP;
	pstPetSvr->PetSvrData.PetStepUp.Succ = iSucc;
	pstPetSvr->PetSvrData.PetStepUp.Wid = ullWid;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int pet_imp_up(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETIMPUP*pstImpUp)
{
	PETDEF *pstPetDef;
	ROLEPET *pstRolePet;
	PETIMPUPDEF*pstImpUpDef;
	PETIMPUPATTRDEF *pstImpUpAttrDef;
	PETIMPUPATTRDEF *pstImpUpAttrDef1;
//	PETGENIUS *pstGenius;
	int iGenius;
	int iSucc = 0;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstImpUp->PetWid, NULL);
	if (!pstRolePet)
	{
		goto final;
	}

	if (pstRolePet->StatFlag&PET_STAT_FIGHT)
	{
		goto final;
	}
	
	if (pstRolePet->Imp >= PET_MAX_IMP)
	{
		goto final;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		goto final;
	}

	pstImpUpDef = pet_imp_up_def_find(pstEnv, pstRolePet->Imp);
	if (!pstImpUpDef)
	{
		goto final;
	}

	if ( package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY,pstImpUpDef->UpUseMoney) < 0)
	{
		goto final;
	}

	if (package_dec_test(pstEnv->pstAppCtx, pstEnv,pstPlayer,pstImpUpDef->UpUseItem, pstImpUpDef->UpUseItemNum, 
						LIST_TYPE_SUNDRIES, NULL, 0) < 0)
	{
		goto final;
	}

	pstImpUpAttrDef = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp,pstPetDef->PetAttrType);
	if (!pstImpUpAttrDef)
	{
		goto final;
	}

	pstImpUpAttrDef1 = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp+1,pstPetDef->PetAttrType);
	if (!pstImpUpAttrDef1)
	{
		goto final;
	}

	if (pstRolePet->ImpExp <pstImpUpDef->ExpUp)
	{
		z_sys_strf( pstEnv,  pstPlayer, SYS_MIDDLE, "潜力值不足，无法进化");
		goto final;
	}

	if(pstRolePet->PetGenius >= MAX_PET_GENIUS)
	{
		goto final;
	}

	iGenius = pstRolePet->PetGenius;
		
	pstRolePet->Imp ++;
	pstRolePet->ImpExp-=pstImpUpDef->ExpUp;

	pstRolePet->PhsicAtkGenius =(pstImpUpAttrDef1->Genius[0].PhsicAtkGenius+
								pstRolePet->PhsicAtkGenius -
								pstImpUpAttrDef->Genius[0].PhsicAtkGenius); 
	pstRolePet->IceAtkGenius =(pstImpUpAttrDef1->Genius[0].IceAtkGenius +
								pstRolePet->IceAtkGenius -
								pstImpUpAttrDef->Genius[0].IceAtkGenius);
	pstRolePet->HPGenius =(pstImpUpAttrDef1->Genius[0].HPGenius+
							pstRolePet->HPGenius -
							pstImpUpAttrDef->Genius[0].HPGenius);
	pstRolePet->HitRateGenius =(pstImpUpAttrDef1->Genius[0].HitRateGenius+
								pstRolePet->HitRateGenius-
								pstImpUpAttrDef->Genius[0].HitRateGenius);
	pstRolePet->FleeGenius =(pstImpUpAttrDef1->Genius[0].FleeGenius +
							pstRolePet->FleeGenius-
							pstImpUpAttrDef->Genius[0].FleeGenius);
	pstRolePet->DefGenius =(pstImpUpAttrDef1->Genius[0].DefGenius +
							pstRolePet->DefGenius -
							pstImpUpAttrDef->Genius[0].DefGenius);
	pstRolePet->IceDefGenius =(pstImpUpAttrDef1->Genius[0].IceDefGenius +
							pstRolePet->IceDefGenius  -
							pstImpUpAttrDef->Genius[0].IceDefGenius);

	if (pstEnv->pstZoneObj->iPetLoveLevelDef )
	{
		pet_love_level_up_in(pstEnv, pstPlayer, pstRolePet,
		pstEnv->pstZoneObj->iPetLoveLevelDef-1-pstRolePet->LoveLevel,pstPetDef,0);
	}

	if (pstRolePet->Imp >= PET_MAX_IMP)
	{
		pstRolePet->ImpExp = 0;
	}
	
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY,pstImpUpDef->UpUseMoney) ;

	ITEMDEC stDec;
	stDec.ID = pstImpUpDef->UpUseItem;
	stDec.Num = pstImpUpDef->UpUseItemNum;
	stDec.BindNum = package_dec(pstEnv->pstAppCtx, pstEnv,pstPlayer,pstImpUpDef->UpUseItem, pstImpUpDef->UpUseItemNum, 
						LIST_TYPE_SUNDRIES, NULL, 0, 0);

	USEMONEY stUseMoney;
	stUseMoney.UseMoney_JinBi = 0;
	stUseMoney.UseMoney_JinQuan = 0;
	stUseMoney.UseMoney_Money = pstImpUpDef->UpUseMoney;
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_IMP_UP, NULL, &stDec, &stUseMoney, NULL);
	iSucc = 1;
	
final:

	pstPetSvr->PetSvrType = PET_SVR_IMP_UP;
	pstPetSvr->PetSvrData.PetImpUp.Succ = iSucc;
	pstPetSvr->PetSvrData.PetImpUp.Wid = pstImpUp->PetWid;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int pet_genius_learn(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETGENIUSLEARN *pstLearn)
{
	ROLEPET *pstRolePet;
	DIATHESISDEF *pstDiathesisDef;
	ROLEGRID *pstGrid;
	int iPos;
	ITEMDEF *pstItemDef;
	int iSkill = 0;
	int iaSkill[4]={0,0,0,0};
	int iRand;
	int iSucc = 0;

	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;

	if (pstPlayer->stOnline.State& CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET166);
		return -1;
	}

	if (pstPlayer->stOnline.State& CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET167);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET168);
		return -1;
	}

	pstRolePet = z_find_role_pet(pstPlayer, pstLearn->PetWid, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	if (!(PET_STAT_FIGHT & pstRolePet->StatFlag))
	{
		z_sys_strf( pstEnv,  pstPlayer, SYS_MIDDLE, "需要拥有出战的宠物才能使用天赋书！");
		goto end;
	}

	
	pstGrid =  get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstLearn->GridIdx);
	if (!pstGrid || pstGrid->Type != GRID_ITEM_ITEM)
	{
		goto end;
	}

	iPos = pstGrid - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];
	if (iPos < 0 || iPos >= pstPlayer->stRoleData.Package.SundriesPackage.Num)
	{
		goto end;
	}

	pstItemDef = z_find_itemdef( pstEnv, pstGrid->GridData.RoleItem.DefID);
	if (!pstItemDef || pstItemDef->Result[0].ResultID != RESULT_PET_GENIUS_BOOK)
	{
		goto end;
	}

	if (pstItemDef->Result[0].ResultVal1 > 0)
	{
		iaSkill[iSkill] = pstItemDef->Result[0].ResultVal1;
		iSkill ++;
	}

	if (pstItemDef->Result[0].ResultVal2 > 0)
	{
		iaSkill[iSkill] = pstItemDef->Result[0].ResultVal2;
		iSkill ++;
	}

	if (pstItemDef->Result[0].ResultVal3 > 0)
	{
		iaSkill[iSkill] = pstItemDef->Result[0].ResultVal3;
		iSkill ++;
	}

	if (pstItemDef->Result[0].ResultVal4 > 0)
	{
		iaSkill[iSkill] = pstItemDef->Result[0].ResultVal4;
		iSkill ++;
	}

	if (!iSkill)
	{
		goto end;
	}

	iRand = RAND1(iSkill);

	pstDiathesisDef = z_find_diathesis(pstEnv, iaSkill[iRand] , 1);
	if (NULL == pstDiathesisDef)
	{
		goto end;
	}

	// 专属宠物天赋
	if (pstItemDef->Result[1].ResultVal1 > 0)
	{
		if (	pstRolePet->PetID != pstItemDef->Result[1].ResultVal1 &&
			pstRolePet->PetID != pstItemDef->Result[1].ResultVal2)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET701);	
			goto end;
		}
	}

	package_dec(pstEnv->pstAppCtx,pstEnv, pstPlayer, pstItemDef->ItemID,
					1, LIST_TYPE_SUNDRIES, &iPos,0, 0);
	
	pstRolePet->NativeSkill.SkillLevel = 1;
	pstRolePet->NativeSkill.SkillID = pstDiathesisDef->DiathesisID;
	pstRolePet->NativeSkill.GridType = pstDiathesisDef->PetDiathesisType;

	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	pet_player_view_action( pstEnv, pstPlayer);

	iSucc  =1;
end:

	pstPetSvr->PetSvrType = PET_SVR_GENIUS_LEARN;
	pstPetSvr->PetSvrData.PetGeniusLearn.Succ = iSucc;
	pstPetSvr->PetSvrData.PetGeniusLearn.Wid = pstRolePet->WID;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int pet_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPETCLT*pstPetClt = &pstCsPkg->Body.PetClt;

	switch(pstPetClt->PetCltType)
	{
		case PET_CLT_ACT:

			if (pstCsPkg->Body.PetClt.PetCltData.PetAct.ActTpye == PET_ACT_CAMPAIGN||
				pstCsPkg->Body.PetClt.PetCltData.PetAct.ActTpye == PET_ACT_CAMPAIGN_PRE)
			{
				if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetAct.PetWid) < 0)
				{
					z_pet_campaign_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
					return -1;
				}
			}
		
			return pet_clt_act(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetAct);
			break;
		case PET_CLT_SKILL_UP:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetSkillUp.PetWid) < 0)
			{
				return -1;
			}
			return pet_clt_skill_up(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetSkillUp);
			break;
		case PET_CLT_LOCK_SKILL:
			return pet_clt_lock_skill(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetLockSkill);
			break;

		case PET_CLT_SKILL_RESET:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetSkillReset.PetWid) < 0)
			{
				return -1;
			}
			return pet_clt_reset_skill(pstEnv, pstPlayer,  &pstCsPkg->Body.PetClt.PetCltData.PetSkillReset);
			break;
		case PET_CLT_CHG_NAME:
			return pet_change_name(pstEnv, pstPlayer,  &pstCsPkg->Body.PetClt.PetCltData.ChgName);
			break;
		case PET_CLT_DROP:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetWid) < 0)
			{
				return -1;
			}
			return pet_drop(pstEnv, pstPlayer, pstCsPkg->Body.PetClt.PetCltData.PetWid);
			break;
		case PET_CLT_STRENGTHEN:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetStrengthen.PetWid) < 0)
			{
				return -1;
			}
			return pet_strengthen(pstEnv, pstPlayer,&pstCsPkg->Body.PetClt.PetCltData.PetStrengthen);
			break;
		case PET_CLT_WASH_SKILL_GRID:
			/*if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_PET_WASH_SKILL_GRID) < 0)
			{
				return -1;
			}
			return pet_wash_skill_grid(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.WashSkillGrid);*/
			break;
		case PET_CLT_WATCH:
			return pet_watch(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.Watch);
			break;
		case PET_CLT_SKILL_LEARN: 
			return pet_skill_learn(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.SkillLearn);
			break;
		case PET_CLT_SET_PASSIVE:
			return pet_set_passive(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.ChgMode);
			break;
		case PET_CLT_ADD_SKILL_GRID:
			//return pet_add_skill_grid(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.AddSkillGrid);
			break;
		/*
		case PET_CLT_FUSE_INVITE:
			return pet_fuse_invite(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.FuseInvite);
			break;
		case PET_CLT_FUSE_REJECT:
			return pet_fuse_reject(pstEnv, pstPlayer, pstCsPkg->Body.PetClt.PetCltData.FuseReject.MemID);
			break;
		case PET_CLT_FUSE_ACCEPT:
			return pet_fuse_accept(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.FuseAccept);
			break;
		case PET_CLT_FUSE_CANCEL:
			return pet_fuse_cancel(pstEnv, pstPlayer);
			break;
		case PET_CLT_FUSE_DEAL:
			return pet_fuse_deal(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.FuseDeal);
			break;
		*/
		case PET_CLT_STRENGTHEN_SAVE:
			return pet_strengthen_save(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetStrengthenSave);
			break;
		case PET_CLT_RESTREN:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetReStren.PetWid) < 0)
			{
				return -1;
			}
			return  pet_restrengthen(pstEnv, pstPlayer,&pstCsPkg->Body.PetClt.PetCltData.PetReStren);
			break;
		case PET_CLT_WASH_GENIUS:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetWashGenius.PetWid) < 0)
			{
				return -1;
			}
			return pet_wash_genius(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetWashGenius);
			break;
		case PET_CLT_WASH_GENIUS_SAVE:
			//if (check_npc_ui(pstEnv, pstPlayer, UI_TYPE_PET_OPERATE) < 0)
			//{
			//	return -1;
			//}
			return pet_wash_genius_save(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetWashGeniusSave);
			break;
		case PET_CLT_HETI_END:
			return pet_clt_heti_end(pstEnv, pstPlayer);
			break;
		case PET_CLT_LOVE_LEVEL_UP:
			return pet_love_level_up(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetLoveLevelUp);
			break;
		case PET_CLT_MOVE_STAR:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetMoveStar.DstPetWid) < 0)
			{
				return -1;
			}

			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetMoveStar.SrcPetWid) < 0)
			{
				return -1;
			}
			return pet_move_star(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetMoveStar);
			break;
		case PET_CLT_EAT:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetEat.DstPetWid) < 0)
			{
				return -1;
			}
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetEat.SrcPetWid) < 0)
			{
				return -1;
			}
			return pet_eat(pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetEat);
			break;
		case PET_CLT_STEP_UP:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetStepUp.PetWid) < 0)
			{
				return -1;
			}
			return pet_step_up( pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetStepUp);
			break;
		case PET_CLT_IMP_UP:

			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetImpUp.PetWid) < 0)
			{
				return -1;
			}
			return pet_imp_up( pstEnv, pstPlayer,  &pstCsPkg->Body.PetClt.PetCltData.PetImpUp);
			break;
		case PET_CLT_GENIUS_LEARN:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetImpUp.PetWid) < 0)
			{
				return -1;
			}
			return pet_genius_learn( pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetGeniusLearn);
			break;
		case PET_CLT_UNBIND:
			if (pet_jiyang_limit(pstEnv, pstPlayer,pstCsPkg->Body.PetClt.PetCltData.PetImpUp.PetWid) < 0)
			{
				return -1;
			}
			return pet_unbind( pstEnv, pstPlayer, &pstCsPkg->Body.PetClt.PetCltData.PetUnbind);
			break;
	default:
		return -1;
		break;
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
	UNUSED(pstFrameHead);	//add by paraunused.pl
}

int pet_shi_skill_valid(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, 
							SKILLDEF *pstSkillDef, PetZoneSkill *pstPetZoneSkill, ZoneAni *pstAniAtkee, int iRate)
{
	int iMs;
	struct timeval stDiff;
	TMAPMASK *pstMask;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	RESPOS stPos;
	ZONEIDX stIdx;
	int iRadius = 0;
	int iSkillDist = 0;

	//技能释放概率
	if(iRate && (int)RAND1(10000) > iRate*100)
	{
		return -1;
	}

	//人物在战斗状态下，宠物才可以释放的技能
	if ( !(pstPlayer->stOnline.State & CS_STAT_FIGHT) && pstPetZoneSkill->iSkillType == PET_SKILL_ATK_FIGHT)
	{
		return -1;
	}

	/*
	//人物在出生技能命中后，宠物才可以释放的技能
	if(pstPetFightShiOnline->fNormalSkillHit == 0 && pstPetZoneSkill->iSkillType == PET_SKILL_ATK_HIT)
	{
		return -1;
	}
	*/
	
	//技能冷却
	TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstPetZoneSkill->stCoolEnd);
	TV_TO_MS(iMs, stDiff);
	if (iMs < 0)
	{
		return -1;
	}

	switch(pstAniAtkee->iType)
	{
		case OBJ_PLAYER:
			stPos = pstAniAtkee->stObj.pstPlayer->stRoleData.Pos;
			break;
		case OBJ_PET:
			stPos = pstAniAtkee->stObj.stZonePet.pstOwner->stRoleData.Pos;
			break;
		case OBJ_MONSTER:
			stPos = pstAniAtkee->stObj.pstMon->stCurrPos;
			iRadius =  pstAniAtkee->stObj.pstMon->unRadius;
			break;
		case OBJ_NPC:
			stPos = pstAniAtkee->stObj.pstNpc->stMon.stCurrPos;
			iRadius =  pstAniAtkee->stObj.pstNpc->stMon.unRadius;
			break;
		default:
			return -1;
			break;
	}

	if (pstSkillDef->DeadTarget)
	{
		iSkillDist =pstSkillDef->Distance + 6*DIST_TORRENT;
	}
	else if (pstSkillDef->MoveAttack)
	{
		iSkillDist = pstSkillDef->Distance + 3*DIST_TORRENT;
	}
	else
	{
		iSkillDist = pstSkillDef->Distance + 2*DIST_TORRENT;
	}

	if (pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE)
	{
		if (z_distance(&pstPlayer->stRoleData.Pos, &stPos)  > iSkillDist+iRadius)
		{
			return -1;
		}
	}
	else
	{
		if (0 > z_in_range(&stPos, &pstPlayer->stRoleData.Pos, &stPos, &pstSkillDef->MainRange, pstSkillDef->Distance, iRadius, 0))
		{
			return -1;
		}
	}

	//检查技能阻挡
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst) return -1;
	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y, stPos.X, stPos.Y))
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstRolePet);	//add by paraunused.pl
}

int pet_add_buff_accord_owner(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	PETSKILLGRID *pstNativeSkill;// = &pstRolePet->NativeSkill;
	DIATHESISDEF *pstDiathesisDef;
	int i;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	ROLEPET *pstRolePet = NULL;
	int iRate;
	int iLast;

	for (i=0; i<pstPlayer->stRoleData.Package.PetPackage.Num; i++)
	{
		if (pstPlayer->stRoleData.Package.PetPackage.RolePets[i].StatFlag&PET_STAT_FIGHT)
		{
			pstRolePet = &pstPlayer->stRoleData.Package.PetPackage.RolePets[i];
			break;
		}
	}

	if (!pstRolePet ||pstRolePet->NativeSkill.SkillID <= 0)
	{
		return 0;
	}

	pstNativeSkill = &pstRolePet->NativeSkill;
	
	pstDiathesisDef = z_find_diathesis(pstEnv, pstNativeSkill->SkillID, pstNativeSkill->SkillLevel);
	if (NULL == pstDiathesisDef)
	{
		return -1;
	}

	for (i=0; i<MAX_DIATHESIS_RESULT; i++)
	{
		if (0 == pstDiathesisDef->DiathesisResult[i].SkillResultID)
		{
			break;
		}

		if (pstDiathesisDef->DiathesisResult[i].SkillResultID == RESULT_PET_OWNER_BUFF)
		{
			if (iCurr - pstPlayer->stOnline.iPetFgtTime < pstDiathesisDef->DiathesisResult[i].SkillResultVal1 )
			{
				break;
			}

			pstPlayer->stOnline.iPetFgtTime = iCurr;
			
			iRate = pstDiathesisDef->DiathesisResult[i].SkillResultVal4/100;
			iLast =  pstDiathesisDef->DiathesisResult[i].SkillResultVal4%100;

			if (RAND1(1000) > iRate)
			{
				continue;
			}
			
			
			z_player_apply_status(	pstEnv->pstAppCtx, pstEnv, pstPlayer, 
									pstDiathesisDef->DiathesisResult[i].SkillResultVal2,
									pstDiathesisDef->DiathesisResult[i].SkillResultVal3, 
									iLast, 1, 0);
			break;
		}
	}
	
	return 0;
}

int pet_shi_active_one(ZONESVRENV* pstEnv, Player *pstPlayer, PetFightShiOnline *pstPetFightShiOnline, ZoneAni *pstAniAtkee)
{
	int i;
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;
	SKILLDEF *pstSkillDef;
	PetZoneSkill *pstPetZoneSkill;
	int iRate = 0;

	//pstPetFightShiOnline->fNormalSkillHit = 0;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstPetFightShiOnline->stRolePetOnline.ullWID, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	// 被动模式宠物不攻击
	if(pstRolePet->Passive)
	{
		return 0;
	}

	// 合体的不攻击
	if (is_pet_heti(pstPlayer))
	{
		return 0;
	}

	if (pstPlayer->stRoleData.Map == BEACH_MAP_ID )
		return 0;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if(NULL == pstPetDef)
	{
		return -1;
	}

	for (i=0; i < pstPetFightShiOnline->bSkillNum; i++)
	{
		pstPetZoneSkill = &pstPetFightShiOnline->astPetZoneSkills[i];

		//出生的是技能
		if(pstPetZoneSkill->SkillID == pstPetDef->NormalSkill)
		{
			pstSkillDef = z_find_skill_idx(pstEnv, pstPetZoneSkill->SkillID, pstPetZoneSkill->SkillLevel, &pstPetZoneSkill->iSkillDefIdx);
			if (NULL == pstSkillDef)
			{
				continue;
			}

			iRate = 0;
		}
		//攻击类素质
		else
		{
		 	DIATHESISDEF *pstDiathesisDef;
			SKILLRESULT *pstResult;

			pstDiathesisDef = z_find_diathesis(pstEnv,  pstPetZoneSkill->SkillID, pstPetZoneSkill->SkillLevel);
			if(NULL == pstDiathesisDef)
			{
				continue;
			}

			pstResult = z_diathesis_def_get_result(pstDiathesisDef, RESULT_PET_ADD_SKILL);
			if(NULL == pstResult)
			{
				continue;
			}
			
			pstSkillDef = z_find_skill_idx(pstEnv, pstResult->SkillResultVal1, pstResult->SkillResultVal2, &pstPetZoneSkill->iSkillDefIdx);
			if (NULL == pstSkillDef)
			{
				continue;
			}
			
			iRate = pstResult->SkillResultVal4;
		}

		if (0 > pet_shi_skill_valid(pstEnv, pstPlayer, pstRolePet, pstSkillDef, pstPetZoneSkill, pstAniAtkee, iRate))
		{
			continue;
		}

		if(pet_shi_hit(pstEnv, pstPlayer, pstRolePet, pstSkillDef, pstPetZoneSkill, pstAniAtkee, pstPetFightShiOnline) > 0)
		{
			//每次只放一个技能
			break;

			
			/*
			//出生技能命中
			if(pstPetZoneSkill->SkillID == pstPetDef->NormalSkill)
			{
				pstPetFightShiOnline->fNormalSkillHit = 1;
			}
			//追招命中
			else if(pstPetZoneSkill->iSkillType == PET_SKILL_ATK_HIT)
			{
				pstPetFightShiOnline->fNormalSkillHit = 0;
			}	
			*/
		}
	}
	
	return 0;
}

int pet_shi_active(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i, iIndex;
	TMEMBLOCK *pstMemBlock;
	char cType;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	ZoneAni stAniAtkee;
	Player *pstOwner;
	Monster *pstMon;
	Npc *pstNpc;

	// 宠物天赋 每隔x秒给主人加buff
	pet_add_buff_accord_owner(pstEnv, pstPlayer);
	
	if (pstPetOnline->bFightShiNum <= 0 || 0 == (CS_STAT_FIGHT & pstPlayer->stOnline.State) ||
		/*pstPlayer->stOnline.cMoving ||*/ ((CS_STAT_AUTOFLY) & pstPlayer->stOnline.State))
	{
		return 0;
	}

	if(is_in_over_battle_pworld(pstEnv, pstPlayer))
	{
		return 0;
	}

	if(pstPlayer->stOnline.State & CS_STAT_QIANXIN)
	{
		return 0;
	}

	if(pstPlayer->stOnline.iHitTarget  <= 0)
	{
		goto _error;
	}

	if (pstPlayer->stOnline.iHitTarget >= PET_ID_BASE)
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iHitTarget%PET_ID_BASE, &cType);
	}
	else
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.iHitTarget, &cType);
	}
	
	if (0 > iIndex)
	{
		goto _error;
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}
	
	stAniAtkee.iType = cType;
	switch(cType)
	{
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				goto _error;
			}
			pstOwner = (Player *)pstMemBlock->szData;
			if (pstOwner->stOnline.cMoving)
			{
				z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstOwner, 0);
			}

			if(pstPlayer == pstOwner)
			{
				goto _error;
			}
			
			if (PLAYER_STATUS_ROLE_LOGIN != pstOwner->eStatus || 
				(((pstOwner->stOnline.State & CS_STAT_DEAD) || pstOwner->stRoleData.RoleDetail.HPCurr == 0)) ||
				(pstOwner->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN)) ||
				pstOwner->stRoleData.Map != pstPlayer->stRoleData.Map)
			{
				goto _error;
			}

			if(is_player_ride_on_machine(pstOwner))
			{
				goto _error;
			}
			
			if (pstPlayer->stOnline.iHitTarget >= PET_ID_BASE)
			{
				stAniAtkee.iType = OBJ_PET;
				stAniAtkee.stObj.stZonePet.pstOwner = pstOwner;
				stAniAtkee.stObj.stZonePet.pstPetFightShi = pet_get_shipet_by_xiedaipos(pstOwner, (pstPlayer->stOnline.iHitTarget/PET_ID_BASE)-1);
				if (NULL == stAniAtkee.stObj.stZonePet.pstPetFightShi)
				{
					goto _error;
				}
				stAniAtkee.stObj.stZonePet.pstRolePet = z_find_role_pet(pstOwner, stAniAtkee.stObj.stZonePet.pstPetFightShi->stRolePetOnline.ullWID, NULL);
				if (NULL == stAniAtkee.stObj.stZonePet.pstRolePet)
				{
					goto _error;
				}
			}
			else
			{
				stAniAtkee.stObj.pstPlayer = pstOwner;
			}
			
			break;
		case OBJ_MONSTER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				goto _error;
			}

			pstMon = (Monster *)pstMemBlock->szData;
			
			if (AI_STAT_HOME == pstMon->bAiStat ||
				pstMon->bSubType >= MON_SUB_TYPE_TRAP ||
				((pstMon->cStat == MONSTER_DIE || pstMon->iHP <= 0)) ||
				 pstEnv->pstAppCtx->stCurr.tv_sec < pstMon->tBirthEnd ||
				 pstMon->stMap.iID != pstPlayer->stRoleData.Map)
			{
				goto _error;
			}

			if (pstMon->cMoving)
			{
				z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
			}

			stAniAtkee.stObj.pstMon = pstMon;
			break;
		case OBJ_NPC:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				goto _error;
			}

			pstNpc = (Npc *)pstMemBlock->szData;
			pstMon = &pstNpc->stMon;

			if (	AI_STAT_HOME == pstMon->bAiStat ||
				pstMon->stMap.iID != pstPlayer->stRoleData.Map ||
				pstMon->bAtkType != ATK_TYPE_INITIATIVE ||
				pstMon->cStat == MONSTER_DIE ||
				pstMon->iHP <= 0)
			{
				goto _error;
			}

			if (pstMon->cMoving)
			{
				z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
			}

			stAniAtkee.stObj.pstNpc = pstNpc;
			break;
		default:
			goto _error;
			break;
	}
	
_final:
	
	for (i=0; i<pstPetOnline->bFightShiNum; i++)
	{
		pet_shi_active_one(pstEnv, pstPlayer, &pstPetOnline->astFightShi[i], &stAniAtkee);
	}

	return 0;

_error:

	//主人没有目标就用最近打他的目标
	if(pstPlayer->stOnline.iAtkerTarget)
	{
		pstPlayer->stOnline.iHitTarget = pstPlayer->stOnline.iAtkerTarget; 
	}

	//防止出错。并且宠物无攻击目标时也会释放技能
	stAniAtkee.iType = OBJ_PLAYER;
	stAniAtkee.stObj.pstPlayer = pstPlayer;

	goto _final;
	
	return 0;
}

int get_rand_genius(int iGeniusVal, int iMinMul, int iMaxMul)
{
	int iMinVal, iMaxVal;
	int iVal;

	iMinVal = iGeniusVal*(100+iMinMul)/100.0;
	iMaxVal = iGeniusVal*(100+iMaxMul)/100.0;

	if (iMaxVal > iMinVal)
	{
		iVal = iMinVal + RAND1(iMaxVal-iMinVal+1);
	}
	else
	{
		iVal = iMaxVal + RAND1(iMinVal-iMaxVal+1);
	}

	return iVal;
}

int get_rand_add_genius(int iMinVal, int iMaxVal)
{
	int iVal;
	if (iMaxVal > iMinVal)
	{
		iVal = iMinVal + RAND1(iMaxVal-iMinVal+1);
	}
	else
	{
		iVal = iMinVal;
	}

	return iVal;
}

//创建宠物各项资质成长
int pet_all_genius_create(ZONESVRENV* pstEnv, PETDEF *pstPetDef, ROLEPET *pstRolePet)
{
	PETGENIUS *pstGenius;
	PETWASHGENIUSDEF *pstPetWashGeniusDef;
	PETIMPUPATTRDEF *pstImpDef;

	pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv, pstRolePet->Imp, pstRolePet->PetGenius);
	if(!pstPetWashGeniusDef)
	{
		return -1;
	}
	
	if(pstRolePet->PetGenius >= MAX_PET_GENIUS)
	{
		return -1;
	}

	pstImpDef = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp, pstPetDef->PetAttrType);
	if (!pstImpDef)
		return -1;

	pstGenius = &pstImpDef->Genius[pstRolePet->PetGenius];

	//pstGenius = &pstPetDef->Genius[pstRolePet->PetGenius];

	pstRolePet->PhsicAtkGenius = pstGenius->PhsicAtkGenius + 
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal,  pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	pstRolePet->IceAtkGenius = pstGenius->IceAtkGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal,  pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	pstRolePet->HPGenius = pstGenius->HPGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal,  pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	pstRolePet->DefGenius = pstGenius->DefGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal,  pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	pstRolePet->HitRateGenius = pstGenius->HitRateGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal,  pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	pstRolePet->FleeGenius = pstGenius->FleeGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal,  pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	pstRolePet->IceDefGenius = pstGenius->IceDefGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal,  pstPetWashGeniusDef->WashRange[0].AddMaxnVal);

	return 0;
}


//低级洗 资质、出生 资质
int pet_normal_wash_genius(ZONESVRENV* pstEnv, PETDEF *pstPetDef, ROLEPET *pstRolePet)
{
	int i;
	int iRnd, iRate;

	iRate =0;
	iRnd = (int)RAND1(10000);
	for(i=0; i<MAX_PET_GENIUS; i++)
	{
		if (pstPetDef->NormalWashGenius[i] == 0)
		{
			continue;
		}
		
		iRate += pstPetDef->NormalWashGenius[i];
		if(iRnd < iRate)
		{
			//创建各项成长
			pstRolePet->PetGenius = i;
			return pet_all_genius_create(pstEnv, pstPetDef, pstRolePet);
		}
	}

	return -1;
}

int pet_wash_genius_money_get(ROLEPET *pstRolePet)
{
	switch(pstRolePet->PetGenius)
	{
		case PET_GENIUS_S:
			return PET_GENIUS_S_WASH_MONEY;
			break;
		case PET_GENIUS_A:
			return PET_GENIUS_A_WASH_MONEY;
			break;
		case PET_GENIUS_B:
			return PET_GENIUS_B_WASH_MONEY;
			break;
		case PET_GENIUS_C:
			return PET_GENIUS_C_WASH_MONEY;
			break;
		case PET_GENIUS_D:
			return PET_GENIUS_D_WASH_MONEY;
			break;
	}

	return 0;
}

int package_pet_use_hatch(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult)
{
	int iPetID = pstResult->ResultVal1;

	if(pstResult->ResultVal3 > 0 && RAND1(10000) < pstResult->ResultVal4)
	{
		iPetID = pstResult->ResultVal3;
	}
	
	if ( NULL == pet_hatch(pstEnv, pstPlayer, iPetID, pstResult->ResultVal2) )
	{
		return -1;
	}
	
	return 0;	
}

int pet_hatch_check(ZONESVRENV* pstEnv, Player *pstPlayer,  RESULT *pstResult, ITEMDEF *pstItemDef)
{
	PETDEF *pstPetDef;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;

	pstPetDef = z_find_pet_def(pstEnv, pstResult->ResultVal1, NULL);
	if (!pstPetDef)
	{
		return -1;
	}

	if(pstResult->ResultVal3 > 0)
	{
		pstPetDef = z_find_pet_def(pstEnv, pstResult->ResultVal3, NULL);
		if (!pstPetDef)
		{
			return -1;
		}
	}

	if (0 >= (pstPetPackage->MaxNum-pstPetPackage->Num))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
						ZONEERR_PET151);
		return -1;
	}
	
	return 0;
	UNUSED(pstItemDef);	//add by paraunused.pl
}

int z_pet_wash_genius_notify(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEPET *pstRolePet)
{
	char *pszMsg = NULL;
	int fBroadcast = 0;
	
	switch(pstRolePet->PetGenius)
	{
		case PET_GENIUS_D:
			pszMsg = PET_GENIUS_D_NAME;
			break;
		case PET_GENIUS_C:
			pszMsg = PET_GENIUS_C_NAME;
			break;
		case PET_GENIUS_B:
			pszMsg = PET_GENIUS_B_NAME;
			break;
		case PET_GENIUS_A:
			pszMsg = PET_GENIUS_A_NAME;
			fBroadcast = 1;
			break;
		case PET_GENIUS_S:
			pszMsg = PET_GENIUS_S_NAME;
			fBroadcast = 1;
			break;
		default:
			return 0;
			break;
	}

	if(pszMsg)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
				ZONEERR_PET152, pstRolePet->Name, pszMsg);

		if(fBroadcast)
		{
			if (pstEnv->pstConf->CountryVer != COUNTRY_VER_KOREA)
			{
				z_sys_strf_broadcast(pstEnv, SYS_SHOW, 
					ZONEERR_BROAD332,
					pstPlayer->stRoleData.RoleName, pstRolePet->Name, pszMsg);
			}
		}
	}
	
	return 0;
}

WASHRANGEINFO* pet_genius_washlist_get(int iCurrGenius, int iDefGenius, 
	PETWASHGENIUSDEF *pstPetWashGeniusDef)
{
	int i;
	
	for(i=0; i<pstPetWashGeniusDef->MaxList; i++)
	{
		if(iCurrGenius < iDefGenius*(100+pstPetWashGeniusDef->WashRange[i].GeniusMul)/100.0)
		{
			return &pstPetWashGeniusDef->WashRange[i];
		}
	}

	if(pstPetWashGeniusDef->MaxList > 0 && pstPetWashGeniusDef->MaxList < MAX_WASH_RANGE_LIST)
	{
		return &pstPetWashGeniusDef->WashRange[pstPetWashGeniusDef->MaxList];
	}
	
	return NULL;
}

int get_max_pet_wash_genius(PETWASHGENIUSDEF *pstPetWashGeniusDef, int iDefVal)
{
	return iDefVal*(100+pstPetWashGeniusDef->WashRange[pstPetWashGeniusDef->MaxList-1].GeniusMul)/100.0;
}

int pet_wash_genius_create(ZONESVRENV* pstEnv,Player *pstPlayer, ROLEPET *pstRolePet,
	PETWASHGENIUSDEF *pstPetWashGeniusDef, PETDEF *pstPetDef, CSPETWASHGENIUS *pstCSWashInfo)
{
	PetWashGeniusInfo *pstPetWashGeniusInfo = &pstPlayer->stOnline.stPetWashGeniusInfo;
	PETGENIUS *pstGenius;
	WASHRANGEINFO *pstWashList;
	int iAdd = 0;
	
	if(pstRolePet->PetGenius >= MAX_PET_GENIUS)
	{
		return -1;
	}

	//pstGenius = &pstPetDef->Genius[pstPetWashGeniusInfo->iGenius];
	PETIMPUPATTRDEF *pstDef = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp, pstPetDef->PetAttrType);
	if (!pstDef)
		return -1;

	pstGenius = &pstDef->Genius[pstPetWashGeniusInfo->iGenius];

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_ATK_GENIUS)
	{
		pstPetWashGeniusInfo->iPhsicAtkGenius = pstRolePet->PhsicAtkGenius;
		pstPetWashGeniusInfo->iIceAtkGenius = pstRolePet->IceAtkGenius;
	}
	else
	{
		pstWashList = pet_genius_washlist_get(pstRolePet->PhsicAtkGenius, pstGenius->PhsicAtkGenius, pstPetWashGeniusDef);
		if(pstWashList)
		{
			iAdd = get_rand_add_genius(pstWashList->AddMinVal, pstWashList->AddMaxnVal);
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_CHINA &&
				iAdd < 1 &&pstRolePet->PetGenius < PET_GENIUS_A)
			{	
				//iAdd = 1;
			}
			pstPetWashGeniusInfo->iPhsicAtkGenius = pstRolePet->PhsicAtkGenius + iAdd;
			if(pstPetWashGeniusInfo->iPhsicAtkGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].PhsicAtkGenius))
			{
				pstPetWashGeniusInfo->iPhsicAtkGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].PhsicAtkGenius);
			}
		}

		pstWashList = pet_genius_washlist_get(pstRolePet->IceAtkGenius, pstGenius->IceAtkGenius, pstPetWashGeniusDef);
		if(pstWashList)
		{
			iAdd = get_rand_add_genius(pstWashList->AddMinVal, pstWashList->AddMaxnVal);
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_CHINA &&
				iAdd < 1&&pstRolePet->PetGenius < PET_GENIUS_A)
			{	
				//iAdd = 1;
			}
			pstPetWashGeniusInfo->iIceAtkGenius = pstRolePet->IceAtkGenius + iAdd;
			if(pstPetWashGeniusInfo->iIceAtkGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].IceAtkGenius))
			{
				pstPetWashGeniusInfo->iIceAtkGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].IceAtkGenius);
			}
		}
	}

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_PHSIC_DEF_GENIUS)
	{
		pstPetWashGeniusInfo->iDefGenius = pstRolePet->DefGenius;
	}
	else
	{
		pstWashList = pet_genius_washlist_get(pstRolePet->DefGenius, pstGenius->DefGenius, pstPetWashGeniusDef);
		if(pstWashList)
		{
			iAdd = get_rand_add_genius(pstWashList->AddMinVal, pstWashList->AddMaxnVal);
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_CHINA &&
				iAdd < 1&&pstRolePet->PetGenius < PET_GENIUS_A)
			{	
				//iAdd = 1;
			}
			
			pstPetWashGeniusInfo->iDefGenius = pstRolePet->DefGenius + iAdd;
			if(pstPetWashGeniusInfo->iDefGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].DefGenius))
			{
				pstPetWashGeniusInfo->iDefGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].DefGenius);
			}
		}
	}

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_ICE_DEF_GENIUS)
	{
		pstPetWashGeniusInfo->iIceDefGenius = pstRolePet->IceDefGenius;
	}
	else
	{
		pstWashList = pet_genius_washlist_get(pstRolePet->IceDefGenius, pstGenius->IceDefGenius, pstPetWashGeniusDef);
		if(pstWashList)
		{
			iAdd = get_rand_add_genius(pstWashList->AddMinVal, pstWashList->AddMaxnVal);
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_CHINA &&
				iAdd < 1&&pstRolePet->PetGenius < PET_GENIUS_A)
			{	
				//iAdd = 1;
			}
			pstPetWashGeniusInfo->iIceDefGenius = pstRolePet->IceDefGenius + iAdd;
			if(pstPetWashGeniusInfo->iIceDefGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].IceDefGenius))
			{
				pstPetWashGeniusInfo->iIceDefGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].IceDefGenius);
			}
		}
	}
	
	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_HP_GENIUS)
	{
		pstPetWashGeniusInfo->iHPGenius = pstRolePet->HPGenius;
	}
	else
	{
		pstWashList = pet_genius_washlist_get(pstRolePet->HPGenius, pstGenius->HPGenius, pstPetWashGeniusDef);
		if(pstWashList)
		{
			iAdd = get_rand_add_genius(pstWashList->AddMinVal, pstWashList->AddMaxnVal);
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_CHINA &&
				iAdd < 1&&pstRolePet->PetGenius < PET_GENIUS_A)
			{	
				//iAdd = 1;
			}
			
			pstPetWashGeniusInfo->iHPGenius = pstRolePet->HPGenius + iAdd;
			if(pstPetWashGeniusInfo->iHPGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].HPGenius))
			{
				pstPetWashGeniusInfo->iHPGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].HPGenius);
			}
		}
	}

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_HITRATE_GENIUS)
	{
		pstPetWashGeniusInfo->iHitRateGenius= pstRolePet->HitRateGenius;
	}
	else
	{
		pstWashList = pet_genius_washlist_get(pstRolePet->HitRateGenius, pstGenius->HitRateGenius, pstPetWashGeniusDef);
		if(pstWashList)
		{
			iAdd = get_rand_add_genius(pstWashList->AddMinVal, pstWashList->AddMaxnVal);
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_CHINA &&
				iAdd < 1&&pstRolePet->PetGenius < PET_GENIUS_A)
			{	
				//iAdd = 1;
			}
			pstPetWashGeniusInfo->iHitRateGenius = pstRolePet->HitRateGenius + iAdd;
			if(pstPetWashGeniusInfo->iHitRateGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].HitRateGenius))
			{
				pstPetWashGeniusInfo->iHitRateGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].HitRateGenius);
			}
		}
	}
	
	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_FLEE_GENIUS)
	{
		pstPetWashGeniusInfo->iFleeGenius =  pstRolePet->FleeGenius;
	}
	else
	{
		pstWashList = pet_genius_washlist_get(pstRolePet->FleeGenius, pstGenius->FleeGenius, pstPetWashGeniusDef);
		if(pstWashList)
		{
			iAdd = get_rand_add_genius(pstWashList->AddMinVal, pstWashList->AddMaxnVal);
			if (pstEnv->pstConf->CountryVer == COUNTRY_VER_CHINA &&
				iAdd < 1&&pstRolePet->PetGenius < PET_GENIUS_A)
			{	
				//iAdd = 1;
			}
			
			pstPetWashGeniusInfo->iFleeGenius = pstRolePet->FleeGenius + iAdd;
			if(pstPetWashGeniusInfo->iFleeGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].FleeGenius))
			{
				pstPetWashGeniusInfo->iFleeGenius = 
					get_max_pet_wash_genius(pstPetWashGeniusDef, pstDef->Genius[PET_GENIUS_S].FleeGenius);
			}
		}
	}
	
	return 0;
}

//高级幻化
int pet_genius_up_high(ZONESVRENV * pstEnv, Player *pstPlayer, CSPACKAGEUSE *pstUse)
{
	ROLEPET *pstRolePet;
	PETIMPUPATTRDEF *pstImpDef;
	PETDEF *pstPetDef;
	//PETWASHGENIUSDEF *pstPetWashGeniusDef;
	
	pstRolePet = pet_find_fight(pstPlayer);
	if (!pstRolePet)
	{
		return -1;
	}

	pstPetDef = z_find_pet_def( pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		return -1;
	}

	pstImpDef = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp,pstPetDef->PetAttrType);
	if (!pstImpDef)
	{
		return -1;
	}

	pstRolePet->PetGenius++;
	if (pstRolePet->PetGenius > PET_GENIUS_S)
	{
		return -1;
	}	

	pstRolePet->PhsicAtkGenius =pstImpDef->Genius[pstRolePet->PetGenius].PhsicAtkGenius;
	pstRolePet->IceAtkGenius = pstImpDef->Genius[pstRolePet->PetGenius].IceAtkGenius;
	pstRolePet->DefGenius =pstImpDef->Genius[pstRolePet->PetGenius].DefGenius;
	pstRolePet->IceDefGenius = pstImpDef->Genius[pstRolePet->PetGenius].IceDefGenius;
	pstRolePet->HitRateGenius = pstImpDef->Genius[pstRolePet->PetGenius].HitRateGenius;
	pstRolePet->FleeGenius = pstImpDef->Genius[pstRolePet->PetGenius].FleeGenius;
	pstRolePet->HPGenius = pstImpDef->Genius[pstRolePet->PetGenius].HPGenius;
	
	/*pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv, pstRolePet->Imp, pstRolePet->PetGenius);
	if(!pstPetWashGeniusDef)
	{
		return -1;
	}

	pstRolePet->PhsicAtkGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, 
							pstPetDef->Genius[pstRolePet->PetGenius].PhsicAtkGenius);
	pstRolePet->IceAtkGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, 
							pstPetDef->Genius[pstRolePet->PetGenius].HPGenius);
	pstRolePet->DefGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, 
							pstPetDef->Genius[pstRolePet->PetGenius].DefGenius);
	pstRolePet->IceDefGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, 
							pstPetDef->Genius[pstRolePet->PetGenius].IceDefGenius);
	pstRolePet->HitRateGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, 
							pstPetDef->Genius[pstRolePet->PetGenius].HitRateGenius);
	pstRolePet->FleeGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, 
							pstPetDef->Genius[pstRolePet->PetGenius].FleeGenius);

	pstRolePet->HPGenius = get_max_pet_wash_genius(pstPetWashGeniusDef, 
							pstPetDef->Genius[pstRolePet->PetGenius].HPGenius);*/


	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	
	pstPetSvr->PetSvrType = PET_SVR_WASH_HIGH;
	pstPetSvr->PetSvrData.PetWashHighSucc.Succ = 1;
	pstPetSvr->PetSvrData.PetWashHighSucc.Wid = pstRolePet->WID;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));		
		
		if (pstRolePet && pstRolePet->PetGenius == PET_GENIUS_S)
		{
			stCond.Type = COND_PET_GET;
			stCond.Val1 = 2;
	
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
	}
	return 0;
}

int pet_genius_up_high_check(ZONESVRENV * pstEnv, Player *pstPlayer,ITEMDEF *pstItemDef,CSPACKAGEUSE *pstUse)
{
	ROLEPET *pstRolePet;
	char szMsg[32];

	pstRolePet = pet_find_fight(pstPlayer);
	if (!pstRolePet)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET195);
		return -1;
	}

	if (pstRolePet->PetGenius != pstItemDef->Result[0].ResultVal1)
	{
		if (pstItemDef->Result[0].ResultVal1 == PET_GENIUS_D)
		{
			strncpy(szMsg, "普通资质",sizeof(szMsg));
		}
		else if (pstItemDef->Result[0].ResultVal1 == PET_GENIUS_C)
		{
			strncpy(szMsg, "良好资质",sizeof(szMsg));
		}
		else if (pstItemDef->Result[0].ResultVal1 == PET_GENIUS_B)
		{
			strncpy(szMsg, "优秀资质",sizeof(szMsg));
		}
		else if (pstItemDef->Result[0].ResultVal1 == PET_GENIUS_A)
		{
			strncpy(szMsg, "杰出资质",sizeof(szMsg));
		}
		else
		{
			return -1;
		}

		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, 
							ZONEERR_PET196,szMsg, pstItemDef->Name);
		return -1;
	}
	return 0;
}

int pet_genius_levelup_check(ZONESVRENV * pstEnv,ROLEPET *pstRolePet, PETDEF *pstPetDef, PETWASHGENIUSDEF *pstPetWashGeniusDef)
{
	PETGENIUS *pstGenius;
	PETIMPUPATTRDEF *pstImpDef;
	
	if(pstRolePet->PetGenius >= PET_GENIUS_S)
	{
		return -1;
	}

	pstImpDef = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp, pstPetDef->PetAttrType);
	if (!pstImpDef)
		return -1;
	//pstGenius = &pstPetDef->Genius[pstRolePet->PetGenius];
	pstGenius = &pstImpDef->Genius[pstRolePet->PetGenius];

	if(pstPetWashGeniusDef->MaxList > MAX_WASH_RANGE_LIST ||
		pstPetWashGeniusDef->MaxList <= 0)
	{
		return -1;
	}

	if(pstPetWashGeniusDef->MaxList == 1)
	{
		return 0;
	}
	else
	{
		int iMul;
		
		iMul = pstPetWashGeniusDef->WashRange[pstPetWashGeniusDef->MaxList-2].GeniusMul;
		
		if(pstRolePet->PhsicAtkGenius < pstGenius->PhsicAtkGenius*(100+iMul)/100.0)
		{
			return -1;
		}

		if(pstRolePet->DefGenius < pstGenius->DefGenius*(100+iMul)/100.0)
		{
			return -1;
		}

		if(pstRolePet->IceDefGenius < pstGenius->IceDefGenius*(100+iMul)/100.0)
		{
			return -1;
		}

		if(pstRolePet->HPGenius < pstGenius->HPGenius*(100+iMul)/100.0)
		{
			return -1;
		}

		if(pstRolePet->HitRateGenius < pstGenius->HitRateGenius*(100+iMul)/100.0)
		{
			return -1;
		}

		if(pstRolePet->FleeGenius < pstGenius->FleeGenius*(100+iMul)/100.0)
		{
			return -1;
		}
	}
	
	return 0;
}

int pet_wash_genius_up(ZONESVRENV* pstEnv, Player *pstPlayer, PETDEF *pstPetDef, ROLEPET *pstRolePet)
{
	PETGENIUS *pstGenius;
	PETWASHGENIUSDEF *pstPetWashGeniusDef;
	PetWashGeniusInfo *pstPetWashGeniusInfo = &pstPlayer->stOnline.stPetWashGeniusInfo;

	pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv,  pstRolePet->Imp, pstPetWashGeniusInfo->iGenius);
	if(!pstPetWashGeniusDef)
	{
		return -1;
	}
	
	if(pstPetWashGeniusInfo->iGenius >= MAX_PET_GENIUS)
	{
		return -1;
	}

	PETIMPUPATTRDEF *pstDef = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp, pstPetDef->PetAttrType);
	if (!pstDef)
		return -1;

	pstGenius = &pstDef->Genius[pstPetWashGeniusInfo->iGenius];

	//pstGenius = &pstPetDef->Genius[pstPetWashGeniusInfo->iGenius];

	pstPetWashGeniusInfo->iPhsicAtkGenius = pstGenius->PhsicAtkGenius + 
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal, pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	if(pstPetWashGeniusInfo->iPhsicAtkGenius < pstRolePet->PhsicAtkGenius)
	{
		pstPetWashGeniusInfo->iPhsicAtkGenius = pstRolePet->PhsicAtkGenius;
	}
	
	pstPetWashGeniusInfo->iIceAtkGenius = pstGenius->IceAtkGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal, pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	if(pstPetWashGeniusInfo->iIceAtkGenius < pstRolePet->IceAtkGenius)
	{
		pstPetWashGeniusInfo->iIceAtkGenius = pstRolePet->IceAtkGenius;
	}
	
	pstPetWashGeniusInfo->iHPGenius = pstGenius->HPGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal, pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	if(pstPetWashGeniusInfo->iHPGenius < pstRolePet->HPGenius)
	{
		pstPetWashGeniusInfo->iHPGenius = pstRolePet->HPGenius;
	}
	
	pstPetWashGeniusInfo->iDefGenius = pstGenius->DefGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal, pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	if(pstPetWashGeniusInfo->iDefGenius < pstRolePet->DefGenius)
	{
		pstPetWashGeniusInfo->iDefGenius = pstRolePet->DefGenius;
	}
	
	pstPetWashGeniusInfo->iHitRateGenius = pstGenius->HitRateGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal, pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	if(pstPetWashGeniusInfo->iHitRateGenius < pstRolePet->HitRateGenius)
	{
		pstPetWashGeniusInfo->iHitRateGenius = pstRolePet->HitRateGenius;
	}
	
	pstPetWashGeniusInfo->iFleeGenius = pstGenius->FleeGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal, pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	if(pstPetWashGeniusInfo->iFleeGenius < pstRolePet->FleeGenius)
	{
		pstPetWashGeniusInfo->iFleeGenius = pstRolePet->FleeGenius;
	}
	
	pstPetWashGeniusInfo->iIceDefGenius = pstGenius->IceDefGenius +
		get_rand_add_genius(pstPetWashGeniusDef->WashRange[0].AddMinVal, pstPetWashGeniusDef->WashRange[0].AddMaxnVal);
	if(pstPetWashGeniusInfo->iIceDefGenius < pstRolePet->IceDefGenius)
	{
		pstPetWashGeniusInfo->iIceDefGenius = pstRolePet->IceDefGenius;
	}

	return 0;
}

int pet_wash_genius_process(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, 
	CSPETWASHGENIUS *pstCSWashInfo)
{
	PETDEF *pstPetDef;
	PETWASHGENIUSDEF *pstPetWashGeniusDef;
	PetWashGeniusInfo *pstPetWashGeniusInfo = &pstPlayer->stOnline.stPetWashGeniusInfo;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		return -1;
	}

	pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv,  pstRolePet->Imp, pstRolePet->PetGenius);
	if(!pstPetWashGeniusDef)
	{
		return -1;
	}

	pstPetWashGeniusInfo->ullPetWid = pstRolePet->WID;
	pstPetWashGeniusInfo->iGenius = pstRolePet->PetGenius;
	
	if(0 == pet_genius_levelup_check( pstEnv,pstRolePet, pstPetDef, pstPetWashGeniusDef))
	{
		//提升资质
		if(pstRolePet->PetGenius < PET_GENIUS_S)
		{
			pstPetWashGeniusInfo->iGenius = pstRolePet->PetGenius+1;
			return pet_wash_genius_up(pstEnv, pstPlayer, pstPetDef, pstRolePet);
		}
	}
	
	pet_wash_genius_create( pstEnv,pstPlayer, pstRolePet, pstPetWashGeniusDef, pstPetDef, pstCSWashInfo);
	return 0;
}

int pet_wash_genius_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet,
	CSPETWASHGENIUS *pstCSWashInfo)
{
	ITEMDEF *pstItemDef;

	if (pstPlayer->stOnline.State& CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET167);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET168);
		return -1;	
	}

	pstItemDef = z_find_itemdef(pstEnv, pstCSWashInfo->ItemID);
	if(!pstItemDef)
	{
		return -1;
	}
	
	if(NULL == z_item_get_result(pstItemDef, RESULT_PET_WASH_GENIUS))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PET113);
		return -1;
	}

	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstCSWashInfo->ItemID, 1, 
							-1, NULL,PACKAGE_DEC_NONE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET153, pstItemDef->Name);
		return -1;
	}

	if (pstPlayer->stOnline.State & CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_PET154);
		return -1;
	}

	if(pstRolePet->StatFlag & PET_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
						ZONEERR_PET155);
		return -1;
	}

	if(pstRolePet->Hierarchy>1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET156);
		return -1;
	}

	return 0;
}

USEDEF *pet_wash_genius_use_def_get(ZONESVRENV* pstEnv, ROLEPET *pstRolePet)
{
	int iID;
	USEDEF *pstUseDef;
	
	switch(pstRolePet->PetGenius)
	{
		case PET_GENIUS_S:
			iID = USE_ID_PET_WASH_GENIUS_S;
			break;
		case PET_GENIUS_A:
			iID = USE_ID_PET_WASH_GENIUS_A;
			break;
		case PET_GENIUS_B:
			iID = USE_ID_PET_WASH_GENIUS_B;
			break;
		case PET_GENIUS_C:
			iID = USE_ID_PET_WASH_GENIUS_C;
			break;
		case PET_GENIUS_D:
			iID = USE_ID_PET_WASH_GENIUS_D;
			break;
		default:
			return NULL;
			break;
	}

	pstUseDef = z_find_usedef(pstEnv, iID);
	if(pstUseDef)
	{
		return pstUseDef;
	}
	
	return NULL;
}

int pet_genius_lock_money_get(CSPETWASHGENIUS *pstCSWashInfo, int iMoney)
{
	int iCount = 0;
	int iTmpMoney = iMoney;

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_ATK_GENIUS)
	{
		iCount++;
	}

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_PHSIC_DEF_GENIUS)
	{
		iCount++;
	}

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_ICE_DEF_GENIUS)
	{
		iCount++;
	}

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_HP_GENIUS)
	{
		iCount++;
	}

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_HITRATE_GENIUS)
	{
		iCount++;
	}

	if(pstCSWashInfo->LockFlag & PET_WASH_LOCK_FLEE_GENIUS)
	{
		iCount++;
	}

	if(iCount == 7)
	{
		return -1;
	}

	iTmpMoney = iTmpMoney*iCount*2;
	
	return iTmpMoney;
}

int pet_wash_genius(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETWASHGENIUS *pstCSWashInfo)
{
	int iRet, iMoney, iMoneyType;
	ITEMDEC stDec;
	USEMONEY stUse;
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;
	USEDEF *pstUseDef;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETWASHGENIUSINFO *pstWashGeniusInfo = &pstPetSvr->PetSvrData.WashGeniusInfo;
	PetWashGeniusInfo *pstRoleWashGeniusInfo = &pstPlayer->stOnline.stPetWashGeniusInfo;

	memset(&stDec, 0, sizeof(stDec));
	memset(&stUse, 0, sizeof(stUse));
	memset(pstWashGeniusInfo, 0, sizeof(*pstWashGeniusInfo));
	
	pstRolePet = z_find_role_pet(pstPlayer, pstCSWashInfo->PetWid,NULL);
	if (!pstRolePet)
	{
		goto _error;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		goto _error;
	}

	if(0 > pet_wash_genius_check(pstEnv, pstPlayer, pstRolePet, pstCSWashInfo))
	{
		goto _error;
	}

	pstUseDef = pet_wash_genius_use_def_get(pstEnv, pstRolePet);
	if(!pstUseDef)
	{
		goto _error;
	}

	if(pstCSWashInfo->MoneyType == PET_WASH_NORMAL)
	{
		if(pstCSWashInfo->LockFlag != 0)
		{
			goto _error;
		}
		iMoney = pstUseDef->UseMoney;
		iMoneyType = ATTR_ID_BULL;
		stUse.UseMoney_Money = iMoney;
	}
	else if(pstCSWashInfo->MoneyType == PET_WASH_BIND_GODCOIN)
	{
		if(pstCSWashInfo->LockFlag == 0)
		{
			goto _error;
		}
		iMoney = pet_genius_lock_money_get(pstCSWashInfo, pstUseDef->UseJinbi);
		iMoneyType = ATTR_ID_BINDGODCOIN;
		stUse.UseMoney_JinBi = iMoney;
	}
	else if(pstCSWashInfo->MoneyType == PET_WASH_GODCOIN)
	{
		if(pstCSWashInfo->LockFlag == 0)
		{
			goto _error;
		}
		iMoney = pet_genius_lock_money_get(pstCSWashInfo, pstUseDef->UseJinquan);
		iMoneyType = ATTR_ID_GODCOIN;
		stUse.UseMoney_JinQuan = iMoney;
	}
	else
	{
		goto _error;
	}

	if(iMoney < 0)
	{
		goto _error;
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, iMoneyType, iMoney))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
		goto _error;
	}

	if(0 > pet_wash_genius_process(pstEnv, pstPlayer, pstRolePet, pstCSWashInfo))
	{
		goto _error;
	}

	iRet = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
					pstCSWashInfo->ItemID, 1, 
					-1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PET_RESWASH);
	if(iRet < 0)
	{
		goto _error;
	}
	
	itemdec_fill(&stDec, pstCSWashInfo->ItemID, 1, iRet);

	iRet = package_dec_money(pstEnv, pstPlayer, iMoneyType, iMoney);
	if (ATTR_ID_GODCOIN == iMoneyType)
	{
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_PETWASH,pstRolePet->PetID,0,0,
					iMoney,"宠物幻化");
	}
	else if (ATTR_ID_BINDGODCOIN == iMoneyType)
	{
		z_bindgold_consume_oplog(pstEnv,pstPlayer,BINDGOLD_OPTYPE_PETWASH,pstRolePet->PetID,
					iMoney,"宠物幻化");
	}
	else if (ATTR_ID_BULL == iMoneyType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETGENIUS, pstRolePet->PetID, iMoney-iRet, iRet, "宠物幻化");
	}
	else if (ATTR_ID_MONEY == iMoneyType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PETGENIUS, 
			pstRolePet->PetID, 0, iMoney, "宠物幻化");
	}
	
	pstWashGeniusInfo->Succ = 1;
	pstWashGeniusInfo->PetWid = pstRoleWashGeniusInfo->ullPetWid;
	pstWashGeniusInfo->Genius = pstRoleWashGeniusInfo->iGenius;
	pstWashGeniusInfo->AtkGenius = pstRoleWashGeniusInfo->iPhsicAtkGenius;
	pstWashGeniusInfo->PhsicDefGenius = pstRoleWashGeniusInfo->iDefGenius;
	pstWashGeniusInfo->IceDefGenius = pstRoleWashGeniusInfo->iIceDefGenius;
	pstWashGeniusInfo->HPGenius = pstRoleWashGeniusInfo->iHPGenius;
	pstWashGeniusInfo->HitRateGenius = pstRoleWashGeniusInfo->iHitRateGenius;
	pstWashGeniusInfo->FleeGenius = pstRoleWashGeniusInfo->iFleeGenius;

	task_pet_trace(pstEnv, pstPlayer, pstRolePet, TASK_OBJ_PET_WASH);
	
	z_oplog_event_begin(pstEnv);
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_WASH, NULL, &stDec, &stUse,NULL);

_final:

	pstPetSvr->PetSvrType = PET_SVR_WASH_GENIUS;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
_error:

	pstWashGeniusInfo->Succ = 0;
	goto _final;
	return 0;
}

int pet_love_level_up_in(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet,
	int iLevel,PETDEF *pstPetDef,int iNotice )
{
	
	pstRolePet->LoveLevel += iLevel;
	//宠物更新
	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	if (iNotice)
	{
		pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

		range_pet_set(pstEnv, pstPlayer, pstRolePet, pstPetDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstRolePet, pstPetDef));
	
		z_oplog_event_begin(pstEnv);
		z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_LOVE_LEVEL_UP, NULL, NULL, NULL,NULL);
	}
	return 0;
}

int pet_love_level_up(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETLOVELEVELUP *pstCSSave)
{
	
	PETDEF *pstPetDef = NULL;
	ROLEPET *pstRolePet;
	PETLOVELEVELDEF* pstLoveLevelDef = NULL;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	
	int iMaxLevel =  pstObj->iPetLoveLevelDef - 1;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstCSSave->PetWid,NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	if (pstRolePet->LoveLevel >= iMaxLevel)
	{
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		return -1;
	}

	pstLoveLevelDef = z_find_pet_love_level_def(pstEnv,iMaxLevel);
	if (!pstLoveLevelDef)
	{		
		tlog_error(pstEnv->pstLogCat, 0, 0, "z_find_pet_love_level_def err iMaxLevel=%d",iMaxLevel);
		return -1;
	}

	pet_love_level_up_in(pstEnv,pstPlayer,pstRolePet,iMaxLevel - pstRolePet->LoveLevel,
						pstPetDef,1);
		
	return 0;
}

int pet_wash_genius_save(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETWASHGENIUSSAVE *pstCSSave)
{
	PETDEF *pstPetDef;
	ROLEPET *pstRolePet;
	PetWashGeniusInfo *pstRoleWashGeniusInfo = &pstPlayer->stOnline.stPetWashGeniusInfo;
	int iOldGenius;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstCSSave->PetWid,NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		return -1;
	}

	if(pstRoleWashGeniusInfo->ullPetWid != pstCSSave->PetWid)
	{
		return -1;
	}
	
	iOldGenius = pstRolePet->PetGenius;

	pstRolePet->PetGenius = pstRoleWashGeniusInfo->iGenius;
	pstRolePet->PhsicAtkGenius = pstRoleWashGeniusInfo->iPhsicAtkGenius;
	pstRolePet->IceAtkGenius = pstRoleWashGeniusInfo->iIceAtkGenius;
	pstRolePet->DefGenius = pstRoleWashGeniusInfo->iDefGenius;
	pstRolePet->IceDefGenius = pstRoleWashGeniusInfo->iIceDefGenius;
	pstRolePet->HPGenius = pstRoleWashGeniusInfo->iHPGenius;
	pstRolePet->HitRateGenius = pstRoleWashGeniusInfo->iHitRateGenius;
	pstRolePet->FleeGenius = pstRoleWashGeniusInfo->iFleeGenius;

	memset(pstRoleWashGeniusInfo, 0, sizeof(*pstRoleWashGeniusInfo));

	 //宠物更新
	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	pstRolePet->HP = pstRolePet->MaxHP;
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));		
		
		if (pstRolePet && pstRolePet->PetGenius == PET_GENIUS_S)
		{
			stCond.Type = COND_PET_GET;
			stCond.Val1 = 2;
	
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
	}

	if(iOldGenius < pstRolePet->PetGenius)
	{
		z_pet_wash_genius_notify(pstEnv, pstPlayer, pstRolePet);
	}
	
	range_pet_set(pstEnv, pstPlayer, pstRolePet, pstPetDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstRolePet, pstPetDef));

	z_oplog_event_begin(pstEnv);
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_WASH_SAVE, NULL, NULL, NULL,NULL);
	return 0;
}

/*int get_pet_trait(ZONESVRENV* pstEnv, PETDEF *pstPetDef, int iTrait)
{
	NormalTrait *pstNormalTrait = pstEnv->pstPetNormalTrait;

	//宠物蛋上指定的性格
	if(iTrait)
	{
		return  iTrait;
	}
	//出生表格定义的性格
	else if(pstPetDef->BornTrait)
	{
		return pstPetDef->BornTrait;
	}
	else
	{
		//性格
		if (pstNormalTrait->iTraitNum)
		{
			return pstNormalTrait->TraitID[(int)RAND1(pstNormalTrait->iTraitNum)];
		}
	}

	return 0;
}*/	


int pet_role_login_check(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;	
	int i;
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;

	for (i=0; i<pstPetPackage->Num; i++)
	{
		pstRolePet = &pstPetPackage->RolePets[i];
		if (pstRolePet->MiscFlag & PET_MISC_FLAG_CHECK_JIYANG)
		{
			continue;
		}

		pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
		if (!pstPetDef)
		{
			continue;
		}

		pstRolePet->WenBao = PET_MAX_WENBAO;
		pstRolePet->Imp = pstPetDef->Important;
		pstRolePet->MiscFlag |= PET_MISC_FLAG_CHECK_JIYANG;
	}
	return 0;
}

ROLEPET* pet_hatch(ZONESVRENV* pstEnv, Player *pstPlayer,  int iPetID, int iTrait)
{
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;
	PETLEVELDEF *pstPetLevelDef;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;	
	CSPKG stPkg;
	CSPETSVR *pstPetSvr  = &stPkg.Body.PetSvr;
	CSPETATTRCHG *pstPetChgData= &pstPetSvr->PetSvrData.AttrChg;
	UNUSED(iTrait);

	pstPetDef = z_find_pet_def(pstEnv, iPetID, NULL);
	if (!pstPetDef)
	{
		goto error;
	}

	pstPetLevelDef = z_find_pet_level_def(pstEnv, 1, NULL);
	if (!pstPetDef)
	{
		goto error;
	}

	pstRolePet = &pstPetPackage->RolePets[pstPetPackage->Num];

	//基本属性
	memset(pstRolePet, 0, sizeof(*pstRolePet));
	pstRolePet->PetID = pstPetDef->PetID;
	STRNCPY(pstRolePet->Name, pstPetDef->Name, sizeof(pstRolePet->Name));
	pstRolePet->WID =z_get_gid(pstEnv->pstAppCtx, pstEnv);
	pstRolePet->LoveVal = BORN_PET_LOVE_VAL;
	pstRolePet->Level = 1;
	pstRolePet->Hierarchy = 1;
	pstRolePet->NextStrenLvl = 1; 
	pstRolePet->Imp = pstPetDef->Important;
	pstRolePet->WenBao = PET_MAX_WENBAO;
	pstRolePet->MiscFlag |= PET_MISC_FLAG_CHECK_JIYANG;
	
	//计算资质
	if(pet_normal_wash_genius(pstEnv, pstPetDef, pstRolePet) < 0)
	{
		goto error;
	}

	//获得天赋
	pet_native_skill(pstEnv, pstPetDef, pstRolePet);
	
	//获得技能栏|新版的宠物出生不会有技能格了
	/*if(pstPetDef->BornSkillGrid)
	{
		pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillID = 0;
		pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillLevel = 0;
		pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].GridType = pstPetDef->BornSkillGrid;
		pstRolePet->PetSkills.SkillGridNum++;
	}*/
	
	pstPetPackage->Num++;

	//通知客户端
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	
	//宠物更新各项属性
	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	pstRolePet->HP = pstRolePet->MaxHP;

	pstPetSvr->PetSvrType = PET_ATTR_CHG;
	pstPetChgData->Type = PET_CHG_HP;
	pstPetChgData->Wid = pstRolePet->WID;
	pstPetChgData->AttrValue.ChgHP.HP = pstRolePet->HP;
	pstPetChgData->AttrValue.ChgHP.XieDaiPos = -1;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_PET157, pstRolePet->Name);
	
	//运营日志
	z_oplog_event_begin(pstEnv);
	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_BORN, NULL, NULL, NULL,NULL);

	range_pet_set(pstEnv, pstPlayer, pstRolePet, pstPetDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstRolePet, pstPetDef));

	//成长之路
	{
		ACHIEVEMENTCOND stCond;
	
		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_PET_GET;
		stCond.Val1 = 0;
	
		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);

		if (pstPetDef->Type == PET_TYPE_ABERRANCE)
		{
			stCond.Val1 = 1;
	
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}

		if (pstRolePet && pstRolePet->PetGenius == PET_GENIUS_S)
		{
			stCond.Val1 = 2;
	
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
		
		
	}


	return pstRolePet;

error:
	return NULL;
}

//随即技能格类型，每一种类型的格子不可多于三个。
int get_ran_skillgrid_type(ROLEPET *pstRolePet)
{
	int i;
	int sMap[MAX_PET_SKILL_TYPE+1];
	int iRndNum;
	char sRndType[MAX_PET_SKILL_TYPE+1];
	int iAtk, iOwner, iSelf;
	PETSKILL *pstPetSkill = &pstRolePet->PetSkills;
	
	if (pstPetSkill->SkillGridNum > MAX_PET_SKILL_GRID)
	{
		return 0;
	}

	memset(sMap, 0, sizeof(sMap));
	iAtk = iOwner = iSelf = 0;
	for(i=0; i<pstPetSkill->SkillGridNum; i++)
	{
		switch(pstPetSkill->SkillGrid[i].GridType)
		{
			case PET_SKILL_ATK:
				iAtk++;
				if(iAtk >= MAX_ONE_PET_SKILL_TYPE)
				{
					sMap[PET_SKILL_ATK] = 1;
				}
				break;
			case PET_SKILL_OWNER_BUFF:
				iOwner++;
				if(iOwner >= MAX_ONE_PET_SKILL_TYPE)
				{
					sMap[PET_SKILL_OWNER_BUFF] = 1;
				}
				break;
			case PET_SKILL_SELF_BUFF:
				iSelf++;
				if(iSelf >= MAX_ONE_PET_SKILL_TYPE)
				{
					sMap[PET_SKILL_SELF_BUFF] = 1;
				}
				break;
			default:
				break;
		}
	}

	iRndNum = 0;
	for (i = PET_SKILL_ATK; i<=MAX_PET_SKILL_TYPE; i++)
	{
		if (!sMap[i])
		{
			sRndType[iRndNum] = i;
			iRndNum++;
		}
	}

	if (iRndNum > 0)
	{
		return sRndType[(int)RAND1(iRndNum)];
	}

	return 0;
}

int z_add_pet_lvup(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet)
{
	PETDEF *pstPetDef;
	PETLEVELDEF *pstPetLevelDef;
	PetExpOnline *pstExp;

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}
	
	pstExp = pet_find_exponline(pstPlayer, pstRolePet);
	if (NULL == pstExp)
	{
		return -1;
	}

	if (pstRolePet->Level>= MAX_PET_LEVEL)
	{
		pstExp->iNextLevelExp = -1;
		return -1;
	}
	
	pstPetLevelDef = z_find_pet_level_def(pstEnv, pstRolePet->Level+1, NULL);
	if (!pstPetLevelDef)
	{
		pstExp->iNextLevelExp = -1;
		return -1;
	}
	pstExp->iNextLevelExp = pstPetLevelDef->LevelUpExp;

	pstRolePet->Exp = 0;
	pstRolePet->Level++;	

	//随即获得技能格子
	if (	pstPetLevelDef->AddSkillGrid &&
		pstRolePet->PetSkills.SkillGridNum < MAX_PET_SKILL_GRID)
	{
		int iRandNum = 0;
		PetRndSkill astRndSkill[PET_RND_SKILL_MAX];	

		// 获取所有可随机的技能
		if (pet_get_all_skill(pstEnv,pstRolePet,&iRandNum, astRndSkill) < 0)
		{
			return -1;
		}
		pet_rand_skill_in(pstEnv,pstPlayer, pstRolePet,&pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum], 
							&iRandNum,astRndSkill);
		pstRolePet->PetSkills.SkillGridNum++;
	}
	/*
	if (pstPetLevelDef->AddSkillGrid && 
		pstRolePet->PetSkills.SkillGridNum < MAX_PET_SKILL_GRID)
	{
		iSkillGridType = get_ran_skillgrid_type(pstRolePet);
		if (iSkillGridType)
		{
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].GridType = iSkillGridType;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillID = 0;
			pstRolePet->PetSkills.SkillGrid[pstRolePet->PetSkills.SkillGridNum].SkillLevel = 0;

			pstRolePet->PetSkills.SkillGridNum++;

			z_sys_strf( pstEnv, pstPlayer, SYS_MIDDLE2, 
				"宠物[%s]获得了1格技能栏,可以学习更多技能了", pstRolePet->Name);
		}
	}
	*/
	
	pet_update_online(pstEnv, pstPlayer,  pstRolePet);
	
	pstRolePet->HP = pstRolePet->MaxHP;
	pet_hp_action(pstEnv, pstPlayer, pstRolePet, pstRolePet->HP);

	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);
	pet_level_up_action(pstEnv, pstPlayer, pstRolePet);

	if (is_pet_heti(pstPlayer))
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	//特殊任务
	task_pet_trace(pstEnv, pstPlayer, pstRolePet, TASK_OBJ_PET);

	range_pet_set(pstEnv, pstPlayer, pstRolePet, pstPetDef, RANGE_MAIN_PET_SUB_FIGHT,
					pet_fight_val(pstEnv, pstRolePet, pstPetDef));

	z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_COMBAT, 
		ZONEERR_PET190, pstRolePet->Name, pstRolePet->Level);

		//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_PET_LEVEL;
		stCond.Val1 = pstRolePet->Level;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}

	return 0;
}

int add_pet_exp_in(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet, unsigned int uiExp)
{

	unsigned int uiLevelExp = 0;
	unsigned int uiTotalExp = 0;
	PETDEF *pstPetDef;
	PETLEVELDEF *pstPetLevelDef;
	PetExpOnline *pstPetExp;
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSPETSVR *pstPetSvr = &pstPkg->Body.PetSvr;
	CSPETATTRCHG *pstPetChg = &pstPetSvr->PetSvrData.AttrChg;

	if (( pstRolePet->Level - pstPlayer->stRoleData.Level) > PET_LEVEL_LIMIT)
	{
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
	if (!pstPetDef)
	{
		return -1;
	}

	pstPetExp = pet_find_exponline(pstPlayer, pstRolePet);
	if (!pstPetExp)
	{
		return -1;
	}

	if (pstRolePet->Level>= MAX_PET_LEVEL)
	{
		pstPetExp->iNextLevelExp = -1;
		return -1;
	}
		
	if (pstPetExp->iNextLevelExp == -1)
	{
		return -1;
	}
	else if (pstPetExp->iNextLevelExp > 0 )
	{
		uiLevelExp = pstPetExp->iNextLevelExp;
	}

	if (uiLevelExp == 0)
	{
		pstPetLevelDef = z_find_pet_level_def(pstEnv, pstRolePet->Level, NULL);
		if (!pstPetLevelDef)
		{
			pstPetExp->iNextLevelExp = -1;
			return -1;
		}
	
		uiLevelExp = pstPetLevelDef->LevelUpExp;
		pstPetExp->iNextLevelExp = pstPetLevelDef->LevelUpExp;
	}
	
	if ( uiLevelExp <= (pstRolePet->Exp + uiExp) )
	{
		int iExpTmp = pstRolePet->Exp + uiExp - uiLevelExp;
		int uiTotalExp = uiLevelExp - pstRolePet->Exp;
		ROLEPET stForwardPet;

		stForwardPet = *pstRolePet;

		if ((pstRolePet->Level - pstPlayer->stRoleData.Level) >= PET_LEVEL_LIMIT)
		{
			pstRolePet->Exp = uiLevelExp;
		}
		else
		{
			//升级
			z_add_pet_lvup(pstEnv, pstPlayer, pstRolePet);		
			pstRolePet->Exp = 0;
			z_oplog_event_begin(pstEnv);
			z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, &stForwardPet, OP_PET_LVUP, NULL, NULL, NULL,NULL);

			if(iExpTmp > 0) 
			{	
				int iRet;
				
				iRet = add_pet_exp_in(pstEnv, pstPlayer, pstRolePet,  iExpTmp);
				if(iRet > 0)
				{
					uiTotalExp += iRet;
				}
			}
			
			return uiTotalExp;	
		
		}
	}
	else 
	{
		uiTotalExp = uiExp;
		pstRolePet->Exp = pstRolePet->Exp + uiExp;
	}

	pstPetSvr->PetSvrType = PET_ATTR_CHG; 
	
	pstPetChg->Type = PET_CHG_EXP;
	pstPetChg->Wid = pstRolePet->WID;
	pstPetChg->AttrValue.ChgExp = pstRolePet->Exp;

	Z_CSHEAD_INIT(&pstPkg->Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, pstPkg);

	return uiTotalExp;
}

int z_add_pet_exp(ZONESVRENV *pstEnv,Player *pstPlayer, ROLEPET *pstRolePet, unsigned int uiExp)
{
	if (!(pstRolePet->StatFlag &  PET_STAT_FIGHT))
	{
		return -1;
	}

	return add_pet_exp_in(pstEnv, pstPlayer, pstRolePet, uiExp);
}

int mon_booty_assign_pet_resource(ZONESVRENV* pstEnv, MONSTERDEF *pstMonDef , 
										 Player *pstPlayer, unsigned int  uiExp, int TeamExpMul)
{
	int i;
	int iAddExp=0;
	unsigned int uiPetExpLimit = 0;
	PETLEVELDEF *pstPetLevelDef;
	PetExpOnline *pstPetExp;
	int iPetBuffExpUp = 0;
	STATUSRESULT stStatusResult;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	int iRet = 0;

	for (i = 0; i < pstPetPackage->Num; i++)
	{
		if ( !(pstPetPackage->RolePets[i].StatFlag & PET_STAT_FIGHT) )
		{
			continue;
		}
		
		pstPetExp = pet_find_exponline(pstPlayer, &pstPetPackage->RolePets[i]);
		if (pstPetExp )
		{
			uiExp = uiExp * (pstPetExp->nExpMul/100.0+1) + pstPetExp->unExpAdd;
			if ( pstPetExp->iNextLevelExp == -1 )
			{
				continue;
			}
		}
		
		pstPetLevelDef = z_find_pet_level_def(pstEnv, pstPetPackage->RolePets[i].Level, NULL);
		if (NULL == pstPetLevelDef)
		{
			continue;
		}
		
		uiPetExpLimit = 0;
		
		switch(pstMonDef->SubType)
		{
			case MON_SUB_TYPE_MINI:
				
				uiPetExpLimit = pstPetLevelDef->MonMiniExpLimit;
				break;
			case MON_SUB_TYPE_WEAK:
				
				uiPetExpLimit = pstPetLevelDef->MonWeakExpLimit;
				break;
			case MON_SUB_TYPE_SOFT:

				uiPetExpLimit = pstPetLevelDef->MonSoftExpLimit;
				break;
			case MON_SUB_TYPE_NORM:

				uiPetExpLimit = pstPetLevelDef->MonNormalExpLimit;
				break;
			case MON_SUB_TYPE_HARD:

				uiPetExpLimit = pstPetLevelDef->MonHardExpLimit;
				break;
			case MON_SUB_TYPE_XI:

				uiPetExpLimit = pstPetLevelDef->MonXiExpLimit;
				break;
			case MON_SUB_TYPE_JIN:

				uiPetExpLimit = pstPetLevelDef->MonJinExpLimit;
				break;
			case MON_SUB_TYPE_BOSS:

				uiPetExpLimit = pstPetLevelDef->MonBossExpLimit;
				break;
			case MON_SUB_TYPE_JI:

				uiPetExpLimit = pstPetLevelDef->MonJiExpLimit;
				break;
			default:
				break;
		}
		
		if (uiPetExpLimit > 0 && uiExp > uiPetExpLimit)
		{
			iAddExp = uiPetExpLimit;
		}
		else
		{
			iAddExp = uiExp;
		}

		//所有宠物影响的经验倍率
		z_get_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_PET_ADDEXP, &stStatusResult, 0);
		iPetBuffExpUp = stStatusResult.ResultVal1;


		iAddExp =  iAddExp*( 100+ TeamExpMul )/100.0*(100-pstPlayer->bAasIncomeLimt)/100.0*(100 + iPetBuffExpUp)/100.0;

		//等级差限制 
		iAddExp = iAddExp *(1+ (pstPlayer->stRoleData.Level - pstPetPackage->RolePets[i].Level)/10.0);
		if(iAddExp < 0)
		{
			iAddExp = 0;
		}
		//宠物天赋,打怪经验	
		pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_EXP_ADD_MAP, &iAddExp,&pstPetPackage->RolePets[i]);
		iRet = z_add_pet_exp(pstEnv, pstPlayer, &pstPetPackage->RolePets[i], iAddExp);
		if(iRet > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_COMBAT, 
				ZONEERR_PET191, pstPetPackage->RolePets[i].Name, iRet);
		}
	}
	
	return 0;
}

int pet_package_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iFlag = 0;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;
	tdr_ulonglong ullWid = pstPetOnline->llPetWidBackup;

	memset(pstPetOnline,0,sizeof(PetOnline));
	pstPetOnline->llPetWidBackup = ullWid;

	for (i=0; i<pstPetPackage->Num; i++)
	{
		pstRolePet = &pstPetPackage->RolePets[i];

		pet_strengthen_val_check(pstRolePet);

		pet_update_online(pstEnv, pstPlayer, pstRolePet);

		if (PET_STAT_FIGHT & pstRolePet->StatFlag)
		{
			iFlag = 1;

			pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
			if (pstPetDef)
			{
				pet_update_skill(pstEnv, pstPlayer, pstRolePet, pstPetDef);
			}
		}
	}

	if(pstPetOnline->bFightShiNum)
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	if (iFlag)
	{
		pet_player_view_action(pstEnv, pstPlayer);
	}

	return 0;
}

int open_pet_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	UNUSED( pstAppCtx );
	
	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->ID = iNpcID;
	pstActionRes->Data.UI.UIType = iUIType;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int pet_svr_act(ZONESVRENV* pstEnv, Player *pstPlayer, int iType)
{
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	CSPETACTINFO *pstActInfo = &pstPetSvr->PetSvrData.PetActInfo;

	pstPetSvr->PetSvrType = PET_SVR_ACT;
	pstActInfo->Type = iType;

	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}


ROLEPET *pet_get_camp(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	ROLEPET *pstRolePet;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	UNUSED(pstEnv);
	
	for(i=0; i <pstPetOnline->bFightShiNum; i++)
	{
		pstRolePet = z_find_role_pet(pstPlayer, pstPetOnline->astFightShi[i].stRolePetOnline.ullWID, NULL);
		if (pstRolePet)
		{
			return pstRolePet;
		}
	}
	return NULL;
}

int pet_strengthen_val_get(ROLEPET *pstRolePet)
{
	int i;
	int iVal = 0;

	for(i=0; i<pstRolePet->StrengthenLevel; i++)
	{
		iVal += pstRolePet->StrengthenInfo[i];
	}

	return iVal;
}

//之前的强化等级兼容
int pet_strengthen_val_check(ROLEPET *pstRolePet)
{
	int i;
	
	for(i=0; i<pstRolePet->StrengthenLevel; i++)
	{
		if(0 == pstRolePet->StrengthenInfo[i])
		{
			pstRolePet->StrengthenInfo[i] = 3;
		}
	}

	return 0;
}


int pet_heti_action(ZONESVRENV* pstEnv, Player *pstPlayer,int iFlag, ROLEPET *pstRolePet)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	CSHETI *pstHeti = &pstActionRes->Data.Heti;
	
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_PET_HETI;
	pstHeti->Flag = iFlag;
	pstHeti->PetGenius = pstRolePet->PetGenius;
	pstHeti->PetID = pstRolePet->WID;
	pstHeti->Zhanchong = (pstRolePet->MiscFlag&PET_MISC_FLAG_COMBAT)?1:0;
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	return 0;
}



int pet_heti_attr_move_in(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, float fPer)
{
	SKILLRESULT stResult;

	memset(&stResult, 0,sizeof(stResult));
	if (player_is_magic_career(pstPlayer->stRoleData.Career))
	{
		stResult.SkillResultID = RESULT_ICE_ATK;
	}
	else
	{
		stResult.SkillResultID = RESULT_PHSIC_ATK;
	}
	stResult.SkillResultVal1 = pstRolePet->FightAttr.PhsicAtk*fPer;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_HPMAX;
	stResult.SkillResultVal1 = pstRolePet->MaxHP*fPer;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_PHSIC_DEF;
	stResult.SkillResultVal1 = pstRolePet->FightAttr.PhsicDef*fPer;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_ICE_DEF;
	stResult.SkillResultVal1 = pstRolePet->FightAttr.IceDef*fPer;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_HITRATE;
	stResult.SkillResultVal2= pstRolePet->FightAttr.HitRate*fPer;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_FLEE;
	stResult.SkillResultVal1 = pstRolePet->FightAttr.Flee*fPer;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);
	return 0;
}

int pet_heti_attr_move(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	#if 0

		物理攻击、
		生命值、
		物理防御、
		魔法防御、
		命中等级、
		闪避等级，合体之后15%的宠物属性附加在人物角色身上
		struct tagPetFightAttr
	{
   	 	unsigned short PhsicAtk;                        	/*   物理攻击 */
    		unsigned short IceAtk;                          	/*   魔法攻击 */
    		unsigned short PhsicDef;                        	/*   物理防御 */
    		unsigned short IceDef;                          	/*   魔法防御 */
    		unsigned short HitRate;                         	/*   命中 */	
    		unsigned short Flee;                            	/*   闪避 */
    		short HeavyHitRate;                    			/*   暴击率 */
    		short TiShen;                          			/*  Ver.163 替身率 */
	};

	#endif

	#define PEI_HETI_ATTR_MOVE_VAL 0.15
	ROLEPET *pstRolePet ;
	SKILLRESULT stResult;
	PETDEF *pstPetDef;
	int i;
	PETPACKAGE *pstPetPackage = &pstPlayer->stRoleData.Package.PetPackage;

	if (!is_pet_heti(pstPlayer))
		return 0;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstPlayer->stOnline.stHeti.PetWID,NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	pstPetDef = z_find_pet_def( pstEnv,pstRolePet->PetID, NULL);
	if (!pstPetDef)
		return -1;

	// 有特技的宠物根据主属性多加5%
	if (pstPetDef->TejiID > 0)
	{
		for (i=0; i<pstRolePet->PetSkills.SkillGridNum; i++)
		{
			if (pstRolePet->PetSkills.SkillGrid[i].SkillID == pstPetDef->TejiID)
			{
				// 0 血宠 1防宠 2攻宠
				memset(&stResult, 0,sizeof(stResult));
				if (pstPetDef->PetAttrType == 0)
				{
					stResult.SkillResultID = RESULT_HPMAX;
					stResult.SkillResultVal1 = pstRolePet->MaxHP*0.05;
					z_calc_diathesis(pstEnv, pstPlayer, &stResult);
				}
				else if(pstPetDef->PetAttrType == 1)
				{
					stResult.SkillResultID = RESULT_PHSIC_DEF;
					stResult.SkillResultVal1 = pstRolePet->FightAttr.PhsicDef*0.05;
					z_calc_diathesis(pstEnv, pstPlayer, &stResult);
				}
				else if (pstPetDef->PetAttrType == 2)
				{
					// 祭司 法师加法功
					// 战士 枪手加物攻
					if (player_is_magic_career(pstPlayer->stRoleData.Career))
					{
						stResult.SkillResultID = RESULT_ICE_ATK;
					}
					else
					{
						stResult.SkillResultID = RESULT_PHSIC_ATK;
					}
					stResult.SkillResultVal1 = pstRolePet->FightAttr.PhsicAtk*0.05;
					z_calc_diathesis(pstEnv, pstPlayer, &stResult);
				}
				break;
			}
		}
	}

	// 战宠合体
	if (pstRolePet->MiscFlag&PET_MISC_FLAG_COMBAT)
	{
		for (i = 0; i < pstPetPackage->Num; i++)
		{
			// 战宠都要计算数值
			if (!(pstPetPackage->RolePets[i].MiscFlag&PET_MISC_FLAG_COMBAT))
			{
				continue;
			}
			
			if  (pstRolePet->WID == pstPetPackage->RolePets[i].WID )
			{
				// 主宠加10%的属性
				pet_heti_attr_move_in(pstEnv, pstPlayer, &pstPetPackage->RolePets[i], 0.40f);
			}
			else
			{
				// 副宠加5%的属性
				pet_heti_attr_move_in(pstEnv, pstPlayer, &pstPetPackage->RolePets[i], 0.25f);
			}
		}		
	}
	else
	{
		pet_heti_attr_move_in(pstEnv, pstPlayer, pstRolePet, 0.25f);
	}

	
	/*
	memset(&stResult, 0,sizeof(stResult));
	if (player_is_magic_career(pstPlayer->stRoleData.Career))
	{
		stResult.SkillResultID = RESULT_ICE_ATK;
	}
	else
	{
		stResult.SkillResultID = RESULT_PHSIC_ATK;
	}
	stResult.SkillResultVal1 = pstRolePet->FightAttr.PhsicAtk*PEI_HETI_ATTR_MOVE_VAL;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_HPMAX;
	stResult.SkillResultVal1 = pstRolePet->MaxHP*PEI_HETI_ATTR_MOVE_VAL;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_PHSIC_DEF;
	stResult.SkillResultVal1 = pstRolePet->FightAttr.PhsicDef*PEI_HETI_ATTR_MOVE_VAL;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_ICE_DEF;
	stResult.SkillResultVal1 = pstRolePet->FightAttr.IceDef*PEI_HETI_ATTR_MOVE_VAL;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_HITRATE;
	stResult.SkillResultVal1 = pstRolePet->FightAttr.HitRate*PEI_HETI_ATTR_MOVE_VAL;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);

	memset(&stResult, 0,sizeof(stResult));
	stResult.SkillResultID = RESULT_FLEE;
	stResult.SkillResultVal1 = pstRolePet->FightAttr.Flee*PEI_HETI_ATTR_MOVE_VAL;
	z_calc_diathesis(pstEnv, pstPlayer, &stResult);
	*/

	return 0;
}

int pet_heti_start(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEPET *pstRolePet, int iUpAttr)
{
	pstPlayer->stOnline.stHeti.PetWID = pstRolePet->WID;
	pstRolePet->MiscFlag |= PET_MISC_FLAG_HETI;				// 宠物标记
	if (iUpAttr)
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);	// 重新计算数值
	}
	pet_heti_action(pstEnv, pstPlayer,0, pstRolePet);			// 通知客户端
	return 0;
}

int pet_heti_end(ZONESVRENV* pstEnv,Player *pstPlayer,ROLEPET *pstRolePet,  int iUpAttr)
{
	pstPlayer->stOnline.stHeti.PetWID = 0;
	pstRolePet->MiscFlag &= ~PET_MISC_FLAG_HETI;				
	if (iUpAttr)
	{
		z_update_all_attr(pstEnv->pstAppCtx, pstEnv, pstPlayer);	// 重新计算数值
	}
	pet_heti_action(pstEnv, pstPlayer,1, pstRolePet);	
	return 0;
}



int pet_heti_in(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEPET *pstRolePet, int iStart)
{
	if (iStart)
	{
		pet_heti_start(pstEnv,pstPlayer,pstRolePet,1);
	}
	else
	{
		pet_heti_end(pstEnv,pstPlayer,pstRolePet,1);
	
		if (pstRolePet->LoveVal < PET_FIGHT_LOVEVAL)
		{
			pet_set_stat(pstEnv, pstPlayer, pstRolePet, 0);
		}
		else if (pstRolePet->StatFlag&PET_STAT_FIGHT)
		{
			z_player_backup_pet(pstEnv, pstPlayer);
			z_player_restore_pet_nowmap(pstEnv, pstPlayer);
		}
	}
	return 0;
}


int pet_heti_check(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET **ppstRolePet)
{
	ROLEPET *pstRolePet;
	PETDEF *pstPetDef;
	int iMaxLoveVal;
		
	pstRolePet = pet_get_camp( pstEnv, pstPlayer);
	if (!pstRolePet)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_PET192);
		return -1;
	}

	if (pstPlayer->stOnline.stHeti.PetWID > 0 ||
		pstRolePet->MiscFlag &PET_MISC_FLAG_HETI )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET193);
		return -1;
	}

	if (pstPlayer->stRoleData.Level < PLAYER_PET_HETI_LEVEL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_PET194,
						PLAYER_PET_HETI_LEVEL);
		return -1;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
		return -1;

	if (pstRolePet->Level < pstPetDef->Heti.MinLevel)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_PET159,pstPetDef->Heti.MinLevel);
		return -1;
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL) )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, 
						SYS_MIDDLE2, ZONEERR_PET158);
		return -1;
	}

	iMaxLoveVal = MAX_PET_LOVE_VAL;
	pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_MAX_LOVE_VAL_ADD_MAP, 
							&iMaxLoveVal,pstRolePet);
	if (iMaxLoveVal <= 0)
	{
		return -1;
	}

	if (iMaxLoveVal > MAX_PET_LOVE_VAL)
	{
		//iMaxLoveVal = MAX_PET_LOVE_VAL;
	}

	if (pstRolePet->LoveVal < 8000 /*(1.0*pstPetDef->Heti.MinLoyalty/100*iMaxLoveVal)*/)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
					ZONEERR_PET160,80);
		return -1;
	}

	iMaxLoveVal =1.0*pstPetDef->Heti.UseLoyalty/100*iMaxLoveVal;

	if (ppstRolePet)
	{
		*ppstRolePet = pstRolePet;
	}

	return iMaxLoveVal; //返回消耗忠诚度
}

int pet_heti(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPET *pstRolePet=NULL;
	int iMaxLoveVal = 0;

	if ((iMaxLoveVal = pet_heti_check(pstEnv, pstPlayer, &pstRolePet)) < 0)
	{
		return -1;
	}

	if (!pstRolePet)
	{
		return -1;
	}
	
	// 开始合体
	pet_love_val_chg(pstEnv, pstPlayer, pstRolePet, -iMaxLoveVal);// 减消耗
	pet_heti_in(pstEnv, pstPlayer,pstRolePet,1);
	return 0;
}

// 每分钟减少一点忠诚度
int pei_heti_tick(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	#define PEI_HETI_DEC 1
	ROLEPET *pstRolePet ;

	if (pstPlayer->stOnline.stHeti.PetWID <= 0)
		return 0;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstPlayer->stOnline.stHeti.PetWID,NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	pet_love_val_chg(pstEnv, pstPlayer, pstRolePet, -(PEI_HETI_DEC*PET_LOVE_VAL_UNIT));// 减消耗
	
	return 0;
}

int is_pet_heti(Player *pstPlayer)
{
	if (pstPlayer->stOnline.stHeti.PetWID > 0)
		return 1;
	return 0;
}

ROLEPET * pet_find_heti(Player *pstPlayer)
{
	int i;
	ROLEPET *pstPet;

	for (i=0; i<pstPlayer->stRoleData.Package.PetPackage.Num; i++)
	{
		pstPet = &pstPlayer->stRoleData.Package.PetPackage.RolePets[i];
		if (pstPet->MiscFlag&PET_MISC_FLAG_HETI)
		{
			return pstPet;
		}
	}

	return NULL;
}

ROLEPET * pet_find_fight(Player *pstPlayer)
{
	int i;
	ROLEPET *pstPet;

	for (i=0; i<pstPlayer->stRoleData.Package.PetPackage.Num; i++)
	{
		pstPet = &pstPlayer->stRoleData.Package.PetPackage.RolePets[i];
		if (pstPet->StatFlag == PET_STAT_FIGHT)
		{
			return pstPet;
		}
	}

	return NULL;
}

tdr_ulonglong is_login_pet_heti(ZONESVRENV* pstEnv,Player *pstPlayer)
{
	ROLEPET *pstRolePet = NULL;
	int i;
	PETPACKAGE *pstPkg = &pstPlayer->stRoleData.Package.PetPackage;

	for (i=0; i<pstPkg->Num; i++)
	{
		if (pstPkg->RolePets[i].StatFlag&PET_STAT_FIGHT)
		{
			pstRolePet = &pstPkg->RolePets[i];
			break;
		}
	}

	if (!pstRolePet)
	{
		return 0;
	}

	if (pstRolePet->MiscFlag & PET_MISC_FLAG_HETI)
	{
		return pstRolePet->WID;
	}
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}


int pet_move_star(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETMOVESTAR *pstCSMove)
{
	int iRet, iStrengthen;
	PETSTRENGTHENDEF *pstPetStrengthenDef;
	ROLEPET *pstDstRolePet = NULL, *pstSrcRolePet;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;

	pstDstRolePet = z_find_role_pet(pstPlayer, pstCSMove->DstPetWid, NULL);
	if(NULL == pstDstRolePet)
	{
		goto _error;
	}

	pstSrcRolePet = z_find_role_pet(pstPlayer, pstCSMove->SrcPetWid, NULL);
	if(NULL == pstDstRolePet)
	{
		goto _error;
	}

	if(pstSrcRolePet->StrengthenLevel < 1)
	{
		goto _error;
	}

	iStrengthen = pet_strengthen_perfect_num_get(pstSrcRolePet);
	if(iStrengthen > 0)
	{
		pstPetStrengthenDef = z_find_pet_strengthen_def(pstEnv, iStrengthen);
		if(!pstPetStrengthenDef)
		{
			goto _error;
		}

		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstPetStrengthenDef->MoveMoneyType, pstPetStrengthenDef->MoveMoneyVal))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
			goto _error;
		}

		iRet = package_dec_money(pstEnv, pstPlayer, pstPetStrengthenDef->MoveMoneyType, pstPetStrengthenDef->MoveMoneyVal);
		if (ATTR_ID_GODCOIN == pstPetStrengthenDef->MoveMoneyType)
		{
			godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
						pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_PETMOVESTAR,pstSrcRolePet->PetID,0,0,
						pstPetStrengthenDef->MoveMoneyVal,"宠物移星");
		}
		else if (ATTR_ID_BINDGODCOIN == pstPetStrengthenDef->MoveMoneyType)
		{
			z_bindgold_consume_oplog(pstEnv,pstPlayer, BINDGOLD_OPTYPE_PET_MOVESTAR,pstSrcRolePet->PetID,
						pstPetStrengthenDef->MoveMoneyVal,"宠物移星");
		}
		else if (ATTR_ID_BULL == pstPetStrengthenDef->MoveMoneyType)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PET_MOVESTAR, 
				pstSrcRolePet->PetID, pstPetStrengthenDef->MoveMoneyVal-iRet, iRet, "宠物移星");
		}
		else if (ATTR_ID_MONEY == pstPetStrengthenDef->MoveMoneyType)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PET_MOVESTAR, 
				pstSrcRolePet->PetID, 0, pstPetStrengthenDef->MoveMoneyVal, "宠物移星");
		}
	}

	//转移
	pstDstRolePet->StrengthenLevel = pstSrcRolePet->StrengthenLevel;
	pstDstRolePet->StrengthenCount = pstSrcRolePet->StrengthenCount;
	memcpy(&pstDstRolePet->StrengthenInfo[0], 
		&pstSrcRolePet->StrengthenInfo[0], MAX_PET_STRENGTHEN_LEVEL*sizeof(pstDstRolePet->StrengthenInfo[0]));

	//源宠物清理强化
	pstSrcRolePet->StrengthenLevel = 0;
	pstSrcRolePet->StrengthenCount = 0;
	memset(&pstSrcRolePet->StrengthenInfo[0], 0, MAX_PET_STRENGTHEN_LEVEL*sizeof(pstSrcRolePet->StrengthenInfo[0]));

	pet_update_online(pstEnv, pstPlayer, pstDstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstDstRolePet);


	pet_update_online(pstEnv, pstPlayer, pstSrcRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstSrcRolePet);

	pstPetSvr->PetSvrData.PetMoveStarSucc.Succ = 1;

	z_oplog_event_begin(pstEnv);
	z_role_pet_oplog( pstEnv, pstPlayer, pstDstRolePet, NULL, OP_PET_MOVE_STAR, NULL, 
						NULL, NULL, NULL);
_final:
	
	pstPetSvr->PetSvrType = PET_SVR_MOVE_STAR_SUCC;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
	
_error:
	
	pstPetSvr->PetSvrData.PetMoveStarSucc.Succ = 0;
	goto _final;
	return 0;
}

/*int pet_eat_genius_val_get(ROLEPET *pstRolePet, PETDEF *pstPetDef, PetWashGeniusInfo *pstEatInfo)
{
	int iMul;
	PETGENIUS *pstGenius;

	PETIMPUPATTRDEF *pstDef = pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp, pstPetDef->PetAttrType);
	if (!pstDef)
		return -1;

	memset(pstEatInfo, 0, sizeof(*pstEatInfo));
	
	pstGenius = &pstDef->Genius[PET_GENIUS_C];

	if(pstRolePet->PetGenius == PET_GENIUS_C)
	{
		iMul = 20;
	}
	else if(pstRolePet->PetGenius == PET_GENIUS_B)
	{
		iMul = 50;
	}
	else if(pstRolePet->PetGenius == PET_GENIUS_A)
	{
		iMul = 80;
	}
	else if(pstRolePet->PetGenius == PET_GENIUS_S)
	{
		iMul = 100;
	}
	else
	{
		return 0;
	}

	pstEatInfo->iPhsicAtkGenius = (pstRolePet->PhsicAtkGenius - pstGenius->PhsicAtkGenius)*iMul/100.0;
	if(pstEatInfo->iPhsicAtkGenius < 0)
	{
		pstEatInfo->iPhsicAtkGenius = 0;
	}

	pstEatInfo->iHPGenius = (pstRolePet->HPGenius - pstGenius->HPGenius)*iMul/100.0;
	if(pstEatInfo->iHPGenius < 0)
	{
		pstEatInfo->iHPGenius = 0;
	}

	pstEatInfo->iDefGenius = (pstRolePet->DefGenius - pstGenius->DefGenius)*iMul/100.0;
	if(pstEatInfo->iDefGenius < 0)
	{
		pstEatInfo->iDefGenius = 0;
	}

	pstEatInfo->iIceDefGenius = (pstRolePet->IceDefGenius - pstGenius->IceDefGenius)*iMul/100.0;
	if(pstEatInfo->iIceDefGenius < 0)
	{
		pstEatInfo->iIceDefGenius = 0;
	}

	pstEatInfo->iHitRateGenius = (pstRolePet->HitRateGenius - pstGenius->HitRateGenius)*iMul/100.0;
	if(pstEatInfo->iHitRateGenius < 0)
	{
		pstEatInfo->iHitRateGenius = 0;
	}

	pstEatInfo->iFleeGenius = (pstRolePet->FleeGenius - pstGenius->FleeGenius)*iMul/100.0;
	if(pstEatInfo->iFleeGenius < 0)
	{
		pstEatInfo->iFleeGenius = 0;
	}

	return 0;
}*/

int pet_eat_exp_get(ZONESVRENV* pstEnv, ROLEPET *pstRolePet, unsigned int  *puiExp)
{
	int i;
	PETLEVELDEF *pstPetLevelDef;

	*puiExp = pstRolePet->Exp;
	for(i=pstRolePet->Level-1; i>=1; i--)
	{
		pstPetLevelDef = z_find_pet_level_def(pstEnv, i, NULL);
		if(pstPetLevelDef)
		{
			*puiExp += pstPetLevelDef->LevelUpExp;
		}
	}

	if(*puiExp > 0)
	{
		*puiExp = *puiExp*0.3;
	}
	
	return 0;
}

PETMONEYDEF *pet_money_def_find(ZONESVRENV* pstEnv, int iImp)
{
	PETMONEYDEF stDef;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stDef.Imp = iImp;

	iIndex = bsearch_int(&stDef, pstObj->sPetMoneyDef, pstObj->iPetMoneyDef, PET_MONEY_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETMONEYDEF *)pstObj->sPetMoneyDef[iIndex];
	}
	
	return NULL;
}

int pet_eat_add(ZONESVRENV* pstEnv,ROLEPET *pstDstPet, PETDEF *pstDstDef, ROLEPET *pstSrcPet, PETDEF *pstSrcDef, int iType)
{
	double d1=0,d2=0,d3=0;
	PETMONEYDEF *pstDstMoney;
	PETMONEYDEF *pstSrcMoney;
	PETIMPUPATTRDEF *pstSrcImpDef;
	PETIMPUPATTRDEF *pstDstImpDef;
	
	pstDstMoney = pet_money_def_find(pstEnv, pstDstPet->Imp);
	if (!pstDstMoney)
	{
		return 0;
	}

	pstSrcMoney = pet_money_def_find(pstEnv, pstSrcPet->Imp);
	if (!pstSrcMoney)
	{
		return 0;
	}

	pstSrcImpDef = pet_imp_upattr_def_find(pstEnv, pstSrcPet->Imp, pstSrcDef->PetAttrType);
	pstDstImpDef = pet_imp_upattr_def_find(pstEnv, pstDstPet->Imp, pstDstDef->PetAttrType);
	if (!pstSrcImpDef || !pstDstImpDef)
	{
		return 0;
	}

	if (	pstSrcPet->PetGenius == PET_GENIUS_D ||
		pstDstPet->PetGenius == PET_GENIUS_S)
	{
		return 0;
	}

	
	if (iType == 1)
	{

		// 第一步
		d1 = 1.0f*(pstSrcPet->PhsicAtkGenius - pstSrcImpDef->Genius[PET_GENIUS_D].PhsicAtkGenius);
		d2 = pstSrcImpDef->Genius[PET_GENIUS_S].PhsicAtkGenius*PET_S_LAST - pstSrcImpDef->Genius[PET_GENIUS_D].PhsicAtkGenius;
		if (d2 == 0)
		{
			return 0;
		}
		d1 = d1/d2;

		// 第二步
		if (pstDstMoney->Count == 0)
		{
			return 0;
		}
		d2 = (1.0f*pstSrcMoney->Count/pstDstMoney->Count)*(pstSrcMoney->Money[pstSrcPet->PetGenius]/10000.0);
	
		// 第三步
		d3 = pstDstImpDef->Genius[PET_GENIUS_S].PhsicAtkGenius*PET_S_LAST- 
			pstDstImpDef->Genius[PET_GENIUS_D].PhsicAtkGenius;
	
	}
	else if (iType == 2)
	{
		// 第一步
		d1 = 1.0f*(pstSrcPet->DefGenius - pstSrcImpDef->Genius[PET_GENIUS_D].DefGenius);
		d2 = pstSrcImpDef->Genius[PET_GENIUS_S].DefGenius*PET_S_LAST - pstSrcImpDef->Genius[PET_GENIUS_D].DefGenius;
		if (d2 == 0)
		{
			return 0;
		}
		d1 = d1/d2;

		// 第二步
		if (pstDstMoney->Count == 0)
		{
			return 0;
		}
		d2 = (1.0f*pstSrcMoney->Count/pstDstMoney->Count)*(pstSrcMoney->Money[pstSrcPet->PetGenius]/10000.0);

		// 第三步
		d3 = pstDstImpDef->Genius[PET_GENIUS_S].DefGenius*PET_S_LAST - 
			pstDstImpDef->Genius[PET_GENIUS_D].DefGenius;
	}
	else if (iType == 3)
	{
		// 第一步
		d1 = 1.0f*(pstSrcPet->IceDefGenius- pstSrcImpDef->Genius[PET_GENIUS_D].IceDefGenius);
		d2 = pstSrcImpDef->Genius[PET_GENIUS_S].IceDefGenius*PET_S_LAST - pstSrcImpDef->Genius[PET_GENIUS_D].IceDefGenius;
		if (d2 == 0)
		{
			return 0;
		}
		d1 = d1/d2;

		// 第二步
		if (pstDstMoney->Count == 0)
		{
			return 0;
		}
		d2 = (1.0f*pstSrcMoney->Count/pstDstMoney->Count)*(pstSrcMoney->Money[pstSrcPet->PetGenius]/10000.0);

		// 第三步
		d3 = pstDstImpDef->Genius[PET_GENIUS_S].IceDefGenius*PET_S_LAST - 
			pstDstImpDef->Genius[PET_GENIUS_D].IceDefGenius;	
	}
	else if (iType == 4)
	{
		// 第一步
		d1 = 1.0f*(pstSrcPet->FleeGenius- pstSrcImpDef->Genius[PET_GENIUS_D].FleeGenius);
		d2 = pstSrcImpDef->Genius[PET_GENIUS_S].FleeGenius*PET_S_LAST - pstSrcImpDef->Genius[PET_GENIUS_D].FleeGenius;
		if (d2 == 0)
		{
			return 0;
		}
		d1 = d1/d2;

		// 第二步
		if (pstDstMoney->Count == 0)
		{
			return 0;
		}
		d2 = (1.0f*pstSrcMoney->Count/pstDstMoney->Count)*(pstSrcMoney->Money[pstSrcPet->PetGenius]/10000.0);

		// 第三步
		d3 = pstDstImpDef->Genius[PET_GENIUS_S].FleeGenius*PET_S_LAST - 
			pstDstImpDef->Genius[PET_GENIUS_D].FleeGenius;		
	}
	else if (iType == 5)
	{
		// 第一步
		d1 = 1.0f*(pstSrcPet->HPGenius- pstSrcImpDef->Genius[PET_GENIUS_D].HPGenius);
		d2 = pstSrcImpDef->Genius[PET_GENIUS_S].HPGenius*PET_S_LAST- pstSrcImpDef->Genius[PET_GENIUS_D].HPGenius;
		if (d2 == 0)
		{
			return 0;
		}
		d1 = d1/d2;

		// 第二步
		if (pstDstMoney->Count == 0)
		{
			return 0;
		}
		d2 = (1.0f*pstSrcMoney->Count/pstDstMoney->Count)*(pstSrcMoney->Money[pstSrcPet->PetGenius]/10000.0);
		
		// 第三步
		d3 = pstDstImpDef->Genius[PET_GENIUS_S].HPGenius *PET_S_LAST - 
			pstDstImpDef->Genius[PET_GENIUS_D].HPGenius;		
	}
	else if(iType == 6)
	{
		// 第一步
		d1 = 1.0f*(pstSrcPet->HitRateGenius- pstSrcImpDef->Genius[PET_GENIUS_D].HitRateGenius);
		d2 = pstSrcImpDef->Genius[PET_GENIUS_S].HitRateGenius*PET_S_LAST - pstSrcImpDef->Genius[PET_GENIUS_D].HitRateGenius;
		if (d2 == 0)
		{
			return 0;
		}
		d1 = d1/d2;

		// 第二步
		if (pstDstMoney->Count == 0)
		{
			return 0;
		}
		d2 = (1.0f*pstSrcMoney->Count/pstDstMoney->Count)*(pstSrcMoney->Money[pstSrcPet->PetGenius]/10000.0);
	
		// 第三步
		d3 = pstDstImpDef->Genius[PET_GENIUS_S].HitRateGenius*PET_S_LAST - 
			pstDstImpDef->Genius[PET_GENIUS_D].HitRateGenius;		
	}
	
	return d1*d2*d3;
}

int pet_eat(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETEAT *pstCSEat)
{
	int iRet;
	unsigned int uiExp;
	PETGENIUSDEF *pstPetGeniusDef;
	PETDEF *psSrctPetDef, *pstDstPetDef;
	PETWASHGENIUSDEF *pstPetWashGeniusDef;
	//PetWashGeniusInfo stEatInfo;
	ROLEPET *pstDstRolePet = NULL, *pstSrcRolePet;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	PETIMPUPATTRDEF *pstImpDef ;//= pet_imp_upattr_def_find(pstEnv, pstRolePet->Imp, pstPetDef->PetAttrType);
	

	pstDstRolePet = z_find_role_pet(pstPlayer, pstCSEat->DstPetWid, NULL);
	if(NULL == pstDstRolePet)
	{
		goto _error;
	}

	pstSrcRolePet = z_find_role_pet(pstPlayer, pstCSEat->SrcPetWid, NULL);
	if(NULL == pstSrcRolePet)
	{
		goto _error;
	}

	pstDstPetDef = z_find_pet_def(pstEnv, pstDstRolePet->PetID, NULL);
	if (!pstDstPetDef)
	{
		goto _error;
	}

	psSrctPetDef = z_find_pet_def(pstEnv, pstSrcRolePet->PetID, NULL);
	if (!psSrctPetDef)
	{
		goto _error;
	}

	if(pstSrcRolePet->PetGenius < PET_GENIUS_C)
	{
		goto _error;
	}

	pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv, pstDstRolePet->Imp, pstDstRolePet->PetGenius);
	if(!pstPetWashGeniusDef)
	{
		goto _error;
	}

	pstPetGeniusDef = z_find_pet_pinzhi_def(pstEnv, pstSrcRolePet->PetGenius+1, NULL);
	if(!pstPetGeniusDef)
	{
		goto _error;
	}

	pstImpDef = pet_imp_upattr_def_find(pstEnv, pstDstRolePet->Imp, pstDstPetDef->PetAttrType);
	if (!pstImpDef)
	{
		goto _error;
	}
	
	if (0 > package_dec_money_test(pstEnv, pstPlayer, pstPetGeniusDef->EatMoneyType, pstPetGeniusDef->EatMoneyVal))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
		goto _error;
	}

	iRet = package_dec_money(pstEnv, pstPlayer, pstPetGeniusDef->EatMoneyType, pstPetGeniusDef->EatMoneyVal);
	if (ATTR_ID_GODCOIN == pstPetGeniusDef->EatMoneyType)
	{
		godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level, GODCOIN_OPER_FLAG_PETEAT,pstSrcRolePet->PetID,0,0,
					pstPetGeniusDef->EatMoneyVal,"宠物吞噬");
	}
	else if (ATTR_ID_BINDGODCOIN == pstPetGeniusDef->EatMoneyType)
	{
		z_bindgold_consume_oplog(pstEnv,pstPlayer, BINDGOLD_OPTYPE_PET_EAT,pstSrcRolePet->PetID,
					pstPetGeniusDef->EatMoneyVal,"宠物吞噬");
	}
	else if (ATTR_ID_BULL == pstPetGeniusDef->EatMoneyType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PET_EAT, 
			pstSrcRolePet->PetID, pstPetGeniusDef->EatMoneyVal-iRet, iRet, "宠物吞噬");
	}
	else if (ATTR_ID_MONEY == pstPetGeniusDef->EatMoneyType)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PET_EAT, 
			pstSrcRolePet->PetID, 0, pstPetGeniusDef->EatMoneyVal, "宠物吞噬");
	}
	

	//pet_eat_genius_val_get(pstSrcRolePet, psSrctPetDef, &stEatInfo);
	pet_eat_exp_get(pstEnv, pstSrcRolePet,&uiExp);

	//吞噬经验
	add_pet_exp_in(pstEnv, pstPlayer, pstDstRolePet, uiExp);

	pstDstRolePet->PhsicAtkGenius += pet_eat_add(pstEnv,pstDstRolePet, pstDstPetDef, pstSrcRolePet, psSrctPetDef, 1);
	if(pstDstRolePet->PhsicAtkGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].PhsicAtkGenius))
	{
		pstDstRolePet->PhsicAtkGenius = 
			get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].PhsicAtkGenius);
	}

	pstDstRolePet->HPGenius+= pet_eat_add(pstEnv,pstDstRolePet, pstDstPetDef, pstSrcRolePet, psSrctPetDef, 5);
	if(pstDstRolePet->HPGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].HPGenius))
	{
		pstDstRolePet->HPGenius = 
			get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].HPGenius);
	}

	pstDstRolePet->DefGenius+= pet_eat_add(pstEnv,pstDstRolePet, pstDstPetDef, pstSrcRolePet, psSrctPetDef, 2);
	if(pstDstRolePet->DefGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].DefGenius))
	{
		pstDstRolePet->DefGenius = 
			get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].DefGenius);
	}

	pstDstRolePet->HitRateGenius+= pet_eat_add(pstEnv,pstDstRolePet, pstDstPetDef, pstSrcRolePet, psSrctPetDef, 6);
	if(pstDstRolePet->HitRateGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].HitRateGenius))
	{
		pstDstRolePet->HitRateGenius = 
			get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].HitRateGenius);
	}

	pstDstRolePet->FleeGenius+= pet_eat_add(pstEnv,pstDstRolePet, pstDstPetDef, pstSrcRolePet, psSrctPetDef, 4);
	if(pstDstRolePet->FleeGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].FleeGenius))
	{
		pstDstRolePet->FleeGenius = 
			get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].FleeGenius);
	}

	pstDstRolePet->IceDefGenius+= pet_eat_add(pstEnv,pstDstRolePet, pstDstPetDef, pstSrcRolePet, psSrctPetDef,3);
	if(pstDstRolePet->IceDefGenius > get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].IceDefGenius))
	{
		pstDstRolePet->IceDefGenius = 
			get_max_pet_wash_genius(pstPetWashGeniusDef, pstImpDef->Genius[PET_GENIUS_S].IceDefGenius);
	}

	while(1)
	{
		if(0 > pet_genius_levelup_check( pstEnv,pstDstRolePet, pstDstPetDef, pstPetWashGeniusDef))
		{
			break;
		}

		
		if(pstDstRolePet->PetGenius >= PET_GENIUS_S)
		{
			break;
		}
		
		pstDstRolePet->PetGenius++;

		pstPetWashGeniusDef = z_find_pet_wash_genius_def(pstEnv, pstDstRolePet->Imp, pstDstRolePet->PetGenius);
		if(!pstPetWashGeniusDef)
		{
			break;
		}
	}

	// 不离不弃
	if (pstEnv->pstZoneObj->iPetLoveLevelDef )
	{
		pstDstRolePet->LoveLevel = pstEnv->pstZoneObj->iPetLoveLevelDef-1;
	}

	if (pstSrcRolePet->MiscFlag & PET_MISC_FLAG_COMBAT)
	{
		pstDstRolePet->MiscFlag |= PET_MISC_FLAG_COMBAT;
	}

	pet_update_online(pstEnv, pstPlayer, pstDstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstDstRolePet);
	pstPetSvr->PetSvrData.PetEatSucc.Succ = 1;

	//成长之路
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));		
		
		if (pstDstRolePet && pstDstRolePet->PetGenius == PET_GENIUS_S)
		{
			stCond.Type = COND_PET_GET;
			stCond.Val1 = 2;
	
			z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		}
	}

	z_oplog_event_begin(pstEnv);

	z_role_pet_oplog( pstEnv, pstPlayer, pstDstRolePet, NULL, OP_PET_EAT, NULL, 
						NULL, NULL, NULL);
	// 删除放最后 避免指针移位
	pet_del(pstEnv, pstPlayer, pstSrcRolePet->WID, 3, NULL);
_final:
	
	pstPetSvr->PetSvrType = PET_SVR_EAT_SUCC;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
	
_error:
	
	pstPetSvr->PetSvrData.PetEatSucc.Succ = 0;
	goto _final;
	return 0;
}

int player_revive_pet_update(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPET *pstRolePet;
	ZONEPWORLD *pstZonePworld;
	
	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if(!pstZonePworld)
	{
		return 0;
	}

	if(!is_battle_pworld(pstZonePworld))
	{
		return 0;
	}
	
	if(pstPlayer->stRoleData.MiscInfo.FightPetWID == 0ULL)
	{
		return 0;
	}

	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if(pstRolePet)
	{
		return 0;
	}

	pstRolePet = z_find_role_pet(pstPlayer, pstPlayer->stRoleData.MiscInfo.FightPetWID, NULL);
	if(!pstRolePet)
	{
		return 0;
	}
	
	pet_set_stat(pstEnv, pstPlayer, pstRolePet, PET_STAT_FIGHT);
	return 0;
}

int player_die_pet_add_power(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEPET *pstRolePet;
	ZONEPWORLD *pstZonePworld;
	
	pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
	if(!pstZonePworld)
	{
		return 0;
	}

	if(!is_battle_pworld(pstZonePworld))
	{
		return 0;
	}
	
	if(pstPlayer->stRoleData.MiscInfo.FightPetWID == 0ULL)
	{
		return 0;
	}

	pstRolePet = pet_get_camp(pstEnv, pstPlayer);
	if(!pstRolePet)
	{
		return 0;
	}
	
	pet_add_power(pstEnv, pstPlayer, pstRolePet, pstRolePet->MaxHP);
	return 0;
}

//宠物解绑需要的银币
int pet_unbind_money_val_by_genius(int PetGenius,int *piMoney)
{	
	switch(PetGenius)
	{
		case PET_GENIUS_D:
			*piMoney = PET_GENIUS_D_UNBIND_MONEY;				
			break;
		case PET_GENIUS_C:
			*piMoney = PET_GENIUS_C_UNBIND_MONEY;
			break;
		case PET_GENIUS_B:
			*piMoney = PET_GENIUS_B_UNBIND_MONEY;
			break;
		case PET_GENIUS_A:
			*piMoney = PET_GENIUS_A_UNBIND_MONEY;
			break;
		case PET_GENIUS_S:
			*piMoney = PET_GENIUS_S_UNBIND_MONEY;
			break;
		default:
			return -1;
			break;
	}
	
	return 0;
}

int pet_unbind(ZONESVRENV* pstEnv, Player *pstPlayer, CSPETUNBIND *pstPetUnbind)
{
	PETDEF *pstPetDef;
	ROLEPET *pstRolePet;
	int iSucc = 0;
	CSPKG stPkg;
	CSPETSVR *pstPetSvr = &stPkg.Body.PetSvr;
	int iMoney = 0;
	ITEMDEC stItemDec;
	int iUseNum = 0;
	ITEMDEF *pstItemDef;

	pstRolePet = z_find_role_pet(pstPlayer, pstPetUnbind->PetWid, NULL);
	if (!pstRolePet)
	{
		goto final;
	}

	pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, NULL);
	if (!pstPetDef)
	{
		goto final;
	}

	if(0 > pet_unbind_money_val_by_genius(pstRolePet->PetGenius,&iMoney))
	{
		goto final;
	}
	
	if ( package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY,iMoney) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_ARM116, iMoney, MONEY_TYPE_MONEY_NAME);
		goto final;
	}
	
	pstItemDef = z_find_itemdef(pstEnv, PET_UNBIND_ITEM_ID);
	if(!pstItemDef)
	{
		return -1;
	}

	iUseNum = 1;
	
	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, PET_UNBIND_ITEM_ID, 
				iUseNum, -1, NULL,PACKAGE_DEC_NONE))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, 
			ZONEERR_PET_UNBIND , pstItemDef->Name);
		goto final;
	}

	package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY,iMoney);
	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_PET_UNBIND, 
				pstRolePet->PetID, 0, iMoney, "宠物解绑");

	memset(&stItemDec,0,sizeof(stItemDec));
	stItemDec.ID = PET_UNBIND_ITEM_ID;
	stItemDec.Num = iUseNum;
	stItemDec.BindNum = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, PET_UNBIND_ITEM_ID, 
				iUseNum, -1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_PET_UNBIND);

	z_role_pet_oplog( pstEnv, pstPlayer, pstRolePet, NULL, OP_PET_UNBIND, NULL, &stItemDec, NULL, NULL);

	pstRolePet->LoveLevel = 0;
	
	pet_update_online(pstEnv, pstPlayer, pstRolePet);
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	iSucc = 1;	
final:

	pstPetSvr->PetSvrType = PET_SVR_UNBIND;
	pstPetSvr->PetSvrData.PetUnbindSvr.Succ = iSucc;
	pstPetSvr->PetSvrData.PetUnbindSvr.Wid = pstPetUnbind->PetWid;
	Z_CSHEAD_INIT(&stPkg.Head, PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

