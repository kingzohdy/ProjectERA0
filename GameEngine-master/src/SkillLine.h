#pragma once
#include "GameSkill_Common.h"

class SkillLine : public GameSpell
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
	virtual void setTargetPoint(const GameXYZ &pos);
	virtual void onAttack(bool passive = false);
	virtual void attackTime();

	DECLARE_SKILL()
private:
	int			m_TargetActorID;	// 选中的单个目标
	bool		m_isAttack;
	SkillLine(void);
};
