#pragma once

#include "EntityEffect.h"
#include "GameMover_Base.h"

class GuideEffect :
	public IGameEffect
{
private:
	EntityEffectPtr	effectPtr;
	GameMoverPtr	moverPtr;

private:
	virtual void onStart();

public:
	GuideEffect(EntityEffect *effect, GameMoverPtr &mover);

	virtual void destory();
	virtual bool update(unsigned int dtick);
	virtual unsigned int cleanup();
};

typedef SmartPtr<GuideEffect>		GuideEffectPtr;
