#ifndef WIN32UTILS_H
#define WIN32UTILS_H

#include "basictype.h"
#include <windows.h>
#include <sstream>

namespace grav {
namespace w32 {

extern HINSTANCE kCurrentInstance;


class MessageBoxShower
{
public:
	enum Category {
		MSG_FATAL,
		MSG_ERROR,
		MSG_INFO,
	};

	MessageBoxShower(Category category, HWND hwnd);

	~MessageBoxShower();

	std::ostream& Stream() { return stream_; }

private:
	Category category_;
	HWND hwnd_;
	std::ostringstream stream_;

	DISABLE_COPY_AND_ASSIGN(MessageBoxShower);
};

#define MSG_BOX(category, hwnd) \
	(::grav::w32::MessageBoxShower( \
	::grav::w32::MessageBoxShower::MSG_##category, hwnd) .Stream())


class CurrentDirKeeper
{
public:
	CurrentDirKeeper(Linker::Value)
	{
		len_ = GetCurrentDirectory(sizeof(buf_), buf_);
	}

	~CurrentDirKeeper()
	{
		if(len_ > 0)
			SetCurrentDirectory(buf_);
	}

private:
	char buf_[MAX_PATH];
	DWORD len_;

	DISABLE_COPY_AND_ASSIGN(CurrentDirKeeper);
};

class PlatformThread
{
public:
	class Delegate
	{
	public:
		virtual ~Delegate() {}

		virtual void ThreadProcess() = 0;
	};

	static bool Alive(HANDLE thread);

	static bool Join(HANDLE thread);

	static HANDLE Start(Delegate *callback);

private:
	static void ThreadEntry(void *args);

	DISABLE_DEFAULT_CONSTRUCTOR(PlatformThread);
};


class EditLinePrinter
{
public:
	EditLinePrinter(HWND hwnd)
		: hwnd_(hwnd)
	{
	}

	~EditLinePrinter()
	{
		stream_ << "\r\n";
		int len = GetWindowTextLength(hwnd_);
		SendMessage(hwnd_, EM_SETSEL, len, len);
		SendMessage(hwnd_, EM_REPLACESEL, 0,
		            reinterpret_cast<LPARAM>(stream_.str().c_str()));
	}

	std::ostream& Stream() { return stream_; }

private:
	HWND hwnd_;
	std::ostringstream stream_;

	DISABLE_COPY_AND_ASSIGN(EditLinePrinter);
};

#define EDIT_LINE(hwnd) \
	(::grav::w32::EditLinePrinter(hwnd) .Stream())


} // namespace w32
} // namespace grav

#endif // WIN32UTILS_H
