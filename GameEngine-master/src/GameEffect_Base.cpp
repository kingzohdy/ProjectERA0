#include "GameEffect_Base.h"
#include "GameEffect_Mgr.h"

IGameEffect::IGameEffect() : starting(false)
{
	EffectManage::getSingleton().lstEffect.push_back(this);
}

IGameEffect::~IGameEffect()
{
}

void IGameEffect::start()
{
	starting = true;
	onStart();
}

bool IGameEffect::isStarting()
{
	return starting;
}

unsigned int IGameEffect::cleanup()
{
	return 0;
}

bool IGameEffect::update(unsigned int dtick)
{
	return true;
}

void IGameEffect::onStart()
{
}
