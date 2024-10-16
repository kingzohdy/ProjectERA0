
#ifndef ZONE_CLAN_H
#define ZONE_CLAN_H

#include "zone_svr.h"

enum tagCLAN_BABY_STAT
{
    CLAN_BABY_STAT_NEW = 0, 		/* 神兽养成状态*/
    CLAN_BABY_STAT_CALL = 1, 	/* 神兽召唤出来的状态*/
    CLAN_BABY_STAT_DIE = 2, 		/* 神兽死亡 */
};

int z_active_clan(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, int iCount);
int clan_role_login(ZONESVRENV* pstEnv, Player *pstPlayer);
ZoneClan *player_get_clan(ZONESVRENV* pstEnv, Player *pstPlayer);
int clan_count_res(ZONESVRENV* pstEnv, TORMSELECTCOUNTRES *pstSelectCountRes);
int clan_select_res(ZONESVRENV* pstEnv, TORMSELECTRES *pstSelectRes);

// 发送所有公会的ID到客户端
int player_clan_id_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer);

// 发送公会广播数据
// fIsMemSnapshot是指广播的是不是公会成员快照
int player_clan_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstClan,
                          CSPKG *pstPkg, int fIsMemSnapshot);

// 向所有公会成员广播文本消息
int player_clan_msg_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                              int iSysType, int iErrno, ...);

// 退出公会
int player_clan_exit(ZONESVRENV *pstEnv, Player *pstPlayer);

// 根据会长的 Role ID 查找他所在的公会
ZoneClan* z_clan_leader_id( ZONESVRENV *pstEnv, tdr_ulonglong ullRoleID);



// 根据CLANID查找公会信息。
ZoneClan* z_clanid_clan(ZONESVRENV *pstEnv, const CLANID *pstClanID);


// 通知客户端打开公会相关面板
int clan_open_generic_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID,
                         int iUI);

// 关闭服务器时处理所有公会的数据库事务
void z_db_clan_fin(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv);

// 是否所有的公会数据都Clean了
// 所有公会数据皆Clean返回非0，否则返回0
int z_db_clan_is_allclean(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv);

// 公会数据库删除结果处理
int clan_delete_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, 
                    unsigned int nClanID);

// 公会数据库更新结果处理
int clan_update_res(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, TORMPKG *pstTormPkg, 
                    unsigned int nClanID);

// 角色登出公会相关操作
int clan_role_logout(ZONESVRENV *pstEnv, Player *pstPlayer);

// 处理客户端发送过来的关于公会的操作。
int player_clan(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer, 
                TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

// 根据Id查找公会信息。
ZoneClan* z_id_clan(ZONESVRENV *pstEnv, unsigned int nID);

ZoneClan* clan_find_by_wid(ZONESVRENV *pstEnv, tdr_ulonglong ullWID);

int player_clan_apply_join_clan(ZONESVRENV *pstEnv, Player *pstPlayer,
                                const CLANID *pstClanID);

int player_clan_member_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer);

int player_clan_info( ZONESVRENV* pstEnv, Player *pstPlayer );

// 查看公会外交信息快照
int player_clan_diplomatism_snapshot(ZONESVRENV *pstEnv, Player *pstPlayer);

// 获取某个玩家在所处公会中的职位。
unsigned int player_clan_position(ZONESVRENV* pstEnv, Player *pstPlayer);

CLANMEM * clan_id_mem(ZoneClan *pstClan, tdr_ulonglong ullRoleID, int *piIdx);

int ClanHashCmp(const void *pv1 , const void *pv2);
unsigned int ClanHashCode(const void *pvData);

int player_clan_broadcast_except(ZONESVRENV *pstEnv, ZoneClan *pstClan, 
	                             CSPKG *pstPkg, int fIsMemChg,
	                             Player *pstPlayerExcept);

// 请求查找公会
int player_clan_find_req(ZONESVRENV *pstEnv, Player *pstPlayer,
                         const CSCLANFINDREQ *pstReq);

