#include "GameUIMsg.h"
#include "GameShortCut.h"
#include "GameManager.h"
#include "GameSkill_Mgr.h"
#include "GameSkill_Control.h"
#include "GamePlayManager.h"
#include "GameActor.h"
#include "GameClientOpt.h"
#include "GameMainPlayer.h"
#include "GameActorManager.h"
#include "GameWizard.h"
#include "GameChatManager.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "OgreRay.h"
#include "GameCamera.h"
#include "OgreCamera.h"
#include "ShowGameMap.h"

#include "../UILib/ui_framemgr.h"

extern FrameManager *g_pFrameMgr;

const int SHORTCUT_KEYINDEX = 13;
const int MAX_SHORTCUTINDEX	= 3;
// TODO: 记得写个配置文件给策划，让他以后方便改动
const int role_ShortCut_Init[][MAX_SHORTCUTINDEX] = 
{
//	快捷键类型			技能/物品ID		快捷键键值			
	{ SCT_SKILL,		1018,			0 },	// 战士
	{ SCT_SKILL,		2012,			0 },	// 法师
	{ SCT_SKILL,		3016,			0 },	// 枪手
	{ SCT_SKILL,		4002,			0 },	// 祭祀

	{ SCT_SKILL,		1010,			1 },	// 战士
	{ SCT_SKILL,		2010,			1 },	// 法师
	{ SCT_SKILL,		3010,			1 },	// 枪手
	{ SCT_SKILL,		4000,			1 },	// 祭祀

	{ SCT_ITEM,			5010000,		8 },	// HP药水
	{ SCT_ITEM,			5011000,		9 },	// MP药水

	{ SCT_SKILL,		7005,			7 },	// 共有技能

	{ SCT_SKILL,		7000,			7 },	// 战士
	{ SCT_SKILL,		7001,			7 },	// 法师
	{ SCT_SKILL,		7002,			7 },	// 枪手
	{ SCT_SKILL,		7003,			7 },	// 祭祀
};
static int bShowActor = true;

static bool NearDis(pair<int, GameActor *> &a, pair<int, GameActor *> &b)
{
	return a.first >= b.first;
}

GameShortCut::GameShortCut(void)
{
	ZeroMemory( &m_ShorCut, sizeof( tagShortCut )*MAX_SHORCUT );
	for( int i = 0; i < SELECT_END; i++ )
	{
		m_SPShortCutSkill[i] = 0;
	}
	for( int i = 0; i < MAX_SHORCUT; i++ )
	{
		m_ShorCut[i].m_nNum = i+1;
	}
	m_bLock = false;
	m_nKey	= KEY_UNUSED;
	m_isMousePress = false;
	memset(m_isKeyPress, 0, sizeof(m_isKeyPress));
	SkillManager::getSingleton().setSpellUIControl(this);
	GameMsgPrint::getSingleton().registGameMsgHandler(this);
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_SHORCUT, sizeof( tagShortCutSave ) * MAX_SHORCUT );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_SHORTCUT_FULL_SKILL_INDEX, sizeof( char )  );
	GameClientOpt::getSingleton().setLevelSize( SAVE_OPT_SPSHORCT, sizeof( int ) * SELECT_END );
}

GameShortCut::~GameShortCut(void)
{
}

void GameShortCut::setShortCut( const tagShortCut& shortcut, int num )
{
	assert( num < MAX_SHORCUT );

	m_ShorCut[num]			= shortcut;
	m_ShorCut[num].m_nNum	= num+1;
	tagUIMsgBody UI_msg;
	UI_msg.ShortCut = m_ShorCut[num];
	GameUIMsg::getSingleton().setUIMsg( UI_msg );
	GameManager::getSingleton().SendUIEvent( "GE_SHORTLIST_CHANGE" );
}

const tagShortCut& GameShortCut::getShortCut( int num )
{
	assert( num < MAX_SHORCUT );
	return getUseShortCut()[num];
}

