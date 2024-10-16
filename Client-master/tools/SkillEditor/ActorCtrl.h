#pragma once
#include "OgreSingleton.h"
#include "OgreInputHandler.h"
#include "GameEngineTypes.h"

class CActorCtrl: public Ogre::Singleton<CActorCtrl>, public Ogre::InputHandler
{
public:
	CActorCtrl(void);
	~CActorCtrl(void);

public:
	virtual int HandleInput(const Ogre::InputEvent &event);
	virtual void update(long lTick);
	
	void showSelectEffect( int id );
private:
	Ogre::Entity*	m_pSelEntity;
	int				m_PreSelId;
	int				m_InUsedSkill;
};
