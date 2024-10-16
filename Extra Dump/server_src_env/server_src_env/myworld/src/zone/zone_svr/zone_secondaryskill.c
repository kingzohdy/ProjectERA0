/*
**  @file $RCSfile: zone_secondaryskill.c,v $
**  general description of this module
**  $Id: zone_secondaryskill.c,v 1.61 2013/05/17 06:27:04 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2013/05/17 06:27:04 $
**  @version $Revision: 1.61 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

//�䷽����λ��ʾÿ������ܿ�����0~255���䷽

#include "zone_secondaryskill.h"
#include "zone_package.h"
#include "zone_clt.h"
#include "zone_move.h"
#include "zone_attr.h"
#include "zone_svr.h"
#include "zone_player.h"
#include "stdlib.h"
#include "zone_oplog.h"
#include "zone_task.h"
#include "zone_err.h"

int player_auto_learn_recipe(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, int iRecipeID);

int player_recipe_pre_check(ZONESVRENV* pstEnv, Player *pstPlayer, RECIPEDEF *pstRecipeDef);

int is_collect_recipe(RECIPEDEF * pstRecipeDef);

int is_collect_secondaryskill(int iSkillType);

int z_get_skill_recipe(SECONDARYSKILLINFO *pstSkillInfo, int iRecipeID)
{
	char cFlag;
	
	if(iRecipeID >= MAX_SECONDARY_SKILL_RECIPE*8)
	{
		return 0;
	}
	
	cFlag	=	1 << (iRecipeID%8); 
	
	if(pstSkillInfo->RecipeInfo[iRecipeID/8] & cFlag)
	{
		return 1;
	}

	return 0;
}

int z_set_skill_recipe(SECONDARYSKILLINFO *pstSkillInfo, int iRecipeID, int iVal)
{
	char cFlag;
	
	if(iRecipeID >= MAX_SECONDARY_SKILL_RECIPE*8)
	{
		return -1;
	}

	cFlag = 1 << (iRecipeID%8); 

	if(iVal > 0)
	{
		pstSkillInfo->RecipeInfo[iRecipeID/8] |= cFlag;
	}
	else
	{
		pstSkillInfo->RecipeInfo[iRecipeID/8] &= ~cFlag;
	}

	return 0;
}


RECIPEDEF * z_find_recipe_def(ZONESVRENV *pstEnv, unsigned short iSkillType, unsigned short iRecipeID)
{
	int iIndex;
	int iEqu;
	RECIPEDEF stRecipe;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stRecipe.SkillType = iSkillType;
	stRecipe.RecipeID = iRecipeID;
	iIndex = bsearch_int(&stRecipe, pstObj->sRecipeDef, pstObj->iRecipeDef, RECIPE_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (RECIPEDEF *)pstObj->sRecipeDef[iIndex];
	}

	return NULL;
}

SECONDARYSKILLDEF  * z_find_secondary_skill_def(ZONESVRENV *pstEnv, unsigned short iSkillType, unsigned short iLevel)
{
	int iIndex;
	int iEqu;
	SECONDARYSKILLDEF stSkill;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stSkill.SkillType = iSkillType;
	stSkill.Level = iLevel;
	iIndex = bsearch_int(&stSkill, pstObj->sSecondarySkillDef, pstObj->iSecondarySkillDef, SECONDARY_SKILL_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (SECONDARYSKILLDEF *)pstObj->sSecondarySkillDef[iIndex];
	}

	return NULL;
}

SECONDARYSKILLINFO* get_player_secondary_skill_info(Player *pstPlayer, int iSkillType)
{
	int i;
	SECONDARYSKILLMISCINFO *pstSkillMiscInfo = &pstPlayer->stRoleData.MiscInfo.SecondarySkillMiscInfo;

	for(i = 0; i<pstSkillMiscInfo->SkillNum; i++)
	{
		if(pstSkillMiscInfo->SkillInfo[i].SkillType == iSkillType)
		{
			return &pstSkillMiscInfo->SkillInfo[i];
		}
	}

	return NULL;
}

int recipe_pre_clear(Player *pstPlayer)
{
	pstPlayer->stOnline.State &= ~CS_STAT_RECIPE_PREPARE;
	memset(&pstPlayer->stOnline.stRecipePre, 0, sizeof(pstPlayer->stOnline.stRecipePre));

	return 0;
}

int z_recipe_brk(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLPREBRK *pstRecipeBrk = &pstCSSecondarySkill->SvrData.SkillPreBrk;
	
	//if(!(pstPlayer->stOnline.State & CS_STAT_RECIPE_PREPARE))
	//{
	//	return -1;
	//}

	recipe_pre_clear(pstPlayer);

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_RECIPE_PRE_BRK;
	pstRecipeBrk->MemID = pstPlayer->iMemID;

	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

RECIPECOOLDOWNINFO* get_recipe_cooldown_info(SECONDARYSKILLINFO *pstSkillInfo, int iRecipeID)
{
	int i;

	for(i = 0; i<pstSkillInfo->CoolDownNum; i++)
	{
		if(pstSkillInfo->CoolDownInfo[i].RecipeID == iRecipeID)
		{
			return &pstSkillInfo->CoolDownInfo[i];
		}
	}
	
	return NULL;
}

int z_add_secondary_skill_val(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, int iVal)
{
	int i;
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLCHG *pstSkillChg = &pstCSSecondarySkill->SvrData.SkillChg;
	SECONDARYSKILLDEF *pstSkillDef = z_find_secondary_skill_def(pstEnv, pstSkillInfo->SkillType, pstSkillInfo->SkillLevel);

	if(NULL == pstSkillDef)
	{
		return -1;
	}

	if((int)(pstSkillInfo->SkillVal + iVal)> pstSkillDef->LevelUpVal)
	{
		pstSkillInfo->SkillVal = pstSkillDef->LevelUpVal;
	}
	else
	{
	
		pstSkillInfo->SkillVal += iVal;
	}

	if ((int)pstSkillInfo->SkillVal == pstSkillDef->LevelUpVal)
	{
		//�ɳ�֮·
	
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_LIFESKILL_LEVEL;
		stCond.Val1 = pstSkillInfo->SkillLevel;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
		
	}

	//ÿ��ְ�Ƶ�1-10���Զ�����
	if(pstSkillDef->Level%10 != 0 && (int )pstSkillInfo->SkillVal >= pstSkillDef->LevelUpVal)
	{
		pstSkillDef = z_find_secondary_skill_def(pstEnv, pstSkillInfo->SkillType, pstSkillInfo->SkillLevel+1);
		if(NULL == pstSkillDef)
		{
			return -1;
		}

		pstSkillInfo->SkillLevel++;
		pstSkillInfo->SkillVal = 0;

		z_role_secondary_skill_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer, OP_SECONDARYSKILL_ACT_UP, pstSkillInfo);


	}
	

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_SKILL_CHG;
	pstSkillChg->SkillType = pstSkillInfo->SkillType;
	pstSkillChg->SkillLevel = pstSkillInfo->SkillLevel;
	pstSkillChg->SkillVal = pstSkillInfo->SkillVal;
	
	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	//ϵͳ�����䷽
	for(i=0; i<MAX_AUTO_LEARN_RECIPE; i++)
	{
		if(pstSkillDef->RecipeID[i] == 0)
		{
			break;
		}
		
		player_auto_learn_recipe(pstEnv, pstPlayer, pstSkillInfo, pstSkillDef->RecipeID[i]);
	}
	
	return 0;
}

int recipe_making_add_skillval(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, RECIPEDEF *pstRecipeDef)
{
	int iRnd;
	int iRate = 0;

	//��ɫ
	if(pstRecipeDef->RecipelLevel > pstSkillInfo->SkillLevel)
	{
		iRate = 100;	
	}
	else
	{
		//��ɫ
		if(pstSkillInfo->SkillLevel - pstRecipeDef->RecipelLevel >= 10)
		{
			return 0;
		}
		//��ɫ
		else if(pstSkillInfo->SkillLevel - pstRecipeDef->RecipelLevel >= 4)
		{
			iRate = 20;
		}
		//��ɫ
		else
		{
			iRate = 80;
		}
	}

	iRnd = (int)RAND1(10000);
	if(iRnd < iRate*100)
	{
		//����������
		SECONDARYSKILLDEF *pstSkillDef = z_find_secondary_skill_def(pstEnv, pstSkillInfo->SkillType, pstSkillInfo->SkillLevel);
		if(NULL == pstSkillDef)
		{
			return -1;
		}
		
		z_add_secondary_skill_val(pstEnv, pstPlayer, pstSkillInfo, pstSkillDef->AddVal);
	}

	return 0;
}

int set_recipe_cooldown(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, RECIPEDEF *pstRecipeDef)
{
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLRECIPECHG *pstRecipeChg = &pstCSSecondarySkill->SvrData.RecipeChg;

	if(pstSkillInfo->CoolDownNum >= MAX_SECONDARY_SKILL_COOLDOWN_RECIPE)
	{
		return -1;
	}
	
	pstSkillInfo->CoolDownInfo[pstSkillInfo->CoolDownNum].RecipeID = pstRecipeDef->RecipeID;
	pstSkillInfo->CoolDownInfo[pstSkillInfo->CoolDownNum].CoolDownTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstRecipeDef->CoolDownTime;
	pstSkillInfo->CoolDownNum++;

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_RECIPE_CHG;
	pstRecipeChg->SkillType = pstRecipeDef->SkillType;
	pstRecipeChg->RecipeID = pstRecipeDef->RecipeID;
	pstRecipeChg->CoolDownTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstRecipeDef->CoolDownTime;
	
	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int z_secondary_arm_dur_dec(ZONESVRENV * pstEnv, Player * pstPlayer, int iSkillType)
{
	ROLEARM *pstRoleArm;

	//��������װ���;�
	pstRoleArm = z_wear_get_secondaryskill_arm(pstEnv, &pstPlayer->stRoleData.Wear, iSkillType);
	if (!pstRoleArm) 
	{
		return 0;
	}

	z_arm_dur_dec(pstEnv, pstPlayer, pstRoleArm, DUR_UNIT);

	if(pstRoleArm->Dur <= 0)
	{
		CSPKG stPkg;
		CSPACKAGEINFO *pstPackageInfo = &stPkg.Body.PackageInfo;
		ROLEPACKAGECHG *pstPackageChg = &pstPackageInfo->PackageInfoData.PackageChg;

		pstPackageInfo->Type = PACKAGE_INFO_CHG;
		pstPackageChg->ItemChg.ChgNum = 0;
		pstPackageChg->ArmChg.ChgNum = 0;

		z_wear_del(pstEnv, pstPlayer, pstRoleArm, pstPackageChg);

		if (pstPackageChg->ArmChg.ChgNum > 0)
		{
			Z_CSHEAD_INIT(&stPkg.Head, PACKAGE_INFO);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

			z_update_avatar(pstEnv, pstPlayer);
		}
	}
	
	return 0;
}

RECIPEGETITEM *z_get_repick_item(RECIPEDEF *pstRecipeDef, int iProduceIdx)
{
	int i;
	int iRnd, iRate;
	int iIdx = iProduceIdx;
	RECIPEPRODUCE *pstProduce;

	if(iIdx < 0 || iIdx >= MAX_RECIPE_GET_ITEM_LIST)
	{
		iIdx = 0;
	}
	pstProduce = &pstRecipeDef->ProduceList[iIdx];
	
	iRate = 0;
	iRnd = RAND1(10000);
	
	
	for(i = 0; i<MAX_RECIPE_GET_ITEM; i++)
	{
		if(pstProduce->GetItem[i].ItemID == 0)
		{
			return NULL;
		}

		iRate += pstProduce->GetItem[i].Rate;
		
		if(iRnd < iRate)
		{
			return &pstProduce->GetItem[i];
		}
	}

	return NULL;
}

int player_recipe_making_consume(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, 
	RECIPEDEF *pstRecipeDef, OPROLERECIPEMAKEINFO *pstOpMakeInfo)
{
	int i, iBind;
	PlayerChagAttrVal stChgVal;
	AttrChgVal *pstVal;
	int iRet;
	
	//����
	for(i = 0; i<MAX_RECIPE_STUFF; i++)
	{
		if(pstRecipeDef->Stuff[i].ItemID == 0)
		{
			break;
		}

		iBind = package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			pstRecipeDef->Stuff[i].ItemID, pstRecipeDef->Stuff[i].Num,
			-1, NULL,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RECIPE);
		if (0 > iBind)
		{
			return -1;
		}

		itemdec_fill(&pstOpMakeInfo->DecItem[i], pstRecipeDef->Stuff[i].ItemID, pstRecipeDef->Stuff[i].Num, iBind);
	}

	//cd
	if(pstRecipeDef->CoolDownTime)
	{
		set_recipe_cooldown(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef);
	}

	//����
	if(pstRecipeDef->ConsumeVim > 0)
	{
		stChgVal.AttrNum = 0;

		pstVal = &stChgVal.AttrList[stChgVal.AttrNum++];
		pstVal->AttrID = ATTR_ID_VIM;
		pstVal->nFlag = 1;
		pstVal->Value.Vim = pstRecipeDef->ConsumeVim;

		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	iRet = package_dec_money(pstEnv, pstPlayer, pstRecipeDef->CostMoneyType, pstRecipeDef->CostMoneyVal);
	if (pstRecipeDef->CostMoneyType == ATTR_ID_MONEY)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_MAKING, pstRecipeDef->RecipeID, 0, pstRecipeDef->CostMoneyVal, "���������");
	}
	else if (pstRecipeDef->CostMoneyType == ATTR_ID_BULL)
	{
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_MAKING, pstRecipeDef->RecipeID, pstRecipeDef->CostMoneyVal-iRet,iRet,  "���������");
	}

	return 0;
}

int player_recipe_making_check(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, RECIPEDEF *pstRecipeDef, int iProduceIdx)
{
	int i;
	ROLEGRID stRoleGrid;
	RECIPEPRODUCE *pstProduceList;

	if (0 == z_get_skill_recipe(pstSkillInfo, pstRecipeDef->RecipeID))
	{
		//ûѧ��
		return -1;
	}
	
	//��ȴ
	if(pstRecipeDef->CoolDownTime)
	{
		RECIPECOOLDOWNINFO *pstCoolDown;
		
		pstCoolDown = get_recipe_cooldown_info(pstSkillInfo, pstRecipeDef->RecipeID);
		if(pstCoolDown && pstCoolDown->CoolDownTime < pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			//�ȵ�������
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SECSKILL4, pstRecipeDef->NameDesc);
			return -1;
		}
	}

	if(pstRecipeDef->CostMoneyType > 0)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstRecipeDef->CostMoneyType, pstRecipeDef->CostMoneyVal))
		{
			char szBuff[32];

			if (money_type_to_string(pstRecipeDef->CostMoneyType, szBuff, sizeof(szBuff)) < 0 )
			{
				strcpy(szBuff, "��Ǯ");
			}

			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
			return -1;
		}
	}

	//����
	if (pstRecipeDef->ConsumeVim > pstPlayer->stRoleData.RoleDetail.VIMCurr)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NO_VIM);
		return -1;
	}

	//����
	for(i = 0; i<MAX_RECIPE_STUFF; i++)
	{
		if(pstRecipeDef->Stuff[i].ItemID == 0)
		{
			break;
		}

		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRecipeDef->Stuff[i].ItemID, 
			pstRecipeDef->Stuff[i].Num,-1, NULL,PACKAGE_DEC_NONE))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SECSKILL1);
			return -1;
		}
	}

	//������Ʒ
	if(iProduceIdx < 0 || iProduceIdx >= MAX_RECIPE_GET_ITEM_LIST)
	{
		return -1;
	}
	
	pstProduceList = &pstRecipeDef->ProduceList[iProduceIdx];
	for(i = 0; i<MAX_RECIPE_GET_ITEM; i++)
	{
		if(pstProduceList->GetItem[i].ItemID== 0)
		{
			break;
		}
		
		if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstProduceList->GetItem[i].ItemID))
		{
			return -1;
		}

		if (GRID_ITEM_ITEM == stRoleGrid.Type)
		{
			stRoleGrid.GridData.RoleItem.Num = pstProduceList->GetItem[i].Num;
		}

		if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
			return -1;
		}
	}

	return 0;
}

int player_learn_recipe_check(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, RECIPEDEF *pstRecipeDef)
{
	if(pstRecipeDef->LearnSkillLevel > pstSkillInfo->SkillLevel)
	{
		//�ȼ������޷�ѧϰ
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SECSKILL2);
		return -1;
	}

	if(pstRecipeDef->RecipeID >= MAX_SECONDARY_SKILL_RECIPE*8)
	{
		//���ID��
		return -1;
	}

	if (z_get_skill_recipe(pstSkillInfo, pstRecipeDef->RecipeID))
	{
		//ѧ����
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SECSKILL3);
		return -1;
	}
		

	if (pstRecipeDef->LearnMoneyType >= ATTR_ID_MONEY && pstRecipeDef->LearnMoneyType <= ATTR_ID_BULL)
	{
		if (0 > package_dec_money_test(pstEnv, pstPlayer, pstRecipeDef->LearnMoneyType, pstRecipeDef->LearnMoneyVal))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOHUOBI);
			return -1;
		}
	}
	
	return 0;
}

int player_learn_recipe_ui_check(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLRECIPE *pstLearn)
{
	 int iUIType = 0;
	 
	switch (pstLearn->SkillType)
	{
		case SECONDARY_SKILL_TYPE_MINING:
			iUIType = UI_TYPE_MINING_RECIPE;
			break;
		case SECONDARY_SKILL_TYPE_HERBALISM:
			iUIType = UI_TYPE_HERBALISM_RECIPE;
			break;
		case SECONDARY_SKILL_TYPE_RESOLVE:
			iUIType = UI_TYPE_RESOLVE_RECIPE;
			break;
		case SECONDARY_SKILL_TYPE_BLACKSMITHING:
			iUIType = UI_TYPE_BLACKSMITHING_RECIPE;
			break;
		case SECONDARY_SKILL_TYPE_ALCHEMY:
			iUIType = UI_TYPE_ALCHEMY_RECIPE;
			break;
		case SECONDARY_SKILL_TYPE_ENCHANT:
			iUIType = UI_TYPE_ENCHANT_RECIPE;
			break;
		case SECONDARY_SKILL_TYPE_ENGINEERING:
			iUIType = UI_TYPE_ENGINEERING_RECIPE;
			break;
		default:
			return -1;
			break;
	}
	
	return check_npc_ui(pstEnv, pstPlayer, iUIType);
}

//ѧϰ�䷽
int player_learn_recipe(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLRECIPE *pstLearn)
{
	RECIPEDEF *pstRecipeDef;
	SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, pstLearn->SkillType);
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLRECIPECHG *pstRecipeChg = &pstCSSecondarySkill->SvrData.RecipeChg;
	int iRet;
	
	if(NULL == pstSkillInfo)
	{
		//û���������ܡ�����ѧϰ�䷽
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SECSKILL5);
		return -1;
	}

	pstRecipeDef = z_find_recipe_def(pstEnv, pstLearn->SkillType, pstLearn->RecipeID);
	if(NULL == pstRecipeDef || pstRecipeDef->LearnType != LEARN_RECIPE_TYPE_UI)
	{
		//���������ѧϰ
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SECSKILL6);
		return -1;
	}
	
	if(0 > player_learn_recipe_check(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef))
	{
		return -1;
	}

	if (pstRecipeDef->LearnMoneyType >= ATTR_ID_MONEY && pstRecipeDef->LearnMoneyType <= ATTR_ID_BULL)
	{
		iRet = package_dec_money(pstEnv, pstPlayer, pstRecipeDef->LearnMoneyType, pstRecipeDef->LearnMoneyVal) ;
		if (iRet < 0)
		{
			return -1;
		}

		if (pstRecipeDef->LearnMoneyType == ATTR_ID_MONEY)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RECIPE, pstRecipeDef->RecipeID, 0, pstRecipeDef->LearnMoneyVal, "ѧϰ�䷽");
		}
		else if (pstRecipeDef->LearnMoneyType == ATTR_ID_BULL)
		{
			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_RECIPE, pstRecipeDef->RecipeID,  pstRecipeDef->LearnMoneyVal-iRet,iRet, "ѧϰ�䷽");
		}
	}

	z_set_skill_recipe(pstSkillInfo, pstRecipeDef->RecipeID, 1);

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_RECIPE_CHG;
	pstRecipeChg->SkillType = pstRecipeDef->SkillType;
	pstRecipeChg->RecipeID = pstRecipeDef->RecipeID;
	pstRecipeChg->CoolDownTime = 0;
	
	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	z_role_secskill_recipe_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef->RecipeID);
	
	return 0;
}

int player_recipe_making(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, RECIPEDEF *pstRecipeDef, int iProduceIdx)
{
	RECIPEGETITEM *pstRecipeItem;
	ROLEGRID stRoleGrid;
	ITEMDEF *pstItemDef;
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLSUCC *pstSuccInfo = &pstCSSecondarySkill->SvrData.SuccInfo;
	OPROLERECIPEMAKEINFO stOpMakeInfo;

	memset(&stOpMakeInfo, 0, sizeof(stOpMakeInfo));
	
	if (0 > player_recipe_making_check(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef, iProduceIdx))
	{
		goto _error;
	}

	if(0 > player_recipe_making_consume(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef, &stOpMakeInfo))
	{
		goto _error;
	}

	//������
	recipe_making_add_skillval(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef);

	//����
	pstRecipeItem = z_get_repick_item(pstRecipeDef, iProduceIdx);
	if(NULL == pstRecipeItem)
	{
		goto _error;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstRecipeItem->ItemID);
	if(NULL == pstItemDef)
	{
		goto _error;
	}
	
	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstRecipeItem->ItemID))
	{
		goto _error;
	}
	stRoleGrid.GridData.RoleItem.Num = pstRecipeItem->Num;

	// δ����
	if (ITEM_TYPE_ARM == pstItemDef->ItemType &&
		pstItemDef->Important > 1)
	{
		stRoleGrid.GridData.RoleArm.InstFlag |= INST_ITEM_UNCHECK;
		stRoleGrid.GridData.RoleArm.AddAttrNum = 0;
	}
	
	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
	{	
		goto _error;
	}

	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_RECIPE);

	stOpMakeInfo.GetItem.ID = pstRecipeItem->ItemID;
	stOpMakeInfo.GetItem.Num= pstRecipeItem->Num;
	stOpMakeInfo.GetItem.BindNum = 0;
	if(stRoleGrid.GridData.RoleItem.InstFlag & INST_ITEM_BIND)
	{	
		stOpMakeInfo.GetItem.BindNum = pstRecipeItem->Num;
	}

	if(is_collect_secondaryskill(pstSkillInfo->SkillType) && !is_collect_recipe(pstRecipeDef))
	{
		//�ɼ��༼���зǲɼ�;�����䷽ �����ʱ�����ĵ����;�
	}
	else
	{
		//��������װ���;�
		z_secondary_arm_dur_dec(pstEnv, pstPlayer, pstSkillInfo->SkillType);
	}

/*
	if(pstItemDef->Important >= 3)
	{
		char szMsg[256];
		CSPKG stChatLinkPkg;
				
		sprintf(szMsg, "%s�Ѿ���ţ����֮�������ռ���ȫ�˲��ϣ�����Ʒ�����������@ITEMLINK@",
			pstPlayer->stRoleData.RoleName);

		if(stRoleGrid.Type == ITEM_TYPE_ARM)
		{
			z_fill_link_msg(&stChatLinkPkg, szMsg, SYS_TIP, CHAT_LINK_ARM, (void *)&stRoleGrid.GridData.RoleArm);
		}
		else
		{
			z_fill_link_msg(&stChatLinkPkg, szMsg, SYS_TIP, CHAT_LINK_ITEM, (void *)&stRoleGrid.GridData.RoleItem);
		}

		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stChatLinkPkg);
	}
*/

	pstPlayer->stOnline.State &= ~CS_STAT_RECIPE_PREPARE;
	
	//�ɹ�֪ͨ
	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_RECIPE_SUCC;
	pstSuccInfo->MemID = pstPlayer->iMemID;
	pstSuccInfo->SkillType = pstRecipeDef->SkillType;
	pstSuccInfo->RecipeID = pstRecipeDef->RecipeID;
	pstSuccInfo->ItemID = pstRecipeItem->ItemID;
	pstSuccInfo->ItemNum = pstRecipeItem->Num;

	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	z_role_recipe_make_oplog(pstEnv, pstPlayer, pstSkillInfo->SkillType, pstSkillInfo->SkillLevel, 
		pstRecipeDef->RecipeID, &stOpMakeInfo);


	
	//�ɳ�֮·
	{
		ACHIEVEMENTCOND stCond;
	
		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_LIFESKILL_MAKE;
		stCond.Val1 = pstItemDef->Important;

		if (pstItemDef->SuitSetID)
		{
			SUITSET *pstSuit = z_find_suitset(pstEnv,pstItemDef->SuitSetID);
			if (pstSuit)
			{
				stCond.Val1 = pstSuit->SuitSetType;
			}			
		}
	
		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}
	
	return 0;

