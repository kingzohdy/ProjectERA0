#include "GameMaxMap.h"
#include "GameManager.h"
#include "GameActor.h"
#include "GameNpc.h"
#include "GameMainPlayer.h"
#include "GameCamera.h"
#include "GameManager.h"
#include "GameTeamManager.h"
#include "GameChatManager.h"
#include "GameMaskMap.h"
#include "../UILib/ui_gameui.h"
#include "GameManager.h"
#include "OgreRect.h"
#include "../UILib/ui_texture.h"
#include "../UILib/ui_framemgr.h"
#include "AutoFindLineManager.h"
#include "GameDBData.h"
#include "../UILib/ui_drawlineframe.h"
#include "OgreResourceManager.h"
#include "OgreTerrainData.h"
#include "OgreOSUtility.h"
#include "GamePlayManager.h"
#include "GameEngineTypes.h"
#include "GameFollowManager.h"
#include "GameClanManager.h"
#include "OgreLog.h"
#include "GameLoginManager.h"
#include <math.h>
#include <algorithm>

using namespace Ogre;
//	美术给的纹理到游戏中最终显示的变换原理:
//	1. 最开始时一张纹理A对应整个世界实际场景(对应字段m_MapWorldTextWidth, m_MapWorldTextHeight); 
//	2. 美术根据需要从纹理A中裁减出有效区域的纹理B(对应字段m_MapClicpTextWidth, m_MapClicpTextHeight);,
//	其中纹理B相对于纹理A的偏移量的对应字段是m_MapTextTop, m_MapTextLeft
//	3. 将纹理B规范化到纹理C(目前标准规格是1024*1024)(对应字段m_MapValidNormalizeTextWidth, m_MapValidNormalizeTextHeight); 
//	4. 将纹理C映射到游戏中的有效视口区域上对应字段(m_MapInfo.nMapPosWidth, m_MapInfo.nMapPosHeight)

// int	GameMapControl::m_nMapWidth		= 100;
// int	GameMapControl::m_nMapHeight	= 100;
// int	GameMapControl::m_nMapId		= 0;

extern FrameManager *g_pFrameMgr;

extern int	FaceAngleBetween( int from_x, int from_y, int to_x, int to_y );

void translateMaxmapPoint( int& x, int& y, int scale )
{
	GameMapControl* pMapControl = GameManager::getSingleton().getGameMapShower()->GetMaxMapContal();
	pMapControl->getPointScreenCoord( x, y, "WorldMapFrameMapTexture", scale );
}

const tagPworldDef_ToLua* getPworldDef( int nPworldId )
{
	static_assert( sizeof( tagPworldDef_ToLua ) == sizeof( tagPworldDef ) );
	const tagPworldDef_ToLua* pDef = (const tagPworldDef_ToLua*)DBDataManager::getSingleton().getPworldDef( nPworldId );
	return (const tagPworldDef_ToLua*)DBDataManager::getSingleton().getPworldDef( nPworldId );
}

const char* getMapName( int nMapId )
{
	const char* szName = DBDataManager::getSingleton().getMapDef(nMapId).Name;
	if( LoginManager::getSingleton().isCloseLine() )
	{
		static char buf[RES_NAME_LEN];
		strcpy(buf,szName);
		char* tmp = buf + strlen(buf) - 2;
		if( strcmp(tmp,"线") == 0 )
		{
			tmp = buf;
			tmp[strlen(szName) -4] = '\0';
			return tmp;
		}
		return szName;
	}else{
		return szName;
	}
// 	return DBDataManager::getSingleton().getMapDef(nMapId).Name;
}

const tagMapDef_ToLua* getMapDef( int nMapId )
{
	static_assert( sizeof( tagMapDef_ToLua ) == sizeof( tagMapDef ) );
	const tagMapDef* pMapDef = &( DBDataManager::getSingleton().getMapDef( nMapId ) );
	return ( const tagMapDef_ToLua* )pMapDef;
}

void writeErrorLog( const char* pBuffer )
{
	LOG_PRIVATE( pBuffer );
}

GameNpcEnterView::GameNpcEnterView(void)
{
	GameMsgPrint::getSingleton().registGameMsgHandler( this );
}

GameMapControl::GameMapControl(void) : m_bMoveMap( false ), m_bBeenMove( false ), m_nMoveOffX( 0 ),m_nMoveOffY( 0 )
{
	m_MapInfo.fZoom		= 0.6f;
	m_pNpcEnterView		= new GameNpcEnterView();

	m_MapTexInfo.m_MapValidNormalizeTextWidth	= 0;
	m_MapTexInfo.m_MapValidNormalizeTextHeight	= 0;
	m_MapTexInfo.m_MapTextTop					= 0;
	m_MapTexInfo.m_MapTextLeft					= 0;
	m_MapTexInfo.m_MapClicpTextWidth			= 0;
	m_MapTexInfo.m_MapClicpTextHeight			= 0;
	m_MapTexInfo.m_MapWorldTextWidth			= 0;
	m_MapTexInfo.m_MapWorldTextHeight			= 0;
	memset( &m_lastFindDest, 0, sizeof( m_lastFindDest ) );
}

GameMapControl::~GameMapControl(void)
{
	delete m_pNpcEnterView;
	m_pNpcEnterView = NULL;
}

void GameMapControl::resetMapControl()
{
	memset( &m_lastFindDest, 0, sizeof( m_lastFindDest ) );
}

void GameMapControl::faceToNorth()
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	pMainPlayer->m_pCamera->SetAngle( 0 );	
}

