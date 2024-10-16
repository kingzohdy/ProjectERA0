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
	unsigned char SelectType;                      	/*   骑乘中NPC 拥有选项类型, 商店、仓库 Bind Macrosgroup:RIDE_NPC_SELECT_TYPE,*/
	unsigned int SelectVal;                       	/*   视具体选项而定,如 打开商店则为商店列表ID */
};

struct tagRideDef_ToLua
{
	unsigned short RideID;                          	/*   骑乘物ID  */
	unsigned short Level;                           	/*   骑乘等级  */
	unsigned char RideType;                        	/*   骑乘类型 Bind Macrosgroup:RIDE_TYPE,*/
	char RideName[RES_NAME_LEN];         
	char RideDesc[SHORT_DESC_LEN];        	/*   骑乘描述 */
	unsigned int RideSpd;                         	/*   骑乘速度 */
	int ModelID;                         	/*   骑乘模型ID */
	int IconID;                          	/*  Ver.134 图标id */
	unsigned short LevelUpRate;                     	/*   骑乘升级概率 */
	unsigned short AmerceRate;                      	/*   骑乘升级爆率 */
	tagRideStuff_ToLua LevelUpStuff[MAX_RIDE_STUFF];    	/*   升级材料 */
	unsigned char RideOnNum;                       	/*   骑乘人数限制, 0视为单人坐骑. */
	unsigned int NpcModelID;                      	/*   骑乘携带 NPC */
	tagRideNpcSelect_ToLua NpcSelect[MAX_RIDE_NPC_SELECT];  	/*   Npc选项 */
	unsigned char PinZhi;                          	/*  Ver.144 骑乘品质 */
	tagResult_ToLua Result[MAX_RIDE_RESULT];         	/*  Ver.950 坐骑Result效果 */
	char ClanLimit;                       	/*  Ver.1058 0-无限制 1-沙巴克老大 2-沙巴克成员 */
	int MoneyType;                       	/*  Ver.1470 货币类型 */
	int MoneyVal;                        	/*  Ver.1470 货币值 */
	int MoveLevelMoneyType;              	/*  Ver.1704 货币类型 */
	int MoveLevelMoneyVal;               	/*  Ver.1704 货币值 */
	int HopeNum;                         	/*  Ver.1770 强化消耗计数达到此值后必成 */
	int RideMapFlag;                     	/*  Ver.1968  Bind Macrosgroup:RIDE_MAP_FLAG,*/
};
struct tagRidePinZhiDef_ToLua
{
	int PinZhi;                          	/*   骑乘品质 */
	char PinZhiName[RES_NAME_LEN];       
	int MaxLevel;                        	/*   最大强化等级 */
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

	unsigned int RideID;								// 骑乘ID                      
	int Endtime;										// 结束时间
	int	nLevel;											// 骑乘等级
	unsigned int	m_nOwnerID;							/*   骑乘拥有者ID */
	unsigned int	m_nPassengerNum;					/*   乘客数量 */
	unsigned int	m_nPassengerID[MAX_RIDE_PASSENGER];	/*   乘客列表 */
	char			m_szFormatEndTime[50];
	int				m_nEndLeft;							//结束剩余时间
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
	// 注册网络消息
	void initRiderNetMessage();
	void releaseRiderNetMessage();
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

	// 骑乘
	void onHorse( int nHorseId, int nBindId=200, bool bPlayRideEffect = true );
	// 多人骑乘上副座
	void onHorseSide( int nActorId );
	// 取消骑乘
	void downHorse();
	// 乘客下马
	void downHorseSide();
	// TODO:分析，在乘客被销毁的时候下马
	void downHorseSideWhenDestroy();
	// 飞行
	void onFly( int nFlyModelId );
	// 取消飞行
	void downFly();
	// 骑乘播放动作
	void horsePlayAct( int seq, const char **pmotions, int num_motion, int motion_class );
	
	void onHorseWithoutBind(int nHorseId,int nBindId=200);
	//tolua_begin
	
	// 骑术吟唱
	void MainPlayerRidePre( unsigned int uRideId, int nLevel );
	// 查看是否有客人等待上坐骑
	void checkWaiter( );

	void rideOn( unsigned int uRideId, int nRideLevel, bool already = false );
	// 上别人
	void rideOnSide( const tagCSRideOnInfo& rideInfo );
	// 将所有的乘客T下马
	void kickOffAllPassenger();

	// 请求下马
	void requestRideDown();
	// 是否在吟唱
	bool isPre();
	// 是否骑马上
	bool isOnHorse();
	// 是否飞行状态
	bool isOnFly();
	// 是否继续上次自动寻路
	bool isContinueAutoFind();

	// 获取骑乘的骑宠
	const tagRideInfo& getRideItem();
	// 获取骑乘最大格子位
	int getMaxRideNum();
	// 掌握的骑乘数量
	int getRidNum();
	// 获取对应格子的骑乘信息
	tagRideInfo& getRideGridInfo( int nIndex ); 
	// 是否有对应骑乘
	bool isHaveRide( int nRideId );
	// 获取骑乘ID
	const int getHorseId() const;
	// 丢弃坐骑
	void dropRide( int nRideId );
	// 升级坐骑
	void levelUpRide( int nRideId, int nListType, int nGrid );
	// 申请乘坐他人坐骑
	void applyRide( int nOwerId );
	// 同意他人申请做自己的坐骑
	void acceptApplyRide( int nApplyId );
	// 拒绝他人申请做自己的坐骑
	void refuseApplyRide( int nOwerId );
	// 邀请乘坐他人共同骑乘
	void inviteRide( int nInvitedId );
	// 同意他人邀请共同骑乘
	void acceptInviteRide( int nRideOwerId );
	// 踢某个乘客下马
	void kickOffPassenger( int nMemId );
	//坐骑等级转移
	void rideMoveLevel( int srcRideID, int desRideID );
	// 拒绝他人邀请共同骑乘
	void refuseInviteRide( int nRideOwerId, int nRefuseType );
	// 骑乘主人NPC操作
	void selectRideNpcOpton( int nSelectType );
	// 获取邀请人的信息
	const tagRideRoleInfo& getInviteRoleInfo();
	// 获取申请人的信息
	const tagRideRoleInfo& getApplyRoleInfo();
	// 判断自己是否是坐骑的主人
	bool isOwnRide();
	// 判断是否能够使用骑乘
	bool canRidePre( unsigned int uRideId, int nLevel );
	// 根据骑乘id获取骑乘的信息
	const tagRideInfo* getRideInfoById( int nRideId );
	// 根据骑乘id获取骑乘的信息
	tagRideInfo& getRideInfoByRideId( int nRideId );
	//获取骑乘品质定义
	const tagRidePinZhiDef_ToLua* getRidePinZhiDef( int nPinzhi );
	int getPassengerNum();
	//tolua_end
	
	// 设置骑乘主人
	void setRideOwner( int nOwnerID );
	// 设置骑乘信息
	void setRideInfo( int nNum, int nMaxNum, tagRideInfo* pRiderGridInfo, bool bNewInGame = false );
	// 设置主人乘客列表信息
	void setPassengersInfo( int nPassengerNum, const unsigned int* pPassengers );
	virtual void Update( unsigned int dtick);
	// 骑乘离开场景
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
