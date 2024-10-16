
#include "SkillExtraHitEff.h"
#include "GameActor.h"
#include "ShowActor.h"

const char *SkillExtraHitEff::tag = NULL;

SkillExtraHitEff::SkillExtraHitEff( XMLNode& node )
{
	if( node.hasAttrib("motionectname") )
	{
		m_MotionEffName = node.attribToString("motionectname");
	}
}

void SkillExtraHitEff::onHited( GameSpell *pSpell, int id )
{
	if( m_MotionEffName.empty() )
	{
		return;
	}
	GameActor* pDesObj = ActorManager::getSingleton().FindActor(id);
	if( pDesObj == NULL) 
	{
		return;
	}
	pDesObj->getShowActor()->getCurEntity()->playMotion( m_MotionEffName.c_str(), true, pSpell->getSpellId() );
}
