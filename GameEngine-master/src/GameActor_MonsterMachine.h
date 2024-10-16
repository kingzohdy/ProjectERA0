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
	int MaxPassenger;                    	/*   ���Я���˿����� */
	tagMachinePassengerUIInfo_ToLua Passengers[MAX_MON_MACHINE_PASSENGER]; 	/*  Ver.747 �˿� ��Ϣ */
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
	int OreItemID;                       	/*   ��Ӧ��ƷID������������Ʒ */
	int ID;                              	/*  Ver.1245 �ɲɼ��Ĳɼ���ID */
};

struct tagMonMachineDef_ToLua
{
	unsigned int MonsterID;                      
	int MaxEnergy;                       	/*   ȼ������ */
	int CollcetTime;                     	/*   �ɼ�ʱ�� */
	int CollcetVal;                      	/*   �ɼ����� */
	int MaxPackOre;                      	/*   Я����ʯ���� */
	int BootyOreID;                      	/*  Ver.413 �����ʯ��ƷID */
	int PassengerNum;                    	/*  Ver.513 �ɳ������� */
	int CollectType;                     	/*  Ver.521 ���Բɼ������Ͷ�ӦMonsterdef->CollectType */
	int EnergyID;                        	/*  Ver.538 ���ĵ�ȼ�ϵ���ƷID, 0��ʾ����ȼ�϶����� */
	int EnergyDec;                       	/*  Ver.591 ÿ3�����ĵ�ȼ��ֵ */
	tagMonMachineOreLimit_ToLua Ore[MAX_MON_MACHINE_ORE];        	/*  Ver.872 ��ʯ��Ϣ */
	int LimitMapID[MAX_MACHINE_LIMIT_MAP]; 	/*  Ver.1034  */
	int CtrlFlag;                        	/*  Ver.1193  */
	int OreViewDist;                     	/*  Ver.1245 ���Կ�����Զ�Ŀ�ʯ */
	int LimitOreMonD[MAX_MACHINE_LIMIT_ORE]; 	/*  Ver.1265  */
	int CollcetEnergyDec;                	/*  Ver.1771 �ɼ��������� */
	int PlayerDie;                       	/*  Ver.1890 ��е��������һ��������� */
	int LeaveLimit;                      	/*  Ver.1890 ��е�������³���Ҫô������Ҫôϵͳ�Ƴ� */
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
	// �Ƿ��ڲɿ���
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

	//1��λ�Ƿ�������
	bool isMainPlayerDriver();
	int getPassenger(int i);
	int getPassengerNum();

	//tolua_end
	int	getTotalOre();
	// �Ͽ�
	void rideOnMonsterMachine( const CSROLEMACHINEINFO& rideOnData );
	// �Ͽ�
	void rideOnMonsterMachine( const CSACTIONRIDEONMACHINE& rideOnData );

	void rideOnMonsterMachine( tagMachineMonsterInfo& monsterInfo );
	// �¿�
	void downMonsterMachine( const CSACTIONLEAVEMACHINE& rideOnMachineInfo );
	// �󶨹��������
	void bindMachineAndOwner();
	// �����������ҵİ�
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

	//�Ƿ��ڼ���������
	bool isInCollectSkill();

	int MonsterMachine::getFirstEmptyIdx();
public:
	//tolua_begin
	int m_nOwnerID;
	tdr_ulonglong			m_FromItemWID;			// ��Ʒ�ٻ�WID
	tagMachineMonsterInfo	m_monsterInfo;
	tagMachinePlayerInfo	m_playerInfo;
	//tolua_end
	enum{
		ESKILLWAIT,
		ESKILLCOLLECT,	//����
		ESKILLOK,
	};

	CSACTIONMACHINEPASSENGERINFO	m_PassengerInfo;

};//tolua_export