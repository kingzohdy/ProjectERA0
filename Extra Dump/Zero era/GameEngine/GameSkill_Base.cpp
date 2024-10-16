#include "GameSkill_Base.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameActor_PVP.h"
#include "GameMap.h"
#include "GameDBData.h"
#include "GameMsgPrint.h"
#include "GameSkill_Common.h"
#include "ShowActor.h"
#include "GameSkill_Package.h"
#include "GameSkill_Mgr.h"
#include "OgreMath.h"
#include "OgreWorldPos.h"
#include "OgreResourceManager.h"
#include "GameSkill_Hit.h"
#include "GameActor_Tick.h"
#include "GameActorManager.h"
#include "GameMaskMap.h"
#include "ShowGameMap.h"
#include "OgreScriptLuaVM.h"
#include "WorldCity.h"

#include "proto_comm.h"
#include "proto_cs.h"
#include "OgreLog.h"
#include "ScriptFunctions.h"
#include "GameTickAction.h"
#include <Windows.h>

using namespace std;
using namespace Ogre;

class AccumulateAttack : public ITickActionDelay
{
protected:
	GameSpellPtr		pSpell;
	std::vector<int>	lstTarget;

	virtual ~AccumulateAttack() {}

public:
	AccumulateAttack(GameSpell *spell, unsigned int delay, const std::vector<int> &targets) : ITickActionDelay(delay), pSpell(spell), lstTarget(targets) {}
	virtual unsigned int expire()
	{
		if( SkillManager::getSingleton().getSpellControler(pSpell->getOwner()) != NULL )
		{
			pSpell->setSpellController(SkillManager::getSingleton().getSpellControler(pSpell->getOwner()));
			pSpell->onAttack(lstTarget);
		}
		unsigned int ret = halt();
		delete this;
		return ret;
	}
};

CenterPosComparer::CenterPosComparer(const GameXYZ &pos) : center(pos)
{
}

bool CenterPosComparer::operator()(GameActor *a, GameActor *b)
{
	return a->GetPosition().subtract(center).length() < b->GetPosition().subtract(center).length();
}

const SKILLRESULT* z_skill_get_result(const SKILLDEF *pstSkillDef, int iResultID)
{
	int i;

	for (i=0; i<MAX_SKILL_RESULT; i++) 
	{
		if (0 == pstSkillDef->SkillResult[i].SkillResultID)
		{
			break;
		}

		if (iResultID == pstSkillDef->SkillResult[i].SkillResultID)
		{
			return pstSkillDef->SkillResult+i;
		}
	}

	return NULL;
}

unsigned int getSystemTick()
{
	return timeGetTime();
}

GameSpell::GameSpell() : 
			m_SkillID(-1), 
			m_OwnerID(-1),
			m_SkillReq(0),
			m_pSpellControler(NULL), 
			m_pGameSkillPackage(NULL),
			m_SkillState(STATE_START),
			m_lastTime(0), 
			m_nPilotNum(0), 
			m_pSpellEffCount(0),
			m_nCollectGrid(0),
			m_pSkillAttrib(NULL),
			m_selfCoolSet(false),
			m_commonCoolSet(false),
			m_isHitBackPrepare(false)
{
	;
}

GameSpell::~GameSpell()
{
	for (size_t i = 0; i < m_lstHits.size(); ++i)
	{
		for (std::list<SkillHit *>::iterator iter = m_lstHits[i].begin();
			iter != m_lstHits[i].end(); ++iter)
		{
			if ((*iter)->actived())
			{
				(*iter)->flush();
			}
			delete (*iter);
		}
	}
	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		delete *i;
	}
	for (vector<GameSkill_Package *>::iterator i = m_pGameSkillPackage.begin();
		i != m_pGameSkillPackage.end(); ++i)
	{
		delete *i;
	}
}

void GameSpell::setTargets(int num, const int targets[MAX_SKILL_TARGET])
{
	size_t index = m_pGameSkillPackage.size();
	if (m_TargetVector.size() <= index)
	{
		m_TargetVector.resize(index + 1);
		for (size_t i = 0; i < num; ++i)
		{
			m_TargetVector[index].push_back(targets[i]);
		}
	}
}

void GameSpell::setSkillMsgPackage( GameSkill_Package* Package )
{
	size_t index = m_pGameSkillPackage.size();
	m_pGameSkillPackage.push_back(Package);
	// 血量变更直接在设置的时候就处理了，不延迟处理了
	Package->updateTargetValue();

	if (m_TargetVector.size() <= index)
	{
		m_TargetVector.resize(index + 1);
		for (size_t i = 0; i < Package->m_HitValue.TotalHit; ++i)
		{
			int nId = Package->m_HitValue.Hits[i].ID;
			GameActor *pActor = ActorManager::getSingleton().FindActor(nId);
			if (pActor && pActor->getType() == GAT_PET && Package->m_HitValue.Hits[i].TiShenFlag != 0)
			{
				nId = pActor->getOwnerId();
			}
			m_TargetVector[index].push_back(nId);
		}
	}
	if (m_lstHits.size() > index)
	{
		std::set<int> targets;
		for (size_t i = 0, j = m_pGameSkillPackage[index]->m_HitValue.TotalHit; i < j; ++i)
		{
			targets.insert(m_pGameSkillPackage[index]->m_HitValue.Hits[i].ID);
		}
		for (std::list<SkillHit *>::iterator i = m_lstHits[index].begin();
			i != m_lstHits[index].end(); ++i)
		{
			for (int id = (*i)->first(); id != 0; id = (*i)->next(id))
			{
				std::set<int>::iterator it = targets.find(id);
				if (it != targets.end())
				{
					targets.erase(it);
				}
			}
			(*i)->setSkillPackage(m_pGameSkillPackage[index]);
		}
		for (std::set<int>::iterator i = targets.begin(); i != targets.end(); ++i)
		{
			m_pGameSkillPackage[index]->doPackage((*i));
		}
	}
}

bool GameSpell::start()
{
	int result = check();
	if (result != RET_SUCCESS && result != RET_OUTRANGE && result != RET_MASKTARGET && result != RET_MASKPOINT)
	{
		return false;
	}
	MainGamePlayer *pActor = ActorManager::getSingleton().getMainPlayer();
	if (result != RET_SUCCESS)
	{
		if (pActor && MOVE_SUCCESS == pActor->CmdMoveTo((int)Ogre::WorldPos::Fix2Flt(getTargetPoint()->x),
			(int)Ogre::WorldPos::Fix2Flt(getTargetPoint()->z), 0))
		{
			m_SkillState = STATE_CATCH;
			return true;
		}
		GameMsgPrint::getSingleton().showMidTip( MID_TARGET_TO_FAR );
		return false;
	}
	
	const tagSkillDef &SkillDef = getSkillDef();
	if ( SkillManager::getSingleton().isNoPrepareSkill( getSpellId() ) )
	{
		pActor->breakBTypePre();
	}

	SkillManager::getSingleton().clearIntentSpell();
	GameSpell *pSpell = m_pSpellControler->getCurSpell();
	if (pSpell)
	{
		pSpell->Break();
	}

	if (pActor->getActState().GetState() != GSACT_AUTOFLY)
	{
		if (pActor->m_Rider.isOnHorse() && pActor->m_Rider.getRideItem().RideID != 0)
		{
			pActor->m_Rider.requestRideDown();
		}
	}
	if(pActor->getActState().GetState() == GSACT_MOVE)
	{
		if (SkillManager::getSingleton().canMoveAttack(getSpellId()))
		{
			if (SkillDef.AttackActTime > 0)
			{
				pActor->CmdMovePause();
			}
		}
		else
		{
			pActor->CmdMoveStop();
		}
	}
	pActor->breakChannel();
	if (getSpellId() == 3223) // tag 暂时措施
	{
		ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_OBJ, getSingleTarget(), getSpellId());
	}
	// 0:顺发法术,1:吟唱,2:集气
	switch (SkillDef.PrepareType)
	{
	case PREPARE_TYPE_NONE:
		{	
			Attack();
		}
		break;
	case PREPARE_TYPE_SING:
		{
			if (SkillManager::getSingleton().getSkillPrepareTime(getSpellId(), getSpellLv()) == 0)
			{
				Attack();
			}
			else
			{
				prepare();
			}
		}
		break;
	case PREPARE_TYPE_COLLECT:
		{
			if (SkillManager::getSingleton().getSkillCollectTime(getSpellId(), getSpellLv()) == 0)
			{
				Attack();
			}
			else
			{
				collect();
			}
		}
		break;
	};
	return true;
}

