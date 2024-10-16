#pragma once

#include "OgreRect.h"
class IAutoFind
{
public:
	virtual void init( GameMaskPath& path ) = 0;
	virtual Ogre::Point2D getlastAutoDestScenePoint() = 0;
	virtual void clearPath() = 0;
};