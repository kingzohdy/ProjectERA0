#include "GameEngineTypes.h"
#include "GameStallManager.h"
#include "NetMessage.h"
#include "GameMainPlayer.h"
#include "GameActorManager.h"
#include "Container.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include "GameActor_Pet.h"
#include "../UILib/ui_framemgr.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameMap.h"
#include "GameMaxMap.h"
#include <Windows.h>
#include <limits.h>
#include "GameNpc.h"

const int SEE_DIS = 500;
unsigned int getMaxUint()
{
	return UINT_MAX;;
}
GameStallManager::GameStallManager(void)
{
	INetworkManager::getSingleton().RegisterMsgHandler( STALL_SVR, this );
	resetSeeStallInfo();
	memset( &m_LastSelfStallItem, 0, sizeof(tagStallData_ToLua) );
}

GameStallManager::~GameStallManager(void)
{
}

bool GameStallManager::isSellGoodsMoneyTooMuch()
{
	tdr_ulonglong uGoodsMoney = 0;
	int nIndex = 0;
	for( int i = 0; i < STALL_MAX_ITEM; i++ )
	{
		Item& oneClientStallItem = m_SelfStallItem.m_ItemData[i];
		if( oneClientStallItem.getItemId() == 0 )
		{
			continue;
		}
		
		uGoodsMoney += oneClientStallItem.getStallSellPrice() * oneClientStallItem.getNum();
	}

	for( int i = 0; i < STALL_MAX_PET; i++ )
	{
		const tagStallPetEntity_ToLua& oneClientPetData = m_SelfStallItem.m_PetData[i];
		if( oneClientPetData.m_nDefID == 0 )
		{
			continue;
		}
		
		uGoodsMoney += oneClientPetData.m_uPrice;
	}

	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();
	uGoodsMoney += ( tdr_ulonglong )pMainplayer->getMoney();
	if ( uGoodsMoney > ( tdr_ulonglong )MAX_MONEY )
	{
		return true;
	}

	return false;
}

// 开始摆摊
void GameStallManager::stall_Start( const char* szStallName )
{
	// 若主角处于摆摊状态, 则不能再开始摆摊
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( !canStartStall() )
	{
		return;
	}
	
	// 若角色处于死亡状态，则不可摆摊
	// 若角色处于战斗状态，则不可摆摊
	tagCSPkgBody msgbody;
	msgbody.StallClt.Type				= CLT_STALL_START;
	STALLDATA& stallStartData		= msgbody.StallClt.StallCltData.StallStartClt.CSStallData;
	stallStartData.ItemNum	= m_SelfStallItem.m_nItemNum;
	int nIndex = 0;
	for( int i = 0; i < STALL_MAX_ITEM; i++ )
	{
		Item& oneClientStallItem = m_SelfStallItem.m_ItemData[i];
		if( oneClientStallItem.getItemId() == 0 )
		{
			continue;
		}

		STALLITEM& oneSvrStallItem		= stallStartData.ItemData[nIndex];
		oneSvrStallItem.DefID			= oneClientStallItem.getItemId();
		oneSvrStallItem.GridIdx			= oneClientStallItem.getGridIdx();
		oneSvrStallItem.ListType		= oneClientStallItem.getListType();
		oneSvrStallItem.Num				= oneClientStallItem.getNum();
		oneSvrStallItem.Price			= oneClientStallItem.getStallSellPrice();
		oneSvrStallItem.WID				= oneClientStallItem.getItemWId();
		oneSvrStallItem.StallGridIdx	= i;
		nIndex++;
	}
	stallStartData.PetNum = m_SelfStallItem.m_nPetNum;
	nIndex = 0;
   
	for( int i = 0; i < STALL_MAX_PET; i++ )
	{
		const tagStallPetEntity_ToLua& oneClientPetData = m_SelfStallItem.m_PetData[i];
		if( oneClientPetData.m_nDefID == 0 )
		{
			continue;
		}

		STALLPET& oneSvrStallPet	= stallStartData.PetData[nIndex];
		memcpy( &oneSvrStallPet.WID, &oneClientPetData.m_WID, sizeof( tdr_ulonglong ) );
		oneSvrStallPet.DefID		= oneClientPetData.m_nDefID;
		oneSvrStallPet.GridIdx		= oneClientPetData.m_nGridIdx;
		oneSvrStallPet.ListType		= oneClientPetData.m_nListType;
		oneSvrStallPet.Price		= oneClientPetData.m_uPrice;
		oneSvrStallPet.StallGridIdx	= nIndex;
		nIndex++;
	}
	m_SaledItem.clear();

	strncpy( stallStartData.StallName, szStallName, strlen( szStallName ) + 1 );
	strncpy( stallStartData.StallAdMsg, m_SelfStallItem.m_szStallAd, 
				strlen( m_SelfStallItem.m_szStallAd ) + 1 );
	SendGameServerMsg( STALL_CLT, msgbody );
}

// 结束摆摊
void GameStallManager::stall_End()
{
	tagCSPkgBody msgbody;
	msgbody.StallClt.Type = CLT_STALL_END;
	SendGameServerMsg( STALL_CLT, msgbody );
	m_SaledItem.clear();
}

