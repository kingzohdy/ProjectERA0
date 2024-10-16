#pragma once
#include "GameSkill_Common.h"

class SkillAttackP2P : public GameSpell
{
private:
	virtual int inspecttarget(int id);

public:
	virtual bool start();

	virtual void update(unsigned int dtick); 

	virtual int	inspect(int id);

	virtual void setSingleTarget(int targetid);
	virtual int  getSingleTarget();
	virtual const GameXYZ *	getTargetPoint();
	virtual void onAttack(bool passive = false);
	virtual void onUpdate(unsigned int dtick);

	DECLARE_SKILL()
private:
	int			m_TargetActorID;	// 选中的单个目标
	SkillAttackP2P(void);
};
