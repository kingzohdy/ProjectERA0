

#include "zone_clan_pet.h"
#include "zone_clan.h"
#include "zone_clt.h"
#include "zone_clan_city.h"
#include "zone_map.h"
#include "zone_pet.h"
#include "zone_package.h"
#include "zone_player.h"
#include "zone_attr.h"
#include "zone_arm.h"
#include "zone_err.h"
#include "zone_mail.h"

#define GAOJI_JIYANG_SEC 1*60*60

int clan_pet_pence_notify(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan);

PETIMPUPATTRDEF *pet_imp_upattr_def_find(ZONESVRENV* pstEnv, int iImp, int iType)
{
	PETIMPUPATTRDEF stDef;

	stDef.Imp = iImp;
	stDef.Type = iType;
	int iIndex,iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	iIndex = bsearch_int(&stDef, pstObj->sPetImpUpAttrDef, pstObj->iPetImpUpAttrDef, PET_IMP_UP_ATTR_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETIMPUPATTRDEF *)pstObj->sPetImpUpAttrDef[iIndex];
	}
	return NULL;
}

PETIMPUPDEF *pet_imp_up_def_find(ZONESVRENV* pstEnv, int iImp)
{
	PETIMPUPDEF stDef;

	stDef.Imp = iImp;
	int iIndex,iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	iIndex = bsearch_int(&stDef, pstObj->sPetImpUpDef, pstObj->iPetImpUpDef, PET_IMP_UP_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETIMPUPDEF *)pstObj->sPetImpUpDef[iIndex];
	}
	return NULL;
}

PETOPAWARDDEF *pet_op_award_def_find(ZONESVRENV* pstEnv, int iType, int iImp)
{
	PETOPAWARDDEF stDef;

	stDef.Type = iType;
	stDef.Imp = iImp;
	int iIndex,iEqu;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	iIndex = bsearch_int(&stDef, pstObj->sPetOpAwardDef, pstObj->iPetOpAwardDef, PET_OP_AWARD_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (PETOPAWARDDEF *)pstObj->sPetOpAwardDef[iIndex];
	}
	return NULL;
}


// 检查栏位变化
// 没必要在升级，降级，创建工会时赋值
int clan_pet_pence_tick(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan)
{
	CLANPET *pstClanPet = &pstZoneClan->stClan.ClanPet;
	CLANLEVELDEF *pstLvDef;
	int iChg = 0;
	int i;
	CLANPETONE *pstOne;
	Player *pstOwner;
	ROLEPET *pstRolePet;
	int iOld = pstClanPet->MaxNum;
	
	pstLvDef = z_clan_level_def(pstEnv, pstZoneClan->stClan.ClanProf.Level);
	if (!pstLvDef)
	{
		return 0;
	}

	if (pstClanPet->MaxNum != pstLvDef->FreeFence+pstLvDef->MoneyFence)
	{
		pstClanPet->MaxNum = pstLvDef->FreeFence+pstLvDef->MoneyFence;
		iChg = 1;
	}

	if (pstClanPet->FreeNum != pstLvDef->FreeFence)
	{
		pstClanPet->FreeNum = pstLvDef->FreeFence;
		pstClanPet->MaxNum = pstLvDef->FreeFence+pstLvDef->MoneyFence;
		iChg = 1;
	}

	// 工会降级导致宠物栏减少
	if (pstClanPet->MaxNum < iOld)
	{
		for (i=pstClanPet->Num-1; i>=0; i--)
		{
			pstOne = &pstClanPet->ClanPets[i];
			if (pstOne->Idx < pstClanPet->MaxNum)
			{
				continue;
			}

			pstOwner = z_name_player(pstEnv->pstAppCtx, pstEnv, pstOne->RoleName);
			if (pstOwner && pstOwner->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				pstRolePet = z_find_role_pet(pstOwner, pstOne->PetWid, NULL);
				if (pstRolePet)
				{
					clan_pet_jiyang_unlock(pstEnv,pstOwner,pstRolePet,pstOne,pstClanPet,pstOwner,pstZoneClan);
				}

				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstOwner, SYS_MIDDLE, ZONEERR_CLAN_PET01);
			}
			else
			{
				clan_pet_jiyang_unlock(pstEnv,NULL,NULL,pstOne,pstClanPet,NULL,pstZoneClan);
				if (pstOne->UseExp > 0)
				{
					char szMsg[256];
					snprintf(szMsg, sizeof(szMsg), LJY_MAIL71,pstOne->RoleName);
					player_sysmail_fast_send(pstEnv, pstOne->RoleName, LJY_MAIL43,szMsg,NULL, 0, 0,LJY_MAIL43, 0);	
				}
			}
		}
	}


	if (iChg)
	{
		// 公会地图广播
		clan_pet_pence_notify( pstEnv, NULL, pstZoneClan);
	}
	return 0;
}


int clan_pet_data_syn(ZONESVRENV* pstEnv, Player *pstPlayer, CLANPETONE *pstOne)
{
	ROLEPET *pstRolePet;
	int iChg = 0;

	pstRolePet = z_find_role_pet(pstPlayer, pstOne->PetWid, NULL);
	if (!pstRolePet)
	{
		return 0;
	}

	if (pstRolePet->ImpExp != pstOne->ImpExp)
	{
		pstRolePet->ImpExp = pstOne->ImpExp;
		iChg = 1;
	}

	if (pstRolePet->WenBao != pstOne->WenBao)
	{
		pstRolePet->WenBao = pstOne->WenBao;
		iChg = 1;
	}

	if (iChg)
	{
		pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);	
	}
	return 0;
}


CLANPETONE *clan_pet_by_player(CLANPET *pstClanPet, Player *pstPlayer)
{
	int i;

	for (i=0; i<pstClanPet->Num; i++)
	{
		if (pstClanPet->ClanPets[i].RoleID == pstPlayer->stRoleData.RoleID)
		{
			return &pstClanPet->ClanPets[i];
		}
	}
	return 0;
}

