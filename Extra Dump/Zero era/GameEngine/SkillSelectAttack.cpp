#include "SkillSelectAttack.h"
#include "GameMsgPrint.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameStateAction.h"
#include "OgreXMLData.h"
#include "OgreStringUtil.h"
#include "ShowActor.h"
#include "OgreEntity.h"
#include "OgreLog.h"
#include "GameSkill_Mgr.h"
#include "GameMap.h"
#include "OgreWorldPos.h"
#include <Windows.h>
#include <MMSystem.h>

using namespace Ogre;
using namespace std;

struct tagSkill_SelectAttack : public tagSkillAttrib
{
	string m_strSelectAllowEff;			// 选择圈特效
	string m_strSelectUnAllowEff;		// 不可选特效
	bool m_isSpin;
};

const char	*xml_selectattack_path[] = {"db/skill/selectattack.xml", "db/skill/blow.xml", NULL};

IMPLEMENT_SKILL(SkillSelectAttack,tagSkill_SelectAttack)

SkillSelectAttack::SkillSelectAttack(void):m_nCatchTime(0), m_bSelected(false), m_bSelecting(false)
{
}

SkillSelectAttack::~SkillSelectAttack(void)
{
}

bool SkillSelectAttack::save()
{
	return false;
}

bool SkillSelectAttack::load()
{
	XMLData xmldata;
	for (int i = 0; xml_selectattack_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_selectattack_path[i] ) )
		{
			LOG_SEVERE("load skillselectattack error: %s", xml_selectattack_path[i]);
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
					ATTRIB_MAP				SkillPropValue;
					tagSkill_SelectAttack*	p	= new tagSkill_SelectAttack;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					if( child.attribToString("alloweff") )
					{
						p->m_strSelectAllowEff = child.attribToString("alloweff");
					}

					if( child.attribToString("unalloweff") )
					{
						p->m_strSelectUnAllowEff = child.attribToString("unalloweff");
					}

					if( child.attribToString("selecteffspin") )
					{
						p->m_isSpin = child.attribToBool("selecteffspin");
					}
					else
					{
						p->m_isSpin = false;
					}

					p->m_strType						= "SkillSelectAttack";

					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillSelectAttack::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_SelectAttack* pp	= new tagSkill_SelectAttack(*p);
						GameSpell::parserSkillAttrib( grandson, pp, SkillPropValue );
						if( grandson.hasAttrib("buff") )
						{
							pp->m_nBuffId = grandson.attribToInt("buff");
						}
						if( grandson.hasAttrib("fitting") )
						{
							pp->m_nFittingId = grandson.attribToInt("fitting");
						}
						if( grandson.attribToString("alloweff") )
						{
							pp->m_strSelectAllowEff = grandson.attribToString("alloweff");
						}
						if( grandson.attribToString("unalloweff") )
						{
							pp->m_strSelectUnAllowEff = grandson.attribToString("unalloweff");
						}
						m_SkillAttrib[p->m_nSkillId].push_back(pp);
					}

					child = node.iterateChild(child);
				}
				node = root.iterateChild(node);
			}else{
				return false;
			}
		}
	}
	return true;
}

bool SkillSelectAttack::start()
{
	tagSkill_SelectAttack* pAttrib = (tagSkill_SelectAttack*)getAttrib();

	if (!m_bSelected)
	{
		if (m_bSelecting)
		{
			return true;
		}
		int nDis = SkillManager::getSingleton().getSkillDistance(getSpellId());
		if (nDis <= 0 || pAttrib->m_strSelectAllowEff.empty())
		{
			assert(ActorManager::getSingleton().getMainPlayer());
			setTargetPoint(ActorManager::getSingleton().getMainPlayer()->GetPosition());
		}
		else
		{
			m_bSelecting = true;
			GameMsgPrint::getSingleton().showSelectEffect(pAttrib->m_strSelectAllowEff.c_str(), 
				pAttrib->m_strSelectUnAllowEff.c_str(), nDis, SkillManager::getSingleton().getSkillMainRange(getSpellId()).RangeVal1, pAttrib->m_isSpin);
			return true;
		}
	}
	return GameSpell::start();
}

