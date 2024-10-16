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
					// �����þܾ��������룬ֱ�ӻظ��ܾ�
					rejectExchange( msgbody.ExchgSvr.RoleName, INITIATIVE_REJECT );
				}
				else
				{
					// �յ���������
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
				// �ܾ���������
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_REJECT" );
			}	
			break;
		case CANCEL:
			{
				// ȡ������
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
				// ��������
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_LOCK" );
			}
			break;
		case DEAL:
			{
				// �Է�ȷ�Ͻ���
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_DEAL" );
			}	
			break;
		case START:
			{
				// ��ʼ����
				m_strExchangeName = msgbody.ExchgSvr.RoleName;
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_START" );
				// �ѽ��׷���Ϊ�ҵ���ʱ����
				GameFriendManager::getSingleton().addTmpFriend( msgbody.ExchgSvr.RoleName );
			}
			break;
		case END:
			{
				// ��������
				GameManager::getSingleton().SendUIEvent( "GE_EXCG_END" );
				endExchange();
			}
			break;

		case CANCEL_LOCK:
			{
				// ȡ����������״̬
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
	// �����Ǵ����ܶ�״̬����Ҫ�����ж��Ƿ�Ҫ�жϽ���
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
	//MessageBoxW( NULL, pszConvertedText, L"����", MB_OK );
}

void GameExchange::helpTest( const char* pszContent )
{
	int num = 10;
	++num;
}

// ������
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

// ��������
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

// �ܾ�����
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

// ȡ������
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

// ��������
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

// ����ȷ��
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

// ȡ����������
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

// ���ý��׵Ľ�Ǯ��Ŀ
void GameExchange::addExchangeMoney( unsigned int nMoney )
{
	m_nSrcMoney = nMoney;
}

// ���һ��������Ʒ
// ����( ��������λ�ã�Ŀ������������ )
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

// ���������Ʒ
void GameExchange::delExchangeItem( int nIndex )
{
	assert( nIndex < EXCHG_MAX_ITEM_NUM && nIndex >= 0 );
	m_srcItemList[nIndex].releaseItem();
}

// ���һ�����׳���
// ����( ��������λ��, Ŀ������(7����, 8����), ���ڳ��������� ) 
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

// ���һ�����׳���
void GameExchange::delExchangePet( int nIndex )
{
	assert( nIndex < EXCHG_MAX_PET_NUM && nIndex >= 0 );
	tagSrcExchangePet& delSelfPetInfo = m_srcPetList[nIndex];
	delSelfPetInfo.reset();
}

// ��ȡ������Ʒ( nType 0Ϊ����1Ϊ���׷� )
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

// ��ȡ���׳�����Ϣ( nType 0Ϊ����1Ϊ���׷�, nIndexΪ�������ϵ�index )
// ע�⣺���ȡ����PetIDΪ0,��ʾ�����λû�г���Ͳ��õ����������ʲ鿴���
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

// �Ƿ������ڽ����е���Ʒ
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

	// ս��״̬��������
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
	//ս��״̬��������
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

	// ���жϽ��׶���ͨ�����ǵĹ�ϵ
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

	//// ���жϽ��׶���ͨ�����ǵĹ�ϵ
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

// ��������
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