// 摊位更名
void GameStallManager::stall_ChangeName( const char* szName )
{
	tagCSPkgBody msgbody;
	msgbody.StallClt.Type = CLT_STALL_CHANGE_NAME;
	strncpy( m_SelfStallItem.m_szStallName, szName,STALL_MAX_NAME_LEN );
	STALLCHGNAMEREQ& chgStallNameReqData = msgbody.StallClt.StallCltData.StallChgNameClt;
	strncpy( chgStallNameReqData.StallName, szName, strlen( szName ) + 1 );
	SendGameServerMsg( STALL_CLT, msgbody );
}

// 摊位广告更改
void GameStallManager::stall_ChangeAd( const char* pszAd )
{
	tagCSPkgBody msgbody;
	msgbody.StallClt.Type = CLT_STALL_CHANGE_ADMSG;
	strncpy( m_SelfStallItem.m_szStallAd, pszAd,STALL_MAX_NAME_LEN );
	STALLCHGNAMEREQ& chgStallAdReqData = msgbody.StallClt.StallCltData.StallChgAdMsgClt;
	strncpy( chgStallAdReqData.StallName, pszAd, strlen( pszAd ) + 1 );
	SendGameServerMsg( STALL_CLT, msgbody );
}

// 不发送摊位广告更改协议
void GameStallManager::localChangeAd( const char* pszAd )
{
	strncpy( m_SelfStallItem.m_szStallAd, pszAd,STALL_MAX_NAME_LEN );
}

// 摊位查看
void GameStallManager::stall_See( int nMemId )
{
	tagCSPkgBody msgbody;
	msgbody.StallClt.Type = CLT_STALL_SEE;
	STALLSEEREQ& clientSeeStallData = msgbody.StallClt.StallCltData.StallSeeClt;
	clientSeeStallData.MemID = nMemId;
	SendGameServerMsg( STALL_CLT, msgbody );
}

// 购物 stalltype 0 物品 1 宠物
void GameStallManager::stall_Buy( int nStallType, int nGridIdx, int nNum, const char* szName )
{
	static_assert(  sizeof(  tdr_ulonglong ) == sizeof( tagLong_ToLua ) );
	static_assert(  sizeof(  tdr_ulonglong ) == sizeof( tagPetId_ToLua ) );
	tagCSPkgBody msgbody;
	msgbody.StallClt.Type = CLT_STALL_BUY;

	const char* pMainPlayerName = ActorManager::getSingleton().getMainPlayer()->getName();
	STALLBUYREQ& stallBuyReqData = msgbody.StallClt.StallCltData.StallBuyClt;
	strncpy( stallBuyReqData.RoleName, szName, strlen( szName )+1 );
	stallBuyReqData.StallType		= nStallType;
	stallBuyReqData.StallGridIdx	= nGridIdx;
	stallBuyReqData.Num				= nNum;
	if ( nStallType == STALL_TYPE_ITEM )
	{
		Item& reqBuyItem = m_SeeStallItem.m_ItemData[nGridIdx];
		stallBuyReqData.WID		= reqBuyItem.getItemWId();
		stallBuyReqData.GirdGID = reqBuyItem.getStallGridWID();
	}
	else
	{
		const tagStallPetEntity_ToLua& reqBuyPetData = m_SeeStallItem.m_PetData[nGridIdx];
		memcpy( &stallBuyReqData.WID,		&reqBuyPetData.m_WID,		sizeof( tdr_ulonglong ) );
		memcpy( &stallBuyReqData.GirdGID,	&reqBuyPetData.m_GridGID,	sizeof( tdr_ulonglong ) );
	}
	
	SendGameServerMsg( STALL_CLT, msgbody );
}

// 扩充栏位
void GameStallManager::stall_AddGrid()
{
	tagCSPkgBody msgbody;
	msgbody.StallClt.Type = CLT_STALL_ADD_GRID;
	SendGameServerMsg( STALL_CLT, msgbody );
}

void GameStallManager::delPetInStall( int nGrid )
{
	if ( !canRemoveGoods() )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.StallClt.Type = CLT_STALL_DOWN;
	STALLDOWNREQ& stallDownReqData = msgbody.StallClt.StallCltData.StallDownClt;
	stallDownReqData.StallType	= STALL_TYPE_PET;
	
	assert( nGrid < STALL_MAX_PET );
	const tagStallPetEntity_ToLua& oneStallPetData = m_SelfStallItem.m_PetData[nGrid];
	memcpy( &stallDownReqData.WID, &oneStallPetData.m_WID, sizeof( tdr_ulonglong ) );

	stallDownReqData.StallGridIdx	= nGrid;
	SendGameServerMsg( STALL_CLT, msgbody );
}

void GameStallManager::delItemInStall( int nGrid )
{
	if ( !canRemoveGoods() )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.StallClt.Type = CLT_STALL_DOWN;
	STALLDOWNREQ& stallDownReqData = msgbody.StallClt.StallCltData.StallDownClt;
	stallDownReqData.StallType	= STALL_TYPE_ITEM;

	assert( nGrid < STALL_MAX_PET );
	Item& delItem = m_SelfStallItem.m_ItemData[nGrid];
	stallDownReqData.WID = delItem.getItemWId();
	
	stallDownReqData.StallGridIdx = nGrid;
	SendGameServerMsg( STALL_CLT, msgbody );
}

