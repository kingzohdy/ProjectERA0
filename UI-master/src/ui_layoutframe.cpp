#include "ui_frame.h"
#include "ui_layoutframe.h"
#include "ui_framemgr.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// Layout Frame
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

void tagRect_ToLua::empty()
{
	m_nLeft = m_nRight = m_nTop = m_nBottom = 0;
}

float tagRect_ToLua::getWidth()
{
	return m_nRight - m_nLeft;
}

float tagRect_ToLua::getHeight()
{
	return m_nBottom - m_nTop;
}

IMPLEMENT_UIOBJECT_CLONE(LayoutFrame)

LayoutFrame::LayoutFrame() : m_bShow(true), m_pParent(NULL)
{
	m_Size = LayoutDim();

	m_FrameStrata = FS_MEDIUM;
	m_iDrawLevel = 0;

	m_Inherits = "";
	m_bSetAllPoints = false;
	m_bInputTransparent = false;

	m_AbsRect = Ogre::RectInt(0,0,0,0);
	m_ClipRect = Ogre::RectInt(0,0,0,0);
	m_ViewStartPoint = Ogre::Point2D(0, 0);
	m_CalAbsRectFrame = 0;

	m_Anchor[0] = m_Anchor[1] = LayoutAnchor();
	m_Size = LayoutDim();

	m_ClientId = 0;
	m_ClientString = "";
	m_fSelfScale = 1.0f;
	m_bFrameDraw = true;

	memset(m_ClientUserData, 0, sizeof(m_ClientUserData));
}

LayoutFrame::~LayoutFrame()
{
}

void LayoutFrame::CopyMembers(LayoutFrame *pdest)
{
	if (pdest)
	{
		UIObject::CopyMembers(pdest);

		pdest->m_FrameStrata = m_FrameStrata;
		pdest->m_iDrawLevel = m_iDrawLevel;
		pdest->m_Inherits = m_Inherits;
		pdest->m_bSetAllPoints = m_bSetAllPoints;
		pdest->m_bShow = m_bShow;
		pdest->m_AbsRect = m_AbsRect;
		pdest->m_CalAbsRectFrame = m_CalAbsRectFrame;
		pdest->m_pParent = m_pParent;
		for (size_t i=0; i<MAX_ANCHOR; i++)
		{
			pdest->m_Anchor[i] = m_Anchor[i];
		}
		pdest->m_Size = m_Size;

		pdest->m_ClientId = m_ClientId;
	}
}

void LayoutFrame::SetFrameStrata(FRAMESTRATA_T strata)
{
	m_FrameStrata = strata;
}

FRAMESTRATA_T LayoutFrame::GetFrameStrata()
{
	return m_FrameStrata;
}

void LayoutFrame::SetLayOutSize(LayoutDim ld)
{
	m_Size = ld;

	g_pFrameMgr->AddReCalFrame( this );
	for (size_t i=0; i<m_RelFrames.size(); i++)
	{
		LayoutFrame* pFrame = m_RelFrames[i];
		g_pFrameMgr->AddReCalFrame( pFrame );
	}
}

void LayoutFrame::SetSizeNoRecal(int x, int y)
{
	LayoutDim ld;
	ld.SetAbsDim(x, y);
	if ( m_Size != ld )
	{
		m_Size = ld;
	}
}

void LayoutFrame::SetSize(int x, int y)
{
	LayoutDim ld;
	ld.SetAbsDim(x, y);
	/*if( m_Size != ld )
	{*/
	SetLayOutSize(ld);
	/*}*/
}

int LayoutFrame::GetTextExtentWidth( const char *name )
{
	float width, height;
	g_pDisplay->GetTextExtent(g_pFrameMgr->m_HMAYFONT[1].h, name, width, height);

	return FloatToInt(width);
}

int LayoutFrame::GetTextExtentHeight( const char *name )
{
	float width, height;
	g_pDisplay->GetTextExtent(g_pFrameMgr->m_HMAYFONT[1].h, name, width, height);

	return FloatToInt(height);
}

void LayoutFrame::SetWidth(int width)
{
	LayoutDim ld(width, (unsigned int)m_Size.GetY());
	SetLayOutSize(ld);
	//m_Size.SetAbsDim((int)m_Size.GetX(), height);
}

int LayoutFrame::GetWidth()
{
	return (int)m_Size.GetX();
}

