#pragma once
#include <utility>
#include "ui_layoutframe.h"
#include "OgreRect.h"
#include "OgreColourValue.h"
#include <deque>

class DrawLineFrame: public LayoutFrame{ //tolua_export
	DECLARE_UIOBJECT_CLONE(DrawLineFrame)
public:
	typedef std::deque<Ogre::Point2D>				POINTDEQUE;
	typedef std::deque< Ogre::Point2D >::iterator	POINTDEQUE_ITER;
	/*enum{ nNumSegment = 3, nFixLength = 10 };*/
	DrawLineFrame(): m_LineColor( 0xff, 0xff, 0xff ), m_hTex( NULL ), m_pointList( NULL )
	{
		m_iTexX = 0;
		m_iTexY = 0;
		m_iTexWidth = 0;
		m_iTexHeight = 0;
	}

	~DrawLineFrame();
	// »­Ïß½Ó¿Ú
	virtual void UpdateSelf(float deltatime);
	virtual void Draw();

	virtual const char *GetTypeName()
	{
		return "DrawLineFrame"; 
	}
	
	//tolua_begin
	void SetDrawPointList( std::deque<Ogre::Point2D> pPointList )
	{
		m_pointList = pPointList;
	}
	void ClearDrawPointList()
	{
		m_pointList.clear();
	}
	
	void AddPoint( int nX, int nY );
	//tolua_end

public:
	Ogre::ColorQuad				m_LineColor;
	Ogre::HUIRES				m_hTex;
	int							m_iTexX;
	int							m_iTexY;
	int							m_iTexWidth;
	int							m_iTexHeight;
private:
	std::deque<Ogre::Point2D>	m_pointList;
};//tolua_export