tagMaxMapInfo& GameMapControl::updateMapInfo()
{
	int RealWidth		= (int)(m_ViewWidth  / m_MapInfo.fZoom);
	int RealHeight		= (int)(m_ViewHeight / m_MapInfo.fZoom);

	int PlayerX, PlayerY;
	// 求出主角相对于完整的有效地图纹理左上角的偏移量
	getMainPlayerPos( PlayerX, PlayerY );

	if ( !m_bBeenMove )
	{
		if ( PlayerX <= RealWidth / 2 )							// 处理左极限情况,此为静态确定的情况
		{
			m_MapInfo.nMapPosStartX = 0;
		}
		else if ( NORMALIZE_TEX_WIDTH - PlayerX <= RealWidth / 2 )	// 处理右极限情况,此为静态确定的情况
		{
			m_MapInfo.nMapPosStartX = NORMALIZE_TEX_WIDTH - RealWidth;
		}
		else
		{															// 处理中间情况,此为动态变换的情况
			m_MapInfo.nMapPosStartX = PlayerX - RealWidth / 2;
		}

		if ( PlayerY <= RealHeight / 2 )							// 处理上部极限情况,此为静态确定的情况
		{
			m_MapInfo.nMapPosStartY = 0;
		}
		else if ( NORMALIZE_TEX_HEIGHT - PlayerY <= RealHeight / 2 )	// 处理下部极限情况,此为静态确定的情况
		{
			m_MapInfo.nMapPosStartY = NORMALIZE_TEX_HEIGHT - RealHeight;
		}
		else
		{													// 处理中间情况,此为动态变换的情况
			m_MapInfo.nMapPosStartY = PlayerY - RealHeight / 2;
		}
	}
	else
	{// 主角色移动前肯定会先静态的确定地图的显示情况,故主角移动的时候地图的显示情况总是以以前的地图显示情况为参考
		if ( m_MapInfo.nMapPosStartX + m_nMoveOffX <= 0 )								// 处理向左移动到极限的情况,此为静态确定的情况
		{
			m_MapInfo.nMapPosStartX = 0;
		}
		else if ( m_MapInfo.nMapPosStartX + RealWidth + m_nMoveOffX >= NORMALIZE_TEX_WIDTH )	// 处理向右移动到极限的情况,此为静态确定的情况
		{
			m_MapInfo.nMapPosStartX = NORMALIZE_TEX_WIDTH - RealWidth;
		}
		else
		{																			// 处理中间情况,此为动态变换的情况
			m_MapInfo.nMapPosStartX = m_MapInfo.nMapPosStartX + m_nMoveOffX;
		}

		if ( m_MapInfo.nMapPosStartY + m_nMoveOffY <= 0 )									// 处理上部极限情况,此为静态确定的情况
		{
			m_MapInfo.nMapPosStartY = 0;
		}
		else if ( m_MapInfo.nMapPosStartY + RealHeight + m_nMoveOffY >= NORMALIZE_TEX_HEIGHT )	// 处理下部极限情况,,此为静态确定的情况
		{
			m_MapInfo.nMapPosStartY = NORMALIZE_TEX_HEIGHT - RealHeight;
		}
		else
		{																				// 处理中间情况,此为动态变换的情况
			m_MapInfo.nMapPosStartY = m_MapInfo.nMapPosStartY + m_nMoveOffY;
		}
	}

	m_MapInfo.nPlayerX = (int)(( PlayerX - m_MapInfo.nMapPosStartX ) * m_MapInfo.fZoom);
	m_MapInfo.nPlayerY = (int)(( PlayerY - m_MapInfo.nMapPosStartY ) * m_MapInfo.fZoom);
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return m_MapInfo;
	}

	m_MapInfo.fAngle		= pMainPlayer->getAngle() + 90.0f;
	m_MapInfo.fCamAngle		= pMainPlayer->m_pCamera->getAngleLR();	

	m_MapInfo.nMapPosWidth	= RealWidth;
	m_MapInfo.nMapPosHeight	= RealHeight;
	m_MapInfo.mapId			= m_nMapId;

	return m_MapInfo;
}

tagMapActorInfo& GameMapControl::updateMapCaptainInfo()
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	m_CaptainInfo.m_nPlayerX = -1;
	m_CaptainInfo.m_nPlayerY = -1;
	if ( pMainPlayer->getTeamName() == "" || pMainPlayer->isCaptain() )
	{
		return m_CaptainInfo;
	}

	const tagTeamInfo_ToLua* TeamInfo = GameTeamManager::getSingleton().getTeamInfo();
	for ( int i = 0; i < TeamInfo->MemberNum; ++i )
	{
		const tagTeamMember_ToLua& teamMember = TeamInfo->TeamMember[i];
		// 若队友不在线或者当前考察的队友不是队长
		if ( teamMember.Position != TEAM_POSITION_CAPTAIN  )
		{
			continue;
		}

		m_CaptainInfo.m_nId				= 0;
		m_CaptainInfo.m_nMapId			= teamMember.MapID;
		m_CaptainInfo.m_nPosition		= teamMember.Position;
		m_CaptainInfo.m_nOffLineTime	= teamMember.OffLineTime;
		strncpy( m_CaptainInfo.m_szName, teamMember.MemberName,NAME_LEN );
		GameTeamManager::getSingleton().updateNearPlayer();
		int nCount = GameTeamManager::getSingleton().getNearPlayerCount();
		for ( int j = 0; j < nCount; ++j )
		{
			GameActor* pNearPlayer = GameTeamManager::getSingleton().getNearPlayer( j );
			if ( pNearPlayer == NULL || !CompareRoleName( pNearPlayer->getName(), teamMember.MemberName) )
			{
				continue;
			}

			m_CaptainInfo.m_nId			= pNearPlayer->GetID();
			m_CaptainInfo.m_nRealX		= pNearPlayer->GetPosition().x / 10;
			m_CaptainInfo.m_nRealY		= pNearPlayer->GetPosition().z / 10;
			Pair coordOViewTex			=  getPointCoordOnTex( m_CaptainInfo.m_nRealX, m_CaptainInfo.m_nRealY );
			m_CaptainInfo.m_nPlayerX	= (int)(( coordOViewTex.first  - m_MapInfo.nMapPosStartX ) * m_MapInfo.fZoom);
			m_CaptainInfo.m_nPlayerY	= (int)(( coordOViewTex.second - m_MapInfo.nMapPosStartY ) * m_MapInfo.fZoom);

			return m_CaptainInfo;
		}	

		m_CaptainInfo.m_nRealX		= teamMember.Pos.X;
		m_CaptainInfo.m_nRealY		= teamMember.Pos.Y;
		Pair coordOViewTex			=  getPointCoordOnTex( m_CaptainInfo.m_nRealX, m_CaptainInfo.m_nRealY );
		m_CaptainInfo.m_nPlayerX	= (int)(( coordOViewTex.first  - m_MapInfo.nMapPosStartX ) * m_MapInfo.fZoom);
		m_CaptainInfo.m_nPlayerY	= (int)(( coordOViewTex.second - m_MapInfo.nMapPosStartY ) * m_MapInfo.fZoom);

		return m_CaptainInfo;
	}

	assert( !"队伍中不存在队长，数据有误！" );
	return m_CaptainInfo;
}

void GameMapControl::drawClanMemberPoint( const char* pszTextureName, const char* clanFrameName, const char* enemyFrameName )
{
	std::deque<Ogre::Point2D> friendPoints,enemyPoints;
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	::Texture* pWroldTexture = (::Texture*)g_pFrameMgr->FindLayoutFrame( pszTextureName );
	ACTOR_ITER iter;
	
	for( iter  = ActorManager::getSingleton().getActorBegin(); 
		iter != ActorManager::getSingleton().getActorEnd(); ++iter )
	{
		if( iter->second->getType() == GAT_PLAYER )
		{
			int nRealX		= iter->second->GetPosition().x / 10;
			int nRealY		= iter->second->GetPosition().z / 10;
			Pair coordOViewTex			=  getPointCoordOnTex( nRealX, nRealY );
			int nPlayerX	= (int)(( coordOViewTex.first  - m_MapInfo.nMapPosStartX ) * m_MapInfo.fZoom);
			int nPlayerY	= (int)(( coordOViewTex.second - m_MapInfo.nMapPosStartY ) * m_MapInfo.fZoom);
			float fRadiusX = pWroldTexture->GetWidth()/2 ;
			float fDistance = sqrt( (fRadiusX - nPlayerX)*(fRadiusX - nPlayerX) + (fRadiusX - nPlayerY)*(fRadiusX - nPlayerY) );
			if( fDistance < fRadiusX )
			{
				if( pMain->isInSameTeam( iter->second->getTeamWID() ) )
				{
					continue;
				}else if( pMain->isInSameClan( iter->second->getClanWID() ) )
				{
					//同公会
					friendPoints.push_back( Ogre::Point2D( nPlayerX, nPlayerY ) );				
				}else{
					enemyPoints.push_back( Ogre::Point2D( nPlayerX, nPlayerY ) );
				}
			}			
		}
	}
	DrawLineFrame* pFriendLine = static_cast<DrawLineFrame*>( g_pFrameMgr->FindLayoutFrame( clanFrameName ) );
	pFriendLine->SetDrawPointList( friendPoints );
	DrawLineFrame* pEnemyLine = static_cast<DrawLineFrame*>( g_pFrameMgr->FindLayoutFrame( enemyFrameName ) );
	pEnemyLine->SetDrawPointList( enemyPoints );
}

