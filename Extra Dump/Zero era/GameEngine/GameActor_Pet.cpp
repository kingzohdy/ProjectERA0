#pragma once 
#include "GameActor_Pet.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "OgreRect.h"
#include "GameMaskMap.h"
#include "GameMap.h"
#include "OgreScriptLuaVM.h"
#include "OgreVector2.h"
#include "GameMsgPrint.h"
#include "GameStateAction.h"
#include "GameActorManager.h"
#include "GameSCTDamageManager.h"
#include "GameSkill_Mgr.h"
#include "proto_cs.h"
#include "ShowActor.h"
#include "GameNpc.h"
#include <memory.h>
#include <algorithm>
#include <Windows.h>
#include <stdlib.h>
#include "../UILib/ui_framemgr.h"
#include "GameCamera.h"
#include "OgreCamera.h"
#include "OgreXMLData.h"
#include "OgreLog.h"
#include "PworldLimitInfo.h"

using namespace Ogre;

const float MY_PI = 3.1415926f;
const float maxr = 180;

int DEFAULT_CLIENT_PET_FREEDOM_SPEED = 300;
int FREEDOM_ANIM_TIME				= 10000;
int FREEDOM_ANIM_MIN_DIST			= 1000;
int FREEDOM_ANIM_MAX_DIST			= 1500;

int		MAX_ANGLE_PARTION			= 30;
float	MAX_RADIAN_PER_PARTION		= ONE_PI / ( 2 * MAX_ANGLE_PARTION );
float	PET_POP_RICH_ADJUST_HEIGHT	= 20;
FollowCamera* getMainCamera();
tagPetSkillGridDef_ToLua* getPetSkillGridDef( int nGridIndex )
{
	static_assert( sizeof( tagPetSkillGridDef_ToLua ) == sizeof( tagPetSkillGridDef ) );
	return ( tagPetSkillGridDef_ToLua* )&DBDataManager::getSingleton().getPetSkillGridDef( nGridIndex );
}

tagPetDef_ToLua* getPetDef( int nId )
{
	static_assert( sizeof( tagPetDef_ToLua ) == sizeof( tagPetDef ) );
	return ( tagPetDef_ToLua* )&DBDataManager::getSingleton().getPetDef( nId );
}
const tagPetGeniusDef_ToLua* getPetGeniusDef( int nGenius )
{
	static_assert( sizeof( tagPetGeniusDef_ToLua ) == sizeof( tagPetGeniusDef ) );
	return ( const tagPetGeniusDef_ToLua* )DBDataManager::getSingleton().getPetGeniusDef( nGenius );
}
const tagPetStrengthenDef_ToLua* getPetStrengthenDef( int nLv )
{
	static_assert( sizeof( tagPetStrengthenDef_ToLua ) == sizeof( tagPetStrengthenDef ) );
	return ( const tagPetStrengthenDef_ToLua* )DBDataManager::getSingleton().getPetStrengthenDef( nLv );
}
const tagPetMoneyDef_ToLua* getPetMoneyDef( int nImp )
{
	static_assert( sizeof( tagPetMoneyDef ) == sizeof( tagPetMoneyDef_ToLua ) );
	return ( const tagPetMoneyDef_ToLua* )DBDataManager::getSingleton().getPetMoneyDef( nImp );
}

const tagPetImpUpDef_ToLua* getPetImpUpDef( int nImp )
{
	static_assert( sizeof( tagPetImpUpDef ) == sizeof( tagPetImpUpDef_ToLua ) );
	return ( const tagPetImpUpDef_ToLua* )DBDataManager::getSingleton().getPetImpUpDef( nImp );
}

const tagPetImpUpAttrDef_ToLua* getPetImpUpAttrDef(int nImp,int nType )
{
	static_assert( sizeof( tagPetImpUpAttrDef ) == sizeof( tagPetImpUpAttrDef_ToLua ) );
	return ( const tagPetImpUpAttrDef_ToLua* )DBDataManager::getSingleton().getPetImpUpAttrDef( nImp,nType );
}

bool isMaxStrengthenLevel( int nLv )
{
	const tagPetStrengthenDef* pStrengthenDef = DBDataManager::getSingleton().getPetStrengthenDef( nLv );
	return pStrengthenDef == NULL;
}

int getPetStrengthenCost( int nLv )
{
	const tagPetStrengthenDef* pStrengthenDef = DBDataManager::getSingleton().getPetStrengthenDef( nLv );
	if ( pStrengthenDef == NULL )
	{
		return 0;
	}

	return pStrengthenDef->MoneyVal;
}

const tagPetLevelDef_ToLua* getPetLevelDef( int nLv )
{
	static_assert( sizeof( tagPetLevelDef_ToLua ) == sizeof( tagPetLevelDef ) );
	const tagPetLevelDef* pDef = DBDataManager::getSingleton().getPetLevelDef( nLv );
	return ( const tagPetLevelDef_ToLua* )DBDataManager::getSingleton().getPetLevelDef( nLv );
}

const tagPetTraitDef_ToLua* getPetTraitDef( int nTraidId )
{
	static_assert( sizeof( tagPetTraitDef_ToLua ) == sizeof( tagPetTraitDef ) );
	return ( const tagPetTraitDef_ToLua* )DBDataManager::getSingleton().getPetTraitDef( nTraidId );
}

tagCSPetFuseBaby_ToLua				GamePet::m_successRongHeInfo;
std::vector<std::string>			GamePet::m_vecPetFreedomText;

GamePet::GamePet(void) : m_nRoleShiMaxNum(0), m_pOwner(NULL)
{
	m_fuseInviteInfo.MemID		= 0;
	m_rongHePlayerInfo.RoleName[0]	= 0;
	memset( &m_selfRongHePetWID, 0, sizeof( m_selfRongHePetWID ) );

	m_mapRolePet.clear();
	memset( &m_WatchRolePet, 0, sizeof( ROLEPET ) );
	for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
	{
		m_pShiActor[i] = NULL;
	}
	m_lastFree = 0;
}

GamePet::~GamePet(void)
{
	for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
	{
		if( m_pShiActor[i] != NULL )
		{
			ActorManager::getSingleton().DestroyActor( m_pShiActor[i], true );
			m_pShiActor[i] = NULL;
		}
	}
}

void GamePet::breakPetPreMutexAction()
{	
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( pMainPlayer->isInRide() )
	{
		pMainPlayer->m_Rider.requestRideDown();
	}
}

// 准备出征
void GamePet::petPre( int nHightId, int nLowId )
{
	if ( !canCampaignPet( nHightId, nLowId ) )
	{
		return;
	}

	callBackAllPets();
	breakPetPreMutexAction();
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_CAMPAIGN_PRE;
	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody ); 
	m_pOwner->startPetChant( msgbody.PetClt.PetCltData.PetAct.PetWid );
}

// 召回
void GamePet::petBackUp( int nHightId, int nLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_ACT;
	msgbody.PetClt.PetCltData.PetAct.ActTpye	= PET_ACT_BACKUP;
	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	msgbody.PetClt.PetCltData.PetAct.PetWid		= *((tdr_ulonglong*)&petId);
// 	if( isPre() )
// 	{
// 		memset( &m_prePet, 0, sizeof( m_prePet ) );
// 		m_pOwner->setInState( GSACT_IN_PETPRE, false );
// 	}
	SendGameServerMsg( PET_CLT, msgbody );
}

// 放生
void GamePet::petFreedom( int nHightId, int nLowId )
{
	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;
	tdr_ulonglong* petWID = (tdr_ulonglong*)&petId;
	petFreedom( *petWID );
}

// 放生
void GamePet::petFreedom( tdr_ulonglong petWID )
{
	if ( isPetInState( petWID, CLIENT_PET_STAT_FIGHT ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_FREEDOM_IN_FIGHT );
		return;
	}

	tagPetId_ToLua* pPetWID_ToLua = (tagPetId_ToLua*)&petWID;
	LOG_PRIVATE( "petFreedom--nHightId = %d, nLowId = %d--304", pPetWID_ToLua->nHeight, pPetWID_ToLua->nLow );
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType			= PET_CLT_DROP;
	msgbody.PetClt.PetCltData.PetWid	= petWID;
	SendGameServerMsg( PET_CLT, msgbody );
}

// 设置宠物出征战斗模式
void GamePet::setPetFightMode( int nMode,int nHightId, int nLowId )
{
	if ( isPetInFreedom( nHightId, nLowId ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_IN_FREEDOM_ANIL );
		return;
	}

	tagRolePet_ToLua* pPetInfo = getPetInfo( nHightId, nLowId );
	if ( pPetInfo == NULL )
	{
		return;
	}

	pPetInfo->Passive = nMode;

	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;

	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType			= PET_CLT_SET_PASSIVE;
	msgbody.PetClt.PetCltData.ChgMode.Passive	= nMode;
	msgbody.PetClt.PetCltData.ChgMode.PetWID	= *((tdr_ulonglong*)&petId);
	SendGameServerMsg( PET_CLT, msgbody );
}

// 宠物技能学习
void GamePet::petSkillLearn( int nHightId, int nLowId, int nGridIdx, Item& item )
{	
	if ( isPetInFreedom( nHightId, nLowId ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_IN_FREEDOM_ANIL );
		return;
	}

	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;

	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType				= PET_CLT_SKILL_LEARN;
	CSPETSKILLLEARN& petSkillLearnData		= msgbody.PetClt.PetCltData.SkillLearn;
	petSkillLearnData.PetWid				= *((tdr_ulonglong*)&petId);
	petSkillLearnData.SkillGridIdx			= nGridIdx;

	petSkillLearnData.StuffData.ItemID		= item.getItemId();
	petSkillLearnData.StuffData.ListType	= item.getListType();
	petSkillLearnData.StuffData.GridIdx		= item.getGridIdx();
	SendGameServerMsg( PET_CLT, msgbody );
}

// 宠物技能升级
void GamePet::petSkillLevelUp( int nHightId, int nLowId, int nSkillId, int nSkillLv, int nGridIdx, Item& item )
{
	if ( isPetInFreedom( nHightId, nLowId ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_IN_FREEDOM_ANIL );
		return;
	}

	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;

	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType			= PET_CLT_SKILL_UP;
	CSPETSKILLUPDATE& petSkillUpData	= msgbody.PetClt.PetCltData.PetSkillUp;
	petSkillUpData.PetWid				= *((tdr_ulonglong*)&petId);
	petSkillUpData.SkillID				= nSkillId;
	petSkillUpData.Idx					= nGridIdx;

// 	petSkillUpData.StuffData.ItemID		= item.getItemId();
// 	petSkillUpData.StuffData.ListType	= item.getListType();
// 	petSkillUpData.StuffData.GridIdx	= item.getGridIdx();
	SendGameServerMsg( PET_CLT, msgbody );
}

// 宠物洗技能格子
void GamePet::petWashSkillGrid( int nHightId, int nLowId, Item& item, int nGridIdx )
{
	if ( isPetInFreedom( nHightId, nLowId ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_IN_FREEDOM_ANIL );
		return;
	}

	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;

	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType								= PET_CLT_WASH_SKILL_GRID;
	CSPETWASHSKILLGRID& petWashGridData		= msgbody.PetClt.PetCltData.WashSkillGrid;
	petWashGridData.PetWid					= *((tdr_ulonglong*)&petId);
	petWashGridData.SkillGridIdx			= nGridIdx;
	petWashGridData.StuffData.ItemID		= item.getItemId();
	petWashGridData.StuffData.ListType		= item.getListType();
	petWashGridData.StuffData.GridIdx		= item.getGridIdx();
	SendGameServerMsg( PET_CLT, msgbody );
}

bool GamePet::isPetStrthenItem( int nItemID )
{
	return nItemID == RESTREN_ITEMID;
}

// 宠物强化
bool GamePet::petStrengthen( int nHightId, int nLowId, Item& stuffItem, Item* safeItem, int num, int nStrthenLevel )
{
	if ( isPetInFreedom( nHightId, nLowId ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_IN_FREEDOM_ANIL );
		return false;
	}

	tagRolePet_ToLua* pPetInfo = getPetInfo( nHightId, nLowId );
	if ( pPetInfo == NULL )
	{
		return false;
	}

	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;

	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType				= PET_CLT_STRENGTHEN;
	CSPETSTRENGTHEN& petStrengthenData		= msgbody.PetClt.PetCltData.PetStrengthen;
	petStrengthenData.PetWid				= *((tdr_ulonglong*)&petId);
	petStrengthenData.StuffNum				= num;
	petStrengthenData.StrenLvl				= nStrthenLevel;
	
	STUFFIDXDATA& stuff = petStrengthenData.SafeData;
	stuff.ItemID		= stuffItem.getItemId();
	stuff.ListType		= stuffItem.getListType();
	stuff.GridIdx		= stuffItem.getGridIdx();
	
	SendGameServerMsg( PET_CLT, msgbody );

	return true;
}

// 宠物星级重新强化
void GamePet::petResetStrengthen( int nHightId, int nLowId, int nStrthenLevel )
{
	tagRolePet_ToLua* pPetInfo = getPetInfo( nHightId, nLowId );
	if ( pPetInfo == NULL )
	{
		return;
	}

	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;

	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType				= PET_CLT_RESTREN;
	CSPETRESTREN& petResetStrengthenData	= msgbody.PetClt.PetCltData.PetReStren;
	petResetStrengthenData.PetWid			= *( ( tdr_ulonglong* )&petId );
	petResetStrengthenData.StrenLvl			= nStrthenLevel /*pPetInfo->NextStrenLvl*/;
	SendGameServerMsg( PET_CLT, msgbody );
}

