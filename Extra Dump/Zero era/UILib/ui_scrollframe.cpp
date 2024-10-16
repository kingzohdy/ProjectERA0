#include <math.h>
#include "ui_scrollframe.h"
#include "OgreScriptLuaVM.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// ScrollFrame control
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

IMPLEMENT_UIOBJECT_CLONE(ScrollFrame)

ScrollFrame::ScrollFrame() : m_pFontString(NULL)
{
	m_nMaxVerticalScrollNum		= 0;
	m_nMaxHorizonalScrollNum	= 0;
	m_nOffsetX					= 0;
	m_nOffsetY					= 0;
	m_nLastVerticalScrollNum	= 0;
	m_nLastHorizonalScrollNum	= 0;
	m_nValueStep				= 1;
	m_nCurChangeY				= 0;
	m_bAdjust					= false;
	m_bReCalAbs					= false;
	m_nVerticalScrollRange		= 0;
	m_nHorizonalScrollRange		= 0;
	m_nVerticalHaveScrollDist	= 0;
	m_nHorizonalHaveScrollDist	= 0;
	m_nPreChildNum				= 0;
	SetClipState( true );
}

ScrollFrame::~ScrollFrame()
{
	OGRE_RELEASE( m_pFontString );
}

void ScrollFrame::CopyMembers(ScrollFrame* pdest)
{
	if (pdest)
	{
		Frame::CopyMembers(pdest);
	}
}

void ScrollFrame::FillDrawItems()
{
	if (m_bShow)
	{
		if (m_pParent)
		{
			m_iDrawLevel = m_pParent->m_iDrawLevel + 1;
		}

		g_pFrameMgr->AddDrawItems(static_cast<LayoutFrame *>(this));
		FillDrawLevel();
		// TODO: �ü��ؼ����ӿؼ�����ȫ�ɸ����ڿ���
		//Frame::FillChildren();
	}
}

void ScrollFrame::Draw()
{	
	// ֻ������Ч��Χ�Ż���л���
	Frame::Draw();

	if ( m_Style & FRMSTYLE_CLIPSELF)
	{
		g_pDisplay->PushClipRect( RectFloat( ( float )m_AbsRect.m_Left, ( float )m_AbsRect.m_Top,
			( float )m_AbsRect.m_Right, ( float )m_AbsRect.m_Bottom ) );
	}

	for ( size_t i=0, size = m_Children.size(); i < size; ++i )
	{
		Frame* pFrame = m_Children[i];
		if ( CanDraw( m_Children[i]->m_AbsRect ) && m_Children[i]->IsShown() )
		{
			m_Children[i]->Draw();
		}
	}

	m_pFontString->Draw();

	if ( m_Style & FRMSTYLE_CLIPSELF )
	{
		g_pDisplay->PopClipRect();
	}
}

bool ScrollFrame::CanDraw( const Ogre::RectInt& srcRect )
{
	const RectFloat& clipRect = g_pDisplay->GetClipRect();
	if ( (float)srcRect.m_Bottom <= clipRect.m_Top || (float)srcRect.m_Top >= clipRect.m_Bottom ||
			(float)srcRect.m_Left >= clipRect.m_Right || (float)srcRect.m_Right <= clipRect.m_Left )
	{
		return false; 
	}

	return true;
}

TiXmlElement *ScrollFrame::Save(TiXmlElement *pElement)
{
	TiXmlElement *pScrollFrame = Frame::Save(pElement);

	if ( !m_Children.empty() )
	{
		TiXmlElement *pScrollChild = new TiXmlElement("ScrollChild");
		pScrollFrame->LinkEndChild(pScrollChild);

		for (size_t i=0; i<m_Children.size(); i++)
		{
			m_Children[i]->Save(pScrollChild);
		}
	}

	return pScrollFrame;
}

void ScrollFrame::FindFrameOnPoint( int mousex, int mousey, std::vector<Frame *>& vFrameTemp )
{
	if ( !PointInLayoutFrame(mousex, mousey) )
	{
		return;
	}

	return Frame::FindFrameOnPoint( mousex, mousey, vFrameTemp );
}

void ScrollFrame::CalFrameRectRecursive(unsigned int iFrame)
{
	Frame::CalFrameRectRecursive( iFrame );
	//ResetChildAnchor();
	m_bReCalAbs = true;
}

