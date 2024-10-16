
#pragma once
#include "OgreLines.h"

class CRotCood : public Ogre::RenderLines
{
public:
	CRotCood(void);
	~CRotCood(void);

	virtual void SetHighLight(bool bhighlight,float highlight) {};

	virtual void update(Ogre::uint dtick);

	struct LineVertex
	{
		Ogre::Vector3 pos;
		Ogre::ColorQuad color;
		Ogre::Vector2 uv;
	};

	float m_fHrot;
	float m_fVrot;
	float m_fLen;

	LineVertex m_circle[100];
	LineVertex m_arrow[2];
	LineVertex m_cood[6];

};
