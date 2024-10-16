#include "ScriptFunctions.h"
#include "GameSkill_Base.h"
#include "OgreLog.h"
#include <Windows.h>
#include <algorithm>
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
#pragma comment(lib, "AHClientInterface.lib")
#else
#pragma comment(lib, "luadll.lib")
#pragma comment(lib, "tolua++.lib")
#pragma comment(lib, "AHClientInterface.lib")
#endif

using namespace std;


ScriptFunctions & ScriptFunctions::getSingleton()
{
	static ScriptFunctions singleton;
	return singleton;
}

ScriptFunctions::ScriptFunctions()
{
	L = NULL;
}

ScriptFunctions::~ScriptFunctions()
{

}

bool ScriptFunctions::callScript(const char *function, const char *params, ...)
{
	bool result = false;

	size_t len = sizeof("return coroutine.create()") + strlen(function);
	char *str = (char *)malloc(len + 1);
	sprintf(str, "return coroutine.create(%s)", function);
	do 
	{
		if (luaL_loadstring(L, str) || lua_pcall(L, 0, 1, 0))
		{
			LOG_WARNING("[script error]%s", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		lua_State *ls = lua_tothread(L, -1);

		va_list vl;
		va_start(vl, params);

		const char *sig = params ? params : "";
		int narg = 0;
		while(*sig)
		{
			if (*sig == 'd')
			{
				lua_pushnumber(ls, va_arg(vl, double));
			}
			else if (*sig == 'f')
			{
				lua_pushnumber(ls, va_arg(vl, float));
			}
			else if (*sig == 'i')
			{
				lua_pushnumber(ls, va_arg(vl, int));
			}
			else if (*sig == 's')
			{
				lua_pushstring(ls, va_arg(vl, char *));
			}
			else if (*sig == 'b')
			{
				lua_pushboolean(ls, va_arg(vl, bool));
			}
			else if (*sig == 'u')
			{
				sig++;
				assert(sig[0] == '[');
				const char *pend = strchr(sig, ']');
				assert(pend != NULL);
				size_t len = pend - sig - 1;
				char *classname = (char *)malloc(len + 1);
				memcpy(classname, sig + 1, len);
				classname[len] = 0;
				tolua_pushusertype(ls, va_arg(vl, void *), classname);
				free(classname);
				sig = pend;
			}
			else
			{
				sig += strlen(sig);
				LOG_SEVERE("[script error]callScript params error: %s", params);
				break;
			}
			sig++;
			narg++;
			luaL_checkstack(ls, 1, "callScript error: too many arguments");
		}

		int nres = (int)strlen(sig);	/* number of expected results */
		int ret = lua_resume(ls, narg);
		if (ret != 0 && ret != LUA_YIELD) /* do	the	call */
		{
			LOG_SEVERE("[script error]%s", lua_tostring(ls, -1));
			lua_pop(ls, 1);
			break;
		}

		result = true;
	} while (false);
	lua_pop(L, 1);
	free(str);
	return result;
}

void ScriptFunctions::callSpellScript(const char *function, GameSpell *pSpell)
{
	size_t len = sizeof("return SkillScript.Create()") + strlen(function);
	char *str = (char *)malloc(len + 1);
	sprintf(str, "return SkillScript.Create(%s)", function);
	int oldtop = lua_gettop(L);
	do 
	{
		if (luaL_loadstring(L, str) || lua_pcall(L, 0, LUA_MULTRET, 0))
		{
			LOG_WARNING("[script error]%s", lua_tostring(L, -1));
			lua_pop(L, 1);
			break;
		}
		int top = lua_gettop(L);
		if (oldtop == top)
		{
			break;
		}
		for (int i = oldtop + 1; i <= top; ++i)
		{
			lua_State *ls = lua_tothread(L, i);
			m_ls2skill[ls] = pSpell;

			tolua_pushusertype(ls, pSpell, "GameSpell");
			int ret = lua_resume(ls, 1);
			if (ret != 0 && ret != LUA_YIELD) /* do	the	call */
			{
				LOG_SEVERE("[script error]%s", lua_tostring(ls, -1));
				lua_pop(ls, 1);
				break;
			}
		}
	} while (false);
	lua_settop(L, oldtop);
	free(str);
}

void ScriptFunctions::update(unsigned int dtick)
{
	unsigned int now = timeGetTime();
	while (m_timeArray.size() > 0)
	{
		if (m_timeArray.front().first > now)
		{
			break;
		}
		lua_State *ls = m_timeArray.front().second;
		pop_heap(m_timeArray.begin(), m_timeArray.end(), FirstGreater());
		m_timeArray.pop_back();
		lua_pushlightuserdata(L, ls);
		lua_pushnil(L);
		lua_settable(L, LUA_REGISTRYINDEX);
		map<lua_State *, GameSpellPtr>::iterator i = m_ls2skill.find(ls);
		if (i != m_ls2skill.end())
		{
			m_skill2ls[i->second].erase(m_skill2ls[i->second].find(ls));
			if (m_skill2ls[i->second].empty())
			{
				m_skill2ls.erase(m_skill2ls.find(i->second));
			}
			int ret = lua_resume(ls, 0);
			if (ret != 0 && ret != LUA_YIELD)
			{
				LOG_SEVERE("[script error]%s", lua_tostring(ls, -1));
				lua_pop(ls, 1);
			}
		}
	}
	for (list<lua_State *>::iterator i = m_conditionArray.begin(); i != m_conditionArray.end();)
	{
		lua_State *ls = *i;
		if (m_ls2skill.find(ls) == m_ls2skill.end())
		{
			i = m_conditionArray.erase(i);
			lua_pushthread(ls);
			lua_pushnil(ls);
			lua_settable(ls, LUA_REGISTRYINDEX);
			lua_pushlightuserdata(L, ls);
			lua_pushnil(L);
			lua_settable(L, LUA_REGISTRYINDEX);
		}
		else
		{
			do 
			{
				lua_pushthread(ls);
				lua_xmove(ls, L, 1);
				lua_gettable(L, LUA_REGISTRYINDEX);
				if (lua_pcall(L, 0, 1, 0))
				{
					LOG_SEVERE("[script error]%s", lua_tostring(L, -1));
					lua_pop(L, 1);
					++i;
					break;
				}
				if (!lua_toboolean(L, -1))
				{
					lua_pop(L, 1);
					++i;
					break;
				}
				i = m_conditionArray.erase(i);
				lua_pushthread(ls);
				lua_pushnil(ls);
				lua_settable(ls, LUA_REGISTRYINDEX);
				lua_pushlightuserdata(L, ls);
				lua_pushnil(L);
				lua_settable(L, LUA_REGISTRYINDEX);
				int ret = lua_resume(ls, 0);
				if (ret != 0 && ret != LUA_YIELD)
				{
					LOG_SEVERE("[script error]%s", lua_tostring(ls, -1));
					lua_pop(ls, 1);
				}
			} while (false);
		}
	}

	for (map<lua_State *, GameSpellPtr>::iterator i = m_ls2skill.begin(); i != m_ls2skill.end();)
	{
		GameSpell *pSpell = i->second;
		if (pSpell->getState() == GameSpell::STATE_END && m_skill2ls.find(pSpell) == m_skill2ls.end())
		{
			i = m_ls2skill.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void ScriptFunctions::waitForTime(lua_State *ls, unsigned int when)
{
	m_skill2ls[m_ls2skill[ls]].insert(ls);
	m_timeArray.push_back(std::pair<unsigned int, lua_State *>(when + timeGetTime(), ls));
	push_heap(m_timeArray.begin(), m_timeArray.end(), FirstGreater());
	lua_pushlightuserdata(ls, ls);
	lua_pushthread(ls);
	lua_settable(ls, LUA_REGISTRYINDEX);
}

void ScriptFunctions::waitForCondition(lua_State *ls)
{
	m_conditionArray.push_back(ls);
	lua_pushlightuserdata(ls, ls);
	lua_pushthread(ls);
	lua_settable(ls, LUA_REGISTRYINDEX);
}

void ScriptFunctions::registerToLua(lua_State *ls)
{
	L = ls;
	static const luaL_Reg tolua[] = {
		{"sleep", sleep},
		{"unless", unless},
		{"block", block},
		{"result", result},
		{"unblock", unblock},

		{"intbytes", intbytes},
		{"bytesint", bytesint},
		{NULL, NULL}
	};
	luaL_register(L, "_G", tolua);
}

int ScriptFunctions::sleep(lua_State *ls)
{
	unsigned int millisecond = unsigned int(luaL_checknumber(ls, 1) * 1000);
	getSingleton().waitForTime(ls, millisecond);
	return lua_yield(ls, 0);
}

int ScriptFunctions::unless(lua_State *ls)
{
	lua_settop(ls, 1);
	luaL_checktype(ls, 1, LUA_TFUNCTION);
	lua_pushthread(ls);
	lua_pushvalue(ls, 1);
	lua_settable(ls, LUA_REGISTRYINDEX);
	getSingleton().waitForCondition(ls);
	return lua_yield(ls, 0);
}

int ScriptFunctions::block(lua_State *ls)
{
	luaL_checkany(ls, 1);
	lua_pushvalue(ls, 1);
	lua_gettable(ls, LUA_REGISTRYINDEX);
	if (lua_type(ls, -1) == LUA_TTHREAD)
	{
		lua_pushvalue(ls, -1);
		lua_gettable(ls, LUA_REGISTRYINDEX);
		int n = luaL_checkint(ls, -1);
		lua_pop(ls, 1);
		lua_pushvalue(ls, -1);
		lua_pushnil(ls);
		lua_settable(ls, LUA_REGISTRYINDEX);
		lua_State *lss = lua_tothread(ls, -1);
		int ret = lua_resume(lss, n);
		if (ret != 0 && ret != LUA_YIELD) /* do	the	call */
		{
			LOG_SEVERE("[script error]%s", lua_tostring(lss, -1));
			lua_pop(lss, 1);
		}
	}
	lua_pop(ls, 1);
	lua_pushvalue(ls, 1);
	lua_pushthread(ls);
	lua_pushvalue(ls, -1);
	lua_pushinteger(ls, 0);
	lua_settable(ls, LUA_REGISTRYINDEX);
	lua_settable(ls, LUA_REGISTRYINDEX);
	return lua_yield(ls, lua_gettop(ls) - 1);
}

int ScriptFunctions::result(lua_State *ls)
{
	luaL_checkany(ls, 1);
	int m = lua_gettop(ls);
	lua_pushvalue(ls, 1);
	lua_gettable(ls, LUA_REGISTRYINDEX);
	if (lua_type(ls, -1) == LUA_TTHREAD)
	{
		lua_State *lss = lua_tothread(ls, -1);
		lua_pushvalue(ls, -1);
		lua_gettable(ls, LUA_REGISTRYINDEX);
		int n = luaL_checkint(ls, -1) + m - 1;
		lua_pop(ls, 1);
		lua_pushinteger(ls, n);
		lua_settable(ls, LUA_REGISTRYINDEX);
		lua_xmove(ls, lss, m - 1);
		lua_pushinteger(ls, n);
		return 1;
	}
	return 0;
}

int ScriptFunctions::unblock(lua_State *ls)
{
	int n = lua_gettop(ls);
	lua_pushvalue(ls, 1);
	lua_insert(ls, 2);
	lua_pushcfunction(ls, result);
	lua_insert(ls, 2);
	lua_call(ls, n, 1);
	int m = lua_tointeger(ls, -1);
	lua_pop(ls, 1);
	lua_pushvalue(ls, 1);
	lua_gettable(ls, LUA_REGISTRYINDEX);
	if (lua_type(ls, -1) != LUA_TNIL)
	{
		lua_pushvalue(ls, -1);
		lua_pushnil(ls);
		lua_settable(ls, LUA_REGISTRYINDEX);
		lua_pushvalue(ls, 1);
		lua_pushnil(ls);
		lua_settable(ls, LUA_REGISTRYINDEX);
	}
	if (lua_type(ls, -1) == LUA_TTHREAD)
	{
		lua_State *lss = lua_tothread(ls, -1);
		int ret = lua_resume(lss, m);
		if (ret != 0 && ret != LUA_YIELD) /* do	the	call */
		{
			LOG_SEVERE("[script error]%s", lua_tostring(lss, -1));
			lua_pop(lss, 1);
		}
		n = lua_gettop(lss);
		lua_xmove(lss, ls, n);
	}
	else
	{
		n = 0;
	}
	return n;
}

int ScriptFunctions::intbytes(lua_State *ls)
{
	int n = lua_gettop(ls);
	luaL_Buffer buffer;
	luaL_buffinit(ls, &buffer);
	for (int i = 1; i <= n; ++i)
	{
		int s = lua_tointeger(ls, i);
		luaL_addlstring(&buffer, ((char *)&s), sizeof(int));
	}
	luaL_pushresult(&buffer);
	return 1;
}

int ScriptFunctions::bytesint(lua_State *ls)
{
	size_t n = 0;
	const char *str = lua_tolstring(ls, 1, &n);
	n = n / sizeof(int);
	for (size_t i = 0; i < n; ++i)
	{
		lua_pushinteger(ls, *(int *)(str + i * sizeof(int)));
	}
	return n;
}