CLANPETONE *clan_pet_by_idx(CLANPET *pstClanPet, int iIdx)
{
	int i;

	for (i=0; i<pstClanPet->Num; i++)
	{
		if (pstClanPet->ClanPets[i].Idx == iIdx)
		{
			return &pstClanPet->ClanPets[i];
		}
	}
	return 0;
}

CLANPETONE *clan_pet_get_wid(CLANPET *pstClanPet, tdr_ulonglong ullPetWid)
{
	int i;

	for (i=0; i<pstClanPet->Num; i++)
	{
		if (pstClanPet->ClanPets[i].PetWid == ullPetWid)
		{
			return &pstClanPet->ClanPets[i];
		}
	}
	return 0;
}

int clan_pet_all_notify(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan)
{
	CSPKG stPkg;
	CSCLANPETSVR *pstSvr = &stPkg.Body.ClanPetSvr;
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEMAPINST *pstMapInst;
	
	pstSvr->Cmd = CLAN_PET_SVR_OP_ALL;
	pstSvr->Data.All.ClanID = pstClan->stClan.ClanProf.Id;
	pstSvr->Data.All.All =  pstClan->stClan.ClanPet;
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_PET_SVR);

	if (pstPlayer)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		return 0;
	}
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstClan);
	if (NULL == pstZonePworldInst)
	{
		return 0;
	}
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstZonePworldInst->astMapInstIdx[0]);
	if(NULL == pstMapInst)
	{
		return 0;
	}

	map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);
	return 0;
}


int clan_pet_pence_notify(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan)
{
	CSPKG stPkg;
	CSCLANPETSVR *pstSvr = &stPkg.Body.ClanPetSvr;
	ZONEPWORLDINST *pstZonePworldInst;
	ZONEMAPINST *pstMapInst;
	
	pstSvr->Cmd = CLAN_PET_SVR_OP_GRID;
	pstSvr->Data.Grid.FreeNum = pstClan->stClan.ClanPet.FreeNum;
	pstSvr->Data.Grid.MaxNum = pstClan->stClan.ClanPet.MaxNum;
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_PET_SVR);
	
	pstZonePworldInst = z_find_clan_city_inst(pstEnv, pstClan);
	if (NULL == pstZonePworldInst)
	{
		return 0;
	}
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstZonePworldInst->astMapInstIdx[0]);
	if(NULL == pstMapInst)
	{
		return 0;
	}

	map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);
	return 0;
}

int clan_pet_box_notify(ZONESVRENV* pstEnv, Player *pstPlayer, CLANPETONE *pstOne, int iType)
{
	CSPKG stPkg;
	CSCLANPETSVR *pstSvr = &stPkg.Body.ClanPetSvr;
	
	pstSvr->Cmd = CLAN_PET_SVR_OP_BOX;
	pstSvr->Data.Box.Idx = pstOne->Idx;
	pstSvr->Data.Box.Wid = pstOne->PetWid;
	pstSvr->Data.Box.Type = iType;
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}


int clan_pet_gaoji_sec_notify(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSCLANPETSVR *pstSvr = &stPkg.Body.ClanPetSvr;
	
	pstSvr->Cmd = CLAN_PET_SVR_OP_GAOJI_SEC;
	pstSvr->Data.Sec.Sec = GAOJI_JIYANG_SEC - pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.GaojiSec;
	if (pstSvr->Data.Sec.Sec < 0)
	{
		pstSvr->Data.Sec.Sec = 0;
	}
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_PET_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int clan_pet_clear_gaoji_sec(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.GaojiSec = 0;
	clan_pet_gaoji_sec_notify( pstEnv, pstPlayer);
	return 0;
}

int clan_pet_one_notify(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlag, CLANPETONE *pstOne, ZoneClan *pstZoneClan)
{
	CSPKG stPkg;
	CSCLANPETSVR *pstSvr = &stPkg.Body.ClanPetSvr;
	
	pstSvr->Cmd = CLAN_PET_SVR_OP_ONE;
	pstSvr->Data.One.One = *pstOne;
	
	Z_CSHEAD_INIT(&stPkg.Head, CLAN_PET_SVR);

	if (!iFlag)
	{
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	}
	else
	{
		ZONEMAPINST *pstMapInst;
		ZONEPWORLDINST* pstPworldInst;

		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj,pstZoneClan->stZoneClanInfo.iCityPworldInstID);
		if (!pstPworldInst)
		{
			return 0;
		}

		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstPworldInst->astMapInstIdx[0]);
		if(NULL == pstMapInst)
		{
			return 0;
		}
		map_broadcast(pstEnv->pstAppCtx, pstEnv, pstMapInst, &stPkg);
	}
	return 0;
}


