
#ifndef __GAMEMASKMAP_H__
#define __GAMEMASKMAP_H__

#include <vector>
#include <assert.h>
#include "OgreRect.h"
#include "MaskMapHeapNode.h"
#include "MapQuadTree.h"

class GameMaskMap;

//Ѱ·���
class GameMaskPath
{
	friend class GameMaskMap;

	struct PathPoint
	{
		int x; //������Ļ�ϵ���������
		int y;
	};
	std::vector<PathPoint>m_Points;
	PathPoint m_RealGoal; //����Ŀ�겻�ɴ m_Points��·�����յ���ܲ���Ŀ��㣬�����������һ��
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
	void looseGoal( int radius ); //�ı�path, ��path��goal�ں�ԭ��Ŀ�����radius�ĵط�
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
		maskStop            =   MASK_STOP,	// ���߲�
		maskWater           =   MASK_WATER,	// ˮ��
		maskMagic           =   MASK_MAGIC,	//���ܻ�����в�, ��ɫ���в�
		maskLoMagic			=	MASK_SPELL,	// ���ܴ�͸��
	};
	enum MaskPathResults
	{
		maskPathArrive      =   0,  //�ѽ��_��
		maskPathFound       =   1,  //�ҵ����Ե��_��·��
		maskPathNear        =   2,  //����Ŀ��
		maskPathFail        =   3,  //����ʧ��
		maskPathError       =   -1, //�e�`
	};
	enum MaskStepResults
	{
		maskStepArrive      =   0,  //���_
		maskStepOnTheWay    =   1,   //��;��
		maskStepObstruct    =   2,   //�����
		maskStepOutstretch  =   3,   //��������
	};

public:
	GameMaskMap();
	~GameMaskMap();

	bool loadFromFile(const char *filepath, const char *quadtree_file=NULL);
	/**
	* �ӿ�ʼ�㵽������֮���ҳ�һ��·��
	* \param path ���ڴ洢���ص�·��
	* \param start ��ʼ�㣬������Ϊ��λ
	* \param end �����㣬������Ϊ��λ
	* \return 0: �ɹ��� 
	*         1�������߼����󣨱����ϴ��ߵ��赲������ȥ�ˣ�
	*         2��Ѱ·�ڵ���ֹ࣬ͣѰ·��
	*         3: ��ʼ���Ŀ�����ͬһ����
	*/
	bool findPath( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask = maskStop );
	int  findLongPath( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask = maskStop );

	void cutBlockPath(GameMaskPath &path, int mask);
	/**
	* �ӿ�ʼ���ƶ��������㣬ֻ�������赲
	* \param start ��ʼ�㣬������Ϊ��λ
	* \param delta �ƶ�ʸ����������Ϊ��λ
	* \return �ƶ��Ľ�����
	*/
	Ogre::Point2D doMotion( const Ogre::Point2D &start, const Ogre::Point2D &delta );

	/**
	* �����Ƿ��ܹ�ֱ�ߴ�start�ƶ���end
	*/
	bool lineOfSightTest( const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop, unsigned int count = 0, Ogre::Point2D *result = NULL );
	bool lineOfSightTest1( const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop );
	bool lineOfSightTest2( const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop );

	/**
	* ��������ĵ㣬�ڸ����ҳ����ߵĵ㣬�Ӵ˵�����Χ��ɢ����
	* \return �Ƿ��ܹ��ҵ�
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

	//�ڰ뾶Ϊradius��ԲȦ����һ������ʹ�����mask(����ˮ��)
	bool findDirFaceMask( Ogre::Point2D &point, const Ogre::Point2D &center, int radius, int mask );
	
	void createQuadTree();

	bool checkPoint( GameMaskPath & path, const Ogre::Point2D &next, int mask );

	/**
	* ���ð붯̬�赲��
	* x��y�����ĵ�����
	* r���뾶
	* m���붯̬״̬
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
		// ������ͼ��Ч��Χ��������Ѱ·
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
	* �ӿ�ʼ�㵽������֮���ҳ�һ��·��
	* \param path ���ڴ洢���ص�·��
	* \param start ��ʼ�㣬������Ϊ��λ
	* \param end �����㣬������Ϊ��λ
	* \return 0: �ɹ��� 
	*         1�������߼����󣨱����ϴ��ߵ��赲������ȥ�ˣ�
	*         2��Ѱ·�ڵ���ֹ࣬ͣѰ·��
	*         3: ��ʼ���Ŀ�����ͬһ����
	*/
	int findNaviPath_Internal( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop );

	//�ҳ���start��end�ߣ���Զ��ֱ��·��
	bool findSimplePath_Internal( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask=maskStop );
};

extern int	FaceAngleBetween( int from_x, int from_y, int to_x, int to_y );
extern bool	IsPointInFan( int x, int y, int ox, int oy, int radius, int face_angle, int angle_range );
extern bool	IsPointInCircle( int x, int y, int ox, int oy, int radius );
extern bool	IsPointInRectangle( int x, int y, int ox, int oy, int xlen, int ylen, int face_angle );

#endif