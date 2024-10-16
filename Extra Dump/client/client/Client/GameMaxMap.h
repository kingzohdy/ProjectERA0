#pragma once
//////////////////////////////////////////////////////////////////////////
// �����ͼ
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
	unsigned char NumEveryDay;                     	/*  Ver.15 ÿ�յĴ������ƣ�0��ʾ������ */
	unsigned char NotEnterAgain;                   	/*  Ver.15 �Ƿ񲻿��ٴν��� 0 ���ٴν��� 1 �����ٴν��� */
	unsigned char NPCNum;                          	/*  Ver.15 �������ж���npc��������Դ���� */
	unsigned short MonNum;                          	/*  Ver.15 �����л�ͬʱ�ж��ٹ��������Դ���� */
	unsigned short Timeout;                         	/*   ����ʱ�����ƣ���λ���ӣ�0��ʾû������ */
	unsigned int InitMapID;                       	/*   ���븱��ʱ����ڵ�ͼid */
	RESPOS InitMapPos;                      	/*   ���븱��ʱ����ڵ�ͼλ�� */
	int InitMapDir;                      	/*   ���븱��ʱ����ڵ�ͼ��ɫ���� */
	unsigned char InitCameraDir;                   	/*  Ver.81 �л���ͼ��ͷ����,������0~180����λ��2�ȣ�0��ʾ���� */
	unsigned char DynType;                         	/*  Ver.57 �Ƿ��Ƕ�̬���� 0 ���� 1�� */
	int CreateStatusID;                  	/*  Ver.107 ��������ʱ,��ɫ��ӵ�״̬ */
	unsigned char CreateStatusLevel;               	/*  Ver.107 ��������ʱ,��ɫ��ӵ�״̬�ȼ� */
	int CreateStatusTime;                	/*  Ver.107 ��������ʱ,��ɫ��ӵ�״̬ʱ��,��λ:�� */
	PWORLDDYNATTR DynAttr;                         	/*  Ver.118 ��̬�����Ѷ����� */
	unsigned char ClanLimit;                       	/*  Ver.122 �Ƿ񹤻�����0-��1-�� */
	unsigned int ContriLimit;                     	/*  Ver.122 �Ƿ񹤻ṱ�׶�����0-�� */
	unsigned int CtrlFlag;                        	/*  Ver.127 ���Ʊ�� */
	unsigned char MinLv;                           	/*  Ver.127 ����FB��С�ȼ� 0--û��Ҫ�� */
	unsigned char MaxLv;                           	/*  Ver.127 ����FB���ȼ� 0--ûҪ�� */
	unsigned short PlayerNum;                       	/*  Ver.128 ������������ 0--ûҪ�� */
	unsigned short PworldNum;                       	/*  Ver.128 �ɴ����������� 0--ûҪ�� */
	unsigned short PlayerInTime;                    	/*  Ver.128 ���븱��ʱ�����Ƶ�λ�� 0--ûҪ�� */
	unsigned int PlayerModeID;                    	/*  Ver.129 ���븱��������ģ����Ҫ�ı��ID  */
	unsigned char CampNum;                         	/*  Ver.191 ��Ӫ����|0��ʾû����Ӫ  */
	unsigned char PolyType;                        	/*  Ver.191 �����|1��ʾ��Ӫģʽ  2��ʾ�Ҷ�ģʽ */
	unsigned char OutLineNotIn;                    	/*  Ver.191 �������µ�¼�Ƿ���FB����|0��ʾ��FB   1��ʾ����FB */
	PWORLDAWARD AwardList[MAX_AWARD_NUM];        	/*  Ver.191 ��ʤ������Ʒ������ */
	int LifeAwardItemID;                 			/*  Ver.203 ���潱����ƷID */
	int EnterTime;                       	/*  Ver.261 �����������֮���ܽ���|��λ:�� */
	int ItemEnterID;                     	/*  Ver.282 ʹ�õ��߽��븱���ĵ���ID */
	int ItemEnterNum;                    	/*  Ver.282 ʹ�õ��߽��븱���������� */
	int RedoLimitTime;                   	/*  Ver.352 �ظ�ˢ������ʱ�������Ƶ�λ�� */
	int BattleID;                        	/*  Ver.499 PolyType ��ս�������ֵ��ʾ��Ӧ��ս������ID */
	int CreateQueuerNum;                 	/*  Ver.499 ����������������������� */
	int DiffPworldID;                    	/*  Ver.785 ����������һ���Ѷȸ��� */
	int OffExpMul;                       	/*  Ver.911 ����δ�μӵĲ�������ϵ�� */
	char IsGetVig;                        	/*  Ver.1378 �����ܷ��ȡ����ֵ1-�� 0-�� */
	int EnterUseVig;                     	/*  Ver.1378 ���븱���ӳ����Ķ��ٻ��� */
	int StrongMapID;                     	/*  Ver.1559 �θ��������ĸ��ݵ� */
	int NumEveryWeek;                    	/*  Ver.1564 ÿ�ܴ������� */
	int AutoPworldUseType;               	/*  Ver.1618 �Զ��������Ļ������� */
	int AutoPworldUseVal;                	/*  Ver.1618 �Զ��������Ļ������� */
	int AutoPworldNum;                   	/*  Ver.1619 �Զ����Ͷ����������� */
	int ReadyTime;                       	/*  Ver.1902 ׼��ʱ�䵥λ�룬����׼��ʱ������ض��ű� */
	int MatchType;                       	/*  Ver.2165 ƥ������|0 �Զ�ƥ�� 1�ֶ�ƥ�� */
	int MinPlayerNum;                    	/*  Ver.2165 �������� */
	int ApplyUnitType;                   	/*  Ver.2172 ������λ����|0 ��� 1���� */
	int BuddyVal;                        	/*  Ver.2362 ���ܶ�ֵ */
};

