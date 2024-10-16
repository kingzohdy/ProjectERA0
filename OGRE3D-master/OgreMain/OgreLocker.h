
#ifndef __Locker_H__
#define __Locker_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	class _OgreExport LockSection
	{
	public:
		LockSection();
		~LockSection();

		void Lock();
		void Unlock();

	private:
		void *m_pSysObj;
	};

	class _OgreExport LockFunctor
	{
	public:
		LockFunctor( LockSection *pSection ) : m_pSection(pSection)
		{
			if(m_pSection == NULL) return;
			assert( m_pSection );
			m_pSection->Lock();
		}

		~LockFunctor()
		{
			if( m_pSection )
			{
				m_pSection->Unlock();
			}			
		}

	private:
		LockSection *m_pSection;
	};
}

#endif