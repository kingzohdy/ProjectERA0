#include "GuideEffect.h"

GuideEffect::GuideEffect(EntityEffect *effect, GameMoverPtr &mover) : effectPtr(effect), moverPtr(mover)
{
	if (effectPtr && moverPtr)
	{
		effectPtr->setpos(moverPtr->getpos());
	}
}

void GuideEffect::destory()
{
	if (effectPtr)
	{
		effectPtr->destory();
		effectPtr = NULL;
	}
}

bool GuideEffect::update(unsigned int dtick)
{
	if (!effectPtr)
	{
		return false;
	}
	if (moverPtr && moverPtr->actived())
	{
		moverPtr->update();
		effectPtr->moveto(moverPtr->getpos());
	}
	return true;
}

unsigned int GuideEffect::cleanup()
{
	if (!effectPtr)
	{
		return 0;
	}
	return effectPtr->cleanup();
}

void GuideEffect::onStart()
{
	if (effectPtr)
	{
		effectPtr->onStart();
	}
}
