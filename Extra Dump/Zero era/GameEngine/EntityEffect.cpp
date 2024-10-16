#define _USE_MATH_DEFINES
#include <math.h>
#include "EntityEffect.h"
#include "OgreEntity.h"
#include "ShowGameMap.h"
#include "GameMap.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include <Windows.h>
#include "OgreRoot.h"
#include "OgreResourceManager.h"

const int	MAX_ENTITYEFFECT = 50;

typedef std::map<string, unsigned int>	EntityCount;

static EntityCount	entityCounter;

EntityEffect::EntityEffect(const char *path, int actorId /*= 0*/, unsigned int inTime /*= 0*/, unsigned int outTime /*= 0*/, unsigned int liveTime /*= 0*/)
: in_delay(inTime), out_delay(outTime), live_delay(liveTime), keyframe(STEP_READY), entity(NULL), filepath(path ? path : ""), actionId(0), actionTime(0)
{
	EntityCount::iterator i = entityCounter.find(filepath);
	if (i == entityCounter.end())
	{
		entityCounter[filepath] = 0;
		i = entityCounter.find(filepath);
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(actorId);
	if (++i->second > MAX_ENTITYEFFECT)
	{
		if (pActor == NULL || pActor->getType() == GAT_PLAYER)
		{
			return;
		}
	}
	entity = new Ogre::Entity;
	if (pActor && pActor->getType() != GAT_PLAYER)
	{
		entity->setPostSceneRenderer(ActorManager::getSingleton().getPostSceneRenderer());
	}

	entity->load(filepath.c_str(), false);
	entity->update(0);
}

EntityEffect::~EntityEffect()
{
	destory();
	--entityCounter[filepath];
}

void EntityEffect::destory()
{
	if (entity)
	{
		entity->detachFromScene();
		entity->release();
		entity = NULL;
	}
}

bool EntityEffect::update(unsigned int dtick)
{
	if (entity == NULL)
	{
		return false;
	}
	playActionReal();
	if (STEP_READY != keyframe)
	{
		unsigned int now = timeGetTime();
		if (STEP_IN == keyframe && keytime + in_delay <= now)
		{
			keyframe = STEP_LIVE;
			entity->setTransparent(1.0f);
		}
		if (STEP_IN == keyframe)
		{
			entity->setTransparent((now - keytime) / (float)in_delay);
		}
		else if (STEP_OUT == keyframe)
		{
			entity->setTransparent(1.0f - (now - keytime) / (float)out_delay);
		}
		if (live_delay + in_delay + out_delay > (now - keytime) || entity->IsPlaying())
		{
			entity->update(dtick);
		}
		else
		{
			destory();
		}
	}
	return true;
}

void EntityEffect::moveto(const GameXYZ &pos)
{
	if (entity == NULL)
	{
		return;
	}
	Ogre::Vector3 diff = pos.toVector3() - entity->getPosition().toVector3();
	setpos(pos);
	if (diff.length() == 0)
	{
		return;
	}
	float yaw = atan2(-diff.z, diff.x) * 180.0f / float(M_PI);
	float pitch = atan2(diff.y, sqrt(diff.x * diff.x + diff.z * diff.z)) * 180.0f / float(M_PI);
	entity->setRotation(yaw - 90, pitch, 0);
}

void EntityEffect::faceto(const GameXYZ &pos)
{
	if (entity == NULL)
	{
		return;
	}
	Ogre::Vector3 diff = pos.toVector3() - entity->getPosition().toVector3();
	if (diff.length() == 0)
	{
		return;
	}
	float yaw = atan2(-diff.z, diff.x) * 180.0f / float(M_PI);
	float pitch = atan2(diff.y, sqrt(diff.x * diff.x + diff.z * diff.z)) * 180.0f / float(M_PI);
	entity->setRotation(yaw - 90, pitch, 0);
}

void EntityEffect::setpos(const GameXYZ &pos)
{
	if (entity == NULL)
	{
		return;
	}
	entity->setPosition(pos);
	if (STEP_READY == keyframe)
	{
		GameMap *map = ActorManager::getSingleton().getMainPlayer()->getCurMap();
		if (map == NULL)
		{
			return;
		}
		Ogre::ShowGameMap *showMap = map->getShowGameMap();
		if (showMap == NULL)
		{
			return;
		}
		Ogre::GameScene *gameScene = showMap->getGameScene();
		if (gameScene == NULL)
		{
			return;
		}
		entity->attachToScene(gameScene);
		if (0 == in_delay)
		{
			keyframe = STEP_LIVE;
		}
		else
		{
			keyframe = STEP_IN;
		}
		keytime = timeGetTime();
	}
}

void EntityEffect::rotation(float yaw, float pitch, float roll)
{
	if (entity == NULL)
	{
		return;
	}
	entity->setRotation(yaw, pitch, roll);
}

void EntityEffect::setAngle(int angle)
{
	rotation(angle - 90.0f, 0, 0);
}

void EntityEffect::setScale(float scale)
{
	if (entity == NULL)
	{
		return;
	}
	entity->setScale(scale);
}

unsigned int EntityEffect::cleanup()
{
	if (STEP_READY == keyframe)
	{
		return 0;
	}
	unsigned int now = timeGetTime();
	if (STEP_OUT == keyframe)
	{
		int time = out_delay - (now - keytime);
		if (time <= 0)
		{
			return 0;
		}
		return time;
	}
	if (STEP_IN == keyframe)
	{
		out_delay = (unsigned int)(out_delay * ((now - keytime) / (double)in_delay));
	}
	keytime = now;
	keyframe = STEP_OUT;
	return out_delay;
}

void EntityEffect::playAction(int actId)
{
	actionTime = timeGetTime();
	actionId = actId;
	playActionReal();
}

void EntityEffect::playActionReal()
{
	if (entity == NULL)
	{
		return;
	}
	if (actionId == 0)
	{
		return;
	}
	entity->playAnim(actionId);
	entity->update(timeGetTime() - actionTime);
	actionId = 0;
	actionTime = 0;
}

bool EntityEffect::setTransparent(float alpha)
{
	if (entity == NULL)
	{
		return false;
	}
	entity->setTransparent(alpha);
	return true;
}
