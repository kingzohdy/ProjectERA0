/*
**  @file $RCSfile: zone_combat.c,v $
**  general description of this module
**  $Id: zone_combat.c,v 1.1329 2014/06/16 09:28:35 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2014/06/16 09:28:35 $
**  @version $Revision: 1.1329 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/
#include "zone_combat.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_skill.h"
#include "zone_move.h"
#include "zone_act_status.h"
#include "zone_attr.h"
#include "mapmask.h"
#include "zone_map.h"
#include "zone_npc.h"
#include "zone_player.h"
#include "zone_status.h"
#include "zone_package.h"
#include "zone_err.h"
#include "zone_ai.h"
#include "zone_team.h"
#include "zone_tmp.h"
#include "zone_task.h"
#include "zone_relation.h"
#include "zone_pet.h"
#include "zone_status.h"
#include "zone_svr.h"
#include "zone_clan.h"
#include "zone_oplog.h"
#include "zone_machine.h"
#include "zone_misc.h"
#include "zone_collect.h"
#include "zone_booty.h"
#include "zone_city.h"
#include "zone_clan_city.h"
#include "zone_rune.h"
#include "zone_script.h"
#include "zone_addattr.h"
#include "zone_designation.h"
#include "zone_trans.h"
#include "zone_genius.h"
#include "zone_err.h"
#include "zone_battle.h"
#include "zone_arm.h"
#include "zone_ploy.h"
#include "zone_span.h"

#define K_HURT_RATE 0.02

extern ScriptEnv stSEnv;
//反射的最长距离
#define REFLECT_MAX_DISTANCE 2500 

int g_iMissTime = 500;

static const double adCollectPowerEffect[] =
{
	100.0f, // 0
	100.0f, // 1
	150.0f, // 2
	200.0f, // 3
	225.0f, // 4
	250.0f, // 5
};

typedef struct
{
	int iCollectNumAtkAdd;
	int iCollectNumDeadAdd;
}CollectNumAdd;

typedef struct{
	short AtkeeFearIceHurtMul;          
       short AtkeeFearFireHurtMul;         
       short AtkeeFearThunderHurtMul;  
       short AtkeeFearLightHurtMul;       
       short AtkeeFearNightHurtMul;      
       short AtkeeFearPhsicHurtMul;       
       short AtkeeFearMagicHurtMul;      
       short AtkeeFearHurtMul;    
	   
       int AtkeeFearIceHurtAdd;             
       int AtkeeFearFireHurtAdd;            
       int AtkeeFearThunderHurtAdd;     
       int AtkeeFearLightHurtAdd;          
       int AtkeeFearNightHurtAdd;          
       int AtkeeFearPhsicHurtAdd;          
       int AtkeeFearMagicHurtAdd;         
       int AtkeeFearHurtAdd; 
	   
       short AtkeeIceHurtMul;          
       short AtkeeFireHurtMul;         
       short AtkeeThunderHurtMul;  
       short AtkeeLightHurtMul;       
       short AtkeeNightHurtMul;      
       short AtkeePhsicHurtMul;       
       short AtkeeMagicHurtMul;      
       short AtkeeHurtMul;            
	   
       int AtkeeIceHurtAdd;             
       int AtkeeFireHurtAdd;            
       int AtkeeThunderHurtAdd;     
       int AtkeeLightHurtAdd;          
       int AtkeeNightHurtAdd;          
       int AtkeePhsicHurtAdd;          
       int AtkeeMagicHurtAdd;         
       int AtkeeHurtAdd;                  

	short nIceAtkChgMul;
	short nFireAtkChgMul;
	short nThunderAtkChgMul;
	short nLightAtkChgMul;
	short nNightAtkChgMul;
	short nPhsicAtkChgMul;
		
	int iIceAtkChgAdd;
	int iFireAtkChgAdd;
	int iThunderAtkChgAdd;
	int iLightAtkChgAdd;
	int iNightAtkChgAdd;
	int iPhsicAtkChgAdd;

	short nDecIceAtkChgMul;
	short nDecFireAtkChgMul;
	short nDecThunderAtkChgMul;
	short nDecLightAtkChgMul;
	short nDecNightAtkChgMul;
	short nDecPhsicAtkChgMul;
		
	int iDecIceAtkChgAdd;
	int iDecFireAtkChgAdd;
	int iDecThunderAtkChgAdd;
	int iDecLightAtkChgAdd;
	int iDecNightAtkChgAdd;
	int iDecPhsicAtkChgAdd;

	short nIceDefChgMul;
	short nFireDefChgMul;
	short nThunderDefChgMul;
	short nLightDefChgMul;
	short nNightDefChgMul;
	short nPhsicDefChgMul;
	
	int iIceDefChgAdd;
	int iFireDefChgAdd;
	int iThunderDefChgAdd;
	int iLightDefChgAdd;
	int iNightDefChgAdd;
	int iPhsicDefChgAdd;

	short nDecIceDefChgMul;
	short nDecFireDefChgMul;
	short nDecThunderDefChgMul;
	short nDecLightDefChgMul;
	short nDecNightDefChgMul;
	short nDecPhsicDefChgMul;
	
	int iDecIceDefChgAdd;
	int iDecFireDefChgAdd;
	int iDecThunderDefChgAdd;
	int iDecLightDefChgAdd;
	int iDecNightDefChgAdd;
	int iDecPhsicDefChgAdd;

	short AtkerIceHurtMul;          
       short AtkerFireHurtMul;         
       short AtkerThunderHurtMul;  
       short AtkerLightHurtMul;       
       short AtkerNightHurtMul;       
       short AtkerPhsicHurtMul;       
       short AtkerMagicHurtMul;      
       short AtkerHurtMul; 
	   
       int AtkerIceHurtAdd;             
       int AtkerFireHurtAdd;            
       int AtkerThunderHurtAdd;     
       int AtkerLightHurtAdd;          
       int AtkerNightHurtAdd;          
       int AtkerPhsicHurtAdd;          
       int AtkerMagicHurtAdd;         
       int AtkerHurtAdd;     

	short DecAtkerIceHurtMul;          
       short DecAtkerFireHurtMul;         
       short DecAtkerThunderHurtMul;  
       short DecAtkerLightHurtMul;       
       short DecAtkerNightHurtMul;       
       short DecAtkerPhsicHurtMul;       
       short DecAtkerMagicHurtMul;      
       short DecAtkerHurtMul; 
	   
       int DecAtkerIceHurtAdd;             
       int DecAtkerFireHurtAdd;            
       int DecAtkerThunderHurtAdd;     
       int DecAtkerLightHurtAdd;          
       int DecAtkerNightHurtAdd;          
       int DecAtkerPhsicHurtAdd;          
       int DecAtkerMagicHurtAdd;         
       int DecAtkerHurtAdd;        

	int iFight;
	
	short nHeavyHitAdd;	//暴击概率
	int nHeavyHitVal; 	//暴击数值
	
	short nAtkerHitRateAdd;
	short nFleeChgMul;
	short nHitChgMul;
	short nDamageMul; //破坏力乘修正
	
	int iHitChgAdd;
	int iFleeChgAdd;
	int iDamageAdd; //破坏力加修正
	   	
}StatusTmpAttr;

typedef enum _COMBAT_CALC_FROM {
	FROM_SKILL,    // 来自于技能
	FROM_EXTENDED, // 来自于附魔
	FROM_STATUS,   // 来自与status本身的result
	               // ...
}COMBAT_CALC_FROM;

int z_get_pet_dyn(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn, tdr_ulonglong ullPetWID);
int z_get_fairy_dyn(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn,SKILLDEF *pstSkillDef,AtkProcess *pstAtkProcess);
static int z_player_normal_atk_cacl(ZONESVRENV* pstEnv, ATKCHG *pstAtkChg,SKILLDEF *pstSkillDef ,Player *pstPlayer, AtkProcess *pstAtkProcess);
int player_hit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
               TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg, ZoneSkill *pstZoneSkill, 
               SKILLDEF *pstSkillDef, SKILLDEF *pstOriginDef, int iAtkChgIdx);
int z_set_cool(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, SKILLDEF *pstSkillDef, 
			   /*CSFITTINGSROW *pstFittings,*/ ZoneSkill *pstZoneSkill, struct timeval *pstCltTime, int iIgnoreSelf);
int z_set_comm_cool(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, SKILLDEF *pstSkillDef, /*CSFITTINGSROW *pstFittings,*/ struct timeval *pstCltTime);
int z_machine_hit_player(ZONESVRENV* pstEnv, Monster *psMachine, Player *pstAtkee);

int z_beat_beak(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, BeatBackInfo *pstBeatBackInfo, ZoneAni *pstAtkee);

// 角色伤害转换计算
static int z_player_hurt_absorb(ZONESVRENV* pstEnv, Player *pstAtkee,
                                CSONEHIT *pstOneHit, int *piHurt);

// 怪物伤害吸收计算
static int z_mon_hurt_absorb(ZONESVRENV* pstEnv, Monster *pstAtkee,
                             CSONEHIT *pstOneHit, int *piHurt);

static int z_hurt_reflect(ZONESVRENV* pstEnv, int *piHurt,
                              STATUSRESULT *pstStatusResult, ZoneReflectedDamage *pstReflectedDamage);

int z_player_be_reflected(ZONESVRENV *pstEnv, Player *pstPlayer, 
                                 AtkProcess *pstAtkProc);

int z_send_player_hit_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                          Player *pstPlayer, RESPOS *pstAtkPos, CSPKG *pstPkg);

int z_send_mon_hit_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon,
                       CSPKG *pstPkg);

int z_do_skill_result_add_hp_by_hurt_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, AtkProcess *pstAtkProcess);

int z_do_skill_result_add_hp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, SKILLRESULT *pstResult);

int one_hit_notify(ZONESVRENV* pstEnv, AtkProcess *pstAtkProc, CSONEHIT *pstOneHit, int iNotify);

int make_hit_res(ZONESVRENV* pstEnv, AtkProcess *pstAtkProc, CSONEHIT *pstOneHitArry, int iHitNum, CSPKG *pstPkg);

SKILLDEF* z_get_rune_skill(ZONESVRENV* pstEnv,ZoneSkill *pstZoneSkill,SKILLDEF *pstOriginDef);

int player_hit_jiqi_full_add_state_target( ZONESVRENV* pstEnv,AtkProcess *pstAtkProcess, 
						AtkeeInfo *pstAtkeeInfo, SKILLDEF *pstSkillDef, int iJiqiNum);

void player_def_hp_chg(Player *pstAtker, Player *pstPlayer, double *pdHP, int iSkillType);

int player_def_hp_chg_rate(Player *pstAtker, Player *pstPlayer, int iSkillType);

int pet_attr_hurt_mul(ZONESVRENV* pstEnv, AtkerInfo *pstAtkerInfo, Player *pstAtkee);

int is_tishen_skill(SKILLDEF *pstSkillDef)
{
	if(pstSkillDef->TargetType & (SKILL_TARGET_ENEMY_MON |SKILL_TARGET_ENEMY_PLAYER))
	{
		return 1;
	}

	return 0;
}

inline void  SET_PLAYER_FIGHT(ZONESVRENV* pstEnv, Player *pstPlayer, time_t  tCurr, int iFightDur)
{	
	int i;
	ZoneTeam *pstTeam;
	Player *pstTeamPlayer;

	if ( !(pstPlayer->stOnline.State & CS_STAT_QIANXIN))
	{
		if (!(pstPlayer->stOnline.State & CS_STAT_FIGHT))
		{
			z_send_player_fight(pstEnv, pstPlayer, ENTER_FIGHT_STATUS);
		}
		pstPlayer->stOnline.tFightStatEnd = tCurr + iFightDur;
		pstPlayer->stOnline.State |= CS_STAT_FIGHT;	
	}
	
	if ( pstPlayer->stRoleData.MiscInfo.TeamID != 0  && is_valid_booty_team(pstEnv, pstPlayer))
	{
		pstTeam = player_team_get(pstEnv, pstPlayer);
		if (NULL == pstTeam)
		{
			return ;
		}

		for (i=0; i<pstTeam->stTeamInfo.MemberNum; i++)
		{
			pstTeamPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstTeam->stTeamInfo.TeamMember[i].MemberName);
			if (NULL == pstTeamPlayer || pstTeamPlayer == pstPlayer || (pstTeamPlayer->stOnline.State & CS_STAT_DEAD))
			{
				continue;
			}

			if ( (pstTeamPlayer->stRoleData.Map == pstPlayer->stRoleData.Map) && 
				(z_distance(&pstPlayer->stRoleData.Pos, &pstTeamPlayer->stRoleData.Pos) < MAX_TEAM_FIGHT_AREA) &&
				!(pstTeamPlayer->stOnline.State & CS_STAT_QIANXIN) &&
				(pstPlayer->stOnline.stWarOL.cCampIdx == pstTeamPlayer->stOnline.stWarOL.cCampIdx) )
			{
				if (!(pstTeamPlayer->stOnline.State & CS_STAT_FIGHT))
				{
					z_send_player_fight(pstEnv, pstTeamPlayer, ENTER_FIGHT_STATUS);
				}
				
				pstTeamPlayer->stOnline.tFightStatEnd = tCurr + iFightDur;
				pstTeamPlayer->stOnline.State |= CS_STAT_FIGHT;
			}
		}
	}
	
	return ;
}

int z_send_player_fight(ZONESVRENV* pstEnv, Player *pstPlayer, int iFightType)
{
	CSPKG stPkgRes;
	CSACTIONRES *pstActionRes = &stPkgRes.Body.ActionRes;

	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->ActionID = ACTION_FIGHT;

	pstActionRes->Data.Fight.FightStatus = iFightType;

	Z_CSHEAD_INIT(&stPkgRes.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkgRes, 0);

	return 0;
}

int z_player_fight_update(ZONESVRENV* pstEnv, Monster* pstMon, ZoneAni *pstAtker)
{
	int i;
	Player *pstPlayer = NULL;
	Monster* pstViewMon;
		
	
	switch(pstAtker->iType)
	{
		case OBJ_PLAYER:
			pstPlayer = pstAtker->stObj.pstPlayer;
			break;
		case OBJ_PET:
			pstPlayer = pstAtker->stObj.stZonePet.pstOwner;
			break;
		default:
			return -1;
			break;
	}
	if( NULL == pstPlayer )
	{
		return -1;
	}
	
	for (i=pstPlayer->stOnline.iVAnimate-1; i>=0; i--)
	{
		
		pstViewMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i]);
		if (pstViewMon && pstViewMon->stAtkTargets.iTargetCur == pstPlayer->iMemID && pstViewMon != pstMon)
		{
			return -1;
		}
	}

	if (pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec, 2);
	}

	return 0;
}

void z_skill_clear(Player *pstPlayer)
{
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;

	pstPlayer->stOnline.State &= ~CS_STAT_PREPARE;
	pstPlayer->stOnline.State &= ~CS_STAT_MOVE_PREPARE;
	pstPlayer->stOnline.State &= ~CS_STAT_ATK;
	pstPlayer->stOnline.State &= ~CS_STAT_MOVE_ATK;
	pstAtkInfo->unPilot = 0;
	pstAtkInfo->unSkillID = 0;
	pstAtkInfo->iJiqiTime = 0;
	pstAtkInfo->cPilotEnd = 0;
	pstAtkInfo->cAtkFlag = 0;
	pstAtkInfo->cFittingsIdx = -1;
	pstAtkInfo->iAtkFlagBit = 0;
			
	return ;
}

int z_player_ride_action(ZONESVRENV* pstEnv, Player *pstPlayer, int fRide)
{
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	pstActionRes->ActionID = ACTION_RIDE;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.Ride.RideItemID = pstPlayer->stOnline.RideItemID;
	pstActionRes->Data.Ride.RideLevel = pstPlayer->stOnline.RideLevel;
	pstActionRes->Data.Ride.RideFlag = fRide;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int z_hitch_ride_clear(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	Player *pstOwner;
	
	if(!(CS_STAT_HITCH_RIDE & pstPlayer->stOnline.State))
	{
		return -1;
	}	
	
	if( pstPlayer->stOnline.cMoving )
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1);
	}

	pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.iOwnerID);
	if (pstOwner)
	{
		//空出座位
		for(i=pstOwner->stOnline.stRideOnInfo.iPassengerNum-1 ; i>=0; i--)
		{
			if (pstOwner->stOnline.stRideOnInfo.PassengerID[i] == pstPlayer->iMemID)
			{
				pstOwner->stOnline.stRideOnInfo.iPassengerNum--;
				if(i != pstOwner->stOnline.stRideOnInfo.iPassengerNum)
				{
					memmove(&pstOwner->stOnline.stRideOnInfo.PassengerID[i],
								&pstOwner->stOnline.stRideOnInfo.PassengerID[i+1],
								(pstOwner->stOnline.stRideOnInfo.iPassengerNum - i)*sizeof(pstOwner->stOnline.stRideOnInfo.PassengerID[0]));
				}
			}	
		}	
		//更新外观
		z_leave_ride(pstEnv, pstOwner, pstPlayer);
	}

	//停下来
	z_player_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, NULL);

	pstPlayer->stOnline.State &= ~CS_STAT_HITCH_RIDE;
	pstPlayer->stOnline.stRideOnInfo.iOwnerID = 0;
	pstPlayer->stOnline.stRideOnInfo.iPassengerNum = 0;
	pstPlayer->stOnline.RideItemID = 0;
	pstPlayer->stOnline.RideLevel = 0;
	
	z_player_chk_mspd(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	return 0;
}

int  z_ride_clear(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	if (pstPlayer->stOnline.State & CS_STAT_RIDE_PREPARE)
	{
		pstPlayer->stOnline.State &= ~CS_STAT_RIDE_PREPARE;

		return 0;
	}
	else if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		return z_hitch_ride_clear(pstEnv, pstPlayer);
	}
	else
	{
		//清理马上得乘客
		if (pstPlayer->stOnline.stRideOnInfo.iPassengerNum)
		{
			int i;
			Player *pstTmpPlayer;

			for(i = 0; i<pstPlayer->stOnline.stRideOnInfo.iPassengerNum; i++)
			{
				pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.stRideOnInfo.PassengerID[i]);
				if (pstTmpPlayer == NULL || pstTmpPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
				{
					continue;
				}

				//客人下马
				z_hitch_ride_clear(pstEnv, pstTmpPlayer);
			}
		}
	}

	pstPlayer->stOnline.State &= ~CS_STAT_HITCH_RIDE;
	pstPlayer->stOnline.State &= ~CS_STAT_RIDE_PREPARE;
	pstPlayer->stOnline.State &= ~CS_STAT_RIDE_ON;
	pstPlayer->stOnline.State &= ~CS_STAT_FLY;
	pstPlayer->stOnline.RideItemID = 0;
	pstPlayer->stOnline.RideLevel = 0;
	pstPlayer->stOnline.stRideOnInfo.iOwnerID = 0;
	pstPlayer->stOnline.stRideOnInfo.iPassengerNum = 0;

	z_player_chk_mspd(pstAppCtx, pstEnv, pstPlayer);

	//z_check_ride_use(pstEnv, pstPlayer);
	
	return 0;
}

int z_ride_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int  fDoNotNotify = 0;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	int iRideFlag = 0;

	if (0 == (pstPlayer->stOnline.State & (CS_STAT_RIDE_PREPARE | CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE)))
	{
		pstActionRes->ActionID = ACTION_RIDE;
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->Data.Ride.RideItemID = 0;
		pstActionRes->Data.Ride.RideLevel = 0;
		pstActionRes->Data.Ride.RideFlag = 2;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
		return 0;
	}

	if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		fDoNotNotify = 1;
	}

	if(( CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE  )& pstPlayer->stOnline.State)
	{
		iRideFlag = 0;
	}
	else
	{
		iRideFlag = 2;
	}
	
	z_ride_clear(pstAppCtx, pstEnv, pstPlayer);

	if (!fDoNotNotify)
	{
		pstActionRes->ActionID = ACTION_RIDE;
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->Data.Ride.RideItemID = pstPlayer->stOnline.RideItemID;
		pstActionRes->Data.Ride.RideLevel = pstPlayer->stOnline.RideLevel;
		pstActionRes->Data.Ride.RideFlag = iRideFlag;

		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}

	return 0;	
}


int z_skill_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,
					int iType, int iSkillID)
{
	CSPKG stPkg;
	CSSKILLBRKS *pstBrkS = &stPkg.Body.SkillBrkS;
	ZoneSkill *pstZoneSkill = NULL;
	long long ullCdTime = 0;
	struct timeval stCltTime;

	memset(pstBrkS,0,sizeof(CSSKILLBRKS));	
	memset(&stCltTime,0,sizeof(stCltTime));
	stCltTime.tv_sec = pstAppCtx->stCurr.tv_sec;
	stCltTime.tv_usec = pstAppCtx->stCurr.tv_usec;
	
	z_skill_clear(pstPlayer);

	if(pstPlayer->stOnline.iSkillAtkerID > 0)
	{
		return 0;
	}

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (!pstZoneSkill || 0 == pstZoneSkill->stCoolEnd.tv_sec)
	{
		ullCdTime = 0;
	}
	else
	{
		ullCdTime = z_compare_time(&stCltTime, &pstZoneSkill->stCoolEnd);
		if (ullCdTime< 0)
		{
			ullCdTime = 0;
		}
	}
	
	pstBrkS->ID = pstPlayer->iMemID;
	pstBrkS->BenchTimeMs = z_bench_time(pstPlayer);
	pstBrkS->SkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq+100;
	pstBrkS->SkillCltSeq = pstPlayer->stOnline.stAtkInfo.iSkillCltSeq;
	pstBrkS->LeftCdTime = ullCdTime;
	pstBrkS->Type = iType;
	if(pstBrkS->Type == SKILL_BRK_CD )
	{
		pstBrkS->Data.LeftCdTime = ullCdTime;
	}
	else if(pstBrkS->Type == SKILL_BRK_POS)
	{
		pstBrkS->Data.CurrPos = pstPlayer->stRoleData.Pos;
	}
	

	Z_CSHEAD_INIT(&stPkg.Head, SKILL_BRK_SVR);

	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	//z_cltmsg_send(pstEnv, pstPlayer, &stPkg);
	
	return 0;
}

int skill_map_check(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLDEF *pstSkillDef)
{
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIdx;
	ZONEPWORLD *pstPworld;
	ZONEIDX stIdx;
	ITEMDEF *pstItemTmpDef;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
		
	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) 
	{
		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld)
		{
			if (pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_SKILL)
			{
				pstItemTmpDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
				if (NULL == pstItemTmpDef || 
					(pstSkillDef->SkillID != pstItemTmpDef->ItemTmp.SkillID_1 && pstSkillDef->SkillID != pstItemTmpDef->ItemTmp.SkillID_2))
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL1);
					z_skill_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillDef->SkillID);
					return -1;
				}
			}

			if (pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_WUSHUANG_SKILL)
			{
				if (pstSkillDef->WushuangFlag > 0)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL2);
					z_skill_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillDef->SkillID);
					return -1;
				}
			}
		}
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;

	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (!pstMapInst) return -1;
	pstMapIdx =  z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx) return -1;

	if (pstMapIdx->stMapDef.CtrlFlag & MAP_CTRL_NO_SKILL)
	{
		pstItemTmpDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (NULL == pstItemTmpDef || 
			(pstSkillDef->SkillID != pstItemTmpDef->ItemTmp.SkillID_1 && pstSkillDef->SkillID != pstItemTmpDef->ItemTmp.SkillID_2))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL1);
			z_skill_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillDef->SkillID);
			return -1;
		}
	}

	return 0;
}

int player_prepare(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	long long llRet;
	struct timeval stConnectTime, stCltTime;
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef;
	SKILLDEF *pstOriginDef = NULL;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSSKILLPRES *pstPrepareS = &pstPkgRes->Body.SkillPreS;
	ZONE_SVRCONF *pstConf = pstEnv->pstConf;
	CSSKILLPRE *pstPrepare = &pstCsPkg->Body.SkillPre;
	int iIdx = -1, iSkillLevel = 0;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int iGnoreCool = 0;
	int i;

	player_sham_dead_clear(pstEnv, pstPlayer);
	
	pstAtkInfo->iSkillCltSeq = pstPrepare->SkillSeq;
		
	if (pstPrepare->SkillSeq <= pstAtkInfo->iSkillSeq)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d seq %d online seq %d", 
					pstPlayer->stRoleData.Uin, pstPrepare->SkillID, pstPrepare->SkillSeq, pstAtkInfo->iSkillSeq);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
		return -1;
	}
	memset(&stConnectTime,0,sizeof(stConnectTime));
	stConnectTime.tv_sec = pstFrameHead->TimeStamp.TimeVal.sec;
	stConnectTime.tv_usec = pstFrameHead->TimeStamp.TimeVal.usec;

	memset(&stCltTime,0,sizeof(stCltTime));
	stCltTime.tv_sec = pstPrepare->SvrTime.BenchTimeSec;
	stCltTime.tv_usec = pstPrepare->SvrTime.BenchTimeUSec;

	llRet = z_compare_time(&stCltTime, &stConnectTime);
	if (llRet > pstConf->BenchTimeAttackMax || llRet < pstConf->BenchTimeAttackMin)
	{
		z_adjust_time(&stCltTime, &stConnectTime, pstPlayer);
		
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time clt and connect time %lld", pstPlayer->stRoleData.Uin, llRet);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
		return -1;
	}
	
	if (pstPlayer->stOnline.cCheckCltTimeFlag)
	{
		if (pstPlayer->stOnline.stLastCltTime.tv_sec !=0)
		{
			llRet = z_compare_time(&pstPlayer->stOnline.stLastCltTime, &stCltTime);

			if (llRet < 0)
			{
				tlog_info(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time lastclttime and clttime fail llRet %lld", 
					pstPlayer->stRoleData.Uin, llRet);
				z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
				return -1;
			}
		}
	
		TV_CLONE(pstPlayer->stOnline.stLastCltTime, stCltTime);
	}
	else
		return -1;

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstPrepare->SkillID);
	if (NULL == pstZoneSkill)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill prepare no zoneskill", 
					pstPlayer->stRoleData.Uin, pstPrepare->SkillID);
		return -1;
	}

	//状态改变技能等级优先
	for(i=0;i<MAX_STATUS_CHG_SKILL_LEVEL;i++)
	{
		if(!pstDyn->astStatusChgSkill[i].unSkillID)
		{
			continue;
		}
		
		if(pstDyn->astStatusChgSkill[i].unSkillID == pstZoneSkill->stSkillData.SkillID)
		{
			iSkillLevel = pstDyn->astStatusChgSkill[i].ucLevel;
			pstOriginDef = z_find_skill_idx(pstEnv, pstZoneSkill->stSkillData.SkillID, iSkillLevel, NULL);
			break;
		}
	}
	
	if(NULL == pstOriginDef)
	{
		//装备影响
		iIdx = z_find_arm_dyn_skill(pstPlayer, pstZoneSkill->stSkillData.SkillID);
		if ( iIdx >= 0)
		{
			iSkillLevel = get_add_skill_level(pstEnv, pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel, pstDyn->astArmSkill[iIdx].ucLevel);
			pstOriginDef = z_find_skill_idx(pstEnv, pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel + iSkillLevel, NULL);
		}
		else
		{
			pstOriginDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
		}
	}

	if (NULL == pstOriginDef)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill prepare no defskill", 
					pstPlayer->stRoleData.Uin, pstPrepare->SkillID);
		return -1;
	}

	pstSkillDef = z_get_rune_skill(pstEnv,pstZoneSkill,pstOriginDef);
	if (!pstSkillDef)
	{
		pstSkillDef = pstOriginDef;
	}

	if (0 > skill_map_check(pstEnv, pstPlayer, pstSkillDef))
	{
		return -1;
	}

	if (z_skill_get_result(pstSkillDef, RESULT_PET_HETI))
	{
		if (pet_heti_check(pstEnv, pstPlayer, NULL) < 0)
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
			return -1;
		}
	}

	if (NO_FIGHT_USE_PREPARE == pstSkillDef->NoFightStatUse && (CS_STAT_FIGHT & pstPlayer->stOnline.State))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL3);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
		return -1;
	}

	if ( ENTER_FIGHT_USE_PREPARE== pstSkillDef->NoFightStatUse && ((CS_STAT_FIGHT & pstPlayer->stOnline.State) == 0))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL4);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
		return -1;
	}

	if(pstSkillDef->CommCool > 0)
	{
		//检查公共冷却
		llRet = z_compare_time(&pstPlayer->stOnline.stSkillList.stCommCoolEnd, &stCltTime);
		if (llRet + CLT_TIME_WAVE < 0)
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time clt and commcool time %lld", pstPlayer->stRoleData.Uin, llRet);
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
			return -1;
		}
	}

	// 检查自我冷却
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_IGNORE_COOL) && (pstZoneSkill->stSkillData.SkillID <= 5000 ||
		(pstZoneSkill->stSkillData.SkillID >= 7000 && pstZoneSkill->stSkillData.SkillID <= 7003)))
	{
		iGnoreCool = 1;
	}

	if (!iGnoreCool)
	{
		llRet = z_compare_time(&pstZoneSkill->stCoolEnd, &stCltTime);
		if (llRet + CLT_TIME_WAVE < 0)
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time clt and selfcool time %lld", pstPlayer->stRoleData.Uin, llRet);
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
			return -1;
		}
	}
	
	if (0 > z_trust_clt_pos(pstAppCtx, pstEnv, pstPlayer, pstPrepare->x, pstPrepare->y, &stCltTime, 1))
	{
		/*
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
		z_player_stop(pstAppCtx, pstEnv, pstPlayer, 1, &stCltTime);
		tlog_error(pstEnv->pstLogCat, 0, 0, "stop player uin %d name %s", pstPlayer->stRoleData.Uin,
		 	pstPlayer->stRoleData.RoleName);
		return -1;*/
		tlog_info(pstEnv->pstLogCat, 0, 0, "check combat pos player uin %d name %s svrpos(%d,%d), prepare pos(%d, %d)", 
			pstPlayer->stRoleData.Uin, 	pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.Pos.X,
			pstPlayer->stRoleData.Pos.Y, pstPrepare->x, pstPrepare->y);
		
		pstPrepare->x = pstPlayer->stRoleData.Pos.X;
		pstPrepare->y = pstPlayer->stRoleData.Pos.Y;
	}

	//处理状态绑定技能
	if (BINDSKILL_TYPE_STATUS == pstSkillDef->BindSkill)
	{
		ZoneStatus *pstZoneStatus;
		pstZoneStatus = z_find_bindskill_status(pstEnv, pstPlayer, pstSkillDef->SkillID);
		if (NULL == pstZoneStatus)
		{
			return -1;
		}

		if (pstZoneStatus->bBindSkillTrig == STATUS_TRIG_END)
		{
			return -1;
		}
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL) && 
		BINDSKILL_TYPE_STATUS != pstSkillDef->BindSkill &&
		pstSkillDef->IgnoreNoSkill == 0)
	{
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
		return -1;
	}
	
	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_PREPARE))
	{
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPrepare->SkillID);
		return -1;
	}

	//设置公共冷却
	z_set_comm_cool(pstAppCtx, pstEnv, pstPlayer, pstSkillDef, &stCltTime);
	//这个放到atk去了
	//z_set_cool(pstAppCtx, pstEnv, pstPlayer, pstSkillDef,  pstZoneSkill, &stCltTime,iGnoreCool);
	

	if (pstSkillDef->MovePrepare)
	{
		pstPlayer->stOnline.State |= CS_STAT_MOVE_PREPARE;
	}
	else
	{
		pstPlayer->stOnline.State |= CS_STAT_PREPARE;
	}
	
	pstAtkInfo->unSkillID = pstPrepare->SkillID;
	pstAtkInfo->unLevel = pstSkillDef->Level;
	pstAtkInfo->iSkillSeq = pstPrepare->SkillSeq;
	//pstAtkInfo->cFittingsIdx = pstPrepare->FittingsIdx;
	pstAtkInfo->stAtkPos = pstPrepare->AtkPos;
	TV_CLONE(pstAtkInfo->stPrepare, stCltTime);

	//广播
	Z_CSHEAD_INIT(&pstPkgRes->Head, SKILL_PREPARE_SVR);
	pstPrepareS->ID = pstPlayer->iMemID;
	pstPrepareS->SkillID = pstPrepare->SkillID;
	pstPrepareS->SkillLevel = pstOriginDef->Level;
	pstPrepareS->AtkPos = pstPrepare->AtkPos;
	pstPrepareS->AtkeeID = 0;
	//pstPrepareS->FittingsEffectID = pstPrepare->FittingsEffectID;
	//TODO: 这里可能要改, 改成符文替换后的id
	pstPrepareS->RuneTransferedID = pstSkillDef->SkillID;

	if(CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State)
	{
		Monster* pstMon = machine_get_by_player( pstEnv, pstPlayer);
		if (pstMon)
			z_sendpkg_mon_area_view(pstEnv->pstAppCtx, pstEnv, pstMon, pstPkgRes);
	}
	else
	{
		z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 0, pstPkgRes, 0);
	}
	return 0;
}

int player_skill_brk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{	
	CSPKG stPkg;
	CSSKILLBRKS *pstBrkS = &stPkg.Body.SkillBrkS;
	UNUSED(pstFrameHead);
	UNUSED(pstCsPkg);
		
	z_skill_clear(pstPlayer);

	pstBrkS->ID = pstPlayer->iMemID;
	pstBrkS->BenchTimeMs = z_bench_time(pstPlayer);
	pstBrkS->SkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq+100;
	pstBrkS->SkillCltSeq = pstPlayer->stOnline.stAtkInfo.iSkillCltSeq;

	Z_CSHEAD_INIT(&stPkg.Head, SKILL_BRK_SVR);

	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 0, &stPkg, 0);

	return 0;
}

int z_set_comm_cool(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
                    SKILLDEF *pstSkillDef, /*CSFITTINGSROW *pstFittings,*/
                    struct timeval *pstCltTime)
{
	long long  llMs=0;
	struct timeval stDiff;
	long long iTmp=0;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	memset(&stDiff,0,sizeof(stDiff));
	//公共冷却
	//if (pstFittings->CommCoolTime > 0)
	if (pstSkillDef->CommCool > 0)
	{
		TV_DIFF(stDiff, pstZoneSkillList->stCommCoolEnd, *pstCltTime);
		TV_TO_MS(llMs, stDiff);
		//iTmp = pstFittings->CommCoolTime;
		iTmp = pstSkillDef->CommCool;
		iTmp = iTmp * 100;
		if (iTmp > llMs)
		{
			TV_ADD_MS(pstZoneSkillList->stCommCoolEnd, *pstCltTime, iTmp);
		}
	}
	return 0;
}
int z_set_self_cool(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,
                    SKILLDEF *pstSkillDef, /*CSFITTINGSROW *pstFittings,*/
                    ZoneSkill *pstZoneSkill, struct timeval *pstCltTime,
                    int iIgnoreSelf)
{

	int i;
	long long  llMs=0;
	struct timeval stDiff;
	SKILLDEF *pstOtherSkillDef;
	ZoneSkill *pstOtherZoneSkill;
	long long iTmp=0;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	memset(&stDiff,0,sizeof(stDiff));

	//自我冷却
	if (pstSkillDef->StatusChg.StatusID > 0 && z_player_find_status(pstPlayer, pstSkillDef->StatusChg.StatusID))
	{
		TV_DIFF(stDiff, pstZoneSkill->stCoolEnd, *pstCltTime);
		TV_TO_MS(llMs, stDiff);
		iTmp = pstSkillDef->StatusChg.Cool;
		iTmp = iTmp*100;
		if (iTmp > llMs)
		{
			TV_ADD_MS(pstZoneSkill->stCoolEnd, *pstCltTime, iTmp);
		}

		if (iIgnoreSelf)
		{
			TV_CLONE(pstZoneSkill->stCoolEnd, pstAppCtx->stCurr);
		}
	}
	//else if (pstFittings->SelfCoolTime > 0)
	else if (pstSkillDef->SelfCool > 0)
	{
		TV_DIFF(stDiff, pstZoneSkill->stCoolEnd, *pstCltTime);
		TV_TO_MS(llMs, stDiff);
		//iTmp = pstFittings->SelfCoolTime;
		iTmp = pstSkillDef->SelfCool;
		iTmp = iTmp*100;
		if (iTmp > llMs)
		{
			TV_ADD_MS(pstZoneSkill->stCoolEnd, *pstCltTime, iTmp);
		}

		if (iIgnoreSelf)
		{
			TV_CLONE(pstZoneSkill->stCoolEnd, pstAppCtx->stCurr);
		}
	}

	//指定冷却
	if (pstSkillDef->SpeCoolType1 > 0 && pstSkillDef->SpeCoolVal1 > 0)
	{
		for (i=0; i<pstZoneSkillList->nSkillNum; i++)
		{
			pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
			if (pstOtherZoneSkill == pstZoneSkill)
			{
				continue;
			}
			
			pstOtherSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
			if (pstOtherSkillDef && pstOtherSkillDef->SelfSpeCoolType == pstSkillDef->SpeCoolType1)
			{
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				iTmp = pstSkillDef->SpeCoolVal1;
				iTmp = iTmp*100;
				if (iTmp > llMs)
				{
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, iTmp);
				}

			}
		}
	}

	if (pstSkillDef->SpeCoolType2 > 0 && pstSkillDef->SpeCoolVal2 > 0)
	{
		for (i=0; i<pstZoneSkillList->nSkillNum; i++)
		{
			pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
			if (pstOtherZoneSkill == pstZoneSkill)
			{
				continue;
			}
			
			pstOtherSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
			if (pstOtherSkillDef && pstOtherSkillDef->SelfSpeCoolType == pstSkillDef->SpeCoolType2)
			{
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				iTmp = pstSkillDef->SpeCoolVal2;
				iTmp = iTmp*100;
				if (iTmp > llMs)
				{
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, iTmp);
				}
			}
		}
	}

	if (pstSkillDef->SpeCoolType3 > 0 && pstSkillDef->SpeCoolVal3 > 0)
	{
		for (i=0; i<pstZoneSkillList->nSkillNum; i++)
		{
			pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
			if (pstOtherZoneSkill == pstZoneSkill)
			{
				continue;
			}
			
			pstOtherSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
			if (pstOtherSkillDef && pstOtherSkillDef->SelfSpeCoolType == pstSkillDef->SpeCoolType3)
			{
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				iTmp = pstSkillDef->SpeCoolVal3;
				iTmp = iTmp*100;
				if (iTmp > llMs)
				{
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, iTmp);
				}
			}
		}
	}

	if (pstSkillDef->SpeCoolType4 > 0 && pstSkillDef->SpeCoolVal4 > 0)
	{
		for (i=0; i<pstZoneSkillList->nSkillNum; i++)
		{
			pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
			if (pstOtherZoneSkill == pstZoneSkill)
			{
				continue;
			}
			
			pstOtherSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
			if (pstOtherSkillDef && pstOtherSkillDef->SelfSpeCoolType == pstSkillDef->SpeCoolType4)
			{
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				iTmp = pstSkillDef->SpeCoolVal4;
				iTmp = iTmp*100;
				if (iTmp > llMs)
				{
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, iTmp);
				}
			}
		}
	}
	return 0;
}

int z_set_cool(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, SKILLDEF *pstSkillDef, 
			   /*CSFITTINGSROW *pstFittings,*/ ZoneSkill *pstZoneSkill, struct timeval *pstCltTime, int iIgnoreSelf)
{
	int i;
	long long  llMs=0;
	struct timeval stDiff;
	SKILLDEF *pstOtherSkillDef;
	ZoneSkill *pstOtherZoneSkill;
	long long iTmp=0;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;

	memset(&stDiff,0,sizeof(stDiff));
	//公共冷却
	//if (pstFittings->CommCoolTime > 0)
	if (pstSkillDef->CommCool > 0)
	{
		TV_DIFF(stDiff, pstZoneSkillList->stCommCoolEnd, *pstCltTime);
		TV_TO_MS(llMs, stDiff);
		//iTmp = pstFittings->CommCoolTime;
		iTmp = pstSkillDef->CommCool;
		iTmp = iTmp * 100;
		if (iTmp > llMs)
		{
			TV_ADD_MS(pstZoneSkillList->stCommCoolEnd, *pstCltTime, iTmp);
		}
	}

	//自我冷却
	if (pstSkillDef->StatusChg.StatusID > 0 &&
		z_player_find_status(pstPlayer, pstSkillDef->StatusChg.StatusID))
	{
		TV_DIFF(stDiff, pstZoneSkill->stCoolEnd, *pstCltTime);
		TV_TO_MS(llMs, stDiff);
		iTmp = pstSkillDef->StatusChg.Cool;
		iTmp = iTmp * 100;
		if (iTmp > llMs)
		{
			TV_ADD_MS(pstZoneSkill->stCoolEnd, *pstCltTime, iTmp);
		}

		if (iIgnoreSelf)
		{
			TV_CLONE(pstZoneSkill->stCoolEnd, pstAppCtx->stCurr);
		}
	}
	//else if (pstFittings->SelfCoolTime > 0)
	else if (pstSkillDef->SelfCool > 0)
	{
		TV_DIFF(stDiff, pstZoneSkill->stCoolEnd, *pstCltTime);
		TV_TO_MS(llMs, stDiff);
		//iTmp = pstFittings->SelfCoolTime;
		iTmp = pstSkillDef->SelfCool;
		iTmp = iTmp * 100;
		if (iTmp > llMs)
		{
			TV_ADD_MS(pstZoneSkill->stCoolEnd, *pstCltTime, iTmp);
		}

		if (iIgnoreSelf)
		{
			TV_CLONE(pstZoneSkill->stCoolEnd, pstAppCtx->stCurr);
		}
	}

	//指定冷却
	if (pstSkillDef->SpeCoolType1 > 0 && pstSkillDef->SpeCoolVal1 > 0)
	{
		for (i=0; i<pstZoneSkillList->nSkillNum; i++)
		{
			pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
			if (pstOtherZoneSkill == pstZoneSkill)
			{
				continue;
			}
			
			pstOtherSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
			if (pstOtherSkillDef && pstOtherSkillDef->SelfSpeCoolType == pstSkillDef->SpeCoolType1)
			{
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				iTmp = pstSkillDef->SpeCoolVal1;
				iTmp = iTmp*100;
				if (iTmp > llMs)
				{
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, iTmp);
				}

			}
		}
	}

	if (pstSkillDef->SpeCoolType2 > 0 && pstSkillDef->SpeCoolVal2 > 0)
	{
		for (i=0; i<pstZoneSkillList->nSkillNum; i++)
		{
			pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
			if (pstOtherZoneSkill == pstZoneSkill)
			{
				continue;
			}
			
			pstOtherSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
			if (pstOtherSkillDef && pstOtherSkillDef->SelfSpeCoolType == pstSkillDef->SpeCoolType2)
			{
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				iTmp = pstSkillDef->SpeCoolVal2;
				iTmp = iTmp*100;
				if (iTmp > llMs)
				{
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, iTmp);
				}
			}
		}
	}

	if (pstSkillDef->SpeCoolType3 > 0 && pstSkillDef->SpeCoolVal3 > 0)
	{
		for (i=0; i<pstZoneSkillList->nSkillNum; i++)
		{
			pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
			if (pstOtherZoneSkill == pstZoneSkill)
			{
				continue;
			}
			
			pstOtherSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
			if (pstOtherSkillDef && pstOtherSkillDef->SelfSpeCoolType == pstSkillDef->SpeCoolType3)
			{
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				iTmp = pstSkillDef->SpeCoolVal3;
				iTmp = iTmp*100;
				if (iTmp > llMs)
				{
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, iTmp);
				}
			}
		}
	}

	if (pstSkillDef->SpeCoolType4 > 0 && pstSkillDef->SpeCoolVal4 > 0)
	{
		for (i=0; i<pstZoneSkillList->nSkillNum; i++)
		{
			pstOtherZoneSkill = &pstZoneSkillList->astZoneSkills[i];
			if (pstOtherZoneSkill == pstZoneSkill)
			{
				continue;
			}
			
			pstOtherSkillDef = z_find_skill_zoneskill(pstEnv, pstOtherZoneSkill);
			if (pstOtherSkillDef && pstOtherSkillDef->SelfSpeCoolType == pstSkillDef->SpeCoolType4)
			{
				TV_DIFF(stDiff, pstOtherZoneSkill->stCoolEnd, *pstCltTime);
				TV_TO_MS(llMs, stDiff);
				iTmp = pstSkillDef->SpeCoolVal4;
				iTmp = iTmp*100;
				if (iTmp > llMs)
				{
					TV_ADD_MS(pstOtherZoneSkill->stCoolEnd, *pstCltTime, iTmp);
				}
			}
		}
	}
	
	return 0;
}

int z_mon_skill_consume(ZONESVRENV* pstEnv, Monster *pstMon, ResourceConsume *pstConsume)
{
	int iOldHP = pstMon->iHP;

	if (pstConsume->iHPUse)
	{
		if(pstMon->iHP - pstConsume->iHPUse <=0 )
		{
			pstMon->iHP = 1; 
		}
		else if(pstMon->iHP - pstConsume->iHPUse > pstMon->iMaxHP)
		{
			pstMon->iHP = pstMon->iMaxHP; 
		}
		else
		{
			pstMon->iHP -= pstConsume->iHPUse;
		}
	}

	if(iOldHP != pstMon->iHP)
	{
		mon_hp_action(pstEnv, pstMon, iOldHP, 0);
	}
	
	return 0;
}


// 客户端需要表现的
int z_skill_consume_is_action(ZONESVRENV* pstEnv, Player *pstPlayer, ResourceConsume *pstConsume, ZoneSkill *pstZoneSkill, 
									int iDeadAtk, int iHitFlag)
{
	PlayerChagAttrVal stChgVal;
	//SKILLDEF *pstSkillDef;	//comment by paraunused.pl
	int iOldHp = pstPlayer->stRoleData.RoleDetail.HPCurr;
	int iOldMp = pstPlayer->stRoleData.RoleDetail.MPCurr;
	int iHpUse = 0;
	int iMPUse = 0;

	stChgVal.AttrNum = 0;
	if (iDeadAtk)
	{
		iMPUse = pstConsume->iMPUse - pstPlayer->stOnline.stDyn.stMidAttrList.uiKillAddMp;
		iHpUse = pstConsume->iHPUse - pstPlayer->stOnline.stDyn.stMidAttrList.uiKillAddHp;
	}
	else
	{
		iMPUse = pstConsume->iMPUse;
		iHpUse = pstConsume->iHPUse;
	}

	if (iMPUse > 0)
	{
		if(iHitFlag && z_player_find_status(pstPlayer, STATUS_ID_NEXT_SKILL_NO_MP))
		{
			iMPUse = 0;
			z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, STATUS_ID_NEXT_SKILL_NO_MP, 1, 0);
		}
		
		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_MP;
		stChgVal.AttrList[stChgVal.AttrNum].nFlag = 1;
		stChgVal.AttrList[stChgVal.AttrNum].Value.MP = iMPUse;
		stChgVal.AttrNum++;	
	}
	else if (iMPUse < 0)
	{
		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_MP;
		stChgVal.AttrList[stChgVal.AttrNum].nFlag = 0;
		stChgVal.AttrList[stChgVal.AttrNum].Value.MP = -iMPUse;
		stChgVal.AttrNum++;
	}

	//天赋 普通攻击触发回蓝
	/*
	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
	if( pstSkillDef != NULL )
	{
		iMPUse = genius_atk_random_add_mp(pstEnv->pstAppCtx,  pstEnv, pstPlayer, pstSkillDef->SkillType );
		// 消耗负数就是加
		if (iMPUse > 0)
		{
			stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_MP;
			stChgVal.AttrList[stChgVal.AttrNum].nFlag = 0;
			stChgVal.AttrList[stChgVal.AttrNum].Value.MP = iMPUse ;
			stChgVal.AttrNum++;
		}
	}*/

	// HP消耗
	if (iHpUse > 0)
	{
		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_HP;
		stChgVal.AttrList[stChgVal.AttrNum].nFlag = 1;
		stChgVal.AttrList[stChgVal.AttrNum].Value.HP = iHpUse;
		stChgVal.AttrNum++;	
	}
	else if(iHpUse < 0)
	{
		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_HP;
		stChgVal.AttrList[stChgVal.AttrNum].nFlag = 0;
		stChgVal.AttrList[stChgVal.AttrNum].Value.HP = -iHpUse;
		stChgVal.AttrNum++;	
	}
	

	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stChgVal, 0);
	}

	if (	iOldHp != (int)pstPlayer->stRoleData.RoleDetail.HPCurr ||
		iOldMp != (int)pstPlayer->stRoleData.RoleDetail.MPCurr)
	{
		player_hp_action(pstEnv, pstPlayer, iOldHp, iOldMp, 0);
	}

	return 0;
	UNUSED(pstZoneSkill);	//add by paraunused.pl
}

int z_skill_consume(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ResourceConsume *pstConsume, 
						ZoneSkill *pstZoneSkill, int iDeadAtk, int iHitFlag)
{
	PlayerChagAttrVal stChgVal;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	SKILLDEF *pstSkillDef=NULL;
		
	stChgVal.AttrNum = 0;
	z_skill_consume_is_action(pstEnv, pstPlayer, pstConsume, pstZoneSkill,iDeadAtk, iHitFlag);

	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
	if( !pstSkillDef )
	{
		return -1;
	}
	
	// Xp 消耗
	if (pstConsume->iXpUse > 0)
	{
		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_XP;
		stChgVal.AttrList[stChgVal.AttrNum].nFlag = 1;
		stChgVal.AttrList[stChgVal.AttrNum].Value.XP = pstConsume->iXpUse;
		stChgVal.AttrNum++;	
	}
	
	if (pstConsume->iCollectNumUse != 0)
	{
		pstZoneSkill->stSkillData.CollectNum -= pstConsume->iCollectNumUse;
		if (pstZoneSkill->stSkillData.CollectNum < 0)
		{
			pstZoneSkill->stSkillData.CollectNum = 0;
		}
		else if (pstZoneSkill->stSkillData.CollectNum > pstZoneSkill->stSkillData.CollectNumMax)
		{
			pstZoneSkill->stSkillData.CollectNum = pstZoneSkill->stSkillData.CollectNumMax;
		}

		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_COLLECTNUM;
		stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.SkillID = pstZoneSkill->stSkillData.SkillID;
		stChgVal.AttrList[stChgVal.AttrNum].Value.CollectNum.CollectNum = pstZoneSkill->stSkillData.CollectNum;
		stChgVal.AttrNum++;	
	}

	if (pstConsume->iEnergeUse != 0)
	{
		if ((int)pstRoleTmpSys->TmpEnerge >= pstConsume->iEnergeUse)
		{
			pstRoleTmpSys->TmpEnerge -= pstConsume->iEnergeUse;
		}
		
		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_TMPENERGE;
		if( pstRoleTmpSys->TmpEnerge > 0)
		{
			stChgVal.AttrList[stChgVal.AttrNum].Value.Energe = pstRoleTmpSys->TmpEnerge;
		}
		else
		{
			stChgVal.AttrList[stChgVal.AttrNum].Value.Energe = 0;
		}
		stChgVal.AttrNum++;	
	}

	// 积累数技能消耗
	if (pstConsume->iCollectSkillUse > 0)
	{
		stChgVal.AttrList[stChgVal.AttrNum].AttrID = ATTR_ID_COLLECTSKILLUSE;
		stChgVal.AttrList[stChgVal.AttrNum].nFlag = 1;
		stChgVal.AttrList[stChgVal.AttrNum].Value.CollectSkillUse = pstConsume->iCollectSkillUse;
		stChgVal.AttrNum++;	
	}
	else if(pstConsume->iCollectSkillUse < 0)
	{
		//加积累数
		player_collectskill_add_slot( pstEnv, pstPlayer,-pstConsume->iCollectSkillUse,COLLECTSKILL_ADDTYPE_VAL);
	}
			
	if (stChgVal.AttrNum > 0)
	{
		z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stChgVal, 1);
	}

	//消耗弹药
	if (pstConsume->iAmmID > 0)
	{
		package_dec(pstAppCtx, pstEnv, pstPlayer, pstConsume->iAmmID, pstConsume->iAmmUse, LIST_TYPE_SUNDRIES, 
			&pstConsume->iAmmPos,PACKAGE_DEC_NONE, IMPITEM_OPTYPE_SKILL);
	}

	return 0;
}

int z_skill_consume_check(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  ATKCHG *pstAtkChg, 
								ResourceConsume *pstConsume, SKILLDEF *pstSkillDef , ZoneSkill *pstZoneSkill)
{
	int iMPUse=0, iCollectNumUse=0, iAmmUse=0, iMPCurr=0, iXpUse=0,iHPUse=0,iCollectSkillUse=0;
	ROLEDETAIL *pstRoleDetail = &pstPlayer->stRoleData.RoleDetail;
	ZoneStatus *pstZoneStatus;
	

	iMPUse = pstSkillDef->MPUse;
	iCollectNumUse = pstSkillDef->CollectNumUse;
	iAmmUse = pstSkillDef->AmmUse;
	iXpUse =  pstSkillDef->UseXp;
	iMPCurr = pstRoleDetail->MPCurr;
	
	if(pstSkillDef->CollectSkillUse > 0)
	{
		if(z_skill_get_result(pstSkillDef, RESULT_COLLECTSKILL_USEALL))
		{
			iCollectSkillUse = pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_SlotCurr;
		}
		else
		{
			if (pstSkillDef->StatusChg.StatusID > 0 && 
				pstSkillDef->StatusChg.CollectNum > 0 &&
				z_player_find_status(pstPlayer, pstSkillDef->StatusChg.StatusID))
			{
				iCollectSkillUse = pstSkillDef->StatusChg.CollectNum;
			}
			else
			{
				iCollectSkillUse = pstSkillDef->CollectSkillUse;
			}
			
			//天赋对技能积累数减少
			/*
			nGeniusCollectSkillDec = genius_collectskill_dec(pstAppCtx, pstEnv, pstPlayer,pstSkillDef->SkillID);
			if( iCollectSkillUse > nGeniusCollectSkillDec)
			{
				iCollectSkillUse -= nGeniusCollectSkillDec;
			}*/
		}
	}
	
	if ( (iMPCurr < pstConsume->iMPUse + iMPUse) ||
		((int)pstRoleDetail->XP < iXpUse)||
		/*((int)pstRoleDetail->SpeMovePower < pstConsume->iSpdUse+ iSpdUse) ||*/
		(pstZoneSkill->stSkillData.CollectNum < pstConsume->iCollectNumUse + iCollectNumUse) ||
		((int)pstPlayer->stRoleData.MiscInfo.TmpSys.TmpEnerge < pstConsume->iEnergeUse + pstSkillDef->TmpEnergeUse) ||
		( pstPlayer->stRoleData.MiscInfo.CollectSkillInfo.CollectSkill_SlotCurr < pstConsume->iCollectSkillUse + iCollectSkillUse) )
	{
		return -1;
	}

	pstZoneStatus = z_get_player_zonestatus_by_resultid(pstEnv, pstPlayer, STATUS_RESULT_CONSUME_MP_BY_OVERLAY);
	if(pstZoneStatus)
	{
		STATUSDEF *pstStatusDef;
		STATUSRESULT *pstStatusResult;
		pstStatusDef = z_find_status_petstatus(pstEnv, pstZoneStatus);
		if(pstStatusDef)
		{
			pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_CONSUME_MP_BY_OVERLAY);
			if(pstStatusResult)
			{
				if((pstStatusResult->ResultVal2 &&
				    pstStatusResult->ResultVal2 == pstSkillDef->SkillID) ||
				    (pstStatusResult->ResultVal3 &&
				    pstStatusResult->ResultVal3 == pstSkillDef->SkillID) ||
				    (pstStatusResult->ResultVal4 &&
				    pstStatusResult->ResultVal4 == pstSkillDef->SkillID) ||
				    (pstStatusResult->ResultVal2 == 0) )
				{
					iMPUse = iMPUse * (1+pstZoneStatus->unOverlay)*(1+pstStatusResult->ResultVal1/100.0);
				}
			}
		}
	}
	

	pstConsume->iMPUse += iMPUse;
	pstConsume->iCollectNumUse += iCollectNumUse;
	pstConsume->iEnergeUse += pstSkillDef->TmpEnergeUse;
	pstConsume->iSpdUse += pstSkillDef->SpdUse;
	pstConsume->iXpUse += pstSkillDef->UseXp;
	pstConsume->iCollectSkillUse += iCollectSkillUse;

	//天赋 普通攻击触发回血
	//iHPUse = genius_atk_random_add_hp( pstAppCtx,  pstEnv, pstPlayer, pstSkillDef->SkillType );
	// 消耗负数就是加
	pstConsume->iHPUse -= iHPUse;

	if ( pstSkillDef->CollectNumAtkAdd > 0 )
	{
		//加积累数
		player_collectskill_add_slot( pstEnv, pstPlayer,pstSkillDef->CollectNumAtkAdd,COLLECTSKILL_ADDTYPE_VAL);
	}

		//消耗弹药
	if (pstSkillDef->AmmID > 0 )
	{
		pstConsume->iAmmID = pstSkillDef->AmmID;
		pstConsume->iAmmUse = pstSkillDef->AmmUse;
		if (0 > package_dec_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstConsume->iAmmID, pstConsume->iAmmUse, LIST_TYPE_SUNDRIES, NULL,PACKAGE_DEC_NONE))
		{
			if(pstSkillDef->BindSkill == BINDSKILL_TYPE_CALN_ITEM)
			{
				player_drop_clan_tem(pstEnv,pstPlayer);
			}
			return -1;
		}
	}	
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstAtkChg);
}

int z_check_skill_arm(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef, int *piAtkChgIdx)
{
	int i,j;
	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ITEMDEF *pstItemDef = NULL;

	//*piAtkChgIdx : 0 表示技能释放包含装备数值， -1表示不包含装备数值
	if (pstZoneSkill && pstZoneSkill->ullPetWID > 0)
	{
		*piAtkChgIdx =0;
		return 0;
	}

	if (ARM_ANY == pstSkillDef->ArmType[0].ArmType)
	{
		*piAtkChgIdx = -1;
		return 0;
	}

	if (ARM_ANY_EX == pstSkillDef->ArmType[0].ArmType)
	{
		*piAtkChgIdx = 0;
		return 0;
	}

	*piAtkChgIdx = -1;	
	for(i=0;i<pstRoleWear->ArmNum;i++)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)&pstRoleWear->RoleArms[i]);
		if(!pstItemDef)
		{
			continue;
		}

		for (j=0; j<MAX_SKILL_ARMTYPE; j++)
		{
			if (0 == pstSkillDef->ArmType[j].ArmType)
			{
				continue;
			}
			if (pstItemDef->NumType == pstSkillDef->ArmType[j].ArmType)
			{
				break;
			}
		}
		if(j<MAX_SKILL_ARMTYPE)
		{
			break;
		}
	}
	
	if(i<pstRoleWear->ArmNum)
	{
		*piAtkChgIdx = 0;
		return 0;
	}
	return -1;
}

SKILLDEF* z_get_rune_skill(ZONESVRENV* pstEnv,ZoneSkill *pstZoneSkill,SKILLDEF *pstOriginDef)
{
	SKILLDEF *pstSkillDef = NULL;
	
	if (pstZoneSkill->stSkillData.RuneSlotEnabled &&
		pstZoneSkill->stSkillData.RuneSlot > 0 &&
		pstZoneSkill->stSkillData.RuneSlot < RUNE_TYPE_NUM)
	{
		switch (pstZoneSkill->stSkillData.RuneSlot)
		{
		case RUNE_RED:
			pstSkillDef = z_find_skill(pstEnv,
			                           pstOriginDef->RedRuneTransferedID,
			                           pstOriginDef->Level, NULL);
			break;

		case RUNE_BLUE:
			pstSkillDef = z_find_skill(pstEnv,
			                           pstOriginDef->BlueRuneTransferedID,
			                           pstOriginDef->Level, NULL);
			break;			

		case RUNE_YELLOW:
			pstSkillDef = z_find_skill(pstEnv,
			                           pstOriginDef->YellowRuneTransferedID,
			                           pstOriginDef->Level, NULL);
			break;

		default:
			pstSkillDef = NULL;
			break;
		}
	}

	return pstSkillDef;
}

//技能怪触发的攻击
int player_atk_by_skill_mon(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	Monster* pstMon;
	SKILLDEF * pstSkillDef;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	CSSKILLATK *pstSkillAtk = &pstCsPkg->Body.SkillAtk;
	AtkInfo stAtkInfoTmp;
	struct timeval stTmp;
	struct timeval stTmpCommCool;	
	int StateTmp;	
	int i=0;
	int j=0;
	
	if(pstSkillAtk->MonID<=0)
	{
		return -1;
	}

	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstSkillAtk->MonID);
	if(!pstMon)
	{
		return -1;
	}

	if(pstMon->iOwnerID != pstPlayer->iMemID || 
		(MON_SUB_TYPE_SKILL_DESTORY != pstMon->bSubType && MON_SUB_TYPE_SKILL_NODESTORY != pstMon->bSubType))
	{
		return -1;
	}

	z_monster_move_curr(pstAppCtx, pstEnv, pstMon, 0);
	
	pstSkillDef = mon_find_one_skilldef(pstEnv, pstMon);
	if(!pstSkillDef)
	{
		goto final;
	}

	if(pstSkillAtk->SkillID != pstSkillDef->SkillID)
	{
		goto final;
	}

	if(pstSkillAtk->TargetNum ==0)
	{
		goto final;
	}	

	if( (400+pstMon->unRadius)<ptolinesegdist_ex(pstMon->stInitPos,pstMon->stCurrPos,pstSkillAtk->AtkPos))
	{
		goto final;
	}

	if( pstSkillAtk->MonID != pstPlayer->stOnline.iSkillMonID )
	{
		pstPlayer->stOnline.SkillMonTargetNum = 0;
	}
	else if(pstPlayer->stOnline.SkillMonTargetNum + pstSkillAtk->TargetNum > MAX_SKILL_TARGET)
	{
		pstPlayer->stOnline.SkillMonTargetNum = 0;
		goto final;
	}
	
	for(i=0;i<pstSkillAtk->TargetNum;i++)
	{
		for(j=0;j<pstPlayer->stOnline.SkillMonTargetNum;j++)
		{
			if(pstSkillAtk->TargetIDs[i] == pstPlayer->stOnline.SkillMonTargetIDs[j])
			{
				pstPlayer->stOnline.SkillMonTargetNum = 0;
				goto final;
			}
		}
	}
	
	for(i=0;i<pstSkillAtk->TargetNum;i++)
	{
		pstPlayer->stOnline.SkillMonTargetIDs[pstPlayer->stOnline.SkillMonTargetNum++]=pstSkillAtk->TargetIDs[i];
	}

	z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstSkillDef->SkillID, pstSkillDef->Level, 0);
	stTmp = pstPlayer->stOnline.stLastCltTime;
	stTmpCommCool= pstPlayer->stOnline.stSkillList.stCommCoolEnd;
	//先存下
	memcpy(&stAtkInfoTmp,pstAtkInfo,sizeof(AtkInfo));
	StateTmp = pstPlayer->stOnline.State;

	pstPlayer->stOnline.iSkillMonID = pstSkillAtk->MonID;
	pstSkillAtk->MonID = 0;
	pstAtkInfo->iAtkFlagBit = 0;
	pstAtkInfo->iAtkFlagBit |= ATK_FLAG_BIT_CLT_ATK;
	player_atk(pstAppCtx, pstEnv, pstPlayer, pstFrameHead,pstCsPkg);
	pstAtkInfo->iAtkFlagBit = 0;
	//恢复,避免吟唱啥的被打断
	memcpy(pstAtkInfo,&stAtkInfoTmp,sizeof(AtkInfo));
	//pstPlayer->stOnline.State = StateTmp;		
    if(!(pstPlayer->stOnline.State&CS_STAT_DEAD))
	{
		pstPlayer->stOnline.State = StateTmp;		
	}
	pstPlayer->stOnline.stLastCltTime = stTmp;
	pstPlayer->stOnline.stSkillList.stCommCoolEnd = stTmpCommCool;	
	z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillDef->SkillID, 0);

	if(pstMon->bSubType != MON_SUB_TYPE_SKILL_NODESTORY ||
	   pstPlayer->stOnline.SkillMonTargetNum >= MAX_SKILL_TARGET )
	{
		z_mon_destroy(pstEnv, pstMon);
	}
	
	return 0;
	
final:
	z_mon_destroy(pstEnv, pstMon);
	return -1;
}
//服务器加技能攻击
int player_atk_by_add_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,
								int iSkillID,int iLevel,RESPOS *pstAtkPos)
{
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef;
	int iOldSkillSeq;
	TFRAMEHEAD stFrameHead;
	CSPKG stPkg;
	struct timeval stTmp;
	struct timeval stTmpCommCool;
	CSSKILLATK *pstSkillAtk = &stPkg.Body.SkillAtk;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	AtkInfo stAtkInfoTmp;
	int StateTmp = 0;
	int iAddFlag = 0;

	pstSkillDef = z_find_skill(pstEnv, iSkillID, iLevel,NULL);
	if(!pstSkillDef)
	{
		return -1;
	}
	
	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (!pstZoneSkill)
	{
		z_add_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, NULL, pstSkillDef->SkillID, pstSkillDef->Level, 0);
		iAddFlag = 1;
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
		if (!pstZoneSkill)
		{
			return -1;
		}
	}
	
	iOldSkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq;
	stFrameHead.TimeStamp.TimeVal.sec = pstEnv->pstAppCtx->stCurr.tv_sec;
	stFrameHead.TimeStamp.TimeVal.usec = pstEnv->pstAppCtx->stCurr.tv_usec;
	pstSkillAtk->SkillID = iSkillID;
	pstSkillAtk->SkillSeq = 0;//客户端有特殊处理,不管这个序号
	pstSkillAtk->RolePos = pstPlayer->stRoleData.Pos;
	pstSkillAtk->AtkPos = *pstAtkPos;
	pstSkillAtk->TargetNum = 0;

	player_atk_svr_get_list(pstAppCtx, pstEnv, pstPlayer, pstZoneSkill, pstSkillDef, pstSkillAtk);
	if (pstSkillAtk->TargetNum <= 0)
	{
		if(iAddFlag)
			z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillDef->SkillID, 0);
		return -1;
	}
	
	pstSkillAtk->SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstSkillAtk->SvrTime.BenchTimeUSec = pstEnv->pstAppCtx->stCurr.tv_usec;
	stTmp = pstPlayer->stOnline.stLastCltTime;
	stTmpCommCool= pstPlayer->stOnline.stSkillList.stCommCoolEnd;
	//先存下
	memcpy(&stAtkInfoTmp,pstAtkInfo,sizeof(AtkInfo));
	StateTmp = pstPlayer->stOnline.State;
		
	pstAtkInfo->iAtkFlagBit |= ATK_FLAG_BIT_CLT_ATK;	
	pstAtkInfo->iAtkFlagBit |= ATK_FLAG_BIT_SVR_ATK;		

	player_atk(pstAppCtx, pstEnv, pstPlayer, &stFrameHead, &stPkg);
	//恢复,避免吟唱啥的被打断
	memcpy(pstAtkInfo,&stAtkInfoTmp,sizeof(AtkInfo));
	if(!(pstPlayer->stOnline.State&CS_STAT_DEAD))
	{
		pstPlayer->stOnline.State = StateTmp;
	}

	pstAtkInfo->iAtkFlagBit = 0;
				
	pstPlayer->stOnline.stLastCltTime = stTmp;
	pstPlayer->stOnline.stSkillList.stCommCoolEnd = stTmpCommCool;

	if(iAddFlag)
		z_del_skill(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillDef->SkillID, 0);
	return 0;
}

//弹道表现
int player_ballistic_atk(ZONESVRENV* pstEnv, Player *pstPlayer, CSSKILLATK *pstSkillAtk, ZoneSkill *pstZoneSkill, 
			   SKILLDEF *pstSkillDef)
{
	ZoneObjList stList;
	struct timeval stCltTime;
	SKILLRESULT *pstSkillResult;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;

	memset(&stCltTime,0,sizeof(stCltTime));
	stCltTime.tv_sec = pstSkillAtk->SvrTime.BenchTimeSec;
       stCltTime.tv_usec = pstSkillAtk->SvrTime.BenchTimeUSec;

	pstPlayer->stOnline.State &= ~CS_STAT_ATK;
	pstPlayer->stOnline.State &= ~CS_STAT_MOVE_ATK;

	pstZoneSkill->stAtkInfo.iSkillSeq = pstSkillAtk->SkillSeq;
	pstZoneSkill->stAtkInfo.RolePos = pstSkillAtk->RolePos;
	pstZoneSkill->stAtkInfo.AtkPos = pstSkillAtk->AtkPos;
	pstZoneSkill->stAtkInfo.iTargetNum = 0;
	
	if(pstSkillDef->AtkType == SKILL_ATK_TYPE_TARGET)
	{
		pstZoneSkill->stAtkInfo.iTargetNum = pstSkillAtk->TargetNum;
		memcpy(pstZoneSkill->stAtkInfo.aiTargetIDs, pstSkillAtk->TargetIDs, 
				sizeof(pstZoneSkill->stAtkInfo.aiTargetIDs[0])*pstSkillAtk->TargetNum);
	}

	if (0 > z_player_atklist(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstSkillDef, pstSkillAtk, &stList, &stCltTime))
	{
		z_skill_break(pstEnv->pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL, pstZoneSkill->stSkillData.SkillID);
		return -1;
	}

	pstActionRes->ActionID = ACTION_BALLISTIC_ATK;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.BallisticAtk.SkillID = pstZoneSkill->stSkillData.SkillID;
	pstActionRes->Data.BallisticAtk.Level = pstZoneSkill->stSkillData.SkillLevel;
	pstActionRes->Data.BallisticAtk.RuneTransferedID = pstSkillDef->SkillID;
	pstActionRes->Data.BallisticAtk.SkillSeq = pstSkillAtk->SkillSeq;
	pstActionRes->Data.BallisticAtk.AtkPos = pstSkillAtk->AtkPos;
	pstActionRes->Data.BallisticAtk.TargetNum = pstSkillAtk->TargetNum;
	memcpy(&pstActionRes->Data.BallisticAtk.TargetIDs[0], &pstSkillAtk->TargetIDs[0], 
		sizeof(pstSkillAtk->TargetIDs[0])*pstSkillAtk->TargetNum);
	pstActionRes->Data.BallisticAtk.SelfPosChgFlag = 0;
	
	pstSkillResult = z_skill_get_result(pstSkillDef, RESULT_SELF_POSCHG);
	if (pstSkillResult)
	{
		RESPOS stPos;
		MapIndex *pstMapIdx;
		ZONEMAPINST *pstMapInst;
		ZONEIDX stIdx;

		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
		if (pstMapInst)
		{
			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
			if (0 == z_skill_result_poschg(pstEnv, pstMapInst, pstMapIdx, pstSkillDef, pstSkillResult->SkillResultVal1, 
											pstSkillResult, pstPlayer, &pstSkillAtk->AtkPos, &pstSkillAtk->RolePos, &stPos))
			{
				pstActionRes->Data.BallisticAtk.SelfPosChgFlag = 1;
				pstActionRes->Data.BallisticAtk.SelfPosChg = stPos;
			}
		}
	}

	Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
	z_send_player_hit_res(pstEnv->pstAppCtx, pstEnv, pstPlayer, &pstSkillAtk->AtkPos, pstPkgRes);

	return 0;
}

//弹道攻击命中
int player_ballistic_hit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	int iAtkChgIdx = 0;
	int iIdx = -1, iSkillLevel = 0;
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef, *pstOriginDef = NULL;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	CSSKILLATK *pstSkillAtk = &pstCsPkg->Body.SkillHit;
	int i;

	if(CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State)
	{
		return player_machine_ballistic_hit(pstEnv, pstPlayer, pstSkillAtk);
	}

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstSkillAtk->SkillID);
	if (NULL == pstZoneSkill)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill  no zoneskill", pstPlayer->stRoleData.Uin, pstSkillAtk->SkillID);
		return -1;
	}

	//天赋对机能等级的影响
	//iSkillLevel = genius_atkskill_addlevel(pstAppCtx, pstEnv, pstPlayer,pstZoneSkill->stSkillData.SkillID);

	//状态改变技能等级优先
	for(i=0;i<MAX_STATUS_CHG_SKILL_LEVEL;i++)
	{
		if(!pstDyn->astStatusChgSkill[i].unSkillID)
		{
			continue;
		}
		
		if(pstDyn->astStatusChgSkill[i].unSkillID == pstZoneSkill->stSkillData.SkillID)
		{
			iSkillLevel = pstDyn->astStatusChgSkill[i].ucLevel;
			pstOriginDef = z_find_skill_idx(pstEnv, pstZoneSkill->stSkillData.SkillID, iSkillLevel, NULL);
			break;
		}
	}

	if(NULL == pstOriginDef)
	{
		//装备对技能等级的影响
		iIdx = z_find_arm_dyn_skill(pstPlayer, pstZoneSkill->stSkillData.SkillID);
		if (iIdx >= 0)
		{
			iSkillLevel += get_add_skill_level(pstEnv, pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel, pstDyn->astArmSkill[iIdx].ucLevel);
		}

		if (iSkillLevel > 0)
		{
			pstOriginDef = z_find_skill_idx(pstEnv, pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel + iSkillLevel, NULL);
		}
		else
		{
			pstOriginDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
		}
	}
	
	if (!pstOriginDef)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill atk no defskill", 
					pstPlayer->stRoleData.Uin, pstSkillAtk->SkillID);
		return -1;
	}

	// 符文的效果
	pstSkillDef = z_get_rune_skill(pstEnv,pstZoneSkill,pstOriginDef);
	if (!pstSkillDef)
	{
		pstSkillDef = pstOriginDef;
	}

	if (0 > z_check_skill_arm(pstEnv, pstPlayer, pstZoneSkill, pstSkillDef, &iAtkChgIdx))
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill atk no arm", 
					pstPlayer->stRoleData.Uin, pstSkillAtk->SkillID);
		return -1;
	}

	if (pstSkillAtk->SkillSeq == 0 ||
		pstSkillAtk->SkillSeq != pstZoneSkill->stAtkInfo.iSkillSeq)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d seq %d online seq %d", pstPlayer->stRoleData.Uin, 
						pstSkillAtk->SkillID, pstSkillAtk->SkillSeq, pstZoneSkill->stAtkInfo.iSkillSeq);
		return -1;
	}
	
	player_hit(pstAppCtx, pstEnv, pstPlayer, pstFrameHead, pstCsPkg,
	           	pstZoneSkill, pstSkillDef, pstOriginDef, iAtkChgIdx);

	memset(&pstZoneSkill->stAtkInfo, 0, sizeof(pstZoneSkill->stAtkInfo));
	return 0;
}

int player_atk_check_noskill_incomplete(ZONESVRENV* pstEnv, int iSkillID,  Player *pstPlayer)
{
	SKILLDEF *pstSkillDef;
	STATUSRESULT *pstResult;

	pstSkillDef = z_find_skill( pstEnv,iSkillID, 1, NULL);
	if (!pstSkillDef)
	{
		return 0;
	}

	if (pstSkillDef->IgnoreNoSkill)
	{
		return 0;
	}

	if (pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0)
	{
		return 0;
	}

	pstResult = player_status_result(pstEnv, pstPlayer,STATUS_RESULT_NOSKILL_INCOMPLETE);
	if (!pstResult)
	{
		return 0;
	}

	if (	pstResult->ResultVal1!= iSkillID && 
		pstResult->ResultVal2!= iSkillID&&
		pstResult->ResultVal3!= iSkillID&&
		pstResult->ResultVal4!= iSkillID)
	{
		return -1;
	}
	return 0;
}

int player_atk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
							TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	long long llRet;
	long long  llMs;
	int iTmp;
	struct timeval stConnectTime, stCltTime;
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef, *pstOriginDef = NULL;
	struct timeval stDiff;
	int iAtkChgIdx;
	ResourceConsume stResourceConsume;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	unsigned int iJiqiTime = 0;
	CSSKILLATK *pstSkillAtk = &pstCsPkg->Body.SkillAtk;
	ZONE_SVRCONF *pstConf = pstEnv->pstConf;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	int iSwitch = 0;
	int iIdx = -1, iSkillLevel = 0;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
	int iGnoreCool = 0;
	ITEMDEF *pstItemTmpDef = NULL;
	LightningAtkInfo *pstLightningAtkInfo = NULL;
	int i;


	player_sham_dead_clear(pstEnv, pstPlayer);
	
	if(pstSkillAtk->MonID >0)
	{
		if( 0 <= player_atk_by_skill_mon(pstAppCtx, pstEnv, pstPlayer, pstFrameHead, pstCsPkg))
		{
			return 0;
		}
	}
	pstPlayer->tActive = pstAppCtx->stCurr.tv_sec;

	if( !(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
		!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK))
	{
		pstAtkInfo->iSkillCltSeq = pstSkillAtk->SkillSeq;
	}

	//机械攻击
	if(CS_STAT_RIDE_MON_MACHINE & pstPlayer->stOnline.State)
	{
		if(0 > player_machine_atk(pstEnv, pstPlayer, pstSkillAtk))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return 0;
		}	

		return 0;
	}
	else
	{
		if ( player_atk_check_noskill_incomplete(pstEnv, pstSkillAtk->SkillID, pstPlayer) < 0)
		{
			return 0;
		}	
	}

	memset(&stConnectTime,0,sizeof(stConnectTime));
	memset(&stCltTime,0,sizeof(stCltTime));
	memset(&stDiff,0,sizeof(stDiff));
	
	stConnectTime.tv_sec = pstFrameHead->TimeStamp.TimeVal.sec;
	stConnectTime.tv_usec = pstFrameHead->TimeStamp.TimeVal.usec;

	stCltTime.tv_sec = pstSkillAtk->SvrTime.BenchTimeSec;
	stCltTime.tv_usec = pstSkillAtk->SvrTime.BenchTimeUSec;

	llRet = z_compare_time(&stCltTime, &stConnectTime);
	if (llRet > pstConf->BenchTimeAttackMax || llRet < pstConf->BenchTimeAttackMin)
	{
		z_adjust_time(&stCltTime, &stConnectTime, pstPlayer);
		
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time clt and connect time %lld", pstPlayer->stRoleData.Uin, llRet);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
		return -1;
	}

	
	if (pstPlayer->stOnline.cCheckCltTimeFlag)
	{
		if (pstPlayer->stOnline.stLastCltTime.tv_sec !=0)
		{
			llRet = z_compare_time(&pstPlayer->stOnline.stLastCltTime,
			                      &stCltTime);
			if (llRet < 0)
			{
				tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time lastclttime and clttime fail", pstPlayer->stRoleData.Uin);
				z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
				return -1;
			}
		}
	
		TV_CLONE(pstPlayer->stOnline.stLastCltTime, stCltTime);
	}
	else
		return -1;

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstSkillAtk->SkillID);
	if (NULL == pstZoneSkill)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill  no zoneskill", pstPlayer->stRoleData.Uin, pstSkillAtk->SkillID);
		return -1;
	}

	//天赋对机能等级的影响
	//iSkillLevel = genius_atkskill_addlevel(pstAppCtx, pstEnv, pstPlayer,pstZoneSkill->stSkillData.SkillID);

	//状态改变技能等级优先
	for(i=0;i<MAX_STATUS_CHG_SKILL_LEVEL;i++)
	{
		if(!pstDyn->astStatusChgSkill[i].unSkillID)
		{
			continue;
		}
		
		if(pstDyn->astStatusChgSkill[i].unSkillID == pstZoneSkill->stSkillData.SkillID)
		{
			iSkillLevel = pstDyn->astStatusChgSkill[i].ucLevel;
			pstOriginDef = z_find_skill_idx(pstEnv, pstZoneSkill->stSkillData.SkillID, iSkillLevel, NULL);
			break;
		}
	}

	if(NULL == pstOriginDef)
	{
		//装备对技能等级的影响
		iIdx = z_find_arm_dyn_skill(pstPlayer, pstZoneSkill->stSkillData.SkillID);
		if (iIdx >= 0)
		{
			iSkillLevel += get_add_skill_level(pstEnv, pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel, pstDyn->astArmSkill[iIdx].ucLevel);
		}

		if (iSkillLevel > 0)
		{
			pstOriginDef = z_find_skill_idx(pstEnv, pstZoneSkill->stSkillData.SkillID, pstZoneSkill->stSkillData.SkillLevel + iSkillLevel, NULL);
		}
		else
		{
			pstOriginDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
		}
	}

	if (!pstOriginDef)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill atk no defskill", 
					pstPlayer->stRoleData.Uin, pstSkillAtk->SkillID);
		return -1;
	}

	// 符文的效果
	pstSkillDef = z_get_rune_skill(pstEnv,pstZoneSkill,pstOriginDef);
	if (!pstSkillDef)
	{
		pstSkillDef = pstOriginDef;
	}

	// 处理打坐
	if (pstPlayer->stOnline.State & ( CS_STAT_SIT|CS_STAT_GAOJI_SIT ))
	{
		player_sit_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		if (pstSkillAtk->SkillID == SIT_SKILLID)
		{
			return 0;
		}
	}

	if (pstRoleTmpSys->TmpItemID > 0)
	{
		pstItemTmpDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (NULL == pstItemTmpDef)
		{
			return -1;
		}

		if (pstItemTmpDef->ItemTmp.Type >= ITEM_TMP_ITEM)
		{
			if (pstSkillDef->SkillID != pstItemTmpDef->ItemTmp.SkillID_1 && pstSkillDef->SkillID != pstItemTmpDef->ItemTmp.SkillID_2)
			{
				tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill not tmp skill", 
							pstPlayer->stRoleData.Uin, pstSkillAtk->SkillID);

				z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
				return -1;
			}
		}

		if (pstItemTmpDef->ItemTmp.Type == ITEM_TMP_CLAN)
		{
			if(pstPlayer->stRoleData.Level < pstSkillDef->Learn.LevelMin)
			{
				z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
				return -1;
			}
		}
	}
	
	
	if(CS_STAT_AUTOFLY & pstPlayer->stOnline.State)
	{
		if (NULL == pstItemTmpDef ||
			(pstSkillDef->SkillID != pstItemTmpDef->ItemTmp.SkillID_1 && 
			pstSkillDef->SkillID != pstItemTmpDef->ItemTmp.SkillID_2))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
	}

	if (	pstPlayer->stOnline.stQiecuoInfo.iMemID > 0 && 
		(pstSkillDef->WushuangFlag > 0 || pstSkillDef->FairySkill > 0))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL5);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
		return -1;
	}

	//特定地图不可以使用技能
	if (0 > skill_map_check(pstEnv, pstPlayer, pstSkillDef))
	{
		return -1;
	}

	if(z_skill_get_result(pstSkillDef, RESULT_SELF_POSCHG) &&
		z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOCLT))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL6);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
		return -1;
	}

	//lzk 夫妻技能之我想你技能提前检查
	if(z_skill_get_result(pstSkillDef, RESULT_TRANSMIT_TO_SPOUSE_POS))
	{
		if(0 > z_skill_transmit_spouse_pos_check(pstAppCtx,pstEnv,pstPlayer))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
	}

	//活下去技能提前检查
	if(z_skill_get_result(pstSkillDef, RESULT_RECOVER_SPOUSE_HP))
	{
		if(0 > z_skill_recover_spouse_hp_check(pstAppCtx,pstEnv,pstPlayer,pstSkillDef))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
	}
	
	if (0 > z_check_skill_arm(pstEnv, pstPlayer, pstZoneSkill, pstSkillDef, &iAtkChgIdx))
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d skill atk no arm", 
					pstPlayer->stRoleData.Uin, pstSkillAtk->SkillID);
		return -1;
	}

	if (0 == pstSkillDef->PilotNum || (pstAtkInfo->unPilot > 0 && 8*CLT_TIME_WAVE < z_compare_time(&pstAtkInfo->stPilotEnd, &stCltTime)))
	{
		pstAtkInfo->unPilot = 0;
	}

	if (pstAtkInfo->unPilot)
	{
		if ( !(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
			!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK) &&
			 (pstSkillAtk->SkillSeq != pstAtkInfo->iSkillSeq || pstSkillAtk->SkillID != pstAtkInfo->unSkillID ) )
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d pilot seq %d onlint seq %d", pstPlayer->stRoleData.Uin, 
						pstSkillAtk->SkillID, pstSkillAtk->SkillSeq, pstAtkInfo->iSkillSeq);
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
		
		if (!( (pstPlayer->stOnline.State & CS_STAT_ATK) || (pstPlayer->stOnline.State & CS_STAT_MOVE_ATK) ) )
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player pilot state %d ", pstPlayer->stRoleData.Uin, pstPlayer->stOnline.State);
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
		
		TV_DIFF(stDiff, stCltTime, pstAtkInfo->stLastPilot);
		TV_TO_MS(llMs, stDiff);
		if (llMs + CLT_TIME_WAVE < pstSkillDef->PilotTime*100.0)
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player pilot interval %lld ms", pstPlayer->stRoleData.Uin, llMs);
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
	}
	else if (pstSkillDef->PrepareType == PREPARE_TYPE_NONE)
	{
		if ( !(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
			!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK) &&
			 (pstSkillAtk->SkillSeq <= pstAtkInfo->iSkillSeq)  )
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill seq %d onlint seq %d", pstPlayer->stRoleData.Uin, 
						pstSkillAtk->SkillSeq, pstAtkInfo->iSkillSeq);
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}

		if( !(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_LIGHTNING) &&
			!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK) &&
			!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK))
		{
			if(pstSkillDef->CommCool > 0)
			{
				//检查公共冷却
				llRet = z_compare_time(&pstPlayer->stOnline.stSkillList.stCommCoolEnd, &stCltTime);
				if (llRet + CLT_TIME_WAVE < 0)
				{
					tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time clt and commcool time %lld", pstPlayer->stRoleData.Uin, llRet);
					z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_CD,pstSkillAtk->SkillID);
					return -1;
				}
			}
			
			//检查自我冷却
			if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_IGNORE_COOL) && (pstZoneSkill->stSkillData.SkillID <= 5000 ||
				(pstZoneSkill->stSkillData.SkillID >= 7000 && pstZoneSkill->stSkillData.SkillID <= 7003)))
			{
				iGnoreCool = 1;
			}

			if (!iGnoreCool)
			{
				llRet = z_compare_time(&pstZoneSkill->stCoolEnd, &stCltTime);
				if (llRet + CLT_TIME_WAVE < 0)
				{
					tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time clt and selfcool time %lld", pstPlayer->stRoleData.Uin, llRet);
					z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_CD,pstSkillAtk->SkillID);
					return -1;
				}
			}
		}
	}
	else //检查prepare
	{
		if (!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
			!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK) &&
			(pstSkillAtk->SkillSeq != pstAtkInfo->iSkillSeq || pstAtkInfo->unSkillID !=  pstSkillAtk->SkillID ))
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill  %d seq %d onlint seq %d", pstPlayer->stRoleData.Uin, 
						pstSkillAtk->SkillID, pstSkillAtk->SkillSeq, pstAtkInfo->iSkillSeq);
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
		
		if ( !((pstPlayer->stOnline.State & CS_STAT_PREPARE) ||   (pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE) ) )
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player state %d", pstPlayer->stRoleData.Uin, pstPlayer->stOnline.State);
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}

		TV_DIFF(stDiff, stCltTime, pstAtkInfo->stPrepare);
		TV_TO_MS(llMs, stDiff);

		if(pstSkillDef->JiqiTime > 0)
		{
			if( llMs > pstSkillDef->JiqiTime * 100.0)
			{
				llMs = pstSkillDef->JiqiTime * 100.0;
			}
			else if (llMs < 0)
			{
				llMs = 0;
			}
			iJiqiTime = llMs;
		}
		else 
		{
			iTmp = pstSkillDef->PrepareTime * 100.0;
			if ((llMs + CLT_TIME_WAVE) < iTmp)
			{
				tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d prepare %d real %d", pstPlayer->stRoleData.Uin, 
				           pstSkillAtk->SkillID, llMs, iTmp);
				z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_CD,pstSkillAtk->SkillID);
				return -1;
			}
		}
		
	}

	if ((NO_FIGHT_USE_ATK == pstSkillDef->NoFightStatUse || NO_FIGHT_USE_PREPARE == pstSkillDef->NoFightStatUse ) && (CS_STAT_FIGHT & pstPlayer->stOnline.State))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL3);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstAtkInfo->unSkillID);
		return -1;
	}
	
	if ((ENTER_FIGHT_USE_ATK == pstSkillDef->NoFightStatUse || ENTER_FIGHT_USE_PREPARE == pstSkillDef->NoFightStatUse)&& ((CS_STAT_FIGHT & pstPlayer->stOnline.State) == 0))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL4);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstAtkInfo->unSkillID);
		return -1;
	}

	//角色坐标的处理
	if (!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
				!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK) &&
				0 > z_trust_clt_pos(pstAppCtx, pstEnv, pstPlayer, pstSkillAtk->RolePos.X, pstSkillAtk->RolePos.Y, &stCltTime, 1))
	{
		/*z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
		z_player_stop(pstAppCtx, pstEnv, pstPlayer, 1, &stCltTime);
		tlog_info(pstEnv->pstLogCat, 0, 0, "stop player uin %d name %s", pstPlayer->stRoleData.Uin,
			pstPlayer->stRoleData.RoleName);
		return -1;*/

		tlog_info(pstEnv->pstLogCat, 0, 0, "check combat pos player uin %d name %s svrpos(%d,%d), atk pos(%d, %d)", 
			pstPlayer->stRoleData.Uin, 	pstPlayer->stRoleData.RoleName, pstPlayer->stRoleData.Pos.X,
			pstPlayer->stRoleData.Pos.Y, pstSkillAtk->RolePos.X, pstSkillAtk->RolePos.Y);

		pstSkillAtk->RolePos = pstPlayer->stRoleData.Pos;
	}
	
	/*
	if (0 == pstSkillDef->MoveAttack &&
		 !(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_LIGHTNING) &&
		 !(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK))
	{
		if (pstPlayer->stOnline.cMoving)
			z_player_move_curr_bytime(pstAppCtx, pstEnv, pstPlayer, 0, &stCltTime, 20);

		//if (pstPlayer->stOnline.cMoving)
		{
			if (!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
				!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK) &&
				0 > z_trust_clt_pos(pstAppCtx, pstEnv, pstPlayer, pstSkillAtk->RolePos.X, pstSkillAtk->RolePos.Y, &stCltTime))
			{
				z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
				z_player_stop(pstAppCtx, pstEnv, pstPlayer, 1, &stCltTime);
				tlog_error(pstEnv->pstLogCat, 0, 0, "stop player uin %d name %s", pstPlayer->stRoleData.Uin,
					pstPlayer->stRoleData.RoleName);
				return -1;
			}
		}

		if (pstSkillDef->AttackActTime > 0) //动作表现时间不能移动等
		{
			llMs = pstSkillDef->AttackActTime * 100.0;
		}
	}*/

	//处理状态绑定技能
	if (BINDSKILL_TYPE_STATUS == pstSkillDef->BindSkill)
	{
		ZoneStatus *pstZoneStatus;
		pstZoneStatus = z_find_bindskill_status(pstEnv, pstPlayer, pstSkillDef->SkillID);
		if (NULL == pstZoneStatus)
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
		
		if (pstZoneStatus->bBindSkillTrig == STATUS_TRIG_END)
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
	} 

	if (!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
		!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK))
	{
		if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL) && 
			BINDSKILL_TYPE_STATUS != pstSkillDef->BindSkill &&
			pstSkillDef->IgnoreNoSkill == 0)
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_POS,pstSkillAtk->SkillID);
			z_send_sys_str_msg(pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL7);
			return -1;
		}
	}

	if(pstPlayer->stOnline.iSkillAtkerID == 0)
	{
		if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_ATK))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
	}
	

	if (pstSkillDef->WushuangFlag > 0)
	{
		if (0 > check_replace_skill(pstEnv, pstPlayer, REPLACE_SKILL_OP_WUSHUANG))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}
	}

	if ( pstSkillDef->SkillSwitch == 1 )  
	{
		SKILLRESULT *pstSkillResult;
		
		pstSkillResult = z_skill_get_result(pstSkillDef, RESULT_SELF_STATUS);
		if (pstSkillResult && z_player_find_status(pstPlayer, pstSkillResult->SkillResultVal2) )
		{
			iSwitch = 1;
		}
	}

	//检查资源消耗
	//不是引导技能或者是引导技能的第一次atk 而且不是开关技能的关
	if (0 == pstAtkInfo->unPilot && iSwitch != 1 &&
		!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_LIGHTNING) &&
		!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK)) 
	{
		memset(&stResourceConsume, 0, sizeof(stResourceConsume));
		if (0 > z_skill_consume_check(pstAppCtx, pstEnv, pstPlayer, NULL, &stResourceConsume, pstSkillDef, pstZoneSkill))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstSkillAtk->SkillID);
			return -1;
		}

		z_skill_consume(pstAppCtx, pstEnv, pstPlayer, &stResourceConsume, pstZoneSkill,0, 1);
	}

	if (pstSkillDef->PilotNum > 0)
	{
		if (pstSkillDef->PrepareType == PREPARE_TYPE_NONE)
		{
			if (0 == pstAtkInfo->unPilot)
			{
				pstAtkInfo->cPilotEnd = 0;
				TV_ADD_MS(pstAtkInfo->stPilotEnd, stCltTime, (pstSkillDef->PilotNum-1)*pstSkillDef->PilotTime*100);

				//设置冷却
				z_set_cool(pstAppCtx, pstEnv, pstPlayer, pstSkillDef, /*pstFittings,*/ pstZoneSkill, &stCltTime,iGnoreCool);
				
			}
			
			pstAtkInfo->unPilot++;
			TV_CLONE(pstAtkInfo->stLastPilot, stCltTime);

			if (pstAtkInfo->unPilot >= pstSkillDef->PilotNum)
			{
				pstAtkInfo->cPilotEnd = 1;
				pstAtkInfo->unPilot = 0;
			}
		}
		else
		{
			z_set_self_cool(pstAppCtx, pstEnv, pstPlayer, pstSkillDef, /*pstFittings,*/ pstZoneSkill, &stCltTime,iGnoreCool);
		}
	}
	else if( !(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_LIGHTNING) &&
			 !(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK))
	{
		if (pstSkillDef->PrepareType == PREPARE_TYPE_NONE)
		{
			z_set_cool(pstAppCtx, pstEnv, pstPlayer, pstSkillDef, pstZoneSkill, &stCltTime,iGnoreCool);
		}
		else
		{
			z_set_self_cool(pstAppCtx, pstEnv, pstPlayer, pstSkillDef,  pstZoneSkill, &stCltTime,iGnoreCool);
		}
	}
	
	pstPlayer->stOnline.State &= ~CS_STAT_PREPARE;
	pstPlayer->stOnline.State &= ~CS_STAT_MOVE_PREPARE;
	if (pstSkillDef->MoveAttack)
	{
		pstPlayer->stOnline.State |= CS_STAT_MOVE_ATK;
	}
	else
	{
		pstPlayer->stOnline.State |= CS_STAT_ATK;
	}


	pstAtkInfo->cAtkFlag = 1; 
	pstAtkInfo->unSkillID = pstSkillAtk->SkillID;
	if(!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
		!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK))
	{
		pstAtkInfo->iSkillSeq = pstSkillAtk->SkillSeq;
	}
	pstAtkInfo->iJiqiTime = iJiqiTime;

	/*
	if (pstSkillDef->AttackActTime > 0)
	{
		TV_CLONE(pstAtkInfo->stAtkTime, stCltTime);
	}*/

	if( pstSkillDef->LightningLinkNum > 0 &&
		!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_LIGHTNING))
	{
		pstLightningAtkInfo = z_storage_lightning_link_info(pstPlayer);
		memset(pstLightningAtkInfo,0,sizeof(LightningAtkInfo));
		pstLightningAtkInfo->iJumpNum = pstSkillDef->LightningLinkNum;
		pstLightningAtkInfo->ushSkillID = pstSkillAtk->SkillID;
		pstLightningAtkInfo->iAtkChgIdx = iAtkChgIdx;
	}

	if ( pstSkillDef->UseXp > 0)
	{
		z_role_use_xp_skill_log(pstEnv, pstPlayer, pstSkillDef->SkillID, pstSkillDef->Level,pstSkillDef->UseXp);
	}	

	if (!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_ATK) && 
		!(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK) &&
		(pstSkillAtk->SkillSeq != pstAtkInfo->iSkillSeq || pstAtkInfo->unSkillID != pstSkillAtk->SkillID || 0 == pstAtkInfo->cAtkFlag ))
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d seq %d online seq %d, atkflag=%d", pstPlayer->stRoleData.Uin, 
						pstSkillAtk->SkillID, pstSkillAtk->SkillSeq, pstAtkInfo->iSkillSeq, pstAtkInfo->cAtkFlag);
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstAtkInfo->unSkillID);
		return -1;
	}
	pstAtkInfo->cAtkFlag = 0;

	//处理潜行
	if ((pstPlayer->stOnline.State & CS_STAT_QIANXIN) && (pstSkillDef->SkillID != SKILL_QIANXIN))
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, STATUS_ID_QIANXIN, 1, 0);
		if(pstPlayer->stOnline.State & CS_STAT_QIANXIN)
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d seq %d goodstatusnum %d", pstPlayer->stRoleData.Uin, 
						pstSkillAtk->SkillID, pstSkillAtk->SkillSeq, pstPlayer->stOnline.stStatusList.bGoodStatusNum);
			pstPlayer->stOnline.State &= ~CS_STAT_QIANXIN;
		}
	}

	if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_SKILL)
	{
		player_status_skill_clean(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	if(pstSkillDef->AtkType == SKILL_ATK_TYPE_NORMAL)
	{
		player_hit(pstAppCtx, pstEnv, pstPlayer, pstFrameHead, pstCsPkg,
	           	pstZoneSkill, pstSkillDef, pstOriginDef, iAtkChgIdx);
	}
	else
	{
		player_ballistic_atk(pstEnv, pstPlayer, pstSkillAtk, pstZoneSkill, pstSkillDef);
	}
	
	pstAtkInfo->iAtkFlagBit = 0;

	return 0;
}


// 检查新手保护规则
int z_check_rookie_protect(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee)
{
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	ZONEIDX stIdx;

	if (pstAtker->stRoleData.Map != pstAtkee->stRoleData.Map)
	{
		return 0;
	}
	
	stIdx.iID = pstAtker->stRoleData.Map;
	stIdx.iPos = pstAtker->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
		return 0;
	}

	if (pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_ROOKIE_PROTECT)
	{
		if (	pstAtker->stRoleData.Level < ROOKIE_PROTECT_LEVEL ||
			pstAtkee->stRoleData.Level < ROOKIE_PROTECT_LEVEL)
		{
			return 1;
		}
	}
	return 0;
}

int is_player_in_leitai(Player *pstPlayer)
{
	if(pstPlayer->stOnline.cSecenOverLay & SCENEOVERLAY_PK)
	{
		return 1;
	}
	
	return 0;
}

/*
	pk规则: 有一方可攻击 则双方都可以攻击
	判断可攻击 if(z_player_hit_player_filter(Atker, Atkee) || z_player_hit_player_filter(Atkee, Atker))
*/
int z_player_hit_player_filter(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstAtker);
	WarFieldOL *pstAtkerOL = &pstAtker->stOnline.stWarOL;
	WarFieldOL *pstAtkeeOL = &pstAtkee->stOnline.stWarOL;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	ZONEIDX stIdx;
	int iPkMode;

	if(pstAtkee->stOnline.fRealBiaoChe == 0)
	{
		if (z_is_qiecuo(pstEnv, pstAtker, pstAtkee))
		{
			return 1;
		}

		if (!is_player_in_leitai(pstAtker) || !is_player_in_leitai(pstAtkee))
		{
			// 新手排除在敌人之外
			if (z_check_rookie_protect(pstEnv, pstAtker, pstAtkee))
			{
				return 0;
			}
		}

		if ((pstAtker->stOnline.cSecenOverLay & SCENEOVERLAY_SAFE) ||(pstAtkee->stOnline.cSecenOverLay & SCENEOVERLAY_SAFE))
		{
			return 0;
		}
	}
	else
	{
		Monster *pstMon = mon_biaoche_get_by_player(pstEnv, pstAtkee);
		if(!pstMon)
		{
			return 0;
		}

		if ((pstAtker->stOnline.cSecenOverLay & SCENEOVERLAY_SAFE) ||(pstMon->cSecenOverLay & SCENEOVERLAY_SAFE))
		{
			return 0;
		}
	}
	

	stIdx.iID = pstAtker->stRoleData.Map;
	stIdx.iPos = pstAtker->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return 0;
	}

	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex)
	{
		return 0;
	}

	if (!is_player_in_leitai(pstAtker) || !is_player_in_leitai(pstAtkee))
	{
		if(pstMapIndex->stMapDef.MapLimit.PK == 0)
		{
			return 0;
		}
	}

	iPkMode = pstAtker->stOnline.ePkMode;
	if(pstMapIndex->stMapDef.MapLimit.PKMode > 0)
	{
		iPkMode = pstMapIndex->stMapDef.MapLimit.PKMode;
	}
	
	switch(iPkMode)
	{
		//自己被动模式
		case PK_MODE_NORMAL:
			return 0;
			break;
		case PK_MODE_POLICE:
			{
				if(is_in_pk_time_player(pstEnv, pstAtkee) || is_bad_palyer(pstAtkee))
				{
					return 1;
				}

				/*
				//对方不是你所在公会成员和同盟公会成员
				if(pstZoneClan && clan_relation_by_player(pstEnv, pstAtkee, pstZoneClan) < 0)
				{
					return 1;
				}
				*/
			}
			return 0;
			break;
		//自己组队模式
		case PK_MODE_TEAM:
			//对方不是你的队友
			if( 0 == player_team_same(pstEnv, pstAtker, pstAtkee))
			{
				return 1;
			}

			return 0;
			break;
		//自己公会模式
		case PK_MODE_CLAN:
			{
				ZoneClan *pstAtkeeClan = player_get_clan(pstEnv, pstAtkee);
				
				if(NULL == pstZoneClan || NULL == pstAtkeeClan)
				{
					return 1;
				}
				
				//对方不是你所在公会成员和同盟公会成员
				if(pstZoneClan != pstAtkeeClan && clan_relation(pstZoneClan, pstAtkeeClan) <= 0)
				{
					return 1;
				}
			}
	
			return 0;
			break;
		//自己阵营模式
		case PK_MODE_CAMP:
			//对方和你不是同阵营
			if(pstAtkerOL->cCampIdx ==  0 || pstAtkerOL->cCampIdx != pstAtkeeOL->cCampIdx)
			{
				return 1;
			}
			
			return 0;
			break;
		//自己全体模式
		case PK_MODE_ALL:
			return 1;
			break;
		//公会宣战模式
		case PK_MODE_ENEMY_CLAN:
			{
				ZoneClan *pstAtkeeClan = player_get_clan(pstEnv, pstAtkee);
				
				if(is_in_pk_time_player(pstEnv, pstAtkee) || is_bad_palyer(pstAtkee))
				{
					if(NULL == pstZoneClan || pstZoneClan != pstAtkeeClan)
					{
						return 1;
					}
				}

				if(NULL == pstZoneClan || NULL == pstAtkeeClan)
				{
					return 0;
				}
				
				//对方不是你所在公会成员和同盟公会成员
				if(pstZoneClan != pstAtkeeClan && clan_relation(pstZoneClan, pstAtkeeClan) < 0)
				{
					return 1;
				}
			}
			break;
		default:
			return 0;
			break;
	}

	return 0;
}

/*
	非敌方，同队伍，或者同公会，或者同阵营为友方
*/
int is_skill_target_friend_player(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee)
{
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstAtker);
	WarFieldOL *pstAtkerOL = &pstAtker->stOnline.stWarOL;
	WarFieldOL *pstAtkeeOL = &pstAtkee->stOnline.stWarOL;
	
	//同队伍
	if(player_team_same(pstEnv, pstAtker, pstAtkee))
	{
		return 1;
	}
	//同公会
	else if(pstZoneClan != NULL && pstZoneClan == player_get_clan(pstEnv, pstAtkee))
	{
		return 1;
	}
	//同阵营
	else if(pstAtkerOL->cCampIdx > 0 && pstAtkerOL->cCampIdx == pstAtkeeOL->cCampIdx)
	{
		return 1;
	}

	return 0;
}

int z_get_hit_player_relation(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee)
{
	int iAniRelation = 0;
	ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstAtker);
	WarFieldOL *pstAtkerOL = &pstAtker->stOnline.stWarOL;
	WarFieldOL *pstAtkeeOL = &pstAtkee->stOnline.stWarOL;

	if(pstAtker == pstAtkee)
	{
		return SKILL_TARGET_SELF;
	}
	
	if(z_player_hit_player_filter(pstEnv, pstAtker, pstAtkee))
	{
		iAniRelation |= SKILL_TARGET_ENEMY_PLAYER;
	}
	else
	{
		//同队伍
		if(player_team_same(pstEnv, pstAtker, pstAtkee))
		{
			iAniRelation |= (SKILL_TARGET_FRIEND_PLAYER | SKILL_TARGET_TEAM_PLAYER);
		}
		//同公会
		else if(pstZoneClan != NULL && pstZoneClan == player_get_clan(pstEnv, pstAtkee))
		{
			iAniRelation |= SKILL_TARGET_FRIEND_PLAYER;
		}
		//同阵营
		else if(pstAtkerOL->cCampIdx > 0 && pstAtkerOL->cCampIdx == pstAtkeeOL->cCampIdx)
		{
			iAniRelation |= SKILL_TARGET_FRIEND_PLAYER;
		}
		//中立
		else
		{
			iAniRelation |= SKILL_TARGET_MID_PLAYER;
		}
	}

	return iAniRelation;
}

int player_hit_convoy_mon_relation(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon)
{
	MapIndex *pstMapIndex;
	ZoneTeam *pstTeam = player_team_get(pstEnv, pstPlayer);
	
	//自己不可以打自己的单人镖车
	if(is_my_convoy_car(pstPlayer, pstMon) && is_single_convoy_car(pstMon))
	{
		return SKILL_TARGET_FRIEND_MON;
	}

	pstMapIndex = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if(NULL == pstMapIndex)
	{
		return SKILL_TARGET_FRIEND_MON;
	}

	if(pstMapIndex->stMapDef.MapLimit.PK == 0)
	{
		return SKILL_TARGET_FRIEND_MON;
	}

	switch(pstPlayer->stOnline.ePkMode)
	{
		case PK_MODE_NORMAL:
			return SKILL_TARGET_FRIEND_MON;
			break;
		case PK_MODE_TEAM:
			if(pstTeam && pstMon->stTeamInfo.ullTeamID == pstTeam->stTeamInfo.TeamID)
			{
				return SKILL_TARGET_FRIEND_MON;
			}
			break;
		//其他客户端来限制
		default:
			break;
	}
	
	return SKILL_TARGET_ENEMY_MON;
}

int z_player_hit_factory(Monster *pstFactory, Player *pstAtker)
{
	//MapIndex *pstMapIndex;

	if(!is_mon_factory(pstFactory))
	{
		return 0;
	}
	
	//pstMapIndex = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &pstFactory->stMap);
	//if(NULL == pstMapIndex)
	//{
	//	return 0;
	//}

	//if(pstMapIndex->stMapDef.MapLimit.PK == 0)
	//{
	//	return 0;
	//}

	if(pstFactory->stCommUnion.stFactory.ullClanGID != pstAtker->stRoleData.MiscInfo.ClanInfo.ClanId.GID)
	{
		return 1;
	}

	return 0;
}

int z_mon_hit_factory(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstFactory)
{
	if(!is_mon_factory(pstFactory))
	{
		return 0;
	}

	if(is_mon_machine(pstMon))
	{
		if(this_machine(pstEnv, pstMon)->stMachineInfo.OwnerType != MACHINE_OWNER_CLAN)
		{
			return 0;
		}
		
		if(pstFactory->stCommUnion.stFactory.ullClanGID != this_machine(pstEnv, pstMon)->stMachineInfo.OwnerID)
		{
			return 1;
		}
	}

	return 0;
}

/*
int player_get_hit_mon_relation(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon)
{
	int iAniRelation = 0;
	MONSTERDEF *pstMonDef = NULL;
	
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef )
	{
		return SKILL_TARGET_NONE;
	}
	
	//镖车
	if(is_convoy_car(pstMon))
	{
		iAniRelation |= player_hit_convoy_mon_relation(pstEnv, pstPlayer, pstMon);
	}
	//友好的怪物
	else if(pstMon->bSubType == MON_SUB_TYPE_HUMAN_FRIEND )
	{
		iAniRelation |= SKILL_TARGET_FRIEND_MON;
	}
	else if(pstMon->bSubType == MON_SUB_TYPE_HUMAN_BROTHER )
	{
		iAniRelation |= SKILL_TARGET_FRIEND_PLAYER;
	}
	else if(pstMon->bSubType == MON_SUB_TYPE_MACHINE)
	{
		if(z_machine_hit_player(pstEnv, pstMon, pstPlayer))
		{
			iAniRelation |= SKILL_TARGET_ENEMY_MON;
		}
		else
		{
			iAniRelation |= SKILL_TARGET_FRIEND_MON;
		}
	}
	//同阵营的应该不能打，也不能给增益的东西吧?
	else if(is_player_same_camp_mon(pstPlayer, pstMon))
	{
		iAniRelation |= SKILL_TARGET_FRIEND_MON;
	}
	else if(is_mon_factory(pstMon))
	{
		if(z_player_hit_factory(pstMon, pstPlayer))
		{
			iAniRelation |= SKILL_TARGET_ENEMY_MON;
		}
		else
		{
			iAniRelation |= SKILL_TARGET_FRIEND_MON;
		}
	}
	else if(pstMon->iOwnerID > 0 )
	{
		Player *pstOwner;
		pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iOwnerID);
		if (pstOwner)
		{ 
			if (pstPlayer == pstOwner)
			{
				iAniRelation |= SKILL_TARGET_SELF;
			}
			else
			{
				iAniRelation |= z_get_hit_player_relation(pstEnv, pstPlayer, pstOwner);
			}
		}
	}
	else if( pstMonDef->MiscType&MON_MISC_TYPE_NO_PRESS ||
			 pstMonDef->MiscType&MON_MISC_TYPE_NO_ATKEE)
	{
		iAniRelation |= SKILL_TARGET_FRIEND_MON;
	}
	else
	{
		iAniRelation |= SKILL_TARGET_ENEMY_MON;
	}

	return iAniRelation;
}*/

int player_get_hit_mon_relation(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon)
{
	MONSTERDEF *pstMonDef = NULL;
	ZONEMAPINST *pstMapInst;
	MapIndex *pstMapIndex;
	ZONEIDX stIdx;
	int iPkMode;

	if(!is_in_siege_city_fast(pstEnv))
	{
		if(is_city_build_mon(pstMon))
		{
			return SKILL_TARGET_NONE;
		}
	}
	else
	{
		if(is_city_build_mon(pstMon))
		{
			int iNum;
			CITYBUILDINFO *pstBuildInfo;
			ZoneClan *pstClan = player_get_clan(pstEnv, pstPlayer);
			
			pstBuildInfo = world_city_build_info_get(pstEnv, pstMon->iCityBuildDefID);
			if(pstBuildInfo && pstBuildInfo->Type == CITY_BUILD_TYPE_CONTROL)
			{
				iNum = world_city_buff_build_alive_num_get(pstEnv);
				if(iNum > 0)
				{
					return SKILL_TARGET_NONE;
				}
			}
			
			if(pstClan)
			{
				if(pstMon->iPKType == MON_PK_TYPE_CLAN &&
					pstMon->ullPKID == pstClan->stClan.ClanProf.GID)
				{
					return SKILL_TARGET_NONE;
				}
			}
		}
	}
	
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if(NULL == pstMonDef)
	{
		return SKILL_TARGET_NONE;
	}

	// 商运破烂镖车不能搞
	if (pstMon->cDartCarNotAtk == 1)
	{
		return SKILL_TARGET_FRIEND_MON;
	}
	

	//天生友好
	if(pstMon->bSubType == MON_SUB_TYPE_HUMAN_FRIEND )
	{
		return SKILL_TARGET_FRIEND_MON;
	}

	//人的朋友
	if(pstMon->bSubType == MON_SUB_TYPE_HUMAN_BROTHER )
	{
		return  SKILL_TARGET_FRIEND_PLAYER;
	}

	//特殊标记
	if(pstMonDef->MiscType & MON_MISC_TYPE_NO_PRESS ||
		pstMonDef->MiscType & MON_MISC_TYPE_NO_ATKEE)
	{
		return  SKILL_TARGET_FRIEND_MON;
	}

	if(pstMon->iPKType == MON_PK_TYPE_NONE && !is_city_build_mon(pstMon))
	{
		//NPC没有PK信息的不可以攻击
		if(pstMon->cType == OBJ_NPC)
		{
			return  SKILL_TARGET_NONE;
		}
		//普通怪物一直视为敌人
		else if( !is_city_build_mon(pstMon))
		{
			return  SKILL_TARGET_ENEMY_MON;
		}
	}

	if(pstMon->iPKType == MON_PK_TYPE_PROTECT)
	{
		return  SKILL_TARGET_FRIEND_MON;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return SKILL_TARGET_NONE;
	}

	//属于人的直接用人的信息检查
	if(pstMon->iPKType == MON_PK_TYPE_PLAYER)
	{
		int iRelation;
		Player *pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->ullPKID);
		if(NULL == pstOwner)
		{
			return SKILL_TARGET_ENEMY_MON;
		}

		if(is_mon_biaoche(pstMon))
		{
			pstOwner->stOnline.fRealBiaoChe = 1;
		}
		
		iRelation = z_get_hit_player_relation(pstEnv, pstPlayer, pstOwner);
		if(iRelation & SKILL_TARGET_ENEMY_PLAYER)
		{
			iRelation = SKILL_TARGET_ENEMY_MON;
		}
		else
		{
			iRelation = SKILL_TARGET_FRIEND_MON;
		}

		pstOwner->stOnline.fRealBiaoChe = 0;
		return iRelation;
	}

	pstMapIndex = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIndex || pstMapIndex->stMapDef.MapLimit.PK == 0)
	{
		return SKILL_TARGET_NONE;
	}

	if(pstMon->iPKType == MON_PK_TYPE_PANISH)
	{
		if(pstPlayer->stOnline.ePkMode == PK_MODE_ALL)
		{
			return  SKILL_TARGET_ENEMY_MON;
		}
		else
		{
			return  SKILL_TARGET_FRIEND_MON;
		}
	}

	iPkMode = pstPlayer->stOnline.ePkMode;
	if(pstMapIndex->stMapDef.MapLimit.PKMode > 0)
	{
		iPkMode = pstMapIndex->stMapDef.MapLimit.PKMode;
	}

	switch(iPkMode)
	{
		case PK_MODE_NORMAL:
			return SKILL_TARGET_FRIEND_MON;
			break;
		case PK_MODE_POLICE:
			if(is_in_pk_time_mon(pstEnv, pstMon) || is_bad_mon(pstEnv, pstMon))
			{
				return SKILL_TARGET_ENEMY_MON;
			}
			return SKILL_TARGET_FRIEND_MON;
			break;
		case PK_MODE_TEAM:
			if (pstPlayer->stRoleData.MiscInfo.TeamID == 0)
			{
				return SKILL_TARGET_ENEMY_MON;
			}

			if(pstMon->iPKType != MON_PK_TYPE_TEAM ||
				pstPlayer->stRoleData.MiscInfo.TeamID != pstMon->ullPKID)
			{
				return SKILL_TARGET_ENEMY_MON;
			}

			return SKILL_TARGET_FRIEND_MON;
			break;
		case PK_MODE_CLAN:
			{
				ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
				
				if(NULL == pstZoneClan)
				{
					return SKILL_TARGET_ENEMY_MON;
				}

				if(pstMon->iPKType != MON_PK_TYPE_CLAN ||
					pstZoneClan->stClan.ClanProf.GID != pstMon->ullPKID)
				{
					return SKILL_TARGET_ENEMY_MON;
				}
			}
			return SKILL_TARGET_FRIEND_MON;
			break;
		case PK_MODE_CAMP:
			if(pstPlayer->stOnline.stWarOL.cCampIdx==  0)
			{
				return SKILL_TARGET_ENEMY_MON;
			}

			if (pstMon->iPKType != MON_PK_TYPE_CAMP ||
				(int)pstPlayer->stOnline.stWarOL.cCampIdx != (int)pstMon->ullPKID)
			{
				return SKILL_TARGET_ENEMY_MON;
			}
			
			return SKILL_TARGET_FRIEND_MON;
			break;
		//自己全体模式
		case PK_MODE_ALL:
			return SKILL_TARGET_ENEMY_MON;
			break;
		case PK_MODE_ENEMY_CLAN:
			{
				ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
				
				if(NULL == pstZoneClan)
				{
					return SKILL_TARGET_ENEMY_MON;
				}

				if(pstMon->iPKType != MON_PK_TYPE_CLAN ||
					pstZoneClan->stClan.ClanProf.GID != pstMon->ullPKID)
				{
					return SKILL_TARGET_ENEMY_MON;
				}
			}
			break;
		default:
			return SKILL_TARGET_ENEMY_MON;
			break;
	}

	return SKILL_TARGET_ENEMY_MON;
}

int z_player_filter_relation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAni, SKILLDEF *pstSkillDef)
{
	int iRelation = 0;
	
	z_player_filter_atkobj(pstAppCtx, pstEnv, pstPlayer, pstAni, pstSkillDef,&iRelation);
    
	if( (iRelation & SKILL_TARGET_ENEMY_MON) ||
		(iRelation & SKILL_TARGET_ENEMY_PLAYER) || 
		(iRelation & SKILL_TARGET_ENEMYPET) )
	{
		return 1;
	}

	return 0;
}

//根据技能规则以及PK 规则等判定是否可攻击
int z_player_filter_atkobj(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAni, 
							SKILLDEF	 *pstSkillDef,int *piRelation)
{
	int iAniRelation = 0;
	UNUSED(pstAppCtx);

	//坐车中 就当自己不是人
	if(is_player_ride_on_machine(pstPlayer))
	{
		Monster *pstMachine = z_id_monster(pstAppCtx, pstEnv, pstPlayer->stOnline.stMachineInfo.iMachineMonID);
		if(NULL == pstMachine)
		{
			return -1;
		}

		return z_mon_filter_atkobj(pstAppCtx, pstEnv, pstMachine, pstAni, pstSkillDef,piRelation);
	}
	else if(pstPlayer->stOnline.iSkillAtkerID > 0)
	{
		Player *pstSkillPlayer = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.iSkillAtkerID);
		if(pstSkillPlayer && pstSkillPlayer != pstPlayer)
		{
			return z_player_filter_atkobj(pstAppCtx, pstEnv, pstSkillPlayer, pstAni, pstSkillDef, piRelation);
		}
	}
	
	switch (pstAni->iType)
	{
		case OBJ_PLAYER:
			if (pstPlayer == pstAni->stObj.pstPlayer)
			{
				iAniRelation |= SKILL_TARGET_SELF;
			}
			else
			{
				iAniRelation |= z_get_hit_player_relation(pstEnv, pstPlayer, pstAni->stObj.pstPlayer);
			}
			break;
		case OBJ_MONSTER:
		case OBJ_NPC:
			iAniRelation |= player_get_hit_mon_relation(pstEnv, pstPlayer, pstAni->stObj.pstMon);
			break;
		case OBJ_PET:
			if (pstPlayer == pstAni->stObj.stZonePet.pstOwner)
			{
				iAniRelation |= (SKILL_TARGET_SELFPET |SKILL_TARGET_FRIENDPET);
			}
			else
			{
				if(z_player_hit_player_filter(pstEnv, pstPlayer, pstAni->stObj.stZonePet.pstOwner))
				{
					iAniRelation |= SKILL_TARGET_ENEMYPET;
				}
				else if (is_skill_target_friend_player(pstEnv, pstPlayer, pstAni->stObj.stZonePet.pstOwner))
				{
					iAniRelation |= SKILL_TARGET_FRIENDPET;
				}
				else
				{
					iAniRelation |= SKILL_TARGET_MIDPET;
				}
			}
			break;
		/*case OBJ_NPC:
			if (	pstAni->stObj.pstNpc->stMon.bAtkType == ATK_TYPE_INITIATIVE &&
				pstPlayer->stRoleData.MiscInfo.PkInfo.PkMode == PK_MODE_ALL)
			{
				iAniRelation |= SKILL_TARGET_ENEMY_MON;
			}
			break;*/
		default:
			return -1;
			break;
	}

	if(piRelation)
	{
		*piRelation = iAniRelation;
	}
	if (pstSkillDef->TargetType & iAniRelation)
	{
		return 0;
	}
	else
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d targettype %d", 
					pstPlayer->stRoleData.Uin, pstSkillDef->SkillID, pstSkillDef->TargetType);
		return -1;
	}
}

int is_friend_mon(Monster *pstMon)
{
	if(pstMon->bSubType == MON_SUB_TYPE_HUMAN_FRIEND || pstMon->bSubType == MON_SUB_TYPE_HUMAN_BROTHER)
	{
		return 1;
	}
	
	return 0;
}


int is_owner_mon(Monster *pstMon)
{
	if(pstMon->iOwnerID > 0)
	{
		return 1;
	}
	
	return 0;
}

int is_camp_mon(Monster *pstMon)
{
	if(pstMon->iCampIdx > 0)
	{
		return 1;
	}

	return 0;
}

int is_player_same_camp_mon(Player *pstPlayer, Monster *pstMon)
{
	if(pstMon->iCampIdx > 0 && pstMon->iCampIdx == pstPlayer->stOnline.stWarOL.cCampIdx)
	{
		return 1;
	}

	return 0;
}

/*
int z_mon_hit_player_filter(ZONESVRENV * pstEnv, Monster *pstAtker, Player *pstAtkee)
{
	//同阵营
	if(is_player_same_camp_mon(pstAtkee, pstAtker))
	{
		return 0;
	}

	if( is_mon_machine(pstAtker) &&
		!z_machine_hit_player(pstEnv, pstAtker, pstAtkee))
	{
		return 0;
	}

	if(is_my_convoy_car(pstAtkee, pstAtker) ||is_friend_mon(pstAtker))
	{
		return 0;	
	}

	return 1;
}
*/

int z_mon_hit_player_filter(ZONESVRENV * pstEnv, Monster *pstMon, Player *pstPlayer)
{
	if(is_friend_mon(pstMon))
	{
		return 0;	
	}

	if(is_city_build_mon(pstMon))
	{
		if(!is_in_siege_city_fast(pstEnv))
		{
			return 0;
		}
	}
	
	switch(pstMon->iPKType)
	{
		case MON_PK_TYPE_NONE:
			return 1;
			break;
		case MON_PK_TYPE_PLAYER:
			//属于人的直接用人的信息检查
			{
				int iRelation;
				Player *pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->ullPKID);
				if(pstOwner)
				{
					iRelation = z_get_hit_player_relation(pstEnv, pstOwner, pstPlayer);
					if(iRelation & SKILL_TARGET_ENEMY_PLAYER)
					{
						return 1;
					}
				}
			}
			break;
		case MON_PK_TYPE_TEAM:
			if(pstPlayer->stRoleData.MiscInfo.TeamID != pstMon->ullPKID)
			{
				return 1;
			}
			break;
		case MON_PK_TYPE_CLAN:
			{
				ZoneClan *pstZoneClan = player_get_clan(pstEnv, pstPlayer);
				if (pstMon->cType == OBJ_MONSTER)
				{
					if(	NULL == pstZoneClan ||
						pstZoneClan->stClan.ClanProf.GID != pstMon->ullPKID)
					{
						return 1;
					}
				}
				else if (pstMon->cType == OBJ_NPC)
				{
					if ((NULL == pstZoneClan ||
						pstZoneClan->stClan.ClanProf.GID != pstMon->ullPKID) && 
						pstPlayer->stOnline.State & CS_STAT_FIGHT)
					{
						return 1;
					}
				}
			}
			break;
		case MON_PK_TYPE_CAMP:
			if((int)pstPlayer->stOnline.stWarOL.cCampIdx != (int)pstMon->ullPKID)
			{
				return 1;
			}
			break;
		case MON_PK_TYPE_PROTECT:
			return 0;
		case MON_PK_TYPE_PANISH:
			if (is_bad_palyer(pstPlayer))
			{
				return 1;
			}
			break;
		default:
			break;
	}

	return 0;
}

int is_player_buddy_mon(Monster *pstMon)
{
	if (is_friend_mon(pstMon))
	{
		return 1;
	}

	if(is_owner_mon(pstMon))
	{
		return 1;
	}

	if(is_convoy_car(pstMon))
	{
		return 1;
	}
	
	if(is_mon_machine(pstMon))
	{
		return 1;
	}

	return 0;
}

int z_machine_hit_player(ZONESVRENV* pstEnv, Monster *pstMachine, Player *pstAtkee)
{
	MapIndex *pstMapIndex;

	if(!is_mon_machine(pstMachine))
	{
		return 1;
	}
	
	pstMapIndex = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &pstMachine->stMap);
	if(NULL == pstMapIndex)
	{
		return 0;
	}

	if(pstMapIndex->stMapDef.MapLimit.PK == 0)
	{
		return 0;
	}

	switch(this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerType)
	{
		case MACHINE_OWNER_SELF:
			if(this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerID != (unsigned int)pstAtkee->iMemID)
			{
				return 1;
			}
			break;
		case MACHINE_OWNER_CLAN:
			if(this_machine(pstEnv,pstMachine)->stMachineInfo.OwnerID != pstAtkee->stRoleData.MiscInfo.ClanInfo.ClanId.GID)
			{
				return 1;
			}
			break;
		default:
			return 1;
			break;
	}

	return 0;
}

int z_machine_hit_machine(ZONESVRENV* pstEnv, Monster *pstAtker, Monster *pstAtkee)
{
	MapIndex *pstMapIndex;
	
	pstMapIndex = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &pstAtker->stMap);
	if(NULL == pstMapIndex)
	{
		return 0;
	}

	if(pstMapIndex->stMapDef.MapLimit.PK == 0)
	{
		return 0;
	}

	if(this_machine(pstEnv,pstAtkee)->iNoIdle == 0)
	{
		return 0;
	}

	if(this_machine(pstEnv,pstAtkee)->stMachineInfo.OwnerID == MACHINE_OWNER_NONE ||
		this_machine(pstEnv,pstAtkee)->stMachineInfo.OwnerID != this_machine(pstEnv,pstAtker)->stMachineInfo.OwnerID)
	{
		return 1;
	}

	return 0;
}

/*
int z_mon_hit_mon_filter(ZONESVRENV* pstEnv, Monster *pstAtker, Monster *pstAtkee)
{
	//不同阵营
	if(pstAtker->iCampIdx > 0)
	{
		if(pstAtkee->iCampIdx > 0 && pstAtker->iCampIdx != pstAtkee->iCampIdx)
		{
			return 1;
		}
	}
	
	//目标是人类的怪物
	if (is_player_buddy_mon(pstAtkee))
	{
		if(is_player_buddy_mon(pstAtker))
		{	
			if(is_mon_machine(pstAtkee) && is_mon_machine(pstAtker) &&
				z_machine_hit_machine(pstEnv, pstAtker, pstAtkee))
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if(is_mon_factory(pstAtkee))
		{
			return z_mon_hit_factory(pstEnv, pstAtker, pstAtkee);
		}
		else if(is_player_buddy_mon(pstAtker))
		{
			return 1;
		}
	}

	return 0;
}*/

int z_mon_hit_mon_relation(ZONESVRENV* pstEnv, Monster *pstAtker, Monster *pstAtkee)
{
	if(!is_in_siege_city_fast(pstEnv))
	{
		if(is_city_build_mon(pstAtker) ||is_city_build_mon(pstAtkee))
		{
			return SKILL_TARGET_NONE;
		}
	}

	// 商运破烂镖车不能搞
	if (pstAtkee->cDartCarNotAtk == 1 || pstAtkee->bSubType == MON_SUB_TYPE_LOST_TOTEM)
	{
		return SKILL_TARGET_FRIEND_MON;
	}

	if(!is_mon_machine(pstAtker))
	{
		//怪物机械可以打熔炉
		if(pstAtkee->bSubType == MON_SUB_TYPE_RONGLU)
		{
			return SKILL_TARGET_FRIEND_MON;
		}
	}

	if(pstAtkee->iPKType == MON_PK_TYPE_PLAYER)
	{
		int iRelation;
		Player *pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtkee->ullPKID);
		if(pstOwner)
		{		
			if(is_mon_biaoche(pstAtkee))
			{
				pstOwner->stOnline.fRealBiaoChe = 1;
			}
			
			if(1 == z_mon_hit_player_filter(pstEnv, pstAtker, pstOwner))
			{
				iRelation = SKILL_TARGET_ENEMY_MON;
			}
			else
			{
				iRelation = SKILL_TARGET_FRIEND_MON;
			}

			pstOwner->stOnline.fRealBiaoChe = 0;
			return iRelation;
		}
		else
		{
			return SKILL_TARGET_ENEMY_MON;
		}
	}

	switch(pstAtker->iPKType)
	{
		case MON_PK_TYPE_NONE:
			if(is_friend_mon(pstAtker))
			{
				if(!is_friend_mon(pstAtkee) && 
					pstAtkee->iPKType == MON_PK_TYPE_NONE)
				{
					return SKILL_TARGET_ENEMY_MON;
				}
			}
			else
			{
				if(is_friend_mon(pstAtkee) ||
					pstAtkee->iPKType != MON_PK_TYPE_NONE)
				{
					return SKILL_TARGET_ENEMY_MON;
				}
			}
			
			return SKILL_TARGET_FRIEND_MON;
			break;
		case MON_PK_TYPE_PLAYER:
			//属于人的直接用人的信息检查
			{
				int iRelation;
				Player *pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtker->ullPKID);
				if(pstOwner)
				{
					iRelation = player_get_hit_mon_relation(pstEnv, pstOwner, pstAtkee);
					if(iRelation & SKILL_TARGET_ENEMY_MON)
					{
						return SKILL_TARGET_ENEMY_MON;
					}
				}
			}
			break;
		case MON_PK_TYPE_TEAM:
		case MON_PK_TYPE_CAMP:
		case MON_PK_TYPE_CLAN:
			if(is_friend_mon(pstAtkee) || pstAtkee->iPKType == MON_PK_TYPE_PROTECT)
			{
				return SKILL_TARGET_FRIEND_MON;
			}
			if(pstAtker->iPKType != pstAtkee->iPKType ||
				pstAtker->ullPKID != pstAtkee->ullPKID )
			{
				return SKILL_TARGET_ENEMY_MON;
			}
			break;
		case MON_PK_TYPE_PROTECT:
			if(pstAtkee->iPKType == MON_PK_TYPE_NONE &&
				!is_friend_mon(pstAtkee))
			{
				return SKILL_TARGET_ENEMY_MON;
			}
		case MON_PK_TYPE_PANISH:
			{
				Player *pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtkee->iOwnerID);
				if (pstOwner && pstOwner->stRoleData.MiscInfo.PkInfo.PkVal >= PLAYER_PK_FRENZY)
				{
					return SKILL_TARGET_ENEMY_MON;
				}
			}
			break;
		default:
			break;
	}

	return SKILL_TARGET_FRIEND_MON;
}

int z_mon_filter_relation(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAni, SKILLDEF *pstSkillDef)
{
	int iRelation = 0;
	z_mon_filter_atkobj(pstAppCtx, pstEnv, pstMon, pstAni, pstSkillDef,&iRelation);
    
	if( (iRelation & SKILL_TARGET_ENEMY_MON) ||
		(iRelation & SKILL_TARGET_ENEMY_PLAYER) || 
		(iRelation & SKILL_TARGET_ENEMYPET) )
	{
		return 1;
	}

	return 0;
}
//根据技能规则判定是否可攻击
int z_mon_filter_atkobj(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, ZoneAni *pstAni, SKILLDEF *pstSkillDef,int *piRelation)
{
	int iAniRelation = 0;
	int fDecHP = 0;
	Player *pstPlayer;
	SKILLRESULT *pstSkillResult;
		
	/*
	if (pstMon->iOwnerID > 0)
	{
		Player *pstOwner;

		pstOwner = z_id_player(pstAppCtx, pstEnv, pstMon->iOwnerID);
		if (pstOwner)
		{
			MapIndex *pstMapIdx;
			ZONEIDX stIdx;

			stIdx.iID = pstOwner->stRoleData.Map;
			stIdx.iPos = pstOwner->stOnline.iMapPos;
			pstMapIdx = z_map_inst_index(pstAppCtx, pstEnv, &stIdx);
			if (NULL == pstMapIdx)
			{
				return -1;
			}
			
			return z_player_filter_atkobj(pstAppCtx, pstEnv, pstOwner, pstAni,  pstSkillDef,piRelation);
		}
	}*/

	if(pstMon->iSkillAtkerID > 0 && !is_mon_machine(pstMon))
	{
		Player *pstSkillPlayer = z_id_player(pstAppCtx, pstEnv, pstMon->iSkillAtkerID);
		if(pstSkillPlayer)
		{
			return z_player_filter_atkobj(pstAppCtx, pstEnv, pstSkillPlayer, pstAni, pstSkillDef, piRelation);
		}
	}
	
	switch (pstAni->iType)
	{
		case OBJ_PLAYER:
			pstPlayer = pstAni->stObj.pstPlayer;

			if(is_player_ride_on_machine(pstPlayer))
			{
				return -1;
			}
			
			if(z_mon_hit_player_filter(pstEnv, pstMon, pstPlayer))
			{
				iAniRelation |= SKILL_TARGET_ENEMY_PLAYER;		
			}
			else
			{
				iAniRelation |= SKILL_TARGET_FRIEND_PLAYER;			
			}

			if (pstPlayer->stRoleData.RoleDetail.HPCurr < pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax)
			{
				fDecHP = 1;
			}
			break;
		case OBJ_MONSTER:
			if(pstMon == pstAni->stObj.pstMon)
			{
				iAniRelation |= SKILL_TARGET_SELF;
			}
			else
			{
				iAniRelation |= z_mon_hit_mon_relation(pstEnv, pstMon, pstAni->stObj.pstMon);
			}
			
			if (pstAni->stObj.pstMon->iHP < pstAni->stObj.pstMon->iMaxHP)
			{
				fDecHP = 1;
			}
			break;
		case OBJ_NPC:
			if(pstMon->iID == pstAni->stObj.pstNpc->stMon.iID)
			{
				iAniRelation |= SKILL_TARGET_SELF;
			}
			else
			{
				iAniRelation |= z_mon_hit_mon_relation(pstEnv, pstMon, &pstAni->stObj.pstNpc->stMon);
			}
			
			if (pstAni->stObj.pstNpc->stMon.iHP < pstAni->stObj.pstNpc->stMon.iMaxHP)
			{
				fDecHP = 1;
			}
			break;
		case OBJ_PET:
			if(is_friend_mon(pstMon))
			{
				iAniRelation |= SKILL_TARGET_FRIENDPET;	
			}
			else
			{
				iAniRelation |= SKILL_TARGET_ENEMYPET;
			}

			if (pstAni->stObj.stZonePet.pstRolePet->HP < pstAni->stObj.stZonePet.pstRolePet->MaxHP)
			{
				fDecHP = 1;
			}
			break;
		default:
			return -1;
			break;
	}
	if(piRelation)
	{
		*piRelation = iAniRelation;
	}

	if (!(pstSkillDef->TargetType & iAniRelation))
	{
		return -1;
	}

	//加血技能效果
	pstSkillResult = z_skill_get_result(pstSkillDef, RESULT_HP_ADD);
	if (pstSkillResult && (pstSkillResult->SkillResultVal1 > 0 || pstSkillResult->SkillResultVal2 > 0))
	{
		if(!fDecHP)
		{
			return -1;
		}
	}
	
	return 0;
	UNUSED(pstAppCtx);	//add by paraunused.pl
}

BOOL  IsPointInRect (int x, int y, int ox, int oy, int xlen, int ylen)
{
	int x1 = x-ox;
	int y1 = y-oy;
	
	if( (x1>= -xlen/2) && (x1<= xlen/2) && (y1>= -ylen/2) && (y1<= ylen/2) ) return 1;
	else return 0;
}
//判断队伍成员有多少个这个正方型里
int z_check_teammember_in_rect(ZoneTeam *pstZoneTeam,int MapID,int ox, int oy, int xlen, int ylen)
{
	int i = 0;
	int iCount = 0;
	TEAMMEMBER *pstTeamMember = NULL;
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		if ((int)pstZoneTeam->stTeamInfo.TeamMember[i].MapID != MapID)
		{
			continue;
		}
		pstTeamMember = &pstZoneTeam->stTeamInfo.TeamMember[i];
		
		if(IsPointInRect(pstTeamMember->Pos.X, pstTeamMember->Pos.Y,  ox, oy,xlen, ylen))
		{
			iCount++;
		}
	}

	return iCount;
}
int z_in_range(RESPOS *pstAtkeePos, RESPOS *pstAtkerPos, RESPOS *pstHitPos, RESRANGE *pstRange, int iSkillDist, int iRadius, int iDir)
{
	int iFaceAngle;
	int x, y;

	if (pstRange->RangeType == SKILL_RANGE_NONE || pstRange->RangeType == SKILL_RANGE_SIGLE)
	{
		return -1;
	}

	switch(pstRange->RangeType)
	{
		case SKILL_RANGE_RECT:

			if(iSkillDist == 0)
			{
				x	=	pstRange->RangeVal1+ DIST_TORRENT + iRadius;
				y	=	pstRange->RangeVal2 + DIST_TORRENT + iRadius;
					
				if (iDir > 0)
				{
					iFaceAngle = iDir;
					
					if(iFaceAngle >= 360)
						iFaceAngle -= 360;

					if( IsPointInRectangle2(pstAtkeePos->X, pstAtkeePos->Y,  pstAtkerPos->X, pstAtkerPos->Y,
							y, x, iFaceAngle))
						return 0;
					else
						return -1;
				}
				else
				{
					if(IsPointInRect(pstAtkeePos->X, pstAtkeePos->Y,  pstHitPos->X, pstHitPos->Y,
							x, y))
						return 0;
					else
						return -1;
				}
			}
			else
			{
				if(iDir > 0)
				{
					iFaceAngle = iDir;

					if(iFaceAngle >= 360)
						iFaceAngle -= 360;
				}
				else
				{
					iFaceAngle	=	FaceAngleBetween(pstAtkerPos->X, pstAtkerPos->Y, pstHitPos->X, pstHitPos->Y);
				}

				if(pstAtkerPos->X == pstAtkeePos->X && pstAtkerPos->Y == pstAtkeePos->Y)
				{
					return 0;
				}
				
				x	=	pstRange->RangeVal1+ DIST_TORRENT + iRadius;
				y	=	pstRange->RangeVal2 + DIST_TORRENT + iRadius;
				
				if( IsPointInRectangle(pstAtkeePos->X, pstAtkeePos->Y,  pstAtkerPos->X, pstAtkerPos->Y,
							y, x, iFaceAngle))
					return 0;
				else
					return -1;
			}
			break;
		case SKILL_RANGE_CIRCLE:
			x 	=	pstRange->RangeVal1 + DIST_TORRENT + iRadius;
			if( IsPointInCircle(pstAtkeePos->X, pstAtkeePos->Y,  pstHitPos->X, pstHitPos->Y, x) )
				return 0;
			else
				return -1;
			break;
		case SKILL_RANGE_ARC:
			iFaceAngle	=	FaceAngleBetween(pstAtkerPos->X, pstAtkerPos->Y, pstHitPos->X, pstHitPos->Y);

			x	=	pstRange->RangeVal1+ DIST_TORRENT + iRadius;
			y	=	pstRange->RangeVal2;

			//暂时只是原地扇形
			if( IsPointInFan(pstAtkeePos->X, pstAtkeePos->Y,  pstAtkerPos->X, pstAtkerPos->Y,
						x, iFaceAngle, y) || IsPointInCircle(pstAtkeePos->X, pstAtkeePos->Y,  pstAtkerPos->X, pstAtkerPos->Y, 2*DIST_TORRENT))
				return 0;
			else
				return -1;
			break;
		case SKILL_RANGE_RING:
			x = pstRange->RangeVal1 + DIST_TORRENT + iRadius;
			y = pstRange->RangeVal2 - iRadius;
			if (y < 0)
			{
				y = 0;
			}

			if (( IsPointInCircle(pstAtkeePos->X, pstAtkeePos->Y,  pstHitPos->X, pstHitPos->Y, x) ) &&
				!( IsPointInCircle(pstAtkeePos->X, pstAtkeePos->Y,  pstHitPos->X, pstHitPos->Y, y) ))
			{
				return 0;
			}
			else 
			{
				return -1;
			}
			break;
		default:
			return -1;
			break;
	}

	return 0;
}

inline int z_find_dup_target(int *piTargets, int iNum, int iID)
{
	int i;

	for (i=0; i<iNum; i++)
	{
		if (piTargets[i] == iID)
		{
			return 1;
		}
	}
	
	return 0;
}


PetFightShiOnline *pet_get_shipet_by_xiedaipos(Player *pstPlayer, int iXiedaiPos)
{
	int i;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	
	for (i=0; i<pstPetOnline->bFightShiNum; i++)
	{
		if (pstPetOnline->astFightShi[i].iXieDaiPos == iXiedaiPos)
		{
			return &pstPetOnline->astFightShi[i];
		}
	}

	return NULL;
}

int is_in_skill_atkinfo_target(ZoneSkill *pstZoneSkill, int iTargetID)
{
	int i;

	for(i=0; i<pstZoneSkill->stAtkInfo.iTargetNum; i++)
	{
		if(pstZoneSkill->stAtkInfo.aiTargetIDs[i] == iTargetID)
		{
			return 1;
		}
	}

	return 0;
}

int z_player_atklist(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  SKILLDEF *pstSkillDef, 
						CSSKILLATK *pstHit, ZoneObjList *pstList, struct timeval *pstCltTime)
{
	int i;
	int iIndex;
	char cType;
	ZoneAni *pstAni;
	TMEMBLOCK *pstMemBlock;
	RESPOS stPos, stAtkPos, stRolePos;
	TMAPMASK *pstMask;
	MapIndex *pstMapIdx;
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;
	Player *pstOther;
	int iRadius = 0;
	int iSkillDist;
	int aiTargetIDs[MAX_SKILL_TARGET];    
	RESRANGE *pstMainRange = &pstSkillDef->MainRange;
	ZONEPWORLDINST *pstPInst;

	pstList->iObj = 0;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstPInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
	if (pstPInst && pstPInst->iHomeID > 0)
	{
		return 0;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if(pstSkillDef->AtkType > SKILL_ATK_TYPE_NORMAL)
	{
		int i;
		ZoneSkill *pstZoneSkill;
		
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstHit->SkillID);
		if (!pstZoneSkill)
		{
			return -1;
		}

		stAtkPos = pstZoneSkill->stAtkInfo.AtkPos;
		stRolePos = pstZoneSkill->stAtkInfo.RolePos;

		if(pstSkillDef->AtkType == SKILL_ATK_TYPE_TARGET)
		{
			for(i=0; i<pstHit->TargetNum; i++)
			{
				if(!is_in_skill_atkinfo_target(pstZoneSkill, pstHit->TargetIDs[i]))
				{
					return -1;
				}
			}
		}
	}
	else
	{
		//stRolePos = pstPlayer->stRoleData.Pos;
		//前面检查过了
		stRolePos = pstHit->RolePos;
		stAtkPos =pstHit->AtkPos;
	}
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr_bytime(pstAppCtx, pstEnv, pstPlayer, 0, pstCltTime, 10);
	}

	/* 前面检查了
	if (z_distance(&pstHit->RolePos, &stRolePos) > 3*DIST_TORRENT)
	{
		tlog_info(pstEnv->pstLogCat, 0, 0, "hit role pos (%d,%d), role pos (%d, %d)",
					pstHit->RolePos.X, pstHit->RolePos.Y, stRolePos.X,
					stRolePos.Y);

		pstHit->RolePos = stRolePos;
	}*/

	//检查打击点魔法阻挡
	if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstHit->RolePos.X, pstHit->RolePos.Y, stAtkPos.X, stAtkPos.Y))
	{
		return -1;
	}

	if (pstSkillDef->DeadTarget)
	{
		iSkillDist = pstSkillDef->Distance + 8 * DIST_TORRENT;
	}
	else if (pstSkillDef->MoveAttack)
	{
		iSkillDist = pstSkillDef->Distance + 4 * DIST_TORRENT;
	}
	else
	{
		iSkillDist = pstSkillDef->Distance + 4 * DIST_TORRENT;
	}

	if (pstMainRange->RangeType == SKILL_RANGE_NONE || pstMainRange->RangeType == SKILL_RANGE_SIGLE)
	{
		if (pstHit->TargetNum > 1)
		{
			tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player skill %d targetnum %d", pstPlayer->stRoleData.Uin,
						pstSkillDef->SkillID, pstHit->TargetNum);
			return -1;
		}
	}
	else
	{
		switch(pstSkillDef->TargetRange)
		{
			case TARGET_RANGE_SELF:
				if (z_distance(&stAtkPos, &stRolePos) > 3*DIST_TORRENT)
				{
					return -1;
				}
				break;
			case TARGET_RANGE_FRIEND:
				{
					Player *pstFriend;

					pstFriend = z_id_player(pstEnv->pstAppCtx, pstEnv, pstHit->TargetIDs[0]);
					if (NULL == pstFriend || pstFriend->eStatus != PLAYER_STATUS_ROLE_LOGIN)
					{
						return -1;
					}

					if (0 == player_team_same(pstEnv, pstPlayer, pstFriend))
					{
						return -1;
					}

					if (pstFriend->stOnline.cMoving)
					{
						z_player_move_curr(pstAppCtx, pstEnv, pstFriend, 0);
						//z_player_move_curr_bytime(pstAppCtx, pstEnv, pstFriend, 0, pstCltTime, 10);
					}

					if (z_distance(&stAtkPos, &pstFriend->stRoleData.Pos) > 8*DIST_TORRENT)
					{
						return -1;
					}
				}
				break;
			case TARGET_RANGE_NORMAL:
				{
					Monster *pstMon;
					int iTmpRadius;

					pstMon = z_id_monster(pstAppCtx,  pstEnv, pstHit->TargetIDs[0]);
					if (NULL == pstMon)
					{
						iTmpRadius = 0;
					}
					else
					{
						iTmpRadius = pstMon->unRadius;
					}

					if(z_distance(&stAtkPos, &stRolePos) > iSkillDist + iTmpRadius)
					{
						return -1;
					}
				}
				break;
			default:
				if(z_distance(&stAtkPos, &stRolePos) > iSkillDist)
				{
					return -1;
				}
				break;
		}
	}

	/*if (0 == pstHit->TargetNum)
	{
		return 0;
	}*/
	
	memset(aiTargetIDs, 0, sizeof(aiTargetIDs[0]) * pstHit->TargetNum);
	for (i=0; i<pstHit->TargetNum; i++)
	{
		if (pstList->iObj >= pstSkillDef->TargetNum)
		{
			break;
		}

		if (z_find_dup_target(aiTargetIDs, i, pstHit->TargetIDs[i]))
		{
			break;
		}
		aiTargetIDs[i] = pstHit->TargetIDs[i];
		if (aiTargetIDs[i] >= PET_ID_BASE)
		{
			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstHit->TargetIDs[i]%PET_ID_BASE, &cType);
		}
		else
		{
			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstHit->TargetIDs[i], &cType);
		}
			
		if (iIndex < 0) continue;

		iRadius = 0;
		pstAni = &pstList->astObj[pstList->iObj];
		pstAni->iType = cType;
		if (cType == OBJ_PLAYER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstOther =  (Player *)pstMemBlock->szData;
			if (aiTargetIDs[i] >= PET_ID_BASE)
			{
				pstAni->iType = OBJ_PET;
				pstAni->stObj.stZonePet.pstOwner = pstOther;
				pstAni->stObj.stZonePet.pstPetFightShi = pet_get_shipet_by_xiedaipos(pstOther, (aiTargetIDs[i]/PET_ID_BASE)-1);
				if (NULL == pstAni->stObj.stZonePet.pstPetFightShi)	
				{
					continue;
				}
				pstAni->stObj.stZonePet.pstRolePet = z_find_role_pet(pstOther, pstAni->stObj.stZonePet.pstPetFightShi->stRolePetOnline.ullWID, NULL);
				if (NULL == pstAni->stObj.stZonePet.pstRolePet)
				{
					continue;
				}

				if(is_pet_heti(pstOther))
				{
					continue;
				}
			}
			else
			{
				pstAni->stObj.pstPlayer = pstOther;
				if(is_player_on_ride(pstOther))
				{
					iRadius = 3*DIST_TORRENT;//乘骑给个半径范围
				}
			}

			if (PLAYER_STATUS_ROLE_LOGIN != pstOther->eStatus || 
				(0 == pstSkillDef->DeadTarget && ((pstOther->stOnline.State & CS_STAT_DEAD) || pstOther->stRoleData.RoleDetail.HPCurr == 0)) ||
				(pstOther->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN)))
			{
				continue;
			}

			if(is_player_ride_on_machine(pstOther))
			{
				continue;
			}

			if (0 > z_player_filter_atkobj(pstAppCtx, pstEnv, pstPlayer, pstAni,  pstSkillDef,NULL))
			{
				continue;
			}

			if (pstOther == pstPlayer)
			{
				pstList->iObj++;
				continue;
			}

			if (pstOther->stRoleData.Map != pstPlayer->stRoleData.Map)
			{
				/*tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "atkee Map %d, atker Map %d", 
							pstOther->stRoleData.Map, pstPlayer->stRoleData.Map );*/
				continue;
			}

			if (pstOther->stOnline.cMoving)
			{
				z_player_move_curr(pstAppCtx, pstEnv, pstOther, 0);
				//z_player_move_curr_bytime(pstAppCtx, pstEnv, pstOther, 0, pstCltTime, 10);
			}

			stPos.X = pstOther->stRoleData.Pos.X;
			stPos.Y = pstOther->stRoleData.Pos.Y;
			
			if (aiTargetIDs[i] >= PET_ID_BASE)
			{
				//宠物的话，坐标可取AtkPos
				if (z_distance(&pstHit->AtkPos, &stPos) < 10*DIST_TORRENT)
				{
					stPos = pstHit->AtkPos;
				}
			}
		}
		else if (cType == OBJ_MONSTER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
			pstAni->stObj.pstMon = (Monster *)pstMemBlock->szData;
			if (AI_STAT_HOME == pstAni->stObj.pstMon->bAiStat ||
				pstAni->stObj.pstMon->bSubType >= MON_SUB_TYPE_TRAP ||
				(0 == pstSkillDef->DeadTarget  && (pstAni->stObj.pstMon->cStat == MONSTER_DIE || pstAni->stObj.pstMon->iHP <= 0)) ||
				    pstAppCtx->stCurr.tv_sec < pstAni->stObj.pstMon->tBirthEnd)
			{
				continue;
			}
			
			if (0 > z_player_filter_atkobj(pstAppCtx, pstEnv, pstPlayer, pstAni,  pstSkillDef,NULL))
			{
				continue;
			}
			
			if (pstAni->stObj.pstMon->stMap.iID != pstPlayer->stRoleData.Map)
			{
				/*tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "atkee Map %d, atker Map %d", 
							pstAni->stObj.pstMon->stMap.iID, pstPlayer->stRoleData.Map );*/
				continue;
			}

			if (pstAni->stObj.pstMon->cMoving)
			{
				//z_monster_move_curr(pstAppCtx, pstEnv, pstAni->stObj.pstMon, 0);
				z_monster_move_bytime(pstAppCtx, pstEnv, pstAni->stObj.pstMon, pstCltTime);
			}

			iRadius = pstAni->stObj.pstMon->unRadius;

			stPos.X = pstAni->stObj.pstMon->stCurrPos.X;
			stPos.Y = pstAni->stObj.pstMon->stCurrPos.Y;
		}
		else if (cType == OBJ_NPC)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}

			pstAni->stObj.pstNpc = (Npc*)pstMemBlock->szData;

			/*if (pstAni->stObj.pstNpc->stMon.bAtkType != ATK_TYPE_INITIATIVE)
			{
				continue;
			}

			if (pstPlayer->stRoleData.MiscInfo.PkInfo.PkMode != PK_MODE_ALL)
			{
				continue;
			}*/

			if (0 > z_player_filter_atkobj(pstAppCtx, pstEnv, pstPlayer, pstAni,  pstSkillDef,NULL))
			{
				continue;
			}
			
			if (pstAni->stObj.pstNpc->stMon.stMap.iID != pstPlayer->stRoleData.Map)
			{
				/*tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "atkee Map %d, atker Map %d", 
							pstAni->stObj.pstNpc->stMon.stMap.iID, pstPlayer->stRoleData.Map );*/
				continue;
			}

			iRadius = pstAni->stObj.pstNpc->stMon.unRadius;

			stPos.X = pstAni->stObj.pstNpc->stMon.stCurrPos.X;
			stPos.Y = pstAni->stObj.pstNpc->stMon.stCurrPos.Y;
		}
		else
		{
			continue;
		}

		//位置检查
		if (pstMainRange->RangeType == SKILL_RANGE_NONE || pstMainRange->RangeType == SKILL_RANGE_SIGLE) //单体
		{
			int iTorrent;

			if (z_skill_get_result(pstSkillDef, RESULT_SELF_POSCHG))
			{
				iTorrent = 10*DIST_TORRENT;
			}
			else
			{
				iTorrent = 6*DIST_TORRENT;
			}
			
			if (z_distance(&stAtkPos, &stPos) > iTorrent)
			{
				tlog_debug(pstEnv->pstLogCat, 0, 0, "hit atk pos (%d,%d), atkee pos (%d, %d)",
					stAtkPos.X, stAtkPos.Y, stPos.X, stPos.Y);

				pstHit->AtkPos = stPos;

				if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, stRolePos.X, stRolePos.Y, stPos.X, stPos.Y))
				{
					continue;
				}
			}
			
			if (z_distance(&stPos, &stRolePos)  > (iSkillDist + iRadius))
			{
				continue;
			}
		}
		else
		{
			if (0 > z_in_range(&stPos, &stRolePos,&stAtkPos, pstMainRange, pstSkillDef->Distance, iRadius, 0))
			{
				continue;
			}
			
			if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, stAtkPos.X, stAtkPos.Y, stPos.X, stPos.Y))
			{
				continue;
			}
		}

		pstList->iObj++;
	}

	if( pstSkillDef->SkillType == SKILL_TYPE_ATKPOS_CALL )
	{
		if (pstList->iObj >= pstSkillDef->TargetNum)
		{
			return 0;
		}
		pstAni = &pstList->astObj[pstList->iObj];
		pstAni->iType = OBJ_PLAYER;
		pstAni->stObj.pstPlayer = pstPlayer;
		pstList->iObj++;
	}
	return 0;
}

int z_hitrate_get(AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo)
{
	int iHit, iFlee, iHitRate;
	DYNATTRLIST *pstAtkeeDyn = &pstAtkeeInfo->stDyn;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;

	//攻击方的命中
	iHit = pstAtkerDyn->stHitChg.unHitBaseAdd*(1+pstAtkerDyn->stHitChg.nHitChgMul/100.0) 
			+ pstAtkerDyn->stHitChg.iHitChgAdd;

	//防御方的闪避
	iFlee = pstAtkeeDyn->stFleeChg.unFleeBaseAdd*(1+pstAtkeeDyn->stFleeChg.nFleeChgMul/100.0) + 
			pstAtkeeDyn->stFleeChg.iFleeChgAdd;

	//命中初值
	if (iFlee > 0)
	{
		//命中率=0.6+攻击者命中等级*0.3/被击者闪避等级
		iHitRate = 60 + 0.3*(iHit*100.0/iFlee);
	}
	else
	{
		iHitRate = 100;
	}

	iHitRate = iHitRate * 
				(1+pstAtkerDyn->stHitRateChg.nAtkerHitRateMul/100.0-pstAtkeeDyn->stHitRateChg.nAtkeeHitRateMul/100.0) + 
				(pstAtkerDyn->stHitRateChg.nAtkerHitRateAdd - pstAtkeeDyn->stHitRateChg.nAtkeeHitRateAdd);

	iHitRate = iHitRate*100;
	
	if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER && 
		pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
	{
		if(pstAtkerInfo->stAtker.stObj.pstPlayer->stRoleData.Gender ==
			pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.Gender)
		{
			if(pstAtkerDyn->stHitRateChg.iTongXingHitRate > 0)
			{
				iHitRate += pstAtkerDyn->stHitRateChg.iTongXingHitRate;
			}
		}
		else
		{
			if(pstAtkerDyn->stHitRateChg.iYiXingHitRate > 0)
			{
				iHitRate += pstAtkerDyn->stHitRateChg.iYiXingHitRate;
			}
		}
	}

	return iHitRate;
}

int z_miss_check(AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo)
{
	int iHitRate;

	iHitRate = z_hitrate_get(pstAtkerInfo, pstAtkeeInfo);
	if ((int)(RAND1(10000))>=iHitRate)
	{
		return -1;
	}

	return 0;
}

void z_get_hit_mon_subtypemul(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef, Monster *pstAtkee,  int *piAtkMul)
{

	MONSTERDEF *pstMonsterDef;
	
	if (!pstSkillDef ||!pstAtkee || !piAtkMul)
	{
		return;
	}
	
	pstMonsterDef = z_get_mon_def(pstEnv, pstAtkee);
	if (pstMonsterDef)
	{
		switch(pstMonsterDef->SubType)
			{
				case MON_SUB_TYPE_MINI:
					
					*piAtkMul = pstSkillDef->MonMiniMul;
					break;
				case MON_SUB_TYPE_WEAK:
					
					*piAtkMul = pstSkillDef->MonWeakMul;
					break;
				case MON_SUB_TYPE_SOFT:

					*piAtkMul = pstSkillDef->MonSoftMul;
					break;
				case MON_SUB_TYPE_NORM:

					*piAtkMul = pstSkillDef->MonNormalMul;
					break;
				case MON_SUB_TYPE_HARD:

					*piAtkMul = pstSkillDef->MonHardMul;
					break;
				case MON_SUB_TYPE_XI:

					*piAtkMul = pstSkillDef->MonXiMul;
					break;
				case MON_SUB_TYPE_JIN:

					*piAtkMul = pstSkillDef->MonJinMul;
					break;
				case MON_SUB_TYPE_BOSS:

					*piAtkMul = pstSkillDef->MonBossMul;
					break;
				case MON_SUB_TYPE_JI:

					*piAtkMul = pstSkillDef->MonJiMul;
					break;
				default:
					break;
			}
	}
	
	return;
}
// 计算积累数全部使用的效果
int z_calculate_collectskill_useall_effect(ZONESVRENV* pstEnv,
                                     AtkProcess *pstAtkProcess, SKILLDEF *pstSkillDef)
{
	Player *pstPlayer = NULL;
	SKILLRESULT *pstSkillResult = NULL;
	int iHurt = 0;
	
	UNUSED(pstEnv);
	
	if(pstAtkProcess->stAtkerInfo.stAtker.iType != OBJ_PLAYER)
	{
		return 0;
	}
	pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;
	if(!pstPlayer)
	{
		return 0;
	}
	
	// 把积累数技能的多余数值转换成伤害
	pstSkillResult =z_skill_get_result(pstSkillDef, RESULT_COLLECTSKILL_USEALL);
	if( pstSkillResult)
	{
		COLLECTSKILLINFO *pstCollectSkillInfo = NULL;
		pstCollectSkillInfo = &pstPlayer->stRoleData.MiscInfo.CollectSkillInfo;
		if( pstCollectSkillInfo->CollectSkill_SlotCurr > pstSkillDef->CollectSkillUse)
		{
			iHurt = (pstCollectSkillInfo->CollectSkill_SlotCurr-pstSkillDef->CollectSkillUse) * (pstSkillResult->SkillResultVal1/100.0);
		}
	}
		
	return iHurt;
}

/* 计算集气效果
X （1 + z * y/t）  
X，技能攻击力
Z，集气可提升攻击力倍数
y，当前集气时间
t，集气最大时间
*/

int z_calculate_jiqi_power_effect(ZONESVRENV* pstEnv,
                                     AtkProcess *pstAtkProcess, int *piRet)
{
	Player *pstPlayer = NULL;
 	AtkInfo *pstAtkInfo = NULL;

	UNUSED(pstEnv);
	if(pstAtkProcess->stAtkerInfo.stAtker.iType != OBJ_PLAYER)
	{
		return 0;
	}

	pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;
	if(!pstPlayer)
	{
		return -1;
	}

	pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
 
	if(pstAtkProcess->stAtkerInfo.pstSkillDef->JiqiTime)
	{
		//公式X（1 + z * y/t）
		*piRet = (*piRet) * (1 + pstAtkProcess->stAtkerInfo.pstSkillDef->JiqiMul * 
			(pstAtkInfo->iJiqiTime/(pstAtkProcess->stAtkerInfo.pstSkillDef->JiqiTime*100.0)));
	}
	
	return 0;
}

int get_skill_hurt(SKILLARMCHG *pstSkillArmChg, SKILLDEF *pstSkillDef, int iSkillAttrType, int *piSkillHurt, int *piHurtMul)
{
	int iHurtMin = 0;
	int iHurtMax = 0;
	int iHurtMul = 0;
	int iHurtMulMax = 0;

	switch(iSkillAttrType)
	{
		case SKILL_ATTR_ICE:
			iHurtMin = pstSkillDef->IceHurtMin;
			iHurtMax = pstSkillDef->IceHurtMax;
			iHurtMul = pstSkillDef->IceHurtMul;
			iHurtMulMax = pstSkillDef->IceHurtMulMax;
			break;
		case SKILL_ATTR_PHSIC:
			iHurtMin = pstSkillDef->PhsicHurtMin;
			iHurtMax = pstSkillDef->PhsicHurtMax;
			iHurtMul = pstSkillDef->PhsicHurtMul;
			iHurtMulMax = pstSkillDef->PhsicHurtMulMax;
			break;
		case SKILL_ATTR_SIEGE:
			iHurtMin = pstSkillDef->SiegeHurtMin;
			iHurtMax = pstSkillDef->SiegeHurtMax;
			iHurtMul = pstSkillDef->SiegeHurtMul;
			iHurtMulMax = pstSkillDef->SiegeHurtMulMax;
			break;
		case SKILL_ATTR_HOLY:
			iHurtMin = pstSkillDef->HolyHurtMin;
			iHurtMax = pstSkillDef->HolyHurtMax;
			iHurtMul = pstSkillDef->HolyHurtMul;
			iHurtMulMax = pstSkillDef->HolyHurtMulMax;
			break;
		case SKILL_ATTR_MACHINE:
			iHurtMin = pstSkillDef->MachineHurtMin;
			iHurtMax = pstSkillDef->MachineHurtMax;
			iHurtMul = pstSkillDef->MachineHurtMul;
			iHurtMulMax = pstSkillDef->MachineHurtMulMax;
			break;
		case SKILL_ATTR_FIRE:
			iHurtMin = pstSkillDef->FireHurtMin;
			iHurtMax = pstSkillDef->FireHurtMax;
			iHurtMul = pstSkillDef->FireHurtMul;
			iHurtMulMax = pstSkillDef->FireHurtMulMax;
			break;
		case SKILL_ATTR_THUNDER:
			iHurtMin = pstSkillDef->ThunderHurtMin;
			iHurtMax = pstSkillDef->ThunderHurtMax;
			iHurtMul = pstSkillDef->ThunderHurtMul;
			iHurtMulMax = pstSkillDef->ThunderHurtMulMax;
			break;
		case SKILL_ATTR_LIGHT:
			iHurtMin = pstSkillDef->LightHurtMin;
			iHurtMax = pstSkillDef->LightHurtMax;
			iHurtMul = pstSkillDef->LightHurtMul;
			iHurtMulMax = pstSkillDef->LightHurtMulMax;
			break;
		case SKILL_ATTR_NIGHT:
			iHurtMin = pstSkillDef->NightHurtMin;
			iHurtMax = pstSkillDef->NightHurtMax;
			iHurtMul = pstSkillDef->NightHurtMul;
			iHurtMulMax = pstSkillDef->NightHurtMulMax;
			break;
		case SKILL_ATTR_WATER:
			iHurtMin = pstSkillDef->WaterHurtMin;
			iHurtMax = pstSkillDef->WaterHurtMax;
			iHurtMul = pstSkillDef->WaterHurtMul;
			iHurtMulMax = pstSkillDef->WaterHurtMulMax;
			break;
		default:
			return 0;
			break;
	}

	//主属性才有可能用到武器修正
	if (pstSkillArmChg && iSkillAttrType == pstSkillDef->MainAttr)	
	{
		if (pstSkillArmChg->HurtMin)      iHurtMin = pstSkillArmChg->HurtMin;
		if (pstSkillArmChg->HurtMax)	  iHurtMax = pstSkillArmChg->HurtMax;
		if (pstSkillArmChg->HurtMul)      iHurtMul = pstSkillArmChg->HurtMul;
		if (pstSkillArmChg->HurtMulMax) iHurtMulMax = pstSkillArmChg->HurtMulMax;
	}

	if(iHurtMulMax > iHurtMul)
	{
		iHurtMul = iHurtMul + RAND1(iHurtMulMax-iHurtMul+1);
	}

	

	*piHurtMul = iHurtMul;
	
	if (iHurtMax >= iHurtMin)
	{
		*piSkillHurt = iHurtMin + RAND1(iHurtMax-iHurtMin+1);
	}
	else
	{
		*piSkillHurt = 0;
	}

	return 0;
}

int z_get_ignore_def(Player *pstPlayer, int iSkillAttrType)
{
	DEFCHG *pstDefChg = &pstPlayer->stOnline.stDyn.stDefChg;
	int iIgnoreDef = 0;
	
	switch(iSkillAttrType)
	{
		case SKILL_ATTR_PHSIC:
		case SKILL_ATTR_ICE:
			iIgnoreDef = pstDefChg->iIgnoreDef;
			break;
		default:
			return 0;
			break;
	}

	if(iIgnoreDef > 0)
	{
		iIgnoreDef =  iIgnoreDef *10000.0/(iIgnoreDef*2+ pstPlayer->stRoleData.Level*pstPlayer->stRoleData.Level/1.5);
	}
	
	return iIgnoreDef;
}

int z_get_dec_def(ATKCHG *pstAtkChg, int iSkillAttrType,int *piDecDef,int *piDecDefMul)
{
	int iDecDef = 0;
	
	switch(iSkillAttrType)
	{
		case SKILL_ATTR_ICE:
			*piDecDef = pstAtkChg->iIceDecDef;
			*piDecDefMul = pstAtkChg->iIceDecDefMul;
			break;
		case SKILL_ATTR_PHSIC:
			*piDecDef = pstAtkChg->iPhsicDecDef;
			*piDecDefMul = pstAtkChg->iPhsicDecDefMul;
			break;
		case SKILL_ATTR_SIEGE:
			*piDecDef = pstAtkChg->iSiegeDecDef;
			break;
		case SKILL_ATTR_HOLY:
			*piDecDef = pstAtkChg->iHolyDecDef;
			break;
		case SKILL_ATTR_MACHINE:
			*piDecDef = pstAtkChg->iMachineDecDef;
			break;

		case SKILL_ATTR_FIRE:
			*piDecDef = pstAtkChg->iFireDecDef;
			break;
		case SKILL_ATTR_THUNDER:
			*piDecDef = pstAtkChg->iThunderDecDef;
			break;
		case SKILL_ATTR_LIGHT:
			*piDecDef = pstAtkChg->iLightDecDef;
			break;
		case SKILL_ATTR_NIGHT:
			*piDecDef = pstAtkChg->iNightDecDef;
			break;
		case SKILL_ATTR_WATER:
			*piDecDef = pstAtkChg->iWaterDecDef;
			break;
		default:
			return 0;
			break;
	}

	return iDecDef;
}

int get_atker_hurtmul(ATKERHURTCHG *pstAtkerHurtChg, int iSkillAttrType, int *piHurtMul, int *piHurtAdd)
{
	*piHurtMul = 0;
	*piHurtAdd = 0;

	if(NULL == pstAtkerHurtChg)
	{
		return 0;
	}
	
	switch(iSkillAttrType)
	{
		case SKILL_ATTR_ICE:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;
		case SKILL_ATTR_PHSIC:
			*piHurtMul = pstAtkerHurtChg->AtkerPhsicHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerPhsicHurtAdd;
			break;
		case SKILL_ATTR_SIEGE:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;
		case SKILL_ATTR_HOLY:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;
		case SKILL_ATTR_MACHINE:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;

		case SKILL_ATTR_FIRE:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;
		case SKILL_ATTR_THUNDER:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;
		case SKILL_ATTR_LIGHT:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;
		case SKILL_ATTR_NIGHT:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;
		case SKILL_ATTR_WATER:
			*piHurtMul = pstAtkerHurtChg->AtkerIceHurtMul;
			*piHurtAdd =  pstAtkerHurtChg->AtkerIceHurtAdd;
			break;
		
		default:
			return 0;
			break;
	}

	return 0;
}

//被击者的伤害修正
int get_atkee_hurtmul(ATKEEHURTCHG *pstAtkeeHurtChg, int iSkillAttrType, int *piHurtMul, int *piHurtAdd)
{
	*piHurtMul = 0;
	*piHurtAdd = 0;

	if(NULL == pstAtkeeHurtChg)
	{
		return 0;
	}
	
	switch(iSkillAttrType)
	{
		//主属性
		case SKILL_ATTR_ICE:
			*piHurtMul = pstAtkeeHurtChg->AtkeeIceHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeIceHurtAdd;
			break;
		case SKILL_ATTR_PHSIC:
			*piHurtMul = pstAtkeeHurtChg->AtkeePhsicHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeePhsicHurtAdd;
			break;
		case SKILL_ATTR_SIEGE:
			*piHurtMul = pstAtkeeHurtChg->AtkeeSiegeHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeSiegeHurtAdd;
			break;
		case SKILL_ATTR_HOLY:
			*piHurtMul = pstAtkeeHurtChg->AtkeeHolyHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeHolyHurtAdd;
			break;
		case SKILL_ATTR_MACHINE:
			*piHurtMul = pstAtkeeHurtChg->AtkeeMachineHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeMachineHurtAdd;
			break;

		//元素
		case SKILL_ATTR_FIRE:
			*piHurtMul = pstAtkeeHurtChg->AtkeeFireHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeFireHurtAdd;
			break;
		case SKILL_ATTR_THUNDER:
			*piHurtMul = pstAtkeeHurtChg->AtkeeThunderHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeThunderHurtAdd;
			break;
		case SKILL_ATTR_LIGHT:
			*piHurtMul = pstAtkeeHurtChg->AtkeeLightHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeLightHurtAdd;
			break;
		case SKILL_ATTR_NIGHT:
			*piHurtMul = pstAtkeeHurtChg->AtkeeNightHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeNightHurtAdd;
			break;
		case SKILL_ATTR_WATER:
			*piHurtMul = pstAtkeeHurtChg->AtkeeWaterHurtMul;
			*piHurtAdd =  pstAtkeeHurtChg->AtkeeWaterHurtAdd;
			break;
			
		default:
			return 0;
			break;
	}

	return 0;
}

int z_cacl_attr_atk(ATKCHG *pstAtkChg, int iSkillAttrType)
{
	int iAtk = 0;
	
	switch(iSkillAttrType)
	{
		//主属性
		case SKILL_ATTR_ICE:
			{
				int iMinAtk, iMaxAtk;
				iMinAtk = (pstAtkChg->unIceBaseArmAtk+pstAtkChg->iIceMinAtk)*(1+pstAtkChg->nIceAtkChgMul/100.0)+pstAtkChg->iIceAtkChgAdd;
				iMaxAtk = (pstAtkChg->unIceBaseArmAtk+pstAtkChg->iIceMaxAtk)*(1+pstAtkChg->nIceAtkChgMul/100.0)+pstAtkChg->iIceAtkChgAdd;

				if(iMaxAtk > iMinAtk)
				{
					iAtk = RAND1(iMaxAtk-iMinAtk+1)+iMinAtk;
				}
				else
				{
					iAtk = iMinAtk;
				}
			}
			break;
		case SKILL_ATTR_PHSIC:
			{
				int iMinAtk, iMaxAtk;
				iMinAtk = (pstAtkChg->unPhsicBaseArmAtk+pstAtkChg->iPhsicMinAtk)*(1+pstAtkChg->nPhsicAtkChgMul/100.0)+pstAtkChg->iPhsicAtkChgAdd;
				iMaxAtk = (pstAtkChg->unPhsicBaseArmAtk+pstAtkChg->iPhsicMaxAtk)*(1+pstAtkChg->nPhsicAtkChgMul/100.0)+pstAtkChg->iPhsicAtkChgAdd;

				if(iMaxAtk > iMinAtk)
				{
					iAtk = RAND1(iMaxAtk-iMinAtk+1)+iMinAtk;
				}
				else
				{
					iAtk = iMinAtk;
				}
			}
			break;
		case SKILL_ATTR_SIEGE:
			iAtk = (pstAtkChg->unSiegeBaseArmAtk*(1+pstAtkChg->nSiegeAtkChgMul/100.0)+pstAtkChg->iSiegeAtkChgAdd);
			break;
		case SKILL_ATTR_HOLY:
			iAtk = (pstAtkChg->unHolyBaseArmAtk*(1+pstAtkChg->nHolyAtkChgMul/100.0)+pstAtkChg->iHolyAtkChgAdd);
			break;
		case SKILL_ATTR_MACHINE:
			iAtk = (pstAtkChg->unMachineBaseArmAtk*(1+pstAtkChg->nMachineAtkChgMul/100.0)+pstAtkChg->iMachineAtkChgAdd);
			break;

		//元素
		case SKILL_ATTR_FIRE:
			iAtk = (pstAtkChg->unFireBaseArmAtk*(1+pstAtkChg->nFireAtkChgMul/100.0)+pstAtkChg->iFireAtkChgAdd);
			break;
		case SKILL_ATTR_THUNDER:
			iAtk = (pstAtkChg->unThunderBaseArmAtk*(1+pstAtkChg->nThunderAtkChgMul/100.0)+pstAtkChg->iThunderAtkChgAdd);
			break;
		case SKILL_ATTR_LIGHT:
			iAtk = (pstAtkChg->unLightBaseArmAtk*(1+pstAtkChg->nLightAtkChgMul/100.0)+pstAtkChg->iLightAtkChgAdd);
			break;
		case SKILL_ATTR_NIGHT:
			iAtk = (pstAtkChg->unNightBaseArmAtk*(1+pstAtkChg->nNightAtkChgMul/100.0)+pstAtkChg->iNightAtkChgAdd);
			break;
		case SKILL_ATTR_WATER:
			iAtk = (pstAtkChg->unWaterBaseArmAtk*(1+pstAtkChg->nWaterAtkChgMul/100.0)+pstAtkChg->iWaterAtkChgAdd);
			break;
			
		default:
			return 0;
			break;
	}

	return iAtk;
}

int z_cacl_attr_def(DEFCHG *pstDefChg, int iSkillAttrType)
{
	int iDef = 0;
	
	switch(iSkillAttrType)
	{
		case SKILL_ATTR_ICE:
			iDef = pstDefChg->unIceDefBaseAdd*(1+pstDefChg->nIceDefChgMul/100.0)+pstDefChg->iIceDefChgAdd;
			break;
		case SKILL_ATTR_PHSIC:
			iDef = pstDefChg->unPhsicDefBaseAdd*(1+pstDefChg->nPhsicDefChgMul/100.0)+pstDefChg->iPhsicDefChgAdd;
			break;
		case SKILL_ATTR_SIEGE:
			iDef = pstDefChg->unSiegeDefBaseAdd*(1+pstDefChg->nSiegeDefChgMul/100.0)+pstDefChg->iSiegeDefChgAdd;
			break;
		case SKILL_ATTR_HOLY:
			iDef = pstDefChg->unHolyDefBaseAdd*(1+pstDefChg->nHolyDefChgMul/100.0)+pstDefChg->iHolyDefChgAdd;
			break;
		case SKILL_ATTR_MACHINE:
			iDef = pstDefChg->unMachineDefBaseAdd*(1+pstDefChg->nMachineDefChgMul/100.0)+pstDefChg->iMachineDefChgAdd;
			break;

		case SKILL_ATTR_FIRE:
			iDef = pstDefChg->unFireDefBaseAdd*(1+pstDefChg->nFireDefChgMul/100.0)+pstDefChg->iFireDefChgAdd;
			break;
		case SKILL_ATTR_THUNDER:
			iDef = pstDefChg->unThunderDefBaseAdd*(1+pstDefChg->nThunderDefChgMul/100.0)+pstDefChg->iThunderDefChgAdd;
			break;
		case SKILL_ATTR_LIGHT:
			iDef = pstDefChg->unLightDefBaseAdd*(1+pstDefChg->nLightDefChgMul/100.0)+pstDefChg->iLightDefChgAdd;
			break;
		case SKILL_ATTR_NIGHT:
			iDef = pstDefChg->unNightDefBaseAdd*(1+pstDefChg->nNightDefChgMul/100.0)+pstDefChg->iNightDefChgAdd;
			break;
		case SKILL_ATTR_WATER:
			iDef = pstDefChg->unWaterDefBaseAdd*(1+pstDefChg->nWaterDefChgMul/100.0)+pstDefChg->iWaterDefChgAdd;
			break;
		default:
			return 0;
			break;
	}

	if(iDef < 0)
	{
		iDef = 0;
	}

	return iDef;
}

//计算第二步 双方自身属性修正
double cacl_hurt2(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef, AtkeeInfo *pstAtkeeInfo, AtkProcess *pstAtkProcess,
		 ATKERHURTCHG *pstAtkerHurtChg, ATKEEHURTCHG *pstAtkeeHurtChg, double iHPTmp, int iHeavyHit, int iSkillAttrType)
{
	int iHurtTmpMul, iHurtMul = 0;
	int iAtkeeHurtMul, iAtkeeHurtAdd;
	int iAtkerHurtMul, iAtkerHurtAdd;
	int MonTypeMul = 0;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkProcess->stAtkerInfo.stDyn;
	
	//攻击者修正
	get_atker_hurtmul(pstAtkerHurtChg, iSkillAttrType, &iAtkerHurtMul, &iAtkerHurtAdd);

	//被攻击者修正
	get_atkee_hurtmul(pstAtkeeHurtChg, iSkillAttrType, &iAtkeeHurtMul, &iAtkeeHurtAdd);

	//追加需要的信息
	if(iSkillAttrType == pstSkillDef->MainAttr)
	{
		pstAddAtkInfo->iMainAttrHurt = iHPTmp;
		pstAddAtkInfo->unAtkerMul = iAtkerHurtMul;
		pstAddAtkInfo->unAtkeeMul = iAtkeeHurtMul;
		pstAddAtkInfo->unAtkerAdd = iAtkerHurtAdd;
		pstAddAtkInfo->unAtkeeAdd = iAtkeeHurtAdd;
	}

	//暴击
	if (iHeavyHit)
	{
		iHPTmp = iHPTmp*(HEAVYHIT_BASE + pstAtkerDyn->stHitRateChg.nHeavyHitVal/10000.0);
	}
	
	//计算攻受双方对应属性伤害修正
	iHPTmp = (iHPTmp *(1+iAtkerHurtMul/100.0-iAtkeeHurtMul/100.0) + iAtkerHurtAdd-iAtkeeHurtAdd);
	if(iHPTmp < 0)
	{
		iHPTmp = 0;
	}

	//所有主属性伤害修正
	if (pstAtkerHurtChg && iSkillAttrType == pstSkillDef->MainAttr)
	{
		iAtkerHurtMul = pstAtkerHurtChg->AtkerHurtMul;
		iAtkerHurtAdd = pstAtkerHurtChg->AtkerHurtAdd;
	}
	else
	{
		iAtkerHurtMul = 0;
		iAtkerHurtAdd = 0;
	}

	//怪物修正
	if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
	{
		z_get_hit_mon_subtypemul(pstEnv, pstSkillDef, pstAtkeeInfo->stAtkee.stObj.pstMon, &MonTypeMul);
	}

	iHurtMul = 0;
	//万分率
	iHurtTmpMul = arm_hurt_mul_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
    	iHurtMul += iHurtTmpMul;

	// 狂暴天赋影响最终伤害万分率
	iHurtTmpMul = genius_hurt_mul_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
    	iHurtMul += iHurtTmpMul;

	// 防御天赋
	iHurtTmpMul = genius_hurt_dec_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
    	iHurtMul += iHurtTmpMul;
	
	if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
	{
		//百分率
		iHurtTmpMul = pet_attr_hurt_mul(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo->stAtkee.stObj.pstPlayer);
	    	iHurtMul += iHurtTmpMul;
	}
	
	if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER && pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PLAYER)
	{
		iHurtTmpMul = player_def_hp_chg_rate(pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer, pstAtkeeInfo->stAtkee.stObj.pstPlayer, iSkillAttrType);
	    	iHurtMul += iHurtTmpMul;
	}

	iHurtMul = iHurtMul + iAtkerHurtMul*100 - pstAtkeeHurtChg->AtkeeHurtMul*100;
	iHPTmp = iHPTmp*(10000 + iHurtMul )/10000.0 +
		iAtkerHurtAdd-pstAtkeeHurtChg->AtkeeHurtAdd;
	
	iHPTmp = iHPTmp*(1+MonTypeMul/100.0);
	if(iHPTmp < 0)
	{
		iHPTmp = 0;
	}
	
	return iHPTmp;
}

int player_attr_atk(AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, int iSkillAttrType)
{
	int iAtk = 0, iMul = 0;;
	int iHitRate;
	ATKCHG *pstAtkChg = &pstAtkProcess->stAtkerInfo.stDyn.stAtkChg;
	
	iHitRate = z_hitrate_get(&pstAtkProcess->stAtkerInfo, pstAtkeeInfo);

	if(iHitRate < 6000)
	{
		iMul = 0;
	}
	else if(iHitRate >= 6000 && iHitRate < 30000)
	{
		iMul = (iHitRate/2.4/10000.0 - 0.25)*100.0;
	}
	else
	{
		iMul = 100;
	}

	switch(iSkillAttrType)
	{
		//主属性
		case SKILL_ATTR_ICE:
			{
				int iMinAtk, iMaxAtk;
				iMinAtk = (pstAtkChg->unIceBaseArmAtk+pstAtkChg->iIceMinAtk)*(1+pstAtkChg->nIceAtkChgMul/100.0)+pstAtkChg->iIceAtkChgAdd;
				iMaxAtk = (pstAtkChg->unIceBaseArmAtk+pstAtkChg->iIceMaxAtk)*(1+pstAtkChg->nIceAtkChgMul/100.0)+pstAtkChg->iIceAtkChgAdd;

				iMinAtk += (iMaxAtk-iMinAtk)*iMul/100.0;
				if(iMaxAtk > iMinAtk)
				{
					iAtk = RAND1(iMaxAtk-iMinAtk+1)+iMinAtk;
				}
				else
				{
					iAtk = iMinAtk;
				}
			}
			break;
		case SKILL_ATTR_PHSIC:
			{
				int iMinAtk, iMaxAtk;
				iMinAtk = (pstAtkChg->unPhsicBaseArmAtk+pstAtkChg->iPhsicMinAtk)*(1+pstAtkChg->nPhsicAtkChgMul/100.0)+pstAtkChg->iPhsicAtkChgAdd;
				iMaxAtk = (pstAtkChg->unPhsicBaseArmAtk+pstAtkChg->iPhsicMaxAtk)*(1+pstAtkChg->nPhsicAtkChgMul/100.0)+pstAtkChg->iPhsicAtkChgAdd;

				iMinAtk += (iMaxAtk-iMinAtk)*iMul/100.0;
				if(iMaxAtk > iMinAtk)
				{
					iAtk = RAND1(iMaxAtk-iMinAtk+1)+iMinAtk;
				}
				else
				{
					iAtk = iMinAtk;
				}
			}
			break;
		default:
			return 0;
			break;
	}
	return iAtk;
}

// iEffectID=0 表示加技能伤害 1加最终伤害
int calc_arm_skill_add(ZONESVRENV* pstEnv,AtkProcess *pstAtkProcess, int iSkillID)
{
	Player *pstPlayer;
	int i,j;
	
	if (pstAtkProcess->stAtkerInfo.stAtker.iType != OBJ_PLAYER)
	{
		return 0;
	}

	pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;
	if (!pstPlayer)
		return 0;

	ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	ARMSKILLATTRDEF *pstDef;
	ARMSKILLDATA *pstData;
	int iAdd = 0;
	ITEMDEF *pstItemDef;
	ATKCHG *pstAtkChg = &pstAtkProcess->stAtkerInfo.stDyn.stAtkChg;
	
	for (i=0; i<pstRoleWear->ArmNum; i++)
	{
		pstData = &pstRoleWear->RoleArms[i].ArmSkill;
		if (pstData->Num <= 0)
		{
			continue;
		}

		pstItemDef = z_find_itemdef(pstEnv, pstRoleWear->RoleArms[i].DefID);
		if ( !pstItemDef ||
			pstItemDef->Important >= 10 ||
			pstItemDef->WearPre.RoleLevelMin > 120)
		{
			continue;
		}

		for (j=0; j<pstData->Num;j++)
		{
			pstDef = z_find_arm_skillattr_def(pstEnv, pstData->One[j].ID);
			if (!pstDef || pstDef->SkillID != iSkillID)
			{
				continue;
			}

			// 等级+位置 +品质 +随机修正
			iAdd = pstDef->BaseVal *(pstDef->LvlMdf[pstItemDef->WearPre.RoleLevelMin/10]/10000.0)*
					(arm_skill_wpos_get_mul(pstDef, pstItemDef)/10000.0) *
					(pstDef->ImpMdf[pstItemDef->Important]/10000.0)*
					(pstDef->RandMdf[(int)pstData->One[j].Idx]/10000.0);

			if (pstDef->EffectID == 0)
			{
				pstAtkChg->iIceAtkChgAdd += iAdd;
				pstAtkChg->iPhsicAtkChgAdd += iAdd;	
			}
			else
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iAdd;
			}
		}
	}

	return 0;
}

//计算第一步 攻防计算伤害
double cacl_hurt1(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, SKILLARMCHG *pstSkillArmChg, 
		ATKCHG *pstAtkChg, DEFCHG *pstDefChg, int iSkillAttrType)
{
	//int iIgnoreDefMul = 0;
	int iAtk, iDef;
	int iSkillHurt = 0;
	int iHurtMul = 0;
	int iAtkMul;
	double iHPTmp;
	DYNATTRLIST *pstAtkerDyn = &pstAtkProcess->stAtkerInfo.stDyn;

	int iDecDef = 0,iDecDefMul = 0;
	
	//技能伤害修正
	get_skill_hurt(pstSkillArmChg, pstSkillDef, iSkillAttrType, &iSkillHurt, &iHurtMul);

	if (pstAtkChg)
	{
		if(pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PLAYER)
		{
			iAtk = player_attr_atk(pstAtkProcess, pstAtkeeInfo, iSkillAttrType);
		}
		else
		{
			//属性攻击力
			iAtk = z_cacl_attr_atk(pstAtkChg, iSkillAttrType);
		}

		//减防
		z_get_dec_def(pstAtkChg, iSkillAttrType,&iDecDef,&iDecDefMul);
	}
	else
	{
		iAtk = iSkillHurt;
		iDecDef = 0;
	}

	iAtk = iAtk *(1+iHurtMul/100.0)+iSkillHurt;
	if(iAtk <= 0 && iSkillHurt <= 0)
	{
		//这里相当于免疫了
		return -1;
	}
	//集气对攻击力的修正
	z_calculate_jiqi_power_effect(pstEnv, pstAtkProcess, &iAtk);

	/*
	if(pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PLAYER)
	{
		iIgnoreDefMul = z_get_ignore_def(pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer, iSkillAttrType);
	}
	*/
	
	iDef = z_cacl_attr_def(pstDefChg, iSkillAttrType);
	iDef = (100 -iDecDefMul)/100.0 * iDef - iDecDef;
	if (iDef <= 0)
	{
		iDef = 1;
	}

	if(!pstAtkChg)
	{
		return -1;
	}
	iAtkMul = pstAtkChg->iAtkLevelMul;
	//防止没有武器的 * 0 无伤害
	if(iAtkMul <= 0)
	{
		iAtkMul = 1;
	}

	if(iSkillAttrType == pstSkillDef->MainAttr)
	{
		int iOriDef = iDef;

		if (iOriDef < 1)
		{
			iOriDef = 1;
		}
		
		iDef = iDef+pstAtkerDyn->stDefChg.iDefLevelMul;
		if(iDef < 1)
		{
			iDef = 1;
		}
		
		//攻击力*攻击系数/(防御力+ 攻击者的防御系数)
		iHPTmp = (double)iAtk*(iAtkMul + K_HURT_RATE*iOriDef) /iDef;
	}
	else
	{
		//攻击力-防御力
		iHPTmp = iAtk - iDef;
	}
	
	return iHPTmp;
}

//闪电对伤害的修正
int lightning_mul_hurt(ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess,SKILLDEF *pstSkillDef,double *piHurt)
{
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	Player *pstPlayer = NULL;
	AtkInfo *pstAtkInfo = NULL;
	LightningAtkInfo *pstLightningAtkInfo=NULL;
	int iJumped;
	
	if( pstAtkerInfo->stAtker.iType != OBJ_PLAYER ||
		pstSkillDef->LightningLinkNum <= 0)
	{
		return -1;
	}
	pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
	if(!pstPlayer)
	{
		return -1;
	}
	pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	pstLightningAtkInfo = &pstPlayer->stOnline.stLightningAtkInfo[pstAtkInfo->iAtkLightningIdx];

	iJumped = pstSkillDef->LightningLinkNum - pstLightningAtkInfo->iJumpNum + 1;
	if( iJumped <= 0)
	{
		return -1;
	}
	
	//目标伤害 = 攻击伤害 * （7 + 目标顺序） / 10
	*piHurt = (*piHurt) * (7+iJumped)/10.0;
	
	return 0;
	UNUSED(pstEnv);

}

int pet_attr_hurt_mul(ZONESVRENV* pstEnv, AtkerInfo *pstAtkerInfo, Player *pstAtkee)
{
	if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
	{
		Player *pstAtker = pstAtkerInfo->stAtker.stObj.pstPlayer;
		ROLEPET *pstAtkerPet, *pstAtkeePet;
		int iAtkerYongQi = 0, iAtkeeYongQi = 0;

		pstAtkerPet = pet_get_camp(pstEnv, pstAtker);
		if(pstAtkerPet && is_pet_heti(pstAtker))
		{
			iAtkerYongQi = pstAtkerPet->FightAttr.YongQi;
		}

		pstAtkeePet = pet_get_camp(pstEnv, pstAtkee);
		if(pstAtkeePet && is_pet_heti(pstAtkee))
		{
			iAtkeeYongQi = pstAtkeePet->FightAttr.YongQi;
		}

		if(iAtkerYongQi > iAtkeeYongQi)
		{
			int iVal = iAtkerYongQi-iAtkeeYongQi;

			//伤害系数增加(A-B)/(30*(A-B)+200)*100
			return iVal*10000.0/(30.0*iVal+200.0);
		}
		else
		{
			int iVal = iAtkeeYongQi-iAtkerYongQi;
			//伤害系数减少(B-A)/(30*(B-A)+200)*100
			return -(iVal*10000.0/(30.0*iVal+200));
		}
	}
	else if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
	{
		ROLEPET *pstRolePet;
		
		if(pstAtkee->stOnline.fPetShouHu)
		{
			pstRolePet = pet_get_camp(pstEnv, pstAtkee);
			if(pstRolePet)
			{
				return -pstRolePet->FightAttr.DecHurtMul;
			}
		}
	}
	
	return 0;
}

int pktype_hurt_mul_get(ZONESVRENV *pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo)
{
	int iHurtMul = 0;
	ZoneAni *pstAtkerAni = &pstAtkerInfo->stAtker;
	ZoneAni *pstAtkeeAni = &pstAtkeeInfo->stAtkee;

	if(pstAtkeeAni->iType != OBJ_PLAYER||
		pstAtkerAni->iType != OBJ_PLAYER)
	{
		return 0;
	}

	if(pstAtkeeAni->iType == OBJ_PLAYER)
	{
		int iPKType;
		Player *psAtkee = pstAtkeeAni->stObj.pstPlayer;

		iPKType = get_player_pkval_type(psAtkee);
		if(iPKType  == PK_VAL_TYPE_BLACK)
		{
			if(psAtkee->stRoleData.MiscInfo.PkInfo.PkVal >= 500)
			{
				iHurtMul += 500;
			}
			else if(psAtkee->stRoleData.MiscInfo.PkInfo.PkVal >= 400)
			{
				iHurtMul += 100;
			}
			else if(psAtkee->stRoleData.MiscInfo.PkInfo.PkVal >= 300)
			{
				iHurtMul += 50;
			}
			else if(psAtkee->stRoleData.MiscInfo.PkInfo.PkVal >= 200)
			{
				iHurtMul += 10;
			}
		}
	}
	
	if(pstAtkerAni->iType == OBJ_PLAYER)
	{
		int iPKType;
		Player *psAtker = pstAtkerAni->stObj.pstPlayer;

		iPKType = get_player_pkval_type(psAtker);
		if(iPKType  == PK_VAL_TYPE_BLACK)
		{
			if(psAtker->stRoleData.MiscInfo.PkInfo.PkVal >= 500)
			{
				iHurtMul -= 90;
			}
			else if(psAtker->stRoleData.MiscInfo.PkInfo.PkVal >= 400)
			{
				iHurtMul -= 50;
			}
			else if(psAtker->stRoleData.MiscInfo.PkInfo.PkVal >= 300)
			{
				iHurtMul -= 30;
			}
			else if(psAtker->stRoleData.MiscInfo.PkInfo.PkVal >= 200)
			{
				iHurtMul -= 10;
			}
		}
	}

	return iHurtMul;
	UNUSED(pstEnv);	//add by paraunused.pl
}

CAREERHURTDEF* z_find_career_hurt_def(ZONESVRENV *pstEnv, int iCareer)
{
	int iIndex;
	int iEqu;
	CAREERHURTDEF stDef;
	ZoneObj *pstObj = pstEnv->pstZoneObj;
	stDef.Career = iCareer;
	iIndex = bsearch_int(&stDef, pstObj->sCareerHurtDef, pstObj->iCareerHurtDef, CAREER_HURT_DEF_DATA, &iEqu);
	if (iEqu)
	{
		return (CAREERHURTDEF*)pstObj->sCareerHurtDef[iIndex];
		
	}
	return NULL;
}

int player_career_hurt_mul_get(ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee)
{
	CAREERHURTDEF *pstCareerHurtDef;

	pstCareerHurtDef = z_find_career_hurt_def(pstEnv, pstAtker->stRoleData.Career);
	if(!pstCareerHurtDef)
	{
		return 0;
	}

	switch(pstAtkee->stRoleData.Career)
	{
		case CAREER_WARRIOR:
			return pstCareerHurtDef->ZhanShi;
			break;
		case CAREER_WARRIOR_SINGLEHAND:
			return pstCareerHurtDef->ShouHu;
			break;
		case CAREER_WARRIOR_DOUBLEHAND:
			return pstCareerHurtDef->ZhengFu;
			break;
		case CAREER_MAGICOR:
			return pstCareerHurtDef->FaShi;
			break;
		case CAREER_MAGICOR_SINGLEHAND:
			return pstCareerHurtDef->JuNeng;
			break;
		case CAREER_MAGICOR_DOUBLEHAND:
			return pstCareerHurtDef->LianHun;
			break;
		case CAREER_GUNER:
			return pstCareerHurtDef->QiangShou;
			break;
		case CAREER_GUNER_SINGLEHAND:
			return pstCareerHurtDef->LieSha;
			break;
		case CAREER_GUNER_DOUBLEHAND:
			return pstCareerHurtDef->HuiMie;
			break;
		case CAREER_CHURCH:
			return pstCareerHurtDef->JiSi;
			break;
		case CAREER_CHURCH_SINGLEHAND:
			return pstCareerHurtDef->ShengYou;
			break;
		case CAREER_CHURCH_DOUBLEHAND:
			return pstCareerHurtDef->QuLei;
			break;
	}
	
	return 0;
}
	
int cacl_attr_hurt(ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo,
		SKILLDEF *pstSkillDef, int iHeavyHit, int iSkillAttrType)
{
	double iHPTmp, iHP;
	DEFCHG *pstDefChg;
	ATKCHG *pstAtkChg = NULL;
	ATKERHURTCHG *pstAtkerHurtChg = NULL;
	SKILLARMCHG *pstSkillArmChg = pstAtkProcess->pstSkillArmChg;
	DYNATTRLIST *pstAtkeeDyn = &pstAtkeeInfo->stDyn;
	ATKEEHURTCHG *pstAtkeeHurtChg = &pstAtkeeDyn->stAtkeeHurtChg;
	int iHurtMul = 100;

	// 确保武器索引正确
	if (pstAtkProcess->iAtkChgIdx < 0 ||
		pstAtkProcess->iAtkChgIdx  >= MAX_ARM_ON)
	{
		return -1;
	}

	pstAtkChg = &pstAtkProcess->stAtkerInfo.stDyn.stAtkChg;
	pstAtkerHurtChg = &pstAtkChg->stAtkerHurtChg;
	pstDefChg = &pstAtkeeDyn->stDefChg;

	//计算第一步 攻防计算伤害
	iHPTmp = cacl_hurt1(pstEnv, pstSkillDef, pstAtkProcess, pstAtkeeInfo, pstSkillArmChg, pstAtkChg, pstDefChg, iSkillAttrType);
	if(iHPTmp < 0)
	{	
		//免疫了
		return 0;
	}
	
	//计算第二步 双方自身属性修正
	iHP = cacl_hurt2(pstEnv, pstSkillDef, pstAtkeeInfo, pstAtkProcess, 
			pstAtkerHurtChg, pstAtkeeHurtChg, iHPTmp, iHeavyHit, iSkillAttrType);
	
	 // 计算积累数全部使用的效果 
	iHP += z_calculate_collectskill_useall_effect(pstEnv, pstAtkProcess, pstSkillDef);
	if (iHP < 1)
	{
		iHP = 1;
	}

	lightning_mul_hurt(pstEnv, pstAtkProcess,pstSkillDef,&iHP);

	if(OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		ZoneAni *pstAtker = &pstAtkProcess->stAtkerInfo.stAtker;

		if (pstAtker->iType == OBJ_PLAYER && 
			pstAtker->stObj.pstPlayer->stRoleData.MiscInfo.TmpSys.TmpItemID > 0 &&
			pstAtker->stObj.pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE)
		{
		}
		else if( 0<=z_mon_dec_one_hp_by_hit(pstEnv,pstAtkeeInfo->stAtkee.stObj.pstMon) )
		{
			iHP = 1;
		}
	}

	//圣佑暴力降低伤害
	if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER && pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PLAYER)
	{
		/*
		Player *pstTmpPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;

		if(pstTmpPlayer->stRoleData.Career == CAREER_CHURCH_SINGLEHAND)
		{
			iHP = iHP*80/100.0;
		}
		*/

	
		iHurtMul = player_career_hurt_mul_get(pstEnv, 
				pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer, pstAtkeeInfo->stAtkee.stObj.pstPlayer);
		iHP = iHP*(100 + iHurtMul)/100.0;
	}

	iHurtMul = atker_hurt_mul_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
	iHP = iHP*iHurtMul/100.0;

	iHurtMul = pktype_hurt_mul_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
	iHP = iHP*(100 + iHurtMul)/100.0;
	if (iHP < 1)
	{
		iHP = 1;
	}
	return iHP;
}


int cacl_abs_def(AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo, int *piHP)
{
	int iAbsDef = pstAtkeeInfo->stDyn.stDefChg.iAbsDef;

	if (iAbsDef <= 0)
	{
		return 0;
	}

	// 必须是人打人
	if (pstAtkeeInfo->stAtkee.iType != OBJ_PLAYER || pstAtkerInfo->stAtker.iType != OBJ_PLAYER)
	{
		return 0;
	}

	if ((*piHP) > iAbsDef/100.0*pstAtkeeInfo->stDyn.stMidAttrList.uiHPMax)
	{
		(*piHP) = iAbsDef/100.0*pstAtkeeInfo->stDyn.stMidAttrList.uiHPMax;
	}
	return 0;
}

int cacl_hurt(ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, 
		SKILLDEF *pstSkillDef, int iHeavyHit)
{
	int iHP = 0;

	//主属性
	iHP += cacl_attr_hurt(pstEnv, pstAtkProcess, pstAtkeeInfo, pstSkillDef, iHeavyHit,  pstSkillDef->MainAttr);

	//元素属性都算进去
	iHP += cacl_attr_hurt(pstEnv, pstAtkProcess, pstAtkeeInfo, pstSkillDef, iHeavyHit, SKILL_ATTR_FIRE);

	iHP += cacl_attr_hurt(pstEnv, pstAtkProcess, pstAtkeeInfo, pstSkillDef, iHeavyHit, SKILL_ATTR_THUNDER);

	iHP += cacl_attr_hurt(pstEnv, pstAtkProcess, pstAtkeeInfo, pstSkillDef, iHeavyHit, SKILL_ATTR_NIGHT);

	iHP += cacl_attr_hurt(pstEnv, pstAtkProcess, pstAtkeeInfo, pstSkillDef, iHeavyHit, SKILL_ATTR_LIGHT);

	iHP += cacl_attr_hurt(pstEnv, pstAtkProcess, pstAtkeeInfo, pstSkillDef, iHeavyHit, SKILL_ATTR_WATER);

	// 计算绝对防御
	cacl_abs_def(&pstAtkProcess->stAtkerInfo,pstAtkeeInfo, &iHP);
	return iHP;
}

/*int z_hit_hurt(ZONESVRENV* pstEnv, DYNATTRLIST *pstAtkerDyn, AtkProcess *pstAtkProcess, DYNATTRLIST *pstAtkeeDyn, SKILLDEF *pstSkillDef, int iHeavyHit, AddAtkInfo *pstAddAtkInfo, Monster *pstAtkee)
{
	DEFCHG *pstDefChg;
	double iHP, iHPTmp;
	int iSkillHurt;
	ATKERHURTCHG *pstAtkerHurtChg1 = NULL;
	ATKEEHURTCHG *pstAtkeeHurtChg = &pstAtkeeDyn->stAtkeeHurtChg;
	int iDamage;
	int iHurtMin;
	int iHurtMax;
	int iHurtMul;
	int iHurtMulMax;
	int iDecDef, iAtkerHurtMul, iAtkerHurtAdd;
	ATKCHG *pstAtkChg1 = NULL;
	SKILLARMCHG *pstSkillArmChg = pstAtkProcess->pstSkillArmChg;
	double iAtk = 0;
	double iDef = 0;
	int iMagicCacl = 0;
	int MonTypeMul = 0;

	if (pstAtkProcess->iAtkChgIdx >= 0)
	{
		pstAtkChg1 = &pstAtkProcess->stAtkerInfo.stDyn.astAtkChg[pstAtkProcess->iAtkChgIdx];
	}

	if (pstAtkChg1)
	{
		pstAtkerHurtChg1 = &pstAtkChg1->stAtkerHurtChg;
		iDamage = pstAtkChg1->nArmLevChgMul*(1+pstAtkChg1->nDamageMul/100.0)+pstAtkChg1->unDamageAdd;
	}
	else
	{
		iDamage = 0;
	}
	
	pstDefChg = &pstAtkeeDyn->stDefChg;
	

	iHP = 0;
	if ((pstSkillDef->MainAttr == SKILL_ATTR_ICE || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->IceHurtMax> 0)
	{
		iMagicCacl = 1;

		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_ICE)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
			iHurtMulMax = pstSkillArmChg->HurtMulMax;
		}
		else
		{
			iHurtMin = pstSkillDef->IceHurtMin;
			iHurtMax = pstSkillDef->IceHurtMax;
			iHurtMul = pstSkillDef->IceHurtMul;
			iHurtMulMax = pstSkillDef->IceHurtMulMax;
		}

		if(iHurtMulMax > iHurtMul)
		{
			iHurtMul = iHurtMul + RAND1(iHurtMulMax-iHurtMul+1);
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin + RAND1(iHurtMax-iHurtMin+1);
		}
		else
		{
			iSkillHurt = 0;
		}

		if (pstAtkChg1)
		{
			//iAtk = (pstAtkChg1->unIceBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg1->nIceAtkChgMul/100.0)+pstAtkChg1->iIceAtkChgAdd;
			iAtk = (pstAtkChg1->unIceBaseArmAtk*(1+pstAtkChg1->nIceAtkChgMul/100.0)+pstAtkChg1->iIceAtkChgAdd)*(1+iHurtMul/100.0)+iSkillHurt;
			iDecDef = pstAtkChg1->iIceDecDef;
		}
		else
		{
			iAtk = iSkillHurt;
			iDecDef = 0;
		}

		if (pstAtkProcess->bContinueNum > 1)
		{
			iAtk = iAtk / pstAtkProcess->bContinueNum;
		}
		z_calculate_jiqi_power_effect(pstEnv, pstAtkProcess, &iAtk);
		
		iDef = pstDefChg->unIceDefBaseAdd*(1+pstDefChg->nIceDefChgMul/100.0)+pstDefChg->iIceDefChgAdd - iDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;
		

		if (pstAtkerHurtChg1)
		{
			iAtkerHurtMul = pstAtkerHurtChg1->AtkerIceHurtMul;
			iAtkerHurtAdd = pstAtkerHurtChg1->AtkerIceHurtAdd;
		}
		else
		{
			iAtkerHurtMul = 0;
			iAtkerHurtAdd = 0;
		}

		if (pstSkillDef->MainAttr == SKILL_ATTR_ICE)
		{
			pstAddAtkInfo->iMainAttrHurt = iHPTmp;
			pstAddAtkInfo->unAtkerMul = iAtkerHurtMul;
			pstAddAtkInfo->unAtkeeMul = pstAtkeeHurtChg->AtkeeIceHurtMul;
			pstAddAtkInfo->unAtkerAdd = iAtkerHurtAdd;
			pstAddAtkInfo->unAtkeeAdd = pstAtkeeHurtChg->AtkeeIceHurtAdd;
			
			if (iHeavyHit)
			{
				iHPTmp = iHPTmp*(HEAVYHIT_BASE + pstAtkerDyn->stHitRateChg.nHeavyHitVal/100.0);
			}
		}
		
		iHP += (iHPTmp*(1+iAtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeIceHurtMul/100.0) + 
				iAtkerHurtAdd-pstAtkeeHurtChg->AtkeeIceHurtAdd);
	}
	
	if ((pstSkillDef->MainAttr == SKILL_ATTR_FIRE || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->FireHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_FIRE)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->FireHurtMin;
			iHurtMax = pstSkillDef->FireHurtMax;
			iHurtMul = pstSkillDef->FireHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin + RAND1(iHurtMax-iHurtMin+1);
		}
		else
		{
			iSkillHurt = 0;
		}

		if (pstAtkChg1)
		{
			//iAtk = (pstAtkChg1->unFireBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg1->nFireAtkChgMul/100.0)+pstAtkChg1->iFireAtkChgAdd;
			iAtk = (pstAtkChg1->unFireBaseArmAtk*(1+pstAtkChg1->nFireAtkChgMul/100.0)+pstAtkChg1->iFireAtkChgAdd)*(1+iHurtMul/100.0)+iSkillHurt;
			iDecDef = pstAtkChg1->iFireDecDef;
		}
		else
		{
			iAtk = iSkillHurt;
			iDecDef = 0;
		}
				
		
		if (pstAtkProcess->bContinueNum > 1)
		{
			iAtk = iAtk / pstAtkProcess->bContinueNum;
		}
		
		iDef = pstDefChg->unFireDefBaseAdd*(1+pstDefChg->nFireDefChgMul/100.0)+pstDefChg->iFireDefChgAdd - iDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;
		
		if (pstAtkerHurtChg1)
		{
			iAtkerHurtMul = pstAtkerHurtChg1->AtkerFireHurtMul;
			iAtkerHurtAdd = pstAtkerHurtChg1->AtkerFireHurtAdd;
		}
		else
		{
			iAtkerHurtMul = 0;
			iAtkerHurtAdd = 0;
		}

		if (pstSkillDef->MainAttr == SKILL_ATTR_FIRE)
		{
			pstAddAtkInfo->iMainAttrHurt = iHPTmp;
			pstAddAtkInfo->unAtkerMul = iAtkerHurtMul;
			pstAddAtkInfo->unAtkeeMul = pstAtkeeHurtChg->AtkeeFireHurtMul;
			pstAddAtkInfo->unAtkerAdd = iAtkerHurtAdd;
			pstAddAtkInfo->unAtkeeAdd = pstAtkeeHurtChg->AtkeeFireHurtAdd;
			if (iHeavyHit)
			{
				iHPTmp = iHPTmp*(HEAVYHIT_BASE + pstAtkerDyn->stHitRateChg.nHeavyHitVal/100.0);
			}
		}
		
		iHP += (iHPTmp*(1+iAtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeFireHurtMul/100.0) + 
				iAtkerHurtAdd-pstAtkeeHurtChg->AtkeeFireHurtAdd);
	}

	if ((pstSkillDef->MainAttr == SKILL_ATTR_THUNDER || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->ThunderHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_THUNDER)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->ThunderHurtMin;
			iHurtMax = pstSkillDef->ThunderHurtMax;
			iHurtMul = pstSkillDef->ThunderHurtMul;
		}

		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin + RAND1(iHurtMax-iHurtMin+1);
		}
		else
		{
			iSkillHurt = 0;
		}

		if (pstAtkChg1)
		{
			//iAtk = (pstAtkChg1->unThunderBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg1->nThunderAtkChgMul/100.0)+pstAtkChg1->iThunderAtkChgAdd;
			iAtk = (pstAtkChg1->unThunderBaseArmAtk*(1+pstAtkChg1->nThunderAtkChgMul/100.0)+pstAtkChg1->iThunderAtkChgAdd)*(1+iHurtMul/100.0)+iSkillHurt;
			iDecDef = pstAtkChg1->iThunderDecDef;
		}
		else
		{
			iAtk = iSkillHurt;
			iDecDef = 0;
		}
				
		if (pstAtkProcess->bContinueNum > 1)
		{
			iAtk = iAtk / pstAtkProcess->bContinueNum;
		}

		iDef = pstDefChg->unThunderDefBaseAdd*(1+pstDefChg->nThunderDefChgMul/100.0)+pstDefChg->iThunderDefChgAdd	- iDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		if (pstAtkerHurtChg1)
		{
			iAtkerHurtMul = pstAtkerHurtChg1->AtkerThunderHurtMul;
			iAtkerHurtAdd = pstAtkerHurtChg1->AtkerThunderHurtAdd;
		}
		else
		{
			iAtkerHurtMul = 0;
			iAtkerHurtAdd = 0;
		}

		if (pstSkillDef->MainAttr == SKILL_ATTR_THUNDER)
		{
			pstAddAtkInfo->iMainAttrHurt = iHPTmp;
			pstAddAtkInfo->unAtkerMul = iAtkerHurtMul;
			pstAddAtkInfo->unAtkeeMul = pstAtkeeHurtChg->AtkeeThunderHurtMul;
			pstAddAtkInfo->unAtkerAdd = iAtkerHurtAdd;
			pstAddAtkInfo->unAtkeeAdd = pstAtkeeHurtChg->AtkeeThunderHurtAdd;
			if (iHeavyHit)
			{
				iHPTmp = iHPTmp*(HEAVYHIT_BASE + pstAtkerDyn->stHitRateChg.nHeavyHitVal/100.0);
			}
		}
		
		iHP += (iHPTmp*(1+iAtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeThunderHurtMul/100.0) + 
				iAtkerHurtAdd-pstAtkeeHurtChg->AtkeeThunderHurtAdd);
	}
	
	if ((pstSkillDef->MainAttr == SKILL_ATTR_LIGHT || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->LightHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_LIGHT)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->LightHurtMin;
			iHurtMax = pstSkillDef->LightHurtMax;
			iHurtMul = pstSkillDef->LightHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin + RAND1(iHurtMax-iHurtMin+1);
		}
		else
		{
			iSkillHurt = 0;
		}

		if (pstAtkChg1)
		{
			//iAtk = (pstAtkChg1->unLightBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg1->nLightAtkChgMul/100.0)+pstAtkChg1->iLightAtkChgAdd;
			iAtk = (pstAtkChg1->unLightBaseArmAtk*(1+pstAtkChg1->nLightAtkChgMul/100.0)+pstAtkChg1->iLightAtkChgAdd)*(1+iHurtMul/100.0)+iSkillHurt;
			iDecDef = pstAtkChg1->iLightDecDef;
		}
		else
		{
			iAtk = iSkillHurt;
			iDecDef = 0;
		}
				
		if (pstAtkProcess->bContinueNum > 1)
		{
			iAtk = iAtk / pstAtkProcess->bContinueNum;
		}
		
		iDef = pstDefChg->unLightDefBaseAdd*(1+pstDefChg->nLightDefChgMul/100.0)+pstDefChg->iLightDefChgAdd - iDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		if (pstAtkerHurtChg1)
		{
			iAtkerHurtMul = pstAtkerHurtChg1->AtkerLightHurtMul;
			iAtkerHurtAdd = pstAtkerHurtChg1->AtkerLightHurtAdd;
		}
		else
		{
			iAtkerHurtMul = 0;
			iAtkerHurtAdd = 0;
		}

		if (pstSkillDef->MainAttr == SKILL_ATTR_LIGHT)
		{
			pstAddAtkInfo->iMainAttrHurt = iHPTmp;
			pstAddAtkInfo->unAtkerMul = iAtkerHurtMul;
			pstAddAtkInfo->unAtkeeMul = pstAtkeeHurtChg->AtkeeLightHurtMul;
			pstAddAtkInfo->unAtkerAdd = iAtkerHurtAdd;
			pstAddAtkInfo->unAtkeeAdd = pstAtkeeHurtChg->AtkeeLightHurtAdd;
			if (iHeavyHit)
			{
				iHPTmp = iHPTmp*(HEAVYHIT_BASE + pstAtkerDyn->stHitRateChg.nHeavyHitVal/100.0);
			}
		}
		
		iHP += (iHPTmp*(1+iAtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeLightHurtMul/100.0) + 
				iAtkerHurtAdd-pstAtkeeHurtChg->AtkeeLightHurtAdd);
	}

	if ((pstSkillDef->MainAttr == SKILL_ATTR_NIGHT || pstSkillDef->MainAttr == SKILL_ATTR_PHSIC) && pstSkillDef->NightHurtMax> 0)
	{
		iMagicCacl = 1;
		
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_NIGHT)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
		}
		else
		{
			iHurtMin = pstSkillDef->NightHurtMin;
			iHurtMax = pstSkillDef->NightHurtMax;
			iHurtMul = pstSkillDef->NightHurtMul;
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin + RAND1(iHurtMax-iHurtMin+1);
		}
		else
		{
			iSkillHurt = 0;
		}

		if (pstAtkChg1)
		{
			//iAtk = (pstAtkChg1->unNightBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg1->nNightAtkChgMul/100.0)+pstAtkChg1->iNightAtkChgAdd;
			iAtk = (pstAtkChg1->unNightBaseArmAtk*(1+pstAtkChg1->nNightAtkChgMul/100.0)+pstAtkChg1->iNightAtkChgAdd)*(1+iHurtMul/100.0)+iSkillHurt;
			iDecDef = pstAtkChg1->iNightDecDef;
		}
		else
		{
			iAtk = iSkillHurt;
			iDecDef = 0;
		}

		if (pstAtkProcess->bContinueNum > 1)
		{
			iAtk = iAtk / pstAtkProcess->bContinueNum;
		}
		
		iDef = pstDefChg->unNightDefBaseAdd*(1+pstDefChg->nNightDefChgMul/100.0)+pstDefChg->iNightDefChgAdd - iDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		if (pstAtkerHurtChg1)
		{
			iAtkerHurtMul = pstAtkerHurtChg1->AtkerNightHurtMul;
			iAtkerHurtAdd = pstAtkerHurtChg1->AtkerNightHurtAdd;
		}
		else
		{
			iAtkerHurtMul = 0;
			iAtkerHurtAdd = 0;
		}

		if (pstSkillDef->MainAttr == SKILL_ATTR_NIGHT)
		{
			pstAddAtkInfo->iMainAttrHurt = iHPTmp;
			pstAddAtkInfo->unAtkerMul = iAtkerHurtMul;
			pstAddAtkInfo->unAtkeeMul = pstAtkeeHurtChg->AtkeeNightHurtMul;
			pstAddAtkInfo->unAtkerAdd = iAtkerHurtAdd;
			pstAddAtkInfo->unAtkeeAdd = pstAtkeeHurtChg->AtkeeNightHurtAdd;
			if (iHeavyHit)
			{
				iHPTmp = iHPTmp*(HEAVYHIT_BASE + pstAtkerDyn->stHitRateChg.nHeavyHitVal/100.0);
			}
		}
		
		iHP += (iHPTmp*(1+iAtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeNightHurtMul/100.0) + 
				iAtkerHurtAdd-pstAtkeeHurtChg->AtkeeNightHurtAdd);
	}

	//法术伤害处理
	if (iMagicCacl)
	{
		if (pstAtkerHurtChg1)
		{
			iAtkerHurtMul = pstAtkerHurtChg1->AtkerMagicHurtMul;
			iAtkerHurtAdd = pstAtkerHurtChg1->AtkerMagicHurtAdd;
		}
		else
		{
			iAtkerHurtMul = 0;
			iAtkerHurtAdd = 0;
		}
		
		iHP = iHP*(1+iAtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeMagicHurtMul/100.0) + 
				iAtkerHurtAdd-pstAtkeeHurtChg->AtkeeMagicHurtAdd;
	}
	
	if (pstSkillDef->MainAttr == SKILL_ATTR_PHSIC && pstSkillDef->PhsicHurtMax> 0)
	{
		if (pstSkillArmChg && pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)	
		{
			iHurtMin = pstSkillArmChg->HurtMin;
			iHurtMax = pstSkillArmChg->HurtMax;
			iHurtMul = pstSkillArmChg->HurtMul;
			iHurtMulMax = pstSkillArmChg->HurtMulMax;
		}
		else
		{
			iHurtMin = pstSkillDef->PhsicHurtMin;
			iHurtMax = pstSkillDef->PhsicHurtMax;
			iHurtMul = pstSkillDef->PhsicHurtMul;
			iHurtMulMax = pstSkillDef->PhsicHurtMulMax;
		}

		if(iHurtMulMax > iHurtMul)
		{
			iHurtMul = iHurtMul + RAND1(iHurtMulMax-iHurtMul+1);
		}
		
		if (iHurtMax >= iHurtMin)
		{
			iSkillHurt = iHurtMin + RAND1(iHurtMax-iHurtMin+1);
		}
		else
		{
			iSkillHurt = 0;
		}

		if (pstAtkerHurtChg1)
		{
			iAtkerHurtMul = pstAtkerHurtChg1->AtkerPhsicHurtMul;
			iAtkerHurtAdd = pstAtkerHurtChg1->AtkerPhsicHurtAdd;
		}
		else
		{
			iAtkerHurtMul = 0;
			iAtkerHurtAdd = 0;
		}

		if (pstAtkChg1)
		{
			//iAtk = (pstAtkChg1->unPhsicBaseArmAtk*(1+iHurtMul/100.0)+iSkillHurt)*(1+pstAtkChg1->nPhsicAtkChgMul/100.0)+pstAtkChg1->iPhsicAtkChgAdd;
			iAtk = (pstAtkChg1->unPhsicBaseArmAtk*(1+pstAtkChg1->nPhsicAtkChgMul/100.0)+pstAtkChg1->iPhsicAtkChgAdd)*(1+iHurtMul/100.0)+iSkillHurt;
			iDecDef = pstAtkChg1->iPhsicDecDef;
		}
		else
		{
			iAtk = iSkillHurt;
			iDecDef = 0;
		}
				
		if (pstAtkProcess->bContinueNum > 1)
		{
			iAtk = iAtk / pstAtkProcess->bContinueNum;
		}
		z_calculate_jiqi_power_effect(pstEnv, pstAtkProcess, &iAtk);
		
		iDef = pstDefChg->unPhsicDefBaseAdd*(1+pstDefChg->nPhsicDefChgMul/100.0)+pstDefChg->iPhsicDefChgAdd - iDecDef;
		if (iDef <= 0)
		{
			iDef = 1;
		}

		//iHPTmp = iAtk*(1+pstAtkChg->nArmLevChgMul/100.0)/iDef;
		iHPTmp = iAtk*(1+iDamage/100.0)/iDef;

		if (pstAtkerHurtChg1)
		{
			iAtkerHurtMul = pstAtkerHurtChg1->AtkerPhsicHurtMul;
			iAtkerHurtAdd = pstAtkerHurtChg1->AtkerPhsicHurtAdd;
		}
		else
		{
			iAtkerHurtMul = 0;
			iAtkerHurtAdd = 0;
		}

		if (pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)
		{
			pstAddAtkInfo->iMainAttrHurt = iHPTmp;
			pstAddAtkInfo->unAtkerMul = iAtkerHurtMul;
			pstAddAtkInfo->unAtkeeMul = pstAtkeeHurtChg->AtkeePhsicHurtMul;
			pstAddAtkInfo->unAtkerAdd = iAtkerHurtAdd;
			pstAddAtkInfo->unAtkeeAdd = pstAtkeeHurtChg->AtkeePhsicHurtAdd;
			if (iHeavyHit)
			{
				iHPTmp = iHPTmp*(HEAVYHIT_BASE + pstAtkerDyn->stHitRateChg.nHeavyHitVal/100.0);
			}
		}
		
		iHPTmp = (iHPTmp*(1+iAtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeePhsicHurtMul/100.0) + 
				 	iAtkerHurtAdd-pstAtkeeHurtChg->AtkeePhsicHurtAdd);
	}
	else
	{
		iHPTmp = 0;
	}

	if (pstAtkerHurtChg1)
	{
		iAtkerHurtMul = pstAtkerHurtChg1->AtkerHurtMul;
		iAtkerHurtAdd = pstAtkerHurtChg1->AtkerHurtAdd;
	}
	else
	{
		iAtkerHurtMul = 0;
		iAtkerHurtAdd = 0;
	}

	if(pstAtkee)
	{
		z_get_hit_mon_subtypemul(pstEnv, pstSkillDef, pstAtkee, &MonTypeMul);
	}

	iHP = ((iHP*1.0 + iHPTmp)*(1+iAtkerHurtMul/100.0-pstAtkeeHurtChg->AtkeeHurtMul/100.0) + 
			iAtkerHurtAdd-pstAtkeeHurtChg->AtkeeHurtAdd)*(1+MonTypeMul/100.0);

	 // 计算积累数全部使用的效果 
	iHP += z_calculate_collectskill_useall_effect(pstEnv, pstAtkProcess, pstSkillDef);

	
	if (iHP < 1)
	{
		iHP = 1;
	}
	
	return iHP;
}*/

int z_get_hithp(CSONEHIT *pstOneHit)
{
	int i;
	int iHP = 0;

	for (i=0; i<pstOneHit->TotalHurt; i++)
	{
		if (pstOneHit->Hurts[i].HurtType == HURT_TYPE_HP)
		{
			iHP += pstOneHit->Hurts[i].Value;
		}
	}

	return iHP;
}

void z_get_mon_dyn(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonsterDef, DYNATTRLIST *pstDyn)
{
	ATKCHG *pstAtkChg;
	BCTMON *pstBctMon;
	UNUSED(pstEnv);
	
	memset(pstDyn, 0, sizeof(*pstDyn));
	pstDyn->stMidAttrList.uiHPMax = pstMon->iMaxHP;
	pstAtkChg = &pstDyn->stAtkChg;

	pstAtkChg->iAtkLevelMul = get_mon_level_mul(pstEnv, pstMon, LEVEL_MUL_ATK);
	pstDyn->stDefChg.iDefLevelMul = get_mon_level_mul(pstEnv, pstMon, LEVEL_MUL_DEF);
	pstDyn->stHitRateChg.iDefHeavyHit = get_mon_level_mul(pstEnv, pstMon, LEVEL_MUL_DEF_HEAVYHIT);
	
	//百层塔怪物数值
	pstBctMon = get_mon_bct_dyn(pstEnv, pstMon);
	if(pstBctMon)
	{
		pstAtkChg->unIceBaseArmAtk = pstBctMon->PhsicAtk;
		pstAtkChg->unPhsicBaseArmAtk = pstBctMon->IceAtk;
		pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd += pstBctMon->IceHurtMul;
		pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd += pstBctMon->PhsicHurtMul;
		pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstBctMon->DynHurtMul;
		pstDyn->stDefChg.unIceDefBaseAdd= pstBctMon->IceDefMul;
		pstDyn->stDefChg.unPhsicDefBaseAdd = pstBctMon->PhsicDefMul;
		pstDyn->stHitChg.unHitBaseAdd = pstBctMon->HitMul;
		pstDyn->stFleeChg.unFleeBaseAdd = pstBctMon->FleeMul;
		pstDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += pstBctMon->IceHurtDefMul;
		pstDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd += pstBctMon->PhsicHurtDefMul;

		return ;
	}
	
	memcpy(&pstAtkChg->stAtkerHurtChg, &pstMonsterDef->AtkerHurtChg, sizeof(pstAtkChg->stAtkerHurtChg));
	pstAtkChg->unIceBaseArmAtk = pstMonsterDef->IceAtk;
	pstAtkChg->unPhsicBaseArmAtk = pstMonsterDef->PhsicAtk;
	pstAtkChg->unSiegeBaseArmAtk = pstMonsterDef->SiegeAtk;;
	pstAtkChg->unHolyBaseArmAtk = pstMonsterDef->HolyAtk;;
	pstAtkChg->unMachineBaseArmAtk = pstMonsterDef->MachineAtk;;
	
	pstAtkChg->unFireBaseArmAtk = pstMonsterDef->FireAtk * (1 + pstMon->stMonDynAttr.ElementAtkMul/100.0);
	pstAtkChg->unThunderBaseArmAtk = pstMonsterDef->ThunderAtk * (1 + pstMon->stMonDynAttr.ElementAtkMul/100.0);
	pstAtkChg->unLightBaseArmAtk = pstMonsterDef->LightAtk * (1 + pstMon->stMonDynAttr.ElementAtkMul/100.0);
	pstAtkChg->unNightBaseArmAtk = pstMonsterDef->NightAtk * (1 + pstMon->stMonDynAttr.ElementAtkMul/100.0);
	pstAtkChg->unNightBaseArmAtk = pstMonsterDef->WaterAtk * (1 + pstMon->stMonDynAttr.ElementAtkMul/100.0);

	pstDyn->stDefChg.unIceDefBaseAdd= pstMonsterDef->IceDef * (1 + pstMon->stMonDynAttr.IceDefMul/100.0);
	pstDyn->stDefChg.unPhsicDefBaseAdd = pstMonsterDef->PhsicDef * (1 + pstMon->stMonDynAttr.PhsicDefMul/100.0);
	pstDyn->stDefChg.unSiegeDefBaseAdd = pstMonsterDef->SiegeDef;
	pstDyn->stDefChg.unHolyDefBaseAdd = pstMonsterDef->HolyDef;
	pstDyn->stDefChg.unMachineDefBaseAdd = pstMonsterDef->MachineDef;
	
	pstDyn->stDefChg.unFireDefBaseAdd = pstMonsterDef->FireDef * (1 + pstMon->stMonDynAttr.ElementDefMul/100.0);
	pstDyn->stDefChg.unThunderDefBaseAdd = pstMonsterDef->ThunderDef * (1 + pstMon->stMonDynAttr.ElementDefMul/100.0);
	pstDyn->stDefChg.unLightDefBaseAdd = pstMonsterDef->LightDef * (1 + pstMon->stMonDynAttr.ElementDefMul/100.0);
	pstDyn->stDefChg.unNightDefBaseAdd = pstMonsterDef->NightDef * (1 + pstMon->stMonDynAttr.ElementDefMul/100.0);
	pstDyn->stDefChg.unWaterDefBaseAdd = pstMonsterDef->WaterDef * (1 + pstMon->stMonDynAttr.ElementDefMul/100.0);
	
	memcpy(&pstDyn->stAtkeeHurtChg, &pstMonsterDef->AtkeeHurtChg, sizeof(pstDyn->stAtkeeHurtChg));
	pstDyn->stAtkeeHurtChg.AtkeeIceHurtMul += pstMon->stMonDynAttr.IceHurtDefMul;
	pstDyn->stAtkeeHurtChg.AtkeePhsicHurtMul+= pstMon->stMonDynAttr.PhsicHurtDefMul;
	
	pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul += pstMon->stMonDynAttr.IceHurtMul;
	pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul += pstMon->stMonDynAttr.PhsicHurtMul;
	pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstMon->stMonDynAttr.DynHurtMul;

	pstDyn->stHitRateChg.nHeavyHitAdd = pstMonsterDef->HeavyHitRate;
	pstDyn->stHitChg.unHitBaseAdd = pstMonsterDef->Hit* (1 + pstMon->stMonDynAttr.HitMul/100.0);
	pstDyn->stFleeChg.unFleeBaseAdd = pstMonsterDef->Flee* (1 + pstMon->stMonDynAttr.FleeMul/100.0);

       // 对召唤出来的怪物 会有个修正
	//pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd += pstMon->stMonDynAttr.PhsicAtkAdd;
	//pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd +=  pstMon->stMonDynAttr.IceAtkAdd;
	
	pstAtkChg->iPhsicAtkChgAdd += pstMon->stMonDynAttr.PhsicAtkAdd;
	pstAtkChg->iIceAtkChgAdd +=  pstMon->stMonDynAttr.IceAtkAdd;

	pstDyn->stDefChg.unIceDefBaseAdd += pstMon->iCallIceDefAdd;
	pstDyn->stDefChg.unPhsicDefBaseAdd += pstMon->iCallPhsicDefadd;
	
	if(is_mon_machine(pstMon))
	{
		pstAtkChg->unIceBaseArmAtk = pstAtkChg->unIceBaseArmAtk*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstAtkChg->unPhsicBaseArmAtk = pstAtkChg->unPhsicBaseArmAtk*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstAtkChg->unSiegeBaseArmAtk = pstAtkChg->unSiegeBaseArmAtk*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstAtkChg->unHolyBaseArmAtk = pstAtkChg->unHolyBaseArmAtk*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstAtkChg->unMachineBaseArmAtk = pstAtkChg->unMachineBaseArmAtk*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);

		pstDyn->stDefChg.unIceDefBaseAdd = pstDyn->stDefChg.unIceDefBaseAdd*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstDyn->stDefChg.unPhsicDefBaseAdd = pstDyn->stDefChg.unPhsicDefBaseAdd*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstDyn->stDefChg.unSiegeDefBaseAdd = pstDyn->stDefChg.unSiegeDefBaseAdd*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstDyn->stDefChg.unHolyDefBaseAdd = pstDyn->stDefChg.unHolyDefBaseAdd*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstDyn->stDefChg.unMachineDefBaseAdd = pstDyn->stDefChg.unMachineDefBaseAdd*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		
		pstDyn->stFleeChg.unFleeBaseAdd = pstDyn->stFleeChg.unFleeBaseAdd*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
		pstDyn->stHitChg.unHitBaseAdd = pstDyn->stHitChg.unHitBaseAdd*(1+this_machine(pstEnv, pstMon)->iDynAttrMul/100.0);
	}
	
	return;
}

static int z_result_youqin(ZONESVRENV* pstEnv, Player *pstPlayer, ATKCHG *pstAtkChg, SKILLRESULT *pstResult, RESPOS *pstPos)
{

	/*
		ResultVal1   -> 半径
		ResultVal2   -> 人数
		ResultVal3   -> 修正
	*/

	int i, iDist, iHurtMul;
	int iNum = 0;
	ZoneTeam *pstZoneTeam;

	if (pstResult->SkillResultVal1 <= 0 || pstResult->SkillResultVal2 <= 0 || pstResult->SkillResultVal3 <= 0)
	{
		return -1;
	}

	if (!pstPlayer)
	{
		return -1;
	}

	pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	if (!pstZoneTeam)
	{
		return -1;
	}

	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		if (strcmp( pstPlayer->stRoleData.RoleName, pstZoneTeam->stTeamInfo.TeamMember[i].MemberName) == 0)
		{
			continue;
		}

		if ((int)pstZoneTeam->stTeamInfo.TeamMember[i].MapID != pstPlayer->stRoleData.Map)
		{
			continue;
		}
		
		iDist = z_distance(pstPos, &pstZoneTeam->stTeamInfo.TeamMember[i].Pos);
		if (iDist <= (int)pstResult->SkillResultVal1)
		{
			iNum ++;
		}
	}

	if (iNum == 0)
	{
		return 0;
	}

	if (iNum > pstResult->SkillResultVal2)
	{
		iNum = pstResult->SkillResultVal2;
	}
	
	iHurtMul = (iNum*1.0/pstResult->SkillResultVal2)*pstResult->SkillResultVal3;
	
	if (pstAtkChg)
	{
		pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iHurtMul;
	}
	return 0;
}

static int z_result_guaiqin(ZONESVRENV* pstEnv, Player *pstPlayer, ATKCHG *pstAtkChg, SKILLRESULT *pstResult, RESPOS *pstPos, int iMonID)
{

	/*
		ResultVal1   -> 半径
		ResultVal2   -> 人数
		ResultVal3   -> 修正
	*/

	int i, iDist, iHurtMul,iIndex;
	int iNum = 0;
	char cType;
	TMEMBLOCK *pstMemBlock;
	Monster *pstMon;

	if (pstResult->SkillResultVal1 <= 0 || pstResult->SkillResultVal2 <= 0 || pstResult->SkillResultVal3 <= 0)
	{
		return -1;
	}

	if (!pstPlayer)
	{
		return -1;
	}

	for (i = 0; i < pstPlayer->stOnline.iVAnimate; i++) 
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i], &cType);
		if (iIndex < 0)
		{
			return -1;
		}
		if (cType == OBJ_MONSTER)
		{
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				continue;
			}
			
			pstMon = (Monster *)pstMemBlock->szData;
			if (iMonID == pstMon->iID)
			{
				continue;
			}
			
			iDist = z_distance(pstPos, &pstMon->stCurrPos);
			if (iDist <= pstResult->SkillResultVal1)
			{
				iNum ++;
			}
			
		}
	}

	if (iNum == 0)
	{
		return 0;
	}

	if (iNum > pstResult->SkillResultVal2)
	{
		iNum = pstResult->SkillResultVal2;
	}

	iHurtMul = (iNum*1.0/pstResult->SkillResultVal2)*pstResult->SkillResultVal3;
	
	if (pstAtkChg)
	{
		pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iHurtMul;
	}
	return 0;
}

int z_cacl_skill_result(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, SKILLRESULT *pstResult)
{
	int iHurtMul;
	int iDist;
	int iMonID;
	Monster *pstMon;
	Player *pstPlayer = NULL;
	ROLEPET *pstRolePet;
	RESPOS stPos;
	ATKCHG *pstAtkChg = NULL;
	DYNATTRLIST *pstDyn = &pstAtkProcess->stAtkerInfo.stDyn;
	MONSTERDEF *pstMonDef;
	
	//Player *pstPlayer;
	switch(pstAtkProcess->stAtkerInfo.stAtker.iType)
	{
		case OBJ_PLAYER:
			pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;
			break;
		case OBJ_PET:
			pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.stZonePet.pstOwner;
			break;
		case OBJ_MONSTER:
			if (pstAtkProcess->stAtkerInfo.stAtker.stObj.pstMon->iOwnerID > 0)
			{
				pstPlayer = z_id_player(pstAppCtx, pstEnv, pstAtkProcess->stAtkerInfo.stAtker.stObj.pstMon->iOwnerID);
			}
			break;
		default:
			pstPlayer = NULL;
			break;
	}

	/*
	if (pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PLAYER)
	{
		pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;
	}
	else if(pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PET)
	{
		pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.stZonePet.pstOwner;
	}
	else
	{
		pstPlayer = NULL;
	}
	*/

	if (pstAtkProcess->iAtkChgIdx >= 0)
	{
		pstAtkChg = &pstAtkProcess->stAtkerInfo.stDyn.stAtkChg;
	}
		
	switch (pstResult->SkillResultID)
	{
		case RESULT_HITRATE:
			if(pstResult->SkillResultVal3 > 0)
			{
				int iType = atker_type_get(&pstAtkeeInfo->stAtkee, NULL);
				if(pstResult->SkillResultVal3 != iType)
				{
					break;
				}
			}

			pstDyn->stHitRateChg.nAtkerHitRateAdd += pstResult->SkillResultVal1;
			pstDyn->stHitChg.iHitChgAdd += pstResult->SkillResultVal2;
			break;
		
		case RESULT_HURTLIMT:
			pstAtkProcess->stAtkerInfo.iHurtLimitAdd = pstResult->SkillResultVal1;
			break;
			
		case RESULT_ATK_ADDMP:
			pstDyn->stResourceAddChg.unMPAtkAdd += pstResult->SkillResultVal1;
			pstDyn->stResourceAddChg.unMPAtkMul += pstResult->SkillResultVal2;
			pstDyn->stResourceAddChg.unMPDeadAdd += pstResult->SkillResultVal3;
			pstDyn->stResourceAddChg.unMPDeadMul += pstResult->SkillResultVal4;
			break;		
			
		case RESULT_ATK_ADDCOLLECTNUM:
			pstDyn->stResourceAddChg.unCollectNumAtkAdd += pstResult->SkillResultVal1;
			pstDyn->stResourceAddChg.unCollectNumAtkMul += pstResult->SkillResultVal2;
			pstDyn->stResourceAddChg.unCollectNumDeadAdd += pstResult->SkillResultVal3;
			pstDyn->stResourceAddChg.unCollectNumDeadMul += pstResult->SkillResultVal4;
			break;	

		case RESULT_ICE_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unIceBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nIceAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iIceAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_FIRE_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unFireBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nFireAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iFireAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_THUNDER_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unThunderBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nThunderAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iThunderAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_LIGHT_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unLightBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nLightAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iLightAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;	
		case RESULT_NIGHT_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unNightBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nNightAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iNightAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_PHSIC_ATK:
			if (pstAtkChg)
			{
				pstAtkChg->unPhsicBaseArmAtk += pstResult->SkillResultVal1;
				pstAtkChg->nPhsicAtkChgMul += pstResult->SkillResultVal2;
				pstAtkChg->iPhsicAtkChgAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_DEC_ICEDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iIceDecDef += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DEC_FIREDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iFireDecDef += pstResult->SkillResultVal1;
			}
			break;	
		case RESULT_DEC_THUNDERDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iThunderDecDef += pstResult->SkillResultVal1;
			}
			break;	
		case RESULT_DEC_LIGHTDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iLightDecDef += pstResult->SkillResultVal1;
			}
			break;	
		case RESULT_DEC_NIGHTDEF:
			if (pstAtkChg)
			{
				pstAtkChg->iNightDecDef += pstResult->SkillResultVal1;
			}
			break;	
			
		case RESULT_ICEHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd += pstResult->SkillResultVal2;
			}
			break;
		case RESULT_FIREHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerFireHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerFireHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_THUNDERHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerThunderHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerThunderHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_LIGHTHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerLightHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerLightHurtAdd += pstResult->SkillResultVal2;
			}
			break;		
		case RESULT_NIGHTHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerNightHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerNightHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_PHISICHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_MAGICHURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerMagicHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerMagicHurtAdd += pstResult->SkillResultVal2;
			}
			break;	
		case RESULT_HURT:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
				pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstResult->SkillResultVal2;
			}
			break;
			
		case RESULT_LOW_HURT:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;
				iHurtMul = (1.0 - (pstMon->iHP *1.0 / pstMon->iMaxHP))*pstResult->SkillResultVal1;
			}
			else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
				iHurtMul = (1.0 - (pstPlayer->stRoleData.RoleDetail.HPCurr*1.0 / pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax))*pstResult->SkillResultVal1;
			}
			else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
			{
				pstRolePet = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet;
				iHurtMul = (1.0 - (pstRolePet->HP*1.0 / pstRolePet->MaxHP))*pstResult->SkillResultVal1;
			}
			else
			{
				return -1;
			}

			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iHurtMul;
			}
			
			break;

		case RESULT_HIGH_HURT:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;
				iHurtMul = (pstMon->iHP *1.0 / pstMon->iMaxHP)*pstResult->SkillResultVal1;
			}
			else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
				iHurtMul = (pstPlayer->stRoleData.RoleDetail.HPCurr*1.0 / pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax)*pstResult->SkillResultVal1;
			}
			else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
			{
				pstRolePet = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet;
				iHurtMul = (pstRolePet->HP*1.0 /pstRolePet->MaxHP)*pstResult->SkillResultVal1;
			}
			else
			{
				return -1;
			}

			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iHurtMul;
			}
			
			break;

		case RESULT_FAR_HURT:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;
				stPos = pstMon->stCurrPos;
			}
			else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
				stPos = pstPlayer->stRoleData.Pos;
			}
			else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
			{
				stPos = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstOwner->stRoleData.Pos;
			}
			else
			{
				return -1;
			}

			if (OBJ_MONSTER == pstAtkProcess->stAtkerInfo.stAtker.iType)
			{
				iDist = z_distance(&stPos, &pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer->stRoleData.Pos);
			}
			else if (OBJ_PLAYER == pstAtkProcess->stAtkerInfo.stAtker.iType)
			{
				iDist = z_distance(&stPos, &pstAtkProcess->stAtkerInfo.stAtker.stObj.pstMon->stCurrPos);
			}
			else if (OBJ_PET == pstAtkProcess->stAtkerInfo.stAtker.iType)
			{
				iDist = z_distance(&stPos, &pstAtkProcess->stAtkerInfo.stAtker.stObj.stZonePet.pstOwner->stRoleData.Pos);
			}
			else
			{
				return -1;
			}

			if (0 == pstResult->SkillResultVal1)
			{
				return -1;
			}

			if (iDist > pstResult->SkillResultVal1)
			{
				iDist = pstResult->SkillResultVal1;
			}

			iHurtMul = (iDist*1.0/pstResult->SkillResultVal1)*pstResult->SkillResultVal2;
			
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iHurtMul;
			}
			break;
		case RESULT_YOUQIN:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				stPos = pstAtkeeInfo->stAtkee.stObj.pstMon->stCurrPos;
			}
			else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				stPos = pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.Pos;
			}
			else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
			{
				stPos = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstOwner->stRoleData.Pos;
			}
			else
			{
				return -1;
			}

			if (!pstPlayer)
			{
				return -1;
			}
			if (pstAtkChg)
			{
				z_result_youqin(pstEnv, pstPlayer,pstAtkChg, pstResult, &stPos);
			}
			break;
		case RESULT_GUAIQIN:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;
				stPos = pstMon->stCurrPos;
				iMonID = pstMon->iID;
			}
			else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
				stPos = pstPlayer->stRoleData.Pos;
				iMonID = 0;
			}
			else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
			{
				pstPlayer = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstOwner;
				stPos = pstPlayer->stRoleData.Pos;
				iMonID = 0;
			}
			else
			{
				return -1;
			}
			if (pstAtkChg)
			{
			 	z_result_guaiqin(pstEnv, pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer,pstAtkChg, pstResult,&stPos, iMonID);
			}
			break;

		case RESULT_SORB_HP:
			// ResultVal1 比率
			pstAtkProcess->iSordHp += pstResult->SkillResultVal2;
			if (pstAtkProcess->cSordHpRate + pstResult->SkillResultVal1 > 100)
			{
				pstAtkProcess->cSordHpRate = 100;
			}
			else
			{
				pstAtkProcess->cSordHpRate += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_SORB_MP:
			if (pstAtkProcess->cSordMpRate + pstResult->SkillResultVal1 > 100)
			{
				pstAtkProcess->cSordMpRate = 100;
			}
			else
			{
				pstAtkProcess->cSordMpRate += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_HURT_REN:
			// result1 伤害成修正 result2伤害加修正
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->MonAttrType != MON_ATTR_TYPE_REN)
				{
					return 0;
				}
				
				if (pstAtkChg)
				{
					pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
					pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstResult->SkillResultVal2;
				}
			}
			break;
		case RESULT_HURT_BIANYIREN:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->MonAttrType != MON_ATTR_TYPE_BIANYIREN)
				{
					return 0;
				}
				
				if (pstAtkChg)
				{
					pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
					pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstResult->SkillResultVal2;
				}
			}
			break;
		case RESULT_HURT_SHENGWU:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->MonAttrType != MON_ATTR_TYPE_SHENGWU)
				{
					return 0;
				}
				
				if (pstAtkChg)
				{
					pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
					pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstResult->SkillResultVal2;
				}
			}
			break;

		case RESULT_ADD_PREPARE:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal2;
				pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstResult->SkillResultVal3;
			}
			break;
		case RESULT_DEC_PREPARE:
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul -= pstResult->SkillResultVal2;
				pstAtkChg->stAtkerHurtChg.AtkerHurtAdd -= pstResult->SkillResultVal3;
			}
			break;
		
		case RESULT_ADD_ATK_TARGET:	// 增加攻击目标
			break;

		case RESULT_ADD_KILL_COLLECT:	// 击杀增加技能积累数获取比率  百分率
			pstAtkProcess->ushKillAddCollectNum += pstResult->SkillResultVal1;
			break;

		case RESULT_ADD_KILL_MP:		// 击杀增加mp获取量
			pstAtkProcess->ushKillAddMpNum += pstResult->SkillResultVal1;
			if (pstPlayer &&pstResult->SkillResultVal2 > 0)
			{
				pstAtkProcess->ushKillAddMpNum += pstResult->SkillResultVal2/100.0*z_get_player_hpmax(pstAppCtx, pstEnv, pstPlayer);
			}
			break;


		/*
		case RESULT_ADD_BULL:		// 
			if (pstPlayer)
			{
				pstPlayer->stOnline.stFitOneline.unBullRate += pstResult->SkillResultVal1;
			}
			break;
	
		case RESULT_ADD_EXP:
			if (pstPlayer)
			{
				pstPlayer->stOnline.stFitOneline.unExpRate += pstResult->SkillResultVal1;
			}
			break;

		case RESULT_ADD_PET_EXP:
			if (pstPlayer)
			{
				pstPlayer->stOnline.stFitOneline.unPetExpRate += pstResult->SkillResultVal1;
			}
			break;

		case RESULT_ADD_SKILL_POINT:
			if (pstPlayer)
			{
				pstPlayer->stOnline.stFitOneline.unSkillPointRate += pstResult->SkillResultVal1;
			}
			break;
		*/

		case RESULT_DESPISE: // 藐视   怪物等级越低 伤害越高               没写完
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if(!pstMonDef)
				{
					return 0;
				}
			}
			break;
		/*
		case RESULT_TASK_ITEM_RATE:
			if (pstPlayer)
			{
				pstPlayer->stOnline.stFitOneline.unTaskItemRate += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_STUFF_ITEM_RATE:
			if (pstPlayer)
			{
				pstPlayer->stOnline.stFitOneline.unStuffItemRate += pstResult->SkillResultVal1;
			}
			break;
		*/
		case RESULT_HEAVY_HIT_VAL:
			pstDyn->stHitRateChg.nHeavyHitVal += pstResult->SkillResultVal1*100;
			break;

		case RESULT_DESPISE_MINI:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_MINI)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DESPISE_WEAK:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_WEAK)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DESPISE_SOFT:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_SOFT)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DESPISE_NORM:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_NORM)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DESPISE_HARD:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_HARD)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DESPISE_XI:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_XI)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DESPISE_JIN:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_JIN)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DESPISE_BOSS:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_BOSS)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;
		case RESULT_DESPISE_JI:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_JI)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			break;	
		case RESULT_DESPISE_MACHINE:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				pstMonDef = z_get_mon_def(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				if (!pstMonDef || pstMonDef->SubType != MON_SUB_TYPE_MACHINE)
				{
					return 0;
				}
				if(pstAtkChg)
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstResult->SkillResultVal1;
			}
			
			break;
		case RESULT_RESET_CD:
			if( pstPlayer)
			{
				player_set_skill_cool_time( pstEnv, pstPlayer, pstResult->SkillResultVal1,pstResult->SkillResultVal2);
			}
			break;
		case RESULT_HEAL_HP:
			pstDyn->stHealChg.nBaseHealHPMul += pstResult->SkillResultVal1;
			pstDyn->stHealChg.nHealHPMul += pstResult->SkillResultVal2;
			pstDyn->stHealChg.unHealHPAdd += pstResult->SkillResultVal3;
			break;
		default:
			break;
	}

	return 0;
}

int z_cacl_skill_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo)
{
	int i;
	SKILLRESULT *pstResult;
	//FITTINGSDEF *pstFittingsDef;
	//FITTINGSGRIDDATA *pstFittingsGridData;
	Player *pstPlayer;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	//CSFITTINGSROW *pstFittings = pstAtkerInfo->pstFittings;
	//ATKCHG *pstAtkChg = NULL;	//comment by paraunused.pl
	
	for (i=0; i<MAX_SKILL_RESULT; i++)
	{
		pstResult = pstSkillDef->SkillResult+i;
		if (RESULT_NONE == pstResult->SkillResultID)
		{
			break;
		}

		z_cacl_skill_result(pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, pstResult);
	}

	for (i=0; i<MAX_TARGET_STATUS; i++)
	{
		if (pstAtkProcess->stAtkerInfo.ucAppendResult[i])
		{
			z_cacl_skill_result(pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, &pstSkillDef->TargetStatus[i].SkillResult);
		}
	}
	
	
	if (OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
	{
		pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;

		/*
		for (i=0; i<MAX_FITTINGS_GRID; i++)
		{
			pstFittingsGridData = &pstFittings->FittingsMap.FittingsGrids[i];
			if (pstFittingsGridData->FittingsID == 0)
			{
				continue;
			}

			pstFittingsDef = z_find_fittings(pstEnv, pstFittingsGridData->FittingsID, pstFittingsGridData->Level, 
				pstFittingsGridData->GroupID, &pstFittingsGridData->DefIdx);
			if (!pstFittingsDef)
			{
				continue;
			}

			for (j=0; j<(int)(sizeof(pstFittingsDef->FittingsResult)/sizeof(pstFittingsDef->FittingsResult[0])); j++)
			{
				pstResult = &pstFittingsDef->FittingsResult[j];
				if (pstResult->SkillResultID == 0)
				{
					break;
				}
				
				z_cacl_skill_result(pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, pstResult);
			}
		}
		*/
		
		//天赋对技能攻击的加强,最终加强在武器身上
		/*
		if (pstAtkProcess->iAtkChgIdx >= 0)
		{
			pstAtkChg = &pstAtkProcess->stAtkerInfo.stDyn.astAtkChg[pstAtkProcess->iAtkChgIdx];
			if (pstAtkChg)
			{
				pstAtkChg->stAtkerHurtChg.AtkerHurtMul += pstAtkerInfo->pstZoneSkill->stSkillMul.AtkerHurtMul;
				pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += pstAtkerInfo->pstZoneSkill->stSkillMul.AtkerHurtAdd;

				genius_atk_random_add_hurt( pstAppCtx, pstEnv, pstPlayer, pstSkillDef->SkillType,pstAtkChg );
			}
		}*/		
	}

	return 0;
}

int z_do_pull(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo, CSONEHIT *pstOneHit, int iDist, int iFreezeTime)
{
	RESPOS stAtkerPos;
	RESPOS stAtkeePos, stAtkeeRealPos;
	RESPOS stNextPos;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	ZONEIDX stIdx;
	Player *pstPlayerAtker = NULL;
	Player *pstPlayerAtkee = NULL;
	Monster *pstMonAtker = NULL;
	Monster *pstMonAtkee = NULL;
	MONSTERDEF *pstMonsterDef;
	int iDir;
	int i;
	TMAPMASK *pstMask;
	int iTmp;
	
	if (OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
	{
		stAtkerPos = pstAtkerInfo->stRolePos;
	}
	else if (OBJ_MONSTER == pstAtkerInfo->stAtker.iType)
	{
		pstMonAtker = pstAtkerInfo->stAtker.stObj.pstMon;
		stAtkerPos = pstMonAtker->stCurrPos;
	}
	else
	{
		return -1;
	}
	
	if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		pstPlayerAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
		stAtkeePos = pstPlayerAtkee->stRoleData.Pos;
	}
	else if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		pstMonAtkee = pstAtkeeInfo->stAtkee.stObj.pstMon;
		stAtkeePos = pstMonAtkee->stCurrPos;
	}
	else
	{
		return -1;
	}
		
	if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		pstPlayerAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
		if (pstPlayerAtkee->stOnline.State & CS_STAT_STALL)
		{
			return -1;
		}

		stIdx.iID = pstPlayerAtkee->stRoleData.Map;
		stIdx.iPos = pstPlayerAtkee->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (!pstMapInst)
		{
			return -1;
		}
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (!pstMapIdx)
		{
			return -1;
		}

	}
	else if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		pstMonAtkee = pstAtkeeInfo->stAtkee.stObj.pstMon;
		
		if (MOVE_TYPE_NO == pstMonAtkee->bMoveType)
		{
			return 0;
		}

		pstMonsterDef = z_get_mon_def(pstEnv, pstMonAtkee);
		if (!pstMonsterDef)
		{
			return -1;
		}	

		iDist = iDist *(1+pstMonsterDef->BackMul/100.0);
		if (0 == iDist)
		{
			return 0;
		}

		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMonAtkee->stMap);
		if (!pstMapInst)
		{
			return -1;
		}
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (!pstMapIdx)
		{
			return -1;
		}

	}
	else
		return -1;

	if (stAtkerPos.X == stAtkeePos.X && stAtkerPos.Y == stAtkeePos.Y && OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
	{
		pstPlayerAtker = pstAtkerInfo->stAtker.stObj.pstPlayer;
		
		if (pstPlayerAtker->stOnline.cLastPathPoints > 0)
			stAtkerPos = pstPlayerAtker->stOnline.stTurnPoints[(pstPlayerAtker->stOnline.cLastPath + pstPlayerAtker->stOnline.cLastPathPoints - 1)%RECORD_TURN];	
		else
			stAtkerPos = pstAtkerInfo->stRolePos;
	}
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	iDir	=	FaceAngleBetween(stAtkerPos.X, stAtkerPos.Y, stAtkeePos.X, stAtkeePos.Y);

	for (i=0; i<3; i++)
	{
		iDist = iDist >> i;
		stNextPos.X		=	(int) (stAtkerPos.X + iDist * cos((iDir)*PI/180.0f));
		stNextPos.Y		=	(int) (stAtkerPos.Y - iDist * sin((iDir)*PI/180.0f));

		if (IsPixelLineThrough(pstMask, stAtkeePos.X, stAtkeePos.Y, stNextPos.X, stNextPos.Y, maskStop)
			&& dyn_msk_check(pstEnv, pstMapInst, pstMask, stAtkeePos.X, stAtkeePos.Y, stNextPos.X, stNextPos.Y, 0,DYN_MSK_STOP)
			&& z_pos_area(pstEnv->pstMapObj, pstMapIdx, &stNextPos)) 
		{
			break;
		}
	}

	if (i==3)
	{
		return -1;
	}

	/*if (stNextPos.X == stAtkeePos.X && stNextPos.Y == stAtkeePos.Y)
	{
		return 0;
	}*/

	if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		if( NULL == pstPlayerAtkee )
		{
			return -1;
		}
		stAtkeeRealPos  = pstPlayerAtkee->stRoleData.Pos;
		
		pstPlayerAtkee->stRoleData.Pos = stNextPos;
		pstPlayerAtkee->stOnline.bDir = (unsigned char) (iDir>>1);
		SET_PLAYER_NOMOVE(pstPlayerAtkee);
		z_player_move_stop(pstAppCtx, pstEnv, pstPlayerAtkee);

		z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayerAtkee->iMemID, &stAtkeeRealPos, 0);
		
		//拉人下马
		if( ( CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE  )& pstPlayerAtkee->stOnline.State)
		{
			z_ride_break(pstAppCtx, pstEnv,  pstPlayerAtkee);
		}

		if(pstPlayerAtkee->stOnline.State & (CS_STAT_PREPARE | CS_STAT_MOVE_PREPARE))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayerAtkee,SKILL_BRK_NORMAL,pstPlayerAtkee->stOnline.stAtkInfo.unSkillID);
		}
		
		z_player_search_view(pstAppCtx, pstEnv,  pstMapInst, &stNextPos, pstPlayerAtkee);

		pstOneHit->HitBackFlag = 1;
		pstOneHit->HitBack.Pos = stNextPos;

		pstPlayerAtkee->stOnline.iPosSeq += 50;
	}
	else
	{
		if( NULL == pstMonAtkee)
		{
			return -1;
		}
		stAtkeeRealPos  = pstMonAtkee->stCurrPos;
		
		pstMonAtkee->stCurrPos = stNextPos;
		pstMonAtkee->cMoving	=	0;
		pstMonAtkee->bDirection		=	(unsigned char) (iDir>>1);

		z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstMonAtkee->iID, &stAtkeeRealPos, 0);

		if (iFreezeTime > 2)
		{
			iTmp = pstEnv->pstAppCtx->stCurr.tv_sec + iFreezeTime;
		}
		else
		{
			iTmp = pstEnv->pstAppCtx->stCurr.tv_sec + 2;
		}

		if (pstMonAtkee->tFreezeEnd < iTmp)
		{
			pstMonAtkee->tFreezeEnd = iTmp;
		}

		if (pstMonAtkee->stMonAtkInfo.cAtkStat == CS_STAT_PREPARE)
		{
			z_mon_skill_break(pstEnv, pstMonAtkee);
		}
		
		z_mon_search_view(pstAppCtx, pstEnv,  pstMapInst, &stNextPos, pstMonAtkee);

		pstOneHit->HitBackFlag = 1;
		pstOneHit->HitBack.Pos = stNextPos;
	}
		
	return 0;
}


int z_do_hitback(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkerInfo *pstAtkerInfo, AtkeeInfo *pstAtkeeInfo, CSONEHIT *pstOneHit, int iRate, int iDist, int iFreezeTime)
{
	RESPOS stAtkerPos;
	RESPOS stAtkeePos, stAtkeeRealPos;
	RESPOS stNextPos,stInter;
	ZONEMAPINST* pstMapInst;
	MapIndex* pstMapIdx;
	ZONEIDX stIdx;
	Player *pstPlayerAtker = NULL;
	Player *pstPlayerAtkee = NULL;
	Monster *pstMonAtkee = NULL;
	MONSTERDEF *pstMonsterDef;
	int iDir;
	int i;
	TMAPMASK *pstMask;
	int iTmp;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	int interNum = 0;
	
	if ((int)(RAND1(10000)) >= iRate*100)
	{
		return 0;
	}

	//被击者的点
	if (pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || 
		pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE)
	{
		stAtkeePos = pstAtkerInfo->stAtkPos;
	}
	else
	{
		if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
		{
			pstPlayerAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
			stAtkeePos = pstPlayerAtkee->stRoleData.Pos;
		}
		else if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
		{
			pstMonAtkee = pstAtkeeInfo->stAtkee.stObj.pstMon;
			stAtkeePos = pstMonAtkee->stCurrPos;
		}
		else
			return -1;
	}

	//攻击者的点
	if (pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || 
		pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE ||
		pstSkillDef->MainRange.RangeType == SKILL_RANGE_ARC ||
		pstSkillDef->MainRange.RangeType == SKILL_RANGE_CIRCLE)
	{
		if (OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
		{
			stAtkerPos = pstAtkerInfo->stRolePos;
		}
		else if (OBJ_MONSTER == pstAtkerInfo->stAtker.iType)
		{
			stAtkerPos = pstAtkerInfo->stAtker.stObj.pstMon->stCurrPos;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		stAtkerPos = pstAtkerInfo->stAtkPos;
	}

	if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		pstPlayerAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
		if (pstPlayerAtkee->stOnline.State & CS_STAT_STALL)
		{
			return -1;
		}

		stIdx.iID = pstPlayerAtkee->stRoleData.Map;
		stIdx.iPos = pstPlayerAtkee->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (!pstMapInst)
		{
			return -1;
		}
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (!pstMapIdx)
		{
			return -1;
		}

	}
	else if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		pstMonAtkee = pstAtkeeInfo->stAtkee.stObj.pstMon;
		
		if (MOVE_TYPE_NO == pstMonAtkee->bMoveType)
		{
			return 0;
		}

		pstMonsterDef = z_get_mon_def(pstEnv, pstMonAtkee);
		if (!pstMonsterDef)
		{
			return -1;
		}

		iDist = iDist *(1+pstMonsterDef->BackMul/100.0);
		if (0 == iDist)
		{
			return 0;
		}

		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMonAtkee->stMap);
		if (!pstMapInst)
		{
			return -1;
		}
		pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
		if (!pstMapIdx)
		{
			return -1;
		}

	}
	else
		return -1;

	if (stAtkerPos.X == stAtkeePos.X && stAtkerPos.Y == stAtkeePos.Y && OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
	{
		pstPlayerAtker = pstAtkerInfo->stAtker.stObj.pstPlayer;
		
		if (pstPlayerAtker->stOnline.cLastPathPoints > 0)
			stAtkerPos = pstPlayerAtker->stOnline.stTurnPoints[(pstPlayerAtker->stOnline.cLastPath + pstPlayerAtker->stOnline.cLastPathPoints-1)%RECORD_TURN];	
		else
			stAtkerPos = pstAtkerInfo->stRolePos;
	}
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	iDir	=	FaceAngleBetween(stAtkerPos.X, stAtkerPos.Y, stAtkeePos.X, stAtkeePos.Y);

	for (i=0; i<3; i++)
	{
		iDist = iDist >> i;
		stNextPos.X		=	(int) (stAtkeePos.X + iDist * cos((iDir)*PI/180.0f));
		stNextPos.Y		=	(int) (stAtkeePos.Y - iDist * sin((iDir)*PI/180.0f));

		if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
		{
			memset(&stInter,0,sizeof(stInter));
			if( 0 <	mon_press_target_dynmsk_pos(pstEnv->pstAppCtx, pstEnv, pstMapInst, 
						&pstAtkeeInfo->stAtkee, &pstAtkeeInfo->stAtkee.stObj.pstMon->stCurrPos, &stNextPos, &stInter,&interNum,1))
			{
				stNextPos = stInter;
			}
		}
		else if(OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
		{
			memset(&stInter,0,sizeof(stInter));
			if( 0 <	mon_press_target_dynmsk_pos(pstEnv->pstAppCtx, pstEnv, pstMapInst, 
						&pstAtkeeInfo->stAtkee, &pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.Pos, &stNextPos, &stInter,&interNum,1))
			{
				stNextPos = stInter;
			}
		}

		if (IsPixelLineThrough(pstMask, stAtkeePos.X, stAtkeePos.Y, stNextPos.X, stNextPos.Y, maskStop) &&
			dyn_msk_check(pstEnv, pstMapInst, pstMask, stAtkeePos.X, stAtkeePos.Y, stNextPos.X, stNextPos.Y, 0,DYN_MSK_STOP) && 
			z_pos_area(pstEnv->pstMapObj, pstMapIdx, &stNextPos)) 
		{
			break;
		}
	}

	if (i==3)
	{
		return -1;
	}

	/*if (stNextPos.X == stAtkeePos.X && stNextPos.Y == stAtkeePos.Y)
	{
		return 0;
	}*/

	if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		if( NULL == pstPlayerAtkee )
		{
			return -1;
		}
		stAtkeeRealPos  = pstPlayerAtkee->stRoleData.Pos;
		
		pstPlayerAtkee->stRoleData.Pos = stNextPos;
		pstPlayerAtkee->stOnline.bDir = (unsigned char) (iDir>>1);
		SET_PLAYER_NOMOVE(pstPlayerAtkee);
		z_player_move_stop(pstAppCtx, pstEnv, pstPlayerAtkee);

		z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayerAtkee->iMemID, &stAtkeeRealPos, 0);

		//击退下马
		if( ( CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE  )& pstPlayerAtkee->stOnline.State)
		{
			z_ride_break(pstAppCtx, pstEnv,  pstPlayerAtkee);
		}

		if(pstPlayerAtkee->stOnline.State & (CS_STAT_PREPARE | CS_STAT_MOVE_PREPARE))
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayerAtkee,SKILL_BRK_NORMAL,pstPlayerAtkee->stOnline.stAtkInfo.unSkillID);
		}

		z_player_search_view(pstAppCtx, pstEnv,  pstMapInst, &stNextPos, pstPlayerAtkee);

		pstOneHit->HitBackFlag = 1;
		pstOneHit->HitBack.Pos = stNextPos;

		pstPlayerAtkee->stOnline.iPosSeq += 50;
	}
	else
	{
		if( NULL == pstMonAtkee )
		{
			return -1;
		}
		stAtkeeRealPos  = pstMonAtkee->stCurrPos;
		
		pstMonAtkee->stCurrPos = stNextPos;
		pstMonAtkee->cMoving	=	0;
		pstMonAtkee->bDirection		=	(unsigned char) (iDir>>1);

		z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstMonAtkee->iID, &stAtkeeRealPos, 0);

		if (iFreezeTime > 0)
		{
			iTmp = pstEnv->pstAppCtx->stCurr.tv_sec + iFreezeTime;
		}
		else
		{
			iTmp = pstEnv->pstAppCtx->stCurr.tv_sec + 1;
		}

		if (pstMonAtkee->tFreezeEnd < iTmp)
		{
			pstMonAtkee->tFreezeEnd = iTmp;
		}

		if (pstMonAtkee->stMonAtkInfo.cAtkStat == CS_STAT_PREPARE)
		{
			z_mon_skill_break(pstEnv, pstMonAtkee);
		}
		
		z_mon_search_view(pstAppCtx, pstEnv,  pstMapInst, &stNextPos, pstMonAtkee);

		pstOneHit->HitBackFlag = 1;
		pstOneHit->HitBack.Pos = stNextPos;
	}
		
	return 0;
}

int z_del_totem(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, 
				Monster *pstTotem)
{
	Monster *pstMonTmp;
	int iTotemID;
	int iFlag = 0;

	if (NULL == pstZoneSkill)
	{
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstTotem->iTotemSkillID);
	}

	//每个技能自己所属的图腾列表
	if (pstZoneSkill)
	{
		iTotemID = pstZoneSkill->iTotemHead;
		while(iTotemID > 0)
		{
			if (iTotemID == pstTotem->iID)
			{
				iFlag = 1;
				break;
			}
			
			pstMonTmp = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTotemID);
			if (NULL == pstMonTmp)
			{
				tlog_error(pstEnv->pstLogCat, 0, 0, "totem %d error", iTotemID);
				return -1;
			}

			iTotemID = pstMonTmp->iTotemNext;

			if (pstMonTmp->iTotemNext == pstTotem->iID	)
			{
				pstMonTmp->iTotemNext = pstTotem->iTotemNext;
			}
		}
		
		if (pstZoneSkill->iTotemHead == pstTotem->iID)
		{
			pstZoneSkill->iTotemHead = pstTotem->iTotemNext;
		}

		if (iFlag==0)
             {
                     tlog_error(pstEnv->pstLogCat, 0, 0,"totem error, Uin %d,skill %d,rune %d,num %d", pstPlayer->stRoleData.Uin, pstZoneSkill->stSkillData.SkillID,pstZoneSkill->stSkillData.RuneSlot,pstZoneSkill->bTotem);
             }
             else
             {
                     pstZoneSkill->bTotem--;
             }
	}
	
	mon_bub_del_owner_info(pstEnv, pstTotem);
	player_totem_info_del(pstEnv, pstPlayer, pstTotem);
	
	return 0;
}

int z_add_totem(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef, 
					Monster *pstMon)
{
	int iTotemID;
	Monster *pstTotem = NULL;

	if (0 == pstSkillDef->TotemLimit)
	{
		pstSkillDef->TotemLimit = 1;
		tlog_error(pstEnv->pstLogCat, 0, 0, "skill %d totemlimit", pstSkillDef->SkillID);
	}
	
	if (pstZoneSkill->bTotem >= pstSkillDef->TotemLimit)
	{
		pstTotem = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstZoneSkill->iTotemHead);
		if (NULL == pstTotem)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "totem %d error", pstZoneSkill->iTotemHead);
			return -1;
		}

		//怪物死亡后 tLeaveMap == 0  表示已经z_monster_leave_map 过了
		if (MONSTER_DIE != pstTotem->cStat || pstTotem->tLeaveMap > 0)
		{
			z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstTotem);
		}
		
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstTotem);
	}

	iTotemID = pstZoneSkill->iTotemHead;
	while(iTotemID > 0)
	{
		pstTotem = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTotemID);
		if (NULL == pstTotem)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "totem %d error", iTotemID);
			return -1;
		}
		iTotemID = pstTotem->iTotemNext;
	}

	if (0 == pstZoneSkill->iTotemHead)
	{
		pstZoneSkill->iTotemHead = pstMon->iID;
	}
	else if(pstTotem)
	{
		pstTotem->iTotemNext = pstMon->iID;
	}

	pstMon->iTotemNext = 0;
	pstMon->iTotemSkillID = pstZoneSkill->stSkillData.SkillID;
	pstZoneSkill->bTotem++;

	if (pstMon->bSubType == MON_SUB_TYPE_TOTEM)
	{
		pstMon->iPlayerTotemNext = pstPlayer->stOnline.iTotemHead;
		pstPlayer->stOnline.iTotemHead = pstMon->iID;
		pstPlayer->stOnline.bTotemNum++;
	}
	if(is_mon_bub(pstMon))
	{
		pstPlayer->stOnline.stBubOnline.iBubID[pstPlayer->stOnline.stBubOnline.iBubNum++] = pstMon->iID;
	}
	
	return 0;
}

int z_do_call_adjust_dir(ZONESVRENV* pstEnv,  SKILLDEF *pstSkillDef,Monster *pstFind,int iFaceAngel)
{
	SKILLRESULT *pstSkillResult = NULL;

	if(!pstSkillDef)
	{
		return -1;
	}
	
	if( MON_SUB_TYPE_XUNLUO == pstFind->bSubType )
	{
		//修正下物理参数
		pstSkillResult = z_get_skill_result(pstSkillDef, RESULT_PHYSICS_PARA_FOR_XUNLUO_MON);
		if(!pstSkillResult)
		{
			return -1;
		}		
		//客户端1度表示两度
		pstFind->bDirection = iFaceAngel/2;
	}
	
	return 0;
	UNUSED(pstEnv);
}

int z_do_call_pos_xunluo_mon(ZONESVRENV* pstEnv, Monster *pstMon, SKILLDEF *pstSkillDef)
{
	RESPOS stNextPos;
	SKILLRESULT *pstSkillResult = NULL;
	float fTime = 0;
	int iFaceAngel = 0;
	memset(&stNextPos,0,sizeof(stNextPos));

	if(!pstSkillDef)
	{
		return -1;
	}

	iFaceAngel = pstMon->bDirection * 2;
	if( MON_SUB_TYPE_XUNLUO == pstMon->bSubType )
	{
		//修正下物理参数
		pstSkillResult = z_get_skill_result(pstSkillDef, RESULT_PHYSICS_PARA_FOR_XUNLUO_MON);
		if(!pstSkillResult)
		{
			return -1;
		}
		
		if(0 != z_refresh_mon_comm_union_data(pstEnv, pstMon, MON_UNION_TYPE_XUNLUO_MON))
		{
			return -1;
		}
		pstMon->stCommUnion.stXunLuoMon.a = pstSkillResult->SkillResultVal1;
		
		fTime = z_move_get_t_time(0,pstSkillResult->SkillResultVal1,pstSkillResult->SkillResultVal2);			
		//计算怪物的初始速度
		pstMon->stMspdInfo.MoveSpdCurr = z_move_get_v_spd(0, pstSkillResult->SkillResultVal1, fTime);
		pstMon->stMspdInfo.NextNum = 0;

		
		stNextPos.X		=	(int) (pstMon->stInitPos.X + pstSkillResult->SkillResultVal2 * cos((iFaceAngel)*PI/180.0f));
		stNextPos.Y		=	(int) (pstMon->stInitPos.Y - pstSkillResult->SkillResultVal2 * sin((iFaceAngel)*PI/180.0f));

		//不做阻挡的判断
			
		//弄成强制移动，并把移动的点计算下
		pstMon->stForcedPos = stNextPos;
		pstMon->bMoveType = MOVE_TYPE_FORCED; // 强制移动
		
	}

	return 0;
}
int z_do_call(ZONESVRENV* pstEnv, Player *pstPlayer, AtkerInfo *pstAtkerInfo, ZONEMAPINST *pstMapInst, int iMonID, 
				int iNum, int iRadius, int iLifeTime, RESPOS *pstPos,SKILLRESULT *pstShapeResult)
{
	int i;
	Monster stMonster;
	MapIndex *pstMapIdx;
	MONSTERDEF *pstMonDef;
	Monster *pstFind;
	RESPOS stPos;
	int iPoint, iFaceAngel = 0;
	SKILLRESULT *pstSkillResult = NULL;
	TMAPMASK *pstMask;
	int iAngelArc = 0;

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	memset(&stMonster, 0, sizeof(stMonster));

	stMonster.cStat = MONSTER_DIE;
	stMonster.iDefIdx = iMonID;
	stMonster.iDefPos = -1;
	stMonster.stMap.iID = stMonster.iInitMap = pstMapInst->iMapInstID;
	stMonster.stMap.iPos=	pstMapInst->iIndex;
	stMonster.iInitMapPos	=	pstMapInst->iMapPos;	
	stMonster.tRefresh	=	pstEnv->pstAppCtx->stCurr.tv_sec;
	if (pstPlayer)
	{
		stMonster.iOwnerID = pstPlayer->iMemID;
		mon_set_pkinfo(&stMonster, MON_PK_TYPE_PLAYER, pstPlayer->iMemID);
		snprintf(stMonster.szOwner,sizeof(stMonster.szOwner),"%s",pstPlayer->stRoleData.RoleName);
		stMonster.szOwner[sizeof(stMonster.szOwner) - 1] = 0;
		
		//设置起始朝向
		iPoint = (pstPlayer->stOnline.cLastPath + pstPlayer->stOnline.cLastPathPoints - 1+RECORD_TURN)%RECORD_TURN;
		iFaceAngel = FaceAngleBetween(pstPlayer->stOnline.stTurnPoints[iPoint].X, pstPlayer->stOnline.stTurnPoints[iPoint].Y, 
									pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y);
		SET_RANPOS_DIR(iFaceAngel, 0, 0);
	}
	
	if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
	{
		pstSkillResult = z_get_skill_result(pstAtkerInfo->pstSkillDef, RESULT_CHG_MAXHP);
		if(pstSkillResult)
		{
			stMonster.iMaxHpbyCall =  pstAtkerInfo->stAtker.stObj.pstMon->iMaxHP *
				(pstSkillResult->SkillResultVal1/100.0);
		}	

		mon_set_camp(&stMonster, pstAtkerInfo->stAtker.stObj.pstMon->iCampIdx);
		
	}
	else if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
	{
		pstSkillResult = z_get_skill_result(pstAtkerInfo->pstSkillDef, RESULT_CHG_MAXHP);
		if(pstSkillResult)
		{
			stMonster.iMaxHpbyCall =  z_get_player_hpmax(pstEnv->pstAppCtx, pstEnv, pstAtkerInfo->stAtker.stObj.pstPlayer) *
				(pstSkillResult->SkillResultVal1/100.0);
		}	
	}
	
	if (pstAtkerInfo->stAtker.iType == OBJ_MONSTER &&
		pstAtkerInfo->stAtker.stObj.pstMon->iOwnerID > 0)
	{
		stMonster.iOwnerID = pstAtkerInfo->stAtker.stObj.pstMon->iOwnerID;
	}
	else if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
	{
		stMonster.iCallMonID = pstAtkerInfo->stAtker.stObj.pstMon->iID;
	}

	pstMonDef = z_get_mon_def(pstEnv, &stMonster);
	if (NULL == pstMonDef)
	{
		return -1;
	}
	
	for (i=0; i<iNum; i++)
	{
		stMonster.bDirection = RAND1(180);
	
		if (!pstShapeResult)
		{
			z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, pstPos, pstPos, iRadius, iRadius, &stPos);
		}
		else
		{
			// 随机分布 val1表示半径
			if (pstShapeResult->SkillResultID == 10000)
			{
				z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, pstPos, pstPos, 
								pstShapeResult->SkillResultVal1, pstShapeResult->SkillResultVal1, &stPos);
			}
			// 矩形
			else if(pstShapeResult->SkillResultID == 10001)
			{
				erea_(i, pstShapeResult->SkillResultVal1, pstShapeResult->SkillResultVal2, 
						pstShapeResult->SkillResultVal3, pstShapeResult->SkillResultVal4, 
						pstPos->X,pstPos->Y, &stPos.X, &stPos.Y);
			}
			// 圆形
			else if(pstShapeResult->SkillResultID == 10002)
			{
				round_(i, pstShapeResult->SkillResultVal1, pstShapeResult->SkillResultVal2, 
						 pstPos->X,pstPos->Y, &stPos.X, &stPos.Y);
			}
			// 扇形
			else if(pstShapeResult->SkillResultID == 10003)
			{
				iAngelArc = FaceAngleBetween(pstPos->X, pstPos->Y,pstAtkerInfo->stAtkPos.X,pstAtkerInfo->stAtkPos.Y);
				arc_(pstEnv, i, pstShapeResult->SkillResultVal1, pstShapeResult->SkillResultVal2, pstShapeResult->SkillResultVal3,
						*pstPos, &stPos,iAngelArc);
			}
			// 指定点
			else if(pstShapeResult->SkillResultID == 10004)
			{
				z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstAtkerInfo->stAtkPos, &pstAtkerInfo->stAtkPos, 0, 0, &stPos);
				if(pstShapeResult->SkillResultVal1)
				{
					if( pstPlayer)
					{
						iFaceAngel = FaceAngleBetween(pstPlayer->stRoleData.Pos.X, pstPlayer->stRoleData.Pos.Y,
							pstAtkerInfo->stAtkPos.X, pstAtkerInfo->stAtkPos.Y);
						
						iFaceAngel = (iFaceAngel+90)%360;
						
						stMonster.bDirection = iFaceAngel/2;
					}
				}
				
			}
			else if (pstShapeResult->SkillResultID == 10005)
			{
				z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, pstPos, pstPos, 
								pstShapeResult->SkillResultVal1, 0, &stPos);
			}
			else
			{
				return -1;
			}

			if (!z_valid_pos(pstEnv->pstMapObj, pstMapInst->iMapInstID, NULL, &stPos)) 
			{
				z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, pstPos, pstPos, iRadius, iRadius, &stPos);
			}
			
			
		}
		
		stMonster.stInitPos = stPos;
		
		pstFind =	z_new_monster(pstEnv->pstAppCtx, pstEnv, &stMonster, pstMonDef);
		if( !pstFind )
			return -1;

		if (pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
		{
			iFaceAngel = FaceAngleBetween(pstAtkerInfo->stAtker.stObj.pstMon->stCurrPos.X, 
							pstAtkerInfo->stAtker.stObj.pstMon->stCurrPos.Y,
							pstAtkerInfo->stAtkPos.X, pstAtkerInfo->stAtkPos.Y);
		}

		//把巡逻怪的方向先确定下
		z_do_call_adjust_dir(pstEnv,  pstAtkerInfo->pstSkillDef, pstFind, iFaceAngel);
	
		pstFind->cFirstBorn = 1;

		if(pstMonDef->AtkMulByCaller > 0 && pstPlayer)
		{
			pstFind->stMonDynAttr.Level = pstPlayer->stRoleData.Level;
		}
		
		//防止野外刷怪物
		if(iLifeTime <= 0)
		{
			iLifeTime = 300;
		}
		pstFind->tLifeEnd = pstEnv->pstAppCtx->stCurr.tv_sec + iLifeTime;
		if (0 > z_refresh_mon(pstEnv->pstAppCtx, pstEnv, pstFind,pstAtkerInfo->pstSkillDef))
		{
			tmempool_free(pstEnv->pstMonsterPool, pstFind->iMIndex);
			return -1;
		}
		pstFind->cFirstBorn = 0;

		if (pstAtkerInfo->stAtker.iType == OBJ_MONSTER &&
			pstAtkerInfo->stAtker.stObj.pstMon->stBctInfo.iBctDefID > 0)
		{
			pstFind->stBctInfo.iBctDefID = pstAtkerInfo->stAtker.stObj.pstMon->stBctInfo.iBctDefID;
		}
		
		if( MON_SUB_TYPE_SKILL_DESTORY == pstFind->bSubType ||
			MON_SUB_TYPE_SKILL_NODESTORY == pstFind->bSubType)
		{
			double iMoveTime = 0;
			RESPOS stNextPos;
			RESPOS stMskPos;
			int dDistance = 0;
			if(pstFind->tLifeEnd > pstEnv->pstAppCtx->stCurr.tv_sec)
			{
				iMoveTime = pstFind->tLifeEnd - pstEnv->pstAppCtx->stCurr.tv_sec;
			}	
	
			dDistance = pstFind->stMspdInfo.MoveSpdCurr*iMoveTime;
			iFaceAngel = FaceAngleBetween(pstPos->X, pstPos->Y,stPos.X,stPos.Y);
			stNextPos.X		=	(int) (pstFind->stInitPos.X + dDistance * cos(iFaceAngel*PI/180.0f));
			stNextPos.Y		=	(int) (pstFind->stInitPos.Y - dDistance * sin(iFaceAngel*PI/180.0f));
			
			//获取阻挡的最进一个点
			pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask +pstMapIdx->iMaskOrigin);
			map_check_lineseg_endpos(pstMask,&pstFind->stInitPos,&stNextPos);
			if(!IsPixelLineThrough3(pstMask, pstFind->stInitPos.X, pstFind->stInitPos.Y,stNextPos.X,stNextPos.Y,
								maskStop|maskLoMagic,&stMskPos.X,&stMskPos.Y))
			{
				stNextPos = stMskPos;
			}

			//弄成强制移动，并把移动的点计算下
			map_check_safe_pos( pstMask,&stNextPos);
			pstFind->stForcedPos = stNextPos;
			pstFind->bMoveType = MOVE_TYPE_FORCED;
			mon_ai_move(pstEnv,  pstFind);
			
		}

		//结界加血的时候要用到
		if(pstPlayer)
		{
			DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;
			
			pstFind->iOwnerMagic = z_cacl_attr_atk(&pstDyn->stAtkChg, SKILL_ATTR_ICE);
		}
		
		if(pstPlayer)
		{	
			DYNATTRLIST *pstDyn = NULL;
			int iPhsicAtk = 0;
			int iIceAtk = 0;
			int iIceDef = 0;
			int iPhsicDef = 0;

			if(pstFind->bSubType == MON_SUB_TYPE_TOTEM)
			{
				z_add_totem(pstEnv, pstPlayer, pstAtkerInfo->pstZoneSkill, pstAtkerInfo->pstSkillDef, pstFind);
			}
			
			// 对召唤出来的图腾做一个怪物伤害的修正
			// 怪物的物理攻击+人物物理攻击*修正； 法术相同
			pstDyn = &pstPlayer->stOnline.stDyn;
			iPhsicAtk = (pstDyn->stAtkChg.unPhsicBaseArmAtk+pstDyn->stAtkChg.iPhsicMinAtk)*(1+pstDyn->stAtkChg.nPhsicAtkChgMul/100.0)+pstDyn->stAtkChg.iPhsicAtkChgAdd;
			iIceAtk = (pstDyn->stAtkChg.unIceBaseArmAtk+pstDyn->stAtkChg.iIceMinAtk)*(1+pstDyn->stAtkChg.nIceAtkChgMul/100.0)+pstDyn->stAtkChg.iIceAtkChgAdd;
			iIceDef = pstDyn->stDefChg.unIceDefBaseAdd*(1+pstDyn->stDefChg.nIceDefChgMul/100.0)+pstDyn->stDefChg.iIceDefChgAdd;
			iPhsicDef = pstDyn->stDefChg.unPhsicDefBaseAdd*(1+pstDyn->stDefChg.nPhsicDefChgMul/100.0)+pstDyn->stDefChg.iPhsicDefChgAdd;	
			
			//借用这个寄存下,不计算抗性的攻击,NB的攻击
			pstFind->stMonDynAttr.IceAtkAdd += iIceAtk*( pstMonDef->AtkMulByCaller/100.0);
			pstFind->stMonDynAttr.PhsicAtkAdd += iPhsicAtk*( pstMonDef->AtkMulByCaller/100.0 );

			pstFind->iCallIceDefAdd = iIceDef*( pstMonDef->AtkMulByCaller/100.0);
			pstFind->iCallPhsicDefadd = iPhsicDef*(pstMonDef->AtkMulByCaller/100.0);
		}
		
	}

	return 0;
}

int z_do_revive(ZONESVRENV* pstEnv, Player *pstPlayerReviver, Player *pstPlayerRevivee, SKILLDEF *pstSkillDef, SKILLRESULT *pstResult)
{
	CSPKG stPkg;
	CSSKILLREVIVEINFO *pstSkillReviveInfo = &stPkg.Body.SkillReviveInfo;
	
	pstPlayerRevivee->stOnline.iReviverID = pstPlayerReviver->iMemID;
	pstPlayerRevivee->stOnline.iReviverSkillID = pstSkillDef->SkillID;
	pstPlayerRevivee->stOnline.iReviverHP = (pstResult->SkillResultVal1/100.0)*pstPlayerRevivee->stOnline.stDyn.stMidAttrList.uiHPMax + pstResult->SkillResultVal2;
	pstPlayerRevivee->stOnline.iReviverMoney = pstPlayerRevivee->stRoleData.MiscInfo.DieInfo.DieDrop.Money*pstResult->SkillResultVal3/100.0;
	pstPlayerRevivee->stOnline.iReviverExp = pstPlayerRevivee->stRoleData.MiscInfo.DieInfo.DieDrop.Exp*pstResult->SkillResultVal4/100.0;
	pstPlayerRevivee->stOnline.stReviverPos = pstPlayerReviver->stRoleData.Pos;
		
	STRNCPY(pstSkillReviveInfo->RoleName, pstPlayerReviver->stRoleData.RoleName, sizeof(pstSkillReviveInfo->RoleName));
	pstSkillReviveInfo->SkillID = pstSkillDef->SkillID;
	pstSkillReviveInfo->PlayerID = pstPlayerReviver->iMemID;
	pstSkillReviveInfo->DropMoney = pstPlayerRevivee->stRoleData.MiscInfo.DieInfo.DieDrop.Money;
	pstSkillReviveInfo->ReviveMoney = pstPlayerRevivee->stOnline.iReviverMoney;
	pstSkillReviveInfo->DropExp= pstPlayerRevivee->stRoleData.MiscInfo.DieInfo.DieDrop.Exp;
	pstSkillReviveInfo->ReviveExp = pstPlayerRevivee->stOnline.iReviverExp;
	

	Z_CSHEAD_INIT(&stPkg.Head, SKILL_REVIVE_INFO);
	z_cltmsg_send(pstEnv, pstPlayerRevivee, &stPkg);
	
	return 0;
}
//怪物攻击只去1点血
int z_mon_dec_one_hp_by_hit(ZONESVRENV* pstEnv,Monster* pstMon)
{
	MONSTERDEF *pstMonsterDef;

	pstMonsterDef = z_get_mon_def(pstEnv, pstMon);
	if(!pstMonsterDef)
	{
		return -1;
	}

	if(pstMonsterDef->MiscType&MON_MISC_TYPE_HIT_ONE_HP)
	{
		return 0;
	}

	return -1;
}
int z_status_val(ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, ZoneStatus *pstZoneStatus)
{
	int i;
	STATUSDEF *pstStatusDef=NULL;
	int iAtk = 0;
	int iAtkMul = 0;
	int iDef = 0;
	ATKCHG *pstAtkChg = NULL;
	int iTotalChg = 0;
	int iMaxHp= 0;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;

	pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
	if (NULL == pstStatusDef)
	{
		return -1;
	}
	
	pstAtkChg = &pstAtkProcess->stAtkerInfo.stDyn.stAtkChg;
	if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		iMaxHp = z_get_player_hpmax(pstEnv->pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer);
	}
	else if(OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		iMaxHp = pstAtkeeInfo->stAtkee.stObj.pstMon->iMaxHP;
	}
	
	if(pstZoneStatus->iJumpNum<=0)
	{
		pstZoneStatus->iJumpNum = 1;
	}
	
	/*if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
	{
 		pstZoneStatus->cApplyRelation = z_mon_filter_relation(pstEnv->pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, 
								&pstAtkerInfo->stAtker, pstAtkerInfo->pstSkillDef);
	}
	else
	{
		pstZoneStatus->cApplyRelation = z_player_filter_relation(pstEnv->pstAppCtx, pstEnv,  pstAtkeeInfo->stAtkee.stObj.pstPlayer,
						&pstAtkerInfo->stAtker, pstAtkerInfo->pstSkillDef);
	}*/
	if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
	{
 		pstZoneStatus->cApplyRelation = z_mon_filter_relation(pstEnv->pstAppCtx, pstEnv, pstAtkerInfo->stAtker.stObj.pstMon,
									&pstAtkeeInfo->stAtkee, pstAtkerInfo->pstSkillDef);
	}
	else if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
	{
		pstZoneStatus->cApplyRelation = z_player_filter_relation(pstEnv->pstAppCtx, pstEnv,  pstAtkerInfo->stAtker.stObj.pstPlayer,
								&pstAtkeeInfo->stAtkee,pstAtkerInfo->pstSkillDef);
	}
	else if(pstAtkerInfo->stAtker.iType == OBJ_PET &&
		    pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner )
	{
		pstZoneStatus->cApplyRelation = z_player_filter_relation(pstEnv->pstAppCtx, pstEnv,  pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner,
								&pstAtkeeInfo->stAtkee,pstAtkerInfo->pstSkillDef);
	}
	else
	{
		return -1;
	}
	
	for (i=0; i<MAX_STATUS_RESULT; i++)
	{
		if (0 == pstStatusDef->Result[i].ResultID)
		{
			break;
		}

		switch(pstStatusDef->Result[i].ResultID)
		{
			case STATUS_RESULT_HPADD:
				pstZoneStatus->iVal = iMaxHp*(pstStatusDef->Result[i].ResultVal1/100.0)+pstStatusDef->Result[i].ResultVal2;
				break;
			case STATUS_RESULT_HPDEC:
				pstZoneStatus->iVal = iMaxHp*(pstStatusDef->Result[i].ResultVal1/100.0) + pstStatusDef->Result[i].ResultVal2;
				break;
			case STATUS_RESULT_ADD_HP_FUNC_MUL:
				if( !pstZoneStatus->cApplyRelation )
				{
					// 每次状态回血值 =（（X % * A + Y）* (1+受方治疗状态加成%) +受方治疗状态加成值）/ (T /3)
					// X为系数，Y为常数，A为状态来源魔法攻击力
					iAtk = z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_ICE);		
					iTotalChg = pstStatusDef->Result[i].ResultVal2;
					
					pstZoneStatus->iVal = (((pstStatusDef->Result[i].ResultVal1/100.0 * iAtk + iTotalChg) *
							(1 + pstAtkeeInfo->stDyn.stHealChg.nAtkeeHealHPMul/100.0) + pstAtkeeInfo->stDyn.stHealChg.unAtkeeHealHPAdd))
							/ pstZoneStatus->iJumpNum;	
				}
				break;
			case STATUS_RESULT_DEC_HP_FUNC_MUL:
			case STATUS_RESULT_DOT_ON_WALK:	
			case STATUS_RESULT_STEP_DEC_HP:	
				if( pstZoneStatus->cApplyRelation )
				{
					// 每次状态减血值 = (X% *( A *攻击系数 / (防御 + 防御系数) + Y))  / T * 3
					//Y为减血基础值，X为系数， A为状态来源主属性对应攻击力
					iAtk = z_cacl_attr_atk(pstAtkChg, pstAtkProcess->stAtkerInfo.pstSkillDef->MainAttr);
					iAtkMul = pstAtkChg->iAtkLevelMul;
					if(iAtkMul <= 0)
					{
						iAtkMul = 1;
					}
					iDef = z_cacl_attr_def(&pstAtkeeInfo->stDyn.stDefChg, pstAtkProcess->stAtkerInfo.pstSkillDef->MainAttr);
					iTotalChg = pstStatusDef->Result[i].ResultVal2;

					if (pstStatusDef->Result[i].ResultID == STATUS_RESULT_STEP_DEC_HP)
					{
						pstZoneStatus->iVal = iAtk*1.0*iAtkMul / (iDef + pstAtkerInfo->stDyn.stDefChg.iDefLevelMul);
					}
					else
					{
						pstZoneStatus->iVal = (pstStatusDef->Result[i].ResultVal1/100.0 *
											(iAtk*1.0*iAtkMul / (iDef + pstAtkerInfo->stDyn.stDefChg.iDefLevelMul)+iTotalChg))
											/ pstZoneStatus->iJumpNum;
					}
					
					{
						int iHP, iHurtMul;
						iHP = pstZoneStatus->iVal;
						iHurtMul = atker_hurt_mul_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
						iHP = iHP*iHurtMul/100.0;

						iHurtMul = arm_hurt_mul_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
						iHP = iHP*(10000+iHurtMul)/10000.0;

						if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER && pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PLAYER)
						{
							iHurtMul = player_career_hurt_mul_get(pstEnv, 
								pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer, pstAtkeeInfo->stAtkee.stObj.pstPlayer);
							iHP = iHP*(100 + iHurtMul)/100.0;

							// t天赋影响最终伤害
							iHurtMul = genius_hurt_mul_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
							iHP = iHP*(10000+iHurtMul)/10000.0;

							// 防御天赋
							iHurtMul = genius_hurt_dec_get(pstEnv, &pstAtkProcess->stAtkerInfo, pstAtkeeInfo);
						    	iHP = iHP*(10000+iHurtMul)/10000.0;
						}
						

						pstZoneStatus->iVal = iHP;
					}
					
					if(pstZoneStatus->iVal == 0)
					{
							pstZoneStatus->iVal = -1;
					}
				}
				break;
			
			default:
				break;
		}
	}	

	if(OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		if( 0<=z_mon_dec_one_hp_by_hit(pstEnv,pstAtkeeInfo->stAtkee.stObj.pstMon) )
		{
			pstZoneStatus->iVal = -1;
		}
	}
	return 0;
}

int z_player_goto_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos)
{
	ZONEIDX stIdx;
	MapIndex *pstMapIdx = NULL;
	ZONEMAPINST* pstMapInst = NULL;
	ITEMDEF *pstItemDef;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	if (0 < pstRoleTmpSys->TmpItemID)
	{
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (pstItemDef && pstItemDef->ItemTmp.Type == ITEM_TMP_MACHINE)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL8);
			return -1;
		}
	}
	
	if (pstPlayer->stOnline.State &  CS_STAT_STALL)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL9);
		return -1;
	}

	if (pstPlayer->stOnline.State &  CS_STAT_DEAD)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL10);
		return -1;
	}

	if (pstPlayer->stOnline.stExchgOnline.bExchgStat != EXCHG_NO)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL11);
		return -1;
	}

	if (player_is_zuolao(pstPlayer))
	{
		if(g_SpousePos == 1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING79);
			return -1;
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL12);
			return -1;
		}
	}

	if (pstPlayer->stOnline.State&CS_STAT_AUTOFLY)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL13);
		return -1;
	}

	if (is_player_ride_on_machine(pstPlayer))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL14);
		return -1;
	}

	if((pstPlayer->stOnline.State & (CS_STAT_RIDE_ON |CS_STAT_FLY)) && 
		pstPlayer->stOnline.stRideOnInfo.iPassengerNum > 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL15);
		return -1;
	}

	if((CS_STAT_HITCH_RIDE & pstPlayer->stOnline.State))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL16);
		return -1;
	}
	
	if (pstPos == NULL || iMap <= 0)
	{
		return 0;
	}
	
	if (iMap >= PWORLD_MAPID_BASE)
	{
		iMap	=   (pstPlayer->stRoleData.Map & 0xffff0000) | iMap;
	}

	if (player_trans_check(pstEnv, pstPlayer, iMap, 1) < 0)
	{
		return -1;
	}

	stIdx.iID	 =	iMap;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst( pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}

	if (NULL == z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstPos))
	{
		return -1;
	}

	if (0 == z_valid_pos(pstEnv->pstMapObj, iMap, &pstMapInst->iMapPos, pstPos))
	{
		return -1;
	}

	return 0;
}

int z_player_goto(ZONESVRENV* pstEnv, Player *pstPlayer, int iMap, RESPOS *pstPos)
{
	ZONEIDX stIdx;
	MapIndex *pstMapIdx = NULL;
	ZONEMAPINST* pstMapInst = NULL;
	
	if (iMap >= PWORLD_MAPID_BASE)
	{
		iMap	=   (pstPlayer->stRoleData.Map & 0xffff0000) | iMap;
	}

	stIdx.iID	 =	iMap;
	stIdx.iPos =	pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst( pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}
	
	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (!pstMapIdx)
	{
		return -1;
	}

	if (NULL == z_pos_area_dyn(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstPos))
	{
		return -1;
	}

	if (0 == z_valid_pos(pstEnv->pstMapObj, iMap, &pstMapInst->iMapPos, pstPos))
	{
		return -1;
	}

	return z_player_change_map( pstEnv->pstAppCtx, pstEnv, pstPlayer, iMap, pstPos, RAND1(180), 0);	
}

int z_del_unique_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iTagAniID, int iStatusID)
{
	int iIndex;
	char cType;
	Player *pstOtherPlayer;
	Monster *pstMon;
	
	iIndex	=	z_id_indextype(pstAppCtx, pstEnv, iTagAniID, &cType);
	if (iIndex == -1)
	{
		return 0;
	}

	switch(cType)
	{
		case OBJ_PLAYER:
			pstOtherPlayer	=	z_id_player(pstAppCtx, pstEnv, iTagAniID);
			if (pstOtherPlayer)
			{
				z_player_del_status( pstAppCtx, pstEnv, pstOtherPlayer, iStatusID, 1 , pstPlayer->iMemID);
			}
			break;
		case OBJ_MONSTER:
			pstMon	=	z_id_monster(pstAppCtx, pstEnv, iTagAniID);
			if (pstMon)
			{
				if (0 == z_mon_del_status(pstAppCtx, pstEnv, pstMon, iStatusID, pstPlayer->iMemID))
				{
					z_mon_send_status(pstAppCtx, pstEnv, pstMon);
				}
			}
			break;
		default:
			break;
	}

	return 0;
}

int z_checked_mark_target(ZONESVRENV *pstEnv, ZoneAni *pstAtker,
                          ZoneAni *pstAtkee, STATUSDEF *pstStatusDef, int iTime)
{
	AIEvt stAIEvt;
	bzero(&stAIEvt, sizeof(stAIEvt));

	stAIEvt.iEvtID = AI_EVT_MARK;
	if(pstAtker->iType != OBJ_MONSTER ||
	   !z_get_status_result(pstStatusDef, STATUS_RESULT_HATRED_MARK))
	{
		return -1;
	}

	stAIEvt.stEvtDetail.stMark.stAtkee = *pstAtkee;
	stAIEvt.stEvtDetail.stMark.iTime = iTime;
	return mon_ai_stat(pstEnv, pstAtker->stObj.pstMon, &stAIEvt);
}

//核实是否是指定的目标反射状态
int z_check_reflect_status(STATUSDEF *pstStatusDef,int iTargetCur)
{
	int iApplyID = 0;
	STATUSRESULT *pstStatusResult = NULL;
	
	pstStatusResult = z_get_status_result(pstStatusDef, STATUS_RESULT_REFLECTION);
	if( pstStatusResult &&
		pstStatusResult->ResultVal3 == 1)
	{
		//怪物Status效果,反射一定比例的伤害,ResultVal3 0表示默认反射,1表示指定一个目标反射 
		//获取怪物当前的攻击目标的memid
		if( iTargetCur > 0)
		{
			iApplyID = iTargetCur;
		}
	}

	return iApplyID;
}
		
int z_calc_apply_status(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
                        AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, 
						int iStatus, int iStatusLevel, int iLast/*,
						COMBAT_CALC_FROM eFrom*/)
{
	int iStatusAddTime = 0;
	int iStatusRate = 0;
	int  iApplyID = 0;
	Player *pstPlayer;
	Monster *pstMon;
	STATUSDEF *pstStatusDef;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit-1];
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;

	/*
	if(eFrom == FROM_SKILL)
	{
		if (pstAtkerInfo->pstFittings)
		{
			iStatusAddTime = pstAtkerInfo->pstFittings->StatusAddTime;
		}
	}
	*/
	 	
	pstStatusDef = z_find_status(pstEnv, iStatus, iStatusLevel);
	if (NULL == pstStatusDef)
	{
		return -1;
	}

	if (OBJ_PLAYER == pstAtkerInfo->stAtker.iType && pstStatusDef->Type == STATUS_TYPE_BAD)
	{
		iStatusRate =  pstAtkerInfo->stAtker.stObj.pstPlayer->stOnline.stDyn.stHealChg.ucBadStatusRate;
	}
	
	if (OBJ_PLAYER == pstAtkerInfo->stAtker.iType )
	{
		iApplyID =  pstAtkerInfo->stAtker.stObj.pstPlayer->iMemID;
	}
	else if (OBJ_PET == pstAtkerInfo->stAtker.iType)
	{
		iApplyID = pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner->iMemID;
	}
	else if(OBJ_MONSTER == pstAtkerInfo->stAtker.iType)
	{
		iApplyID = pstAtkerInfo->stAtker.stObj.pstMon->iID;
	}

	if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		int iStatusDecTime = 0;
		CSMSPDINFO *pstMspdInfo;
		int iMspd;
		ROLETMPSYS *pstRoleTmpSys;
		int iAppStatusFlag = 1;
		
		pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
		pstMspdInfo = &pstPlayer->stOnline.stMspdInfo;
		iMspd = pstPlayer->stOnline.stMspdInfo.MoveSpdCurr;

		pstRoleTmpSys= &pstPlayer->stRoleData.MiscInfo.TmpSys;
		iStatusDecTime = pstPlayer->stOnline.stDyn.stHealChg.unBadStatusDecTime;
			
		iStatusRate += pstStatusDef->PlayerRate;

		//这个是不良状态的抗性,所以一定要是坏状态
		if(pstStatusDef->Type == STATUS_TYPE_BAD)
		{
			if (iStatusRate > pstPlayer->stOnline.stDyn.stHealChg.ucBadStatusDefRate)
			{
				iStatusRate = iStatusRate - pstPlayer->stOnline.stDyn.stHealChg.ucBadStatusDefRate;
			}
			else
			{
				iStatusRate= 0;
			}
		}
		
		if (pstStatusDef->PlayerRate > 0 && (int)(RAND1(10000)) >= iStatusRate*100.0)
		{
			return -1;
		}	

		if (iStatusDecTime >= iLast+iStatusAddTime)
		{
			return -1;
		}

		// 被攻击的玩家如果是在临时机械上,要限定相关状态
		if (pstRoleTmpSys->TmpItemID > 0)
		{
			ITEMDEF *pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
			if (pstItemDef && pstItemDef->ItemTmp.Type == ITEM_TMP_MACHINE)
			{
				if (pstStatusDef->StatusID != STATUS_FOOD_RESTORE_HP && 
					pstStatusDef->StatusID != STATUS_FOOD_RESTORE_MP &&
					pstStatusDef->StatusID != STATUS_BEACH_ID1 &&
					pstStatusDef->StatusID != STATUS_BEACH_ID2 &&
					pstStatusDef->StatusID != STATUS_BEACH_ID3 &&
					pstStatusDef->StatusID != STATUS_BEACH_ID4 &&
					pstStatusDef->StatusID != 3210)
				{
					iAppStatusFlag = 0;
				}
			}
		}

		if (iAppStatusFlag && 0 == z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, iStatus, 
			iStatusLevel, iLast+iStatusAddTime-iStatusDecTime, 0, iApplyID))
		{
			ZoneStatus *pstZoneStatus;
			
			
			pstZoneStatus = z_player_find_status_by_level(pstPlayer, iStatus,iStatusLevel);
			if (pstZoneStatus)
			{
				z_status_val(pstEnv, pstAtkProcess,pstAtkeeInfo,pstZoneStatus);
			}
			z_checked_mark_target(pstEnv, &pstAtkProcess->stAtkerInfo.stAtker,
			                      &pstAtkeeInfo->stAtkee, pstStatusDef,
			                      iLast + iStatusAddTime - iStatusDecTime);

			//这个函数之后不要用pstZoneStatus了,有内存操作
			if (pstZoneStatus)
			{
				z_player_check_hp_chunnel_status(pstEnv,pstPlayer,*pstZoneStatus);
			}
			pstOneHit->StatusFlag = 1;
			//z_get_player_csstatusinfo(pstAppCtx, &pstOneHit->Status, &pstPlayer->stOnline.stStatusList);

		}

		//速度减慢、恐惧则下马
		if( (iMspd > pstMspdInfo->MoveSpdNext.MoveSpd && pstMspdInfo->NextNum) || 
			z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_RANDOM_RUN))
		{
			if( ( CS_STAT_RIDE_ON | CS_STAT_FLY | CS_STAT_HITCH_RIDE | CS_STAT_RIDE_PREPARE ) & pstPlayer->stOnline.State)
			{
				z_ride_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
			}
		}
	}
	else if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;

		if(is_mon_machine(pstMon))
		{
			if (pstStatusDef->MachineRate > 0 && (int)(RAND1(10000)) >= (pstStatusDef->MachineRate+iStatusRate)*100.0)
			{
				return -1;
			}
		}
		else if (pstMon->bSubType <= MON_SUB_TYPE_NORM)
		{
			if (pstStatusDef->NormRate > 0 && (int)(RAND1(10000)) >= (pstStatusDef->NormRate+iStatusRate)*100.0)
			{
				return -1;
			}
		}
		else if (pstMon->bSubType == MON_SUB_TYPE_JIN)
		{
			if (pstStatusDef->JinYinRate > 0 && (int)(RAND1(10000)) >= (pstStatusDef->JinYinRate+iStatusRate)*100.0)
			{
				return -1;
			}
		}
		else if (pstMon->bSubType == MON_SUB_TYPE_XI || 
			pstMon->bSubType == MON_SUB_TYPE_BOSS ||
			pstMon->bSubType == MON_SUB_TYPE_HARD
			/*pstMon->bSubType == MON_SUB_TYPE_JI*/)
		{
			if (pstStatusDef->BossRate > 0 && (int)(RAND1(10000)) >= (pstStatusDef->BossRate+iStatusRate)*100.0)
			{
				return -1;
			}
		}
		
		if (0 == z_mon_apply_status(pstAppCtx, pstEnv, pstMon, iStatus, 
			iStatusLevel, iLast+iStatusAddTime, 0, iApplyID))
		{
			ZoneStatus *pstZoneStatus;
			
			pstZoneStatus = z_mon_find_status_by_level(pstMon, iStatus,iStatusLevel);
			if (pstZoneStatus)
			{
				z_status_val(pstEnv, pstAtkProcess,pstAtkeeInfo, pstZoneStatus);
			}

			z_checked_mark_target(pstEnv, &pstAtkProcess->stAtkerInfo.stAtker,
			                      &pstAtkeeInfo->stAtkee, pstStatusDef,
			                      iLast + iStatusAddTime);

			//这个函数之后不要用pstZoneStatus了,有内存操作
			if (pstZoneStatus)
			{
				z_mon_check_hp_chunnel_status(pstEnv,pstMon,*pstZoneStatus);
			}
			pstOneHit->StatusFlag = 1;
			//z_get_mon_csstatusinfo(pstAppCtx, &pstOneHit->Status, &pstMon->stStatusList);
		}
	}
	else if(OBJ_PET == pstAtkeeInfo->stAtkee.iType)
	{
		ROLEPET *pstRolePet;
		
		pstRolePet = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet;
		pstPlayer = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstOwner;
		
		if (0 == z_pet_apply_status(pstAppCtx, pstEnv, pstPlayer, pstRolePet, iStatus, 
			iStatusLevel, iLast, 0))
		{
			PetFightShiOnline *pstPetFightShiOnline;

			pstPetFightShiOnline = pet_find_fight_online(pstPlayer, pstRolePet);
			if(NULL == pstPetFightShiOnline)
			{
				return -1;
			}

			pstOneHit->StatusFlag = 1;
			//z_get_pet_csstatusinfo(pstAppCtx, &pstOneHit->Status, &pstPetFightShiOnline->stStatusInfo);

			z_checked_mark_target(pstEnv, &pstAtkProcess->stAtkerInfo.stAtker,
			                      &pstAtkeeInfo->stAtkee, pstStatusDef, iLast);
		}
	}
	else
		return -1;

	return 0;
}

int player_result_team_add_xp(ZONESVRENV* pstEnv,Player *pstPlayer,SKILLRESULT *pstResult)
{

	ZoneTeam *pstZoneTeam = player_team_get(pstEnv, pstPlayer);
	int iAddXp;
	int i;
	Player *pstTeamMember;
	MIDATTRLIST *pstMidAttrList;

	pstMidAttrList = &pstPlayer->stOnline.stDyn.stMidAttrList;
	iAddXp = pstMidAttrList->uiXpMax * pstResult->SkillResultVal1 + pstResult->SkillResultVal2;
	player_add_xp(pstEnv, pstPlayer, iAddXp, 0,1);

	if (!pstZoneTeam)
	{
		return 0;
	}
	
	for (i=0; i<pstZoneTeam->stTeamInfo.MemberNum; i++)
	{
		pstTeamMember = z_name_player(pstEnv->pstAppCtx,pstEnv,pstZoneTeam->stTeamInfo.TeamMember[i].MemberName);
		if (!pstTeamMember)
		{
			continue;
		}

		if (pstTeamMember == pstPlayer)
		{
			continue;
		}

		if(pstResult->SkillResultVal3 >0 &&
			0 > z_check_op_dist(pstEnv, pstPlayer, pstTeamMember->stRoleData.Map, 
			&pstTeamMember->stRoleData.Pos,pstResult->SkillResultVal3))
		{
			continue;
		}

		pstMidAttrList = &pstTeamMember->stOnline.stDyn.stMidAttrList;
		iAddXp = pstMidAttrList->uiXpMax * pstResult->SkillResultVal1 + pstResult->SkillResultVal2;
		player_add_xp(pstEnv, pstTeamMember, iAddXp, 0,1);
		
	}
	
	return 0;
}

// 使用技能冷却所有技能 不包括自己
int player_result_flush_skill(ZONESVRENV* pstEnv, Player *pstPlayer, int iFlushSkillID)
{
	int i;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	ZoneSkill *pstZoneSkill = NULL;
	CSPKG stPkg;
	CSSKILLINFO *pstSkillInfo = &stPkg.Body.SkillInfo;

	UNUSED(&iFlushSkillID);

	for (i =0; i < pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		
		if (pstZoneSkill->stSkillData.SkillID <= 5000 || 
			(pstZoneSkill->stSkillData.SkillID >= 7000 && pstZoneSkill->stSkillData.SkillID <= 7003))
		{
			TV_CLONE(pstZoneSkill->stCoolEnd, pstEnv->pstAppCtx->stCurr);
		}
	}

	pstSkillInfo->Type = SKILL_COOL_CLEAN;
	pstSkillInfo->Data.SkillCool.SkillType = SKILL_COOL_CAREER;
	pstSkillInfo->Data.SkillCool.SkillID = 0;

	Z_CSHEAD_INIT(&stPkg.Head, SKILL_INFO);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

	return 0;
}

// 怪物直接归西，且不分配战利品
int z_mon_destroy(ZONESVRENV* pstEnv, Monster *pstMon)
{
	MONSTERDEF *pstMonDef = NULL;

	//z_monster_leave_map(pstEnv->pstAppCtx, pstEnv, pstMon);
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (pstMonDef)
	{
		ZoneAni stAni;

		if(MON_SUB_TYPE_SKILL_DESTORY == pstMon->bSubType ||
			MON_SUB_TYPE_SKILL_NODESTORY == pstMon->bSubType)
		{
			pstMon->iFirstAtkerID = 0; //没有战利品
			pstMon->szOwner[0] = 0;
			pstMon->cRefresh = 0;
		}
		
		stAni.iType = OBJ_UNKNOW;
		z_mon_die(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, &stAni, 1, 1);
	}
	else
	{
		z_free_monster(pstEnv->pstAppCtx, pstEnv, pstMon);
	}
	return 0;
}

int z_add_hp_with_press(ZONESVRENV *pstEnv, ZoneAni *pstAtker, Player *pstAtkee,
                        int iAddHP, int iSkillPressRate)
{
	int i, iID = 0, iPressValue = 0;
	Player *pstPlayer = NULL;

	switch(pstAtker->iType)
	{
	case OBJ_PLAYER:
		pstPlayer = pstAtker->stObj.pstPlayer;
		iID = pstAtker->stObj.pstPlayer->iMemID;
		break;

	//case OBJ_MONSTER:
	//	iID = pstAtker->stObj.pstMon->iID;
	//	break;

	default:
		return -1;
	}

	if(pstAtkee->stRoleData.RoleDetail.HPCurr + iAddHP > 
	   (unsigned int)z_get_player_hpmax(pstEnv->pstAppCtx, pstEnv, pstAtkee))
	{
		iAddHP = z_get_player_hpmax(pstEnv->pstAppCtx, pstEnv, pstAtkee) -
		         pstAtkee->stRoleData.RoleDetail.HPCurr;
	}

	i = pstAtkee->stOnline.iPressedMonNum;
	while(i--)
	{
		MONSTERDEF *pstMonDef = NULL;
		Monster *pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv,
		                               pstAtkee->stOnline.aiPressedMon[i]);
		if(!pstMon)
		{
			continue;
		}

		pstMonDef = z_get_mon_def(pstEnv, pstMon);
		if(!pstMonDef)
		{
			return -1;
		}

		if (pstMonDef->PressHurtRate > 0)
		{
			iPressValue = (iAddHP / pstMonDef->PressHurtRate) *
			              (1 + iSkillPressRate / 100.0);
		}
		else
		{
			iPressValue = iAddHP * (1 + iSkillPressRate / 100.0);
		}

		//被治疗者没有在怪物的压力表中
		if(mon_press_find(pstMon, pstAtkee->iMemID) < 0)
		{
			continue;
		}

		// 把治疗者加入压力表
		if(mon_press_find(pstMon, iID) < 0)
		{
			mon_press_insert_player(pstEnv, pstMon, pstMonDef, pstPlayer);
		}
		mon_ai_addpress(pstEnv, pstMon, pstAtker, iPressValue);
	}
	return 0;
}

//魔法治疗
int cacl_magic_add_hp(ATKCHG *pstAtkChg, SKILLRESULT *pstResult)
{
	int iMagicAtk;
	int iHP;
	int iHPMul = pstResult->SkillResultVal1;
	int iHPMulMax = pstResult->SkillResultVal2;
	int iHPAdd = pstResult->SkillResultVal3;
	int iHPAddMax = pstResult->SkillResultVal4;
			
	//魔法攻击力
	iMagicAtk = z_cacl_attr_atk(pstAtkChg, SKILL_ATTR_ICE);
	
	if(iHPMul < iHPMulMax)
	{
		iHPMul = iHPMul + RAND1(iHPMulMax - iHPMul + 1);
	}

	if(iHPAdd < iHPAddMax)
	{
		iHPAdd = iHPAdd + RAND1(iHPAddMax - iHPAdd + 1);
	}

	iHP =  iMagicAtk*(iHPMul/100.0)+ iHPAdd;
	
	return iHP;
}

//魔法治疗
int cacl_mon_magic_add_hp(Monster *pstMon, SKILLRESULT *pstResult)
{
	int iMagicAtk;
	int iHP;
	int iHPMul = pstResult->SkillResultVal1;
	int iHPMulMax = pstResult->SkillResultVal2;
	int iHPAdd = pstResult->SkillResultVal3;
	int iHPAddMax = pstResult->SkillResultVal4;
			
	//魔法攻击力
	iMagicAtk = pstMon->iOwnerMagic;
	
	if(iHPMul < iHPMulMax)
	{
		iHPMul = iHPMul + RAND1(iHPMulMax - iHPMul + 1);
	}

	if(iHPAdd < iHPAddMax)
	{
		iHPAdd = iHPAdd + RAND1(iHPAddMax - iHPAdd + 1);
	}

	iHP =  iMagicAtk*(iHPMul/100.0)+ iHPAdd;
	
	return iHP;
}

int z_do_skill_result_add_hp(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, SKILLRESULT *pstResult)
{
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	Monster *pstMon;
	int iTmp = 0;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit-1];
	
	int iAtkerMul, iAtkerAdd;
	int iAtkeeMul, iAtkeeAdd;
	int iReduHealHP;

	iAtkerMul = pstAtkerInfo->stDyn.stHealChg.nHealHPMul + pstAtkerInfo->stDyn.stHealChg.nBaseHealHPMul;
	iAtkerAdd = pstAtkerInfo->stDyn.stHealChg.unHealHPAdd;

	iAtkeeMul = pstAtkeeInfo->stDyn.stHealChg.nAtkeeHealHPMul;
	iAtkeeAdd = pstAtkeeInfo->stDyn.stHealChg.unAtkeeHealHPAdd;

	iReduHealHP = pstAtkeeInfo->stDyn.stHealChg.nReduceAtkeeHealHPMul;
	
	if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;

		// 检查有没有禁止回血状态
		if(!z_is_mon_status_result(pstEnv, pstMon, STATUS_RESULT_CANNOT_ADDHP))
		{
			iTmp = pstResult->SkillResultVal1/100.0*pstMon->iMaxHP + pstResult->SkillResultVal2;
			//iTmp = iTmp*(1.0+iAtkerMul/10000.0+iAtkeeMul/100.0)+iAtkerAdd + iAtkeeAdd;
			iTmp = iTmp*(1.0+iAtkerMul/10000.0+iAtkeeMul/100.0)*(1+iReduHealHP/100.0)+iAtkerAdd + iAtkeeAdd;
			
			
			pstMon->iHP += iTmp;
			if (pstMon->iHP > pstMon->iMaxHP)
			{
				pstMon->iHP = pstMon->iMaxHP;
			}

			if (pstOneHit->TotalHurt < MAX_HURT)
			{
				pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
				pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iTmp;
				pstOneHit->TotalHurt++;
			}
		}
	}
	else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		Player *pstAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
		
		iTmp = pstResult->SkillResultVal1/100.0*pstAtkee->stOnline.stDyn.stMidAttrList.uiHPMax + pstResult->SkillResultVal2;
		//iTmp = iTmp*(1.0+iAtkerMul/10000.0+iAtkeeMul/100.0)+iAtkerAdd + iAtkeeAdd;
		iTmp = iTmp*(1.0+iAtkerMul/10000.0+iAtkeeMul/100.0)*(1+iReduHealHP/100.0)+iAtkerAdd + iAtkeeAdd;
		
		if (pstOneHit->TotalHurt < MAX_HURT)
		{
			pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
			pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iTmp;
			pstOneHit->TotalHurt++;

			z_add_hp_with_press(pstEnv, &pstAtkerInfo->stAtker,
			                    pstAtkee, iTmp,
			                    pstAtkerInfo->pstSkillDef->PressRate);
		}
	}
	else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
	{
		ROLEPET *pstAtkeePet = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet;
		
		iTmp = pstResult->SkillResultVal1/100.0*pstAtkeePet->MaxHP + pstResult->SkillResultVal2;
		//iTmp = iTmp*(1.0+iAtkerMul/10000.0+iAtkeeMul/100.0)+iAtkerAdd + iAtkeeAdd;
		iTmp = iTmp*(1.0+iAtkerMul/10000.0+iAtkeeMul/100.0)*(1+iReduHealHP/100.0)+iAtkerAdd + iAtkeeAdd;
		
		pet_add_power(pstEnv, pstAtkeeInfo->stAtkee.stObj.stZonePet.pstOwner, pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet, iTmp);
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);
}

int z_do_skill_result_add_hp_restore(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, SKILLRESULT *pstResult)
{
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	Monster *pstMon;
	int iTmp = 0;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit-1];
		
	if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;

		// 检查有没有禁止回血状态
		if(!z_is_mon_status_result(pstEnv, pstMon, STATUS_RESULT_CANNOT_ADDHP))
		{
			iTmp = pstResult->SkillResultVal1/100.0*pstMon->iMaxHP + pstResult->SkillResultVal2;
			if( pstMon->iHP >= iTmp )
			{
				return -1;
			}
			iTmp = iTmp - pstMon->iHP;
			//pstMon->iHP += iTmp;
			//if (pstMon->iHP > pstMon->iMaxHP)
			//{
			//	pstMon->iHP = pstMon->iMaxHP;
			//}

			if (pstOneHit->TotalHurt < MAX_HURT)
			{
				pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
				pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iTmp;
				pstOneHit->TotalHurt++;
			}
		}
	}
	else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		Player *pstAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
		
		iTmp = pstResult->SkillResultVal1/100.0*pstAtkee->stOnline.stDyn.stMidAttrList.uiHPMax + pstResult->SkillResultVal2;

		if( (int)pstAtkee->stRoleData.RoleDetail.HPCurr >= iTmp )
		{
			return -1;
		}
		iTmp = iTmp - pstAtkee->stRoleData.RoleDetail.HPCurr;
					
		if (pstOneHit->TotalHurt < MAX_HURT)
		{
			pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
			pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iTmp;
			pstOneHit->TotalHurt++;

			z_add_hp_with_press(pstEnv, &pstAtkerInfo->stAtker,
			                    pstAtkee, iTmp,
			                    pstAtkerInfo->pstSkillDef->PressRate);
		}
	}
	else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
	{
		ROLEPET *pstAtkeePet = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet;
		
		iTmp = pstResult->SkillResultVal1/100.0*pstAtkeePet->MaxHP + pstResult->SkillResultVal2;
		if( (int)pstAtkeePet->HP >= iTmp )
		{
			return -1;
		}
		iTmp = iTmp - pstAtkeePet->HP;
		
		pet_add_power(pstEnv, pstAtkeeInfo->stAtkee.stObj.stZonePet.pstOwner, pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet, iTmp);
	}
	else
	{
		return -1;
	}
	
	return 0;
	UNUSED(pstAppCtx);
}

SKILLRESULT *z_skill_result(ZONESVRENV* pstEnv,  SKILLDEF *pstDef, int iResultID)
{
	int i = 0;
	SKILLRESULT *pstResult = NULL;

	for (i = 0; i < (int)CCH(pstDef->SkillResult); ++i)
	{
		pstResult = &pstDef->SkillResult[i];
		if (pstResult->SkillResultID == iResultID)
		{
			return pstResult;
		}
	}
	return NULL;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_contaminate_status(ZONESVRENV* pstEnv, ZoneAni *pstAtker, int iID,
                         RESPOS *pstPos, int iDist, int iStatus, int iLevel,
                         int iLast, int iApplyID)
{
	char cType;
	if (z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType) < 0)
	{
		return -1;
	}
	
	switch (cType)
	{
	case OBJ_PLAYER:
		{
			Player *pstItem = z_id_player(pstEnv->pstAppCtx, pstEnv, iID);
			switch (pstAtker->iType)
			{
			case OBJ_PLAYER:
				if (!is_player_enemy_player(pstEnv, pstItem,
				                            pstAtker->stObj.pstPlayer))
				{
					return -1;
				}
				break;
			
			case OBJ_MONSTER:
				if (!is_player_enemy_mon(pstEnv, pstItem,
				                         pstAtker->stObj.pstMon))
				{
					return -1;
				}
				break;
				
			default:
				return -1;
				break;
			}
			if (pstItem &&
				z_distance(pstPos, &pstItem->stRoleData.Pos) <= iDist)
			{
				z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstItem,
				                      iStatus, iLevel, iLast, 1, iApplyID);
			}
		}
		break;
		
	case OBJ_MONSTER:
		{
			Monster *pstItem = z_id_monster(pstEnv->pstAppCtx, pstEnv, iID);
			switch (pstAtker->iType)
			{
			case OBJ_PLAYER:
				if (!is_player_enemy_mon(pstEnv, pstAtker->stObj.pstPlayer,
				                         pstItem))
				{
					return -1;
				}
				break;
			
			case OBJ_MONSTER:
				if (!is_mon_enemy_mon(pstEnv, pstAtker->stObj.pstMon, pstItem))
				{
					return -1;
				}
				break;
				
			default:
				return -1;
				break;
			}
			if (pstItem &&
				z_distance(pstPos, &pstItem->stCurrPos) <= iDist)
			{
				z_mon_apply_status(pstEnv->pstAppCtx, pstEnv, pstItem,
				                   iStatus, iLevel, iLast, 1, iApplyID);
			}
		}
		break;

	default:
		return -1;
		break;
	}
	return 0;
}

int z_calc_contaminate_status(ZONESVRENV* pstEnv,
                              AtkProcess *pstAtkProcess,
                              AtkeeInfo *pstAtkeeInfo,
                              const SKILLRESULT *pstResult,
                              int iRate, int iDist)
{
	RESPOS *pstPos = NULL;
	//int iApplyID = 0;

	if (iRate > 0 && (int)(RAND1(10000)) >= iRate * 100.0f)
	{
		return 0;
	}

	switch (pstAtkeeInfo->stAtkee.iType)
	{
	case OBJ_PLAYER:
		pstPos = &pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.Pos;
		break;

	case OBJ_MONSTER:
		pstPos = &pstAtkeeInfo->stAtkee.stObj.pstMon->stCurrPos;
		break;
	
	default:
		return -1;
		break;
	}
	if(!pstPos)
	{
		return -1;
	}
	

	switch (pstAtkProcess->stAtkerInfo.stAtker.iType)
	{
	case OBJ_PLAYER:
		{
			Player *pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer;
			int i, iApplyID = pstPlayer->iMemID;
			for (i = 0; i < pstPlayer->stOnline.iVAnimate; ++i)
			{
				z_contaminate_status(pstEnv, &pstAtkProcess->stAtkerInfo.stAtker,
				                     pstPlayer->stOnline.aiVAnimate[i],
				                     pstPos, iDist,
				                     pstResult->SkillResultVal2,
				                     pstResult->SkillResultVal3,
				                     pstResult->SkillResultVal4, iApplyID);
			}
		}
		break;

	case OBJ_MONSTER:
		{
			Monster *pstMon = pstAtkProcess->stAtkerInfo.stAtker.stObj.pstMon;
			int i, iApplyID = pstMon->iMIndex;
			for (i = 0; i < pstMon->iVPlayer; ++i)
			{
				z_contaminate_status(pstEnv, &pstAtkProcess->stAtkerInfo.stAtker,
				                     pstMon->aiVPlayer[i],
				                     pstPos, iDist,
				                     pstResult->SkillResultVal2,
				                     pstResult->SkillResultVal3,
				                     pstResult->SkillResultVal4, iApplyID);
			}
		}
		break;

	default:
		return -1;
		break;
	}
	return 0;
}


//  完全治愈，目标(人)附加状态
int revert_full_add_state(ZONESVRENV* pstEnv, Player *pstTarget, int iAddHp, SKILLRESULT *pstSkillResult)
{
	int iRate;
	int iCurrHp = pstTarget->stRoleData.RoleDetail.HPCurr;
	int iMaxHp = z_get_player_hpmax(pstEnv->pstAppCtx, pstEnv, pstTarget);

	if (iCurrHp < iMaxHp && (iCurrHp+iAddHp >= iMaxHp))
	{
		iRate = RAND1(100);
		if (iRate < pstSkillResult->SkillResultVal4)
		{
			z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstTarget, pstSkillResult->SkillResultVal1,
				pstSkillResult->SkillResultVal2, pstSkillResult->SkillResultVal3, 1, pstTarget->iMemID);
		}
	}
	return 0;
}

int z_do_skill_result(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo, SKILLRESULT *pstResult)
{
	Player *pstPlayer=NULL;
	Monster *pstMon;
	int iDeadHitRate;
	int iTmp = 0;
	PlayerChagAttrVal stAttr;
	ZONEMAPINST *pstMapInst;
	RESPOS stPos;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit-1];
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	int iHP = 0;
	int iMap = 0;
	MIDATTRLIST *pstMidAttrList;
	SKILLRESULT *pstExtraResult = NULL;
	int iHurt = 0;

	switch (pstResult->SkillResultID)
	{
		case RESULT_HITRATE:

			
			break;
			
		case RESULT_DEADHIT:
			if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				iHP = pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.RoleDetail.HPCurr;
			}
			else if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				iHP = pstAtkeeInfo->stAtkee.stObj.pstMon->iHP;
			}
			else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
			{
				iHP = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet->HP;
			}
			else
				return -1;

			if (iHP > 0)
			{
				iTmp = DEADHIT_BASE;
				iTmp = iTmp - pstResult->SkillResultVal1;
				if (iTmp < 0)
				{
					iTmp = 0;
				}
				
				iDeadHitRate = ((pstAtkProcess->iHurt*1.0/iHP)*100 - iTmp)*pstResult->SkillResultVal2;
				if ((int)(RAND1(10000)) < iDeadHitRate)
				{
					pstAtkProcess->cDeadHit = 1;
				}
			}
			else
			{
				pstAtkProcess->cDeadHit = 1;
				break;
			}
			
			break;

		case RESULT_ADD_ATK: //追招要忽略频数
			if (pstAtkProcess->iContinueCount+1 == pstAtkProcess->bContinueNum && pstAtkProcess->stAddAtkInfo.nAddAtkNum < MAX_ADD_ATK)
			{
				pstAtkProcess->stAddAtkInfo.aiAddAtkMul[pstAtkProcess->stAddAtkInfo.nAddAtkNum] = pstResult->SkillResultVal1;
				pstAtkProcess->stAddAtkInfo.nAddAtkNum++;	
			}
			break;
			
		case RESULT_HITBACK:
			z_do_hitback(pstAppCtx, pstEnv, pstAtkerInfo, pstAtkeeInfo, pstOneHit, pstResult->SkillResultVal1, pstResult->SkillResultVal2, 
						pstResult->SkillResultVal3);
			break;

		case RESULT_STATUS:
			if (pstAtkerInfo->pstSkillDef->PilotNum > 0 && pstAtkerInfo->iPilot != 1)
			{
				if (z_skill_result(pstEnv, pstAtkerInfo->pstSkillDef, RESULT_STATUS_FIRST_PILOT))
				{
					break;
				}
			}
			
			if (pstResult->SkillResultVal1 > 0 &&
				(int)(RAND1(10000)) >= pstResult->SkillResultVal1*100.0)
			{
				break;
			}

			if (z_calc_apply_status(pstAppCtx, pstEnv, pstAtkProcess,
			                        pstAtkeeInfo, pstResult->SkillResultVal2, 
				                    pstResult->SkillResultVal3,
				                    pstResult->SkillResultVal4) < 0)
			{
				return -1;
			}
			
			pstExtraResult = z_skill_result(pstEnv,  pstAtkerInfo->pstSkillDef,RESULT_CONTAGION);
			if (!pstExtraResult)
			{
				return 0;
			}
			return !pstExtraResult ? 0 : 
			        z_calc_contaminate_status(pstEnv, pstAtkProcess, pstAtkeeInfo,
			                                  pstResult,
			                                  pstExtraResult->SkillResultVal1,
			                                  pstExtraResult->SkillResultVal2);
			break;

		case RESULT_DEFY:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType && OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
			{
				AIEvt stAIEvt;

				stAIEvt.iEvtID = AI_EVT_BEDEFY;
				stAIEvt.stEvtDetail.stBeDefy.iTime = pstResult->SkillResultVal1;
				stAIEvt.stEvtDetail.stBeDefy.pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				mon_ai_stat(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, &stAIEvt);
			}
			break;

		case RESULT_TMPFILL:
			if (OBJ_PLAYER != pstAtkerInfo->stAtker.iType || OBJ_PLAYER != pstAtkeeInfo->stAtkee.iType || pstAtkerInfo->pstZoneSkill->stSkillData.CollectNum <= 0)
			{
				return -1;
			}

			tmp_fill(pstEnv , pstAtkerInfo->stAtker.stObj.pstPlayer, pstAtkerInfo->pstZoneSkill, pstAtkeeInfo->stAtkee.stObj.pstPlayer);
			break;
			
			
		case RESULT_ADD_XP:
			// ResultVal1 最大XP 比率
			// ResultVal2 

			// 加满XP
			{
				int iFairyMaxXP;

				pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
				if (pstAtkeeInfo->stAtkee.iType != OBJ_PLAYER || !pstPlayer)
				{
					return -1;
				}

				iFairyMaxXP = get_fairy_xp_upper(pstEnv, pstPlayer);
				
				if ((int)pstPlayer->stRoleData.RoleDetail.XP < iFairyMaxXP)
				{
					iTmp = iFairyMaxXP * (pstResult->SkillResultVal1/100.0) +pstResult->SkillResultVal2;
					//pstPlayer->stRoleData.MiscInfo.XpPrecise += iTmp * 100;
					player_add_xp(pstEnv, pstPlayer, iTmp, 0,1);
				}	

			}
			break;

		case RESULT_SKILL_FLUSH:	// 刷新
			pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;

			if (pstAtkerInfo->stAtker.iType != OBJ_PLAYER || !pstPlayer)
			{
				return -1;
			}

			player_result_flush_skill(pstEnv, pstPlayer, pstAtkerInfo->pstSkillDef->SkillID);
			
			break;
			
		case RESULT_HP_CHG_MP:
			// result1 hp百分比 2-hp值 3--mp百分比 4--mp值
			pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
			if (pstAtkeeInfo->stAtkee.iType != OBJ_PLAYER || !pstPlayer)
				return -1;
			pstMidAttrList = &pstPlayer->stOnline.stDyn.stMidAttrList;
			iTmp = pstMidAttrList->uiHPMax *(pstResult->SkillResultVal1/100.0) + pstResult->SkillResultVal2;
			if (pstPlayer->stRoleData.RoleDetail.HPCurr <= (unsigned int)iTmp)
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL28);
				return -1;
			}

			if (pstOneHit->TotalHurt < MAX_HURT)
			{
				pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
				pstOneHit->Hurts[pstOneHit->TotalHurt].Value = (0 - iTmp);
				pstOneHit->TotalHurt++;
			}

			iTmp = pstMidAttrList->uiMPMax *(pstResult->SkillResultVal3/100.0) + pstResult->SkillResultVal4;
			if (pstOneHit->TotalHurt < MAX_HURT)
			{
				pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_MP;
				pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iTmp;
				pstOneHit->TotalHurt++;
			}

			break;

		case RESULT_HP_ADD:
			z_do_skill_result_add_hp(pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, pstResult);
			break;	
		case RESULT_HP_ADD_RESTORE:
			z_do_skill_result_add_hp_restore(pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, pstResult);
			break;
		case RESULT_MP_ADD_RESTORE:
			if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				iTmp = pstAtkeeInfo->stDyn.stMidAttrList.uiMPMax * (pstResult->SkillResultVal1/100.0) + pstResult->SkillResultVal2;
				if((int)pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.RoleDetail.MPCurr >= iTmp)
				{
					break;
				}
				iTmp = iTmp-pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.RoleDetail.MPCurr;	
				if (pstOneHit->TotalHurt < MAX_HURT)
				{
					pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_MP;
					pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iTmp;
					pstOneHit->TotalHurt++;
				}
			}
			break;
		case RESULT_MP_ADD:
			if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{	
				iTmp = pstAtkeeInfo->stDyn.stMidAttrList.uiMPMax * (pstResult->SkillResultVal1/100.0) + pstResult->SkillResultVal2;
				if(iTmp>0)
				{
					iTmp = iTmp*(1+pstAtkerInfo->stDyn.stHealChg.nHealMPMul/100.0)+pstAtkerInfo->stDyn.stHealChg.unHealMPAdd;
				}
				

				if (pstOneHit->TotalHurt < MAX_HURT)
				{
					pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_MP;
					pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iTmp;
					pstOneHit->TotalHurt++;
				}
			}
			else
			{
				return -1;
			}
			break;

		case RESULT_CALL_TARGET:
			if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
			{
				pstMon = pstAtkerInfo->stAtker.stObj.pstMon;
				pstPlayer = NULL;	
			}
			else if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
			{
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
			}
			else
			{
				return -1;
			}

			if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
			{
				Monster *pstAtkee = pstAtkeeInfo->stAtkee.stObj.pstMon;
				pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstAtkee->stMap);
				if(!pstMapInst)
				{
					return -1;
				}
				stPos = pstAtkee->stCurrPos;
			}
			else if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
			{
				ZONEIDX stIdx;
				Player *pstAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;

				stIdx.iID = pstAtkee->stRoleData.Map;
				stIdx.iPos = pstAtkee->stOnline.iMapPos;
				pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
				if(!pstMapInst)
				{
					return -1;
				}
				stPos = pstAtkee->stRoleData.Pos;
			}
			else
			{
				return -1;
			}
			/*
			if(pstPlayer && pstAtkerInfo->pstFittings)
			{
				MonsterID = pstResult->SkillResultVal2;
				ExsitTime = pstResult->SkillResultVal4;
				memset(&stResultTmp,0,sizeof(stResultTmp));
				if( 0 == z_get_skill_fittings_result( pstEnv, pstAtkerInfo->pstFittings, RESULT_FITTING_CHG_CALL_ID,&stResultTmp))
				{
					MonsterID = stResultTmp.SkillResultVal2;
				}
				if( 0 == z_get_skill_fittings_result( pstEnv, pstAtkerInfo->pstFittings, RESULT_FITTING_CHG_CALL_TIME,&stResultTmp) )
				{
					ExsitTime = stResultTmp.SkillResultVal1;
				}
				
			          z_do_call(pstEnv, pstPlayer, pstAtkerInfo,
				          pstMapInst, MonsterID,
				          pstResult->SkillResultVal1, pstResult->SkillResultVal3,
				          ExsitTime, &stPos,NULL);
			}
			else
			*/
			{
				if(pstPlayer)
				{
					z_do_call(pstEnv, pstPlayer, pstAtkerInfo,
				          pstMapInst, pstResult->SkillResultVal2,
				          pstResult->SkillResultVal1, pstResult->SkillResultVal3,
				          pstResult->SkillResultVal4, &stPos,NULL);
				}
				else
				{
					z_do_call(pstEnv, NULL, pstAtkerInfo,
				          pstMapInst, pstResult->SkillResultVal2,
				          pstResult->SkillResultVal1, pstResult->SkillResultVal3,
				          pstResult->SkillResultVal4, &stPos,NULL);
				}
			}
			break;

		case RESULT_CALL:
			if(OBJ_MONSTER == pstAtkerInfo->stAtker.iType)
			{
				pstMon = pstAtkerInfo->stAtker.stObj.pstMon;
				pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
				if (NULL == pstMapInst)
				{
					return -1;
				}
				stPos = pstMon->stCurrPos;
				pstPlayer = NULL;	
			}
			else if(OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
			{
				ZONEIDX stIdx;
				
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				stIdx.iID = pstPlayer->stRoleData.Map;
				stIdx.iPos = pstPlayer->stOnline.iMapPos;
				pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
				if (NULL == pstMapInst)
				{
					return -1;
				}
				stPos = pstPlayer->stRoleData.Pos;
			}
			else
			{
				return -1;
			}
			/*
			if(pstPlayer && pstAtkerInfo->pstFittings)
			{
				MonsterID = pstResult->SkillResultVal2;
				ExsitTime = pstResult->SkillResultVal4;
				memset(&stResultTmp,0,sizeof(stResultTmp));
				if( 0 == z_get_skill_fittings_result( pstEnv, pstAtkerInfo->pstFittings, RESULT_FITTING_CHG_CALL_ID,&stResultTmp) )
				{
					MonsterID = stResultTmp.SkillResultVal2;
				}
				if( 0 == z_get_skill_fittings_result( pstEnv, pstAtkerInfo->pstFittings, RESULT_FITTING_CHG_CALL_TIME,&stResultTmp) )
				{
					ExsitTime = stResultTmp.SkillResultVal1;
				}

				// val3表示第几个result里面保存了均匀分布的 形状信息
				if (pstResult->SkillResultVal3 > 0 && pstResult->SkillResultVal3<= MAX_SKILL_RESULT)
				{
					z_do_call(pstEnv, pstPlayer, pstAtkerInfo, pstMapInst,MonsterID, 
							pstResult->SkillResultVal1, pstResult->SkillResultVal3, ExsitTime, &stPos,
							&pstAtkerInfo->pstSkillDef->SkillResult[pstResult->SkillResultVal3-1]);
				}
				
			}
			else
			*/
			{
				if (pstResult->SkillResultVal3 > 0 && pstResult->SkillResultVal3<= MAX_SKILL_RESULT)
				{
					if(pstPlayer)
					{
						z_do_call(pstEnv, pstPlayer, pstAtkerInfo, pstMapInst,pstResult->SkillResultVal2, 
						pstResult->SkillResultVal1, pstResult->SkillResultVal3, pstResult->SkillResultVal4, &stPos,
						&pstAtkerInfo->pstSkillDef->SkillResult[pstResult->SkillResultVal3-1]);
					}
					else
					{
						z_do_call(pstEnv, NULL, pstAtkerInfo, pstMapInst,pstResult->SkillResultVal2, 
						pstResult->SkillResultVal1, pstResult->SkillResultVal3, pstResult->SkillResultVal4, &stPos,
						&pstAtkerInfo->pstSkillDef->SkillResult[pstResult->SkillResultVal3-1]);
					}
				}
			}
			break;

		case RESULT_REVIVE:
			if (OBJ_PLAYER ==  pstAtkeeInfo->stAtkee.iType)
			{
				pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
				if (!(CS_STAT_DEAD & pstPlayer->stOnline.State))
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}

			if (OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
			{
				z_do_revive(pstEnv, pstAtkerInfo->stAtker.stObj.pstPlayer, pstPlayer, pstAtkProcess->stAtkerInfo.pstSkillDef, pstResult);
			}
			else
			{
				return -1;
			}
			break;

		case RESULT_CLEAN:
			if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				player_status_clean(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer, pstResult, STATUS_SUBTYPE_BAD_MAGIC);
			}
			else if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				mon_status_clean(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, pstResult, STATUS_SUBTYPE_BAD_MAGIC);
			}
			else
			{
				return -1;
			}
			
			break;

		case RESULT_DEPURATE:
			if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				player_status_clean(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer, pstResult, STATUS_SUBTYPE_BAD_CURSE);
			}
			else if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
			{
				mon_status_clean(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, pstResult, STATUS_SUBTYPE_BAD_CURSE);
			}
			else
			{
				return -1;
			}
			
			break;

		/*case RESULT_FLY:
			if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				player_fly(pstAppCtx, pstEnv,  pstAtkeeInfo->stAtkee.stObj.pstPlayer);
			}
			else
			{
				return -1;
			}
			
			break;
		*/
		case RESULT_TARGET_HP_RATE_HURT:
			if (pstAtkProcess->iContinueCount+1 == pstAtkProcess->bContinueNum)
			{
				// result1 计算追招数据   result2 比率
				if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
				{
					iHP = pstAtkeeInfo->stAtkee.stObj.pstMon->iHP;
				}
				else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
				{
					iHP = pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.RoleDetail.HPCurr;
				}
				else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
				{
					iHP = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet->HP;
				}
				else
				{
					return -1;
				}

				pstAtkProcess->stHpRateHurt.iAddAtkMul = pstResult->SkillResultVal1;
				pstAtkProcess->stHpRateHurt.iHpRateHurt = (iHP / 100.0) * pstResult->SkillResultVal2;
			}

			break;
		case RESULT_TARGET_MAXHP_RATE_HURT:
			{
				// result1 计算追招数据   result2 比率
				if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
				{
					iHP = pstAtkeeInfo->stAtkee.stObj.pstMon->iMaxHP;
				}
				else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
				{
					iHP = z_get_player_hpmax(pstEnv->pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer);
				}
				else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
				{
					iHP = pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet->MaxHP;
				}
				else
				{
					return -1;
				}

				if (pstOneHit->TotalHurt < MAX_HURT)
				{
					pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
					pstOneHit->Hurts[pstOneHit->TotalHurt].Value = (iHP / 100.0) * pstResult->SkillResultVal1;;
					pstOneHit->TotalHurt++;
				}
			}
			break;
		case RESULT_BACK_HOME:
			if (OBJ_PLAYER == pstAtkProcess->stAtkerInfo.stAtker.iType)
			{
				iMap = pstResult->SkillResultVal2;
				stPos.X = pstResult->SkillResultVal3;
				stPos.Y = pstResult->SkillResultVal4;

				return player_home(pstEnv,pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer,  iMap, &stPos);
			}
			break;

		case RESULT_MAKE_TMP_ITEM:
			if (OBJ_PLAYER == pstAtkProcess->stAtkerInfo.stAtker.iType)
			{
				return make_tmp_item(pstAppCtx, pstEnv, pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer, pstResult->SkillResultVal2, pstResult->SkillResultVal1);
			}
			break;

		case RESULT_SIT:
			if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				player_sit(pstAppCtx, pstEnv,  pstAtkeeInfo->stAtkee.stObj.pstPlayer);
			}
			else
			{
				return -1;
			}
			break;
		case RESULT_UNIQUE_STATUS:
			//只有人释放标记类状态
			if( OBJ_PLAYER != pstAtkerInfo->stAtker.iType)
			{
				return -1;
			}
			
			pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
			if (pstAtkProcess->stAtkerInfo.pstZoneSkill->iTagAniID != 0)
			{
				z_del_unique_status(pstAppCtx, pstEnv, pstPlayer, pstAtkerInfo->pstZoneSkill->iTagAniID, pstResult->SkillResultVal2);
			}
			
			return z_calc_apply_status(pstAppCtx, pstEnv, pstAtkProcess,
			                           pstAtkeeInfo, pstResult->SkillResultVal2, 
			                           pstResult->SkillResultVal3,
			                           pstResult->SkillResultVal4/*, FROM_SKILL*/);
			break;
		case RESULT_PULL:
			return z_do_pull(pstAppCtx, pstEnv, pstAtkerInfo, pstAtkeeInfo, pstOneHit, pstResult->SkillResultVal1, pstResult->SkillResultVal2);
			break;
		case RESULT_DEC_MP:
			if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
			{
				int iMp = 0;
				int iOldHP = 0;
				int iOldMP = 0;
	
				pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;

				iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
				iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;

				iMp = z_get_player_mpmax(pstAppCtx, pstEnv, pstPlayer);
				iMp = pstResult->SkillResultVal1 + iMp * pstResult->SkillResultVal2/100.0;
				
				stAttr.AttrNum = 0;
				
				stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_MP;
				stAttr.AttrList[stAttr.AttrNum].nFlag = 1;
				//stAttr.AttrList[stAttr.AttrNum].Value.MP = pstResult->SkillResultVal1;
				stAttr.AttrList[stAttr.AttrNum].Value.MP= iMp;
				stAttr.AttrNum++;

				z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);

				if (iOldHP != (int)pstPlayer->stRoleData.RoleDetail.HPCurr ||
					iOldMP != (int)pstPlayer->stRoleData.RoleDetail.MPCurr)
				{
					player_hp_action(pstEnv, pstPlayer, iOldHP, iOldMP, 0);
				}
			}
			break;
		
		case RESULT_ATK_POS_CALL:
			if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
			{
				pstMon = pstAtkerInfo->stAtker.stObj.pstMon;
				pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
				if (NULL == pstMapInst)
				{
					return -1;
				}
				
				pstPlayer = NULL;	
			}
			else if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
			{
				ZONEIDX stIdx;
					
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;

				stIdx.iID = pstPlayer->stRoleData.Map;
				stIdx.iPos = pstPlayer->stOnline.iMapPos;
				pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
				if(!pstMapInst)
				{
					return -1;
				}
			}
			else
			{
				return -1;
			}

			stPos = pstAtkProcess->stAtkerInfo.stAtkPos;

			/*
			if(pstPlayer && pstAtkerInfo->pstFittings)
			{
				MonsterID = pstResult->SkillResultVal2;
				ExsitTime = pstResult->SkillResultVal4;
				memset(&stResultTmp,0,sizeof(stResultTmp));
				if( 0 == z_get_skill_fittings_result( pstEnv, pstAtkerInfo->pstFittings, RESULT_FITTING_CHG_CALL_ID,&stResultTmp))
				{
					MonsterID = stResultTmp.SkillResultVal2;
				}
				if( 0 == z_get_skill_fittings_result( pstEnv, pstAtkerInfo->pstFittings, RESULT_FITTING_CHG_CALL_TIME,&stResultTmp) )
				{
					ExsitTime = stResultTmp.SkillResultVal1;
				}

			          z_do_call(pstEnv, pstPlayer, pstAtkerInfo,
				          pstMapInst, MonsterID,
				          pstResult->SkillResultVal1, pstResult->SkillResultVal3,
				          ExsitTime, &stPos,NULL);
			}
			else
			*/
			{
				if(pstPlayer)
				{
					z_do_call(pstEnv, pstPlayer, pstAtkerInfo,
				          pstMapInst, pstResult->SkillResultVal2,
				          pstResult->SkillResultVal1, pstResult->SkillResultVal3,
				          pstResult->SkillResultVal4, &stPos,NULL);
				}
				else
				{
					z_do_call(pstEnv, NULL, pstAtkerInfo,
				          pstMapInst, pstResult->SkillResultVal2,
				          pstResult->SkillResultVal1, pstResult->SkillResultVal3,
				          pstResult->SkillResultVal4, &stPos,NULL);
				}
			}
			break;
		case RESULT_COPY_MAX_PRESS:
			if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType && OBJ_PLAYER == pstAtkerInfo->stAtker.iType)
			{
				AIEvt stAIEvt;

				stAIEvt.iEvtID = AI_EVT_COPY_PRESS;
				stAIEvt.stEvtDetail.stCopyPress.pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				mon_ai_stat(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, &stAIEvt);
			}
			break;
		case RESULT_MAGIC_HP_ADD:
			{
				int iAtkerMul, iAtkerAdd;
				int iAtkeeMul, iAtkeeAdd;
				int iReduHealHP;
				int iHP = 0, iMul = 0;;
				ATKCHG *pstAtkChg = NULL;
				ResourceConsume *pstResource = &pstAtkerInfo->stResourceConsume;
				SKILLRESULT *pstSkillResult;
				

				if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
				{
					iHP = cacl_mon_magic_add_hp(pstAtkerInfo->stAtker.stObj.pstMon, pstResult);
				}
				else if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
				{
					if(pstAtkProcess->iAtkChgIdx < 0 || pstAtkProcess->iAtkChgIdx >= MAX_ARM_ON )
					{
						break;
					}
					
					pstAtkChg = &pstAtkProcess->stAtkerInfo.stDyn.stAtkChg;
					iHP = cacl_magic_add_hp(pstAtkChg, pstResult);

					if(pstAtkerInfo->stAtker.stObj.pstPlayer->stRoleData.Career == CAREER_CHURCH_DOUBLEHAND)
					{
						iMul = -20;
					}
				}
				else
				{
					break;
				}

				//治疗者效果
				iAtkerMul = pstAtkerInfo->stDyn.stHealChg.nHealHPMul + pstAtkerInfo->stDyn.stHealChg.nBaseHealHPMul;
				iAtkerAdd = pstAtkerInfo->stDyn.stHealChg.unHealHPAdd;

				//被治疗者效果
				iAtkeeMul = pstAtkeeInfo->stDyn.stHealChg.nAtkeeHealHPMul;
				iAtkeeAdd = pstAtkeeInfo->stDyn.stHealChg.unAtkeeHealHPAdd;

				iReduHealHP = pstAtkeeInfo->stDyn.stHealChg.nReduceAtkeeHealHPMul;

				//最终治疗量
				//iHP = iHP*(1.0+iAtkerMul/10000.0+iAtkeeMul/100.0)+iAtkerAdd + iAtkeeAdd;
				iHP = iHP*(1.0+iAtkerMul/10000.0+iAtkeeMul/100.0)*(1+iReduHealHP/100.0)+iAtkerAdd + iAtkeeAdd;

				iHP = iHP*(100+iMul)/100.0;
				if(iHP <= 0)
				{
					break;
				}

				pstSkillResult = z_get_skill_result( pstAtkerInfo->pstSkillDef,RESULT_CURE_END_REVERT);
				if (pstSkillResult)
				{
					pstResource->iHPUse -= iHP*(1.0*pstSkillResult->SkillResultVal1/100);
				}
				
				if (OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
				{
					pstMon = pstAtkeeInfo->stAtkee.stObj.pstMon;

					// 检查有没有禁止回血状态
					if(!z_is_mon_status_result(pstEnv, pstMon, STATUS_RESULT_CANNOT_ADDHP))
					{
						pstMon->iHP += iHP;
						if (pstMon->iHP > pstMon->iMaxHP)
						{
							pstMon->iHP = pstMon->iMaxHP;
						}

						if (pstOneHit->TotalHurt < MAX_HURT)
						{
							pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
							pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iHP;
							pstOneHit->TotalHurt++;
						}
					}
				}
				else if (OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
				{
					Player *pstAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
					SKILLRESULT *pstSkillResult;
					
					if (pstOneHit->TotalHurt < MAX_HURT)
					{
						pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
						pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iHP;
						pstOneHit->TotalHurt++;

						z_add_hp_with_press(pstEnv, &pstAtkerInfo->stAtker,
						                    pstAtkee, iHP,
						                    pstAtkerInfo->pstSkillDef->PressRate);

						pstSkillResult = z_get_skill_result( pstAtkerInfo->pstSkillDef,RESULT_REVERT_FULL_ADD_STAT);
						if (pstSkillResult)
						{
							revert_full_add_state(pstEnv, pstAtkee, iHP, pstSkillResult);
						}
						
					}
				}
				else if (OBJ_PET == pstAtkeeInfo->stAtkee.iType)
				{
					pet_add_power(pstEnv, pstAtkeeInfo->stAtkee.stObj.stZonePet.pstOwner, pstAtkeeInfo->stAtkee.stObj.stZonePet.pstRolePet, iHP);
				}
				else
				{
					return -1;
				}
			}
			break;
		case RESULT_ADD_MACHINE_ENERGY:
			if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
			{
				int iEnergy = 0;
				Monster *pstAtkee = pstAtkeeInfo->stAtkee.stObj.pstMon;
				if( !is_mon_machine(pstAtkee) )
				{
					return -1;
				}
				iEnergy = this_machine(pstEnv,pstAtkee)->stMachineInfo.MaxEnergy * (pstResult->SkillResultVal2/100.0);
				iEnergy += pstResult->SkillResultVal1;
				if( iEnergy != 0)
				{
					z_machine_attr_chg(pstEnv, pstAtkee, MON_MACHINE_ATTR_ENERGY, iEnergy);
				}
			}	
			break;
		case RESULT_DEL_STATUS_BY_ID:
			if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
			{
				z_mon_del_status(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, pstResult->SkillResultVal1,0);
			}
			else if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
			{
				z_player_del_status( pstAppCtx, pstEnv,  pstAtkeeInfo->stAtkee.stObj.pstPlayer, pstResult->SkillResultVal1, 1,0);
			}
			break;
		case RESULT_DEL_SELF_STATUS_BY_ID:
			if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
			{
				z_mon_del_status(pstAppCtx, pstEnv, pstAtkerInfo->stAtker.stObj.pstMon, pstResult->SkillResultVal1,0);
			}
			else if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
			{
				z_player_del_status( pstAppCtx, pstEnv,  pstAtkerInfo->stAtker.stObj.pstPlayer, pstResult->SkillResultVal1, 1,0);
			}
			break;
		case RESULT_HP_CHUNNEL:
			{
				z_calc_apply_status(pstAppCtx, pstEnv, pstAtkProcess,
			                        pstAtkeeInfo, pstResult->SkillResultVal1, 
				                    pstResult->SkillResultVal2,
				                    pstResult->SkillResultVal3);
				/*int  iApplyID = 0;
				ZoneStatus *pstZoneStatus = NULL;
				
				if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
				{
					iApplyID =  pstAtkerInfo->stAtker.stObj.pstMon->iID;
				}
				else if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
				{
					iApplyID =  pstAtkerInfo->stAtker.stObj.pstPlayer->iMemID;
				}
				
				if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
				{
					
					
					z_mon_apply_status(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, 
						pstResult->SkillResultVal1, pstResult->SkillResultVal2, pstResult->SkillResultVal3,
						0, iApplyID);
					
					pstZoneStatus = z_mon_find_status(pstAtkeeInfo->stAtkee.stObj.pstMon,pstResult->SkillResultVal1);
					if(!pstZoneStatus)
					{
						z_mon_del_status(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, pstResult->SkillResultVal1,0);
						return -1;
					}
					
					pstZoneStatus->cApplyRelation = z_mon_filter_relation(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, 
								&pstAtkerInfo->stAtker, pstAtkerInfo->pstSkillDef);
					
					z_mon_send_status(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				}			
				else if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
				{
					pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
					z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, 
						pstResult->SkillResultVal1, pstResult->SkillResultVal2, pstResult->SkillResultVal3,
						0, iApplyID);

					pstZoneStatus = z_player_find_status( pstPlayer, pstResult->SkillResultVal1);
					if(!pstZoneStatus)
					{
						z_player_del_status(pstAppCtx, pstEnv, pstPlayer, pstResult->SkillResultVal1,0,0);
						return -1;
					}
					pstZoneStatus->cApplyRelation = z_player_filter_relation(pstAppCtx, pstEnv,  pstPlayer,
						&pstAtkerInfo->stAtker, pstAtkerInfo->pstSkillDef);
					z_player_send_status(pstAppCtx, pstEnv, pstPlayer);
				}*/
			}
			break;
		case RESULT_RANDOM_DEL_DEBUFF:
			if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
			{
				z_mon_del_random_one_debuff(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon,pstResult,pstOneHit);
			}
			else if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
			{
				z_player_del_random_one_debuff(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer,pstResult,pstOneHit);
			}
			break;
		case RESULT_MERGE_DEBUFF:
			iHurt = z_status_megre_debuff(pstEnv,&pstAtkeeInfo->stAtkee,pstResult,pstOneHit);
			if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
			{
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				//z_do_skill_result_hurt_chg_team_hp(pstEnv, pstPlayer,pstAtkProcess, 
				//										&pstAtkProcess->stAtkerInfo.stAtkPos, iHurt);
				z_do_skill_result_hurt_chg_team_hp(pstEnv, pstPlayer,pstAtkProcess, 
														&pstPlayer->stRoleData.Pos, iHurt);
			}
			break;	

		case RESULT_ADD_STATUS_BY_RELATION:
			if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
			{
				int iRelation = 0;
				iRelation = z_mon_filter_relation(pstAppCtx, pstEnv, pstAtkerInfo->stAtker.stObj.pstMon, 
														&pstAtkeeInfo->stAtkee, pstAtkerInfo->pstSkillDef);
				if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
				{
					if( (iRelation && pstResult->SkillResultVal1) ||
						(!iRelation && !pstResult->SkillResultVal1) )
					{
						z_mon_apply_status(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, 
								pstResult->SkillResultVal2, pstResult->SkillResultVal3, pstResult->SkillResultVal4,
								0, 0);
					}
				}
				else if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
				{
					if( (iRelation && pstResult->SkillResultVal1) ||
						(!iRelation && !pstResult->SkillResultVal1) )
					{
						z_player_apply_status(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer, 
							pstResult->SkillResultVal2, pstResult->SkillResultVal3, pstResult->SkillResultVal4,
						0, 0);
					}
				}
			}
			else if(pstAtkerInfo->stAtker.iType == OBJ_PLAYER)
			{
				int iRelation = 0;
				iRelation = z_player_filter_relation(pstAppCtx, pstEnv, pstAtkerInfo->stAtker.stObj.pstPlayer, 
														&pstAtkeeInfo->stAtkee, pstAtkerInfo->pstSkillDef);
				if(pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
				{
					if( (iRelation && pstResult->SkillResultVal1) ||
						(!iRelation && !pstResult->SkillResultVal1) )
					{
						z_mon_apply_status(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon, 
								pstResult->SkillResultVal2, pstResult->SkillResultVal3, pstResult->SkillResultVal4,
								0, 0);
					}
				}
				else if(pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
				{
					if( (iRelation && pstResult->SkillResultVal1) ||
						(!iRelation && !pstResult->SkillResultVal1) )
					{
						z_player_apply_status(pstAppCtx, pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer, 
						pstResult->SkillResultVal2, pstResult->SkillResultVal3, pstResult->SkillResultVal4,
						0, 0);
					}
				}
			}
			break;
		case RESULT_PET_HETI:
			if (OBJ_PLAYER == pstAtkProcess->stAtkerInfo.stAtker.iType)
			{
				pet_heti(pstEnv, pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer);
			}
			break;
		case RESULT_DROP_TMP_ITEM:
			if (pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER && pstAtkerInfo->stAtker.iType)
			{
				ROLETMPSYS *pstRoleTmpSys = &pstAtkeeInfo->stAtkee.stObj.pstPlayer->stRoleData.MiscInfo.TmpSys;
				
				if ((int)(RAND1(10000)) >= pstResult->SkillResultVal2*100.0)
				{
					break;
				}					

				if (pstRoleTmpSys->TmpItemID)
				{
					ITEMDEF* pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
					
					if (pstItemDef && pstResult->SkillResultVal1)
					{
						int k = 0;
						int iValid = 0;
						int iImmune = 0;
						
						for (k = 0; k < MAX_ITEM_RESULT; k++)
						{
							RESULT* pstTmpItemResult = pstItemDef->Result + k;
							if (0 == pstTmpItemResult->ResultID)
							{
								break;
							}
							if (pstResult->SkillResultVal1 == pstTmpItemResult->ResultID)
							{
								iValid = 1;
							}
							
							if (pstResult->SkillResultVal3 == pstTmpItemResult->ResultID)
							{
								iValid = 0;
								iImmune = 1;
								break;								
							}
							
						}
						if (iValid)
						{
							//tmp_item_drop(pstEnv,pstAtkeeInfo->stAtkee.stObj.pstPlayer,1);

							pstAtkeeInfo->stAtkee.stObj.pstPlayer->stOnline.iDropTmpID = pstRoleTmpSys->TmpItemID;
							pstAtkeeInfo->stAtkee.stObj.pstPlayer->stOnline.tDropTmpItemEnd = pstEnv->pstAppCtx->stCurr.tv_sec + 3;
							z_send_sys_str_msg(pstAppCtx, pstEnv,pstAtkeeInfo->stAtkee.stObj.pstPlayer, SYS_CHAT,
									ZONEERR_SKILL29, 
									pstAtkerInfo->stAtker.stObj.pstPlayer->stRoleData.RoleName);
						}
						
					}
					
				}
				
			}
			break;
		case RESULT_ADD_POWER:
			{
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				if (pstAtkerInfo->stAtker.iType != OBJ_PLAYER || !pstPlayer)
				{
					return -1;
				}
				
				{
					ROLEPET *pstRolePet = pet_get_camp(pstEnv, pstPlayer);
					if(pstRolePet)
					{	
						pet_add_power(pstEnv, pstPlayer, pstRolePet, pstResult->SkillResultVal1);
					}
				}
			}
			break;
		case RESULT_SELF_ADD_XP:
			// ResultVal1 最大XP 比率
			// ResultVal2 

			{
				int iFairyMaxXP;

				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				if (pstAtkerInfo->stAtker.iType != OBJ_PLAYER || !pstPlayer)
				{
					return -1;
				}

				iFairyMaxXP = get_fairy_xp_upper(pstEnv, pstPlayer);
				
				if ((int)pstPlayer->stRoleData.RoleDetail.XP < iFairyMaxXP)
				{
					iTmp = iFairyMaxXP * (pstResult->SkillResultVal1/100.0) +pstResult->SkillResultVal2;
					//pstPlayer->stRoleData.MiscInfo.XpPrecise += iTmp * 100;
					player_add_xp(pstEnv, pstPlayer, iTmp, 0,1);
				}	

			}
			break;
		case RESULT_SELF_HP_ADD:
			{
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				if (pstAtkerInfo->stAtker.iType != OBJ_PLAYER || !pstPlayer)
				{
					return -1;
				}

				pstAtkerInfo->stResourceConsume.iHPUse -= (pstResult->SkillResultVal1/100.0*pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax + pstResult->SkillResultVal2);
			}
			break;
		case RESULT_SELF_MP_ADD:
			{
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				if (pstAtkerInfo->stAtker.iType != OBJ_PLAYER || !pstPlayer)
				{
					return -1;
				}

				pstAtkerInfo->stResourceConsume.iMPUse -= (pstResult->SkillResultVal1/100.0*pstPlayer->stOnline.stDyn.stMidAttrList.uiMPMax + pstResult->SkillResultVal2);
			}
			break;
		case RESULT_THROUGH_SORB:
			pstAtkerInfo->iFlag |= ATKER_INFO_FLAG_THROUGH_ABSORB;
			break;
		case RESULT_BREAK_SKILL:
			if (pstAtkerInfo->pstSkillDef->PilotNum > 0 && pstAtkerInfo->iPilot != 1)
			{
				break;
			}

			if (pstResult->SkillResultVal1 > 0 &&
				(int)(RAND1(10000)) >= pstResult->SkillResultVal1*100)
			{
				break;
			}

			if (pstAtkeeInfo->stAtkee.iType == OBJ_PLAYER)
			{
				player_trace_act_break_prepare(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer, BRK_PRE_ACT_BEATK_CTRLSKILL);
			}
			else if (pstAtkeeInfo->stAtkee.iType == OBJ_MONSTER)
			{
				if (pstAtkeeInfo->stAtkee.stObj.pstMon->stMonAtkInfo.cAtkStat == CS_STAT_PREPARE)
				{
					z_mon_skill_break(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
				}
			}
			
			break;
		case RESULT_MON_OWNER_ADDHP_HIT_PLAYER:
			{
				int iMul = 10000;
				int iOldHP;
				Monster *pstOwner;
				
				if(pstAtkerInfo->stAtker.iType != OBJ_MONSTER)
				{
					return -1;
				}
				pstMon = pstAtkerInfo->stAtker.stObj.pstMon;
				
				if(pstAtkeeInfo->stAtkee.iType != OBJ_PLAYER)
				{
					return -1;
				}
				pstPlayer = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
					
				pstMidAttrList = &pstPlayer->stOnline.stDyn.stMidAttrList;
				iTmp = pstMidAttrList->uiHPMax *(pstResult->SkillResultVal2/10000.0) ;
				if (pstOneHit->TotalHurt < MAX_HURT)
				{
					pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
					pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iTmp;
					pstOneHit->TotalHurt++;
				}

				if(pstPlayer->stRoleData.RoleDetail.HPCurr < (unsigned int)iTmp)
				{
					iMul = pstPlayer->stRoleData.RoleDetail.HPCurr*1.0/iTmp*10000;
				}

				iMul = iMul*pstResult->SkillResultVal1/10000;
				
				pstOwner = z_id_monster(pstAppCtx, pstEnv, pstMon->iCallMonID);
				if(!pstOwner)
				{
					return -1;
				}

				iTmp = pstOwner->iMaxHP*(iMul/10000.0);
				iOldHP = pstOwner->iHP;
				pstOwner->iHP += iTmp;
				if(pstOwner->iHP > pstOwner->iMaxHP)
				{
					pstOwner->iHP = pstOwner->iMaxHP;
				}

				mon_hp_action(pstEnv, pstOwner, iOldHP, 0);
			}
			break;
		case RESULT_TRANSMIT_TO_SPOUSE_POS:
			{
				RESPOS stPos;
				MapIndex *pstMapIdx;
				ZONEIDX stIdx;
				ZONEMAPINST *pstMapInst;
				int iMap;
				Player *pstPlayerOther = NULL;
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;

				if (pstAtkerInfo->stAtker.iType != OBJ_PLAYER || !pstPlayer)
				{
					return -1;
				}

				if(0 > z_skill_transmit_spouse_pos_check(pstAppCtx,pstEnv,pstPlayer))
				{
					return -1;
				}

				pstPlayerOther = z_name_player(pstAppCtx, pstEnv, pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);
				if (!pstPlayerOther)
				{
					z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
					return -1;	
				}

				stIdx.iID = pstPlayerOther->stRoleData.Map;
				stIdx.iPos = pstPlayerOther->stOnline.iMapPos;
				pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
				if (NULL == pstMapInst)
				{
					return -1;
				}

				pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
				if (NULL == pstMapIdx)
				{
					return -1;
				}

				iMap = pstPlayerOther->stRoleData.Map;
				z_get_ran_pos(pstEnv, pstMapInst, pstMapIdx, &pstPlayerOther->stRoleData.Pos, &pstPlayerOther->stRoleData.Pos, 400, 50,  &stPos);

				z_player_change_map2(pstAppCtx, pstEnv, pstPlayer, iMap, &stPos, RAND1(180), 0);
			}
			break;
		case RESULT_RECOVER_SPOUSE_HP:
			{
				Player *pstPlayerOther = NULL;
				pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
				int iTmpHP = 0;
				int iOldHP = 0;
				int iOldMP = 0;

				if (pstAtkerInfo->stAtker.iType != OBJ_PLAYER || !pstPlayer)
				{
					return -1;
				}

				if(0 > z_skill_recover_spouse_hp_check(pstAppCtx,pstEnv,pstPlayer,pstAtkerInfo->pstSkillDef))
				{
					return -1;
				}

				if (INST_MODE_ZONE_SVR==pstEnv->iInstMode)
				{
					pstPlayerOther = z_name_player(pstAppCtx, pstEnv, pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);
				}
				else
				{
					char szName[NAME_LEN];
					unsigned short unWorldID;
					
					unWorldID = role_world_id_get(pstPlayer);
					
					snprintf(szName,sizeof(szName),"w%d"BATTLE_NAME_SYMBOL"%s",
									unWorldID,
									pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);

					pstPlayerOther = z_name_player(pstAppCtx, pstEnv, szName);
				}
				
				if (!pstPlayerOther)
				{
					z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
					return -1;	
				}

				iOldHP = (int)pstPlayerOther->stRoleData.RoleDetail.HPCurr;
				iOldMP = (int)pstPlayerOther->stRoleData.RoleDetail.MPCurr;
				
				iTmpHP = pstResult->SkillResultVal1/100.0*pstPlayerOther->stOnline.stDyn.stMidAttrList.uiHPMax;

				if(iOldHP + iTmpHP > (int)pstPlayerOther->stOnline.stDyn.stMidAttrList.uiHPMax)
				{
					iTmpHP = pstPlayerOther->stOnline.stDyn.stMidAttrList.uiHPMax - iOldHP;
				}

				if(0 == iTmpHP)
				{
					return -1;
				}
						
				stAttr.AttrNum = 0;
				
				stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_HP;
				stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
				stAttr.AttrList[stAttr.AttrNum].Value.HP = iTmpHP;
				stAttr.AttrNum++;

				z_attr_chg(pstAppCtx, pstEnv, pstPlayerOther, &stAttr, 1);

				if (iOldHP != (int)pstPlayerOther->stRoleData.RoleDetail.HPCurr ||
					iOldMP != (int)pstPlayerOther->stRoleData.RoleDetail.MPCurr)
				{
					player_hp_action(pstEnv, pstPlayerOther, iOldHP, iOldMP, 0);
				}
			}
			break;
		default:
			break;
	}

	return 0;
}

int z_do_extended_result(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                         AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo,
                         const ROLEARM *pstArm, int iIndex)
{
	const EXTENDEDRESULT *pstResult = &pstArm->ExtendedResult;
	Player *pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PLAYER ?
	                    pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer :
	                    NULL;
	if(!pstPlayer)
	{
		return -1;
	}

	if(pstPlayer->stOnline.tExtendedResultEnd[iIndex] &&
	   pstEnv->pstAppCtx->stCurr.tv_sec <=
	   pstPlayer->stOnline.tExtendedResultEnd[iIndex])
	{
		return -1;
	}

	switch(pstResult->ResultID)
	{
	case RESULT_STATUS: // 给敌人加dot
		if(z_calc_apply_status(pstAppCtx, pstEnv, pstAtkProcess,
		                       pstAtkeeInfo, pstResult->ResultVal2, 
		                       pstResult->ResultVal3, pstResult->ResultVal4/*,
		                       FROM_EXTENDED*/) == 0)
		{
			pstPlayer->stOnline.tExtendedResultEnd[iIndex] =
				pstAppCtx->stCurr.tv_sec +
				z_extend_item_property(pstEnv, pstArm->ExtendedFromDefID, 1);
		}
		break;

	//case ... 其他效果添加到此处
	//此处新加其他效果以后还要在z_wear_extended_bit_set里添加必要代码
	
	default:
		break;
	}
	return 0;
}

int z_do_extended_attr(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                       AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo,
                       ATKCHG *pstAtkChg)
{
	tdr_ulonglong nBitmap = 0;
	const ROLEWEAR *pstRoleWear = NULL;
	Player *pstPlayer = pstAtkProcess->stAtkerInfo.stAtker.iType == OBJ_PLAYER ?
	                    pstAtkProcess->stAtkerInfo.stAtker.stObj.pstPlayer :
	                    NULL;
	if(!pstPlayer)
	{
		return -1;
	}

	pstRoleWear = &pstPlayer->stRoleData.Wear;
	nBitmap = pstPlayer->stOnline.nExtendedResultBitMap;
	if(nBitmap >= (1ULL << MAX_WEAR))
	{
		// nBitmap最大值应该是 (1U << (MAX_WEAR + 1)) - 1 == 0x00003FFF
		return -1;
	}

	while(nBitmap)
	{
		tdr_ulonglong nBit = (nBitmap & ~(nBitmap - 1));
		int iIndex = (int)log2(nBit); // 必定不会越界的
		const ROLEARM *pstArm = pstRoleWear->RoleArms + iIndex;
		
		z_do_extended_result(pstAppCtx, pstEnv, pstAtkProcess, 
			pstAtkeeInfo, pstArm, iIndex);
		
		nBitmap &= ~nBit;
	}
	return 0;
	UNUSED(pstAtkChg);	//add by paraunused.pl
}

// 这个函数处理的效果是一次攻击才出现的,而非每一击都触发
int z_do_final_extend_result(ZONESVRENV *pstEnv, Player *pstPlayer,
                             const ROLEARM *pstArm, int iIndex)
{
	const EXTENDEDRESULT *pstResult = &pstArm->ExtendedResult;

	if(pstPlayer->stOnline.tExtendedResultEnd[iIndex] &&
	   pstEnv->pstAppCtx->stCurr.tv_sec <=
	   pstPlayer->stOnline.tExtendedResultEnd[iIndex])
	{
		return -1;
	}

	switch(pstResult->ResultID)
	{
	case RESULT_SELF_STATUS: // 给自己加buff
		if((int)(RAND1(10000)) < pstResult->ResultVal1*100.0)
		{
			if(z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer,
			                         pstResult->ResultVal2, 
								     pstResult->ResultVal3,
								     pstResult->ResultVal4, 1, 0) == 0)
			{
				pstPlayer->stOnline.tExtendedResultEnd[iIndex] =
					pstEnv->pstAppCtx->stCurr.tv_sec +
					z_extend_item_property(pstEnv, pstArm->ExtendedFromDefID, 1);
			}
		}
		break;

	//case ... 其他效果添加到此处
	//此处新加其他效果以后还要在z_wear_extended_bit_set里添加必要代码
	
	default:
		break;
	}
	return 0;
}

int z_final_extend_attr(ZONESVRENV *pstEnv, Player *pstPlayer, int iAtkChgIdx)
{
	const ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;
	tdr_ulonglong nBitmap = pstPlayer->stOnline.nExtendedResultBitMap;
	const ATKCHG *pstAtkChg = NULL;
	if(nBitmap >= (1ULL << MAX_WEAR))
	{
		// nBitmap最大值应该是 (1U << (MAX_WEAR + 1)) - 1 == 0x00003FFF
		return -1;
	}
	if(iAtkChgIdx < 0)
	{
		return -1;
	}
	pstAtkChg = &pstPlayer->stOnline.stDyn.stAtkChg;

	while(nBitmap)
	{
		tdr_ulonglong nBit = (nBitmap & ~(nBitmap - 1));
		int iIndex = (int)log2(nBit); // 必定不会越界的
		const ROLEARM *pstArm = pstRoleWear->RoleArms + iIndex;
		z_do_final_extend_result(pstEnv, pstPlayer, pstArm, iIndex);
		nBitmap &= ~nBit;
	}
	return 0;
}
// 放技能被闪避了加状态
int z_do_skill_result_by_flee(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,SKILLDEF *pstSkillDef,Player *pstPlayer,Monster *pstMon)
{
	SKILLRESULT *pstResult = NULL;
	
	pstResult = z_get_skill_result(pstSkillDef,RESULT_ADD_STATUS_BY_FLEE);
	if( !pstResult )
	{
		return -1;
	}

	if ( (int)(RAND1(10000)) >= pstResult->SkillResultVal1*100 )
	{
		return -1;
	}

	if(pstPlayer)
	{
		z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, pstResult->SkillResultVal2, pstResult->SkillResultVal3, 
				pstResult->SkillResultVal4, 1, 0);
	}

	if(pstMon)
	{
		z_mon_apply_status(pstAppCtx, pstEnv, pstMon, pstResult->SkillResultVal2, pstResult->SkillResultVal3, 
				pstResult->SkillResultVal4, 1, 0);
	}
	
	return 0;
}
int z_do_skill_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo)
{
	int i;
	SKILLRESULT *pstResult;
	Player *pstPlayer;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	STATUSRESULT *pstInvalidResult = NULL;
	
	//被控制技能攻击
	if (is_ctrl_skill(pstSkillDef))
	{
		Player *pstAtkee = NULL;
		
		if(OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
		{
			pstAtkee = pstAtkeeInfo->stAtkee.stObj.pstPlayer;
		}
		else if(OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
		{
			if(is_mon_machine(pstAtkeeInfo->stAtkee.stObj.pstMon))
			{
				pstAtkee = machine_get_driver(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon);
			}
		}

		if(pstAtkee)
		{
			player_trace_act_break_prepare(pstEnv, pstAtkee, BRK_PRE_ACT_BEATK_CTRLSKILL);
		}
	}

	if(OBJ_PLAYER == pstAtkeeInfo->stAtkee.iType)
	{
		pstInvalidResult = player_status_result(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstPlayer,STATUS_RESULT_INVALID_SKILLRESULLTID);
	}
	else if(OBJ_MONSTER == pstAtkeeInfo->stAtkee.iType)
	{
		pstInvalidResult = mon_status_result(pstEnv, pstAtkeeInfo->stAtkee.stObj.pstMon,STATUS_RESULT_INVALID_SKILLRESULLTID);
	}
	
	for (i=0; i<MAX_SKILL_RESULT; i++)
	{
		pstResult = pstSkillDef->SkillResult+i;
		if (RESULT_NONE == pstResult->SkillResultID)
		{
			break;
		}
		if( (pstInvalidResult && pstInvalidResult->ResultVal1 && (pstInvalidResult->ResultVal1 == pstResult->SkillResultID)) ||
			(pstInvalidResult && pstInvalidResult->ResultVal2 && (pstInvalidResult->ResultVal2 == pstResult->SkillResultID)) ||
			(pstInvalidResult && pstInvalidResult->ResultVal3 && (pstInvalidResult->ResultVal3 == pstResult->SkillResultID)) ||
			(pstInvalidResult && pstInvalidResult->ResultVal4 && (pstInvalidResult->ResultVal4 == pstResult->SkillResultID)) )
		{
			//指定的技能效果无效
		}
		else
		{
			z_do_skill_result(pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, pstResult);
		}
	}

	for (i=0; i<MAX_TARGET_STATUS; i++)
	{
		if (pstAtkProcess->stAtkerInfo.ucAppendResult[i])
		{
			z_do_skill_result(pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, &pstSkillDef->TargetStatus[i].SkillResult);
		}
	}

	if (OBJ_PLAYER == pstAtkerInfo->stAtker.iType) //技能配件效果
	{
		pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;

		//成长之路
		{
			SkillDefTail* pstTail = z_get_skill_tail(pstSkillDef);
			if (pstTail && pstTail->iAchievementID)
			{
				z_player_achievement_done(pstEnv,pstPlayer,pstTail->iAchievementID);
			}
		}
		/*
		for (i=0; i<MAX_FITTINGS_GRID; i++)
		{
			pstFittingsGridData = &pstFittings->FittingsMap.FittingsGrids[i];
			if (pstFittingsGridData->FittingsID == 0)
			{
				continue;
			}

			pstFittingsDef = z_find_fittings(pstEnv, pstFittingsGridData->FittingsID, pstFittingsGridData->Level,
				pstFittingsGridData->GroupID,&pstFittingsGridData->DefIdx);
			if (!pstFittingsDef)
			{
				continue;
			}

			for (j=0; j<(int)(sizeof(pstFittingsDef->FittingsResult)/sizeof(pstFittingsDef->FittingsResult[0])); j++)
			{
				pstResult = &pstFittingsDef->FittingsResult[j];
				if (pstResult->SkillResultID == 0)
				{
					break;
				}
				z_do_skill_result(pstAppCtx, pstEnv, pstAtkProcess, pstAtkeeInfo, pstResult);
			}
		}
		*/
	}

	return 0;
}

int z_do_no_target_skill_attr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess)
{
	int i;
	RESPOS stPos;
	SKILLRESULT *pstResult;
	Monster *pstMon;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	ZONEMAPINST *pstMapInst;
	
	for (i=0; i<MAX_SKILL_RESULT; i++)
	{
		pstResult = pstSkillDef->SkillResult+i;
		if (RESULT_NONE == pstResult->SkillResultID)
		{
			break;
		}

		switch (pstResult->SkillResultID)
		{
			case RESULT_NO_TARGET_CALL:
				if(pstAtkerInfo->stAtker.iType == OBJ_MONSTER)
				{
					pstMon = pstAtkerInfo->stAtker.stObj.pstMon;
					pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
					if(!pstMapInst)
					{
						return -1;
					}
					stPos = pstMon->stMonAtkInfo.stAtkPos;
				}
				else
				{
					return -1;
				}
				
				{
					if (pstResult->SkillResultVal3 > 0 && pstResult->SkillResultVal3<= MAX_SKILL_RESULT)
					{
						z_do_call(pstEnv, NULL, pstAtkerInfo, pstMapInst,pstResult->SkillResultVal2, 
							pstResult->SkillResultVal1, pstResult->SkillResultVal3, pstResult->SkillResultVal4, &stPos,
							&pstSkillDef->SkillResult[pstResult->SkillResultVal3-1]);
					}
					else
					{
						z_do_call(pstEnv, NULL, pstAtkerInfo,
					          pstMapInst, pstResult->SkillResultVal2,
					          pstResult->SkillResultVal1, pstResult->SkillResultVal3,
					          pstResult->SkillResultVal4, &stPos, NULL);
					}
				}
				break;
			default:
				break;
		}
	}

	return 0;
}

int z_add_dead_resource(ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, MONSTERDEF *pstMonsterDef, RESOURCEADDCHG *pstResourceChg)
{
	int iTmp;
	SKILLDEF *pstSkillDef = pstAtkProcess->stAtkerInfo.pstSkillDef;

	UNUSED(pstEnv);
	
	if (pstSkillDef->MPDeadAdd > 0)
	{
		iTmp = pstSkillDef->MPDeadAdd*(1 + pstResourceChg->unMPDeadMul/100.0)*(1+pstMonsterDef->DeadAddResourceMul/100.0) + 
				pstResourceChg->unMPDeadAdd;

		pstAtkProcess->stAtkerInfo.stResourceConsume.iMPUse -= iTmp + pstAtkProcess->ushKillAddMpNum;
	}

	/*if (pstSkillDef->CollectNumDeadAdd > 0)
	{
		iTmp = pstSkillDef->CollectNumDeadAdd*(1+pstResourceChg->unCollectNumDeadMul/100.0)*
				(1+pstMonsterDef->DeadAddResourceMul/100.0)  + pstResourceChg->unCollectNumDeadAdd;

		pstAtkProcess->stAtkerInfo.stCollectNumAdd.iCollectNumDeadAdd += iTmp + pstAtkProcess->ushKillAddCollectNum;
	}*/
	
	return 0;
}

int z_add_atk_resource(ZONESVRENV* pstEnv, AtkProcess *pstAtkProcess, MONSTERDEF *pstMonsterDef, RESOURCEADDCHG *pstResourceChg)
{
	int iTmp;
	SKILLDEF *pstSkillDef = pstAtkProcess->stAtkerInfo.pstSkillDef;

	UNUSED(pstEnv);
	UNUSED(pstMonsterDef);
	
	if (pstSkillDef->MPAtkAdd > 0)
	{
		iTmp = pstSkillDef->MPAtkAdd*(1 + pstResourceChg->unMPAtkMul/100.0) + 
				pstResourceChg->unMPAtkAdd;

		pstAtkProcess->stAtkerInfo.stResourceConsume.iMPUse -= iTmp;
	}

	/*if (pstSkillDef->CollectNumAtkAdd > 0)
	{
		iTmp = pstSkillDef->CollectNumAtkAdd*(1+pstResourceChg->unCollectNumAtkMul/100.0) + 
				pstResourceChg->unCollectNumAtkAdd;

		pstAtkProcess->stAtkerInfo.stCollectNumAdd.iCollectNumAtkAdd += iTmp;
	}*/
	
	return 0;
}

/*
int z_cacl_status_result(DYNATTRLIST *pstDyn, StatusTmpAttr *pstTmpAttr, ATKCHG *pstAtkChg, STATUSRESULT *pstResult)
{
	switch(pstResult->ResultID)
	{
		case STATUS_RESULT_FEAR_ICEHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeFearIceHurtMul)
			{
				pstTmpAttr->AtkeeFearIceHurtMul = pstResult->ResultVal1;
			}
			
			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeFearIceHurtAdd)
			{
				pstTmpAttr->AtkeeFearIceHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_FEAR_FIREHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeFearFireHurtMul)
			{
				pstTmpAttr->AtkeeFearFireHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeFearFireHurtAdd)
			{
				pstTmpAttr->AtkeeFearFireHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_FEAR_THUNDERHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeFearThunderHurtMul)
			{
				pstTmpAttr->AtkeeFearThunderHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeFearThunderHurtAdd)
			{
				pstTmpAttr->AtkeeFearThunderHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_FEAR_LIGHTHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeFearLightHurtMul)
			{
				pstTmpAttr->AtkeeFearLightHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeFearLightHurtAdd)
			{
				pstTmpAttr->AtkeeFearLightHurtAdd = pstResult->ResultVal2;
			}
			
			break;
		case STATUS_RESULT_FEAR_NIGHTHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeFearNightHurtMul)
			{
				pstTmpAttr->AtkeeFearNightHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeFearNightHurtAdd)
			{
				pstTmpAttr->AtkeeFearNightHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_FEAR_PHISICHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeFearPhsicHurtMul)
			{
				pstTmpAttr->AtkeeFearPhsicHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeFearPhsicHurtAdd)
			{
				pstTmpAttr->AtkeeFearPhsicHurtAdd = pstResult->ResultVal2;
			}
			
			break;
		case STATUS_RESULT_DEF_ICEHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeIceHurtMul)
			{
				pstTmpAttr->AtkeeIceHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeIceHurtAdd)
			{
				pstTmpAttr->AtkeeIceHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_DEF_FIREHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeFireHurtMul)
			{
				pstTmpAttr->AtkeeFireHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeFireHurtAdd)
			{
				pstTmpAttr->AtkeeFireHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_DEF_THUNDERHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeThunderHurtMul)
			{
				pstTmpAttr->AtkeeThunderHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeThunderHurtAdd)
			{
				pstTmpAttr->AtkeeThunderHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_DEF_LIGHTHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeLightHurtMul)
			{
				pstTmpAttr->AtkeeLightHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeLightHurtAdd)
			{
				pstTmpAttr->AtkeeLightHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_DEF_NIGHTHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeeNightHurtMul)
			{
				pstTmpAttr->AtkeeNightHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeeNightHurtAdd)
			{
				pstTmpAttr->AtkeeNightHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_DEF_PHISICHURT:
			if (pstResult->ResultVal1 > pstTmpAttr->AtkeePhsicHurtMul)
			{
				pstTmpAttr->AtkeePhsicHurtMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->AtkeePhsicHurtAdd)
			{
				pstTmpAttr->AtkeePhsicHurtAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_ICEATK:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nIceAtkChgMul)
				{
					pstTmpAttr->nIceAtkChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iIceAtkChgAdd)
				{
					pstTmpAttr->iIceAtkChgAdd = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_ICEATK_DEC:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nDecIceAtkChgMul)
				{
					pstTmpAttr->nDecIceAtkChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iDecIceAtkChgAdd)
				{
					pstTmpAttr->iDecIceAtkChgAdd = pstResult->ResultVal2;
				}
			}
			break;	
		case STATUS_RESULT_FIREATK:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nFireAtkChgMul)
				{
					pstTmpAttr->nFireAtkChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iFireAtkChgAdd)
				{
					pstTmpAttr->iFireAtkChgAdd = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_THUNDERATK:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nThunderAtkChgMul)
				{
					pstTmpAttr->nThunderAtkChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iThunderAtkChgAdd)
				{
					pstTmpAttr->iThunderAtkChgAdd = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_LIGHTATK:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nLightAtkChgMul)
				{
					pstTmpAttr->nLightAtkChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iLightAtkChgAdd)
				{
					pstTmpAttr->iLightAtkChgAdd = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_NIGHTATK:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nNightAtkChgMul)
				{
					pstTmpAttr->nNightAtkChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iNightAtkChgAdd)
				{
					pstTmpAttr->iNightAtkChgAdd = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_PHISICATK:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nPhsicAtkChgMul)
				{
					pstTmpAttr->nPhsicAtkChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iPhsicAtkChgAdd)
				{
					pstTmpAttr->iPhsicAtkChgAdd = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_PHSICATK_DEC:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nDecPhsicAtkChgMul)
				{
					pstTmpAttr->nDecPhsicAtkChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iDecPhsicAtkChgAdd)
				{
					pstTmpAttr->iDecPhsicAtkChgAdd = pstResult->ResultVal2;
				}
			}
			break;	
		case STATUS_RESULT_ICEDEF:
			if (pstResult->ResultVal1 > pstTmpAttr->nIceDefChgMul)
			{
				pstTmpAttr->nIceDefChgMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->iIceDefChgAdd)
			{
				pstTmpAttr->iIceDefChgAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_ICEDEF_DEC:
			if (pstResult->ResultVal1 > pstTmpAttr->nDecIceDefChgMul)
			{
				pstTmpAttr->nDecIceDefChgMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->iDecIceDefChgAdd)
			{
				pstTmpAttr->iDecIceDefChgAdd = pstResult->ResultVal2;
			}
			break;	
		case STATUS_RESULT_FIREDEF:
			if (pstResult->ResultVal1 > pstTmpAttr->nFireDefChgMul)
			{
				pstTmpAttr->nFireDefChgMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->iFireDefChgAdd)
			{
				pstTmpAttr->iFireDefChgAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_THUNDERDEF:
			if (pstResult->ResultVal1 > pstTmpAttr->nThunderDefChgMul)
			{
				pstTmpAttr->nThunderDefChgMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->iThunderDefChgAdd)
			{
				pstTmpAttr->iThunderDefChgAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_LIGHTDEF:
			if (pstResult->ResultVal1 > pstTmpAttr->nLightDefChgMul)
			{
				pstTmpAttr->nLightDefChgMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->iLightDefChgAdd)
			{
				pstTmpAttr->iLightDefChgAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_NIGHTDEF:
			if (pstResult->ResultVal1 > pstTmpAttr->nNightDefChgMul)
			{
				pstTmpAttr->nNightDefChgMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->iNightDefChgAdd)
			{
				pstTmpAttr->iNightDefChgAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_PHISICDEF:
			if (pstResult->ResultVal1 > pstTmpAttr->nPhsicDefChgMul)
			{
				pstTmpAttr->nPhsicDefChgMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->iPhsicDefChgAdd)
			{
				pstTmpAttr->iPhsicDefChgAdd = pstResult->ResultVal2;
			}
			break;
		case STATUS_RESULT_PHSICDEF_DEC:
			if (pstResult->ResultVal1 > pstTmpAttr->nDecPhsicDefChgMul)
			{
				pstTmpAttr->nDecPhsicDefChgMul = pstResult->ResultVal1;
			}

			if (pstResult->ResultVal2 > pstTmpAttr->iDecPhsicDefChgAdd)
			{
				pstTmpAttr->iDecPhsicDefChgAdd = pstResult->ResultVal2;
			}
			break;	
             case STATUS_RESULT_ICEHURT:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkerIceHurtMul)
				{
					pstTmpAttr->AtkerIceHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkerIceHurtAdd)
				{
					pstTmpAttr->AtkerIceHurtAdd  = pstResult->ResultVal2;
				}
			}	
			break;
		case STATUS_RESULT_ICEHURT_DEC:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->DecAtkerIceHurtMul)
				{
					pstTmpAttr->DecAtkerIceHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->DecAtkerIceHurtAdd)
				{
					pstTmpAttr->DecAtkerIceHurtAdd  = pstResult->ResultVal2;
				}
			}	
			break;	
		case STATUS_RESULT_FIREHURT:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkerFireHurtMul)
				{
					pstTmpAttr->AtkerFireHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkerFireHurtAdd)
				{
					pstTmpAttr->AtkerFireHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_THUNDERHURT:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkerThunderHurtMul)
				{
					pstTmpAttr->AtkerThunderHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkerThunderHurtAdd)
				{
					pstTmpAttr->AtkerThunderHurtAdd  = pstResult->ResultVal2;

				}
			}
			break;
		case STATUS_RESULT_LIGHTHURT:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkerLightHurtMul)
				{
					pstTmpAttr->AtkerLightHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkerLightHurtAdd)
				{
					pstTmpAttr->AtkerLightHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_NIGHTHURT:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkerNightHurtMul)
				{
					pstTmpAttr->AtkerNightHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkerNightHurtAdd)
				{
					pstTmpAttr->AtkerNightHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_PHSICHURT:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkerPhsicHurtMul)
				{
					pstTmpAttr->AtkerPhsicHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkerPhsicHurtAdd)
				{
					pstTmpAttr->AtkerPhsicHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_PHSICHURT_DEC:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->DecAtkerPhsicHurtMul)
				{
					pstTmpAttr->DecAtkerPhsicHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->DecAtkerPhsicHurtAdd)
				{
					pstTmpAttr->DecAtkerPhsicHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;	
		case STATUS_RESULT_MAGICHURT:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkerMagicHurtMul)
				{
					pstTmpAttr->AtkerMagicHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkerMagicHurtAdd)
				{
					pstTmpAttr->AtkerMagicHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_MAGICHURT_DEC:
			
			{
				if (pstResult->ResultVal1 > pstTmpAttr->DecAtkerMagicHurtMul)
				{
					pstTmpAttr->DecAtkerMagicHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->DecAtkerMagicHurtAdd)
				{
					pstTmpAttr->DecAtkerMagicHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;	
		case STATUS_RESULT_HURT:
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkerHurtMul)
				{
					pstTmpAttr->AtkerHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkerHurtAdd)
				{
					pstTmpAttr->AtkerHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;	
		case STATUS_RESULT_HURT_DEC:
			{
				if (pstResult->ResultVal1 > pstTmpAttr->DecAtkerHurtMul)
				{
					pstTmpAttr->DecAtkerHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->DecAtkerHurtAdd)
				{
					pstTmpAttr->DecAtkerHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;	
		case STATUS_RESULT_FIGHTVAL: //状态对战斗力的影响都是负的
			if (pstResult->ResultVal1 < pstTmpAttr->iFight)
			{
				pstTmpAttr->iFight = pstResult->ResultVal1;
			}
			break;
		case STATUS_RESULT_BAOJI_RATE:
			if (pstResult->ResultVal1 > pstTmpAttr->nHeavyHitAdd)
			{
				pstTmpAttr->nHeavyHitAdd = pstResult->ResultVal1;
			}
			break;
		case STATUS_RESULT_HIT_RATE:
			if (pstResult->ResultVal1 > pstTmpAttr->nAtkerHitRateAdd)
			{
				pstTmpAttr->nAtkerHitRateAdd = pstResult->ResultVal1;
			}
			break;
		case STATUS_RESULT_FLEE_VAL:
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nFleeChgMul)
				{
					pstTmpAttr->nFleeChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iFleeChgAdd)
				{
					pstTmpAttr->iFleeChgAdd  = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_HIT_VAL:
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nHitChgMul)
				{
					pstTmpAttr->nHitChgMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iHitChgAdd)
				{
					pstTmpAttr->iHitChgAdd  = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_HEAVY_HIT_VAL:
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nHeavyHitVal)
				{
					pstTmpAttr->nHeavyHitVal = pstResult->ResultVal1;
				}
			}
			break;
		case STATUS_RESULT_DEF_HURT:
			{
				if (pstResult->ResultVal1 > pstTmpAttr->AtkeeHurtMul)
				{
					pstTmpAttr->AtkeeHurtMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->AtkeeHurtAdd)
				{
					pstTmpAttr->AtkeeHurtAdd  = pstResult->ResultVal2;
				}
			}
			break;
		case STATUS_RESULT_DAMAGE:
			{
				if (pstResult->ResultVal1 > pstTmpAttr->nDamageMul)
				{
					pstTmpAttr->nDamageMul = pstResult->ResultVal1;
				}

				if (pstResult->ResultVal2 > pstTmpAttr->iDamageAdd)
				{
					pstTmpAttr->iDamageAdd  = pstResult->ResultVal2;
				}
			}
		break;
		default:
			break;
	}
	
	return 0;
}
*/

// 计算契约的攻防修正数值
int z_cacl_player_qiyue_result(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ATKCHG *pstAtkChg, Player *pstPlayer)
{
	QIYUERESULT stResultAtk, stResultDef;
	DEFCHG *pstDefChg = &pstDyn->stDefChg;
	ZoneTeam * pstZoneTeam = player_team_get( pstEnv, pstPlayer );

	// 没有组队或没有有效的契约，就不计算了。
	if( !pstZoneTeam || pstZoneTeam->stTeamInfo.QiYue == 0 )
	{
		return 0;
	}
	
	memset( &stResultAtk, 0, sizeof(stResultAtk) );
	memset( &stResultDef, 0, sizeof(stResultDef) );

	// 获取攻防加成的契约效果	
	stResultAtk.QiYueResultID = QIYUE_RESULT_ID_ATK;
	if( player_team_checked_qiyue_result( pstEnv, pstPlayer, pstZoneTeam, 
		&stResultAtk ) == 0 )
	{
		pstAtkChg->nIceAtkChgMul += stResultAtk.ResultVal1;
		pstAtkChg->iIceAtkChgAdd += stResultAtk.ResultVal2;

		pstAtkChg->nFireAtkChgMul += stResultAtk.ResultVal1;
		pstAtkChg->iFireAtkChgAdd += stResultAtk.ResultVal2;

		pstAtkChg->nThunderAtkChgMul += stResultAtk.ResultVal1;
		pstAtkChg->iThunderAtkChgAdd += stResultAtk.ResultVal2;

		pstAtkChg->nLightAtkChgMul += stResultAtk.ResultVal1;
		pstAtkChg->iLightAtkChgAdd += stResultAtk.ResultVal2;

		pstAtkChg->nNightAtkChgMul += stResultAtk.ResultVal1;
		pstAtkChg->iNightAtkChgAdd += stResultAtk.ResultVal2;

		pstAtkChg->nPhsicAtkChgMul += stResultAtk.ResultVal1;
		pstAtkChg->iPhsicAtkChgAdd += stResultAtk.ResultVal2;
	}

	stResultDef.QiYueResultID = QIYUE_RESULT_ID_DEF;
	if( player_team_checked_qiyue_result( pstEnv, pstPlayer, pstZoneTeam, 
		&stResultDef ) == 0 )
	{
		pstDefChg->nIceDefChgMul += stResultDef.ResultVal1;
		pstDefChg->iIceDefChgAdd += stResultDef.ResultVal2;

		pstDefChg->nFireDefChgMul += stResultDef.ResultVal1;
		pstDefChg->iFireDefChgAdd += stResultDef.ResultVal2;

		pstDefChg->nThunderDefChgMul += stResultDef.ResultVal1;
		pstDefChg->iThunderDefChgAdd += stResultDef.ResultVal2;

		pstDefChg->nLightDefChgMul += stResultDef.ResultVal1;
		pstDefChg->iLightDefChgAdd += stResultDef.ResultVal2;

		pstDefChg->nNightDefChgMul += stResultDef.ResultVal1;
		pstDefChg->iNightDefChgAdd += stResultDef.ResultVal2;

		pstDefChg->nPhsicDefChgMul += stResultDef.ResultVal1;
		pstDefChg->iPhsicDefChgAdd += stResultDef.ResultVal2;
	}
	
	return 0;
}

/*
int z_cacl_status_result_real(DYNATTRLIST *pstDyn, StatusTmpAttr *pstTmpAttr, ATKCHG *pstAtkChg)
{
	ATKEEHURTCHG *pstAtkeeHurtChg = &pstDyn->stAtkeeHurtChg;
	DEFCHG *pstDefChg = &pstDyn->stDefChg;
	long long llFight;
	
	pstAtkeeHurtChg->AtkeeIceHurtMul = pstAtkeeHurtChg->AtkeeIceHurtMul - pstTmpAttr->AtkeeFearIceHurtMul + pstTmpAttr->AtkeeIceHurtMul;
	pstAtkeeHurtChg->AtkeeIceHurtAdd = pstAtkeeHurtChg->AtkeeIceHurtAdd - pstTmpAttr->AtkeeFearIceHurtAdd + pstTmpAttr->AtkeeIceHurtAdd;

	pstAtkeeHurtChg->AtkeeFireHurtMul = pstAtkeeHurtChg->AtkeeFireHurtMul - pstTmpAttr->AtkeeFearFireHurtMul + pstTmpAttr->AtkeeFireHurtMul;
	pstAtkeeHurtChg->AtkeeFireHurtAdd = pstAtkeeHurtChg->AtkeeFireHurtAdd - pstTmpAttr->AtkeeFearFireHurtAdd + pstTmpAttr->AtkeeFireHurtAdd;

	pstAtkeeHurtChg->AtkeeThunderHurtMul = pstAtkeeHurtChg->AtkeeThunderHurtMul - pstTmpAttr->AtkeeFearThunderHurtMul + pstTmpAttr->AtkeeThunderHurtMul;
	pstAtkeeHurtChg->AtkeeThunderHurtAdd = pstAtkeeHurtChg->AtkeeThunderHurtAdd - pstTmpAttr->AtkeeFearThunderHurtAdd + pstTmpAttr->AtkeeThunderHurtAdd;

	pstAtkeeHurtChg->AtkeeLightHurtMul = pstAtkeeHurtChg->AtkeeLightHurtMul - pstTmpAttr->AtkeeFearLightHurtMul + pstTmpAttr->AtkeeLightHurtMul;
	pstAtkeeHurtChg->AtkeeLightHurtAdd = pstAtkeeHurtChg->AtkeeLightHurtAdd - pstTmpAttr->AtkeeFearLightHurtAdd + pstTmpAttr->AtkeeLightHurtAdd;

	pstAtkeeHurtChg->AtkeeNightHurtMul = pstAtkeeHurtChg->AtkeeNightHurtMul - pstTmpAttr->AtkeeFearNightHurtMul + pstTmpAttr->AtkeeNightHurtMul;
	pstAtkeeHurtChg->AtkeeNightHurtAdd = pstAtkeeHurtChg->AtkeeNightHurtAdd - pstTmpAttr->AtkeeFearNightHurtAdd + pstTmpAttr->AtkeeNightHurtAdd;

	pstAtkeeHurtChg->AtkeePhsicHurtMul = pstAtkeeHurtChg->AtkeePhsicHurtMul - pstTmpAttr->AtkeeFearPhsicHurtMul + pstTmpAttr->AtkeePhsicHurtMul;
	pstAtkeeHurtChg->AtkeePhsicHurtAdd = pstAtkeeHurtChg->AtkeePhsicHurtAdd - pstTmpAttr->AtkeeFearPhsicHurtAdd + pstTmpAttr->AtkeePhsicHurtAdd;

	pstAtkeeHurtChg->AtkeeMagicHurtMul = pstAtkeeHurtChg->AtkeeMagicHurtMul - pstTmpAttr->AtkeeFearMagicHurtMul + pstTmpAttr->AtkeeMagicHurtMul;
	pstAtkeeHurtChg->AtkeeMagicHurtAdd = pstAtkeeHurtChg->AtkeeMagicHurtAdd - pstTmpAttr->AtkeeFearMagicHurtAdd + pstTmpAttr->AtkeeMagicHurtAdd;

	pstAtkeeHurtChg->AtkeeHurtMul = pstAtkeeHurtChg->AtkeeHurtMul - pstTmpAttr->AtkeeFearHurtMul + pstTmpAttr->AtkeeHurtMul;
	pstAtkeeHurtChg->AtkeeHurtAdd = pstAtkeeHurtChg->AtkeeHurtAdd - pstTmpAttr->AtkeeFearHurtAdd + pstTmpAttr->AtkeeHurtAdd;

	if (pstAtkChg)
	{
		pstAtkChg->nIceAtkChgMul += (pstTmpAttr->nIceAtkChgMul - pstTmpAttr->nDecIceAtkChgMul);
		pstAtkChg->iIceAtkChgAdd += (pstTmpAttr->iIceAtkChgAdd - pstTmpAttr->iDecIceAtkChgAdd);

		pstAtkChg->nFireAtkChgMul += pstTmpAttr->nFireAtkChgMul;
		pstAtkChg->iFireAtkChgAdd += pstTmpAttr->iFireAtkChgAdd;

		pstAtkChg->nThunderAtkChgMul += pstTmpAttr->nThunderAtkChgMul;
		pstAtkChg->iThunderAtkChgAdd += pstTmpAttr->iThunderAtkChgAdd;

		pstAtkChg->nLightAtkChgMul += pstTmpAttr->nLightAtkChgMul;
		pstAtkChg->iLightAtkChgAdd += pstTmpAttr->iLightAtkChgAdd;

		pstAtkChg->nNightAtkChgMul += pstTmpAttr->nNightAtkChgMul;
		pstAtkChg->iNightAtkChgAdd += pstTmpAttr->iNightAtkChgAdd;

		pstAtkChg->nPhsicAtkChgMul += (pstTmpAttr->nPhsicAtkChgMul - pstTmpAttr->nDecPhsicAtkChgMul);
		pstAtkChg->iPhsicAtkChgAdd += (pstTmpAttr->iPhsicAtkChgAdd - pstTmpAttr->iDecPhsicAtkChgAdd);

		pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul += (pstTmpAttr->AtkerIceHurtMul - pstTmpAttr->DecAtkerIceHurtMul);
		pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd += (pstTmpAttr->AtkerIceHurtAdd - pstTmpAttr->DecAtkerIceHurtAdd);

		pstAtkChg->stAtkerHurtChg.AtkerFireHurtMul += pstTmpAttr->AtkerFireHurtMul;
		pstAtkChg->stAtkerHurtChg.AtkerFireHurtAdd += pstTmpAttr->AtkerFireHurtAdd;

		pstAtkChg->stAtkerHurtChg.AtkerThunderHurtMul += pstTmpAttr->AtkerThunderHurtMul;
		pstAtkChg->stAtkerHurtChg.AtkerThunderHurtAdd += pstTmpAttr->AtkerThunderHurtAdd;

		pstAtkChg->stAtkerHurtChg.AtkerLightHurtMul += pstTmpAttr->AtkerLightHurtMul;
		pstAtkChg->stAtkerHurtChg.AtkerLightHurtAdd += pstTmpAttr->AtkerLightHurtAdd;

		pstAtkChg->stAtkerHurtChg.AtkerNightHurtMul += pstTmpAttr->AtkerNightHurtMul;
		pstAtkChg->stAtkerHurtChg.AtkerNightHurtAdd += pstTmpAttr->AtkerNightHurtAdd;

		pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul += (pstTmpAttr->AtkerPhsicHurtMul - pstTmpAttr->DecAtkerPhsicHurtMul);
		pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd += (pstTmpAttr->AtkerPhsicHurtAdd - pstTmpAttr->DecAtkerPhsicHurtAdd);

		pstAtkChg->stAtkerHurtChg.AtkerMagicHurtMul += (pstTmpAttr->AtkerMagicHurtMul - pstTmpAttr->DecAtkerMagicHurtMul);
		pstAtkChg->stAtkerHurtChg.AtkerMagicHurtAdd += (pstTmpAttr->AtkerMagicHurtAdd - pstTmpAttr->DecAtkerMagicHurtAdd);

		pstAtkChg->stAtkerHurtChg.AtkerHurtMul += (pstTmpAttr->AtkerHurtMul - pstTmpAttr->DecAtkerHurtMul);
		pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += (pstTmpAttr->AtkerHurtAdd - pstTmpAttr->DecAtkerHurtAdd);

		pstAtkChg->nDamageMul = pstTmpAttr->nDamageMul;
		pstAtkChg->unDamageAdd = pstTmpAttr->iDamageAdd;
	}

	pstDefChg->nIceDefChgMul += (pstTmpAttr->nIceDefChgMul - pstTmpAttr->nDecIceDefChgMul);
	pstDefChg->iIceDefChgAdd += (pstTmpAttr->iIceDefChgAdd - pstTmpAttr->iDecIceDefChgAdd);

	pstDefChg->nFireDefChgMul += pstTmpAttr->nFireDefChgMul;
	pstDefChg->iFireDefChgAdd += pstTmpAttr->iFireDefChgAdd;

	pstDefChg->nThunderDefChgMul += pstTmpAttr->nThunderDefChgMul;
	pstDefChg->iThunderDefChgAdd += pstTmpAttr->iThunderDefChgAdd;

	pstDefChg->nLightDefChgMul += pstTmpAttr->nLightDefChgMul;
	pstDefChg->iLightDefChgAdd += pstTmpAttr->iLightDefChgAdd;

	pstDefChg->nNightDefChgMul += pstTmpAttr->nNightDefChgMul;
	pstDefChg->iNightDefChgAdd += pstTmpAttr->iNightDefChgAdd;

	pstDefChg->nPhsicDefChgMul += (pstTmpAttr->nPhsicDefChgMul - pstTmpAttr->nDecPhsicDefChgMul);
	pstDefChg->iPhsicDefChgAdd += (pstTmpAttr->iPhsicDefChgAdd - pstTmpAttr->iDecPhsicDefChgAdd);
	
	llFight = pstDyn->stMidAttrList.uiFight;
	if (llFight + pstTmpAttr->iFight < 0)
	{
		pstDyn->stMidAttrList.uiFight = 0;
	}
	else
	{
		pstDyn->stMidAttrList.uiFight += pstTmpAttr->iFight;
	}

	pstDyn->stHitRateChg.nHeavyHitAdd += pstTmpAttr->nHeavyHitAdd;

	pstDyn->stHitRateChg.nAtkerHitRateAdd += pstTmpAttr->nAtkerHitRateAdd;

	pstDyn->stHitRateChg.nHeavyHitVal += pstTmpAttr->nHeavyHitVal;

	pstDyn->stHitChg.nHitChgMul += pstTmpAttr->nHitChgMul;
	pstDyn->stHitChg.iHitChgAdd += pstTmpAttr->iHitChgAdd;

	pstDyn->stFleeChg.nFleeChgMul += pstTmpAttr->nFleeChgMul;
	pstDyn->stFleeChg.iFleeChgAdd += pstTmpAttr->iFleeChgAdd;
	
	return 0;
}*/

int z_cacl_pet_status_result(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn, ROLEPET *pstRolePet)
{
	int i,j;
	STATUSDEF *pstStatusDef;
	PetFightShiOnline *pstPetFightShiOnline;
	PetStatusInfo *pstStatusList;
	ZoneStatus *pstZoneStatus;

	pstPetFightShiOnline = pet_find_fight_online(pstPlayer, pstRolePet);
	if(NULL == pstPetFightShiOnline)
	{
		return -1;
	}
	pstStatusList = &pstPetFightShiOnline->stStatusInfo;

	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_petstatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}
			
			z_cacl_dyn_status_result(pstDyn, pstZoneStatus, &pstStatusDef->Result[j]);
		}
	}

	return 0;
}

/*
int z_cacl_player_status_result(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ATKCHG *pstAtkChg, Player *pstPlayer)
{
	int i,j;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	//StatusTmpAttr stStatusTmpAttr;
	ZoneStatusList *pstStatusList = &pstPlayer->stOnline.stStatusList;

	//memset(&stStatusTmpAttr, 0, sizeof(stStatusTmpAttr));
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;

		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}

			//z_cacl_status_result(pstDyn, &stStatusTmpAttr, pstAtkChg, &pstStatusDef->Result[j]);
			z_cacl_dyn_status_result(pstDyn, &pstStatusDef->Result[j]);
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;
				
		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}

			//z_cacl_status_result(pstDyn, &stStatusTmpAttr, pstAtkChg, &pstStatusDef->Result[j]);
			z_cacl_dyn_status_result(pstDyn, &pstStatusDef->Result[j]);
		}
	}

	for (i=0; i<pstStatusList->bNoLimitStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astNoLimitStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}

			//z_cacl_status_result(pstDyn, &stStatusTmpAttr, pstAtkChg, &pstStatusDef->Result[j]);
			z_cacl_dyn_status_result(pstDyn, &pstStatusDef->Result[j]);
		}
	}

	//z_cacl_status_result_real(pstDyn, &stStatusTmpAttr, pstAtkChg);

	return 0;
}
*/

int z_cacl_mon_status_result(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ATKCHG *pstAtkChg, Monster *pstMon)
{
	int i,j;
	ZoneStatus *pstZoneStatus;
	STATUSDEF *pstStatusDef;
	ZoneMonStatusList *pstStatusList = &pstMon->stStatusList;

	UNUSED(pstAtkChg);
	for (i=0; i<pstStatusList->bGoodStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astGoodStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}

			//z_cacl_status_result(pstDyn, &stStatusTmpAttr, pstAtkChg, &pstStatusDef->Result[j]);
			z_cacl_dyn_status_result(pstDyn, pstZoneStatus, &pstStatusDef->Result[j]);
		}
	}

	for (i=0; i<pstStatusList->bBadStatusNum; i++)
	{
		pstZoneStatus = pstStatusList->astBadStatus+i;

		pstStatusDef = z_find_status_zonestatus(pstEnv, pstZoneStatus);
		if (NULL == pstStatusDef)
			continue;
		
		for (j=0; j<MAX_STATUS_RESULT; j++)
		{
			if (STATUS_RESULT_NONE == pstStatusDef->Result[j].ResultID)
			{
				break;
			}

			//z_cacl_status_result(pstDyn, &stStatusTmpAttr, pstAtkChg, &pstStatusDef->Result[j]);
			z_cacl_dyn_status_result(pstDyn, pstZoneStatus, &pstStatusDef->Result[j]);
		}
	}

	//z_cacl_status_result_real(pstDyn, &stStatusTmpAttr, pstAtkChg);
	
	return 0;
}

int z_mon_skill_break(ZONESVRENV* pstEnv, Monster *pstMon)
{
	CSPKG stPkg;
	CSSKILLBRKS *pstBrkS = &stPkg.Body.SkillBrkS;

	pstMon->stMonAtkInfo.cAtkStat = 0;
	pstMon->stMonAtkInfo.unPilot = 0;
	pstMon->stMonAtkInfo.cPilotEnd = 0;
	pstMon->stMonAtkInfo.bNextSkillIdx = 0;
	pstMon->stMonAtkInfo.bHeadSkillIdx = 0;
		
	pstBrkS->ID = pstMon->iID;
	pstBrkS->BenchTimeMs = 0;
	pstBrkS->SkillSeq = 0;

	Z_CSHEAD_INIT(&stPkg.Head, SKILL_BRK_SVR);

	z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);

	return 0;
}

int z_add_hurt (AtkProcess *pstAtkProcess, CSONEHIT *pstOneHit)
{
	int iTotalHurt = 0;
	int i, iHurt;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	
	for (i=0; i<pstAddAtkInfo->nAddAtkNum; i++) //处理追加攻击效果
	{
		iHurt = pstAddAtkInfo->iMainAttrHurt*(1+ pstAddAtkInfo->aiAddAtkMul[i]/100.0)*
			     (1+pstAddAtkInfo->unAtkerMul/100.0-pstAddAtkInfo->unAtkeeMul/100.0)+
			     pstAddAtkInfo->unAtkerAdd-pstAddAtkInfo->unAtkeeAdd;

		if (0 > iHurt)
		{
			iHurt = 0;
		}
		
		if (pstOneHit->TotalHurt < MAX_HURT)
		{
			iTotalHurt += iHurt;
			
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;
			pstOneHit->TotalHurt++;
		}
	}

	if (pstAddAtkInfo->iContinueNum > 0) //处理连续攻击效果
	{
		iHurt = pstAddAtkInfo->iMainAttrHurt*
			     (1+pstAddAtkInfo->unAtkerMul/100.0-pstAddAtkInfo->unAtkeeMul/100.0)/pstAddAtkInfo->iContinueNum +
			     pstAddAtkInfo->unAtkerAdd-pstAddAtkInfo->unAtkeeAdd;

		if (0 > iHurt)
		{
			iHurt = 0;
		}

		for (i=0;i<pstAddAtkInfo->iContinueNum; i++)
		{
			if (pstOneHit->TotalHurt < MAX_HURT)
			{
				iTotalHurt += iHurt;
				
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;
				pstOneHit->TotalHurt++;
			}
		}
	}

	if (pstAtkProcess->stHpRateHurt.iHpRateHurt > 0) //比例追加伤害
	{
		iHurt = pstAddAtkInfo->iMainAttrHurt*(1+ pstAtkProcess->stHpRateHurt.iAddAtkMul/100.0)*
			     (1+pstAddAtkInfo->unAtkerMul/100.0-pstAddAtkInfo->unAtkeeMul/100.0)+
			     pstAddAtkInfo->unAtkerAdd-pstAddAtkInfo->unAtkeeAdd;

		if (0 > iHurt)
		{
			iHurt = 0;
		}

		iHurt = pstAtkProcess->stHpRateHurt.iHpRateHurt > iHurt ? iHurt: pstAtkProcess->stHpRateHurt.iHpRateHurt;

		if (pstOneHit->TotalHurt < MAX_HURT)
		{
			iTotalHurt += iHurt;
			
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;
			pstOneHit->TotalHurt++;
		}
	}

	return iTotalHurt;
}

// 被攻击者状态影响攻击者伤害,和下面函数成双成队的用哟
int z_atkee_status_result(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,AtkerInfo *pstAtkerInfo, ATKCHG *pstAtkChg, ZoneStatus *pstStatus)
{
	int i;
	DYNATTRLIST *pstDyn = &pstAtkerInfo->stDyn;
	STATUSDEF  *pstStatusDef = NULL;
	STATUSRESULT *pstStatusResult = NULL;

	UNUSED(pstEnv);

	pstStatusDef = z_find_status_zonestatus(pstEnv,pstStatus);
	if(!pstStatusDef)
	{
		return 0;
	}
	
	for (i=0; i<MAX_TARGET_STATUS; i++)
	{
		if (pstSkillDef->TargetStatus[i].StatusID == 0)
		{
			break;
		}	
		//0表示对方的状态,1表示自身的状态
		if(pstSkillDef->TargetStatus[i].SelfFlag == 1)
		{
			continue;
		}

		if (pstSkillDef->TargetStatus[i].StatusID == pstStatus->unStatusID)
		{
			pstDyn->stHitRateChg.nHeavyHitAdd += pstSkillDef->TargetStatus[i].HeavyHitRate * (1 + pstStatus->nOverlayMul/100.0);
			if (pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_ICE)
			{
				//pstAtkChg->nIceAtkChgMul +=  pstSkillDef->TargetStatus[i].HurtMul * (1 + pstStatus->nOverlayMul/100.0);
				//pstAtkChg->iIceAtkChgAdd +=  pstSkillDef->TargetStatus[i].HurtAdd * (1 + pstStatus->nOverlayMul/100.0);
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul +=  pstSkillDef->TargetStatus[i].HurtMul * (1 + pstStatus->nOverlayMul/100.0);
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd +=  pstSkillDef->TargetStatus[i].HurtAdd * (1 + pstStatus->nOverlayMul/100.0);
			}
			else if(pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)
			{
				//pstAtkChg->nPhsicAtkChgMul +=  pstSkillDef->TargetStatus[i].HurtMul * (1 + pstStatus->nOverlayMul/100.0);
				//pstAtkChg->iPhsicAtkChgAdd +=  pstSkillDef->TargetStatus[i].HurtAdd * (1 + pstStatus->nOverlayMul/100.0);
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul +=  pstSkillDef->TargetStatus[i].HurtMul * (1 + pstStatus->nOverlayMul/100.0);
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd +=  pstSkillDef->TargetStatus[i].HurtAdd * (1 + pstStatus->nOverlayMul/100.0);
			}

			if (pstSkillDef->TargetStatus[i].SkillResult.SkillResultID > 0)
			{
				pstAtkerInfo->ucAppendResult[i] = 1;
			}
		}
	}

	// 状态对指定技能的伤害修正
	for(i=0;i<MAX_STATUS_RESULT;i++)
	{
		pstStatusResult = &pstStatusDef->Result[i];
		if(!pstStatusResult->ResultID)
		{
			break;
		}
		
		if( pstStatusResult->ResultID == STATUS_RESULT_TARGETSKILL_HURT_MUL &&
			pstStatusResult->ResultVal4 == 0)
		{
			if( (pstStatusResult->ResultVal1 &&
				 pstStatusResult->ResultVal1 != pstSkillDef->SkillID) ||
				(pstStatusResult->ResultVal2 && 
				 pstStatusResult->ResultVal2 != pstSkillDef->Level) )
			{
				continue;
			}
			
			if (pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_ICE)
			{
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul +=  pstStatusResult->ResultVal3 * (1 + pstStatus->nOverlayMul/100.0);
				//pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd +=  pstStatusResult->ResultVal4 * (1 + pstStatus->nOverlayMul/100.0);
			}
			else if(pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)
			{
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul +=  pstStatusResult->ResultVal3 * (1 + pstStatus->nOverlayMul/100.0);
				//pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd +=  pstStatusResult->ResultVal4 * (1 + pstStatus->nOverlayMul/100.0);
			}
		}
	}
	
	return 0;
}
// 攻击者状态影响攻击者伤害,和上面函数成双成队的用哟
int z_atker_status_result(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef,AtkerInfo *pstAtkerInfo, ATKCHG *pstAtkChg, ZoneStatus *pstStatus,AtkProcess *pstAtkProcess)
{
	int i;
	DYNATTRLIST *pstDyn = &pstAtkerInfo->stDyn;
	STATUSDEF  *pstStatusDef = NULL;
	STATUSRESULT *pstStatusResult = NULL;
	int iStatusID = 0;
	UNUSED(pstEnv);

	iStatusID = pstStatus->unStatusID;
	
	pstStatusDef = z_find_status_zonestatus(pstEnv,pstStatus);
	if(!pstStatusDef)
	{
		return 0;
	}
	
	for (i=0; i<MAX_TARGET_STATUS; i++)
	{
		if (pstSkillDef->TargetStatus[i].StatusID == 0)
		{
			break;
		}	

		// 0表示对方的状态,1表示自身的状态
		if(pstSkillDef->TargetStatus[i].SelfFlag == 0)
		{
			continue;
		}

		if (pstSkillDef->TargetStatus[i].StatusID == iStatusID)
		{
			pstDyn->stHitRateChg.nHeavyHitAdd += pstSkillDef->TargetStatus[i].HeavyHitRate;
			if (pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_ICE)
			{
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul +=  pstSkillDef->TargetStatus[i].HurtMul;
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd +=  pstSkillDef->TargetStatus[i].HurtAdd;
			}
			else if(pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)
			{
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul +=  pstSkillDef->TargetStatus[i].HurtMul;
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd +=  pstSkillDef->TargetStatus[i].HurtAdd;
			}

			if (pstSkillDef->TargetStatus[i].SkillResult.SkillResultID > 0)
			{
				pstAtkerInfo->ucAppendResult[i] = 1;
			}
		}
	}

	// 状态对指定技能的伤害修正
	for(i=0;i<MAX_STATUS_RESULT;i++)
	{
		pstStatusResult = &pstStatusDef->Result[i];
		if(!pstStatusResult->ResultID)
		{
			break;
		}
		
		if( pstStatusResult->ResultID == STATUS_RESULT_TARGETSKILL_HURT_MUL&&
			pstStatusResult->ResultVal4 == 1)
		{
			if( (pstStatusResult->ResultVal1 &&
				 pstStatusResult->ResultVal1 != pstSkillDef->SkillID) ||
				(pstStatusResult->ResultVal2 && 
				 pstStatusResult->ResultVal2 != pstSkillDef->Level) )
			{
				continue;
			}
			if (pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_ICE)
			{
				pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul +=  pstStatusResult->ResultVal3 * (1 + pstStatus->nOverlayMul/100.0);
				//pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul +=  pstStatusResult->ResultVal3;
				//pstAtkChg->stAtkerHurtChg.AtkerIceHurtAdd +=  pstStatusResult->ResultVal4;
			}
			else if(pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)
			{	
				pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul +=  pstStatusResult->ResultVal3 * (1 + pstStatus->nOverlayMul/100.0);
				//pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul +=  pstStatusResult->ResultVal3;
				//pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtAdd +=  pstStatusResult->ResultVal4;
			}
		}

		if( pstStatusResult->ResultID == STATUS_RESULT_XI_BLOOD)
		{
			if(pstStatusResult->ResultVal1)
			{
				pstAtkProcess->cSordHpRate += pstStatusResult->ResultVal1;
			}
		}
	}
	
	return 0;
}

int z_get_fairy_dyn(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn, SKILLDEF *pstSkillDef,AtkProcess *pstAtkProcess)
{
	UNUSED(pstSkillDef);
	pstAtkProcess->iAtkChgIdx = 0;
	pstDyn->stAtkChg.unPhsicBaseArmAtk = pstPlayer->stOnline.stFairyOnline.stFairyAttr.unPhyAtk;
	pstDyn->stAtkChg.unIceBaseArmAtk = pstPlayer->stOnline.stFairyOnline.stFairyAttr.unMgcAtk;
	pstDyn->stAtkChg.nArmLevChgMul = pstPlayer->stOnline.stFairyOnline.stFairyAttr.unBrkATk;

	pstDyn->stAtkChg.iAtkLevelMul = get_player_level_mul(pstEnv, pstPlayer, LEVEL_MUL_ATK);;
	pstDyn->stDefChg.iDefLevelMul = get_player_level_mul(pstEnv, pstPlayer, LEVEL_MUL_DEF);
	pstDyn->stHitRateChg.iDefHeavyHit = get_player_level_attr(pstEnv, pstPlayer, BASE2ATTR_TYPE_DEFHEAVYHIT);
	if(pstDyn->stHitRateChg.iDefHeavyHit <= 0)
	{
		pstDyn->stHitRateChg.iDefHeavyHit = 1;
	}
	
	pstDyn->stAtkChg.unFireBaseArmAtk = pstPlayer->stOnline.stFairyOnline.stFairyAttr.unFireATk;
	pstDyn->stAtkChg.unThunderBaseArmAtk = pstPlayer->stOnline.stFairyOnline.stFairyAttr.unThunderATk;
	pstDyn->stAtkChg.unLightBaseArmAtk = pstPlayer->stOnline.stFairyOnline.stFairyAttr.unLightATk;
	pstDyn->stAtkChg.unNightBaseArmAtk = pstPlayer->stOnline.stFairyOnline.stFairyAttr.unNightATk;
	pstDyn->stAtkChg.unWaterBaseArmAtk = pstPlayer->stOnline.stFairyOnline.stFairyAttr.unWaterATk;
	
	return 0;
}

int get_continuenum_hurt(int *piTotalHurt, int iContinueNum)
{
	int iHurt = *piTotalHurt;

	if(iContinueNum <= 1)
	{
		return iHurt;
	}

	//80%- 120%
	iHurt = iHurt/100.0/iContinueNum*(RAND1(41)+80);
	*piTotalHurt -= iHurt;
	
	return iHurt;
}

int z_player_hit_mon(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, Monster *pstAtkee, AtkProcess *pstAtkProcess)
{
	int i;
	int iHP;
	long long llTotalHurt;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	int iRealHurt, iHurt, iHeavyHit;
	MONSTERDEF *pstMonsterDef;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	ATKCHG *pstAtkChg = NULL;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	ZoneSkill *pstZoneSkill = pstAtkerInfo->pstZoneSkill;
	RESOURCEADDCHG *pstResourceAddChg = &pstAtkerDyn->stResourceAddChg;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	ROLETMPSYS *pstRoleTmpSys = &pstAtker->stRoleData.MiscInfo.TmpSys;
	ITEMDEF *pstItemDefTmp = NULL;
	Player *pstSkillAtker = pstAtker;
	ZoneReflectedDamage *pstReflectedDamage = &pstAtkProcess->stReflectedDamage;
	int iReflect = 0;
	STATUSRESULT *pstStatusResult = NULL;
	SKILLRESULT *pstSkillResultAddHurt = NULL;
	ZoneAni stAtkerAni;
	int iZJret = 0; 

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		if (is_mon_machine(pstAtkee))
		{
			Player *pstPlayer1 = machine_get_driver(pstEnv, pstAtkee);
			
			if (pstPlayer1 && pstPlayer1->stRoleData.MiscInfo.DartCarInfo.MonMemID > 0 && pstAtker)
			{
				ZoneClan *pstClan1 = player_get_clan(pstEnv, pstPlayer1);
				if (!(pstClan1 && clan_relation_by_player(pstEnv, pstAtker, pstClan1)< 0))
				{
					return -1;
				}
			}
		}
	}

	//融焰Boss不可被其他玩家攻击，只能被召唤boss的玩家攻击
	if(0 > player_hit_team_call_rongyan_mon(pstEnv,pstAtker,pstAtkee))
	{
		return -1;
	}

	if (pstAtkee->bSubType >= MON_SUB_TYPE_TRAP || pstAtkee->cStat == MONSTER_DIE)
	{
		return -1;
	}

	
		
	stAtkeeInfo.stAtkee.iType = OBJ_MONSTER;
	stAtkeeInfo.stAtkee.stObj.pstMon = pstAtkee;

	pstOneHit->ID = pstAtkee->iID;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;

	pstAtkee->bTraceTry = 0;

	if (pstSkillDef->FairySkill)
	{
		z_get_fairy_dyn(pstEnv, pstAtker, pstAtkerDyn,pstSkillDef,pstAtkProcess);
	}
	else
	{
		memcpy(pstAtkerDyn, &pstAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
		if(pstAtker->stOnline.iSkillAtkerID > 0)
		{
			Player *pstRealAtker = z_id_player(pstAppCtx, pstEnv, pstAtker->stOnline.iSkillAtkerID);
			if(pstRealAtker)
			{
				memcpy(pstAtkerDyn, &pstRealAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
			}
		}
		
		if (0 > pstAtkProcess->iAtkChgIdx)
		{
			pstAtkProcess->iAtkChgIdx = 0;
			memset(&pstAtkerDyn->stAtkChg, 0, sizeof(pstAtkerDyn->stAtkChg));

			pstAtkerDyn->stAtkChg.iAtkLevelMul = get_player_level_mul(pstEnv, pstAtker, LEVEL_MUL_ATK);
		}
	}

	if (pstAtkProcess->iAtkChgIdx >= 0)
	{
		pstAtkChg = &pstAtkerDyn->stAtkChg;
	}
	if( NULL == pstAtkChg)
	{
		return -1;
	}
		
	z_player_normal_atk_cacl(pstEnv, pstAtkChg,pstSkillDef ,pstAtker, pstAtkProcess);
	
	memset (pstAtkerInfo->ucAppendResult, 0 , sizeof(pstAtkerInfo->ucAppendResult));
	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;

	//检查对方坏的状态对技能的修正
	for (i=0; i<pstAtkee->stStatusList.bBadStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstSkillDef, pstAtkerInfo, pstAtkChg, &pstAtkee->stStatusList.astBadStatus[i]);
	}
	//检查对方坏的状态对技能的修正
	for (i=0; i<pstAtkee->stStatusList.bGoodStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstSkillDef, pstAtkerInfo, pstAtkChg, &pstAtkee->stStatusList.astGoodStatus[i]);
	}
       //检查自己好的状态对技能的修正
	for (i=0; i<pstAtker->stOnline.stStatusList.bGoodStatusNum; i++)
	{
		z_atker_status_result(pstEnv, pstSkillDef, pstAtkerInfo, pstAtkChg, &pstAtker->stOnline.stStatusList.astGoodStatus[i],pstAtkProcess);
	}
	

	z_cacl_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	

	if (pstRoleTmpSys->TmpItemID > 0)
	{
		pstItemDefTmp = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	}

	if (0 == (pstItemDefTmp && pstItemDefTmp->ItemTmp.Type >= ITEM_TMP_ITEM))
	{
		//z_cacl_player_status_result(pstEnv, pstAtkerDyn, pstAtkChg, pstAtker);

		// 计算组队契约对攻防的影响
		z_cacl_player_qiyue_result(pstEnv, pstAtkerDyn, pstAtkChg, pstAtker);
	}
	
	if (pstSkillDef->StatusChg.StatusID > 0 && z_player_find_status(pstAtker, pstSkillDef->StatusChg.StatusID))
	{
		int iStatusChgHurt = 0;
		int iStatusChgHurtMul = 0;
		ZoneStatus *pstZoneStatus = NULL;

		iStatusChgHurt = pstSkillDef->StatusChg.Hurt;
		iStatusChgHurtMul = pstSkillDef->StatusChg.HurtMul;
		
		pstZoneStatus = z_player_find_status(pstAtker, pstSkillDef->StatusChg.StatusID);
		if(pstZoneStatus)
		{
			z_status_mul_skill_hurt(pstEnv,pstZoneStatus ,&iStatusChgHurt,&iStatusChgHurtMul);
		}

		pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += iStatusChgHurt;
		pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iStatusChgHurtMul;

		pstAtkerDyn->stHitRateChg.nHeavyHitAdd += pstSkillDef->StatusChg.Heavy;
	}
	
	pstMonsterDef = z_get_mon_def(pstEnv, pstAtkee);
	if (NULL == pstMonsterDef)
	{
		return -1;
	}

	if(0 > z_span_map_if_player_hit_mon(pstEnv,pstAtker, pstAtkee,pstMonsterDef))
	{
		return -1;
	}

	//记录攻打过终极熔岩巨兽
	iZJret	= zj_rongyan_mon_if_have_by_wid(pstEnv,pstAtkee->WID);
	if(iZJret > 0)
	{
		pstAtker->stRoleData.MiscInfo.ZJRongYanMisc.WID = pstAtkee->WID;
	}

	if(pstMonsterDef->MiscType & MON_MISC_TYPE_TOUCH_LIMIT)
	{
		//记录打过这种怪
		player_baoxiang_touch_limit_set(pstEnv,pstAtker,pstAtkee->WID);
	}

	if (0 == pstSkillDef->NotBootyFirstAtk)
	{
		mon_booty_first_atker(pstAppCtx, pstEnv, pstAtkee, pstAtker);
	}

	//攻击计算副本次数了
	map_pworld_chg_limitflag(pstAppCtx, pstEnv, pstAtker);
		
	z_get_mon_dyn(pstEnv, pstAtkee, pstMonsterDef, pstAtkeeDyn);
	z_cacl_mon_status_result(pstEnv, pstAtkeeDyn, NULL, pstAtkee);

	pstHits->TotalHit++;

	// 任务处理 提高了大部分情况下的性能
	if(pstAtker->stOnline.cMiscFlag & MISC_FLAG_HIT_TRACE)
	{
		task_hit_monster_handler(pstEnv, pstAtker, pstZoneSkill, pstAtkee);
	}

	if(pstAtker->stOnline.iSkillAtkerID)
	{
		Player *pstTmpPlayer;
		
		pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstAtker->stOnline.iSkillAtkerID);
		if(pstTmpPlayer && pstTmpPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			pstSkillAtker = pstTmpPlayer;
		}
	}

	stAtkerAni.iType = OBJ_PLAYER;
	stAtkerAni.stObj.pstPlayer = pstSkillAtker;
	atker_set_pktime(pstEnv, &stAtkerAni, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);

	if(pstSkillDef->NoHurtSkill == 0)
	{
		//符文额外附加技能效果
		rune_addattr_skill(pstEnv, pstAtkProcess, &stAtkeeInfo);
	}

	player_hit_jiqi_full_add_state_target( pstEnv, pstAtkProcess, 
						&stAtkeeInfo, pstSkillDef, pstAtkerInfo->cJiqiNum);
	
	if (pstSkillDef->NoHurtSkill) //非伤害技能，只需要处理技能效果
	{
		if (!is_friend_mon(pstAtkee) && z_distance(&pstSkillAtker->stRoleData.Pos, &pstAtkee->stCurrPos) <= (int)pstMonsterDef->TraceDist)
		{
			ZoneAni stAni;

			stAni.iType = OBJ_PLAYER;
			stAni.stObj.pstPlayer = pstSkillAtker;

			if(pstAtkee->bAtkType != ATK_TYPE_NO && !is_mon_bub(pstAtkee))
			{
				if (0 > mon_press_find(pstAtkee, pstSkillAtker->iMemID) )
				{
					mon_press_insert_player(pstEnv, pstAtkee, pstMonsterDef, pstSkillAtker);
					mon_ai_addpress(pstEnv, pstAtkee, &stAni, pstSkillDef->NoHurtPress);
				}
				else
				{
					mon_ai_addpress(pstEnv, pstAtkee, &stAni, pstSkillDef->NoHurtPress);
				}
			}
		}

		if ((pstSkillDef->TargetType & SKILL_TARGET_ENEMY_PLAYER) && z_is_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
			pstOneHit->TotalHurt++;
			return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
		}

		z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}
		
	llTotalHurt = 0;
	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;

	//特殊处理超远距离攻击，阻止怪物马上回血
	if (pstSkillDef->Distance >= 10000 && pstAtkee->stAtkTargets.PressNum == 0)
	{
		pstAtkee->stMonAtkInfo.stLastAtk.tv_sec = pstEnv->pstAppCtx->stCurr.tv_sec + 15;
	}
	
	{
		char cOldFlag = pstAtkee->cMonMiscFlag;
		
		pstAtkee->cMonMiscFlag |= MON_MISC_FLAG_RESTORE_HP;

		//之前没战斗,现在战斗了
		if (!(cOldFlag & MON_MISC_FLAG_RESTORE_HP))
		{
			//发表战斗感言
			z_mon_speech_begin_or_end_fight(pstEnv,pstAtkee,SPEECH_MON_BEGIN_FIGHT);
		}
	}
	
	if (0 > mon_press_find(pstAtkee, pstSkillAtker->iMemID) && pstSkillDef->PressRate > -100  &&
			z_distance(&pstSkillAtker->stRoleData.Pos, &pstAtkee->stCurrPos) <= (int)pstMonsterDef->TraceDist)
	{
		//佣兵被人打不还手
		if(!is_mon_bub(pstAtkee))
		{	
			mon_press_insert_player(pstEnv, pstAtkee, pstMonsterDef, pstSkillAtker);
		}
	}

	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		z_do_skill_result_by_flee(pstAppCtx, pstEnv,pstSkillDef,pstAtker,NULL);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//受攻方是否无敌状态
	if (z_is_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	// 受功放是否绝对闪避
	if (z_is_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_UTTERLY_FLEE))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		z_do_skill_result_by_flee(pstAppCtx, pstEnv,pstSkillDef,pstAtker,NULL);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	if(pstAtkee->cMonMiscFlag & MON_MISC_FLAG_STATUS_TRACE)
	{
		mon_status_beatk(pstEnv->pstAppCtx, pstEnv, pstAtkee);
	}

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += pstZoneSkill->stSkillMul.HeavyHitRateMul;//天赋的暴击修正
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		//暴击力转化暴击率:暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0 /(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}

		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;

			//爆击修正攻击时资源增加
			pstResourceAddChg->unCollectNumAtkMul += (50 + pstAtkerDyn->stHitRateChg.nHeavyHitVal/100.0);
		}
	}

	
	//追招的暂时没用
	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//天赋 普通攻击触发对自身的buff
	//genius_atk_random_add_status(pstAppCtx, pstEnv, pstAtker, pstSkillDef->SkillType);
	z_del_result_by_heavyhit(pstEnv->pstAppCtx, pstEnv,pstAtker,NULL,iHeavyHit);

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;
	z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	z_do_extended_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo,
		                   pstAtkChg);

	pstStatusResult = mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_REFLECTION);
	if(pstStatusResult &&
		pstReflectedDamage->iNum < (int)(sizeof(pstReflectedDamage->astEntries)/sizeof(pstReflectedDamage->astEntries[0])))
	{
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stFrom.iType = OBJ_MONSTER;
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stFrom.stObj.pstMon = pstAtkee;
		/*
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stHit = *pstOneHit;
		*/
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stHit.TotalHurt = 0;
		++pstReflectedDamage->iNum;

		iReflect = 1;
	}

	pstSkillResultAddHurt = z_get_skill_result(pstSkillDef,RESULT_ADDHURT_BY_TARGETSTATUS);
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		pstAtkProcess->iContinueCount--;

		if(iHurt <= 0)
		{
			continue;
		}

		if (iReflect)
		{
			z_hurt_reflect(pstEnv, &iHurt, pstStatusResult, pstReflectedDamage);
		}
			
		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstAtkee->iHP;
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}
		else
		{
			// 怪物伤害吸收计算
			if (0 == (pstAtkerInfo->iFlag & ATKER_INFO_FLAG_THROUGH_ABSORB))
			{
				z_mon_hurt_absorb(pstEnv, pstAtkee, pstOneHit, &iHurt);
				z_mon_hurt_status_absorb(pstEnv,pstAtkee, pstOneHit,  &iHurt);
			}
		}

		if(iHurt <= 0)
		{
			continue;
		}

		llTotalHurt += iHurt;

		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;
		pstOneHit->TotalHurt++;

		if( pstSkillResultAddHurt &&
			RAND1(100) <= pstSkillResultAddHurt->SkillResultVal1)
		{
			ZoneStatus *pstZoneStatusTmp = NULL;
			if(pstSkillResultAddHurt->SkillResultVal3 ==0)
			{
				pstZoneStatusTmp = z_mon_find_status(pstAtkee, pstSkillResultAddHurt->SkillResultVal2);
			}
			else 
			{
				pstZoneStatusTmp = z_mon_find_status_by_level(pstAtkee, pstSkillResultAddHurt->SkillResultVal2,pstSkillResultAddHurt->SkillResultVal3);
			} 
			if(pstZoneStatusTmp)
			{
				llTotalHurt += iHurt*pstSkillResultAddHurt->SkillResultVal4/100.0;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt*pstSkillResultAddHurt->SkillResultVal4/100.0;
				pstOneHit->TotalHurt++;
			}
		}
		
		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		
		if (llTotalHurt >= (iHP + pstAtkee->iHP))
		{
			pstAtkProcess->cDeadHit = 1; //借用一下一击必杀，表示已经死了
			z_add_dead_resource(pstEnv, pstAtkProcess, pstMonsterDef, pstResourceAddChg);
			break;
		}


		z_add_atk_resource(pstEnv, pstAtkProcess, pstMonsterDef, pstResourceAddChg);
	}while (pstAtkProcess->iContinueCount > 0);

	if (!pstAtkProcess->cDeadHit && pstAddAtkInfo->iMainAttrHurt != 0) //处理追招等附加伤害
	{
		iHurt = z_add_hurt(pstAtkProcess, pstOneHit);
		if (iHurt > 0)
		{
			llTotalHurt += iHurt;

			iHP = z_get_hithp(pstOneHit);
			if (llTotalHurt >= (iHP + pstAtkee->iHP))
			{
				z_add_dead_resource(pstEnv, pstAtkProcess, pstMonsterDef, pstResourceAddChg);
			}
		}
	}

	//特殊处理超远距离攻击，阻止怪物马上回血
	/*if (pstSkillDef->Distance >= 10000)
	{
		pstAtkee->stMonAtkInfo.stLastAtk.tv_sec = pstEnv->pstAppCtx->stCurr.tv_sec + 15;
	}*/

	pstAtkee->iAtkerID = pstSkillAtker->iMemID;
	pstAtkee->stAtkerPos = pstSkillAtker->stRoleData.Pos;
	pstAtkee->cAtkerInfoNew = 1;

	//mon_fight_speech(pstEnv, pstAtkee, SPEECH_MON_BEHIT);

	//记录打过的BOSS
	player_boss_info_insert(pstAtker, pstAtkee);

	if (pstAtkee->cMonScriptFlag & MON_SCRIPT_PROC_ATK_BY_PLAYER_MAP)
	{
		char szName[128];				

		stSEnv.pstMonster = pstAtkee;
		stSEnv.pstNpc = NULL;
		stSEnv.pstPworldInst = NULL;
		stSEnv.pstPlayer = pstAtker;
		stSEnv.pstKiller = NULL;
		stSEnv.pstMapinst = NULL;
		
		snprintf(szName, sizeof(szName), "monster%d.mac", pstAtkee->iDefIdx);
		szName[sizeof(szName) - 1] = 0;
		
		z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName, &pstAtkee->stScriptIdx, MON_SCRIPT_PROC_ATK_BY_PLAYER);
	}
	
	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0); 
}

int z_cacl_tmpitem_attr(ZONESVRENV* pstEnv, ROLETMPSYS *pstRoleTmpSys, DYNATTRLIST *pstAtkeeDyn)
{
	ITEMDEF *pstItemDef;

	pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
	if (pstItemDef && (pstItemDef->ItemTmp.HurtDefMul != 0 || 
						pstItemDef->ItemTmp.HurtDefAdd != 0 || 
						pstItemDef->ItemTmp.FleeAdd       != 0))
	{
		pstAtkeeDyn->stAtkeeHurtChg.AtkeeHurtMul += pstItemDef->ItemTmp.HurtDefMul;
		pstAtkeeDyn->stAtkeeHurtChg.AtkeeHurtAdd += pstItemDef->ItemTmp.HurtDefAdd;
		pstAtkeeDyn->stFleeChg.iFleeChgAdd += pstItemDef->ItemTmp.FleeAdd;
	}
		
	return 0;
}

int z_find_tmpenemy(Player *pstPlayer,tdr_ulonglong RoleID)
{
	int i;

	for (i=0; i<pstPlayer->stOnline.iTmpEnemyNum; i++)
	{
		if (pstPlayer->stOnline.ullTmpEnemy[i] == RoleID)
		{
			return i;
		}
	}
	return -1;
}

void  z_insert_tmpenemy(Player *pstAtker, Player *pstAtkee)
{
	if (pstAtkee->stOnline.iTmpEnemyNum >= MAX_SKILL_TARGET)
	{
		memmove(&pstAtkee->stOnline.ullTmpEnemy[0],&pstAtkee->stOnline.ullTmpEnemy[1],
			(pstAtkee->stOnline.iTmpEnemyNum -1)*sizeof(pstAtkee->stOnline.ullTmpEnemy[0]));
		pstAtkee->stOnline.ullTmpEnemy[pstAtkee->stOnline.iTmpEnemyNum - 1] = pstAtker->stRoleData.RoleID;
	}
	else
	{
		pstAtkee->stOnline.ullTmpEnemy[pstAtkee->stOnline.iTmpEnemyNum ++] = pstAtker->stRoleData.RoleID;
	}
}

int z_get_pet_dyn(ZONESVRENV* pstEnv, Player *pstPlayer, DYNATTRLIST *pstDyn, tdr_ulonglong ullPetWID)
{
	ROLEPET *pstRolePet;
	RolePetOnline *pstRolePetOnline;
	PetFightAttrOnline *pstPetFightAttr;
	
	memset(pstDyn, 0, sizeof(*pstDyn));
	pstRolePet = z_find_role_pet(pstPlayer, ullPetWID, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	pstRolePetOnline = pet_find_online(pstPlayer, pstRolePet);
	if (NULL == pstRolePetOnline)
	{
		return -1;
	}

	pstPetFightAttr = pet_find_fightattronline_byonline(pstPlayer, pstRolePet, pstRolePetOnline);
	if (NULL == pstPetFightAttr)
	{
		return -1;
	}
	
	pstDyn->stAtkChg = pstPetFightAttr->stAtkChg;
	pstDyn->stAtkeeHurtChg = pstPetFightAttr->stAtkeeChg; 
	pstDyn->stHitChg = pstPetFightAttr->stHitChg;
	pstDyn->stFleeChg = pstPetFightAttr->stFleeChg;
	pstDyn->stHitRateChg = pstPetFightAttr->stHitRateChg;
	pstDyn->stDefChg = pstPetFightAttr->stDefChg;

	if(pstDyn->stHitRateChg.iDefHeavyHit <= 0)
	{
		pstDyn->stHitRateChg.iDefHeavyHit = 1;
	}
		
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

static int z_player_hurt_absorb(ZONESVRENV* pstEnv, Player *pstAtkee,
                                CSONEHIT *pstOneHit, int *piHurt)
{
	int iHurt = *piHurt;
	ZoneStatus *pstZoneStatus = NULL;

	// 伤害转MP效果
	if(pstAtkee->stRoleData.RoleDetail.MPCurr > 0 &&
	   z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DAMAGE2MP) &&
	   pstOneHit->TotalHurt < MAX_HURT)
	{
		int iAddMP ;
		STATUSRESULT stResult;
		z_get_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DAMAGE2MP,
		                           &stResult, 0);
		// 先算转换百分比
		iAddMP = (iHurt * stResult.ResultVal1) / 100;
		iHurt -= iAddMP;
		if(iHurt < 0)
		{
			iHurt = 0;
		}
		// 再算转换率
		iAddMP = (iAddMP * stResult.ResultVal2) / 100;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MP;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = -iAddMP;
		pstOneHit->TotalHurt++;
	}

	// 伤害转HP效果
	pstZoneStatus = z_is_player_status_result(pstEnv, pstAtkee,
	                                          STATUS_RESULT_DAMAGE2HP);
	if(pstZoneStatus)
	{
		STATUSRESULT stResult;
		z_get_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DAMAGE2HP,
		                           &stResult, 0);
		if(!stResult.ResultVal1)
		{
			if (pstOneHit->TotalHurt < MAX_HURT)
			{
				// 立即生效
				int iAddHP = (iHurt * stResult.ResultVal2) / 100;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iAddHP;
				pstOneHit->TotalHurt++;

				iHurt -= iAddHP; // 减免伤害了
				if(iHurt < 0)
				{
					iHurt = 0;
				}
			}
		}
		else
		{
			// 延迟生效
			int iAddHP = (iHurt * stResult.ResultVal2) / 100;
			pstZoneStatus->iVal += iAddHP; // 暂时寄存，日后结算
		}
	}

	*piHurt = iHurt;
	return 0;
}

static int z_mon_hurt_absorb(ZONESVRENV* pstEnv, Monster *pstAtkee,
                             CSONEHIT *pstOneHit, int *piHurt)
{
	int iHurt = *piHurt;
	ZoneStatus *pstZoneStatus = NULL;

	// 伤害转HP效果
	pstZoneStatus = z_is_mon_status_result(pstEnv, pstAtkee,
	                                       STATUS_RESULT_DAMAGE2HP);
	if(pstZoneStatus)
	{
		STATUSRESULT stResult;
		z_get_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_DAMAGE2HP,
		                        &stResult);
		if(!stResult.ResultVal1)
		{
			if (pstOneHit->TotalHurt < MAX_HURT)
			{
				// 立即生效
				int iAddHP = (iHurt * stResult.ResultVal2) / 100;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iAddHP;
				pstOneHit->TotalHurt++;

				iHurt -= iAddHP; // 减免伤害了
				if(iHurt < 0)
				{
					iHurt = 0;
				}
			}
		}
		else
		{
			// 延迟生效
			int iAddHP = (iHurt * stResult.ResultVal2) / 100;
			pstZoneStatus->iVal += iAddHP; // 暂时寄存，日后再说
		}
	}

	*piHurt = iHurt;
	return 0;
}

static int z_hurt_reflect(ZONESVRENV* pstEnv, int *piHurt,
                              STATUSRESULT *pstStatusResult, ZoneReflectedDamage *pstReflectedDamage)
{
	int iHurt = *piHurt;
	
	if(pstStatusResult)
	{
		CSONEHIT *pstHit = &pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum - 1].stHit;

		if(pstStatusResult->ResultVal1 * 100 < RAND1(10000))
		{
			return -1;
		}

		if (pstHit->TotalHurt < MAX_HURT)
		{
			pstHit->Hurts[pstHit->TotalHurt].HurtType = HURT_TYPE_DMG;
			pstHit->Hurts[pstHit->TotalHurt].Value =
				(iHurt * pstStatusResult->ResultVal2) / 100;
			++pstHit->TotalHurt;
		}
	}
	*piHurt = iHurt;
	return 0;
	UNUSED(pstEnv);
}

int z_be_reflected_ballistic_atk(ZONESVRENV *pstEnv, Player *pstAtker, AtkProcess *pstAtkProc, 
	CSACTIONRES *pstActionRes, ZoneAni *pstFrom)
{
	CSPKG stPkg;
	CSACTIONRES *pstRes = &stPkg.Body.ActionRes;

	if(pstAtkProc->stAtkerInfo.pstSkillDef->AtkType == SKILL_ATK_TYPE_NORMAL)
	{
		return 0;
	}
	
	pstRes->ActionID = ACTION_BALLISTIC_ATK;
	pstRes->ID = pstActionRes->ID;
	pstRes->Data.BallisticAtk.SkillID = pstAtkProc->stAtkerInfo.pstSkillDef->SkillID;
	pstRes->Data.BallisticAtk.Level = pstAtkProc->stAtkerInfo.pstSkillDef->Level;
	pstRes->Data.BallisticAtk.RuneTransferedID = pstAtkProc->stAtkerInfo.pstSkillDef->SkillID;
	pstRes->Data.BallisticAtk.SkillSeq = 0;
	pstRes->Data.BallisticAtk.AtkPos = pstActionRes->Data.Hit.AtkPos;
	pstRes->Data.BallisticAtk.TargetNum = 1;
	pstRes->Data.BallisticAtk.TargetIDs[0] = pstAtker->iMemID;
	pstRes->Data.BallisticAtk.SelfPosChgFlag = 0;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	switch(pstFrom->iType)
	{
		case OBJ_PLAYER:
			z_send_player_hit_res(pstEnv->pstAppCtx, pstEnv, pstFrom->stObj.pstPlayer,
	                              &pstActionRes->Data.Hit.AtkPos,
	                              &stPkg);
			break;

		case OBJ_MONSTER:
			z_send_mon_hit_res(pstEnv->pstAppCtx, pstEnv, pstFrom->stObj.pstMon,
			                   &stPkg);
			break;

		default:
			return -1;
	}

	return 0;
}

int z_player_be_reflected(ZONESVRENV *pstEnv, Player *pstAtker, 
                                 AtkProcess *pstAtkProc)
{
	ZoneReflectedDamage *pstReflectedDamage = &pstAtkProc->stReflectedDamage;
	int i = pstReflectedDamage->iNum;
	Player *pstFromPlayer = NULL;
	Monster *pstFromMon = NULL;
	CSACTIONRES *pstActionRes = &pstEnv->stCSRes.Body.ActionRes;
	PlayerChagAttrVal stValChg;
	int j = 0;
	int iOldHP = 0;
	int iOldMP = 0;
	int iIndex = 0;
	char cType = 0;
	Player *pstPlayer = NULL;
	Player *pstPlayerTmp = NULL;
	ZoneStatus *pstZoneStatus = NULL;
	STATUSRESULT stResult;
	TMEMBLOCK *pstMemBlock = NULL;
	RESPOS stFromAtkPos;
	Player *pstOwnerAtker;

	//切磋结束的这一下啊就不反击了
	if(pstAtkProc->cQieCuoEnd)
	{
		return 0;
	}

	memset(&stFromAtkPos,0,sizeof(stFromAtkPos));		
	memset(&stResult,0,sizeof(stResult));
	memset(&stValChg,0,sizeof(stValChg));
	memset(pstActionRes,0,sizeof(CSACTIONRES));

	while(!pstAtkProc->stDeadList.iSelf && i--)
	{
		//主人
		pstOwnerAtker = get_atker_player(pstEnv, &pstReflectedDamage->astEntries[i].stFrom);
		
		pstPlayer = pstAtker;
		
		pstActionRes->Data.Hit.AtkPos = pstPlayer->stRoleData.Pos;
		switch(pstReflectedDamage->astEntries[i].stFrom.iType)
		{
		case OBJ_PLAYER:
			pstFromPlayer = 
				pstReflectedDamage->astEntries[i].stFrom.stObj.pstPlayer;
			pstActionRes->ID = pstFromPlayer->iMemID;
			stFromAtkPos = pstFromPlayer->stRoleData.Pos;
			break;

		case OBJ_MONSTER:
			pstFromMon = pstReflectedDamage->astEntries[i].stFrom.stObj.pstMon;
			pstActionRes->ID = pstFromMon->iID;
			stFromAtkPos = pstFromMon->stCurrPos;
				
			z_get_mon_status_result(pstEnv, pstFromMon, STATUS_RESULT_REFLECTION,
		                        &stResult);
			if( stResult.ResultID == STATUS_RESULT_REFLECTION &&
				stResult.ResultVal3 == 1 )
			{
				pstZoneStatus = z_is_mon_status_result(pstEnv, pstFromMon,
				                                       STATUS_RESULT_REFLECTION);
				if(pstZoneStatus)
				{
					//指定玩家反射
					iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstZoneStatus->iApplyID, &cType);
					if (iIndex >= 0 )
					{
						if(cType == OBJ_PLAYER)
						{
							pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
							if (pstMemBlock->fValid)
							{
								pstPlayerTmp	=	(Player *)pstMemBlock->szData;
								if (pstPlayerTmp->eStatus == PLAYER_STATUS_ROLE_LOGIN)
								{
									pstPlayer = pstPlayerTmp;
									break;
								}
							}
						}
					}
				}
			}
			pstPlayer = pstAtker;
			break;

		default:
			return -1;
		}
		if(!pstReflectedDamage->astEntries[i].stHit.TotalHurt)
		{
			continue;
		}

		if(z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_INVIN))
		{
			continue;
		}

		if ( z_distance(&stFromAtkPos, &pstPlayer->stRoleData.Pos)  > REFLECT_MAX_DISTANCE )
		{
			continue;
		}

		iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr,
		iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
			
		// 生命值减少
		stValChg.AttrNum = 1;
		stValChg.AttrList[0].AttrID = ATTR_ID_HP;
		stValChg.AttrList[0].nFlag = 1;
		stValChg.AttrList[0].Value.HP = 0;
		
		for(j=0; j<pstReflectedDamage->astEntries[i].stHit.TotalHurt; j++)
		{
			int iHit_absorb = 0;
			int *piOneHurtHp = 0;
			CSONEHIT stHit_absorb;
			//切磋防止打死
			if(pstOwnerAtker && z_is_qiecuo(pstEnv, pstPlayer, pstOwnerAtker))
			{
				int *piOneHurt = &pstReflectedDamage->astEntries[i].stHit.Hurts[j].Value;

				if(stValChg.AttrList[0].Value.HP + (*piOneHurt) >=  pstPlayer->stRoleData.RoleDetail.HPCurr)
				{
					//调整伤害次数、数值
					*piOneHurt = pstPlayer->stRoleData.RoleDetail.HPCurr - stValChg.AttrList[0].Value.HP - 1;
					pstReflectedDamage->astEntries[i].stHit.TotalHurt = j+1;

					//切磋结束，留一点血
					stValChg.AttrList[0].Value.HP = pstPlayer->stRoleData.RoleDetail.HPCurr -1;

					//自己输了特殊处理
					pstAtkProc->cQieCuoEnd = 2;

					break;
				}
			}

			//计算反击伤害吸收
			piOneHurtHp = &pstReflectedDamage->astEntries[i].stHit.Hurts[j].Value;
			memset(&stHit_absorb,0,sizeof(stHit_absorb));
			iHit_absorb = z_player_hurt_status_absorb(pstEnv,pstPlayer, &stHit_absorb, 
						piOneHurtHp);
			if( (iHit_absorb>=0) && (*piOneHurtHp == 0) )
			{
				pstReflectedDamage->astEntries[i].stHit.Hurts[j].HurtType = HURT_TYPE_SBSORB;
			}
			
			stValChg.AttrList[0].Value.HP += 
				pstReflectedDamage->astEntries[i].stHit.Hurts[j].Value;
		}
		
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stValChg, 0);
		//player_hp_action(pstEnv, pstPlayer, iOldHP, iOldMP, pstActionRes->ID);

		if(0 >= pstPlayer->stRoleData.RoleDetail.HPCurr)
		{
			pstAtkProc->stDeadList.iSelf = 1;
			pstAtkProc->stDeadList.stSelfDead.stAtker = 
				pstReflectedDamage->astEntries[i].stFrom;

			//player_die(pstEnv->pstAppCtx, pstEnv, pstPlayer,
			//           &pstAtkProc->stDeadList.stSelfDead.stAtker, 1);
			//break;
		}

		//特殊技能需要先有个弹道飞行的协议
		//z_be_reflected_ballistic_atk(pstEnv, pstAtker, pstAtkProc, 
		//	pstActionRes, &pstReflectedDamage->astEntries[i].stFrom);

		// 将敌人技能回放出来
		pstActionRes->ActionID = ACTION_HIT;
		pstActionRes->Data.Hit.Flags = HIT_FLAG_BE_REFLECTED;
		pstActionRes->Data.Hit.DirFace = 0;
		pstActionRes->Data.Hit.SkillID = 
			pstAtkProc->stAtkerInfo.pstSkillDef->SkillID;
		pstActionRes->Data.Hit.Level =
			pstAtkProc->stAtkerInfo.pstSkillDef->Level;
		
		pstActionRes->Data.Hit.SkillSeq = 0;
		pstActionRes->Data.Hit.SelfPosChgFlag = 0;
		//pstActionRes->Data.Hit.FittingsEffectID = 0;
		pstActionRes->Data.Hit.RuneTransferedID =
			pstActionRes->Data.Hit.SkillID;
		pstActionRes->Data.Hit.TotalHit = 1;
 		pstActionRes->Data.Hit.Hits[0] =
 			pstReflectedDamage->astEntries[i].stHit;
		pstActionRes->Data.Hit.Hits[0].ID = pstPlayer->iMemID;
		pstActionRes->Data.Hit.Hits[0].HP = 
			pstPlayer->stRoleData.RoleDetail.HPCurr;
		if(pstAtkProc->stDeadList.iSelf)
		{
			pstActionRes->Data.Hit.Hits[0].DeadFlag = 1;
		}
		
		Z_CSHEAD_INIT(&pstEnv->stCSRes.Head, ACTION_RES);
		switch(pstReflectedDamage->astEntries[i].stFrom.iType)
		{
		case OBJ_PLAYER:
			if(pstFromPlayer)
			{
				z_send_player_hit_res(pstEnv->pstAppCtx, pstEnv, pstFromPlayer,
                                  &pstActionRes->Data.Hit.AtkPos,
                                  &pstEnv->stCSRes);
			}
			break;

		case OBJ_MONSTER:
			z_send_mon_hit_res(pstEnv->pstAppCtx, pstEnv, pstFromMon,
			                   &pstEnv->stCSRes);
			break;

		default:
			return -1;
		}
	}

	return 0;
}

int z_mon_be_reflect(ZONESVRENV *pstEnv, Monster *pstAtker, 
                                 AtkProcess *pstAtkProc)
{
	ZoneReflectedDamage *pstReflectedDamage = &pstAtkProc->stReflectedDamage;
	int i = pstReflectedDamage->iNum;
	Player *pstFromPlayer = NULL;
	Monster *pstFromMon = NULL;
	CSACTIONRES *pstActionRes = &pstEnv->stCSRes.Body.ActionRes;
	int j = 0;
	int iHP = 0;
	STATUSRESULT stResult;
	RESPOS stFromAtkPos;

	memset(&stFromAtkPos,0,sizeof(stFromAtkPos));		
	memset(&stResult,0,sizeof(stResult));
	memset(pstActionRes,0,sizeof(CSACTIONRES));

	while(!pstAtkProc->stDeadList.iSelf && i--)
	{
		pstActionRes->Data.Hit.AtkPos = pstAtker->stCurrPos;
		switch(pstReflectedDamage->astEntries[i].stFrom.iType)
		{
		case OBJ_PLAYER:
			pstFromPlayer = 
				pstReflectedDamage->astEntries[i].stFrom.stObj.pstPlayer;
			pstActionRes->ID = pstFromPlayer->iMemID;
			stFromAtkPos = pstFromPlayer->stRoleData.Pos;
			break;

		case OBJ_MONSTER:
			pstFromMon = pstReflectedDamage->astEntries[i].stFrom.stObj.pstMon;
			pstActionRes->ID = pstFromMon->iID;
			stFromAtkPos = pstFromMon->stCurrPos;
				
			break;

		default:
			return -1;
		}
		
		if(!pstReflectedDamage->astEntries[i].stHit.TotalHurt)
		{
			continue;
		}

		if(z_is_mon_status_result(pstEnv, pstAtker, STATUS_RESULT_INVIN))
		{
			continue;
		}

		if ( z_distance(&stFromAtkPos, &pstAtker->stCurrPos)  > REFLECT_MAX_DISTANCE )
		{
			continue;
		}

		// 生命值减少
		j = pstReflectedDamage->astEntries[i].stHit.TotalHurt;
		while(j--)
		{
			int iHit_absorb = 0;
			int *piOneHurtHp = 0;
			CSONEHIT stHit_absorb;
	
			piOneHurtHp = &pstReflectedDamage->astEntries[i].stHit.Hurts[j].Value;
			memset(&stHit_absorb,0,sizeof(stHit_absorb));
			iHit_absorb = z_mon_hurt_status_absorb(pstEnv,pstAtker, &stHit_absorb, 
						piOneHurtHp);
			if( (iHit_absorb>=0) && (*piOneHurtHp == 0) )
			{
				pstReflectedDamage->astEntries[i].stHit.Hurts[j].HurtType = HURT_TYPE_SBSORB;
			}
			
			iHP += pstReflectedDamage->astEntries[i].stHit.Hurts[j].Value;
		}

		 if( 0 <= z_mon_dec_one_hp_by_hit(pstEnv, pstAtker) )
		{
			iHP = 1;
		}
		 
		if (iHP > pstAtker->iHP)
		{
			pstAtker->iHP = 0;
		}
		else
		{
			pstAtker->iHP -= iHP;
		}
		
		if(0 >= pstAtker->iHP)
		{
			pstAtkProc->stDeadList.iSelf = 1;
			pstAtkProc->stDeadList.stSelfDead.stAtker = 
				pstReflectedDamage->astEntries[i].stFrom;

			//break;
		}

		// 将敌人技能回放出来
		pstActionRes->ActionID = ACTION_HIT;
		pstActionRes->Data.Hit.Flags = HIT_FLAG_BE_REFLECTED;
		pstActionRes->Data.Hit.DirFace = 0;
		pstActionRes->Data.Hit.SkillID = 
			pstAtkProc->stAtkerInfo.pstSkillDef->SkillID;
		pstActionRes->Data.Hit.Level =
			pstAtkProc->stAtkerInfo.pstSkillDef->Level;
		
		pstActionRes->Data.Hit.SkillSeq = 0;
		pstActionRes->Data.Hit.SelfPosChgFlag = 0;
		//pstActionRes->Data.Hit.FittingsEffectID = 0;
		pstActionRes->Data.Hit.RuneTransferedID =
			pstActionRes->Data.Hit.SkillID;
		pstActionRes->Data.Hit.TotalHit = 1;
 		pstActionRes->Data.Hit.Hits[0] =
 			pstReflectedDamage->astEntries[i].stHit;
		pstActionRes->Data.Hit.Hits[0].ID = pstAtker->iID;
		pstActionRes->Data.Hit.Hits[0].HP = pstAtker->iHP;
		if(pstAtkProc->stDeadList.iSelf)
		{
			pstActionRes->Data.Hit.Hits[0].DeadFlag = 1;
		}
		
		Z_CSHEAD_INIT(&pstEnv->stCSRes.Head, ACTION_RES);
		switch(pstReflectedDamage->astEntries[i].stFrom.iType)
		{
		case OBJ_PLAYER:
			if(pstFromPlayer)
			{
				z_send_player_hit_res(pstEnv->pstAppCtx, pstEnv, pstFromPlayer,
                                  &pstActionRes->Data.Hit.AtkPos,
                                  &pstEnv->stCSRes);
			}
			break;

		case OBJ_MONSTER:
			z_send_mon_hit_res(pstEnv->pstAppCtx, pstEnv, pstFromMon,
			                   &pstEnv->stCSRes);
			break;

		default:
			return -1;
		}
	}

	return 0;
}
		
int z_player_hit_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, Player *pstAtkee, AtkProcess *pstAtkProcess)
{
	int i;
	int iHP;
	//int iIdx;
	long long llTotalHurt;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	int iRealHurt, iHurt, iHeavyHit;
	ATKCHG *pstAtkChg = NULL;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	ZoneSkill *pstZoneSkill = pstAtkerInfo->pstZoneSkill;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	ROLETMPSYS *pstRoleTmpSys = &pstAtkee->stRoleData.MiscInfo.TmpSys;
	ZoneStatusList *pstZoneStatusList = &pstAtkee->stOnline.stStatusList;
	ROLETMPSYS *pstRoleTmpSysAtker = &pstAtker->stRoleData.MiscInfo.TmpSys;
	ITEMDEF *pstItemDefTmp = NULL;
	struct timeval stCltTime;
	ZoneReflectedDamage *pstReflectedDamage = &pstAtkProcess->stReflectedDamage;
	int iReflect = 0;
	STATUSRESULT *pstStatusResult = NULL;
	SKILLRESULT *pstSkillResultAddHurt = NULL;
	Player *pstSkillAtker = pstAtker;
	ZoneAni stAtkerAni;

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		if (pstAtkee->stRoleData.Map ==  2400 && z_player_find_status(pstAtkee, 3009))	
		{
			return -1;
		}
	}

	//隐身受击显形
	if ((pstAtkee->stOnline.State & CS_STAT_QIANXIN) && (pstSkillDef->TargetType & SKILL_TARGET_ENEMY_PLAYER))
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstAtkee, STATUS_ID_QIANXIN, 1, 0);
	}

	memset(&stCltTime,0,sizeof(stCltTime));
	stCltTime.tv_sec = pstAtkProcess->stAtkerInfo.SvrTime.BenchTimeSec;
	stCltTime.tv_usec = pstAtkProcess->stAtkerInfo.SvrTime.BenchTimeUSec;
	
	stAtkeeInfo.stAtkee.iType = OBJ_PLAYER;
	stAtkeeInfo.stAtkee.stObj.pstPlayer = pstAtkee;

	pstOneHit->ID = pstAtkee->iMemID;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;

	pstAtkee->stOnline.iAtkerTarget = pstAtker->iMemID;

	if (pstSkillDef->FairySkill)
	{
		z_get_fairy_dyn(pstEnv, pstAtker, pstAtkerDyn,pstSkillDef,pstAtkProcess);
		//z_get_pet_dyn(pstAtker, pstAtkerDyn, pstZoneSkill->ullPetWID);
	}
	else
	{
		memcpy(pstAtkerDyn, &pstAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
		if(pstAtker->stOnline.iSkillAtkerID > 0)
		{
			Player *pstRealAtker = z_id_player(pstAppCtx, pstEnv, pstAtker->stOnline.iSkillAtkerID);
			if(pstRealAtker)
			{
				memcpy(pstAtkerDyn, &pstRealAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
			}
		}
		
		if (0 > pstAtkProcess->iAtkChgIdx)
		{
			pstAtkProcess->iAtkChgIdx = 0;
			memset(&pstAtkerDyn->stAtkChg, 0, sizeof(pstAtkerDyn->stAtkChg));

			pstAtkerDyn->stAtkChg.iAtkLevelMul = get_player_level_mul(pstEnv, pstAtker, LEVEL_MUL_ATK);
		}
	}

	if (pstAtkProcess->iAtkChgIdx >= 0)
	{
		pstAtkChg = &pstAtkerDyn->stAtkChg;
	}
	if( pstAtkChg == NULL)
	{
		return 0;
	}

	z_player_normal_atk_cacl(pstEnv, pstAtkChg,pstSkillDef ,pstAtker, pstAtkProcess);
	
	memset (pstAtkerInfo->ucAppendResult, 0, sizeof(pstAtkerInfo->ucAppendResult));
	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;
	
	for (i=0; i<pstZoneStatusList->bBadStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstSkillDef,pstAtkerInfo, pstAtkChg, &pstZoneStatusList->astBadStatus[i]);
	}
	for (i=0; i<pstZoneStatusList->bGoodStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstSkillDef,pstAtkerInfo, pstAtkChg, &pstZoneStatusList->astGoodStatus[i]);
	}

	 //检查自己好的状态对技能的修正
	for (i=0; i<pstAtker->stOnline.stStatusList.bGoodStatusNum; i++)
	{
		z_atker_status_result(pstEnv, pstSkillDef, pstAtkerInfo, pstAtkChg, &pstAtker->stOnline.stStatusList.astGoodStatus[i],pstAtkProcess);
	}
	
	z_cacl_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	player_collectskill_add_slot(pstEnv, pstAtkee,0,COLLECTSKILL_ADDTYPE_AKEE);
	

	if (pstRoleTmpSysAtker->TmpItemID > 0)
	{
		pstItemDefTmp = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSysAtker);
	}

	if (0 == (pstItemDefTmp && pstItemDefTmp->ItemTmp.Type >= ITEM_TMP_ITEM))
	{
		//z_cacl_player_status_result(pstEnv, pstAtkerDyn, pstAtkChg, pstAtker);

		// 计算组队契约对攻防的影响
		z_cacl_player_qiyue_result(pstEnv, pstAtkerDyn, pstAtkChg, pstAtker);
	}
	
	if (pstSkillDef->StatusChg.StatusID > 0 && z_player_find_status(pstAtker, pstSkillDef->StatusChg.StatusID))
	{
		int iStatusChgHurt = 0;
		int iStatusChgHurtMul = 0;
		ZoneStatus *pstZoneStatus = NULL;

		iStatusChgHurt = pstSkillDef->StatusChg.Hurt;
		iStatusChgHurtMul = pstSkillDef->StatusChg.HurtMul;
		
		pstZoneStatus = z_player_find_status(pstAtker, pstSkillDef->StatusChg.StatusID);
		if(pstZoneStatus)
		{
			z_status_mul_skill_hurt(pstEnv,pstZoneStatus ,&iStatusChgHurt,&iStatusChgHurtMul);
		}
		
		pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += iStatusChgHurt;
		pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iStatusChgHurtMul;

		pstAtkerDyn->stHitRateChg.nHeavyHitAdd += pstSkillDef->StatusChg.Heavy;
	}

	memcpy(pstAtkeeDyn, &pstAtkee->stOnline.stDyn, sizeof(*pstAtkeeDyn));
	//z_cacl_player_status_result(pstEnv, pstAtkeeDyn, NULL, pstAtkee);
	if (pstRoleTmpSys->TmpItemID > 0)
	{
		z_cacl_tmpitem_attr(pstEnv, pstRoleTmpSys, pstAtkeeDyn);
	}

	if(pstAtker->stOnline.iSkillAtkerID)
	{
		Player *pstTmpPlayer;
		
		pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstAtker->stOnline.iSkillAtkerID);
		if(pstTmpPlayer && pstTmpPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			pstSkillAtker = pstTmpPlayer;
		}
	}

	stAtkerAni.iType = OBJ_PLAYER;
	stAtkerAni.stObj.pstPlayer = pstSkillAtker;
	atker_set_pktime(pstEnv, &stAtkerAni, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);

	pstHits->TotalHit++;

	if(pstSkillDef->NoHurtSkill == 0)
	{
		//符文额外附加技能效果
		rune_addattr_skill(pstEnv, pstAtkProcess, &stAtkeeInfo);
	}

	player_hit_jiqi_full_add_state_target( pstEnv, pstAtkProcess, 
						&stAtkeeInfo, pstSkillDef, pstAtkerInfo->cJiqiNum);

	if (pstSkillDef->NoHurtSkill) //非伤害技能，只需要处理技能效果
	{
		if ((pstSkillDef->TargetType & SKILL_TARGET_ENEMY_PLAYER) && z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
			pstOneHit->TotalHurt++;
			
			return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
		}
				
		z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//攻击计算副本次数了
	map_pworld_chg_limitflag(pstAppCtx, pstEnv, pstAtker);
		
	llTotalHurt = 0;
	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;

	
	//受攻方是否无敌状态
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	// 受功放是否绝对闪避
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_UTTERLY_FLEE))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		//genius_add_status_by_flee(pstAppCtx, pstEnv, pstAtkee);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	if(is_in_miss_time_player(&stCltTime, pstAtkee))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		//genius_add_status_by_flee(pstAppCtx, pstEnv, pstAtkee);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += pstZoneSkill->stSkillMul.HeavyHitRateMul;//天赋的暴击修正
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		//暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0/(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}	
		
		iHeavyHit += player_addattr_heavyhit_add(pstAtkerInfo, &stAtkeeInfo);
		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	z_del_result_by_heavyhit(pstEnv->pstAppCtx, pstEnv,pstAtker,NULL,iHeavyHit);
	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	// 吸收全部伤害
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_SORB_ALL_HURT))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_SBSORB;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iRealHurt;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DEC_PERCENT_MAX_HP))
	{
		STATUSRESULT stResult;
		z_get_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DEC_PERCENT_MAX_HP,
			                           &stResult, 0);

		MIDATTRLIST *pstMidAttrList = &pstAtkee->stOnline.stDyn.stMidAttrList;
		int iTemp  = pstMidAttrList->uiHPMax * (stResult.ResultVal1/100.0);

		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iTemp;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//被暴击加hp
	//genius_add_hp_by_heavyhit( pstAppCtx, pstEnv, pstAtkee,pstOneHit,iHeavyHit);
	//genius_reduce_skill_cd_time_by_heavyhit( pstAppCtx,  pstEnv, pstAtkee, &stCltTime,iHeavyHit);
	//genius_add_status_by_player_atk( pstAppCtx, pstEnv, pstAtker, pstAtkee);
	//天赋 普通攻击触发对自身的buff
	//genius_atk_random_add_status(pstAppCtx, pstEnv, pstAtker, pstSkillDef->SkillType);

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;

	z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	z_do_extended_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo,
		                   pstAtkChg);

	pstStatusResult = player_status_result(pstEnv, pstAtkee, STATUS_RESULT_REFLECTION);
	if(pstStatusResult &&
		pstReflectedDamage->iNum < (int)(sizeof(pstReflectedDamage->astEntries)/sizeof(pstReflectedDamage->astEntries[0])))
	{
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stFrom.iType = OBJ_PLAYER;
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stFrom.stObj.pstPlayer = pstAtkee;
		/*
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stHit = *pstOneHit;
		*/

		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stHit.TotalHurt = 0;
		++pstReflectedDamage->iNum;

		iReflect = 1;
	}

	pstSkillResultAddHurt = z_get_skill_result(pstSkillDef,RESULT_ADDHURT_BY_TARGETSTATUS);
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		pstAtkProcess->iContinueCount--;

		if (iHurt <= 0)
		{
			continue;
		}
		
		if (iReflect)
		{
			z_hurt_reflect(pstEnv, &iHurt, pstStatusResult, pstReflectedDamage);
		}
		
		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstAtkee->stRoleData.RoleDetail.HPCurr;
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}
		else
		{
			// 伤害吸收计算
			if (0 == (pstAtkerInfo->iFlag & ATKER_INFO_FLAG_THROUGH_ABSORB))
			{
				z_player_hurt_absorb(pstEnv, pstAtkee, pstOneHit, &iHurt);
				z_player_hurt_status_absorb(pstEnv,pstAtkee, pstOneHit,  &iHurt);
			}
		}

		if (iHurt <= 0)
		{
			continue;
		}

		llTotalHurt += iHurt;
		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;
		pstOneHit->TotalHurt++;

		
		if( pstSkillResultAddHurt &&
			RAND1(100) <= pstSkillResultAddHurt->SkillResultVal1)
		{
			ZoneStatus *pstZoneStatusTmp = NULL;
			if(pstSkillResultAddHurt->SkillResultVal3 ==0)
			{
				pstZoneStatusTmp = z_player_find_status(pstAtkee, pstSkillResultAddHurt->SkillResultVal2);
			}
			else 
			{
				pstZoneStatusTmp = z_player_find_status_by_level(pstAtkee, pstSkillResultAddHurt->SkillResultVal2,pstSkillResultAddHurt->SkillResultVal3);
			} 
			if(pstZoneStatusTmp)
			{
				llTotalHurt += iHurt*pstSkillResultAddHurt->SkillResultVal4/100.0;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt*pstSkillResultAddHurt->SkillResultVal4/100.0;
				pstOneHit->TotalHurt++;
			}
		}

		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		if (llTotalHurt >= (iHP + pstAtkee->stRoleData.RoleDetail.HPCurr))
		{
			pstAtkProcess->cDeadHit = 1;
			break;
		}
	}while (pstAtkProcess->iContinueCount > 0);

	if (!pstAtkProcess->cDeadHit && pstAddAtkInfo->iMainAttrHurt != 0) //处理追招等附加伤害
	{
		z_add_hurt(pstAtkProcess, pstOneHit);
	}

	// 通知客户端添加临时仇人  副本中不加
	/*iIdx = z_find_tmpenemy(pstAtkee, pstAtker->stRoleData.RoleID);

	if (iIdx < 0 && pstAtkee->stRoleData.Map < PWORLD_MAPID_BASE)
	{
		relation_chg_clt_detail(pstEnv, pstAtkee, pstAtker, RELATION_TYPE_TMPENEMY);
		z_insert_tmpenemy(pstAtker, pstAtkee);
	}*/

	if (0 == pstSkillDef->BeatBack && 
		pstAtkProcess->stBeatBackList.iBeatBackNum < 
			(int)(sizeof(pstAtkProcess->stBeatBackList.astBeatBackInfo)/sizeof(pstAtkProcess->stBeatBackList.astBeatBackInfo[0])))
	{
		STATUSRESULT stResult;
		
		z_get_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_BEATBACK, &stResult, 1);
		if (stResult.ResultVal2 != 0)
		{
			BeatBackInfo *pstBeatBackInfo;
			
			pstBeatBackInfo = &pstAtkProcess->stBeatBackList.astBeatBackInfo[pstAtkProcess->stBeatBackList.iBeatBackNum];
			pstBeatBackInfo->pstZoneSkill = z_get_player_zoneskill(pstAtkee, stResult.ResultVal2);
			if (pstBeatBackInfo->pstZoneSkill)
			{
				pstBeatBackInfo->pstBeater = pstAtkee;
				pstAtkProcess->stBeatBackList.iBeatBackNum++;	
			}
		}
	}

	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0); 
}

/*
int  z_cacl_hurt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, AtkProcess *pstAtkProc)
{
	int i,j;
	float fHurtRate;
	CSONEHIT *pstOneHit;
	char HurtType, cType;
	int Value;
	int iHP, iMP;
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayer;
	Player *pstAtker;
	Monster *pstMon;
	PlayerChagAttrVal stValChg;
	unsigned int uiHurtLimit;
	ZoneDeadList *pstZoneDeadList = &pstAtkProc->stDeadList;
	CSHITS *pstHits = &pstAtkProc->stHits;
	SKILLDEF *pstSkillDef = pstAtkProc->stAtkerInfo.pstSkillDef;
	long long llTotalHurt = 0;
	unsigned short unTraceDist;
	short shPressRate = 0;
	short shPressRateTmp = 0;
	AtkerInfo *pstAtkerInfo = &pstAtkProc->stAtkerInfo;
	int iRet = 0;

	for (i=0; i<pstHits->TotalHit; i++)
	{
		pstOneHit = &pstHits->Hits[i];
		for (j=0; j<pstOneHit->TotalHurt; j++)
		{
			HurtType = pstOneHit->Hurts[j].HurtType;
			Value = pstOneHit->Hurts[j].Value;
			switch(HurtType)
			{
				case HURT_TYPE_DMG:
				case HURT_TYPE_HEAVY_DMG:
				case HURT_TYPE_HITDEAD:
					llTotalHurt +=  Value;
					break;
				case HURT_TYPE_HP:
					if (Value < 0)
					{
						Value = -Value;
						llTotalHurt += Value;
						pstOneHit->Hurts[j].HurtType = HURT_TYPE_DMG;
						pstOneHit->Hurts[j].Value = Value;
					}
					break;
				default:
					break;
			}
		}
	}

	uiHurtLimit = pstSkillDef->HurtLimit;
	fHurtRate = 1;
	if (uiHurtLimit > 0)
	{
		if (OBJ_PLAYER == pstAtkProc->stAtkerInfo.stAtker.iType)
		{
			uiHurtLimit += pstAtkProc->stAtkerInfo.iHurtLimitAdd;
		}

		if (llTotalHurt > uiHurtLimit)
		{
			fHurtRate = (1.0*uiHurtLimit)/llTotalHurt;
		}
	}

	for (i=0; i<pstHits->TotalHit; i++)
	{
		llTotalHurt = 0;
		iHP = iMP = 0;
		pstOneHit = &pstHits->Hits[i];
		for (j=0; j<pstOneHit->TotalHurt; j++)
		{
			HurtType = pstOneHit->Hurts[j].HurtType;
			Value = pstOneHit->Hurts[j].Value;
			switch(HurtType)
			{
				case HURT_TYPE_DMG:
				case HURT_TYPE_HEAVY_DMG:
				case HURT_TYPE_HITDEAD:
					pstOneHit->Hurts[j].Value = fHurtRate * Value;
					llTotalHurt += Value;
					break;
				case HURT_TYPE_HP:
					iHP += Value;
					break;
				case HURT_TYPE_MP:
					iMP += Value;
					break;
				default:
					break;
			}
		}

		if (pstOneHit->ID >= PET_ID_BASE)
		{
			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstOneHit->ID%PET_ID_BASE, &cType);
		}
		else
		{
			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstOneHit->ID, &cType);
		}
		
		if (0 > iIndex)
		{
			continue;
		}

		switch(cType)
		{
			case OBJ_PLAYER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					break;
				}
				pstPlayer = (Player *)pstMemBlock->szData;

				if (pstOneHit->ID >= PET_ID_BASE) //宠物 
				{
					PetFightShiOnline *pstPetFightShiOnline;
					ROLEPET *pstRolePet;
						
					pstPetFightShiOnline = pet_get_shipet_by_xiedaipos(pstPlayer, (pstOneHit->ID/PET_ID_BASE)-1);
					if (NULL == pstPetFightShiOnline)
					{
						break;
					}
					pstRolePet = z_find_role_pet(pstPlayer, pstPetFightShiOnline->stRolePetOnline.ullWID, NULL);
					if (NULL == pstRolePet)
					{
						break;
					}

					pet_add_power(pstEnv, pstPlayer, pstRolePet, iHP-llTotalHurt);
					pstOneHit->HP = pstRolePet->HP;
					if (0 == pstRolePet->HP)
					{
						pstOneHit->DeadFlag = 1;
					}
					
					break;
				}

				z_atkee_prepare_break(pstEnv->pstAppCtx, pstEnv, pstPlayer, llTotalHurt, 0);

				//特殊任务被击
				if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_BEATK_TRACE)
				{
					task_beatk_player_trace(pstEnv, pstPlayer);
				}
				
				stValChg.AttrNum = 0;
				
				stValChg.AttrList[stValChg.AttrNum].AttrID = ATTR_ID_HP;
				if (llTotalHurt - iHP > 0)
				{
					stValChg.AttrList[stValChg.AttrNum].nFlag = 1;
					stValChg.AttrList[stValChg.AttrNum].Value.HP = llTotalHurt - iHP;

					pstAtker = get_atker_player(pstEnv,  &pstAtkProc->stAtkerInfo.stAtker);
					if (	(pstPlayer->stRoleData.RoleDetail.HPCurr - (llTotalHurt - iHP)<= 0)  && 
						pstAtker &&
						z_is_qiecuo(pstEnv,pstAtker, pstPlayer))
					{
						stValChg.AttrList[stValChg.AttrNum].Value.HP = 	pstPlayer->stRoleData.RoleDetail.HPCurr-1;
						iRet = 1;
					}
				}
				else 
				{
					stValChg.AttrList[stValChg.AttrNum].nFlag = 0;
					stValChg.AttrList[stValChg.AttrNum].Value.HP = iHP - llTotalHurt;
				}
				stValChg.AttrNum++;

				stValChg.AttrList[stValChg.AttrNum].AttrID = ATTR_ID_MP;
				if (iMP > 0)
				{
					stValChg.AttrList[stValChg.AttrNum].nFlag = 0;
					stValChg.AttrList[stValChg.AttrNum].Value.MP = iMP;
				}
				else
				{
					stValChg.AttrList[stValChg.AttrNum].nFlag = 1;
					stValChg.AttrList[stValChg.AttrNum].Value.MP = -iMP;
				}
				stValChg.AttrNum++;

				z_attr_chg(pstAppCtx, pstEnv, pstPlayer, &stValChg, 0);

				if (pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax > 0)
				{
					pstOneHit->HP = pstPlayer->stRoleData.RoleDetail.HPCurr;
				}

				if (0 == pstPlayer->stRoleData.RoleDetail.HPCurr && !(pstPlayer->stOnline.State & CS_STAT_DEAD))
				{
					pstOneHit->DeadFlag = 1;
					
					pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtkee.iType = OBJ_PLAYER;
					pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtkee.stObj.pstPlayer = pstPlayer;
					pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtker = pstAtkProc->stAtkerInfo.stAtker;
					pstZoneDeadList->iCount++;
				}

				break;
			case OBJ_MONSTER:
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
				pstMon = (Monster *)pstMemBlock->szData;
				pstMon->iHP -= (llTotalHurt - iHP); 
				if (pstMon->iHP < 0)
				{
					pstMon->iHP = 0;
				}
				else if (pstMon->iHP > pstMon->iMaxHP)
				{
					pstMon->iHP = pstMon->iMaxHP;
				}

				pstOneHit->HP = pstMon->iHP;
				
				if (0 == pstMon->iHP &&  (pstMon->cStat != MONSTER_DIE))
				{
					pstOneHit->DeadFlag = 1;
						
					pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtkee.iType = OBJ_MONSTER;
					pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtkee.stObj.pstMon = pstMon;
					pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtker = pstAtkProc->stAtkerInfo.stAtker;
					pstZoneDeadList->iCount++;
				}
				
				unTraceDist = 0;
				if(pstAtkProc->stAtkerInfo.stAtker.iType == OBJ_MONSTER)
				{
					unTraceDist =  z_distance(&pstAtkProc->stAtkerInfo.stAtker.stObj.pstMon->stCurrPos, &pstMon->stCurrPos);
				}
				else
				{
				 	unTraceDist = z_distance(&(pstAtkProc->stAtkerInfo.stAtker.stObj.pstPlayer)->stRoleData.Pos, &pstMon->stCurrPos);
				}
					
				//if ( (llTotalHurt - iHP) > 0 && 
				//	(0 == pstMon->unTraceDist || z_distance(&(pstAtkProc->stAtkerInfo.stAtker.stObj.pstPlayer)->stRoleData.Pos, &pstMon->stCurrPos) <= pstMon->unTraceDist))
				if ( (llTotalHurt - iHP) > 0 && 
					(0 == pstMon->unTraceDist || unTraceDist <= pstMon->unTraceDist) )
				{
					if (pstAtkProc->stAiList.iCount < (int)(sizeof(pstAtkProc->stAiList.astMonAi)/sizeof(pstAtkProc->stAiList.astMonAi[0])))
					{
						AIEvt *pstAIEvt = &pstAtkProc->stAiList.astMonAi[pstAtkProc->stAiList.iCount];
						//保证下,最小也不要超过-100
						shPressRateTmp = shPressRate + pstSkillDef->PressRate;
						if(shPressRateTmp < -100 )
						{
							shPressRateTmp =  -100;
						}
						
						pstAIEvt->iEvtID = AI_EVT_BEATK;
						pstAIEvt->stEvtDetail.stBeAtk.iHurt = llTotalHurt - iHP;

						if (pstSkillDef)
						{
							//pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = pstSkillDef->PressRate+shPressRate;
							pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = shPressRateTmp;
						}
						else
						{
							pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = 0;
						}

						pstAtkProc->stAiList.apstMon[pstAtkProc->stAiList.iCount] = pstMon;

						if (OBJ_PLAYER == pstAtkProc->stAtkerInfo.stAtker.iType)
						{
							pstAIEvt->stEvtDetail.stBeAtk.stAtker.iType = OBJ_PLAYER;
							pstAIEvt->stEvtDetail.stBeAtk.stAtker.stObj.pstPlayer = pstAtkProc->stAtkerInfo.stAtker.stObj.pstPlayer;
							pstAtkProc->stAiList.iCount++;
						}
						else if (OBJ_PET == pstAtkProc->stAtkerInfo.stAtker.iType)
						{
							pstAIEvt->stEvtDetail.stBeAtk.stAtker.iType = OBJ_PLAYER;
							pstAIEvt->stEvtDetail.stBeAtk.stAtker.stObj.pstPlayer = pstAtkProc->stAtkerInfo.stAtker.stObj.stZonePet.pstOwner;
							pstAtkProc->stAiList.iCount++;
						}
						else if (OBJ_MONSTER == pstAtkProc->stAtkerInfo.stAtker.iType)
						{
							Monster *pstAtkerMon;

							pstAtkerMon = pstAtkProc->stAtkerInfo.stAtker.stObj.pstMon;
							if (pstAtkerMon->iOwnerID > 0)
							{
								pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtkerMon->iOwnerID);
								if (pstPlayer)
								{
								       // 图腾主人的压力值填0 
									pstAIEvt->stEvtDetail.stBeAtk.iHurt = 0;
									pstAIEvt->stEvtDetail.stBeAtk.stAtker.iType = OBJ_PLAYER;
									pstAIEvt->stEvtDetail.stBeAtk.stAtker.stObj.pstPlayer = pstPlayer;
									pstAtkProc->stAiList.iCount++;
								}
                                                        //图腾的压力值正常压入怪物压力表
							}

							if (pstAtkProc->stAiList.iCount < (int)(sizeof(pstAtkProc->stAiList.astMonAi)/sizeof(pstAtkProc->stAiList.astMonAi[0])))
							{
								AIEvt *pstAIEvt = &pstAtkProc->stAiList.astMonAi[pstAtkProc->stAiList.iCount];

								pstAIEvt->iEvtID = AI_EVT_BEATK;
								pstAIEvt->stEvtDetail.stBeAtk.iHurt = llTotalHurt - iHP;

								if (pstSkillDef)
								{
									//pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = pstSkillDef->PressRate+shPressRate;
									pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = shPressRateTmp;
								}
								else
								{
									pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = 0;
								}

								pstAtkProc->stAiList.apstMon[pstAtkProc->stAiList.iCount] = pstMon;
					
								pstAIEvt->stEvtDetail.stBeAtk.stAtker.iType = OBJ_MONSTER;
								pstAIEvt->stEvtDetail.stBeAtk.stAtker.stObj.pstMon = pstAtkerMon;
								pstAtkProc->stAiList.iCount++;
							}
						}
					}
				}
				
				break;
			default:
				break;
		}

		
		// 吸收HP
		if (pstAtkProc->cSordHpRate != 0)
		{
			pstAtkerInfo->stResourceConsume.iHPUse -= llTotalHurt*pstAtkProc->cSordHpRate/100.0;
		}

		if(pstAtkProc->iSordHp != 0)
		{
			pstAtkerInfo->stResourceConsume.iHPUse -= pstAtkProc->iSordHp;
		}

		// 吸收MP
		if (pstAtkProc->cSordMpRate != 0)
		{
			pstAtkerInfo->stResourceConsume.iMPUse -= llTotalHurt*pstAtkProc->cSordMpRate/100.0;
		}
	}
	return iRet;
}*/

int z_send_player_hit_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstAtkPos, CSPKG *pstPkg)
{
	ZONEMAPINST *pstMapInst;
	ZONEIDX stIdx;

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (pstMapInst->bBroadcast)
	{
		ZONEPWORLDINST *pstPworldInst;
		
		pstPworldInst = z_find_pworld_inst(pstEnv->pstMapObj, pstMapInst->iPworldInstID);
		if (pstPworldInst)
		{
			return pworld_map_broadcast(pstAppCtx, pstEnv, pstPworldInst, pstMapInst, 0, pstPkg);
		}
	}
	else
	{
		z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, pstPkg, 1);
	}

/*
	for (i=0; i<pstHits->TotalHit; i++)
	{
		pstOneHit = pstHits->Hits+i;
		pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstOneHit->ID);
		if (pstPlayerTmp)
		{
			apstPlayerHit[iPlayerHit++] = pstPlayerTmp;
			pstPlayerTmp->stOnline.cMiscFlag &= ~MISC_FLAG_TAG;
		}
	}
	apstPlayerHit[iPlayerHit++] = pstPlayer;
	pstPlayer->stOnline.cMiscFlag &= ~MISC_FLAG_TAG;

	if (z_pos_in_view_half(&pstPlayer->stRoleData.Pos, pstAtkPos)) //以主动攻击者视野为准
	{
		for (i = 0; i < pstPlayer->stOnline.iVAnimate; i++)
		{
			if (iPlayerNotify >= VISIBLE_ANIMATE_VALVE)
			{
				break;
			}
			
			pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.aiVAnimate[i]);
			if (NULL == pstPlayerTmp) continue;
			if (pstPlayer->stOnline.iVAnimate > VISIBLE_ANIMATE_VALVE && 
				!z_pos_in_view_half(&pstPlayer->stRoleData.Pos, &pstPlayerTmp->stRoleData.Pos))
			{
				continue;
			}

			apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
			pstPlayerTmp->stOnline.cMiscFlag |= MISC_FLAG_TAG;
		}
	}
	else //以打击点为准
	{
		if(0 == z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, pstAtkPos, &stSArea))
		{
			int j;
			AREADYN *pstArea;
			
			for(i = 0; i < stSArea.iNum; i++)
			{
				if (iPlayerNotify >= VISIBLE_ANIMATE_VALVE)
				{
					break;
				}
				
				pstArea = stSArea.pastArea[i];

				for (j = 0; j < pstArea->iAnimate; j++)
				{
					if (iPlayerNotify >= VISIBLE_ANIMATE_VALVE)
					{
						break;
					}
					
					pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
					if (NULL == pstPlayerTmp || PLAYER_STATUS_ROLE_LOGIN != pstPlayerTmp->eStatus)
					{
						continue;
					}

					if (pstArea->iAnimate > VISIBLE_ANIMATE_VALVE && 
						!z_pos_in_view_half(&pstPlayer->stRoleData.Pos, &pstPlayerTmp->stRoleData.Pos))
					{
						continue;
					}

					apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
					pstPlayerTmp->stOnline.cMiscFlag |= MISC_FLAG_TAG;
				}
			}
		}
	}
	
	for (i=0; i<iPlayerHit; i++)
	{
		pstPlayerTmp = apstPlayerHit[i];
		if (0 == (pstPlayerTmp->stOnline.cMiscFlag & MISC_FLAG_TAG))
		{
			pstPlayerTmp->stOnline.cMiscFlag |= MISC_FLAG_TAG;
			apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
		}
	}

	z_cltmsg_limit_broadcast(pstAppCtx, pstEnv, apstPlayerNotify, iPlayerNotify, pstPkg);
*/
	
	return 0;
	UNUSED(pstAtkPos);
}

SKILLRESULT *z_skill_get_result(SKILLDEF *pstSkillDef, int iResultID)
{
	int i;

	for (i=0; i<MAX_SKILL_RESULT; i++) 
	{
		if (0 == pstSkillDef->SkillResult[i].SkillResultID)
		{
			break;
		}

		if (iResultID == pstSkillDef->SkillResult[i].SkillResultID)
		{
			return pstSkillDef->SkillResult+i;
		}
	}

	return NULL;
}

int z_skill_result_poschg_by_mon(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, SKILLDEF *pstSkillDef, int iDist,
								Monster *pstMon, RESPOS *pstTargetPos, RESPOS *pstRolePos, RESPOS *pstPos)
{
	int iDir;
	TMAPMASK *pstMask;
	RESPOS stPosOri = pstMon->stCurrPos;

	UNUSED(pstSkillDef);
	
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if (0 > iDist) //避免往后跳
	{
		if (z_distance(&stPosOri, pstTargetPos) < -iDist)
		{
			//不能发生位移
			return -1;
		}
	}
	
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	iDir	=	FaceAngleBetween(pstRolePos->X, pstRolePos->Y, pstTargetPos->X, pstTargetPos->Y);
	pstPos->X		=	(int) (pstTargetPos->X + iDist * cos((iDir)*PI/180.0f));
	pstPos->Y		=	(int) (pstTargetPos->Y - iDist * sin((iDir)*PI/180.0f));

	if (IsPixelStop(pstMask, pstPos->X, pstPos->Y) || 
		!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstPos->X, pstPos->Y,
							pstPos->X, pstPos->Y, 0,DYN_MSK_STOP) ||
		NULL == z_pos_area(pstEnv->pstMapObj, pstMapIdx, pstPos))
	{
		if (IsPixelStop(pstMask, pstTargetPos->X, pstTargetPos->Y) ||
			!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstTargetPos->X, pstTargetPos->Y,
							pstTargetPos->X, pstTargetPos->Y, 0,DYN_MSK_STOP))
		{
			return -1;
		}

		//只能位移到AtkPos
		*pstPos = *pstTargetPos;
	}

	if(is_mon_machine(pstMon))
	{
		Player *pstPlayer = machine_get_driver(pstEnv, pstMon);
		if(!pstPlayer)
		{
			return -1;	
		}

		stPosOri = pstPlayer->stRoleData.Pos;
		pstPlayer->stRoleData.Pos = *pstPos;
		SET_PLAYER_NOMOVE(pstPlayer);
		z_player_move_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer);
		z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPosOri, 0);

		z_mon_machine_move(pstEnv, pstPlayer);		
	}
	else
	{
		pstMon->stCurrPos= *pstPos;
		pstMon->cMoving = 0;
		z_mon_stop(pstEnv,pstMon);
		z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstMon->iID, &stPosOri, 0);
	}
	

	return 0;
}


int z_skill_result_poschg(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, MapIndex *pstMapIdx, SKILLDEF *pstSkillDef, int iDist,
								SKILLRESULT *pstSkillResult, Player *pstPlayer, RESPOS *pstTargetPos, RESPOS *pstRolePos, RESPOS *pstPos)
{
	int iDir;
	TMAPMASK *pstMask;
	RESPOS stPosOri = pstPlayer->stRoleData.Pos;

	UNUSED(pstSkillDef);
	
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if (0 == pstSkillResult->SkillResultVal2)
	{
		if (0 > iDist) //避免往后跳
		{
			if (z_distance(&stPosOri, pstTargetPos) < -iDist)
			{
				//不能发生位移
				return -1;
			}
		}
	}
		
	pstMask = (TMAPMASK *)(pstEnv->pstMapObj->sMask + pstMapIdx->iMaskOrigin);

	iDir = FaceAngleBetween(pstRolePos->X, pstRolePos->Y, pstTargetPos->X, pstTargetPos->Y);
	if (0 == pstSkillResult->SkillResultVal2)
	{
		pstPos->X		=	(int) (pstTargetPos->X + iDist * cos((iDir)*PI/180.0f));
		pstPos->Y		=	(int) (pstTargetPos->Y - iDist * sin((iDir)*PI/180.0f));
	}
	else
	{
		pstPos->X		=	(int) (pstRolePos->X + iDist * cos((iDir)*PI/180.0f));
		pstPos->Y		=	(int) (pstRolePos->Y - iDist * sin((iDir)*PI/180.0f));
	}
	
	if (!IsPixelLineThrough(pstMask, pstRolePos->X, pstRolePos->Y, pstPos->X, pstPos->Y, maskMagic) ||
		!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstRolePos->X, pstRolePos->Y, 
			pstPos->X, pstPos->Y, 0, DYN_MSK_MAGIC) ||
		IsPixelStop(pstMask, pstPos->X, pstPos->Y) || 
		!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstPos->X, pstPos->Y,
							pstPos->X, pstPos->Y, 0,DYN_MSK_STOP) ||
		NULL == z_pos_area(pstEnv->pstMapObj, pstMapIdx, pstPos))
	{
		if (0 == pstSkillResult->SkillResultVal2)
		{
			if (IsPixelStop(pstMask, pstTargetPos->X, pstTargetPos->Y) ||
				!dyn_msk_check(pstEnv, pstMapInst, pstMask, pstTargetPos->X, pstTargetPos->Y,
								pstTargetPos->X, pstTargetPos->Y, 0,DYN_MSK_STOP))
			{
				return -1;
			}

			//移动到atkpos
			*pstPos = *pstTargetPos;
		}
		else
		{
			return -1;
		}
	}
	
	pstPlayer->stRoleData.Pos = *pstPos;

	//不是所有自身位移都无敌，写屎了。
	if(pstSkillDef->SkillID == 1022 ||
		pstSkillDef->SkillID == 7001 ||
		pstSkillDef->SkillID == 7002)
	{
		pstPlayer->stOnline.stSkillMissTime = pstEnv->pstAppCtx->stCurr;
	}

	SET_PLAYER_NOMOVE(pstPlayer);
	z_player_move_stop(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	z_obj_area_move(pstEnv, pstMapIdx, pstMapInst, pstPlayer->iMemID, &stPosOri, 0);

	pstPlayer->stOnline.iPosSeq += 50;
/*
	if( !z_in_same_area(pstMapIdx, pstEnv->pstAreaDynPool, pstMapInst, pstPosOri, pstPos) )
	{
		if (0 > (iRet = z_ani_leave_area(pstMapIdx, pstEnv->pstAreaDynPool, pstPlayer->iMemID, pstPosOri, pstMapInst)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_ani_leave_area ret %d", pstPlayer->stRoleData.Uin, iRet);
		}
		
		if (0 > (iRet = z_ani_enter_area(pstMapIdx, pstEnv->pstAreaDynPool, pstPlayer->iMemID, pstPos, pstMapInst)))
		{
			tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "player(%d) z_ani_enter_area ret %d", pstPlayer->stRoleData.Uin, iRet);
		}
	}
*/

	return 0;
}

int player_target_totem(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAni)
{
	int iTotemID;
	Monster *pstMonTmp;

	iTotemID = pstPlayer->stOnline.iTotemHead;
	while(iTotemID > 0)
	{
		pstMonTmp = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTotemID);
		if (NULL == pstMonTmp)
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "totem %d error", iTotemID);
			return -1;
		}

		iTotemID = pstMonTmp->iPlayerTotemNext;

		if(pstAni->iType == OBJ_PLAYER)
		{
			player_bub_insert_enemy_player(pstEnv, pstPlayer, pstAni->stObj.pstPlayer);
		}
		else if(pstAni->iType == OBJ_MONSTER)
		{
			player_bub_insert_enemy_mon(pstEnv, pstPlayer, pstAni->stObj.pstMon);
		}
		else if (pstAni->iType == OBJ_NPC)
		{
			player_bub_insert_enemy_mon(pstEnv, pstPlayer, &pstAni->stObj.pstNpc->stMon);
		}

		mon_ai_totem_addpress(pstEnv, pstMonTmp, pstAni, 1);
	}
	
	return 0;
}

int  z_pet_beatk_die(ZONESVRENV* pstEnv, CSHITS *pstHits)
{
	int i;
	Player *pstPlayer;
	PETDEF *pstPetDef;
	CSONEHIT *pstOneHit;
	int iFlag = 0;
	int iLoveVal = PET_DIE_DEC_LOVEVAL;
	ZONEPWORLD *pstZonePworld;
	
	for (i=0; i<pstHits->TotalHit; i++)
	{
		pstOneHit = &pstHits->Hits[i];
		if (pstOneHit->ID >= PET_ID_BASE && pstOneHit->DeadFlag) //宠物 
		{
			PetFightShiOnline *pstPetFightShiOnline;
			ROLEPET *pstRolePet;

			pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstOneHit->ID%PET_ID_BASE);
			if (!pstPlayer)
			{
				continue;
			}
			 
			pstPetFightShiOnline = pet_get_shipet_by_xiedaipos(pstPlayer, (pstOneHit->ID/PET_ID_BASE)-1);
			if (!pstPetFightShiOnline)
			{
				continue;
			}

			pstRolePet = z_find_role_pet(pstPlayer, pstPetFightShiOnline->stRolePetOnline.ullWID, NULL);
			if (!pstRolePet)
			{
				continue;
			}

			pstPetDef = z_find_pet_def(pstEnv, pstRolePet->PetID, &pstRolePet->PetDefIdx);
			if (!pstPetDef)
			{
				return -1;
			}

			if (pstRolePet->HP== 0)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL17, pstRolePet->Name);
				
				pet_set_stat(pstEnv, pstPlayer, pstRolePet, 0);
	
				pstZonePworld = z_find_pworld2(pstEnv, pstPlayer->stRoleData.Map);
				if(pstZonePworld && is_battle_pworld(pstZonePworld))
				{
					pet_add_power(pstEnv, pstPlayer, pstRolePet, pstRolePet->MaxHP);
				}
				else
				{
					pet_add_power(pstEnv, pstPlayer, pstRolePet, pstRolePet->MaxHP*50/100.0);
				}

				//dec loveval
				//宠物天赋,死亡忠诚度不减				
				pet_native_skill_result(pstEnv,pstPlayer,RESULT_NATIVE_LOVE_VAL_DIE_MAP, &iLoveVal,pstRolePet);
				pet_love_val_chg(pstEnv, pstPlayer, pstRolePet, -iLoveVal);

				iFlag = 1;
			}
		}
	}
	
	return 0;
}

int z_player_hit_pet(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtker, ZonePet *pstZonePet, AtkProcess *pstAtkProcess)
{
	//int i;
	int iHP;
	long long llTotalHurt;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	int iRealHurt, iHurt, iHeavyHit;
	ATKCHG *pstAtkChg = NULL;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	ZoneSkill *pstZoneSkill = pstAtkerInfo->pstZoneSkill;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	Player *pstOwner = pstZonePet->pstOwner;
	ROLETMPSYS *pstRoleTmpSysAtker = &pstAtker->stRoleData.MiscInfo.TmpSys;
	ITEMDEF *pstItemDefTmp = NULL;
	SKILLRESULT *pstSkillResultAddHurt = NULL;

	if (0 == (PET_STAT_FIGHT & pstZonePet->pstRolePet->StatFlag))
	{
		return -1;
	}
		
	stAtkeeInfo.stAtkee.iType = OBJ_PET;
	stAtkeeInfo.stAtkee.stObj.stZonePet = *pstZonePet;
	
	pstOneHit->ID = pstOwner->iMemID+(pstZonePet->pstPetFightShi->iXieDaiPos+1)*PET_ID_BASE;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;

	pstOwner->stOnline.iAtkerTarget = pstAtker->iMemID;

	if (pstSkillDef->FairySkill)
	{
		z_get_fairy_dyn(pstEnv, pstAtker, pstAtkerDyn, pstSkillDef,pstAtkProcess);
	}
	else
	{
		memcpy(pstAtkerDyn, &pstAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
		if(pstAtker->stOnline.iSkillAtkerID > 0)
		{
			Player *pstRealAtker = z_id_player(pstAppCtx, pstEnv, pstAtker->stOnline.iSkillAtkerID);
			if(pstRealAtker)
			{
				memcpy(pstAtkerDyn, &pstRealAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
			}
		}
		
		if (0 > pstAtkProcess->iAtkChgIdx)
		{
			pstAtkProcess->iAtkChgIdx = 0;
			memset(&pstAtkerDyn->stAtkChg, 0, sizeof(pstAtkerDyn->stAtkChg));

			pstAtkerDyn->stAtkChg.iAtkLevelMul = get_player_level_mul(pstEnv, pstAtker, LEVEL_MUL_ATK);
		}
	}

	if (pstAtkProcess->iAtkChgIdx >= 0)
	{
		pstAtkChg = &pstAtkerDyn->stAtkChg;
	}

	if( pstAtkChg == NULL)
	{
		return 0;
	}

	z_player_normal_atk_cacl(pstEnv, pstAtkChg,pstSkillDef ,pstAtker, pstAtkProcess);

	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;
	
	z_cacl_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	

	if (pstRoleTmpSysAtker->TmpItemID > 0)
	{
		pstItemDefTmp = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSysAtker);
	}

	if (0 == (pstItemDefTmp && pstItemDefTmp->ItemTmp.Type >= ITEM_TMP_ITEM))
	{
		//z_cacl_player_status_result(pstEnv, pstAtkerDyn, pstAtkChg, pstAtker);
		
		// 计算组队契约对攻防的影响
		z_cacl_player_qiyue_result(pstEnv, pstAtkerDyn, pstAtkChg, pstAtker);
	}
	
	if (pstSkillDef->StatusChg.StatusID > 0 && z_player_find_status(pstAtker, pstSkillDef->StatusChg.StatusID))
	{
		int iStatusChgHurt = 0;
		int iStatusChgHurtMul = 0;
		ZoneStatus *pstZoneStatus = NULL;

		iStatusChgHurt = pstSkillDef->StatusChg.Hurt;
		iStatusChgHurtMul = pstSkillDef->StatusChg.HurtMul;
		
		pstZoneStatus = z_player_find_status(pstAtker, pstSkillDef->StatusChg.StatusID);
		if(pstZoneStatus)
		{
			z_status_mul_skill_hurt(pstEnv,pstZoneStatus ,&iStatusChgHurt,&iStatusChgHurtMul);
		}
		/*for (i=0; i<MAX_ARM_ON; i++)
		{
			pstAtkerDyn->astAtkChg[i].stAtkerHurtChg.AtkerHurtAdd += iStatusChgHurt;
		}
		if (pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_ICE)
		{
			pstAtkChg->stAtkerHurtChg.AtkerIceHurtMul +=  pstSkillDef->StatusChg.HurtMul;
		}
		else if(pstAtkChg && pstSkillDef->MainAttr == SKILL_ATTR_PHSIC)
		{
			pstAtkChg->stAtkerHurtChg.AtkerPhsicHurtMul +=  pstSkillDef->StatusChg.HurtMul;
		}*/
		pstAtkChg->stAtkerHurtChg.AtkerHurtAdd += iStatusChgHurt;
		pstAtkChg->stAtkerHurtChg.AtkerHurtMul += iStatusChgHurtMul;
		

		pstAtkerDyn->stHitRateChg.nHeavyHitAdd += pstSkillDef->StatusChg.Heavy;
	}

	z_get_pet_dyn(pstEnv, pstOwner, pstAtkeeDyn, pstZonePet->pstRolePet->WID);
		
	pstHits->TotalHit++;
	player_hit_jiqi_full_add_state_target( pstEnv, pstAtkProcess, 
						&stAtkeeInfo, pstSkillDef, pstAtkerInfo->cJiqiNum);

	if(pstSkillDef->NoHurtSkill == 0)
	{
		//符文额外附加技能效果
		rune_addattr_skill(pstEnv, pstAtkProcess, &stAtkeeInfo);
	}

	//检查设置灰名状态
	atker_set_pktime(pstEnv, &pstAtkerInfo->stAtker, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);

	if (pstSkillDef->NoHurtSkill) //非伤害技能，只需要处理技能效果
	{
		z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//攻击计算副本次数了
	map_pworld_chg_limitflag(pstAppCtx, pstEnv, pstAtker);
		
	llTotalHurt = 0;
	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;

	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += pstZoneSkill->stSkillMul.HeavyHitRateMul;//天赋的暴击修正
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		//暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0/(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}	
		
		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//天赋 普通攻击触发对自身的buff
	//genius_atk_random_add_status(pstAppCtx, pstEnv, pstAtker, pstSkillDef->SkillType);
	z_del_result_by_heavyhit(pstEnv->pstAppCtx, pstEnv,pstAtker,NULL,iHeavyHit);

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;
	z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	z_do_extended_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo,
		                   pstAtkChg);

	pstSkillResultAddHurt = z_get_skill_result(pstSkillDef,RESULT_ADDHURT_BY_TARGETSTATUS);
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		
		pstAtkProcess->iContinueCount--;
				
		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstZonePet->pstRolePet->HP;
			
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}

		if (iHurt <= 0)
		{
			continue;
		}
				
		llTotalHurt += iHurt;

		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;
		pstOneHit->TotalHurt++;

		if( pstSkillResultAddHurt &&
			RAND1(100) <= pstSkillResultAddHurt->SkillResultVal1)
		{
			ZoneStatus *pstZoneStatusTmp = NULL;
			if(pstSkillResultAddHurt->SkillResultVal3 ==0)
			{
				pstZoneStatusTmp = z_pet_find_status(pstEnv,  pstOwner, pstZonePet->pstRolePet,pstSkillResultAddHurt->SkillResultVal2);
			}
			else 
			{
				pstZoneStatusTmp = z_pet_find_status(pstEnv,  pstOwner, pstZonePet->pstRolePet,pstSkillResultAddHurt->SkillResultVal2);
			} 
			if(pstZoneStatusTmp)
			{
				llTotalHurt += iHurt*pstSkillResultAddHurt->SkillResultVal4/100.0;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
				pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt*pstSkillResultAddHurt->SkillResultVal4/100.0;
				pstOneHit->TotalHurt++;
			}
		}
				
		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		if (llTotalHurt >= (iHP + pstZonePet->pstRolePet->HP))
		{
			pstAtkProcess->cDeadHit = 1;
			break;
		}
	}while (pstAtkProcess->iContinueCount > 0);

	if (!pstAtkProcess->cDeadHit && pstAddAtkInfo->iMainAttrHurt != 0) //处理追招等附加伤害
	{
		z_add_hurt(pstAtkProcess, pstOneHit);
	}
	
	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0); 
}

int get_real_self_poschg(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef, int iTargetID, int iChgDist)
{
	int iRealDist = iChgDist;
	
	//有攻击目标的技能
	if (TARGET_RANGE_NORMAL == pstSkillDef->TargetRange ||
		pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || 
		pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE)
	{
		Monster *pstMon;
		pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, iTargetID);
		if(pstMon)
		{
			if (iRealDist <= 0)
			{
				iRealDist -= pstMon->unRadius;
			}
			else
			{
				iRealDist += pstMon->unRadius;
			}
		}
	}

	return iRealDist;
}


int z_player_skill_touch_status(ZONESVRENV* pstEnv, Player *pstPlayer,ZoneSkill *pstZoneSkill,SKILLDEF *pstSkillDef)
{
	int i;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;

	pstZoneSkill->stSkillData.SkillUseNum++;

	for (i=0; i<pstDyn->iGnUseSkill; i++)
	{
		if (	pstDyn->astGnUseSkill[i].iSkillID == pstSkillDef->SkillID &&
			pstZoneSkill->stSkillData.SkillUseNum == pstDyn->astGnUseSkill[i].iTouchNum)
		{
			z_player_apply_status( 	pstEnv->pstAppCtx, pstEnv, pstPlayer, pstDyn->astGnUseSkill[i].iStatusID,
								    	pstDyn->astGnUseSkill[i].iLevel, pstDyn->astGnUseSkill[i].iTime, 1, 0	);
			pstZoneSkill->stSkillData.SkillUseNum = 0;
			break;
		}
	}
	return 0;
}

int player_beihouling(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, const CSHITS *pstHits)
{
	int i;
	int iRate = 0;
	BHLingOL *pstBhlingOl = &pstPlayer->stOnline.stBhlingOl;
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef;
	int iOldSkillSeq;
	TFRAMEHEAD stFrameHead;
	CSPKG stPkg;
	struct timeval stTmp;
	struct timeval stTmpCommCool;
	CSSKILLATK *pstSkillAtk = &stPkg.Body.SkillAtk;
	
	if (!(pstPlayer->stRoleData.MiscInfo.Flag &FAIRY_BEIHOULING))
		return -1;

	if (pstBhlingOl->bBHlingRate <= 0)
		return -1;

	iRate = RAND1(10000);
	if (pstBhlingOl->bBHlingRate*100 > iRate)
		return -1;

	pstBhlingOl->bFlag = 1; //防止无限递归
	

	

	z_add_skill(pstAppCtx, pstEnv, pstPlayer, NULL, pstBhlingOl->iSkillID, pstBhlingOl->bSkillLvl, 0);
	pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstBhlingOl->iSkillID);
	if (NULL == pstZoneSkill)
		goto final;
	
	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
	if (NULL == pstSkillDef)
		goto final;
	
	iOldSkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq;
	stFrameHead.TimeStamp.TimeVal.sec = pstEnv->pstAppCtx->stCurr.tv_sec;
	stFrameHead.TimeStamp.TimeVal.usec = pstEnv->pstAppCtx->stCurr.tv_usec;
	
	pstSkillAtk->SkillID = pstBhlingOl->iSkillID;
	pstSkillAtk->SkillSeq = iOldSkillSeq+1;
	pstSkillAtk->RolePos = pstPlayer->stRoleData.Pos;
	pstSkillAtk->AtkPos = pstPlayer->stRoleData.Pos;

	pstSkillAtk->SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstSkillAtk->SvrTime.BenchTimeUSec = pstEnv->pstAppCtx->stCurr.tv_usec;
	//pstSkillAtk->FittingsIdx = 0;

	for (i=0; i<pstHits->TotalHit; i++)
	{
		if (pstHits->Hits[i].DeadFlag == 0)
		{
			break;
		}
	}

	if (i >= pstHits->TotalHit)
	{
		goto final;
	}

	pstSkillAtk->TargetNum = 1;
	pstSkillAtk->TargetIDs[0] = pstHits->Hits[i].ID;

	stTmp = pstPlayer->stOnline.stLastCltTime;
	stTmpCommCool= pstPlayer->stOnline.stSkillList.stCommCoolEnd;
		
	player_atk(pstAppCtx, pstEnv, pstPlayer, &stFrameHead, &stPkg);
	pstPlayer->stOnline.stLastCltTime = stTmp;
	pstPlayer->stOnline.stSkillList.stCommCoolEnd = stTmpCommCool;


	pstPlayer->stOnline.stAtkInfo.iSkillSeq = iOldSkillSeq;
	pstPlayer->stOnline.iSkillAtkerID = 0;


final:
	z_del_skill(pstAppCtx, pstEnv, pstPlayer, pstBhlingOl->iSkillID, 0);
	pstBhlingOl->bFlag = 0;
	return 0;
	
}
int z_player_get_ContinueNum(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,SKILLDEF *pstSkillDef,AtkProcess *pstAtkProc)
{
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	int iLeftConntinueNum = 0;

	 
	if (pstAtkProc->pstSkillArmChg)
	{
		pstAtkProc->bContinueNum = pstAtkProc->pstSkillArmChg->ContinueNum;
	}
	else if (pstSkillDef->StatusChg.StatusID > 0 && z_player_find_status(pstPlayer, pstSkillDef->StatusChg.StatusID))
	{
		pstAtkProc->bContinueNum = pstSkillDef->StatusChg.ContinueNum;
	}
	else if(pstSkillDef->ContinueNum > 0)
	{
		pstAtkProc->bContinueNum = pstSkillDef->ContinueNum;
	}

	// 获取抢手的左手频数
	iLeftConntinueNum = package_get_guner_wear_left_continuenum(pstAppCtx, pstEnv, pstPlayer,pstSkillDef);
	if(-1 !=  iLeftConntinueNum)
	{
		pstAtkProc->bContinueNum  = iLeftConntinueNum;
	}
	

	//把集气格子表现为频数
	if( pstSkillDef->JiqiChgFlag==1)
	{
		pstAtkProc->bContinueNum = ceil(3 * (pstAtkInfo->iJiqiTime/(pstSkillDef->JiqiTime*100.0)));
		if(pstAtkProc->bContinueNum < 1)
		{
			pstAtkProc->bContinueNum =1;
		}
		else if(pstAtkProc->bContinueNum > 3)
		{
			pstAtkProc->bContinueNum = 3;
		}
	}
	
	if(pstAtkProc->bContinueNum < 1)
	{
		pstAtkProc->bContinueNum = 1;
	}
	
	return 0;
}


// 计算普通攻击时的加成 
static int z_player_normal_atk_cacl(ZONESVRENV* pstEnv, ATKCHG *pstAtkChg,SKILLDEF *pstSkillDef ,
										Player *pstPlayer, AtkProcess *pstAtkProcess)
{
	STATUSRESULT *pstResult;

	calc_arm_skill_add(pstEnv, pstAtkProcess, pstSkillDef->SkillID);
	
	if (pstSkillDef->SkillType != SKILL_TYPE_NORMAL)
	{
		return -1;
	}

	pstResult = player_status_result(pstEnv, pstPlayer,STATUS_RESULT_NORMAL_ELEMENT_ATK);
	if (pstResult)
	{
		int iVal;
		
		if (	pstResult->ResultVal3-pstResult->ResultVal2 < 0 ||
			pstResult->ResultVal2 <= 0)
			return -1;
		
		iVal = pstResult->ResultVal2+ RAND1(pstResult->ResultVal3-pstResult->ResultVal2);
		
		if (pstResult->ResultVal1 == FAIRY_ATTR_FIRE)
		{
			pstAtkChg->unFireBaseArmAtk += iVal;
		}
		else if (pstResult->ResultVal1 == FAIRY_ATTR_LIGHT)
		{
			pstAtkChg->unLightBaseArmAtk += iVal;
		}
		else if (pstResult->ResultVal1 == FAIRY_ATTR_NIGHT)
		{
			pstAtkChg->unNightBaseArmAtk += iVal;
		}
		else if (pstResult->ResultVal1 == FAIRY_ATTR_THUNDER)
		{
			pstAtkChg->unThunderBaseArmAtk += iVal;
		}
		else if (pstResult->ResultVal1 == FAIRY_ATTR_WATER)
		{
			pstAtkChg->unWaterBaseArmAtk += iVal;
		}
		else
		{
			return -1;
		}
		
	}
	return 0;
}

int z_player_hit_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef, 
						SKILLDEF *pstOriginDef, ZoneObjList *pstList,  CSSKILLATK *pstHit, int iAtkChgIdx)
{
	int i;
	int j;
	int iMapID = pstPlayer->stRoleData.Map;
	AtkProcess stAtkProc;
	ZoneAni *pstAni=NULL;
	ZoneOneDead *pstDead;
	MONSTERDEF * pstMonDef;
	SKILLRESULT *pstSkillResult;
	CSPKG *pstPkg = &pstEnv->stCSRes;
	CSHITS *pstHits = &stAtkProc.stHits;
	ZoneDeadList *pstZoneDeadList = &stAtkProc.stDeadList;
	int iDeadAtk = 0;
	int iPetDead = 0;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	LightningAtkInfo *pstLightningAtkInfo=NULL;
	DebuffObjList stDebuffObjList; 
	int iTotalDebuff = 0;
	int iSeqDebff = 0;

	pstLightningAtkInfo = &pstPlayer->stOnline.stLightningAtkInfo[pstAtkInfo->iAtkLightningIdx];
	
	memset(&stAtkProc, 0, sizeof(stAtkProc));
	memset(&stDebuffObjList,0,sizeof(stDebuffObjList));

	pstHits->SkillID = pstSkillDef->SkillID;
	pstHits->SkillSeq = pstHit->SkillSeq;

	stAtkProc.stAtkerInfo.stAtker.iType = OBJ_PLAYER;
	stAtkProc.stAtkerInfo.stAtker.stObj.pstPlayer = pstPlayer;
	stAtkProc.stAtkerInfo.pstZoneSkill = pstZoneSkill;
	stAtkProc.stAtkerInfo.pstSkillDef = pstSkillDef;
	stAtkProc.stAtkerInfo.pstOriginDef = pstOriginDef;
	stAtkProc.stAtkerInfo.stAtkPos = pstHit->AtkPos;
	stAtkProc.stAtkerInfo.stRolePos = pstHit->RolePos;
	stAtkProc.stAtkerInfo.SvrTime = pstHit->SvrTime;
	stAtkProc.stAtkerInfo.iPilot = pstAtkInfo->unPilot;
	stAtkProc.stAtkerInfo.cJiqiNum = pstHit->CollectPowerNum;

	if(pstPlayer->stOnline.iSkillAtkerID)
	{
		Player *pstTmpPlayer;

		pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.iSkillAtkerID);
		if(pstTmpPlayer && pstTmpPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			stAtkProc.stAtkerInfo.pstStatusSkillAtker = pstTmpPlayer;
		}
	}

	pstSkillResult = z_skill_get_result(pstSkillDef, RESULT_SELF_POSCHG);
	if (pstSkillResult && pstSkillDef->AtkType == SKILL_ATK_TYPE_NORMAL)
	{
		RESPOS stPos;
		MapIndex *pstMapIdx;
		ZONEMAPINST *pstMapInst;
		ZONEIDX stIdx;

		stIdx.iID = pstPlayer->stRoleData.Map;
		stIdx.iPos = pstPlayer->stOnline.iMapPos;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (pstMapInst)
		{
			int iDist;
			
			iDist = get_real_self_poschg(pstEnv, pstSkillDef, pstHit->TargetIDs[0], pstSkillResult->SkillResultVal1);

			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
			if (0 == z_skill_result_poschg(pstEnv, pstMapInst, pstMapIdx, pstSkillDef, iDist, 
				pstSkillResult, pstPlayer, &pstHit->AtkPos, &pstHit->RolePos, &stPos))
			{
				stAtkProc.stAtkerInfo.SelfPosChg = stPos;
				stAtkProc.stAtkerInfo.SelfPosChgFlag = 1;
			}
		}
	}

	//获取武器修正技能数值
	stAtkProc.pstSkillArmChg = NULL;
	if (iAtkChgIdx >= 0)
	{
		int j;
		int iFind;
		ITEMDEF *pstItemDef;
		ROLEWEAR *pstRoleWear = &pstPlayer->stRoleData.Wear;

		for(i=0;i<pstRoleWear->ArmNum;i++)
		{
			pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)&pstRoleWear->RoleArms[i]);
			if(!pstItemDef)
			{
				continue;
			}

			iFind = 0;
			for (j=0; j<MAX_SKILL_ARM_CHG; j++)
			{
				if (0 == pstSkillDef->SkillArmChg[j].ArmType)
				{
					break;
				}

				if (pstItemDef->NumType == pstSkillDef->SkillArmChg[j].ArmType)
				{
					stAtkProc.pstSkillArmChg = &pstSkillDef->SkillArmChg[j];
					iFind = 1;
					break;
				}
			}
			
			if(iFind)
			{
				break;
			}
		}
	}

	//获取攻击频数
	z_player_get_ContinueNum(pstAppCtx, pstEnv, pstPlayer,pstSkillDef,&stAtkProc);
	
	//一次攻击或引导攻击算一次
	if(pstPlayer->stOnline.stAtkInfo.unPilot == 0)
	{
		player_collectskill_add_slot(pstEnv, pstPlayer,0,COLLECTSKILL_ADDTYPE_AKER);	
	}
	
	for (i=0; i<pstList->iObj; i++)
	{
		stAtkProc.iAtkChgIdx = iAtkChgIdx;
		
		pstAni = &pstList->astObj[i];

		switch(pstAni->iType)
		{
			case OBJ_PLAYER:
				z_player_hit_player(pstAppCtx, pstEnv, pstPlayer, pstAni->stObj.pstPlayer, &stAtkProc);
				z_player_byatk_clear_status(pstEnv, pstAni->stObj.pstPlayer);
				//判断战斗状态
				switch(pstSkillDef->FightStat)
				{
					case SKILL_FIGHT_STAT_BOTH:
						SET_PLAYER_FIGHT(pstEnv, pstAni->stObj.pstPlayer, pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						break;
					case SKILL_FIGHT_STAT_SINGLE:
						if (pstAni->stObj.pstPlayer->stOnline.State & CS_STAT_FIGHT )
						{
							SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						}
						break;
					default:
						break;
				}

				if( pstSkillDef->LightningLinkNum > 0 && 
					pstLightningAtkInfo->iJumpNum > 0)
				{
					pstLightningAtkInfo->iJumpNum--;
					pstLightningAtkInfo->arrJumpedPos[pstLightningAtkInfo->iJumpNum] = pstAni->stObj.pstPlayer->stRoleData.Pos;
					pstLightningAtkInfo->iMemID[pstLightningAtkInfo->iJumpNum] = pstAni->stObj.pstPlayer->iMemID;
				}

				if( pstAni->stObj.pstPlayer->stOnline.stStatusList.bBadStatusNum > 0 &&
					stDebuffObjList.iObj < MAX_SKILL_TARGET)
				{
					stDebuffObjList.astObj[stDebuffObjList.iObj].iDebuffNum = pstAni->stObj.pstPlayer->stOnline.stStatusList.bBadStatusNum;
					stDebuffObjList.astObj[stDebuffObjList.iObj].stObj = *pstAni;
					stDebuffObjList.iObj++;
					iTotalDebuff += pstAni->stObj.pstPlayer->stOnline.stStatusList.bBadStatusNum;
				}
				break;
			case OBJ_MONSTER:
				z_player_hit_mon(pstAppCtx, pstEnv, pstPlayer, pstAni->stObj.pstMon, &stAtkProc);
				if( pstSkillDef->LightningLinkNum > 0 && 
					pstLightningAtkInfo->iJumpNum > 0)
				{
					pstLightningAtkInfo->iJumpNum--;
					pstLightningAtkInfo->arrJumpedPos[pstLightningAtkInfo->iJumpNum] = pstAni->stObj.pstMon->stCurrPos;
					pstLightningAtkInfo->iMemID[pstLightningAtkInfo->iJumpNum] = pstAni->stObj.pstMon->iID;
				}
				
				if( pstAni->stObj.pstMon->stStatusList.bBadStatusNum > 0 &&
					stDebuffObjList.iObj < MAX_SKILL_TARGET)
				{
					stDebuffObjList.astObj[stDebuffObjList.iObj].iDebuffNum = pstAni->stObj.pstMon->stStatusList.bBadStatusNum;
					stDebuffObjList.astObj[stDebuffObjList.iObj].stObj = *pstAni;
					stDebuffObjList.iObj++;
					iTotalDebuff += pstAni->stObj.pstMon->stStatusList.bBadStatusNum;
				}
				break;
			case OBJ_NPC:
				z_player_hit_mon(pstAppCtx, pstEnv, pstPlayer, &pstAni->stObj.pstNpc->stMon, &stAtkProc);
				break;
			case OBJ_PET:
				z_player_hit_pet(pstAppCtx, pstEnv, pstPlayer, &pstAni->stObj.stZonePet, &stAtkProc);
				switch(pstSkillDef->FightStat)
				{
					case SKILL_FIGHT_STAT_BOTH:
						SET_PLAYER_FIGHT(pstEnv, pstAni->stObj.stZonePet.pstOwner, pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						break;
					case SKILL_FIGHT_STAT_SINGLE:
						if (pstAni->stObj.pstPlayer->stOnline.State & CS_STAT_FIGHT )
						{
							SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						}
						break;
					default:
						break;
				}
				if( pstSkillDef->LightningLinkNum > 0 && 
					pstLightningAtkInfo->iJumpNum > 0)
				{
					pstLightningAtkInfo->iJumpNum--;
					pstLightningAtkInfo->arrJumpedPos[pstLightningAtkInfo->iJumpNum] = pstAni->stObj.stZonePet.pstOwner->stRoleData.Pos;
					//pstLightningAtkInfo->iMemID[pstLightningAtkInfo->iJumpNum] = pstAni->stObj.stZonePet.pstOwner->iMemID;
					pstLightningAtkInfo->iMemID[pstLightningAtkInfo->iJumpNum] = 
							pstAni->stObj.stZonePet.pstOwner->iMemID+(pstAni->stObj.stZonePet.pstPetFightShi->iXieDaiPos+1)*PET_ID_BASE;
				}
				break;
			default:
				break;
		}
		
		

	}

	//无解的需求
	pstSkillResult = z_skill_get_result(pstSkillDef, RESULT_MUL_TARGET_DEL_DEBUFF);
	if(pstSkillResult && stDebuffObjList.iObj > 0 )
	{
		//开始随机
		for( i=0;i<pstSkillResult->SkillResultVal1;i++)
		{
			SKILLRESULT stResult;
			memset(&stResult,0,sizeof(stResult));
			
			iSeqDebff = RAND1(stDebuffObjList.iObj);
			pstAni = &stDebuffObjList.astObj[iSeqDebff].stObj;
			if(OBJ_PLAYER == pstAni->iType)
			{
				for(j=0;j<pstHits->TotalHit;j++)
				{
					if(pstHits->Hits[j].ID == pstAni->stObj.pstPlayer->iMemID)
					{
						stResult.SkillResultVal1 = 1;
						z_player_del_random_one_debuff(pstAppCtx, pstEnv, pstAni->stObj.pstPlayer,&stResult,&pstHits->Hits[j]);
						if(pstHits->Hits[j].StatusFlag)
						{
							z_get_player_csstatusinfo(pstEnv->pstAppCtx, &pstHits->Hits[j].Status, &pstAni->stObj.pstPlayer->stOnline.stStatusList);
						}
					}
				}
			}
			else if(OBJ_MONSTER == pstAni->iType)
			{
				for(j=0;j<pstHits->TotalHit;j++)
				{
					if(pstHits->Hits[j].ID == pstAni->stObj.pstMon->iID)
					{
						stResult.SkillResultVal1 = 1;
						z_mon_del_random_one_debuff(pstAppCtx, pstEnv, pstAni->stObj.pstMon,&stResult,&pstHits->Hits[j]);
						if(pstHits->Hits[j].StatusFlag)
						{
							z_get_mon_csstatusinfo(pstEnv->pstAppCtx, &pstHits->Hits[j].Status, &pstAni->stObj.pstMon->stStatusList);
						}
					}
				}
			}
			stDebuffObjList.astObj[iSeqDebff].iDebuffNum--;
			if(stDebuffObjList.astObj[iSeqDebff].iDebuffNum <=0)
			{
				stDebuffObjList.iObj--;
				if(stDebuffObjList.iObj<=0)
				{
					break;
				}
				else if(iSeqDebff < stDebuffObjList.iObj)
				{
					stDebuffObjList.astObj[iSeqDebff] = stDebuffObjList.astObj[stDebuffObjList.iObj];
				}
			}
			
		}
	}
	
	if (pstList->iObj > 0 && pstSkillDef->FightStat == SKILL_FIGHT_STAT_BOTH)
	{
		SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
	}

	z_do_skill_result_add_hp_by_hurt_skill(pstAppCtx, pstEnv, pstPlayer, &stAtkProc);

	make_hit_res(pstEnv, &stAtkProc, pstHits->Hits, pstHits->TotalHit, pstPkg);
	if(iMapID == pstPlayer->stRoleData.Map)
	{
		z_send_player_hit_res(pstAppCtx, pstEnv, pstPlayer, &pstHit->AtkPos, pstPkg);
	}
		
	//计算反射技能
	if(!pstZoneDeadList->iSelf && stAtkProc.stReflectedDamage.iNum > 0)
	{
		z_player_be_reflected(pstEnv, pstPlayer, &stAtkProc);
	}

	//宠物死亡列表处理
	iPetDead = z_pet_beatk_die(pstEnv, pstHits);

	//死亡列表处理
	for (i=0; i<pstZoneDeadList->iCount; i++)
	{
		pstDead = &pstZoneDeadList->astDeads[i];
		switch(pstDead->stAtkee.iType)
		{
			case OBJ_PLAYER:
				player_die(pstAppCtx, pstEnv, pstDead->stAtkee.stObj.pstPlayer, &pstDead->stAtker, 0);
				break;
			case OBJ_MONSTER:
				pstMonDef = z_get_mon_def(pstEnv, pstDead->stAtkee.stObj.pstMon);
				if (pstMonDef)
				{
					z_mon_die(pstAppCtx, pstEnv, pstDead->stAtkee.stObj.pstMon, pstMonDef, &pstDead->stAtker, 0, 1);
				}
				break;
			default:
				break;
		}

		player_kill_restore_hp_mp(pstEnv, &stAtkProc.stAtkerInfo, &pstDead->stAtkee);
	}

	if (pstZoneDeadList->iCount > 0 || iPetDead > 0)
	{
		// 击杀触发状态
		//genius_add_status_by_player_kill(pstEnv, pstPlayer);
		iDeadAtk = 1;
	}

	//攻击者的技能消耗结算
	z_skill_consume(pstAppCtx, pstEnv, pstPlayer, &stAtkProc.stAtkerInfo.stResourceConsume, 
					pstZoneSkill,iDeadAtk, 0);

	// 指定ID技能使用一定次数后触发对自身的BUFF
	z_player_skill_touch_status(pstEnv,pstPlayer,pstZoneSkill,pstSkillDef);

	if (pstZoneDeadList->iSelf)
	{
		pstDead = &pstZoneDeadList->stSelfDead;
		player_die(pstAppCtx, pstEnv, pstPlayer, &pstDead->stAtker, 0);
	}
	else 
	{
		if (pstPlayer->stOnline.bTotemNum > 0 && pstList->iObj > 0)
		{
			player_target_totem(pstEnv, pstPlayer, &pstList->astObj[0]);
		}

		arm_calc_teshu_buff(pstEnv,pstPlayer);
	}

	//要在反击类攻击的前面结束
	if(stAtkProc.cQieCuoEnd)
	{
		Player *pstQiecuoer;
		int fWin = 1;

		if(stAtkProc.cQieCuoEnd > 1)
		{
			fWin = 0;
		}

		pstQiecuoer = z_id_player(pstAppCtx, pstEnv, pstPlayer->stOnline.stQiecuoInfo.iMemID);
		if(pstQiecuoer)
		{
			z_player_qiecuo_end(pstEnv, pstQiecuoer, !fWin);
		}
		
		z_player_qiecuo_end(pstEnv, pstPlayer, fWin);	
	}
	

	//反击
	if(!stAtkProc.cQieCuoEnd && !(pstPlayer->stOnline.State & CS_STAT_DEAD))
	{
		for (i=0; i<stAtkProc.stBeatBackList.iBeatBackNum; i++)
		{
			z_beat_beak(pstAppCtx, pstEnv, &stAtkProc.stBeatBackList.astBeatBackInfo[i], &stAtkProc.stAtkerInfo.stAtker);
			if (pstPlayer->stOnline.State & CS_STAT_DEAD) break;
		}
	}

	//处理怪物ai 触发
	for (i=0; i<stAtkProc.stAiList.iCount; i++)
	{
		mon_ai_stat(pstEnv, stAtkProc.stAiList.apstMon[i], &stAtkProc.stAiList.astMonAi[i]);
	}

	if (!stAtkProc.cQieCuoEnd && !pstPlayer->stOnline.stBhlingOl.bFlag)
	{
		// 触发精灵背后灵
		player_beihouling(pstAppCtx,pstEnv, pstPlayer, pstHits);
	}
	
	return 0;
}

int player_hit_jiqi_full_add_state_target( ZONESVRENV* pstEnv,AtkProcess *pstAtkProcess, 
						AtkeeInfo *pstAtkeeInfo, SKILLDEF *pstSkillDef, int iJiqiNum)
{
	SKILLRESULT *pstResult;

	if (pstSkillDef->JiqiMul <= 0 || iJiqiNum < pstSkillDef->JiqiMul)
	{
		return 0;
	}

	pstResult = z_get_skill_result( pstSkillDef,RESULT_JIQI_FULL_ADD_STAT);
	if (!pstResult)
	{
		return 0;
	}

	if (!pstResult->SkillResultVal4)
		return 0;
	z_calc_apply_status(pstEnv->pstAppCtx, pstEnv, pstAtkProcess,
			                     pstAtkeeInfo, pstResult->SkillResultVal1, 
				              pstResult->SkillResultVal2,pstResult->SkillResultVal3);
	return 0;
}

int player_hit_jiqi_full_add_self(ZONESVRENV* pstEnv, Player *pstPlayer, 
									SKILLDEF *pstSkillDef, int iJiqiNum)
{
	SKILLRESULT *pstResult;

	if (pstSkillDef->JiqiMul <= 0 || iJiqiNum < pstSkillDef->JiqiMul)
	{
		return 0;
	}

	pstResult = z_get_skill_result( pstSkillDef,RESULT_JIQI_FULL_ADD_STAT);
	if (!pstResult)
	{
		return 0;
	}

	if (!pstResult->SkillResultVal4)
	{
		z_player_apply_status(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstResult->SkillResultVal1,
				pstResult->SkillResultVal2, pstResult->SkillResultVal3, 1, pstPlayer->iMemID);
	}
	
	return 0;
}

int player_hit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			   TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg, ZoneSkill *pstZoneSkill, 
			   SKILLDEF *pstSkillDef, SKILLDEF *pstOriginDef, int iAtkChgIdx)
{
	int iMapID = pstPlayer->stRoleData.Map;
	SKILLRESULT *pstSkillResult;
	ZoneObjList stList;
	struct timeval stCltTime;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	CSSKILLATK *pstHit = &pstCsPkg->Body.SkillHit;
	CSPKG *pstPkgRes = &pstEnv->stCSRes;
	CSACTIONRES *pstActionRes = &pstPkgRes->Body.ActionRes;
	int i = 0;
	UNUSED(pstFrameHead);

	memset(&stCltTime,0,sizeof(stCltTime));
	stCltTime.tv_sec = pstHit->SvrTime.BenchTimeSec;
       stCltTime.tv_usec = pstHit->SvrTime.BenchTimeUSec;


	//计算攻击列表
	if (0 > z_player_atklist(pstAppCtx, pstEnv, pstPlayer, pstSkillDef, pstHit, &stList, &stCltTime))
	{
		z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstAtkInfo->unSkillID);
		return -1;
	}

	if (stList.iObj > 0)
	{
		if (0 == pstSkillDef->NoHurtSkill)
		{
			switch(stList.astObj[0].iType)
			{
				case OBJ_PLAYER:
					pstPlayer->stOnline.iHitTarget = stList.astObj[0].stObj.pstPlayer->iMemID;
					break;
				case OBJ_MONSTER:
					pstPlayer->stOnline.iHitTarget = stList.astObj[0].stObj.pstMon->iID;
					break;
				case OBJ_PET:
					pstPlayer->stOnline.iHitTarget = stList.astObj[0].stObj.stZonePet.pstOwner->iMemID + 
													(stList.astObj[0].stObj.stZonePet.pstPetFightShi->iXieDaiPos+1)*PET_ID_BASE;
					break;
				case OBJ_NPC:
					pstPlayer->stOnline.iHitTarget = stList.astObj[0].stObj.pstNpc->stMon.iID;
					break;
				default:
					break;
			}
		}

		z_player_hit_notify(pstAppCtx, pstEnv, pstPlayer,  pstZoneSkill, 
							pstSkillDef, pstOriginDef, &stList, pstHit, iAtkChgIdx );
		player_hit_jiqi_full_add_self(pstEnv, pstPlayer, pstSkillDef, pstHit->CollectPowerNum);
	}
	else if( (pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_LIGHTNING) ||
		 	 (pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_CLT_ATK) )
	{
		return -1;
	}
	else
	{
		memset(pstActionRes,0,sizeof(CSACTIONRES));
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->ActionID = ACTION_HIT;
		pstActionRes->Data.Hit.SkillID = pstOriginDef->SkillID;
		pstActionRes->Data.Hit.Level = pstOriginDef->Level;
		pstActionRes->Data.Hit.AtkPos = pstHit->AtkPos;
		pstActionRes->Data.Hit.SkillSeq = pstHit->SkillSeq;
		pstActionRes->Data.Hit.RuneTransferedID = pstSkillDef->SkillID;
		pstActionRes->Data.Hit.FromID = pstPlayer->iMemID;
		pstActionRes->Data.Hit.FromPos = pstHit->RolePos;

		pstSkillResult = z_skill_get_result(pstSkillDef, RESULT_SELF_POSCHG);
		if (pstSkillResult && pstSkillDef->AtkType == SKILL_ATK_TYPE_NORMAL)
		{
			RESPOS stPos;
			MapIndex *pstMapIdx;
			ZONEMAPINST *pstMapInst;
			ZONEIDX stIdx;

			stIdx.iID = pstPlayer->stRoleData.Map;
			stIdx.iPos = pstPlayer->stOnline.iMapPos;
			pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
			if (pstMapInst)
			{
				pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
				if (0 == z_skill_result_poschg(pstEnv, pstMapInst, pstMapIdx, pstSkillDef, pstSkillResult->SkillResultVal1, 
												pstSkillResult, pstPlayer, &pstHit->AtkPos, &pstHit->RolePos, &stPos))
				{
					pstActionRes->Data.Hit.SelfPosChgFlag = 1;
					pstActionRes->Data.Hit.SelfPosChg = stPos;
				}
			}
		}

		if(iMapID == pstPlayer->stRoleData.Map)
		{
			if (pstSkillResult || (SKILL_RANGE_SIGLE != pstSkillDef->MainRange.RangeType && SKILL_RANGE_NONE != pstSkillDef->MainRange.RangeType))
			{
				Z_CSHEAD_INIT(&pstPkgRes->Head, ACTION_RES);
				z_send_player_hit_res(pstAppCtx, pstEnv, pstPlayer, &pstHit->AtkPos, pstPkgRes);
			}
		}

		player_hit_jiqi_full_add_self(pstEnv, pstPlayer, pstSkillDef, pstHit->CollectPowerNum);
	}

	//特殊处理对自身施加状态
	/*pstSkillResult = z_skill_get_result(pstSkillDef, RESULT_SELF_STATUS);
	if (pstSkillResult && ((int)(RAND1(10000)) < pstSkillResult->SkillResultVal1*100.0))
	{
		if ( pstSkillDef->SkillSwitch == 1 )  
		{
			if ( z_player_find_status(pstPlayer, pstSkillResult->SkillResultVal2) )
			{
				z_player_del_status(pstAppCtx, pstEnv, pstPlayer, pstSkillResult->SkillResultVal2, 1, 0);
			}
			else
			{
				z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, pstSkillResult->SkillResultVal2, 

										pstSkillResult->SkillResultVal3, pstSkillResult->SkillResultVal4, 1, pstPlayer->iMemID);
			}
		}
		else
		{
			z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, pstSkillResult->SkillResultVal2, 

										pstSkillResult->SkillResultVal3, pstSkillResult->SkillResultVal4, 1, pstPlayer->iMemID);
		}
	}*/

	for (i=0; i<MAX_SKILL_RESULT; i++) 
	{
		if (0 == pstSkillDef->SkillResult[i].SkillResultID)
		{
			break;
		}

		if (RESULT_SELF_STATUS == pstSkillDef->SkillResult[i].SkillResultID)
		{
			pstSkillResult = &pstSkillDef->SkillResult[i];
			if (pstSkillResult && ((int)(RAND1(10000)) < pstSkillResult->SkillResultVal1*100.0))
			{
				if ( pstSkillDef->SkillSwitch == 1 )  
				{
					if ( z_player_find_status(pstPlayer, pstSkillResult->SkillResultVal2) )
					{
						z_player_del_status(pstAppCtx, pstEnv, pstPlayer, pstSkillResult->SkillResultVal2, 1, 0);
					}
					else
					{
						z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, pstSkillResult->SkillResultVal2, 

												pstSkillResult->SkillResultVal3, pstSkillResult->SkillResultVal4, 1, pstPlayer->iMemID);
					}
				}
				else
				{
					z_player_apply_status(pstAppCtx, pstEnv, pstPlayer, pstSkillResult->SkillResultVal2, 

												pstSkillResult->SkillResultVal3, pstSkillResult->SkillResultVal4, 1, pstPlayer->iMemID);
				}
			}
		}
	}
		
	// 处理附魔效果
	z_final_extend_attr(pstEnv, pstPlayer, iAtkChgIdx);

	if (pstSkillDef->WushuangFlag > 0)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_WUSHUANG;
	}

	if (0 == pstSkillDef->PilotNum || (pstSkillDef->PilotNum > 0 && pstAtkInfo->cPilotEnd))
	{
		if(pstAtkInfo->unSkillID == pstOriginDef->SkillID)
		{
			z_skill_clear(pstPlayer);
		}
	}

	//处理绑定技能
	if (BINDSKILL_TYPE_STATUS == pstSkillDef->BindSkill)
	{
		ZoneStatus *pstZoneStatus;
		pstZoneStatus = z_find_bindskill_status(pstEnv, pstPlayer, pstSkillDef->SkillID);
		if (pstZoneStatus && pstZoneStatus->bBindSkillTrig == STATUS_TRIG_START)
		{
			z_del_skill(pstAppCtx, pstEnv, pstPlayer, pstSkillDef->SkillID, 1);
		}
	}
	else if (BINDSKILL_TYPE_TMP == pstSkillDef->BindSkill)
	{
		ITEMDEF *pstItemDef;
		ROLETMPSYS *pstRoleTmpSys;
	
		pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (pstItemDef && (ITEM_TMP_ITEM == pstItemDef->ItemTmp.Type || ITEM_TMP_ARM == pstItemDef->ItemTmp.Type))
		{
			if (pstZoneSkill->stSkillData.CollectNum <= 0)
			{
				tmp_item_del(pstEnv, pstPlayer);
			}
		}
	}

	
	return 0;
}

int z_mon_get_atk_pet(ZONESVRENV* pstEnv, SKILLDEF *pstSkillDef, Player *pstPlayer, Monster *pstMon, ZoneObjList *pstZoneObjList)
{
	ZoneAni *pstAni;
	ROLEPET *pstRolePet;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

	if(pstZoneObjList->iObj >= pstSkillDef->TargetNum)
	{
		return -1;
	}
	
	if(pstPlayer->stOnline.stPetOnline.bFightShiNum == 0)
	{
		return -1;
	}

	if(is_pet_heti(pstPlayer))
	{
		return -1;
	}
	
	/*lzk  .... modify ....start  2014-2-19*/
	//由if(is_tishen_skill(pstSkillDef))   改为
	//非替身技能
	if(!is_tishen_skill(pstSkillDef))
	{
		return -1;
	}
	/*modify end....*/
			
	pstRolePet = z_find_role_pet(pstPlayer, pstPetOnline->astFightShi[0].stRolePetOnline.ullWID, NULL);
	if (NULL == pstRolePet)
	{
		return -1;
	}

	pstAni = &pstZoneObjList->astObj[pstZoneObjList->iObj];
	pstAni->iType = OBJ_PET;
	pstAni->stObj.stZonePet.pstOwner = pstPlayer;
	pstAni->stObj.stZonePet.pstPetFightShi = (PetFightShiOnline *)pet_find_online(pstPlayer, pstRolePet);
	pstAni->stObj.stZonePet.pstRolePet = pstRolePet;

	if (0 > z_mon_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstMon, pstAni, pstSkillDef,NULL))
	{
		return -1;
	}

	pstZoneObjList->iObj++;

	return 0;
}

int z_mon_get_atk_list(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, SKILLDEF *pstSkillDef, 
							ZoneAni *pstZoneAni, RESPOS *pstHitPos, ZoneObjList *pstZoneObjList, int iSearch)
{
	int i;
	int iIndex;
	char cType;
	Player *pstPlayer = NULL;
	TMEMBLOCK *pstMemBlock;
	ZoneAni *pstAni;
	MapIndex *pstMapIdx;
	TMAPMASK *pstMask;
	ZONEMAPINST *pstMapInst;
	RESPOS stAtkeePos;
	SearchArea stSArea;
	Player *pstAniPlayer = NULL;
	Monster *pstAniMon = NULL;
	int iAtkeeRadius = 0;
	int iDir = 0;

	UNUSED(pstMonDef);

	pstZoneObjList->iObj = 0;
	
	if(pstSkillDef->TargetRange == TARGET_RANGE_DIRECTION)
	{
		iDir = pstMon->bDirection*2 + 90;
	}

	//会打怪物的怪物需要从地图上搜索而不是视野
	if(pstMon->bSubType == MON_SUB_TYPE_TOTEM || 
		pstMon->bSubType == MON_SUB_TYPE_BUILD || 
		is_friend_mon(pstMon) ||
		(pstMonDef->MiscType&MON_MISC_TYPE_NO_ATKEE) ||
		pstSkillDef->TargetNum > MAX_MONSTER_VPLAYER_NORM)
	{
		iSearch = 1;
	}

	if(pstSkillDef->TargetType & SKILL_TARGET_ENEMY_MON)
	{
		iSearch = 1;
	}

	// SKILL_TYPE_SPECIFY_AREA_ATK 没有pstZoneAni   从地图搜索
	if (pstSkillDef->SkillType == SKILL_TYPE_SPECIFY_AREA_ATK)
	{
		pstZoneAni = NULL;
		iSearch = 1;
	}

	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
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

	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstAppCtx, pstEnv, pstMon, 0);
	}

	//优先目标
	if (pstZoneAni && (0 == z_mon_filter_atkobj(pstAppCtx, pstEnv, pstMon, pstZoneAni, pstSkillDef,NULL)))
	{
		if (OBJ_PLAYER == pstZoneAni->iType)
		{
			pstAniPlayer = pstZoneAni->stObj.pstPlayer;
			stAtkeePos = pstAniPlayer->stRoleData.Pos;
		}
		else if (OBJ_MONSTER == pstZoneAni->iType)
		{
			pstAniMon = pstZoneAni->stObj.pstMon;
			stAtkeePos = pstAniMon->stCurrPos;

			iAtkeeRadius = pstAniMon->unRadius;
		}
		else
		{
			return -1;
		}

		//pstZoneAni外面已经move curr了

		if (pstSkillDef->PrepareTime == 0 || (0 == z_in_range(&stAtkeePos, &pstMon->stCurrPos, pstHitPos, &pstSkillDef->MainRange, pstSkillDef->Distance, iAtkeeRadius, iDir) &&
			IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, stAtkeePos.X, stAtkeePos.Y)))
		{
			pstZoneObjList->iObj = 1;
			memcpy(&pstZoneObjList->astObj[0], pstZoneAni, sizeof(pstZoneObjList->astObj[0]));
		}

		if(pstAniPlayer && pstZoneAni->iType == OBJ_PLAYER)
		{
			z_mon_get_atk_pet(pstEnv, pstSkillDef, pstAniPlayer, pstMon, pstZoneObjList);
		}
	}

	if (pstZoneAni && OBJ_MONSTER == pstZoneAni->iType && (pstSkillDef->TargetType & (SKILL_TARGET_FRIEND_MON | SKILL_TARGET_MID_MON |SKILL_TARGET_SELF)))
	{
		iSearch = 1;
	}

	if (0 == iSearch)
	{
		for (i=0; i<pstMon->iVPlayer; i++) 
		{
			if (pstZoneObjList->iObj >= pstSkillDef->TargetNum)
			{
				break;
			}

			iIndex = z_id_indextype(pstAppCtx, pstEnv, pstMon->aiVPlayer[i], &cType);
			if (0 > iIndex)
			{
				continue;
			}

			pstAni = &pstZoneObjList->astObj[pstZoneObjList->iObj];
			pstAni->iType = cType;

			iAtkeeRadius = 0;
			if (cType == OBJ_PLAYER)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
										
				pstAni->stObj.pstPlayer = pstPlayer = (Player *)pstMemBlock->szData;

				if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus || 
					(0 == pstSkillDef->DeadTarget && ((pstPlayer->stOnline.State & CS_STAT_DEAD) || pstPlayer->stRoleData.RoleDetail.HPCurr == 0)) ||
					(pstPlayer->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN))|| pstPlayer->stOnline.fShamDead)
				{
					continue;
				}

				if(is_player_ride_on_machine(pstPlayer))
				{
					continue;
				}

				if (pstPlayer == pstAniPlayer)
				{
					continue;
				}
				
				if (pstPlayer->stOnline.cMoving)
				{
					z_player_move_curr(pstAppCtx, pstEnv, pstPlayer, 0);
				}

				stAtkeePos = pstPlayer->stRoleData.Pos;
			}
			else if (cType == OBJ_MONSTER)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
										
				pstAni->stObj.pstMon = (Monster *)pstMemBlock->szData;
				if (AI_STAT_HOME == pstAni->stObj.pstMon->bAiStat ||   
					pstAni->stObj.pstMon->bSubType >= MON_SUB_TYPE_TRAP ||
					 pstAppCtx->stCurr.tv_sec < pstAni->stObj.pstMon->tBirthEnd ||
					(0 == pstSkillDef->DeadTarget && (pstAni->stObj.pstMon->cStat != MONSTER_ALIVE || pstAni->stObj.pstMon->iHP == 0)))
				{
					continue;
				}

				if (pstAni->stObj.pstMon == pstAniMon)
				{
					continue;
				}

				if (pstAni->stObj.pstMon->cMoving)
				{
					z_monster_move_curr(pstAppCtx, pstEnv, pstAni->stObj.pstMon, 0);
				}

				stAtkeePos = pstAni->stObj.pstMon->stCurrPos;
				iAtkeeRadius = pstAni->stObj.pstMon->unRadius;
			}
			else if (cType == OBJ_NPC)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					continue;
				}
										
				pstAni->stObj.pstNpc = (Npc *)pstMemBlock->szData;
				if (AI_STAT_HOME == pstAni->stObj.pstNpc->stMon.bAiStat ||
					pstAni->stObj.pstNpc->stMon.bSubType >= MON_SUB_TYPE_TRAP ||
					 pstAppCtx->stCurr.tv_sec < pstAni->stObj.pstNpc->stMon.tBirthEnd ||
					(0 == pstSkillDef->DeadTarget && (pstAni->stObj.pstNpc->stMon.cStat != MONSTER_ALIVE || pstAni->stObj.pstNpc->stMon.iHP == 0)))
				{
					continue;
				}

				if ((Monster *)pstAni->stObj.pstNpc== pstAniMon)
				{
					continue;
				}

				if (pstAni->stObj.pstNpc->stMon.cMoving)
				{
					z_monster_move_curr(pstAppCtx, pstEnv, &pstAni->stObj.pstNpc->stMon, 0);
				}

				stAtkeePos = pstAni->stObj.pstNpc->stMon.stCurrPos;
				iAtkeeRadius = pstAni->stObj.pstNpc->stMon.unRadius;
			}
			
			else
			{
				continue;
			}

			if (0 > z_mon_filter_atkobj(pstAppCtx, pstEnv, pstMon, pstAni, pstSkillDef,NULL))
			{
				continue;
			}

			if (0 > z_in_range(&stAtkeePos, &pstMon->stCurrPos, pstHitPos, &pstSkillDef->MainRange, pstSkillDef->Distance, iAtkeeRadius, iDir))
			{
				continue;
			}

			if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, stAtkeePos.X, stAtkeePos.Y))
			{
				continue;
			}

			pstZoneObjList->iObj++;

			//如果可以打人顺便看看可否打宠物
			if(pstPlayer && pstAni->iType == OBJ_PLAYER)
			{
				z_mon_get_atk_pet(pstEnv, pstSkillDef, pstPlayer, pstMon, pstZoneObjList);
			}
		}
	}
	else
	{
		if(0 == z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, &pstMon->stCurrPos, &stSArea))
		{
			int j;
			AREADYN *pstArea;

			for(i = 0; i < stSArea.iNum; i++)
			{
				if (pstZoneObjList->iObj >= pstSkillDef->TargetNum)
				{
					break;
				}
				
				pstArea = stSArea.pastArea[i];

				for (j = 0; j < pstArea->iAnimate; j++)
				{
					if (pstZoneObjList->iObj >= pstSkillDef->TargetNum)
					{
						break;
					}
					
					iIndex	=	z_id_indextype(pstAppCtx, pstEnv, pstArea->aiAnimate[j], &cType);
					if (iIndex < 0) continue;

					pstAni = &pstZoneObjList->astObj[pstZoneObjList->iObj];
					pstAni->iType = cType;

					iAtkeeRadius = 0;

					if (cType == OBJ_PLAYER)
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
						if (!pstMemBlock->fValid)
						{
							continue;
						}
												
						pstAni->stObj.pstPlayer = pstPlayer = (Player *)pstMemBlock->szData;

						if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus ||
							(0 == pstSkillDef->DeadTarget && (pstPlayer->stOnline.State & CS_STAT_DEAD || pstPlayer->stRoleData.RoleDetail.HPCurr == 0)) ||
							(pstPlayer->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN))|| pstPlayer->stOnline.fShamDead)
						{
							continue;
						}

						if(is_player_ride_on_machine(pstPlayer))
						{
							continue;
						}

						if (pstPlayer == pstAniPlayer)
						{
							continue;
						}
						
						if (pstPlayer->stOnline.cMoving)
						{
							z_player_move_curr(pstAppCtx, pstEnv, pstPlayer, 0);
						}

						stAtkeePos = pstPlayer->stRoleData.Pos;
					}
					else if (cType == OBJ_MONSTER)
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
						if (!pstMemBlock->fValid)
						{
							continue;
						}
												
						pstAni->stObj.pstMon = (Monster *)pstMemBlock->szData;
						if (AI_STAT_HOME == pstAni->stObj.pstMon->bAiStat ||
							pstAni->stObj.pstMon->bSubType >= MON_SUB_TYPE_TRAP ||
							 pstAppCtx->stCurr.tv_sec < pstAni->stObj.pstMon->tBirthEnd ||
							(0 == pstSkillDef->DeadTarget && (pstAni->stObj.pstMon->cStat != MONSTER_ALIVE || pstAni->stObj.pstMon->iHP == 0)))
						{
							continue;
						}

						if (pstAni->stObj.pstMon == pstAniMon)
						{
							continue;
						}

						if (pstAni->stObj.pstMon->cMoving)
						{
							z_monster_move_curr(pstAppCtx, pstEnv, pstAni->stObj.pstMon, 0);
						}

						stAtkeePos = pstAni->stObj.pstMon->stCurrPos;

						iAtkeeRadius = pstAni->stObj.pstMon->unRadius;
					}
					else if (cType == OBJ_NPC)
					{
						pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
						if (!pstMemBlock->fValid)
						{
							continue;
						}
												
						pstAni->stObj.pstNpc = (Npc *)pstMemBlock->szData;
						if (AI_STAT_HOME == pstAni->stObj.pstNpc->stMon.bAiStat ||
							pstAni->stObj.pstNpc->stMon.bSubType >= MON_SUB_TYPE_TRAP ||
							 pstAppCtx->stCurr.tv_sec < pstAni->stObj.pstNpc->stMon.tBirthEnd ||
							(0 == pstSkillDef->DeadTarget && (pstAni->stObj.pstNpc->stMon.cStat != MONSTER_ALIVE || pstAni->stObj.pstNpc->stMon.iHP == 0)))
						{
							continue;
						}

						if ((Monster *)pstAni->stObj.pstNpc == pstAniMon)
						{
							continue;
						}

						if (pstAni->stObj.pstNpc->stMon.cMoving)
						{
							z_monster_move_curr(pstAppCtx, pstEnv, &pstAni->stObj.pstNpc->stMon, 0);
						}

						stAtkeePos = pstAni->stObj.pstNpc->stMon.stCurrPos;
						iAtkeeRadius = pstAni->stObj.pstNpc->stMon.unRadius;
					}
					else
					{
						continue;
					}

					if (0 > z_mon_filter_atkobj(pstAppCtx, pstEnv, pstMon, pstAni, pstSkillDef,NULL))
					{
						continue;
					}

					if (pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE)
					{
						if (z_distance(&pstMon->stCurrPos, &stAtkeePos)  > pstSkillDef->Distance+pstMon->unRadius+iAtkeeRadius)
						{
							continue;
						}
					}
					else	if (0 > z_in_range(&stAtkeePos, &pstMon->stCurrPos, pstHitPos, &pstSkillDef->MainRange, pstSkillDef->Distance, iAtkeeRadius, iDir))
					{
						continue;
					}

					if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, stAtkeePos.X, stAtkeePos.Y))
					{
						continue;
					}

					pstZoneObjList->iObj++;

					//如果可以打人顺便看看可否打宠物
					if(pstPlayer && pstAni->iType == OBJ_PLAYER)
					{
						z_mon_get_atk_pet(pstEnv, pstSkillDef, pstPlayer, pstMon, pstZoneObjList);
					}
				}
			}
		}
	}

	return 0;
}

//指定点范围获取攻击列表
int z_mon_get_atk_list_by_atkpos(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon,SKILLDEF *pstSkillDef,RESPOS *pstHitPos,
									ZoneObjList *pstZoneObjList)
{
	int i;
	int iIndex;
	char cType;
	Player *pstPlayer = NULL;
	TMEMBLOCK *pstMemBlock;
	ZoneAni *pstAni;
	MapIndex *pstMapIdx;
	TMAPMASK *pstMask;
	RESPOS stAtkeePos;
	SearchArea stSArea;
	Player *pstAniPlayer = NULL;
	Monster *pstAniMon = NULL;
	int iAtkeeRadius = 0;
	int iDir = 0;
	ZONEMAPINST *pstMapInst;

	pstZoneObjList->iObj = 0;
	pstMapInst	=	z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
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

	
	if(0 == z_pos_search_area(pstEnv->pstAppCtx, pstEnv, pstMapInst, pstHitPos, &stSArea))
	{
		int j;
		AREADYN *pstArea;

		for(i = 0; i < stSArea.iNum; i++)
		{
			if (pstZoneObjList->iObj >= pstSkillDef->TargetNum)
			{
				break;
			}
			
			pstArea = stSArea.pastArea[i];

			for (j = 0; j < pstArea->iAnimate; j++)
			{
				if (pstZoneObjList->iObj >= pstSkillDef->TargetNum)
				{
					break;
				}
				
				iIndex	=	z_id_indextype(pstAppCtx, pstEnv, pstArea->aiAnimate[j], &cType);
				if (iIndex < 0) continue;

				pstAni = &pstZoneObjList->astObj[pstZoneObjList->iObj];
				pstAni->iType = cType;

				iAtkeeRadius = 0;

				if (cType == OBJ_PLAYER)
				{
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						continue;
					}
											
					pstAni->stObj.pstPlayer = pstPlayer = (Player *)pstMemBlock->szData;

					if (PLAYER_STATUS_ROLE_LOGIN != pstPlayer->eStatus ||
						(0 == pstSkillDef->DeadTarget && (pstPlayer->stOnline.State & CS_STAT_DEAD || pstPlayer->stRoleData.RoleDetail.HPCurr == 0)) ||
						(pstPlayer->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN))|| pstPlayer->stOnline.fShamDead)
					{
						continue;
					}

					if(is_player_ride_on_machine(pstPlayer))
					{
						continue;
					}

					if (pstPlayer == pstAniPlayer)
					{
						continue;
					}
					
					if (pstPlayer->stOnline.cMoving)
					{
						z_player_move_curr(pstAppCtx, pstEnv, pstPlayer, 0);
					}

					stAtkeePos = pstPlayer->stRoleData.Pos;
				}
				else if (cType == OBJ_MONSTER)
				{
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						continue;
					}
											
					pstAni->stObj.pstMon = (Monster *)pstMemBlock->szData;
					if (AI_STAT_HOME == pstAni->stObj.pstMon->bAiStat ||
						pstAni->stObj.pstMon->bSubType >= MON_SUB_TYPE_TRAP ||
						 pstAppCtx->stCurr.tv_sec < pstAni->stObj.pstMon->tBirthEnd ||
						(0 == pstSkillDef->DeadTarget && (pstAni->stObj.pstMon->cStat != MONSTER_ALIVE || pstAni->stObj.pstMon->iHP == 0)))
					{
						continue;
					}

					if (pstAni->stObj.pstMon == pstAniMon)
					{
						continue;
					}

					if (pstAni->stObj.pstMon->cMoving)
					{
						z_monster_move_curr(pstAppCtx, pstEnv, pstAni->stObj.pstMon, 0);
					}

					stAtkeePos = pstAni->stObj.pstMon->stCurrPos;

					iAtkeeRadius = pstAni->stObj.pstMon->unRadius;
				}
				else if (cType == OBJ_NPC)
				{
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						continue;
					}
											
					pstAni->stObj.pstNpc = (Npc *)pstMemBlock->szData;
					if (AI_STAT_HOME == pstAni->stObj.pstNpc->stMon.bAiStat ||
						pstAni->stObj.pstNpc->stMon.bSubType >= MON_SUB_TYPE_TRAP ||
						 pstAppCtx->stCurr.tv_sec < pstAni->stObj.pstNpc->stMon.tBirthEnd ||
						(0 == pstSkillDef->DeadTarget && (pstAni->stObj.pstNpc->stMon.cStat != MONSTER_ALIVE || pstAni->stObj.pstNpc->stMon.iHP == 0)))
					{
						continue;
					}

					if ((Monster *)pstAni->stObj.pstNpc== pstAniMon)
					{
						continue;
					}

					if (pstAni->stObj.pstNpc->stMon.cMoving)
					{
						z_monster_move_curr(pstAppCtx, pstEnv, &pstAni->stObj.pstNpc->stMon, 0);
					}

					stAtkeePos = pstAni->stObj.pstNpc->stMon.stCurrPos;
					iAtkeeRadius = pstAni->stObj.pstNpc->stMon.unRadius;
				}
				else
				{
					continue;
				}

				if (0 > z_mon_filter_atkobj(pstAppCtx, pstEnv, pstMon, pstAni, pstSkillDef,NULL))
				{
					continue;
				}

				if (pstSkillDef->MainRange.RangeType == SKILL_RANGE_NONE || pstSkillDef->MainRange.RangeType == SKILL_RANGE_SIGLE)
				{
					if (z_distance(&pstMon->stCurrPos, &stAtkeePos)  > pstSkillDef->Distance+pstMon->unRadius+iAtkeeRadius)
					{
						continue;
					}
				}
				else	if (0 > z_in_range(&stAtkeePos, pstHitPos, pstHitPos, &pstSkillDef->MainRange, 9999, iAtkeeRadius, iDir))
				{
					continue;
				}

				if (!IsSkillThrough(pstEnv, pstMapInst, pstSkillDef, pstMask, pstMon->stCurrPos.X, pstMon->stCurrPos.Y, stAtkeePos.X, stAtkeePos.Y))
				{
					continue;
				}

				pstZoneObjList->iObj++;

				//如果可以打人顺便看看可否打宠物
				if(pstPlayer && pstAni->iType == OBJ_PLAYER)
				{
					z_mon_get_atk_pet(pstEnv, pstSkillDef, pstPlayer, pstMon, pstZoneObjList);
				}
			}
		}
	}
	return 0;
}


void player_def_hp_chg(Player *pstAtker, Player *pstPlayer, double *pdHP, int iSkillType)
{
	int iRate = 0;
	int iIgonreMul = 0;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;

	if(pstDyn->stDefChg.iDefHP > 0)
	{
		iRate = pstDyn->stDefChg.iDefHP*100.0/
			(pstDyn->stDefChg.iDefHP+pstPlayer->stRoleData.Level*pstPlayer->stRoleData.Level/1.5);
	}

	iIgonreMul = z_get_ignore_def(pstAtker, iSkillType);
	
	*pdHP = (*pdHP)*(100+iIgonreMul-iRate)/100.0;
	return ;
}

int player_def_hp_chg_rate(Player *pstAtker, Player *pstPlayer, int iSkillType)
{
	int iRate = 0;
	int iIgonreMul = 0;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;

	if(pstDyn->stDefChg.iDefHP > 0)
	{
		iRate = pstDyn->stDefChg.iDefHP*10000.0/
			(pstDyn->stDefChg.iDefHP*2+pstPlayer->stRoleData.Level*pstPlayer->stRoleData.Level/1.5);
	}

	iIgonreMul = z_get_ignore_def(pstAtker, iSkillType);
	
	iRate = iIgonreMul - iRate;
	return iRate;
}

int player_reflect_hp(ZONESVRENV* pstEnv, Player *pstPlayer, AtkProcess *pstAtkProc, int iChgHP)
{
	int iRate;
	int iHP = 0, fDead = 0;
	PlayerChagAttrVal stValChg;
	DYNATTRLIST *pstDyn = &pstPlayer->stOnline.stDyn;

	if(pstDyn->stDefChg.iReflectHP <= 0)
	{
		return 0;
	}
	
	iRate = pstDyn->stDefChg.iReflectHP*10000.0/
		(pstDyn->stDefChg.iReflectHP*2+pstPlayer->stRoleData.Level*pstPlayer->stRoleData.Level);
	
	iHP  = iChgHP*iRate/10000.0;
	if(iHP <= 0)
	{
		return 0;
	}

	if(pstAtkProc->stAtkerInfo.stAtker.iType ==OBJ_MONSTER)
	{
		int iOldHP;
		Monster *pstMon = pstAtkProc->stAtkerInfo.stAtker.stObj.pstMon;

		 if( 0<=z_mon_dec_one_hp_by_hit(pstEnv, pstMon) )
		{
			iHP = 1;
		}
		 
		if(pstMon->bSubType > MON_SUB_TYPE_TMP)
		{
			return 0;
		}
		
		iOldHP = pstMon->iHP;

		pstMon->iHP -= iHP;
		if(pstMon->iHP < 0)
		{
			pstMon->iHP = 0;
		}

		if(iOldHP != pstMon->iHP)
		{
			mon_hp_action(pstEnv, pstMon, iOldHP, pstPlayer->iMemID);
		}

		if(pstMon->iHP <= 0)
		{
			fDead = 1;
		}
	}
	else if(pstAtkProc->stAtkerInfo.stAtker.iType == OBJ_PLAYER)
	{
		int iOldHP;
		Player *pstAtkeePlayer = pstAtkProc->stAtkerInfo.stAtker.stObj.pstPlayer;

		iOldHP = pstAtkeePlayer->stRoleData.RoleDetail.HPCurr;

		if(iHP >= iOldHP && z_is_qiecuo(pstEnv, pstAtkeePlayer, pstPlayer))
		{
			iHP = iOldHP -1;
			pstAtkProc->cQieCuoEnd = 2;
		}
		
		stValChg.AttrNum = 1;
		stValChg.AttrList[0].AttrID = ATTR_ID_HP;
		stValChg.AttrList[0].nFlag = 1;
		stValChg.AttrList[0].Value.HP = iHP;
		z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstAtkeePlayer, &stValChg, 0);

		if(pstAtkeePlayer->stRoleData.RoleDetail.HPCurr != (unsigned int)iOldHP)
		{
			player_hp_action(pstEnv, pstAtkeePlayer, 
				iOldHP, pstAtkeePlayer->stRoleData.RoleDetail.MPCurr, pstPlayer->iMemID);
		}

		if(pstAtkeePlayer->stRoleData.RoleDetail.HPCurr <= 0)
		{
			fDead = 1;
		}
	}

	if(fDead)
	{
		if(pstAtkProc->stAtkerInfo.pstStatusSkillAtker)
		{
			ZoneAni stAtker;

			stAtker.iType = OBJ_PLAYER;
			stAtker.stObj.pstPlayer = pstPlayer;
			
			player_die(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAtker, 0);
		}
		else
		{
			pstAtkProc->stDeadList.iSelf = 1;
			pstAtkProc->stDeadList.stSelfDead.stAtker.iType = OBJ_PLAYER;
			pstAtkProc->stDeadList.stSelfDead.stAtker.stObj.pstPlayer = pstPlayer;
		}
	}
	
	return 0;
}

int is_beatk_player(ZONESVRENV* pstEnv, ZoneAni *pstAtker, Player *pstPlayer)
{
	if(pstAtker->iType == OBJ_PLAYER)
	{
		return is_player_enemy_player(pstEnv, pstAtker->stObj.pstPlayer, pstPlayer);
	}
	else if(pstAtker->iType == OBJ_PET)
	{
		return is_player_enemy_player(pstEnv, pstAtker->stObj.stZonePet.pstOwner, pstPlayer);
	}
	else if(pstAtker->iType == OBJ_MONSTER)
	{
		return 1;
	}

	return 0;
}

int one_hit_notify(ZONESVRENV* pstEnv, AtkProcess *pstAtkProc, CSONEHIT *pstOneHit, int iNotify)
{
	CSPKG stPkg;
	int iIndex;
	TMEMBLOCK *pstMemBlock;
	char cType, HurtType;
	int j, iHP, iMP, iTotalHurt;
	int Value;
	Player *pstPlayer;
	Player *pstAtker;
	SKILLDEF *pstSkillDef = pstAtkProc->stAtkerInfo.pstSkillDef;
	PlayerChagAttrVal stValChg;
	ZoneDeadList *pstZoneDeadList = &pstAtkProc->stDeadList;
	Monster *pstMon;
	unsigned short unTraceDist;
	short shPressRate = 0;
	short shPressRateTmp = 0;
	AtkerInfo *pstAtkerInfo = &pstAtkProc->stAtkerInfo;
	ZoneAni stTmpAtker;
	int iOldHP;

	iHP = iMP = iTotalHurt = 0;
	for (j=0; j<pstOneHit->TotalHurt; j++)
	{
		HurtType = pstOneHit->Hurts[j].HurtType;
		Value = pstOneHit->Hurts[j].Value;
		switch(HurtType)
		{
			case HURT_TYPE_DMG:
			case HURT_TYPE_HEAVY_DMG:
			case HURT_TYPE_HITDEAD:
				iTotalHurt += Value;
				break;
			case HURT_TYPE_HP:
				if (Value < 0)
				{
					Value = -Value;
					pstOneHit->Hurts[j].HurtType = HURT_TYPE_DMG;
					pstOneHit->Hurts[j].Value = Value;
					iTotalHurt += Value;
				}
				else
				{
					iHP += Value;
				}
				break;
			case HURT_TYPE_MP:
				iMP += Value;
				break;
			default:
				break;
		}
	}	

	if (pstOneHit->ID >= PET_ID_BASE)
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstOneHit->ID%PET_ID_BASE, &cType);
	}
	else
	{
		iIndex = z_id_indextype(pstEnv->pstAppCtx, pstEnv, pstOneHit->ID, &cType);
	}
	
	if (0 > iIndex)
	{
		return -1;
	}

	if (pstAtkerInfo->pstStatusSkillAtker)
	{
		stTmpAtker = pstAtkProc->stAtkerInfo.stAtker;
		pstAtkProc->stAtkerInfo.stAtker.iType = OBJ_PLAYER;
		pstAtkProc->stAtkerInfo.stAtker.stObj.pstPlayer = pstAtkerInfo->pstStatusSkillAtker;
	}

	switch(cType)
	{
		case OBJ_PLAYER:
			pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
			if (!pstMemBlock->fValid)
			{
				break;
			}
			pstPlayer = (Player *)pstMemBlock->szData;

			if (pstOneHit->ID >= PET_ID_BASE) //宠物 
			{
				PetFightShiOnline *pstPetFightShiOnline;
				ROLEPET *pstRolePet;
					
				pstPetFightShiOnline = pet_get_shipet_by_xiedaipos(pstPlayer, (pstOneHit->ID/PET_ID_BASE)-1);
				if (NULL == pstPetFightShiOnline)
				{
					break;
				}
				pstRolePet = z_find_role_pet(pstPlayer, pstPetFightShiOnline->stRolePetOnline.ullWID, NULL);
				if (NULL == pstRolePet)
				{
					break;
				}

				pet_add_power(pstEnv, pstPlayer, pstRolePet, iHP-iTotalHurt);
				pstOneHit->HP = pstRolePet->HP;
				if (0 == pstRolePet->HP)
				{
					pstOneHit->DeadFlag = 1;
				}

				if(pstOneHit->StatusFlag)
				{
					z_get_pet_csstatusinfo(pstEnv->pstAppCtx, &pstOneHit->Status, &pstPetFightShiOnline->stStatusInfo);
				}

				z_status_pet_refresh_time_by_hit(pstEnv, pstSkillDef,pstPlayer, pstRolePet);
				break;
			}
			else 
			{
				z_status_player_refresh_time_by_hit(pstEnv, pstSkillDef,pstPlayer);
			}

			if(iTotalHurt > 0 && (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_STATUS_TRACE))
			{
				player_status_beatk(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstOneHit);
			}

			if(is_beatk_player(pstEnv, &pstAtkProc->stAtkerInfo.stAtker, pstPlayer))
			{
				player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_BEATK);
				
				z_atkee_prepare_break(pstEnv->pstAppCtx, pstEnv, pstPlayer, iTotalHurt, 0);

				player_trace_act_break_prepare(pstEnv, pstPlayer, BRK_PRE_ACT_SPE_MON);
			}

			//特殊任务被击
			if (pstPlayer->stOnline.cMiscFlag & MISC_FLAG_BEATK_TRACE)
			{
				task_beatk_player_trace(pstEnv, pstPlayer);
			}
			
			stValChg.AttrNum = 0;
			
			stValChg.AttrList[stValChg.AttrNum].AttrID = ATTR_ID_HP;
			if (iTotalHurt - iHP > 0)
			{
				stValChg.AttrList[stValChg.AttrNum].nFlag = 1;
				stValChg.AttrList[stValChg.AttrNum].Value.HP = iTotalHurt - iHP;

				pstAtker = get_atker_player(pstEnv,  &pstAtkProc->stAtkerInfo.stAtker);
				if (	((int)pstPlayer->stRoleData.RoleDetail.HPCurr - (iTotalHurt - iHP)<= 0)  && 
					pstAtker &&
					z_is_qiecuo(pstEnv,pstAtker, pstPlayer))
				{
					stValChg.AttrList[stValChg.AttrNum].Value.HP = 	pstPlayer->stRoleData.RoleDetail.HPCurr-1;

					pstAtkProc->cQieCuoEnd = 1;
				}
			}
			else 
			{
				stValChg.AttrList[stValChg.AttrNum].nFlag = 0;
				stValChg.AttrList[stValChg.AttrNum].Value.HP = iHP - iTotalHurt;
			}
			stValChg.AttrNum++;

			stValChg.AttrList[stValChg.AttrNum].AttrID = ATTR_ID_MP;
			if (iMP > 0)
			{
				stValChg.AttrList[stValChg.AttrNum].nFlag = 0;
				stValChg.AttrList[stValChg.AttrNum].Value.MP = iMP;
			}
			else
			{
				stValChg.AttrList[stValChg.AttrNum].nFlag = 1;
				stValChg.AttrList[stValChg.AttrNum].Value.MP = -iMP;
			}
			stValChg.AttrNum++;

			iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
			z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stValChg, 0);

			//减血了尝试反射
			if(pstPlayer->stRoleData.RoleDetail.HPCurr < (unsigned int )iOldHP)
			{
				player_reflect_hp(pstEnv, pstPlayer, pstAtkProc, 
							iOldHP - pstPlayer->stRoleData.RoleDetail.HPCurr);
			}

			if (pstPlayer->stOnline.stDyn.stMidAttrList.uiHPMax > 0)
			{
				pstOneHit->HP = pstPlayer->stRoleData.RoleDetail.HPCurr;
			}

			if (0 == pstPlayer->stRoleData.RoleDetail.HPCurr && !(pstPlayer->stOnline.State & CS_STAT_DEAD))
			{
				pstOneHit->DeadFlag = 1;
				
				pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtkee.iType = OBJ_PLAYER;
				pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtkee.stObj.pstPlayer = pstPlayer;
				pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtker = pstAtkProc->stAtkerInfo.stAtker;
				pstZoneDeadList->iCount++;
			}

			if(pstOneHit->StatusFlag)
			{
				z_get_player_csstatusinfo(pstEnv->pstAppCtx, &pstOneHit->Status, &pstPlayer->stOnline.stStatusList);
			}

			if (iNotify)
			{
				make_hit_res(pstEnv, pstAtkProc, pstOneHit, 1, &stPkg);
				z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
			}
			break;
		case OBJ_MONSTER:
		case OBJ_NPC:
			if (OBJ_MONSTER == cType)
			{
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					break;
				}			
				pstMon = (Monster *)pstMemBlock->szData;
			}
			else
			{
				Npc *pstNpc;
				pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstNpcPool, iIndex);
				if (!pstMemBlock->fValid)
				{
					break;
				}
				pstNpc = (Npc  *)pstMemBlock->szData;
				pstMon = &pstNpc->stMon;
			}
			
			pstMon->iHP -= (iTotalHurt - iHP); 
			if (pstMon->iHP < 0)
			{
				pstMon->iHP = 0;
			}
			else if (pstMon->iHP > pstMon->iMaxHP)
			{
				pstMon->iHP = pstMon->iMaxHP;
			}

			pstOneHit->HP = pstMon->iHP;
			
			if (0 == pstMon->iHP &&  (pstMon->cStat != MONSTER_DIE))
			{
				pstOneHit->DeadFlag = 1;
					
				pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtkee.iType = OBJ_MONSTER;
				pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtkee.stObj.pstMon = pstMon;
				pstZoneDeadList->astDeads[pstZoneDeadList->iCount].stAtker = pstAtkProc->stAtkerInfo.stAtker;
				pstZoneDeadList->iCount++;
			}
			
			unTraceDist = 0;
			if(pstAtkProc->stAtkerInfo.stAtker.iType == OBJ_MONSTER)
			{
				unTraceDist =  z_distance(&pstAtkProc->stAtkerInfo.stAtker.stObj.pstMon->stCurrPos, &pstMon->stCurrPos);
			}
			else
			{
			 	unTraceDist = z_distance(&(pstAtkProc->stAtkerInfo.stAtker.stObj.pstPlayer)->stRoleData.Pos, &pstMon->stCurrPos);
			}
				
			if ( (iTotalHurt - iHP) > 0 && 
				(0 == pstMon->unTraceDist || unTraceDist <= pstMon->unTraceDist) )
			{
				if (pstAtkProc->stAiList.iCount < (int)(sizeof(pstAtkProc->stAiList.astMonAi)/sizeof(pstAtkProc->stAiList.astMonAi[0])))
				{
					AIEvt *pstAIEvt = &pstAtkProc->stAiList.astMonAi[pstAtkProc->stAiList.iCount];
					//保证下,最小也不要超过-100
					shPressRateTmp = shPressRate + pstSkillDef->PressRate;
					if(shPressRateTmp < -100 )
					{
						shPressRateTmp =  -100;
					}
					
					pstAIEvt->iEvtID = AI_EVT_BEATK;
					pstAIEvt->stEvtDetail.stBeAtk.iHurt = iTotalHurt - iHP;

					if (pstSkillDef)
					{
						pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = shPressRateTmp;
					}
					else
					{
						pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = 0;
					}

					pstAtkProc->stAiList.apstMon[pstAtkProc->stAiList.iCount] = pstMon;

					if (OBJ_PLAYER == pstAtkProc->stAtkerInfo.stAtker.iType)
					{
						pstAIEvt->stEvtDetail.stBeAtk.stAtker.iType = OBJ_PLAYER;
						pstAIEvt->stEvtDetail.stBeAtk.stAtker.stObj.pstPlayer = pstAtkProc->stAtkerInfo.stAtker.stObj.pstPlayer;
						pstAtkProc->stAiList.iCount++;
					}
					else if (OBJ_PET == pstAtkProc->stAtkerInfo.stAtker.iType)
					{
						pstAIEvt->stEvtDetail.stBeAtk.stAtker.iType = OBJ_PLAYER;
						pstAIEvt->stEvtDetail.stBeAtk.stAtker.stObj.pstPlayer = pstAtkProc->stAtkerInfo.stAtker.stObj.stZonePet.pstOwner;
						pstAtkProc->stAiList.iCount++;
					}
					else if (OBJ_MONSTER == pstAtkProc->stAtkerInfo.stAtker.iType)
					{
						Monster *pstAtkerMon = NULL;
						MONSTERDEF *pstAtkerMonDef = NULL;
						
						pstAtkerMon = pstAtkProc->stAtkerInfo.stAtker.stObj.pstMon;
						if (pstAtkerMon->iOwnerID > 0)
						{
							pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstAtkerMon->iOwnerID);
							if (pstPlayer)
							{
							    pstAtkerMonDef = z_get_mon_def(pstEnv, pstAtkerMon);
								if( pstAtkerMonDef && 
									pstAtkerMon->bSubType >= MON_SUB_TYPE_TRAP &&
									(pstAtkerMonDef->MiscType&MON_MISC_TYPE_NO_PRESS) )
								{
									pstAIEvt->stEvtDetail.stBeAtk.iHurt = iTotalHurt - iHP;
								}
								else
								{
									pstAIEvt->stEvtDetail.stBeAtk.iHurt = 0;
								}
								pstAIEvt->stEvtDetail.stBeAtk.stAtker.iType = OBJ_PLAYER;
								pstAIEvt->stEvtDetail.stBeAtk.stAtker.stObj.pstPlayer = pstPlayer;
								pstAtkProc->stAiList.iCount++;
							}
                                                
						}

						if (pstAtkProc->stAiList.iCount < (int)(sizeof(pstAtkProc->stAiList.astMonAi)/sizeof(pstAtkProc->stAiList.astMonAi[0])))
						{
							AIEvt *pstAIEvt = &pstAtkProc->stAiList.astMonAi[pstAtkProc->stAiList.iCount];

							pstAIEvt->iEvtID = AI_EVT_BEATK;
							pstAIEvt->stEvtDetail.stBeAtk.iHurt = iTotalHurt - iHP;

							if (pstSkillDef)
							{
								pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = shPressRateTmp;
							}
							else
							{
								pstAIEvt->stEvtDetail.stBeAtk.iSkillPressRate = 0;
							}

							pstAtkProc->stAiList.apstMon[pstAtkProc->stAiList.iCount] = pstMon;
				
							pstAIEvt->stEvtDetail.stBeAtk.stAtker.iType = OBJ_MONSTER;
							pstAIEvt->stEvtDetail.stBeAtk.stAtker.stObj.pstMon = pstAtkerMon;
							pstAtkProc->stAiList.iCount++;
						}
					}
				}
			}

			if(pstOneHit->StatusFlag)
			{
				z_get_mon_csstatusinfo(pstEnv->pstAppCtx, &pstOneHit->Status, &pstMon->stStatusList);
			}

			if (iNotify)
			{
				make_hit_res(pstEnv, pstAtkProc, pstOneHit, 1, &stPkg);
				z_sendpkg_monview(pstEnv->pstAppCtx, pstEnv, pstMon, &stPkg, 0);
			}

			world_city_build_warning(pstEnv, pstMon);
			clan_city_build_warning_byhit(pstEnv,pstMon);
			mon_factory_warning_byhit(pstEnv, pstMon);
			z_status_monster_refresh_time_by_hit(pstEnv, pstSkillDef,pstMon);
			break;
		default:
			pstOneHit->StatusFlag = 0;
			break;
	}

	if (pstAtkerInfo->pstStatusSkillAtker)
	{
		pstAtkProc->stAtkerInfo.stAtker = stTmpAtker;
	}
	
	// 吸收HP
	if (pstAtkProc->cSordHpRate != 0)
	{
		pstAtkerInfo->stResourceConsume.iHPUse -= iTotalHurt*(pstAtkProc->cSordHpRate/100.0);
	}

	if(pstAtkProc->iSordHp != 0)
	{
		pstAtkerInfo->stResourceConsume.iHPUse -= pstAtkProc->iSordHp;
	}

	// 吸收MP
	if (pstAtkProc->cSordMpRate != 0)
	{
		pstAtkerInfo->stResourceConsume.iMPUse -= iTotalHurt*(pstAtkProc->cSordMpRate/100.0);
	}

	return 0;
}

int z_mon_hit_player(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, Player *pstAtkee,
						AtkProcess *pstAtkProcess)
{
	long long llTotalHurt;
	int iHP;
	int iRealHurt, iHurt, iHeavyHit;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	ROLETMPSYS *pstRoleTmpSys = &pstAtkee->stRoleData.MiscInfo.TmpSys;
	ZoneStatusList *pstZoneStatusList = &pstAtkee->stOnline.stStatusList;
	ATKCHG *pstAtkChg = NULL;
	STATUSRESULT stResult;
	int iAllMiss = 1;
	int i;
	struct timeval stCltTime;
	int iNotify = 0;
	ZoneReflectedDamage *pstReflectedDamage = &pstAtkProcess->stReflectedDamage;
	int iReflect = 0;
	STATUSRESULT *pstStatusResult = NULL;
	ZoneAni stAtkerAni;

	memset(&stCltTime,0,sizeof(stCltTime));
	stCltTime.tv_sec = pstAtkProcess->stAtkerInfo.SvrTime.BenchTimeSec;
	stCltTime.tv_usec = pstAtkProcess->stAtkerInfo.SvrTime.BenchTimeUSec;
	
	if (pstAtkee->stOnline.State & CS_STAT_DEAD)
	{
		return -1;
	}

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		if (pstAtkee->stRoleData.Map ==  2400 && z_player_find_status(pstAtkee, 3009) && 
			is_mon_machine(pstMon))	
		{
			return -1;
		}
	}

	// 称号:崔蒂的炮友
	if (pstSkillDef->SkillID == 10192)
	{
		player_desig_get( pstEnv, pstAtkee,GET_DESIG_CUIDI_PAOYOU);
	}

	//隐身受击显形
	if ((pstAtkee->stOnline.State & CS_STAT_QIANXIN) && (pstSkillDef->TargetType & SKILL_TARGET_ENEMY_PLAYER))
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstAtkee, STATUS_ID_QIANXIN, 1, 0);
	}

	pstAtkee->stOnline.iAtkerTarget = pstMon->iID;

	stAtkeeInfo.stAtkee.iType = OBJ_PLAYER;
	stAtkeeInfo.stAtkee.stObj.pstPlayer = pstAtkee;

	pstOneHit->ID = pstAtkee->iMemID;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;
	pstOneHit->TiShenFlag = pstAtkee->stOnline.fPetShouHu;

	z_get_mon_dyn(pstEnv, pstMon, pstMonDef, pstAtkerDyn);
	if(pstMon->iSkillAtkerID > 0 && !is_mon_machine(pstMon))
	{
		Player *pstRealAtker = z_id_player(pstAppCtx, pstEnv, pstMon->iSkillAtkerID);
		if(pstRealAtker)
		{
			memcpy(pstAtkerDyn, &pstRealAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
		}
	}
	
	pstAtkProcess->iAtkChgIdx = 0;
	pstAtkChg = &pstAtkerDyn->stAtkChg;

	memset (pstAtkerInfo->ucAppendResult, 0 , sizeof(pstAtkerInfo->ucAppendResult));
	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;
	
	for (i=0; i<pstZoneStatusList->bBadStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstSkillDef,pstAtkerInfo, pstAtkChg, &pstZoneStatusList->astBadStatus[i]);
	}
	for (i=0; i<pstZoneStatusList->bGoodStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstSkillDef,pstAtkerInfo, pstAtkChg, &pstZoneStatusList->astGoodStatus[i]);
	}
	 //检查自己好的状态对技能的修正
	for (i=0; i<pstMon->stStatusList.bGoodStatusNum; i++)
	{
		z_atker_status_result(pstEnv, pstSkillDef, pstAtkerInfo, pstAtkChg, &pstMon->stStatusList.astGoodStatus[i],pstAtkProcess);
	}

	z_cacl_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	z_cacl_mon_status_result(pstEnv, pstAtkerDyn, pstAtkChg, pstMon);

	player_collectskill_add_slot(pstEnv, pstAtkee,0,COLLECTSKILL_ADDTYPE_AKEE);
	
	memcpy(pstAtkeeDyn, &pstAtkee->stOnline.stDyn, sizeof(*pstAtkeeDyn));
	//z_cacl_player_status_result(pstEnv, pstAtkeeDyn, NULL, pstAtkee);
	if (pstRoleTmpSys->TmpItemID > 0)
	{
		z_cacl_tmpitem_attr(pstEnv, pstRoleTmpSys, pstAtkeeDyn);
	}
	if (pstMon->iOwnerID == 0)
	{
		pstAtkeeDyn->stAtkeeHurtChg.AtkeeHurtMul += pstAtkeeDyn->stMonChg.nAtkeeHurtMul;
		pstAtkeeDyn->stAtkeeHurtChg.AtkeeHurtAdd +=  pstAtkeeDyn->stMonChg.unAtkeeHurtAdd;
		pstAtkeeDyn->stFleeChg.nFleeChgMul += pstAtkeeDyn->stMonChg.nFleeChgMul;
		pstAtkeeDyn->stFleeChg.iFleeChgAdd += pstAtkeeDyn->stMonChg.unFleeChgAdd;
		pstAtkeeDyn->stAtkeeHurtChg.AtkeeMagicHurtAdd += pstAtkeeDyn->stMonChg.nAtkeeMagicDefAdd;
		pstAtkeeDyn->stAtkeeHurtChg.AtkeeMagicHurtMul += pstAtkeeDyn->stMonChg.nAtkeeMagicDefMul;
		pstAtkeeDyn->stAtkeeHurtChg.AtkeeIceHurtAdd += pstAtkeeDyn->stMonChg.nAtkeeIceDefAdd;
		pstAtkeeDyn->stAtkeeHurtChg.AtkeeIceHurtMul += pstAtkeeDyn->stMonChg.nAtkeeIceDefMul;
		pstAtkeeDyn->stAtkeeHurtChg.AtkeePhsicHurtAdd += pstAtkeeDyn->stMonChg.nAtkeePhsicDefAdd;
		pstAtkeeDyn->stAtkeeHurtChg.AtkeePhsicHurtMul += pstAtkeeDyn->stMonChg.nAtkeePhsicDefMul;
	}

	if (pstSkillDef->TargetNum <= MAX_SKILL_TARGET)
	{
		pstHits->TotalHit++;
	}
	else
	{
		pstOneHit = &pstHits->Hits[0];
		memset(pstOneHit, 0, sizeof(*pstOneHit));
		pstOneHit->ID = pstAtkee->iMemID;
		pstHits->TotalHit = 1;
		iNotify = 1;
	}

	stAtkerAni = pstAtkerInfo->stAtker;
	if(pstMon->iSkillAtkerID && !is_mon_machine(pstMon))
	{
		Player *pstTmpPlayer;
		
		pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstMon->iSkillAtkerID);
		if(pstTmpPlayer && pstTmpPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			stAtkerAni.iType = OBJ_PLAYER;
			stAtkerAni.stObj.pstPlayer = pstTmpPlayer;
		}
	}

	//检查设置灰名状态
	atker_set_pktime(pstEnv, &stAtkerAni, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);

	if (pstSkillDef->NoHurtSkill) //非伤害技能，只需要处理技能效果
	{
		if ((pstSkillDef->TargetType & SKILL_TARGET_ENEMY_PLAYER) && z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
			pstOneHit->TotalHurt++;

			return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
		}
		
		z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	llTotalHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;

	//受攻方是否无敌状态
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	// 受功放是否绝对闪避
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_UTTERLY_FLEE))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		//genius_add_status_by_flee(pstAppCtx, pstEnv, pstAtkee);
		z_do_skill_result_by_flee(pstAppCtx, pstEnv,pstSkillDef,NULL,pstMon);

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		//genius_add_status_by_flee(pstAppCtx, pstEnv, pstAtkee);
		z_do_skill_result_by_flee(pstAppCtx, pstEnv,pstSkillDef,NULL,pstMon);
		
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	if(is_in_miss_time_player(&stCltTime, pstAtkee))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	iAllMiss = 0;

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0 /(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}	
		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	// 吸收全部伤害
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_SORB_ALL_HURT))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_SBSORB;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iRealHurt;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DEC_PERCENT_MAX_HP))
	{
		STATUSRESULT stResult;
		z_get_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DEC_PERCENT_MAX_HP,
			                           &stResult, 0);

		MIDATTRLIST *pstMidAttrList = &pstAtkee->stOnline.stDyn.stMidAttrList;
		int iTemp  = pstMidAttrList->uiHPMax * (stResult.ResultVal1/100.0);

		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iTemp;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//被暴击加hp
	//genius_add_hp_by_heavyhit( pstAppCtx, pstEnv, pstAtkee,pstOneHit,iHeavyHit);
	//genius_reduce_skill_cd_time_by_heavyhit( pstAppCtx,  pstEnv, pstAtkee, &stCltTime,iHeavyHit);
	//genius_add_status_by_monster_atk( pstAppCtx, pstEnv, pstAtkee, pstMon);
	z_del_result_by_heavyhit(pstAppCtx, pstEnv,NULL,pstMon,iHeavyHit);

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;
	z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);

	pstStatusResult = player_status_result(pstEnv, pstAtkee, STATUS_RESULT_REFLECTION);
	if(pstStatusResult &&
		pstReflectedDamage->iNum < (int)(sizeof(pstReflectedDamage->astEntries)/sizeof(pstReflectedDamage->astEntries[0])))
	{
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stFrom.iType = OBJ_PLAYER;
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stFrom.stObj.pstPlayer = pstAtkee;
		/*
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stHit = *pstOneHit;
		*/
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stHit.TotalHurt = 0;
		++pstReflectedDamage->iNum;

		iReflect = 1;
	}
	
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		
		pstAtkProcess->iContinueCount--;

		if (iHurt <= 0)
		{
			continue;
		}

		if (iReflect)
		{
			z_hurt_reflect(pstEnv, &iHurt, pstStatusResult, pstReflectedDamage);
		}
		
		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstAtkee->stRoleData.RoleDetail.HPCurr;
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}
		else
		{
			// 伤害吸收计算
			if (0 == (pstAtkerInfo->iFlag & ATKER_INFO_FLAG_THROUGH_ABSORB))
			{
				z_player_hurt_absorb(pstEnv, pstAtkee, pstOneHit, &iHurt);
				z_player_hurt_status_absorb(pstEnv,pstAtkee, pstOneHit,  &iHurt);
			}
		}

		if (iHurt <= 0)
		{
			continue;
		}

		llTotalHurt += iHurt;

		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;

		pstOneHit->TotalHurt++;

		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		if (llTotalHurt >= (iHP + pstAtkee->stRoleData.RoleDetail.HPCurr))
		{
			break;
		}
	}while (pstAtkProcess->iContinueCount > 0);

	if (0 == iAllMiss && 0 == pstSkillDef->BeatBack && 
		pstAtkProcess->stBeatBackList.iBeatBackNum < 
			(int)(sizeof(pstAtkProcess->stBeatBackList.astBeatBackInfo)/sizeof(pstAtkProcess->stBeatBackList.astBeatBackInfo[0])))
	{
		z_get_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_BEATBACK, &stResult, 1);
		if (stResult.ResultVal2 != 0)
		{
			BeatBackInfo *pstBeatBackInfo;
			
			pstBeatBackInfo = &pstAtkProcess->stBeatBackList.astBeatBackInfo[pstAtkProcess->stBeatBackList.iBeatBackNum];
			pstBeatBackInfo->pstZoneSkill = z_get_player_zoneskill(pstAtkee, stResult.ResultVal2);
			if (pstBeatBackInfo->pstZoneSkill)
			{
				pstBeatBackInfo->pstBeater = pstAtkee;
				pstAtkProcess->stBeatBackList.iBeatBackNum++;	
			}
		}
	}
	
	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
}

int z_mon_hit_mon(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, Monster *pstAtkee,
						AtkProcess *pstAtkProcess)
{
	long long llTotalHurt;
	int i;
	int iHP;
	int iRealHurt, iHurt, iHeavyHit;
	MONSTERDEF *pstMonAtkeeDef;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	Player *pstOwner = NULL;
	Player *pstTrig = NULL;
	ATKCHG *pstAtkChg = NULL;
	int iNotify = 0;
	ZoneReflectedDamage *pstReflectedDamage = &pstAtkProcess->stReflectedDamage;
	int iReflect = 0;
	STATUSRESULT *pstStatusResult = NULL;

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		if (is_mon_machine(pstAtkee) && (is_mon_machine(pstMon) || pstMon->iOwnerID > 0))
		{
			Player *pstPlayer1 = machine_get_driver(pstEnv, pstAtkee);
			Player *pstPlayer2 = NULL;

			if (is_mon_machine(pstMon))
			{
				pstPlayer2 = machine_get_driver(pstEnv, pstMon);
			}
			else if (pstMon->iOwnerID > 0)
			{
				pstPlayer2 = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iOwnerID);
			}

			if (pstPlayer1 && pstPlayer1->stRoleData.MiscInfo.DartCarInfo.MonMemID > 0 && pstPlayer2)
			{
				ZoneClan *pstClan1 = player_get_clan(pstEnv, pstPlayer1);
				if (!(pstClan1 && clan_relation_by_player(pstEnv, pstPlayer2, pstClan1)< 0))
				{
					return -1;
				}
			}
		}
	}

	if (pstAtkee->bSubType >= MON_SUB_TYPE_TRAP && is_convoy_car(pstMon))
	{
		return -1;
	}

	if (pstAtkee->cStat == MONSTER_DIE && 0 == pstSkillDef->DeadTarget)
	{
		return -1;
	}

	stAtkeeInfo.stAtkee.iType = OBJ_MONSTER;
	stAtkeeInfo.stAtkee.stObj.pstMon = pstAtkee;

	pstOneHit->ID = pstAtkee->iID;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;

	pstAtkee->bTraceTry =0;

	z_get_mon_dyn(pstEnv, pstMon, pstMonDef, pstAtkerDyn);
	if(pstMon->iSkillAtkerID > 0 && !is_mon_machine(pstMon))
	{
		Player *pstRealAtker = z_id_player(pstAppCtx, pstEnv, pstMon->iSkillAtkerID);
		if(pstRealAtker)
		{
			memcpy(pstAtkerDyn, &pstRealAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
		}
	}
	
	pstAtkProcess->iAtkChgIdx = 0;
	pstAtkChg = &pstAtkerDyn->stAtkChg;

	memset (pstAtkerInfo->ucAppendResult, 0 , sizeof(pstAtkerInfo->ucAppendResult));
	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;
	
	for (i=0; i<pstAtkee->stStatusList.bBadStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstSkillDef,pstAtkerInfo, pstAtkChg, &pstAtkee->stStatusList.astBadStatus[i]);
	}
	for (i=0; i<pstAtkee->stStatusList.bGoodStatusNum; i++)
	{
		z_atkee_status_result(pstEnv, pstSkillDef,pstAtkerInfo, pstAtkChg, &pstAtkee->stStatusList.astGoodStatus[i]);
	}

	//检查自己好的状态对技能的修正
	for (i=0; i<pstMon->stStatusList.bGoodStatusNum; i++)
	{
		z_atker_status_result(pstEnv, pstSkillDef, pstAtkerInfo, pstAtkChg, &pstMon->stStatusList.astGoodStatus[i],pstAtkProcess);
	}

	z_cacl_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	z_cacl_mon_status_result(pstEnv, pstAtkerDyn, pstAtkChg, pstMon);
	
	pstMonAtkeeDef = z_get_mon_def(pstEnv, pstAtkee);
	if (NULL == pstMonAtkeeDef)
	{
		return -1;
	}

	if(0 > z_span_map_if_mon_hit_mon(pstEnv,pstMon,pstAtkee,pstMonAtkeeDef))
	{
		return -1;
	}

	if (pstMon->iOwnerID > 0)
	{
		pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iOwnerID);
		if (!pstOwner)
		{
			return -1;
		}
		if (0 == pstSkillDef->NotBootyFirstAtk)
		{
			mon_booty_first_atker(pstAppCtx, pstEnv, pstAtkee, pstOwner);
		}
	}
	else if(pstMon->iSkillAtkerID > 0)
	{
		Player *pstTmpPlayer;
		
		pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstMon->iSkillAtkerID);
		if(pstTmpPlayer && pstTmpPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			if (0 == pstSkillDef->NotBootyFirstAtk)
			{
				mon_booty_first_atker(pstAppCtx, pstEnv, pstAtkee, pstTmpPlayer);
			}
			
			if(is_player_ride_on_machine(pstTmpPlayer))
			{
				pstOwner = NULL;
			}
			else
			{
				pstOwner = pstTmpPlayer;
			}
		}
	}
	else if (pstMon->bSubType == MON_SUB_TYPE_TMP)
	{
		pstTrig = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iAtkerID);
		if (0 == pstSkillDef->NotBootyFirstAtk && pstTrig)
		{
			mon_booty_first_atker(pstAppCtx, pstEnv, pstAtkee, pstTrig);
		}

		pstOwner = pstTrig;
	}

	z_get_mon_dyn(pstEnv, pstAtkee, pstMonAtkeeDef, pstAtkeeDyn);
	z_cacl_mon_status_result(pstEnv, pstAtkeeDyn, NULL, pstAtkee);

	if (pstSkillDef->TargetNum <= MAX_SKILL_TARGET)
	{
		pstHits->TotalHit++;
	}
	else
	{
		pstOneHit = &pstHits->Hits[0];
		memset(pstOneHit, 0, sizeof(*pstOneHit));
		pstOneHit->ID = pstAtkee->iID;
		pstHits->TotalHit = 1;
		iNotify = 1;
	}

	//检查设置灰名状态
	atker_set_pktime(pstEnv, &pstAtkerInfo->stAtker, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);

	if (pstSkillDef->NoHurtSkill) //非伤害技能，只需要处理技能效果
	{
		if(pstSkillDef->NoHurtPress > 0)
		{
			ZoneAni stAni;
			RESPOS stPos;
			int iMemID;

			if(pstOwner)
			{
				stAni.iType = OBJ_PLAYER;
				stAni.stObj.pstPlayer = pstOwner;
				iMemID = pstOwner->iMemID;
				
				stPos = pstOwner->stRoleData.Pos;
			}
			else
			{
				stAni.iType = OBJ_MONSTER;
				stAni.stObj.pstMon = pstMon;
				iMemID = pstMon->iID;
				
				stPos = pstMon->stCurrPos;
			}
			
			if( z_distance(&stPos, &pstAtkee->stCurrPos) <= (int)pstMonDef->TraceDist)
			{
				if (0 > mon_press_find(pstAtkee, iMemID) )
				{
					if(pstOwner && stAni.iType == OBJ_PLAYER && !is_mon_bub(pstAtkee))
					{
						mon_press_insert_player(pstEnv, pstAtkee, pstMonDef, pstOwner);
					}
					else if (stAni.iType == OBJ_MONSTER)
					{
						mon_press_insert_mon(pstEnv, pstAtkee, pstMon);
					}
				}
				
				mon_ai_addpress(pstEnv, pstAtkee, &stAni, pstSkillDef->NoHurtPress);
			}
		}

		if ((pstSkillDef->TargetType & SKILL_TARGET_ENEMY_PLAYER) && z_is_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
			pstOneHit->TotalHurt++;
			return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
		}

		z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	llTotalHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;

	if (pstOwner)
	{
		//处理潜行
		if (pstOwner->stOnline.State & CS_STAT_QIANXIN)
		{
			z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstOwner, STATUS_ID_QIANXIN, 1, 0);
		}

		if(!is_mon_bub(pstAtkee))
		{
			if (0 > mon_press_find(pstAtkee, pstOwner->iMemID) &&
				z_distance(&pstOwner->stRoleData.Pos, &pstAtkee->stCurrPos) <= (int)pstMonAtkeeDef->TraceDist)
			{
				mon_press_insert_player(pstEnv, pstAtkee, pstMonAtkeeDef, pstOwner);
			}

			pstAtkee->iAtkerID = pstOwner->iMemID;
			pstAtkee->stAtkerPos = pstOwner->stRoleData.Pos;
			pstAtkee->cAtkerInfoNew = 1;

			// 把图腾的压力也弄进来压力表来
			if (0 > mon_press_find(pstAtkee, pstMon->iID) &&
				z_distance(&pstMon->stCurrPos, &pstAtkee->stCurrPos) <= (int)pstMonAtkeeDef->TraceDist &&
				pstAtkee != pstMon)
			{
				mon_press_insert_mon(pstEnv, pstAtkee, pstMon);
			}			
		}
	}
	else
	{
		if (0 > mon_press_find(pstAtkee, pstMon->iID) &&
			z_distance(&pstMon->stCurrPos, &pstAtkee->stCurrPos) <= (int)pstMonAtkeeDef->TraceDist &&
			pstAtkee != pstMon)
		{
			mon_press_insert_mon(pstEnv, pstAtkee, pstMon);
		}
	}

	//受攻方是否无敌状态
	if (z_is_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	// 受功放是否绝对闪避
	if (z_is_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_UTTERLY_FLEE))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	if(pstAtkee->cMonMiscFlag & MON_MISC_FLAG_STATUS_TRACE)
	{
		mon_status_beatk(pstEnv->pstAppCtx, pstEnv, pstAtkee);
	}

	pstStatusResult = mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_REFLECTION);
	if(pstMon->bSubType < MON_SUB_TYPE_TMP && pstAtkee != pstMon && pstStatusResult &&
		pstReflectedDamage->iNum < (int)(sizeof(pstReflectedDamage->astEntries)/sizeof(pstReflectedDamage->astEntries[0])))
	{
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stFrom.iType = OBJ_MONSTER;
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stFrom.stObj.pstMon = pstAtkee;

		/*
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stHit = *pstOneHit;
		*/
		
		pstReflectedDamage->astEntries[
			pstReflectedDamage->iNum].stHit.TotalHurt = 0;
		++pstReflectedDamage->iNum;

		iReflect = 1;
	}

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		//暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0 /(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}	
		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	z_del_result_by_heavyhit(pstEnv->pstAppCtx, pstEnv,NULL,pstMon,iHeavyHit);
	
	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;
	z_do_skill_attr(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		
		pstAtkProcess->iContinueCount--;

		if (iHurt <= 0)
		{
			continue;
		}

		if (iReflect)
		{
			z_hurt_reflect(pstEnv, &iHurt, pstStatusResult, pstReflectedDamage);
		}
				
		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstAtkee->iHP;
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}
		else
		{
			// 怪物伤害吸收计算
			if (0 == (pstAtkerInfo->iFlag & ATKER_INFO_FLAG_THROUGH_ABSORB))
			{
				z_mon_hurt_absorb(pstEnv, pstAtkee, pstOneHit, &iHurt);
				z_mon_hurt_status_absorb(pstEnv,pstAtkee, pstOneHit,  &iHurt);
			}
		}

		if (iHurt <= 0)
		{
			continue;
		}

		llTotalHurt += iHurt;

		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;

		pstOneHit->TotalHurt++;

		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		if (llTotalHurt >= (iHP + pstAtkee->iHP))
		{
			break;
		}
	}while (pstAtkProcess->iContinueCount > 0);

	if(is_mon_machine(pstMon))
	{
		char szName[128];	
		Player *pstMachinePlayer = z_id_player(pstAppCtx, pstEnv, pstMon->iSkillAtkerID);;

		if (pstAtkee->cMonScriptFlag & MON_SCRIPT_PROC_ATK_BY_PLAYER_MAP)
		{
			stSEnv.pstMonster = pstAtkee;
			stSEnv.pstNpc = NULL;
			stSEnv.pstPworldInst = NULL;
			stSEnv.pstPlayer = pstMachinePlayer;
			stSEnv.pstKiller = NULL;
			stSEnv.pstMapinst = NULL;
			
			snprintf(szName, sizeof(szName), "monster%d.mac", pstAtkee->iDefIdx);
			szName[sizeof(szName) - 1] = 0;
			
			z_exec_script(pstEnv->pstAppCtx, pstEnv, NULL, szName, &pstAtkee->stScriptIdx, MON_SCRIPT_PROC_ATK_BY_PLAYER);
		}

		if (pstMachinePlayer)
		{
			player_boss_info_insert(pstMachinePlayer, pstAtkee);
		}
	}

	//mon_fight_speech(pstEnv, pstAtkee, SPEECH_MON_BEHIT);
	
	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
}

int z_send_mon_hit_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, CSPKG *pstPkg)
{
	//int i;
	//Player *pstPlayerTmp;
	//Player *apstPlayerHit[MAX_SKILL_TARGET];
	//int iPlayerHit = 0;
	//Player *apstPlayerNotify[VISIBLE_ANIMATE_VALVE + MAX_SKILL_TARGET];
	//int iPlayerNotify = 0;
	//CSHITS *pstHits = &pstPkg->Body.ActionRes.Data.Hit;
	//CSONEHIT *pstOneHit;
	ZONEMAPINST *pstMapInst;
	//SearchArea stSArea;
	//MonAtkInfo *pststMonAtkInfo = &pstMon->stMonAtkInfo;
	//SKILLDEF* pstSkillDef;

	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &pstMon->stMap);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	if (pstMapInst->bBroadcast || 
		(pstMon->cMonMiscFlag & (MON_MISC_FLAG_IMPVIEW|MON_MISC_FLAG_MAPBROAD)))
	{
		return z_sendpkg_mon_area_view(pstAppCtx, pstEnv, pstMon, pstPkg);
	}
	else
	{
		z_sendpkg_monview(pstAppCtx, pstEnv, pstMon, pstPkg, 1);
	}


/*
	for (i=0; i<pstHits->TotalHit; i++)
	{
		pstOneHit = pstHits->Hits+i;
		pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstOneHit->ID);
		if (pstPlayerTmp)
		{
			apstPlayerHit[iPlayerHit++] = pstPlayerTmp;
			pstPlayerTmp->stOnline.cMiscFlag &= ~MISC_FLAG_TAG;
		}
	}
	
	if (z_pos_in_view_half(&pstMon->stCurrPos, &pstMon->stMonAtkInfo.stAtkPos)) //以主动攻击者视野为准
	{
		for (i = 0; i < pstMon->iVPlayer; i++)
		{
			if (iPlayerNotify >= VISIBLE_ANIMATE_VALVE)
			{
				break;
			}
			
			pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstMon->aiVPlayer[i]);
			if (NULL == pstPlayerTmp) continue;
			
			apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
			pstPlayerTmp->stOnline.cMiscFlag |= MISC_FLAG_TAG;
		}
	}
	else //以打击点为准
	{
		if(0 == z_pos_search_area(pstAppCtx, pstEnv, pstMapInst, &pstMon->stMonAtkInfo.stAtkPos, &stSArea))
		{
			int j;
			AREADYN *pstArea;
			
			for(i = 0; i < stSArea.iNum; i++)
			{
				if (iPlayerNotify >= VISIBLE_ANIMATE_VALVE)
				{
					break;
				}
				
				pstArea = stSArea.pastArea[i];

				for (j = 0; j < pstArea->iAnimate; j++)
				{
					if (iPlayerNotify >= VISIBLE_ANIMATE_VALVE)
					{
						break;
					}
					
					pstPlayerTmp = z_id_player(pstAppCtx, pstEnv, pstArea->aiAnimate[j]);
					if (NULL == pstPlayerTmp || PLAYER_STATUS_ROLE_LOGIN != pstPlayerTmp->eStatus)
					{
						continue;
					}

					if (pstArea->iAnimate > VISIBLE_ANIMATE_VALVE && 
						!z_pos_in_view_half(&pstMon->stCurrPos, &pstPlayerTmp->stRoleData.Pos))
					{
						continue;
					}

					apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
					pstPlayerTmp->stOnline.cMiscFlag |= MISC_FLAG_TAG;
				}
			}
		}
	}
	
	for (i=0; i<iPlayerHit; i++)
	{
		pstPlayerTmp = apstPlayerHit[i];
		if (0 == (pstPlayerTmp->stOnline.cMiscFlag & MISC_FLAG_TAG))
		{
			apstPlayerNotify[iPlayerNotify++] = pstPlayerTmp;
		}
	}

	z_cltmsg_limit_broadcast(pstAppCtx, pstEnv, apstPlayerNotify, iPlayerNotify, pstPkg);
*/

	return 0;
}

int z_pet_tishen_select(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int i;
	int iTishen;
	ROLEPET *pstRolePet;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;

	for (i=0; i<pstPetOnline->bFightShiNum; i++)
	{
		pstRolePet = z_find_role_pet(pstPlayer, pstPetOnline->astFightShi[i].stRolePetOnline.ullWID, NULL);
		if (NULL == pstRolePet)
		{
			continue;
		}
		// 被动模式的宠物不受替身率影响
		if(pstRolePet->Passive)
		{
			continue;
		}

		iTishen = pstRolePet->FightAttr.YongQi*100.0/(pstRolePet->FightAttr.YongQi*5+200);
		
		if ((int)(RAND1(10000)) < iTishen*100)
		{
			return pstPetOnline->astFightShi[i].iXieDaiPos;
		}
	}

	return -1;
	UNUSED(pstEnv);	//add by paraunused.pl
}

/* 每次攻击均尝试替身，替身只替当前攻击。*/
PetFightShiOnline *z_pet_tishen(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iXieDaiPos;
	PetFightShiOnline *pstPetFightShiOnline;
	PetOnline *pstPetOnline = &pstPlayer->stOnline.stPetOnline;
	
	if (0 == pstPetOnline->bFightShiNum)
	{
		return NULL;
	}

	iXieDaiPos = z_pet_tishen_select(pstEnv, pstPlayer);

	if (0 > iXieDaiPos)
	{
		return NULL;
	}

	pstPetFightShiOnline = pet_get_shipet_by_xiedaipos(pstPlayer, iXieDaiPos);

	return pstPetFightShiOnline;
}

int z_mon_hit_pet(ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, ZonePet *pstZonePet, AtkProcess *pstAtkProcess)
{
	int iHP;
	long long llTotalHurt;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	int iRealHurt, iHurt, iHeavyHit;
	ATKCHG *pstAtkChg = NULL;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	Player *pstOwner = pstZonePet->pstOwner;
	int iNotify = 0;

	if (0 == (PET_STAT_FIGHT & pstZonePet->pstRolePet->StatFlag))
	{
		return -1;
	}
		
	stAtkeeInfo.stAtkee.iType = OBJ_PET;
	stAtkeeInfo.stAtkee.stObj.stZonePet = *pstZonePet;
	
	pstOneHit->ID = pstOwner->iMemID+(pstZonePet->pstPetFightShi->iXieDaiPos+1)*PET_ID_BASE;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;

	pstOwner->stOnline.iAtkerTarget = pstMon->iID;
	//pstOneHit->TiShenFlag = pstZonePet->pstPetFightShi->cTiShenFlag;
		
	z_get_mon_dyn(pstEnv, pstMon, pstMonDef, pstAtkerDyn);
	if(pstMon->iSkillAtkerID > 0 && !is_mon_machine(pstMon))
	{
		Player *pstRealAtker = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iSkillAtkerID);
		if(pstRealAtker)
		{
			memcpy(pstAtkerDyn, &pstRealAtker->stOnline.stDyn, sizeof(*pstAtkerDyn));
		}
	}
	
	pstAtkProcess->iAtkChgIdx = 0;
	pstAtkChg = &pstAtkerDyn->stAtkChg;

	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;
	z_cacl_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	z_cacl_mon_status_result(pstEnv, pstAtkerDyn, pstAtkChg, pstMon);

	z_get_pet_dyn(pstEnv, pstOwner, pstAtkeeDyn, pstZonePet->pstRolePet->WID);

	if (pstSkillDef->TargetNum <= MAX_SKILL_TARGET)
	{
		pstHits->TotalHit++;
	}
	else
	{
		pstOneHit = &pstHits->Hits[0];
		memset(pstOneHit, 0, sizeof(*pstOneHit));
		pstOneHit->ID = pstOwner->iMemID+(pstZonePet->pstPetFightShi->iXieDaiPos+1)*PET_ID_BASE;
		pstHits->TotalHit = 1;
		iNotify = 1;
	}

	atker_set_pktime(pstEnv, &pstAtkerInfo->stAtker, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);

	if (pstSkillDef->NoHurtSkill) //非伤害技能，只需要处理技能效果
	{
		z_do_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	llTotalHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;

	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
	}

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		//暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0 /(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}	
		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	z_del_result_by_heavyhit(pstEnv->pstAppCtx, pstEnv,NULL,pstMon,iHeavyHit);
	
	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;
	z_do_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		
		pstAtkProcess->iContinueCount--;
		
		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstZonePet->pstRolePet->HP;
			
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}

		if (iHurt <= 0)
		{
			continue;
		}
				
		llTotalHurt += iHurt;

		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;

		pstOneHit->TotalHurt++;

		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		if (llTotalHurt >= (iHP + pstZonePet->pstRolePet->HP))
		{
			break;
		}
	}while (pstAtkProcess->iContinueCount > 0);

	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, iNotify);
}

int z_beat_beak(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, BeatBackInfo *pstBeatBackInfo, ZoneAni *pstAtkee)
{
	SKILLDEF *pstSkillDef;	
	ZoneObjList stObjList;	
	CSSKILLATK stCSSkillAtk;
	int iAtkeeID = 0;
	int iAtkChgIdx;
	RESPOS stPos;
	int iSkillDist;
	Player *pstAtker = pstBeatBackInfo->pstBeater;	
	ZoneSkill *pstZoneSkill = pstBeatBackInfo->pstZoneSkill;
	int iAtkeeRadius = 0;

	if (pstAtker->stOnline.State & CS_STAT_DEAD)
	{
		return -1;
	}
	
	switch(pstAtkee->iType)
	{
		case OBJ_MONSTER:
			if (pstAtkee->stObj.pstMon->cStat == MONSTER_DIE)
			{
				return -1;
			}

			iAtkeeID = pstAtkee->stObj.pstMon->iID;
			stPos = pstAtkee->stObj.pstMon->stCurrPos;

			iAtkeeRadius = pstAtkee->stObj.pstMon->unRadius;
			break;
		case OBJ_PLAYER:

			if (pstAtkee->stObj.pstPlayer->stOnline.State & CS_STAT_DEAD)
			{
				return -1;
			}
			
			iAtkeeID = pstAtkee->stObj.pstPlayer->iMemID;
			stPos = pstAtkee->stObj.pstPlayer->stRoleData.Pos;

			iAtkeeRadius = 0;
			break;
		default:
			return -1;
			break;
	}

	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);	
	if (pstSkillDef)	
	{		
		iSkillDist = pstSkillDef->Distance + 2*DIST_TORRENT;
		if( z_distance(&stPos, &pstAtker->stRoleData.Pos) > iSkillDist + iAtkeeRadius)
		{
			return -1;
		}
		
		stObjList.iObj = 1;		
		stObjList.astObj[0] = *pstAtkee;		
		stCSSkillAtk.SkillSeq = 0;		
		stCSSkillAtk.RolePos = pstAtker->stRoleData.Pos;		
		stCSSkillAtk.SvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec;		
		stCSSkillAtk.SvrTime.BenchTimeUSec = pstAppCtx->stCurr.tv_usec;		
		stCSSkillAtk.AtkPos = pstAtker->stRoleData.Pos;		
		stCSSkillAtk.SkillID = pstSkillDef->SkillID;		
		//stCSSkillAtk.FittingsIdx = 0;		
		stCSSkillAtk.TargetNum = 1;		
		stCSSkillAtk.TargetIDs[0] = 	iAtkeeID;		

		if (0 > z_check_skill_arm(pstEnv, pstAtker, pstZoneSkill, pstSkillDef, &iAtkChgIdx))
		{
			iAtkChgIdx = -1;
		}

		z_player_hit_notify(pstAppCtx, pstEnv, pstAtker, /*&pstZoneSkill->stSkillData.CSFittingsRowList[0],*/ 
				pstZoneSkill, pstSkillDef, pstSkillDef, &stObjList, &stCSSkillAtk, iAtkChgIdx /*,0*/);	
	}
	
	return 0;
}

int make_hit_res(ZONESVRENV* pstEnv, AtkProcess *pstAtkProc, CSONEHIT *pstOneHitArry, int iHitNum, CSPKG *pstPkg)
{
	CSACTIONRES *pstActionRes;
	AtkerInfo *pstAtkerInfo = &pstAtkProc->stAtkerInfo;
	Monster *pstMon;
	Player *pstPlayer;
	AtkInfo *pstAtkInfo;
	LightningAtkInfo *pstLightningAtkInfo;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	ZonePet *pstZonePet;
	
	pstActionRes = &pstPkg->Body.ActionRes;
	memset(pstActionRes,0,sizeof(CSACTIONRES));	

	pstActionRes->ActionID = ACTION_HIT;
	if (pstAtkerInfo->pstOriginDef)
	{
		pstActionRes->Data.Hit.SkillID =  pstAtkerInfo->pstOriginDef->SkillID;
		pstActionRes->Data.Hit.Level = pstAtkerInfo->pstOriginDef->Level;
	}
	else
	{
		pstActionRes->Data.Hit.SkillID =  pstSkillDef->SkillID;
		pstActionRes->Data.Hit.Level = pstSkillDef->Level;
	}
	
	if (pstAtkProc->stDeadList.iSelf)
	{
		pstActionRes->Data.Hit.SelfDead = 1;
	}
	
	pstActionRes->Data.Hit.RuneTransferedID = pstSkillDef->SkillID;
	pstActionRes->Data.Hit.TotalHit = iHitNum;
	memcpy(pstActionRes->Data.Hit.Hits, pstOneHitArry, iHitNum*sizeof(pstActionRes->Data.Hit.Hits[0]));

	if (pstAtkerInfo->SelfPosChgFlag)
	{
		pstActionRes->Data.Hit.SelfPosChgFlag = 1;
		pstActionRes->Data.Hit.SelfPosChg = pstAtkerInfo->SelfPosChg;
	}

	switch(pstAtkerInfo->stAtker.iType)
	{
		case OBJ_PLAYER:
			pstPlayer = pstAtkerInfo->stAtker.stObj.pstPlayer;
			pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
			pstLightningAtkInfo = &pstPlayer->stOnline.stLightningAtkInfo[pstAtkInfo->iAtkLightningIdx];

			pstActionRes->ID = pstPlayer->iMemID;
			pstActionRes->Data.Hit.SkillSeq = pstAtkProc->stHits.SkillSeq;
			pstActionRes->Data.Hit.AtkPos = pstAtkerInfo->stAtkPos;

			if(pstSkillDef->LightningLinkNum > 0)
			{
				if(pstAtkInfo->iAtkFlagBit&ATK_FLAG_BIT_SVR_LIGHTNING)
				{
					if( pstLightningAtkInfo->iJumpNum+1 < MAX_LIGHTNING_JUMP_NUM )
					{
						pstActionRes->Data.Hit.FromID = pstLightningAtkInfo->iMemID[pstLightningAtkInfo->iJumpNum+1];
					}
				}
			}

			if (pstPlayer->stOnline.stBhlingOl.bFlag )
			{
				pstActionRes->Data.Hit.Flags = HIT_FLAG_BEIHOULING;
			}
			break;
		case OBJ_MONSTER:
			pstMon = pstAtkerInfo->stAtker.stObj.pstMon;

			pstActionRes->ID = pstMon->iID;
			pstActionRes->Data.Hit.AtkPos = pstMon->stMonAtkInfo.stAtkPos;
			pstActionRes->Data.Hit.DirFace = pstMon->stMonAtkInfo.DirFace;
			pstActionRes->Data.Hit.SkillSeq = 0;
			if(is_mon_machine(pstMon))
			{
				pstActionRes->Data.Hit.RealMemID = pstMon->iSkillAtkerID;

				pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iSkillAtkerID);
				if(pstPlayer)
				{
					pstActionRes->Data.Hit.SkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq;
				}
			}
			break;
		case OBJ_PET:
			pstZonePet = &pstAtkerInfo->stAtker.stObj.stZonePet;

			pstActionRes->ID = pstZonePet->pstOwner->iMemID + 
								(1+get_pet_shi_xiedaipos(pstEnv, pstZonePet->pstOwner, pstZonePet->pstRolePet))*PET_ID_BASE;
			break;
		default:
			break;
	}

	if (pstAtkerInfo->pstStatusSkillAtker) //状态触发技能要被施加者看到
	{
		//pstActionRes->ID = pstAtkerInfo->pstStatusSkillAtker->iMemID;
		pstActionRes->Data.Hit.SkillSeq = 0;
	}
	pstActionRes->Data.Hit.FromPos = pstAtkProc->stAtkerInfo.stRolePos;

	Z_CSHEAD_INIT(&pstPkg->Head, ACTION_RES);
	
	return 0;
	UNUSED(pstEnv);	//add by paraunused.pl
}

int z_mon_hit_notify(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster *pstMon, MONSTERDEF *pstMonDef, SKILLDEF *pstSkillDef,
						ZoneObjList *pstList)
{
	int i;
	AtkProcess stAtkProc;
	ZoneAni *pstAni;
	CSPKG stPkg;
	ZoneOneDead *pstDead;
	MONSTERDEF *pstMonOtherDef;
	CSHITS *pstHits = &stAtkProc.stHits;
	ZoneDeadList *pstZoneDeadList = &stAtkProc.stDeadList;
	Player *pstTrig;
	Monster *pstDeadMon;
	Player *pstOwner;
	PetFightShiOnline *pstPetTiShen;
	STATUSDEF * pstStatusDef = NULL;
	int iApplyID = 0;
	SKILLRESULT *pstSkillResult = NULL;

	memset(&stAtkProc, 0, sizeof(stAtkProc));

	pstHits->SkillID = pstSkillDef->SkillID;
	stAtkProc.stAtkerInfo.stAtker.iType = OBJ_MONSTER;
	stAtkProc.stAtkerInfo.stAtker.stObj.pstMon = pstMon;
	stAtkProc.stAtkerInfo.pstZoneSkill = NULL;
	//stAtkProc.stAtkerInfo.pstFittings = NULL;
	stAtkProc.stAtkerInfo.pstSkillDef = pstSkillDef;
	stAtkProc.stAtkerInfo.stAtkPos = pstMon->stMonAtkInfo.stAtkPos;
	stAtkProc.pstSkillArmChg = NULL;
	stAtkProc.bContinueNum = pstSkillDef->ContinueNum;
	//取下服务时间
	stAtkProc.stAtkerInfo.SvrTime.BenchTimeSec = pstAppCtx->stCurr.tv_sec;
	stAtkProc.stAtkerInfo.SvrTime.BenchTimeUSec = pstAppCtx->stCurr.tv_usec;

	if(pstMon->iSkillAtkerID && !is_mon_machine(pstMon))
	{
		Player *pstTmpPlayer;
		
		pstTmpPlayer = z_id_player(pstAppCtx, pstEnv, pstMon->iSkillAtkerID);
		if(pstTmpPlayer && pstTmpPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			stAtkProc.stAtkerInfo.pstStatusSkillAtker = pstTmpPlayer;
		}
	}

	pstSkillResult = z_skill_get_result(pstSkillDef, RESULT_SELF_POSCHG);
	if (pstSkillResult)
	{
		RESPOS stPos;
		MapIndex *pstMapIdx;
		ZONEMAPINST *pstMapInst;
		ZONEIDX stIdx;

		stIdx.iID = pstMon->stMap.iID;
		stIdx.iPos = pstMon->stMap.iPos;
		pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
		if (pstMapInst)
		{
			int iDist = pstSkillResult->SkillResultVal1;
			
			//iDist = get_real_self_poschg(pstEnv, pstSkillDef, pstMon->iID, pstSkillResult->SkillResultVal1);

			pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
			if (0 == z_skill_result_poschg_by_mon(pstEnv, pstMapInst, pstMapIdx, pstSkillDef, iDist, pstMon, 
				&pstMon->stMonAtkInfo.stAtkPos, &pstMon->stCurrPos, &stPos))
			{
				stAtkProc.stAtkerInfo.SelfPosChg = stPos;
				stAtkProc.stAtkerInfo.SelfPosChgFlag = 1;
			}
		}
	}

	for (i=0; i<pstList->iObj; i++)
	{
		pstAni = &pstList->astObj[i];

		switch(pstAni->iType)
		{
			case OBJ_PLAYER:
				pstOwner = pstAni->stObj.pstPlayer;
				pstPetTiShen = z_pet_tishen(pstEnv, pstOwner);
				if (	NULL == pstPetTiShen ||
					!is_tishen_skill(pstSkillDef) ||
					is_pet_heti(pstOwner))
				{
					z_mon_hit_player(pstAppCtx, pstEnv, pstMon, pstMonDef, pstOwner, &stAtkProc);
				}
				else
				{
					pstOwner->stOnline.fPetShouHu = 1;
					z_mon_hit_player(pstAppCtx, pstEnv, pstMon, pstMonDef, pstOwner, &stAtkProc);
					pstOwner->stOnline.fPetShouHu = 0;
					
					/*
					pstAni->iType = OBJ_PET;
					pstAni->stObj.stZonePet.pstOwner = pstOwner;
					pstAni->stObj.stZonePet.pstPetFightShi = pstPetTiShen;
					pstAni->stObj.stZonePet.pstRolePet = z_find_role_pet(pstOwner, pstPetTiShen->stRolePetOnline.ullWID, NULL);
					if (NULL == pstAni->stObj.stZonePet.pstRolePet )
					{
						z_mon_hit_player(pstAppCtx, pstEnv, pstMon, pstMonDef, pstOwner, &stAtkProc);
						
					}
					else
					{
						pstAni->stObj.stZonePet.pstPetFightShi->cTiShenFlag = 1;
						z_mon_hit_pet(pstEnv, pstMon, pstMonDef, &pstAni->stObj.stZonePet, &stAtkProc);
						pstAni->stObj.stZonePet.pstPetFightShi->cTiShenFlag = 0;
					}
					*/
				}

				z_player_byatk_clear_status(pstEnv, pstAni->stObj.pstPlayer);

				//判断战斗状态
				switch(pstSkillDef->FightStat)
				{
					case SKILL_FIGHT_STAT_BOTH:
						SET_PLAYER_FIGHT(pstEnv, pstOwner, pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						break;
					default:
						break;
				}
				
				break;
				
			case OBJ_MONSTER:
				z_mon_hit_mon(pstAppCtx, pstEnv, pstMon, pstMonDef, pstAni->stObj.pstMon, &stAtkProc);

				if(is_mon_machine(pstAni->stObj.pstMon) && pstSkillDef->FightStat == SKILL_FIGHT_STAT_BOTH)
				{
					machine_set_fight( pstEnv, pstAni->stObj.pstMon);
				}
				
				break;

			case OBJ_PET:
				z_mon_hit_pet(pstEnv, pstMon, pstMonDef, &pstAni->stObj.stZonePet, &stAtkProc);
				break;
			case OBJ_NPC:
				z_mon_hit_mon(pstAppCtx, pstEnv, pstMon, pstMonDef, &pstAni->stObj.pstNpc->stMon, &stAtkProc);
				break;
			default:
				break;
		}
	}

	//无目标技能效果
	z_do_no_target_skill_attr(pstAppCtx, pstEnv, &stAtkProc);

	if (pstList->iObj > 0 && pstMon->iOwnerID > 0 && pstSkillDef->FightStat == SKILL_FIGHT_STAT_BOTH)
	{
		if(pstMon->iOwnerID > 0)
		{
			Player *pstMonOwner;
			
			pstMonOwner = z_id_player(pstAppCtx, pstEnv, pstMon->iOwnerID);
			if (pstMonOwner && pstMonOwner->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				SET_PLAYER_FIGHT(pstEnv, pstMonOwner, pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
			}
		}
		else if(is_mon_machine(pstMon))
		{
			machine_set_fight( pstEnv, pstMon);
		}
	}

	if (pstSkillDef->TargetNum <= MAX_SKILL_TARGET)
	{
		make_hit_res(pstEnv, &stAtkProc, pstHits->Hits, pstHits->TotalHit, &stPkg);
		z_send_mon_hit_res(pstAppCtx, pstEnv, pstMon, &stPkg);
	}

	//技能消耗结算
	z_mon_skill_consume(pstEnv, pstMon, &stAtkProc.stAtkerInfo.stResourceConsume);

	if (!pstZoneDeadList->iSelf && stAtkProc.stReflectedDamage.iNum > 0)
	{
		z_mon_be_reflect(pstEnv, pstMon, &stAtkProc);
	}
	
	//处理宠物死亡
	z_pet_beatk_die(pstEnv, pstHits);

	//死亡列表处理
	for (i=0; i<pstZoneDeadList->iCount; i++)
	{
		pstDead = &pstZoneDeadList->astDeads[i];
		switch(pstDead->stAtkee.iType)
		{
			case OBJ_PLAYER:
				player_die(pstAppCtx, pstEnv, pstDead->stAtkee.stObj.pstPlayer, &pstDead->stAtker, 0);
				break;
			case OBJ_MONSTER:
				pstDeadMon = pstDead->stAtkee.stObj.pstMon;
				pstTrig = NULL;
				if (pstMon->bSubType == MON_SUB_TYPE_TMP)
				{
					pstTrig = z_id_player(pstAppCtx, pstEnv, pstMon->iAtkerID);
					if (pstTrig)
					{
					/*
						if (MISC_FLAG_TRACE & pstTrig->stOnline.cMiscFlag)
						{
							task_trace_mon(pstEnv, pstTrig, pstDeadMon->iDefIdx);
						}
					*/

						pstDead->stAtker.iType = OBJ_PLAYER;
						pstDead->stAtker.stObj.pstPlayer = pstTrig;
					}
				}
				
				pstMonOtherDef = z_get_mon_def(pstEnv, pstDeadMon);
				if (pstMonOtherDef)
				{
					z_mon_die(pstAppCtx, pstEnv, pstDeadMon, pstMonOtherDef, &pstDead->stAtker, 0, 1);
				}

				break;
			default:
				break;
		}
	}

	
	{
		Player *pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->ullPKID);
		if (pstOwner && player_is_in_tank_pworld(pstEnv, pstOwner))
		{
			poly_tank_pworld_tip1(pstEnv, pstOwner, pstZoneDeadList->iCount);		
		}
	}

	if (pstZoneDeadList->iSelf)
	{
		pstDead = &pstZoneDeadList->stSelfDead;
		z_mon_die(pstAppCtx, pstEnv, pstMon, pstMonDef, &pstDead->stAtker, 0, 1);
	}
	else
	{
		for (i=0; i<MAX_SKILL_RESULT; i++) //特殊处理对自身施加状态
		{
			if (0 == pstSkillDef->SkillResult[i].SkillResultID)
			{
				break;
			}

			if (RESULT_SELF_STATUS == pstSkillDef->SkillResult[i].SkillResultID)
			{
				if ((int)(RAND1(10000)) >= pstSkillDef->SkillResult[i].SkillResultVal1*100.0)
				{
					continue;
				}
				pstStatusDef = z_find_status(pstEnv, pstSkillDef->SkillResult[i].SkillResultVal2, pstSkillDef->SkillResult[i].SkillResultVal3);
				if(pstStatusDef)
				{
					iApplyID = z_check_reflect_status(pstStatusDef,pstMon->stAtkTargets.iTargetCur);
				}
				
				z_mon_apply_status(pstAppCtx, pstEnv, pstMon, pstSkillDef->SkillResult[i].SkillResultVal2, 
										pstSkillDef->SkillResult[i].SkillResultVal3, pstSkillDef->SkillResult[i].SkillResultVal4, 1, iApplyID);

			}
		}
	}

	//地雷攻击后要消失
	if (MON_SUB_TYPE_TRAP == pstMon->bSubType)
	{
		ZoneAni stAni;
		
		pstMon->cFirstAtkerType = 0;
		pstMon->ullFirstAtkerWid = 0;
		pstMon->iFirstAtkerID = 0; //没有战利品
		pstMon->iFirstAtkerMemID = 0;
		pstMon->szOwner[0] = 0;
		pstMon->cRefresh = 0;

		stAni.iType = OBJ_UNKNOW;
		
		z_mon_die(pstAppCtx, pstEnv, pstMon, pstMonDef, &stAni, 1, 1);
	}
	else if (pstMon->cStat == MONSTER_ALIVE &&
		(z_skill_get_result(pstSkillDef, RESULT_DESTROY) || z_skill_get_result(pstSkillDef, RESULT_SACRIFICE)))
	{
		z_mon_destroy(pstEnv, pstMon);
	}

	//要在反击类攻击的前面结束
	if(stAtkProc.cQieCuoEnd && pstMon->iOwnerID > 0)
	{
		Player *pstQiecuoer;
		Player *pstOwner;

		//主人
		pstOwner = z_id_player(pstAppCtx, pstEnv, pstMon->iOwnerID);
		if(pstOwner)
		{
			//对手
			pstQiecuoer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstOwner->stOnline.stQiecuoInfo.iMemID);
			if(pstQiecuoer)
			{
				z_player_qiecuo_end(pstEnv, pstQiecuoer, 0);
			}

			z_player_qiecuo_end(pstEnv, pstOwner, 1);
		}
	}
	
	if (!stAtkProc.cQieCuoEnd && pstMon->cStat == MONSTER_ALIVE)
	{
		//反击
		if (pstMon->bSubType < MON_SUB_TYPE_TMP)
		{
			for (i=0; i<stAtkProc.stBeatBackList.iBeatBackNum; i++)
			{
				z_beat_beak(pstAppCtx, pstEnv, &stAtkProc.stBeatBackList.astBeatBackInfo[i], &stAtkProc.stAtkerInfo.stAtker);
				if (pstMon->cStat != MONSTER_ALIVE) break;
			}
		}
	}

	if (pstMon->cStat == MONSTER_ALIVE)
	{
		for(i=0; i<pstList->iObj; i++)
		{
			if (pstList->astObj[i].iType == OBJ_PLAYER)
			{
				ZoneAni stAni;

				if (pstMon->cType == OBJ_MONSTER)
				{
					stAni.iType = OBJ_MONSTER;
					stAni.stObj.pstMon = pstMon;
					player_target_totem(pstEnv, pstList->astObj[i].stObj.pstPlayer, &stAni);
				}
				else if(pstMon->cType == OBJ_NPC)
				{
					Npc *pstNpc = z_id_npc(pstAppCtx, pstEnv, pstMon->iID);
					if (pstNpc)
					{
						stAni.iType = OBJ_NPC;
						stAni.stObj.pstNpc = pstNpc;
						player_target_totem(pstEnv, pstList->astObj[i].stObj.pstPlayer, &stAni);
					}
				}
			}
		}
	}

	//处理怪物ai 触发
	//if (pstMon->iOwnerID > 0)
	{
		for (i=0; i<stAtkProc.stAiList.iCount; i++)
		{
			mon_ai_stat(pstEnv, stAtkProc.stAiList.apstMon[i], &stAtkProc.stAiList.astMonAi[i]);
		}
	}

	return 0;
}

//服务器主动获取攻击列表
int player_atk_svr_get_list(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, SKILLDEF *pstSkillDef, CSSKILLATK *pstSkillAtk)
{
	int i;
	CSSKILLATK stHit;
	ZoneObjList stZoneObjList;
	pstSkillAtk->TargetNum = 0;
	stHit.AtkPos = pstPlayer->stRoleData.Pos;
	stHit.RolePos = pstPlayer->stRoleData.Pos;

	//自己也算进去
	stHit.TargetNum = 1;
	stHit.TargetIDs[0] = pstPlayer->iMemID;
	z_player_atklist(pstAppCtx, pstEnv, pstPlayer, pstSkillDef, &stHit, &stZoneObjList, &pstAppCtx->stCurr);
	if (stZoneObjList.iObj > 0)
	{
		if (pstSkillAtk->TargetNum >= pstSkillDef->TargetNum)
		{
			return 0;
		}
		
		pstSkillAtk->TargetIDs[pstSkillAtk->TargetNum] = stHit.TargetIDs[0];
		pstSkillAtk->TargetNum++;
	}
		
	for (i=0; i<pstPlayer->stOnline.iVAnimate; i++)
	{
		stHit.TargetNum = 1;
		stHit.TargetIDs[0] = pstPlayer->stOnline.aiVAnimate[i];
		z_player_atklist(pstAppCtx, pstEnv, pstPlayer, pstSkillDef, &stHit, &stZoneObjList, &pstAppCtx->stCurr);
		if (stZoneObjList.iObj > 0)
		{
			if (pstSkillAtk->TargetNum >= pstSkillDef->TargetNum)
			{
				return 0;
			}
			
			pstSkillAtk->TargetIDs[pstSkillAtk->TargetNum] = stHit.TargetIDs[0];
			pstSkillAtk->TargetNum++;
		}
	}

	

	return 0;
	UNUSED(pstZoneSkill);	//add by paraunused.pl
}

//服务器主动攻击
int player_atk_by_svr(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillID, int iAtkerID)
{
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef;
	int iOldSkillSeq;
	TFRAMEHEAD stFrameHead;
	CSPKG stPkg;
	struct timeval stTmp;
	struct timeval stTmpCommCool;
	CSSKILLATK *pstSkillAtk = &stPkg.Body.SkillAtk;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	AtkInfo stAtkInfoTmp;
	int StateTmp = 0;

	//嵌套了
	if (ATK_FLAG_BIT_SVR_ATK & pstAtkInfo->iAtkFlagBit)
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
				
				if(pstAtkerPlayer->stRoleData.Map != pstPlayer->stRoleData.Map)
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

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, iSkillID);
	if (NULL == pstZoneSkill)
	{
		return -1;
	}

	//检查自我冷却
	if(0 > z_compare_time(&pstZoneSkill->stCoolEnd, &pstAppCtx->stCurr))
	{
		return -1;
	}
	
	pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
	if (NULL == pstSkillDef)
	{
		return -1;
	}
	
	iOldSkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq;
	stFrameHead.TimeStamp.TimeVal.sec = pstEnv->pstAppCtx->stCurr.tv_sec;
	stFrameHead.TimeStamp.TimeVal.usec = pstEnv->pstAppCtx->stCurr.tv_usec;
	
	pstSkillAtk->SkillID = iSkillID;
	//pstSkillAtk->SkillSeq = iOldSkillSeq+1;
	pstSkillAtk->SkillSeq = 0;//客户端有特殊处理,不管这个序号
	pstSkillAtk->RolePos = pstPlayer->stRoleData.Pos;
	pstSkillAtk->AtkPos = pstPlayer->stRoleData.Pos;

	pstSkillAtk->SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstSkillAtk->SvrTime.BenchTimeUSec = pstEnv->pstAppCtx->stCurr.tv_usec;
	//pstSkillAtk->FittingsIdx = 0;
	pstPlayer->stOnline.iSkillAtkerID = iAtkerID;

	//先存下
	memcpy(&stAtkInfoTmp,pstAtkInfo,sizeof(AtkInfo));
	StateTmp = pstPlayer->stOnline.State;

	player_atk_svr_get_list(pstAppCtx, pstEnv, pstPlayer, pstZoneSkill, pstSkillDef, pstSkillAtk);
	if (pstSkillAtk->TargetNum <= 0)
	{
		goto _error;
	}

	stTmp = pstPlayer->stOnline.stLastCltTime;
	memset(&pstPlayer->stOnline.stLastCltTime, 0, sizeof(pstPlayer->stOnline.stLastCltTime));
	stTmpCommCool= pstPlayer->stOnline.stSkillList.stCommCoolEnd;
	pstAtkInfo->iAtkFlagBit |= ATK_FLAG_BIT_SVR_ATK;		
	player_atk(pstAppCtx, pstEnv, pstPlayer, &stFrameHead, &stPkg);
	memcpy(pstAtkInfo,&stAtkInfoTmp,sizeof(AtkInfo));
	if(!(pstPlayer->stOnline.State&CS_STAT_DEAD))
	{
		pstPlayer->stOnline.State = StateTmp;
	}

	pstAtkInfo->iAtkFlagBit = 0;
	
	pstPlayer->stOnline.stLastCltTime = stTmp;
	pstPlayer->stOnline.stSkillList.stCommCoolEnd = stTmpCommCool;

_final:

	pstPlayer->stOnline.stAtkInfo.iSkillSeq = iOldSkillSeq;
	pstPlayer->stOnline.iSkillAtkerID = 0;
	return 0;
	
_error:
	
	goto _final;
	return 0;
}
int z_find_lightning_memid(LightningAtkInfo *pstLightningAtkInfo,int iMemID)
{
	int z = 0;
	
	for(z=0;z<MAX_LIGHTNING_JUMP_NUM;z++)
	{
		if(pstLightningAtkInfo->iMemID[z]==iMemID)
		{
			break;
		}
	}
	if(z < MAX_LIGHTNING_JUMP_NUM)
	{
		return 0;
	}

	return -1;
}
//服务器闪电链攻击
int player_atk_by_lightning(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ZoneSkill *pstZoneSkill;
	SKILLDEF *pstSkillDef;
	int iOldSkillSeq;
	TFRAMEHEAD stFrameHead;
	CSPKG stPkg;
	struct timeval stTmp;
	struct timeval stTmpCommCool;
	CSSKILLATK *pstSkillAtk = &stPkg.Body.SkillAtk;
	int i=0,j=0;
	PlayerOnline *pststOnline = &pstPlayer->stOnline;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	AtkInfo stAtkInfoTmp;
	LightningAtkInfo *pstLightningAtkInfo = NULL;
	int iDistance = 0;
	ZoneAni stAni;
	RESPOS AtkPos;  
	int iID = 0;
	int iIndex = 0;
	char cType = 0;
	TMEMBLOCK *pstMemBlock;
	Player *pstPlayerView =NULL;
	RESPOS  stLastPos;
	Monster *pstMon	= NULL;
	int TargetIDs = 0;
	int TargetIDsTmp = 0;
	int iJumpNum = 0;
	int StateTmp = 0;

	for( i=0; i<MAX_LIGHTNING_SKILL_NUM; i++)
	{
		if( pststOnline->stLightningAtkInfo[i].iJumpNum <= 0 )
		{
			continue;
		}
		pstLightningAtkInfo = &pststOnline->stLightningAtkInfo[i];
	
		pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstLightningAtkInfo->ushSkillID);
		if (NULL == pstZoneSkill)
		{
			continue;
		}
		pstSkillDef = z_find_skill_zoneskill(pstEnv, pstZoneSkill);
		if (NULL == pstSkillDef)
		{
			continue;
		}

		//上次的的跳跃点 
		memset(&stLastPos,0,sizeof(stLastPos));		
		stLastPos	=	pstLightningAtkInfo->arrJumpedPos[pstLightningAtkInfo->iJumpNum];
		if(stLastPos.X == 0 || stLastPos.Y == 0 )
		{
			memset(pstLightningAtkInfo,0,sizeof(LightningAtkInfo));
			continue;
		}

		//找玩家视野里,并且离上次跳跃最近的对象
		iDistance = 9999;
		for (j=0; j<pstPlayer->stOnline.iVAnimate; j++)
		{
			//看是否是已经找过的 
			iID	=	pstPlayer->stOnline.aiVAnimate[j];		
			/*for(z=0;z<MAX_LIGHTNING_JUMP_NUM;z++)
			{
				if(pstLightningAtkInfo->iMemID[z]==iID)
				{
					break;
				}
			}
			if(z < MAX_LIGHTNING_JUMP_NUM)
			{
				continue;
			}*/
			
			iIndex	=	z_id_indextype(pstEnv->pstAppCtx, pstEnv, iID, &cType);
			if( -1==iIndex )
			{
				continue;
			}			
			switch(cType)
			{
				case OBJ_PLAYER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstPlayerPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					pstPlayerView	=	(Player *)pstMemBlock->szData;

					if (pstPlayerView->eStatus == PLAYER_STATUS_ROLE_LOGIN)
					{
						stAni.iType = OBJ_PLAYER;
						stAni.stObj.pstPlayer = pstPlayerView;
						if(z_player_filter_relation(pstEnv->pstAppCtx,pstEnv, pstPlayer, &stAni, pstSkillDef))
						{
							//是敌对关系
							int iDistanceTmp = 0;
							PetFightShiOnline *pstPetTiShen = NULL;
							ROLEPET *pstRolePet = NULL;
								
							iDistanceTmp = z_distance(&stLastPos, &pstPlayerView->stRoleData.Pos);
							if( iDistanceTmp < iDistance)
							{
								pstPetTiShen = z_pet_tishen(pstEnv,  pstPlayerView);
								if (NULL == pstPetTiShen || !is_tishen_skill(pstSkillDef))
								{
									TargetIDsTmp = pstPlayerView->iMemID;
								}
								else
								{
									pstRolePet = z_find_role_pet(pstPlayerView, pstPetTiShen->stRolePetOnline.ullWID, NULL);
									if (NULL == pstRolePet)
									{
										TargetIDsTmp = pstPlayerView->iMemID;
									}
									else
									{
										TargetIDsTmp = pstPlayerView->iMemID+(pstPetTiShen->iXieDaiPos+1)*PET_ID_BASE;
									}
								}
								if( 0 == z_find_lightning_memid(pstLightningAtkInfo,TargetIDsTmp))
								{
									break;
								}
								
								iDistance = iDistanceTmp;
								AtkPos = pstPlayerView->stRoleData.Pos;
								TargetIDs = TargetIDsTmp;
								
							}
						}
					}
					break;
				case OBJ_MONSTER:
					pstMemBlock = TMEMPOOL_GET_PTR(pstEnv->pstMonsterPool, iIndex);
					if (!pstMemBlock->fValid)
					{
						break;
					}
					pstMon	=	(Monster *)pstMemBlock->szData;
					stAni.iType = OBJ_MONSTER;
					stAni.stObj.pstMon= pstMon;

					if( 0 == z_find_lightning_memid(pstLightningAtkInfo,pstMon->iID))
					{
						break;
					}
					 
					if( pstMon->bSubType <MON_SUB_TYPE_TRAP &&
						z_player_filter_relation(pstEnv->pstAppCtx,pstEnv, pstPlayer, &stAni, pstSkillDef))
					{
						//是敌对关系
						int iDistanceTmp = 0;
						iDistanceTmp = z_distance(&stLastPos, &pstMon->stCurrPos);
						if( iDistanceTmp < iDistance)
						{
							iDistance = iDistanceTmp;
							AtkPos =  pstMon->stCurrPos;
							TargetIDs = pstMon->iID;
						}
					}
					break;
				default:
					break;
			}
		}
		if(iDistance == 9999)
		{
			memset(pstLightningAtkInfo,0,sizeof(LightningAtkInfo));
			continue;
		}
		
		iOldSkillSeq = pstPlayer->stOnline.stAtkInfo.iSkillSeq;
		stFrameHead.TimeStamp.TimeVal.sec = pstEnv->pstAppCtx->stCurr.tv_sec;
		stFrameHead.TimeStamp.TimeVal.usec = pstEnv->pstAppCtx->stCurr.tv_usec;
		pstSkillAtk->SkillID = pstLightningAtkInfo->ushSkillID;
		//pstSkillAtk->SkillSeq = iOldSkillSeq+1;
		pstSkillAtk->SkillSeq = 0;//客户端有特殊处理,不管这个序号
		pstSkillAtk->RolePos = pstPlayer->stRoleData.Pos;
		pstSkillAtk->AtkPos = AtkPos;
		pstSkillAtk->TargetNum = 1;
		pstSkillAtk->TargetIDs[0] = TargetIDs;
		
		
		pstSkillAtk->SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
		pstSkillAtk->SvrTime.BenchTimeUSec = pstEnv->pstAppCtx->stCurr.tv_usec;
		stTmp = pstPlayer->stOnline.stLastCltTime;
		stTmpCommCool= pstPlayer->stOnline.stSkillList.stCommCoolEnd;

		pstAtkInfo->iAtkLightningIdx = i;
		//先存下
		memcpy(&stAtkInfoTmp,pstAtkInfo,sizeof(AtkInfo));
		StateTmp = pstPlayer->stOnline.State;
		
		pstAtkInfo->iAtkFlagBit |= ATK_FLAG_BIT_SVR_LIGHTNING;	
		pstAtkInfo->iAtkFlagBit |= ATK_FLAG_BIT_SVR_ATK;		
		iJumpNum = pststOnline->stLightningAtkInfo[i].iJumpNum;
		player_atk(pstAppCtx, pstEnv, pstPlayer, &stFrameHead, &stPkg);
		//恢复,避免吟唱啥的被打断
		memcpy(pstAtkInfo,&stAtkInfoTmp,sizeof(AtkInfo));
		if(!(pstPlayer->stOnline.State&CS_STAT_DEAD))
		{
			pstPlayer->stOnline.State = StateTmp;
		}

		pstAtkInfo->iAtkFlagBit = 0;
		if(iJumpNum == pststOnline->stLightningAtkInfo[i].iJumpNum)
		{
			pststOnline->stLightningAtkInfo[i].iJumpNum--;
		}
				
		pstPlayer->stOnline.stLastCltTime = stTmp;
		pstPlayer->stOnline.stSkillList.stCommCoolEnd = stTmpCommCool;
	}

	return 0;
}

int z_get_package_ride(Player *pstPlayer, unsigned int iItemID)
{
	int i;
	ROLERIDELIST   *pstRideList = &pstPlayer->stRoleData.MiscInfo.RideList;

	for (i = 0 ; i < pstRideList->Num; i++)
	{
		if (pstRideList->RideGrids[i].ItemID == iItemID )
		{
			return i;
		}
	}

	return -1;
}
int player_ride_prepare(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	long long llRet;
	int iIdx;
	struct timeval stConnectTime, stCltTime;
	ZoneStatus *pstStutas = NULL;
	RIDEDEF *pstRideDef;
	ZONE_SVRCONF *pstConf = pstEnv->pstConf;
	CSRIDEPRE *pstPrepare = &pstCsPkg->Body.RidePre;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	int iRet;
		
	if( CS_STAT_HITCH_RIDE & pstPlayer->stOnline.State )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL18);
		return -1;
	}
	
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL19);
		return -1;
	}

	// 检查禁止召唤状态
	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NO_BECKON))
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL19);
		return -1;
	}

	iIdx = z_get_package_ride(pstPlayer, pstPrepare->RideItemID);
	if (iIdx < 0)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL20);
		return -1;
	}

	if(pstRoleTmpSys->TmpItemID)
	{
		ITEMDEF *pstItemDef;
		
		pstItemDef = z_get_itemdef_roleitem(pstEnv, (ROLEITEM *)pstRoleTmpSys);
		if (pstItemDef && ITEM_TMP_MACHINE == pstItemDef->ItemTmp.Type)
		{
			z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL19);
			return -1;
		}
	}
	
	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) //在副本中
	{
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld && 
			(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_RIDE))
		{
			z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL21);
			return -1;
		}
	}

	pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].ItemID, 
									pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].Level);
	if (NULL == pstRideDef)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);	
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d Ride prepare no RideDef", 
					pstPlayer->stRoleData.Uin, pstPrepare->RideItemID);
		return -1;
	}

	iRet = z_pkg_check_clanlimit(pstEnv, pstPlayer,pstRideDef->ClanLimit);
	if (iRet < 0)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);	
		if (iRet == -1)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL22);
		}
		else if (iRet == -2)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL23);
		}
		return -1;
	}	
	
	if (pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].Endtime > 0 &&
		pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].Endtime < pstAppCtx->stCurr.tv_sec)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);	
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SKILL24);
		return -1;
	}

	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_RIDE))
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return -1;
	}

	if( ( CS_STAT_RIDE_ON & pstPlayer->stOnline.State) ||  (CS_STAT_FLY & pstPlayer->stOnline.State))
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return 0;
	}
	
	pstStutas = z_player_find_status(pstPlayer, STATUS_ID_JIZOU);
	if (pstStutas)
	{
		z_player_del_status(pstAppCtx, pstEnv, pstPlayer, STATUS_ID_JIZOU, 1, 0);
	}

	pstStutas = z_player_find_status(pstPlayer, STATUS_ID_QIANXIN);
	if (pstStutas)
	{
		z_player_del_status(pstAppCtx, pstEnv, pstPlayer, STATUS_ID_QIANXIN, 1, 0);
	}

	if ((CS_STAT_SIT|CS_STAT_GAOJI_SIT) & pstPlayer->stOnline.State)
	{
		player_sit_break(pstAppCtx, pstEnv, pstPlayer);
	}

	memset(&stConnectTime,0,sizeof(stConnectTime));
	memset(&stCltTime,0,sizeof(stCltTime));
	
	stConnectTime.tv_sec = pstFrameHead->TimeStamp.TimeVal.sec;
	stConnectTime.tv_usec = pstFrameHead->TimeStamp.TimeVal.usec;

	stCltTime.tv_sec = pstPrepare->SvrTime.BenchTimeSec;
	stCltTime.tv_usec = pstPrepare->SvrTime.BenchTimeUSec;

	llRet = z_compare_time(&stCltTime, &stConnectTime);
	if (llRet > pstConf->BenchTimeAttackMax || llRet < pstConf->BenchTimeAttackMin)
	{
		z_adjust_time(&stCltTime, &stConnectTime, pstPlayer);
		
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time clt and connect time %lld", pstPlayer->stRoleData.Uin, llRet);
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return -1;
	}
	
	if (pstPlayer->stOnline.cCheckCltTimeFlag)
	{
		if (pstPlayer->stOnline.stLastCltTime.tv_sec !=0)
		{
			llRet = z_compare_time(&pstPlayer->stOnline.stLastCltTime, &stCltTime);

			if (llRet < 0)
			{
				tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time lastclttime and clttime fail", pstPlayer->stRoleData.Uin);
				z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
				return -1;
			}
		}
	
		TV_CLONE(pstPlayer->stOnline.stLastCltTime, stCltTime);
	}
	else
		return -1;

	pstPlayer->stOnline.State |= CS_STAT_RIDE_PREPARE;
	
	TV_CLONE(pstPlayer->stOnline.stRidePrepare, stCltTime);

	pstActionRes->ActionID = ACTION_RIDE_PRE;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.RidePre.RideItemID = pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].ItemID;
	pstActionRes->Data.RidePre.RideLevel = pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].Level;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
	
}

int is_valid_map_ride(MapIndex *pstMapIdx, RIDEDEF *pstRideDef)
{
	if(pstMapIdx->stMapDef.MapType == MAP_TYPE_NORMAL)
	{
		if(pstRideDef->RideMapFlag & RIDE_MAP_FLAG_NORMAL)
		{
			return 1;
		}
	}
	else if(pstMapIdx->stMapDef.MapType == MAP_TYPE_DIGONG)
	{
		if(pstRideDef->RideMapFlag & RIDE_MAP_FLAG_DIGONG)
		{
			return 1;
		}
	}
	else if(pstMapIdx->stMapDef.MapType == MAP_TYPE_POLY)
	{
		if(pstRideDef->RideMapFlag & RIDE_MAP_FLAG_POLY)
		{
			return 1;
		}
	}
	else if(pstMapIdx->stMapDef.MapType == MAP_TYPE_BATTLE)
	{
		if(pstRideDef->RideMapFlag & RIDE_MAP_FLAG_BATTLE)
		{
			return 1;
		}
	}
	else if(pstMapIdx->stMapDef.MapType == MAP_TYPE_PWORLD)
	{
		if(pstRideDef->RideMapFlag & RIDE_MAP_FLAG_PWORLD)
		{
			return 1;
		}
	}
	
	return 0;
}

int player_ride_on(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	long long llRet;
	//long long llMs;
	int iRet;
	int iIdx;
	//struct timeval stDiff;
	struct timeval stConnectTime, stCltTime;
	RIDEDEF *pstRideDef;
	ZONEIDX stIdx;
	MapIndex *pstMapIdx;
	ZONE_SVRCONF *pstConf = pstEnv->pstConf;
	CSRIDEON *pstRideOn = &pstCsPkg->Body.RideOn;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;

	player_sham_dead_clear(pstEnv, pstPlayer);

	if( CS_STAT_HITCH_RIDE & pstPlayer->stOnline.State )
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL18);
		return -1;
	}

	if (z_is_player_status_result(pstEnv, pstPlayer, STATUS_RESULT_NOSKILL))
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL19);
		return -1;
	}
	
	if (0 == z_player_act_status(pstAppCtx, pstEnv, pstPlayer, RES_ACTION_RIDE))
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return -1;
	}

	if ((CS_STAT_SIT|CS_STAT_GAOJI_SIT) & pstPlayer->stOnline.State)
	{
		player_sit_break(pstEnv->pstAppCtx, pstEnv, pstPlayer);
	}

	if(player_has_bianshen_status(pstAppCtx, pstEnv, pstPlayer))
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL25);
		return -1;
	}

	/*if (CS_STAT_FLY & pstPlayer->stOnline.State)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return 0;
	}*/

	/*
	if ( !(pstPlayer->stOnline.State & CS_STAT_RIDE_PREPARE))
	{
		//tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player state %d", pstPlayer->stRoleData.Uin, pstPlayer->stOnline.State);
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return -1;
	}*/

	if (pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) //在副本中
	{
		ZONEPWORLD *pstPworld;
		ZONEIDX stIdx;

		stIdx.iID = GET_PWORLDID(pstPlayer->stRoleData.Map);
		stIdx.iPos = -1;
		
		pstPworld = z_find_pworld(pstEnv->pstMapObj, &stIdx);	
		if (pstPworld && 
			(pstPworld->stPworldDef.CtrlFlag & PWORLD_NO_RIDE))
		{
			z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL21);
			return -1;
		}
	}

	stConnectTime.tv_sec = pstFrameHead->TimeStamp.TimeVal.sec;
	stConnectTime.tv_usec = pstFrameHead->TimeStamp.TimeVal.usec;

	stCltTime.tv_sec = pstRideOn->SvrTime.BenchTimeSec;
	stCltTime.tv_usec = pstRideOn->SvrTime.BenchTimeUSec;

	llRet = z_compare_time(&stCltTime, &stConnectTime);
	if (llRet > pstConf->BenchTimeAttackMax || llRet < pstConf->BenchTimeAttackMin)
	{
		z_adjust_time(&stCltTime, &stConnectTime, pstPlayer);
		
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time clt and connect time %d", pstPlayer->stRoleData.Uin, llRet);
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return -1;
	}
	
	if (pstPlayer->stOnline.cCheckCltTimeFlag)
	{
		if (pstPlayer->stOnline.stLastCltTime.tv_sec !=0)
		{
			llRet = z_compare_time(&pstPlayer->stOnline.stLastCltTime, &stCltTime);

			if (llRet < 0)
			{
				tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player z_compare_time lastclttime and clttime fail", pstPlayer->stRoleData.Uin);
				z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
				return -1;
			}
		}
	
		TV_CLONE(pstPlayer->stOnline.stLastCltTime, stCltTime);
	}
	else
		return -1;

	/*
	TV_DIFF(stDiff, stCltTime, pstPlayer->stOnline.stRidePrepare);
	TV_TO_MS(llMs, stDiff);
	if ( llMs + CLT_TIME_WAVE*30 < MAX_RIDE_PRE_TIME)
	{
		tlog_debug(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player RIDE %d prepare %d real %d", pstPlayer->stRoleData.Uin, 
					pstRideOn->RideItemID, llMs, MAX_RIDE_PRE_TIME);
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return -1;
	}*/

	iIdx = z_get_package_ride(pstPlayer, pstRideOn->RideItemID);
	if (iIdx < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL20);
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		return -1;
	}

	if (pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].Endtime > 0 &&
		pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].Endtime < pstAppCtx->stCurr.tv_sec)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_TIP, ZONEERR_SKILL24);
		return -1;
	}


	pstRideDef = z_find_ride_def(pstEnv, pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].ItemID, 
									pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].Level);
	if (NULL == pstRideDef)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);
		tlog_error(pstEnv->pstLogCat, pstPlayer->stRoleData.Uin, 0, "%d player %d Ride prepare no RideDef", 
					pstPlayer->stRoleData.Uin, pstRideOn->RideItemID);
		return -1;
	}

	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapIdx = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	if(!is_valid_map_ride(pstMapIdx, pstRideDef))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_COMBAT1);
		return -1;
	}

	iRet = z_pkg_check_clanlimit(pstEnv, pstPlayer,pstRideDef->ClanLimit);
	if (iRet < 0)
	{
		z_ride_break(pstAppCtx, pstEnv,  pstPlayer);	
		if (iRet == -1)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL22);
		}
		else if (iRet == -2)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx,pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SKILL23);
		}
		return -1;
	}	

	//清除临时道具
	if(pstRoleTmpSys->TmpItemID)
	{
		tmp_item_drop(pstEnv, pstPlayer,1);
	}

	if (pstRideDef->RideType == RIDE_TYPE_LAND)
	{
		pstPlayer->stOnline.State |= CS_STAT_RIDE_ON;
	}
	else
	{
		pstPlayer->stOnline.State |= CS_STAT_FLY;
	}
	

	pstPlayer->stOnline.State &= ~CS_STAT_RIDE_PREPARE;	
	pstPlayer->stOnline.RideItemID = pstRideOn->RideItemID;
	pstPlayer->stOnline.RideLevel = pstPlayer->stRoleData.MiscInfo.RideList.RideGrids[iIdx].Level;
	pstPlayer->stOnline.stRideOnInfo.iOwnerID = pstPlayer->iMemID;
	pstPlayer->stOnline.stRideOnInfo.iPassengerNum = 0;
		
	z_player_chk_mspd(pstAppCtx, pstEnv, pstPlayer);

	pstActionRes->ActionID = ACTION_RIDE;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.Ride.RideItemID = pstPlayer->stOnline.RideItemID;
	pstActionRes->Data.Ride.RideLevel  = pstPlayer->stOnline.RideLevel;
	pstActionRes->Data.Ride.RideFlag = 1;

	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);

	//提示
	z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL26, pstRideDef->RideName);
	
	return 0;	
}

int player_do_ride_brk( ZONESVRENV* pstEnv, Player *pstPlayer, int iRideItemID )
{
	int fDoNotNotify = 0;
	int iRideFlag = 0;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;

	UNUSED(&iRideItemID);

	if (0 == ((CS_STAT_RIDE_ON|CS_STAT_RIDE_PREPARE | CS_STAT_FLY | CS_STAT_HITCH_RIDE) & pstPlayer->stOnline.State))
	{
		return 0;
	}

	if(CS_STAT_RIDE_PREPARE&pstPlayer->stOnline.State)
	{
		iRideFlag = 2;
	}

	if (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE)
	{
		fDoNotNotify = 1;
	}
	
	z_ride_clear(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	if (!fDoNotNotify)
	{
		pstActionRes->ActionID = ACTION_RIDE;
		pstActionRes->ID = pstPlayer->iMemID;
		pstActionRes->Data.Ride.RideItemID= pstPlayer->stOnline.RideItemID;
		pstActionRes->Data.Ride.RideLevel = pstPlayer->stOnline.RideLevel;
		pstActionRes->Data.Ride.RideFlag = iRideFlag;
		//pstActionRes->Data.Ride.RideFlag = 0;
		
		Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
		z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	}
	
	return 0;
}

int player_ride_brk(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSRIDEBRK *pstRideBrk = &pstCsPkg->Body.RideBrk;

	UNUSED( pstAppCtx );
	UNUSED( pstFrameHead );

	return player_do_ride_brk( pstEnv, pstPlayer, pstRideBrk->RideItemID );	
}

int pet_hit_player(ZONESVRENV* pstEnv, Player *pstAtker,ROLEPET *pstRolePet, Player *pstAtkee, AtkProcess *pstAtkProcess)
{
	int iHP;
	long long llTotalHurt;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	ZoneAni stAtkerAni;
	int iRealHurt, iHurt, iHeavyHit;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	ROLETMPSYS *pstRoleTmpSys = &pstAtkee->stRoleData.MiscInfo.TmpSys;
	struct timeval stCltTime;

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		if (pstAtkee->stRoleData.Map ==  2400 && z_player_find_status(pstAtkee, 3009))	
		{
			return -1;
		}
	}

	//隐身受击显形
	if ((pstAtkee->stOnline.State & CS_STAT_QIANXIN) && (pstSkillDef->TargetType & SKILL_TARGET_ENEMY_PLAYER))
	{
		z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstAtkee, STATUS_ID_QIANXIN, 1, 0);
	}

	stCltTime.tv_sec = pstAtkProcess->stAtkerInfo.SvrTime.BenchTimeSec;
	stCltTime.tv_usec = pstAtkProcess->stAtkerInfo.SvrTime.BenchTimeUSec;
	
	stAtkeeInfo.stAtkee.iType = OBJ_PLAYER;
	stAtkeeInfo.stAtkee.stObj.pstPlayer = pstAtkee;

	pstOneHit->ID = pstAtkee->iMemID;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;

	pstAtkee->stOnline.iAtkerTarget = pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner->iMemID 
									+ (pstAtkerInfo->stAtker.stObj.stZonePet.pstPetFightShi->iXieDaiPos +1)*PET_ID_BASE;

	z_get_pet_dyn(pstEnv, pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner, pstAtkerDyn, pstRolePet->WID);

	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;
	z_cacl_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);

	player_collectskill_add_slot(pstEnv, pstAtkee,0,COLLECTSKILL_ADDTYPE_AKEE);
	//宠物状态效果
	//z_cacl_pet_status_result(pstEnv, pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner, pstAtkerDyn, &pstAtkerDyn->astAtkChg[0], pstRolePet);
		
	memcpy(pstAtkeeDyn, &pstAtkee->stOnline.stDyn, sizeof(*pstAtkeeDyn));
	//z_cacl_player_status_result(pstEnv, pstAtkeeDyn, NULL, pstAtkee);
	if (pstRoleTmpSys->TmpItemID > 0)
	{
		z_cacl_tmpitem_attr(pstEnv, pstRoleTmpSys, pstAtkeeDyn);
	}

	stAtkerAni.iType = OBJ_PLAYER;
	stAtkerAni.stObj.pstPlayer = pstAtker;
	atker_set_pktime(pstEnv, &stAtkerAni, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);

	pstHits->TotalHit++;

	if (pstSkillDef->NoHurtSkill) //非伤害技能，只需要处理技能效果
	{
		if ((pstSkillDef->TargetType & SKILL_TARGET_ENEMY_PLAYER) && z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
			pstOneHit->TotalHurt++;
			return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
		}
				
		z_do_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	llTotalHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;


	//受攻方是否无敌状态
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	// 受功放是否绝对闪避
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_UTTERLY_FLEE))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		//genius_add_status_by_flee(pstEnv->pstAppCtx, pstEnv, pstAtkee);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}
	
	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		//genius_add_status_by_flee(pstEnv->pstAppCtx, pstEnv, pstAtkee);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	if(is_in_miss_time_player(&pstEnv->pstAppCtx->stCurr, pstAtkee))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		//暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0 /(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}	
		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	// 吸收全部伤害
	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_SORB_ALL_HURT))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_SBSORB;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iRealHurt;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	if (z_is_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DEC_PERCENT_MAX_HP))
	{
		STATUSRESULT stResult;
		z_get_player_status_result(pstEnv, pstAtkee, STATUS_RESULT_DEC_PERCENT_MAX_HP,
			                           &stResult, 0);

		MIDATTRLIST *pstMidAttrList = &pstAtkee->stOnline.stDyn.stMidAttrList;
		int iTemp  = pstMidAttrList->uiHPMax * (stResult.ResultVal1/100.0);

		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iTemp;
		pstOneHit->TotalHurt++;

		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}
	
	//被暴击加hp
	//genius_add_hp_by_heavyhit( pstEnv->pstAppCtx, pstEnv, pstAtkee,pstOneHit,iHeavyHit);
	//genius_reduce_skill_cd_time_by_heavyhit( pstEnv->pstAppCtx,  pstEnv, pstAtkee, &stCltTime,iHeavyHit);
	//genius_add_status_by_pet_atk(pstEnv->pstAppCtx, pstEnv, pstAtker, pstRolePet, pstAtkee);

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;
	z_do_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		
		pstAtkProcess->iContinueCount--;

		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstAtkee->stRoleData.RoleDetail.HPCurr;
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}
		else
		{
			 // 伤害吸收计算
			if (0 == (pstAtkerInfo->iFlag & ATKER_INFO_FLAG_THROUGH_ABSORB))
			{
				z_player_hurt_absorb(pstEnv, pstAtkee, pstOneHit, &iHurt);
				z_player_hurt_status_absorb(pstEnv,pstAtkee, pstOneHit,  &iHurt);
			}
		}

		if (iHurt <= 0)
		{
			continue;
		}
				
		llTotalHurt += iHurt;

		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;

		pstOneHit->TotalHurt++;
		
		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		if (llTotalHurt >= (iHP + pstAtkee->stRoleData.RoleDetail.HPCurr))
		{
			break;
		}
	}while (pstAtkProcess->iContinueCount > 0);
	
	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	UNUSED(pstAtker);	//add by paraunused.pl
}

/*
RESULT*  z_get_pet_attrs_result(ZONESVRENV* pstEnv, ROLEPET *pstRolePet, int iResultID)
{
	int i = 0;
	int j = 0;
	ADDATTRDEF *pstAddAttrDef =NULL;

	for (i = 0; i < pstRolePet->PetAttrs.Num; i++)
	{
		pstAddAttrDef = z_find_addattr(pstEnv, pstRolePet->PetAttrs.AddAttrID[i]);
		if (NULL == pstAddAttrDef)
		{
			continue;
		}

		for(j = 0; j <MAX_ADDATTR_RESULT; j++)
		{
			if (pstAddAttrDef->Result[j].ResultID == 0)
			{
				break;
			}
			
			if ( pstAddAttrDef->Result[j].ResultID == iResultID)
			{
				return (RESULT *) &pstAddAttrDef->Result[j];
			}
		}
	}
	
	return NULL;
}

int z_pet_hit_mon_dyn(ZONESVRENV* pstEnv, DYNATTRLIST *pstDyn, ROLEPET *pstRolePet, MONSTERDEF *pstMonDef)
{
	RESULT *pstResult = NULL;
	
	switch(pstMonDef->SubType)
	{
		case MON_SUB_TYPE_MINI:
			
			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_MINI);
			break;
		case MON_SUB_TYPE_WEAK:
			
			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_WEAK);
			break;
		case MON_SUB_TYPE_SOFT:

			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_SOFT);
			break;
		case MON_SUB_TYPE_NORM:

			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_NORM);
			break;
		case MON_SUB_TYPE_HARD:

			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_HARD);
			break;
		case MON_SUB_TYPE_XI:

			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_XI);
			break;
		case MON_SUB_TYPE_JIN:

			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_JIN);
			break;
		case MON_SUB_TYPE_BOSS:

			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_BOSS);
			break;
		case MON_SUB_TYPE_JI:

			pstResult = z_get_pet_attrs_result(pstEnv, pstRolePet, RESULT_DESPISE_JI);
			break;
		default:
			break;
	}

	if (pstResult)
	{
		pstDyn->astAtkChg[0].stAtkerHurtChg.AtkerHurtMul += pstResult->ResultVal1;
	}

	return 0;
}*/

/*int mon_fight_speech(ZONESVRENV* pstEnv, Monster *pstMon, int iFightType)
{
	int i;
	int iNum = 0;
	unsigned int uiRnd,uiJiChu=0;
	MONSTERSPEECHDEF *pstMonSpeech;
	FIGHTSPEECH *pstFightSpeech;
	
	//怪物被击发言
	pstMonSpeech = z_find_monsterspeech_def(pstEnv,  pstMon->iDefIdx, NULL);
	if (pstMonSpeech)
	{
		switch(iFightType)
		{
			case  SPEECH_MON_ATTACK:
				pstFightSpeech = pstMonSpeech->MonAttackSpeech;
				iNum = MAX_MON_ATTACK_SPEECH;
				break;
			case  SPEECH_MON_BEHIT:
				pstFightSpeech = pstMonSpeech->MonBehitSpeech;
				iNum = MAX_MON_BEHIT_SPEECH;
				break;
			default:
				return 0;
				break;
		}
		
		uiRnd = ( unsigned int)RAND()%10000;
		for (i = 0; i<iNum; i++)
		{
			if (pstFightSpeech[i].Rate == 0)
			{
				break;
			}
			
			if( uiRnd < (pstFightSpeech[i].Rate + uiJiChu)*100 )
			{
				if (strlen(pstFightSpeech[i].Desc) > 0)
				{
					z_mon_speech(pstEnv, pstMon, pstFightSpeech[i].BroadcastType, iFightType, i);
				}
				break;
			}
			uiJiChu += pstFightSpeech[i].Rate;
		}
	}

	return 0;
}*/


int pet_hit_mon(ZONESVRENV* pstEnv, ROLEPET *pstRolePet, Monster *pstAtkee, AtkProcess *pstAtkProcess)
{
	int iHP;
	long long llTotalHurt;
	MONSTERDEF *pstMonsterDef;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	ZoneAni stAtkerAni;
	int iRealHurt, iHurt, iHeavyHit;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	Player *pstOwner = pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner;

	if (pstEnv->pstConf->CountryVer == COUNTRY_VER_KOREA)
	{
		if (is_mon_machine(pstAtkee))
		{
			Player *pstPlayer1 = machine_get_driver(pstEnv, pstAtkee);
			
			if (pstPlayer1 && pstPlayer1->stRoleData.MiscInfo.DartCarInfo.MonMemID > 0 && pstOwner)
			{
				ZoneClan *pstClan1 = player_get_clan(pstEnv, pstPlayer1);
				if (!(pstClan1 && clan_relation_by_player(pstEnv, pstOwner, pstClan1)< 0))
				{
					return -1;
				}
			}
		}
	}

	//融焰Boss不可被其他玩家的宠物攻击，只能被召唤boss的玩家宠物攻击
	if(0 > player_hit_team_call_rongyan_mon(pstEnv,pstOwner,pstAtkee))
	{
		return -1;
	}
		
	stAtkeeInfo.stAtkee.iType = OBJ_MONSTER;
	stAtkeeInfo.stAtkee.stObj.pstMon = pstAtkee;

	pstOneHit->ID = pstAtkee->iID;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;

	pstAtkee->bTraceTry = 0;

	z_get_pet_dyn(pstEnv, pstOwner, pstAtkerDyn, pstRolePet->WID);

	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;
	
	z_cacl_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);

	//宠物状态效果
	//z_cacl_pet_status_result(pstEnv, pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner, pstAtkerDyn, &pstAtkerDyn->astAtkChg[0], pstRolePet);
		
	pstMonsterDef = z_get_mon_def(pstEnv, pstAtkee);
	if (NULL == pstMonsterDef)
	{
		return -1;
	}

	//z_pet_hit_mon_dyn(pstEnv, pstAtkerDyn, pstRolePet, pstMonsterDef);

	if (0 == pstSkillDef->NotBootyFirstAtk)
	{
		mon_booty_first_atker(pstEnv->pstAppCtx, pstEnv, pstAtkee, pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner);
	}
	
	z_get_mon_dyn(pstEnv, pstAtkee, pstMonsterDef, pstAtkeeDyn);
	z_cacl_mon_status_result(pstEnv, pstAtkeeDyn, NULL, pstAtkee);

	pstHits->TotalHit++;

	stAtkerAni.iType = OBJ_PLAYER;
	stAtkerAni.stObj.pstPlayer = pstOwner;
	atker_set_pktime(pstEnv, &stAtkerAni, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);
	
	if (pstSkillDef->NoHurtSkill) //非伤害技能，只需要处理技能效果
	{
		if ((pstSkillDef->TargetType & SKILL_TARGET_ENEMY_MON) && z_is_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
			pstOneHit->TotalHurt++;
			return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
		}

		z_do_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	llTotalHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;

	if (0 > mon_press_find(pstAtkee, pstOwner->iMemID) && pstSkillDef->PressRate > -100  &&
			z_distance(&pstOwner->stRoleData.Pos, &pstAtkee->stCurrPos) <= (int)pstMonsterDef->TraceDist &&
			!is_mon_bub(pstAtkee))
	{
		mon_press_insert_player(pstEnv, pstAtkee, pstMonsterDef, pstOwner);
	}

	//受攻方是否无敌状态
	if (z_is_mon_status_result(pstEnv, pstAtkee, STATUS_RESULT_INVIN))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}
	
	if(pstAtkee->cMonMiscFlag & MON_MISC_FLAG_STATUS_TRACE)
	{
		mon_status_beatk(pstEnv->pstAppCtx, pstEnv, pstAtkee);
	}

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		//暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0 /(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}	
		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;
	z_do_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		
		pstAtkProcess->iContinueCount--;
				
		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstAtkee->iHP;
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}
		else
		{
			// 怪物伤害吸收计算
			if (0 == (pstAtkerInfo->iFlag & ATKER_INFO_FLAG_THROUGH_ABSORB))
			{
				z_mon_hurt_absorb(pstEnv, pstAtkee, pstOneHit, &iHurt);
				z_mon_hurt_status_absorb(pstEnv,pstAtkee, pstOneHit,  &iHurt);
			}
		}

		if (iHurt <= 0)
		{
			continue;
		}
				
		llTotalHurt += iHurt;

		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;

		pstOneHit->TotalHurt++;
		
		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		if (llTotalHurt >= (iHP + pstAtkee->iHP))
		{
			break;
		}
	}while (pstAtkProcess->iContinueCount > 0);

	pstAtkee->iAtkerID = pstOwner->iMemID;
	pstAtkee->stAtkerPos = pstOwner->stRoleData.Pos;
	pstAtkee->cAtkerInfoNew = 1;
	
	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
}

int pet_hit_pet(ZONESVRENV* pstEnv, ROLEPET *pstRolePet, ZonePet *pstAtkee, AtkProcess *pstAtkProcess)
{
	int iHP;
	long long llTotalHurt;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	ZoneAni stAtkerAni;
	int iRealHurt, iHurt, iHeavyHit;
	AddAtkInfo  *pstAddAtkInfo = &pstAtkProcess->stAddAtkInfo;
	AtkerInfo *pstAtkerInfo = &pstAtkProcess->stAtkerInfo;
	DYNATTRLIST *pstAtkerDyn = &pstAtkerInfo->stDyn;
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF *pstSkillDef = pstAtkerInfo->pstSkillDef;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = &pstHits->Hits[pstHits->TotalHit];
	Player *pstOwner = pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner;

	if (0 == (PET_STAT_FIGHT & pstAtkee->pstRolePet->StatFlag))
	{
		return -1;
	}

	//宠物状态效果
	//z_cacl_pet_status_result(pstEnv, pstAtkerInfo->stAtker.stObj.stZonePet.pstOwner, pstAtkerDyn, &pstAtkerDyn->astAtkChg[0], pstRolePet);
		
	stAtkeeInfo.stAtkee.iType = OBJ_PET;
	stAtkeeInfo.stAtkee.stObj.stZonePet = *pstAtkee;

	pstOneHit->ID = pstAtkee->pstOwner->iMemID+(pstAtkee->pstPetFightShi->iXieDaiPos+1)*PET_ID_BASE;
	pstOneHit->HP = 0;
	pstOneHit->TotalHurt = 0;

	pstAtkee->pstOwner->stOnline.iAtkerTarget = pstOwner->iMemID+(pstAtkerInfo->stAtker.stObj.stZonePet.pstPetFightShi->iXieDaiPos+1)*PET_ID_BASE;

	z_get_pet_dyn(pstEnv, pstOwner, pstAtkerDyn, pstRolePet->WID);

	pstAtkProcess->cSordHpRate = 0;
	pstAtkProcess->cSordMpRate = 0;
	pstAtkProcess->iSordHp = 0;
	z_cacl_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		
	z_get_pet_dyn(pstEnv, pstAtkee->pstOwner, pstAtkeeDyn, pstAtkee->pstRolePet->WID);

	pstHits->TotalHit++;
	
	stAtkerAni.iType = OBJ_PLAYER;
	stAtkerAni.stObj.pstPlayer = pstOwner;
	atker_set_pktime(pstEnv, &stAtkerAni, &stAtkeeInfo.stAtkee, pstSkillDef->NoHurtSkill);
	
	if (pstSkillDef->NoHurtSkill) 
	{
		z_do_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	llTotalHurt = 0;
	pstAtkProcess->iContinueCount = pstAtkProcess->bContinueNum;

	//首先计算命中
	if (0 > z_miss_check(pstAtkerInfo, &stAtkeeInfo))
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_MISS;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	iHeavyHit = 0;
	if (pstSkillDef->HeavyHitRate > 0)
	{
		iHeavyHit = pstSkillDef->HeavyHitRate;
		iHeavyHit += pstAtkerDyn->stHitRateChg.nHeavyHitAdd;
		iHeavyHit += skill_result_add_heavyhit_rate(pstSkillDef, &stAtkeeInfo);
		iHeavyHit = iHeavyHit*100;
		//暴击力/ (暴击力+ 暴击抵抗)/3
		if(pstAtkerDyn->stHitRateChg.iHeavyHit != 0 && pstAtkeeDyn->stHitRateChg.iDefHeavyHit != 0)
		{
			iHeavyHit += pstAtkerDyn->stHitRateChg.iHeavyHit*10000.0 /(pstAtkerDyn->stHitRateChg.iHeavyHit +pstAtkeeDyn->stHitRateChg.iDefHeavyHit)/2;
		}	
		if ((int)(RAND1(10000)) >= iHeavyHit)
		{
			iHeavyHit = 0;
		}
		else
		{
			iHeavyHit = 1;
		}
	}

	pstAddAtkInfo->iMainAttrHurt = 0;
	pstAddAtkInfo->unAtkerAdd = 0;
	pstAddAtkInfo->unAtkerMul = 0;
	pstAddAtkInfo->unAtkeeAdd = 0;
	pstAddAtkInfo->unAtkeeMul = 0;
	iRealHurt = iHurt = cacl_hurt(pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillDef, iHeavyHit);
	if(iRealHurt <= 0)
	{
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_NOEFFECT;
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = 0;
		pstOneHit->TotalHurt++;
		return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	pstAtkProcess->cDeadHit = 0;
	pstAddAtkInfo->nAddAtkNum = 0;
	pstAddAtkInfo->iContinueNum = 0;
	pstAtkProcess->cHeavyHit = iHeavyHit;
	pstAtkProcess->iHurt = iHurt;
	z_do_skill_attr(pstEnv->pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo);
	
	do
	{
		if (pstOneHit->TotalHurt >= MAX_HURT)
		{
			break;
		}
		
		iHurt = get_continuenum_hurt(&iRealHurt, pstAtkProcess->iContinueCount);
		
		pstAtkProcess->iContinueCount--;
				
		if (pstAtkProcess->cDeadHit)
		{
			iHurt = pstAtkee->pstRolePet->HP;
			pstOneHit->TotalHurt = 0;
			llTotalHurt = 0;
		}
		else
		{
			 
		}

		if (iHurt <= 0)
		{
			continue;
		}
				
		llTotalHurt += iHurt;

		if (pstAtkProcess->cDeadHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HITDEAD;
		}
		else if (iHeavyHit)
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_HEAVY_DMG;
		}
		else
		{
			pstOneHit->Hurts[(int)pstOneHit->TotalHurt].HurtType = HURT_TYPE_DMG;
		}
		
		pstOneHit->Hurts[(int)pstOneHit->TotalHurt].Value = iHurt;

		pstOneHit->TotalHurt++;
		
		//看一下被攻击者是否已经要死了
		iHP = z_get_hithp(pstOneHit);
		if (llTotalHurt >= (iHP + pstAtkee->pstRolePet->HP))
		{
			break;
		}
	}while (pstAtkProcess->iContinueCount > 0);

	return one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
}

int z_get_atkee_ani_pos(ZoneAni *pstTarAni, RESPOS *pstAtkPos)
{
	if (pstTarAni->iType == OBJ_MONSTER)
	{
		*pstAtkPos = pstTarAni->stObj.pstMon->stCurrPos;
		
	}
	else if (pstTarAni->iType == OBJ_PLAYER)
	{
		*pstAtkPos = pstTarAni->stObj.pstPlayer->stRoleData.Pos;
	}
	else if (pstTarAni->iType == OBJ_PET)
	{
		*pstAtkPos = pstTarAni->stObj.stZonePet.pstOwner->stRoleData.Pos;
	}
	else if (pstTarAni->iType == OBJ_NPC)
	{
		*pstAtkPos = pstTarAni->stObj.pstNpc->stMon.stCurrPos;
	}
	else 
	{
		return-1;
	}

	return 0;
}

int z_pet_atklist(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, 
					ZoneAni *pstAniAtkee, SKILLDEF *pstSkillDef, ZoneObjList *pstList, RESPOS *pstAtkPos)
{
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	ZoneAni stTarAni;

	UNUSED(pstAppCtx);
	pstList->iObj = 0;
	
	stIdx.iID = pstPlayer->stRoleData.Map;
	stIdx.iPos = pstPlayer->stOnline.iMapPos;
	pstMapIdx = z_map_inst_index(pstEnv->pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapIdx)
	{
		return -1;
	}

	
	//对宠物自身
	if (pstSkillDef->TargetType & SKILL_TARGET_SELFPET)
	{
		stTarAni.iType = OBJ_PET;
		stTarAni.stObj.stZonePet.pstOwner = pstPlayer;
		stTarAni.stObj.stZonePet.pstRolePet = pstRolePet;
		stTarAni.stObj.stZonePet.pstPetFightShi = (PetFightShiOnline *)pet_find_online(pstPlayer, pstRolePet);
		if (NULL == stTarAni.stObj.stZonePet.pstPetFightShi)	
		{
			return -1;
		}
	}
	//对自己的主人释放
	else if (pstSkillDef->TargetType & SKILL_TARGET_SELF)
	{
		stTarAni.iType = OBJ_PLAYER;
		stTarAni.stObj.pstPlayer = pstPlayer;
	}
	//攻击目标
	else
	{
		//防范
		switch(pstAniAtkee->iType)
		{
			case OBJ_PLAYER:
				if(pstAniAtkee->stObj.pstPlayer == pstPlayer)
				{
					return -1;
				}
				
				if (PLAYER_STATUS_ROLE_LOGIN != pstAniAtkee->stObj.pstPlayer->eStatus || 
					(0 == pstSkillDef->DeadTarget && ((pstAniAtkee->stObj.pstPlayer->stOnline.State & CS_STAT_DEAD) || pstAniAtkee->stObj.pstPlayer->stRoleData.RoleDetail.HPCurr == 0)) ||
					(pstAniAtkee->stObj.pstPlayer->stOnline.State & (CS_STAT_AUTOFLY | CS_STAT_QIANXIN)))
				{
					return -1;
				}

				if(is_player_ride_on_machine(pstAniAtkee->stObj.pstPlayer))
				{
					return -1;
				}
				break;
			case OBJ_MONSTER:
				if(0 == pstSkillDef->DeadTarget && (pstAniAtkee->stObj.pstMon->cStat != MONSTER_ALIVE || pstAniAtkee->stObj.pstMon->iHP <= 0))
				{
					return -1;
				}

				if (AI_STAT_HOME == pstAniAtkee->stObj.pstMon->bAiStat ||
					pstAniAtkee->stObj.pstMon->bSubType >= MON_SUB_TYPE_TRAP ||
					pstEnv->pstAppCtx->stCurr.tv_sec < pstAniAtkee->stObj.pstMon->tBirthEnd ||
					pstAniAtkee->stObj.pstMon->stMap.iID != pstPlayer->stRoleData.Map)
				{
					return -1;
				}
				break;
			case OBJ_NPC:
				if(	0 == pstSkillDef->DeadTarget && 
					(pstAniAtkee->stObj.pstNpc->stMon.cStat != MONSTER_ALIVE || pstAniAtkee->stObj.pstNpc->stMon.iHP <= 0))
				{
					return -1;
				}

				if (AI_STAT_HOME == pstAniAtkee->stObj.pstNpc->stMon.bAiStat ||
					pstEnv->pstAppCtx->stCurr.tv_sec < pstAniAtkee->stObj.pstNpc->stMon.tBirthEnd ||
					pstAniAtkee->stObj.pstNpc->stMon.stMap.iID != pstPlayer->stRoleData.Map)
				{
					return -1;
				}

				break;
			case OBJ_PET:
				if(pstAniAtkee->stObj.stZonePet.pstRolePet->StatFlag !=  PET_STAT_FIGHT)
				{
					return -1;
				}

				if(is_pet_heti(pstAniAtkee->stObj.stZonePet.pstOwner))
				{
					return -1;
				}
				break;
			default:
				return -1;
				break;
		}
		
		memcpy(&stTarAni, pstAniAtkee, sizeof(stTarAni));
	}

	//打击点
	if( 0> z_get_atkee_ani_pos(&stTarAni, pstAtkPos))
	{
		return -1;
	}

	if (SKILL_RANGE_SIGLE == pstSkillDef->MainRange.RangeType || SKILL_RANGE_NONE == pstSkillDef->MainRange.RangeType)
	{	
		if (0 == z_player_filter_atkobj(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stTarAni,  pstSkillDef,NULL))
		{
			pstList->iObj = 1;
			pstList->astObj[0] = stTarAni;
		}
	}
	else
	{
		z_get_skill_atklist(pstEnv, pstPlayer, pstSkillDef, &stTarAni, pstList, pstAtkPos);
	}

	if(pstList->iObj <= 0)
	{
		return -1;
	}
	
	return 0;
}

//暂时侍卫只有瞬发单攻技能
int pet_shi_hit(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, SKILLDEF *pstSkillDef, 
					PetZoneSkill *pstPetZoneSkill, ZoneAni *pstAniAtkee, PetFightShiOnline *pstPetFightShiOnline)
{
	int i, iMs;
	RESPOS stAtkPos;
	AtkProcess stAtkProc;
	//CSACTIONRES *pstActionRes;
	CSPKG stPkg;
	ZoneOneDead *pstDead;
	MONSTERDEF *pstMonDef;
	Player *pstOwner;
	CSHITS *pstHits = &stAtkProc.stHits;
	ZoneDeadList *pstZoneDeadList = &stAtkProc.stDeadList;
	ZoneObjList stList;
	ZoneAni *pstAni;
	int iRet = 0;
	
	memset(&stAtkProc, 0, sizeof(stAtkProc));

	//memset (&pstPlayer->stOnline.stFitOneline, 0,sizeof(pstPlayer->stOnline.stFitOneline));

	pstHits->SkillID = pstSkillDef->SkillID;
	stAtkProc.stAtkerInfo.stAtker.iType = OBJ_PET;
	stAtkProc.stAtkerInfo.stAtker.stObj.stZonePet.pstOwner = pstPlayer;
	stAtkProc.stAtkerInfo.stAtker.stObj.stZonePet.pstPetFightShi = pstPetFightShiOnline;
	stAtkProc.stAtkerInfo.stAtker.stObj.stZonePet.pstRolePet = pstRolePet;
	stAtkProc.stAtkerInfo.pstZoneSkill = NULL;
	//stAtkProc.stAtkerInfo.pstFittings = NULL;
	stAtkProc.stAtkerInfo.pstSkillDef = pstSkillDef;
	stAtkProc.stAtkerInfo.stAtkPos = pstPlayer->stRoleData.Pos;
	stAtkProc.pstSkillArmChg = NULL;
	stAtkProc.bContinueNum = pstSkillDef->ContinueNum;

	//计算攻击列表
	if (0 > z_pet_atklist(pstEnv->pstAppCtx, pstEnv, pstPlayer, pstRolePet, pstAniAtkee, pstSkillDef, &stList, &stAtkPos))
	{
		return -1;
	}

	//攻击计算副本次数了
	map_pworld_chg_limitflag(pstEnv->pstAppCtx, pstEnv, pstPlayer);

	//技能冷却
	iMs = pstSkillDef->SelfCool*100.0;
	TV_ADD_MS(pstPetZoneSkill->stCoolEnd, pstEnv->pstAppCtx->stCurr, iMs);
	
	for (i=0; i<stList.iObj; i++)
	{
		pstAni = &stList.astObj[i];
		
		switch(pstAni->iType)
		{
			case OBJ_PLAYER:
				pet_hit_player(pstEnv, pstPlayer,pstRolePet, pstAni->stObj.pstPlayer, &stAtkProc);
				z_player_byatk_clear_status(pstEnv, pstAni->stObj.pstPlayer);
				switch(pstSkillDef->FightStat)
				{
					case SKILL_FIGHT_STAT_BOTH:
						SET_PLAYER_FIGHT(pstEnv, pstAni->stObj.pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						break;
					case SKILL_FIGHT_STAT_SINGLE:
						if (pstAni->stObj.pstPlayer->stOnline.State & CS_STAT_FIGHT )
						{
							SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						}
						break;
					default:
						break;
				}
				break;
			case OBJ_MONSTER:
				pet_hit_mon(pstEnv, pstRolePet, pstAni->stObj.pstMon, &stAtkProc);
				SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
				break;
			case OBJ_NPC:
				pet_hit_mon(pstEnv, pstRolePet, &pstAni->stObj.pstNpc->stMon, &stAtkProc);
				SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
				break;
			case OBJ_PET:
				pstOwner = pstAni->stObj.stZonePet.pstOwner;

				pet_hit_pet(pstEnv, pstRolePet, &pstAni->stObj.stZonePet, &stAtkProc);
				switch(pstSkillDef->FightStat)
				{
					case SKILL_FIGHT_STAT_BOTH:
						SET_PLAYER_FIGHT(pstEnv, pstOwner, pstEnv->pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						break;
					case SKILL_FIGHT_STAT_SINGLE:
						if (pstOwner->stOnline.State & CS_STAT_FIGHT )
						{
							SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
						}
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}

		//攻击命中
		if(stAtkProc.iHurt && iRet == 0)
		{
			iRet = stAtkProc.iHurt;
		}
	}

	if (stList.iObj > 0 && pstSkillDef->FightStat == SKILL_FIGHT_STAT_BOTH)
	{
		SET_PLAYER_FIGHT(pstEnv, pstPlayer, pstEnv->pstAppCtx->stCurr.tv_sec, FIGHT_STAT_DUR);
	}
	
	make_hit_res(pstEnv, &stAtkProc, pstHits->Hits, pstHits->TotalHit, &stPkg);
	z_send_player_hit_res(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAtkPos, &stPkg);

	//宠物死亡列表处理
	z_pet_beatk_die(pstEnv, pstHits);
	
	//死亡列表处理
	for (i=0; i<pstZoneDeadList->iCount; i++)
	{
		pstDead = &pstZoneDeadList->astDeads[i];
		switch(pstDead->stAtkee.iType)
		{
			case OBJ_PLAYER:
				player_die(pstEnv->pstAppCtx, pstEnv, pstDead->stAtkee.stObj.pstPlayer, &pstDead->stAtker, 0);
				break;
			case OBJ_MONSTER:
				pstMonDef = z_get_mon_def(pstEnv, pstDead->stAtkee.stObj.pstMon);
				if (pstMonDef)
				{
					z_mon_die(pstEnv->pstAppCtx, pstEnv, pstDead->stAtkee.stObj.pstMon, pstMonDef, &pstDead->stAtker, 0, 1);
				}

				/*
					if (MISC_FLAG_TRACE & pstPlayer->stOnline.cMiscFlag)
					{
						task_trace_mon(pstEnv, pstPlayer, pstDead->stAtkee.stObj.pstMon->iDefIdx);
					}
				*/
				break;
			default:
				break;
		}
	}

	//处理怪物ai 触发
	for (i=0; i<stAtkProc.stAiList.iCount; i++)
	{
		mon_ai_stat(pstEnv, stAtkProc.stAiList.apstMon[i], &stAtkProc.stAiList.astMonAi[i]);
	}

	//要在反击类攻击的前面结束
	if(stAtkProc.cQieCuoEnd)
	{
		Player *pstQiecuoer;
		
		pstQiecuoer = z_id_player(pstEnv->pstAppCtx, pstEnv, pstPlayer->stOnline.stQiecuoInfo.iMemID);
		if(pstQiecuoer)
		{
			z_player_qiecuo_end(pstEnv, pstQiecuoer, 0);
		}

		z_player_qiecuo_end(pstEnv, pstPlayer, 1);
	}

	//memset (&pstPlayer->stOnline.stFitOneline, 0,sizeof(pstPlayer->stOnline.stFitOneline));

	return iRet;
}

int z_pet_campaign_clear(Player *pstPlayer)
{
	pstPlayer->stOnline.State &= ~CS_STAT_CAMPAIGN_PRE;

	return 0;
}

int z_pet_campaign_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkg;

	UNUSED(pstAppCtx);
	
	stPkg.Body.PetPreBrk = pstPlayer->iMemID;
		
	z_pet_campaign_clear(pstPlayer);


	Z_CSHEAD_INIT(&stPkg.Head, PET_CAMPAIGN_BRK_SVR);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}


int z_pre_break_check(TAPPCTX* pstAppCtx, struct timeval *pstPretime, int iMaxPre)
{
	long long llMs;
	struct timeval stDiff;
	
	TV_DIFF(stDiff, pstAppCtx->stCurr, *pstPretime);
	TV_TO_MS(llMs, stDiff);
	
	if ( llMs  < iMaxPre*50/100.0)
	{
		return 1;
	}
	else if (llMs <iMaxPre*85/100.0)
	{
		int iRnd;

		iRnd =(int) RAND1(10000);
		if (iRnd < 5000)
		{
			return 1;
		}
	}	

	return 0;
}

int z_skill_pre_time_add(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iPreTimeMul;
	//int iPreTimeMulDec;	//comment by paraunused.pl
	ZoneSkill *pstZoneSkill;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;
	int irand = 0;
	int iBrkRand = 0;
	SKILLDEF *pstSkillDef = NULL;
	SKILLDEF *pstOriginDef;
	long long llPreparedTime = 0;
	struct timeval stDiff;
	CSPKG stPkg;
	CSACTIONRES *pstActionRes = &stPkg.Body.ActionRes;
	
	//固定20%回退
	iPreTimeMul = 20;
	irand = RAND1(100);

	pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstAtkInfo->unSkillID);
	if(!pstZoneSkill)
	{
		return -1;
	}
	pstOriginDef = z_find_skill_zoneskill(pstEnv,pstZoneSkill);
	if (!pstOriginDef)
	{
		return -1;
	}

	pstSkillDef = z_get_rune_skill(pstEnv,pstZoneSkill,pstOriginDef);
	if (!pstSkillDef)
	{
		pstSkillDef = pstOriginDef;
	}
	
	//回退概率 = 30% * (1 - 累积读条时间t/吟唱时间T)
	TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstAtkInfo->stPrepare);
	TV_TO_MS(llPreparedTime, stDiff);

	if(pstSkillDef->PrepareType == PREPARE_TYPE_COLLECT)
	{
		if (llPreparedTime > pstSkillDef->JiqiTime*100.0)
		{
			llPreparedTime = pstSkillDef->JiqiTime*100.0;
		}
		
		iBrkRand = 30 * (1- llPreparedTime/(pstSkillDef->JiqiTime*100.0));
	}
	else if (pstSkillDef->PrepareType == PREPARE_TYPE_SING)
	{
		if (llPreparedTime > pstSkillDef->PrepareTime*100.0)
		{
			llPreparedTime = pstSkillDef->PrepareTime*100.0;
		}
		
		iBrkRand = 30 * (1- llPreparedTime/(pstSkillDef->PrepareTime*100.0));
	}
	else
	{
		return -1;
	}
	
	
	if( llPreparedTime <=0 ||
		irand >= iBrkRand)
	{
		return -1;
	}
	
	//天赋 吟唱打退时间比率
	/*iPreTimeMulDec = genius_reduce_pre_time_rate_by_atk(pstEnv->pstAppCtx, pstEnv,  pstPlayer);
	if(iPreTimeMulDec <= iPreTimeMul )
	{
		iPreTimeMul -= iPreTimeMulDec;
	}
	else 
	{
		iPreTimeMul = 0;
	}
	if( 0 == iPreTimeMul)
	{
		return 0;
	}*/

	if( pstSkillDef->JiqiTime > 0)
	{
		TV_ADD_MS(pstAtkInfo->stPrepare, pstAtkInfo->stPrepare, (int)(pstSkillDef->JiqiTime * 100 * iPreTimeMul/100.0) );
	}
	else
	{
		TV_ADD_MS(pstAtkInfo->stPrepare, pstAtkInfo->stPrepare, (int)(pstSkillDef->PrepareTime * 100 * iPreTimeMul/100.0) );
	}
		
	
	if (pstEnv->pstAppCtx->stCurr.tv_sec < pstAtkInfo->stPrepare.tv_sec ||
		(pstEnv->pstAppCtx->stCurr.tv_sec == pstAtkInfo->stPrepare.tv_sec && 
			pstEnv->pstAppCtx->stCurr.tv_usec < pstAtkInfo->stPrepare.tv_usec))
	{
		TV_CLONE(pstAtkInfo->stPrepare, pstEnv->pstAppCtx->stCurr);
	}

	TV_DIFF(stDiff, pstEnv->pstAppCtx->stCurr, pstAtkInfo->stPrepare);
	TV_TO_MS(llPreparedTime, stDiff);
	
	pstActionRes->ActionID = ACTION_PREPARE_ADD_BY_HIT;
	pstActionRes->ID = pstPlayer->iMemID;
	pstActionRes->Data.PrepareAddbyHit.SkillID= pstSkillDef->SkillID;
	pstActionRes->Data.PrepareAddbyHit.Level = pstSkillDef->Level;
	pstActionRes->Data.PrepareAddbyHit.PrepareTimeUse = llPreparedTime;
	pstActionRes->Data.PrepareAddbyHit.SvrTime.BenchTimeSec = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstActionRes->Data.PrepareAddbyHit.SvrTime.BenchTimeUSec = pstEnv->pstAppCtx->stCurr.tv_usec;
	
	Z_CSHEAD_INIT(&stPkg.Head, ACTION_RES);
	z_sendpkg_playerview(pstEnv->pstAppCtx, pstEnv, pstPlayer, 1, &stPkg, 0);
	
	return 0;
}

int z_atkee_prepare_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iHurt, int iFlag)
{
	if ((pstPlayer->stOnline.State & CS_STAT_PREPARE) 
		|| (pstPlayer->stOnline.State & CS_STAT_MOVE_PREPARE) )
	{
		//CSFITTINGSROW *pstFittings;
		//ZoneSkill *pstZoneSkill;
		//AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;

		//pstZoneSkill = z_get_player_zoneskill(pstPlayer, pstAtkInfo->unSkillID);
		//if (NULL == pstZoneSkill)
		//{
		//	return -1;
		//}

		//切地图等强制打断
		if (iFlag == 1)
		{
			z_skill_break(pstAppCtx, pstEnv,  pstPlayer,SKILL_BRK_NORMAL,pstPlayer->stOnline.stAtkInfo.unSkillID);
		}
		else
		{
			//技能吟唱延时
			//z_skill_pre_time_add(pstEnv, pstPlayer);
		}
	}
	
	//打坐
	if (pstPlayer->stOnline.State & ( CS_STAT_SIT|CS_STAT_GAOJI_SIT ))
	{
		if (iHurt > 0)
		{
			player_sit_break(pstAppCtx, pstEnv, pstPlayer);
		}
		//player_sit_break(pstAppCtx, pstEnv, pstPlayer);
	}

	return 0;
}

// 被暴击触发天赋的关键字加hp
int genius_add_hp_by_heavyhit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstAtkee,CSONEHIT *pstOneHit,int  iHeavyHit)
{
	int iAddHp = 0;
	
	if( iHeavyHit !=  1)
	{
		return 0;
	}

	iAddHp = genius_add_hp_by_heavy_hit( pstAppCtx, pstEnv, pstAtkee);
	if( iAddHp != 0)
	{
		if (pstOneHit->TotalHurt < MAX_HURT)
		{
			pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_HP;
			pstOneHit->Hurts[pstOneHit->TotalHurt].Value = iAddHp;
			pstOneHit->TotalHurt++;
		}
	}
	
	return 0;
}
// 设置技能的冷却时间
int  player_set_skill_cool_time(ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short unSkillID,unsigned short  SkillCDTime)
{
	int i;
	ZoneSkillList *pstZoneSkillList = &pstPlayer->stOnline.stSkillList;
	ZoneSkill *pstZoneSkill = NULL;
	CSPKG stPkg;
	CSSKILLINFO *pstSkillInfo = &stPkg.Body.SkillInfo;

	pstSkillInfo->Type = SKILL_COOL_TIME;
	for (i =0; i < pstZoneSkillList->nSkillNum; i++)
	{
		pstZoneSkill = &pstZoneSkillList->astZoneSkills[i];
		
		if (unSkillID && pstZoneSkill->stSkillData.SkillID == unSkillID)
		{
			pstZoneSkill->stCoolEnd.tv_usec = pstEnv->pstAppCtx->stCurr.tv_usec+SkillCDTime;
			//服务器时间比客户端时间 要先跑
			pstZoneSkill->stCoolEnd.tv_usec = pstZoneSkill->stCoolEnd.tv_usec -1;

			pstSkillInfo->Data.SetSkillCDTime.SkillID = unSkillID;
			pstSkillInfo->Data.SetSkillCDTime.SkillCDTime = SkillCDTime;
			Z_CSHEAD_INIT(&stPkg.Head, SKILL_INFO);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkg);

			return 0;
		}
	}
	return 0;	
}
int z_mon_prepareatk_player(ZONESVRENV* pstEnv, Player *pstPlayer,CSPREPAREATKBYHIT *pstPrepareAtkByHit)
{
	ZoneObjList stObjList;
	MONSTERDEF *pstMonDef;
	SKILLDEF *pstSkillDef;
	Monster *pstMon;
	MonAtkInfo *pstMonAtkInfo = NULL;
	struct timeval stDiff;
	long long llMs = 0;
	RESPOS stAtkPos;

	memset(&stObjList,0,sizeof(stObjList));
	
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstPrepareAtkByHit->AtkerID);
	if (NULL == pstMon || MONSTER_DIE == pstMon->cStat || 0 >= pstMon->iHP || pstMon->bSubType >= MON_SUB_TYPE_TMP)
	{
		return -1;
	}
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef )
	{
		return -1;
	}
	
	pstMonAtkInfo = &pstMon->stMonAtkInfo;
	if( pstPrepareAtkByHit->SkillID != pstMonAtkInfo->unSkillID)
	{
		return -1;
	}

	//吟唱攻击结束了,不处理
	TV_DIFF(stDiff, pstMon->stMonAtkInfo.stAtkStatEnd, pstEnv->pstAppCtx->stCurr);
	TV_TO_MS(llMs, stDiff);
	if (0 >= llMs)
	{
		return 0;
	}

	//不是吟唱攻击技能不处理
	pstSkillDef = z_find_skill(pstEnv, pstMonAtkInfo->unSkillID, pstMonAtkInfo->bSkillLevel, &pstMonAtkInfo->iSkillIdx);
	if (NULL == pstSkillDef ||  pstSkillDef->SkillType != SKILL_TYPE_PREPARE_ATK )
	{
		return -1;
	}
	
	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv,  pstMon, 0);
	}

	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}

	if(is_player_ride_on_machine(pstPlayer))
	{
		Monster *pstMachine = machine_get_by_player(pstEnv, pstPlayer);
		if(!pstMachine)
		{
			return -1;
		}

		if (pstMachine->cDartCarNotAtk == 1)
		{
			return -1;
		}
		
		stObjList.iObj =1;
		stObjList.astObj[0].iType = OBJ_MONSTER;
		stObjList.astObj[0].stObj.pstMon = pstMachine;
		pstMon->stMonAtkInfo.stAtkPos = pstMachine->stCurrPos;
	}
	else
	{
		stObjList.iObj =1;
		stObjList.astObj[0].iType = OBJ_PLAYER;
		stObjList.astObj[0].stObj.pstPlayer = pstPlayer;

		pstMon->stMonAtkInfo.stAtkPos = pstPlayer->stRoleData.Pos;
	}
		
	stAtkPos = pstMon->stMonAtkInfo.stAtkPos;
	if (stObjList.iObj > 0)
	{
		z_mon_hit_notify(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, pstSkillDef, &stObjList);
	}
	 pstMon->stMonAtkInfo.stAtkPos = stAtkPos;
	
	return 0;
}

int z_mon_normalatk_player(ZONESVRENV* pstEnv, Player *pstPlayer,CSNORMALATKBYHIT *pstNormalAtkByHit)
{
	ZoneObjList stObjList;
	MONSTERDEF *pstMonDef = NULL;
	SKILLDEF *pstSkillDef = NULL;
	Monster *pstMon = NULL;
	RESPOS stAtkPos;
	ZoneAni stAtkee;
	int iHaveSkill = 0;
	PetFightShiOnline *pstPetTiShen = NULL;
	MonAtkInfo *pstMonAtkInfo = NULL;

	memset(&stObjList,0,sizeof(stObjList));
	memset(&stAtkee,0,sizeof(stAtkee));
	
	pstMon = z_id_monster(pstEnv->pstAppCtx, pstEnv, pstNormalAtkByHit->AtkerID);
	if (NULL == pstMon || MONSTER_DIE == pstMon->cStat || 0 >= pstMon->iHP || pstMon->bSubType <= MON_SUB_TYPE_TMP)
	{
		return -1;
	}
	pstMonDef = z_get_mon_def(pstEnv, pstMon);
	if (NULL == pstMonDef )
	{
		return -1;
	}

	pstMonAtkInfo = &pstMon->stMonAtkInfo;

	stAtkee.iType = OBJ_PLAYER;
	stAtkee.stObj.pstPlayer = pstPlayer;
	mon_select_atkskill(pstEnv, pstMon, &stAtkee, &iHaveSkill);

	pstSkillDef = z_find_skill(pstEnv, pstMonAtkInfo->unSkillID, pstMonAtkInfo->bSkillLevel, &pstMonAtkInfo->iSkillIdx);
	if (NULL == pstSkillDef)
	{
		return -1;
	}
	
	if(pstNormalAtkByHit->HitTarget > PET_ID_BASE)
	{
		pstPetTiShen = z_pet_tishen(pstEnv, pstPlayer);
		if (NULL == pstPetTiShen || !is_tishen_skill(pstSkillDef))
		{
			stObjList.iObj =1;
			stObjList.astObj[0].iType = OBJ_PLAYER;
			stObjList.astObj[0].stObj.pstPlayer = pstPlayer;
		}
		else
		{
			stObjList.iObj =1;
			stObjList.astObj[0].iType = OBJ_PET;
			stObjList.astObj[0].stObj.stZonePet.pstOwner = pstPlayer;
			stObjList.astObj[0].stObj.stZonePet.pstPetFightShi = pstPetTiShen;
			stObjList.astObj[0].stObj.stZonePet.pstRolePet = z_find_role_pet(pstPlayer, pstPetTiShen->stRolePetOnline.ullWID, NULL);
		}		
	}
	else if(machine_get_by_player(pstEnv,pstPlayer))
	{
		stObjList.iObj =1;
		stObjList.astObj[0].iType = OBJ_MONSTER;
		stObjList.astObj[0].stObj.pstMon = machine_get_by_player(pstEnv,pstPlayer);
	}
	else
	{
		stObjList.iObj =1;
		stObjList.astObj[0].iType = OBJ_PLAYER;
		stObjList.astObj[0].stObj.pstPlayer = pstPlayer;
	}
	
		
	if (pstMon->cMoving)
	{
		z_monster_move_curr(pstEnv->pstAppCtx, pstEnv,  pstMon, 0);
	}
	if (pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstEnv->pstAppCtx, pstEnv, pstPlayer, 0);
	}


	stAtkPos = pstMon->stMonAtkInfo.stAtkPos;
	pstMon->stMonAtkInfo.stAtkPos = pstPlayer->stRoleData.Pos;

	if (stObjList.iObj > 0)
	{
		z_mon_hit_notify(pstEnv->pstAppCtx, pstEnv, pstMon, pstMonDef, pstSkillDef, &stObjList);
	}

	 pstMon->stMonAtkInfo.stAtkPos = stAtkPos;
	
	return 0;
}

//怪物吟唱攻击
int z_prepareatk_mon_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSPREPAREATKINFO *pstPrepareAtkInfo = &pstCsPkg->Body.PrepareAtkInfo;
	UNUSED(pstFrameHead);
	UNUSED(pstAppCtx);

	switch(pstPrepareAtkInfo->PrepareAtkOp)
	{
		case  PREPAREATK_OP_AKEE:
			z_mon_prepareatk_player( pstEnv, pstPlayer,&pstPrepareAtkInfo->PrepareAtkData.PrepareAtkByHit);
			break;
		case  NORMALATK_OP_AKEE:
			z_mon_normalatk_player( pstEnv, pstPlayer,&pstPrepareAtkInfo->PrepareAtkData.NormalAtkByHit);
			break;
		default:
			break;
	}
	return 0;
}

int z_player_hurt_status_absorb(ZONESVRENV* pstEnv,Player *pstPlayer, CSONEHIT *pstOneHit, int *piHurt)
{
	ZoneAni stAni;

	stAni.iType = OBJ_PLAYER;
	stAni.stObj.pstPlayer = pstPlayer;

	return z_obj_hurt_status_absorb(pstEnv,&stAni, pstOneHit, piHurt);
	
}
int z_mon_hurt_status_absorb(ZONESVRENV* pstEnv,Monster *pstMon, CSONEHIT *pstOneHit, int *piHurt)
{
	ZoneAni stAni;

	stAni.iType = OBJ_MONSTER;
	stAni.stObj.pstMon= pstMon;

	return z_obj_hurt_status_absorb(pstEnv,&stAni, pstOneHit, piHurt);
}

int z_state_sorb_end_addhp(ZONESVRENV* pstEnv, Player *pstPlayer, int iID, int iLvl)
{
	STATUSDEF *pstDef = z_find_status(pstEnv,iID,iLvl);
	STATUSRESULT *pstResult;

	if (pstDef)
	{
		pstResult = z_get_status_result(pstDef, STATUS_RESULT_HPCHG_STATE_DESTROY);
		if (pstResult)
		{
			PlayerChagAttrVal stAttr;
			stAttr.AttrNum = 0;
			int iOldHP = pstPlayer->stRoleData.RoleDetail.HPCurr;
			int iOldMP = pstPlayer->stRoleData.RoleDetail.MPCurr;
			stAttr.AttrList[stAttr.AttrNum].AttrID = ATTR_ID_HP;
			stAttr.AttrList[stAttr.AttrNum].Value.HP = pstResult->ResultVal1;
			stAttr.AttrList[stAttr.AttrNum].nFlag = 0;
			stAttr.AttrNum = 1;
			z_attr_chg(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stAttr, 1);
			player_hp_action(pstEnv, pstPlayer, iOldHP, iOldMP, 0);
		}
	}
	return 0;
}
// 状态对伤害的吸收
int z_obj_hurt_status_absorb(ZONESVRENV* pstEnv,ZoneAni *pstAni, CSONEHIT *pstOneHit, int *piHurt)
{

	ZoneStatus *pstZoneStatus= NULL;

	
	if(!pstAni )
	{
		return -1;
	}

	switch(pstAni->iType)
	{
		case OBJ_PLAYER:
			pstZoneStatus = z_get_player_zonestatus_by_resultid(pstEnv,pstAni->stObj.pstPlayer,STATUS_RESULT_ABSORB_HURT);
			break;
		case OBJ_MONSTER:
			pstZoneStatus = z_get_mon_zonestatus_by_resultid(pstEnv,pstAni->stObj.pstMon,STATUS_RESULT_ABSORB_HURT);
			break;
		default:
			break;
	}
	
	if(!pstZoneStatus || pstZoneStatus->iVal <= 0 )
	{
		return -1;
	}

	// 伤害被完全吸收 
	if(pstZoneStatus->iVal >= *piHurt)
	{
		if (pstOneHit->TotalHurt < MAX_HURT)
		{
			pstOneHit->Hurts[pstOneHit->TotalHurt].HurtType = HURT_TYPE_SBSORB;
			pstOneHit->Hurts[pstOneHit->TotalHurt].Value = *piHurt;
			pstOneHit->TotalHurt++;
		}

		pstZoneStatus->iVal -= *piHurt;
		*piHurt = 0;
	}
	else
	{
		*piHurt -= pstZoneStatus->iVal;
		pstZoneStatus->iVal = 0;
	}

	if( 0 == pstZoneStatus->iVal)
	{
		//伤害吸收完了
		switch(pstAni->iType)
		{
			case OBJ_PLAYER:
				z_player_del_status(pstEnv->pstAppCtx, pstEnv, pstAni->stObj.pstPlayer, pstZoneStatus->unStatusID, 1, 0);
				z_state_sorb_end_addhp(pstEnv, pstAni->stObj.pstPlayer, pstZoneStatus->unStatusID,pstZoneStatus->bLevel);
				break;
			case OBJ_MONSTER:
				if(0==z_mon_del_status(pstEnv->pstAppCtx,  pstEnv, pstAni->stObj.pstMon, pstZoneStatus->unStatusID, 0))
				{
					z_mon_send_status(pstEnv->pstAppCtx, pstEnv, pstAni->stObj.pstMon);
				}
				break;
			default:
				break;
		}
	}
	
	return 0;
}

int is_player_enemy_mon(ZONESVRENV* pstEnv, Player *pstPlayer, Monster *pstMon)
{
	int iRelation = 0;
	
	iRelation = player_get_hit_mon_relation(pstEnv, pstPlayer, pstMon);
	if(iRelation & SKILL_TARGET_ENEMY_MON)
	{
		return 1;
	}

	return 0;
}

int is_player_enemy_player(ZONESVRENV* pstEnv, Player *pstPlayer, Player *pstOtherPlayer)
{
	int iRelation = 0;
	
	iRelation = z_get_hit_player_relation(pstEnv, pstPlayer, pstOtherPlayer);
	if(iRelation & SKILL_TARGET_ENEMY_PLAYER)
	{
		return 1;
	}

	return 0;
}

int is_mon_enemy_mon(ZONESVRENV* pstEnv, Monster *pstMon, Monster *pstAtkee)
{
	int iRelation = 0;

	iRelation = z_mon_hit_mon_relation(pstEnv, pstMon, pstAtkee);
	if(iRelation & SKILL_TARGET_ENEMY_MON)
	{
		return 1;
	}
		
	return 0;
}

int is_mon_enemy_player(ZONESVRENV* pstEnv, Monster *pstMon, Player *pstAtkee)
{
	/*
	Player *pstOwner;
	
	pstOwner = z_id_player(pstEnv->pstAppCtx, pstEnv, pstMon->iOwnerID);
	if (pstOwner)
	{
		return is_player_enemy_player(pstEnv, pstOwner, pstAtkee);
	}
	*/
	
	return z_mon_hit_player_filter(pstEnv, pstMon, pstAtkee);
}
//存储闪电链的相关数据
LightningAtkInfo *z_storage_lightning_link_info(Player *pstPlayer)
{
	int i=0;
	PlayerOnline *pststOnline = &pstPlayer->stOnline;
	AtkInfo *pstAtkInfo = &pstPlayer->stOnline.stAtkInfo;

	for( i=0; i<MAX_LIGHTNING_SKILL_NUM; i++)
	{
		if( pststOnline->stLightningAtkInfo[i].iJumpNum <= 0 )
		{
			pstAtkInfo->iAtkLightningIdx = i;
			return &pststOnline->stLightningAtkInfo[i];
		}
	}

	pstAtkInfo->iAtkLightningIdx = 0;
	return &pststOnline->stLightningAtkInfo[0];
}


int z_get_in_range_team_member_list(ZONESVRENV* pstEnv, Player *pstPlayer, SKILLDEF *pstSkillDef,  ZoneObjList *pstZoneObjList, RESPOS *pstHitPos)
{
	RESPOS stPos;
	int iRadius  = 0;
	ZoneTeam *pstZoneTeam = NULL;
	TEAMINFO *pstTeamInfo = NULL;
	int i=0;
	Player *pstPlayerMember = NULL;
	TEAMMEMBER *pstTeamMember = NULL;
	ZoneAni *pstAni = NULL;
	RESRANGE stMainRange = pstSkillDef->MainRange;

	if (stMainRange.RangeType == SKILL_RANGE_NONE || stMainRange.RangeType == SKILL_RANGE_SIGLE)
	{
		stMainRange.RangeType = SKILL_RANGE_CIRCLE;
		stMainRange.RangeVal1 = pstSkillDef->Distance;
		stMainRange.RangeVal2 = 0;
	}

	pstZoneObjList->iObj = 0;
	memset(&stPos,0,sizeof(stPos));
	pstZoneTeam = player_team_get(pstEnv, pstPlayer);

	if(pstZoneTeam)
	{
		pstTeamInfo = &pstZoneTeam->stTeamInfo;
		for (i=0; i<pstTeamInfo->MemberNum; i++)
		{
			pstTeamMember = &pstTeamInfo->TeamMember[i];
			pstPlayerMember = z_name_player(pstEnv->pstAppCtx, pstEnv,pstTeamMember->MemberName);
			if ( pstPlayerMember && pstPlayer!=pstPlayerMember && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
			{
				if (pstPlayer->stRoleData.Map != pstPlayerMember->stRoleData.Map)
				{
					continue;
				}
				
				stPos = pstPlayerMember->stRoleData.Pos;
				if (0 > z_in_range(&stPos, &pstPlayer->stRoleData.Pos,pstHitPos, &stMainRange, pstSkillDef->Distance, iRadius, 0))
				{
					continue;
				}
				pstAni = &pstZoneObjList->astObj[pstZoneObjList->iObj];
				pstAni->iType = OBJ_PLAYER;
				pstAni->stObj.pstPlayer = pstPlayerMember;
				pstZoneObjList->iObj++;
			}
		}
	}

	stPos = pstPlayer->stRoleData.Pos;
	if (0 > z_in_range(&stPos, &pstPlayer->stRoleData.Pos,pstHitPos, &stMainRange, pstSkillDef->Distance, iRadius, 0))
	{
		return 0;
	}
	pstAni = &pstZoneObjList->astObj[pstZoneObjList->iObj];
	pstAni->iType = OBJ_PLAYER;
	pstAni->stObj.pstPlayer = pstPlayer;
	pstZoneObjList->iObj++;

	return 0;
}

int z_do_skill_result_add_hp_by_hurt_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, AtkProcess *pstAtkProcess)
{
	int i = 0;
	Player *pstAtkee = NULL;
	AtkeeInfo stAtkeeInfo; //被攻击者信息
	DYNATTRLIST *pstAtkeeDyn = &stAtkeeInfo.stDyn;
	SKILLDEF * pstSkillDef = pstAtkProcess->stAtkerInfo.pstSkillDef;
	SKILLRESULT *pstSkillResult = NULL;
	ZoneObjList stZoneObjList;
	CSHITS *pstHits = &pstAtkProcess->stHits;
	CSONEHIT *pstOneHit = NULL;
	
	pstSkillResult = z_get_skill_result(pstSkillDef, RESULT_ADD_HP_BY_HURT_SKILL);
	if(!pstSkillResult)
	{
		return -1;
	}
	memset(&stZoneObjList,0,sizeof(stZoneObjList));
	
	z_get_in_range_team_member_list(pstEnv, pstPlayer, pstSkillDef,  &stZoneObjList, &pstAtkProcess->stAtkerInfo.stAtkPos);

	for(i=0; i<stZoneObjList.iObj; i++)
	{
		if( stZoneObjList.astObj[i].iType != OBJ_PLAYER ||
			pstHits->TotalHit >= MAX_SKILL_TARGET )
		{
			continue;
		}
		pstAtkee = stZoneObjList.astObj[i].stObj.pstPlayer;
		memcpy(pstAtkeeDyn, &pstAtkee->stOnline.stDyn, sizeof(DYNATTRLIST));
		stAtkeeInfo.stAtkee.iType = OBJ_PLAYER;
		stAtkeeInfo.stAtkee.stObj.pstPlayer = pstAtkee;

		pstHits->TotalHit++;
		pstOneHit = &pstHits->Hits[pstHits->TotalHit-1];
		pstOneHit->ID = pstAtkee->iMemID;
		pstOneHit->TotalHurt = 0;
		z_do_skill_result_add_hp(pstAppCtx, pstEnv, pstAtkProcess, &stAtkeeInfo, pstSkillResult);
		pstOneHit->HP = pstAtkee->stRoleData.RoleDetail.HPCurr;

		one_hit_notify(pstEnv, pstAtkProcess, pstOneHit, 0);
	}

	return 0; 
}

//伤害转换成附近范围队员的hp
int z_do_skill_result_hurt_chg_team_hp(ZONESVRENV * pstEnv, Player *pstPlayer,AtkProcess *pstAtkProcess, 
													const RESPOS *pstAtkPos, int iHurt)
{
	int i =0;
	ZoneObjList stZoneObjList;
	SKILLDEF * pstSkillDef = pstAtkProcess->stAtkerInfo.pstSkillDef;
	SKILLRESULT *pstSkillResult = NULL;
	Player *pstTeamPlayer = NULL;
	RESPOS stAtkPos;
	int iOldHP;

	stAtkPos = *pstAtkPos;
	pstSkillResult = z_get_skill_result(pstSkillDef, RESULT_MERGE_DEBUFF);
	if(!pstSkillResult)
	{
		return -1;
	}
	memset(&stZoneObjList,0,sizeof(stZoneObjList));
	if(iHurt <0)
	{
		iHurt = -iHurt;
	}

	iHurt = iHurt * (1+pstSkillResult->SkillResultVal3/100.0) + pstSkillResult->SkillResultVal4;
	z_get_in_range_team_member_list(pstEnv, pstPlayer, pstSkillDef,  &stZoneObjList, &stAtkPos);

	if(stZoneObjList.iObj<=0 || iHurt<=0)
	{
		return -1;
	}

	for(i=0; i<stZoneObjList.iObj; i++)
	{
		
		if( stZoneObjList.astObj[i].iType != OBJ_PLAYER )
		{
			continue;
		}
		pstTeamPlayer = stZoneObjList.astObj[i].stObj.pstPlayer;
		iOldHP = pstTeamPlayer->stRoleData.RoleDetail.HPCurr;

		player_add_hp(pstEnv, pstTeamPlayer,iHurt, 0,0);
		if(pstTeamPlayer->stRoleData.RoleDetail.HPCurr != (unsigned int)iOldHP)
		{
			player_hp_action(pstEnv, pstTeamPlayer, 
				iOldHP, pstTeamPlayer->stRoleData.RoleDetail.MPCurr, pstPlayer->iMemID);
		}
	}

	return 0;
}

//检查有技能替换条的操作
int check_replace_skill(ZONESVRENV* pstEnv, Player *pstPlayer, int iReplaceOP)
{
	int iRet = 0;
	ROLETMPSYS *pstRoleTmpSys = &pstPlayer->stRoleData.MiscInfo.TmpSys;
	
	if (is_player_ride_on_machine(pstPlayer) ||
		(REPLACE_SKILL_OP_CLANITEM == iReplaceOP && 
		 (is_player_on_ride(pstPlayer) || (pstPlayer->stOnline.State & CS_STAT_HITCH_RIDE ))))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL27);
		return -1;
	}
	else if (MISC_FLAG_WUSHUANG & pstPlayer->stOnline.cMiscFlag)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_SKILL25);	
		return -1;
	}

	if (pstRoleTmpSys->TmpItemID && (REPLACE_SKILL_OP_TMP != iReplaceOP))
	{
		iRet = tmp_item_drop(pstEnv, pstPlayer,0);
	}

	if(0 == iRet && pstPlayer->stOnline.ClanItemID>0 &&  REPLACE_SKILL_OP_CLANITEM != iReplaceOP)
	{
		iRet = player_drop_clan_tem(pstEnv,pstPlayer);
	}

	/*
	switch(iReplaceOP)
	{
		case REPLACE_SKILL_OP_CLANITEM:
			
			break;
		case REPLACE_SKILL_OP_MACHINE:
			
			break;
		case REPLACE_SKILL_OP_TMP:
			
			break;
		case REPLACE_SKILL_OP_WUSHUANG:
			
			break;
		default:
			break;
	}*/

	return iRet;
}

int IsSkillThrough(ZONESVRENV* pstEnv, ZONEMAPINST *pstMapInst, SKILLDEF *pstSkillDef ,TMAPMASK* pstMask, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2)
{
	if (!(pstSkillDef->NoSkillStop&NOSKILLSTOP_TYPE_MAGIC))
	{
		if (!IsPixelLineThrough(pstMask, lPixelX1, lPixelY1, lPixelX2, lPixelY2, maskMagic) ||
			!dyn_msk_check(pstEnv, pstMapInst, pstMask, lPixelX1, lPixelY1, lPixelX2, lPixelY2,0, DYN_MSK_MAGIC))
		{
			return 0;
		}
	}

	if (10 > pstSkillDef->ThroughLoMagic && !PixelLineThroughCount(pstMask, lPixelX1, lPixelY1, lPixelX2, lPixelY2, maskLoMagic, pstSkillDef->ThroughLoMagic))
	{
		return 0;
	}

	return 1;
}

int is_in_miss_time_player(struct timeval *pstCltTime, Player *pstPlayer)
{
	struct timeval stDiff;
	long long llMs = 0;
	
	TV_DIFF(stDiff, *pstCltTime, pstPlayer->stOnline.stSkillMissTime);
	TV_TO_MS(llMs, stDiff);
	if ( llMs <= g_iMissTime)
	{
		return 1;
	}

	return 0;
}

int player_hit_team_call_rongyan_mon(ZONESVRENV*pstEnv,Player *pstAtker, Monster *pstAtkee)
{
	ZoneTeam *pstTeam = NULL;
	TEAMMEMBER *pstCaptain = NULL;
	Player *pstPlayer = NULL;
	int i;
	int iRongYanTeamCapId = pstAtkee->iRongYanTeamCapId;
	if(iRongYanTeamCapId <= 0)
	{
		return 0;
	}

	pstPlayer = z_id_player(pstEnv->pstAppCtx, pstEnv, iRongYanTeamCapId);
	if(NULL == pstPlayer)
	{
		return 0;
	}
	
	pstTeam = player_team_get(pstEnv, pstPlayer);
	if(NULL == pstTeam)
	{
		return 0;
	}

	pstCaptain = player_team_get_captain(pstTeam);
	if(pstCaptain == NULL)
	{
		return 0;
	}
	
	for(i=0;i<pstTeam->stTeamInfo.MemberNum;i++)
	{		
		if(0 == strcmp(pstTeam->stTeamInfo.TeamMember[i].MemberName, pstAtker->stRoleData.RoleName))
		{
			return 0;
		}
	}
	
	return -1;
}

int z_skill_transmit_spouse_pos_check(TAPPCTX* pstAppCtx,ZONESVRENV*pstEnv,Player *pstPlayer)
{
	MapIndex *pstMapIdx;
	ZONEIDX stIdx;
	ZONEMAPINST *pstMapInst;
	Player *pstPlayerOther = NULL;

	if (pstPlayer->stRoleData.Buddy.SpouseNum <= 0 || pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
		return -1;		
	}

	if((pstPlayer->stRoleData.Map >= PWORLD_MAPID_BASE) || (PLAYER_STATUS_ROLE_NOTHING == pstPlayer->eStatus))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING49);
		return -1;
	}

	if (INST_MODE_ZONE_SVR==pstEnv->iInstMode)
	{
		pstPlayerOther = z_name_player(pstAppCtx, pstEnv, pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);
	}
	else
	{
		char szName[NAME_LEN];
		unsigned short unWorldID;
		
		unWorldID = role_world_id_get(pstPlayer);
		
		snprintf(szName,sizeof(szName),"w%d"BATTLE_NAME_SYMBOL"%s",
						unWorldID,
						pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);

		pstPlayerOther = z_name_player(pstAppCtx, pstEnv, szName);
	}
	
	if (!pstPlayerOther)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
		return -1;	
	}

	if((pstPlayerOther->stRoleData.Map >= PWORLD_MAPID_BASE) || (PLAYER_STATUS_ROLE_NOTHING == pstPlayerOther->eStatus))
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING50);
		return -1;
	}

	g_SpousePos = 1;
	if (0 > z_player_goto_check(pstEnv, pstPlayer, 0, NULL))
	{
		g_SpousePos = 0;
		return -1;
	}
	
	if (player_is_zuolao(pstPlayerOther))
	{
		g_SpousePos = 0;
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING72);
		return -1;
	}
	g_SpousePos = 0;

	if(pstPlayer->stOnline.State & CS_STAT_FIGHT)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_DIANFENG36);
		return -1;
	}

	stIdx.iID = pstPlayerOther->stRoleData.Map;
	stIdx.iPos = pstPlayerOther->stOnline.iMapPos;
	pstMapInst = z_find_map_inst(pstAppCtx, pstEnv, &stIdx);
	if (NULL == pstMapInst)
	{
		return -1;
	}

	pstMapIdx = z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if (NULL == pstMapIdx)
	{
		return -1;
	}
	
	return 0;
}

int z_skill_recover_spouse_hp_check(TAPPCTX* pstAppCtx,ZONESVRENV*pstEnv,Player *pstPlayer,SKILLDEF *pstSkillDef)
{
	Player *pstPlayerOther = NULL;

	if(PLAYER_STATUS_ROLE_NOTHING == pstPlayer->eStatus || pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING74);
		return -1;
	}
		
	if (pstPlayer->stRoleData.Buddy.SpouseNum <= 0 || pstPlayer->stRoleData.Buddy.SpouseNum > MAX_BUDDY_SPOUSE)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING33);
		return -1;		
	}

	if (INST_MODE_ZONE_SVR==pstEnv->iInstMode)
	{
		pstPlayerOther = z_name_player(pstAppCtx, pstEnv, pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);
	}
	else
	{
		char szName[NAME_LEN];
		unsigned short unWorldID;
		
		unWorldID = role_world_id_get(pstPlayer);
		
		snprintf(szName,sizeof(szName),"w%d"BATTLE_NAME_SYMBOL"%s",
						unWorldID,
						pstPlayer->stRoleData.Buddy.Spouse[0].RoleName);

		pstPlayerOther = z_name_player(pstAppCtx, pstEnv, szName);
	}
	
	if (!pstPlayerOther)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING8);
		return -1;	
	}

	if(pstPlayer->stOnline.cMoving)
	{
		z_player_move_curr(pstAppCtx, pstEnv, pstPlayer, 0);
	}
	
	if(pstPlayerOther->stOnline.cMoving)
	{
		z_player_move_curr(pstAppCtx, pstEnv, pstPlayerOther, 0);
	}

	if(z_distance(&pstPlayer->stRoleData.Pos, &pstPlayerOther->stRoleData.Pos) > pstSkillDef->Distance)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING73);
		return -1;
	}

	if(CS_STAT_DEAD & pstPlayerOther->stOnline.State)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE, ZONEERR_WEDDING78);
		return -1;
	}
	
	return 0;
}

//-1 不能打，1可以打
int z_span_map_if_player_hit_mon(ZONESVRENV*pstEnv,Player *pstAtker, Monster *pstAtkee,MONSTERDEF *pstMonsterDef)
{		
	//不在跨服野外的，不判断下面条件
	if(0 > player_if_in_span_map(pstEnv,pstAtker))
	{
		return 1;
	}

	if (pstAtkee->cType != OBJ_NPC )
	{
		return 1;
	}
	
	//无阵营的不能打
	if(!is_camp_mon(pstAtkee))
	{
		return -1;
	}

	//同个阵营的不能打
	if(is_player_same_camp_mon(pstAtker, pstAtkee))
	{
		return -1;
	}

	//不是卫兵的不能打
	if(!z_is_guard(pstMonsterDef))
	{
		return -1;
	}
	
	return 1;
}

//是不是卫兵
int z_is_guard(MONSTERDEF *pstMonsterDef)
{
	if(!pstMonsterDef || pstMonsterDef->AINormalSkills[0].SkillID == 0)
	{
		return 0;
	}

	return 1;
}

//-1 不能打，1可以打
int z_span_map_if_mon_hit_mon(ZONESVRENV*pstEnv,Monster *pstMon,Monster *pstAtkee,MONSTERDEF *pstAtkeeMonsterDef)
{	
	//不在跨服野外的，不判断下面条件
	if(0 > mon_if_in_span_map(pstEnv,pstMon))
	{
		return 1;
	}

	if (pstAtkee->cType != OBJ_NPC)
	{
		return 1;
	}

	if(!is_mon_machine(pstMon))
	{
		return 1;
	}
	
	//无阵营的不能打
	if(!is_camp_mon(pstAtkee))
	{
		return -1;
	}

	//同个阵营的不能打
	if(pstMon->iCampIdx == pstAtkee->iCampIdx)
	{
		return -1;
	}

	//不是卫兵的不能打
	if(!z_is_guard(pstAtkeeMonsterDef))
	{
		return -1;
	}
	
	return 1;
}

int mon_if_in_span_map(ZONESVRENV *pstEnv,Monster *pstMon)
{
	MapIndex* pstMapIndex = NULL;
	ZONEMAPINST* pstMapInst = NULL;
	
	pstMapInst = z_find_map_inst(pstEnv->pstAppCtx, pstEnv, &pstMon->stMap);
	if(!pstMapInst)
	{
		return -1;
	}	

	pstMapIndex	= z_mappos_index(pstEnv->pstMapObj, &pstMapInst->iMapPos, pstMapInst->iMapInstID);
	if( !pstMapIndex)
	{
		return -1;
	}
	
	if(!(pstMapIndex->stMapDef.CtrlFlag & MAP_CTRL_PVP_LOAD_MAP))
	{
		return -1;
	}

	return 0;
}

