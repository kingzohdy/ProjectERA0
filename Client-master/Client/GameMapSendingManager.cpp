#include "GameMapSendingManager.h"
#include "GameDBData.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "WorldCity.h"
#include "GameVipManager.h"
#include <stack>

GameMapSendingManager::GameMapSendingManager(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( TRANS_LIST, this );
	DBDataManager::getSingleton().loadMapTransDef( m_MapTransDef );
	for( int i = 0 ; i < (int)m_MapTransDef.size() ; i++ )
	{
		m_nearMap[m_MapTransDef[i]->FromMapID].push_back( m_MapTransDef[i]->ToMapID );
	}
}

GameMapSendingManager::~GameMapSendingManager(void)
{
}
int GameMapSendingManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody )
{
	if ( msgid == TRANS_LIST )
	{
		m_nFromMapID	=	msgbody.TransList.FromMapID;
		m_nNpcID		=	msgbody.TransList.NpcID;
		m_useItem.SrcGridIdx = 0;
		m_useItem.SrcListType = 0;
		for( int i=0; i<msgbody.TransList.Count; i++ )
		{
			static_assert( sizeof(tagMapTransInfo_tolua) == sizeof(tagMapTransInfo) );
		}
		setShortestPathesData();
		GameManager::getSingleton().getScriptVM()->callString( "MapSendUpdate()" );
	}
	return 0;
}
void GameMapSendingManager::mapSendingSelect( int npcID )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof( tagCSPkgBody ) );
	msgbody.TransSelect.NpcID = npcID;
	for( size_t i = 0 ; i < m_vecPath.size() ; i ++ )
	{
		msgbody.TransSelect.TransID[msgbody.TransSelect.TransIDNum++] = m_vecPath[i];
		if( msgbody.TransSelect.TransIDNum >= MAX_TRANS )
			break;
	}
	msgbody.TransSelect.UseItem.SrcGridIdx = m_useItem.SrcGridIdx;
	msgbody.TransSelect.UseItem.SrcListType = m_useItem.SrcListType;
	SendGameServerMsg( TRANS_SELECT,  msgbody );
}

int GameMapSendingManager::getFormMapID()
{
	return m_nFromMapID;
}

int GameMapSendingManager::getNpcID()
{
	return m_nNpcID;
}

int GameMapSendingManager::getMapTransDefNum()
{
	return (int)m_MapTransDef.size();
}

tagMapTransDef_ToLua* GameMapSendingManager::getMapTransDef( int nIdx )
{
	static_assert( sizeof(tagMapTransDef_ToLua) == sizeof(tagMapTransDef) );
	if( nIdx >= (int)m_MapTransDef.size() )
	{
		return NULL;
	}
	return (tagMapTransDef_ToLua*) m_MapTransDef[nIdx];
}

tagMapTransDef_ToLua* GameMapSendingManager::getMapTransDefByID( int nMapID )
{
	static_assert( sizeof(tagMapTransDef_ToLua) == sizeof(tagMapTransDef) );
	for( size_t i = 0 ; i < m_MapTransDef.size() ; ++i )
	{
		if ( m_MapTransDef[i]->FromMapID == nMapID )
		{
			return (tagMapTransDef_ToLua*) m_MapTransDef[i];
		}
	}
	return NULL;
}

int GameMapSendingManager::getMapTransItemList()
{
	return m_useItem.SrcListType;
}

int GameMapSendingManager::getMapTransItemGrid()
{
	return m_useItem.SrcGridIdx;
}

int	GameMapSendingManager::getNextMapNum( int nSrcMapID )
{
	std::map<int, std::vector<int>>::iterator i = m_nearMap.find(nSrcMapID);
	if (i != m_nearMap.end())
	{
		return i->second.size();
	}
	return 0;
}

int	GameMapSendingManager::getNextMapInfo(int nSrcMapID, int nIndex)
{
	return	m_nearMap[nSrcMapID][nIndex];
}

