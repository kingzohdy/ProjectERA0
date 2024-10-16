#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "Item.h" 
#include "GameActor.h"
#include "GameLoginManager.h"
#include "proto_cs.h"
#include "proto_comm.h"

//tolua_begin
/* 晶石背包 */
struct tagSparPkg_ToLua
{
	unsigned short MaxNum;                          	/*   当前最大格子数 */
	unsigned short Num;                             	/*   物品数量 */
	tagRoleItem_ToLua RoleItems[MAX_SPAR_PKG];         	/*   物品数组 */
};

//tolua_end

class GameWatchPlayer//tolua_export
		: public NetMsgHandler, public Ogre::Singleton<GameWatchPlayer>
{//tolua_export
public:
	GameWatchPlayer(void);
	~GameWatchPlayer(void);

public:
	//tolua_begin 
	// 向服务器发出观察请求
	void sendWatchRequest( int nMemID );	
	//tolua_end

	// 处理服务器发回的消息
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	//tolua_begin 
	// 获取被观察玩家的MemID
	int getWatchPlayerMemId(){ return m_nSawActorId; }
	
	// 获取被观察玩家的装备ITEM
	Item&	getItem( int GridIndex );

	// 组织模型装备显示数据
	void setAvatarAttrib();

	// 是否是被观察玩家身上的装备
	bool IsSawArmItem( int nArmId );

	GameActor* getWatchPlayer();

	tagSparPkg_ToLua* getWatchSparPkg();

	Item* getWatchSparItem( int nIdx );

	//获取所在服务器ID
	int getWatchPlayerWorldID() { return m_nSawActorWorldId; }
	//获取所在服务器名称
	const char* getWatchPlayerWorldName();

	//获取被观察玩家天赋点
	int getWatchPlayerGeniusPoint() { return m_nGeniusPoint; }
	int getAttrLv( int nIdx, int nAttrId );
	int getWatchPlayerGeniusTypePoint( int nType );
	void setFashionSuitSelect( int nIndex );
	int getFashionSuitSelect() { return m_nFashionSelect ;}
	//tolua_end

private:
	int			m_nSawActorId;
	int			m_nSawActorWorldId;
	Item		m_SawArm[EQUIP_POS_END];
	GameActor*	m_pWatchPlayer;
	tagSparPkg	m_SparPkg;
	int			m_nGeniusPoint;
	tagGeniusInfo m_GeniusInfo;
	int			m_nFashionSelect;	// 时装套数选择
};//tolua_export
