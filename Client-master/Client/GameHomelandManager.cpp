#include "GameHomelandManager.h"
#include "GameDBData.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameMsgPrint.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameMap.h"
#include "GameStateManager.h"
#include "GameUIMsg.h"
#include "../UILib/ui_framemgr.h"

GameHomelandManager::GameHomelandManager() {
	m_GardenPet = NULL;
	m_FurnitureItem = NULL;
	m_plantItem = NULL;
	m_PetIndex	= 123;
	m_nPetID	= 0;
	m_moveBound.x = m_moveBound.z = 0;
	m_isPlanting = false;
	m_isfurnitureAlready = false;
	m_isMoving = false;
	m_furnitureMoster = NULL;
	memset( &m_HomeSvrPage, 0, sizeof( tagHomeSvrPage ) );
	INetworkManager::getSingleton().RegisterMsgHandler( HOME_SVR, this );
}

GameHomelandManager::~GameHomelandManager() {
	//SAFE_DELETE ( m_GardenPet );
};

void GameHomelandManager::setOwnerHome( const tagHome& home ) {
	memcpy( &m_OwnerHome, &home, sizeof( tagHome ) );
	const tagHomeLvlDef* homeLvlDef = DBDataManager::getSingleton().getHomeLvlDef( m_OwnerHome.Lvl );
	if ( !homeLvlDef ) {
		return;
	}
	memcpy( &m_houseScope, &homeLvlDef->HousePos, sizeof( HOMEPOS ) );
}

int GameHomelandManager::HandleNetMsg( int msgid, const tagCSPkgBody& msgbody ) {
	if( msgid != HOME_SVR ) {
		return 0;
	}
	switch( msgbody.HomeSvr.Cmd ) {
		case HOME_SVR_OP_PAGE:
			memcpy( &m_HomeSvrPage, &msgbody.HomeSvr.Data.Page, sizeof( tagHomeSvrPage ) );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_PAGE" );
			break;
		case HOME_SVR_OP_SINGLE:
			static_assert( sizeof( tagHomeSvrSingle_ToLua ) == sizeof( tagHomeSvrSingle ) );
			setOwnerHome( msgbody.HomeSvr.Data.Single.Home );
			UpdatePageInfo( msgbody.HomeSvr.Data.Single );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_SINGLE" );
			break;
		case HOME_SVR_OP_NAME_CHG:
			static_assert( sizeof( tagHomeSvrNameChg_ToLua ) == sizeof( tagHomeSvrNameChg ) );
			UpdatePageInfo( msgbody.HomeSvr.Data.Name );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_NAMECHG" );
			break;
		case HOME_SVR_OP_STAT_CHG:
			static_assert( sizeof( tagHomeSvrStatChg_ToLua ) == sizeof(tagHomeSvrStatChg) );
			UpdatePageInfo( msgbody.HomeSvr.Data.Stat );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_STATCHG" );
			break;
		case HOME_SVR_OP_LIUYAN_CHG:
			static_assert( sizeof( tagHomeSvrLiuyanChg_ToLua ) == sizeof( tagHomeSvrLiuyanChg ) );
			UpdatePageInfo( msgbody.HomeSvr.Data.Liuyan );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_LIUYANCHG" );
			break;
		case HOME_SVR_OP_SALE:
			static_assert( sizeof( tagHomeSvrSaleChg_ToLua ) == sizeof( tagHomeSvrSaleChg ) );
			UpdatePageInfo( msgbody.HomeSvr.Data.Sale );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_SALECHG" );
			break;
		case HOME_SVR_OP_BUY:
			static_assert( sizeof( tagHomeSvrBuy_ToLua ) == sizeof( tagHomeSvrBuy ) );
			UpdatePageInfo( msgbody.HomeSvr.Data.Buy );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_BUY" );
			break;
		case HOME_SVR_OP_DEL:
			static_assert( sizeof( tagHomeSvrDel_ToLua ) == sizeof( tagHomeSvrDel ) );
			UpdatePageInfo( msgbody.HomeSvr.Data.Del );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_DEL" );
			break;
		case HOME_SVR_OP_PET:
			memcpy( &m_OwnerHome.Info.Pet, &msgbody.HomeSvr.Data.Pet.Pet, sizeof( tagHomeSvrPet_ToLua ) );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_PET" );
			break;
		case HOME_SVR_OP_HOUSE_INFO:
			static_assert( sizeof( tagHomeSvrHouse_ToLua ) == sizeof( tagHomeSvrHouse ) );
			memcpy( &m_HomeSvrHouse, &msgbody.HomeSvr.Data.House.House, sizeof( HOMESVRHOUSE ) );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_HOUSE" );
			break;
		case HOME_SVR_OP_FARM:
			static_assert( sizeof( tagHomeSiol_ToLua ) == sizeof( tagHomeSiol ) );
			memcpy( &m_OwnerHome.Info.Siol, &msgbody.HomeSvr.Data.Farm.Farm, sizeof( tagHomeSiol ) );
			GameManager::getSingleton().SendUIEvent( "GE_HOMELAND_FARM" );
			break;
		default:
			break;
	}
	return 0;
}

