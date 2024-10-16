
#include ".\mliquid.h"

//#include <vld.h>

#include "igame.h"

#include "mtexmanager.h"
#include "gchunk.h"
#include "MTerrainRender.h"

#include "OgreSceneRenderer.h"
#include "OgreShaderContext.h"
#include "OgreResourceManager.h"
#include "OgreTexture.h"
#include "OgreColourValue.h"

using namespace TE;
using namespace Ogre;

extern unsigned long g_dwCurTime;

#ifndef RGB
#define RGB(r,g,b)          ((unsigned int)((unsigned char)(r)|(unsigned short)((g)<<8)|(unsigned int)((b)<<16)))
#endif
