#pragma once
//////////////////////////////////////////////////////////////////////////
// 区域地图
//////////////////////////////////////////////////////////////////////////
#include "GameDBData.h"
#include "GameUIMsg.h"
#include "GameDefineTolua.h"
#include "GameMsgPrint.h"
#include "resdb_meta.h"
#include "AutoFindLineManager.h"
#include "MultiMapAutoFind.h"
#include <map>
#include <vector>
#include <utility>
#include <set>

//tolua_begin
#pragma pack( 1 )
struct tagPworldDef_ToLua
{
	unsigned int PworldID;                       
	char Name[RES_NAME_LEN];             
	unsigned char Type;                            	/*  Ver.15  Bind Macrosgroup:PWORLD_TYPE,*/
	unsigned char NumEveryDay;                     	/*  Ver.15 每日的次数限制，0表示不限制 */
	unsigned char NotEnterAgain;                   	/*  Ver.15 是否不可再次进入 0 可再次进入 1 不可再次进入 */
	unsigned char NPCNum;                          	/*  Ver.15 副本中有多少npc，用于资源分配 */
	unsigned short MonNum;                          	/*  Ver.15 副本中会同时有多少怪物，用于资源分配 */
	unsigned short Timeout;                         	/*   副本时间限制，单位分钟，0表示没有限制 */
	unsigned int InitMapID;                       	/*   进入副本时的入口地图id */
	RESPOS InitMapPos;                      	/*   进入副本时的入口地图位置 */
	int InitMapDir;                      	/*   进入副本时的入口地图角色朝向 */
	unsigned char InitCameraDir;                   	/*  Ver.81 切换地图后镜头朝向,可以填0~180，单位是2度，0表示正北 */
	unsigned char DynType;                         	/*  Ver.57 是否是动态副本 0 不是 1是 */
	int CreateStatusID;                  	/*  Ver.107 创建副本时,角色添加的状态 */
	unsigned char CreateStatusLevel;               	/*  Ver.107 创建副本时,角色添加的状态等级 */
	int CreateStatusTime;                	/*  Ver.107 创建副本时,角色添加的状态时间,单位:秒 */
	PWORLDDYNATTR DynAttr;                         	/*  Ver.118 动态副本难度修正 */
	unsigned char ClanLimit;                       	/*  Ver.122 是否工会限制0-否1-是 */
	unsigned int ContriLimit;                     	/*  Ver.122 是否工会贡献度限制0-否 */
	unsigned int CtrlFlag;                        	/*  Ver.127 控制标记 */
	unsigned char MinLv;                           	/*  Ver.127 进入FB最小等级 0--没有要求 */
	unsigned char MaxLv;                           	/*  Ver.127 进入FB最大等级 0--没要求 */
	unsigned short PlayerNum;                       	/*  Ver.128 进入人数限制 0--没要求 */
	unsigned short PworldNum;                       	/*  Ver.128 可创建数量限制 0--没要求 */
	unsigned short PlayerInTime;                    	/*  Ver.128 进入副本时间限制单位秒 0--没要求 */
	unsigned int PlayerModeID;                    	/*  Ver.129 进入副本后人物模型需要改变的ID  */
	unsigned char CampNum;                         	/*  Ver.191 阵营数量|0表示没有阵营  */
	unsigned char PolyType;                        	/*  Ver.191 活动类型|1表示阵营模式  2表示乱斗模式 */
	unsigned char OutLineNotIn;                    	/*  Ver.191 掉线重新登录是否在FB里面|0表示在FB   1表示不在FB */
	PWORLDAWARD AwardList[MAX_AWARD_NUM];        	/*  Ver.191 优胜奖章物品和数量 */
	int LifeAwardItemID;                 			/*  Ver.203 生存奖章物品ID */
	int EnterTime;                       	/*  Ver.261 副本开启多久之后不能进入|单位:分 */
	int ItemEnterID;                     	/*  Ver.282 使用道具进入副本的道具ID */
	int ItemEnterNum;                    	/*  Ver.282 使用道具进入副本次数限制 */
	int RedoLimitTime;                   	/*  Ver.352 重复刷副本的时间间隔限制单位秒 */
	int BattleID;                        	/*  Ver.499 PolyType 是战场，则此值表示对应的战场定义ID */
	int CreateQueuerNum;                 	/*  Ver.499 创建副本需求队列人数限制 */
	int DiffPworldID;                    	/*  Ver.785 副本属于哪一个难度副本 */
	int OffExpMul;                       	/*  Ver.911 副本未参加的补偿经验系数 */
	char IsGetVig;                        	/*  Ver.1378 副本能否获取活力值1-是 0-否 */
	int EnterUseVig;                     	/*  Ver.1378 进入副本队长消耗多少活力 */
	int StrongMapID;                     	/*  Ver.1559 次副本属于哪个据点 */
	int NumEveryWeek;                    	/*  Ver.1564 每周次数限制 */
	int AutoPworldUseType;               	/*  Ver.1618 自动传送消耗货币类型 */
	int AutoPworldUseVal;                	/*  Ver.1618 自动传送消耗货币消耗 */
	int AutoPworldNum;                   	/*  Ver.1619 自动传送队伍人数限制 */
	int ReadyTime;                       	/*  Ver.1902 准备时间单位秒，到达准备时间进入特定脚本 */
	int MatchType;                       	/*  Ver.2165 匹配类型|0 自动匹配 1手动匹配 */
	int MinPlayerNum;                    	/*  Ver.2165 最少人数 */
	int ApplyUnitType;                   	/*  Ver.2172 报名单位类型|0 玩家 1公会 */
	int BuddyVal;                        	/*  Ver.2362 亲密度值 */
};