// 保存宠物强化数据
void GamePet::petSaveStrengthen( int nHightId, int nLowId, int nVal, int nStrthenLevel )
{
	tagRolePet_ToLua* pRolePetInfo = getPetInfo( nHightId, nLowId );
	if ( pRolePetInfo == NULL )
	{
		return;
	}
	
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_STRENGTHEN_SAVE;
	CSPETSTRENGTHENSAVE& petStrengthenSaveData	= msgbody.PetClt.PetCltData.PetStrengthenSave;
	petStrengthenSaveData.PetID					= pRolePetInfo->PetID;
	memcpy( &petStrengthenSaveData.PetWid, &( pRolePetInfo->WID ), sizeof( pRolePetInfo->WID ) );
	petStrengthenSaveData.Val					= nVal;
	petStrengthenSaveData.StrenLvl				= nStrthenLevel;
	SendGameServerMsg( PET_CLT, msgbody );
}

// 观察他人宠物
void GamePet::petWatch( int nMemId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType				= PET_CLT_WATCH;
	msgbody.PetClt.PetCltData.Watch.MemID	= nMemId;
	SendGameServerMsg( PET_CLT, msgbody ); 
}

// 是否可以修改宠物昵称
bool GamePet::canChangePetName( int nHightId, int nLowId )
{
	if ( isPetInFreedom( nHightId, nLowId ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_IN_FREEDOM_ANIL );
		return false;
	}

	return true;
}

// 修改宠物名
void GamePet::petChangeName( int nHightId, int nLowId, const char* szName )
{
	if ( !canChangePetName( nHightId, nLowId ) )
	{
		return;
	}

	tagPetId_ToLua petId;
	petId.nHeight	= nHightId;
	petId.nLow		= nLowId;

	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType					= PET_CLT_CHG_NAME;
	msgbody.PetClt.PetCltData.ChgName.PetWid	= *((tdr_ulonglong*)&petId);
	memset( msgbody.PetClt.PetCltData.ChgName.PetName, 0, ROLE_NAME_LEN_CLT * sizeof( char ) );
	strncpy( msgbody.PetClt.PetCltData.ChgName.PetName, szName, strlen( szName ) );

	SendGameServerMsg( PET_CLT, msgbody ); 
}

//设置侍卫实体数据
void GamePet::setPetActor( int nXieDaiPos, GameActor* pActor )
{
	assert( nXieDaiPos < PET_MAX_FIGHT_SHI );
	m_pShiActor[nXieDaiPos] = pActor;
}

// 获得侍卫实体
GameActor* GamePet::getPetActor( int nXieDaiPos )
{
	assert( nXieDaiPos < PET_MAX_FIGHT_SHI );
	return m_pShiActor[nXieDaiPos];
}

// 获取处于出征状态的宠物实体
GameActor* GamePet::getPetActor( tdr_ulonglong WID )
{
	for ( int i = 0; i < PET_MAX_FIGHT_SHI; ++i )
	{
		if ( m_pShiActor[i] != NULL && m_pShiActor[i]->getRoleWId() == WID )
		{
			return m_pShiActor[i];
		}
	}

	return NULL;
}

// 召唤回所有的宠物
void GamePet::callBackAllPets()
{
	for ( int i = 0; i < getPetNum(); ++i )
	{
		tagRolePet_ToLua*	pPetInfo = getPetInfo( i );
		if ( isPetInState( i, CLIENT_PET_STAT_FIGHT ) )
		{
			petBackUp( pPetInfo->WID.nHeight, pPetInfo->WID.nLow );
		}
	}
}

// 开启宠物技能格子
void GamePet::addPetSkillGrid( int nHeightId, int nLowId )
{
	tagPetId_ToLua petID;
	petID.nHeight	= nHeightId;
	petID.nLow		= nLowId;
	tdr_ulonglong	WID = *( (tdr_ulonglong*)&petID );
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType						= PET_CLT_ADD_SKILL_GRID;
	msgbody.PetClt.PetCltData.AddSkillGrid.PetWid	= WID;
	
	SendGameServerMsg( PET_CLT, msgbody );
}

// 注册网络消息
void GamePet::initPetNetMessage()
{
	INetworkManager::getSingleton().RegisterMsgHandler( PET_SVR, this );
}
void GamePet::releasePetNetMessage()
{
	INetworkManager::getSingleton().unRegisterMsgHander( PET_SVR );
}

