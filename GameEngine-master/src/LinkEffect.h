#pragma once

#include "GameEffect_Base.h"

class LinkEffect :
	public IGameEffect
{
private:
	std::string		effect_path;
	int				actor_id;
	int				bind_id;
	int				motion_class;
	GameXYZ			target_pos;
	bool			living;

public:
	LinkEffect(const char *path, int actorId, int bindId, const GameXYZ &endPos, int motionClass);
	virtual ~LinkEffect();
	virtual void destory();
	virtual bool update(unsigned int dtick);

	void	setTargetPos(const GameXYZ &endPos);
};

typedef SmartPtr<LinkEffect>		LinkEffectPtr;

class LinkFollowEffect :
	public LinkEffect
{
private:
	int		target_id;
	int		target_bindId;

public:
	LinkFollowEffect(const char *path, int actorId, int bindId, int targetId, int targetBindId, int motionClass);
	virtual bool update(unsigned int dtick);
};

typedef SmartPtr<LinkFollowEffect>		LinkFollowEffectPtr;
