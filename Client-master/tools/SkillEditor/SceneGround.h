#pragma once
#include "OgreGroundGrid.h"

class CSceneGround: public Ogre::GroundGrid
{
public:
	CSceneGround(void);
	~CSceneGround(void);

	enum GRIDSIZE
	{
		SIZE_SMALL,
		SIZE_MIDDLE,
		SIZE_BIG
	};

	float m_width;
	float m_length;
	GRIDSIZE m_gridSize;

	void SetGridSize(GRIDSIZE gridSize) { m_gridSize = gridSize; };
	GRIDSIZE GetGridSize() { return m_gridSize; };

	void setGroundColor(Ogre::ColorQuad color)
	{
	}
};
