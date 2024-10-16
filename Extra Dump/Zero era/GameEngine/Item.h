#pragma once
#include "ItemDefine.h"
#include <string>

//tolua_begin
enum EITEM_TYPE
{
	ITEM_SLOTTYPE_NONE,		// ��
	ITEM_SLOTTYPE_DISABLE,	// ������
	ITEM_SLOTTYPE_ITEM,		// ��Ʒ
	ITEM_SLOTTYPE_AMM,		// װ��
};

enum
{
	ITEM_TYPE_TOOLS		= 5,	// ����
};

enum
{
	UNUSED_AMM						= -1,		// ����Ҫʹ���ӵ�
	AMM_ID							= 512,		// ��ҩ 	
	AMM_ID_TEM						= 513,		// ��ʱ��ҩ
	
	ITEM_PILL_HP_ID					= 5000,		// ҩˮHP������(һ����)
	ITEM_PILL_MP_ID					= 5001,		// ҩˮMP������(һ����)
	ITEM_PILL_HPLONG_ID				= 5002,		// ҩˮHP������(������)
	ITEM_PILL_MPLONG_ID				= 5003,		// ҩˮMP������(������)
	ITEM_PILL_BUFF_ID				= 5004,		// ҩˮ״̬��
	ITEM_PILL_BUFFLONG_ID			= 5005,		// ҩˮBUFF��
	ITEM_PILL_MORPH_ID				= 5006,		// ҩˮ������
	ITEM_PILL_PET_ID				= 5007,		// ҩˮ������
	ITEM_PILL_HPMP_ID				= 5008,		// ҩˮ������(ȫ�ָ�)
	ITEM_PILL_HPMPLONG_ID			= 5009,		// ҩˮ������(������ȫ�ָ�)
	
	ITEM_FOOD_HPLONG_ID				= 5010,		// ʳ��HP������(������)
	ITEM_FOOD_MPLONG_ID				= 5011,		// ʳ��MP������(������)
	ITEM_FOOD_BUFFLONG_ID			= 5012,		// ʳ��BUFF��
	ITEM_FOOD_BASEATTR_ID			= 5013,		// ȫ�ظ�+һ������(��)
	ITEM_FOOD_RACEATTR_ID			= 5014,		// ȫ�ظ�+ְҵ����(��)

	ITEM_MAP_POSITION_ID			= 5030,		// ��λ��

	ITEM_SCROLL_SKILL_ID			= 5040,		// ���Ἴ��ѧϰ��
	ITEM_SCROLL_ID					= 5041,		// ���������Ϊ��
	ITEM_SCROLL_QIYUE_ID			= 5042,		// ������ ��Լ
	ITEM_SCROLL_DESIGNATION_ID		= 5043,		// ������ �ƺ�
	
	ITEM_NO_DIE_PUNISH_ID			= 5050,		//��������BUFF�ĵ���
	ITEM_CLAN_ITEM					= 5051,		// �������
	ITEM_HELP_PRESENT_ID			= 5052,		// ��������༰����
	ITEM_HELP_BOX_ID				= 5053,		// ������ٱ���
	ITEM_HELP_RAND_ID				= 5054,		// �������ʯ�ɽ�

	ITEM_TRUST_ID					= 5060,		// �����ί����Ʒ
	ITEM_PROMOTION_ID				= 5062,		// ����ʹ�õ��� �ر�ͼ

	ITEM_GEM_STAR_ID				= 5200,		// Ʒ��ǿ����ʯ����Ƭ
	ITEM_GEM_STRENGTH_ID			= 5209,		// ǿ�����շ�����Ƭ

	ITEM_INLAYGEM_RED_ID			= 5220,		// ǿ�����շ�����Ƭ
	ITEM_INLAYGEM_GREEN_ID			= 5221,		// ǿ�����շ�����Ƭ
	ITEM_INLAYGEM_BLUE_ID			= 5222,		// ǿ�����շ�����Ƭ
	ITEM_INLAYGEM_COLORFUL_ID		= 5223,		// ǿ�����շ�����Ƭ
	ITEM_INLAYGEM_OTHER_ID			= 5229,		// ǿ�����շ�����Ƭ

	ITEM_PET_EXP_ID					= 5230,		// ������߾������
	ITEM_PETXUNFU_ID				= 5233,		// �����ѱ������
	ITEM_PETLEVELUP_ID				= 5235,		// ����ȸ������ �о���ʯ
	ITEM_FIGHTTING_BOOK1_ID			= 5240,		// ���������������
	ITEM_FIGHTTING_BOOK2_ID			= 5241,		// ���������������
	ITEM_FIGHTTING_BOOK3_ID			= 5242,		// ���������������

	ITEM_LVUP_ID					= 5250,		// �ɳ���
	ITEM_HELP_LVUP_ID				= 5251,		// ������
	ITEM_HELP_MAIL_ID				= 5255,		// �Զ�ʰȡ���ʼ�����

	ITEM_TASK_MAINLINE_ID			= 5300,		// �������(�����������)
	ITEM_TASK_MAINLINE_ID_TASK	= 5301,		// �������(�����������)
			
