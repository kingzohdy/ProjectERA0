/*
**  @file $RCSfile: zone_ai.c,v $
**  general description of this module
**  $Id: zone_ai.c,v 1.484 2014/06/16 04:28:54 lzk Exp $
**  @author $Author: lzk $
**  @date $Date: 2014/06/16 04:28:54 $
**  @version $Revision: 1.484 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_ai.h"
#include "zone_npc.h"
#include "zone_map.h"
#include "zone_move.h"
#include "zone_attr.h"
#include "zone_skill.h"
#include "zone_clt.h"
#include "zone_combat.h"
#include "mapmask.h"
#include "zone_script.h"
#include "zone_status.h"
#include "zone_team.h"
#include "zone_svr.h"
#include "zone_machine.h"
#include "zone_move.h"
#include "zone_battle.h"
#include "zone_booty.h"
#include "zone_player.h"
#include "zone_city.h"

extern ScriptEnv stSEnv;

#define RETRY_TRACE_TIME 2
#define MOVE_INTERVAL_MIN 3
#define MOVE_INTERVAL_MAX 10
#define MON_HOME_TIME 5
#define TRACE_TRY 40
#define FOLLOW_TRY 7
#define FOLLOW_MSK 1
#define MAX_PATROL_TRY 20
#define FILL_HP_TIME 3

//û��ָ��Ŀ��,λ����
#define SPECIFIC_TARGET_NO   1
//�Ѿ�ָ����Ŀ�� 
#define SPECIFIC_TARGET_HAVE   2

int mon_ai_move(ZONESVRENV* pstEnv,  Monster *pstMon);
int mon_patrol_script(ZONESVRENV* pstEnv, Monster *pstMon);
//int mon_ai_evt_outpress(ZONESVRENV* pstEnv, Monster *pstMon);
int mon_ai_start_atk(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAtkee);
int is_no_restore_hp_mon(ZONESVRENV* pstEnv, Monster *pstMon);

static SKILLRESULT* mon_will_atk_specific_target(ZONESVRENV* pstEnv,
                                                 Monster *pstMon);

static int mon_ai_select_specific_target(ZONESVRENV* pstEnv, Monster *pstMon,
                                         const SKILLRESULT *pstContext);

//static int mon_ai_atk_specific_target(ZONESVRENV* pstEnv, Monster *pstMon);

static int player_delete_pressed_mon(ZONESVRENV* pstEnv, Player *pstPlayer,
                                     int iPressedID);

int get_mon_atk_skill_dist(ZONESVRENV* pstEnv, Monster *pstMon);

int mon_help_by_map(ZONESVRENV* pstEnv, Monster *pstMon, 
	MONSTERDEF *pstMonDef, ZoneAni *pstAtker);

typedef struct tagPredContext
{
	int iKey;
	int iValue;
}PredContext;

static int ordered_find_from_dummy(PredContext* astContext, int iNum,
                                   int iOrder);

typedef enum tagMonSkillType
{
	MST_NONE = 0,
	MST_NORMAL,
	MST_ATKER,
	MST_ATKEE,
}MonSkillType;

static MonSkillType mon_find_skill_by_defid(const MONSTERDEF *pstMonDef,
                                            int iSkillDefID, int *piSkillLevel);

// ����������ȴ�����Ҵ��ʩ��
static inline int mon_skill_begin_cool_and_break(ZONESVRENV* pstEnv,
                                                Monster *pstMon)
{
	mon_skill_begin_cool(pstEnv, pstMon);
	z_mon_skill_break(pstEnv, pstMon);
	return -1;
}

static MonSkillType mon_find_skill_by_defid(const MONSTERDEF *pstMonDef,
                                            int iSkillDefID, int *piSkillLevel)
{
	size_t i = 0;
	for(i = 0; i < CCH(pstMonDef->AINormalSkills); ++i)
	{
		if(!pstMonDef->AINormalSkills[i].SkillID)
		{
			break;
		}
		if(iSkillDefID == pstMonDef->AINormalSkills[i].SkillID)
		{
			if(piSkillLevel)
			{
				*piSkillLevel = pstMonDef->AINormalSkills[i].SkillLevel;
			}
			return MST_NORMAL;
		}
	}

	for(i = 0; i < CCH(pstMonDef->AIAtkerSkills); ++i)
	{
		if(!pstMonDef->AIAtkerSkills[i].SkillID)
		{
			break;
		}
		if(iSkillDefID == pstMonDef->AIAtkerSkills[i].SkillID)
		{
			if(piSkillLevel)
			{
				*piSkillLevel = pstMonDef->AIAtkerSkills[i].SkillLevel;
			}
			return MST_ATKER;
		}
	}

	for(i = 0; i < CCH(pstMonDef->AIAtkeeSkills); ++i)
	{
		if(!pstMonDef->AIAtkeeSkills[i].SkillID)
		{
			break;
		}
		if(iSkillDefID == pstMonDef->AIAtkeeSkills[i].SkillID)
		{
			if(piSkillLevel)
			{
				*piSkillLevel = pstMonDef->AIAtkeeSkills[i].SkillLevel;
			}
			return MST_ATKEE;
		}
	}

	return MST_NONE;
}

int z_mon_skill_atk_dist(SKILLDEF *pstSkillDef)
{
	if (pstSkillDef->Distance > 0)
	{
		return pstSkillDef->Distance;
	}

	switch(pstSkillDef->MainRange.RangeType)
	{
	case SKILL_RANGE_RECT:
		return pstSkillDef->MainRange.RangeVal2;
		break;
	case SKILL_RANGE_CIRCLE:
		return pstSkillDef->MainRange.RangeVal1;
		break;
	case SKILL_RANGE_ARC:
		return pstSkillDef->MainRange.RangeVal1;
		break;
	default:
		return pstSkillDef->Distance;
		break;
	}

	return pstSkillDef->Distance;
}

int  z_mon_ai_data(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef)
{
	pstMon->bAtkType = pstMonDef->AtkType;
	pstMon->bMoveType = pstMonDef->MoveType;
	pstMon->unViewDist = pstMonDef->ViewDist;
	pstMon->unPressSelectAdd = pstMonDef->PressSelectAdd;
	pstMon->bRandSelect = pstMonDef->RandSelect;
	pstMon->unTraceDist = pstMonDef->TraceDist;
	pstMon->unRadius = pstMonDef->Radius;
	pstMon->bSubType = pstMonDef->SubType;

	//�����е����
	mon_machine_data(pstEnv, pstMon);

	mon_refresh_skill(pstEnv->pstAppCtx, pstEnv, pstMon);	
	return 0;
}
//ˢ״̬����,CoolFlag 0��ʾˢ������Ҫ��ȴ��1��ʾˢ������Ҫ��ȴ
int mon_refresh_state_skill( ZONESVRENV* pstEnv, Monster* pstMon,
								MONSTERSTATESKILLDEF *pstMonStateSkillDef,int CoolFlag)
{
	int i;
	int iOnceCool = 0, iOnceDist = 0;
	SKILLDEF *pstSkillDef;
	int SkillNum = 0;
	MONSTERDEF *pstMonDef;

	//����ǰ�ļ��ܶ������,һ��Ū�ɾ�
	memset(pstMon->astNormalSkill,0,sizeof(MonNormalSkill)*MAX_NORM_SKILL);
	memset(pstMon->astAtkerInsertSkill,0,sizeof(MonInsertSkill)*MAX_ATKER_SKILL);
	memset(pstMon->astAtkeeInsertSkill,0,sizeof(MonInsertSkill)*MAX_ATKEE_SKILL);

	iOnceCool = 9999;
	iOnceDist = 9999;
	pstMon->unMinAtkSkillDist = 9999;
	pstMon->unMinAtkSkillCool = 9999;
	pstMon->cAtkInsert = 0;

	pstMonDef = z_get_mon_def( pstEnv,  pstMon);
	if(!pstMonDef)
	{
		return 0;
	}
	
	SkillNum = 0;
	for (i=0; i<MAX_NORM_SKILL; i++)
	{	
		pstMon->astNormalSkill[SkillNum].stSkill = pstMonStateSkillDef->AINormalSkills[i];
		
		pstSkillDef = z_find_skill(pstEnv, pstMon->astNormalSkill[SkillNum].stSkill.SkillID, pstMon->astNormalSkill[SkillNum].stSkill.SkillLevel, 
					&pstMon->astNormalSkill[SkillNum].iSkillIdx);
		if (!pstSkillDef)
		{
			continue;
		}
		if(!CoolFlag)
		{
			TV_ADD_MS(pstMon->astNormalSkill[SkillNum].stCoolEnd, pstEnv->pstAppCtx->stCurr,
			pstSkillDef->SelfCool*100);
		}
		else
		{
			pstMon->astNormalSkill[SkillNum].stCoolEnd = pstEnv->pstAppCtx->stCurr;
		}
	
		//if (pstSkillDef->SelfCool < pstMon->unMinAtkSkillCool) //��漼��
		if (pstSkillDef->SelfCool < pstMon->unMinAtkSkillCool ) //��漼��
		{
			if(pstMonDef->AINormalSkills[SkillNum].Once == 0)
			{
				pstMon->unMinAtkSkillCool = pstSkillDef->SelfCool;
				pstMon->unMinAtkSkillDist = z_mon_skill_atk_dist(pstSkillDef) + pstMonDef->Radius;
			}
			else
			{
				if(pstSkillDef->SelfCool < iOnceCool)
				{
					iOnceCool  = pstSkillDef->SelfCool;
					iOnceDist = z_mon_skill_atk_dist(pstSkillDef) + pstMonDef->Radius;
				}
			}
		}
		
		SkillNum++;
	}

	SkillNum = 0;
	for (i=0; i<MAX_ATKER_SKILL; i++)
	{
		pstMon->astAtkerInsertSkill[SkillNum].stSkill = pstMonStateSkillDef->AIAtkerSkills[i];
		pstSkillDef = z_find_skill(pstEnv, pstMon->astAtkerInsertSkill[SkillNum].stSkill.SkillID, 
								pstMon->astAtkerInsertSkill[SkillNum].stSkill.SkillLevel, 
								&pstMon->astAtkerInsertSkill[SkillNum].iSkillIdx);

		if (!pstSkillDef)
		{
			continue;
		}
		
		pstMon->cAtkInsert = 1;
		
		if(!CoolFlag)
		{
			TV_ADD_MS(pstMon->astAtkerInsertSkill[SkillNum].stCoolEnd, pstEnv->pstAppCtx->stCurr,
				pstSkillDef->SelfCool*100);
		}
		else
		{
			pstMon->astAtkerInsertSkill[SkillNum].stCoolEnd = pstEnv->pstAppCtx->stCurr;
		}
		
		SkillNum++;
	}

	SkillNum = 0;
	for (i=0; i<MAX_ATKEE_SKILL; i++)
	{
		pstMon->astAtkeeInsertSkill[SkillNum].stSkill = pstMonStateSkillDef->AIAtkeeSkills[i];
		pstSkillDef = z_find_skill(pstEnv, pstMon->astAtkeeInsertSkill[SkillNum].stSkill.SkillID, 
			pstMon->astAtkeeInsertSkill[SkillNum].stSkill.SkillLevel, 
			&pstMon->astAtkeeInsertSkill[SkillNum].iSkillIdx);

		if (!pstSkillDef)
		{
			continue;
		}

		if(!CoolFlag)
		{
			TV_ADD_MS(pstMon->astAtkeeInsertSkill[SkillNum].stCoolEnd, pstEnv->pstAppCtx->stCurr,
				pstSkillDef->SelfCool*100);
		}
		else
		{
			pstMon->astAtkeeInsertSkill[SkillNum].stCoolEnd = pstEnv->pstAppCtx->stCurr;
		}
		
		SkillNum++;
	}

	if(pstMon->unMinAtkSkillCool == 9999 ||pstMon->unMinAtkSkillDist == 9999)
	{
		pstMon->unMinAtkSkillCool = iOnceCool;
		pstMon->unMinAtkSkillDist = iOnceDist;
	}

	return 0;
}
	
int mon_refresh_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon)
{
	MONSTERDEF *pstMonsterDef = NULL;
	MONSTERSTATESKILLDEF stMonStateSkillDef;
	STATUSDEF  *pstStatusDef;
	ZoneStatus *pstZoneStatus = NULL;

	memset(&stMonStateSkillDef,0,sizeof(stMonStateSkillDef));
	
	pstMonsterDef = z_get_mon_def(pstEnv, pstMon);
	if(!pstMonsterDef)
	{
		return -1;
	}

	pstZoneStatus = z_get_mon_zonestatus_by_resultid(pstEnv, pstMon, STATUS_RESULT_MON_ADD_SKILL);
	if(pstZoneStatus)
	{
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if(pstStatusDef)
		{
			z_mon_chg_status_skill(pstEnv, pstMon,pstStatusDef,1);
			return 0;
		}
	}

	memcpy(stMonStateSkillDef.AINormalSkills,pstMonsterDef->AINormalSkills,sizeof(AINORMALSKILL)*MAX_NORM_SKILL);
	memcpy(stMonStateSkillDef.AIAtkeeSkills,pstMonsterDef->AIAtkeeSkills,sizeof(AIINSERTSKILL)*MAX_ATKEE_SKILL);
	memcpy(stMonStateSkillDef.AIAtkerSkills,pstMonsterDef->AIAtkerSkills,sizeof(AIINSERTSKILL)*MAX_ATKER_SKILL);
	
	mon_refresh_state_skill(pstEnv, pstMon,&stMonStateSkillDef,1);
	
	return 0;
	UNUSED(pstAppCtx);
}

int mon_have_movespd(Monster *pstMon)
{
	if (pstMon->stMspdInfo.NextNum)
	{
		return pstMon->stMspdInfo.MoveSpdNext.MoveSpd;
	}
	else
	{
		return pstMon->stMspdInfo.MoveSpdCurr;
	}
}

int mon_ai_skill_valid(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, TMAPMASK *pstMask, Monster *pstMon, ZoneAni *pstAniAtkee, SKILLDEF *pstSkillDef)
{
	int iAtkeeRadius = 0;
	RESPOS stAtkeePos;
	
	if (NULL == pstSkillDef)
	{
		return 0;
	}

	if (  pstSkillDef->SkillType == SKILL_TYPE_SPECIFY_AREA_ATK)
	{
		return 1;
	}

	if(pstSkillDef->Distance <= 0)
	{
		return 1;
	}
	else
	{
		if (OBJ_PLAYER == pstAniAtkee->iType)
		{
			stAtkeePos = pstAniAtkee->stObj.pstPlayer->stRoleData.Pos;
		}
		else if (OBJ_MONSTER == pstAniAtkee->iType)
		{
			stAtkeePos = pstAniAtkee->stObj.pstMon->stCurrPos;
			iAtkeeRadius = pstAniAtkee->stObj.pstMon->unRadius;
		}
		else if (OBJ_NPC == pstAniAtkee->iType)
		{
			stAtkeePos = pstAniAtkee->stObj.pstNpc->stMon.stCurrPos;
			iAtkeeRadius =  pstAniAtkee->stObj.pstNpc->stMon.unRadius;
		}
		else
		{
			return 0;
		}
	}

	if (z_distance(&pstMon->stCurrPos, &stAtkeePos) > z_mon_skill_atk_dist(pstSkillDef) +pstMon->unRadius+iAtkeeRadius)
	{
		return 0;
	}

	if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, stAtkeePos.X, stAtkeePos.Y, pstMon->stCurrPos.X, pstMon->stCurrPos.Y))
	{
		return 0;
	}

	return 1;
}

SKILLDEF * get_mon_nextskill_def(ZONESVRENV * pstEnv, Monster * pstMon)
{
	SKILLDEF *pstSkillDef;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	AINORMALSKILL *pstAISkill;
	AINEXTSKILL *pstNextSkill;

	if(pstMonAtkInfo->bNextSkillIdx == 0)
	{
		return NULL;
	}
	
	//��ʱֻ��normal �����к���
	if(pstMonAtkInfo->bHeadSkillIdx >= MAX_NORM_SKILL)
	{
		return NULL;
	}

	pstAISkill = &pstMon->astNormalSkill[pstMonAtkInfo->bHeadSkillIdx].stSkill;

	if(pstMonAtkInfo->bNextSkillIdx > MAX_NEXT_SKILL)
	{
		return NULL;
	}
	
	pstNextSkill = &pstAISkill->NextSkill[pstMonAtkInfo->bNextSkillIdx-1];
	
	pstSkillDef = z_find_skill(pstEnv, pstNextSkill->SkillID, pstNextSkill->SkillLevel, &pstMonAtkInfo->iSkillIdx);
	if(pstSkillDef == NULL)
	{
		return NULL;
	}

	return pstSkillDef;
}

int mon_select_nextskill(ZONESVRENV * pstEnv, ZONEMAPINST *pstMapInst, TMAPMASK *pstMask, 
					Monster * pstMon, ZoneAni * pstAtkee, int * piHaveSkill)
{
	SKILLDEF *pstSkillDef;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	
	pstSkillDef = get_mon_nextskill_def(pstEnv, pstMon);
	if(pstSkillDef == NULL)
	{
		return 0;
	}

	//Ŀǰ��һֱ�ȵ�������ܿ��Էų���
	if (mon_ai_skill_valid(pstEnv, pstMapInst, pstMask, pstMon, pstAtkee, pstSkillDef))
	{
		pstMonAtkInfo->unSkillID = pstSkillDef->SkillID;
		pstMonAtkInfo->bSkillLevel = pstSkillDef->Level;
		pstMonAtkInfo->bSkillType = MON_SKILL_TYPE_ERROR;
		pstMonAtkInfo->bNextSkillIdx++;
	}

	if (piHaveSkill)
	{
		*piHaveSkill = 1;
	}
	
	return 1;
}

int mon_select_atkskill(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAtkee, int *piHaveSkill)
{
	int i;
	struct timeval stDiff;
	long long  llMs;
	MonNormalSkill *pstNormSkill;
	SKILLDEF *pstSkillDef;
	TMAPMASK *pstMask;
	MapIndex *pstMapIdx;
	ZONEMAPINST *pstMapInst;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	int iRand = 0;

	pstMonAtkInfo->DirFace = 0;
	
	if(!piHaveSkill)
	{
		return 0;
	}
	
	*piHaveSkill = 0;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);;
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return 0;
	}
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	//�����ͷź�������
	if(mon_select_nextskill(pstEnv, pstMapInst, pstMask, pstMon, pstAtkee, piHaveSkill))
	{
		return 0;
	}
	
	
	//����һ��
	pstMonAtkInfo->bNextSkillIdx = 0;
	
	for (i=0; i<MAX_NORM_SKILL; i++)
	{
		pstNormSkill = pstMon->astNormalSkill + i;
			
		if (0 == pstNormSkill->stSkill.SkillID)
		{
			break;
		}

		if (pstNormSkill->iOnce && pstNormSkill->stSkill.Once)
		{
			continue;
		}

		pstSkillDef = z_find_skill(pstEnv, pstNormSkill->stSkill.SkillID, pstNormSkill->stSkill.SkillLevel,
												&pstNormSkill->iSkillIdx);
		if (NULL == pstSkillDef)
		{
			continue;
		}
		
		if (0 == mon_ai_skill_valid(pstEnv, pstMapInst, pstMask, pstMon, pstAtkee, pstSkillDef))
		{
			continue;
		}

		if(0 > mon_ai_check_skill_conditionid(pstEnv,pstMon,pstSkillDef,
					pstNormSkill->stSkill.ConditionID,pstNormSkill->stSkill.ConditionVal,0) )
		{
			continue;
		}
		iRand = RAND1(10000);
		if (iRand >= 100.0*pstNormSkill->stSkill.Rate)
		{
			if (pstSkillDef->SelfCool <= pstMon->unMinAtkSkillCool)
			{
				if (piHaveSkill )
            	{
                		*piHaveSkill = 1;
            	}
			}
			continue;
		}

		TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstNormSkill->stCoolEnd);
		TV_TO_MS(llMs, stDiff);
		if (llMs < 0)
		{
			if (pstSkillDef->SelfCool <= pstMon->unMinAtkSkillCool)
			{
				if (piHaveSkill ) 
				{
					*piHaveSkill = 1;
				}
			}
			
			continue;
		}

		if (piHaveSkill ) 
		{
			*piHaveSkill = 1;
		}

		pstMonAtkInfo->unSkillID = pstSkillDef->SkillID;
		pstMonAtkInfo->bSkillLevel = pstSkillDef->Level;
		pstMonAtkInfo->iSkillIdx = pstNormSkill->iSkillIdx;
		pstMonAtkInfo->bSkillType = MON_SKILL_TYPE_NORMAL;
		pstMonAtkInfo->bMonSkillIdx = i;

		//�к�������
		if(pstNormSkill->stSkill.NextSkill[0].SkillID > 0)
		{
			pstMonAtkInfo->bNextSkillIdx = 1;
			pstMonAtkInfo->bHeadSkillIdx=  i;			
		}

		return 0;
	}

	return -1;
}

int mon_select_insertskill(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	struct timeval stDiff;
	long long  llMs;
	MonInsertSkill *pstInsertSkill;
	int iRand;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	SKILLDEF *pstSkillDef;

	pstMonAtkInfo->DirFace = 0;
	
	for (i=0; i<MAX_ATKER_SKILL; i++)
	{
		pstInsertSkill = pstMon->astAtkerInsertSkill + i;
			
		if (0 == pstInsertSkill->stSkill.SkillID)
		{
			break;
		}

		if (pstInsertSkill->iOnce && pstInsertSkill->stSkill.Once)
		{
			continue;
		}

		pstSkillDef = z_find_skill(pstEnv, pstInsertSkill->stSkill.SkillID, 
								pstInsertSkill->stSkill.SkillLevel, &pstInsertSkill->iSkillIdx);
		if (NULL == pstSkillDef)
		{
			continue;
		}

		//������жϻ�Ӱ��mon_help���󾮸����еİ¸�˹
		if(pstSkillDef->NoFightStatUse == ENTER_FIGHT_USE_ATK)
		{
			if(!(pstMon->bAiStat == AI_STAT_ATK || pstMon->bAiStat == AI_STAT_TRACE))
			{
				continue;
			}
		}

		iRand = RAND1(10000);
		if (iRand >= 100*pstInsertSkill->stSkill.Rate)
		{
		     continue;
		}

		if(0 > mon_ai_check_skill_conditionid(pstEnv,pstMon,pstSkillDef,
					pstInsertSkill->stSkill.ConditionID,pstInsertSkill->stSkill.ConditionVal,0) )
		{
			continue;
		}
		

		TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstInsertSkill->stCoolEnd);
		TV_TO_MS(llMs, stDiff);
		if (llMs < 0)
		{
			continue;
		}

		pstMonAtkInfo->unSkillID = pstInsertSkill->stSkill.SkillID;
		pstMonAtkInfo->bSkillLevel = pstInsertSkill->stSkill.SkillLevel;
		pstMonAtkInfo->iSkillIdx = pstInsertSkill->iSkillIdx;
		pstMonAtkInfo->bSkillType = MON_SKILL_TYPE_INSERT;
		pstMonAtkInfo->bMonSkillIdx = i;

		return 0;
	}

	return -1;
}

int mon_ai_insert_atk(ZONESVRENV* pstEnv, Monster *pstMon)
{
	struct timeval stDiff;
	long long  llMs;
	ZoneAni stAtkee;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	SKILLDEF *pstSkillDef;

	if (pstMon->stMonAtkInfo.cAtkStat == CS_STAT_PREPARE)
	{
		if (pstMon->stMonAtkInfo.bSkillType == MON_SKILL_TYPE_INSERT)
		{
			SKILLDEF *pstSkillDef;

			pstSkillDef = z_find_skill(pstEnv, pstMon->stMonAtkInfo.unSkillID, pstMon->stMonAtkInfo.bSkillLevel, &pstMon->stMonAtkInfo.iSkillIdx);
			if (NULL == pstSkillDef)
			{
				pstMon->stMonAtkInfo.cAtkStat = 0;
				return -1;
			}
			
			TV_DIFF(stDiff, pstMon->stMonAtkInfo.stAtkStatEnd, pstEnv->pstAppCtx->stCurr);
			TV_TO_MS(llMs, stDiff);

			if (0 >= llMs)
			{
				return mon_prepare_end(pstEnv, pstMon, pstSkillDef);
			}
			else
			{
				return -1;
			}
			
		}
		else
		{
			return -1;
		}
	}
	//����������
	else if(pstMonAtkInfo->unPilot > 0)
	{
		return -1;
	}
	
	if (0 > mon_select_insertskill(pstEnv, pstMon))
	{
		return -1;
	}

	pstMon->stMonAtkInfo.fIsSpecificTarget = 
		!!mon_will_atk_specific_target(pstEnv, pstMon);
	if(pstMon->stMonAtkInfo.fIsSpecificTarget)
	{
		return mon_ai_start_atk(pstEnv, pstMon, NULL);
	}

	stAtkee.iType = OBJ_MONSTER;
	stAtkee.stObj.pstMon = pstMon;

	if(0 > mon_ai_start_atk(pstEnv, pstMon, &stAtkee))
	{
		return -1;
	}

	pstSkillDef = z_find_skill(pstEnv, pstMonAtkInfo->unSkillID, 
							pstMonAtkInfo->bSkillLevel, &pstMonAtkInfo->iSkillIdx);
	if (pstSkillDef && pstSkillDef->MoveAttack)
	{
		return 1;
	}
	
	return 0;
}

// �����Ƿ�Ҫ����һ���ض���Ŀ��
static SKILLRESULT* mon_will_atk_specific_target(
	ZONESVRENV* pstEnv,
	Monster *pstMon)
{
	SKILLDEF *pstSkillDef = NULL;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;

	pstSkillDef = z_find_skill(pstEnv,
	                           pstMonAtkInfo->unSkillID,
	                           pstMonAtkInfo->bSkillLevel,
	                           &pstMonAtkInfo->iSkillIdx);
	if(!pstSkillDef)
	{
		pstMonAtkInfo->cAtkStat = 0;
		return NULL;
	}

	return z_skill_get_result(pstSkillDef, RESULT_ATK_SPECIFIC_TARGET);
}

// ������ָ���ļ��ܹ���һ��ָ����Ŀ��
int mon_ai_atk_specific_target_by_id(ZONESVRENV* pstEnv, Monster *pstMon,
                                     int iSkillID, int iTargetID)
{
	SKILLDEF *pstSkillDef = NULL;
	MonAtkInfo *pstAtkInfo = &pstMon->stMonAtkInfo;
	int iSkillLevel = 0;
	const MONSTERDEF *pstMonDef = z_get_mon_def(pstEnv, pstMon);

	if(!pstMonDef)
	{
		return -1;
	}

	// ѡ����: ���еļ���
	if(mon_find_skill_by_defid(pstMonDef, iSkillID, &iSkillLevel) == MST_NONE)
	{
		return -1;
	}

	pstAtkInfo->unSkillID   = (unsigned short)iSkillID;
	pstAtkInfo->bSkillLevel = (unsigned char)iSkillLevel;
	pstSkillDef = z_find_skill(pstEnv, pstAtkInfo->unSkillID,
	                           pstAtkInfo->bSkillLevel, &pstAtkInfo->iSkillIdx);
	if(!pstSkillDef)
	{
		pstAtkInfo->cAtkStat = 0;
		return -1;
	}

	// ѡĿ��
	if(iTargetID < 0)
	{
		return -1;
	}
	pstAtkInfo->iAtkeeID = iTargetID; // Ŀ�걻ָ��
	
	// ����ȴ
	mon_skill_begin_cool(pstEnv, pstMon);

	// ����!
	pstMon->stMonAtkInfo.fIsSpecificTarget = 0;
	pstMon->stMonAtkInfo.fIsSpecificTarget |= SPECIFIC_TARGET_HAVE;
	return mon_ai_start_atk(pstEnv, pstMon, NULL);
}

// ����ѡ���ض�Ŀ��
static int mon_ai_select_specific_target(ZONESVRENV* pstEnv, Monster *pstMon,
                                         const SKILLRESULT *pstContext)
{
	MonAtkInfo *pstAtkInfo = &pstMon->stMonAtkInfo;
	int iTargetID = -1;

	switch(pstContext->SkillResultVal1)
	{
    case SPECIFIC_TARGET_RANDOM: 	/* ���Ŀ�� */
    	break;

    case SPECIFIC_TARGET_PRESS_ORDER: 	/* ѹ�����е���λ */
    	iTargetID = mon_press_order(pstMon, pstContext->SkillResultVal2);
    	break;

    case SPECIFIC_TARGET_CAREER: 	/* ָ��ְҵ */
    	iTargetID = mon_press_specific_career(pstEnv, pstMon,
    	                                      pstContext->SkillResultVal2);
    	break;

    case SPECIFIC_TARGET_HP_ORDER: 	/* Ѫ����λ */
    	iTargetID = mon_press_hp_order(pstEnv, pstMon,
    	                               pstContext->SkillResultVal2);
    	break;

    case SPECIFIC_TARGET_HP_PERCENT_ORDER: 	/* Ѫ���ٷֱ���λ */
    	iTargetID = mon_press_hp_percent_order(pstEnv, pstMon,
    	                                       pstContext->SkillResultVal2);
    	break;

    default:
    	return -1;
    	break;
	}

	// ������Ϲ����Ҳ���Ŀ��, �����ѡ��Ŀ��
	if(iTargetID < 0)
	{
		iTargetID = mon_press_forced_rand(pstMon);
	}
	if(iTargetID < 0)
	{
		return -1;
	}

	pstAtkInfo->iAtkeeID = iTargetID; // Ŀ�걻ָ��
	return 0;
}