// 玩家加入公会
int player_clan_join_clan(ZONESVRENV *pstEnv, Player *pstPlayer,
                          ZoneClan *pstClan);

// 设置公会成员的职位和封号
int player_clan_set_job_and_title(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  const CSCLANSETJOBANDTITLE *pstReq);

// 查找公会相关定义表
CLANLEVELDEF* z_clan_level_def(ZONESVRENV *pstEnv, int iLevel);
CLANCITYLEVELDEF* z_clan_city_level_def(ZONESVRENV *pstEnv, int iLevel);
CLANCITYBUILDINGLEVELDEF* z_clan_city_building_level_def(ZONESVRENV *pstEnv,
	                                                     int iLevel, int iID);

// 发工资 
int player_clan_pay(ZONESVRENV *pstEnv, Player *pstPlayer, int iPayWhat,int * piGet);


// 生成所有的公会城市副本
int clan_init_all_city_pworld(ZONESVRENV *pstEnv);

// 判断两个人是不是同一公会人员
static inline int player_check_same_clan(ZONESVRENV *pstEnv, Player *pstPlayer,
                                         Player *pstOther)
{
	ZoneClan *pstMyClan = player_get_clan(pstEnv, pstPlayer);
	ZoneClan *pstOtherClan = player_get_clan(pstEnv, pstOther);
	return (pstMyClan != pstOtherClan) ? -1 : 0;
}


int clan_broadcast_if(
	ZONESVRENV *pstEnv,
	const ZoneClan *pstZoneClan,
	CSPKG *pstPkg,
	Player* (*pfnPred)(ZONESVRENV *, const ROLEMEMBER *));

Player *ClanEveryBody(ZONESVRENV *pstEnv, const ROLEMEMBER *pstMember);

// 更新角色在自己公会中的状态
int player_clan_update_mem(ZONESVRENV *pstEnv, Player *pstPlayer);

// 把公会聊天信息转发到MUD
int clan_chat_transmit(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                       const CSCHATRES *pstChatRes);

int clan_develop_val(ZoneClan *pstClan);

// 从所有公会中找到某一角色, 并将其从公会成员列表中删除, 效率低,角色删除时调用
int clan_global_del_mem_slow(ZONESVRENV *pstEnv, tdr_ulonglong ullRoleID);

typedef enum tagCLAN_RESOURCE
{
	CLAN_MONEY,           // 公会资金
	CLAN_STABLE_VALUE,    // 公会安定度
	CLAN_OWNED_CONTRIBUT, // 公会个人贡献度
	CLAN_TECH_POINT,      // 公会科技点
}CLAN_RESOURCE;

int clan_resource_inc(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                      Player *pstPlayer, CLAN_RESOURCE iWhich,
                      unsigned int nValue);

// 修改公会的属性值(+/-)
int clan_property_modify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich,
                         int iValue, int fDec);

int clan_property_modify_test(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich,
                         int iValue, int fDec);

int clan_spend_notify(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneClan *pstClan);

// 根据值的大小修改公会属性值(自动判断+/-)
static inline int clan_property_adapt(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iWhich, int iValue)
{
	return iValue < 0 ? clan_property_modify(pstEnv, pstZoneClan, iWhich,
	                                         -iValue, 1)
	                  : clan_property_modify(pstEnv, pstZoneClan, iWhich,
	                                         iValue, 0);
}

// 直接通知客户端某个属性的值
static inline int clan_property_notify(ZONESVRENV *pstEnv,
                                       ZoneClan *pstZoneClan, int iWhich)
{
	return clan_property_modify(pstEnv, pstZoneClan, iWhich, 0, 0);
}

// 设置"脏"标志
static inline void clan_set_dirty(ZoneClan *pstZoneClan, int iDirty)
{
	if (pstZoneClan->stZoneClanInfo.iDirty < iDirty)
		pstZoneClan->stZoneClanInfo.iDirty = iDirty;
}

static inline void clan_set_dirty_notify(ZoneClan *pstZoneClan, int iDirty)
{
	clan_set_dirty(pstZoneClan, iDirty);
	pstZoneClan->stZoneClanInfo.fWillViewNotify = 1;
}

