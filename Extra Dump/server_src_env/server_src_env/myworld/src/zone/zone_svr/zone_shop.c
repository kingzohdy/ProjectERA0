#include "zone_shop.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "role_db_meta.h"
#include "zone_clt.h"
#include "zone_attr.h"
#include "zone_package.h"
#include "zone_player.h"
#include "zone_oplog.h"
#include "zone_package.h"
#include "zone_move.h"
#include "zone_mail.h"
#include "zone_relation.h"
#include "zone_safelock.h"
#include "zone_span.h"
#include "zone_err.h"
#include "zone_clanboon.h"
#include "zone_clan.h"
#include "zone_misc.h"


extern int g_iZoneBus;
#define MAX_ONEPLAYER_TRADE_RECORD 5
char szWatch[MAX_SIGLE_USER_TRADERECORD_NUM-MAX_ONEPLAYER_TRADE_RECORD];

// 这直接用来寄存下读取来的文件
static CSSHOPPINGGETXMLFILERES g_stShoppingGetXmlFileRes;
// 记录当前的扫描数
static unsigned int   g_uiSellScanNum;
static unsigned int   g_uiBuyScanNum;
static unsigned int   g_uiShopLimitDefScanNum;
// 商品的一些临时数据,先存这里吧
unsigned int g_astShopDef[MAX_SHOP_DEF_LINES];
int g_iModifyNum ;
unsigned int g_astShopDefTmp[MAX_SHOP_DEF_LINES];
int g_iShopDefTmp ;
int g_SelectCountShopNum;
int g_LoadAllShopNum;
int g_SelectCountShopLimitNum;
unsigned int g_astShopLimitDef[MAX_SHOP_DEF_LINES];
int g_iModifyLimitNum ;	
int g_LoadAllShopLimitNum;
unsigned int g_astShopLimitDefTmp[MAX_SHOP_DEF_LINES];
int g_iShopLimitDefTmp ;

int  g_iTradeSellBeginRecord;
int  g_iTradeSellRecordNum;
int  g_iTradeBuyBeginRecord;
int  g_iTradeBuyRecordNum;
int g_iShopMsgSendFlag;
int g_iShopGetPropsDefTime;

//修改标志位
#define   SHOPLIMIT_FLAG_MODIFY   1
//限时开始提示
#define   SHOPLIMIT_FLAG_TIMEBEGIN_TIPS   2
//限时最后1小时提示
#define   SHOPLIMIT_FLAG_TIMEHALF_TIPS   4
//限时结束
#define   SHOPLIMIT_FLAG_TIMEEND_TIPS   8
//限量开始提示
#define   SHOPLIMIT_FLAG_NUMBEGIN_TIPS   	  16
//限量到1半提示
#define   SHOPLIMIT_FLAG_NUMHALF_TIPS   	  32
//限量结束提示
#define   SHOPLIMIT_FLAG_NUMBEND_TIPS   	  64

#define SHOP_TRADE_MAX 10000000

GMTRADE *z_find_gmtrade(ZONESVRENV* pstEnv, int iID)
{
	GMTRADE  stGmTrade;
	int iIndex = 0;
	int iEqu = 0;
	ZoneObj *pstObj = pstEnv->pstZoneObj;

	stGmTrade.ID= iID;
	iIndex = bsearch_int(&stGmTrade, pstObj->sGmTrade, pstObj->iGmTrade, GM_TRADE_DATA, &iEqu);
	if (iEqu)
	{
		return (GMTRADE *)pstObj->sGmTrade[iIndex];
	}
	
	return NULL;	
}

// 哈希表相关函数 以序列号做hash的索引
int hash_cmp_int(const void * pv1 , const void *pv2)
{
	const unsigned int * pszBucket1 = (unsigned int * )pv1;
	const unsigned int * pszBucket2 = (unsigned int * )pv2;

	return *pszBucket1 - *pszBucket2;
}
unsigned int hash_code_int(const void* pv)
{
	const int * pszBucket = (int * )pv;
	
	return (unsigned int)*pszBucket;
}

static  int hash_cmp_ull(const void * pv1 , const void *pv2)
{
	const unsigned long long * pszBucket1 = (unsigned long long * )pv1;
	const unsigned long long * pszBucket2 = (unsigned long long * )pv2;

	if( *pszBucket1 > *pszBucket2 )
	{
		return 1;
	}
	else if(*pszBucket1 < *pszBucket2)
	{
		return -1;
	}
	
	return 0;
	//return *pszBucket1 - *pszBucket2;
}
static unsigned int hash_code_ull(const void* pv)
{
	const long long * pszBucket = (long long * )pv;
	
	return (unsigned int)*pszBucket;
}




/* c不支持模板定义 纠结*/
int  hash_insert_shopdef(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize)
{
	int iRet = 0;
	ShopBucket *pstPos, stDummy;
	SHOPDEF *pstShopDefOld = NULL;

	if( iSize > sizeof(ShopBucket))
	{
		return -1;
	}

	pstShopDefOld = z_hash_shopdef(pstEnv,uiKey);
	if( NULL!= pstShopDefOld )
	{
		if( memcmp(pszData,pstShopDefOld,sizeof(SHOPDEF)) != 0   ) 
		{
			/* 修改的数据 */
			memcpy( pstShopDefOld, pszData,sizeof(SHOPDEF));
			iRet = 1;
		}
	}
	else
	{
		iRet = 1;
	}
	
	// 不管有没有 都做插入操作
	stDummy.uiKey = uiKey;
	pstPos = (ShopBucket *)sht_insert_unique(pstEnv->pstShopHash, &stDummy, hash_cmp_int,
		                        hash_code_int);
	if(pstPos)
	{
		pstPos->uiKey = uiKey;
		memcpy( pstPos->szData, pszData,iSize);
	}
	
	return iRet;
}

int  hash_insert_shoplimitdef(ZONESVRENV *pstEnv,unsigned int uiKey,char* pszData,unsigned int iSize)
{
	int iRet = 0;
	ShopLimitBucket *pstPos, stDummy;
	SHOPLIMITDEF *pstShopLimitDefOld = NULL;

	if( iSize > sizeof(ShopLimitBucket))
	{
		return -1;
	}

	pstShopLimitDefOld = z_hash_shoplimitdef(pstEnv,uiKey);
	if( NULL!= pstShopLimitDefOld )
	{
		if( memcmp(pszData,pstShopLimitDefOld,sizeof(SHOPLIMITDEF)) != 0   ) 
		{
			/* 修改的数据 */
			memcpy( pstShopLimitDefOld, pszData,sizeof(SHOPLIMITDEF));
			iRet = 1;
		}
	}
	else
	{
		iRet = 1;
	}
	
	// 不管有没有 都做插入操作
	stDummy.uiKey = uiKey;
	pstPos = (ShopLimitBucket *)sht_insert_unique(pstEnv->pstShopLimitDefHash, &stDummy, hash_cmp_int,
		                        hash_code_int);
	if(pstPos)
	{
		pstPos->uiKey = uiKey;
		memcpy( pstPos->szData, pszData,iSize);
	}
	
	return iRet;
}

SHOPDEF * z_hash_shopdef(ZONESVRENV *pstEnv,unsigned int uiKey)
{
	ShopBucket *pstPos, stDummy;

	stDummy.uiKey = uiKey;
	pstPos = (ShopBucket *)sht_find(pstEnv->pstShopHash, &stDummy, hash_cmp_int, hash_code_int);

	if( pstPos != NULL )
	{
		return (SHOPDEF *)pstPos->szData;
	}
	
	return  NULL;
}

SHOPLIMITDEF * z_hash_shoplimitdef(ZONESVRENV *pstEnv,unsigned int uiKey)
{
	ShopLimitBucket *pstPos, stDummy;

	stDummy.uiKey = uiKey;
	pstPos = (ShopLimitBucket *)sht_find(pstEnv->pstShopLimitDefHash, &stDummy, hash_cmp_int, hash_code_int);

	if( pstPos != NULL )
	{
		return (SHOPLIMITDEF *)pstPos->szData;
	}
	
	return  NULL;
}

// 只做对内存的初始化
int z_init_shopdef_hash_mem(ZONESVRENV *pstEnv)
{	
	pstEnv->pstShopHash = sht_init(pstEnv->pstZoneObj->stShopMemory.sMem, 
	                             			sizeof(pstEnv->pstZoneObj->stShopMemory.sMem),
								MAX_BUCKET_SHOP_RECORD, 
								MAX_SHOP_DEF_LINES, 
								sizeof(ShopBucket));
	return 0;
}
int z_load_shopdef_data(ZONESVRENV *pstEnv)
{
	memset((char*)g_astShopDef,0,sizeof(g_astShopDef));
	g_iModifyNum = 0;
	g_SelectCountShopNum = 0;
	g_LoadAllShopNum = 0;
	g_SelectCountShopLimitNum = 0;
	memset((char*)g_astShopLimitDef,0,sizeof(g_astShopLimitDef));
	g_iModifyLimitNum = 0;
	g_LoadAllShopLimitNum = 0;
	g_iShopMsgSendFlag = pstEnv->pstAppCtx->stCurr.tv_sec;
	memset((char*)g_astShopDefTmp, 0, sizeof(g_astShopDefTmp));
	g_iShopDefTmp = 0;
	memset((char*)g_astShopLimitDefTmp, 0, sizeof(g_astShopLimitDefTmp));
	g_iShopLimitDefTmp = 0;
	
	shopping_db_shopmsgdef_count_req(pstEnv);
	shopping_db_shopdef_count_req(pstEnv);
	
	return 0;
}

// 查找商品的定义
SHOPDEF * z_find_shopdef( ZONESVRENV* pstEnv, int iShopID )
{
	return z_hash_shopdef(pstEnv,iShopID);
	
}

/*
PROPSDEF * z_find_propsdef( ZONESVRENV* pstEnv, unsigned int iPropsID )
{
	int i=0;
	for(i=0;i<NEW_PROPSDEF_RECORD_MAX;i++)
	{
		if(pstEnv->PropsDef[i].PropsID == iPropsID )
		{
			return &pstEnv->PropsDef[i];
		}
	}
	return NULL;
}
*/

/* 查找插入交易表 */
TRADESELL * z_find_tradesell( ZONESVRENV* pstEnv, unsigned long long ullID  )
{
	TRADESELL *pstPos = NULL;
	TRADESELL  stDummy;
	memset(&stDummy,0,sizeof(stDummy));
	stDummy.TradeID = ullID;
	pstPos = (TRADESELL *)sht_find(pstEnv->pstZoneObj->pstTradeSellHash, &stDummy, hash_cmp_ull, hash_code_ull);

	return  pstPos;
}

TRADEBUY * z_find_tradebuy( ZONESVRENV* pstEnv, unsigned long long ullID )
{
	TRADEBUY *pstPos = NULL;
	TRADEBUY  stDummy;
	memset(&stDummy,0,sizeof(stDummy));
	stDummy.TradeID = ullID;
	pstPos = (TRADEBUY *)sht_find(pstEnv->pstZoneObj->pstTradeBuyHash, &stDummy, hash_cmp_ull, hash_code_ull);

	return  pstPos;
}

int z_insert_tradesell( ZONESVRENV* pstEnv, TRADESELL *pstTradeSell)
{
	TRADESELL stDummy, *pstInserted = NULL;
		
	stDummy.TradeID= pstTradeSell->TradeID;
	pstInserted = sht_insert_unique(pstEnv->pstZoneObj->pstTradeSellHash, &stDummy, hash_cmp_ull, hash_code_ull);
	if(pstInserted)
	{
		memcpy(pstInserted, pstTradeSell, sizeof(TRADESELL));
	}
	else
	{
		return -1;
	}
		
	return 0;
}
	
int z_insert_tradebuy( ZONESVRENV* pstEnv, TRADEBUY *pstTradeBuy)
{
	TRADEBUY stDummy, *pstInserted = NULL;
	
	stDummy.TradeID= pstTradeBuy->TradeID;
	pstInserted = sht_insert_unique(pstEnv->pstZoneObj->pstTradeBuyHash, &stDummy, hash_cmp_ull, hash_code_ull);
	if(pstInserted)
	{
		memcpy(pstInserted, pstTradeBuy, sizeof(TRADEBUY));
	}
	else
	{
		return -1;
	}

	return 0;
}

int z_delete_tradesell( ZONESVRENV* pstEnv, unsigned long long  ullTradeID)
{
	TRADESELL stDummy;
	stDummy.TradeID= ullTradeID;

	if(!sht_remove(pstEnv->pstZoneObj->pstTradeSellHash, &stDummy, hash_cmp_ull, hash_code_ull))
	{
		return -1;
	}
		
	return 0;
}

int z_delete_tradebuy( ZONESVRENV* pstEnv, unsigned long long  ullTradeID)
{
	TRADEBUY stDummy;
	stDummy.TradeID= ullTradeID;

	if(!sht_remove(pstEnv->pstZoneObj->pstTradeBuyHash, &stDummy, hash_cmp_ull, hash_code_ull))
	{
		return -1;
	}
		
	return 0;
}
int shopping_check_limitdef(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,	SHOPLIMITDEF *pstShopLimitDef)
{
	int i = 0;
	//int j = 0;
	int tm_hour = 0;
	int tm_min = 0;
	int iBeginTime = 0;
	int iCurrTime;
	time_t tTime;
	SHOPDEF *pstShopDef = NULL;
	struct tm tmTime;
	UNUSED(pstEnv);
	
	tTime = pstAppCtx->stCurr.tv_sec;
	localtime_r(&tTime, &tmTime);
	iCurrTime = tmTime.tm_hour * 3600  + tmTime.tm_min*60;
	
	pstShopDef =  z_find_shopdef( pstEnv, pstShopLimitDef->ShopID);
	if(!pstShopDef )
	{
		return -1;
	}

	for ( i=0; i<MAX_SHOP_LIMIT_ITME_COUNT; i++ )
	{
		if (pstShopLimitDef->ShopLimitTime[i].SellBeginTime[0] == 0)
		{
			break;
		}
		
		tm_hour = 0;
		tm_min = 0;
		sscanf(pstShopLimitDef->ShopLimitTime[i].SellBeginTime,"%2d:%2d",
	                  &tm_hour,
	                  &tm_min);
		iBeginTime = tm_hour * 3600 + tm_min*60 ;
		
		if (iBeginTime <= iCurrTime && (int)pstShopLimitDef->FreshTime < iBeginTime)
		{
			if (0 == pstShopLimitDef->LimitSelledNum)
			{
				pstShopLimitDef->FreshTime = iBeginTime;
			}
			else
			{
				pstShopLimitDef->FreshTime = iBeginTime;
				pstShopLimitDef->LimitSelledNum = 0;
				pstShopLimitDef->ModifyFlag |= SHOPLIMIT_FLAG_MODIFY;
			}

			break;
		}
	}
			
	return 0;
}


/* 核实限量物品是否能买 */
int shopping_check_limitshop(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,
						SHOPDEF *pstShopDef,CSSHOPPINGBUYREQ *pstShoppingBuyReq)
{	
	SHOPLIMITDEF *pstShopLimitDef;
	
	pstShopLimitDef = z_hash_shoplimitdef(pstEnv,pstShopDef->ShopID);
	if( NULL == pstShopLimitDef )
	{
		// 没有限制定义
		return 0;
	}
	//判断下个数
	if( pstShopLimitDef->LimitSellTotalNum != 0  &&
		((pstShopLimitDef->LimitSelledNum +  pstShoppingBuyReq->ShoppingNum) > pstShopLimitDef->LimitSellTotalNum) )
	{
		z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP1);
		return -1;
	}

	return 0;
}

int shopping_package_add(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,
		CSSHOPPINGBUYREQ *pstShoppingBuyReq,SHOPDEF *pstShopDef,int iTestFlag)
{
	ROLEGRID astRoleGrid[MAX_CHG_NUM];
	int  i, k;
	int iGridNum = 0;
	ITEMDEF *pstItemDef = NULL;
	SHOPLIMITDEF *pstShopLimitDef;
		
	memset(&astRoleGrid,0,sizeof(astRoleGrid));

	if (0 >= pstShoppingBuyReq->ShoppingNum)
	{
		return -1;
	}

	if(is_mt_gift_item(pstShopDef->Items[0].ItemID))
	{
		if(pstShoppingBuyReq->ShoppingNum != 1)
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHOP301);
			return -1;
		}

		if(0 > player_mt_gift_add_check(pstEnv, pstPlayer))
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHOP302);
			return -1;
		}

		if(iTestFlag)
		{
			return 0;
		}
		else
		{
			player_mt_gift_add(pstEnv, pstPlayer);
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SHOP303);
			return 0;
		}
	}

	for (i = 0; i<(int)pstShoppingBuyReq->ShoppingNum; i++)
	{
		for (k=0; k<SHOP_ITEM_COUNT; k++)
		{
			if (iGridNum >= MAX_CHG_NUM)
			{
				z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP2);
				return -1;
			}
			
			if (0 == pstShopDef->Items[k].ItemID)
			{
				break;
			}
			
			pstItemDef = z_find_itemdef( pstEnv, pstShopDef->Items[k].ItemID);
			if (NULL == pstItemDef)
			{
				return -1;
			}

			if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &astRoleGrid[iGridNum], pstShopDef->Items[k].ItemID))
			{
				return -1;
			}

			if (astRoleGrid[iGridNum].Type == GRID_ITEM_ARM)
			{
				if (pstShopDef->Items[k].ItemNum != 1)
				{
					return -1;
				}
			}
			else if (astRoleGrid[iGridNum].Type == GRID_ITEM_ITEM)
			{
				if (pstShopDef->Items[k].ItemNum > 65535)
				{
					return -1;
				}

				astRoleGrid[iGridNum].GridData.RoleItem.Num = pstShopDef->Items[k].ItemNum;
			}
			else
			{
				return -1;
			}
			
			//绑定货币购买的东西一定是绑定的
			if(pstShopDef->BuyType == 2)
			{
				astRoleGrid[iGridNum].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}

			iGridNum++;	
		}
	}

	//合并堆叠
	for (i=iGridNum-1; i>=0; i--)
	{
		if (astRoleGrid[i].Type == GRID_ITEM_ITEM)
		{
			pstItemDef = z_find_itemdef(pstEnv, astRoleGrid[i].GridData.RoleItem.DefID);
			if (pstItemDef && pstItemDef->MaxPile > 1)
			{
				for (k=0; k<i; k++)
				{
					if (astRoleGrid[k].Type == GRID_ITEM_ITEM &&
						astRoleGrid[k].GridData.RoleItem.DefID == astRoleGrid[i].GridData.RoleItem.DefID)
					{
						if (astRoleGrid[k].GridData.RoleItem.Num > 0 && astRoleGrid[k].GridData.RoleItem.Num < pstItemDef->MaxPile &&
							astRoleGrid[i].GridData.RoleItem.Num > 0 && astRoleGrid[i].GridData.RoleItem.Num < pstItemDef->MaxPile)
						{
							if (astRoleGrid[k].GridData.RoleItem.Num + astRoleGrid[i].GridData.RoleItem.Num > pstItemDef->MaxPile)
							{
								astRoleGrid[i].GridData.RoleItem.Num -= (pstItemDef->MaxPile - astRoleGrid[k].GridData.RoleItem.Num);
								astRoleGrid[k].GridData.RoleItem.Num = pstItemDef->MaxPile;
							}
							else
							{
								astRoleGrid[k].GridData.RoleItem.Num += astRoleGrid[i].GridData.RoleItem.Num;
								astRoleGrid[i].GridData.RoleItem.Num = 0;
								iGridNum--;
								if (i != iGridNum)
								{
									memcpy(&astRoleGrid[i], &astRoleGrid[iGridNum], sizeof(astRoleGrid[0]));
								}
							}
						}
					}
				}
			}
		}
	}
	
	if (iTestFlag)
	{
		if (pstShoppingBuyReq->GiftFlag == 1)
		{
			if (pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
				return -1;
			}
			
			if (pstEnv->pstMailPool->iUsed + MAX_CHG_NUM > pstEnv->pstMailPool->iMax)
			{
				return -1;
			}
		}
		else
		{
			if (0 > package_add_test(pstAppCtx, pstEnv, pstPlayer, astRoleGrid, iGridNum,2))
			{
				if (pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
				{
					z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_NOSPACE);
					return -1;
				}
				
				if (pstEnv->pstMailPool->iUsed + MAX_CHG_NUM > pstEnv->pstMailPool->iMax)
				{
					return -1;
				}
			}
		}
	}
	else
	{
		char szMailTitle[NAME_LEN];
		char szMailTxt[SHORT_DESC_LEN];
		
		if (pstShoppingBuyReq->GiftFlag == 1)
		{
			snprintf(szMailTitle,sizeof(szMailTitle),LJY_MAIL16);
			szMailTitle[sizeof(szMailTitle) - 1] = 0;

			g_iZoneBus = player_master_busid_get(pstEnv, pstPlayer);
			player_sysmail_fast_send( pstEnv, pstShoppingBuyReq->GiftToPlayerName, szMailTitle,
								 pstShoppingBuyReq->GiftToDesc, astRoleGrid, iGridNum,
								 0,NULL,0);	
			g_iZoneBus = 0;
		}
		else
		{
			if(0 == package_add_test(pstAppCtx, pstEnv, pstPlayer, astRoleGrid, iGridNum,1))
			{
				package_add(pstAppCtx, pstEnv, pstPlayer, astRoleGrid, iGridNum,1,IMPITEM_OPTYPE_SHOPPING);
			}
			else
			{
				snprintf(szMailTitle,sizeof(szMailTitle),LJY_MAIL17);
				szMailTitle[sizeof(szMailTitle) - 1] = 0;

				snprintf(szMailTxt, sizeof(szMailTxt), LJY_MAIL18,
						pstShoppingBuyReq->ShoppingNum, pstShopDef->Name);
				szMailTxt[sizeof(szMailTxt) - 1] = 0;
			
				player_sysmail_fast_send( pstEnv, pstPlayer->stRoleData.RoleName, szMailTitle,
								 szMailTxt, astRoleGrid, iGridNum, 0,NULL,0);
			}
		}
	}
	
	//　把定义的数据也加回去
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		iTestFlag == 0 )
	{
		pstShopLimitDef = z_hash_shoplimitdef(pstEnv, pstShopDef->ShopID);
		if(pstShopLimitDef != NULL )
		{
			if(pstShopLimitDef->LimitSellTotalNum >0 )
			{
				pstShopLimitDef->LimitSelledNum += pstShoppingBuyReq->ShoppingNum;
				pstShopLimitDef->ModifyFlag |= SHOPLIMIT_FLAG_MODIFY;
				shopping_modify_limit_res( pstEnv->pstAppCtx,pstEnv,pstPlayer,pstShopLimitDef, 1);

				shopping_limit_num_add_bulletin( pstAppCtx, pstEnv, pstShopLimitDef,pstShoppingBuyReq->ShoppingNum);
			}
		}
	}
	
	return 0;
}

//用金券来买物品
int shopping_by_bindgodcoin(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,
		CSSHOPPINGBUYREQ *pstShoppingBuyReq,SHOPDEF *pstShopDef)
{	
	unsigned int uigodcoin = 0; /* 金券*/

	unsigned long long ullBuyPrice = 0;
	unsigned long long ullShoppingNum = 0;
	unsigned long long ullgodcoin = 0;
	unsigned long long ullgodcoinTmp = 0;

	if( 0 > shopping_check_limitshop(pstAppCtx,pstEnv,pstPlayer,pstShopDef,pstShoppingBuyReq) )
	{
		return -1;
	}

	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING)
	{
		SSPKG stPkg;
		SSBATTLEBUYREQ *pstBattleBuyReq = &stPkg.Body.BattleBuyReq;

		if(pstPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_CONNECT)
		{
			return -1;
		}

		memset(pstBattleBuyReq,0,sizeof(SSBATTLEBUYREQ));
		pstBattleBuyReq->Uin = pstPlayer->stRoleData.Uin;
		pstBattleBuyReq->RoleID = pstPlayer->stRoleData.RoleID;
		pstBattleBuyReq->ExtLen = 0;
		memcpy(pstBattleBuyReq->ExtData+pstBattleBuyReq->ExtLen,pstShoppingBuyReq,sizeof(CSSHOPPINGBUYREQ));
		pstBattleBuyReq->ExtLen += sizeof(CSSHOPPINGBUYREQ);
		memcpy(pstBattleBuyReq->ExtData+pstBattleBuyReq->ExtLen,pstShopDef,sizeof(SHOPDEF));
		pstBattleBuyReq->ExtLen += sizeof(SHOPDEF);

		Z_SSHEAD_INIT(&stPkg.Head, BATTLE_SHOPPING_REQ);
		z_ssmsg_send(pstEnv, &stPkg, pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID);
		
		return 0;
	}

	ullBuyPrice =  pstShopDef->BuyPrice;
	ullShoppingNum = pstShoppingBuyReq->ShoppingNum;
	ullgodcoin = ( (100 - pstShopDef->Sale ) / 100.00 ) *  ullBuyPrice *ullShoppingNum;

	/* 处理下打折物品 */
	uigodcoin = ( (100 - pstShopDef->Sale ) / 100.00 ) * pstShopDef->BuyPrice *pstShoppingBuyReq->ShoppingNum;

	ullgodcoinTmp = uigodcoin;
	if(ullgodcoin != ullgodcoinTmp)
	{
		// 溢出处理了
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP3);
		return -1;
	}

	if(uigodcoin <= 0 )
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP3);
		return -1;
	}

	/* 金券检查 */
	if(package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN,
		                          uigodcoin ) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_NOMONEY1);
		return -1;
	}

	//此类道具不能赠送
	if(buy_limit_date_send_check(pstEnv,pstShoppingBuyReq,pstShopDef) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP_LIMIT4,pstShopDef->Name);
		return -1;
	}

	//此类道具一次只能买一个
	if(buy_limit_date_num_check(pstEnv,pstShoppingBuyReq,pstShopDef) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP_LIMIT3,pstShopDef->Name);
		return -1;
	}

	/* 购买时间限制检查 */
	if(buy_limit_date_check(pstEnv,pstPlayer,pstShopDef) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP_LIMIT1,pstShopDef->LimitDate,pstShopDef->Name);
		return -1;
	}

	//检查是否还未使用就又购买了
	if(buy_limit_date_use_check(pstEnv,pstPlayer,pstShopDef) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP_LIMIT2);
		return -1;
	}
	
	/* 核实商品能否加入背包 */
	if( 0 >  shopping_package_add( pstAppCtx, pstEnv,pstPlayer,pstShoppingBuyReq,pstShopDef,1) )
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP3);
		return -1;
	}

	// 扣钱加商品 
	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_BINDGODCOIN,uigodcoin))
	{
		return -1;
	}

	shopping_package_add( pstAppCtx, pstEnv,pstPlayer,pstShoppingBuyReq,pstShopDef,0);

	if (pstShoppingBuyReq->GiftFlag == 1)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP4);
	}
	else
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP5);
	}

	//限制购买时间的商品，记录购买时间
	buy_limit_date_record(pstEnv,pstPlayer,pstShopDef);

	/* 写消耗金券的日志 */
	z_role_godcoin_oplog(pstEnv, pstPlayer, 1,uigodcoin, pstShoppingBuyReq, pstShopDef);
	z_bindgold_consume_oplog(pstEnv, pstPlayer, BINDGOLD_OPTYPE_SHOPPING, pstShoppingBuyReq->ShoppingID, 
				uigodcoin, "金券商城");

	tlog_info(pstEnv->pstLogCat, 0, 0,	
		"|3007|%s购买成功|account|%s|roleid|%llu|rolename|%s|GiftFlag|%u|GiftToPlayerName|%s|GiftToRoleID|%llu|bindpoint|%u|ShoppingID|%u|ShoppingNum|%u|",
		MONEY_TYPE_BIND_GODCOIN_NAME, 
		pstPlayer->szAccount,
		pstShoppingBuyReq->RoleID,
		pstPlayer->stRoleData.RoleName,
		pstShoppingBuyReq->GiftFlag,
		pstShoppingBuyReq->GiftToPlayerName,
		pstShoppingBuyReq->GiftToRoleID,
		uigodcoin,
		pstShoppingBuyReq->ShoppingID,
		pstShoppingBuyReq->ShoppingNum	
		);	
	
	return 0;
}

int shopping_by_godcoin(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,
		CSSHOPPINGBUYREQ *pstShoppingBuyReq,SHOPDEF *pstShopDef)
{	
	unsigned int uigodcoin = 0; /* 金币*/

	unsigned long long ullBuyPrice = 0;
	unsigned long long ullShoppingNum = 0;
	unsigned long long ullgodcoin = 0;
	unsigned long long ullgodcoinTmp = 0;

	if( 0 > shopping_check_limitshop(pstAppCtx,pstEnv,pstPlayer,pstShopDef,pstShoppingBuyReq) )
	{
		return -1;
	}

	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode &&
		pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING)
	{
		SSPKG stPkg;
		SSBATTLEBUYREQ *pstBattleBuyReq = &stPkg.Body.BattleBuyReq;

		if(pstPlayer->stOnline.stSpanOnline.iStat != SPAN_STAT_CONNECT)
		{
			return -1;
		}

		memset(pstBattleBuyReq,0,sizeof(SSBATTLEBUYREQ));
		pstBattleBuyReq->Uin = pstPlayer->stRoleData.Uin;
		pstBattleBuyReq->RoleID = pstPlayer->stRoleData.RoleID;
		pstBattleBuyReq->ExtLen = 0;
		memcpy(pstBattleBuyReq->ExtData+pstBattleBuyReq->ExtLen,pstShoppingBuyReq,sizeof(CSSHOPPINGBUYREQ));
		pstBattleBuyReq->ExtLen += sizeof(CSSHOPPINGBUYREQ);
		memcpy(pstBattleBuyReq->ExtData+pstBattleBuyReq->ExtLen,pstShopDef,sizeof(SHOPDEF));
		pstBattleBuyReq->ExtLen += sizeof(SHOPDEF);

		Z_SSHEAD_INIT(&stPkg.Head, BATTLE_SHOPPING_REQ);
		z_ssmsg_send(pstEnv, &stPkg, pstPlayer->stOnline.stSpanOnline.stInfo.stConnect.iDstID);
		
		return 0;
	}

	ullBuyPrice =  pstShopDef->BuyPrice;
	ullShoppingNum = pstShoppingBuyReq->ShoppingNum;
	ullgodcoin = ( 100 - pstShopDef->Sale )  *  ullBuyPrice *ullShoppingNum/100.0;

	/* 处理下打折物品 */
	uigodcoin = ( 100 - pstShopDef->Sale  ) * pstShopDef->BuyPrice *pstShoppingBuyReq->ShoppingNum/100.0;

	ullgodcoinTmp = uigodcoin;
	if(ullgodcoin != ullgodcoinTmp)
	{
		// 溢出处理了
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP3);
		return -1;
	}

	if(uigodcoin <= 0 )
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP3);
		return -1;
	}

	/* 金币检查 */
	if(package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN,uigodcoin ) < 0)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_NOMONEY2);
		return -1;
	}

	//此类道具不能赠送
	if(buy_limit_date_send_check(pstEnv,pstShoppingBuyReq,pstShopDef) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP_LIMIT4,pstShopDef->Name);
		return -1;
	}
		
	//此类道具一次只能买一个
	if(buy_limit_date_num_check(pstEnv,pstShoppingBuyReq,pstShopDef) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP_LIMIT3,pstShopDef->Name);
		return -1;
	}

	/* 购买时间限制检查 */
	if(buy_limit_date_check(pstEnv,pstPlayer,pstShopDef) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP_LIMIT1,pstShopDef->LimitDate,pstShopDef->Name);
		return -1;
	}

	//检查是否还未使用就又购买了
	if(buy_limit_date_use_check(pstEnv,pstPlayer,pstShopDef) < 0)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP_LIMIT2);
		return -1;
	}

	/* 核实商品能否加入背包 */
	if( 0 >  shopping_package_add( pstAppCtx, pstEnv,pstPlayer,pstShoppingBuyReq,pstShopDef,1) )
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP3);
		return -1;
	}

	// 扣钱加商品 
	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN,uigodcoin))
	{
		return -1;
	}
	shopping_package_add( pstAppCtx, pstEnv,pstPlayer,pstShoppingBuyReq,pstShopDef,0);

	if (pstShoppingBuyReq->GiftFlag == 1)
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP4);
	}
	else
	{
		z_send_sys_str_msg( pstEnv->pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP5);
	}

	
	//限制购买时间的商品，记录购买时间
	buy_limit_date_record(pstEnv,pstPlayer,pstShopDef);

	godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_SHOPPING,0,pstShopDef->ShopID, pstShoppingBuyReq->ShoppingNum,
					uigodcoin,"商城购买商品");

	/* 写消耗金币的日志 */
	z_role_godcoin_oplog(pstEnv, pstPlayer, 0,uigodcoin, pstShoppingBuyReq, pstShopDef);

	tlog_info(pstEnv->pstLogCat, 0, 0,	
		"|3007|%s购买成功|account|%s|roleid|%llu|rolename|%s|GiftFlag|%u|GiftToPlayerName|%s|GiftToRoleID|%llu|bindpoint|%u|ShoppingID|%u|ShoppingNum|%u|",
		MONEY_TYPE_GODCOIN_NAME, 
		pstPlayer->szAccount,
		pstShoppingBuyReq->RoleID,
		pstPlayer->stRoleData.RoleName,
		pstShoppingBuyReq->GiftFlag,
		pstShoppingBuyReq->GiftToPlayerName,
		pstShoppingBuyReq->GiftToRoleID,
		uigodcoin,
		pstShoppingBuyReq->ShoppingID,
		pstShoppingBuyReq->ShoppingNum	
		);	

	return 0;
}

/**
 * Pass9Account 操作用户名为Pass9Account的用户点券
 * uin 操作者的uin
 */
int shopping_operate_point_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,  char *Pass9Account,
									int Uin,
									unsigned int  iTianYuan,void * pData,
									unsigned int uiLen, char DataType,char OperType)
{
	SSDECTIANYUANREQ stDecTianYuanReq;

	memset(&stDecTianYuanReq,0,sizeof(stDecTianYuanReq));

	STRNCPY(stDecTianYuanReq.Pass9Account,Pass9Account,
		      sizeof(stDecTianYuanReq.Pass9Account));
	stDecTianYuanReq.TianYuan = iTianYuan;
	stDecTianYuanReq.Uin = Uin;
	stDecTianYuanReq.WorldID = pstEnv->unWorldID;
	stDecTianYuanReq.OperType = OperType;
	stDecTianYuanReq.ExtraData.DataType = DataType;	
	stDecTianYuanReq.ExtraData.ExtraDataLen = 0;

	if(uiLen>MAX_DEC_TIANYUAN_PKG_EXTRA_LEN)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"shopping_operate_point_req error uiLen:%d",uiLen);
		return -1;
	}
	memcpy(stDecTianYuanReq.ExtraData.ExtraData + stDecTianYuanReq.ExtraData.ExtraDataLen,
		pData,uiLen);
	stDecTianYuanReq.ExtraData.ExtraDataLen += uiLen;

	shopping_ss_send( pstAppCtx, pstEnv, DEC_BILL_REQ,(char *)&stDecTianYuanReq,sizeof(stDecTianYuanReq));
	return 0;
}


