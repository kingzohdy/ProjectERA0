#ifndef ZONE_RUNE_H
#define ZONE_RUNE_H

#include "zone_combat.h"

int player_rune(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv, Player *pstPlayer,
                TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int player_rune_enable(ZONESVRENV *pstEnv, Player *pstPlayer,
                       const CSRUNEENABLECLT *pstReq);

int player_rune_set(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSRUNESETCLT *pstReq);

int player_rune_del(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSRUNEDELCLT *pstReq);


int player_rune_level_up(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSACTIONRUNELEVELUP *pstReq);

int player_rune_reset_ex(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSACTIONRESETRUNEEX *pstReq);

int player_rune_num_check(ZONESVRENV *pstEnv, Player *pstPlayer);


//���ĸ��Ӷ��⼼��Ч��
int rune_addattr_skill(ZONESVRENV *pstEnv, AtkProcess *pstAtkProcess, AtkeeInfo *pstAtkeeInfo);

//���ĸ��Ӷ���������������
int rune_addattr_self(ZONESVRENV *pstEnv, Player *pstPlayer);

//�����ȡһ����ɫδӵ�еķ��Ķ���Ч��
int player_rune_addattr_rand_get(ZONESVRENV *pstEnv, Player *pstPlayer);

//���ý�ɫ�������Ч��
int player_rune_addattr_set(ZONESVRENV *pstEnv, Player *pstPlayer, int iRuneID, int iRuneType);

int player_rune_ex_enable_info(ZONESVRENV *pstEnv, Player *pstPlayer);

int player_skill_rune_enable(ZONESVRENV * pstEnv, Player * pstPlayer);

int player_rune_skill_chg(ZONESVRENV *pstEnv, Player *pstPlayer,
                    const CSACTIONRUNNESKILLCHG *pstReq);

RUNEADDATTRDEF* z_find_run_addattr_def(ZONESVRENV *pstEnv, int iID);

int rune_op_info(ZoneSkill *pstZoneSkill,OPROLESKILLRUNECHG* pstRuneChg,int iRuneType,Player* pstPlayer);

int rune_only_inlay_which_skill(ZONESVRENV *pstEnv, const ITEMDEF *pstDef);

#endif // ZONE_RUNE_H