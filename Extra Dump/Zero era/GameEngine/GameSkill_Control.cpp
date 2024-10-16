#include "GameSkill_Control.h"
#include "GameSkill_Mgr.h"
#include "GameDBData.h"
#include "OgreLog.h"
#include "ServerTimer.h"
#include "proto_cs.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameMsgPrint.h"
#include "GameStateAction.h"
#include "GameSkill_Package.h"
#include <Windows.h>
#include <MMSystem.h>
#include "GameActorManager.h"
#include "OgreScriptLuaVM.h"
#include "GameMap.h"
#include "ShowActor.h"
#include "OgreResourceManager.h"

using namespace std;

const int	LockMoveDelay = 200;

static int calLockMoveState(GameActor *pActor, int skillId, int lastActId, int oldx, int oldy, int newx, int newy)
{
	tagSkillAttrib *attrib = (tagSkillAttrib *)(SkillManager::getSingleton().getSpellSample(skillId)->getAttrib());
	int nActId = 0;
	if (oldx == newx && oldy == newy)
	{
		nActId = pActor->getActState().getActionId(attrib->m_ActId[0]);
	}
	else
	{
		int angle = ((FaceAngleBetween(oldx, oldy, newx, newy) - pActor->getAngle()) % 360 + 360) % 360;

		lastActId = lastActId % 1000;
		switch (lastActId)
		{
		case 522:
			if (angle < 60 || angle > 300)
			{
				nActId = pActor->getActState().getActionId(522);
			}
			break;

		case 523:
			if (angle > 120 && angle < 240)
			{
				nActId = pActor->getActState().getActionId(523);
			}
			break;

		case 521:
			if (angle <= 150 && angle >= 30)
			{
				nActId = pActor->getActState().getActionId(521);
			}
			break;

		case 520:
			if (angle >= 210 && angle <= 330)
			{
				nActId = pActor->getActState().getActionId(520);
			}
			break;
		}
		if (nActId == 0)
		{
			if (angle < 45 || angle > 315)
			{
				nActId = pActor->getActState().getActionId(522);
			}
			else if (angle > 135 && angle < 225)
			{
				nActId = pActor->getActState().getActionId(523);
			}
			else if (angle < 180)
			{
				nActId = pActor->getActState().getActionId(521);
			}
			else
			{
				nActId = pActor->getActState().getActionId(520);
			}
		}
	}
	if (pActor->getShowActor()->getCurAction() != nActId)
	{
		pActor->getShowActor()->stopCurAction();
		const char* szFootString[4];
		{
			char buf[256];
			char szPath[256];
			sprintf(buf, "%s_%d", attrib->m_ActEffName.c_str(), pActor->m_Equip.getRightHandEquipType());
			sprintf(szPath, "effect//public//%s.emo", buf);
			int res = Ogre::ResourceManager::getSingleton().getFileAttrib(szPath);
			if( res != 0 )
			{
				szFootString[0] = buf;
			}
			else
			{
				szFootString[0] = attrib->m_ActEffName.c_str();
			}
		}
		pActor->getShowActor()->playAction(nActId, szFootString, 1, 0);
	}
	return nActId;
}

GameSpellControler::GameSpellControler() :
	m_curIdx(0), m_pCurSpell(NULL), m_lockSkillId(0), m_lastTime(0)
{

}

GameSpellControler::~GameSpellControler()
{

}

void GameSpellControler::onPrepare(GameSpell* pSpell)
{
	m_pCurSpell = pSpell;
	pSpell->onPrepare();
	SkillManager::getSingleton().getSpellUIControl()->setCollect(
		SkillManager::getSingleton().getSkillPrepareTime(pSpell->getSpellId(), pSpell->getSpellLv()) / 10.0f);
}

void GameSpellControler::onCollect(GameSpell* pSpell, unsigned int time /*= 0*/)
{
	if (time == 0)
	{
		m_pCurSpell = pSpell;
		pSpell->onCollect();

		int target = pSpell->getSingleTarget();
		if (target && target != ActorManager::getSingleton().getMainPlayer()->GetID())
		{
			ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_OBJ, target, pSpell->getSpellId());
		}
		else
		{
			ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_DIR,
				ActorManager::getSingleton().getMainPlayer()->getAngle(), pSpell->getSpellId());
		}
	}
	else
	{
		pSpell->onCollectTime(time);
	}
	SkillManager::getSingleton().getSpellUIControl()->setCollect(
		SkillManager::getSingleton().getSkillCollectTime(pSpell->getSpellId(), pSpell->getSpellLv()) / 10.0f, 0, time + 1);
}