static inline void clan_set_clean(ZoneClan *pstZoneClan)
{
	if (pstZoneClan->stZoneClanInfo.iDirty != CACHE_FLAG_INEXIST)
		pstZoneClan->stZoneClanInfo.iDirty = CACHE_FLAG_CLEAN;
}

// 比较两个公会成员的官衔大小
int clan_pos_cmp(int iRhs, int iLhs);

static inline int player_clan_pos_cmp(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      int iLhs)
{
	return clan_pos_cmp(player_clan_position(pstEnv, pstPlayer), iLhs);
}

// 判断两个公会是不是敌对公会
static inline int clan_is_enemy(const ZoneClan *pstSelf,
                                const ZoneClan *pstZoneClan)
{
	tdr_ulonglong ullSelf = pstSelf->stClan.ClanProf.GID;
	int i = pstZoneClan->stClan.ClanInner.ClanEnemyNum;
	while (i--)
	{
		if (ullSelf == pstZoneClan->stClan.ClanInner.ClanEnemys[i].ClanId.GID)
		{
			return 1;
		}
	}
	return 0;
}

// 判断两个公会是不是同盟公会
static inline int clan_is_friend(const ZoneClan *pstSelf,
                                 const ZoneClan *pstZoneClan)
{
	tdr_ulonglong ullSelf = pstSelf->stClan.ClanProf.GID;
	int i = pstZoneClan->stClan.ClanInner.ClanFriendNum;
	while (i--)
	{
		if (ullSelf == pstZoneClan->stClan.ClanInner.ClanFriends[i].ClanId.GID)
		{
			return 1;
		}
	}
	return 0;
}

// 获得两个公会的关系
// 返回值:
//   <0: 敌对关系
//  ==0: 无关系
//   >0: 盟友关系
static inline int clan_relation(const ZoneClan *pstSelf,
                                const ZoneClan *pstZoneClan)
{
	if (clan_is_enemy(pstSelf, pstZoneClan))
	{
		return -1;
	}
	if (clan_is_friend(pstSelf, pstZoneClan))
	{
		return 1;
	}
	return 0;
}

static inline int clan_relation_by_player(ZONESVRENV *pstEnv, Player *pstPlayer,
                                          const ZoneClan *pstZoneClan)
{
	ZoneClan *pstSelf = player_get_clan(pstEnv, pstPlayer);
	return !pstSelf ? 0 : clan_relation(pstSelf, pstZoneClan);
}

// 获取公会ID
static inline const CLANID* clan_id_warp(const CLAN *pstClan)
{
	static CLANID stID;
	bzero(&stID, sizeof(stID));
	if (!pstClan)
	{
		return &stID;
	}
	stID.GID = pstClan->ClanProf.GID;
	stID.Id  = pstClan->ClanProf.Id;
	STRNCPY(stID.Name, pstClan->ClanProf.Name, CCH(stID.Name));
	return &stID;
}


//
// CS CGI使用的接口
//
int ss_clan_all_info(ZONESVRENV *pstEnv,
                     const ZONECTLGETCLANALLINFOREQ *pstReq);

int ss_clan_del_member(ZONESVRENV *pstEnv,
                       const ZONECTLDELCLANMEMBERREQ *pstReq);

int ss_clan_disband(ZONESVRENV *pstEnv, const ZONECTLDISBANDCLANREQ *pstReq);

int ss_clan_clear_diplomatism(ZONESVRENV *pstEnv,
                              const ZONECTLCLEARCLANDIPLOMATISMREQ *pstReq);

int ss_clan_del_diplomatism(ZONESVRENV *pstEnv,
                            const ZONECTLDELCLANDIPLOMATISMREQ *pstReq);

int ss_clan_exchange_leader(ZONESVRENV *pstEnv,
                            const ZONECTLEXCHANGECLANLEADERREQ *pstReq);

int clan_online_member_to_mud(ZONESVRENV *pstEnv,char *pszName,CLANID *pstID);