// 重置摆摊信息
void GameStallManager::resetAllStallInfo()
{
	memset( &m_SelfStallItem, 0, sizeof(tagStallData_ToLua) );
	memset( &m_LastSelfStallItem, 0, sizeof(tagStallData_ToLua) );
}

void GameStallManager::resetStallInfo()
{
	memset( &m_SelfStallItem, 0, sizeof(tagStallData_ToLua) );
	//memset( &m_SeeStallItem, 0, sizeof(tagStallData_ToLua) );
}

void GameStallManager::resetSeeStallInfo()
{
	memset( &m_SeeStallItem, 0, sizeof(tagStallData_ToLua) );
}

// 获取摆摊的栏位信息
int GameStallManager::getStallGridNum()
{
	return m_StallGridNum;
}

// 获取摊主的栏位信息
int GameStallManager::getSeeStallItemNum()
{
	int nCount = 0;
	for( int i = 0; i < STALL_MAX_ITEM; i++ )
	{
		Item& oneSeeItem = m_SeeStallItem.m_ItemData[i];
		if( oneSeeItem.getItemId() != 0 )
		{
			nCount++;
		}
	}
	return nCount;
}

int GameStallManager::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == STALL_SVR )
	{
		const CSSTALLDATASVR& stallData = msgbody.StallSvr.StallCltData;
		switch( msgbody.StallSvr.Type )
		{
		case SVR_STALL_START:
			{}
			break;
		case SVR_STALL_END:
			{}
			break;
		case SVR_STALL_CHANGE_NAME:
			{}
			break;
		case SVR_STAL_SHOW_ADMSG:
			{
				const STALLSHOWADMSGRES& stallAddData = stallData.StallShowAdMsgSvr;
				GameActor* pActor = ActorManager::getSingleton().FindActor( stallAddData.WID  );
				if( pActor == NULL )
				{
					break;
				}
				
				g_pFrameMgr->addPopWindow( "ChatPop",pActor->getName(),
									stallAddData.StallShowAdMsg, -100, -100, stallAddData.ShowTime, 
									"PopBackArrowTexture", true, 0, 0 );
				
			}
			break;

		case SVR_STALL_SEE:
			{
				const STALLSEERES& stallSeeResData = stallData.StallSeeSvr;
				OnStallSeeRes( stallSeeResData );
			}
			break;
		case SVR_STALL_BUY:
			{
				const STALLINFOCHG& stallBuyChgResData = stallData.StallBuySvr.StallInfoBuy;
				OnStallBuyerChgRes( stallBuyChgResData );
			}
			break;
		case SVR_STALL_INFO_CHG:
			{
				const STALLINFOCHG& stallSellerChgData = stallData.StallInfoChg;
				OnStallSellerChgRes( stallSellerChgData );
			}
			break;
		case SVR_STALL_GRID_NUM:
			{
				m_StallGridNum = stallData.StallGridNum;
				GameManager::getSingleton().SendUIEvent( "GE_STALL_GRIDNUM_CHG" );
			}
			break;

		case SVR_STAL_UP_RES:
			{
				const STALLUPRES& stallUpResData = stallData.StallUpRes;
				OnStallUpRes( stallUpResData );
			}
			break;

		case SVR_STAL_DOWN_RES:
			{
				const STALLDOWNRES& stallDownResData = stallData.StallDownRes;
				OnStallDownRes( stallDownResData );
			}
			break;

		case SVR_STAL_CHG_ADMSG_RES:
			{
				const STALLCHGADMSGRES& stallChgAdRes = stallData.StallChgAdMsgRes;
				char szScript[256] = { 0 };
				sprintf_s( szScript, _countof( szScript ), "CallScript_StallChgAdRes(%d)", stallChgAdRes.iSucc );
				GameManager::getSingleton().getScriptVM()->callString( szScript );
			}
			break;
		}
	}
	return 0;
}

void GameStallManager::update(unsigned int dtick)
{
	// 若没查看任何玩家的摊位信息，则不判断距离
	if ( m_SeeStallItem.m_szRoleName[0] == 0 )
	{
		return;
	}

	GameActor* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}

	GameActor* pActor = ActorManager::getSingleton().findActorByName( m_SeeStallItem.m_szRoleName );
	if ( pActor == NULL )
	{
		GameManager::getSingleton().SendUIEvent( "GE_STALL_BUY_END" );
		return;
	}

	GameXYZ pos = pMainPlayer->GetPosition();
	pos.y		= 0;

	GameXYZ pos2	= pActor->GetPosition();
	pos2.y			= 0;

	int nDis = (int)pos2.subtract(pos).length();

	// 超出距离关闭对话框
	if( nDis > SEE_DIS * 2 )
	{
		//GameMsgPrint::getSingleton().showMidTip( MID_STALL_DISTANCE_LONG );
		GameManager::getSingleton().SendUIEvent( "GE_STALL_BUY_END" );
	}
}

// 是否没有任何商品上架
bool GameStallManager::isNoneGoodsOnStall()
{
	return m_SelfStallItem.m_nItemNum == 0 && m_SelfStallItem.m_nPetNum == 0;
}

