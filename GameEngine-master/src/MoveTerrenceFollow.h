#pragma once

#include "GameMover_Base.h"
#include "GameMaskMap.h"

class GameMap;

class MoveTerrenceFollow :
	public IGameMover
{
protected:
	unsigned int	lastTime;
	unsigned int	cost;
	unsigned int	during;
	Ogre::Vector3	cursor;
	int				who;
	float			speed;
	int				height;
	GameMaskPath	path;
	bool			cross;

	GameMap			*map;

public:
	MoveTerrenceFollow(const GameXYZ &start, int actorId, unsigned int time);
	virtual void update();
};