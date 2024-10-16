#pragma once

#include "GameMover_Base.h"
#include "EntityEffect.h"
#include "GameActor.h"

class EffectXPMover : public IGameMover
{
	enum State
	{
		UP, DOWN, FOLLOW
	};
private:
	float			m_startRad;
	float			m_startSpeed;
	State			m_state;
	unsigned int	m_upTick;
	unsigned int	m_downTick;
	unsigned int	m_lastTime;
	unsigned int	m_costTime;

	int				m_actorId;
	int				m_bindId;

	GameXYZ			m_startPos;
	GameMap			*map;

public:
	EffectXPMover(const GameXYZ &start, int actorId, int bindId);
	virtual void update();
};

class EffectXPEntity : public EntityEffect
{
private:
	IGameMover		*pMover;
	int				actorId;
	int				nValue;

public:
	EffectXPEntity(const GameXYZ &pos, int targetId, int value = 0);
	virtual ~EffectXPEntity();

	virtual bool update(unsigned int dtick);
};

class EffectXPPlay : public IActorDeathAction
{
public:
	EffectXPPlay(GameActor *source, GameActor *target, int value);
	~EffectXPPlay();
	virtual void doAction(GameActor *actor);

protected:
	GameActor		*m_source;
	int				m_targetId;
	int				m_value;
};