struct tagStallPoint_ToLua
{
	tagResPos_ToLua StallPos;                        	/*   地图上的摊位点 */
	unsigned int Radius;                          	/*   摊位点半径 */
};

struct tagTotemPoint_ToLua
{
	tagResPos_ToLua TotemPos;                        	/*   地图上的打坐图腾 */
	unsigned int Radius;                          	/*   打坐图腾有效半径 */
};

struct tagMapLimit_ToLua
{
	unsigned char PK;                              	/*   0 禁止pk 1 可以pk */
	unsigned char PKMode;                          	/*  Ver.174 地图Pk模式限制0(不限制) 1 组队 2公会 3阵营 4全体 */
};

struct tagEnterLimit_ToLua
{
	unsigned char Type;                            	/*   进入地图的限制 */
	int Val;                            
};

struct tagMapDef_ToLua
{
	unsigned int MapID;                          
	unsigned int ReferMapID;                      	/*  Ver.41  */
	int ReferMapIDPos;                   	/*  Ver.41  */
	char Name[RES_NAME_LEN];             
	unsigned int EffectID;                        	/*   地图效果ID */
	unsigned int ReviveMap;                       	/*  Ver.15 手动复活地图id */
	tagMapLimit_ToLua MapLimit;                        	/*  Ver.15 地图限制 */
	unsigned char Broadcast;                       	/*   1 全地图广播，针对玩家在有些副本地图中需要看到所有怪物的情况，很影响效率，尽量少用 */
	tagStallPoint_ToLua StallPointList[MAX_MAP_STALL_POINT]; 	/*  Ver.75  */
	tagTotemPoint_ToLua SitTotemList[MAX_SIT_TOTEM];     	/*  Ver.83  */
	unsigned char MapArea;                         	/*  Ver.77 地图所在洲 */
	tagResPos_ToLua ReferAreaFrom;                   	/*  Ver.88 地图占用区域矩阵 */
	tagResPos_ToLua ReferAreaTo;                     	/*  Ver.88 地图占用区域矩阵 */
	int IconID;                          	/*  Ver.97 图标id */
	char MapDesc[SHORT_DESC_LEN];         	/*  Ver.97  */
	tagResPos_ToLua RevivePs[REVIVEPOS_NUM];         	/*  Ver.191 复活点 */
	unsigned char NotDiePunish;                    	/*  Ver.209 是否不受死亡惩罚 1-不受死亡惩罚  0-受 */
	int CtrlFlag;                        	/*  Ver.407 地图标记， 1 地图可以骑乘机械 2 地图复活，进入均不获得保护状态 */
	int IsSafeMap;                       	/*  Ver.465 是否安全地图 */
	int MapLine;                         	/*  Ver.484 线路，需ReferMapID 相同，则视为多线的几个地图 */
	RESPOS TransPos[CAMP_TRANS_NUM];        /*  Ver.508 战场传送点 */
	int DontList;                        	/*  Ver.517 不在客户端的地图列表显示的地图 */
	int UnrealLayer;                     	/*  Ver.1186 幻境多少层进入 */
	int EnterTime;                       	/*  Ver.1186 玩家能在此地图呆多久|单位:分 */
	int Span;                            	/*  Ver.1362 跨服野外地图，0 不是 1是 */
	tagEnterLimit_ToLua EnterLimit[MAX_MAP_ENTER_LIMIT]; 	/*  Ver.1421  */
	int MapWidth;                        	/*  Ver.1502  */
	int MapHeight;                       	/*  Ver.1502  */
	int AppDropItemID;                   	/*  Ver.1884  */
	int MapMachine[MAP_MACHINE_CAMP_NUM]; 	/*  Ver.1902 地图强制阵营相关机械 */
	int RepairMap;                       	/*  Ver.1933 自动修理Map */
	int RepairNpc;                       	/*  Ver.1933 自动修理Npc */
	int MapType;                         	/*  Ver.1968  Bind Macrosgroup:MAP_TYPE,*/
};

