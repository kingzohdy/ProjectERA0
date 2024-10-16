
#include "stdafx.h"
#include "EditorGround.h"
#include <vector>

CSceneGround::CSceneGround(bool ztest) : Ogre::GroundGrid(ztest)
{
	m_gridSize = SIZE_SMALL;
	m_width = 0;
	m_length = 0;

	setGridGround(1.0f, 1.0f, 100, 100);
}

CSceneGround::~CSceneGround()
{
}
/*
void CSceneGround::BuildDecalMesh(const Ogre::BoxBound& aabbBox, Ogre::Vector3* pVB, Ogre::ushort* pIB, 
								  int nVertexBase, int nFaceLimit, int& nVertexCount, int& nFaceCount)
{
	nVertexCount = 0;
	nFaceCount = 0;

	if(nFaceLimit < 2)
		return;

	nFaceCount = 2;
	nVertexCount = 4;
	pVB[0] = Ogre::Vector3(-m_width/2, -0.1f,  m_length/2);
	pVB[1] = Ogre::Vector3(-m_width/2, -0.1f,  -m_length/2);
	pVB[2] = Ogre::Vector3(m_width/2, -0.1f,  m_length/2);
	pVB[3] = Ogre::Vector3(m_width/2, -0.1f, -m_length/2);

	pIB[0] = nVertexBase + 0;
	pIB[1] = nVertexBase + 1;
	pIB[2] = nVertexBase + 2;

	pIB[3] = nVertexBase + 1;
	pIB[4] = nVertexBase + 2;
	pIB[5] = nVertexBase + 3;
}
*/