int shopping_operate_point_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSDECTIANYUANRES *pstDecTianYuanRes)
{
	//Player *pstPlayer = NULL;
	UNUSED(pstAppCtx);

	if(DATATYPE_POINT_BY_EXCHANGE == pstDecTianYuanRes->ExtraData.DataType)
	{
		shopping_exchange_point_res(pstAppCtx,pstEnv,pstDecTianYuanRes);
		return 0;
	}
	else
	{
		return -1;
	}
}

int shopping_get_sigle_roleinfo(Player *pstPlayer,ROLELISTINFO *stRoleListInfo)
{
	if(stRoleListInfo == NULL || pstPlayer== NULL )
	{
		return -1;
	}
	stRoleListInfo->Uin = pstPlayer->stRoleData.Uin;
	stRoleListInfo->RoleID = pstPlayer->stRoleData.RoleID;
	stRoleListInfo->Level = pstPlayer->stRoleData.Level;
	STRNCPY(stRoleListInfo->RoleName,pstPlayer->stRoleData.RoleName,CCH(stRoleListInfo->RoleName));
	stRoleListInfo->Career = pstPlayer->stRoleData.Career;
	stRoleListInfo->DelTime = pstPlayer->stRoleData.DelTime;

	return 0;
}
int shopping_buy_check(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGBUYREQ *pstShoppingBuyReq,
							SHOPDEF *pstShopDef,ROLELISTINFO *pstRoleListInfo)
{
	UNUSED(pstShopDef);
	UNUSED(pstAppCtx);
	if(pstShoppingBuyReq->GiftFlag == 1 )
	{
		if( pstRoleListInfo == NULL )
		{
			return -1;
		}
		if(pstRoleListInfo->RoleID != pstShoppingBuyReq->GiftToRoleID )
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP6);
			return -1;
		}
		/* 如果是赠送 */
		if (!relation_buddy_online_find(&pstPlayer->stRoleData.Buddy, pstRoleListInfo->RoleID))
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP7);
			return -1;
		}
		if(pstRoleListInfo->DelTime > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP8);
			return -1;
		}
	}

	// 这里只做vip和vip的等级限制，稍候实现 
	
	return 0;
}

int shopping_buy_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGBUYREQ *pstShoppingBuyReq)
{
	SHOPDEF *pstShopDef = NULL;
	Player *pstBuddyPlayer = NULL;
	ROLELISTINFO stRoleListInfo;

	if(!pstPlayer)
	{
		return -1;
	}

	if (0 >= pstShoppingBuyReq->ShoppingNum || pstShoppingBuyReq->ShoppingNum > 1000000)
	{
		return -1;
	}
	
	if ( player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	if( pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN  ||
		(pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING &&
		 (pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT)/* &&
		 pstShoppingBuyReq->GiftFlag == 0*/) )
	{
		//可以操作的
	}
	else
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
		return -1;
	}

	pstShopDef = z_find_shopdef(pstEnv,pstShoppingBuyReq->ShoppingID);
	if( NULL == pstShopDef )
	{
		z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP3);
		return -1;
	}
	
	if( strlen(pstShopDef->ShowDate)>0 && 	
		pstAppCtx->stCurr.tv_sec < string2time(pstShopDef->ShowDate) )
	{
		z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP9);
		return -1;
	}

	if(strlen(pstShopDef->HideDate)>0 &&
	      pstAppCtx->stCurr.tv_sec > string2time(pstShopDef->HideDate) )
	{
		z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP10);
		return -1;
	}
	
	
	if(pstShoppingBuyReq->GiftFlag == 1 )
	{
		if (INST_MODE_ZONE_SVR != pstEnv->iInstMode)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			return -1;
		}

		//暂时屏蔽金券赠送
		if (2 == pstShoppingBuyReq->BuyFlag)
		{
			return -1;
		}

		//MT礼包不可以赠送，不是道具
		if(is_mt_gift_item(pstShopDef->Items[0].ItemID))
		{
			z_send_sysmsg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHOP304);
			return -1;
		}
		
		/* 对方不在线 */
		pstBuddyPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstShoppingBuyReq->GiftToPlayerName);
		if (NULL == pstBuddyPlayer)
		{
			/* 要去拉取这个人的数据 */
			shopping_db_select_role_req(pstEnv, pstPlayer->stRoleData.Uin,  pstShoppingBuyReq->GiftToRoleID,
						(void *)pstShoppingBuyReq,sizeof(CSSHOPPINGBUYREQ),1);
			return -1;
		}
		shopping_get_sigle_roleinfo(pstBuddyPlayer,&stRoleListInfo);

		if( 0 > shopping_buy_check(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq,
							pstShopDef,&stRoleListInfo) )
		{
			return -1;
		}
	}
	else 
	{
		if ( 0 > shopping_buy_check(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq,
					pstShopDef,NULL) )
		{
			return -1;
		}
	}
	
	if( 1 == pstShopDef->BuyType )
	{
		/* 金币购买 */
		if( 0 > shopping_by_godcoin(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq, pstShopDef) )
		{
			return -1;
		}
	}
	else if( 2 == pstShopDef->BuyType )
	{
		/* 金券购买 */
		if( 0 > shopping_by_bindgodcoin(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq, pstShopDef) )
		{
			return -1;
		}
	}
		
	return 0;
}

int shopping_fresh_shopdef_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGFRESHRECORDREQ *pstShoppingFreshRecordReq)
{
	int i,j = 0;
	int iValid = 0;
	int iCount = 0;
	int iCountPerSec = 5*MAX_PKG_RECORD;
	ShopBucket *pstShopBucket;
	SHOPDEF *pstShopDef;
	CSPKG stPkgRes;
	CSSHOPPINGFRESHRECORDRES *pstShoppingFreshRecordRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingFreshRecordRes;
	//ITEMDEF *pstItemDef = NULL;
	
	UNUSED(pstAppCtx);
	UNUSED(pstShoppingFreshRecordReq);
	
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_FRESH_RES;
	memset(pstShoppingFreshRecordRes,0,sizeof(CSSHOPPINGFRESHRECORDRES));
	
	/* 最少有一个包下去 */
	
	if( pstEnv->pstShopHash->iItem % MAX_PKG_RECORD!= 0)
	{
		pstShoppingFreshRecordRes->PkgTotal = 1;
	}
	pstShoppingFreshRecordRes->PkgTotal += pstEnv->pstShopHash->iItem/MAX_PKG_RECORD;
	pstShoppingFreshRecordRes->ShopTbVersion = 0;

	for( i=pstPlayer->stOnline.iShopSendSeq;  i<pstEnv->pstShopHash->iMax ; i++)
	{
		pstPlayer->stOnline.iShopSendSeq++;
				
		iValid = 0;

		pstShopBucket = (ShopBucket *)sht_pos(pstEnv->pstShopHash, i, &iValid);
		if (NULL == pstShopBucket || !iValid )
		{
			continue;
		}
		pstShopDef = (SHOPDEF *)pstShopBucket->szData;		
		if(strlen(pstShopDef->HideDate)>0 &&
		      pstAppCtx->stCurr.tv_sec > string2time(pstShopDef->HideDate) )
		{
			
			continue;
		}

		iValid = 1;
		for (j=0; j<SHOP_ITEM_COUNT; j++)
		{
			if (0 == pstShopDef->Items[j].ItemID)
			{
				break;
			}

			if (NULL == z_find_itemdef( pstEnv, pstShopDef->Items[j].ItemID))
			{
				iValid = 0;
				break;
			}
		}

		if (!iValid)
		{
			continue;
		}
		
		memcpy(&pstShoppingFreshRecordRes->ShopRecord[pstShoppingFreshRecordRes->RecordNum ],
			(char *)pstShopDef,sizeof(SHOPDEF));
		pstShoppingFreshRecordRes->RecordNum += 1;

		if(pstShoppingFreshRecordRes->RecordNum % MAX_PKG_RECORD == 0 )
		{
			pstPlayer->stOnline.iShopCurrPkg += 1;
			pstShoppingFreshRecordRes->CurrPkg = pstPlayer->stOnline.iShopCurrPkg;
			
			Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

			pstShoppingFreshRecordRes->RecordNum = 0;
		}	

		iCount++;
		if (iCount >= iCountPerSec)
		{
			break;
		}
	}
	
	if( pstShoppingFreshRecordRes->RecordNum >0 )
	{
		pstPlayer->stOnline.iShopCurrPkg += 1;
		pstShoppingFreshRecordRes->CurrPkg = pstPlayer->stOnline.iShopCurrPkg;

		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

		pstShoppingFreshRecordRes->RecordNum = 0;
	} 

	if (pstPlayer->stOnline.iShopSendSeq >= pstEnv->pstShopHash->iMax)
	{
		pstPlayer->stOnline.cMiscFlag |= MISC_FLAG_SEND_SHOPDEF;
		
		shopping_modify_msg_res(pstAppCtx, pstEnv, pstPlayer,0);
		shopping_fresh_shoplimitdef_req(pstAppCtx,pstEnv, pstPlayer);
	}
		
	return 0;
}

int shopping_fresh_shoplimitdef_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer)
{	
	int i = 0;
	int iCount = 0;
	int iValid = 0;
	ShopLimitBucket *pstShopLimitBucket;
	SHOPLIMITDEF *pstShopLimitDef;
	SHOPDEF *pstShopDef;
	CSPKG stPkgRes;
	CSSHOPPINGLIMITFRESHRECORDRES *pstShoppingLimitFreshRecordRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingLimitFreshRecordRes;
	UNUSED(pstAppCtx);
	
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_LIMIT_FRESH_RES;
	memset(pstShoppingLimitFreshRecordRes,0,sizeof(CSSHOPPINGLIMITFRESHRECORDRES));
	
	/* 最少有一个包下去 */
	
	if( pstEnv->pstShopLimitDefHash->iItem % MAX_PKG_RECORD!= 0)
	{
		pstShoppingLimitFreshRecordRes->PkgTotal = 1;
	}
	pstShoppingLimitFreshRecordRes->PkgTotal += pstEnv->pstShopLimitDefHash->iItem/MAX_PKG_RECORD;
	pstShoppingLimitFreshRecordRes->ShopTbVersion = 0;
	
	for( i=0;  i<pstEnv->pstShopLimitDefHash->iMax ; i++)
	{
		iValid = 0;
		pstShopLimitBucket = (ShopLimitBucket *)sht_pos(pstEnv->pstShopLimitDefHash, i, &iValid);
		if (NULL == pstShopLimitBucket || !iValid )
		{
			continue;
		}

		pstShopLimitDef = (SHOPLIMITDEF *)pstShopLimitBucket->szData;
		pstShopDef = z_hash_shopdef(pstEnv, pstShopLimitDef->ShopID);
		if (NULL == pstShopDef || ((strlen(pstShopDef->HideDate)>0 &&
		      pstAppCtx->stCurr.tv_sec > string2time(pstShopDef->HideDate)) ))
		{
			continue;
		}
		
		iCount++;
		
		memcpy(&pstShoppingLimitFreshRecordRes->ShopRecord[pstShoppingLimitFreshRecordRes->RecordNum ],
			(char *)pstShopLimitDef,sizeof(SHOPLIMITDEF));
		pstShoppingLimitFreshRecordRes->RecordNum += 1;

		if(pstShoppingLimitFreshRecordRes->RecordNum % MAX_PKG_RECORD == 0 )
		{
			pstShoppingLimitFreshRecordRes->CurrPkg += 1;
			/* 下发数据包 */
			Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

			pstShoppingLimitFreshRecordRes->RecordNum = 0;
		}		
	}
	
	if( pstShoppingLimitFreshRecordRes->RecordNum >0 )
	{
		pstShoppingLimitFreshRecordRes->CurrPkg += 1;
		/* 下发数据包 */
		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

		pstShoppingLimitFreshRecordRes->RecordNum = 0;
	}
	
	return 0;
}

 int shopping_modify_msg_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,int iBroadcaseFlag)
{	
	//int i = 0;
	//SHOPMSGDEF *pstShopMsgDef = NULL;
	CSPKG stPkgRes;
	CSSHOPPINGMSGRES *pstCSShoppingMsgRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingMsgRes;
	UNUSED(pstAppCtx);
	
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_SHOW_MSG_RES;

	memcpy(&pstCSShoppingMsgRes->ShopMsgDef, pstEnv->pstZoneObj->sShopMsgDef, 
				sizeof(pstCSShoppingMsgRes->ShopMsgDef));
	if (0 == pstCSShoppingMsgRes->ShopMsgDef.ShopMsg[0])
	{
		return 0;
	}

	if(iBroadcaseFlag == 0 && NULL != pstPlayer)
	{
		/* 下发数据包 */
		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	}
	else if(iBroadcaseFlag ==1 )
	{
		/* 广播 */
		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);
	}
	
	return 0;
}

int shopping_modify_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SHOPDEF *pstShopDef, int iCount)
{	
	int i = 0;
	CSPKG stPkgRes;
	CSSHOPPINGMODIFYRECORDRES *pstShoppingModifyRecord = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingModifyRecordRes;
	UNUSED(pstAppCtx);

	memset(pstShoppingModifyRecord,0,sizeof(CSSHOPPINGMODIFYRECORDRES));
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_MODIFY_RES;
	for( i=0; i<iCount; i++)
	{
		memcpy(&pstShoppingModifyRecord->ShopRecord[pstShoppingModifyRecord->RecordNum], pstShopDef+i,sizeof(SHOPDEF));
		pstShoppingModifyRecord->RecordNum += 1;
		if(pstShoppingModifyRecord->RecordNum %MAX_PKG_RECORD == 0)
		{
			/* 广播数据包 */
			Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
			z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);
			
			pstShoppingModifyRecord->RecordNum = 0;
		}
	}

	if( pstShoppingModifyRecord->RecordNum  > 0)
	{
		/* 广播数据包 */
		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);
	}
	
	return 0;
}

int shopping_del_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SHOPDEF *pstShopDef, int iCount)
{	
	int i = 0;
	CSPKG stPkgRes;
	CSSHOPPINGSHOPDELRES *pstShoppingDel = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingShopDelRes;
	UNUSED(pstAppCtx);

	memset(pstShoppingDel,0,sizeof(CSSHOPPINGSHOPDELRES));
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_SHOP_DEL_RES;
	for( i=0; i<iCount && pstShoppingDel->DelNum<MAX_SHOP_DEF_LINES; i++)
	{
		pstShoppingDel->ShopIDs[pstShoppingDel->DelNum] = pstShopDef[i].ShopID;
		pstShoppingDel->DelNum++;
	}
	
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);

	return 0;
}

int shopping_limitdel_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SHOPLIMITDEF *pstShopLimitDef, int iCount)
{	
	int i = 0;
	CSPKG stPkgRes;
	CSSHOPPINGSHOPLIMITDELRES *pstShoppingLimitDel = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingShopLimitDelRes;
	UNUSED(pstAppCtx);

	memset(pstShoppingLimitDel,0,sizeof(CSSHOPPINGSHOPLIMITDELRES));
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_SHOPLIMIT_DEL_RES;
	for( i=0; i<iCount && pstShoppingLimitDel->DelNum<MAX_SHOP_DEF_LINES; i++)
	{
		pstShoppingLimitDel->ShopIDs[pstShoppingLimitDel->DelNum] = pstShopLimitDef[i].ShopID;
		pstShoppingLimitDel->DelNum++;
	}
	
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);

	return 0;
}


int shopping_ss_send(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, 
					int iCmd,char *szData,int iLen)
{
	SSPKG stPkg;
	TDRDATA stNet;
	int iSrc, iDst;

	memcpy(&stPkg.Body,szData,iLen);

	stPkg.Head.Cmd = iCmd;
	stPkg.Head.Magic = PROTOCOL_MAGIC; 
	stPkg.Head.Ver = TDR_METALIB_PROTO_SS_VERSION;
	stPkg.Head.HeadLen = sizeof(stPkg.Head);

	stNet.iBuff = sizeof(pstEnv->szSendBuffer);
	stNet.pszBuff = pstEnv->szSendBuffer;
	if(ss_pack(pstEnv->pstSSHan, &stNet, &stPkg, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"ss_pack fail.");
		return -1;
	}

	iSrc = pstAppCtx->iId;
	iDst = pstEnv->iRegionGateID;
	if(tbus_forward(pstAppCtx->iBus, &iSrc, &iDst, stNet.pszBuff, stNet.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,	"tbus_forward fail.");
		return -1;
	}
	return 0;
}

int shopping_query_dianquan_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,
				unsigned int  OperType,void *pData,unsigned int uiLen)
{		
	SSQUERYTIANYUANREQ stQueryTianYuanReq;
	
	memset(&stQueryTianYuanReq,0,sizeof(stQueryTianYuanReq));
	STRNCPY(stQueryTianYuanReq.Pass9Account,pstPlayer->szAccount,
		      sizeof(stQueryTianYuanReq.Pass9Account));
	stQueryTianYuanReq.Uin = pstPlayer->stRoleData.Uin;
	stQueryTianYuanReq.OperType = OperType;
	stQueryTianYuanReq.ExtraData.DataType = OperType;

	if( uiLen > 0)
	{
		memcpy(stQueryTianYuanReq.ExtraData.ExtraData,pData,uiLen);
		stQueryTianYuanReq.ExtraData.ExtraDataLen = uiLen;
	}

	shopping_ss_send(pstAppCtx,pstEnv,  USER_QUERY_BILL_REQ,(char *)&stQueryTianYuanReq,sizeof(SSQUERYTIANYUANREQ));
	
	return 0;
}

int shopping_query_dianquan_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSQUERYTIANYUANRES *pstSSQueryTianYuanRes)
{	
	
	 Player *pstPlayer = NULL;
	CSPKG stPkgRes;
	CSSHOPPINGQUERYTIANYUANRES *pstShoppingQueryTianYuanRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryTianYuanRes;
	UNUSED(pstAppCtx);
	
	pstPlayer = player_get_by_uin(pstEnv, pstSSQueryTianYuanRes->Uin);
	if (!pstPlayer || !((pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN) || (pstPlayer->eStatus == PLAYER_STATUS_ROLE_NOTHING && pstPlayer->stOnline.stSpanOnline.iStat == SPAN_STAT_CONNECT) ))
	{
		return -1;
	}

	if(pstSSQueryTianYuanRes->Result != 0 )
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP31, MONEY_TYPE_POINTCOIN_NAME);
		return -1;
	}

	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_TIANYUAN_RES;
	memset(pstShoppingQueryTianYuanRes,0,sizeof(CSSHOPPINGQUERYTIANYUANRES));
	
	STRNCPY(pstShoppingQueryTianYuanRes->Pass9Account,pstSSQueryTianYuanRes->BillRecord.Pass9Account,
			CCH(pstShoppingQueryTianYuanRes->Pass9Account));
	pstShoppingQueryTianYuanRes->ServerID = pstSSQueryTianYuanRes->BillRecord.ServerID;
	pstShoppingQueryTianYuanRes->Amount = pstSSQueryTianYuanRes->BillRecord.Amount;
	pstShoppingQueryTianYuanRes->Points = pstSSQueryTianYuanRes->BillRecord.Points;
	pstShoppingQueryTianYuanRes->Bonus = pstSSQueryTianYuanRes->BillRecord.Bonus;
	pstShoppingQueryTianYuanRes->FreezePoint = pstSSQueryTianYuanRes->BillRecord.FreezePoint;
	
	/* 下发数据包 */
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

	return 0;
}


int shopping_package_present_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, 
		CSSHOPPINGPACKAGEPRESENTREQ *pstShoppingPackagePresentReq,int bCheckBuddyName )
{
	ITEMDEF *pstItemDef;
	Player *pstBuddyPlayer = NULL;
	ROLEITEM *pstRoleItem=NULL;
	int iPos=0;
	ROLEGRID stRoleGrid;
	ZONECTLSENDMAILREQ stZoneCtlSendMailReq;
	CSPKG stPkgRes;
	CSSHOPPINGPACKAGEPRESENTRES *pstShoppingPackagePresentRes;

	if(!pstPlayer)
	{
		return -1;
	}
	
	if ( player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	if( LIST_TYPE_SHOPPING != pstShoppingPackagePresentReq->SrcListType ||
		0 >= pstShoppingPackagePresentReq->ItemNum ||
		0 == pstShoppingPackagePresentReq->GiftToRoleID ||
		1 != pstShoppingPackagePresentReq->GiftFlag ||
		pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN )
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP11);
		return -1;
	}
	if (0 > package_get_item(pstEnv, pstPlayer,pstShoppingPackagePresentReq->SrcListType, 
							pstShoppingPackagePresentReq->SrcGridIdx, &pstItemDef, &pstRoleItem, &iPos))
	{
		return -1;
	}
	if( !pstRoleItem)
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP11);
		return -1;
	}
	if(pstShoppingPackagePresentReq->ItemNum  > pstRoleItem->Num )
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP6);
		return -1;
	}
	
	if( 0  >  package_dec_test(pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, pstShoppingPackagePresentReq->ItemNum, 
				pstShoppingPackagePresentReq->SrcListType, &iPos,PACKAGE_DEC_NONE) )
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP6);
		return -1;
	}

	if (!relation_buddy_online_find(&pstPlayer->stRoleData.Buddy, pstShoppingPackagePresentReq->GiftToRoleID))
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP7);
		return -1;
	}	

	if( bCheckBuddyName == 1 )
	{
		/* 对方不在线 */
		pstBuddyPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pstShoppingPackagePresentReq->GiftToPlayerName);
		if (NULL == pstBuddyPlayer)
		{
			/* 要去拉取这个人的数据 */
			shopping_db_select_role_req(pstEnv, pstPlayer->stRoleData.Uin,  pstShoppingPackagePresentReq->GiftToRoleID,
						pstShoppingPackagePresentReq,sizeof(CSSHOPPINGPACKAGEPRESENTREQ),2);
			return -1;
		}

		if( pstBuddyPlayer->stRoleData.DelTime > 0)
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP8);
			return -1;
		}
	}

	
	memset(&stRoleGrid,0,sizeof(stRoleGrid));
	stRoleGrid = pstPlayer->stRoleData.Package.ShoppingPackage.RoleGrids[iPos];
	if( 0 >  package_dec(pstAppCtx, pstEnv, pstPlayer, pstItemDef->ItemID, pstShoppingPackagePresentReq->ItemNum ,
				pstShoppingPackagePresentReq->SrcListType, &iPos,PACKAGE_DEC_NONE, 0) )
	{
		z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP6);
		return -1;
	}
	/* 赠送的多少，就是扣多少*/
	stRoleGrid.GridData.RoleItem.Num = pstShoppingPackagePresentReq->ItemNum;
	/* 发送赠送邮件*/
	
	memset(&stZoneCtlSendMailReq,0,sizeof(stZoneCtlSendMailReq));
	snprintf(stZoneCtlSendMailReq.Core.To,sizeof(stZoneCtlSendMailReq.Core.To),"%s",
		pstShoppingPackagePresentReq->GiftToPlayerName);
	snprintf(stZoneCtlSendMailReq.Core.Send,sizeof(stZoneCtlSendMailReq.Core.Send),"%s",
		pstPlayer->stRoleData.RoleName);
	
	snprintf(stZoneCtlSendMailReq.Core.Title,sizeof(stZoneCtlSendMailReq.Core.Title),
		LJY_MAIL1);
	snprintf(stZoneCtlSendMailReq.Core.Text,sizeof(stZoneCtlSendMailReq.Core.Text),"%s",
		pstShoppingPackagePresentReq->GiftToDesc);

	stZoneCtlSendMailReq.Core.ItemNum = 1;
	stZoneCtlSendMailReq.Core.Items[0] = stRoleGrid;
	ss_mail_sys_send(pstEnv, &stZoneCtlSendMailReq,MAIL_FLAG_SYSTEM);

	/* 下发数据包 */

	pstShoppingPackagePresentRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingPackagePresentRes;
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_PACKAGE_PRESENT_RES;
	pstShoppingPackagePresentRes->Result =0;
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);	

	tlog_info(pstEnv->pstLogCat, 0, 0,	
		"|3008|从商品背包赠送成功|account|%s|RoleName|%s|itemid|%u|req_Num|%u|GiftToPlayerName|%s|GiftToRoleID|%llu|gift_num|%u|remain_num|%u|",
		pstPlayer->szAccount,
		pstPlayer->stRoleData.RoleName,
		stRoleGrid.GridData.RoleItem.DefID,
		pstShoppingPackagePresentReq->ItemNum,
		pstShoppingPackagePresentReq->GiftToPlayerName,
		pstShoppingPackagePresentReq->GiftToRoleID,
		stRoleGrid.GridData.RoleItem.Num,		
		pstRoleItem->Num		
		);	
	return 0;
}

int tradesell_record(ZONESVRENV* pstEnv, TRADERECORD *pstRecord, TRADESELL *pstTradeSell)
{
	pstRecord->CountedDownTime = pstTradeSell->StartTime + pstTradeSell->Time;
	pstRecord->Money = pstTradeSell->Moeny;
	pstRecord->TradeID = pstTradeSell->TradeID;
	pstRecord->Gold = pstTradeSell->Gold;
	pstRecord->TradeType = 0;
	if (TRADE_FLAG_ANON & pstTradeSell->Flag)
	{
		pstRecord->OwnerName[0] = 0;
	}
	else
	{
		STRNCPY(pstRecord->OwnerName,
			pstTradeSell->RoleName, sizeof(pstRecord->OwnerName));
	}
	
	return 0;
}


int tradebuy_record(ZONESVRENV* pstEnv, TRADERECORD *pstRecord, TRADEBUY *pstTradeBuy)
{
	pstRecord->CountedDownTime = pstTradeBuy->StartTime + pstTradeBuy->Time;
		
	pstRecord->Money = pstTradeBuy->Moeny;
	pstRecord->TradeID = pstTradeBuy->TradeID;
	pstRecord->Gold = pstTradeBuy->Gold;
	pstRecord->TradeType = 1;
	if (TRADE_FLAG_ANON & pstTradeBuy->Flag)
	{
		pstRecord->OwnerName[0] = 0;
	}
	else
	{
		STRNCPY(pstRecord->OwnerName,
			pstTradeBuy->RoleName, sizeof(pstRecord->OwnerName));
	}
	
	return 0;
}


int shopping_query_tradesell_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGQUERYTRADESELLREQ *pstShoppingQueryTradeSellReq)
{
	int i = 0;
	unsigned int iCount = 0;
	int iValid;
	CSPKG stPkgRes;
	CSSHOPPINGQUERYTRADESELLRES *pstShoppingQueryTradeSellRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryTradeSellRes;
	TRADESELL *pstTradeSell = NULL;
	time_t tLastTime = 0;
	time_t tCurr = pstAppCtx->stCurr.tv_sec;
	LPSHTABLE pstTradeSellHash = pstEnv->pstZoneObj->pstTradeSellHash;

	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_TRADESELL_RES;
	pstShoppingQueryTradeSellRes->RecordNum = 0;
	pstShoppingQueryTradeSellRes->SvrCurrTotalNum = pstTradeSellHash->iItem;
	
	// 下标是从0开始,所以用<=
	if((unsigned int)pstTradeSellHash->iMax <= pstShoppingQueryTradeSellReq->QueryBeginNum ||
		0 >= pstShoppingQueryTradeSellReq->QueryTotalNum || 
		pstShoppingQueryTradeSellReq->QueryTotalNum > MAX_TRADE_RECORD)
	{
		goto _end;
	}

	for (i=0; i<pstTradeSellHash->iMax; i++)
	{
		pstTradeSell = sht_pos(pstTradeSellHash, i, &iValid);
		if(pstTradeSell == NULL || !iValid )
		{
			continue;
		}

		iCount++;
		if (iCount <= pstShoppingQueryTradeSellReq->QueryBeginNum)
		{
			continue;
		}
		
		if (pstShoppingQueryTradeSellRes->RecordNum > pstShoppingQueryTradeSellReq->QueryTotalNum)
		{
			break;
		}

		tLastTime = pstTradeSell->StartTime + pstTradeSell->Time;
		if( tCurr >= tLastTime )
		{
			continue;
		}

		tradesell_record(pstEnv, &pstShoppingQueryTradeSellRes->TradeRecord[pstShoppingQueryTradeSellRes->RecordNum], pstTradeSell);
		
		pstShoppingQueryTradeSellRes->RecordNum++;
	}
	
_end:	
	/* 下发数据包 */
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

	return 0;
}
int shopping_query_tradebuy_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGQUERYTRADEBUYREQ *pstShoppingQueryTradeBuyReq)
{
	int i = 0;
	unsigned int iCount = 0;
	int iValid;
	CSPKG stPkgRes;
	CSSHOPPINGQUERYTRADEBUYRES *pstShoppingQueryTradeBuyRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryTradeBuyRes;
	TRADEBUY*pstTradeBuy = NULL;
	time_t tLastTime = 0;
	time_t tCurr = pstAppCtx->stCurr.tv_sec;
	LPSHTABLE pstTradeBuyHash = pstEnv->pstZoneObj->pstTradeBuyHash;

	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_TRADEBUY_RES;
	pstShoppingQueryTradeBuyRes->RecordNum = 0;
	pstShoppingQueryTradeBuyRes->SvrCurrTotalNum = pstTradeBuyHash->iItem;

	if((unsigned int)pstTradeBuyHash->iMax <= pstShoppingQueryTradeBuyReq->QueryBeginNum ||
		0 >= pstShoppingQueryTradeBuyReq->QueryTotalNum || 
		pstShoppingQueryTradeBuyReq->QueryTotalNum > MAX_TRADE_RECORD)
	{
		goto _end;
	}

	for (i=0; i<pstTradeBuyHash->iMax; i++)
	{
		pstTradeBuy = sht_pos(pstTradeBuyHash, i, &iValid);
		if(pstTradeBuy == NULL || !iValid )
		{
			continue;
		}

		iCount++;
		if (iCount <= pstShoppingQueryTradeBuyReq->QueryBeginNum)
		{
			continue;
		}
		
		if (pstShoppingQueryTradeBuyRes->RecordNum > pstShoppingQueryTradeBuyReq->QueryTotalNum)
		{
			break;
		}

		tLastTime = pstTradeBuy->StartTime + pstTradeBuy->Time;
		if( tCurr >= tLastTime )
		{
			continue;
		}

		tradebuy_record(pstEnv, &pstShoppingQueryTradeBuyRes->TradeRecord[pstShoppingQueryTradeBuyRes->RecordNum], pstTradeBuy);
		
		pstShoppingQueryTradeBuyRes->RecordNum++;
	}

_end:	
	/* 下发数据包 */
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

	return 0;
}

int shopping_trade_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGTRADEREQ *pstShoppingTradeReq)
{
	TRADESELL *pstTradeSell = NULL;
	TRADEBUY   *pstTradeBuy = NULL;
	unsigned int uiTax = 0;
	
	if ( player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	if( 0 == pstShoppingTradeReq->TradeType )
	{
		/* B用户购买A用户的金币 */
		pstTradeSell = z_find_tradesell( pstEnv, pstShoppingTradeReq->TradeID );
		if( NULL  == pstTradeSell)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHOP12);
			return -1;
		}
		
		if ( pstTradeSell->RoleID ==pstPlayer->stRoleData.RoleID)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHOP13);
			return -1;
		}

		uiTax = pstTradeSell->Moeny * 1.0 * TRADE_TAX_RATE/100;

		/* 判断下B用户的银币数 */
		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstTradeSell->Moeny+uiTax))
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_NOMONEY);
			return -1;
		}

		if (0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstTradeSell->Gold))
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP14, MONEY_TYPE_GODCOIN_NAME);
			return -1;
		}	
		
		{
			/* 删除A用户的发布记录 */
			char szCallBack[1024];
			int iLen = 0;
			TRADEPLAYERINFO stTradePlayerInfo;
						
			memset(szCallBack,0,sizeof(szCallBack));
			shopping_tradeplayerinfo_init(pstPlayer,&stTradePlayerInfo);
			memcpy(szCallBack+iLen,pstShoppingTradeReq,sizeof(CSSHOPPINGTRADEREQ));
			iLen += sizeof(CSSHOPPINGTRADEREQ);
			memcpy(szCallBack+iLen,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));
			iLen += sizeof(TRADEPLAYERINFO);
			shopping_db_tradesell_delete_req(pstEnv,pstPlayer->stRoleData.Uin, pstTradeSell,OPERATE_BY_USER_TRADE,
								szCallBack,iLen);
			
			/* 把内存数据删除掉 */
			z_delete_tradesell(pstEnv,pstTradeSell->TradeID);
		}
	}
	else if( 1 == pstShoppingTradeReq->TradeType)
	{
		TRADEPLAYERINFO stTradePlayerInfo;

		shopping_tradeplayerinfo_init(pstPlayer,&stTradePlayerInfo);
		
		/* C用户购买D用户的银币 */
		pstTradeBuy= z_find_tradebuy( pstEnv, pstShoppingTradeReq->TradeID );
		if( NULL  == pstTradeBuy)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHOP12);
			return -1;
		}
		if ( pstTradeBuy->RoleID == pstPlayer->stRoleData.RoleID)
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_SHOP15);
			return -1;
		}

		uiTax = pstTradeBuy->Moeny * 1.0* TRADE_TAX_RATE/100;
		
		/* 判断下C用户的金币数 */
		if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstTradeBuy->Gold))
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_NOMONEY2);
			return -1;
		}
		
		if (0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstTradeBuy->Moeny-uiTax))
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP14, MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		{
			/* 删除D用户数据 */
			char szCallBack[1024];
			int iLen = 0;
			memset(szCallBack,0,sizeof(szCallBack));
			memcpy(szCallBack+iLen,pstShoppingTradeReq,sizeof(CSSHOPPINGTRADEREQ));
			iLen += sizeof(CSSHOPPINGTRADEREQ);
			memcpy(szCallBack+iLen,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));
			iLen += sizeof(TRADEPLAYERINFO);
			shopping_db_tradebuy_delete_req(pstEnv,pstPlayer->stRoleData.Uin, pstTradeBuy,OPERATE_BY_USER_TRADE,
								szCallBack,iLen);	
						
			z_delete_tradebuy(pstEnv,pstTradeBuy->TradeID);
		}
	}
	
	return 0;
}