int GamePet::HandleNetMsg(int msgid, const tagCSPkgBody& msgbody)
{
	if( msgid == PET_SVR )
	{
		const CSPETSVR& msg = msgbody.PetSvr;
		switch( msg.PetSvrType )
		{
		case PET_SVR_ACT:
			{
				const CSPETACTINFO& petActionInfo = msg.PetSvrData.PetActInfo;
				char buffer[100];
				sprintf( buffer, "SuccessPetOperation( %d )", petActionInfo.Type );
				ActorManager::getSingleton().getScriptVM()->callString( buffer );
			}
			break;

		case PET_INFO_CHG:
			{
				OnGetPetInfoChg( msg.PetSvrData.PetInfo );
			}
			break;

		case PET_ATTR_CHG:
			{
				ROLEPETMAP::iterator iter = m_mapRolePet.find( msg.PetSvrData.AttrChg.Wid );
				if( iter != m_mapRolePet.end() )
				{
					setPetAttr( iter->second, (CSPETATTRCHG*)&msg.PetSvrData.AttrChg );
					GameMsgPrint::getSingleton().SendUIEvent("GE_PET_ATTR_CHG");
					// 若宠物处于出征状态，再更新场景中宠物的血量
					if ( isPetInState( iter->first, CLIENT_PET_STAT_FIGHT ) )
					{
						GameActor* pPet = getPetActor( iter->first );
						if ( pPet != NULL )
						{
							pPet->reSetHp( iter->second.HP );
							pPet->setMaxHP( iter->second.MaxHP );
							GameMsgPrint::getSingleton().SendUIEvent("GE_PET_HP_CHG");
						}
					}
				}
			}
			break;

		case PET_SVR_DEL:
			{
				ROLEPETMAP::iterator iter		= m_mapRolePet.find( msg.PetSvrData.Del.Wid );
				tdr_ulonglong petWID			= msg.PetSvrData.Del.Wid;
				tagPetId_ToLua* pPetWID_ToLua	= (tagPetId_ToLua*)&petWID;
				LOG_PRIVATE( "GamePet::HandleNetMsg--PET_SVR_DEL--nHeightId = %d, nLowId = %d--607", 
								pPetWID_ToLua->nHeight, pPetWID_ToLua->nLow );
				if( msg.PetSvrData.Del.Flag == 0 )
				{
					addFreedomPet( pPetWID_ToLua->nHeight, pPetWID_ToLua->nLow );	
				}

				// 若宠物处于出征状态，则从场景中删除宠物实体
				if ( isPetInState( iter->first, CLIENT_PET_STAT_FIGHT ) )
				{
					setShiModle( 0, 0, 0, "", 0, 0, 0, 0, PET_OP_BACK );
				}
				
				updatePetIndexDelPet( msg.PetSvrData.Del.Wid );
				if( iter != m_mapRolePet.end() )
				{
					m_mapRolePet.erase(iter);
					GameMsgPrint::getSingleton().SendUIEvent( "GE_DEL_PET" );
				}
			}
			break;

		case PET_SVR_STRENGTHEN:
			{
				const CSPETSVRSTRENGTHEN& petStrengthenInfo = msg.PetSvrData.Strengthen;
				OnGetPetStrengthenInfo( petStrengthenInfo );
			}
			break;

		case PET_SVR_FIGHT_ATTR:
			{
				ROLEPETMAP::iterator iter = m_mapRolePet.find( msg.PetSvrData.PetFightAttr.PetWID );
				if( iter != m_mapRolePet.end() )
				{
					iter->second.FightAttr = msg.PetSvrData.PetFightAttr.Attr;	
					GameMsgPrint::getSingleton().SendUIEvent("GE_PET_FIGHT_CHG");
				}
			}
			break;

		case PET_SVR_WATCH:
			{
				m_strWatchPetOwer	= msg.PetSvrData.WatchInfo.RoleName;
				m_WatchRolePet		= msg.PetSvrData.WatchInfo.Pet;
				ActorManager::getSingleton().getScriptVM()->callString("OpenWatchPetFrame()");
			}
			break;

		case PET_SVR_FUSE_INVITE:
			{
				// 收到邀请融合
				const CSPETFUSEINVITEINFO& petFuseInviteInfo = msg.PetSvrData.FuseInvite;
				OnGetFuseInvite( petFuseInviteInfo );
			}
			break;
		
		case PET_SVR_FUSE_REJECT:
			{
				// 收到拒绝融合
				const CSPETFUSEREJECT& petFuseRejectInfo = msg.PetSvrData.FuseReject;
				OnGetRejectFuse( petFuseRejectInfo );
			}
			break;

		case PET_SVR_FUSE_CANCEL:
			{
				// 收到取消融合
				const CSPETFUSECANCEL& cancelFuseRes = msg.PetSvrData.FuseCancel;
				OnGetCancelFuse( cancelFuseRes );
			}
			break;

		case PET_SVR_FUSE_DEAL:
			{
				// 收到同意融合
				const CSPETFUSEDEAL& dealFuseRes = msg.PetSvrData.FuseDeal;
				OnGetDealFuse( dealFuseRes );
			}
			break;
		
		case PET_SVR_FUSE_UI:
			{
				// 收到对方同意融合邀请的消息
				const CSPETFUSEUI& fuseUIRes = msg.PetSvrData.FuseUI;
				OnGetOtherAcceptInviteRes( fuseUIRes );
			}
			break;

		case PET_SVR_FUSE_SUCC:
			{
				// 收到标志融合是否成功
				const CSPETFUSESUCC& fuseResultRes = msg.PetSvrData.FuseSucc;
				OnGetFuseResultRes( fuseResultRes );
			}
			break;

		case PET_SVR_WASH_GENIUS:
			{
				const tagCSPetWashGeniusInfo &info = msg.PetSvrData.WashGeniusInfo;
				if (info.Succ == 1)
				{
					PetGrowValue value;
					const tagPetId_ToLua *ID = (const tagPetId_ToLua *)&(info.PetWid);
					value.Attack	= info.AtkGenius;
					value.HP		= info.HPGenius;
					value.Defend	= info.PhsicDefGenius;
					value.Resist	= info.IceDefGenius;
					value.Exact		= info.HitRateGenius;
					value.Dodge		= info.FleeGenius;
					ActorManager::getSingleton().getScriptVM()->callFunction("PetGrowOnReset", "iiiu[PetGrowValue]", ID->nHeight, ID->nLow, info.Genius, &value);
				}
				else
				{
					ActorManager::getSingleton().getScriptVM()->callFunction("PetGrowOnReset", "");
				}
			}
			break;

		case PET_SVR_RESET_SKILL:
			{
				const tagCSResetSkillSucc &info = msg.PetSvrData.ResetSkillSucc;
				if (info.Succ == 1)
				{
					const tagPetId_ToLua *wid = ((const tagPetId_ToLua *)&(info.Wid));
					{
						char script[200];
						sprintf(script, "PetSkillOnReset(%d, %d)", wid->nHeight, wid->nLow);
						ActorManager::getSingleton().getScriptVM()->callString(script);
					}
				}
				else
				{
					ActorManager::getSingleton().getScriptVM()->callString("PetSkillOnReset()");
				}
			}
			break;
		case PET_SVR_MOVE_STAR_SUCC:
			{
				int succ = msg.PetSvrData.PetMoveStarSucc.Succ;
				char buff[256];
				sprintf( buff, "PetStarMoveSucc(%d)",succ );
				ActorManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case PET_SVR_EAT_SUCC:
			{
				int succ = msg.PetSvrData.PetEatSucc.Succ;
				/*if( succ == 1 )
				{
					GameMsgPrint::getSingleton().SendUIEvent("GE_PET_EAT_SUCC");
				}*/
				char buff[256];
				sprintf( buff, "PetEatSucc(%d)",succ );
				ActorManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case PET_SVR_STEP_UP:
			{
				int nSucc = msg.PetSvrData.PetStepUp.Succ;
				char buff[256];
				sprintf( buff, "PetStepUpSucc(%d)",nSucc );
				ActorManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case PET_SVR_IMP_UP:
			{
				int nSucc = msg.PetSvrData.PetImpUp.Succ;
				char buff[256];
				sprintf( buff, "PetImpUpSucc(%d)",nSucc );
				ActorManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case PET_SVR_GENIUS_LEARN:
			{
				char buff[256];
				sprintf( buff, "PetGeniusLearnResult(%d)",msg.PetSvrData.PetGeniusLearn.Succ );
				ActorManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		case PET_SVR_WASH_HIGH:
			{
				ActorManager::getSingleton().getScriptVM()->callString( "PetGeniusUpResult()" );
			}
			break;
		case PET_SVR_UNBIND:
			{
				int nSucc = msg.PetSvrData.PetStepUp.Succ;
				char buff[256];
				sprintf( buff, "PetUnbindSucc(%d)",nSucc );
				ActorManager::getSingleton().getScriptVM()->callString( buff );
			}
			break;
		default:
			break;
		}
	}
	return 0;
}

void GamePet::initPetPackage( tagPetPackage* petInfo )
{
	m_nRoleShiMaxNum	= petInfo->MaxNum;
	m_mapRolePet.clear();
	m_mapRolePetIndex.clear();

	static_assert( sizeof(tagRolePet_ToLua) == sizeof( tagRolePet ) );
	for ( int i = 0; i < petInfo->Num; i++ )
	{
		m_mapRolePet[petInfo->RolePets[i].WID]		= petInfo->RolePets[i];
		m_mapRolePetIndex[petInfo->RolePets[i].WID] = i;
	}

	GameMsgPrint::getSingleton().SendUIEvent( "GE_PET_PACKAGE_INIT" );
}

// 宠物栏位数变化
void GamePet::setPetPackageGridInfo( int nMaxNum )
{
	m_nRoleShiMaxNum = nMaxNum;	
	
	GameMsgPrint::getSingleton().SendUIEvent("GE_PET_GRID_CHG");
}

// 设置宠物属性
void GamePet::setPetAttr( ROLEPET& petAttr, CSPETATTRCHG* pdata )
{
	switch( pdata->Type )
	{
	case PET_CHG_NAME:
		{
			memcpy( petAttr.Name, pdata->AttrValue.ChgName, strlen(pdata->AttrValue.ChgName)+1 );
			tagPetId_ToLua* pPetId = (tagPetId_ToLua*)(&petAttr.WID);
			char buffer[256];
			sprintf( buffer, "PetChgName(%d,%d)", pPetId->nHeight, pPetId->nLow );
			ActorManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case PET_CHG_EXP:
		{
			petAttr.Exp = pdata->AttrValue.ChgExp;
			GameMsgPrint::getSingleton().SendUIEvent( "GE_PET_EXP_CHANGE" );
		}
		break;
	case PET_CHG_MAXHP:
		{
			petAttr.MaxHP = pdata->AttrValue.ChgMaxHP.HP;
			petAttr.HP = pdata->AttrValue.ChgMaxHP.HP;
			if( pdata->AttrValue.ChgMaxHP.XieDaiPos >= 0 )
			{
				int nXieDaiPos = pdata->AttrValue.ChgMaxHP.XieDaiPos;
				if( m_pShiActor[nXieDaiPos] != NULL )
				{
					m_pShiActor[nXieDaiPos]->setMaxHP( petAttr.MaxHP );
					m_pShiActor[nXieDaiPos]->reSetHp( petAttr.MaxHP );
				}
			}
		}
		break;
	case PET_CHG_HP:
		{
			petAttr.HP = pdata->AttrValue.ChgHP.HP;
			if( pdata->AttrValue.ChgHP.XieDaiPos >= 0 )
			{
				int nXieDaiPos = pdata->AttrValue.ChgHP.XieDaiPos;
				if( m_pShiActor[nXieDaiPos] != NULL )
				{
					m_pShiActor[nXieDaiPos]->reSetHp( petAttr.HP );
					m_pShiActor[nXieDaiPos]->setMaxHP( petAttr.MaxHP );
				}
				
				tagPetId_ToLua* pPetId = (tagPetId_ToLua*)(&petAttr.WID);
				char buffer[256];
				sprintf( buffer, "PetHPChg(%d,%d,%d)", petAttr.HP, pPetId->nHeight, pPetId->nLow );
				ActorManager::getSingleton().getScriptVM()->callString( buffer );
			}
		}
		break;
	case PET_CHG_LEVEL:
		{
			petAttr.Level = pdata->AttrValue.ChgLevel;
	
			char buffer[50];
			sprintf( buffer, "PetLevelUp(%d )", petAttr.Level );
			ActorManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case PET_CHG_LOVE_VAL:
		{
			petAttr.LoveVal = pdata->AttrValue.ChgLoveVal;
			tagPetId_ToLua* pPetId = (tagPetId_ToLua*)(&petAttr.WID);
			char buffer[256];
			sprintf( buffer, "PetLoveValChg(%d,%d,%d)", petAttr.LoveVal, pPetId->nHeight, pPetId->nLow );
			ActorManager::getSingleton().getScriptVM()->callString( buffer );
		}
		break;
	case PET_CHG_BREEDNUM:
		{
			petAttr.BreedNum = pdata->AttrValue.ChgBreedNum;
		}
		break;
	case PET_CHG_STAT:
		{
			petAttr.StatFlag = pdata->AttrValue.ChgStatFlag;
			GameMsgPrint::getSingleton().SendUIEvent( "GE_PET_STATE_CHANGE" );
		}
		break;
	}
}

void GamePet::setOwner( GameActor* pOwner )
{
	m_pOwner = pOwner;
}

// 设置侍卫模型出征
void GamePet::setShiModle( int nOwnerId, int nXieDaiPos, int nPetId, const char* szPetName, 
						  int nPower, int nMaxPower, int nLevel, tdr_ulonglong PetWID, int nOpCode, 
						  int nPetFightMode, int nDiathesisNum, const tagCSPetDiathesisView* pPetDiathesises,
						  int nGoodNum, const CSROLESTATUS* pGoodList, int nGeniusType, int nStrengthenLevel,
						  int nBattleFlag )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return;
	}

	if ( nOpCode == PET_OP_FIGHT )
	{
		if( nOwnerId == 0 )
		{
			return;
		}

		const tagPetDef& petDef	= DBDataManager::getSingleton().getPetDef( nPetId );
		GameActor* pPetOwner = ActorManager::getSingleton().FindActor( nOwnerId );
		// TODO: 若以后要同时出征多只宠物，则要做改动，当前只处理出征一只宠物的情况
		// 先确认当前要出征的宠物的尸体是否还在地上，若在，将尸体去掉
		GameActor* pPet = m_pShiActor[nXieDaiPos];
		if ( pPet != NULL && pPet->isDead() )
		{
			pPet->OnLeaveMap();
			ActorManager::getSingleton().DestroyActor( pPet, true );
			m_pShiActor[nXieDaiPos] = NULL;
		}
		
		assert( pPetOwner != NULL );
		int nPetDyId			= nOwnerId + ( nXieDaiPos + 1 ) * PET_ID_BASE;	
		m_pShiActor[nXieDaiPos]	= ActorManager::getSingleton().CreateActor( nPetDyId, GAT_PET, petDef.MouldID );
		m_pShiActor[nXieDaiPos]->setName( szPetName );
		m_pShiActor[nXieDaiPos]->OnEnterMap( pMainPlayer->getCurMap() );
		m_pShiActor[nXieDaiPos]->setOwnerName( pPetOwner->getName() );
		m_pShiActor[nXieDaiPos]->setOwnerId( nOwnerId );
		m_pShiActor[nXieDaiPos]->setLv( nLevel );

		m_pShiActor[nXieDaiPos]->setType( GAT_PET );
		m_pShiActor[nXieDaiPos]->setSubType();
		m_pShiActor[nXieDaiPos]->setAtkType();
		m_pShiActor[nXieDaiPos]->ChangeActState( GSACT_STAND );
		m_pShiActor[nXieDaiPos]->setSpeed( 300 );
		m_pShiActor[nXieDaiPos]->setRoleWId( PetWID );
		m_pShiActor[nXieDaiPos]->m_nPetIconId = petDef.IconID;
		m_pShiActor[nXieDaiPos]->m_nPetDefID = petDef.PetID;
		m_pShiActor[nXieDaiPos]->m_nBattePetFlag = nBattleFlag;
		// 设置位置
		setPetPostion( nXieDaiPos );
		// 设置血量
		m_pShiActor[nXieDaiPos]->reSetHp( nPower );
		m_pShiActor[nXieDaiPos]->setMaxHP( nMaxPower );
		
		if( m_pOwner->m_ObjCamp & SKILL_TARGET_FRIEND_PLAYER)
		{ 
			m_pShiActor[nXieDaiPos]->m_ObjCamp = SKILL_TARGET_FRIENDPET;
		}
		else if( m_pOwner->m_ObjCamp & SKILL_TARGET_MID_PLAYER )
		{
			m_pShiActor[nXieDaiPos]->m_ObjCamp = SKILL_TARGET_MIDPET;
		}
		else if( m_pOwner->m_ObjCamp & SKILL_TARGET_SELF )
		{
			m_pShiActor[nXieDaiPos]->m_ObjCamp = SKILL_TARGET_SELFPET;
		}
		else
		{
			m_pShiActor[nXieDaiPos]->m_ObjCamp = SKILL_TARGET_ENEMYPET;
		}

		m_pShiActor[nXieDaiPos]->setPetDiathesis( nDiathesisNum, pPetDiathesises );
		m_pShiActor[nXieDaiPos]->m_BufStatus.setGoodBufStatus( nGoodNum, pGoodList );
		m_pShiActor[nXieDaiPos]->m_nPetGenius			= nGeniusType;
		m_pShiActor[nXieDaiPos]->m_nPetStrengthenLevel	= nStrengthenLevel;
		
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if( m_pOwner == pMainPlayer )
		{
			char szScript[256];
			sprintf( szScript, "UpdateTargetPetUI(%d)", nXieDaiPos );
			ActorManager::getSingleton().getScriptVM()->callString( szScript );
			SCTDamageManager::getSingleton().addDamageActor( nPetDyId );
		}
		
		m_pShiActor[nXieDaiPos]->setPetFightMode( nPetFightMode );
		GameMsgPrint::getSingleton().updateActorPetStatus( m_pOwner, true );

		m_pOwner->finishChant( ePet_Chant );
		
		ActorManager& actorMgr		= ActorManager::getSingleton(); 
		int nHideOtherPlayerOption	= actorMgr.getShowOtherPlayerModelOption();

		if( nHideOtherPlayerOption != HIDE_MODEL_NONE )
		{
			if ( m_pOwner->getType() == GAT_MAINPLAYER /*|| pMainPlayer->isInSameTeam( m_pOwner->getTeamWID() )*/ )
			{
				return;
			}

			Ogre::ShowActor* pPetShowActor = m_pShiActor[nXieDaiPos]->getShowActor();
			GameMap* pMapMgr = m_pShiActor[nXieDaiPos]->getCurMap();
			pPetShowActor->onLeaveMap( pMapMgr->getShowGameMap() );
		}
		return;
	}
	else
	{
		if( nOwnerId == 0 )
		{
			GameActor* pPet = m_pShiActor[nXieDaiPos];
			if ( pPet == NULL )
			{
				return;
			}

			if ( m_pOwner == pMainPlayer && pPet->getHP() == 0 )
			{
				m_pShiActor[nXieDaiPos]->ChangeActState( GSACT_DEAD );			
				GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_PET_DEAD" );
			}
			else
			{
				m_pShiActor[nXieDaiPos]->OnLeaveMap();
				ActorManager::getSingleton().DestroyActor( m_pShiActor[nXieDaiPos], true );
				m_pShiActor[nXieDaiPos] = NULL;
				GameMsgPrint::getSingleton().updateActorPetStatus( m_pOwner, false );
			}
			if( m_pOwner == pMainPlayer )
			{
				GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_PET_CALLBACK" );
			}
			if ( m_pOwner->GetID() == pMainPlayer->GetSelectTarget() )
			{
				GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_PET_CALLBACK" );
			}
			return;
		}
	}
	assert(0);
}

// 改变宠物出征模式或者名字
void GamePet::changePetNameOrFightMode( const CSPETVIEW& fightPetServeData )
{
	const char* pszPetName = fightPetServeData.PetName;
	if ( pszPetName[0] == 0 )
	{
		const tagPetDef_ToLua* pPetDef = getPetDef( fightPetServeData.PetID );
		pszPetName = pPetDef->Name;
	}
	
	GameActor* pFightPet = m_pShiActor[fightPetServeData.XieDaiPos];
	pFightPet->setName( pszPetName );
	pFightPet->setPetFightMode( fightPetServeData.Passive );
}

void GamePet::SetGeniusInfo( GameActor* pFightPet, int GeniusId, int GeniusLv )
{
	pFightPet->m_nPetGeniusId			= GeniusId;
	pFightPet->m_nPetGenniusLv			= GeniusLv;
	int nOwnerId				= pFightPet->getOwnerId();
	GameActor* pOwner			= ActorManager::getSingleton().FindActor( nOwnerId );
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	if ( pOwner == pMainPlayer )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_PET_CALLBACK" );
	}
}

bool GamePet::tryMove( GameActor* pFightPet, int nDestX, int nDestY, int nCurX, int nCurY )
{
	int nMaskBit = MASK_STOP;
	Point2D end( nDestX, nDestY );
	GameMap* pMapControl	= pFightPet->getCurMap();
	GameMaskMap* pMaskMap	= pMapControl->getMaskMap();

	Point2D scale( pMaskMap->m_lMaskPixelWidth, pMaskMap->m_lMaskPixelHeight );
	Point2D s_EndGrid = end / scale;
	if( !pMaskMap->canWalk(s_EndGrid.x, s_EndGrid.y,nMaskBit) )
	{
		if( !pMaskMap->findPointWalkable( end, nMaskBit ) )
		{
			return false;
		}else{
			nDestX	= end.x;
			nDestY	= end.y; 
		}
	}
	
	GameMaskPath path;
	if( !pMapControl->FindPath(path, nCurX, nCurY, nDestX, nDestY, nMaskBit, true ) )
	{
		return false;
	}
	
	pFightPet->m_pMapMover->StartOnPath(&path);
	pFightPet->ChangeActState( GSACT_MOVE );
	return true;
}

// 强化属性提升值是否变化
bool GamePet::isStrthenValChg( const char* pszLeftVals, const char* pszRighVals )
{
	for ( int i = 0; i < MAX_PET_STRENGTHEN_LEVEL; ++i )
	{
		if ( pszLeftVals[i] != pszRighVals[i] )
		{
			return true;
		}
	}

	return false;
}

int GamePet::getChgStrthenLevel( const char* pszLeftVals, const char* pszRighVals )
{
	for ( int i = 0; i < MAX_PET_STRENGTHEN_LEVEL; ++i )
	{
		if ( pszLeftVals[i] != pszRighVals[i] )
		{
			return i;
		}
	}

	return -1;
}

// 是否是重置强化星级
bool GamePet::isResetStrthenLevel( int nNewCanStrthenLevel, int nOldCanStrthenLevel )
{
	return nNewCanStrthenLevel != nOldCanStrthenLevel;
}

void GamePet::generateRandomMovePoint( int& nDestX, int& nDestY, int nCurPosX, int nCurPosY, int nRangeIndex )
{
	const float CUR_MIN_RADIAN = nRangeIndex * MAX_RADIAN_PER_PARTION;
	const float CUR_MAX_RADIAN = ( nRangeIndex + 1 ) * MAX_RADIAN_PER_PARTION;

	const int SCALE		= 10000;
	float fCurRadian	= atan2( (float)nCurPosX, (float)nCurPosY );
	srand( timeGetTime() );
	int nMoveDist = rand() % ( FREEDOM_ANIM_MAX_DIST - FREEDOM_ANIM_MIN_DIST ) + FREEDOM_ANIM_MIN_DIST;

	int nNewRandomRadian	= rand() % (int)( MAX_RADIAN_PER_PARTION * SCALE ) + (int)( CUR_MIN_RADIAN * SCALE );
	float fCurLength		= sqrt( (float)nCurPosX * nCurPosX + (float)nCurPosY * nCurPosY );

	float fNewLength = fCurLength + nMoveDist;
	float fNewRadian = (float)nNewRandomRadian / SCALE;
	if ( fNewRadian < 0 )
	{
		fNewRadian = 0;
	}

	nDestX = static_cast<int>( fNewLength * cos( fNewRadian ) );
	if ( nDestX < 0 )
	{
		nDestX = 0;
	}

	nDestY = static_cast<int>( fNewLength * sin( fNewRadian ) );
	if ( nDestY < 0 )
	{
		nDestY = 0;
	}
}

void GamePet::clientSelfPetMove( GameActor* pPetActor )
{
	GameMaskPath path;
	int nCurX, nCurY;
	pPetActor->m_pMapMover->GetCurPos( nCurX, nCurY );
	int nDestX = 0, nDestY = 0;

	for ( int nPartionIndex = 0, maxPartion = MAX_ANGLE_PARTION-1; nPartionIndex < maxPartion; ++nPartionIndex )
	{
		generateRandomMovePoint( nDestX, nDestY, nCurX, nCurY, nPartionIndex );
		// tido:
		/*nDestX = 27200;
		nDestY = 26000;*/
		if ( tryMove( pPetActor, nDestX, nDestY, nCurX, nCurY ) )
		{
			break;
		}
	}
}

// 添加要放生的宠物
void GamePet::addFreedomPet( int nHeightId, int nLowId )
{
	tagRolePet_ToLua* pPetInfo = getPetInfo( nHeightId, nLowId );
	if ( pPetInfo == NULL )
	{
		return;
	}

	//if ( isPetInState( pPetInfo->WID, CLIENT_PET_STAT_FIGHT ) )
	//{
	//	GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_FREEDOM_IN_FIGHT );
	//	return;
	//}

	const tagPetDef_ToLua* pPetDef = getPetDef( pPetInfo->PetID );
	LOG_PRIVATE( "addFreedomPet--nHeightId = %d, nLowId = %d--peDyntName = %s--petDefName=%s--1140",nHeightId, nLowId,
					pPetInfo->Name, pPetDef->Name );
	//petFreedom( nHeightId, nLowId );
	GameActor* pFreedomPetActor = new GameNPC;
	pFreedomPetActor->setType( GAT_PET );
	pFreedomPetActor->LoadRes( pPetDef->MouldID );

	tdr_ulonglong* pPetWID = (tdr_ulonglong*)( &pPetInfo->WID );
	pFreedomPetActor->setRoleWId( *pPetWID );
	pFreedomPetActor->OnEnterMap( m_pOwner->getCurMap() );
	pFreedomPetActor->ChangeActState( GSACT_STAND );
	// 一般设定为300
	pFreedomPetActor->setSpeed( DEFAULT_CLIENT_PET_FREEDOM_SPEED );
	pFreedomPetActor->m_nPetDefID	= pPetDef->PetID;
	setPetPostion( pFreedomPetActor );
	clientSelfPetMove( pFreedomPetActor );
	
	tagFreedomPetInfo freedomPet;
	freedomPet.m_uStartFreeTime = timeGetTime();
	freedomPet.m_falpha			= 1.0f;
	m_freedomPets[pFreedomPetActor] = freedomPet;
	
	std::string strPetPopText = "主人我要走了，再见！";
	if ( !m_vecPetFreedomText.empty() )
	{
		srand( timeGetTime() );
		int nAllTextNum = m_vecPetFreedomText.size();
		int nRandonTextIndex = rand() % nAllTextNum;
		strPetPopText = m_vecPetFreedomText[nRandonTextIndex];
	}
	char szPopName[256];
	sprintf( szPopName, "%d, %d, %d", m_pOwner->GetID(), nHeightId, nLowId );
	const char* pszDrawBack = "ChatPop";
	const char* pszArrowTex = "PopBackArrowTexture";
	bool bShowArrowTex = true;
	int nOffsetX = 12;
	int nOffsetY = 0;
	g_pFrameMgr->addPopWindow(	pszDrawBack, szPopName, strPetPopText.c_str(), nOffsetX, nOffsetY, FREEDOM_ANIM_TIME / 1000, 
									pszArrowTex,  bShowArrowTex, 0, 0 );
}

// 宠物是否正在放生中
bool GamePet::isPetInFreedom( int nHeightId, int nLowId )
{
	for ( std::map<GameActor*, tagFreedomPetInfo>::iterator iter = m_freedomPets.begin(), end = m_freedomPets.end();
			iter != end; ++iter )
	{
		GameActor* pFreedomPet = iter->first;
		tdr_ulonglong petWID = pFreedomPet->getRoleWId();
		tagPetId_ToLua* pPetWID_ToLua = ( tagPetId_ToLua* )&petWID;
		if ( nHeightId == pPetWID_ToLua->nHeight && nLowId == pPetWID_ToLua->nLow )
		{
			return true;
		}
	}

	return false;
}

// 获得某个强化等级的提升值
int	GamePet::getStrthenVal( tagRolePet_ToLua* pOnePetInfo, int nIndex )
{
	return (int)pOnePetInfo->StrengthenInfo[nIndex];
}

void GamePet::UpdateFreedomPetPopRich( FollowCamera *pcamera )
{
	unsigned int nCurTime = timeGetTime();
	for ( std::map<GameActor*, tagFreedomPetInfo>::iterator iter = m_freedomPets.begin(), end = m_freedomPets.end(); 
		iter != end; ++iter )
	{
		GameActor* pFreedomPet = iter->first;
		const tagFreedomPetInfo& oneFreePet = iter->second;
		Ogre::ShowActor* pShowActor = pFreedomPet->getShowActor();

		Ogre::Vector3 pos = pShowActor->getCurEntity()->getWorldPosition();
		pos.y += pShowActor->getActorHeight();
		Ogre::Vector3 scrpos;
		bool bFront = pcamera->GetCameraNode()->pointWorldToWindow( scrpos, pos );
		if ( !bFront )
		{
			continue;
			//return;
		}
		
		float fScale = g_pFrameMgr->GetScreenScaleX() < g_pFrameMgr->GetScreenScaleY() ?
						g_pFrameMgr->GetScreenScaleX() : g_pFrameMgr->GetScreenScaleY();
		
		tdr_ulonglong petWID = pFreedomPet->getRoleWId();
		tagPetId_ToLua* pPetWID = (tagPetId_ToLua*)( &petWID );
		char szFlyActorName[256];
		sprintf( szFlyActorName, "%d, %d, %d", m_pOwner->GetID(), pPetWID->nHeight, pPetWID->nLow );
		std::map<std::string, tagPopWin>::iterator richIter = g_pFrameMgr->m_PopWinMap.find( szFlyActorName );
		if( richIter != g_pFrameMgr->m_PopWinMap.end() )
		{
			scrpos.x += richIter->second.m_PopOffset.x;
			scrpos.y += richIter->second.m_PopOffset.y; 
		}
		g_pFrameMgr->updatePopWinPos( szFlyActorName, scrpos.x , scrpos.y + PET_POP_RICH_ADJUST_HEIGHT * fScale, 
										oneFreePet.m_falpha );
	}
}

// 收回宠物
void GamePet::callBackPet( const CSPETVIEW& fightPetServeData )
{
	int nXieDaiPos	= fightPetServeData.XieDaiPos;
	GameActor* pPet = m_pShiActor[nXieDaiPos];
	if ( pPet == NULL )
	{
		return;
	}
	
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( m_pOwner == pMainPlayer && pPet->getHP() == 0 )
	{
		pPet->ChangeActState( GSACT_DEAD );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_PET_CALLBACK" );

		if ( m_pOwner->GetID() == pMainPlayer->GetSelectTarget() )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_PET_CALLBACK" );
		}
	}
	else
	{
		pPet->OnLeaveMap();
		ActorManager::getSingleton().DestroyActor( pPet, true );
		m_pShiActor[nXieDaiPos] = NULL;

		if ( m_pOwner->GetID() == pMainPlayer->GetSelectTarget() )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_PET_CALLBACK" );
		}

		if( m_pOwner == pMainPlayer )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_PET_CALLBACK" );
		}

		GameMsgPrint::getSingleton().updateActorPetStatus( m_pOwner, false );
	}
}

// 重新显示侍卫模型
void GamePet::reShowModle()
{
	for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
	{
		if( m_pShiActor[i] != NULL )
		{
			setPetPostion( i );
			m_pShiActor[i]->ChangeActState(GSACT_STAND); 
		}
	}
}

// 设置宠物位置
void GamePet::setPetPostion( GameActor* pPetActor )
{
	if ( m_pOwner == NULL )
	{
		return;
	}

	if( pPetActor->getCurMap() != m_pOwner->getCurMap() )
	{
		pPetActor->OnEnterMap( m_pOwner->getCurMap() );
	}
	int x = m_pOwner->GetPosition().x/10 /*+ MIN_DIST*/;
	int z = m_pOwner->GetPosition().z/10 /*+ MIN_DIST*/;
	pPetActor->SetPosition( x, z );

	GameMapMover* pMapMoveControl = pPetActor->m_pMapMover;
	pMapMoveControl->GetCurPos(x, z);
	x*=10;
	z*=10;

	int y = 0;
	Ogre::ShowActor* pShowActor = pPetActor->getShowActor();
	GameMap* pMapControl		= pPetActor->getCurMap();
	pMapControl->getTerrainH(x, z, y);
	y += pShowActor->getActorOffsetZ();
	//cout << "setPetPostion( GameActor* pPetActor ) y = " << y << "，x = " << x << " ，z = " << z << endl; 
	pShowActor->setPosition( x, y, z );



	/*pShowActor->update(dtick);
	GameMapMover* pMapMoveControl = pFreedomPet->m_pMapMover;
	pMapMoveControl->Update(Ogre::TickToTime(dtick));
	int x, y, z;
	pMapMoveControl->GetCurPos(x, z);
	x*=10;
	z*=10;

	GameMap* pMapControl	= pFreedomPet->getCurMap();
	pMapControl->getTerrainH(x, z, y);
	y += pShowActor->getActorOffsetZ();

	pShowActor->setPosition( x, y, z );*/
}

void GamePet::setPetPostion( int nXieDaiPos )
{
	if( m_pOwner != NULL )
	{
		if( m_pShiActor[nXieDaiPos]->getCurMap() != m_pOwner->getCurMap() )
		{
			m_pShiActor[nXieDaiPos]->OnEnterMap( m_pOwner->getCurMap() );
		}
		int x = m_pOwner->GetPosition().x/10 + MIN_DIST;
		int y = m_pOwner->GetPosition().z/10 + MIN_DIST;
		m_pShiActor[nXieDaiPos]->SetPosition( x, y );
	}
}

// 设置宠物移动位置
void GamePet::setMoveToPoint( int to_x, int to_z )
{
	m_movePoint.x = to_x;
	m_movePoint.z = to_z;
}

// 宠物移动
void GamePet::moveTo( int nXiDaiPos, int to_x, int to_z )
{
	GameActor* pActor = m_pShiActor[nXiDaiPos];
	if (pActor->getSpeed() == 0)
	{
		return;
	}
	GameMap *curMap = pActor->getCurMap();// 判断宠物是否在当前地图
	if (curMap == NULL)
	{
		return;
	}
	int x = to_x;
	int y = to_z;
	if (!m_pOwner->m_pMapMover->isReach())
	{
		Ogre::Point2D pos = m_pOwner->m_pMapMover->m_CurPath.getPathPoint(m_pOwner->m_pMapMover->m_CurPath.getNumSegment());
		x = pos.x;
		y = pos.y;
	}
	GameMaskPath path;
	int cur_x, cur_z;
	pActor->m_pMapMover->GetCurPos(cur_x, cur_z);
	for (int i = 0; i < 3; ++i)
	{
		float r = pow(rand() / float(RAND_MAX), 1 / 3.0f) * maxr;
		float angle = rand() / float(RAND_MAX) * MY_PI * 2;
		int offx = int(r * cos(angle));
		int offy = int(r * sin(angle));
		int tox = x + offx;
		int toy = y + offy;
		bool reach = true;
		if( curMap->getMaskMap()->lineOfSightTest(Ogre::Point2D(cur_x, cur_z), Ogre::Point2D(tox, toy), MASK_STOP|DYN_MASK_STOP) )
		{
			x = tox;
			y = toy;
			break;
		}
	}

	if( cur_x == x && cur_z == y )
	{
		return;
	}
	path.reset(curMap->getMaskMap());
	path.addPathPoint(cur_x, cur_z);
	if (!m_pOwner->m_pMapMover->isReach())
	{
		path.addPathPoint(to_x, to_z);
		for (size_t i = m_pOwner->m_pMapMover->m_CurSeg + 1; i < m_pOwner->m_pMapMover->m_CurPath.getNumPoint(); ++i)
		{
			Ogre::Point2D pos = m_pOwner->m_pMapMover->m_CurPath.getPathPoint(i);
			path.addPathPoint(pos.x, pos.y);
		}
	}
	path.addPathPoint(x, y);

	pActor->m_pMapMover->StartOnPath(&path);
	pActor->ChangeActState(GSACT_MOVE);
}

// 刷新宠物移动
void GamePet::updatePetMoveSpe()
{	
	if (m_pOwner == NULL || m_pOwner->isDead())
	{
		return;
	}
	for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
	{
		GameActor* pPet = m_pShiActor[i];
		if ( pPet == NULL || pPet->isDead() || pPet->m_isHeti)
		{
			continue;
		}
		if( m_pOwner->isInFly() )
		{
			if( pPet->getCurMap() != NULL )
			{
				pPet->OnLeaveMap();
			}
			continue;
		}
		if( pPet->getCurMap() == NULL )
		{
			pPet->OnEnterMap( m_pOwner->getCurMap() );
		}

		GameXYZ pos2	= pPet->GetPosition();
		GameXYZ pos		= m_pOwner->GetPosition();
		int nDis		= (int)pos2.subtract(pos).length();

		if( nDis > 700 )
		{
			float r = pow(rand() / float(RAND_MAX), 1 / 3.0f) * maxr;
			float angle = rand() / float(RAND_MAX) * MY_PI * 2;
			pPet->onStop( pos.x/10 + int(r * cos(angle)), pos.z/10 + int(r * sin(angle)) );
		}
		else
		{
			if ((pPet->getActState().GetState() == GSACT_STAND || nDis >= 450) && !pPet->isInSkillTetanic())
			{
				if (nDis >= 250)
				{
					moveTo( i, pos.x/10, pos.z/10 );
				}
				else
				{
					unsigned int now = timeGetTime();
					if (m_lastFree == 0)
					{
						m_lastFree = now + rand() % 5000 + 5000;
					}
					else if (now > m_lastFree)
					{
						m_lastFree = 0;
						moveTo( i, pos.x/10, pos.z/10 );
					}
				}
			}

			if (nDis > 400)
			{
				pPet->setSpeed( m_pOwner->getSpeed() + 100 );
			}
			else if (nDis >= 200)
			{
				pPet->setSpeed( max(m_pOwner->getSpeed() - 100, 100) );
			}
			else
			{
				pPet->setSpeed( 100 );
			}
		}
	}
}

void GamePet::FreedomPetOnLeaveMap()
{
	for ( std::map<GameActor*, tagFreedomPetInfo>::iterator iter = m_freedomPets.begin(), end = m_freedomPets.end(); 
		iter != end;  )
	{
		GameActor* pFreedomPet = iter->first;

		if ( pFreedomPet == NULL )
		{
			m_freedomPets.erase( iter++ );
			continue;
		}
		
		tdr_ulonglong petWID			= pFreedomPet->getRoleWId();
		tagPetId_ToLua* pPetWID_ToLua	= (tagPetId_ToLua*)&petWID;
		pFreedomPet->OnLeaveMap();
		delete pFreedomPet;
		m_freedomPets.erase( iter++ );
	}
}

// 刷新放生宠物动画
void GamePet::UpdateFreedomPetAnim( unsigned int dtick )
{
	unsigned int nCurTime = timeGetTime();
	for ( std::map<GameActor*, tagFreedomPetInfo>::iterator iter = m_freedomPets.begin(), end = m_freedomPets.end(); 
		iter != end; )
	{
		GameActor* pFreedomPet = iter->first;
		tagFreedomPetInfo& oneFreePet = iter->second;
		if ( pFreedomPet == NULL )
		{
			m_freedomPets.erase( iter++ );
			continue;
		}

		if ( nCurTime - oneFreePet.m_uStartFreeTime > FREEDOM_ANIM_TIME )
		{
			pFreedomPet->OnLeaveMap();
			tdr_ulonglong petWID			= pFreedomPet->getRoleWId();
			tagPetId_ToLua* pPetWID_ToLua	= (tagPetId_ToLua*)&petWID;
			delete pFreedomPet;
			m_freedomPets.erase( iter++ );
			continue;
		}

		float fAlpha = (float)( nCurTime - oneFreePet.m_uStartFreeTime ) / FREEDOM_ANIM_TIME;
		fAlpha = 1.0f - fAlpha;
		if ( fAlpha > 1.0f )
		{
			fAlpha = 1.0f;
		}

		if ( fAlpha < 0.0f )
		{
			fAlpha = 0.0f;
		}
		oneFreePet.m_falpha = fAlpha;

		Ogre::ShowActor* pShowActor = pFreedomPet->getShowActor();

		pShowActor->getCurEntity()->setTransparent( fAlpha ); 
		pFreedomPet->Update( dtick );
		++iter;
	}
}


// 删除宠物的时候更新索引信息
void GamePet::updatePetIndexDelPet( tdr_ulonglong delPetWid )
{
	ROLEPETINDEXMAP::iterator indexIter = m_mapRolePetIndex.find( delPetWid );
	int nDelIndex = 0;
	if ( indexIter == m_mapRolePetIndex.end() )
	{
		return;
	}

	nDelIndex = indexIter->second;
	m_mapRolePetIndex.erase( indexIter );
	for ( ROLEPETINDEXMAP::iterator iter = m_mapRolePetIndex.begin(), end = m_mapRolePetIndex.end(); iter != end; ++iter )
	{
		if ( iter->second > nDelIndex )
		{
			--( iter->second );
		}
	}
}

// 获取宠物信息
tagRolePet_ToLua* GamePet::getPetInfo( int nIndex )
{
	/*assert( nIndex>=0 && nIndex < MAX_PET_PACKAGE );
	static_assert( sizeof(tagRolePet_ToLua) == sizeof( tagRolePet ) );
	ROLEPETMAP::iterator iter = m_mapRolePet.begin();
	int nCount = 0;
	while( iter != m_mapRolePet.end() )
	{
		if( nCount == nIndex )
		{
			return ( tagRolePet_ToLua* )&(iter->second);
		}
		nCount++;
		iter++;
	}
	return NULL;*/
	static_assert( sizeof(tagRolePet_ToLua) == sizeof( tagRolePet ) );
	tdr_ulonglong destPetWID = 0;
	for ( ROLEPETINDEXMAP_ITER indexIter = m_mapRolePetIndex.begin(), end = m_mapRolePetIndex.end(); 
			indexIter != end; ++indexIter )
	{
		if ( indexIter->second == nIndex )
		{
			return getPetInfo( indexIter->first );
		}
	}

	return NULL;
}

// 获取宠物信息
tagRolePet_ToLua* GamePet::getPetInfo( tdr_ulonglong WID )
{
	ROLEPETMAP::iterator iter = m_mapRolePet.find( WID );
	if ( iter != m_mapRolePet.end() )
	{
		return ( tagRolePet_ToLua* )&(iter->second);
	}

	return NULL;
}

// 获取宠物信息
tagRolePet_ToLua* GamePet::getPetInfo( int nHeightId, int nLowId )
{
	tagPetId_ToLua petWID;
	petWID.nHeight	= nHeightId;
	petWID.nLow		= nLowId;
	return getPetInfo( *(tdr_ulonglong*)&petWID );
}

tagRolePet_ToLua* GamePet::getPetInfo(GameActor *pActor)
{
	if (pActor == NULL)
	{
		return NULL;
	}
	return getPetInfo(pActor->getRoleWId());
}

// 获取宠物在服务器上对应索引值
int	GamePet::getPetIndex( tagRolePet_ToLua* pPetInfo )
{
	tdr_ulonglong petWID = *( ( tdr_ulonglong* )( &( pPetInfo->WID ) ) );
	//memcpy( &petWID, &( pPetInfo->WID ), sizeof( tdr_ulonglong ) );
	ROLEPETINDEXMAP_ITER indexIter = m_mapRolePetIndex.find( petWID );
	if ( indexIter != m_mapRolePetIndex.end() )
	{
		return indexIter->second;
	}
	else
	{
		return -1;
	}

	/*int nIndex = 0;
	for ( ROLEPETMAP_ITER iter = m_mapRolePet.begin(), end = m_mapRolePet.end(); iter != end; ++iter )
	{
		if ( iter->first == petWID )
		{
			return nIndex;
		}
		++nIndex;
	}

	return -1;*/
}

// 获取被观察宠物信息
tagRolePet_ToLua* GamePet::getWatchPetInfo()
{
	static_assert( sizeof(tagRolePet_ToLua) == sizeof( tagRolePet ) );
	return ( tagRolePet_ToLua* )&m_WatchRolePet;
}

// 获取宠物静态信息
tagPetDef_ToLua* GamePet::getPetDef( int nId )
{
	static_assert( sizeof( tagPetDef_ToLua ) == sizeof( tagPetDef ) );
	return ( tagPetDef_ToLua* )&DBDataManager::getSingleton().getPetDef( nId );
}

// 判断宠物的状态
bool GamePet::isPetInState( int nIndex, int nState )
{
	tagRolePet_ToLua* pPetInfo = getPetInfo( nIndex );
	if ( pPetInfo == NULL )
	{
		return false;
	}
	bool bResult = ( pPetInfo->StatFlag & nState ) != 0;
	return  ( pPetInfo->StatFlag & nState ) != 0;
}

//  判断是否有空余栏位存放宠物
bool GamePet::isPetFull()
{
	return getPetNum() >= getPetMaxNum();
}

// 判断是否能够继续出征宠物
bool GamePet::canCampaignPet( int nHightId, int nLowId )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if ( m_pOwner != pMainPlayer )
	{
		return false;
	}

	if ( isPetInFreedom( nHightId, nLowId ) )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_FAIL_IN_FREEDOM_ANIL );
		return false;
	}

	if ( !pMainPlayer->canUsedSpell() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_SOME_STATE );
		return false;
	}

	/*if ( pMainPlayer->isInFight() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_FIGHT );
		return false;
	}*/
	

	if( SkillManager::getSingleton().getSpellControler()->getCurSpell() != NULL)
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_SKILLPRE );
		return false;
	}

	/*pSpell = SkillManager::getSingleton().getSpellControler()->getCurPilSpell();
	if( pSpell != NULL)
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_OTHER );
		return false;
	}*/
	
	if ( pMainPlayer->isInStall() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_STALL );
		return false;
	}

	if( pMainPlayer->getActState().GetState() == GSACT_COLLECTITEM )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_COLLECT );
		return false;
	}

	if ( pMainPlayer->isOnMonsterMachine() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_IN_RIDE_MACHINE );
		return false;
	}


	Rider* pRider = pMainPlayer->getRider();
	if ( pRider->isOnHorse() && !pRider->isOwnRide() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_IN_OTHER_RIDE );
		return false;
	}

	if(!pMainPlayer->canControl() )
	{
		return false;
	}

	GameMap *map = pMainPlayer->getCurMap();
	if (map == NULL)
	{
		return false;
	}
	const tagMapDef &mapdef = DBDataManager::getSingleton().getMapDef(map->getRealId());
	if( mapdef.CtrlFlag & MAP_CTRL_NO_PET_FIGHT )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_PET_MAP_NO_PET_FIGHT );
		return false;
	}
	int pworldId = PworldLimitInfo::getSingleton().getCurPworldId();
	if (pworldId != 0)
	{
		const tagPworldDef *pworldDef = DBDataManager::getSingleton().getPworldDef(pworldId);
		if( pworldDef->CtrlFlag & PWORLD_NO_PET_FIGHT )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_PET_MAP_NO_PET_FIGHT );
			return false;
		}
	}

	return pMainPlayer->canStartChant( ePet_Chant );
}