// ���＼��������ʼ��ȴ
int mon_skill_begin_cool(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i = 0;
	long long llMs = 0;
	const MonAtkInfo *pstAtkInfo = &pstMon->stMonAtkInfo;
	const SKILLDEF *pstSkillDef = z_find_skill(
		pstEnv,
		pstMon->stMonAtkInfo.unSkillID,
		pstMon->stMonAtkInfo.bSkillLevel,
		&pstMon->stMonAtkInfo.iSkillIdx);

	if(!pstSkillDef)
	{
		pstMon->stMonAtkInfo.cAtkStat = 0;
		return -1;
	}
	
	// ����������ȴ
	llMs = pstSkillDef->SelfCool * 100;
	switch(pstAtkInfo->bSkillType)
	{
	case MON_SKILL_TYPE_NORMAL:
		if(pstAtkInfo->bMonSkillIdx < MAX_NORM_SKILL)
		{
			TV_ADD_MS(pstMon->astNormalSkill[pstAtkInfo->bMonSkillIdx].stCoolEnd, 
			          pstEnv->pstAppCtx->stCurr, llMs);
		}
		break;
	case MON_SKILL_TYPE_INSERT:
		if(pstAtkInfo->bMonSkillIdx < MAX_ATKER_SKILL)
		{
			TV_ADD_MS(
				pstMon->astAtkerInsertSkill[pstAtkInfo->bMonSkillIdx].stCoolEnd, 
				pstEnv->pstAppCtx->stCurr, llMs);
		}
		break;
	case MON_SKILL_TYPE_ATKEE:
		if(pstAtkInfo->bMonSkillIdx < MAX_ATKEE_SKILL)
		{
			TV_ADD_MS(
				pstMon->astAtkeeInsertSkill[pstAtkInfo->bMonSkillIdx].stCoolEnd, 
				pstEnv->pstAppCtx->stCurr, llMs);
		}
		break;
	default:
		break;
	}

	// ������ȴ
	if(pstSkillDef->CommCool > 0)
	{
		long long  llTmp;
		struct timeval stDiff;
		
		llMs = pstSkillDef->CommCool*100.0;
		for(i = 0; i < MAX_NORM_SKILL; i++)
		{
			if(!pstMon->astNormalSkill[i].stSkill.SkillID)
			{
				break;
			}

			TV_DIFF(stDiff, pstMon->astNormalSkill[i].stCoolEnd,
			        pstEnv->pstAppCtx->stCurr);
			TV_TO_MS(llTmp, stDiff);
			
			if(llMs > llTmp)
			{
				TV_ADD_MS(pstMon->astNormalSkill[i].stCoolEnd,
				          pstEnv->pstAppCtx->stCurr, llMs);
			}
		}

		for(i = 0; i < MAX_ATKER_SKILL; i++)
		{
			if(!pstMon->astAtkerInsertSkill[i].stSkill.SkillID)
			{
				break;
			}

			TV_DIFF(stDiff, pstMon->astAtkerInsertSkill[i].stCoolEnd,
			        pstEnv->pstAppCtx->stCurr);
			TV_TO_MS(llTmp, stDiff);
			
			if(llMs > llTmp)
			{
				TV_ADD_MS(pstMon->astAtkerInsertSkill[i].stCoolEnd,
				          pstEnv->pstAppCtx->stCurr, llMs);
			}
		}
	}

	return 0;
}

int mon_ai_atk_continue(ZONESVRENV* pstEnv, Monster *pstMon)
{
	if (0 > mon_ai_atk(pstEnv, pstMon, 0))
	{
		/*
		if (MOVE_TYPE_NO == pstMon->bMoveType)
		{
			pstMon->bAiStat = AI_STAT_IDLE;
			return 0;
		}
		else*/
		{
			mon_ai_trace(pstEnv, pstMon, 0);
		}
	}
	
	return 0;
}

int mon_atk_end(ZONESVRENV* pstEnv, Monster *pstMon, SKILLDEF *pstSkillDef, ZoneObjList *pstObjList)
{
	MONSTERDEF *pstMonDef;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (!pstMonDef)
	{
		pstMonAtkInfo->cAtkStat = 0;
		return -1;
	}
		
	switch(pstMonAtkInfo->bSkillType)
	{
		case MON_SKILL_TYPE_NORMAL:
			if (pstMonAtkInfo->bMonSkillIdx < MAX_NORM_SKILL)
			{
				pstMon->astNormalSkill[pstMonAtkInfo->bMonSkillIdx].iOnce = 1;
			}
			break;
		case MON_SKILL_TYPE_INSERT:
			if (pstMonAtkInfo->bMonSkillIdx < MAX_ATKER_SKILL)
			{
				pstMon->astAtkerInsertSkill[pstMonAtkInfo->bMonSkillIdx].iOnce = 1;
			}
			break;
		case MON_SKILL_TYPE_ATKEE:
			if (pstMonAtkInfo->bMonSkillIdx < MAX_ATKEE_SKILL)
			{
				pstMon->astAtkeeInsertSkill[pstMonAtkInfo->bMonSkillIdx].iOnce = 1;
			}
			break;
		default:
			break;	
	}

	z_mon_hit_notify(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, pstSkillDef, pstObjList);
		
	if((pstSkillDef->PilotNum == 0 ||pstMonAtkInfo->cPilotEnd == 1) && pstMonAtkInfo->cAtkStat != CS_STAT_PREPARE)
	{
		pstMonAtkInfo->cAtkStat = 0;

		//��������ֹͣ�����¿ͻ���
		if(pstMonAtkInfo->cPilotEnd ==1)
		{			
			if (pstMon->cMoving)
			{
				z_mon_stop(pstEnv, pstMon); //ͣ����
			}
			z_mon_skill_break(pstEnv, pstMon);
		}
	}

	pstMon->bTraceTry = 0;
	pstMon->bFollowTry = 0;
	
	return 0;
}

int mon_svr_atk(ZONESVRENV* pstEnv, Monster *pstMon, int iSkillID, int iSkillLevel, int iAtkerID)
{
	int iAtkeeRadius = 0;
	ZoneAni stAni;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	ZoneObjList stObjList;
	SKILLDEF *pstSkillDef;

	pstSkillDef = z_find_skill(pstEnv, iSkillID, iSkillLevel, NULL);
	if(NULL == pstSkillDef)
	{
		return -1;
	}

	if(iAtkerID > 0)
	{
		int iIndex = 0;
		char cType = 0;
		TMEMBLOCK *pstMemBlock;
		Player *pstAtkerPlayer;
		//Monster *pstAtkerMon;
		
		iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iAtkerID, &cType);
		if( -1==iIndex )
		{
			return -1;
		}			
		switch(cType)
		{
			case OBJ_PLAYER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					break;
				}
				pstAtkerPlayer = (Player *)pstMemBlock->szData;
				if(pstAtkerPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					return -1;
				}
				
				if(pstAtkerPlayer->stRoleData.Map != pstMon->stMap.iID)
				{
					return -1;
				}
				break;
			case OBJ_MONSTER:
				/*
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					break;
				}
				pstAtkerMon = (Monster *)pstMemBlock->szData;
				if(pstAtkerMon->stMap.iID != pstPlayer->stRoleData.Map)
				{
					return -1;
				}
				break;
				*/
				break;
			default:
				break;
		}
	}


	pstMonAtkInfo->unSkillID = pstSkillDef->SkillID;
	pstMonAtkInfo->bSkillLevel = pstSkillDef->Level;
	pstMonAtkInfo->iSkillIdx = 0;
	pstMonAtkInfo->bSkillType = MON_SKILL_TYPE_ERROR;

	//Ŀ��Ĭ���Լ�
	stAni.iType = OBJ_MONSTER;
	stAni.stObj.pstMon = pstMon;

	iAtkeeRadius = pstMon->unRadius;
	
	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv,  pstMon, 0);
	}

	pstMonAtkInfo->stAtkPos = pstMon->stCurrPos;

	pstMon->iSkillAtkerID = iAtkerID;
	//�������������
	if (SKILL_RANGE_SIGLE == pstSkillDef->MainRange.RangeType || SKILL_RANGE_NONE == pstSkillDef->MainRange.RangeType)
	{
		if( 0 > z_mon_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstMon, &stAni, pstSkillDef,NULL))
		{
			goto _error;
		}
		
		stObjList.iObj = 1;
		stObjList.astObj[0] = stAni;
	}
	else
	{
		MONSTERDEF *pstMonDef;
				
		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if (!pstMonDef)
		{
			pstMonAtkInfo->cAtkStat = 0;
			goto _error;
		}

		z_mon_get_atk_list(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, pstSkillDef, &stAni, &pstMonAtkInfo->stAtkPos, &stObjList, 0);
		if (stObjList.iObj == 0 && pstSkillDef->PrepareTime == 0)
		{
			pstMonAtkInfo->cAtkStat = 0;
			goto _error;
		}
	}

	if (pstSkillDef->AttackActTime > 0 && (pstMon->tFreezeEnd < pstEnv->pstAppCtx->stCurr.tv_sec + pstSkillDef->AttackActTime/10))
	{
		pstMon->tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstSkillDef->AttackActTime/10;
	}

	mon_atk_end(pstEnv, pstMon, pstSkillDef, &stObjList);
	pstMon->iSkillAtkerID = 0;
	
	return 0;
_error:
	pstMon->iSkillAtkerID = 0;
	return 0;
}


int mon_prepare_end(ZONESVRENV* pstEnv, Monster *pstMon, SKILLDEF *pstSkillDef)
{
	int iAtkeeRadius = 0;
	Player *pstPlayerAtkee;
	Monster *pstMonAtkee;
	Npc *pstNpc;
	ZoneAni stAni;
	int iIndex;
	char cType;
	RESPOS stPos;
	TMEMBLOCK *pstMemBlock;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	ZoneObjList stObjList;
	char cOldFlag = pstMon->cMonMiscFlag;

	pstMonAtkInfo->cAtkStat = CS_STAT_ATK;

	if( pstSkillDef->SkillType == SKILL_TYPE_PREPARE_ATK )
	{
		return z_mon_skill_break(pstEnv, pstMon);
	}

	//�������ܼ��
	if(pstMonAtkInfo->unPilot)
	{
		long long  llMs;
		struct timeval stDiff;		
		
		TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstMonAtkInfo->stLastPilot);
		TV_TO_MS(llMs, stDiff);
		if (llMs < pstSkillDef->PilotTime*100.0)
		{
			return -1;
		}
	}

	if ((pstSkillDef->SkillType != SKILL_TYPE_SPECIFY_AREA_ATK) )
	{
		if (0 >= pstMonAtkInfo->iAtkeeID)
		{
			return z_mon_skill_break(pstEnv, pstMon);
		}
		
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstMonAtkInfo->iAtkeeID, &cType);
		if (0 > iIndex)
		{
			return z_mon_skill_break(pstEnv, pstMon);
		}

		if (cType == OBJ_PLAYER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				return z_mon_skill_break(pstEnv, pstMon);
			}
			
			pstPlayerAtkee = (Player *)pstMemBlock->szData;

			if (pstPlayerAtkee->stOnline.cMoving)
			{
				z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayerAtkee, 0);
			}

			stPos = pstPlayerAtkee->stRoleData.Pos;
			stAni.iType = OBJ_PLAYER;
			stAni.stObj.pstPlayer = pstPlayerAtkee;
		}
		else if (cType == OBJ_MONSTER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				return z_mon_skill_break(pstEnv, pstMon);
			}
			
			pstMonAtkee = (Monster *)pstMemBlock->szData;

			if (pstMonAtkee->cMoving)
			{
				z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMonAtkee, 0);
			}

			if(pstEnv->pstAppCtx->stCurr.tv_sec < pstMonAtkee->tBirthEnd)
			{
				return z_mon_skill_break(pstEnv, pstMon);
			}

			stPos = pstMonAtkee->stCurrPos;
			stAni.iType = OBJ_MONSTER;
			stAni.stObj.pstMon = pstMonAtkee;

			iAtkeeRadius = pstMonAtkee->unRadius;
		}
		else if (cType == OBJ_NPC)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				return z_mon_skill_break(pstEnv, pstMon);
			}
			pstNpc = (Npc *)pstMemBlock->szData;
			if (pstNpc->stMon.cMoving)
			{
				z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon, 0);
			}

			if(pstEnv->pstAppCtx->stCurr.tv_sec < pstNpc->stMon.tBirthEnd)
			{
				return z_mon_skill_break(pstEnv, pstMon);
			}

			stPos = pstNpc->stMon.stCurrPos;
			stAni.iType = OBJ_NPC;
			stAni.stObj.pstNpc= pstNpc;

			iAtkeeRadius = pstNpc->stMon.unRadius;
		}
		else
		{
			return -1;
		}

		pstMonAtkInfo->stAtkPos = stPos;
	}
	else
	{
		if (pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE)
			return -1;
		stPos = pstMonAtkInfo->stAtkPos;
	}

	if (pstMon->cMoving)
	{
		if (0 == pstSkillDef->MoveAttack)
		{
			z_mon_stop(pstEnv, pstMon); //ͣ�����ٴ�
		}
		else
		{
			z_monster_move_curr(pstEnv->pstAppCtx, pstEnv,  pstMon, 0);
		}
	}

	if (pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE)
	{
		if (z_distance(&pstMon->stCurrPos, &stPos)  > pstSkillDef->Distance+pstMon->unRadius+iAtkeeRadius)
		{
			return z_mon_skill_break(pstEnv, pstMon);
		}
	}
	else
	{
		int iDir = 0;

		if(pstSkillDef->TargetRange == TARGET_RANGE_DIRECTION)
		{
			iDir = pstMon->bDirection;
		}

		if(pstSkillDef->Distance <= 0 && 
			pstMon->stCurrPos.X == pstMonAtkInfo->stAtkPos.X &&
			pstMon->stCurrPos.Y == pstMonAtkInfo->stAtkPos.Y)
		{

		}
		else
		{
			if ( 0 > z_in_range(&stPos, &pstMon->stCurrPos, &pstMonAtkInfo->stAtkPos, 
								&pstSkillDef->MainRange, pstSkillDef->Distance, iAtkeeRadius, iDir) &&
								pstSkillDef->PrepareTime == 0)
			{
				return z_mon_skill_break(pstEnv, pstMon);
			}
		}
	}

	if (SKILL_RANGE_SIGLE == pstSkillDef->MainRange.RangeType || SKILL_RANGE_NONE == pstSkillDef->MainRange.RangeType)
	{
		stObjList.iObj = 1;
		stObjList.astObj[0] = stAni;
	}
	else
	{
		MONSTERDEF *pstMonDef;
				
		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if (!pstMonDef)
		{
			pstMonAtkInfo->cAtkStat = 0;
			z_mon_skill_break(pstEnv, pstMon);
			return -1;
		}

		z_mon_get_atk_list(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, pstSkillDef, &stAni, &pstMonAtkInfo->stAtkPos, &stObjList, 0);
		if ( !(pstSkillDef->TargetType&SKILL_TARGET_NO_PLAYER) && 
			 stObjList.iObj == 0 && 
			 pstSkillDef->PrepareTime == 0 && 
			 pstSkillDef->PilotNum == 0 && 
			 pstMon->bSubType != MON_SUB_TYPE_BUILD)
		{
			pstMonAtkInfo->cAtkStat = 0;
			z_mon_skill_break(pstEnv, pstMon);
			return -1;
		}
	}

	TV_CLONE(pstMonAtkInfo->stLastAtk, pstEnv->pstAppCtx->stCurr);
	pstMon->cMonMiscFlag |= MON_MISC_FLAG_RESTORE_HP;

	//֮ǰûս��,����ս����
	if (!(cOldFlag & MON_MISC_FLAG_RESTORE_HP))
	{
		//����ս������
		z_mon_speech_begin_or_end_fight(pstEnv,pstMon,SPEECH_MON_BEGIN_FIGHT);
	}

	if(pstSkillDef->PilotNum > 0)
	{
		if (0 == pstMonAtkInfo->unPilot)
		{
			pstMonAtkInfo->cPilotEnd = 0;
			mon_skill_begin_cool(pstEnv, pstMon);
		}
		
		pstMonAtkInfo->unPilot++;
		TV_CLONE(pstMonAtkInfo->stLastPilot, pstEnv->pstAppCtx->stCurr);

		if (pstMonAtkInfo->unPilot >= pstSkillDef->PilotNum)
		{
			pstMonAtkInfo->cPilotEnd = 1;
			pstMonAtkInfo->unPilot = 0;
		}
	}
	
	if(!pstMon->stMonAtkInfo.fIsSpecificTarget)
	{
		if(pstSkillDef->PilotNum == 0)
		{
			// ָ���ض�Ŀ��ʩ�ŵļ����Ѿ�����ȴ����
			mon_skill_begin_cool(pstEnv, pstMon);
		}
	}
	else
	{
		// ȡ�������־
		pstMon->stMonAtkInfo.fIsSpecificTarget = 0;
	}

	if (pstSkillDef->AttackActTime > 0 && (pstMon->tFreezeEnd < pstEnv->pstAppCtx->stCurr.tv_sec + pstSkillDef->AttackActTime/10))
	{
		pstMon->tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstSkillDef->AttackActTime/10;
	}

	//�����������̿����ƶ�
	if(pstSkillDef->MoveAttack && pstMonAtkInfo->unPilot > 0)
	{
		pstMonAtkInfo->cAtkStat = CS_STAT_MOVE_ATK;

		//��һ�β�ǿ����
		if(pstMonAtkInfo->unPilot == 1)
		{
			if (	pstMon->stMonAtkInfo.stMovePos.X > 0 && 
				pstMon->stMonAtkInfo.stMovePos.Y > 0)
			{
				z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
				
				pstMon->stNextPos = pstMon->stMonAtkInfo.stMovePos;
		 		pstMon->stForcedPos = pstMon->stMonAtkInfo.stMovePos;

				pstMon->bMoveType = MOVE_TYPE_FORCED; // ǿ���ƶ�
				pstMon->cMoving = 1;
				TV_CLONE(pstMon->stLastMoveTv, pstEnv->pstAppCtx->stCurr);
				z_monster_move_notify(pstEnv->pstAppCtx, pstEnv, pstMon, CS_MOVE_WALK);
			}
		}
	}
	
	return mon_atk_end(pstEnv, pstMon, pstSkillDef, &stObjList);
}

int mon_ai_specify_area_atk_pos(ZONESVRENV* pstEnv, Monster *pstMon,MonAtkInfo *pstMonAtkInfo, SKILLDEF *pstSkillDef, RESPOS*pstPos)
{
	SKILLRESULT *pstResult;
	int iDist;
	int iRadius;
	int iVPlayer = 0;
	int aiVPlayer[MAX_MONSTER_VPLAYER];
	int i,j;
	Player *pstPlayer;
	int iMaxPressID;
	RESPOS stPos_rotate;
	
	TMAPMASK *pstMask;
	MapIndex *pstMapIdx;
	ZONEMAPINST *pstMapInst;
	int iPlayerToMonDist = 0;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);;
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	

	pstResult = z_skill_get_result(pstSkillDef, RESULT_SPECIFY_AREA_ATK);
	if (!pstResult)
	{
		return -1;
	}

	iDist = pstResult->SkillResultVal2;		// ǿ���ƶ������ʩ������
	iRadius = pstSkillDef->MainRange.RangeVal1;
	
	if(pstResult->SkillResultVal1 == AREA_ATK_TYPE_SELF)
	{
		*pstPos = pstMon->stCurrPos;
	}
	// ���
	else if (pstResult->SkillResultVal1 == AREA_ATK_TYPE_RAND)
	{
		for (i=0; i<pstMon->iVPlayer; i++)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->aiVPlayer[i]);
			if (	pstPlayer && 
				z_distance(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos) < (iDist+pstMon->unRadius))
			{
				if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y))
				{
					continue;
				}
				
				aiVPlayer[iVPlayer++] = pstMon->aiVPlayer[i];
			}			
		}

		if (iVPlayer > 0)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, aiVPlayer[(int)RAND1(iVPlayer)]);
			if (pstPlayer)
			{
				*pstPos = pstMonAtkInfo->stAtkPos = pstPlayer->stRoleData.Pos;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	// �˶�
	else if(pstResult->SkillResultVal1 == AREA_ATK_TYPE_ROLE_MORE ||
			pstResult->SkillResultVal1 == AREA_ATK_TYPE_MACHINE	)
	{
		int iPlayerRound = 0;
		int iPlayerIdx = -1;
		int iCount = 0;
		
		int iMachine = 0;
		int iMachinePlayerRound = 0;
		int iMachinePlayerIdx = -1;
		
		Player *pstTmpPlayer = NULL;
		
		for (i=0; i<pstMon->iVPlayer; i++)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->aiVPlayer[i]);
			if (pstPlayer && z_distance(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos) <= (iDist+pstMon->unRadius))
			{
				if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y))
				{
					continue;
				}

				if( pstTmpPlayer && machine_get_by_player(pstEnv, pstPlayer))
				{
					iMachine = 1;
				}
				else
				{
					iMachine = 0;
				}
				iCount = 1;
				
				for(j=0; j<pstMon->iVPlayer; j++)
				{
					if (i==j)
					{
						continue;
					}

					pstTmpPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->aiVPlayer[j]);
					if (pstTmpPlayer && (z_distance(&pstPlayer->stRoleData.Pos, &pstTmpPlayer->stRoleData.Pos) <= iRadius))
					{
						iCount ++;
					}
					
					if( pstTmpPlayer && machine_get_by_player(pstEnv, pstPlayer))
					{
						iMachine++;
					}
				}

				if (iPlayerRound < iCount)
				{
					iPlayerRound = iCount;
					iPlayerIdx = i;
				}

				if (iMachinePlayerRound < iMachine)
				{
					iMachinePlayerRound = iMachine;
					iMachinePlayerIdx = i;
				}
				
			}
		}

		// ��е���ȵ�
		if(pstResult->SkillResultVal1 == AREA_ATK_TYPE_MACHINE &&
			iMachinePlayerIdx >=0 )
		{
			iPlayerIdx = iMachinePlayerIdx;
		}
		
		if (iPlayerIdx >= 0)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->aiVPlayer[iPlayerIdx]);	
			if (pstPlayer)
			{
				*pstPos = pstMonAtkInfo->stAtkPos = pstPlayer->stRoleData.Pos;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	else if(pstResult->SkillResultVal1 == AREA_ATK_TYPE_BACKHIT )
	{
		iMaxPressID = mon_press_max(pstEnv->pstAppCtx, pstEnv, pstMon, NULL);
		if (iMaxPressID > 0)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iMaxPressID);
			if (pstPlayer )
			{
				if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y))
				{
					return -1;
				}
				
				iPlayerToMonDist =  z_distance(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos);
				if (iPlayerToMonDist <(iDist+pstMon->unRadius))
				{
					*pstPos = pstMonAtkInfo->stAtkPos = pstPlayer->stRoleData.Pos;
				}
				else
				{
					pstPos->X = pstMon->stCurrPos.X + 
								(1.0*iDist/iPlayerToMonDist*(pstPlayer->stRoleData.Pos.X-pstMon->stCurrPos.X));
					pstPos->Y = pstMon->stCurrPos.Y + 
								(1.0*iDist/iPlayerToMonDist*(pstPlayer->stRoleData.Pos.Y -pstMon->stCurrPos.Y));
						
				}

				//�Ȱѹ���ĳ����ס
				pstMonAtkInfo->DirFace = pstResult->SkillResultVal3;
				
				//�ѹ�����������ת
				stPos_rotate = z_rotate_pos(pstMon->stCurrPos,pstResult->SkillResultVal3,*pstPos);
				iPlayerToMonDist = z_distance(&stPos_rotate, &pstMon->stCurrPos);
				//����������
				pstPos->X = pstMon->stCurrPos.X + 
							( (1.0*pstResult->SkillResultVal4/100.0) * (stPos_rotate.X-pstMon->stCurrPos.X));
				pstPos->Y = pstMon->stCurrPos.Y + 
							( (1.0*pstResult->SkillResultVal4/100.0) * (stPos_rotate.Y -pstMon->stCurrPos.Y));
				
				if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, pstPos->X, pstPos->Y))
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	// ָ��Ŀ��
	else
	{
		iMaxPressID = mon_press_max(pstEnv->pstAppCtx, pstEnv, pstMon, NULL);
		if (iMaxPressID > 0)
		{
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iMaxPressID);
			if (pstPlayer )
			{
				if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y))
				{
					return -1;
				}
				
				iPlayerToMonDist =  z_distance(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos);
				if (iPlayerToMonDist <(iDist+pstMon->unRadius))
				{
					*pstPos = pstMonAtkInfo->stAtkPos = pstPlayer->stRoleData.Pos;
				}
				else
				{
					pstPos->X = pstMon->stCurrPos.X + 
								(1.0*iDist/iPlayerToMonDist*(pstPlayer->stRoleData.Pos.X-pstMon->stCurrPos.X));
					pstPos->Y = pstMon->stCurrPos.Y + 
								(1.0*iDist/iPlayerToMonDist*(pstPlayer->stRoleData.Pos.Y -pstMon->stCurrPos.Y));		
				}
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	return 0;
}

