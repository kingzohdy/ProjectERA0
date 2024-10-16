#define _USE_MATH_DEFINES
#include <math.h>
#include <Windows.h>
#include "MoveParabola.h"

MoveParabola::MoveParabola(const GameXYZ &start, const GameXYZ &end, int height, unsigned int time) :
	IGameMover(start), target(end.toVector3()), cursor(start.toVector3()), cost(0), during(time)
{
	lastTime = timeGetTime();
	{// 抛物线参数初始化
		if (end.y - start.y > height)
		{
			height += end.y - start.y;
		}
		parama = (1 + sqrt(1 - (end.y - start.y) * 1.0 / height)) * 2 * height / time;
		paramb = -parama * parama / (4 * height);

		startHeight = start.y;
	}
}

void MoveParabola::update()
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
		cursor.y = (paramb * cost * cost + parama * cost + startHeight) / GameXYZ::UNIT;
		setpos(cursor);
	}
}
