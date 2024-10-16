#include <Windows.h>
#include "GameMarketManager.h"
#include "GamePlayManager.h"
#include "GameMainPlayer.h"
#include "GameFriendManager.h"
#include "ServerTimer.h"
#include "OgreScriptLuaVM.h"
#include <time.h>
#include <algorithm>
#include "GameClientOpt.h"
#include <fstream>
using namespace Ogre;
CMarketManager::CMarketManager()
{
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_MARKET_LATEST_GOLD_COIN_BUY, sizeof(m_LatestGoldCoinBuy) );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_MARKET_LATEST_TICK_BUY, sizeof(m_LatestTicketBuy) );
}

void CMarketManager::create()
{
	INetworkManager::getSingleton().RegisterMsgHandler(SHOPPING_SVR,this);
	INetworkManager::getSingleton().RegisterMsgHandler(STARDATA_SVR,this);
}
CMarketManager::~CMarketManager()
{
}

static bool LessThan( const tagShopDef& lhs,const tagShopDef& rhs )
{
	if( (lhs.OrderID != 0) && (rhs.OrderID == 0 || lhs.OrderID < rhs.OrderID) )
	{
		return true;
	}
		
	if( lhs.OrderID == rhs.OrderID && lhs.ShopID < rhs.ShopID )
	{
		return true;
	}
	return false;
}

int CMarketManager::HandleNetMsg( int msgid, const tagCSPkgBody &msgbody )
{
	if( SHOPPING_SVR == msgid )
	{
		const CSSHOPPINGRES& msg = msgbody.ShoppingSvr;
		switch( msg.Type )
		{
		case SHOPPING_GET_XML_FILE_RES:
			{
				const CSSHOPPINGGETXMLFILERES& xmlData = msg.ShoppingResData.ShoppingGetXmlFileRes;
				onGetXmlResponse(xmlData);
			}
			break;

		case SHOPPING_FRESH_RES:
			{
				const CSSHOPPINGFRESHRECORDRES& record = msg.ShoppingResData.ShoppingFreshRecordRes;
				onFreshResponse(record);
			}
			break;

		case SHOPPING_MODIFY_RES:
			{
				const CSSHOPPINGMODIFYRECORDRES& record = msg.ShoppingResData.ShoppingModifyRecordRes;
				onModifyResponse(record);
				checkLatestBuyData();
			}
			break;

		case SHOPPING_SHOW_MSG_RES:
			{
				m_showMsg = msg.ShoppingResData.ShoppingMsgRes.ShopMsgDef;
				GameManager::getSingleton().SendUIEvent("GE_UPDATE_SHOW_MSG");
			}
			break;
		case SHOPPING_QUERY_TIANYUAN_RES:
			{
				const CSSHOPPINGQUERYTIANYUANRES& queryTianyuan = msg.ShoppingResData.ShoppingQueryTianYuanRes;
				m_nCurDianquan = (int)(queryTianyuan.Points + queryTianyuan.Bonus - queryTianyuan.FreezePoint);
				GameManager::getSingleton().SendUIEvent("UI_MARKET_TIANYUAN_RESPONSE");
			}
			break;
		case SHOPPING_EXCHANGE_POINT_RES:
			{
				const CSSHOPPINGEXCHANGERES& exchangePoint = msg.ShoppingResData.ShoppingExchangeRes;
				onExchangePointResponse( exchangePoint );
			}
		case SHOPPING_QUERY_TRADESELL_RES:
			{
				m_nSellTotalNum	 = msg.ShoppingResData.ShoppingQueryTradeSellRes.SvrCurrTotalNum;
				m_nSellRecordNum = msg.ShoppingResData.ShoppingQueryTradeSellRes.RecordNum;
				//const CSSHOPPINGQUERYTRADESELLRES& tradeSells = msg.ShoppingResData.ShoppingQueryTradeSellRes;
				//onQueryTradeSellResponse(tradeSells);
				for (int i = 0; i < m_nSellRecordNum ;i++)
				{
					modifyTradeRecordData( msg.ShoppingResData.ShoppingQueryTradeSellRes.TradeRecord[i] ,RECORD_SELL_TRADE_LIST );
				}
				GameManager::getSingleton().SendUIEvent( "GE_UPDATE_TRADE_LIST" );
			}
			break;
		case SHOPPING_QUERY_TRADEBUY_RES:
			{	
				m_nBuyTotalNum = msg.ShoppingResData.ShoppingQueryTradeBuyRes.SvrCurrTotalNum;
				m_nBuyRecordNum	= msg.ShoppingResData.ShoppingQueryTradeBuyRes.RecordNum;
				//const CSSHOPPINGQUERYTRADEBUYRES& tradeBuys = msg.ShoppingResData.ShoppingQueryTradeBuyRes;
				//onQueryTradeBuyResponse(tradeBuys);
				for (int i = 0; i < m_nBuyRecordNum ;i++)
				{
					modifyTradeRecordData( msg.ShoppingResData.ShoppingQueryTradeBuyRes.TradeRecord[i],RECORD_BUY_TRADE_LIST );
				}
				GameManager::getSingleton().SendUIEvent( "GE_UPDATE_TRADE_LIST" );
			}
			break;
		case SHOPPING_PACKAGE_PRESENT_RES:
			{
				const CSSHOPPINGPACKAGEPRESENTRES& packagePresent = msg.ShoppingResData.ShoppingPackagePresentRes;
				onPackagePresentResponse(packagePresent);
			}
			break;
		case SHOPPING_TRADE_RES:
			{
				const CSSHOPPINGTRADERES& tradeRes =  msg.ShoppingResData.ShoppingTradeRes;
				onTradeResponse(tradeRes);
			}
			break;
		case SHOPPING_SEND_TRADERECORD_RES:
			{
				const CSSHOPPINGSENDTRADERECORDRES& sendRecord = msg.ShoppingResData.ShoppingSendTradeRecordRes;
				onSendTradeRecordResponse(sendRecord);
			}
			break; 
		case SHOPPING_QUERY_MY_TRADERECORD_RES:
			{
				m_nMyTradeNum = msg.ShoppingResData.ShoppingQueryMyTradeRecordRes.RecordNum;
				//const CSSHOPPINGQUERYMYTRADERECORDRES& myRecord = msg.ShoppingResData.ShoppingQueryMyTradeRecordRes;
				//onQueryMyTradeRecordResponse(myRecord);
				for (int i = 0; i < m_nMyTradeNum ;i++)
				{
					modifyTradeRecordData( msg.ShoppingResData.ShoppingQueryMyTradeRecordRes.TradeRecord[i],RECORD_MY_TRADE_LIST );
				}
			}
			break;
		case SHOPPING_QUERY_RECORD_BY_ID_RES:
			{
				const CSSHOPPINGQUERYRECORDBYIDRES& record = msg.ShoppingResData.ShoppingQueryRecordByIDRes;
				onSearchTradeRespone(record);
			}
			break;
		case SHOPPING_DESTROY_TRADERECORD_RES:
			{
				const CSSHOPPINGDESTROYTRADERECORDRES& record = msg.ShoppingResData.ShoppingDestroyTradeRecordRes;
				onDestroyTradeRecord(record);
			}
			break;
		case SHOPPING_MODIFY_LIMIT_RES:
			{
				const CSSHOPPINGMODIFYLIMITRECORDRES& modify = msg.ShoppingResData.ShoppingModifyLimitRecordRes;
				for(unsigned int i = 0 ; i < modify.RecordNum ; i ++ )
				{
					bool bFound = false;
					for(unsigned int j = 0 ; j < m_ShopLimit.size() ; j++ )
					{
						if( m_ShopLimit[j].ShopID == modify.ShopLimitRecord[i].ShopID )
						{
							m_ShopLimit[j] = modify.ShopLimitRecord[i];
							bFound = true;
							break;
						}
					}
					if( !bFound )
					{
						m_ShopLimit.push_back(modify.ShopLimitRecord[i]);
					}
				}
				GameManager::getSingleton().SendUIEvent("GE_MARKET_LIMIT_CHG");
			}
			break;
		case SHOPPING_LIMIT_FRESH_RES:
			{
				const CSSHOPPINGLIMITFRESHRECORDRES& fresh = msg.ShoppingResData.ShoppingLimitFreshRecordRes;
				if( fresh.CurrPkg == 1 )
				{
					//第一页
					m_ShopLimit.clear();
				}
				for(unsigned int i = 0 ; i < fresh.RecordNum ; i++ )
				{
					m_ShopLimit.push_back(fresh.ShopRecord[i]);
				}
				GameManager::getSingleton().SendUIEvent("GE_MARKET_LIMIT_CHG");
			}
			break;
		case SHOPPING_QUERY_PROPSDEF_RES:
			break;
		case SHOPPING_QUERY_PROPSGIFT_RES:
			break;
		case SHOPPING_SET_PROPSGIFT_RES:
			break;
		case SHOPPING_SHOP_DEL_RES:
			{
				for( int i = 0 ; i < msg.ShoppingResData.ShoppingShopDelRes.DelNum ; i++ )
				{
					VSHOPITEM::iterator it = m_MarketItem.begin();
					for( ; it != m_MarketItem.end() ; it++ )
					{
						if( it->ShopID == msg.ShoppingResData.ShoppingShopDelRes.ShopIDs[i] )
						{
							m_MarketItem.erase( it );
							break;
						}
					}
				}
				GameManager::getSingleton().SendUIEvent("GE_UPDATE_ITEM_LIST");
			}
			break;
		case SHOPPING_SHOPLIMIT_DEL_RES:
			{
				for( int i = 0 ; i < msg.ShoppingResData.ShoppingShopLimitDelRes.DelNum ; i++ )
				{
					VSSHOPLIMIT::iterator it = m_ShopLimit.begin();
					for( ; it != m_ShopLimit.end() ; it++ )
					{
						if( it->ShopID == msg.ShoppingResData.ShoppingShopLimitDelRes.ShopIDs[i] )
						{
							m_ShopLimit.erase( it );
							break;
						}
					}
				}
				GameManager::getSingleton().SendUIEvent("GE_MARKET_LIMIT_CHG");
			}
			break;
		}
	}
	return 0;
}

