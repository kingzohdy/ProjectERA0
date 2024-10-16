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
	int						m_nSrcLinkId;		// 起始绑定点
	int						m_nDesLinkId;		// 目标绑定点
	int						m_nLinkTime;		// 链接时间
	bool					m_bRevert;
	int						m_nLinkDelateTime;	// 每次链接延迟
	string					m_strLinkEffect;	// 链接特效

	list<LinkFollowEffectPtr>	m_pLinkEffectList;

public:
	SkillLink(XMLNode &node);

	virtual void onAttack(GameSpell *pSpell);
	virtual void onStop(GameSpell *pSpell);
};
