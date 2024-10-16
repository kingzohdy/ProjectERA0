#pragma once
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "ItemDefine.h"
#include <map>
#include <vector>

//tolua_begin
struct tagRideRoleInfo
{
	int		m_nMemID;
	char	m_szRoleName[NAME_LEN];
};

#pragma pack(1)

struct tagRideStuff_ToLua
{
	unsigned int ItemID;                         
	unsigned char ItemNum;                        
};

struct tagRideNpcSelect_ToLua
{
	unsigned char SelectType;                      	/*   �����NPC ӵ��ѡ������, �̵ꡢ�ֿ� Bind Macrosgroup:RIDE_NPC_SELECT_TYPE,*/
	unsigned int SelectVal;                       	/*   �Ӿ���ѡ�����,�� ���̵���Ϊ�̵��б�ID */
};

struct tagRideDef_ToLua
{
	unsigned short RideID;                          	/*   �����ID  */
	unsigned short Level;                           	/*   ��˵ȼ�  */
	unsigned char RideType;                        	/*   ������� Bind Macrosgroup:RIDE_TYPE,*/
	char RideName[RES_NAME_LEN];         
	char RideDesc[SHORT_DESC_LEN];        	/*   ������� */
	unsigned int RideSpd;                         	/*   ����ٶ� */
	int ModelID;                         	/*   ���ģ��ID */
	int IconID;                          	/*  Ver.134 ͼ��id */
	unsigned short LevelUpRate;                     	/*   ����������� */
	unsigned short AmerceRate;                      	/*   ����������� */
	tagRideStuff_ToLua LevelUpStuff[MAX_RIDE_STUFF];    	/*   �������� */
	unsigned char RideOnNum;                       	/*   �����������, 0��Ϊ��������. */
	unsigned int NpcModelID;                      	/*   ���Я�� NPC */
	tagRideNpcSelect_ToLua NpcSelect[MAX_RIDE_NPC_SELECT];  	/*   Npcѡ�� */
	unsigned char PinZhi;                          	/*  Ver.144 ���Ʒ�� */
	tagResult_ToLua Result[MAX_RIDE_RESULT];         	/*  Ver.950 ����ResultЧ�� */
	char ClanLimit;                       	/*  Ver.1058 0-������ 1-ɳ�Ϳ��ϴ� 2-ɳ�Ϳ˳�Ա */
	int MoneyType;                       	/*  Ver.1470 �������� */
	int MoneyVal;                        	/*  Ver.1470 ����ֵ */
	int MoveLevelMoneyType;              	/*  Ver.1704 �������� */
	int MoveLevelMoneyVal;               	/*  Ver.1704 ����ֵ */
	int HopeNum;                         	/*  Ver.1770 ǿ�����ļ����ﵽ��ֵ��س� */
	int RideMapFlag;                     	/*  Ver.1968  Bind Macrosgroup:RIDE_MAP_FLAG,*/
};
struct tagRidePinZhiDef_ToLua
{
	int PinZhi;                          	/*   ���Ʒ�� */
	char PinZhiName[RES_NAME_LEN];       
	int MaxLevel;                        	/*   ���ǿ���ȼ� */
};

#pragma pack()

const tagRideDef_ToLua*	getRideDef( int nId, int nLv );

struct tagRideInfo
{
	tagRideInfo( unsigned int id = 0, int time = 0, int level = 0 )
	{
		RideID	= id;
		Endtime	= time;
		nLevel	= level;
		memset( m_szFormatEndTime, 0, sizeof( m_szFormatEndTime ) );
		m_nEndLeft = 0;
	}

	bool isValidEnd();

	unsigned int RideID;								// ���ID                      
	int Endtime;										// ����ʱ��
	int	nLevel;											// ��˵ȼ�
	unsigned int	m_nOwnerID;							/*   ���ӵ����ID */
	unsigned int	m_nPassengerNum;					/*   �˿����� */
	unsigned int	m_nPassengerID[MAX_RIDE_PASSENGER];	/*   �˿��б� */
	char			m_szFormatEndTime[50];
	int				m_nEndLeft;							//����ʣ��ʱ��
};

//tolua_end

enum
{
	CLIENT_ACTION_RIDE_DOWN			= 0,
	CLIENT_ACTION_RIDE_ON			= 1,
	CLIENT_ACTION_RIDE_PRE_BREAK	= 2,
};

