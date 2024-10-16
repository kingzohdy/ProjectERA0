#include "GameBufStatus.h"
#include "GameActor.h"
#include "ServerTimer.h"
#include "OgreDataStream.h"
#include "OgreResourceManager.h"
#include "ShowActor.h"
#include "OgreEntity.h"
#include "GameMsgPrint.h"
#include "GameSkill_Mgr.h"
#include "GameStateAction.h"
#include "GameDBData.h"
#include "GameSkill_Common.h"
#include "OgreVector3.h"
#include "GameMainPlayer.h"
#include "OgreScriptLuaVM.h"
#include "GameActor_Tick.h"
#include "ogrebloom.h"
#include "GameEffect_Mgr.h"
#include "TMPL.h"
#include <Windows.h>
#include <MMSystem.h>

const tagStatusDef_ToLua* getStatusDef(int id, int lv)
{
	static_assert( sizeof( tagStatusDef_ToLua ) == sizeof( tagStatusDef ) );
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( id, lv );
	const tagStatusDef_ToLua* pStatusDef = (const tagStatusDef_ToLua*)( &statusDef );
	return pStatusDef;
}

const tagStatusOverlayDef_ToLua* getStatusOverlayDef(int id, int lv, int overlay )
{
	static_assert( sizeof( tagStatusOverlayDef_ToLua ) == sizeof( tagStatusOverlayDef ) );
	const tagStatusOverlayDef& statusOverlayDef = DBDataManager::getSingleton().getStatusOverlayDef( id, lv, overlay );
	const tagStatusOverlayDef_ToLua* pStatusDef = (const tagStatusOverlayDef_ToLua*)( &statusOverlayDef );
	return pStatusDef;
}

bool isBuffCanCancel( int id, int lv )
{
	//bool bCanCancel = DBDataManager::getSingleton().getStatusDef( id, lv ).CanCancel != 0;
	const tagStatusDef& def = DBDataManager::getSingleton().getStatusDef( id, lv );
	return def.CanCancel != 0 && def.Type == STATUS_TYPE_GOOD ;
}

bool canShowBuff( int id, int lv )
{
	bool bResult = ( DBDataManager::getSingleton().getStatusDef( id, lv ).StatusFlag & STATUS_UNSHOWN_TIME ) == 0;
	return bResult;
}

bool isStatusShowAsDebuff( int id, int lv )
{
	bool bResult = ( DBDataManager::getSingleton().getStatusDef( id, lv ).StatusFlag & STATUS_SHOW_AS_DEBUFF ) == STATUS_SHOW_AS_DEBUFF;
	return bResult;
}

bool isCltShowBuff( int id, int lv )
{
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( id, lv );
	if ( &statusDef == NULL )
	{
		return false;
	}

	return statusDef.IsCltShow == 0; 
}

GameBuf::GameBuf(void) : m_pBufStatus( NULL ), m_nStartTime( 0 ), m_bDead( true ), m_bShow( false ), 
m_ownerId( 0 ), m_nTotalLife( 0 )
{
	memset( &m_StateInfo, 0, sizeof( m_StateInfo ) );
}

GameBuf::~GameBuf(void)
{
}

