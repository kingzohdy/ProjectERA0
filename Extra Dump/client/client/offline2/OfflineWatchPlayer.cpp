#include "OfflineWatchPlayer.h"
#include "GameActor.h"
#include "Equip_Common.h"
#include "OfflineManager.h"
#include "RelationManager.h"
using namespace std;

OfflineWatchPlayer::OfflineWatchPlayer(void):m_nSawActorId( 0 )
{
	INetworkManager::getSingleton().RegisterMsgHandler( WATCH_PLAYER_SVR, this );
	m_pWatchPlayer = NULL ;
}

OfflineWatchPlayer::~OfflineWatchPlayer(void)
{
	SAFE_DELETE(m_pWatchPlayer);
}

// 向服务器发出观察请求
void OfflineWatchPlayer::sendWatchRequest( const char* szName )
{
	tagCSPkgBody msgbody;
	strcpy(msgbody.WatchFriendClt.FriendName,szName);
	if ( RelationManager::getSingleton().getOnlineinfo(szName) != MUD_ONLINE )
	{
		addmessage("对方不处于引力空间状态，无法观看其装备!",0);
		return;
	}
	SendGameServerMsg( WATCH_FRIEND_CLT , msgbody ); 
}

int OfflineWatchPlayer::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if ( msgid == WATCH_PLAYER_SVR )
	{
		const CSWATCHPLAYERSVR& watchPlayerData = msgbody.WatchPlayerSvr;
		m_nSawActorId = watchPlayerData.PlayerMemID;

		memset( m_SawArm, 0, sizeof(Item)*EQUIP_POS_END );

		int nItemArmNum	= watchPlayerData.RoleWear.ArmNum;
		for( int i = 0; i < nItemArmNum; i++ )
		{
			int nGridIdx = watchPlayerData.RoleWear.RoleArms[i].GridIdx;
			m_SawArm[nGridIdx] = watchPlayerData.RoleWear.RoleArms[i];
		}


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
		OfflineManager::getSingleton().SendUIEvent("GE_WATCHPLAYER_WATCH");
	}
	return 0;	
}

// 获取被观察玩家的装备ITEM
Item& OfflineWatchPlayer::getItem( int GridIndex )
{
	return m_SawArm[GridIndex];
}

// 组织模型装备显示数据
void OfflineWatchPlayer::setAvatarAttrib()
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_nSawActorId );
	if( pActor == NULL )
	{
		pActor = m_pWatchPlayer;
	}
	if( pActor != NULL )
	{
		for ( int i = 0; i < EQUIP_POS_END; i++ )
		{
			if( EQUIP_POS_FASHION_HAIR == i )
			{
				int nHair = DBDataManager::getSingleton().getItemDisplayInfo(g_ArmEquipId_List[i]).hairstyle;
				if( m_SawArm[EQUIP_POS_FASHION_HEAD].getItemId() != 0 )
				{
					nHair = 0;
				}
				::setAvatarAttrib( EquipPos_2_PosId[i], nHair );
			}else{
				::setAvatarAttrib( EquipPos_2_PosId[i], m_SawArm[i].getItemId() );
			}			
		}

		::setAvatarAttrib( 0, pActor->getRace() );
		::setAvatarAttrib( 1, pActor->getGender()-1 );
		::setAvatarAttrib( POS_ID_FACE, pActor->getFace() );
		::setAvatarAttrib( POS_ID_HEAD, pActor->getHead() );
		::setAvatarAttrib( POS_ID_HAIR, pActor->getHair() + 1 );
		g_ConfigFashionFair();
	}
}

// 是否是被观察玩家身上的装备
bool OfflineWatchPlayer::IsSawArmItem( int nArmId )
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

GameActor* OfflineWatchPlayer::getWatchPlayer()
{
	return m_pWatchPlayer;
}