int mon_ai_start_atk(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAtkee)
{
	CSPKG stPkg;
	SKILLDEF *pstSkillDef;
	int iMs;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	RESPOS stPos;

	/*
	pstMon->bTraceTry = 0;
	pstMon->bFollowTry = 0;
	*/

	//������������Ϣ
	pstMonAtkInfo->unPilot = 0;
	pstMonAtkInfo->cPilotEnd = 0;

	pstSkillDef = z_find_skill(pstEnv, pstMonAtkInfo->unSkillID, pstMonAtkInfo->bSkillLevel, &pstMonAtkInfo->iSkillIdx);
	if (NULL == pstSkillDef)
	{
		pstMonAtkInfo->cAtkStat = 0;
		return -1;
	}
	/*if(pstSkillDef->SkillID == 10281 )
	{
		int test = 0;
		test = 0;
	}*/

	if (z_is_mon_status_result(pstEnv, pstMon, STATUS_RESULT_NOSKILL) && pstSkillDef->IgnoreNoSkill == 0)
	{
		return -1;
	}

	if(pstMon->stMonAtkInfo.fIsSpecificTarget & SPECIFIC_TARGET_HAVE )
	{
		//�����Ѿ�������Ŀ����
	}
	//else if( pstMon->stMonAtkInfo.fIsSpecificTarget & SPECIFIC_TARGET_NO )
	else if( pstMon->stMonAtkInfo.fIsSpecificTarget & SPECIFIC_TARGET_NO ||
		 	 !!mon_will_atk_specific_target(pstEnv, pstMon) )
	{
		SKILLRESULT *pstSkillResult = NULL;
		pstMon->stMonAtkInfo.fIsSpecificTarget = 
				!!mon_will_atk_specific_target(pstEnv, pstMon);
		 pstSkillResult= mon_will_atk_specific_target(
			pstEnv,
			pstMon);
		if(!pstSkillResult)
		{
			return -1;
		}

		// ���ּ�������,����ȴ
		mon_skill_begin_cool(pstEnv, pstMon);

		// ѡĿ��
		if(mon_ai_select_specific_target(pstEnv, pstMon, pstSkillResult) < 0)
		{
			z_mon_skill_break(pstEnv, pstMon);
			return -1;
		}
	}
	// ָ���㷶Χ����
	else if (SKILL_TYPE_SPECIFY_AREA_ATK == pstSkillDef->SkillType)
	{
		if (mon_ai_specify_area_atk_pos(pstEnv, pstMon,pstMonAtkInfo, pstSkillDef, &stPos) < 0)
		{
			return -1;
		}
		else
		{
			pstMonAtkInfo->stAtkPos = stPos;
		}
	}
	else 
	{
		if (pstSkillDef->Distance <= 0) //�������ڵ�Ϊ������
		{
			pstMonAtkInfo->iAtkeeID = pstMon->iID;
			pstMonAtkInfo->stAtkPos = pstMon->stCurrPos;
		}
		else if(pstAtkee)
		{
			if (pstAtkee->iType == OBJ_PLAYER)
			{
				pstMonAtkInfo->iAtkeeID = 
					pstAtkee->stObj.pstPlayer->iMemID;
				pstMonAtkInfo->stAtkPos =
					pstAtkee->stObj.pstPlayer->stRoleData.Pos;
			}
			else if (pstAtkee->iType == OBJ_MONSTER)
			{
				pstMonAtkInfo->iAtkeeID = pstAtkee->stObj.pstMon->iID;
				pstMonAtkInfo->stAtkPos = pstAtkee->stObj.pstMon->stCurrPos;
			}
			else if (pstAtkee->iType == OBJ_NPC)
			{
				pstMonAtkInfo->iAtkeeID = pstAtkee->stObj.pstNpc->stMon.iID;
				pstMonAtkInfo->stAtkPos =  pstAtkee->stObj.pstNpc->stMon.stCurrPos;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}

	if (z_skill_get_result(pstSkillDef, RESULT_POLIT_MOVE_ATK))
	{
		if (mon_ai_specify_area_atk_pos(pstEnv, pstMon,pstMonAtkInfo, pstSkillDef, &stPos) < 0)
		{
			return -1;
		}
		pstMonAtkInfo->stMovePos = stPos;
	}
	else
	{
		if (pstMon->cMoving)
		{
			z_monster_move_curr(pstEnv->pstAppCtx, pstEnv,  pstMon, 0);
		}
	}
	
	if (pstSkillDef->PrepareTime > 0)
	{
		CSSKILLPRES *pstPrepareS;

		if (pstMon->cMoving)
		{
			z_mon_stop(pstEnv, pstMon); //�´�ͣ�����ٴ�
		}

		pstPrepareS = &stPkg.Body.SkillPreS;
		memset(pstPrepareS,0,sizeof(CSSKILLPRES));
		
		pstMonAtkInfo->cAtkStat = CS_STAT_PREPARE;
		iMs = pstSkillDef->PrepareTime*100.0;
		TV_ADD_MS(pstMonAtkInfo->stAtkStatEnd, pstEnv->pstAppCtx->stCurr, iMs);

		pstPrepareS->ID = pstMon->iID;
		pstPrepareS->SkillID = pstSkillDef->SkillID;
		pstPrepareS->SkillLevel = pstSkillDef->Level;
		pstPrepareS->AtkPos = pstMonAtkInfo->stAtkPos;
		
		/*
		if (pstSkillDef->SkillType == SKILL_TYPE_SPECIFY_AREA_ATK)
		{
			pstPrepareS->AtkeeID = 0;
		}
		else*/
		{
			pstPrepareS->AtkeeID = pstMonAtkInfo->iAtkeeID;
		}
		
		if( pstSkillDef->SkillType == SKILL_TYPE_PREPARE_ATK )
		{
			MapIndex *pstMapIdx = NULL;
			ZONEMAPINST *pstMapInst = NULL;
			int iDist = 0; 
			RESPOS stPos;

			memset(&stPos,0,sizeof(stPos));
			
			pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
			if (NULL == pstMapInst)
			{
				return -1;
			}
			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
			if (NULL == pstMapIdx)
			{
				return -1;
			}
	
			//�ѵ�ƫ��5��
			iDist = z_distance(&pstMon->stCurrPos, &pstMonAtkInfo->stAtkPos);
			z_get_clockwise_pos( pstEnv, pstMapInst, pstMapIdx, pstSkillDef,
							&pstMon->stCurrPos, &pstMonAtkInfo->stAtkPos, iDist, iDist,6, &stPos);

			pstPrepareS->AtkPos = stPos;
			pstMonAtkInfo->stAtkPos = stPos;
            mon_skill_begin_cool(pstEnv, pstMon);
		}

		Z_CSHEAD_INIT(&stPkg.Head, SKILL_PREPARE_SVR);
		z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);
		
		return 0;
	}

	return mon_prepare_end(pstEnv, pstMon, pstSkillDef);
}

int mon_ai_select_target(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int iTarget;
	int iIdx;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	iTarget = mon_press_max(pstEnv->pstAppCtx, pstEnv, pstMon, &iIdx);
	if (0 > iTarget)
	{
		return -1;
	}

	if (iTarget != pstTargetList->iTargetCur) //�״�ѡ��
	{
		pstTargetList->iTargetCur = iTarget;
		if(iIdx >= 0)
		{
			pstTargetList->astPressEntrys[iIdx].iPress += pstMon->unPressSelectAdd;
		}
	}

	pstTargetList->tTargetSelect = pstEnv->pstAppCtx->stCurr.tv_sec;

	return iTarget;
}

int mon_ai_atk(ZONESVRENV* pstEnv, Monster *pstMon, int iDelay)
{
	Player *pstPlayerAtkee;
	Monster *pstMonAtkee;
	Npc *pstNpc;
	int iIndex;
	char cType;
	struct timeval stDiff;
	long long llMs;
	int iHaveSkill;
	RESPOS stPos;
	ZoneAni stAtkee;
	TMEMBLOCK *pstMemBlock;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iAtkeeRadius = 0;
	char cOldFlag = pstMon->cMonMiscFlag;

	if(pstMon->bMoveType == MOVE_TYPE_FORCED)
	{
		return 0;
	}

	if (0 != pstMonAtkInfo->cAtkStat)
	{
		return 0;
	}

	if (0 >= pstTargetList->iTargetCur)
	{
		if (0 > mon_ai_select_target(pstEnv, pstMon))
		{
			return -1;
		}
	}

	iIndex = z_id_indextype(NULL, pstEnv, pstTargetList->iTargetCur, &cType);
	if (0 > iIndex)
	{
		mon_ai_select_target(pstEnv, pstMon);
		pstMon->bAiStat = AI_STAT_TRACE;
		return -1;
	}

	if (cType == OBJ_PLAYER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
	       if (!pstMemBlock->fValid)
	       {
	           return -1;
	       }
		pstPlayerAtkee = (Player *)pstMemBlock->szData;

		if (pstPlayerAtkee->stOnline.State & CS_STAT_DEAD)
		{
			mon_press_delete(pstMon, pstPlayerAtkee->iMemID);
			mon_ai_evt_outpress(pstEnv, pstMon);
			
			if (0 > mon_ai_select_target(pstEnv, pstMon))
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}

		if (pstPlayerAtkee->stOnline.cMoving)
		{
			z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayerAtkee, 0);
		}

		stPos = pstPlayerAtkee->stRoleData.Pos;
		stAtkee.iType = OBJ_PLAYER;
		stAtkee.stObj.pstPlayer = pstPlayerAtkee;
	}
	else if (cType == OBJ_MONSTER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
	       if (!pstMemBlock->fValid)
	       {
	           return -1;
	       }
		pstMonAtkee = (Monster *)pstMemBlock->szData;	

		if (pstMonAtkee->cStat == MONSTER_DIE)
		{
			mon_press_delete(pstMon, pstMonAtkee->iID);
			mon_ai_evt_outpress(pstEnv, pstMon);
			
			if (0 > mon_ai_select_target(pstEnv, pstMon))
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}

		if (pstMonAtkee->cMoving)
		{
			z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMonAtkee, 0);
		}

		stPos = pstMonAtkee->stCurrPos;
		stAtkee.iType = OBJ_MONSTER;
		stAtkee.stObj.pstMon = pstMonAtkee;
		iAtkeeRadius = pstMonAtkee->unRadius;
	}
	else if (cType == OBJ_NPC)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
	       if (!pstMemBlock->fValid)
	       {
	           return -1;
	       }
		pstNpc = (Npc *)pstMemBlock->szData;	

		if (pstNpc->stMon.cStat == MONSTER_DIE)
		{
			mon_press_delete(pstMon, pstNpc->stMon.iID);
			mon_ai_evt_outpress(pstEnv, pstMon);
			
			if (0 > mon_ai_select_target(pstEnv, pstMon))
			{
				return -1;
			}
			else
			{
				return 0;
			}
		}

		if (pstNpc->stMon.cMoving)
		{
			z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, &pstNpc->stMon, 0);
		}

		stPos = pstNpc->stMon.stCurrPos;
		stAtkee.iType = OBJ_NPC;
		stAtkee.stObj.pstNpc = pstNpc;
		iAtkeeRadius = pstNpc->stMon.unRadius;
	}
	else
	{
		return -1;
	}

	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
	}

	
	pstMon->cMonMiscFlag |= MON_MISC_FLAG_RESTORE_HP;

	//֮ǰûս��,����ս����
	if (!(cOldFlag & MON_MISC_FLAG_RESTORE_HP))
	{
		//����ս������
		z_mon_speech_begin_or_end_fight(pstEnv,pstMon,SPEECH_MON_BEGIN_FIGHT);
	}
	
	TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstMonAtkInfo->stLastAtk);
	TV_TO_MS(llMs, stDiff);
	if (llMs < pstMon->unMinAtkSkillCool*100.0)
	{
		if (z_distance(&pstMon->stCurrPos, &stPos) <= pstMon->unMinAtkSkillDist+iAtkeeRadius) //�ȴ��´ι���
		{
			pstMon->bAiStat = AI_STAT_TRACE;
			return 0;
		}
		else //׷��
		{
			return -1;
		}
	}

	pstMonAtkInfo->unSkillID = 0;
	mon_select_atkskill(pstEnv, pstMon, &stAtkee, &iHaveSkill);

	if (iDelay && pstMon->bSubType != MON_SUB_TYPE_TRAP)
	{
		if (iHaveSkill) //�ȴ��´ι���
		{
			if (pstMonAtkInfo->unSkillID > 0)
			{
				pstMon->stMonAtkInfo.bNextSkillIdx = 0;
				pstMon->stMonAtkInfo.bHeadSkillIdx = 0;
			}
	
			pstMon->bAiStat = AI_STAT_TRACE;
			return 0;
		}
		else //׷��
		{
			return -1;
		}
	}
	
	if (pstMonAtkInfo->unSkillID > 0)
	{
		pstMon->bAiStat = AI_STAT_TRACE;
		
		mon_ai_start_atk(pstEnv, pstMon, &stAtkee);
		if( pstMon->bAiStat != AI_STAT_HOME && (pstMonAtkInfo->cAtkStat == CS_STAT_PREPARE || pstMonAtkInfo->unPilot > 0))
		{
			pstMon->bAiStat = AI_STAT_ATK;
		}
		
		return 0;
	}
	else
	{
		if (iHaveSkill) //�ȴ��´ι���
		{
			pstMon->bAiStat = AI_STAT_TRACE;
			return 0;
		}
		else //׷��
		{
			return -1;
		}
	}

	return 0;
}

int z_get_follow_angle(int iAngelMin, int iAngelMax, unsigned char bFollowAngel,	unsigned char *pbFollowAngels)
{
	int i, j;
	int iScore;
	int iScoreMax = -1;
	int iAngel;
	int iAngelLast = -1;

	UNUSED(iAngelMax);

	for (i=0; i<13; i++)
	{
		iAngel = iAngelMin + i*10;
		if (iAngel >= 360)
		{
			iAngel -= 360;
		}
		else if (iAngel < 0)
		{
			iAngel += 360;
		}
		
		iScore = 0;

		for (j=0; j<bFollowAngel; j++)
		{
			iScore += abs(pbFollowAngels[j]*2 - iAngel);
		}

		if (iScore > iScoreMax)
		{
			iScoreMax = iScore;
			iAngelLast = iAngel;
		}
	}

	return iAngelLast;
}

int z_get_follow_player_pos(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst , MapIndex * pstMapIdx, Monster *pstMon, Player *pstPlayer,  RESPOS *pstNextPos)
{
	TMAPMASK *pstMask;
	int iDist, iDistMax, iDistMin;
	int iFaceAngel;
	int i, j;
	int iAngel;
	unsigned char *pbFollowAngel;
	RESPOS *pstOriPos = &pstMon->stCurrPos;
	RESPOS *pstTargetPos = &pstPlayer->stRoleData.Pos;
	int iSkillDist = get_mon_atk_skill_dist(pstEnv, pstMon);;
			
	if ( !pstMapIdx)
	{
		memcpy(pstNextPos, pstOriPos, sizeof(*pstNextPos));

		return -1;
	}

	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	if (pstPlayer->stOnline.cMoving)
	{
		iDistMax = iSkillDist*0.5;
		iDistMin = iSkillDist*0.3;
	}
	else
	{
		iDistMax = iSkillDist*0.9;
		iDistMin = iSkillDist*0.7;
	}

	for (i=0; i<3; i++)
	{
		iDist = iDistMin + (RAND1(iDistMax-iDistMin));
		
		iFaceAngel = FaceAngleBetween(pstOriPos->X, pstOriPos->Y, pstTargetPos->X, pstTargetPos->Y);
		if (iFaceAngel >= 360)
		{
			iFaceAngel -= 360;
		}

		if(pstPlayer->stOnline.cMoving && z_distance(pstTargetPos, pstOriPos) > mon_have_movespd(pstMon))
		{
			iAngel = iFaceAngel;
		}
		else
		{
			pbFollowAngel = &pstPlayer->stOnline.abFollowAngel[iFaceAngel/60];
			iAngel = ((*pbFollowAngel)%6)*10 + ((*pbFollowAngel)/6)*5 + (iFaceAngel/60)*60;
			(*pbFollowAngel)++;
			if ((*pbFollowAngel) > 11)
			{
				(*pbFollowAngel) = 0;
			}
		}
				
		for (j=0; j<3; j++)
		{
			iDist = iDist >> j;
			pstNextPos->X		=	(int) (pstTargetPos->X - iDist * cos((iAngel)*PI/180.0f));
			pstNextPos->Y		=	(int) (pstTargetPos->Y + iDist * sin((iAngel)*PI/180.0f));

			if (IsPixelLineThrough(pstMask, pstOriPos->X, pstOriPos->Y, pstNextPos->X, pstNextPos->Y, maskStop) &&
				IsPixelLineThrough(pstMask, pstNextPos->X, pstNextPos->Y, pstTargetPos->X, pstTargetPos->Y, maskStop) &&
				dyn_msk_check(pstEnv, pstMapInst, pstMask, pstOriPos->X, pstOriPos->Y, 
								pstNextPos->X, pstNextPos->Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
				z_pos_area(pstEnv->pstMapObj, pstMapIdx, pstNextPos)) 
			{
				break;
			}
			else if (/*pstMon->bFollowTry > FOLLOW_MSK  && */
				!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstOriPos->X, pstOriPos->Y,
									pstNextPos->X, pstNextPos->Y,z_get_monsterdef_misctype(pstEnv,pstMon), DYN_MSK_STOP) )
			{
				//�ж�̬�ϰ���Ҫ�����
				if(0 == mon_press_target_dynmsk(pstEnv->pstAppCtx, pstEnv, pstMapInst, pstMon,&pstPlayer->stRoleData.Pos))
				{
					return -1;
				}
			}
			
			if (pstMon->bFollowTry > FOLLOW_TRY) //ǿ�ƴ�Խ
			{
				pstNextPos->X = pstTargetPos->X;
				pstNextPos->Y = pstTargetPos->Y;
				if (0 == IsPixelStop(pstMask, pstNextPos->X, pstNextPos->Y) && 
					dyn_msk_check(pstEnv, pstMapInst, pstMask, pstNextPos->X, pstNextPos->Y,
									pstNextPos->X, pstNextPos->Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
				       z_pos_area(pstEnv->pstMapObj, pstMapIdx, pstNextPos))
				{
					break;
				}
			}
		}

		if (j < 3)
		{
			break;
		}
	}

	if (i == 3)
	{
		return -1;
	}

	return 0;
}

int mon_press_add_by_memid(ZONESVRENV* pstEnv, Monster *pstMon, int iTargetID, int iPress)
{
	int iIdx = 0;
	AtkTargetList *pstAtkTarget = NULL;

	pstAtkTarget = &pstMon->stAtkTargets;
	
	iIdx = mon_press_find(pstMon, iTargetID);
	if (iIdx < 0)
	{
		iIdx = mon_press_insert(pstEnv, pstMon, iTargetID, iPress);
	}
	
	if( iIdx >=0 )
	{
		if( MAX_PRESSVAL - iPress > pstAtkTarget->astPressEntrys[iIdx].iPress)
		{
			pstAtkTarget->astPressEntrys[iIdx].iPress += iPress;
		}
	}
	else
	{
		return -1;
	}

	return 0;
}

int z_get_follow_mon_pos(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst ,MapIndex * pstMapIdx, Monster *pstMon, Monster *pstTraceMon,  RESPOS *pstNextPos)
{
	TMAPMASK *pstMask;
	int iDist, iDistMax, iDistMin;
	int iFaceAngel, iAngel;
	int i, j;
	RESPOS *pstOriPos = &pstMon->stCurrPos;
	RESPOS *pstTargetPos = &pstTraceMon->stCurrPos;
	int iSkillDist = get_mon_atk_skill_dist(pstEnv, pstMon) + pstTraceMon->unRadius;
			
	if ( !pstMapIdx)
	{
		memcpy(pstNextPos, pstOriPos, sizeof(*pstNextPos));

		return -1;
	}

	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	if (pstTraceMon->cMoving)
	{
		iDistMax = iSkillDist*0.6;
		iDistMin = iSkillDist*0.3;
	}
	else
	{
		iDistMax = iSkillDist*0.9;
		iDistMin = iSkillDist*0.7;
	}
	
	for (i=0; i<3; i++)
	{
		iDist = iDistMin + (int)RAND1(iDistMax-iDistMin+1);
		
		iFaceAngel = FaceAngleBetween(pstOriPos->X, pstOriPos->Y, pstTargetPos->X, pstTargetPos->Y);
		if (iFaceAngel >= 360)
		{
			iFaceAngel -= 360;
		}

		if(pstTraceMon->cMoving && z_distance(pstTargetPos, pstOriPos) > mon_have_movespd(pstMon))
		{
			iAngel = iFaceAngel;
		}
		else
		{
			unsigned char *pbFollowAngel;
			
			pbFollowAngel = &pstTraceMon->abFollowAngel[iFaceAngel/60];
			iAngel = ((*pbFollowAngel)%6)*10 + ((*pbFollowAngel)/6)*5 + (iFaceAngel/60)*60;
			(*pbFollowAngel)++;
			if ((*pbFollowAngel) > 11)
			{
				(*pbFollowAngel) = 0;
			}
		}
				
		for (j=0; j<3; j++)
		{
			iDist = iDist >> j;
			pstNextPos->X		=	(int) (pstTargetPos->X - iDist * cos((iAngel)*PI/180.0f));
			pstNextPos->Y		=	(int) (pstTargetPos->Y + iDist * sin((iAngel)*PI/180.0f));

			if (IsPixelLineThrough(pstMask, pstOriPos->X, pstOriPos->Y, pstNextPos->X, pstNextPos->Y, maskStop) &&
				IsPixelLineThrough(pstMask, pstNextPos->X, pstNextPos->Y, pstTargetPos->X, pstTargetPos->Y, maskStop) &&
				dyn_msk_check(pstEnv, pstMapInst, pstMask, pstOriPos->X, pstOriPos->Y, 
							pstNextPos->X, pstNextPos->Y,z_get_monsterdef_misctype(pstEnv,pstMon), DYN_MSK_STOP) && 
				z_pos_area(pstEnv->pstMapObj, pstMapIdx, pstNextPos)) 
			{
				break;
			}
			else if (/*pstMon->bFollowTry > FOLLOW_MSK && */
					!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstOriPos->X, pstOriPos->Y, 
						pstNextPos->X, pstNextPos->Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP))
			{
				//�ж�̬�ϰ���Ҫ�����
				if(0==mon_press_target_dynmsk(pstEnv->pstAppCtx, pstEnv, pstMapInst, pstMon,&pstTraceMon->stCurrPos))
				{
					return -1;
				}
			}

			if (pstMon->bFollowTry > FOLLOW_TRY) //ǿ�ƴ�Խ
			{
				pstNextPos->X = pstTargetPos->X;
				pstNextPos->Y = pstTargetPos->Y;
				
				if (0 == IsPixelStop(pstMask, pstNextPos->X, pstNextPos->Y) && 
					dyn_msk_check(pstEnv, pstMapInst, pstMask, pstNextPos->X, pstNextPos->Y, 
									pstNextPos->X, pstNextPos->Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
					z_pos_area(pstEnv->pstMapObj, pstMapIdx, pstNextPos))
				{
					break;
				}
			}
		}

		if (j < 3)
		{
			break;
		}
	}

	if (i == 3)
	{
		return -1;
	}

	return 0;
}

int mon_ai_trace(ZONESVRENV* pstEnv, Monster *pstMon, int iDelay)
{
	int i, iRet;
	int iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayer;
	RESPOS stTracePos;
	ZONEMAPINST *pstMapInst;
	MapIndex * pstMapIdx;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	Monster *pstTraceMon;
	MONSTERDEF *pstMonDef;

	// ǿ���ƶ���׷��
	if(pstMon->bMoveType == MOVE_TYPE_FORCED)
	{
		return 0;
	}

	//�����ƶ����������״̬
	if(MOVE_TYPE_NO == pstMon->bMoveType)
	{
		return 0;
	}

	pstMonDef = z_get_mon_def(pstEnv,pstMon);
	if(!pstMonDef)
	{
		return -1;
	}
	
	if (0 >= pstTargetList->iTargetCur)
	{
		if (0 > mon_ai_select_target(pstEnv, pstMon))
		{
			//�����ƶ��������л�״̬
			if(pstMon->stMonAtkInfo.cAtkStat ==  CS_STAT_MOVE_ATK || pstMon->stMonAtkInfo.unPilot > 0)
			{
				return -1;
			}
			
			pstMon->bAiStat = AI_STAT_MOVE;
			return -1;
		}
	}

	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstTargetList->iTargetCur, &cType);
	if (0 > iIndex)
	{
		return -1;
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	//�����ƶ��������л�״̬
	if(pstMon->stMonAtkInfo.cAtkStat !=  CS_STAT_MOVE_ATK &&  pstMon->stMonAtkInfo.unPilot == 0)
	{
		pstMon->bAiStat = AI_STAT_TRACE;
	}

	if (iDelay)
	{
		return 0;
	}
	
	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
	}

	z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 0);

	if(0 == mon_have_movespd(pstMon))
	{
		return 0;
	}

	pstMon->bTraceTry++;

	/*
	if (pstMon->bTraceTry > TRACE_TRY) //�������ϴ򲻵���ү����������
	{
		tlog_debug(pstEnv->pstLogCat, 0, 0, "trace try max, home");

		if( (pstMonDef->MiscType&MON_MISC_TYPE_TRACE) &&
			pstMon->stAtkTargets.PressNum > 0 &&
			z_distance(&pstMon->stInitPos, &pstMon->stCurrPos) < pstMon->unTraceDist)
		{
			//1.������׷�з�Χ��,������������׷�Ǹ��һ�,���Ҳ�aihome 2.��ֹ���
			pstMon->bTraceTry = TRACE_TRY+1;
		}
		else
		{
			return mon_ai_home(pstEnv, pstMon);
		}
	}*/

	/*
	if (pstMon->iPatrolID > 0 && pstMon->bTraceTry > TRACE_TRY/3) //׷�����ͼ���Ѳ��
	{
		pstMon->bTraceTry = 0;
		return mon_ai_move(pstEnv, pstMon);
	}
	*/

	if (cType == OBJ_PLAYER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "trace target %d not valid", pstTargetList->iTargetCur);
			mon_ai_home(pstEnv, pstMon);
			return -1;
		}
		pstPlayer = (Player *)pstMemBlock->szData;
		if (pstPlayer->stRoleData.Map != pstMon->stMap.iID)
		{
			tlog_info(pstEnv->pstLogCat, 0, 0, "trace player (%d) in map %d, mon (%d,%d) map %d", pstPlayer->stRoleData.Uin, pstPlayer->stRoleData.Map, 
						pstMon->iID, pstMon->iDefIdx, pstMon->stMap.iID);
			mon_ai_home(pstEnv, pstMon);
			return -1;
		}

		// ׷�ˣ��ӳ����˵�ս��״̬ʱ��
		SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec,
		                 FIGHT_STAT_DUR);

		if (0 > z_get_follow_player_pos(pstEnv, pstMapInst, pstMapIdx, pstMon, pstPlayer, &stTracePos))
		{
			int iFlag;
			int iPoint;

			iFlag = 0;

			pstMon->bFollowTry++;
			
			if (pstPlayer->iMemID == pstMon->iAtkerID && pstMon->cAtkerInfoNew) //������������λ��
			{
				if (0 == z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stAtkerPos, 100, 0, &stTracePos))
				{
					iFlag = 1;
				}
				pstMon->cAtkerInfoNew = 0;
			}

			/*
			if (0 == iFlag && 0 < pstPlayer->stOnline.cLastPathPoints)
			{
				for (i=pstPlayer->stOnline.cLastPathPoints-1; i>=0; i--) //Ŀ���·��
				{
					iPoint = (pstPlayer->stOnline.cLastPath + i)%RECORD_TURN;
					if (0 == z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, pstPlayer->stOnline.stTurnPoints+iPoint, 0, 0, &stTracePos)) 
					{
						iFlag = 1;
						break;
					}
				}
			}*/

			if (0 == iFlag)
			{
				int iStartPos;
				
				iStartPos = pstPlayer->stOnline.cCurrPoint;
				for (i = 0; i<7; i++)
				{
					iPoint = ((--iStartPos)+RECORD_TURN)%RECORD_TURN;
					if (0 == pstPlayer->stOnline.stTurnPoints[iPoint].X &&
						0 == pstPlayer->stOnline.stTurnPoints[iPoint].Y)
					{
						break;
					}
					
					if (0 == z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, pstPlayer->stOnline.stTurnPoints+iPoint, 0, 0, &stTracePos)) 
					{
						iFlag = 1;
						break;
					}
				}
			}

			if (0 == iFlag) //����Ҹ���
			{
				if (0 == z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stCurrPos, 400, 0, &stTracePos)) 
				{
					iFlag = 1;
				}
			}

			if (0 == iFlag)
			{
				return -1;
			}
		}
	}
	else
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
		if (!pstMemBlock->fValid)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "trace target %d not valid", pstTargetList->iTargetCur);
			mon_ai_home(pstEnv, pstMon);
			return -1;
		}
		
		pstTraceMon = (Monster *)pstMemBlock->szData;
		if (pstTraceMon->stMap.iID != pstMon->stMap.iID)
		{
			tlog_info(pstEnv->pstLogCat, 0, 0, "trace tracemon (%d,%d) map %d, mon (%d,%d) map %d", pstTraceMon->iID, pstTraceMon->iDefIdx, pstTraceMon->stMap.iID,
						pstMon->iID, pstMon->iDefIdx, pstMon->stMap.iID);
			mon_ai_home(pstEnv, pstMon);
			return -1;
		}

		if (0 > z_get_follow_mon_pos(pstEnv, pstMapInst, pstMapIdx, pstMon, pstTraceMon, &stTracePos))
		{
			int iFlag = 0;

			pstMon->bFollowTry++;
			
			if (0 == z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stCurrPos, 400, 0, &stTracePos)) 
			{
				iFlag = 1;
			}

			if (0 == iFlag)
			{
				return -1;
			}
		}
	}

	pstMon->tLastTrace = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstMon->stNextPos = stTracePos;

	iRet = mon_have_movespd(pstMon);
	if (0 == iRet) //����
	{
		return 0;
	}
	else
	{
		pstMon->stMspdInfo.MoveSpdCurr = iRet;
		pstMon->stMspdInfo.NextNum = 0;
	}

	pstMon->cMoving = 1;
	z_monster_move_notify(pstEnv->pstAppCtx, pstEnv, pstMon, CS_MOVE_WALK);
	TV_CLONE(pstMon->stLastMoveTv, pstEnv->pstAppCtx->stCurr);
	
	return 0;
}

int mon_press_find(Monster *pstMon, int iTargetID)
{
	int i;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	for (i=0; i<pstTargetList->PressNum; i++)
	{
		if (iTargetID == pstTargetList->astPressEntrys[i].iTargetID)
		{
			return i;
		}
	}

	return -1;
}

// ָ��ְҵ
int mon_press_specific_career(ZONESVRENV* pstEnv, Monster *pstMon, int iCareer)
{
	int i = 0, iTargetID = -1;
	const AtkTargetList *pstTargets = &pstMon->stAtkTargets;
	Player *pstPlayer = NULL;

	i = pstTargets->PressNum;
	while(i--)
	{
		char cType = 0;
		int iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv,
		                            pstTargets->astPressEntrys[i].iTargetID,
		                            &cType);
		if(iIndex < 0)
		{
			continue;
		}

		if(cType == OBJ_PLAYER)
		{
			TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool,
			                                          iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstPlayer = (Player *)pstMemBlock->szData;
			//if(pstPlayer->stRoleData.Career == iCareer)
			if( CAREER_NON !=(iCareer&0xF0) && pstPlayer->stRoleData.Career == iCareer )
			{
				iTargetID = pstTargets->astPressEntrys[i].iTargetID;
				break;
			}
			else if( CAREER_NON ==(iCareer&0xF0) && (pstPlayer->stRoleData.Career&0x0F) == (iCareer&0x0F))
			{
				iTargetID = pstTargets->astPressEntrys[i].iTargetID;
				break;
			}
		}
	}
	return iTargetID;
}

int mon_press_add_by_career(ZONESVRENV* pstEnv, Monster *pstMon, int iCareer,
                            int iValue)
{
	int i = -1, iID = mon_press_specific_career(pstEnv, pstMon, iCareer);
	char cType = 0;
	ZoneAni stAni;
	TMEMBLOCK* pstMemBlock = NULL;

	if(iID < 0)
	{
		return -1;
	}

	i = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);
	if(i < 0 || cType != OBJ_PLAYER)
	{
		return -1;
	}

	pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, i);
	if (!pstMemBlock->fValid)
	{
		return -1;
	}

	stAni.iType = OBJ_PLAYER;
	stAni.stObj.pstPlayer = (Player *)pstMemBlock->szData;
	mon_ai_addpress(pstEnv, pstMon, &stAni, iValue);
	return 0;
}

