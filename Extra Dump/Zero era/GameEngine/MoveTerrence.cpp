#include <Windows.h>
#include "MoveTerrence.h"
#include "GameActorManager.h"
#include "GameMap.h"
#include "GameMainPlayer.h"
#include "OgreRect.h"

MoveTerrence::MoveTerrence(const GameXYZ &start, int x, int y, unsigned int time) :
	IGameMover(start), cost(0), during(time), target(x / float(GameXYZ::UNIT), y / float(GameXYZ::UNIT)), offset(0)
{
	map = ActorManager::getSingleton().getMainPlayer()->getCurMap();
	if (map == NULL)
	{
		reach();
		return;
	}
	Ogre::Vector3 v = start.toVector3();
	cursor.x = v.x;
	cursor.y = v.z;
	map->getTerrainH(start.x, start.z, height);
	height = start.y - height;
	lastTime = timeGetTime();

	path.reset(map->getMaskMap());
	if (map->FindPath(path, int(cursor.x), int(cursor.y), int(target.x), int(target.y),
		MASK_STOP | DYN_MASK_STOP, true))
	{
		speed = float(path.getPathLength() / time);
		seg = 0;
	}
	else
	{
		speed = 0;
	}
}

void MoveTerrence::update()
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
	lastTime = now;
	cost += dtick;
	if (cost >= during)
	{
		cursor.x = target.x;
		cursor.y = target.y;
		reach();
	}
	else if (path.getNumPoint() > 1)
	{
		offset += speed * dtick;
		float seglen;
		while (true)
		{
			seglen = (float)path.getSegmentLen(seg);
			if (offset < seglen)
				break;

			offset -= seglen;
			if (++seg == path.getNumSegment())
			{
				cursor.x = target.x;
				cursor.y = target.y;
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
			cursor.y = pt1.y + t * (pt2.y - pt1.y);
		}
	}
	else
	{
		cursor += (target - cursor) / (1 + (during - cost) * 1.0f / dtick);
	}
	GameXYZ pos(Ogre::Vector3(cursor.x, 0, cursor.y));
	map->getTerrainH(pos.x, pos.z, pos.y);
	pos.y += height;
	setpos(pos);
}