//新的发布交易流程
int shopping_send_traderecord_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGSENDTRADERECORDREQ *pstShoppingSendTradeRecordReq)
{
	unsigned int 		uiHandingMoney = 0; //发布的手续费
	unsigned int 		uiOffset = 0;
	char 			szBuf[1024];
	unsigned int           iMoney = 0;
	unsigned int           iGold = 0;

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	 
	if( pstShoppingSendTradeRecordReq->Money<=0 ||
		pstShoppingSendTradeRecordReq->Gold<= 0  ||
		pstShoppingSendTradeRecordReq->Time != MAX_TRADE_SHOW_TIME )
	{
		return -1;
	}

	iMoney = pstShoppingSendTradeRecordReq->Money;
	iGold = pstShoppingSendTradeRecordReq->Gold;
	if (0 == pstShoppingSendTradeRecordReq->TradeType)
	{
		if (iGold != 10 && iGold != 50 && iGold != 100 &&
			iGold != 500 && iGold != 1000)
		{
			return -1;
		}
	}
	else
	{
		if (iMoney != 1000 && iMoney != 5000 && iMoney != 10000 && iMoney != 50000 &&
			iMoney != 100000)
		{
			return -1;
		}
	}

	uiHandingMoney = (iMoney * 1.0 * TRADE_HANDLE_RATE / 100) ;
	if( uiHandingMoney == 0 )
	{
		uiHandingMoney = 1;
	}

	if( 0 == pstShoppingSendTradeRecordReq->TradeType )
	{
		// A用户 要出售金币了 
		TRADESELL stTradeSell;
		memset(&stTradeSell,0, sizeof(stTradeSell));
		
		if( pstEnv->pstZoneObj->pstTradeSellHash->iItem >= MAX_TRADE_RECORD_NUM -10 )
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP16);
			return -1;
		}
		if(pstPlayer->stRoleData.MiscInfo.RoleTradeRecordInfo.TradeSellNum >= MAX_ONEPLAYER_TRADE_RECORD)
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP17);
			return -1;
		}
		
		if( 0 != package_dec_money_test(pstEnv,pstPlayer,ATTR_ID_GODCOIN,iGold) )
		{
			z_send_sys_str_msg( pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP18, MONEY_TYPE_GODCOIN_NAME);
			return -1;
		}

		if( 0 != package_dec_money_test(pstEnv,pstPlayer,ATTR_ID_MONEY,uiHandingMoney) )
		{
			z_send_sys_str_msg( pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP19);
			return -1;
		}

		// 扣A用户的金币
		package_dec_money(pstEnv,pstPlayer,ATTR_ID_GODCOIN,iGold);

		// 扣手续费
		package_dec_money(pstEnv,pstPlayer,ATTR_ID_MONEY,uiHandingMoney);

		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHOP_HANDLE, 0, 0, uiHandingMoney, "金币交易手续费");
								           
		stTradeSell.Gold = iGold;
		STRNCPY(stTradeSell.Pass9Account,pstPlayer->szAccount,CCH(stTradeSell.Pass9Account));
		stTradeSell.Uin = pstPlayer->stRoleData.Uin;
		stTradeSell.RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(stTradeSell.RoleName,pstPlayer->stRoleData.RoleName,CCH(stTradeSell.RoleName));
		stTradeSell.StartTime = pstAppCtx->stCurr.tv_sec;
		stTradeSell.Time = pstShoppingSendTradeRecordReq->Time*3600;
		stTradeSell.Moeny = iMoney;
		if (pstShoppingSendTradeRecordReq->NotAnonymous == 0)
		{
			stTradeSell.Flag |= TRADE_FLAG_ANON;
		}
		
		shopping_db_tradesell_insert_req(pstEnv,pstPlayer->stRoleData.Uin,&stTradeSell );

		tlog_info(pstEnv->pstLogCat, 0, 0,	
			"|3009|出售%s,请求db|account|%s|%s|TradeType|%u|Money|%u|Gold|%u|Time|%u|uiHandingMoney|%u|",
			MONEY_TYPE_GODCOIN_NAME,
			stTradeSell.Pass9Account,
			stTradeSell.RoleName,
			pstShoppingSendTradeRecordReq->TradeType,
			pstShoppingSendTradeRecordReq->Money,
			pstShoppingSendTradeRecordReq->Gold,
			pstShoppingSendTradeRecordReq->Time,
			uiHandingMoney);		
		
	}
	else if( 1 == pstShoppingSendTradeRecordReq->TradeType )
	{ 
		// D用户 要求购金币了 
		TRADEBUY stTradeBuy;
		memset(&stTradeBuy,0, sizeof(stTradeBuy));

		if(pstPlayer->stRoleData.MiscInfo.RoleTradeRecordInfo.TradeBuyNum >= MAX_ONEPLAYER_TRADE_RECORD)
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP20);
			return -1;
		}
		if(pstEnv->pstZoneObj->pstTradeBuyHash->iItem >= MAX_TRADE_RECORD_NUM -10 )
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP21);
			return -1;
		}

		if( 0 != package_dec_money_test(pstEnv,pstPlayer,ATTR_ID_MONEY,iMoney+uiHandingMoney) )
		{
			z_send_sys_str_msg( pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP18, MONEY_TYPE_MONEY_NAME);
			return -1;
		}

		package_dec_money(pstEnv,pstPlayer,ATTR_ID_MONEY,uiHandingMoney+iMoney);
		z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHOP_HANDLE, 1, 0, uiHandingMoney, "银币交易手续费");
	
		stTradeBuy.Moeny = iMoney;
		STRNCPY(stTradeBuy.Pass9Account,pstPlayer->szAccount,CCH(stTradeBuy.Pass9Account));
		stTradeBuy.Uin = pstPlayer->stRoleData.Uin;
		stTradeBuy.Gold = iGold;
		stTradeBuy.RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(stTradeBuy.RoleName,pstPlayer->stRoleData.RoleName,CCH(stTradeBuy.RoleName));
		stTradeBuy.StartTime = pstAppCtx->stCurr.tv_sec;
		stTradeBuy.Time = pstShoppingSendTradeRecordReq->Time*3600; 
		if (pstShoppingSendTradeRecordReq->NotAnonymous == 0)
		{
			stTradeBuy.Flag |= TRADE_FLAG_ANON;
		}
		
		memset(szBuf,0,sizeof(szBuf));
		memcpy(szBuf,&stTradeBuy,sizeof(stTradeBuy));
		uiOffset = sizeof(stTradeBuy);
		memcpy(szBuf+uiOffset,pstShoppingSendTradeRecordReq,sizeof(CSSHOPPINGSENDTRADERECORDREQ));
		uiOffset += sizeof(CSSHOPPINGSENDTRADERECORDREQ);

		shopping_db_tradebuy_insert_req( pstEnv,pstPlayer->stRoleData.Uin,&stTradeBuy,szBuf,uiOffset );

		tlog_info(pstEnv->pstLogCat, 0, 0,	
			"|3010|出售%s,请求db|account|%s|TradeType|%u|Money|%u|Gold|%u|Time|%u|uiHandingMoney|%u",
			MONEY_TYPE_MONEY_NAME,
			stTradeBuy.Pass9Account,
			pstShoppingSendTradeRecordReq->TradeType,
			pstShoppingSendTradeRecordReq->Money,
			pstShoppingSendTradeRecordReq->Gold,
			pstShoppingSendTradeRecordReq->Time,
			uiHandingMoney
			);				
	}
	return 0;
}

int shopping_del_traderecord_by_del_role(ZONESVRENV* pstEnv, tdr_ulonglong RoleID )
{
	int  i = 0;
	TRADESELL *pstTradeSell = NULL;
	TRADEBUY *pstTradeBuy = NULL;
	int iValid  = 0;

	for(  i=0; i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++ ) 
	{
		pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
		if(pstTradeSell == NULL || !iValid )
		{
			continue;
		}
		if(pstTradeSell->RoleID == RoleID)
		{
			
			shopping_db_tradesell_delete_req( pstEnv,0, pstTradeSell,OPERATE_BY_DEL_ROLE,
							pstTradeSell,sizeof(TRADESELL));
			z_delete_tradesell(pstEnv,pstTradeSell->TradeID);
		}
	}
	
	for(  i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++ ) 
	{		
		pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
		if( NULL == pstTradeBuy || !iValid)
		{
			continue;
		}
		if(pstTradeBuy->RoleID == RoleID)
		{
			shopping_db_tradebuy_delete_req( pstEnv,0,pstTradeBuy,
					OPERATE_BY_DEL_ROLE,pstTradeBuy,sizeof(TRADEBUY));
			z_delete_tradebuy(pstEnv,pstTradeBuy->TradeID);	
		}	
	}	

	return 0;
}

int shopping_query_my_traderecord_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGQUERYMYTRADERECORDREQ *pstShoppingQueryMyTradeRecordReq)
{
	unsigned int i = 0, j;
	int  iValid;
	ROLETRADERECORDINFO *pstRoleTradeRecordInfo;
	ROLETRADERECORDINFO  stRoleTradeRecordInfoTmp;
	TRADESELL *pstTradeSell;
	TRADEBUY *pstTradeBuy;
	CSPKG stPkgRes;
	CSSHOPPINGQUERYMYTRADERECORDRES *pstShoppingQueryMyTradeRecordRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryMyTradeRecordRes;
	UNUSED(pstAppCtx);

	memset(pstShoppingQueryMyTradeRecordRes,0,sizeof(CSSHOPPINGQUERYMYTRADERECORDRES));
	memset(&stRoleTradeRecordInfoTmp,0,sizeof(stRoleTradeRecordInfoTmp));
	
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_MY_TRADERECORD_RES;
	/* 先查出售内存 */
	pstRoleTradeRecordInfo = &pstPlayer->stRoleData.MiscInfo.RoleTradeRecordInfo;

	for (i=0; (int)i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++)
	{
		pstTradeSell = (TRADESELL *)sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
		if (NULL == pstTradeSell || !iValid )
		{
			continue;
		}

		if (pstTradeSell->RoleID == pstPlayer->stRoleData.RoleID)
		{
			for (j=0; j<pstRoleTradeRecordInfo->TradeSellNum; j++)
			{
				if (pstRoleTradeRecordInfo->TradeSellID[j] == pstTradeSell->TradeID)
				{
					break;
				}
			}

			if (j == pstRoleTradeRecordInfo->TradeSellNum)
			{
				if (pstRoleTradeRecordInfo->TradeSellNum < MAX_SIGLE_USER_TRADERECORD_NUM)
				{
					pstRoleTradeRecordInfo->TradeSellID[pstRoleTradeRecordInfo->TradeSellNum] = pstTradeSell->TradeID;
					pstRoleTradeRecordInfo->TradeSellNum++;

					if (pstTradeSell->StartTime > pstRoleTradeRecordInfo->TradeSendTime)
					{
						pstRoleTradeRecordInfo->TradeSendTime = pstTradeSell->StartTime;
					}
				}
			}
		}
	}

	for (i=0; (int)i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++)
	{
		pstTradeBuy = (TRADEBUY *)sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
		if (NULL == pstTradeBuy || !iValid )
		{
			continue;
		}

		if (pstTradeBuy->RoleID == pstPlayer->stRoleData.RoleID)
		{
			for (j=0; j<pstRoleTradeRecordInfo->TradeBuyNum; j++)
			{
				if (pstRoleTradeRecordInfo->TradeBuyID[j] == pstTradeBuy->TradeID)
				{
					break;
				}
			}

			if (j == pstRoleTradeRecordInfo->TradeBuyNum)
			{
				if (pstRoleTradeRecordInfo->TradeBuyNum < MAX_SIGLE_USER_TRADERECORD_NUM)
				{
					pstRoleTradeRecordInfo->TradeBuyID[pstRoleTradeRecordInfo->TradeBuyNum] = pstTradeBuy->TradeID;
					pstRoleTradeRecordInfo->TradeBuyNum++;

					if (pstTradeBuy->StartTime > pstRoleTradeRecordInfo->TradeSendTime)
					{
						pstRoleTradeRecordInfo->TradeSendTime = pstTradeBuy->StartTime;
					}
				}
			}
		}
	}


	pstShoppingQueryMyTradeRecordRes->RecordNum = 0;
	for( i=0; i< pstRoleTradeRecordInfo->TradeSellNum; i++ )
	{
		pstTradeSell = z_find_tradesell(pstEnv,pstRoleTradeRecordInfo->TradeSellID[i] );
		if( pstTradeSell == NULL )
		{
			stRoleTradeRecordInfoTmp.TradeSellID[stRoleTradeRecordInfoTmp.TradeSellNum] = pstRoleTradeRecordInfo->TradeSellID[i];
			stRoleTradeRecordInfoTmp.TradeSellNum++;		
			continue;
		}

		if (pstShoppingQueryMyTradeRecordRes->RecordNum >= MAX_TRADE_RECORD)
		{
			break;
		}

		tradesell_record(pstEnv, &pstShoppingQueryMyTradeRecordRes->TradeRecord[pstShoppingQueryMyTradeRecordRes->RecordNum], pstTradeSell);
		pstShoppingQueryMyTradeRecordRes->RecordNum++;
	}

	for( i=0; i< pstRoleTradeRecordInfo->TradeBuyNum; i++ )
	{
		pstTradeBuy = z_find_tradebuy(pstEnv,pstRoleTradeRecordInfo->TradeBuyID[i] );
		if( pstTradeBuy == NULL )
		{
			stRoleTradeRecordInfoTmp.TradeBuyID[stRoleTradeRecordInfoTmp.TradeBuyNum] = pstRoleTradeRecordInfo->TradeBuyID[i];
			stRoleTradeRecordInfoTmp.TradeBuyNum++;	
					
			continue;
		}

		if (pstShoppingQueryMyTradeRecordRes->RecordNum >= MAX_TRADE_RECORD)
		{
			break;
		}

		tradebuy_record(pstEnv, &pstShoppingQueryMyTradeRecordRes->TradeRecord[pstShoppingQueryMyTradeRecordRes->RecordNum], pstTradeBuy);
		pstShoppingQueryMyTradeRecordRes->RecordNum++;
	}
	
	for( i=0; i<stRoleTradeRecordInfoTmp.TradeSellNum; i++)
	{
		shopping_delete_user_tradecord(pstEnv,pstPlayer,0,stRoleTradeRecordInfoTmp.TradeSellID[i]);
	}
	for( i=0; i<stRoleTradeRecordInfoTmp.TradeBuyNum; i++)
	{
		shopping_delete_user_tradecord(pstEnv,pstPlayer,1,stRoleTradeRecordInfoTmp.TradeBuyID[i]);
	}	

	/* 下发数据包 */
	if( pstShoppingQueryMyTradeRecordReq != NULL )
	{
		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	}
	
	return 0;
}
int shopping_delete_user_tradecord(ZONESVRENV* pstEnv, Player *pstPlayer,char cType,unsigned  long long ullID)
{
	unsigned int i = 0;
	ROLETRADERECORDINFO *pstRoleTradeRecordInfo;
	UNUSED(pstEnv);
	pstRoleTradeRecordInfo = &pstPlayer->stRoleData.MiscInfo.RoleTradeRecordInfo;

	if( cType == 0 )
	{
		for( i=0; i< pstRoleTradeRecordInfo->TradeSellNum; i++ )
		{
			if( pstRoleTradeRecordInfo->TradeSellID[i] == ullID)
			{
				if (i < pstRoleTradeRecordInfo->TradeSellNum-1)
				{
					memmove( pstRoleTradeRecordInfo->TradeSellID+i,
					 			pstRoleTradeRecordInfo->TradeSellID+i+1,
					 			sizeof(pstRoleTradeRecordInfo->TradeSellID[0])*(pstRoleTradeRecordInfo->TradeSellNum-i-1));
				}
				pstRoleTradeRecordInfo->TradeSellNum--;
				break;
			}
		}
	}
	else if( cType == 1 )
	{
		for( i=0; i< pstRoleTradeRecordInfo->TradeBuyNum; i++ )
		{
			if( pstRoleTradeRecordInfo->TradeBuyID[i] == ullID)
			{
				if (i < pstRoleTradeRecordInfo->TradeBuyNum-1)
				{
					memmove( pstRoleTradeRecordInfo->TradeBuyID+i,
					 	pstRoleTradeRecordInfo->TradeBuyID+i+1,
					 	sizeof(pstRoleTradeRecordInfo->TradeBuyID[0])*(pstRoleTradeRecordInfo->TradeBuyNum-i-1));
				}
				pstRoleTradeRecordInfo->TradeBuyNum--;
				break;
			}
		}
	}
	
	return 0;
}
int shopping_tradeplayerinfo_init(Player *pstPlayer,TRADEPLAYERINFO *pstTradePlayerInfo)
{
	memset(pstTradePlayerInfo,0,sizeof(TRADEPLAYERINFO));
	if (pstPlayer)
	{
		pstTradePlayerInfo->Uin = pstPlayer->stRoleData.Uin;
		pstTradePlayerInfo->RoleID = pstPlayer->stRoleData.RoleID;
		STRNCPY(pstTradePlayerInfo->RoleName,pstPlayer->stRoleData.RoleName,CCH(pstTradePlayerInfo->RoleName));
		STRNCPY(pstTradePlayerInfo->Pass9Account,pstPlayer->szAccount,CCH(pstTradePlayerInfo->Pass9Account));
	}
	return 0;
}
int shopping_destroy_traderecord_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGDESTROYTRADERECORDREQ *pstShoppingDestroyTradeRecordReq)
{
	TRADESELL *pstTradeSell;
	TRADEBUY *pstTradeBuy;
	UNUSED(pstAppCtx);

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	/* 这个函数稍后*/
	if( pstShoppingDestroyTradeRecordReq->TradeType == 0 )
	{
		// 对出售记录的消除
		pstTradeSell = z_find_tradesell( pstEnv, pstShoppingDestroyTradeRecordReq->TradeID );
		if(pstTradeSell == NULL || pstTradeSell->RoleID != pstPlayer->stRoleData.RoleID)
		{
			return -1;
		}

		if (0>package_add_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstTradeSell->Gold))
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP22);
			return -1;
		}
		
		shopping_db_tradesell_delete_req( pstEnv,pstPlayer->stRoleData.Uin, pstTradeSell,OPERATE_BY_USER_DESTROY,NULL,0);
		z_delete_tradesell(pstEnv, pstTradeSell->TradeID);
	}
	else if(pstShoppingDestroyTradeRecordReq->TradeType == 1 )
	{
		TRADEPLAYERINFO stTradePlayerInfo;

		// 对求购记录的消除
		pstTradeBuy = z_find_tradebuy( pstEnv, pstShoppingDestroyTradeRecordReq->TradeID );
		if(pstTradeBuy == NULL || pstTradeBuy->RoleID != pstPlayer->stRoleData.RoleID)
		{
			return -1;
		}

		if (0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstTradeBuy->Moeny))
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP23);
			return -1;
		}
				
		shopping_tradeplayerinfo_init(pstPlayer,&stTradePlayerInfo);
		shopping_db_tradebuy_delete_req( pstEnv,pstPlayer->stRoleData.Uin, pstTradeBuy,OPERATE_BY_USER_DESTROY,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));	
		z_delete_tradebuy(pstEnv, pstTradeBuy->TradeID);
	}
	
	return 0;
}

int shopping_get_xml_file_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer)
{
	CSPKG stPkgRes;
	CSSHOPPINGGETXMLFILERES *pstShoppingGetXmlFileRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingGetXmlFileRes;
	UNUSED(pstAppCtx);

	memset(pstShoppingGetXmlFileRes,0,sizeof(CSSHOPPINGGETXMLFILERES));
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_GET_XML_FILE_RES;
	*pstShoppingGetXmlFileRes = g_stShoppingGetXmlFileRes;
	
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

	return 0;
}
int shopping_get_xml_file_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGGETXMLFILEREQ *pstShoppingGetXmlFileReq)
{
	/* 这个函数稍后*/
	UNUSED(pstAppCtx);
	UNUSED(pstShoppingGetXmlFileReq);
	
	/*if(pstShoppingGetXmlFileReq->Version == pstEnv->pstConf->ShoppingVersion )
	{
		// 已经是最新的不下发
		return -1;
	}*/

	shopping_get_xml_file_res(pstAppCtx,pstEnv,pstPlayer);
	
	return 0;
}

int shopping_query_traderecord_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,
	CSSHOPPINGQUERYRECORDBYIDREQ *pstShoppingQueryRecordByIDReq,int OperType)
{
	TRADESELL *pstTradeSell;
	TRADEBUY *pstTradeBuy;
	CSPKG stPkgRes;
	CSSHOPPINGQUERYRECORDBYIDRES *pstShoppingQueryRecordByIDRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryRecordByIDRes;
	UNUSED(pstAppCtx);

	memset(pstShoppingQueryRecordByIDRes,0,sizeof(CSSHOPPINGQUERYRECORDBYIDRES));
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_RECORD_BY_ID_RES;
		
	/* 这个函数稍后*/
	pstShoppingQueryRecordByIDRes->OperType = OperType;
	if( pstShoppingQueryRecordByIDReq->TradeType == 0 )
	{
		// 对出售记录的消除
		pstTradeSell = z_find_tradesell( pstEnv, pstShoppingQueryRecordByIDReq->TradeID );
		if(pstTradeSell == NULL )
		{
			pstShoppingQueryRecordByIDRes->Result = 0;
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_SHOP24);
			pstShoppingQueryRecordByIDRes->OperType = 2; //叫客户端删除记录
			pstShoppingQueryRecordByIDRes->TradeRecord.TradeID=pstShoppingQueryRecordByIDReq->TradeID;
			pstShoppingQueryRecordByIDRes->TradeRecord.TradeType = pstShoppingQueryRecordByIDReq->TradeType;
		}
		else
		{
			pstShoppingQueryRecordByIDRes->Result = 0;

			tradesell_record(pstEnv, &pstShoppingQueryRecordByIDRes->TradeRecord, pstTradeSell);
		}
	}
	else if(pstShoppingQueryRecordByIDReq->TradeType == 1 )
	{
		// 对求购记录的消除
		pstTradeBuy = z_find_tradebuy( pstEnv, pstShoppingQueryRecordByIDReq->TradeID );
		if(pstTradeBuy == NULL )
		{
			pstShoppingQueryRecordByIDRes->Result = 0;
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_MIDDLE, ZONEERR_SHOP24);
			pstShoppingQueryRecordByIDRes->OperType = 2; //叫客户端删除记录
			pstShoppingQueryRecordByIDRes->TradeRecord.TradeID=pstShoppingQueryRecordByIDReq->TradeID;
			pstShoppingQueryRecordByIDRes->TradeRecord.TradeType = pstShoppingQueryRecordByIDReq->TradeType;	
		}
		else 
		{
			pstShoppingQueryRecordByIDRes->Result = 0;
			tradebuy_record(pstEnv, &pstShoppingQueryRecordByIDRes->TradeRecord, pstTradeBuy);
		}
		
	}

	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	return 0;
}

int shopping_open_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer,CSSHOPPINGOPENREQ *pstShoppingOpenReq)
{
	UNUSED(pstShoppingOpenReq);
	
	shopping_fresh_shoplimitdef_req(pstAppCtx,pstEnv, pstPlayer);

	return 0;
}
int player_shopping_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, 
						TFRAMEHEAD *pstFrameHead, CSPKG *pstCsPkg)
{
	CSSHOPPINGREQ *pstShoppingReq = &pstCsPkg->Body.ShoppingClt;
	UNUSED(pstFrameHead);


	if(pstEnv->iInstMode == INST_MODE_BATTLE_SVR)
	{
		if( pstShoppingReq->Type == SHOPPING_BUY_REQ ||
			pstShoppingReq->Type == SHOPPING_OPEN_REQ ||
			pstShoppingReq->Type == SHOPPING_QUERY_TIANYUAN_REQ)
		{
			if (pstShoppingReq->Type == SHOPPING_BUY_REQ && pstShoppingReq->ShoppingReqData.ShppingBuyReq.GiftFlag)
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
				return -1;
			}
			else
			{
				battle_proxy_request(pstEnv, pstPlayer, pstCsPkg);
				return 0;
			}
		}
		else
		{
			z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			return -1;
		}
	}
	
	
	switch(pstShoppingReq->Type)
	{
		case SHOPPING_REQ_RES_ALL:
			break;
		case SHOPPING_BUY_REQ:
			shopping_buy_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShppingBuyReq);
			break;
		case SHOPPING_FRESH_REQ:
			 // 放流量控制的地方去了
			//shopping_fresh_shopdef_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingFreshRecordReq);
			break;
		case SHOPPING_QUERY_TIANYUAN_REQ:
			shopping_query_dianquan_req(pstAppCtx, pstEnv, pstPlayer, DATATYPE_POINT_BY_SHOPPING,
				&pstShoppingReq->ShoppingReqData.ShoppingQueryTianYuanReq,sizeof(CSSHOPPINGQUERYTIANYUANREQ) );
			break;
		case SHOPPING_PACKAGE_PRESENT_REQ:
			/*
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_package_present_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingPackagePresentReq,1);
			}
			else
			{
				z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "跨服不允许该操作");
			}*/
			break;
		case SHOPPING_QUERY_TRADESELL_REQ:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_query_tradesell_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingQueryTradeSellReq);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			}
			break;
		case SHOPPING_QUERY_TRADEBUY_REQ:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_query_tradebuy_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingQueryTradeBuyReq);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			}
			break;
		case SHOPPING_TRADE_REQ:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_trade_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingTradeReq);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			}
			break;
		case SHOPPING_SEND_TRADERECORD_REQ:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_send_traderecord_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingSendTradeRecordReq);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			}
			break;
		case SHOPPING_QUERY_MY_TRADERECORD_REQ:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_query_my_traderecord_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingQueryMyTradeRecordReq);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			}
			break;
		case SHOPPING_DESTROY_TRADERECORD_REQ:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_destroy_traderecord_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingDestroyTradeRecordReq);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			}
			break;
		case SHOPPING_GET_XML_FILE_REQ:
			shopping_get_xml_file_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingGetXmlFileReq);
			break;
		case SHOPPING_QUERY_RECORD_BY_ID_REQ:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_query_traderecord_req(pstAppCtx, pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingQueryRecordByIDReq,0);
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			}
			break;
		case SHOPPING_OPEN_REQ:
			//shopping_open_req(pstAppCtx,pstEnv, pstPlayer, &pstShoppingReq->ShoppingReqData.ShoppingOpenReq);
			break;
		case SHOPPING_QUERY_PROPSDEF_REQ:
			//shopping_query_propsdef_req(pstAppCtx, pstEnv,pstPlayer,&pstShoppingReq->ShoppingReqData.ShoppingQueryPropsDefReq);
			break;
		case SHOPPING_QUERY_PROPSGIFT_REQ:
			//shopping_query_propsgift_req(pstAppCtx, pstEnv,pstPlayer,&pstShoppingReq->ShoppingReqData.ShoppingQueryPropsGiftReq );
			break;
		case SHOPPING_SET_PROPSGIFT_REQ:
			//shopping_set_propsgift_req(pstAppCtx, pstEnv,pstPlayer,&pstShoppingReq->ShoppingReqData.ShoppingSetPropsGiftReq );
			break;			
		case SHOPPING_EXCHANGE_POINT_REQ:
			if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
			{
				shopping_exchange_point_req(pstAppCtx, pstEnv,pstPlayer,&pstShoppingReq->ShoppingReqData.ShoppingExchangeReq );
			}
			else
			{
				z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_BATTLE_SVR_OP);
			}
			break;
		default:
			break;
	}
	
	return 0; 
			
}

void shopping_db_init_torm_head(	TAPPCTX *pstAppCtx, 
	                      					ZONESVRENV *pstEnv,
	                      					int Uin,
                              				  	TORMPKGHEAD * pstHead,
                              				  	int iCmd, 
                              				  	int iCallBackCmd,
	                       					const char *pszMetaName, 
	                      					const char * pstCallBackData, 
	                      					int iContextLen )
{
	UNUSED(pstEnv);
	
	pstHead->HeadComm.InstID = pstAppCtx->iId;
	pstHead->HeadComm.Magic = TORM_MAGIC;
       pstHead->HeadCmd.Cmd = iCmd;
	
	if (pszMetaName)
	{
	       //如果有元的名字 把元的名字赋值
		STRNCPY(pstHead->HeadCmd.MetaNameVer.MetaName, pszMetaName, 
			      sizeof(pstHead->HeadCmd.MetaNameVer.MetaName));
	}
	pstHead->HeadCmd.MetaNameVer.MetaVer = 0;
	
	pstHead->HeadApp.AppCmd = 0;
	pstHead->HeadApp.Seq = 0;
	pstHead->HeadApp.ObjID = Uin;

	//把长度和内容都记录下
	pstHead->AsynCallBackDataLen = 0;
	memcpy(pstHead->AsynCallBackData +pstHead->AsynCallBackDataLen,&iCallBackCmd, sizeof(iCallBackCmd));
	pstHead->AsynCallBackDataLen += sizeof(iCallBackCmd);
	if(pstCallBackData != NULL)
	{
		if(iContextLen > 0)
		{
			memcpy(pstHead->AsynCallBackData +pstHead->AsynCallBackDataLen,pstCallBackData, iContextLen);
			pstHead->AsynCallBackDataLen +=iContextLen;
		}
	}
}
static int shopping_db_torm_send(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,TORMPKG *pstPkg,int *piDst,int DBProtoVer)
{
	TDRDATA stData;
	int iSrc = 0;

	stData.pszBuff = pstEnv->szSendBuffer;
	stData.iBuff = sizeof(pstEnv->szSendBuffer);
	if(0 != torm_pack_msg(pstEnv->hTorm, &stData, pstPkg, DBProtoVer))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_pack_msg error");
		return -1;
	}
	
	iSrc = pstAppCtx->iId;
	if(tbus_forward(pstAppCtx->iBus, &iSrc, piDst, stData.pszBuff, stData.iBuff, 0) < 0)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0,"tbus_forward auth_svr fail.");
	}
	
	return 0;
}
int shopping_db_send_req(TAPPCTX *pstAppCtx, ZONESVRENV *pstEnv,
	                     TORMPKG *pstTormPkg, const void *pvData, int iDataLen,int *piDst,int DBProtoVer)
{
	int iRet;
	TDRDATA stNetData;
	TDRDATA stMetaData;
	
	stNetData.pszBuff = pstEnv->szSendBuffer;
	stNetData.iBuff = sizeof(pstEnv->szSendBuffer);
	stMetaData.pszBuff = (char *)pvData;
	stMetaData.iBuff = iDataLen;

	iRet = torm_make_req_msg(pstEnv->hTorm, &stNetData, pstTormPkg, &stMetaData, 
		                        DBProtoVer);
	if(0 != iRet)
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "torm_make_req_msg fail:%s", torm_error_string(iRet));
		return -1;
	}
	
	if (0 > shopping_db_torm_send(pstAppCtx, pstEnv,pstTormPkg,piDst,DBProtoVer))
	{
		return -1;
	}

	return 0;	
}

int shopping_db_select_role_req(ZONESVRENV *pstEnv, int Uin, unsigned long long  GiftToRoleID,
								void *pData,unsigned int uiLen,int iCallCase)
									
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	ROLELISTINFO stRoleListInfo;
	char szBuf[1024];
	
	memset(szBuf,0,sizeof(szBuf));
	memset(&stRoleListInfo,0,sizeof(stRoleListInfo));
	
	if(uiLen + sizeof(int) > 1024 )
	{
		return -1;
	}
	memcpy(szBuf,&iCallCase,sizeof(int));
	memcpy(szBuf+sizeof(int),pData,uiLen);
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,Uin,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_PLAYER_ROLE_LIST, 
			               			"RoleListInfo", 
			               			(char *)szBuf, 
			               			uiLen + sizeof(int));/* update 修改点1*/
	
	pstSelectReq->OffSet = 0;
	pstSelectReq->Limit = ROLE_MAX_NUM;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	sprintf(pstSelectReq->OperInfo.WhereDef, "where RoleID=%llu", GiftToRoleID);

		
	iDst = pstEnv->iRoleDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stRoleListInfo, 
				sizeof(ROLELISTINFO),&iDst,pstEnv->iRoleDBProtoVer);

}
int shopping_db_select_role_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer, TORMPKG *pstPkg)
{
	int i;
	ROLELISTINFO stRoleListInfo;
	TORMSELECTRES *pstSelectRes;
       CSSHOPPINGBUYREQ *pstShoppingBuyReq  = NULL;
	SHOPDEF *pstShopDef = NULL;
	int iFind = 0;
	int iCallCase = 0;
	int iOffset = 0;
	
	pstSelectRes = &pstPkg->Body.SelectRes;

	//cmd的偏移
	iOffset += sizeof(int);
	memcpy(&iCallCase,pstPkg->Head.AsynCallBackData+iOffset,sizeof(int));
	iOffset += sizeof(int);
	if( iCallCase == 1 && pstPlayer)
	{
		pstShoppingBuyReq = (CSSHOPPINGBUYREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		iOffset += sizeof(CSSHOPPINGBUYREQ);

		if( !pstPlayer ||  pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN  || 
			pstPlayer->stRoleData.RoleID != pstShoppingBuyReq->RoleID )
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP6);
			return -1;
		}


		pstShopDef = z_find_shopdef(pstEnv, pstShoppingBuyReq->ShoppingID);
		if(pstShopDef == NULL)
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP6);
			return -1;
		}
		
		for(i=0; i<pstSelectRes->SelectResult.ResultNum && i<ROLE_MAX_NUM; i++)
		{
			if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleListInfo, sizeof(ROLELISTINFO)))
			{
				continue;
			}

			if( stRoleListInfo.RoleID == pstShoppingBuyReq->GiftToRoleID &&
				stRoleListInfo.DelTime == 0)
			{
				iFind = 1;
				break;
			}
		}

		if( !iFind )
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, 
								SYS_TIP, ZONEERR_SHOP25, pstShoppingBuyReq->GiftToPlayerName);
			return -1;
		}

		if ( 0 > shopping_buy_check(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq,
						pstShopDef,&stRoleListInfo) )
		{
			return -1;
		}


		if( 1 == pstShopDef->BuyType )
		{
			if( 0 > shopping_by_godcoin(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq, pstShopDef) )
			{
				return -1;
			}
		}
		else if( 2 == pstShopDef->BuyType )
		{
			/* 银币购买 */
			if( 0 > shopping_by_bindgodcoin(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq, pstShopDef) )
			{
				return -1;
			}
		}
	}
	else if(iCallCase == 2 && pstPlayer)
	{
		/* 商品背包赠送的时候的调用 */
		CSSHOPPINGPACKAGEPRESENTREQ *pstShoppingPackagePresentReq = (CSSHOPPINGPACKAGEPRESENTREQ *)(pstPkg->Head.AsynCallBackData+iOffset);
		for(i=0; i<pstSelectRes->SelectResult.ResultNum && i<ROLE_MAX_NUM; i++)
		{
			if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stRoleListInfo, sizeof(ROLELISTINFO)))
			{
				continue;
			}

			if( stRoleListInfo.RoleID == pstShoppingPackagePresentReq->GiftToRoleID &&
				stRoleListInfo.DelTime == 0)
			{
				iFind = 1;
				break;
			}
		}

		if( !iFind )
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, 
							SYS_TIP, ZONEERR_SHOP25, pstShoppingPackagePresentReq->GiftToPlayerName);
			return -1;
		}	
		shopping_package_present_req(pstAppCtx, pstEnv, pstPlayer,pstShoppingPackagePresentReq,0);
	}
		
	return 0;
}