bool SkillSelectAttack::Attack()
{
	/// 检测魔法阻挡层
	const tagSkillDef& SkillDef = DBDataManager::getSingleton().GetSkillDef( getSpellId(), getSpellLv() );
	GameActor *pSrcObj = ActorManager::getSingleton().FindActor(getOwner());
	if( !((SkillDef.NoSkillStop & NOSKILLSTOP_TYPE_MAGIC) ||
		pSrcObj->getCurMap()->getMaskMap()->lineOfSightTest( 
		Ogre::Point2D( pSrcObj->GetPosition().x/10, pSrcObj->GetPosition().z/10 ),
		Ogre::Point2D( getTargetPoint()->x/10, getTargetPoint()->z/10 ), MASK_MAGIC | DYN_MASK_MAGIC ) ))
	{
		m_SkillState = STATE_END;
		return false;
	}
	if ((SkillDef.NoSkillStop & NOSKILLSTOP_TYPE_MOVE) && !pSrcObj->getCurMap()->getMaskMap()->canWalkPixelCoord(getTargetPoint()->x/10, getTargetPoint()->z/10))
	{
		m_SkillState = STATE_END;
		return false;
	}
	return GameSpell::Attack();
}

void SkillSelectAttack::setTargetPoint( const GameXYZ &pos )
{
	m_bSelected = true;
	m_TargetPoint = pos;
}

void SkillSelectAttack::onAttack(bool passive/* = false*/)
{
	GameActor *pSrcObj	= ActorManager::getSingleton().FindActor(getOwner());
	if (pSrcObj && !passive)
	{
		// 施法者朝向施法点
		pSrcObj->SetFaceTo( m_selfPos.x/10, 
			m_selfPos.z/10, 
			m_TargetPoint.x/10,
			m_TargetPoint.z/10, true );
	}
	GameSpell::onAttack(passive);
}

const GameXYZ * SkillSelectAttack::getTargetPoint()
{
	if (!m_bSelected)
	{
		return NULL;
	}
	return &m_TargetPoint;
}

int SkillSelectAttack::inspect(int x, int y)
{
	if (x == 0 && y == 0)
	{
		return RET_SUCCESS;
	}
	GameActor *pMainPlayer = ActorManager::getSingleton().getMainPlayer();
	GameXYZ selfPos = pMainPlayer->GetPosition();
	GameXYZ targetPos = selfPos;
	targetPos.x = x;
	targetPos.z = y;
	if (SkillManager::getSingleton().getSkillDistance(getSpellId()) < selfPos.subtract(targetPos).length())
	{
		return RET_OUTRANGE;
	}
	const tagSkillDef& SkillDef = getSkillDef();
	if ((SkillDef.NoSkillStop & NOSKILLSTOP_TYPE_MOVE) && !pMainPlayer->getCurMap()->getMaskMap()->canWalkPixelCoord(x/10, y/10))
	{
		return RET_MASKPOINT;
	}
	if (!(SkillDef.NoSkillStop & NOSKILLSTOP_TYPE_MAGIC) &&
		!pMainPlayer->getCurMap()->getMaskMap()->lineOfSightTest( 
		Ogre::Point2D(pMainPlayer->GetPosition().x/10, pMainPlayer->GetPosition().z/10),
		Ogre::Point2D(x/10, y/10), GameMaskMap::maskMagic))
	{
		return RET_MASKPOINT;
	}
	if (!pMainPlayer->getCurMap()->getMaskMap()->lineOfSightTest( 
		Ogre::Point2D(pMainPlayer->GetPosition().x/10, pMainPlayer->GetPosition().z/10),
		Ogre::Point2D(x/10, y/10), GameMaskMap::maskLoMagic, SkillDef.ThroughLoMagic))
	{
		return RET_MASKPOINT;
	}
	return RET_SUCCESS;
}
