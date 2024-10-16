#ifndef __MD5_H__
#define __MD5_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	_OgreExport void Md5Calc(char output[16], const char *input, size_t length);
	_OgreExport bool Md5Verify(const char target[16], const char *input, size_t length);
}

#endif