void GameShortCut::setCoolTime( int id, int starttime, int endtime )
{
	tagUIMsgBody UI_msg;
	UI_msg.CoolTime.num			= id;
	UI_msg.CoolTime.starttime	= ( (float)starttime )/1000;
	UI_msg.CoolTime.endtime		= ( (float)endtime )/1000;
	GameUIMsg::getSingleton().setUIMsg( UI_msg );
	GameManager::getSingleton().SendUIEvent( "GE_SKILL_COOLD" );
}

void GameShortCut::setCollect( float totleTime, float during, int times )
{
	tagUIMsgBody UI_msg;
	UI_msg.PowerCollect.fTotleTime	= totleTime;
	UI_msg.PowerCollect.fStartTime	= during;
	UI_msg.PowerCollect.times = times;
	GameUIMsg::getSingleton().setUIMsg( UI_msg );
	GameManager::getSingleton().SendUIEvent( "GE_SKILL_POWERCOLLECT_BEGIN" );
}

void GameShortCut::lockUsed( bool flage )
{
	m_bLock = flage;
}

void GameShortCut::addSpringSpell( int skillId, int nFittingIndex, int Lv, int type )
{
	{
		if (SkillManager::getSingleton().getSkillDyAttrib(skillId) == NULL)
		{
			tagSkillDyAttrib *attrib = new tagSkillDyAttrib;
			memset(attrib, 0, sizeof(tagSkillDyAttrib));
			attrib->SkillLevel = Lv;
			SkillManager::getSingleton().setSkillDyAttrib(skillId, attrib);
		}
	}
	for( std::list<tagShortCut>::iterator iter = m_replaySpellList.begin(); iter != m_replaySpellList.end(); ++iter )
	{
		if( iter->m_Id == skillId )
		{
			iter->m_nFittingIndex		= nFittingIndex;
			iter->m_nType				= type;
			iter->m_Lv					= Lv;
			return;
		}
	}
	tagShortCut spell;
	spell.m_Id					= skillId;
	spell.m_nFittingIndex		= nFittingIndex;
	spell.m_nType				= type;
	spell.m_Lv					= Lv;
	m_replaySpellList.push_back( spell );
}

void GameShortCut::delSpringSpell( int skillId )
{
	for( std::list<tagShortCut>::iterator iter = m_replaySpellList.begin(); iter != m_replaySpellList.end(); ++iter )
	{
		if( iter->m_Id == skillId )
		{
			m_replaySpellList.erase( iter );
			if (skillId == 3223) // tag 暂时措施
			{
				ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_NONE, 0, skillId);
			}
			SkillManager::getSingleton().delSkillDyAttrib(skillId);
			break;
		}
	}
}

void GameShortCut::clearSpringSpell()
{
	for( std::list<tagShortCut>::iterator iter = m_replaySpellList.begin(); iter != m_replaySpellList.end(); )
	{
		if (iter->m_Id == 3223) // tag 暂时措施
		{
			if (ActorManager::getSingleton().getMainPlayer())
			{
				ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_NONE, 0, iter->m_Id);
			}
		}
		SkillManager::getSingleton().delSkillDyAttrib(iter->m_Id);
		iter = m_replaySpellList.erase( iter );
	}
}

void GameShortCut::newSkill(int id, int lv)
{
	const tagSkillDef &skillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	if (skillDef.SkillType == SKILL_TYPE_NORMAL)
	{
		addNormalAttackSkill(id, lv);
	}
}

void GameShortCut::forgetSkill(int id, int lv)
{
	const tagSkillDef &skillDef = DBDataManager::getSingleton().GetSkillDef(id, lv);
	if (skillDef.SkillType == SKILL_TYPE_NORMAL)
	{
		delNormalAttackSkill(id);
	}
}

void GameShortCut::addNormalAttackSkill(int id, int lv)
{
	int normal = SkillManager::getSingleton().getNormalAttackSkill();
	if (normal == id)
	{
		return;
	}
	SkillManager::getSingleton().addInsteadBind(normal, id);
	if (SkillManager::getSingleton().getSkillDyAttrib(id) == NULL)
	{
		tagSkillDyAttrib *attrib = new tagSkillDyAttrib;
		memset(attrib, 0, sizeof(tagSkillDyAttrib));
		attrib->SkillLevel = lv;
		SkillManager::getSingleton().setSkillDyAttrib(id, attrib);
	}
}

