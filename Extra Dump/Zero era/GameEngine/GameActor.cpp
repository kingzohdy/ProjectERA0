#include "GameActor.h"
#include "GameEngineTypes.h"
#include "GameMap.h"
#include "OgreLog.h"
#include "GameDBData.h"
#include "GameCamera.h"
#include "ShowActor.h"
#include "ServerTimer.h"
#include "cs_net.h"
#include "GameMsgPrint.h"
#include "OgreVector2.h"
#include "ShowGameMap.h"
#include "OgreRay.h"
#include "OgreEntity.h"
#include "AvatarManager.h"
#include "GameSkill_Mgr.h"
#include "GameEffect_Mgr.h"
#include "GameSkill_Control.h"
#include "OgreScriptLuaVM.h"
#include "OgreResourceManager.h"
#include "GameNpc.h"
#include "OgreScriptLuaVM.h"
#include "GameMainPlayer.h"
#include "OgreGameScene.h"
#include "GameActor_Tick.h"
#include "GameActor_PVP.h"
#include "ogredistort.h"
#include <Windows.h>
#include <algorithm>
#include "TMPL.h"
#include "GameDamageHealCounter.h"
#include "EntityEffect.h"

#include "OgreMaterialTemplate.h"

#include "../UILib/ui_framemgr.h"

#include "GhostSchema.h"
#include "ogrebloom.h"
#include "OgreEntityMotion.h"

#include "OgreRoot.h"
#include "OgreDeathScene.h"

#include "UseItemChant.h"
#include "TrumpChant.h"
#include "RideChant.h"
#include "PetChant.h"
#include "SecondaryChant.h"
#include "ResolveChant.h"

using namespace std;
using namespace Ogre;

const int NOMAL_SPEED			= 400;		// 正常行走速度
const int FIGHT_STILLTIME		= 15000;	// 战斗状态持续时间
const int BONE_TIME				= 4000;		
const int WATER_MOTION			= 10001;
const int PET_BONE_TIME			= 5000;

const float MY_PI = 3.1415926f;

const int PLAY_FREE_TIME		= 10000;	//切换到休闲动作的时间

const int STAND_JUMP			= 250;		/// 站立时的起跳延迟
const int MOVE_JUMP				= 0;		/// 跑动时的起跳延迟
const int JUMP_DELAY			= 900;		/// 跳跃的滞空时间

enum PLAYER_DISPLAY_ID
{
	MALE_FIGHTER		= 100000,
	MALE_WARRIOR_SINGLEHAND		= 100020,
	MALE_WARRIOR_DOUBLEHAND		= 100040,

	MALE_WIZARD			= 100002,
	MALE_MAGICOR_SINGLEHAND		= 100022,
	MALE_MAGICOR_DOUBLEHAND		= 100042,

	MALE_GUN			= 100001,
	MALE_GUNER_SINGLEHAND		= 100021,
	MALE_GUNER_DOUBLEHAND		= 100041,

	MALE_CHURCH			= 100003,
	MALE_CHURCH_SINGLEHAND		= 100023,
	MALE_CHURCH_DOUBLEHAND		= 100043,

	FEMALE_FIGHTER		= 100010,
	FEMALE_WARRIOR_SINGLEHAND	= 100030,
	FEMALE_WARRIOR_DOUBLEHAND	= 100050,

	FEMALE_WIZARD		= 100012,
	FEMALE_MAGICOR_SINGLEHAND	= 100032,
	FEMALE_MAGICOR_DOUBLEHAND	= 100052,

	FEMALE_GUN			= 100011,
	FEMALE_GUNER_SINGLEHAND		= 100031,
	FEMALE_GUNER_DOUBLEHAND		= 100051,

	FEMALE_CHURCH		= 100013,
	FEMALE_CHURCH_SINGLEHAND	= 100033,
	FEMALE_CHURCH_DOUBLEHAND	= 100053,
};

const tagNpcDisplayInfo_ToLua&	GetNpcDisplayInfo(int id)
{
	static_assert( sizeof( tagNpcDisplayInfo ) == sizeof( tagNpcDisplayInfo_ToLua ) );
	const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( id );
	return *( ( const tagNpcDisplayInfo_ToLua* )( &npcDisplayInfo ) );
}

const tagLevelUP_ToLua*	getLevelUP( int lv )
{
	static_assert( sizeof( tagLevelUP ) == sizeof( tagLevelUP_ToLua ) );
	const tagLevelUP& LevelUpDef = DBDataManager::getSingleton().getLevelUP( lv );
	return ( const tagLevelUP_ToLua* )&LevelUpDef;
}

const tagMonsterDef_ToLua* getMonsterDef( int id ) {
	static_assert( sizeof( tagMonsterDef ) == sizeof( tagMonsterDef_ToLua ) );
	const tagMonsterDef& monsterDef = DBDataManager::getSingleton().getMonsterDef( id );
	return ( const tagMonsterDef_ToLua* )&monsterDef;
}

const tagLevelFeng_ToLua* getLevelFeng( int lv )
{
	static_assert( sizeof(tagLevelFeng) == sizeof(tagLevelFeng_ToLua) );
	return (const tagLevelFeng_ToLua*)DBDataManager::getSingleton().getLevelFeng( lv );
}

int getPlayerDisplayId( int gender, int race )
{
	static int nDisPlayList[] = {
		MALE_FIGHTER,
		MALE_WARRIOR_SINGLEHAND,
		MALE_WARRIOR_DOUBLEHAND,

		MALE_WIZARD,
		MALE_MAGICOR_SINGLEHAND,
		MALE_MAGICOR_DOUBLEHAND,

		MALE_GUN,
		MALE_GUNER_SINGLEHAND,
		MALE_GUNER_DOUBLEHAND,

		MALE_CHURCH,
		MALE_CHURCH_SINGLEHAND,
		MALE_CHURCH_DOUBLEHAND,

		FEMALE_FIGHTER,
		FEMALE_WARRIOR_SINGLEHAND,
		FEMALE_WARRIOR_DOUBLEHAND,

		FEMALE_WIZARD,
		FEMALE_MAGICOR_SINGLEHAND,
		FEMALE_MAGICOR_DOUBLEHAND,

		FEMALE_GUN,
		FEMALE_GUNER_SINGLEHAND,
		FEMALE_GUNER_DOUBLEHAND,

		FEMALE_CHURCH,
		FEMALE_CHURCH_SINGLEHAND,
		FEMALE_CHURCH_DOUBLEHAND,
	};
	int index = ( gender - 1 ) * 12 + ((race & 0x0F) - 1) * 3 + (race >> 4);
	return nDisPlayList[index];
}

const char* GetModePathById(int nModelId)
{
	const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( nModelId );
	FixedString modelpath = npcDisplayInfo.model;
	return modelpath;	
}

const char *GetSocketPathById(int nModelId)
{
	const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( nModelId );
	return npcDisplayInfo.socket;	
}

void logPrivateInfo( const char* pMessage )
{
	LOG_PRIVATE( pMessage );
}

GameActor::GameActor() : m_Pos(0,0,0), 
						 m_ResID(-1), 
						 m_RealID(0),
						 m_nMorphResID( 0 ),
						 m_pShowActor(NULL), 
						 m_ActState(GSACT_STAND), 
						 m_ReviveEff(NULL),
						 m_nAtkType(0),
						 m_nSubType(0), 
						 m_pCurMap(NULL),
						 m_bWaterPlay(false),
						 m_curSkillAction( 0 ),
						 m_pFightTime(NULL),
						 m_pPlayerFreeTime(NULL),
						 m_uLastFightTime(UINT_MAX),
						 m_nLastJumpTime(0),
						 m_isJumping(false),
						 m_nPrepareSkill(0),
						 m_nPilotSkill(0),
						 m_skillTetanicTime(0),
						 m_nOverHead(0),
						 CollectSkill_SlotMax(0),
						 CollectSkill_SlotCurr(0),
						 CollectSkill_AddType(0),
						 m_isShow(true),
						 m_showWeaponTime(0),
						 m_isShowWeapon(true),
						 m_flagDead(false),
						 m_leastNoCrimeTime(0),
						 m_lastChangeNameColorTime(0),
						 m_isLastChangeNameColor(false),
						 m_CollectTime(0),
						 m_isAFK(false),
						 m_nShowFlag(0),
						 m_optimizeShow(true),
						 m_optimizeClear(false),
						 m_nHitTargetID(0),
						 m_lastOptimizeTime(0),
						 m_BuildFinishTime(0),
						 m_pChantPart(NULL),
						 m_mustUpdateUI(false),
						 m_DartCarType(0),
						 m_nVipFlag(0),
						 m_stickyFrame( 0 )
{
	m_Equip.setOwner(this);
	m_pMapMover		= new GameMapMover(this);
	m_pSpecialMove	= new SpecialMove;
	m_MoveEndPos.x = m_MoveEndPos.y = m_MoveEndPos.z = 0;
	m_MoveEndDis = 0;
	m_AttackLock.m_nAttackLockType = ATTACK_LOCK_NONE;
	m_AttackLock.m_nLockValue = 0;
	m_nFinalAngle = m_nAngle = 0;
	m_SelectTargetId = 0;
	m_strDesignation = "";
	m_bCaptain = false;
	m_ViewFlag = 0;
	m_ObjCamp = 0;
	m_RoleText.setOwner( this );
	m_Rider.setOwner( this );
	m_Pet.setOwner( this );
	m_Trump.setOwner( this );
	memset(m_curSkillEffect, 0, sizeof(m_curSkillEffect));
	memset( &m_petDiathesisInfo, 0, sizeof( m_petDiathesisInfo ) );
	m_petDiathesisInfo.m_nFightMode = -1;
	m_isHeti = false;
	m_pCarry = new GameActor_Carry(this);
	m_MonsterMachine.clearRideMachinInfo();
	memset( &m_BootyInfo, 0, sizeof(tagCSActionMonBootyFirst) );
	memset(&m_NetBarInfo,0,sizeof(tagNetBarInfo_ToLua));
	memset(&m_ViewBanner,0,sizeof(tagViewBanner));

	m_nPetGenius			= -1;
	m_nPetStrengthenLevel	= -1;
	m_nChatActionID			= 0;
	m_nSpeed				= 0;

	m_pSmallDecal = new SmallDecal();
	if (m_pSmallDecal)
		m_pSmallDecal->setTextureRes("effect/texture/fxtexture/weapon_tail/heiqiu.dds", false);
	m_pkEffect.m_pOwner = this;

	m_fJumpTime = 0.0f;
	m_bStartJump = false;
	m_RollType = ITEM_ROLL_NONE;
	m_nTargetCatchPetID	= 0;
	m_nCatchPetNum		= 0;
	m_nLiveTime			= 0;

	m_IsBaseWarResMon	= false;
	m_BaseWarClanWID	= 0;
	m_nPerfectStar		= 0;
	m_szActorWorldName[0] = '\0';
}

GameActor::~GameActor()
{
	m_SelectTargetId = 0;
	for ( std::list<GameActor_Tick*>::iterator iter = m_TickList.begin(), end = m_TickList.end(); 
			iter != end; ++iter )
	{
		GameActor_Tick* pTick = *iter;
		delete pTick;
	}
	m_TickList.clear();

	for (list<IActorDeathAction *>::iterator i = m_lstDeathAction.begin(); i != m_lstDeathAction.end(); ++i)
	{
		assert(false);
		(*i)->doAction(this);
		delete *i;
	}
	m_lstDeathAction.clear();
	
	char szPopName[256];
	if ( getType() == GAT_PLAYER || getType() == GAT_MAINPLAYER )
	{
		sprintf( szPopName, "%s", getName() );
	}
	else if( getType() == GAT_PET )
	{
		sprintf( szPopName, "%d", GetID() );
	}
	else
	{
		sprintf( szPopName, "%s%d", getName(), GetID() );
	}
	
	if ( needAddButtonName() )
	{
		const tagItemDef& itemDef = DBDataManager::getSingleton().getItemDef( getResId() );
		if ( &itemDef != NULL && itemDef.ItemTmp.Type == ITEM_TMP_NO )
		{
			char szUIName[256];
			sprintf( szUIName, "%s%d的UI名字", getName(), GetID() );
			g_pFrameMgr->deleteBindingFrame( szUIName );
		}
	}

	if( getType() == GAT_PLAYER || getType() == GAT_MAINPLAYER ) 
	{
		char strUIName[256];
		sprintf( strUIName, "%s%d的摊位的头顶UI", getName(), GetID() );
		g_pFrameMgr->deleteBindingFrame( strUIName );	
	}

	for ( std::vector<UIObject*>::iterator iter = m_vecActorBindFrames.begin(), end = m_vecActorBindFrames.end();
			iter != end; ++iter )
	{
		UIObject* pFrame = *iter;
		if ( pFrame == NULL )
		{
			continue;
		}

		g_pFrameMgr->deleteBindingFrame( pFrame->GetName() );
		pFrame->release();
	}

	if( m_stickyFrame ) {
		g_pFrameMgr->hideFrame( m_stickyFrame->GetName() );
		m_stickyFrame = NULL;
	}

	g_pFrameMgr->hidePopWin( szPopName );

	delete m_pMapMover;
	delete m_pSpecialMove;
	if ( getType() != GAT_MAINPLAYER && m_Rider.isOnHorse() && !m_Rider.isOwnRide() )
	{
		m_Rider.downHorseSideWhenDestroy();
	}
	OGRE_RELEASE(m_pShowActor);
	if( m_ReviveEff != NULL )
	{
		m_ReviveEff->detachFromScene();
		OGRE_RELEASE(m_ReviveEff);
	}
	m_pCurMap = NULL;
	if( SkillManager::getSingletonPtr() != NULL )
	{
		SkillManager::getSingleton().deleteActorSpell( GetID() );
	}
	OGRE_DELETE(m_pCarry);

	OGRE_RELEASE(m_pSmallDecal);

	OGRE_DELETE(m_pChantPart);
}

void GameActor::LoadRes(int resid)
{
	if(m_pShowActor != NULL)
	{
		OGRE_RELEASE(m_pShowActor);
		m_ResID = -1;
	}

	m_pShowActor = new ShowActor(resid, false);
	if(m_pShowActor == NULL)
	{
		LOG_SEVERE("load actor %d failed", resid);
		return;
	}
	switch (getType())
	{
	case GAT_MONSTER:
		m_pShowActor->setCanSel(true);
		break;
	case GAT_ITEM:
	case GAT_PLAYER:
	case GAT_MAINPLAYER:
	case GAT_PET:
	case GAT_NPC:
		m_pShowActor->setCanSel(true);
	}
	m_ResID = resid;
}

