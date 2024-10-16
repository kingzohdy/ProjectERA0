
#include "SkillCastEff.h"
#include "GameActor.h"
#include "ShowActor.h"

const char *SkillCastEff::tag = NULL;

SkillCastEff::SkillCastEff( XMLNode& node )
{
	if( node.hasAttrib("casteff") )
	{
		m_MotionEffName = node.attribToString("casteff");
	}
}

void SkillCastEff::onAttack( GameSpell *pSpell )
{
	if( m_MotionEffName.empty() )
	{
		return;
	}
	GameActor* pSrcObj = ActorManager::getSingleton().FindActor(pSpell->getOwner());
	if( pSrcObj != NULL) 
	{
		GameActor *pActor = pSrcObj;
		if (pSrcObj->isOnMonsterMachine())
		{
			pActor = ActorManager::getSingleton().FindActor(pSrcObj->m_MonsterMachine.m_playerInfo.m_nMonsterMemID);
		}
		pActor->getShowActor()->getCurEntity()->playMotion( m_MotionEffName.c_str(), true, pSpell->getSpellId() * 200 );
	}
}
