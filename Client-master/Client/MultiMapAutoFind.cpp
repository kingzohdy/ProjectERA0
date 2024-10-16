#include <algorithm>
#include <utility>
#include <limits>
#include <limits.h>
#include <stack>
#include <Windows.h>
#include "GameDBData.h"
#include "GameMaxMap.h"
#include "GameNpcDialog.h"
#include "GameMainPlayer.h"
#include "GameManager.h"
#include "MultiMapAutoFind.h"
#include "GameActorManager.h"
#include "GameEngineTypes.h"
#include "GameMap.h"
#include "OgreScriptLuaVM.h"
#include "GameSkill_Mgr.h"


using namespace std;

typedef std::vector<tagWorldMapSearchDef>			MAPSERCHVEC;
typedef std::vector<tagWorldMapSearchDef>::iterator	MAPSERCHVEC_ITER;

tagWorldMapSearchDef_ToLua*	getWorldMapSearchDef( int nFromMapID )
{
	static_assert( sizeof( tagWorldMapSearchDef ) == sizeof( tagWorldMapSearchDef_ToLua ) );
	const tagWorldMapSearchDef* pWorldMapSearchDef = DBDataManager::getSingleton().getWorldMapSearchDef( nFromMapID );
	return ( tagWorldMapSearchDef_ToLua* )pWorldMapSearchDef;
}

const unsigned int MIN_DELTA_TIME = 300;
MultiMapAutoFind::MultiMapAutoFind()
{
	DBDataManager::getSingleton().loadWorldMapSearchDef( m_mapSearchVec );
	mergeNearMap();
	m_uLastCallTime		= 0;
	m_AutoMoveMapId		= -1;
}

void MultiMapAutoFind::reset()
{
	m_uLastCallTime = 0;
	m_multiMapPath.clear();
	m_curFindTarget.m_nMapId = 0;
}

int MultiMapAutoFind::beginAutoFind( int nSrcMapID, int nDestMapID, int nDestRealX, int nDestRealY, int nDist )
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	//pMainplayer = NULL;
	if ( pMainplayer->isTimeShort() )
	{
		return MOVE_TIME_INTERVAL_SMALL;
	}

	if ( pMainplayer == NULL )
	{
		return MOVE_STATUS_FORBID;
	}

	GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
	if (pSpell != NULL)
	{
		if (pSpell->getState() == GameSpell::STATE_COLLECT)
		{
			return MOVE_STATUS_FORBID;
		}
		tagSkillAttrib *attrib = (tagSkillAttrib *)pSpell->getAttrib();
		if (attrib && attrib->m_isNoBreak)
		{
			return MOVE_STATUS_FORBID;
		}
	}

	if (SkillManager::getSingleton().getSpellControler()->getCurCloSpell() != NULL)
	{
		return MOVE_STATUS_FORBID;
	}

	if (!pMainplayer->getBufStatus()->canMove())
	{
		return MOVE_STATUS_FORBID;
	}

// 	// ����������ֹͣ�ƶ�
// 	if( pMainplayer->getActState().GetState() == GSACT_MOVE )
// 	{
// 		pMainplayer->CmdMoveStop();
// 	}
	m_multiMapPath.clear();
	m_curFindTarget.m_nMapId = 0;
	if ( nSrcMapID != nDestMapID )
	{
		generateShortestPath( nSrcMapID, nDestMapID );

		if ( m_multiMapPath.empty() )
		{
			//GameMsgPrint::getSingleton().showMidTip( MID_MULTIMAP_AUTOFIND_FAIL );
			char buffer[40];
			sprintf( buffer, "ShowMoveFailReson( %d )", MID_MULTIMAP_AUTOFIND_FAIL );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
			return MOVE_POINT_CANNOT_WALK;
		}
	}
	
	tagPassMapPath	mapPath;
	mapPath.m_nMapId		= nDestMapID;
	mapPath.m_bNpcTran		= false;
	mapPath.m_bInCopy		= false;
	mapPath.m_Dest.m_Pos.X	= nDestRealX;
	mapPath.m_Dest.m_Pos.Y	= nDestRealY;
	m_multiMapPath.push_back( mapPath );
	
	int nMoveResult = MOVE_SUCCESS;
	pMainplayer->setAutoFind( true );
	if ( m_multiMapPath[0].m_bNpcTran )
	{
		nMoveResult = GameNpcDialog::getSingleton().talkByResId( m_multiMapPath[0].m_Dest.m_nNpcResId );
	}
	else
	{
		nMoveResult = pMainplayer->CmdMoveTo( m_multiMapPath[0].m_Dest.m_Pos.X, m_multiMapPath[0].m_Dest.m_Pos.Y, nDist );
	}

	if ( nMoveResult != MOVE_SUCCESS)
	{
		if (nMoveResult != MOVE_ALREADY_IN_DEST)
		{
			char buffer[40];
			sprintf( buffer, "ShowMoveFailReson( %d )", nMoveResult );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
		}
		pMainplayer->setAutoFind( false );
	} else {
		GameManager::getSingleton().getScriptVM()->callString( "autoRideHorse()");
	}

	m_curFindTarget = m_multiMapPath.front();

	m_multiMapPath.pop_front();
	return nMoveResult;
}

