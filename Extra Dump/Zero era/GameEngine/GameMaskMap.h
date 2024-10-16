
#ifndef __GAMEMASKMAP_H__
#define __GAMEMASKMAP_H__

#include <vector>
#include <assert.h>
#include "OgreRect.h"
#include "MaskMapHeapNode.h"
#include "MapQuadTree.h"

class GameMaskMap;

//寻路结果
class GameMaskPath
{
	friend class GameMaskMap;

	struct PathPoint
	{
		int x; //虚拟屏幕上的像素坐标
		int y;
	};
	std::vector<PathPoint>m_Points;
	PathPoint m_RealGoal; //由于目标不可达， m_Points中路经的终点可能不是目标点，所以在这里存一下
	GameMaskMap *m_pMap;

public:
	GameMaskPath() : m_pMap(NULL){}
	void reset( GameMaskMap *pmap )
	{ 
		m_Points.resize(0); 
		m_pMap = pmap; 
	}
	void resize(size_t n)
	{
		m_Points.resize(n);
	}
	void append(const GameMaskPath&path)
	{
		m_Points.insert(m_Points.end(), path.m_Points.begin(), path.m_Points.end());
	}
	void addPathPoint( int x, int y );
	int  findNextVisiblePoint( int i, int mask );
	Ogre::Point2D getPathPoint( size_t i )
	{ 
		assert(i < m_Points.size() );
		return Ogre::Point2D(m_Points[i].x, m_Points[i].y); 
	}
	Ogre::Point2D getPathGoal()
	{
		assert( m_Points.size() > 1 );
		PathPoint &pt = m_Points[ m_Points.size()-1 ];
		return Ogre::Point2D( pt.x, pt.y );
	}
	size_t getNumPoint()
	{
		return m_Points.size(); 
	}
	void looseGoal( int radius ); //改变path, 让path的goal在和原来目标距离radius的地方
	void smoothPath( int mask );
	void limitPathPoint(size_t number)
	{
		assert( number >= 2 );
		if( number >= m_Points.size() ) 
		{
			return;
		}

		m_Points.resize( number );
	}

	bool isPointOnPath( const Ogre::Point2D &pt );
	double	getPathLength();
	int getSegmentLen(size_t i);
	size_t getNumSegment()
	{
		return m_Points.size() - 1;
	}
	bool isValidPath();

	static bool IsPointInSegment( int x, int y, int x1, int y1, int x2, int y2 );
};

class GameMaskMap
{
public:
	enum MaskBits
	{
		maskStop            =   MASK_STOP,	// 行走层
		maskWater           =   MASK_WATER,	// 水层
		maskMagic           =   MASK_MAGIC,	//技能火球飞行层, 角色飞行层
		maskLoMagic			=	MASK_SPELL,	// 技能穿透层
	};
	enum MaskPathResults
	{
		maskPathArrive      =   0,  //已經達到
		maskPathFound       =   1,  //找到可以到達的路徑
		maskPathNear        =   2,  //靠近目標
		maskPathFail        =   3,  //尋徑失敗
		maskPathError       =   -1, //錯誤
	};
	enum MaskStepResults
	{
		maskStepArrive      =   0,  //到達
		maskStepOnTheWay    =   1,   //在途中
		maskStepObstruct    =   2,   //被阻隔
		maskStepOutstretch  =   3,   //超出範圍
	};

public:
	GameMaskMap();
	~GameMaskMap();

	bool loadFromFile(const char *filepath, const char *quadtree_file=NULL);
	/**
	* 从开始点到结束点之间找出一条路经
	* \param path 用于存储返回的路径
	* \param start 开始点，以像素为单位
	* \param end 结束点，以像素为单位
	* \return 0: 成功， 
	*         1：程序逻辑错误（比如上次走到阻挡格里面去了）
	*         2：寻路节点过多，停止寻路，
	*         3: 起始点和目标点是同一个点
	*/
	bool findPath( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask = maskStop );
	int  findLongPath( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask = maskStop );

	void cutBlockPath(GameMaskPath &path, int mask);
	/**
	* 从开始点移动到结束点，只到碰到阻挡
	* \param start 开始点，以像素为单位
	* \param delta 移动矢量，以像素为单位
	* \return 移动的结束点
	*/
	Ogre::Point2D doMotion( const Ogre::Point2D &start, const Ogre::Point2D &delta );

	/**
	* 测试是否能够直线从start移动到end
	*/
	bool lineOfSightTest( const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop, unsigned int count = 0, Ogre::Point2D *result = NULL );
	bool lineOfSightTest1( const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop );
	bool lineOfSightTest2( const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop );

