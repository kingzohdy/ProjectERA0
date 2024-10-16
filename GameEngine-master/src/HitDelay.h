#pragma once
#include "GameSkill_Hit.h"
#include "GameTickAction.h"

class HitDelay;

class HitDelayAction : public ITickActionDelay
{
private:
	HitDelay	*parent;

public:
	HitDelayAction(HitDelay *hit, unsigned int delay);
	virtual unsigned int expire();
};

class HitDelay :
	public SkillHit
{
	friend HitDelayAction;

private:
	HitDelayAction		*delayAction;

public:
	HitDelay(GameSpell *p);
	void	setDelay(unsigned int delay);
	~HitDelay(void);
};