// 是否有NPC距离过近
bool GameStallManager::isNpcTooNear()
{
	ActorManager& actorMgr = ActorManager::getSingleton();
	MainGamePlayer* pMainPlayer = actorMgr.getMainPlayer();
	GameXYZ mainPos = pMainPlayer->GetPosition();
	mainPos.y		= 0;
	const float MIN_SQR_DIST_NEAR_NPC = pow( (float)CLIENT_MIN_STALL_DIST_BETWEEN_NPC, 2.0f );

	for ( ACTOR_ITER iter = actorMgr.getActorBegin(), end = actorMgr.getActorEnd(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor->getType() != GAT_NPC )
		{
			continue;
		}
		
		// 以毫米为单位
		GameNPC* pOneNPC = (GameNPC*)pOneActor;
		if ( pOneNPC->isPatrol() )
		{
			continue;
		}

		GameXYZ npcPos	= pOneNPC->GetPosition();
		npcPos.y		= 0;

		float fDis = npcPos.subtract( mainPos ).lengthSqr();

		// 超出距离关闭对话框
		if( fDis <=  MIN_SQR_DIST_NEAR_NPC )
		{
			return true;
		}
	}

	return false;
}

// 是否有其他摊位距离过近
bool GameStallManager::isOtherStallTooNear()
{
	ActorManager& actorMgr = ActorManager::getSingleton();
	MainGamePlayer* pMainPlayer = actorMgr.getMainPlayer();
	GameXYZ mainPos = pMainPlayer->GetPosition();
	mainPos.y		= 0;
	const float MIN_SQR_DIST_NEAR_OTHER_STALL = pow( (float)CLIENT_MIN_STALL_DIST_BETWEEN_OTHER_STALL, 2.0f );

	for ( ACTOR_ITER iter = actorMgr.getActorBegin(), end = actorMgr.getActorEnd(); iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;
		if ( pOneActor->getType() != GAT_PLAYER || !pOneActor->isInStall() )
		{
			continue;
		}

		// 以毫米为单位
		GameXYZ otherStallPos	= pOneActor->GetPosition();
		otherStallPos.y				= 0;

		float fDis = otherStallPos.subtract( mainPos ).lengthSqr();
		// 超出距离关闭对话框
		if( fDis <=  MIN_SQR_DIST_NEAR_OTHER_STALL )
		{
			return true;
		}
	}

	return false;
}

void GameStallManager::ValidateLastStallItems()
{
	if ( m_LastSelfStallItem.m_nItemNum == 0 )
	{
		return;
	}
	
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer == NULL )
	{
		return;
	}

	Container& containerMgr= pMainPlayer->getContainer();
	for ( int i = 0; i < STALL_MAX_ITEM; ++i )
	{
		Item& onePreStallItem		= m_LastSelfStallItem.m_ItemData[i];
		if ( onePreStallItem.getItemId() == 0 )
		{
			continue;
		}

		Item* pCurItemInContainer	= containerMgr.getItem( onePreStallItem.getItemWId() );
		if ( pCurItemInContainer == NULL )
		{
			onePreStallItem.releaseItem();
			--m_LastSelfStallItem.m_nItemNum;
		}
		else
		{
			int nStallNum = onePreStallItem.getNum();
			if ( onePreStallItem.getNum() > pCurItemInContainer->getNum() )
			{
				nStallNum = pCurItemInContainer->getNum();
			}

			unsigned int nStallSellPrice = onePreStallItem.getStallSellPrice();
			onePreStallItem = *pCurItemInContainer;
			onePreStallItem.setNum( nStallNum );
			onePreStallItem.m_nStallSellPrice = nStallSellPrice;
		}
	}
}

void GameStallManager::ValidateLastStallPets()
{
	if ( m_LastSelfStallItem.m_nPetNum == 0 )
	{
		return;
	}

	MainGamePlayer*		pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GamePet*			pPet		= pMainPlayer->getPet();

	for ( int i = 0; i < STALL_MAX_PET; ++i )
	{
		tagStallPetEntity_ToLua& onePreStallPetInfo = m_LastSelfStallItem.m_PetData[i];
		if ( onePreStallPetInfo.m_nDefID == 0 )
		{
			continue;
		}
		
		tagPetId_ToLua&		petWID		= onePreStallPetInfo.m_WID;
		tagRolePet_ToLua*	pPetInfo	= pPet->getPetInfo( petWID.nHeight, petWID.nLow );
		if ( pPetInfo == NULL )
		{
			memset( &m_LastSelfStallItem.m_PetData[i], 0, sizeof(tagStallPetEntity_ToLua) );
			--m_LastSelfStallItem.m_nPetNum;
		}
	}
}

void GameStallManager::ValidateLastStallGoods()
{
	ValidateLastStallItems();
	ValidateLastStallPets();
}

// 添加摆摊物品
void GameStallManager::addStallItem( int nStallGridIdx, int nListType, int nGrid, int nNum, unsigned int nPrice )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	
	Item& newAddItem = pMainPlayer->getContainer().getItem( nListType, nGrid );
	Item& destStallItem = m_SelfStallItem.m_ItemData[nStallGridIdx];
	destStallItem = newAddItem;
	destStallItem.setNum( nNum );
	destStallItem.m_nStallSellPrice = nPrice;
	m_SelfStallItem.m_nItemNum++;
}

