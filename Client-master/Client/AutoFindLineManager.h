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
	// ��ʼ���սǵ�·����ƽ����·��
	void init( GameMaskPath& path );

	// �������·����������
	void clearPath();

	// �����Զ�Ѱ·��·����Ϣ
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

	// ���Բ�ֵ2��֮���·�ߵ㼯��
	void	linearInterpolate( const Ogre::Point2D& firstPoint, const Ogre::Point2D& endPoint );
	// �����Զ�Ѱ··��ͼ
	void	calculatePath( int nBeginPathIndex );
	// �Ƴ��Զ�Ѱ·ͼ�������Ѿ��߹��Ķ���ĵ�
	void	removePassedPoint( const Ogre::Point2D& playerPoint, int nCurPosition );
	// �ж�ĳ�����Ƿ���ָ��·����
	bool	isPointInPath( const Ogre::Point2D& pathBeg, const Ogre::Point2D& pathEnd, const Ogre::Point2D& point );

private:
	// ���´洢���ݽṹ
	// ��Ϊ�����Զ�Ѱ··���ĵ㼯��
	AUTO_PATH					m_autoFindPath;
	std::vector<Ogre::Point2D>	m_pathKeyPoints;
	std::vector<Ogre::Point2D>	m_pathSceneKeyPoints;
	// ����ʵ�ʵ�����·��
	float						m_fScreenScaleX;
	float						m_fScreenScaleY;
	// �������һ���Զ�Ѱ·��
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
