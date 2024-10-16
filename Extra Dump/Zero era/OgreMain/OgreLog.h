
#ifndef __Log_H__
#define __Log_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	enum
	{
		LOGL_CONFIG = 1,
		LOGL_INFO = 2,
		LOGL_WARNING = 4,
		LOGL_SEVERE = 8,
		LOGL_SEVEREMSG = 16, 
		LOGL_DUMP = 32,
		LOGL_PRIVATE = 64,

		LOGL_ALLLEVEL = 16-1
	};

	class LogHandler
	{
	public:
		LogHandler(unsigned int levels) : m_LevelFlags(levels){}

		virtual ~LogHandler(){}
		virtual bool Handle( const char *pfilename, int line, unsigned int level, const char *pstr ) = 0;

		uint getLevels()
		{
			return m_LevelFlags;
		}

	protected:
		unsigned int m_LevelFlags;
	};

	_OgreExport int		LogInit();
	_OgreExport void	LogRelease();
	_OgreExport int		LogAddHandler( LogHandler *phandler );
	_OgreExport int		LogAddFileHandler(const char *logfile, unsigned int levels);
	_OgreExport void	LogSetCurParam( const char *pfilename, int line, unsigned int level );
	_OgreExport void	LogMessage( const char *format, ... );

};

#define LOG_CONFIG		Ogre::LogSetCurParam(__FILE__, __LINE__, Ogre::LOGL_CONFIG),	Ogre::LogMessage
#define LOG_INFO		Ogre::LogSetCurParam(__FILE__, __LINE__, Ogre::LOGL_INFO),		Ogre::LogMessage
#define LOG_WARNING		Ogre::LogSetCurParam(__FILE__, __LINE__, Ogre::LOGL_WARNING),	Ogre::LogMessage
#define LOG_SEVERE		Ogre::LogSetCurParam(__FILE__, __LINE__, Ogre::LOGL_SEVERE),	Ogre::LogMessage
#define LOG_SEVEREMSG	Ogre::LogSetCurParam(__FILE__, __LINE__, Ogre::LOGL_SEVEREMSG), Ogre::LogMessage
#define LOG_DUMP		Ogre::LogSetCurParam(__FILE__, __LINE__, Ogre::LOGL_DUMP),		Ogre::LogMessage
#define LOG_PRIVATE		Ogre::LogSetCurParam(__FILE__, __LINE__, Ogre::LOGL_PRIVATE),	Ogre::LogMessage
#endif