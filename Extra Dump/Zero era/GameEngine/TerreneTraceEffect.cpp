#include "TerreneTraceEffect.h"
#include "GameMap.h"
#include "ShowGameMap.h"
#include "GameActorManager.h"
#include "GameMainPlayer.h"
#include <Windows.h>

using namespace Ogre;

TerreneTraceEffect::TerreneTraceEffect(const char *szPath, int fadeTime, int totalTime, int speed, int time, const GameXYZ &startPos, const GameXYZ &endPos)
	: m_totalTime(totalTime), m_fadeTime(fadeTime), m_birthTime(0)
{
	m_startPos = startPos.toVector3();
	m_endPos = endPos.toVector3();
	m_extendTime = 0;
	if (speed > 0)
	{
		m_extendTime = (m_startPos - m_endPos).length() / speed * 1000;
	}
	if (time > m_extendTime)
	{
		m_extendTime = (float)time;
	}
	m_pDirDecal = new DirDecal;
	m_pDirDecal->setTextureRes(szPath);
	m_pDirDecal->setBlendMode(1);
	m_pDirDecal->setWidth(160.0f);
	m_pDirDecal->setWidthGridNum(6);
}

TerreneTraceEffect::~TerreneTraceEffect()
{
	destory();
}

bool TerreneTraceEffect::update(unsigned int dtick)
{
	if (m_pDirDecal == NULL)
	{
		return false;
	}
	unsigned int now = timeGetTime();
	if (now - m_birthTime > m_totalTime)
	{
		return false;
	}
	m_pDirDecal->setPosition(WorldPos(m_startPos));
	m_pDirDecal->update(dtick);

	return true;
}

void TerreneTraceEffect::onStart()
{
	GameMap *map = ActorManager::getSingleton().getMainPlayer()->getCurMap();
	if (map == NULL)
	{
		return;
	}
	Ogre::ShowGameMap *showMap = map->getShowGameMap();
	if (showMap == NULL)
	{
		return;
	}
	Ogre::GameScene *gameScene = showMap->getGameScene();
	if (gameScene == NULL)
	{
		return;
	}
	m_birthTime = timeGetTime();
	m_pDirDecal->attachToScene(gameScene, false);
	m_pDirDecal->setPosition(WorldPos(m_startPos));
	m_pDirDecal->playTrace(m_startPos, m_endPos, m_extendTime,
		(float)m_totalTime, (float)m_fadeTime);

	m_pDirDecal->setRebuild(true);
	m_pDirDecal->update(0);
}

void TerreneTraceEffect::destory()
{
	if (m_pDirDecal != NULL)
	{
		m_pDirDecal->detachFromScene();
		OGRE_RELEASE(m_pDirDecal);
	}
}

unsigned int TerreneTraceEffect::cleanup()
{
	if (0 == m_birthTime)
	{
		return 0;
	}
	int time = m_totalTime - (timeGetTime() - m_birthTime);
	if (time < 0)
	{
		return 0;
	}
	return time;
}