bool GameSpell::Break()
{
	if (getState() != STATE_BREAK && getState() != STATE_END)
	{
		SkillManager::getSingleton().sendBreakMsg(this);
		m_SkillState = STATE_BREAK;
		m_pSpellControler->onBreak(this);
	}
	SkillManager::getSingleton().setNextSpell();
	return true;
}

bool GameSpell::prepare()
{
	GameActor *pSrc = ActorManager::getSingleton().FindActor(getOwner());
	if (pSrc == NULL)
	{
		return false;
	}
	// 移动需向服务器发送移动停止的包
	if (pSrc->getActState().GetState() == GSACT_MOVE)
	{
		pSrc->CmdMoveStop();
	}
	m_pSpellControler->onPrepare(this);
	SkillManager::getSingleton().sendPrepareMsg(this);
	m_lastTime = timeGetTime();

	setCommonCD();

	return true;
}

bool GameSpell::collect()
{
	GameActor *pSrc = ActorManager::getSingleton().FindActor(getOwner());
	if (pSrc == NULL)
	{
		return false;
	}
	// 移动需向服务器发送移动停止的包
	if (pSrc->getActState().GetState() == GSACT_MOVE)
	{
		pSrc->CmdMoveStop();
	}
	m_pSpellControler->onCollect(this);
	SkillManager::getSingleton().sendPrepareMsg(this);

	setCommonCD();

	return true;
}

void GameSpell::endCollect()
{
	Attack();
}

RESPOS GameSpell::circualMovePoint(const SKILLRESULT* pstSkillResult)
{
	assert( pstSkillResult != NULL );

	GameActor *pSrc = ActorManager::getSingleton().FindActor(getOwner());
	assert( pSrc != NULL );

	RESPOS pos;

	int iDist = pstSkillResult->SkillResultVal1; 
	GameXYZ srcPos, desPos;
	srcPos = pSrc->GetPosition();

	GameActor *pTargetActor = ActorManager::getSingleton().FindActor(getSingleTarget());
	if (pTargetActor == NULL)
	{
		if ( 0 > iDist) //避免往后跳
		{
			if ( getAttackDist() < -iDist )
			{
				pos.X = srcPos.x/10;
				pos.Y = srcPos.z/10;	
				return pos;
			}
		}
		desPos = *(getTargetPoint());

		int angle	=	FaceAngleBetween( srcPos.x/10, srcPos.z/10, desPos.x/10, desPos.z/10 );
		pos.X		=	(int) (desPos.x/10 + (double)iDist * cos((double)(angle)*3.14159265/180.0f));
		pos.Y		=	(int) (desPos.z/10 - (double)iDist * sin((double)(angle)*3.14159265/180.0f));
	}
	else
	{
		if (pTargetActor->getType() == GAT_MONSTER)
		{
			if (iDist > 0)
			{
				iDist += DBDataManager::getSingleton().getMonsterDef(pTargetActor->getResId()).Radius;
			}
			else
			{
				iDist -= DBDataManager::getSingleton().getMonsterDef(pTargetActor->getResId()).Radius;
			}
		}
		if ( 0 > iDist && pstSkillResult->SkillResultVal2 == 0) //避免往后跳
		{
			int nDist = (int)getAttackDist();
			if (pTargetActor->getType() == GAT_MONSTER)
			{
				nDist += DBDataManager::getSingleton().getMonsterDef(pTargetActor->getResId()).Radius;
			}
			if ( nDist < -iDist && pstSkillResult->SkillResultVal2 == 0 )
			{
				pos.X = srcPos.x/10;
				pos.Y = srcPos.z/10;	
				return pos;
			}
		}

		desPos		= pTargetActor->GetPosition();
		int angle	=	FaceAngleBetween( srcPos.x/10, srcPos.z/10, desPos.x/10, desPos.z/10 );

		if( pstSkillResult->SkillResultVal2 != 0 )
		{
			pTargetActor	= pSrc;
			desPos			= pTargetActor->GetPosition();
		}

		pos.X		=	(int) (desPos.x/10 + (double)iDist * cos((double)(angle)*3.14159265/180.0f));
		pos.Y		=	(int) (desPos.z/10 - (double)iDist * sin((double)(angle)*3.14159265/180.0f));
		if( !pSrc->getCurMap()->getMaskMap()->lineOfSightTest( 
			Ogre::Point2D( pSrc->GetPosition().x/10, pSrc->GetPosition().z/10 ),
			Ogre::Point2D( pos.X, pos.Y ),
			MASK_MAGIC | DYN_MASK_MAGIC ) )
		{
			pos.X = desPos.x / 10;
			pos.Y = desPos.z / 10;
		}
	}
	if (!pSrc->getCurMap()->getMaskMap()->canWalkPixelCoord(pos.X, pos.Y))
	{
		pos.X = srcPos.x / 10;
		pos.Y = srcPos.z / 10;
	}
	return pos;
}

void GameSpell::setSingleTarget(int targetid)
{
	m_preSelectTarget = targetid;
}

float GameSpell::getAttackDist()
{
	GameActor *pSrcActor = ActorManager::getSingleton().FindActor(getOwner());
	if( pSrcActor == NULL ) 
	{
		return 0;
	}
	GameActor *pDesActor = ActorManager::getSingleton().FindActor(getSingleTarget());
	GameXYZ selfPos = pSrcActor->GetPosition();
	if (pDesActor == NULL)
	{
		GameXYZ targetPos = *(getTargetPoint());
		selfPos.y = targetPos.y;
		int nnn = (int)selfPos.subtract(targetPos).length();
		return selfPos.subtract(targetPos).length();
	}
	else
	{
		GameXYZ targetPos = pDesActor->GetPosition();
		selfPos.y = targetPos.y;
		unsigned int nRadius = 0;
		if (pDesActor->getType() == GAT_MONSTER)
		{
			nRadius = DBDataManager::getSingleton().getMonsterDef(pDesActor->getResId()).Radius;
		}
		return selfPos.subtract(targetPos).length()///减去目标的触摸半径
			- nRadius;
	}
}

int GameSpell::getSpellLv()
{
	return m_SkillLv;
}

void GameSpell::playAction(int who, int actionId, int motion_class )
{
	GameActor* pself = ActorManager::getSingleton().FindActor( who );
	if( pself == NULL ) 
		return;
	if( actionId == ACTION_ID_NONE )
	{
		if( m_pSpellEffCount > 0 && m_pSpellEffName[0] != NULL )
		{
			pself->playMotion( m_pSpellEffName[0], motion_class );
		}
		m_pSpellEffCount	= 0;
		m_pSpellEffName[0]	= NULL;	
		return;
	}

	if( ( pself->getActState().GetState() == GSACT_ATTACK || pself->getActState().GetState() == GSACT_PREPARE || pself->getActState().getInState(GSACT_IN_CARRY)) )
	{
		return;
	}
	
	int actId = 0;
	if ( actionId == 0 )
	{
		actId = pself->getShowActor()->getCurAction();
	}else{
		actId = pself->getActState().getActionId( actionId );
		if( !pself->hasAnim( actId ) )
		{
			actId = getActId( POSE_NONE, actionId );
		}
	}

	if (pself->isOnMonsterMachine())
	{
		GameActor *pActor = ActorManager::getSingleton().FindActor(pself->m_MonsterMachine.m_playerInfo.m_nMonsterMemID);
		if (pActor != NULL)
		{
			const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(pActor->getResId());
			int pose = POSE_NONE;
			if (info.PoseType != 0)
			{
				pose = info.PoseType;
			}
			pActor->playAction(getActId(pose, actionId), m_pSpellEffName, m_pSpellEffCount, motion_class );
		}
	}else if( pself->isInMorph() )
	{
		//变身状态下没有动作的时候，播放默认动作, hit的时候不播放
		if( !pself->getShowActor()->hasAnim(actId) && motion_class == 0 )
		{
			pself->playAction (getActId(POSE_NONE,400), m_pSpellEffName, m_pSpellEffCount, motion_class );
		}
		else
		{
			pself->playAction (actId, m_pSpellEffName, m_pSpellEffCount, motion_class );
		}
	}else
	{
		if( pself->m_Rider.isOnHorse() )
		{
			pself->m_Rider.horsePlayAct( getActId( POSE_NONE, actionId ), m_pSpellEffName, m_pSpellEffCount, motion_class );
		}else{
			pself->playAction( actId, m_pSpellEffName, m_pSpellEffCount, motion_class );
		}
	}
	
	//// 机枪攻击为循环攻击
	//if( pself->m_Equip.getRightHandEquipType() == ARM_HEVY_SCATTERGUN )
	//{
	//	pself->getActState().SetState( GSACT_ATTACK );
	//}

//	m_pSpellEffCount	= 0;
//	m_pSpellEffName[0]	= NULL;	
}

