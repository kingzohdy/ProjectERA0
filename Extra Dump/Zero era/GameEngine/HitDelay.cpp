#include "HitDelay.h"

HitDelayAction::HitDelayAction(HitDelay *hit, unsigned int delay) : ITickActionDelay(delay), parent(hit)
{
}

unsigned int HitDelayAction::expire()
{
	parent->hit();
	parent->stop();
	return halt();
}

HitDelay::HitDelay(GameSpell *p) : SkillHit(p), delayAction(NULL)
{
}

HitDelay::~HitDelay(void)
{
	delete delayAction;
}

void HitDelay::setDelay(unsigned int delay)
{
	delete delayAction;
	if (delay == 0)
	{
		delayAction = NULL;
		hit();
		stop();
	}
	else
	{
		delayAction = new HitDelayAction(this, delay);
	}
}
