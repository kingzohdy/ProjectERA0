#include "GameExchange.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameManager.h"
#include "GameFriendManager.h"
#include "GameStateAction.h"
#include "OgreScriptLuaVM.h"
#include "SystemSettingConfig.h"
#include "GameActor_Trump.h"
#include <Windows.h>
//#include "OgreStringUtil.h"
//#include <iostream>
//using namespace std;
using namespace Ogre;

GameExchange::GameExchange(void) : m_nSrcMoney(0), m_nDesMoney(0), m_bSrcLockExchange(false), m_bDesLockExchange(false),
									m_bConfirmExchange( false )
									
{
	INetworkManager::getSingleton().RegisterMsgHandler( EXCHG_SVR, this );
	//memset( &m_desPetList,  0,	sizeof(tagRolePet_ToLua)*EXCHG_MAX_PET_NUM );
}

GameExchange::~GameExchange(void)
{
}

int GameExchange::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == EXCHG_SVR )
	{
		const EXCHGENTIY& exchange_entiy = msgbody.ExchgSvr.ExchgEntiy;
		
		switch( msgbody.ExchgSvr.type )
		{
		case INVITE:
			{
				SystemSettingConfig& systemConfig = GameManager::getSingleton().getUserSystemConfig();
				if ( systemConfig.getGameOpt( GAME_OPT_REFUSE_TRADE_REQ ) )
				{
					// 若设置拒绝交易邀请，直接回复拒绝
					rejectExchange( msgbody.ExchgSvr.RoleName, INITIATIVE_REJECT );
				}
				else
				{
					// 收到交易邀请
					char szScript[256];
					sprintf(szScript,"GetExchageInvite( \"%s\")", msgbody.ExchgSvr.RoleName );
					//GameManager::getSingleton().SendUIEvent( "GE_EXCG_INVITE" );
					GameManager::getSingleton().getScriptVM()->callString( szScript );
					GameManager::getSingleton().FlashWindow();
				}
			}
			break;
		case REJECT:
			{
				// 拒绝交易邀请
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_REJECT" );
			}	
			break;
		case CANCEL:
			{
				// 取消交易
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_CANCEL" );
				endExchange();
			}
			break;
		case LOCK:
			{
				memset( &m_desPetList,  0,	sizeof( tagRolePet_ToLua ) * EXCHG_MAX_PET_NUM );

				for( int i = 0; i < EXCHG_MAX_PET_NUM; i++ )
				{
					for( int j = 0; j < exchange_entiy.PetNum; j++ )
					{
						int nIndex = exchange_entiy.PetEntiy[j].ExchgGridIdx;
						if( nIndex == i )
						{
							static_assert( sizeof(tagRolePet_ToLua) == sizeof(tagRolePet) );
							m_desPetList[i] = *( (tagRolePet_ToLua*)&exchange_entiy.PetEntiy[j].RolePet );
							break;
						}
					}
				}
				
				memset( &m_desItemList,  0,	sizeof( Item ) * EXCHG_MAX_ITEM_NUM );
				for( int i = 0; i < EXCHG_MAX_ITEM_NUM; i++ )
				{
					for( int j = 0; j < exchange_entiy.ItemNum; j++ )
					{
						int nIndex = exchange_entiy.ItemEntiy[j].ExchgGridIdx;
						if( nIndex == i )
						{
							if( exchange_entiy.ItemEntiy[j].RoleGrid.Type == GRID_ITEM_ITEM )
							{
								m_desItemList[i] = exchange_entiy.ItemEntiy[j].RoleGrid.GridData.RoleItem;
							}else{
								m_desItemList[i] = exchange_entiy.ItemEntiy[j].RoleGrid.GridData.RoleArm;
							}
							break;
						}
					}
				}
				
				m_nDesMoney			= exchange_entiy.Money;
				m_bDesLockExchange  = true;
				// 锁定交易
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_LOCK" );
			}
			break;
		case DEAL:
			{
				// 对方确认交易
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_DEAL" );
			}	
			break;
		case START:
			{
				// 开始交易
				m_strExchangeName = msgbody.ExchgSvr.RoleName;
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_START" );
				// 把交易方加为我的临时好友
				GameFriendManager::getSingleton().addTmpFriend( msgbody.ExchgSvr.RoleName );
			}
			break;
		case END:
			{
				// 结束交易
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_END" );
				endExchange();
			}
			break;

		case CANCEL_LOCK:
			{
				// 取消锁定交易状态
				memset( &m_desPetList,  0,	sizeof(tagRolePet_ToLua)*EXCHG_MAX_PET_NUM );
				memset( &m_desItemList,	0,	sizeof(Item)*EXCHG_MAX_ITEM_NUM );
				m_nDesMoney			= 0;
				m_bDesLockExchange	= false;
				m_bSrcLockExchange	= false;
				m_bConfirmExchange	= false;
				GameManager::getSingleton().SendUIEvent( "GE_CANCEL_LOCK" );
			}
			break;

		default:
			break;
		}
	}
	return 0;
}