int mon_press_add_by_max(ZONESVRENV* pstEnv, Monster *pstMon, int iValue)
{
	int i = -1, iID = mon_press_max(pstEnv->pstAppCtx, pstEnv, pstMon, NULL);
	char cType = 0;
	TMEMBLOCK* pstMemBlock = NULL;
	ZoneAni stAni;

	if(iID < 0)
	{
		return -1;
	}

	i = z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);
	if(i < 0)
	{
		return -1;
	}

	switch(cType)
	{
	case OBJ_PLAYER:
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, i);
		if (!pstMemBlock->fValid)
		{
			return -1;
		}
		stAni.iType = OBJ_PLAYER;
		stAni.stObj.pstPlayer = (Player *)pstMemBlock->szData;
		break;

	case OBJ_MONSTER:
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, i);
		if (!pstMemBlock->fValid)
		{
			return -1;
		}
		stAni.iType = OBJ_MONSTER;
		stAni.stObj.pstMon = (Monster *)pstMemBlock->szData;		
		break;

	default:
		return -1;
		break;
	}

	mon_ai_addpress(pstEnv, pstMon, &stAni, iValue);
	return 0;
}

// iOrder >= 0 ��ʾ���ĵ�iOrderλ iOrder < 0 ��ʾ��С�ĵ�iOrderλ
// ���ֲ�������,��Ч��ʵ��O(m*n),���ֶ�������ԴﵽO(ln(m*n))?
static int ordered_find_from_dummy(PredContext* astContext, int iNum,
                                   int iOrder)
{
	int i = 0;

	if(iOrder >= 0)
	{
		iOrder++;
		if(iOrder > iNum)
		{
			return -1;
		}

		for(i = 0; i < iOrder; ++i)
		{
			int j = 0;
			for(j = i + 1; j < iNum; ++j)
			{
				if(astContext[j].iValue > astContext[i].iValue)
				{
					PredContext stTmp = astContext[j];
					astContext[j] = astContext[i];
					astContext[i] = stTmp;
				}
			}
		}
	}
	else
	{
		iOrder = -iOrder;
		if(iOrder > iNum)
		{
			return -1;
		}

		for(i = 0; i < iOrder; ++i)
		{
			int j = 0;
			for(j = i + 1; j < iNum; ++j)
			{
				if(astContext[j].iValue < astContext[i].iValue)
				{
					PredContext stTmp = astContext[j];
					astContext[j] = astContext[i];
					astContext[i] = stTmp;
				}
			}
		}
	}

	return astContext[iOrder - 1].iKey;
}


// ָ��ѹ��ֵ���/С�ĵڼ���Ŀ��
int mon_press_order(Monster *pstMon, int iOrder)
{
	int i = 0;
	const AtkTargetList *pstTargets = &pstMon->stAtkTargets;
	PredContext astContext[MAX_PRESS_ENTRY];

	for(i = 0; i < pstTargets->PressNum; ++i)
	{
		astContext[i].iKey = i;
		astContext[i].iValue = pstTargets->astPressEntrys[i].iPress;
	}

	i = ordered_find_from_dummy(astContext, pstTargets->PressNum, iOrder);
	if(i < 0)
	{
		return -1;
	}

	return pstTargets->astPressEntrys[i].iTargetID; // iӦ�ò���Խ��
}

// Ѫ����λ
int mon_press_hp_order(ZONESVRENV* pstEnv, Monster *pstMon, int iOrder)
{
	int i = 0;
	const AtkTargetList *pstTargets = &pstMon->stAtkTargets;
	PredContext astContext[MAX_PRESS_ENTRY];

	// �����Ѫ��
	for(i = 0; i < pstTargets->PressNum; ++i)
	{
		char cType = 0;
		int iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv,
		                            pstTargets->astPressEntrys[i].iTargetID,
		                            &cType);
		if(iIndex < 0)
		{
			return -1;
		}

		if(cType == OBJ_PLAYER)
		{
			Player *pstPlayer = NULL;
			TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool,
			                                          iIndex);
			if (!pstMemBlock->fValid)
			{
				return -1;
			}

			pstPlayer = (Player *)pstMemBlock->szData;
			astContext[i].iKey = i;
			astContext[i].iValue = pstPlayer->stRoleData.RoleDetail.HPCurr;
		}
		else if(cType == OBJ_MONSTER)
		{
			Monster *pstMonster = NULL;
			TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool,
			                                          iIndex);
			if(!pstMemBlock->fValid)
			{
				return -1;
			}
			
			pstMonster = (Monster *)pstMemBlock->szData;
			astContext[i].iKey = i;
			astContext[i].iValue = pstMonster->iHP;
		}
	}

	i = ordered_find_from_dummy(astContext, pstTargets->PressNum, iOrder);
	if(i < 0)
	{
		return -1;
	}

	return pstTargets->astPressEntrys[i].iTargetID;
}

// Ѫ���ٷֱ���λ
int mon_press_hp_percent_order(ZONESVRENV* pstEnv, Monster *pstMon,
                               int iOrder)
{
	int i = 0;
	const AtkTargetList *pstTargets = &pstMon->stAtkTargets;
	PredContext astContext[MAX_PRESS_ENTRY];

	// �����Ѫ��
	for(i = 0; i < pstTargets->PressNum; ++i)
	{
		char cType = 0;
		int iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv,
		                            pstTargets->astPressEntrys[i].iTargetID,
		                            &cType);
		if(iIndex < 0)
		{
			return -1;
		}

		if(cType == OBJ_PLAYER)
		{
			Player *pstPlayer = NULL;
			TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool,
			                                          iIndex);
			if (!pstMemBlock->fValid)
			{
				return -1;
			}

			pstPlayer = (Player *)pstMemBlock->szData;
			astContext[i].iKey = i;
			astContext[i].iValue = 
				pstPlayer->stRoleData.RoleDetail.HPCurr * 100 /
				z_get_player_hpmax(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		}
		else if(cType == OBJ_MONSTER)
		{
			Monster *pstMonster = NULL;
			TMEMBLOCK* pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool,
			                                          iIndex);
			if(!pstMemBlock->fValid)
			{
				return -1;
			}

			pstMonster = (Monster *)pstMemBlock->szData;
			astContext[i].iKey = i;
			astContext[i].iValue =
				pstMonster->iHP * 100 / 
				(!pstMonster->iMaxHP ? 1 : pstMonster->iMaxHP);
		}
	}

	i = ordered_find_from_dummy(astContext, pstTargets->PressNum, iOrder);
	if(i < 0)
	{
		return -1;
	}

	return pstTargets->astPressEntrys[i].iTargetID;
}

int mon_press_min_no_owner_idx(ZONESVRENV * pstEnv, Monster *pstMon)
{
	int i;
	Player * pstPlayer;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iMinPress = pstTargetList->astPressEntrys[0].iPress;
	int iIdx = 0;
	int iNoOwnerIdx = -1;

	if (0 >= pstTargetList->PressNum)
	{
		return -1;
	}

	for (i=0; i<pstTargetList->PressNum; i++)
	{
		if (pstTargetList->astPressEntrys[i].iPress < iMinPress)
		{
			iMinPress = pstTargetList->astPressEntrys[i].iPress;
			iIdx = i;
		}

		pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
		if( NULL == pstPlayer || !is_mon_booty_owner(pstMon, pstPlayer))
		{
			if(iNoOwnerIdx < 0 ||
				pstTargetList->astPressEntrys[i].iPress < pstTargetList->astPressEntrys[iNoOwnerIdx].iPress)
			{
				iNoOwnerIdx = i;
			}
		}
	}

	if(iNoOwnerIdx >= 0)
		return iNoOwnerIdx;

	return iIdx;
}

int mon_press_insert(ZONESVRENV * pstEnv, Monster *pstMon, int iTargetID, int iPress)
{
	int iIdx;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	if(AI_STAT_HOME == pstMon->bAiStat)
	{
		return -1;
	}
	
	if (0 <= mon_press_find(pstMon, iTargetID))
	{
		return -1;
	}

	if (pstTargetList->PressNum >= MAX_PRESS_ENTRY)
	{
		Player * pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iTargetID);

		if(pstPlayer && is_mon_booty_owner(pstMon, pstPlayer))
		{
			iIdx = mon_press_min_idx(pstMon);
		}
		else
		{
			iIdx = mon_press_min_no_owner_idx(pstEnv, pstMon);	
		}

		pstTargetList->astPressEntrys[iIdx].iTargetID = iTargetID;
		pstTargetList->astPressEntrys[iIdx].iPress = iPress;
	}
	else
	{
		iIdx = pstTargetList->PressNum;
		pstTargetList->astPressEntrys[iIdx].iTargetID = iTargetID;
		pstTargetList->astPressEntrys[iIdx].iPress = iPress;
		pstTargetList->PressNum++;
	}

	return iIdx;
}

int mon_press_max(TAPPCTX *pstAppCtx, ZONESVRENV * pstEnv, Monster *pstMon, int *piIdx)
{
	int i, iIndex;
	char cType;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iMaxPress = pstTargetList->astPressEntrys[0].iPress;
	int iIdx = 0;
	int iOwnerIdx = -1;
	Player *pstPlayer = NULL;
	
	if(piIdx)
	{
		*piIdx = -1;
	}

	if(pstTargetList->iMarkedID > 0) // ����ǵ�Ŀ���һ����
	{
		if (pstTargetList->tMarkedEnd >= pstAppCtx->stCurr.tv_sec)
		{
			return pstTargetList->iMarkedID;
		}
		else
		{
			pstTargetList->iMarkedID = 0;
		}
	}
	else if(pstTargetList->iDefyID > 0) // �������Ŀ��ڶ�����
	{
		if (pstTargetList->tDefyEnd >= pstAppCtx->stCurr.tv_sec)
		{
			return pstTargetList->iDefyID;
		}
		else
		{
			pstTargetList->iDefyID = 0;
		}
	}

	if (0 >= pstTargetList->PressNum)
	{
		return -1;
	}

	for (i=0; i<pstTargetList->PressNum; i++) // ѹ��ֵ����Ŀ���������
	{
		if (pstTargetList->astPressEntrys[i].iPress > iMaxPress)
		{
			iMaxPress = pstTargetList->astPressEntrys[i].iPress;
			iIdx = i;
		}


		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID, &cType);
		if (0 > iIndex)
		{
			continue;
		}

		if (cType == OBJ_PLAYER)
		{
			
			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
		}
		else
		{
			Monster *pstTmpMon;

			pstTmpMon = z_id_monster(pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
			if(pstTmpMon && pstTmpMon->iOwnerID > 0)
			{
				pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTmpMon->iOwnerID);
			}
		}

		if(pstPlayer && is_mon_booty_owner(pstMon, pstPlayer))
		{
			if(iOwnerIdx < 0 ||
				pstTargetList->astPressEntrys[i].iPress > pstTargetList->astPressEntrys[iOwnerIdx].iPress)
			{
				iOwnerIdx = i;
			}
		}
	}

	if(iOwnerIdx >= 0)
		iIdx = iOwnerIdx;

	if (piIdx)
	{
		*piIdx = iIdx;
	}

	return pstTargetList->astPressEntrys[iIdx].iTargetID;
}

/*
int mon_press_max(TAPPCTX *pstAppCtx, Monster *pstMon, int *piIdx)
{
	int i;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iMaxPress = pstTargetList->astPressEntrys[0].iPress;
	int iIdx = 0;

	if(piIdx)
	{
		*piIdx = -1;
	}

	if(pstTargetList->iMarkedID > 0) // ����ǵ�Ŀ���һ����
	{
		if (pstTargetList->tMarkedEnd >= pstAppCtx->stCurr.tv_sec)
		{
			return pstTargetList->iMarkedID;
		}
		else
		{
			pstTargetList->iMarkedID = 0;
		}
	}
	else if(pstTargetList->iDefyID > 0) // �������Ŀ��ڶ�����
	{
		if (pstTargetList->tDefyEnd >= pstAppCtx->stCurr.tv_sec)
		{
			return pstTargetList->iDefyID;
		}
		else
		{
			pstTargetList->iDefyID = 0;
		}
	}

	if (0 >= pstTargetList->PressNum)
	{
		return -1;
	}

	for (i=1; i<pstTargetList->PressNum; i++) // ѹ��ֵ����Ŀ���������
	{
		if (pstTargetList->astPressEntrys[i].iPress > iMaxPress)
		{
			iMaxPress = pstTargetList->astPressEntrys[i].iPress;
			iIdx = i;
		}
	}

	if (piIdx)
	{
		*piIdx = iIdx;
	}

	return pstTargetList->astPressEntrys[iIdx].iTargetID;
}
*/

int mon_press_rand(TAPPCTX *pstAppCtx, Monster *pstMon)
{
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iIdx;

	if(pstTargetList->iMarkedID > 0) // ����ǵ�Ŀ���һ����
	{
		if (pstTargetList->tMarkedEnd >= pstAppCtx->stCurr.tv_sec)
		{
			return pstTargetList->iMarkedID;
		}
		else
		{
			pstTargetList->iMarkedID = 0;
		}
	}
	else if(pstTargetList->iDefyID > 0) // �������Ŀ��ڶ�����
	{
		if (pstTargetList->tDefyEnd >= pstAppCtx->stCurr.tv_sec)
		{
			return pstTargetList->iDefyID;
		}
		else
		{
			pstTargetList->iDefyID = 0;
		}
	}

	if (0 >= pstTargetList->PressNum)
	{
		return -1;
	}

	iIdx = RAND1(pstTargetList->PressNum);
	
	return pstTargetList->astPressEntrys[iIdx].iTargetID;
}

// ǿ�����ѡ��
int mon_press_forced_rand(Monster *pstMon)
{
	AtkTargetList *pstTargets = &pstMon->stAtkTargets;
	int i = 0;

	if(pstTargets->PressNum <= 0)
	{
		return -1;
	}

	i = RAND1(pstTargets->PressNum);
	return pstTargets->astPressEntrys[i].iTargetID;
}

int mon_press_min_idx(Monster *pstMon)
{
	int i;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iMinPress = pstTargetList->astPressEntrys[0].iPress;
	int iIdx = 0;

	if (0 >= pstTargetList->PressNum)
	{
		return -1;
	}

	for (i=1; i<pstTargetList->PressNum; i++)
	{
		if (pstTargetList->astPressEntrys[i].iPress < iMinPress)
		{
			iMinPress = pstTargetList->astPressEntrys[i].iPress;
			iIdx = i;
		}
	}

	return iIdx;
}

/*
int mon_press_insert(Monster *pstMon, int iTargetID, int iPress)
{
	int iIdx;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	if(AI_STAT_HOME == pstMon->bAiStat)
	{
		return -1;
	}
	
	if (0 <= mon_press_find(pstMon, iTargetID))
	{
		return -1;
	}

	if (pstTargetList->PressNum >= MAX_PRESS_ENTRY)
	{
		iIdx = mon_press_min_idx(pstMon);
		pstTargetList->astPressEntrys[iIdx].iTargetID = iTargetID;
		pstTargetList->astPressEntrys[iIdx].iPress = iPress;
	}
	else
	{
		iIdx = pstTargetList->PressNum;
		pstTargetList->astPressEntrys[iIdx].iTargetID = iTargetID;
		pstTargetList->astPressEntrys[iIdx].iPress = iPress;
		pstTargetList->PressNum++;
	}

	return iIdx;
}
*/

int mon_press_delete(Monster *pstMon, int iTargetID)
{
	int iIdx;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	if (pstTargetList->iMarkedID == iTargetID)
	{
		pstTargetList->iMarkedID = 0;
	}

	if (pstTargetList->iDefyID == iTargetID)
	{
		pstTargetList->iDefyID = 0;
	}

	if (pstTargetList->iTargetCur == iTargetID)
	{
		pstTargetList->iTargetCur = 0;
	}

	iIdx = mon_press_find(pstMon, iTargetID);
	if (0 > iIdx)
	{
		return -1;
	}

	pstTargetList->PressNum--;

	if (iIdx != pstTargetList->PressNum)
	{
		memcpy(pstTargetList->astPressEntrys+iIdx, pstTargetList->astPressEntrys+pstTargetList->PressNum, sizeof(pstTargetList->astPressEntrys[0]));
	}

	return 0;
}

int mon_press_delete_player(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	AIEvt stAIEvt;

	if (ATK_TYPE_NO == pstMon->bAtkType)
	{
		return -1;
	}

	if (0 > mon_press_delete(pstMon, pstPlayer->iMemID))
	{
		return -1;
	}

	// ͬʱ�ѹ����Player���б���ɾ��
	player_delete_pressed_mon(pstEnv, pstPlayer, pstMon->iID);

	stAIEvt.iEvtID = AI_EVT_OUTPRESS;
	stAIEvt.stEvtDetail.stOutPress.stAni.iType = OBJ_PLAYER;
	stAIEvt.stEvtDetail.stOutPress.stAni.stObj.pstPlayer = pstPlayer;
	mon_ai_stat(pstEnv, pstMon, &stAIEvt);

	//tlog_debug(pstEnv->pstLogCat, 0, 0, "%d mon(%d) del target id %d", pstMon->iDefIdx, pstMon->iID, pstPlayer->iMemID);

	return 0;
}

int mon_press_delete_mon(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstTar)
{
	AIEvt stAIEvt;

	if (ATK_TYPE_NO == pstMon->bAtkType)
	{
		return -1;
	}

	if (0 > mon_press_delete(pstMon, pstTar->iID))
	{
		return -1;
	}

	stAIEvt.iEvtID = AI_EVT_OUTPRESS;
	stAIEvt.stEvtDetail.stOutPress.stAni.iType = OBJ_MONSTER;
	stAIEvt.stEvtDetail.stOutPress.stAni.stObj.pstMon = pstTar;
	mon_ai_stat(pstEnv, pstMon, &stAIEvt);

	return 0;
}


int mon_press_insert_player2(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, Player *pstPlayer)
{
	int i;
	int iPress;
	AIEvt stAIEvt;
	float fRate;

	if (ATK_TYPE_NO == pstMon->bAtkType)
	{
		return -1;
	}

	if(is_mon_machine(pstMon) || is_mon_factory(pstMon))
	{
		return -1;
	}

	if(is_mon_bub(pstMon))
	{
		if(mon_bub_get_mode(pstEnv, pstMon) == BUB_STAT_FOLLOW)
		{
			return -1;
		}
	}
	
	iPress = 0;
	if (pstMon->cType == OBJ_MONSTER)
	{
		for (i=0; i<MAX_PRESS_INIT; i++)
		{
			if (0 == pstMonDef->PressInit[i].PressInitType)
			{
				break;
			}
			
			//if (PRESS_INIT_CAREER == pstMonDef->PressInit[i].PressInitType && pstPlayer->stRoleData.Career == pstMonDef->PressInit[i].Val1)
			if (PRESS_INIT_CAREER == pstMonDef->PressInit[i].PressInitType)
			{
				if( CAREER_NON != (pstMonDef->PressInit[i].Val1&0xF0) &&  pstPlayer->stRoleData.Career == pstMonDef->PressInit[i].Val1 )
				{
					iPress = pstMonDef->PressInit[i].Val2;
					break;
				}
				else if( CAREER_NON == (pstMonDef->PressInit[i].Val1&0xF0) &&  (pstPlayer->stRoleData.Career&0x0F) == (pstMonDef->PressInit[i].Val1&0x0F) )
				{
					iPress = pstMonDef->PressInit[i].Val2;
					break;
				}
			}

			fRate = pstPlayer->stRoleData.RoleDetail.HPCurr*1.0/pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax;
			if (PRESS_INIT_HP == pstMonDef->PressInit[i].PressInitType && 
				fRate*100 < pstMonDef->PressInit[i].Val1)
			{
				iPress = pstMonDef->PressInit[i].Val2 * (1.0 - fRate);
				break;
			}

			if (PRESS_INIT_OWNER == pstMonDef->PressInit[i].PressInitType && 
				(0 == strcmp(pstPlayer->stRoleData.RoleName, pstMon->szOwner)))
			{
				iPress = pstMonDef->PressInit[i].Val2;
				break;
			}

			// ������ 
			if (PRESS_INIT_PLAYER == pstMonDef->PressInit[i].PressInitType)
			{
				iPress = pstMonDef->PressInit[i].Val2;
				break;
			}
		}
	}
	else if (pstMon->cType == OBJ_NPC)
	{
		// ����������ҵ�npc
		// ����ôд�� ��ѹ��ֵ�Լ��Ҷ���
		if (pstMon->iPKType == MON_PK_TYPE_PANISH)
		{
			if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_PUBLIC_ENEMY)
			{
				iPress  =100;	
			}
			else if(pstPlayer->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_FRENZY)
			{
				iPress  =50;	
			}
			else
			{
				//return -1;
			}
		}
		else if (	pstMon->iPKType == MON_PK_TYPE_CLAN)
		{
			if (	pstMon->ullPKID != pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID &&
				(pstPlayer->stOnline.State & CS_STAT_FIGHT))
			{
				iPress  =100;	
			}
		}
	}
	
	if (0 > iPress)
	{
		iPress = 0;
	}
	
	if (0 <= mon_press_insert(pstEnv, pstMon, pstPlayer->iMemID, iPress))
	{
		//tlog_debug(pstEnv->pstLogCat, 0, 0, "%d mon(%d) add target id %d press %d", pstMon->iDefIdx, pstMon->iID,  pstPlayer->iMemID, iPress);
	}
	else
	{
		return -1;
	}


	if(pstMon->bSubType != MON_SUB_TYPE_TOTEM)
	{
		// ͬʱ�ѹ�����뵽Player���б���
		// ʹѹ�������˫�����
		for(i = 0; i < pstPlayer->stOnline.iPressedMonNum; ++i)
		{
			if(pstPlayer->stOnline.aiPressedMon[i] == pstMon->iID)
			{
				break;
			}
		}
		if(i >= pstPlayer->stOnline.iPressedMonNum)
		{
			if(pstPlayer->stOnline.iPressedMonNum ==
			   CCH(pstPlayer->stOnline.aiPressedMon))
			{
				memmove(pstPlayer->stOnline.aiPressedMon,
				        pstPlayer->stOnline.aiPressedMon + 1,
				        sizeof(pstPlayer->stOnline.aiPressedMon[0]) *
				        (CCH(pstPlayer->stOnline.aiPressedMon) - 1));
				--pstPlayer->stOnline.iPressedMonNum;
			}
			pstPlayer->stOnline.aiPressedMon[pstPlayer->stOnline.iPressedMonNum++]
				= pstMon->iID;
		}
	}

	stAIEvt.iEvtID = AI_EVT_INPRESS;
	stAIEvt.stEvtDetail.stInPress.stAni.iType = OBJ_PLAYER;
	stAIEvt.stEvtDetail.stInPress.stAni.stObj.pstPlayer = pstPlayer;
	
	mon_ai_stat(pstEnv, pstMon, &stAIEvt);
	
	return 0;
}

int mon_press_insert_player(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, Player *pstPlayer)
{
	if( 	0 == is_mon_enemy_player(pstEnv, pstMon, pstPlayer) && 
		pstMon->iPKType != MON_PK_TYPE_PANISH)
	{
		return -1;
	}
	
	if (0 > mon_press_insert_player2(pstEnv, pstMon, pstMonDef, pstPlayer))
	{
		return -1;
	}
	
	// ����������Ĵ���,һ�˰���,ȫ�ҽ�ѹ����
 	if(pstMon->iLinkedNext >= 0 && pstMon->iLinkedPrev >= 0)
 	{
		Monster *pstLinkedMon = NULL;

		for(pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstMon->iLinkedNext);
		    pstLinkedMon != pstMon;
		    pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstLinkedMon->iLinkedNext))
		{
			if(!pstLinkedMon)
			{
				break;
			}

			if(pstLinkedMon->cStat != MONSTER_DIE)
			{
				mon_press_insert_player2(pstEnv, pstLinkedMon, pstMonDef, pstPlayer);
			}
		}
 	}

	//����ĺ��ȣ�����ѹ���������
	if(pstMonDef->CallHelpHP == -1 && pstMon->stAtkTargets.PressNum == 1)
	{
		ZoneAni stAtker;

		stAtker.iType = OBJ_PLAYER;
		stAtker.stObj.pstPlayer = pstPlayer;
		
		mon_help_by_map(pstEnv, pstMon, pstMonDef, &stAtker);
	}
	
	return 0;
}


int npc_in_press_check_punish(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	if (	pstMon->iPKType == MON_PK_TYPE_PANISH &&
		is_bad_palyer(pstPlayer))
	{
		return 1;
	}
	UNUSED(pstEnv);
	return 0;
}

int npc_in_press_check_clanmode(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	
	if (	pstMon->iPKType == MON_PK_TYPE_CLAN &&
		pstMon->ullPKID != pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.GID &&
		(pstPlayer->stOnline.State & CS_STAT_FIGHT))
	{
		return 1;
	}
	UNUSED(pstEnv);
	return 0;
}

int npc_in_press_check_camp(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	if (pstMon->iPKType == MON_PK_TYPE_CAMP &&
		pstMon->iCampIdx != pstPlayer->stOnline.stWarOL.cCampIdx)
	{
		return 1;
	}
	UNUSED(pstEnv);
	return 0;
}

int npc_in_press_check(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	if (pstMon->cType == OBJ_MONSTER)
	{
		return 1;
	}

	if (npc_in_press_check_punish(pstEnv,pstMon, pstPlayer))
		return 1;

	if (npc_in_press_check_clanmode(pstEnv,pstMon, pstPlayer))
		return 1;

	if (npc_in_press_check_camp(pstEnv,pstMon, pstPlayer))
		return 1;

	return 0;
}

int mon_press_update_player_in(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	MONSTERDEF *pstMonDef;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef )
	{
		return -1;
	}

	if (npc_in_press_check(pstEnv, pstMon, pstPlayer) == 0)
	{
		return -1;
	}
	/*if (pstMon->cType == OBJ_NPC)
	{	
		if (	pstMon->bAtkType != ATK_TYPE_INITIATIVE || 
			!is_bad_palyer(pstPlayer))
		{
			return 0;
		}
	}*/
	
	if ((pstPlayer->stOnline.State & (CS_STAT_DEAD | CS_STAT_AUTOFLY | CS_STAT_QIANXIN)) || 
		is_friend_mon(pstMon)|| 
		is_mon_machine(pstMon) ||
		is_mon_factory(pstMon) ||
		is_mon_bub(pstMon) ||
		pstMon->bSubType == MON_SUB_TYPE_BUILD ||
		is_player_same_camp_mon(pstPlayer, pstMon) ||
		(pstMonDef->MiscType&MON_MISC_TYPE_NO_PRESS)  ||
		is_player_ride_on_machine(pstPlayer) ||
		pstPlayer->stOnline.fShamDead)
	{
		return 0;
	}

	if (pstMon->stMap.iID == pstPlayer->stRoleData.Map && z_distance(&pstPlayer->stRoleData.Pos, &pstMon->stCurrPos) <= pstMon->unViewDist)
	{
		if (ATK_TYPE_INITIATIVE == pstMon->bAtkType && 0 > mon_press_find(pstMon, pstPlayer->iMemID))
		{
			//MONSTERDEF *pstMonDef;

			pstMonDef = z_get_mon_def(pstEnv, pstMon);
			if (NULL == pstMonDef)
			{
				return -1;
			}
			
			mon_press_insert_player(pstEnv, pstMon, pstMonDef, pstPlayer);
		}
	}
		
	return 0;
}

int mon_press_insert_mon2(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstVMon)
{
	int i;
	int iPress;
	AIEvt stAIEvt;
	MONSTERDEF *pstMonDef;

	if (ATK_TYPE_NO == pstMon->bAtkType)
	{
		return -1;
	}
	
	if(pstVMon->bSubType >= MON_SUB_TYPE_TRAP &&  !is_convoy_car(pstVMon))
	{
		return -1;
	}

	if(is_mon_bub(pstMon))
	{
		if(mon_bub_get_mode(pstEnv, pstMon) == BUB_STAT_FOLLOW)
		{
			return -1;
		}
	}

	if(is_mon_machine(pstMon) || is_mon_factory(pstMon))
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}

	iPress = 0;
	for (i=0; i<MAX_PRESS_INIT; i++)
	{
		if (0 == pstMonDef->PressInit[i].PressInitType)
		{
			break;
		}

		if (PRESS_INIT_OWNER == pstMonDef->PressInit[i].PressInitType && 
			pstVMon->iID == pstMon->iCallMonID)
		{
			iPress = pstMonDef->PressInit[i].Val2;
			break;
		}
	}
	
	if (0 > iPress)
	{
		iPress = 0;
	}

	if (0 == iPress && pstMon->stAtkTargets.PressNum >= MAX_PRESS_ENTRY)
	{
		return -1;
	}

	if (0 <= mon_press_insert(pstEnv, pstMon, pstVMon->iID, iPress))
	{
		//tlog_debug(pstEnv->pstLogCat, 0, 0, "%d mon(%d) add target id %d press %d", pstMon->iDefIdx, pstMon->iID,  pstPlayer->iMemID, iPress);
	}
	else
	{
		return -1;
	}

	stAIEvt.iEvtID = AI_EVT_INPRESS;
	stAIEvt.stEvtDetail.stInPress.stAni.iType = OBJ_MONSTER;
	stAIEvt.stEvtDetail.stInPress.stAni.stObj.pstMon = pstVMon;
	
	mon_ai_stat(pstEnv, pstMon, &stAIEvt);
	return 0;
}