void CMarketManager::ParserTypeLabel( XMLNode node, VTYPELABEL& vLabel )
{
	XMLNode pChildNode = node.iterateChild();
	while(!pChildNode.isNull())
	{
		if( stricmp(pChildNode.getName(),"Type")==0 )
		{	
			TypeLabel label;
			label.key = pChildNode.attribToInt("TypeValue");
			if( pChildNode.hasAttrib("TypeName") )
			{
				strncpy(label.labelName,pChildNode.attribToString("TypeName"),16);
			}
			ParserSubType(pChildNode,&label);
			vLabel.push_back( label );
		}
		pChildNode = node.iterateChild(pChildNode);
	}
}

void CMarketManager::ParserSubType( XMLNode node, TypeLabel* label )
{
	XMLNode pChildNode = node.iterateChild();
	int nCount = 0;
	while(!pChildNode.isNull())
	{
		SubTypeLabel subLabel;
		if( stricmp(pChildNode.getName(),"SubType")==0 )
		{
			subLabel.key = pChildNode.attribToInt("SubTypeValue");
			strncpy(subLabel.labelName,pChildNode.attribToString("SubTypeName"),16);
			label->subLabel[nCount] = subLabel;
			nCount ++;
		}
		pChildNode = node.iterateChild(pChildNode);
	}
	label->subLabelCount = nCount;
}

int CMarketManager::getLabelCount( int nBuyType )
{
	TYPELABELMAP::iterator it = m_TypeLabel.find( nBuyType );
	if( it != m_TypeLabel.end() )
	{
		return (int) it->second.size();
	}
	return 0;
}

int CMarketManager::getSubLabelCount( int nBuyType, int labelIndex )
{
	TYPELABELMAP::iterator it = m_TypeLabel.find( nBuyType );
	if( it != m_TypeLabel.end() )
	{
		const VTYPELABEL& vLabel = it->second;
		if( labelIndex < vLabel.size() )
		{
			return vLabel[labelIndex].subLabelCount;
		}
	}
	return 0;
}

TypeLabel* CMarketManager::getTypeLabel( int nBuyType, int labelIndex )
{
	TYPELABELMAP::iterator it = m_TypeLabel.find( nBuyType );
	if( it != m_TypeLabel.end() )
	{
		return &(it->second[labelIndex]);
	}
	return NULL;
};

bool CMarketManager::onGetXmlResponse( const CSSHOPPINGGETXMLFILERES& xmlData )
{
	m_TypeLabel.clear();
	if( xmlData.FileLen > 0 )
	{
		Ogre::XMLData xml;
		xml.loadBuffer(xmlData.FileBuff,xmlData.FileLen);
		XMLNode pRootNode = xml.getRootNode();
		XMLNode pChildNode = pRootNode.iterateChild();

		while(!pChildNode.isNull())
		{
			if( stricmp(pChildNode.getName(), "Shopping") == 0 )
			{
				
				XMLNode subChildNode = pChildNode.getChild("ShoppingName");
				if( !subChildNode.isNull() )
				{
					int nBuyType = BUY_TYPE_TICKET;
					VTYPELABEL vLabel;
					if( stricmp( subChildNode.getText(), "金券商城" ) == 0 )
					{
						nBuyType = BUY_TYPE_TICKET;						
					}else if( stricmp( subChildNode.getText(), "金币商城" ) == 0 )
					{
						nBuyType = BUY_TYPE_BINDGOLD;
					}
					subChildNode = pChildNode.getChild("LableList");
					ParserTypeLabel( subChildNode, vLabel );
					m_TypeLabel.insert( make_pair( nBuyType, vLabel ) );
				}
			}			
			pChildNode = pRootNode.iterateChild(pChildNode);
		}
		GameManager::getSingleton().SendUIEvent("GE_SET_LABEL_TEXT");
		return true;
	}
	return false;
}

bool CMarketManager::onFreshResponse(const CSSHOPPINGFRESHRECORDRES& record )
{
	if( record.CurrPkg == 1 && !m_MarketItem.empty())
	{
		m_MarketItem.clear();
	}
	assert(record.CurrPkg <= record.PkgTotal );
	for( unsigned int i = 0 ; i < record.RecordNum ; i ++ )
	{
		bool bFound = false;
		for( size_t index = 0 ; index < m_MarketItem.size(); index ++ )
		{
			if( m_MarketItem[index].ShopID == record.ShopRecord[i].ShopID )
			{
				m_MarketItem[index] = record.ShopRecord[i];
				bFound = true;
				break;
			}
		}
		if( !bFound )
		{
			m_MarketItem.push_back(record.ShopRecord[i]);
		}
	}
	if( record.CurrPkg == record.PkgTotal )
	{
		checkLatestBuyData();
	}
	GameManager::getSingleton().SendUIEvent("GE_UPDATE_ITEM_LIST");
	return true;
}

