#pragma once
#include "GameSkill_Common.h"

class SkillGenera : public GameSpell
{
public:
	virtual bool start();

	virtual int  getSingleTarget();

	virtual const GameXYZ *	getTargetPoint();

	DECLARE_SKILL()
private:
	SkillGenera(void);
	~SkillGenera(void);
};
