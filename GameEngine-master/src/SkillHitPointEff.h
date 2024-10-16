

#ifndef _SKILLHITPOINTEFF_H
#define _SKILLHITPOINTEFF_H

#include <string>
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
using namespace std;
using namespace Ogre;

class SkillHitPointEff : public SKillObserver
{
	DECLARE_OBSERVER(SkillHitPointEff)
protected:
	string			m_strHitPointEff;	// ÊÜ»÷²ÐÁôÌØÐ§

public:
	SkillHitPointEff(XMLNode& node);

	virtual void onHited(GameSpell *pSpell);
};


#endif // _SKILLTHROW_H