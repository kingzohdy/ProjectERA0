#pragma once

#include "OgreSingleton.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

#pragma pack(1)
//tolua_begin
#define  INVITE_ENTER_SHIKONG_PWORLD_TIME_TOLUA INVITE_ENTER_SHIKONG_PWORLD_TIME/* SHIKONG_QUEUE_TYPE*/
enum tagSHIKONG_QUEUE_TYPE_TOLUA
{
	SHIKONG_APPLY_QUEUE_TOLUA = 1, 	/* 报名队列 */
	SHIKONG_WAIT_QUEUE_TOLUA = 2, 	/* 等待队列 */
};
//tolua_end
#pragma pack()
class GameShiKong//tolua_export
	:public NetMsgHandler, public Ogre::Singleton<GameShiKong>
{//tolua_export
public:
	GameShiKong(void);
	~GameShiKong(void);
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin
	void ExitApplyQueue( int nPworldID );
	void EnterPworld( int nPworldID );
	//tolua_end
};//tolua_export
