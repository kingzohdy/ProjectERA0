
#ifndef __LogFile_H__
#define __LogFile_H__

#include "OgreLog.h"
#include <stdio.h>

namespace Ogre
{
	class FileLogHandler : public LogHandler
	{
	public:
		FileLogHandler( uint levels, const char *filename,size_t maxlength );
		~FileLogHandler();

		virtual bool Handle( const char *pfilename, int line, uint level, const char *pstr );

	private:
		FILE *m_fp;
	};

}

#endif