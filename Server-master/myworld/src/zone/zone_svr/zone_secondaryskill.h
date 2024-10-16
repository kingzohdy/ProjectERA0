#ifndef ZONE_SECONDARYSKILL_H
#define ZONE_SECONDARYSKILL_H

#include "zone_svr.h" 

int z_get_skill_recipe(SECONDARYSKILLINFO *pstSkillInfo, int iRecipeID);

RECIPEDEF * z_find_recipe_def(ZONESVRENV *pstEnv, unsigned short iSkillType, unsigned short iRecipeID);

SECONDARYSKILLDEF  * z_find_secondary_skill_def(ZONESVRENV *pstEnv, unsigned short iSkillType, unsigned short iLevel);

SECONDARYSKILLINFO* get_player_secondary_skill_info(Player *pstPlayer, int iSkillType);

int player_secondary_skill_info(ZONESVRENV* pstEnv, Player *pstPlayer);

int player_learn_secondary_skill_check(ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillType, int iSkillLevel);
	
int player_learn_secondary_skill(ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillType, int iSkillLevel);

int player_secondary_skill_clt(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
			TFRAMEHEAD	 *pstFrameHead, CSPKG *pstCsPkg);

int player_recipe(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLRECIPE *pstMaking);

int z_set_skill_recipe(SECONDARYSKILLINFO *pstSkillInfo, int iRecipeID, int iVal);

int player_secondary_skill_del(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLDEL*pstDel);

int player_learn_recipe(ZONESVRENV* pstEnv, Player *pstPlayer, CSSECONDARYSKILLRECIPE *pstLearn);

int player_learn_secondary_skill(ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillType, int iSkillLevel);

int open_ui(ZONESVRENV* pstEnv, Player *pstPlayer, int iNpcID, int iUIType);

int z_add_secondary_skill_val(ZONESVRENV* pstEnv, Player *pstPlayer, SECONDARYSKILLINFO *pstSkillInfo, int iVal);

int z_recipe_brk(ZONESVRENV* pstEnv, Player *pstPlayer);

#endif /* ZONE_SECONDARYSKILL_H */