void GameActor::SetPosition( int x, int z, int height )
{	
	m_pMapMover->StopOnPoint(x, z);
	if (getActState().GetState() == GSACT_MOVE)
	{
		ChangeActState(GSACT_STAND);
	}

	int y;
	x *= 10;
	z *= 10;
	m_pCurMap->getTerrainH(x, z, y);
	y += m_pShowActor->getActorOffsetZ();

	m_Pos = GameXYZ(x, y, z);
	if( height != 0 )
	{
		m_pShowActor->setPosition(m_Pos.x, m_Pos.y + height, m_Pos.z);

		if (m_pSmallDecal)
		{
			m_pSmallDecal->setPosition(WorldPos(m_Pos.x, m_Pos.y + height, m_Pos.z));
		}

	}
	else
	{
		m_pShowActor->setPosition(m_Pos.x, m_Pos.y, m_Pos.z);

		if (m_pSmallDecal)
		{
			m_pSmallDecal->setPosition(WorldPos(m_Pos.x, m_Pos.y, m_Pos.z));
		}
	}
}

void GameActor::SetPositionTM( int x, int y, int z )
{
	//m_pMapMover->StopOnPoint(x, z);
	m_Pos = GameXYZ(x, y, z);
	m_pShowActor->setPosition(m_Pos.x, m_Pos.y, m_Pos.z);
	if (m_pSmallDecal)
	{
		m_pSmallDecal->setPosition(WorldPos(m_Pos.x, m_Pos.y, m_Pos.z));
	}
}

void GameActor::OnEnterMap(GameMap *pmap)
{
	if( m_pCurMap == pmap )
	{
		return;
	}

	m_pCurMap = pmap;
	m_pShowActor->onEnterMap(pmap->getShowGameMap());
	if (m_Rider.m_pShowActor != NULL)
	{
		m_Rider.m_pShowActor->onEnterMap(pmap->getShowGameMap());
	}

	if (getType() == GAT_PLAYER || getType() == GAT_MAINPLAYER || getType() == GAT_NPC)
	{
		if (m_pSmallDecal)
			m_pSmallDecal->attachToScene(pmap->getShowGameMap()->getGameScene());
	}
}

void GameActor::OnLeaveMap()
{
	if (getType() == GAT_MAINPLAYER)
	{
		Root::getSingleton().setCurrentTerrainScene(0);

		DeathGameScene::getSingleton().clear();
	}

	if (getType() == GAT_PLAYER || getType() == GAT_MAINPLAYER || getType() == GAT_NPC)
	{
		if (m_pSmallDecal)
			m_pSmallDecal->detachFromScene();
	}

	if( getType() == GAT_MAINPLAYER )
	{
		ActorManager::getSingleton().getScriptVM()->callString("MainPlayerLeaveMap()");
		EffectManage::getSingleton().reset();
		SkillManager::getSingleton().reset();
	}
	if( m_pCurMap != NULL )
	{
		/// 离开地图时，如果玩家以之为当前目标，则清除目标选择
		GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		//处理目标的目标
		if (pMainPlayer && pMainPlayer->GetSelectTarget() == GetID())
		{
			pMainPlayer->SetSelectTarget(0);
		}

		if (getActState().GetState() == GSACT_DEAD)
		{
			if (getType() == GAT_MONSTER)
			{
				char szBuf[256];
				char szBuf2[256];
				const tagMonsterDef& MonsterDef = DBDataManager::getSingleton().getMonsterDef(getResId());
				const tagNpcDisplayInfo &DisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo(getResId());
				sprintf(szBuf, "effect\\entity\\%d_20.ent", MonsterDef.IconID);
				sprintf(szBuf2, "effect\\public\\%d_19.emo", MonsterDef.IconID);
				if (Ogre::ResourceManager::getSingleton().getFileAttrib(szBuf) == 0 && Ogre::ResourceManager::getSingleton().getFileAttrib(szBuf2) == 0)
				{
					if ((getSubType() !=  MON_SUB_TYPE_TRAP) &&
						(getSubType() != MON_SUB_TYPE_TOTEM) && (getSubType()!=MON_SUB_TYPE_COLLECT)
						&& (getSubType()!=MON_SUB_TYPE_BUILD))
					{
						Entity *ent1 = NULL;
						Entity *ent2 = NULL;
						if (DisplayInfo.deadstate & POWER<2, DEAD_STATE_SOUL>::value)
						{
							ent1 = new Entity(*(getShowActor()->getCurEntity()));
						}
						else if (DisplayInfo.deadstate & POWER<2, DEAD_STATE_DECAY>::value)
						{
							ent2 = getShowActor()->getCurEntity();
							if( ent2->isLoading() )
							{
								ent2->detachFromScene();
								ent2 = getShowActor()->getRepEntity();
							}
						}
						else
						{
							m_pShowActor->onLeaveMap(m_pCurMap->getShowGameMap());
						}
						if (ent2)
						{
							DeathGameScene::getSingleton().addObject(ent1, ent2);
						}
					}
					else
					{
						m_pShowActor->onLeaveMap(m_pCurMap->getShowGameMap());
					}
				}
				else
				{
					m_pShowActor->onLeaveMap(m_pCurMap->getShowGameMap());
				}
			}
			else
			{
				m_pShowActor->onLeaveMap(m_pCurMap->getShowGameMap());
			}
		}
		else
		{
			m_pShowActor->onLeaveMap(m_pCurMap->getShowGameMap());
		}

		for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
		{
			if( m_Pet.getPetActor( i ) != NULL )
			{
				m_Pet.getPetActor( i )->OnLeaveMap();
			}
		}

		m_Pet.FreedomPetOnLeaveMap();
		if( m_Rider.isOnHorse() || getActState().GetState() == GSACT_AUTOFLY )
		{
			m_Rider.leaveMap();
		}
		if( isOnMonsterMachine() )
		{
			m_MonsterMachine.m_playerInfo.m_bBind = false;
		}
		if( getType() == GAT_MAINPLAYER || getType() == GAT_PET )
		{
			m_pCurMap = NULL;
		}
		if( m_pCarry->getCarryState() != eNo_Carry )
		{
			m_pCarry->onLeaveMap();
		}

		if( m_pkEffect.m_pEntity != 0 )
		{
			m_pkEffect.m_pEntity->detachFromScene();
			OGRE_RELEASE(m_pkEffect.m_pEntity);
		}
	}
}

void GameActor::OnReachPathTarget()
{
	if (  m_MoveEndPos.x != 0 || m_MoveEndPos.z != 0 || m_MoveEndPos.y || 0 )
	{
		CmdMoveTo( m_MoveEndPos.x, m_MoveEndPos.y, m_MoveEndDis );
	}else{
		if( m_ActState.GetState() == GSACT_MOVE && getType() == GAT_MAINPLAYER )
		{
			bool bWeixing = false;
			ActorManager::getSingleton().getScriptVM()->callFunction("isMainPlayerInWeixing",">b",&bWeixing);
			if( !bWeixing )
			{
				ActorManager::getSingleton().canPickTmpItem();
				ActorManager::getSingleton().tryPickMonsterMachine();
			}			
		}

		if( m_ActState.GetState() == GSACT_MOVE )
		{
			ChangeActState(GSACT_STAND);
		}

		if (m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE)
		{
			playAction( getActState().getActionId(524), (m_curSkillEffect && m_curSkillEffect[0]) ? m_curSkillEffect : NULL, (m_curSkillEffect && m_curSkillEffect[0]) ? 1 : 0 );
		}

		if( m_ActState.GetState() == GSACT_AUTOFLY && getType() == GAT_MAINPLAYER )
		{
			tagCSPkgBody msgbody;
			msgbody.AutoFlyClt.AutoFlyEnd = 1;
			m_pMapMover->GetCurPos( msgbody.AutoFlyClt.EndPos.X, msgbody.AutoFlyClt.EndPos.Y );
			SendGameServerMsg( AUTO_FLY_CLT, msgbody );
		}
	}
}

void GameActor::OnPathMove(GameMaskPath *ppath)
{
	m_pMapMover->StartOnPath(ppath);

	if(  m_ActState.GetState() == GSACT_AUTOFLY )
	{

	}
	else if( ppath->getNumPoint() > 1 )
	{
		ChangeActState(GSACT_MOVE);
	}
}

void GameActor::onStop(int x, int z, int state )
{
	m_pMapMover->StopOnPoint(x, z);
	if( CanChangeActState(state) )
	{
		ChangeActState(state);
	}
	m_MoveEndPos.x = m_MoveEndPos.y = m_MoveEndPos.z = 0;

	int nX, nY, nZ;
	m_pMapMover->GetCurPos(nX, nZ);
	nX*=10;
	nZ*=10;
	m_pCurMap->getTerrainH(nX, nZ, nY);
	nY += m_pShowActor->getActorOffsetZ();
	m_Pos = GameXYZ(nX, nY, nZ);
	m_pShowActor->setPosition(m_Pos.x, m_Pos.y, m_Pos.z);
}

void GameActor::OnRevive()
{
	m_flagDead = false;
	ChangeActState( GSACT_STAND, true );
	getShowActor()->getCurEntity()->stopAnim( m_ActState.getPoseState()*1000+160 );
	getShowActor()->getCurEntity()->stopAnim( 10160 );
	getShowActor()->getCurEntity()->playMotion( "9501_16", false, 300 );

	if (getType() == GAT_MAINPLAYER)
	{
		if(DistortEffect::getSingletonPtr())
			DistortEffect::getSingleton().setGray(false);
		BloomEffect::getSingleton().stopBlend();
	}
}

void GameActor::onHorse( int nHorseId,bool bBind/*=true*/ )
{
	if( getActState().getInState( GSACT_IN_RIDE ) && getType() == GAT_MAINPLAYER )
	{
		m_Rider.requestRideDown();
	}
	if( bBind )
	{
		m_Rider.onHorse( nHorseId );
	}else{
		m_Rider.onHorseWithoutBind(nHorseId);
	}	
}

void GameActor::doBack( int skillid, int sourceid, int x, int z, int BackTime, int type, int nHeight, int speed, const std::string &extra, int delay, bool passive, int actId, const char *atkEff )
{
	tagBackInfo &info = m_BackInfoList[skillid];
	info.startTime = timeGetTime() + delay;
	info.mapid = m_pCurMap != NULL ? m_pCurMap->getMapId() : 0;
	info.x = x;
	info.z = z;
	info.backTime = BackTime;
	info.type = type;
	info.height = nHeight;
	info.speed = speed;
	info.passive = passive;
	info.actId = actId;
	info.effect = atkEff;

	if (type == BACK_CIRCLE)
	{
		GameXYZ centre;
		if (extra == "attacker")
		{
			GameActor *pActor = ActorManager::getSingleton().FindActor(sourceid);
			if (pActor)
			{
				centre = pActor->GetPosition();
				
			}
			else
			{
				centre.x = x * 10;
				centre.z = z * 10;
			}
		}
		else if(extra == "attacked")
		{
			centre = GetPosition();
		}
		else
		{
			centre.x = x * 10;
			centre.z = z * 10;
		}
		info.extra.circle.x = centre.x;
		info.extra.circle.y = centre.z;
		info.extra.circle.inverted = false;
	}
}

void GameActor::stopBack()
{
	m_BackInfoList.clear();
}

// 添加一条飞行路径
void GameActor::setAutoFly( int indexId, bool bFristFly )
{
	m_BackInfoList.clear();
	if( indexId == 1 || indexId > FLY_ANIM_START_INDEX )
	{
		m_pSpecialMove->setAnimFly( this, indexId );
	}else{
		m_pSpecialMove->setAutoFly( this, indexId, bFristFly );	
	}
}

void GameActor::UpdateRotateState( unsigned int dtick )
{
	// 如果处于转向过程中，慢慢转向过去
	const float TURN_SPEED = 0.5f;
	if (m_nFinalAngle != m_nAngle)
	{
		int turnAngle = int(TURN_SPEED * dtick + 0.5);
		if (turnAngle <= 0)
		{
			turnAngle = 1;
		}
		if (m_nFinalAngle > m_nAngle)
		{
			if (m_nFinalAngle - m_nAngle > 180)
			{
				m_nAngle -= turnAngle;
				if (m_nAngle < 0)
				{
					m_nAngle += 360;
					if (m_nAngle < m_nFinalAngle)
					{
						m_nAngle = m_nFinalAngle;
					}
				}
			}
			else
			{
				m_nAngle += turnAngle;
				if (m_nAngle > m_nFinalAngle)
				{
					m_nAngle = m_nFinalAngle;
				}
			}
		}
		else
		{
			if (m_nAngle - m_nFinalAngle > 180)
			{
				m_nAngle += turnAngle;
				if (m_nAngle >= 360)
				{
					m_nAngle -= 360;
					if (m_nAngle > m_nFinalAngle)
					{
						m_nAngle = m_nFinalAngle;
					}
				}
			}
			else
			{
				m_nAngle -= turnAngle;
				if (m_nAngle < m_nFinalAngle)
				{
					m_nAngle = m_nFinalAngle;
				}
			}
		}

		m_pShowActor->setFaceTo(m_nAngle);
	}

}

void GameActor::setType( int type )
{
	RoleAttr::setType( type );
	m_RoleText.CreateActorTitleControl();
}

void GameActor::setPkValue( int nPkValue )
{
	GameActor_PVP& actorPvpMgr = GameActor_PVP::getSingleton();
	int nOldAmok = actorPvpMgr.getAmok( m_nPkValue );
	RoleAttr::setPkValue( nPkValue );
	int nNewAmok = actorPvpMgr.getAmok( m_nPkValue );
	if ( nOldAmok != nNewAmok )
	{
		m_RoleText.UpdateTextColor();
	}
}

void GameActor::setDesignationID( int nDesignationID )
{
	int nOldDesignationID = getDesignationID();
	RoleAttr::setDesignationID( nDesignationID );
	if ( nOldDesignationID != nDesignationID )
	{
		m_RoleText.BuildTitle();
	}
}

void GameActor::setRelationType( int nRelationType )
{
	int nOldRelationType = getRelationType();
	RoleAttr::setRelationType( nRelationType );
	if ( nOldRelationType != nRelationType )
	{
		m_RoleText.UpdateTextColor();
	}
}

void GameActor::setObjCamp( int nObjCamp )
{
	int nOldObjCamp = getObjCamp();
	RoleAttr::setObjCamp( nObjCamp );
	if ( nOldObjCamp != nObjCamp )
	{
		m_RoleText.UpdateTextColor();
	}
}

void GameActor::setHP( int hp )
{
	int nOldHp = getHP();
	RoleAttr::setHP( hp );
	if ( nOldHp != hp )
	{
		m_RoleText.UpdateTitleHp();
	}
}

void GameActor::setMaxHP(int maxhp)
{
	int nOldMaxHp = getMaxHP();
	RoleAttr::setMaxHP( maxhp );
	if ( nOldMaxHp != maxhp )
	{
		m_RoleText.UpdateTitleHp();
	}
}

