#pragma once
#include "GameSkill_Common.h"
#include <string>
#include "OgreXMLData.h"
#include "GameTickAction.h"
using namespace std;

class SkillBlow : public SKillObserver
{
	DECLARE_OBSERVER(SkillBlow)

	class DelayTick : public ITickActionDelay
	{
	private:
		SkillBlow	*ptr;
	public:
		DelayTick(SkillBlow *parent, unsigned int time) : ITickActionDelay(time), ptr(parent) {}
		unsigned int expire()
		{
			ptr->onStart();
			return halt();
		}
	};
protected:
	int				m_nAttackDelay;		// 每播导弹攻击间隔时间
	int				m_nAttackTime;		// 导弹批次
	int				m_nAttackRadius;	// 导弹半径
	int				m_nBeginThrowDelay;	// 丢出飞行物体的延迟
	string			m_strBlowEffName;	// 导弹特效
	GameSpell		*m_pSpell;

	DelayTick		*tick;

public:
	SkillBlow(Ogre::XMLNode &node);
	virtual ~SkillBlow();

	virtual void onAttack(GameSpell *pSpell);
	virtual void onStart();
};
