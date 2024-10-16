#pragma once
#include "OgreRect.h"
#include "IAutoFind.h"
#include "GameMaskMap.h"
#include <utility>
#include <deque>
#include <vector>

class AutoFindLineManager : public IAutoFind { //tolua_export
public:
	typedef	std::deque< Ogre::Point2D >				AUTO_PATH;
	typedef std::deque< Ogre::Point2D >::iterator	POINTDEQUE_ITER;

	enum { STAND_FIX_POINT_DISTANCE = 800, STANDARD_MAP_SIZE = 51200 };
	AutoFindLineManager::AutoFindLineManager() : m_lastAutoDestScreenPoint( -1, -1 ),
		m_lastAutoDestScenePoint( -1, -1 )
	{}
	// 初始化拐角点路径和平滑点路径
	void init( GameMaskPath& path );

	// 清除所有路径顶点数据
	void clearPath();

	// 更新自动寻路的路径信息
	void updatePath( Ogre::Point2D playerPoint, int nCurPosition );

	Ogre::Point2D	getlastAutoDestScenePoint();
	Ogre::Point2D&	getlastAutoDestScreenPoint();
	bool			empty() const;

	AUTO_PATH*	getPathPoints()
	{
		return &m_autoFindPath;
	}

private:
	// helper

	// 线性插值2点之间的路线点集合
	void	linearInterpolate( const Ogre::Point2D& firstPoint, const Ogre::Point2D& endPoint );
	// 计算自动寻路路线图
	void	calculatePath( int nBeginPathIndex );
	// 移除自动寻路图中主角已经走过的多余的点
	void	removePassedPoint( const Ogre::Point2D& playerPoint, int nCurPosition );
	// 判断某个点是否在指定路径上
	bool	isPointInPath( const Ogre::Point2D& pathBeg, const Ogre::Point2D& pathEnd, const Ogre::Point2D& point );

private:
	// 最新存储数据结构
	// 此为保存自动寻路路径的点集合
	AUTO_PATH					m_autoFindPath;
	std::vector<Ogre::Point2D>	m_pathKeyPoints;
	std::vector<Ogre::Point2D>	m_pathSceneKeyPoints;
	// 主角实际的行走路线
	float						m_fScreenScaleX;
	float						m_fScreenScaleY;
	// 主角最近一次自动寻路点
	Ogre::Point2D				m_lastAutoDestScreenPoint;
	Ogre::Point2D				m_lastAutoDestScenePoint;
};//tolua_export

inline Ogre::Point2D AutoFindLineManager::getlastAutoDestScenePoint()
{
	return	m_lastAutoDestScenePoint;
}

inline Ogre::Point2D& AutoFindLineManager::getlastAutoDestScreenPoint()
{
	return m_lastAutoDestScreenPoint;
}

inline bool AutoFindLineManager::empty() const
{
	return m_autoFindPath.empty();
}
