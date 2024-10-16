#include "GameDBData.h"
#include "Item.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "OgreScriptLuaVM.h"
#include "GameActorManager.h"
#include "ServerTimer.h"
#include "GameMsgPrint.h"

#define NOITEMID	1000

bool bitAnd( int nLhs, int nRhs )
{
	return ( nLhs & nRhs ) != 0;
}

int  bitAndOperation( int nLhs, int nRhs )
{
	return nLhs & nRhs;
}

int  bitOr( int nLhs, int nRhs )
{
	return nLhs | nRhs;
}

int  bitNotAnd( int nLhs, int nRhs )
{
	return nLhs & ~nRhs;
}

bool isItemCanUsed( int nItemSubType )
{
	const int nCanUsedTable[] = {
		ITEM_PILL_HP_ID,				// 药水HP补给类(一次性)
		ITEM_PILL_MP_ID,				// 药水MP补给类(一次性)
		ITEM_PILL_HPLONG_ID,			// 药水HP补给类(持续性)
		ITEM_PILL_MPLONG_ID,			// 药水MP补给类(持续性)
		ITEM_PILL_BUFF_ID,				// 药水状态类
		ITEM_PILL_BUFFLONG_ID,			// 药水BUFF类
		ITEM_PILL_MORPH_ID,				// 药水变身类
		ITEM_PILL_PET_ID,				// 药水宠物类
		ITEM_PILL_HPMP_ID,				// 药水补给类(全恢复)
		ITEM_PILL_HPMPLONG_ID,			// 药水补给类(持续性全恢复)

		ITEM_FOOD_HPLONG_ID,			// 食物HP补给类(持续性)
		ITEM_FOOD_MPLONG_ID,			// 食物MP补给类(持续性)
		ITEM_FOOD_BUFFLONG_ID,			// 食物BUFF类
		ITEM_FOOD_BASEATTR_ID,			// 全回复+一级属性(长)
		ITEM_FOOD_RACEATTR_ID,			// 全回复+职业属性(长)

		ITEM_MAP_POSITION_ID,			// 定位符

		ITEM_SCROLL_SKILL_ID,			// 卷轴技能学习类
		ITEM_SCROLL_ID,					// 卷轴基础行为类
		ITEM_SCROLL_QIYUE_ID,			// 卷轴类 契约
		ITEM_SCROLL_DESIGNATION_ID,		// 卷轴类 称号
		ITEM_NO_DIE_PUNISH_ID,			//死亡赦免BUFF的道具
		ITEM_CLAN_ITEM,					// 公会道具
		ITEM_HELP_PRESENT_ID,			// 辅助礼包类及卷轴
		ITEM_HELP_BOX_ID,				// 辅助类百宝箱
		ITEM_HELP_RAND_ID,				// 辅助类点石成金

		ITEM_TRUST_ID,					// 活动道具委托物品
		ITEM_PROMOTION_ID,				// 参与活动使用道具 藏宝图


		ITEM_GEM_STAR_ID,				// 品质强化宝石及碎片
		ITEM_GEM_STRENGTH_ID,			// 强化保险符及碎片

		ITEM_INLAYGEM_RED_ID,			// 强化保险符及碎片
		ITEM_INLAYGEM_GREEN_ID,			// 强化保险符及碎片
		ITEM_INLAYGEM_BLUE_ID,			// 强化保险符及碎片
		ITEM_INLAYGEM_COLORFUL_ID,		// 强化保险符及碎片
		ITEM_INLAYGEM_OTHER_ID,			// 强化保险符及碎片
		
		ITEM_PET_EXP_ID,
		ITEM_PETXUNFU_ID,				// 宠物等驯服道具
		ITEM_PETLEVELUP_ID,				// 宠物等改造相关 残卷、碎石
		ITEM_FIGHTTING_BOOK1_ID,		// 技能配件相关配件书
		ITEM_FIGHTTING_BOOK2_ID,		// 技能配件相关配件书
		ITEM_FIGHTTING_BOOK3_ID,		// 技能配件相关配件书

		ITEM_LVUP_ID,					// 成长类
		ITEM_HELP_LVUP_ID,				// 辅助类
		ITEM_HELP_MAIL_ID,

		ITEM_TASK_MAINLINE_ID,			// 任务道具(主线任务相关)
		ITEM_TASK_MAINLINE_ID_TASK,		// 任务道具(主线任务相关)

		ITEM_PET_ID,					// 载体
		ITEM_PETSHIEGG_ID,				// 侍卫蛋
		ITEM_PETSHIEGG_ID2,				// 宠物蛋
		ITEM_PETXIANEGG_ID,				// 仙子蛋
		ITEM_TRUMP_ID,

		ITEM_LEARN_SECONDARY_ID1,		//生活技能学习
		ITEM_LEARN_SECONDARY_ID2,
		ITEM_LEARN_SECONDARY_ID3,
	};
	for( int i = 0; i < sizeof( nCanUsedTable ) / sizeof( int ); i++ )
	{
		if( nItemSubType == nCanUsedTable[i] )
		{
			return true;
		}
	}
	return false;
}

int getItemType( int id )
{
	int nType = id/1000000;
	if ( nType == ITEM_IDTYPE_ARM || nType == ITEM_IDTYPE_EQUIP || nType == ITEM_IDTYPE_BARDAIN )
	{
		return ITEM_SLOTTYPE_AMM;
	}else if( nType ==  ITEM_IDTYPE_ITEM ){
		return ITEM_SLOTTYPE_ITEM;
	}else if( id == NOITEMID )
	{
		return ITEM_SLOTTYPE_ITEM;
	}
	assert(0);
	return ITEM_SLOTTYPE_NONE;
}

tagItemDef_ToLua* getItemDef( int id )
{
	static_assert( sizeof(tagItemDef) == sizeof(tagItemDef_ToLua) );
	tagItemDef* pItemDef = const_cast<tagItemDef*>( &DBDataManager::getSingleton().getItemDef( id ) );
	tagItemDef_ToLua* pLuaItemDef = (tagItemDef_ToLua*)( pItemDef );
	return pLuaItemDef;
}

const char* getArmAttrName( int id )
{
	tagArmAddAttrDef* pArmAttrDef = const_cast<tagArmAddAttrDef*>( &DBDataManager::getSingleton().getArmAddAttrDef( id ) );
	return pArmAttrDef->AddAttrName;
}

int getArmAttrValue( int id, int nAttrIdx )
{
	assert( nAttrIdx >= 0 );
	tagArmAddAttrDef* pArmAttrDef = const_cast<tagArmAddAttrDef*>( &DBDataManager::getSingleton().getArmAddAttrDef( id ) );
	return pArmAttrDef->Result[nAttrIdx].ResultVal1;
}


