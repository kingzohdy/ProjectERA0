#pragma once
#include "OgreMemoryDelegation.h"
#include "SmartPtr.h"
#include "GameSkill_Base.h"
#include <list>
#include <set>
#include <map>

class SkillHit : public Ogre::MemoryDelegation
{
	friend class GameSpell;
	typedef std::map<std::string, void *>	TagCreator;
	typedef std::list<SkillHit *> HitsCreator(GameSpell *spell, const std::vector<int> &targets);

	SkillHit(const SkillHit &other);
	SkillHit & operator = (const SkillHit &other);

private:
	static TagCreator & enroller();

public:
	static std::list<SkillHit *>	createDefault(GameSpell *spell, const std::vector<int> &targets);
	static HitsCreator *			findCreator(GameSpell *spell);
	static bool						init();

public:
	SkillHit(GameSpell *p);
	virtual	~SkillHit(void);
	bool	actived();
	void	setSkillPackage(GameSkill_Package *p);
	void	addTarget(int id);
	void	flush();
	virtual void update(unsigned int dtick) {}

protected:
	void					hit();
	void					hit(int id);
	void					stop();
	bool					complete();
	int						first();
	int						next(int id);
	GameSpell				*whichSpell;
	typedef std::list<int>	IdList;
	IdList					targets;
	IdList					waitTargets;

private:
	GameSkill_Package		*package;

private:
	bool			isActive;
	bool			isEmpty;
};

inline bool SkillHit::actived()
{
	return isActive;
}
