#include "SkillGenera.h"
#include "GameSkill_Mgr.h"
#include "OgreXMLData.h"
#include "OgreLog.h"
#include "GameDBData.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameMsgPrint.h"
#include "OgreStringUtil.h"
#include "OgreEntity.h"
#include "ShowActor.h"
#include <Windows.h>
#include <MMSystem.h>

using namespace Ogre;
using namespace std;

struct tagSkill_Genera : public tagSkillAttrib
{
};

const char	*xml_genera_path[] = {"db/skill/genera.xml", "db/skill/moveattack.xml", "db/skill/link.xml", NULL};

IMPLEMENT_SKILL(SkillGenera,tagSkill_Genera)

SkillGenera::SkillGenera(void)
{
}

SkillGenera::~SkillGenera(void)
{
}

bool SkillGenera::save()
{
	return false;
}

bool SkillGenera::load()
{
	XMLData xmldata;
	for (int i = 0; xml_genera_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_genera_path[i] ) )
		{
			LOG_SEVERE("load skillgenera error: %s", xml_genera_path[i]);
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
					tagSkill_Genera* p = new tagSkill_Genera;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					p->m_strType					= "SkillGenera";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillGenera::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_Genera* pp	= new tagSkill_Genera(*p);
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
			}else{
				return false;
			}
		}
	}
	return true;
}

bool SkillGenera::start()
{
	if (getOwner() == 0)
	{
		assert(ActorManager::getSingleton().getMainPlayer());
		setOwner(ActorManager::getSingleton().getMainPlayer()->GetID());
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(getOwner());
	if (pActor != NULL)
	{
		setTargetPoint(pActor->GetPosition());
	}
	return GameSpell::start();
}

int SkillGenera::getSingleTarget()
{
	return getOwner();
}

const GameXYZ * SkillGenera::getTargetPoint()
{
	GameActor *pActor = ActorManager::getSingleton().FindActor(getOwner());
	if (pActor)
	{
		m_TargetPoint = pActor->GetPosition();
	}
	return &m_TargetPoint;
}
