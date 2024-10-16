#include "GameActor_Trump.h"
#include "GameMsgPrint.h"
#include "OgreScriptLuaVM.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameActorManager.h"
#include "GameSkill_Mgr.h"
#include "ShowActor.h"
#include "OgreEntity.h"
#include "GameMap.h"

GameTrump::GameTrump(void):m_nRoleTrumpNum(0),m_nRoleMaxTrumpNum(0),m_bPre(false), m_pOwner( NULL ), m_nCurCampFairyId(0), m_nCurCampFairyLv(0)
{
	memset( m_RoleTrump, 0, sizeof( tagRoleFairy_ToLua ) * FAIRY_MAX_NUM );
}

GameTrump::~GameTrump(void)
{
}

// 出征吟唱
void GameTrump::trumpPre( int nHightId, int nLowId, int nGridIdx )
{
	if ( !canCampaignTrump() )
	{
		return;
	}
	
	tagCSPkgBody msgbody;
	msgbody.FairyClt.OpType   = OP_CAMP_PRE;

	tagTrumpId trumpId;
	trumpId.nHeight	= nHightId;
	trumpId.nLow	= nLowId;

	msgbody.FairyClt.WID	    = *( (tdr_ulonglong*)&trumpId );
	msgbody.FairyClt.Grididx    = nGridIdx;

	SendGameServerMsg( FAIRY_CLT, msgbody );

	m_pOwner->startTrumpChant( nHightId,nLowId,nGridIdx );
}

// 法宝炼化
void GameTrump::trumpEvolve( int nHightId, int nLowId, int nGridIdx )
{
	tagCSPkgBody msgbody;
	msgbody.FairyClt.OpType   = OP_EVOLVE;

	tagTrumpId trumpId;
	trumpId.nHeight	= nHightId;
	trumpId.nLow	= nLowId;

	msgbody.FairyClt.WID	    = *( (tdr_ulonglong*)&trumpId );
	msgbody.FairyClt.Grididx    = nGridIdx;

	SendGameServerMsg( FAIRY_CLT, msgbody );
}

// 停止炼化
void GameTrump::trumpStopEvolve( int nHightId, int nLowId, int nGridIdx ) 
{
	tagCSPkgBody msgbody;
	msgbody.FairyClt.OpType   = OP_EVOLVE_STOP;

	tagTrumpId trumpId;
	trumpId.nHeight	= nHightId;
	trumpId.nLow	= nLowId;

	msgbody.FairyClt.WID	    = *( (tdr_ulonglong*)&trumpId );
	msgbody.FairyClt.Grididx    = nGridIdx;

	SendGameServerMsg( FAIRY_CLT, msgbody );
}

// 法宝召回
void GameTrump::trumpBack( int nHightId, int nLowId, int nGridIdx )
{
	if ( !canBackTrump() )
	{
		return;
	}

	tagCSPkgBody msgbody;
	msgbody.FairyClt.OpType   = OP_BACK;

	tagTrumpId trumpId;
	trumpId.nHeight	    = nHightId;
	trumpId.nLow		= nLowId;

	msgbody.FairyClt.WID	    = *( (tdr_ulonglong*)&trumpId );
	msgbody.FairyClt.Grididx    = nGridIdx;

	SendGameServerMsg( FAIRY_CLT, msgbody );
}

// 注入经验值
void GameTrump::trumpInfuseExp( int nHightId, int nLowId, int nGridIdx, int nIntoExp )
{
	tagCSPkgBody msgbody;
	msgbody.FairyClt.OpType   = OP_INTO_EXP;

	tagTrumpId trumpId;
	trumpId.nHeight	    = nHightId;
	trumpId.nLow		= nLowId;

	msgbody.FairyClt.WID	    = *( (tdr_ulonglong*)&trumpId );
	msgbody.FairyClt.Grididx    = nGridIdx;
	msgbody.FairyClt.IntoExp	= nIntoExp;

	SendGameServerMsg( FAIRY_CLT, msgbody );
}

// 注册网络消息
void GameTrump::initTrumpNetMessage()
{
	INetworkManager::getSingleton().RegisterMsgHandler( FAIRY_SVR, this );
	INetworkManager::getSingleton().RegisterMsgHandler( KEYIN_ITEM_SVR,this );
}