	ITEM_PET_ID						= 5600,		// ����
	ITEM_PETSHIEGG_ID				= 5610,		// ������
	ITEM_PETSHIEGG_ID2				= 5611,		// ���ﵰ
	ITEM_PETXIANEGG_ID				= 5620,		// ���ӵ�
	ITEM_TRUMP_ID					= 5622,		// ������������

	ITEM_LEARN_SECONDARY_ID1		= 5254,		// ѧϰ�����
	ITEM_LEARN_SECONDARY_ID2		= 5255,		// ѧϰ�����
	ITEM_LEARN_SECONDARY_ID3		= 5256,		// ѧϰ�����
};

enum// װ������
{
	EQUIP_NORMAL		= 0,		// ��ͨ
	EQUIP_ACTIVITY		= 13,		// �
	EQUIP_PEI			= 14,		// 
	EQUIP_MARKET		= 15 ,
	EQUIP_SUIT			= 16,
	EQUIP_NORMALSUIT	= 17,
};

enum
{
	ITEM_IMPORTANT_NORMAL = 0,					// ��ͨƷ��
	ITEM_IMPORTANT_EXCELLENCE,					// ����Ʒ��
	ITEM_IMPORTANT_BEST,						// ����Ʒ��
	ITEM_IMPORTANT_NONSUCH,						// ��ƷƷ��
	ITEM_IMPORTANT_EPIC,						// ��װ  ��ͨ
	ITEM_IMPORTANT_EPIC_A,						// ��װ  ʷʫ��װ
	ITEM_IMPORTANT_EPIC_S,						// ��װ  Ӣ����װ
	ITEM_IMPORTANT_LEGEND,						// ����Ʒ��
	ITEM_IMPORTANT_END,
};

enum
{
	ITEM_IDTYPE_NONE		= 0,     
	ITEM_IDTYPE_ARM			= 1,	// ����
	ITEM_IDTYPE_EQUIP		= 2,	// ����
	ITEM_IDTYPE_BARDAIN		= 3,	// ����
	ITEM_IDTYPE_ITEM		= 5,	// ����
};

enum 
{
	ITEM_EXTENDED_TYPE_NONE = 0, 	/* ���Ǹ�ħ���� */
	ITEM_EXTENDED_TYPE_RESULT = 1, 	/* װ��Ч��(RESULT)ָ������ЩЧ�� */
	ITEM_EXTENDED_TYPE_ATTR = 2, 	/* װ���ؼ��� */
};

enum
{
	ITEM_VALID_24H,
	ITEM_VALID_3H,
	ITEM_VALID_10M,
	ITEM_VALID_END,
};

