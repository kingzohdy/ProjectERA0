
#ifndef ZONE_OPLOG_H
#define ZONE_OPLOG_H

#include "zone_svr.h"
#include "op_log_meta.h"

ROLEMEMBER * z_rolemem_warp( const Player * pstPlayer, ROLEMEMBER * pstRoleMember );

int z_oplog_init(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int z_oplog_fini(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int z_oplog_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, OP_LOG_META *pstOpLog);

int z_role_loging_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int z_rolenew_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,
						tdr_ulonglong ullRoleID, int iCareer, int iGender,char *pszName);
int z_rolelogout_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int z_roleshop_oplog(ZONESVRENV* pstEnv, Player *pstPlayer ,ITEMDEF *pstItemDef , OPSHOPDATA*OldShopData 
	, int iNum , tdr_ulonglong llWid, int ShopAct, ITEMDEC *pstItemDec, int iClanConDec);

int z_rolemail_oplog( ZONESVRENV* pstEnv, Player *pstPlayer, int iOperation, const MAIL * pstMail );

int z_role_exchg_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstExchgObj,EXCHGENTIY *pstIn, EXCHGENTIY *pstOut);

int z_role_stall_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,STALLSEERES *pstStallSee,int iFlag);

int z_role_stall_buy_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,Player *pstStallOwner,
							ROLEGRID*pstRoleGrid,ROLEPET *pstRolePet,int iType,unsigned int iPrice);

int z_role_machining_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEARM *pstOldRoelArm, 
								ROLEARM *pstRoleArm, MACHINNGEXPENDDATA *pstExpendData, 
								int iActType, int iSuccFlag, ITEMDEC *pstObtainItem, int iGetNum);

// 怪物死亡记录
int z_monster_die_oplog(ZONESVRENV* pstEnv, MONSTERDEF *pstMonDef, Monster* pstMon, ZoneAni *pstAtker );

// 记录玩家拾取的物品
int z_role_pickitem_oplog( ZONESVRENV* pstEnv, Player * pstPlayer, 
	const ROLEGRID * pstGrids, int iGridNum, int iHowToPick );

// 记录玩家的任务记录
int z_role_task_oplog( ZONESVRENV* pstEnv, Player * pstPlayer, 
	unsigned short iTaskID, int iState, ROLEGRID *pstRoleGrid, int iRoleGrid,
							int iMoney, int iMoneyType, int iExp, int iDecMoney);

// 记录角色的聊天信息
int z_role_chat_oplog( ZONESVRENV* pstEnv, Player * pstPlayer, 
	CSCHATREQ * pstChatReq );

// 记录角色的公会操作
int z_role_clan_oplog( ZONESVRENV* pstEnv, char * szRoleName, 
	const CLANID * pstClanIdent, int iOperation,OPCLANDATA* pstData);

// 记录宠物
int z_role_pet_oplog( ZONESVRENV* pstEnv, Player * pstPlayer, 
	const ROLEPET * pstPet, const ROLEPET * pstForwardPet, int iEvent, ITEMDEC *pstGetItem, 
	ITEMDEC *pstItemDec, USEMONEY *pstUse, PETSTRENINFO *pstStrenInfo);

int z_role_del_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, OPROLEDEL *pstOpRoleDel);

int z_pet_attr_chg_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, ROLEPET *pstRolePet, 
							unsigned char ucActType,  unsigned short unObjID, ITEMDEC *pstDec,
							USEMONEY *pstUseMoney);

int z_role_die_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, ZoneAni *pstAtker);

// 记录GM指令
int z_gm_cmd_oplog(ZONESVRENV *pstEnv, Player *pstPlayer, const char *pszCmdLine);

// 记录玩家从邮件中收取物品和金钱
int z_role_get_mailgoods_oplog(ZONESVRENV *pstEnv, Player *pstPlayer, 
	                             const OPROLEGETMAILGOODS *pstGoods);

//记录角色切换地图
int z_role_chg_map_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iPreMapID, RESPOS *pstPrePos);

//记录角色学习技能
int z_role_skill_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iLearnType, 
							int iSkillID, int iLevel, int iPreLevel, int iSkillExp, tdr_longlong iPreSkillExp, SKILLLEARN *pstLearn,
							OPROLESKILLRUNECHG* pstRuneBefore,int iUseItemID,OPROLESKILLRUNECOST* pstCost);



