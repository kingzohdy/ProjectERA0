#include "SkillTrig.h"
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
#include "GameSkill_Package.h"
#include <Windows.h>
#include <MMSystem.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace Ogre;
using namespace std;

struct tagSkill_Trig : public tagSkillAttrib
{
};

const char	*xml_trig_path[] = {"db/skill/trig.xml", NULL};

IMPLEMENT_SKILL(SkillTrig,tagSkill_Trig)

SkillTrig::SkillTrig(void)
{
}

SkillTrig::~SkillTrig(void)
{
}

bool SkillTrig::save()
{
	return false;
}

bool SkillTrig::load()
{
	XMLData xmldata;
	for (int i = 0; xml_trig_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_trig_path[i] ) )
		{
			LOG_SEVERE("load skilltrig error: %s", xml_trig_path[i]);
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
					tagSkill_Trig* p = new tagSkill_Trig;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					p->m_strType					= "SkillTrig";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillTrig::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_Trig* pp	= new tagSkill_Trig(*p);
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

int SkillTrig::getSingleTarget()
{
	if (getTargetNum() > 0)
	{
		return getTarget(0);
	}
	return getOwner();
}

bool SkillTrig::isAutoSkill()
{
	return true;
}

const GameXYZ * SkillTrig::getTargetPoint()
{
	return &m_TargetPoint;
}

void SkillTrig::setTargetPoint(const GameXYZ &pos)
{
	m_TargetPoint = pos;
}