void GameTrump::releasePetNetMessage()
{
	INetworkManager::getSingleton().unRegisterMsgHander( FAIRY_SVR );
	INetworkManager::getSingleton().unRegisterMsgHander( KEYIN_ITEM_SVR );
}

// 设置拥有者
void GameTrump::setOwner( GameActor* pOwner )
{
	m_pOwner = pOwner;
}

// 初始化法宝信息
void GameTrump::initTrumpPackage( const FAIRYPACKAGE& fairypackage )
{
	static_assert( sizeof(ROLEFAIRY) == sizeof(tagRoleFairy_ToLua) );
	m_nRoleMaxTrumpNum  = fairypackage.FairyMaxNum;
	m_nRoleTrumpNum  = fairypackage.FairyNum;
	memset( m_RoleTrump, 0, sizeof( ROLEFAIRY ) * FAIRY_MAX_NUM );
	for( int i = 0; i < m_nRoleTrumpNum; i++ )
	{
		memcpy( &m_RoleTrump[i], &fairypackage.RoleFairys[i], sizeof( tagRoleFairy_ToLua ) );
		if( ActorManager::getSingleton().getMainPlayer() != NULL )
		{
			if( isFairyInState( fairypackage.RoleFairys[i].Stat, FAIRY_STAT_CAMP ) )
			{
				m_nCurCampFairyId = fairypackage.RoleFairys[i].FairyID;
				m_nCurCampFairyLv = fairypackage.RoleFairys[i].FairyLevel;
			}
		}
	}
	ActorManager::getSingleton().getMainPlayer()->loadFairyEffect();
	GameMsgPrint::getSingleton().SendUIEvent( "GE_TRUMP_ATTRUPDATE" );
}

int GameTrump::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == FAIRY_SVR )
	{
		switch( msgbody.FairySvr.FairyResType )
		{
		case FAIRY_RES_TYPE_DATA: 
			{
				setTrumpAttr( msgbody.FairySvr.FairySvrData.FairyData );
				GameMsgPrint::getSingleton().SendUIEvent("GE_TRUMP_UPDATE");
			}
			break;
		case FAIRY_RES_TYPE_PRE_BRK:
			{
				m_pOwner->onBreakChant( eTrump_Chant );
			}
			break;
		}
	}else if( msgid == KEYIN_ITEM_SVR )
	{
		if( msgbody.ExtendItemSvr.Succeeded )
		{
			GameMsgPrint::getSingleton().SendUIEvent("GE_ENCHANT_SUCCEED");
		}
	}
	return 0;
}

// 获取法宝信息
tagRoleFairy_ToLua* GameTrump::getRoleFairy( int nGridIdx )
{
	static_assert( sizeof(tagRoleFairy_ToLua) == sizeof(tagRoleFairy) );
	assert( nGridIdx <= m_nRoleMaxTrumpNum );
	return &m_RoleTrump[nGridIdx];
}

// 获取当前出征法宝的信息
tagRoleFairy_ToLua* GameTrump::getCampRoleFairy()
{
	for( int i = 0; i < FAIRY_MAX_NUM; i++ )
	{
		if( isFairyInState( m_RoleTrump[i].Stat, FAIRY_STAT_CAMP ) )
		{
			return &m_RoleTrump[i]; 
		}
	}
	return NULL;
}

// 是否有精灵出征
bool GameTrump::isAnyFairyCamp()
{
	return getCampRoleFairy() != NULL;
}

// 获取法宝静态信息
tagFairyDef_ToLua* GameTrump::getFairyDef( int nId, int nLv )
{
	static_assert( sizeof(tagFairyDef_ToLua) == sizeof(tagFairyDef) );
	return (tagFairyDef_ToLua*)DBDataManager::getSingleton().getFairyDef( nId, nLv );
}

bool GameTrump::isPre()
{ 
	return m_pOwner->isInPre(eTrump_Chant);
}

// 判断是否能够召回出征精灵
bool GameTrump::canBackTrump()
{
	if (SkillManager::getSingleton().getSpellControler()->getCurSpell())
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_BACK_FAIL );
		return false;
	}
	return true;
}

// 判断是否能够继续出征精灵
bool GameTrump::canCampaignTrump()
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( m_pOwner != pMainPlayer )
	{
		return false;
	}

// 	if( pMainPlayer->isDead() )
// 	{
// 		GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PER_FAIL_INDEAD );
// 		return false;
// 	} 

