#pragma once
#include "GameNpcDialog.h"
#include "Item.h"

//tolua_begin

#define MAX_SHOP_ITEM_REQUIRE_TECH 1

// 出售物品
struct tagShopSellItem
{
	unsigned int GoodsType;             /*    Bind Macrosgroup:SHOP_ITEM_TYPE,*/
	unsigned int GoodsID;                        
	unsigned int Price;
	unsigned int SpecialMoneyType;
};

// 回购物品
struct tagShopSaveItem 
{
	Item			m_SaveItem;                       
	unsigned int	m_nSellPrice;                      
	int				m_nMoneyType;                    
	int				m_nSellTime;           
};

enum tagREBATE_TYPE_TOLUA 
{
	REBATE_TYPE_RED_NAME_TOLUA = 1, 	/* 红名折扣 */
	REBATE_TYPE_CITY_OWNER_1_TOLUA = 2, 	/* 占领时间城1周折扣 */
	REBATE_TYPE_CITY_OWNER_2_TOLUA = 3, 	/* 占领时间城2-4周折扣 */
	REBATE_TYPE_CITY_OWNER_3_TOLUA = 4, 	/* 占领时间城5周折扣 */
};

// 商店列表
struct tagClientShopList
{
	int					m_nNpcId;
	int					m_nShopItemNum;						// 出售物品的数量
	int					m_nShopSaveItemNum;					// 购回物品的数量
	tagShopSellItem		m_ShopSellItems[MAX_SELL_GOODS];  
	tagShopSaveItem		m_ShopSaveItems[MAX_SELL_SAVE];
	unsigned short		RebateCount;			          	/*  Ver.974 总折扣 */
	char RebateTypeNum;                   					/*  Ver.974 折扣类型数量 */
	int  RebateTypes[REBATE_TYPE_MAX_NUM]; 	/*  Ver.974 折扣类型数组 */
};

// 出售要求科技
struct ShopItemRequireTech
{
	int num;
	int id[MAX_SHOP_ITEM_REQUIRE_TECH];
	int level[MAX_SHOP_ITEM_REQUIRE_TECH];
};

enum tagSHOP_ITEM_TYPE_CLIENT 
{
	SHOP_ITEM_NO_CLIENT = 0, 
	SHOP_ITEM_ITEM_CLIENT = 1, 
	SHOP_ITEM_CLAN_ITEM_CLIENT = 2, 
};

enum
{
	PET_DIATHESIS_ADJUST_REPAIR_VALUE = 10000,
};

//tolua_end

class GameNpcShop//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameNpcShop>
{//tolua_export
public:
	GameNpcShop(void);
	~GameNpcShop(void);

public:
	//tolua_begin
	const tagClientShopList& getShopList() const;
	// 购买物品
	void		 buyItem( int index, int num );
	// 回购物品
	void		 buySaveItem( int index );
	// 出售物品
	void		 sellItem( int listType, int gridIndex, int num );
	// 修理单个物品
	void		 repairOneItem( int listType, int gridIndex, int itemId );
	// 修理全部物品
	void		 repairAllItem();
	// 获取全部修理价格
	int			 getRepairAllPrice();
	int			getRepairOnePrice( Item& repairItem );

	void		buyClanItem( int index,int num );

	int			findItemindex( int itemid);

	ShopItemRequireTech		getRequireTech(int id);

	void		resetConditions();

	//tolua_end

	virtual int	 HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);

private:
	int	petNativeSkillResultDurPrice( int nPrice );
private:
	tagClientShopList		m_ShopList;
	unsigned short			m_ConditionNum;
	struct
	{
		int id;
		unsigned int type;
		int value1;
		int value2;
		int value3;
	}						m_Conditions[MAX_SELL_GOODS];
};//tolua_export