// 查看是否处于吟唱状态
bool GamePet::isPre()
{
	return m_pOwner->isInPre( ePet_Chant ) ;
}

bool GamePet::isPetStrenthenItem( int nStrengthenLvel, int nItemID )
{
	const tagPetStrengthenDef_ToLua* pOnePetStrengthenDef = getPetStrengthenDef( nStrengthenLvel );
	if ( pOnePetStrengthenDef == NULL )
	{
		return false;
	}

	if ( nItemID == 0 )
	{
		return false;
	}

	return nItemID == pOnePetStrengthenDef->UseItemID;
}

// 判断宠物的状态
bool GamePet::isPetInState( int nHeightId, int nLowId, int nState )
{
	tagPetId_ToLua WID;
	WID.nHeight = nHeightId;
	WID.nLow	= nLowId;
	return isPetInState( WID, nState );
}

// 判断宠物的状态
bool GamePet::isPetInState( tagPetId_ToLua& WID, int nState )
{
	for ( ROLEPETMAP_ITER iter = m_mapRolePet.begin(), end = m_mapRolePet.end(); iter != end; ++iter )
	{
		if ( iter->second.WID == ( *(tdr_ulonglong*)&( WID ) ) )
		{
			return  ( iter->second.StatFlag & nState ) != 0;
		}
	}

	assert( false );
	return false;
}