int clan_pet_jiyang(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETJIYANG *pstJiYang)
{
	ZoneClan *pstClan;
	CLANPET *pstClanPet;
	ROLEPET *pstRolePet;
	PETIMPUPDEF *pstUpDef;
	int iUseExp = 0;
	CLANPETONE *pstOne;
	int iUseMoney = 0;
	int iUseContri = 0;
	int iFlag = 0;
	
	pstRolePet = z_find_role_pet(pstPlayer, pstJiYang->WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	if (pstRolePet->Imp >= PET_MAX_IMP)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET02);
		return -1;
	}

	if (PET_STAT_FIGHT & pstRolePet->StatFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET03);
		return -1;
	}
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}
	
	if (!is_in_my_clan_city(pstEnv,pstPlayer))
	{
		return -1;
	}

	pstClanPet = &pstClan->stClan.ClanPet;
	pstOne = clan_pet_by_idx(pstClanPet, pstJiYang->Idx);
	if (pstOne)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET04);
		return -1;
	}
	
	if (clan_pet_by_player(pstClanPet, pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET05);
		return -1;
	}
	
	if (pstClanPet->Num >= pstClanPet->MaxNum)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET06);
		return -1;
	}

	if (pstJiYang->Idx >= pstClanPet->MaxNum)
	{
		return -1;
	}

	if (pstJiYang->Hour!=1&&pstJiYang->Hour!= 2 && pstJiYang->Hour != 4 && pstJiYang->Hour!=6)
	{
		return -1;
	}

	pstUpDef = pet_imp_up_def_find( pstEnv, pstRolePet->Imp);
	if (!pstUpDef)
	{
		return -1;
	}

	if (pstJiYang->Idx < pstClanPet->FreeNum)
	{
		iUseContri = pstJiYang->Hour*pstUpDef->UseContri;
		if (0 > player_contribute_dec(pstEnv,pstPlayer,iUseContri,1))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET07);
			return -1;
		}
	}
	else
	{
		iUseMoney = pstJiYang->Hour*pstUpDef->UseMoney;
		if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY, iUseMoney) < 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET08);
			return -1;
		}
	}

	if (pstJiYang->Flag)
	{
		if (pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.GaojiSec >= GAOJI_JIYANG_SEC)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET09);
			return -1;
		}

		iFlag |=JIYANG_GAOJI;
	}
	else
	{
		iUseExp = pstUpDef->UseExp*20*pstJiYang->Hour;
		if (iUseExp <= 0)
		{
			return -1;
		}

		if (pstPlayer->stRoleData.RoleDetail.Exp < iUseExp)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET10);
			return -1;
		}

		player_dec_exp(pstEnv, pstPlayer, iUseExp);
	}

	
	if (pstJiYang->Idx < pstClanPet->FreeNum)
	{
		player_contribute_dec(pstEnv,pstPlayer,iUseContri,0);
	}
	else
	{
		package_dec_money(pstEnv, pstPlayer,ATTR_ID_MONEY, iUseMoney) ;
	}
	
	memset(&pstClanPet->ClanPets[pstClanPet->Num], 0, sizeof(pstClanPet->ClanPets[pstClanPet->Num]));
	pstClanPet->ClanPets[pstClanPet->Num].Idx = pstJiYang->Idx;
	pstClanPet->ClanPets[pstClanPet->Num].ImpExp = pstRolePet->ImpExp;
	pstClanPet->ClanPets[pstClanPet->Num].JiYangEnd = pstEnv->pstAppCtx->stCurr.tv_sec+pstJiYang->Hour*3600;
	pstClanPet->ClanPets[pstClanPet->Num].PetID = pstRolePet->PetID;
	pstClanPet->ClanPets[pstClanPet->Num].PetWid = pstRolePet->WID;
	pstClanPet->ClanPets[pstClanPet->Num].RoleID = pstPlayer->stRoleData.RoleID;
	pstClanPet->ClanPets[pstClanPet->Num].UseExp = iUseExp;
	pstClanPet->ClanPets[pstClanPet->Num].Imp = pstRolePet->Imp;
	pstClanPet->ClanPets[pstClanPet->Num].WenBao = pstRolePet->WenBao;
	pstClanPet->ClanPets[pstClanPet->Num].JiYangStart = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstClanPet->ClanPets[pstClanPet->Num].UseTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	//pstClanPet->ClanPets[pstClanPet->Num].ClanWid = pstClan->stClan.ClanProf.GID;
	strncpy(pstClanPet->ClanPets[pstClanPet->Num].RoleName, pstPlayer->stRoleData.RoleName, 
			sizeof(pstClanPet->ClanPets[pstClanPet->Num].RoleName));
	pstClanPet->ClanPets[pstClanPet->Num].JiyangFlag |= iFlag;
	
	pstClanPet->Num++;

	pstRolePet->MiscFlag |= PET_MISC_FLAG_JIYANG;
	pet_update_to_clt(pstEnv, pstPlayer, pstRolePet);

	// 公会地图广播一下 todo:
	clan_pet_one_notify(pstEnv, pstPlayer,1, &pstClanPet->ClanPets[pstClanPet->Num-1],pstClan);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET11, pstRolePet->Name);

	player_clan_msg_broadcast(pstEnv, pstClan, SYS_CLAN,
	                          ZONEERR_CLANPET1,pstPlayer->stRoleData.RoleName, pstRolePet->Name);
	return 0;
}

// 根据地图获取公会
int clan_get_by_map(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZONEPWORLDINST *pstPInst;
	ZONEMAPINST*pstMInst;
	ZONEIDX stMapIdx;

	stMapIdx.iID = pstPlayer->stRoleData.Map;
	stMapIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stMapIdx);
	if(NULL == pstMInst ||pstMInst->iPworldInstID <= 0)
	{
		return 0;
	}

	pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMInst->iPworldInstID);
	if (NULL == pstPInst ||pstPInst->iType != PWORLD_TYPE_CLAN)
	{
		return 0;
	}
	
	return pstPInst->stPworldTypeData.stClan.iID;
}


int clan_pet_op_fill(ZONESVRENV* pstEnv, Player *pstPlayer, int iVal, int iOp, CLANPETONE*pstClanPetOne)
{
	CLANPETOP *pstOp;
	
	if (pstClanPetOne->OpNum >= CLAN_PET_MAX_OP)
	{
		memmove(&pstClanPetOne->OpInfo[0], &pstClanPetOne->OpInfo[1], sizeof(pstClanPetOne->OpInfo[0])*(CLAN_PET_MAX_OP-1));
		pstClanPetOne->OpNum --;
	}

	pstOp = &pstClanPetOne->OpInfo[pstClanPetOne->OpNum];
	pstOp->OpType = iOp;
	pstOp->Val = iVal;
	pstOp->Time = pstEnv->pstAppCtx->stCurr.tv_sec;
	strncpy(pstOp->RoleName, pstPlayer->stRoleData.RoleName, sizeof(pstOp->RoleName));

	pstClanPetOne->OpNum ++;
	return 0;
}