struct tagMapOreList_ToLua
{
	int MonsterID;                      
	int Num;                            
	int RefreshTime;                     	/*  Ver.1043 刷新时间单位秒 */
};

struct tagMapOreDef_ToLua
{
	unsigned int MapID;                           	/*   NPC所在地图 */
	tagMapOreList_ToLua OreList[MAX_MAP_ORE_LIST];       	/*   地图上矿石数量 */
	int MachineView;                     	/*  Ver.1247 可见矿，为1则乘坐在机械上可以看到的矿石 */
};

#pragma pack()

struct tagMapActorInfo
{
	int		m_nId;
	int		m_nMapId;
	int		m_nPosition;			// 此为玩家在组队中的索引
	int		m_nRealX;				// 此为玩家在实际场景坐标系中的X坐标
	int		m_nRealY;				// 此为玩家在实际场景坐标系中的Y坐标
	int		m_nPlayerX;				// 此为玩家相对于最终经过变换所得地图的左上角顶点且位于最终变换所得地图坐标系FinalCoord的X坐标
	int		m_nPlayerY;				// 此为玩家相对于最终经过变换所得地图的左上角顶点且位于最终变换所得地图坐标系FinalCoord的Y坐标
	char	m_szName[NAME_LEN];		// 队员名称
	int		m_nOffLineTime;			// 表示队员是否在线
	bool	m_bShow;				// 是否显示
	char	m_szMapDesc[NAME_LEN];	// npc 功能
	int	m_nSpanMapCamp;                     	/*  Ver.2475 处于跨服野外地图上的哪个阵营 */
};

struct tagMapTextureInfo
{
	int		m_MapValidNormalizeTextWidth;	// 此为1024*1024标准纹理地图上有效区域的宽度
	int		m_MapValidNormalizeTextHeight;	// 此为1024*1024标准纹理地图上有效区域的高度
	int		m_MapTextTop;					// 此为从完整纹理裁剪出的纹理相对于完整纹理的Y偏移
	int		m_MapTextLeft;					// 此为从完整纹理裁剪出的纹理相对于完整纹理的X偏移
	int		m_MapClicpTextWidth;			// 此为从完整纹理裁剪出的纹理的宽度
	int		m_MapClicpTextHeight;			// 此为从完整纹理裁剪出的纹理的高度
	int		m_MapWorldTextWidth;			// 此为从完整纹理宽度
	int		m_MapWorldTextHeight;			// 此为从完整纹理高度
	int		m_MapViewWidth;					// 此为地图的视口区域宽度
	int		m_MapViewHeight;				// 此为地图的视口区域高度
};

