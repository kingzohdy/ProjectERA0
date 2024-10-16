#include "SkillBlow.h"
#include "GameActor.h"
#include "GameMap.h"
#include "EntityEffect.h"
#include "GameEffect_Mgr.h"
#include <Windows.h>

SkillBlow::SkillBlow( Ogre::XMLNode &node ) : tick(NULL)
{
	if (node.hasAttrib( "attackdelay" ))
	{
		m_nAttackDelay				= node.attribToInt( "attackdelay" );
	}
	else
	{
		m_nAttackDelay = 0;
	}

	if (node.hasAttrib( "attacktime" ))
	{
		m_nAttackTime				= node.attribToInt( "attacktime" );
	}
	else
	{
		m_nAttackTime = 1;
	}

	if (node.hasAttrib( "radius" ))
	{
		m_nAttackRadius				= node.attribToInt( "radius" );
	}
	else
	{
		m_nAttackRadius = 1;
	}

	if (node.hasAttrib( "bloweff" ))
	{
		m_strBlowEffName			= node.attribToString("bloweff");
	}

	if (node.hasAttrib( "throwdelay" ))
	{
		m_nBeginThrowDelay			= node.attribToInt("throwdelay");
	}
	else
	{
		m_nBeginThrowDelay = 0;
	}
}

SkillBlow::~SkillBlow()
{
	delete tick;
}

void SkillBlow::onAttack( GameSpell *pSpell )
{
	m_pSpell = pSpell;
	tick = new DelayTick(this, m_nBeginThrowDelay);
}

void SkillBlow::onStart()
{
	if( !m_strBlowEffName.empty() )
	{
		GameActor* pSrcObj = ActorManager::getSingleton().FindActor(m_pSpell->getOwner());
		if (pSrcObj == NULL)
		{
			return;
		}
		GameXYZ selfPos = pSrcObj->GetPosition();
		GameXYZ targetPos = *(m_pSpell->getTargetPoint());
		int nAngle = FaceAngleBetween(selfPos.x, selfPos.z, 
			targetPos.x, targetPos.z);
		for( int i = 0; i < m_nAttackTime; i++ )
		{ 
			int randLenth			= int(pow(rand() / double(RAND_MAX), 1 / 3.0) * m_nAttackRadius * 1000 / 2 + 0.5);
			int randDir				= rand() % 360;
			GameXYZ pos;
			pos.x		=	(int) (targetPos.x + (double)randLenth * cos((double)(randDir)*3.14159265/180.0f));
			pos.z		=	(int) (targetPos.z - (double)randLenth * sin((double)(randDir)*3.14159265/180.0f));
			pSrcObj->getCurMap()->getTerrainH(pos.x, pos.z, pos.y);
			EntityEffect *effect = new EntityEffect(m_strBlowEffName.c_str(), pSrcObj->GetID());
			effect->setpos(pos);
			effect->setAngle(nAngle);
			EffectManage::getSingleton().display(effect, i * m_nAttackDelay);
		} 
	}
}
