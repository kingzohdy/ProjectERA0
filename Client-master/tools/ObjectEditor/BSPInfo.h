
#pragma once
#include "OgrePrerequisites.h"

class CBsp
{
public:
	CBsp(void);
	~CBsp(void);
	//Ogre::BSPData m_Bsp;
	bool m_bIsValid;
	void SetBsp(Ogre::BSPData* pBsp);
	Ogre::BSPData* CreateBspData();
};
