//  »ùÓÑÅä¶Ô
#pragma once;
#include "NetMessage.h"
#include "OgreSingleton.h"

class GamePetLand //tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GamePetLand>
{//tolua_export
public:
	GamePetLand();
	~GamePetLand();

	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	void replyGayInvite( const char* roleName, int Flag );
	void replyGayInviteTimeOut( const char* roleName );
	//tolua_end

};//tolua_export