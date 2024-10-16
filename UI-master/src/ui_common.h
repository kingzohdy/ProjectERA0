
#ifndef __UICOMMON_H__
#define __UICOMMON_H__

#pragma warning(disable : 4786)
//#include <zmouse.h>
#include <Windows.h>
#include <assert.h>

#include "OgreColourValue.h"
#include "OgreVector2.h"
#include "OgreUIRenderer.h"
#include "OgreScriptLuaVM.h"

#include <vector>
#include <deque>
#include <map>
#include <set>
#include <stack>
#include <algorithm>
#include <math.h>
#include "ui_enumdef.h"
//#include "../sdk/DX9SDK/Include/strsafe.h"

//using Ogre::ScriptVM;


//using Ogre::CVar;
//using Ogre::CVarFloat;
//using Ogre::CVarString;
//using Ogre::CVarArray;
//using Ogre::CVarTable;
//using Ogre::CVarFunc;

/////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
// Defines and macros 
//--------------------------------------------------------------------------------------
#define EVENT_BUTTON_CLICKED                0x0101
#define EVENT_COMBOBOX_SELECTION_CHANGED    0x0201
#define EVENT_RADIOBUTTON_CHANGED           0x0301
#define EVENT_CHECKBOX_CHANGED              0x0401
#define EVENT_SLIDER_VALUE_CHANGED          0x0501
#define EVENT_EDITBOX_STRING                0x0601
// EVENT_EDITBOX_CHANGE is sent when the listbox content changes
// due to user input.
#define EVENT_EDITBOX_CHANGE                0x0602
#define EVENT_LISTBOX_ITEM_DBLCLK           0x0701
// EVENT_LISTBOX_SELECTION is fired off when the selection changes in
// a single selection list box.
#define EVENT_LISTBOX_SELECTION             0x0702
#define EVENT_LISTBOX_SELECTION_END         0x0703