class Rider//tolua_export
	: public NetMsgHandler
{//tolua_export
public:
	Rider(void);
	~Rider(void);

public:
	void setOwner( GameActor* pActor );
	GameActor *getOwner () { return m_pOwner; }
	// ע��������Ϣ
	void initRiderNetMessage();
	void releaseRiderNetMessage();
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	// ���
	void onHorse( int nHorseId, int nBindId=200, bool bPlayRideEffect = true );
	// ��������ϸ���
	void onHorseSide( int nActorId );
	// ȡ�����
	void downHorse();
	// �˿�����
	void downHorseSide();
	// TODO:�������ڳ˿ͱ����ٵ�ʱ������
	void downHorseSideWhenDestroy();
	// ����
	void onFly( int nFlyModelId );
	// ȡ������
	void downFly();
	// ��˲��Ŷ���
	void horsePlayAct( int seq, const char **pmotions, int num_motion, int motion_class );
	
	void onHorseWithoutBind(int nHorseId,int nBindId=200);
	//tolua_begin
	
	// ��������
	void MainPlayerRidePre( unsigned int uRideId, int nLevel );
	// �鿴�Ƿ��п��˵ȴ�������
	void checkWaiter( );

	void rideOn( unsigned int uRideId, int nRideLevel, bool already = false );
	// �ϱ���
	void rideOnSide( const tagCSRideOnInfo& rideInfo );
	// �����еĳ˿�T����
	void kickOffAllPassenger();

	// ��������
	void requestRideDown();
	// �Ƿ�������
	bool isPre();
	// �Ƿ�������
	bool isOnHorse();
	// �Ƿ����״̬
	bool isOnFly();
	// �Ƿ�����ϴ��Զ�Ѱ·
	bool isContinueAutoFind();

	// ��ȡ��˵����
	const tagRideInfo& getRideItem();
	// ��ȡ���������λ
	int getMaxRideNum();
	// ���յ��������
	int getRidNum();
	// ��ȡ��Ӧ���ӵ������Ϣ
	tagRideInfo& getRideGridInfo( int nIndex ); 
	// �Ƿ��ж�Ӧ���
	bool isHaveRide( int nRideId );
	// ��ȡ���ID
	const int getHorseId() const;
	// ��������
	void dropRide( int nRideId );
	// ��������
	void levelUpRide( int nRideId, int nListType, int nGrid );
	// ���������������
	void applyRide( int nOwerId );
	// ͬ�������������Լ�������
	void acceptApplyRide( int nApplyId );
	// �ܾ������������Լ�������
	void refuseApplyRide( int nOwerId );
	// ����������˹�ͬ���
	void inviteRide( int nInvitedId );
	// ͬ���������빲ͬ���
	void acceptInviteRide( int nRideOwerId );
	// ��ĳ���˿�����
	void kickOffPassenger( int nMemId );
	//����ȼ�ת��
	void rideMoveLevel( int srcRideID, int desRideID );
	// �ܾ��������빲ͬ���
	void refuseInviteRide( int nRideOwerId, int nRefuseType );
	// �������NPC����
	void selectRideNpcOpton( int nSelectType );
	// ��ȡ�����˵���Ϣ
	const tagRideRoleInfo& getInviteRoleInfo();
	// ��ȡ�����˵���Ϣ
	const tagRideRoleInfo& getApplyRoleInfo();
	// �ж��Լ��Ƿ������������
	bool isOwnRide();
	// �ж��Ƿ��ܹ�ʹ�����
	bool canRidePre( unsigned int uRideId, int nLevel );
	// �������id��ȡ��˵���Ϣ
	const tagRideInfo* getRideInfoById( int nRideId );
	// �������id��ȡ��˵���Ϣ
	tagRideInfo& getRideInfoByRideId( int nRideId );
	//��ȡ���Ʒ�ʶ���
	const tagRidePinZhiDef_ToLua* getRidePinZhiDef( int nPinzhi );
	int getPassengerNum();
	//tolua_end
	
	// �����������
	void setRideOwner( int nOwnerID );
	// ���������Ϣ
	void setRideInfo( int nNum, int nMaxNum, tagRideInfo* pRiderGridInfo, bool bNewInGame = false );
	// �������˳˿��б���Ϣ
	void setPassengersInfo( int nPassengerNum, const unsigned int* pPassengers );
	virtual void Update( unsigned int dtick);
	// ����뿪����
	void leaveMap();

	void bindOwner();

	void rideDown();

public:
	Ogre::ShowActor*	m_pShowActor;
	int					m_nCurBindingId;
	bool				m_bBind;


private:
	GameActor*			m_pOwner;		
	tagRideInfo			m_UsingRider;
	
	int					m_nRideNum;
	int					m_nMaxRideNum;
	int					m_nHorseId;
	tagRideInfo			m_RideGrids[MAX_RIDE_GRID];   
	GameMap*			m_curMap;

	tagRideRoleInfo		m_applyRole;
	tagRideRoleInfo		m_inviteRole;
	std::map<int,int>	m_bindEntities;
	bool				m_bIsContinueAutoFind;
};//tolua_export
