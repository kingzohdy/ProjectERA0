#include "SkillScan.h"
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

struct tagSkill_Scan : public tagSkillAttrib
{
	int m_nSpinSpeed;			// 旋转角速度
	int m_nDelay;
	int	m_nPreAct;				// 准备延迟动作
	std::string	m_strPreShow;	// 准备特效名字
};

const char	*xml_scan_path[] = {"db/skill/scan.xml", NULL};

IMPLEMENT_SKILL(SkillScan,tagSkill_Scan)

SkillScan::SkillScan(void)
{
	m_start = false;
	m_angle = 0;
	m_lastAttack = 0;
	m_lastTime = 0;
}

SkillScan::~SkillScan(void)
{
}

bool SkillScan::save()
{
	return false;
}

bool SkillScan::load()
{
	XMLData xmldata;
	for (int i = 0; xml_scan_path[i]; i++)
	{
		if( !xmldata.loadFile( xml_scan_path[i] ) )
		{
			LOG_SEVERE("load skillscan error: %s", xml_scan_path[i]);
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
					tagSkill_Scan* p = new tagSkill_Scan;

					GameSpell::parserSkillAttrib( child, p, SkillPropValue );
					if( child.attribToString("spinspeed") )
					{
						p->m_nSpinSpeed = child.attribToInt("spinspeed");
					}
					else
					{
						p->m_nSpinSpeed = 0;
					}
					if( child.attribToString("predelay") )
					{
						p->m_nDelay = child.attribToInt("predelay");
					}
					else
					{
						p->m_nDelay = 0;
					}
					if ( child.attribToString("predelayact") )
					{
						p->m_nPreAct = child.attribToInt("predelayact");
					}
					else
					{
						p->m_nPreAct = 0;
					}
					if ( child.attribToString("predelayeff") )
					{
						p->m_strPreShow = child.attribToString("predelayeff");
					}
					p->m_strType					= "SkillScan";
					m_SkillAttrib[p->m_nSkillId].push_back(p);

					// 将构造函数注册给管理器
					SkillManager::registerSpell( p->m_nSkillId, SkillScan::create, p );
					SkillManager::registerSpellAttrib( p->m_nSkillId, SkillPropValue );

					for (XMLNode grandson = child.iterateChild(); !grandson.isNull(); grandson = child.iterateChild(grandson))
					{
						tagSkill_Scan* pp	= new tagSkill_Scan(*p);
						GameSpell::parserSkillAttrib( grandson, pp, SkillPropValue );
						if( grandson.hasAttrib("buff") )
						{
							pp->m_nBuffId = grandson.attribToInt("buff");
						}
						if( grandson.hasAttrib("fitting") )
						{
							pp->m_nFittingId = grandson.attribToInt("fitting");
						}
						if( grandson.attribToString("spinspeed") )
						{
							pp->m_nSpinSpeed = grandson.attribToInt("spinspeed");
						}
						if( grandson.attribToString("predelay") )
						{
							pp->m_nDelay = grandson.attribToInt("predelay");
						}
						if ( grandson.attribToString("predelayact") )
						{
							pp->m_nPreAct = grandson.attribToInt("predelayact");
						}
						else
						{
							pp->m_nPreAct = 0;
						}
						if ( grandson.attribToString("predelayeff") )
						{
							pp->m_strPreShow = grandson.attribToString("predelayeff");
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

bool SkillScan::start()
{
	if (getOwner() == 0)
	{
		assert(ActorManager::getSingleton().getMainPlayer());
		setOwner(ActorManager::getSingleton().getMainPlayer()->GetID());
	}
	GameActor *pActor = ActorManager::getSingleton().FindActor(getOwner());
	if (pActor != NULL)
	{
		GameXYZ selfPos = pActor->GetPosition();
		selfPos.x += (int)(50 * Cos((float)pActor->getAngle()));
		selfPos.z -= (int)(50 * Sin((float)pActor->getAngle()));
		setTargetPoint(selfPos);
	}
	return GameSpell::start();
}

int SkillScan::getSingleTarget()
{
	return getOwner();
}

void SkillScan::onPrepare()
{
	GameActor *pSrcObj		= ActorManager::getSingleton().FindActor(getOwner());
	if( pSrcObj == NULL )
	{
		Break();
		return;
	}
	tagSkill_Scan* pAttrib = (tagSkill_Scan*)getAttrib();
	int during = pSrcObj->getPrepareTime() - pAttrib->m_nDelay;
	bool prepareing = false;
	m_lastTime = timeGetTime();
	if (during < 0)
	{
		m_lastTime -= during;
		during = 0;
		prepareing = true;
	}
	GameXYZ pos = *(getTargetPoint());
	GameXYZ selfPos = pSrcObj->GetPosition();
	double angle = atan2((double)selfPos.z - pos.z, pos.x - selfPos.x) * 180 / M_PI;
	int spinspeed = pAttrib->m_nSpinSpeed;
	angle += spinspeed * during * 0.01;
	m_angle = angle;
	pSrcObj->SetFaceTo(int(m_angle));
	setTargetPoint(selfPos);
	if (prepareing)
	{
		if( !pAttrib->m_strPreShow.empty() )
		{
			m_pSpellEffName[0]	= pAttrib->m_strPreShow.c_str();
			m_pSpellEffCount	= 1;
		}
		else
		{
			m_pSpellEffCount	= 0;
		}
		// 播放准备特效
		playAction( m_OwnerID, pAttrib->m_nPreAct);
		pSrcObj->setPrepareSkill(getSpellId(), getSpellLv(), getFittingIndex(), m_lastTime - pAttrib->m_nDelay, pAttrib->m_nDelay);
	}
	else
	{
		m_start = true;
		GameSpell::onPrepare();
		pSrcObj->setPrepareSkill();
	}
}

void SkillScan::onUpdate(unsigned int dtick)
{
	GameActor *pSrcObj		= ActorManager::getSingleton().FindActor(getOwner());
	if( pSrcObj == NULL )
	{
		Break();
		return;
	}

	unsigned int curtime = timeGetTime();
	int during = curtime - m_lastTime;
	if (during < 0)
	{
		return;
	}
	if (!m_start)
	{
		m_start = true;
		GameSpell::onPrepare();
		pSrcObj->setPrepareSkill();
	}
	m_lastTime = curtime;
	tagSkill_Scan* pAttrib = (tagSkill_Scan*)getAttrib();
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef( getSpellId(), getSpellLv() );
	if (m_lastAttack + SkillDef.PilotTime * 100 <= curtime)
	{
		checkAttack();
	}
	m_angle += pAttrib->m_nSpinSpeed * during * 0.01;
	pSrcObj->SetFaceTo(int(m_angle));

	for (list<SKillObserver *>::iterator i = m_lstObserver.begin(); i != m_lstObserver.end(); ++i)
	{
		(*i)->onUpdate(this, curtime);
	}
}

void SkillScan::checkAttack()
{
	GameActor *pSrcObj		= ActorManager::getSingleton().FindActor(getOwner());
	GameActor *pMainPlayer	= ActorManager::getSingleton().getMainPlayer();
	if(pSrcObj == NULL || pMainPlayer == NULL)
	{
		Break();
		return;
	}

	bool result = false;
	const tagSkillDef &SkillDef = DBDataManager::getSingleton().GetSkillDef( getSpellId(), getSpellLv() );
	const RESRANGE& MainRange = SkillDef.MainRange;
	switch( MainRange.RangeType )
	{
	case SKILL_RANGE_SIGLE:
		result = pMainPlayer->GetID() == getSingleTarget();
		break;
	case SKILL_RANGE_CIRCLE:
		{
			GameXYZ targetPos = pMainPlayer->GetPosition();
			result = IsPointInCircle( targetPos.x/10, targetPos.z/10,  m_TargetPoint.x/10, m_TargetPoint.z/10, MainRange.RangeVal1);
		}
		break;
	case SKILL_RANGE_RECT:
		{
			GameXYZ targetPos = pMainPlayer->GetPosition();
			GameXYZ selfPos = pSrcObj->GetPosition();
			result = IsPointInRectangle( targetPos.x / 10, targetPos.z / 10, selfPos.x / 10, selfPos.z / 10,
				MainRange.RangeVal2, MainRange.RangeVal1, int(m_angle));
		}
		break;
	case SKILL_RANGE_ARC:
		{
			GameXYZ targetPos = pMainPlayer->GetPosition();
			GameXYZ selfPos = pSrcObj->GetPosition();
			result = IsPointInFan( targetPos.x / 10, targetPos.z / 10, selfPos.x / 10, selfPos.z / 10, MainRange.RangeVal1,
				int(m_angle), MainRange.RangeVal2);
		}
		break;
	};

	if (result)
	{
		m_lastAttack = timeGetTime();
		tagCSPkgBody msgbody;
		msgbody.PrepareAtkInfo.PrepareAtkOp = PREPAREATK_OP_AKEE;
		msgbody.PrepareAtkInfo.PrepareAtkData.PrepareAtkByHit.SkillID = getSpellId();
		msgbody.PrepareAtkInfo.PrepareAtkData.PrepareAtkByHit.AtkerID = getOwner();
		SendGameServerMsg( PREPAREATK_CLT, msgbody );
	}
}

void SkillScan::setSkillMsgPackage(GameSkill_Package* Package)
{
	Package->updateTargetValue();
	Package->doPackage();
}

const GameXYZ * SkillScan::getTargetPoint()
{
	return &m_TargetPoint;
}

void SkillScan::setTargetPoint(const GameXYZ &pos)
{
	m_TargetPoint = pos;
}
