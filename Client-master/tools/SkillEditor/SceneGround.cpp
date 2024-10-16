#include "StdAfx.h"
#include "SceneGround.h"

CSceneGround::CSceneGround(void)
{
	m_gridSize = SIZE_SMALL;
	m_width = 0;
	m_length = 0;

	setGridGround(1.0f, 1.0f, 100, 100);
}

CSceneGround::~CSceneGround(void)
{
}