void GameExchange::update(unsigned int dtick)
{
	GameManager::getSingleton().getScriptVM()->callString( "UpdateInviteExchange()" );
	if ( m_strExchangeName.empty() )
	{
		return;
	}

	GameActor* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}

	int nActState = pMainPlayer->getActState().GetState();
	// 若主角处于跑动状态，才要进行判断是否要中断交易
	if ( nActState != GSACT_MOVE && nActState != GSACT_FLY && nActState != GSACT_AUTOFLY )
	{
		return;
	}

	GameActor* pactor = ActorManager::getSingleton().findActorByName( m_strExchangeName.c_str() );
	if ( pactor == NULL )
	{
		cancelExchange( INITIATIVE_REJECT );
		GameManager::getSingleton().SendUIEvent( "GE_EXCG_BREAK" );
		//GameMsgPrint::getSingleton().showMidTip( MID_TRADE_DISTANCE_LONG );
		return;
	}

	GameXYZ pos  = pMainPlayer->GetPosition();
	pos.y		 = 0;

	GameXYZ pos2	= pactor->GetPosition();
	pos2.y			= 0;

	int nDis = (int)(pos2.subtract( pos ).length() / 100);
	if ( nDis > CLIENT_MAX_TRADE_DIST )
	{
		cancelExchange( INITIATIVE_REJECT );
		GameManager::getSingleton().SendUIEvent( "GE_EXCG_BREAK" );
		//GameMsgPrint::getSingleton().showMidTip( MID_TRADE_DISTANCE_LONG );
		return;
	}
}

void GameExchange::test( const char* pszContent )
{
	//const wchar_t* pszConvertedText = StringUtil::Gb2312ToUnicode(pszContent);
	//MessageBoxW( NULL, pszConvertedText, L"标题", MB_OK );
}

void GameExchange::helpTest( const char* pszContent )
{
	int num = 10;
	++num;
}

// 请求交易
void GameExchange::inviteExchange( const char* szName )
{
	if ( !canExchange( szName ) )
	{
		return;
	}
	
	tagCSPkgBody msgbody;
	msgbody.ExchgClt.type = INVITE;
	strncpy( msgbody.ExchgClt.RoleName, szName, strlen(szName)+1 );
	memset( &msgbody.ExchgClt.ExchgData, 0, sizeof(EXCHGDATA) );
	SendGameServerMsg( EXCHG_CLT, msgbody );	
}

// 接受邀请
void GameExchange::acceptExchange( const char* pszName )
{
	if ( !canExchange( pszName ) )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.ExchgClt.type = ACCEPT;
	strncpy( msgbody.ExchgClt.RoleName, pszName, strlen( pszName ) + 1 );
	memset( &msgbody.ExchgClt.ExchgData, 0, sizeof(EXCHGDATA) );
	SendGameServerMsg( EXCHG_CLT, msgbody );
}

// 拒绝邀请
void GameExchange::rejectExchange( const char* pszName, int nRejectType )
{
	tagCSPkgBody msgbody;
	msgbody.ExchgClt.type	= REJECT;
	msgbody.ExchgClt.Reason = nRejectType;
	strncpy( msgbody.ExchgClt.RoleName, pszName, strlen( pszName ) + 1 );
	memset( &msgbody.ExchgClt.ExchgData, 0, sizeof(EXCHGDATA) );
	SendGameServerMsg( EXCHG_CLT, msgbody );	
	m_strExchangeName.clear();
}

// 取消交易
void GameExchange::cancelExchange( int nRejectType )
{
	tagCSPkgBody msgbody;
	msgbody.ExchgClt.type	= CANCEL;
	msgbody.ExchgClt.Reason = nRejectType;
	strncpy( msgbody.ExchgClt.RoleName, m_strExchangeName.c_str(), m_strExchangeName.length() + 1 );
	memset( &msgbody.ExchgClt.ExchgData, 0, sizeof(EXCHGDATA) );
	SendGameServerMsg( EXCHG_CLT, msgbody );	

	GameManager::getSingleton().SendUIEvent( "GE_EXCG_CANCEL" );
	endExchange();
}