int ScrollFrame::OnInputMessage(const Ogre::InputEvent &event)
{
	switch(event.msg)
	{
	case GIE_LBTNDOWN:
		{
			if (m_bShow)
			{
				m_State |= FRMSTATE_CLICKDOWN;
			}
			return 0;
		}

	case GIE_LBTNUP:
		{
			if( m_State & FRMSTATE_CLICKDOWN )
			{	
				m_State &= ~FRMSTATE_CLICKDOWN;	

				if (hasScriptsEvent(SC_ONCLICK))
				{
					CallScript(SC_ONCLICK);
				}
			}
			return 0;
		}

	case GIE_MOUSEWHEEL:
		{
			if ( m_pParent != NULL )
			{
				return m_pParent->OnInputMessage( event );
			}
			break;
		}

	default:
		{
			return Frame::OnInputMessage(event);
		}

	}
	return 1;
}

void ScrollFrame::AdjustFrameByViewPoint()
{
	m_AbsRect.m_Top		+= m_ViewStartPoint.y;
	m_AbsRect.m_Left	+= m_ViewStartPoint.x;
	m_AbsRect.m_Bottom	+= m_ViewStartPoint.y;
	m_AbsRect.m_Right	+= m_ViewStartPoint.x;
	
	m_ViewStartPoint.x = 0;
	m_ViewStartPoint.y = 0;
}

void ScrollFrame::AdjustChildren()
{
	for ( int i = 0, size = (int)m_Children.size(); i < size; ++i )
	{
		Frame* pFrame = m_Children[i];
		int x = pFrame->m_AbsRect.m_Left - m_AbsRect.m_Left;
		int y = pFrame->m_AbsRect.m_Top	 - m_AbsRect.m_Top;
		pFrame->SetPoint( "topleft", GetName(), "topleft",(int) (x / g_pFrameMgr->GetScreenScaleY()),
							(int) (y / g_pFrameMgr->GetScreenScaleY()) );
		if ( std::find( m_RelFrames.begin(), m_RelFrames.end(), pFrame ) == m_RelFrames.end() )
		{
			m_RelFrames.push_back( pFrame );
		}
	}
	
	CalVerticalScrollRange();
	CalHorizonalScrollRange();
	m_bAdjust = true;
	m_nPreChildNum = (int)m_Children.size();
}

void ScrollFrame::reCalChildrenAbs()
{
	for ( int i = 0, size = (int)m_Children.size(); i < size; ++i )
	{
		Frame* pFrame = m_Children[i];
		if ( std::find( m_RelFrames.begin(), m_RelFrames.end(), pFrame ) == m_RelFrames.end() )
		{
			m_RelFrames.push_back( pFrame );
		}
	}

	m_bReCalAbs = false;
}

void ScrollFrame::CalVerticalScrollRange()
{
	// ���ҵ�����������ϽǶ�����С��ֵ����СֵΪ������
	// ���ҵ�����������ϽǶ�������ֵ�����ֵΪ������
	// ���ּ�������ӿؼ��������ScrollFrame��topleft��topleft

	float fShortestDist = 0.0f;
	float fLongestDist	= 0.0f;
	for( int i = 0, size = (int)m_Children.size(); i < size; ++i )
	{	
		Frame* pFrame = m_Children[i];
		if( !pFrame->IsShown() )
		{
			continue;
		}

		LayoutDim& anchorOffset = pFrame->m_Anchor[0].m_Offset;
		if( fShortestDist > anchorOffset.GetY() )
		{
			fShortestDist = anchorOffset.GetY();
		}

		if( fLongestDist < ( anchorOffset.GetY() + pFrame->GetHeight() ) )
		{
			fLongestDist = anchorOffset.GetY() + pFrame->GetHeight();
		}
	}

	float fTotalScrollDist	= abs( fLongestDist - fShortestDist ) - GetHeight();
	// ������Ϊ��׼���м��㣬�����Թ�������Ϊ��׼����Ϊ���տ����������صı���Ч��
	m_nMaxVerticalScrollNum = (int) (fTotalScrollDist < 0 ? 0 : ceil( fTotalScrollDist / m_nValueStep ));
	m_nVerticalScrollRange	= m_nMaxVerticalScrollNum * m_nValueStep;
}

