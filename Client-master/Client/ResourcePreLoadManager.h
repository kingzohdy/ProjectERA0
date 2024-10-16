#pragma once
#include "OgreSingleton.h"
#include "OgreResourceManager.h"
#include <vector>

//资源预加载管理类
class ResoucePreLoadManager
	: public Ogre::Singleton<ResoucePreLoadManager>
{
public:
	ResoucePreLoadManager():m_ThreadHandle(NULL), m_bQuitLoading(false){};
	~ResoucePreLoadManager();

	void loadPublicMotions();
	void startLoadResource();

private:
	std::vector<Ogre::Resource*> m_vResHandle;
	bool m_bQuitLoading;
	void* m_ThreadHandle;
};