int shopping_db_tradesell_count_req(ZONESVRENV *pstEnv)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	TRADESELL stTradeSell;

	memset(&stTradeSell,0,sizeof(stTradeSell));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_TRADE_SELL_RES, 
			               			"TradeSell", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectCountReq->WhereDef[0] = 0;
		
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stTradeSell, 
				sizeof(TRADESELL),&iDst,pstEnv->iRoleDBProtoVer);

}
int shopping_db_shopdef_count_req(ZONESVRENV *pstEnv)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	SHOPDEF stShopDef;

	memset(&stShopDef,0,sizeof(stShopDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_SHOPDEF_RES, 
			               			"ShopDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	strcpy(pstSelectCountReq->WhereDef, "where ShopID != 0");

	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		iDst = pstEnv->iRegionDBID;
	}
	else
	{
		iDst = pstEnv->iMiscDBID;
	}
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stShopDef, 
				sizeof(SHOPDEF),&iDst,pstEnv->iRoleDBProtoVer);

}

int shopping_db_shopmsgdef_count_req(ZONESVRENV *pstEnv)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	SHOPMSGDEF stShopMsgDef;

	memset(&stShopMsgDef,0,sizeof(stShopMsgDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_SHOPMSGDEF_RES, 
			               			"ShopMsgDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectCountReq->WhereDef[0] = 0;
	snprintf(pstSelectCountReq->WhereDef,
	         sizeof(pstSelectCountReq->WhereDef),
	         "where seqID != 0 order by seqID desc limit 1");
		
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		iDst = pstEnv->iRegionDBID;
	}
	else
	{
		iDst = pstEnv->iMiscDBID;
	}
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stShopMsgDef, 
				sizeof(SHOPMSGDEF),&iDst,pstEnv->iRoleDBProtoVer);

}
int shopping_db_shoplimitdef_count_req(ZONESVRENV *pstEnv)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	SHOPLIMITDEF stShopLimitDef;

	memset(&stShopLimitDef,0,sizeof(stShopLimitDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_SHOPLIMITDEF_RES, 
			               			"ShopLimitDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectCountReq->WhereDef[0] = 0;
	snprintf(pstSelectCountReq->WhereDef,
	         sizeof(pstSelectCountReq->WhereDef),
	         "where ShopID != 0");
		
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		iDst = pstEnv->iRegionDBID;
	}
	else
	{
		iDst = pstEnv->iMiscDBID;
	}
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stShopLimitDef, 
				sizeof(SHOPLIMITDEF),&iDst,pstEnv->iRoleDBProtoVer);

}
int shopping_db_shoplimitdef_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{
	TORMSELECTCOUNTRES *pstSelectCountRes;
	int iRecordNum = 0;
	unsigned int uiBeginRecord = 0;
	UNUSED(pstAppCtx);
	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo,torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	
	iRecordNum = pstSelectCountRes->RowsCount;

	g_SelectCountShopLimitNum = iRecordNum;
	if (0 == iRecordNum)
	{
		if (iRecordNum != pstEnv->pstShopLimitDefHash->iItem)
		{
			shopping_db_shoplimitdef_select_req( pstEnv, uiBeginRecord);
		}
	}
	
	while ( iRecordNum > 0 )
	{
		/* 开始取数据了 */
		shopping_db_shoplimitdef_select_req( pstEnv, uiBeginRecord);
		
		uiBeginRecord +=  PRE_GET_DB_RECORD;
		iRecordNum -= 	PRE_GET_DB_RECORD;
	}
	
	return 0;
}
int shopping_db_shoplimitdef_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	SHOPLIMITDEF stShopLimitDef;

	memset(&stShopLimitDef,0,sizeof(stShopLimitDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_SHOPLIMITDEF_RES, 
			               			"ShopLimitDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectReq->OffSet = iBeginRecord;
	pstSelectReq->Limit = PRE_GET_DB_RECORD;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,
	         sizeof(pstSelectReq->OperInfo.WhereDef),
	         "where ShopID != 0");	
	
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		iDst = pstEnv->iRegionDBID;
	}
	else
	{
		iDst = pstEnv->iMiscDBID;
	}
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stShopLimitDef, 
			sizeof(SHOPLIMITDEF),&iDst,pstEnv->iRoleDBProtoVer);
	
}


int shopping_db_shoplimitdef_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{
	int i, j;
	TORMSELECTRES *pstSelectRes;
	SHOPLIMITDEF stShopLimitDef;
	int iRet = 0;
	SHOPLIMITDEF astShopLimitDef[MAX_SHOP_DEF_LINES];
	SHOPDEF *pstShopDef;
	int iModifyNum = 0;
	SHOPLIMITDEF *pstShopLimitDef;
	ShopLimitBucket *pstShopLimitBucket;
	ShopLimitBucket stShopLimitBucket;
	int iValid = 0;
	
	UNUSED(pstAppCtx);

	if (pstEnv->pstZoneObj->iShopGetStat == 0)
	{
		return -1;
	}
	
	pstSelectRes = &pstPkg->Body.SelectRes;
		
	for(i=0; i<pstSelectRes->SelectResult.ResultNum ; i++)
	{
		memset(&stShopLimitDef,0,sizeof(stShopLimitDef));
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stShopLimitDef, sizeof(SHOPLIMITDEF)))
		{
			continue;
		}
		/*数据都先弄出来,等扫描cache的时候我们再来做删除操作,清晰点*/

		pstShopDef = z_hash_shopdef(pstEnv, stShopLimitDef.ShopID);
		if (NULL == pstShopDef || ((strlen(pstShopDef->HideDate)>0 &&
		      pstAppCtx->stCurr.tv_sec > string2time(pstShopDef->HideDate)) ))
		{
			continue;
		}

		g_astShopLimitDefTmp[g_iShopLimitDefTmp++] = stShopLimitDef.ShopID;

		iRet =   hash_insert_shoplimitdef(pstEnv,stShopLimitDef.ShopID,(char*)&stShopLimitDef,sizeof(SHOPLIMITDEF));
		if( 1 == iRet )
		{
			// 有修改的需要增量下发, 先记录下来，等下再发出去
			g_astShopLimitDef[g_iModifyLimitNum] = stShopLimitDef.ShopID;
			g_iModifyLimitNum++;
		}
	}
	g_LoadAllShopLimitNum += pstSelectRes->SelectResult.ResultNum;

	if( g_LoadAllShopLimitNum >= g_SelectCountShopLimitNum)
	{
		//调用修改的下发 
		memset(astShopLimitDef,0,sizeof(astShopLimitDef));
		iModifyNum = 0;
		for(i=0; i<g_iModifyLimitNum && i<MAX_SHOP_DEF_LINES; i++ )
		{
			pstShopLimitDef = z_hash_shoplimitdef(pstEnv, g_astShopLimitDef[i]);
			if( pstShopLimitDef != NULL )
			{
				memcpy(&astShopLimitDef[iModifyNum],	pstShopLimitDef,sizeof(SHOPLIMITDEF));
				iModifyNum++;
			}
			
		}
		if( iModifyNum > 0 )
		{
			shopping_modify_limit_res( pstAppCtx,pstEnv,NULL,astShopLimitDef, iModifyNum);
		}

		//把删除的商品限制清除掉
		iModifyNum = 0;
		for (i=0; i<pstEnv->pstShopLimitDefHash->iMax; i++)
		{
			pstShopLimitBucket = (ShopLimitBucket *)sht_pos(pstEnv->pstShopLimitDefHash, i, &iValid);
			if (NULL == pstShopLimitBucket || !iValid )
			{
				continue;
			}

			pstShopLimitDef = (SHOPLIMITDEF *)pstShopLimitBucket->szData;

			iValid = 0;
			for (j=0; j<g_iShopLimitDefTmp; j++)
			{
				if (pstShopLimitDef->ShopID == g_astShopLimitDefTmp[j])
				{
					iValid = 1;
					break;
				}
			}

			if (!iValid)
			{
				memcpy(&astShopLimitDef[iModifyNum],	pstShopLimitDef,sizeof(SHOPLIMITDEF));
				iModifyNum++;
			}
		}

		for (i=0; i<iModifyNum; i++)
		{
			stShopLimitBucket.uiKey = astShopLimitDef[i].ShopID;
			sht_remove(pstEnv->pstShopLimitDefHash, &stShopLimitBucket, hash_cmp_int, hash_code_int);
		}

		if( iModifyNum > 0 )
		{
			shopping_limitdel_res( pstAppCtx,pstEnv,astShopLimitDef, iModifyNum);
		}
	}

	return 0;
}
int shopping_modify_limit_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,Player *pstPlayer,SHOPLIMITDEF *pstShopLimitDef, int iCount)
{
	int i = 0;
	CSPKG stPkgRes;
	CSSHOPPINGMODIFYLIMITRECORDRES *pstShoppingModifyLimitRecord = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingModifyLimitRecordRes;
	UNUSED(pstAppCtx);
	
	memset(pstShoppingModifyLimitRecord,0,sizeof(CSSHOPPINGMODIFYLIMITRECORDRES));
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_MODIFY_LIMIT_RES;
	for( i=0; i<iCount; i++)
	{
		memcpy(&pstShoppingModifyLimitRecord->ShopLimitRecord[pstShoppingModifyLimitRecord->RecordNum], pstShopLimitDef+i,sizeof(SHOPLIMITDEF));
		pstShoppingModifyLimitRecord->RecordNum += 1;
		if(pstShoppingModifyLimitRecord->RecordNum %MAX_PKG_RECORD == 0)
		{
			/* 广播数据包 */
			Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
			z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);
			
			pstShoppingModifyLimitRecord->RecordNum = 0;
		}
	}

	if( pstShoppingModifyLimitRecord->RecordNum  > 0)
	{
		/* 广播数据包 */
		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		if(pstPlayer)
		{
			 z_cltmsg_send(pstEnv, pstPlayer,  &stPkgRes);
		}
		else
		{
			z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);
		}
	}	
	return 0;
}

int shopping_db_shopdef_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{	
	TORMSELECTCOUNTRES *pstSelectCountRes;
	int iRecordNum = 0;
	unsigned int uiBeginRecord = 0;
	UNUSED(pstAppCtx);
	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo,torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	
	iRecordNum = pstSelectCountRes->RowsCount;

	g_SelectCountShopNum = iRecordNum;
	while ( iRecordNum > 0 )
	{
		/* 开始取数据了 */
		shopping_db_shopdef_select_req( pstEnv, uiBeginRecord);
		
		uiBeginRecord +=  PRE_GET_DB_RECORD;
		iRecordNum -= 	PRE_GET_DB_RECORD;
	}
	
	return 0;
}
int shopping_db_shopmsgdef_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{	
	TORMSELECTCOUNTRES *pstSelectCountRes;
	int iRecordNum = 0;
	unsigned int uiBeginRecord = 0;
	UNUSED(pstAppCtx);

	pstEnv->pstZoneObj->iShopGetStat = 1;
	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo,torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	
	iRecordNum = pstSelectCountRes->RowsCount;


	while ( iRecordNum > 0 )
	{
		/* 开始取数据了 */
		shopping_db_shopmsgdef_select_req( pstEnv, uiBeginRecord);
		
		uiBeginRecord +=  PRE_GET_DB_RECORD;
		iRecordNum -= 	PRE_GET_DB_RECORD;
	}
	
	return 0;
}
int shopping_db_shopdef_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	SHOPDEF stShopDef;

	memset(&stShopDef,0,sizeof(stShopDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_SHOPDEF_RES, 
			               			"ShopDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectReq->OffSet = iBeginRecord;
	pstSelectReq->Limit = PRE_GET_DB_RECORD;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	strcpy(pstSelectReq->OperInfo.WhereDef, "where ShopID != 0");
	
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		iDst = pstEnv->iRegionDBID;
	}
	else
	{
		iDst = pstEnv->iMiscDBID;
	}
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stShopDef, 
			sizeof(SHOPDEF),&iDst,pstEnv->iRoleDBProtoVer);
	
}
int shopping_db_shopmsgdef_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	SHOPMSGDEF stShopMsgDef;

	memset(&stShopMsgDef,0,sizeof(stShopMsgDef));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_SHOPMSGDEF_RES, 
			               			"ShopMsgDef", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectReq->OffSet = iBeginRecord;
	pstSelectReq->Limit = PRE_GET_DB_RECORD;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,
	         sizeof(pstSelectReq->OperInfo.WhereDef),
	         "where seqID != 0 order by seqID desc limit 1");	
	
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		iDst = pstEnv->iRegionDBID;
	}
	else
	{
		iDst = pstEnv->iMiscDBID;
	}
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stShopMsgDef, 
			sizeof(SHOPMSGDEF),&iDst,pstEnv->iRoleDBProtoVer);
	
}
int shopping_db_shopdef_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{
	int i, j, iValid;
	TORMSELECTRES *pstSelectRes;
	SHOPDEF stShopDef;
	int iRet = 0;
	SHOPDEF astShopDef[MAX_SHOP_DEF_LINES];
	int iModifyNum = 0;
	SHOPDEF *pstShopDef;
	ShopBucket *pstShopBucket;
	ShopBucket stShopBucket;
	UNUSED(pstAppCtx);

	if (pstEnv->pstZoneObj->iShopGetStat == 0)
	{
		return -1;
	}
	
	pstSelectRes = &pstPkg->Body.SelectRes;
		
	for(i=0; i<pstSelectRes->SelectResult.ResultNum ; i++)
	{
		memset(&stShopDef,0,sizeof(stShopDef));
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stShopDef, sizeof(SHOPDEF)))
		{
			continue;
		}
		pstShopDef = &stShopDef;
		
		/*数据都先弄出来,等扫描cache的时候我们再来做删除操作,清晰点*/
		if(strlen(pstShopDef->HideDate)>0 &&
		      pstAppCtx->stCurr.tv_sec > string2time(pstShopDef->HideDate) )
		{
			continue;
		}

		iValid = 1;
		for (j=0; j<SHOP_ITEM_COUNT; j++)
		{
			if (0 == pstShopDef->Items[j].ItemID)
			{
				break;
			}

			if (0 == pstShopDef->Items[j].ItemNum || NULL == z_find_itemdef( pstEnv, pstShopDef->Items[j].ItemID))
			{
				iValid = 0;
				break;
			}
		}

		g_astShopDefTmp[g_iShopDefTmp++] = stShopDef.ShopID;

		if (!iValid)
		{
			continue;
		}

		iRet =   hash_insert_shopdef(pstEnv,stShopDef.ShopID,(char*)&stShopDef,sizeof(SHOPDEF));
		if( 1 == iRet )
		{
			// 有修改的需要增量下发, 先记录下来，等下再发出去
			g_astShopDef[g_iModifyNum] = stShopDef.ShopID;
			g_iModifyNum++;
		}
	}
	g_LoadAllShopNum += pstSelectRes->SelectResult.ResultNum;

	if( g_LoadAllShopNum >= g_SelectCountShopNum)
	{
		shopping_db_shoplimitdef_count_req(pstEnv);
			
		//调用修改的下发 
		memset(astShopDef,0,sizeof(astShopDef));
		iModifyNum = 0;
		for(i=0; i<g_iModifyNum && iModifyNum<MAX_SHOP_DEF_LINES; i++ )
		{
			pstShopDef = z_find_shopdef(pstEnv, g_astShopDef[i]);
			if( pstShopDef != NULL )
			{
				memcpy(&astShopDef[iModifyNum],	pstShopDef,sizeof(SHOPDEF));
				iModifyNum++;
			}
		}

		if( iModifyNum > 0 )
		{
			shopping_modify_res( pstAppCtx,pstEnv,astShopDef, iModifyNum);
		}

		//把删除的商品清除掉
		iModifyNum = 0;
		for (i=0; i<pstEnv->pstShopHash->iMax; i++)
		{
			pstShopBucket = (ShopBucket *)sht_pos(pstEnv->pstShopHash, i, &iValid);
			if (NULL == pstShopBucket || !iValid )
			{
				continue;
			}

			pstShopDef = (SHOPDEF *)pstShopBucket->szData;

			iValid = 0;
			for (j=0; j<g_iShopDefTmp; j++)
			{
				if (pstShopDef->ShopID == g_astShopDefTmp[j])
				{
					iValid = 1;
					break;
				}
			}

			if (!iValid)
			{
				memcpy(&astShopDef[iModifyNum],	pstShopDef,sizeof(SHOPDEF));
				//一定下架
				strcpy(astShopDef[iModifyNum].HideDate, "20100810120000");
				iModifyNum++;
			}
		}

		for (i=0; i<iModifyNum; i++)
		{
			stShopBucket.uiKey = astShopDef[i].ShopID;
			sht_remove(pstEnv->pstShopHash, &stShopBucket, hash_cmp_int, hash_code_int);
		}

		if( iModifyNum > 0 )
		{
			shopping_del_res( pstAppCtx,pstEnv,astShopDef, iModifyNum);
		}
	}

	return 0;
}

int shopping_db_shoplimitdef_update_req( ZONESVRENV* pstEnv,SHOPLIMITDEF *pstShopLimitDef)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMUPDATEREQ *pstUpdateReq =  &pstPkg->Body.UpdateReq;
	char szCallBackBuf[1024];
	int  iOffset = 0;
	SHOPLIMITMODIFYDEF stShopLimitModifyDef;
	
	memset(&stShopLimitModifyDef,0,sizeof(stShopLimitModifyDef));
	memset(szCallBackBuf,0,sizeof(szCallBackBuf));
	memcpy(szCallBackBuf,pstShopLimitDef,sizeof(SHOPLIMITDEF));
	iOffset += sizeof(SHOPDEF);

	stShopLimitModifyDef.FreshTime = pstShopLimitDef->FreshTime;
	stShopLimitModifyDef.LimitSelledNum = pstShopLimitDef->LimitSelledNum;
	stShopLimitModifyDef.ModifyFlag = pstShopLimitDef->ModifyFlag;
	stShopLimitModifyDef.ShopID = pstShopLimitDef->ShopID;
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_UPDATE_REQ,DB_UPDATE_SHOPLIMITDEF_RES, 
			               			"ShopLimitModifyDef", 
			               			(char *)szCallBackBuf, 
			               			iOffset);/* update 修改点1*/
	
	pstUpdateReq->DoInsert = 0; //update失败不做插入处理
	pstUpdateReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstUpdateReq->OperInfo.EntryNameList.EntryNameNum = 0;
	snprintf(pstUpdateReq->OperInfo.WhereDef,
	         sizeof(pstUpdateReq->OperInfo.WhereDef),
	         "where ShopID = %u limit 1",
		       stShopLimitModifyDef.ShopID);
	
	
	if(INST_MODE_BATTLE_SVR == pstEnv->iInstMode )
	{
		iDst = pstEnv->iRegionDBID;
	}
	else
	{
		iDst = pstEnv->iMiscDBID;
	}
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stShopLimitModifyDef, 
			sizeof(SHOPLIMITMODIFYDEF),&iDst,pstEnv->iRoleDBProtoVer);
	
}

int shopping_db_shoplimitdef_update_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,TORMPKG *pstPkg)
{
	TORMUPDATERES *pstUpdateRes;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);

	pstUpdateRes = &pstPkg->Body.UpdateRes;	
	if (TORM_ERR_IS_ERROR(pstUpdateRes->ErrNo))
	{	
		tlog_error(pstEnv->pstLogCat, 0, 0, "error !!! shopdef update fail");
		return -1;
	}
	else if(pstUpdateRes->AffectRows != 1 )
	{

		tlog_error(pstEnv->pstLogCat, 0, 0, "error !!! shopdef update mutil rows %d ",
						pstUpdateRes->AffectRows);
		return -1;
	}

	return 0;
}

int shopping_db_shopmsgdef_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{
	//int i;
	TORMSELECTRES *pstSelectRes;
	SHOPMSGDEF stShopMsgDef;
	UNUSED(pstAppCtx);

	if (pstEnv->pstZoneObj->iShopGetStat == 0)
	{
		return -1;
	}

	memset(&stShopMsgDef,0,sizeof(stShopMsgDef));
	pstSelectRes = &pstPkg->Body.SelectRes;

	if (0 == pstSelectRes->SelectResult.ResultNum)
	{
		return 0;
	}
	else if (1 == pstSelectRes->SelectResult.ResultNum)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stShopMsgDef, sizeof(SHOPMSGDEF)))
		{
			return -1;
		}

		if (memcmp(pstEnv->pstZoneObj->sShopMsgDef, &stShopMsgDef, sizeof(SHOPMSGDEF)) == 0)
		{
			return 0;
		}

		memcpy(pstEnv->pstZoneObj->sShopMsgDef, &stShopMsgDef, sizeof(SHOPMSGDEF));
	}
	else
	{
		return -1;
	}

	shopping_modify_msg_res(pstAppCtx,pstEnv,NULL,1);
		
	return 0;
}

int shopping_db_tradesell_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{	
	TORMSELECTCOUNTRES *pstSelectCountRes;
	UNUSED(pstAppCtx);
	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo,torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	
	if( pstSelectCountRes->RowsCount > MAX_TRADE_RECORD_NUM )
	{
		g_iTradeSellRecordNum = MAX_TRADE_RECORD_NUM;
	}
	else 
	{
		g_iTradeSellRecordNum = pstSelectCountRes->RowsCount;
	}

	if( pstEnv->pstConf->PreGetTradeRecordNum < 1 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "conf err PreGetTradeRecordNum: %d", 
					pstEnv->pstConf->PreGetTradeRecordNum );
		return -1;
	}
	
	//while ( g_iTradeSellRecordNum > 0 )
	//{
		/* 开始取数据了 */
		shopping_db_tradesell_select_req( pstEnv, g_iTradeSellBeginRecord);
		
		g_iTradeSellBeginRecord +=  pstEnv->pstConf->PreGetTradeRecordNum;
		g_iTradeSellRecordNum -= 	pstEnv->pstConf->PreGetTradeRecordNum;
	//}
	
	return 0;
}

int shopping_db_tradesell_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TRADESELL stTradeSell;

	memset(&stTradeSell,0,sizeof(stTradeSell));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_TRADE_SELL_RES, 
			               			"TradeSell", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectReq->OffSet = iBeginRecord;
	pstSelectReq->Limit = pstEnv->pstConf->PreGetTradeRecordNum;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,
	         sizeof(pstSelectReq->OperInfo.WhereDef),
	         "where TradeID != 0");	
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stTradeSell, 
			sizeof(TRADESELL),&iDst,pstEnv->iRoleDBProtoVer);
	
}
int shopping_db_tradesell_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{
	int i;
	TORMSELECTRES *pstSelectRes;
	TRADESELL stTradeSell;
	UNUSED(pstAppCtx);

	if (pstEnv->pstZoneObj->iShopGetStat == 0)
	{
		return -1;
	}

	memset(&stTradeSell,0,sizeof(stTradeSell));
	pstSelectRes = &pstPkg->Body.SelectRes;
		
	for(i=0; i<pstSelectRes->SelectResult.ResultNum ; i++)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stTradeSell, sizeof(TRADESELL)))
		{
			continue;
		}
		
		if( 0 != z_insert_tradesell(pstEnv, &stTradeSell) )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "insert cache tradesell error !");
		}
	}

	if( g_iTradeSellRecordNum > 0)
	{
		shopping_db_tradesell_select_req( pstEnv, g_iTradeSellBeginRecord);
		g_iTradeSellBeginRecord +=  pstEnv->pstConf->PreGetTradeRecordNum;
		g_iTradeSellRecordNum -= 	pstEnv->pstConf->PreGetTradeRecordNum;
	}
		
	return 0;
}


int shopping_db_tradesell_insert_req( ZONESVRENV* pstEnv,int iUin, TRADESELL *pstTradeSell)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMINSERTREQ *pstInsertReq =  &pstPkg->Body.InsertReq;

	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,iUin,&pstPkg->Head, 
								TORM_INSERT_REQ,DB_INSERT_TRADE_SELL_RES, 
			               			"TradeSell", 
			               			(char *)pstTradeSell, 
			               			sizeof(TRADESELL));/* update 修改点1*/
	
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,pstTradeSell, 
			sizeof(TRADESELL),&iDst,pstEnv->iRoleDBProtoVer);
	
}

int shopping_broadcast_tradesell(ZONESVRENV* pstEnv, TRADESELL *pstTradeSell)
{
	CSPKG stPkgRes;
	CSSHOPPINGQUERYTRADESELLRES *pstShoppingQueryTradeSellRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryTradeSellRes;

	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_TRADESELL_RES;
	pstShoppingQueryTradeSellRes->RecordNum = 0;
	//特殊含义，表示是增量
	pstShoppingQueryTradeSellRes->SvrCurrTotalNum = 0;

	tradesell_record(pstEnv, &pstShoppingQueryTradeSellRes->TradeRecord[pstShoppingQueryTradeSellRes->RecordNum], pstTradeSell);
	pstShoppingQueryTradeSellRes->RecordNum++;

	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);
	
	return 0;
}

int shopping_broadcast_tradebuy(ZONESVRENV* pstEnv, TRADEBUY *pstTradeBuy)
{
	CSPKG stPkgRes;
	CSSHOPPINGQUERYTRADEBUYRES *pstShoppingQueryTradeBuyRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryTradeBuyRes;

	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_TRADEBUY_RES;
	pstShoppingQueryTradeBuyRes->RecordNum = 0;
	//特殊含义，表示是增量
	pstShoppingQueryTradeBuyRes->SvrCurrTotalNum = 0;

	tradebuy_record(pstEnv, &pstShoppingQueryTradeBuyRes->TradeRecord[pstShoppingQueryTradeBuyRes->RecordNum], pstTradeBuy);
	pstShoppingQueryTradeBuyRes->RecordNum++;

	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);
	
	return 0;
}

int shopping_db_tradesell_insert_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg)
{
	TORMINSERTRES *pstInsertRes;
	DBAsyn *pstDBAsyn;
	TRADESELL *pstTradeSell;
	ROLETRADERECORDINFO *pstRoleTradeRecordInfo;
	OPTRADEINFO stOpTradeInfo;
	CSPKG stPkgRes;
	CSSHOPPINGSENDTRADERECORDRES *pstShoppingSendTradeRecordRes;
	
	pstInsertRes = &pstPkg->Body.InsertRes;
	pstDBAsyn = (DBAsyn *)pstPkg->Head.AsynCallBackData;
	pstTradeSell = (TRADESELL *)(pstDBAsyn->AsynCmd.szMaxBuf);

	memset(&stOpTradeInfo,0,sizeof(stOpTradeInfo));
	stOpTradeInfo.RoleInfo.RoleID = pstTradeSell->RoleID;
	STRNCPY(stOpTradeInfo.RoleInfo.RoleName,pstTradeSell->RoleName,CCH(stOpTradeInfo.RoleInfo.RoleName));
	STRNCPY(stOpTradeInfo.Account, pstTradeSell->Pass9Account,CCH(stOpTradeInfo.Account));
	stOpTradeInfo.OperType = 0;
	pstTradeSell->TradeID = stOpTradeInfo.TradeID = pstInsertRes->LastInsertID;
	stOpTradeInfo.Gold = pstTradeSell->Gold;
	stOpTradeInfo.Money = pstTradeSell->Moeny;

	if (pstTradeSell->Flag & TRADE_FLAG_SYS)
	{
		if (0 == TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
		{
			if (0 == z_insert_tradesell(pstEnv,pstTradeSell) )
			{
				snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"9999|发布系统金币出售单account:%s,rolename:%s %s: %u 换%s:%u",
				pstTradeSell->Pass9Account, pstTradeSell->RoleName,
				MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold,
				MONEY_TYPE_MONEY_NAME, pstTradeSell->Moeny);
				
				stOpTradeInfo.Flag = 9999;
				z_role_trade_oplog(pstEnv,&stOpTradeInfo);

				shopping_broadcast_tradesell(pstEnv, pstTradeSell);
			}
		}
		return 0;
	}

	if (TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		// 这里写条运营日志
		snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"3|发布金币出售单失败,邮件返还用户account:%s,rolename:%s %s: %u ",
				pstTradeSell->Pass9Account, 
				pstTradeSell->RoleName,
				MONEY_TYPE_GODCOIN_NAME,
				pstTradeSell->Gold);
		stOpTradeInfo.Flag = 3;
		z_role_trade_oplog(pstEnv,&stOpTradeInfo);

		tlog_error(pstEnv->pstLogCat, 0, 0, "3|发布金币出售单失败,邮件返还用户account:%s,rolename:%s %s: %u , err %s, dbno %d",
				pstTradeSell->Pass9Account, 
				pstTradeSell->RoleName,
				MONEY_TYPE_GODCOIN_NAME,
				pstTradeSell->Gold,
				torm_error_string(pstInsertRes->ErrNo),
				pstInsertRes->DBMSErrNo);

		if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP26);
		}

		player_sysmail_fast_send(pstEnv, pstTradeSell->RoleName, LJY_MAIL19, 
			LJY_MAIL20, NULL, 0,
			pstTradeSell->Gold, LJY_MAIL21, 1);
		return 0;
	}

	//全服通知
	shopping_broadcast_tradesell(pstEnv, pstTradeSell);

	if (!pstPlayer   ||
		pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
		pstPlayer->stRoleData.RoleID != pstTradeSell->RoleID)
	{
		snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"41|发布金币出售单时数据库成功但角色不在线无需补偿account:%s,rolename:%s %s: %u ",
				pstTradeSell->Pass9Account, 
				pstTradeSell->RoleName,
				MONEY_TYPE_GODCOIN_NAME,
				pstTradeSell->Gold);
		stOpTradeInfo.Flag = 41;
		z_role_trade_oplog(pstEnv,&stOpTradeInfo);
		
		tlog_error(pstEnv->pstLogCat, 0, 0, "41|数据库成功但角色不在线了无需补偿account:%s,rolename:%s %s: %u ",
				pstTradeSell->Pass9Account, 
				pstTradeSell->RoleName,
				MONEY_TYPE_GODCOIN_NAME,
				pstTradeSell->Gold);

		z_insert_tradesell(pstEnv,pstTradeSell);

		return -1;
	}
	else
	{
		pstRoleTradeRecordInfo = &pstPlayer->stRoleData.MiscInfo.RoleTradeRecordInfo;
		
		if( pstRoleTradeRecordInfo->TradeSellNum >= MAX_SIGLE_USER_TRADERECORD_NUM ||
			0 != z_insert_tradesell(pstEnv,pstTradeSell) )
		{
			// 这里写条运营日志
			z_rolemem_warp(pstPlayer, &stOpTradeInfo.RoleInfo);
			snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
					"4|发布金币出售单时数据库成功，但发布失败,无需补偿account:%s,rolename:%s %s: %u ",
					pstTradeSell->Pass9Account, 
					pstTradeSell->RoleName,
					MONEY_TYPE_GODCOIN_NAME,
					pstTradeSell->Gold);
			stOpTradeInfo.Flag = 4;
			z_role_trade_oplog(pstEnv,&stOpTradeInfo);

			tlog_error(pstEnv->pstLogCat, 0, 0, "4|数据库成功，但发布出售单失败,无需补偿account:%s,rolename:%s %s: %u ",
					pstTradeSell->Pass9Account, 
					pstTradeSell->RoleName,
					MONEY_TYPE_GODCOIN_NAME,
					pstTradeSell->Gold);

			return -1;
		}
		
		pstRoleTradeRecordInfo->TradeSendTime = pstAppCtx->stCurr.tv_sec;
		pstRoleTradeRecordInfo->TradeSellID[pstRoleTradeRecordInfo->TradeSellNum] = pstTradeSell->TradeID;
		pstRoleTradeRecordInfo->TradeSellNum++;
		
		pstShoppingSendTradeRecordRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingSendTradeRecordRes;
		memset(pstShoppingSendTradeRecordRes,0,sizeof(CSSHOPPINGSENDTRADERECORDRES));

		stPkgRes.Body.ShoppingSvr.Type = SHOPPING_SEND_TRADERECORD_RES;
		pstShoppingSendTradeRecordRes->Result = 0;
		
		tradesell_record(pstEnv, &pstShoppingSendTradeRecordRes->TradeRecord, pstTradeSell);
		
		// 下发数据包 
		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

		z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP27);

		// 这里写条运营日志
		snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"5|发布金币出售单成功 account:%s,rolename:%s %s: %u 换%s:%u",
				pstTradeSell->Pass9Account, pstTradeSell->RoleName,
				MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold,
				MONEY_TYPE_MONEY_NAME, pstTradeSell->Moeny);
		stOpTradeInfo.Flag = 5;
		z_role_trade_oplog(pstEnv,&stOpTradeInfo);			

		tlog_info(pstEnv->pstLogCat, 0, 0,	
			"|5|发布出售单成功|account|%s|RoleName|%s|TradeID|%llu|TradeNum|%u|Price|%u|",
			pstTradeSell->Pass9Account,
			pstTradeSell->RoleName,
			pstTradeSell->TradeID,
			pstTradeSell->Gold,
			pstTradeSell->Moeny
			);
	}

	return 0;
}

char *shopping_time_str(time_t tTime)
{
	static char szTime[64];
	szTime[0] = 0;
	strftime(szTime,sizeof(szTime),"%Y-%m-%d %H:%M",localtime(&tTime));

	return szTime;
}

int shopping_broadcast_tradeid_del(ZONESVRENV* pstEnv, int iType, tdr_ulonglong TradeID)
{
	CSPKG stPkgRes;
	CSSHOPPINGQUERYRECORDBYIDRES *pstShoppingQueryRecordByIDRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryRecordByIDRes;

	memset(pstShoppingQueryRecordByIDRes,0,sizeof(CSSHOPPINGQUERYRECORDBYIDRES));
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_RECORD_BY_ID_RES;

	pstShoppingQueryRecordByIDRes->Result = 0;
	pstShoppingQueryRecordByIDRes->OperType = 2;
	pstShoppingQueryRecordByIDRes->TradeRecord.TradeID = TradeID;
	pstShoppingQueryRecordByIDRes->TradeRecord.TradeType = iType;

	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_broadcast(pstEnv->pstAppCtx, pstEnv, &stPkgRes);
	
	return 0;
}

/*
**  @operType   	1 表示用户交易触发
** 					2 表示用户撤销触发
**					3 表示淘汰触发
*/
int shopping_db_tradesell_delete_req( ZONESVRENV* pstEnv,int iUin, TRADESELL *pstTradeSell,
								int operType,void *pData,unsigned int uiLen)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;
	char szBuf[1023];
	int   iOffset = 0;
	
	memset(szBuf,0,sizeof(szBuf));
	memcpy(szBuf,pstTradeSell,sizeof(TRADESELL));
	iOffset = sizeof(TRADESELL);
	memcpy(szBuf+iOffset,&operType,sizeof(operType));
	iOffset += sizeof(operType);	
	if( uiLen > 0 )
	{
		memcpy(szBuf+iOffset,pData,uiLen);
		iOffset += uiLen;
	}

	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,iUin,&pstPkg->Head, 
								TORM_DELETE_REQ,DB_DELETE_TRADE_SELL_RES, 
			               			"TradeSell", 
			               			szBuf, 
			               			iOffset);/* update 修改点1*/
	
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	//pstDeleteReq->OperInfo.WhereDef[0] = 0; // wait testing
		snprintf(pstDeleteReq->OperInfo.WhereDef,
		       sizeof(pstDeleteReq->OperInfo.WhereDef),
		       "where TradeID = %llu and Uin = %d",
		       pstTradeSell->TradeID,
		       pstTradeSell->Uin);
		
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,pstTradeSell, 
			sizeof(TRADESELL),&iDst,pstEnv->iRoleDBProtoVer);
	
}

