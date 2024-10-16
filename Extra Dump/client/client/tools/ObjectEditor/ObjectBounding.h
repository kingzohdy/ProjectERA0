
#pragma once
#include "OgreVector3.h"
#include "OgreMatrix3.h"

class CBounding
{
public:
	CBounding(void);
	~CBounding(void);

	void SetBounding(Ogre::BoxBound* pBound);
	Ogre::BoxBound* CreateBoundVolume();
	bool IsValid(){ return m_isValid;};

	bool m_isValid;
	Ogre::Vector3 m_Center;
	Ogre::Vector3 m_Extent;
	Ogre::Matrix3 m_Xform;
	float m_Radius;
	float m_Height;


	void ShowAABB(bool bShow);
	bool m_bShowAABB;

	void DoFrame(Ogre::uint dtime);
};
