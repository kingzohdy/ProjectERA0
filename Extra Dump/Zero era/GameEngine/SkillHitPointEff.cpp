
#include "SkillHitPointEff.h"
#include "GameActor.h"
#include "EntityEffect.h"
#include "GameEffect_Mgr.h"
#include "GameMaskMap.h"

const char *SkillHitPointEff::tag = NULL;

SkillHitPointEff::SkillHitPointEff( XMLNode& node )
{
	if( node.hasAttrib("hitpointeff") )
	{
		m_strHitPointEff = node.attribToString("hitpointeff");
	}
}

void SkillHitPointEff::onHited( GameSpell *pSpell )
{
	if( !m_strHitPointEff.empty() )
	{
		GameActor* pSrcObj = ActorManager::getSingleton().FindActor( pSpell->getOwner() );
		if (pSrcObj != NULL)
		{
			const GameXYZ &selfPos = pSpell->getSelfPoint();
			GameXYZ targetPos = *(pSpell->getTargetPoint());
			EntityEffect *effect = new EntityEffect(m_strHitPointEff.c_str(), pSrcObj->GetID());
			effect->start();
			effect->setpos(targetPos);
			if (selfPos == targetPos)
			{
				effect->setAngle(pSrcObj->getAngle() + 180);
			}
			else
			{
				int nAngle = FaceAngleBetween(targetPos.x / 10, targetPos.z / 10, selfPos.x / 10, selfPos.z / 10);
				effect->setAngle(nAngle);
			}
		}
	}
}
