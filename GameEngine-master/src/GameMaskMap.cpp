#include <math.h>
#include "GameMaskMap.h"
#include "OgreBitArray.h"
#include "OgreResourceManager.h"
#include "OgreDataStream.h"
#include "OgreLog.h"
#include "proto_comm.h"
#include "GameEffect_Mgr.h"
#include "OgreMemoryDelegation.h"
#include <Windows.h>

using namespace Ogre;

const double PI = 3.1415926;

struct MNavHead
{
	char magic_number[4];
	int version;
	int headsize;
	int filesize;
	int ngridx;
	int ngridz;
	int gridsizex;
	int gridsizez;
};

const int MAX_NAVIGRID = 5000;
const int MAX_NAVIGRID_LONG = 100000;	// 大路径寻路的CLOSE节点数 

const float MY_PI = 3.1415926f;

bool  IsPointInCircle( int x, int y, int ox, int oy, int radius )
{
	long r2 = (x-ox)*(x-ox) + (y-oy)*(y-oy);
	return r2<radius*radius ? 1 : 0;
}

inline float mycos( float angle )
{
	return cos(angle*MY_PI/180.0f);
}
inline float mysin( float angle )
{
	return sin(angle*MY_PI/180.0f);
}

int FaceAngleBetween( int from_x, int from_y, int to_x, int to_y )
{
	double dx = to_x - from_x;
	double dy = to_y - from_y;

	double length = sqrt(dx*dx + dy*dy);
	if( length < 0.5 ) return 0;

	double tmp = dx / length;
	double angle = acos(tmp) * 180 / MY_PI;
	if( dy > 0 ) angle = (360 - angle);

	return (int)angle;
}

bool  IsPointInFan( int x, int y, int ox, int oy, int radius, int face_angle, int angle_range )
{
	float EPSILON = 5.0f;
	if( !IsPointInCircle(x, y, ox, oy, radius) ) return 0;

	float r = (float)sqrt(double((x-ox)*(x-ox) + (y-oy)*(y-oy)));
	if( r < EPSILON ) return 1;

	float mydot = ((x-ox)/r)*mycos((float)face_angle) - ((y-oy)/r)*mysin((float)face_angle);
	return mydot>mycos(angle_range*0.5f) ? 1 : 0;
}

bool  IsPointInRectangle( int x, int y, int ox, int oy, int xlen, int ylen, int face_angle )
{
	int x1 = (int)( (x-ox)*mycos((float)face_angle) - (y-oy)*mysin((float)face_angle) );
	int y1 = (int)( (x-ox)*mysin((float)face_angle) + (y-oy)*mycos((float)face_angle) );

	if( x1>0 && x1<xlen && y1>-ylen/2 && y1<ylen/2 ) return 1;
	else return 0;
}

typedef struct tagDPoint
{
	double X;                              
	double Y;                              
}DPOINT;

/* 基本几何结构 */
// 直线的解析方程 a*x+b*y+c=0 为统一表示，约定 a>= 0
typedef struct  tagLINE
{
	double a;
	double b;
	double c;
}LINE;
typedef struct LINESEGS
{
	DPOINT s;
	DPOINT e;
}LINESEG;

double multiply(DPOINT sp,DPOINT ep,DPOINT op);

int InsideConvexPolygon(int vcount, RESPOS polygon[], RESPOS q);

RESPOS z_rotate_pos(RESPOS o,int iAngle,RESPOS p);

// 返回点p以点o为圆心逆时针旋转iAngle(单位：角度)后所在的位置
RESPOS z_rotate_pos(RESPOS o,int iAngle,RESPOS p)
{
	RESPOS tp;
	p.X -=o.X;
	p.Y -=o.Y;
	tp.X=p.X*cos(iAngle*PI/180.0f) - p.Y*sin(iAngle*PI/180.0f)+o.X;
	tp.Y=p.Y*cos(iAngle*PI/180.0f) + p.X*sin(iAngle*PI/180.0f)+o.Y;

	return tp;
}

/*(sp-op)*(ep-op)的叉积
r=multiply(sp,ep,op),得到(sp-op)*(ep-op)的叉积
r>0:sp在矢量op ep的顺时针方向；
r=0：op sp ep三点共线；
r<0: sp在矢量op ep的逆时针方向 */
double multiply(DPOINT sp,DPOINT ep,DPOINT op)
{	
	return((sp.X-op.X)*(ep.Y-op.Y) - (ep.X-op.X)*(sp.Y-op.Y));
}

//判断点q在凸多边形polygon内
// 点q是凸多边形polygon内[包括边上]时，返回true
// 注意：多边形polygon一定要是凸多边形
int InsideConvexPolygon(int vcount, RESPOS polygon[], RESPOS q1)
{
	DPOINT p;
	DPOINT q;
	LINESEG l;
	int i;

	q.X= q1.X;
	q.Y= q1.Y;

	p.X=0; p.Y=0;
	for(i=0;i<vcount;i++) // 寻找一个肯定在多边形polygon内的点p：多边形顶点平均值
	{
		p.X+=polygon[i].X;
		p.Y+=polygon[i].Y;
	}

	p.X /= vcount;
	p.Y /= vcount;
	for(i=0;i<vcount;i++)
	{
		l.s.X=polygon[i].X;
		l.s.Y=polygon[i].Y;
		l.e.X=polygon[(i+1)%vcount].X;
		l.e.Y=polygon[(i+1)%vcount].Y;
		if(multiply(p,l.e,l.s)*multiply(q,l.e,l.s)<0)
		{
			/* 点p和点q在边l的两侧，说明点q肯定在多边形外 */
			return 0;
		}
	}
	return 1;
}