tagMapActorInfo& GameMapControl::updateMapTeamMemberInfo(  int nIndex )
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	const tagTeamInfo_ToLua* TeamInfo = GameTeamManager::getSingleton().getTeamInfo();
	m_TeamMemberInfo.m_nPlayerX = -1;
	m_TeamMemberInfo.m_nPlayerY = -1;
	if ( TeamInfo == NULL)
	{
		return m_TeamMemberInfo;
	}

	assert( nIndex < TeamInfo->MemberNum && nIndex >= 0 );

	const tagTeamMember_ToLua& teamMember = TeamInfo->TeamMember[nIndex];

	m_TeamMemberInfo.m_nId				= 0;
	m_TeamMemberInfo.m_nMapId			= teamMember.MapID;
	m_TeamMemberInfo.m_nPosition		= teamMember.Position;
	m_TeamMemberInfo.m_nOffLineTime		= teamMember.OffLineTime;
	strncpy( m_TeamMemberInfo.m_szName, teamMember.MemberName,NAME_LEN );

	GameTeamManager::getSingleton().updateNearPlayer();
	int nCount = GameTeamManager::getSingleton().getNearPlayerCount();
	for( int i = 0; i < nCount; i++ )
	{
		GameActor* pNearPlayer = GameTeamManager::getSingleton().getNearPlayer(i);
		if ( pNearPlayer != NULL && CompareRoleName(pNearPlayer->getName(), TeamInfo->TeamMember[nIndex].MemberName) )
		{
			m_TeamMemberInfo.m_nId			= pNearPlayer->GetID();
			m_TeamMemberInfo.m_nRealX		= pNearPlayer->GetPosition().x / 10;
			m_TeamMemberInfo.m_nRealY		= pNearPlayer->GetPosition().z / 10;
			Pair coordOViewTex				=  getPointCoordOnTex( m_TeamMemberInfo.m_nRealX, m_TeamMemberInfo.m_nRealY );
			m_TeamMemberInfo.m_nPlayerX		= (int)(( coordOViewTex.first  - m_MapInfo.nMapPosStartX ) * m_MapInfo.fZoom);
			m_TeamMemberInfo.m_nPlayerY		= (int)(( coordOViewTex.second - m_MapInfo.nMapPosStartY ) * m_MapInfo.fZoom);

			return m_TeamMemberInfo;
		}
	}

	m_TeamMemberInfo.m_nRealX	= TeamInfo->TeamMember[nIndex].Pos.X;
	m_TeamMemberInfo.m_nRealY	= TeamInfo->TeamMember[nIndex].Pos.Y;
	Pair coordOViewTex			=  getPointCoordOnTex( m_TeamMemberInfo.m_nRealX, m_TeamMemberInfo.m_nRealY );
	m_TeamMemberInfo.m_nPlayerX	= (int)(( coordOViewTex.first  - m_MapInfo.nMapPosStartX ) * m_MapInfo.fZoom);
	m_TeamMemberInfo.m_nPlayerY	= (int)(( coordOViewTex.second - m_MapInfo.nMapPosStartY ) * m_MapInfo.fZoom);
	return m_TeamMemberInfo;
}

tagMapActorInfo& GameMapControl::updateMinMapTargetInfo()
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GameActor* pTargetPlayer = ActorManager::getSingleton().FindActor( pMainPlayer->GetSelectTarget() );
	if ( pTargetPlayer != NULL)
	{
		m_SelectTargetInfo.m_nRealX		= pTargetPlayer->GetPosition().x / 10;
		m_SelectTargetInfo.m_nRealY		= pTargetPlayer->GetPosition().z / 10;
		Pair coordOViewTex				=  getPointCoordOnTex( m_SelectTargetInfo.m_nRealX, m_SelectTargetInfo.m_nRealY );
		m_SelectTargetInfo.m_nPlayerX	= (int)(( coordOViewTex.first  - m_MapInfo.nMapPosStartX ) * m_MapInfo.fZoom);
		m_SelectTargetInfo.m_nPlayerY	= (int)(( coordOViewTex.second - m_MapInfo.nMapPosStartY ) * m_MapInfo.fZoom);
		strncpy( m_SelectTargetInfo.m_szName, pTargetPlayer->getName(),NAME_LEN );
	}
	else
	{
		m_SelectTargetInfo.m_nPlayerX = 0;
		m_SelectTargetInfo.m_nPlayerY = 0;
	}

	return m_SelectTargetInfo;
}

void GameMapControl::setView( int nWidth, int nHeight )
{
	m_ViewWidth		= nWidth;
	m_ViewHeight	= nHeight;
}

void GameMapControl::setMapZoom( float fZoom )
{
	m_MapInfo.fZoom = fZoom;
}

// 通过像素偏移值计算所点击的像素在实际场景中的坐标位置
// 前置条件: 调用者要保证传递给该转换函数的坐标一定是有效区域的坐标点
// 应用: 在区域地图上点击的时候使用
// 前置条件: 调用者应保证传递给getMouseCurPos的x,y为相对于地图有效视口区域左上角顶点的偏移量(其中偏移量为屏幕坐标系)
// 后置条件: 输出当前鼠标点在地图上的位置与实际场景对应的坐标值
Pair GameMapControl::getMouseCurPos( int& x, int& y )
{	// nPlayerOffsetXOnClicpTexture
	// 定义用于存储被点击的点在真实游戏场景中的坐标	nPlayerOffsetXOnNormalizeTexture

	int nPlayerOffsetXOnNormalizeTexture = (int)(x / m_MapInfo.fZoom + m_MapInfo.nMapPosStartX);
	int nPlayerOffsetYOnNormalizeTexture = (int)(y / m_MapInfo.fZoom + m_MapInfo.nMapPosStartY);

	int nPlayerOffsetXOnClicpTexture = nPlayerOffsetXOnNormalizeTexture * m_MapTexInfo.m_MapClicpTextWidth / 
		NORMALIZE_TEX_WIDTH;
	int nPlayerOffsetYOnClicpTexture = nPlayerOffsetYOnNormalizeTexture * m_MapTexInfo.m_MapClicpTextHeight / 
		NORMALIZE_TEX_HEIGHT;

	int	nPlayerOffsetXOnWorldTexture = nPlayerOffsetXOnClicpTexture + m_MapTexInfo.m_MapTextLeft;
	int	nPlayerOffsetYOnWorldTexture = nPlayerOffsetYOnClicpTexture + m_MapTexInfo.m_MapTextTop;

	x = nPlayerOffsetXOnWorldTexture * m_nMapWidth / m_MapTexInfo.m_MapWorldTextWidth;
	y = m_nMapHeight - ( nPlayerOffsetYOnWorldTexture * m_nMapHeight / m_MapTexInfo.m_MapWorldTextHeight );

	return Pair( x, y );
}