// 判断宠物的状态
bool GamePet::isPetInState( tdr_ulonglong WID, int nState )
{
	ROLEPETMAP_ITER iter = m_mapRolePet.find( WID );
	if ( iter == m_mapRolePet.end() )
	{
		return false;
	}

	return  ( iter->second.StatFlag & nState ) != 0;
}

// 邀请融合
void GamePet::inviteFuse( int nDestPetMemID )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType			= PET_CLT_FUSE_INVITE;
	CSPETFUSEINVITE& petFuseInviteData	= msgbody.PetClt.PetCltData.FuseInvite;
	petFuseInviteData.MemID				= nDestPetMemID;
	
	SendGameServerMsg( PET_CLT, msgbody );
}

// 同意融合
void GamePet::acceptFuseInvite( int nDestPlayerMemID )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType			= PET_CLT_FUSE_ACCEPT;
	CSPETFUSEACCEPT& petFuseAcceptData	= msgbody.PetClt.PetCltData.FuseAccept;
	petFuseAcceptData.MemID				= nDestPlayerMemID;
	
	SaveRongHePetInfo();
	//callBackAllPets();
	m_fuseInviteInfo.MemID = 0;
	SendGameServerMsg( PET_CLT, msgbody );
}

// 拒绝融合
void GamePet::rejectFuse( int nDestPlayerMemID )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType			= PET_CLT_FUSE_REJECT;
	CSPETFUSEREJECT& petFuseRejectData	= msgbody.PetClt.PetCltData.FuseReject;
	petFuseRejectData.MemID				= nDestPlayerMemID;
	
	m_fuseInviteInfo.MemID = 0;
	SendGameServerMsg( PET_CLT, msgbody );
}