bool GameMaskPath::IsPointInSegment( int x, int y, int x1, int y1, int x2, int y2 )
{
	if( x1 <= x2 )
	{
		if( x<x1 || x>x2 ) return false;
	}
	else if( x<x2 || x>x1 ) return false;

	if( y1 <= y2 )
	{
		if( y<y1 || y>y2 ) return false;
	}
	else if( y<y2 || y>y1 ) return false;

	int r = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1);
	if( r == 0 ) return true;
	int dist = (x-x1)*(y2-y1) - (y-y1)*(x2-x1);
	dist = dist*dist / r;

	return dist<=2;
}

bool GameMaskPath::isPointOnPath( const Point2D &pt )
{
	if(m_Points.size() == 0) return false;
	for(size_t i=0; i<m_Points.size()-1; i++ )
	{
		if( IsPointInSegment(pt.x, pt.y, m_Points[i].x, m_Points[i].y, m_Points[i+1].x, m_Points[i+1].y) ) return true;
	}
	return false;
}

double  GameMaskPath::getPathLength()
{
	double len = 0;
	if(m_Points.size() == 0) return 0;

	for(size_t i=0; i<m_Points.size()-1; i++ )
	{
		PathPoint &p1 = m_Points[i];
		PathPoint &p2 = m_Points[i+1];

		len += sqrt(double((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)));
	}

	return len;
}

int GameMaskPath::getSegmentLen(size_t i)
{
	// TODO:这边不可能出现的。机器人测试服务器发过来的数据有问题，故先打开
	if( i+1 >= m_Points.size() )
	{
		return 0;
	} 

	PathPoint &p1 = m_Points[i];
	PathPoint &p2 = m_Points[i+1];

	return (int)sqrt(double((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y)));
}

void GameMaskPath::addPathPoint( int x, int y )
{
	if( m_Points.size() > 0 )
	{
		PathPoint &tmp = m_Points[m_Points.size()-1];
		if( x==tmp.x && y==tmp.y ) return;
	}
	PathPoint pt;
	pt.x = x;
	pt.y = y;

	m_Points.push_back(pt);
}

int  GameMaskPath::findNextVisiblePoint( int i, int mask )
{
	assert( m_Points.size()>1 && i<(int)m_Points.size() );
	Point2D begin(m_Points[i].x, m_Points[i].y);

	int next = i+1;
	while( next < (int)m_Points.size() )
	{
		bool b = m_pMap->lineOfSightTest( begin, Point2D(m_Points[next].x, m_Points[next].y), mask );
		//assert( b == m_pMap->lineOfSightTest1(begin, Point2D(m_Points[next].x, m_Points[next].y)), mask );
		if( !b ) break;
		next++;
	}
	return next-1;
}

bool GameMaskPath::isValidPath()
{
	Point2D curpt = getPathPoint(0);
	for(size_t i=1; i<m_Points.size(); i++)
	{
		Point2D next = getPathPoint(i);
		bool b1 = m_pMap->lineOfSightTest(curpt, next);
		bool b2 = m_pMap->lineOfSightTest1(curpt, next);

		if(!b1 || !b2)
		{
			return false;
		}
		curpt = next;
	}
	return true;
}

void GameMaskPath::looseGoal( int radius ) //改变path, 让path的goal在和原来目标距离radius的地方
{
	if( radius <= 0 ) return;

	size_t i;
	for( i=0; i<m_Points.size(); i++ )
	{
		int dx = m_Points[i].x - m_RealGoal.x;
		int dy = m_Points[i].y - m_RealGoal.y;
		if( dx*dx+dy*dy <= radius*radius )
		{
			break;
		}
	}

	if( i < m_Points.size() )
	{
		m_Points.resize(i+1);
	}
}

void GameMaskPath::smoothPath( int mask )
{
	if (mask & MASK_STOP)
	{
		mask |= DYN_MASK_STOP;
	}
	if (mask & MASK_MAGIC)
	{
		mask |= DYN_MASK_MAGIC;
	}
	int begin = 0;
	while(1)
	{
		int end = findNextVisiblePoint(begin, mask);
		if( end == begin ) break; //到终点了
		if( end > begin+1 ) //之间有多余的路径点
		{
			m_Points.erase( m_Points.begin()+begin+1, m_Points.begin()+end );
		}
		begin++;
	}
}


GameMaskMap::GameMaskMap() : m_pMaskData(NULL), m_pDyWalkMaskData(NULL), m_pDyMagicMaskData(NULL), m_pVisitMap(NULL), m_lastChangeTime(0)
{
	m_ClosedNodes.reserve( MAX_NAVIGRID );
}

GameMaskMap::~GameMaskMap()
{
	if( m_pMaskData ) 
	{
		Ogre::release( m_pMaskData );
		m_pMaskData = NULL;
		Ogre::release(m_pDyWalkMaskData);
		m_pDyWalkMaskData = NULL;
		Ogre::release(m_pDyMagicMaskData);
		m_pDyMagicMaskData = NULL;
	}
	delete m_pVisitMap;
	m_pVisitMap = NULL;
}

