#pragma once
#include "OgreSingleton.h"
#include "Item.h" 
#include "GameActor.h"

class OfflineWatchPlayer//tolua_export
		: public NetMsgHandler, public Ogre::Singleton<OfflineWatchPlayer>
{//tolua_export
public:
	OfflineWatchPlayer(void);
	~OfflineWatchPlayer(void);

public:
	//tolua_begin 
	// 向服务器发出观察请求
	void sendWatchRequest( const char* szName );	
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
	//tolua_end

private:
	int			m_nSawActorId;
	Item		m_SawArm[EQUIP_POS_END];
	GameActor*	m_pWatchPlayer;
};//tolua_export
