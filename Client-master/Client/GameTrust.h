#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "GameDefineTolua.h"
#include "GameManager.h"
#include "OgreScriptLuaVM.h"
#include <map>

//tolua_begin
struct tagTrustTaskItem_ToLua
{
	unsigned int	m_uItemID;                         
	unsigned short	m_nNum;                            
};

struct tagTrustTask_ToLua
{
	unsigned int			m_uTaskID;                         
	unsigned char			m_uItemNum;                        
	tagTrustTaskItem_ToLua	m_TrustItem[MAX_TASK_ITEM];       
};

struct tagFriendTrust
{
	tagLong_ToLua			m_RoleId;
	int						m_nVerson;
	int						m_nTrustNum;                 
	tagTrustTask_ToLua		m_TrustTaskData[MAX_TRUST_ORDER]; 
};
//tolua_end

class GameTrust //tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameTrust>
{//tolua_export
public:
	GameTrust(void);
	~GameTrust(void);

public:
	// c->s
	
	//tolua_begin
	// 打开委托挂单信息面板
	void trust_OpenTrustFace();
	// 获取挂单信息
	void trust_GetOrderInfo();
	// 添加赠送物品
	void addTrustItem( int nIndex, int nItemId, int nNum );
	// 删除委托赠送物品
	void delTrustItem();
	// 赠送委托物品
	void trust_Contribute( int nHeightId, int nLowId, int nTaskId );
	// 发布挂单
	void trust_Order( int nTrustID );
	// 撤销挂单
	void trust_CancelOrder( int nTrustID );
	// 委托是否完成
	bool trust_TrustIsComplete( int nTrustID );
	// 获取发布挂单的好友数量
	int getTrustRoleNum();
	// 获取玩家挂单信息列表
	const tagFriendTrust* getFriendTrustList( int nRow )const;
	// 获取主角挂单信息列表
	const tagFriendTrust* getMainPlayerTrustList()const;
	// 查询某个任务是否处于挂单状态
	bool	isInMainTrust( int	nTaskID );
	//tolua_end
	
	// 查询委托挂单是否有某个人
	bool	isInTrust( tdr_ulonglong roleWID );
	
	// 清空好友挂单信息
	void clearFriendTrust();
	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

private:
	std::map<tdr_ulonglong, tagFriendTrust>		m_FriendTrustMap;			// 挂单信息列表
	std::list<tagLong_ToLua>					m_GetTrustRoleList;			// 请求挂单信息玩家列表	
	tagTrustTaskItem_ToLua						m_TrustItem[MAX_TASK_ITEM]; // 赠送物品信息
	tagFriendTrust								m_selfTrust;
};//tolua_export