struct tagStallPoint_ToLua
{
	tagResPos_ToLua StallPos;                        	/*   ��ͼ�ϵ�̯λ�� */
	unsigned int Radius;                          	/*   ̯λ��뾶 */
};

struct tagTotemPoint_ToLua
{
	tagResPos_ToLua TotemPos;                        	/*   ��ͼ�ϵĴ���ͼ�� */
	unsigned int Radius;                          	/*   ����ͼ����Ч�뾶 */
};

struct tagMapLimit_ToLua
{
	unsigned char PK;                              	/*   0 ��ֹpk 1 ����pk */
	unsigned char PKMode;                          	/*  Ver.174 ��ͼPkģʽ����0(������) 1 ��� 2���� 3��Ӫ 4ȫ�� */
};

struct tagEnterLimit_ToLua
{
	unsigned char Type;                            	/*   �����ͼ������ */
	int Val;                            
};

struct tagMapDef_ToLua
{
	unsigned int MapID;                          
	unsigned int ReferMapID;                      	/*  Ver.41  */
	int ReferMapIDPos;                   	/*  Ver.41  */
	char Name[RES_NAME_LEN];             
	unsigned int EffectID;                        	/*   ��ͼЧ��ID */
	unsigned int ReviveMap;                       	/*  Ver.15 �ֶ������ͼid */
	tagMapLimit_ToLua MapLimit;                        	/*  Ver.15 ��ͼ���� */
	unsigned char Broadcast;                       	/*   1 ȫ��ͼ�㲥������������Щ������ͼ����Ҫ�������й�����������Ӱ��Ч�ʣ��������� */
	tagStallPoint_ToLua StallPointList[MAX_MAP_STALL_POINT]; 	/*  Ver.75  */
	tagTotemPoint_ToLua SitTotemList[MAX_SIT_TOTEM];     	/*  Ver.83  */
	unsigned char MapArea;                         	/*  Ver.77 ��ͼ������ */
	tagResPos_ToLua ReferAreaFrom;                   	/*  Ver.88 ��ͼռ��������� */
	tagResPos_ToLua ReferAreaTo;                     	/*  Ver.88 ��ͼռ��������� */
	int IconID;                          	/*  Ver.97 ͼ��id */
	char MapDesc[SHORT_DESC_LEN];         	/*  Ver.97  */
	tagResPos_ToLua RevivePs[REVIVEPOS_NUM];         	/*  Ver.191 ����� */
	unsigned char NotDiePunish;                    	/*  Ver.209 �Ƿ��������ͷ� 1-���������ͷ�  0-�� */
	int CtrlFlag;                        	/*  Ver.407 ��ͼ��ǣ� 1 ��ͼ������˻�е 2 ��ͼ������������ñ���״̬ */
	int IsSafeMap;                       	/*  Ver.465 �Ƿ�ȫ��ͼ */
	int MapLine;                         	/*  Ver.484 ��·����ReferMapID ��ͬ������Ϊ���ߵļ�����ͼ */
	RESPOS TransPos[CAMP_TRANS_NUM];        /*  Ver.508 ս�����͵� */
	int DontList;                        	/*  Ver.517 ���ڿͻ��˵ĵ�ͼ�б���ʾ�ĵ�ͼ */
	int UnrealLayer;                     	/*  Ver.1186 �þ����ٲ���� */
	int EnterTime;                       	/*  Ver.1186 ������ڴ˵�ͼ�����|��λ:�� */
	int Span;                            	/*  Ver.1362 ���Ұ���ͼ��0 ���� 1�� */
	tagEnterLimit_ToLua EnterLimit[MAX_MAP_ENTER_LIMIT]; 	/*  Ver.1421  */
	int MapWidth;                        	/*  Ver.1502  */
	int MapHeight;                       	/*  Ver.1502  */
	int AppDropItemID;                   	/*  Ver.1884  */
	int MapMachine[MAP_MACHINE_CAMP_NUM]; 	/*  Ver.1902 ��ͼǿ����Ӫ��ػ�е */
	int RepairMap;                       	/*  Ver.1933 �Զ�����Map */
	int RepairNpc;                       	/*  Ver.1933 �Զ�����Npc */
	int MapType;                         	/*  Ver.1968  Bind Macrosgroup:MAP_TYPE,*/
};

