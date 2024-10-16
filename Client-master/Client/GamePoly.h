// 光影之战模块
#pragma once

#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "OgreSingleton.h"

//tolua_begin
struct tagCSDoubleMonsterRoleInfo_ToLua
{
	int MemID;                          
	int Career;                         
	int CampIdx;                         	/*   阵营 */
	char RoleName[NAME_LEN];             
	char ClanName[NAME_LEN];             
	int KillMonster;                    
	int Exp;                            
	int Money;                           	/*  Ver.1970  */
};

struct tagCSDoubleMonsterEndInfo_ToLua
{
	int RoleNum;                        
	tagCSDoubleMonsterRoleInfo_ToLua RoleList[MAX_BATTLE_PWORLD_MEMBER];
};
struct tagCSDoubleMonsterInfo_ToLua
{
	int MonsterCamp1;                   
	int MonsterCamp2;                   
	int StartTime;                      
	int VCamp;        
	tagCSDoubleMonsterEndInfo_ToLua MonsterEndInfo;                  	/*  Ver.1955 结束信息 */                             
};
struct tagCSMachineWarInfo_ToLua
{
	int Human;                           	/*   人类数量 */
	int Titan;                           	/*   巨人数量 */
	int Monster;                         	/*   怪兽数量 */
	int ToTalExp;                        	/*   总计获得经验 */
	int StartTime;                       	/*   副本真正开始时间 */
	int SelfCamp;                        	/*   自己的阵营 */
	int VCamp;                           	/*   胜利方阵营 */
	int EndExp;                          	/*   结束胜负奖励的经验 */
};

#define PWORLD_POLY_TANK_TOLUA PWORLD_POLY_TANK

struct tagCSTankRoleInfo_ToLua
{
	int MemID;                          
	int Career;                         
	int CampIdx;                         	/*   阵营 */
	char RoleName[NAME_LEN];             
	char ClanName[NAME_LEN];             
	int KillMonster;                    
	int Exp;                            
	int Money;                          
};
//tolua_end
class GamePoly //tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GamePoly>
{//tolua_export

public:
	GamePoly();
	~GamePoly();
	virtual int		HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	//tolua_begin

	//Double_Monster
	bool isInPolyPworld();
	void resetData();
	const tagCSDoubleMonsterInfo_ToLua*	getDoubleMonsterInfo();
	int	getSelfCampIdx();

	//Machine_War
	bool isInPloyPworld_MachineWar();
	void resetMachineWar();
	const tagCSMachineWarInfo_ToLua* getMachineWarInfo();
	int getMachineWarReadyTime();							//获取机械改造活动副本准备时间
	int getMachineWarTimeOut();							//获取机械改造活动副本存活时间
	int getMachineWarTitanPosX();
	int getMachineWarTitanPosY();
	int getMachineWarMachinePosX();
	int getMachineWarMachinePosY();
	int getTankSelfCampIdx();

	//Tank_War
	void resetTank();
	const tagCSTankRoleInfo_ToLua* getTankPlayerByIdx( int idx );
	int getTankPlayerNum();
	int getTankKillCamp1(){ return m_TankInfo.KillCamp1; }
	int getTankKillCamp2(){ return m_TankInfo.KillCamp2; }
	int getSelfCampInTankWar();
	void setTankContineKill( int nVal ){ m_ContineKill = nVal; }
	int getTankContineKill(){ return m_ContineKill; }
	//tolua_end

private:
	CSDOUBLEMONSTERINFO			m_sDoubleMonsterInfo;

	CSMACHINEWARINFO			m_MachineWarInfo;				//机械改造信息
	CSPOLYBOSSPOSITION			m_MachineWarBossPos;			//机械改造（尖峰时刻）非人类阵营首领的位置

	CSTANKINFO					m_TankInfo;						//坦克大战信息
	int							m_ContineKill;					//坦克大战连杀信息
};//tolua_export