#pragma once
#include "OgreMemoryDelegation.h"
#include <vector>
#include <list>
#include <time.h>

class ITickActionDelay;
class ITickActionCondition;

class GameTickActionManager
{
	friend ITickActionDelay;
	friend ITickActionCondition;

protected:
	GameTickActionManager(void);
public:
	~GameTickActionManager(void);
	void	update();
	static GameTickActionManager &		getSingleton();

	class RegTag
	{
		friend GameTickActionManager;
	public:
		RegTag() : isReg(false) {}
		virtual ~RegTag() {}
	private:
		bool	isReg;
	};

	bool	isReg(const RegTag *tag);

private:
	std::vector<std::pair<clock_t, ITickActionDelay *>>		timeArray;
	std::list<ITickActionCondition *>						testArray;

	void registerTick(ITickActionDelay *ticker, unsigned int time);
	void registerTick(ITickActionCondition *ticker);
	void unregisterTick(ITickActionDelay *ticker);
	void unregisterTick(ITickActionCondition *ticker);

	class TimeComp
	{
	public:
		bool operator () (const std::pair<clock_t, ITickActionDelay *> &a, const std::pair<clock_t, ITickActionDelay *> &b)
		{
			return a.first > b.first;
		}
	};
};

class ITickActionDelay : public GameTickActionManager::RegTag, public Ogre::MemoryDelegation
{
	friend GameTickActionManager;

private:
	unsigned int	lastDelay;

protected:
	unsigned int	halt() { return 0; }
	unsigned int	repeat() { return lastDelay; }

public:
	ITickActionDelay(unsigned int time);
	virtual ~ITickActionDelay();
	virtual unsigned int expire() = 0;
};

class ITickActionCondition : public GameTickActionManager::RegTag, public Ogre::MemoryDelegation
{
	friend GameTickActionManager;

public:
	ITickActionCondition();
	virtual ~ITickActionCondition();
	virtual bool	until() = 0;
	virtual bool	work() = 0;
};