void LayoutFrame::SetHeight(int height)
{
	LayoutDim ld((unsigned int)m_Size.GetX(), height);
	SetLayOutSize(ld);
	//m_Size.SetAbsDim((int)m_Size.GetX(), height);
}

int LayoutFrame::GetHeight()
{
	return (int)m_Size.GetY();
}

void LayoutFrame::MoveUp(float value)
{
	float offset = m_Anchor[0].m_Offset.GetY();
	offset += value;
	m_Anchor[0].m_Offset.SetAbsDim((int)m_Anchor[0].m_Offset.GetX(), (int)offset);

	offset = m_Anchor[1].m_Offset.GetY();
	offset += value;
	m_Anchor[1].m_Offset.SetAbsDim((int)m_Anchor[1].m_Offset.GetX(), (int)offset);
}

LayoutDim LayoutFrame::GetSize()
{
	return m_Size;
}

int LayoutFrame::GetLeft()
{
	return m_AbsRect.m_Left;
}

int LayoutFrame::GetRight()
{
	return m_AbsRect.m_Right;
}

int LayoutFrame::GetTop()
{
	return m_AbsRect.m_Top;
}

int LayoutFrame::GetBottom()
{
	return m_AbsRect.m_Bottom;
}

int LayoutFrame::GetRealHeight()
{
	return m_AbsRect.m_Bottom - m_AbsRect.m_Top;
}

int LayoutFrame::GetRealWidth()
{
	return m_AbsRect.m_Right - m_AbsRect.m_Left;
}

int LayoutFrame::GetRealLeft()
{
	return FloatToInt( m_AbsRect.m_Left / g_pFrameMgr->GetScreenScaleY() );
}

int LayoutFrame::GetRealRight()
{
	return FloatToInt( m_AbsRect.m_Right / g_pFrameMgr->GetScreenScaleY() );
}

int LayoutFrame::GetRealTop()
{
	return FloatToInt( m_AbsRect.m_Top / g_pFrameMgr->GetScreenScaleY() );
}

int LayoutFrame::GetRealBottom()
{
	return FloatToInt( m_AbsRect.m_Bottom / g_pFrameMgr->GetScreenScaleY() );
}


void LayoutFrame::setAbsRect(float fLeft,float fTop, float fRight, float fBottom)
{
	m_AbsRect.m_Left	= FloatToInt( fLeft );
	m_AbsRect.m_Top		= FloatToInt( fTop );
	m_AbsRect.m_Right	= FloatToInt( fRight );
	m_AbsRect.m_Bottom	= FloatToInt( fBottom );
}

void LayoutFrame::resizeRect( int Width, int Height )
{
	m_AbsRect.m_Right	= m_AbsRect.m_Left + Width;
	m_AbsRect.m_Top		= m_AbsRect.m_Bottom - Height;
}

void LayoutFrame::extendRect( int Width, int Height )
{
	m_AbsRect.m_Right	= m_AbsRect.m_Left + Width;
	m_AbsRect.m_Bottom	= m_AbsRect.m_Top + Height;
}

const char *LayoutFrame::GetParent()
{
	if (m_pParent)
	{
		return m_pParent->GetName();
	}

	return NULL;
}

void LayoutFrame::AddAnchor(const LayoutAnchor &anchor)
{
	m_Anchor[0] = anchor;
	//m_Anchors.push_back(anchor);
}

void LayoutFrame::Show()
{
	m_bShow = true;
	
	if (hasScriptsEvent(SC_ONSHOW))
	{
		CallScript(SC_ONSHOW);
	}
	
	if( m_FrameStrata == FS_DIALOG )
	{
		((Frame*)this)->AddLevelRecursive(1);
	}

	if( ( strcmp( this->GetTypeName(), "WebBrowerFrame") == 0 || strcmp( this->GetTypeName(), "Frame") == 0) 
			&& !((Frame*)this)->m_strShowSoundPath.empty() )
	{
		playUISound( ((Frame*)this)->m_strShowSoundPath.c_str() );
	}
}

void LayoutFrame::Active()
{
	if (!IsShown())
	{
		return;
	}

	if( m_FrameStrata == FS_DIALOG )
	{
		((Frame*)this)->AddLevelRecursive(1);
	}
}

