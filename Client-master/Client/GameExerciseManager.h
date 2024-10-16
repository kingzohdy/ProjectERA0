#pragma once
#include "NetMessage.h"
#include "OgreSingleton.h"
#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"

//tolua_begin
#pragma pack(1)
struct tagRongYan_ToLua
{
	int ExpMul;                          	/*   经验倍数 */
	int ItemNum;                         	/*   熔岩骇心多少个 */
};

struct tagRongYanJuShouDef_ToLua
{
	int Level;                           	/*   层数 */
	int MonID;                           	/*   怪物id */
	int MonLifeTime;                     	/*   怪物生存时间(单位秒) */
	int Odds;                            	/*   出现真boss的几率 */
	tagRongYan_ToLua RongYan[MAX_RONGYAN_BEISHU];    
};

struct tagRongYanMonDynAttrDef_ToLua
{
	unsigned int Level;                           	/*   平均级别 */
	short IceDefMul;                       	/*   冰属性防御力修正 */
	short PhsicDefMul;                     	/*   物理属性防御力修正 */
	short HitMul;                          	/*   命中力 */
	short FleeMul;                         	/*   闪避力 */
	short DynHPMul;                        	/*   血量修正 */
	short DynHurtMul;                      	/*   最后伤害修正 */
	short IceHurtMul;                      	/*   冰伤害修正 */
	short PhsicHurtMul;                    	/*   物伤害修正 */
	short IceHurtDefMul;                   	/*   冰属性抗性修正 */
	short PhsicHurtDefMul;                 	/*   物理属性抗性修正 */
	short ElementAtkMul;                   	/*   元素攻击修正 */
	short ElementDefMul;                   	/*   元素防御修正 */
	short DynExpMul;                       	/*   获得经验修正 */
};

#pragma pack()
#define CLT_RONG_YAN_ITEM_ID			RONGYAN_HAIXIN_ITEM_ID
#define ZJ_RONGYAN_ITEM_ID_TOLUA		ZJ_RONGYAN_ITEM_ID
//tolua_end

class GameExerciseManager  //tolua_export
	: public NetMsgHandler,public Ogre::Singleton<GameExerciseManager>
{//tolua_export
public:
	GameExerciseManager(void);
	~GameExerciseManager(void);
	//tolua_begin

    /* 熔岩巨兽部分*/
	void sendBossChooseOption( bool bChosen );				// 真假boss是否选择
	int  getRongYanMonsterBootyExp( int nID );				// 获得基础经验值
	void sendBeginMultiplityNum( int nNum );				// 选择倍数索引 1-4

	//高级熔岩巨兽
	void CallHighLvRongYanMonster();
	void sendHighLvBossChooseOption( bool bChosen );				// 终极熔岩，真假boss是否选择
	
	const tagRongYanJuShouDef_ToLua* getRongYanJuShouDef( int nIndex );
	const tagRongYanMonDynAttrDef_ToLua* getRongYanMonAttrDef( int nLv );
	//tolua_end
public:
	virtual int HandleNetMsg(int msgid, const tagCSPkgBody& msgbody);
};//tolua_export	