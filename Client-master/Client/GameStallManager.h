#pragma once
#include "OgreSingleton.h"
#include "Item.h"  
#include "GameActor_Pet.h"
#include "GameDefineTolua.h"

//tolua_begin 
struct tagStallPetEntity_ToLua
{
	int					m_nDefID;                          
	tagPetId_ToLua		m_WID;                            
	int					m_nListType;                       
	int					m_nGridIdx;                        
	int					m_nStallGridIdx;                   
	tagRolePet_ToLua	m_RolePet;  
	unsigned int		m_uPrice;  
	tagLong_ToLua		m_GridGID;
};

struct tagStallData_ToLua
{
	char					m_szRoleName[NAME_LEN];
	char					m_szStallName[STALL_MAX_NAME_LEN]; 
	char					m_szStallAd[STALL_MAX_NAME_LEN];
	int						m_nItemNum;                        
	Item					m_ItemData[STALL_MAX_ITEM];       
	int						m_nPetNum;                         
	tagStallPetEntity_ToLua m_PetData[STALL_MAX_PET];         
};

enum
{
	CLIENT_STALL_MAX_PET		= STALL_MAX_PET,
	CLIENT_STALL_MAX_ITEM_PAGE	= STALL_MAX_ITEM_PAGE,
	CLIENT_STALL_MAX_ITEM		= STALL_MAX_ITEM,
	CLIENT_MIN_STALL_LEVEL		= 20,
};

enum
{
	CLIENT_STALL_START	= 0,
	CLIENT_STALL_END	= 1,
};

enum
{
	CLIENT_STALL_TYPE_ITEM	= STALL_TYPE_ITEM,
	CLIENT_STALL_TYPE_PET	= STALL_TYPE_PET,
};

enum
{
	CLIENT_MIN_STALL_DIST_BETWEEN_NPC = 300,
	CLIENT_MIN_STALL_DIST_BETWEEN_OTHER_STALL = 300,
};

enum
{
	STALL_CHG_ITEM_INSTALL_FAILED	= 0,
	STALL_CHG_ITEM_INSTALL_SUCCESS	= 1,
};

enum
{
	STALL_CHG_AD_FAILED		= 0,
	STALL_CHG_AD_SUCCESS	= 1,
};

enum
{
	CLIENT_STALL_MAP = 1000,
	CLIENT_MAX_MONEY = MAX_MONEY,
};

unsigned int getMaxUint();
//tolua_end

class GameStallManager//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameStallManager>
{//tolua_export
public:
	GameStallManager(void);
	~GameStallManager(void);

public:

	// c->s
	
	//tolua_begin 
	// 开始摆摊
	bool isSellGoodsMoneyTooMuch();
	void stall_Start( const char* szStallName );
	// 结束摆摊
	void stall_End();
	// 摊位更名
	void stall_ChangeName( const char* szName );
	// 摊位广告更改
	void stall_ChangeAd( const char* pszAd );
	// 不发送摊位广告更改协议
	void localChangeAd( const char* pszAd );
	// 摊位查看
	void stall_See( int nMemId );
	// 购物 stalltype 0 物品 1 宠物
	void stall_Buy( int nStallType, int nGridIdx, int nNum, const char* szName );
	// 扩充栏位
	void stall_AddGrid();
	// 摆摊状态下架宠物
	void	delPetInStall( int nGrid );
	// 摆摊状态上下架道具
	void	delItemInStall( int nGrid );
	// 重置摆摊信息
	void resetAllStallInfo();
	void resetStallInfo();
	// 重置所获取对方摊位信息
	void resetSeeStallInfo();
	// 获取摆摊的栏位信息
	int getStallGridNum();
	// 获取摊主的栏位信息
	int getSeeStallItemNum();
	//tolua_end

	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	virtual void update(unsigned int dtick);

public:
	void ValidateLastStallItems();
	void ValidateLastStallPets();
	//tolua_begin
	void ValidateLastStallGoods();
	// 是否没有任何商品上架
	bool isNoneGoodsOnStall();
	// 是否有NPC距离过近
	bool isNpcTooNear();
	// 是否有其他摊位距离过近
	bool isOtherStallTooNear();
	// 添加摆摊物品
	void addStallItem( int nStallGridIdx, int nListType, int nGrid, int nNum, unsigned int nPrice );
	// 摆摊状态上架道具
	void addStallItemInStall( int nStallGridIdx, int nListType, int nGrid, int nNum, unsigned int nPrice );
	// 移除摆摊物品
	void delLastStallItem( int nIndex );
	void delStallItem( int nIndex );
	// 移除摆摊宠物
	void delStallPet( int nIndex );
	// 添加摆摊宠物
	void addStallPet( int nStallIndex, int nType, int nIndex, unsigned int nPrice );
	// 摆摊状态添加摆摊宠物
	void addStallPetInStall( int nStallIndex, int nType, int nIndex, unsigned int nPrice );
	// 检查摊位上的物品是否已摆满
	bool isStallItemListFull();
	// 是否是已在摊位上架的物品
	bool isInStallItem( int nListType, int nGrid );
	// 检查摊位上的宠物是否已摆满
	bool isStallPetListFull();
	// 是否已在摊位上架的宠物
	bool isInStallIPet( int nType, int nGrid );
	bool isInStallIPet( int nType, int nHeightID, int nLowID );

	// 获取自己即将摆摊的物品
	void SaveLastStallGoodsToStall();
	void PutLastStallGoodsToStall();
	const tagStallData_ToLua& getSelfStallItem() const;
	const tagStallData_ToLua& getSelfLastStallItem() const;
	// 获取查看对象摆摊的物品
	const tagStallData_ToLua& getSeeStallItem() const;
	// nType 0表示自己，1表示对方
	bool	getItemLink( int nType, int GridIndex );
	bool	canSeeStall();
	// 检查双方是否可以展开交易
	bool	canStartStall( );

	bool	isInStallArea();
	// 是否可以下架商品
	bool	canRemoveGoods();

	Item*	getSaledItem( int idx );
	//tolua_end

private:
	void OnStallSeeRes( const STALLSEERES& stallSeeResData );
	void OnStallBuyerChgRes( const STALLINFOCHG& stallBuyChgResData );
	void OnStallSellerChgRes( const STALLINFOCHG& stallSellerChgData );
	void OnStallUpRes( const STALLUPRES& stallUpResData );
	void OnStallDownRes( const STALLDOWNRES& stallDownResData );

private:
	tagStallData_ToLua	m_SelfStallItem;		// 自己即将摆摊的物品
	tagStallData_ToLua	m_LastSelfStallItem;		// 自己即将摆摊的物品
	tagStallData_ToLua	m_SeeStallItem;			// 查看的摆摊物品
	int					m_StallGridNum;			// 摆摊的栏位数

	std::vector<Item>	m_SaledItem;			// 已经卖出去的物品
};//tolua_export
