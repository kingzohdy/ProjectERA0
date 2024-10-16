#pragma once
#include "GameSkill_Common.h"

class SkillVigor : public GameSpell
{
public:
	virtual bool	start();
	virtual void	setTargetPoint(const GameXYZ &pos);
	virtual bool	collect();
	virtual void	endCollect();
	virtual void	onStop();
	virtual void	update(unsigned int dtick);

	virtual bool	Attack();
	virtual void	onAttack(bool passive = false);

	DECLARE_SKILL()
private:
	SkillVigor(void);
	~SkillVigor(void);

private:
	unsigned int		cumulateTime;
	Ogre::Entity*		m_pSelectEff;
};
