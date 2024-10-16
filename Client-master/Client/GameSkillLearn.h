#pragma once
#include "GameEngineTypes.h"
#include "OgreSingleton.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include "resdb_meta.h"
#include <map>

const int MAX_SKILLLEARN = 300;

//tolua_begin 
enum
{
	SKILLLEARN_TYPE_PUBLIC,				// 共有
};

enum
{
	SKILLLEARN_SUCCED,					// 学习技能成功

	SKILLLEARN_FAIL_LVMIN,				// 等级不够
	SKILLLEARN_FAIL_CAREER,				// 职业不符合
	SKILLLEARN_FAIL_PREDIATHESIS,		// 前置素质不符合
	SKILLLEARN_FAIL_ITEM,				// 道具不符合
	SKILLLEARN_FAIL_SKILLPOINT,			// 技能点不足
	SKILLLEARN_FAIL_MONEY,				// 金钱不足
	SKILLLEARN_FAIL_QUEST,				// 任务不符合
	SKILLLEARN_FAIL_NONEED,				// 学习方式不符合
	SKILLlEARN_FAIL_SKILLLV,			// 技能学习等级过高 
	SKILLlEARN_FAIL_EXP,				// 经验不够
};

struct tagOneSkillLearnInfo
{
	int		m_nSkillId;
	int		m_nLv;
};

struct tagSkillLearnInfo
{
	int						m_nType;	// 类型（共有或按武器类型区分）
	int						m_nNum;		// 数量
	tagOneSkillLearnInfo	m_SkillInfo[MAX_SKILLLEARN];				
};
//tolua_end

class GameSkillLearn//tolua_export
	: public Ogre::Singleton<GameSkillLearn>
{//tolua_export
public:
	GameSkillLearn(void);
	~GameSkillLearn(void);

public:
	//tolua_begin 
	// 技能是否自动学习
	bool	isSkillAutoLearn(int skillId, int lv);
	// 技能是否需要技能书学习
	bool	isSkillNeedBookLearn(int skillId, int lv);
	// 技能是否可以学习
	int		checkSkillLearn( int skillId, int lv );
	// 刷新技能学习列表
	void	updateSkillLearnList();
	// 获取全部
	const tagSkillLearnInfo* getAllSkill( int type )const;
	// 获取可学
	const tagSkillLearnInfo* getCanLearnSkill( int type )const;
	
	//获取所指定的技能
	const tagOneSkillLearnInfo* getSkillInfo( int nSkillType, int nIndex, bool isAll ) const;
	// 技能学习
	void skillLearn( int npcId, int skillId, int lv );
	// 技能升级
	void skillLvUp( int npcId, int skillId, int lv );
	// 素质学习
	void diathesisLearn( int npcId, int diathesisId, int lv );
	// 素质升级
	void diathesisLvUp( int npcId, int diathesisId, int lv );
	// 天赋学习
	void geniusLearn( int npcId, int geniusId, int lv );
	// 天赋升级
	void geniusLvUp( int npcId, int geniusId, int lv );
	// 天赋重置
	void geniusReset( int npcId );
	//tolua_end
private:
	std::map<int, tagSkillLearnInfo>	m_SkillLearnAll;	// 全部
	std::map<int, tagSkillLearnInfo>	m_SkillLearnCan;	// 可学

	std::vector<tagSkillDef>			m_AllSkillDef;
};//tolua_export