struct Pair
{
	int first;
	int second;

	Pair( )
	{
		first	= -1;
		second	= -1;
	}
	Pair( int firstValue, int secondValue )
	{
		first	= firstValue;
		second	= secondValue;
	}
};

struct tagMinMapTeamMebPos
{
	int nX;
	int nY;
};

struct tagMapInfo
{
	int		m_nMapID;
	int		m_nMapArea;
	char	m_szMapName[RES_NAME_LEN];
	int		m_nReferMapID;
	int		DontList;
};

enum
{
	MAP_AREA_ASIA			= 1,	// 亚洲
	MAP_AREA_EUROPE			= 2,	// 欧洲
	MAP_AREA_NORTHAMERICA	= 4,	// 北美洲
	MAP_AREA_SOUTHAMERICA	= 8,	// 南美洲
	MAP_AREA_AFRICA			= 16,	// 非洲
	MAP_AREA_BIRTH			= 32,	// 新手村
	MAP_AREA_CITY			= 64,	// 主城
	MAP_AREA_COPY			= 4096,	// 副本
	MAP_AREA_All			= 8192	// 全部
}; 

enum
{
	NORMALIZE_TEX_WIDTH		= 1024,
	NORMALIZE_TEX_HEIGHT	= 1024,
};

enum
{
	CLIENT_GAME_PWORLD_MAPID = GAME_PWORLD_MAPID,
};

const tagPworldDef_ToLua*	getPworldDef( int nPworldId );
const char* getMapName( int nMapId );
const tagMapDef_ToLua* getMapDef( int nMapId );
void writeErrorLog( const char* pBuffer );
//tolua_end

class GameNpcEnterView : public IGameMsgPrint
{
public:
	GameNpcEnterView(void);
	~GameNpcEnterView(void);

	virtual void npcEnterView( int id );
	virtual void npcLeaveView( int id );
};

class GameMapControl//tolua_export
{//tolua_export
public:
	GameMapControl(void);
	~GameMapControl(void);
	
	// 重置信息
	void	resetMapControl();
	//tolua_begin	
	// 让人物朝北
	void				faceToNorth();
	tagMaxMapInfo&		updateMapInfo();
	tagMapActorInfo&	updateMapCaptainInfo();
	tagMapActorInfo&	updateMapTeamMemberInfo( int nIndex );
	tagMapActorInfo&	updateMinMapTargetInfo();
	
	void setView( int nWidth, int nHeight );
	void setMapZoom( float fZoom );
	Pair getMouseCurPos( int& x, int& y );
	void mapMoveto( int x, int y );// 此为当前点击位置与视口的偏移值,偏移值以像素为单位
	// 参数: nDestRealx, nDestRealY为目标点在实际游戏场景中的坐标
	int linkMoveto( int nDestMapId, int nDestRealX, int nDestRealY );
	int linkMoveto( int nDestMapId, int nNpcResId );
	const tagPassMapPath& getLastFindDest()
	{
		return m_lastFindDest;
	}

	void follow( int nDestMapId, int nDestRealx, int nDestRealY, int nDist = 0 );
	void DrawPoint( const char* szLineName );
	// TODO
	Pair getPointScreenCoord( int& x, int& y, const char* pszTextureName, int nScale );
	Pair getPointCoordOnTex( int x, int y );
	// TODO

	void setMapTextRect( int nWidth, int nHeight, int nLeftOffset, int nTopOffset, 
		int nCompleteTextWidth, int nCompleteTextHeight, int nClicpTextWidth, 
		int nClicpTextHeight, int nViewWidth, int nViewHeight );
	void updateMovemap( int x, int y );	
	void endMoveMap(); 

