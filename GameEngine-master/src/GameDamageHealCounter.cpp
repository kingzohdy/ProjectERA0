#include "GameDamageHealCounter.h"
#include <math.h>
#include "GameActorManager.h"
#include "GameActor.h"
#include <Windows.h>
#include "OgreScriptLuaVM.h"


GameDamageHealCounter & GameDamageHealCounter::getSingleton()
{
	static GameDamageHealCounter instance;
	return instance;
}

GameActor * GameDamageHealCounter::getNextPlayer(GameActor *pActor)
{
	PLAYERLIST::iterator i;
	if (pActor == NULL)
	{
		i = players.begin();
	}
	else
	{
		i = players.find(getPlayerIndex(pActor));
		if (i == players.end())
		{
			return NULL;
		}
		else
		{
			++i;
		}
	}
	GameActor *ret = NULL;
	while (ret == NULL && i != players.end())
	{
		ret = ActorManager::getSingleton().FindActor(indexToId[i->first]);
		++i;
	}
	return ret;
}

const GameDamageHealInfo * GameDamageHealCounter::getPlayerInfo(GameActor *pActor)
{
	assert(pActor);
	if (!pActor)
	{
		return NULL;
	}
	int index = getPlayerIndex(pActor);
	PLAYERLIST::iterator i = players.find(index);
	if (i == players.end())
	{
		return NULL;
	}
	TIMELIST::iterator t = playersTime.find(index);
	if (t != playersTime.end())
	{
		unsigned int now = timeGetTime();
		i->second.time += (now - t->second) / 1000.0;
		t->second = now;
	}

	return &(i->second);
}

GameDamageHealCounter::GameDamageHealCounter()
{
	playerIndexMax = 0;
	filter = NULL;
}

void GameDamageHealCounter::reset()
{
	players.clear();
	unsigned int now = timeGetTime();
	for (TIMELIST::iterator i = playersTime.begin(); i != playersTime.end(); ++i)
	{
		i->second = now;
	}
	for (std::map<std::string, int>::iterator i = playerIndex.begin(); i != playerIndex.end(); )
	{
		if (playersTime.find(i->second) == playersTime.end())
		{
			indexToId.erase(indexToId.find(i->second));
			playerIndexIdle.push_back(i->second);
			i = playerIndex.erase(i);
		}
		else
		{
			++i;
		}
	}
}

GameDamageHealInfo & GameDamageHealCounter::insertOrGetPlayerInfo(GameActor *pActor)
{
	int index = getPlayerIndex(pActor);
	PLAYERLIST::iterator i = players.find(index);
	if (i == players.end())
	{
		GameDamageHealInfo & info = players[index];
		info.damage = info.heal = info.hurt = 0;
		info.time = 0;

		i = players.find(index);
	}

	return i->second;
}

void GameDamageHealCounter::IncDamage(int id, int damage)
{
	if (damage <= 0)
	{
		return;
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (!pActor)
	{
		return;
	}
	if (pActor->getOwnerId() != 0)
	{
		id = pActor->getOwnerId();
	}
	pActor = ActorManager::getSingleton().FindActor(id);
	if (!pActor || (pActor->getType() != GAT_MAINPLAYER && pActor->getType() != GAT_PLAYER))
	{
		return;
	}
	if (!Filter(pActor->getName()))
	{
		return;
	}
	GameDamageHealInfo &info = insertOrGetPlayerInfo(pActor);
	damage += info.damage;
	if (damage > info.damage)
	{
		info.damage = damage;
	}
	else
	{
		info.damage = INT_MAX;
	}
}

void GameDamageHealCounter::IncHeal(int id, int heal)
{
	if (heal <= 0)
	{
		return;
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (!pActor)
	{
		return;
	}
	if (pActor->getOwnerId() != 0)
	{
		id = pActor->getOwnerId();
	}
	pActor = ActorManager::getSingleton().FindActor(id);
	if (!pActor || (pActor->getType() != GAT_MAINPLAYER && pActor->getType() != GAT_PLAYER))
	{
		return;
	}
	if (!Filter(pActor->getName()))
	{
		return;
	}
	GameDamageHealInfo &info = insertOrGetPlayerInfo(pActor);
	heal += info.heal;
	if (heal > info.heal)
	{
		info.heal = heal;
	}
	else
	{
		info.heal = INT_MAX;
	}
}

void GameDamageHealCounter::InFight(int id)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (!pActor || (pActor->getType() != GAT_MAINPLAYER && pActor->getType() != GAT_PLAYER))
	{
		return;
	}
	if (!Filter(pActor->getName()))
	{
		return;
	}
	int index = getPlayerIndex(pActor);
	TIMELIST::iterator i = playersTime.find(index);
	if (i == playersTime.end())
	{
		playersTime[index] = timeGetTime();
	}
}

void GameDamageHealCounter::OutFight(int id)
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (!pActor || (pActor->getType() != GAT_MAINPLAYER && pActor->getType() != GAT_PLAYER))
	{
		return;
	}
	if (!Filter(pActor->getName()))
	{
		return;
	}
	int index = getPlayerIndex(pActor);
	TIMELIST::iterator i = playersTime.find(index);
	if (i != playersTime.end())
	{
		insertOrGetPlayerInfo(pActor).time += (timeGetTime() - i->second) / 1000.0;
		playersTime.erase(i);
	}
}

