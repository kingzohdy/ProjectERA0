#pragma once
#include "OgreMemoryDelegation.h"
#include "SmartPtr.h"
#include "GameEngineTypes.h"
#include <string>

class IGameEffect : public AutoRelease<IGameEffect>, public Ogre::MemoryDelegation
{
	friend class GuideEffect;
	friend class AutoRelease<IGameEffect>;
private:
	bool		starting;

private:
	virtual void onStart();

public:
	IGameEffect();

protected:
	virtual ~IGameEffect();

public:
	virtual void destory() = 0;
	virtual bool update(unsigned int dtick);
	virtual unsigned int cleanup();

public:
	void start();
	bool isStarting();
};

typedef SmartPtr<IGameEffect>		GameEffectPtr;