int MultiMapAutoFind::beginAutoFind( int nSrcMapID, int nDestMapID, int nNpcResId )
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	//pMainplayer = NULL;
	if ( pMainplayer->isTimeShort() )
	{
		return MOVE_TIME_INTERVAL_SMALL;
	}

	if ( pMainplayer == NULL )
	{
		return MOVE_STATUS_FORBID;
	}
	
	if ( isSameDestination( nNpcResId ) )
	{
		return MOVE_SUCCESS;
	}

	if (!pMainplayer->getBufStatus()->canMove())
	{
		return MOVE_STATUS_FORBID;
	}

// 	// ����������ֹͣ�ƶ�
// 	if( pMainplayer->getActState().GetState() == GSACT_MOVE )
// 	{
// 		pMainplayer->CmdMoveStop();
// 	}
	
	m_multiMapPath.clear();
	m_curFindTarget.m_nMapId = 0;
	
	if ( nSrcMapID != nDestMapID )
	{
		generateShortestPath( nSrcMapID, nDestMapID );

		if ( m_multiMapPath.empty() )
		{
			char buffer[40];
			sprintf( buffer, "ShowMoveFailReson( %d )", MID_MULTIMAP_AUTOFIND_FAIL );
			GameManager::getSingleton().getScriptVM()->callString( buffer );
			//GameMsgPrint::getSingleton().showMidTip( MID_MULTIMAP_AUTOFIND_FAIL );
			return MOVE_POINT_CANNOT_WALK;
		}
	}

	tagPassMapPath	mapPath;
	mapPath.m_nMapId			= nDestMapID;
	mapPath.m_bNpcTran			= true;
	mapPath.m_bInCopy			= false;
	mapPath.m_Dest.m_nNpcResId	= nNpcResId;
	m_multiMapPath.push_back( mapPath );
	
	int nMoveResult = MOVE_SUCCESS;
	pMainplayer->setAutoFind( true );
	if ( m_multiMapPath[0].m_bNpcTran )
	{
		nMoveResult = GameNpcDialog::getSingleton().talkByResId( m_multiMapPath[0].m_Dest.m_nNpcResId );
	}
	else
	{
		nMoveResult = pMainplayer->CmdMoveTo( m_multiMapPath[0].m_Dest.m_Pos.X, m_multiMapPath[0].m_Dest.m_Pos.Y, 0 );
	}

	if ( nMoveResult != MOVE_SUCCESS )
	{
		char buffer[40];
		sprintf( buffer, "ShowMoveFailReson( %d )", nMoveResult );
		GameManager::getSingleton().getScriptVM()->callString( buffer );
		pMainplayer->setAutoFind( false );
	} else {
		GameManager::getSingleton().getScriptVM()->callString( "autoRideHorse()");
	}

	m_curFindTarget = m_multiMapPath.front();

	m_multiMapPath.pop_front();
	return nMoveResult;
}

bool MultiMapAutoFind::succesConstructPath( PATH& path, MAP_POINT& nCurPathEndInfo, 
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
	
	// ��·�����ܱ������ӳ�������·�����
	if ( !bCanExtendPath )
	{
		trimPath( path );
	}
	
	return !bCanExtendPath;
}

