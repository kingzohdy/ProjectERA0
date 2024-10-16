#pragma once
#include "ui_common.h"
//#include "ui_enumdef.h"

//支持相对和绝对坐标
class LayoutDim
{
public:
	~LayoutDim();
	LayoutDim(unsigned int x, unsigned int y);
	LayoutDim();
	void SetAbsDim(int x, int y);
	void SetRelDim(float x, float y);

	float GetX();
	float GetY();

	LayoutDim &operator=(const LayoutDim &src)
	{
		m_bRelDim = src.m_bRelDim;

		m_x = src.m_x;
		m_y = src.m_y;

		return *this;
	}

	bool operator!=(const LayoutDim &src)
	{
		if ((m_x == src.m_x) && (m_y == src.m_y))
		{
			return false;
		}
		return true;
	}

	bool m_bRelDim;
private:

	float m_x;
	float m_y;
};

//-----------------------------------------------------------------------------
// Layout Anchor
//-----------------------------------------------------------------------------
class LayoutAnchor
{
public:
	LayoutAnchor();
	~LayoutAnchor();
	bool SetPoint(FRAMEPOINT_T mypt, FRAMEPOINT_T relpt, const LayoutDim &offset);
	void SetRelFrame(std::string frameName);
	std::string GetRelFrame();

	LayoutAnchor &operator=(const LayoutAnchor &src)
	{
		m_MyPoint = src.m_MyPoint;
		m_RelPoint = src.m_RelPoint;
		m_RelFrame = src.m_RelFrame;

		m_Offset = src.m_Offset;

		return *this;
	}

public:
	FRAMEPOINT_T m_MyPoint;
	FRAMEPOINT_T m_RelPoint;
	std::string m_RelFrame;
	LayoutDim m_Offset;
};