_error:
	z_recipe_brk(pstEnv, pstPlayer);
	return -1;
}

int player_recipe(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLRECIPE *pstMaking)
{
	int iIdx = 0;
	RECIPEDEF *pstRecipeDef;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, pstMaking->SkillType);
	
	if(NULL == pstSkillInfo)
	{
		//û���������ܡ�����ѧϰ�䷽
		z_recipe_brk(pstEnv, pstPlayer);
		return -1;
	}

	pstRecipeDef = z_find_recipe_def(pstEnv, pstMaking->SkillType, pstMaking->RecipeID);
	if(NULL == pstRecipeDef)
	{
		//�������䷽
		z_recipe_brk(pstEnv, pstPlayer);
		return -1;
	}

	if(0 > player_recipe_pre_check(pstEnv, pstPlayer, pstRecipeDef))
	{
		z_recipe_brk(pstEnv, pstPlayer);
		return -1;
	}

	//�����б����
	if(pstDyn->stHealChg.unSecondarySkillType == pstRecipeDef->SkillType)
	{
		iIdx = pstDyn->stHealChg.unRecipeProduceIdx;
	}

	return player_recipe_making(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef, iIdx);
}

int player_recipe_resolve(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLRESOLVE *pstResolve)
{
	int iPos;
	int iRecipeID;
	ITEMDEF *pstItemDef;
	ROLEITEM *pstRoleItem;
	RECIPEDEF *pstRecipeDef;
	RECIPEGETITEM *pstRecipeItem;
	ROLEGRID stRoleGrid;
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLSUCC *pstSuccInfo = &pstCSSecondarySkill->SvrData.SuccInfo;
	OPROLERECIPEMAKEINFO stOpMakeInfo;
	SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, SECONDARY_SKILL_TYPE_RESOLVE);
	

	memset(&stOpMakeInfo, 0, sizeof(stOpMakeInfo));
	

	if(NULL == pstSkillInfo)
	{
		//û����������
		goto _error;
	}
	
	if (0 > package_get_item(pstEnv, pstPlayer, pstResolve->ListType, 
							pstResolve->GridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		goto _error;
	}

	if (pstItemDef->Important <= 0 || pstItemDef->NoResolve)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SECSKILL7);
		goto _error;
	}

	if (ITEM_TYPE_ARM == pstItemDef->ItemType)
	{
		stOpMakeInfo.ResolveItem.Type = GRID_ITEM_ARM;
		stOpMakeInfo.ResolveItem.GridData.RoleArm = *(ROLEARM *)pstRoleItem;
	}
	else
	{
		stOpMakeInfo.ResolveItem.Type = GRID_ITEM_ITEM;
		stOpMakeInfo.ResolveItem.GridData.RoleItem = *pstRoleItem;
	}

	//ÿ0~9�� Ϊһ���䷽
	iRecipeID = pstItemDef->WearPre.RoleLevelMin/10;

	pstRecipeDef = z_find_recipe_def(pstEnv, SECONDARY_SKILL_TYPE_RESOLVE, iRecipeID);
	if(NULL == pstRecipeDef)
	{
		goto _error;
	}

	if(0 > player_recipe_pre_check(pstEnv, pstPlayer, pstRecipeDef))
	{
		goto _error;
	}

	if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstItemDef->ItemID, 1, 
							pstResolve->ListType, &iPos,PACKAGE_DEC_NONE))
	{
		goto _error;
	}

	if (0 > player_recipe_making_check(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef, pstItemDef->Important-1))
	{
		goto _error;
	}

	if(0 > player_recipe_making_consume(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef, &stOpMakeInfo))
	{
		goto _error;
	}

	if (0 > package_dec(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
							pstItemDef->ItemID, 1, 
							pstResolve->ListType, &iPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_RECIPE))
	{
		goto _error;
	}

	//������
	recipe_making_add_skillval(pstEnv, pstPlayer, pstSkillInfo, pstRecipeDef);

	//����
	pstRecipeItem = z_get_repick_item(pstRecipeDef, pstItemDef->Important-1);
	if(NULL == pstRecipeItem)
	{
		goto _error;
	}

	pstItemDef = z_find_itemdef(pstEnv, pstRecipeItem->ItemID);
	if(NULL == pstItemDef)
	{
		goto _error;
	}
	
	if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstRecipeItem->ItemID))
	{
		goto _error;
	}
	stRoleGrid.GridData.RoleItem.Num = pstRecipeItem->Num;
	
	if(0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
	{	
		goto _error;
	}
	package_add(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0, IMPITEM_OPTYPE_RECIPE);

	stOpMakeInfo.GetItem.ID = pstRecipeItem->ItemID;
	stOpMakeInfo.GetItem.Num= pstRecipeItem->Num;
	stOpMakeInfo.GetItem.BindNum = 0;
	if(stRoleGrid.GridData.RoleItem.InstFlag & INST_ITEM_BIND)
	{	
		stOpMakeInfo.GetItem.BindNum = pstRecipeItem->Num;
	}

	pstPlayer->stOnline.State &= ~CS_STAT_RECIPE_PREPARE;
	
	//�ɹ�֪ͨ
	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_RECIPE_SUCC;
	pstSuccInfo->MemID = pstPlayer->iMemID;
	pstSuccInfo->SkillType = pstRecipeDef->SkillType;
	pstSuccInfo->RecipeID = pstRecipeDef->RecipeID;
	pstSuccInfo->ItemID = pstRecipeItem->ItemID;
	pstSuccInfo->ItemNum = pstRecipeItem->Num;

	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	z_role_recipe_make_oplog(pstEnv, pstPlayer, pstSkillInfo->SkillType, pstSkillInfo->SkillLevel, 
		pstRecipeDef->RecipeID, &stOpMakeInfo);
	
	return 0;

_error:
	z_recipe_brk(pstEnv, pstPlayer);
	return -1;
}