void GameSpellControler::onCollectFull(GameSpell* pSpell)
{
	pSpell->onCollectFull();
	//GameMsgPrint::getSingleton().SendUIEvent("GE_SKILL_POWERCOLLECT_END");
}

void GameSpellControler::onAttack(GameSpell* pSpell, bool passive)
{
	std::set<GameSpell *>::iterator i = m_lstForceAttack.find(pSpell);
	if (i != m_lstForceAttack.end())
	{
		m_lstForceAttack.erase(i);
		pSpell->onAttack(passive);
	}
}

void GameSpellControler::onBreak(int skillreq, int oldSkillreq, unsigned int LeftCool)
{
	GameSpell *pSpell = getSpell(0, 0, oldSkillreq);
	if (pSpell != NULL)
	{
		onBreak(pSpell);
		SkillManager::getSingleton().setSkillCDTime(pSpell->getSpellId(), LeftCool);
	}
	SkillManager::getSingleton().setSkillReq(skillreq);
}

void GameSpellControler::onBreak(GameSpell *pSpell)
{
	if (pSpell == NULL)
	{
		return;
	}
	std::set<GameSpell *>::iterator i = m_lstForceAttack.find(pSpell);
	if (i != m_lstForceAttack.end())
	{
		m_lstForceAttack.erase(i);
	}
	pSpell->onBreak();
	SkillManager::getSingleton().setNextSpell();
	GameMsgPrint::getSingleton().SendUIEvent("GE_SKILL_POWERCOLLECT_END");
	GameMsgPrint::getSingleton().SendUIEvent("GE_SKILL_CHANNEL_END");
	MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if (pMainPlayer)
	{
		pMainPlayer->sendLockFace(ATTACK_LOCK_NONE, 0, pSpell->getSpellId());
	}
	if (m_pCurSpell == pSpell)
	{
		m_pCurSpell = NULL;
	}
}

void GameSpellControler::onPilot(GameSpell* pSpell)
{
	m_pCurSpell = pSpell;
	pSpell->onChannel();
	tagSkillAttrib *attrib = (tagSkillAttrib *)pSpell->getAttrib();
	if (attrib && !attrib->m_isNoBreak)
	{
		SkillManager::getSingleton().getSpellUIControl()->setChannel(
			SkillManager::getSingleton().getSkillPilotTime(pSpell->getSpellId(), pSpell->getSpellLv()) *
			(SkillManager::getSingleton().getSkillPilotNum(pSpell->getSpellId(), pSpell->getSpellLv()) - 1) / 10.0f + pSpell->getHitDelay() / 1000.0f);
	}

	int target = pSpell->getSingleTarget();
	if (target && target != ActorManager::getSingleton().getMainPlayer()->GetID())
	{
		ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_OBJ, target, pSpell->getSpellId());
	}
	else
	{
		ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_DIR,
			ActorManager::getSingleton().getMainPlayer()->getAngle(), pSpell->getSpellId());
	}
}

void GameSpellControler::onComplete(GameSpell* pSpell)
{
	if (pSpell->getSpellId() != 3223)// tag ÔÝÊ±´ëÊ©
	{
		ActorManager::getSingleton().getMainPlayer()->sendLockFace(ATTACK_LOCK_NONE, 0, pSpell->getSpellId());
	}
	if (m_pCurSpell == pSpell)
	{
		GameMsgPrint::getSingleton().SendUIEvent("GE_SKILL_POWERCOLLECT_END");
		m_pCurSpell = NULL;
	}
}

GameSpell* GameSpellControler::getCurSpell()
{
	return m_pCurSpell;
}

GameSpell* GameSpellControler::getSpell(int skillId, int skillLv, int skillReq)
{
	GameSpell *ret = NULL;
	map<int, GameSpellPtr>::iterator i;
	if (skillReq != 0)
	{
		i = m_lstSpell[m_curIdx].find(skillReq);
		if (i != m_lstSpell[m_curIdx].end())
		{
			ret = i->second;
		}
		else
		{
			i = m_lstSpell[1 - m_curIdx].find(skillReq);
			if (i != m_lstSpell[1 - m_curIdx].end())
			{
				ret = i->second;
			}
		}
	}
	if (ret == NULL || ret->getOwner() != ActorManager::getSingleton().getMainPlayer()->GetID())
	{
		if (skillId)
		{
			ret = SkillManager::getSingleton().createSpell(skillId, this);
			ret->setSpellLv(skillLv);
			addSpell(ret, skillReq);
			m_lstForceAttack.insert(ret);
		}
	}
	else if (skillReq == 0)
	{
		m_lstForceAttack.insert(ret);
	}
	return ret;
}