void ScrollFrame::CalHorizonalScrollRange()
{
	// ���ҵ�����������ϽǶ�����С��ֵ����СֵΪ������
	// ���ҵ�����������ϽǶ�������ֵ�����ֵΪ������
	// ���ּ�������ӿؼ��������ScrollFrame��topleft��topleft

	float fShortestDist = 0.0f;
	float fLongestDist	= 0.0f;
	for( int i = 0, size =(int) m_Children.size(); i < size; ++i )
	{	
		Frame* pFrame = m_Children[i];
		if( !pFrame->IsShown() )
		{
			continue;
		}

		LayoutDim& anchorOffset = pFrame->m_Anchor[0].m_Offset;
		if( fShortestDist > anchorOffset.GetX() )
		{
			fShortestDist = anchorOffset.GetX();
		}

		if( fLongestDist < ( anchorOffset.GetX() + pFrame->GetWidth() ) )
		{
			fLongestDist = anchorOffset.GetX() + pFrame->GetWidth();
		}
	}

	float fTotalScrollDist		= abs( fLongestDist - fShortestDist ) - GetWidth();
	// ������Ϊ��׼���м��㣬�����Թ�������Ϊ��׼����Ϊ���տ����������صı���Ч��
	m_nMaxHorizonalScrollNum	=(int) (fTotalScrollDist < 0 ? 0 : ceil( fTotalScrollDist / m_nValueStep ));
	m_nHorizonalScrollRange		= m_nMaxHorizonalScrollNum * m_nValueStep;
}

void ScrollFrame::SetValueStep(int value)
{
	m_nValueStep = value;
}

int ScrollFrame::GetValueStep()
{
	return m_nValueStep;
}

// ����ScrollFrame��slider�ӿؼ�
void ScrollFrame::SetScrollChildRect( float fX, float fY )
{
	IncreaseScrollChildRect( fX - m_nOffsetX, fY - m_nOffsetY );
}

void ScrollFrame::IncreaseScrollChildRect( float fOffsetX, float fOffsetY )
{
	// ���ǵ���ʱ��䶯�ķ�Χ���ܻ�����1�Σ�������С��λ�ı䶯��ΧҲ������м������ز���������ʾ
	ClampHorizonalScroll( fOffsetX );
	ClampVerticalScroll( fOffsetY );
	
	// �ڽ������λ������ǰ��һ��Ҫ��֤���ݸ����Ĳ����Ѿ�����Ч��Χ������������
	for( size_t i = 0, size = m_Children.size(); i < size; ++i )
	{	
		LayoutAnchor& anchor = m_Children[i]->m_Anchor[0];
		m_Children[i]->SetPoint( anchor.m_MyPoint, anchor.GetRelFrame().c_str(), anchor.m_RelPoint, 
									(int)(anchor.m_Offset.GetX() + ( -m_nCurChangeX * m_nValueStep )),
									(int)(anchor.m_Offset.GetY() + ( -m_nCurChangeY * m_nValueStep )) );
	}
}

void ScrollFrame::SetVerticalScroll( float value )
{
	SetScrollChildRect( 0, value );
}

void ScrollFrame::IncreaseVerticalScroll( float value )
{
	IncreaseScrollChildRect( 0, value );
}

int ScrollFrame::GetVerticalOffset()
{
	return m_nOffsetY;
}

void ScrollFrame::SetVerticalScrollRange( int nRange )
{
	m_nMaxVerticalScrollNum = nRange;
}

int ScrollFrame::GetVerticalScrollRange()
{
	return m_nMaxVerticalScrollNum;
}

void ScrollFrame::SetHorizonalScroll( float value )
{
	SetScrollChildRect( value, 0 );
}

void ScrollFrame::IncreaseHorizonalScroll( float value )
{
	IncreaseScrollChildRect( value, 0 );
}

int	ScrollFrame::GetHorizonalOffset()
{
	return m_nOffsetX;
}

void ScrollFrame::SetHorizonalScrollRange( int nRange )
{
	m_nHorizonalScrollRange = nRange;
}

int ScrollFrame::GetHorizonalScrollRange()
{
	return m_nHorizonalScrollRange;
}