void GameShortCut::delNormalAttackSkill(int id)
{
	int normal = SkillManager::getSingleton().getNormalAttackSkill();
	SkillManager::getSingleton().delInsteadBind(normal, id);
	SkillManager::getSingleton().delSkillDyAttrib(id);
}

// 获取普通攻击技能
int GameShortCut::getNormalAttackSkill()
{
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();

	// 小技能&临时武器技
	int tmpSkillId	= SkillManager::getSingleton().getTmpSkillId( 0 );
	int tmpArm		= pMainPlayer->m_Equip.getTmpArm();
	int tmpMachine	= pMainPlayer->m_Equip.getTmpMachin();

	// 处于变身状态并且不是机械变身不能使用左键技能
	if( pMainPlayer->getInState( GSACT_IN_MORPH ) && tmpMachine == 0 && tmpArm == 0 && pMainPlayer->m_bChangeSkillShortCut )
	{
		return 0;
	}

	if( tmpArm != 0 )
	{
		// 临时武器如能使用，左键技能为临时武器技
		tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( tmpArm );
		if( itemDef.ItemTmp.Type == ITEM_TMP_ARM || itemDef.ItemTmp.Type == ITEM_TMP_ITEM || itemDef.ItemTmp.Type == ITEM_TMP_CLAN )
		{
			const tagSkillDyAttrib* pSkillDyAttrib = SkillManager::getSingleton().getSkillDyAttrib( tmpSkillId );
			if( pSkillDyAttrib != NULL )
			{
				return tmpSkillId;
			}
		}
	}

	if( tmpMachine != 0 )
	{
		// 临时武器如能使用，左键技能为临时武器技
		tagItemDef itemDef  = DBDataManager::getSingleton().getItemDef( tmpMachine );
		if( itemDef.ItemTmp.Type == ITEM_TMP_MACHINE )
		{
			const tagSkillDyAttrib* pSkillDyAttrib = SkillManager::getSingleton().getSkillDyAttrib( tmpSkillId );
			if( pSkillDyAttrib != NULL )
			{
				return tmpSkillId;
			}
		}
	}

	return SkillManager::getSingleton().getNormalAttackSkill();
}

bool GameShortCut::canDrag( int index )
{
	if( m_bLock ) return false;

	if( m_ShorCut[index-1].m_nType != SCT_NONE )
	{
		return true;
	}
	return false;
}

bool GameShortCut::canReceiveDrag(int index)
{
	return !m_bLock;
}

