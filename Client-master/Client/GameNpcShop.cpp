#include "GameNpcShop.h"
#include "proto_cs.h"
#include "GameManager.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameUIMsg.h"
#include "GamePlayManager.h"
#include "../UILib/ui_gameui.h"
#include "Equip.h"
#include "GameMainPlayer.h"

const int TALK_DIS = OP_DIST;

GameNpcShop::GameNpcShop(void)
{
	resetConditions();
	INetworkManager::getSingleton().RegisterMsgHandler( SHOP_SVR, this );
}

GameNpcShop::~GameNpcShop(void)
{
}

const tagClientShopList& GameNpcShop::getShopList() const
{
	return m_ShopList;
}

// 购买物品
void GameNpcShop::buyItem( int index, int num )
{
	assert( index < m_ShopList.m_nShopItemNum );

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL ) 
	{
		return;
	}
	const tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( m_ShopList.m_ShopSellItems[index].GoodsID );
	//优先使用shoplist中定义的货币类型
	if ( m_ShopList.m_ShopSellItems[index].SpecialMoneyType > 0 )
	{
		int moneyType = m_ShopList.m_ShopSellItems[index].SpecialMoneyType;	
		if( moneyType == ATTR_ID_MONEY )
		{
			if( pMainPlayer->getMoney() < m_ShopList.m_ShopSellItems[index].Price * m_ShopList.RebateCount / 100 ) 
			{
				// Money不够
				GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_MONEYNOTENOUGH_SELLFAIL );
				return;
			}
		}else if( moneyType == ATTR_ID_BULL ){
			if( pMainPlayer->getBull() + pMainPlayer->getMoney() < m_ShopList.m_ShopSellItems[index].Price * m_ShopList.RebateCount / 100 )
			{
				// Bull不够
				GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_BULLNOTENOUGH_SELLFAIL );
				return;
			}
		}
	}else{
		if( itemDef.ExchgItemID > 0 )
		{
			if( pMainPlayer->getContainer().getItemCount( itemDef.ExchgItemID ) < (int)itemDef.BuyPrice )
			{
				// 兑换的牌子不够
				GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_EXCHANGEITEMNOTENOUGH_SELLFAIL );
				return;
			}
		}else if ( itemDef.MoneyType == ATTR_ID_SPARTA_GLORY )
		{
			if( pMainPlayer->getSpartaGlory() < itemDef.BuyPrice ) 
			{
				// 竞技场荣誉不够
				GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_SPARTA_GLORY_SELLFAIL );
				return;
			}
		}else{
			if( itemDef.MoneyType == ATTR_ID_MONEY )
			{
				if( pMainPlayer->getMoney() < m_ShopList.m_ShopSellItems[index].Price * m_ShopList.RebateCount / 100 ) 
				{
					// Money不够
					GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_MONEYNOTENOUGH_SELLFAIL );
					return;
				}
			}else if( itemDef.MoneyType == ATTR_ID_BULL ){
				if( pMainPlayer->getBull() + pMainPlayer->getMoney() < m_ShopList.m_ShopSellItems[index].Price * m_ShopList.RebateCount / 100 )
				{
					// Bull不够
					GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_BULLNOTENOUGH_SELLFAIL );
					return;
				}
			}
		}

		for( int i = 0 ; i < MAX_OTHER_MONEY_USE ; i++ )
		{
			if( itemDef.MoneyUses[i].Val > 0 )
			{
				if( itemDef.MoneyUses[i].ID == ATTR_ID_BATTLE_GLORY 
					&& pMainPlayer->getBattleGlory() < itemDef.MoneyUses[i].Val )
				{
					//荣誉不足
					GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_BATTLE_GLORY_SELLFAIL );
					return;
				}else if( itemDef.MoneyUses[i].ID == ATTR_ID_AOSHAN_BRAND 
					&& pMainPlayer->getAoShanBrand() < itemDef.MoneyUses[i].Val )
				{
					//绿洲牌子 不足
					GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_AOSHAN_BRAND_SELLFAIL );
					return;
				}else if( itemDef.MoneyUses[i].ID == ATTR_ID_LIFT_BRAND 
					&& pMainPlayer->getLiftBrand() < itemDef.MoneyUses[i].Val )
				{
					//狂沙牌子 不足
					GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_LIFT_BRAND_SELLFAIL );
					return;
				}
			}		
		}
	}	
	
	tagCSPkgBody msgbody;
	msgbody.ShopClt.NpcID							= m_ShopList.m_nNpcId;
	msgbody.ShopClt.OpType							= SHOP_BUY;
	msgbody.ShopClt.ShopCltData.ShopBuy.Idx			= index;
	msgbody.ShopClt.ShopCltData.ShopBuy.GoodsID		= m_ShopList.m_ShopSellItems[index].GoodsID;
	msgbody.ShopClt.ShopCltData.ShopBuy.GoodsType	= m_ShopList.m_ShopSellItems[index].GoodsType;
	msgbody.ShopClt.ShopCltData.ShopBuy.Num			= num;
	SendGameServerMsg( SHOP_CLT, msgbody );
}