int mon_press_insert_mon(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstVMon)
{
	MONSTERDEF *pstMonDef;

	if(0 == is_mon_enemy_mon(pstEnv, pstMon, pstVMon) &&
		pstMon->iPKType != MON_PK_TYPE_PANISH)
	{
		return -1;
	}

	if (pstVMon->cDartCarNotAtk == 1)
	{
		return -1;
	}

	if (pstVMon->iRongYanTeamCapId > 0 && pstMon->iPKType == MON_PK_TYPE_PANISH)
	{
		return -1;
	}

	if(0 > mon_press_insert_mon2(pstEnv, pstMon, pstVMon))
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(pstMonDef)
	{
		//����ĺ��ȣ�����ѹ���������
		if(pstMon->stAtkTargets.PressNum == 1 && pstMonDef->CallHelpHP == -1)
		{
			ZoneAni stAtker;

			stAtker.iType = OBJ_MONSTER;
			stAtker.stObj.pstMon = pstVMon;
			
			mon_help_by_map(pstEnv, pstMon, pstMonDef, &stAtker);
		}
	}
	
	return 0;
}
SKILLDEF * mon_find_one_skilldef(ZONESVRENV* pstEnv, Monster *pstMon)
{
	SKILLDEF *pstSkillDef = NULL;
	MonNormalSkill *pstMonNormalSkill = &pstMon->astNormalSkill[0];//��ȷ������ͬ��������
	MonInsertSkill *pstAtkerInsertSkill = &pstMon->astAtkerInsertSkill[0];
	MonInsertSkill *pstAtkeeInsertSkill = &pstMon->astAtkeeInsertSkill[0];

	pstSkillDef = z_find_skill(pstEnv,  pstMonNormalSkill->stSkill.SkillID, pstMonNormalSkill->stSkill.SkillLevel, &pstMonNormalSkill->iSkillIdx);
	if(pstSkillDef)
	{
		return pstSkillDef;
	}

	pstSkillDef = z_find_skill(pstEnv,  pstAtkerInsertSkill->stSkill.SkillID, pstAtkerInsertSkill->stSkill.SkillLevel, &pstAtkerInsertSkill->iSkillIdx);
	if(pstSkillDef)
	{
		return pstSkillDef;
	}


	pstSkillDef = z_find_skill(pstEnv,  pstAtkeeInsertSkill->stSkill.SkillID, pstAtkeeInsertSkill->stSkill.SkillLevel, &pstAtkeeInsertSkill->iSkillIdx);
	if(pstSkillDef)
	{
		return pstSkillDef;
	}

	return NULL;
}
int mon_press_update_mon_in(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstVMon)
{	
	RESPOS stPos = pstMon->stCurrPos;
	
	if (pstVMon->cStat == MONSTER_DIE || 
		pstVMon->bAiStat == AI_STAT_HOME || 
		is_mon_machine(pstMon) ||
		is_mon_factory(pstMon) ||
		//is_mon_factory(pstVMon) ||
		(pstVMon->bSubType >= MON_SUB_TYPE_TRAP &&  !is_convoy_car(pstVMon)) )
	{
		return 0;
	}

	if (pstMon->cType == OBJ_NPC )
	{
		/* ԭ����
		if (	pstMon->iPKType != MON_PK_TYPE_PANISH ||
			!is_bad_mon(pstEnv, pstVMon))
		{
			return 0;	 
		}
		*/

		// �ƶ�ģʽ
		if(pstMon->iPKType == MON_PK_TYPE_PANISH)
		{
			if (!is_bad_mon(pstEnv, pstVMon))
			{
				return 0;
			}
		}
		//��Ӫģʽ
		else if(pstMon->iPKType == MON_PK_TYPE_CAMP)
		{
			if(!(is_mon_machine(pstVMon) && 
				pstVMon->iPKType == MON_PK_TYPE_PLAYER && 
				is_camp_mon(pstVMon) && 
				pstVMon->iCampIdx != pstMon->iCampIdx))
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	if (pstMon->iOwnerID > 0 || 
		pstMon->iCallMonID > 0 || 
		is_friend_mon(pstMon) || 
		is_friend_mon(pstVMon) ||
		is_camp_mon(pstMon) ||
		is_camp_mon(pstVMon) ||
		is_mon_machine(pstVMon) ||
		is_mon_factory(pstVMon) ||
		pstVMon->iOwnerID >0)
	{
		//�Ƶ�insert��
	}
	else
	{
		return 0;
	}


	if (pstMon->stMap.iID == pstVMon->stMap.iID)
	{
		if(is_mon_bub(pstMon))
		{
			ZoneAni stAni;
			Player *pstPlayer;
		
			stAni.iType = OBJ_MONSTER;
			stAni.stObj.pstMon = pstVMon;

			if(0 > mon_bub_inpress_check(pstEnv, pstMon, &stAni))
			{
				return 0;
			}
		
			pstPlayer = mon_bub_get_owner(pstEnv, pstMon);
			if(pstPlayer)
			{
				stPos = pstPlayer->stRoleData.Pos;
			}
		}
		
		if(z_distance(&pstVMon->stCurrPos, &stPos) > pstMon->unViewDist)
		{
			return 0;
		}
		
		if (ATK_TYPE_INITIATIVE == pstMon->bAtkType && 0 > mon_press_find(pstMon, pstVMon->iID))
		{
			mon_press_insert_mon(pstEnv, pstMon, pstVMon);
		}
	}
	
	return 0;
}

int mon_press_update_out(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i, iIndex;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayerTarget;
	Monster *pstMonTarget;
	Npc *pstNpc;
	RESPOS stInitPos = pstMon->stInitPos;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	if(is_mon_bub(pstMon))
	{
		Player *pstOwnerPlayer = mon_bub_get_owner(pstEnv, pstMon);
		if(pstOwnerPlayer)
		{
			stInitPos = pstOwnerPlayer->stRoleData.Pos;
		}
	}
	
	pstMon->tLastPressUpdate = pstEnv->pstAppCtx->stCurr.tv_sec;

	for (i=pstTargetList->PressNum-1; i>=0; i--)
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID, &cType);
		if (0 > iIndex)
		{
			mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
			mon_ai_evt_outpress(pstEnv, pstMon);
			continue;
		}

		//�ǹ���ս�ڼ乥�ǽ���������������
		if(!is_in_siege_city_fast(pstEnv) &&
			is_city_build_mon(pstMon) )
		{
			mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
			mon_ai_evt_outpress(pstEnv, pstMon);
			continue;
		}


		if (cType == OBJ_PLAYER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
				mon_ai_evt_outpress(pstEnv, pstMon);
				
				continue;
			}

			pstPlayerTarget = (Player *)pstMemBlock->szData;
			if ((pstPlayerTarget->stOnline.State & (CS_STAT_DEAD | CS_STAT_AUTOFLY | CS_STAT_QIANXIN)) ||
				pstMon->stMap.iID != pstPlayerTarget->stRoleData.Map || 
				z_distance(&pstPlayerTarget->stRoleData.Pos, &stInitPos) > pstMon->unTraceDist ||
				pstPlayerTarget->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
				mon_ai_evt_outpress(pstEnv, pstMon);
			}
		}
		else if (cType == OBJ_MONSTER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
				mon_ai_evt_outpress(pstEnv, pstMon);
				
				continue;
			}

			pstMonTarget = (Monster *)pstMemBlock->szData;

			//�ǹ���ս�ڼ䲻���Թ����ǳؽ���
			if(!is_in_siege_city_fast(pstEnv) &&
				is_city_build_mon(pstMonTarget) )
			{
				mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
				mon_ai_evt_outpress(pstEnv, pstMon);
				continue;
			}

			if (pstMonTarget->cStat == MONSTER_DIE || pstMonTarget->bAiStat == AI_STAT_HOME ||
				pstMon->stMap.iID != pstMonTarget->stMap.iID || 
				z_distance(&pstMonTarget->stCurrPos, &stInitPos) > pstMon->unTraceDist)
			{
				mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
				mon_ai_evt_outpress(pstEnv, pstMon);
			}
		}
		else if (cType == OBJ_NPC)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
				mon_ai_evt_outpress(pstEnv, pstMon);
				
				continue;
			}

			pstNpc = (Npc *)pstMemBlock->szData;
			if (pstNpc->stMon.cStat == MONSTER_DIE || pstNpc->stMon.bAiStat == AI_STAT_HOME ||
				pstMon->stMap.iID != pstNpc->stMon.stMap.iID || 
				z_distance(&pstNpc->stMon.stCurrPos, &stInitPos) > pstMon->unTraceDist)
			{
				mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
				mon_ai_evt_outpress(pstEnv, pstMon);
			}
		}
		else
		{
			mon_press_delete(pstMon, pstTargetList->astPressEntrys[i].iTargetID);
			mon_ai_evt_outpress(pstEnv, pstMon);
		}
	}
	
	return 0;
}

int mon_ai_rand_select_target(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int iTarget;
	int iIdx;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	iTarget = mon_press_rand(pstEnv->pstAppCtx, pstMon);
	if (0 > iTarget)
	{
		return -1;
	}

	if (iTarget != pstTargetList->iTargetCur) //�״�ѡ��
	{
		pstTargetList->iTargetCur = iTarget;

		iIdx = mon_press_find(pstMon, iTarget);
		if (iIdx >= 0)
		{
			pstTargetList->astPressEntrys[iIdx].iPress += pstMon->unPressSelectAdd;
		}
	}

	pstTargetList->tTargetSelect = pstEnv->pstAppCtx->stCurr.tv_sec;

	return 0;
}

PATROLDEF *z_find_patroldef(ZONESVRENV* pstEnv, int iPatrolID, int *piPatrolIdx)
{
	return (PATROLDEF *)z_find_def(pstEnv->pstZoneObj->sPatrolDef, iPatrolID, PATROL_DATA, 
									pstEnv->pstZoneObj->iPatrolDef, piPatrolIdx);
}

int mon_bub_get_follow_dir(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstPlayer)
{
	int iPoint;
	int iIdx;
	int iFaceAngel;
	RESPOS *pstOriPos = &pstMon->stCurrPos;
	RESPOS *pstTargetPos = &pstPlayer->stRoleData.Pos;
	
	if(pstPlayer->stOnline.cLastPathPoints <= 0)
	{
		goto _error;
	}

	iIdx = mon_bub_get_idx(pstEnv, pstMon);
	if(iIdx < 0)
	{
		goto _error;
	}

	iPoint = (pstPlayer->stOnline.cLastPath + pstPlayer->stOnline.cLastPathPoints - 1+RECORD_TURN)%RECORD_TURN;
	iFaceAngel = FaceAngleBetween(pstPlayer->stOnline.stTurnPoints[iPoint].X, pstPlayer->stOnline.stTurnPoints[iPoint].Y,
						pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);

	if(pstPlayer->stOnline.stBubOnline.iBubNum%2 == 0)
	{
		iFaceAngel = iFaceAngel -15;
		if(iFaceAngel < 0)
		{
			iFaceAngel = 360 + iFaceAngel;
		}
	}

	iIdx = iIdx+1;
	if(iIdx%2 == 0)
	{
		iFaceAngel = iFaceAngel + iIdx/2*30;
	}
	else
	{
		iFaceAngel = iFaceAngel - iIdx/2*30;
		if(iFaceAngel < 0)
		{
			iFaceAngel = 360 + iFaceAngel;
		}
	}

	return iFaceAngel;

_error:

	iFaceAngel = FaceAngleBetween(pstOriPos->X, pstOriPos->Y, pstTargetPos->X, pstTargetPos->Y);
	if (iFaceAngel >= 360)
	{
		iFaceAngel -= 360;
	}

	return iFaceAngel;
}

int mon_get_move2player_pos(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst , MapIndex * pstMapIdx, Monster *pstMon, Player *pstPlayer,  RESPOS *pstNextPos)
{
	TMAPMASK *pstMask;
	int iDist;
	int i, iFaceAngel;
	RESPOS *pstOriPos = &pstMon->stCurrPos;
	RESPOS *pstTargetPos = &pstPlayer->stRoleData.Pos;
	
	if ( !pstMapIdx)
	{
		memcpy(pstNextPos, pstOriPos, sizeof(*pstNextPos));

		return -1;
	}

	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	iFaceAngel = mon_bub_get_follow_dir(pstEnv, pstMon, pstPlayer);
	iDist = 200;

	for (i=0; i<3; i++)
	{
		iDist = iDist >> i;
		pstNextPos->X		=	(int) (pstTargetPos->X - iDist * cos((iFaceAngel)*PI/180.0f));
		pstNextPos->Y		=	(int) (pstTargetPos->Y + iDist * sin((iFaceAngel)*PI/180.0f));

		if (IsPixelLineThrough(pstMask, pstOriPos->X, pstOriPos->Y, pstNextPos->X, pstNextPos->Y, maskStop) &&
			IsPixelLineThrough(pstMask, pstNextPos->X, pstNextPos->Y, pstNextPos->X, pstNextPos->Y, maskStop) &&
			dyn_msk_check(pstEnv, pstMapInst, pstMask, pstOriPos->X, pstOriPos->Y, 
							pstTargetPos->X, pstTargetPos->Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
			z_pos_area(pstEnv->pstMapObj, pstMapIdx, pstNextPos)) 
		{
			break;
		}
					
		if (pstMon->bFollowTry > FOLLOW_TRY) //ǿ�ƴ�Խ
		{
			pstNextPos->X = pstTargetPos->X;
			pstNextPos->Y = pstTargetPos->Y;
			if (0 == IsPixelStop(pstMask, pstNextPos->X, pstNextPos->Y) && 
				dyn_msk_check(pstEnv, pstMapInst, pstMask, pstNextPos->X, pstNextPos->Y,
								pstNextPos->X, pstNextPos->Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
			       z_pos_area(pstEnv->pstMapObj, pstMapIdx, pstNextPos))
			{
				break;
			}
		}
	}

	if (i == 3)
	{
		return -1;
	}

	return 0;
}


int mon_move_follow_owner(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx,
	Monster *pstMon, Player *pstOnwer)
{
	int i;
	RESPOS stNext;

	if(0 > mon_get_move2player_pos(pstEnv, pstMapInst, pstMapIdx, pstMon, pstOnwer, &stNext))
	{
		int iFlag = 0;
		int iPoint;

		pstMon->bFollowTry++;		
		
		for (i=pstOnwer->stOnline.cLastPathPoints-1; i>=0; i--) //Ŀ���·��
		{
			iPoint = (pstOnwer->stOnline.cLastPath + i)%RECORD_TURN;
			if (0 == z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, pstOnwer->stOnline.stTurnPoints+iPoint, 150, 0, &stNext)) 
			{
				iFlag = 1;
				break;
			}
		}
		
		if (0 == iFlag) //����Ҹ���
		{
			if (0 == z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stCurrPos, 400, 0, &stNext)) 
			{
				iFlag = 1;
			}
		}

		if (0 == iFlag)
		{
			return -1;
		}
	}

	pstMon->stNextPos = stNext;
	return 0;
}

int mon_select_movepos(ZONESVRENV* pstEnv,  Monster *pstMon, MONSTERDEF *pstMonDef)
{
	RESPOS stNext;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	PATROLDEF *pstPatrolDef;
	TMAPMASK *pstMask;
	int iSpd = mon_have_movespd(pstMon)*3;
	RESPOS stInter;
	ZoneAni stAni;
	int interNum=0;
	

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	switch(pstMon->bMoveType)
	{
	case MOVE_TYPE_NO:
		return -1;
		break;
	case MOVE_TYPE_FORCED:
		// ǿ���ƶ�����ǿ�Ƶ���һ���㣬������
		pstMon->stNextPos = pstMon->stForcedPos;
		break;
	case MOVE_TYPE_RANDOM:
		if (MON_MISC_TYPE_RAND_MOVE & pstMonDef->MiscType)
		{
			if (0 > z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stCurrPos, iSpd, 10, &stNext)) 
			{
				if (0 > z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stInitPos, iSpd, 10, &stNext))
				{
					return -1;
				}
			}
		}
		else
		{
			if (0 > z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stInitPos, iSpd, 10, &stNext)) 
			{
				if (0 > z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stCurrPos, iSpd, 10, &stNext))
				{
					return -1;
				}
			}
		}

		if(z_distance(&pstMon->stInitPos, &stNext) > pstMon->unTraceDist)
		{
			return -1;
		}

		/*
		if (0 > z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stInitPos, iSpd, 10, &stNext))
		{
			//ԭ�������
			if (0 > z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstMon->stCurrPos, &pstMon->stCurrPos, iSpd, 10, &stNext)) 
			{
				return -1;
			}

			//��ֹ�Լ���ԶAI HOME
			if(z_distance(&pstMon->stInitPos, &stNext) > pstMon->unTraceDist)
			{
				return -1;
			}
		}*/

		pstMon->stNextPos = stNext;
		break;
	case MOVE_TYPE_PATROL: //todo Ѳ�ߵ�
		if (0 >= pstMon->iPatrolID)
		{
			return 0;
		}

		pstPatrolDef = z_find_patroldef(pstEnv, pstMon->iPatrolID, &pstMon->iPatrolIdx);
		if (NULL == pstPatrolDef)
		{
			return -1;
		}

		if (pstMon->stCurrPos.X == pstPatrolDef->PatrolPos[pstMon->bPatrolPos].X && pstMon->stCurrPos.Y == pstPatrolDef->PatrolPos[pstMon->bPatrolPos].Y)
		{
			//Ѳ�߹��趨��λ��
			pstMon->stInitPos = pstMon->stCurrPos;
			
			if (0 == pstMon->bPatrolTurn)
			{
				pstMon->bPatrolPos++;
				if (pstMon->bPatrolPos >= MAX_PATROL_POINT || 0 == pstPatrolDef->PatrolPos[pstMon->bPatrolPos].X)
				{
					//��������
					if(pstPatrolDef->PatrolPos[pstMon->bPatrolPos-1].X == pstPatrolDef->PatrolPos[0].X 
						&& pstPatrolDef->PatrolPos[pstMon->bPatrolPos-1].Y == pstPatrolDef->PatrolPos[0].Y )
					{
						pstMon->bPatrolTurn = 0;
						pstMon->bPatrolPos = 1;
					}
					else
					{
						pstMon->bPatrolTurn = 1;
						pstMon->bPatrolPos -= 2;
					}
				}
			}
			else
			{
				if (0 == pstMon->bPatrolPos)
				{
					pstMon->bPatrolTurn = 0;
					pstMon->bPatrolPos++;
				}
				else
				{
					pstMon->bPatrolPos--;
				}
			}
		}

		stNext = pstPatrolDef->PatrolPos[pstMon->bPatrolPos];

		
		pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
	/*	if (!IsPixelLineThrough(pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, stNext.X, stNext.Y, maskStop) )
		{
			if (0 > z_get_ran_pos(pstEnv, pstMapIdx, &pstMon->stCurrPos, &pstPatrolDef->PatrolPos[pstMon->bPatrolPos], 400, 0, &stNext))
			{
				return -1;
			}
		}
		*/

		memset(&stInter,0,sizeof(stInter));
		memset(&stAni,0,sizeof(stAni));
		stAni.iType = OBJ_MONSTER;
		stAni.stObj.pstMon = pstMon;
		if( !dyn_msk_check(pstEnv, pstMapInst, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, 
			stNext.X, stNext.Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP) &&
			0 < mon_press_target_dynmsk_pos(pstEnv->pstAppCtx, pstEnv, pstMapInst, 
									&stAni,&pstMon->stCurrPos, &stNext, &stInter,&interNum,1))
		{
			pstMon->stNextPos = stInter;

			//��ʱ��һ�»᲻�����ƶ�����ܴ�����
			if(z_distance(&stInter, &pstMon->stCurrPos) > 1000)
			{
				tlog_info(pstEnv->pstLogCat, 0, 0, "intersection,MonsterID:%d,iPatrolID:%d,MapID:%d,X:%d,Y:%d, next: %d %d, inter: %d %d",
							pstMon->iDefIdx,pstMon->iPatrolID,pstMon->stMap.iID,pstMon->stCurrPos.X, pstMon->stCurrPos.Y,
							stNext.X, stNext.Y, stInter.X, stInter.X);
			}
		}
		else
		{
			//ǿ�ƴ�Խ��·����
			pstMon->stNextPos = stNext;
		}
		pstMon->bPatrolTry = 0;

		break;

	case MOVE_TYPE_ROUNDROLL:
		{
			// Ϊ��ֹ��������������ai_home, ���ʵ�ʰ뾶��׷�а뾶����10����
			int iRetry = 0;
			float fRadius = pstMon->unTraceDist - 10;
			float fNewAngle = pstMon->fRoundRollAngle + PI * 0.5f + (RAND1(PI*10000))/10000.0;
			RESPOS stMaybeNext = { .X = 0, .Y = 0, };
			TMAPMASK *pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask +
			                                 pstMapIdx->iMaskOrigin);

			if (fNewAngle > 2 * PI)
				fNewAngle = fNewAngle - 2 * PI; // ��ֹ�Ƕ�̫�������
			if (fRadius < 0.00001f)
				fRadius = 0.0f;
			stMaybeNext.X = pstMon->stInitPos.X + fRadius * cosf(fNewAngle);
			stMaybeNext.Y = pstMon->stInitPos.Y + fRadius * sinf(fNewAngle);

			while(!IsPixelLineThrough(pstMask, pstMon->stCurrPos.X,
			                          pstMon->stCurrPos.Y,
			                          stMaybeNext.X, stMaybeNext.Y,
			                          maskStop) ||
			      !dyn_msk_check(pstEnv, pstMapInst, pstMask,
			                     stMaybeNext.X, stMaybeNext.Y,
			                     pstMon->stCurrPos.X, pstMon->stCurrPos.Y,
			                     z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP))
			{
				int iWidth = stMaybeNext.X - pstMon->stCurrPos.X;
				int iHeigh = stMaybeNext.Y - pstMon->stCurrPos.Y;

				stMaybeNext.X = pstMon->stCurrPos.X + iWidth / 2;
				stMaybeNext.Y = pstMon->stCurrPos.Y + iHeigh / 2;

				if (iRetry++ >= 4)
				{
					return -1; // ʵ���Ҳ�����Ч�ĵ�
				}
			}
			pstMon->stNextPos = stMaybeNext;
			

			pstMon->fRoundRollAngle = fNewAngle;
			
		}
		break;
	case MOVE_TYPE_FOLLOW_OWNER:
		{
			Player *pstPlayer;

			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iOwnerID);
			if(NULL == pstPlayer)
			{
				return -1;
			}

			if(0 > mon_move_follow_owner(pstEnv, pstMapInst, pstMapIdx, pstMon, pstPlayer))
			{
				return -1;
			}
		}
		break;
	default:
		return -1;
		break;
	}

	return 0;
}

int mon_ai_move(ZONESVRENV* pstEnv,  Monster *pstMon)
{
	int iRet;
	MONSTERDEF *pstMonDef;
	TMAPMASK *pstMask;
			
	if (pstMon->stAtkTargets.iTargetCur > 0)
	{
		pstMon->stAtkTargets.iTargetCur = 0;
	}
	
	pstMon->bAiStat = AI_STAT_MOVE;

	//��Ѳ�߹���Ҳ���ǿ���ƶ����������˲���~
	if(0 == pstMon->iVPlayer && 0 == pstMon->iPatrolID &&
	   pstMon->bMoveType != MOVE_TYPE_FORCED &&
	   !is_mon_bub(pstMon))
	{
		return 0;
	}

	if(pstEnv->pstAppCtx->stCurr.tv_sec <= pstMon->tFreezeEnd ||
	   pstEnv->pstAppCtx->stCurr.tv_sec < pstMon->tBirthEnd)
	{
		return 0;
	}

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef)
	{
		return -1;
	}

	if (pstMon->cMoving && !is_mon_bub(pstMon))
	{
		return 0;
	}

	z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
	
	//ѡ��һ��
	if (0 > mon_select_movepos(pstEnv, pstMon, pstMonDef))
	{
		pstMon->bPatrolTry++;
		if (pstMon->bPatrolTry > MAX_PATROL_TRY)
		{
			tlog_info(pstEnv->pstLogCat, 0, 0, "patrol try max,MonsterID:%d,iPatrolID:%d,MapID:%d,X:%d,Y:%d",
							pstMon->iDefIdx,pstMon->iPatrolID,pstMon->stMap.iID,pstMon->stCurrPos.X, pstMon->stCurrPos.Y);
			mon_ai_home(pstEnv, pstMon);
		}
		
		return -1;
	}

	iRet = mon_have_movespd(pstMon);
	if (0 == iRet) //����
	{
		return 0;
	}
	else
	{
		pstMon->stMspdInfo.MoveSpdCurr = iRet;
		pstMon->stMspdInfo.NextNum = 0;
	}

	if (pstMon->cType == OBJ_NPC)
	{
		ZONEMAPINST *pstMapInst;
		MapIndex *pstMapIdx;
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
		if (NULL == pstMapInst)
		{
			return -1;
		}
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (NULL == pstMapIdx)
		{
			return -1;
		}
		pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);
		if (!IsPixelLineThrough(pstMask, pstMon->stCurrPos.X,
				                          pstMon->stCurrPos.Y,
				                          pstMon->stInitPos.X, pstMon->stInitPos.Y,
				                          maskStop))
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
			z_safe_monster_entermap(pstEnv->pstAppCtx, pstEnv, pstMon, pstMon->stMap.iID, &pstMon->stInitPos);
			return 0;
		}
	}
	
	

	pstMon->cMoving = 1;
	z_monster_move_notify(pstEnv->pstAppCtx, pstEnv, pstMon, CS_MOVE_WALK);
	TV_CLONE(pstMon->stLastMoveTv, pstEnv->pstAppCtx->stCurr);

	return 0;
}

int mon_ai_evt_sysout(ZONESVRENV* pstEnv, Monster *pstMon)
{
	TMEMBLOCK *pstMemBlock;
	
	mon_patrol_script(pstEnv, pstMon);

	if(pstMon->cType == OBJ_MONSTER)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, pstMon->iMIndex);
		if (!pstMemBlock->fValid)
		{
			return 0;
		}
	}
	else if(pstMon->cType == OBJ_NPC)
	{
		pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, pstMon->iMIndex);
		if (!pstMemBlock->fValid)
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
	

	if (ATK_TYPE_INITIATIVE == pstMon->bAtkType)
	{
		if (0 > mon_ai_select_target(pstEnv, pstMon))
		{
			if (MOVE_TYPE_NO == pstMon->bMoveType)
			{
				pstMon->bAiStat = AI_STAT_IDLE;
				return 0;
			}
			else //Ѳ��/���/ǿ���ƶ�
			{
				mon_ai_move(pstEnv, pstMon);
				return 0;
			}
		}
		else //������׷��
		{
			if (0 > mon_ai_atk(pstEnv, pstMon, 1)) 
			{
				/*
				if (MOVE_TYPE_NO == pstMon->bMoveType)
				{
					pstMon->bAiStat = AI_STAT_IDLE;
					return 0;
				}
				else*/
				{
					mon_ai_trace(pstEnv, pstMon, 1);
				}
			}
		}
	}
	else
	{
		if (MOVE_TYPE_NO == pstMon->bMoveType)
		{
			pstMon->bAiStat = AI_STAT_IDLE;
			return 0;
		}
		else //Ѳ��/���/ǿ���ƶ�
		{
			mon_ai_move(pstEnv, pstMon);
			return 0;
		}
	}
	
	return 0;
}

int mon_ai_evt_inpress(ZONESVRENV* pstEnv, Monster *pstMon, AIPress *pstAIPress)
{
	int iTargetID;
	int iIdx;

	if (OBJ_PLAYER == pstAIPress->stAni.iType)
	{
		iTargetID = pstAIPress->stAni.stObj.pstPlayer->iMemID;
	}
	else if (OBJ_MONSTER == pstAIPress->stAni.iType)
	{
		iTargetID = pstAIPress->stAni.stObj.pstMon->iID;
	}
	else
	{
		return -1;
	}
	
	if (iTargetID != pstMon->stAtkTargets.iTargetCur && iTargetID == mon_press_max(pstEnv->pstAppCtx, pstEnv, pstMon, &iIdx))
	{
		int iDelay = 1;
		
		if (0 > mon_ai_select_target(pstEnv, pstMon))
		{
			return -1;
		}

		if (pstMon->tFreezeEnd < pstEnv->pstAppCtx->stCurr.tv_sec )
		{
			pstMon->tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec ;
		}

		if(pstEnv->pstAppCtx->stCurr.tv_sec <= pstMon->tBirthEnd)
		{
			pstMon->bAiStat = AI_STAT_TRACE;
			return 0;
		}

		//�����ƶ��������֣�ֱ�ӿ���
		if(pstMon->bMoveType == MOVE_TYPE_NO && pstMon->bAtkType == ATK_TYPE_INITIATIVE)
		{
			iDelay = 0;
		}

		if (0 > mon_ai_atk(pstEnv, pstMon, iDelay)) 
		{
			/*
			if (MOVE_TYPE_NO == pstMon->bMoveType)
			{
				pstMon->bAiStat = AI_STAT_IDLE;
				return 0;
			}
			else*/
			{
				mon_ai_trace(pstEnv, pstMon, 0);
			}
		}		
	}
	
	return 0;
}

