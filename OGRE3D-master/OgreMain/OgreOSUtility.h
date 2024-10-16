
#ifndef __OgreOSUtility_H__
#define __OgreOSUtility_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	struct RuntimeSysInfo
	{
		int				num_cpu;					// cpu������Ŀ
		int				num_thread;					// �߳���Ŀ
		float			kernel_cpu_usage;			// ��ǰ���̵�Cpuռ����
		float			user_cpu_usage;				// ��ǰ���̵�Cpuռ����
		size_t			memory_usage;				// ��ǰ����ռ���ڴ���
	};
	
	_OgreExport void GetRuntimeSysInfo(RuntimeSysInfo &info);

	_OgreExport void ThreadSleep(uint tick);

	_OgreExport void InitDisplayMode();
	_OgreExport bool ChangeDisplayMode(int w, int h);
	_OgreExport void RestoreDisplayMode();
	_OgreExport size_t GetNumDisplayMode();
	_OgreExport void GetDisplayMode(size_t i, int &w, int &h);
	_OgreExport void GetScreenMode( bool bFullScreen, int& width, int& height );
	_OgreExport bool IsSolutionSupport( int width, int height );
}

#endif