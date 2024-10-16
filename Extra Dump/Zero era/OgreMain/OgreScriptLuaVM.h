
#ifndef __OgreScriptLuaVM_H__
#define __OgreScriptLuaVM_H__

#include "OgrePrerequisites.h"
#include <stdio.h>
#include <assert.h>
#include <vector>

struct lua_State;
namespace Ogre
{
	class _OgreExport ScriptVM
	{
	public:
		ScriptVM();
		~ScriptVM();
		bool callStringNoBack(const char *pstr, int nres = 0 );
		bool callString(const char *pstr, int nres = 0 );
		bool callFile(const char *path);
		//eg. format = "fisu[GameSpellCast]f>i"
		bool callFunction(const char *funcname, const char *format, ...);
		void setUserTypePointer(const char *globalname, const char *classname, void *ptr);
		void setValue(const char *globalname, int v);

		lua_State *getLuaState()
		{
			return m_pState;
		}

	protected:
		static void *	LuaStateAlloc(void *ud, void *ptr, size_t osize, size_t nsize);

	protected:
		lua_State *m_pState;
	};
}

#endif