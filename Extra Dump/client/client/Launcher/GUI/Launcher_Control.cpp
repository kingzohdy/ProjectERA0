#include "stdafx.h"

CClient_Control::CClient_Control()
{
	m_Pos.x = m_Pos.y = 0;
	m_hDestDC = 0;
}

void CClient_Control::SetDestDC(HDC hDestDC)
{
	m_hDestDC = hDestDC;
}

void CClient_Control::SetPos(int nX, int nY)
{
	m_Pos.x = nX;
	m_Pos.y = nY;
}
int CClient_Control::X(void)
{
	return m_Pos.x;
}
int CClient_Control::Y(void)
{
	return m_Pos.y;
}

CClient_BitmapControl::CClient_BitmapControl()
{
	m_pImage = NULL;
}

void CClient_BitmapControl::SetImage(CClient_Image* pImage)
{
	m_pImage = pImage;
}
CClient_Image* CClient_BitmapControl::Image(void)
{
	return m_pImage;
}
