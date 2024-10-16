#pragma once
#include "OgreSingleton.h"
#include "NetMessage.h"
#include "Item.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include <map>
#include <vector>

const int MAX_STUFF_ARMPROCE = 10;

//tolua_begin
enum
{
	STUFF_NAMAL			= 2000,				// ������ͨ��Ʒ
	STUFF_LEVELGEM		= 2001,				// ��������Ҫ�õı�ʯ
	STUFF_INLAYGEM		= 2002,				// ������ǶҪ�õı�ʯ
	STUFF_EQUIP			= 2003,				// ����װ��
	STUFF_INDENTITY		= 2004,				// �������װ��
	STUFF_NEEDHOLE		= 2005,				// �������װ��
	STUFF_NEEDINLAYGEM	= 2006,				// �������Ƕ��ʯװ��
	STUFF_STARITEM		= 2007,				// ����Ʒ����������
	STUFF_FIGHTITEM		= 2008,				// ����ս������������
	STUFF_LEVELITEM		= 2009,				// ����ȼ���������
	STUFF_PICKGEM		= 2010,				// װ���ϴ�ժȡ�ı�ʯ
	STUFF_CANLEVEL		= 2011,				// ���������װ��
	STUFF_SKILLFITTING	= 2012,				// ���뼼�������������

	CLIENT_ARM_SAFE_ITEM_ID		= 5209001, 	/* װ��Ʒ���������շ� */
	CLIENT_FIGHT_SAFE_ITEM_ID	= 5219010,	/* װ��ս�����������շ� */
	CLIENT_GEM_SAFE_ITEM_ID		= 5219001 	/* ��ʯ��Ƕ���շ� */
};

#pragma pack(1) 
struct tagGemAttrData_ToLua
{
	short GemRate;                         	/*   ��ʯ���ø��� */
	unsigned int GemMoney;                        	/*   ���Ľ�Ǯ */
};

struct tagGemDef_ToLua
{
	unsigned int ItemID;                         
	unsigned char GemLevel;                        	/*   ��ʯ�ȼ� */
	unsigned char GemEffectType;                   	/*   ��ʯӰ������ Bind Macrosgroup:GEM_EFFECT_TYPE,*/
	unsigned char GemType;                         	/*   ��Ƕ�ı�ʯ���� Bind Macrosgroup:GEM_INLAY_TYPE,*/
	tagGemAttrData_ToLua GemAttr[MAX_GEM_ATTR_LEVEL];     	/*   ��ʯЧ�� */
	unsigned int LevelUpItemID;                   	/*  Ver.81 ������ʯ������� */
	unsigned int PickItemID;                      	/*  Ver.82 ժȡ��ʯ������� */
	unsigned char GemColor;                        	/*  Ver.131 ��ʯ��ɫ */
	int GemMerit;                        	/*  Ver.946 ��ʯ��ֵ|���ڽ��װ�� */
};

struct tagGemAttr_ToLua
{
	int AttrID;                         
	int Rate;                           
};
struct tagGemFengJie_ToLua
{
	int ID;                             
	int Min;                            
	int Max;                            
};
struct tagGemUse_ToLua
{
	int JinBi;                          
	int JinQuan;                        
	int ItemID;                         
};
struct tagGemNewDef_ToLua
{
	int ItemID;                         
	int Level;                          
	int IconID;                         
	tagGemAttr_ToLua Attr[MAX_GEM_ATTR];              	/*   �����б����10�� */
	int UpID;                            	/*   ����Ϊ�ĸ���ʯ */
	int UpRate;                          	/*   ������ʯ��������|����� */
	int UpMin;                           	/*   ������Ҫ������ʯ�������� */
	int GemMerit;                        	/*   ���ڽ������ */
	int Color;                           	/*   ��ɫ */
	tagGemFengJie_ToLua GemFengJie[MAX_GEM_FENGJIE];     	/*   ��ʯ�ֽⶨ�� */
	tagGemUse_ToLua ReSet;                           	/*   �������ģ����ֻ��������ѡ����һ */
	int PickID;                          	/*   ժȡ���� */
	short ArmLevel;                        	/*  Ver.1136 ��Ƕװ����С�ȼ� */
	int UseMoney;                        	/*  Ver.1144 �������� */
	int InlayUseMoney;                   	/*  Ver.2198 ��Ƕ���Ļ��� */
	int InlayUseType;                    	/*  Ver.2198 ��Ƕ���Ļ������� */
};
struct tagGemLevelStarDef_ToLua
{
	short Level;                          
	short Star;                            	/*   0-7���㿪ʼ��0��ʾû���Ǽ� */
	int PowerUp;                         	/*   ��������,0��ʾ�����Գ��� */
	int GetPower;                        	/*   ��������ֵ,0��ʾû������ֵ */
};

