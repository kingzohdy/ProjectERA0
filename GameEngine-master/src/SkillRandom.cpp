#include "SkillRandom.h"
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

struct tagSkill_Random : public tagSkillAttrib
{
};

static int GeneraActTime = 0;

const char	*xml_random_path[] = {"db/skill/random.xml", NULL};

IMPLEMENT_SKILL(SkillRandom, tagSkill_Random)

SkillRandom::SkillRandom(void) : m_TargetActorID(0)
{
}

bool SkillRandom::save()
{
	return false;
}

bool SkillRandom::load()
{
	XMLData xmldata;
	for (int i = 0; xml_random_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_random_path[i] ) )
		{
			LOG_SEVERE("load skillrandom error: %s", xml_random_path[i]);
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
					tagSkill_Random* p	= new tagSkill_Random;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					p->m_strType							= "SkillRandom";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillRandom::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_Random* pp	= new tagSkill_Random(*p);
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

bool SkillRandom::start()
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

void SkillRandom::setSingleTarget( int targetid )
{
	m_TargetActorID = targetid;
	GameSpell::setSingleTarget(targetid);
}

int SkillRandom::getSingleTarget()
{
	return m_TargetActorID;
}

void SkillRandom::onAttack(bool passive/* = false*/)
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

int SkillRandom::inspect(int id)
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

const GameXYZ * SkillRandom::getTargetPoint()
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

void SkillRandom::attackTime()
{
	m_lastTime = timeGetTime();
	// 计算目范法术攻击目标
	selectNextTarget();
	if (m_SkillState >= STATE_BREAK)
	{
		return;
	}
	tagSkillAttrib* pAttrib = (tagSkillAttrib*)getAttrib();
	if( pAttrib && pAttrib->m_nMoveTime != -1 )
	{
		setHitDelay(getHitDelay() + DEFAULT_DELAY);/// 加上位移技能的默认延迟
		const tagSkillDef& SkillDef = getSkillDef();
		const SKILLRESULT* pstSkillResult = z_skill_get_result( &SkillDef, RESULT_SELF_POSCHG );
		if( pstSkillResult )
		{
			RESPOS		pos = circualMovePoint(pstSkillResult);
			setSelfPosChange( pos.X, pos.Y );
			if (getSingleTarget() == 0)
			{
				setTargetPoint(pos.X * 10, pos.Y * 10);
			}
		}
	}
	// 攻击计算
	onAttack();
	++m_nPilotNum;

	SkillManager::getSingleton().sendAttackMsg(this);
	// 设置下次循环调用法术
	const tagSkillDef & SkillDef = getSkillDef();
	const GameXYZ *pos = getTargetPoint();
	std::vector<int> nextskills;
	if (getSpellId() == 7001 || getSpellId() == 7002 || getSpellId() == 7003)//tag 暂时措施
	{
		SkillManager::getSingleton().setNextSpell(-1, m_preSelectTarget);
	}
	else
	{
		if (SkillDef.NextSkill != 0)
		{
			nextskills.push_back(SkillDef.NextSkill);
		}
		if (SkillDef.NextSkill2 != 0)
		{
			nextskills.push_back(SkillDef.NextSkill2);
		}
		if (SkillDef.NextSkill3 != 0)
		{
			nextskills.push_back(SkillDef.NextSkill3);
		}
		if (nextskills.size() == 0)
		{
			SkillManager::getSingleton().setNextSpell();
		}
		else
		{
			int nextskill = nextskills[rand() % nextskills.size()];
			SkillManager::getSingleton().setNextSpell(nextskill, m_preSelectTarget, pos->x, pos->z, m_SkillID == nextskill ||
				DBDataManager::getSingleton().GetSkillDef(nextskill, SkillManager::getSingleton().getSpellLv(nextskill)).SkillType != SKILL_TYPE_NORMAL);
		}
	}
	if (getState() == STATE_PILOT)
	{
		if (m_nPilotNum >= SkillManager::getSingleton().getSkillPilotNum(getSpellId()))
		{
			SkillManager::getSingleton().spellBreak(this, getHitDelay() + 200);
		}
	}
}

void SkillRandom::selectNextTarget()
{
	size_t index = m_TargetVector.size();
	m_TargetVector.resize(index + 1);
	if (index > 0)
	{
		int n = 0;
		int target = m_TargetActorID;
		for( ACTOR_ITER iter  = ActorManager::getSingleton().getActorBegin(); 
			iter != ActorManager::getSingleton().getActorEnd(); ++iter)
		{
			// 过滤自己和目标
			if (iter->first == m_TargetActorID || iter->first == getOwner())
			{
				continue;
			}
			GameActor* pActor = iter->second;
			if( pActor->getType() == GAT_NPC ||			// 不能以NPC为目标
				pActor->getType() == GAT_ITEM ||		// 不能以Item为目标
				(pActor->getType() == GAT_MONSTER &&
				(pActor->getSubType() == MON_SUB_TYPE_COLLECT ||
				pActor->getSubType() == MON_SUB_TYPE_TRAP ||
				pActor->getSubType() == MON_SUB_TYPE_BUILD)))	// 过滤特殊物品
			{
				continue;
			}
			if (filter(pActor))
			{
				n = n + 1;
				if (rand() / ((double)RAND_MAX + 1) < 1.0 / n)
				{
					target = pActor->GetID();
				}
			}
		}
		if (target == m_TargetActorID)
		{
			GameActor *pActor = ActorManager::getSingleton().FindActor(target);
			if (pActor && filter(pActor))
			{
				m_TargetVector[index].push_back(target);
				m_TargetActorID = target;
			}
			else
			{
				Break();
			}
		}
		else
		{
			m_TargetVector[index].push_back(target);
			m_TargetActorID = target;
		}
	}
	else
	{
		m_TargetVector[index].push_back(m_TargetActorID);
	}
}