void GameActor::Update(unsigned int dtick)
{
	UpdateMin(dtick);
	if ( !m_pShowActor->isInMap() )
	{
		return;
	}
	int sec, usec;
	ServerTimer::getSingleton().getServerTime(sec, usec);
	unsigned int serverTimeNow = sec * 1000 + usec / 1000;
	/*if( !isInRide() )
	{
		m_pShowActor->update(dtick);
	}*/
	m_pShowActor->update(dtick);
	switch (Root::getSingleton().getShadowmapSize())
	{
	case 0:
		{
			if( m_isShow )
			{
				if (m_pSmallDecal )
				{
					m_pSmallDecal->show(true);
					m_pSmallDecal->setWidth(100.0f);
					m_pSmallDecal->setPosition(WorldPos(m_Pos.x, m_Pos.y, m_Pos.z));
					m_pSmallDecal->update(dtick);
				}
			}else{
				m_pSmallDecal->show(false);
			}			
		}
		break;

	case 1:
		{
			// 中级别阴影，如果是玩家屏蔽掉阴影
			if (m_pSmallDecal)
			{
				m_pSmallDecal->show(false);
			}
			if (getType() == GAT_PLAYER)
			{
				Entity *ent = NULL;
				ent = getShowActor()->getCurEntity();
				if (ent)
					ent->SetForceShadowMapCull(true);

				if (m_Rider.m_pShowActor)
				{
					ShowActor *showActor = m_Rider.m_pShowActor;
					if (showActor)
					{
						Entity *ent = showActor->getCurEntity();
						if (ent)
						{
							ent->SetForceShadowMapCull(true);
						}
					}
				}

				for (int i=0; i<PET_MAX_FIGHT_SHI; i++)
				{
					GameActor *actor = m_Pet.getFightPet(i);
					if (actor)
					{
						ShowActor *showActor = actor->getShowActor();
						if (showActor)
						{
							Entity *ent = showActor->getCurEntity();

							if (ent)
								ent->SetForceShadowMapCull(true);
						}
					}
				}
			}
		}
		break;

	default:
		{
			if (m_pSmallDecal)
			{
				m_pSmallDecal->show(false);
			}
			Entity *ent = NULL;
			ent = getShowActor()->getCurEntity();
			if (ent)
				ent->SetForceShadowMapCull(false);

			if (m_Rider.m_pShowActor)
			{
				ShowActor *showActor = m_Rider.m_pShowActor;
				if (showActor)
				{
					Entity *ent = showActor->getCurEntity();
					if (ent)
					{
						ent->SetForceShadowMapCull(false);
					}
				}
			}

			for (int i=0; i<PET_MAX_FIGHT_SHI; i++)
			{
				GameActor *actor = m_Pet.getFightPet(i);
				if (actor)
				{
					ShowActor *showActor = actor->getShowActor();
					if (showActor)
					{
						Entity *ent = showActor->getCurEntity();

						if (ent)
							ent->SetForceShadowMapCull(false);
					}
				}
			}
		}
	}

	m_RoleText.Update( dtick );

	if (m_pShowActor->getCurEntity()->getModelPlayRadio() != 1.0f)
	{
		if (SkillManager::getSingleton().getSpellControler()->getCurPilSpell())
		{
			m_pShowActor->getCurEntity()->setModePlayRadio(1.0f);
		}
	}

	if (getActState().GetState() == GSACT_SIT)
	{
		if (getShowActor()->getCurAction() != 10200 && !getShowActor()->getCurEntity()->hasAnimPlaying(10201))
		{
			playAction( 10200, NULL, 0, 0 );
		}
	}
	else if (getActState().GetState() == GSACT_MOVE)
	{
		if( canActWingFloat() &&m_pShowActor->getCurAction() != 10127 )
		{
				m_pShowActor->playAction( 10127, NULL, 0, 0 );
		}
	}
	else if (getActState().GetState() == GSACT_STAND)
	{
		if( canActWingFloat() &&m_pShowActor->getCurAction() != 10123 )
		{
			m_pShowActor->playAction( 10123, NULL, 0, 0 );
		}
		if (getShowActor()->getCurAction() == 10202 && !getShowActor()->getCurEntity()->hasAnimPlaying(10202))
		{
			int actId = m_ActState.getActionId();
			if( m_pCarry->getCarryState() == eCarrying || m_pCarry->getCarryState() == eCarryed )
			{
				actId = m_pCarry->getCarryStandAction();
			}
			if( !m_pShowActor->hasAnim( actId ) )
			{
				actId = m_ActState.getActionId(true);
			}
			m_pShowActor->playAction( actId, NULL, 0, 0 );
		}
	}

	if( m_pCurMap == NULL ) return;

	unsigned int uCurTime = timeGetTime();
	if (m_showWeaponTime && uCurTime >= m_showWeaponTime)
	{
		m_showWeaponTime = 0;
		if (m_isShowWeapon)
		{
			m_Equip.showWeapon();
		}
	}
	if (m_BuildFinishTime > 0 && m_BuildFinishTime <= serverTimeNow)
	{
		m_BuildFinishTime = 0;
		onRestore(m_ResID * 100);
	}

	if (getShowActor()->getCurEntity()->hasAnimPlaying(10133) || getShowActor()->getCurEntity()->hasAnimPlaying(10134) ||
		getShowActor()->getCurEntity()->hasAnimPlaying(10137) || getShowActor()->getCurEntity()->hasAnimPlaying(10138))
	{
		m_isJumping = true;
		int nDruing = uCurTime - m_nLastJumpTime;
		if (nDruing < 0)
		{
			nDruing = 0;
		}
		else if (nDruing > JUMP_DELAY)
		{
			nDruing = JUMP_DELAY;
		}
		m_nOverHead = (int)((1 - (nDruing - JUMP_DELAY / 2) * (nDruing - JUMP_DELAY / 2) * 4 / (JUMP_DELAY * JUMP_DELAY * 1.0)) * 2000);
	}
	else
	{
		m_nOverHead = 0;
		if (m_isJumping)
		{
			m_isJumping = false;
			if (getActState().GetState() == GSACT_MOVE)
			{
				m_pShowActor->getCurEntity()->playMotion( "footprint", false, 0 );
			}
		}
	}

	if (m_AttackLock.m_nAttackLockType == ATTACK_LOCK_OBJ)
	{
		GameActor *pActor = ActorManager::getSingleton().FindActor(m_AttackLock.m_nLockValue);
		if (pActor == NULL || pActor->getActState().GetState() == GSACT_DEAD)
		{
			unLockFace();
		}
		else
		{
			Ogre::Vector3 pos = pActor->GetPosition().toVector3();
			SetFaceTo(int(pos.x), int(pos.z));
		}
	}

	
	float fw = 0.0f;
	int wY = getCurMap()->getShowGameMap()->getGroundPoint( GetPosition().x, GetPosition().z, NULL, &fw );

	if( m_bWaterPlay )
	{
		if( fw < wY/10 || getActState().GetState() != GSACT_MOVE )
		{
			if (m_Rider.m_pShowActor != NULL)
			{
				m_Rider.m_pShowActor->getCurEntity()->stopMotion( WATER_MOTION );
			}
			m_pShowActor->getCurEntity()->stopMotion( WATER_MOTION );
			m_bWaterPlay = false;
		}
	}else{
		if( fw > wY/10 && getActState().GetState() == GSACT_MOVE && (  getType() == GAT_PLAYER || getType() == GAT_MAINPLAYER ) )
		{
			if (m_Rider.m_pShowActor != NULL)
			{
				m_Rider.m_pShowActor->getCurEntity()->playMotion( "WaterSwamp", true, WATER_MOTION );
			}
			else
			{
				m_pShowActor->getCurEntity()->playMotion( "WaterSwamp", true, WATER_MOTION );
			}
			m_bWaterPlay = true;
		}
	}

	if( m_ReviveEff != NULL )
	{
		m_ReviveEff->update( dtick );
	}

	if( getType() == GAT_MAINPLAYER )
	{
		GameActor* pActor = ActorManager::getSingleton().FindActor( GetSelectTarget() ); 
		if( pActor != NULL && pActor->getType() == GAT_MONSTER && pActor->getActState().GetState() == GSACT_DEAD )
		{
			GameSpell *pSPell = SkillManager::getSingleton().getNextSpell();
			if( pSPell && pSPell->getSelectTarget() == GetSelectTarget() )
			{
				SkillManager::getSingleton().setNextSpell(0);

				if( getActState().GetState() == GSACT_ATTACK )
				{
					ChangeActState( GSACT_STAND );
				}
				// 再次屏蔽目标死亡后自动切换目标
//  				GameActor* pTarget = ActorManager::getSingleton().getNearestActor( ActorManager::getSingleton().getMainPlayer()->GetID(), 7500 );
//  				if( pTarget != NULL )
//  				{
//  					SetSelectTarget( pTarget->GetID() );
//  				}
			}
		}
	}
	if (uCurTime > m_uLastFightTime)
	{
		m_uLastFightTime = UINT_MAX;
		onLeaveFight();
	}
	int nCurActionID = getShowActor()->getCurAction();
	if( m_nChatActionID != 0 && 
		(nCurActionID != m_nChatActionID || !getShowActor()->getCurEntity()->hasAnimPlaying(m_nChatActionID)) )
	{
		if( nCurActionID != 10300 && nCurActionID != 10251 )
		{
			m_Equip.showWeapon();
		}		
		m_nChatActionID = 0;
		if( getType() == GAT_MAINPLAYER )
		{
			MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
			pMainPlayer->requestSendMotionAction(0,0);
		}
	}
}

void GameActor::updateUI( int nChangeValue )
{
	if( ActorManager::getSingleton().getMainPlayer() == NULL  ) return;

	if( nChangeValue == ATTR_ID_LEVEL )
	{
		GameMsgPrint::getSingleton().playerLvChange( getLv() );
	}
	m_mustUpdateUI = true;
}

void GameActor::actualUpdateUI()
{
	int selectTargetID = ActorManager::getSingleton().getMainPlayer()->GetSelectTarget();
	if( GetID() == selectTargetID )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "UI_TARGET_ATTRIBUTE_CHG" );
	}
	GameActor* pTarget = ActorManager::getSingleton().FindActor( selectTargetID );
	if( pTarget != NULL && GetID() == pTarget->getHitTargetID() )
	{
		char buf[256];
		sprintf(buf,"UpdateTargetOfTargetFrame( %d )",pTarget->getHitTargetID());
		ActorManager::getSingleton().getScriptVM()->callString( buf );
		//GameMsgPrint::getSingleton().SendUIEvent( "UI_TARGETOFTARGET_ATTRIBUTE_CHG" );
	}
}

void GameActor::ChangeActState(int state,bool bRevive)
{
	if(!bRevive && m_ActState.GetState() == GSACT_DEAD )
	{
		return;
	}
	if( isOnMonsterMachine() )
	{
		GameActor* pActor = ActorManager::getSingleton().FindActor(  m_MonsterMachine.m_playerInfo.m_nMonsterMemID );
		if( pActor != NULL )
		{
			pActor->ChangeActState( state );
		}
	}
	if( m_ActState.GetState() == GSACT_STOP && state != GSACT_STOP )
	{
		//m_pShowActor->getCurEntity()->resetUpdate( false );
	}

	if( m_ActState.GetState() == GSACT_COLLECTITEM && state != GSACT_COLLECTITEM )
	{
		endCollect();
	}

	if( m_ActState.GetState() == GSACT_AUTOFLY && state != GSACT_AUTOFLY )
	{
		if( getType() == GAT_MAINPLAYER )
		{
			//ActorManager::getSingleton().getMainPlayer()->releaseControl();
		}
		m_Rider.downHorse();
	}

	if( m_ActState.GetState() == GSACT_FLY && ( state == GSACT_MOVE || state == GSACT_STAND ) )
	{
		return;
	}

	if (m_ActState.GetState() == GSACT_SIT && (m_pShowActor->getCurAction() == 10200 || m_pShowActor->getCurAction() == 10201))
	{
		//playAction( 10202, NULL, 0, 0 );
	}
	
	if ( m_ActState.GetState() == GSACT_STALL && state == GSACT_DEAD && getType() == GAT_MAINPLAYER )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_STOP_STALL_BY_DEAD" );
	}
	if( m_ActState.GetState() == GSACT_SHAMDEAD && getType() == GAT_MAINPLAYER )
	{
		requestCancelShamDead();
	}
	m_ActState.SetState(state);

	if (!isOnMonsterMachine())
	{
		switch(state)
		{
		case GSACT_STAND:
			onChangeStand();
			break;
		case GSACT_MOVEBACK:
		case GSACT_MOVE:
			onChangeMove();
			break;
		case GSACT_SIT:
			{
				getShowActor()->getCurEntity()->stopAnim();// 打断休闲动作
				if( getType() == GAT_MAINPLAYER )
				{
					ActorManager::getSingleton().getMainPlayer()->endAutoTalkToNpc();
				}
			}
			break;

		case GSACT_DEAD:
			{
				if( getType() == GAT_ITEM )
				{
					break;
				}

				m_RoleText.UpdateTitleHp();
				playAFK(false);

				if (getType() == GAT_MAINPLAYER)
				{
					if (DistortEffect::getSingletonPtr())
						DistortEffect::getSingleton().setGray(true);
					BloomEffect::getSingleton().playBlend();
					breakBTypePre();
					SkillManager::getSingleton().onStop();
				}
				getShowActor()->getCurEntity()->resetUpdate( false );

				for (list<IActorDeathAction *>::iterator i = m_lstDeathAction.begin(); i != m_lstDeathAction.end(); ++i)
				{
					(*i)->doAction(this);
					delete *i;
				}
				m_lstDeathAction.clear();

				// TODO: 问策划如何定显示时间
				if ( getType() == GAT_PET )
				{
					registerTickEvent( new GameActor_Tick_OnDead( this, timeGetTime(), PET_BONE_TIME ) );
				}
				else
				{
					GameActor_Tick_OnDead *tick = new GameActor_Tick_OnDead( this, timeGetTime(), BONE_TIME );

					if (tick)
					{
						registerTickEvent(tick);
					}
				}
				for( int i = 0; i < PET_MAX_FIGHT_SHI; i++ )
				{
					GameActor *pet = getPet()->getPetActor(i);
					if (pet)
					{
						pet->OnLeaveMap();
					}
				}
			}
			break;
		case GSACT_COLLECTITEM:
			{
				if( m_Rider.isOnHorse() || isInCarry() )
				{
					break;
				}
				m_pShowActor->stopCurAction();
				m_pShowActor->playAction( 10250, NULL, 0, 0 );
			}
			break;
		case GSACT_AUTOFLY:
			{
				//onHorse( 110000 );
				if( getType() == GAT_MAINPLAYER )
				{
					//ActorManager::getSingleton().getMainPlayer()->lockControl();
				}
			}
			break;
		case GSACT_FLY:
			{
				int actionId = m_ActState.getActionId();
				if( !m_pShowActor->hasAnim( actionId ) )
				{
					m_pShowActor->playAction( m_ActState.getActionId(true), NULL, 0, 0 );
				}else{
					m_pShowActor->playAction( actionId, NULL, 0, 0 );
				}
			}
			break;
		case GSACT_STALL:
		case GSACT_STOP:
			{
				if( getType() == GAT_MAINPLAYER )
				{
					MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
					if ( pMainPlayer->canAutoTalkToNpc() )
					{
						pMainPlayer->endAutoTalkToNpc();
					}
				}
			}
		case GSACT_SHAMDEAD:
			{
				int actionId = m_ActState.getActionId();
				if( m_pShowActor->hasAnim(actionId) )
				{
					m_pShowActor->playAction( actionId, NULL, 0, 0 );
				}
			}
			break;
		default:
			break;
		}
	}
}

