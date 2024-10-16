#pragma once
#include "GameEngineTypes.h"
#include "GameSkill_Common.h"
#include "GameDBData.h"
#include <set>
#include <map>

const int SPELL_LIFE_MAX = 60000;

class GameSpellControler
	: public ISpellControler
{
protected:
	GameSpellControler();
public:
	virtual ~GameSpellControler();

public:
	static GameSpellControler & getSingleton();

public:
	virtual void	onPrepare(GameSpell* pSpell);
	virtual void	onCollect(GameSpell* pSpell, unsigned int time = 0);
	virtual void	onCollectFull(GameSpell* pSpell);
	virtual void	onAttack(GameSpell* pSpell, bool passive);
	virtual void	onBreak(int skillreq, int oldSkillreq, unsigned int LeftCool);
	virtual void	onBreak(GameSpell *pSpell);
	virtual void	onPilot(GameSpell* pSpell);
	virtual void	onComplete(GameSpell* pSpell);

	virtual void	onLock(int id = 0);
	virtual void	onHitBack(GameSpell *pSpell, int during, int totalTime, int times);

	virtual int			getOwnerId();
	virtual GameActor *	getOwner();
	virtual GameSpell*	getCurSpell();
	virtual GameSpell*	getSpell(int skillId, int skillLv, int skillReq);
	void		addSpell(GameSpell *pSpell, int skillReq);
	void		clear();
	virtual void		update(unsigned int dtick);

protected:
	void	breakSpell(GameSpell *pSpell);
	void	swapSpellList();
	std::map<int, GameSpellPtr> &	getSpellList();

private:
	std::map<int, GameSpellPtr>	m_lstSpell[2];
	size_t						m_curIdx;
	GameSpellPtr				m_pCurSpell;
	int							m_lockSkillId;
	int							m_lastActId;
	RESPOS						m_lastPos;
	unsigned int				m_lastTime;

	std::set<GameSpell *>		m_lstForceAttack;
};

class ActorSpellControler
	: public ISpellControler
{
public:
	ActorSpellControler(int actorId);
	virtual ~ActorSpellControler();

public:
	virtual void	onPrepare(GameSpell* pSpell);
	virtual void	onCollect(GameSpell* pSpell, unsigned int time = 0);
	virtual void	onCollectFull(GameSpell* pSpell);
	virtual void	onAttack(GameSpell* pSpell, bool passive);
	virtual void	onBreak(int skillreq, int oldSkillreq, unsigned int LeftCool);
	virtual void	onBreak(GameSpell *pSpell);
	virtual void	onPilot(GameSpell* pSpell);
	virtual void	onComplete(GameSpell* pSpell);

	virtual void	onLock(int id = 0);
	virtual void	onHitBack(GameSpell *pSpell, int during, int totalTime, int times);

	virtual int			getOwnerId();
	virtual GameActor *	getOwner();
	virtual GameSpell*	getCurSpell();
	virtual GameSpell*	getSpell(int skillId, int skillLv, int skillReq);
	virtual void		update(unsigned int dtick);

protected:
	void	swapDelSpellList();
	void	delSpell(GameSpell *pSpell);

private:
	int		m_ownerId;
	std::map<int, GameSpellPtr>		m_lstSpell;
	int								m_curSpellId;
	std::set<GameSpellPtr>			m_delSpell[2];
	size_t							m_curIdx;
	int								m_lockSkillId;
	int								m_lastActId;
	RESPOS							m_lastPos;
	unsigned int					m_lastTime;
};