int player_recipe_pre(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLRECIPE *pstPre)
{
	RECIPEDEF *pstRecipeDef;
	SECONDARYSKILLINFO *pstSkillInfo = get_player_secondary_skill_info(pstPlayer, pstPre->SkillType);
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLPRE *pstRecipePre = &pstCSSecondarySkill->SvrData.SkillPre;
	
	if(NULL == pstSkillInfo)
	{
		//û���������ܡ�����ѧϰ�䷽
		z_recipe_brk(pstEnv, pstPlayer);
		return -1;
	}

	pstRecipeDef = z_find_recipe_def(pstEnv, pstPre->SkillType, pstPre->RecipeID);
	if(NULL == pstRecipeDef)
	{
		//û������䷽
		z_recipe_brk(pstEnv, pstPlayer);
		return -1;
	}

	if (0 == z_get_skill_recipe(pstSkillInfo, pstPre->RecipeID))
	{
		//ûѧ��
		z_recipe_brk(pstEnv, pstPlayer);
		return -1;
	}

	if(pstPlayer->stOnline.State & CS_STAT_RECIPE_PREPARE)
	{
		z_recipe_brk(pstEnv, pstPlayer);
		return -1;
	}

	pstPlayer->stOnline.State |= CS_STAT_RECIPE_PREPARE;
	pstPlayer->stOnline.stRecipePre.RecipeID = pstPre->RecipeID;
	pstPlayer->stOnline.stRecipePre.SkillType = pstPre->SkillType;
	pstPlayer->stOnline.stRecipePre.tPreEndTime = pstEnv->pstAppCtx->stCurr.tv_sec + pstRecipeDef->MachiningTime;

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_RECIPE_PRE;
	pstRecipePre->MemID = pstPlayer->iMemID;
	pstRecipePre->SkillType = pstPre->SkillType;
	pstRecipePre->RecipeID = pstPre->RecipeID;

	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int player_secondary_skill_del(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLDEL*pstDel)
{
	int i;
	SECONDARYSKILLMISCINFO *pstSkillMiscInfo = &pstPlayer->stRoleData.MiscInfo.SecondarySkillMiscInfo;
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLDEL *pstSkillDel = &pstCSSecondarySkill->SvrData.DelSkill;
	
	/*	-lxk ��ɾ��
	if(pstDel->SkillType == SECONDARY_SKILL_TYPE_ENGINEERING)
	{
		//��������ܲ�������
		z_sys_strf(pstEnv, pstPlayer, SYS_MIDDLE2, "�����Ƽ�רҵ��������");
		return -1;
	}
	*/
	for(i = 0; i<pstSkillMiscInfo->SkillNum; i++)
	{
		if(pstSkillMiscInfo->SkillInfo[i].SkillType == pstDel->SkillType)
		{
			break;
		}
	}

	if(i >= pstSkillMiscInfo->SkillNum)
	{
		//û����������
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SECSKILL8);
		return -1;
	}

	z_role_secondary_skill_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer, 
			OP_SECONDARYSKILL_ACT_DEL, &pstSkillMiscInfo->SkillInfo[i]);
	
	pstSkillMiscInfo->SkillNum--;
	if(i != pstSkillMiscInfo->SkillNum)
	{
		memcpy(&pstSkillMiscInfo->SkillInfo[i], &pstSkillMiscInfo->SkillInfo[pstSkillMiscInfo->SkillNum], sizeof(pstSkillMiscInfo->SkillInfo[0]));
	}

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_DEL_SKILL;
	pstSkillDel->SkillType = pstDel->SkillType;

	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	del_secondaryskill_task(pstEnv, pstPlayer, pstDel->SkillType);
	
	return 0;
}

