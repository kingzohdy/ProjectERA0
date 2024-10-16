#pragma once

#include "GameMover_Base.h"
#include "OgreVector2.h"
#include "GameMaskMap.h"

class GameMap;

class MoveTerrence :
	public IGameMover
{
protected:
	unsigned int	lastTime;
	unsigned int	cost;
	unsigned int	during;
	Ogre::Vector2	target;
	Ogre::Vector2	cursor;
	float			speed;
	size_t			seg;
	GameMaskPath	path;
	int				height;

	float			offset;

	GameMap			*map;

public:
	MoveTerrence(const GameXYZ &start, int x, int y, unsigned int time);
	virtual void update();
};