void GameNpcShop::buyClanItem( int index,int num )
{
	tagCSPkgBody msgbody;
	msgbody.ShopClt.NpcID							= m_ShopList.m_nNpcId;
	msgbody.ShopClt.OpType							= SHOP_BUY;
	msgbody.ShopClt.ShopCltData.ShopBuy.Idx			= index;
	msgbody.ShopClt.ShopCltData.ShopBuy.GoodsID		= m_ShopList.m_ShopSellItems[index].GoodsID;
	msgbody.ShopClt.ShopCltData.ShopBuy.GoodsType	= m_ShopList.m_ShopSellItems[index].GoodsType;
	msgbody.ShopClt.ShopCltData.ShopBuy.Num			= num;
	SendGameServerMsg( SHOP_CLT, msgbody );
}

// 回购物品
void GameNpcShop::buySaveItem( int index )
{
	assert( index < m_ShopList.m_nShopSaveItemNum );

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL ) return;
	const tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( m_ShopList.m_ShopSaveItems[index].m_SaveItem.getItemId() );
	if( itemDef.MoneyType == ATTR_ID_MONEY )
	{
		if( pMainPlayer->getMoney() < m_ShopList.m_ShopSaveItems[index].m_nSellPrice ) 
		{
			// Money不够
			GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_MONEYNOTENOUGH_BUYSAVEFAIL );
			return;
		}
	}else if( itemDef.MoneyType == ATTR_ID_BULL )
	{
		if ( pMainPlayer->getBull() + pMainPlayer->getMoney() < m_ShopList.m_ShopSaveItems[index].m_nSellPrice )
		{
			// Bull不够
			GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_BULLNOTENOUGH_BUYSAVEFAIL );
			return;
		}
	}
// 	for( int i = 0 ; i < MAX_OTHER_MONEY_USE ; i++ )
// 	{
// 		if( itemDef.MoneyUses[i].Val > 0 )
// 		{
// 			if( itemDef.MoneyUses[i].ID == ATTR_ID_BATTLE_GLORY 
// 				&& pMainPlayer->getBattleGlory() < itemDef.MoneyUses[i].Val )
// 			{
// 				//荣誉不足
// 				GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_BATTLE_GLORY_SELLFAIL );
// 				return;
// 			}else if( itemDef.MoneyUses[i].ID == ATTR_ID_AOSHAN_BRAND 
// 				&& pMainPlayer->getAoShanBrand() < itemDef.MoneyUses[i].Val )
// 			{
// 				//绿洲牌子 不足
// 				GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_AOSHAN_BRAND_SELLFAIL );
// 				return;
// 			}else if( itemDef.MoneyUses[i].ID == ATTR_ID_LIFT_BRAND 
// 				&& pMainPlayer->getLiftBrand() < itemDef.MoneyUses[i].Val )
// 			{
// 				//狂沙牌子 不足
// 				GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_LIFT_BRAND_SELLFAIL );
// 				return;
// 			}
// 		}		
// 	}

	tagCSPkgBody msgbody;
	msgbody.ShopClt.NpcID							= m_ShopList.m_nNpcId;
	msgbody.ShopClt.OpType							= SHOP_BUY_SELLED;
	msgbody.ShopClt.ShopCltData.ShopBuySelled.Idx	= index;
	msgbody.ShopClt.ShopCltData.ShopBuySelled.WID	= m_ShopList.m_ShopSaveItems[index].m_SaveItem.getItemWId();
	SendGameServerMsg( SHOP_CLT, msgbody );
}

