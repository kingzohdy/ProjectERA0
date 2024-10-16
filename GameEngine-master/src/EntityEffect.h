#pragma once

#include "GameEffect_Base.h"
#include <string>

class EntityEffect :
	public IGameEffect
{
public:
	enum
	{
		STEP_READY, STEP_IN, STEP_LIVE, STEP_OUT,
	};

private:
	Ogre::Entity		*entity;
	unsigned int		in_delay;
	unsigned int		out_delay;
	unsigned int		live_delay;
	unsigned int		keytime;
	int					keyframe;
	std::string			filepath;
	int					actionId;
	unsigned int		actionTime;

public:
	EntityEffect(const char *path, int actorId = 0, unsigned int inTime = 0, unsigned int outTime = 0, unsigned int liveTime = 0);
	virtual ~EntityEffect();
	virtual void destory();
	virtual bool update(unsigned int dtick);
	virtual unsigned int cleanup();

	void	setpos(const GameXYZ &pos);
	void	moveto(const GameXYZ &pos);
	void	faceto(const GameXYZ &pos);
	void	rotation(float yaw, float pitch, float roll);
	void	setAngle(int angle);
	void	setScale(float scale);
	void	playAction(int actId);
	bool	setTransparent(float alpha);

private:
	void	playActionReal();
};

typedef SmartPtr<EntityEffect>		EntityEffectPtr;