int clan_pet_player_chg_name(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszNewName)
{
	ZoneClan *pstClan;
	CLANPET *pstClanPet;
	int i;


	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return 0;
	}

	pstClanPet = &pstClan->stClan.ClanPet;

	for (i=0; i<pstClanPet->Num; i++)
	{
		if (strcmp(pstClanPet->ClanPets[i].RoleName, pstPlayer->stRoleData.RoleName) == 0)
		{
			strncpy(pstClanPet->ClanPets[i].RoleName, pszNewName, 
				sizeof(pstClanPet->ClanPets[i].RoleName));
			clan_pet_one_notify(pstEnv, pstPlayer, 0, &pstClanPet->ClanPets[i],pstClan);
			break;
		}
	}
	
	return 0;
}

int clan_pet_wanshua(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETWANSHUA*pstWanshua)
{
	CLANPETONE *pstClanPetOne;
	CLANPET *pstClanPet;
	ZoneClan *pstClan;
	ROLEGRID *pstGrid;
	ITEMDEF *pstItemDef;
	int iHappy = 0;
	int iBoxType;
	int iPos;
	PETOPAWARDDEF *pstOpDef;
	int iOldHappy;
	Player *pstOwner;
	ROLEPET *pstRolePet;
	
	pstClan = z_id_clan(pstEnv, clan_get_by_map(pstEnv, pstPlayer));
	if (!pstClan)
	{
		return -1;
	}
	
	pstClanPet = &pstClan->stClan.ClanPet;
	pstClanPetOne = clan_pet_get_wid(pstClanPet, pstWanshua->WID);
	if (!pstClanPetOne || pstWanshua->Idx != pstClanPetOne->Idx)
	{
		return -1;
	}

	pstOwner = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClanPetOne->RoleName);
	if (!pstOwner)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET12);
		return -1;
	}

	pstRolePet =  z_find_role_pet(pstOwner, pstWanshua->WID, NULL);
	if (!pstRolePet)
	{	
		return -1;
	}

	if (pstClanPetOne->JiYangEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	if (pstClanPetOne->HappyVal >= PET_MAX_HAPPY)
	{
		return -1;
	}

	iOldHappy = pstClanPetOne->HappyVal;

	if (package_get_freegrid_num(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SUNDRIES) < 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET13);
		return -1;
	}

	if (pstWanshua->Type == 1)
	{
		pstGrid = get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstWanshua->GridIdx);
		if (!pstGrid )
		{
			return -1;
		}

		pstItemDef = z_find_itemdef( pstEnv, pstGrid->GridData.RoleItem.DefID);
		if (!pstItemDef || pstItemDef->Result[0].ResultID != RESULT_PET_WANJU)
		{
			return -1;	
		}
		iHappy = pstItemDef->Result[0].ResultVal1;
		iBoxType = 1;
		iPos = pstGrid - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];
	}
	else
	{
		if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY, PET_WANSHUA_USE_MONEY) < 0)
		{
			return -1;
		}

		iHappy = PET_USE_WANJU_ADD;
		iBoxType = 2;
	}

	pstOpDef = pet_op_award_def_find( pstEnv, iBoxType, 0);
	if (!pstOpDef)
	{
		return -1;
	}

	pstClanPetOne->HappyVal += iHappy;
	if (pstClanPetOne->HappyVal > PET_MAX_HAPPY)
		pstClanPetOne->HappyVal  = PET_MAX_HAPPY;
	
	clan_pet_op_fill(pstEnv, pstPlayer, pstClanPetOne->HappyVal-iOldHappy,2, pstClanPetOne);
	clan_pet_one_notify(pstEnv, pstPlayer,1, pstClanPetOne,pstClan);

	if (pstWanshua->Type == 1)
	{
		package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, LIST_TYPE_SUNDRIES, &iPos, 0, 0);
	}
	else
	{
		package_dec_money(pstEnv, pstPlayer,ATTR_ID_MONEY, PET_WANSHUA_USE_MONEY);
	}

	if (pstWanshua->Type != 1)
	{
		package_add_by_id(pstEnv, pstPlayer,pstOpDef->ItemID, 1, 1);
		clan_pet_box_notify( pstEnv, pstPlayer, pstClanPetOne, iBoxType);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET14, pstRolePet->Name);
	}

	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET15, pstClanPetOne->HappyVal-iOldHappy);
	return 0;
}

int clan_pet_jiyang_unlock(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, 
								CLANPETONE *pstOne, CLANPET *pstClanPet,Player *pstOwner, ZoneClan *pstClan)
{
	CLANPETONE stOne;
	int iIdx;
	
	// 宠物变化通知
	if (pstRolePet && pstOwner)
	{
		pstRolePet->WenBao = pstOne->WenBao;
		pstRolePet->ImpExp = pstOne->ImpExp;
		pstRolePet->MiscFlag &= ~PET_MISC_FLAG_JIYANG;
		pet_update_to_clt(pstEnv, pstOwner, pstRolePet);
	}
	

	// 单个宠物信息通知客户端，petid为0表示删除这只宠物
	memset(&stOne, 0, sizeof(stOne));
	stOne.Idx = pstOne->Idx;
	stOne.PetID = 0;

	if (pstOne->UseExp > 0 && pstOwner)
	{
		player_add_exp(pstEnv, pstOwner, pstOne->UseExp,0);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstOwner, SYS_MIDDLE, ZONEERR_CLAN_PET16, pstOne->UseExp);
	}

	iIdx = pstOne - pstClanPet->ClanPets;
	pstClanPet->Num --;
	if (iIdx != pstClanPet->Num )
	{
		pstClanPet->ClanPets[iIdx] = pstClanPet->ClanPets[pstClanPet->Num] ;
	}

	// 公会地图广播一下 todo:
	clan_pet_one_notify(pstEnv, pstPlayer,1, &stOne,pstClan);
	return 0;
}