struct tagMapOreList_ToLua
{
	int MonsterID;                      
	int Num;                            
	int RefreshTime;                     	/*  Ver.1043 ˢ��ʱ�䵥λ�� */
};

struct tagMapOreDef_ToLua
{
	unsigned int MapID;                           	/*   NPC���ڵ�ͼ */
	tagMapOreList_ToLua OreList[MAX_MAP_ORE_LIST];       	/*   ��ͼ�Ͽ�ʯ���� */
	int MachineView;                     	/*  Ver.1247 �ɼ���Ϊ1������ڻ�е�Ͽ��Կ����Ŀ�ʯ */
};

#pragma pack()

struct tagMapActorInfo
{
	int		m_nId;
	int		m_nMapId;
	int		m_nPosition;			// ��Ϊ���������е�����
	int		m_nRealX;				// ��Ϊ�����ʵ�ʳ�������ϵ�е�X����
	int		m_nRealY;				// ��Ϊ�����ʵ�ʳ�������ϵ�е�Y����
	int		m_nPlayerX;				// ��Ϊ�����������վ����任���õ�ͼ�����ϽǶ�����λ�����ձ任���õ�ͼ����ϵFinalCoord��X����
	int		m_nPlayerY;				// ��Ϊ�����������վ����任���õ�ͼ�����ϽǶ�����λ�����ձ任���õ�ͼ����ϵFinalCoord��Y����
	char	m_szName[NAME_LEN];		// ��Ա����
	int		m_nOffLineTime;			// ��ʾ��Ա�Ƿ�����
	bool	m_bShow;				// �Ƿ���ʾ
	char	m_szMapDesc[NAME_LEN];	// npc ����
	int	m_nSpanMapCamp;                     	/*  Ver.2475 ���ڿ��Ұ���ͼ�ϵ��ĸ���Ӫ */
};