bool GameMaskMap::loadFromFile(const char *filepath, const char *quadtree_file)
{
	Ogre::DataStream *pstream = Ogre::ResourceManager::getSingleton().openFileStream(filepath, true);
	if( pstream == NULL )
	{
		assert(0); 
		return false;
	}
	MNavHead header;
	pstream->read(&header, sizeof(header));

	m_lWidthMasks = header.ngridx;
	m_lHeightMasks = header.ngridz;
	m_lMaskPixelWidth = header.gridsizex;
	m_lMaskPixelHeight = header.gridsizez;
	m_lPlaneWidth = m_lWidthMasks * m_lMaskPixelWidth;
	m_lPlaneHeight = m_lHeightMasks * m_lMaskPixelHeight;

	if(m_pMaskData)
	{
		Ogre::release(m_pMaskData);
		m_pMaskData = NULL;
		Ogre::release(m_pDyWalkMaskData);
		m_pDyWalkMaskData = NULL;
		Ogre::release(m_pDyMagicMaskData);
		m_pDyMagicMaskData = NULL;
		delete m_pVisitMap;
		m_pVisitMap = NULL;
	}
	m_pMaskData = (unsigned char *)Ogre::alloc(m_lWidthMasks*m_lHeightMasks);
	m_pDyWalkMaskData = (unsigned int *)Ogre::alloc(m_lWidthMasks*m_lHeightMasks * sizeof(int));
	memset(m_pDyWalkMaskData, 0, m_lWidthMasks*m_lHeightMasks * sizeof(int));
	m_pDyMagicMaskData = (unsigned int *)Ogre::alloc(m_lWidthMasks*m_lHeightMasks * sizeof(int));
	memset(m_pDyMagicMaskData, 0, m_lWidthMasks*m_lHeightMasks * sizeof(int));
	pstream->read(m_pMaskData, m_lWidthMasks*m_lHeightMasks);

	m_pVisitMap = new BitArray2D(m_lWidthMasks, m_lHeightMasks, 1);

	bool create_qdtree = true;
	if(quadtree_file)
	{
		if(m_QuadTree.LoadQdtFile(quadtree_file)) create_qdtree = false;
	}
	
	if(create_qdtree) createQuadTree();

	OGRE_DELETE(pstream);
	return true;
}

static Point2D s_StartGrid;
inline int ComputeHeuticsCost( int x, int y )
{
	return (abs(x-s_StartGrid.x) + abs(y-s_StartGrid.y))*2/3;
}
static std::vector<PathHeapNode>::iterator FindHeapNodeInArray( std::vector<PathHeapNode> &array, unsigned int index )
{
	std::vector<PathHeapNode>::iterator iter = array.begin();
	for( ; iter!=array.end(); iter++ )
	{
		if( iter->index == index ) return iter;
	}
	return array.end();
}

void GameMaskMap::tryGotoNeighbour( PathHeapNode &from, int dx, int dy, int mask )
{
	int x1 = INDEX_TO_X(from.index) + dx;
	int y1 = INDEX_TO_Y(from.index) + dy;

	if( x1<0 || x1>=m_lWidthMasks || y1<0 || y1>=m_lHeightMasks ) return; //超出地图外
	if( getGridAttrib(x1,y1) & mask ) return; //人物阻挡，不能通过

	unsigned int curindex = MAKEINDEX( x1, y1 );
	int aval = from.aval + 1;
	if( m_pVisitMap->getPixel(x1,y1) )//已经访问过了
	{
		std::vector<PathHeapNode>::iterator iter = FindHeapNodeInArray( m_PathHeap.m_AllNodes, curindex );
		if( iter != m_PathHeap.m_AllNodes.end() )
		{
			if( aval < iter->aval )
			{
				iter->aval = aval;
				iter->from = from.index;
				m_PathHeap.AdjustNode( iter );
			}
		}
		return;
	}

	PathHeapNode tmpnode;
	tmpnode.from = from.index;
	tmpnode.index = curindex;
	tmpnode.aval = aval;
	tmpnode.hval = ComputeHeuticsCost(x1, y1);
	m_PathHeap.PushNode( tmpnode );

	m_pVisitMap->setPixel(x1, y1, 1);
}

int GameMaskMap::findNaviPath_Internal( GameMaskPath &path, const Point2D &start, const Point2D &end, int mask )
{
	Point2D scale(m_lMaskPixelWidth, m_lMaskPixelHeight);
	s_StartGrid = start / scale;
	if( !canWalk(s_StartGrid.x, s_StartGrid.y, mask) ) return 1;

	Point2D pathend(end);
	Point2D endgrid = pathend / scale;
	if( !canWalk(endgrid.x, endgrid.y, mask) )
	{
		if( !findPointWalkable(pathend,mask) ) return 2;
		endgrid = pathend / scale;
	}
	if( start == pathend ) return 3;

	m_pVisitMap->clear(0);
	m_PathHeap.Init();
	m_ClosedNodes.resize(0);

	PathHeapNode node;
	node.from = node.index = MAKEINDEX(endgrid.x, endgrid.y);
	node.hval = node.aval = 0;
	m_PathHeap.PushNode( node );
	m_pVisitMap->setPixel(endgrid.x, endgrid.y, 1);

	bool bfind = false;
	unsigned int startindex = MAKEINDEX( s_StartGrid.x, s_StartGrid.y );
	while( m_PathHeap.HasNode() )
	{
		m_PathHeap.PopNode( node );
		m_ClosedNodes.push_back( node );
		if( m_ClosedNodes.size() >= MAX_NAVIGRID )
			break;

		if( node.index == startindex )
		{
			bfind = true;
			break;
		}
		else
		{
			tryGotoNeighbour( node, -1, 0, mask );
			tryGotoNeighbour( node, +1, 0, mask );
			tryGotoNeighbour( node, 0, -1, mask );
			tryGotoNeighbour( node, 0, +1, mask );
		}
	}

	if( bfind )
	{
		path.reset(this);
		path.addPathPoint( start.x, start.y );

		while( node.from != node.index )
		{
			int x = INDEX_TO_X(node.index)*m_lMaskPixelWidth + m_lMaskPixelWidth/2;
			int y = INDEX_TO_Y(node.index)*m_lMaskPixelHeight + m_lMaskPixelHeight/2;
			path.addPathPoint( x, y );
			assert( canWalkPixelCoord(x, y, mask) );

			std::vector<PathHeapNode>::iterator iter = FindHeapNodeInArray(m_ClosedNodes, node.from);
			assert( iter != m_ClosedNodes.end() );
			node = *iter;
		}

		path.addPathPoint( pathend.x, pathend.y );
		assert( canWalkPixelCoord(pathend.x, pathend.y,mask) );

		assert( path.m_Points.size() > 1 );
		return 0;
	}
	return 2;
}

