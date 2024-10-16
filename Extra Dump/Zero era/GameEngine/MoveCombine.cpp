#include <Windows.h>
#include "MoveCombine.h"

MoveCombine::MoveCombine(const GameXYZ &start) :
	IGameMover(start), cursor(start)
{

}

void MoveCombine::addMover(IGameMover *mover)
{
	combineList.push_back(MoverPos(mover, mover->getpos()));
}

void MoveCombine::update()
{
	if (combineList.empty())
	{
		reach();
	}
	else
	{
		GameXYZ pos(0, 0, 0);
		for (MoverPosList::iterator i = combineList.begin(); i != combineList.end();)
		{
			if (i->first->actived())
			{
				i->first->update();
				pos += i->first->getpos() - i->second;
				++i;
			}
			else
			{
				i = combineList.erase(i);
			}
		}
		setpos(cursor + pos);
	}
}