//  �˼��ع����ǻ�����������������㷨
//	��Ϊ�������·���Ľ�����׼������ջû���κε�ͼ�ڵ㣬�򲻼���������һ��·��
//	��һ��·��������Ϻ�Ҫɾ������һ��Ҫ�����·�����غϵĲ���
//	�ж��ܷ���һ����ͼ�ڵ�Nѹ��ջ�ı�׼�ǣ���һ����ͼ�ڵ�N�������еĵ�ͼ�ڵ�·����
//	�жϵ�ǰ·���Ƿ�����ϵı�׼�ǣ�1.����ǰ�ڵ㣨����ջ�������Ľڵ㣩û�к�̽ڵ���������쵱ǰ·����
//	2.����ǰ�ڵ㣨����ջ�������Ľڵ㣩�ĺ�̽���Ѿ���·����ǰһ���ִ��ڣ�3.����ǰ�ڵ㣨����ջ�������Ľڵ㣩
//	��һ���ڵ���Ϊ�������ʼĿ��㣬��������쵱ǰ·��
//	������һ��·����Ĺ�������ջ��Ϊ�գ������ڵ�ǰ·���в���ջ��Ԫ�أ�����·��ɾ����ջ��Ԫ��Ϊ����·������
void MultiMapAutoFind::loadShortestPaths( int nBenginMapId )
{
	// ���ҳ���ǰ��ͼ�����е�ͼ�����·����
	m_shortestPathes.clear();

	int		nCurMapId = nBenginMapId;
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
		}

		// ��ջ����û�нڵ㣬��û����Ҫ�����������һ��·��
		if ( pathPoints.empty() )
		{
			break;
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

void MultiMapAutoFind::continueNextMapAuto( MainGamePlayer* pMainplayer )
{
	if ( m_multiMapPath.empty() )
	{
		pMainplayer->setAutoFind( false );
		return;
	}
	m_uLastCallTime = timeGetTime();
}

void MultiMapAutoFind::CheckArriveTarget( MainGamePlayer* pMainplayer )
{
	if ( pMainplayer == NULL )
	{
		return;
	}
	GameMap *map = pMainplayer->getCurMap();
	if (map == NULL)
	{
		return;
	}
	int mapId = map->getMapId();
	if ( m_multiMapPath.empty() )
	{
		if (m_curFindTarget.m_nMapId == mapId)
		{
			if (m_curFindTarget.m_bNpcTran)
			{
				GameNpcDialog::getSingleton().talkByResId(m_curFindTarget.m_Dest.m_nNpcResId);
			}
			else
			{
				pMainplayer->CmdMoveTo(m_curFindTarget.m_Dest.m_Pos.X, m_curFindTarget.m_Dest.m_Pos.Y);
			}
		}
	}
	else
	{
		if (m_multiMapPath.back().m_bNpcTran)
		{
			beginAutoFind(mapId, m_multiMapPath.back().m_nMapId, m_multiMapPath.back().m_Dest.m_nNpcResId);
		}
		else
		{
			beginAutoFind(mapId, m_multiMapPath.back().m_nMapId, m_multiMapPath.back().m_Dest.m_Pos.X, m_multiMapPath.back().m_Dest.m_Pos.Y);
		}
	}
}

void MultiMapAutoFind::stopMultiFind()
{
	m_uLastCallTime = 0;
	m_multiMapPath.clear();
	m_curFindTarget.m_nMapId = 0;
}

tagWorldMapSearchDef_ToLua*	MultiMapAutoFind::getWorldMapSearchDef( int nFromMapID )
{
	static_assert( sizeof( tagWorldMapSearchDef ) == sizeof( tagWorldMapSearchDef_ToLua ) );

	for ( vector<tagWorldMapSearchDef>::iterator iter = m_mapSearchVec.begin(), end = m_mapSearchVec.end();
			iter != end; ++iter )
	{
		if ( iter->FromMapID == nFromMapID )
		{
			return	( tagWorldMapSearchDef_ToLua* )&( *iter );
		}
	}

	return NULL;
}


tagWorldMapSearchDef* MultiMapAutoFind::getMapInfo( int nFromMapID, int nIndex )
{
	int	nCount = 0;
	for ( vector<tagWorldMapSearchDef>::iterator iter = m_mapSearchVec.begin(), end = m_mapSearchVec.end();
		iter != end; ++iter )
	{
		if ( iter->FromMapID == nFromMapID )
		{
			if ( nCount == nIndex )
			{
				return	&( *iter );
			}
			++nCount;
		}
	}

	return NULL;
}

void MultiMapAutoFind::savePathToFile()
{

}

void MultiMapAutoFind::update()
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainplayer == NULL )
	{
		m_multiMapPath.clear();
		m_curFindTarget.m_nMapId = 0;
		m_AutoMoveMapId = -1;
		return;
	}
	
	unsigned int uCurTime = timeGetTime();
	if (m_AutoMoveMapId != -1)
	{
		if (!pMainplayer->getAutoFind())
		{
			unsigned int now = uCurTime;
			if (now - m_AutoMoveTime >= 1000)
			{
				m_AutoMoveTime = now;
				const GameXYZ &pos = pMainplayer->GetPosition();
				if (pos.x / 10 == m_AutoMoveX && pos.z / 10 == m_AutoMoveY)
				{
					FinishAutoMove();
				}
				else
				{
					GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
					beginAutoFind(pGameMapShower->GetMaxMapContal()->getMapID(), m_AutoMoveMapId, m_AutoMoveX, m_AutoMoveY);
				}
			}
		}
	}

	if ( m_uLastCallTime == 0 || ( uCurTime - m_uLastCallTime ) < 1000 )
	{
		return;
	}

	m_uLastCallTime = 0;

	if ( m_multiMapPath.empty() )
	{
		return;
	}

	pMainplayer->setAutoFind( true );

	if ( m_multiMapPath[0].m_bNpcTran )
	{
		GameNpcDialog::getSingleton().talkByResId( m_multiMapPath[0].m_Dest.m_nNpcResId );
	}
	else
	{
		pMainplayer->CmdMoveTo( m_multiMapPath[0].m_Dest.m_Pos.X, m_multiMapPath[0].m_Dest.m_Pos.Y );
	}

	m_curFindTarget = m_multiMapPath.front();

	m_multiMapPath.pop_front();
}