bool GameMaskMap::findPath( GameMaskPath &path, const Point2D &start, const Point2D &end, int mask )
{
	// 保存下真正目标
	path.m_RealGoal.x = end.x;
	path.m_RealGoal.y = end.y;

	int naviret = findNaviPath_Internal( path, start, end, mask );

	//	assert( naviret != 1 );
	if( naviret == 0 ) return true;
	else return false;

	if( naviret == 2 )
	{
		return findSimplePath_Internal( path, start, end, mask );
	}
	else return false;
}

////////////////////////////////////////////////////////////
//基于四叉树的长路径寻径
////////////////////////////////////////////////////////////
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
int GameMaskMap::findLongPath( GameMaskPath &path, const Ogre::Point2D &start, const Ogre::Point2D &end, int mask  )
{
	// 保存下真正目标
	path.m_RealGoal.x = end.x;
	path.m_RealGoal.y = end.y;

	if ( m_QuadTree.m_vecAllNodes.empty() )
	{
		return 2;
	}

	Point2D scale(m_lMaskPixelWidth, m_lMaskPixelHeight);
	s_StartGrid.y = start.y / m_lMaskPixelHeight;
	s_StartGrid.x = start.x / m_lMaskPixelWidth;

	if( !canWalk(s_StartGrid.x, s_StartGrid.y, mask) ) 
	{
		return 1;
	}

	Point2D pathend(end);

	Point2D endgrid;
	endgrid.y = pathend.y / m_lMaskPixelHeight;
	endgrid.x = pathend.x / m_lMaskPixelWidth;

	if( !canWalk(endgrid.x, endgrid.y, mask) )
	{
		if( !findPointWalkable(pathend, mask) ) 
		{
			return 2;
		}

		endgrid.y = pathend.y / m_lMaskPixelHeight;
		endgrid.x = pathend.x / m_lMaskPixelWidth;
	}

	if( start == pathend ) 
	{
		return 3;
	}

	m_PathHeap.Init();
	m_ClosedNodesLong.resize(0);

	//将所有的四叉树节点设置为未被访问
	for( size_t i=0; i < m_QuadTree.m_vecAllNodes.size(); ++i)
	{
		m_QuadTree.m_vecAllNodes[i].bVisited = false;
	}
	//从四叉树中查找起始点和目标点所在节点
	int startindex = m_QuadTree.MapPixelToQtnode(start.x, start.y);
	int endIndex = m_QuadTree.MapPixelToQtnode(pathend.x, pathend.y);

	int endx = m_QuadTree.m_vecAllNodes[endIndex].px;
	int endy = m_QuadTree.m_vecAllNodes[endIndex].py;

	int startx = m_QuadTree.m_vecAllNodes[startindex].px;
	int starty = m_QuadTree.m_vecAllNodes[startindex].py;

	PathHeapNode node;
	node.from = node.index = endIndex;
	node.hval = node.aval = 0;
	m_PathHeap.PushNode(node);
	m_QuadTree.m_vecAllNodes[node.index].bVisited = true;

	bool bfind = false;
	int indexX = -1;
	int indexY = -1;
	while(m_PathHeap.HasNode())
	{
		m_PathHeap.PopNode(node);
		m_ClosedNodesLong.push_back( node );
		if( m_ClosedNodesLong.size() >= MAX_NAVIGRID_LONG ) break;

		if(node.index == startindex)
		{
			bfind = true;
			break;
		}
		else
		{
			QTNode* pQTNode = &( m_QuadTree.m_vecAllNodes[node.index] );
			QTNode* pQTNodeNb;
			int nCount = 0;
			int nbStart = 0;
			if( mask & maskMagic )
			{
				nCount	= pQTNode->nbMagicCount;
				nbStart = pQTNode->nbMagicStart;
			}else{
				nCount	= pQTNode->nbCount;
				nbStart = pQTNode->nbStart;
			}
			for(int i=0; i<nCount; ++i)
			{
				if( mask & maskMagic )
				{
					pQTNodeNb = &( m_QuadTree.m_vecAllNodes[m_QuadTree.m_vecNbMagicIndex[nbStart + i]] );
				}else{
					pQTNodeNb = &( m_QuadTree.m_vecAllNodes[m_QuadTree.m_vecNbIndex[nbStart + i]] );
				}

				unsigned int curindex = pQTNodeNb->ni;
				int aval = node.aval + abs(pQTNodeNb->px - pQTNode->px) + abs(pQTNodeNb->py - pQTNode->py);

				//如果该节点被访问过
				if(pQTNodeNb->bVisited)
				{
					std::vector<PathHeapNode>::iterator iter = m_PathHeap.m_AllNodes.begin();
					for( ; iter!=m_PathHeap.m_AllNodes.end(); iter++ )
					{
						if( iter->index == pQTNodeNb->ni ) break;
					}
					if(iter != m_PathHeap.m_AllNodes.end())
					{
						if( aval < iter->aval )
						{
							iter->aval = aval;
							iter->from = node.index;
							m_PathHeap.AdjustNode( iter );
						}
					}
					continue;
				}
				PathHeapNode tmpnode;
				tmpnode.from = node.index;
				tmpnode.index = pQTNodeNb->ni;
				tmpnode.aval = aval;
				tmpnode.hval = (abs(pQTNodeNb->px - startx) + abs(pQTNodeNb->py - starty)) * 2/3;
				m_PathHeap.PushNode( tmpnode );
				pQTNodeNb->bVisited = true;
			}
		}
	}

	if( bfind )
	{
		path.reset(this);

		path.addPathPoint( start.x, start.y );
		int n = 0;
		while(node.from != node.index)
		{
			++n;
			/*if( !checkPoint( path, Point2D( m_QuadTree.m_vecAllNodes[node.index].px, m_QuadTree.m_vecAllNodes[node.index].py ), mask ) )
			{*/
				path.addPathPoint( m_QuadTree.m_vecAllNodes[node.index].px, m_QuadTree.m_vecAllNodes[node.index].py );
			//}
			 
			assert( canWalkPixelCoord( m_QuadTree.m_vecAllNodes[node.index].px, m_QuadTree.m_vecAllNodes[node.index].py, mask ) );

			std::vector<PathHeapNode>::iterator iter = FindHeapNodeInArray(m_ClosedNodesLong, node.from);
			node = *iter;
		}
		/*if( !checkPoint( path, Point2D( endx, endy ), mask ) )
		{*/
			path.addPathPoint( endx, endy );
			path.addPathPoint( pathend.x, pathend.y );
		//}
		assert( path.m_Points.size() > 1 );

		cutBlockPath(path, mask);

		return 0;
	}
	return 2;
}