void GameShortCut::usedShortCut( int index, int nTargetIndex )
{
	assert( index < MAX_SHORCUT );
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	
// 	if( pMainPlayer->isOnMonsterMachine() )
// 	{
// 		pMainPlayer->getMonsterMachine()->useMonsterSkill(getUseShortCut()[index].m_Id,getUseShortCut()[index].m_Lv);
// 		m_bLock = true;
// 		m_nKey = index + 1;
// 		return;
// 	}
	
	// 技能使用    快捷键123
	if( getUseShortCut()[index].m_nType == SCT_SKILL || getUseShortCut()[index].m_nType == SCT_FAIRYSKILL )
	{
		if( pMainPlayer->isInStall() && m_nShortCutType == SHORTCUT_NORMAL )
		{
			ChatManager::getSingleton().showMidTip( MID_SHORTCUT_CANNOTUSESPELL_INSTALL );
			return;
		}

// 		if( pMainPlayer->getActState().getInState( GSACT_IN_MORPH ) && m_nShortCutType == SHORTCUT_NORMAL )
// 		{
// 			ChatManager::getSingleton().showMidTip( MID_SHORTCUT_CANNOTUSESPELL_INMORPH );
// 			return;
// 		}
		if( pMainPlayer->isInFly() && m_nShortCutType != SHORTCUT_TMP )
		{
			GameMsgPrint::getSingleton().showMidTip(MID_SHORTCUT_IN_FLY_STATE);
		}
		const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef( getUseShortCut()[index].m_Id, getUseShortCut()[index].m_Lv );
		if( SkillDef.WushuangFlag > 0 || SkillDef.UseXp > 0 )
		{
			int nReturn = pMainPlayer->checkXP( getUseShortCut()[index].m_Id );
			if( nReturn != 0 )
			{
				ChatManager::getSingleton().showMidTip( ( EMID_MESSAGE )nReturn );
				return;
			}
		}
		if (m_nKey != KEY_UNUSED)
		{
			shorCutUpKey();
		}
		useSkill(getUseShortCut()[index].m_Id);
		m_bLock = true;
		m_nKey = index + 1;
	}
	// 物品使用
	if( getUseShortCut()[index].m_nType == SCT_ITEM )
	{
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		if( pMainPlayer != NULL )
		{
			if( pMainPlayer->getContainer().getItemCount( getUseShortCut()[index].m_Id ) < 1 )
			{
				ChatManager::getSingleton().showMidTip( MID_SHORTCUT_NOTITEM );
				return;
			}
			Container& container =	pMainPlayer->getContainer(); 
			Item& used_Item = container.getItem( getUseShortCut()[index].m_Id );
			pMainPlayer->getContainer().usedItem( used_Item.getListType(), used_Item.getGridIdx(), 0, nTargetIndex );
		}
	}
	// 骑乘
	if(  getUseShortCut()[index].m_nType == SCT_RIDE )
	{
		MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
		Rider* pRider = pMainPlayer->getRider();
		const tagRideInfo& rideItem = pRider->getRideItem();
		const tagShortCut& shortCutItem = getUseShortCut()[index];
		
		if ( !pRider->isHaveRide( shortCutItem.m_Id ) )
		{
			GameMsgPrint::getSingleton().showMidTip( MID_RIDE_PRE_FAIL_NO_RIDE );
			return;
		}

		if ( pRider->isOnHorse() && rideItem.RideID == shortCutItem.m_Id )
		{
			pRider->requestRideDown();
		}
		else
		{
			if (!pRider->isPre())
			{
				if ( rideItem.RideID != 0 )
				{
					pRider->requestRideDown();
				}
				pRider->MainPlayerRidePre( shortCutItem.m_Id, shortCutItem.m_Lv );
			}
		}
		
	}
}

void GameShortCut::shorCutUpKey()
{
	SkillManager::getSingleton().unUseSpell();
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL && pMainPlayer->isOnMonsterMachine() )
	{
		pMainPlayer->getMonsterMachine()->endCollectSkill();		
	}
	m_bLock = false;
	m_nKey	= KEY_UNUSED;
}

void GameShortCut::delShortCut( int index )
{
	assert( index < MAX_SHORCUT );
	if( m_bLock ) return;

	getUseShortCut()[index].m_nType = 0;
	getUseShortCut()[index].m_Id = 0;
	setShortCut( getUseShortCut()[index], index );
	sendsetshortcutMsg();
}

void GameShortCut::swapShortCut( int src, int des )
{
	assert( src < MAX_SHORCUT );
	assert( des < MAX_SHORCUT );
	if( m_bLock ) return;

	std::swap( getUseShortCut()[src], getUseShortCut()[des] );
	updateUI( src );
	updateUI( des );
	sendsetshortcutMsg();
}

