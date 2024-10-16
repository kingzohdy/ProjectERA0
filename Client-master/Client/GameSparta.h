#pragma once

#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "OgreSingleton.h"
#include "ClientInterface.h"

//tolua_begin
struct tagCSSpartaFightRoleInfo_ToLua
{
    char RoleName[NAME_LEN];             
    int Camp;                           
    int SpartaGlory;                    
    int Leave;                          
    int FightVal;                       
    int ChgFightVal;                    
};
struct tagCSSpartaRoleSpartaInfo_ToLua
{
	int FightVal;                       
	int Victory;                        
	int RunAway;                        
	int Defeat;                         
};
#define MAX_SPARTA_ROLE_TOLUA MAX_SPARTA_ROLE
//tolua_end

class GameSparta //tolua_export
	: public NetMsgHandler, public IGameSparta, public Ogre::Singleton<GameSparta>
{//tolua_export
public:
	GameSparta();
	~GameSparta();

	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	virtual void	objEnterView(GameActor *actor);

	//tolua_begin

	void SpartaGetReady();
	void SpartaApply();
	void SpartaExitApply();

	int IsSpartaRoleReady( int idx );
	const char* GetSpartaRoleName( int idx );
	int GetSelfCampIdx();
	tagCSSpartaFightRoleInfo_ToLua GetSpartaEndInfo( int idx );

	int GetSpartaVCamp();
	int GetSelfTeamKill();
	int GetEnemyTeamKill();

	bool IsInSpartaPworld();

	//获取个人竞技场相关信息
	tagCSSpartaRoleSpartaInfo_ToLua GetSpartaRoleSaprtaInfo();
	//tolua_end

private:
	CSSPARTAFIGHTINFO m_SpartaFightInfo;			//战斗信息
	CSSPARTAREADYROLEINFO m_SpartaReadyRoleInfo[MAX_SPARTA_ROLE];	//玩家准备信息
	CSSPARTAROLESPARTAINFO  m_SpartaRoleSpartaInfo;		//个人竞技场相关信息
};//tolua_export