struct tagMapTextureInfo
{
	int		m_MapValidNormalizeTextWidth;	// ��Ϊ1024*1024��׼�����ͼ����Ч����Ŀ��
	int		m_MapValidNormalizeTextHeight;	// ��Ϊ1024*1024��׼�����ͼ����Ч����ĸ߶�
	int		m_MapTextTop;					// ��Ϊ����������ü�����������������������Yƫ��
	int		m_MapTextLeft;					// ��Ϊ����������ü�����������������������Xƫ��
	int		m_MapClicpTextWidth;			// ��Ϊ����������ü���������Ŀ��
	int		m_MapClicpTextHeight;			// ��Ϊ����������ü���������ĸ߶�
	int		m_MapWorldTextWidth;			// ��Ϊ������������
	int		m_MapWorldTextHeight;			// ��Ϊ����������߶�
	int		m_MapViewWidth;					// ��Ϊ��ͼ���ӿ�������
	int		m_MapViewHeight;				// ��Ϊ��ͼ���ӿ�����߶�
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
	MAP_AREA_ASIA			= 1,	// ����
	MAP_AREA_EUROPE			= 2,	// ŷ��
	MAP_AREA_NORTHAMERICA	= 4,	// ������
	MAP_AREA_SOUTHAMERICA	= 8,	// ������
	MAP_AREA_AFRICA			= 16,	// ����
	MAP_AREA_BIRTH			= 32,	// ���ִ�
	MAP_AREA_CITY			= 64,	// ����
	MAP_AREA_COPY			= 4096,	// ����
	MAP_AREA_All			= 8192	// ȫ��
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
	
	// ������Ϣ
	void	resetMapControl();
	//tolua_begin	
	// �����ﳯ��
	void				faceToNorth();
	tagMaxMapInfo&		updateMapInfo();
	tagMapActorInfo&	updateMapCaptainInfo();
	tagMapActorInfo&	updateMapTeamMemberInfo( int nIndex );
	tagMapActorInfo&	updateMinMapTargetInfo();
	
	void setView( int nWidth, int nHeight );
	void setMapZoom( float fZoom );
	Pair getMouseCurPos( int& x, int& y );
	void mapMoveto( int x, int y );// ��Ϊ��ǰ���λ�����ӿڵ�ƫ��ֵ,ƫ��ֵ������Ϊ��λ
	// ����: nDestRealx, nDestRealYΪĿ�����ʵ����Ϸ�����е�����
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

	// ���npc����
	int		getNpcRace( int nResId );
	// �ж�npc����
	bool	isNpcRaceType( int nResId, int nType );
	// �ж��Ƿ��ܹ������Զ�Ѱ·
	bool	canAutoFindPath();

	void drawClanMemberPoint( const char* pszTextureName, const char* clanFrameName, const char* enemyFrameName );

	void SetMapSize( int mapID );
	//tolua_end
protected:
	tagPassMapPath		m_lastFindDest;
	tagMaxMapInfo		m_MapInfo;
	int					m_ViewWidth;		// ��Ϊ�������ձ任�����õĵ�ͼ�Ŀ��
	int					m_ViewHeight;		// ��Ϊ�������ձ任�����õĵ�ͼ�ĸ߶�
	tagMapTextureInfo	m_MapTexInfo;		// ��Ϊ��ͼ��Ӧ��������Ϣ		

	int				m_nMapWidth;		// ��Ϊ��ͼ��Ӧ��ʵ�ʳ����Ŀ��
	int				m_nMapHeight;		// ��Ϊ��ͼ��Ӧ��ʵ�ʳ����ĸ߶�
	int				m_nMapId;			// ��ΪRefer Map ID

