#include "SkillVigor.h"
#include "GameSkill_Mgr.h"
#include "OgreXMLData.h"
#include "OgreLog.h"
#include "GameDBData.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "OgreEntity.h"
#include "ShowActor.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "GameMsgPrint.h"

using namespace Ogre;
using namespace std;

struct tagSkill_Vigor : public tagSkillAttrib
{
	string m_strSelectAllowEff;					// 选择圈特效（固定方向）
	string m_strSelectChangeAllowEff;			// 选择圈特效（鼠标方向）
};

const char	*xml_vigor_path[] = {"db/skill/vigor.xml", NULL};

IMPLEMENT_SKILL(SkillVigor,tagSkill_Vigor)

SkillVigor::SkillVigor(void) : m_pSelectEff(NULL)
{
}

SkillVigor::~SkillVigor(void)
{
	if( m_pSelectEff != NULL )
	{
		m_pSelectEff->detachFromScene();
		OGRE_RELEASE(m_pSelectEff);
	}
}

bool SkillVigor::save()
{
	return false;
}

bool SkillVigor::load()
{
	XMLData xmldata;
	for (int i = 0; xml_vigor_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_vigor_path[i] ) )
		{
			LOG_SEVERE("load skillvigor error: %s", xml_vigor_path[i]);
			continue;
		}

		XMLNode	root = xmldata.getRootNode();
		XMLNode	node = root.iterateChild();
		while(!node.isNull())
		{	
			if( strcmp( node.getName(), "skills" ) == 0 )
			{
				XMLNode child = node.iterateChild();
				while(!child.isNull())
				{
					ATTRIB_MAP		 SkillPropValue;
					tagSkill_Vigor* p = new tagSkill_Vigor;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					if( child.attribToString("mousealloweff") )
					{
						p->m_strSelectChangeAllowEff = child.attribToString("mousealloweff");
					}
					if( child.attribToString("alloweff") )
					{
						p->m_strSelectAllowEff = child.attribToString("alloweff");
					}
					
					p->m_strType					= "SkillVigor";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillVigor::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_Vigor* pp	= new tagSkill_Vigor(*p);
						GameSpell::parserSkillAttrib( grandson, pp, SkillPropValue );
						if( grandson.hasAttrib("buff") )
						{
							pp->m_nBuffId = grandson.attribToInt("buff");
						}
						if( grandson.hasAttrib("fitting") )
						{
							pp->m_nFittingId = grandson.attribToInt("fitting");
						}
						if( grandson.attribToString("mousealloweff") )
						{
							pp->m_strSelectChangeAllowEff = grandson.attribToString("mousealloweff");
						}
						if( grandson.attribToString("alloweff") )
						{
							pp->m_strSelectAllowEff = grandson.attribToString("alloweff");
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

bool SkillVigor::start()
{
	GameActor *pOwner = ActorManager::getSingleton().FindActor(getOwner());
	if (pOwner == NULL)
	{
		return false;
	}
	setTargetPoint(pOwner->GetPosition());
	return GameSpell::start();
}

void SkillVigor::setTargetPoint(const GameXYZ &pos)
{
	m_TargetPoint = pos;
	if (m_pSelectEff)
	{
		m_pSelectEff->setPosition(pos);
	}
}

bool SkillVigor::collect()
{
	if (!GameSpell::collect())
	{
		return false;
	}

	GameActor *pOwner = ActorManager::getSingleton().FindActor(getOwner());
	if (pOwner == NULL)
	{
		return false;
	}
	GameMap *map = pOwner->getCurMap();
	if (map == NULL)
	{
		return false;
	}

	cumulateTime = 0;

	tagSkill_Vigor* pAttrib = (tagSkill_Vigor*)getAttrib();
	const char *path = pAttrib->m_strSelectChangeAllowEff.c_str();

	if ( pAttrib->m_strSelectChangeAllowEff.empty() )
	{
		path = pAttrib->m_strSelectAllowEff.c_str();
	}

	if( m_pSelectEff != NULL )
	{
		m_pSelectEff->detachFromScene();
		OGRE_RELEASE(m_pSelectEff);
	}
	m_pSelectEff = new Ogre::Entity;
	m_pSelectEff->load(path, false );
	m_pSelectEff->update(0);
	m_pSelectEff->attachToScene(map->getShowGameMap()->getGameScene());

	SkillManager::getSingleton().setAngleSpell(this);
	return true;
}

void SkillVigor::endCollect()
{
	if( m_pSelectEff != NULL )
	{
		m_pSelectEff->detachFromScene();
		OGRE_RELEASE(m_pSelectEff);
	}
	GameSpell::endCollect();
}

bool SkillVigor::Attack()
{
	SkillManager::getSingleton().setAngleSpell();
	return GameSpell::Attack();
}

void SkillVigor::onAttack(bool passive/* = false*/)
{
	GameActor *pSrcObj	= ActorManager::getSingleton().FindActor( getOwner() );
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

void SkillVigor::update(unsigned int dtick)
{
	tagSkill_Vigor* pAttrib = (tagSkill_Vigor*)getAttrib();
	GameActor *pOwner = ActorManager::getSingleton().FindActor(getOwner());
	if (pOwner == NULL)
	{
		Break();
		return;
	}
	cumulateTime += dtick;
	const tagSkillDef &SkillDef = getSkillDef();
	unsigned int totalTime = SkillManager::getSingleton().getSkillCollectTime(getSpellId(), getSpellLv()) * SkillManager::getSingleton().getSkillCollectNum(getSpellId(), getSpellLv()) * 100;
	if (cumulateTime > totalTime)
	{
		cumulateTime = totalTime;
	}
	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	if ( pAttrib->m_strSelectChangeAllowEff.empty() || pActor != pOwner )
	{
		int length = int(cumulateTime / 1000.0 * SkillDef.Distance * 10 + 0.5);
		int angle = pOwner->getAngle();
		GameXYZ pos = pOwner->GetPosition();
		pos.x += length * Cos(angle);
		pos.z -= length * Sin(angle);
		setTargetPoint(pos);
	}else{
		GameMsgPrint::getSingleton().showAngleEffect( pAttrib->m_strSelectChangeAllowEff.c_str(),cumulateTime,
					SkillDef.Distance);
	}
	if (m_pSelectEff)
	{
		m_pSelectEff->update(dtick);
	}
	GameSpell::update(dtick);
}

void SkillVigor::onStop()
{
	if( m_pSelectEff != NULL )
	{
		m_pSelectEff->detachFromScene();
		OGRE_RELEASE(m_pSelectEff);
	}
	GameSpell::onStop();
}
