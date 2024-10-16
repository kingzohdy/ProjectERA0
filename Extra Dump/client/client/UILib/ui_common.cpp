#include "ui_common.h"
#include "ui_framemgr.h"

#include "OgreScriptLuaVM.h"

#include <fstream>
#include <string>

#ifdef _DEBUG
#pragma comment(lib, "luadll_d.lib")
#else
#pragma comment(lib, "luadll.lib")
#endif

extern "C"
{
#include "lua/src/lua.h"
#include "lua/src/lualib.h"
#include "lua/src/lauxlib.h"
}

using namespace Ogre;

extern Ogre::UIRenderer* g_pDisplay;
extern FrameManager* g_pFrameMgr;

IMPLEMENT_UIOBJECT_CLONE(UIObject)


void UIObject::CopyMembers(UIObject *pdest)
{
	if (pdest)
	{
		pdest->m_Name = m_Name;
		pdest->m_pScriptStr = m_pScriptStr;

// 		for (int i=0; i<SC_MAXEVENT; ++i)
// 		{
// 			pdest->m_pScriptStr[i] = m_pScriptStr[i];
// 		}
	}
}

const char *UIObject::GetName()
{
	return m_Name.c_str();
}

void UIObject::SetName(const char *name)
{
	if (name)
	{
		m_Name = name;
	}
	else
	{
		assert(0);
	}
}

HUIRES UIObject::AssignHUIRes(HUIRES h)
{
	g_pDisplay->AddRef(h);
	return h;
}

//static int ExecuteString( const char* Str )
//{
//	int status	= luaL_dostring( g_pFrameMgr->m_LS,Str );
//	if(status != 0)
//	{
//		ScriptVM::InstRef().ErrorInfoOut( NULL,"error: %s\n", lua_tostring(g_pFrameMgr->m_LS, -1) );
//		return 0;
//	}
//	return 1;
//}

int UIObject::CallFunction(int type, const char *format, ...)
{
	if ( !hasScriptsEvent( type ) )
	{
		return 0;
	}
	
	lua_State *ls = g_pUIScriptVM->getLuaState();

	lua_getglobal( ls, "this" );
	lua_getglobal( ls, m_Name.c_str() );
	lua_setglobal(ls, "this");

	va_list	argptr;
	va_start( argptr, format );
	const char* pfmt = format;
	int count = 0;
	static char* arg[] = { "arg1", "arg2", "arg3", "arg4" };
	while ( *pfmt )
	{
		if ( *pfmt == 'i' )
		{
			int value = va_arg( argptr, int );
			lua_pushnumber( ls, value );
		}
		else if ( *pfmt == 'f' )
		{
			float value = va_arg( argptr, float );
			lua_pushnumber( ls, value );
		}
		else if ( *pfmt == 's' )
		{
			char* pstr = va_arg( argptr, char* );
			lua_pushstring( ls, pstr );
		}
		else if ( *pfmt == '>' )
		{
			++pfmt;
			break;
		}
		else
		{
			assert(0);
		}
		++pfmt;
		lua_setglobal( ls, arg[count++] );
		luaL_checkstack( ls, 1, "too many	arguments" );
	}
	
	int nres = strlen( pfmt );
	const char *pfunc = m_pScriptStr[type].c_str();
	int top = lua_gettop(ls);
	g_pUIScriptVM->callStringNoBack(pfunc, nres);

	nres = 0;
	while ( *pfmt )
	{
		switch ( *pfmt )
		{
		case 'i':
			{
				assert( lua_isnumber( ls, nres ) );
				*va_arg( argptr, int* ) = ( int )lua_tonumber( ls, nres );
			}
			break;

		case 'f':
			{
				assert( lua_isnumber( ls, nres ) );
				*va_arg( argptr, float* ) = ( float )lua_tonumber( ls, nres );
			}
			break;

		case 's':
			{
				assert( lua_isstring( ls, nres ) );
				strcpy( va_arg( argptr, char* ), lua_tostring( ls, nres ) );
			}
		case 'b': /* boolean argument */
			{
				assert( lua_isboolean( ls, nres ) );
				*va_arg( argptr, bool* ) = ( 0 != lua_toboolean( ls, nres ) );
			}
			break;

		default:
			assert( false );
			break;
		}
		++pfmt;
		++nres;
	}
	
	va_end(argptr);
	lua_settop(ls, top);
	lua_setglobal(ls, "this");
	
	return 0;
}