void LayoutFrame::Hide()
{
	m_bShow = false;

	if (hasScriptsEvent(SC_ONHIDE))
	{
		CallScript(SC_ONHIDE);
	}

	if( ( strcmp( this->GetTypeName(), "WebBrowerFrame" ) == 0 || strcmp( this->GetTypeName(), "Frame") == 0 ) 
			&& !((Frame*)this)->m_strShowSoundPath.empty() )
	{
		playUISound( ((Frame*)this)->m_strHideSoundPath.c_str() );
	}
}

void LayoutFrame::DrawShow(bool bShow) //不掉用脚本消息
{
	if (bShow)
	{
		m_bShow = true;
	}
	else
	{
		m_bShow = false;
	}
}

bool LayoutFrame::IsShown()
{
	return m_bShow;
}

float LayoutFrame::GetAnchorOffsetX()
{
	return m_Anchor[0].m_Offset.GetX();
}

float LayoutFrame::GetAnchorOffsetY()
{
	return m_Anchor[0].m_Offset.GetY();
}

void LayoutFrame::removeRelFrames(LayoutFrame* pFrame)
{
	std::vector<LayoutFrame *>::iterator it = m_RelFrames.begin();
	if( it != m_RelFrames.end() )
	{
		m_RelFrames.erase( it );
	}
}

void LayoutFrame::SetPoint(const char *strpoint, const char *relFrame,
						   const char *strrelpoint, int xoffset, int yoffset)
{
	if (strpoint != NULL && relFrame != NULL && strrelpoint != NULL )
	{
		FRAMEPOINT_T point = FP_TOPLEFT;
		for (size_t i=0; i<sizeof( s_FramePoint ); i++)
		{
			if (i > FP_CENTER)
			{
				break;
			}
			if( stricmp(strpoint, s_FramePoint[i]) == 0 )
			{
				point = FRAMEPOINT_T(i);
				break;
			}
		}

		FRAMEPOINT_T relpoint = FP_TOPLEFT;
		for (size_t i=0; i<sizeof(s_FramePoint); i++)
		{
			if (i > 8)
			{
				break;
			}
			if(stricmp(strrelpoint, s_FramePoint[i]) == 0)
			{
				relpoint = FRAMEPOINT_T(i);
				break;
			}
		}

		LayoutDim ld;
		ld.SetAbsDim(xoffset, yoffset);
		//if (strcmp(m_Anchor[0].m_RelFrame.c_str(), relFrame))
		//{
		//	//g_pFrameMgr->m_bUpdate = true;
		//}
		if ( ( ( m_Anchor[0].SetPoint( point, relpoint, ld ) || strcmp( m_Anchor[0].m_RelFrame.c_str(), relFrame ) != 0 ) ) )
		{
			g_pFrameMgr->AddReCalFrame( this );
			for (size_t i=0; i<m_RelFrames.size(); i++)
			{
				LayoutFrame* pFrame = m_RelFrames[i];
				g_pFrameMgr->AddReCalFrame( pFrame );
			}
		}
		m_Anchor[0].m_RelFrame = relFrame;
	}
	else
	{
		assert(0);
	}
}

void LayoutFrame::SetPointAlwaysRecalc(const char *strpoint, const char *relFrame,
						   const char *strrelpoint, int xoffset, int yoffset)
{
	if (strpoint != NULL && relFrame != NULL && strrelpoint != NULL )
	{
		FRAMEPOINT_T point = FP_TOPLEFT;
		for (size_t i=0; i<sizeof( s_FramePoint ); i++)
		{
			if (i > FP_CENTER)
			{
				break;
			}
			if( stricmp(strpoint, s_FramePoint[i]) == 0 )
			{
				point = FRAMEPOINT_T(i);
				break;
			}
		}

		FRAMEPOINT_T relpoint = FP_TOPLEFT;
		for (size_t i=0; i<sizeof(s_FramePoint); i++)
		{
			if (i > 8)
			{
				break;
			}
			if(stricmp(strrelpoint, s_FramePoint[i]) == 0)
			{
				relpoint = FRAMEPOINT_T(i);
				break;
			}
		}

		LayoutDim ld;
		ld.SetAbsDim(xoffset, yoffset);
		m_Anchor[0].SetPoint( point, relpoint, ld );
		g_pFrameMgr->AddReCalFrame( this );
		for (size_t i=0; i<m_RelFrames.size(); i++)
		{
			LayoutFrame* pFrame = m_RelFrames[i];
			g_pFrameMgr->AddReCalFrame( pFrame );
		}
		m_Anchor[0].m_RelFrame = relFrame;
	}
}


