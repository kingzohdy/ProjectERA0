#include "GameDiathesis.h"
#include "NetMessage.h"
#include "GameDBData.h"
#include "GameMsgPrint.h"
#include <algorithm>
#include "OgreXMLData.h"
#include "GameSkill_Mgr.h"
using namespace Ogre;

const tagDiathesisDef_ToLua* getDiathesisDef( int id, int lv )
{
	static_assert( sizeof( tagDiathesisDef_ToLua ) == sizeof( tagDiathesisDef ) );

	const tagDiathesisDef* pDiathesisDef  = &DBDataManager::getSingleton().getDiathesisDef( id, lv );
	if( pDiathesisDef == NULL )
	{
		return NULL;
	}

	return (const tagDiathesisDef_ToLua*)pDiathesisDef;
}

DiathesisManager::DiathesisManager()
{
	INetworkManager::getSingleton().RegisterMsgHandler( DIATHESIS_INFO,  this );
	INetworkManager::getSingleton().RegisterMsgHandler( DIATHESIS_INFO_CHG,  this );
	DBDataManager::getSingleton().loadDiathesisDef( m_DiathesisDefTable );
	// TODO: 这边要做个操作，防止reload出现错误
	mergeDiathesisTable();
}

DiathesisManager::~DiathesisManager()
{
	INetworkManager::getSingleton().unRegisterMsgHander( DIATHESIS_INFO );
	INetworkManager::getSingleton().unRegisterMsgHander( DIATHESIS_INFO_CHG );
}

const tagDiathesisInfo_toLua& DiathesisManager::getDiathesisInfo() const
{
	static_assert( sizeof( DIATHESISINFO ) == sizeof( tagDiathesisInfo_toLua ));
	return m_DiathesisInfo;
}

// 是否已经学习过该天赋
bool DiathesisManager::isLearnedGenius( int nId, int nLv )
{
	for ( size_t i = 0; i < m_DiathesisInfo.DiathesisNum; ++i )
	{
		const tagDiathesisData_toLua& diathesData = m_DiathesisInfo.DiathesisList[i];
		if ( diathesData.GeniusFlag != 1 )
		{
			continue;
		}

		if ( diathesData.DiathesisID == nId && diathesData.DiathesisLevel == nLv )
		{
			return true;
		}
	}

	return false;
}

int DiathesisManager::getLearnedGeniusLevel( int nId )
{
	for ( size_t i = 0; i < m_DiathesisInfo.DiathesisNum; ++i )
	{
		const tagDiathesisData_toLua& diathesData = m_DiathesisInfo.DiathesisList[i];
		if ( diathesData.GeniusFlag != 1 )
		{
			continue;
		}

		if ( diathesData.DiathesisID == nId /*&& diathesData.DiathesisLevel == nLv*/ )
		{
			return diathesData.DiathesisLevel;
		}
	}

	return 0;
}

const tagDiathesisDef_ToLua& DiathesisManager::getDiathesis( int nIndex )
{
	assert( nIndex < (int)m_AllGeniusDef.size() );
	return m_AllGeniusDef[nIndex];
}

int DiathesisManager::getAllGeniusNum()
{
	return (int)m_AllGeniusDef.size();
}

int DiathesisManager::getLearnedDiathesisNum()
{
	return m_DiathesisInfo.DiathesisNum;
}

const tagDiathesisData_toLua& DiathesisManager::getDiathesisData( int nIndex )
{
	assert( nIndex < (int)m_DiathesisInfo.DiathesisNum );
	return m_DiathesisInfo.DiathesisList[nIndex];
}

