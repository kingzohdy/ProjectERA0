

#ifndef _SKILLATTACKLEAVEEFF_H
#define _SKILLATTACKLEAVEEFF_H

#include <string>
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
using namespace std;
using namespace Ogre;

class SkillAttackLeaveEff : public SKillObserver
{
	DECLARE_OBSERVER(SkillAttackLeaveEff)
protected:
	string			m_strAttackLeaveEff;

public:
	SkillAttackLeaveEff(XMLNode& node);

	virtual void onAttack(GameSpell *pSpell);
};


#endif // _SKILLATTACKLEAVEEFF_H