struct tagGemAttrStarDef_ToLua
{
	short AttrID;                         
	char Level;                           	/*  Ver.1166  */
	char Star;  
	tagGemAttr_ToLua Attr[MAX_GEM_VAL];               	/*   ��ֵ�б����10�� */
};
struct tagUseDef_ToLua
{
	int UseID;                           	/*   ȡֵΪUSE_ID */
	int UseJinbi;                        	/*   ���Ľ�� */
	int UseJinquan;                      	/*   ���Ľ�ȯ */
	int UseMoney;                        	/*   �������� */
};
struct tagArmIdxData_ToLua
{
	char ArmListType;                     	/*    Bind Macrosgroup:LIST_TYPE,*/
	unsigned short ArmGridIdx;                     
};
struct tagAddPowerStuff_ToLua
{
	short ItemNum;                        
	short ListType;                        	/*    Bind Macrosgroup:LIST_TYPE,*/
	int GridIdx;                        
	int ItemID;                         
};

struct tagGemAddPower_ToLua
{
	int OpType;                          	/*  Ver.1150 0-��ͨ����-vip����*/
	int UseType;                         	/*  Ver.1150 0-���-��ȯ*/
	int ArmHoleIdx;                      	/*   װ���ϵڼ�����ʯ,����Ǳ������Դ��ֶ�*/
	tagArmIdxData_ToLua ArmData;                        
	unsigned char StuffNum;                        	/*   ��������*/
	tagAddPowerStuff_ToLua StuffData[GEM_ADD_POWER_LIST];   	/*   �����б�*/
};


struct tagRecipeGetItem_ToLua
{
    unsigned int ItemID;                         
    unsigned short Num;                            
    unsigned short Rate;                           
};

struct tagArmFenJieDef_ToLua
{
	short Level;                           	/*   �ȼ� */
	short Important;                       	/*   Ʒ�� */
	tagRecipeGetItem_ToLua GetItem[MAX_ARMFENJIE_ITEM];     	/*   �ֽ���Ʒ�б����� */
	int UseMoney;                        	/*  Ver.1144 �������� */
	tagRecipeGetItem_ToLua AppendItem[MAX_ARMFENJIE_APPEND]; 	/*  Ver.1227 ������Ʒ�б����� */
	short SparExp;                         	/*  Ver.1231 ��ȡ��ʯ���� */
};

struct tagCSActionArmWash_ToLua
{
	int Succ;                            	/*   0 ʧ�� 1�ɹ� */
	unsigned char AddAttrNum;                     
	tagAddAttrInst_ToLua AddAttr[MAX_ARM_ADD_ATTR];      
	unsigned char WashAttrNum;                    
	tagAddAttrInst_ToLua WashAddAttr[MAX_ARM_WASH_ADD_ATTR];
};

struct tagSparSorbData_ToLua
{
	tagArmIdxData_ToLua Src;                             	/*   Դλ��  */
	unsigned char StuffNum;                        	/*   �������� */
	tagAddPowerStuff_ToLua StuffData[MAX_SPAR_SORB_STUFF];  	/*   �����б� */
};

struct tagSparDef_ToLua
{
	int ID;                              	/*   ����ID */
	char MaxLvl;                          	/*   ��ID�ľ�ʯ��ߵȼ� */
	int UpExp[SPAR_MAX_LEVEL];           	/*   ÿ���ȼ���ʯ������Ҫ���� */
	int SparExp;                         	/*   ��ID�ľ�ʯ�������پ���|�뾧ʯ�ȼ��޹� */
	int Step;                            	/*  Ver.1769 ���׾�ʯ */
};

struct tagArmLevelUPStuff_ToLua
{
	unsigned int ItemID;                         
	unsigned int Num;                            
};

