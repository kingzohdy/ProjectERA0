#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "GameActorManager.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin

//tolua_end
// fake actor manager
class ExtraActorMgr //tolua_export
	: public Ogre::Singleton<ExtraActorMgr>
{//tolua_export

public:
	ExtraActorMgr();
	~ExtraActorMgr();
	GameActor*		mounseOnActor(const Ogre::WorldRay& worldray);
	void			drawActorTitles(FollowCamera *pcamera);
	GameActor*		CreateActor( int id, GameActorType type, int resid, bool background = false );


	void			DestroyAllActor();

	void			Update( unsigned int dtick );
	
	//tolua_begin
	void			setNpcTalkID( int nNpcID );
	int				getNpcTalkID();
	void			setLastNpcTalkID( int nNpcID );
	int				getLastNpcTalkID();
	GameActor*		FindActor( int id );
	//tolua_end
public:
	int									m_nNpcMemId;
	int									m_nLastNpcID;

private:
	std::map<int, GameActor*>			m_ExtraActors;	//	fake NPC

};//tolua_export