void GameSpell::update(unsigned int dtick)
{
	if (!SkillManager::getSingleton().isSkillLearned(getSpellId()))
	{
		Break();
		return;
	}
	GameActor *pSrcObj = ActorManager::getSingleton().FindActor(getOwner());
	if (pSrcObj == NULL || pSrcObj->getActState().GetState() == GSACT_DEAD) 
	{
		Break();
		return;
	}
	unsigned int curtime = timeGetTime();

	switch (m_SkillState)
	{
	case STATE_PREPARE:
		{
			const tagSkillDef& SkillDef = getSkillDef();
			bool bInFight = pSrcObj->isInFight();
			if ((bInFight && SkillDef.NoFightStatUse == NO_FIGHT_USE_PREPARE) || !bInFight && SkillDef.NoFightStatUse == ENTER_FIGHT_USE_PREPARE)
			{
				Break();
				return;
			}
			unsigned int nPreTime = SkillManager::getSingleton().getSkillPrepareTime(getSpellId()) * 100;
			if (curtime - m_lastTime >= nPreTime)
			{
				pSrcObj->ChangeActState( GSACT_STAND );
				Attack();
			}
		}
		break;

	case STATE_COLLECT:
		{
			const tagSkillDef& SkillDef = getSkillDef();
			bool bInFight = pSrcObj->isInFight();
			if ((bInFight && SkillDef.NoFightStatUse == NO_FIGHT_USE_PREPARE) || !bInFight && SkillDef.NoFightStatUse == ENTER_FIGHT_USE_PREPARE)
			{
				Break();
				return;
			}
			if(!SkillManager::getSingleton().isSkillLearned(getSpellId()))
			{
				Break();
				return;
			}
		}
		break;

	case STATE_PILOT:
		{
			if (m_nPilotNum < SkillManager::getSingleton().getSkillPilotNum(getSpellId()))
			{
				if ( curtime - m_lastTime >= SkillManager::getSingleton().getSkillPilotTime(getSpellId()) * 100 )
				{
					attackTime();
				}
			}
		}
		break;
	}
	onUpdate(dtick);
}

void GameSpell::onUpdate(unsigned int dtick)
{
	unsigned int curtime = timeGetTime();

	switch (m_SkillState)
	{
	case STATE_COLLECT:
		{
			unsigned int nPassOneGridTime  = SkillManager::getSingleton().getSkillCollectTime(getSpellId(), getSpellLv()) * 100;
			int nCollectNum = SkillManager::getSingleton().getSkillCollectNum(getSpellId(), getSpellLv());
			if (m_nCollectGrid < nCollectNum)
			{
				if (curtime - m_lastTime >= nPassOneGridTime * (m_nCollectGrid + 1))
				{
					++m_nCollectGrid;
					if (m_nCollectGrid < nCollectNum)
					{
						m_pSpellControler->onCollect(this, m_nCollectGrid);
					}
					else
					{
						m_pSpellControler->onCollectFull(this);
					}
				}
			}
		}
		break;
	}
	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onUpdate(this, curtime);
	}
}

bool GameSpell::canAttackSelf()
{
	const tagSkillDef& SkillDef = getSkillDef();

	return ( ( SkillDef.TargetType & SKILL_TARGET_SELF ) == SKILL_TARGET_SELF );
}

void GameSpell::setSelfPosChange( int x, int y )
{
	m_SelfPosChange.X = x;
	m_SelfPosChange.Y = y;	
}

bool GameSpell::filter(GameActor *pActor)
{
	GameActor* pSrc = ActorManager::getSingleton().FindActor( getOwner() );
	if( pSrc == NULL ) 
	{
		return false;
	}
	GameMap *map = pSrc->getCurMap();
	if (map == NULL)
	{
		return false;
	}
	if (pActor->isOnMonsterMachine())
	{
		return false;
	}
	// 正在放出场动画的BOSS不能打
	if (pActor->getActState().getInState(GSACT_IN_BIRTH) || pActor->getActState().GetState() == GSACT_AUTOFLY)
	{
		return false;
	}
	//时间城增幅器存在，则终端控制器不能攻击
	if ( !WorldCityManager::getSingleton().objCanAttack(pActor) )
	{
		return false;
	}
	int resid = pActor->getResId();
	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(resid);
	if (info.MiscType & (NPCDISPLAYINFO_MISC_TYPE_NO_SELECT|NPCDISPLAYINFO_MISC_TYPE_NO_ATKEE))
	{
		return false;
	}
	const tagSkillDef &SkillDef	= getSkillDef();
	// 过滤施法对象阵营选择
	if (!pActor->isCamp(SkillDef.TargetType))
	{
		return false;
	}
	// 过滤对死亡目标使用
	if( pActor->getActState().GetState() == GSACT_DEAD && !SkillDef.DeadTarget )
	{
		return false;
	}
	// 过滤合体的宠物
	if (pActor->getType() == GAT_PET && pActor->m_isHeti)
	{
		return false;
	}

	const GameXYZ &selfPos = pSrc->GetPosition();
	// 检测法术阻挡层
	if( !(SkillDef.NoSkillStop & NOSKILLSTOP_TYPE_MAGIC) && !map->getMaskMap()->lineOfSightTest( 
		Ogre::Point2D( selfPos.x/10, selfPos.z/10 ),
		Ogre::Point2D( pActor->GetPosition().x/10, pActor->GetPosition().z/10 ), GameMaskMap::maskMagic )) 
	{
		return false;
	}
	if (!map->getMaskMap()->lineOfSightTest(
		Ogre::Point2D( selfPos.x/10, selfPos.z/10 ),
		Ogre::Point2D( pActor->GetPosition().x/10, pActor->GetPosition().z/10 ), GameMaskMap::maskLoMagic, SkillDef.ThroughLoMagic))
	{
		return false;
	}
	// 技能范围类型
	const RESRANGE& MainRange = SkillManager::getSingleton().getSkillMainRange(getSpellId());
	unsigned int nRadius = 0;
	if (pActor->getType() == GAT_MONSTER)
	{
		nRadius = DBDataManager::getSingleton().getMonsterDef(pActor->getResId()).Radius;
	}
	switch( MainRange.RangeType )
	{
	case SKILL_RANGE_SIGLE:
		{
			return selfPos.subtract(pActor->GetPosition()).length() - nRadius <= SkillManager::getSingleton().getSkillDistance(getSpellId());
		}
		return pActor->GetID() == getSingleTarget();
	case SKILL_RANGE_CIRCLE:
		{
			GameXYZ targetPos = pActor->GetPosition();
			if (pActor->getType() == GAT_PET)
			{
				GameActor *owner = ActorManager::getSingleton().FindActor(pActor->getOwnerId());
				if (owner)
				{
					targetPos = owner->GetPosition();
				}
			}
			return IsPointInCircle( targetPos.x/10, targetPos.z/10,  m_TargetPoint.x/10, m_TargetPoint.z/10, MainRange.RangeVal1 + nRadius);
		}
		break;
	case SKILL_RANGE_RECT:
		{
			GameXYZ targetPos = pActor->GetPosition();
			if (pActor->getType() == GAT_PET)
			{
				GameActor *owner = ActorManager::getSingleton().FindActor(pActor->getOwnerId());
				if (owner)
				{
					targetPos = owner->GetPosition();
				}
			}
			int angle = pSrc->getAngle();
			const GameXYZ *AttackPos = getTargetPoint();
			if (AttackPos)
			{
				const GameXYZ &attackPos = *AttackPos;
				if (attackPos.x  != selfPos.x || attackPos.z != selfPos.z)
				{
					angle = FaceAngleBetween(selfPos.x, selfPos.z, attackPos.x, attackPos.z);
				}
			}
			return IsPointInRectangle( targetPos.x / 10, targetPos.z / 10, selfPos.x / 10, selfPos.z / 10,
				MainRange.RangeVal2 + nRadius, MainRange.RangeVal1 + nRadius, angle);
		}
		break;
	case SKILL_RANGE_ARC:
		{
			GameXYZ targetPos = pActor->GetPosition();
			if (pActor->getType() == GAT_PET)
			{
				GameActor *owner = ActorManager::getSingleton().FindActor(pActor->getOwnerId());
				if (owner)
				{
					targetPos = owner->GetPosition();
				}
			}
			int angle = pSrc->getAngle();
			const GameXYZ *AttackPos = getTargetPoint();
			if (AttackPos)
			{
				const GameXYZ &attackPos = *AttackPos;
				if (attackPos.x  != selfPos.x || attackPos.z != selfPos.z)
				{
					angle = FaceAngleBetween(selfPos.x, selfPos.z, attackPos.x, attackPos.z);
				}
			}
			return IsPointInFan( targetPos.x / 10, targetPos.z / 10, selfPos.x / 10, selfPos.z / 10, MainRange.RangeVal1 + nRadius,
				angle, MainRange.RangeVal2);
		}
		break;
	};

	return false;
}

