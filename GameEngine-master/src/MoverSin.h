#pragma once

#include "GameMover_Base.h"

class MoveSin :
	public IGameMover
{
protected:
	unsigned int	lastTime;
	unsigned int	cost;
	unsigned int	during;
	float			angle;
	float			dgree;
	float			period;
	GameXYZ			cursor;
	unsigned int	swing;
	float			phaseoffset;
	float			swingoffset;

public:
	MoveSin(const GameXYZ &start, unsigned int radius, unsigned int time, int hway, int vway = 0, unsigned int loop = 1, float initPhase = 0);
	virtual void update();
};