// 获取关键字信息
tagAddAttrDef_ToLua* getAddAttrDef( int id )
{
	static_assert( sizeof(tagAddAttrDef_ToLua) == sizeof(tagAddAttrDef) );
	tagAddAttrDef* pAddAttrDef = const_cast<tagAddAttrDef*>( &DBDataManager::getSingleton().getAddAttrDef(id));
	tagAddAttrDef_ToLua* pLuaAddAttrDef = ( tagAddAttrDef_ToLua* )( pAddAttrDef );
	return pLuaAddAttrDef;
}

tagSuitSet_ToLua* getSuitSet( int id )
{
	static_assert( sizeof( tagSuitSet ) == sizeof( tagSuitSet_ToLua ) );
	tagSuitSet* pSuitSet = const_cast<tagSuitSet*>( &DBDataManager::getSingleton().getSuitSet( id ) );
	tagSuitSet_ToLua* pLuaSuitSet = ( tagSuitSet_ToLua* )( pSuitSet );
	return pLuaSuitSet;
}

tagArmGradeDef_ToLua* getArmGradeDef( int nArmType, int nArmLevel )
{
	static_assert( sizeof( tagArmGradeDef ) == sizeof( tagArmGradeDef_ToLua ) );
	tagArmGradeDef* pArmGrade = const_cast<tagArmGradeDef*>( &DBDataManager::getSingleton().getArmGradeDef( nArmType, nArmLevel ) );
	tagArmGradeDef_ToLua* pLuaArmGrade = ( tagArmGradeDef_ToLua* )( pArmGrade );
	return pLuaArmGrade;
}

tagArmStarDef_ToLua* getArmStarDef( int nStar )
{
	static_assert( sizeof( tagArmStarDef ) == sizeof( tagArmStarDef_ToLua ) );
	tagArmStarDef* pAramStar = const_cast<tagArmStarDef*>( &DBDataManager::getSingleton().getArmStarDef( nStar ) );
	tagArmStarDef_ToLua* pLuaArmStar = ( tagArmStarDef_ToLua* )( pAramStar );
	return pLuaArmStar;
}

tagArmHideAttrDef_ToLua* getArmHideAttrDef( int nCareer, int nArmStarLevel )
{
	static_assert( sizeof( tagArmHideAttrDef ) == sizeof( tagArmHideAttrDef_ToLua ) );
	tagArmHideAttrDef* pArmHideAttr = const_cast<tagArmHideAttrDef*>( &DBDataManager::getSingleton().getArmHideAttrDef( nCareer, nArmStarLevel ) );
	tagArmHideAttrDef_ToLua* pLuaArmHideAttr = ( tagArmHideAttrDef_ToLua* )( pArmHideAttr );
	return pLuaArmHideAttr;
} 

tagArmAddAttrDef_ToLua* getArmAddAttrDef( int id )
{
	static_assert( sizeof( tagArmAddAttrDef ) == sizeof( tagArmAddAttrDef_ToLua ) );
	tagArmAddAttrDef* pArmAttrDef = const_cast<tagArmAddAttrDef*>( &DBDataManager::getSingleton().getArmAddAttrDef( id ) );
	return (tagArmAddAttrDef_ToLua*)pArmAttrDef;
}

tagAddAttrBaseDef_ToLua* getAddAttrBaseDef( int nLv )
{
	static_assert( sizeof( tagAddAttrBaseDef_ToLua ) == sizeof( tagAddAttrBaseDef ) );
	return ( tagAddAttrBaseDef_ToLua* )DBDataManager::getSingleton().getAddAttrBaseDef( nLv );
}

tagFixedAddAttr_ToLua* getFixedAddAttr( int id )
{
    static_assert( sizeof( tagFixedAddAttr_ToLua ) == sizeof( tagFixedAddAttr ) );
    return ( tagFixedAddAttr_ToLua* )DBDataManager::getSingleton().getFixedAddAttr( id );
}

const tagNewAddAttrDef_ToLua* getNewAddAttrDef( int id )
{
	static_assert( sizeof( tagNewAddAttrDef_ToLua ) == sizeof( tagNewAddAttrDef ) );
	return ( const tagNewAddAttrDef_ToLua* )DBDataManager::getSingleton().getNewAddAttrDef( id );
}

const tagAddAttrRandDef_ToLua* getAddAttrRandDef( int level, int important )
{
	static_assert( sizeof( tagAddAttrRandDef_ToLua ) == sizeof( tagAddAttrRandDef ) );
	return ( const tagAddAttrRandDef_ToLua* )DBDataManager::getSingleton().getAddAttrRandDef( level, important );
}

const tagItemResultDesc_ToLua* getItemResultDesc( int id )
{
	static_assert( sizeof(tagItemResultDesc_ToLua) == sizeof(tagItemResultDesc) );
	return( const tagItemResultDesc_ToLua* )DBDataManager::getSingleton().getItemResultDescDef( id );
}

const tagBaoXiangDef_ToLua* getBaoXiangDef( int id )
{
	static_assert( sizeof(tagBaoXiangDef_ToLua) == sizeof(tagBaoXiangDef) );
	return ( const tagBaoXiangDef_ToLua* ) DBDataManager::getSingleton().getBaoXiangDef( id );
}

const std::string& getItemColor( int important, int suitsetId )
{
	static std::string strImportantColor[] ={
		"fef9ea",	 // 白色(普通品质)
		"42a83d",	 // 绿色(优秀品质)
		"3d6da8",	 // 蓝色(精良品质)
		"972df9",	 // 紫色(极品品质)
		"ffb9ff",	 // 套装，浅紫
		"ffb9ff",	 // 套装，浅紫
		"ffb9ff",	 // 套装，浅紫
		"ca3621",	// 橙
		"ca3621",	// 橙
		"82ff87",	 // 套装，浅绿
		"a0ffff",	 // 套装，浅蓝
		"ffb9ff",	 // 套装，浅紫
	};
	if( suitsetId > 0 )
	{
		tagSuitSet_ToLua* suitDef = getSuitSet( suitsetId );
		if( suitDef != NULL && suitDef->SuitSetType <= 3 )
		{
			return strImportantColor[ 8 + suitDef->SuitSetType ];
		}	 
	}
// 	assert( important < ITEM_IMPORTANT_END );
	
	return strImportantColor[important];
}


// 计算额外的耐久度
int getArmExtraDur( tagItemDef_ToLua* pItemDef )
{
	if( pItemDef == NULL )
	{
		return 0;
	}

	int i, iExtraDur = 0;

	i = MAX_ITEM_RESULT;
	while( i-- )
	{
		// 计算额外的耐久度上限
		if(pItemDef->Result[i].ResultID == RESULT_ADD_MAX_DUR)
		{
			iExtraDur = (pItemDef->Result[i].ResultVal1 * pItemDef->ArmDur.MaxDur) / 100 + pItemDef->Result[i].ResultVal2;
			break;
		}
	}
	
	// 计算关键字增加耐久度
	for( int i=0; i < MAX_ITEM_ADD_ATTR; i++ )	
	{
		if (0 == pItemDef->AddAttr[i].ID)
		{
			break;
		}

		if (ADD_ATTR_RESULT_MAX_DUR == pItemDef->AddAttr[i].ID)
		{
			iExtraDur += getArmAttrValue( pItemDef->AddAttr[i].ID,(int) (( pItemDef->AddAttr[i].Idx - 1 )*pItemDef->ArmDur.MaxDur/100.0f) );	
		}
	}

	return iExtraDur;
}