int	DiathesisManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if ( msgid != DIATHESIS_INFO )
	{
		return 0;
	}

	if ( msgbody.DiathesisInfo.Type == DIATHESIS_INFO_CHG )
	{
		bool bCircleToEnd = true;
		const DIATHESISINFO& diathesisChgData = msgbody.DiathesisInfo.DiathesisInfoData.DiathesisChg;
		assert( diathesisChgData.DiathesisNum < MAX_DIATHESIS );
		for ( size_t i = 0; i < diathesisChgData.DiathesisNum; ++i )
		{
			bCircleToEnd = true;
			const DIATHESISDATA& newDiathesisData = diathesisChgData.DiathesisList[i];
			for ( size_t j = 0; j < m_DiathesisInfo.DiathesisNum; ++j )
			{
				tagDiathesisData_toLua& diathesisCache = m_DiathesisInfo.DiathesisList[j];
				if ( newDiathesisData.DiathesisID == diathesisCache.DiathesisID )
				{
					diathesisCache.DiathesisLevel = newDiathesisData.DiathesisLevel;
					bCircleToEnd = false;
					break;
				}
			}

			if ( bCircleToEnd )
			{
				if ( m_DiathesisInfo.DiathesisNum < MAX_DIATHESIS )
				{
					addDiathesisInfo( newDiathesisData );
				}
			}				
		}

		GameMsgPrint::getSingleton().SendUIEvent( "GE_UPDATE_DIATHESIS" );
	}
	else if ( msgbody.DiathesisInfo.Type == GENIUS_INFO_CHG )
	{
		const CSGENIUSINFO& geniusData = msgbody.DiathesisInfo.DiathesisInfoData.GeniusInfo;
		if ( m_DiathesisInfo.LearnGeniusPoint > geniusData.LearnGeniusPoint )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_DEC_GENIUS_POINT" );
		}
		m_DiathesisInfo.LearnGeniusPoint	= geniusData.LearnGeniusPoint;
		m_DiathesisInfo.UsedGeniusPoint		= geniusData.UsedGeniusPoint;
		//GameMsgPrint::getSingleton().SendUIEvent( "GE_GENIUS_CHG" );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_UPDATE_DIATHESIS" );
		if ( m_DiathesisInfo.LearnGeniusPoint > 0 )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_GET_GENIUS_POINT" );
		}
	}
	else if ( msgbody.DiathesisInfo.Type == DIATHESIS_ALL_DATA )
	{
		const DIATHESISINFO& diathesisAllData = msgbody.DiathesisInfo.DiathesisInfoData.DiathesisInfo;
		memcpy( &m_DiathesisInfo, &diathesisAllData, sizeof( DIATHESISINFO ) );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_UPDATE_DIATHESIS" );
	}
	else if (msgbody.DiathesisInfo.Type == SKILL_REDUCE_CD_TIME)
	{
		const CSSKILLREDUCECDTIMEARR &data = msgbody.DiathesisInfo.DiathesisInfoData.SkillReduceCdTime;
		for (int i = 0; i < data.ChgNum; ++i)
		{
			const CSSKILLREDUCECDTIME& skillReduceCdTimeData = data.CSSkillReduceCdTime[i];
			SkillManager::getSingleton().reduceCD( skillReduceCdTimeData.SkillID, 
																	skillReduceCdTimeData.ReduceCDTime);
		}
	}
	
	return 0;
}

void DiathesisManager::setDiathesisInfo( const DIATHESISINFO& diathesInfo )
{
	assert( sizeof( DIATHESISINFO ) == sizeof( tagDiathesisInfo_toLua ));
	memcpy( &m_DiathesisInfo, &diathesInfo, sizeof( DIATHESISINFO ) );
	GameMsgPrint::getSingleton().SendUIEvent( "GE_UPDATE_DIATHESIS" );
}

void DiathesisManager::addDiathesisInfo( const DIATHESISDATA& newDiathesInfo )
{
	tagDiathesisData_toLua& diathesisData = m_DiathesisInfo.DiathesisList[m_DiathesisInfo.DiathesisNum];
	
	diathesisData.DiathesisID		= newDiathesInfo.DiathesisID;
	diathesisData.DiathesisLevel	= newDiathesInfo.DiathesisLevel;
	diathesisData.GeniusFlag		= newDiathesInfo.GeniusFlag;
	++m_DiathesisInfo.DiathesisNum;
}

void DiathesisManager::ResetDiathesisInfo()
{
	memset( &m_DiathesisInfo, 0, sizeof( m_DiathesisInfo ) );
}

void DiathesisManager::mergeDiathesisTable()
{
	for ( size_t i = 0, size = m_DiathesisDefTable.size(); i < size; ++i )
	{
		const tagDiathesisDef& diathesisDef = m_DiathesisDefTable[i];
		if ( isGenius( diathesisDef ) )
		{
			m_AllGeniusDef.push_back( (const tagDiathesisDef_ToLua&)diathesisDef );
		}
		else
		{
			m_AllDiathesisDef.push_back( (const tagDiathesisDef_ToLua&)diathesisDef );
		}
	}
}

bool DiathesisManager::isGenius( const tagDiathesisDef& diathesis )
{
	if ( diathesis.PreUsedGeniusPoint != 0 || diathesis.UseGeniusPoint != 0 || diathesis.GeniusType > 1 )
	{
		return true;
	}

	return false;
}