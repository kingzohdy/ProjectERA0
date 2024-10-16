#pragma warning( disable : 4996 )

#include "ui_drawlineframe.h"
#include "OgreUIRenderer.h"
#include "ui_framemgr.h"

extern Ogre::UIRenderer* g_pDisplay;
extern FrameManager *g_pFrameMgr;

IMPLEMENT_UIOBJECT_CLONE(DrawLineFrame)

DrawLineFrame::~DrawLineFrame()
{
	//m_PointList.clear();
	m_LineColor.set( 0xff, 0xff, 0xff );
	g_pDisplay->ReleaseUIRes(m_hTex);
}

void DrawLineFrame::CopyMembers(DrawLineFrame *pdest)
{
	if ( pdest != NULL )
	{
		LayoutFrame::CopyMembers(pdest);
		pdest->m_LineColor			= m_LineColor;
		pdest->m_pointList			= m_pointList;
		pdest->m_hTex				= m_hTex;
		g_pDisplay->AddRef( pdest->m_hTex );
		pdest->m_iTexX				= m_iTexX;
		pdest->m_iTexY				= m_iTexY;
		pdest->m_iTexWidth			= m_iTexWidth;
		pdest->m_iTexHeight			= m_iTexHeight;
	}
}

void DrawLineFrame::UpdateSelf(float deltatime)
{
	/*if ( !m_bShow )
	{
	return;
	}

	m_AbsRect.m_Top		+= m_ViewStartPoint.y;
	m_AbsRect.m_Left	+= m_ViewStartPoint.x;
	m_AbsRect.m_Bottom	+= m_ViewStartPoint.y;
	m_AbsRect.m_Right	+= m_ViewStartPoint.x;*/
}


void DrawLineFrame::Draw()
{
	// 绘制多个点的路线
	// 绘制特点,拐角点容器中的第一个拐角点总是不绘制,总是先从平滑点中的第一个点开始绘制,因为平滑点中的第一个总是主角的当前位置
	if ( !m_pointList.empty() )
	{	
		const int POINT_SIZE	= 12;
// 		LayoutFrame* pRelFrame	= ( LayoutFrame* )g_pFrameMgr->FindLayoutFrame( "WorldMapFrameMapTexture" );
		int x = 0, y = 0;
		x = m_AbsRect.m_Left;
		y = m_AbsRect.m_Top;
// 		pRelFrame->GetFramePoint( FP_TOPLEFT, x, y );
		g_pDisplay->BeginDraw( m_hTex );

		for ( std::deque<Ogre::Point2D>::iterator iter = m_pointList.begin(), end = m_pointList.end();
			iter != end; ++iter )
		{
			g_pDisplay->StretchRect((float)(FloatToInt( iter->x * g_pFrameMgr->GetScreenScaleY() ) + x - POINT_SIZE / 2), 
				(float)(FloatToInt( iter->y * g_pFrameMgr->GetScreenScaleY() ) + y - POINT_SIZE / 2 ), 
				POINT_SIZE * g_pFrameMgr->GetScreenScaleY(), POINT_SIZE * g_pFrameMgr->GetScreenScaleY(), 
				m_LineColor.c, false, m_iTexX, m_iTexY, m_iTexWidth, m_iTexHeight );
		}
		g_pDisplay->EndDraw();
	}
}

void DrawLineFrame::AddPoint( int nX, int nY )
{
	m_pointList.push_back( Ogre::Point2D(nX,nY) );	
}