bool GameActor::CanChangeActState(int state)
{
	if( m_ActState.GetState() == GSACT_DEAD ) 
	{
		return false;
	}

	return true;
}

void GameActor::onChangeStand()
{
	if (m_pShowActor)
	{
		m_pShowActor->getCurEntity()->setModePlayRadio( 1.0f );
	}

	if (isAFK() && !isInRide() && !isInMorph())
	{
		playAFK(true);
	}
	if( isOnMonsterMachine() )
	{
		return;
	}

	if (m_AttackLock.m_nAttackLockType == ATTACK_LOCK_NONE)
	{
		int actionId = m_ActState.getActionId();
		if( m_pCarry->getCarryState() == eCarrying || m_pCarry->getCarryState() == eCarryed )
		{
			actionId = m_pCarry->getCarryStandAction();
		}
		if( m_Rider.isOnHorse() )
		{
			if( m_Rider.m_pShowActor )
			{
				m_Rider.m_pShowActor->getCurEntity()->setModePlayRadio( 1.0f );
			}
			m_Rider.horsePlayAct( m_ActState.getActionId(true), NULL, 0, 0 );
		}
		else if (m_pShowActor)
		{
			if (!getShowActor()->getCurEntity()->hasAnimPlaying(10202))
			{
				if( !m_pShowActor->hasAnim( actionId ) )
				{
					m_pShowActor->playAction( m_ActState.getActionId(true), NULL, 0, 0 );
				}
				else
				{
					m_pShowActor->playAction( actionId, NULL, 0, 0 );
				}
			}
		}
	}

	m_curRunAngle = RUN_NONE;
	if( getType() == GAT_PLAYER || getType() == GAT_MAINPLAYER )
	{
// 		if (!haveTickEvent(m_pPlayerFreeTime) )
// 		{
// 			m_pPlayerFreeTime = new GameActor_Tick_MainPlayerFree(this,timeGetTime() + PLAY_FREE_TIME,LOOP_TICK);
// 			registerTickEvent(m_pPlayerFreeTime);
// 		}else{
// 			m_pPlayerFreeTime->setTime(timeGetTime() + PLAY_FREE_TIME,LOOP_TICK);
// 		}
	}
}

void GameActor::onChangeMove()
{
	// 锁定攻击跑动
	if (m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE)
	{
		return;
	}
	if (isInSkillTetanic())
	{
		return;
	}
	int actionId = m_ActState.getActionId();

	if (m_Rider.isOnHorse())
	{
		actionId = m_ActState.getActionId(true);
		
		if (m_Rider.m_pShowActor && m_Rider.m_pShowActor->getCurAction() != actionId)
		{
			const char* szFootString[4];
			std::string strFoot("footprint");
			szFootString[0] = strFoot.c_str();
			m_Rider.horsePlayAct( actionId, szFootString, 1, 0 );
		//	m_Rider.horsePlayAct( actionId, NULL, 0, 0 );
		}
	}
	else
	{
		if( isOnMonsterMachine() )
		{
			return;
		}
		int brkActId = m_ActState.getActionId(444);
		if (!hasAnim(brkActId))
		{
			brkActId = getActId(POSE_NONE, 444);
		}
		if( hasAnim(brkActId) )
		{
			playAction(brkActId);
		}
		if( m_pCarry->getCarryState() == eCarrying || m_pCarry->getCarryState() == eCarryed )
		{
			actionId = m_pCarry->getCarryMoveAction();
		}
		else if (getType() == GAT_MONSTER || getType() == GAT_NPC)
		{
			const tagMonsterDef &monsterDef = DBDataManager::getSingleton().getMonsterDef(getResId());
			if( getSubType() == MON_SUB_TYPE_MACHINE )
			{
				if (m_nSpeed < monsterDef.MoveSpd)
				{
					actionId = m_ActState.getWalkActId();
				}
			}else{
				if (m_nSpeed <= monsterDef.MoveSpd)
				{
					actionId = m_ActState.getWalkActId();
				}
			}
			
		}

		if (!m_pShowActor->hasAnim( actionId ))
		{
			actionId = m_ActState.getActionId(true);
		}

		if (m_pShowActor->getCurAction() != actionId)
		{
			//m_pShowActor->stopCurAction();
			const char* szFootString[4];
			std::string strFoot("footprint");
			szFootString[0] = strFoot.c_str();
			m_pShowActor->playAction( actionId, szFootString, 1, 0 );
		}
	}
	
	if( m_nSpeed > NOMAL_SPEED )
	{
		float fRadio = (float)m_nSpeed / NOMAL_SPEED;
		m_pShowActor->getCurEntity()->setModePlayRadio( fRadio );
		if( isInRide() && m_Rider.m_pShowActor != NULL )
		{
			m_Rider.m_pShowActor->getCurEntity()->setModePlayRadio( fRadio );
		}
	}else if( m_nSpeed < NOMAL_SPEED && getType() != GAT_MONSTER && getType() != GAT_NPC && getType() != GAT_PET )
	{
		float fRadio = (float)m_nSpeed / NOMAL_SPEED;
		m_pShowActor->getCurEntity()->setModePlayRadio( fRadio );
		if( isInRide() && m_Rider.m_pShowActor != NULL )
		{
			m_Rider.m_pShowActor->getCurEntity()->setModePlayRadio( fRadio );
		}
	}
}

void GameActor::onChangeBone()
{
	m_pShowActor->playAction( m_ActState.getDeadActId(), NULL, 0, 0 );
	m_ActState.SetState( GSACT_DEAD );
}

// 进入战斗
void GameActor::onFight(bool bForce/* = true*/)
{
	if( getType() == GAT_MONSTER )
	{
		if( !haveTickEvent(m_pFightTime) )
		{
			m_pFightTime = new GameActor_Tick_InFightTime( this, timeGetTime(), FIGHT_STILLTIME );
			registerTickEvent( m_pFightTime );
			m_pFightTime->onBeginEvent();
		}else{
			m_pFightTime->setTime( timeGetTime(), FIGHT_STILLTIME );
		}
	}else{
		m_ActState.setInState( GSACT_IN_FIGHT, true );
		if (this == ActorManager::getSingleton().getMainPlayer())
		{
			GameMsgPrint::getSingleton().showMidTip( MID_FIGHT_IN );
			GameMsgPrint::getSingleton().SendUIEvent("GE_ACTOR_FIGHT_IN");
		}
		if (bForce)
		{
			m_uLastFightTime = UINT_MAX;
		}
		else
		{
			m_uLastFightTime = timeGetTime() + 3000;
		}
	}

	if (bForce && (getType() == GAT_PLAYER || getType() == GAT_MAINPLAYER))
	{
		GameDamageHealCounter::getSingleton().InFight(GetID());
	}

	int nCanChangeFight = m_ActState.canChangeState( GSACT_IN_FIGHT );
	if(nCanChangeFight != STATE_UNACT_CHANGE && !SkillManager::getSingleton().getSpellControler()->getCurSpell()
		&& !isOnMonsterMachine() )
	{
		m_pShowActor->playAction( m_ActState.getActionId() );
	}
}

// 离开战斗状态
void GameActor::onLeaveFight()
{
	if (getType() == GAT_PLAYER || getType() == GAT_MAINPLAYER)
	{
		GameDamageHealCounter::getSingleton().OutFight(GetID());
	}

	int nCanChangeFight = m_ActState.canChangeState( GSACT_IN_LEAVEFIGHT );

	m_ActState.setInState( GSACT_IN_FIGHT, false );
	if( this == (GameActor*)ActorManager::getSingleton().getMainPlayer() )
	{
		GameMsgPrint::getSingleton().showMidTip( MID_FIGHT_OUT );
		GameMsgPrint::getSingleton().SendUIEvent("GE_ACTOR_FIGHT_OUT");
	}

	if(nCanChangeFight != STATE_UNACT_CHANGE && !SkillManager::getSingleton().getSpellControler()->getCurSpell()
		&& !isOnMonsterMachine())
	{
		m_pShowActor->playAction( m_ActState.getActionId() );
	}
}

void GameActor::setInState( int state, bool flag )
{	
	m_ActState.setInState( state, flag );
	if( m_ActState.GetState() == GSACT_COLLECT || m_ActState.GetState() ==  GSACT_PREPARE 
		|| m_ActState.GetState() ==  GSACT_ATTACK || m_ActState.GetState() == GSACT_DEAD )
	{
		return;
	}

	if( m_Rider.isOnHorse() )
	{
		m_Rider.horsePlayAct( m_ActState.getActionId(true), NULL, 0, 0 );
	}
	else
	{
		int actionId = m_ActState.getActionId();
		if (getType() == GAT_MONSTER || getType() == GAT_NPC)
		{
			const tagMonsterDef &monsterDef = DBDataManager::getSingleton().getMonsterDef(getResId());
			if (m_nSpeed <= monsterDef.MoveSpd)
			{
				actionId = m_ActState.getWalkActId();
			}
		}
		if (!m_pShowActor->hasAnim(actionId))
		{
			actionId = m_ActState.getActionId(true);
		}
		m_pShowActor->playAction( actionId );
	}
}

bool GameActor::getInState( int state )
{
	return m_ActState.getInState( state );
}

void GameActor::lockFace(int type, int nvalue, int skillId)
{
	m_AttackLock.m_nAttackLockType = type;
	m_AttackLock.m_nLockValue	   = nvalue;
	m_curRunAngle				   = RUN_NONE;

	if (type == ATTACK_LOCK_NONE)
	{
		SkillManager::getSingleton().getSpellControler(m_ID)->onLock();
	}
	else
	{
		SkillManager::getSingleton().getSpellControler(m_ID)->onLock(skillId);
	}

	if (type == ATTACK_LOCK_DIR)
	{
		SetFaceTo(nvalue);
	}

	if (m_ActState.GetState() == GSACT_MOVE)
	{
		onChangeMove();
	}
	else
	{
		onChangeStand();
	}
}

void GameActor::unLockFace()
{
	lockFace(ATTACK_LOCK_NONE, 0, 0);
	if (m_ActState.GetState() == GSACT_MOVE)
	{
		onChangeMove();
	}
	else if (m_ActState.GetState() == GSACT_STAND)
	{
		onChangeStand();
	}
}

void GameActor::SetFaceTo(int x, int z, bool immediately/* = false*/)
{
	int ox, oz;
	m_pMapMover->GetCurPos(ox, oz);
	SetFaceTo(ox, oz, x, z, immediately);
}

void GameActor::SetFaceTo(int from_x, int from_z, int to_x, int to_z, bool immediately/* = false*/)
{
	if( m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE)
	{
		if( m_AttackLock.m_nAttackLockType == ATTACK_LOCK_OBJ )
		{
			GameActor* pObj = ActorManager::getSingleton().FindActor( m_AttackLock.m_nLockValue );
			if ( pObj == NULL ) 
			{
				return;
			}

			from_x	= GetPosition().x/10;
			from_z	= GetPosition().z/10;
			to_x	= pObj->GetPosition().x/10;
			to_z	= pObj->GetPosition().z/10;
		}

		if( m_AttackLock.m_nAttackLockType == ATTACK_LOCK_DIR )
		{
			return;
		}
	}

	float dx = float(to_x - from_x);
	float dz = float(to_z - from_z);
	if (dx || dz)
	{
		SetFaceTo(FaceAngleBetween(from_x, from_z, to_x, to_z), immediately);
	}
	//对机械怪物攻击时，转向处理
	if( getType() == GAT_MONSTER && immediately )
	{
		if( m_MonsterMachine.m_monsterInfo.m_nMonsterMemID > 0  )
		{
			GameActor* pDriver = ActorManager::getSingleton().FindActor( m_MonsterMachine.m_monsterInfo.m_nPassengerID[0] );
			if( pDriver != NULL )
			{
				pDriver->SetFaceTo( FaceAngleBetween(from_x, from_z, to_x, to_z), immediately );
			}
		}
	}
}

void GameActor::SetFaceTo(int angle, bool immediately/* = false*/)
{
	// 先把角度转变到0~360
	if (angle < 0)
	{
		angle += ((-angle) / 360 + 1) * 360;
	}
	angle %= 360;
	if (immediately)
	{
		m_nFinalAngle = m_nAngle = angle;
		m_pShowActor->setFaceTo(angle);
	}
	else
	{
		m_nFinalAngle = angle;
	}
	if (isOnMonsterMachine())
	{
		GameActor *pActor = ActorManager::getSingleton().FindActor(m_MonsterMachine.m_playerInfo.m_nMonsterMemID);
		if (pActor != NULL)
		{
			pActor->SetFaceTo(angle, immediately);
		}
	}
}

void GameActor::playMotion( const char* szEffect, bool bResetPlay )
{
	m_pShowActor->getCurEntity()->playMotion( szEffect, bResetPlay, 200 );
}

void GameActor::playMotion(const char* szEffect, int motionclass)
{
	m_pShowActor->getCurEntity()->playMotion(szEffect, true, motionclass);
}

bool GameActor::isMotionPlaying(const char* szEffect, int motionclass)
{
	return m_pShowActor->getCurEntity()->isMotionPlaying(szEffect, motionclass);
}

void GameActor::stopMotion( const char* szEffect )
{
	m_pShowActor->getCurEntity()->stopMotion( szEffect );
}

void GameActor::playAction( int actId, const char* szEffect )
{
	if( szEffect != NULL )
	{
		m_curSkillEffect[0] = szEffect;
		playAction( actId, m_curSkillEffect, 1 );
	}else{
		playAction( actId, NULL, 0 );
	}
}

void GameActor::circularMoveAngle( int from_x, int from_z, int to_x, int to_z )
{
	if( m_curSkillAction == 0)
	{
		return;
	}

	if( m_ActState.GetState() == GSACT_MOVE && m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE )
	{
		float nAngleFaceTo;
		float nAngleMoveTo;
		if (from_x == to_x && from_z == to_z)
		{
			return;
		}
		else
		{
			nAngleMoveTo = atan2(float(to_z - from_z), float(to_x - from_x));
		}

		if( m_AttackLock.m_nAttackLockType == ATTACK_LOCK_OBJ )
		{
			GameActor* pObj = ActorManager::getSingleton().FindActor( m_AttackLock.m_nLockValue );
			if( pObj == NULL || pObj->getActState().GetState() == GSACT_DEAD)
			{
				unLockFace();
				return;
			}

			Vector3 pos = pObj->GetPosition().toVector3();

			if( from_x ==  pos.x && from_z ==  pos.z)
			{
				return;
			}
			nAngleFaceTo = atan2(float(pos.z - from_z), float(pos.x - from_x));
		}
		else
		{
			nAngleFaceTo = getAngle() * MY_PI / 180;
		}

		float fRadDis = cos(nAngleFaceTo - nAngleMoveTo);
		int nActId;
		if (fRadDis > cos(MY_PI / 4))
		{
			nActId = getActState().getActionId(522);
			m_curRunAngle = RUN_BEFORE;
		}
		else if (fRadDis < cos(MY_PI / 4 * 3))
		{
			nActId = getActState().getActionId(523);
			m_curRunAngle = RUN_BACK;
		}
		else if (nAngleFaceTo < nAngleMoveTo)
		{
			nActId = getActState().getActionId(520);
			m_curRunAngle = RUN_LEFT;
		}
		else
		{
			nActId = getActState().getActionId(521);
			m_curRunAngle = RUN_RIGHT;
		}
		playAction( nActId, m_curSkillEffect, (m_curSkillEffect && m_curSkillEffect[0]) ? 1 : 0 );
	}	
}

