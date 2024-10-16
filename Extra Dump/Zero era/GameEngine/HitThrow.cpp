#define _USE_MATH_DEFINES
#include <math.h>
#include "HitThrow.h"
#include "GameMover_Base.h"
#include "GameActorManager.h"
#include "GameActor.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "GameSkill_Mgr.h"
#include "MoveLine.h"
#include "MoveLineFollow.h"
#include "MoveTerrenceFollow.h"
#include "MoveParabola.h"
#include "GameEffect_Mgr.h"
#include "GameMainPlayer.h"
#include <Windows.h>

HitThrow::HitThrow(GameSpell *p) :
	SkillHit(p)
{
}

HitThrow::~HitThrow(void)
{
	for (size_t i = 0; i < bullets.size(); ++i)
	{
		delete bullets[i];
	}
	for (size_t i = 0; i < entitys.size(); ++i)
	{
		entitys[i]->destory();
	}
}

void HitThrow::start(const char *path, unsigned int time, unsigned int delay)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(whichSpell->getOwner());
	if ( pActor == NULL || ( pActor != NULL && pActor->isDead() ) )
	{
		return;
	}

	unsigned int now = timeGetTime();

	for (unsigned int i = 0; i < time; ++i)
	{
		EntityEffect *effect = new EntityEffect(path, whichSpell->getOwner());
		waitEntitys.push_back(std::pair<EntityEffectPtr, unsigned int>(
			effect, now + i * delay));
		effect->setAngle(pActor->getAngle());
	}
}

void HitThrow::update(unsigned int dtick)
{
	unsigned int now = timeGetTime();
	if (waitEntitys.empty() && bullets.empty())
	{
		stop();
	}
	else
	{
		while (!waitEntitys.empty())
		{
			if (waitEntitys.front().second > now)
			{
				break;
			}
			IGameMover *mover = createMover();
			if (mover)
			{
				waitEntitys.front().first->start();
				waitEntitys.front().first->setpos(mover->getpos());
				entitys.push_back(waitEntitys.front().first);
				bullets.push_back(mover);
			}
			else
			{
				waitEntitys.front().first->destory();
			}
			waitEntitys.pop_front();
		}
		std::vector<IGameMover *>::iterator i = bullets.begin();
		std::vector<EntityEffectPtr>::iterator j = entitys.begin();
		for (; i != bullets.end() && j != entitys.end();)
		{
			if ((*i)->actived())
			{
				(*i)->update();
				const GameXYZ &pos = (*i)->getpos();
				MainGamePlayer *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if (true && pMainPlayer->getCurMap()->getMaskMap()->canWalkPixelCoord(pos.x / 10, pos.z / 10, GameMaskMap::maskMagic))
				{
					(*j)->moveto(pos);
					++i;
					++j;
				}
				else
				{
					delete (*i);
					EffectManage::getSingleton().cleanup(*j);
					i = bullets.erase(i);
					j = entitys.erase(j);
				}
			}
			else
			{
				delete (*i);
				EffectManage::getSingleton().cleanup(*j);
				i = bullets.erase(i);
				j = entitys.erase(j);

				hit();
				if (waitEntitys.empty() && bullets.empty())
				{
					stop();
				}
			}
		}
	}
}

IGameMover * HitThrow::createMover()
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(whichSpell->getOwner());
	if (pActor == NULL)
	{
		return NULL;
	}
	if (pActor->isOnMonsterMachine())
	{
		GameActor *pActor_ = ActorManager::getSingleton().FindActor(pActor->m_MonsterMachine.m_playerInfo.m_nMonsterMemID);
		if (pActor_ != NULL)
		{
			pActor = pActor_;
		}
	}
	const tagSkillAttrib *attrib = (tagSkillAttrib *)whichSpell->getAttrib();
	GameXYZ startPos = pActor->getShowActor()->getCurEntity()->getAnchorWorldPos(attrib->m_nFlySrcAnchorId);
	if (attrib->m_strThrowtype == "2")
	{
		GameActor *target = ActorManager::getSingleton().FindActor(first());
		if (target)
		{
			GameXYZ endPos = target->getShowActor()->getCurEntity()->getAnchorWorldPos(attrib->m_nFlyDesAnchorId);
			return new MoveLineFollow(startPos, target->GetID(), attrib->m_nFlyDesAnchorId, unsigned int((startPos - endPos).toVector3().length() / attrib->m_flySpeed * 1000));
		}
	}
	else if (attrib->m_strThrowtype == "5")
	{
		GameActor *target = ActorManager::getSingleton().FindActor(first());
		if (target)
		{
			GameXYZ endPos = target->getShowActor()->getCurEntity()->getAnchorWorldPos(attrib->m_nFlyDesAnchorId);
			return new MoveTerrenceFollow(startPos, target->GetID(), unsigned int((startPos - endPos).toVector3().length() / attrib->m_flySpeed * 1000));
		}
	}
	else if (attrib->m_strThrowtype == "3")
	{
		GameXYZ endPos = *(whichSpell->getTargetPoint());
		float length = (startPos - endPos).toVector3().length();
		return new MoveParabola(startPos, endPos, int(length * attrib->m_nFlyHeightRadio * 10), unsigned int(length / attrib->m_flySpeed * 1000));
	}
	else
	{
		GameXYZ endPos = *(whichSpell->getTargetPoint());
		return new MoveLine(startPos, endPos, unsigned int((startPos - endPos).toVector3().length() / attrib->m_flySpeed * 1000));
	}

	return NULL;
}