void GameStallManager::addStallItemInStall( int nStallGridIdx, int nListType, int nGrid, int nNum, unsigned int nPrice )
{
	addStallItem( nStallGridIdx, nListType, nGrid, nNum, nPrice );
	Item& oneAddStallItem = m_SelfStallItem.m_ItemData[nStallGridIdx];

	tagCSPkgBody msgbody;
	msgbody.StallClt.Type			= CLT_STALL_UP;
	STALLUPREQ& stallUpReqData		= msgbody.StallClt.StallCltData.StallUpClt;
	stallUpReqData.Type				= STALL_TYPE_ITEM;

	STALLITEM& stallUpReqPetData	= stallUpReqData.StallUpData.StallItem;
	stallUpReqPetData.DefID			= oneAddStallItem.getItemId();
	stallUpReqPetData.GridIdx		= oneAddStallItem.getGridIdx();
	stallUpReqPetData.ListType		= oneAddStallItem.getListType();
	stallUpReqPetData.Num			= oneAddStallItem.getNum();
	stallUpReqPetData.Price			= oneAddStallItem.getStallSellPrice();
	stallUpReqPetData.WID			= oneAddStallItem.getItemWId();
	stallUpReqPetData.StallGridIdx	= nStallGridIdx;

	SendGameServerMsg( STALL_CLT, msgbody );
}

// 移除摆摊物品
void GameStallManager::delLastStallItem( int nIndex )
{
	m_LastSelfStallItem.m_ItemData[nIndex].releaseItem();
	--m_LastSelfStallItem.m_nItemNum;
}
void GameStallManager::delStallItem( int nIndex )
{
	if ( !canRemoveGoods() )
	{
		return;
	}
	m_SelfStallItem.m_ItemData[nIndex].releaseItem();
	--m_SelfStallItem.m_nItemNum;
}

// 移除摆摊宠物
void GameStallManager::delStallPet( int nIndex )
{
	if ( !canRemoveGoods() )
	{
		return;
	}

	memset( &m_SelfStallItem.m_PetData[nIndex], 0, sizeof(tagStallPetEntity_ToLua) );
	m_SelfStallItem.m_nPetNum--;
}

// 添加摆摊宠物( type 0 仙子 1宠物 )对应GamePet接口
void GameStallManager::addStallPet( int nStallIndex, int nType, int nIndex, unsigned int nPrice )
{
	MainGamePlayer*		pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GamePet*			pPet		= pMainPlayer->getPet();
	tagRolePet_ToLua*	pPetInfo	= pPet->getPetInfo( nIndex );
	
	// Item& newAddItem
	m_SelfStallItem.m_PetData[nStallIndex].m_nDefID			= pPetInfo->PetID;
	m_SelfStallItem.m_PetData[nStallIndex].m_WID			= *(tagPetId_ToLua*)&pPetInfo->WID;
	// TODO
	m_SelfStallItem.m_PetData[nStallIndex].m_nGridIdx		= nIndex;
	m_SelfStallItem.m_PetData[nStallIndex].m_nStallGridIdx	= nStallIndex;
	m_SelfStallItem.m_PetData[nStallIndex].m_uPrice			= nPrice;
	m_SelfStallItem.m_PetData[nStallIndex].m_RolePet		= *pPetInfo;
	if( nType == 0 )
	{
		m_SelfStallItem.m_PetData[nStallIndex].m_nListType = LIST_TYPE_XIAN; 
	}else{
		m_SelfStallItem.m_PetData[nStallIndex].m_nListType = LIST_TYPE_SHI;
	}
	m_SelfStallItem.m_nPetNum++;
}

// 摆摊状态添加摆摊宠物
void GameStallManager::addStallPetInStall( int nStallIndex, int nType, int nIndex, unsigned int nPrice )
{
	addStallPet( nStallIndex, nType, nIndex, nPrice );
	
	const tagStallPetEntity_ToLua& oneClientPetData = m_SelfStallItem.m_PetData[nStallIndex];

	tagCSPkgBody msgbody;
	msgbody.StallClt.Type			= CLT_STALL_UP;
	STALLUPREQ& stallUpReqData		= msgbody.StallClt.StallCltData.StallUpClt;
	stallUpReqData.Type				= STALL_TYPE_PET;
	
	STALLPET& stallUpReqPetData		= stallUpReqData.StallUpData.StallPet;
	stallUpReqPetData.DefID			= oneClientPetData.m_nDefID;
	stallUpReqPetData.GridIdx		= oneClientPetData.m_nGridIdx;
	stallUpReqPetData.ListType		= oneClientPetData.m_nListType;
	stallUpReqPetData.Price			= oneClientPetData.m_uPrice;
	//stallUpReqPetData.StallGridIdx	= m_SelfStallItem.m_nPetNum - 1;
	stallUpReqPetData.StallGridIdx	= nStallIndex;
	memcpy( &stallUpReqPetData.WID, &oneClientPetData.m_WID, sizeof( tdr_ulonglong ) );

	SendGameServerMsg( STALL_CLT, msgbody );
}

// 检查摊位上的宠物是否已摆满
bool GameStallManager::isStallPetListFull()
{
	if ( m_SelfStallItem.m_nPetNum < STALL_MAX_PET )
	{
		return false;
	}

	return true;
}

// 检查摊位上的物品是否已摆满
bool GameStallManager::isStallItemListFull()
{
	if ( m_SelfStallItem.m_nItemNum < getStallGridNum() )
	{
		return false;
	}

	return true;
}