int ss_set_clan_base_attr(ZONESVRENV *pstEnv,
                              const ZONECTLSETCLANBASEATTRREQ *pstReq);

// 广播公会成员增量信息 
int player_clan_mem_chg_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstClan,
                                         const CLANMEM *pstMem, int iChgCode);

//设置宣战cd时间
int clan_set_enemy_cd(ZONESVRENV * pstEnv, ZoneClan * pstClan, ZoneClan * pstOtherClan,int iCD);
// 清除公会外交关系
int clan_clear_diplomatism(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);

int player_clan_create(ZONESVRENV *pstEnv, Player *pstPlayer,
                       const char *psClanName,int iCheck);

int player_clan_color_info(ZONESVRENV *pstEnv, Player *pstPlayer);

int player_clan_map_pos_info(ZONESVRENV *pstEnv, Player *pstPlayer);

const char* time_text_zh(int iSec, char *psRev);

int player_clan_yesterday_clan_touch_notify_broadcast(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);

int player_clan_yesterday_clan_touch_notify(ZONESVRENV *pstEnv, Player *pstPlayer);

int clan_touch_check(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,int iNotify);


int clan_touch_check_all(ZONESVRENV *pstEnv);
//公会刺探活动UI
int player_clan_active_ui_info_clan_spy(ZONESVRENV * pstEnv,Player* pstPlayer,int iType);
int clan_spy_open_ui_all(ZONESVRENV *pstEnv);

//公会枢纽副本活动UI
int player_clan_active_ui_info_clan_machine_pworld(ZONESVRENV * pstEnv,Player* pstPlayer,int iType);

int clan_init_touch_status(ZONESVRENV* pstEnv);

//
// 配置信息
//
extern int g_iClanDestroyMemMinSecs;
extern int g_iClanDestroyActiveMinSecs;
extern int g_iClanExitCDSecs;
extern int g_iClanApplyMaxSecs;
extern int g_iClanOnlineActiveSecs;
extern int g_iClanMaxApplyNum;
extern int g_aiLookupClanLevelMemNum[];
extern int g_iClanSelectTaskGroupCDTime;
extern int g_iClanAutoRecoverDefaultTaskGroupTime;
extern int g_iClanPayCDTime;
extern int g_iClanCityFirstDowngradeWarningTime;
extern int g_iClanCityLastDowngradeWarningTime;

extern const char *kClanPosName[] ;

#define MAX_CLAN_OWNED_CONTRIBUT     990000
#define CLAN_BATTLE_HB_TIMEOUT 60 //公会战场同步超时时间

int player_clan_view_chg(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                int iWhat);

int clan_action_clan_donate_log(
	ZONESVRENV *pstEnv,
	const ZoneClan *pstZoneClan,
	CLANDONATELOGENTRY *pstLog);
int clan_action_clan_skill_info(ZONESVRENV * pstEnv,  ZoneClan* pstZoneClan);
int player_clan_skill_learn_in(ZONESVRENV *pstEnv, Player *pstPlayer, 
							CLANSKILLDEF* pstDef,int iPosIdx);
int clan_skill_addattr_self(ZONESVRENV *pstEnv, Player *pstPlayer);
int clan_skill_valid_level_chg(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan,CLANSKILL *pstClanSkill);

//奖励整点更新时间
int clan_map_resource_time_update(time_t t1, int *piTime);

int clan_disband(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,char* pszRoleName);

// 从名字获取公会成员信息
CLANMEM* clan_name_mem(ZoneClan *pstZoneClan, const char *pszRoleName,
                              int *piIdx);

int player_clan_summon_ask_check(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);

int player_clan_summon(ZONESVRENV* pstEnv, Player *pstPlayer, RESULT *pstResult);

int player_clan_summon_answer(ZONESVRENV* pstEnv, Player *pstPlayer, CSCLANSUMMONANSWER *pstSummonAnswer);

int clan_leader_check(ZONESVRENV* pstEnv, ZoneClan *pstZoneClan);

//神兽宝宝现身
int clan_baby_call(ZONESVRENV* pstEnv, Player *pstPlayer);

