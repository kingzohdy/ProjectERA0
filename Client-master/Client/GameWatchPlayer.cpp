#include "GameWatchPlayer.h"
#include "GameManager.h"
#include "GameActor.h"
#include "GameActorManager.h"
#include "Equip_Common.h"

GameWatchPlayer::GameWatchPlayer(void):m_nSawActorId( 0 )
{
	INetworkManager::getSingleton().RegisterMsgHandler( WATCH_PLAYER_SVR, this );
	m_pWatchPlayer = NULL ;
}

GameWatchPlayer::~GameWatchPlayer(void)
{
	SAFE_DELETE(m_pWatchPlayer);
}

// 向服务器发出观察请求
void GameWatchPlayer::sendWatchRequest( int nMemID )
{
	tagCSPkgBody msgbody;
	msgbody.WatchPlayerClt.PlayerMemID	= nMemID;
	SendGameServerMsg( WATCH_PLAYER_CLT, msgbody ); 
}

int GameWatchPlayer::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == WATCH_PLAYER_SVR )
	{
		const CSWATCHPLAYERSVR& watchPlayerData = msgbody.WatchPlayerSvr;
		m_nSawActorId = watchPlayerData.PlayerMemID;
		m_nSawActorWorldId = watchPlayerData.WorldID;
				
		memset( m_SawArm, 0, sizeof(Item)*EQUIP_POS_END );

		int nItemArmNum	= watchPlayerData.RoleWear.ArmNum;
		for( int i = 0; i < nItemArmNum; i++ )
		{
			int nGridIdx = watchPlayerData.RoleWear.RoleArms[i].GridIdx;
			m_SawArm[nGridIdx] = watchPlayerData.RoleWear.RoleArms[i];
		}
		setAvatarSelectSuit( watchPlayerData.SelectFashion );
		setFashionSuitSelect( watchPlayerData.SelectFashion );
// 		GameActor* pActor = ActorManager::getSingleton().FindActor( m_nSawActorId );
// 		if( pActor == NULL )
		{
			if( m_pWatchPlayer == NULL || m_pWatchPlayer->GetID() != m_nSawActorId )
			{
				SAFE_DELETE(m_pWatchPlayer);
				int resid = getPlayerDisplayId( watchPlayerData.RoleGender, watchPlayerData.RoleCareer );
				m_pWatchPlayer = new GameActor;
				m_pWatchPlayer->LoadRes(resid);
				m_pWatchPlayer->SetID(m_nSawActorId);
			}
			m_pWatchPlayer->setLv(watchPlayerData.RoleLevel);
			m_pWatchPlayer->m_Gender = watchPlayerData.RoleGender;
			m_pWatchPlayer->m_Race		= watchPlayerData.RoleCareer;
			m_pWatchPlayer->m_Head		= watchPlayerData.RoleHead;
			m_pWatchPlayer->m_Hair		= watchPlayerData.RoleHair;
			m_pWatchPlayer->m_Face		= watchPlayerData.RoleFace;
			m_pWatchPlayer->setName( watchPlayerData.RoleName );
			m_pWatchPlayer->setType( GAT_PLAYER );
			m_pWatchPlayer->setClanWID( watchPlayerData.ClanWID );
			//战勋信息
			m_pWatchPlayer->setDaoBaClanWid(watchPlayerData.DaoBaInfo.ClanWID);
			m_pWatchPlayer->setDaoBaEnd(watchPlayerData.DaoBaInfo.End);
			m_pWatchPlayer->setDaoBaName(watchPlayerData.DaoBaInfo.Name);
			
			for( int i = 0; i < watchPlayerData.RoleAttrVal.AttrNum; i++ )
			{
				const CSATTRVAL& oneAttr = watchPlayerData.RoleAttrVal.AttrList[i];
				m_pWatchPlayer->setAttriValue( oneAttr.AttrID, oneAttr.Value );
			}

			for( int i = 0; i < watchPlayerData.RoleBaseAttrVal.AttrNum; i++ )
			{
				const CSATTRVAL& oneAttr = watchPlayerData.RoleBaseAttrVal.AttrList[i];
				m_pWatchPlayer->setBaseAttriValue( oneAttr.AttrID, oneAttr.Value );
			}

			const CSATTRIDVAL* p = m_pWatchPlayer->getAttriValule( ATTR_ID_MSPD );
			if( p != NULL )
			{
				m_pWatchPlayer->setSpeed( p->MSpd );
			}
			
			m_pWatchPlayer->m_nShowFlag = watchPlayerData.RoleMiscFlag;

			m_SparPkg = watchPlayerData.SparPkg;

			m_nGeniusPoint = watchPlayerData.Genius;
			memcpy( &m_GeniusInfo, &watchPlayerData.GeniusInfo, sizeof(tagGeniusInfo) );

			GameManager::getSingleton().SendUIEvent("GE_WATCHPLAYER_FARWATCH");
		}
// 		else
// 		{
// 			for( int i = 0; i < watchPlayerData.RoleAttrVal.AttrNum; i++ )
// 			{
// 				const CSATTRVAL& oneAttr = watchPlayerData.RoleAttrVal.AttrList[i];
// 				pActor->setAttriValue( oneAttr.AttrID, oneAttr.Value );
// 			}
// 
// 			for( int i = 0; i < watchPlayerData.RoleBaseAttrVal.AttrNum; i++ )
// 			{
// 				const CSATTRVAL& oneAttr = watchPlayerData.RoleBaseAttrVal.AttrList[i];
// 				pActor->setBaseAttriValue( oneAttr.AttrID, oneAttr.Value );
// 			}
// 
// 			const CSATTRIDVAL* p = pActor->getAttriValule( ATTR_ID_MSPD );
// 			if( p != NULL )
// 			{
// 				pActor->setSpeed( p->MSpd );
// 			}
// 
// 			pActor->m_nShowFlag = watchPlayerData.RoleMiscFlag;
// 
// 			GameManager::getSingleton().SendUIEvent("GE_WATCHPLAYER_WATCH");
// 		}
	}
	return 0;	
}

