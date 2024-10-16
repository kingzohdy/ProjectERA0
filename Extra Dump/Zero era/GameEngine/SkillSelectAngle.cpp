#include "SkillSelectAngle.h"
#include "GameSkill_Mgr.h"
#include "GameActor.h"
#include "GameMainPlayer.h"
#include "GameMaskMap.h"
#include "OgreXMLData.h"
#include "OgreLog.h"
#include "GameDBData.h"
#include "OgreStringUtil.h"
#include "GameMap.h"
#include "ShowActor.h"
#include "OgreEntity.h"
#include "GameMsgPrint.h"
#include <string>
#include <Windows.h>
#include <MMSystem.h>

using namespace Ogre;
using namespace std;

struct tagSkill_SelectAngle : public tagSkillAttrib
{
	string m_strSelectAllowEff;			// 选择圈特效
	string m_strSelectUnAllowEff;		// 不可选特效
};

const char *xml_angle_path[] = {"db/skill/angle.xml", NULL};

IMPLEMENT_SKILL(SkillSelectAngle,tagSkill_SelectAngle)

SkillSelectAngle::SkillSelectAngle(void):m_bSelected(false), m_bSelecting(false)
{
}

SkillSelectAngle::~SkillSelectAngle(void)
{
}

bool SkillSelectAngle::save()
{
	return false;
}

