// ��Ӱ֮սģ��
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
	int CampIdx;                         	/*   ��Ӫ */
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
	tagCSDoubleMonsterEndInfo_ToLua MonsterEndInfo;                  	/*  Ver.1955 ������Ϣ */                             
};
struct tagCSMachineWarInfo_ToLua
{
	int Human;                           	/*   �������� */
	int Titan;                           	/*   �������� */
	int Monster;                         	/*   �������� */
	int ToTalExp;                        	/*   �ܼƻ�þ��� */
	int StartTime;                       	/*   ����������ʼʱ�� */
	int SelfCamp;                        	/*   �Լ�����Ӫ */
	int VCamp;                           	/*   ʤ������Ӫ */
	int EndExp;                          	/*   ����ʤ�������ľ��� */
};

#define PWORLD_POLY_TANK_TOLUA PWORLD_POLY_TANK

struct tagCSTankRoleInfo_ToLua
{
	int MemID;                          
	int Career;                         
	int CampIdx;                         	/*   ��Ӫ */
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
	int getMachineWarReadyTime();							//��ȡ��е��������׼��ʱ��
	int getMachineWarTimeOut();							//��ȡ��е�����������ʱ��
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

	CSMACHINEWARINFO			m_MachineWarInfo;				//��е������Ϣ
	CSPOLYBOSSPOSITION			m_MachineWarBossPos;			//��е���죨���ʱ�̣���������Ӫ�����λ��

	CSTANKINFO					m_TankInfo;						//̹�˴�ս��Ϣ
	int							m_ContineKill;					//̹�˴�ս��ɱ��Ϣ
};//tolua_export