// 	if ( pMainPlayer->getActState().getInState( GSACT_IN_FIGHT ) )
// 	{
// 		GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PER_FAIL_INFIGHT );
// 		return false;
// 	}
	if( !pMainPlayer->getBufStatus()->canUsedSpell() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PER_FAIL_BADSTATUS );
		return false;
	}
	if( pMainPlayer->getActState().GetState() == GSACT_MOVE )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PER_FAIL_MOVE );
		return false;
	}

// 	if( SkillManager::getSingleton().getSpellControler()->getCurSpell() != NULL)
// 	{
// 		GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PRE_FAIL_SKILLPRE );
// 		return false;
// 	}

	if( pMainPlayer->getActState().GetState() == GSACT_COLLECTITEM )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PRE_FAIL_COLLECT );
		return false;
	}

	if( !pMainPlayer->canControl() )
	{
		return false;
	}

	GameMap *map = pMainPlayer->getCurMap();
	if (map == NULL)
	{
		return false;
	}
	const tagMapDef &mapdef = DBDataManager::getSingleton().getMapDef(map->getRealId());
	if( mapdef.CtrlFlag & MAP_CTRL_NO_FAIRY_F )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_MAP_NO_FAIRY_FIGHT );
		return false;
	}
	return pMainPlayer->canStartChant( eTrump_Chant );
}

// 判断是否能在副本出征精灵
bool GameTrump::canCampInPworld( int nRealMapId )
{
	if( nRealMapId >= GAME_PWORLD_MAPID )
	{
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nRealMapId / 100  );
		if( pPworldDef != NULL && ( (pPworldDef->CtrlFlag & PWORLD_NO_FAIRY_F) != 0 ))
		{
			return false;
		}
	}
	return true;
}

// 判断宠物的状态
bool GameTrump::isFairyInState( int nFairyState, int nState )
{
	return  ( nFairyState & nState ) != 0;
}

void GameTrump::setTrumpAttr( const FAIRYDATA& fairydata )
{
	int nGridIdx = fairydata.Grididx;
	if( fairydata.Stat == FAIRY_STAT_DROP )
	{
		memset( &m_RoleTrump[nGridIdx], 0, sizeof(tagRoleFairy_ToLua) );
		m_nRoleTrumpNum--;
	}else{
		bool bAddNew = false;
		bool bLevelUp = false;
		if( *((tdr_ulonglong*)&m_RoleTrump[nGridIdx].WID) == 0 )
		{
			m_nRoleTrumpNum++;
			bAddNew = true;			
		}else if( m_RoleTrump[nGridIdx].FairyLevel != fairydata.FairyLv )
		{
			bLevelUp = true;			
		}else if( m_RoleTrump[nGridIdx].StarLvl < fairydata.Star )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_TRUMP_STARUP" );
		}

		m_RoleTrump[nGridIdx].FairyID        = fairydata.FairyID;
		m_RoleTrump[nGridIdx].FairyLevel     = fairydata.FairyLv;
		m_RoleTrump[nGridIdx].EvolveLeftTM   = fairydata.EvolveLeftTM;
		m_RoleTrump[nGridIdx].Stat           = fairydata.Stat;
		m_RoleTrump[nGridIdx].Exp			 = fairydata.Exp;
		memcpy( &m_RoleTrump[nGridIdx].WID, &fairydata.WID, sizeof(tagTrumpId) );
		m_RoleTrump[nGridIdx].StarLvl		 = fairydata.Star;
		m_RoleTrump[nGridIdx].TraitID		 = fairydata.TraitID;
		m_RoleTrump[nGridIdx].IsKeyin		 = fairydata.IsKeyin;
		if( bAddNew )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_TRUMP_ADD" );
		}
		if( bLevelUp )
		{
			char szTmp[256];
			sprintf( szTmp, "TrumpLevelUp(%d,%d)", fairydata.FairyLv, nGridIdx );
			ActorManager::getSingleton().getScriptVM()->callString( szTmp );
		}
	}
	GameMsgPrint::getSingleton().SendUIEvent( "GE_TRUMP_ATTRUPDATE" );
}

const tagFairyStarDef_ToLua* GameTrump::getFairyStarDef( int nFairyId,int nStar )
{
	static_assert( sizeof(tagFairyStarDef_ToLua) == sizeof(tagFairyStarDef) );
	return ( const tagFairyStarDef_ToLua* )DBDataManager::getSingleton().getFairyStarDef( nFairyId,nStar );
}

