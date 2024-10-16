#ifndef _SKILLPRELOCKEFF_H
#define _SKILLPRELOCKEFF_H

#include <string>
#include "GameSkill_Common.h"
#include "OgreXMLData.h"
using namespace std;
using namespace Ogre;

class SkillPreLockEff : public SKillObserver
{
	DECLARE_OBSERVER(SkillPreLockEff)
protected:
	string		m_LockEffName;	// Ò÷³ªËø¶¨Ð§¹û
	
public:
	SkillPreLockEff(XMLNode& node);
	virtual void	onPrepare(GameSpell *pSpell);
	virtual void	onStop(GameSpell *pSpell);
};


#endif // _SKILLTHROW_H