void GameBuf::begin( const CSROLESTATUS& StateInfo, unsigned int startTime, int ownerId, int index )
{
	m_index = index;
	m_StateInfo	= StateInfo;
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
	if( statusDef.StatusID == 0 )
	{
		return;
	}
	m_bShow = true;

	m_nTotalLife		= StateInfo.Time;
	m_nStartTime		= timeGetTime();
	m_bDead				= false;
	m_ownerId			= ownerId;

	if (m_StateInfo.Time > (m_nStartTime - startTime) / 1000)
	{
		m_StateInfo.Time	-= (m_nStartTime - startTime) / 1000;
	}
	else
	{
		m_StateInfo.Time = 0;
	}
	m_nStartTime		-= (m_nStartTime - startTime) % 1000;

	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	// 角色变身
	if( pActor && !pActor->isOnMonsterMachine() && statusDef.ModeID != 0 )
	{
		pActor->onMorph( statusDef.ModeID );
	}

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	// 有绑定技能自动释放
	if( statusDef.BindSkill.SkillID != 0 && pActor == pMainPlayer )
	{
		//	STATUS_TRIG_NONE = 0, 	/* 无触发 */
		//	STATUS_TRIG_START = 1, 	/* 状态开始触发 */
		//	STATUS_TRIG_TIMER = 2, 	/* 状态过程中定时触发，间隔为技能冷却条件 */
		//	STATUS_TRIG_END = 4, 	/* 状态结束时触发 */
		//	STATUS_TRIG_HAND = 8, 	/* 手动触发 */
		SkillManager::getSingleton().addTmpSkill(statusDef.BindSkill.SkillID);
		switch( statusDef.BindSkill.Trig ) 
		{
		case STATUS_TRIG_TIMER:
			{
				ISpellUIControl* pSpellUIControl = SkillManager::getSingleton().getSpellUIControl();
				if( pSpellUIControl != NULL )
				{
					pSpellUIControl->addSpringSpell( statusDef.BindSkill.SkillID, 0, statusDef.BindSkill.Level, statusDef.BindSkill.Trig );
				}
			}
			break;
		case STATUS_TRIG_HAND:
			{
				ISpellUIControl* pSpellUIControl = SkillManager::getSingleton().getSpellUIControl();
				if( pSpellUIControl != NULL )
				{
					pSpellUIControl->setTmpShortCut( statusDef.BindSkill.SkillID,statusDef.BindSkill.Level, 0, 0 );
				}
			}
			break;
		}
	}

	// 触发xp
	if( statusDef.IsWushuang == 1 && pActor == pMainPlayer )
	{
		// 参数:xp(秒)时间
		char szBuf[256];
		sprintf( szBuf, "BeginXP( %d,%d )", m_StateInfo.Time,statusDef.ModeID );
		ActorManager::getSingleton().getScriptVM()->callString( szBuf );

		pMainPlayer->m_bChangeSkillShortCut = true;
	}
	if (pActor == pMainPlayer)
	{
		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			if (statusDef.Result[i].ResultID == STATUS_RESULT_ADD_BIND_SKILL)
			{
				GameMsgPrint::getSingleton().newSkill(statusDef.Result[i].ResultVal1, statusDef.Result[i].ResultVal2);
			}
		}
	}
	if ( pActor != NULL && pActor->canPlayEffect() )
	{
		// 加载初始特效
		buf_eff( statusDef.EffectID );
		// 中毒等播放特效状态
		buf_begin_eff( statusDef.EffectID );

		// 体型逐渐变化
		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			if (statusDef.Result[i].ResultID == STATUS_RESULT_CHGSIZE)
			{
				int nScale = statusDef.Result[i].ResultVal1;
				if ( m_StateInfo.Overlay > 0 && m_StateInfo.Overlay <= statusDef.OverlayLimit )
				{
					const tagStatusOverlayDef& statusOverlayDef = DBDataManager::getSingleton().getStatusOverlayDef( m_StateInfo.StatusID, m_StateInfo.Level, m_StateInfo.Overlay );
					nScale = (int)statusOverlayDef.Mul*100/statusDef.Result[i].ResultVal1;
				}
				pActor->registerTickEvent( new GameActor_Tick_ChgSize( pActor, timeGetTime(), 
					nScale, statusDef.Result[i].ResultVal2 ) );
			}
		}
	}
	//开始背
	if( pActor != NULL &&  statusDef.AddModelID > 0 )
	{
		pActor->m_pCarry->carry(10136,10135,statusDef.BindPoint,statusDef.AddModelID,0);
	}

	// 中了禁魔buff主动打断
	GameSpell *pSpell = SkillManager::getSingleton().getSpellControler()->getCurSpell();
	if (pSpell && pSpell->getSpellId() != statusDef.BindSkill.SkillID && pActor == ActorManager::getSingleton().getMainPlayer() && pSpell->getSkillDef().IgnoreNoSkill == 0)
	{
		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			if (statusDef.Result[i].ResultID == STATUS_RESULT_NOSKILL)
			{
				pSpell->Break();
				break;
			}
			int skillId = pSpell->getSpellId();
			if( statusDef.Result[i].ResultID != 0 && statusDef.Result[i].ResultID == STATUS_RESULT_NOSKILL_INCOMPLETE )
			{
				if (skillId != statusDef.Result[i].ResultVal1 &&
					skillId != statusDef.Result[i].ResultVal2 &&
					skillId != statusDef.Result[i].ResultVal3 &&	
					skillId != statusDef.Result[i].ResultVal4 ) 
				{
					pSpell->Break();
					break;
				}
			}
		}
	}
}

void GameBuf::end()
{
	m_bDead				= true;
	m_bShow				= false;
	m_nStartTime		= 0;
	m_StateInfo.Time	= 0;

	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
	if( statusDef.StatusID == 0 )
	{
		return;
	}
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	// 模型还原
	if( statusDef.ModeID != 0 )
	{	
		pActor->onRestore(statusDef.ModeID);
	}

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	// 结束xp
	if( statusDef.IsWushuang == 1 && pActor == pMainPlayer )
	{
		ActorManager::getSingleton().getScriptVM()->callString( "EndXP()" );
		pMainPlayer->m_bChangeSkillShortCut = false;
	}
	// 取消循环施放法术
	if( statusDef.BindSkill.SkillID != 0 && pActor == pMainPlayer )
	{
		SkillManager::getSingleton().delTmpSkill(statusDef.BindSkill.SkillID);
		switch( statusDef.BindSkill.Trig ) 
		{
		case STATUS_TRIG_TIMER:
			{
				ISpellUIControl* pSpellUIControl = SkillManager::getSingleton().getSpellUIControl();
				if( pSpellUIControl != NULL )
				{
					pSpellUIControl->delSpringSpell( statusDef.BindSkill.SkillID );
				}
			}
			break;
		case STATUS_TRIG_HAND:
			{
				ISpellUIControl* pSpellUIControl = SkillManager::getSingleton().getSpellUIControl();
				if( pSpellUIControl != NULL )
				{
					pSpellUIControl->delSpringSpell( statusDef.BindSkill.SkillID );
				}
			}
			break;
		}
	}

	for( int i = 0; i < MAX_STATUS_RESULT; i++ )
	{
		if (statusDef.Result[i].ResultID == STATUS_RESULT_ADD_BIND_SKILL)
		{
			GameMsgPrint::getSingleton().forgetSkill(statusDef.Result[i].ResultVal1, statusDef.Result[i].ResultVal2);
		}
	}

	// 加载结束特效
	buf_eff( statusDef.EffectID, 1 );
	// 中毒等播放特效状态
	buf_end_eff();
	// 体型逐渐变化
	if (pActor)
	{
		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			if (statusDef.Result[i].ResultID == STATUS_RESULT_CHGSIZE)
			{
				int nScale = statusDef.Result[i].ResultVal1;
				if ( m_StateInfo.Overlay > 0 && m_StateInfo.Overlay <= statusDef.OverlayLimit )
				{
					const tagStatusOverlayDef& statusOverlayDef = DBDataManager::getSingleton().getStatusOverlayDef( m_StateInfo.StatusID, m_StateInfo.Level, m_StateInfo.Overlay );
					nScale = (int)statusOverlayDef.Mul;
				}
				pActor->registerTickEvent( new GameActor_Tick_ChgSize( pActor, timeGetTime(),
					(int)(10000.0 /nScale ), statusDef.Result[i].ResultVal2 ) );
			}
		}
	}
}