// clampˮƽλ��
void ScrollFrame::ClampHorizonalScroll( float fOffsetX )
{
	// clampˮƽ���ؾ���
	// ���������ˮƽ�������뷶Χ
	int nChangeX = (int)( FloatToInt( abs( fOffsetX ) ) * ( fOffsetX / abs( fOffsetX ) ) );
	if ( ( nChangeX * m_nValueStep ) + m_nHorizonalHaveScrollDist > m_nHorizonalScrollRange )
	{
		// Ҫ��֤����ƶ��ľ�������ܹ�����ValueStep
		nChangeX = ( m_nHorizonalScrollRange - m_nHorizonalHaveScrollDist ) / m_nValueStep;
	}

	if ( ( nChangeX * m_nValueStep ) + m_nHorizonalHaveScrollDist < 0 )
	{
		nChangeX = -m_nHorizonalHaveScrollDist / m_nValueStep;
	}

	m_nCurChangeX				= nChangeX;
	m_nHorizonalHaveScrollDist	+= m_nCurChangeX * m_nValueStep;
	m_nOffsetX					+= m_nCurChangeX;
}

void ScrollFrame::ClampVerticalScroll( float fOffsetY )
{	
	// clamp��ֱ���ؾ���
	// �����������ֱ�������뷶Χ
	int nChangeY = (int)( FloatToInt( abs( fOffsetY ) ) * ( fOffsetY / abs( fOffsetY ) ) );
	if ( ( nChangeY * m_nValueStep ) + m_nVerticalHaveScrollDist > m_nVerticalScrollRange )
	{
		// Ҫ��֤����ƶ��ľ�������ܹ�����ValueStep
		nChangeY = ( m_nVerticalScrollRange - m_nVerticalHaveScrollDist ) / m_nValueStep;
	}

	if ( ( nChangeY * m_nValueStep ) + m_nVerticalHaveScrollDist < 0 )
	{
		nChangeY = -m_nVerticalHaveScrollDist / m_nValueStep;
	}
	
	m_nCurChangeY				= nChangeY;
	m_nVerticalHaveScrollDist	+= m_nCurChangeY * m_nValueStep;
	m_nOffsetY					+= m_nCurChangeY;
}

void ScrollFrame::UpdateSelf( float deltatime )
{
	if ( !m_bShow )
	{
		return;
	}

	Frame::UpdateSelf( deltatime );
	if ( !m_bAdjust || m_nPreChildNum != m_Children.size() )
	{
		AdjustChildren();
	}

	if ( m_bReCalAbs )
	{
		reCalChildrenAbs();
	}
	
	// ��ֱ������ʱ����Ӧ
	if ( m_nCurChangeY != 0 && hasScriptsEvent(SC_ONVERTICALSCROLL) )
	{
		CallScript( SC_ONVERTICALSCROLL, "i", m_nCurChangeY );
		m_nCurChangeY = 0;
	}
	
	// ˮƽ������ʱ����Ӧ
	if ( m_nCurChangeX != 0 && hasScriptsEvent(SC_ONHORIZONTALSCROLL) )
	{
		CallScript( SC_ONHORIZONTALSCROLL, "i", m_nCurChangeX );
		m_nCurChangeX = 0;
	}

	if ( m_nLastHorizonalScrollNum != m_nMaxHorizonalScrollNum )
	{
		if ( hasScriptsEvent(SC_ONSCROLLRANGECHANGED) )
		{
			CallScript( SC_ONSCROLLRANGECHANGED, "i", m_nMaxHorizonalScrollNum );
		}
		m_nLastHorizonalScrollNum = m_nMaxHorizonalScrollNum;
	}

	if ( m_nLastVerticalScrollNum != m_nMaxVerticalScrollNum )
	{
		if ( hasScriptsEvent(SC_ONSCROLLRANGECHANGED) )
		{
			CallScript( SC_ONSCROLLRANGECHANGED, "i", m_nMaxVerticalScrollNum );
		}
		m_nLastVerticalScrollNum = m_nMaxVerticalScrollNum;
	}
}

/*
//-----------------------------------------------------------------------------
// ScrollFrame lua function
//-----------------------------------------------------------------------------
DECLARE_CLASS_TO_SCRIPT_EX(ScrollFrame, Frame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

//ScrollFrame
DECLARE_MEM_FUNC_TO_SCRIPT(ScrollFrame, UpdateScrollChildRect);
//DECLARE_MEM_FUNC_TO_SCRIPT(ScrollFrame, GetVerticalScroll, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(ScrollFrame, SetVerticalScroll, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(ScrollFrame, GetVerticalScrollRange, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(ScrollFrame, SetOffset, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(ScrollFrame, GetVerticalOffset, SVT_INT);

*/
