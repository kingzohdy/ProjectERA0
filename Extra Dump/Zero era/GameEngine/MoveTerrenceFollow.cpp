#include <Windows.h>
#include "MoveTerrenceFollow.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include "GameMap.h"
#include "OgreRect.h"

MoveTerrenceFollow::MoveTerrenceFollow(const GameXYZ &start, int actorId, unsigned int time) :
	IGameMover(start), cursor(start.toVector3()), who(actorId), cost(0), during(time), speed(0), cross(false)
{
	map = ActorManager::getSingleton().getMainPlayer()->getCurMap();
	if (map == NULL)
	{
		reach();
		return;
	}
	map->getTerrainH(start.x, start.z, height);
	height = start.y - height;
	lastTime = timeGetTime();
}

void MoveTerrenceFollow::update()
{
	unsigned int now = timeGetTime();
	unsigned int dtick = now - lastTime;
	if (dtick == 0)
	{
		return;
	}
	if (map != ActorManager::getSingleton().getMainPlayer()->getCurMap())
	{
		reach();
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
		GameXYZ pos = target->GetPosition();
		if (cost >= during)
		{
			cursor = pos.toVector3();
			reach();
		}
		else if (cross)
		{
			float distance = (pos - cursor).toVector3().length();
			float newSpeed = distance / (during - cost + dtick);
			speed = speed < newSpeed ? newSpeed : speed;
			if (speed * dtick >= distance)
			{
				cost = during;
				cursor = pos.toVector3();
				reach();
			}
			else
			{
				cursor += (pos - cursor).normalize() * (speed * dtick);
			}
		}
		else
		{
			if (map == NULL)
			{
				reach();
				return;
			}
			path.reset(map->getMaskMap());
			Ogre::Vector3 pt = pos.toVector3();
			if (!map->FindPath(path, int(cursor.x), int(cursor.y), int(pt.x), int(pt.z),
				MASK_STOP | DYN_MASK_STOP, true))
			{
				cross = true;
			}
			else
			{
				if (path.getNumPoint() <= 1)
				{
					cross = true;
				}
				else
				{
					float newSpeed = float(path.getPathLength() / (during - cost + dtick));
					speed = speed < newSpeed ? newSpeed : speed;

					float offset = speed * dtick;
					float seglen;
					unsigned int seg = 0;
					while (true)
					{
						seglen = (float)path.getSegmentLen(seg);
						if (offset < seglen)
							break;

						offset -= seglen;
						if (++seg == path.getNumSegment())
						{
							cursor = pos.toVector3();
							reach();
							break;
						}
					}

					if (seg < path.getNumSegment())
					{
						Ogre::Point2D pt1, pt2;
						pt1 = path.getPathPoint(seg);
						pt2 = path.getPathPoint(seg + 1);
						float t = offset / seglen;
						cursor.x = pt1.x + t * (pt2.x - pt1.x);
						cursor.z = pt1.y + t * (pt2.y - pt1.y);
					}
				}
			}
		}
		pos = cursor;
		map->getTerrainH(pos.x, pos.z, pos.y);
		pos.y += height;
		setpos(pos);
	}
}