int GameBuf::update( unsigned int dtick )
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	const tagStatusDef& statusDef	= DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
	// 角色死亡且死亡清空的Buf结束
	if( pActor == NULL )
	{
		return 1;
	}
	if( pActor->getActState().GetState() == GSACT_DEAD && statusDef.DeadClear == 1 )
	{
		end();
		return 1;
	}

	unsigned int ncurtime = timeGetTime();
	if(!m_bDead && m_nStartTime != 0)
	{
		unsigned int npasssec = ( ncurtime - m_nStartTime ) / 1000;

		if ( npasssec >= m_StateInfo.Time)
		{
			int nResult = 0;
			if ( m_bShow ) 
			{
				nResult = 1;
			}
			GameActor* pActor = ActorManager::getSingleton().FindActor(m_ownerId);
			if (pActor && pActor->getType() == GAT_PET)
			{
				end();
			}
			else
			{
				m_bShow = false;
				m_StateInfo.Time = 0;
				m_nStartTime = ncurtime;
			}
			for( int i = 0; i < MAX_STATUS_RESULT; i++ )
			{
				if (statusDef.Result[i].ResultID == STATUS_RESULT_ADD_BIND_SKILL)
				{
					GameMsgPrint::getSingleton().forgetSkill(statusDef.Result[i].ResultVal1, statusDef.Result[i].ResultVal2);
				}
			}

			return nResult;
		}
		else
		{
			m_StateInfo.Time	-= npasssec;
			if ( npasssec != 0 )
			{
				m_nStartTime += npasssec * 1000;
			}

			if ( m_bShow ) 
			{
				return 2;
			}
		}
	}
	return 0;
}

void GameBuf::buf_begin_actorStop()
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	if( pActor != NULL )
	{
		pActor->getShowActor()->getCurEntity()->resetUpdate( true );
		pActor->ChangeActState( GSACT_STOP );
	}
}

void GameBuf::buf_end_actorStop()
{
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	// TODO:在死亡以后会调用结束状态
	if( pActor != NULL )
	{
		pActor->getShowActor()->getCurEntity()->resetUpdate( false );
		if( pActor->getActState().GetState() == GSACT_STOP )
		{
			pActor->ChangeActState( GSACT_STAND );
		}
	}	
}

void GameBuf::buf_begin_eff( int id )
{
	if (id == 0)
	{
		return;
	}
	for( std::list<int>::iterator iter = m_pBufStatus->m_effidlist.begin(); iter != m_pBufStatus->m_effidlist.end(); ++iter )
	{
		if( *iter == id )
		{
			return;
		}
	}

	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	if( pActor != NULL )
	{
		char szBuf[256];
		sprintf( szBuf, "%d", id );
		pActor->getShowActor()->getCurEntity()->playMotion( szBuf, false, id );
		m_pBufStatus->m_effidlist.push_back( id );
	}

	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
	for( int i = 0; i < MAX_STATUS_RESULT; i++ )
	{
		if (statusDef.Result[i].ResultID == STATUS_RESULT_DIAPHANEITY)
		{
			Ogre::Entity *entity = pActor->getShowActor()->getCurEntity();
			ActorManager::getSingleton().removeActorFade(pActor);
			int transparent = statusDef.Result[i].ResultVal1;
			if (transparent < 0)
			{
				transparent = 0;
			}
			else if (transparent >= 100)
			{
				transparent = 99;
			}
			entity->setTransparent(entity->getTransparent() * (1 - transparent / 100.0f));
		}
		else if (statusDef.Result[i].ResultID == STATUS_RESULT_HP_CHUNNEL_FUNC)
		{
			if( pActor != NULL )
			{
				char path[MAX_PATH];
				if (m_StateInfo.ApplyRelation)// 敌对
				{
					sprintf(path, "%d_le", id);
				}
				else
				{
					sprintf(path, "%d_lf", id);
				}
				EffectManage::getSingleton().addLink(m_ownerId, 107, m_StateInfo.ApplyID, 107, path);
			}
		}
	}

	// 冰冻，石化等冻结动作状态
	if( ( statusDef.CltMiscFlag & POWER<2, STATUS_MISC_TYPE_STOP_ACTION>.value ) && pActor && !pActor->isDead() )
	{
		buf_begin_actorStop();
	}
	if (pActor == ActorManager::getSingleton().getMainPlayer())
	{
		if (statusDef.CltMiscFlag & POWER<2, STATUS_MISC_TYPE_VISION_BLUR>::value)
		{
			Ogre::BloomEffect::getSingleton().setGaussBlur(true);
		}

		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			if (statusDef.CltMiscFlag & POWER<2, STATUS_MISC_TYPE_MOTION_BLUR>::value)
			{
				Ogre::BloomEffect::getSingleton().playBlend();
			}
		}
	}
}

void GameBuf::buf_end_eff()
{
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	// 冰冻，石化等冻结动作状态
	if( statusDef.CltMiscFlag & POWER<2, STATUS_MISC_TYPE_STOP_ACTION>.value )
	{
		buf_end_actorStop();
	}

	if (pActor == ActorManager::getSingleton().getMainPlayer())
	{
		if (statusDef.CltMiscFlag & POWER<2, STATUS_MISC_TYPE_VISION_BLUR>::value)
		{
			Ogre::BloomEffect::getSingleton().setGaussBlur(false);
		}

		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			if (statusDef.CltMiscFlag & POWER<2, STATUS_MISC_TYPE_MOTION_BLUR>::value)
			{
				Ogre::BloomEffect::getSingleton().stopBlend();
			}
		}
	}

	if( pActor != NULL && pActor->getShowActor() )
	{
		const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
		if (statusDef.EffectID != 0)
		{
			pActor->getShowActor()->getCurEntity()->stopMotion( statusDef.EffectID );
		}
	}

	for( int i = 0; i < MAX_STATUS_RESULT; i++ )
	{
		if (statusDef.Result[i].ResultID == STATUS_RESULT_DIAPHANEITY)
		{
			Ogre::Entity *entity = pActor->getShowActor()->getCurEntity();
			int transparent = statusDef.Result[i].ResultVal1;
			if (transparent < 0)
			{
				transparent = 0;
			}
			else if (transparent >= 100)
			{
				transparent = 99;
			}
			entity->setTransparent(entity->getTransparent() / (1 - transparent / 100.0f));
		}
		else if (statusDef.Result[i].ResultID == STATUS_RESULT_HP_CHUNNEL_FUNC)
		{
			EffectManage::getSingleton().delLink(m_ownerId, m_StateInfo.ApplyID);
		}
	}

	//取消背
	if( pActor != NULL && pActor->m_pCarry != NULL && statusDef.AddModelID > 0 )
	{
		pActor->m_pCarry->unCarry(0);
	}

	for( std::list<int>::iterator iter = m_pBufStatus->m_effidlist.begin(); iter != m_pBufStatus->m_effidlist.end(); ++iter )
	{
		const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
		if( *iter == statusDef.EffectID )
		{
			m_pBufStatus->m_effidlist.erase( iter );
			return;
		}
	}
}

