#pragma once
#include "ItemDefine.h"
#include <string>

//tolua_begin
enum EITEM_TYPE
{
	ITEM_SLOTTYPE_NONE,		// 空
	ITEM_SLOTTYPE_DISABLE,	// 不可用
	ITEM_SLOTTYPE_ITEM,		// 物品
	ITEM_SLOTTYPE_AMM,		// 装备
};

enum
{
	ITEM_TYPE_TOOLS		= 5,	// 道具
};

enum
{
	UNUSED_AMM						= -1,		// 不需要使用子弹
	AMM_ID							= 512,		// 弹药 	
	AMM_ID_TEM						= 513,		// 临时弹药
	
	ITEM_PILL_HP_ID					= 5000,		// 药水HP补给类(一次性)
	ITEM_PILL_MP_ID					= 5001,		// 药水MP补给类(一次性)
	ITEM_PILL_HPLONG_ID				= 5002,		// 药水HP补给类(持续性)
	ITEM_PILL_MPLONG_ID				= 5003,		// 药水MP补给类(持续性)
	ITEM_PILL_BUFF_ID				= 5004,		// 药水状态类
	ITEM_PILL_BUFFLONG_ID			= 5005,		// 药水BUFF类
	ITEM_PILL_MORPH_ID				= 5006,		// 药水变身类
	ITEM_PILL_PET_ID				= 5007,		// 药水宠物类
	ITEM_PILL_HPMP_ID				= 5008,		// 药水补给类(全恢复)
	ITEM_PILL_HPMPLONG_ID			= 5009,		// 药水补给类(持续性全恢复)
	
	ITEM_FOOD_HPLONG_ID				= 5010,		// 食物HP补给类(持续性)
	ITEM_FOOD_MPLONG_ID				= 5011,		// 食物MP补给类(持续性)
	ITEM_FOOD_BUFFLONG_ID			= 5012,		// 食物BUFF类
	ITEM_FOOD_BASEATTR_ID			= 5013,		// 全回复+一级属性(长)
	ITEM_FOOD_RACEATTR_ID			= 5014,		// 全回复+职业属性(长)

	ITEM_MAP_POSITION_ID			= 5030,		// 定位符

	ITEM_SCROLL_SKILL_ID			= 5040,		// 卷轴技能学习类
	ITEM_SCROLL_ID					= 5041,		// 卷轴基础行为类
	ITEM_SCROLL_QIYUE_ID			= 5042,		// 卷轴类 契约
	ITEM_SCROLL_DESIGNATION_ID		= 5043,		// 卷轴类 称号
	
	ITEM_NO_DIE_PUNISH_ID			= 5050,		//死亡赦免BUFF的道具
	ITEM_CLAN_ITEM					= 5051,		// 公会道具
	ITEM_HELP_PRESENT_ID			= 5052,		// 辅助礼包类及卷轴
	ITEM_HELP_BOX_ID				= 5053,		// 辅助类百宝箱
	ITEM_HELP_RAND_ID				= 5054,		// 辅助类点石成金

	ITEM_TRUST_ID					= 5060,		// 活动道具委托物品
	ITEM_PROMOTION_ID				= 5062,		// 参与活动使用道具 藏宝图

	ITEM_GEM_STAR_ID				= 5200,		// 品质强化宝石及碎片
	ITEM_GEM_STRENGTH_ID			= 5209,		// 强化保险符及碎片

	ITEM_INLAYGEM_RED_ID			= 5220,		// 强化保险符及碎片
	ITEM_INLAYGEM_GREEN_ID			= 5221,		// 强化保险符及碎片
	ITEM_INLAYGEM_BLUE_ID			= 5222,		// 强化保险符及碎片
	ITEM_INLAYGEM_COLORFUL_ID		= 5223,		// 强化保险符及碎片
	ITEM_INLAYGEM_OTHER_ID			= 5229,		// 强化保险符及碎片

	ITEM_PET_EXP_ID					= 5230,		// 宠物道具经验果等
	ITEM_PETXUNFU_ID				= 5233,		// 宠物等驯服道具
	ITEM_PETLEVELUP_ID				= 5235,		// 宠物等改造相关 残卷、碎石
	ITEM_FIGHTTING_BOOK1_ID			= 5240,		// 技能配件相关配件书
	ITEM_FIGHTTING_BOOK2_ID			= 5241,		// 技能配件相关配件书
	ITEM_FIGHTTING_BOOK3_ID			= 5242,		// 技能配件相关配件书

	ITEM_LVUP_ID					= 5250,		// 成长类
	ITEM_HELP_LVUP_ID				= 5251,		// 辅助类
	ITEM_HELP_MAIL_ID				= 5255,		// 自动拾取、邮件道具

