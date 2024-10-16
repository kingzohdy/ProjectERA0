#include "ui_layoutanchor.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// LayoutDim
//-----------------------------------------------------------------------------

LayoutDim::LayoutDim()
{
	m_bRelDim = false;
	m_x = m_y = 0;
}

LayoutDim::LayoutDim(unsigned int x, unsigned int y)
{
	m_bRelDim = false;

	m_x = (float)x;
	m_y = (float)y;
}

LayoutDim::~LayoutDim()
{
}

void LayoutDim::SetAbsDim(int x, int y)
{
	m_bRelDim = false;

	m_x = (float)x;
	m_y = (float)y;
}

void LayoutDim::SetRelDim(float x, float y)
{
	m_bRelDim = true;

	m_x = x;
	m_y = y;
}

float LayoutDim::GetX()
{
	return m_x;
}

float LayoutDim::GetY()
{
	return m_y;
}

//-----------------------------------------------------------------------------
// LayoutAnchor
//-----------------------------------------------------------------------------
LayoutAnchor::LayoutAnchor()
{
	m_MyPoint = m_RelPoint = FP_TOPLEFT;
	m_RelFrame = "";
	m_Offset.SetAbsDim(0,0);
}

LayoutAnchor::~LayoutAnchor()
{
}


bool LayoutAnchor::SetPoint(FRAMEPOINT_T mypt, FRAMEPOINT_T relpt, const LayoutDim &offset)
{
	if ((m_Offset != offset) || (m_MyPoint != mypt) || (m_RelPoint != relpt))
	{
		m_MyPoint = mypt;
		m_RelPoint = relpt;

		m_Offset = offset;

		return true;
	}
	return false;
}

void LayoutAnchor::SetRelFrame(std::string frameName)
{
	m_RelFrame = frameName;
}

std::string LayoutAnchor::GetRelFrame()
{
	return m_RelFrame;
}
