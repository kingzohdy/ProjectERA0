#include "ScriptTickAction.h"
#include "GameTickAction.h"
#include "OgreLog.h"
extern "C"
{
#include "../sdk/lua/src/lua.h"
#include "../sdk/lua/src/lauxlib.h"
#include "../sdk/lua/src/lualib.h"
}

#include "tolua++/include/tolua++.h"

#ifdef _DEBUG
#pragma comment(lib, "luadll_d.lib")
#pragma comment(lib, "tolua++_d.lib")
#else
#pragma comment(lib, "luadll.lib")
#pragma comment(lib, "tolua++.lib")
#endif

class ScriptActionDelay : public ITickActionDelay
{
private:
	lua_State			*L;

public:
	ScriptActionDelay(unsigned int time, lua_State *ls);
	virtual ~ScriptActionDelay();
	virtual unsigned int expire();
};

class ScriptActionTest : public ITickActionCondition
{
private:
	lua_State			*L;

	class DelayDelete : public ITickActionDelay
	{
	private:
		ScriptActionTest	*who;
	public:
		DelayDelete(ScriptActionTest *ptr) : ITickActionDelay(0), who(ptr) {}
		virtual ~DelayDelete() {}
		virtual unsigned int expire();
	};

public:
	ScriptActionTest(lua_State *ls);
	virtual ~ScriptActionTest();
	virtual bool	until() { return true; }
	virtual bool	work();
};

ScriptActionDelay::ScriptActionDelay(unsigned int time, lua_State *ls) : 
	ITickActionDelay(time), L(ls)
{
}

ScriptActionDelay::~ScriptActionDelay()
{
	lua_pushlightuserdata(L, this);
	lua_pushnil(L);
	lua_settable(L, LUA_REGISTRYINDEX);
}

unsigned int ScriptActionDelay::expire()
{
	lua_pushlightuserdata(L, this);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_State *ls = lua_tothread(L, -1);
	lua_pop(L, 1);
	int ret = lua_resume(ls, 0);
	if (ret == LUA_YIELD)
	{
		lua_settop(ls, 1);
		if (lua_isnumber(ls, 1))
		{
			return unsigned int(lua_tonumber(ls, 1) * 1000 + 0.5);
		}
		else
		{
			return repeat();
		}
	}
	if (ret != 0)
	{
		LOG_SEVERE("lua_resume error: %s", lua_tostring(ls, -1));
		lua_pop(ls, 1);
	}
	unsigned int n = halt();
	delete this;
	return n;
}

ScriptActionTest::ScriptActionTest(lua_State *ls) :
	L(ls)
{
}

ScriptActionTest::~ScriptActionTest()
{
	lua_pushlightuserdata(L, this);
	lua_pushnil(L);
	lua_settable(L, LUA_REGISTRYINDEX);
}

bool ScriptActionTest::work()
{
	lua_pushlightuserdata(L, this);
	lua_gettable(L, LUA_REGISTRYINDEX);
	lua_State *ls = lua_tothread(L, -1);
	lua_pop(L, 1);
	int ret = lua_resume(ls, 0);
	if (ret == LUA_YIELD)
	{
		return true;
	}
	if (ret != 0)
	{
		LOG_SEVERE("lua_resume error: %s", lua_tostring(ls, -1));
		lua_pop(ls, 1);
	}
	new DelayDelete(this);
	return false;
}

unsigned int ScriptActionTest::DelayDelete::expire()
{
	unsigned int n = halt();

	delete who;
	delete this;

	return n;
}

static int timerToLua(lua_State *L)
{
	int result = 0;
	lua_settop(L, 2);
	if (luaL_dostring(L, "return function(f) return coroutine.create(f) end"))
	{
		LOG_SEVERE("luaL_dostring error: %s", lua_tostring(L, -1));
	}
	else
	{
		lua_pushvalue(L, 1);
		if (lua_pcall(L, 1, 1, 0))
		{
			LOG_SEVERE("lua_pcall error: %s", lua_tostring(L, -1));
		}
		else
		{
			lua_pushlightuserdata(L, new ScriptActionDelay(unsigned int(lua_tonumber(L, 2) * 1000 + 0.5), L));
			lua_pushvalue(L, -1);
			lua_pushvalue(L, -3);
			lua_settable(L, LUA_REGISTRYINDEX);
			result = 1;
		}
	}
	return result;
}

static int tryToLua(lua_State *L)
{
	int result = 0;
	lua_settop(L, 1);
	if (luaL_dostring(L, "return function(f) return coroutine.create(f) end"))
	{
		LOG_SEVERE("luaL_dostring error: %s", lua_tostring(L, -1));
	}
	else
	{
		lua_pushvalue(L, 1);
		if (lua_pcall(L, 1, 1, 0))
		{
			LOG_SEVERE("lua_pcall error: %s", lua_tostring(L, -1));
		}
		else
		{
			lua_pushlightuserdata(L, new ScriptActionTest(L));
			lua_pushvalue(L, -1);
			lua_pushvalue(L, -3);
			lua_settable(L, LUA_REGISTRYINDEX);
			result = 1;
		}
	}
	return result;
}

static int hangupToLua(lua_State *L)
{
	int n = lua_gettop(L);
	for (int i = 1; i <= n; i++)
	{
		void *ptr = lua_touserdata(L, i);
		lua_pushlightuserdata(L, ptr);
		lua_gettable(L, LUA_REGISTRYINDEX);
		int type = lua_type(L, -1);
		lua_pop(L, 1);
		if (type == LUA_TTHREAD)
		{
			GameTickActionManager::RegTag *tag = (GameTickActionManager::RegTag *)ptr;
			if (GameTickActionManager::getSingleton().isReg(tag))
			{
				delete tag;
			}
		}
	}
	return 0;
}

void scriptTickRegisterToLua(lua_State *ls)
{
	static const luaL_Reg tolua[] = {
		{"timer", timerToLua},
		{"try", tryToLua},
		{"hangup", hangupToLua},
		{NULL, NULL}
	};
	luaL_register(ls, "_G", tolua);
}

