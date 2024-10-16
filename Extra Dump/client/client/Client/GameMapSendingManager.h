#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "MultiMapAutoFind.h"
#include<vector>

//tolua_begin
struct tagMapTransInfo_tolua
{
	int TransID;                         	/*   ��ͼ����ID */
	int ToMapID;                         	/*   Ŀ���ͼID(����ʱΪ���ߵ�ͼID) */
	int Money;                           	/*   ���ͷ� */
	int Condition;                       	/*  Ver.1049 �������� Bind Macrosgroup:TRANS_CONDITION_TYPE,*/
};

struct tagMapTransDef_ToLua
{
	int FromMapID;                       	/*   ��ͼID(����ʱΪ���ߵ�ͼID) */
	int ListIndex;                       	/*   ��ͼ����·�ߵ�Ψһ���� */
	int ToMapID;                         	/*   Ŀ���ͼID(����ʱΪ���ߵ�ͼID) */
	int X;                               	/*   ����x */
	int Y;                               	/*   ����y */
	int Money;                           	/*   ���ͷ� */
	int TransCondition;                  	/*  Ver.1051 �������� Bind Macrosgroup:TRANS_CONDITION_TYPE,*/
	int NeedLevel;                       	/*  Ver.1325 ����������ҵȼ� */
};
//tolua_end

class GameMapSendingManager//tolua_export
	:public NetMsgHandler ,public Ogre::Singleton<GameMapSendingManager>
{//tolua_export
public:
	GameMapSendingManager(void);
	~GameMapSendingManager(void);
public:
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

	//tolua_begin
	void mapSendingSelect( int npcID );
	int	getFormMapID();
	int getNpcID();
	int getMapTransDefNum();
	tagMapTransDef_ToLua* getMapTransDef( int nIdx );
	tagMapTransDef_ToLua* getMapTransDefByID( int nMapID );
	bool beginCalcTranList( int toMapId, bool bConditionCheck = true );
	int getCostMoney( int toMapId );
	void openTranFrameByItem( int nGridIdx, int nListType );
	int getMapTransItemList();
	int getMapTransItemGrid();
	//tolua_end
	int	getNextMapNum( int nSrcMapID );
	int	getNextMapInfo(int nSrcMapID, int nIndex);
	bool succesConstructPath( PATH& path, MAP_POINT& nCurPathEndInfo, 
							std::stack<MAP_POINT>& pathBuffer, 
							std::stack<MAP_POINT>& branchBuffer );
	bool		isSubPath( const PATH& subPath, const PATH& parentPath );
	bool		isInPath( int nMapID, const PATH& path );
	PATH_ITER	findMap( int nMapID, PATH& path );
	tagMapTransDef* getMapInfo( int nFromMapID, int nIndex );
	tagMapTransDef* getMapInfo( int nListIdx );
	void		setShortestPathesData();
	bool doCalcTranList( std::vector<int>& result, int toMapId, bool bConditionCheck );
private:
	std::vector<tagMapTransDef*>		m_MapTransDef;
	std::map<int, std::vector<int>>		m_nearMap;
	std::vector<int>					m_vecPath;
	bool								m_bFound;
	int									m_nFromMapID;
	int									m_nNpcID;
	std::vector<PATH>					m_shortestPathes;
	tagCSTransUseItem					m_useItem;
};//tolua_export
