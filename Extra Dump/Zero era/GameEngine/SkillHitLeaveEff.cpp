
#include "SkillHitLeaveEff.h"
#include "GameActor.h"
#include "EntityEffect.h"
#include "GameEffect_Mgr.h"
#include "GameMaskMap.h"

const char *SkillHitLeaveEff::tag = NULL;


SkillHitLeaveEff::SkillHitLeaveEff( XMLNode& node )
{
	if( node.hasAttrib("hitleaveeff") )
	{
		m_strHitLeaveEff = node.attribToString("hitleaveeff");
	}
}

void SkillHitLeaveEff::onHited( GameSpell *pSpell, int id )
{
	if( !m_strHitLeaveEff.empty() )
	{
		GameActor* pSrcObj = ActorManager::getSingleton().FindActor( pSpell->getOwner() );
		if (pSrcObj != NULL)
		{
			GameActor* pDesObj = ActorManager::getSingleton().FindActor(id);
			if( pDesObj == NULL) 
			{
				return;
			}
			const GameXYZ &selfPos = pSpell->getSelfPoint();
			GameXYZ targetPos = pDesObj->GetPosition();
			EntityEffect *effect = new EntityEffect(m_strHitLeaveEff.c_str(), pSrcObj->GetID());
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
