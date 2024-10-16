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
	bool			m_bInCopy;	//是否在FB中
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

// TODO: 为路径写一个class A，用于保存自动寻路经过所有的点，以以下结构为类A的数据保存容器
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

	// TODO: 将当前路径保存到文件上
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
	// 是否是同样的目的地
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
	std::deque<tagPassMapPath>			m_multiMapPath;		// 当前跨地图自动寻路所经过路径的集合
	std::vector<tagWorldMapSearchDef>	m_mapSearchVec;		// 相邻地图集合
	std::vector<PATH>					m_shortestPathes;
	std::map<int, std::vector<int>>		m_nearMap;			// 相邻地图归类集合
	tagPassMapPath						m_curFindTarget;
	unsigned int						m_uLastCallTime;

	int									m_AutoMoveMapId;
	int									m_AutoMoveX;
	int									m_AutoMoveY;
	unsigned int						m_AutoMoveTime;
};//tolua_export