bool GameActor::isInScene()
{
	if ( m_pShowActor == NULL )
	{
		return false;
	}

	return m_pShowActor->isInMap();
}

bool GameActor::IsInSpecialPworldNotShowTitle()
{
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();

	//是不是在机械改造副本
	int mapid = pMain->getMapRealId();
	if( mapid >= GAME_PWORLD_MAPID )
	{
		int nPworldId = mapid/100;
		const tagPworldDef* pPworldDef = DBDataManager::getSingleton().getPworldDef( nPworldId );
		if( pPworldDef != NULL && pPworldDef->PolyType == PWORLD_POLY_MACHINE_WAR )
		{
			return true;
		}
	}
	return false;
}

void GameActor::RawSetSelectTarget( int nTargetId )
{
	m_SelectTargetId = nTargetId;
}

bool GameActor::canSelectTarget( GameActor* pTarget)
{
	GameActor* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
// 	if (pMainPlayer->m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE)
// 	{
// 		return false;
// 	}
	
	if ( pTarget != NULL && ( pTarget->getSubType() == MON_SUB_TYPE_TRAP || 
								pTarget->getSubType() == MON_SUB_TYPE_BUILD ) )
	{
		return false;
	}

	if ( pTarget != NULL && pTarget->getType() == GAT_MONSTER && pTarget->getSubType() == MON_SUB_TYPE_COLLECT )
	{
		const char* pszOwerName = pTarget->getOwnerName();
		if ( pszOwerName[0] != 0 && strcmp( pszOwerName, pMainPlayer->getName() ) != 0 )
		{
			return false;
		}
	}

	if (pTarget != NULL && !pTarget->m_isShow)
	{
		return false;
	}

	if (pTarget != NULL && pTarget->getType() == GAT_PET && 
			ActorManager::getSingleton().getGameOpt( GAME_OPT_FORBIT_SELECT_PET ) )
	{
		return false;
	}

	return true;
}
void GameActor::SetSelectTarget( int nTargetId )
{
	ActorManager& actorMgr	= ActorManager::getSingleton();
	GameActor *pActor		= actorMgr.FindActor(nTargetId);
	if ( !canSelectTarget( pActor ) )
	{
		return;
	}

	if( pActor != NULL && (pActor->getType() == GAT_MAINPLAYER || pActor->getType() == GAT_PLAYER)
		&& pActor->m_MonsterMachine.m_playerInfo.m_nMonsterMemID != 0 )
	{
		pActor = ActorManager::getSingleton().FindActor( pActor->m_MonsterMachine.m_playerInfo.m_nMonsterMemID );
	}
	
	int nSelectTargetIdOld = m_SelectTargetId;
	m_SelectTargetId = pActor==NULL?0:pActor->GetID() ;
	if( nSelectTargetIdOld != m_SelectTargetId )
	{
		GameMsgPrint::getSingleton().showSelEffect();
		GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_CHANGED" );
		UpdateTargetDrawTitle( nSelectTargetIdOld );
		UpdateTargetDrawTitle( m_SelectTargetId );

		if( getType() == GAT_MAINPLAYER )
		{
			tagCSPkgBody msgbody;
			msgbody.PlayerMiscClt.PlayerMiscCltData.TargetID = m_SelectTargetId;
			msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_SELECT_TARGET_CLT;
			SendGameServerMsg( PLAYER_MISC_CLT,msgbody );
		}
		if( pActor != NULL && pActor->getType() == GAT_MAINPLAYER )
		{
			setHitTargetID( pActor->GetID() );
		}
	}
}

void GameActor::UpdateTargetDrawTitle( int nTargetID )
{
	//在特殊的副本不能显示
	if (IsInSpecialPworldNotShowTitle())
	{
		return;
	}
	if ( nTargetID == 0 )
	{
		return;
	}
	
	ActorManager& actorMgr	= ActorManager::getSingleton();
	GameActor* pTargetActor = actorMgr.FindActor( nTargetID );
	if ( pTargetActor == NULL )
	{
		return;
	}

	RoleText& actorTitleMgr = pTargetActor->getRoleText();
	actorTitleMgr.BuildTitle();
}

int GameActor::GetTargetID()
{
	return m_SelectTargetId;
}

void GameActor::addPopRich( const char* pszPopName )
{
	std::string strPopName		= std::string( pszPopName ) + std::string( "_Pop" );
	std::string strBackName		= std::string( pszPopName ) + std::string( "_PopBack" );
	UIObject* pRichBackGroup	= g_pFrameMgr->FindLayoutFrame( strBackName.c_str() );
	UIObject* pRichText			= g_pFrameMgr->FindLayoutFrame( strPopName.c_str() );
	addActorBindFrame( pRichBackGroup );
	addActorBindFrame( pRichText );
}

void GameActor::addActorBindFrame( UIObject* pFrame )
{
	if ( pFrame == NULL )
	{
		return;
	}

	if ( std::find( m_vecActorBindFrames.begin(), m_vecActorBindFrames.end(), pFrame ) != m_vecActorBindFrames.end() )
	{
		return;
	}

	m_vecActorBindFrames.push_back( pFrame );
}

const char* GameActor::GetModePathById(int nModelId)
{
	const tagNpcDisplayInfo& npcDisplayInfo = DBDataManager::getSingleton().GetNpcDisplayInfo( nModelId );
	FixedString modelpath = npcDisplayInfo.model;
	return modelpath;	

}

int GameActor::getIconId()
{
	if( m_Type == GAT_MONSTER ||  m_Type == GAT_NPC || m_Type == GAT_PET )
	{
		const tagMonsterDef* pMonsterDef = &DBDataManager::getSingleton().getMonsterDef( m_ResID );
		if( pMonsterDef != NULL )
		{
			return pMonsterDef->IconID;
		}
	}
	return 0;
}

void GameActor::startGray()
{
	if (getType() == GAT_MAINPLAYER)
	{
		if(DistortEffect::getSingletonPtr())
			DistortEffect::getSingleton().setGray(true);
		BloomEffect::getSingleton().playBlend();
	}
}

void GameActor::stopGray()
{
	if (getType() == GAT_MAINPLAYER)
	{
		if(DistortEffect::getSingletonPtr())
			DistortEffect::getSingleton().setGray(false);
		BloomEffect::getSingleton().stopBlend();
	}
}

bool GameActor::canPlayEffect()
{
	static const int s_fobitEffectStates[] = { GSACT_STALL };
	const int* pEnd = s_fobitEffectStates + ARRAY_ELEMENTS( s_fobitEffectStates );
	int nCurState = getActState().GetState();
	if ( std::find( s_fobitEffectStates, pEnd, nCurState ) != pEnd )
	{
		return false;
	}

	return true;
}

void GameActor::playAction(const char *pname)
{
	m_pShowActor->playAction(pname);	
}

void GameActor::playAction( int seqid, const char **pmotions, int num_motion, int motion_class )
{
	if( num_motion > 0 )
	{
		m_preSkillEffect = pmotions[0];
	}

	m_pShowActor->playAction( seqid, pmotions, num_motion, motion_class );
}

bool GameActor::hasAnim( int seq )
{
	return m_pShowActor->hasAnim( seq );
}

void GameActor::onMorph( int resId )
{
	Rider* pRider = getRider();
	if ( pRider->isOnHorse() )
	{
		pRider->requestRideDown();
	}
	requestBreakChant( eRide_Chant );
	requestBreakChant( ePet_Chant );

	m_BufStatus.clearAllEff();
	GameMsgPrint::getSingleton().showSelEffect( true );

	m_pShowActor->stopCurAction();
	m_pShowActor->onMorph( resId, m_pCurMap != NULL ? m_pCurMap->getShowGameMap() : NULL );
	m_nMorphResID = resId;
	m_pShowActor->setFaceTo(m_nAngle);
	
	// 摆摊状态不播放特效
	if ( canPlayEffect() )
	{
		m_BufStatus.reLoadAllEff();
		loadFairyEffect();
		setXuKongEffect();
	}
	
	getActState().setInState( GSACT_IN_MORPH, true );
	getActState().setPoseState( POSE_NONE );
	ChangeActState( getActState().GetState() );
	
	GameMsgPrint::getSingleton().showSelEffect();
	
	ActorManager& actorMgr		= ActorManager::getSingleton(); 
	MainGamePlayer* pMainplayer = actorMgr.getMainPlayer();
	int nHideOtherPlayerOption	= actorMgr.getShowOtherPlayerModelOption();
	if ( nHideOtherPlayerOption != HIDE_MODEL_NONE && getType() == GAT_PLAYER && 
			!pMainplayer->isInSameTeam( getTeamWID() ) )
	{
		if ( nHideOtherPlayerOption == HIDE_OTHER_PLAYER_AND_PET && isInStall() )
		{
			return;
		}

		if (m_pCurMap != NULL)
		{
			m_pShowActor->onLeaveMap( m_pCurMap->getShowGameMap() );
		}
	}
	if (getPet()->getPetHeti() != -1)
	{
		GameActor* fightPet = getPet()->getFightPet( getPet()->getPetHeti() );
		if ( fightPet->m_nBattePetFlag == 1 )
		{
			m_Pet.setBattlePetEntity( this ,fightPet->getPetGenius() );
		}
		else
		{
			m_Pet.setPetHetiEntity( this ,fightPet->getPetGenius() );
		}
		//getShowActor()->getCurEntity()->playMotion("32300_8", false, PET_HETI_SKILL_ID * 100);
	}
	else
	{
		getShowActor()->getCurEntity()->stopMotion(PET_HETI_SKILL_ID * 100);
	}
}

void GameActor::onRestore( int resId )
{
	m_BufStatus.clearAllEff();
	GameMsgPrint::getSingleton().showSelEffect( true );

	m_pShowActor->stopCurAction();
	m_nMorphResID = m_pShowActor->onRestore( resId, m_pCurMap != NULL ? m_pCurMap->getShowGameMap() : NULL );
	if (!m_pShowActor->isMorphing())
	{
		m_nMorphResID = 0;
	}
	m_pShowActor->setFaceTo(m_nAngle);
	// 摆摊状态不播放特效
	if ( canPlayEffect() )
	{
		m_BufStatus.reLoadAllEff();
		loadFairyEffect();
	}

	getActState().setInState( GSACT_IN_MORPH, false );
	if (m_nMorphResID == 0)
	{
		m_Equip.reLoadAllEquip();
	}

	if (getActState().GetState() != GSACT_DEAD)
	{
		ChangeActState( getActState().GetState() );
	}

	GameMsgPrint::getSingleton().showSelEffect();
	
	ActorManager& actorMgr		= ActorManager::getSingleton(); 
	MainGamePlayer* pMainplayer = actorMgr.getMainPlayer();
	int nHideOtherPlayerOption	= actorMgr.getShowOtherPlayerModelOption();
	
	if ( nHideOtherPlayerOption != HIDE_MODEL_NONE && getType() == GAT_PLAYER &&
			!pMainplayer->isInSameTeam( getTeamWID() ) )
	{
		if ( nHideOtherPlayerOption == HIDE_OTHER_PLAYER_AND_PET && isInStall() )
		{
			return;
		}

		if (m_pCurMap != NULL)
		{
			m_pShowActor->onLeaveMap( m_pCurMap->getShowGameMap() );
		}
	}
	if (getPet()->getPetHeti() != -1)
	{
		//getShowActor()->getCurEntity()->playMotion("32300_8", false, PET_HETI_SKILL_ID * 100);
		GameActor* fightPet = getPet()->getFightPet( getPet()->getPetHeti() );
		if ( fightPet->m_nBattePetFlag == 1 )
		{
			m_Pet.setBattlePetEntity( this ,fightPet->getPetGenius() );
		}
		else
		{
			m_Pet.setPetHetiEntity( this ,fightPet->getPetGenius() );
		}
	}
	else
	{
		getShowActor()->getCurEntity()->stopMotion( PET_HETI_SKILL_ID * 100 );
	}
}

void GameActor::loadFairyEffect()
{
	if ( m_Trump.m_nCurCampFairyId != 0 )
	{
		const tagFairyDef* pFairyDef = DBDataManager::getSingleton().getFairyDef( m_Trump.m_nCurCampFairyId, 
																					m_Trump.m_nCurCampFairyLv );
		if ( pFairyDef != NULL )
		{
			char szFairyEffect[256];
			sprintf( szFairyEffect, "%d_11", pFairyDef->ModeID );
			getShowActor()->getCurEntity()->playMotion( szFairyEffect, false, pFairyDef->ModeID );
			m_nCurFairyEffect = pFairyDef->ModeID;
		}
	}
	else
	{
		getShowActor()->getCurEntity()->stopMotion( m_nCurFairyEffect );
	}
}

AvatarEntity* GameActor::getAvatar()
{
	if (IS_KIND_OF(AvatarEntity, m_pShowActor->getCurEntity()))
	{
		return (AvatarEntity*)m_pShowActor->getCurEntity();
	}
	return NULL;
}

bool GameActor::isJumping()
{
	if (getShowActor()->getCurEntity()->hasAnimPlaying(10133) ||
		getShowActor()->getCurEntity()->hasAnimPlaying(10134) ||
		getShowActor()->getCurEntity()->hasAnimPlaying(10137) ||
		getShowActor()->getCurEntity()->hasAnimPlaying(10138))
	{
		return true;
	}

	return false;
}

void GameActor::jump()
{
	const char* szFootString[4];
	std::string strFoot( "footprint" );
	szFootString[0] = strFoot.c_str();

	m_nLastJumpTime = timeGetTime();
	if( getActState().GetState() == GSACT_STAND )
	{
		if (rand() % 10 < 8)
		{
			playAction( 10133, NULL, 0, 0 );
		}
		else
		{
			playAction( 10137, NULL, 0, 0 );
		}
		m_nLastJumpTime += STAND_JUMP;
	}

	if( getActState().GetState() == GSACT_MOVE )
	{
		if (rand() % 10 < 8)
		{
			playAction( 10134, NULL, 0, 0 );
		}
		else
		{
			playAction( 10138, NULL, 0, 0 );
		}
	}

	m_fJumpTime = 0.0f;
	m_bStartJump = true;
}

void GameActor::registerTickEvent( GameActor_Tick* pTick )
{
	m_TickList.push_back( pTick );
}

