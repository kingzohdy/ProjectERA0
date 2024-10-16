
#include "SkillPrepareAttackPointEff.h"
#include "GameActor.h"
#include "EntityEffect.h"
#include "GameEffect_Mgr.h"
#include "GameMaskMap.h"

const char *SkillPrepareAttackPointEff::tag = NULL;

SkillPrepareAttackPointEff::SkillPrepareAttackPointEff( XMLNode& node )
{
	if( node.hasAttrib("preattackeff") )
	{
		m_strPreAttackeff = node.attribToString("preattackeff");
	}
}

void SkillPrepareAttackPointEff::onPrepare(GameSpell *pSpell)
{
	if( !m_strPreAttackeff.empty() )
	{
		GameActor* pSrcObj = ActorManager::getSingleton().FindActor( pSpell->getOwner() );
		if (pSrcObj == NULL)
		{
			return;
		}
		const GameXYZ &selfPos = pSpell->getSelfPoint();
		GameXYZ targetPos = *(pSpell->getTargetPoint());
		EntityEffect *effect = new EntityEffect(m_strPreAttackeff.c_str(), pSrcObj->GetID());
		effect->start();
		effect->setpos(targetPos);
		if (selfPos == targetPos)
		{
			effect->setAngle(pSrcObj->getAngle());
		}
		else
		{
			int nAngle = FaceAngleBetween(selfPos.x / 10, selfPos.z / 10, targetPos.x / 10, targetPos.z / 10);
			effect->setAngle(nAngle);
		}
	}
}