struct tagArmLevelUPDef_ToLua
{
	int ItemID;                         
	tagArmLevelUPStuff_ToLua Stuff[MAX_ARM_LEVELUP_STUFF];   
	int MoneyType;                      
	int MoneyVal;                       
	int NextItemID; 
	char NextType;                        	/*  Ver.2225 ������׳�ɶ��װ���߻��Ϳͻ��˹�ͨ */
};

struct tagHighWashUseDef_ToLua
{
	int LockNum;                        
	int UseType;                        
	int UseVal;                         
};

struct tagBackLevelDef_ToLua
{
	int ItemID;                         
	int SubType;                        
	int Level;                          
	int NextItemID;                     
};

struct tagArmStepUp_ToLua
{
	tagArmIdxData_ToLua Src;                             	/*   ����װ�� */
	int ArmNum;                         
	tagArmIdxData_ToLua ArmStuffs[ARM_STEP_UP_STUFF];    	/*   ���ĵ�װ��  */
};

struct tagArmStepAttr_ToLua
{
	int Val;                             	/*   ����ֵ */
	int ItemNum;                        
	int MoneyNum;                       
	int ArmNum;                         
};

struct tagArmStepUpDef_ToLua
{
	short Lvl;                             	/*   �ȼ���|�ȼ�/10,��0��ʼ */
	unsigned char Imp;                             	/*   Ʒ�� */
	unsigned char WPos;                            	/*   ��λ */
	unsigned char MaxStep;                         	/*   �������ײ� */
	int ItemID;                          	/*   ���ĵ���ID */
	int MoneyID;                         	/*   ���Ļ���ID */
	int AttrID;                          	/*   ��������ID */
	tagArmStepAttr_ToLua StepAttr[ARM_MAX_STEP];          	/*   �ײ��������� */
	unsigned char UseArmImp;                       	/*  Ver.2164 ����װ��Ʒ�� */
};

struct tagArmGradeValDef_ToLua
{
	int Imp;                             	/*   Ʒ�� */
	int Val;                             	/*   ����ֵ */
};

struct tagResetRate_ToLua
{
	int Val;                            
	int Rate[ARM_RESET_MAX_RATE];       
};

struct tagArmReSetDef_ToLua
{
	int Lvl;                             	/*   �ȼ� */
	tagResetRate_ToLua Reset[ARM_RESET_MAX_DIST];      
	int UseMoney;                       
};

struct tagArmBaseReSet_ToLua
{
	tagArmIdxData_ToLua Src;                             	/*   ����װ�� */
	tagArmIdxData_ToLua UseItem;                         	/*   ʹ�õ��� */
	int ArmNum;                         
	tagArmIdxData_ToLua ArmStuffs[ARM_BASE_ATTR_RESET_MAX]; 	/*   ���ĵ�װ��  */
};

struct tagSkillActiveOne_ToLua
{
	int Rate;                           
	int UseItemID;                      
	int UseItemNum;                     
	int UseMoney;                       
};

struct tagArmSkillNumDef_ToLua
{
	short Lvl;                            
	unsigned char Imp;                            
	unsigned char WPos;                           
	tagSkillActiveOne_ToLua Active[MAX_ARM_SKILL_NUM];      
};

struct tagArmSkillAttrDef_ToLua
{
	int ID;                             
	char DefDesc[ARM_SKILL_MAX_DESC];    
	unsigned char Lvl;                            
	int SkillID;                        
	int EffectID;                       
	int BaseVal;                        
	int LvlMdf[MAX_LVL_GRADE];           	/*   �ȼ������� */
	tagAddAttrWearPosLimit_ToLua WposMdf;   /*   ��λ���� */
	int ImpMdf[MAX_IMP_GRADE];           	/*   Ʒ������ */
	short RandMdf[MAX_IMP_GRADE];          	/*   ������� */
	unsigned char MinLvlLimit;                    
	unsigned char MaxLvlLimit;                    
	int CareerLimit;                    
	int WposLimit;                      
	int ImpLimit;    
	char Type;                            	/*  Ver.2293  */
};
struct tagArmSkillUse_ToLua
{
	int ItemID;                         
	unsigned char ItemNum;                        
	unsigned char MoneyType;                      
	int MoneyNum;                       
};

struct tagArmSkillRecastDef_ToLua
{
	int Lvl;                            
	tagArmSkillUse_ToLua Active[MAX_ARM_SKILL_NUM];      
	int ResetItemID;                    
	unsigned char ResetItemNum;                   
	unsigned char ResetMoneyType;                 
	int ResetMoneyNum;                  
};
#pragma pack()