	bool				m_bBeenMove;		// ָʾ����ɫ��ǰ�Ƿ����ƶ�״̬,��Ϊtrue,���ʾ�����ƶ�״̬,��Ϊfalse,���ʾ���ھ�ֹ״̬
	bool				m_bMoveMap;			// �Ƿ�ס�϶���ͼ
	int					m_nMoveOffX;		// ��Ϊ����ɫ��Xλ��
	int					m_nMoveOffY;		// ��Ϊ����ɫ��Yλ��
	int					m_nStartX;			// ��Ϊ����ɫ�ƶ�ǰ������λ�õ�X����
	int					m_nStartY;			// ��Ϊ����ɫ�ƶ�ǰ������λ�õ�Y����

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

	// ��ȡָ����ͼNPC�Ľӿڼ�
	// ȡMap tex ID
	bool	isInMapNpcDataBase( int nMapId );
	int		getMapNpcCount( int nMapId );
	const	tagMapActorInfo& getMapNpcInfo( int nId, int nIndex );
	
	void	clearMapNpc( int nMapId );
	void	updateMapNpcInfo( int nDefId, int nRealX, int nRealY );
	void	deleteMapNpcInfo( int nDefId );
	const	tagMapActorInfo* getNpcInfoById( int nMapId, int Id );
	const	tagMapActorInfo* getNpcInfoByName( int nMapId, const char* pszName );

	// ��ȡָ����ͼ��������Ľӿڼ�
	// ȡMap tex ID
	int		getMapSceneAreaCount( int nMapId );
	const	tagSceneAreaDef_ToLua& getMapSceneAreaInfo( int nMapId, int nIndex );
	int		getMapSceneAreainfocounts( int nMapId);

	// ���ڸ��ݵ�ͼID�鲢��ͼ�ϵ�NPC�͹���������Ϣ
	bool	isMultiLineMap( int nRealMapID );
	void	fillMultiLineMap();
	void	mergeMapNpc();
	void	mergeMapSceneArea();
	int		getRealMapId()				
	{ 
		return m_nRealMapId; 
	}

	// �����ݿ���ReferMapIDΪ�յ�ʱ��,RealMapID��GameMapControl�е�MapIDֵ��һ����
	void setRealMapId(int mapId);

	int getLineNo()
	{
		return m_nLineNo;
	}

	// ���ڻ�ȡ��Ϸ�����е�ͼ����Ϣ
	const tagMapInfo* getMapInfoByName( const char* pszName );
	const tagMapInfo& getMapInfoByIndex( int nMapArea, int nIndex );
	const tagMapInfo* getMapInfoByID( int mapId );
	// ��ȡĳ���޵ĵ�ͼ����
	unsigned int getMapInfoNum( int nMapArea );
	// ��ȡ��Ϸ�����е�ͼ������
	int getMapNum();
	tagMapInfo* getMapInfoByIndex( int nIndex );
	// ���ݵ�ͼ��Tex ID��ȡ��ͼʵ�ʳ����ĳߴ�
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

	std::vector<MAPNPC>					m_mapNpcVector;		//�洢������Ϸ��ͼNPC,�����ڽ����ͼǰִ�е�,��ȡ���е�ͼ�ϵ�NPC
	std::vector<tagSceneAreaDef>		m_mapAllSceneArea;  //�洢������Ϸ��ͼ���������,�����ڽ����ͼǰִ�е�,��ȡ���е�ͼ�ϵ��������
	std::map<int,MAPSCENEAREAVECTOR>	m_mapSceneAreaMap;	// ���ǰ��յ�ͼ�������������,�����ڽ���һ�ŵ�ͼ��ʱ��,��ȡĳ����ͼ�ϵ��������
	int									m_nRealMapId;
	int									m_nLineNo;
	AutoFindLineManager					m_AutoLine;
	std::vector<MAPDEF>					m_mapVector;
	std::map<int, MAPINFOVECTOR>		m_mapInfoMap;		// ��ֵΪ������Χ
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