void GameBuf::buf_reload_eff(int id)
{
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	for( int i = 0; i < MAX_STATUS_RESULT; i++ )
	{
		if (statusDef.Result[i].ResultID == STATUS_RESULT_CHGSIZE)
		{
			pActor->getShowActor()->getCurEntity()->setScale(pActor->getShowActor()->getCurEntity()->getScale() * (statusDef.Result[i].ResultVal1 / 100.0f));
		}
	}
}

void GameBuf::buf_clear_eff()
{
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	for( int i = 0; i < MAX_STATUS_RESULT; i++ )
	{
		if (statusDef.Result[i].ResultID == STATUS_RESULT_CHGSIZE)
		{
			pActor->getShowActor()->getCurEntity()->setScale(pActor->getShowActor()->getCurEntity()->getScale() * (100.0f / statusDef.Result[i].ResultVal1));
		}
	}
}

void GameBuf::buf_eff( int id, int state )
{
	if (id == 0)
	{
		return;
	}
	char szBuf[256];
	std::string strstate = "_1";
	if( state == 1 )
	{
		strstate = "_2";
	}
	sprintf( szBuf, "%d%s", id, strstate.c_str() );
	GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
	if( pActor != NULL )
	{
		pActor->getShowActor()->getCurEntity()->playMotion( szBuf, false, id + 100000000 );
	}
}

int GameBuf::getIconId()
{
	if( m_bDead )
	{
		return 0;
	}
	return DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level ).IconID;
}

bool GameBuf::isShow()   
{ 
	if( m_bDead )
	{
		return false;
	}
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );
	return m_bShow && statusDef.IsCltShow != 1; 
}	

// 是否中断技能
bool GameBuf::isSkillStop( int skillId )
{
	if( !m_bDead )
	{
		const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, 
			m_StateInfo.Level );
		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			int nResult		= statusDef.Result[i].ResultID;
			int nBindSkill	= statusDef.BindSkill.SkillID;
			if( nResult != 0 )
			{
				if( nResult == STATUS_RESULT_NOSKILL && skillId != nBindSkill )
				{
					return true;
				}
			}
			if( nResult != 0 && nResult == STATUS_RESULT_NOSKILL_INCOMPLETE )
			{
				if (skillId == statusDef.Result[i].ResultVal1 ||
					skillId == statusDef.Result[i].ResultVal2 ||
					skillId == statusDef.Result[i].ResultVal3 ||	
					skillId == statusDef.Result[i].ResultVal4 ) 
				{
					return false;
				}else{
					return true;
				}
			}
		}
	}
	return false;
}

bool GameBuf::isSkillStop()
{
	if( !m_bDead )
	{
		const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, 
			m_StateInfo.Level );
		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			int nResult		= statusDef.Result[i].ResultID;
			if( nResult != 0 )
			{
				if( nResult == STATUS_RESULT_NOSKILL)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool GameBuf::isMoveStop()
{
	if (!m_bDead)
	{
		const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, 
			m_StateInfo.Level );
		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			if (statusDef.Result[i].ResultID == STATUS_RESULT_NOCLT)
			{
				return true;
			}
		}
	}
	return false;
}

bool GameBuf::isHasResult(unsigned short result)
{
	if (!m_bDead)
	{
		const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef(m_StateInfo.StatusID, 
			m_StateInfo.Level);
		for (int i = 0; i < MAX_STATUS_RESULT; i++)
		{
			if (statusDef.Result[i].ResultID == result)
			{
				return true;
			}
		}
	}
	return false;
}

bool GameBuf::isEndure()
{
	if (!m_bDead)
	{
		const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, 
			m_StateInfo.Level );
		return statusDef.CltMiscFlag & POWER<2, STATUS_MISC_TYPE_ENDURE>::value;
	}
	return false;
}