const tagFairyStarStuffDef_ToLua* GameTrump::getFairyStarStuffDef( int itemId )
{
	static_assert( sizeof(tagFairyStarStuffDef_ToLua) == sizeof(tagFairyStarStuffDef) );
	return (const tagFairyStarStuffDef_ToLua*)DBDataManager::getSingleton().getFairyStarStuffDef( itemId );
}

const tagPetTraitDef_ToLua* GameTrump::getPetTraitDef( int nTraidId,int nStar )
{
	static_assert( sizeof( tagPetTraitDef_ToLua ) == sizeof( tagPetTraitDef ) );
	return ( const tagPetTraitDef_ToLua* )DBDataManager::getSingleton().getPetTraitDef( nTraidId,nStar );
}

//品质提升
void GameTrump::requestOpStarUp( int nIdx,int nStuffID,int nStuffNum,int nMoney )
{
	tagCSPkgBody msgbody;
	memset(&msgbody,0,sizeof(tagCSPkgBody));
	msgbody.FairyClt.OpType = OP_STAR_UP;
	memcpy(&msgbody.FairyClt.WID,&m_RoleTrump[nIdx].WID,sizeof(tdr_ulonglong));
	msgbody.FairyClt.StarUpStuff.MainStuffID = nStuffID;
	msgbody.FairyClt.StarUpStuff.MainStuffNum = nStuffNum;
	msgbody.FairyClt.StarUpStuff.Money = nMoney;
	SendGameServerMsg( FAIRY_CLT,msgbody );
}

// //刻印
void GameTrump::requestOpKeyIn( int nIdx )
{
	if( nIdx > m_nRoleMaxTrumpNum )
	{
		return;
	}
	tagCSPkgBody msgbody;
	msgbody.FairyClt.OpType = OP_KEYIN;
	memcpy( &msgbody.FairyClt.WID,&m_RoleTrump[nIdx].WID,sizeof( tdr_ulonglong ) );
	SendGameServerMsg( FAIRY_CLT,msgbody );
}

void GameTrump::requestKeyInItem( int nSrcList,int nSrcGrid,int nDestList,int nDestGrid )
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain == NULL )
	{
		return;
	}
	Item& srcItem = pMain->getContainer().getItem( nSrcList,nSrcGrid );
	Item& destItem = pMain->getContainer().getItem( nDestList,nDestGrid );
	if( srcItem.getItemId() != 0 && destItem.getItemId() != 0  )
	{
		tagCSPkgBody msgbody;
		msgbody.ExtendItemClt.BeUsed.ListType= nSrcList ;
		msgbody.ExtendItemClt.BeUsed.GridIdx = nSrcGrid;
		msgbody.ExtendItemClt.BeUsed.WID = srcItem.getItemWId();
		msgbody.ExtendItemClt.BeExtended.ListType = nDestList ;
		msgbody.ExtendItemClt.BeExtended.GridIdx = nDestGrid;
		msgbody.ExtendItemClt.BeExtended.WID = destItem.getItemWId(); 
		SendGameServerMsg(KEYIN_ITEM_CLT,msgbody);
	}
}

void GameTrump::requestDecEvolveTime( int nIdx )
{
	if( nIdx > m_nRoleMaxTrumpNum )
	{
		return;
	}
	tagCSPkgBody msgbody;
	memset( &msgbody,0,sizeof(tagCSPkgBody) );
	msgbody.FairyClt.OpType = OP_DEC_EVOLVE_TIME;
	memcpy( &msgbody.FairyClt.WID,&m_RoleTrump[nIdx].WID,sizeof( tdr_ulonglong ) );
	SendGameServerMsg( FAIRY_CLT,msgbody );
}

int GameTrump::getStrengthFairyLevel( int fairyID, int StarLv )
{
	for( int i = 0 ; i < 12 ; i++ )
	{
		const FAIRYDEF* pDef = DBDataManager::getSingleton().getFairyDef( fairyID , i+1 );
		if( pDef != NULL && pDef->MaxStar >= StarLv )
		{
			return pDef->Level;
		}
	}
	return 0;
}

void GameTrump::clearTrumpData()
{
	memset( m_RoleTrump, 0, sizeof( tagRoleFairy_ToLua ) * FAIRY_MAX_NUM );
}