Point2D GameMaskMap::doMotion( const Point2D &start, const Point2D &delta )
{
	return start+delta;
}

//返回到下一个格子
static Point2D LineIntersectGrid( const Point2D &pt1, const Point2D &pt2, const Point2D &start, const Point2D &dp )
{
	/*
	int y = -1;
	if( dp.x > 0 ) y = dp.y*(pt2.x-start.x)/dp.x + start.y;
	else if( dp.x < 0 ) y = dp.y*(pt1.x-start.x)/dp.x + start.y;
	if( y>=pt1.y && y<=pt2.y )  //从边 x=sx或者x=ex出去这个格子
	{
	if( dp.x > 0 ) return Point2D(1, 0);
	else return Point2D(-1, 0);
	}
	else //从边 y=sy或者y=ey出去
	{
	if( dp.y > 0 ) return Point2D(0, 1);
	else return Point2D(0, -1);
	}
	*/

	int y1 = pt1.y*dp.x;
	int y2 = pt2.y*dp.x;
	if( dp.x > 0 )
	{
		int y = dp.y*(pt2.x-start.x) + start.y*dp.x;
		if( y == y2 ) return Point2D(1, 1);
		else if( y>=y1 && y<y2 ) return Point2D(1, 0);
	}
	else if( dp.x < 0 )
	{
		int y = dp.y*(pt1.x-start.x) + start.y*dp.x;

		if( y == y2 ) return Point2D(0, 1);
		else if( y>y2 && y<=y1 ) return Point2D(-1, 0);
	}

	if( dp.y > 0 ) return Point2D(0, 1);
	else return Point2D(0, -1);
}

bool GameMaskMap::lineOfSightTest(const Ogre::Point2D &start, const Ogre::Point2D &end, int mask/*=maskStop*/, unsigned int count /*= 0*/, Ogre::Point2D *result /*= NULL */)
{
	Point2D step( m_lMaskPixelWidth, m_lMaskPixelHeight );
	Point2D startgrid( start.x/m_lMaskPixelWidth, start.y/m_lMaskPixelHeight );
	Point2D endgrid( end.x/m_lMaskPixelWidth, end.y/m_lMaskPixelHeight );
	Point2D dp = end - start;

	Point2D curgrid = startgrid;
	Point2D lastgrid = startgrid;

	bool ThroughFlag = false;
	unsigned int ThroughCount = 0;
	while (true)
	{
		if (canWalk(curgrid.x, curgrid.y, mask))
		{
			ThroughFlag = false;
		}
		else
		{
			if (!ThroughFlag)
			{
				++ThroughCount;

				if (count < ThroughCount)
				{
					if (result != NULL)
					{
						result->x = lastgrid.x * m_lMaskPixelWidth;
						result->y = lastgrid.y * m_lMaskPixelHeight;
					}
					return false;
				}
			}
			ThroughFlag = true;
		}
		if (curgrid == endgrid)
		{
			break;
		}
		lastgrid = curgrid;
		Point2D pt1 = curgrid * step;
		curgrid += LineIntersectGrid( pt1, pt1+step, start, dp );
	}
	return true;
}