void LayoutFrame::SetPointEx(const char *strpoint, const char *relFrame,
							 const char *strrelpoint, int xoffset, int yoffset)
{
	LayoutDim ld;
	ld.SetAbsDim(xoffset, yoffset);
	
	if ( strpoint == NULL || relFrame == NULL || strrelpoint == NULL )
	{
		return;
	}

	FRAMEPOINT_T point = FP_TOPLEFT;
	for (size_t i=0; i<sizeof( s_FramePoint ); i++)
	{
		if (i > FP_CENTER)
		{
			break;
		}
		if( stricmp(strpoint, s_FramePoint[i]) == 0 )
		{
			point = FRAMEPOINT_T(i);
			break;
		}
	}

	FRAMEPOINT_T relpoint = FP_TOPLEFT;
	for (size_t i=0; i<sizeof(s_FramePoint); i++)
	{
		if (i > 8)
		{
			break;
		}
		if(stricmp(strrelpoint, s_FramePoint[i]) == 0)
		{
			relpoint = FRAMEPOINT_T(i);
			break;
		}
	}
	m_Anchor[0].SetPoint( point, relpoint, ld );

	g_pFrameMgr->AddReCalFrame( this );

	if( strcmp( m_Anchor[0].m_RelFrame.c_str(),relFrame ) != 0 )
	{
		LayoutFrame* pRelFrames = static_cast<LayoutFrame*>(g_pFrameMgr->FindLayoutFrame( m_Anchor[0].m_RelFrame ));
		if( pRelFrames )
		{
			pRelFrames->removeRelFrames( this );
		}
		pRelFrames = static_cast<LayoutFrame*>(g_pFrameMgr->FindLayoutFrame(relFrame));
		if( pRelFrames )
		{
			pRelFrames->m_RelFrames.push_back( this );
		}
	}

	for ( size_t i = 0, size = m_RelFrames.size(); i < size; ++i )
	{
		LayoutFrame* pFrame = m_RelFrames[i];
		g_pFrameMgr->AddReCalFrame( pFrame );
	}

	m_Anchor[0].m_RelFrame = relFrame;
}


void LayoutFrame::SetPoint( FRAMEPOINT_T myPoint, const char* relFrame, FRAMEPOINT_T relPoint, 
							int xoffset, int yoffset)
{
	LayoutDim ld;
	ld.SetAbsDim(xoffset, yoffset);
	
	if ( ( ( m_Anchor[0].SetPoint( myPoint, relPoint, ld ) || 
				strcmp( m_Anchor[0].m_RelFrame.c_str(), relFrame ) != 0 ) ) )
	{
		g_pFrameMgr->AddReCalFrame( this );
		for ( size_t i = 0, size = m_RelFrames.size(); i < size; ++i )
		{
			LayoutFrame* pFrame = m_RelFrames[i];
			g_pFrameMgr->AddReCalFrame( pFrame );
		}
	}

	m_Anchor[0].m_RelFrame = relFrame;
}

void LayoutFrame::SetViewStartPoint(int x, int y)
{
	m_ViewStartPoint = Ogre::Point2D(x, y);
}

void LayoutFrame::IncreaseViewStartPoint(int x, int y)
{
	m_ViewStartPoint.x += x;
	m_ViewStartPoint.y += y;
}

void LayoutFrame::GetFramePoint(FRAMEPOINT_T pt, int &x, int &y)
{
	//assert(m_CalAbsRectFrame == g_pFrameMgr->GetCalAbsRectFrame());

	int w = m_AbsRect.getWidth();
	int h = m_AbsRect.getHeight();

	switch(pt)
	{
	case FP_TOPLEFT:
		{
			x = m_AbsRect.m_Left;
			y = m_AbsRect.m_Top;
			break;
		}

	case FP_TOPRIGHT:
		{
			x = m_AbsRect.m_Right;
			y = m_AbsRect.m_Top;
			break;
		}

	case FP_BOTTOMLEFT:
		{
			x = m_AbsRect.m_Left;
			y = m_AbsRect.m_Bottom;
			break;
		}

	case FP_BOTTOMRIGHT:
		{
			x = m_AbsRect.m_Right;
			y = m_AbsRect.m_Bottom;
			break;
		}

	case FP_TOP:
		{
			x = m_AbsRect.m_Left + w/2;
			y = m_AbsRect.m_Top;
			break;
		}

	case FP_BOTTOM:
		{
			x = m_AbsRect.m_Left + w/2;
			y = m_AbsRect.m_Bottom;
			break;
		}

	case FP_LEFT:
		{
			x = m_AbsRect.m_Left;
			y = m_AbsRect.m_Top + h/2;
			break;
		}

	case FP_RIGHT:
		{
			x = m_AbsRect.m_Right;
			y = m_AbsRect.m_Top + h/2;
			break;
		}

	case FP_CENTER:
		{
			x = m_AbsRect.m_Left + w/2;
			y = m_AbsRect.m_Top + h/2;
			break;
		}

	default:
		{
			assert(0 && "FRAMEPOINT_T type is not correct!");
			break;
		}
	}
}