void GameSpell::calSkillTargets()
{
	const GameXYZ *AttackPos = getTargetPoint();
	size_t index = m_TargetVector.size();
	m_TargetVector.resize(index + 1);
	const tagSkillDef &SkillDef		= getSkillDef();
	switch (SkillManager::getSingleton().getSkillMainRange(getSpellId()).RangeType)
	{
	case SKILL_RANGE_SIGLE:
		{
			m_TargetVector[index].push_back(getSingleTarget());
		}
		break;
	case SKILL_RANGE_RECT:
	case SKILL_RANGE_ARC:
	case SKILL_RANGE_CIRCLE:
		std::vector<GameActor *> targets;
		// 优先添加自己
		if( SkillDef.TargetType & SKILL_TARGET_SELF )
		{
			GameActor *pSrcActor = ActorManager::getSingleton().FindActor( getOwner() );
			if (pSrcActor)
			{
				targets.push_back(pSrcActor);
			}
		}
		// 优先添加法术目标
		if (getSingleTarget() != getOwner())
		{
			GameActor *pDesActor = ActorManager::getSingleton().FindActor( getSingleTarget() );
			if (pDesActor)
			{
				targets.push_back(pDesActor);
			}
		}
		for( ACTOR_ITER iter  = ActorManager::getSingleton().getActorBegin(); 
			iter != ActorManager::getSingleton().getActorEnd(); ++iter)
		{
			// 过滤自己和目标
			if (iter->first == getSingleTarget() || iter->first == getOwner())
			{
				continue;
			}
			GameActor* pActor = iter->second;
			if( pActor->getType() == GAT_ITEM ||		// 不能以Item为目标
				(pActor->getType() == GAT_MONSTER &&
				(pActor->getSubType() == MON_SUB_TYPE_COLLECT ||
				pActor->getSubType() == MON_SUB_TYPE_TRAP ||
				pActor->getSubType() == MON_SUB_TYPE_BUILD)))	// 过滤特殊物品
			{
				continue;
			}
			targets.push_back(pActor);
		}
		sortTargets(targets);
		unsigned int TargetNum = SkillManager::getSingleton().getSkillMaxTargets(getSpellId());
		if (TargetNum > 0)
		{
			for (std::vector<GameActor *>::iterator i = targets.begin(); i != targets.end(); ++i)
			{
				if (filter(*i))
				{
					m_TargetVector[index].push_back((*i)->GetID());
					if (0 == --TargetNum)
					{
						break;
					}
				}
			}
		}
	}
}

const char* GameSpell::getAttackEffectName( const char* szEffectName )
{
	GameActor* pSrc = ActorManager::getSingleton().FindActor( m_OwnerID );
	if( pSrc == NULL )
	{
		return szEffectName;
	}
	
	static char buf1[256];
	static char buf2[256];
	char szPath[256];
	const char *result = szEffectName;
	sprintf(buf1, "%s_%d", result, pSrc->m_Equip.getRightHandEquipType());
	sprintf(szPath, "effect//public//%s.emo", buf1);
	if (Ogre::ResourceManager::getSingleton().getFileAttrib(szPath) != 0)
	{
		result = buf1;
	}

	const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(pSrc->getResId());
	sprintf(buf2, "%s_%s", result, info.gender ? "f" : "m");
	sprintf(szPath, "effect//public//%s.emo", buf2);
	if (Ogre::ResourceManager::getSingleton().getFileAttrib(szPath) != 0)
	{
		result = buf2;
	}

	return result;
}

const char* GameSpell::getHitEffectName(int id, const char* szEffectName)
{
	GameActor* pSrc = ActorManager::getSingleton().FindActor( m_OwnerID );
	if( pSrc == NULL )
	{
		return szEffectName;
	}

	static char buf1[256];
	static char buf2[256];
	char szPath[256];
	const char *result = szEffectName;
	sprintf(buf1, "%s_%d", result, pSrc->m_Equip.getRightHandEquipType());
	sprintf(szPath, "effect//public//%s.emo", buf1);
	if (Ogre::ResourceManager::getSingleton().getFileAttrib( szPath ) != 0)
	{
		result = buf1;
	}

	GameActor *pDes = ActorManager::getSingleton().FindActor(id);
	if (pDes && (pDes->getType() == GAT_PLAYER || pDes->getType() == GAT_MAINPLAYER))
	{
		const tagNpcDisplayInfo &info = DBDataManager::getSingleton().GetNpcDisplayInfo(pDes->getResId());
		sprintf(buf2, "%s_%s", result, info.gender ? "f" : "m");
		sprintf(szPath, "effect//public//%s.emo", buf2);
		if (Ogre::ResourceManager::getSingleton().getFileAttrib(szPath) != 0)
		{
			result = buf2;
		}
	}

	return result;
}

const GameXYZ * GameSpell::getTargetPoint()
{
	return &m_TargetPoint;
}

void GameSpell::setSelfPoint(int x, int y)
{
	GameXYZ pos(x, 0, y);
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer != NULL)
	{
		pMainPlayer->getCurMap()->getTerrainH(pos.x, pos.z, pos.y);
	}
	setSelfPoint(pos);
}

void GameSpell::setSelfPoint(const GameXYZ &pos)
{
	m_selfPos = pos;
}

const GameXYZ & GameSpell::getSelfPoint()
{
	return m_selfPos;
}

void GameSpell::setTargetPoint(int x, int y)
{
	GameXYZ pos(x, 0, y);
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer != NULL)
	{
		pMainPlayer->getCurMap()->getTerrainH(pos.x, pos.z, pos.y);
	}
	setTargetPoint(pos);
}

void GameSpell::setSpellId( int id )				
{ 
	m_SkillID = id;
	m_pSkillAttrib = NULL;
	GameActor* pSrcObj = ActorManager::getSingleton().FindActor(getOwner());
	if( pSrcObj && pSrcObj->getType() == GAT_MAINPLAYER 
		&& SkillManager::getSingleton(). getSkillCommCoolTime(id) == 0 )
	{
		setCommonCD();
	}
}

int GameSpell::getSingleTarget()
{
	return 0;
}

void GameSpell::setSpellLv(int lv)
{
	m_SkillLv = lv;
}

int GameSpell::getTarget(size_t index)
{
	if (index >= getTargetNum())
	{
		return 0;
	}
	return m_TargetVector.back()[index];
}

size_t GameSpell::getTargetNum()
{
	if (m_TargetVector.empty())
	{
		return 0;
	}
	if (m_nPilotNum > m_TargetVector.size())
	{
		return 0;
	}
	return m_TargetVector.back().size();
}

bool GameSpell::isAutoSkill()
{
	return false;
}

void * GameSpell::getBestAttrib(std::vector<void *> &v)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(getOwner());
	for (size_t i = v.size(); i != 0; i--)
	{
		tagSkillAttrib *attrib = (tagSkillAttrib*)v[i - 1];
		if ((attrib->m_nFittingId == 0) && (attrib->m_nBuffId == 0 || (pActor && pActor->getBufStatus()->checkBuf(attrib->m_nBuffId))))
		{
			for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
			{
				delete *i;
			}
			m_lstObserver.clear();
			for (list<SKillObserver *>::iterator i = attrib->m_lstObserver.begin(); i != attrib->m_lstObserver.end(); ++i)
			{
				m_lstObserver.push_back((*i)->clone());
			}
			return attrib;
		}
	}
	return (tagSkillAttrib*)v[0];
}

bool GameSpell::Attack()
{
	GameActor* pSrcObj = ActorManager::getSingleton().FindActor(getOwner());

	if (pSrcObj->getActState().GetState() == GSACT_MOVE)
	{
		if (!SkillManager::getSingleton().canMoveAttack(getSpellId()))
		{
			pSrcObj->CmdMoveStop();
		}
	}
	attackTime();

	setCommonCD();
	setSelfCD();

	return true;
}

