

#ifndef _SKILLHITLEAVE_H
#define _SKILLHITLEAVE_H

#include <string>
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
using namespace std;
using namespace Ogre;

class SkillHitLeaveEff : public SKillObserver
{
	DECLARE_OBSERVER(SkillHitLeaveEff)
protected:
	string			m_strHitLeaveEff;	// ÊÜ»÷²ÐÁôÌØÐ§

public:
	SkillHitLeaveEff(XMLNode& node);

	virtual void onHited(GameSpell *pSpell, int id);
};


#endif // _SKILLTHROW_H