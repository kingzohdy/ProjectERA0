#ifndef ZONE_SHOP_H
#define ZONE_SHOP_H

#include "zone_svr.h"

/* 二分法查找相关的定义 */
SHOPDEF * z_find_shopdef( ZONESVRENV* pstEnv, int iShopID );

/* hash算法查找相关的定义 */
SHOPDEF * z_hash_shopdef(ZONESVRENV *pstEnv,unsigned int uiKey);

SHOPLIMITDEF * z_hash_shoplimitdef(ZONESVRENV *pstEnv,unsigned int uiKey);

int player_shopping_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg);

int shopping_operate_point_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,  char *Pass9Account,
									int Uin,
									unsigned int  iTianYuan,void * pData,
									unsigned int uiLen, char DataType,char OperType);
int shopping_db_select_role_req(ZONESVRENV *pstEnv,int Uin, unsigned long long  GiftToRoleID,
								void *pData,unsigned int uiLen,int iCallCase);
int shopping_db_select_role_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstPkg);

int shopping_operate_point_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSDECTIANYUANRES *pstDecTianYuanRes);

int shopping_modify_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SHOPDEF *pstShopDef, int iCount);

int shopping_query_dianquan_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSQUERYTIANYUANRES *pstSSQueryTianYuanRes);

int shopping_ss_send(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,  
					int iCmd,char *szData,int iLen);

int shopping_modify_msg_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,int iBroadcaseFlag);

int shopping_db_tradebuy_count_req(ZONESVRENV *pstEnv);

int shopping_db_tradebuy_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);


int shopping_db_tradebuy_delete_req( ZONESVRENV* pstEnv,int iUin, TRADEBUY *pstTradeBuy,
					int operType,void *pData,unsigned int uiLen);

int shopping_db_tradebuy_delete_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg);

int shopping_db_tradebuy_insert_req( ZONESVRENV* pstEnv,int iUin, TRADEBUY *pstTradeBuy,void *pData,unsigned int uiLen);

int shopping_db_tradebuy_insert_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg);

int shopping_db_tradebuy_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord);

int shopping_db_tradebuy_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int shopping_db_tradesell_count_req(ZONESVRENV *pstEnv);

int shopping_db_tradesell_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int shopping_db_tradesell_delete_req( ZONESVRENV* pstEnv,int iUin, TRADESELL *pstTradeSell,
								int operType,void *pData,unsigned int uiLen);

int shopping_db_tradesell_delete_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg);

int shopping_db_tradesell_insert_req( ZONESVRENV* pstEnv,int iUin, TRADESELL *pstTradeSell);

int shopping_db_tradesell_insert_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg);

int shopping_db_tradesell_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord);

int shopping_db_tradesell_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int shopping_db_tradesell_update_req( ZONESVRENV* pstEnv,int iUin, TRADESELL *pstTradeSell,
	                                                                 CSSHOPPINGTRADEREQ *pstShoppingTradeReq,TRADEPLAYERINFO *pstTradePlayerInfo);
int shopping_db_tradesell_update_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg);	                                                                 

int z_insert_tradesell( ZONESVRENV* pstEnv, TRADESELL *pstTradeSell);

int z_insert_tradebuy( ZONESVRENV* pstEnv, TRADEBUY *pstTradeBuy);

int z_delete_tradesell( ZONESVRENV* pstEnv, unsigned long long  ullTradeID);

int z_delete_tradebuy( ZONESVRENV* pstEnv, unsigned long long  ullTradeID);

TRADEBUY * z_find_tradebuy( ZONESVRENV* pstEnv, unsigned long long ullID );

TRADESELL * z_find_tradesell( ZONESVRENV* pstEnv, unsigned long long ullID  );

int shopping_delete_user_tradecord(ZONESVRENV* pstEnv, Player *pstPlayer,char cType,unsigned  long long ullID);

int shopping_destroy_traderecord_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGDESTROYTRADERECORDREQ *pstShoppingDestroyTradeRecordReq);

int shopping_operate_point_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSDECTIANYUANRES *pstDecTianYuanRes);

int z_shopping_init(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int shop_chg_rolename(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, const char* pszNewName);


int z_shopping_trade_tick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,long long uiPreScanNum);

int shopping_query_traderecord_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,
	CSSHOPPINGQUERYRECORDBYIDREQ *pstShoppingQueryRecordByIDReq,int OperType);

int shopping_db_shopdef_count_req(ZONESVRENV *pstEnv);

int shopping_db_shopdef_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int shopping_db_shopdef_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord);

int shopping_db_shopdef_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int shopping_db_shopmsgdef_count_req(ZONESVRENV *pstEnv);

int shopping_db_shopmsgdef_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int shopping_db_shopmsgdef_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord);

int shopping_db_shopmsgdef_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int z_load_shopdef_data(ZONESVRENV *pstEnv);

int z_init_shopdef_hash_mem(ZONESVRENV *pstEnv);

int shopping_db_shopdef_update_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,TORMPKG *pstPkg);

int shopping_db_shoplimitdef_count_req(ZONESVRENV *pstEnv);

int shopping_fresh_shoplimitdef_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer);

int shopping_db_shoplimitdef_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord);

