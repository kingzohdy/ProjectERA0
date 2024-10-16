#pragma once
#include "GameEngineTypes.h"
#include "NetMessage.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include "GameDBData.h"
#include "GameActor_Pet.h"

#pragma pack(1)
//tolua_begin 

#define FAIRY_MAX_STAR_TOLUA FAIRY_MAX_STAR
#define DEC_EVOLVE_TIME_ITEMID_TOLUA DEC_EVOLVE_TIME_ITEMID
#define ZEROREELID_TOLUA ZEROREELID
struct tagTrumpId 
{
	int nHeight;
	int nLow;
};

struct tagRoleFairy_ToLua
{
	unsigned short FairyID;                        
	unsigned short FairyLevel;                     
	tagTrumpId WID;
	char Stat;                           
	unsigned int EvolveLeftTM;                   
	char GridIdx;                         	/*  Ver.136  */
	unsigned int Flag;                            	/*  Ver.150  */
	int Exp;                             	/*  Ver.191  */
	int StarLvl;                         	/*  Ver.539 精灵星级 */
	int TraitID;                         	/*  Ver.539 精灵性格ID */
	int IsKeyin;                         	/*  Ver.539 今天是否产出过卷轴 1是 0否 */
};

struct tagFairySkill_ToLua
{
	unsigned char SkillType;                       	/*   0--技能  1--素质 */
	unsigned int SkillID;                        
	unsigned char SkillLevel;       
};

struct tagEvolveStuff_ToLua
{
	unsigned int ItemID;                         
	unsigned char ItemNum;                        
};

struct tagFairyDef_ToLua
{
	unsigned short FairyID;                         	/*   精灵ID */
	unsigned short Level;                           	/*   精灵等级 */
	char Name[RES_NAME_LEN];             
	unsigned int ModeID;                          	/*   模型ID */
	unsigned int BuffID;                          	/*   BuffID */
	char FairyDesc[SHORT_DESC_LEN];       	/*   精灵描述 */
	tagFairySkill_ToLua FairySkillList[FAIRY_MAX_SKILL]; 	/*   技能列表 */
	unsigned short EvolveTime;                      	/*   炼化时间 单位分 */
	unsigned char EvolveLevelLimit;                	/*   角色炼化等级限制 */
	unsigned int PhyAtk;                          	/*  Ver.133 物理攻击力 */
	unsigned int MgcAtk;                          	/*  Ver.133 魔法攻击力 */
	unsigned int BrkAtk;                          	/*  Ver.133 破坏力 */
	short BaseXp;                          	/*  Ver.183 每10秒增加的xp值 */
	unsigned int UseMoney;                        	/*  Ver.183 炼化消耗多少金币 */
	unsigned int UseExp;                          	/*  Ver.183 炼化消耗多少经验 */
	unsigned char GetXpRate;                       	/*  Ver.223 角色杀死怪物 获取xp的概率  |百分率 */
	int XpUpperLimit;                    	/*  Ver.240 xp上限 */
	int FireAtk;                         	/*  Ver.426 火属性攻击力 */
	int ThunderAtk;                      	/*  Ver.426 雷属性攻击力 */
	int LightAtk;                        	/*  Ver.426 光属性攻击力 */
	int NightAtk;                        	/*  Ver.426 暗属性攻击力 */
	int WaterAtk;                        	/*  Ver.426 水属性攻击力 */
	unsigned char MaxStar;                         	/*  Ver.539 最大能升到的星级 */
	unsigned char TraitID;                         	/*  Ver.539 性格ID */
	unsigned char PNum;                            	/*  Ver.539 能产出道具总数 */
	int PItems[FAIRY_MAX_PRODUCE];       	/*  Ver.539 精灵产出道具列表 */
	int ZeroReelID;                      	/*  Ver.540 此字段改为:精灵产出(刻印)时需要道具的数量,ID写死 */
	int KeyinUseXp;                      	/*  Ver.805 精灵刻印消耗多少Xp(值) */
};

struct tagFairyStuffRate_ToLua
{
	unsigned char Star;                            	/*   对应星级 */
	unsigned short Rate;                            	/*   成功概率 |万分率 */
};

struct tagFairyStarStuffDef_ToLua
{
	int ItemID;                         
	unsigned char MinStar;                        
	unsigned char MaxStar;                        
	tagFairyStuffRate_ToLua StuffRate[FAIRY_MAX_STAR];      
};