	void getMainPlayerPos( int& x, int& y );
	void setMapInfo( int width, int height, int mapId );

	void startMoveMap( int x, int y );
	void setMapWithPlayer( bool flag );
	tagMapActorInfo& getTargetPos(int x, int y);
	const char* getMapName( int nMapId );
	tagMinMapTeamMebPos& getPosition( int nSrcX, int nSrcY, int nDesX, int nDesY, int nDist );

	int  getMapID()					
	{ 
		return m_nMapId; 
	}
	int  getMapWidth()				
	{ 
		return m_nMapWidth;  
	}
	int  getMapHeight()				
	{ 
		return m_nMapHeight; 
	}		

	tagMapTextureInfo& getMapTexInfo()
	{
		return m_MapTexInfo;
	}

	// 获得npc类型
	int		getNpcRace( int nResId );
	// 判断npc类型
	bool	isNpcRaceType( int nResId, int nType );
	// 判断是否能够进行自动寻路
	bool	canAutoFindPath();

	void drawClanMemberPoint( const char* pszTextureName, const char* clanFrameName, const char* enemyFrameName );

	void SetMapSize( int mapID );
	//tolua_end
protected:
	tagPassMapPath		m_lastFindDest;
	tagMaxMapInfo		m_MapInfo;
	int					m_ViewWidth;		// 此为经过最终变换后所得的地图的宽度
	int					m_ViewHeight;		// 此为经过最终变换后所得的地图的高度
	tagMapTextureInfo	m_MapTexInfo;		// 此为地图对应的纹理信息		

	int				m_nMapWidth;		// 此为地图对应的实际场景的宽度
	int				m_nMapHeight;		// 此为地图对应的实际场景的高度
	int				m_nMapId;			// 此为Refer Map ID

	bool				m_bBeenMove;		// 指示主角色当前是否处于移动状态,若为true,则表示处于移动状态,若为false,则表示处于静止状态
	bool				m_bMoveMap;			// 是否按住拖动地图
	int					m_nMoveOffX;		// 此为主角色的X位移
	int					m_nMoveOffY;		// 此为主角色的Y位移
	int					m_nStartX;			// 此为主角色移动前所处的位置的X坐标
	int					m_nStartY;			// 此为主角色移动前所处的位置的Y坐标

	tagMapActorInfo  m_TargetPos;
	GameNpcEnterView*	m_pNpcEnterView;
	tagMapActorInfo  m_ActorInfo;
	tagMapActorInfo  m_CaptainInfo;
	tagMapActorInfo  m_SelectTargetInfo;
	tagMapActorInfo	m_TeamMemberInfo;
	std::map<int, tagMapActorInfo> m_mapMinMapActorInfo;
	tagMinMapTeamMebPos m_TeamMebPos;
};//tolua_export

bool operator< ( const tagSceneAreaDef_ToLua& lhs, const tagSceneAreaDef_ToLua& rhs );

typedef std::vector<tagMapActorInfo>		MAPNPCINFOVECTOR;
typedef std::vector<tagSceneAreaDef_ToLua>	MAPSCENEAREAVECTOR;
typedef std::vector<tagMapInfo>				MAPINFOVECTOR;
typedef std::map<int, MAPINFOVECTOR>::iterator		MAPINFOMAP_ITER;
enum
{
	DEFAULT_LINE_NO = 0,
	MIN_COPY_ID = 10000,
};

class GameMapShower//tolua_export
{//tolua_export
public:
	GameMapShower();
	~GameMapShower();

	//tolua_begin
	void calculateMapSize( int nMapID );
	GameMapControl* GetMaxMapContal() 
	{ 
		return m_pMaxMapContal;
	}

	GameMapControl* GetMinMapContal() 
	{
		return m_pMinMapContal; 
	}

	// 获取指定地图NPC的接口集
	// 取Map tex ID
	bool	isInMapNpcDataBase( int nMapId );
	int		getMapNpcCount( int nMapId );
	const	tagMapActorInfo& getMapNpcInfo( int nId, int nIndex );
	
