
#include "GameMap.h"
#include "GameActor.h"
#include "ShowGameMap.h"
#include "GameDBData.h"
#include "OgreLog.h"
#include "GameSkill_Mgr.h"
#include "GameMainPlayer.h"
#include <Windows.h>
#include <process.h>

using namespace Ogre;

struct LoadInfo
{
	GameMap	*map;
	int		mapid;
	int		realmapid;
	int		x;
	int		y;

	LoadInfo(GameMap *map, int mapid, int realmapid, int x, int y)
	{
		this->map = map;
		this->mapid = mapid;
		this->realmapid = realmapid;
		this->x = x;
		this->y = y;
	}
};

static unsigned __stdcall asynMapLoad(void *info)
{
	LoadInfo *ptr = (LoadInfo *)info;
	LoadInfo loadinfo = *ptr;
	delete ptr;
	loadinfo.map->Load(loadinfo.mapid, loadinfo.realmapid, loadinfo.x, loadinfo.y);
	return (unsigned)loadinfo.map;
}

GameMapMover::GameMapMover(GameActor *pactor) : m_pActor(pactor), m_Speed(2000.0f), m_CurSeg(-1), m_fixSpeed(0)
{
}

void GameMapMover::SetSpeed(float speed)
{
	if (m_fixSpeed != 0)
	{
		m_fixSpeed = m_fixSpeed * speed / m_Speed;
	}
	m_Speed = speed;
}

void GameMapMover::StartOnPath(GameMaskPath *ppath)
{
	m_CurPath = *ppath;
	m_CurSeg = 0;
	m_SegOffset = 0;
	if (m_CurPath.getNumPoint() > 0)
	{
		Point2D pt;
		pt = m_CurPath.getPathPoint(0);
		m_CurPosX = pt.x;
		m_CurPosZ = pt.y;
	}
	
	if( m_CurPath.getNumPoint() <= 1 )
	{
		return;
	}
	Point2D pt1, pt2;
	pt1 = m_CurPath.getPathPoint(0);
	pt2 = m_CurPath.getPathPoint(1);
	if (m_pActor->isInSkillTetanic())
	{
		return;
	}
	if (m_pActor->m_AttackLock.m_nAttackLockType == ATTACK_LOCK_NONE)
	{
		m_pActor->SetFaceTo(pt1.x, pt1.y, pt2.x, pt2.y);
	}
}

void GameMapMover::StopOnPoint(int x, int z)
{
	m_CurSeg = -1;
	m_CurPosX = x;
	m_CurPosZ = z;
}

void GameMapMover::Update(float dtime)
{
	if(m_CurSeg < 0) return;
	
	if( true || m_SegOffset != 0 )
	{
		if (m_fixSpeed == 0)
		{
			m_SegOffset += m_Speed * dtime;
		}
		else
		{
			m_SegOffset += m_fixSpeed * dtime;
		}
	}else{
		m_SegOffset += 1;
	}
	
	float seglen = 0;
	while(1)
	{
		seglen = (float)m_CurPath.getSegmentLen(m_CurSeg);
		if(m_SegOffset < seglen) break;

		m_SegOffset -= seglen;
		m_CurSeg++;
		if(m_CurSeg >= (int)m_CurPath.getNumSegment())
		{
			Point2D pt = m_CurPath.getPathPoint(m_CurPath.getNumSegment());
			m_CurPosX = pt.x;
			m_CurPosZ = pt.y;
			m_CurSeg = -1;
			m_fixSpeed = 0;
			m_pActor->OnReachPathTarget();
			return;
		}
	}

	Point2D pt1, pt2;
	pt1 = m_CurPath.getPathPoint(m_CurSeg);
	pt2 = m_CurPath.getPathPoint(m_CurSeg+1);
	
	//char buf[256];
	//sprintf(buf, "最大节点数:%d, 当前节点数:%d\n", (int)m_CurPath.getNumPoint(), m_CurSeg );
	//OutputDebugString(buf);

	float t = m_SegOffset / seglen;
	m_CurPosX = int(pt1.x + t*(pt2.x-pt1.x));
	m_CurPosZ = int(pt1.y + t*(pt2.y-pt1.y));
	if( m_pActor->m_AttackLock.m_nAttackLockType != ATTACK_LOCK_NONE && m_pActor->m_curSkillAction != 0 )
	{
		return;
	}
	if (m_pActor->isInSkillTetanic())
	{
		return;
	}
	if (m_pActor->m_AttackLock.m_nAttackLockType == ATTACK_LOCK_NONE)
	{
		m_pActor->SetFaceTo(pt1.x, pt1.y, pt2.x, pt2.y);
	}
}

