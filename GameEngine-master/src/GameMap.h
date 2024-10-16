
#ifndef __GameMap_H__
#define __GameMap_H__

#include "OgreMemoryDelegation.h"
#include "GameEngineTypes.h"
#include "GameMaskMap.h"
#include <deque>
#include <list>
#include <limits>

class GameActor;
class GameMapMover : public Ogre::MemoryDelegation
{
public:
	GameMapMover(GameActor *pactor);

	void SetSpeed(float speed);
	void SetFixSpeed(float speed);
	void StartOnPath(GameMaskPath *ppath);
	void StopOnPoint(int x, int z);
	void Update(float dtime);
	void GetCurPos(int &x, int &z);
	bool isReach();
/*	void SetCurPos(int x, int z );*/
public:
	GameActor*		m_pActor;		// 当前所描述对象
	GameMaskPath	m_CurPath;		// 当前所描述对象的路径描述
	int				m_CurSeg;		// 当前所描述对象所在位置在其移动路径所有点中点索引
	float			m_SegOffset;	// 当前所描述对象在其移动路径相对于起始点的偏移
	float			m_Speed;		// 当前所描述对象的速度
	float			m_fixSpeed;
	int				m_CurPosX;		// 当前所描述对象在实际场景中位置的X坐标
	int				m_CurPosZ;		// 当前所描述对象在实际场景中位置的Z坐标
};

class GameMap
{
public:
	GameMap();
	~GameMap();

	void update(unsigned int dtick, Ogre::Camera *pcamera, Ogre::WorldPos &player_pos);

	bool getTerrainH(int x, int z, int &y);
	void UpdateMinMap(int x, int y ,int nMinLevel);
	const char*  GetMapName();

	bool Load(int Mapid, int realMapId, int x, int z);
	uintptr_t asynLoad(int Mapid, int realMapId, int x, int z);
	bool FindPath(GameMaskPath &path, int from_x, int from_z, int to_x, int to_z, unsigned int maskbit, bool bLong = false );
	bool InMap(int x, int y);
	int GetGridW();
	int GetGridH();
	bool GridHasMask(int x,int y, unsigned int mask);
	bool IsBlock( int X,int Y );
	int GetLinkThePointWaypoint( int vNum, int vIdx );

	int BetweenCost( int vSrc,int vDes );

	int GetLinkThePointWaypointNum( int vIdx ){ return 8; }

	void Pos2GridIdx(const int&x, const int&y, int &outx,int &outy)
	{
		outx = x;
		outy = y;
		m_pMaskMap->coordPixel2Grid(outx, outy);
	}

	Ogre::ShowGameMap *getShowGameMap()
	{
		return m_pShowMap;
	}

	GameMaskMap *getMaskMap()
	{
		return m_pMaskMap;
	}
	
	int getMapId() 
	{
		return m_MapID;
	}

	int getRealId()
	{
		return m_RealMapID;
	}

protected:
	int m_MapID;
	int m_RealMapID;
	GameMaskMap *m_pMaskMap;
	Ogre::ShowGameMap *m_pShowMap;
};

#endif