	void	clearMapNpc( int nMapId );
	void	updateMapNpcInfo( int nDefId, int nRealX, int nRealY );
	void	deleteMapNpcInfo( int nDefId );
	const	tagMapActorInfo* getNpcInfoById( int nMapId, int Id );
	const	tagMapActorInfo* getNpcInfoByName( int nMapId, const char* pszName );

	// 获取指定地图怪物区域的接口集
	// 取Map tex ID
	int		getMapSceneAreaCount( int nMapId );
	const	tagSceneAreaDef_ToLua& getMapSceneAreaInfo( int nMapId, int nIndex );
	int		getMapSceneAreainfocounts( int nMapId);

	// 用于根据地图ID归并地图上的NPC和怪物区域信息
	bool	isMultiLineMap( int nRealMapID );
	void	fillMultiLineMap();
	void	mergeMapNpc();
	void	mergeMapSceneArea();
	int		getRealMapId()				
	{ 
		return m_nRealMapId; 
	}

	// 当数据库中ReferMapID为空的时候,RealMapID和GameMapControl中的MapID值是一样的
	void setRealMapId(int mapId);

	int getLineNo()
	{
		return m_nLineNo;
	}

	// 用于获取游戏中所有地图的信息
	const tagMapInfo* getMapInfoByName( const char* pszName );
	const tagMapInfo& getMapInfoByIndex( int nMapArea, int nIndex );
	const tagMapInfo* getMapInfoByID( int mapId );
	// 获取某个洲的地图数量
	unsigned int getMapInfoNum( int nMapArea );
	// 获取游戏中所有地图的数量
	int getMapNum();
	tagMapInfo* getMapInfoByIndex( int nIndex );
	// 根据地图的Tex ID获取地图实际场景的尺寸
	Pair&	getMapSize( int nMapID );
	MultiMapAutoFind&	getMultiMapFindControl();
	bool	addMapNpc( int nId );
	void	deleteMapNpc( int nId );
	AutoFindLineManager* getAutoLine();

	void CheckAutoPathFind();
	const tagMapOreDef_ToLua* getMapOreDef( int nId );
	void ClearPVPMapNPC( int mapid );
	//tolua_end
	std::vector<MAPDEF>&	getAllMapDef()
	{
		return m_mapVector;
	}
	void	initMapInfo();
	void	update( float dtick );
private:
	MultiMapAutoFind		m_multiMapAutoFind;
	GameMapControl*			m_pMinMapContal;
	GameMapControl*			m_pMaxMapContal;

	std::vector<MAPNPC>					m_mapNpcVector;		//存储整个游戏地图NPC,这是在进入地图前执行的,读取所有地图上的NPC
	std::vector<tagSceneAreaDef>		m_mapAllSceneArea;  //存储整个游戏地图的区域分类,这是在进入地图前执行的,读取所有地图上的区域分类
	std::map<int,MAPSCENEAREAVECTOR>	m_mapSceneAreaMap;	// 这是按照地图来归类区域分类,仅仅在进入一张地图的时候,读取某个地图上的区域分类
	int									m_nRealMapId;
	int									m_nLineNo;
	AutoFindLineManager					m_AutoLine;
	std::vector<MAPDEF>					m_mapVector;
	std::map<int, MAPINFOVECTOR>		m_mapInfoMap;		// 键值为场景范围
	std::map<int, Pair>					m_mapSize;
	std::map<int,MAPNPCINFOVECTOR>		m_allMapNpcMap;
	std::map<int,MAPSCENEAREAVECTOR>	m_allMapSceneAreaMap;
	std::set<int>						m_multiLineMap;
};//tolua_export

inline unsigned int GameMapShower::getMapInfoNum( int nMapArea )
{
	return	(int)m_mapInfoMap[nMapArea].size();
}

inline AutoFindLineManager* GameMapShower::getAutoLine()
{
	return &m_AutoLine;
}


