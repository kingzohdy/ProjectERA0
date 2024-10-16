#pragma once
#include "GameSkill_Hit.h"
#include "EntityEffect.h"

class IGameMover;
class SkillHit;

class HitThrow :
	public SkillHit
{
private:
	std::vector<EntityEffectPtr>							entitys;
	std::list<std::pair<EntityEffectPtr, unsigned int>>		waitEntitys;
	std::vector<IGameMover *>								bullets;

protected:
	IGameMover *	createMover();

public:
	HitThrow(GameSpell *p);
	virtual ~HitThrow(void);
	void			start(const char *path, unsigned int time, unsigned int delay);
	virtual void	update(unsigned int dtick);
};