//记录角色学习素质
int z_role_diathesis_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iLearnType, int iDiathesisID, int iLevel, int iPreLevel);

/* 玩家契约日志 */
int z_role_qiyue_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iQiYueID, int Operation);

//int z_role_fairy_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEFAIRY *pstRoleFairy, int iOpType);
//int z_role_fairy_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEFAIRY *pstRoleFairy, int iOpType,int iPreStar);
int z_role_fairy_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,ROLEFAIRY *pstRoleFairy, int iOpType,
							int iPreStar,ITEMDEC *pstItemDec,int iMoneyDec);

// 角色称号日志
int z_role_designation_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,
                             int iDesignationID, int iEvent);

// 角色声望日志                             
int z_role_respact_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,
                         const RESPACTENTRY *pstRespact, int iEvent);

// 
int z_role_ride_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, int iRideID, int iLevel, int iOpType);

// 掉落日志   
int z_role_booty_drop_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,
			int iMap,RESPOS *pstPos,ROLEGRID *pstGrid,int iDropType, int iFromID);

int z_role_snapshot_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer);

int z_strong_fini_oplog(ZONESVRENV* pstEnv, Player *pstPlayer);
int z_role_godcoin_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iOperate,
								int iTianYuan, CSSHOPPINGBUYREQ *pstShoppingBuyReq, SHOPDEF *pstShopDef);

int z_role_trade_oplog(ZONESVRENV* pstEnv, OPTRADEINFO *pstOpTradeInfo);

// 记录角色升级快照
int z_role_lvup_snapshot_oplog(ZONESVRENV* pstEnv, Player *pstPlayer,
                               int iForwardLv, tdr_longlong iForwardExp,char CareerUpFlag);

int z_role_gift_log(ZONESVRENV* pstEnv, Player *pstPlayer, char *pszRoleName, int iGiftType, char *szGiftName, int iGiftID,tdr_ulonglong CashUp);

int z_role_use_xp_skill_log(ZONESVRENV* pstEnv, Player *pstPlayer, int iSkillID, int iSkillLv,int iUseXp);

int z_role_enter_pworld_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iPworldID);

int z_map_role_num_log(ZONESVRENV* pstEnv);
int z_pworld_time_oplog(ZONESVRENV* pstEnv, int iPworldID,int iTime);

int z_role_secondary_skill_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
	int iActType, SECONDARYSKILLINFO *pstInfo);

int z_role_secskill_recipe_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
	SECONDARYSKILLINFO *pstInfo, int iRecipeID);


int z_acc_login_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,int iUin, const char* pszAccName, int iFlag,Player* pstPlayer);

int z_role_shopping_exchange_oplog(ZONESVRENV* pstEnv, unsigned long long ullRolelID, char *RoleName,
								char *szAccount,int Uin,int iLevel, int PointNum,int GodCoin,int UserGodCoin,char Operate);

int z_role_item_dec_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, ITEMDEC *pstItemDec,int iOpType);

int z_oplog_proxy_send(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, OP_LOG_META *pstOpLog,int iFrom);
int z_role_item_drop_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,ROLEGRID* pstGrid);

int z_role_gift_log1(ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID,const char*pszRoleName,
					const char*pszAcc,int iUin,int iLvl,int iGiftType, char *szGiftName);

int z_role_die_drop_oplog(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,OPROLEDIEDROP *pstOpDieDrop);
int z_role_task_item_award(ZONESVRENV* pstEnv, Player * pstPlayer, unsigned short iTaskID, ROLEGRID *pstRoleGrid, int iRoleGrid);

int z_role_pworld_award_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iPworldID, int iExp, int iMoney);
int z_role_pworld_card_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, int iPworldID, int iExp, int iMoney, ROLEGRID* pstGrid);

int z_role_battle_award_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, int iBattleID, OPROLEBATTLEAWARDINFO *pstAwardInfo);

int z_role_recipe_make_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, 
	int iSkillType, int iSkillLevel, int iRecipeID, OPROLERECIPEMAKEINFO *pstMakeInfo);

int z_role_pet_catch_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, 
	int iUseItemID, int iTargetID, ITEMDEC *pstGetItem);

void z_oplog_event_begin(ZONESVRENV* pstEnv);

