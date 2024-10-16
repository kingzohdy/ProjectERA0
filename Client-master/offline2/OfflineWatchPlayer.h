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
	// ������������۲�����
	void sendWatchRequest( const char* szName );	
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
	//tolua_end

private:
	int			m_nSawActorId;
	Item		m_SawArm[EQUIP_POS_END];
	GameActor*	m_pWatchPlayer;
};//tolua_export