bool CMarketManager::onModifyResponse( const CSSHOPPINGMODIFYRECORDRES& record )
{
	for( unsigned int i = 0 ; i < record.RecordNum ; i ++ )
	{
		bool bFound = false;
		for( size_t index = 0 ; index < m_MarketItem.size(); index ++ )
		{
			if( m_MarketItem[index].ShopID == record.ShopRecord[i].ShopID )
			{
				m_MarketItem[index] = record.ShopRecord[i];
				bFound = true;
				break;
			}
		}
		if( !bFound )
		{
			m_MarketItem.push_back(record.ShopRecord[i]);
		}
	}
	stable_sort(m_MarketItem.begin(),m_MarketItem.end(),LessThan);
	GameManager::getSingleton().SendUIEvent("GE_UPDATE_ITEM_LIST");
	return true;
}

//请求购买商品
void CMarketManager::requestBuy( int id, int num, int giftFlag, const char* GiftToPlayerName, const char* szDesc, int nBuyFlag )
{
	tagCSShopDef_ToLua* shopItemDef = getItemByID(id);
	if( shopItemDef == NULL )
	{
		return;
	}
	int nPrice = shopItemDef->BuyPrice*(100 - shopItemDef->Sale)/100;
	int nTotalNeed = nPrice*num;
	int haveCount = 0;

	if( shopItemDef->BuyType == 1 )
	{
		haveCount = GamePlayManager::getSingleton().getMainPlayer()->getGodCoin() ;
	}else{
		haveCount = GamePlayManager::getSingleton().getMainPlayer()->getBindGodCoin();
	}
	if( haveCount < nTotalNeed )
	{
		if( shopItemDef->BuyType == 1 )
		{
			GameMsgPrint::getSingleton().showMidTip(MID_MARKET_NOT_ENOUGH_GOLD);
		}else{
			GameMsgPrint::getSingleton().showMidTip(MID_MARKET_NOT_ENOUGH_BIND_GOLD);
		}
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_BUY_REQ;
	memset(&msgbody.ShoppingClt.ShoppingReqData.ShppingBuyReq,0,sizeof(CSSHOPPINGBUYREQ));
	msgbody.ShoppingClt.ShoppingReqData.ShppingBuyReq.RoleID = GamePlayManager::getSingleton().getMainPlayer()->getRoleWId();
	msgbody.ShoppingClt.ShoppingReqData.ShppingBuyReq.GiftFlag = giftFlag;
	if( GiftToPlayerName != NULL )
	{
		strncpy(msgbody.ShoppingClt.ShoppingReqData.ShppingBuyReq.GiftToPlayerName,GiftToPlayerName,ROLE_NAME_LEN);
	}
	if( szDesc !=NULL )
	{
		strncpy(msgbody.ShoppingClt.ShoppingReqData.ShppingBuyReq.GiftToDesc,szDesc,SHORT_DESC_LEN);
	}
	if( giftFlag == 1)
	{
		 tagFriendInfo* info = GameFriendManager::getSingleton().getFriendInfoByName(GiftToPlayerName);
		 if( info != NULL&&info->m_nRelationType == CLIENT_RELATION_TYPE_FRIEND)
		 {
			 memcpy(&msgbody.ShoppingClt.ShoppingReqData.ShppingBuyReq.GiftToRoleID,&info->m_RoleID,sizeof(tagLong_ToLua));
		 }else{
			 GameMsgPrint::getSingleton().showMidTip(MID_MARKET_NOT_FRIEND);
			 return;
		 }
	}
	msgbody.ShoppingClt.ShoppingReqData.ShppingBuyReq.ShoppingID = id;
	msgbody.ShoppingClt.ShoppingReqData.ShppingBuyReq.ShoppingNum = num;
	SendGameServerMsg( SHOPPING_CLT, msgbody );

	if( nBuyFlag == BUY_TYPE_TICKET )
	{
		for( int i = 0 ; i < ONE_PAGE_NUM ; i++ )
		{
			if( m_LatestTicketBuy[i] == id )
			{
				for( int j = i ; j < ONE_PAGE_NUM - 1 ; j++ )
				{
					m_LatestTicketBuy[j] = m_LatestTicketBuy[j+1];
				}
				m_LatestTicketBuy[ONE_PAGE_NUM-1] = 0;
			}
			if( m_LatestTicketBuy[i] == 0 )
			{
				m_LatestTicketBuy[i] = id;
				break;
			}else if( i == ONE_PAGE_NUM - 1 )
			{
				for( int j = 0 ; j < i ; j++ )
				{
					m_LatestTicketBuy[j] = m_LatestTicketBuy[j+1];
				}
				m_LatestTicketBuy[i] = id;
			}
			GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_MARKET_LATEST_TICK_BUY, (char*)m_LatestTicketBuy );
		}
	}else if( nBuyFlag == BUY_TYPE_BINDGOLD )
	{
		for( int i = 0 ; i < ONE_PAGE_NUM ; i++ )
		{
			if( m_LatestGoldCoinBuy[i] == id )
			{
				for( int j = i ; j < ONE_PAGE_NUM - 1 ; j++ )
				{
					m_LatestGoldCoinBuy[j] = m_LatestGoldCoinBuy[j+1];
				}
				m_LatestGoldCoinBuy[ONE_PAGE_NUM-1] = 0;
			}
			if( m_LatestGoldCoinBuy[i] == 0 )
			{
				m_LatestGoldCoinBuy[i] = id;
				break;
			}else if( i == ONE_PAGE_NUM - 1 )
			{
				for( int j = 0 ; j < i ; j++ )
				{
					m_LatestGoldCoinBuy[j] = m_LatestGoldCoinBuy[j+1];
				}
				m_LatestGoldCoinBuy[i] = id;
			}
		}
		GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_MARKET_LATEST_GOLD_COIN_BUY, (char*)m_LatestGoldCoinBuy );
	}
}

void CMarketManager::resetMarketInfo()
{
	memset( &m_showMsg, 0, sizeof(SHOPMSGDEF) );
	m_MarketItem.clear();
	m_CurSelItem.clear();
	m_TypeLabel.clear();
	m_nCurDianquan = 0;

	for( int i = 0 ; i < ONE_PAGE_NUM ; i++ )
	{
		m_LatestGoldCoinBuy[i] = 0;
		m_LatestTicketBuy[i] = 0;
	}
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_GET_XML_FILE_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingGetXmlFileReq.Version = 0;
	SendGameServerMsg( SHOPPING_CLT, msgbody );
	msgbody.ShoppingClt.Type = SHOPPING_FRESH_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingFreshRecordReq.ShopTbVersion = 0;
	SendGameServerMsg(SHOPPING_CLT,msgbody);

	m_nSellRecordIndex	= 0;
	m_nSellRecordNum	= 1;
	m_nSellTotalNum		= 1;
	m_sellChosenVec.clear();
	m_sellRecordVec.clear();

	m_nBuyRecordNum     = 1;
	m_nBuyRecordIndex	= 0;
	m_nBuyTotalNum		= 1;
	m_buyChosenVec.clear();
	m_buyRecordVec.clear();

	m_bMyTradeFlag		= true;
	m_myTradeVec.clear();

	m_RecordSortType = 1;
	m_RecordSortFlag = 1;

	m_uLastBuyTick = 0;
	m_bBuyPreviewItem = false;
}

tagCSShopDef_ToLua* CMarketManager::getMarketItem( int nIndex, int nSelType, int nSelSubType )
{
	static_assert(sizeof(tagCSShopDef_ToLua)==sizeof(SHOPDEF));
	if( nIndex > (int)m_CurSelItem.size() || nIndex < 0 )
	{
		return NULL;
	}
	return (tagCSShopDef_ToLua*)(&m_CurSelItem[nIndex - 1]);
}