void GameSpell::Hit()
{
	const tagSkillDef& SkillDef = getSkillDef();
	GameActor *pOwner = ActorManager::getSingleton().FindActor(getOwner());
	if (SkillDef.AtkType == SKILL_ATK_TYPE_POS && pOwner == ActorManager::getSingleton().getMainPlayer())
	{
		// 计算目范法术攻击目标
		calSkillTargets();
		SkillManager::getSingleton().sendHitMsg(this);
	}
	size_t index = m_lstHits.size();
	if (m_TargetVector.size() > index)
	{
		m_lstHits.push_back(SkillHit::findCreator(this)(this, m_TargetVector[index]));
		if (m_pGameSkillPackage.size() > index)
		{
			std::set<int> targets;
			for (size_t i = 0, j = m_pGameSkillPackage[index]->m_HitValue.TotalHit; i < j; ++i)
			{
				targets.insert(m_pGameSkillPackage[index]->m_HitValue.Hits[i].ID);
			}
			for (std::vector<int>::iterator i = m_TargetVector[index].begin(), j = m_TargetVector[index].end();
				i != j; ++i)
			{
				std::set<int>::iterator it = targets.find((*i));
				if (it != targets.end())
				{
					targets.erase(it);
				}
			}
			for (std::list<SkillHit *>::iterator i = m_lstHits[index].begin();
				i != m_lstHits[index].end(); ++i)
			{
				(*i)->setSkillPackage(m_pGameSkillPackage[index]);
			}
			for (std::set<int>::iterator i = targets.begin(); i != targets.end(); ++i)
			{
				m_pGameSkillPackage[index]->doPackage((*i));
			}
		}
	}
	else
	{
		m_lstHits.resize(index + 1);
	}
	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onHit(this);
	}
}

void GameSpell::attackTime()
{
	GameActor* pSrcObj = ActorManager::getSingleton().FindActor(getOwner());
	if (pSrcObj == NULL)
	{
		Break();
		return;
	}
	setSelfPoint(pSrcObj->GetPosition());
	int result = check();
	if (result != RET_SUCCESS)
	{
		Break();
		return;
	}
	m_lastTime = timeGetTime();
	const tagSkillDef& SkillDef = getSkillDef();
	if (SkillDef.AtkType != SKILL_ATK_TYPE_POS)
	{
		// 计算目范法术攻击目标
		calSkillTargets();
	}
	tagSkillAttrib* pAttrib = (tagSkillAttrib*)getAttrib();
	if( pAttrib && pAttrib->m_nMoveTime != -1 )
	{
		setHitDelay(getHitDelay() + DEFAULT_DELAY);/// 加上位移技能的默认延迟
		const SKILLRESULT* pstSkillResult = z_skill_get_result( &SkillDef, RESULT_SELF_POSCHG );
		if( pstSkillResult )
		{
			RESPOS		pos = circualMovePoint(pstSkillResult);
			setSelfPosChange( pos.X, pos.Y );
			if (getSingleTarget() == 0)
			{
				setTargetPoint(pos.X * 10, pos.Y * 10);
			}
		}
	}
	// 攻击计算
	onAttack();
	++m_nPilotNum;

	SkillManager::getSingleton().sendAttackMsg(this);
	// 设置下次循环调用法术
	const GameXYZ *pos = getTargetPoint();
	std::vector<int> nextskills;
	if (getSpellId() == 7003)//tag 暂时措施
	{
		SkillManager::getSingleton().setNextSpell(-1, m_preSelectTarget);
	}
	else
	{
		if (SkillDef.NextSkill != 0)
		{
			nextskills.push_back(SkillDef.NextSkill);
		}
		if (SkillDef.NextSkill2 != 0)
		{
			nextskills.push_back(SkillDef.NextSkill2);
		}
		if (SkillDef.NextSkill3 != 0)
		{
			nextskills.push_back(SkillDef.NextSkill3);
		}
		if (nextskills.size() == 0)
		{
			SkillManager::getSingleton().setNextSpell();
		}
		else
		{
			int nextskill = nextskills[rand() % nextskills.size()];
			SkillManager::getSingleton().setNextSpell(nextskill, m_preSelectTarget, pos->x, pos->z, m_SkillID == nextskill ||
				DBDataManager::getSingleton().GetSkillDef(nextskill, SkillManager::getSingleton().getSpellLv(nextskill)).SkillType != SKILL_TYPE_NORMAL);
		}
	}
	if (getState() == STATE_PILOT)
	{
		if (m_nPilotNum >= SkillManager::getSingleton().getSkillPilotNum(getSpellId()))
		{
			SkillManager::getSingleton().spellBreak(this, getHitDelay() + 200);
		}
	}
}

// 播放吟唱动作
void GameSpell::onPrepare()	 
{
	GameActor *pSrcObj		= ActorManager::getSingleton().FindActor(getOwner());
	if( pSrcObj == NULL )
	{
		Break();
		return;
	}
	pSrcObj->setPrepareSkill(getSpellId(), getSpellLv(), getFittingIndex(), timeGetTime());
	tagSkillAttrib* pAttrib = (tagSkillAttrib*)getAttrib();
	m_SkillState			= STATE_PREPARE;
	pSrcObj->ChangeActState(GSACT_COLLECT);
	if (pAttrib->m_isPreHideWeapon)
	{
		pSrcObj->hideWeapon(true);
	}
	// 施法者朝向施法点
	const GameXYZ *targetPos = getTargetPoint();
	if (targetPos != NULL)
	{
		pSrcObj->SetFaceTo( pSrcObj->GetPosition().x/10, 
			pSrcObj->GetPosition().z/10, 
			targetPos->x/10,
			targetPos->z/10 );
	}

	// 技能吟唱发言
	if( pSrcObj->getType() == GAT_MONSTER)
	{
		char szScript[256];
		sprintf( szScript, "SetMonsterSkillSpeech(%d,%d,%d,%d)", pSrcObj->getResId(), pSrcObj->GetID(), SKILL_PREPARE, pAttrib->m_nSkillId );
		ActorManager::getSingleton().getScriptVM()->callString( szScript );
	}


	if( !pAttrib->m_strPreEffName.empty() )
	{
		m_pSpellEffName[0]	= pAttrib->m_strPreEffName.c_str();
		m_pSpellEffCount	= 1;
	}
	else
	{
		m_pSpellEffCount	= 0;
	}
	// 播放准备特效
	playAction( m_OwnerID, pAttrib->m_PreActId);
	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onPrepare(this);
	}
	for( int i = 0 ; i < SkillManager::getSingleton().MAX_ARMTYPE ; i++ )
	{
		if( SkillManager::getSingleton().getSpellWeaponType(getSpellId(),getSpellLv(),i) == ARM_LIUDAN_GUN )
		{
			pSrcObj->m_Equip.ShowShoulder( true );
			break;
		}
	}
}

// 播放集气动作
void GameSpell::onCollect()	 
{
	GameActor *pSrcObj		= ActorManager::getSingleton().FindActor( m_OwnerID );
	if( pSrcObj == NULL )
	{
		Break();
		return;
	}
	pSrcObj->setPrepareSkill(getSpellId(), getSpellLv(), getFittingIndex(), timeGetTime());
	tagSkillAttrib* pAttrib = (tagSkillAttrib*)getAttrib();
	m_SkillState			= STATE_COLLECT;
	m_lastTime = timeGetTime();
	pSrcObj->ChangeActState(GSACT_COLLECT);
	if (pAttrib->m_isPreHideWeapon)
	{
		pSrcObj->hideWeapon(true);
	}
	if( !pAttrib->m_CollectEffectName.empty() )
	{
		m_pSpellEffName[0]	= pAttrib->m_CollectEffectName.c_str();
		m_pSpellEffCount	= 1;
	}
	playAction( m_OwnerID, pAttrib->m_CollectActId);

	pSrcObj->SetFaceTo( pSrcObj->GetPosition().x/10, 
		pSrcObj->GetPosition().z/10, 
		m_TargetPoint.x/10,
		m_TargetPoint.z/10 );
	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onCollect(this);
	}
	for( int i = 0 ; i < SkillManager::getSingleton().MAX_ARMTYPE ; i++ )
	{
		if( SkillManager::getSingleton().getSpellWeaponType(getSpellId(),getSpellLv(),i) == ARM_LIUDAN_GUN )
		{
			pSrcObj->m_Equip.ShowShoulder( true );
			break;
		}
	}
}

// 播放爆气动作
void GameSpell::onCollectTime( int time ) 
{
	tagSkillAttrib* pAttrib = (tagSkillAttrib*)getAttrib();
	GameActor* pSrcObj		= ActorManager::getSingleton().FindActor( getOwner() );
	if( pSrcObj == NULL )
	{
		m_SkillState = STATE_END;
		return;
	}

	int actId = pSrcObj->getActState().getActionId( pAttrib->m_CollectActId );
	if( !pSrcObj->hasAnim( actId ) )
	{
		actId = getActId( POSE_NONE, pAttrib->m_CollectActId );
	}

	if( !pAttrib->m_CollectBlastEffectName.empty() )
	{
		m_pSpellEffName[0]	= pAttrib->m_CollectBlastEffectName.c_str();
		m_pSpellEffCount	= 1;
		pSrcObj->playAction( actId, m_pSpellEffName, m_pSpellEffCount, 2 );
	}

	if (time > 0 && !pAttrib->m_CollectPower[time - 1].empty())
	{
		m_pSpellEffName[0]	= pAttrib->m_CollectPower[time - 1].c_str();
		m_pSpellEffCount	= 1;
		pSrcObj->playAction( actId, m_pSpellEffName, m_pSpellEffCount );
	}
	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onCollectTime(this, time);
	}
}