bool MultiMapAutoFind::isSameDestination( int nNpcResID )
{
	int nCurTaklNpcMemID = GameNpcDialog::getSingleton().getTalkNpcMemID();
	if ( nCurTaklNpcMemID == 0 )
	{
		return false;
	}
	
	ActorManager& actorMgr	= ActorManager::getSingleton();
	GameActor*	pNpc		= actorMgr.FindActor( actorMgr.getActorIdByResId( nNpcResID ) );
	
	if ( nCurTaklNpcMemID != 0 && pNpc != NULL && nCurTaklNpcMemID == pNpc->GetID() )
	{
		return true;
	}

	return false;
}

bool MultiMapAutoFind::isSubPath( const PATH& subPath, const PATH& parentPath )
{
	return	std::search( parentPath.begin(), parentPath.end(), subPath.begin(), subPath.end() ) != parentPath.end();
}

void MultiMapAutoFind::trimPath( const PATH& newPath )
{
	for ( vector<PATH>::iterator iter = m_shortestPathes.begin(), end = m_shortestPathes.end();
			iter != end; ++iter )
	{
		// ����֤�¼����·���Ƿ����Ѵ���·�����Ӽ�
		if ( isSubPath( newPath, *iter ) )
		{
			return;
		}

		// ����֤�¼����·���Ƿ����Ѵ��ڵ�·���ĸ�����
		if ( isSubPath( *iter, newPath ) )
		{
			m_shortestPathes.erase( iter );
			break;
		}
	}

	// ���¼����·�������Ѵ��ڵ�·�����ϲ������κθ��ӹ�ϵ
	m_shortestPathes.push_back( newPath );
}

bool MultiMapAutoFind::isInPath( int nMapID, const PATH& path )
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

PATH_ITER MultiMapAutoFind::findMap( int nMapID, PATH& path )
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

