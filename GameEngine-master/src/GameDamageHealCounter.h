#include <map>
#include <string>
#include <list>

class GameActor;

struct GameDamageHealInfo//tolua_export
{//tolua_export
	//tolua_begin
	int		damage;
	int		heal;
	int		hurt;
	double	time;
	//tolua_end
};//tolua_export

struct GameBossInfo//tolua_export
{//tolua_export
	//tolua_begin
	int		targetId;
	//tolua_end
};//tolua_export

class GameDamageHealCounter//tolua_export
{//tolua_export
private:
	typedef std::map<int, GameDamageHealInfo>	PLAYERLIST;
	typedef std::map<int, GameBossInfo>			BOSSLIST;
	typedef std::map<int, unsigned int>			TIMELIST;
	typedef std::map<std::string, int>			INDEXLIST;
	typedef std::map<int, int>			INDEXTOID;
	INDEXLIST					playerIndex;
	INDEXTOID					indexToId;
	std::list<int>				playerIndexIdle;
	int							playerIndexMax;
	PLAYERLIST	players;
	TIMELIST	playersTime;
	BOSSLIST	bosses;

	const char *filter;

protected:
	GameDamageHealCounter();

public:
	static GameDamageHealCounter & getSingleton();

private:
	GameDamageHealInfo &		insertOrGetPlayerInfo(GameActor *pActor);
	int							getPlayerIndex(GameActor *pActor);
	bool						Filter(const char *name);

public:
	void		InFight(int id);
	void		OutFight(int id);
	void		IncDamage(int id, int damage);
	void		IncHeal(int id, int heal);
	void		IncHurt(int id, int hurt);
	void		InsertBoss(int bossId, int targetId);

	//tolua_begin
	void						SetFilter(const char *func);
	GameActor *					getNextPlayer(GameActor *pActor);
	const GameDamageHealInfo *	getPlayerInfo(GameActor *pActor);
	void						reset();
	GameActor *					getNextBoss(GameActor *pActor);
	const GameBossInfo *		getBossInfo(GameActor *pActor);
	//tolua_end
};//tolua_export