void GameShortCut::setShortCut2UI( int nType, int Id, int index, int nFitting, int nLevel )
{
	if( nType == SCT_SKILL || nType == SCT_FAIRYSKILL )
	{
		const tagSkillDyAttrib* pDyAttrib = SkillManager::getSingleton().getSkillDyAttrib( Id );
		if( pDyAttrib == NULL ) return;

		tagShortCut shortcut;
		shortcut.m_Id			 = Id;
		shortcut.m_Lv			 = pDyAttrib->SkillLevel;
		shortcut.m_nType		 = nType;
		shortcut.m_nFittingIndex = nFitting;
		setShortCut( shortcut, index );
	}
	if( nType == SCT_ITEM || nType == SCT_RIDE )
	{
		tagShortCut shortcut;
		shortcut.m_Id			 = Id;
		shortcut.m_Lv			 = nLevel;
		shortcut.m_nType		 = nType;
		shortcut.m_nFittingIndex = nFitting;
		setShortCut( shortcut, index );
		// TODO:检查所有物品CD广播的地方 重新整理
		if( nType == SCT_ITEM )
		{
			GameManager::getSingleton().SendUIEvent( "GE_ITEMUSED_PASSCD" );
		}
	}
}

void GameShortCut::setShortCut( int nType, int Id, int index, int nFitting, int nLevel )
{
	if( m_bLock )
	{
		return;
	}
	// 组织给UI的数据
	setShortCut2UI( nType, Id, index, nFitting, nLevel );
	// 保存快捷键信息到服务器
	sendsetshortcutMsg();
}

// 初始化快捷键
void GameShortCut::initNewPlayerShortCut()
{
	if( GameWizard::getSingleton().isDo( GAMEWIZARD_SETSHORTCUT ) )
	{
		return;
	}
	int nRace = ActorManager::getSingleton().getMainPlayer()->getRace() & 0x0f;
	setShortCut2UI( role_ShortCut_Init[nRace-1][0], role_ShortCut_Init[nRace-1][1], role_ShortCut_Init[nRace-1][2], 0 );
	setShortCut2UI( role_ShortCut_Init[nRace+3][0], role_ShortCut_Init[nRace+3][1], role_ShortCut_Init[nRace+3][2], 0 );
	setShortCut2UI( role_ShortCut_Init[nRace+10][0], role_ShortCut_Init[nRace+10][1], role_ShortCut_Init[nRace+10][2], 0 );
	setShortCut2UI( role_ShortCut_Init[8][0], role_ShortCut_Init[8][1], role_ShortCut_Init[8][2], 0 );
	setShortCut2UI( role_ShortCut_Init[9][0], role_ShortCut_Init[9][1], role_ShortCut_Init[9][2], 0 );
	setShortCut2UI( role_ShortCut_Init[10][0], role_ShortCut_Init[10][1], role_ShortCut_Init[10][2], 0 );
	GameShortCut::getSingleton().setNewSkillShortcutIndex( 4 );
	// 保存快捷键信息到服务器
	sendsetshortcutMsg();
	GameManager::getSingleton().SendUIEvent( "GE_ITEMUSED_PASSCD" );
}

void GameShortCut::setPetFittingShortCut( int Id, int nSkillLv, int index, int nFitting )
{
	tagShortCut shortcut;
	shortcut.m_Id			 = Id;
	shortcut.m_Lv			 = nSkillLv;
	shortcut.m_nType		 = SCT_FAIRYSKILL;
	shortcut.m_nFittingIndex = nFitting;
	setShortCut( shortcut, index );

	// 保存快捷键信息到服务器
	sendsetshortcutMsg();
	GameManager::getSingleton().SendUIEvent( "GE_ITEMUSED_PASSCD" );
}

// 设置临时机械技能快捷键
void GameShortCut::setTmpShortCut( int Id, int nSkillLv, int index, int nFitting, int nType  )
{
	m_TmpShorCut[index].m_Id			= Id;
	m_TmpShorCut[index].m_Lv			= nSkillLv;
	m_TmpShorCut[index].m_nType			= nType;
	m_TmpShorCut[index].m_nNum			= index+1;
	m_TmpShorCut[index].m_nFittingIndex = 0;

	GameManager::getSingleton().SendUIEvent( "GE_ITEMUSED_PASSCD" );
}

// 获取临时技能快捷键
int GameShortCut::getTmpShortCutId( int nIndex )
{
	return m_TmpShorCut[nIndex].m_Id;
}

// 获取临时技能快捷键
int GameShortCut::getTmpShortCutLv( int nIndex )
{
	return m_TmpShorCut[nIndex].m_Lv;
}

