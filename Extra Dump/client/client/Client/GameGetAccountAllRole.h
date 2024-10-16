#pragma once
#include "proto_comm.h"
#include "proto_cs.h"
#include "OgreSingleton.h"
// #include "OgreLocker.h"
#include <vector>
#include <string>

//tolua_begin
struct tagAccRoleInfo_ToLua
{
	char RoleName[NAME_LEN];             
	int Level;                          
	int WorldID;                        
	int LastLogout;                     
};

struct tagAccGetRoleSvr_ToLua
{
	int Succ;                            	/*   1-成功 */
	int RegionID;                        	/*   大区id */
	int Num;                            
	tagAccRoleInfo_ToLua RoleInfo[ACC_ROLE_MAX];         
};

struct WorldRoleInfo
{
	WorldRoleInfo()
	{
		memset( &roleInfo, 0, sizeof(ACCGETROLESVR) );
	};
	unsigned short WorldID;
	unsigned short RegionId;
	std::string AuthIP;
	std::string szName;
	unsigned short AuthPort;
	tagAccGetRoleSvr_ToLua roleInfo;
};

//tolua_end
class GameGetAccountAllRole//tolua_export
	: public Ogre::Singleton<GameGetAccountAllRole>
{//tolua_export
public:
	GameGetAccountAllRole() {};
	~GameGetAccountAllRole() {};

	void Reset();

	void CloseAllThread();

	void AddWorldData( unsigned int WorldId, unsigned int RegionId, const char* szAuhIp, unsigned short AuthPort, const char* szName );

	void StartGetRoleData();

	//tolua_begin
	bool IsFinish();
	int GetWorldRoleInfoNum();
	const WorldRoleInfo* getWorldRoleInfoByIdx( int nIdx );
	//tolua_end
private:
	std::vector<WorldRoleInfo> m_vWorldRoleInfo;
	std::vector<void*>	m_vThreadHandle;

// 	LockSection			m_Mutex;
};//tolua_export
