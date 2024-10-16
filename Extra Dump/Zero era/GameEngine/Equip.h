#pragma once
#include "GameEngineTypes.h"
#include "Item.h"
#include "Equip_Common.h"
#include <map>
#include <vector>

const int TMPAMR_UNFIXDELAY = 1000;// 临时武器消失延迟

//tolua_begin
int getItemDisplayInfoHairStyle( int armDefId );
//tolua_end

bool IsFashionArm( const ROLEARM& arm );

class Equip//tolua_export
{//tolua_export
public:
	Equip(void);
	~Equip(void);

	static int getMonsterEquip(int resid, int slot);

	void setOwner( GameActor *pActor );
	void changEquip( int GridIndex, const ROLEARM& arm );
	void changEquip( int GridIndex, int armDefId,int nStar = 0 );
	void changAmm( int GridIndex, int ammId );
	void reLoadAllEquip();
	void clearAllEquip();
	int  checkSuit();
	void checkWeapEquip( int pos );
	bool checkAmmUsed( int ammId );

	// 获取装备的位置
	int getEquipPos( int armDefId );
	// 获得右手武器类型
	int getRightHandEquipType();

	void setTmpOpType(int opType){m_nTmpOpType = opType;};

	//tolua_begin
	Item&	getItem( int GridIndex );

	// 获取装备的超链接
	bool getItemLink( int GridIndex );

	// 获取武器装备类型
	int		getArmType( int pos );
	
	// 设置临时装备
	void	setTmpArm( int ArmId );
	// 绑定临时武器	 
	void	bindTmpArm( GameActor* pActor, int nArmId );
	// 取消绑定临时武器
	void	unBindTmpArm( GameActor* pActor, int nArmId );
	
	// 查询是否使用临时道具
	bool	isUsedTmp();
	// 获取临时装备
	int		getTmpArm();
	// 获取临时机械
	int		getTmpMachin();
	// 使用临时道具
	void	usedTmpArm( int armId=0 );
	// 退下临时道具
	void	unUsedTmpArm();
	// 丢弃临时道具
	void	looseTmpArm();
	// 屏蔽武器
	void	hideWeapon();
	// 显示武器
	void	showWeapon();

	// 请求装装备
	void send_Equip( int nListType, const Item& item, int wearPos = 0 );
	// 请求脱装备
	void send_unEquip( int Id, int desListType, int gridIndex, int wearPos );
	// 请求装备弹药
	void send_EquipAmm( int ammId );
	// 设置组织的换装属性列表
	void setAvatarAttrib( int nRace, int nGender );
	// 获取场景模型的装备模型
	int getAvatarId( int id );
	// 获取人物身上属于套装的装备数量
	int getPlayerSuitEquipNum( int id );
	// 获得对应格位物品
	int getEquipItemID( int nGridx );
	//是否在临时机械巡逻
	bool isTmpPatronid();
	int getAvatarStarLv( int id );

	void ShowShoulder( bool bShow );
	void send_SelectFashionSuit( int nSelectIdx );
	void setSelectFashionBag( int nSelect, int nNum );      // 设置时装包
	int	 getSelectFashionIndex();							// 获得选择时装id;
	int  getSelectFashionNum();								// 获得时装套数;
	// 是否在沙滩椅上
	bool isOnChairTmp();
	//tolua_end
	void CheckAvatarHair();

	

	//主角身上装备数据变化
	void SetArmInfo( int GridIndex, const ROLEARM& arm );
public:
	GameActor										*m_Owner;
	Item											m_Arm[EQUIP_POS_END];	
	int												m_ArmEquipId[EQUIP_POS_END];
	int												m_AmmMap[AMM_POS_END];
	int												m_ArmStar[EQUIP_POS_END];
	
	int												m_nFashionSelect;	// 时装套数选择
	int												m_nFashionNum;		// 时装数目

	int												m_TmpEquip;			// 临时道具位
	int												m_TmpMachin;		// 临时机械

	bool											m_bHideWeap;		// 是否隐藏武器
	int												m_nTmpOpType;		// 临时机械操作标记
	static std::map<int,std::vector<int>>			m_mapSuitSet;		// 这是按照套装ID来归类装备物品
};//tolua_export