int clan_pet_jiyang_end(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETJIYANGEND*pstEnd)
{
	ROLEPET *pstRolePet;
	CLANPETONE *pstOne;
	ZoneClan *pstClan;
	CLANPET *pstClanPet;

	pstRolePet = z_find_role_pet(pstPlayer, pstEnd->WID, NULL);
	if (!pstRolePet || !(pstRolePet->MiscFlag &PET_MISC_FLAG_JIYANG))
	{
		return -1;
	}

	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}
	
	if (!is_in_my_clan_city(pstEnv,pstPlayer))
	{
		return -1;
	}

	pstClanPet = &pstClan->stClan.ClanPet;
	
	pstOne = clan_pet_by_player(pstClanPet, pstPlayer);
	if (!pstOne || pstOne->PetWid != pstRolePet->WID || pstOne->Idx != pstEnd->Idx)
	{
		return -1;
	}

	
	clan_pet_jiyang_unlock(pstEnv,pstPlayer,pstRolePet,pstOne,pstClanPet,pstPlayer,pstClan);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET17);
	return 0;
}


int clan_pet_work_award(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETWORKAWARD*pstAward)
{
	ZoneClan *pstClan;
	CLANPET *pstClanPet;
	ROLEPET *pstRolePet;
	CLANPETONE *pstClanPetOne;
	PETOPAWARDDEF *pstOpDef;
	int iRate;

	pstRolePet = z_find_role_pet(pstPlayer, pstAward->WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}
	
	if (!is_in_my_clan_city(pstEnv,pstPlayer))
	{
		return -1;
	}

	pstClanPet = &pstClan->stClan.ClanPet;
	if (!(pstClanPetOne = clan_pet_by_player(pstClanPet, pstPlayer)))
	{
		return -1;
	}

	if (pstClanPetOne->PetWid != pstAward->WID)
	{
		return -1;
	}

	if (!pstClanPetOne->WorkFlag)
	{
		return -1;
	}

	if (	pstClanPetOne->WorkEnd >0 &&
		pstEnv->pstAppCtx->stCurr.tv_sec < pstClanPetOne->WorkEnd)
	{
		return -1;
	}

	pstOpDef = pet_op_award_def_find( pstEnv, 3, 0);
	if (!pstOpDef)
	{
		return -1;
	}

	if (package_get_freegrid_num(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SUNDRIES) < 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET13);
		return -1;
	}

	iRate = RAND1(10000);
	if (iRate < pstOpDef->Rate)
	{
		package_add_by_id(pstEnv, pstPlayer,pstOpDef->ItemID, 1, 1);
		clan_pet_box_notify( pstEnv, pstPlayer, pstClanPetOne, 3);	
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET18);
	}

	pstClanPetOne->WorkFlag = 0;
	pstClanPetOne->WorkEnd = 0;
	// 通知自己就够了
	clan_pet_one_notify(pstEnv, pstPlayer,0, pstClanPetOne,pstClan);
	return 0;
}

int clan_pet_work(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETWORK*pstWork)
{
	ZoneClan *pstClan;
	CLANPET *pstClanPet;
	ROLEPET *pstRolePet;
	CLANPETONE *pstClanPetOne;

	pstRolePet = z_find_role_pet(pstPlayer, pstWork->WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}
	
	if (!is_in_my_clan_city(pstEnv,pstPlayer))
	{
		return -1;
	}

	pstClanPet = &pstClan->stClan.ClanPet;
	if (!(pstClanPetOne = clan_pet_by_player(pstClanPet, pstPlayer)))
	{
		return -1;
	}

	if (pstClanPetOne->PetWid != pstWork->WID)
	{
		return -1;
	}

	if (pstClanPetOne->JiYangEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	if (pstClanPetOne->HappyVal < PET_WORK_HAPPY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET19);
		return -1;
	}

	if (pstClanPetOne->JiYangEnd-pstEnv->pstAppCtx->stCurr.tv_sec< PET_WORK_TIME+10)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET20);
		return -1;
	}

	// 可以领取了
	if (pstClanPetOne->WorkFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET21);
		return -1;
	}

	// 打工还没结束
	if (	pstClanPetOne->WorkEnd >0 &&
		pstEnv->pstAppCtx->stCurr.tv_sec  < pstClanPetOne->WorkEnd)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET22);
		return -1;
	}

	pstClanPetOne->HappyVal -= PET_WORK_HAPPY;
	pstClanPetOne->WorkEnd = pstEnv->pstAppCtx->stCurr.tv_sec+PET_WORK_TIME;

	clan_pet_one_notify(pstEnv, pstPlayer,1, pstClanPetOne,pstClan);
	return 0;
}


int clan_pet_player_enter_city(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iClanID;
	ZoneClan *pstClan;
	
	iClanID = clan_get_by_map(pstEnv, pstPlayer);
	if (iClanID <= 0)
		return -1;

	pstClan = z_id_clan(pstEnv, iClanID);
	if (!pstClan)
	{
		return -1;
	}

	clan_pet_all_notify(pstEnv, pstPlayer, pstClan);
	return 0;
}