// 是否是正在交易中的物品
bool GameStallManager::isInStallItem( int nListType, int nGrid )
{
	for( int i = 0; i < STALL_MAX_ITEM; i++ )
	{
		if( m_SelfStallItem.m_ItemData[i].getItemId() != 0 
			&& m_SelfStallItem.m_ItemData[i].getListType() == nListType && m_SelfStallItem.m_ItemData[i].getGridIdx() == nGrid )
		{
			return true;
		}
	}

	return false;
}

bool GameStallManager::isInStallIPet( int nType, int nGrid )
{
	int nListType = LIST_TYPE_SHI;
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GamePet* petPackage = pMainPlayer->getPet();
	tagRolePet_ToLua* pPetInfo = petPackage->getPetInfo( nGrid );
	if ( pPetInfo == NULL )
	{
		return false;
	}

	for ( int i = 0; i < STALL_MAX_PET; ++i )
	{
		if ( m_SelfStallItem.m_PetData[i].m_WID.nHeight == pPetInfo->WID.nHeight && 
				m_SelfStallItem.m_PetData[i].m_WID.nLow == pPetInfo->WID.nLow )
		{
			return true;
		}
	}

	return false;
}

bool GameStallManager::isInStallIPet( int nType, int nHeightID, int nLowID )
{
	assert( !( nHeightID == 0 && nLowID == 0 ) );
	for ( int i = 0; i < STALL_MAX_PET; ++i )
	{
		const tagPetId_ToLua& oneStallPetWID = m_SelfStallItem.m_PetData[i].m_WID;
		if ( oneStallPetWID.nHeight == nHeightID && 
				oneStallPetWID.nLow == nLowID )
		{
			return true;
		}
	}

	return false;
}

void GameStallManager::SaveLastStallGoodsToStall()
{
	memcpy( &m_LastSelfStallItem, &m_SelfStallItem, sizeof( m_SelfStallItem ) );
}

void GameStallManager::PutLastStallGoodsToStall()
{
	memcpy( &m_SelfStallItem, &m_LastSelfStallItem, sizeof( m_SelfStallItem ) );
}

// 获取自己即将摆摊的物品
const tagStallData_ToLua& GameStallManager::getSelfStallItem()const
{
	return m_SelfStallItem;
}

const tagStallData_ToLua& GameStallManager::getSelfLastStallItem()const
{
	return m_LastSelfStallItem;
}


// 获取查看对象摆摊的物品
const tagStallData_ToLua& GameStallManager::getSeeStallItem()const
{
	return m_SeeStallItem;
}

// nType 0表示自己，1表示对方
bool  GameStallManager::getItemLink( int nType, int GridIndex )
{
	Item* pItem = NULL;
	if ( nType == 0 )
	{
		pItem = &( m_SelfStallItem.m_ItemData[GridIndex] );
	}
	else
	{
		pItem = &( m_SeeStallItem.m_ItemData[GridIndex] );
	}

	if( pItem->getItemId() != 0 && ( (GetKeyState(VK_LSHIFT) & 0xff00) || (GetKeyState(VK_RSHIFT) & 0xff00) ) )
	{
		GameMsgPrint::getSingleton().setItemLink( *pItem );
		return true;
	}
	return false;
}

bool GameStallManager::canSeeStall()
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();

	// PK与战斗状态 均不做处理
	/*if ( pMainplayer->getActState().getInState( GSACT_IN_FIGHT ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SEE_FIGHT );
		return false;
	}*/
	/*if ( pMainplayer->isInPK() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SEE_PK );
		return false;
	}*/

	if ( pMainplayer->getActState().GetState( ) == GSACT_DEAD )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SEE_DEAD );
		return false;
	}

	return true;
}

bool GameStallManager::canStartStall( )
{
	MainGamePlayer* pMainplayer = ActorManager::getSingleton().getMainPlayer();

	// 先判断主角自身是否处于战斗状态,若主角处于战斗状态,则不能进行申请交易请求 
	if ( pMainplayer->getActState().getInState( GSACT_IN_FIGHT ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SELF_FIGHT );
		return false;
	}

	if ( pMainplayer->isDead() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SELF_DEAD );
		return false;
	}

	if ( pMainplayer->isInPK() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SELF_PK );
		return false;
	}

	if ( pMainplayer->getActState().GetState() == GSACT_STALL )
	{
		return false;
	}

	if ( pMainplayer->getActState().getInState( GSACT_IN_RIDE ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SELF_RIDE );
		return false;
	}

	if ( pMainplayer->getActState().getInState( GSACT_IN_MORPH ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SELF_MORPH );
		return false;
	}

	GameTrump* pTrump = pMainplayer->getTrump();
	/*if ( pTrump->isPre() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SELF_TRUMP_PRE );
		return false;
	}*/

	if ( isNpcTooNear() ) 
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SELF_NEAR_NPC );
		return false;
	}

	if ( isOtherStallTooNear() ) 
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_SELF_NEAR_OTHER_STALL );
		return false;
	}
	
	if ( pMainplayer->getLv() < CLIENT_MIN_STALL_LEVEL )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_NOTENOUGHLV );
		return false;
	}

	if ( isSellGoodsMoneyTooMuch() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_TOOMUCH_MONEY );
		return false;
	}

	return true;
}

