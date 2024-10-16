
#ifndef __MemoryManager_H__
#define __MemoryManager_H__

#include "OgrePrerequisites.h"
#include "OgreSingleton.h"

namespace Ogre
{
	class FixedSizePool;
	class MemPoolMgr : public Singleton<MemPoolMgr>
	{
		enum
		{
			POOL_TABLE_SIZE = 256    //内存池table的大小
		};
	public:
		MemPoolMgr();
		~MemPoolMgr();

		void *Alloc( size_t n );
		void Free( void *p, size_t n );

	private:
		FixedSizePool *m_pPools[POOL_TABLE_SIZE];
	};
}

#endif