int clan_pet_info_notify(ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (!IsSameDay(iCurr, pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.Time))
	{
		pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.Time = iCurr;
		pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.TouNum = 0;
	}
	
	CSPKG stPkg;
	CSPLAYERMISCSVR *pstMiscSvr = &stPkg.Body.PlayerMiscSvr;
	
	pstMiscSvr->PlayerMiscType = PLAYER_CLAN_PET_NOTIFY;
	pstMiscSvr->PlayerMiscSvrData.ClanPetNotify.Notify = pstPlayer->stRoleData.MiscInfo.PetJiYangInfo;
	
	Z_CSHEAD_INIT(&stPkg.Head, PLAYER_MISC_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	return 0;
}

int clan_pet_tou(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETTOU*pstTou)
{
	CLANPETONE *pstClanPetOne;
	CLANPET *pstClanPet;
	ZoneClan *pstClan;
	int iClanID;
	ROLEPET *pstRolePet;
	int i;
	int iRate;
	PETOPAWARDDEF *pstOpDef;
	int iBoxType = 4;
	int iUseMoney = 0;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	PETIMPUPDEF *pstUpDef;
	int iOldExp;
	int iBeishu = 1;
	Player *pstOwner;


	iClanID = clan_get_by_map(pstEnv, pstPlayer);
	if (iClanID <= 0)
		return -1;

	pstClan = z_id_clan(pstEnv, iClanID);
	if (!pstClan)
	{
		return -1;
	}
	
	pstClanPet = &pstClan->stClan.ClanPet;
	pstClanPetOne = clan_pet_get_wid(pstClanPet, pstTou->WID);
	if (!pstClanPetOne || pstTou->Idx != pstClanPetOne->Idx)
	{
		return -1;
	}

	pstOwner = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClanPetOne->RoleName);
	if (!pstOwner)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET12);
		return -1;
	}

	if (pstOwner == pstPlayer)
	{
		return -1;
	}

	pstRolePet = z_find_role_pet(pstOwner, pstTou->WID, NULL);
	if (!pstRolePet)
	{
		return -1;
	}

	// 寄养没满1小时
	if (pstClanPetOne->JiYangStart+3600 > iCurr)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET23);
		return -1;
	}

	if (pstClanPetOne->TouNum >= 5)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET24);
		return -1;
	}

	for (i=0; i<pstClanPetOne->TouNum; i++)
	{
		if (pstClanPetOne->Tou[i].RoleID == pstPlayer->stRoleData.RoleID)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET25);
			return -1;
		}
	}

	if (pstClanPetOne->HappyVal < 50)
	{
		iBoxType = 5;  // 奖励类型不同	
		iBeishu = 2;	// 惩罚翻倍
	}

	pstOpDef = pet_op_award_def_find( pstEnv, iBoxType, pstRolePet->Imp);
	if (!pstOpDef)
	{
		return -1;
	}

	if (!IsSameDay(iCurr, pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.Time))
	{
		pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.Time = iCurr;
		pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.TouNum = 0;
	}
	iUseMoney = pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.TouNum*100+100;

	if (package_dec_money_test(pstEnv, pstPlayer,ATTR_ID_MONEY, iUseMoney) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET26);
		return -1;
	}

	if (package_get_freegrid_num(pstEnv->pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SUNDRIES) < 1)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET13);
		return -1;
	}

	pstUpDef = pet_imp_up_def_find( pstEnv, pstRolePet->Imp);
	if (!pstUpDef)
	{
		return -1;
	}

	if (pstClanPetOne->WenBao < 30)
	{
		return -1;
	}
	else if (pstClanPetOne->WenBao < 60)
	{
		iRate = 100;
	}
	else if (pstClanPetOne->WenBao < 80)
	{
		iRate = 75;
	}
	else
	{
		iRate = 50;
	}

	package_dec_money(pstEnv, pstPlayer,ATTR_ID_MONEY, iUseMoney);
	if (RAND1(100) < iRate)
	{
		package_add_by_id(pstEnv, pstPlayer,pstOpDef->ItemID, 1, 1);
		clan_pet_box_notify( pstEnv, pstPlayer, pstClanPetOne, iBoxType);

		iOldExp = pstClanPetOne->ImpExp;
		pstClanPetOne->ImpExp-=pstUpDef->TouDecExp*iBeishu;
		if (pstClanPetOne->ImpExp < 0)
		{
			pstClanPetOne->ImpExp = 0;	
		}
		
		pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.TouNum++;
		clan_pet_info_notify( pstEnv, pstPlayer);

		pstClanPetOne->Tou[pstClanPetOne->TouNum].RoleID = pstPlayer->stRoleData.RoleID;
		pstClanPetOne->TouNum++;
		clan_pet_op_fill(pstEnv, pstPlayer,iOldExp-pstClanPetOne->ImpExp, 3,pstClanPetOne);
		clan_pet_one_notify(pstEnv, pstPlayer,1, pstClanPetOne,pstClan);

		if (iBoxType==4)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET27, iUseMoney);
		}
		else if(iBoxType==5)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET28, iUseMoney);
		}
		clan_pet_data_syn(pstEnv, pstOwner, pstClanPetOne);
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET29, iUseMoney);
	}

	return 0;
}