void GameBuf::reset(const CSROLESTATUS& StateInfo, unsigned int startTime, int index)
{
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_StateInfo.StatusID, m_StateInfo.Level );

	for( int i = 0; i < MAX_STATUS_RESULT; i++ )
	{
		if (statusDef.Result[i].ResultID == STATUS_RESULT_HP_CHUNNEL_FUNC)
		{
			EffectManage::getSingleton().delLink(m_ownerId, m_StateInfo.ApplyID);
		}
	}

	if (m_StateInfo.Overlay < StateInfo.Overlay)
	{
		buf_eff(statusDef.EffectID);

		// 体型逐渐变化
		for( int i = 0; i < MAX_STATUS_RESULT; i++ )
		{
			if (statusDef.Result[i].ResultID == STATUS_RESULT_CHGSIZE)
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
				int nScale = statusDef.Result[i].ResultVal1;
				int oldMul = statusDef.Result[i].ResultVal1;
				if ( m_StateInfo.Overlay > 0 &&  m_StateInfo.Overlay < statusDef.OverlayLimit  )
				{
					const tagStatusOverlayDef& oldStatusOverlayDef = DBDataManager::getSingleton().getStatusOverlayDef( m_StateInfo.StatusID, m_StateInfo.Level, m_StateInfo.Overlay );
					oldMul = oldStatusOverlayDef.Mul;
				}
				if ( StateInfo.Overlay > 0 && StateInfo.Overlay <= statusDef.OverlayLimit )
				{
					const tagStatusOverlayDef& statusOverlayDef = DBDataManager::getSingleton().getStatusOverlayDef( m_StateInfo.StatusID, m_StateInfo.Level, StateInfo.Overlay );
					nScale = (int)statusOverlayDef.Mul*100/oldMul;
				}
				pActor->registerTickEvent( new GameActor_Tick_ChgSize( pActor, timeGetTime(), 
					nScale, statusDef.Result[i].ResultVal2 ) );
			}
		}
	}
	m_index = index;
	m_StateInfo			= StateInfo;
	if (m_nTotalLife < StateInfo.Time)
	{
		m_nTotalLife = StateInfo.Time;
	}
	m_nStartTime		= timeGetTime();
	m_bDead				= false;
	m_bShow				= true;

	if (m_StateInfo.Time > (m_nStartTime - startTime) / 1000)
	{
		m_StateInfo.Time	-= (m_nStartTime - startTime) / 1000;
	}
	else
	{
		m_StateInfo.Time = 0;
	}
	m_nStartTime		-= (m_nStartTime - startTime) % 1000;

	
	for( int i = 0; i < MAX_STATUS_RESULT; i++ )
	{
		if (statusDef.Result[i].ResultID == STATUS_RESULT_HP_CHUNNEL_FUNC)
		{
			char path[MAX_PATH];
			if (m_StateInfo.ApplyRelation)// 敌对
			{
				sprintf(path, "%d_le", statusDef.EffectID);
			}
			else
			{
				sprintf(path, "%d_lf", statusDef.EffectID);
			}
			EffectManage::getSingleton().addLink(m_ownerId, 107, m_StateInfo.ApplyID, 107, path);
		}
	}
}

void GameBuf::reset(const CSROLESTATUS& StateInfo, int index)
{
	reset(StateInfo, timeGetTime(), index);
}

double GameBuf::getLife()
{
	return m_StateInfo.Time - (timeGetTime() - m_nStartTime) * 0.001;
}

GameBufStatus::GameBufStatus(void) : m_bNextSpeed( false ), m_nGoodStatusNum( 0 ), m_nBadStatusNum( 0 ), m_nNoLimitStatusNum( 0 ), m_lastSetIdx(0)
{
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		m_GoodStatusList[i].m_pBufStatus = this;
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		m_BadStatusList[i].m_pBufStatus = this;
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		m_NolimitStatusList[i].m_pBufStatus = this;
	}
}

GameBufStatus::~GameBufStatus(void)
{
}

void GameBufStatus::setGoodBufStatus( int nNum, const CSROLESTATUS* pGoodStatusList )
{
	unsigned int startTime = timeGetTime();
	int nIndex = 0;
	int index;

	std::map<int, int>	statusList;

	// 先结束buff，再逐个新加
	int nGoodCount = 0;
	for( int i = 0; i < nNum; ++i )
	{
		const CSROLESTATUS& buff = pGoodStatusList[i];
		if( buff.Time > 0 )
		{
			statusList[(buff.StatusID << 16) | buff.Level] = i;
			++nGoodCount;
		}
	}

	m_nGoodStatusNum = nGoodCount;

	for ( int i = 0; i < MAX_GOOD_STATUS; ++i )
	{
		if( !m_GoodStatusList[i].m_bDead )
		{
			std::map<int, int>::iterator it = statusList.find( ( m_GoodStatusList[i].getBufId() << 16 ) | 
				m_GoodStatusList[i].getBufLv() );
			if (it == statusList.end())
			{
				m_GoodStatusList[i].end();
			}
			else
			{
				m_GoodStatusList[i].reset( pGoodStatusList[it->second], startTime, it->second);
				statusList.erase( it );
			}
		}
	}
	index = 0;
	for (std::map<int, int>::iterator i = statusList.begin(); i != statusList.end(); ++i)
	{
		while (!m_GoodStatusList[index].m_bDead)
		{
			++index;
		}
		m_GoodStatusList[index].begin( pGoodStatusList[i->second], startTime, m_ownerId, index );
	}

	updateUI();
}

void GameBufStatus::setBufStatus( const CSSTATUSINFO& status, unsigned int idx)
{
	setBufStatus(status, timeGetTime(), idx);
}

