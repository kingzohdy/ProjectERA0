#ifndef ZONE_CLAN_CITY_H
#define ZONE_CLAN_CITY_H

#include "zone_svr.h"

enum tagEveryDayPworldStat
{
	 EVERYDAY_PWORLD_UNOPEN = 0,
	 EVERYDAY_PWORLD_OPEN,	
	 EVERYDAY_PWORLD_CLOSE,  
};


#define CLAN_EVERYDAY_PWORLD_PLAYER_AWARD 500	//公会驱逐活动角色奖励银币基础值

//创建一个公会城市
int clan_city_create(ZONESVRENV* pstEnv, Player *pstPlayer);

//公会会员进入城市前条件检查
int clan_mem_enter_city_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstPos);

//进入公会城市
int clan_mem_enter_city(ZONESVRENV* pstEnv, Player *pstPlayer, RESPOS *pstPos);

// 在公会城市内瞬移
int player_clan_goto_city(ZONESVRENV *pstEnv, Player *pstPlayer, RESPOS *pstPos);

// 创建公会城市NPC
int clan_city_npc_create(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

//删除公会城市NPC
int clan_city_npc_del(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

//创建一个公会城市NPC
int  clan_city_npc_create_one(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, CLANNPC*pstClanNpc);

//删除一个公会城市NPC
int clan_city_npc_del_one(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, int iDefId, RESPOS *pstPos);

//绑定一个NPC 售卖列表
int clan_city_npc_bind_selllist(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan , int iDefID, int iSellLIst);

//清除一个公会城市
int clan_destroy_city(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

// 建立城市副本
int z_new_clan_city(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

// 获取公会城市副本实例
ZONEPWORLDINST *z_find_clan_city_inst(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);

//活动UI信息
int player_clan_everyday_pworld_action(ZONESVRENV* pstEnv, Player *pstPlayer);

//个人积分变化
int player_clan_everyday_pworld_val_chg(ZONESVRENV* pstEnv, Player *pstPlayer, int iChg);

//关闭公会活动UI
int clan_everyday_pworld_close_ui(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

//公会积分活动副本状态
int clan_everyday_pworld_stat_get(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

//创建公会积分活动副本
int clan_everyday_pworld_create(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

//进入公会积分活动副本
int clan_everyday_pworld_enter(ZONESVRENV* pstEnv, Player *pstPlayer);

//公会积分变化
int clan_everyday_pworld_val(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, CLANLEVELDEF *pstClanLevel, int iVal);

//销毁公会积分活动副本
int clan_destroy_everyday_pworld(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

int player_exit_clan_clear_everydaypworld(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, Player *pstPlayer);


int active_game_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

//角色是否在所属公会城市
int is_in_my_clan_city(ZONESVRENV* pstEnv, Player *pstPlayer);


int player_enter_clan_city_clt(ZONESVRENV* pstEnv, Player *pstPlayer, int iClanID,int iSpyTransID);

//公会建筑攻击报警
int clan_city_build_warning_byhit(ZONESVRENV *pstEnv, Monster *pstMon);

//公会建筑死亡提示
int clan_city_build_warning_die(ZONESVRENV *pstEnv, Monster *pstMon,Player * pstKiller);
//公会枢纽副本活动UIUI
int clan_active_ui_info_clan_machine_pworld(ZONESVRENV * pstEnv,ZoneClan* pstClan,int iType, ZONEPWORLDINST*p, int iWaveInfo);

int clan_dny_active_helper_info(ZONESVRENV * pstEnv,ZoneClan* pstClan,int iID,int iStatus);

//公会刺探活动UI
int clan_active_ui_info_clan_spy(ZONESVRENV * pstEnv,ZoneClan* pstClan,int iType);
int clan_machine_pworld_create(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan, int iShare);
ZONEPWORLDINST *z_find_clan_machine_pworld_inst(ZONESVRENV * pstEnv, ZoneClan *pstZoneClan);
int clan_machine_pworld_enter(ZONESVRENV* pstEnv, Player *pstPlayer, int iID);
int clan_pworld_wave_countdown_action(ZONESVRENV* pstEnv,ZoneClan* pstZoneClan,ZONEPWORLDINST *pstPworldInst);
int player_everyday_pworld_ui_info(ZONESVRENV* pstEnv, Player *pstPlayer, CSPKG *pstPkg);
int clan_broadcast_do_pkg(ZONESVRENV* pstEnv, ZoneClan *pstClan, int (*pfnPred)(ZONESVRENV *, Player *, CSPKG *));
int item_back_to_clan_city_check(ZONESVRENV *pstEnv, Player *pstPlayer);

#endif /* ZONE_CLAN_CITY_H */