// 取消融合
void GamePet::cancelFuse( int nDestPlayerMemID )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType			= PET_CLT_FUSE_CANCEL;
	CSPETFUSECANCEL& petFuseCancelData	= msgbody.PetClt.PetCltData.FuseCancel;
	petFuseCancelData.MemID				= nDestPlayerMemID;
	
	SendGameServerMsg( PET_CLT, msgbody );
}

// 同意融合
void GamePet::agreeFuse( int nDestPlayerMemID )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType			= PET_CLT_FUSE_DEAL;
	CSPETFUSEDEAL& petFuseDealData		= msgbody.PetClt.PetCltData.FuseDeal;
	petFuseDealData.MemID				= nDestPlayerMemID;
	
	SendGameServerMsg( PET_CLT, msgbody );
}

const tagCSPetFuseInviteInfo_ToLua& GamePet::getPetFuseInviteInfo()
{
	static_assert( sizeof( tagCSPetFuseInviteInfo_ToLua ) == sizeof( tagCSPetFuseInviteInfo ) );
	return m_fuseInviteInfo;
}

const tagCSPetFuseUI_ToLua& GamePet::getRongHePlayerInfo()
{
	return m_rongHePlayerInfo;
}

tagPetId_ToLua& GamePet::getSelfRongHePetWID()
{
	return m_selfRongHePetWID;
}

void GamePet::clearSelfRongHePetInfo()
{
	m_selfRongHePetWID.nHeight	= 0;
	m_selfRongHePetWID.nLow		= 0;
}

tagCSPetFuseBaby_ToLua& GamePet::getSuccessFuseInfo()
{
	return m_successRongHeInfo;
}

// 收到宠物信息变化回复
void GamePet::OnGetPetInfoChg( const ROLEPET& newPetInfo )
{

	ROLEPETMAP::iterator iter = m_mapRolePet.find( newPetInfo.WID );
	if ( iter != m_mapRolePet.end() )
	{
		OnUpdatePetInfo( iter->second, newPetInfo );
	}
	else
	{
		OnGetNewPet( newPetInfo );
	}

	GameMsgPrint::getSingleton().SendUIEvent("GE_PET_INFO_CHG");
}

void GamePet::OnGetNewPet( const ROLEPET& newPetInfo )
{
	m_mapRolePet[newPetInfo.WID]		= newPetInfo;
	m_mapRolePetIndex[newPetInfo.WID]	= (int)m_mapRolePet.size()-1;
	tagPetId_ToLua petWID;
	memcpy( &petWID, &newPetInfo.WID, sizeof( tagPetId_ToLua ) );
	char buffer[100];
	sprintf( buffer, "GetNewPet( %d, %d )", petWID.nHeight, petWID.nLow );
	ActorManager::getSingleton().getScriptVM()->callString( buffer );
	GameMsgPrint::getSingleton().SendUIEvent( "GE_GET_NEW_PET" );
}

void GamePet::OnUpdatePetInfo( ROLEPET& oldPetInfo, const ROLEPET& newPetInfo )
{
	bool bStrthenalChg		= isStrthenValChg( oldPetInfo.StrengthenInfo, newPetInfo.StrengthenInfo );
	int nChgLevel			= getChgStrthenLevel( oldPetInfo.StrengthenInfo, newPetInfo.StrengthenInfo ) + 1;
	bool bResetStrthenLevel	= isResetStrthenLevel( newPetInfo.NextStrenLvl, oldPetInfo.NextStrenLvl );
	int	 nOldCanStrthenLevel= oldPetInfo.NextStrenLvl;
	oldPetInfo = newPetInfo;
	/*if ( bStrthenalChg )
	{*/
		GameMsgPrint::getSingleton().SendUIEvent( "GE_PET_STRENGTHEN_CHG" );
		tagPetId_ToLua petWID;
		memcpy( &petWID, &newPetInfo.WID, sizeof( tagPetId_ToLua ) );
		char buffer[100];
		sprintf( buffer, "GetPetSuccessStrthen( %d, %d, %d )", petWID.nHeight, petWID.nLow, nOldCanStrthenLevel );
		ActorManager::getSingleton().getScriptVM()->callString( buffer );
	/*}*/

	if ( bResetStrthenLevel )
	{
		if ( newPetInfo.NextStrenLvl != 0 && newPetInfo.NextStrenLvl != ( newPetInfo.StrengthenLevel + 1 ) )
		{
			sprintf( buffer, "GetPetResetStrthen( %d, %d, %d )", petWID.nHeight, petWID.nLow, newPetInfo.NextStrenLvl );
			ActorManager::getSingleton().getScriptVM()->callString( buffer );
		}
	}

	// 若宠物处于出征状态，再更新场景中宠物的血量
	if ( !isPetInState( oldPetInfo.WID, CLIENT_PET_STAT_FIGHT ) )
	{
		return;
	}

	GameActor* pPet = getPetActor( oldPetInfo.WID );
	if ( pPet == NULL )
	{
		return;
	}

	pPet->reSetHp( oldPetInfo.HP );
	pPet->setMaxHP( oldPetInfo.MaxHP );
	pPet->setPetDiathesis( oldPetInfo.PetSkills.SkillGridNum, (const PETSKILLGRID*)&oldPetInfo.PetSkills.SkillGrid );
	GameMsgPrint::getSingleton().SendUIEvent( "GE_PET_HP_CHG" );
}

// 收到宠物强化回复
void GamePet::OnGetPetStrengthenInfo( const CSPETSVRSTRENGTHEN& petStrengthenInfo )
{
	tagPetId_ToLua *petWID = ((tagPetId_ToLua *)&(petStrengthenInfo.Wid));
	char szScript[256];
	sprintf( szScript, "PetStrengthenRes( %d, %d, %d, %d )", petWID->nHeight, petWID->nLow, petStrengthenInfo.SuccFlag,
		petStrengthenInfo.StrenLvl );
	ActorManager::getSingleton().getScriptVM()->callString( szScript );
}

// 保存融合的宠物WID
void GamePet::SaveRongHePetInfo()
{
	GameActor* pPetActor = m_pShiActor[0];
	if ( pPetActor == NULL )
	{
		memset( &m_selfRongHePetWID, 0, sizeof( m_selfRongHePetWID ) );
		return;
	}

	tagRolePet_ToLua* pPetInfo = getPetInfo( pPetActor->getRoleWId() );
	m_selfRongHePetWID = pPetInfo->WID;
}

// 收到邀请融合
void GamePet::OnGetFuseInvite( const CSPETFUSEINVITEINFO& petFuseInviteRes )
{
	static_assert( sizeof( tagCSPetFuseInviteInfo_ToLua ) == sizeof( tagCSPetFuseInviteInfo ) );
	
	// 发过来的内存ID邀请玩家的内存ID和玩家的名字
	memcpy( &m_fuseInviteInfo, &petFuseInviteRes, sizeof( tagCSPetFuseInviteInfo_ToLua ) );
	GameMsgPrint::getSingleton().SendUIEvent("GE_GET_PET_FUSE_INVITE");
}	

// 收到拒绝融合
void GamePet::OnGetRejectFuse( const CSPETFUSEREJECT& rejectFuseRes )
{
	char szScript[256] = { 0 };
	
	sprintf_s( szScript, ARRAY_ELEMENTS( szScript ), "CallScript_GetRejectFuseRes(%d)", rejectFuseRes.MemID );
	ActorManager::getSingleton().getScriptVM()->callString( szScript );
}

// 收到取消融合
void GamePet::OnGetCancelFuse( const CSPETFUSECANCEL& cancelFuseRes )
{
	char szScript[256] = { 0 };
	
	sprintf_s( szScript, ARRAY_ELEMENTS( szScript ), "CallScript_GetCancelFuseRes(%d)", cancelFuseRes.MemID );
	ActorManager::getSingleton().getScriptVM()->callString( szScript );
}

// 收到同意融合
void GamePet::OnGetDealFuse( const CSPETFUSEDEAL& dealFuseRes )
{
	char szScript[256] = { 0 };
	sprintf_s( szScript, ARRAY_ELEMENTS( szScript ), "CallScript_GetDealFuseRes(%d)", dealFuseRes.MemID );
	ActorManager::getSingleton().getScriptVM()->callString( szScript );
	//GameMsgPrint::getSingleton().SendUIEvent("GE_GET_PET_FUSE_AGREE_RONG_HE");
}

// 收到对方同意融合邀请的消息
void GamePet::OnGetOtherAcceptInviteRes( const CSPETFUSEUI& fuseUIRes )
{
	SaveRongHePetInfo();
	/*callBackAllPets();*/
	static_assert( sizeof( tagCSPetFuseUI_ToLua ) == sizeof( tagCSPetFuseUI ) );
	// 发过来的RoleName是同意邀请的玩家名字
	memcpy( &m_rongHePlayerInfo, &fuseUIRes, sizeof( tagCSPetFuseUI_ToLua ) );
	
	GameMsgPrint::getSingleton().SendUIEvent("GE_GET_OTHER_ACCEPT_INVITE_RES");
}

// 收到标志融合是否成功
void GamePet::OnGetFuseResultRes( const CSPETFUSESUCC& fuseResultRes )
{
	static_assert( sizeof( tagCSPetFuseBaby_ToLua ) == sizeof( tagCSPetFuseBaby ) );
	char szScript[256] = { 0 };
	const CSPETFUSEBABY& allPetFuseBabyData = fuseResultRes.Baby;
	CSPETFUSEBABY* pConvertTypeDestPetData = (CSPETFUSEBABY*)&m_successRongHeInfo;
	pConvertTypeDestPetData->MemID	= allPetFuseBabyData.MemID;
	pConvertTypeDestPetData->Pet	= allPetFuseBabyData.Pet;

	pConvertTypeDestPetData->OtherMemID	= allPetFuseBabyData.OtherMemID;
	pConvertTypeDestPetData->OtherPet	= allPetFuseBabyData.OtherPet;
	//memcpy( &m_successRongHeInfo, &fuseResultRes.Baby, sizeof( tagCSPetFuseBaby_ToLua ) );
	sprintf_s( szScript, ARRAY_ELEMENTS( szScript ), "CallScript_GetResultFuseRes(%d)", fuseResultRes.Succ );
	
	ActorManager::getSingleton().getScriptVM()->callString( szScript );
}

