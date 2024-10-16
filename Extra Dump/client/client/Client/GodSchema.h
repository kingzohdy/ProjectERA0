#pragma once

#include "GameCamera.h"
#include "GameActor.h"
#include "GameSkill_Effect.h"
#include "GameSelectManager.h"

class Ogre::ShowActor;

class GodMouseEventHandle : public IMouseEventAction
{
	bool onLButtonDown( GameActor* pTarget, int x, int z );
	bool onRButtonDown( GameActor* pTarget, int x, int z );
};

class GodUnit : public GameActor
{
public:
	unsigned int	deadTime;
	int				life;
public:
	GodUnit();
	static	GodUnit * create(int resid, int x, int y, int angle);
	static	GodUnit * clone(int x, int y, int angle);
	int		getRadius();
	bool	Death();
	void	hit();
	void	update(unsigned int tick);
};

class GodEffect : public GameSpellEffect
{
public:
	float			m_angle;

public:
	GodEffect(const char *respath, int speed, int life);
	static	GodEffect * create(const char *respath, int speed, int life, float angle);
	virtual	bool hitTest(GodUnit *unit);
	virtual void onHit(GodUnit *unit);
	virtual void update(int dtick);
};

class GodSlash : public GodEffect
{
public:
	GodSlash(const char *respath, int speed, int life);
	static	GodSlash * create(const char *respath, int speed, int life, float angle);
	virtual	bool hitTest(GodUnit *unit);
	virtual void onHit(GodUnit *unit);
};

typedef std::list<GodUnit *> GodUnitList;
typedef std::list<GodEffect *> GodEffectList;

extern GodUnitList godUnitList;
extern GodUnitList godTesterList;
extern GodEffectList godEffectList;

//tolua_begin
void god_begin(int resid, int num);
void god_end();
void god_test(int num);
void god_test_end();
//tolua_end

void god_update(unsigned int tick);