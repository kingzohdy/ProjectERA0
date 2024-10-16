#pragma once

#include "GameMover_Base.h"

class MoveLineFollow :
	public IGameMover
{
protected:
	unsigned int	lastTime;
	unsigned int	cost;
	unsigned int	during;
	Ogre::Vector3	cursor;
	int				who;
	int				bindpoint;
	float			speed;

public:
	MoveLineFollow(const GameXYZ &start, int actorId, int bindId, unsigned int time);
	virtual void update();
};