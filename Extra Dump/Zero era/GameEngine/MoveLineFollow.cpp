#include <Windows.h>
#include "MoveLineFollow.h"
#include "GameActorManager.h"
#include "GameMap.h"
#include "GameActor.h"
#include "ShowActor.h"

MoveLineFollow::MoveLineFollow(const GameXYZ &start, int actorId, int bindId, unsigned int time) :
	IGameMover(start), who(actorId), bindpoint(bindId), cursor(start.toVector3()), cost(0), during(time), speed(0)
{
	lastTime = timeGetTime();
}

void MoveLineFollow::update()
{
	unsigned int now = timeGetTime();
	unsigned int dtick = now - lastTime;
	if (dtick == 0)
	{
		return;
	}
	GameActor *target = ActorManager::getSingleton().FindActor(who);
	if (target == NULL)
	{
		reach();
	}
	else
	{
		lastTime = now;
		cost += dtick;
		GameXYZ pos = target->getShowActor()->getCurEntity()->getAnchorWorldPos(bindpoint);
		if (cost >= during)
		{
			setpos(pos);
			reach();
		}
		else
		{
			float distance = (pos - cursor).toVector3().length();
			float newSpeed = distance / (during - cost + dtick);
			speed = speed < newSpeed ? newSpeed : speed;
			if (speed * dtick >= distance)
			{
				cost = during;
				setpos(pos);
				reach();
			}
			else
			{
				cursor += (pos - cursor).normalize() * (speed * dtick);
				setpos(cursor);
			}
		}
	}
}