/*
**  @operType   	1 表示用户交易触发
** 					2 表示用户撤销触发
**					3 表示淘汰触发
*/
int shopping_db_tradesell_delete_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg)
{	
	TORMDELETERES *pstDeleteRes;
	DBAsyn *pstDBAsyn;
	TRADESELL *pstTradeSell;
	TRADESELL stTmp;
	int operType;
	unsigned int uiTax  = 0;
	CSSHOPPINGTRADEREQ *pstShoppingTradeReq;
	TRADEPLAYERINFO *pstTradePlayerInfo = NULL;
	char szText[512];
	char szTitle[512];
	CSPKG stPkgRes;
	CSSHOPPINGDESTROYTRADERECORDRES *pstShoppingDestroyTradeRecordRes = NULL;
	OPTRADEINFO stOpTradeInfo;
	Player *pstOwner;

	pstDeleteRes = &pstPkg->Body.DeleteRes;
	pstDBAsyn = (DBAsyn *)pstPkg->Head.AsynCallBackData;
	pstTradeSell = (TRADESELL *)(pstDBAsyn->AsynCmd.szMaxBuf);
	memcpy(&stTmp, pstTradeSell, sizeof(stTmp));
	pstTradeSell = &stTmp;
	memcpy(&operType, pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADESELL),sizeof(operType));	
	
	memset(&stOpTradeInfo,0,sizeof(stOpTradeInfo));
	stOpTradeInfo.RoleInfo.RoleID = pstTradeSell->RoleID;
	STRNCPY(stOpTradeInfo.RoleInfo.RoleName,pstTradeSell->RoleName,CCH(stOpTradeInfo.RoleInfo.RoleName));
	STRNCPY(stOpTradeInfo.Account, pstTradeSell->Pass9Account,CCH(stOpTradeInfo.Account));
	stOpTradeInfo.OperType = 0;
	stOpTradeInfo.TradeID = pstTradeSell->TradeID;
	stOpTradeInfo.Money = pstTradeSell->Moeny;
	stOpTradeInfo.Gold = pstTradeSell->Gold;

	z_delete_tradesell(pstEnv, pstTradeSell->TradeID);
	//全服通知删除这个单
	shopping_broadcast_tradeid_del(pstEnv, 0, pstTradeSell->TradeID);

	if (TORM_ERR_IS_ERROR(pstDeleteRes->ErrNo) || 
		pstDeleteRes->AffectRows != 1 )
	{
		snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
			"6|用户account %s RoleName:%s出售金币单交易失败数据库错误无需补偿%s %u",
				pstTradeSell->Pass9Account,
				pstTradeSell->RoleName,
				MONEY_TYPE_GODCOIN_NAME,
				pstTradeSell->Gold);	
		stOpTradeInfo.Flag = 6;
		z_role_trade_oplog(pstEnv,&stOpTradeInfo);
		
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			"6|用户RoleName:%s出售金币单失败数据库错误无需补偿%s %u, tradeid %llu, err %s dbno %d",
				pstTradeSell->RoleName,
				MONEY_TYPE_GODCOIN_NAME,
				pstTradeSell->Gold, pstTradeSell->TradeID, torm_error_string(pstDeleteRes->ErrNo), pstDeleteRes->DBMSErrNo);	
		
		return 0;
	}
	else
	{
		if( OPERATE_BY_USER_TRADE == operType)
		{
			OPTRADEINFO stOpTradeInfo1;
			
			// 扣下B用户的银币数,这里扣可以避免被刷,如果后面失败可以根据日志运维补送回去 
			pstShoppingTradeReq = (CSSHOPPINGTRADEREQ *)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADESELL)+sizeof(operType));
			pstTradePlayerInfo = (TRADEPLAYERINFO *)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADESELL)+sizeof(operType)+sizeof(CSSHOPPINGTRADEREQ));

			uiTax = pstTradeSell->Moeny *1.0*TRADE_TAX_RATE/100;
				
			if ( !pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
				pstPlayer->stRoleData.RoleID != pstTradePlayerInfo->RoleID ||
				 0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, uiTax+pstTradeSell->Moeny))
			{
				snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
					"7|用户account %s RoleName:%s出售金币单交易失败,已邮件补偿，%s %u",
						pstTradeSell->Pass9Account, pstTradeSell->RoleName,
						MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold);			
				stOpTradeInfo.Flag = 7;
				z_role_trade_oplog(pstEnv,&stOpTradeInfo);
				
				tlog_error(pstEnv->pstLogCat, 0, 0, 
					"7|用户RoleName:%s出售金币单失败,已邮件补偿，%s %u",
						pstTradeSell->RoleName,
						MONEY_TYPE_GODCOIN_NAME,
						pstTradeSell->Gold);	

				player_sysmail_fast_send(pstEnv, pstTradeSell->RoleName, LJY_MAIL22, 
					LJY_MAIL23, NULL, 0,
					pstTradeSell->Gold, LJY_MAIL21, 1);
				return -1;
			}	
						
			// 扣B用户的银币
			package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, uiTax+pstTradeSell->Moeny);
			// 给B用户加金币 
			package_add_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstTradeSell->Gold);

			/*z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你消耗了%u银币购买了%u金币，请按【V】键打开商城查收。", 
						uiTax+pstTradeSell->Moeny, pstTradeSell->Gold);*/
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SHOP32, 
						uiTax+pstTradeSell->Moeny, pstTradeSell->Gold);

			if (pstTradeSell->Flag & TRADE_FLAG_SYS)
			{
				z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHOP_TEX, 2, 0, uiTax+pstTradeSell->Moeny, "系统金币交易");
				godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_SYSGOLD,0,0, 0,
					pstTradeSell->Gold,"系统出售金币产出");	
			}
			else
			{
				// 给A用户加银币,以邮件的形式	
				z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHOP_TEX, 0, 0, uiTax, "金币交易税");
								
				snprintf(szText,sizeof(szText),LJY_MAIL24,
					pstTradeSell->Gold, pstTradeSell->Moeny, shopping_time_str(pstAppCtx->stCurr.tv_sec));

				memset(szTitle,0,sizeof(szTitle));	
				snprintf(szTitle,sizeof(szTitle),LJY_MAIL25, MONEY_TYPE_GODCOIN_NAME);		
				player_sysmail_fast_send(pstEnv,pstTradeSell->RoleName,
					szTitle, szText,NULL,0, pstTradeSell->Moeny,  LJY_MAIL21,0);

				pstOwner = z_name_player(pstAppCtx, pstEnv, pstTradeSell->RoleName);
				if (pstOwner)
				{
					/*z_sys_strf(pstEnv, pstOwner, SYS_TIP, "您出售的%u金币已经出售成功，请查收邮件。", 
						pstTradeSell->Gold);*/
					z_send_sys_str_msg(pstAppCtx, pstEnv, pstOwner, SYS_SVR, ZONEERR_SHOP33, pstTradeSell->Gold);

					shopping_delete_user_tradecord(pstEnv, pstOwner, 0, pstTradeSell->TradeID);
				}
			}

			snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"8|用户RoleName:%s  购买 RoleName:%s %s %u 成功，花费%s:%u",
					pstPlayer->stRoleData.RoleName, pstTradeSell->RoleName,
					MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold,
					MONEY_TYPE_MONEY_NAME, pstTradeSell->Moeny);			
			if (pstTradeSell->Flag & TRADE_FLAG_SYS)
			{
				stOpTradeInfo.Flag = 8661;
			}
			else
			{
				stOpTradeInfo.Flag = 8;
			}
			z_role_trade_oplog(pstEnv,&stOpTradeInfo);

			memcpy(&stOpTradeInfo1, &stOpTradeInfo, sizeof(stOpTradeInfo1));
			snprintf(stOpTradeInfo1.OperDesc,sizeof(stOpTradeInfo1.OperDesc),
				"108|用户RoleName:%s  购买 RoleName:%s %s %u 成功，花费%s:%u",
					pstPlayer->stRoleData.RoleName, pstTradeSell->RoleName,
					MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold,
					MONEY_TYPE_MONEY_NAME, pstTradeSell->Moeny);	
			stOpTradeInfo1.Flag = 108;
			stOpTradeInfo1.RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
			STRNCPY(stOpTradeInfo1.RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stOpTradeInfo1.RoleInfo.RoleName));
			STRNCPY(stOpTradeInfo1.Account, pstPlayer->szAccount, sizeof(stOpTradeInfo1.Account));
			z_role_trade_oplog(pstEnv,&stOpTradeInfo1);
		
			tlog_info(pstEnv->pstLogCat, 0, 0,	
					"|8|购买%s, 给A用户发邮件加%s通知|account|%s|target_account|%s|TradeID|%llu|TradeType|%u|Gold|%u|Money|%u|",
					MONEY_TYPE_GODCOIN_NAME,
					MONEY_TYPE_MONEY_NAME,
					pstTradePlayerInfo->Pass9Account,
					pstTradeSell->Pass9Account,
					pstShoppingTradeReq->TradeID,
					pstShoppingTradeReq->TradeType,
					pstTradeSell->Gold,
					pstTradeSell->Moeny
					);	
					
		}
		else if (OPERATE_BY_SYS_TRADE == operType)
		{
			pstShoppingTradeReq = (CSSHOPPINGTRADEREQ *)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADESELL)+sizeof(operType));
			pstTradePlayerInfo = (TRADEPLAYERINFO *)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADESELL)+sizeof(operType)+sizeof(CSSHOPPINGTRADEREQ));
			
			snprintf(szText,sizeof(szText),LJY_MAIL24,
				pstTradeSell->Gold, pstTradeSell->Moeny, shopping_time_str(pstAppCtx->stCurr.tv_sec));

			memset(szTitle,0,sizeof(szTitle));	
			snprintf(szTitle,sizeof(szTitle),LJY_MAIL25, MONEY_TYPE_GODCOIN_NAME);		
			player_sysmail_fast_send(pstEnv,pstTradeSell->RoleName,
				szTitle, szText,NULL,0, pstTradeSell->Moeny,  LJY_MAIL21,0);

			pstOwner = z_name_player(pstAppCtx, pstEnv, pstTradeSell->RoleName);
			if (pstOwner)
			{
				/*z_sys_strf(pstEnv, pstOwner, SYS_TIP, "您出售的%u金币已经出售成功，请查收邮件。", 
					pstTradeSell->Gold);*/
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstOwner, SYS_SVR, ZONEERR_SHOP33, pstTradeSell->Gold);

				shopping_delete_user_tradecord(pstEnv, pstOwner, 0, pstTradeSell->TradeID);
			}
			

			snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"9993|系统购买 用户%s %s %u 成功，花费%s:%u",
					pstTradeSell->RoleName,
					MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold,
					MONEY_TYPE_MONEY_NAME, pstTradeSell->Moeny);			
			stOpTradeInfo.Flag = 9993;
			z_role_trade_oplog(pstEnv,&stOpTradeInfo);
		
			tlog_info(pstEnv->pstLogCat, 0, 0,	
					"|9993|购买%s, 给A用户发邮件加%s通知|account|%s|target_account|%s|TradeID|%llu|TradeType|%u|Gold|%u|Money|%u|",
					MONEY_TYPE_GODCOIN_NAME,
					MONEY_TYPE_MONEY_NAME,
					pstTradePlayerInfo->Pass9Account,
					pstTradeSell->Pass9Account,
					pstShoppingTradeReq->TradeID,
					pstShoppingTradeReq->TradeType,
					pstTradeSell->Gold,
					pstTradeSell->Moeny
					);	

			godcoin_consume_log(pstEnv,0,0,
					0,GODCOIN_OPER_FLAG_SYSGOLD,1,0, 0,
					pstTradeSell->Gold,"系统购买金币消耗");

			// 记录系统回收金币,系统交易单数,消耗银币
			pstEnv->pstGlobalObj->stGlobal.Misc.SysShop.ExchgInfo.Count += pstTradeSell->Gold;
			pstEnv->pstGlobalObj->stGlobal.Misc.SysShop.ExchgInfo.OutMoneyCount += pstTradeSell->Moeny;
			pstEnv->pstGlobalObj->stGlobal.Misc.SysShop.ExchgInfo.ExchgNum++;
		}
		else if( OPERATE_BY_USER_DESTROY == operType)
		{
		      if (pstTradeSell->Flag & TRADE_FLAG_SYS)
		      {
			  	return 0;
		      }
				
		      if(!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
				 pstPlayer->stRoleData.RoleID  != pstTradeSell->RoleID  ||
				 0>package_add_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstTradeSell->Gold) )
			{
				player_sysmail_fast_send(pstEnv, pstTradeSell->RoleName, LJY_MAIL26, 
						LJY_MAIL27, NULL, 0,
						pstTradeSell->Gold, LJY_MAIL21, 1);	
				
				snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc)," 用户account %s rolename %s 撤销%s出售单邮件返还金币%u",
					pstTradeSell->Pass9Account,  pstTradeSell->RoleName,
					MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold);			
				stOpTradeInfo.Flag = 74;
				z_role_trade_oplog(pstEnv,&stOpTradeInfo);

				return -1;
			}
			  
			// A用户撤单流程 
			pstShoppingDestroyTradeRecordRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingDestroyTradeRecordRes;
			memset(pstShoppingDestroyTradeRecordRes,0,sizeof(CSSHOPPINGDESTROYTRADERECORDRES));
			stPkgRes.Body.ShoppingSvr.Type = SHOPPING_DESTROY_TRADERECORD_RES;
			pstShoppingDestroyTradeRecordRes->Result = 0;
			pstShoppingDestroyTradeRecordRes->TradeType = 0;
			pstShoppingDestroyTradeRecordRes->TradeID = pstTradeSell->TradeID;			
			
			Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
			z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);	

			// 把金币加回去
			package_add_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstTradeSell->Gold);	
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP34, MONEY_TYPE_GODCOIN_NAME);
			
			snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),"43|  用户account %s rolename %s 撤销%s出售单成功, %u %s",
					pstPlayer->szAccount, pstPlayer->stRoleData.RoleName,
					MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold, MONEY_TYPE_GODCOIN_NAME);			
			stOpTradeInfo.Flag = 43;
			z_role_trade_oplog(pstEnv,&stOpTradeInfo);

			shopping_delete_user_tradecord(pstEnv, pstPlayer, 0, pstTradeSell->TradeID);

			
		}
		else if( OPERATE_BY_SYSTEM_SCAN == operType)
		{
			char szText[512];
			char szTitle[512];
			
			// 邮件返还A用户的金币 
			memset(szText,0,sizeof(szText));	
			snprintf(szText,sizeof(szText),LJY_MAIL28,
						pstTradeSell->Gold, MONEY_TYPE_GODCOIN_NAME);
			memset(szTitle,0,sizeof(szTitle));	
			snprintf(szTitle,sizeof(szTitle),LJY_MAIL29);
			player_sysmail_fast_send(pstEnv,pstTradeSell->RoleName, szTitle,szText,NULL,0, pstTradeSell->Gold,LJY_MAIL21,1);

			pstOwner = z_name_player(pstAppCtx, pstEnv, pstTradeSell->RoleName);
			if (pstOwner)
			{
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstOwner, SYS_SVR, ZONEERR_SHOP35);
				shopping_delete_user_tradecord(pstEnv, pstOwner, 0, pstTradeSell->TradeID);
			}

			snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),"用户account %s rolename %s 的金币出售单过期了,系统返%s %u", 
						pstTradeSell->Pass9Account, pstTradeSell->RoleName,
						MONEY_TYPE_GODCOIN_NAME, pstTradeSell->Gold);			

			if (pstTradeSell->Flag & TRADE_FLAG_SYS)
			{
				stOpTradeInfo.Flag = 9997;
			}
			else
			{
				stOpTradeInfo.Flag = 89;
			}
			z_role_trade_oplog(pstEnv,&stOpTradeInfo);
		}
		else if(OPERATE_BY_DEL_ROLE == operType )
		{
			
		}
		
	}

	return 0;
}

int shopping_db_tradebuy_count_req(ZONESVRENV *pstEnv)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTCOUNTREQ *pstSelectCountReq = &pstPkg->Body.SelectCountReq;
	TRADEBUY stTradeBuy;

	memset(&stTradeBuy,0,sizeof(stTradeBuy));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_COUNT_REQ,DB_SELECT_COUNT_TRADE_BUY_RES, 
			               			"TradeBuy", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectCountReq->WhereDef[0] = 0;

		
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stTradeBuy, 
				sizeof(TRADEBUY),&iDst,pstEnv->iRoleDBProtoVer);

}

int shopping_db_tradebuy_count_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{	
	TORMSELECTCOUNTRES *pstSelectCountRes;
	UNUSED(pstAppCtx);
	
	pstSelectCountRes = &pstPkg->Body.SelectCountRes;
	if (TORM_ERR_IS_ERROR(pstSelectCountRes->ErrNo))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "select count res error errno=%d, errmsg=%s", 
					pstSelectCountRes->ErrNo, torm_error_string(pstSelectCountRes->ErrNo));
		return -1;
	}
	
	if( pstSelectCountRes->RowsCount > MAX_TRADE_RECORD_NUM )
	{
		g_iTradeBuyRecordNum = MAX_TRADE_RECORD_NUM;
	}
	else 
	{
		g_iTradeBuyRecordNum = pstSelectCountRes->RowsCount;
	}

	if( pstEnv->pstConf->PreGetTradeRecordNum < 1 )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "conf err PreGetTradeRecordNum: %d", 
					pstEnv->pstConf->PreGetTradeRecordNum );
		return -1;
	}

	/* 开始取数据了 */
	shopping_db_tradebuy_select_req( pstEnv, g_iTradeBuyBeginRecord);
	
	g_iTradeBuyBeginRecord +=  pstEnv->pstConf->PreGetTradeRecordNum;
	g_iTradeBuyRecordNum -= 	pstEnv->pstConf->PreGetTradeRecordNum;
	
	
	return 0;
}

int shopping_db_tradebuy_select_req( ZONESVRENV* pstEnv,unsigned int iBeginRecord)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMSELECTREQ *pstSelectReq = &pstPkg->Body.SelectReq;
	TRADEBUY stTradeBuy;

	memset(&stTradeBuy,0,sizeof(stTradeBuy));
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,-1,&pstPkg->Head, 
								TORM_SELECT_REQ,DB_SELECT_TRADE_BUY_RES, 
			               			"TradeBuy", 
			               			NULL, 
			               			0);/* update 修改点1*/
	
	pstSelectReq->OffSet = iBeginRecord;
	pstSelectReq->Limit = pstEnv->pstConf->PreGetTradeRecordNum;
	pstSelectReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstSelectReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstSelectReq->OperInfo.WhereDef[0] = 0;
	snprintf(pstSelectReq->OperInfo.WhereDef,
	         sizeof(pstSelectReq->OperInfo.WhereDef),
	         "where TradeID != 0");	
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,&stTradeBuy, 
			sizeof(TRADEBUY),&iDst,pstEnv->iRoleDBProtoVer);
	
}
int shopping_db_tradebuy_select_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, TORMPKG *pstPkg)
{	
	int i;
	TORMSELECTRES *pstSelectRes;
	TRADEBUY stTradeBuy;
	UNUSED(pstAppCtx);

	if (pstEnv->pstZoneObj->iShopGetStat == 0)
	{
		return -1;
	}
	
	memset(&stTradeBuy,0,sizeof(stTradeBuy));
	pstSelectRes = &pstPkg->Body.SelectRes;
		
	for(i=0; i<pstSelectRes->SelectResult.ResultNum ; i++)
	{
		if (0 > torm_fetch_one_meta(pstEnv->hTorm, (char *)&stTradeBuy, sizeof(TRADEBUY)))
		{
			continue;
		}
		
		if( 0 != z_insert_tradebuy(pstEnv, &stTradeBuy) )
		{
			tlog_error(pstEnv->pstLogCat, 0, 0, "insert cache tradebuy error !");
		}
	}

	if(g_iTradeBuyRecordNum > 0 )
	{
		shopping_db_tradebuy_select_req( pstEnv, g_iTradeBuyBeginRecord);
		g_iTradeBuyBeginRecord +=  pstEnv->pstConf->PreGetTradeRecordNum;
		g_iTradeBuyRecordNum -= 	pstEnv->pstConf->PreGetTradeRecordNum;
	}
		
	return 0;
}


int shopping_db_tradebuy_insert_req( ZONESVRENV* pstEnv,int iUin, TRADEBUY *pstTradeBuy,void *pData,unsigned int uiLen)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMINSERTREQ *pstInsertReq =  &pstPkg->Body.InsertReq;

	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,iUin,&pstPkg->Head, 
								TORM_INSERT_REQ,DB_INSERT_TRADE_BUY_RES, 
			               			"TradeBuy", 
			               			pData, 
			               			uiLen);/* update 修改点1*/
	
	pstInsertReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstInsertReq->OperInfo.EntryNameList.EntryNameNum = 0;
	pstInsertReq->OperInfo.WhereDef[0] = 0;
	
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,pstTradeBuy, 
			sizeof(TRADEBUY),&iDst,pstEnv->iRoleDBProtoVer);
	
}

int shopping_db_tradebuy_insert_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg)
{
	TORMINSERTRES *pstInsertRes;
	DBAsyn *pstDBAsyn;
	TRADEBUY *pstTradeBuy;
	CSSHOPPINGSENDTRADERECORDREQ *pstShoppingSendTradeRecordReq;
	ROLETRADERECORDINFO *pstRoleTradeRecordInfo; 
	OPTRADEINFO stOpTradeInfo;
		
	pstInsertRes = &pstPkg->Body.InsertRes;
	pstDBAsyn =(DBAsyn *)pstPkg->Head.AsynCallBackData;
	pstTradeBuy = (TRADEBUY *)(pstDBAsyn->AsynCmd.szMaxBuf);

	memset(&stOpTradeInfo,0,sizeof(stOpTradeInfo));
	stOpTradeInfo.RoleInfo.RoleID = pstTradeBuy->RoleID;
	STRNCPY(stOpTradeInfo.RoleInfo.RoleName,pstTradeBuy->RoleName,CCH(stOpTradeInfo.RoleInfo.RoleName));
	STRNCPY(stOpTradeInfo.Account, pstTradeBuy->Pass9Account,CCH(stOpTradeInfo.Account));
	stOpTradeInfo.OperType = 1;
	pstTradeBuy->TradeID = stOpTradeInfo.TradeID = pstInsertRes->LastInsertID;
	stOpTradeInfo.Gold = pstTradeBuy->Gold;
	stOpTradeInfo.Money = pstTradeBuy->Moeny;
	
	if (pstTradeBuy->Flag & TRADE_FLAG_SYS)
	{
		if (0 == TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
		{
			if (0 == z_insert_tradebuy(pstEnv,pstTradeBuy) )
			{
				snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"9998|发布系统银币出售单成功account:%s,rolename:%s %s: %u 换%s:%u",
				pstTradeBuy->Pass9Account, pstTradeBuy->RoleName,
				MONEY_TYPE_MONEY_NAME,	pstTradeBuy->Moeny,
				MONEY_TYPE_GODCOIN_NAME, pstTradeBuy->Gold);
				
				stOpTradeInfo.Flag = 9998;
				z_role_trade_oplog(pstEnv,&stOpTradeInfo);
				shopping_broadcast_tradebuy(pstEnv, pstTradeBuy);
			}
		}
		
		return 0;
	}

	pstShoppingSendTradeRecordReq = (CSSHOPPINGSENDTRADERECORDREQ *) (pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADEBUY));
	
	if(TORM_ERR_IS_ERROR(pstInsertRes->ErrNo))
	{
		// 这里写条运营日志
		snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
			"9|发布银币出售单失败,邮件返还用户account:%s,rolename:%s %s: %u ",
			pstTradeBuy->Pass9Account, 
			pstTradeBuy->RoleName,
			MONEY_TYPE_MONEY_NAME,
			pstTradeBuy->Moeny);	
		stOpTradeInfo.Flag = 9;
		z_role_trade_oplog(pstEnv,&stOpTradeInfo);

		tlog_error(pstEnv->pstLogCat, 0, 0, "9|发布出售单失败,邮件返还用户account:%s,rolename:%s %s: %u , err %s, dbno %d",
				pstTradeBuy->Pass9Account, 
				pstTradeBuy->RoleName,
				MONEY_TYPE_MONEY_NAME,
				pstTradeBuy->Moeny,
				torm_error_string(pstInsertRes->ErrNo),
				pstInsertRes->DBMSErrNo);	

		if (pstPlayer && pstPlayer->eStatus == PLAYER_STATUS_ROLE_LOGIN)
		{
			z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP28);
		}

		player_sysmail_fast_send(pstEnv, pstTradeBuy->RoleName, LJY_MAIL30, 
			LJY_MAIL31, NULL, 0,
			pstTradeBuy->Moeny, LJY_MAIL32, 0);
		return 0;
	}

	//全服通知
	shopping_broadcast_tradebuy(pstEnv, pstTradeBuy);
	
	if (!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
		pstPlayer->stRoleData.RoleID != pstTradeBuy->RoleID  )
	{
		// 这里写条运营日志
		snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
			"51|发布银币出售单数据库成功但角色不在线无需补偿account:%s,rolename:%s %s: %u ",
			pstTradeBuy->Pass9Account, 
			pstTradeBuy->RoleName,
			MONEY_TYPE_MONEY_NAME,
			pstTradeBuy->Moeny);	
		stOpTradeInfo.Flag = 51;
		z_role_trade_oplog(pstEnv,&stOpTradeInfo);

		tlog_error(pstEnv->pstLogCat, 0, 0,"51|数据库成功但角色不在线了无需补偿account:%s,rolename:%s %s: %u ",
			pstTradeBuy->Pass9Account, 
			pstTradeBuy->RoleName,
			MONEY_TYPE_MONEY_NAME,
			pstTradeBuy->Moeny);

		z_insert_tradebuy(pstEnv,pstTradeBuy);
		
		return -1;
	}
	else
	{
		CSPKG stPkgRes;
		CSSHOPPINGSENDTRADERECORDRES *pstShoppingSendTradeRecordRes;
				
		pstRoleTradeRecordInfo = &pstPlayer->stRoleData.MiscInfo.RoleTradeRecordInfo;
				
		if( pstRoleTradeRecordInfo->TradeBuyNum >= MAX_SIGLE_USER_TRADERECORD_NUM ||
			0 != z_insert_tradebuy(pstEnv,pstTradeBuy) )
		{
			z_rolemem_warp(pstPlayer, &stOpTradeInfo.RoleInfo);
			snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"11|发布银币出售单数据库成功但插入交易单失败无需补偿account:%s,rolename:%s %s: %u ",
				pstTradeBuy->Pass9Account, 
				pstTradeBuy->RoleName,
				MONEY_TYPE_MONEY_NAME,
				pstTradeBuy->Moeny);	
			stOpTradeInfo.Flag = 11;
			z_role_trade_oplog(pstEnv,&stOpTradeInfo);

			tlog_error(pstEnv->pstLogCat, 0, 0, "11|数据库成功但插入交易单失败无需补偿account:%s,rolename:%s %s: %u ",
					pstTradeBuy->Pass9Account, 
					pstTradeBuy->RoleName,
					MONEY_TYPE_MONEY_NAME,
					pstTradeBuy->Moeny);	

			return -1;
		}
		
		pstRoleTradeRecordInfo->TradeSendTime = pstAppCtx->stCurr.tv_sec;
		pstRoleTradeRecordInfo->TradeBuyID[pstRoleTradeRecordInfo->TradeBuyNum] = pstTradeBuy->TradeID;
		pstRoleTradeRecordInfo->TradeBuyNum++;
		
		pstShoppingSendTradeRecordRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingSendTradeRecordRes;
		memset(pstShoppingSendTradeRecordRes,0,sizeof(CSSHOPPINGSENDTRADERECORDRES));

		stPkgRes.Body.ShoppingSvr.Type = SHOPPING_SEND_TRADERECORD_RES;
		pstShoppingSendTradeRecordRes->Result = 0;

		tradebuy_record(pstEnv, &pstShoppingSendTradeRecordRes->TradeRecord, pstTradeBuy);
		
		/* 下发数据包 */
		Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
		z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

		z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP27);
		
		tlog_info(pstEnv->pstLogCat, 0, 0,	
			"|12|发布求购成功|account|%s|TradeType|%u|Price|%u|TradeNum|%u|Time|%u|",
			pstPlayer->szAccount,
			pstShoppingSendTradeRecordReq->TradeType,
			pstShoppingSendTradeRecordReq->Money,
			pstShoppingSendTradeRecordReq->Gold,
			pstShoppingSendTradeRecordReq->Time
			);	

		snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
			"12|发布银币出售单成功 account:%s,rolename:%s %s: %u 换%s:%u",
			pstTradeBuy->Pass9Account, pstTradeBuy->RoleName,
			MONEY_TYPE_MONEY_NAME,	pstTradeBuy->Moeny,
			MONEY_TYPE_GODCOIN_NAME, pstTradeBuy->Gold);
		stOpTradeInfo.Flag = 12;
		z_role_trade_oplog(pstEnv,&stOpTradeInfo);
	}

	return 0;
	UNUSED(pstAppCtx);
}

int shopping_db_tradebuy_delete_req( ZONESVRENV* pstEnv,int iUin, TRADEBUY *pstTradeBuy,
					int operType,void *pData,unsigned int uiLen)
{
	int iDst = 0;
	TORMPKG *pstPkg = &pstEnv->stOrmPkg;
	TORMDELETEREQ *pstDeleteReq = &pstPkg->Body.DeleteReq;
	char szBuf[1023];
	int   iOffset = 0;
	
	memset(szBuf,0,sizeof(szBuf));
	memcpy(szBuf,pstTradeBuy,sizeof(TRADEBUY));
	iOffset = sizeof(TRADEBUY);
	memcpy(szBuf+iOffset,&operType,sizeof(operType));
	iOffset += sizeof(operType);
	if( uiLen > 0 )
	{
		memcpy(szBuf+iOffset,pData,uiLen);
		iOffset += uiLen;
	}
	
	shopping_db_init_torm_head(pstEnv->pstAppCtx, pstEnv,iUin,&pstPkg->Head, 
								TORM_DELETE_REQ,DB_DELETE_TRADE_BUY_RES, 
			               			"TradeBuy", 
			               			szBuf, 
			               			iOffset);/* update 修改点1*/
	
	pstDeleteReq->OperInfo.ResFlag = TORM_RESFLAG_RET_NOINFO;
	pstDeleteReq->OperInfo.EntryNameList.EntryNameNum = 0;
	//pstDeleteReq->OperInfo.WhereDef[0] = 0; // wait testing
		snprintf(pstDeleteReq->OperInfo.WhereDef,
		       sizeof(pstDeleteReq->OperInfo.WhereDef),
		       "where TradeID = %llu and Uin = %d",
		       pstTradeBuy->TradeID,
		       pstTradeBuy->Uin);
		
	iDst = pstEnv->iMiscDBID;
	return shopping_db_send_req(pstEnv->pstAppCtx, pstEnv,pstPkg,pstTradeBuy, 
			sizeof(TRADEBUY),&iDst,pstEnv->iRoleDBProtoVer);
	
}