// 查看事件是否处理完
bool GameActor::haveTickEvent( GameActor_Tick* pTick )
{
	for( std::list<GameActor_Tick*>::iterator iter = m_TickList.begin(); iter != m_TickList.end(); ++iter )
	{
		if( *iter == pTick )
		{
			return true;
		}
	}
	return false;
}

void GameActor::setSpeed(int s)
{
	if( m_nSpeed == 0 && s != 0 && m_ActState.GetState() == GSACT_STOP )
	{
		ChangeActState( GSACT_STAND );
	}

	if( s == 0 && m_nSpeed != 0 && m_ActState.GetState() != GSACT_DEAD )
	{
		if ( canAutoTalkToNpc() )
		{
			endAutoTalkToNpc();
		}

		ChangeActState( GSACT_STAND );
	}

	if (m_nSpeed != s && m_ActState.GetState() == GSACT_MOVE)
	{
		float fRadio = (float)s / NOMAL_SPEED;
		if (fRadio < 1 && (getType() == GAT_MONSTER || getType() == GAT_NPC || getType() == GAT_PET))
		{
			fRadio = 1;
		}
		m_pShowActor->getCurEntity()->setModePlayRadio( fRadio );
	}

	m_pMapMover->SetSpeed((float)s);
	m_nSpeed = s;

	if( s != 0  && m_ActState.GetState() == GSACT_MOVE)
	{
		ChangeActState( GSACT_MOVE );
	}

	if ( getType() == GAT_MAINPLAYER )
	{
		CSATTRIDVAL attrVal;
		attrVal.MSpd = s;
		setAttriValue( ATTR_ID_MSPD, attrVal );
		if ( m_nSpeed == 0 )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_MAINPLAYER_SPEED_ZERO" );
		}
	}
}

int GameActor::getNpcQuestShow()
{
	return 0;
}

// 获取人物当前的X和Z坐标
tagPlayerPosInfo& GameActor::getPlayerPostion()
{
	m_PlayerPosInfo.m_nPosX = m_Pos.x / 1000;
	m_PlayerPosInfo.m_nPosZ = m_Pos.z / 1000;
	return m_PlayerPosInfo;
}

GamePet* GameActor::getPet()
{
	return &m_Pet;
}

GameQiYue& GameActor::getQiYue()
{
	return m_QiYue;
}
// 获取法宝
GameTrump* GameActor::getTrump()
{
	return &m_Trump;
}

// 获取藏宝图信息
GameCBT& GameActor::getCBTInfo()
{
	return m_CangBaoTuInfo;
}

int GameActor::getOverHead()
{
	return 0;
	return m_nOverHead;
}

void GameActor::setPrepareSkill(int id /*= 0*/, int lv /*= 0*/, int index /*= 0*/, unsigned int time /*= 0*/)
{
	unsigned int total = 0;
	if (id != 0)
	{
		const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef( id, lv );
		if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT)
		{
			total = SkillDef.JiqiTime * 100;
		}
		else
		{
			total = SkillDef.PrepareTime * 100;
		}
	}
	setPrepareSkill(id, lv, index, time, total);
}

void GameActor::setPrepareSkill(int id, int lv, int index, unsigned int time, unsigned int total)
{
	m_nPrepareSkill = id;
	if (id != 0)
	{
		m_uPrepareTime = time;
		m_nTotalPrepareTime = total;
		GameMsgPrint::getSingleton().SendUIEvent("GE_OTHER_PREPARE");
	}
}

void GameActor::setPilotSkill(int id /*= 0*/)
{
	m_nPilotSkill = id;
}

int GameActor::getPrepareTime()
{
	if (m_nPrepareSkill == 0)
	{
		return 0;
	}
	return timeGetTime() - m_uPrepareTime;
}

int GameActor::getTotalPrepareTime()
{
	if (m_nPrepareSkill == 0)
	{
		return 0;
	}
	return m_nTotalPrepareTime;
}

void GameActor::setSkillTetanic(unsigned int time)
{
	time += timeGetTime();
	if (time > m_skillTetanicTime)
	{
		m_skillTetanicTime = time;
	}
	getActState().setInState( GSACT_IN_TETANIC, true );
}

void GameActor::CmdMovePause()
{
	int x, y;
	m_pMapMover->GetCurPos(x, y);
	m_pMapMover->StopOnPoint( x, y );
	ChangeActState(GSACT_STAND); 
}

void GameActor::CmdMoveStop()
{
	CmdMovePause();
};

void GameActor::setDeathAction(IActorDeathAction *action)
{
	m_lstDeathAction.push_back(action);
}

void GameActor::setPetDiathesis( int nNum, const CSPETDIATHESISVIEW* pDiathesisDatas )
{
	m_petDiathesisInfo.m_nNum = nNum;
	for ( int i = 0; i < nNum; ++i )
	{
		m_petDiathesisInfo.m_diathesisData[i].m_nID		= pDiathesisDatas[i].ID;
		m_petDiathesisInfo.m_diathesisData[i].m_nLevel	= pDiathesisDatas[i].Level;
	}
}

void GameActor::setPetDiathesis( int nMAXNum, const PETSKILLGRID* pDiathesisGrides )
{
	int nCount = 0;
	for ( int i = 0; i < nMAXNum; ++i )
	{
		const tagDiathesisDef* pDiathesisDef  = &DBDataManager::getSingleton().getDiathesisDef( 
													pDiathesisGrides[i].SkillID, pDiathesisGrides[i].SkillLevel );
		if ( pDiathesisDef == NULL )
		{
			continue;
		}

		m_petDiathesisInfo.m_diathesisData[nCount].m_nID	= pDiathesisGrides[i].SkillID;
		m_petDiathesisInfo.m_diathesisData[nCount].m_nLevel	= pDiathesisGrides[i].SkillLevel;
		++nCount;
	}

	m_petDiathesisInfo.m_nNum = nCount;
	if( getOwnerId() == ActorManager::getSingleton().getMainPlayer()->GetID() )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_BUF_LIST_UPDATE" );
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAINPET_BUF_LIST_UPDATE" );
	}
}

bool GameActor::needAddButtonName()
{
	if ( getType() != GAT_ITEM )
	{
		return false;
	}

	const tagItemDef& itemDef = DBDataManager::getSingleton().getItemDef( getResId() );
	if ( &itemDef == NULL )
	{
		return false;
	}

	return itemDef.ItemTmp.Type == ITEM_TMP_NO;
}

void GameActor::setPetFightMode( int nFightMode )
{
	if ( nFightMode == m_petDiathesisInfo.m_nFightMode )
	{
		return;
	}

	m_petDiathesisInfo.m_nFightMode = nFightMode;
	int nOwnerId				= getOwnerId();
	GameActor* pOwner			= ActorManager::getSingleton().FindActor( nOwnerId );
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	
	if ( pOwner == pMainPlayer )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_UPDATE_MAIN_PET_FIGHT_MODE" );
	}
	
	if ( nOwnerId != 0 && nOwnerId == pMainPlayer->GetSelectTarget() )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_UPDATE_TARGET_PLAYER_PET_FIGHT_MODE" );
	}

	GameMsgPrint::getSingleton().updateActorPetStatus( pOwner, true );
}

int  GameActor::GetCatchPet()
{
	return m_nTargetCatchPetID;
}

void GameActor::onTurn(int x, int z)
{
	const float TurnDelay = 0.2f;
	if (getType() == GAT_MONSTER)
	{
		setSkillTetanic(int(TurnDelay * 1000 + 0.5f));
	}
	else
	{
		setSkillTetanic(0);
	}
	int cur_x, cur_z;
	m_pMapMover->GetCurPos(cur_x, cur_z);

	if( getType() == GAT_MONSTER && getSubType() == MON_SUB_TYPE_MACHINE )
	{
		return;
	}
	if (x != cur_x || z != cur_z)
	{
		GameMaskPath path;
		path.addPathPoint(cur_x, cur_z);
		path.addPathPoint(x, z);
		m_pMapMover->SetFixSpeed((float)path.getPathLength() / TurnDelay);

		OnPathMove(&path);
		for(int i = 0; i < PET_MAX_FIGHT_SHI; i ++)
		{
			GameActor* pActor = m_Pet.getPetActor(i);
			if( pActor != NULL )
			{
				pActor->m_Pet.setMoveToPoint(x, z);
			}
		}
	}else if( getType() == GAT_PLAYER ){
		m_pMapMover->StopOnPoint( x, z );
		ChangeActState(GSACT_STAND);
	}
}

void GameActor::onUpdateSkill(int skillid)
{
	getShowActor()->getCurEntity()->playMotion( "9505_16", true, skillid);
}

bool GameActor::hasFashion()
{
	return m_Equip.m_ArmEquipId[EQUIP_POS_FASHION_BODY] != 0;
}

