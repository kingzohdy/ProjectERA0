#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "cs_net.h"


class GameActionManager
	: public NetMsgHandler, public Ogre::Singleton<GameActionManager>
{
public:
	GameActionManager(void);
	~GameActionManager(void);
public:
	virtual int	HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

private:
	void OnActionPetView( const CSACTIONRES& Msg );
	
	// ��������Ƿ�ɹ�
	void OnActionSuccessCatchPet( const CSACTIONRES& Msg );
	void OnActionFailCatchPet( const CSACTIONRES& Msg );
	void OnActionCatchPet( const CSACTIONRES& Msg );

	// ���
	void OnActionTeam( const CSACTIONRES& Msg );
	
	void OnActionOtherPlayerLeaveTeam( const CSACTIONRES& Msg );
	void OnActionMainPlayerLeaveTeam( const CSACTIONRES& Msg );
	void OnActionLeaveTeam( const CSACTIONRES& Msg );

	void OnActionOtherPlayerJoinTeam( const CSACTIONRES& Msg );
	void OnActionMainPlayerJoinTeam( const CSACTIONRES& Msg );
	void OnActionJoinTeam( const CSACTIONRES& Msg );
	// ���

	// ACTION_FAKE_STATUS
	void OnActionFakeStatus( const CSACTIONRES& FakeStatus  );
};
