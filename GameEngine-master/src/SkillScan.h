#pragma once
#include "GameSkill_Common.h"

class GameSkill_Package;

class SkillScan : public GameSpell
{
public:
	virtual bool start();

	virtual int  getSingleTarget();

	virtual void onPrepare();

	virtual void onUpdate(unsigned int dtick);

	virtual void setSkillMsgPackage( GameSkill_Package* Package );

	virtual const GameXYZ *	getTargetPoint();

	virtual void setTargetPoint(const GameXYZ &pos);

	DECLARE_SKILL()
private:
	double m_angle;
	unsigned int m_lastAttack;
	unsigned int m_lastTime;
	bool m_start;
	void	checkAttack();
private:
	SkillScan(void);
	~SkillScan(void);
};