void GameSpell::onCollectFull()
{
	onCollectTime(m_nCollectGrid);
	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onCollectFull(this);
	}
}

// 播放攻击动作
void GameSpell::onAttack(bool passive/* = false*/)
{
	GameActor *pSrcObj		= ActorManager::getSingleton().FindActor(getOwner());
	if (pSrcObj == NULL || pSrcObj->isDead() )
	{
		if( pSrcObj->getType() != GAT_MAINPLAYER )
		{
			GameSpell::Hit();
		}
		return;
	}
	GameSpell *curSpell = SkillManager::getSingleton().getSpellControler(m_OwnerID)->getCurSpell();
	const tagSkillDef &SkillDef	= getSkillDef();
	tagSkillAttrib* pAttrib = (tagSkillAttrib*)getAttrib();
	if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT && SkillDef.JiqiChgFlag == 1)
	{
		if (m_SkillState < STATE_PILOT)
		{
			int delay = getHitDelay() + 200;
			int n = Min(m_nCollectGrid, 2);
			for (int i = 0; i < n; ++i)
			{
				new AccumulateAttack(this, (i + 1) * delay, m_TargetVector.back());
			}
		}
	}
	if ((curSpell == NULL || curSpell == this) && pAttrib->m_nMoveTime != -1 && pSrcObj->getActState().GetState() != GSACT_STAND && SkillDef.PilotNum == 0)
	{
		pSrcObj->ChangeActState(GSACT_STAND);
	}
	if (passive)
	{
		for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
		{
			(*i)->onAttack(this);
		}

		if (pAttrib->m_strScript != "")
		{
			ScriptFunctions::getSingleton().callSpellScript(pAttrib->m_strScript.c_str(), this);
		}

		if( pAttrib->m_nMoveTime != -1 )
		{
			const char *effect = "";
			if( !pAttrib->m_ActEffName.empty() )
			{
				effect = getAttackEffectName(pAttrib->m_ActEffName.c_str());
			}
			pSrcObj->doBack( getSpellId(), getOwner(), m_SelfPosChange.X, m_SelfPosChange.Y, pAttrib->m_nMoveTime, BACK_LINE, 0, 0, "", DEFAULT_DELAY, false, pAttrib->m_ActId[0], effect );
		}

		int delay = getHitDelay();
		if (delay == 0)
		{
			Hit();
		}
		else
		{
			SkillManager::getSingleton().spellAttack(this, delay);
		}
	}
	else
	{
		// 进入战斗状态
		if( SkillDef.FightStat == SKILL_FIGHT_STAT_BOTH || SkillDef.FightStat == SKILL_FIGHT_STAT_SINGLE)
		{
			if (getTargetNum() > 0 && SkillDef.FightStat == SKILL_FIGHT_STAT_BOTH && !pSrcObj->getActState().getInState( GSACT_IN_FIGHT ))
			{
				pSrcObj->onFight(false);
			}
			// 将怪物带入战斗状态
			for (size_t i = 0; i < getTargetNum(); ++i)
			{
				GameActor* pActor = ActorManager::getSingleton().FindActor( getTarget(i) );
				if( pActor != NULL && pActor->getType() == GAT_MONSTER )
				{
					pActor->onFight();
				}
			}
		}

		// 锁定攻击判定
		if(m_SkillState < STATE_PREPARE && (curSpell == NULL || curSpell == this))
		{
			if( getSpellId() != 7005 && pSrcObj->getActState().GetState() != GSACT_MOVE )
			{
				if( pSrcObj->getActState().GetState() != GSACT_SIT && 
					pSrcObj->getActState().GetState() != GSACT_PREPARE && 
					pSrcObj->getActState().GetState() != GSACT_ATTACK &&
					pSrcObj->getActState().GetState() != GSACT_AUTOFLY )
				{
					pSrcObj->ChangeActState( GSACT_STAND );
				}
			}
		}
		pSrcObj->setPrepareSkill();
		if( SkillDef.PilotNum > 0 )
		{
			if(m_SkillState < STATE_PILOT )
			{
				m_pSpellControler->onPilot(this);
				pSrcObj->setPilotSkill(getSpellId());
			}
		}
		else
		{
			m_pSpellControler->onComplete(this);
			if (m_SkillState != STATE_ATTACK)
			{
				m_SkillState = STATE_ATTACK;
				pSrcObj->setPilotSkill();
			}
		}

		if ((SkillDef.TargetType & SKILL_TARGET_ENEMY_MON) || (SkillDef.TargetType & SKILL_TARGET_ENEMY_PLAYER))
		{
			GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
			int nMainPlayerId = pActor ? pActor->GetID() : 0;
			if (getOwner() == nMainPlayerId)
			{
				for (size_t i = 0; i < getTargetNum(); ++i)
				{
					ActorManager::getSingleton().lastAttacker[getTarget(i)] = timeGetTime() + LOCK_ENEMY;
				}
			}
			else
			{
				for (size_t i = 0; i < getTargetNum(); ++i)
				{
					if (nMainPlayerId == getTarget(i))
					{
						ActorManager::getSingleton().lastAttacker[getOwner()] = timeGetTime() + LOCK_ENEMY;
						ActorManager::getSingleton().playerEnemy.insert(getOwner());
						break;
					}
				}
			}
		}

		if (pAttrib->m_nHideWeapon > 0)
		{
			pSrcObj->hideWeapon((unsigned int)pAttrib->m_nHideWeapon);
		}
		for( int i = 0 ; i < SkillManager::getSingleton().MAX_ARMTYPE ; i++ )
		{
			if( SkillManager::getSingleton().getSpellWeaponType(getSpellId(),getSpellLv(),i) == ARM_LIUDAN_GUN )
			{
				pSrcObj->m_Equip.ShowShoulder( true );
				break;
			}
		}

		// 设置法术僵直
		if( SkillDef.AttackActTime > 0 )
		{
			pSrcObj->setSkillTetanic( SkillDef.AttackActTime*100 );
		}

		if( !pAttrib->m_ActEffName.empty() )
		{
			m_pSpellEffName[0]	= getAttackEffectName(pAttrib->m_ActEffName.c_str());
			m_pSpellEffCount	= 1;
		}
		else
		{
			m_pSpellEffName[0] = NULL;
			m_pSpellEffCount	= 0;
		}
		// 怪物发言
		char szScript[256];
		GameActor* pDesObj = NULL;
		if( getTargetNum() > 0 )
		{
			pDesObj = ActorManager::getSingleton().FindActor( getTarget(0) );
		}
		sprintf( szScript, "SetMonsterSkillAttackSpeech(%d,%d,[===[%s]===],%d,%d)", pSrcObj->getResId(), pSrcObj->GetID(),pDesObj==NULL?"":pDesObj->getName(), SKILL_ATK, pAttrib->m_nSkillId );
		ActorManager::getSingleton().getScriptVM()->callString( szScript );

		// 普通攻击可以有三套攻击动作按顺序切换
		if( pAttrib->m_nMoveTime == -1 )
		{
			if (getSpellId() != 3223) // tag 暂时措施
			{
				if (curSpell == NULL || curSpell == this)
				{
					if (m_nPilotNum == 0 || SkillDef.JiqiChgFlag == 1)
					{
						static int GeneraActTime = 0;
						int nActTime = GeneraActTime++%3;
						if( pAttrib->m_ActId[nActTime] == 0 || pAttrib->m_ActId[nActTime] == -1 )
						{
							playAction( m_OwnerID, pAttrib->m_ActId[0]);
						}
						else
						{
							playAction( m_OwnerID, pAttrib->m_ActId[nActTime]);
						}
						pSrcObj->setCurSkillInfo( pSrcObj->getActState().getActionId(pAttrib->m_ActId[0]), m_pSpellEffName[0] );
					}else if(m_nPilotNum > 0){
						if( m_pSpellEffCount > 0 && m_pSpellEffName[0] != NULL && !pSrcObj->isMotionPlaying(m_pSpellEffName[0], 0) )
						{
							playAction( m_OwnerID, ACTION_ID_NONE );
						}
					}
				}
			}
		}

		if (SkillDef.PilotNum > 0)
		{
			pSrcObj->ChangeActState(GSACT_PREPARE);
		}
		onAttack(true);
		if (m_SkillState != STATE_PILOT && m_SkillState != STATE_END)
		{
			if (SkillDef.AttackActTime > 0)
			{
				SkillManager::getSingleton().spellStop(this, SkillDef.AttackActTime * 100 - 1);
			}
			else
			{
				onStop();
			}
		}
	}
}