bool GameActor::canPlayChatAction()
{
	if( m_ActState.GetState() == GSACT_STAND )
	{
		for( int i = MAX_ACT_STATE; i < MAX_IN_STATE ; i++ )
		{
			if( m_ActState.getInState(i) )
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

void GameActor::playChatAction(int actionID,bool bReset)
{
	if( bReset )
	{
		onChangeStand();
	}
	if( actionID > 0 )
	{
		m_Equip.hideWeapon();
		playAction(actionID,"");
		m_nChatActionID = actionID;
	}
}

int GameActor::getPilotSkill()
{
	return m_nPilotSkill;
}

void GameActor::hide(bool b /*= true*/)
{
	m_isShow = !b;
	if (b)
	{
		getShowActor()->onLeaveMap( getCurMap()->getShowGameMap());
		if( m_Rider.m_pShowActor != NULL )
		{
			m_Rider.m_pShowActor->onLeaveMap( getCurMap()->getShowGameMap() );
		}
		GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		//处理 目标的目标
		if( pMainPlayer != NULL )
		{
			GameActor* pActor = ActorManager::getSingleton().FindActor( pMainPlayer->GetSelectTarget() );
			if( pActor != NULL && pActor->getHitTargetID() == GetID() )
			{
				pActor->setHitTargetID( 0 );
			}
		}
		if (pMainPlayer && pMainPlayer->GetSelectTarget() == GetID())
		{
			pMainPlayer->SetSelectTarget(0);
		}
		if (m_pSmallDecal )
		{
			m_pSmallDecal->show( false );
		}
	}
	else
	{
		getShowActor()->onEnterMap( getCurMap()->getShowGameMap());
		if( m_Rider.m_pShowActor != NULL )
		{
			m_Rider.m_pShowActor->onEnterMap( getCurMap()->getShowGameMap() );
		}
	}
}

void GameActor::hideWeapon(unsigned int time)
{
	unsigned int t = timeGetTime() + time;
	if (m_showWeaponTime < t)
	{
		m_Equip.hideWeapon();
		m_showWeaponTime = t;
	}
}

void GameActor::hideWeapon(bool b)
{
	m_isShowWeapon = !b;
	if (b)
	{
		m_Equip.hideWeapon();
	}
	else if (timeGetTime() >= m_showWeaponTime)
	{
		m_Equip.showWeapon();
	}
}

bool GameActor::isOnMonsterMachine()
{
	if( m_MonsterMachine.m_playerInfo.m_nMonsterMemID >0 )
	{
		return true;
	}
	return false;
}

bool GameActor::isInCarry()
{
	return getActState().getInState(GSACT_IN_CARRY);
}

GameActor_PKEffect::~GameActor_PKEffect()
{
	if( m_pEntity != NULL )
	{
		//m_pEntity->detachFromScene();
		OGRE_RELEASE(m_pEntity);
	}
}

void GameActor_PKEffect::setState(int nState)
{
	m_nState = nState;
	if( m_pEntity != NULL )
	{
		m_pEntity->detachFromScene();
		OGRE_RELEASE(m_pEntity);
	}
	if( nState < EFFECT_STATE_MAX )
	{
		m_pEntity = new Ogre::Entity;
		m_pEntity->load( EffectPath[m_nState].c_str(), true );
		if( nState == EFFECT_STATE_START && m_pEntity->getIthMotion(0) != NULL)
		{
			m_pEntity->getIthMotion(0)->m_loop = ML_ONCE_STOP;
		}		
	}else{
		if( (m_pOwner->getActState().GetState() == GSACT_STAND || m_pOwner->getActState().GetState() == GSACT_ATTACK)
			&& !m_pOwner->isInRide() && !m_pOwner->isInMorph() )
		{
			if( m_nVictory == 0 )
			{
				m_pOwner->playAction( 10799 );
			}else if(m_nVictory == 1){
				m_pOwner->playAction( 10798 );
			}
		}
		m_nVictory = -1;		
	}
	if( m_pEntity != NULL )
	{
		int y;
		m_pOwner->getCurMap()->getTerrainH(m_nX, m_nZ, y);
		GameXYZ pos = GameXYZ(m_nX, y, m_nZ);
		m_pEntity->setPosition(pos);			
		m_pEntity->update(0);
		m_pEntity->attachToScene( m_pOwner->getCurMap()->getShowGameMap()->getGameScene() );
	}
}

void GameActor_PKEffect::onUpdate(unsigned int dtick)
{
	if( m_pEntity != NULL )
	{
		m_pEntity->update(dtick);
		if( !m_pEntity->IsPlaying() )
		{
			setState( ++m_nState );
		}
	}
}

void GameActor_PKEffect::setPos( int x,int z )
{
	m_nX = x;
	m_nZ = z;
}

bool GameActor::isInSafeOverlay()
{
	return (m_nSafeOverlayFlag >0 ) && !(m_nSafeOverlayFlag&SCENEOVERLAY_PK);
}

bool GameActor::isNeedShowSafeAreaBuf()
{
	return m_nSafeOverlayFlag&SCENEOVERLAY_MAP;
}

int	GameActor::getPetGenius()
{
	return m_nPetGenius;
}

int GameActor::getPetGeniusId()
{
	return m_nPetGeniusId;
}

int GameActor::getPetGeniusLv()
{
	return m_nPetGenniusLv;
}

bool GameActor::isRollItem()
{
	return m_RollType != ITEM_ROLL_NONE ;
}

void GameActor::setRollType( int nType )
{
	m_RollType = nType;
}

void GameActor::onAFK(bool b)
{
	m_isAFK = b;
	if (getActState().GetState() != GSACT_DEAD && !isInRide() && !isInMorph())
	{
		playAFK(b);
	}
}

bool GameActor::isAFK()
{
	return m_isAFK;
}

bool GameActor::isShowHead()
{
	return SHOW_HEAD_FLAG & m_nShowFlag;
}

bool GameActor::isShowFashion()
{
	return (SHOW_FASHION_FLAG & m_nShowFlag)== 0?false:true ;
}

bool GameActor::isShowBack()
{
	return (SHOW_BACK_FLAG & m_nShowFlag) == 0?false:true ;
}

bool GameActor::isShowFace()
{
	return ( SHOW_FACE_FLAG & m_nShowFlag ) == 0 ? false:true; 
}

bool GameActor::isShowLove()
{
	return ( SHOW_WEDDING_DSN_FLAG & m_nShowFlag ) == 0 ? false:true; 
}

void GameActor::sitdown()
{
	ChangeActState( GSACT_SIT );
	playAction( 10201, NULL, 0, 0 );
}

void GameActor::situp()
{
	Rider* pRider = getRider();
	if (!pRider->isPre())
	{
		if (getActState().GetState() == GSACT_SIT)
		{
			ChangeActState( GSACT_STAND );
		}
		m_Equip.showWeapon();
	}
}

bool GameActor::hasOtherBooty()
{
	if( m_BootyInfo.BootyOwnerWid == 0 )
	{
		return false;
	}
 	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
 	if (pMainPlayer == NULL)
 	{
 		return true;
 	}
	if( m_BootyInfo.BootyOwnerType == MON_BOOTY_OWNER_PLAYER )
	{
		if( pMainPlayer->getRoleWId() == m_BootyInfo.BootyOwnerWid )
		{
			return false;
		}
	}else if( m_BootyInfo.BootyOwnerType == MON_BOOTY_OWNER_TEAM )
	{
		if( pMainPlayer->isInTeam() && pMainPlayer->getTeamWID() == m_BootyInfo.BootyOwnerWid )
		{
			return false;
		}
	}else if( m_BootyInfo.BootyOwnerType == MON_BOOTY_OWNER_CLAN )
	{
		if( pMainPlayer->getClanWID() == m_BootyInfo.BootyOwnerWid )
		{
			return false;
		}
	}
	return true;
}

int GameActor::getCurActionId()
{
	if( m_pShowActor )
	{
		return m_pShowActor->getCurAction();
	}
	return -1;
}

void GameActor::addPopWindow( const char* szText )
{
	g_pFrameMgr->addPopWindow( "ChatPop", getName(), szText, -100, -100, 8, 
		"PopBackArrowTexture", true, 0, 0 );
}

unsigned int GameActor::getClanColor()
{
	return m_clanColor;
}

void GameActor::setClanColor(  unsigned int nClanColor )
{
	m_clanColor = nClanColor;
	m_RoleText.UpdateTextColor();
}

void GameActor::setClanAlly()
{
	m_clanRelation = CLAN_ALLY;
	setClanColor( CLAN_TITLE_ALLY_COLOR );
}

void GameActor::setClanSame()
{
	m_clanRelation = CLAN_SAME;
	setClanColor( CLAN_TITLE_MY_COLOR );
}

void GameActor::setClanEnemy()
{
	m_clanRelation = CLAN_ENEMY;
	setClanColor( CLAN_TITLE_ENEMY_COLOR );
}

void GameActor::resetClan()
{
	m_clanRelation = CLAN_OTHER;
	setClanColor( CLAN_TITLE_OTHER_COLOR );
}

bool GameActor::isClanSame()
{
	return m_clanRelation == CLAN_SAME;
}

bool GameActor::isClanAlly()
{
	return m_clanRelation == CLAN_ALLY;
}

void GameActor::setTeamColor(int r, int g, int b)
{
	Entity *entity = m_pShowActor->getCurEntity();
	entity->enableUVMask(true, true);
	entity->setUVMaskSpeed(Vector2(0));
	entity->setUVMaskColor(Vector3(r / 255.0f, g / 255.0f, b / 255.0f));
	entity->setLiuGuangTexture("uires\\TuPianLei\\TeamColor.bmp");
}

void GameActor::playAFK(bool b)
{
	static const int MOTIONCLASS = 998959872;
	if (b)
	{
		m_pShowActor->getCurEntity()->playMotion("1009101", false, MOTIONCLASS);
		sitdown();
	}
	else
	{
		m_pShowActor->getCurEntity()->stopMotion(MOTIONCLASS);
		situp();
	}
}

bool GameActor::isRideMonster( GameActor* pActor )
{
	if(pActor != NULL && pActor->getType() == GAT_MONSTER && pActor->getSubType() == MON_SUB_TYPE_MACHINE )
	{
		for( int i = 0 ; i < pActor->m_MonsterMachine.m_monsterInfo.m_nMaxPassenger ; i++ )
		{
			if( GetID() == pActor->m_MonsterMachine.m_monsterInfo.m_nPassengerID[i] )
			{
				return true;
			}
		}
	}
	return false;
}

void GameActor::OnStall(bool bStall)
{
	if (bStall)
	{
		onMorph(350012);
	}
	else
	{
		onRestore(350012);
	}
}

void GameActor::optimizeClear(bool show /*= false*/)
{
	if (m_pCurMap == NULL)
	{
		return;
	}
	if (m_optimizeClear != show)
	{
		return;
	}
	m_optimizeClear = !show;
	if (show && m_isShow)
	{
		if ( ActorManager::getSingleton().canShowActorModelInScene( this ) && isOptimizeDisplay() )
		{
			if (m_lastOptimizeTime != 0)
			{
				m_pShowActor->update(timeGetTime() - m_lastOptimizeTime);
				m_lastOptimizeTime = 0;
			}
			m_pShowActor->onEnterMap(m_pCurMap->getShowGameMap());
			if (m_Rider.m_pShowActor != NULL)
			{
				m_Rider.m_pShowActor->onEnterMap(m_pCurMap->getShowGameMap());
			}
			m_pSmallDecal->show(true);
			ActorManager::getSingleton().removeActorFade(this);
			m_pShowActor->getCurEntity()->setTransparent(1);
			for(int i = 0; i < PET_MAX_FIGHT_SHI; ++i)
			{
				GameActor* pActor = m_Pet.getPetActor(i);
				if (pActor != NULL)
				{
					pActor->optimizeClear(show);
				}
			}
		}
	}
	else
	{
		if (m_lastOptimizeTime == 0)
		{
			m_lastOptimizeTime = timeGetTime();
		}
		m_pShowActor->onLeaveMap(m_pCurMap->getShowGameMap());
		if (m_Rider.m_pShowActor != NULL)
		{
			m_Rider.m_pShowActor->onLeaveMap(m_pCurMap->getShowGameMap());
		}
		m_pSmallDecal->show(false);
		ActorManager::getSingleton().removeActorFade(this);
		for(int i = 0; i < PET_MAX_FIGHT_SHI; ++i)
		{
			GameActor* pActor = m_Pet.getPetActor(i);
			if (pActor != NULL)
			{
				pActor->optimizeClear(show);
			}
		}
	}
}

void GameActor::optimizeDisplay(bool show /*= false*/)
{
	if (m_optimizeShow == show)
	{
		return;
	}
	m_optimizeShow = show;
	if (m_pCurMap == NULL)
	{
		return;
	}
	if (show)
	{
		if (ActorManager::getSingleton().canShowActorModelInScene(this) && !isOptimizeClear())
		{
			if (m_lastOptimizeTime != 0)
			{
				m_pShowActor->update(timeGetTime() - m_lastOptimizeTime);
				m_lastOptimizeTime = 0;
			}
			m_pShowActor->onEnterMap(m_pCurMap->getShowGameMap());
			if (m_Rider.m_pShowActor != NULL)
			{
				m_Rider.m_pShowActor->onEnterMap(m_pCurMap->getShowGameMap());
			}
			m_pSmallDecal->show(true);
			ActorManager::getSingleton().addActorFade(this, true);
			for(int i = 0; i < PET_MAX_FIGHT_SHI; ++i)
			{
				GameActor* pActor = m_Pet.getPetActor(i);
				if (pActor != NULL && pActor->getCurMap() != NULL)
				{
					pActor->getShowActor()->onEnterMap(m_pCurMap->getShowGameMap());
					ActorManager::getSingleton().addActorFade(pActor, true);
				}
			}
		}
	}
	else
	{
		ActorManager::getSingleton().addActorFade(this, false);
		for(int i = 0; i < PET_MAX_FIGHT_SHI; ++i)
		{
			GameActor* pActor = m_Pet.getPetActor(i);
			if (pActor != NULL && pActor->getCurMap() != NULL)
			{
				ActorManager::getSingleton().addActorFade(pActor, false);
			}
		}
	}
}

// 判断是否为采集车 2012.04.11
bool GameActor::isCollectMachine( )
{
	if ( getType( ) == GAT_MONSTER )
	{
		if ( getSubType( ) == MON_SUB_TYPE_MACHINE/*采集车*/ )
		{
			return true;
		}
	}
	return false;
}

void GameActor::setHitTargetID( int nTargetID )
{
// 	if( getType() != GAT_MONSTER /*|| m_nHitTargetID == nTargetID*/ )
// 	{
// 		return ;
// 	}
	m_nHitTargetID = nTargetID;
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain != NULL && pMain->GetTargetID() == GetID() )
	{
		char buf[256];
		sprintf(buf,"OnChangeHitTargetID( %d )",nTargetID);
		ActorManager::getSingleton().getScriptVM()->callString(buf);
	}
}

void GameActor::UpdateMin(unsigned int dtick)
{
	unsigned int uCurTime = timeGetTime();

	if (m_mustUpdateUI)
	{
		m_mustUpdateUI = false;
		actualUpdateUI();
	}

	for (std::map<int, tagBackInfo>::iterator i = m_BackInfoList.begin(); i != m_BackInfoList.end();)
	{
		if (uCurTime > i->second.startTime)
		{
			MainGamePlayer *player = ActorManager::getSingleton().getMainPlayer();
			if (i->second.actId != 0)
			{
				const char *effects[1];
				int effectnum = 0;
				if (!i->second.effect.empty())
				{
					effectnum = 1;
					effects[0] = i->second.effect.c_str();
				}
				int actId = m_ActState.getActionId(i->second.actId);
				if (!hasAnim(actId))
				{
					actId = getActId(POSE_NONE, i->second.actId);
				}
				if (isOnMonsterMachine())
				{
					GameActor *pActor = ActorManager::getSingleton().FindActor(m_MonsterMachine.m_playerInfo.m_nMonsterMemID);
					if (pActor != NULL)
					{
						pActor->playAction(getActId(POSE_NONE, i->second.actId), effects, effectnum, 0);
					}
				}
				else if (isInMorph())
				{
					//变身状态下没有动作的时候，播放默认动作, hit的时候不播放
					if (!m_pShowActor->hasAnim(actId))
					{
						playAction(getActId(POSE_NONE, 400), effects, effectnum, 0);
					}
					else
					{
						playAction(actId, effects, effectnum, 0);
					}
				}
				else
				{
					if( m_Rider.isOnHorse() )
					{
						m_Rider.horsePlayAct(getActId(POSE_NONE, i->second.actId), effects, effectnum, 0);
					}else{
						playAction( actId, effects, effectnum, 0 );
					}
				}
			}else if( !i->second.effect.empty() ){
				playMotion(i->second.effect.c_str(), 0);
			}
			i->second.actId = 0;
			i->second.effect.clear();
			if (uCurTime > i->second.startTime + 100)
			{
				m_pSpecialMove->doBack(this, i->second.mapid, i->second.x, i->second.z, i->second.backTime, i->second.type, i->second.height, i->second.speed);
				if (this == player)
				{
					if (i->second.passive)
					{
						BloomEffect::getSingleton().playBlend(450);
					}
					else
					{
						player->addMoveSeq(50);
					}
				}
				if (i->second.type == BACK_CIRCLE)
				{
					m_pSpecialMove->setCircleInfo(this, i->second.extra.circle.x, i->second.extra.circle.y,
						i->second.extra.circle.inverted);
				}
				//playAction( m_ActState.getAttackFlyActId(), NULL, 0, -1 );
				i = m_BackInfoList.erase(i);
				continue;
			}
		}
		++i;
	}

	if( m_skillTetanicTime != 0 && uCurTime >= m_skillTetanicTime )
	{
		m_skillTetanicTime = 0;
		getActState().setInState( GSACT_IN_TETANIC, false );
		if (m_ActState.GetState() == GSACT_MOVE)
		{
			onChangeMove();
		}
	}

	if (m_pCurMap == NULL) return;

	// pet
	m_Pet.updatePetMoveSpe();

	if( isOnMonsterMachine() || getSubType() == MON_SUB_TYPE_MACHINE  )
	{
		m_MonsterMachine.Update();
	}
	int x, y, z;
	if (m_pMapMover->isReach())
	{
		GameXYZ pos = GetPosition();
		x = pos.x;
		y = pos.y;
		z = pos.z;
	}
	else
	{
		m_pMapMover->Update(Ogre::TickToTime(dtick));
		m_pMapMover->GetCurPos(x, z);
		x*=10;
		z*=10;
		m_pCurMap->getTerrainH(x, z, y);
		y += m_pShowActor->getActorOffsetZ();
	}
	if (GhostSchema::isGhost && this == ActorManager::getSingleton().getMainPlayer())
	{
		m_Pos = GameXYZ(x, GhostSchema::height, z);
		m_pShowActor->setPosition(m_Pos.x, m_Pos.y, m_Pos.z);
	}
	else if( m_pSpecialMove->getType() == MOVETYPE_NOMAL )
	{
		m_Pos = GameXYZ(x, y, z);
		m_pShowActor->setPosition(m_Pos.x, m_Pos.y, m_Pos.z);
	}else if( m_pSpecialMove->getType() == MOVETYPE_FLY )
	{
		m_pSpecialMove->update( this, dtick );
		y = m_pSpecialMove->getPointHeight( this, GameXYZ( GetPosition().x/10, GetPosition().z/10, GetPosition().y/10 ) );
		m_Pos = GameXYZ(x, y*10, z);
		m_pShowActor->setPosition(m_Pos.x, m_Pos.y, m_Pos.z);
	}

	m_pSpecialMove->update( this, dtick );

	//TODO HERE
	m_Rider.Update( dtick );

	m_BufStatus.update(dtick);

	UpdateRotateState( dtick );

	for( std::list<GameActor_Tick*>::iterator iter = m_TickList.begin(); iter != m_TickList.end(); )
	{
		GameActor_Tick* pTick = *iter;
		if( pTick->updateTick( uCurTime ) == GameActor_Tick::TICK_END )
		{
			iter = m_TickList.erase( iter );
			delete pTick;
			continue;
		}
		iter++;
	}
	m_pCarry->update(dtick);
	m_pkEffect.onUpdate(dtick);
	if( m_pChantPart )
	{
		if( m_pChantPart->OnUpdate() )
		{
			m_pChantPart->OnBreak();
			OGRE_DELETE( m_pChantPart );
		}
	}
}

bool GameActor::canPickUp()
{
	if( getType() != GAT_ITEM )
	{
		return false;
	}
	MainGamePlayer* pMain = ActorManager::getSingleton().getMainPlayer();
	if( pMain == NULL )
	{
		return false;
	}
	if( getRoleWId() != 0 )
	{
		return pMain->getRoleWId() == getRoleWId();
	}if( getTeamWID() != 0 )
	{
		return pMain->getTeamWID() == getTeamWID();
	}if( getClanWID() != 0 )
	{
		return pMain->getClanWID() == getClanWID();
	}
	return isRollItem();
}

bool GameActor::isInSkillTetanic()
{
// 	if( m_skillTetanicTime != 0 && timeGetTime() >= m_skillTetanicTime )
// 	{
// 		m_skillTetanicTime = 0;
// 		getActState().setInState( GSACT_IN_TETANIC, false );
// 		if (m_ActState.GetState() == GSACT_MOVE)
// 		{
// 			onChangeMove();
// 		}
// 	}
	return getActState().getInState(GSACT_IN_TETANIC);
}

void GameActor::setBuildTime(unsigned int remain)
{
	if (remain > 0)
	{
		int sec, usec;
		ServerTimer::getSingleton().getServerTime(sec, usec);
		m_BuildFinishTime = sec * 1000 + usec / 1000 + remain;
		onMorph(m_ResID * 100);
	}
}
int GameActor::getMonsterPhysicType()
{
	if( getType() == GAT_MAINPLAYER || getType() == GAT_PLAYER )
	{
		return 0;
	}
	const tagMonsterDef* pMonsteDef = DBDataManager::getSingleton().getMonsterDefPtr(getResId());
	if( pMonsteDef == NULL )
	{
		return 0;
	}
	return pMonsteDef->ShowType;
}

void GameActor::setSubType( int nSubType )
{
	m_nSubType = nSubType;
}

int GameActor::getServerId()
{
	const char* pszID = strpbrk( getName(), "$" );
	if( pszID != NULL )
	{
		pszID = getName() + 1;
		return atoi( pszID );
	}
	return -1;
}

void GameActor::startUseItemChant( unsigned int chantTime, int nItemID, int nTargetID, int nListType, int nGridIdx )
{
	UseItemChant* pChant = new UseItemChant(this,timeGetTime(),chantTime,nItemID,nTargetID,nListType,nGridIdx);
	setChantData( pChant );
}

void GameActor::startTrumpChant( int nHeighId, int nLowId, int nGridIdx )
{
	TrumpChant* pChant = new TrumpChant( this,timeGetTime(),nHeighId,nLowId,nGridIdx );
	setChantData(pChant);
}

void GameActor::startRideChant( int nRideId, int nLevel )
{
	RideChant* pChant = new RideChant(this,timeGetTime(),nRideId,nLevel);
	setChantData( pChant );
}

void GameActor::startPetChant( tdr_longlong wid )
{
	PetChant* pChant = new PetChant(this,timeGetTime(),wid);
	setChantData( pChant );
}

void GameActor::startSecondaryChant( unsigned int uChantTime, int nType, int nId )
{
	SecondaryChant* pChant = new SecondaryChant(this,timeGetTime(),uChantTime,nType,nId);
	setChantData( pChant );
}

void GameActor::startResolveChant( int nListType, int nGridIdx, tdr_ulonglong Wid, int nType )
{
	ResolveChant* pChant = new ResolveChant( this, timeGetTime(), nListType, nGridIdx, Wid, nType );
	setChantData( pChant );
}

void GameActor::setChantData( IChant_Base* pChant )
{
	if( m_pChantPart != NULL )
	{
		m_pChantPart->OnBreak();
	}
	OGRE_DELETE( m_pChantPart );
	if( pChant->OnStart() )
	{
		m_pChantPart = pChant;
	}
}

void GameActor::requestBreakChant( int nType )
{
	if( isInPre(nType) )
	{
		if( m_pChantPart->RequestBreak() )
		{
			m_pChantPart->OnBreak();
			OGRE_DELETE( m_pChantPart );
		}
	}
}

void GameActor::finishChant( int nType )
{
	if( isInPre(nType) )
	{
		if( m_pChantPart->OnFinish() )
		{
			m_pChantPart->OnBreak();
			OGRE_DELETE(m_pChantPart);
		}
	}
}

void GameActor::onBreakChant( int nType )
{
	if( isInPre(nType) )
	{
		m_pChantPart->OnBreak();
		OGRE_DELETE(m_pChantPart);
	}
}

bool GameActor::isInPre( int nType )
{
	if( m_pChantPart && m_pChantPart->GetType() == nType )
	{
		return true;
	}
	return false;
}

bool GameActor::canStartChant( int nType )
{
	if( m_pChantPart == NULL )
	{
		return true;
	}
	
	//暂时先这样弄
	switch( nType )
	{
	case eUseItem_Chant:
		{
			if( m_pChantPart->GetType() == eUseItem_Chant )
			{
				return false;
			}
		}
		break;
	case eRide_Chant:
		{
			if( m_pChantPart->GetType() == eUseItem_Chant )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_RIDE_PRE_FAIL_USEITEM );
				return false;
			}else if( m_pChantPart->GetType() == ePet_Chant )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_RIDE_PRE_FAIL_PET );
				return false;
			}
		}
		break;
	case eTrump_Chant:
		{
			if( m_pChantPart->GetType() == eUseItem_Chant )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PRE_FAIL_USEITEM );
				return false;
			}else if( m_pChantPart->GetType() == ePet_Chant )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PRE_FAIL_PET );
				return false;
			}else if(m_pChantPart->GetType() == eRide_Chant )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_TRUMP_PRE_FAIL_RIDEPRE );
				return false;
			}
		}
		break;
	case ePet_Chant:
		{
			if( m_pChantPart->GetType() == eUseItem_Chant )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_USEITEM );
				return false;
			}else if( m_pChantPart->GetType() == eRide_Chant )
			{
				GameMsgPrint::getSingleton().showMidTip( MID_PET_PRE_FAIL_RIDEPRE );
				return false;
			}
		}
		break;
	case eResolve_Chant:
		{

		}
		break;
	case eSecondary_Chant:
		{

		}
		break;
	default:
		break;
	}
	requestBreakChant( m_pChantPart->GetType() );
	return true;
}

