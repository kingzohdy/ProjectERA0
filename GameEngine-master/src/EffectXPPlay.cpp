#define _USE_MATH_DEFINES
#include <math.h>
#include "EffectXPPlay.h"
#include "GameMainPlayer.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "GameEffect_Mgr.h"
#include <Windows.h>

const int MAXHEIGHT = 2000;
const double SPEED_PRE_SECOND = 1000;

EffectXPMover::EffectXPMover(const GameXYZ &start, int actorId, int bindId)
	: IGameMover(start), m_startPos(start), m_state(UP), m_actorId(actorId), m_bindId(bindId), m_costTime(0)
{
	map = ActorManager::getSingleton().getMainPlayer()->getCurMap();
	if (map == NULL)
	{
		reach();
		return;
	}
	m_startRad = (float)(abs(rand() / float(RAND_MAX)) * M_PI * 2);
	m_startSpeed = (float)(abs(rand() / float(RAND_MAX)) * 1.5 + 1.5);
	m_upTick = (abs(rand()) % 401) + 800;
	m_downTick = 800;
	m_lastTime = timeGetTime();
}

void EffectXPMover::update()
{
	unsigned int now = timeGetTime();
	unsigned int dtick = now - m_lastTime;
	if (dtick == 0)
	{
		return;
	}
	m_lastTime = now;
	m_costTime += dtick;

	if (map != ActorManager::getSingleton().getMainPlayer()->getCurMap())
	{
		reach();
		return;
	}
	GameActor *actor = ActorManager::getSingleton().FindActor(m_actorId);
	if (actor == NULL)
	{
		reach();
		return;
	}
	GameXYZ pos = getpos();
	switch (m_state)
	{
	case UP:
		{
			pos = m_startPos;
			if (m_costTime > m_upTick)
			{
				m_costTime = 0;
				m_state = DOWN;
				pos.x += (int)(cos(m_startRad) * m_upTick * m_startSpeed);
				pos.z += (int)(sin(m_startRad) * m_upTick * m_startSpeed);
				pos.y += MAXHEIGHT;
			}
			else
			{
				pos.x += (int)(cos(m_startRad) * m_costTime * m_startSpeed);
				pos.z += (int)(sin(m_startRad) * m_costTime * m_startSpeed);
				pos.y += (int)(MAXHEIGHT - (m_upTick - m_costTime) * (m_upTick - m_costTime) * double(MAXHEIGHT) / (m_upTick * m_upTick));
			}
		}
		break;

	case DOWN:
		{
			if (m_costTime > m_downTick)
			{
				m_costTime = 0;
				m_state = FOLLOW;
			}
			else
			{
				pos.x += (int)(dtick * (m_startSpeed - m_costTime * m_startSpeed / m_downTick) * cos(m_startRad));
				pos.z += (int)(dtick * (m_startSpeed - m_costTime * m_startSpeed / m_downTick) * sin(m_startRad));
				pos.y -= (int)(MAXHEIGHT * (m_costTime - m_costTime * m_costTime / 2.0 / m_downTick) / m_downTick * dtick / 1000);
			}
		}
		break;

	case FOLLOW:
		{
			GameXYZ targetPos = actor->getShowActor()->getCurEntity()->getAnchorWorldPos(m_bindId);
			float distance = float(dtick * (SPEED_PRE_SECOND / 1000));
			if (Ogre::WorldPosDiff(targetPos, pos).length() < distance)
			{
				reach();
			}
			Ogre::Vector3 dir = (targetPos - pos).normalize();
			dir *= distance;
			pos += dir;
		}
		break;
	}
	int h;
	map->getTerrainH(pos.x, pos.z, h);
	if (pos.y < h)
	{
		pos.y = h;
	}
	setpos(pos);
}

EffectXPEntity::EffectXPEntity(const GameXYZ &pos, int targetId, int value /*= 0*/)
	: EntityEffect("effect\\entity\\9563_16.ent", targetId, 0, 1000), actorId(targetId), nValue(value)
{
	pMover = new EffectXPMover(pos, targetId, 107);
	start();
	setpos(pos);
}

EffectXPEntity::~EffectXPEntity()
{
	delete pMover;
	if (nValue != 0)
	{
		GameActor *pActor = ActorManager::getSingleton().FindActor(actorId);
		if (pActor)
		{
			pActor->setXP(Ogre::Min(pActor->getXP() + nValue, pActor->getMaxXP()));
			nValue = 0;
		}
	}
}

bool EffectXPEntity::update(unsigned int dtick)
{
	if (pMover && pMover->actived())
	{
		pMover->update();
		moveto(pMover->getpos());
		return EntityEffect::update(dtick);
	}
	if (nValue != 0)
	{
		GameActor *pActor = ActorManager::getSingleton().FindActor(actorId);
		if (pActor)
		{
			pActor->setXP(Ogre::Min(pActor->getXP() + nValue, pActor->getMaxXP()));
			nValue = 0;
		}
	}
	EffectManage::getSingleton().cleanup(this);
	return false;
}

EffectXPPlay::EffectXPPlay(GameActor *source, GameActor *target, int value)
: m_source(source), m_targetId(target ? target->GetID() : 0), m_value(value)
{
}

EffectXPPlay::~EffectXPPlay()
{

}

void EffectXPPlay::doAction(GameActor *actor)
{
	static int border[] = {1000, 400, 200, 100, 0};
	static int min[] = {31, 21, 16, 11, 1};
	static int max[] = {40, 30, 20, 15, 10};

	GameXYZ pos = actor->GetPosition();
	for (int i = 0; i < sizeof(border) / sizeof(int); ++i)
	{
		if (m_value >= border[i])
		{
			int count = min[i];
			if (i == 0)
			{
				count += (max[i] - min[i]) * (m_value - border[i]) / (m_value - border[i] + 99);
			}
			else
			{
				count += (max[i] - min[i]) * (m_value - border[i]) / (border[i - 1] - border[i]);
			}
			int preXP = (int)ceil(float(m_value) / count);
			GameActor *pTarget = ActorManager::getSingleton().FindActor(m_targetId);
			if (pTarget != NULL)
			{
				while (count-- > 0)
				{
					if (m_value <= 0)
					{
						break;
					}

					if (m_value - preXP > 0)
					{
						new EffectXPEntity(pos, m_targetId, preXP);
						m_value -= preXP;
					}
					else
					{
						new EffectXPEntity(pos, m_targetId, m_value);
						m_value = 0;
					}
				}
			}

			break;
		}
	}
}
