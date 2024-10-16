
#include "stdafx.h"
#include "ObjectBounding.h"
#include "EditorManager.h"

#include "OgreLines.h"

CBounding::CBounding(void) : m_isValid(false)
{
	m_bShowAABB = false;
}

CBounding::~CBounding(void)
{
}

void CBounding::ShowAABB(bool bShow)
{
	m_bShowAABB = bShow;
}


void CBounding::SetBounding(Ogre::BoxBound* pBound)
{
	if(pBound == NULL)
	{
		m_isValid = false;
		return;
	}

	m_Center = pBound->getCenter();
	m_Extent = pBound->getExtension();
	m_isValid = true;
}

Ogre::BoxBound* CBounding::CreateBoundVolume()
{
	if(m_isValid == false)
		return NULL;

	Ogre::BoxBound* pBound = new Ogre::BoxBound(m_Center-m_Extent, m_Center+m_Extent);
	return pBound;
}

void CBounding::DoFrame(Ogre::uint dtime)
{
	if(m_bShowAABB && m_isValid)
	{
		Ogre::Vector3 vec[8];
		vec[0] = Ogre::Vector3(-m_Extent.x, -m_Extent.y, -m_Extent.z);
		vec[1] = Ogre::Vector3( m_Extent.x, -m_Extent.y, -m_Extent.z);
		vec[2] = Ogre::Vector3( m_Extent.x, -m_Extent.y,  m_Extent.z);
		vec[3] = Ogre::Vector3(-m_Extent.x, -m_Extent.y,  m_Extent.z);

		vec[4] = Ogre::Vector3(-m_Extent.x,  m_Extent.y, -m_Extent.z);
		vec[5] = Ogre::Vector3( m_Extent.x,  m_Extent.y, -m_Extent.z);
		vec[6] = Ogre::Vector3( m_Extent.x,  m_Extent.y,  m_Extent.z);
		vec[7] = Ogre::Vector3(-m_Extent.x,  m_Extent.y,  m_Extent.z);

		for(int i = 0; i < 8; i++)
		{
			vec[i] += m_Center;
		}

		Ogre::ColorQuad clr = Ogre::ColorQuad(255, 0, 0);

		Ogre::RenderLines* pDbg = CEnginAgent::GetInstance()->m_pLineRenderer;

		pDbg->addLine(vec[0], vec[1], clr.c);
		pDbg->addLine(vec[1], vec[2], clr.c);
		pDbg->addLine(vec[2], vec[3], clr.c);
		pDbg->addLine(vec[3], vec[0], clr.c);

		pDbg->addLine(vec[4], vec[5], clr.c);
		pDbg->addLine(vec[5], vec[6], clr.c);
		pDbg->addLine(vec[6], vec[7], clr.c);
		pDbg->addLine(vec[7], vec[4], clr.c);

		pDbg->addLine(vec[0], vec[4], clr.c);
		pDbg->addLine(vec[1], vec[5], clr.c);
		pDbg->addLine(vec[2], vec[6], clr.c);
		pDbg->addLine(vec[3], vec[7], clr.c);

	}
}