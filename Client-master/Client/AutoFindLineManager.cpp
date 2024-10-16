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

// ����2��֮������2�η�ֵ
int calPointsQuadraticDis( const Ogre::Point2D& lhs, const Ogre::Point2D& rhs )
{
	Ogre::Point2D tmpPoint	= lhs - rhs;
	return (int)( static_cast<float>( tmpPoint.x * tmpPoint.x + tmpPoint.y * tmpPoint.y ) );
}

void AutoFindLineManager::init( GameMaskPath& path )
{
	// �ȶ����еĵ㼯�Ͻ�����ղ���
	clearPath();
	// ���浱ǰ�������ߵ�·��ͼ���Է�����·������ȡ���޷���ȡ���ǵ�ǰ���ߵ�·��
	m_fScreenScaleX	= g_pFrameMgr->GetScreenScaleX();
	m_fScreenScaleY	= g_pFrameMgr->GetScreenScaleY();
#ifdef _DEBUG
	assert( path.getNumPoint() >= 2 && "���Ϸ���·��!!!" );
#else
	if ( path.getNumPoint() < 2 )
	{
		return;
	}
#endif

	Ogre::Point2D keyPoint;
	GameMapControl* pMapControl = GameManager::getSingleton().getGameMapShower()->GetMaxMapContal();
	// �ȱ���·���ؼ������Ļ���꼯��
	for ( size_t i = 0, size = path.getNumPoint(); i < size; ++i )
	{
		keyPoint = path.getPathPoint( i );
		m_pathSceneKeyPoints.push_back( keyPoint );
		translateMaxmapPoint( keyPoint.x, keyPoint.y, 1 );
		m_pathKeyPoints.push_back( keyPoint );
	}

	// �������������·����·���еĵ���ʵ�ʳ�������Ϊ��λ
	calculatePath( 0 );
	// ���浱ǰ�����Զ�Ѱ·��Ŀ���
	m_lastAutoDestScenePoint	= path.getPathPoint( path.getNumPoint() - 1 );
	Ogre::Point2D	tmpPoint( m_lastAutoDestScenePoint );
	translateMaxmapPoint( tmpPoint.x, tmpPoint.y, 1 );
	m_lastAutoDestScreenPoint	= tmpPoint;
	// �ٽ����еĶ��������ʵ�ʳ�������ת��Ϊ��Ļ����
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

	// �����ǵ����յ�
	if ( nCurPosition >= (int)m_pathKeyPoints.size() - 1 )
	{
		clearPath();
		return;
	}

	// �����ǵ������ת��Ϊ��Ļ�е��������ϵ
	translateMaxmapPoint( playerPoint.x, playerPoint.y, 10 );
	// ��Ŀ���Ϊ�ο��㣬�Ƴ�����ĵ�
	removePassedPoint( playerPoint, nCurPosition );
}

// Pre: ������Ҫ��֤���ݸ�����ӿڵ�2��֮��ľ���һ��Ҫ����2��֮����Ƶ���С����
void AutoFindLineManager::linearInterpolate( const Ogre::Point2D& firstPoint, const Ogre::Point2D& endPoint )
{
	// �ֶδ洢ÿ��·��֮���ƽ����
	int				nAngle	= FaceAngleBetween( firstPoint.x, firstPoint.y, endPoint.x,	endPoint.y );
	Ogre::Point2D	smoothPoint;

	int nWidth = GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->getMapWidth();
	const int FIX_POINT_DISTANC = STAND_FIX_POINT_DISTANCE * nWidth / STANDARD_MAP_SIZE;
	// �ȼ����ж��������д洢������
	int nLength		= calPointsDis( firstPoint, endPoint );
	int nNumSegment	= (int)floor( static_cast<float>( nLength ) / static_cast<float>( FIX_POINT_DISTANC ) );
	assert( nNumSegment > 0 && "����ļ�����!!!" ); 
	// ȷ���������
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

// ����������·����������λ�ü������·���ĵ㼯��ͼ
void AutoFindLineManager::calculatePath( int nBeginPathIndex )
{
	int nWidth = GameManager::getSingleton().getGameMapShower()->GetMaxMapContal()->getMapWidth();
	const int FIX_POINT_DIST = STAND_FIX_POINT_DISTANCE * nWidth / STANDARD_MAP_SIZE;
	// �ȴ洢���еĹսǵ�
	int nFirstIndex	 = nBeginPathIndex;
	int nSecondIndex = nBeginPathIndex + 1;
	for ( size_t i = nBeginPathIndex, size = m_pathSceneKeyPoints.size() - 1; i < size; ++i )
	{	
		const Ogre::Point2D& firstPoint	 = m_pathSceneKeyPoints[nFirstIndex];
		const Ogre::Point2D& secondPoint = m_pathSceneKeyPoints[nSecondIndex];

		// ��2���սǵ����С��ָ������С����ʱ��,��ȡ��ǰ�Ĺսǵ�
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
	// ��·���еĵ���������ǵ�ǰѰ·��Ŀ���ĳ��ȴ���Ŀ��㵽���ǵ�ǰλ�õĳ��ȣ����Ƴ�·���еĵ�
	int nPlayerDis = ( playerPoint.x - secondKeyPoint.x ) * ( playerPoint.x - secondKeyPoint.x ) + 
		( playerPoint.y - secondKeyPoint.y ) * ( playerPoint.y - secondKeyPoint.y );
	for ( AUTO_PATH::iterator iter = m_autoFindPath.begin(); iter != m_autoFindPath.end(); )
	{
		if ( isPointInPath( firstKeyPoint, secondKeyPoint, *iter ) )
		{
			int nPointDis = ( iter->x - secondKeyPoint.x ) * ( iter->x - secondKeyPoint.x ) + 
				( iter->y - secondKeyPoint.y ) * ( iter->y - secondKeyPoint.y );
			// ��鵱ǰ���ĵ�ǰ���Ƿ��������㣬�������ʾǰ��������һЩ��û���
			if ( bFirst )
			{
				// ��鵱ǰ��·���ĵ�ǰ���Ƿ��е�û���
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