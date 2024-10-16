#ifndef ZONE_CLAN_ADVANCE_H
#define ZONE_CLAN_ADVANCE_H

#include "zone_machine.h"

extern const ClanMachinePropertyEntry *kEmptyMachineProperty;
extern const CLANGENERALRESULT *kEmptyGeneralResult;

// 获取公会科技定义信息
const CLANTECHDEF* clan_tech_def(ZONESVRENV *pstEnv, int iCategory, int iLevel);

// 获取公会产品定义信息,目前有物品,机械两大类
CLANPRODUCTDEF* clan_product_def(ZONESVRENV *pstEnv, int iID);

// 公会高级信息的快照
int player_clan_advance(ZONESVRENV *pstEnv, Player *pstPlayer);

// 研发科技
int player_clan_research(ZONESVRENV *pstEnv, Player *pstPlayer,
                         const CSCLANRESEARCH *pstReq);

// 取消研发科技
int player_clan_cancel_research(ZONESVRENV *pstEnv, Player *pstPlayer,
                                const CSCLANCANCELRESEARCH *pstReq);

// 制造物品/机械
int player_clan_make(ZONESVRENV *pstEnv, Player *pstPlayer,
                     const CSCLANMAKE *pstReq);

// 取消制造物品/机械
int player_clan_cancel_making(ZONESVRENV *pstEnv, Player *pstPlayer,
                              const CSCLANCANCELMAKING *pstReq);

// 从仓库中提取机械
int player_clan_grab_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                             const CSCLANGRABMACHINE *pstReq);

// 召回机械放回仓库中
int player_clan_callback_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const CSCLANCALLBACKMACHINE *pstReq);

// 对机械进行补给
int player_clan_machine_supply(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const CSCLANMACHINESUPPLY *pstReq);

// 公会仓库格子锁切换
int player_clan_storage_grid_lock_switch(ZONESVRENV *pstEnv, Player *pstPlayer,
                                     const CSCLANSTORAGEGRIDLOCKSWITCH *pstReq);

// 公会仓库摧毁物品
int player_clan_storage_grid_destroy(ZONESVRENV *pstEnv, Player *pstPlayer,
		                             const CSCLANSTORAGEGRIDDESTROY *pstReq);

// 公会高级数据初始化
void clan_advance_init(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);


// 公会高级数据清理
// 这个函数在公会被删除的时候调用一次
void clan_advance_cleanup(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);


// 公会高级tick
int clan_advance_tick(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan);


// 扩充公会仓库栏位
int clan_extend_storage(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich,
		                int iGridNum);

// 添加东西到公会仓库中
int clan_storage_add_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             int iWhich, int iDefID, int iNum);

// 从外部添加东西到仓库中, 比如: 角色提交的道具...
int clan_storage_add_entries_external(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum);

// 清空公会仓库
int clan_storage_clear(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iWhich);


// 从公会仓库中删除东西
int clan_storage_del_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             int iWhich, int iDefID, int iNum);

// 检查是否能从公会仓库中删除指定的东西
int clan_storage_check_del_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                   int iWhich, int iID, int iNum);

// 当机械被收回公会仓库时调用此函数
int clan_machine_on_callback(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                             Monster *pstMon);

// 当机械被摧毁(战损...)时调用此函数
int clan_machine_on_destroy(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                            Monster *pstMon);

// 当机械刚刷新时调用此函数
int clan_machine_on_update(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                           Monster *pstMon);

// 获取公会机械数据的增量值
const ClanMachinePropertyEntry* 
clan_machine_cached_effect(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iID);

// 获取公会科技消耗的科技点数.
// iTechID的含义:
// > 0: 指定某一项科技所消耗的科技点数.
// = 0: 所有科技所消耗的科技点数.
// < 0: 某一类科技所消耗的科技点数.
int clan_used_tech_point(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                         int iTechID);

// 角色主动交换自己骑乘的机械
int player_clan_commit_machine(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 Npc *pstNpc);

// 角色开着矿车上交矿石
int player_clan_commit_ore(ZONESVRENV *pstEnv, Player *pstPlayer, Npc *pstNpc, ROLEGRID *pastRoleGrid, int iNum);


// 确保角色不在公会机械上
int player_clan_machine_ensure_down(ZONESVRENV *pstEnv, Player *pstPlayer);


// 从公会商店中买公会物品, 这个函数几乎不检查参数, 慎用之
int player_clan_shop_buy(ZONESVRENV *pstEnv, Player *pstPlayer, Npc *pstNpc,
                         const SELLGOODS *pstSellGoods, int iNum);


// 根据Id查找公会信息。
extern ZoneClan* z_id_clan(ZONESVRENV *pstEnv, unsigned int nID);


// 研发加速
int player_clan_research_speedup(ZONESVRENV *pstEnv, Player *pstPlayer,
                                 const CSCLANRESEARCHSPEEDUP *pstReq);

// 制造加速
int player_clan_making_speedup(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const CSCLANMAKINGSPEEDUP *pstReq);

// 角色将自己背包杂物包中的物品提交到公会仓库.
int player_clan_commit_item(ZONESVRENV *pstEnv, Player *pstPlayer, int iGridIdx);

// 从外部添加东西到资源中心中, 比如资源或产品...
int clan_resource_add_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum,int* piRealNum);

// 公会资源中心格子锁切换
int player_clan_resource_lock_switch(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      const CSCLANRESOURCELOCKSWITCH *pstReq);
// 公会资源中心物品兑换
int player_clan_resource_grab_item(ZONESVRENV *pstEnv, Player *pstPlayer,
                                      const CSCLANRESOURCEGRABITEM* pstReq);

// 获取资源信息
CLANRESOURCEINFO* clan_resource_get_info(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID);

// 减少资源中心的资源
int clan_resource_dec_entries(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      int iID, int iNum);

// 资源信息变化通知
int clan_resource_notify(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan,
                                      CLANRESOURCEINFO* pstNotify);

// 减少资源中心的资源
int clan_resource_dec_entries_in(ZONESVRENV *pstEnv, CLANRESOURCE *pstResource,
                                      int iID, int iNum,CLANRESOURCEINFO* pstNotify);

// 提交公会资源
int player_clan_commit_item_byid(ZONESVRENV *pstEnv, Player *pstPlayer, int iItemID,int* piNum);

// 怪物.提交公会资源
int clan_commit_item_byid(ZONESVRENV *pstEnv, ZoneClan *pstZoneClan, int iItemID,int iNum);

int player_open_res_info_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType);
int clan_getres_info_msg(ZONESVRENV* pstEnv, ZoneClan *pstClan, Player *pstPlayer, int iPage);
int player_clan_getres_info(ZONESVRENV *pstEnv, Player *pstPlayer, CSCLANVIEWGETRES *pstReq);

// 研发/制造进度通知
int clan_running_progress_notify(ZONESVRENV *pstEnv,
                                        ZoneClan *pstZoneClan,
                                        const CLANRUNNINGPROGRESS *pstProgress,
                                        int iNotify, int iCode);
#endif //  ZONE_CLAN_ADVANCE_H
