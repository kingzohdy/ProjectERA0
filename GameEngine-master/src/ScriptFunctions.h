#ifndef _SCRIPTFUNCTION_H
#define _SCRIPTFUNCTION_H

#include "OgreScriptLuaVM.h"
#include "GameSkill_Base.h"
#include <vector>
#include <map>
#include <set>
#include <list>

class GameSpell;

class ScriptFunctions
{
private:
	lua_State *L;
protected:
	ScriptFunctions();

public:
	~ScriptFunctions();

public:
	static ScriptFunctions & getSingleton();

public:
	void registerToLua(lua_State *ls);
	void update(unsigned int dtick);

private:
	void waitForTime(lua_State *ls, unsigned int when);
	void waitForCondition(lua_State *ls);

private:
	class FirstGreater
	{
	public:
		template<class T>
		bool operator () (const T &a, const T &b)
		{
			return a.first >= b.first;
		}
	};
	typedef std::vector<std::pair<unsigned int, lua_State *>> TimeArray;
	TimeArray	m_timeArray;
	std::list<lua_State *>							m_conditionArray;
	std::map<GameSpellPtr, std::set<lua_State *>>	m_skill2ls;
	std::map<lua_State *, GameSpellPtr>				m_ls2skill;

public:
	bool callScript(const char *function, const char *params, ...);/// paramsø…“‘Œ™bsnupfi
	void callSpellScript(const char *function, GameSpell *pSpell);

private:
	static int sleep(lua_State *ls);
	static int unless(lua_State *ls);
	static int block(lua_State *ls);
	static int result(lua_State *ls);
	static int unblock(lua_State *ls);

	static int intbytes(lua_State *ls);
	static int bytesint(lua_State *ls);
};


#endif