void GameSpell::onAttack(const std::vector<int> &targets, bool passive/* = false*/)
{
	m_TargetVector.push_back(targets);
	onAttack(passive);
}

// 播放受击动作
void GameSpell::onHit()
{
	GameActor *pSrcObj		= ActorManager::getSingleton().FindActor(getOwner());
	const tagSkillDef& SkillDef = getSkillDef();
	if (SkillDef.AtkType == SKILL_ATK_TYPE_TARGET && pSrcObj == ActorManager::getSingleton().getMainPlayer())
	{
		SkillManager::getSingleton().sendHitMsg(this);
	}
	if (pSrcObj != NULL)
	{
		pSrcObj->cast(getSpellId(), getSpellLv());
	}

	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onHited(this);
	}
}


void GameSpell::onHit(int id, GameSkill_Package *package)
{
	int nHitAction = ACTION_HIT_HIT;
	if (package)
	{
		nHitAction = package->doPackage(id);
	}
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GameActor *pSrcObj = ActorManager::getSingleton().FindActor(getOwner());
	GameActor* pDesObj = ActorManager::getSingleton().FindActor(id);
	if (pDesObj == NULL)
	{
		return;
	}

	if (pDesObj == pMainPlayer && pMainPlayer->m_bUnaware)
	{
		pMainPlayer->m_bUnaware = false;
		if (pMainPlayer->GetSelectTarget() == 0)
		{
			if (pSrcObj && pSrcObj->getSubType() != MON_SUB_TYPE_COLLECT &&
				pSrcObj->getSubType() != MON_SUB_TYPE_TRAP &&
				(pSrcObj->isCamp(SKILL_TARGET_ENEMY_MON) || pSrcObj->isCamp(SKILL_TARGET_ENEMY_PLAYER)))
			{
				// 暂时屏蔽措手不及状态下自动选择目标
				//pMainPlayer->SetSelectTarget(getOwner());
			}
		}
	}

	tagSkillAttrib* pAttrib = (tagSkillAttrib *)getAttrib();
	if( !pAttrib->m_HitEffName.empty() )
	{
		m_pSpellEffName[0]	= getHitEffectName(id, pAttrib->m_HitEffName.c_str());
		m_pSpellEffCount	= 1;
	}
	else
	{
		m_pSpellEffName[0] = NULL;
		m_pSpellEffCount	= 0;
	}
	if (pDesObj == pMainPlayer && pSrcObj != NULL)
	{
		// 怪物发言
		char szScript[256];
		sprintf( szScript, "SetMonsterSkillAttackSpeech(%d,%d,[===[%s]===],%d,%d)", pSrcObj->getResId(), pSrcObj->GetID(),pDesObj==NULL?"":pDesObj->getName(), SKILL_HIT, pAttrib->m_nSkillId );
		ActorManager::getSingleton().getScriptVM()->callString( szScript );
	}
	if (nHitAction >= ACTION_HIT_HIT || nHitAction == ACTION_HIT_NONE)
	{
		if( pDesObj->getActState().GetState() != GSACT_STOP && 
			pDesObj->getActState().GetState() != GSACT_DEAD )
		{
			ISpellControler *sc = SkillManager::getSingleton().getSpellControler(id);
			if (sc && sc->getCurSpell() == NULL)
			{
				if (pDesObj->getBufStatus()->isInEndure())
				{
					pDesObj->getShowActor()->getCurEntity()->playMotion( m_pSpellEffName[0], false, getSpellId() );
				}
				else
				{
					playAction( pDesObj->GetID(), pAttrib->m_BehitId, 1 );
				}
			}
			else if( m_pSpellEffName[0] != NULL )
			{
				pDesObj->getShowActor()->getCurEntity()->playMotion( m_pSpellEffName[0], false, getSpellId() );
			}
		}
		else
		{
			if( m_pSpellEffName[0] != NULL )
			{
				pDesObj->getShowActor()->getCurEntity()->playMotion( m_pSpellEffName[0], false, getSpellId() );
			}
		}
	}
	else if (nHitAction == ACTION_HIT_INVALID)
	{
		if( m_pSpellEffName[0] != NULL )
		{
			pDesObj->getShowActor()->getCurEntity()->playMotion( m_pSpellEffName[0], false, getSpellId() );
		}
	}

	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onHited(this, id);
	}

	for (size_t i = 0; i < m_TargetVector.size(); ++i)
	{
		if (!m_TargetVector[i].empty() && id == m_TargetVector[i][0])
		{
			onHit();
		}
	}
}

void GameSpell::onBreak()
{
	if (m_SkillState == STATE_END)
	{
		return;
	}
	GameActor* pSrcActor	= ActorManager::getSingleton().FindActor( m_OwnerID );

	bool mustStop = false;
	if (m_SkillState != STATE_BREAK && pSrcActor == ActorManager::getSingleton().getMainPlayer())
	{
		mustStop = true;
	}

	if (pSrcActor != NULL)
	{
		pSrcActor->setPrepareSkill();
		pSrcActor->setPilotSkill();
		if( pSrcActor->getActState().GetState() == GSACT_ATTACK  ||
			pSrcActor->getActState().GetState() == GSACT_COLLECT ||
			pSrcActor->getActState().GetState() == GSACT_PREPARE )
		{
			if (!pSrcActor->isDead())
			{
				pSrcActor->ChangeActState( GSACT_STAND );
				if (!pSrcActor->m_pMapMover->isReach())
				{
					pSrcActor->ChangeActState( GSACT_MOVE );
				}
			}
		}
	}
	onStop();

	if (mustStop)
	{
		SkillManager::getSingleton().onStop();
	}
}

void GameSpell::onStop()
{
	m_SkillState = STATE_END;
	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onStop(this);
	}
	GameActor *pSrcObj = ActorManager::getSingleton().FindActor(getOwner());
	if (pSrcObj != NULL)
	{
		tagSkillAttrib* pAttrib = (tagSkillAttrib*)getAttrib();
		if (pAttrib->m_isPreHideWeapon)
		{
			pSrcObj->hideWeapon(false);
		}
		const tagSkillDef &SkillDef = getSkillDef();
		if (SkillDef.PrepareType != PREPARE_TYPE_NONE)
		{
			if (!pSrcObj->isDead() && pSrcObj->getActState().GetState() != GSACT_MOVE)
			{
				GameSpell *curSpell = SkillManager::getSingleton().getSpellControler(m_OwnerID)->getCurSpell();
				if (curSpell == NULL || curSpell == this)
				{
					pSrcObj->ChangeActState( GSACT_STAND );
				}
			}
		}
		for( int i = 0 ; i < SkillManager::getSingleton().MAX_ARMTYPE ; i++ )
		{
			if( SkillManager::getSingleton().getSpellWeaponType(getSpellId(),getSpellLv(),i) == ARM_LIUDAN_GUN )
			{
				pSrcObj->m_Equip.ShowShoulder( false );
				break;
			}
		}
	}
}

void GameSpell::onChannel()
{
	m_SkillState = STATE_PILOT;
	tagSkillAttrib* pAttrib = (tagSkillAttrib*)getAttrib();
	GameActor *pSrcObj		= ActorManager::getSingleton().FindActor(getOwner());
	if (pSrcObj != NULL)
	{
		if (pAttrib->m_isPreHideWeapon)
		{
			pSrcObj->hideWeapon(true);
		}
		for( int i = 0 ; i < SkillManager::getSingleton().MAX_ARMTYPE ; i++ )
		{
			if( SkillManager::getSingleton().getSpellWeaponType(getSpellId(),getSpellLv(),i) == ARM_LIUDAN_GUN )
			{
				pSrcObj->m_Equip.ShowShoulder( true );
				break;
			}
		}
	}
}