typedef struct {
	Point2D s_StartPt;
	Point2D s_dPt;
	Point2D s_CurGrid;
} GridPoint;

static void LineIntersectGrid(const Point2D *pt1, const Point2D *pt2, GridPoint *pstGP)
{
	int dx = pstGP->s_dPt.x;
	int y1 = pt1->y * dx;
	int y2 = pt2->y * dx;
	if( dx > 0 )
	{
		int y = pstGP->s_dPt.y*(pt2->x - pstGP->s_StartPt.x) + pstGP->s_StartPt.y*dx;
		if( y == y2 )
		{
			pstGP->s_CurGrid.x++;
			pstGP->s_CurGrid.y++;
			return;
		}
		if( y>=y1 && y<y2 )
		{
			pstGP->s_CurGrid.x++;
			return;
		}
	}
	else if( dx < 0 )
	{
		int y = pstGP->s_dPt.y*(pt1->x - pstGP->s_StartPt.x) + pstGP->s_StartPt.y*dx;
		if( y == y2 )
		{
			pstGP->s_CurGrid.y++;
			return;
		}
		if( y>y2 && y<=y1 )
		{
			pstGP->s_CurGrid.x--;
			return;
		}
	}

	if( pstGP->s_dPt.y > 0 ) pstGP->s_CurGrid.y++;
	else pstGP->s_CurGrid.y--;
}

bool CMapMask_IsPixelLineThrough(GameMaskMap *pmap, int lPixelX1, int lPixelY1, int lPixelX2, int lPixelY2, int mask)
{
	Point2D pt1, pt2;
	int grid_sx;
	int grid_sy;
	int grid_ex;
	int grid_ey;
	GridPoint stGP;

	stGP.s_StartPt.x = lPixelX1;
	stGP.s_StartPt.y = lPixelY1;
	stGP.s_dPt.x = lPixelX2 - lPixelX1;
	stGP.s_dPt.y = lPixelY2 - lPixelY1;

	grid_sx = lPixelX1/pmap->m_lMaskPixelWidth;
	grid_sy = lPixelY1/pmap->m_lMaskPixelHeight;
	grid_ex = lPixelX2/pmap->m_lMaskPixelWidth;
	grid_ey = lPixelY2/pmap->m_lMaskPixelHeight;

	stGP.s_CurGrid.x = grid_sx;
	stGP.s_CurGrid.y = grid_sy;
	if( !pmap->canWalk(grid_sx, grid_sy, mask) ) return false;
	while( stGP.s_CurGrid.x!=grid_ex || stGP.s_CurGrid.y!=grid_ey )
	{
		pt1.x = stGP.s_CurGrid.x * pmap->m_lMaskPixelWidth;
		pt1.y = stGP.s_CurGrid.y * pmap->m_lMaskPixelHeight;
		pt2.x = pt1.x + pmap->m_lMaskPixelWidth;
		pt2.y = pt1.y + pmap->m_lMaskPixelHeight;

		LineIntersectGrid(&pt1, &pt2, &stGP);
		if( !pmap->canWalk(stGP.s_CurGrid.x, stGP.s_CurGrid.y, mask) ) return false; 
	}
	return true;
}

bool GameMaskMap::lineOfSightTest1( const Point2D &start, const Point2D &end, int mask )
{
	return CMapMask_IsPixelLineThrough(this, start.x, start.y, end.x, end.y, mask );
}

bool GameMaskMap::lineOfSightTest2( const Point2D &start, const Point2D &end, int mask )
{
	int   dx,dy,dm,dn,m,n,k,u,v,l,sum;

	dx = end.x - start.x, dy = end.y - start.y;
	if(dx == 0)
	{
		u = start.y, v = end.y;
		if(dy < 0) u = end.y, v = start.y;
		for (l = u; l <= v; l ++)
		{
			if( !canWalkPixelCoord(end.x, l, mask) ) return false;
		}
		return true;
	}
	if(dy==0)
	{
		u = start.x, v = end.x;
		if(dx < 0) u = end.x,v = start.x;
		for(l = u; l <= v; l ++)
		{
			if( !canWalkPixelCoord(l, end.y, mask) ) return false;
		}
		return true;
	}

	dm = dn = 1;
	if(dx < 0) dx = -dx, dm = -1;
	if(dy < 0) dy = -dy, dn = -1;
	m = dx, n = dy, k = 1, u = start.x, v = start.y;
	if(dx<dy) m = dy, n = dx, k = dm, dm = dn, dn = k, k = 0, u = start.y, v = start.x;
	l = 0;
	sum = m;

	if( !canWalkPixelCoord(start.x, start.y, mask) ) return false;
	while (sum != 0)
	{
		sum--;
		l += n;
		u += dm;
		if(l >= m)
		{
			v += dn;
			l -= m;
		}
		if(k == 1)
		{
			if( !canWalkPixelCoord(u, v, mask) ) return false;
		}
		else
		{
			if( !canWalkPixelCoord(v, u, mask) ) return false;
		}
	}
	return true;
}