void GameMapControl::mapMoveto( int x, int y )//此为当前点击位置与视口的像素偏移值
{
	if( (GetKeyState(VK_LBUTTON) & 0xff00) )
	{
		Pair mouseSceneCoord = getMouseCurPos( x, y );
		if( ( (GetKeyState(VK_LSHIFT) & 0xff00) || (GetKeyState(VK_RSHIFT) & 0xff00) ) )
		{
			ChatManager::getSingleton().setPositionLink( getMapID(), mouseSceneCoord.first, mouseSceneCoord.second );
		}
		else
		{
			MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
			if (  pMainPlayer != NULL ) 
			{
				pMainPlayer->CmdMoveTo( mouseSceneCoord.first, mouseSceneCoord.second );
			}
		}
	}
}

int GameMapControl::linkMoveto( int nDestMapId, int nDestRealX, int nDestRealY )
{
	if ( !canAutoFindPath() )
	{
		return MOVE_POINT_CANNOT_WALK;  
	}

	GameFollowManager& flowMgr = GameFollowManager::getSingleton();
	if ( flowMgr.isWeiXing() )
	{
		flowMgr.cancelWeiXing();
	}

	// 取消鼠标自动点击
	GamePlayManager::getSingleton().getSelectManager()->breakAutoClick();
	GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
	MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();

	const tagMapDef &mapDef = DBDataManager::getSingleton().getMapDef( nDestMapId );
	nDestMapId = mapDef.ReferMapID ? mapDef.ReferMapID : mapDef.MapID;

	m_lastFindDest.m_bNpcTran	= false;
	m_lastFindDest.m_nMapId		= nDestMapId;
	m_lastFindDest.m_bInCopy	= false;
	m_lastFindDest.m_Dest.m_Pos.X = nDestRealX;
	m_lastFindDest.m_Dest.m_Pos.Y = nDestRealY;

	if( nDestMapId != getMapID() )
	{
		char buf[128];
		sprintf(buf,"ChangeAreaMap( %d, %d )", getMapID(), pGameMapShower->getRealMapId() );
		GameManager::getSingleton().getScriptVM()->callString( buf );
	}

	int nSrcMapID = getMapID();
	// 处理在副本地图中，跨地图寻路
	if ( pGameMapShower->getRealMapId() >= GAME_PWORLD_MAPID && nSrcMapID != nDestMapId )
	{
		nSrcMapID = pGameMapShower->getRealMapId();
		m_lastFindDest.m_bInCopy = true;
		nDestMapId = 0;
	}
	int nMoveResult = mutilMapFinder.beginAutoFind( nSrcMapID, nDestMapId, nDestRealX, nDestRealY );	
	GameManager::getSingleton().SendUIEvent( "GE_BEGIN_AUTO_FIND" );
	return nMoveResult;
}

int GameMapControl::linkMoveto( int nDestMapId, int nNpcResId )
{
	if ( !canAutoFindPath() )
	{
		return MOVE_POINT_CANNOT_WALK;
	}
	
	GameFollowManager& flowMgr = GameFollowManager::getSingleton();
	if ( flowMgr.isWeiXing() )
	{
		flowMgr.cancelWeiXing();
	}

	// 取消鼠标自动点击
	GamePlayManager::getSingleton().getSelectManager()->breakAutoClick();
	GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
	MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();
	
	m_lastFindDest.m_bNpcTran	= true;
	m_lastFindDest.m_nMapId		= nDestMapId;
	m_lastFindDest.m_bInCopy	= false;
	m_lastFindDest.m_Dest.m_nNpcResId = nNpcResId;

	if( nDestMapId != getMapID() )
	{
		char buf[128];
		sprintf(buf,"ChangeAreaMap( %d, %d )", getMapID(), pGameMapShower->getRealMapId() );
		GameManager::getSingleton().getScriptVM()->callString( buf );
	}

	int nSrcMapID = getMapID();
	// 处理在副本地图中，跨地图寻路
	if ( pGameMapShower->getRealMapId() >= GAME_PWORLD_MAPID && nSrcMapID != nDestMapId )
	{
		nSrcMapID = pGameMapShower->getRealMapId();
		m_lastFindDest.m_bInCopy = true;
		nDestMapId = 0;
	}
	//int nMoveResult = mutilMapFinder.beginAutoFind(  getMapID(), nDestMapId, nNpcResId );
	int nMoveResult = mutilMapFinder.beginAutoFind( nSrcMapID, nDestMapId, nNpcResId );
	GameManager::getSingleton().SendUIEvent( "GE_BEGIN_AUTO_FIND" );
	return nMoveResult;
}


void GameMapControl::follow( int nDestMapId, int nDestRealx, int nDestRealY, int nDist  )
{
	GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
	MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();
	mutilMapFinder.beginAutoFind( pGameMapShower->getRealMapId(), nDestMapId, nDestRealx, nDestRealY, nDist );
}

void GameMapControl::DrawPoint( const char* pszLineName )
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	// 当玩家不处于自动寻路状态的时候,清空所有的寻路点集合
	if ( pMainPlayer == NULL || !pMainPlayer->getAutoFind() )
	{
		return;
	}

	// 下面只处理处于自动寻路的情况
	char buf[256];
	AutoFindLineManager* pAutoLine = static_cast<AutoFindLineManager*>( 
		GameManager::getSingleton().getGameMapShower()->getAutoLine() );
	Ogre::Point2D	playerPoint( pMainPlayer->GetPosition().x, pMainPlayer->GetPosition().z ); 
	DrawLineFrame*	pLine		= static_cast<DrawLineFrame*>( g_pFrameMgr->FindLayoutFrame( pszLineName ) );
	LayoutFrame*	pRelFrame	= static_cast<LayoutFrame*>( g_pFrameMgr->FindLayoutFrame( "WorldMapFrameMapTexture" ) );
	// 更新自动路线图前,设置其坐标转换算法
	pAutoLine->updatePath( playerPoint, pMainPlayer->getCurPosotion() );
	pLine->SetDrawPointList( *(pAutoLine->getPathPoints()) );
	sprintf( buf, "DrawEffect(%d, %d, %d)", pAutoLine->getlastAutoDestScreenPoint().x + pRelFrame->GetRealLeft(), 
		pAutoLine->getlastAutoDestScreenPoint().y + pRelFrame->GetRealTop(), 1 );
	GameManager::getSingleton().getScriptVM()->callString( buf );
}

// x, y为实际场景中的坐标, nScale是为了确保在实际场景较小的时候获得更高的精度
Pair GameMapControl::getPointScreenCoord( int& x, int& y, const char* pszTextureName, int nScale )
{
	::Texture* pWroldTexture	= (::Texture*)g_pFrameMgr->FindLayoutFrame( pszTextureName );

	float nPlayerOffsetXOnClicpTexture = x * m_MapTexInfo.m_MapWorldTextWidth / ( (float)m_nMapWidth * 
		nScale ) - m_MapTexInfo.m_MapTextLeft;
	float nPlayerOffsetYOnClicpTexture = ( m_nMapHeight * nScale - y ) * m_MapTexInfo.m_MapWorldTextHeight  
		/ ( (float)m_nMapHeight * nScale ) - m_MapTexInfo.m_MapTextTop;

	float nPlayerOffsetXOnNormalizeTexture = nPlayerOffsetXOnClicpTexture * NORMALIZE_TEX_WIDTH / m_MapTexInfo.m_MapClicpTextWidth ;
	float nPlayerOffsetYOnNormalizeTexture = nPlayerOffsetYOnClicpTexture * NORMALIZE_TEX_HEIGHT / m_MapTexInfo.m_MapClicpTextHeight;

	float nPlayerOffsetXOnViewTexture	= nPlayerOffsetXOnNormalizeTexture * m_MapTexInfo.m_MapViewWidth / NORMALIZE_TEX_WIDTH;
	float nPlayerOffsetYOnViewTexture	= nPlayerOffsetYOnNormalizeTexture * m_MapTexInfo.m_MapViewHeight / NORMALIZE_TEX_HEIGHT;

	x = (int)nPlayerOffsetXOnViewTexture;
	y = (int)nPlayerOffsetYOnViewTexture;
	return Pair( x, y );
}