bool GameMapSendingManager::succesConstructPath( PATH& path, MAP_POINT& nCurPathEndInfo, 
										   std::stack<MAP_POINT>& pathBuffer, 
										   std::stack<MAP_POINT>& branchBuffer )
{
	bool bCanExtendPath = false;	// ��־·���Ƿ񱻱��ӳ�
	int	 nBranchCount	= 0;

	for ( int i = 0, size = getNextMapNum( nCurPathEndInfo.second ); i < size; ++i )
	{
		int	nNextMapId = getNextMapInfo( nCurPathEndInfo.second, i );
		// ��������·���е��򲻻��棬Ҳ��ֹͣ����·������Ϊ����·�����Թ��쾡���ܳ���·��Ϊ��׼
		if ( isInPath( nNextMapId, path ) )
		{
			continue;
		}

		pathBuffer.push( make_pair( i, nNextMapId ) );
		branchBuffer.push( make_pair( i, nCurPathEndInfo.second ) );
		++nBranchCount;
		bCanExtendPath = true;
	}

	if ( nBranchCount > 0 )
	{
		path.back()		= branchBuffer.top();
		nCurPathEndInfo = branchBuffer.top();
		branchBuffer.pop();
	}
	else
	{
		( path.back() ).first	= 0;
		nCurPathEndInfo.first	= 0;
	}

	return !bCanExtendPath;
}

bool GameMapSendingManager::isSubPath( const PATH& subPath, const PATH& parentPath )
{
	return	std::search( parentPath.begin(), parentPath.end(), subPath.begin(), subPath.end() ) != parentPath.end();
}

bool GameMapSendingManager::isInPath( int nMapID, const PATH& path )
{
	for ( CONST_PATH_ITER iter = path.begin(), end = path.end(); iter != end; ++iter )
	{
		if ( nMapID == iter->second )
		{
			return true;
		}
	}

	return false;
}

PATH_ITER GameMapSendingManager::findMap( int nMapID, PATH& path )
{
	for ( PATH_ITER iter = path.begin(), end = path.end(); iter != end; ++iter )
	{
		if ( iter->second == nMapID )
		{
			return	iter;
		}
	}

	return path.end();
}

tagMapTransDef* GameMapSendingManager::getMapInfo( int nFromMapID, int nIndex )
{
	int	nCount = 0;
	for ( vector<tagMapTransDef*>::iterator iter = m_MapTransDef.begin(), end = m_MapTransDef.end();
		iter != end; ++iter )
	{
		if ( (*iter)->FromMapID == nFromMapID )
		{
			if ( nCount == nIndex )
			{
				return	( *iter );
			}
			++nCount;
		}
	}
	return NULL;
}

tagMapTransDef* GameMapSendingManager::getMapInfo( int nListIdx )
{
	for( int i = 0 ; i < (int)m_MapTransDef.size() ; i++ )
	{
		if( m_MapTransDef[i]->ListIndex == nListIdx )
		{
			return m_MapTransDef[i];
		}
	}
	return NULL;
}

void GameMapSendingManager::setShortestPathesData()
{
	m_shortestPathes.clear();

	int		nCurMapId = m_nFromMapID;
	PATH	path;
	path.push_back( make_pair( getNextMapNum( nCurMapId ) - 1, nCurMapId ) );

	std::stack<MAP_POINT>	pathPoints;
	std::stack<MAP_POINT>	branchPoints;

	for ( int i = 0, size = getNextMapNum( nCurMapId ); i < size; ++i )
	{
		pathPoints.push( make_pair( i, getNextMapInfo( nCurMapId, i ) ) );
	}

	for ( int i = 0, size = getNextMapNum( nCurMapId ) - 1; i < size; ++i )
	{
		branchPoints.push( make_pair( i, nCurMapId ) );
	}

	MAP_POINT	newMapInfo;
	while ( !pathPoints.empty() )
	{
		newMapInfo = pathPoints.top();
		path.push_back( newMapInfo );
		pathPoints.pop();

		// 1.����ǰ�ڵ㣨����ջ�������Ľڵ㣩û�к�̽ڵ���������쵱ǰ·���������һ��·��
		if ( !succesConstructPath( path, newMapInfo, pathPoints, branchPoints ) )
		{
			continue;	
		}else{		
			for ( vector<PATH>::iterator iter = m_shortestPathes.begin(), end = m_shortestPathes.end();
				iter != end; ++iter )
			{
				// ����֤�¼����·���Ƿ����Ѵ���·�����Ӽ�
				if ( isSubPath( path, *iter ) )
				{
					break;
				}
				// ����֤�¼����·���Ƿ����Ѵ��ڵ�·���ĸ�����
				if ( isSubPath( *iter, path ) )
				{
					m_shortestPathes.erase( iter );
					break;
				}
			}
			// ���¼����·�������Ѵ��ڵ�·�����ϲ������κθ��ӹ�ϵ
			m_shortestPathes.push_back( path );

			// ��ջ����û�нڵ㣬��û����Ҫ�����������һ��·��
			if ( pathPoints.empty() )
			{
				break;
			}
		}
		// ������һ��·����Ĺ�������ջ��Ϊ�գ������ڵ�ǰ·���в���ջ��Ԫ�أ�����·��ɾ����ջ��Ԫ��Ϊ����·������
		for ( PATH_ITER iter = path.begin(), end = path.end(); iter != end; ++iter )
		{
			if ( iter->second == ( branchPoints.top() ).second )
			{
				path.erase( iter, path.end() );
				path.push_back( branchPoints.top() );
				branchPoints.pop();
				break;
			}
		}	
	}	
}

