#pragma once

#include "GameMover_Base.h"

class MoveParabola :
	public IGameMover
{
private:
	double			parama, paramb;
	int				startHeight;
protected:
	unsigned int	lastTime;
	unsigned int	cost;
	unsigned int	during;
	Ogre::Vector3	target;
	Ogre::Vector3	cursor;

public:
	MoveParabola(const GameXYZ &start, const GameXYZ &end, int height, unsigned int time);
	virtual void update();
};