void GameShortCut::usedTab( int nType )
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL )
	{
		GameActor* pActor;
		switch( nType )
		{
		case 0:
			{
				pActor = ActorManager::getSingleton().getNearestActor( pMainPlayer->GetID(), 15000 );
			}
			break;
		case 1:
			{
				pActor = ActorManager::getSingleton().getNearestActor( pMainPlayer->GetID(), 15000, false, 0, GAT_MONSTER );
			}
			break;
		case 2:
			{
				pActor = ActorManager::getSingleton().getNearestActor( pMainPlayer->GetID(), 15000, true, 0, GAT_PLAYER );
			}
			break;
		default:
			pActor = NULL;
		}
		if( pActor != NULL )
		{
			pMainPlayer->SetSelectTarget( pActor->GetID() );
		}
		else
		{
			pMainPlayer->SetSelectTarget(NULL);
		}
	}
}

// 是用F12屏蔽所有玩家
void GameShortCut::usedF12()
{
	ActorManager& actorMgr			= ActorManager::getSingleton();
	actorMgr.changeShowOtherPlayerModel();
	MainGamePlayer* pMainPlayer		= actorMgr.getMainPlayer();
	tdr_ulonglong mainplayerTeamWID = pMainPlayer->getTeamWID();
	for( ACTOR_ITER iter = actorMgr.getActorBegin(), end = actorMgr.getActorEnd(); 
			iter != end; ++iter )
	{
		GameActor* pOneActor = iter->second;

		if ( ActorManager::getSingleton().isActorIgnoreF12( pOneActor ) )
		{
			continue;
		}

		GameMap* pMapMgr = pOneActor->getCurMap();
		if ( pMapMgr == NULL )
		{
			continue;
		}

		Ogre::ShowActor* pSelfShowActor = pOneActor->getShowActor();
		int nHideOtherPlayerOption	= actorMgr.getShowOtherPlayerModelOption();

		if ( nHideOtherPlayerOption != HIDE_MODEL_NONE )
		{
			if ( nHideOtherPlayerOption == HIDE_OTHER_PLAYER_AND_PET && pOneActor->getType() == GAT_PLAYER && pOneActor->isInStall() )
			{
				continue;
			}
			
			pSelfShowActor->onLeaveMap( pMapMgr->getShowGameMap() );
			if ( pOneActor->m_Rider.m_pShowActor != NULL )
			{
				pOneActor->m_Rider.m_pShowActor->onLeaveMap( pMapMgr->getShowGameMap() );
			}
			pOneActor->m_pSmallDecal->show(false);
		}
		else if (pOneActor->isOptimizeDisplay() && !pOneActor->isOptimizeClear())
		{
			pSelfShowActor->update(60000);
			pSelfShowActor->onEnterMap( pMapMgr->getShowGameMap() );
			if( pOneActor->m_Rider.m_pShowActor != NULL )
			{
				pOneActor->m_Rider.m_pShowActor->onEnterMap( pMapMgr->getShowGameMap() );
			}
			pOneActor->m_pSmallDecal->show(true);
		}

		if ( pOneActor->isInStall() )
		{
			RoleText& actorTitleMgr = pOneActor->getRoleText();
			actorTitleMgr.BuildTitle();
		}
	}
}

// 用F11屏蔽所有玩家名字
void GameShortCut::usedF11()
{
	ActorManager::getSingleton().setShowItherPlayerTitle();
}

// 设置组合键位的技能
void GameShortCut::setSPShortCut( int nType, int nValue )
{
	assert( nType < SELECT_END );
	m_SPShortCutSkill[nType] = nValue;
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_SPSHORCT, (char*)&m_SPShortCutSkill );
}

// 获取组合键位技能
int GameShortCut::getSPShortCut( int nType )
{
	return m_SPShortCutSkill[nType];
}