// 获取出售价格
int arm_sell_price( ROLEARM *pstRoleArm, tagItemDef_ToLua *pstItemDef )
{
	int iPrice;
	if ( pstItemDef->ArmDur.MaxDur > 0 )
	{
		iPrice = (int)(((pstRoleArm->Dur/DUR_UNIT)*1.0f/(pstItemDef->ArmDur.MaxDur + getArmExtraDur( pstItemDef )))*pstItemDef->SellPrice);
	}else{
		iPrice = pstItemDef->SellPrice;
	}
	return iPrice;
}

// 获取全部维修的时候单个物品的维修价格
int arm_repairAll_price( ROLEARM *pstRoleArm, tagItemDef_ToLua *pstItemDef )
{
	int iPrice = 0;
	if ( pstItemDef->ArmDur.MaxDur > 0 )
	{
		if ( pstRoleArm->Dur >= pstItemDef->ArmDur.MaxDur * DUR_UNIT )
		{
			return 0;
		}

		// 持久为0     维修系数1.5      否则0.4
		if (pstRoleArm->Dur <= 0)
		{
			iPrice = (int)floor( pstItemDef->RepairBull * BROKEN/100.0 );
		}
		else
		{
			iPrice = (int)floor( (1.0 - (pstRoleArm->Dur/DUR_UNIT)*1.0/(pstItemDef->ArmDur.MaxDur + 
				getArmExtraDur( pstItemDef )))*pstItemDef->RepairBull * NOTBROKEN/100 );
		}

		if ( iPrice < 0 )
		{
			iPrice = 0;
		}

	}
	else
	{
		iPrice = 0;
	}

	return iPrice;
}

// 获取维修价格
int arm_repair_price( ROLEARM *pstRoleArm, tagItemDef_ToLua *pstItemDef )
{
	if ( pstItemDef->ArmDur.MaxDur <= 0 )
	{
		return 0;
	}
	
	if ( pstRoleArm->Dur >= pstItemDef->ArmDur.MaxDur * DUR_UNIT )
	{
		return 0;
	}
	
	int iPrice;
	// 持久为0     维修系数1.5      否则0.4
	if (pstRoleArm->Dur <= 0)
	{
		iPrice = pstItemDef->RepairBull * BROKEN / 100.0;
	}
	else
	{
		unsigned int uDurAdjustValue = pstRoleArm->Dur/DUR_UNIT;
		int nAdjustMaxDur = pstItemDef->ArmDur.MaxDur + getArmExtraDur( pstItemDef );
		iPrice = ( 1.0 - uDurAdjustValue * 1.0 / nAdjustMaxDur ) * pstItemDef->RepairBull * NOTBROKEN / 100;
	}

	return iPrice;
}

int z_get_arm_result_total(tagItemDef_ToLua *pstItemDef, RESULT *pstResult, int iResultID)
{
	int i;
	memset(pstResult, 0, sizeof(*pstResult));
	for (i=0; i<MAX_ITEM_RESULT; i++)
	{
		if (0 ==pstItemDef->Result[i].ResultID)
		{
			//break;
			continue;
		}

		if (pstItemDef->Result[i].ResultID == iResultID)
		{
			pstResult->ResultVal1 += pstItemDef->Result[i].ResultVal1;
			pstResult->ResultVal2 += pstItemDef->Result[i].ResultVal2;
			pstResult->ResultVal3 += pstItemDef->Result[i].ResultVal3;
			pstResult->ResultVal4 += pstItemDef->Result[i].ResultVal4;
		}
	}
	return 0;
}

//获取 NumType
int itemid_num( tagItemDef_ToLua *pstItemDef )
{
	int iTypeNum;
	char szItemID[16];

	memset(szItemID, 0, sizeof(szItemID));
	sprintf(szItemID, "%u", pstItemDef->ItemID);

	//取前3 位
	szItemID[3] = 0;
	iTypeNum = atoi(szItemID);
	return iTypeNum;
}

bool isSameWid( tdr_ulonglong lhs, tdr_ulonglong rhs )
{
	return lhs == rhs;
}

Item::Item(void) : m_ListType(0), m_nStallSellPrice(0),m_nValidLeft(0),m_nLostTotemState(0)
{
	releaseItem();
	m_bCanUsed = true;
	m_ItemType = ITEM_SLOTTYPE_DISABLE; 
}

Item::~Item(void)
{
}

void Item::setItemDate( int type, void* pValue )
{
	if( type == GRID_ITEM_ITEM )
	{
		m_ItemType = ITEM_SLOTTYPE_ITEM;
		m_ItemData.m_Item = *((ROLEITEM*)pValue);
	}else if( type == GRID_ITEM_ARM ){ 
		m_ItemType = ITEM_SLOTTYPE_AMM;
		m_ItemData.m_Arm = *((ROLEARM*)pValue);	
	}
	InitItemName();
}

tagItemDef_ToLua* Item::getItemDef()
{
	static_assert( sizeof(tagItemDef) == sizeof(tagItemDef_ToLua) );
	if( getItemId() == 0 )
	{
		return NULL;
	}
	tagItemDef* pItemDef = const_cast<tagItemDef*>( &DBDataManager::getSingleton().getItemDef( getItemId() ) );
	tagItemDef_ToLua* pLuaItemDef = (tagItemDef_ToLua*)( pItemDef );
	return pLuaItemDef;
}

int Item::getItemId()
{
	if( ITEM_SLOTTYPE_NONE == m_ItemType || ITEM_SLOTTYPE_DISABLE == m_ItemType )
	{
		return 0;
	}
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return m_ItemData.m_Arm.DefID;
	}
	return m_ItemData.m_Item.DefID;
}

tdr_ulonglong Item::getItemWId()
{
	if( ITEM_SLOTTYPE_NONE == m_ItemType || ITEM_SLOTTYPE_DISABLE == m_ItemType )
	{
		return 0;
	}
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return m_ItemData.m_Arm.WID;
	}
	return m_ItemData.m_Item.WID;
}

bool Item::canUsed()
{
	if( m_ItemType == ITEM_SLOTTYPE_NONE || m_ItemType == ITEM_SLOTTYPE_DISABLE )
	{
		return false;
	}

	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		int itemSubType = m_ItemData.m_Item.DefID/1000;
		if( isItemCanUsed( itemSubType ) )
		{
			int itemCD = (int)ActorManager::getSingleton().getMainPlayer()->getContainer().getItemCD( getItemId() );
			if( itemCD > 0 )
			{
				return false;
			}
		}
	}

	return m_bCanUsed;
}

