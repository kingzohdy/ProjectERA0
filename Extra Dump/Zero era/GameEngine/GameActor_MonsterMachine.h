#pragma once
#include <vector>
#include <map>
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

class GameActor;
//tolua_begin
#pragma pack(1)

struct tagMachinePassengerUIInfo_ToLua
{
	int PassengerID;
	unsigned char Level;
	unsigned char Career;
	unsigned char Head;
	unsigned char Hair;
	unsigned char Face;                           
	unsigned char Gender;
	char RoleName[NAME_LEN];
	char ClanName[NAME_LEN];
};

struct tagCSActionMachinePassengerInfo_ToLua
{
	int MaxPassenger;                    	/*   最大携带乘客数量 */
	tagMachinePassengerUIInfo_ToLua Passengers[MAX_MON_MACHINE_PASSENGER]; 	/*  Ver.747 乘客 信息 */
};
#pragma pack()

struct tagMachineMonsterInfo
{
	int		m_nMonsterMemID;
	int		m_nMonsterDefID;
	int		m_nCollcetTime;
	int		m_nCollcetVal;
	int		m_nOre[MAX_MON_MACHINE_ORE];
	int		m_nMaxOre;
	int		m_nEnergy;
	int		m_nMaxEnergy;
	int		m_nMaxPassenger;
	int		m_nPassengerID[MAX_MON_MACHINE_PASSENGER];
	int		m_OwnerType;
	tdr_ulonglong	m_OwnerClanWID;
	int		m_nCollectType;
};

struct tagMachinePlayerInfo
{
	int				m_nMonsterMemID;
	int				m_nMonsterDefID;
	int				m_nSitIdx;
	bool			m_bBind;
};

struct tagMonMachineOreLimit_ToLua
{
	int OreItemID;                       	/*   对应物品ID，掉落给予的物品 */
	int ID;                              	/*  Ver.1245 可采集的采集物ID */
};

struct tagMonMachineDef_ToLua
{
	unsigned int MonsterID;                      
	int MaxEnergy;                       	/*   燃料上限 */
	int CollcetTime;                     	/*   采集时间 */
	int CollcetVal;                      	/*   采集数量 */
	int MaxPackOre;                      	/*   携带矿石上限 */
	int BootyOreID;                      	/*  Ver.413 掉落矿石物品ID */
	int PassengerNum;                    	/*  Ver.513 可乘坐人数 */
	int CollectType;                     	/*  Ver.521 可以采集的类型对应Monsterdef->CollectType */
	int EnergyID;                        	/*  Ver.538 消耗的燃料的物品ID, 0表示任意燃料都可以 */
	int EnergyDec;                       	/*  Ver.591 每3秒消耗的燃耗值 */
	tagMonMachineOreLimit_ToLua Ore[MAX_MON_MACHINE_ORE];        	/*  Ver.872 矿石信息 */
	int LimitMapID[MAX_MACHINE_LIMIT_MAP]; 	/*  Ver.1034  */
	int CtrlFlag;                        	/*  Ver.1193  */
	int OreViewDist;                     	/*  Ver.1245 可以看到多远的矿石 */
	int LimitOreMonD[MAX_MACHINE_LIMIT_ORE]; 	/*  Ver.1265  */
	int CollcetEnergyDec;                	/*  Ver.1771 采集能量消耗 */
	int PlayerDie;                       	/*  Ver.1890 机械死亡人物一起死亡标记 */
	int LeaveLimit;                      	/*  Ver.1890 机械不可以下车。要么死亡。要么系统移除 */
};

//tolua_end

class MonsterMachine//tolua_export
{//tolua_export
public:
	MonsterMachine();
	~MonsterMachine();

public:
	void setOwner( int ownerID );
	void clearRideMachinInfo();
	void SetMonsterMachineID( int nMachineActorID );

	//tolua_begin
	// 是否在采矿车上
	bool isInMonsterMachine();

	int getOre(int i);
	int	getMaxOre();
	int getEnergy();
	int	getMaxEnergy();
	int getOreId(int i);
	
	MonsterMachine*	getPlayerMonsterMachine();

	tagMonMachineDef_ToLua* getMonMachineDef();

	bool cancelSkillSelectData();

	float getLeavePrepareTime();

	tagCSActionMachinePassengerInfo_ToLua* getMachinePassgerInfo();
	bool	canChangeSide();
	tagMonMachineDef_ToLua* getMonMachineDef( int nMonsterID );

	bool canRideOn();

	//1号位是否是主角
	bool isMainPlayerDriver();
	int getPassenger(int i);
	int getPassengerNum();

	//tolua_end
	int	getTotalOre();
	// 上矿车
	void rideOnMonsterMachine( const CSROLEMACHINEINFO& rideOnData );
	// 上矿车
	void rideOnMonsterMachine( const CSACTIONRIDEONMACHINE& rideOnData );

	void rideOnMonsterMachine( tagMachineMonsterInfo& monsterInfo );
	// 下矿车
	void downMonsterMachine( const CSACTIONLEAVEMACHINE& rideOnMachineInfo );
	// 绑定怪物与玩家
	void bindMachineAndOwner();
	// 解除怪物与玩家的绑定
	void unbindMachineAndOwner( const RESPOS& standPos );

	void setMachineRideData( const MONMACHINEINFO& machineInfo );

	void useMonsterSkill( int skillID,int skillLv );

	bool canMove();

	bool hasPatrolID();

	void setAttriValule( int attriId, int Value );

	void Update();

	void ChangePassengerInfo( const CSACTIONMACHINEPASSENGERINFO& info );

	bool canAttackTarget( int nId );

	void trySendAttackMsg();

	bool setCollectSelectData( int angle );

	bool isSameCamp();

	void setMachineSkill();

	int	getMonsterEnergy();
	
	void onPassengeLeave( int nActorId );

	bool isPlayerOnMachine( GameActor* pActor );

	void endCollectSkill();

	//是否在集气技能中
	bool isInCollectSkill();

	int MonsterMachine::getFirstEmptyIdx();
public:
	//tolua_begin
	int m_nOwnerID;
	tdr_ulonglong			m_FromItemWID;			// 物品召唤WID
	tagMachineMonsterInfo	m_monsterInfo;
	tagMachinePlayerInfo	m_playerInfo;
	//tolua_end
	enum{
		ESKILLWAIT,
		ESKILLCOLLECT,	//集气
		ESKILLOK,
	};

	CSACTIONMACHINEPASSENGERINFO	m_PassengerInfo;

};//tolua_export