// 获取要使用的快捷键
tagShortCut* GameShortCut::getUseShortCut()
{
	MainGamePlayer* pMainPlayer = GamePlayManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL )
	{
		return NULL;
	}
	if( m_nKey != KEY_UNUSED && pMainPlayer->m_bChangeSkillShortCut && m_nShortCutType == SHORTCUT_NORMAL )
	{
		SkillManager::getSingleton().clearIntentSpell();
		if( m_ShorCut[m_nKey-1].m_Id != 0 && m_ShorCut[m_nKey-1].m_Lv != 0 )
		{
			SkillManager::getSingleton().unUseSpell();
		}
		m_nKey = KEY_UNUSED;
		m_bLock = false;
	}
	if(	pMainPlayer->m_bChangeSkillShortCut )
	{
		m_nShortCutType = SHORTCUT_TMP;
		return m_TmpShorCut;
	}else{
		m_nShortCutType = SHORTCUT_NORMAL;
		return m_ShorCut;
	}
}

// 获取要使用的快捷键上的道具
tagShortCut& GameShortCut::getUseShortCut( int nIndex )
{
	assert( nIndex < MAX_SHORCUT );
	return getUseShortCut()[nIndex];
}

// 重置快捷键
void  GameShortCut::resetShortCut()
{
	memset( &m_ShorCut, 0, sizeof( tagShortCut )*MAX_SHORCUT );
	memset( &m_TmpShorCut, 0, sizeof( tagShortCut )*MAX_SHORCUT );
}

void GameShortCut::update( unsigned int tick )
{
	if (m_nKey != KEY_UNUSED && m_isKeyPress[m_nKey] != 0)
	{
		if ((GetKeyState(m_isKeyPress[m_nKey]) & 0xff00) == 0)
		{
			m_isKeyPress[m_nKey] = 0;
		}
	}
	if( m_nKey != KEY_UNUSED && m_isKeyPress[m_nKey] == 0 && !m_isMousePress)
	{ 
		shorCutUpKey();
	}

	if (SkillManager::getSingleton().getIntentSpell() == NULL && SkillManager::getSingleton().getSpellControler()->getCurSpell() == NULL)
	{
		for( std::list<tagShortCut>::iterator iter = m_replaySpellList.begin(); iter != m_replaySpellList.end(); ++iter )
		{
			int result = SkillManager::getSingleton().castInspect(iter->m_Id);
			if (result == GameSpell::RET_SUCCESS)
			{
				SkillManager::getSingleton().useSpell(iter->m_Id);
				SkillManager::getSingleton().unUseSpell();
			}
			else if (iter->m_Id == 3223)
			{
				if (result != GameSpell::RET_CD && result != GameSpell::RET_DOOTHER && result != GameSpell::RET_SPELLPRE && result != GameSpell::RET_TETANIC)
				{
					ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_NONE, 0, iter->m_Id);
				}
			}
		}
	}
}

void GameShortCut::updateUI( int num )
{
	assert( num < MAX_SHORCUT );
	getUseShortCut()[num].m_nNum = num+1;
	tagUIMsgBody UI_msg;
	UI_msg.ShortCut = getUseShortCut()[num];
	GameUIMsg::getSingleton().setUIMsg( UI_msg );
	GameManager::getSingleton().SendUIEvent( "GE_SHORTLIST_CHANGE" );
}

void GameShortCut::sendsetshortcutMsg()
{
	tagShortCutSave SaveShorCut[MAX_SHORCUT];
	ZeroMemory( SaveShorCut, sizeof( tagShortCutSave ) * MAX_SHORCUT );
	for( int i = 0; i < MAX_SHORCUT; i++ )
	{
		SaveShorCut[i].Id		= m_ShorCut[i].m_Id;
		SaveShorCut[i].Fitting	= m_ShorCut[i].m_nFittingIndex;
		SaveShorCut[i].nType	= m_ShorCut[i].m_nType;
		SaveShorCut[i].nLv		= m_ShorCut[i].m_Lv;
	}
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_SHORCUT, (char*)&SaveShorCut );
	GameClientOpt::getSingleton().setClientOpt( SAVE_OPT_SHORTCUT_FULL_SKILL_INDEX, (char*)&m_cShortcutIndex );

}