bool GameMaskMap::findPointWalkable( Point2D &point, int mask, int range )
{ 
	Point2D grid(point);
	coordPixel2Grid( grid.x, grid.y );

	if( canWalk(grid.x, grid.y, mask) ) return true;

	for( int i=1; i<=range; i++ )
	{
		int x, y;
		unsigned int flags = 0;
		int sx = grid.x - i; if( sx < 0 ) { sx = 0; flags |= 1; }
		int sy = grid.y - i; if( sy < 0 ) { sy = 0; flags |= 2; }
		int ex = grid.x + i; if( ex >= m_lWidthMasks ) { ex = m_lWidthMasks-1; flags |= 4; }
		int ey = grid.y + i; if( ey >= m_lHeightMasks ) { ey = m_lHeightMasks-1; flags |= 8; }

		if( !(flags&2) )
		{
			for( x=sx; x<=ex; x++ )
			{
				if( canWalk(x, sy, mask) ){ point.x=x, point.y=sy; goto find_end; }
			}
		}

		if( !(flags&8) )
		{
			for( x=sx; x<=ex; x++ )
			{
				if( canWalk(x, ey, mask) ){ point.x=x, point.y=ey; goto find_end; }
			}
		}

		if( !(flags&1) )
		{
			for( y=sy; y<=ey; y++ )
			{
				if( canWalk(sx, y, mask) ){ point.x=sx, point.y=y; goto find_end; }
			}
		}

		if( !(flags&4) )
		{
			for( y=sy; y<=ey; y++ )
			{
				if( canWalk(ex, y, mask) ){ point.x=ex, point.y=y; goto find_end; }
			}
		}
	}
	return false;

find_end:
	coordGrid2Pixel( point.x, point.y );
	return true;
}

struct stGoalGrid
{
	Point2D grid;
	int dirprior; //方向和目标方向越一致，优先级越大

	bool operator < ( stGoalGrid &rhs )
	{
		return dirprior > rhs.dirprior;
	}
};

bool GameMaskMap::findSimplePath_Internal( GameMaskPath &path, const Point2D &start, const Point2D &end, int mask )
{
	int range = 4;
	stGoalGrid gridbuf[100];
	int count = 0;
	if( start == end ) return false;

	Point2D startgrid( start.x/m_lMaskPixelWidth, start.y/m_lMaskPixelHeight );
	for( int y=startgrid.y-range; y<=startgrid.y+range; y++ )
	{
		if( y<0 || y>=m_lHeightMasks ) continue;

		if( startgrid.x-range >= 0 ) gridbuf[count++].grid = Point2D( startgrid.x-range, y );
		if( startgrid.x+range <m_lWidthMasks ) gridbuf[count++].grid = Point2D( startgrid.x+range, y );
	}

	range--;
	for( int x=startgrid.x-range; x<=startgrid.x+range; x++ )
	{
		if( x<0 || x>=m_lWidthMasks ) continue;

		if( startgrid.y-range >= 0 ) gridbuf[count++].grid = Point2D( x, startgrid.y-range );
		if( startgrid.y+range < m_lHeightMasks ) gridbuf[count++].grid = Point2D( x, startgrid.y+range );
	}

	if( count == 0 ) return false;
	assert( count < 100 );

	//对每个goalgrid, 计算寻路优先级
	Point2D dp = end - start;
	Point2D step( m_lMaskPixelWidth, m_lMaskPixelHeight );
	Point2D halfstep( m_lMaskPixelWidth/2, m_lMaskPixelHeight/2 );

	int i;
	for( i=0; i<count; i++ )
	{
		Point2D goalpt = gridbuf[i].grid * step + halfstep - start;
		gridbuf[i].dirprior = goalpt.x*dp.x + goalpt.y*dp.y;
	}

	std::sort( &gridbuf[0], &gridbuf[count] );
	while( count > 0 )
	{
		if( gridbuf[count-1].dirprior < 0 ) count--;
		else break;
	}

	for( i=0; i<count; i++ )
	{
		Point2D goalpt = gridbuf[i].grid * step + halfstep;
		if( findNaviPath_Internal(path, start, goalpt, mask) == 0 ) return true;
	}

	return false;
}

bool GameMaskMap::findDirFaceMask( Point2D &point, const Point2D &center, int radius, int mask )
{
	const int NUM_SEG = 18; //把圆周分为多少等分
	const float TWOPI = 3.1415926f * 2;

	for( int i=0; i<NUM_SEG; i++ )
	{
		float angle = i*TWOPI/NUM_SEG;
		int x = int(center.x + radius*cos(angle));
		int y = int(center.y + radius*sin(angle));
		if( !canWalkPixelCoord(x, y, mask) )
		{
			point.x = x;
			point.y = y;
			return true;
		}
	}
	return false;
} 

void GameMaskMap::createQuadTree()
{
	//创建四叉树
	m_QuadTree.Create(m_pMaskData, m_lWidthMasks, m_lHeightMasks, m_lMaskPixelWidth);

	//int iSize = m_QuadTree.m_vecNbIndex.size();
	//int iCount = 0;
	//for ( int i = 0; i < m_QuadTree.m_vecAllNodes.size(); i++ )
	//{
	//	if ( m_QuadTree.m_vecAllNodes[i].bLeaf 
	//		&& ( m_QuadTree.m_vecAllNodes[i].value == MASK_WALK ) )
	//	{
	//		iCount ++;
	//	}	
	//}

	//char buf[256];
	//sprintf( buf, "地图非空节点个数:%d, 总节点数:%d \n", iCount, iSize );
	//OutputDebugString( buf );

}

