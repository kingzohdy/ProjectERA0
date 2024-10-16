#pragma once
#include "GameSkill_Common.h"

class SkillTrig : public GameSpell
{
public:
	virtual int  getSingleTarget();

	virtual bool isAutoSkill();

	virtual const GameXYZ *	getTargetPoint();

	virtual void setTargetPoint(const GameXYZ &pos);

	DECLARE_SKILL()
private:
	SkillTrig(void);
	~SkillTrig(void);
};
