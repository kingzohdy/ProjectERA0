
#pragma once
#include "OgreGroundGrid.h"

class CSceneGround : public Ogre::GroundGrid
{
public:
	CSceneGround(bool ztest=false);
	~CSceneGround();

	enum GRIDSIZE
	{
		SIZE_SMALL,
		SIZE_MIDDLE,
		SIZE_BIG
	};

	//virtual void BuildDecalMesh(const Ogre::BoxBound& aabbBox, Ogre::Vector3* pVB, Ogre::ushort* pIB, 
	//	int nVertexBase, int nFaceLimit, int& nVertexCount, int& nFaceCount);

	float m_width;
	float m_length;
	GRIDSIZE m_gridSize;

	void SetGridSize(GRIDSIZE gridSize) { m_gridSize = gridSize; };
	GRIDSIZE GetGridSize() { return m_gridSize; };

	void setGroundColor(Ogre::ColorQuad color)
	{
	}
};

