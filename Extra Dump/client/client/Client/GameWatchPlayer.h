#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "Item.h" 
#include "GameActor.h"
#include "GameLoginManager.h"
#include "proto_cs.h"
#include "proto_comm.h"

//tolua_begin
/* ��ʯ���� */
struct tagSparPkg_ToLua
{
	unsigned short MaxNum;                          	/*   ��ǰ�������� */
	unsigned short Num;                             	/*   ��Ʒ���� */
	tagRoleItem_ToLua RoleItems[MAX_SPAR_PKG];         	/*   ��Ʒ���� */
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
	// ������������۲�����
	void sendWatchRequest( int nMemID );	
	//tolua_end

	// ������������ص���Ϣ
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	//tolua_begin 
	// ��ȡ���۲���ҵ�MemID
	int getWatchPlayerMemId(){ return m_nSawActorId; }
	
	// ��ȡ���۲���ҵ�װ��ITEM
	Item&	getItem( int GridIndex );

	// ��֯ģ��װ����ʾ����
	void setAvatarAttrib();

	// �Ƿ��Ǳ��۲�������ϵ�װ��
	bool IsSawArmItem( int nArmId );

	GameActor* getWatchPlayer();

	tagSparPkg_ToLua* getWatchSparPkg();

	Item* getWatchSparItem( int nIdx );

	//��ȡ���ڷ�����ID
	int getWatchPlayerWorldID() { return m_nSawActorWorldId; }
	//��ȡ���ڷ���������
	const char* getWatchPlayerWorldName();

	//��ȡ���۲�����츳��
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
	int			m_nFashionSelect;	// ʱװ����ѡ��
};//tolua_export
