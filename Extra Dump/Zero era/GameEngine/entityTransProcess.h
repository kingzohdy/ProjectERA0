
#pragma once

#include "OgreSingleton.h"
#include "OgreEntity.h"
#include <vector>

class TransEntity
{
public:
	TransEntity()
	{
		pEntity = NULL;
		fOriginTransparent = 1.0f;
	}
	~TransEntity()
	{
	}

	Ogre::Entity *pEntity;
	float fOriginTransparent;
};

// �Դ���Actor����͸����������� 
class EntityTransProcess : public Ogre::Singleton<EntityTransProcess>
{
public:
	EntityTransProcess();
	~EntityTransProcess();

	// һ�����ӵ�͸����
	void SetStepPerSecond (float step);

	void addEntity (Ogre::Entity *pEntity);
	void removeEntity (Ogre::Entity *pEntity);
	void clear ();

	void update (unsigned int tick);

private:
	float m_fStepPerSecond;
	std::vector<TransEntity*> m_TransEntities;
};