void GameBufStatus::setBufStatus(const CSSTATUSINFO& status, unsigned int startTime, unsigned int idx)
{
	if (m_lastSetIdx > idx)
	{
		return;
	}
	m_lastSetIdx = idx;
	int nIndex = 0;
	int index;

	std::map<int, int>		statusList;

	// 先结束buff，再逐个新加
	statusList.clear();
	int nGoodCount = 0;
	for( int i = 0; i < status.GoodStatusNum; i++ )
	{
		const CSROLESTATUS &buff = status.GoodStatusList[i];
		if( buff.Time > 0 )
		{
			statusList[(buff.StatusID << 16) | buff.Level] = i;
			++nGoodCount;
		}
	}
	m_nGoodStatusNum = nGoodCount;

	for (int i = 0; i < MAX_GOOD_STATUS; i++)
	{
		if( !m_GoodStatusList[i].m_bDead )
		{
			std::map<int, int>::iterator it = statusList.find((m_GoodStatusList[i].getBufId() << 16) | m_GoodStatusList[i].getBufLv());
			if (it == statusList.end())
			{
				m_GoodStatusList[i].end();
			}
			else
			{
				m_GoodStatusList[i].reset(status.GoodStatusList[it->second], startTime, it->second);
				statusList.erase(it);
			}
		}
	}
	index = 0;
	for (std::map<int, int>::iterator i = statusList.begin(); i != statusList.end(); ++i)
	{
		while (!m_GoodStatusList[index].m_bDead)
		{
			++index;
		}
		m_GoodStatusList[index].begin( status.GoodStatusList[i->second], startTime, m_ownerId, i->second);
	}

	statusList.clear();

	int nBadCount = 0;
	for( int i = 0; i < status.BadStatusNum; i++ )
	{
		const CSROLESTATUS &buff = status.BadStatusList[i];
		if( buff.Time > 0 )
		{
			statusList[(buff.StatusID << 16) | buff.Level] = i;
			++nBadCount;
		}
	}
	m_nBadStatusNum = nBadCount;

	for (int i = 0; i < MAX_BAD_STATUS; i++)
	{
		if( !m_BadStatusList[i].m_bDead )
		{
			std::map<int, int>::iterator it = statusList.find((m_BadStatusList[i].getBufId() << 16) | m_BadStatusList[i].getBufLv());
			if (it == statusList.end())
			{
				m_BadStatusList[i].end();
			}
			else
			{
				m_BadStatusList[i].reset(status.BadStatusList[it->second], startTime, it->second);
				statusList.erase(it);
			}
		}
	}
	index = 0;
	for (std::map<int, int>::iterator i = statusList.begin(); i != statusList.end(); ++i)
	{
		while (!m_BadStatusList[index].m_bDead)
		{
			++index;
		}
		m_BadStatusList[index].begin( status.BadStatusList[i->second], startTime, m_ownerId, i->second);
	}

	statusList.clear();
	int nNoLimitCount = 0;
	for( int i = 0; i < status.NoLimitStatusNum; i++ )
	{
		const CSROLESTATUS &buff = status.NolimitStatusList[i];
		if( buff.Time > 0 )
		{
			statusList[(buff.StatusID << 16) | buff.Level] = i;
			++nNoLimitCount;
		}
	}
	m_nNoLimitStatusNum = nNoLimitCount;

	for (int i = 0; i < MAX_NOLIMIT_STATUS; i++)
	{
		if( !m_NolimitStatusList[i].m_bDead )
		{
			std::map<int, int>::iterator it = statusList.find((m_NolimitStatusList[i].getBufId() << 16) | m_NolimitStatusList[i].getBufLv());
			if (it == statusList.end())
			{
				m_NolimitStatusList[i].end();
			}
			else
			{
				m_NolimitStatusList[i].reset(status.NolimitStatusList[it->second], startTime, it->second);
				statusList.erase(it);
			}
		}
	}
	index = 0;
	for (std::map<int, int>::iterator i = statusList.begin(); i != statusList.end(); ++i)
	{
		while (!m_NolimitStatusList[index].m_bDead)
		{
			++index;
		}
		m_NolimitStatusList[index].begin( status.NolimitStatusList[i->second], startTime, m_ownerId, i->second );
	}

	updateUI();
}

void GameBufStatus::setNextSpeed( const CSONEMSPD& nextspeed )
{
	m_bNextSpeed	= true;
	m_nextSpeed		= nextspeed;
}

void GameBufStatus::update( unsigned int dtick )
{
	updateSpeed( dtick );
	int needUpdateUI = 0;
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if( !m_GoodStatusList[i].m_bDead )
		{
			needUpdateUI = m_GoodStatusList[i].update( dtick );
			if( needUpdateUI == 1 )
			{
				updateUI();
			}
		}
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( !m_BadStatusList[i].m_bDead )
		{
			needUpdateUI = m_BadStatusList[i].update( dtick );
			if( needUpdateUI == 1 )
			{
				updateUI();
			}
		}
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if( !m_NolimitStatusList[i].m_bDead )
		{
			needUpdateUI = m_NolimitStatusList[i].update( dtick );
			if( needUpdateUI == 1 )
			{
				updateUI();
			}
		}
	}
}

void GameBufStatus::updateSpeed( unsigned int dtick )
{
	if( !m_bNextSpeed ) return;

	int sec, usec;
	ServerTimer::getSingleton().getServerTime( sec, usec );
	if( m_nextSpeed.SvrTime.BenchTimeSec <= sec )
	{
		if( m_nextSpeed.SvrTime.BenchTimeSec == sec && m_nextSpeed.SvrTime.BenchTimeUSec < usec )
		{
			return;
		}else{
			m_bNextSpeed = false;
			GameActor* pActor = ActorManager::getSingleton().FindActor( m_ownerId );
			assert( pActor != NULL );
			pActor->setSpeed( m_nextSpeed.MoveSpd );

			if( m_nextSpeed.MoveSpd == 0 )
			{
				pActor->onStop( pActor->GetPosition().x/10, pActor->GetPosition().z/10 );
			}
		}
	} 
}

void GameBufStatus::updateUI()
{
	GameActor* pActor		= ActorManager::getSingleton().FindActor( m_ownerId );
	GameActor* pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	assert( pActor != NULL );
	assert( pMainPlayer != NULL );
	if( pActor == pMainPlayer )
	{
		// update MainPlayerUI
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAIN_BUF_LIST_UPDATE" );
	}

	if( pActor->GetID() == pMainPlayer->GetSelectTarget() )
	{
		// update TargetUI
		GameMsgPrint::getSingleton().SendUIEvent( "GE_TARGET_BUF_LIST_UPDATE" );
	}

	if ( pActor->getType() == GAT_PET && pActor->getOwnerId() == pMainPlayer->GetID() )
	{
		GameMsgPrint::getSingleton().SendUIEvent( "GE_MAINPET_BUF_LIST_UPDATE" );
	}
	//是主角的怪物机械
	if( pMainPlayer->isOnMonsterMachine() )
	{
		if( pMainPlayer->getMonsterMachine()->m_playerInfo.m_nMonsterMemID == m_ownerId )
		{
			GameMsgPrint::getSingleton().SendUIEvent( "GE_MONSTER_MACHINE_BUFF_UPDATE" );
		}
	}
}