void LayoutFrame::CalAbsRectSelf(size_t frame)
{
	m_CalAbsRectFrame = frame;
	
	LayoutFrame *prelframe = m_pParent;
	LayoutAnchor &anchor = m_Anchor[0];
	if ( !anchor.m_RelFrame.empty() )
	{
		prelframe = static_cast<LayoutFrame*>( g_pFrameMgr->FindLayoutFrame( anchor.m_RelFrame ) );
		if ( prelframe != NULL && frame != -1 )
		{
			if ( prelframe->GetCalAbsRectFrame() != frame )
			{
				prelframe->CalAbsRectSelf( frame );
			}
		}
	}
	GetAbsRect( m_AbsRect );
}

void LayoutFrame::GetAbsRect( Ogre::RectInt& rect )
{
	LayoutFrame *prelframe = m_pParent;
	LayoutAnchor &anchor = m_Anchor[0];
	if ( !anchor.m_RelFrame.empty() )
	{
		prelframe = static_cast<LayoutFrame*>( g_pFrameMgr->FindLayoutFrame( anchor.m_RelFrame ) );
	}

	int x, y;
	if ( ( prelframe == NULL ) )
	{
		g_pFrameMgr->GetFramePoint( anchor.m_RelPoint, x, y );
	}
	else
	{
		prelframe->GetFramePoint( anchor.m_RelPoint, x, y );		
	}

	LayoutDim& anchorOffsetConfig = anchor.m_Offset;
	if ( anchorOffsetConfig.m_bRelDim )
	{
		if ( m_pParent != NULL )
		{
			x += FloatToInt( anchorOffsetConfig.GetX() * m_pParent->m_AbsRect.getWidth() );
			y += FloatToInt( anchorOffsetConfig.GetY() * m_pParent->m_AbsRect.getHeight() );
		}
		else
		{
			x += FloatToInt( anchorOffsetConfig.GetX() * g_pFrameMgr->GetScreenWidth() );
			y += FloatToInt( anchorOffsetConfig.GetY() * g_pFrameMgr->GetScreenHeight() );
		}
	}
	else
	{
		x += FloatToInt( anchorOffsetConfig.GetX() * g_pFrameMgr->GetScreenScaleY() );
		y += FloatToInt( anchorOffsetConfig.GetY() * g_pFrameMgr->GetScreenScaleY() );
	}


	int w, h;
	if ( m_Size.m_bRelDim )
	{
		if ( m_pParent != NULL )
		{
			w = FloatToInt( m_Size.GetX() * m_pParent->m_AbsRect.getWidth() );
			h = FloatToInt( m_Size.GetY() * m_pParent->m_AbsRect.getHeight() );
		}
		else
		{
			w = FloatToInt( m_Size.GetX() * g_pFrameMgr->GetScreenWidth() );
			h = FloatToInt( m_Size.GetY() * g_pFrameMgr->GetScreenHeight() );
		}
	}
	else
	{
		w = FloatToInt( m_Size.GetX() * g_pFrameMgr->GetScreenScaleY() );
		h = FloatToInt( m_Size.GetY() * g_pFrameMgr->GetScreenScaleY() );
	}

	switch(anchor.m_MyPoint)
	{
	case FP_TOPLEFT:
		rect.m_Left = x;
		rect.m_Top = y;
		break;

	case FP_TOPRIGHT:
		rect.m_Left = x - w;
		rect.m_Top = y;
		break;

	case FP_BOTTOMLEFT:
		rect.m_Left = x;
		rect.m_Top = y - h;
		break;

	case FP_BOTTOMRIGHT:
		rect.m_Left = x - w;
		rect.m_Top = y - h;
		break;

	case FP_TOP:
		rect.m_Left = x - w/2;
		rect.m_Top = y;
		break;

	case FP_BOTTOM:
		rect.m_Left = x - w/2;
		rect.m_Top = y - h;
		break;

	case FP_LEFT:
		rect.m_Left = x;
		rect.m_Top = y - h/2;
		break;

	case FP_RIGHT:
		rect.m_Left = x - w;
		rect.m_Top = y - h/2;
		break;

	case FP_CENTER:
		rect.m_Left = x - w/2;
		rect.m_Top = y - h/2;
	}

	rect.m_Right = rect.m_Left + w;
	rect.m_Bottom = rect.m_Top + h;
}

