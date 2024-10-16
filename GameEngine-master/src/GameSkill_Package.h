#pragma once

#include "GameEngineTypes.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include <map>
#include <set>

enum{
	ACTION_HIT_NONE	= 0,	
	ACTION_HIT_MISS,		// 闪避
	ACTION_HIT_INVALID,		// 无效
	ACTION_HIT_HIT,			// 受击
	ACTION_HIT_HEAVY,		// 重击
	ACTION_HIT_BACK,		// 击飞
	ACTION_HIT_BACKDEAD,	// 击飞死亡
	ACTION_HIT_DEAD,		// 死亡
};

class GameSkill_Package
{
public:
	GameSkill_Package(void);
	~GameSkill_Package(void);

public:
	// 是否对所有人都显示冒血伤害
	bool showDamageForAll();
	// 执行技能包(返回动作表现类型)
	void	doPackage();
	int		doPackage(int id);
	// 设置对象
	void setTargetState(int id, const CSHITS& Package);
	// 刷新目标HP，MP
	void updateTargetValue();
	// 获取收到包时刻的时间
	unsigned int getTime();
	
private:
	// 计算技能吟唱回退打断
	void calSkillHitBack( GameActor* pObj, int nHitValue );

public:
	CSHITS			m_HitValue;			
	int				m_HitType;			// 受击伤害飘动类型
	int				m_nAttackerID;		// 攻击者ID
	unsigned int	m_uStartTime;
	unsigned int	m_uStatusIdx;
	std::map<int, tagCSOneHit *>	m_Hits;
	std::set<int>		m_hitbackIds;
};