// 交易锁定
void GameExchange::lockExchange()
{
	tagCSPkgBody msgbody;
	msgbody.ExchgClt.type			 = LOCK;
	msgbody.ExchgClt.ExchgData.Money = m_nSrcMoney;
	strncpy( msgbody.ExchgClt.RoleName, m_strExchangeName.c_str(), m_strExchangeName.length() + 1 );

	int nItemNum = 0;
	for( int i = 0; i < EXCHG_MAX_ITEM_NUM; i++ )
	{
		if( m_srcItemList[i].getItemId() != 0 )
		{
			msgbody.ExchgClt.ExchgData.ItemData[nItemNum].DefID			= m_srcItemList[i].getItemId();
			msgbody.ExchgClt.ExchgData.ItemData[nItemNum].WID			= m_srcItemList[i].getItemWId();
			msgbody.ExchgClt.ExchgData.ItemData[nItemNum].ListType		= m_srcItemList[i].getListType();
			msgbody.ExchgClt.ExchgData.ItemData[nItemNum].GridIdx		= m_srcItemList[i].getGridIdx();
			msgbody.ExchgClt.ExchgData.ItemData[nItemNum].Num			= m_srcItemList[i].getNum();
			msgbody.ExchgClt.ExchgData.ItemData[nItemNum].ExchgGridIdx	= i;	
			nItemNum++;
		}
	}
	msgbody.ExchgClt.ExchgData.ItemNum	= nItemNum;
	
	int nPetNum = 0;
	for( int i = 0; i < EXCHG_MAX_PET_NUM; i++ )
	{
		if ( m_srcPetList[i].isValid() )
		{
			tagRolePet_ToLua* pPetInfo = getExchangePetInfo( 0, i );
			msgbody.ExchgClt.ExchgData.PetData[nPetNum].DefID		 = pPetInfo->PetID;
			msgbody.ExchgClt.ExchgData.PetData[nPetNum].WID			 = *((tdr_ulonglong*)&(pPetInfo->WID));
			msgbody.ExchgClt.ExchgData.PetData[nPetNum].ListType	 = m_srcPetList[i].m_nListType;
			msgbody.ExchgClt.ExchgData.PetData[nPetNum].GridIdx		 = m_srcPetList[i].m_nGridIdx;
			msgbody.ExchgClt.ExchgData.PetData[nPetNum].ExchgGridIdx = i;
			nPetNum++;
		}
	}
	msgbody.ExchgClt.ExchgData.PetNum = nPetNum;
	
	m_bSrcLockExchange = true;
	SendGameServerMsg( EXCHG_CLT, msgbody );	
}

// 交易确认
void GameExchange::dealExchange()
{
	if ( m_bConfirmExchange )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_CANNOT_REPEAT_CONFIRM_TRADE_ );
		return;
	}
	assert( m_bSrcLockExchange && m_bDesLockExchange );

	tagCSPkgBody msgbody;
	msgbody.ExchgClt.type = DEAL;
	strncpy( msgbody.ExchgClt.RoleName, m_strExchangeName.c_str(), m_strExchangeName.length() + 1 );
	memset( &msgbody.ExchgClt.ExchgData, 0, sizeof(EXCHGDATA) );
	SendGameServerMsg( EXCHG_CLT, msgbody );

	m_bConfirmExchange = true;
}

// 取消锁定交易
void GameExchange::cancelLock()
{
	assert( m_bSrcLockExchange );
	m_bSrcLockExchange = false;
	tagCSPkgBody msgbody;
	msgbody.ExchgClt.type = CANCEL_LOCK;
	strncpy( msgbody.ExchgClt.RoleName, m_strExchangeName.c_str(), m_strExchangeName.length() + 1 );
	memset( &msgbody.ExchgClt.ExchgData, 0, sizeof( EXCHGDATA ) );
	SendGameServerMsg( EXCHG_CLT, msgbody );

	m_bConfirmExchange = false;
}

// 设置交易的金钱数目
void GameExchange::addExchangeMoney( unsigned int nMoney )
{
	m_nSrcMoney = nMoney;
}

// 添加一件交易物品
// 参数( 交易栏的位置，目标容器，格子 )
void GameExchange::addExchangeItem( int nIndex, int nListType, int nGrid, int nNum )
{
	assert( nIndex < EXCHG_MAX_ITEM_NUM && nIndex >= 0 );
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( isInExchangeItem( nListType, nGrid ) )
	{
		return;
	}

	m_srcItemList[nIndex] = pMainPlayer->getContainer().getItem( nListType, nGrid );
	m_srcItemList[nIndex].setNum( nNum );
}

