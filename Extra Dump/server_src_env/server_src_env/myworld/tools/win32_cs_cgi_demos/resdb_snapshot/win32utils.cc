#include "win32utils.h"
#include <process.h>


namespace grav {
namespace w32 {

HINSTANCE kCurrentInstance = NULL;

MessageBoxShower::MessageBoxShower(Category category, HWND hwnd)
	: category_(category)
	, hwnd_(hwnd)
{
}

MessageBoxShower::~MessageBoxShower()
{
	const char *caption = NULL;
	UINT flags = 0;
	switch(category_)
	{
	case MSG_FATAL:
		flags = MB_OK | MB_ICONERROR;
		caption = "´íÎó";
		break;
	case MSG_ERROR:
		flags = MB_OK | MB_ICONWARNING;
		caption = "´íÎó";
		break;
	case MSG_INFO:
		flags = MB_OK | MB_ICONINFORMATION;
		caption = "ÏûÏ¢";
		break;
	default:
		return;
	}

	MessageBox(hwnd_, stream_.str().c_str(), caption, flags);
	if(category_ == MSG_FATAL)
		ExitProcess(1);
}


//static
bool PlatformThread::Alive(HANDLE thread)
{
	if(!thread)
		return false;

	DWORD code = 0UL;
	if(!GetExitCodeThread(thread, &code))
		return false;

	return (code == STILL_ACTIVE);
}

//static
bool PlatformThread::Join(HANDLE thread)
{
	DWORD rv = WaitForSingleObject(thread, INFINITE);
	return (rv == WAIT_OBJECT_0);
}

//static
HANDLE PlatformThread::Start(Delegate *callback)
{
	return reinterpret_cast<HANDLE>(_beginthread(ThreadEntry, 0, callback));
}

//static
void PlatformThread::ThreadEntry(void *args)
{
	setlocale(LC_ALL, "Chinese_China.ACP");

	Delegate* callback = static_cast<Delegate*>(args);
	callback->ThreadProcess();
}


} // namespace w32
} // namespace grav
