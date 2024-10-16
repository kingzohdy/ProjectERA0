#include "GameCBT.h"
#include <assert.h>
#include <memory.h>
#include <algorithm>
#include "GameEngineTypes.h"
#include "proto_cs.h"
#include "NetMessage.h"
#include "GameDBData.h"
#include "GameMsgPrint.h"

const tagCangBaoTuDef_ToLua* getCangBaoTuDefDef( int nMapID, int nLevel, int nItemID )
{
	static_assert( sizeof( tagCangBaoTuDef_ToLua ) == sizeof( tagCangBaoTuDef ) );
	const tagCangBaoTuDef* pCangBaoTuDef = DBDataManager::getSingleton().getCangBaoTuDefDef( nMapID, nLevel, nItemID );
	return ( const tagCangBaoTuDef_ToLua* )pCangBaoTuDef;
}

GameCBT::GameCBT(void)
{
	DBDataManager::getSingleton().loadCangBaoTuDef( m_cangBaoTuDef ); 
	m_bRequestReset = false;
}

GameCBT::~GameCBT(void)
{
}

void GameCBT::setCangBaoTuInfo( const tagCangBaoTuInfo& cangBaoTuList )
{
	static_assert( sizeof( tagCangBaoTuInfo ) == sizeof( tagCangBaoTuInfo_ToLua ) );
	if ( cangBaoTuList.DayTimes < m_cangBaoTuList.DayTimes )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_SUCCESS_GET_WA_JUE" );
	}
	memcpy( &m_cangBaoTuList, &cangBaoTuList, sizeof( m_cangBaoTuList ) );
	m_bRequestReset = false;
}

bool GameCBT::isCangBaoTuItem( int nItemID )
{
	for ( std::vector<tagCangBaoTuDef*>::iterator iter = m_cangBaoTuDef.begin(), end = m_cangBaoTuDef.end();
			iter != end; ++iter )
	{
		if ( (*iter)->ItemID == nItemID )
		{
			return true;
		}
	}
	return false;
}

int GameCBT::getMaxCangBaoTuMap()
{
	return CANGBAOTU_NUM;
}

const tagCangBaoTuMapInfo_ToLua& GameCBT::getCangBaoTuMapInfo( int nIndex ) const
{
	assert( nIndex < CANGBAOTU_NUM );
	return m_cangBaoTuList.CangBaoTuMapInfo[nIndex];
}

const tagCangBaoTuMapInfo_ToLua& GameCBT::getCangBaoTuMapInfo( int nIndex )
{
	assert( nIndex < CANGBAOTU_NUM );
	return m_cangBaoTuList.CangBaoTuMapInfo[nIndex]; 
}

const tagCangBaoTuMapInfo_ToLua* GameCBT::getCangBaoTuMapInfoByItemID( int nItemID )
{
	for ( int i = 0; i < CANGBAOTU_NUM; ++i )
	{
		const tagCangBaoTuMapInfo_ToLua& cangBaoTuInfo = m_cangBaoTuList.CangBaoTuMapInfo[i];
		if ( cangBaoTuInfo.ItemID == nItemID )
		{
			return &cangBaoTuInfo;
		}
	}

	return NULL;
}

bool GameCBT::isArriveMaxCangBaoTimesOneDay()
{
	return getCanUseTimes() == 0;
}

int GameCBT::getAlreadyUseTimes()
{
	return MAX_CANG_BAO_TIMES_ONE_DAY - m_cangBaoTuList.DayTimes;
}

int GameCBT::getCanUseTimes()
{
	return m_cangBaoTuList.DayTimes;
}

// ÇëÇóÖØÖÃ²Ø±¦Í¼
void GameCBT::send_reset( int nItemId )
{
	tagCSPkgBody msgbody;
	msgbody.PackageReq.Type	= PACKAGE_REQ_CANGBAOTU_RESET;
	CSPACKAGECANGBAOTURESET& resetCangBaoTuInfo = msgbody.PackageReq.PackageReqData.PackageCangBaoTuReset;
	resetCangBaoTuInfo.ItemID = nItemId;
	SendGameServerMsg( PACKAGE_REQ, msgbody );
	m_bRequestReset = true;
}

bool GameCBT::isRequestReset()
{
	return m_bRequestReset;
}

void GameCBT::setRequestReset( bool bRequestReset )
{
	m_bRequestReset = bRequestReset;
}