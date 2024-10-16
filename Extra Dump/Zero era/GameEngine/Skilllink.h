#pragma once
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
#include "LinkEffect.h"
#include <list>
using namespace std;
using namespace Ogre;

class SkillLink : public SKillObserver
{
	DECLARE_OBSERVER(SkillLink)
protected:
	int						m_nSrcLinkId;		// ��ʼ�󶨵�
	int						m_nDesLinkId;		// Ŀ��󶨵�
	int						m_nLinkTime;		// ����ʱ��
	bool					m_bRevert;
	int						m_nLinkDelateTime;	// ÿ�������ӳ�
	string					m_strLinkEffect;	// ������Ч

	list<LinkFollowEffectPtr>	m_pLinkEffectList;

public:
	SkillLink(XMLNode &node);

	virtual void onAttack(GameSpell *pSpell);
	virtual void onStop(GameSpell *pSpell);
};