int shopping_db_shoplimitdef_update_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,TORMPKG *pstPkg);

int shopping_modify_limit_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,SHOPLIMITDEF *pstShopLimitDef, int iCount);

int shopping_db_shoplimitdef_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int shopping_db_shoplimitdef_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg);

int z_shopdef_tick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,unsigned int uiPreScanNum);
int shop_buy_tip(ZONESVRENV* pstEnv);

int shopping_query_my_traderecord_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGQUERYMYTRADERECORDREQ *pstShoppingQueryMyTradeRecordReq);

int shopping_tradeplayerinfo_init(Player *pstPlayer,TRADEPLAYERINFO *pstTradePlayerInfo);

int shopping_fresh_shopdef_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGFRESHRECORDREQ *pstShoppingFreshRecordReq);

int shopping_delete_user_all_traderecord(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID);

int shopping_ss_query_propsdef_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv);

int shopping_ss_query_propsdef_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSBILLQUERYPROPSDEFRES *pstBillQueryPropsDefRes);

int shopping_query_propsdef_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer,CSSHOPPINGQUERYPROPSDEFREQ *pstShoppingQueryPropsDefReq);

int shopping_query_propsgift_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer,CSSHOPPINGQUERYPROPSGIFTREQ *pstShoppingQueryPropsGiftReq );

int shopping_query_propsgift_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSBILLQUERYPROPSGIFTRES *pstSSBillQueryPropsGiftRes);

int shopping_set_propsgift_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer,CSSHOPPINGSETPROPSGIFTREQ* pstShoppingSetPropsGiftReq );

int shopping_set_propsgift_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSBILLSETPROPSGIFTRES *pstSSBillSetPropsGiftRes);

int shopping_add_bulletin(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,int iActiveTime,int iRollInterval, char *szText);

int shopping_limit_num_add_bulletin(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SHOPLIMITDEF *pstShopLimitDef,int iSellNum);

int shopping_exchange_point_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGEXCHANGEREQ *pstShoppingExchangeReq);

int shopping_exchange_point_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSDECTIANYUANRES *pstDecTianYuanRes);

int shopping_del_traderecord_by_del_role(ZONESVRENV* pstEnv, tdr_ulonglong RoleID );

int shopping_check_user_all_traderecord(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID);

//opertype 参考 GODCOIN_OPER_FLAG
int godcoin_consume_log(ZONESVRENV* pstEnv,int Uin,unsigned long long RoleID, int RoleLevel,
								int OperType, int OperSubType, int ShopID,int ShopNum,
								unsigned long long GodCoin,const char *pszStr, ...);

int shopping_battle_buy(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEBUYREQ *pstBattleBuyReq);

int shopping_battle_buy_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEBUYRES *pstBattleBuyRes);

void shopping_db_init_torm_head(	TAPPCTX *pstAppCtx, 
	                      					ZONESVRENV *pstEnv,
	                      					int Uin,
                              				  	TORMPKGHEAD * pstHead,
                              				  	int iCmd, 
                              				  	int iCallBackCmd,
	                       					const char *pszMetaName, 
	                      					const char * pstCallBackData, 
	                      					int iContextLen );

int shopping_db_send_req(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
	                     TORMPKG *pstTormPkg, const void *pvData, int iDataLen,int *piDst,int DBProtoVer);
int hash_cmp_int(const void * pv1 , const void *pv2);
unsigned int hash_code_int(const void* pv);

int gm_trade_do(ZONESVRENV* pstEnv, int iSysID, char *pszOut, int iOut);
int gm_trade_cancel(ZONESVRENV* pstEnv, int iType, tdr_ulonglong TradeID, char *pszOut, int iOut);
int gm_trade_list(ZONESVRENV* pstEnv, TRADERECORD *pstRecord, int *piCount, char *pszOut, int iOut);

#define MIN_GOLD_RATE 10
#define MAX_GOLD_RATE 10000
int gold_rate_data(ZONESVRENV* pstEnv, TRADEBUY *pstTradeBuy);
int gold_rate_get_money(ZONESVRENV* pstEnv, Player *pstPlayer);
int gold_rate_dec_money(ZONESVRENV* pstEnv, Player *pstPlayer);
int gold_rate_get_daynum(ZONESVRENV* pstEnv, Player *pstPlayer);
int shop_cgi_req(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq, SHOPCGIRES *pstShopCgiRes);
int shop_cgi_init(ZONESVRENV* pstEnv);

int buy_limit_date_check(ZONESVRENV* pstEnv,Player *pstPlayer,SHOPDEF *pstShopDef);
int buy_limit_date_use_check(ZONESVRENV* pstEnv,Player *pstPlayer,SHOPDEF *pstShopDef);
int buy_limit_date_record(ZONESVRENV* pstEnv,Player *pstPlayer,SHOPDEF *pstShopDef);
int buy_limit_date_num_check(ZONESVRENV* pstEnv,CSSHOPPINGBUYREQ *pstShoppingBuyReq,SHOPDEF *pstShopDef);
int buy_limit_date_send_check(ZONESVRENV* pstEnv,CSSHOPPINGBUYREQ *pstShoppingBuyReq,SHOPDEF *pstShopDef);



#endif /* ZONE_SHOP_H */