bool GameStallManager::isInStallArea()
{
	/*GameMapShower* pMapShower = GameManager::getSingleton().getGameMapShower();
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain == NULL || pMain->getCurMap() == NULL )
	{
		return false;
	}

	const MAPDEF& mapdef = DBDataManager::getSingleton().getMapDef( pMain->getCurMap()->getMapId() );
	const GameXYZ& mainplayerPos = pMain->GetPosition();
	Ogre::Vector2 begin( (float)mainplayerPos.x/10, (float)mainplayerPos.z/10 );
	
	for( int i = 0, size = _countof( mapdef.StallPointList ) ; i < size; ++i )
	{
		const STALLPOINT& onePoint = mapdef.StallPointList[i];
		if ( onePoint.Radius <= 0 )
		{
			break;
		}

		Ogre::Vector2 end( (float)onePoint.StallPos.X, (float)onePoint.StallPos.Y );
		if( (end-begin).length() <= onePoint.Radius )
		{
			return true;
		}
	}

	return false;*/
	GameMapShower* pMapShower	= GameManager::getSingleton().getGameMapShower();
	GameMapControl* pMapControl	= pMapShower->GetMaxMapContal();
	int nMapID = pMapControl->getMapID();
	return nMapID == CLIENT_STALL_MAP;
}

// 是否可以下架商品
bool GameStallManager::canRemoveGoods()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( !pMainPlayer->isInStall() )
	{
		return true;
	}

	if ( m_SelfStallItem.m_nItemNum == 0 && m_SelfStallItem.m_nPetNum == 1 )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_CANNOT_EMPTY );
		return false;
	}

	if ( m_SelfStallItem.m_nPetNum == 0 && m_SelfStallItem.m_nItemNum == 1 )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_STALL_CANNOT_EMPTY );
		return false;
	}

	return true;
}

void GameStallManager::OnStallSeeRes( const STALLSEERES& stallSeeResData )
{
	memset( m_SeeStallItem.m_ItemData,	0, sizeof(Item) * STALL_MAX_ITEM );
	memset( m_SeeStallItem.m_PetData,	0, sizeof(tagStallPetEntity_ToLua) * STALL_MAX_PET );

	int nPetNum		= stallSeeResData.PetNum;
	int nItemNum	= stallSeeResData.ItemNum;
	for( int i = 0; i < nItemNum; i++ )
	{
		const STALLITEMENTITY& oneSrcItemData	= stallSeeResData.ItemEntity[i];
		int nGridIdx							= oneSrcItemData.StallGridIdx;
		Item& oneDestItemData					= m_SeeStallItem.m_ItemData[nGridIdx];
		if ( oneSrcItemData.RoleGrid.Type == GRID_ITEM_ARM )
		{
			oneDestItemData = oneSrcItemData.RoleGrid.GridData.RoleArm;
		}
		else
		{
			oneDestItemData = oneSrcItemData.RoleGrid.GridData.RoleItem;
		}
		oneDestItemData.m_nStallSellPrice	= oneSrcItemData.Price;
		oneDestItemData.m_StallGridGID		= oneSrcItemData.GridGID;
	}

	for( int i = 0; i < nPetNum; i++ )
	{
		const STALLPETENTITY& oneSrcPetData = stallSeeResData.PetEntity[i];
		int nGridIdx = oneSrcPetData.StallGridIdx;
		tagStallPetEntity_ToLua& oneDestPetData = m_SeeStallItem.m_PetData[nGridIdx];
		oneDestPetData.m_RolePet = *(tagRolePet_ToLua*)&oneSrcPetData.RolePet;
		oneDestPetData.m_uPrice	 = oneSrcPetData.Price;
		oneDestPetData.m_WID	 = *(tagPetId_ToLua*)&oneSrcPetData.RolePet.WID;
		oneDestPetData.m_nStallGridIdx	= oneSrcPetData.StallGridIdx;
		memcpy( &oneDestPetData.m_GridGID, &oneSrcPetData.GridGID, sizeof( tdr_ulonglong ) );
	}

	m_SeeStallItem.m_nPetNum		= nPetNum;
	m_SeeStallItem.m_nItemNum		= nItemNum;

	strncpy( m_SeeStallItem.m_szRoleName,	stallSeeResData.RoleName,	strlen( stallSeeResData.RoleName ) + 1 );
	strncpy( m_SeeStallItem.m_szStallName,	stallSeeResData.StallName,	strlen( stallSeeResData.StallName ) + 1 );
	GameManager::getSingleton().SendUIEvent("GE_STALL_SEE");
}

void GameStallManager::OnStallBuyerChgRes( const STALLINFOCHG& stallBuyChgResData )
{
	int nStallType	= stallBuyChgResData.StallType;
	int nGridIdx	= stallBuyChgResData.StallGridIdx;
	if( nStallType == STALL_TYPE_ITEM )
	{
		Item& oneClientChgItem = m_SeeStallItem.m_ItemData[nGridIdx];
		if ( oneClientChgItem.getItemWId() == stallBuyChgResData.WID )
		{
			if ( stallBuyChgResData.Num == 0 )
			{
				oneClientChgItem.releaseItem();
				--m_SeeStallItem.m_nItemNum;
			}
			else
			{
				oneClientChgItem.setNum( stallBuyChgResData.Num );
			}
		}
		else
		{
			assert( false && "服务器错发物品WID" );
		}
	}
	else
	{
		tagStallPetEntity_ToLua&	oneClientChgPetData = m_SeeStallItem.m_PetData[nGridIdx];
		tdr_ulonglong*				clientChgPetWID		= (tdr_ulonglong*)&oneClientChgPetData.m_WID;
		if( *clientChgPetWID == stallBuyChgResData.WID )
		{
			memset( &oneClientChgPetData, 0, sizeof(tagStallPetEntity_ToLua) );
			--m_SeeStallItem.m_nPetNum;
		}
		else
		{
			assert( false && "服务器错发宠物WID" );
		}
	}
	GameManager::getSingleton().SendUIEvent("GE_STALL_BUYER_CHG");
}