bool Item::canExchange()
{
	tagItemDef* pItemDef = const_cast<tagItemDef*>( &DBDataManager::getSingleton().getItemDef( getItemId() ) );
	if ( pItemDef->NoDeal || pItemDef->NoDrop || pItemDef->ItemType == ITEM_TYPE_TASK )
	{
		return false;
	}
	return true;
}

int Item::getGridIdx()
{
	if( ITEM_SLOTTYPE_NONE == m_ItemType || ITEM_SLOTTYPE_DISABLE == m_ItemType )
	{
		return 0;
	}
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return m_ItemData.m_Arm.GridIdx;
	}
	return m_ItemData.m_Item.GridIdx;
}

int	Item::getListType()
{
	return m_ListType;
}

int Item::getItemType()
{
	return m_ItemType;
}

// 获取道具子类型
int Item::getItemSubType()
{
	if( ITEM_SLOTTYPE_NONE == m_ItemType || ITEM_SLOTTYPE_DISABLE == m_ItemType )
	{
		return 0;
	}
	
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		return m_ItemData.m_Item.DefID/1000;

	}else if( ITEM_SLOTTYPE_AMM == m_ItemType )
	{
		return m_ItemData.m_Arm.DefID/1000;
	}

	return 0;
}

int Item::getNum()
{
	if( ITEM_SLOTTYPE_NONE == m_ItemType || ITEM_SLOTTYPE_DISABLE == m_ItemType )
	{
		return 0;
	}
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return m_ItemData.m_Arm.Num;
	}
	return m_ItemData.m_Item.Num;
}

void Item::setNum( int nNUm )
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		m_ItemData.m_Arm.Num = nNUm;
		return;
	}
	m_ItemData.m_Item.Num = nNUm;
}

int Item::getItemDur()
{
	if( ITEM_SLOTTYPE_NONE == m_ItemType || ITEM_SLOTTYPE_DISABLE == m_ItemType || ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		return -1;
	}
	return m_ItemData.m_Arm.Dur/DUR_UNIT;
}

int Item::getItemMaxDur()
{	
	if( ITEM_SLOTTYPE_NONE == m_ItemType || ITEM_SLOTTYPE_DISABLE == m_ItemType || ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		return -1;
	}
	return getItemDef()->ArmDur.MaxDur + getArmExtraDur( getItemDef() );
}

int Item::getRepairPrice()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}

	return arm_repair_price( &m_ItemData.m_Arm, getItemDef() );
}

int Item::getSellPrice()
{
	return 	arm_sell_price( &m_ItemData.m_Arm, getItemDef() );
}

int Item::getStart()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.StarLv;
}

int Item::getStrengthenCount()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.StrengthenCount;
}

int Item::getArmPefectStarNum()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	int perfectStarNum = 0;
	for( int i = 0; i < m_ItemData.m_Arm.StarLv; i++ )
	{
		if ( m_ItemData.m_Arm.StarInfo[ i ].ValIdx >= MAX_STAR_MUL_LIST -1 )
		{
			perfectStarNum++;
		}
	}
	return perfectStarNum;
}

int Item::getGemStart()
{
	return m_ItemData.m_Item.InstVal1;
}
int Item::getGemPower()
{
	return m_ItemData.m_Item.InstVal2;
}

unsigned int Item::getStallSellPrice()
{
	return m_nStallSellPrice;
}

// 获取摆摊格位全局唯一ID
tdr_ulonglong	Item::getStallGridWID()
{
	return m_StallGridGID;
}

int Item::getFight()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.FightLv;
}

int Item::getHoleNum()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.HoleNum;
}

// 获取宝石的效果
int  Item::getHoleGemResultIndex( int nIndex )
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.HoleGem[nIndex].HoleGemResultIndex;
}
int Item::getHoleGemAttrIdx( int nIndex )
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.HoleGem[nIndex].GemAttrIdx;
}

// 获取孔位上可镶嵌宝石Id
int Item::getHoleGemId( int nIndex )
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.HoleGem[nIndex].HoleGemID;
}

// 获取孔位上可镶嵌宝石颜色
int Item::getHoleGemColor( int nIndex )
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return ( int )m_ItemData.m_Arm.HoleGem[nIndex].HoleColor;
}
//获取孔位上宝石的星级
int Item::getHoleGemStar( int nIndex )
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return ( int )m_ItemData.m_Arm.HoleGem[nIndex].GemStar;
}

//获取孔位上宝石的能量
int Item::getHoleGemPower( int nIndex )
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return ( int )m_ItemData.m_Arm.HoleGem[nIndex].GemPower;
}
// 获取孔位宝石数值修正值
int Item::getHoleGemMul( int nIndex )
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return ( int )m_ItemData.m_Arm.HoleGem[nIndex].Mul;
}

// 获取物品剩余使用次数
int Item::getItemSurplus()
{
	if( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		const tagItemDef& ItemDef = DBDataManager::getSingleton().getItemDef( getItemId() );
		if( ItemDef.UseMaxDur > 0 )
		{
			return m_ItemData.m_Item.InstVal1;
		}
	}
	return -1;
}

int Item::needAmm()
{
	if( m_ItemType != ITEM_SLOTTYPE_NONE )
	{
		const tagItemDef& itemDef = DBDataManager::getSingleton().getItemDef( getItemId() );
		if( itemDef.WeaponAmmNeed != 0 )
		{
			return itemDef.WeaponAmmType;
		}
	}
	return UNUSED_AMM;
}

bool Item::isAmm()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		if( m_ItemData.m_Item.DefID/10000 == AMM_ID || m_ItemData.m_Item.DefID/10000 == AMM_ID_TEM )	
		{
			return true;
		}
	}

	return false;
}

bool Item::isExpendable()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		int itemSubType = m_ItemData.m_Item.DefID/1000;
		if( isItemCanUsed( itemSubType ) )
		{
			return true;
		}
	}
	return false;
}

bool Item::isPetUsed()
{
	assert( getItemId() != 0 );
	const tagItemDef& itemDef  = DBDataManager::getSingleton().getItemDef( getItemId() ); 
	return itemDef.UseTarget == USE_TARGET_PET ? true : false;
}

// 是否是戒指
bool Item::isRing()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType )
	{
		tagItemDef itemDef = DBDataManager::getSingleton().getItemDef( getItemId() );
		if( itemDef.WearPos1 == EQUIP_POS_RING || itemDef.WearPos1 == EQUIP_POS_AMULET1 )
		{
			return true;
		}
	}
	return false;
}