int mon_ai_evt_mark(ZONESVRENV* pstEnv, Monster *pstMon, AIMark *pstAIMark)
{
	MONSTERDEF *pstMonDef = NULL;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	if(pstMon->bAtkType == ATK_TYPE_NO)
	{
		return -1;
	}

	pstMonDef = z_get_mon_def(pstEnv,  pstMon);
	if(!pstMonDef)
	{
		return -1;
	}

	pstTargetList->tMarkedEnd = pstEnv->pstAppCtx->stCurr.tv_sec +
	                            pstAIMark->iTime;
	switch(pstAIMark->stAtkee.iType)
	{
	case OBJ_PLAYER:
		pstTargetList->iMarkedID = pstAIMark->stAtkee.stObj.pstPlayer->iMemID;
		mon_press_insert_player(pstEnv, pstMon, pstMonDef,
		                        pstAIMark->stAtkee.stObj.pstPlayer);
		break;

	case OBJ_MONSTER:
		pstTargetList->iMarkedID = pstAIMark->stAtkee.stObj.pstMon->iID;
		mon_press_insert_mon(pstEnv, pstMon, pstAIMark->stAtkee.stObj.pstMon);
		break;

	case OBJ_PET:
		pstTargetList->iMarkedID = 
			pstAIMark->stAtkee.stObj.stZonePet.pstOwner->iMemID;
		mon_press_insert_player(pstEnv, pstMon, pstMonDef,
		                        pstAIMark->stAtkee.stObj.stZonePet.pstOwner);
		break;

	default:
		pstTargetList->tMarkedEnd = 0;
		return -1;
		break;
	}
	
	if(pstTargetList->iMarkedID == pstTargetList->iTargetCur)
	{
		return 0;
	}

	if(mon_ai_select_target(pstEnv, pstMon) < 0)
	{
		return -1;
	}

	if(mon_ai_atk(pstEnv, pstMon, 1) < 0)
	{
		/*
		if(pstMon->bMoveType == MOVE_TYPE_NO)
		{
			pstMon->bAiStat = AI_STAT_IDLE;
			return 0;
		}
		else*/
		{
			mon_ai_trace(pstEnv, pstMon, 1);
		}
	}
	
	return 0;
}

int mon_ai_evt_unmark(ZONESVRENV* pstEnv, Monster *pstMon, AIMark *pstAIMark)
{
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	if(pstMon->bAtkType == ATK_TYPE_NO)
	{
		return -1;
	}

	switch(pstAIMark->stAtkee.iType)
	{
	case OBJ_PLAYER:
		if(pstTargetList->iMarkedID !=
		   pstAIMark->stAtkee.stObj.pstPlayer->iMemID)
		{
			return -1;
		}
		break;

	case OBJ_MONSTER:
		if(pstTargetList->iMarkedID != pstAIMark->stAtkee.stObj.pstMon->iID)
		{
			return -1;
		}
		break;

	case OBJ_PET:
		if(pstTargetList->iMarkedID !=
		   pstAIMark->stAtkee.stObj.stZonePet.pstOwner->iMemID)
		{
			return -1;
		}
		break;

	default:
		return -1;
		break;
	}

	// ���ȡ����
	pstTargetList->iMarkedID = 0;
	pstTargetList->tMarkedEnd = 0;
	return mon_ai_evt_outpress(pstEnv, pstMon);
}

int mon_ai_press_modify(ZONESVRENV* pstEnv, Monster *pstMon, int iID, int iVal)
{
	int iIdx;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	
	iIdx = mon_press_find(pstMon, iID);
	if (iIdx >= 0)
	{
		pstTargetList->astPressEntrys[iIdx].iPress = iVal;
	}

	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int get_mon_press_max(TAPPCTX *pstAppCtx, Monster *pstMon, int *piIdx)
{
	int i;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	int iMaxPress = pstTargetList->astPressEntrys[0].iPress;
	int iIdx = 0;
	
	if (0 >= pstTargetList->PressNum)
	{
		return -1;
	}

	for (i=1; i<pstTargetList->PressNum; i++)
	{
		if (pstTargetList->astPressEntrys[i].iPress > iMaxPress)
		{
			iMaxPress = pstTargetList->astPressEntrys[i].iPress;
			iIdx = i;
		}
	}

	if (piIdx)
	{
		*piIdx = iIdx;
	}

	return pstTargetList->astPressEntrys[iIdx].iTargetID;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

int mon_ai_evt_copy_press(ZONESVRENV* pstEnv, Monster *pstMon, AICopyPress *pstAICopyPress)
{
	int iIdx, iTarget, iPressVal;
	MONSTERDEF *pstMonDef;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	
	if(ATK_TYPE_NO == pstMon->bAtkType)
	{
		return -1;
	}

	if(pstTargetList->iMarkedID > 0 &&
	   pstTargetList->tMarkedEnd >= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	pstMonDef = z_get_mon_def( pstEnv,  pstMon);
	if (pstMonDef)
	{
		mon_press_insert_player(pstEnv, pstMon, pstMonDef, pstAICopyPress->pstPlayer);
	}

	//���Ƶ�ǰѹ����ѹ����ߵ��˵�ѹ��ֵ
	iTarget = get_mon_press_max(pstEnv->pstAppCtx, pstMon, &iIdx);
	if (0 > iTarget || iTarget == pstAICopyPress->pstPlayer->iMemID)
	{
		return -1;
	}
	iPressVal = pstTargetList->astPressEntrys[iIdx].iPress;

	//�޸�ѹ��ֵ
	mon_ai_press_modify(pstEnv, pstMon, pstAICopyPress->pstPlayer->iMemID, iPressVal+1);

	//�ٴ�ѡ��Ŀ��
	if (0 > mon_ai_select_target(pstEnv, pstMon))
	{
		return -1;
	}

	if (0 > mon_ai_atk(pstEnv, pstMon, 1)) 
	{
		/*
		if (MOVE_TYPE_NO == pstMon->bMoveType)
		{
			pstMon->bAiStat = AI_STAT_IDLE;
			return 0;
		}
		else*/
		{
			mon_ai_trace(pstEnv, pstMon, 1);
		}
	}
	
	return 0;
}

int mon_ai_evt_bedefy(ZONESVRENV* pstEnv, Monster *pstMon, AIBeDefy *pstAIBeDefy)
{
	MONSTERDEF *pstMonDef;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	
	//todo: �����⳰��
	if(ATK_TYPE_NO == pstMon->bAtkType)
	{
		return -1;
	}

	if(pstTargetList->iMarkedID > 0 &&
	   pstTargetList->tMarkedEnd >= pstEnv->pstAppCtx->stCurr.tv_sec)
	{
		return -1;
	}

	pstTargetList->iDefyID = pstAIBeDefy->pstPlayer->iMemID;
	pstTargetList->tDefyEnd = pstEnv->pstAppCtx->stCurr.tv_sec + pstAIBeDefy->iTime;
	pstMonDef = z_get_mon_def( pstEnv,  pstMon);
	if (pstMonDef)
	{
		mon_press_insert_player(pstEnv, pstMon, pstMonDef, pstAIBeDefy->pstPlayer);
	}
	
	if (pstTargetList->iDefyID == pstTargetList->iTargetCur)
	{
		return 0;
	}

	if (0 > mon_ai_select_target(pstEnv, pstMon))
	{
		return -1;
	}

	if (0 > mon_ai_atk(pstEnv, pstMon, 1)) 
	{
		/*
		if (MOVE_TYPE_NO == pstMon->bMoveType)
		{
			pstMon->bAiStat = AI_STAT_IDLE;
			return 0;
		}
		else*/
		{
			mon_ai_trace(pstEnv, pstMon, 1);
		}
	}
	
	return 0;
}

int mon_patrol_script(ZONESVRENV* pstEnv, Monster *pstMon)
{
	PATROLDEF *pstPatrolDef;
	char szName[128];
	int iRet = 0;
	
	if ( (pstMon->cMonScriptFlag & MON_SCRIPT_PROC_PATROL_MAP) && 0 == pstMon->cMoving && pstMon->iPatrolID > 0 &&
			0 == pstMon->bPatrolTurn && 0 == pstMon->cPatrolScript)
	{
		
		pstPatrolDef = z_find_patroldef(pstEnv, pstMon->iPatrolID, &pstMon->iPatrolIdx);
		if (NULL == pstPatrolDef)
		{
			return -1;
		}
		
		//if (pstMon->bPatrolPos >= MAX_PATROL_POINT-1 ||0 ==  pstPatrolDef->PatrolPos[pstMon->bPatrolPos+1].X)
		if(pstMon->bPatrolPos < MAX_PATROL_POINT &&
		   pstPatrolDef->PatrolPos[pstMon->bPatrolPos].X != 0)
		{
			if (pstMon->stCurrPos.X == pstPatrolDef->PatrolPos[pstMon->bPatrolPos].X && 
				pstMon->stCurrPos.Y == pstPatrolDef->PatrolPos[pstMon->bPatrolPos].Y)
			{	
				stSEnv.pstMonster = pstMon;
				stSEnv.pstNpc = NULL;
				stSEnv.pstPlayer = NULL;
				stSEnv.pstPworldInst = NULL;
				stSEnv.pstKiller = NULL;
				stSEnv.pstMapinst = NULL;

				snprintf(szName, sizeof(szName), "monster%d.mac", pstMon->iDefIdx);
				szName[sizeof(szName) - 1] = 0;

				if(pstMon->bPatrolPos >= MAX_PATROL_POINT - 1 ||
				   pstPatrolDef->PatrolPos[pstMon->bPatrolPos + 1].X == 0)
				{
					iRet = 1;
					pstMon->cPatrolScript = 1;
					
					z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName,
					              &pstMon->stScriptIdx, MON_SCRIPT_PROC_PATROL);
				}
				else
				{
					z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName,
					              &pstMon->stScriptIdx,
					              MON_SCRIPT_PROC_PATROL_MIDDLE);
				}
			}
		}
	}

	return iRet;
}

int mon_ai_evt_outpress(ZONESVRENV* pstEnv, Monster *pstMon)
{
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	switch(pstMon->bAiStat)
	{
		case AI_STAT_ATK:
			if (MOVE_TYPE_NO == pstMon->bMoveType)
			{
				if (0 == pstTargetList->PressNum &&
				    0 >= pstTargetList->iDefyID &&
				    0 >= pstTargetList->iMarkedID)
				{
					if(pstMon->stMonAtkInfo.cAtkStat > 0)
					{
						z_mon_skill_break(pstEnv, pstMon);
					}
					
					pstMon->bAiStat = AI_STAT_IDLE;
					return 0;
				}

				if (0 >= pstTargetList->iTargetCur)
				{
					if (0 <= mon_ai_select_target(pstEnv, pstMon))
					{
						if (0 <= mon_ai_atk(pstEnv, pstMon, 1))
						{
							return 0;
						}
					}

					pstMon->bAiStat = AI_STAT_IDLE;

					return 0;
				}
			}
			else
			{
				if (0 == pstTargetList->PressNum &&
				    0 >= pstTargetList->iDefyID &&
				    0 >= pstTargetList->iMarkedID)
				{
					if(pstMon->stMonAtkInfo.cAtkStat > 0)
					{
						z_mon_skill_break(pstEnv, pstMon);
					}
					
					if (pstMon->tFreezeEnd < pstEnv->pstAppCtx->stCurr.tv_sec)
					{
						mon_ai_move(pstEnv, pstMon);
					}
					else
					{
						pstMon->bAiStat = AI_STAT_MOVE;
					}
					
					return 0;
				}

				if (0 >= pstTargetList->iTargetCur)
				{
					if (0 > mon_ai_select_target(pstEnv, pstMon))
					{
						return -1;
					}

					if (0 > mon_ai_atk(pstEnv, pstMon, 1)) 
					{
						mon_ai_trace(pstEnv, pstMon, 1);
					}	
				}
			}
			
			break;
			
		case AI_STAT_TRACE:
			if (0 == pstTargetList->PressNum &&
			    0 >= pstTargetList->iDefyID &&
			    0 >= pstTargetList->iMarkedID)
			{
				if (MOVE_TYPE_NO == pstMon->bMoveType)
				{
					pstMon->bAiStat = AI_STAT_IDLE;
					return 0;
				}
				else
				{
					mon_ai_move(pstEnv, pstMon);
					return 0;
				}
			}

			if (0 >= pstTargetList->iTargetCur)
			{
				if (0 > mon_ai_select_target(pstEnv, pstMon))
				{
					return -1;
				}

				if (0 > mon_ai_atk(pstEnv, pstMon, 1)) 
				{
					mon_ai_trace(pstEnv, pstMon, 1);
				}	
			}
			break;

		case AI_STAT_IDLE:
			if (ATK_TYPE_NO == pstMon->bAtkType)
			{
				break;
			}
			
			if (pstTargetList->PressNum > 0)
			{
				if (0 > mon_ai_select_target(pstEnv, pstMon))
				{
					return -1;
				}

				mon_ai_atk(pstEnv, pstMon, 1);
			}
			break;
			
		default:
			return -1;
			break;
	}
	
	return 0;
}

//�ͷż��ܵ�����,û������ֱ�ӿ��Էż���
int mon_ai_check_skill_conditionid(ZONESVRENV* pstEnv, Monster *pstMon,
		SKILLDEF *pstSkillDef, int ConditionID, int ConditionVal, int iBeAtkHurt)
{
	ZoneStatus *pstZoneStatus = NULL;
		
	switch(ConditionID)
	{
		case INSERT_SKILL_CONDITION_NO:
			break;
		case INSERT_SKILL_CONDITION_DEAD:
			if (0 != pstMon->iHP)
			{
				return -1;
			}
			break;
		case INSERT_SKILL_CONDITION_HPDEC:
			if (!((pstMon->iHP+iBeAtkHurt) >= (ConditionVal/100.0)*pstMon->iMaxHP && 
				pstMon->iHP < (ConditionVal/100.0)*pstMon->iMaxHP))
			{
				return -1;
			}
			break;
		case INSERT_SKILL_CONDITION_STATUSID:
			pstZoneStatus = z_mon_find_status(pstMon, ConditionVal);
			if(!pstZoneStatus)
			{
				return -1;
			}
			break;
		default:
			break;
	}

	return 0;
	UNUSED(pstEnv);
	UNUSED(pstSkillDef);
}
//Ŀǰֻ֧����˲���ļ���
int mon_ai_beatk_skill(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, AIBeAtk *pstAIBeAtk)
{
	int i;
	struct timeval stDiff;
	long long llMs;
	MonInsertSkill *pstInsertSkill;
	SKILLDEF *pstSkillDef;
	ZoneAni stAni;
	TMAPMASK *pstMask;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	int iFlag = 0;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;

	UNUSED(pstMonDef);

	if(is_mon_machine(pstMon) || is_mon_factory(pstMon))
	{
		return -1;
	}

	if(pstMonAtkInfo->cAtkStat)
	{
		return 0;
	}

	pstMonAtkInfo->DirFace = 0;
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	for (i=0; i<MAX_ATKEE_SKILL; i++)
	{
		pstInsertSkill = pstMon->astAtkeeInsertSkill + i;
			
		if (0 == pstInsertSkill->stSkill.SkillID)
		{
			break;
		}

		pstSkillDef = z_find_skill(pstEnv, pstInsertSkill->stSkill.SkillID, pstInsertSkill->stSkill.SkillLevel,
												&pstInsertSkill->iSkillIdx);
		if (NULL == pstSkillDef)
		{
			continue;
		}

		if ((int)(RAND1(10000)) >= 100*pstInsertSkill->stSkill.Rate)
		{
		     continue;
		}

		if (pstInsertSkill->iOnce && pstInsertSkill->stSkill.Once)
		{
			continue;
		}

		if(0 > mon_ai_check_skill_conditionid(pstEnv,pstMon,pstSkillDef,
					pstInsertSkill->stSkill.ConditionID,pstInsertSkill->stSkill.ConditionVal,pstAIBeAtk->iHurt) )
		{
			if(INSERT_SKILL_CONDITION_HPDEC == pstInsertSkill->stSkill.ConditionID &&
				pstInsertSkill->stSkill.Once &&
				pstInsertSkill->iOnce == 0)
			{
				if (pstMon->iHP <= pstInsertSkill->stSkill.ConditionVal/100.0*pstMon->iMaxHP )
				{
					//��ֹѪ��Խ���߽磬���Ǽ��ܻ�δ����
				}
				else
				{
					continue;
				}
			}
			else
			{
				continue;
			}
			
		}
		/*switch(pstInsertSkill->stSkill.ConditionID)
		{
			case INSERT_SKILL_CONDITION_NO:
				break;
			case INSERT_SKILL_CONDITION_DEAD:
				if (0 != pstMon->iHP)
				{
					continue;
				}
				break;
			case INSERT_SKILL_CONDITION_HPDEC:
				if (!((pstMon->iHP+pstAIBeAtk->iHurt) >= (pstInsertSkill->stSkill.ConditionVal/100.0)*pstMon->iMaxHP && 
					pstMon->iHP < (pstInsertSkill->stSkill.ConditionVal/100.0)*pstMon->iMaxHP))
				{
					continue;
				}
				break;
			default:
				continue;
				break;
		}*/

		TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstInsertSkill->stCoolEnd);
		TV_TO_MS(llMs, stDiff);
		if (llMs < 0)
		{
			continue;
		}

		if ((pstSkillDef->TargetType & (SKILL_TARGET_FRIEND_MON | SKILL_TARGET_MID_MON |SKILL_TARGET_SELF)) ||
			pstSkillDef->Distance <= 0)
		{
			stAni.iType = OBJ_MONSTER;
			stAni.stObj.pstMon = pstMon;
		}
		else
		{
			memcpy(&stAni, &pstAIBeAtk->stAtker, sizeof(stAni));
		}

		if (0 == mon_ai_skill_valid(pstEnv, pstMapInst, pstMask, pstMon, &stAni, pstSkillDef))
		{
			continue;
		}

		iFlag = 1;
		break;
	}

	if (0 == iFlag)
	{
		return -1;
	}

	//����Ƕ��
	if (pstMon->stAIBeAtkSKill.tv_sec != 0)
	{
		TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstMon->stAIBeAtkSKill);
		TV_TO_MS(llMs, stDiff);
		if (llMs < 20)
		{
			return -1;
		}
	}
	TV_CLONE(pstMon->stAIBeAtkSKill, pstEnv->pstAppCtx->stCurr);

	if (pstMonAtkInfo->cAtkStat)
	{
		z_mon_skill_break(pstEnv, pstMon);
	}

	pstMonAtkInfo->unSkillID = pstSkillDef->SkillID;
	pstMonAtkInfo->bSkillLevel = pstSkillDef->Level;
	pstMonAtkInfo->iSkillIdx = pstInsertSkill->iSkillIdx;
	pstMonAtkInfo->bSkillType = MON_SKILL_TYPE_ATKEE;
	pstMonAtkInfo->bMonSkillIdx = i;

	pstMon->bAiStat = AI_STAT_ATK;

	mon_ai_start_atk(pstEnv, pstMon, &stAni);

	return 0;
}

int mon_ai_totem_addpress(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstZoneAni, int iPress)
{
	int iAtkerID;
	int iIdx;
	AtkTargetList *pstAtkTarget;

	if (OBJ_PLAYER == pstZoneAni->iType)
	{
		iAtkerID = pstZoneAni->stObj.pstPlayer->iMemID;
	}
	else if (OBJ_MONSTER == pstZoneAni->iType)
	{
		iAtkerID = pstZoneAni->stObj.pstMon->iID;
	}
	else if (OBJ_NPC== pstZoneAni->iType)
	{
		iAtkerID = pstZoneAni->stObj.pstNpc->stMon.iID;
	}
	else
	{
		return -1;
	}

	pstAtkTarget = &pstMon->stAtkTargets;
	iIdx = mon_press_find(pstMon, iAtkerID);
	if (iIdx >= 0)
	{
		pstAtkTarget->astPressEntrys[iIdx].iPress += iPress;
	}
	else
	{
		return -1;
	}

	if (iAtkerID != pstAtkTarget->iTargetCur && iAtkerID == mon_press_max(pstEnv->pstAppCtx, pstEnv, pstMon, &iIdx))
	{
		if (0 > mon_ai_select_target(pstEnv, pstMon))
		{
			return -1;
		}

		if (0 > mon_ai_atk(pstEnv, pstMon, 1)) 
		{
			/*
			if (MOVE_TYPE_NO == pstMon->bMoveType)
			{
				pstMon->bAiStat = AI_STAT_IDLE;
				return 0;
			}
			else*/
			{
				mon_ai_trace(pstEnv, pstMon, 1);
			}
		}		
	}

	return 0;
}

int mon_ai_addpress(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstZoneAni, int iPress)
{
	int iAtkerID;
	int iIdx;
	AtkTargetList *pstAtkTarget;

	if(pstMon->bSubType == MON_SUB_TYPE_TOTEM)
	{
		return -1;
	}
	
	if (OBJ_PLAYER == pstZoneAni->iType)
	{
		iAtkerID = pstZoneAni->stObj.pstPlayer->iMemID;
	}
	else if (OBJ_MONSTER == pstZoneAni->iType)
	{
		iAtkerID = pstZoneAni->stObj.pstMon->iID;
	}
	else
	{
		return -1;
	}

	if(pstZoneAni->iType == OBJ_PLAYER &&
	   z_is_player_status_result(pstEnv,
	                             pstZoneAni->stObj.pstPlayer,
	                             STATUS_RESULT_PRESS_MUL_CHG))
	{
		STATUSRESULT stResult;

		// ����STATUS��ѹ���ı仯��
		z_get_player_status_result(
			pstEnv,
			pstZoneAni->stObj.pstPlayer,
		    STATUS_RESULT_PRESS_MUL_CHG,
		    &stResult,
		    0);

		if(stResult.ResultVal1 == 0)
		{
			// +x%
			iPress += ((iPress * stResult.ResultVal2) / 100);
		}
		else
		{
			// -x%
			iPress -= ((iPress * stResult.ResultVal2) / 100);
		}
	}

	pstAtkTarget = &pstMon->stAtkTargets;
	iIdx = mon_press_find(pstMon, iAtkerID);
	if (iIdx >= 0)
	{
		pstAtkTarget->astPressEntrys[iIdx].iPress += iPress;
	}
	else
	{
		//ͼ�ڹ���������Ŀ�������һЩӰ�죬�����ٳ���inpress
		return -1;
	}

	if (iAtkerID != pstAtkTarget->iTargetCur && iAtkerID == mon_press_max(pstEnv->pstAppCtx, pstEnv, pstMon, &iIdx))
	{
		if (0 > mon_ai_select_target(pstEnv, pstMon))
		{
			return -1;
		}

		if (0 > mon_ai_atk(pstEnv, pstMon, 1)) 
		{
			/*
			if (MOVE_TYPE_NO == pstMon->bMoveType)
			{
				pstMon->bAiStat = AI_STAT_IDLE;
				return 0;
			}
			else*/
			{
				mon_ai_trace(pstEnv, pstMon, 1);
			}
		}		
	}

	return 0;
}

Player *mon_get_target(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAtker)
{
	AtkTargetList *pstTargetList; 
	Player *pstPlayerAtker;

	pstTargetList = NULL;
	pstPlayerAtker = NULL;
	
       /* ֻȡ�������Ϣ */
	if (pstAtker != NULL  &&  OBJ_PLAYER == pstAtker->iType)
	{
		return pstAtker->stObj.pstPlayer;
	}
	else
	{
		pstTargetList = &pstMon->stAtkTargets;
		if( 0 >= pstTargetList->iTargetCur )
		{
			if( 0 > mon_ai_select_target(pstEnv,pstMon) )
			{
				goto quit;
			}
		}

		pstPlayerAtker = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTargetList->iTargetCur);
		if (pstPlayerAtker && pstPlayerAtker->eStatus == PLAYER_STATUS_ROLE_LOGIN 
			&& !(pstPlayerAtker->stOnline.State & CS_STAT_DEAD))
		{
			if (pstPlayerAtker->stOnline.cMoving)
			{
				z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayerAtker, 0);
			}
				
			return pstPlayerAtker;
		}
		else
		{
			goto quit;
		}
	}

quit:
	if (pstMon->iAtkerID)
	{
		pstPlayerAtker = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iAtkerID);
		if (pstPlayerAtker && pstPlayerAtker->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			return pstPlayerAtker;
		}
	}
		
	return NULL;
}

int is_mon_help_buddy(MONSTERDEF *pstMonDef, int iBuddyDefID)
{
	int i;

	if (pstMonDef->CallHelpMonsterID[0] == 0)
	{
		//Ĭ�Ϻ��з�Χ�ڵ����й�
		return 1;
	}
	for (i=0; i<MAX_CALL_MOSTER_NUM; i++)
	{
		if (pstMonDef->CallHelpMonsterID[i] == 0)
			break;
		
		if ((int)pstMonDef->CallHelpMonsterID[i] == iBuddyDefID)
		{
			return 1;
		}
	}

	return 0;
}

int mon_help_by_map(ZONESVRENV* pstEnv, Monster *pstMon, 
	MONSTERDEF *pstMonDef, ZoneAni *pstAtker)
{
	MapIndex *pstMapIdx;
	TMAPMASK *pstMask;
	ZONEMAPINST *pstMapInst;
	SearchArea stSArea;
	int iDist = pstMonDef->CallHelpDist;

	pstMapInst	=	z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	if(0 == z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, &stSArea))
	{
		int i, j;
		AREADYN *pstArea;
		Monster *pstMonBuddy;
		MONSTERDEF *pstMonBuddyDef;

		for(i = 0; i < stSArea.iNum; i++)
		{
			pstArea = stSArea.pastArea[i];

			for (j = 0; j < pstArea->iAnimate; j++)
			{
				pstMonBuddy = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
				if(NULL == pstMonBuddy)
				{
					continue;
				}

				//��ֹ��ѭ��   help->insert->help->insert....
				if(pstMonBuddy == pstMon)
				{
					continue;
				}

				//ֻ������ͨ�Ĺ���
				if(pstMonBuddy->bSubType >= MON_SUB_TYPE_SINGLE_CAR|| pstMonBuddy->iOwnerID > 0)
				{
					continue;
				}

				pstMonBuddyDef = z_get_mon_def(pstEnv, pstMonBuddy);
				if (!pstMonBuddyDef)
				{
					continue;
				}

				//�ɺ��ȵĹ���
				if(0 == is_mon_help_buddy(pstMonDef, pstMonBuddyDef->MonsterID))
				{
					continue;
				}

				if(pstMonBuddy->cStat != MONSTER_ALIVE ||
					iDist < z_distance(&pstMonBuddy->stCurrPos, &pstMon->stCurrPos))
				{
					continue;
				}

				if (pstAtker->iType == OBJ_PLAYER)
				{	
					if(is_mon_enemy_player(pstEnv, pstMon, pstAtker->stObj.pstPlayer))
					{
						//��ֹ��ѭ��
						mon_press_insert_player2(pstEnv, pstMonBuddy, pstMonBuddyDef,
											pstAtker->stObj.pstPlayer);
					}
				}
				else if(pstAtker->iType == OBJ_MONSTER)
				{
					if(is_mon_enemy_mon(pstEnv, pstMonBuddy, pstAtker->stObj.pstMon))
					{
						//��ֹ��ѭ��
						mon_press_insert_mon2(pstEnv, pstMonBuddy, pstAtker->stObj.pstMon);
					}
				}	
			}
		}
	}

	return 0;
}

int mon_help_copy_press_list(ZONESVRENV* pstEnv, Monster *pstMon, 
	Monster *pstMonBuddy, MONSTERDEF *pstMonBuddyDef)
{
	int i, iIndex;
	char cType;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	for (i=0; i<pstTargetList->PressNum; i++)
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID, &cType);
		if (0 > iIndex)
		{
			continue;
		}

		if (cType == OBJ_PLAYER)
		{
			Player *pstTarget;
		
			pstTarget = z_id_player(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
			if(pstTarget == NULL  ||(pstTarget->stOnline.State & CS_STAT_DEAD) ||pstTarget->eStatus != PLAYER_STATUS_ROLE_LOGIN)
			{
				continue;
			}

			if(is_mon_enemy_player(pstEnv, pstMonBuddy, pstTarget))
			{
				mon_press_insert_player(pstEnv, pstMonBuddy, pstMonBuddyDef, pstTarget);
			}
		}
		else if(cType == OBJ_MONSTER)
		{
			Monster *pstTarget;
		
			pstTarget = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstTargetList->astPressEntrys[i].iTargetID);
			if(pstTarget == NULL)
			{
				continue;
			}
			
			if(is_mon_enemy_mon(pstEnv, pstMonBuddy, pstTarget))
			{
				mon_press_insert_mon(pstEnv, pstMonBuddy, pstTarget);
			}
		}
	}

	return 0;
}