tdr_ulonglong z_get_logwid(ZONESVRENV* pstEnv);

void z_set_logwid(ZONESVRENV* pstEnv, tdr_ulonglong ullWID);

int z_map_trans_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,OPMAPTRANSINFO* pstData);
int z_role_fengyin_arm_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,Player* pstFengyin,
							int iType,int iUse,ROLEARM *pstArm);
int z_lucky_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iOpType,long long llValue);

int z_world_city_siege_oplog(ZONESVRENV* pstEnv, OPWORLDCITYINFO *pstPreWorldCityInfo);

int z_role_media_gift_log(ZONESVRENV* pstEnv, Player *pstPlayer,  char *szGiftName, int iUseItemID,GIFTITEM * pstSelOneGiftItem,GIFTITEM * pstSelAllGiftItem,int iSelAllGiftItemsLen);

int z_role_ltr_gift_log(ZONESVRENV* pstEnv, Player *pstPlayer,  char *szGiftName, int iUseItemID,int iUseNum,GIFTITEM * pstGiftItem);

int z_clan_active_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iActiveID,int iStatus);

int z_daily_goal_award_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iAwardID);

int z_bindgold_consume_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int OpType, int OpSubType, unsigned int BindGoldNum, char *pszDesc);
int z_bindgold_create_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int OpType, int OpSubType, unsigned int BindGoldNum, char *pszDesc);
int z_money_consume_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int OpType, int OpSubType, unsigned int BindMoneyNum, unsigned int MoneyNum, char *pszDesc);
int z_money_create_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int OpType, int OpSubType, unsigned int BindMoneyNum, unsigned int MoneyNum, char *pszDesc);

int z_impitem_consume_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int OpType, unsigned int iNum,  int iBindNum);
int z_impitem_create_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iItemID, int OpType, unsigned int iNum, int iBindNum);

int z_clanprop_consume_oplog(ZONESVRENV* pstEnv, ZoneClan *pstClan, int iPropID, int OpType, int OpSubType, unsigned int iNum);
int z_clanprop_create_oplog(ZONESVRENV* pstEnv, ZoneClan *pstClan, int iPropID, int OpType, int OpSubType, unsigned int iNum);

int z_contri_consume_oplog(ZONESVRENV* pstEnv, ZoneClan *pstClan, int OpType, int OpSubType, unsigned int iNum);
int z_contri_create_oplog(ZONESVRENV* pstEnv, ZoneClan *pstClan, int OpType, int OpSubType, unsigned int iNum);
int z_strong_oplog(ZONESVRENV* pstEnv, unsigned int uiClanID, int iStrongMapID);
int z_genius_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, int iOp, int iMoney, int iBindMoney, int iExp, int iSub, int iType);

int z_spar_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, int iOp, ROLEITEM *pstOldSparItem, 
								ROLEITEM *pstNewSparItem, MACHINNGEXPENDDATA *pstExpendData);
int z_clan_boon_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,int iMoneyType, int iMoney);
int z_script_add_exp_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,unsigned long long iExp, const char *pszScript);
int z_clan_boon_cond_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, CLAN*pstClan);
int z_store_buy_oplog(ZONESVRENV* pstEnv,Player* pstPlayer, ZoneStore *pstStore, 
									ROLEGRID *pstGrid, int iMoney, int iNum, int iType, int iPreMoney);
int z_battle_money_oplog(ZONESVRENV* pstEnv, Player *pstPlayer, int iType, unsigned int MoneyNum, char *pszDesc);

int z_city_fini_oplog(ZONESVRENV* pstEnv, WORLDCITYCLANFIGHTINFO *pstList, int iIdx);
int z_role_open_box(ZONESVRENV* pstEnv,Player* pstPlayer,int iType, int iDec, int iAdd, 
						GIFTITEM *pstDec, GIFTITEM *pstAdd);

int z_spanstrong_oplog(ZONESVRENV* pstEnv, Player* pstPlayer, int iStrongPoint, int iEnd);

int z_home_oplog(ZONESVRENV* pstEnv,Player* pstPlayer,Player* pstPlayerB,unsigned int HomeID,
					  char Lvl,int UseMoney,int UseItem,unsigned int Time,char *Desc);


#endif /* ZONE_OPLOG_H */