int shopping_db_tradebuy_delete_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, Player *pstPlayer,TORMPKG *pstPkg)
{	
	TORMDELETERES *pstDeleteRes;
	DBAsyn *pstDBAsyn;
	TRADEBUY *pstTradeBuy;
	TRADEBUY stTmp;
	unsigned int operType = 0;
	CSSHOPPINGTRADEREQ *pstShoppingTradeReq = NULL;
	TRADEPLAYERINFO *pstTradePlayerInfo = NULL;
 	OPTRADEINFO stOpTradeInfo;
	OPTRADEINFO stOpTradeInfo1;
	char szText[512];
	char szTitle[512];
	unsigned uiTax = 0;
	Player *pstOwner;
					
	pstDeleteRes = &pstPkg->Body.DeleteRes;
	pstDBAsyn = (DBAsyn *)pstPkg->Head.AsynCallBackData;
	pstTradeBuy = (TRADEBUY *)(pstDBAsyn->AsynCmd.szMaxBuf);
	memcpy(&stTmp, pstTradeBuy, sizeof(stTmp));
	pstTradeBuy = &stTmp;
	memcpy(&operType, pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADEBUY),sizeof(operType));

	memset(&stOpTradeInfo,0,sizeof(stOpTradeInfo));
	stOpTradeInfo.RoleInfo.RoleID = pstTradeBuy->RoleID;
	STRNCPY(stOpTradeInfo.RoleInfo.RoleName,pstTradeBuy->RoleName,CCH(stOpTradeInfo.RoleInfo.RoleName));
	STRNCPY(stOpTradeInfo.Account, pstTradeBuy->Pass9Account,CCH(stOpTradeInfo.Account));
	stOpTradeInfo.OperType = 1;
	stOpTradeInfo.TradeID = pstTradeBuy->TradeID;
	stOpTradeInfo.Money = pstTradeBuy->Moeny;
	stOpTradeInfo.Gold = pstTradeBuy->Gold;

	SHOPEXCHGINFO *pstExchgInfo = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop.ExchgInfo;

	z_delete_tradebuy(pstEnv, pstTradeBuy->TradeID);
	//全服通知删除这个单
	shopping_broadcast_tradeid_del(pstEnv, 1, pstTradeBuy->TradeID);
	
	if (TORM_ERR_IS_ERROR(pstDeleteRes->ErrNo) ||
		pstDeleteRes->AffectRows != 1 )
	{
		snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
			"57|用户account %s RoleName:%s出售银币单交易失败数据库错误无需补偿%s %u",
				pstTradeBuy->Pass9Account, pstTradeBuy->RoleName,
				MONEY_TYPE_MONEY_NAME, pstTradeBuy->Moeny);
		stOpTradeInfo.Flag = 57;
		z_role_trade_oplog(pstEnv,&stOpTradeInfo);
		
		tlog_error(pstEnv->pstLogCat, 0, 0, 
			"57|用户RoleName:%s出售银币单失败数据库错误无需补偿%s %u, tradeid %llu, err %s dbno %d",
				pstTradeBuy->RoleName,
				MONEY_TYPE_MONEY_NAME,
				pstTradeBuy->Moeny, pstTradeBuy->TradeID,
				torm_error_string(pstDeleteRes->ErrNo), pstDeleteRes->DBMSErrNo);	
		
		return -1;
	}
	else 
	{
		if( OPERATE_BY_USER_TRADE == operType)
		{
			//交易流程
			pstShoppingTradeReq = (CSSHOPPINGTRADEREQ *)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADEBUY)+sizeof(operType));
			pstTradePlayerInfo = (TRADEPLAYERINFO *)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADEBUY)+sizeof(operType)+sizeof(CSSHOPPINGTRADEREQ));

			uiTax = pstTradeBuy->Moeny * 1.0*TRADE_TAX_RATE/100;

			if( !pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
				pstPlayer->stRoleData.RoleID != pstTradePlayerInfo->RoleID ||
				0>package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstTradeBuy->Gold))
			{
				snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
					"58|用户account %s RoleName:%s出售银币单交易失败%s %u, 已邮件补偿",
						pstTradeBuy->Pass9Account,	pstTradeBuy->RoleName,
						MONEY_TYPE_MONEY_NAME, pstTradeBuy->Moeny);
				stOpTradeInfo.Flag = 58;
				z_role_trade_oplog(pstEnv,&stOpTradeInfo);
				
				tlog_error(pstEnv->pstLogCat, 0, 0, 
					"58|用户RoleName:%s出售银币单失败%s %u, 已邮件补偿",
						pstTradeBuy->RoleName,
						MONEY_TYPE_MONEY_NAME,
						pstTradeBuy->Moeny);

				player_sysmail_fast_send(pstEnv, pstTradeBuy->RoleName, "出售银币交易失败，银币返还", 
					LJY_MAIL34, NULL, 0, 
					pstTradeBuy->Moeny, LJY_MAIL32 , 0);
				return -1;
			}
		
			// 扣c用户的金币
			package_dec_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, pstTradeBuy->Gold);
			// 给c用户加银币
			package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstTradeBuy->Moeny-uiTax);

			/*z_sys_strf(pstEnv, pstPlayer, SYS_TIP, "你消耗了%u金币购买了%u银币（已收税），请按【B】键打开背包查收。", 
						pstTradeBuy->Gold,pstTradeBuy->Moeny-uiTax);*/
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_SVR, ZONEERR_SHOP36, 
						pstTradeBuy->Gold,pstTradeBuy->Moeny-uiTax);

			if (pstTradeBuy->Flag & TRADE_FLAG_SYS)
			{
				z_money_create_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHOP_TEX, 2, 0, pstTradeBuy->Moeny-uiTax, "系统银币交易");
				godcoin_consume_log(pstEnv,pstPlayer->stRoleData.Uin,pstPlayer->stRoleData.RoleID,
					pstPlayer->stRoleData.Level,GODCOIN_OPER_FLAG_SYSMONEY,0,0, 0,
					pstTradeBuy->Gold,"系统出售银币消耗");	
			}
			else
			{
				z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHOP_TEX, 1, 0, uiTax, "银币交易");
				
				// 给D用户加金币
				snprintf(szText,sizeof(szText),LJY_MAIL35,
					pstTradeBuy->Moeny, pstTradeBuy->Gold, shopping_time_str(pstAppCtx->stCurr.tv_sec));
				
				snprintf(szTitle,sizeof(szTitle),LJY_MAIL25, MONEY_TYPE_MONEY_NAME);
				player_sysmail_fast_send(pstEnv,pstTradeBuy->RoleName,
					szTitle, szText,NULL,0, pstTradeBuy->Gold,LJY_MAIL32,1);

				pstOwner = z_name_player(pstAppCtx, pstEnv, pstTradeBuy->RoleName);
				if (pstOwner)
				{
					/*z_sys_strf(pstEnv, pstOwner, SYS_TIP, "您出售的%u银币已经出售成功，请查收邮件。", 
								pstTradeBuy->Moeny);*/
					z_send_sys_str_msg(pstAppCtx , pstEnv, pstOwner, SYS_SVR, ZONEERR_SHOP37, pstTradeBuy->Moeny);
					shopping_delete_user_tradecord(pstEnv, pstOwner, 1, pstTradeBuy->TradeID);
				}
			}

			snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"14|银币出售单交易成功,用户%s购买用户%s银币%u, 价格: %s %u",
					pstPlayer->stRoleData.RoleName, pstTradeBuy->RoleName, pstTradeBuy->Moeny,
					MONEY_TYPE_GODCOIN_NAME,
					pstTradeBuy->Gold);

			if (pstTradeBuy->Flag & TRADE_FLAG_SYS)
			{
				stOpTradeInfo.Flag = 8662;
			}
			else
			{
				stOpTradeInfo.Flag = 14;
			}
			z_role_trade_oplog(pstEnv,&stOpTradeInfo);

			memcpy(&stOpTradeInfo1, &stOpTradeInfo, sizeof(stOpTradeInfo1));
			stOpTradeInfo1.Flag = 114;
			stOpTradeInfo1.RoleInfo.RoleID = pstPlayer->stRoleData.RoleID;
			STRNCPY(stOpTradeInfo1.RoleInfo.RoleName, pstPlayer->stRoleData.RoleName, sizeof(stOpTradeInfo1.RoleInfo.RoleName));
			STRNCPY(stOpTradeInfo1.Account, pstPlayer->szAccount, sizeof(stOpTradeInfo1.Account));
			z_role_trade_oplog(pstEnv,&stOpTradeInfo1);

			gold_rate_data(pstEnv, pstTradeBuy);
		}
		else if (OPERATE_BY_SYS_TRADE == operType)
		{
			// 这里在判断一下，回收金币小于支出金币，不干； 
			if (pstExchgInfo->Count - pstExchgInfo->OutCount < pstTradeBuy->Gold)
			{
				return -1;
			}

			pstShoppingTradeReq = (CSSHOPPINGTRADEREQ *)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADEBUY)+sizeof(operType));
			pstTradePlayerInfo = (TRADEPLAYERINFO *)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADEBUY)+sizeof(operType)+sizeof(CSSHOPPINGTRADEREQ));

			// 给D用户加金币
			snprintf(szText,sizeof(szText),LJY_MAIL35,
				pstTradeBuy->Moeny, pstTradeBuy->Gold, shopping_time_str(pstAppCtx->stCurr.tv_sec));
			
			snprintf(szTitle,sizeof(szTitle),LJY_MAIL25, MONEY_TYPE_MONEY_NAME);
			player_sysmail_fast_send(pstEnv,pstTradeBuy->RoleName,
				szTitle, szText,NULL,0, pstTradeBuy->Gold,LJY_MAIL32,1);

			pstOwner = z_name_player(pstAppCtx, pstEnv, pstTradeBuy->RoleName);
			if (pstOwner)
			{
				/*z_sys_strf(pstEnv, pstOwner, SYS_TIP, "您出售的%u银币已经出售成功，请查收邮件。", 
							pstTradeBuy->Moeny);*/
				z_send_sys_str_msg(pstAppCtx, pstEnv, pstOwner, SYS_SVR,  ZONEERR_SHOP37, pstTradeBuy->Moeny);
				shopping_delete_user_tradecord(pstEnv, pstOwner, 1, pstTradeBuy->TradeID);
			}
				
			snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
				"9991|银币出售单交易成功,系统购买 用户account %s rolename %s %s %u, 价格%s %u",
					pstTradeBuy->Pass9Account, pstTradeBuy->RoleName, 
					MONEY_TYPE_MONEY_NAME,	pstTradeBuy->Moeny,
					MONEY_TYPE_GODCOIN_NAME, pstTradeBuy->Gold);
			stOpTradeInfo.Flag = 9991;
			z_role_trade_oplog(pstEnv,&stOpTradeInfo);

			z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_SHOP_SYS, 0, 0, pstTradeBuy->Moeny, "系统收购银币消耗");

			// 记下收进来的银币和支出金币
			pstExchgInfo->OutCount += pstTradeBuy->Gold;
			pstExchgInfo->MoneyCount += pstTradeBuy->Moeny;
			pstExchgInfo->ExchgNum ++;
			
		}
		else if( OPERATE_BY_USER_DESTROY == operType)
		{
			if (pstTradeBuy->Flag & TRADE_FLAG_SYS)
			{
				return 0;
			}
			
			pstTradePlayerInfo= (TRADEPLAYERINFO*)(pstDBAsyn->AsynCmd.szMaxBuf+sizeof(TRADEBUY)+sizeof(operType));
			
			if( !pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
				pstPlayer->stRoleData.RoleID != pstTradePlayerInfo->RoleID ||
				0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstTradeBuy->Moeny) )
			{
				//返还银币
				player_sysmail_fast_send(pstEnv,pstTradeBuy->RoleName, LJY_MAIL36,
					LJY_MAIL37,NULL,0, 
					pstTradeBuy->Moeny,LJY_MAIL32,0);
				
				snprintf(stOpTradeInfo.OperDesc, sizeof(stOpTradeInfo.OperDesc),
					" 用户account %s rolename %s 撤销%s出售单邮件返还银币%u",
					pstTradeBuy->Pass9Account,  pstTradeBuy->RoleName,
					MONEY_TYPE_MONEY_NAME, pstTradeBuy->Moeny);			
				stOpTradeInfo.Flag = 79;
				z_role_trade_oplog(pstEnv,&stOpTradeInfo);

				return -1;

			}
			else
			{
				// D用户撤单成功 
				CSPKG stPkgRes;
				CSSHOPPINGDESTROYTRADERECORDRES *pstShoppingDestroyTradeRecordRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingDestroyTradeRecordRes;
				memset(pstShoppingDestroyTradeRecordRes,0,sizeof(CSSHOPPINGDESTROYTRADERECORDRES));
				stPkgRes.Body.ShoppingSvr.Type = SHOPPING_DESTROY_TRADERECORD_RES;
				pstShoppingDestroyTradeRecordRes->Result = 0;
				pstShoppingDestroyTradeRecordRes->TradeType = 1;
				pstShoppingDestroyTradeRecordRes->TradeID = pstTradeBuy->TradeID;			
				
				Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
				z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);	

				package_add_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstTradeBuy->Moeny);
				z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP29);

				snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
					" 26|用户account %s rolename %s 撤销%s出售单成功, %u %s ",
					pstPlayer->szAccount, pstPlayer->stRoleData.RoleName, 
					MONEY_TYPE_MONEY_NAME, pstTradeBuy->Moeny, MONEY_TYPE_MONEY_NAME);			
				stOpTradeInfo.Flag = 26;
				z_role_trade_oplog(pstEnv,&stOpTradeInfo);

				shopping_delete_user_tradecord(pstEnv, pstPlayer, 1, pstTradeBuy->TradeID);
			}

		}
		else if( OPERATE_BY_SYSTEM_SCAN == operType)
		{			
				char szText[512];
				char szTitle[512];
								
				snprintf(szText,sizeof(szText),LJY_MAIL38,
							pstTradeBuy->Moeny, MONEY_TYPE_MONEY_NAME);

				snprintf(szTitle,sizeof(szTitle),LJY_MAIL39);
				
				player_sysmail_fast_send(pstEnv,pstTradeBuy->RoleName, szTitle,szText,NULL,0, pstTradeBuy->Moeny,LJY_MAIL32,0);

				pstOwner = z_name_player(pstAppCtx, pstEnv, pstTradeBuy->RoleName);
				if (pstOwner)
				{
					z_send_sys_str_msg(pstAppCtx, pstEnv, pstOwner, SYS_SVR, ZONEERR_SHOP38);
					shopping_delete_user_tradecord(pstEnv, pstOwner, 1, pstTradeBuy->TradeID);
				}

				
				snprintf(stOpTradeInfo.OperDesc,sizeof(stOpTradeInfo.OperDesc),
					"用户account %s rolename %s 银币出售单过期了,系统返%s %u", 
					pstTradeBuy->Pass9Account, pstTradeBuy->RoleName,
					MONEY_TYPE_MONEY_NAME, pstTradeBuy->Moeny);			
				if (pstTradeBuy->Flag & TRADE_FLAG_SYS)
				{
					stOpTradeInfo.Flag = 9996;
				}
				else
				{
					stOpTradeInfo.Flag = 75;
				}
				z_role_trade_oplog(pstEnv,&stOpTradeInfo);
		}
		else if(OPERATE_BY_DEL_ROLE == operType )
		{
			
		}
	}
	return 0;
}

/* 整理个初始化函数出来先 */
int z_shopping_init(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{	
	FILE *fp;
	struct stat stStat;
	UNUSED(pstAppCtx);

	if (0 != stat(pstEnv->pstConf->ShoppingXmlFile, &stStat))
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "open xml fail file:%s ",
							pstEnv->pstConf->ShoppingXmlFile);
		
		return -1;
	}
	fp = fopen(pstEnv->pstConf->ShoppingXmlFile, "rb");
	if (NULL == fp)
	{
		return -1;
	}
	if( stStat.st_size > MAX_SHOPPING_XML_FILE_SIZE )
	{
		tlog_error(pstEnv->pstLogCat, 0, 0, "open xml fail! file max size:%d ",
							stStat.st_size);
		return -1;
	}
	g_stShoppingGetXmlFileRes.FileLen = stStat.st_size;
	fread(g_stShoppingGetXmlFileRes.FileBuff, stStat.st_size, 1, fp);
	fclose(fp);

	// 初始化下共享内存
	pstEnv->pstZoneObj->pstTradeSellHash = sht_init(pstEnv->pstZoneObj->stTradeSellMemory.sMem, 
	                             			sizeof(pstEnv->pstZoneObj->stTradeSellMemory.sMem),
								MAX_TRADESELL_BUCKET, 
								MAX_TRADESELL_BUCKET, 
								sizeof(TRADESELL));
	if(!pstEnv->pstZoneObj->pstTradeSellHash)
	{
		printf("z_shopping_init pstTradeSellHash fail \n");
		return -1;
	}
	pstEnv->pstZoneObj->pstTradeBuyHash = sht_init(pstEnv->pstZoneObj->stTradeBuyMemory.sMem, 
	                             			sizeof(pstEnv->pstZoneObj->stTradeBuyMemory.sMem),
								MAX_TRADESELL_BUCKET, 
								MAX_TRADESELL_BUCKET, 
								sizeof(TRADEBUY));
	if(!pstEnv->pstZoneObj->pstTradeBuyHash)
	{
		printf("z_shopping_init pstTradeSellHash fail \n");
		return -1;
	}



	/* the other */
 	g_uiSellScanNum = 0;
	g_uiBuyScanNum =0 ;
	g_uiShopLimitDefScanNum = 0;
	g_iTradeSellBeginRecord = 0;
	g_iTradeSellRecordNum = 0;
	g_iTradeBuyBeginRecord = 0;
	g_iTradeBuyRecordNum = 0;
	g_iShopMsgSendFlag = 0;
	g_iShopGetPropsDefTime = pstAppCtx->stCurr.tv_sec;

	// 把表数据给弄出来
	if( INST_MODE_ZONE_SVR == pstEnv->iInstMode )
	{
		shopping_db_tradesell_count_req(pstEnv);
		shopping_db_tradebuy_count_req(pstEnv);

		//shopping_ss_query_propsdef_req(pstAppCtx, pstEnv);
	}
	return 0;
}
int z_shopping_trade_tick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, long long uiPreScanNum)
{	
	long long  i = 0;
	int j;
	unsigned long long  uiScanIndex = 0;
	TRADESELL *pstTradeSell = NULL;
	TRADEBUY *pstTradeBuy = NULL;
	time_t 	tCurr = pstAppCtx->stCurr.tv_sec;
	int iValid;
	SYSSHOPINFO *pstSysShop;
	UNUSED(pstAppCtx);

	if (g_iTradeSellRecordNum <= 0)
	{
		for(  i=0; i<uiPreScanNum; i++ ) 
		{
			uiScanIndex = g_uiSellScanNum % pstEnv->pstZoneObj->pstTradeSellHash->iMax;
			pstTradeSell = (TRADESELL *)sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, uiScanIndex, &iValid);
			if (NULL == pstTradeSell || !iValid )
			{
				g_uiSellScanNum++;
				continue;
			}
			g_uiSellScanNum++;

			if( (unsigned int)tCurr >= pstTradeSell->StartTime+pstTradeSell->Time )
			{
				//做个删除操作
				shopping_db_tradesell_delete_req( pstEnv,0, pstTradeSell,OPERATE_BY_SYSTEM_SCAN,
								pstTradeSell,sizeof(TRADESELL));
				z_delete_tradesell(pstEnv, pstTradeSell->TradeID);
			}	
		}
	}

	if (g_iTradeBuyRecordNum <= 0)
	{
		for(  i=0; i<uiPreScanNum; i++ ) 
		{
			uiScanIndex = g_uiBuyScanNum% pstEnv->pstZoneObj->pstTradeBuyHash->iMax;
			pstTradeBuy = (TRADEBUY *)sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, uiScanIndex, &iValid);
			if (NULL == pstTradeBuy || !iValid )
			{
				g_uiBuyScanNum++;
				continue;
			}
			g_uiBuyScanNum++;
			
			if( (unsigned int)tCurr >= pstTradeBuy->StartTime+pstTradeBuy->Time )
			{
				//做个删除操作
				shopping_db_tradebuy_delete_req( pstEnv,0,pstTradeBuy,
						OPERATE_BY_SYSTEM_SCAN,pstTradeBuy,sizeof(TRADEBUY));
				z_delete_tradebuy(pstEnv, pstTradeBuy->TradeID);
			}
		}	
	}

	if(pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_STAT_FINI)
	{
		return 1;
	}

	pstSysShop = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop;
	
	// cgi操作
	for (i=pstSysShop->iSellGold-1; i>=0; i--)
	{
		if (tCurr < (time_t)pstSysShop->SellGold[i].EndTime && pstSysShop->SellGold[i].NextTime <= (unsigned int)tCurr && pstSysShop->SellGold[i].RemNum > 0)
		{
			TRADESELL stTradeSell;
			if( pstEnv->pstZoneObj->pstTradeSellHash->iItem >= MAX_TRADE_RECORD_NUM -10 )
			{
				return -1;
			}

			memset(&stTradeSell,0, sizeof(stTradeSell));	
			stTradeSell.Gold = pstSysShop->SellGold[i].Num;
			stTradeSell.StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			stTradeSell.Time = MAX_TRADE_SHOW_TIME*3600;
			stTradeSell.Moeny = pstSysShop->SellGold[i].Price;
			stTradeSell.Flag |= TRADE_FLAG_SYS;
			stTradeSell.SysID = 0;

			// 这里不管成功与否,接下来都更新挂单信息
			shopping_db_tradesell_insert_req(pstEnv, -1, &stTradeSell );

			// 下一次挂单时间
			pstSysShop->SellGold[i].NextTime = tCurr +pstSysShop->SellGold[i].InterVal;
			pstSysShop->SellGold[i].RemNum --;

			// 挂完了,删除
			/*if (pstSysShop->SellGold[i].RemNum == 0)
			{
				pstSysShop->iSellGold --;
				if (pstSysShop->iSellGold != i)
				{
					pstSysShop->SellGold[i] = pstSysShop->SellGold[pstSysShop->iSellGold];		
				}
			}*/
		} 
	}

	for (i=pstSysShop->iSellMoney-1; i>=0; i--)
	{
		if (tCurr < (time_t)pstSysShop->SellMoney[i].EndTime && pstSysShop->SellMoney[i].NextTime <= (unsigned int)tCurr && pstSysShop->SellMoney[i].RemNum > 0)
		{
			TRADEBUY stTradeBuy;

			if( pstEnv->pstZoneObj->pstTradeBuyHash->iItem >= MAX_TRADE_RECORD_NUM -10 )
			{
				return -1;
			}
			
			memset(&stTradeBuy,0, sizeof(stTradeBuy));	
			stTradeBuy.Moeny = pstSysShop->SellMoney[i].Num;
			stTradeBuy.Time = MAX_TRADE_SHOW_TIME*3600;
			stTradeBuy.StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			stTradeBuy.Gold = pstSysShop->SellMoney[i].Price;
			stTradeBuy.Flag |= TRADE_FLAG_SYS;
			stTradeBuy.SysID = 0;
			shopping_db_tradebuy_insert_req(pstEnv, -1, &stTradeBuy, &stTradeBuy, sizeof(stTradeBuy));

			pstSysShop->SellMoney[i].NextTime =  tCurr +pstSysShop->SellMoney[i].InterVal;
			pstSysShop->SellMoney[i].RemNum --;

			/*if (pstSysShop->SellMoney[i].RemNum == 0)
			{
				pstSysShop->iSellMoney --;
				if (pstSysShop->iSellMoney != i)
				{
					pstSysShop->SellMoney[i] = pstSysShop->SellMoney[pstSysShop->iSellMoney];		
				}
			}*/
		}	
	}

	for (i=pstSysShop->iBuyMoney-1; i>=0; i--)
	{
		if (tCurr > (time_t)pstSysShop->BuyMoney[i].EndTime ||	pstSysShop->BuyMoney[i].NextTime > (unsigned int)tCurr || 
			pstSysShop->BuyMoney[i].RemNum <= 0)
		{
			continue;
		}
			
		unsigned int iMinPrice = pstSysShop->BuyMoney[i].Price;
		int iIdx = -1;
		for (j=0; j<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; j++)
		{
			pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, j, &iValid);
			if (	NULL == pstTradeBuy || !iValid || (pstTradeBuy->Flag & TRADE_FLAG_SYS) ||
				(int)pstTradeBuy->Moeny != pstSysShop->BuyMoney[i].Num)
			{
				continue;
			}

			if (iMinPrice >= pstTradeBuy->Gold)
			{
				iMinPrice = pstTradeBuy->Gold;
				iIdx = j;
			}
		}

		if (0 > iIdx)
		{
			continue;
		}

		pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, iIdx, &iValid);
		if (NULL == pstTradeBuy || !iValid )
		{
			continue;
		}

		{
			/* 删除A用户的发布记录 */
			char szCallBack[1024];
			int iLen = 0;
			TRADEPLAYERINFO stTradePlayerInfo;
			CSSHOPPINGTRADEREQ stShoppingTradeReq;
						
			memset(szCallBack,0,sizeof(szCallBack));
			stShoppingTradeReq.TradeID = pstTradeBuy->TradeID;
			stShoppingTradeReq.TradeType = 0;
			shopping_tradeplayerinfo_init(NULL,&stTradePlayerInfo);
			memcpy(szCallBack+iLen, &stShoppingTradeReq,sizeof(CSSHOPPINGTRADEREQ));
			iLen += sizeof(CSSHOPPINGTRADEREQ);
			memcpy(szCallBack+iLen,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));
			iLen += sizeof(TRADEPLAYERINFO);
			shopping_db_tradebuy_delete_req(pstEnv,-1, pstTradeBuy,OPERATE_BY_SYS_TRADE,
								szCallBack,iLen);
			
			z_delete_tradebuy(pstEnv,pstTradeBuy->TradeID);
			pstSysShop->BuyMoney[i].NextTime = pstSysShop->BuyMoney[i].InterVal+tCurr;
			pstSysShop->BuyMoney[i].RemNum --;

			/*if (pstSysShop->BuyMoney[i].RemNum == 0)
			{
				pstSysShop->iBuyMoney --;
				if (pstSysShop->iBuyMoney != i)
				{
					pstSysShop->BuyMoney[i] = pstSysShop->BuyMoney[pstSysShop->iBuyMoney];		
				}
			}*/
		}

	}

	for (i=pstSysShop->iBuyGold-1; i>=0; i--)
	{
		if (tCurr > (time_t)pstSysShop->BuyGold[i].EndTime ||	pstSysShop->BuyGold[i].NextTime > (unsigned int)tCurr || 
			pstSysShop->BuyGold[i].RemNum <= 0)
		{
			continue;
		}
		
		unsigned int iMinPrice =pstSysShop->BuyGold[i].Price;
		int iIdx = -1;
		for (j=0; j<pstEnv->pstZoneObj->pstTradeSellHash->iMax; j++)
		{
			pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, j, &iValid);
			if (NULL == pstTradeSell || !iValid || (pstTradeSell->Flag & TRADE_FLAG_SYS) || 
				(int)pstTradeSell->Gold != pstSysShop->BuyGold[i].Num)
			{
				continue;
			}

			if (iMinPrice >= pstTradeSell->Moeny)
			{
				iMinPrice = pstTradeSell->Moeny;
				iIdx = j;
			}
		}

		if (0 > iIdx)
		{
			continue;
		}

		pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, iIdx, &iValid);
		if (NULL == pstTradeSell || !iValid )
		{
			continue;
		}

		{
			/* 删除A用户的发布记录 */
			char szCallBack[1024];
			int iLen = 0;
			TRADEPLAYERINFO stTradePlayerInfo;
			CSSHOPPINGTRADEREQ stShoppingTradeReq;
						
			memset(szCallBack,0,sizeof(szCallBack));
			stShoppingTradeReq.TradeID = pstTradeSell->TradeID;
			stShoppingTradeReq.TradeType = 0;
			shopping_tradeplayerinfo_init(NULL,&stTradePlayerInfo);
			memcpy(szCallBack+iLen, &stShoppingTradeReq,sizeof(CSSHOPPINGTRADEREQ));
			iLen += sizeof(CSSHOPPINGTRADEREQ);
			memcpy(szCallBack+iLen,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));
			iLen += sizeof(TRADEPLAYERINFO);
			shopping_db_tradesell_delete_req(pstEnv,-1, pstTradeSell,OPERATE_BY_SYS_TRADE,
								szCallBack,iLen);
			
			/* 把内存数据删除掉 */
			z_delete_tradesell(pstEnv,pstTradeSell->TradeID);

			pstSysShop->BuyGold[i].NextTime = pstSysShop->BuyGold[i].InterVal+tCurr;
			pstSysShop->BuyGold[i].RemNum --;

			/*if (pstSysShop->BuyGold[i].RemNum == 0)
			{
				pstSysShop->iBuyGold --;
				if (pstSysShop->iBuyGold != i)
				{
					pstSysShop->BuyGold[i] = pstSysShop->BuyGold[pstSysShop->iBuyGold];		
				}
			}*/
		}	
	}

	return 0;
}

int shop_buy_tip(ZONESVRENV* pstEnv)
{
	int i;
	int iCurr = pstEnv->pstAppCtx->stCurr.tv_sec;
	int iValid;
	TRADEBUY *pstTradeBuy = NULL;
	int iGoldRate = 1000;
	GLOBAGOLDRATE *pstGoldRate = &pstEnv->pstGlobalObj->stGlobal.GoldRate;
	int iN = 0;

	if (iCurr - pstEnv->iShopBuyTipTime < 30*60)
	{
		return 0;
	}

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat == GLOBAL_FINI)
	{
		iGoldRate = pstGoldRate->GoldRate;
	}

	for(  i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iItem; i++ ) 
	{					
		pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
		if( NULL == pstTradeBuy || 0 == iValid)
		{
			continue;
		}

		if ((int)(pstTradeBuy->Moeny/pstTradeBuy->Gold) > iGoldRate)
		{
			iN++;
		}
	}

	if (iN > 0)
	{
		z_sys_strf_broadcast(pstEnv, SYS_MIDDLE2, ZONEERR_CLANPET2, iN);
	}

	pstEnv->iShopBuyTipTime = iCurr;
	return 0;
}

int shop_chg_rolename(ZONESVRENV* pstEnv, tdr_ulonglong ullRoleID, const char* pszNewName)
{
	int i;
	TRADESELL *pstTradeSell = NULL;
	TRADEBUY *pstTradeBuy = NULL;
	int iValid;

	for(  i=0; i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++ ) 
	{
		pstTradeSell = (TRADESELL *)sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
		if (NULL == pstTradeSell || !iValid )
		{
			continue;
		}

		if (pstTradeSell->RoleID == ullRoleID && 0 != pstTradeSell->RoleName[0])
		{
			STRNCPY(pstTradeSell->RoleName, pszNewName, sizeof(pstTradeSell->RoleName));
		}
	}

	for(  i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++ ) 
	{
		pstTradeBuy = (TRADEBUY *)sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
		if (NULL == pstTradeBuy || !iValid )
		{
			continue;
		}

		if (pstTradeBuy->RoleID == ullRoleID && 0 != pstTradeBuy->RoleName[0])
		{
			STRNCPY(pstTradeBuy->RoleName, pszNewName, sizeof(pstTradeBuy->RoleName));
		}
	}
	
	return 0;
}

int z_shopdef_tick(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,unsigned int uiPreScanNum)
{
	unsigned int i =0;
	int iValid = 0;
	ShopLimitBucket *pstShopLimitBucket;
	SHOPLIMITDEF *pstShopLimitDef = NULL;
	SHOPLIMITDEF astShopLimitDef[MAX_SHOP_DEF_LINES];
	int iModifyNum = 0;
	//int  tTime = pstAppCtx->stCurr.tv_sec;

	memset(&astShopLimitDef[0], 0, sizeof(astShopLimitDef));
	for (i=0; i<uiPreScanNum; i++)
	{		
		g_uiShopLimitDefScanNum = g_uiShopLimitDefScanNum%pstEnv->pstShopLimitDefHash->iMax;
		
		pstShopLimitBucket = (ShopLimitBucket *)sht_pos(pstEnv->pstShopLimitDefHash, g_uiShopLimitDefScanNum, &iValid);
		if (NULL == pstShopLimitBucket || !iValid )
		{
			g_uiShopLimitDefScanNum++;
			continue;
		}
		pstShopLimitDef = (SHOPLIMITDEF *)pstShopLimitBucket->szData;
				
		g_uiShopLimitDefScanNum++;
		shopping_check_limitdef( pstAppCtx,pstEnv,	pstShopLimitDef);
				
		// 回写过或不需要回写的直接跳过
		if( !(pstShopLimitDef->ModifyFlag & SHOPLIMIT_FLAG_MODIFY) )
		{
			continue;
		}

		pstShopLimitDef->ModifyFlag &= ~SHOPLIMIT_FLAG_MODIFY;
		
		//回写
		shopping_db_shoplimitdef_update_req( pstEnv, pstShopLimitDef);
		
		memcpy(&astShopLimitDef[iModifyNum],pstShopLimitDef,sizeof(SHOPLIMITDEF));
		iModifyNum++;
		
	}
	
	if( (iModifyNum > 0) && (uiPreScanNum != MAX_SHOP_DEF_LINES) )
	{
		 shopping_modify_limit_res( pstAppCtx,pstEnv,NULL,astShopLimitDef, iModifyNum);
	}

	return 0;
}

#if 0
//每10分钟拉取下大区处的相关配置
int shopping_ss_query_propsdef_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv)
{
	SSBILLQUERYPROPSDEFREQ stQueryPropsDef;
	
	memset(&stQueryPropsDef,0,sizeof(stQueryPropsDef));

	stQueryPropsDef.OperType = 0;
	
	shopping_ss_send(pstAppCtx,pstEnv,  BILL_QUERY_PROPSDEF_REQ,(char *)&stQueryPropsDef,sizeof(SSBILLQUERYPROPSDEFREQ));

	return 0;
}


int shopping_ss_query_propsdef_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSBILLQUERYPROPSDEFRES *pstBillQueryPropsDefRes)
{	
	UNUSED(pstAppCtx);
	// 把这个数据存储到zone_svr上
	memcpy(pstEnv->PropsDef, pstBillQueryPropsDefRes->PropsDef,NEW_PROPSDEF_RECORD_MAX*sizeof(PROPSDEF));

	return 0;
}



//客户段查询配置定义
int shopping_query_propsdef_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer,CSSHOPPINGQUERYPROPSDEFREQ *pstShoppingQueryPropsDefReq)
{
	CSPKG stPkgRes;
	CSSHOPPINGQUERYPROPSDEFRES *pstShoppingQueryPropsDefRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryPropsDefRes;
	UNUSED(pstAppCtx);
	UNUSED(pstShoppingQueryPropsDefReq);
	
	
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_PROPSDEF_RES;
	memset(pstShoppingQueryPropsDefRes,0,sizeof(CSSHOPPINGQUERYPROPSDEFRES));

	//拷贝配置信息下发
	memcpy( pstShoppingQueryPropsDefRes->PropsDef,pstEnv->PropsDef,NEW_PROPSDEF_RECORD_MAX*sizeof(PROPSDEF));
	
	/* 下发数据包 */
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);

	return 0;
}


int shopping_check_propsgift(ZONESVRENV* pstEnv,PROPSGIFT *pstPropsGift)
{
	int i=0;
	ITEMDEF *pstItemDef=NULL;
	
	for(i=0;i<PROPS_GIFT_ITEM_NUM;i++)
	{
		pstItemDef = z_find_itemdef( pstEnv, pstPropsGift->PropsGiftItem[i].ItemID );
		if( pstItemDef == NULL )
		{
			continue;
		}

		if(ITEM_TYPE_ARM == pstItemDef->ItemType)
		{
			pstPropsGift->PropsGiftItem[i].ItemNum = 1;
		}
		else
		{
			if( pstItemDef->MaxPile > 1 && pstPropsGift->PropsGiftItem[i].ItemNum > pstItemDef->MaxPile)
			{
				pstPropsGift->PropsGiftItem[i].ItemNum = pstItemDef->MaxPile;
			}
		}
	}
	return 0;
}

int shopping_query_propsgift_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer,CSSHOPPINGQUERYPROPSGIFTREQ *pstShoppingQueryPropsGiftReq )
{
	SSBILLQUERYPROPSGIFTREQ stQueryPropsGift;

	UNUSED(pstShoppingQueryPropsGiftReq);
	memset(&stQueryPropsGift,0,sizeof(stQueryPropsGift));

	STRNCPY(stQueryPropsGift.Pass9Account, pstPlayer->szAccount,CCH(stQueryPropsGift.Pass9Account));
	stQueryPropsGift.Uin = pstPlayer->stRoleData.Uin;
	
	shopping_ss_send(pstAppCtx,pstEnv,  BILL_QUERY_PROPSGIFT_REQ,(char *)&stQueryPropsGift,sizeof(SSBILLQUERYPROPSGIFTREQ));

	return 0;
}


int shopping_query_propsgift_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSBILLQUERYPROPSGIFTRES *pstSSBillQueryPropsGiftRes)
{	
 	Player *pstPlayer = NULL;
	CSPKG stPkgRes;
	CSSHOPPINGQUERYPROPSGIFTRES *pstShoppingQueryPropsGiftRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingQueryPropsGiftRes;
	UNUSED(pstAppCtx);
	
	pstPlayer = player_get_by_uin(pstEnv, pstSSBillQueryPropsGiftRes->Uin);
	if (!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN )
	{
		return -1;
	}

	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_QUERY_PROPSGIFT_RES;
	memset(pstShoppingQueryPropsGiftRes,0,sizeof(CSSHOPPINGQUERYPROPSGIFTRES));
	
	pstShoppingQueryPropsGiftRes->PropsGift = pstSSBillQueryPropsGiftRes->PropsGift;	
	shopping_check_propsgift( pstEnv,&pstShoppingQueryPropsGiftRes->PropsGift);
	/* 下发数据包 */
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	return 0;
}