void GameBufStatus::reLoadAllEff()
{
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if( !m_GoodStatusList[i].m_bDead )
		{
			const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( 
				m_GoodStatusList[i].m_StateInfo.StatusID, m_GoodStatusList[i].m_StateInfo.Level );
			m_GoodStatusList[i].buf_begin_eff( statusDef.EffectID );
			m_GoodStatusList[i].buf_reload_eff(statusDef.EffectID);
		}
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( !m_BadStatusList[i].m_bDead )
		{
			const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_BadStatusList[i].m_StateInfo.StatusID,
				m_BadStatusList[i].m_StateInfo.Level );
			m_BadStatusList[i].buf_begin_eff( statusDef.EffectID );
			m_BadStatusList[i].buf_reload_eff(statusDef.EffectID);
		}
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if( !m_NolimitStatusList[i].m_bDead )
		{
			const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( m_NolimitStatusList[i].m_StateInfo.StatusID,
				m_NolimitStatusList[i].m_StateInfo.Level );
			m_NolimitStatusList[i].buf_begin_eff( statusDef.EffectID );
			m_NolimitStatusList[i].buf_reload_eff(statusDef.EffectID);
		}
	}
}

void GameBufStatus::clearAllEff()
{
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if( !m_GoodStatusList[i].m_bDead )
		{
			m_GoodStatusList[i].buf_clear_eff();
			m_GoodStatusList[i].buf_end_eff();
		}
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( !m_BadStatusList[i].m_bDead )
		{
			m_BadStatusList[i].buf_clear_eff();
			m_BadStatusList[i].buf_end_eff();
		}
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if( !m_NolimitStatusList[i].m_bDead )
		{
			m_NolimitStatusList[i].buf_clear_eff();
			m_NolimitStatusList[i].buf_end_eff();
		}
	}
}

// 取消主角的某个buffer状态
void GameBufStatus::cancelBuffer( int nBufferID, int nBuffLevel )
{	
	const tagStatusDef& statusDef = DBDataManager::getSingleton().getStatusDef( nBufferID, nBuffLevel );
	GameBuf* pBuf = NULL;
	int nIndex = 0;
	if ( statusDef.Type == STATUS_TYPE_GOOD )
	{
		for ( int i = 0; i < MAX_GOOD_STATUS; ++i )
		{
			pBuf = getGoodStatus( i );
			if ( pBuf->getBufId() == nBufferID && pBuf->getBufLv() == nBuffLevel )
			{
				nIndex = pBuf->getIndex();
				break;
			}
		}
	}
	else if ( statusDef.Type == STATUS_TYPE_OTHER )
	{
		for ( int i = 0; i < MAX_NOLIMIT_STATUS; ++i )
		{
			pBuf = getNolimitStatus( i );
			if ( pBuf->getBufId() == nBufferID && pBuf->getBufLv() == nBuffLevel )
			{
				nIndex = pBuf->getIndex();
				break;
			}
		}
	}
	if( pBuf )
	{
		tagCSPkgBody msgbody;
		msgbody.CancelStatus.StatusBeCanceledID = nBufferID;
		msgbody.CancelStatus.Index				= nIndex;
		msgbody.CancelStatus.RemainTime			= unsigned int(pBuf->getLife());
		// 找到被删除的buff
		SendGameServerMsg( CANCEL_STATUS, msgbody );
	}
}

GameBuf* GameBufStatus::getGoodStatus( int index )
{
	assert( index < MAX_GOOD_STATUS );
	return &m_GoodStatusList[index];
}

GameBuf* GameBufStatus::getBadStatus( int index )
{
	assert( index < MAX_BAD_STATUS );
	return &m_BadStatusList[index];
}

GameBuf* GameBufStatus::getNolimitStatus( int index )
{
	assert( index < MAX_NOLIMIT_STATUS );
	return &m_NolimitStatusList[index];
}

GameBuf* GameBufStatus::getJiaStatus( int index )
{
	return &( m_JiaStatusList[index] );
}

// 是否可以使用技能
bool GameBufStatus::canUsedSpell( int nSkillId )
{
	const tagSkillDef& skillDef = DBDataManager::getSingleton().GetSkillDef(nSkillId, SkillManager::getSingleton().getSpellLv(nSkillId));
	if (skillDef.IgnoreNoSkill)
	{
		return true;
	}
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if( m_GoodStatusList[i].isSkillStop( nSkillId ) )
		{
			return false;
		}
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( m_BadStatusList[i].isSkillStop( nSkillId ) )
		{
			return false;
		}
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if( m_NolimitStatusList[i].isSkillStop( nSkillId ) )
		{
			return false;
		}
	}
	return true;
}

bool GameBufStatus::canUsedSpell()
{
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if( m_GoodStatusList[i].isSkillStop() )
		{
			return false;
		}
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( m_BadStatusList[i].isSkillStop() )
		{
			return false;
		}
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if( m_NolimitStatusList[i].isSkillStop() )
		{
			return false;
		}
	}
	return true;
}

// 是否存在Buf
bool GameBufStatus::checkBuf( int nBufId )
{
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if(  !m_GoodStatusList[i].m_bDead && m_GoodStatusList[i].getBufId() == nBufId )
		{
			return true;
		}
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( !m_BadStatusList[i].m_bDead && m_BadStatusList[i].getBufId() == nBufId )
		{
			return true;
		}
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if( !m_NolimitStatusList[i].m_bDead && m_NolimitStatusList[i].getBufId() == nBufId )
		{
			return true;
		}
	}
	return false;
}

