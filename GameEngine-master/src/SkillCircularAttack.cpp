#include "SkillCircularAttack.h"
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
#include "OgreTexture.h"
#include <string>
#include <Windows.h>
#include <MMSystem.h>

using namespace Ogre;
using namespace std;

struct tagSkill_SelectArrow : public tagSkillAttrib
{
	string			m_strSelectAllowEff;			// 选择圈特效
	unsigned int	m_nLimit;		// 不可选特效
};

const char *xml_circular_path[] = {"db/skill/circular.xml", NULL};

IMPLEMENT_SKILL(SkillSelectArrow,tagSkill_SelectArrow)

SkillSelectArrow::SkillSelectArrow(void):m_bSelected(false), m_bSelecting(false)
{
}

SkillSelectArrow::~SkillSelectArrow(void)
{
}

bool SkillSelectArrow::save()
{
	return false;
}

bool SkillSelectArrow::load()
{
	XMLData xmldata;
	for (int i = 0; xml_circular_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_circular_path[i] ) )
		{
			LOG_SEVERE("load skillcircular error: %s", xml_circular_path[i]);
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
					tagSkill_SelectArrow*  p = new tagSkill_SelectArrow;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					if( child.hasAttrib("alloweff") )
					{
						p->m_strSelectAllowEff = child.attribToString("alloweff");
					}

					if( child.hasAttrib("efflimit") )
					{
						p->m_nLimit = child.attribToInt("efflimit");
					}
					else
					{
						p->m_nLimit = 0;
					}
					p->m_strType				 = "SkillCircularAttack";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillSelectArrow::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_SelectArrow* pp	= new tagSkill_SelectArrow(*p);
						GameSpell::parserSkillAttrib( grandson, pp, SkillPropValue );
						if( grandson.hasAttrib("buff") )
						{
							pp->m_nBuffId = grandson.attribToInt("buff");
						}
						if( grandson.hasAttrib("fitting") )
						{
							pp->m_nFittingId = grandson.attribToInt("fitting");
						}
						if( grandson.hasAttrib("alloweff") )
						{
							pp->m_strSelectAllowEff = grandson.attribToString("alloweff");
						}
						if( grandson.hasAttrib("efflimit") )
						{
							pp->m_nLimit = grandson.attribToInt("efflimit");
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

bool SkillSelectArrow::start()
{
	tagSkill_SelectArrow* pAttrib = (tagSkill_SelectArrow*)getAttrib();

	if (!m_bSelected)
	{
		if (m_bSelecting)
		{
			return true;
		}

		if (pAttrib->m_strSelectAllowEff.empty())
		{
			GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
			assert(pActor);
			GameXYZ selfPos = pActor->GetPosition();
			const tagSkillDyAttrib* pDyAttrib	= getSkillAttrib();
			int nDis = SkillManager::getSingleton().getSkillDistance(getSpellId()) * 10;
			selfPos.x += (int)(nDis * Cos((float)pActor->getAngle()));
			selfPos.z -= (int)(nDis * Sin((float)pActor->getAngle()));
			setTargetPoint(selfPos);
		}
		else
		{
			m_bSelecting = true;
			unsigned int w = 0, h = 0;
			if (pAttrib->m_nLimit != 0)
			{
				w = pAttrib->m_nLimit;
			}
			else
			{
				TextureData *tex = new TextureData();
				if(tex->loadFromImageFile(pAttrib->m_strSelectAllowEff.c_str()))
				{
					Ogre::TextureDesc texDesc;
					tex->getDesc(texDesc);
					w = texDesc.width;
					h = texDesc.height;
				}
				tex->release();

				unsigned int range	= SkillManager::getSingleton().getSkillMainRange(getSpellId()).RangeVal1 * 2;
				w = range * w / h;
				h = range;
			}
			GameMsgPrint::getSingleton().showCircularEffect( pAttrib->m_strSelectAllowEff.c_str(), 
				w, (void *)h );
			return true;
		}
	}
	GameActor *pActor = ActorManager::getSingleton().getMainPlayer();
	assert(pActor);
	GameXYZ selfPos = pActor->GetPosition();
	GameXYZ targetPos = *(getTargetPoint());
	pActor->SetFaceTo(FaceAngleBetween(selfPos.x, selfPos.z, targetPos.x, targetPos.z));
	return GameSpell::start();
}

void SkillSelectArrow::setTargetPoint( const GameXYZ &pos )
{
	m_bSelected = true;
	if (getOwner() != ActorManager::getSingleton().getMainPlayer()->GetID())
	{
		m_TargetPoint = pos;
	}
	else
	{
		GameXYZ targetPos = pos;
		GameActor *pSrc = ActorManager::getSingleton().FindActor(getOwner());
		GameXYZ selfPos = pSrc->GetPosition();
		int iDir = FaceAngleBetween( selfPos.x/10, selfPos.z/10, targetPos.x/10, targetPos.z/10 );
		int nDis = (int)selfPos.subtract( targetPos ).length();
		int maxDis = SkillManager::getSingleton().getSkillDistance(getSpellId());
		if (nDis > maxDis)
		{
			nDis = maxDis;
			targetPos.x		=	(int) (selfPos.x + (double)nDis * 10 * cos((double)(iDir)*3.14159265/180.0f));
			targetPos.z		=	(int) (selfPos.z - (double)nDis * 10 * sin((double)(iDir)*3.14159265/180.0f));
		}
		Ogre::Point2D result;
		result.x =	targetPos.x / 10;
		result.y =	targetPos.z / 10;
		const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef( getSpellId(), getSpellLv() );
		if (!(SkillDef.NoSkillStop & NOSKILLSTOP_TYPE_MAGIC))
		{
			pSrc->getCurMap()->getMaskMap()->lineOfSightTest(
				Ogre::Point2D( selfPos.x/10, selfPos.z/10 ), result, GameMaskMap::maskMagic, 0, &result);
		}
		!pSrc->getCurMap()->getMaskMap()->lineOfSightTest( 
			Ogre::Point2D( selfPos.x/10, selfPos.z/10 ), result, GameMaskMap::maskLoMagic, SkillDef.ThroughLoMagic, &result);
		targetPos.x = result.x * 10;
		targetPos.z = result.y * 10;
		m_TargetPoint = targetPos;
	}
}

float SkillSelectArrow::getAttackDist()
{
	return 0;
}

void SkillSelectArrow::onAttack(bool passive/* = false*/)
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

const GameXYZ * SkillSelectArrow::getTargetPoint()
{
	if (!m_bSelected)
	{
		return NULL;
	}
	return &m_TargetPoint;
}