int player_recipe_pre_check(ZONESVRENV* pstEnv, Player *pstPlayer, RECIPEDEF *pstRecipeDef)
{
	//�ߵĲɼ�·�ߵĲ���Ҫ�����
	if(is_collect_recipe(pstRecipeDef))
	{
		return 0;
	}
	
	if(!(pstPlayer->stOnline.State & CS_STAT_RECIPE_PREPARE))
	{
		return -1;
	}
	
	if(pstPlayer->stOnline.stRecipePre.RecipeID != pstRecipeDef->RecipeID)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stRecipePre.SkillType != pstRecipeDef->SkillType)
	{
		return -1;
	}

	if(pstPlayer->stOnline.stRecipePre.tPreEndTime > pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	return 0;
}

int player_secondary_skill_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSSECONDARYSKILLCLT *pstSecondarySkillClt = &pstCsPkg->Body.SecondarySkillClt;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);

	switch(pstSecondarySkillClt->CltOp)
	{
		case SECONDARY_SKILL_CLT_LEARN_RECIPE:
			return player_learn_recipe(pstEnv, pstPlayer, &pstSecondarySkillClt->CltData.LearnRecipe);
			break;
		case SECONDARY_SKILL_CLT_RECIPE_PRE:
			return player_recipe_pre(pstEnv, pstPlayer, &pstSecondarySkillClt->CltData.RecipePre);
			break;
		case SECONDARY_SKILL_CLT_RECIPE_PRE_BRK:
			return z_recipe_brk(pstEnv, pstPlayer);
			break;
		case SECONDARY_SKILL_CLT_RECIPE_RESOLVE:
			if(!(pstPlayer->stOnline.State & CS_STAT_RECIPE_PREPARE))
			{
				return -1;
			}
			return player_recipe_resolve(pstEnv, pstPlayer, &pstSecondarySkillClt->CltData.Resolve);
			break;
		case SECONDARY_SKILL_CLT_RECIPE_BLACKSMITHING:
		case SECONDARY_SKILL_CLT_RECIPE_ALCHEMY:
		case SECONDARY_SKILL_CLT_RECIPE_ENCHANT:
		case SECONDARY_SKILL_CLT_RECIPE_ENGINEERING:
		case SECONDARY_SKILL_CLT_RECIPE_MINING:
		case SECONDARY_SKILL_CLT_RECIPE_HERBALISM:
			if(!(pstPlayer->stOnline.State & CS_STAT_RECIPE_PREPARE))
			{
				return -1;
			}
			return player_recipe(pstEnv, pstPlayer, &pstSecondarySkillClt->CltData.Blacksmithing);
			break;
		case SECONDARY_SKILL_CLT_DEL_SKILL:
			return player_secondary_skill_del(pstEnv, pstPlayer, &pstSecondarySkillClt->CltData.DelSkill);
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

int player_learn_secondary_skill_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillType, int iSkillLevel)
{
	int i;
	SECONDARYSKILLINFO *pstSkillInfo;
	SECONDARYSKILLMISCINFO *pstSkillMiscInfo = &pstPlayer->stRoleData.MiscInfo.SecondarySkillMiscInfo;
	UNUSED(pstEnv);
	
	if(iSkillLevel == 1)
	{
		for(i = 0; i< pstSkillMiscInfo->SkillNum; i++)
		{
			if(pstSkillMiscInfo->SkillInfo[i].SkillType == iSkillType)
			{
				//ѧ����
				return -1;
			}
		}

		if(pstSkillMiscInfo->SkillNum >= MAX_SECONDARY_SKILL)
		{
			return -1;
		}
	}
	else
	{
		pstSkillInfo = get_player_secondary_skill_info(pstPlayer, iSkillType);
		if(NULL == pstSkillInfo)
		{
			return -1;
		}
	}

	return 0;
}