// 清除交易物品
void GameExchange::delExchangeItem( int nIndex )
{
	assert( nIndex < EXCHG_MAX_ITEM_NUM && nIndex >= 0 );
	m_srcItemList[nIndex].releaseItem();
}

// 添加一个交易宠物
// 参数( 交易栏的位置, 目标容器(7仙子, 8侍卫), 所在宠物栏格子 ) 
void GameExchange::addExchangePet( int nIndex, int nListType, int nGrid, int nHeightID, int nLowID )
{
	assert( nIndex < EXCHG_MAX_PET_NUM && nIndex >= 0 );
	
	if ( isInExchangePet( nListType, nGrid ) )
	{
		return;
	}
	
	tagSrcExchangePet& addSelfPet	= m_srcPetList[nIndex];
	addSelfPet.m_nListType			= nListType;
	addSelfPet.m_nGridIdx			= nGrid;
	addSelfPet.m_nHeightID			= nHeightID;
	addSelfPet.m_nLowID				= nLowID;
}

// 清除一个交易宠物
void GameExchange::delExchangePet( int nIndex )
{
	assert( nIndex < EXCHG_MAX_PET_NUM && nIndex >= 0 );
	tagSrcExchangePet& delSelfPetInfo = m_srcPetList[nIndex];
	delSelfPetInfo.reset();
}

// 获取交易物品( nType 0为自身1为交易方 )
Item& GameExchange::getExchangItem( int nType, int nIndex )
{	
	assert( nIndex < EXCHG_MAX_ITEM_NUM && nIndex >= 0 );
	if( nType == 0 )
	{
		return m_srcItemList[nIndex];
	}else{
		return m_desItemList[nIndex];
	}	
}

void GameExchange::synchronizeSelfPet()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GamePet* pPetPackage		= pMainPlayer->getPet();
	for ( int i = 0; i < EXCHG_MAX_PET_NUM; ++i )
	{
		tagSrcExchangePet& oneSrcPetInfo = m_srcPetList[i];
		if ( !oneSrcPetInfo.isValid() )
		{
			continue;
		}

		tagRolePet_ToLua* pPetInfo = pPetPackage->getPetInfo( oneSrcPetInfo.m_nHeightID, oneSrcPetInfo.m_nLowID );
		if ( pPetInfo == NULL )
		{
			oneSrcPetInfo.reset();
		}
	}
}

// 获取交易宠物信息( nType 0为自身1为交易方, nIndex为交易栏上的index )
// 注意：如果取到的PetID为0,表示这个栏位没有宠物就不用弹开宠物资质查看面板
tagRolePet_ToLua* GameExchange::getExchangePetInfo( int nType, int nIndex )
{
	if( nType == 0 )
	{
		GamePet* pPet = ActorManager::getSingleton().getMainPlayer()->getPet();
		if ( !m_srcPetList[nIndex].isValid() )
		{
			return NULL;
		}
		
		int nListType = 0;
		m_srcPetList[nIndex].m_nListType == LIST_TYPE_SHI ? nListType = 1 : nListType = 0;
		return pPet->getPetInfo( m_srcPetList[nIndex].m_nGridIdx );
	}else{
		return &m_desPetList[nIndex];
	}

	return NULL;
}

// 是否是正在交易中的物品
bool GameExchange::isInExchangeItem( int nListType, int nGrid )
{
	for( int i = 0; i < EXCHG_MAX_ITEM_NUM; i++ )
	{
		if( m_srcItemList[i].getItemId() != 0 
			&& m_srcItemList[i].getListType() == nListType && m_srcItemList[i].getGridIdx() == nGrid )
		{
			return true;
		}
	}

	return false;
}