bool Item::isAmuleft()
{
    if( ITEM_SLOTTYPE_AMM == m_ItemType )
    {
        tagItemDef itemDef = DBDataManager::getSingleton().getItemDef( getItemId() );
        if( itemDef.WearPos1 == WEAR_POS_AMULET )
        {
            return true;
        }
    }
    return false;
    
};
bool Item::isFashionSuit()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType )
	{
		tagItemDef itemDef = DBDataManager::getSingleton().getItemDef( getItemId() );
		if( itemDef.WearPos1 == EQUIP_POS_FASHION_BODY || itemDef.WearPos1 == EQUIP_POS_FASHION_HEAD 
			|| itemDef.WearPos1 == EQUIP_POS_FASHION_HAIR || itemDef.WearPos1 == EQUIP_POS_FASHION_BACK
			|| itemDef.WearPos1 == EQUIP_POS_FASHION_SHOES || itemDef.WearPos1 == EQUIP_POS_FASHION_HAND
			|| itemDef.WearPos1 == EQUIP_POS_FASHION_LEG )
		{
			return true;
		}
	}
	return false;
}
// 装备是否绑定
bool Item::isItemBind()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType )
	{
		if( ( m_ItemData.m_Arm.InstFlag & INST_ITEM_BIND ) != 0 )
		{
			return true;
		}
	}else if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		if( ( m_ItemData.m_Item.InstFlag & INST_ITEM_BIND ) != 0)
		{
			return true;
		}
	}
	return false;
}

// 装备是否被封印
bool Item::isArmFengYin()
{
	if ( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		return false;
	}

	if ( m_ItemType == ITEM_SLOTTYPE_AMM )
	{
		return m_ItemData.m_Arm.FengyinFlag > 0;
	}

	return false;
}

// 装备是否鉴定
bool Item::isArmIdentity()
{
	if ( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		return false;
	}

	if ( m_ItemType == ITEM_SLOTTYPE_AMM )
	{
		if ( ( m_ItemData.m_Arm.InstFlag & INST_ITEM_UNCHECK ) != 0 )
		{
			return true;
		}
	}
	return false;
}

// 是否是鉴定器
bool Item::isIdentity()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		tagItemDef_ToLua* pItemDef = getItemDef();
		for( int i = 0 ; i < MAX_ITEM_RESULT ; i ++ )
		{
			if( pItemDef->Result[i].ResultID == RESULT_ARM_CHECK )
			{
				return true;
			}
		}
	}
	return false;	
}

// 获得鉴定器最低等级
int	 Item::getIdentityLevel()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		tagItemDef_ToLua* pItemDef = getItemDef();
		for( int i = 0 ; i < MAX_ITEM_RESULT ; i ++ )
		{
			if( pItemDef->Result[i].ResultID == RESULT_ARM_CHECK )
			{
				return pItemDef->Result->ResultVal1;
			}
		}
	}
	return 0;	
}
// 是否背饰进阶卷轴
bool Item::isBackLevelUp()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		tagItemDef_ToLua* pItemDef = getItemDef();
		for( int i = 0 ; i < MAX_ITEM_RESULT ; i ++ )
		{
			if( pItemDef->Result[i].ResultID == RESULT_BACK_LEVELUP )
			{
				return true;
			}
		}
	}
	return false;	
}
// 是否背饰改造卷轴
bool Item::isBackChange()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		tagItemDef_ToLua* pItemDef = getItemDef();
		for( int i = 0 ; i < MAX_ITEM_RESULT ; i ++ )
		{
			if( pItemDef->Result[i].ResultID == RESULT_BACK_CHG_TYPE )
			{
				return true;
			}
		}
	}
	return false;	
}
// 解封装备所需要的钱
unsigned int Item::getUnFengYinArmMoney()
{
	if ( !isArmFengYin() )
	{
		return 0;
	}

	const tagItemDef_ToLua* pItemDef = getItemDef();
	if ( pItemDef == NULL )
	{
		return 0;
	}

	const ROLEARM&			oneRoleArmInfo	= m_ItemData.m_Arm;
	const tagArmRandDef*	pOneArmRandDEF	=  DBDataManager::getSingleton().getArmRandDef( getArmPefectStarNum(), ARM_UNFENGYIN_TYPE );
	if ( pOneArmRandDEF == NULL )
	{
		return 0;
	}

	unsigned int uiGenius = 0;
	for ( int i = 0; i < MAX_ARM_HOLE; ++i )
	{
		const HOLEGEM& oneHoleGemInfo = oneRoleArmInfo.HoleGem[i];
		if ( oneHoleGemInfo.HoleGemID <= 0 )
		{
			continue;
		}

		const tagGemNewDef* pGemDef = &DBDataManager::getSingleton().getGemNewDef( oneHoleGemInfo.HoleGemID );
		if ( pGemDef == NULL )
		{
			continue;
		}

		uiGenius += pGemDef->GemMerit;
	}

	unsigned int uPinZhiScale  = 0;
	if ( pItemDef->Important == ARM_NONE_PIN_ZHI )
	{
		uPinZhiScale = 80;
	}
	else if ( pItemDef->Important == ARM_GREEN_PIN_ZHI )
	{
		uPinZhiScale = 100;
	}
	else if ( pItemDef->Important == ARM_BLUE_PIN_ZHI )
	{
		uPinZhiScale = 150;
	}
	else if ( pItemDef->Important > ARM_BLUE_PIN_ZHI && pItemDef->Important < ITEM_IMPORTANT_LEGEND )
	{
		uPinZhiScale = 300;
	}
	else 
	{
		uPinZhiScale = 500;
	}

	unsigned int uTotoalCostMoney = 1.0f * pOneArmRandDEF->ArmUnFengYinUse * uPinZhiScale / 100 *
					pItemDef->WearPre.RoleLevelMin / 10000 + uiGenius;

	return uTotoalCostMoney > 0 ? uTotoalCostMoney : 1;
}

void Item::reSetDur()
{
	assert( getItemId() != 0 );
	m_ItemData.m_Arm.Dur = getItemMaxDur()*DUR_UNIT;
}

void Item::releaseItem()
{	
	m_ItemType	= ITEM_SLOTTYPE_NONE;
	m_bCanUsed	= false;
	m_ListType	= 0;
	m_strItemName = "";
	m_nValidLeft = 0;
	memset( &m_ItemData, 0, sizeof(tagItemData) );
}

// 获取道具可使用的地图ID
int Item::getItemUsedMapId()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		for( int i=0; i < MAX_ITEM_RESULT; i++ )
		{
			if ( RESULT_TREASURE_PIC == getItemDef()->Result[i].ResultID)
			{
				return m_ItemData.m_Item.Flag;
			}
		}
	}
	return -1;
}

// 获取道具有效结束时间
int Item::getItemValidEndTime()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return m_ItemData.m_Arm.ValidEnd;
	}else if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		return m_ItemData.m_Item.ValidEnd;
	}
	return 0;
}

void Item::updateItemData()
{
	if( ITEM_SLOTTYPE_NONE == m_ItemType || ITEM_SLOTTYPE_DISABLE == m_ItemType )
	{
		return;
	}

	MainGamePlayer* pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	if( pMainPlayer != NULL )
	{
		Item& srcItem = pMainPlayer->getContainer().getItem( getListType(), getGridIdx() );
		m_ItemType	= srcItem.m_ItemType;
		m_ItemData	= srcItem.m_ItemData;
		InitItemName();
	}
}	