// 出售物品
void GameNpcShop::sellItem( int listType, int gridIndex, int num )
{
	tagCSPkgBody msgbody;
	msgbody.ShopClt.NpcID								= m_ShopList.m_nNpcId;
	msgbody.ShopClt.OpType								= SHOP_SELL;
	msgbody.ShopClt.ShopCltData.ShopSell.SrcListType	= listType;
	msgbody.ShopClt.ShopCltData.ShopSell.SrcGridIdx		= gridIndex;
	msgbody.ShopClt.ShopCltData.ShopSell.SrcGridNum		= num;
	SendGameServerMsg( SHOP_CLT, msgbody );
}

// 修理单个物品
void GameNpcShop::repairOneItem( int listType, int gridIndex, int itemId )
{
	assert( listType == CONTAINER_TYPE_SUNDRIES || listType == CONTAINER_TYPE_WEAR && gridIndex >= 0 );

	// 判断维修价格
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	Item item;
	if ( CONTAINER_TYPE_SUNDRIES == listType )
	{
		item = (pMainPlayer->getContainer().getItem( listType, gridIndex ));
	}else if( CONTAINER_TYPE_WEAR == listType ) {
		item = (pMainPlayer->getEquip().getItem( gridIndex ));
	}

	assert( item.getItemType() != ITEM_SLOTTYPE_NONE  );
	//int nRepairPrice = getRepairOnePrice( item );
	//if ( nRepairPrice > (int)pMainPlayer->getMoney() )
	//{
	//	//布尔不够
	//	GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_BULLNOTENOUGH_REPAIRFAIL );
	//	return;
	//}
	
	tagCSPkgBody msgbody;
	msgbody.ShopClt.NpcID									= m_ShopList.m_nNpcId;
	msgbody.ShopClt.OpType									= SHOP_REPAIR_ONE;
	msgbody.ShopClt.ShopCltData.ShopRepairOne.SrcListType	= listType;
	msgbody.ShopClt.ShopCltData.ShopRepairOne.SrcGridIdx	= gridIndex;
	msgbody.ShopClt.ShopCltData.ShopRepairOne.ItemID		= itemId;
	SendGameServerMsg( SHOP_CLT, msgbody );
}

// 修理全部物品
void GameNpcShop::repairAllItem()
{
	// 判断维修价格
	//MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	//if (  getRepairAllPrice() > pMainPlayer->getMoney() + pMainPlayer->getBull() )
	//{
	//	//布尔不够
	//	GameMsgPrint::getSingleton().showMidTip( MID_NPCSHOP_BULLNOTENOUGH_REPAIRFAIL );
	//	return;
	//}

	tagCSPkgBody msgbody;
	msgbody.ShopClt.NpcID		= m_ShopList.m_nNpcId;
	msgbody.ShopClt.OpType		= SHOP_REPAIR_ALL;
	msgbody.ShopClt.ShopCltData.ShopRepairAll =	REPAIR_WAER;
	SendGameServerMsg( SHOP_CLT, msgbody );
}

int GameNpcShop::getRepairAllPrice()
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	Equip &equip = pMainPlayer->getEquip();
	int nRepairTotalPrice(0);
	bool bNeedRepair = false;

	for ( int i = 0; i < EQUIP_POS_END; i++ )
	{
		Item &item = equip.getItem(i);		
		
		if ( item.getItemType() == ITEM_SLOTTYPE_NONE || item.getItemId() == 0 )
		{
			continue;
		}

		tagItemDef_ToLua* pItemDef = item.getItemDef();
		if( item.canRepair() )
		{
			nRepairTotalPrice += item.getRepairPrice();
			bNeedRepair = true;
		}
	}

	if ( bNeedRepair && nRepairTotalPrice <= 0 )
	{
		nRepairTotalPrice = 1;
	}

	nRepairTotalPrice = petNativeSkillResultDurPrice( nRepairTotalPrice );
	return nRepairTotalPrice;
}

int GameNpcShop::getRepairOnePrice( Item& repairItem )
{
	int nRepairPrice = repairItem.getRepairPrice();
	if ( nRepairPrice <= 0 )
	{
		nRepairPrice = 1;
	}

	nRepairPrice = petNativeSkillResultDurPrice( nRepairPrice );
	return nRepairPrice;
}