	ITEM_TASK_MAINLINE_ID			= 5300,		// 任务道具(主线任务相关)
	ITEM_TASK_MAINLINE_ID_TASK	= 5301,		// 任务道具(主线任务相关)
			
	ITEM_PET_ID						= 5600,		// 载体
	ITEM_PETSHIEGG_ID				= 5610,		// 侍卫蛋
	ITEM_PETSHIEGG_ID2				= 5611,		// 宠物蛋
	ITEM_PETXIANEGG_ID				= 5620,		// 仙子蛋
	ITEM_TRUMP_ID					= 5622,		// 精灵炼化加速

	ITEM_LEARN_SECONDARY_ID1		= 5254,		// 学习生活技能
	ITEM_LEARN_SECONDARY_ID2		= 5255,		// 学习生活技能
	ITEM_LEARN_SECONDARY_ID3		= 5256,		// 学习生活技能
};

enum// 装备内容
{
	EQUIP_NORMAL		= 0,		// 普通
	EQUIP_ACTIVITY		= 13,		// 活动
	EQUIP_PEI			= 14,		// 
	EQUIP_MARKET		= 15 ,
	EQUIP_SUIT			= 16,
	EQUIP_NORMALSUIT	= 17,
};

enum
{
	ITEM_IMPORTANT_NORMAL = 0,					// 普通品质
	ITEM_IMPORTANT_EXCELLENCE,					// 优秀品质
	ITEM_IMPORTANT_BEST,						// 精良品质
	ITEM_IMPORTANT_NONSUCH,						// 极品品质
	ITEM_IMPORTANT_EPIC,						// 套装  普通
	ITEM_IMPORTANT_EPIC_A,						// 套装  史诗套装
	ITEM_IMPORTANT_EPIC_S,						// 套装  英雄套装
	ITEM_IMPORTANT_LEGEND,						// 传奇品质
	ITEM_IMPORTANT_END,
};

enum
{
	ITEM_IDTYPE_NONE		= 0,     
	ITEM_IDTYPE_ARM			= 1,	// 武器
	ITEM_IDTYPE_EQUIP		= 2,	// 防具
	ITEM_IDTYPE_BARDAIN		= 3,	// 个性
	ITEM_IDTYPE_ITEM		= 5,	// 道具
};

enum 
{
	ITEM_EXTENDED_TYPE_NONE = 0, 	/* 不是附魔卷轴 */
	ITEM_EXTENDED_TYPE_RESULT = 1, 	/* 装备效果(RESULT)指定的那些效果 */
	ITEM_EXTENDED_TYPE_ATTR = 2, 	/* 装备关键字 */
};

enum
{
	ITEM_VALID_24H,
	ITEM_VALID_3H,
	ITEM_VALID_10M,
	ITEM_VALID_END,
};

/*   重要程度|装备使用此字段作为绿装蓝装等的判断:1绿 2蓝 3紫 4橙 */
enum
{
	ARM_NONE_PIN_ZHI	= 0,
	ARM_GREEN_PIN_ZHI	= 1,
	ARM_BLUE_PIN_ZHI	= 2,
	ARM_PURPLE_PIN_ZHI	= 3,
	ARM_ORANGE_PIN_ZHI	= 4,
};

#define MAX_ITEM_ADD_ATTR_TOLUA MAX_ITEM_ADD_ATTR

enum tagITEM_CTRL_FLAG_TOLUA 
{
	ITEM_FIGHT_NO_USE_TOLUA = 1, 	/* 战斗状态不可以使用物品 */
	ITEM_RIDE_MACHINE_USE_TOLUA = 2, 	/* 骑乘怪物机械可以使用的道具 */
	ITEM_PRE_USE_TOLUA			= 4, 	/* 吟唱状态可以使用的道具 */
	ITEM_SHOW_LIGHT_TOLUA		= 8, 	/* 物品流光标记 */
	ITEM_NO_LEVELLIMIT_TOLUA	= 16, 	/* 物品穿戴忽略等级限制 */
	ITEM_IMP_LOGTRACE_TOLUA		= 32, 	/* 重要物品记录流水日志 */
	ITEM_COLOR_GREEN_TOLUA		= 64, 	/* 道具绿色边框 */
	ITEM_COLOR_BLUE_TOLUA		= 128, 	/* 道具蓝色边框 */
	ITEM_COLOR_PURPLE_TOLUA		= 256, 	/* 道具紫色边框 */
	ITEM_COLOR_PINK_TOLUA		= 512, 	/* 道具粉色边框 */
	ITEM_MO_SHI_NO_USE_ToLUA	= 1024, 	/* 此物品无法在末世荣耀殿堂内使用 */
	ITEM_DROP_TIME_ToLUA		= 2048, 	/* 此物品掉落保护时间大于生存时间 */
	ITEM_S_ToLUA				= 4096, 	/* 客户端用 */
	ITEM_A_ToLUA				= 8192, 
	ITEM_B_ToLUA				= 16384,
	ITEM_CAN_STEP_UP_BACK_TOLUA = 65536, 	/* 可进阶背饰,用于作为背饰幻化的目标等 */
	ITEM_NO_SHOW_PREFIX_TOLUA = 131072, 	/* 表示不显示前缀 */
	ITEM_HAVE_FLOAT_ACT_TOLUA = 262144, 	/* 有漂浮动作 */
};