void GameSpellControler::update(unsigned int dtick)
{
	if (m_pCurSpell != NULL)
	{
		if (m_pCurSpell->getState() == GameSpell::STATE_END)
		{
			m_pCurSpell = NULL;
		}
		else
		{
			m_pCurSpell->update(dtick);
		}
	}
	unsigned int now = timeGetTime();
	if (m_lastTime + LockMoveDelay <= now)
	{
		m_lastTime = now;
		if (m_lockSkillId && !getCurSpell())
		{
			GameActor *pActor = getOwner();
			if (pActor)
			{
				GameXYZ pos = pActor->GetPosition();
				m_lastActId = calLockMoveState(pActor, m_lockSkillId, m_lastActId, m_lastPos.X, m_lastPos.Y, pos.x, pos.z);
				m_lastPos.X = pos.x;
				m_lastPos.Y = pos.z;
			}
		}
	}
}

GameActor * GameSpellControler::getOwner()
{
	return ActorManager::getSingleton().getMainPlayer();
}

void GameSpellControler::breakSpell(GameSpell *pSpell)
{
	if (pSpell == NULL)
	{
		return;
	}
	pSpell->Break();
}

void GameSpellControler::swapSpellList()
{
	size_t otherIdx = 1 - m_curIdx;
	m_lstSpell[otherIdx].clear();
	m_curIdx = otherIdx;
}

std::map<int, GameSpellPtr> & GameSpellControler::getSpellList()
{
	static unsigned int lastSwapTime = 0;
	unsigned int now = timeGetTime();
	if (lastSwapTime + SPELL_LIFE_MAX < now)
	{
		lastSwapTime = now;
		swapSpellList();
	}
	return m_lstSpell[m_curIdx];
}

GameSpellControler & GameSpellControler::getSingleton()
{
	static GameSpellControler t;
	return t;
}

void GameSpellControler::addSpell(GameSpell *pSpell, int skillReq)
{
	pSpell->setSpellReq(skillReq);
	getSpellList()[skillReq] = pSpell;
}

void GameSpellControler::onLock(int id /*= 0*/)
{
	m_lockSkillId = id;
	if (id)
	{
		m_lastActId = 0;
		GameActor *pActor = getOwner();
		if (pActor)
		{
			GameXYZ pos = pActor->GetPosition();
			m_lastPos.X = pos.x;
			m_lastPos.Y = pos.z;
		}
	}
}

void GameSpellControler::onHitBack(GameSpell *pSpell, int during, int totalTime, int times /*= 0*/)
{
	SkillManager::getSingleton().getSpellUIControl()->setCollect(totalTime / 1000.0f, during / 1000.0f, times + 1);
}

int GameSpellControler::getOwnerId()
{
	GameActor *pActor = getOwner();
	if (pActor == NULL)
	{
		return 0;
	}
	return pActor->GetID();
}

void GameSpellControler::clear()
{
	for (size_t i = 0; i < sizeof(m_lstSpell) / sizeof(m_lstSpell[0]); ++i)
	{
		m_lstSpell[i].clear();
	}
	m_lstForceAttack.clear();
	m_pCurSpell = NULL;
	m_lockSkillId = 0;
}

//////////////////////////////////////////////////////////////////////////

ActorSpellControler::ActorSpellControler(int actorId) :
	m_ownerId(actorId), m_curSpellId(0), m_curIdx(0), m_lockSkillId(0), m_lastTime(0)
{

}

ActorSpellControler::~ActorSpellControler()
{
}

void ActorSpellControler::onPrepare(GameSpell* pSpell)
{
	m_curSpellId = pSpell->getSpellId();
	pSpell->onPrepare();
}

void ActorSpellControler::onCollect(GameSpell* pSpell, unsigned int time /*= 0*/)
{
	if (time == 0)
	{
		m_curSpellId = pSpell->getSpellId();
		pSpell->onCollect();
	}
	else
	{
		pSpell->onCollectTime(time);
	}
}

void ActorSpellControler::onCollectFull(GameSpell* pSpell)
{
	pSpell->onCollectFull();
}

void ActorSpellControler::onAttack(GameSpell* pSpell, bool passive)
{
	const tagSkillDef &SkillDef	= pSpell->getSkillDef();
	pSpell->onAttack(passive);
	if (pSpell->getSkillDef().PilotNum > 0)
	{
		m_curSpellId = pSpell->getSpellId();
	}
}

void ActorSpellControler::onBreak(int skillreq, int oldSkillreq, unsigned int LeftCool)
{
	onBreak(getCurSpell());	
}

