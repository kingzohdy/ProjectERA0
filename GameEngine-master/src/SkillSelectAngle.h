#pragma once
#include "GameSkill_Common.h"

class SkillSelectAngle : public GameSpell
{
public:
	virtual bool start();

	virtual void setTargetPoint(const GameXYZ &pos);

	virtual bool Attack();
	virtual void onBreak();
	virtual void onStop();
	virtual void onChannel();
	virtual void onAttack(bool passive = false);
	virtual bool collect();

	virtual const GameXYZ *	getTargetPoint();

	DECLARE_SKILL()

private:
	SkillSelectAngle(void);
	~SkillSelectAngle(void);

private:
	bool		m_bSelected;
	bool		m_bSelecting;
};
