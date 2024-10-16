#ifndef __OGREMCI_H__
#define __OGREMCI_H__

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include "OgrePlatform.h"
#include "OgreSingleton.h"
#include <list>
#include <string>
#include <Windows.h>

extern "C"
{
	#include "../sdk/ffmpeg/include/avformat.h"
};

namespace Ogre
{
	class _OgreExport MCIWrap : public Singleton<MCIWrap>
	{
	private:
		std::list<std::string>	playList;
		HWND			drawWnd;
		bool			running;
		bool			finish;

		AVFormatContext	*pFormatCtx;
		int				cursor;
		int				videoStream;
		int				audioStream;
		AVCodecContext	*pCodecCtx;

	public:
		MCIWrap();
		~MCIWrap();
		void play(const char *filename);
		void start();
		void pause();
		void stop();
		void reset();
		bool isPlaying();
		void updateWindow();
		void update(unsigned int dtick);
	};
}


#endif