enum
{
	CLIENT_GEM_FAIL		= 0,
	CLIENT_GEM_SUCCESS	= 1,
	CLIENT_GEM_PERFECT	= 2,
};

enum
{
	CLIENT_GEM_EFFECT_STAR		= GEM_EFFECT_STAR,
	CLIENT_GEM_EFFECT_FIGHT		= GEM_EFFECT_FIGHT,
	CLIENT_GEM_EFFECT_LEVEL		= GEM_EFFECT_LEVEL,
	CLIENT_GEM_EFFECT_INLAY		= GEM_EFFECT_INLAY,
};

enum
{
	CLIENT_GEM_HOLE1_RANDOM = GEM_HOLE1_RANDOM, 	/* ��һ���׵ĸ��� */
	CLIENT_GEM_HOLE2_RANDOM = GEM_HOLE2_RANDOM, 	/* �ڶ����׵ĸ��� */
	CLIENT_GEM_HOLE3_RANDOM = GEM_HOLE3_RANDOM, 	/* �������׵ĸ��� */
	CLIENT_GEM_HOLE4_RANDOM = GEM_HOLE4_RANDOM, 	/* ���ĸ��׵ĸ��� */
};

const tagGemDef_ToLua* getGemDef( int nItemID );
const tagArmLevelUPDef_ToLua* getArmLevelUPDef( int nID );

int getArmMoveMoney( int nType );
int getArmResetMoney();

#define ARM_STAR_ITEM_TOLUA ARM_STAR_ITEM 	/* ���ᱦ�� װ�����Ǻ�ת���Ǽ���Ҫ�ı�ʯ���� */
#define ARM_UNBIND_ITEM_LOW_TOLUA ARM_UNBIND_ITEM_LOW         	/* �ͼ����������Խ��60������װ��  */
#define ARM_UNBIND_ITEM_MID_TOLUA ARM_UNBIND_ITEM_MID		 	/* �м����������Խ��61~90��װ�� */
#define ARM_UNBIND_ITEM_HIGH_TOLUA ARM_UNBIND_ITEM_HIGH			/* �߼����������Խ��91������װ�� */
#define ARM_GEM_PICK_ITEM_TOLUA	ARM_GEM_PICK_ITEM				//��ʯժȡ�� ���鱦ʯ�����
#define ARM_BASE_ATTR_RESET_MAX_TOLUA ARM_BASE_ATTR_RESET_MAX   /* ����������������װ���������� */
#define ARM_RESET_MAX_DIST_TOLUA ARM_RESET_MAX_DIST				/* �������õĵ��� */
#define ITEM_STAT_UP_FIRST_PERFECT_TOLUA ITEM_STAT_UP_FIRST_PERFECT /* ����ǿ���Զ�����ǿ������id */

enum tagCONSUME_TYPE_TOLUA 
{
	CONSUME_TYPE_BINDGODCOIN_TOLUA = CONSUME_TYPE_BINDGODCOIN, 	/* ������� */
	CONSUME_TYPE_GODCOIN_TOLUA = CONSUME_TYPE_GODCOIN, 	/* ��ȯ���� */
};

