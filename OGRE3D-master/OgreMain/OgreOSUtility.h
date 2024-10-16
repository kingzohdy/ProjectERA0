
#ifndef __OgreOSUtility_H__
#define __OgreOSUtility_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	struct RuntimeSysInfo
	{
		int				num_cpu;					// cpu核心数目
		int				num_thread;					// 线程数目
		float			kernel_cpu_usage;			// 当前进程的Cpu占用率
		float			user_cpu_usage;				// 当前进程的Cpu占用率
		size_t			memory_usage;				// 当前进程占用内存数
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