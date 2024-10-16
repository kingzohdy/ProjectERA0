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
	int				m_nAttackDelay;		// ÿ�������������ʱ��
	int				m_nAttackTime;		// ��������
	int				m_nAttackRadius;	// �����뾶
	int				m_nBeginThrowDelay;	// ��������������ӳ�
	string			m_strBlowEffName;	// ������Ч
	GameSpell		*m_pSpell;

	DelayTick		*tick;

public:
	SkillBlow(Ogre::XMLNode &node);
	virtual ~SkillBlow();

	virtual void onAttack(GameSpell *pSpell);
	virtual void onStart();
};