GameActor * GameDamageHealCounter::getNextBoss(GameActor *pActor)
{
	BOSSLIST::iterator i;
	if (pActor == NULL)
	{
		i = bosses.begin();
	}
	else
	{
		i = bosses.find(pActor->GetID());
		if (i == bosses.end())
		{
			return NULL;
		}
		else
		{
			++i;
		}
	}
	GameActor *ret = NULL;
	while (i != bosses.end())
	{
		ret = ActorManager::getSingleton().FindActor(i->first);
		if (ret == NULL)
		{
			i = bosses.erase(i);
		}
		else
		{
			break;
		}
	}
	return ret;
}

void GameDamageHealCounter::InsertBoss(int bossId, int targetId)
{
	bosses[bossId].targetId = targetId;
}

const GameBossInfo * GameDamageHealCounter::getBossInfo(GameActor *pActor)
{
	assert(pActor);
	if (!pActor)
	{
		return NULL;
	}
	BOSSLIST::iterator i = bosses.find(pActor->GetID());
	if (i == bosses.end())
	{
		return NULL;
	}
	return &(i->second);
}

void GameDamageHealCounter::IncHurt(int id, int hurt)
{
	if (hurt <= 0)
	{
		return;
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(id);
	if (!pActor)
	{
		return;
	}
	if (pActor->getOwnerId() != 0)
	{
		id = pActor->getOwnerId();
	}
	pActor = ActorManager::getSingleton().FindActor(id);
	if (!pActor || (pActor->getType() != GAT_MAINPLAYER && pActor->getType() != GAT_PLAYER))
	{
		return;
	}
	if (!Filter(pActor->getName()))
	{
		return;
	}
	GameDamageHealInfo &info = insertOrGetPlayerInfo(pActor);
	hurt += info.hurt;
	if (hurt > info.hurt)
	{
		info.hurt = hurt;
	}
	else
	{
		info.hurt = INT_MAX;
	}
}

int GameDamageHealCounter::getPlayerIndex(GameActor *pActor)
{
	if (pActor == NULL)
	{
		return -1;
	}
	std::string name(pActor->getName());
	INDEXLIST::iterator i = playerIndex.find(name);
	if (i == playerIndex.end())
	{
		if (playerIndexIdle.empty())
		{
			playerIndex[name] = playerIndexMax++;
		}
		else
		{
			playerIndex[name] = playerIndexIdle.back();
			playerIndexIdle.pop_back();
		}
		i = playerIndex.find(name);
	}
	indexToId[i->second] = pActor->GetID();
	return i->second;
}

void GameDamageHealCounter::SetFilter(const char *func)
{
	filter = func;
}

bool GameDamageHealCounter::Filter(const char *name)
{
	if (filter)
	{
		bool result;
		if (ActorManager::getSingleton().getScriptVM()->callFunction(filter, "s>b", name, &result))
		{
			return result;
		}
		return false;
	}
	return true;
}