// 获取星级装备增加的值
float Item::getArmStarAddValue( int nType )
{
	float fAddAttr = 0.0;
	if( ITEM_SLOTTYPE_AMM != m_ItemType || m_ItemData.m_Arm.StarLv <= 0 || m_ItemData.m_Arm.StarLv > MAX_ARM_STAR_LEVEL )
	{
		return fAddAttr;
	}

	RESULT stResult;
	tagItemDef_ToLua* pItemDef = getItemDef();
	const float COEFICIENCY = 100.0f;
	tagStarAttr_ToLua* pstStarAttr = &getArmGradeDef( getItemId()/10000, pItemDef->WearPre.RoleLevelMin / 10 )->ArmStar[m_ItemData.m_Arm.StarLv-1];
	z_get_arm_result_total( getItemDef(), &stResult, nType);
	tagArmStarDef_ToLua* pArmStarDef;
	int nMul = 100;
	for ( int i = 0; i < m_ItemData.m_Arm.StarLv ; i++ )
	{
		pArmStarDef = getArmStarDef( i + 1 );
		if ( pArmStarDef == NULL )
		{
			continue;
		}
		nMul *= ( pArmStarDef->StarInfo[m_ItemData.m_Arm.StarInfo[i].ValIdx].Mul +100 );
		nMul /= 100;
	}
	fAddAttr = nMul - 100;
// 	switch( nType )
// 	{
// 	case RESULT_WEAPON_MIN_PHSIC_ATK:
// 	case RESULT_WEAPON_MAX_PHSIC_ATK:
// 	case RESULT_BASE_ATTR_PHSIC_ATK:
// 		fAddAttr = pstStarAttr->PhsicAtkMul;
// 		break;
// 	case RESULT_WEAPON_MIN_MAGIC_ATK:
// 	case RESULT_WEAPON_MAX_MAGIC_ATK:
// 	case RESULT_BASE_ATTR_MAGIC_ATK:
// 		fAddAttr = pstStarAttr->IceAtkMul;
// 		break;
// 	case RESULT_BASE_ATTR_HP:
// 		fAddAttr = pstStarAttr->MaxHPMul;
// 		break;
// 	case RESULT_BASE_ATTR_PHSIC_DEF:
// 		fAddAttr = pstStarAttr->PhsicDefMul;
// 		break;
// 	case RESULT_BASE_ATTR_MAGIC_DEF:
// 		fAddAttr = pstStarAttr->IceDefMul;
// 		break;
// 	case RESULT_BASE_ATTR_WIND_DEF:
// 		fAddAttr = pstStarAttr->WaterDefMul;
// 		break;
// 	case RESULT_BASE_ATTR_SOIL_DEF:
// 		fAddAttr = pstStarAttr->FireDefMul;
// 		break;
// 	case RESULT_BASE_ATTR_THUNDER_DEF:
// 		fAddAttr = pstStarAttr->ThunderDefMul;
// 		break;
// 	case RESULT_BASE_ATTR_LIGHT_DEF:
// 		fAddAttr = pstStarAttr->LightDefMul;
// 		break;
// 	case RESULT_BASE_ATTR_NIGHT_DEF:
// 		fAddAttr = pstStarAttr->FleeMul;
// 		break;
// 	case RESULT_BASE_ATTR_HIT_LEVEL:
// 		fAddAttr = pstStarAttr->HitRateMul;
// 		break;
// 	case RESULT_BASE_ATTR_HEAVY_HIT_VAL:
// 		fAddAttr = pstStarAttr->HeavyHitValMul;
// 		break;
// 	default:
// 		break;
// 	}
// 	switch( nType )
// 	{
// 	case RESULT_HPMAX:
// 		//血量上限
// 		fAddAttr = ( ( stResult.ResultVal1 + stResult.ResultVal3 ) * pstStarAttr->MaxHPMul/COEFICIENCY );
// 		break;
// 	case RESULT_MPMAX:
// 		//MP 上限
// 		fAddAttr = (stResult.ResultVal1*pstStarAttr->MaxMPMul/COEFICIENCY);
// 	    break;
// 	case RESULT_DEF_ICEHURT:
// 		//冰抗
// 		fAddAttr = (stResult.ResultVal2*pstStarAttr->IceHurtDefMul/COEFICIENCY);
// 	    break;
// 	case RESULT_DEF_PHISICHURT:
// 		//物抗
// 		fAddAttr = (stResult.ResultVal2*pstStarAttr->PhsicHurtDefMul/COEFICIENCY);
// 		break;
// 	case RESULT_MON_ICE_HURT:
// 		//只对怪物起作用冰抗
// 		fAddAttr = (stResult.ResultVal2*pstStarAttr->IceHurtDefMul/COEFICIENCY);
// 		break;
// 	case RESULT_MON_PHSIC_HURT:
// 		//只对怪物起作用物抗
// 		fAddAttr = (stResult.ResultVal2*pstStarAttr->PhsicHurtDefMul/COEFICIENCY);
// 	    break;
// 	case RESULT_PHSIC_DEF:
// 		//物防
// 		fAddAttr = (stResult.ResultVal1*pstStarAttr->PhsicDefMul/COEFICIENCY);
// 		break;
// 	case RESULT_ICE_DEF:
// 		//冰防
// 		fAddAttr = (stResult.ResultVal1*pstStarAttr->IceDefMul/COEFICIENCY);
// 		break;
// 	case RESULT_ARM_HPADD:
// 		//随时间恢复HP
// 		fAddAttr = (stResult.ResultVal1*pstStarAttr->HPRecoverMul/COEFICIENCY);
// 		break;
// 	case RESULT_ARM_MPADD:
// 		//随时间恢复MP
// 		fAddAttr = (stResult.ResultVal1*pstStarAttr->MPRecoverMul/COEFICIENCY);
// 		break;
// 	case RESULT_MON_FLEE:
// 		//只对怪物起作用闪避
// 		fAddAttr = (stResult.ResultVal2*pstStarAttr->FleeMul/COEFICIENCY);
// 		break;
// 	case RESULT_FLEE:
// 		//闪避
// 		fAddAttr = (stResult.ResultVal1*pstStarAttr->FleeMul/COEFICIENCY);
// 		break;
// 	case RESULT_HITRATE:
// 		//命中
// 		fAddAttr =  (stResult.ResultVal2*pstStarAttr->HitRateMul/COEFICIENCY);
// 		break;
// 	case RESULT_WATER_DEF:
// 		//风防御
// 		fAddAttr =  (stResult.ResultVal1*pstStarAttr->WaterDefMul/COEFICIENCY);
// 		break;
// 	case RESULT_FIRE_DEF:
// 		//土防御
// 		fAddAttr =  (stResult.ResultVal1*pstStarAttr->FireDefMul/COEFICIENCY);
// 		break;
// 	case RESULT_THUNDER_DEF:
// 		//电防御
// 		fAddAttr =  ( stResult.ResultVal1 * pstStarAttr->ThunderDefMul / COEFICIENCY );
// 		break;
// 	case RESULT_LIGHT_DEF:
// 		//光防御
// 		fAddAttr =  (stResult.ResultVal1*pstStarAttr->LightDefMul/COEFICIENCY);
// 		break;
// 	case RESULT_NIGHT_DEF:
// 		//暗防御
// 		fAddAttr =  (stResult.ResultVal1*pstStarAttr->NightDefMul/COEFICIENCY);
// 		break;
// 	case RESULT_ICE_ATK:
// 		//物理攻击力
// 		fAddAttr =  (stResult.ResultVal1*pstStarAttr->IceAtkMul/COEFICIENCY);
// 		break;
// 	case RESULT_PHSIC_ATK:
// 		//魔法攻击力
// 		fAddAttr =  (stResult.ResultVal1*pstStarAttr->PhsicAtkMul/COEFICIENCY);
// 		break;
// 
// 	case RESULT_HEAVY_HIT_VAL:
// 		//暴击伤害
// 		fAddAttr =  (stResult.ResultVal1*pstStarAttr->HeavyHitValMul/COEFICIENCY);
// 		break;
// 	default:
// 	    break;
// 	}
	return fAddAttr;
}