/*   ��Ҫ�̶�|װ��ʹ�ô��ֶ���Ϊ��װ��װ�ȵ��ж�:1�� 2�� 3�� 4�� */
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
	ITEM_FIGHT_NO_USE_TOLUA = 1, 	/* ս��״̬������ʹ����Ʒ */
	ITEM_RIDE_MACHINE_USE_TOLUA = 2, 	/* ��˹����е����ʹ�õĵ��� */
	ITEM_PRE_USE_TOLUA			= 4, 	/* ����״̬����ʹ�õĵ��� */
	ITEM_SHOW_LIGHT_TOLUA		= 8, 	/* ��Ʒ������ */
	ITEM_NO_LEVELLIMIT_TOLUA	= 16, 	/* ��Ʒ�������Եȼ����� */
	ITEM_IMP_LOGTRACE_TOLUA		= 32, 	/* ��Ҫ��Ʒ��¼��ˮ��־ */
	ITEM_COLOR_GREEN_TOLUA		= 64, 	/* ������ɫ�߿� */
	ITEM_COLOR_BLUE_TOLUA		= 128, 	/* ������ɫ�߿� */
	ITEM_COLOR_PURPLE_TOLUA		= 256, 	/* ������ɫ�߿� */
	ITEM_COLOR_PINK_TOLUA		= 512, 	/* ���߷�ɫ�߿� */
	ITEM_MO_SHI_NO_USE_ToLUA	= 1024, 	/* ����Ʒ�޷���ĩ����ҫ������ʹ�� */
	ITEM_DROP_TIME_ToLUA		= 2048, 	/* ����Ʒ���䱣��ʱ���������ʱ�� */
	ITEM_S_ToLUA				= 4096, 	/* �ͻ����� */
	ITEM_A_ToLUA				= 8192, 
	ITEM_B_ToLUA				= 16384,
	ITEM_CAN_STEP_UP_BACK_TOLUA = 65536, 	/* �ɽ��ױ���,������Ϊ���λû���Ŀ��� */
	ITEM_NO_SHOW_PREFIX_TOLUA = 131072, 	/* ��ʾ����ʾǰ׺ */
	ITEM_HAVE_FLOAT_ACT_TOLUA = 262144, 	/* ��Ư������ */
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
int						getArmExtraDur( tagItemDef_ToLua* pItemDef ); // ���������;ö�

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
	int					getItemId();		// ��ȡ��Ʒ��̬ID
	tdr_ulonglong		getItemWId();		// ��ȡ��Ʒȫ��ΨһID
	bool				canUsed();			// ��Ʒ�Ƿ��ʹ��
	bool				canExchange();		// ��Ʒ�Ƿ�ɽ���		
	
	int  getGridIdx();						// ��ȡ������
	int	 getListType();						// ������������
	int	 getItemType();						// ��ȡ��Ʒ��������(���߻���װ��)
	int  getItemSubType();					// ��ȡ����������
	int  getNum();							// ��ȡ��������
	void setNum( int nNUm );				// ���õ�������

	int  getItemDur();						// ��ǰ�־ö�
	int  getItemMaxDur();					// ���־ö�
	
	int  getRepairPrice();					// ��ȡά�޼۸�
	int	 getSellPrice();					// ��ȡ���ۼ۸�
	int	 getStart();						// ��ȡ�����Ǽ�
	int  getArmPefectStarNum();				//��ȡװ�������Ǽ���Ŀ
	int	 getGemStart();						//��ȡ��ʯ�Ǽ�
	int  getGemPower();						//��ȡ��ʯ����ֵ
	unsigned int	getStallSellPrice();		// ��ȡ��Ʒ��̯λ�ϵĳ��ۼ۸�
	tdr_ulonglong	getStallGridWID();	// ��ȡ��̯��λȫ��ΨһID
	int  getFight();						// ���ս�����ȼ�
	int  getHoleNum();						// ��ȡ��ʯ����
	int  getHoleGemResultIndex( int nIndex );// ��ȡ��ʯ��Ч��
	int  getHoleGemAttrIdx( int nIndex );	//��ȡ��ʯ���Ա�����
	int  getHoleGemId( int nIndex );		// ��ȡ��λ�Ͽ���Ƕ��ʯId
	int	 getHoleGemColor( int nIndex );		// ��ȡ��λ�Ͽ���Ƕ��ʯ��ɫ
	int	 getHoleGemStar( int nIndex );		//��ȡ��λ�ϱ�ʯ���Ǽ�
	int  getHoleGemPower( int nIndex );		//��ȡ��λ�ϱ�ʯ������
	int	 getHoleGemMul( int nIndex );		// ��ȡ��λ��ʯ��ֵ����ֵ
	int  getItemSurplus();					// ��ȡ��Ʒʣ��ʹ�ô���
	int  getStrengthenCount();				//��ȡǿ������

	int  needAmm();							// �Ƿ���Ҫ��ҩ
	bool isAmm();							// �Ƿ��ǵ�ҩ
	bool isExpendable();					// �Ƿ�������Ʒ
	bool isPetUsed();						// �Ƿ��ǶԳ���ʹ��
	bool isRing();							// �Ƿ��ǽ�ָ
    bool isAmuleft();						// �Ƿ����
	bool isFashionSuit();					// �Ƿ���ʱװ
	bool isItemBind();						// װ���Ƿ��
	bool isArmFengYin();					// װ���Ƿ񱻷�ӡ
	bool isArmIdentity();					// װ���Ƿ񱻼���
	unsigned int getUnFengYinArmMoney();	// ���װ������Ҫ��Ǯ

	void reSetDur();						// ��װ���;ö���Ϊ��

	void updateItemData();					// ˢ����Ʒ����
	float	getArmStarAddValue( int nType );	// ��ȡ�Ǽ�װ�����ӵ�ֵ

	int getArmAddAttrNum();					// ��ȡװ���ؼ�������
	tagAddAttrInst_ToLua* getArmAddAttr( int nIndex );	// ��ȡװ���ؼ���

	void releaseItem();	

	const char* getDisplayName(){ return m_strItemName.c_str(); };

	// ��ȡ���߿�ʹ�õĵ�ͼID
	int getItemUsedMapId();
	// ��ȡ������Ч����ʱ��
	int getItemValidEndTime();
	//�Ƿ�ħ��Ʒ
	bool isExtend();

	bool isFairyKeyIn();
	//��ȡ��ħ�䷽id
	int getExtendedFromDefID();
	//��ȡ��ħ����
	tagExtendedResult_ToLua* getExtendedResult();
	// �Ƿ��������Ʒ
	bool isIdentity();
	int	 getIdentityLevel();
	bool isBackLevelUp();						//���ν�����Ʒ
	bool isBackChange();						//���θ�����Ʒ
	//�Ƿ������������Ʒ
	bool canRepair();
	bool isArmItem();
	//��� ��Ʒ�Ŀ�ӡ����
	const tagAddAttrInst_ToLua* getAddAttrInst();
	int getKeyEndTime();
	bool isValidEnd();
	int getValidLeftTime(){return m_nValidLeft;}
	void setValidLeftTime(int time){m_nValidLeft = time;}

	//��ȡװ���Ǽ�ǿ����������
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

	int getUnRealBackID();												//��ȡ�û��ı���ID
	int getUnRealBackTmEnd();										//��ȡ�û�����
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

	int			m_nValidLeft;		//��Ч��ʣ��ʱ��
	
	int			m_nLostTotemState;	//ʧ��ͼ�ڣ�0-��ͨ��ͼ��505233����1-���ͼ��505234����2-������ͼ��505235��
};//tolua_export
