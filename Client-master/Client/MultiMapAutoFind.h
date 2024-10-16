#pragma once

#include <map>
#include <vector>
#include <deque>
#include <list>
#include <utility>
#include <stack>

class MainGamePlayer;

//tolua_begin
struct tagPos
{
	int X;                              
	int Y; 
};

union	MapTransPoint
{
	tagPos	m_Pos;
	int		m_nNpcResId;
};

struct tagPassMapPath
{
	int				m_nMapId;
	bool			m_bNpcTran;
	MapTransPoint	m_Dest;
	bool			m_bInCopy;	//�Ƿ���FB��
};

struct tagWorldMapSearchDef_ToLua
{
	unsigned int FromMapID;                      
	unsigned int ToMapID;                        
	unsigned int NpcID;                          
	int X;                              
	int Y;                              
};

tagWorldMapSearchDef_ToLua*	getWorldMapSearchDef( int nFromMapID );
//tolua_end

// TODO: Ϊ·��дһ��class A�����ڱ����Զ�Ѱ·�������еĵ㣬�����½ṹΪ��A�����ݱ�������
typedef std::vector<std::pair<int, int> >					PATH;
typedef std::vector<std::pair<int, int> >::iterator			PATH_ITER;
typedef std::vector<std::pair<int, int> >::const_iterator	CONST_PATH_ITER;
typedef std::vector<std::pair<int, int> >::pointer			PATH_POINTER;
typedef	std::pair<int, int>									MAP_POINT;

class MultiMapAutoFind//tolua_export
{//tolua_export
public:
	
	MultiMapAutoFind();
	void reset();

	//tolua_begin
	int		beginAutoFind( int nSrcMapID, int nDestMapID, int nDestRealX, int nDestRealY, int nDist = 0 );
	int		beginAutoFind( int nSrcMapID, int nDestMapID, int nNpcResId );
	void	loadShortestPaths( int nBenginMapId );
	void	continueNextMapAuto( MainGamePlayer* pMainplayer );
	void	CheckArriveTarget( MainGamePlayer* pMainplayer );
	void	stopMultiFind();
	unsigned int	getLastAutoFindTime(){ return m_uLastCallTime; }
	tagWorldMapSearchDef_ToLua*	getWorldMapSearchDef( int nFromMapID );
	//tolua_end

	// TODO: ����ǰ·�����浽�ļ���
	tagWorldMapSearchDef*	getMapInfo( int nFromMapID, int nIndex );
	void	savePathToFile();
	void	update();

	void	SetAutoMove(int mapId, int x, int y);
	void	StopAutoMove();
	bool	IsAutoMoving();
	void	FinishAutoMove();

	int		getNextMapNum(int nSrcMapID);
	int		getNextMapInfo(int nSrcMapID, int nIndex);

private:
	// �Ƿ���ͬ����Ŀ�ĵ�
	bool	isSameDestination( int nNpcResID );
	//bool	isSameDestination( int nDestRealX, int nDestRealY );
	bool	isSubPath( const PATH&	subPath, const PATH& parentPath );
	void	trimPath( const PATH&	newPath );
	bool	isInPath( int nMapID, const PATH& path );
	PATH_ITER	findMap( int nMapID, PATH& path );
	void	generateShortestPath( int nSrcMapID, int nDestMapID );
	void	mergeNearMap();
	bool	succesConstructPath( PATH& path, MAP_POINT& nCurPathEndInfo, 
									std::stack<MAP_POINT>& pathBuffer, std::stack<MAP_POINT>& branchBuffer );

private:
	std::deque<tagPassMapPath>			m_multiMapPath;		// ��ǰ���ͼ�Զ�Ѱ·������·���ļ���
	std::vector<tagWorldMapSearchDef>	m_mapSearchVec;		// ���ڵ�ͼ����
	std::vector<PATH>					m_shortestPathes;
	std::map<int, std::vector<int>>		m_nearMap;			// ���ڵ�ͼ���༯��
	tagPassMapPath						m_curFindTarget;
	unsigned int						m_uLastCallTime;

	int									m_AutoMoveMapId;
	int									m_AutoMoveX;
	int									m_AutoMoveY;
	unsigned int						m_AutoMoveTime;
};//tolua_export