int player_auto_learn_recipe(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, int iRecipeID)
{
	RECIPEDEF *pstRecipeDef;
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLRECIPECHG *pstRecipeChg = &pstCSSecondarySkill->SvrData.RecipeChg;

	pstRecipeDef = z_find_recipe_def(pstEnv, pstSkillInfo->SkillType, iRecipeID);
	if(NULL == pstRecipeDef )
	{
		return -1;
	}
	
	if(pstRecipeDef->RecipeID >= MAX_SECONDARY_SKILL_RECIPE*8)
	{
		//���ID��
		return -1;
	}

	if (z_get_skill_recipe(pstSkillInfo, pstRecipeDef->RecipeID))
	{
		//ѧ����
		return -1;
	}

	z_set_skill_recipe(pstSkillInfo, pstRecipeDef->RecipeID, 1);

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_RECIPE_CHG;
	pstRecipeChg->SkillType = pstRecipeDef->SkillType;
	pstRecipeChg->RecipeID = pstRecipeDef->RecipeID;
	pstRecipeChg->CoolDownTime = 0;
	
	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	z_role_secskill_recipe_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillInfo, iRecipeID);
	
	return 0;
}

//ͨ������ѧϰ
int player_learn_secondary_skill(ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillType, int iSkillLevel)
{
	int i;
	int iActType;
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	CSSECONDARYSKILLCHG *pstSkillChg = &pstCSSecondarySkill->SvrData.SkillChg;
	SECONDARYSKILLINFO *pstSkillInfo;
	SECONDARYSKILLMISCINFO *pstSkillMiscInfo = &pstPlayer->stRoleData.MiscInfo.SecondarySkillMiscInfo;
	SECONDARYSKILLDEF *pstSkillDef = z_find_secondary_skill_def(pstEnv, iSkillType, iSkillLevel);

	if(NULL == pstSkillDef)
	{
		return -1;
	}

	if(0 > player_learn_secondary_skill_check(pstEnv, pstPlayer, iSkillType, iSkillLevel))
	{
		return -1;
	}

	if(iSkillLevel == 1)
	{
		pstSkillInfo = &pstSkillMiscInfo->SkillInfo[pstSkillMiscInfo->SkillNum++];
		memset(pstSkillInfo, 0, sizeof(*pstSkillInfo));

		iActType = OP_SECONDARYSKILL_ACT_LEARN;
	}
	else
	{
		pstSkillInfo = get_player_secondary_skill_info(pstPlayer, iSkillType);
		if(NULL == pstSkillInfo)
		{
			return -1;
		}

		iActType = OP_SECONDARYSKILL_ACT_UP;
	}

	pstSkillInfo->SkillType = iSkillType;
	pstSkillInfo->SkillLevel = iSkillLevel;
	pstSkillInfo->SkillVal = 0;

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_SKILL_CHG;
	pstSkillChg->SkillType = pstSkillInfo->SkillType;
	pstSkillChg->SkillLevel = pstSkillInfo->SkillLevel;
	pstSkillChg->SkillVal = pstSkillInfo->SkillVal;
	
	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	z_role_secondary_skill_oplog(pstEnv->pstAppCtx, pstEnv, pstPlayer, iActType, pstSkillInfo);

	//ϵͳ�����䷽
	for(i = 0; i < MAX_AUTO_LEARN_RECIPE; i++)
	{
		if(pstSkillDef->RecipeID[i] == 0)
		{
			break;
		}
		
		player_auto_learn_recipe(pstEnv, pstPlayer, pstSkillInfo, pstSkillDef->RecipeID[i]);
	}

	//�ɳ�֮·
	{
		ACHIEVEMENTCOND stCond;

		memset(&stCond,0,sizeof(stCond));
		
		stCond.Type = COND_LIFESKILL_LEARN;

		z_player_achievement_cond(pstEnv,pstPlayer,&stCond);
	}
	
	return 0;
}

