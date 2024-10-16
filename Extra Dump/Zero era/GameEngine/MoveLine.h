#pragma once

#include "GameMover_Base.h"

class MoveLine :
	public IGameMover
{
protected:
	unsigned int	lastTime;
	unsigned int	cost;
	unsigned int	during;
	Ogre::Vector3	target;
	Ogre::Vector3	cursor;

public:
	MoveLine(const GameXYZ &start, const GameXYZ &end, unsigned int time);
	virtual void update();
};