int shopping_set_propsgift_req(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,Player *pstPlayer,CSSHOPPINGSETPROPSGIFTREQ* pstShoppingSetPropsGiftReq )
{
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pstPlayer);
	UNUSED(pstShoppingSetPropsGiftReq);
	
	/*int iNum = 0;
	SSBILLSETPROPSGIFTREQ stBillSetPropsGiftReq;
	ROLEGRID		stRoleGrid;
	PROPSDEF *pstPropsDef = NULL;
	int iPropsItemNum=0;

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}
	
	memset(&stBillSetPropsGiftReq,0,sizeof(SSBILLSETPROPSGIFTREQ));

	STRNCPY(stBillSetPropsGiftReq.Pass9Account, pstPlayer->szAccount,CCH(stBillSetPropsGiftReq.Pass9Account));
	stBillSetPropsGiftReq.Uin = pstPlayer->stRoleData.Uin;
	stBillSetPropsGiftReq.PropsID = pstShoppingSetPropsGiftReq->PropsID;
	stBillSetPropsGiftReq.Index = pstShoppingSetPropsGiftReq->Index;
	stBillSetPropsGiftReq.GetType = pstShoppingSetPropsGiftReq->GetType;
	stBillSetPropsGiftReq.GiftItem = pstShoppingSetPropsGiftReq->GiftItem;


	if( (pstShoppingSetPropsGiftReq->GetType == 1 &&
		(pstShoppingSetPropsGiftReq->Index<0 || pstShoppingSetPropsGiftReq->Index>=PROPS_LIMIT_NUM )) ||
		 (pstShoppingSetPropsGiftReq->GetType == 2 &&
		(pstShoppingSetPropsGiftReq->Index<0 || pstShoppingSetPropsGiftReq->Index>=PROPS_GIFT_ITEM_NUM)) )
	{
		z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "参数错误,获取礼包失败!");
		return -1;
	}
	
	//先要有三个格子才能领取
	iNum = package_get_freegrid_num(pstAppCtx, pstEnv, pstPlayer, LIST_TYPE_SUNDRIES);
	if(pstShoppingSetPropsGiftReq->GetType == 2 )
	{
		if(iNum < 1)
		{
			z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "背包空间不足,请先整理背包!");
			return -1;
		}
	}
	else 
	{
		pstPropsDef = z_find_propsdef( pstEnv, pstShoppingSetPropsGiftReq->PropsID);
		if( !pstPropsDef )
		{
			z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "获取礼包失败!");
			return -1;
		}	
		if(pstPropsDef->PropsLimit[pstShoppingSetPropsGiftReq->Index].ItemID1>0)
		{
			iPropsItemNum++;
		}
		if(pstPropsDef->PropsLimit[pstShoppingSetPropsGiftReq->Index].ItemID2>0)
		{
			iPropsItemNum++;
		}
		if(pstPropsDef->PropsLimit[pstShoppingSetPropsGiftReq->Index].ItemID3>0)
		{
			iPropsItemNum++;
		}
		
		if(iPropsItemNum > iNum )
		{
			z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "背包空间不足,请整理背包");
			return -1;
		}
	}
	
	if(pstShoppingSetPropsGiftReq->GetType == 2 )
	{
		memset(&stRoleGrid,0,sizeof(stRoleGrid));
		if (0 > item_create(pstEnv->pstAppCtx, pstEnv, &stRoleGrid, pstShoppingSetPropsGiftReq->GiftItem.ItemID))
		{
			z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "获取礼包失败!");
			return -1;
		}

		if (GRID_ITEM_ITEM == stRoleGrid.Type)
		{
			stRoleGrid.GridData.RoleItem.Num = pstShoppingSetPropsGiftReq->GiftItem.ItemNum;
		}
		else if( pstShoppingSetPropsGiftReq->GiftItem.ItemNum > iNum )
		{
			z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "背包空间不足,请先整理背包!");
			return -1;
		}

		if (0 > package_add_test(pstEnv->pstAppCtx, pstEnv, pstPlayer, &stRoleGrid, 1,0))
		{
			z_send_sys_str(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, "背包空间不足,请先整理背包!");
			return -1;
		}
	}

	
	shopping_ss_send(pstAppCtx,pstEnv,  BILL_SET_PROPSGIFT_REQ,(char *)&stBillSetPropsGiftReq,sizeof(SSBILLSETPROPSGIFTREQ));*/

	return 0;
}
int shopping_set_propsgift_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SSBILLSETPROPSGIFTRES *pstSSBillSetPropsGiftRes)
{	
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
	UNUSED(pstSSBillSetPropsGiftRes);
 	/*Player *pstPlayer = NULL;
	CSPKG stPkgRes;
	CSSHOPPINGSETPROPSGIFTRES *pstShoppingSetPropsGiftRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingSetPropsGiftRes;
	PROPSDEF *pstPropsDef = NULL;
	ROLEGRID stRoleGrid[3];
	unsigned int iItemID = 0;
	unsigned int iItemNum = 0;
	unsigned int iCount = 0;
	int iRes = 0;
	SSBILLSETPROPSGIFTREQ *pstBillSetPropsGiftReq=NULL;
	ITEMDEF *pstItemDef=NULL;
	UNUSED(pstAppCtx);
	
	pstPlayer = player_get_by_uin(pstEnv, pstSSBillSetPropsGiftRes->Uin);
	if (!pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN )
	{
		z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "领取礼包失败");
		return -1;
	}

	if(pstSSBillSetPropsGiftRes->Result == 2)
	{
		z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "该礼包不能重复领取!");
		goto error;
	}
	else if(pstSSBillSetPropsGiftRes->Result != 0)
	{
		z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "领取礼包失败");
		goto error;
	}
	pstBillSetPropsGiftReq = &pstSSBillSetPropsGiftRes->BillSetPropsGiftReq;
	
	if( (pstBillSetPropsGiftReq->GetType == 1 &&
		(pstBillSetPropsGiftReq->Index<0 || pstBillSetPropsGiftReq->Index>=PROPS_LIMIT_NUM )) ||
		( pstBillSetPropsGiftReq->GetType == 2 &&
		(pstBillSetPropsGiftReq->Index<0 || pstBillSetPropsGiftReq->Index>=PROPS_GIFT_ITEM_NUM)) )
	{
		z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "领取礼包失败");
		goto error;
	}
	if(pstBillSetPropsGiftReq->GetType == 1)
	{
		pstPropsDef = z_find_propsdef( pstEnv, pstBillSetPropsGiftReq->PropsID);
		if(!pstPropsDef )
		{
			z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "领取礼包失败");
			goto error;
		}
	
		//加物品
		iItemID = pstPropsDef->PropsLimit[pstBillSetPropsGiftReq->Index].ItemID1;
		iItemNum = pstPropsDef->PropsLimit[pstBillSetPropsGiftReq->Index].ItemNum1;
		if( 0 != iItemID  && 
			0 != iItemNum )
		{
			if (0 == item_create(pstAppCtx, pstEnv, &stRoleGrid[iCount], iItemID ))
			{
				if (GRID_ITEM_ITEM == stRoleGrid[iCount].Type)
				{
					pstItemDef = z_find_itemdef( pstEnv, iItemID);					
					if(pstItemDef && pstItemDef->MaxPile && iItemNum > pstItemDef->MaxPile)
					{
					       //直接写死 不支持大于每个格子的叠加数
						iItemNum = pstItemDef->MaxPile;
					}
					stRoleGrid[iCount].GridData.RoleItem.Num = iItemNum;
				}
				iCount++;
			}
		}
		iItemID = pstPropsDef->PropsLimit[pstBillSetPropsGiftReq->Index].ItemID2;
		iItemNum = pstPropsDef->PropsLimit[pstBillSetPropsGiftReq->Index].ItemNum2;
		if( 0 != iItemID  && 
			0 != iItemNum )
		{
			if (0 == item_create(pstAppCtx, pstEnv, &stRoleGrid[iCount], iItemID ))
			{
				if (GRID_ITEM_ITEM == stRoleGrid[iCount].Type)
				{
					pstItemDef = z_find_itemdef( pstEnv, iItemID);					
					if(pstItemDef && pstItemDef->MaxPile && iItemNum > pstItemDef->MaxPile)
					{
					       //直接写死 不支持大于每个格子的叠加数
						iItemNum = pstItemDef->MaxPile;
					}
					stRoleGrid[iCount].GridData.RoleItem.Num = iItemNum;
				}
				iCount++;
			}
		}
		iItemID = pstPropsDef->PropsLimit[pstBillSetPropsGiftReq->Index].ItemID3;
		iItemNum = pstPropsDef->PropsLimit[pstBillSetPropsGiftReq->Index].ItemNum3;
		if( 0 != iItemID  && 
			0 != iItemNum )
		{
			if (0 == item_create(pstAppCtx, pstEnv, &stRoleGrid[iCount], iItemID ))
			{
				if (GRID_ITEM_ITEM == stRoleGrid[iCount].Type)
				{
					pstItemDef = z_find_itemdef( pstEnv, iItemID);					
					if(pstItemDef && pstItemDef->MaxPile && iItemNum > pstItemDef->MaxPile)
					{
					       //直接写死 不支持大于每个格子的叠加数
						iItemNum = pstItemDef->MaxPile;
					}
					stRoleGrid[iCount].GridData.RoleItem.Num = iItemNum;
				}
				iCount++;
			}
		}
	}
	else if(pstBillSetPropsGiftReq->GetType == 2)
	{
		iItemID = pstBillSetPropsGiftReq->GiftItem.ItemID;
		iItemNum = pstBillSetPropsGiftReq->GiftItem.ItemNum;
		if( 0 != iItemID  && 
			0 != iItemNum )
		{
			if (0 == item_create(pstAppCtx, pstEnv, &stRoleGrid[iCount], iItemID ))
			{
				if (GRID_ITEM_ITEM == stRoleGrid[iCount].Type)
				{
					pstItemDef = z_find_itemdef( pstEnv, iItemID);					
					if(pstItemDef && pstItemDef->MaxPile && iItemNum > pstItemDef->MaxPile)
					{
					       //直接写死 不支持大于每个格子的叠加数
						iItemNum = pstItemDef->MaxPile;
					}
					stRoleGrid[iCount].GridData.RoleItem.Num = iItemNum;
				}
				iCount++;
			}
		}
	}

	iRes=package_add_test(pstAppCtx, pstEnv, pstPlayer, &stRoleGrid[0], iCount,0);
	if (iRes == -1)
	{
		char Title[256];
		z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "领取礼包成功,请查收邮件");
		//失败了用邮件送
		memset(Title,0,sizeof(Title));
		snprintf(Title,sizeof(Title),"领取礼包成功,快查收吧!");
		player_sysmail_fast_send( pstEnv, pstPlayer->stRoleData.RoleName, Title,
							 "领取冲值礼包成功", &stRoleGrid[0], iCount,
							 0,NULL,0);	
		
		return 0;
	}
	

	package_add(pstAppCtx , pstEnv , pstPlayer, &stRoleGrid[0], iCount,0);
	z_send_sys_str( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, "领取冲值礼包成功");
	
final:
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_SET_PROPSGIFT_RES;
	memset(pstShoppingSetPropsGiftRes,0,sizeof(CSSHOPPINGSETPROPSGIFTRES));
	
	pstShoppingSetPropsGiftRes->PropsGift = pstSSBillSetPropsGiftRes->PropsGift;
	pstShoppingSetPropsGiftRes->PropsID = pstSSBillSetPropsGiftRes->PropsID;
	pstShoppingSetPropsGiftRes->Result = pstSSBillSetPropsGiftRes->Result;
	
	shopping_check_propsgift( pstEnv,&pstShoppingSetPropsGiftRes->PropsGift);
	
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	return 0;
error:	
	goto final;*/
	return 0;
}

#endif

int shopping_add_bulletin(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,int iActiveTime,int iRollInterval, char *szText)
{
	int iID = 0;
	SSBULLETIN stReq;
	UNUSED(pstAppCtx);
	
	memset(&stReq,0,sizeof(stReq));	
	
	iID = z_gen_bulletin_id();
	stReq.ActiveTime = iActiveTime;
	stReq.ActiveTimeStart = 0;
	stReq.Core.ID = iID;
	stReq.Core.RollInterval = iRollInterval;
	STRNCPY(stReq.Core.Text, szText, CCH(stReq.Core.Text));

	if(z_add_bulletin(pstEnv, &stReq) < 0)
	{
		return -1;
	}

	return 0;
}
int shopping_limit_num_add_bulletin(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, SHOPLIMITDEF *pstShopLimitDef,int iSellNum)
{
	//char szText[256];
	//SHOPDEF *pstShopDef=NULL;
	//ITEMDEF *pstItemDef = NULL;

	return 0;
	
	/*memset(szText,0,sizeof(szText));
	

	pstShopDef =  z_find_shopdef( pstEnv, pstShopLimitDef->ShopID);
	if(!pstShopDef )
	{
		return -1;
	}
	pstItemDef = z_find_itemdef( pstEnv, pstShopDef->ItemID);
	if(!pstItemDef )
	{
		return -1;
	}
	
	if(pstShopLimitDef->LimitSelledNum >= pstShopLimitDef->LimitSellTotalNum)
	{
		snprintf(szText,sizeof(szText),
		"敬爱的各位玩家，[#L<%s>@@%u#n]本次热销已经全部售罄，敬请期待下次！",
		pstItemDef->Name,
		pstItemDef->ItemID
		);
		shopping_add_bulletin(pstAppCtx,pstEnv,SHOP_BULLETIN_TIME,SHOP_BULLETIN_TIME, szText);
	}
	else if(pstShopLimitDef->LimitSelledNum-iSellNum < pstShopLimitDef->LimitSellTotalNum/2 &&
		        pstShopLimitDef->LimitSelledNum >= (pstShopLimitDef->LimitSellTotalNum/2) )
	{
		snprintf(szText,sizeof(szText),
		"敬爱的各位玩家，[#L<%s>@@%u#n]销售异常火爆，仅剩下%d个，请需要购买的玩家迅速抢货！",
		pstItemDef->Name,
		pstItemDef->ItemID,
		abs(pstShopLimitDef->LimitSellTotalNum-pstShopLimitDef->LimitSelledNum)
		);
		shopping_add_bulletin(pstAppCtx,pstEnv,SHOP_BULLETIN_TIME,SHOP_BULLETIN_TIME, szText);
	}*/
	
	return 0;
}
//兑换点券
int shopping_exchange_point_req(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv, Player *pstPlayer, CSSHOPPINGEXCHANGEREQ *pstShoppingExchangeReq)
{
	unsigned int uiGodCoin = 0;
	unsigned long long ullGodCoin = 0;

	pstShoppingExchangeReq->RoleLevel = pstPlayer->stRoleData.Level;
	
	uiGodCoin = pstShoppingExchangeReq->PointNum;
	ullGodCoin = pstShoppingExchangeReq->PointNum;
	ullGodCoin =  ullGodCoin;

	if (player_check_safelock(pstEnv, pstPlayer) < 0)
	{
		return -1;
	}

	if(pstShoppingExchangeReq->PointNum <= 0 ||
		uiGodCoin != ullGodCoin)
	{
		z_send_sys_str_msg( pstAppCtx,  pstEnv,  pstPlayer, SYS_TIP, ZONEERR_SHOP30);
		return -1;
	}
	pstShoppingExchangeReq->RoleID = pstPlayer->stRoleData.RoleID;
	memset(pstShoppingExchangeReq->RoleName,0,sizeof(pstShoppingExchangeReq->RoleName));
	STRNCPY(pstShoppingExchangeReq->RoleName, pstPlayer->stRoleData.RoleName, 
						CCH(pstShoppingExchangeReq->RoleName));
	
	shopping_operate_point_req(pstAppCtx,pstEnv,pstPlayer->szAccount,pstPlayer->stRoleData.Uin,
									pstShoppingExchangeReq->PointNum,	pstShoppingExchangeReq,sizeof(CSSHOPPINGEXCHANGEREQ),
									DATATYPE_POINT_BY_EXCHANGE,OPERATE_POINT_DEC );

	return 0;
}

int shopping_exchg_award(ZONESVRENV* pstEnv, char *pszName, int iGoldNum)
{
	GOLDAWARD *pstAward = &pstEnv->pstConf->GoldAward;
	int iNum = 0;
	ROLEGRID stGrid[MAIL_MAX_ITEM];
	ITEMDEF *pstDef;
	char szMsg[64];
	int i;
	int iGird = 0;
	int iMaxPile;
	Player *pstPlayer;

	if (pstAward->ItemID <=0 || pstAward->GoldNum <= 0)
	{
		return 0;
	}

	iNum = iGoldNum/pstAward->GoldNum;
	if (iNum <= 0)
		return 0;

	pstDef = z_find_itemdef(pstEnv, pstAward->ItemID);
	if (!pstDef)
		return 0;

	if (pstDef->MaxPile<=1)
	{
		iMaxPile = 1;
	}
	else
	{
		iMaxPile = pstDef->MaxPile;
	}

	snprintf(szMsg,sizeof(szMsg),LJY_MAIL66,iGoldNum,iNum, pstDef->Name);
	while(iNum > 0)
	{
		iGird = 0;
		for (i=0; i<MAIL_MAX_ITEM; i++)
		{
			if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGrid[iGird], pstAward->ItemID ) )
			{
				if (pstAward->Bind)
					stGrid[iGird].GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
			}
			else
			{
				return 0;
			}
		
			if (iNum >= iMaxPile)
			{
				stGrid[iGird].GridData.RoleItem.Num = iMaxPile;
				iNum -= iMaxPile;
			}	
			else
			{
				stGrid[iGird].GridData.RoleItem.Num  = iNum;
				iNum = 0;
			}

			iGird++;

			if (iNum <= 0)
			{
				break;
			}
		}
		

		if (iGird > 0)
			player_sysmail_fast_send(pstEnv, pszName, LJY_MAIL65,
									szMsg,&stGrid[0], iGird, 0,NULL, 0);
	}

	pstPlayer = z_name_player(pstEnv->pstAppCtx, pstEnv, pszName);
	if (pstPlayer)
	{
		z_send_sys_str_msg(pstEnv->pstAppCtx, pstEnv, pstPlayer, SYS_MIDDLE2, ZONEERR_DIANFENG22);
	}
	
/*	if (0 == item_create(pstEnv->pstAppCtx, pstEnv, &stGrid, pstAward->ItemID ) )
	{
		if (pstAward->Bind)
			stGrid.GridData.RoleItem.InstFlag |= INST_ITEM_BIND;
		stGrid.GridData.RoleItem.Num = iNum;
	}
	else
	{
		return 0;
	}

	snprintf(szMsg,sizeof(szMsg),LJY_MAIL66,iGoldNum,iNum, pstDef->Name);

	player_sysmail_fast_send(pstEnv, pszName, LJY_MAIL65,
									szMsg,&stGrid, 1, 0,NULL, 0);*/
	return 0;
}

int shopping_exchange_point_res(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv, SSDECTIANYUANRES *pstDecTianYuanRes)
{
	CSPKG stPkgRes;
	Player *pstPlayer = NULL;
	CSSHOPPINGEXCHANGEREQ *pstShoppingExchangeReq = NULL;
	CSSHOPPINGEXCHANGERES *pstShoppingExchangeRes = NULL;
	int iOffset = 0;
	unsigned int uiGodCoin = 0;
	char szText[512];
	char szTitle[512];
	
	stPkgRes.Body.ShoppingSvr.Type = SHOPPING_EXCHANGE_POINT_RES;
	pstShoppingExchangeRes = &stPkgRes.Body.ShoppingSvr.ShoppingResData.ShoppingExchangeRes;
	memset(pstShoppingExchangeRes,0,sizeof(CSSHOPPINGEXCHANGERES));

	pstPlayer = player_get_by_uin(pstEnv, pstDecTianYuanRes->Uin);
	if (pstDecTianYuanRes->Result != 0)
	{
		if (NULL == pstPlayer)
		{
			return -1;
		}
		
		pstShoppingExchangeRes->Result = pstDecTianYuanRes->Result;

		if (BILL_DEC_NO_ENOUGH_CASH == pstShoppingExchangeRes->Result ||
			BILL_DEC_NO_CASH == pstDecTianYuanRes->Result)
		{
			
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP39);
			pstShoppingExchangeRes->LeftPoint = pstDecTianYuanRes->BillRecord.Points;
		}
		else
		{
			z_send_sys_str_msg(pstAppCtx, pstEnv, pstPlayer, SYS_TIP, ZONEERR_SHOP40);
		}
		
		goto error;
	}

	pstShoppingExchangeRes->LeftPoint = pstDecTianYuanRes->BillRecord.Points;
	
	pstShoppingExchangeReq = (CSSHOPPINGEXCHANGEREQ *)(pstDecTianYuanRes->ExtraData.ExtraData+iOffset);
	uiGodCoin = pstShoppingExchangeReq->PointNum;

	shopping_exchg_award(pstEnv, pstShoppingExchangeReq->RoleName, uiGodCoin);
	
	if ( !pstPlayer || pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN || 
		 pstPlayer->stRoleData.RoleID != pstShoppingExchangeReq->RoleID ||
		 0 > package_add_money_test(pstEnv, pstPlayer, ATTR_ID_GODCOIN, uiGodCoin))
	{
		if( 0 == pstDecTianYuanRes->Result )
		{
			//邮件自己加金币
			memset(szText,0,sizeof(szText));	
			snprintf(szText,sizeof(szText),LJY_MAIL40,
							pstShoppingExchangeReq->PointNum,
							MONEY_TYPE_POINTCOIN_NAME,
							uiGodCoin, 
							MONEY_TYPE_GODCOIN_NAME);
			memset(szTitle,0,sizeof(szTitle));	
			snprintf(szTitle,sizeof(szTitle),LJY_MAIL41, MONEY_TYPE_POINTCOIN_NAME);
			
			player_sysmail_fast_send(pstEnv,pstShoppingExchangeReq->RoleName,
				szTitle, szText,NULL,0, uiGodCoin,NULL,1);

//			z_role_shopping_exchange_oplog(pstEnv, pstShoppingExchangeReq->RoleID, pstShoppingExchangeReq->RoleName,
//								pstDecTianYuanRes->BillRecord.Pass9Account,pstDecTianYuanRes->Uin,0,
//								pstShoppingExchangeReq->PointNum,uiGodCoin,uiGodCoin,0);

			z_role_shopping_exchange_oplog(pstEnv, pstShoppingExchangeReq->RoleID, pstShoppingExchangeReq->RoleName,
								pstDecTianYuanRes->BillRecord.Pass9Account,pstDecTianYuanRes->Uin, pstShoppingExchangeReq->RoleLevel,
								pstShoppingExchangeReq->PointNum,uiGodCoin,uiGodCoin,0);
			
		}
		return 0;
	}	

	package_add_money(pstEnv, pstPlayer, ATTR_ID_GODCOIN, uiGodCoin);
	pstShoppingExchangeRes->Result = 0;

	{	
		// 记录下金币提取
		ZoneClan *pstClan = z_id_clan(pstEnv, pstPlayer->stRoleData.MiscInfo.ClanInfo.ClanId.Id);
		if (pstClan)
		{
			pstClan->stClan.ClanBoon.Cond.Gold += uiGodCoin;
			// todo 通知客户端
			clan_boon_condition_chg(pstEnv,pstClan);
		}
	}
	
	z_role_shopping_exchange_oplog(pstEnv, pstShoppingExchangeReq->RoleID, pstShoppingExchangeReq->RoleName,
								pstDecTianYuanRes->BillRecord.Pass9Account,pstDecTianYuanRes->Uin,pstPlayer->stRoleData.Level,
								pstShoppingExchangeReq->PointNum,uiGodCoin,uiGodCoin,0);
error:		
	// 下发数据包 
	Z_CSHEAD_INIT(&stPkgRes.Head, SHOPPING_SVR);
	z_cltmsg_send(pstEnv, pstPlayer, &stPkgRes);
	return 0;
}

//opertype 参考 GODCOIN_OPER_FLAG
int godcoin_consume_log(ZONESVRENV* pstEnv,int Uin,unsigned long long RoleID, int RoleLevel,
								int OperType, int OperSubType, int ShopID,int ShopNum,
								unsigned long long GodCoin,const char *pszStr, ...)
{
	SSGODCOINCONSUMELOGREQ stGodCoinConsumeLogReq;
	va_list valist;
	int iSize=0;

	memset(&stGodCoinConsumeLogReq,0,sizeof(stGodCoinConsumeLogReq));
	iSize = sizeof(stGodCoinConsumeLogReq.OperDesc);
	va_start( valist, pszStr );
	vsnprintf(stGodCoinConsumeLogReq.OperDesc, iSize, pszStr, valist);
	stGodCoinConsumeLogReq.OperDesc[iSize -1] = 0;
	va_end(valist);

	stGodCoinConsumeLogReq.Uin = Uin;
	stGodCoinConsumeLogReq.RoleID = RoleID;
	stGodCoinConsumeLogReq.RoleLevel = RoleLevel;
	stGodCoinConsumeLogReq.WorldID = pstEnv->unWorldID;
	stGodCoinConsumeLogReq.OperType = OperType;
	stGodCoinConsumeLogReq.OperSubType = OperSubType;
	stGodCoinConsumeLogReq.GodCoin = GodCoin;
	stGodCoinConsumeLogReq.ShopID = ShopID;
	stGodCoinConsumeLogReq.ShopNum = ShopNum;
	
	shopping_ss_send( pstEnv->pstAppCtx, pstEnv, GODCOIN_CONSUME_LOG_REQ,(char *)&stGodCoinConsumeLogReq,sizeof(stGodCoinConsumeLogReq));
	return 0;
}

int shopping_battle_buy(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEBUYREQ *pstBattleBuyReq)
{	
	Player *pstPlayer = NULL;
	SSPKG stPkg;	
	SSBATTLEBUYRES *pstBattleBuyRes = &stPkg.Body.BattleBuyRes;
	CSSHOPPINGBUYREQ *pstShoppingBuyReq;
	SHOPDEF *pstShopDef;
	
	pstPlayer = player_get_by_uin(pstEnv, pstBattleBuyReq->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}

	if(pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}

	pstShoppingBuyReq = (CSSHOPPINGBUYREQ *)pstBattleBuyReq->ExtData;
	pstShopDef = (SHOPDEF *)(pstBattleBuyReq->ExtData+sizeof(CSSHOPPINGBUYREQ));
	if( 1 == pstShopDef->BuyType )
	{		
		/* 金币购买 */
		if( 0 > shopping_by_godcoin(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq, pstShopDef) )
		{
			return -1;
		}
	}
	else if( 2 == pstShopDef->BuyType )
	{
		/* 银币购买 */
		if( 0 > shopping_by_bindgodcoin(pstAppCtx,pstEnv, pstPlayer, pstShoppingBuyReq, pstShopDef) )
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	memset(pstBattleBuyRes,0,sizeof(SSBATTLEBUYRES));
	pstBattleBuyRes->Result = 0;
	pstBattleBuyRes->Uin = pstPlayer->stRoleData.Uin;
	pstBattleBuyRes->RoleID = pstPlayer->stRoleData.RoleID;
	pstBattleBuyRes->ExtLen = 0;
	memcpy(pstBattleBuyRes->ExtData+pstBattleBuyRes->ExtLen,pstShoppingBuyReq,sizeof(CSSHOPPINGBUYREQ));
	pstBattleBuyRes->ExtLen += sizeof(CSSHOPPINGBUYREQ);
	memcpy(pstBattleBuyRes->ExtData+pstBattleBuyRes->ExtLen,pstShopDef,sizeof(SHOPDEF));
	pstBattleBuyRes->ExtLen += sizeof(SHOPDEF);

	Z_SSHEAD_INIT(&stPkg.Head, BATTLE_SHOPPING_RES);
	z_ssmsg_send(pstEnv, &stPkg, player_master_busid_get(pstEnv, pstPlayer));
	
	return 0;
}

int shopping_battle_buy_res(TAPPCTX* pstAppCtx,ZONESVRENV* pstEnv,SSBATTLEBUYRES *pstBattleBuyRes)
{
	Player *pstPlayer = NULL;
	CSSHOPPINGBUYREQ *pstShoppingBuyReq;
	SHOPDEF *pstShopDef;
	SHOPLIMITDEF *pstShopLimitDef;
	
	pstPlayer = player_get_by_uin(pstEnv, pstBattleBuyRes->Uin);
	if( pstPlayer == NULL )
	{
		return -1;
	}	
	pstShoppingBuyReq = (CSSHOPPINGBUYREQ *)pstBattleBuyRes->ExtData;
	pstShopDef = (SHOPDEF *)(pstBattleBuyRes->ExtData+sizeof(CSSHOPPINGBUYREQ));

	//　把定义的数据也加回去
	pstShopLimitDef = z_hash_shoplimitdef(pstEnv, pstShopDef->ShopID);
	if(pstShopLimitDef != NULL )
	{
		if(pstShopLimitDef->LimitSellTotalNum >0 )
		{
			pstShopLimitDef->LimitSelledNum += pstShoppingBuyReq->ShoppingNum;
			pstShopLimitDef->ModifyFlag |= SHOPLIMIT_FLAG_MODIFY;
			shopping_modify_limit_res( pstEnv->pstAppCtx,pstEnv,pstPlayer,pstShopLimitDef, 1);

			shopping_limit_num_add_bulletin( pstAppCtx, pstEnv, pstShopLimitDef,pstShoppingBuyReq->ShoppingNum);
		}
	}
	
	return 0;
	UNUSED(pstAppCtx);
	UNUSED(pstEnv);
}

int shopping_delete_user_all_traderecord(TAPPCTX* pstAppCtx, ZONESVRENV* pstEnv,tdr_ulonglong ullRoleID)
{
	int  i = 0;
	TRADESELL *pstTradeSell = NULL;
	TRADEBUY *pstTradeBuy = NULL;
	int iValid;
	
	UNUSED(pstAppCtx);

	for(  i=0; i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++ ) 
	{	
		pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
		if(pstTradeSell == NULL || 0 == iValid)
		{
			continue;
		}

		if( pstTradeSell->RoleID == ullRoleID )
		{
			pstTradeSell->StartTime = 0;
			//做个删除操作
			shopping_db_tradesell_delete_req( pstEnv,0, pstTradeSell,OPERATE_BY_SYSTEM_SCAN,
							pstTradeSell,sizeof(TRADESELL));
			z_delete_tradesell(pstEnv,pstTradeSell->TradeID);
		}	
	}

	for(  i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++ ) 
	{					
		pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
		if( NULL == pstTradeBuy || 0 == iValid)
		{
			continue;
		}

		if( pstTradeBuy->RoleID == ullRoleID )
		{
			pstTradeBuy->StartTime = 0;
			//做个删除操作
			shopping_db_tradebuy_delete_req( pstEnv,0,pstTradeBuy,
					OPERATE_BY_SYSTEM_SCAN,pstTradeBuy,sizeof(TRADEBUY));
			z_delete_tradebuy(pstEnv,pstTradeBuy->TradeID);	
		}	
	}	
	return 0;
}

int gm_trade_do(ZONESVRENV* pstEnv, int iSysID, char *pszOut, int iOut)
{
	int i, iValid;
	GMTRADE *pstGmTrade;
	TRADESELL *pstTradeSell;
	TRADEBUY *pstTradeBuy;

	pstGmTrade = z_find_gmtrade(pstEnv, iSysID);
	if (NULL == pstGmTrade || pstGmTrade->Num <= 0 || pstGmTrade->Price <= 0)
	{
		if (pszOut)
		{
			snprintf(pszOut, iOut, "id 为%d 的交易单定义错误", iSysID);
		}
		return -1;
	}

	if (1 == pstGmTrade->TradeType) //出售
	{
		if (pstGmTrade->MoneyType == ATTR_ID_GODCOIN)
		{
			TRADESELL stTradeSell;

			if( pstEnv->pstZoneObj->pstTradeSellHash->iItem >= MAX_TRADE_RECORD_NUM -10 )
			{
				if (pszOut)
				{
					snprintf(pszOut, iOut, "金币出售单达到上限");
				}
				return -1;
			}
			
			memset(&stTradeSell,0, sizeof(stTradeSell));	
			stTradeSell.Gold = pstGmTrade->Num;
			stTradeSell.StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;

			if (pstGmTrade->Timer == 0)
			{
				stTradeSell.Time = MAX_TRADE_SHOW_TIME*3600;
			}
			else
			{
				stTradeSell.Time = pstGmTrade->Timer*3600;
			}
			
			stTradeSell.Moeny = pstGmTrade->Price;
			stTradeSell.Flag |= TRADE_FLAG_SYS;

			stTradeSell.SysID = pstGmTrade->ID;
						
			shopping_db_tradesell_insert_req(pstEnv, -1, &stTradeSell );	

			if (pszOut)
			{
				snprintf(pszOut, iOut, "操作成功");
			}
		}
		else if (pstGmTrade->MoneyType == ATTR_ID_MONEY)
		{
			TRADEBUY stTradeBuy;

			if( pstEnv->pstZoneObj->pstTradeBuyHash->iItem >= MAX_TRADE_RECORD_NUM -10 )
			{
				if (pszOut)
				{
					snprintf(pszOut, iOut, "银币出售单达到上限");
				}
				
				return -1;
			}
			
			memset(&stTradeBuy,0, sizeof(stTradeBuy));	
			stTradeBuy.Moeny = pstGmTrade->Num;
			if (pstGmTrade->Timer == 0)
			{
				stTradeBuy.Time = MAX_TRADE_SHOW_TIME*3600;
			}
			else
			{
				stTradeBuy.Time = pstGmTrade->Timer*3600;
			}
			stTradeBuy.StartTime = pstEnv->pstAppCtx->stCurr.tv_sec;
			stTradeBuy.Gold = pstGmTrade->Price;
			stTradeBuy.Flag |= TRADE_FLAG_SYS;
			stTradeBuy.SysID = pstGmTrade->ID;

			shopping_db_tradebuy_insert_req(pstEnv, -1, &stTradeBuy, &stTradeBuy, sizeof(stTradeBuy));	

			if (pszOut)
			{
				snprintf(pszOut, iOut, "操作成功");
			}
		}
		else
		{
			return -1;
		}
			
	}
	else if (2 == pstGmTrade->TradeType) //购买
	{
		if (pstGmTrade->MoneyType == ATTR_ID_GODCOIN)
		{
			unsigned int iMinPrice = pstGmTrade->Price;
			int iIdx = -1;
			for (i=0; i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++)
			{
				pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
				if (NULL == pstTradeSell || !iValid || (pstTradeSell->Flag & TRADE_FLAG_SYS) || (int)pstTradeSell->Gold != pstGmTrade->Num)
				{
					continue;
				}

				if (iMinPrice >= pstTradeSell->Moeny)
				{
					iMinPrice = pstTradeSell->Moeny;
					iIdx = i;
				}
			}

			if (0 > iIdx)
			{
				if (pszOut)
				{
					snprintf(pszOut, iOut, "没有合适的金币出售单");
				}
				return -1;
			}

			pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, iIdx, &iValid);
			if (NULL == pstTradeSell || !iValid )
			{
				return -1;
			}

			{
				/* 删除A用户的发布记录 */
				char szCallBack[1024];
				int iLen = 0;
				TRADEPLAYERINFO stTradePlayerInfo;
				CSSHOPPINGTRADEREQ stShoppingTradeReq;
							
				memset(szCallBack,0,sizeof(szCallBack));
				stShoppingTradeReq.TradeID = pstTradeSell->TradeID;
				stShoppingTradeReq.TradeType = 0;
				shopping_tradeplayerinfo_init(NULL,&stTradePlayerInfo);
				memcpy(szCallBack+iLen, &stShoppingTradeReq,sizeof(CSSHOPPINGTRADEREQ));
				iLen += sizeof(CSSHOPPINGTRADEREQ);
				memcpy(szCallBack+iLen,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));
				iLen += sizeof(TRADEPLAYERINFO);
				shopping_db_tradesell_delete_req(pstEnv,-1, pstTradeSell,OPERATE_BY_SYS_TRADE,
									szCallBack,iLen);
				
				/* 把内存数据删除掉 */
				z_delete_tradesell(pstEnv,pstTradeSell->TradeID);

				if (pszOut)
				{
					snprintf(pszOut, iOut, "成功购买金币出售单，银币%u",
						pstTradeSell->Moeny);
				}
			}
		}
		else if (pstGmTrade->MoneyType == ATTR_ID_MONEY)
		{
			unsigned int iMinPrice = pstGmTrade->Price;
			int iIdx = -1;
			for (i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++)
			{
				pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
				if (NULL == pstTradeBuy || !iValid || (pstTradeBuy->Flag & TRADE_FLAG_SYS) || (int)pstTradeBuy->Moeny != pstGmTrade->Num)
				{
					continue;
				}

				if (iMinPrice >= pstTradeBuy->Gold)
				{
					iMinPrice = pstTradeBuy->Gold;
					iIdx = i;
				}
			}

			if (0 > iIdx)
			{
				if (pszOut)
				{
					snprintf(pszOut, iOut, "没有合适的银币出售单");
				}
				return -1;
			}

			pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, iIdx, &iValid);
			if (NULL == pstTradeBuy || !iValid )
			{
				return -1;
			}

			{
				/* 删除A用户的发布记录 */
				char szCallBack[1024];
				int iLen = 0;
				TRADEPLAYERINFO stTradePlayerInfo;
				CSSHOPPINGTRADEREQ stShoppingTradeReq;
							
				memset(szCallBack,0,sizeof(szCallBack));
				stShoppingTradeReq.TradeID = pstTradeBuy->TradeID;
				stShoppingTradeReq.TradeType = 1;
				shopping_tradeplayerinfo_init(NULL,&stTradePlayerInfo);
				memcpy(szCallBack+iLen, &stShoppingTradeReq,sizeof(CSSHOPPINGTRADEREQ));
				iLen += sizeof(CSSHOPPINGTRADEREQ);
				memcpy(szCallBack+iLen,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));
				iLen += sizeof(TRADEPLAYERINFO);
				shopping_db_tradebuy_delete_req(pstEnv,-1, pstTradeBuy,OPERATE_BY_SYS_TRADE,
									szCallBack,iLen);
				
				/* 把内存数据删除掉 */
				z_delete_tradebuy(pstEnv,pstTradeBuy->TradeID);

				if (pszOut)
				{
					snprintf(pszOut, iOut, "成功购买银币出售单，金币%u",
						    pstTradeBuy->Gold);
				}
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

	return 0;
}

