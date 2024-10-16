#include "SkillPreLockEff.h"
#include "GameActor.h"
#include "ShowActor.h"

const char *SkillPreLockEff::tag = NULL;

SkillPreLockEff::SkillPreLockEff( XMLNode& node )
{
	if( node.hasAttrib("prelockname") )
	{
		m_LockEffName = node.attribToString("prelockname");
	}
}

void SkillPreLockEff::onPrepare(GameSpell *pSpell)
{
	if( m_LockEffName.empty() )
	{
		return;
	}
	GameActor* pDesObj = ActorManager::getSingleton().FindActor(pSpell->getSingleTarget());
	if( pDesObj == NULL) 
	{
		return;
	}
	pDesObj->getShowActor()->getCurEntity()->playMotion( m_LockEffName.c_str(), true, pSpell->getSpellId() * 10 + 16 );
}

void SkillPreLockEff::onStop(GameSpell *pSpell)
{
	GameActor* pDesObj = ActorManager::getSingleton().FindActor(pSpell->getSingleTarget());
	if( pDesObj == NULL) 
	{
		return;
	}
	pDesObj->getShowActor()->getCurEntity()->stopMotion( pSpell->getSpellId()* 10 + 16 );
}