void CMarketManager::getLatestType( int& nType, int& nSubType, int nBuyType )
{
	nType = 0;
	nSubType = 0;
	TYPELABELMAP::iterator it = m_TypeLabel.find( nBuyType );
	if( it != m_TypeLabel.end() )
	{
		VTYPELABEL& vLabel = it->second;
		for( size_t i = 0 ; i < vLabel.size() ; i ++ )
		{
			for( int j = 0 ; j < vLabel[i].subLabelCount ; j++ )
			{
				if( strcmp( vLabel[i].subLabel[j].labelName, "最近购买" ) == 0 )
				{
					nType = vLabel[i].key;
					nSubType = vLabel[i].subLabel[j].key;
					return;
				}
			}
		}
	}
}

const char* CMarketManager::getTypeName( int nType, int nBuyType )
{
	TYPELABELMAP::iterator it = m_TypeLabel.find( nBuyType );
	if( it != m_TypeLabel.end() )
	{
		VTYPELABEL& vLabel = it->second;
		for( size_t i = 0 ; i < vLabel.size() ; i ++ )
		{
			if( vLabel[i].key == nType )
			{
				return vLabel[i].labelName;
			}
		}
	}
	return "";
}

int CMarketManager::getSubTypeItemNum(int nSelType,int nSelSubType,int nBuyType)
{
	onSelectType(nSelType,nSelSubType,nBuyType);
	return (int)m_CurSelItem.size();
}

void CMarketManager::onSelectType(int nSelType,int nSelSubType,int nBuyType)
{
	m_CurSelItem.clear();
	m_LastRefreshTime = getServerTime().sec;
	if( nSelType == 0 )
	{
		for( size_t i = 0 ; i < m_MarketItem.size(); i++ )
		{
			if( isInShowTime( m_MarketItem[i] ) )
			{
				if( m_MarketItem[i].BuyType == nBuyType && strcmp( getTypeName(m_MarketItem[i].Type,nBuyType), "特惠区" ) != 0 )
				{
					m_CurSelItem.push_back( m_MarketItem[i] );
				}
			}		
		}
	}else{
		int nLatestType,nLatestSubType;
		getLatestType( nLatestType, nLatestSubType, nBuyType );
		if( nSelType == nLatestType && nSelSubType == nLatestSubType )
		{
			if( nBuyType == BUY_TYPE_TICKET )
			{
				for( int i = ONE_PAGE_NUM - 1 ; i >= 0 ; i -- )
				{
					if( m_LatestTicketBuy[i] > 0  )
					{
						SHOPDEF* pDef = (SHOPDEF*) getItemByID( m_LatestTicketBuy[i] );
						if( pDef != NULL && pDef->BuyType == nBuyType && isInShowTime( *pDef ) )
						{
							m_CurSelItem.push_back( *pDef );
						}
					}
				}
			}else if( nBuyType == BUY_TYPE_BINDGOLD )
			{
				for( int i = ONE_PAGE_NUM - 1 ; i >= 0 ; i -- )
				{
					if( m_LatestGoldCoinBuy[i] > 0  )
					{
						SHOPDEF* pDef = (SHOPDEF*) getItemByID( m_LatestGoldCoinBuy[i] );
						if( pDef != NULL && pDef->BuyType == nBuyType && isInShowTime( *pDef ) )
						{
							m_CurSelItem.push_back( *pDef );
						}
					}
				}
			}
			return;
		}else{
			for( size_t i = 0 ; i < m_MarketItem.size(); i++ )
			{
				if( isInShowTime( m_MarketItem[i] ) )
				{
					if( m_MarketItem[i].BuyType == nBuyType && m_MarketItem[i].Type == nSelType && m_MarketItem[i].SubType == nSelSubType )
					{
						m_CurSelItem.push_back( m_MarketItem[i] );
					}
				}		
			}
		}
	}
	stable_sort( m_CurSelItem.begin(), m_CurSelItem.end(), LessThan );
}

int CMarketManager::getCanByNum( int shopItemID )
{
	tagCSShopDef_ToLua* item = getItemByID( shopItemID );
	if( item == NULL )
	{
		return 0;
	}

	int nPrice = item->BuyPrice*(100 - item->Sale)/100 ;
	int num = 0;
	for( ; num < SHOP_ITEM_COUNT; num ++ )
	{
		if( item->Items[num].ItemID == 0 )
			break;
	}
	if( num == 0 )
	{
		return 0;
	}
	int nMaxNum = MAX_CAN_BUY_NUM / num;
	if( nPrice == 0 )
	{
		return nMaxNum;
	}	
	
	if( item->BuyType == 1 )
	{
		int nHave = GamePlayManager::getSingleton().getMainPlayer()->getGodCoin();
		nMaxNum = nMaxNum < nHave/nPrice ? nMaxNum : ( nHave/nPrice );
	}else{
		int nHave = GamePlayManager::getSingleton().getMainPlayer()->getBindGodCoin();
		nMaxNum = nMaxNum < nHave/nPrice ? nMaxNum : ( nHave/nPrice );
	}

	tagShopLimitDef_ToLua* pLimit = getShopLimit( shopItemID );
	if( pLimit != NULL )
	{
		int nLeft = pLimit->LimitSellTotalNum - pLimit->LimitSelledNum;
		nMaxNum = nMaxNum < nLeft ? nMaxNum : nLeft;
	}	
	return nMaxNum;
}

tagCSShopDef_ToLua* CMarketManager::getItemByID( int shopItemID )
{
	static_assert( sizeof(tagCSShopDef_ToLua) == sizeof(SHOPDEF) );
	for( size_t i = 0 ; i < m_MarketItem.size(); i ++ )
	{
		if( m_MarketItem[i].ShopID == shopItemID )
		{
			return (tagCSShopDef_ToLua*)(&m_MarketItem[i]);
		}
	}
	return NULL;
}

void CMarketManager::selectMarketItem( int nItemID )
{
	//TODO HERE
}

int CMarketManager::getCurSaleTotalPage(int nType,int nSubType)
{
	if( m_CurSelItem.empty() )
	{
		return 1;
	}
	return (int)((m_CurSelItem.size()-1)/ONE_PAGE_NUM + 1);
}

void CMarketManager::requestQueryTianyuan()
{
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_QUERY_TIANYUAN_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryTianYuanReq.RoleID = GamePlayManager::getSingleton().getMainPlayer()->getRoleWId();
	SendGameServerMsg( SHOPPING_CLT, msgbody );
}

bool CMarketManager::onPackagePresentResponse(const CSSHOPPINGPACKAGEPRESENTRES& packagePresent)
{
	if( packagePresent.Result == 0 )
	{
		GameMsgPrint::getSingleton().showMidTip(MID_MARKET_PACKAGE_PRESENT_SUCESS);
	}
	return true;
}

void CMarketManager::requestMoveAllToBag()
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type = PACKAGE_REQ_MOVE_ALL_SHOPPING ;
	msgbody.PackageReq.PackageReqData.PackageMoveAllShopping.ListType = CONTAINER_TYPE_MARKET;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
}