bool LayoutFrame::PointInLayoutFrame(int x, int y)
{
	if ((m_AbsRect.m_Left<x)&&(x<m_AbsRect.m_Right) &&
		(m_AbsRect.m_Top<y) && (y<m_AbsRect.m_Bottom))
		return true;
	else
		return false;
}

void LayoutFrame::UpdateSelf(float deltatime)
{
}

void LayoutFrame::Draw()
{
}

void LayoutFrame::FillDrawItems()
{
}

TiXmlElement *LayoutFrame::Save(TiXmlElement *pElement)
{
	if (!strcmp(this->GetName(), UI_ROOT_NAME.c_str()))
	{
		return NULL;
	}

	TiXmlElement *pLayoutFrame = new TiXmlElement(GetTypeName());
	pElement->LinkEndChild(pLayoutFrame);

	//Name
	if (!m_Name.empty())
	{
		pLayoutFrame->SetAttribute("name", m_Name.c_str());
	}

	//inherits
	if (!m_Inherits.empty())
	{
		pLayoutFrame->SetAttribute("inherits", m_Inherits.c_str());
	}

	//Hidden
	if (!m_bShow)
	{
		pLayoutFrame->SetAttribute("hidden", "true");
	}

	//Parent
	if (m_pParent)
	{
		if(!strcmp(m_pParent->GetName(), UI_ROOT_NAME.c_str()))
		{
			pLayoutFrame->SetAttribute("parent", UI_ROOT_NAME.c_str());
		}
	}

	//Size
	TiXmlElement *pSize = new TiXmlElement("Size");
	pLayoutFrame->LinkEndChild(pSize);
	
	TiXmlElement *pAbsDimension = new TiXmlElement("AbsDimension");
	pSize->LinkEndChild(pAbsDimension);

	pAbsDimension->SetAttribute("x", FloatToInt(m_Size.GetX()));
	pAbsDimension->SetAttribute("y", FloatToInt(m_Size.GetY()));

	//Anchors
	TiXmlElement *pAnchors = new TiXmlElement("Anchors");
	pLayoutFrame->LinkEndChild(pAnchors);

	TiXmlElement *pAnchor = new TiXmlElement("Anchor");
	pAnchors->LinkEndChild(pAnchor);

	pAnchor->SetAttribute("point", s_FramePoint[m_Anchor[0].m_MyPoint]);

	if (!m_Anchor[0].m_RelFrame.empty())
	{
		if (m_pParent)
		{
			if (!strcmp(m_Anchor[0].m_RelFrame.c_str(), m_pParent->GetName()))
			{
				pAnchor->SetAttribute("relativeTo", "$parent");
			}
			else
			{
				pAnchor->SetAttribute("relativeTo", m_Anchor[0].m_RelFrame.c_str());
			}
		}
		else
		{
			pAnchor->SetAttribute("relativeTo", m_Anchor[0].m_RelFrame.c_str());
		}
		pAnchor->SetAttribute("relativePoint", s_FramePoint[m_Anchor[0].m_RelPoint]);
	}

	//Offset
	if ((m_Anchor[0].m_Offset.GetX() != 0) || (m_Anchor[0].m_Offset.GetY() != 0))
	{
		TiXmlElement *pOffset = new TiXmlElement("Offset");
		pAnchor->LinkEndChild(pOffset);

		if (!m_Anchor[0].m_Offset.m_bRelDim)
		{
			TiXmlElement *pAbsDimension = new TiXmlElement("AbsDimension");
			pOffset->LinkEndChild(pAbsDimension);
			pAbsDimension->SetAttribute("x", int(m_Anchor[0].m_Offset.GetX()));
			pAbsDimension->SetAttribute("y", int(m_Anchor[0].m_Offset.GetY()));
		}
		else
		{
			TiXmlElement *pRelDimension = new TiXmlElement("RelDimension");
			pOffset->LinkEndChild(pRelDimension);
			pRelDimension->SetAttribute("x", int(m_Anchor[0].m_Offset.GetX()));
			pRelDimension->SetAttribute("y", int(m_Anchor[0].m_Offset.GetY()));
		}

	}

	return pLayoutFrame;
}

