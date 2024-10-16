#pragma once
#include "OgreSingleton.h"
#include "OgreResourceManager.h"
#include <vector>

//��ԴԤ���ع�����
class ResoucePreLoadManager
	: public Ogre::Singleton<ResoucePreLoadManager>
{
public:
	ResoucePreLoadManager():m_ThreadHandle(NULL){};
	~ResoucePreLoadManager();

	void loadPublicMotions();
	void unloadPublicMontions();
	void startLoadResource();

private:
	std::vector<Ogre::Resource*> m_vResHandle;
	Ogre::LockSection m_Mutex;
	void* m_ThreadHandle;
};