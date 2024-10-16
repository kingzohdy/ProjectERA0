#ifndef _SKILLEXTRAHITEFF_H
#define _SKILLEXTRAHITEFF_H

#include <string>
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
using namespace std;
using namespace Ogre;

class SkillExtraHitEff : public SKillObserver
{
	DECLARE_OBSERVER(SkillExtraHitEff)
protected:
	string		m_MotionEffName;	// 被击额外效果

public:
	SkillExtraHitEff(XMLNode& node);

	virtual void onHited(GameSpell *pSpell, int id);
};


#endif // _SKILLTHROW_H