#ifndef ZONE_RESPACT_H
#define ZONE_RESPACT_H

const RESPACTPARTYDEF* z_respact_party_def(ZONESVRENV *pstEnv, unsigned int nID);

const RESPACTPHASEDEF* z_respact_phase_def(ZONESVRENV *pstEnv,
                                           unsigned int nPhase);

// 根据声望值直接得到当前的声望阶段定义
const RESPACTPHASEDEF* z_respact_phase_refer_value(ZONESVRENV *pstEnv,
                                                   unsigned int nRespactValue);

// 查找角色的声望信息
RESPACTENTRY* player_find_respact(ZONESVRENV *pstEnv, Player *pstPlayer,
                                  unsigned int nID);

// 登录时候发送给客户端的声望信息快照
int player_respact_info(ZONESVRENV *pstEnv, Player *pstPlayer);

// 角色的声望变化的通知
int player_respact_chg(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID);

// 角色的声望增长
int player_respact_inc(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID,
                       unsigned int nValue, int fInCombat);

// 声望值检查
int player_respact_value_check(ZONESVRENV *pstEnv, Player *pstPlayer,
                               const RESPACTENTRY *pstEntry);

// 删除一条角色声望信息
int player_respact_del(ZONESVRENV *pstEnv, Player *pstPlayer, unsigned int nID);

#endif // ZONE_RESPACT_H
