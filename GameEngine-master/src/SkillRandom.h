#pragma once
#include "GameSkill_Common.h"

class SkillRandom : public GameSpell
{
public:
	virtual bool start();

	virtual int	inspect(int id);

	virtual void attackTime();
	void  selectNextTarget();

	virtual void setSingleTarget(int targetid);
	virtual int  getSingleTarget();
	virtual const GameXYZ *	getTargetPoint();
	virtual void onAttack(bool passive = false);

	DECLARE_SKILL()
private:
	int			m_TargetActorID;	// 选中的单个目标
	SkillRandom(void);
};