void CMarketManager::requestPackagePresent(int nGridIndex,int num,const char* GiftToPlayerName,const char* szDesc)
{
	if( strlen(GiftToPlayerName) == 0 )
	{
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_PACKAGE_PRESENT_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingPackagePresentReq.GiftFlag = 1;
	tagFriendInfo* info = GameFriendManager::getSingleton().getFriendInfoByName(GiftToPlayerName);
	if( info != NULL&&info->m_nRelationType == CLIENT_RELATION_TYPE_FRIEND )
	{
		memcpy(&msgbody.ShoppingClt.ShoppingReqData.ShoppingPackagePresentReq.GiftToRoleID,&info->m_RoleID,sizeof(tagLong_ToLua));
	}else{
		GameMsgPrint::getSingleton().showMidTip(MID_MARKET_NOT_FRIEND);
		return;
	}

	strncpy(msgbody.ShoppingClt.ShoppingReqData.ShoppingPackagePresentReq.GiftToDesc,szDesc,SHORT_DESC_LEN);
	strncpy(msgbody.ShoppingClt.ShoppingReqData.ShoppingPackagePresentReq.GiftToPlayerName,GiftToPlayerName,ROLE_NAME_LEN);
	msgbody.ShoppingClt.ShoppingReqData.ShoppingPackagePresentReq.ItemNum = num;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingPackagePresentReq.SrcGridIdx = nGridIndex;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingPackagePresentReq.SrcListType = CONTAINER_TYPE_MARKET;
	SendGameServerMsg(SHOPPING_CLT,msgbody);
}

void CMarketManager::requestQueryTradeSell(unsigned int nBeginNum,unsigned int nTotalNum)
{
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_QUERY_TRADESELL_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryTradeSellReq.QueryBeginNum = nBeginNum;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryTradeSellReq.QueryTotalNum = nTotalNum;
	SendGameServerMsg(SHOPPING_CLT,msgbody);
}

bool CMarketManager::onQueryTradeSellResponse(const CSSHOPPINGQUERYTRADESELLRES& tradeSells)
{	
	memcpy(m_TradeRecord,tradeSells.TradeRecord,sizeof(m_TradeRecord));
	m_nTradeNum = tradeSells.RecordNum;
	m_nTotalNum = tradeSells.SvrCurrTotalNum;
	GameManager::getSingleton().SendUIEvent("GE_UPDATE_TRADE_LIST");
	return true;
}

void CMarketManager::requestQueryTradeBuy(unsigned int nBeginNum,unsigned int nTotalNum)
{
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_QUERY_TRADEBUY_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryTradeBuyReq.QueryBeginNum = nBeginNum;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryTradeBuyReq.QueryTotalNum = nTotalNum;
	SendGameServerMsg(SHOPPING_CLT,msgbody);
}

bool CMarketManager::onQueryTradeBuyResponse(const CSSHOPPINGQUERYTRADEBUYRES& tradeBuys)
{
	memcpy(m_TradeRecord,tradeBuys.TradeRecord,sizeof(m_TradeRecord));
	m_nTradeNum = tradeBuys.RecordNum;
	m_nTotalNum = tradeBuys.SvrCurrTotalNum;
	GameManager::getSingleton().SendUIEvent("GE_UPDATE_TRADE_LIST");
	return true;
}

void CMarketManager::requestQueryMyRecord(int nNum, int nTotalNum)
{	
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_QUERY_MY_TRADERECORD_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryMyTradeRecordReq.QueryBeginNum = nNum;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryMyTradeRecordReq.QueryTotalNum = nTotalNum;
	SendGameServerMsg( SHOPPING_CLT,msgbody );
}

bool CMarketManager::onQueryMyTradeRecordResponse(const CSSHOPPINGQUERYMYTRADERECORDRES& myRecord)
{
	memcpy(m_myTradeRecord,myRecord.TradeRecord,sizeof(m_myTradeRecord));
	m_nMyTradeNum = myRecord.RecordNum;
	setMyDisplayRecord();
	GameManager::getSingleton().SendUIEvent( "GE_UPDATE_TRADE_LIST" );
	return true;
}

void CMarketManager::requestTrade(int type,unsigned long long tradeId)
{
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_TRADE_REQ;

	msgbody.ShoppingClt.ShoppingReqData.ShoppingTradeReq.TradeID = tradeId;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingTradeReq.TradeType = type;
	SendGameServerMsg(SHOPPING_CLT,msgbody);
}

bool CMarketManager::onTradeResponse(const CSSHOPPINGTRADERES& tradeRes)
{
	if( tradeRes.Result == 0  )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_MARKET_TRADE_SUCESS );
	}
	return true;
}

void CMarketManager::requestSendTradeRecord(int type, int nMoney,int nGold,int nTime,int nAnonymous )
{
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_SEND_TRADERECORD_REQ;  //elmer
	msgbody.ShoppingClt.ShoppingReqData.ShoppingSendTradeRecordReq.Money = nMoney;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingSendTradeRecordReq.Time = nTime;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingSendTradeRecordReq.Gold = nGold;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingSendTradeRecordReq.TradeType = type;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingSendTradeRecordReq.NotAnonymous = nAnonymous;
	SendGameServerMsg(SHOPPING_CLT,msgbody);
}

bool CMarketManager::onSendTradeRecordResponse(const CSSHOPPINGSENDTRADERECORDRES& sendRecord)
{
	if( sendRecord.Result == 0 )
	{
		tagTradeRecordSortInfo_ToLua recordMyInfo;
		setRecordInfo( &recordMyInfo, &sendRecord.TradeRecord );
		m_myTradeVec.push_back( recordMyInfo );
	
		GameManager::getSingleton().SendUIEvent( "GE_UPDATE_TRADE_LIST" );
	}
	return true;

}

void CMarketManager::searchTradeByID(char* szID,int type)
{
	if( type == TRADE_LIST_SALE_TYPE || type == TRADE_LIST_BUY_TYPE )
	{
		tdr_ulonglong id =_atoi64(szID);
		tagCSPkgBody msgbody;
		msgbody.ShoppingClt.Type = SHOPPING_QUERY_RECORD_BY_ID_REQ;
		msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryRecordByIDReq.TradeID = id;
		msgbody.ShoppingClt.ShoppingReqData.ShoppingQueryRecordByIDReq.TradeType = type;
		SendGameServerMsg( SHOPPING_CLT,msgbody );
	}else if( type == TRADE_LIST_MY_TRADE_TYPE ){
		tdr_ulonglong id =_atoi64(szID);
		int nCount = 0;
		for( int i = 0 ; i < m_nMyTradeNum ; i ++ )
		{
			if(m_myTradeRecord[i].TradeID == id )
			{
				m_TradeRecord[nCount ++] = m_myTradeRecord[i];
			}
		}
		m_nTotalNum = nCount;
		m_nTradeNum = nCount;
		GameManager::getSingleton().SendUIEvent( "GE_UPDATE_TRADE_LIST" );
		if(nCount == 0 )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_MARKET_NO_SEARCH );
		}
	}
}

