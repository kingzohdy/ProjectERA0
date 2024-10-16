#include "GameEffect_Mgr.h"
#include "EntityEffect.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameMap.h"
#include "ShowActor.h"
#include <algorithm>
#include <Windows.h>

static const int LINK_MOTION_CLASS = 9541;

EffectManage::EffectManage()
{

}

EffectManage& EffectManage::getSingleton()
{
	static EffectManage singleton;

	return singleton;
}

void EffectManage::update(unsigned int dtick)
{
	unsigned int now = timeGetTime();

	while (!displayEffects.empty())
	{
		if (displayEffects.front().first > now)
		{
			break;
		}
		GameEffectPtr effect = displayEffects.front().second;
		std::pop_heap(displayEffects.begin(), displayEffects.end(), PairFirstGreater());
		displayEffects.pop_back();
		effect->start();
	}

	while (!destoryEffects.empty())
	{
		if (destoryEffects.front().first > now)
		{
			break;
		}
		GameEffectPtr effect = destoryEffects.front().second;
		std::pop_heap(destoryEffects.begin(), destoryEffects.end(), PairFirstGreater());
		destoryEffects.pop_back();
		effect->destory();
	}

	for (std::list<GameEffectPtr>::iterator i = lstEffect.begin();
		i != lstEffect.end();)
	{
		if ((*i)->isStarting())
		{
			if (!(*i)->update(dtick))
			{
				(*i)->destory();
				i = lstEffect.erase(i);
				continue;
			}
		}
		++i;
	}

	for (std::map<int, LINK_INFO>::iterator i = linkInfos.begin();
		i != linkInfos.end();)
	{
		GameActor *startActor = ActorManager::getSingleton().FindActor(i->first);
		if (startActor == NULL)
		{
			i = linkInfos.erase(i);
			continue;
		}
		GameActor *endActor = ActorManager::getSingleton().FindActor(i->second.target);
		if (endActor == NULL)
		{
			++i;
			continue;
		}
		Ogre::Vector3 pos = endActor->getShowActor()->getCurEntity()->getAnchorWorldPos(i->second.bindb);
		startActor->getShowActor()->getCurEntity()->playFlashChain(i->second.path.c_str(), pos, LINK_MOTION_CLASS, i->second.binda);
		++i;
	}
}

void EffectManage::reset()
{
	for (size_t i = 0, j = destoryEffects.size();
		i < j; ++i)
	{
		destoryEffects[i].second->destory();
	}
	destoryEffects.clear();
	for (std::list<GameEffectPtr>::iterator i = lstEffect.begin();
		i != lstEffect.end(); i = lstEffect.erase(i))
	{
		(*i)->destory();
	}
}

void EffectManage::destory(IGameEffect *effect)
{
	destory(effect, 60000);
}

void EffectManage::destory(IGameEffect *effect, unsigned int delay)
{
	destoryEffects.push_back(std::pair<unsigned int, GameEffectPtr>(timeGetTime() + delay, effect));
	std::push_heap(destoryEffects.begin(), destoryEffects.end(), PairFirstGreater());
}

EntityEffect * EffectManage::playEffect(int x, int y, int angle, const char *path, unsigned int delay/* = 0*/)
{
	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer == NULL || pMainPlayer->getCurMap() == NULL )
	{
		return NULL;
	}
	EntityEffect *effect = new EntityEffect(path, pMainPlayer->GetID(), 0, 0, delay);
	if (delay != 0)
	{
		EffectManage::getSingleton().destory(effect, delay);
	}	
	int h;
	x *= 10;
	y *= 10;
	pMainPlayer->getCurMap()->getTerrainH(x, y, h);
	effect->start();
	effect->setpos(GameXYZ(x, h, y));
	effect->setAngle(angle);
	return effect;
}

void EffectManage::cleanup(IGameEffect *effect)
{
	destory(effect, effect->cleanup());
}

void EffectManage::display(IGameEffect *effect, unsigned int delay)
{
	displayEffects.push_back(std::pair<unsigned int, GameEffectPtr>(timeGetTime() + delay, effect));
	std::push_heap(displayEffects.begin(), displayEffects.end(), PairFirstGreater());
}

void EffectManage::display(IGameEffect *effect)
{
	display(effect, 0);
}

void EffectManage::addLink(int startId, int startBindId, int endId, int endBindId, const std::string &path)
{
	LINK_INFO &info = linkInfos[startId];
	info.target = endId;
	info.binda = startBindId;
	info.bindb = endBindId;
	info.path = path;
}

void EffectManage::delLink(int startId, int endId)
{
	std::map<int, LINK_INFO>::iterator i = linkInfos.find(startId);
	if (i != linkInfos.end())
	{
		if (i->second.target == endId)
		{
			linkInfos.erase(i);
			GameActor *startActor = ActorManager::getSingleton().FindActor(startId);
			if (startActor)
			{
				startActor->getShowActor()->getCurEntity()->stopMotion(LINK_MOTION_CLASS);
			}
		}
	}
}

void EffectManage::clearLink(int startId)
{
	std::map<int, LINK_INFO>::iterator i = linkInfos.find(startId);
	if (i != linkInfos.end())
	{
		linkInfos.erase(i);
		GameActor *startActor = ActorManager::getSingleton().FindActor(startId);
		if (startActor)
		{
			startActor->getShowActor()->getCurEntity()->stopMotion(LINK_MOTION_CLASS);
		}
	}
}
