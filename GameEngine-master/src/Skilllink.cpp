#include "Skilllink.h"
#include "GameActor.h"
#include "GameSkill_Mgr.h"
#include "GameEffect_Mgr.h"

const char *SkillLink::tag = NULL;

SkillLink::SkillLink( XMLNode &node )
{
	if (!node.hasAttrib("srclinkid") || !node.attribToInt("srclinkid", m_nSrcLinkId))
	{
		m_nSrcLinkId = 0;
	}

	if (!node.hasAttrib("deslinkid") || !node.attribToInt("deslinkid", m_nDesLinkId))
	{
		m_nDesLinkId = 0;
	}

	if (!node.hasAttrib("linktime") || !node.attribToInt("linktime", m_nLinkTime))
	{
		m_nLinkTime = 0;
	}

	if (!node.hasAttrib("linkdelatetime") || !node.attribToInt("linkdelatetime", m_nLinkDelateTime))
	{
		m_nLinkDelateTime = 0;
	}

	if( node.hasAttrib("linkeffect") )
	{
		m_strLinkEffect = node.attribToString("linkeffect");	
	}
	m_bRevert = m_nLinkTime < 0;
	m_nLinkTime = m_nLinkTime > 0 ? m_nLinkTime : -m_nLinkTime;
}

void SkillLink::onAttack( GameSpell *pSpell )
{
	GameActor *pSrcObj = ActorManager::getSingleton().FindActor( pSpell->getOwner() );
	for (list<LinkFollowEffectPtr>::iterator i = m_pLinkEffectList.begin(); i != m_pLinkEffectList.end(); ++i)
	{
		(*i)->destory();
	}
	m_pLinkEffectList.clear();
	if (pSrcObj == NULL) 
	{
		return;
	}
	if (pSpell->getTargetNum() > 0)
	{
		GameActor* pDesObj = ActorManager::getSingleton().FindActor(pSpell->getTarget(0));
		if (pDesObj != NULL)
		{
			GameActor *pActor = pSrcObj;
			if (pSrcObj->isOnMonsterMachine())
			{
				pActor = ActorManager::getSingleton().FindActor(pSrcObj->m_MonsterMachine.m_playerInfo.m_nMonsterMemID);
			}
			LinkFollowEffect *effect;
			if (m_bRevert)
			{
				effect = new LinkFollowEffect(m_strLinkEffect.c_str(), pDesObj->GetID(), m_nDesLinkId,
					pActor->GetID(), m_nSrcLinkId, pSpell->getSpellId());
			}
			else
			{
				effect = new LinkFollowEffect(m_strLinkEffect.c_str(), pActor->GetID(), m_nSrcLinkId,
					pDesObj->GetID(), m_nDesLinkId, pSpell->getSpellId());
			}
			effect->start();
			m_pLinkEffectList.push_back(effect);
			EffectManage::getSingleton().destory(effect, m_nLinkTime);
		}
	}
}

void SkillLink::onStop( GameSpell *pSpell )
{
	if (pSpell->getSkillDef().PilotNum > 1)
	{
		for (list<LinkFollowEffectPtr>::iterator i = m_pLinkEffectList.begin(); i != m_pLinkEffectList.end(); ++i)
		{
			(*i)->destory();
		}
	}
	m_pLinkEffectList.clear();
}