//获取客户端相关数据
int LayoutFrame::GetClientID()
{
	return m_ClientId;
}

void LayoutFrame::SetClientID(int id)
{
	m_ClientId = id;
}

void LayoutFrame::SetClientUserData(int index, int data)
{
	m_ClientUserData[index] = data;
}

int LayoutFrame::GetClientUserData(int index)
{
	return m_ClientUserData[index];
}

void LayoutFrame::SetClientString(const char *str)
{
	m_ClientString = str;
}

const char *LayoutFrame::GetClientString()
{
	return m_ClientString.c_str();
}

// 获取窗口绘制级别
int LayoutFrame::getFrameDrawLevel()
{
	return m_iDrawLevel;
}

void LayoutFrame::setInputTransparent( bool bTransparent )
{
	m_bInputTransparent = bTransparent;
}

void LayoutFrame::AddRelFrame( LayoutFrame* pRelFrame )
{
	std::vector<LayoutFrame*>::iterator iter = std::find( m_RelFrames.begin(), m_RelFrames.end(), pRelFrame );
	if ( iter != m_RelFrames.end() )
	{
		return;
	}

	m_RelFrames.push_back( pRelFrame );
}

void LayoutFrame::SetSelfScale(float scale)
{ 
	m_fSelfScale = scale; 
}

void LayoutFrame::SetFrameDraw( bool bFrameDraw )
{
	m_bFrameDraw = bFrameDraw;
}

void LayoutFrame::MoveFrameAbsrect( int nOffsetX, int nOffsetY )
{
	m_AbsRect.m_Left	+= nOffsetX;
	m_AbsRect.m_Right	+= nOffsetX;
	m_AbsRect.m_Top		+= nOffsetY;
	m_AbsRect.m_Bottom	+= nOffsetY;
}

void LayoutFrame::AdjustFrameByViewPoint()
{
	m_AbsRect.m_Top		+= m_ViewStartPoint.y;
	m_AbsRect.m_Left	+= m_ViewStartPoint.x;
	m_AbsRect.m_Bottom	+= m_ViewStartPoint.y;
	m_AbsRect.m_Right	+= m_ViewStartPoint.x;

	m_ViewStartPoint.x = 0;
	m_ViewStartPoint.y = 0;
}

float LayoutFrame::getFrameSizeX()
{
	if( m_Size.m_bRelDim && m_pParent != NULL )
	{
		return m_Size.GetX() * m_pParent->m_AbsRect.getWidth();
	}else{
		return m_Size.GetX();
	}
}

float LayoutFrame::getFrameSizeY()
{
	if( m_Size.m_bRelDim && m_pParent != NULL )
	{
		return m_Size.GetY() * m_pParent->m_AbsRect.getHeight();
	}else{
		return m_Size.GetY();
	}
}

/*
//-----------------------------------------------------------------------------
// layoutframe lua function
//-----------------------------------------------------------------------------
DECLARE_CLASS_TO_SCRIPT_EX(LayoutFrame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, SetName, SVT_EMPTY, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetName, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, Show);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, Hide);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, IsShown, SVT_BOOL);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, SetPoint, SVT_EMPTY, SVT_PTR_STRING, SVT_PTR_STRING, SVT_PTR_STRING, SVT_INT, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetRight, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetWidth, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, SetWidth, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetHeight, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, SetHeight, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetRealLeft, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetRealTop, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetParent, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, SetSize, SVT_EMPTY, SVT_INT, SVT_INT);

//客户端数据
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, SetClientID, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetClientID, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, SetClientUserData, SVT_EMPTY, SVT_INT, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetClientUserData, SVT_INT, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, SetClientString, SVT_EMPTY, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(LayoutFrame, GetClientString, SVT_PTR_STRING);
*/