bool GameExchange::canExchange( const char* pszName )
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	
// 	GameTrump* pTrump = pMainplayer->getTrump();
// 	if ( pTrump->isPre() )
// 	{
// 		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_SELF_TRUMP_PRE );
// 		return false;
// 	}

	// 战斗状态不做处理
	/*if ( pMainplayer->getActState().getInState( GSACT_IN_FIGHT ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_SELF_FIGHT );
		return false;
	}
	*/
	
	/*if ( pMainplayer->isInMorph() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_SELF_MORPH );
		return false;
	}*/

	if ( pMainplayer->getActState().GetState( ) == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_SELF_STALL );
		return false;
	}

	if ( pMainplayer->getActState().GetState( ) == GSACT_DEAD )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_SELF_DEAD );
		return false;
	}

	if ( pMainplayer->getActState().GetState( ) == GSACT_AUTOFLY || pMainplayer->getActState().GetState( ) == GSACT_FLY )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_SELF_FLY );
		return false;
	}

	GameActor* pActor = ActorManager::getSingleton().findActorByName( pszName );
	if ( pActor == NULL )
	{
		return false;
	}
	//战斗状态不做处理
	/*if ( pActor->getActState().getInState( GSACT_IN_FIGHT ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_ACTOR_FIGHT );
		return false;
	}*/

	if ( pActor->getActState().GetState( ) == GSACT_DEAD )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_ACTOR_DEAD );
		return false;
	}

	if ( pActor->getActState().GetState( ) == GSACT_STALL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_ACTOR_STALL );
		return false;
	}

	if ( pActor->getActState().GetState( ) == GSACT_AUTOFLY || pActor->getActState().GetState( ) == GSACT_FLY )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_ACTOR_FLY );
		return false;
	}

	// 再判断交易对象通过主角的关系
	//if ( GameFriendManager::getSingleton().getPlayerRelation( pszName ) == CLIENT_RELATION_TYPE_XIONG_SHOU )
	//{
	//	GameMsgPrint::getSingleton().showMidTip( MID_TRADE_RELATION_XIONGSHOU );
	//	return false;
	//}

	/*if ( GameFriendManager::getSingleton().getPlayerRelation( pszName ) == CLIENT_RELATION_TYPE_SU_DI )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRADE_RELATION_SUDI );
		return false;
	}*/

	//// 再判断交易对象通过主角的关系
	//if ( GameFriendManager::getSingleton().getPlayerRelation( pszName ) == CLIENT_RELATION_TYPE_BLACK )
	//{
	//	GameMsgPrint::getSingleton().showMidTip( MID_TRADE_RELATION_BLACK );
	//	return false;
	//}

	return true;
}

bool GameExchange::isInExchangePet( int nListType, int nGrid )
{
	for ( int i = 0; i < EXCHG_MAX_PET_NUM; ++i )
	{
		if ( m_srcPetList[i].isValid() && m_srcPetList[i].m_nListType == nListType && 
				m_srcPetList[i].m_nGridIdx == nGrid )
		{
			return true;
		}
	}

	return false;
}

bool GameExchange::isInExchangePetByWID( int nHeightID, int nLowID )
{
	for ( int i = 0; i < EXCHG_MAX_PET_NUM; ++i )
	{
		tagSrcExchangePet& oneSelfPet = m_srcPetList[i];
		if ( !oneSelfPet.isValid() )
		{
			continue;
		}

		if ( oneSelfPet.m_nHeightID == nHeightID && oneSelfPet.m_nLowID == nLowID )
		{
			return true;
		}
	}

	return false;
}

bool GameExchange::isSrcItemlistFull()
{
	for ( int i = 0; i < EXCHG_MAX_ITEM_NUM; ++i )
	{
		if ( m_srcItemList[i].getItemId() == 0 )
		{
			return false;
		}
	}

	return true;
}

bool GameExchange::isSrcPetlistFull()
{
	for ( int i = 0; i < EXCHG_MAX_PET_NUM; ++i )
	{
		if ( !m_srcPetList[i].isValid() )
		{
			return false;
		}
	}

	return true;
}

bool GameExchange::isInExchange()
{
	return !m_strExchangeName.empty();
}

bool GameExchange::isConfirmExchage()
{
	return m_bConfirmExchange;
}

// 结束交易
void GameExchange::endExchange()
{
	memset( &m_srcPetList,  -1,	sizeof(tagSrcExchangePet)*EXCHG_MAX_PET_NUM );
    memset( &m_desPetList,  0,	sizeof(tagRolePet_ToLua)*EXCHG_MAX_PET_NUM );
	memset( &m_srcItemList, 0,  sizeof(Item)*EXCHG_MAX_ITEM_NUM );
	memset( &m_desItemList, 0,	sizeof(Item)*EXCHG_MAX_ITEM_NUM );
	m_nSrcMoney			= 0;
	m_nDesMoney			= 0;
	m_bSrcLockExchange	= false;
	m_bDesLockExchange	= false;
	m_strExchangeName.clear();
	m_bConfirmExchange = false;
}
