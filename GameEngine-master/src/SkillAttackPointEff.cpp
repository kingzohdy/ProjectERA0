
#include "SkillAttackPointEff.h"
#include "GameActor.h"
#include "EntityEffect.h"
#include "GameEffect_Mgr.h"
#include "GameMaskMap.h"

const char *SkillAttackPointEff::tag = NULL;

SkillAttackPointEff::SkillAttackPointEff( XMLNode& node )
{
	if( node.hasAttrib("attackpointeff") )
	{
		m_strAttackPointEff = node.attribToString("attackpointeff");
	}
}

void SkillAttackPointEff::onAttack( GameSpell *pSpell )
{
	if( !m_strAttackPointEff.empty() )
	{
		GameActor* pSrcObj = ActorManager::getSingleton().FindActor( pSpell->getOwner() );
		if (pSrcObj == NULL)
		{
			return;
		}
		const GameXYZ &selfPos = pSpell->getSelfPoint();
		GameXYZ targetPos = *(pSpell->getTargetPoint());
		EntityEffect *effect = new EntityEffect(m_strAttackPointEff.c_str(), pSrcObj->GetID());
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