// 获取被观察玩家的装备ITEM
Item& GameWatchPlayer::getItem( int GridIndex )
{
	return m_SawArm[GridIndex];
}

// 组织模型装备显示数据
void GameWatchPlayer::setAvatarAttrib()
{	
	GameActor* pActor = m_pWatchPlayer;
	if( pActor == NULL )
	{
		pActor = ActorManager::getSingleton().FindActor( m_nSawActorId );
	}
	if( pActor != NULL )
	{
		clearEquipIdList();
		for( int i = 0; i < EQUIP_POS_END; i++ )
		{
			if ( m_SawArm[i].getItemId() != 0 && !m_SawArm[i].isValidEnd() )
			{
				setArmEquipIdList( i, m_SawArm[i].getItemId(),m_SawArm[i].getArmPefectStarNum() );
			}
		}
	/*	if( !pActor->isShowFashion() )
		{
			setArmEquipIdList( EQUIP_POS_FASHION_BODY,0,0 );
			for( int i= EQUIP_POS_FASHION_HEAD ; i < EQUIP_POS_END ;i++ )
			{
				setArmEquipIdList( i,0,0 );
			}
		//	setArmEquipIdList( EQUIP_POS_FASHION_HEAD,0,0 );
		//	setArmEquipIdList( EQUIP_POS_FASHION_HAIR,0,0 );
		//	setArmEquipIdList( EQUIP_POS_FASHION_BACK,0,0 );
		//	setArmEquipIdList( EQUIP_POS_FASHION_SHOES,0,0 );
		//	setArmEquipIdList( EQUIP_POS_FASHION_HAND,0,0 );
		//	setArmEquipIdList( EQUIP_POS_FASHION_LEG,0,0 );
		}else{
			::setAvatarAttrib( POS_ID_FASHION_HAIR,1 );
		}  */
	
		int backIndex = EQUIP_POS_FASHION_BACK + m_nFashionSelect * 7;
		if ( pActor->isShowBack() )
		{
			if ( m_SawArm[backIndex].getItemId() != 0 && !m_SawArm[backIndex].isValidEnd() )
			{
				setArmEquipIdList( backIndex,m_SawArm[backIndex].getItemId(),m_SawArm[backIndex].getArmPefectStarNum() );
			}	
		}
		else
		{
			setArmEquipIdList( EQUIP_POS_FASHION_BACK,0,0 );
			setArmEquipIdList( EQUIP_POS_FASHION_BACK2,0,0 );
			setArmEquipIdList( EQUIP_POS_FASHION_BACK3,0,0 );
		}
		g_AvatarChange(); 
		::setAvatarAttrib(0, pActor->getRace() );
		::setAvatarAttrib(1, pActor->getGender() - 1 );
		::setAvatarAttrib( POS_ID_FACE, pActor->getFace() );
		::setAvatarAttrib( POS_ID_HEAD, pActor->getHead() );
 		::setAvatarAttrib( POS_ID_HAIR, pActor->getHair() + 1 );
		g_ConfigFashionFair();
	}
}

// 是否是被观察玩家身上的装备
bool GameWatchPlayer::IsSawArmItem( int nArmId )
{
	for( int i = 0; i < EQUIP_POS_END; i++ )
	{
		if( nArmId != 0 && nArmId == m_SawArm[i].getItemId() )
		{
			return true;
		}
	}
	return false;
}

GameActor* GameWatchPlayer::getWatchPlayer()
{
	return m_pWatchPlayer;
}

tagSparPkg_ToLua* GameWatchPlayer::getWatchSparPkg()
{
	static_assert( sizeof(tagSparPkg) == sizeof(tagSparPkg_ToLua) );
	return (tagSparPkg_ToLua*)&m_SparPkg;
}

Item* GameWatchPlayer::getWatchSparItem( int nIdx )
{
	static Item item;
	item.releaseItem();
	if( nIdx < m_SparPkg.Num )
	{
		item.setItemDate( GRID_ITEM_ITEM, &m_SparPkg.RoleItems[nIdx] );
	}
	return &item;
}
const char* GameWatchPlayer::getWatchPlayerWorldName()
{
	return LoginManager::getSingleton().getWorldNameById( m_nSawActorWorldId );
}

int GameWatchPlayer::getAttrLv( int nIdx, int nAttrId )
{
	if( nIdx < m_GeniusInfo.Num )
	{
		for( int i = 0 ; i < m_GeniusInfo.One[nIdx].Num; ++i )
		{
			if( m_GeniusInfo.One[nIdx].SubOne[i].AttrID == nAttrId )
			{
				return m_GeniusInfo.One[nIdx].SubOne[i].Lvl;
			}
		}
		return m_GeniusInfo.One[nIdx].Type;
	}
	return 0;
}

int GameWatchPlayer::getWatchPlayerGeniusTypePoint( int nType )
{
	assert( nType < MAX_GENIUS_TYPE );
	int nVal = 0;
	for ( int i = 0; i < m_GeniusInfo.One[nType].Num; ++i )
	{
		nVal += m_GeniusInfo.One[nType].SubOne[i].Lvl;
	}
	return nVal;
}

void GameWatchPlayer::setFashionSuitSelect( int nIndex )
{
	m_nFashionSelect = nIndex;
}