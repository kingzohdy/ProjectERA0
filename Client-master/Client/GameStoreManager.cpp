#include "GameStoreManager.h"
#include "proto_cs.h"
#include "GameEngineTypes.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameMsgPrint.h"
#include "GameDBData.h"
#include "GameUIMsg.h"

int GetCeilFormC( float x )
{
	return ceil(x);
}

GameStoreManager::GameStoreManager()
{
	INetworkManager::getSingleton().RegisterMsgHandler( STORE_SVR, this );
}

GameStoreManager::~GameStoreManager()
{

}

int GameStoreManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if ( msgid != STORE_SVR )
	{
		return 0;
	}
	switch( msgbody.StoreSvr.Cmd )
	{
	case STORE_SVR_OP_ONE_INFO:
		{
			m_mapItemChg.clear();
			m_vecStore.clear();
			m_vecStore.push_back( msgbody.StoreSvr.Data.One.Store );

			char buffer[256];
			sprintf( buffer, "UpdateStoreInfo(%d)", msgbody.StoreSvr.Data.One.Self );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case STORE_SVR_OP_PAGE:
		{
			static_assert( sizeof(tagStorePageSvr_ToLua) == sizeof(tagStorePageSvr) );
			tagUIMsgBody UI_Msg;
			memcpy( &UI_Msg.StoreData.Page, &msgbody.StoreSvr.Data.Page, sizeof(tagStorePageSvr_ToLua) );
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_STORE_PAGE" );
		}
		break;
	case STORE_SVR_OP_GRID_CHG:
		{
			char buffer[256];
			sprintf( buffer, "UpdateStoreGrid(%d,%d)", msgbody.StoreSvr.Data.GridChg.ID, msgbody.StoreSvr.Data.GridChg.Max );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case STORE_SVR_OP_MONEY_CHG:
		{
			char buffer[256];
			sprintf( buffer, "UpdateStoreMoney(%d,%d)", msgbody.StoreSvr.Data.MoneyChg.ID, msgbody.StoreSvr.Data.MoneyChg.Money );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case STORE_SVR_OP_ITEM_CHG:
		{
			int gridIdx = 0;
			if ( msgbody.StoreSvr.Data.ItemChg.StoreGrid.Grid.Type == GRID_ITEM_ITEM )
			{
				gridIdx = msgbody.StoreSvr.Data.ItemChg.StoreGrid.Grid.GridData.RoleItem.GridIdx;
			}
			else
			{
				gridIdx = msgbody.StoreSvr.Data.ItemChg.StoreGrid.Grid.GridData.RoleArm.GridIdx;
			}

			MAPITEMCHG::iterator iter = m_mapItemChg.find( gridIdx );
			if ( iter != m_mapItemChg.end() )
			{
				iter->second = msgbody.StoreSvr.Data.ItemChg;
			}
			else
			{
				m_mapItemChg[gridIdx] = msgbody.StoreSvr.Data.ItemChg;
			}

			tagUIMsgBody UI_Msg;
			memcpy( &UI_Msg.StoreData.ItemChg, &msgbody.StoreSvr.Data.ItemChg, sizeof(tagStoreItemChg_Tolua) );
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_STORE_ITEMCHG" );
		}
		break;
	case STORE_SVR_OP_CLOSE:
		{
			char buffer[256];
			sprintf( buffer, "CloseStoreResult(%d)", msgbody.StoreSvr.Data.Close.ID );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case STORE_SVR_OP_OPEN:
		{
			char buffer[256];
			sprintf( buffer, "OpenStoreResult(%d,%d,\"%s\")", msgbody.StoreSvr.Data.Open.ID, msgbody.StoreSvr.Data.Open.Succ,
						msgbody.StoreSvr.Data.Open.StoreName );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case STORE_SVR_OP_OPEN_UI:
		{
			m_mapItemChg.clear();
			m_vecStore.clear();
			m_vecStore.push_back( msgbody.StoreSvr.Data.OpenUI.Store );

			static_assert( sizeof(tagStoreOpenUI_ToLua) == sizeof(tagStoreOpenUI) );
			tagUIMsgBody UI_Msg;
			memcpy( &UI_Msg.StoreData.OpenUI, &msgbody.StoreSvr.Data.OpenUI, sizeof(tagStoreOpenUI_ToLua) );
			GameUIMsg::getSingleton().setUIMsg( UI_Msg );
			GameManager::getSingleton().SendUIEvent( "GE_STORE_OPENUI" );
		}
		break;
	case STORE_SVR_OP_POS:
		{
			char buffer[256];
			sprintf( buffer, "UpdateStorePos(%d,%d,%d)", msgbody.StoreSvr.Data.Pos.ID, msgbody.StoreSvr.Data.Pos.Pos,
							msgbody.StoreSvr.Data.Pos.Exchg );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case STORE_SVR_OP_SEARCH:
		{
			GameManager::getSingleton().getScriptVM()->callString( "ClearSearchResult()" );
			for ( int i = 0; i < msgbody.StoreSvr.Data.Search.Num; i++ )
			{
				char buffer[256];
				sprintf( buffer, "AddSearchResult(%d)", msgbody.StoreSvr.Data.Search.IDs[i] );
				GameManager::getSingleton().getScriptVM()->callString( buffer );
			}
			GameManager::getSingleton().getScriptVM()->callString( "ChangeSearchStoreResult()" );
		}
		break;
	}
	return 0;
}

const tagStore_ToLua* GameStoreManager::getStoreData()
{
	static_assert( sizeof(tagStore_ToLua) == sizeof(tagStore) );
	if ( m_vecStore.size() > 0 )
	{
		return (const tagStore_ToLua*)( &m_vecStore[0] );
	}
	return NULL;
}

const tagStoreGridDef_ToLua* GameStoreManager::getStoreGridDef( int nIdx )
{
	static_assert( sizeof(tagStoreGridDef_ToLua) == sizeof(tagStoreGridDef) );
	return (const tagStoreGridDef_ToLua*)DBDataManager::getSingleton().getStoreGridDef( nIdx );
}

int GameStoreManager::getSaveGridIdx( int nGridIdx )
{
	for ( int i = 0; i < STORE_MAX_GRID; ++i )
	{
		if ( m_vecStore[0].StoreInfo.Grid[i].Grid.GridData.RoleItem.GridIdx == nGridIdx )
		{
			return i;
		}
	}
	return 0;
}

Item* GameStoreManager::getStoreItem( int nGridIdx )
{
	assert( nGridIdx < STORE_MAX_GRID );
	if ( m_mapItemChg.size() > 0 )
	{
		MAPITEMCHG::iterator iter = m_mapItemChg.find( nGridIdx );
		if ( iter != m_mapItemChg.end() )
		{
			if ( iter->second.StoreGrid.Grid.Type == GRID_ITEM_ITEM )
			{
				if ( iter->second.StoreGrid.Grid.GridData.RoleItem.Num > 0 )
				{
					m_StoreItem.setItemDate( GRID_ITEM_ITEM, &iter->second.StoreGrid.Grid.GridData.RoleItem );
					return &m_StoreItem;
				}				
			}
			else
			{
				if ( iter->second.StoreGrid.Grid.GridData.RoleArm.Num > 0 )
				{
					m_StoreItem.setItemDate( GRID_ITEM_ARM, &iter->second.StoreGrid.Grid.GridData.RoleArm );
					return &m_StoreItem;
				}
			}
			return NULL;
		}
	}

	if ( m_vecStore.size() > 0 && nGridIdx < m_vecStore[0].StoreInfo.MaxNum  )
	{
		const int nIdx = getSaveGridIdx( nGridIdx );
		if ( m_vecStore[0].StoreInfo.Grid[nIdx].Grid.Type == GRID_ITEM_ITEM )
		{
			m_StoreItem.setItemDate( GRID_ITEM_ITEM, &m_vecStore[0].StoreInfo.Grid[nIdx].Grid.GridData.RoleItem );
		}else if ( m_vecStore[0].StoreInfo.Grid[nIdx].Grid.Type == GRID_ITEM_ARM )
		{
			m_StoreItem.setItemDate( GRID_ITEM_ARM, &m_vecStore[0].StoreInfo.Grid[nIdx].Grid.GridData.RoleItem );
		}
		return &m_StoreItem;
	}
	return NULL;
}

int GameStoreManager::getStoreItemPrice( int nGridIdx )
{
	assert( nGridIdx < STORE_MAX_GRID );
	if ( m_mapItemChg.size() > 0 )
	{
		MAPITEMCHG::iterator iter = m_mapItemChg.find( nGridIdx );
		if ( iter != m_mapItemChg.end() )
		{
			if ( iter->second.StoreGrid.Grid.Type == GRID_ITEM_ITEM )
			{
				if ( iter->second.StoreGrid.Grid.GridData.RoleItem.Num > 0 )
				{
					return iter->second.StoreGrid.Price;
				}				
			}
			else
			{
				if ( iter->second.StoreGrid.Grid.GridData.RoleArm.Num > 0 )
				{
					return iter->second.StoreGrid.Price;
				}
			}
			return 0;
		}
	}

	if ( m_vecStore.size() > 0 )
	{
		const int nIdx = getSaveGridIdx( nGridIdx );		
		return m_vecStore[0].StoreInfo.Grid[nIdx].Price;
	}
	return 0;
}

bool GameStoreManager::setItemLink( int nIdx )
{
	Item* pItem = getStoreItem( nIdx );
	if( pItem != NULL && pItem->getItemId() != 0 && ( (GetKeyState(VK_LSHIFT) & 0xff00) || (GetKeyState(VK_RSHIFT) & 0xff00) ) )
	{
		GameMsgPrint::getSingleton().setItemLink( *pItem );
		return true;
	}
	return false;
}

void GameStoreManager::requestStoreOpen( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_OPEN;
	strncpy( msgbody.StoreClt.Data.Open.StoreName, szName, strlen(szName) + 1 );
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStoreClose( int nStoreID )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_CLOSE;
	msgbody.StoreClt.Data.Close.ID = nStoreID;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStorePage( int nIndex )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_PAGE;
	msgbody.StoreClt.Data.Page.Idx = nIndex;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStoreView( int nStoreID )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_VIEW;
	msgbody.StoreClt.Data.View.ID = nStoreID;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStoreAddGrid( int nIdx )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_ADD_GRID;
	msgbody.StoreClt.Data.AddGrid.Idx = nIdx;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStoreDecGrid( int nIdx )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_DEC_GRID;
	msgbody.StoreClt.Data.DecGrid.Idx = nIdx;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStoreMoneyOpt( int nFlag, int nNum )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_MONEY_IN;
	msgbody.StoreClt.Data.MoneyIn.Flag = nFlag;
	msgbody.StoreClt.Data.MoneyIn.Money = nNum;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStoreUpItem( int nGridIdx, int nPrice, int nNum )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_UP;
	msgbody.StoreClt.Data.Up.GridIdx = nGridIdx;
	msgbody.StoreClt.Data.Up.Price = nPrice;
	msgbody.StoreClt.Data.Up.Num = nNum;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStoreDownItem( int nGridIdx, tdr_ulonglong wId )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_DOWN;
	msgbody.StoreClt.Data.Down.GridIdx = nGridIdx;
	msgbody.StoreClt.Data.Down.WID = wId;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStoreBuyItem( int nStoreID, int nGridIdx, int nMoney, int nNum, tdr_ulonglong wId )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_BUY;
	msgbody.StoreClt.Data.Buy.ID = nStoreID;
	msgbody.StoreClt.Data.Buy.GridIdx = nGridIdx;
	msgbody.StoreClt.Data.Buy.Money = nMoney;
	msgbody.StoreClt.Data.Buy.Num = nNum;
	msgbody.StoreClt.Data.Buy.WID = wId;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestStorePos( int nStoreID )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_POS;
	msgbody.StoreClt.Data.Pos.ID = nStoreID;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestVipStore()
{
	tagCSPkgBody msgbody;
	msgbody.StrongClt.Cmd = STORE_CLT_OP_OPEN_UI;
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestSearch( const char* szInfo )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_SEARCH;
	strncpy( msgbody.StoreClt.Data.Search.KeyWorlds, szInfo, strlen(szInfo) + 1 );
	SendGameServerMsg( STORE_CLT, msgbody );
}

void GameStoreManager::requestChgStoreName( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.StoreClt.Cmd = STORE_CLT_OP_CHG_NAME;
	strncpy( msgbody.StoreClt.Data.ChgName.StoreName, szName, strlen(szName) + 1 );
	SendGameServerMsg( STORE_CLT, msgbody );
}