bool bitAnd( int nLhs, int nRhs );
int  bitAndOperation( int nLhs, int nRhs );
int  bitOr( int nLhs, int nRhs );
int  bitNotAnd( int nLhs, int nRhs );

int						getItemType( int id );
tagItemDef_ToLua*		getItemDef( int id );
const char*				getArmAttrName( int id );
int						getArmAttrValue( int id, int nAttrIdx );
tagAddAttrDef_ToLua*	getAddAttrDef( int id );
tagSuitSet_ToLua*		getSuitSet( int id );
tagArmGradeDef_ToLua*	getArmGradeDef(int nArmType, int nArmLevel );
tagArmHideAttrDef_ToLua*	getArmHideAttrDef( int nCareer, int nArmStarLevel );
tagArmStarDef_ToLua*	getArmStarDef( int nStar );
tagArmAddAttrDef_ToLua* getArmAddAttrDef( int id );
tagAddAttrBaseDef_ToLua* getAddAttrBaseDef( int nLv );
tagFixedAddAttr_ToLua*  getFixedAddAttr( int id );
const tagNewAddAttrDef_ToLua* getNewAddAttrDef( int id );
const tagAddAttrRandDef_ToLua* getAddAttrRandDef( int level, int important );
const tagItemResultDesc_ToLua* getItemResultDesc( int id );
const tagBaoXiangDef_ToLua* getBaoXiangDef( int id );

const std::string&		getItemColor( int important, int suitsetId = 0 );
int						getArmExtraDur( tagItemDef_ToLua* pItemDef ); // 计算额外的耐久度

const char*				getDisplayName(tagItemDef_ToLua* pItemDef );

int itemid_num( tagItemDef_ToLua *pstItemDef );

bool isSameWid( tdr_ulonglong lhs, tdr_ulonglong rhs );

bool isItemCanUsed( int nItemSubType );
//tolua_end

class Item//tolua_export
{//tolua_export
public:
	Item(void);
	~Item(void);

	const Item &operator =( const ROLEITEM& item )
	{
		m_ItemType			= ITEM_SLOTTYPE_ITEM;
		m_ItemData.m_Item	= item;
		InitItemName();
		return *this;
	}
	
	const Item &operator =( const ROLEARM& item )
	{
		m_ItemType			= ITEM_SLOTTYPE_AMM;
		m_ItemData.m_Arm	= item;
		InitItemName();
		return *this;
	}
	
	const Item &operator =( const Item& item )
	{
		m_ItemType				= item.m_ItemType;
		m_ItemData.m_Arm		= item.m_ItemData.m_Arm;
		m_ItemData.m_Item		= item.m_ItemData.m_Item;
		m_ListType				= item.m_ListType;
		m_bCanUsed				= item.m_bCanUsed;
		m_strItemName			= item.m_strItemName;
		return *this;
	}
	
public:
	//tolua_begin
	static int			getDisplayIndex(int id);
	void				setItemDate( int type, void* pValue );
	tagItemDef_ToLua*	getItemDef();
	int					getItemId();		// 获取物品静态ID
	tdr_ulonglong		getItemWId();		// 获取物品全局唯一ID
	bool				canUsed();			// 物品是否可使用
	bool				canExchange();		// 物品是否可交易		
	
	int  getGridIdx();						// 获取格子数
	int	 getListType();						// 所在容器类型
	int	 getItemType();						// 获取物品格子类型(道具或者装备)
	int  getItemSubType();					// 获取道具子类型
	int  getNum();							// 获取道具数量
	void setNum( int nNUm );				// 设置道具数量

	int  getItemDur();						// 当前持久度
	int  getItemMaxDur();					// 最大持久度
	
