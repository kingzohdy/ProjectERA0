

#ifndef _SKILLATTACKPOINTEFF_H
#define _SKILLATTACKPOINTEFF_H

#include <string>
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
using namespace std;
using namespace Ogre;

class SkillAttackPointEff : public SKillObserver
{
	DECLARE_OBSERVER(SkillAttackPointEff)
protected:
	string			m_strAttackPointEff;	// ÊÜ»÷²ÐÁôÌØÐ§

public:
	SkillAttackPointEff(XMLNode& node);

	virtual void onAttack(GameSpell *pSpell);
};


#endif // _SKILLTHROW_H