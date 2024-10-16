#pragma once
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "NetMessage.h"

class GameDynPWorld //tolua_export
	: public NetMsgHandler, Ogre::Singleton<GameDynPWorld>
{//tolua_export
public:
	GameDynPWorld(void);
	~GameDynPWorld(void);

	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

public:
	//tolua_begin
	// �˳�����
	void exitDynPWorld( int nMapDefID );
	// ��̬����ѡ��
	void selectDynPWorld( int nGrade, int nLevel, int nDifficulty = PWORLD_DIFFICULTY_NORMAL );
	//tolua_end

private:
	int m_nPWorldId;
};//tolua_export
