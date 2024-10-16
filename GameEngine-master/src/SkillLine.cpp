#include "SkillLine.h"
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

struct tagSkill_Line : public tagSkillAttrib
{
};

static int GeneraActTime = 0;

const char	*xml_line_path[] = {"db/skill/line.xml", NULL};

IMPLEMENT_SKILL(SkillLine, tagSkill_Line)

SkillLine::SkillLine(void) : m_TargetActorID(0), m_isAttack(false)
{
}

bool SkillLine::save()
{
	return false;
}

bool SkillLine::load()
{
	XMLData xmldata;
	for (int i = 0; xml_line_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_line_path[i] ) )
		{
			LOG_SEVERE("load skillline error: %s", xml_line_path[i]);
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
					tagSkill_Line* p	= new tagSkill_Line;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					p->m_strType							= "SkillLine";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillLine::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_Line* pp	= new tagSkill_Line(*p);
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

bool SkillLine::start()
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
		/// 尽可能选择合适的目标
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
	return GameSpell::start();
}

void SkillLine::update(unsigned int dtick)
{
	int nResult = inspect(getSingleTarget());
	if (nResult != RET_SUCCESS)
	{
		Break();
	}
	GameSpell::update(dtick);
}

void SkillLine::setSingleTarget( int targetid )
{
	m_TargetActorID = targetid;
	GameSpell::setSingleTarget(targetid);
}

int SkillLine::getSingleTarget()
{
	int ret = m_TargetActorID;
	if (ret == 0 && getTargetNum() > 0)
	{
		ret = getTarget(0);
	}
	return ret;
}

void SkillLine::onAttack(bool passive/* = false*/)
{
	GameActor *pSrcObj	= ActorManager::getSingleton().FindActor(getOwner());
	GameActor *pDesObj	= ActorManager::getSingleton().FindActor(getSingleTarget());
	if (pSrcObj && pDesObj && !passive)
	{
		// 施法者朝向施法点
		pSrcObj->SetFaceTo( pSrcObj->GetPosition().x/10, 
			pSrcObj->GetPosition().z/10, 
			pDesObj->GetPosition().x/10,
			pDesObj->GetPosition().z/10, true );
	}
	GameSpell::onAttack(passive);
}

int SkillLine::inspect(int id)
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

int SkillLine::inspecttarget(int id)
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
	if (pDesObj->getActState().getInState(GSACT_IN_BIRTH))
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
			// 施法对象不符合
			return RET_NOTTYPE;
		}
	}
	if( !pDesObj->isCamp(SkillDef.TargetType))
	{
		// 注意法术是对队友辅助法术，则目标对象忽略判断
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
				// 施法对象阵营不符合
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

const GameXYZ * SkillLine::getTargetPoint()
{
	if (!m_isAttack)
	{
		GameActor *pActor = ActorManager::getSingleton().FindActor(getSingleTarget());
		if (pActor != NULL)
		{
			m_TargetPoint = pActor->GetPosition();
		}
		else
		{
			m_TargetPoint = ActorManager::getSingleton().FindActor(getOwner())->GetPosition();
		}
	}
	return &m_TargetPoint;
}

void SkillLine::setTargetPoint(const GameXYZ &pos)
{
	m_TargetPoint = pos;
}

void SkillLine::attackTime()
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(getOwner());
	if (pActor != NULL)
	{
		m_isAttack = true;
		setTargetPoint(pActor->GetPosition());
	}
	GameSpell::attackTime();
}
