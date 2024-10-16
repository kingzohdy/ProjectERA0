#include "GameActorManager.h"
#include "AutoFindLineManager.h"
#include "GameMaskMap.h"
#include "GameMainPlayer.h"
#include "GameManager.h"
#include "OgreRect.h"
#include "GameMaxMap.h"
#include "../UILib/ui_framemgr.h"
#include <algorithm>
#include <utility>
#include <functional>
#include <math.h>

extern FrameManager *g_pFrameMgr;
extern void translateMaxmapPoint( int& x, int& y, int scale );

Ogre::Point2D* getLastCornerpoint();

int calPointsDis( const Ogre::Point2D& first, const Ogre::Point2D& second )
{
	Ogre::Point2D tmpPoint	= first - second;
	return (int)sqrt( static_cast<float>( tmpPoint.x * tmpPoint.x + tmpPoint.y * tmpPoint.y) );
}

// 计算2点之间距离的2次方值
int calPointsQuadraticDis( const Ogre::Point2D& lhs, const Ogre::Point2D& rhs )
{
	Ogre::Point2D tmpPoint	= lhs - rhs;
	return (int)( static_cast<float>( tmpPoint.x * tmpPoint.x + tmpPoint.y * tmpPoint.y ) );
}

void AutoFindLineManager::init( GameMaskPath& path )
{
	// 先对已有的点集合进行清空操作
	clearPath();
	// 保存当前主角行走的路线图，以防过长路径被截取后无法获取主角当前行走的路线
	m_fScreenScaleX	= g_pFrameMgr->GetScreenScaleX();
	m_fScreenScaleY	= g_pFrameMgr->GetScreenScaleY();
#ifdef _DEBUG
	assert( path.getNumPoint() >= 2 && "不合法的路径!!!" );
#else
	if ( path.getNumPoint() < 2 )
	{
		return;
	}
#endif

	Ogre::Point2D keyPoint;
	GameMapControl* pMapControl = GameManager::getSingleton().getGameMapShower()->GetMaxMapContal();
	// 先保存路径关键点的屏幕坐标集合
	for ( size_t i = 0, size = path.getNumPoint(); i < size; ++i )
	{
		keyPoint = path.getPathPoint( i );
		m_pathSceneKeyPoints.push_back( keyPoint );
		translateMaxmapPoint( keyPoint.x, keyPoint.y, 1 );
		m_pathKeyPoints.push_back( keyPoint );
	}

	// 计算出整条绘制路径，路径中的点以实际场景坐标为单位
	calculatePath( 0 );
	// 保存当前主角自动寻路的目标点
	m_lastAutoDestScenePoint	= path.getPathPoint( path.getNumPoint() - 1 );
	Ogre::Point2D	tmpPoint( m_lastAutoDestScenePoint );
	translateMaxmapPoint( tmpPoint.x, tmpPoint.y, 1 );
	m_lastAutoDestScreenPoint	= tmpPoint;
	// 再将所有的顶点坐标从实际场景坐标转换为屏幕坐标
	for ( std::deque<Ogre::Point2D>::iterator iter = m_autoFindPath.begin(), end = m_autoFindPath.end(); iter != end; ++iter )
	{
		translateMaxmapPoint( iter->x, iter->y, 1 );
	}
}

void AutoFindLineManager::clearPath()
{
	m_autoFindPath.clear();
	m_pathKeyPoints.clear();
	m_pathSceneKeyPoints.clear();
}

void AutoFindLineManager::updatePath( Ogre::Point2D playerPoint, int nCurPosition )
{
	if ( empty() )
	{
		return;
	}

	// 若主角到达终点
	if ( nCurPosition >= (int)m_pathKeyPoints.size() - 1 )
	{
		clearPath();
		return;
	}

	// 将主角的坐标点转换为屏幕中的相对坐标系
	translateMaxmapPoint( playerPoint.x, playerPoint.y, 10 );
	// 以目标点为参考点，移除多余的点
	removePassedPoint( playerPoint, nCurPosition );
}