void GameStallManager::OnStallSellerChgRes( const STALLINFOCHG& stallSellerChgData )
{
	int nStallType	= stallSellerChgData.StallType;
	int nGridIdx	= stallSellerChgData.StallGridIdx;
	int nChgItemNum = 0;

	if( nStallType == STALL_TYPE_ITEM )
	{
		Item& oneClientChgItem = m_SelfStallItem.m_ItemData[nGridIdx];
		if( oneClientChgItem.getItemWId() == stallSellerChgData.WID )
		{
			nChgItemNum = oneClientChgItem.getNum() - stallSellerChgData.Num;
		}
	}
	else
	{
		nChgItemNum = 1;
	}

	char szScript[256];
	sprintf( szScript, "CallScript_UpdateStallInfo( %d, %d, %d, \"%s\",%d )", nStallType, nGridIdx, nChgItemNum, stallSellerChgData.BuyName, m_SaledItem.size() );
	GameManager::getSingleton().getScriptVM()->callString( szScript );

	if( nStallType == STALL_TYPE_ITEM )
	{
		Item& oneClientChgItem = m_SelfStallItem.m_ItemData[nGridIdx];
		m_SaledItem.push_back(oneClientChgItem);
		if( oneClientChgItem.getItemWId() == stallSellerChgData.WID )
		{
			nChgItemNum = oneClientChgItem.getNum() - stallSellerChgData.Num;
			if( stallSellerChgData.Num == 0 )
			{
				oneClientChgItem.releaseItem();
				--m_SelfStallItem.m_nItemNum;
			}else{
				oneClientChgItem.setNum( stallSellerChgData.Num );
			}
		}else{
			assert(0);
		}
	}
	else
	{

		if( *(tdr_ulonglong*)&m_SelfStallItem.m_PetData[nGridIdx].m_WID == stallSellerChgData.WID )
		{
			memset( &m_SelfStallItem.m_PetData[nGridIdx], 0, sizeof(tagStallPetEntity_ToLua) );
			m_SelfStallItem.m_nPetNum--;
		}else{
			assert(0);
		}
	}
	GameManager::getSingleton().SendUIEvent( "GE_STALL_SELLER_CHG" );
}

void GameStallManager::OnStallUpRes( const STALLUPRES& stallUpResData )
{
	if ( stallUpResData.Succ == STALL_CHG_ITEM_INSTALL_FAILED )
	{
		// 若是宠物，则更新
		if ( stallUpResData.StallType == STALL_TYPE_ITEM )
		{
			Item& failItem = m_SelfStallItem.m_ItemData[stallUpResData.StallGridIdx];
			failItem.releaseItem();
			--m_SelfStallItem.m_nItemNum;
		}
		else if ( stallUpResData.StallType == STALL_TYPE_PET )
		{
			tagStallPetEntity_ToLua& failPet = m_SelfStallItem.m_PetData[stallUpResData.StallGridIdx];
			memset( &failPet, 0, sizeof(tagStallPetEntity_ToLua) );
			m_SelfStallItem.m_nPetNum--;
		}
	}
	else
	{
		char szScript[256];
		sprintf( szScript, "CallScript_StallUpSuccRes( %d, %d )", stallUpResData.StallType, stallUpResData.StallGridIdx );
		GameManager::getSingleton().getScriptVM()->callString( szScript );
	}
}

void GameStallManager::OnStallDownRes( const STALLDOWNRES& stallDownResData )
{
	if ( stallDownResData.Succ == STALL_CHG_ITEM_INSTALL_SUCCESS )
	{
		int nList = -1;
		int nGrid = -1;
		int nId	  = 0;
		if ( stallDownResData.StallType == STALL_TYPE_ITEM )
		{
			Item& failItem = m_SelfStallItem.m_ItemData[stallDownResData.StallGridIdx];
			nList	= failItem.getListType();
			nGrid	= failItem.getGridIdx();
			nId		= failItem.getItemId();
			failItem.releaseItem();
			--m_SelfStallItem.m_nItemNum;
		}
		else if ( stallDownResData.StallType == STALL_TYPE_PET )
		{
			tagStallPetEntity_ToLua& failPet = m_SelfStallItem.m_PetData[stallDownResData.StallGridIdx];
			memset( &failPet, 0, sizeof(tagStallPetEntity_ToLua) );
			--m_SelfStallItem.m_nPetNum;
		}

		char szScript[256];
		sprintf( szScript, "StallDownRes( %d, %d, %d, %d, %d )", stallDownResData.StallType, stallDownResData.StallGridIdx, nList,
							nGrid, nId );
		GameManager::getSingleton().getScriptVM()->callString( szScript );
	}
}

Item* GameStallManager::getSaledItem( int idx )
{
	if( idx >= m_SaledItem.size() )
	{
		return NULL;
	}
	return &(m_SaledItem[idx]);
}