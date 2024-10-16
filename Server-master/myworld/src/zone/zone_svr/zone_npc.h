
#ifndef ZONE_NPC_H
#define ZONE_NPC_H

#include "zone_svr.h"

#define FIRST_ATKER_TIME 10
#define USE_MONDEF_BOOTYDROPTYPE 40
typedef struct {
	int iColorExpMul;
	int iResultExpMul;
	int iQiYueExpPlayerMul;
	int iQiYueExpPetMul;
	int iMonDynExpMul;
	int iNormalExpMul;
	int iPetExpMul;
	float fClanSkillExpMul;
}MonExpMul;

typedef struct {
	MonExpMul  ExpMul;
	int iMoneyMul;
	int iItemRateMul;
}MonRateMul;

#if defined(ZONE_ID_TRACKED)
int z_refresh_mon_tracked(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
                          Monster *pstMon, SKILLDEF *pstDoCallSkillDef, const char *pszFile, int iLine);

#define z_refresh_mon(app, env, mon,skill) \
	z_refresh_mon_tracked(app, env, mon,skill, __FILE__, __LINE__)
#else
int z_refresh_mon(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon,SKILLDEF *pstSkillDef);
#endif

int z_mon_die(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef, ZoneAni *pstAtker, int iNotify, int fPworldSrcipt);
MONSTERDEF * z_get_mon_def(ZONESVRENV* pstEnv, Monster* pstMon);
int mon_booty_owner(ZoneBootyPak *pstBootyPak, Player *pstPlayer);
NPCSELLLIST *z_get_npc_selllist(ZONESVRENV* pstEnv, Npc *pstNpc);
//TRANSLIST *z_get_npc_translist(ZONESVRENV* pstEnv, Npc *pstNpc);
int player_shop(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);	
NPCINFODEF *npcinfo_def_find(ZONESVRENV *pstEnv, int iNpcID, int *piNpcInfoIdx);
int z_check_npc_script(ZONESVRENV* pstEnv, Npc *pstNpc);

int z_mon_world_speech(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer, int iSpeechType, int iSpeechIdx);
int z_mon_map_speech(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer, int iSpeechType, int iSpeechIdx);
int z_mon_view_speech(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer, int iSpeechType, int iSpeechIdx);
int z_mon_speech(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer, int iBroadcastType, int iSpeechType, int iSpeechIdx);

int  z_find_actspeech_by_conditiontype(MONSTERSPEECHDEF *pstMonSpeech, int iType, int iValue);

MONSTERSPEECHDEF *z_find_monsterspeech_def(ZONESVRENV *pstEnv, unsigned int uiMonsterID, int *pIidx);

MONSTERSTATESKILLDEF *z_find_monsterstateskill_def(ZONESVRENV *pstEnv, unsigned int uiMonsterStateSkillID, int *pIidx);

int selled_del(ZONESVRENV* pstEnv,ITEMSELLSAVE *pstItemSellSave);

int z_active_npc(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount);

int z_mon_aberrants(ZONESVRENV* pstEnv, Monster* pstMon);

int z_mon_resile(ZONESVRENV* pstEnv, Monster* pstMon);

int z_refresh_mon_speech(ZONESVRENV* pstEnv, Monster* pstMon);

int mon_trace_valid_player (Monster *pstMon, Player *pstPlayer);

int shop_vip_open(ZONESVRENV* pstEnv, Player *pstPlayer);

int is_convoy_car(Monster* pstMon);

//直接给玩家所在的地方加物品
int player_add_booty(ZONESVRENV* pstEnv, Player *pstPlayer, TASKGETITEM *apstItemID,unsigned int iNum,int iviewFlag, RESPOS *pstPos);	

int z_mon_speech_ex(ZONESVRENV* pstEnv, Monster* pstMon, Player *pstPlayer,
                    const char *pszMsg, int iBroadcastType, int iSpeechType);

