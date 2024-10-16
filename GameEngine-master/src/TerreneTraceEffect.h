#pragma once

#include "GameEffect_Base.h"
#include "OgreDirDecal.h"

class TerreneTraceEffect : public IGameEffect
{
protected:
	Ogre::DirDecal		*m_pDirDecal;
	Ogre::Vector3		m_startPos;
	Ogre::Vector3		m_endPos;
	unsigned int		m_birthTime;
	unsigned int		m_totalTime;
	unsigned int		m_fadeTime;
	float				m_extendTime;

public:
	TerreneTraceEffect(const char *szPath, int fadeTime, int totalTime, int speed, int time, const GameXYZ &startPos, const GameXYZ &endPos);
	virtual ~TerreneTraceEffect();
	virtual void onStart();
	virtual void destory();
	virtual bool update(unsigned int dtick);
	virtual unsigned int cleanup();
};