bool SkillSelectAngle::load()
{
	XMLData xmldata;
	for (int i = 0; xml_angle_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_angle_path[i] ) )
		{
			LOG_SEVERE("load skillselectangle error: %s", xml_angle_path[i]);
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
					ATTRIB_MAP			SkillPropValue;
					tagSkill_SelectAngle*  p = new tagSkill_SelectAngle;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					if( child.attribToString("alloweff") )
					{
						p->m_strSelectAllowEff = child.attribToString("alloweff");
					}

					if( child.attribToString("unalloweff") )
					{
						p->m_strSelectUnAllowEff = child.attribToString("unalloweff");
					}
					p->m_strType				 = "SkillCircularAttack";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillSelectAngle::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_SelectAngle* pp	= new tagSkill_SelectAngle(*p);
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
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

bool SkillSelectAngle::start()
{
	tagSkill_SelectAngle* pAttrib = (tagSkill_SelectAngle*)getAttrib();

	if (!m_bSelected)
	{
		if (m_bSelecting)
		{
			return true;
		}
		const tagSkillDef &SkillDef = getSkillDef();
		if (SkillDef.PrepareType == PREPARE_TYPE_COLLECT || pAttrib->m_strSelectAllowEff.empty())
		{
			GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
			assert(pActor);
			GameActor *pSelect = ActorManager::getSingleton().FindActor(pActor->GetSelectTarget());
			if (pSelect == NULL)
			{
				GameXYZ selfPos = pActor->GetPosition();
				selfPos.x += (int)(15000 * Cos((float)pActor->getAngle()));
				selfPos.z -= (int)(15000 * Sin((float)pActor->getAngle()));
				setTargetPoint(selfPos);
			}
			else
			{
				setTargetPoint(pSelect->GetPosition());
			}
		}
		else
		{
			m_bSelecting = true;
			const char *path = pAttrib->m_strSelectAllowEff.c_str();
			const char *pdot = strrchr(path, '.');
			if (pdot && stricmp(pdot, ".ent") == 0)
			{
				GameMsgPrint::getSingleton().showAdjustEffect(path, 
					pAttrib->m_strSelectUnAllowEff.c_str(), NULL );
			}
			else
			{
				unsigned int w = 0, h = 0;
				TextureData *tex = new TextureData();
				if(tex->loadFromImageFile(pAttrib->m_strSelectAllowEff.c_str()))
				{
					Ogre::TextureDesc texDesc;
					tex->getDesc(texDesc);
					w = texDesc.width;
					h = texDesc.height;
				}
				tex->release();
				RectEffectData data;
				data.limit = SkillDef.MainRange.RangeVal2;
				GameMsgPrint::getSingleton().showRectEffect(path, 
					SkillDef.MainRange.RangeVal1, &data);
			}
			return true;
		}
	}
	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	assert(pActor);
	GameXYZ selfPos = pActor->GetPosition();
	GameXYZ targetPos = *(getTargetPoint());
	pActor->SetFaceTo(FaceAngleBetween(selfPos.x, selfPos.z, targetPos.x, targetPos.z), true);
	setTargetPoint(targetPos);
	return GameSpell::start();
}

void SkillSelectAngle::setTargetPoint( const GameXYZ &pos )
{
	m_bSelected = true;
	m_TargetPoint = pos;
	GameActor *pActor = ActorManager::getSingleton().FindActor(getOwner());
	if (pActor == NULL || pActor != ActorManager::getSingleton().getMainPlayer())
	{
		return;
	}
	GameXYZ selfPos = pActor->GetPosition();
	int angle = pActor->getAngle();
	int distance = SkillManager::getSingleton().getSkillDistance(getSpellId(), getSpellLv());
	m_TargetPoint = selfPos;
	m_TargetPoint.x += (int)(distance * 8 * Cos((float)angle) + 0.5);
	m_TargetPoint.z -= (int)(distance * 8 * Sin((float)angle) + 0.5);
	Ogre::Point2D result;
	result.x =	m_TargetPoint.x / 10;
	result.y =	m_TargetPoint.z / 10;
	const tagSkillDef &SkillDef = getSkillDef();
	if (!(SkillDef.NoSkillStop & NOSKILLSTOP_TYPE_MAGIC))
	{
		pActor->getCurMap()->getMaskMap()->lineOfSightTest(
			Ogre::Point2D( selfPos.x/10, selfPos.z/10 ), result, GameMaskMap::maskMagic, 0, &result);
	}
	!pActor->getCurMap()->getMaskMap()->lineOfSightTest( 
		Ogre::Point2D( selfPos.x/10, selfPos.z/10 ), result, GameMaskMap::maskLoMagic, SkillDef.ThroughLoMagic, &result);
	m_TargetPoint.x = result.x * 10;
	m_TargetPoint.z = result.y * 10;
}

void SkillSelectAngle::onAttack(bool passive/* = false*/)
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

const GameXYZ * SkillSelectAngle::getTargetPoint()
{
	if (!m_bSelected)
	{
		return NULL;
	}
	return &m_TargetPoint;
}

bool SkillSelectAngle::collect()
{
	if (!GameSpell::collect())
	{
		return false;
	}
	const tagSkillDef &SkillDef = getSkillDef();
	tagSkill_SelectAngle* pAttrib = (tagSkill_SelectAngle*)getAttrib();
	const char *path = pAttrib->m_strSelectAllowEff.c_str();
	const char *pdot = strrchr(path, '.');
	if (pdot && stricmp(pdot, ".ent") == 0)
	{
		GameMsgPrint::getSingleton().showAdjustEffect(path, 
			pAttrib->m_strSelectUnAllowEff.c_str(), NULL );
	}
	else
	{
		unsigned int w = 0, h = 0;
		TextureData *tex = new TextureData();
		if(tex->loadFromImageFile(pAttrib->m_strSelectAllowEff.c_str()))
		{
			Ogre::TextureDesc texDesc;
			tex->getDesc(texDesc);
			w = texDesc.width;
			h = texDesc.height;
		}
		tex->release();
		RectEffectData data;
		data.limit = SkillDef.MainRange.RangeVal2;
		GameMsgPrint::getSingleton().showRectEffect(path, 
			SkillDef.MainRange.RangeVal1, &data);
	}
	SkillManager::getSingleton().setReadySpell(this);
	return true;
}

bool SkillSelectAngle::Attack()
{
	GameMsgPrint::getSingleton().hideSelectEffect();
	SkillManager::getSingleton().setReadySpell();
	return GameSpell::Attack();
}

void SkillSelectAngle::onBreak()
{
	if (ActorManager::getSingleton().FindActor(getOwner()) == ActorManager::getSingleton().getMainPlayer())
	{
		GameMsgPrint::getSingleton().hideSelectEffect();
		SkillManager::getSingleton().setReadySpell();
	}
	GameSpell::onBreak();
}

void SkillSelectAngle::onStop()
{
	if (ActorManager::getSingleton().FindActor(getOwner()) == ActorManager::getSingleton().getMainPlayer())
	{
		SkillManager::getSingleton().setTurnSpell();
	}
	GameSpell::onStop();
}

void SkillSelectAngle::onChannel()
{
	if (ActorManager::getSingleton().FindActor(getOwner()) == ActorManager::getSingleton().getMainPlayer())
	{
		SkillManager::getSingleton().setTurnSpell(this);
	}
	GameSpell::onChannel();
}
