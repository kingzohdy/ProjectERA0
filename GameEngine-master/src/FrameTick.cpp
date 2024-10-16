
#include "FrameTick.h"
#include "OgreTimer.h"
#include <Windows.h>

FrameTick::FrameTick() : m_bInit(false), m_LastTick(0), m_FPSAccumTick(0), m_FPSAccumFrame(0), m_CurFPS(0), m_bFPSUpdated(false), m_FPSLimit(0)
{
}

unsigned int FrameTick::DoFrame()
{
	unsigned int curtick = Ogre::Timer::getSystemTick();
	if(!m_bInit)
	{
		m_LastTick = curtick;
		m_bInit = true;
	}

	unsigned int dtick = curtick - m_LastTick;
	if(m_FPSLimit > 0)
	{
		unsigned int interval = Ogre::TimeToTick(1.0f/m_FPSLimit);
		if(dtick < interval)
		{
			::Sleep(interval-dtick);
			curtick = Ogre::Timer::getSystemTick();
			dtick = curtick - m_LastTick;
		}
	}

	m_FPSAccumTick += dtick;
	m_FPSAccumFrame++;

	if(m_FPSAccumTick >= 1000)
	{
		m_CurFPS = float(m_FPSAccumFrame*1000.0f / m_FPSAccumTick);
		m_FPSAccumTick = 0;
		m_FPSAccumFrame = 0;
		m_bFPSUpdated = true;
	}

	m_LastTick = curtick;
	return dtick;
}

void FrameTick::setFrameLimit(int n)
{
	m_FPSLimit = float(n);
}