void ActorSpellControler::onBreak(GameSpell *pSpell)
{
	if (pSpell != NULL)
	{
		pSpell->onBreak();
		GameActor *pActor = getOwner();
		if (pActor && pActor->getType() != GAT_PLAYER)
		{
			pActor->lockFace(ATTACK_LOCK_NONE, 0, pSpell->getSpellId());
		}
		if (pSpell->getSpellId() == m_curSpellId)
		{
			m_curSpellId = 0;
		}
	}
}

void ActorSpellControler::onPilot(GameSpell* pSpell)
{
	m_curSpellId = pSpell->getSpellId();
	pSpell->onChannel();
	GameActor *pActor = getOwner();
	if (pActor && pActor->getType() != GAT_PLAYER)
	{
		int target = pSpell->getSingleTarget();
		if (target && target != pActor->GetID())
		{
			pActor->lockFace(ATTACK_LOCK_OBJ, target, pSpell->getSpellId());
		}
		else
		{
			pActor->lockFace(ATTACK_LOCK_DIR, pActor->getAngle(), pSpell->getSpellId());
		}
	}
}

void ActorSpellControler::onComplete(GameSpell* pSpell)
{
	if (pSpell->getSpellId() == m_curSpellId)
	{
		m_curSpellId = 0;
	}
}

GameActor * ActorSpellControler::getOwner()
{
	return ActorManager::getSingleton().FindActor(m_ownerId);
}

GameSpell* ActorSpellControler::getCurSpell()
{
	if (m_curSpellId == 0)
		return NULL;

	return getSpell(m_curSpellId, 0, 0);
}

GameSpell* ActorSpellControler::getSpell(int skillId, int skillLv, int skillReq)
{
	if (skillId == 0)
	{
		return NULL;
	}
	std::map<int, GameSpellPtr>::iterator i = m_lstSpell.find(skillId);
	if (i == m_lstSpell.end())
	{
		m_lstSpell[skillId] = SkillManager::getSingleton().createSpell(skillId, this);
		i = m_lstSpell.find(skillId);
	}
	else if (i->second->getState() == GameSpell::STATE_END || i->second->getState() == GameSpell::STATE_ATTACK)
	{
		delSpell(i->second);
		m_lstSpell[skillId] = SkillManager::getSingleton().createSpell(skillId, this);
		i = m_lstSpell.find(skillId);
	}
	if (skillLv)
	{
		i->second->setSpellLv(skillLv);
	}
	return i->second;
}

void ActorSpellControler::update(unsigned int dtick)
{
	if (m_curSpellId != 0)
	{
		std::map<int, GameSpellPtr>::iterator i = m_lstSpell.find(m_curSpellId);
		if (i != m_lstSpell.end())
		{
			i->second->onUpdate(dtick);
		}
	}
	unsigned int now = timeGetTime();
	if (m_lastTime + LockMoveDelay <= now)
	{
		m_lastTime = now;
		if (m_lockSkillId && !getCurSpell())
		{
			GameActor *pActor = getOwner();
			if (pActor)
			{
				GameXYZ pos = pActor->GetPosition();
				m_lastActId = calLockMoveState(pActor, m_lockSkillId, m_lastActId, m_lastPos.X, m_lastPos.Y, pos.x, pos.z);
				m_lastPos.X = pos.x;
				m_lastPos.Y = pos.z;
			}
		}
	}
}

void ActorSpellControler::swapDelSpellList()
{
	size_t otherIdx = 1 - m_curIdx;
	m_delSpell[otherIdx].clear();
	m_curIdx = otherIdx;
}

void ActorSpellControler::delSpell(GameSpell *pSpell)
{
	static unsigned int lastSwapTime = 0;
	unsigned int now = timeGetTime();
	if (lastSwapTime + SPELL_LIFE_MAX < now)
	{
		lastSwapTime = now;
		swapDelSpellList();
	}
	m_delSpell[m_curIdx].insert(pSpell);
}

void ActorSpellControler::onLock(int id /*= 0*/)
{
	m_lockSkillId = id;
	if (id)
	{
		m_lastActId = 0;
		GameActor *pActor = getOwner();
		if (pActor)
		{
			GameXYZ pos = pActor->GetPosition();
			m_lastPos.X = pos.x;
			m_lastPos.Y = pos.z;
		}
	}
}

void ActorSpellControler::onHitBack(GameSpell *pSpell, int during, int totalTime, int times)
{

}

int ActorSpellControler::getOwnerId()
{
	return m_ownerId;
}