void GameShortCut::setChannel( float totleTime )
{
	tagUIMsgBody UI_msg;
	UI_msg.PowerCollect.fTotleTime	= totleTime;
	GameUIMsg::getSingleton().setUIMsg( UI_msg );
	GameManager::getSingleton().SendUIEvent( "GE_SKILL_CHANNEL_BEGIN" );
}

void GameShortCut::setShortCutPress(bool bPress)
{
	m_isMousePress = bPress;
}

void GameShortCut::setShortCutKeyPress(int index, int bPress)
{
	m_isKeyPress[index] = bPress;
}

bool GameShortCut::useSkill(int skillId)
{
	GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (!pMainPlayer)
	{
		return false;
	}
	if (!SkillManager::getSingleton().castPlayerLv(skillId))
	{
		const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef(skillId, SkillManager::getSingleton().getSpellLv(skillId));
		char buff[100];
		sprintf(buff, "AddGameMiddleTips([===[角色必须达到%d级才能使用此技能]===])", SkillDef.Learn.LevelMin);
		GameManager::getSingleton().getScriptVM()->callString(buff);
		return false;
	}

	pMainPlayer->requestBreakChant( eTrump_Chant );

	SkillManager::getSingleton().unUseSpell();
	if ( SkillManager::getSingleton().isNormalSkill(skillId))
	{
		GameActor *target = ActorManager::getSingleton().FindActor(pMainPlayer->GetSelectTarget());
		if (target == NULL || target->isDead() || (!target->isCamp( SKILL_TARGET_ENEMY_MON ) && !target->isCamp( SKILL_TARGET_ENEMY_PLAYER )))
		{
			usedTab();
		}
		/*if (target != NULL || target->isDead())
		{
			usedTab();
 			target = ActorManager::getSingleton().getNearestActor(pMainPlayer->GetID(), SkillManager::getSingleton().getSkillDistance(skillId) * 10);
 			if (target != NULL)
 			{
				pMainPlayer->SetSelectTarget(target->GetID());
 			}
		}*/		
	}
	return SkillManager::getSingleton().testResult(SkillManager::getSingleton().useSpell(skillId));
}

void GameShortCut::useSpecialSkill()
{
	GameMap*		pCurMap		= GamePlayManager::getSingleton().getCurMap();	
	if (pCurMap == NULL)
	{
		return;
	}
	MainGamePlayer*	pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer == NULL)
	{
		return;
	}
	FollowCamera*	pMainCamera = GamePlayManager::getSingleton().getMainCamera();
	Ogre::WorldRay	ray;
	POINT p;
	GetCursorPos(&p);
	ScreenToClient((HWND)g_pFrameMgr->getWndHandle(), &p);
	pMainCamera->GetCameraNode()->getViewRayByScreenPt(&ray, p.x, p.y);
	float dist;
	if(pCurMap->getShowGameMap()->pickGround( ray, &dist ) )
	{
		Ogre::WorldPos pickpoint = ray.m_Origin + ray.m_Dir*dist;

		GameActor* pActor = ActorManager::getSingleton().mounseOnActor(ray);
		static const int nSkillList[] = {
			0,
			7000,
			7001,
			7002,
			7003
		};
		int nRace		= pMainPlayer->getRace();
		int nSkillId	= nSkillList[nRace & 0x0f];
		if (SkillManager::getSingleton().isSkillLearned(nSkillId))
		{
			int result = GameSpell::RET_SUCCESS;
			if (pActor)
			{
				result = SkillManager::getSingleton().useSpellOnce(nSkillId, pActor->GetID(), pickpoint.x, pickpoint.z);
			}
			else
			{
				result = SkillManager::getSingleton().useSpellOnce(nSkillId, pickpoint.x, pickpoint.z);
			}
			SkillManager::getSingleton().testResult(result);
		}
	}
}
void  GameShortCut::setNewSkillShortcutIndex(char index)
{
	 m_cShortcutIndex = index;
}
char  GameShortCut::getNewSkillShortcutIndex()
{
	return m_cShortcutIndex;
}