bool GameMaskMap::checkPoint( GameMaskPath & path, const Ogre::Point2D &next, int mask )
{
	if( path.getPathPoint( path.getNumPoint() - 1 ) == next )
	{
		return false;
	}

	int startindex = m_QuadTree.MapPixelToQtnode(path.getPathPoint( path.getNumPoint() - 1 ).x,
		path.getPathPoint( path.getNumPoint() - 1 ).y );
	int endIndex = m_QuadTree.MapPixelToQtnode(next.x, next.y);
	if( startindex != endIndex )
	{
		return false;
	}
	
	
	if( lineOfSightTest( path.getPathPoint( path.getNumPoint() - 1 ), next, mask ) )
	{
		return true;
	}
	
	GameMaskPath smallPath;
	findPath( smallPath, path.getPathPoint( path.getNumPoint() - 1 ), next, mask );
	for( size_t i = 1; i < smallPath.getNumPoint(); i++ )
	{
		path.addPathPoint( smallPath.getPathPoint( i ).x, smallPath.getPathPoint( i ).y );
	}
	return false;
}

void GameMaskMap::setDynMsk(int x, int y, int rx, int ry, int angle, int m)
{
	bool enter = true;
	if (m < 0)
	{
		m = -m;
		enter = false;
	}
	int mask = 0;
	if (m & DYN_MSK_STOP)
	{
		mask |= DYN_MASK_STOP;
	}
	if (m & DYN_MSK_MAGIC)
	{
		mask |= DYN_MASK_MAGIC;
	}
	int tx = m_lWidthMasks;
	int ty = m_lHeightMasks;
	int px = m_lMaskPixelWidth;
	int py = m_lMaskPixelHeight;

	int x1 = x - rx;
	int x2 = x + rx;
	int y1 = y - ry;
	int y2 = y + ry;
	RESPOS center;
	center.X = x;
	center.Y = y;
	RESPOS pos[4];
	pos[0].X = x1;
	pos[0].Y = y1;
	pos[1].X = x1;
	pos[1].Y = y2;
	pos[2].X = x2;
	pos[2].Y = y2;
	pos[3].X = x2;
	pos[3].Y = y1;
	for (int i = 0; i < 4; ++i)
	{
		pos[i] = z_rotate_pos(center, 90 - angle, pos[i]);
		if (pos[i].X < 0)
		{
			pos[i].X = 0;
		}
		else if (pos[i].X > tx * px)
		{
			pos[i].X = tx * px;
		}
		if (pos[i].Y < 0)
		{
			pos[i].Y = 0;
		}
		else if (pos[i].Y > ty * py)
		{
			pos[i].Y = ty * py;
		}
	}
	int i1 = pos[0].X;
	int i2 = pos[0].X;
	int j1 = pos[0].Y;
	int j2 = pos[0].Y;
	for (int i = 0; i < 4; ++i)
	{
		if (i1 > pos[i].X)
		{
			i1 = pos[i].X;
		}
		if (i2 < pos[i].X)
		{
			i2 = pos[i].X;
		}
		if (j1 > pos[i].Y)
		{
			j1 = pos[i].Y;
		}
		if (j2 < pos[i].Y)
		{
			j2 = pos[i].Y;
		}
	}
	i1 /= px;
	i2 /= px;
	j1 /= py;
	j2 /= py;

	for (int i = i1; i <= i2; ++i)
	{
		for (int j = j1; j <= j2; ++j)
		{
			RESPOS p;
			p.X = i * px;
			p.Y = j * py;
			if (InsideConvexPolygon(4, pos, p))
			{
				if (mask & DYN_MASK_STOP)
				{
					if (enter)
					{
						++m_pDyWalkMaskData[j * ty + i];
						m_pMaskData[j * ty + i] |= DYN_MASK_STOP;
						//EffectManage::getSingleton().playEffect(i * px, j * py, 0, "effect\\entity\\1021.ent");
					}
					else
					{
						if (m_pDyWalkMaskData[j * ty + i] > 0)
						{
							if (0 == --m_pDyWalkMaskData[j * ty + i])
							{
								m_pMaskData[j * ty + i] &= ~DYN_MASK_STOP;
							}
						}
					}
				}

				if (mask & DYN_MASK_MAGIC)
				{
					if (enter)
					{
						++m_pDyMagicMaskData[j * ty + i];
						m_pMaskData[j * ty + i] |= DYN_MASK_MAGIC;
					}
					else
					{
						if (m_pDyMagicMaskData[j * ty + i] > 0)
						{
							if (0 == --m_pDyMagicMaskData[j * ty + i])
							{
								m_pMaskData[j * ty + i] &= ~DYN_MASK_MAGIC;
							}
						}
					}
				}
			}
		}
	}
	m_lastChangeTime = timeGetTime();
}

void GameMaskMap::UpdateSearchNodes()
{
	if (m_lastChangeTime && timeGetTime() > m_lastChangeTime)
	{
		//m_lastChangeTime = 0;
		//m_QuadTree.UpdateSearchNodes(m_pMaskData, m_lWidthMasks, m_lHeightMasks, m_lMaskPixelWidth);
	}
}

void GameMaskMap::cutBlockPath( GameMaskPath &path, int mask )
{
	if (mask & MASK_STOP)
	{
		mask |= DYN_MASK_STOP;
	}
	if (mask & MASK_MAGIC)
	{
		mask |= DYN_MASK_MAGIC;
	}
	for (size_t i = 0; i < path.getNumPoint() - 1; i++)
	{
		if (!lineOfSightTest(path.getPathPoint(i), path.getPathPoint(i + 1), mask))
		{
			path.limitPathPoint(i + 1);
			break;
		}
	}
}