	int  getRepairPrice();					// 获取维修价格
	int	 getSellPrice();					// 获取出售价格
	int	 getStart();						// 获取武器星级
	int  getArmPefectStarNum();				//获取装备完美星级数目
	int	 getGemStart();						//获取宝石星级
	int  getGemPower();						//获取宝石能量值
	unsigned int	getStallSellPrice();		// 获取物品在摊位上的出售价格
	tdr_ulonglong	getStallGridWID();	// 获取摆摊格位全局唯一ID
	int  getFight();						// 获得战斗力等级
	int  getHoleNum();						// 获取宝石孔数
	int  getHoleGemResultIndex( int nIndex );// 获取宝石的效果
	int  getHoleGemAttrIdx( int nIndex );	//获取宝石属性表索引
	int  getHoleGemId( int nIndex );		// 获取孔位上可镶嵌宝石Id
	int	 getHoleGemColor( int nIndex );		// 获取孔位上可镶嵌宝石颜色
	int	 getHoleGemStar( int nIndex );		//获取孔位上宝石的星级
	int  getHoleGemPower( int nIndex );		//获取孔位上宝石的能量
	int	 getHoleGemMul( int nIndex );		// 获取孔位宝石数值修正值
	int  getItemSurplus();					// 获取物品剩余使用次数
	int  getStrengthenCount();				//获取强化次数

	int  needAmm();							// 是否需要弹药
	bool isAmm();							// 是否是弹药
	bool isExpendable();					// 是否是消耗品
	bool isPetUsed();						// 是否是对宠物使用
	bool isRing();							// 是否是戒指
    bool isAmuleft();						// 是否护身符
	bool isFashionSuit();					// 是否是时装
	bool isItemBind();						// 装备是否绑定
	bool isArmFengYin();					// 装备是否被封印
	bool isArmIdentity();					// 装备是否被鉴定
	unsigned int getUnFengYinArmMoney();	// 解封装备所需要的钱

	void reSetDur();						// 将装备耐久度置为满

	void updateItemData();					// 刷新物品数据
	float	getArmStarAddValue( int nType );	// 获取星级装备增加的值

	int getArmAddAttrNum();					// 获取装备关键字数量
	tagAddAttrInst_ToLua* getArmAddAttr( int nIndex );	// 获取装备关键字

	void releaseItem();	

	const char* getDisplayName(){ return m_strItemName.c_str(); };

	// 获取道具可使用的地图ID
	int getItemUsedMapId();
	// 获取道具有效结束时间
	int getItemValidEndTime();
	//是否附魔物品
	bool isExtend();

	bool isFairyKeyIn();
	//获取附魔配方id
	int getExtendedFromDefID();
	//获取附魔属性
	tagExtendedResult_ToLua* getExtendedResult();
	// 是否鉴定器物品
	bool isIdentity();
	int	 getIdentityLevel();
	bool isBackLevelUp();						//背饰进阶物品
	bool isBackChange();						//背饰改造物品
	//是否是能修理的物品
	bool canRepair();
	bool isArmItem();
	//获得 物品的刻印属性
	const tagAddAttrInst_ToLua* getAddAttrInst();
	int getKeyEndTime();
	bool isValidEnd();
	int getValidLeftTime(){return m_nValidLeft;}
	void setValidLeftTime(int time){m_nValidLeft = time;}

	//获取装备星级强化属性索引
	int getArmAddStarValIdx( int nStarIdx );

	int getItemInstVal1();
	int getItemInstVal2();
	int getItemInstVal3();
	int getItemFlag();

	char getItemMapNum();
	tagItemMapInfo_ToLua* getItemMapInfo( int index);

	int getItemMulVal( int resultId );

	int getArmWashAddAttrNum();
	
	tagAddAttrInst_ToLua* getArmWashAddAttrInst( int idx );

	bool IsAttrFengYin( int nIdx );

	int getLostTotemState();
	int getStepLv();
	int getStepUse();
	int getArmSkillMax();
	int getArmSkillNum();
	tagArmSkillOne_ToLua* getArmSkillOne( int idx );

	int getUnRealBackID();												//获取幻化的背饰ID
	int getUnRealBackTmEnd();										//获取幻化期限
	//tolua_end

	void InitItemName();
	void Update( unsigned int dtick );
public:
	int			m_ItemType;
	tagItemData m_ItemData;
	int			m_ListType;
	bool		m_bCanUsed;
	unsigned int	m_nStallSellPrice;
	tdr_ulonglong	m_StallGridGID;

	std::string m_strItemName;

	int			m_nValidLeft;		//有效期剩余时间
	
	int			m_nLostTotemState;	//失落图腾，0-普通（图标505233），1-激活（图标505234），2-攒满（图标505235）
};//tolua_export
