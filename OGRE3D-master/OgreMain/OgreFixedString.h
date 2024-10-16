
#ifndef __FixedString_H__
#define __FixedString_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	typedef void * FIXED_STRING_HANDLE;

	class _OgreExport FixedString
	{
	public:
		static int sysInit();
		static int sysRelease();

		static FIXED_STRING_HANDLE insert(const char *pstr, int len=-1);
		static void addRef(FIXED_STRING_HANDLE h);
		static void release(FIXED_STRING_HANDLE h);

		static const char *handle2Ptr(FIXED_STRING_HANDLE h)
		{
			return (const char *)h;
		}

	public:
		FixedString() : m_StrHandle(NULL){}
		FixedString(FIXED_STRING_HANDLE h) : m_StrHandle(h)
		{
			addRef(h);
		}

		FixedString(const FixedString &rhs) : m_StrHandle(rhs.m_StrHandle)
		{
			addRef(m_StrHandle);
		}

		FixedString(const char *pstr)
		{
			m_StrHandle = insert(pstr); 
		}

		FixedString(const char *pstr, size_t len)
		{
			m_StrHandle = insert(pstr, (int)len); 
		}

		~FixedString()
		{
			release(m_StrHandle); 
		}

		operator const char *() const
		{
			return handle2Ptr(m_StrHandle);
		}

		FIXED_STRING_HANDLE getHandle() const
		{
			return m_StrHandle;
		}

		FixedString &operator=( const FixedString &str );
		FixedString &operator=( const char *pstr );
		size_t length() const;
		FixedString substr(size_t start, size_t count=size_t(-1)) const;

		friend bool operator==( const FixedString &s1, const FixedString &s2 );
		friend bool operator!=( const FixedString &s1, const FixedString &s2 );

	protected:
		union
		{
			FIXED_STRING_HANDLE m_StrHandle;
			char *m_pStr;
		};
	};

	inline bool operator==( const FixedString &s1, const FixedString &s2 )
	{
		return s1.m_StrHandle==s2.m_StrHandle;
	}

	inline bool operator!=( const FixedString &s1, const FixedString &s2 )
	{
		return s1.m_StrHandle!=s2.m_StrHandle;
	}

	_OgreExport bool operator==( const FixedString &s1, const char *s2 );
	_OgreExport bool operator!=( const FixedString &s1, const char *s2 );

}

#endif