// x, y为实际场景中的坐标
// 输出x,y在当前相对实际显示的有效地图纹理左上角的偏移量
Pair GameMapControl::getPointCoordOnTex( int x, int y )
{
	double nPlayerOffsetXOnClicpTexture = x * m_MapTexInfo.m_MapWorldTextWidth / (double)m_nMapWidth - m_MapTexInfo.m_MapTextLeft;
	double nPlayerOffsetYOnClicpTexture = ( m_nMapHeight - y ) * m_MapTexInfo.m_MapWorldTextHeight / (double)m_nMapHeight - 
		m_MapTexInfo.m_MapTextTop;

	int nPlayerOffsetXOnNormalizeTexture = int(nPlayerOffsetXOnClicpTexture * NORMALIZE_TEX_WIDTH / 
		m_MapTexInfo.m_MapClicpTextWidth);
	int nPlayerOffsetYOnNormalizeTexture = int(nPlayerOffsetYOnClicpTexture * NORMALIZE_TEX_HEIGHT / 
		m_MapTexInfo.m_MapClicpTextHeight);

	// 不进行任何拉伸变化,否则会造成图像显示有误
	/*int nPlayerOffsetXOnViewTexture = nPlayerOffsetXOnNormalizeTexture * NORMALIZE_TEX_WIDTH / 
	m_MapTexInfo.m_MapValidNormalizeTextWidth;
	int nPlayerOffsetYOnViewTexture = nPlayerOffsetYOnNormalizeTexture * NORMALIZE_TEX_HEIGHT / 
	m_MapTexInfo.m_MapValidNormalizeTextHeight;*/

	return Pair( nPlayerOffsetXOnNormalizeTexture, nPlayerOffsetYOnNormalizeTexture );
}

void GameMapControl::setMapTextRect( int nWidth, int nHeight, int nLeftOffset, int nTopOffset, 
									int nCompleteTextWidth, int nCompleteTextHeight, int nClicpTextWidth, 
									int nClicpTextHeight, int nViewWidth, int nViewHeight )
{
	m_MapTexInfo.m_MapValidNormalizeTextWidth	= nWidth;
	m_MapTexInfo.m_MapValidNormalizeTextHeight	= nHeight;
	m_MapTexInfo.m_MapTextTop					= nTopOffset;
	m_MapTexInfo.m_MapTextLeft					= nLeftOffset;
	m_MapTexInfo.m_MapWorldTextWidth			= nCompleteTextWidth;
	m_MapTexInfo.m_MapWorldTextHeight			= nCompleteTextHeight;
	m_MapTexInfo.m_MapClicpTextWidth			= nClicpTextWidth;
	m_MapTexInfo.m_MapClicpTextHeight			= nClicpTextHeight;
	m_MapTexInfo.m_MapViewWidth					= nViewWidth;
	m_MapTexInfo.m_MapViewHeight				= nViewHeight;
}

void GameMapControl::updateMovemap( int x, int y )
{
	if( m_bMoveMap && (GetKeyState(VK_RBUTTON) & 0xff00) )
	{
		m_nMoveOffX = -( x - m_nStartX ); // TODO: 需要理解的地方
		m_nMoveOffY = -( y - m_nStartY );
		m_nStartX = x;
		m_nStartY = y;
		GameManager::getSingleton().GetGameUI()->SetCurrentCursor("loot");
		updateMapInfo();
		m_nMoveOffX = 0;
		m_nMoveOffY = 0;
	}
}

void GameMapControl::endMoveMap()
{
	m_bMoveMap = false;
	m_nMoveOffX	= 0;
	m_nMoveOffY = 0;
}

void GameMapControl::getMainPlayerPos( int& x, int& y )
{
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}

	Pair pointViewTexCoord = getPointCoordOnTex( pMainPlayer->GetPosition().x / 10, pMainPlayer->GetPosition().z / 10 );
	x = pointViewTexCoord.first;
	y = pointViewTexCoord.second;
}

void GameMapControl::setMapInfo( int width, int height, int mapId )
{
	m_nMapWidth		= width;
	m_nMapHeight	= height;
	m_nMapId		= mapId;
}

void GameMapControl::startMoveMap( int x, int y )
{
	if( (GetKeyState(VK_RBUTTON) & 0xff00) )
	{
		m_bMoveMap  = true;
		m_bBeenMove = true;
		m_nStartX	= x;
		m_nStartY	= y;
		m_nMoveOffX	= 0;
		m_nMoveOffY = 0;
	}
}

void GameMapControl::setMapWithPlayer( bool flag )
{
	m_bBeenMove = !flag;
}

tagMapActorInfo& GameMapControl::getTargetPos(int x, int y)
{
	m_TargetPos.m_nPlayerX = x;
	m_TargetPos.m_nPlayerY = y;
	Pair targetSceneCoord = getMouseCurPos( m_TargetPos.m_nPlayerX, m_TargetPos.m_nPlayerY );
	m_TargetPos.m_nPlayerX = targetSceneCoord.first / 10 * m_MapTexInfo.m_MapValidNormalizeTextWidth / m_nMapWidth;
	m_TargetPos.m_nPlayerY = ( m_nMapHeight - targetSceneCoord.second / 10 ) * m_MapTexInfo.m_MapValidNormalizeTextHeight / m_nMapHeight;
	return m_TargetPos;
}

const char* GameMapControl::getMapName( int nMapId )
{
	/*char szMapName[32] = {0};
	strncpy( szMapName, DBDataManager::getSingleton().getMapDef(nMapId).Name, 32 );*/
	return ::getMapName(nMapId);
}

tagMinMapTeamMebPos& GameMapControl::getPosition( int nSrcX, int nSrcY, int nDesX, int nDesY, int nDist )
{
	int nDir = FaceAngleBetween( nSrcX, nSrcY, nDesX, nDesY);
	m_TeamMebPos.nX   = (int) (nSrcX + (double)nDist * cos((double)(nDir)*3.14159265/180.0f));
	m_TeamMebPos.nY   = (int) (nSrcY - (double)nDist * sin((double)(nDir)*3.14159265/180.0f));
	return m_TeamMebPos;
}


// 获得npc类型
int GameMapControl::getNpcRace( int nResId )
{
	return DBDataManager::getSingleton().GetNpcDisplayInfo( nResId ).race;
}

// 判断npc类型
bool GameMapControl::isNpcRaceType( int nResId, int nType )
{
	const tagNpcDisplayInfo& npcInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( nResId );
	bool bResult = ( npcInfo.race & nType ) != 0;
	return bResult;
}

