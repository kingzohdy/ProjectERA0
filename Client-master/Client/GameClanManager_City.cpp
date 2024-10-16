#include <stdio.h>
#include <string.h>
#include "GameClanManager_City.h"
#include "NetMessage.h"
#include "GameDBData.h"

const tagClanCityLevelDef_ToLua*	getClanCityLevelDef( int nLevel )
{
	static_assert( sizeof(tagClanCityLevelDef_ToLua) == sizeof(tagClanCityLevelDef) );
	return (tagClanCityLevelDef_ToLua*)DBDataManager::getSingleton().getClanCityLevelDef( nLevel );
}

const tagClanCityBuildingLevelDef_ToLua*	getClanCityBuildingLevelDef( int nType, int nLevel )
{
	const tagClanCityBuildingLevelDef* ptr = DBDataManager::getSingleton().getClanCityBuildingLevelDef( nType, nLevel );
	static_assert( sizeof(tagClanCityBuildingLevelDef_ToLua) == sizeof(tagClanCityBuildingLevelDef) );
	return (tagClanCityBuildingLevelDef_ToLua*)ptr;
}

const tagClanBuildingDisplayInfo_ToLua*		getBuildingDisplayInfo( int nIndex )
{
	const tagClanBuildingDisplayInfo* pInfo = DBDataManager::getSingleton().getBuildingDisplayInfo( nIndex );
	static_assert( sizeof(tagClanBuildingDisplayInfo_ToLua) == sizeof(tagClanBuildingDisplayInfo) );
	return (tagClanBuildingDisplayInfo_ToLua*)pInfo;
}

void GameClanCityInfo::upgradeClanBuilding( int nBuildingID )
{
	assert( nBuildingID >= 0 && "不合法的ID数据" );
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation						= CLAN_OP_UPGRADE_CITY_BUILDING;
	msgbody.ClanClt.ClanCltData.ClanCityBuildingID	= nBuildingID;
	SendGameServerMsg( CLAN_CLT, msgbody );	
}

void GameClanCityInfo::enableClanCity( const char* pszCityName )
{
	assert( pszCityName != NULL );
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation	= CLAN_OP_ENABLE_CITY;
	strncpy( msgbody.ClanClt.ClanCltData.ClanCityName, pszCityName, strlen( pszCityName ) + 1 );
	SendGameServerMsg( CLAN_CLT, msgbody );	
}

void GameClanCityInfo::upgradeClanCity( const char* pszCityName )
{
	assert( pszCityName != NULL );
	tagCSPkgBody msgbody;
	msgbody.ClanClt.Operation	= CLAN_OP_UPGRADE_CITY;
	strncpy( msgbody.ClanClt.ClanCltData.ClanCityName, pszCityName, strlen( pszCityName ) + 1 );
	SendGameServerMsg( CLAN_CLT, msgbody );	
}

void GameClanCityInfo::setSelectBuilding( int nType )
{
	m_curSelectBuilding.m_nType		= nType;
	int nLevel = m_clanCityInfo.m_buildings[nType].Level;
	if( m_clanCityInfo.m_buildings[nType].Pending )
	{
		nLevel --;
	}
	m_curSelectBuilding.m_nCurLevel	= nLevel;
}

void GameClanCityInfo::setCityInfo( const tagClanCity& cityInfo )
{
	static_assert( sizeof( tagClanCity_ToLua) == sizeof( tagClanCity ) );
	memcpy( &m_clanCityInfo, &cityInfo, sizeof( tagClanCity ) );
}