bool CMarketManager::onSearchTradeRespone(const CSSHOPPINGQUERYRECORDBYIDRES& record)
{	
	if( record.Result == 0 )
	{
		if(record.OperType == 0)  //表示客户端查询
		{
			return true;
		}else if( record.OperType == 1 ) //表示更新数据
		{
			return true;
		}else if ( record.OperType == 2 )  //表示删除数据
		{
			/*std::vector<tagTradeRecordSortInfo_ToLua>::iterator it = std::find( m_myTradeVec.begin(), m_myTradeVec.end(), record.TradeRecord.TradeID);
			if( it != m_myTradeVec.end() )
			{
				m_myTradeVec.erase(it);
			}*/
			std::vector<tagTradeRecordSortInfo_ToLua>::iterator it;
			for ( it = m_myTradeVec.begin(); it != m_myTradeVec.end(); ++it )
			{
				if ( it->TradeRecordData.TradeID == record.TradeRecord.TradeID && 
					it->TradeRecordData.TradeType == record.TradeRecord.TradeType )
				{
					m_myTradeVec.erase(it);
					break;
				}
			}
			std::vector<tagTradeRecordSortInfo_ToLua>::iterator itBuy = std::find( m_buyRecordVec.begin(), m_buyRecordVec.end(), record.TradeRecord.TradeID);
			if( itBuy != m_buyRecordVec.end() && itBuy->TradeRecordData.TradeType == record.TradeRecord.TradeType  )
			{
				m_buyRecordVec.erase(itBuy);
			}
			std::vector<tagTradeRecordSortInfo_ToLua>::iterator itSell = std::find( m_sellRecordVec.begin(), m_sellRecordVec.end(), record.TradeRecord.TradeID);
			if( itSell != m_sellRecordVec.end() && itSell->TradeRecordData.TradeType == record.TradeRecord.TradeType )
			{
				m_sellRecordVec.erase( itSell );
			}
			GameManager::getSingleton().SendUIEvent( "GE_UPDATE_TRADE_LIST" );
		}
	}
	return true;
}
bool CMarketManager::onExchangePointResponse( const CSSHOPPINGEXCHANGERES& exchangePoint )
{
	if ( exchangePoint.Result == 0 
		|| exchangePoint.Result == BILL_DEC_NO_ENOUGH_CASH
		|| exchangePoint.Result == BILL_DEC_NO_CASH )
	{
		m_nCurDianquan = (int) exchangePoint.LeftPoint ;
		GameManager::getSingleton().SendUIEvent("UI_MARKET_TIANYUAN_RESPONSE");
	}
	return true;
}

void CMarketManager::destroyTradeRecord(unsigned long long nTradeID,int nType)
{
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_DESTROY_TRADERECORD_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingDestroyTradeRecordReq.TradeID = nTradeID;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingDestroyTradeRecordReq.TradeType = nType;
	SendGameServerMsg(SHOPPING_CLT,msgbody);
}

bool CMarketManager::onDestroyTradeRecord(const CSSHOPPINGDESTROYTRADERECORDRES& record)
{
	if ( record.Result == 0 )
	{
		std::vector<tagTradeRecordSortInfo_ToLua>::iterator it;
		for ( it = m_myTradeVec.begin(); it != m_myTradeVec.end(); ++it )
		{
			if ( it->TradeRecordData.TradeID == record.TradeID && it->TradeRecordData.TradeType == record.TradeType )
			{
				m_myTradeVec.erase(it);
				break;
			}
		}
	}	
	GameManager::getSingleton().SendUIEvent( "GE_UPDATE_TRADE_LIST" );
	return true;
}

tagTradeRecord_ToLua* CMarketManager::getTradeRecord(int nIndex)
{
	static_assert( sizeof(tagTradeRecord_ToLua) == sizeof(TRADERECORD) );
	if( nIndex < m_nTradeNum )
	{
		return (tagTradeRecord_ToLua*)(&m_TradeRecord[nIndex]);
	}
	return NULL;
}

void CMarketManager::showTradeInfo(int nPage,int nCurType)
{
	switch(nCurType)
	{
	case TRADE_LIST_SALE_TYPE:
		requestQueryTradeSell((nPage - 1)*ONE_PAGE_TRADE_LIST_NUM,ONE_PAGE_TRADE_LIST_NUM);
		break;
	case TRADE_LIST_BUY_TYPE:
		requestQueryTradeBuy((nPage - 1)*ONE_PAGE_TRADE_LIST_NUM,ONE_PAGE_TRADE_LIST_NUM);
		break;
	case TRADE_LIST_MY_TRADE_TYPE:
		//我的订单一次全部取完
		requestQueryMyRecord(0,MAX_TRADE_RECORD);
		m_nCurMyPage = nPage;
		break;
	default:
		break;
	}
	m_nTotalNum = 0;
	m_nTradeNum = 0;
	m_nMyTradeNum = 0;
}

void CMarketManager::setMyDisplayRecord()
{
	int nCount = 0;
	for(int i = (m_nCurMyPage - 1)*ONE_PAGE_TRADE_LIST_NUM ; i < m_nMyTradeNum && nCount < ONE_PAGE_TRADE_LIST_NUM ; i ++ )
	{
		m_TradeRecord[nCount] = m_myTradeRecord[i];
		nCount ++;
	}
	m_nTradeNum  = nCount;
	m_nTotalNum = m_nMyTradeNum;
}

int	CMarketManager::getTotalRecordNum()
{
	return m_nTotalNum;
}

long string2time(const char * szTime)
{
	struct tm tmTime;
	time_t   tTime;

	sscanf_s(szTime,"%4d%2d%2d%2d%2d%2d",
		&tmTime.tm_year,
		&tmTime.tm_mon,
		&tmTime.tm_mday,
		&tmTime.tm_hour,
		&tmTime.tm_min,
		&tmTime.tm_sec);
	tmTime.tm_year -= 1900;
	tmTime.tm_mon --;
	tmTime.tm_isdst=-1;

	tTime = mktime(&tmTime);

	return tTime;
}

void CMarketManager::requestOpenMarket()
{
	tagCSPkgBody msgbody;
	msgbody.ShoppingClt.Type = SHOPPING_OPEN_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingOpenReq.OperType = 0;
	SendGameServerMsg(SHOPPING_CLT,msgbody);
}

tagShopLimitDef_ToLua* CMarketManager::getShopLimit( int shopID )
{
	for( size_t i = 0 ; i < m_ShopLimit.size(); i ++ )
	{
		if( m_ShopLimit[i].ShopID == shopID )
		{
			return (tagShopLimitDef_ToLua*)&m_ShopLimit[i];
		}
	}
	return NULL;
}

int CMarketManager::getShopItemNum()
{
	return (int)m_MarketItem.size();
}

tagCSShopDef_ToLua* CMarketManager::getShopItemByIndex( int nIdex )
{
	if( nIdex >= m_MarketItem.size() )
	{
		assert(0);
		return NULL;
	}
	return (tagCSShopDef_ToLua*)(&m_MarketItem[nIdex]);
}

void CMarketManager::requestExchangePoint( int nNum )
{
	tagCSPkgBody msgbody;
	msgbody.ShoppingSvr.Type = SHOPPING_EXCHANGE_POINT_REQ;
	msgbody.ShoppingClt.ShoppingReqData.ShoppingExchangeReq.PointNum = nNum;
	MainGamePlayer* pMain = GamePlayManager::getSingleton().getMainPlayer();
	msgbody.ShoppingClt.ShoppingReqData.ShoppingExchangeReq.RoleID = pMain->getRoleWId();
	strcpy(msgbody.ShoppingClt.ShoppingReqData.ShoppingExchangeReq.RoleName,pMain->getName());
	SendGameServerMsg(SHOPPING_CLT,msgbody);
}

void CMarketManager::searchItems( const char* name,int nBuyType )
{
	m_CurSelItem.clear();
	for( size_t i = 0 ; i < m_MarketItem.size() ; i++ )
	{
		if( isInShowTime( m_MarketItem[i] ) )
		{
			if( m_MarketItem[i].BuyType == nBuyType && strstr( m_MarketItem[i].Name,name ) )
			{
				m_CurSelItem.push_back( m_MarketItem[i] );
			}
		}
	}
	stable_sort(m_CurSelItem.begin(),m_CurSelItem.end(),LessThan);
}