int gm_trade_cancel(ZONESVRENV* pstEnv, int iType, tdr_ulonglong TradeID, char *pszOut, int iOut)
{
	int i, iValid;
	TRADESELL *pstTradeSell;
	TRADEBUY *pstTradeBuy;
	int iOutCount = 0;

	if (iType == 0)
	{
		for (i=0; i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++)
		{
			pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
			if (NULL == pstTradeSell || !iValid)
			{
				continue;
			}

			if ((pstTradeSell->Flag & TRADE_FLAG_SYS) && pstTradeSell->TradeID == TradeID)
			{
				if (pszOut && iOut > iOutCount)
				{
					iOutCount += snprintf(pszOut+iOutCount, iOut-iOutCount, "撤销金币出售单(%llu)\n",
						pstTradeSell->TradeID);
				}
				
				shopping_db_tradesell_delete_req( pstEnv,-1, pstTradeSell,OPERATE_BY_USER_DESTROY,NULL,0);
				z_delete_tradesell(pstEnv, pstTradeSell->TradeID);
				return 0;
			}
		}
	}
	else
	{
		for (i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++)
		{
			pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
			if (NULL == pstTradeBuy || !iValid)
			{
				continue;
			}

			if ((pstTradeBuy->Flag & TRADE_FLAG_SYS) && pstTradeBuy->TradeID == TradeID)
			{
				TRADEPLAYERINFO stTradePlayerInfo;
				
				if (pszOut && iOut > iOutCount)
				{
					iOutCount += snprintf(pszOut+iOutCount, iOut-iOutCount, "撤销银币出售单(%llu)\n",
						pstTradeBuy->TradeID);
				}
				
				shopping_tradeplayerinfo_init(NULL, &stTradePlayerInfo);
				shopping_db_tradebuy_delete_req( pstEnv,-1, pstTradeBuy,OPERATE_BY_USER_DESTROY,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));	
				z_delete_tradebuy(pstEnv, pstTradeBuy->TradeID);

				return 0;
			}
		}
	}

	snprintf(pszOut, iOut, "撤销失败，订单号不存在");
	
	return 0;
}

int gm_trade_list(ZONESVRENV* pstEnv, TRADERECORD *pstRecord, int *piCount, char *pszOut, int iOut)
{
	int iCount = 0;
	int i, iValid;
	TRADESELL *pstTradeSell;
	TRADEBUY *pstTradeBuy;
	int iOutCount = 0;

	for (i=0; i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++)
	{
		pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
		if (NULL == pstTradeSell || !iValid)
		{
			continue;
		}

		if (pstTradeSell->Flag & TRADE_FLAG_SYS)
		{
			if (pszOut && iOut > iOutCount)
			{
				iOutCount += snprintf(pszOut+iOutCount, iOut-iOutCount, "系统金币出售单(%llu), Gold %u, Money %u\n",
					pstTradeSell->TradeID, pstTradeSell->Gold, pstTradeSell->Moeny);
			}
			
			if (pstRecord && piCount && iCount < *piCount)
			{
				tradesell_record(pstEnv, pstRecord+iCount, pstTradeSell);
				iCount++;
			}
		}
	}

	for (i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++)
	{
		pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
		if (NULL == pstTradeBuy || !iValid)
		{
			continue;
		}

		if (pstTradeBuy->Flag & TRADE_FLAG_SYS)
		{
			if (pszOut && iOut > iOutCount)
			{
				iOutCount += snprintf(pszOut+iOutCount, iOut-iOutCount, "系统银币出售单(%llu), Money %u, Gold %u\n",
					pstTradeBuy->TradeID, pstTradeBuy->Moeny, pstTradeBuy->Gold);
			}
			
			if (pstRecord && piCount && iCount < *piCount)
			{
				tradebuy_record(pstEnv, pstRecord+iCount, pstTradeBuy);
				iCount++;
			}
		}
	}
	
	if (pstRecord && piCount)
	{
		*piCount = iCount;
	}

	return 0;
}

int gold_rate_data(ZONESVRENV* pstEnv, TRADEBUY *pstTradeBuy)
{
	int i;
	long long llTmp;
	GLOBAGOLDRATE *pstGoldRate = &pstEnv->pstGlobalObj->stGlobal.GoldRate;
	unsigned int uRate = 0;
	unsigned int iOldRate = pstGoldRate->GoldRate;

	if (NULL == pstTradeBuy || pstTradeBuy->Gold == 0 || pstTradeBuy->Moeny == 0)
	{
		return -1;
	}

	if (pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
	{
		return -1;
	}

	uRate = pstTradeBuy->Moeny/pstTradeBuy->Gold;
	if (uRate < MIN_GOLD_RATE || uRate > MAX_GOLD_RATE)
	{
		return -1;
	}

	if (pstGoldRate->GoldRateData.DataNum >= MAX_REAL_TRADE)
	{
		memmove(&pstGoldRate->GoldRateData.GoldTradeDatas[0],
			&pstGoldRate->GoldRateData.GoldTradeDatas[1], 
			(MAX_REAL_TRADE-1)*sizeof(pstGoldRate->GoldRateData.GoldTradeDatas[0]));
		pstGoldRate->GoldRateData.GoldTradeDatas[MAX_REAL_TRADE-1].OneGoldRate = uRate;
	}
	else
	{
		pstGoldRate->GoldRateData.GoldTradeDatas[pstGoldRate->GoldRateData.DataNum].OneGoldRate = uRate;
		pstGoldRate->GoldRateData.DataNum++;
	}

	llTmp = 0;
	for (i=0; i<pstGoldRate->GoldRateData.DataNum; i++)
	{
		llTmp += pstGoldRate->GoldRateData.GoldTradeDatas[i].OneGoldRate;
	}
	
	pstGoldRate->GoldRate = llTmp/pstGoldRate->GoldRateData.DataNum;

	if (iOldRate != pstGoldRate->GoldRate)
	{
		player_gold_rate_notify(pstEnv,NULL, 0);	
	}
	return 0;
}

int gold_rate_get_money(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	int iRand;
	int GoldRateBoxMoney;
	ROLEMISCGOLDRATEBOX *pstGoldRateBox = &pstPlayer->stRoleData.MiscInfo.GoldRateBox;

	if (pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN ||
		pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_FINI)
	{
		return -1;
	}

	if (pstGoldRateBox->GetTime != 0 && 
		IsSameDay(pstGoldRateBox->GetTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		if (pstGoldRateBox->GoldRateBoxMoney > 0)
		{
			return pstGoldRateBox->GoldRateBoxMoney;
		}
	}

	iRand = RAND1(51);
	GoldRateBoxMoney = pstEnv->pstGlobalObj->stGlobal.GoldRate.GoldRate * ((800+iRand)/100.0);
	if ((GoldRateBoxMoney < MIN_GOLD_RATE*8) || (GoldRateBoxMoney > MAX_GOLD_RATE*9))
	{
		return -1;
	}
		
	pstGoldRateBox->GetTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstGoldRateBox->GoldRateBoxMoney = GoldRateBoxMoney;
	
	return pstGoldRateBox->GoldRateBoxMoney;
}

int gold_rate_dec_money(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEMISCGOLDRATEBOX *pstGoldRateBox = &pstPlayer->stRoleData.MiscInfo.GoldRateBox;
	
	if (pstPlayer->eStatus != PLAYER_STATUS_ROLE_LOGIN)
	{
		return -1;
	}
	
	if (0 == pstGoldRateBox->GetTime ||
		0 >= pstGoldRateBox->GoldRateBoxMoney)
	{
		return -1;
	}

	if (0 == IsSameDay(pstGoldRateBox->GetTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return -1;
	}

	if (0 > package_dec_money_test(pstEnv, pstPlayer, ATTR_ID_MONEY, pstGoldRateBox->GoldRateBoxMoney))
	{
		return -2;
	}

	if (0 > package_dec_money(pstEnv, pstPlayer, ATTR_ID_MONEY, pstGoldRateBox->GoldRateBoxMoney))
	{
		return -2;
	}

	z_money_consume_oplog(pstEnv, pstPlayer, MONEY_OPTYPE_GOLDBOX, 0,  0, pstGoldRateBox->GoldRateBoxMoney, "乞丐宝箱");

	if (0 != pstGoldRateBox->DecTime && IsSameDay(pstGoldRateBox->DecTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		if (pstGoldRateBox->Num < 255)
		{
			pstGoldRateBox->Num++;
		}
	}
	else
	{
		pstGoldRateBox->Num = 1;
	}

	pstGoldRateBox->DecTime = pstEnv->pstAppCtx->stCurr.tv_sec;
	pstGoldRateBox->GetTime = 0;
	pstGoldRateBox->GoldRateBoxMoney = 0;
	
	return 0;
}

int gold_rate_get_daynum(ZONESVRENV* pstEnv, Player *pstPlayer)
{
	ROLEMISCGOLDRATEBOX *pstGoldRateBox = &pstPlayer->stRoleData.MiscInfo.GoldRateBox;

	if (0 == pstGoldRateBox->DecTime)
	{
		return 0;
	}

	if (IsSameDay(pstGoldRateBox->DecTime, pstEnv->pstAppCtx->stCurr.tv_sec))
	{
		return pstGoldRateBox->Num;
	}
	
	return 0;
}


int shop_buy_player_trade(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq)
{
	int i, iValid;
	TRADESELL *pstTradeSell;
	TRADEBUY *pstTradeBuy;
	SYSSHOPINFO *pstSysShop = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop;
	
	if (!pstReq->TradeFlag)	// 系统购买金币单
	{
		int iIdx = -1;
		for (i=0; i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++)
		{
			pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
			if (	NULL == pstTradeSell || 
				!iValid || 
				(pstTradeSell->Flag & TRADE_FLAG_SYS) ||
				pstTradeSell->TradeID != pstReq->TradeID)
			{
				continue;
			}
			iIdx = i;
		}

		if (0 > iIdx)
		{
			return -5;
		}

		pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, iIdx, &iValid);
		if (NULL == pstTradeSell || !iValid )
		{
			return -99;
		}

		{
			/* 删除A用户的发布记录 */
			char szCallBack[1024];
			int iLen = 0;
			TRADEPLAYERINFO stTradePlayerInfo;
			CSSHOPPINGTRADEREQ stShoppingTradeReq;
						
			memset(szCallBack,0,sizeof(szCallBack));
			stShoppingTradeReq.TradeID = pstTradeSell->TradeID;
			stShoppingTradeReq.TradeType = 0;
			shopping_tradeplayerinfo_init(NULL,&stTradePlayerInfo);
			memcpy(szCallBack+iLen, &stShoppingTradeReq,sizeof(CSSHOPPINGTRADEREQ));
			iLen += sizeof(CSSHOPPINGTRADEREQ);
			memcpy(szCallBack+iLen,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));
			iLen += sizeof(TRADEPLAYERINFO);
			shopping_db_tradesell_delete_req(pstEnv,-1, pstTradeSell,OPERATE_BY_SYS_TRADE,
								szCallBack,iLen);
			
			/* 把内存数据删除掉 */
			z_delete_tradesell(pstEnv,pstTradeSell->TradeID);
		}
	}
	else 			// 系统购买银币单
	{
		int iIdx = -1;
		for (i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++)
		{
			pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
			if (	NULL == pstTradeBuy || 
				!iValid || 
				(pstTradeBuy->Flag & TRADE_FLAG_SYS) || 
				pstTradeBuy->TradeID != pstReq->TradeID)
			{
				continue;
			}
			iIdx = i;
		}

		if (0 > iIdx)
		{
			return -5;
		}

		pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, iIdx, &iValid);
		if (NULL == pstTradeBuy || !iValid )
		{
			return -99;
		}

		if (pstSysShop->ExchgInfo.Count - pstSysShop->ExchgInfo.OutCount < pstTradeBuy->Gold)
		{
			return -6;
		}

		{
			/* 删除A用户的发布记录 */
			char szCallBack[1024];
			int iLen = 0;
			TRADEPLAYERINFO stTradePlayerInfo;
			CSSHOPPINGTRADEREQ stShoppingTradeReq;
						
			memset(szCallBack,0,sizeof(szCallBack));
			stShoppingTradeReq.TradeID = pstTradeBuy->TradeID;
			stShoppingTradeReq.TradeType = 1;
			shopping_tradeplayerinfo_init(NULL,&stTradePlayerInfo);
			memcpy(szCallBack+iLen, &stShoppingTradeReq,sizeof(CSSHOPPINGTRADEREQ));
			iLen += sizeof(CSSHOPPINGTRADEREQ);
			memcpy(szCallBack+iLen,&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));
			iLen += sizeof(TRADEPLAYERINFO);
			shopping_db_tradebuy_delete_req(pstEnv,-1, pstTradeBuy,OPERATE_BY_SYS_TRADE,
								szCallBack,iLen);
			
			/* 把内存数据删除掉 */
			z_delete_tradebuy(pstEnv,pstTradeBuy->TradeID);
		}
	}
	
	return 0;
}

int shop_cancel_sys_trade(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq)
{
	int i, iValid;
	TRADESELL *pstTradeSell;
	TRADEBUY *pstTradeBuy;
	if (!pstReq->TradeFlag)
	{
		for (i=0; i<pstEnv->pstZoneObj->pstTradeSellHash->iMax; i++)
		{
			pstTradeSell = sht_pos(pstEnv->pstZoneObj->pstTradeSellHash, i, &iValid);
			if (NULL == pstTradeSell || !iValid)
			{
				continue;
			}

			if ((pstTradeSell->Flag & TRADE_FLAG_SYS) && pstTradeSell->TradeID == pstReq->TradeID)
			{
				shopping_db_tradesell_delete_req( pstEnv,-1, pstTradeSell,OPERATE_BY_USER_DESTROY,NULL,0);
				return z_delete_tradesell(pstEnv, pstTradeSell->TradeID);
			}
		}
	}
	else 
	{
		for (i=0; i<pstEnv->pstZoneObj->pstTradeBuyHash->iMax; i++)
		{
			pstTradeBuy = sht_pos(pstEnv->pstZoneObj->pstTradeBuyHash, i, &iValid);
			if (NULL == pstTradeBuy || !iValid)
			{
				continue;
			}

			if ((pstTradeBuy->Flag & TRADE_FLAG_SYS) && pstTradeBuy->TradeID == pstReq->TradeID)
			{
				TRADEPLAYERINFO stTradePlayerInfo;
				
				shopping_tradeplayerinfo_init(NULL, &stTradePlayerInfo);
				shopping_db_tradebuy_delete_req( pstEnv,-1, pstTradeBuy,OPERATE_BY_USER_DESTROY,
													&stTradePlayerInfo,sizeof(TRADEPLAYERINFO));	
				return z_delete_tradebuy(pstEnv, pstTradeBuy->TradeID);
			}
		}
	}

	return -5;
}

int shop_cgi_sell_gold(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq)
{
	SYSSHOPINFO *pstSysShop = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop;
	int i = 0;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (pstReq->ExchgNum < 1 || pstReq->ExchgNum > 100)
	{
		return -7;
	}
	
	if (	pstReq->Num != 10 && 
		pstReq->Num != 50 && 
		pstReq->Num != 100 && 
		pstReq->Num != 500 && 
		pstReq->Num != 1000)
	{
		return -1;
	}

	if (pstSysShop->iSellGold>= SYS_SHOP_MAX)
	{
		return -2;
	}

	for (i=0; i<pstSysShop->iSellGold; i++)
	{
		if (pstSysShop->SellGold[i].Num == pstReq->Num)
		{
			return -3;
		}
	}

	if (pstReq->StartTime < (unsigned int)tCurr )
	{
		pstReq->StartTime  = 	tCurr;
	}
	
	if (pstReq->EndTime < pstReq->StartTime && pstReq->EndTime != 0)
	{
		return -4;
	}
	
	pstSysShop->SellGold[pstSysShop->iSellGold].Num = pstReq->Num;
	pstSysShop->SellGold[pstSysShop->iSellGold].EndTime = pstReq->EndTime;
	pstSysShop->SellGold[pstSysShop->iSellGold].ExchgNum = pstReq->ExchgNum;
	pstSysShop->SellGold[pstSysShop->iSellGold].InterVal = pstReq->InterVal;
	pstSysShop->SellGold[pstSysShop->iSellGold].NextTime = pstReq->StartTime;
	pstSysShop->SellGold[pstSysShop->iSellGold].Price = pstReq->Price;
	pstSysShop->SellGold[pstSysShop->iSellGold].RemNum = pstReq->ExchgNum;
	pstSysShop->SellGold[pstSysShop->iSellGold].StartTime = pstReq->StartTime;
	pstSysShop->SellGold[pstSysShop->iSellGold].Type = 0;
	pstSysShop->iSellGold ++;
	return 0;
}

int shop_cgi_sell_money(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq)
{
	SYSSHOPINFO *pstSysShop = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop;
	int i = 0;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (pstReq->ExchgNum < 1 || pstReq->ExchgNum > 100)
	{
		return -7;
	}
	
	if (	pstReq->Num != 1000 && 
		pstReq->Num != 5000 && 
		pstReq->Num != 10000 && 
		pstReq->Num != 50000 && 
		pstReq->Num != 100000)
	{
		return -1;
	}

	if (pstSysShop->iSellMoney	>= SYS_SHOP_MAX)
	{
		return -2;
	}

	for (i=0; i<pstSysShop->iSellMoney; i++)
	{
		if (pstSysShop->SellMoney[i].Num == pstReq->Num)
		{
			return -3;
		}
	}

	if (pstReq->StartTime < (unsigned int)tCurr )
	{
		pstReq->StartTime  = 	tCurr;
	}
	
	if (pstReq->EndTime < pstReq->StartTime && pstReq->EndTime != 0)
	{
		return -4;
	}

	pstSysShop->SellMoney[pstSysShop->iSellMoney].Num = pstReq->Num;
	pstSysShop->SellMoney[pstSysShop->iSellMoney].EndTime = pstReq->EndTime;
	pstSysShop->SellMoney[pstSysShop->iSellMoney].ExchgNum = pstReq->ExchgNum;
	pstSysShop->SellMoney[pstSysShop->iSellMoney].InterVal = pstReq->InterVal;
	pstSysShop->SellMoney[pstSysShop->iSellMoney].NextTime = pstReq->StartTime;
	pstSysShop->SellMoney[pstSysShop->iSellMoney].Price = pstReq->Price;
	pstSysShop->SellMoney[pstSysShop->iSellMoney].RemNum = pstReq->ExchgNum;
	pstSysShop->SellMoney[pstSysShop->iSellMoney].StartTime = pstReq->StartTime;
	pstSysShop->SellMoney[pstSysShop->iSellMoney].Type = 1;
	pstSysShop->iSellMoney ++;
	return 0;
}


int shop_cgi_buy_money(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq)
{
	SYSSHOPINFO *pstSysShop = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop;
	int i = 0;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (pstReq->ExchgNum < 1 || pstReq->ExchgNum > 100)
	{
		return -7;
	}
	
	if (	pstReq->Num != 1000 && 
		pstReq->Num != 5000 && 
		pstReq->Num != 10000 && 
		pstReq->Num != 50000 && 
		pstReq->Num != 100000)
	{
		return -1;
	}

	if (pstSysShop->iBuyMoney	>= SYS_SHOP_MAX)
	{
		return -2;
	}

	for (i=0; i<pstSysShop->iBuyMoney; i++)
	{
		if (pstSysShop->BuyMoney[i].Num == pstReq->Num)
		{
			return -3;
		}
	}

	if (pstReq->StartTime < (unsigned int)tCurr )
	{
		pstReq->StartTime  = 	tCurr;
	}
	
	if (pstReq->EndTime < pstReq->StartTime && pstReq->EndTime != 0)
	{
		return -4;
	}

	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].Num = pstReq->Num;
	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].EndTime = pstReq->EndTime;
	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].ExchgNum = pstReq->ExchgNum;
	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].InterVal = pstReq->InterVal;
	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].NextTime = pstReq->StartTime;
	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].Price = pstReq->Price;
	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].RemNum = pstReq->ExchgNum;
	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].StartTime = pstReq->StartTime;
	pstSysShop->BuyMoney[pstSysShop->iBuyMoney].Type = 1;
	pstSysShop->iBuyMoney ++;
	return 0;
}

int shop_cgi_buy_gold(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq)
{
	SYSSHOPINFO *pstSysShop = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop;
	int i = 0;
	time_t tCurr = pstEnv->pstAppCtx->stCurr.tv_sec;

	if (pstReq->ExchgNum < 1 || pstReq->ExchgNum > 100)
	{
		return -7;
	}
	
	if (	pstReq->Num != 10 && 
		pstReq->Num != 50 && 
		pstReq->Num != 100 && 
		pstReq->Num != 500 && 
		pstReq->Num != 1000)
	{
		return -1;
	}

	if (pstSysShop->iBuyGold >= SYS_SHOP_MAX)
	{
		return -2;
	}

	for (i=0; i<pstSysShop->iBuyGold; i++)
	{
		if (pstSysShop->BuyGold[i].Num == pstReq->Num)
		{
			return -3;
		}
	}

	if (pstReq->StartTime < (unsigned int)tCurr )
	{
		pstReq->StartTime  = 	tCurr;
	}
	
	if (pstReq->EndTime < pstReq->StartTime && pstReq->EndTime != 0 )
	{
		return -4;
	}

	pstSysShop->BuyGold[pstSysShop->iBuyGold].Num = pstReq->Num;
	pstSysShop->BuyGold[pstSysShop->iBuyGold].EndTime = pstReq->EndTime;
	pstSysShop->BuyGold[pstSysShop->iBuyGold].ExchgNum = pstReq->ExchgNum;
	pstSysShop->BuyGold[pstSysShop->iBuyGold].InterVal = pstReq->InterVal;
	pstSysShop->BuyGold[pstSysShop->iBuyGold].NextTime = pstReq->StartTime;
	pstSysShop->BuyGold[pstSysShop->iBuyGold].Price = pstReq->Price;
	pstSysShop->BuyGold[pstSysShop->iBuyGold].RemNum = pstReq->ExchgNum;
	pstSysShop->BuyGold[pstSysShop->iBuyGold].StartTime = pstReq->StartTime;
	pstSysShop->BuyGold[pstSysShop->iBuyGold].Type = 1;
	pstSysShop->iBuyGold ++;
	return 0;
}


int shop_cgi_remove(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq)
{
	int i;
	SYSSHOPINFO *pstSysShop = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop;
	
	if (pstReq->ReMoveType == 0)
	{
		for (i=0; i<pstSysShop->iSellGold; i++)
		{
			if (pstReq->Num == pstSysShop->SellGold[i].Num)
			{	
				pstSysShop->iSellGold --;
				if (pstSysShop->iSellGold != i)
				{
					pstSysShop->SellGold[i] = pstSysShop->SellGold[pstSysShop->iSellGold];
				}
				return 0;
			}
		}

		if (i >= pstSysShop->iSellGold)
		{
			return -5;
		}
	}
	else if (pstReq->ReMoveType == 1)
	{
		for (i=0; i<pstSysShop->iBuyGold; i++)
		{
			if (pstReq->Num == pstSysShop->BuyGold[i].Num)
			{	
				pstSysShop->iBuyGold --;
				if (pstSysShop->iBuyGold != i)
				{
					pstSysShop->BuyGold[i] = pstSysShop->BuyGold[pstSysShop->iBuyGold];
				}
				return 0;
			}
		}

		if (i >= pstSysShop->iBuyGold)
		{
			return -5;
		}
	}
	else if (pstReq->ReMoveType == 2)
	{
		for (i=0; i<pstSysShop->iSellMoney; i++)
		{
			if (pstReq->Num == pstSysShop->SellMoney[i].Num)
			{	
				pstSysShop->iSellMoney --;
				if (pstSysShop->iSellMoney != i)
				{
					pstSysShop->SellMoney[i] = pstSysShop->SellMoney[pstSysShop->iSellMoney];
				}
				return 0;
			}
		}

		if (i >= pstSysShop->iSellMoney)
		{
			return -5;
		}
	}
	else if (pstReq->ReMoveType == 3)
	{
		for (i=0; i<pstSysShop->iBuyMoney; i++)
		{
			if (pstReq->Num == pstSysShop->BuyMoney[i].Num)
			{	
				pstSysShop->iBuyMoney --;
				if (pstSysShop->iBuyMoney != i)
				{
					pstSysShop->BuyMoney[i] = pstSysShop->BuyMoney[pstSysShop->iBuyMoney];
				}
				return 0;
			}
		}

		if (i >= pstSysShop->iBuyMoney)
		{
			return -5;
		}
	}
	else
	{
		return -5;
	}
	return 0;	
}


int shop_cgi_init(ZONESVRENV* pstEnv)
{
	if ( pstEnv->pstGlobalObj->stGlobal.Misc.SysShop.ExchgInfo.Time == 0)
	{
		pstEnv->pstGlobalObj->stGlobal.Misc.SysShop.ExchgInfo.Time= pstEnv->pstAppCtx->stCurr.tv_sec;	
	}
	return 0;
}


/*
	return 0:成功
	-99:系统错误
	-1  : 操作货币数量错误
	-2  : 此类型的操作次数达到上限
	-3  : 对应的价格正在操作
	-4  : 时间错误
	-5  : 找不到要操作的单
	-6  : 购买某条玩家银币单时，系统交易进来的金币比将要交易出去的金币少
	-7  : 执行总单数错误
*/

int shop_cgi_req(ZONESVRENV* pstEnv, SHOPCGIREQ *pstReq, SHOPCGIRES *pstShopCgiRes)
{
	if(pstEnv->pstGlobalObj->stGlobalObjInfo.iGlobalStat != GLOBAL_STAT_FINI)
	{
		return -99;
	}
	
	int iRet = 0;
	SYSSHOPINFO *pstSysShop = &pstEnv->pstGlobalObj->stGlobal.Misc.SysShop;
	
	switch(pstReq->Type)
	{
	case SHOP_CGI_OP_SELL_GOLD:
		iRet = shop_cgi_sell_gold( pstEnv, pstReq);
		break;
	case SHOP_CGI_OP_BUY_GOLD:
		iRet = shop_cgi_buy_gold( pstEnv, pstReq);
		break;
	case SHOP_CGI_OP_SELL_MONEY:
		iRet = shop_cgi_sell_money(pstEnv, pstReq);
		break;
	case SHOP_CGI_OP_BUY_MONEY:
		iRet = shop_cgi_buy_money( pstEnv, pstReq);
		break;
	case SHOP_CGI_OP_SELECT:
		iRet = 0;
		break;
	
	case SHOP_CGI_OP_REMOVE:		// 撤销某次操作
		iRet = shop_cgi_remove(pstEnv, pstReq);
		break;
	case SHOP_CGI_OP_BUY_ONE:		// 购买某条玩家挂单
		iRet = shop_buy_player_trade(pstEnv, pstReq);
		break;
	case SHOP_CGI_OP_REMOVE_ONE: 	// 撤销某条系统挂单
		iRet = shop_cancel_sys_trade(pstEnv, pstReq);
		break;
	default:
		iRet = -99;
		break;
	}

	pstShopCgiRes->Type = pstReq->Type;
	pstShopCgiRes->Succ = iRet;
	pstShopCgiRes->SysShopInfo = *pstSysShop;
	return iRet;
}

int buy_limit_date_check(ZONESVRENV* pstEnv,Player *pstPlayer,SHOPDEF *pstShopDef)
{	
	if(!pstShopDef->LimitDate)
	{
		return 0;
	}

	GIFTBAGINFO *pstGiftBagInfo = &pstPlayer->stRoleData.MiscInfo.GiftBagInfo;
	int i = 0;
	int iNum = min(pstGiftBagInfo->GiftBagNum ,MAX_PLAYER_GIFT_BAG_NUM);
	GIFTBAG *pstGiftBag = NULL;

	for(i=0;i<iNum;i++)
	{
		pstGiftBag = &pstGiftBagInfo->GiftBag[i];
				
		if(pstGiftBag->ShopID == pstShopDef->ShopID)
		{
			break;
		}
	}

	if(i>=iNum)
	{
		return 0;
	}

	if(pstEnv->pstAppCtx->stCurr.tv_sec <=  pstGiftBag->BuyTm + (pstShopDef->LimitDate*24*60*60))
	{
		return -1;
	}

	return 0;
}

int buy_limit_date_use_check(ZONESVRENV* pstEnv,Player *pstPlayer,SHOPDEF *pstShopDef)
{
	if(!pstShopDef->LimitDate)
	{
		return 0;
	}
		
	int k = 0;
	int num = 0;
	ITEMDEF *pstItemDef = NULL;

	for (k=0; k<SHOP_ITEM_COUNT; k++)
	{
		if (0 == pstShopDef->Items[k].ItemID)
		{
			break;
		}
			
		pstItemDef = z_find_itemdef( pstEnv, pstShopDef->Items[k].ItemID);
		if (NULL == pstItemDef)
		{
			continue;
		}
	
		num = z_get_pak_item_num(pstEnv, &pstPlayer->stRoleData.Package, pstItemDef->ItemID, -2,0,PACKAGE_DEC_NONE);
		if(num > 0)
		{
			return -1;
		}
	}

	return 0;
}

int buy_limit_date_record(ZONESVRENV* pstEnv,Player *pstPlayer,SHOPDEF *pstShopDef)
{	
	if(!pstShopDef->LimitDate)
	{
		return 0;
	}
	
	GIFTBAGINFO *pstGiftBagInfo = &pstPlayer->stRoleData.MiscInfo.GiftBagInfo;
	int iNum = pstGiftBagInfo->GiftBagNum;

	if(iNum > MAX_PLAYER_GIFT_BAG_NUM)
	{
		return -1;
	}

	int i = 0;
	GIFTBAG *pstGiftBag = NULL;
	GIFTBAG stGiftBag;
	memset(&stGiftBag,0,sizeof(stGiftBag));

	for(i=iNum-1;i>=0;i--)
	{
		pstGiftBag = &pstGiftBagInfo->GiftBag[i];
				
		if(pstGiftBag->ShopID == pstShopDef->ShopID)
		{
			break;
		}
	}

	stGiftBag.ShopID = pstShopDef->ShopID;
	stGiftBag.BuyTm = pstEnv->pstAppCtx->stCurr.tv_sec;

	if(i<0)
	{
		//增加	
		if(pstGiftBagInfo->GiftBagNum >= MAX_PLAYER_GIFT_BAG_NUM)
		{
			memmove( &pstGiftBagInfo->GiftBag[0], &pstGiftBagInfo->GiftBag[1],sizeof(pstGiftBagInfo->GiftBag[0])*(MAX_PLAYER_GIFT_BAG_NUM-1));
			memcpy(&pstGiftBagInfo->GiftBag[MAX_PLAYER_GIFT_BAG_NUM-1],&stGiftBag,sizeof(pstGiftBagInfo->GiftBag[MAX_PLAYER_GIFT_BAG_NUM-1]));
		}
		else
		{
			memcpy(&pstGiftBagInfo->GiftBag[pstGiftBagInfo->GiftBagNum],&stGiftBag,sizeof(pstGiftBagInfo->GiftBag[pstGiftBagInfo->GiftBagNum]));
			pstGiftBagInfo->GiftBagNum ++;
		}
	}
	else
	{
		//替换
		pstGiftBagInfo->GiftBagNum --;
		if(pstGiftBagInfo->GiftBagNum != i)
		{
			memmove( &pstGiftBagInfo->GiftBag[i], &pstGiftBagInfo->GiftBag[i+1],sizeof(pstGiftBagInfo->GiftBag[0])*(pstGiftBagInfo->GiftBagNum-i));	
		}
	
		memcpy(&pstGiftBagInfo->GiftBag[pstGiftBagInfo->GiftBagNum],&stGiftBag,sizeof(pstGiftBagInfo->GiftBag[pstGiftBagInfo->GiftBagNum]));
		pstGiftBagInfo->GiftBagNum ++;
	}

	return 0;
}

//一次只能买一个
int buy_limit_date_num_check(ZONESVRENV* pstEnv,CSSHOPPINGBUYREQ *pstShoppingBuyReq,SHOPDEF *pstShopDef)
{
	if(!pstShopDef->LimitDate)
	{
		return 0;
	}
	
	if(pstShoppingBuyReq->ShoppingNum > 1)
	{
		return -1;
	}
	
	return 0;
}

//有时间限制的商品不能赠送
int buy_limit_date_send_check(ZONESVRENV* pstEnv,CSSHOPPINGBUYREQ *pstShoppingBuyReq,SHOPDEF *pstShopDef)
{
	if(!pstShopDef->LimitDate)
	{
		return 0;
	}
	
	if(pstShoppingBuyReq->GiftFlag == 1)
	{
		return -1;
	}
	
	return 0;
}