// 设置出征宠物的属性
void GamePet::setFightPetAttr( const CSPETVIEW& fightPetServeData )
{
	GameActor* pFightPet			= m_pShiActor[fightPetServeData.XieDaiPos];
	const tagPetDef_ToLua* pPetDef	= getPetDef( fightPetServeData.PetID );
	const char* pszPetName = fightPetServeData.PetName;
	if( pszPetName[0] == 0 )
	{
		pszPetName = pPetDef->Name;
	}

	pFightPet->setName( pszPetName );
	pFightPet->OnEnterMap( m_pOwner->getCurMap() );
	pFightPet->setOwnerName( m_pOwner->getName() );
	pFightPet->setOwnerId( m_pOwner->GetID() );
	pFightPet->setLv( fightPetServeData.Level );

	pFightPet->setType( GAT_PET );
	pFightPet->setSubType();
	pFightPet->setAtkType();
	pFightPet->ChangeActState( GSACT_STAND );
	pFightPet->setSpeed( 300 );
	pFightPet->setRoleWId( fightPetServeData.PetWID );
	pFightPet->m_nPetIconId = pPetDef->IconID;
	pFightPet->m_nPetDefID	= pPetDef->PetID;
	// 设置位置
	setPetPostion( fightPetServeData.XieDaiPos );
	// 设置血量
	pFightPet->reSetHp( fightPetServeData.Power );
	pFightPet->setMaxHP( fightPetServeData.MaxPower );

	setFightPetFightMode( pFightPet );

	pFightPet->setPetDiathesis( fightPetServeData.DiathesisNum, fightPetServeData.Diathesis );
	pFightPet->m_BufStatus.setGoodBufStatus( fightPetServeData.GoodStatusNum, fightPetServeData.GoodStatusList );
	pFightPet->m_nPetGenius			= fightPetServeData.Genius;
	pFightPet->m_nPetStrengthenLevel= fightPetServeData.StrengthenLevel;
	pFightPet->m_nBattePetFlag		= fightPetServeData.Zhanchong;
	pFightPet->setPetFightMode( fightPetServeData.Passive );
}

// 设置出征宠物的战斗模式
void GamePet::setFightPetFightMode( GameActor* pFightPet )
{
	if( m_pOwner->m_ObjCamp & SKILL_TARGET_FRIEND_PLAYER)
	{ 
		pFightPet->m_ObjCamp = SKILL_TARGET_FRIENDPET;
	}
	else if( m_pOwner->m_ObjCamp & SKILL_TARGET_MID_PLAYER )
	{
		pFightPet->m_ObjCamp = SKILL_TARGET_MIDPET;
	}
	else if( m_pOwner->m_ObjCamp & SKILL_TARGET_SELF )
	{
		pFightPet->m_ObjCamp = SKILL_TARGET_SELFPET;
	}
	else
	{
		pFightPet->m_ObjCamp = SKILL_TARGET_ENEMYPET;
	}
}

void GamePet::loadPetConfig()
{
	const char* pPetCofigFile = "uires\\Scripts\\Config\\Pet.xml";
	Ogre::XMLData xmldata;
	if( !xmldata.loadFile( pPetCofigFile ) )
	{
		return;
	}

	XMLNode pRootNode = xmldata.getRootNode();

	XMLNode pChildNode = pRootNode.iterateChild();
	while ( !pChildNode.isNull() )
	{
		if( stricmp( pChildNode.getName(), "PetFreeAnim" ) == 0 )
		{

			DEFAULT_CLIENT_PET_FREEDOM_SPEED	= pChildNode.attribToInt( "speed" );
			FREEDOM_ANIM_TIME					= pChildNode.attribToInt( "animTime" ) * 1000;
			FREEDOM_ANIM_MIN_DIST				= pChildNode.attribToInt( "minMoveDist" );
			FREEDOM_ANIM_MAX_DIST				= pChildNode.attribToInt( "maxMoveDist" );
			MAX_ANGLE_PARTION					= pChildNode.attribToInt( "maxAnglePartion" );
			PET_POP_RICH_ADJUST_HEIGHT			= pChildNode.attribToFloat( "adjustPopRichHeight" );
			MAX_RADIAN_PER_PARTION				= ONE_PI / ( 2 * MAX_ANGLE_PARTION );
			XMLNode pFreeAnimsChild = pChildNode.iterateChild();
			while ( !pFreeAnimsChild.isNull() )
			{
				if ( stricmp( pFreeAnimsChild.getName(), "PopRich" ) == 0 )
				{
					const char* pszText = pFreeAnimsChild.attribToString( "text" );
					if ( pszText != NULL )
					{
						m_vecPetFreedomText.push_back( pszText );
					}
				}
				pFreeAnimsChild = pChildNode.iterateChild( pFreeAnimsChild );
			}
		}

		pChildNode = pRootNode.iterateChild(pChildNode);
	}
}

bool GamePet::isPetResetSkillLockUseCoin(tagRolePet_ToLua *info)
{
	return info && (info->MiscFlag & PET_MISC_FLAG_USE_JINBI);
}

int GamePet::getPetResetSkillLockNum(tagRolePet_ToLua *info)
{
	return info ? info->LockNum : 0;
}

int GamePet::getPetResetSkillLockIdx(tagRolePet_ToLua *info, int idx)
{
	return info ? info->LockIdxs[idx] : 0;
}

void GamePet::LockPetSkill(int high, int low, int idx, bool usecoin)
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_LOCK_SKILL;
	tagPetId_ToLua info;
	info.nHeight = high;
	info.nLow = low;
	memcpy(&(msgbody.PetClt.PetCltData.PetLockSkill.PetWid), &info, sizeof(tdr_ulonglong));
	msgbody.PetClt.PetCltData.PetLockSkill.Idx = idx;
	msgbody.PetClt.PetCltData.PetLockSkill.Lock = 1;
	msgbody.PetClt.PetCltData.PetLockSkill.UseType = (usecoin ? 0 : 1);
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::UnLockPetSkill(int high, int low, int idx)
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_LOCK_SKILL;
	tagPetId_ToLua info;
	info.nHeight = high;
	info.nLow = low;
	memcpy(&(msgbody.PetClt.PetCltData.PetLockSkill.PetWid), &info, sizeof(tdr_ulonglong));
	msgbody.PetClt.PetCltData.PetLockSkill.Idx = idx;
	msgbody.PetClt.PetCltData.PetLockSkill.Lock = 0;
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::ResetPetSkill(int high, int low)
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_SKILL_RESET;
	tagPetId_ToLua info;
	info.nHeight = high;
	info.nLow = low;
	memcpy(&(msgbody.PetClt.PetCltData.PetSkillReset.PetWid), &info, sizeof(tdr_ulonglong));
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::LevelUpPetSkill(int high, int low, int idx)
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_SKILL_UP;
	tagPetId_ToLua info;
	info.nHeight = high;
	info.nLow = low;
	memcpy(&(msgbody.PetClt.PetCltData.PetSkillUp.PetWid), &info, sizeof(tdr_ulonglong));
	msgbody.PetClt.PetCltData.PetSkillUp.Idx = idx;
	SendGameServerMsg(PET_CLT, msgbody);
}

int GamePet::getPetResetSkillCostMoney()
{
	const tagUseDef *UseDef = DBDataManager::getSingleton().getUseDef(USE_ID_PETSKILL_RESET);
	if (UseDef == NULL)
	{
		return 0;
	}
	return UseDef->UseMoney;
}

int GamePet::getPetResetSkillCostMoney(int num, bool usecoin)
{
	const tagUseDef *UseDef = DBDataManager::getSingleton().getUseDef(USE_ID_PETSKILL_LOCK);
	if (UseDef == NULL)
	{
		return 0;
	}
	int result;
	if (usecoin)
	{
		result = UseDef->UseJinbi;
	}
	else
	{
		result = UseDef->UseJinquan;
	}
	return result * (1 << (num - 1) * 2);
}

int GamePet::getPetSkillLevelUpCostItemCount(int id, int level)
{
	return level;
}

