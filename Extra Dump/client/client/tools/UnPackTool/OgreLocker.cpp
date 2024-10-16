#include "stdafx.h"
#include "OgreLocker.h"
#include <windows.h>

LockSection::LockSection()
{
	LPCRITICAL_SECTION psection = new CRITICAL_SECTION;
	InitializeCriticalSection( psection );

	m_pSysObj = psection;
}

LockSection::~LockSection()
{
	DeleteCriticalSection( (LPCRITICAL_SECTION)m_pSysObj );
	delete((LPCRITICAL_SECTION)m_pSysObj);
}

void LockSection::Lock()
{
	EnterCriticalSection( (LPCRITICAL_SECTION)m_pSysObj );
}

void LockSection::Unlock()
{
	LeaveCriticalSection( (LPCRITICAL_SECTION)m_pSysObj );
}