int is_my_convoy_car(Player *pstPlayer, Monster* pstMon);

int mon_life_action(ZONESVRENV* pstEnv, Monster *pstMon);

Player * get_atker_player(ZONESVRENV* pstEnv,  ZoneAni *pstAtker);

int z_npc_die(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Npc* pstNpc);

int is_valid_booty_team(ZONESVRENV* pstEnv , Player *pstPlayer);

int player_view_booty_valid(ZONESVRENV* pstEnv, DropItem *pstDropItem, Player *pstPlayer);

void WindingItemAxisDir(int n, int iStep, int *piDir, int *piRound);
int erea_(int iIdx,int iK,int iC,int iRn,int iCn,int iHitx,int iHity,int *piOutX,int *piOutY);
int round_(int i,int iRound, int iRoundNum,int iHitx,int iHity,int *piOutX,int *piOutY);
int arc_(ZONESVRENV *pstEnv,int i,int iRound, int iAngle,int iCn,RESPOS o,RESPOS *pstCallPos,int iAngelArc);
int booty_money_drop_ex(ZONESVRENV* pstEnv, Player *pstPlayer, Monster * pstMon, int iMoneyType, int iMoneyVal,int iIsOwnPlayer,int iUseMonDefDropType, int iFlag);
int booty_money_drop(ZONESVRENV* pstEnv, Player *pstPlayer, Monster * pstMon, int iMoneyType, int iMoneyVal);

int booty_discrete_drop_item(ZONESVRENV* pstEnv, Monster * pstMon, int iID, int iNum);

void rand_position_init();

int dropitem_create(ZONESVRENV * pstEnv, ZoneBootyGrid * pstBootyGrid, RESPOS * pstPos, int iMap, int iFromID);

int mon_booty_drop(ZONESVRENV* pstEnv, Monster *pstMon, ROLEGRID *pstRoleGrid, Player *pstPlayer);

int z_refresh_mon_comm_union_data(ZONESVRENV *pstEnv, Monster *pstMon, int iCommType);

int mon_set_camp(Monster *pstMon, int iCamp);

int z_mon_def_data(ZONESVRENV* pstEnv, Monster* pstMon, MONSTERDEF *pstMonDef);

int z_mon_speech_begin_or_end_fight(ZONESVRENV* pstEnv, Monster* pstMon,int iType);

int z_get_monsterdef_misctype(ZONESVRENV* pstEnv, Monster* pstMon);

int player_cacl_rebate( ZONESVRENV* pstEnv, Player *pstPlayer, unsigned int *uipRebateCount,
						char *pRebateTypeNum, char *pRebateTypes);

//没有刷新出地图之前设置
int mon_set_pkinfo(Monster *pstMon, int iType, tdr_ulonglong iID);
int is_my_team_convoy_car(ZONESVRENV* pstEnv,Player *pstPlayer, Monster* pstMon);



//lzk 当融焰怪物死亡时调用
int mon_rongyan_boss_die(ZONESVRENV* pstEnv,Monster* pstMon);
int mon_rongyan_player_add_exp(ZONESVRENV* pstEnv,ZoneTeam *pstTeam,Monster* pstMon);
int mon_rongyan_die_player_add_exp(ZONESVRENV* pstEnv,Player *pstPlayer,int BeginSelectMul,int MonBasicExp,int DynExpMul,int CurLevel);
NPCSELLLIST *z_get_npc_selllist_ex(ZONESVRENV* pstEnv, int iID);
int rongyan_all_status_clean(ZONESVRENV* pstEnv,ZoneTeam *pstTeam,int iTeamMemberNum);
int mon_action_map_show(ZONESVRENV * pstEnv,  PworldCampMonInfo *pstMonInfo,ZONEMAPINST *pstMapInst,
	int iOpType,int iBroadcastType,Player* pstPlayer);

#endif /* ZONE_NPC_H */