//神兽宝宝吃东西
int clan_baby_feed_chg(ZONESVRENV* pstEnv, ZoneClan *pstClan, int iChg);

//神兽宝宝现在的状态
int clan_baby_stat_get(ZONESVRENV* pstEnv, ZoneClan *pstClan);

//公会是否有神兽宝宝
int clan_has_baby(ZONESVRENV* pstEnv, ZoneClan *pstClan);

int player_clan_baby_award(ZONESVRENV* pstEnv, Player *pstPlayer);

int clan_baby_die_award(ZONESVRENV * pstEnv, ZoneClan * pstClan, Monster * pstMon);

int clan_baby_tick(ZONESVRENV* pstEnv, ZoneClan *pstClan);

int clan_baby_clear_all(ZoneClan *pstClan);

//神兽宝宝营养值
int clan_baby_feed_get(ZONESVRENV* pstEnv, ZoneClan *pstClan);

//检查角色在据点地图是否合法
int player_in_strong_check(ZONESVRENV *pstEnv, Player *pstPlayer);

int clan_level_down_group_clear(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);

int player_clan_group_info(ZONESVRENV *pstEnv, Player *pstPlayer);

int clan_officers_check(ZONESVRENV *pstEnv, ZoneClan * pstClan);

int clan_group_valid_check(ZoneClan *pstZoneClan, int iGroupID);

int is_in_strong_group_player(ZoneClan *pstZoneClan, Player *pstPlayer, int iMapID);

int player_clan_group_broadcast_except(ZONESVRENV *pstEnv, ZoneClan *pstClan,
                                 CSPKG *pstPkg, Player *pstPlayerExcept);

int clan_award_by_def(ZONESVRENV *pstEnv,ZoneClan * pstClan,int iAwardDefID,
						char * pszTip,int iTipType,int iErrTip,int iTest);
//int clan_kick_contribute_mail(ZONESVRENV *pstEnv, char *pszClanName, char *pszRoleName, int iContri);

int clan_dart_check_end(ZONESVRENV *pstEnv, ZoneClan *pstClan);
int clan_open_dart(ZONESVRENV *pstEnv, Player *pstPlayer);
int clan_is_open_dart(ZONESVRENV *pstEnv, ZoneClan *pstClan);
int clan_is_open_dart_by_role(ZONESVRENV *pstEnv, Player *pstPlayer);
int clan_dart_login(ZONESVRENV *pstEnv, Player *pstPlayer);
void clan_dart_die(ZONESVRENV *pstEnv, Player *pstPlayer, ZoneAni * pstAtker, Monster* pstMon);
int clan_battle_apply_info_action(ZONESVRENV * pstEnv,  ZoneClan* pstClan,int iOpType);
int clan_battle_award_co(ZONESVRENV *pstEnv, Player *pstPlayer,COROUTINEDATA *pstData,int iDstID);
int clan_battle_award_in(ZONESVRENV *pstEnv,CLANID* pstID,int iClanWarAwardDefID,int iWin,int iMul);

int clan_skill_reset_return_money(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan);
int clan_player_skill_reset_return_contri(ZONESVRENV *pstEnv,Player *pstPlayer);
int clan_skill_business_exp_chg(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan,CSCLANSKILLRESEARCH *pstCSClanSkillResearch);
int clan_skill_mem_limit_chg(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan,CSCLANSKILLRESEARCH *pstCSClanSkillResearch);
int clan_exit_contribute_mail(ZONESVRENV *pstEnv,Player *pstPlayer,ZoneClan *pstZoneClan);
int clan_exit_return_contribute(ZONESVRENV *pstEnv, Player *pstPlayer);
int clan_mem_num_top_jian_yan(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan);
int clan_skill_business_exp_jian_yan(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan);



int player_clan_group_broadcast(ZONESVRENV *pstEnv,ZoneClan *pstZoneClan);

int player_clan_check(ZONESVRENV *pstEnv, Player *pstPlayer,CSCLANCLT *pstClanClt);




#endif /* ZONE_CLAN_H */