const char* CMarketManager::getNextRefreshTime( const tagShopLimitDef_ToLua* limit )
{
	if( limit == NULL )
	{
		return "";
	}
	int i = 0;
	time_t t = getServerTime().sec;
	struct tm* curTime;
	curTime = gmtime( &t );
	int serverSec = ( curTime->tm_hour + 8 )*60*60 + curTime->tm_min*60 + curTime->tm_sec;
	for( i = 0 ; i < MAX_SHOP_LIMIT_ITME_COUNT ; i++ )
	{
		if( strlen( limit->ShopLimitTime[i].SellBeginTime ) > 0 )
		{
			int hour,min,sec;
			sscanf_s( limit->ShopLimitTime[i].SellBeginTime, "%d:%d", &hour, &min );
			sec = hour*60*60 + min*60;
			if( sec > serverSec )
			{
				break;
			}
		}
	}
	if( i >= MAX_SHOP_LIMIT_ITME_COUNT )
	{
		i = 0;
	}
	return limit->ShopLimitTime[i].SellBeginTime;
}

const char* CMarketManager::getShowMsg()
{
	return m_showMsg.ShopMsg;
}

bool CMarketManager::CheckShowDate()
{
	time_t t = getServerTime().sec;
	for( size_t i = 0 ; i < m_CurSelItem.size() ; i++ )
	{
		if( strlen( m_CurSelItem[i].ShowDate ) > 0 )
		{
			time_t showDate = string2time( m_CurSelItem[i].ShowDate );
			if( t >= showDate && m_LastRefreshTime < showDate )
			{
				return true;
			}
		}
		if( strlen( m_CurSelItem[i].HideDate ) > 0 )
		{
			time_t hideDate = string2time( m_CurSelItem[i].HideDate );
			if( t >= hideDate && m_LastRefreshTime < hideDate )
			{
				return true;
			}
		}
	}
	return false;
}

bool CMarketManager::isInShowTime( const SHOPDEF& shopdef )
{
	time_t t = getServerTime().sec;
	if( strlen( shopdef.ShowDate ) > 0 )
	{
		time_t showDate = string2time( shopdef.ShowDate );
		if( t < showDate )
		{
			return false;
		}
	}
	if( strlen(shopdef.HideDate ) > 0 )
	{
		time_t hideDate = string2time( shopdef.HideDate );
		if( t > hideDate )
		{
			return false;
		}
	}
	return true;
}

void CMarketManager::checkLatestBuyData()
{
	bool bChange = false;
	for( int i = 0 ; i < ONE_PAGE_NUM ; i ++ )
	{
		if( m_LatestGoldCoinBuy[i] > 0 && getItemByID( m_LatestGoldCoinBuy[i] ) == NULL )
		{
			for( int j = ONE_PAGE_NUM - 1 ; j > i ; j -- )
			{
				m_LatestGoldCoinBuy[j--] = m_LatestTicketBuy[j];
				bChange = true;
			}
		}
	}
	if( bChange )
	{
		GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_MARKET_LATEST_GOLD_COIN_BUY, (char*)m_LatestGoldCoinBuy );
	}
	bChange = false;
	for( int i = 0 ; i < ONE_PAGE_NUM ; i ++ )
	{
		if( m_LatestTicketBuy[i] > 0 && getItemByID( m_LatestTicketBuy[i] ) == NULL )
		{
			for( int j = ONE_PAGE_NUM - 1 ; j > i ; j -- )
			{
				m_LatestTicketBuy[j--] = m_LatestTicketBuy[j];
				bChange = true;
			}
		}
	}
	if( bChange )
	{
		GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_MARKET_LATEST_TICK_BUY, (char*)m_LatestTicketBuy );
	}
}

void CMarketManager::update()
{
	if( GameManager::getSingleton().getTickTime() - m_uLastRequestTick >= 1*1000 )
	{
		if ( m_nSellRecordNum != 0 && m_nSellTotalNum != 0 )
		{
			requestQueryTradeSell( m_nSellRecordIndex ,3 * ONE_PAGE_TRADE_LIST_NUM );	
			m_nSellRecordIndex	= m_nSellRecordIndex + 3 * ONE_PAGE_TRADE_LIST_NUM;
			m_nSellRecordNum	= 0; 
		}
		if ( m_nBuyRecordNum != 0 && m_nBuyTotalNum != 0 )
		{
			requestQueryTradeBuy( m_nBuyRecordIndex, 3 * ONE_PAGE_TRADE_LIST_NUM );
			m_nBuyRecordIndex	= m_nBuyRecordIndex + 3 * ONE_PAGE_TRADE_LIST_NUM;
			m_nBuyRecordNum		= 0;
		}
		if ( m_bMyTradeFlag )
		{
			requestQueryMyRecord( 0 ,MAX_SIGLE_USER_TRADERECORD_NUM );
			m_bMyTradeFlag = false;
		}
		m_uLastRequestTick	= GameManager::getSingleton().getTickTime();
	}

	if ( m_bBuyPreviewItem && GameManager::getSingleton().getTickTime() - m_uLastBuyTick >= 1*100 )
	{
		GameManager::getSingleton().getScriptVM()->callString( "BuyAllPreViewItem()" );
		m_uLastBuyTick = GameManager::getSingleton().getTickTime();	
	}
}
void CMarketManager::modifyTradeRecordData( tagTradeRecord recordData ,int recordType)
{
	std::vector<tagTradeRecordSortInfo_ToLua>::iterator itSell = std::find( m_sellRecordVec.begin(), m_sellRecordVec.end(), recordData.TradeID);
	std::vector<tagTradeRecordSortInfo_ToLua>::iterator itBuy = std::find( m_buyRecordVec.begin(), m_buyRecordVec.end(), recordData.TradeID);

	switch( recordType )
	{
	case RECORD_SELL_TRADE_LIST:
		//一次性购买的，无需更新交易信息
		if ( itSell == m_sellRecordVec.end() )
		{	
			tagTradeRecordSortInfo_ToLua recordInfo;
			setRecordInfo( &recordInfo, &recordData );
			m_sellRecordVec.push_back( recordInfo );
		}
		break;
	case RECORD_BUY_TRADE_LIST:
		if ( itBuy == m_buyRecordVec.end() )
		{	
			tagTradeRecordSortInfo_ToLua recordInfo;
			setRecordInfo( &recordInfo, &recordData );
			m_buyRecordVec.push_back( recordInfo );
		}	
		break;
	case RECORD_MY_TRADE_LIST:
		tagTradeRecordSortInfo_ToLua recordInfo;
		setRecordInfo( &recordInfo, &recordData );
		m_myTradeVec.push_back( recordInfo );
		break;
	}
} 