//获取装备星级强化属性索引
int Item::getArmAddStarValIdx( int nStarIdx )
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.StarInfo[nStarIdx].ValIdx;
}

// 获取装备关键字数量
int Item::getArmAddAttrNum()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return m_ItemData.m_Arm.AddAttrNum;
	}

	return 0;
}

// 获取装备关键字
tagAddAttrInst_ToLua* Item::getArmAddAttr( int nIndex)
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return (tagAddAttrInst_ToLua*)&m_ItemData.m_Arm.AddAttr[nIndex];
	}

	return NULL;
}

void Item::InitItemName()
{
	static_assert(sizeof(tagRoleArm) == sizeof(tagRoleArm_ToLua)); 
	m_nValidLeft = 0;
	tagItemDef_ToLua* pItemDef = getItemDef();
	if( pItemDef == NULL )
	{
		m_strItemName = "";
		return;
	}
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		m_strItemName = pItemDef->Name;
	}else if( ITEM_SLOTTYPE_AMM == m_ItemType ){	
		//套装
		char szName[RES_NAME_LEN] = {0};
		if ( ::getItemType( pItemDef->ItemID ) != ITEM_SLOTTYPE_AMM )
		{
			m_strItemName = pItemDef->Name;
		}
		else
		{
			ActorManager::getSingleton().getScriptVM()->callFunction("GetArmName","iu[tagRoleArm_ToLua]>s",
				m_ItemData.m_Arm.AddAttrNum,
				&m_ItemData.m_Arm,
				szName);
			m_strItemName = szName;
			m_strItemName += pItemDef->Name;
		}
		//装备星级的名称
		if (getStart() > 0)
		{
			sprintf(szName," +%d",getStart());
			m_strItemName += szName;
		}

		if ( isArmFengYin() )
		{
			m_strItemName += "（部分封印）";
		}
	}
}

const char* getDisplayName(tagItemDef_ToLua* pItemDef )
{
	static_assert( sizeof(tagItemDef_ToLua) ==sizeof(tagItemDef) );
	if(pItemDef == NULL )
	{
		return "";
	}
	static std::string strResult;

	if(!pItemDef->RandAddAttr)
	{
		char szName[RES_NAME_LEN] = {0};
		ActorManager::getSingleton().getScriptVM()->callFunction("GetStaticArmName","u[tagItemDef_ToLua]>s",
			pItemDef,
			szName);
		strResult = szName;
		strResult = strResult + pItemDef->Name;
	}else{
		strResult = pItemDef->Name;
	}
	return strResult.c_str();
}

bool Item::isExtend()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		return ( getItemDef()->ExtendedType != EXTENDED_TYPE_NONE );
	}
	return false;
}

//是否是刻印卷轴
bool Item::isFairyKeyIn()
{
	if( ITEM_SLOTTYPE_ITEM == m_ItemType )
	{
		tagItemDef_ToLua* pItemDef = getItemDef();
		for( int i = 0 ; i < MAX_ITEM_RESULT ; i ++ )
		{
			if( pItemDef->Result[i].ResultID == RESULT_ELEMENT_ATK ||
				pItemDef->Result[i].ResultID == RESULT_ELEMENT_DEF )
			{
				return true;
			}
		}
	}
	return false;	
}

const tagAddAttrInst_ToLua* Item::getAddAttrInst()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType )
	{
		return (const tagAddAttrInst_ToLua*)&m_ItemData.m_Arm.KeYinAddAttr;
	}
	return NULL;
}

int Item::getKeyEndTime()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType )
	{
		return m_ItemData.m_Arm.KeYinEnd;
	}
	return 0;
}

int Item::getExtendedFromDefID()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType )
	{
		return m_ItemData.m_Arm.ExtendedFromDefID;
	}
	return 0;
};

tagExtendedResult_ToLua* Item::getExtendedResult()
{
	static_assert( sizeof(tagExtendedResult_ToLua) == sizeof(tagExtendedResult) );
	if( ITEM_SLOTTYPE_AMM == m_ItemType )
	{
		return (tagExtendedResult_ToLua*) &m_ItemData.m_Arm.ExtendedResult;
	}
	return NULL;
}

int Item::getDisplayIndex(int id)
{
	static const int range[][3] = {//缺少枪手武器和背、时装、饰品、项链
		{102, 1000000, 1010000},// 剑
		{101, 1010000, 1020000},// 刀
		{102, 1030000, 1040000},// 对手剑
		{107, 1060000, 1070000},// 盾
		{103, 1100000, 1180000},// 魔杖
		{105, 1180000, 1190000},// 法器
		{103, 1500000, 1570000},// 神杖
		{105, 1580000, 1590000},// 法器
		{108, 2000000, 2100000},// 衣
		{106, 2100000, 2200000},// 帽
		{107, 2200000, 2300000},// 肩
		{109, 2400000, 2500000},// 面
		{111, 2500000, 2600000},// 饰品
		{110, 2600000, 2700000},// 鞋
		{111, 2700000, 2800000},// 饰品
		{111, 2800000, 2900000},// 项链
		{111, 2900000, 3000000},// 戒指
		{113, 5000000, 5010000},// 药水
		{116, 5210000, 5220000},// 宝石

		{123, 5610000, 5620000},// 宠物蛋
	};
	const tagItemDef& itemDef = DBDataManager::getSingleton().getItemDef(id);
	if (itemDef.ModelIndex)
	{
		return itemDef.ModelIndex;
	}

	for (int i = 0; i < sizeof(range) / (sizeof(int) * 3); ++i)
	{
		if (id >= range[i][1] && id < range[i][2])
		{
			return range[i][0];
		}
	}
	return 124;
}
bool Item::canRepair()
{
	tagItemDef* pItemDef = const_cast<tagItemDef*>( &DBDataManager::getSingleton().getItemDef( getItemId() ) );
	for( int j = 0 ; j < MAX_ITEM_RESULT; j++ )
	{
		if( pItemDef->Result[j].ResultID == RESULT_NO_REPAIR )
		{
			return false;
		}
	}

	if ( pItemDef->ArmDur.MaxDur == 0 )
	{
		return false;
	}

	if ( m_ItemData.m_Arm.Dur >= pItemDef->ArmDur.MaxDur * DUR_UNIT )
	{
		return false;
	}

	return true;
}