void MultiMapAutoFind::generateShortestPath( int nSrcMapID, int nDestMapID )
{
	vector<size_t>	validPathKeyes;
	for ( size_t i = 0, size = m_shortestPathes.size(); i < size; ++i )
	{
		// ����ǰ·�����ܵ���Ŀ���ͼ��������
		if ( isInPath( nDestMapID, m_shortestPathes[i] ) )
		{
			// �������пɴ��ͼnDestMapID��·������Ϣ
			validPathKeyes.push_back( i );
		}
	}

	// �ҳ����·����Ӧ�ı�ʶ
	MAP_POINT	shortestPathInfo( INT_MAX, INT_MAX );
	for ( vector<size_t>::iterator iter = validPathKeyes.begin(), end = validPathKeyes.end(); 
			iter != end; ++iter )
	{
		PATH_ITER destMapPos = findMap( nDestMapID, m_shortestPathes[*iter] );

		int	nCurPathLength = (int)(destMapPos - m_shortestPathes[*iter].begin());
		if ( ( shortestPathInfo.second ) > (int)nCurPathLength )
		{
			shortestPathInfo = make_pair( (int)*iter, (int)nCurPathLength );
		}
	}

	// ��2�ŵ�ͼ֮��û��ͨ·�������κβ���
	if ( shortestPathInfo.first == INT_MAX )
	{
		return;
	}

	PATH&		shortestPath	= m_shortestPathes[(shortestPathInfo.first)];
	PATH_ITER	destMapPos		= findMap( nDestMapID, shortestPath );
	assert( destMapPos != shortestPath.end() );
	if ( destMapPos == shortestPath.end() )
	{
		return;
	}

	tagPassMapPath	mapPath;
	mapPath.m_bInCopy = false;
	for ( PATH_ITER iter = shortestPath.begin(); iter != destMapPos; ++iter )
	{
		// Ѱ�ҵ���Ӧ��ͼ���л������ķ�ʽ
		tagWorldMapSearchDef* pMapInfo = getMapInfo( iter->second, iter->first );
		assert( pMapInfo != NULL );
		if ( pMapInfo == NULL )
		{
			continue;
		}

		mapPath.m_nMapId = pMapInfo->FromMapID;
		if ( pMapInfo->NpcID != 0 )
		{
			mapPath.m_bNpcTran			= true;
			mapPath.m_Dest.m_nNpcResId	= pMapInfo->NpcID;
		}
		else
		{
			mapPath.m_bNpcTran		= false;
			mapPath.m_Dest.m_Pos.X	= pMapInfo->X;
			mapPath.m_Dest.m_Pos.Y	= pMapInfo->Y;
		}
		m_multiMapPath.push_back( mapPath );
	}
}

void MultiMapAutoFind::mergeNearMap()
{
	for ( MAPSERCHVEC_ITER iter = m_mapSearchVec.begin(), end = m_mapSearchVec.end(); iter != end; ++iter )
	{
		m_nearMap[iter->FromMapID].push_back( iter->ToMapID );
	}
}

int	MultiMapAutoFind::getNextMapNum( int nSrcMapID )
{
	std::map<int, std::vector<int>>::iterator i = m_nearMap.find(nSrcMapID);
	if (i != m_nearMap.end())
	{
		return i->second.size();
	}
	return 0;
}

int	MultiMapAutoFind::getNextMapInfo(int nSrcMapID, int nIndex)
{
	return	m_nearMap[nSrcMapID][nIndex];
}

void MultiMapAutoFind::SetAutoMove(int mapId, int x, int y)
{
	m_AutoMoveMapId = mapId;
	m_AutoMoveX = x;
	m_AutoMoveY = y;
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player)
	{
		player->SetAutoMove(true);
	}
}

void MultiMapAutoFind::StopAutoMove()
{
	m_AutoMoveMapId = -1;
	MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
	if (player)
	{
		player->SetAutoMove(false);
	}
}

bool MultiMapAutoFind::IsAutoMoving()
{
	return m_AutoMoveMapId != -1;
}

void MultiMapAutoFind::FinishAutoMove()
{
	tagCSPkgBody msgbody;
	msgbody.ActionReq.ActionID = ACTION_CMD_RET;
	msgbody.ActionReq.Data.CmdRet.CmdType = CLIENT_CMD_AUTO_MOVE;
	msgbody.ActionReq.Data.CmdRet.CmdRet = 0;
	SendGameServerMsg(ACTION_REQ, msgbody);
}