int GameSpell::inspect()
{
	MainGamePlayer *pActor = ActorManager::getSingleton().getMainPlayer();
	const tagSkillDef &SkillDef = getSkillDef();
	if (pActor == NULL)
	{
		return RET_UNKNOWN;
	}
	if (!pActor->canControl())
	{
		return RET_BUFSTOP;
	}
	if (pActor->getActState().GetState() == GSACT_DEAD)
	{
		return RET_SELFDEAD;
	}
	if (!SkillManager::getSingleton().castInMap(getSpellId()))
	{
		return RET_ENV;
	}
	if (!SkillManager::getSingleton().castInPworld(getSpellId()))
	{
		return RET_ENV;
	}
	if (SkillDef.JiqiChgFlag == 2 && pActor->isInSafeOverlay())
	{
		return RET_SAFEENV;
	}
	if(!SkillManager::getSingleton().castWeapon(getSpellId()))
	{
		return RET_WEAPONSHORT;
	}
	if (!m_selfCoolSet && !SkillManager::getSingleton().castSkillCD(getSpellId()))
	{
		return RET_CD;
	}
	if (!m_commonCoolSet && !SkillManager::getSingleton().castCommonCD(getSpellId()))
	{
		return RET_CD;
	}
	if (m_nPilotNum == 0)
	{
		if (!SkillManager::getSingleton().castMP(getSpellId()))
		{
			return RET_MPSHORT;
		}
		if (!SkillManager::getSingleton().castXP(getSpellId()))
		{
			return RET_XPSHORT;
		}
		if (!SkillManager::getSingleton().castCollect(getSpellId()))
		{
			return RET_COLLECTSHORT;
		}
		if (!SkillManager::getSingleton().castEnergy(getSpellId()))
		{
			return RET_ENERGYSHORT;
		}
		if (!SkillManager::getSingleton().castMachineEnergy(getSpellId()))
		{
			return RET_MACHINEENERGYSHORT;
		}
		if( !SkillManager::getSingleton().castItem(getSpellId()) )
		{
			return RET_NOITEM;
		}
	}
	if (pActor->isInFight())
	{
		if (SkillDef.NoFightStatUse == NO_FIGHT_USE_PREPARE)
		{
			if (SkillDef.PrepareType != PREPARE_TYPE_NONE)
			{
				return RET_INFIGHT;
			}
		}
		else if (SkillDef.NoFightStatUse == NO_FIGHT_USE_ATK)
		{
			if (SkillDef.PrepareType == PREPARE_TYPE_NONE || getState() != STATE_START)
			{
				return RET_INFIGHT;
			}
		}
	}
	else
	{
		if (SkillDef.NoFightStatUse == ENTER_FIGHT_USE_PREPARE)
		{
			if (SkillDef.PrepareType != PREPARE_TYPE_NONE)
			{
				return RET_OUTFIGHT;
			}
		}
		else if (SkillDef.NoFightStatUse == ENTER_FIGHT_USE_ATK)
		{
			if (SkillDef.PrepareType == PREPARE_TYPE_NONE || getState() != STATE_START)
			{
				return RET_OUTFIGHT;
			}
		}
	}
	if(!pActor->getBufStatus()->canUsedSpell(getSpellId()))
	{
		return RET_BUFSTOP;
	}
	if(pActor->getActState().GetState() == GSACT_AUTOFLY)
	{
		if(!SkillManager::getSingleton().isTmpSkill(getSpellId()))
		{
			return RET_BUFSTOP;
		}
	}
	if(!SkillManager::getSingleton().castMorph(getSpellId()))
	{
		return RET_BUFSTOP;
	}
	if (pActor->getActState().GetState() == GSACT_STALL) 
	{
		return RET_BUFSTOP;
	}
	const SKILLRESULT *pstSkillResult = z_skill_get_result(&SkillDef, RESULT_SELF_POSCHG);
	if (pstSkillResult && !pActor->getBufStatus()->canMove())
	{
		return RET_MOVESTOP;
	}
	/*if (pActor->getActState().GetState() == GSACT_COLLECTITEM)
	{
		return RET_DOOTHER;
	}*/
	if ( !SkillManager::getSingleton().isNoPrepareSkill(getSpellId()) &&
		( pActor->m_Container.isUseItemPre() ||
		pActor->m_Trump.isPre() ||
		pActor->m_Rider.isPre() ||
		pActor->m_Pet.isPre() ))
	{
		return RET_DOOTHER;
	}
	GameSpell *pSpell = m_pSpellControler->getCurSpell();
	if (pSpell && pSpell != this)
	{
		if (pSpell->getState() != STATE_PILOT)
		{
			return RET_SPELLPRE;
		}
		tagSkillAttrib *attrib = (tagSkillAttrib *)pSpell->getAttrib();
		if (attrib && attrib->m_isNoBreak)
		{
			return RET_TETANIC;
		}
	}
// 	if (pActor->isInSkillTetanic())
// 	{
// 		return RET_TETANIC;
// 	}
	return RET_SUCCESS;
}

int GameSpell::inspect(int id, int x, int y)
{
	int result = inspect();
	if (result == RET_SUCCESS)
	{
		result = inspect(x, y);
		if (result == RET_SUCCESS)
		{
			return inspect(id);
		}
	}
	return result;
}

const tagSkillDef & GameSpell::getSkillDef()
{
	return DBDataManager::getSingleton().GetSkillDef(getSpellId(), getSpellLv());
}

const tagSkillDyAttrib * GameSpell::getSkillAttrib()
{
	return SkillManager::getSingleton().getSkillDyAttrib(getSpellId());
}

unsigned int GameSpell::getPilotNum()
{
	return m_nPilotNum;
}

int GameSpell::check()
{
	const GameXYZ *pos = getTargetPoint();
	if (pos)
	{
		return inspect(getSingleTarget(), pos->x, pos->z);
	}
	else
	{
		int result = inspect();
		if (result == RET_SUCCESS)
		{
			return inspect(getSingleTarget());
		}
		return result;
	}
}

int GameSpell::getHitDelay()
{
	tagSkillAttrib *attrib = (tagSkillAttrib *)getAttrib();
	if (attrib == NULL)
	{
		return 0;
	}
	if (attrib->m_nMoveTime != -1)
	{
		return attrib->m_nBehitDelay + DEFAULT_DELAY;
	}
	return attrib->m_nBehitDelay;
}

void GameSpell::setCommonCD()
{
	if (m_commonCoolSet)
	{
		return;
	}
	m_commonCoolSet = true;
	SkillManager::getSingleton().setCommonCDTime(getSpellId());
}

void GameSpell::setSelfCD()
{
	if (m_selfCoolSet)
	{
		return;
	}
	m_selfCoolSet = true;
	SkillManager::getSingleton().setSkillCDTime(getSpellId());
}

void GameSpell::resetCD()
{
	if (m_selfCoolSet)
	{
		SkillManager::getSingleton().resetSkillCDTime(getSpellId());
		m_selfCoolSet = false;
	}
}

void GameSpell::sortTargets(std::vector<GameActor *> &targets)
{
	std::sort(targets.begin(), targets.end(), CenterPosComparer(*getTargetPoint()));
}

void GameSpell::HitBackPrepareTimeTo(unsigned int time)
{
	m_isHitBackPrepare = true;
	unsigned int now = timeGetTime();
	m_lastTime = now - time;
}

bool GameSpell::isHitBackPrepareTime()
{
	bool result = m_isHitBackPrepare;
	m_isHitBackPrepare = false;
	return result;
}

void GameSpell::onHitBackPrepareTime()
{
	if (m_lastTime == 0)
	{
		return;
	}
	unsigned int now = timeGetTime();
	unsigned int prepareTime = now - m_lastTime;
	const tagSkillDef &SkillDef = getSkillDef();
	switch (SkillDef.PrepareType)
	{
	case PREPARE_TYPE_SING:
		{
			unsigned int nTotalTime = SkillManager::getSingleton().getSkillPrepareTime(getSpellId(), getSpellLv()) * 100;
			if (prepareTime > nTotalTime)
			{
				prepareTime = nTotalTime;
			}
			m_pSpellControler->onHitBack(this, prepareTime,
				nTotalTime);
		}
		break;
	case PREPARE_TYPE_COLLECT:
		{
			unsigned int nPassOneGridTime  = SkillManager::getSingleton().getSkillCollectTime(getSpellId(), getSpellLv()) * 100;
			int nCollectNum = SkillManager::getSingleton().getSkillCollectNum(getSpellId(), getSpellLv());

			int nCollectGird = prepareTime / nPassOneGridTime;
			if (nCollectGird >= nCollectNum)
			{
				nCollectGird = nCollectNum - 1;
			}
			unsigned int nTotalTime = SkillManager::getSingleton().getSkillCollectTime(getSpellId(), getSpellLv()) * 100;
			prepareTime = prepareTime - nCollectGird * nPassOneGridTime;
			if (prepareTime > nTotalTime)
			{
				prepareTime = nTotalTime;
			}
			m_pSpellControler->onHitBack(this, prepareTime,
				nTotalTime,
				nCollectGird);
		}
		break;
	}
}

void GameSpell::setOwner(int id)
{
	m_OwnerID = id;
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (pActor)
	{
		setSelfPoint(pActor->GetPosition());
	}
}

void GameSpell::setSpellReq(int req)
{
	m_SkillReq = req;
}

int GameSpell::getSpellReq()
{
	return m_SkillReq;
}