int	GameNpcShop::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{	
	if( SHOP_SVR == msgid )
	{
		const CSSHOPSVR& shopServerDate = msgbody.ShopSvr;

		if( shopServerDate.OpType == SHOP_LIST )
		{
			m_ShopList.m_nNpcId				= shopServerDate.ShopSvrData.ShopList.NpcID;
			m_ShopList.m_nShopItemNum		= shopServerDate.ShopSvrData.ShopList.ShopItemNum;
			m_ShopList.m_nShopSaveItemNum	= shopServerDate.ShopSvrData.ShopList.ItemSellSave.ItemSellNum;
			m_ShopList.RebateCount			= shopServerDate.ShopSvrData.ShopList.RebateCount;
			m_ShopList.RebateTypeNum		= shopServerDate.ShopSvrData.ShopList.RebateTypeNum;
			for( int i = 0 ; i < REBATE_TYPE_MAX_NUM ; i ++ )
			{
				m_ShopList.RebateTypes[i] = shopServerDate.ShopSvrData.ShopList.RebateTypes[i];
			}
			for( int i = 0; i < m_ShopList.m_nShopItemNum; i++ )
			{
				m_ShopList.m_ShopSellItems[i].GoodsID			= shopServerDate.ShopSvrData.ShopList.ShopSellItems[i].GoodsID;
				m_ShopList.m_ShopSellItems[i].GoodsType			= shopServerDate.ShopSvrData.ShopList.ShopSellItems[i].GoodsType;
				m_ShopList.m_ShopSellItems[i].Price				= shopServerDate.ShopSvrData.ShopList.ShopSellItems[i].Price;
				m_ShopList.m_ShopSellItems[i].SpecialMoneyType	= shopServerDate.ShopSvrData.ShopList.ShopSellItems[i].SpecialMoneyType;
			}
			for( int i = 0; i < m_ShopList.m_nShopSaveItemNum; i++ )
			{
				const ROLEGRID& roleGrid = shopServerDate.ShopSvrData.ShopList.ItemSellSave.ItemSells[i].RoleGrid;
				if( roleGrid.Type == GRID_ITEM_ARM )
				{
					m_ShopList.m_ShopSaveItems[i].m_SaveItem = roleGrid.GridData.RoleArm;
				}else if( roleGrid.Type == GRID_ITEM_ITEM )
				{
					m_ShopList.m_ShopSaveItems[i].m_SaveItem = roleGrid.GridData.RoleItem;	
				}
				m_ShopList.m_ShopSaveItems[i].m_nSellPrice	= shopServerDate.ShopSvrData.ShopList.ItemSellSave.ItemSells[i].SellPrice;
				m_ShopList.m_ShopSaveItems[i].m_nMoneyType	= shopServerDate.ShopSvrData.ShopList.ItemSellSave.ItemSells[i].MoneyType;
				m_ShopList.m_ShopSaveItems[i].m_nSellTime	= shopServerDate.ShopSvrData.ShopList.ItemSellSave.ItemSells[i].SellTime;
			}

			m_ConditionNum = shopServerDate.ShopSvrData.ShopList.ConditionNum;
			for (size_t i = 0; i < m_ConditionNum; ++i)
			{
				const tagCSShopCondition &condition = shopServerDate.ShopSvrData.ShopList.Conditions[i];
				m_Conditions[i].id = m_ShopList.m_ShopSellItems[condition.GridIdx].GoodsID;
				m_Conditions[i].type = condition.Condition.Type;
				m_Conditions[i].value1 = condition.Condition.Value1;
				m_Conditions[i].value2 = condition.Condition.Value2;
				m_Conditions[i].value3 = condition.Condition.Value3;
			}

			GameManager::getSingleton().SendUIEvent( "GE_NPC_SHOPLIST" );
		} else if( shopServerDate.OpType == SHOP_SELLED_LIST )
		{
			m_ShopList.m_nShopSaveItemNum	= shopServerDate.ShopSvrData.ShopSelledList.ItemSellNum;
			for( int i = 0; i < m_ShopList.m_nShopSaveItemNum; i++ )
			{
				const ROLEGRID& roleGrid = shopServerDate.ShopSvrData.ShopSelledList.ItemSells[i].RoleGrid;
				if( roleGrid.Type == GRID_ITEM_ARM )
				{
					m_ShopList.m_ShopSaveItems[i].m_SaveItem = roleGrid.GridData.RoleArm;
				}else if( roleGrid.Type == GRID_ITEM_ITEM )
				{
					m_ShopList.m_ShopSaveItems[i].m_SaveItem = roleGrid.GridData.RoleItem;	
				}
				m_ShopList.m_ShopSaveItems[i].m_nSellPrice	= shopServerDate.ShopSvrData.ShopSelledList.ItemSells[i].SellPrice;
				m_ShopList.m_ShopSaveItems[i].m_nMoneyType	= shopServerDate.ShopSvrData.ShopSelledList.ItemSells[i].MoneyType;
				m_ShopList.m_ShopSaveItems[i].m_nSellTime	= shopServerDate.ShopSvrData.ShopSelledList.ItemSells[i].SellTime;
			}

			GameManager::getSingleton().SendUIEvent( "GE_NPC_SHOPBUYLIST" );
		} else if ( shopServerDate.OpType == SHOP_REPAIR_ALL )
		{
			if ( shopServerDate.ShopSvrData.ShopRepairAll != REPAIR_WAER )
			{
				return 0;
			}

			MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
			if ( pMainPlayer == NULL )
			{
				return 0;
			}

			Equip&	mainplayerEquip = pMainPlayer->getEquip();

			for ( int i =0; i < EQUIP_POS_END; ++i )
			{
				Item&	item = mainplayerEquip.getItem( i );

				if ( item.getItemId() != 0 && item.canRepair() )
				{
					item.reSetDur();
				}
			}
			GameManager::getSingleton().SendUIEvent( "GE_CONTAINER_UPDATE" );
			GameManager::getSingleton().SendUIEvent("GE_WEAR_ARM_UPDATE");
		}
	}
	return 0;
}

