#ifndef __GamePlayManager_H__
#define __GamePlayManager_H__

#include "OgreSingleton.h"
#include "OgreInputHandler.h"
#include "GameEngineTypes.h"
#include "GameSelectManager.h"
#include "proto_comm.h"
#include "proto_cs.h"
#include <list>

#define NUMSXBLOCKS 1024
struct tagPlayerCameraDir 
{
	unsigned int m_nAngle;
	unsigned int m_nMapId;
};

class GamePlayManager : public Ogre::Singleton<GamePlayManager>, public Ogre::InputHandler
{
public:
	GamePlayManager();
	~GamePlayManager();

	virtual int HandleInput(const Ogre::InputEvent &event);

	void doFrame(unsigned int dtick);
	void enterPworld( int nMapId );
	void leavePwolrd( int nMapId );
	void enterMap(GameMap *map, int x, int z);
	void leveMapSoon();
	void leaveMap();
	void ShowLoading(bool show);
	MainGamePlayer*	enterWorld(int mapid, int x, int z, int player_id, int player_resid);

	FollowCamera* getMainCamera()
	{
		return m_pMainCamera;
	}

	GameMap* getCurMap()
	{
		return m_pCurMap;
	}

	GameSelectManager* getSelectManager()
	{
		return &m_SelectMgr;
	}

	tagPlayerCameraDir* getPlayerCameraDir()
	{
		return &m_PlayerCameraDir;
	}
	
	void setCameraAngle( int nRealMapId );
	int getCameraAngle();

	MainGamePlayer* getMainPlayer();

	bool	isMapLoadOK();
	void	startFade();
	void	finishFade();
	void	preLoadMap(const CSENTERMAP& entermap);
	void	trySwapMap();

	void setFashionChangeAngle(float pullDist);
	void resetPlayAngle();

protected:
	FollowCamera*			m_pMainCamera;
	GameMap*				m_pCurMap;
	GameSelectManager		m_SelectMgr;
	int						m_CurMapID;
	bool					m_isInWorld;
	tagPlayerCameraDir		m_PlayerCameraDir;
	std::list<std::pair<uintptr_t, CSENTERMAP>>	m_LoadThread;
	bool					m_isFaded;
	bool					m_isLoading;
	Ogre::WorldPos m_prevTarget;
};

#endif