	/**
	* 根据输入的点，在附近找出能走的点，从此点往周围扩散着找
	* \return 是否能够找到
	*/
	bool findPointWalkable( Ogre::Point2D &point, int mask = maskStop, int range=50 );

	bool canWalkPixelCoord( int x, int y, int mask=maskStop )
	{
		if( !isPointInMap(Ogre::Point2D(x,y)) ) return false;

		int x1(x), y1(y); 
		coordPixel2Grid(x1, y1);
		return canWalk(x1,y1,mask);
	}

	bool canWalkGridCoord( int x, int y, int mask=maskStop )
	{
		return canWalk( x, y, mask );
	}

	bool isPointInMap( Ogre::Point2D &point )
	{
		return point.x>=0 && point.x<m_lPlaneWidth && point.y>=0 && point.y<m_lPlaneHeight;
	}

	//在半径为radius的圆圈上找一个方向，使得面对mask(比如水层)
	bool findDirFaceMask( Ogre::Point2D &point, const Ogre::Point2D &center, int radius, int mask );
	
	void createQuadTree();

	bool checkPoint( GameMaskPath & path, const Ogre::Point2D &next, int mask );

	/**
	* 设置半动态阻挡层
	* x，y：中心点坐标
	* r：半径
	* m：半动态状态
	*/
	void setDynMsk(int x, int y, int rx, int ry, int angle, int m);
	void UpdateSearchNodes();
public:
	int   m_lWidthMasks;
	int   m_lHeightMasks;
	int   m_lMaskPixelWidth;
	int   m_lMaskPixelHeight;
	int   m_lPlaneWidth;
	int   m_lPlaneHeight;
	CQuadTree		m_QuadTree;
	unsigned char*	m_pMaskData;			
	unsigned int*	m_pDyWalkMaskData;
	unsigned int*	m_pDyMagicMaskData;
	unsigned int	m_lastChangeTime;

	PathFindHeap m_PathHeap;
	Ogre::BitArray2D *m_pVisitMap;
	std::vector<PathHeapNode> m_ClosedNodes;
	std::vector<PathHeapNode> m_ClosedNodesLong;

	void tryGotoNeighbour( PathHeapNode &node, int dx, int dy, int mask );

	unsigned char getGridAttrib( int x, int y )
	{ 
		return m_pMaskData[y*m_lWidthMasks+x]; 
	}

	bool canWalk( int x, int y, int mask=maskStop )
	{ 
		assert( y*m_lWidthMasks+x < m_lWidthMasks*m_lHeightMasks );
		if (x < 0 || y < 0)
		{
			return false;
		}
		if (m_pMaskData == NULL)
		{
			return false;
		}
		int index = y*m_lWidthMasks+x;
		if (index < 0 || index >= m_lWidthMasks*m_lHeightMasks)
		{
			return false;
		}
		// 超出地图有效范围，则不允许寻路
		if (mask & MASK_STOP)
		{
			mask |= DYN_MASK_STOP;
		}
		if (mask & MASK_MAGIC)
		{
			mask |= DYN_MASK_MAGIC;
		}
		return ((m_pMaskData[index]&mask)==0);
	}

	void coordPixel2Grid( int &x, int &y )
	{ 
		x/=m_lMaskPixelWidth, y/=m_lMaskPixelHeight; 
	}

	void coordGrid2Pixel( int &x, int &y )
	{ 
		x = x*m_lMaskPixelWidth+m_lMaskPixelWidth/2;
		y = y*m_lMaskPixelHeight+m_lMaskPixelHeight/2;

	}

	/**
	* 从开始点到结束点之间找出一条路经
	* \param path 用于存储返回的路径
	* \param start 开始点，以像素为单位
	* \param end 结束点，以像素为单位
	* \return 0: 成功， 
	*         1：程序逻辑错误（比如上次走到阻挡格里面去了）
	*         2：寻路节点过多，停止寻路，
	*         3: 起始点和目标点是同一个点
	*/
	int findNaviPath_Internal( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop );

	//找出从start往end走，最远的直线路径
	bool findSimplePath_Internal( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop );
};

extern int	FaceAngleBetween( int from_x, int from_y, int to_x, int to_y );
extern bool	IsPointInFan( int x, int y, int ox, int oy, int radius, int face_angle, int angle_range );
extern bool	IsPointInCircle( int x, int y, int ox, int oy, int radius );
extern bool	IsPointInRectangle( int x, int y, int ox, int oy, int xlen, int ylen, int face_angle );

#endif