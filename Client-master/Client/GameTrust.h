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
	// ��ί�йҵ���Ϣ���
	void trust_OpenTrustFace();
	// ��ȡ�ҵ���Ϣ
	void trust_GetOrderInfo();
	// ���������Ʒ
	void addTrustItem( int nIndex, int nItemId, int nNum );
	// ɾ��ί��������Ʒ
	void delTrustItem();
	// ����ί����Ʒ
	void trust_Contribute( int nHeightId, int nLowId, int nTaskId );
	// �����ҵ�
	void trust_Order( int nTrustID );
	// �����ҵ�
	void trust_CancelOrder( int nTrustID );
	// ί���Ƿ����
	bool trust_TrustIsComplete( int nTrustID );
	// ��ȡ�����ҵ��ĺ�������
	int getTrustRoleNum();
	// ��ȡ��ҹҵ���Ϣ�б�
	const tagFriendTrust* getFriendTrustList( int nRow )const;
	// ��ȡ���ǹҵ���Ϣ�б�
	const tagFriendTrust* getMainPlayerTrustList()const;
	// ��ѯĳ�������Ƿ��ڹҵ�״̬
	bool	isInMainTrust( int	nTaskID );
	//tolua_end
	
	// ��ѯί�йҵ��Ƿ���ĳ����
	bool	isInTrust( tdr_ulonglong roleWID );
	
	// ��պ��ѹҵ���Ϣ
	void clearFriendTrust();
	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

private:
	std::map<tdr_ulonglong, tagFriendTrust>		m_FriendTrustMap;			// �ҵ���Ϣ�б�
	std::list<tagLong_ToLua>					m_GetTrustRoleList;			// ����ҵ���Ϣ����б�	
	tagTrustTaskItem_ToLua						m_TrustItem[MAX_TASK_ITEM]; // ������Ʒ��Ϣ
	tagFriendTrust								m_selfTrust;
};//tolua_export
