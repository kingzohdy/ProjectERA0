#include "GameTickAction.h"
#include <time.h>
#include <algorithm>

using namespace std;

GameTickActionManager::GameTickActionManager(void)
{
}

GameTickActionManager::~GameTickActionManager(void)
{
}


GameTickActionManager & GameTickActionManager::getSingleton()
{
	static GameTickActionManager singleton;
	return singleton;
}

void GameTickActionManager::update()
{
	clock_t now = clock();
	while (timeArray.size() > 0)
	{
		if (timeArray.front().first > now)
		{
			break;
		}
		ITickActionDelay *action = timeArray.front().second;
		std::pop_heap(timeArray.begin(), timeArray.end(), TimeComp());
		timeArray.pop_back();
		if (action)
		{
			action->isReg = false;
			unsigned int next = action->expire();
			if (next > 0)
			{
				registerTick(action, next);
			}
		}
	}
	for (std::list<ITickActionCondition *>::iterator i = testArray.begin(); i != testArray.end();)
	{
		if ((*i)->until())
		{
			if (!(*i)->work())
			{
				(*i)->isReg = false;
				i = testArray.erase(i);
				continue;
			}
		}
		++i;
	}
}

void GameTickActionManager::registerTick(ITickActionDelay *ticker, unsigned int time)
{
	if (isReg(ticker))
	{
		return;
	}
	ticker->isReg = true;
	ticker->lastDelay = time;
	timeArray.push_back(pair<clock_t, ITickActionDelay *>(clock() + clock_t(time * CLOCKS_PER_SEC / 1000), ticker));
	std::push_heap(timeArray.begin(), timeArray.end(), TimeComp());
}

void GameTickActionManager::registerTick(ITickActionCondition *ticker)
{
	if (isReg(ticker))
	{
		return;
	}
	ticker->isReg = true;
	testArray.push_back(ticker);
}

void GameTickActionManager::unregisterTick(ITickActionDelay *ticker)
{
	if (!isReg(ticker))
	{
		return;
	}
	ticker->isReg = false;
	for (std::vector<std::pair<clock_t, ITickActionDelay *>>::iterator i = timeArray.begin();
		i != timeArray.end(); ++i)
	{
		if (i->second == ticker)
		{
			i->second = NULL;
			break;
		}
	}
}

void GameTickActionManager::unregisterTick(ITickActionCondition *ticker)
{
	if (!isReg(ticker))
	{
		return;
	}
	ticker->isReg = false;
	for (std::list<ITickActionCondition *>::iterator i = testArray.begin();
		i != testArray.end(); ++i)
	{
		if (*i == ticker)
		{
			testArray.erase(i);
			break;
		}
	}
}

bool GameTickActionManager::isReg(const RegTag *tag)
{
	return tag->isReg;
}

ITickActionDelay::ITickActionDelay(unsigned int time)
{
	GameTickActionManager::getSingleton().registerTick(this, time);
}

ITickActionDelay::~ITickActionDelay()
{
	GameTickActionManager::getSingleton().unregisterTick(this);
}

ITickActionCondition::ITickActionCondition()
{
	GameTickActionManager::getSingleton().registerTick(this);
}

ITickActionCondition::~ITickActionCondition()
{
	GameTickActionManager::getSingleton().unregisterTick(this);
}
