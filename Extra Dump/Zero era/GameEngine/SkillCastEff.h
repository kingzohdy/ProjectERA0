

#ifndef _SKILLCASTEFF_H
#define _SKILLCASTEFF_H

#include <string>
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
using namespace std;
using namespace Ogre;

class SkillCastEff : public SKillObserver
{
	DECLARE_OBSERVER(SkillCastEff)
protected:
	string		m_MotionEffName;	// ¹¥»÷¶îÍâÐ§¹û

public:
	SkillCastEff(XMLNode& node);

	virtual void onAttack(GameSpell *pSpell);
};


#endif