// 判断是否能够进行自动寻路
bool GameMapControl::canAutoFindPath()
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	Rider* pRider = pMainplayer->getRider();
	if ( pMainplayer->isInRide() && !pRider->isOwnRide() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTOFIND_SELF_LIMIT );
		return false;
	}

	GameMapShower*		pGameMapShower = GameManager::getSingleton().getGameMapShower();
	MultiMapAutoFind&	mutilMapFinder = pGameMapShower->getMultiMapFindControl();
	if (mutilMapFinder.IsAutoMoving())
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTOFIND_SELF_LIMIT );
		return false;
	}

	// 若处于自动飞行状态
	if ( pMainplayer->getActState().GetState() == GSACT_AUTOFLY )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTOFIND_IN_FLY_STATE );
		return false;
	}
	
	// 摆摊状态不能寻路
	if ( pMainplayer->getActState().GetState() == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_AUTOFIND_SELF_LIMIT );
		return false;
	}
	return true;
}

GameNpcEnterView::~GameNpcEnterView(void)
{
	//GameMsgPrint::getSingleton().unRegistGameMsgHandler( this );
}

void GameNpcEnterView::npcEnterView( int id )
{
	tagUIMsgBody uiMsg;
	uiMsg.NpcId.m_nNpcId = id;
	
	GameUIMsg::getSingleton().setUIMsg( uiMsg );
	GameManager::getSingleton().SendUIEvent("GE_MINMAPNPC_ENTER");
}

void GameNpcEnterView::npcLeaveView( int id )
{

}

void GameMapShower::calculateMapSize( int nMapID )
{
	if ( m_mapSize.find( nMapID) != m_mapSize.end() )
	{
		return;
	}
	const tagMapDef &mapdef = DBDataManager::getSingleton().getMapDef(nMapID);
	if( mapdef.MapWidth > 0 && mapdef.MapHeight > 0 )
	{
		m_mapSize[nMapID] = Pair( mapdef.MapWidth, mapdef.MapHeight );
		return;
	}

	char chunkname[256] = { '\0' };
	sprintf(chunkname, "maps\\%d\\%d.tile", nMapID, nMapID);

	Resource *pres = ResourceManager::getSingleton().blockLoad(chunkname);
	if(pres == NULL)
	{
		return;
	}

	TerrainTileSource *ptiledata = static_cast<TerrainTileSource *>(pres);
	int		nWidthGrid = ptiledata->m_TileDesc.gridX;
	int		nHeightGrid = ptiledata->m_TileDesc.gridY;
	float	fGridSize = ptiledata->m_TileDesc.gridSize;

	int		mapWidth  = (int)(nWidthGrid  * fGridSize);
	int		mapHeight = (int)(nHeightGrid * fGridSize);
	m_mapSize[nMapID] = Pair( mapWidth, mapHeight );
	OGRE_RELEASE(pres);
}

GameMapShower::GameMapShower() : m_nRealMapId(0), m_nLineNo( DEFAULT_LINE_NO )
{
	m_pMaxMapContal = new GameMapControl();
	m_pMinMapContal = new GameMapControl();

	DBDataManager::getSingleton().loadMapNpc( m_mapNpcVector );
	DBDataManager::getSingleton().loadSceneAreaDef( m_mapAllSceneArea );
	DBDataManager::getSingleton().loadMapDef( m_mapVector );
	//DBDataManager::getSingleton().loadPworldDef( m_pworldVector );
		
	fillMultiLineMap();
	mergeMapNpc();
	mergeMapSceneArea();
}

bool GameMapShower::isMultiLineMap( int nRealMapID )
{
	return m_multiLineMap.find( nRealMapID ) != m_multiLineMap.end();
}

void GameMapShower::fillMultiLineMap()
{
	for ( std::vector<MAPDEF>::iterator iter = m_mapVector.begin(), end = m_mapVector.end(); iter != end; ++iter )
	{
		const MAPDEF& oneMapDef = *iter;
		if ( oneMapDef.ReferMapID != 0 )
		{
			m_multiLineMap.insert( oneMapDef.MapID );
			m_multiLineMap.insert( oneMapDef.ReferMapID );
		}
	}
}

GameMapShower::~GameMapShower()
{
	m_mapNpcVector.clear();   
	m_allMapNpcMap.clear();  
	m_allMapSceneAreaMap.clear();
	m_mapAllSceneArea.clear();   
	delete m_pMaxMapContal;
	delete m_pMinMapContal;
}


bool GameMapShower::isInMapNpcDataBase( int nMapId )
{
	std::map<int,MAPNPCINFOVECTOR>::iterator iter = m_allMapNpcMap.find( nMapId );
	return iter != m_allMapNpcMap.end();
}

int GameMapShower::getMapNpcCount( int nMapId )
{
	std::map<int,MAPNPCINFOVECTOR>::iterator iter = m_allMapNpcMap.find( nMapId );
	if( iter == m_allMapNpcMap.end() )
	{
		return 0;
	}
	return (int)m_allMapNpcMap[nMapId].size();
}

const tagMapActorInfo& GameMapShower::getMapNpcInfo( int nId, int nIndex )
{
	return m_allMapNpcMap[nId][nIndex];
}

bool operator== ( const tagMapActorInfo& lhs, int nDefId )
{
	return lhs.m_nId == nDefId;
}

void GameMapShower::clearMapNpc( int nMapId )
{
	MAPNPCINFOVECTOR& vectorNpc = m_allMapNpcMap[nMapId];
	vectorNpc.clear();
	for (std::vector<MAPNPC>::iterator iter = m_mapNpcVector.begin(), end = m_mapNpcVector.end(); iter != end; ++iter )
	{
		if( iter->MapID == nMapId )
		{
			tagMapActorInfo npcInfo;
			npcInfo.m_nId		= iter->MonsterID;	
			npcInfo.m_nMapId	= iter->MapID;
			npcInfo.m_nRealX	= iter->Pos.X;
			npcInfo.m_nRealY	= iter->Pos.Y;
			npcInfo.m_bShow		= !( iter->Unshown );
			//npcInfo.m_nSpanMapCamp = iter->SpanMapCamp;
			strncpy( npcInfo.m_szName, DBDataManager::getSingleton().getMonsterDef(npcInfo.m_nId).Name,NAME_LEN );
			getQuestList( npcInfo.m_nId );
			vectorNpc.push_back( npcInfo );
		}		
	}

}

void GameMapShower::updateMapNpcInfo( int nDefId, int nRealX, int nRealY )
{
		int nMapId = getRealMapId();
		MAPNPCINFOVECTOR& vectorNpc = m_allMapNpcMap[nMapId];
		MAPNPCINFOVECTOR::iterator iter = std::find( vectorNpc.begin(), vectorNpc.end(), nDefId );
		if ( iter != vectorNpc.end() )
		{
			iter->m_nRealX = nRealX;
			iter->m_nRealY = nRealY;
		}
		else
		{
			tagMapActorInfo npcInfo;
			npcInfo.m_nId		= nDefId;	
			npcInfo.m_nMapId	= nMapId;
			npcInfo.m_nRealX	= nRealX;
			npcInfo.m_nRealY	= nRealY;
			npcInfo.m_bShow		= true;
			strncpy( npcInfo.m_szName, DBDataManager::getSingleton().getMonsterDef( nDefId ).Name,NAME_LEN );
			npcInfo.m_szMapDesc[0] = '\0';
			getQuestList( nDefId );

			vectorNpc.push_back( npcInfo );
		}

		GameManager::getSingleton().SendUIEvent( "GE_UPDATE_NPC" );
}