void GameActor::BreakCurChant()
{
	if( m_pChantPart )
	{
		if( m_pChantPart->RequestBreak() )
		{
			m_pChantPart->OnBreak();
			OGRE_DELETE( m_pChantPart );
		}
	}
}
tagAchievementCategoryDef_ToLua* GameActor::getAchievementCategoryDef( int categoryId )
{
	assert( sizeof( tagAchievementCategoryDef_ToLua ) == sizeof( tagAchievementCategoryDef ) );
	const tagAchievementCategoryDef* pAchievementCategoryDef = DBDataManager::getSingleton().getAchievementCategoryDef( categoryId );
	if ( pAchievementCategoryDef->Name != NULL )
	{
		return (tagAchievementCategoryDef_ToLua*)pAchievementCategoryDef;
	}
	return NULL;
}
tagAchievementDef_ToLua* GameActor::getAchievementDef( int achievementId, int categoryId )
{
	assert( sizeof( tagAchievementDef_ToLua ) == sizeof( tagAchievementDef ) );
	const tagAchievementDef* pAchievementDef = DBDataManager::getSingleton().getAchievementDef( achievementId, categoryId );
	if ( pAchievementDef->Name != NULL )
	{
		return (tagAchievementDef_ToLua*)pAchievementDef;
	}
	return NULL;
}

int GameActor::getMapId()
{
	if (m_pCurMap == NULL)
	{
		return 0;
	}
	return m_pCurMap->getMapId();
}

int GameActor::getMapRealId()
{
	if (m_pCurMap == NULL)
	{
		return 0;
	}
	return m_pCurMap->getRealId();
}

void GameActor::setDartCarType( unsigned char type )
{
	m_DartCarType = type;
}

bool GameActor::isPolanCar()
{
	return m_DartCarType == DART_CAR_TYPE_POLAN;
}

void GameActor::requestCancelShamDead()
{
	tagCSPkgBody msgbody;
	memset( &msgbody, 0, sizeof(tagCSPkgBody) );
	msgbody.PlayerMiscClt.PlayerMiscType = PLAYER_SHAM_DEAD_BRK;
	SendGameServerMsg( PLAYER_MISC_CLT, msgbody );
}

void GameActor::setTextureByID( int id,const char* szPath )
{
	if( getShowActor() && getShowActor()->getCurEntity() )
	{
		getShowActor()->getCurEntity()->setTextureByID( id, szPath );
	}
}

bool GameActor::IsCurMapHasStrongPoint( int mapID )
{
	for ( int i = 0; i < MAX_BASE_WAR_MAP_CLAN_INFO ; i++ )
	{
		if ( ActorManager::getSingleton().m_BaseWarMapOwnerList[i].nMapID == mapID )
		{
			return true;
		}
	}
	return false;
}
tdr_ulonglong GameActor::getCurMapStrongOwnerWID( int mapID )
{
	for ( int i = 0; i < MAX_BASE_WAR_MAP_CLAN_INFO ; i++ )
	{
		if ( ActorManager::getSingleton().m_BaseWarMapOwnerList[i].nMapID == mapID )
		{
			return ActorManager::getSingleton().m_BaseWarMapOwnerList[i].nOwnerClanWID;
		}
	}
	return 0;
}
void GameActor::setBannerInfo()
{
	if (  strstr( getName(), "公会旗帜" ) != NULL )
	{
		//确定是公会旗帜npc的话，根据设置来贴图，如果没有设置过，给默认设置为1，
		((GameNPC*)this)->m_strNpcFunctionDes = m_ViewBanner.XuanYan;
		char Buff[256];
		if ( m_ViewBanner.Back / 1000 == 2 )
		{
			int levelIndex = m_ViewBanner.Back/100%10;
			int backIndex = m_ViewBanner.Back%10;
			sprintf( Buff, "character\\monster\\261217\\diwen_%d.dds", levelIndex*3+backIndex );
			setTextureByID( 1, Buff );
		}
		else if ( m_ViewBanner.Back == 0 )
		{
			setTextureByID( 1, "character\\monster\\261217\\diwen_1.dds" );
		}

		if ( m_ViewBanner.Style / 1000 == 1 )
		{
			int levelIndex = m_ViewBanner.Style/100%10;
			int styleIndex = m_ViewBanner.Style%10;
			sprintf( Buff, "character\\monster\\261217\\qizi_%d.dds", levelIndex*3+styleIndex );
			setTextureByID( 2, Buff );
		}
		else if ( m_ViewBanner.Style == 0 )
		{
			setTextureByID( 2, "character\\monster\\261217\\qizi_1.dds" );
		}

		if ( m_ViewBanner.Sign  / 1000 == 3 )
		{
			int levelIndex = m_ViewBanner.Sign/100%10;
			int signIndex = m_ViewBanner.Sign%10;
			sprintf( Buff, "character\\monster\\261217\\wen_%d.dds", levelIndex*3+signIndex );
			setTextureByID( 3, Buff );
		}
		else if ( m_ViewBanner.Sign == 0 )
		{
			setTextureByID( 3, "character\\monster\\261217\\wen_1.dds" );
		}
		if ( m_ViewBanner.GloryEffect == 1 )
		{
			if (!isMotionPlaying( "3195001", 3195001 ))
			{
				playMotion( "3195001", 3195001 );
			}
		}
		else
		{
			stopMotion( "3195001" );
		}
	}
}

void GameActor::setActorWorldName( const char* szName )
{
	strncpy( m_szActorWorldName, szName, WORLD_NAME_LEN );
}

const char* GameActor::getActorWorldName()
{
	return m_szActorWorldName;
}

void GameActor::SetFootEffect( const GameXYZ &pos ,int nStar )
{
	char szBuf[256];
	if ( nStar == 9 )
	{
		sprintf( szBuf, "effect\\entity\\wanmeiXX9.ent");
	}
	else if ( nStar == 8 )
	{
		sprintf( szBuf, "effect\\entity\\wanmeiXX8.ent");
	}
	else if ( nStar == 7 )
	{
		sprintf( szBuf, "effect\\entity\\wanmeiXX7.ent");
	}
	else if ( nStar == 6 )
	{
		sprintf( szBuf, "effect\\entity\\wanmeiXX6.ent");
	}
	else
	{
		return;
	}
	EntityEffect *effect = new EntityEffect(szBuf, GetID());
	effect->start();
	effect->setAngle( getAngle() );
	effect->setpos( pos );
}

int GameActor::getWanMeiStarNum()
{
	if ( ActorManager::getSingleton().getGameOpt( GAME_OPT_SHOW_PERFECT_STAR_EFFECT ) )
	{
		return 0;
	}
	return m_nPerfectStar;
}
void GameActor::setPerfectEffect()
{
	char szBuf[256];
	int  nStar = getWanMeiStarNum() ;
	if ( nStar == 9 )
	{
		sprintf( szBuf, "wanmeiX9");
	}
	else if ( nStar == 8 )
	{
		sprintf( szBuf, "wanmeiX8");
	}
	else if ( nStar == 7 )
	{
		sprintf( szBuf, "wanmeiX7");
	}
	else if ( nStar == 6 )
	{
		sprintf( szBuf, "wanmeiX6");
	}
	else
	{
		m_pShowActor->getCurEntity()->stopMotion( 30006 );
		return;
	}
	playMotion( szBuf, 30006);
	
}

void GameActor::setLostTotemEffect()
{
	int val = m_nLostTotemExpNum*100/m_nLostTotemExpMax;
	if ( val > 90 )
	{
		playMotion( "252010_3", 252010);
	}
	else if ( val > 50 )
	{
		playMotion( "252010_2", 252010);
	}
	else if ( val > 10 )
	{
		playMotion( "252010_1", 252010);
	}
	else
	{
		stopMotion( "252010_1" );
		stopMotion( "252010_2" );
		stopMotion( "252010_3" );
	}
}

void GameActor::setVipFlag( int nFlag )
{
	m_nVipFlag = nFlag;
}

int GameActor::getVipFlag()
{
	return m_nVipFlag;
}

void GameActor::setXuKongEffect()
{
	if ( m_nXuKongFlag == 1 )
	{
		playMotion( "9640", 9640);
	}
	else
	{
		m_pShowActor->getCurEntity()->stopMotion( 9640 );
	}
}

bool GameActor::canActWingFloat()
{
	// 锁定攻击跑动
	if (m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE)
	{
		return false;
	}
	if (isInSkillTetanic())
	{
		return false;
	}
	if( isOnMonsterMachine() )
	{
		return false;
	}
	if (m_Rider.isOnHorse())
	{
		return false;
	}
	if ( isInFight() )
	{
		return false;
	}
	if ( isInMorph() )
	{
		return false;
	}
	//如果在播放动作表情时
	if (getShowActor()->getCurEntity()->hasAnimPlaying(10702) ||
		getShowActor()->getCurEntity()->hasAnimPlaying(10703) ||
		getShowActor()->getCurEntity()->hasAnimPlaying(10704) ||
		getShowActor()->getCurEntity()->hasAnimPlaying(10713))
	{
		return false;
	}
	if(getType() == GAT_PLAYER|| getType() == GAT_MAINPLAYER)
	{
		if(getActState().GetState() == GSACT_STAND||getActState().GetState() == GSACT_MOVE)
		{
			if(m_Equip.getEquipItemID(EQUIP_POS_FASHION_BACK)  > 0)
			{
				tagItemDef_ToLua* itemdef = getItemDef( m_Equip.getEquipItemID(EQUIP_POS_FASHION_BACK));
				if(bitAnd(itemdef->CtrlFlag, ITEM_HAVE_FLOAT_ACT))
				{
					return true;
				}
			}
		}
	}
	return false;
}