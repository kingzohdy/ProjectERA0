#pragma once
#include "GameSkill_Common.h"

class SkillSelectAttack : public GameSpell
{
public:
	virtual bool start();
	virtual bool Attack();

	virtual void setTargetPoint(const GameXYZ &pos);

	virtual int inspect(int x, int y);
	virtual void onAttack(bool passive = false);

	virtual const GameXYZ *	getTargetPoint();

	DECLARE_SKILL()
private:
	SkillSelectAttack(void);
	~SkillSelectAttack(void);

private:
	unsigned int	m_uStartThrow;
	int				m_nCatchTime;
	int				m_nHitDelay;
	bool			m_bSelected;
	bool			m_bSelecting;
};