int clan_pet_eat(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETEAT *pstEat)
{
	CLANPETONE *pstClanPetOne;
	CLANPET *pstClanPet;
	ZoneClan *pstClan;
	ROLEGRID *pstGrid;
	ITEMDEF *pstItemDef;
	int iPos = 0;
	int iOldWenbao;
	int iClanID;
	Player *pstOwner;

	iClanID = clan_get_by_map(pstEnv, pstPlayer);
	if (iClanID <= 0)
		return -1;

	pstClan = z_id_clan(pstEnv, iClanID);
	if (!pstClan)
	{
		return -1;
	}
		
	pstClanPet = &pstClan->stClan.ClanPet;
	pstClanPetOne = clan_pet_get_wid(pstClanPet, pstEat->WID);
	if (!pstClanPetOne || pstEat->Idx != pstClanPetOne->Idx)
	{
		return -1;
	}

	pstOwner = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClanPetOne->RoleName);
	if (!pstOwner)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET12);
		return -1;
	}

	if (pstClanPetOne->JiYangEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	if (pstClanPetOne->WenBao >= PET_MAX_WENBAO)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET30);
		return -1;
	}

	iOldWenbao = pstClanPetOne->WenBao;
	pstGrid = get_pak_item_grid(pstPlayer, LIST_TYPE_SUNDRIES, pstEat->GridIdx);
	if (!pstGrid )
	{
		return -1;
	}

	pstItemDef = z_find_itemdef( pstEnv, pstGrid->GridData.RoleItem.DefID);
	if (!pstItemDef || pstItemDef->Result[0].ResultID != RESULT_PET_SILIAO)
	{
		return -1;	
	}

	if (pstItemDef->Result[0].ResultVal2 > 0 && pstItemDef->Result[0].ResultVal2 < pstClanPetOne->Imp)
	{
		return -1;
	}

	iPos = pstGrid - &pstPlayer->stRoleData.Package.SundriesPackage.RoleGrids[0];
	package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, 1, LIST_TYPE_SUNDRIES, &iPos, 0, 0);

	pstClanPetOne->WenBao+=pstItemDef->Result[0].ResultVal1;
	if (pstClanPetOne->WenBao >= PET_MAX_WENBAO)
	{
		pstClanPetOne->WenBao = PET_MAX_WENBAO;
	}

	clan_pet_op_fill(pstEnv, pstPlayer, pstClanPetOne->WenBao-iOldWenbao,1, pstClanPetOne);
	clan_pet_one_notify(pstEnv, pstPlayer,1, pstClanPetOne,pstClan);
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET31,pstItemDef->Name);

	clan_pet_data_syn(pstEnv, pstOwner, pstClanPetOne);
	return 0;
}


int clan_pet_calc_exp(ZONESVRENV* pstEnv,ROLEPET *pstRolePet, CLANPETONE *pstOne)
{
	float f = 1.0;
	PETDEF *pstPetDef;
	int iExp = 0;

	pstPetDef = z_find_pet_def(pstEnv,pstRolePet->PetID, NULL);
	if (!pstPetDef)
		return 0;
	
	if (pstRolePet->MiscFlag&PET_MISC_FLAG_COMBAT)
	{
		f+=0.2;
	}

	if (pstPetDef->Type == PET_TYPE_ABERRANCE)
	{
		f+=0.3;
	}

	if (pstOne->WenBao == 0)
	{
		return 0;
	}
	
	if (pstOne->JiyangFlag & JIYANG_GAOJI)
	{
		if (pstOne->WenBao >= 80)
		{
			iExp = 450*f;
		}
		else
		{
			iExp = 300*f;
		}
	}
	else
	{
		iExp = f*10;
		if (pstOne->WenBao<30)
		{
			iExp*=0.5;	
		}
		else if (pstOne->WenBao<60)
		{
			iExp*=0.8;	
		}
		else if (pstOne->WenBao<80)
		{
			iExp*=1;
		}
		else
		{
			iExp*=1.5;
		}
	}

	return iExp;
}

int clan_pet_tick(ZONESVRENV* pstEnv,ZoneClan *pstClan)
{
	CLANPET *pstClanPet = &pstClan->stClan.ClanPet;
	int i;
	CLANPETONE *pstOne;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	Player *pstPlayer;
	ROLEPET *pstRolePet;
	PETIMPUPDEF *pstUpDef;
	
	clan_pet_pence_tick(pstEnv, pstClan);

	if (pstClanPet->Num <= 0)
	{
		return 0;
	}

	for (i=0; i<pstClanPet->Num; i++)
	{
		pstOne = &pstClanPet->ClanPets[i];
		if (pstOne->WorkEnd && pstOne->WorkEnd < iCurr)
		{
			pstOne->WorkFlag = 1;
		}
		
		if (iCurr-pstOne->UseTime < 180)
		{
			continue;
		}

		pstOne->UseTime = iCurr;

		if (pstOne->WenBao <= 0)
		{
			continue;
		}

		if (iCurr > pstOne->JiYangEnd+10)
		{
			continue;
		}

		if (!(JIYANG_GAOJI&pstOne->JiyangFlag))
		{
			if (pstOne->UseExp <= 0)
			{
				continue;
			}
		}
		

		pstPlayer = z_name_player(pstEnv->pstAppCtx,  pstEnv, pstOne->RoleName);
		if (!pstPlayer ||PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus)
		{
			continue;
		}

		pstRolePet = z_find_role_pet(pstPlayer, pstOne->PetWid, NULL);
		if (!pstRolePet)
		{
			continue;
		}

		pstUpDef = pet_imp_up_def_find( pstEnv, pstRolePet->Imp);
		if (!pstUpDef)
		{
			continue;
		}

		pstOne->WenBao --;
		if (!(JIYANG_GAOJI&pstOne->JiyangFlag))
		{
			pstOne->UseExp -= pstUpDef->UseExp;
		}
		
		pstOne->ImpExp +=clan_pet_calc_exp(pstEnv,pstRolePet,pstOne);
		clan_pet_one_notify(pstEnv, pstPlayer,1, pstOne,pstClan);
		clan_pet_data_syn(pstEnv, pstPlayer, pstOne);

		if (pstOne->JiyangFlag & JIYANG_GAOJI)
		{
			pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.GaojiSec += 180;
			if (pstPlayer->stRoleData.MiscInfo.PetJiYangInfo.GaojiSec >= GAOJI_JIYANG_SEC)
			{
				pstOne->JiyangFlag &= ~JIYANG_GAOJI;
			}

			clan_pet_gaoji_sec_notify(pstEnv, pstPlayer);
		}
	}
	return 0;
}


int pet_is_jiyang(ROLEPET *pstRolePet)
{
	return pstRolePet->MiscFlag&PET_MISC_FLAG_JIYANG;
}

