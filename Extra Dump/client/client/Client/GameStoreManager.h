#pragma once
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "GameDefineTolua.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameRollManager.h"

//tolua_begin
#pragma pack(1)
struct tagExchgRecord_ToLua
{
	unsigned int ItemID;                         
	char Who[NAME_LEN];                  
	unsigned int Time;
	int Num;                             	/*  Ver.2139  */
	int Price;                           	/*  Ver.2139  */
};

struct tagStoreGrid_ToLua
{
	tagRoleGrid_ToLua Grid;                           
	int Price;                          
};

struct tagStoreInfo_ToLua
{
	char StoreName[NAME_LEN];            
	int MaxNum;                         
	int Num;                            
	tagStoreGrid_ToLua Grid[STORE_MAX_GRID];           
	tdr_ulonglong Exchg;                           	/*   交易额度 */
	tdr_ulonglong Tax;                             	/*   税收 */
	int RecordNum;                      
	tagExchgRecord_ToLua Record[STORE_MAX_RECORD];       
	int Money;                           	/*   资产 */
	unsigned int OpenTime;                       
	unsigned int CloseTime;                       	/*   强迫关闭计时 */
	unsigned int PreDecTime;                      	/*   上一次扣钱时间 */
};

struct tagStore_ToLua
{
	unsigned int StoreID;                        
	tdr_ulonglong RoleID;                         
	char Name[NAME_LEN];                 
	tagStoreInfo_ToLua StoreInfo;                      
};

struct tagStoreGridDef_ToLua
{
	int Idx;                             	/*   第几个格子 */
	int UseItemID;                       	/*   消耗道具ID */
	int UseItemNum;                      	/*   消耗道具数量 */
};
#pragma pack()
int GetCeilFormC( float x );
//tolua_end
typedef std::vector<tagStore> VECSTORE;
typedef std::map<int,tagStoreItemChg> MAPITEMCHG;

class GameStoreManager//tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameStoreManager>
{//tolua_export
public:
	GameStoreManager();
	~GameStoreManager();
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );
	//tolua_begin
	const tagStore_ToLua* getStoreData();
	const tagStoreGridDef_ToLua* getStoreGridDef( int nIdx );
	int getSaveGridIdx( int nGridIdx );
	Item* getStoreItem( int nGridIdx );
	int getStoreItemPrice( int nGridIdx );
	bool setItemLink( int nIdx );

	void requestStoreOpen( const char* szName );
	void requestStoreClose( int nStoreID );
	void requestStorePage( int nIndex );
	void requestStoreView( int nStoreID );
	void requestStoreAddGrid( int nIdx );
	void requestStoreDecGrid( int nIdx );
	void requestStoreMoneyOpt( int nFlag, int nNum );
	void requestStoreUpItem( int nGridIdx, int nPrice, int nNum );
	void requestStoreDownItem( int nGridIdx, tdr_ulonglong wId );
	void requestStoreBuyItem( int nStoreID, int nGridIdx, int nMoney, int nNum, tdr_ulonglong wId );
	void requestStorePos( int nStoreID );
	void requestVipStore();
	void requestSearch( const char* szInfo );
	void requestChgStoreName( const char* szName );
	//tolua_end
private:
	VECSTORE	m_vecStore;
	MAPITEMCHG	m_mapItemChg;
	Item		m_StoreItem;
};//tolua_export