enum tagVIPRECASTCONSUME_TOLUA 
{
	LEVEL40_TOLUA = LEVEL40, 	/* С�ڵ���40��װ�����Ľ��OR��ȯ */
	LEVEL60_TOLUA = LEVEL60, 	/* С�ڵ���60��װ�����Ľ��OR��ȯ */
	LEVEL80_TOLUA = LEVEL80, 	/* С�ڵ���80��װ�����Ľ��OR��ȯ */
	LEVEL100_TOLUA = LEVEL100, 	/* С�ڵ���100��װ�����Ľ��OR��ȯ */
	LEVEL120_TOLUA = LEVEL120, 	/* С�ڵ���120��װ�����Ľ��OR��ȯ */
};
enum 
{
	ONE_GEM_HECHENG_GODCOIN_TOLUA	= ONE_GEM_HECHENG_GODCOIN ,		/* һ����ʯ�ϳɷ���Ҫ���ĵĽ��ֵ */
	TWO_GEM_HECHENG_GODCOIN_TOLUA	= TWO_GEM_HECHENG_GODCOIN  , 	/* ������ʯ�ϳɷ���Ҫ���ĵĽ��ֵ */
	THREE_GEM_HECHENG_GODCOIN_TOLUA = THREE_GEM_HECHENG_GODCOIN ,	/* ������ʯ�ϳɷ���Ҫ���ĵĽ��ֵ */
	FOUR_GEM_HECHENG_GODCOIN_TOLUA  = FOUR_GEM_HECHENG_GODCOIN , 	/* �ļ���ʯ�ϳɷ���Ҫ���ĵĽ��ֵ */
	FIVE_GEM_HECHENG_GODCOIN_TOLUA  = FIVE_GEM_HECHENG_GODCOIN , 	/* �弶��ʯ�ϳɷ���Ҫ���ĵĽ��ֵ */
	SIX_GEM_HECHENG_GODCOIN_TOLUA   = SIX_GEM_HECHENG_GODCOIN , 	/* ������ʯ�ϳɷ���Ҫ���ĵĽ��ֵ */
	SEVEN_GEM_HECHENG_GODCOIN_TOLUA	= SEVEN_GEM_HECHENG_GODCOIN, 	/* �߼���ʯ�ϳɷ���Ҫ���ĵĽ��ֵ */
	EIGHT_GEM_HECHENG_GODCOIN_TOLUA		= EIGHT_GEM_HECHENG_GODCOIN,/* �˼���ʯ�ϳɷ���Ҫ���ĵĽ��ֵ */
	FIVE_GEM_HECHENG_GODCOIN_GJ_TOLUA	= FIVE_GEM_HECHENG_GODCOIN_GJ,	/* �弶��ʯ�ϳɷ���Ҫ���ĵĽ��ֵ_�߼� */
	SIX_GEM_HECHENG_GODCOIN_GJ_TOLUA	= SIX_GEM_HECHENG_GODCOIN_GJ,	/* ������ʯ�ϳɷ���Ҫ���ĵĽ��ֵ_�߼� */
	SEVEN_GEM_HECHENG_GODCOIN_GJ_TOLUA	= SEVEN_GEM_HECHENG_GODCOIN_GJ,	/* �߼���ʯ�ϳɷ���Ҫ���ĵĽ��ֵ_�߼� */
	EIGHT_GEM_HECHENG_GODCOIN_GJ_TOLUA	= EIGHT_GEM_HECHENG_GODCOIN_GJ,	/* �˼���ʯ�ϳɷ���Ҫ���ĵĽ��ֵ_�߼� */
};
 
//tolua_end

