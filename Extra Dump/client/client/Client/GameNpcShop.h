#pragma once
#include "GameNpcDialog.h"
#include "Item.h"

//tolua_begin

#define MAX_SHOP_ITEM_REQUIRE_TECH 1

// ������Ʒ
struct tagShopSellItem
{
	unsigned int GoodsType;             /*    Bind Macrosgroup:SHOP_ITEM_TYPE,*/
	unsigned int GoodsID;                        
	unsigned int Price;
	unsigned int SpecialMoneyType;
};

// �ع���Ʒ
struct tagShopSaveItem 
{
	Item			m_SaveItem;                       
	unsigned int	m_nSellPrice;                      
	int				m_nMoneyType;                    
	int				m_nSellTime;           
};

enum tagREBATE_TYPE_TOLUA 
{
	REBATE_TYPE_RED_NAME_TOLUA = 1, 	/* �����ۿ� */
	REBATE_TYPE_CITY_OWNER_1_TOLUA = 2, 	/* ռ��ʱ���1���ۿ� */
	REBATE_TYPE_CITY_OWNER_2_TOLUA = 3, 	/* ռ��ʱ���2-4���ۿ� */
	REBATE_TYPE_CITY_OWNER_3_TOLUA = 4, 	/* ռ��ʱ���5���ۿ� */
};

// �̵��б�
struct tagClientShopList
{
	int					m_nNpcId;
	int					m_nShopItemNum;						// ������Ʒ������
	int					m_nShopSaveItemNum;					// ������Ʒ������
	tagShopSellItem		m_ShopSellItems[MAX_SELL_GOODS];  
	tagShopSaveItem		m_ShopSaveItems[MAX_SELL_SAVE];
	unsigned short		RebateCount;			          	/*  Ver.974 ���ۿ� */
	char RebateTypeNum;                   					/*  Ver.974 �ۿ��������� */
	int  RebateTypes[REBATE_TYPE_MAX_NUM]; 	/*  Ver.974 �ۿ��������� */
};

// ����Ҫ��Ƽ�
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
	// ������Ʒ
	void		 buyItem( int index, int num );
	// �ع���Ʒ
	void		 buySaveItem( int index );
	// ������Ʒ
	void		 sellItem( int listType, int gridIndex, int num );
	// ��������Ʒ
	void		 repairOneItem( int listType, int gridIndex, int itemId );
	// ����ȫ����Ʒ
	void		 repairAllItem();
	// ��ȡȫ������۸�
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
