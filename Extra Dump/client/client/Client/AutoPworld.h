#pragma once
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "GameDefineTolua.h"
#include "NetMessage.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
struct tagAutoPworldTeamInfo_ToLua
{
	int MemID;                          
	int Ready;                          
};
//tolua_end

class AutoPworldManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<AutoPworldManager>
{//tolua_export
public:
	AutoPworldManager(void);
	~AutoPworldManager(void);
	//tolua_begin

	//�����
	void requestPortalFrame( int nType ); //0 - �����, 1 - ����Ƿ���Ҫ���Ÿ���������Ч
	// ͬ����븱��
	void requestAgree( bool bAgree );

	//������и�������
	void requestPworldPortal( int nPworldID );

	//��������״̬
	int	getPworldState() { return m_nPworldState; }
	void resetPworldState();

	int getMemberNum();
	tagAutoPworldTeamInfo_ToLua* getMemberData( int idx );
	unsigned int getStartTime();
	//tolua_end
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

private:
	int m_nPworldState;
	int m_nMemberNum;
	unsigned int m_uStartTime;
	AUTOPWORLDTEAMINFO m_TeamInfo[MAX_TEAM_MEMBER];
};//tolua_export