void GamePet::setPetHeti(tdr_ulonglong WID,int nPetGenius , bool heti, int nBattleFlag )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	int hetiIdx = -1;
	if (heti)
	{
		for (size_t i = 0; i < PET_MAX_FIGHT_SHI; ++i)
		{
			if (m_pShiActor[i] != NULL && m_pShiActor[i]->getRoleWId() == WID)
			{
				hetiIdx = i;
				m_pShiActor[i]->m_BufStatus.petClearBuff();
				m_pShiActor[i]->OnLeaveMap();
				if (m_pShiActor[i]->m_isHeti != heti)
				{
					m_pShiActor[i]->m_isHeti = heti;
					onPetHeti(m_pShiActor[i], heti);
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < PET_MAX_FIGHT_SHI; ++i)
		{
			if (m_pShiActor[i] != NULL && m_pShiActor[i]->getRoleWId() == WID)
			{
				m_pShiActor[i]->OnEnterMap(pMainPlayer->getCurMap());
				if (m_pShiActor[i]->m_isHeti != heti)
				{
					m_pShiActor[i]->m_isHeti = heti;
					onPetHeti(m_pShiActor[i], heti);
				}
			}
		}
	}
	if (heti)
	{	
		if ( nBattleFlag == 1 || ( hetiIdx > 0 && m_pShiActor[hetiIdx]->m_nBattePetFlag == 1 ) )
		{
			setBattlePetEntity( m_pOwner, nPetGenius );
		}
		else
		{
			setPetHetiEntity( m_pOwner, nPetGenius );
		}
	}
	else
	{
		m_pOwner->getShowActor()->getCurEntity()->stopMotion(PET_HETI_SKILL_ID * 100);
	}
	if (m_pOwner->getType() == GAT_MAINPLAYER)
	{
		GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_BUF_LIST_UPDATE");
		GameMsgPrint::getSingleton().SendUIEvent("GE_MAIN_PET_HETI");
	}
	if (m_pOwner->GetID() == pMainPlayer->GetSelectTarget())
	{
		// update TargetUI
		GameMsgPrint::getSingleton().SendUIEvent("GE_TARGET_BUF_LIST_UPDATE");
		GameMsgPrint::getSingleton().SendUIEvent("GE_TARGET_PET_HETI");
	}
}

void GamePet::setPetHetiEntity( GameActor* pOwner,int nPetGenius )
{
	if ( nPetGenius == CLIENT_PET_GENIUS_B )
	{
		pOwner->getShowActor()->getCurEntity()->playMotion("32300_8", false, PET_HETI_SKILL_ID * 100);
	}
	else if ( nPetGenius == CLIENT_PET_GENIUS_A )
	{
		pOwner->getShowActor()->getCurEntity()->playMotion("32303_8", false, PET_HETI_SKILL_ID * 100);
	}
	else if ( nPetGenius == CLIENT_PET_GENIUS_S )
	{
		pOwner->getShowActor()->getCurEntity()->playMotion("32302_8", false, PET_HETI_SKILL_ID * 100);
	}
}

void GamePet::setBattlePetEntity( GameActor* pOwner,int nPetGenius )
{
	if ( nPetGenius == CLIENT_PET_GENIUS_B )
	{
		pOwner->getShowActor()->getCurEntity()->playMotion("32304_8", false, PET_HETI_SKILL_ID * 100);
	}
	else if ( nPetGenius == CLIENT_PET_GENIUS_A )
	{
		pOwner->getShowActor()->getCurEntity()->playMotion("32305_8", false, PET_HETI_SKILL_ID * 100);
	}
	else if ( nPetGenius == CLIENT_PET_GENIUS_S )
	{
		pOwner->getShowActor()->getCurEntity()->playMotion("32306_8", false, PET_HETI_SKILL_ID * 100);
	}
}

int GamePet::getPetHeti()
{
	for (size_t i = 0; i < PET_MAX_FIGHT_SHI; ++i)
	{
		if (m_pShiActor[i] != NULL && m_pShiActor[i]->m_isHeti)
		{
			return i;
		}
	}
	return -1;
}

bool GamePet::isPetSkillCanLevelUp(int id, int level)
{
	const tagDiathesisDef *DiathesisDef = &DBDataManager::getSingleton().getDiathesisDef(id, level + 1);
	return DiathesisDef != NULL;
}

void GamePet::onPetHeti(GameActor* pPetActor, bool heti)
{
}

void GamePet::onPetHeti(bool heti)
{
	if (!heti)
	{
		m_pOwner->playMotion("32301", 60100);
	}
}

const tagUseDef * GamePet::getPetGrowResetCostMoneyUseDef(int high, int low)
{
	tagRolePet_ToLua *info = getPetInfo(high, low);
	if (info == NULL)
	{
		return NULL;
	}
	int idx = USE_ID_PET_WASH_GENIUS_S;
	switch (info->PetGenius)
	{
	case PET_GENIUS_D:
		idx = USE_ID_PET_WASH_GENIUS_D;
		break;
	case PET_GENIUS_C:
		idx = USE_ID_PET_WASH_GENIUS_C;
		break;
	case PET_GENIUS_B:
		idx = USE_ID_PET_WASH_GENIUS_B;
		break;
	case PET_GENIUS_A:
		idx = USE_ID_PET_WASH_GENIUS_A;
		break;
	case PET_GENIUS_S:
		idx = USE_ID_PET_WASH_GENIUS_S;
		break;
	}
	return DBDataManager::getSingleton().getUseDef(idx);
}

int GamePet::getPetGrowResetCostMoney(int high, int low)
{
	const tagUseDef *UseDef = getPetGrowResetCostMoneyUseDef(high, low);
	if (UseDef == NULL)
	{
		return 0;
	}
	return UseDef->UseMoney;
}

int GamePet::getPetGrowResetCostMoney(int high, int low, int locknum, bool usecoin)
{
	const tagUseDef *UseDef = getPetGrowResetCostMoneyUseDef(high, low);
	if (UseDef == NULL)
	{
		return 0;
	}
	int cost;
	if (usecoin)
	{
		cost = UseDef->UseJinbi;
	}
	else
	{
		cost = UseDef->UseJinquan;
	}
	return cost * 2 * locknum;
}

PetGrowRange GamePet::getPetGrowResetRange( int id,int nImp )
{
	const tagPetDef &PetDef = DBDataManager::getSingleton().getPetDef(id);
	const tagPetWashGeniusDef &	GeniusDef		= *(DBDataManager::getSingleton().getPetWashGeniusDef(PetDef.Important,PET_GENIUS_S));
	const tagPetImpUpAttrDef&	ImpUpAttrDef	= *( DBDataManager::getSingleton().getPetImpUpAttrDef(nImp, PetDef.PetAttrType) );
	double minscale = 0;
	double maxscale = (100 + GeniusDef.WashRange[GeniusDef.MaxList-1].GeniusMul ) / 100.0;
	const tagPetGenius &info1 = ImpUpAttrDef.Genius[PET_GENIUS_D];
	const tagPetGenius &info2 = ImpUpAttrDef.Genius[PET_GENIUS_S];
	PetGrowRange range;
	range.Min.Attack = (int)info1.PhsicAtkGenius * minscale;
	range.Max.Attack = (int)info2.PhsicAtkGenius * maxscale;
	range.Min.HP = (int)info1.HPGenius * minscale;
	range.Max.HP = (int)info2.HPGenius * maxscale;
	range.Min.Defend = (int)info1.DefGenius * minscale;
	range.Max.Defend = (int)info2.DefGenius * maxscale;
	range.Min.Resist = (int)info1.IceDefGenius * minscale;
	range.Max.Resist = (int)info2.IceDefGenius * maxscale;
	range.Min.Exact = (int)info1.HitRateGenius * minscale;
	range.Max.Exact = (int)info2.HitRateGenius * maxscale;
	range.Min.Dodge = (int)info1.FleeGenius * minscale;
	range.Max.Dodge = (int)info2.FleeGenius * maxscale;
	return range;
}

PetGrowRange GamePet::getPetGrowResetRange(tagRolePet_ToLua *info)
{
	return getPetGrowResetRange(info->PetID,info->Imp);
}

PetGrowRange GamePet::getPetBlinkMinRange( int nGrade, int nPetID, int nImp )
{
	const tagPetDef &PetDef = DBDataManager::getSingleton().getPetDef(nPetID);
	const tagPetWashGeniusDef &	GeniusDef		= *( DBDataManager::getSingleton().getPetWashGeniusDef(PetDef.Important, nGrade) );
	const tagPetImpUpAttrDef&	ImpUpAttrDef	= *( DBDataManager::getSingleton().getPetImpUpAttrDef(nImp, PetDef.PetAttrType) );
	PetGrowRange range;
	if( GeniusDef.MaxList <= 1 )
	{
		range.Min.Attack = 0;
		range.Min.HP = 0;
		range.Min.Defend = 0;
		range.Min.Resist= 0;
		range.Min.Exact = 0;
		range.Min.Dodge = 0;
	}else{
		double scale = (100 + GeniusDef.WashRange[GeniusDef.MaxList-2].GeniusMul ) / 100.0;
		const tagPetGenius &genius = ImpUpAttrDef.Genius[nGrade];
		range.Min.Attack	= scale*genius.PhsicAtkGenius;
		range.Min.HP		= scale*genius.HPGenius;
		range.Min.Defend	= scale*genius.DefGenius;
		range.Min.Resist	= scale*genius.IceDefGenius;
		range.Min.Exact		= scale*genius.HitRateGenius;
		range.Min.Dodge		= scale*genius.FleeGenius;
	}
	return range;
}

PetGrowRange GamePet::getPetGrowResetLimit(int id, int nImp )
{
	return getPetGrowResetRange( id,nImp );
// 	const tagPetDef &PetDef = DBDataManager::getSingleton().getPetDef(id);
// 	const tagPetWashGeniusDef &GeniusDef = *(DBDataManager::getSingleton().getPetWashGeniusDef(PetDef.Important));
// 	int idx = 0;
// 	switch (genius)
// 	{
// 	case PET_GENIUS_D:
// 		idx = 0;
// 		break;
// 	case PET_GENIUS_C:
// 		idx = 1;
// 		break;
// 	case PET_GENIUS_B:
// 		idx = 2;
// 		break;
// 	case PET_GENIUS_A:
// 		idx = 3;
// 		break;
// 	case PET_GENIUS_S:
// 		idx = 4;
// 		break;
// 	}
// 	int min = 0, max = 0;
// 	for (int i = 0; i < MAX_WASH_RANGE_LIST; ++i)
// 	{
// 		const tagWashRangeInfo &info = GeniusDef.WashRange[i];
// 		if (info.AddMinVal < min)
// 		{
// 			min = info.AddMinVal;
// 		}
// 		if (info.AddMaxnVal > max)
// 		{
// 			max = info.AddMaxnVal;
// 		}
// 	}
// 	double minscale = (100 + min) / 100.0;
// 	double maxscale = (100 + max) / 100.0;
// 	const tagPetGenius &info = PetDef.Genius[idx];
// 
// 	PetGrowRange range;
// 
// 	range.Min.Attack = (int)info.PhsicAtkGenius * minscale;
// 	range.Max.Attack = (int)info.PhsicAtkGenius * maxscale;
// 	range.Min.HP = (int)info.HPGenius * minscale;
// 	range.Max.HP = (int)info.HPGenius * maxscale;
// 	range.Min.Defend = (int)info.DefGenius * minscale;
// 	range.Max.Defend = (int)info.DefGenius * maxscale;
// 	range.Min.Resist = (int)info.IceDefGenius * minscale;
// 	range.Max.Resist = (int)info.IceDefGenius * maxscale;
// 	range.Min.Exact = (int)info.HitRateGenius * minscale;
// 	range.Max.Exact = (int)info.HitRateGenius * maxscale;
// 	range.Min.Dodge = (int)info.FleeGenius * minscale;
// 	range.Max.Dodge = (int)info.FleeGenius * maxscale;
// 
// 	return range;
}

PetGrowRange GamePet::getPetGrowResetLimit(tagRolePet_ToLua *info)
{
	return getPetGrowResetLimit(info->PetID, info->Imp);
}

PetGrowValue GamePet::getPetGrowValue(tagRolePet_ToLua *info)
{
	PetGrowValue value;
	value.Attack = info->PhsicAtkGenius;
	value.HP = info->HPGenius;
	value.Defend = info->DefGenius;
	value.Resist = info->IceDefGenius;
	value.Exact = info->HitRateGenius;
	value.Dodge = info->FleeGenius;
	return value;
}

PetGrowValue GamePet::getPetGrowValue(int high, int low)
{
	tagRolePet_ToLua *info = getPetInfo(high, low);
	if (info == NULL)
	{
		PetGrowValue value;
		memset(&value, 0, sizeof(value));
		return value;
	}
	return getPetGrowValue(info);
}

void GamePet::ResetPetGrow(int high, int low)
{
	ResetPetGrow(high, low, 0, false);
}

void GamePet::ResetPetGrow(int high, int low, int lock, bool usecoin)
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_WASH_GENIUS;
	tagPetId_ToLua info;
	info.nHeight = high;
	info.nLow = low;
	memcpy(&(msgbody.PetClt.PetCltData.PetWashGenius.PetWid), &info, sizeof(tdr_ulonglong));
	msgbody.PetClt.PetCltData.PetWashGenius.LockFlag = lock;
	msgbody.PetClt.PetCltData.PetWashGenius.ItemID = PET_GROW_RESET_ITEM;
	if (lock == 0)
	{
		msgbody.PetClt.PetCltData.PetWashGenius.MoneyType = PET_WASH_NORMAL;
	}
	else if (usecoin)
	{
		msgbody.PetClt.PetCltData.PetWashGenius.MoneyType = PET_WASH_BIND_GODCOIN;
	}
	else
	{
		msgbody.PetClt.PetCltData.PetWashGenius.MoneyType = PET_WASH_GODCOIN;
	}
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::ResetPetGrowSave(int high, int low)
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_WASH_GENIUS_SAVE;
	tagPetId_ToLua info;
	info.nHeight = high;
	info.nLow = low;
	memcpy(&(msgbody.PetClt.PetCltData.PetWashGeniusSave.PetWid), &info, sizeof(tdr_ulonglong));
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::petFreeHeti(int high, int low)
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_HETI_END;
	tagPetId_ToLua info;
	info.nHeight = high;
	info.nLow = low;
	memcpy(&(msgbody.PetClt.PetCltData.PetHetiEnd.PetWid), &info, sizeof(tdr_ulonglong));
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::loveLevelUp( int high, int low )
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.PetClt.PetCltType = PET_CLT_LOVE_LEVEL_UP;
	tagPetId_ToLua info;
	info.nHeight = high;
	info.nLow = low;
	memcpy(&(msgbody.PetClt.PetCltData.PetLoveLevelUp.PetWid), &info, sizeof(tdr_ulonglong));
	SendGameServerMsg( PET_CLT, msgbody );
}

void GamePet::startEatPet( int nSrcHightId, int nSrcLowId ,int nDesHightId, int nDesLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_EAT;
	tagPetId_ToLua srcInfo;
	tagPetId_ToLua desInfo;
	srcInfo.nHeight	= nSrcHightId;
	srcInfo.nLow	= nSrcLowId;
	desInfo.nHeight = nDesHightId;
	desInfo.nLow	= nDesLowId;
	memcpy(&(msgbody.PetClt.PetCltData.PetEat.SrcPetWid), &srcInfo, sizeof(tdr_ulonglong));
	memcpy(&(msgbody.PetClt.PetCltData.PetEat.DstPetWid), &desInfo, sizeof(tdr_ulonglong));
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::startMoveStar( int nSrcHightId, int nSrcLowId ,int nDesHightId, int nDesLowId )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_MOVE_STAR;
	tagPetId_ToLua srcInfo;
	tagPetId_ToLua desInfo;
	srcInfo.nHeight	= nSrcHightId;
	srcInfo.nLow	= nSrcLowId;
	desInfo.nHeight = nDesHightId;
	desInfo.nLow	= nDesLowId;
	memcpy(&(msgbody.PetClt.PetCltData.PetMoveStar.SrcPetWid), &srcInfo, sizeof(tdr_ulonglong));
	memcpy(&(msgbody.PetClt.PetCltData.PetMoveStar.DstPetWid), &desInfo, sizeof(tdr_ulonglong));
	SendGameServerMsg(PET_CLT, msgbody);
}

bool GamePet::isExtraBattlePet( tagRolePet_ToLua * info )
{
	return info && ( info->MiscFlag & PET_MISC_FLAG_COMBAT );
}

bool GamePet::isExtraClanJiYangPet( tagRolePet_ToLua * info )
{
	return info && ( info->MiscFlag & PET_MISC_FLAG_JIYANG );
}

int GamePet::getBattlePetNum()
{
	int nNum = 0;
	for ( ROLEPETMAP_ITER iter = m_mapRolePet.begin(), end = m_mapRolePet.end(); iter != end; ++iter )
	{
		if ( iter->second.MiscFlag & PET_MISC_FLAG_COMBAT )
		{
			nNum++;
		}
	}
	return nNum;
}

void GamePet::petStartStepUp( int nHighID, int nLowID )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_STEP_UP;

	tagPetId_ToLua info;
	info.nHeight	= nHighID;
	info.nLow		= nLowID;

	memcpy(&(msgbody.PetClt.PetCltData.PetStepUp.PetWid), &info, sizeof(tdr_ulonglong));
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::petStartImpUp( int nHighID, int nLowID )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_IMP_UP;

	tagPetId_ToLua info;
	info.nHeight	= nHighID;
	info .nLow		= nLowID;

	memcpy(&(msgbody.PetClt.PetCltData.PetStepUp.PetWid), &info, sizeof(tdr_ulonglong));
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::petGiftBookUse( int nGrid )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_GENIUS_LEARN;
	msgbody.PetClt.PetCltData.PetGeniusLearn.GridIdx = nGrid;
	msgbody.PetClt.PetCltData.PetGeniusLearn.PetWid = getFightPet(0)->getRoleWId();
	SendGameServerMsg(PET_CLT, msgbody);
}

void GamePet::petStartUnBind( int nHighID, int nLowID )
{
	tagCSPkgBody msgbody;
	msgbody.PetClt.PetCltType = PET_CLT_UNBIND;

	tagPetId_ToLua info;
	info.nHeight	= nHighID;
	info.nLow		= nLowID;

	memcpy(&(msgbody.PetClt.PetCltData.PetUnbind.PetWid), &info, sizeof(tdr_ulonglong));
	SendGameServerMsg(PET_CLT, msgbody);
}