int mon_help(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, 
				Player *pstTarget,AIBeAtk *pstAIBeAtk, int fHelpDist)
{
	int i;
	int j;
	Monster *pstMonBuddy; //���� ͬ���ָ��
	MONSTERDEF *pstMonBuddyDef;
	int iDist = pstMonDef->CallHelpDist;
	

	i = 0;
	j = 0;
	pstMonBuddy = NULL;

	if(fHelpDist > 0)
	{
		iDist = fHelpDist;
	}
	
	/* ��ȡ�����Χ�Ĺ���,���Ƿ��ǵ�ǰ����Ļ�� */
	for(i=0;i<pstTarget->stOnline.iVAnimate;i++)
	{
		pstMonBuddy = z_id_monster(pstEnv->pstAppCtx,pstEnv,pstTarget->stOnline.aiVAnimate[i]);
		if (pstMonBuddy && pstMonBuddy->cStat == MONSTER_ALIVE && iDist > z_distance(&pstMonBuddy->stCurrPos, &pstMon->stCurrPos))
		{
			//ֻ������ͨ�Ĺ���
			if(pstMonBuddy->bSubType >= MON_SUB_TYPE_SINGLE_CAR|| pstMonBuddy->iOwnerID > 0)
			{
				continue;
			}
			
			pstMonBuddyDef = z_get_mon_def(pstEnv, pstMonBuddy);
			if (!pstMonBuddyDef)
			{
				continue;
			}

			//�����ڵ����й��ﶼ����
			if(fHelpDist > 0 || pstMonDef->CallHelpMonsterID[0] == 0)
			{
				if (pstAIBeAtk->stAtker.iType == OBJ_PLAYER)
				{
					if(is_mon_enemy_player(pstEnv, pstMon, pstAIBeAtk->stAtker.stObj.pstPlayer))
					{
						mon_press_insert_player(pstEnv, pstMonBuddy, pstMonBuddyDef,
											pstAIBeAtk->stAtker.stObj.pstPlayer);
					}
				}
				else if(pstAIBeAtk->stAtker.iType == OBJ_MONSTER)
				{
					if(is_mon_enemy_mon(pstEnv, pstMonBuddy, pstAIBeAtk->stAtker.stObj.pstMon))
					{
						mon_press_insert_mon(pstEnv, pstMonBuddy,
											pstAIBeAtk->stAtker.stObj.pstMon);
					}
					
				}

				mon_help_copy_press_list(pstEnv, pstMon, pstMonBuddy, pstMonBuddyDef);
				
				continue;
			}

			for (j=0; j<MAX_CALL_MOSTER_NUM; j++)
			{
				if (pstMonDef->CallHelpMonsterID[j] == 0)
					break;
				
				if (pstMonDef->CallHelpMonsterID[j] == pstMonBuddyDef->MonsterID)
				{
					if (pstAIBeAtk->stAtker.iType == OBJ_PLAYER)
					{
						if(is_mon_enemy_player(pstEnv, pstMon, pstAIBeAtk->stAtker.stObj.pstPlayer))
						{
							mon_press_insert_player(pstEnv, pstMonBuddy, pstMonBuddyDef,
													pstAIBeAtk->stAtker.stObj.pstPlayer);
						}
					}
					else if(pstAIBeAtk->stAtker.iType == OBJ_MONSTER)
					{
						if(is_mon_enemy_mon(pstEnv, pstMonBuddy, pstAIBeAtk->stAtker.stObj.pstMon))
						{
							mon_press_insert_mon(pstEnv, pstMonBuddy,
													pstAIBeAtk->stAtker.stObj.pstMon);
						}
					}

					mon_help_copy_press_list(pstEnv, pstMon, pstMonBuddy, pstMonBuddyDef);

					break;
				}
			}
		}
	}
	 
	return 0;
}

int mov_ai_evt_beatk(ZONESVRENV* pstEnv, Monster *pstMon, AIBeAtk *pstAIBeAtk)
{
	MONSTERSPEECHDEF *pstMonSpeech;
	MONSTERDEF *pstMonDef;
	int iPress = 0;
	//int iIndex;
	//char cType;
	//TMEMBLOCK *pstMemBlock;
	//Monster *pstCallHelpMonster;
	int iRand;
	//MonAtkInfo *pstMonAtkInfo;
	//SKILLDEF *pstSkillDef;
	Player *pstTarget = NULL;
	int fHelpDist = 0;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (!pstMonDef)
	{
		return -1;
	}	

	//����������������
	mon_ai_beatk_skill(pstEnv, pstMon, pstMonDef, pstAIBeAtk);

	// ǿ���ƶ���׷����
	if(pstMon->bMoveType == MOVE_TYPE_FORCED)
	{
		return 0;
	}

	//�ٲ���ǿ�����й��ﱻ����������Χ�Ĺ��
	if(GET_PWORLDID(pstMon->stMap.iID) == BCT_PWORLD_ID && 
		((pstMon->iHP < pstMon->iMaxHP) &&
		((pstMon->iHP+pstAIBeAtk->iHurt) >= pstMon->iMaxHP)))
	{
		fHelpDist = BCT_HELP_MON_DIST;
	}

	/* add ouyang v20101108*/
	pstTarget = mon_get_target(pstEnv, pstMon, &pstAIBeAtk->stAtker);
	if (NULL != pstTarget)
	{
		if ( fHelpDist ||
			(pstMonDef->CallHelpHP == 0 && pstMon->iHP == 0) ||
			((pstMon->iHP < pstMonDef->CallHelpHP) &&
				((pstMon->iHP+pstAIBeAtk->iHurt) >= pstMonDef->CallHelpHP)) )
		{
			/* �������� */
			mon_help(pstEnv, pstMon, pstMonDef, pstTarget,pstAIBeAtk, fHelpDist);
		}
		
		if (pstMon->iHP < pstMonDef->RunAwayHP)
		{
			/* �������� */
			if( pstMonDef->RunAwayTime > 0 && 
				pstMon->tRunAwayStart+(int)pstMonDef->RunAwayTime < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				pstMon->tRunAwayStart = pstEnv->pstAppCtx->stCurr.tv_sec;
				pstMon->iRunAwayTime =   pstMonDef->RunAwayTime;
				mon_ai_run_away(pstEnv,pstMon, pstTarget);
			}
		}
		else if (pstMonDef->HoldDistanceRandom)
		{
			iRand = (int)(RAND1(10000));
			if (iRand < pstMonDef->HoldDistanceRandom)
			{
				int iMoveSpd = pstMonDef->FightMoveSpd;
				int iDist = z_distance(&pstMon->stCurrPos, &pstTarget->stRoleData.Pos);

				if (iMoveSpd < (int)(pstMon->unMinAtkSkillDist - iDist))
				{
					pstMon->tRunAwayStart = pstEnv->pstAppCtx->stCurr.tv_sec;
	                     	pstMon->iRunAwayTime = (pstMon->unMinAtkSkillDist - iDist)/iMoveSpd;
					mon_ai_run_away(pstEnv,pstMon, pstTarget);
				}
			}
		}
	}
	else
	{
		if (pstMon->iHP < pstMonDef->RunAwayHP)
		{
			/* �������� */
			if( pstMonDef->RunAwayTime > 0 && 
				pstMon->tRunAwayStart+(int)pstMonDef->RunAwayTime < pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				pstMon->tRunAwayStart = pstEnv->pstAppCtx->stCurr.tv_sec;
				pstMon->iRunAwayTime =   pstMonDef->RunAwayTime;
				z_mon_run_away(pstEnv, pstMon, NULL, 0, AI_STAT_RUNAWAY);
			}
		}
	}

	//�����Ѫ����
	pstMonSpeech = z_find_monsterspeech_def(pstEnv,  pstMon->iDefIdx, NULL);
	if (pstMonSpeech)
	{
		int iActIdx;
		
		iActIdx = z_find_actspeech_by_conditiontype( pstMonSpeech, SPEECH_MON_HP, pstMon->iHP*100.0/pstMon->iMaxHP);
		if (	iActIdx > -1 &&
			(pstMon->iHP+pstAIBeAtk->iHurt) >= (pstMonSpeech->MonActSpeech[iActIdx].ConditionVal/100.0)*pstMon->iMaxHP && 
			pstMon->iHP < (pstMonSpeech->MonActSpeech[iActIdx].ConditionVal/100.0)*pstMon->iMaxHP)
		{
			if(pstTarget)
			{
				z_mon_speech(pstEnv, pstMon, pstTarget, pstMonSpeech->MonActSpeech[iActIdx].BroadcastType, SPEECH_MON_HP, iActIdx);
			}
		}
	}

	if (ATK_TYPE_NO == pstMon->bAtkType || MONSTER_ALIVE != pstMon->cStat)
	{
		return -1;
	}

	//�ȸ���ѹ����
	if (pstMonDef->PressHurtRate > 0)
	{
		iPress = (pstAIBeAtk->iHurt/pstMonDef->PressHurtRate)*(1+pstAIBeAtk->iSkillPressRate/100.0);
	}
	else
	{
		iPress = (pstAIBeAtk->iHurt)*(1+pstAIBeAtk->iSkillPressRate/100.0);
	}
	
	if (0 >= iPress)
	{
		return 0;
	}


	/*
	// ����������Ĵ���,һ�˰���,ȫ�ҽ�ѹ����
 	if(pstMon->iLinkedNext >= 0 && pstMon->iLinkedPrev >= 0)
 	{
		Monster *pstLinkedMon = NULL;

		for(pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstMon->iLinkedNext);
		    pstLinkedMon != pstMon;
		    pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstLinkedMon->iLinkedNext))
		{
			if(!pstLinkedMon)
			{
				break;
			}

			if(pstLinkedMon->bAiStat != AI_STAT_DEAD)
			{
				mon_ai_addpress(pstEnv, pstLinkedMon, &pstAIBeAtk->stAtker, 1);
			}
		}
 	}
	*/
	
	mon_ai_addpress(pstEnv, pstMon, &pstAIBeAtk->stAtker, iPress);
	return 0;
}

int mon_ai_stat(ZONESVRENV* pstEnv, Monster *pstMon, AIEvt *pstAIEvt)
{
	if(is_mon_factory(pstMon))
	{
		return -1;
	}
	
	if (AI_EVT_SYSOUT != pstAIEvt->iEvtID && AI_STAT_HOME == pstMon->bAiStat)
	{
		return -1;
	}
	
	switch(pstAIEvt->iEvtID)
	{
	case AI_EVT_SYSOUT:
		mon_ai_evt_sysout(pstEnv, pstMon);
		break;

	case AI_EVT_INPRESS:
		mon_ai_evt_inpress(pstEnv, pstMon, &pstAIEvt->stEvtDetail.stInPress);
		break;

	case AI_EVT_BEDEFY:
		mon_ai_evt_bedefy(pstEnv, pstMon, &pstAIEvt->stEvtDetail.stBeDefy);
		break;

	case AI_EVT_OUTPRESS:
		mon_ai_evt_outpress(pstEnv, pstMon);
		break;

	case AI_EVT_BEATK:
		mov_ai_evt_beatk(pstEnv, pstMon, &pstAIEvt->stEvtDetail.stBeAtk);
		break;

	case AI_EVT_MARK:
		mon_ai_evt_mark(pstEnv, pstMon, &pstAIEvt->stEvtDetail.stMark);
		break;

	case AI_EVT_UNMARK:
		mon_ai_evt_unmark(pstEnv, pstMon, &pstAIEvt->stEvtDetail.stMark);
		break;
	case AI_EVT_COPY_PRESS:
		mon_ai_evt_copy_press(pstEnv, pstMon, &pstAIEvt->stEvtDetail.stCopyPress);
		break;
	default:
		return -1;
		break;
	}
		
	return 0;
}

int mon_ai_restore_hp(ZONESVRENV* pstEnv, Monster *pstMon)
{

	if(is_no_restore_hp_mon(pstEnv, pstMon))
	{
		return 0;
	}
	
	pstMon->iHP = pstMon->iMaxHP;
	if (pstMon->cMonMiscFlag & MON_MISC_FLAG_RESTORE_HP)
	{
		//ִ�нű�AI_HOME�����λ���¼�
		if (pstMon->cMonScriptFlag & MON_SCRIPT_PROC_AI_RESTORE_HP_MAP)
		{
			char szName[128];				

			stSEnv.pstMonster = pstMon;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstPlayer = NULL;
			stSEnv.pstKiller = NULL;
			stSEnv.pstMapinst = NULL;
			
			snprintf(szName, sizeof(szName), "monster%d.mac", pstMon->iDefIdx);
			szName[sizeof(szName) - 1] = 0;
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName, &pstMon->stScriptIdx, MON_SCRIPT_PROC_AI_RESTORE_HP);
		}

		//����ս������
		z_mon_speech_begin_or_end_fight(pstEnv,pstMon,SPEECH_MON_END_FIGHT);
				
		pstMon->cMonMiscFlag &= ~MON_MISC_FLAG_RESTORE_HP;
	}
	

	return 0;
}

int mon_ai_home_press_clear(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i = 0;
	Player *pstPlayer;

	pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv,pstMon->stAtkTargets.iTargetCur);

	i = pstMon->stAtkTargets.PressNum;
	while(i--)
	{
		Player *pstIter = z_id_player(pstEnv->pstAppCtx, pstEnv,
		                      pstMon->stAtkTargets.astPressEntrys[i].iTargetID);
		if (!pstIter)
		{
			continue;
		}
		player_delete_pressed_mon(pstEnv, pstIter, pstMon->iID);
	}
	
	memset(&pstMon->stAtkTargets, 0, sizeof(pstMon->stAtkTargets));
	mon_target_action(pstEnv, pstMon);
	
	if (pstPlayer && (pstPlayer->stOnline.State & CS_STAT_FIGHT))
	{
		ZoneAni stAni;
		
		stAni.iType = OBJ_PLAYER;
		stAni.stObj.pstPlayer = pstPlayer;
		z_player_fight_update(pstEnv, pstMon, &stAni);
	}

	return 0;
}

int mon_link_ai_home(ZONESVRENV* pstEnv, Monster *pstMon)
{
	// ����������Ĵ���
 	if(pstMon->iLinkedNext >= 0 && pstMon->iLinkedPrev >= 0)
 	{
		Monster *pstLinkedMon = NULL;

		for(pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstMon->iLinkedNext);
		    pstLinkedMon != pstMon;
		    pstLinkedMon = tmempool_get(pstEnv->pstMonsterPool,
		                                pstLinkedMon->iLinkedNext))
		{
			if(!pstLinkedMon)
			{
				break;
			}
			if(pstLinkedMon->bAiStat == AI_STAT_HOME)
			{
				continue;
			}

			if(pstLinkedMon->cStat == MONSTER_DIE)
			{
				int fSaved = pstLinkedMon->cRefresh;
				pstLinkedMon->cRefresh = 1;
				z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstLinkedMon,NULL);
				pstLinkedMon->cRefresh = fSaved;
			}
			else
			{
				mon_ai_home(pstEnv, pstLinkedMon);
			}
		}
 	}

	return 0;
}

//�˺�����ָ�AI״̬��������MON_AI_HOME�е���
int mon_ai_home_process(ZONESVRENV* pstEnv, Monster *pstMon)
{
	//int iOldHP = pstMon->iHP;

	//�����ƶ����¸�TICK ���˳�AI_HOME
	if (MOVE_TYPE_NO == pstMon->bMoveType)
	{
		pstMon->bAiStat = AI_STAT_IDLE;
	}
	//
	else
	{
		if(pstMon->stCurrPos.X == pstMon->stInitPos.X &&
			pstMon->stCurrPos.Y == pstMon->stInitPos.Y)
		{
			pstMon->bAiStat = AI_STAT_MOVE;
			
			//�ٶȸ��£��˳�AI_HOME ҲҪ����
			z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 1);
		}
		else
		{
			ZONEMAPINST *pstMapInst;
			TMAPMASK *pstMask;
			MapIndex *pstMapIdx;

			pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
			if (NULL == pstMapInst)
			{
				return -1;
			}
			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
			if (NULL == pstMapIdx)
			{
				return -1;
			}

			pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask +
			                                 pstMapIdx->iMaskOrigin);

			if (pstMon->cType == OBJ_NPC)
			{
				if (!IsPixelLineThrough(pstMask, pstMon->stCurrPos.X,
			                          pstMon->stCurrPos.Y,
			                          pstMon->stInitPos.X, pstMon->stInitPos.Y,
			                          maskStop))
				{
					pstMon->bAiStat = AI_STAT_MOVE;
					z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 1);

					z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
					z_safe_monster_entermap(pstEnv->pstAppCtx, pstEnv, pstMon, pstMon->stMap.iID, &pstMon->stInitPos);
					return 0;
				}
			}
			
		/*	if (!IsPixelLineThrough(pstMask, pstMon->stCurrPos.X,
			                          pstMon->stCurrPos.Y,
			                          pstMon->stInitPos.X, pstMon->stInitPos.Y,
			                          maskStop))
			{
				pstMon->bAiStat = AI_STAT_MOVE;
				z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 1);

				z_mon_chgpos(pstEnv, pstMon, &pstMon->stInitPos);
			}
			else*/
			{
				z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
				pstMon->stNextPos = pstMon->stInitPos;
			
				pstMon->cMoving = 1;
				z_monster_move_notify(pstEnv->pstAppCtx, pstEnv, pstMon, CS_MOVE_WALK);
				TV_CLONE(pstMon->stLastMoveTv, pstEnv->pstAppCtx->stCurr);
			}
		}
	}

	/*
	if(pstMon->iHP != pstMon->iMaxHP)
	{	
		mon_ai_restore_hp(pstEnv,pstMon);
		if (pstMon->iHP != iOldHP)
		{
			mon_hp_action(pstEnv, pstMon, iOldHP, 0);
		}
	}*/

	return 0;
}

int mon_ai_home(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int iOldHP = pstMon->iHP;

	if(pstMon->bAiStat == AI_STAT_HOME)
	{
		return 0;
	}
	
	if (0 != pstMon->stMonAtkInfo.cAtkStat)
	{
		z_mon_skill_break(pstEnv, pstMon);
	}

	//����ѹ����
	mon_ai_home_press_clear(pstEnv, pstMon);
	
	pstMon->bTraceTry = 0;
	pstMon->bPatrolTry = 0;
	
	pstMon->iFirstAtkerID = 0;
	pstMon->cFirstAtkerType = 0;
	pstMon->ullFirstAtkerWid = 0;
	mon_booty_first_action(pstEnv, pstMon);

	if(is_mon_bub(pstMon))
	{
		return mon_bub_ai_home(pstEnv, pstMon);
	}

	if(GET_PWORLDID(pstMon->stMap.iID) == BCT_PWORLD_ID)
	{
		return 0;
	}
	
	pstMon->cMoving = 0;
	pstMon->bAiStat = AI_STAT_HOME;
	
	mon_ai_restore_hp(pstEnv,pstMon);
	if (pstMon->iHP != iOldHP)
	{
		mon_hp_action(pstEnv, pstMon, iOldHP, 0);
	}

	//AI_HOME Ҳ�൱���˳�ս��״̬
	mon_out_fight_status_clear(pstEnv->pstAppCtx, pstEnv, pstMon);
	
	//�Ѽ��ܵ����ݳ�ʼ����
	mon_refresh_skill(pstEnv->pstAppCtx, pstEnv, pstMon);

	tlog_debug(pstEnv->pstLogCat, 0, 0, "mon id %d index %d home", pstMon->iID, pstMon->iDefIdx);

	// ����������Ĵ���
	mon_link_ai_home(pstEnv, pstMon);
	
	return 0;
}

int mon_ai_run_away(ZONESVRENV* pstEnv,  Monster *pstMon, Player *pstTarget)
{
	//�ƶ���֪ͨ�ͻ��� ����������ƶ�
       z_mon_run_away(pstEnv,pstMon,&pstTarget->stRoleData.Pos,0,AI_STAT_RUNAWAY);

	return 0;
}
int mon_ai_defy_end(ZONESVRENV* pstEnv, Monster *pstMon)
{
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	pstTargetList->iDefyID = 0;
	pstTargetList->iTargetCur = 0;
	mon_ai_evt_outpress(pstEnv, pstMon);

	return 0;
}

int mon_ai_mark_end(ZONESVRENV* pstEnv, Monster *pstMon)
{
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	pstTargetList->iMarkedID = 0;
	pstTargetList->iTargetCur = 0;
	mon_ai_evt_outpress(pstEnv, pstMon);
	return 0;
}



int mon_tick_restore_hp(ZONESVRENV* pstEnv, Monster *pstMon)
{
	if(pstMon->bAtkType == ATK_TYPE_NO && pstMon->iVPlayer != 0)
	{
		return 0;
	}

	if(pstMon->stAtkTargets.PressNum != 0 ||  
		pstEnv->pstAppCtx->stCurr.tv_sec - pstMon->stMonAtkInfo.stLastAtk.tv_sec <= FILL_HP_TIME)
	{
		return 0;
	}
	
	//if (is_convoy_car(pstMon) || is_friend_mon(pstMon))
	if(is_convoy_car(pstMon) || is_mon_bub(pstMon))
	{
		return 0;
	}

	mon_ai_restore_hp(pstEnv,pstMon);
	
	return 0;
}


int get_mon_atk_skill_dist(ZONESVRENV* pstEnv, Monster *pstMon)
{
	SKILLDEF *pstSkillDef;
	MONSTERDEF *pstMonDef = z_get_mon_def(pstEnv, pstMon);
	int iSkillDist = pstMon->unMinAtkSkillDist;

	if(pstMonDef == NULL)
	{
		return iSkillDist;
	}

	//�������ܣ�����һ����Χ���߳�����Ĺ�������Ҫ�ߵ��ܽ��ĵط���
	pstSkillDef = get_mon_nextskill_def(pstEnv, pstMon);
	if(pstSkillDef)
	{
		iSkillDist = z_mon_skill_atk_dist(pstSkillDef) + pstMonDef->Radius;
	}

	return iSkillDist;
}

int mon_atk_distance_valid(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int iSkillDist = 0;
	int iIndex = 0;
	int iAtkeeRadius;
	char cType = 0;
	RESPOS stPos;
	
	iSkillDist =get_mon_atk_skill_dist(pstEnv, pstMon);
		
	iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstMon->stAtkTargets.iTargetCur, &cType);
	if (0 > iIndex)
	{
		return 0;
	}

	if (cType == OBJ_PLAYER)
	{
		Player *pstTarget;
		
		pstTarget = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->stAtkTargets.iTargetCur);
		if(pstTarget == NULL  ||(pstTarget->stOnline.State & CS_STAT_DEAD) ||pstTarget->eStatus != PLAYER_STATUS_ROLE_LOGIN)
		{
			mon_press_delete(pstMon, pstMon->stAtkTargets.iTargetCur);
			mon_ai_evt_outpress(pstEnv, pstMon);
			return 0;
		}

		if (pstTarget->stOnline.cMoving)
		{
			z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstTarget, 0);
			if (pstMon->cMoving)
			{
				iSkillDist = pstMon->unMinAtkSkillDist*0.4;
			}
		}

		stPos = pstTarget->stRoleData.Pos;
		iAtkeeRadius = 0;
	}
	else if(cType == OBJ_MONSTER)
	{
		Monster *pstTarget;
		
		pstTarget = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstMon->stAtkTargets.iTargetCur);
		if(pstTarget == NULL)
		{
			mon_press_delete(pstMon, pstMon->stAtkTargets.iTargetCur);
			mon_ai_evt_outpress(pstEnv, pstMon);
			return 0;
		}

		if (pstTarget->cMoving)
		{
			z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
		}

		stPos = pstTarget->stCurrPos;
		iAtkeeRadius = pstTarget->unRadius;
	}
	else if (cType == OBJ_NPC)
	{
		Npc *pstTarget;
		pstTarget = z_id_npc(pstEnv->pstAppCtx, pstEnv, pstMon->stAtkTargets.iTargetCur);
		if(pstTarget == NULL)
		{
			mon_press_delete(pstMon, pstMon->stAtkTargets.iTargetCur);
			mon_ai_evt_outpress(pstEnv, pstMon);
			return 0;
		}
		if (pstTarget->stMon.cMoving)
		{
			z_monster_move_curr(pstEnv->pstAppCtx, pstEnv, pstMon, 0);
		}

		stPos = pstTarget->stMon.stCurrPos;
		iAtkeeRadius = pstTarget->stMon.unRadius;
	}
	else
	{
		return 0;
	}

	//�򲻵�ƴ��׷
	if (z_distance(&pstMon->stCurrPos, &stPos) > iSkillDist + iAtkeeRadius)
	{	
		return -1;
	}

	return 0;
}