void GameHomelandManager::requestHomePage( int nPage ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_PAGE;
	msgbody.HomeClt.Data.Page.Idx		= nPage;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeNew() {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_NEW;
	msgbody.HomeClt.Data.New.New	= 0;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeSetName( int nID, const char* szName ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_SET_NAME;
	msgbody.HomeClt.Data.SetName.Id = nID;
	strncpy( msgbody.HomeClt.Data.SetName.Name, szName, NAME_LEN );
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeSetStat( int nID, int nStat ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_SET_STAT;
	msgbody.HomeClt.Data.SetStat.Id	= nID;
	msgbody.HomeClt.Data.SetStat.Stat = nStat;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeEnter( int nID ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_ENTER;
	msgbody.HomeClt.Data.Enter.Id		= nID;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeMsgBoard( int nID, const char* szMsg ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_MSG_BOARD;
	msgbody.HomeClt.Data.Msg.Id			= nID;
	strncpy( msgbody.HomeClt.Data.Msg.Msg, szMsg, HOME_MAX_LIUYAN_TEXT );
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeDestroy( int nID ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_DESTORY;
	msgbody.HomeClt.Data.Destory.Id	=nID;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeSale( int nID, int nMoney ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_SALE;
	msgbody.HomeClt.Data.Sale.Id			=nID;
	msgbody.HomeClt.Data.Sale.Money = nMoney;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeBuy( int nID ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_BUY;
	msgbody.HomeClt.Data.Buy.Id			=nID;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomePet( int nType, int nGridIdx ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_PET;
	msgbody.HomeClt.Data.Pet.Type		= nType;
	msgbody.HomeClt.Data.Pet.GridIdx	= nGridIdx;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeFurniturePut( int nGridIdx, int nDir, int nX, int nY ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd					= HOME_CLT_OP_JIAJU_PUT;
	msgbody.HomeClt.Data.Put.GridIdx	= nGridIdx;
	msgbody.HomeClt.Data.Put.Dir			= nDir;
	msgbody.HomeClt.Data.Put.Pos.X		= nX;
	msgbody.HomeClt.Data.Put.Pos.Y		= nY;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomeFurniturePutAlready( const tagJiaJuAdjustInfo &oldInfo, const tagJiaJuAdjustInfo &newInfo ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd							= HOME_CLT_OP_JIAJU_ADJUST;
	msgbody.HomeClt.Data.JiaJuAdjust.Old	= oldInfo;
	msgbody.HomeClt.Data.JiaJuAdjust.New	= newInfo;
	SendGameServerMsg( HOME_CLT, msgbody );
}

void GameHomelandManager::requestHomePlant( int nIdx, int nGridIdx ) {
	tagCSPkgBody msgbody;
	msgbody.HomeClt.Cmd								= HOME_CLT_OP_ZHONGZHI;
	msgbody.HomeClt.Data.Zhongzhi.Idx			= nIdx;
	msgbody.HomeClt.Data.Zhongzhi.GridIdx		= nGridIdx;
	SendGameServerMsg( HOME_CLT, msgbody );
}

const tagHomeLvlDef_ToLua* GameHomelandManager::getHomeLvlDef( int nLvl ) {
	static_assert( sizeof( tagHomeLvlDef_ToLua ) == sizeof( tagHomeLvlDef ) );
	return ( const tagHomeLvlDef_ToLua* )DBDataManager::getSingleton().getHomeLvlDef( nLvl );
}

const tagHomeMonLvlDef_ToLua* GameHomelandManager::getHomeMonLvlDef( int nLvl ) {
	static_assert( sizeof( tagHomeMonLvlDef_ToLua ) == sizeof( tagHomeMonLvlDef ) );
	return ( const tagHomeMonLvlDef_ToLua* )DBDataManager::getSingleton().getHomeMonLvlDef( nLvl );
}

const tagHomeFarmDef_ToLua* GameHomelandManager::getHomeFarmDef( int nLvl ) {
	static_assert( sizeof( tagHomeFarmDef_ToLua ) == sizeof( tagHomeFarmDef ) );
	return ( const tagHomeFarmDef_ToLua* )DBDataManager::getSingleton().getHomeFarmDef( nLvl );
}

const tagHomeMonSegDef_ToLua* GameHomelandManager::getHomeMonSegDef( int nLvl ) {
	static_assert( sizeof( tagHomeMonSegDef_ToLua ) == sizeof( tagHomeMonSegDef ) );
	return ( const tagHomeMonSegDef_ToLua* )DBDataManager::getSingleton().getHomeMonSegDef( nLvl );
}

const tagHomeSvrPage_ToLua* GameHomelandManager::getHomeSvrPage() {
	static_assert( sizeof( tagHomeSvrPage_ToLua ) == sizeof( tagHomeSvrPage ) );
	return ( const tagHomeSvrPage_ToLua* )&m_HomeSvrPage;
}

const tagHomeMon_ToLua*	GameHomelandManager::getHomePet() {
	static_assert( sizeof( tagHomeMon_ToLua ) == sizeof( tagHomeMon ) );
	return ( const tagHomeMon_ToLua* )&( m_OwnerHome.Info.Pet );
}

int GameHomelandManager::getHomePetID() {
	if ( m_GardenPet ) {
		return m_GardenPet->GetID();
	}
	return 0;
}

const tagHomeSvrHouse_ToLua* GameHomelandManager::getHomeSvrHouse() {
	static_assert( sizeof( tagHomeSvrHouse_ToLua ) == sizeof( tagHomeSvrHouse ) );
	return ( const tagHomeSvrHouse_ToLua* )&m_HomeSvrHouse;
}

const tagHomeSvrFarm_ToLua* GameHomelandManager::getHomeSvrFarm() {
	static_assert( sizeof( tagHomeSvrFarm_ToLua ) == sizeof( tagHomeSvrFarm ) );
	return ( const tagHomeSvrFarm_ToLua* )&m_HomeSvrFarm;
}

void GameHomelandManager::UpdatePageInfo( const HOMESVRNAMECHG& nameChg ) {
	for( size_t i = 0; i < m_HomeSvrPage.Num; ++i ) {
		HOME& home = m_HomeSvrPage.Homes[ i ];
		if( nameChg.ID == home.HomeID ) {
			strncpy( home.Info.Name, nameChg.Name, NAME_LEN );
			break;
		}
	}
}

void GameHomelandManager::UpdatePageInfo( const HOMESVRSINGLE& single ) {
	bool isFound( false );
	for( size_t i = 0; i < m_HomeSvrPage.Num; ++i ) {
		HOME& home = m_HomeSvrPage.Homes[ i ];
		if( single.Home.HomeID == home.HomeID ) {
			memcpy( &home, &single, sizeof( HOME ) );
			isFound = true;
			break;
		}
	}
	if( !isFound ) {
		if( m_HomeSvrPage.Num < HOME_PAGE_MAX ) {
			m_HomeSvrPage.Num++;
			m_HomeSvrPage.CountHome++;
			memcpy( &m_HomeSvrPage.Homes[ m_HomeSvrPage.Num - 1 ], &single.Home, sizeof( HOME ) );
		}
	}
}

void GameHomelandManager::UpdatePageInfo( const HOMESVRSTATCHG& stat ) {
	for( size_t i = 0; i < m_HomeSvrPage.Num; ++i ) {
		HOME& home = m_HomeSvrPage.Homes[ i ];
		if( stat.ID == home.HomeID ) {
			home.Info.Stat = stat.Stat;
			break;
		}
	}
}

void GameHomelandManager::UpdatePageInfo( const HOMESVRLIUYANCHG& message ) {
	for( size_t i = 0; i < m_HomeSvrPage.Num; ++i ) {
		HOME& home = m_HomeSvrPage.Homes[ i ];
		if( message.ID == home.HomeID ) {
			memcpy( &home.Info.Liuyan, &message.Liuyan, sizeof( HOMELIUYAN ) );
			break;
		}
	}
}

void GameHomelandManager::UpdatePageInfo( const HOMESVRSALECHG& sale ) {
	for( size_t i = 0; i < m_HomeSvrPage.Num; ++i ) {
		HOME& home = m_HomeSvrPage.Homes[ i ];
		if( sale.ID == home.HomeID ) {
			home.Info.Sale = sale.Sale;
			break;
		}
	}
}

void GameHomelandManager::UpdatePageInfo( const HOMESVRBUY& buy ) {
	for( size_t i = 0; i < m_HomeSvrPage.Num; ++i ) {
		HOME& home = m_HomeSvrPage.Homes[ i ];
		if( buy.ID == home.HomeID ) {
			strncpy( home.Info.Name, buy.Name, NAME_LEN );
			break;
		}
	}
}

void GameHomelandManager::UpdatePageInfo( const HOMESVRDEL& del ) {
	for( size_t i = 0; i < m_HomeSvrPage.Num; ++i ) {
		HOME& home = m_HomeSvrPage.Homes[ i ];
		if( del.ID == home.HomeID ) {
			memcpy( &m_HomeSvrPage.Homes[ i ], &m_HomeSvrPage.Homes[ --m_HomeSvrPage.Num], sizeof( HOME) );
			break;
		}
	}
	m_HomeSvrPage.CountHome = m_HomeSvrPage.CountHome > 0 ? m_HomeSvrPage.CountHome - 1 : 0;
}

void GameHomelandManager::displayHomelandPet( int x, int z ) {
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL ) {
		return;
	}

	const tagHomeLvlDef* homeLvlDef = DBDataManager::getSingleton().getHomeLvlDef( m_OwnerHome.Lvl );
	if ( !homeLvlDef ) {
		return;
	}
	if ( m_GardenPet ) {
		removeHomelandPet();
	}
	int petid =m_OwnerHome.Info.Pet.ID;
	if ( petid <= 0 ) {
		petid = homeLvlDef->GardenDefaultMonID;
	}
	if ( petid <= 0 ) {
		return;
	}
	const tagMonsterDef& petDef	= DBDataManager::getSingleton().getMonsterDef( petid );
	int nPetDyId			= pMainPlayer->GetID() + ( m_PetIndex + 1 ) * PET_ID_BASE;
	if  (m_GardenPet ) {
		m_GardenPet->OnLeaveMap();
		ActorManager::getSingleton().DestroyActor( m_GardenPet );
		m_GardenPet = NULL;
	}
	m_GardenPet	= ActorManager::getSingleton().CreateActor( nPetDyId, GAT_MONSTER, petDef.MonsterID );
	m_GardenPet->setName( petDef.Name );
	m_GardenPet->OnEnterMap( pMainPlayer->getCurMap() );
	m_GardenPet->setOwnerName( "" );
	m_GardenPet->setOwnerId( pMainPlayer->GetID() );
	m_GardenPet->setLv( m_OwnerHome.Info.Pet.Lvl );
	m_GardenPet->SetFaceTo( -90, true );

	m_GardenPet->setType( GAT_NPC );
	m_GardenPet->setSpeed( 300 );
	m_GardenPet->setRoleWId( *(tdr_ulonglong*)&nPetDyId );
	m_GardenPet->m_nPetIconId = petDef.IconID;
	m_GardenPet->m_nPetDefID = petDef.MonsterID;
	m_GardenPet->m_nBattePetFlag = 0;
	m_GardenPet->SetPosition( x, z );
	m_GardenPet->reSetHp( petDef.HP );
	m_GardenPet->setMaxHP( petDef.HP );

	m_GardenPet->m_ObjCamp = SKILL_TARGET_FRIENDPET;

	m_GardenPet->m_nPetStrengthenLevel	= 0;
	m_GardenPet->ChangeActState( GSACT_STAND );
	//m_GardenPet->playAction( 10200, NULL, 0, 0 );
	//m_GardenPet->playMotion( "9622_16" );
	m_GardenPet->playMotion( "252010_3", 252010);
	m_moveCenter.x = x;
	m_moveCenter.z = z;
	m_moveBound.x = 1500;
	m_moveBound.z = 1500;
	m_nPetID = nPetDyId;
}

void GameHomelandManager::setPetBound( int w, int h ) {
	m_moveBound.x = w;
	m_moveBound.z = h;
}

// 宠物移动
void GameHomelandManager::petMoveTo( int to_x, int to_z ) {
	GameActor* pActor = m_GardenPet;
	if ( !pActor || pActor->getSpeed() == 0 ) {
		return;
	}
	GameMap *curMap = pActor->getCurMap();// 判断宠物是否在当前地图
	if (curMap == NULL) {
		return;
	}
	int x = to_x;
	int y = to_z;
	const float maxr = 180;
	const float MY_PI = 3.1415926f;
	if ( !pActor->m_pMapMover->isReach() ) {
		Ogre::Point2D pos = pActor->m_pMapMover->m_CurPath.getPathPoint( pActor->m_pMapMover->m_CurPath.getNumSegment() );
		x = pos.x;
		y = pos.y;
	}
	GameMaskPath path;
	int cur_x, cur_z;
	pActor->m_pMapMover->GetCurPos( cur_x, cur_z );
	for ( int i = 0; i < 3; ++i ) {
		float r = pow( rand() / float( RAND_MAX ), 0.333333f ) * maxr;
		float angle = rand() / float( RAND_MAX ) * MY_PI * 2.0f;
		int offx = int(r * cos(angle));
		int offy = int(r * sin(angle));
		int tox = x + offx;
		int toy = y + offy;
		bool reach = true;
		if( curMap->getMaskMap()->lineOfSightTest( Ogre::Point2D( cur_x, cur_z ), Ogre::Point2D( tox, toy ), MASK_STOP|DYN_MASK_STOP ) ) {
			x = tox;
			y = toy;
			break;
		}
	}

	if( cur_x == x && cur_z == y ) {
		return;
	}
	path.reset(curMap->getMaskMap());
	path.addPathPoint( cur_x, cur_z );
	if ( !pActor->m_pMapMover->isReach() ) {
		path.addPathPoint(to_x, to_z);
		for ( size_t i = pActor->m_pMapMover->m_CurSeg + 1; i < pActor->m_pMapMover->m_CurPath.getNumPoint(); ++i ) {
			Ogre::Point2D pos = pActor->m_pMapMover->m_CurPath.getPathPoint( i );
			path.addPathPoint( pos.x, pos.y );
		}
	}
	path.addPathPoint( x, y );

	pActor->m_pMapMover->StartOnPath( &path );
	pActor->ChangeActState( GSACT_MOVE );
}

void GameHomelandManager::removeHomelandPet() {
	if ( m_GardenPet ) {
		//m_GardenPet->OnLeaveMap();
		//ActorManager::getSingleton().DestroyActor( m_GardenPet );
		m_GardenPet = NULL;
	}
}

void GameHomelandManager::openHomePetFrame() {
	GameManager::getSingleton().getScriptVM()->callString( "OpenHomelandPetFrame()");
}

void GameHomelandManager::closeHomePetFrame() {
	GameManager::getSingleton().getScriptVM()->callString( "CloseHomelandPetFrame()");
}

void GameHomelandManager::closeHomelandFrames() {
	GameManager::getSingleton().getScriptVM()->callString( "CloseHomelandFrames()");
}

void GameHomelandManager::putFurnitureConfirm() {
	GameManager::getSingleton().getScriptVM()->callString( "PutHomelandFurniture()");
}

int GameHomelandManager::getFurnitureID() {
	return m_furnitureID;
}

void GameHomelandManager::beginMoveMode() {
	while(ShowCursor(FALSE) >= 0);
	m_isMoving = true;
}

void GameHomelandManager::endMoveMode() {
	m_isMoving = false;
	while(ShowCursor(TRUE) < 0);
}

bool GameHomelandManager::isMovingMode() {
	return m_isMoving;
}

bool GameHomelandManager::beginFurnitureMode( Item* item ) {
	tagItemDef_ToLua* itemdef = item->getItemDef();
	if ( !itemdef ) {
		return false;
	}

	const tagMonsterDef& monsterdef = DBDataManager::getSingleton().getMonsterDef( itemdef->Result[0].ResultVal2 );

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainPlayer ) {
		return false;
	}

	int nid			= pMainPlayer->GetID() + ( m_PetIndex + 1 ) * PET_ID_BASE + 1;	
	GameActor*	monster = ActorManager::getSingleton().CreateActor( nid, GAT_MONSTER, monsterdef.MonsterID );
	m_furnitureMoster = monster;
	m_furnitureID = nid;
	int angle		= pMainPlayer->getAngle();
	float cosa		= Ogre::Cos( angle );
	float sina		= Ogre::Sin( angle );
	monster->setName( "" );
	monster->setStickyFrame( g_pFrameMgr->FindLayoutFrame( "FurnitureOpFrame" ) );
	monster->OnEnterMap( pMainPlayer->getCurMap() );
	monster->setOwnerName( "" );
	monster->setOwnerId( pMainPlayer->GetID() );

	monster->setType( GAT_MONSTER );
	monster->setSpeed( pMainPlayer->getSpeed() );
	monster->setRoleWId( *(tdr_ulonglong*)&nid );
	monster->m_nPetIconId = monsterdef.IconID;
	monster->m_nBattePetFlag = 0;
	monster->SetPosition( pMainPlayer->GetPosition().x * 0.1 + 100 * cosa, pMainPlayer->GetPosition().z * 0.1 - 100 * sina);
	monster->SetFaceTo( pMainPlayer->getAngle(), true );
	pMainPlayer->SetSelectTarget( monster->GetID() );
	//g_pFrameMgr->m_pCurCursor->HideCursor();
	//monster->reSetHp( 100 );
	//monster->setMaxHP( monsterdef.HP );

	monster->m_ObjCamp = SKILL_TARGET_FRIENDPET;

	//pMainPlayer->SetStickyMonster( monster );
	m_FurnitureItem = item;
}

void GameHomelandManager::beginFurnitureModeAlready( GameActor* pActor ) {
	if( !pActor ) {
		return;
	}
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer ) {
		return;
	}
	if( !isInSelfHome() ) {
		GameManager::getSingleton().getScriptVM()->callString( "ShowForbiddenTip()" );
		return;
	}
	if( m_furnitureMoster ) {
		if( pActor != m_furnitureMoster ) {
			GameManager::getSingleton().getScriptVM()->callString( "ShowFurnitureClickTip()" );
		}
		return;
	}
	m_furnitureMoster = pActor;
	//pMainplayer->SetStickyMonster( pActor );
	m_furnitureID = pActor->GetID();
	m_oldFurnitureInfo.MonID = pActor->GetID();
	m_oldFurnitureInfo.Dir = pActor->getAngle();
	m_oldFurnitureInfo.Pos.X = pActor->GetPosition().x * 0.1;
	m_oldFurnitureInfo.Pos.Y = pActor->GetPosition().z * 0.1;
	pActor->setSpeed( pMainplayer->getSpeed() );
	GameManager::getSingleton().getScriptVM()->callString( "ShowFurnitureFrame()" );
	pActor->setStickyFrame( g_pFrameMgr->FindLayoutFrame( "FurnitureOpFrame" ) );
	m_isfurnitureAlready = true;
}

void GameHomelandManager::endFurnitureModeAlready() {
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer ) {
		return;
	}
	GameActor* pMonster = m_furnitureMoster;//ActorManager::getSingleton().FindActor( m_furnitureID );
	if ( !pMonster ) {
		return;
	}
	tagJiaJuAdjustInfo newInfo;
	newInfo.MonID = pMonster->GetID();
	newInfo.Dir = pMonster->getAngle();
	newInfo.Pos.X = pMonster->GetPosition().x * 0.1;
	newInfo.Pos.Y = pMonster->GetPosition().z * 0.1;
	requestHomeFurniturePutAlready( m_oldFurnitureInfo, newInfo );
	pMonster->setSpeed( 0 );
	pMonster->setStickyFrame( NULL );
	//pMainplayer->SetStickyMonster( NULL );
	pMainplayer->SetSelectTarget( 0 );
	GameManager::getSingleton().getScriptVM()->callString( "HideFurnitureFrame()" );
	//pMonster->OnLeaveMap();
	//ActorManager::getSingleton().DestroyActor( pMonster );
	m_furnitureMoster = NULL;
	m_isfurnitureAlready = false;
}

void GameHomelandManager::selectFurniture() {
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	GameActor *pMonster = ActorManager::getSingleton().FindActor( m_furnitureID );
	if( pMainplayer && pMonster ) {
		pMainplayer->SetSelectTarget( m_furnitureID );
	}
}

bool GameHomelandManager::isFurnitureAlready() {
	return m_isfurnitureAlready;
}

bool GameHomelandManager::isFurnitureMode() {
	return m_FurnitureItem != NULL || m_isfurnitureAlready;
}

bool GameHomelandManager::endFurnitureMode() {
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer ) {
		return false;
	}
	GameActor* pMonster = ActorManager::getSingleton().FindActor( m_furnitureID );
	if ( !pMonster ) {
		return false;
	}
	if ( !m_FurnitureItem ) {
		return false;
	}
	tagItemDef_ToLua* itemdef = m_FurnitureItem->getItemDef();
	if ( !itemdef ) {
		return false;
	}
	const tagMonsterDef& monsterdef = DBDataManager::getSingleton().getMonsterDef( itemdef->Result[0].ResultVal2 );
	//pMainplayer->SetStickyMonster( NULL );
	pMonster->setName( "" );
	pMonster->setSubType( MON_SUB_TYPE_BUILD );
	pMainplayer->SetSelectTarget( 0 );
	requestHomeFurniturePut( m_FurnitureItem->getGridIdx(), pMonster->getAngle(), pMonster->GetPosition().x * 0.1, pMonster->GetPosition().z * 0.1 );
	GameManager::getSingleton().getScriptVM()->callString( "HideFurnitureFrame()" );
	pMonster->setStickyFrame( NULL );
	pMonster->OnLeaveMap();
	ActorManager::getSingleton().DestroyActor( pMonster );
	m_furnitureMoster = NULL;
	char buf[256];
	sprintf_s( buf, "UnLockItem( %d, %d, %d )", m_FurnitureItem->getListType(), m_FurnitureItem->getGridIdx(), m_FurnitureItem->getItemId() );
	GameManager::getSingleton().getScriptVM()->callString( buf );
	m_FurnitureItem = NULL;
}

void GameHomelandManager::cancelFurnitureMode() {
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer ) {
		return;
	}
	GameActor* pMonster = ActorManager::getSingleton().FindActor( m_furnitureID );
	if ( !pMonster ) {
		return;
	}
	if ( !m_FurnitureItem ) {
		return;
	}
	tagItemDef_ToLua* itemdef = m_FurnitureItem->getItemDef();
	if ( !itemdef ) {
		return;
	}
	//pMonster->getActState().SetState( GSACT_DEAD );
	//pMonster->setSubType( MON_SUB_TYPE_TMP );
	GameManager::getSingleton().getScriptVM()->callString( "HideFurnitureFrame()" );
	pMonster->setStickyFrame( NULL );
	pMonster->OnLeaveMap();
	ActorManager::getSingleton().DestroyActor( pMonster );
	m_furnitureMoster = NULL;
	char buf[256];
	sprintf_s( buf, "UnLockItem( %d, %d, %d )", m_FurnitureItem->getListType(), m_FurnitureItem->getGridIdx(), m_FurnitureItem->getItemId() );
	GameManager::getSingleton().getScriptVM()->callString( buf );
	//pMainplayer->SetStickyMonster( NULL );
	pMainplayer->SetSelectTarget( 0 );
	m_FurnitureItem = NULL;
}

void GameHomelandManager::cancelFurnitureModeAlready() {
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer ) {
		return;
	}
	GameActor* pMonster = ActorManager::getSingleton().FindActor( m_furnitureID );
	if ( !pMonster ) {
		return;
	}
	pMonster->SetPosition( m_oldFurnitureInfo.Pos.X, m_oldFurnitureInfo.Pos.Y );
	pMonster->SetFaceTo( m_oldFurnitureInfo.Dir, true );
	pMonster->setSpeed( 0 );
	pMonster->setStickyFrame( NULL );
	GameManager::getSingleton().getScriptVM()->callString( "HideFurnitureFrame()" );

	//pMainplayer->SetStickyMonster( NULL );
	m_furnitureMoster = NULL;
	pMainplayer->SetSelectTarget( 0 );
	//pMonster->OnLeaveMap();
	//ActorManager::getSingleton().DestroyActor( pMonster );
	m_isfurnitureAlready = false;
}

bool GameHomelandManager::isPetExist() {
	if ( !m_GardenPet || m_nPetID <= 0 ) {
		return false;
	}
	if( ActorManager::getSingleton().FindActor( m_nPetID ) == NULL ) {
		return false;
	}
	return true;
}

void GameHomelandManager::Update( unsigned int dtick ) {
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	static unsigned int elapsedTime(0);
	if ( GameStateManager::getGameState() != GameStateManager::STATE_INWORLD ) {
		m_GardenPet = NULL;
		return;
	}
	if ( !pMainplayer || pMainplayer->getMapId() != HOMELAND_MAP_ID ) {
		m_GardenPet = NULL;
		return;
	}
	if ( !isPetExist() ) {
		m_GardenPet = NULL;
		return;
	}
	if ( isFurnitureMode() ) {
		if ( !isInHouse() ) {
			if( m_isfurnitureAlready ) {
				cancelFurnitureModeAlready();
			} else {
				cancelFurnitureMode();
			}
			GameManager::getSingleton().getScriptVM()->callString( "ShowOutsideOfHouseTip()" );
		} else {
			GameManager::getSingleton().getScriptVM()->callString( "ShowFurnitureFrame()" );
		}
	}
	if( !m_GardenPet ) {
		return;
	}
	elapsedTime += dtick;
	if ( elapsedTime < 3000 ) {
		return;
	} else {
		elapsedTime = 0;
	}
	/*
	GameState& actState = m_GardenPet->getActState();
	if ( actState.GetState() == GSACT_MOVE ) {
		return;
	}
	enum
	{
		GSACT_STAND = 0,		// 站立
		GSACT_MOVE,				// 移动
		//GSACT_SIT,				// 坐下

		MAX_ACT_STATE_MAX
	};
	int state = rand() % MAX_ACT_STATE_MAX;
	if ( state == GSACT_MOVE )
	{
		if ( m_GardenPet->isMotionPlaying( "252010_3", 252010 ) ) {
			m_GardenPet->stopMotion( "252010_3" );
		}
		int newX = m_GardenPet->GetPosition().x * 0.1 + m_moveCenter.x * rand();
		int newZ = m_GardenPet->GetPosition().z * 0.1 + m_moveCenter.z * rand();
		int tmp( 0 );
		if ( newX < ( tmp = m_moveCenter.x - m_moveBound.x * 0.5 ) ) {
			newX = tmp + m_moveBound.x * 0.5;
		} else if ( newX > ( tmp = m_moveCenter.x + m_moveBound.x * 0.5) ) {
			newX = tmp - m_moveBound.x * 0.5;
		}
		if ( newZ < ( tmp = m_moveCenter.z - m_moveBound.z * 0.5 ) ) {
			newZ = tmp + m_moveBound.z * 0.5;
		} else if ( newZ > ( tmp = m_moveCenter.z + m_moveBound.z * 0.5) ) {
			newZ = tmp - m_moveBound.z * 0.5;
		}

		petMoveTo( newX, newZ );
	} else {
		m_GardenPet->playMotion( "252010_3", 252010 );
		m_GardenPet->ChangeActState( state, true );
	}
	*/
}

void GameHomelandManager::playPetAction( int id ) {
	if( m_GardenPet ) {
		if( m_GardenPet->hasAnim( id ) ) {
			m_GardenPet->playAction( id );
		}
	}
}

bool GameHomelandManager::isInHome() {
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer ) {
		return false;
	}
	return pMainplayer->getMapId() == HOMELAND_MAP_ID;
}

bool GameHomelandManager::isInSelfHome() {
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer ) {
		return false;
	}
	return pMainplayer->getMapId() == HOMELAND_MAP_ID && pMainplayer->getRoleWId() == m_OwnerHome.RoleID;
}

int GameHomelandManager::getFarmSoilIndex( GameActor* pActor ) {
	if ( !pActor ) {
		return -1;
	}
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer || pMainplayer->getMapId() != HOMELAND_MAP_ID ) {
		return -1;
	}
	const tagHomeLvlDef* homeLvlDef = DBDataManager::getSingleton().getHomeLvlDef( m_OwnerHome.Lvl );
	if ( !homeLvlDef ) {
		return -1;
	}
	const int xpos = pActor->GetPosition().x * 0.1;
	const int zpos = pActor->GetPosition().z * 0.1;
	for ( int i = 0; i < FARM_MAX_SOIL; ++i ) {
		const RESPOS& tp = homeLvlDef->Soil[i].Pos;
		if ( tp.X == xpos && tp.Y == zpos ) {
			return i;
		}
	}
	return -1;
}

bool GameHomelandManager::isInPlantMode() {
	return m_isPlanting;
}

bool GameHomelandManager::beginPlantMode( Item* item ) {
	if ( !item ) {
		return false;
	}
	m_plantItem = item;
	m_isPlanting = true;
	return m_isPlanting;
}

bool GameHomelandManager::planting(GameActor* pTarget, int nIndex /* = - 1 */) {
	if ( !pTarget ) {
		return false;
	}
	if ( !m_plantItem ) {
		m_isPlanting = false;
		return false;
	}
	if ( nIndex < 0 || nIndex >= FARM_MAX_SOIL ) {
		nIndex = getFarmSoilIndex( pTarget );
		if ( nIndex < 0 ) {
			return false;
		}
	}
	const tagHomeFarmOne& soil = m_OwnerHome.Info.Siol.One[ nIndex ];
	if ( soil.Stat <= 0 ) {
		GameManager::getSingleton().getScriptVM()->callString( "ShowPlantUnreclaimed()" );
		return false;
	}
	if ( soil.MonID > 0 ) {
		GameManager::getSingleton().getScriptVM()->callString( "ShowPlantOverride()" );
		return false;
	}
	requestHomePlant( nIndex, m_plantItem->getGridIdx() );
	endPlantMode( pTarget, nIndex );
}

bool GameHomelandManager::endPlantMode( GameActor* pTarget, int nIndex ) {
	if ( !pTarget ) {
		return false;
	}
	if ( !m_plantItem ) {
		m_isPlanting = false;
		return false;
	}
	if ( nIndex < 0 || nIndex >= FARM_MAX_SOIL ) {
		nIndex = getFarmSoilIndex( pTarget );
		if ( nIndex < 0 ) {
			return false;
		}
	}
	m_isPlanting = false;
	GameUIMsg::getSingleton().getUIGV()->DragFrom = DRAG_NONE;
	GameManager::getSingleton().getScriptVM()->callString( "EndPlantMode()" );
	char buf[256];
	sprintf_s( buf, "CancelPlantMode( %d, %d, %d )", m_plantItem->getListType(), m_plantItem->getGridIdx(), m_plantItem->getItemId() );
	GameManager::getSingleton().getScriptVM()->callString( buf );
	m_plantItem = NULL;
}

void GameHomelandManager::cancelPlantMode() {
	if ( !isInPlantMode() || !m_plantItem ) {
		m_isPlanting = false;
		m_plantItem = NULL;
		return;
	}

	char buf[256];
	sprintf_s( buf, "CancelPlantMode( %d, %d, %d )", m_plantItem->getListType(), m_plantItem->getGridIdx(), m_plantItem->getItemId() );
	GameManager::getSingleton().getScriptVM()->callString( buf );
	m_plantItem = NULL;
	m_isPlanting = false;
}

void GameHomelandManager::resetHomelandStatus() {
	if ( isInPlantMode() ) {
		cancelPlantMode();
	}
	if ( isFurnitureMode() ) {
		if( m_isfurnitureAlready ) {
			cancelFurnitureModeAlready();
		} else {
			cancelFurnitureMode();
		}
	}
}

bool GameHomelandManager::isInHouse() {
	static const int xmin = m_houseScope.OPos.X - m_houseScope.Xlen * 0.5, xmax = m_houseScope.OPos.X + m_houseScope.Xlen * 0.5;
	static const int zmin = m_houseScope.OPos.Y - m_houseScope.Ylen * 0.5, zmax = m_houseScope.OPos.Y + m_houseScope.Ylen * 0.5;

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainplayer ) {
		return false;
	}
	int xpos;
	int zpos;
	if( m_furnitureMoster ) {
		xpos = m_furnitureMoster->GetPosition().x * 0.1;
		zpos = m_furnitureMoster->GetPosition().z * 0.1;
		if ( xpos < xmin || xpos > xmax ) {
			return false;
		}
		if ( zpos < zmin || xpos > zmax ) {
			return false;
		}
	}
	xpos = pMainplayer->GetPosition().x * 0.1;
	zpos = pMainplayer->GetPosition().z * 0.1;
	if ( xpos < xmin || xpos > xmax ) {
		return false;
	}
	if ( zpos < zmin || xpos > zmax ) {
		return false;
	}
	return true;
}

void GameHomelandManager::stepFurnitureAngle( int deltaAngle ) {
	MainGamePlayer *mainplayer = ActorManager::getSingleton().getMainPlayer();
	if( !mainplayer ) {
		return;
	}
	if( !m_furnitureMoster ) {
		return;
	}

	int currAngle = m_furnitureMoster->getAngle();
	currAngle += deltaAngle;
	m_furnitureMoster->SetFaceTo( currAngle, true );
}

void GameHomelandManager::stepFurnitureLocation( int dx, int dy ) {
	if( !m_furnitureMoster ) {
		return;
	}
	int angle = m_furnitureMoster->getAngle();
	//int nDis = (int) ( Ogre::Sqrt( dx * dx + dy * dy ) );
	int resultX =	m_furnitureMoster->GetPosition().x * 0.1; //* Ogre::Cos(angle));
	int resultY =	m_furnitureMoster->GetPosition().z * 0.1; //* Ogre::Sin(angle));
	//float coef = Ogre::Max( Ogre::Abs( Ogre::Cos( angle ) ), Ogre::Abs( Ogre::Sin( angle ) ) );
	resultX += dx;
	resultY -= dy;
	m_furnitureMoster->SetPosition( resultX, resultY );
}