int z_recipe_cooldown_check(ZONESVRENV* pstEnv, SECONDARYSKILLINFO *pstSkillInfo)
{
	int i;

	for(i = pstSkillInfo->CoolDownNum-1; i>= 0; i--)
	{
		if(pstSkillInfo->CoolDownInfo[i].CoolDownTime <= pstEnv->pstAppCtx->stCurr.tv_sec)
		{
			pstSkillInfo->CoolDownNum--;
			if(i != pstSkillInfo->CoolDownNum)
			{
				memmove(&pstSkillInfo->CoolDownInfo[i], &pstSkillInfo->CoolDownInfo[i+1], 
					(pstSkillInfo->CoolDownNum-i)*sizeof(pstSkillInfo->CoolDownInfo[0]));
			}
		}
	}
	
	return 0;
}

int player_secondary_skill_info(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	CSPKG stPkg;
	CSSECONDARYSKILLSVR *pstCSSecondarySkill = &stPkg.Body.SecondarySkillSvr;
	SECONDARYSKILLMISCINFO *pstSkillInfo = &pstCSSecondarySkill->SvrData.AllInfo;
	SECONDARYSKILLMISCINFO *pstSkillMiscInfo = &pstPlayer->stRoleData.MiscInfo.SecondarySkillMiscInfo;

	pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SECONDARY_SKILL_ALL;

	for(i = 0; i<pstSkillMiscInfo->SkillNum; i++)
	{
		z_recipe_cooldown_check(pstEnv, &pstSkillMiscInfo->SkillInfo[i]);
	}

	pstCSSecondarySkill->SvrOp = SECONDARY_SKILL_SVR_INFO;
	memcpy(pstSkillInfo, pstSkillMiscInfo, sizeof(*pstSkillInfo));
	
	Z_CSHEAD_INIT(&stPkg.Head, SECONDARY_SKILL_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

int open_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ActionID = ACTION_UI;
	pstActionRes->ID = iNpcID;
	pstActionRes->Data.UI.UIType = iUIType;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int is_collect_secondaryskill(int iSkillType)
{
	if(iSkillType == SECONDARY_SKILL_TYPE_HERBALISM ||
		iSkillType == SECONDARY_SKILL_TYPE_MINING)
	{
		return 1;
	}

	return 0;
}

int is_collect_recipe(RECIPEDEF *pstRecipeDef)
{
	//�ߵĲɼ�·�ߵ��䷽
	if(pstRecipeDef->SkillType == SECONDARY_SKILL_TYPE_HERBALISM &&
		pstRecipeDef->RecipeType1 == 1)
	{
		return 1;
	}

	if(pstRecipeDef->SkillType == SECONDARY_SKILL_TYPE_MINING &&
		pstRecipeDef->RecipeType1 == 1)
	{
		return 1;
	}

	return 0;
}