bool Item::isArmItem()
{
	return m_ItemType == ITEM_SLOTTYPE_AMM;
}

void Item::Update( unsigned int dtick )
{
	if( m_ItemType == ITEM_SLOTTYPE_NONE || m_ItemType == ITEM_SLOTTYPE_DISABLE )
	{
		return;
	}
	if( getItemValidEndTime() > 0 )
	{
		int sec, usec;
		ServerTimer::getSingleton().getServerTime(sec,usec);
		int dt = getItemValidEndTime() - sec;
		if( dt <= 0 )
		{
			if( m_nValidLeft > 0 )
			{
				//过期
				char buf[256];
				sprintf(buf,"ItemValidNotify(\"%s\",%d)",m_strItemName.c_str(),ITEM_VALID_END);
				ActorManager::getSingleton().getScriptVM()->callString( buf );
				GameMsgPrint::getSingleton().SendUIEvent("GE_ITEM_VALID_END");
			}
 		}else if( dt < 10*60  )
 		{
			if(  m_nValidLeft >= 10*60 )
			{
  				//10分钟
  				char buf[256];
  				sprintf(buf,"ItemValidNotify(\"%s\",%d)",m_strItemName.c_str(),ITEM_VALID_10M);
  				ActorManager::getSingleton().getScriptVM()->callString( buf );
			}
 		}else if( dt < 3*60*60 ){
			if( m_nValidLeft >= 3*60*60 )
			{
				//剩3小时提示
				char buf[256];
				sprintf(buf,"ItemValidNotify(\"%s\",%d)",m_strItemName.c_str(),ITEM_VALID_3H);
				ActorManager::getSingleton().getScriptVM()->callString( buf );
			}
 		}else if( dt < 24*60*60 ){
			if( m_nValidLeft >= 24*60*60 )
			{
				//24小时提示
				char buf[256];
				sprintf(buf,"ItemValidNotify(\"%s\",%d)",m_strItemName.c_str(),ITEM_VALID_24H);
				ActorManager::getSingleton().getScriptVM()->callString( buf );
			}
  		}
		m_nValidLeft = dt;
	}
}

bool Item::isValidEnd()
{
	if( getItemValidEndTime() > 0 )
	{
		int sec, usec;
		ServerTimer::getSingleton().getServerTime(sec,usec);
		if( sec >= getItemValidEndTime() )
		{
			return true;
		}
	}
	return false;
}

int Item::getItemInstVal1()
{
	if( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		return m_ItemData.m_Item.InstVal1;
	}
	return 0;
}

int Item::getItemInstVal2()
{
	if( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		return m_ItemData.m_Item.InstVal2;
	}
	return 0;
}

int Item::getItemInstVal3()
{
	if( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		return m_ItemData.m_Item.InstVal3;
	}
	return 0;
}

int Item::getItemFlag()
{
	if( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		return m_ItemData.m_Item.Flag;
	}
	return 0;
}
char Item::getItemMapNum()
{
	if( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		return m_ItemData.m_Item.ItemMapNum;
	}
	return 0;
}
tagItemMapInfo_ToLua* Item::getItemMapInfo( int index)
{
	if( m_ItemType == ITEM_SLOTTYPE_ITEM )
	{
		return (tagItemMapInfo_ToLua*)&m_ItemData.m_Item.ItemMapInfo[index];
	}
	return 0;
}
int Item::getItemMulVal( int resultId )
{
	if( m_ItemType == ITEM_SLOTTYPE_AMM )
	{
		for( int i = 0 ; i < m_ItemData.m_Arm.BaseAttrNum ; i++ )
		{
			if( m_ItemData.m_Arm.BaseAttr[i].ResultID == resultId )
			{
				return m_ItemData.m_Arm.BaseAttr[i].Mul;
			}
		}
	}
	return 0;
}

int Item::getArmWashAddAttrNum()
{
	if( m_ItemType == ITEM_SLOTTYPE_AMM )
	{
		return m_ItemData.m_Arm.WashAttrNum;
	}
	return 0;
}

tagAddAttrInst_ToLua* Item::getArmWashAddAttrInst( int idx )
{
	if( m_ItemType == ITEM_SLOTTYPE_AMM )
	{
		return ( tagAddAttrInst_ToLua* )&m_ItemData.m_Arm.WashAddAttr[idx];
	}
	return NULL;
}

bool Item::IsAttrFengYin( int nIdx )
{
	if( m_ItemType == ITEM_SLOTTYPE_AMM )
	{
		if( nIdx < m_ItemData.m_Arm.AddAttrNum + m_ItemData.m_Arm.WashAttrNum )
		{
			return m_ItemData.m_Arm.FengyinFlag & ( (int)pow(2.0,nIdx) );
		}
	}
	return false;
}

int Item::getLostTotemState()
{
	int state = m_nLostTotemState;
	//判断是否攒满
	if ( getItemInstVal2() == getItemInstVal1() )
	{
		state = 2;
	}
	return state;
}

int Item::getStepLv()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.Step;
}

int Item::getStepUse()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.StepUse;
}

int Item::getArmSkillMax()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.ArmSkill.Max;
}

int Item::getArmSkillNum()
{
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return -1;
	}
	return m_ItemData.m_Arm.ArmSkill.Num;
}

tagArmSkillOne_ToLua* Item::getArmSkillOne( int idx )
{
	static_assert( sizeof(tagArmSkillOne_ToLua) == sizeof(tagArmSkillOne_ToLua) );
	if ( m_ItemType != ITEM_SLOTTYPE_AMM )
	{
		return NULL;
	}
	return (tagArmSkillOne_ToLua*)&m_ItemData.m_Arm.ArmSkill.One[idx];
}

//获取幻化的背饰ID
int Item::getUnRealBackID()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return m_ItemData.m_Arm.UnrealBackID;
	}

	return 0;
}
//获取幻化期限
int Item::getUnRealBackTmEnd()
{
	if( ITEM_SLOTTYPE_AMM == m_ItemType)
	{
		return m_ItemData.m_Arm.UnrealBackTmEnd;
	}

	return 0;
}