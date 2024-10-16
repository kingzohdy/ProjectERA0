#include "GameGetAccountAllRole.h"
#include "NetMessage.h"
#include <Windows.h>


void GameGetAccountAllRole::Reset()
{
	CloseAllThread();
	m_vWorldRoleInfo.clear();
}

void GameGetAccountAllRole::CloseAllThread()
{
// 	LockFunctor functor(&m_Mutex);
	for( size_t i = 0 ; i < m_vThreadHandle.size() ; ++i )
	{
		if( m_vThreadHandle[i] != NULL )
		{
			CloseHandle( m_vThreadHandle[i] );
			m_vThreadHandle[i] = NULL;
		}
	}
	m_vThreadHandle.clear();
}

void GameGetAccountAllRole::AddWorldData( unsigned int WorldId, unsigned int RegionId, const char* szAuhIp, unsigned short AuthPort, const char* szName )
{
	for( size_t i = 0 ; i < m_vWorldRoleInfo.size() ; ++i )
	{
		if( (m_vWorldRoleInfo[i].WorldID == WorldId ) && (m_vWorldRoleInfo[i].RegionId == RegionId) )
		{
			return;
		}
	}
	WorldRoleInfo info;
	info.WorldID = WorldId;
	info.RegionId = RegionId;
	info.AuthIP = szAuhIp;
	info.AuthPort = AuthPort;
	info.szName = szName;
	m_vWorldRoleInfo.push_back( info );
}

// void GameGetAccountAllRole::clearWorldData( unsigned int WorldId, unsigned int RegionId )
// {
// 	for( size_t i = 0 ; i < m_vWorldRoleInfo.size() ; ++i )
// 	{
// 		if( (m_vWorldRoleInfo[i].WorldID == WorldId ) && (m_vWorldRoleInfo[i].RegionId == RegionId) )
// 		{
// 			memset( &m_vWorldRoleInfo[i].roleInfo, 0, sizeof(ACCGETROLESVR) );
// 			return;
// 		}
// 	}
// }

DWORD WINAPI GetRoleDataWorkFunction( void* data )
{
	WorldRoleInfo* roleInfo = (WorldRoleInfo*)data;
	INetworkManager::getSingleton().GetAccountRole( roleInfo->AuthIP.c_str(),
													roleInfo->AuthPort,
													roleInfo->szName.c_str(),
													&(roleInfo->roleInfo) );
	return 0;
}

void GameGetAccountAllRole::StartGetRoleData()
{
	static_assert( sizeof(tagAccGetRoleSvr_ToLua) == sizeof(tagAccGetRoleSvr) );
	for( size_t i = 0 ; i < m_vWorldRoleInfo.size() ; ++i )
	{
		if( m_vWorldRoleInfo[i].roleInfo.Succ != 1 )
		{
			DWORD threadid;
			HANDLE handle = CreateThread( NULL, 0, GetRoleDataWorkFunction, &m_vWorldRoleInfo[i], 0, &threadid );
			m_vThreadHandle.push_back( handle );
		}
	}
}

bool GameGetAccountAllRole::IsFinish()
{
// 	LockFunctor functor(&m_Mutex);
	for( std::vector<void*>::iterator it = m_vThreadHandle.begin(); it != m_vThreadHandle.end(); )
	{
		
		if( WaitForSingleObject( *it, 0 ) != WAIT_TIMEOUT )
		{
			CloseHandle( *it );
			it = m_vThreadHandle.erase( it );
		}else{
			++it;
		}
	}
	if( m_vThreadHandle.empty() )
	{
		return true;
	}
	return false;
}

int GameGetAccountAllRole::GetWorldRoleInfoNum()
{
	return (int)m_vWorldRoleInfo.size();
}

const WorldRoleInfo* GameGetAccountAllRole::getWorldRoleInfoByIdx( int nIdx )
{
	if( nIdx >= GetWorldRoleInfoNum() )
	{
		return NULL;
	}
	return (const WorldRoleInfo*)(&m_vWorldRoleInfo[nIdx]);
}
