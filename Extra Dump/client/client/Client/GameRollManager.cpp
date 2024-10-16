#include "GameRollManager.h"
#include "NetMessage.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"


GameRollManager::GameRollManager()
{
	static_assert( sizeof(tagRoleItem_ToLua) == sizeof(tagRoleItem) );
	static_assert( sizeof(tagCSRollUIInfo_ToLua) == sizeof(tagCSRollUIInfo) );
	INetworkManager::getSingleton().RegisterMsgHandler( ROLL_SVR,this );
}
GameRollManager::~GameRollManager()
{
}

int GameRollManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == ROLL_SVR )
	{
		if( msgbody.RollSvr.Type == ROLL_SVR_UI )
		{
			m_RollUIInfo.push_back( msgbody.RollSvr.Data.RollUIInfo );
			char szBuf[256];
			sprintf(szBuf,"ShowRollItem(%d)",msgbody.RollSvr.Data.RollUIInfo.BootyID );
			GameManager::getSingleton().getScriptVM()->callString( szBuf );
		}
	}
	return 0;
}

tagCSRollUIInfo_ToLua* GameRollManager::getRollUIInfo( int nBootId )
{
	for( size_t i = 0 ; i < m_RollUIInfo.size(); i++ )
	{
		if( m_RollUIInfo[i].BootyID == nBootId )
		{
			return (tagCSRollUIInfo_ToLua*)( &m_RollUIInfo[i] );
		}
	}
	return NULL;
}

void GameRollManager::sendRollMessage( int nFlag,int bootID )
{
	tagCSPkgBody msgbody;
	msgbody.RollClt.BootyID = bootID ;
	msgbody.RollClt.Roll = nFlag;
	SendGameServerMsg( ROLL_CLT,msgbody );
	eraseRollUIInfo( bootID );
}

void GameRollManager::eraseRollUIInfo( int nBootId )
{
	VERROLLUIINFO::iterator it = m_RollUIInfo.begin();
	for( ;it!=m_RollUIInfo.end();it++ )
	{
		if( it->BootyID == nBootId )
		{
			m_RollUIInfo.erase( it );
			char szBuf[256];
			sprintf( szBuf,"RollItemFrameRemoveOne(%d)",nBootId );
			GameManager::getSingleton().getScriptVM()->callString( szBuf );
			return;
		}
	}
}

Item* GameRollManager::getRollItem( int nBootId )
{
	tagCSRollUIInfo_ToLua* pRollInfo = getRollUIInfo( nBootId );
	if( pRollInfo == NULL )
	{
		return NULL;
	}
	if( pRollInfo->BootyInfo.Type == GRID_ITEM_ITEM )
	{
		m_RollItem.setItemDate( GRID_ITEM_ITEM,&pRollInfo->BootyInfo.GridData.RoleItem );
	}else if( pRollInfo->BootyInfo.Type == GRID_ITEM_ARM )
	{
		m_RollItem.setItemDate( GRID_ITEM_ARM,&pRollInfo->BootyInfo.GridData.RoleArm );
	}
	m_RollItem.InitItemName();
	return &m_RollItem;
}