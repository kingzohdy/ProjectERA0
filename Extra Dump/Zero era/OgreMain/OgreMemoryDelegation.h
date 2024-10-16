#pragma once
#ifndef _MEMORYDELEGATION_H__
#define _MEMORYDELEGATION_H__

#include "OgrePlatform.h"
#include "OgrePrerequisites.h"

#if defined ( OGRE_GCC_VISIBILITY )
#   pragma GCC visibility push(default)
#endif

namespace Ogre {
	class _OgreExport MemoryDelegation
	{
#if defined(USE_MEMORYDELEGATION)
	public:
		static void *	operator new(size_t size);
		static void *	operator new(size_t size, void *ptr);
		static void *	operator new(size_t size, const std::nothrow_t &);
		static void *	operator new[](size_t size);
		static void *	operator new[](size_t size, void *ptr);
		static void *	operator new[](size_t size, const std::nothrow_t &);
		static void		operator delete(void *ptr, size_t size);
		static void		operator delete[](void *ptr, size_t size);
#endif
	};

	_OgreExport void *	alloc(size_t size);
	_OgreExport void	release(void *ptr);
	_OgreExport void *	resize(void *ptr, size_t size);
}

#if defined ( OGRE_GCC_VISIBILITY )
#   pragma GCC visibility pop
#endif

#endif
