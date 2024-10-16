
#include "SkillAttackLeaveEff.h"
#include "GameActor.h"
#include "EntityEffect.h"
#include "GameEffect_Mgr.h"
#include "GameMaskMap.h"

const char *SkillAttackLeaveEff::tag = NULL;


SkillAttackLeaveEff::SkillAttackLeaveEff( XMLNode& node )
{
	if( node.hasAttrib("attackleaveeff") )
	{
		m_strAttackLeaveEff = node.attribToString("attackleaveeff");
	}
}

void SkillAttackLeaveEff::onAttack( GameSpell *pSpell )
{
	if( !m_strAttackLeaveEff.empty() )
	{
		GameActor* pSrcObj = ActorManager::getSingleton().FindActor( pSpell->getOwner() );
		if (pSrcObj == NULL)
		{
			return;
		}
		const GameXYZ &selfPos = pSpell->getSelfPoint();
		GameXYZ targetPos = *(pSpell->getTargetPoint());
		EntityEffect *effect = new EntityEffect(m_strAttackLeaveEff.c_str(), pSrcObj->GetID());
		effect->start();
		effect->setpos(selfPos);
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