void GameMapShower::deleteMapNpcInfo( int nDefId )
{
	int nMapId = getRealMapId();
	MAPNPCINFOVECTOR& vectorNpc = m_allMapNpcMap[nMapId];
	MAPNPCINFOVECTOR::iterator iter = std::find( vectorNpc.begin(), vectorNpc.end(), nDefId );
	if ( iter != vectorNpc.end() )
	{
		vectorNpc.erase( iter );
		GameManager::getSingleton().SendUIEvent( "GE_UPDATE_NPC" );
	}
}

const tagMapActorInfo* GameMapShower::getNpcInfoById( int nMapId, int Id )
{
	std::map<int,MAPNPCINFOVECTOR>::iterator iter = m_allMapNpcMap.find( nMapId );
	if( iter == m_allMapNpcMap.end() )
	{
		return NULL;
	}

	MAPNPCINFOVECTOR& vectorNpc = m_allMapNpcMap[nMapId];
	MAPNPCINFOVECTOR::iterator Npciter = std::find( vectorNpc.begin(),vectorNpc.end(),Id );
	if( Npciter != vectorNpc.end() )
	{
		return &(*Npciter);
	}
	return NULL;
}

void GameMapShower::ClearPVPMapNPC( int mapid ) {
	std::map<int, MAPNPCINFOVECTOR>::iterator iter = m_allMapNpcMap.find( mapid );
	if ( iter == m_allMapNpcMap.end() ) {
		return;
	}

	MAPNPCINFOVECTOR& vectorNpc = m_allMapNpcMap[ mapid ];
	MainGamePlayer*	mainplayer = ActorManager::getSingleton().getMainPlayer();
	if( mainplayer == NULL && mainplayer->m_nCampIdx > 0 ) {
		return;
	}

	for ( MAPNPCINFOVECTOR::iterator it = vectorNpc.begin(); it != vectorNpc.end();  ) {
		if ( it->m_nSpanMapCamp > 0 && it->m_nSpanMapCamp != mainplayer->m_nCampIdx ) {
			it = vectorNpc.erase( it );
		} else {
			++it;
		}
	}
}

const tagMapActorInfo* GameMapShower::getNpcInfoByName( int nMapId, const char* pszName )
{
	assert( pszName != NULL );
	if ( pszName == NULL )
	{
		return NULL;
	}

	std::map<int,MAPNPCINFOVECTOR>::iterator iter = m_allMapNpcMap.find( nMapId );
	if( iter == m_allMapNpcMap.end() )
	{
		return NULL;
	}

	MAPNPCINFOVECTOR& vectorNpc = m_allMapNpcMap[nMapId];
	for( MAPNPCINFOVECTOR::iterator iter = vectorNpc.begin(); iter != vectorNpc.end(); ++iter )
	{
		if( stricmp( iter->m_szName, pszName ) == 0 )
		{
			return &(*iter);
		}
	}
	return NULL;
}

int	GameMapShower::getMapSceneAreaCount( int nMapId )
{
	const MAPSCENEAREAVECTOR& areaVector = m_allMapSceneAreaMap[nMapId];
	return	(int)areaVector.size();
}

const tagSceneAreaDef_ToLua& GameMapShower::getMapSceneAreaInfo( int nMapId, int nIndex )
{
	assert( nIndex < (int)m_allMapSceneAreaMap[nMapId].size() && "越界访问!" );
	const MAPSCENEAREAVECTOR& areaVector = m_allMapSceneAreaMap[nMapId];
	return	m_allMapSceneAreaMap[nMapId][nIndex];
}

int GameMapShower::getMapSceneAreainfocounts( int nMapId)
{
	const MAPSCENEAREAVECTOR& areaVector = m_allMapSceneAreaMap[nMapId];
	return m_allMapSceneAreaMap[nMapId].size();
}

void GameMapShower::mergeMapNpc()
{
	// 该表只需加载一次，无需重复加载
	if ( !m_allMapNpcMap.empty() )
	{
		return;
	}

	for (std::vector<MAPNPC>::iterator iter = m_mapNpcVector.begin(), end = m_mapNpcVector.end(); iter != end; ++iter )
	{
		tagMapActorInfo npcInfo;
		npcInfo.m_nId		= iter->MonsterID;	
		npcInfo.m_nMapId	= iter->MapID;
		npcInfo.m_nRealX	= iter->Pos.X;
		npcInfo.m_nRealY	= iter->Pos.Y;
		npcInfo.m_bShow		= !( iter->Unshown );
		//npcInfo.m_nSpanMapCamp	= iter->SpanMapCamp;
		strncpy( npcInfo.m_szName, DBDataManager::getSingleton().getMonsterDef(npcInfo.m_nId).Name,NAME_LEN );
		strncpy( npcInfo.m_szMapDesc, iter->MapDesc,NAME_LEN );
		getQuestList( npcInfo.m_nId );
		
		m_allMapNpcMap[iter->MapID].push_back( npcInfo );
	}
	//m_mapNpcVector.clear();
}

bool operator< ( const tagSceneAreaDef_ToLua& lhs, const tagSceneAreaDef_ToLua& rhs )
{
	return getMonsterLevel( lhs.MonsterID ) < getMonsterLevel( rhs.MonsterID );
}

void GameMapShower::mergeMapSceneArea()
{
	static_assert( sizeof( tagSceneAreaDef_ToLua ) == sizeof( tagSceneAreaDef ) );
	// 该表只需加载一次，无需重复加载
	if ( !m_allMapSceneAreaMap.empty() )
	{
		return;
	}

	for ( std::vector<tagSceneAreaDef>::iterator sceneIter = m_mapAllSceneArea.begin(), end = m_mapAllSceneArea.end(); 
			sceneIter != end; ++sceneIter )
	{
		tagSceneAreaDef_ToLua	sceneAreaInfo = *( (tagSceneAreaDef_ToLua*)&( *sceneIter ) );
		m_allMapSceneAreaMap[sceneIter->MapID].push_back( sceneAreaInfo );;
	}

	// 根据怪物等级排序
	/*for ( std::map<int,MAPSCENEAREAVECTOR>::iterator iter = m_allMapSceneAreaMap.begin(),
			end = m_allMapSceneAreaMap.end(); iter != end; ++iter )
	{
		std::sort( iter->second.begin(), iter->second.end() );
	}*/
}

void GameMapShower::setRealMapId(int mapId)	
{ 
	m_nRealMapId = mapId;
	if ( m_nRealMapId >= MIN_COPY_ID )
	{
		m_nLineNo = DEFAULT_LINE_NO;
		return;
	}
	
	const tagMapDef& mapDef = DBDataManager::getSingleton().getMapDef( m_nRealMapId );
	if ( &mapDef == NULL || !isMultiLineMap( m_nRealMapId ) )
	{
		m_nLineNo = DEFAULT_LINE_NO;
		return;
	}
	
	int nLineMapID = m_nRealMapId - ( m_nRealMapId / 1000 ) * 1000;
	nLineMapID = nLineMapID - ( nLineMapID / 100 ) * 100;
	m_nLineNo = nLineMapID + 1;
}