//设置交易列表
void CMarketManager::setRecordInfo( tagTradeRecordSortInfo_ToLua* pRecordInfo, const void* pInfo)
{
	static_assert( sizeof( tagTradeRecord_ToLua ) == sizeof( tagTradeRecord ) );
	tagTradeRecord_ToLua* pRecordData	= ( tagTradeRecord_ToLua* )pInfo;
	//pRecordInfo->TradeRecordData		= * pRecordData;
	float unitPrice ;
	memcpy( &pRecordInfo->TradeRecordData, pRecordData, sizeof( tagTradeRecord_ToLua ) );

	unitPrice = (float) pRecordData->Money / pRecordData->Gold;
	pRecordInfo->UnitPrice = unitPrice;
}
void CMarketManager::setTradeRecordByNum( int tradeType, int num)
{
	if ( tradeType == 1 && m_sellRecordVec.size() != 0 )
	{	
		m_sellChosenVec.clear();
		for ( int i = 0 ;i < m_sellRecordVec.size() ; i++ )
		{
			if ( m_sellRecordVec[i].TradeRecordData.Gold == num )
			{
				m_sellChosenVec.push_back( m_sellRecordVec[i] );
			}
			if ( num == 0 )
			{
				m_sellChosenVec.push_back( m_sellRecordVec[i] );
			}
		}
	}
	else if( tradeType == 2 && m_buyRecordVec.size() != 0 )
	{
		m_buyChosenVec.clear();
		for( int i = 0;i < m_buyRecordVec.size(); i++ )
		{
			if ( m_buyRecordVec[i].TradeRecordData.Money == num )
			{
				m_buyChosenVec.push_back( m_buyRecordVec[i] );
			}
			//num==0 表示全部
			if ( num == 0 )
			{
				m_buyChosenVec.push_back( m_buyRecordVec[i] );
			}
		}
	}
	if ( m_sellRecordVec.size() == 0 )
	{
		m_sellChosenVec.clear();
	}
	if ( m_buyRecordVec.size() == 0 )
	{
		m_buyChosenVec.clear();
	}
}
// 获取服务器指定页查询信息  卖金币||卖银币||订单
const tagTradeRecordSortInfo_ToLua* CMarketManager::getOneTradeInfoList( int nPage, int nRow, int listType )const
{
	size_t nIndex = nPage*ONE_PAGE_TRADE_LIST_NUM + nRow ;

	if( listType == 1 )
	{
		if( nIndex < m_sellChosenVec.size() )
		{ 
			return &m_sellChosenVec[nIndex];
		}
	}
	else if( listType == 2 )
	{
		if ( nIndex < m_buyChosenVec.size() )
		{
			return &m_buyChosenVec[nIndex];
		}
	}	
	else
	{	
		if ( nIndex < m_myTradeVec.size() )
		{
			return &m_myTradeVec[nIndex];
		}
	}
	return NULL;
}

float CMarketManager::getUnitPriceMin( int nType )
{
	if ( nType == 0 )
	{
		std::vector<tagTradeRecordSortInfo_ToLua>::iterator its = std::min_element( m_sellRecordVec.begin(),m_sellRecordVec.end(),UnitPriceLessThan);
		if (m_sellRecordVec.size() > 0)
		{
			return its->UnitPrice;
		}
		return 0;		
	}
	else
	{
		std::vector<tagTradeRecordSortInfo_ToLua>::iterator itb = std::max_element( m_buyRecordVec.begin(),m_buyRecordVec.end(),UnitPriceLessThan);
		if ( m_buyRecordVec.size() > 0 )
		{
			return itb->UnitPrice;
		}
		return 0;	
	}
}
bool TradeRecordCompare( const tagTradeRecordSortInfo_ToLua& lhs, const tagTradeRecordSortInfo_ToLua& rhs )
{
	int nType = CMarketManager::getSingleton().getRecordSortType();
	int nFlag = CMarketManager::getSingleton().getRecordSortFlag();
	switch ( nType )
	{
	case  RECORD_SORT_NUM:
		{
			return ( ( int )( lhs.TradeRecordData.Gold- rhs.TradeRecordData.Gold ) ) * nFlag > 0?true:false;
		}
		break;
	case RECORD_SORT_PRICE:
		{
			return ( ( int )( lhs.TradeRecordData.Money - rhs.TradeRecordData.Money ) ) * nFlag > 0?true:false;
		}
		break;
	case RECORD_SORT_UNIT_PRICE:
		{
			return ( ( float )( lhs.UnitPrice - rhs.UnitPrice ) ) * nFlag > 0?true:false;
		}
		break;
	}
	return true;
}
bool UnitPriceLessThan( const tagTradeRecordSortInfo_ToLua& lhs, const tagTradeRecordSortInfo_ToLua& rhs )
{
	//return  ( int )( lhs.UnitPrice - rhs.UnitPrice ) < 0?true:false;
	return lhs.UnitPrice < rhs.UnitPrice;
}

void CMarketManager::sortTradeRecord( int nType, int nFlag,int tradeType )
{
	m_RecordSortType = nType;
	m_RecordSortFlag = nFlag;
	if( tradeType == 1 )
	{
		std::stable_sort( m_sellChosenVec.begin(),m_sellChosenVec.end(),TradeRecordCompare );
	}
	else
	{
		std::stable_sort( m_buyChosenVec.begin(),m_buyChosenVec.end(),TradeRecordCompare );
	}

}
int CMarketManager::getCurTradeTotalPage(int nType )
{
	if ( nType == RECORD_SELL_TRADE_LIST && !m_sellChosenVec.empty())
	{
		return (int)( ( m_sellChosenVec.size() - 1 ) / ONE_PAGE_TRADE_LIST_NUM + 1 );
	}
	else if ( nType == RECORD_BUY_TRADE_LIST && !m_buyChosenVec.empty())
	{
		return (int)( ( m_buyChosenVec.size() - 1 )/ ONE_PAGE_TRADE_LIST_NUM + 1 );
	}
	else if ( nType == RECORD_MY_TRADE_LIST && !m_myTradeVec.empty())
	{
		return (int)( ( m_myTradeVec.size() - 1 )/ ONE_PAGE_TRADE_LIST_NUM + 1 );
	}
	else
	{
		return 1;
	}

}
bool CMarketManager::checkMyTradeRecord( unsigned long long nTradeID )
{
	std::vector<tagTradeRecordSortInfo_ToLua>::iterator it = std::find( m_myTradeVec.begin(), m_myTradeVec.end(), nTradeID );
	if (it != m_myTradeVec.end())
	{
		return true;
	}else
	{
		return false;
	}
}
bool operator== ( const tagTradeRecordSortInfo_ToLua& recordInfo,  const tdr_ulonglong tradeID )
{
	return *( tdr_ulonglong* )( &recordInfo.TradeRecordData.TradeID ) == tradeID;

}

void CMarketManager::SetBuyPreviewState( bool state )
{
	m_bBuyPreviewItem = state;
}

void CMarketManager::ChangeRecordName( const char* oldName, const char* newName )
{
	if ( m_buyRecordVec.size() != 0 )
	{
		std::vector<tagTradeRecordSortInfo_ToLua>::iterator it = m_buyRecordVec.begin();
		for( ; it != m_buyRecordVec.end() ; it++ )
		{
			if ( strncmp( it->TradeRecordData.OwnerName,oldName,ROLE_NAME_LEN_SAVE ) == 0 )
			{
				strncpy( it->TradeRecordData.OwnerName, newName, ROLE_NAME_LEN_SAVE );		
			}
		}
	}

	if ( m_sellRecordVec.size() != 0 )
	{
		std::vector<tagTradeRecordSortInfo_ToLua>::iterator it = m_sellRecordVec.begin();
		for( ; it != m_sellRecordVec.end() ; it++ )
		{
			if ( strncmp( it->TradeRecordData.OwnerName,oldName,ROLE_NAME_LEN_SAVE ) == 0 )
			{
				strncpy( it->TradeRecordData.OwnerName, newName, ROLE_NAME_LEN_SAVE );	
			}
		}
	}

	GameManager::getSingleton().SendUIEvent( "GE_UPDATE_TRADE_NAME" );
}

int CMarketManager::getShopIDByID( int buyType, int ItemID )
{
	for( size_t i = 0 ; i < m_MarketItem.size(); i ++ )
	{
		if( m_MarketItem[i].BuyType == buyType && m_MarketItem[i].Items[0].ItemID == ItemID )
		{
			return m_MarketItem[i].ShopID;
		}
	}
	return 0;
}

void CMarketManager::openAdLink( const char* url )
{
	ifstream fin(url);
	if ( !fin.fail() )
	{
		string path;
		getline( fin, path );
		if ( path.length() != 0 )
		{
			::ShellExecute( NULL, "open", path.c_str(), NULL, NULL, SW_SHOWNORMAL );
		}
		fin.close();
	}
}