int pet_jiyang_limit(ZONESVRENV* pstEnv, Player *pstPlayer, tdr_ulonglong PetWid)
{
	ROLEPET *pstRolePet;

	pstRolePet = z_find_role_pet(pstPlayer,PetWid, NULL);
	if (!pstRolePet)
	{
		return -1;
	}
	
	if (pet_is_jiyang(pstRolePet))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET32);
		return -1;
	}
	return 0;
}

int clan_pet_clear(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANPETCLEAR*pstClear)
{
	ZoneClan *pstClan;
	CLANPET *pstClanPet;
	CLANPETONE *pstClanPetOne;
	int iPosition;
	Player *pstOwner;
	
	pstClan = player_get_clan(pstEnv, pstPlayer);
	if (!pstClan)
	{
		return -1;
	}
	
	if (!is_in_my_clan_city(pstEnv,pstPlayer))
	{
		return -1;
	}

	pstClanPet = &pstClan->stClan.ClanPet;
	pstClanPetOne = clan_pet_get_wid(pstClanPet, pstClear->WID);
 	if (!pstClanPetOne || pstClanPetOne->Idx != pstClear->Idx)
 	{
		return -1;
	}

	if (pstClanPetOne->JiYangEnd > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET33);
		return -1;
	}

	iPosition = player_clan_position(pstEnv, pstPlayer);
	if (	iPosition!= CLAN_POS_VICE_PRESIDENT && 
		iPosition!= CLAN_POS_PRESIDENT )
	{
		//return -1;
	}

	pstOwner = z_name_player(pstEnv->pstAppCtx, pstEnv, pstClanPetOne->RoleName);
	if (pstOwner && pstOwner->stRoleData.RoleID != pstPlayer->stRoleData.RoleID)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET34);
		return -1;
	}
	
	clan_pet_jiyang_unlock(pstEnv,pstPlayer,NULL, pstClanPetOne,pstClanPet,NULL,pstClan);	
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_CLAN_PET35);
	return 0;
}


int clan_pet_player_leave(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan)
{
	int i;
	PETPACKAGE *pstPetPkg = &pstPlayer->stRoleData.Package.PetPackage;
	CLANPETONE *pstOne;
	CLANPET *pstClanPet ;

	if (!pstClan)
		return 0;

	pstClanPet = &pstClan->stClan.ClanPet;

	pstOne = clan_pet_by_player(pstClanPet, pstPlayer);
	if (!pstOne)
	{
		return 0;
	}

	for (i=0; i<pstPetPkg->Num; i++)
	{
		if (pstPetPkg->RolePets[i].MiscFlag&PET_MISC_FLAG_JIYANG)
		{
			clan_pet_jiyang_unlock(pstEnv,pstPlayer,&pstPetPkg->RolePets[i],pstOne,pstClanPet,pstPlayer,pstClan);
			break;
		}
	}

	return 0;
}

int clan_pet_player_login_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iNotify)
{
	int i;
	PETPACKAGE *pstPetPkg = &pstPlayer->stRoleData.Package.PetPackage;
	ZoneClan *pstClan;
	pstClan = player_get_clan(pstEnv, pstPlayer);
	CLANPET *pstClanPet;
	CLANPETONE *pstOne;

	for (i=0; i<pstPetPkg->Num; i++)
	{
		if (pstPetPkg->RolePets[i].MiscFlag&PET_MISC_FLAG_JIYANG)
		{
			if (!pstClan)
			{
				pstPetPkg->RolePets[i].MiscFlag &=~PET_MISC_FLAG_JIYANG;
				if (iNotify)
				{
					pet_update_to_clt(pstEnv, pstPlayer, &pstPetPkg->RolePets[i]);
				}
				continue;
			}

			pstClanPet = &pstClan->stClan.ClanPet;
			pstOne = clan_pet_get_wid(pstClanPet, pstPetPkg->RolePets[i].WID);
			if (!pstOne)
			{
				pstPetPkg->RolePets[i].MiscFlag &=~PET_MISC_FLAG_JIYANG;
				if (iNotify)
				{
					pet_update_to_clt(pstEnv, pstPlayer, &pstPetPkg->RolePets[i]);
				}
			}
		}
	}
	return 0;
}

int clan_pet_op(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
					TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
	{
		return 0;
	}

	if (pstPlayer->stOnline.State&CS_STAT_DEAD)
	{
		return -1;
	}

	CSCLANPETCLT *pstClt = &pstCsPkg->Body.ClanPetClt;
		
	switch(pstClt->Cmd)
	{
	case CLAN_PET_CLT_OP_JIYANG:
		return clan_pet_jiyang(pstEnv, pstPlayer, &pstClt->Data.JiYang);
		break;
	case CLAN_PET_CLT_OP_JIYANG_END:
		return clan_pet_jiyang_end(pstEnv, pstPlayer, &pstClt->Data.JiYangEnd);
		break;
	case CLAN_PET_CLT_OP_WANSHUA:
		return  clan_pet_wanshua(pstEnv, pstPlayer, &pstClt->Data.Wanshua);
		break;
	case CLAN_PET_CLT_OP_TOU:
		return clan_pet_tou( pstEnv, pstPlayer, &pstClt->Data.Tou);
		break;
	case CLAN_PET_CLT_OP_EAT:
		return clan_pet_eat(pstEnv, pstPlayer, &pstClt->Data.Eat);
		break;
	case CLAN_PET_CLT_OP_WORK:
		return clan_pet_work(pstEnv, pstPlayer, &pstClt->Data.Work);
		break;
	case CLAN_PET_CLT_OP_WORK_AWARD:
		return clan_pet_work_award(pstEnv, pstPlayer, &pstClt->Data.WorkAward);
		break;
	case CLAN_PET_CLT_OP_CLEAR:
		return clan_pet_clear(pstEnv, pstPlayer,&pstClt->Data.Clear);
		break;
	default:

	break;
	}
	return 0;
}