int UIObject::CallScript(int type, const char *format, ...)
{
	if(!hasScriptsEvent(type))
	{
		return 0;
	}

	const char *pfunc = m_pScriptStr[type].c_str();

	//copy from the following function
	lua_State *ls = g_pUIScriptVM->getLuaState();

	lua_getglobal(ls, "this");//backup pre this;
	lua_getglobal(ls, m_Name.c_str());
	lua_setglobal(ls, "this");

	va_list		argptr;
	va_start(argptr, format);
	const char *pfmt = format;
	int count = 0;
	static char *args[] = {"arg1", "arg2", "arg3", "arg4"};
	while(pfmt[count])
	{
		if( pfmt[count] == 'i')
		{
			int value = va_arg(argptr, int);
			lua_pushnumber(ls, value);
		}
		else if( pfmt[count] == 'f')
		{
			float value = va_arg(argptr, float);
			lua_pushnumber(ls, value);
		}
		else if( pfmt[count] == 's')
		{
			char *pstr = va_arg(argptr, char *);
			lua_pushstring(ls, pstr);
		}
		else
		{
			assert(0);
		}
		lua_setglobal(ls, args[count++]);
	}
	va_end(argptr);

	{
		//LuaStackBackup;
		g_pUIScriptVM->callString(pfunc);
	}

	lua_setglobal(ls, "this");
	return 0;
}

int UIObject::CallScript(const char *pfunc, const char *format, ...)
{
	lua_State *ls = g_pUIScriptVM->getLuaState();

	lua_getglobal(ls, "this");//backup pre this;
	lua_getglobal(ls, m_Name.c_str());
	lua_setglobal(ls, "this");

	va_list		argptr;
	va_start(argptr, format);
	const char *pfmt = format;
	int count = 0;
	static char *args[] = {"arg1", "arg2", "arg3", "arg4"};
	while(pfmt[count])
	{
		if(pfmt[count] == 'i')
		{
			int value = va_arg(argptr, int);
			lua_pushnumber(ls, value);
		}
		else if(pfmt[count] == 'f')
		{
			float value = va_arg(argptr, float);
			lua_pushnumber(ls, value);
		}
		else if(pfmt[count] == 's')
		{
			char *pstr = va_arg(argptr, char *);
			lua_pushstring(ls, pstr);
		}
		else
		{
			assert(0);
		}
		lua_setglobal(ls, args[count++]);
	}
	va_end(argptr);

	{
		//LuaStackBackup;
		g_pUIScriptVM->callString(pfunc);
	}

	lua_setglobal(ls, "this");
	return 0;
}

bool UIObject::ReLoadLuaFile( const char *uires )
{
	if (uires)
	{
		for(size_t i=0; i<g_pFrameMgr->m_LuaFile.size(); i++)
		{
			std::string strname = g_pFrameMgr->m_LuaFile[i];

			if(!g_pUIScriptVM->callFile(g_pFrameMgr->m_LuaFile[i].c_str()))
			{
				std::string str = "\tReload lua file error!\n\nFileName:";
				str += g_pFrameMgr->m_LuaFile[i];
				MessageBox(NULL, str.c_str(), "Error", MB_OK);

				return false;
			}
		}

		return true;
	}
	
	return false;
}

bool UIObject::hasScriptsEvent(int eventId)
{
	std::map<int,std::string>::iterator it = m_pScriptStr.find(eventId);
	if( it == m_pScriptStr.end() )
	{
		return false;
	}
	return true;
}

//È«¾Öº¯Êý
int FloatToInt(float f)
{
	return static_cast<int>(f + 0.5f);
}

bool Sys_IsDBCSLeadByte(unsigned char c)
{
	return ::IsDBCSLeadByte(c)==TRUE;
}
