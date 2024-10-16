#pragma once
#include "GameSkill_Common.h"

class SkillSelectArrow : public GameSpell
{
public:
	virtual bool start();

	virtual void setTargetPoint(const GameXYZ &pos);

	virtual float getAttackDist();

	virtual void onAttack(bool passive = false);

	virtual const GameXYZ *	getTargetPoint();

	DECLARE_SKILL()

private:
	void circualAttackPoint( RESPOS& pos, SKILLRESULT* pstSkillResult, GameActor* pSrc, GameXYZ& desPos, bool bFind=false );
private:
	SkillSelectArrow(void);
	~SkillSelectArrow(void);

private:
	bool		m_bSelected;
	bool		m_bSelecting;
};