// Pre: 调用者要保证传递给这个接口的2点之间的距离一定要大于2点之间绘制的最小距离
void AutoFindLineManager::linearInterpolate( const Ogre::Point2D& firstPoint, const Ogre::Point2D& endPoint )
{
	// 分段存储每段路径之间的平滑点
	int				nAngle	= FaceAngleBetween( firstPoint.x, firstPoint.y, endPoint.x,	endPoint.y );
	Ogre::Point2D	smoothPoint;

	int nWidth = GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->getMapWidth();
	const int FIX_POINT_DISTANC = STAND_FIX_POINT_DISTANCE * nWidth / STANDARD_MAP_SIZE;
	// 先计算行动管理器中存储的数据
	int nLength		= calPointsDis( firstPoint, endPoint );
	int nNumSegment	= (int)floor( static_cast<float>( nLength ) / static_cast<float>( FIX_POINT_DISTANC ) );
	assert( nNumSegment > 0 && "意外的计算结果!!!" ); 
	// 确保意外情况
	nNumSegment = ( nNumSegment == 0 ) ? 1 : nNumSegment;
	for ( int nSmoothIndex = 0, size = nNumSegment - 1; nSmoothIndex < size; ++nSmoothIndex )
	{
		smoothPoint.x	= (int)( firstPoint.x + (double)( FIX_POINT_DISTANC * ( nSmoothIndex + 1 ) )
			* cos( ( double )( nAngle ) * 3.14159265 / 180.0f ) );
		smoothPoint.y	= (int)( firstPoint.y - (double)( FIX_POINT_DISTANC * ( nSmoothIndex + 1 ) )
			* sin( ( double )( nAngle ) * 3.14159265 / 180.0f ) );
		m_autoFindPath.push_back( smoothPoint );
	}
}

// 根据主角在路径中所处的位置计算绘制路径的点集合图
void AutoFindLineManager::calculatePath( int nBeginPathIndex )
{
	int nWidth = GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->getMapWidth();
	const int FIX_POINT_DIST = STAND_FIX_POINT_DISTANCE * nWidth / STANDARD_MAP_SIZE;
	// 先存储所有的拐角点
	int nFirstIndex	 = nBeginPathIndex;
	int nSecondIndex = nBeginPathIndex + 1;
	for ( size_t i = nBeginPathIndex, size = m_pathSceneKeyPoints.size() - 1; i < size; ++i )
	{	
		const Ogre::Point2D& firstPoint	 = m_pathSceneKeyPoints[nFirstIndex];
		const Ogre::Point2D& secondPoint = m_pathSceneKeyPoints[nSecondIndex];

		// 当2个拐角点距离小于指定的最小间距的时候,则不取当前的拐角点
		if ( calPointsDis( firstPoint, secondPoint ) < FIX_POINT_DIST )
		{
			++nSecondIndex;
			continue;
		}

		m_autoFindPath.push_back( firstPoint );
		linearInterpolate( firstPoint, secondPoint );
		m_autoFindPath.push_back( secondPoint );

		nFirstIndex = nSecondIndex;
		++nSecondIndex;
	}
}

void AutoFindLineManager::removePassedPoint( const Ogre::Point2D& playerPoint, int nCurPosition )
{
	if (nCurPosition < 0 || nCurPosition >= m_pathKeyPoints.size() - 1)
	{
		return;
	}
	const Ogre::Point2D& firstKeyPoint	= m_pathKeyPoints[nCurPosition];
	const Ogre::Point2D& secondKeyPoint	= m_pathKeyPoints[nCurPosition+1];
	int vector1_x = firstKeyPoint.x - playerPoint.x;
	int vector1_y = firstKeyPoint.y - playerPoint.y;
	bool bFirst	  = true;
	// 若路径中的点相对于主角当前寻路的目标点的长度大于目标点到主角当前位置的长度，则移除路径中的点
	int nPlayerDis = ( playerPoint.x - secondKeyPoint.x ) * ( playerPoint.x - secondKeyPoint.x ) + 
		( playerPoint.y - secondKeyPoint.y ) * ( playerPoint.y - secondKeyPoint.y );
	for ( AUTO_PATH::iterator iter = m_autoFindPath.begin(); iter != m_autoFindPath.end(); )
	{
		if ( isPointInPath( firstKeyPoint, secondKeyPoint, *iter ) )
		{
			int nPointDis = ( iter->x - secondKeyPoint.x ) * ( iter->x - secondKeyPoint.x ) + 
				( iter->y - secondKeyPoint.y ) * ( iter->y - secondKeyPoint.y );
			// 检查当前检查的点前面是否还有其他点，若有则表示前面误差造成一些点没清楚
			if ( bFirst )
			{
				// 检查当前在路径的点前面是否有点没清除
				if ( iter != m_autoFindPath.begin() )
				{
					m_autoFindPath.erase( m_autoFindPath.begin(), iter );
				}
			}

			bFirst = false;
			if ( nPlayerDis <= nPointDis )
			{
				iter = m_autoFindPath.erase( iter );
				continue;
			}
		}

		++iter;
	}
}

bool AutoFindLineManager::isPointInPath( const Ogre::Point2D& pathBeg, const Ogre::Point2D& pathEnd, const Ogre::Point2D& point )
{
	return point.x >= min( pathBeg.x, pathEnd.x ) && point.x <= max( pathBeg.x, pathEnd.x ) && 
		point.y >= min( pathBeg.y, pathEnd.y ) && point.y <= max( pathBeg.y, pathEnd.y ); 
}