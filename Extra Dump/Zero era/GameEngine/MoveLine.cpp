#include <Windows.h>
#include "MoveLine.h"

MoveLine::MoveLine(const GameXYZ &start, const GameXYZ &end, unsigned int time) :
	IGameMover(start), target(end.toVector3()), cursor(start.toVector3()), cost(0), during(time)
{
	lastTime = timeGetTime();
}

void MoveLine::update()
{
	unsigned int now = timeGetTime();
	unsigned int dtick = now - lastTime;
	if (dtick == 0)
	{
		return;
	}
	lastTime = now;
	cost += dtick;
	if (cost >= during)
	{
		setpos(target);
		reach();
	}
	else
	{
		cursor += (target - cursor) / (1 + (during - cost) * 1.0f / dtick);
		setpos(cursor);
	}
}