const tagMapInfo* GameMapShower::getMapInfoByName( const char* pszName )
{
	assert( pszName != NULL && "传递进来的指针非法!" );

	if ( pszName == NULL )
	{
		return NULL;
	}

	for ( std::map<int, MAPINFOVECTOR>::iterator map_iter = m_mapInfoMap.begin(), map_end = m_mapInfoMap.end(); map_iter != map_end;
		++map_iter )
	{
		for ( std::vector<tagMapInfo>::iterator vec_iter = map_iter->second.begin(), vec_end = map_iter->second.end(); 
			vec_iter != vec_end; ++vec_iter )
		{
			char* pdest = strstr( vec_iter->m_szMapName, pszName );
			int result = (int)(pdest - vec_iter->m_szMapName + 1);
			if ( strstr( vec_iter->m_szMapName, pszName ) != NULL )
			{
				if( (int)(strstr( vec_iter->m_szMapName, pszName ) - vec_iter->m_szMapName) == 0 )
				{
					return	&( *vec_iter );
				}
			}
		}
	}

	return NULL;
}

const tagMapInfo& GameMapShower::getMapInfoByIndex( int nMapArea, int nIndex )
{
	return m_mapInfoMap[nMapArea][nIndex];
}

const tagMapInfo* GameMapShower::getMapInfoByID( int mapId )
{
	assert( mapId != 0 && "传入的地图ID非法!" );
	if ( mapId == 0 )
	{
		return NULL;
	}

	for ( MAPINFOMAP_ITER map_iter = m_mapInfoMap.begin(), map_end = m_mapInfoMap.end(); map_iter != map_end;
		++map_iter )
	{
		for ( std::vector<tagMapInfo>::iterator vec_iter = map_iter->second.begin(), vec_end = map_iter->second.end(); 
			vec_iter != vec_end; ++vec_iter )
		{
			if ( mapId == vec_iter->m_nMapID )
			{
				return	&( *vec_iter );
			}
		}
	}

	return NULL;

}

int GameMapShower::getMapNum()
{
	int nMapNum = 0;

	for ( MAPINFOMAP_ITER map_iter = m_mapInfoMap.begin(), map_end = m_mapInfoMap.end(); map_iter != map_end;
		++map_iter )
	{
		nMapNum += (int)map_iter->second.size();
	}

	return nMapNum;
}

tagMapInfo* GameMapShower::getMapInfoByIndex( int nIndex )
{
	assert( nIndex < (int)m_mapVector.size() );
	int nMapNum = 0;

	for ( MAPINFOMAP_ITER map_iter = m_mapInfoMap.begin(), map_end = m_mapInfoMap.end(); map_iter != map_end;
		++map_iter )
	{
		for ( std::vector<tagMapInfo>::iterator vec_iter = map_iter->second.begin(), vec_end = map_iter->second.end(); 
			vec_iter != vec_end; ++vec_iter )
		{
			if ( nMapNum == nIndex )
			{
				return	&(*vec_iter);
			}
			++nMapNum;
		}
	}

	return NULL;
}

Pair& GameMapShower::getMapSize( int nMapID )
{
	std::map<int, Pair>::iterator iter = m_mapSize.find( nMapID );
	if ( iter == m_mapSize.end() )
	{
		calculateMapSize( nMapID );
	}
	
	return m_mapSize[nMapID];
}

MultiMapAutoFind& GameMapShower::getMultiMapFindControl()
{
	return m_multiMapAutoFind;
}

void GameMapShower::initMapInfo()
{
	tagMapInfo	mapInfo;
	for ( std::vector<tagMapDef>::iterator iter = m_mapVector.begin(), end = m_mapVector.end(); iter != end; ++iter )
	{
		mapInfo.m_nMapID		= iter->MapID;
		mapInfo.m_nMapArea		= iter->MapArea;
		mapInfo.m_nReferMapID	= iter->ReferMapID;
		mapInfo.DontList		= iter->DontList;
		strncpy( mapInfo.m_szMapName, iter->Name, strlen( iter->Name ) + 1 );
		m_mapInfoMap[iter->MapArea].push_back( mapInfo );
	}
}

void GameMapShower::update( float dtick )
{
	if (GamePlayManager::getSingleton().isMapLoadOK())
	{
		m_multiMapAutoFind.update();
	}
}

bool GameMapShower::addMapNpc( int nId )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( nId );
	assert( pActor!=NULL );

	std::map<int,MAPNPCINFOVECTOR>::iterator iter = m_allMapNpcMap.find( getRealMapId() );
	if( iter != m_allMapNpcMap.end() )
	{
		MAPNPCINFOVECTOR::iterator npciter = iter->second.begin();
		for(; npciter != iter->second.end(); npciter++  )
		{
			if( (*npciter).m_nId == pActor->getResId() )
			{
				return false;
			}
		}
	}
	
	tagMapActorInfo npcInfo;
	npcInfo.m_nId		= pActor->getResId();	
	npcInfo.m_nMapId	= getRealMapId();
	npcInfo.m_nRealX	= pActor->GetPosition().x/10;
	npcInfo.m_nRealY	= pActor->GetPosition().z/10;
	npcInfo.m_bShow		= true;
	npcInfo.m_nSpanMapCamp = pActor->m_nCampIdx;
	strncpy( npcInfo.m_szName, DBDataManager::getSingleton().getMonsterDef(npcInfo.m_nId).Name,NAME_LEN );
	npcInfo.m_szMapDesc[0] = '\0';
	getQuestList( npcInfo.m_nId );
	m_allMapNpcMap[getRealMapId()].push_back( npcInfo );
	return true;
}

void GameMapShower::deleteMapNpc( int nId )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( nId );
	assert( pActor!=NULL );

	std::map<int,MAPNPCINFOVECTOR>::iterator iter = m_allMapNpcMap.find( getRealMapId() );
	if( iter != m_allMapNpcMap.end() )
	{
		MAPNPCINFOVECTOR::iterator npciter = iter->second.begin();
		for(; npciter != iter->second.end(); npciter++  )
		{
			if( (*npciter).m_nId == pActor->getResId() )
			{
				iter->second.erase(npciter);
				break;
			}
		}
	}
}

void GameMapShower::CheckAutoPathFind()
{
	const tagPassMapPath& lastFindDest = m_pMaxMapContal->getLastFindDest();
	if( lastFindDest.m_bInCopy )
	{
		MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if( pMainPlayer )
		{
			pMainPlayer->m_LastCmdMoveTick = 0;
		}
		m_multiMapAutoFind.stopMultiFind();
		if( lastFindDest.m_bNpcTran )
		{
			m_pMaxMapContal->linkMoveto( lastFindDest.m_nMapId, lastFindDest.m_Dest.m_nNpcResId );
		}else{
			m_pMaxMapContal->linkMoveto( lastFindDest.m_nMapId, lastFindDest.m_Dest.m_Pos.X, lastFindDest.m_Dest.m_Pos.Y );
		}
	}
}

void GameMapControl::SetMapSize( int mapID )
{
	Pair& size = GameManager::getSingleton().getGameMapShower()->getMapSize( mapID );
	m_nMapWidth = size.first;
	m_nMapHeight = size.second;
}

const tagMapOreDef_ToLua* GameMapShower::getMapOreDef( int nId )
{
	static_assert(sizeof(tagMapOreDef_ToLua) == sizeof(tagMapOreDef));
	return (const tagMapOreDef_ToLua*) DBDataManager::getSingleton().getMapOreDef( nId );
}