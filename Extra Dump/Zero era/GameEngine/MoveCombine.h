#pragma once

#include "GameMover_Base.h"
#include <list>

class MoveCombine :
	public IGameMover
{
	typedef std::pair<GameMoverPtr, GameXYZ>	MoverPos;
	typedef std::list<MoverPos>					MoverPosList;
protected:
	GameXYZ					cursor;
	MoverPosList			combineList;

public:
	MoveCombine(const GameXYZ &start);
	void	addMover(IGameMover *mover);
	virtual void update();
};