struct tagFairyStarStuff_ToLua
{
	int MainStuffID;                     	/*   主材料物品ID */
    int MainStuffNum;                    	/*   最少需要这么多主材料 */
    int Money;                           	/*   金钱 */
};

struct tagFairyStarDef_ToLua
{
	short FairyID;                        
	short Star;                           
	unsigned char StarBackRate;                    	/*   星级回退概率|百分率 */
	unsigned char PetExpMul;                       	/*   宠物增加Exp|百分比 */
	unsigned char AttrType;                        	/*   元素属性类型 */
	int Atk;                             	/*   元素伤害 */
	tagFairyStarStuff_ToLua StarUpStuff;                     	/*   品质升级材料 */
	int StarSuccRate;                    	/*  Ver.805 成功率|万分率 */
};


//tolua_end
#pragma pack()

class GameTrump//tolua_export 
	: public NetMsgHandler
{//tolua_export
public:
	GameTrump(void);
	~GameTrump(void);

	// 注册网络消息
	void initTrumpNetMessage();
	void releasePetNetMessage();
	// 设置拥有者
	void setOwner( GameActor* pOwner );

public:
	//tolua_begin 
	// 出征吟唱
	void trumpPre( int nHightId, int nLowId, int nGridIdx );
	// 法宝炼化
	void trumpEvolve( int nHightId, int nLowId, int nGridIdx );
	// 停止炼化
	void trumpStopEvolve( int nHightId, int nLowId, int nGridIdx );
	// 法宝召回
	void trumpBack( int nHightId, int nLowId, int nGridIdx );
	// 注入经验值
	void trumpInfuseExp( int nHightId, int nLowId, int nGridIdx, int nIntoExp );
	//tolua_end

	// 初始化法宝信息
	void initTrumpPackage( const FAIRYPACKAGE& fairypackage );
	// s->c
	virtual int HandleNetMsg( int msgid, const tagCSPkgBody& msgbody );

public:
	//tolua_begin 
	// 获取法宝的最大数量
	int getTrumpMaxNum(){ return m_nRoleMaxTrumpNum; }
	// 获取法宝数量
	int getTrumpNum(){ return m_nRoleTrumpNum; }

	// 获取法宝信息
	tagRoleFairy_ToLua* getRoleFairy( int nGridIdx );
	// 获取当前出征法宝的信息
	tagRoleFairy_ToLua* getCampRoleFairy();
	// 是否有精灵出征
	bool isAnyFairyCamp();
	// 获取法宝静态信息
	tagFairyDef_ToLua* getFairyDef( int nId, int nLv );
	// 法定是否在出征吟唱
	bool isPre();
	// 判断是否能够召回出征精灵
	bool canBackTrump();
	// 判断是否能够继续出征精灵
	bool canCampaignTrump();
	// 判断是否能在副本出征精灵
	bool canCampInPworld( int nRealMapId );
	// 判断宠物的状态
	bool isFairyInState( int nFairyState, int nState );
	// 获取正在出征的精灵ID
	int getCampFairyId()
	{
		return m_nCurCampFairyId;
	}

	int getCampFairyLv()
	{
		return m_nCurCampFairyLv;
	}
	const tagFairyStarStuffDef_ToLua*	getFairyStarStuffDef( int itemId );
	const tagFairyStarDef_ToLua*		getFairyStarDef( int nFairyId,int nStar );
	const tagPetTraitDef_ToLua*			getPetTraitDef( int nTraidId,int nStar );


	void requestOpStarUp( int nIdx,int nStuffID,int nStuffNum,int nMoney );
	void requestOpKeyIn(int nIndex);
	void requestKeyInItem( int nSrcList,int nSrcGrid,int nDestList,int nDestGrid );

	void requestDecEvolveTime( int nIdx );

	//精灵强化需要的等级
	int getStrengthFairyLevel( int fairyID, int StarLv );

	//角色离开游戏的时候，清空精灵数据
	void clearTrumpData();
	//tolua_end

private:
	void setTrumpAttr( const FAIRYDATA& fairydata );

private:
	tagRoleFairy_ToLua      m_RoleTrump[FAIRY_MAX_NUM];
	int                     m_nRoleMaxTrumpNum;
	int                     m_nRoleTrumpNum;
	bool                    m_bPre;
	GameActor*				m_pOwner;

public:
	int						m_nCurCampFairyId;
	int						m_nCurCampFairyLv;
};//tolua_export 

