

#ifndef _SKILLPREPAREATTACKPOINTEFF_H
#define _SKILLPREPAREATTACKPOINTEFF_H

#include <string>
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
using namespace std;
using namespace Ogre;

class SkillPrepareAttackPointEff : public SKillObserver
{
	DECLARE_OBSERVER(SkillPrepareAttackPointEff)
protected:
	string			m_strPreAttackeff;	// 吟唱目标点特效

public:
	SkillPrepareAttackPointEff(XMLNode& node);

	virtual void	onPrepare(GameSpell *pSpell);
};


#endif // _SKILLTHROW_H