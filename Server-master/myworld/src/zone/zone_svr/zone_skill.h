
#ifndef ZONE_SKILL_H
#define ZONE_SKILL_H

#include "zone_svr.h"

SKILLDEF *z_find_skill_zoneskill(ZONESVRENV* pstEnv, ZoneSkill *pstZoneSkill);
SKILLDEF *z_find_skill_idx(ZONESVRENV* pstEnv, int iSkillID, int iSkillLevel, int *piIdx);

int z_skill_online(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
int z_skill_roledata(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);
ZoneSkill * z_get_player_zoneskill(Player *pstPlayer, unsigned short unSkillID);
int z_skill_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill);

int z_update_one_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, ZoneSkill *pstZoneSkill);

int z_add_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, unsigned short unSkillID, unsigned short unLevel, int iNotify);
int z_add_diathesis(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short iDiathesisID, unsigned short unLevel);
DIATHESISDATA *z_get_player_diathesis(Player *pstPlayer, unsigned short unDiathesisID);

FITTINGSDEF *z_find_fittings(ZONESVRENV* pstEnv, unsigned short bFittingsID, unsigned char bLevel, unsigned char bGroup,unsigned short *pucIdx);

DIATHESISDEF *z_find_diathesis(ZONESVRENV* pstEnv, unsigned short unDiathesisID, unsigned char bLevel);

int z_diathesis_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, DIATHESISDATA *pstDiathesisData);

//int z_check_fittings_grid(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneSkill *pstZoneSkill, unsigned char bRowIdx, FITTINGSMAPLISTDATA *pstFittingMap);

int z_del_skill(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, unsigned short unSkillID, int iNotify);

SKILLDEF* z_find_skill(ZONESVRENV* pstEnv, unsigned short unSkillID, unsigned short unSkillLevel, int *piIdx);

int skill_check_by_ui(ZONESVRENV* pstEnv, Player* pstPlayer, SKILLLEARN *pstSkillLearn);
int skill_check_by_book(ZONESVRENV* pstEnv, Player* pstPlayer, SKILLLEARN *pstSkillLearn);

int skill_learn(ZONESVRENV* pstEnv, Player* pstPlayer, int iSkillID, int iLevel);

int skill_levelup(ZONESVRENV* pstEnv, Player* pstPlayer, int iSkillID, int iLevel);

int diathesis_learn(ZONESVRENV* pstEnv, Player* pstPlayer, int iDiathesisID, int iLevel);

int diathesis_levelup(ZONESVRENV* pstEnv, Player* pstPlayer, int iDiathesisID, int iLevel);

int open_diathesis_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID);
int open_skill_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID);

int player_skill_learn(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int skill_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillID, int iLevel, int iType);
int skill_update(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player* pstPlayer,int iSkillID, int iLevel);

int player_fly(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,  Player *pstPlayer);

int open_skill_fitting_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iNpcID,short uiType);

//FITTINGLIST * z_get_skill_fitlist(SKILLDEF *pstSkillDef, unsigned short iFittingsId);

int get_add_skill_level(ZONESVRENV* pstEnv, unsigned short unSkillID, unsigned short unSkillLevel, short nLevel);
int z_check_addattr_skillid(ADDATTRDEF *pstAddAttrDef, unsigned short unSkillID);

int player_sit(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,  Player *pstPlayer);
int sit_action(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,  int iType);
int player_sit_break(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,  Player *pstPlayer);

int z_get_diathesis_result(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID, SKILLRESULT *pstResult);

//int z_skill_fittings_attr(ZONESVRENV* pstEnv, Player *pstPlayer, CSSKILLDATA *pstSkillData, SKILLDEF *pstSkillDef);

SKILLRESULT *z_diathesis_def_get_result(DIATHESISDEF *pstDiathesisDef, int iResultID);
int open_genius_ui(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,int iNpcID);

int genius_all_reset(ZONESVRENV* pstEnv, Player* pstPlayer);

int z_genius_chg(ZONESVRENV* pstEnv, Player *pstPlayer,DIATHESISINFO *pstDiathesisInfo);

int z_diathesis_all_chg(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

//int z_get_skill_fittings_result(ZONESVRENV* pstEnv,  CSFITTINGSROW *pstFittingsRow, unsigned short SkillResultID,SKILLRESULT *pstResult);

int z_get_diathesis_result_by_value1(ZONESVRENV* pstEnv, Player *pstPlayer, int iResultID,int iVal1, SKILLRESULT *pstResult);

SKILLRESULT *z_get_skill_result( SKILLDEF *pstSkillDef,int iResultID);

SKILLDEF *z_get_player_normalskilldef(ZONESVRENV * pstEnv,Player *pstPlayer);

int is_ctrl_skill_result(SKILLRESULT *pstResult);

int is_ctrl_skill(SKILLDEF *pstSkillDef);

#endif /* ZONE_SKILL_H */