// 查询某个buf的状态
GameBuf* GameBufStatus::getBufStatus( int nBufId, int nLv )
{
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if(  !m_GoodStatusList[i].m_bDead && m_GoodStatusList[i].getBufId() == nBufId && 
			m_GoodStatusList[i].getBufLv() == nLv )
		{
			return &m_GoodStatusList[i];
		}
	}

	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( !m_BadStatusList[i].m_bDead && m_BadStatusList[i].getBufId() == nBufId && 
			m_BadStatusList[i].getBufLv() == nLv )
		{
			return &m_BadStatusList[i];
		}
	}

	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if ( !m_NolimitStatusList[i].m_bDead && m_NolimitStatusList[i].getBufId() == nBufId && 
			m_NolimitStatusList[i].getBufLv() == nLv )
		{
			return &m_NolimitStatusList[i];
		}
	}

	return NULL;
}

GameBuf* GameBufStatus::getBufResult(unsigned short result)
{
	for( int i = 0; i < m_nGoodStatusNum; i++ )
	{
		if( m_GoodStatusList[i].isHasResult(result) )
		{
			return &m_GoodStatusList[i];
		}
	}
	for( int i = 0; i < m_nBadStatusNum; i++ )
	{
		if( m_BadStatusList[i].isHasResult(result) )
		{
			return &m_BadStatusList[i];
		}
	}
	for( int i = 0; i < m_nNoLimitStatusNum; i++ )
	{
		if( m_NolimitStatusList[i].isHasResult(result) )
		{
			return &m_NolimitStatusList[i];
		}
	}
	return NULL;
}

const tagStatusDef& GameBufStatus::getStatusDef( int id, int lv )
{
	return DBDataManager::getSingleton().getStatusDef( id, lv );
}

bool GameBufStatus::StatusNoSkill( int id, int lv )
{
	const tagStatusDef& StatusDef = getStatusDef(id, lv);
	for (int i = 0; i < MAX_STATUS_RESULT; ++i)
	{
		if (StatusDef.Result[i].ResultID == STATUS_RESULT_NOSKILL)
		{
			return true;
		}
	}
	return false;
}

void GameBufStatus::addJiaStatus( int nID, int nLv )
{
	int nOldSize = m_JiaStatusList.size();
	m_JiaStatusList.resize( nOldSize + 1 );
	GameBuf&		newJiaStatus	= m_JiaStatusList[nOldSize];
	CSROLESTATUS&	statusInfo		= newJiaStatus.m_StateInfo;
	
	statusInfo.StatusID	= nID;
	statusInfo.Level	= nLv;
	updateUI();
}

void GameBufStatus::delJiaStatus( int nID, int nLv )
{
	for ( std::vector<GameBuf>::iterator iter = m_JiaStatusList.begin(), end = m_JiaStatusList.end(); 
			iter != end; ++iter )
	{
		const GameBuf&		oneBufInfo		= *iter;
		const CSROLESTATUS&	oneStatusInfo	= oneBufInfo.m_StateInfo;
		if ( oneStatusInfo.StatusID == nID )
		{
			m_JiaStatusList.erase( iter );
			updateUI();
			break;
		}
	}
}

bool GameBufStatus::canMove()
{
	for( int i = 0; i < m_nGoodStatusNum; i++ )
	{
		if( m_GoodStatusList[i].isMoveStop() )
		{
			return false;
		}
	}
	for( int i = 0; i < m_nBadStatusNum; i++ )
	{
		if( m_BadStatusList[i].isMoveStop() )
		{
			return false;
		}
	}
	for( int i = 0; i < m_nNoLimitStatusNum; i++ )
	{
		if( m_NolimitStatusList[i].isMoveStop() )
		{
			return false;
		}
	}
	return true;
}

void GameBufStatus::setOwnerId( int Id )
{
	m_ownerId = Id;
	for( int i = 0 ; i < MAX_GOOD_STATUS ; i ++ )
	{
		m_GoodStatusList[i].m_ownerId = Id;
	}
	for( int i = 0 ; i < MAX_BAD_STATUS ; i++ )
	{
		m_BadStatusList[i].m_ownerId = Id;
	}
	for( int i = 0 ; i < MAX_NOLIMIT_STATUS ; i++ )
	{
		m_NolimitStatusList[i].m_ownerId = Id;
	}
}

bool GameBufStatus::isInEndure()
{
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if( m_GoodStatusList[i].isEndure())
		{
			return true;
		}
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( m_BadStatusList[i].isEndure())
		{
			return true;
		}
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if( m_NolimitStatusList[i].isEndure())
		{
			return true;
		}
	}

	return false;
}

bool GameBufStatus::isInNoSkill()
{
	for( int i = 0; i < MAX_GOOD_STATUS; i++ )
	{
		if( m_GoodStatusList[i].isSkillStop( -1 ) )
		{
			return true;
		}
	}
	for( int i = 0; i < MAX_BAD_STATUS; i++ )
	{
		if( m_BadStatusList[i].isSkillStop( -1 ) )
		{
			return true;
		}
	}
	for( int i = 0; i < MAX_NOLIMIT_STATUS; i++ )
	{
		if( m_NolimitStatusList[i].isSkillStop( -1 ) )
		{
			return true;
		}
	}

	return false;
}

unsigned int GameBufStatus::getBufStatusIdx()
{
	static unsigned int ret = 0;
	return ret++;
}

void GameBufStatus::petClearBuff()
{
	for( int i = 0 ; i < MAX_BAD_STATUS ; ++ i )
	{
		m_BadStatusList[i].m_bDead = true;
	}
	m_nBadStatusNum = 0;
	for( int i = 0 ; i < MAX_GOOD_STATUS ; ++ i )
	{
		m_GoodStatusList[i].m_bDead = true;
	}
	m_nGoodStatusNum = 0;
	for( int i = 0 ; i < MAX_NOLIMIT_STATUS ; ++ i )
	{
		m_NolimitStatusList[i].m_bDead = true;
	}
	m_nNoLimitStatusNum = 0;
	GameMsgPrint::getSingleton().SendUIEvent( "GE_MAINPET_BUF_LIST_UPDATE" );
}