#define UITRACE_ERR(str,hr)           (hr)

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = (x); if( FAILED(hr) ) { UITRACE_ERR( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return UITRACE_ERR( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#endif

#undef stricmp
#define stricmp _stricmp

const float UI_NEAR_BUTTON_DEPTH = 0.6f;
const float UI_FAR_BUTTON_DEPTH = 0.8f;

enum UI_CONTROL_TYPE
{ 
	UI_CONTROL_BUTTON,
	UI_CONTROL_STATIC,
	UI_CONTROL_CHECKBOX,
	UI_CONTROL_RADIOBUTTON,
	UI_CONTROL_COMBOBOX,
	UI_CONTROL_SLIDER,
	UI_CONTROL_EDITBOX,
	UI_CONTROL_IMEEDITBOX,
	UI_CONTROL_LISTBOX,
	UI_CONTROL_SCROLLBAR,
};

enum UI_CONTROL_STATE
{ 
	UI_STATE_NORMAL = 0,
	UI_STATE_DISABLED,
	UI_STATE_HIDDEN,
	UI_STATE_FOCUS,
	UI_STATE_MOUSEOVER,
	UI_STATE_PRESSED,
};

//数字
inline bool IsDigit( char c )
{
	return( (c>='0')&&(c<='9') );
}

//符号
inline bool IsSymbol( char c  )
{
	// "_ 符号单独处理"
	if( c == '_' )  return false;
	if((c>=' ')&&(c<='/')) return true;
	if((c>=':')&&(c<='@')) return true;
	if((c>='[')&&(c<='`')) return true;
	if((c>='{')&&(c<='~')) return true;

	return false;	

}

//英文
inline bool IsAlpha(char c )
{
	return (( (c>='A')&&(c<='Z') )||( (c>='a')&&(c<='z') ));
}

//tolua_begin
//英文或汉字
inline bool IsPrint(char c )
{
	return (!((c>=0)&&(c<' ')||(c==127 )));
}
//tolua_end

// 是否合法字符
inline bool IsLegal( char c )
{
	//return ( !( c == '#' ) );
	return true;
}

extern bool Sys_IsDBCSLeadByte(unsigned char c);

//0-asc,  1-dbchar head,  2-dbchar tail
inline int GetDBCSCharState( const char *buffer, int pos )
{
	int state = 0;
	if (buffer)
	{
		assert( pos < (int)strlen(buffer) );

		for( int i=0; i<=pos; i++ )
		{
			unsigned char c = (unsigned char)buffer[i];
			switch( state )
			{
			case 0:
				if(Sys_IsDBCSLeadByte(c)) state = 1;
				break;
			case 1:
				state = 2;
				break;
			case 2:
				if(Sys_IsDBCSLeadByte(c)) state = 1;
				else state = 0;
				break;
			}
		}
	}

	return state;
}

//tolua_begin
inline bool IsDBCSChar( const char *buffer, int pos )
{
	return ( GetDBCSCharState(buffer, pos) != 0 );
}

inline bool IsDBCSHead( const char *buffer, int pos )
{
	return ( GetDBCSCharState(buffer, pos) == 1 );
}

inline bool IsDBCSTail( const char *buffer, int pos )
{
	return ( GetDBCSCharState(buffer, pos) == 2 );
}
//tolua_end

/////////////////////////////////////////////////////////////////////////////////

class CRefObject
{
public:
	CRefObject() : m_RefCount(1){}
	void IncRefCount(){ m_RefCount++; }
	int  GetRefCount(){ return m_RefCount; }
	void Release()
	{
		m_RefCount--;
		if( m_RefCount == 0 ) delete this;
	}

protected:
	virtual ~CRefObject(){;}

private:
	int m_RefCount;
};

/////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline T Max( T t1, T t2 )
{
	return t1>t2 ? t1 : t2;
}

template<typename T>
inline T Min( T t1, T t2 )
{
	return t1<t2 ? t1 : t2;
}

template<typename T>
inline void Swap( T &t1, T &t2 )
{
	T tmp(t1);
	t1 = t2;
	t2 = tmp;
}

template<typename T>
inline T Clamp( T t1, T t2, T t )
{
	return t<t1 ? t1 : (t>t2? t2:t);
}

template <typename T>
inline bool Parallel(T t1, T t2)
{
	return (t1>=0 && t2>=0)||(t1<=0 && t2<=0);
}
//////////////////////////////////


#define DECLARE_CLONE(rootclass, classname) \
	public:  \
	virtual rootclass *CreateClone(); \
	void CopyMembers(classname *pdest);

#define IMPLEMENT_CLONE(rootclass, classname) \
	rootclass *classname::CreateClone() \
{ \
	classname *pobj = new classname; \
	assert(pobj != NULL); \
	CopyMembers(pobj);	\
	return pobj;	\
}

#define DECLARE_UIOBJECT_CLONE(classname) DECLARE_CLONE(UIObject, classname)
#define IMPLEMENT_UIOBJECT_CLONE(classname) IMPLEMENT_CLONE(UIObject, classname)


enum SC_EVENT_T
{
	SC_ONANIMFINISHER = 0,
	SC_ONCHAR,
	SC_ONFINISHCHAR,
	SC_ONQUERYCHAR,
	SC_ONCLICK,
	SC_ONCOLORSELECT,
	SC_ONCURSORCHANGER,
	SC_ONDOUBLECLICK,
	SC_ONDRAGSTART,
	SC_ONDRAGSTOP,
	SC_ONEDITFOCUSGAINED,
	SC_ONEDITFOCUSLOST,
	SC_ONENTER,
	SC_ONENTERPRESSED,
	SC_ONCTRLENTERPRESSED,
	SC_ONESCAPEPRESSED,
	SC_ONEVENT,
	SC_ONFOCUSGAINED,
	SC_ONFOCUSLOST,
	SC_ONHIDE,
	SC_ONHORIZONTALSCROLL,
	SC_ONHYPERLINKCLICK,
	SC_ONHYPERLINKENTER,
	SC_ONHYPERLINKLEAVE,
	SC_ONINPUTLANGUAGECHANGED,
	SC_ONKEYDOWN,
	SC_ONKEYUP,
	SC_ONLEAVE,
	SC_ONLOAD,
	SC_ONMESSAGESCROLLCHANGED,
	SC_ONMOUSEDOWN,
	SC_ONMOUSEUP,
	SC_ONMOUSEWHEEL,
	SC_ONMOUSEMOVE,
	SC_ONMOVIEFINISHED,
	SC_ONMOVIEHIDESUBTITLE,
	SC_ONMOVIESHOWSUBTITLE,
	SC_ONRECEIVEDRAG,
	SC_ONSCROLLRANGECHANGED,
	SC_ONSHOW,
	SC_ONSIZECHANGED,
	SC_ONSPACEPRESSED,
	SC_ONTABPRESSED,
	SC_ONTEXTCHANGED,
	SC_ONTEXTSET,
	SC_ONUPDATE,
	SC_ONUPDATEMODEL,
	SC_ONVALUECHANGED,
	SC_ONVERTICALSCROLL,
	SC_ONBEGINDRAG,
	SC_ONBUTTONCLICK,
	SC_ONWEBEVEN,
	SC_ONENDMOVE,
	SC_MAXEVENT,
};

class UIObject{ //tolua_export
	DECLARE_UIOBJECT_CLONE(UIObject)

public:
	UIObject() : m_bTemplate(false), m_RefCount(1)
	{
	}
	virtual const char *GetTypeName(){ return "UIObject"; }

	int CallFunction(int type, const char *format="", ...);
	int CallScript(int type, const char *format="", ...);
	int CallScript(const char *pfunc, const char *format="", ...);

	bool ReLoadLuaFile( const char *uires );

	void addRef()
	{
		m_RefCount++;
	}
	void release()
	{
		m_RefCount--;
		assert(m_RefCount >= 0);
		if(m_RefCount == 0)
		{
			delete this;
		}
	}
	bool hasScriptsEvent( int eventId );

public:
	//tolua_begin
	void SetName(const char *name);
	const char *GetName();
	//tolua_end
public:
	bool m_bTemplate;
	std::string m_Name;
	std::string m_Inherits;
	std::map<int,std::string> m_pScriptStr;

public:
	//供底派生类调用的函数
	Ogre::HUIRES AssignHUIRes(Ogre::HUIRES h);

protected:
	int m_RefCount;

	virtual ~UIObject()
	{
	}
};//tolua_export

//单句脚本最大字符串长度
const int UI_SCRIPT_STRLEN_MAX = 4096;

//UI模块脚本函数中出现的字符串默认长度(例如textlist中文本最大长度等)
const int UI_DEFAULT_BUFSIZE = 1024;

//UI模块临时字符串长度
const int UI_TEMPORARY_BUFSIZE = 128;

const int UI_EDIT_HISTORY = 8;

//Label控件的相关常量
const int DEFAULT_LABEL_MAX_WIDTH = 800;
const int DEFAULT_LABEL_MIN_HEIGHT = 0;
const int DEFAULT_LABEL_FLASHLIVE = 700;
const int DEFAULT_LABEL_FLASHDEATH = 400;

//UI内部的层级结构最大值，用于排序
const int DEFAULT_FRAME_MAX_DEPTH = 10;
//Top Level
const int DEFAULT_FRAME_MAX_LEVLE = 0x7fffff00;

//UI尺寸的基准
//tolua_begin
//const int DEFAULT_UI_WIDTH = 1024;
//const int DEFAULT_UI_HEIGHT = 768;
enum
{
	DEFAULT_UI_WIDTH = 1024,
	DEFAULT_UI_HEIGHT = 768
};
//tolua_end

const std::string UI_ROOT_NAME = "UIClient";

struct  stUIEncrypt
{
	unsigned char *pInBuf;
	unsigned char *pOutBuf;
	int	 nInBufLen;
	int  nOutBufLen;

};

//快捷键相关的定义
const int UI_ACCEL_SCRIPT = 128;

// 表情闪烁的时间
const unsigned int BLINK_CIRCLE = 2000;
const unsigned int BLINK_HALFCIRCLE = 1000;


enum eUIKeyState
{
	UI_KEY_DOWN		=0,
	UI_KEY_UP		=1,
	UI_KEY_PRESS	=2,
};

struct AccelItem
{
	int accel_key;

	bool modifier_shift;
	bool modifier_ctrl;
	bool modifier_alt;

	char accel_script[UI_ACCEL_SCRIPT]; 

	AccelItem()
	{
		accel_key=0;

		modifier_shift=false;
		modifier_ctrl=false;
		modifier_alt=false;

		memset(accel_script,0,UI_ACCEL_SCRIPT);
	}

	AccelItem(int keyPrimary,bool shiftFlag=false,bool ctrlFlag=false,bool altFlag=false, const char* pString="")
	{
		accel_key=keyPrimary;

		modifier_shift=shiftFlag;
		modifier_ctrl=ctrlFlag;
		modifier_alt=altFlag;

		memcpy(accel_script,pString,sizeof(pString));
	}

	bool operator==(const AccelItem& src)
	{
		return( 
			(accel_key==src.accel_key)
			&&(modifier_shift==src.modifier_shift)
			&&(modifier_ctrl==src.modifier_ctrl)
			&&(modifier_alt==src.modifier_alt)
			);
	}
};

extern int FloatToInt(float f);

extern Ogre::ScriptVM *g_pUIScriptVM;
extern Ogre::UIRenderer *g_pDisplay;

#endif //__UICOMMON_H__