int mon_ai_tick(ZONESVRENV* pstEnv, Monster *pstMon)
{
	struct timeval stDiff;
	long long  llMs;
	MonAtkInfo *pstMonAtkInfo;
	SKILLDEF *pstSkillDef;
	int iTarget;
	TMEMBLOCK *pstMemBlock;
	Player *pstTarget;
	MONSTERDEF *pstMonDef;	
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;

	llMs = 0;
	iTarget = 0;
	pstMonAtkInfo = NULL;
	pstMemBlock = NULL;
	pstTarget = NULL;

	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef )
	{
		return -1;
	}
	
	if(pstEnv->pstAppCtx->stCurr.tv_sec <= pstMon->tFreezeEnd ||
	   pstEnv->pstAppCtx->stCurr.tv_sec < pstMon->tBirthEnd)
	{
		return 0;
	}

	if(pstMonDef->MonsterID == 200035)
	{
		llMs = 0;
	}
	
	if(pstMon->bMoveType == MOVE_TYPE_FORCED)
	{
		if(pstMon->stNextPos.X != pstMon->stForcedPos.X ||
		   pstMon->stNextPos.Y != pstMon->stForcedPos.Y)
		{
			pstMon->stNextPos = pstMon->stForcedPos;
		}
	}
	else if(pstMon->unTraceDist > 0 &&  
		mon_have_movespd(pstMon))
	{
		RESPOS stPos = pstMon->stInitPos;
		
		if(is_mon_bub(pstMon))
		{
			Player *pstPlayer = mon_bub_get_owner(pstEnv, pstMon);
			if(pstPlayer)
			{
				stPos = pstPlayer->stRoleData.Pos;
			}

			//�����λ
			if( z_distance(&pstMon->stCurrPos, &stPos) > pstMon->unTraceDist)
			{
				mon_ai_home(pstEnv, pstMon);
				return 0;
			}
		}
		else if( z_distance(&pstMon->stCurrPos, &stPos) > pstMon->unTraceDist)
		{
			mon_press_update_out(pstEnv, pstMon);

			if ((0 == pstTargetList->PressNum &&
			    0 >= pstTargetList->iDefyID &&
			    0 >= pstTargetList->iMarkedID))
			{
				mon_ai_home(pstEnv, pstMon);
				return 0;
			}
		}
	}

	if(	pstMon->cAtkInsert )
	{
		if( ATK_TYPE_INITIATIVE == pstMonDef->AtkType &&
			MOVE_TYPE_NO != pstMonDef->MoveType )
		{
			if(pstTargetList->PressNum > 0 && 
			   mon_ai_insert_atk(pstEnv, pstMon) == 0)
			{
				return 0;
			}
		}
		else 
		{
			if ( pstMon->iVPlayer > 0 && 
				 mon_ai_insert_atk(pstEnv, pstMon) == 0)
			{
				return 0;
			}
		}
	}

	switch(pstMon->bAiStat)
	{
	case AI_STAT_ATK:
		pstMonAtkInfo = &pstMon->stMonAtkInfo;
		iTarget = pstMon->stAtkTargets.iTargetCur;

		//��ǵ�ʱ�������
		if (pstMon->stAtkTargets.iMarkedID > 0 &&
		    pstEnv->pstAppCtx->stCurr.tv_sec > pstMon->stAtkTargets.tMarkedEnd)
		{
			return mon_ai_mark_end(pstEnv, pstMon);
		}

		//����ʱ�������
		if (pstMon->stAtkTargets.iDefyID > 0 &&
		    pstEnv->pstAppCtx->stCurr.tv_sec > pstMon->stAtkTargets.tDefyEnd)
		{
			return mon_ai_defy_end(pstEnv, pstMon);
		}

		//����л�Ŀ��
		if ((pstMon->bRandSelect > 0 && pstEnv->pstAppCtx->stCurr.tv_sec - pstMon->stAtkTargets.tTargetSelect > pstMon->bRandSelect))
		{
			return mon_ai_rand_select_target(pstEnv, pstMon);
		}

		if (0 == pstMonAtkInfo->cAtkStat)
		{
			mon_ai_atk_continue(pstEnv, pstMon);
			return 0;
		}

		pstSkillDef = z_find_skill(pstEnv, pstMonAtkInfo->unSkillID, pstMonAtkInfo->bSkillLevel, &pstMonAtkInfo->iSkillIdx);
		if (NULL == pstSkillDef)
		{
			pstMonAtkInfo->cAtkStat = 0;
			return -1;
		}

		if (CS_STAT_ATK == pstMonAtkInfo->cAtkStat  || CS_STAT_MOVE_ATK == pstMonAtkInfo->cAtkStat)
		{
			mon_prepare_end(pstEnv, pstMon, pstSkillDef);

			//�ƶ�����������û��ǿ���ƶ���ʱ��׷�С�
			if(CS_STAT_MOVE_ATK == pstMonAtkInfo->cAtkStat &&pstMon->bMoveType != MOVE_TYPE_FORCED)
			{
				if(0 > mon_atk_distance_valid(pstEnv, pstMon))
				{
					mon_ai_trace(pstEnv, pstMon, 0);
				}
			}
		}
		else if (CS_STAT_PREPARE == pstMonAtkInfo->cAtkStat)
		{
			TV_DIFF(stDiff, pstMonAtkInfo->stAtkStatEnd, pstEnv->pstAppCtx->stCurr);
			TV_TO_MS(llMs, stDiff);

			if (0 >= llMs)
			{
				mon_prepare_end(pstEnv, pstMon, pstSkillDef);
			}
		}
		break;

	case AI_STAT_TRACE:
		//����ʱ����
		if (pstMon->stAtkTargets.iDefyID > 0 && pstEnv->pstAppCtx->stCurr.tv_sec > pstMon->stAtkTargets.tDefyEnd)
		{
			return mon_ai_defy_end(pstEnv, pstMon);
		}

		if (0 == pstTargetList->PressNum &&
		    0 >= pstTargetList->iDefyID &&
		    0 >= pstTargetList->iMarkedID)
		{
			if (MOVE_TYPE_NO == pstMon->bMoveType)
			{
				pstMon->bAiStat = AI_STAT_IDLE;
				return 0;
			}
			else
			{
				mon_ai_move(pstEnv, pstMon);
				return 0;
			}
		}

		//�򲻵�ƴ��׷
		if(0 > mon_atk_distance_valid(pstEnv, pstMon))
		{
			mon_ai_trace(pstEnv, pstMon, 0);
		}
		else
		{
			if (0 > mon_ai_atk(pstEnv, pstMon,0))
			{
				mon_ai_trace(pstEnv, pstMon, 0);
			}
		}
		break;

	case AI_STAT_MOVE:
		//ִ��Ѳ���յ�ű����ڼ����ߡ�
		if (mon_patrol_script(pstEnv, pstMon))
		{
			break;
		}

		if(pstMon->cType == OBJ_MONSTER)
		{	
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, pstMon->iMIndex);
			if (!pstMemBlock->fValid)
			{
				break;
			}
		}
		else if(pstMon->cType == OBJ_NPC)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, pstMon->iMIndex);
			if (!pstMemBlock->fValid)
			{
				break;
			}
		}

		mon_tick_restore_hp(pstEnv, pstMon);

		//С�ܷ�Ӧ������
		if(is_mon_bub(pstMon))
		{
			Player *pstPlayer = mon_bub_get_owner(pstEnv, pstMon);
			if(NULL == pstPlayer)
			{
				return 0;
			}

			if(z_distance(&pstMon->stCurrPos, &pstPlayer->stRoleData.Pos) > 400)
			{
				mon_ai_move(pstEnv, pstMon);
			}
		}
		else if(0 == pstMon->cMoving)
		{
			if(	is_convoy_car(pstMon) || 
				pstMon->bSubType == MON_SUB_TYPE_BUILD || 
				is_friend_mon(pstMon) ||
				(pstMonDef->MiscType&MON_MISC_TYPE_NO_STOP) )
			{
				mon_ai_move(pstEnv, pstMon);
			}
			else if(pstMon->cType == OBJ_NPC)
			{
				mon_ai_move(pstEnv, pstMon);
			}
			else if(pstEnv->pstAppCtx->stCurr.tv_sec - pstMon->stLastMoveTv.tv_sec >= MOVE_INTERVAL_MIN)
			{
				mon_ai_move(pstEnv, pstMon);
			}
		}

		if( pstMon->bSubType == MON_SUB_TYPE_XUNLUO &&
			pstMon->iCommType == MON_UNION_TYPE_XUNLUO_MON &&
			pstMon->stCommUnion.stXunLuoMon.a != 0 &&
			pstMon->stForcedPos.X >0 &&
			pstMon->stForcedPos.Y >0)
		{
			z_mon_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstMon, 1);
		}

		if (pstMon->stAtkTargets.PressNum > 0) 
		{
			if (0 > mon_ai_atk(pstEnv, pstMon,0))
			{
				mon_ai_trace(pstEnv, pstMon, 0);
			}
		}
		break;
		
	case AI_STAT_IDLE:
		if (ATK_TYPE_NO == pstMon->bAtkType)
		{
			break;
		}

		if (pstMon->stAtkTargets.PressNum > 0)
		{
			int iTarget = mon_ai_select_target(pstEnv, pstMon);
			if (0 > iTarget)
			{
				return -1;
			}
			// jerry add
			else
			{
				mon_press_delete(pstMon, iTarget);
				mon_ai_evt_outpress(pstEnv, pstMon);
				//pstMon->bAiStat = AI_STAT_ATK;
			}
			// jerry add end
			
		}
		else if (pstEnv->pstAppCtx->stCurr.tv_sec - pstMon->stMonAtkInfo.stLastAtk.tv_sec > FILL_HP_TIME)
		{
			mon_ai_restore_hp(pstEnv,pstMon);
		}
		break;
	case AI_STAT_RUNAWAY:
		/* ���ܵ�״̬���� add by ouyang v2101108 */

		/* ����:����ʱ�䵽ʱ */
		 if(pstMon->iRunAwayTime + pstMon->tRunAwayStart < pstEnv->pstAppCtx->stCurr.tv_sec )
		 {
		 	/* �������пɹ����Ķ��� */
		 	if (pstMon->stAtkTargets.PressNum > 0) 
			{
			        /* ����״̬--->����״̬ ����Ϊ�򲻵�*/
				if (0 > mon_ai_atk(pstEnv, pstMon,0))
				{
				       /* ����״̬---> ׷��״̬  */
					mon_ai_trace(pstEnv, pstMon, 0);
				}
			}
			 else
			 {
			 	/*����״̬--->����߶� */
				mon_ai_move(pstEnv, pstMon);
			 }
			 break;
		 }
		/* ÿ�����һ�� */			
		//if(pstMon->tLastTrace != pstEnv->pstAppCtx->stCurr.tv_sec )
		{
			pstTarget = mon_get_target(pstEnv, pstMon, NULL);
			if (pstTarget)
			{
		       	/* �������ܵ�ʱ��״̬�� */
				mon_ai_run_away(pstEnv, pstMon, pstTarget);
			}
			else 
			{
				z_mon_run_away(pstEnv, pstMon, NULL, 0, AI_STAT_RUNAWAY);
				
				/* ��������� û��Ŀ���� ����߶�״̬ */
				//mon_ai_move(pstEnv, pstMon);
			}
		}
		break;
		
	default:
		break;
	}
	
	return 0;
}

int mon_ai_atk_anybody(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int iAtkeeRadius = 0;
	ZoneAni stAni;
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;
	ZoneObjList stObjList;
	SKILLDEF *pstSkillDef = z_find_skill(pstEnv, pstMonAtkInfo->unSkillID,
	                                     pstMonAtkInfo->bSkillLevel, NULL);
	MONSTERDEF *pstMonDef = z_get_mon_def(pstEnv, pstMon);

	if(!pstSkillDef)
	{
		pstMonAtkInfo->cAtkStat = 0;
		return -1;
	}

	if (!pstMonDef)
	{
		pstMonAtkInfo->cAtkStat = 0;
		return -1;
	}

	iAtkeeRadius = pstMon->unRadius;

	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv,  pstMon, 0);
	}

	stAni.iType = OBJ_MONSTER;
	stAni.stObj.pstMon = pstMon;
	pstMonAtkInfo->stAtkPos = pstMon->stCurrPos;
	z_mon_get_atk_list(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef,
	                   pstSkillDef, &stAni, &pstMonAtkInfo->stAtkPos,
	                   &stObjList, 0);

	//�����������һ��
	if(pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE ||
	   pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE)
	{
		if (stObjList.iObj <= 0)
		{
			pstMonAtkInfo->cAtkStat = 0;
			return -1;
		}
			
		stAni = stObjList.astObj[(int)RAND1(stObjList.iObj)];
		if(z_mon_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstMon, &stAni,
		                       pstSkillDef,NULL) < 0)
		{
			pstMonAtkInfo->cAtkStat = 0;
			return -1;
		}
		
		stObjList.iObj = 1;
		stObjList.astObj[0] = stAni;
	}
	else
	{
		if(!stObjList.iObj && !pstSkillDef->PrepareTime)
		{
			pstMonAtkInfo->cAtkStat = 0;
			return -1;
		}
	}

	if(pstSkillDef->AttackActTime > 0 &&
	   (pstMon->tFreezeEnd <
	   pstEnv->pstAppCtx->stCurr.tv_sec + pstSkillDef->AttackActTime / 10))
	{
		pstMon->tFreezeEnd = pstEnv->pstAppCtx->stCurr.tv_sec +
		                     pstSkillDef->AttackActTime / 10;
	}

	pstMon->iSkillAtkerID = pstMon->iID;
	mon_atk_end(pstEnv, pstMon, pstSkillDef, &stObjList);	
	pstMon->iSkillAtkerID = 0;
	return 0;
}

int mon_ai_refreshed_skill(ZONESVRENV* pstEnv, Monster *pstMon,
                           AIREFRESHEDSKILL *pstProfile)
{
	MonAtkInfo *pstMonAtkInfo = &pstMon->stMonAtkInfo;

	if(!pstProfile->SkillID)
	{
		return 0;
	}

	if(pstProfile->Rate * 100 <= RAND1(10000))
	{
		return 0;
	}

	// ���Լ�ʹ�ü���
	if(pstProfile->Self)
	{
		return mon_svr_atk(pstEnv, pstMon, pstProfile->SkillID,
		                   pstProfile->SkillLevel, pstMon->iID);
	}

	// �Ե���ʹ��
	pstMonAtkInfo->unSkillID = pstProfile->SkillID;
	pstMonAtkInfo->bSkillLevel = pstProfile->SkillLevel;
	pstMonAtkInfo->iSkillIdx = 0;
	pstMonAtkInfo->bSkillType = MON_SKILL_TYPE_ERROR;
	pstMonAtkInfo->bMonSkillIdx = 0;

	// ѡ��һ��ָ����Ŀ����
	pstMon->stMonAtkInfo.fIsSpecificTarget = 
		!!mon_will_atk_specific_target(pstEnv, pstMon);
	if(pstMon->stMonAtkInfo.fIsSpecificTarget)
	{
		return mon_ai_start_atk(pstEnv, pstMon,NULL);
	}

	// �Ҳ���Ŀ����,������Ұ����һ������
	return mon_ai_atk_anybody(pstEnv, pstMon);
}

static int player_delete_pressed_mon(ZONESVRENV* pstEnv, Player *pstPlayer,
                                     int iPressedID)
{
	int i = 0;

	UNUSED(pstEnv);

	// ͬʱ�ѹ����Player���б���ɾ��
	for(i = 0; i < pstPlayer->stOnline.iPressedMonNum; ++i)
	{
		if(pstPlayer->stOnline.aiPressedMon[i] == iPressedID)
		{
			break;
		}
	}
	if(i < pstPlayer->stOnline.iPressedMonNum)
	{
		--pstPlayer->stOnline.iPressedMonNum;
		if(i != pstPlayer->stOnline.iPressedMonNum)
		{
			pstPlayer->stOnline.aiPressedMon[i] =
				pstPlayer->stOnline.aiPressedMon[
					pstPlayer->stOnline.iPressedMonNum];
		}
	}
	return 0;
}

int mon_ai_get_dynmskmon_distance(ZONESVRENV* pstEnv, Monster* pstDynMskMon, MONSTERDEF *pstDynMskMonDef, RESPOS *pstFrom, RESPOS *pstTo,RESPOS *pstInter)
{
	int i;
	RESPOS polygon[4];
	LINESEG l1;
	LINESEG l2;
	RESPOS inter;
	int iDistance = 0;
	int iDistanceTmp = 0;

	memset(polygon,0,sizeof(RESPOS)*4);

	//��һ������
	if(pstDynMskMonDef->DynMaskX > 0 )
	{
		polygon[0].X = pstDynMskMon->stCurrPos.X - (int)pstDynMskMonDef->DynMaskX;
	}
	else
	{
		polygon[0].X = pstDynMskMon->stCurrPos.X - (int)pstDynMskMonDef->Radius;
	}
	if (0 > polygon[0].X)
	{
		polygon[0].X = 0;
	}
	if(pstDynMskMonDef->DynMaskY > 0 )
	{
		polygon[0].Y = pstDynMskMon->stCurrPos.Y - (int)pstDynMskMonDef->DynMaskY;
	}
	else
	{
		polygon[0].Y= pstDynMskMon->stCurrPos.Y - (int)pstDynMskMonDef->Radius;
	}
	if (0 > polygon[0].Y )
	{
		polygon[0].Y = 0;
	}

	//�ڶ�������
	if(pstDynMskMonDef->DynMaskX > 0 )
	{
		polygon[1].X = pstDynMskMon->stCurrPos.X + (int)pstDynMskMonDef->DynMaskX;
	}
	else
	{
		polygon[1].X = pstDynMskMon->stCurrPos.X + (int)pstDynMskMonDef->Radius;
	}
	if (0 > polygon[1].X)
	{
		polygon[1].X = 0;
	}
	if(pstDynMskMonDef->DynMaskY > 0 )
	{
		polygon[1].Y = pstDynMskMon->stCurrPos.Y - (int)pstDynMskMonDef->DynMaskY;
	}
	else
	{
		polygon[1].Y= pstDynMskMon->stCurrPos.Y - (int)pstDynMskMonDef->Radius;
	}
	if (0 > polygon[1].Y )
	{
		polygon[1].Y = 0;
	}
	
	//����������
	if(pstDynMskMonDef->DynMaskX > 0 )
	{
		polygon[2].X = pstDynMskMon->stCurrPos.X + (int)pstDynMskMonDef->DynMaskX;
	}
	else
	{
		polygon[2].X = pstDynMskMon->stCurrPos.X + (int)pstDynMskMonDef->Radius;
	}
	if (0 > polygon[2].X)
	{
		polygon[2].X = 0;
	}
	if(pstDynMskMonDef->DynMaskY > 0 )
	{
		polygon[2].Y = pstDynMskMon->stCurrPos.Y + (int)pstDynMskMonDef->DynMaskY;
	}
	else
	{
		polygon[2].Y= pstDynMskMon->stCurrPos.Y + (int)pstDynMskMonDef->Radius;
	}
	if (0 > polygon[2].Y )
	{
		polygon[2].Y = 0;
	}

	//���ĸ�����
	if(pstDynMskMonDef->DynMaskX > 0 )
	{
		polygon[3].X = pstDynMskMon->stCurrPos.X - (int)pstDynMskMonDef->DynMaskX;
	}
	else
	{
		polygon[3].X = pstDynMskMon->stCurrPos.X - (int)pstDynMskMonDef->Radius;
	}
	if (0 > polygon[3].X)
	{
		polygon[3].X = 0;
	}
	if(pstDynMskMonDef->DynMaskY > 0 )
	{
		polygon[3].Y = pstDynMskMon->stCurrPos.Y + (int)pstDynMskMonDef->DynMaskY;
	}
	else
	{
		polygon[3].Y= pstDynMskMon->stCurrPos.Y + (int)pstDynMskMonDef->Radius;
	}
	if (0 > polygon[3].Y )
	{
		polygon[3].Y = 0;
	}

	//���ĸ�����ת��
	for(i=0;i<4;i++)
	{
		RESPOS ret_p;
		int iAngle;
		RESPOS p;
		
		iAngle = pstDynMskMon->bDirection;
		iAngle = -iAngle*2;
		p.X = polygon[i].X;
		p.Y = polygon[i].Y;
		
		ret_p = z_rotate_pos(pstDynMskMon->stCurrPos,iAngle,p);
		polygon[i].X = ret_p.X;
		if(polygon[i].X < 0)
		{
			polygon[i].X = 0;
		}
		polygon[i].Y = ret_p.Y;
		if(polygon[i].Y < 0)
		{
			polygon[i].Y = 0;
		}
	}

	//����ڶ���������
	if( //InsideConvexPolygon(4, polygon, *pstFrom) ||
		InsideConvexPolygon(4, polygon, *pstTo) )
	{
		if(pstInter)
		{
			*pstInter = *pstFrom;
		}
		iDistance = 1;
		return 1;
	}
	
	//��ȡ�ĸ��߶�,���Ƿ��н���
	for(i=0;i<4;i++)
	{
		l1.e.X = polygon[i].X;
		l1.e.Y = polygon[i].Y;
		if(i<3)
		{
			l1.s.X = polygon[i+1].X;
			l1.s.Y = polygon[i+1].Y;
		}
		else
		{
			l1.s.X = polygon[0].X;
			l1.s.Y = polygon[0].Y;
		}
		l2.e.X = pstFrom->X;
		l2.e.Y = pstFrom->Y;
		l2.s.X = pstTo->X;
		l2.s.Y = pstTo->Y;
		memset(&inter,0,sizeof(inter));
		if(intersection(l1,l2,&inter))
		{
			iDistanceTmp = z_distance(pstTo, &inter);
			if(iDistanceTmp > 1000)
			{
				//tlog_error(pstEnv->pstLogCat, 0, 0, "intersection l1:%f,%f,%f,%f,l2:%f,%f,%f,%f, inter:%d,%d", l1.e.X,l1.e.Y,l1.s.X,l1.s.Y, l2.e.X,l2.e.Y, l2.s.X,l2.s.Y,inter.X,inter.Y);
			}
			if(iDistanceTmp<=0 && inter.X >0 && inter.Y>0)
			{
				iDistanceTmp=1;
			}

		}
		if( iDistanceTmp > iDistance )
		{
			iDistance = iDistanceTmp;
			if(pstInter)
			{
				*pstInter = inter;
			}
		}
	}
		
	return iDistance;
	UNUSED(pstEnv);
}
//���ҹ��ﵽĿ����м�����Ķ�̬��Ϊ����Ŀ��
int mon_press_target_dynmsk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, Monster *pstMon,RESPOS *pstTracePos)
{
	int iTargetDynMskMonID = 0;
	ZoneAni stAni;
	RESPOS stInter;
	int interNum = 0;
	
	memset(&stInter,0,sizeof(stInter));
	memset(&stAni,0,sizeof(stAni));
	stAni.iType = OBJ_MONSTER;
	stAni.stObj.pstMon = pstMon;
	iTargetDynMskMonID = mon_press_target_dynmsk_pos(pstAppCtx, pstEnv, pstMapInst, 
									&stAni,&pstMon->stCurrPos, pstTracePos, &stInter,&interNum,0);

	if( iTargetDynMskMonID > 0)
	{
		pstMon->stAtkTargets.iTargetCur = iTargetDynMskMonID;
		mon_press_add_by_memid(pstEnv, pstMon, pstMon->stAtkTargets.iTargetCur,10000000 );
	}
	return 0;
}
 
//ͨ���߶��ཻ���㷨�ҵ�·��������Ķ�̬�赲�ֵ���ײ��
int mon_press_target_dynmsk_pos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, ZONEMAPINST* pstMapInst, 
									ZoneAni *pstAni,RESPOS *pstForm, RESPOS *pstTo, RESPOS *pstInter,
									int *pinsterCount,int iCheckPathFlag)
{
	int 	i = 0;
	int  	iIndex = 0;
	char 	cType = 0;
	int     iDelMemID = 0;
	int     iMemID = 0;
	TMEMBLOCK 	*pstMemBlock = NULL;
	MONSTERDEF  *pstMonDef = NULL;
	Monster 	*pstMonArea = NULL;
	int 		iDistance = 0;
	int 		iDistanceTmp = 0;
	int     	iTargetDynMskMonID =0;
	ZoneAni 	stAni;
	SKILLDEF 	*pstSkillDef;
	MapIndex 	*pstMapIdx;
	TMAPMASK    *pstMask;
	RESPOS stPosTmp;

	*pinsterCount = 0;
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	memset(pstInter,0,sizeof(RESPOS));
	
	for ( i=pstMapInst->iDynMskMonCount-1; i>=0; i--)
    {
    	iMemID = pstMapInst->aiDynMskMonMemID[i];
		iIndex = z_id_indextype(pstAppCtx, pstEnv, iMemID, &cType);
		if (iIndex < 0) 
		{
			iDelMemID = iMemID;
			continue;
		}

		if (cType == OBJ_MONSTER)
		{	
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
            if(!pstMemBlock->fValid)
            {
            	iDelMemID = iMemID;
            	continue;
            }
            pstMonArea = (Monster *)pstMemBlock->szData;	
			if ( !(pstMonArea->cMonMiscFlag & MON_MISC_FLAG_DYN) )
			{
				iDelMemID = iMemID;
				continue;
			}
			
			pstMonDef = z_get_mon_def(pstEnv,  pstMonArea);
			if(!pstMonDef)
			{
				iDelMemID = iMemID;
				continue;
			}
			
			stAni.iType = OBJ_MONSTER;
			stAni.stObj.pstMon = pstMonArea;

			if(!iCheckPathFlag && pstAni->iType == OBJ_MONSTER)
			{
				pstSkillDef = mon_find_one_skilldef(pstEnv, pstAni->stObj.pstMon);
				if (pstSkillDef && !z_mon_filter_relation(pstEnv->pstAppCtx, pstEnv, pstAni->stObj.pstMon, &stAni, pstSkillDef))
				{
					continue;
				}
			}
			else if(!iCheckPathFlag && pstAni->iType == OBJ_PLAYER)
			{
				pstSkillDef = z_get_player_normalskilldef(pstEnv,pstAni->stObj.pstPlayer);
				if (pstSkillDef && !z_player_filter_relation(pstAppCtx, pstEnv, pstAni->stObj.pstPlayer, &stAni, pstSkillDef))
				{
					continue;
				}
			}

			iDistanceTmp = mon_ai_get_dynmskmon_distance(pstEnv, pstMonArea, pstMonDef,pstForm,pstTo, &stPosTmp);
			if(iDistanceTmp > iDistance)
			{	
				*pstInter = stPosTmp;
				iDistance = iDistanceTmp;
				iTargetDynMskMonID = pstMonArea->iID;
			}
			if(iDistanceTmp > 0)
			{
				(*pinsterCount) += 1;
			}
			
		}
		else
		{
			iDelMemID = iMemID;
		}
	}
	
	if(iDelMemID)
	{
		//������쳣id,ÿ����̭һ��
		map_dynmsk_mon_memid_del(pstEnv, pstMapInst, iDelMemID);
	}
	
	if(iDistance == 0)
	{
		return -1;
	}
	
	return iTargetDynMskMonID;
}



int is_mon_bub(Monster *pstMon)
{
	if(pstMon->bSubType == MON_SUB_TYPE_TOTEM &&
		pstMon->bMoveType == MOVE_TYPE_FOLLOW_OWNER)
	{
		return 1;
	}

	return 0;
}

Player* mon_bub_get_owner(ZONESVRENV *pstEnv, Monster *pstMon)
{
	Player *pstPlayer;

	pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iOwnerID);
	if(NULL == pstPlayer)
	{
		return NULL;
	}

	return pstPlayer;
}

int mon_bub_get_mode(ZONESVRENV *pstEnv, Monster *pstMon)
{	
	Player *pstPlayer;

	pstPlayer = mon_bub_get_owner(pstEnv, pstMon);
	if(NULL == pstPlayer)
	{
		return 0;
	}

	return pstPlayer->stOnline.stBubOnline.iStat;
}

int mon_bub_near_owner(ZONESVRENV * pstEnv, Monster *pstMon, Player *pstPlayer)
{
	int iRet;
	ZONEIDX stIdx;
	ZONEMAPINST* pstMapInst = NULL;
	MapIndex *pstMapIdx;
	RESPOS stPos;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		goto _error;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		goto _error;
	}

	z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayer->stRoleData.Pos, &pstPlayer->stRoleData.Pos, 400, 0, &stPos);

	if (NULL == z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, &stPos))
	{
		goto _error;
	}

	z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);

	iRet = z_safe_monster_entermap(pstEnv->pstAppCtx, pstEnv, pstMon, pstPlayer->stRoleData.Map, &stPos);
	if(iRet < 0)
	{
		goto _error;
	}
	
	return 0;
	
_error:

	//�����Ƿ����£�active������������ġ�
	return -1;
}

int mon_bub_ai_home(ZONESVRENV * pstEnv, Monster *pstMon)
{
	Player *pstOwner;
	AtkTargetList *pstTargetList = &pstMon->stAtkTargets;
	
	memset(pstTargetList, 0, sizeof(*pstTargetList));

	pstMon->stMonAtkInfo.cAtkStat = 0;
	pstMon->stMonAtkInfo.unPilot = 0;
	pstMon->stMonAtkInfo.cPilotEnd = 0;
	pstMon->stMonAtkInfo.bNextSkillIdx = 0;
	pstMon->stMonAtkInfo.bHeadSkillIdx = 0;

	pstMon->bAiStat = AI_STAT_MOVE;

	pstOwner = mon_bub_get_owner(pstEnv, pstMon);
	if(NULL == pstOwner)
	{
		return -1;
	}
	
	if( z_distance(&pstMon->stCurrPos, &pstOwner->stRoleData.Pos) > pstMon->unTraceDist)
	{
		mon_bub_near_owner(pstEnv, pstMon, pstOwner);
	}

	return 0;
}

int mon_bub_del_owner_info(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	Player *pstPlayer;

	if(!is_mon_bub(pstMon))
	{
		return -1;
	}

	pstPlayer = mon_bub_get_owner(pstEnv, pstMon);
	if(NULL == pstPlayer)
	{
		return -1;
	}

	for(i = pstPlayer->stOnline.stBubOnline.iBubNum-1; i>=0; i--)
	{
		if(pstPlayer->stOnline.stBubOnline.iBubID[i] == pstMon->iID)
		{
			pstPlayer->stOnline.stBubOnline.iBubNum--;
			if (i != pstPlayer->stOnline.stBubOnline.iBubNum)
			{
				memmove(&pstPlayer->stOnline.stBubOnline.iBubID[i], &pstPlayer->stOnline.stBubOnline.iBubID[i+1], 
							(pstPlayer->stOnline.stBubOnline.iBubNum - i) * sizeof(pstPlayer->stOnline.stBubOnline.iBubID[0]));
			}
		}
	}

	return 0;
}

int mon_bub_get_idx(ZONESVRENV* pstEnv, Monster *pstMon)
{
	int i;
	Player *pstPlayer;

	if(!is_mon_bub(pstMon))
	{
		return -1;
	}

	pstPlayer = mon_bub_get_owner(pstEnv, pstMon);
	if(NULL == pstPlayer)
	{
		return -1;
	}

	for(i = pstPlayer->stOnline.stBubOnline.iBubNum-1; i>=0; i--)
	{
		if(pstPlayer->stOnline.stBubOnline.iBubID[i] == pstMon->iID)
		{
			return i;
		}
	}

	return -1;
}

int mon_bub_inpress_check(ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAni)
{
	Player *pstOwner;
	RESPOS *pstPos;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	TMAPMASK *pstMask;

	if(!is_mon_bub(pstMon))
	{
		return 0;
	}
	
	pstOwner = mon_bub_get_owner(pstEnv, pstMon);
	if(NULL == pstOwner)
	{
		return -1;
	}

	if(z_distance(&pstOwner->stRoleData.Pos, &pstMon->stCurrPos) > 500)
	{
		return -1;
	}

	if(pstAni->iType == OBJ_PLAYER)
	{
		pstPos = &pstAni->stObj.pstPlayer->stRoleData.Pos;
	}
	else if(pstAni->iType == OBJ_MONSTER)
	{
		pstPos = &pstAni->stObj.pstMon->stCurrPos;

		//Ӷ������������Ӷ��
		if(is_mon_bub(pstAni->stObj.pstMon))
		{
			return -1;
		}
	}
	
	else
	{
		return -1;
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	//�߲���ȥ�ĵط�Ҳ��"����"��ѹ����
	if (!IsPixelLineThrough(pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, pstPos->X, pstPos->Y, maskStop) ||
		!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, 
			pstPos->X, pstPos->Y, z_get_monsterdef_misctype(pstEnv,pstMon),DYN_MSK_STOP)) 
	{
		return -1;
	}

	return 0;
}

int is_no_restore_hp_mon(ZONESVRENV* pstEnv, Monster *pstMon)
{
	ZONEMAPINST* pstMapInst;
	ZONEPWORLDINST* pstPworldInst = NULL;
	ZONEPWORLD *pstZonePworld;
	BATTLEDEF *pstBattleDef;
	MONSTERDEF *pstMonDef;
	
	pstMonDef = z_get_mon_def(pstEnv,  pstMon);
	if(pstMonDef && (pstMonDef->MiscType & MON_MISC_TYPE_NO_RESTORE_HP))
	{
		return 1;
	}
	
	if(is_city_build_mon(pstMon))
	{
		return 1;
	}

	if(pstMon->bSubType != MON_SUB_TYPE_BOSS)
	{
		return 0;
	}

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
		return 0;


	pstPworldInst	=	z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if(NULL == pstPworldInst)
		return 0;


	pstZonePworld = z_find_pworld(pstEnv->pstMapObj, &pstPworldInst->stPworldIdx);
	if(NULL == pstZonePworld || !is_normal_battle_pworld(pstZonePworld))
	{
		return 0;
	}

	pstBattleDef = z_find_battledef(pstEnv, pstZonePworld->stPworldDef.BattleID);
	if(NULL == pstBattleDef)
	{
		return 0;
	}

	if(pstBattleDef->FightType == BATTLE_FIGHT_TYPE_AOSHAN)
	{
		return 1;
	}

	return 0;
}

int is_single_convoy_car(Monster *pstMon)
{
	if(pstMon->bSubType == MON_SUB_TYPE_SINGLE_CAR || 
		pstMon->bSubType == MON_SUB_TYPE_SINGLE_SAFECAR)
	{
		return 1;
	}

	return 0;
}
