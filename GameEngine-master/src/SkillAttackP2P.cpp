#include "SkillAttackP2P.h"
#include "GameSkill_Mgr.h"
#include "OgreXMLData.h"
#include "OgreLog.h"
#include "GameDBData.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameStateAction.h"
#include "GameMsgPrint.h"
#include "OgreStringUtil.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "GameMaskMap.h"
#include "OgreEntity.h"
#include "WorldCity.h"
#include <Windows.h>
#include <MMSystem.h>

using namespace Ogre;
using namespace std;

struct tagSkill_AttackP2P : public tagSkillAttrib
{
};

static int GeneraActTime = 0;

const char	*xml_attackp2p_path[] = {"db/skill/attackp2p.xml", NULL};

IMPLEMENT_SKILL(SkillAttackP2P, tagSkill_AttackP2P)

SkillAttackP2P::SkillAttackP2P(void) : m_TargetActorID(0)
{
}

bool SkillAttackP2P::save()
{
	return false;
}

bool SkillAttackP2P::load()
{
	XMLData xmldata;
	for (int i = 0; xml_attackp2p_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_attackp2p_path[i] ) )
		{
			LOG_SEVERE("load skillattackp2p error: %s", xml_attackp2p_path[i]);
			continue;
		}

		XMLNode	root = xmldata.getRootNode();
		XMLNode	node = root.iterateChild();
		while(!node.isNull())
		{	
			if( stricmp( node.getName(), "skills" ) == 0 )
			{
				XMLNode child = node.iterateChild();
				while(!child.isNull())
				{
					ATTRIB_MAP SkillPropValue;
					tagSkill_AttackP2P* p	= new tagSkill_AttackP2P;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					p->m_strType							= "SkillAttackP2P";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// �����캯��ע���������
					SkillManager::registerSpell( p->m_nSkillId, SkillAttackP2P::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_AttackP2P* pp	= new tagSkill_AttackP2P(*p);
						GameSpell::parserSkillAttrib( grandson, pp, SkillPropValue );
						if( grandson.hasAttrib("buff") )
						{
							pp->m_nBuffId = grandson.attribToInt("buff");
						}
						if( grandson.hasAttrib("fitting") )
						{
							pp->m_nFittingId = grandson.attribToInt("fitting");
						}
						m_SkillAttrib[p->m_nSkillId].push_back(pp);
					}

					child = node.iterateChild(child);
				}
				node = root.iterateChild(node);
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

bool SkillAttackP2P::start()
{
	GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	assert(pMainPlayer != NULL);

	if (getSingleTarget() == 0)
	{
		m_TargetActorID = pMainPlayer->GetSelectTarget();
	}
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef( getSpellId(), getSpellLv() );
	if( SkillDef.TargetType & SKILL_TARGET_SELF )
	{
		/// ������ѡ����ʵ�Ŀ��
		if (getSingleTarget() != 0)
		{
			GameActor* pDesObj = ActorManager::getSingleton().FindActor(getSingleTarget());
			if (pDesObj != NULL)
			{
				if (!pDesObj->isCamp(SkillDef.TargetType))
				{
					m_TargetActorID = getOwner();
				}
			}
			else
			{
				m_TargetActorID = getOwner();
			}
		}
		else
		{
			m_TargetActorID = getOwner();
		}
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(getSingleTarget());
	if (pActor != NULL)
	{
		setTargetPoint(pActor->GetPosition());
	}
	else
	{
		setTargetPoint(pMainPlayer->GetPosition());
	}
	return GameSpell::start();
}

void SkillAttackP2P::update(unsigned int dtick)
{
	int nResult = inspect(getSingleTarget());
	if (nResult != RET_SUCCESS)
	{
		Break();
	}
	GameSpell::update(dtick);
}

void SkillAttackP2P::setSingleTarget( int targetid )
{
	m_TargetActorID = targetid;
	GameSpell::setSingleTarget(targetid);
}

int SkillAttackP2P::getSingleTarget()
{
	int ret = m_TargetActorID;
	if (ret == 0 && getTargetNum() > 0)
	{
		ret = getTarget(0);
	}
	return ret;
}

void SkillAttackP2P::onAttack(bool passive/* = false*/)
{
	GameActor *pSrcObj	= ActorManager::getSingleton().FindActor(getOwner());
	GameActor *pDesObj	= ActorManager::getSingleton().FindActor(getSingleTarget());
	if (pSrcObj && pDesObj && !passive)
	{
		// ʩ���߳���ʩ����
		pSrcObj->SetFaceTo( pSrcObj->GetPosition().x/10, 
			pSrcObj->GetPosition().z/10, 
			pDesObj->GetPosition().x/10,
			pDesObj->GetPosition().z/10, true );
	}
	GameSpell::onAttack(passive);
}

int SkillAttackP2P::inspect(int id)
{
	int result = inspecttarget(id);
	if (result == RET_NOTARGET ||
		result == RET_NOTTYPE ||
		result == RET_NOTCAMP ||
		result == RET_DEAD)
	{
		if (!ActorManager::getSingleton().getGameOpt(GAME_OPT_AUTO_SKILL_RELEASE) && !SkillManager::getSingleton().isNormalSkill(m_SkillID))
		{
			const tagSkillDef& SkillDef = getSkillDef();
			if ((SkillDef.TargetType & SKILL_TARGET_ENEMY_MON) || (SkillDef.TargetType & SKILL_TARGET_ENEMY_PLAYER) || (SkillDef.TargetType & SKILL_TARGET_ENEMYPET))
			{
				GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
				if (pMainPlayer != NULL && pMainPlayer->GetSelectTarget() == id)
				{
					GameActor *pActor = ActorManager::getSingleton().getNearestActor(pMainPlayer->GetID(), 15000);
					if (pActor != NULL)
					{
						id = pActor->GetID();
						setSingleTarget(id);
						pMainPlayer->SetSelectTarget(id);
						return inspecttarget(id);
					}
				}
			}
		}
	}
	return result;
}

const GameXYZ * SkillAttackP2P::getTargetPoint()
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(getSingleTarget());
	if (pActor == NULL)
	{
		pActor = ActorManager::getSingleton().FindActor(getOwner());
	}
	if (pActor)
	{
		m_TargetPoint = pActor->GetPosition();
	}
	return &m_TargetPoint;
}

int SkillAttackP2P::inspecttarget(int id)
{
	const tagSkillDef& SkillDef = getSkillDef();
	GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GameActor *pDesObj = ActorManager::getSingleton().FindActor(id);
	if (pDesObj == NULL)
	{
		if (SkillDef.TargetType & SKILL_TARGET_SELF)
		{
			setSingleTarget(pMainPlayer->GetID());
			pDesObj = pMainPlayer;
			return RET_SUCCESS;
		}
		else
		{
			return RET_NOTARGET;
		}
	}
	if (pDesObj == pMainPlayer)
	{
		if (SkillDef.TargetType & SKILL_TARGET_SELF)
		{
			return RET_SUCCESS;
		}
		else
		{
			return RET_NOTTYPE;
		}
	}
	if (pDesObj->getActState().getInState(GSACT_IN_BIRTH) || pDesObj->getActState().GetState() == GSACT_AUTOFLY)
	{
		return RET_UNKNOWN;
	}
	if ( !WorldCityManager::getSingleton().objCanAttack(pDesObj) )
	{
		return RET_WORLDCITYCONTROL;
	}
	if( pDesObj->getType() == GAT_ITEM ||
		( pDesObj->getType() == GAT_MONSTER &&
		(pDesObj->getSubType() == MON_SUB_TYPE_COLLECT ||
		pDesObj->getSubType() == MON_SUB_TYPE_TRAP ||
		pDesObj->getSubType() == MON_SUB_TYPE_BUILD) ) )
	{
		if (SkillDef.TargetType & SKILL_TARGET_SELF)
		{
			setSingleTarget(pMainPlayer->GetID());
			pDesObj = pMainPlayer;
			return RET_SUCCESS;
		}
		else
		{
			// ʩ�����󲻷���
			return RET_NOTTYPE;
		}
	}
	if( !pDesObj->isCamp(SkillDef.TargetType))
	{
		// ע�ⷨ���ǶԶ��Ѹ�����������Ŀ���������ж�
		if(SkillDef.TargetRange != TARGET_RANGE_FRIEND)
		{
			if (SkillDef.TargetType & SKILL_TARGET_SELF)
			{
				setSingleTarget(pMainPlayer->GetID());
				pDesObj = pMainPlayer;
				return RET_SUCCESS;
			}
			else
			{
				// ʩ��������Ӫ������
				return RET_NOTCAMP;
			}
		}
	}
	if( pDesObj->getActState().GetState() == GSACT_DEAD && !SkillDef.DeadTarget )
	{
		if (SkillDef.TargetType & SKILL_TARGET_SELF)
		{
			setSingleTarget(pMainPlayer->GetID());
			pDesObj = pMainPlayer;
			return RET_SUCCESS;
		}
		else
		{
			return RET_DEAD;
		}
	}
	if (SkillManager::getSingleton().getSkillDistance(getSpellId()) < ActorManager::getSingleton().getActorDistance(id))
	{
		return RET_OUTRANGE;
	}
	if (!(SkillDef.NoSkillStop & NOSKILLSTOP_TYPE_MAGIC) &&
		!pMainPlayer->getCurMap()->getMaskMap()->lineOfSightTest( 
		Ogre::Point2D(pMainPlayer->GetPosition().x/10, pMainPlayer->GetPosition().z/10),
		Ogre::Point2D(pDesObj->GetPosition().x/10, pDesObj->GetPosition().z/10), GameMaskMap::maskMagic))
	{
		return RET_MASKTARGET;
	}
	if (!pMainPlayer->getCurMap()->getMaskMap()->lineOfSightTest( 
		Ogre::Point2D(pMainPlayer->GetPosition().x/10, pMainPlayer->GetPosition().z/10),
		Ogre::Point2D(pDesObj->GetPosition().x/10, pDesObj->GetPosition().z/10), GameMaskMap::maskLoMagic, SkillDef.ThroughLoMagic))
	{
		return RET_MASKTARGET;
	}
	return RET_SUCCESS;
}

void SkillAttackP2P::onUpdate(unsigned int dtick)
{
	GameActor *pSrcObj = ActorManager::getSingleton().FindActor(getOwner());
	GameActor *pDesObj = ActorManager::getSingleton().FindActor(getSingleTarget());
	if ( pSrcObj && pDesObj )
	{
		// ����ʩ��Ŀ��
		pSrcObj->SetFaceTo( pSrcObj->GetPosition().x/10, 
			pSrcObj->GetPosition().z/10, 
			pDesObj->GetPosition().x/10,
			pDesObj->GetPosition().z/10, true );
	}
	GameSpell::onUpdate(dtick);
}