bool GameMapSendingManager::doCalcTranList( std::vector<int>& result, int toMapId, bool bConditionCheck )
{
	result.clear();
	// �ҳ����·����Ӧ�ı�ʶ
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	vector<size_t>	validPathKeyes;
	for ( size_t i = 0, size = m_shortestPathes.size(); i < size; ++i )
	{
		// ����ǰ·�����ܵ���Ŀ���ͼ��������
		if ( isInPath( toMapId, m_shortestPathes[i] ) )
		{
			// �������пɴ��ͼnDestMapID��·������Ϣ
			validPathKeyes.push_back( i );
		}
	}

	MAP_POINT	shortestPathInfo( INT_MAX, INT_MAX );
	for ( vector<size_t>::iterator iter = validPathKeyes.begin(), end = validPathKeyes.end(); 
		iter != end; ++iter )
	{
		PATH_ITER destMapPos = findMap( toMapId, m_shortestPathes[*iter] );

		int	nCurPathLength = (int)(destMapPos - m_shortestPathes[*iter].begin());
		if ( ( shortestPathInfo.second ) > (int)nCurPathLength )
		{
			shortestPathInfo = make_pair( (int)*iter, (int)nCurPathLength );
		}
	}

	// ��2�ŵ�ͼ֮��û��ͨ·�������κβ���
	if ( shortestPathInfo.first == INT_MAX )
	{
		return false;
	}

	PATH&		shortestPath	= m_shortestPathes[(shortestPathInfo.first)];
	PATH_ITER	destMapPos		= findMap( toMapId, shortestPath );
	assert( destMapPos != shortestPath.end() );
	if ( destMapPos == shortestPath.end() )
	{
		return false;
	}

	for ( PATH_ITER iter = shortestPath.begin(); iter != destMapPos; ++iter )
	{
		tagMapTransDef* pMapTransDef = getMapInfo( iter->second, iter->first );
		if( pMapTransDef )
		{
			if( bConditionCheck )
			{
				if( pMain && pMain->getLv() < pMapTransDef->NeedLevel )
				{
					char buf[256];
					sprintf( buf,"MapSenderLvLow(%d)",pMapTransDef->NeedLevel );
					GameManager::getSingleton().getScriptVM()->callString( buf );
					return false;
				}
				if( pMapTransDef->TransCondition )
				{
					if ( !GameVipManager::getSingleton().isEnableVip( STAR_STONE_SILVER ) 
						&& !WorldCityManager::getSingleton().IsWorldCityKeeper(pMain) )
					{
						return false;
					}
				}
			}			
			result.push_back( pMapTransDef->ListIndex );
		}
	}
	if( result.size() > 0 )
	{
		return true;
	}else{
		return false;
	}
}
bool GameMapSendingManager::beginCalcTranList( int toMapId, bool bConditionCheck )
{
	return doCalcTranList( m_vecPath, toMapId, bConditionCheck );
}

int GameMapSendingManager::getCostMoney( int toMapId )
{
	std::vector<int> result;
	int nCost = 0;
	if( doCalcTranList( result, toMapId, false ) )
	{
		for( int i = 0 ; i < (int)result.size(); i++ )
		{
			tagMapTransDef* pMapTransDef = getMapInfo( result[i] );
			nCost += pMapTransDef->Money;
		}
	}
	return nCost;
}

void GameMapSendingManager::openTranFrameByItem( int nGridIdx, int nListType )
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if ( pMain != NULL )
	{
		m_nNpcID = 0;
		m_nFromMapID = pMain->getMapId();
		m_useItem.SrcGridIdx = nGridIdx;
		m_useItem.SrcListType = nListType;
		setShortestPathesData();
		GameManager::getSingleton().getScriptVM()->callString( "MapSendUpdate()" );
	}
}