int GameNpcShop::petNativeSkillResultDurPrice( int nPrice )
{
	nPrice = nPrice * m_ShopList.RebateCount / 100 ;
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GamePet* pPetPackage = pMainPlayer->getPet();
	
	GameActor* pFightPet = NULL;
	for ( int i = 0; i < PET_MAX_FIGHT_SHI; ++i )
	{
		pFightPet = pPetPackage->getFightPet( i );
		if ( pFightPet != NULL )
		{
			break;
		}
	}

	if ( pFightPet == NULL )
	{
		return nPrice;
	}

	tagRolePet_ToLua* pPetInfo = pPetPackage->getPetInfo( pFightPet->getRoleWId() );
	if( pPetInfo == NULL )
	{
		return nPrice;
	}
	const tagPetSkillGrid_ToLua& petDiathesis = pPetInfo->NativeSkill;
	if ( petDiathesis.SkillID == 0 )
	{
		return nPrice;
	}

	if ( petDiathesis.GridType != PET_SKILL_SELF_BUFF)
	{
		return nPrice;
	}

	const tagDiathesisDef_ToLua* pDiathesisDef = getDiathesisDef( petDiathesis.SkillID, petDiathesis.SkillLevel );
	if ( pDiathesisDef == NULL )
	{
		return nPrice;
	}
	
	for (int i=0; i<MAX_DIATHESIS_RESULT; i++)
	{
		const SKILLRESULT& oneSkillResult = pDiathesisDef->DiathesisResult[i];
		if ( 0 == oneSkillResult.SkillResultID )
		{
			return nPrice;
		}

		//影响修理费的result
		if ( RESULT_NATIVE_REPAIR_COST == oneSkillResult.SkillResultID )
		{
			nPrice -= oneSkillResult.SkillResultVal1 / (float)PET_DIATHESIS_ADJUST_REPAIR_VALUE * nPrice;
			if ( nPrice < 0 )
			{
				nPrice = 0;
			}
		}

	}

	return nPrice;
}

int	GameNpcShop::findItemindex( int itemid)
{
	for (int i = 0;i < MAX_SELL_GOODS; i++)
	{
		if ( m_ShopList.m_ShopSellItems[i].GoodsID == itemid )
		{
			return i;
		}
	}
	return -1;
}

ShopItemRequireTech GameNpcShop::getRequireTech(int id)
{
	ShopItemRequireTech tech;
	tech.num = 0;
	for (size_t i = 0; i < m_ConditionNum; ++i)
	{
		if (m_Conditions[i].id == id && m_Conditions[i].type == SHOP_CONDITION_CLAN_TECH)
		{
			tech.id[tech.num] = m_Conditions[i].value1;
			tech.level[tech.num] = m_Conditions[i].value2;
			if (++tech.num == MAX_SHOP_ITEM_REQUIRE_TECH)
			{
				break;
			}
		}
	}
	return tech;
}

void GameNpcShop::resetConditions()
{
	m_ConditionNum = 0;
}
