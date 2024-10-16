#include "GameSkill_Hit.h"
#include "GameSkill_Package.h"
#include "HitDelay.h"
#include "HitThrow.h"
#include "GameSkill_Common.h"
#include "GameSkill_Mgr.h"

#define ENROLL_HITCREATOR(creator, tag)	\
	TagCreator::iterator i = enroller().find(#tag);\
	if (i != enroller().end())\
	{\
		return false;\
	}\
	enroller()[#tag] = creator;

SkillHit::SkillHit(GameSpell *p) : isActive(true), whichSpell(p), package(NULL), isEmpty(true)
{
	SkillManager::getSingleton().hitArray.insert(this);
}

SkillHit::~SkillHit(void)
{
	if (SkillManager::getSingletonPtr())
	{
		std::set<SkillHit *>::iterator i = SkillManager::getSingleton().hitArray.find(this);
		if (i != SkillManager::getSingleton().hitArray.end())
		{
			SkillManager::getSingleton().hitArray.erase(i);
		}
	}
}

void SkillHit::hit()
{
	if (isEmpty)
	{
		whichSpell->onHit();
	}
	else
	{
		for (IdList::iterator i = targets.begin(); i != targets.end(); ++i)
		{
			whichSpell->onHit(*i, package);
			if (package == NULL)
			{
				waitTargets.push_back(*i);
			}
		}
		targets.clear();
	}
	package = NULL;
}

void SkillHit::hit(int id)
{
	for (IdList::iterator i = targets.begin(); i != targets.end(); ++i)
	{
		if ((*i) == id)
		{
			targets.erase(i);
			whichSpell->onHit(id, package);
			if (package == NULL)
			{
				waitTargets.push_back(id);
			}
			break;
		}
	}
}

void SkillHit::setSkillPackage(GameSkill_Package *p)
{
	if (actived())
	{
		package = p;
	}
	else
	{
		for (IdList::iterator i = targets.begin(); i != targets.end(); ++i)
		{
			p->doPackage(*i);
		}
		for (IdList::iterator i = waitTargets.begin(); i != waitTargets.end(); ++i)
		{
			p->doPackage(*i);
		}
	}
}

void SkillHit::flush()
{
	if (package)
	{
		package->doPackage();
	}
}

void SkillHit::stop()
{
	isActive = false;
}

int SkillHit::first()
{
	if (targets.empty())
	{
		return 0;
	}
	return *(targets.begin());
}

int SkillHit::next(int id)
{
	IdList::iterator i = targets.begin();
	for (; i != targets.end(); ++i)
	{
		if ((*i) == id)
		{
			break;
		}
	}
	if (i == targets.end())
	{
		return 0;
	}
	if (++i == targets.end())
	{
		return 0;
	}
	return *i;
}

void SkillHit::addTarget(int id)
{
	isEmpty = false;
	targets.push_back(id);
}

bool SkillHit::complete()
{
	return targets.empty();
}

SkillHit::TagCreator & SkillHit::enroller()
{
	static TagCreator ret;

	return ret;
}

static std::list<SkillHit *> createThrow(GameSpell *spell, const std::vector<int> &targets);
SkillHit::HitsCreator * SkillHit::findCreator(GameSpell *spell)
{
	const tagSkillAttrib *attrib = (tagSkillAttrib *)spell->getAttrib();
	if (attrib->m_strFlyActorName.empty())
	{
		return createDefault;
	}
	else
		return createThrow;
	//////////////////////////////////////////////////////////////////////////
	TagCreator::iterator i = enroller().find("");
	if (i != enroller().end())
	{
		return (HitsCreator *)(i->second);
	}
	return createDefault;
}

std::list<SkillHit *> SkillHit::createDefault(GameSpell *spell, const std::vector<int> &targets)
{
	std::list<SkillHit *>	hits;
	HitDelay *hit = new HitDelay(spell);
	hits.push_back(hit);

	for (size_t i = 0; i < targets.size(); ++i)
	{
		hit->addTarget(targets[i]);
	}
	hit->setDelay(0);

	return hits;
}

//////////////////////////////////////////////////////////////////////////

static std::list<SkillHit *> createThrow(GameSpell *spell, const std::vector<int> &targets)
{
	std::list<SkillHit *>	hits;
	const tagSkillAttrib *attrib = (tagSkillAttrib *)spell->getAttrib();
	if (attrib->m_bFlyMulti)
	{
		for (size_t i = 0; i < targets.size(); ++i)
		{
			HitThrow *hit = new HitThrow(spell);
			hits.push_back(hit);
			hit->addTarget(targets[i]);
			hit->start(attrib->m_strFlyActorName.c_str(), attrib->m_nFlyActorNum, attrib->m_nFlyActorDelay);
		}
	}
	else
	{
		HitThrow *hit = new HitThrow(spell);
		hits.push_back(hit);

		for (size_t i = 0; i < targets.size(); ++i)
		{
			hit->addTarget(targets[i]);
		}
		hit->start(attrib->m_strFlyActorName.c_str(), attrib->m_nFlyActorNum, attrib->m_nFlyActorDelay);
	}

	return hits;
}

bool SkillHit::init()
{
	//ENROLL_HITCREATOR(creator, tag)
	return true;
}