void GameMapMover::GetCurPos(int &x, int &z)
{
	x = m_CurPosX;
	z = m_CurPosZ;
}

void GameMapMover::SetFixSpeed(float speed)
{
	m_fixSpeed = speed;
}

bool GameMapMover::isReach()
{
	return m_CurSeg < 0;
}

// void GameMapMover::SetCurPos(int x, int z )
// {
// 	m_CurPosX = x;
// 	m_CurPosZ = z;
// }

GameMap::GameMap() : m_MapID(-1), m_pMaskMap(NULL), m_pShowMap(NULL), m_RealMapID(-1)
{
}

GameMap::~GameMap()
{
	delete m_pMaskMap;
	delete m_pShowMap;
}

void GameMap::update(unsigned int dtick, Ogre::Camera *pcamera, Ogre::WorldPos &player_pos)
{
	m_pShowMap->update(dtick, pcamera, player_pos);
}

bool GameMap::getTerrainH(int x, int z, int &y)
{
	y = m_pShowMap->getGroundPoint(x, z, NULL, NULL);
	return true;
}

void GameMap::UpdateMinMap(int x, int y ,int nMinLevel)
{

}

const char*  GameMap::GetMapName()
{
	return "aaa";
}

bool GameMap::Load(int mapId, int realMapId, int x, int z)
{
	m_MapID = mapId;
	m_RealMapID = realMapId;
	m_pShowMap = new ShowGameMap;
	
	const tagMapDef &mapdata = DBDataManager::getSingleton().getMapDef(mapId);
	if(!m_pShowMap->LoadFromMapDesc(mapdata, WorldPos(x, 0, z)))
	{
		LOG_SEVERE("load map %d failed", mapId);
		return false;
	}

	char path[256], qdtreepath[256];
	sprintf(path, "maps\\%d\\%d.msk", mapId, mapId);
	sprintf(qdtreepath, "maps\\%d\\%d.mqt", mapId, mapId);
	m_pMaskMap = new GameMaskMap;
	if(!m_pMaskMap->loadFromFile(path, qdtreepath))
	{
		LOG_WARNING("failed to load %s", path);
		return false;
	}
/*
	bool b = m_pMaskMap->lineOfSightTest1(Point2D(20250,20750), Point2D(20225,20725));
	b = m_pMaskMap->lineOfSightTest(Point2D(20250,20750), Point2D(20225,20725));
	b = b;
	*/

	
	//GameMaskPath pp1, pp2;
	////int r3 = m_pMaskMap->findLongPath(pp1, Point2D(20250, 20750), Point2D(20225, 20725));
	//int r1 = m_pMaskMap->findLongPath(pp1, Point2D(20250, 20750), Point2D(20423, 20544));
	//bool r2 = m_pMaskMap->findPath(pp2, Point2D(20250, 20750), Point2D(20423, 20544));

	//assert(pp1.isValidPath());
	//assert(pp2.isValidPath());
	
	return true;
}

uintptr_t GameMap::asynLoad(int Mapid, int realMapId, int x, int z)
{
	return _beginthreadex(NULL, 0, asynMapLoad, new LoadInfo(this, Mapid, realMapId, x, z), 0, NULL);
}


bool GameMap::FindPath(GameMaskPath &path, int from_x, int from_z, int to_x, int to_z, unsigned int maskbit, bool bLong )
{
	if( bLong )
	{
		if( m_pMaskMap->findLongPath(path, Point2D(from_x, from_z), Point2D(to_x, to_z), maskbit) != 0 )
		{
			return false;
		}
	}else{
		if( !m_pMaskMap->findPath(path, Point2D(from_x, from_z), Point2D(to_x, to_z), maskbit) != 0 )
		{
			return false;
		}
	}
	
	if(path.getNumPoint() == 0)
	{
		return false;
	}

	path.smoothPath( maskbit );
	return true;
}

bool GameMap::InMap(int x, int y)
{
	return m_pMaskMap->isPointInMap(Point2D(x,y));
}
int GameMap::GetGridW()
{
	return m_pMaskMap->m_lWidthMasks;
}
int GameMap::GetGridH()
{
	return m_pMaskMap->m_lHeightMasks;
}

bool GameMap::GridHasMask( int X,int Y, unsigned int mask )
{
	return m_pMaskMap->canWalk(X, Y, mask);
}

bool GameMap::IsBlock( int X,int Y )
{
	return m_pMaskMap->canWalk(X, Y);
}