class GameArmProce//tolua_export
	: public NetMsgHandler, public Ogre::Singleton<GameArmProce>
{//tolua_export
public:
	GameArmProce(void);
	~GameArmProce(void);

public:

	// c->s

	//tolua_begin
	// װ��Ʒ������
	void armAddStar( int nListType, int nGridIdx, int nSelectStar, int nBind, int nMust );
	//����ǿ��
	void armAddSave( int nArmListType, int nArmGridIdx, int nSelectStarIdx, int nRateIdx );
	// װ���ȼ�����
	void armAddLevel( int nListType, int nGridIdx );
	// װ��ս��������
	void armAddFight( int nListType, int nGridIdx, Item& safeItem );
	// װ������
	void armIdentifiability( int nListType, int nGridIdx );
	//װ�������������ǣ�
	void armIdentifyStart( int nSrcType,int nSrcGrid,int nDestType,int nDestGrid );
	// ��ʯ����
	void gemLevelUp( Item& stuffItem,int nType );
	// ��ʯ����
	void openGemHole( int nHoleIdx, int nListType );
	void requireFengYinPlayerInfo( int nListType, int nGridIdx );
	void requireUnFengYin( int nListType, int nGridIdx );
	//��ʯ����
	void resetGem( int nHoleIdx, int nUseMoneyType, int nListType, int nGirdId );
	//��ʯ��������
	void saveResetGem( int nListType, int nGridIdx );
	//��ʯ����
	void chargeGem( tagGemAddPower_ToLua Info );
	tagGemAddPower_ToLua getChargeGemAddPowerInfo();
	int getChargePerPowerUseMoney();
	
	// ��ʯ��Ƕ nSafeFlag ���ո���־ 0 ��ʹ��1ʹ��
	void gemInlay( int nHoleIdx, Item& gemItem, int nListType/*, Item& safeItem*/ );
	// ��ʯժȡ
	void gemPick( int nHoleIdx, int nList,int nGrid,int nStuffID,int nStuffList,int nStuffGrid );
	// �Ƿ��ڴ�װ���б��е���Ʒ
	bool isInStuffItem( int nListType, int nGrid );
	//װ����
	void bindArm( int nListType, int nGridIdx );

	void unBindArm( int nListType,int nGridIdx,int nStuffID,int nStuffList,int nStuffGridIdx );

	void moveArmStar( int nSrcList,int nSrcGrid,int nDestList,int nDestGrid,int nBind );

	//��ͨ ����
	void normalRecastArm( int nItemId, int nListType, int nGridIndex, int nBind, int nMust );

	//�߼� ����
	void advanceRecastArm( int nType, int nListType, int nGridIndex );

	//���� ����
	void batchRecastArm( int nType, int nListType, int nGridIndex );

	void saveRecastArm( int nSelectIdx, int nListType, int nGridIdex );

	//ϴ��װ��
	void requestArmWash( int stuffItemId, int armList, int armGrid, int nBind, int nLockFlag, int nSafeFlag, int nMust );
	void requestArmWashExtent( int armList, int armGrid, int idx );
	//����ϴ��
	void saveWashArm( int armList, int armGrid );
	//��ʯ��Ƕ
	bool requestSparInlay( int nSrcList, int nSrcGrid, int nDestList, int nDestGrid );
	//��ʯ����
	tagSparSorbData_ToLua* getSparSorbData();
	void requestSparSorb();

	void requestArmAttrMove( int nSrcList, int nSrcGrid, int nDstList, int nDstGrid, int nUseList, int nUseGrid, int nType );

	void requestArmAttrReset( int nList, int nGrid, int nItemID, int nBind, int nIdx );

	// ���ν���
	const	tagBackLevelDef_ToLua* getBackLevelDef( int id );
	void	requestBackLevelUp( int nBackList, int nBackGrid,tdr_ulonglong backWId, int nItemList, int nItemGrid,tdr_ulonglong itemWId );
	// ���θ���
	int		getBackLevelID( int nType, int nLevel );
	void	requestBackChange( int nBackList, int nBackGrid, tdr_ulonglong backWId, int nItemList, int nItemGrid,tdr_ulonglong itemWId );
	//tolua_end

public:
	// s->c
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
	
	//tolua_begin
	// ���Ŀ����Ʒ
	int addSrcItem( Item& item, int nType = STUFF_EQUIP );
	// ���װ����������Ҫ���ϻ�����Ҫ����(ע��ű�ʯ��nTypeΪSTUFF_GEM)
	int addStuff( Item& item, int nIndex, int nType = STUFF_NAMAL );
	// ɾ��װ����������Ҫ����
	void delStuff( int nIndex );
	// ɾ��������װ��
	void delSrcItem();
	// �����λ(ע��:ÿ�δ�ҳ�����)
	void clear();
	
	// ��ȡĿ����Ʒ
	Item& getSrcItem();
	// �����Ƿ�Ϊ��
	bool isStuffItemEmpty();
	// ��ȡ��ӵĲ���
	Item& getStuffItem( int nIndex );
	// ��ȡ���豦ʯ������ID
	int getGemUpdateId();
	// ��ȡ��ʯժȡ������ƷID
	int getGemPickUpUsedItem( int nGemId );
	// ��ȡ��ʯ����������
	int getGemSuccessRate( int index, int nGemId );
	// ��ȡ��ʯ����������
	unsigned int getGemCostMoney( int index, int nGemId );
	// ��ȡ��ʯ������
	int getGemType( int nGemId );
	// ��ȡ��ʯ����ɫ
	int getGemColor( int nGemId );
	
	// ��ȡ��ʯ����
	const tagGemDef_ToLua* getGemDef( int nItemID );
	const tagGemNewDef_ToLua* getGemNewDef( int nItemID );
	const tagGemAttrStarDef_ToLua* getGemAttrStarDef( int nAttrID, int nLevel, int nStar );
	const tagGemLevelStarDef_ToLua* getGemLevelStarDef( int nLevel, int nStar );

	const tagUseDef_ToLua* getUseDef( int nID );
	const tagSparDef_ToLua* getSparDef( int id );

	// �Ƿ��Ǳ�ʯ
	bool isGem( int nItemID );

	const tagArmFenJieDef_ToLua* getArmFenJieDef( int nLevel, int nImportant );
	void requestContainerResolvePre( tdr_ulonglong WId, char ListType, unsigned short GirdId, int nType );
	//װ������
	void requestSparEpurate( tdr_ulonglong armWId, char armListType, unsigned short armGirdId, 
		tdr_ulonglong itemWId, char itemListType, unsigned short itemGirdId );
	void requestBreakContainerResolvePre();

	int getStarNeedStuffNum( int nStar );
	int getStarNeedMoney( int nStar );

	int getBindNeedMoney( int nStar );
	int getMoveStarNeedStuffNum( int nStar );
	int getUnBindNeedGold( int nStar );
	int getBackStar( int nStar );
	const tagGemDef_ToLua* getNextHeChengGemDef( const tagGemDef_ToLua* pCurGemDef );

	void stopContainerResolve();
	bool isResolvePre();

	int getArmRecastArea( int resultID, int mul );

	// ��ȡ������ʯ��λ�ĵȼ�
	int getSparOpenLv( int nIdx );
	// ��ȡ����ÿ��װ����������
	int getSparSorbCostMoney();
	// �߼�ϴ��
	const tagHighWashUseDef_ToLua* getHighWashDef( int nNum );
	// װ������
	void requestArmStepUp( tagArmStepUp_ToLua Data );
	tagArmStepUp_ToLua getArmStepUpData();
	const tagArmStepUpDef_ToLua* getArmStepUpDef( int nLv, int nImp, int nPos );
	// ������������
	void requestArmBaseReset( tagArmBaseReSet_ToLua Data );
	void requestArmBaseResetSave( int nList, int nGrid );
	tagArmBaseReSet_ToLua getArmBaseResetData();
	const tagArmGradeValDef_ToLua* getArmGradeValDef( int nImp );
	const tagArmReSetDef_ToLua* getArmResetDef( int nLv );
	tagAddAttrInst_ToLua& getArmResetResult( int idx );
	// װ������
	const tagArmSkillNumDef_ToLua* getArmSkillNumDef( int nLv, int nImp, int nPos );
	const tagArmSkillAttrDef_ToLua* getArmSkillAttrDef( int nId );
	void requestArmSkillActive( int idx, int nArmList, int nArmGird, int nUseList, int nUseGrid );
	// ��װ����װ
	void requestArmUpOrange( int nUseList, int nUseGrid, int nArmList, int nArmGird, int nNum, int nOption );
	//���λû�
	void requestUnRealBack( int nSrcList, int nSrcGrid, int nDesList, int nDesGrid );
	//װ�����ܼ���
	void requestArmSkillActive( int srcGridIdx, int srcListType );
	//װ��������
	void requestArmSkillLock( int nIdx, int srcGridIdx, int srcListType );
	//װ����������
	void requestArmSkillReset( int stuffGridIdx, int stuffListType, int srcGridIdx, int srcListType );
	//װ����������
	const tagArmSkillRecastDef_ToLua* getArmSkillRecastDef( int nLv );
	//tolua_end

private:
	// ��ȡ���뱦ʯ�����ͺ͵ȼ�
	void getGemTypeLv( int* nType, int* nLv );

private:
	Item	m_Stuff[MAX_STUFF_ARMPROCE];	// װ������ʹ�ò���
	Item	m_SrcItem;						// Ҫ��������Ʒ��װ��
	std::vector<tagGemDef*>		m_gemDef;
	std::vector<tagGemNewDef*>	m_gemNewDef;
	std::map<int, std::vector<tagGemDef*> >	m_gemTypePartion;
	bool	m_bCanOpenHole;
	tagGemAddPower_ToLua		m_GemAddPowerInfo;
	SPARSORBDATA				m_SparSorbData;

	// ��ʯ���ӿ����ȼ�
	std::vector<int>			m_vSparAddLv;
	tagArmStepUp_ToLua			m_ArmStepUpData;
	tagArmBaseReSet_ToLua		m_ArmBaseResetData;
	tagAddAttrInst_ToLua		m_ArmBaseResetResult[MAX_ARM_ADD_ATTR];

};//tolua_export
