#pragma warning( disable : 4996 )

#include <math.h>
#include "ui_button.h"
#include "ui_framemgr.h"
#include "OgreScriptLuaVM.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// Button control
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

IMPLEMENT_UIOBJECT_CLONE(Button)


Button::Button() : m_pNormalRegion(NULL), m_pOverlayRegion(NULL), m_pNormalFontString(NULL) , m_pAddon(NULL) 
	,m_bLocked(false),m_NormalColor(200,200,200),m_HighlightColor(200,200,200),m_DisableColor(200,200,200),m_PushedColor(200,200,200)
{
	m_Type = FRM_BUTTON;

	m_bCooldown = false;
	m_bFade		= false;
	m_fCooldownDuration = m_fCooldownStart = 0.0f;

	m_bIntonate = false;
	m_fIntonateStart = m_fIntonateTotal = 0.0f;
	
	m_CooldownRect = m_AbsRect;
	m_hNormalTex = m_hPushedTex = m_hDisableTex = m_hHighlightTex = (HUIRES)0;
	m_State = FRMSTATE_VISIBLE;
}

void Button::Init()
{
	assert(m_pNormalRegion==NULL && m_pNormalFontString==NULL);

	//初始化贴图
	std::string nodename = "$parentNormalRegion";
	m_pNormalRegion = static_cast<::Texture *>(g_pFrameMgr->CreateObject("Texture", nodename.c_str()));
	m_pNormalRegion->m_Anchor[0].m_RelFrame = "$parent";
	m_pNormalRegion->m_Size = m_Size;
	m_pNormalRegion->m_Anchor[0].m_MyPoint = m_pNormalRegion->m_Anchor[0].m_RelPoint = FP_TOPLEFT;
	m_pNormalRegion->m_Anchor[0].m_Offset.SetAbsDim(0,0);
	m_pNormalRegion->m_FrameStrata = m_FrameStrata;

	nodename = "$parentOverlayRegion";
	m_pOverlayRegion = static_cast<::Texture *>(g_pFrameMgr->CreateObject("Texture", nodename.c_str()));
	m_pOverlayRegion->m_Anchor[0].m_RelFrame = "$parent";
	m_pOverlayRegion->m_Size = m_Size;
	m_pOverlayRegion->m_Anchor[0].m_MyPoint = m_pOverlayRegion->m_Anchor[0].m_RelPoint = FP_TOPLEFT;
	m_pOverlayRegion->m_Anchor[0].m_Offset.SetAbsDim(0,0);
	m_pOverlayRegion->m_FrameStrata = m_FrameStrata;
	m_pOverlayRegion->DrawShow(false);

	nodename = "$parentNormalFontString";
	m_pNormalFontString = static_cast<FontString *>(g_pFrameMgr->CreateObject("FontString", nodename.c_str()));
	m_pNormalFontString->m_Anchor[0].m_RelFrame = "$parent";
	m_pNormalFontString->m_TextColor = ColorQuad(200, 200, 200, 255);
	m_pNormalFontString->m_Size.SetRelDim(1.0f, 1.0f);
	m_pNormalFontString->m_Anchor[0].m_MyPoint = m_pNormalFontString->m_Anchor[0].m_RelPoint = FP_CENTER;
	m_pNormalFontString->m_Anchor[0].m_Offset.SetAbsDim(2,2);
	m_pNormalFontString->m_FrameStrata = m_FrameStrata;
}

Button::~Button()
{
	OGRE_RELEASE(m_pNormalRegion);
	OGRE_RELEASE(m_pOverlayRegion);
	OGRE_RELEASE(m_pNormalFontString);

	g_pDisplay->ReleaseUIRes(m_hNormalTex);
	g_pDisplay->ReleaseUIRes(m_hPushedTex);
	g_pDisplay->ReleaseUIRes(m_hDisableTex);
	g_pDisplay->ReleaseUIRes(m_hHighlightTex);
}

void Button::CopyMembers(Button *pdest)
{
	if (pdest)
	{
		Frame::CopyMembers(pdest);

		//pdest->m_DrawRegions.resize(0);
		//if(m_pNormalRegion)
		//{
		//	pdest->m_pNormalRegion = static_cast<Texture *>(m_pNormalRegion->CreateClone());
		//	pdest->AddTexture(DL_ARTWORK, pdest->m_pNormalRegion);
		//}
		//if(m_pOverlayRegion)
		//{
		//	pdest->m_pOverlayRegion = static_cast<Texture *>(m_pOverlayRegion->CreateClone());
		//	pdest->AddTexture(DL_ARTWORK, pdest->m_pOverlayRegion);
		//}

		assert(pdest->m_pNormalRegion==NULL && pdest->m_pNormalFontString==NULL);
		for (size_t i=0; i<pdest->m_DrawRegions.size(); ++i)
		{
			LayoutFrame *pregion = m_DrawRegions[i].pregion;
			LayoutFrame *pdestregion = pdest->m_DrawRegions[i].pregion;

			if(pregion == m_pNormalRegion)
			{
				pdestregion->addRef();
				pdest->m_pNormalRegion = static_cast<::Texture *>(pdestregion);
			}
			else if(pregion == m_pOverlayRegion)
			{
				pdestregion->addRef();
				pdest->m_pOverlayRegion = static_cast<::Texture *>(pdestregion);
			}
			else if(pregion == m_pNormalFontString)
			{
				pdestregion->addRef();
				pdest->m_pNormalFontString = static_cast<FontString *>(pdestregion);
			}
		}
		if(pdest->m_pNormalFontString==NULL && m_pNormalFontString)
		{
			pdest->m_pNormalFontString = static_cast<FontString *>(m_pNormalFontString->CreateClone());
		}

		pdest->m_NormalTexName = m_NormalTexName;
		pdest->m_PushedTexName = m_PushedTexName;
		pdest->m_DisabledTexName = m_DisabledTexName;
		pdest->m_HighlightTexName = m_HighlightTexName;

		assert(pdest->m_hNormalTex == 0);
		pdest->m_hNormalTex = AssignHUIRes(m_hNormalTex);
		pdest->m_hPushedTex = AssignHUIRes(m_hPushedTex);
		pdest->m_hDisableTex = AssignHUIRes(m_hDisableTex);
		pdest->m_hHighlightTex = AssignHUIRes(m_hHighlightTex);

		//if (m_hNormalFont)
		//	pdest->m_hNormalFont = AssignHUIRes(m_hNormalFont);

		//pdest->m_HighlightFont = m_HighlightFont;
		//pdest->m_DisableFont = m_DisableFont;
		pdest->m_NormalColor = m_NormalColor;
		pdest->m_HighlightColor = m_HighlightColor;
		pdest->m_DisableColor = m_DisableColor;
		pdest->m_PushedColor = m_PushedColor;
		pdest->m_PushedTextOffset = m_PushedTextOffset;

		pdest->m_bCooldown = m_bCooldown;
		pdest->m_fCooldownStart = m_fCooldownStart;
		pdest->m_fCooldownDuration = m_fCooldownDuration;

		pdest->m_bIntonate = m_bIntonate;
		pdest->m_fIntonateStart = m_fIntonateStart;
		pdest->m_fIntonateTotal = m_fIntonateTotal;

		pdest->m_Caption = m_Caption;
		pdest->m_OverlayTexUV = m_OverlayTexUV;
	}
}

void Button::SetNormalTexture(const char *path, const char *strdrawmode)
{
	if (path)
	{
		m_NormalTexName = path;

		UIBlendMode drawmode = BLEND_ALPHABLEND;

		if (strdrawmode)
		{
			if(!stricmp(strdrawmode, "opaque"))
			{
				drawmode = BLEND_OPAQUE;
			}
			if (!stricmp(strdrawmode, "add"))
			{
				drawmode = BLEND_ADD;
			}
		}

		HUIRES h = m_hNormalTex;
		if (strlen(path) != 0)
		{
			m_hNormalTex = g_pDisplay->CreateTexture(path, drawmode,
				&m_pNormalRegion->m_iRelWidth, &m_pNormalRegion->m_iRealHeight);
		}
		else
		{
			m_hNormalTex = g_pDisplay->GetNullTexture();
		}
		g_pDisplay->ReleaseUIRes(h);

		m_pNormalRegion->SetTextureHuires(m_hNormalTex);
	}
}

void Button::SetPushedTexture(const char *path, const char *strdrawmode)
{
	if (path)
	{
		m_PushedTexName = path;

		UIBlendMode drawmode = BLEND_ALPHABLEND;

		if (strdrawmode)
		{
			if(!stricmp(strdrawmode, "opaque"))
			{
				drawmode = BLEND_OPAQUE;
			}
			if (!stricmp(strdrawmode, "add"))
			{
				drawmode = BLEND_ADD;
			}
		}

		HUIRES h = m_hPushedTex;
		if (strlen(path) != 0)
		{
			m_hPushedTex = g_pDisplay->CreateTexture(path, drawmode,
				&m_pNormalRegion->m_iRelWidth, &m_pNormalRegion->m_iRealHeight);

		}
		else
		{
			m_hPushedTex = g_pDisplay->GetNullTexture();
		}
		g_pDisplay->ReleaseUIRes(h);

		m_pOverlayRegion->DrawShow(false);
		m_pOverlayRegion->SetTextureHuires(m_hPushedTex);
	}
}

void Button::SetDisableTexture(const char *path, const char *strdrawmode)
{
	if (path)
	{
		m_DisabledTexName = path;

		UIBlendMode drawmode = BLEND_ALPHABLEND;

		if (strdrawmode)
		{
			if(!stricmp(strdrawmode, "opaque"))
			{
				drawmode = BLEND_OPAQUE;
			}
			if (!stricmp(strdrawmode, "add"))
			{
				drawmode = BLEND_ADD;
			}
		}

		HUIRES h = m_hDisableTex;
		if (strlen(path) != 0)
		{
			m_hDisableTex = g_pDisplay->CreateTexture(path, drawmode,
				&m_pOverlayRegion->m_iRelWidth, &m_pOverlayRegion->m_iRealHeight);

		}
		else
		{
			m_hDisableTex = g_pDisplay->GetNullTexture();
		}
		g_pDisplay->ReleaseUIRes(h);

		m_pOverlayRegion->DrawShow(false);
		m_pOverlayRegion->SetTextureHuires(m_hDisableTex);
	}
}

std::string Button::GetNormalTexture()
{
	return m_pNormalRegion->GetName();
}

void Button::SetNormalTextureHuires(HUIRES h)
{
	g_pDisplay->AddRef(h);
	g_pDisplay->ReleaseUIRes(m_hNormalTex);
	m_hNormalTex = h;

	m_pNormalRegion->SetTextureHuires(m_hNormalTex);
}

void Button::AddLock()
{
    m_bLocked = true;
    if(!m_pAddon)
    {
        //std::string strBtnName = m_Name.c_str();
        //strBtnName += "MagicLock";
        //m_pAddon = g_pFrameMgr->FindLayoutFrame(strBtnName);
        m_pAddon = g_pFrameMgr->FindLayoutFrame("MagicLock");
    }
 
    if (m_pAddon)
    {
        static_cast<LayoutFrame*>(m_pAddon)->SetPoint("center" , m_Name.c_str() , "center" , 0 , 0);
        //static_cast<Button*>(m_pAddon)->SetFrameLevel(this->GetFrameLevel() + 1);
    } 
}

void Button::SetPushedTexture(HUIRES h)
{
	if(m_pOverlayRegion)
	{
		g_pDisplay->AddRef(h);
		g_pDisplay->ReleaseUIRes(m_hPushedTex);
		m_hPushedTex = h;

		m_pOverlayRegion->SetTextureHuires(m_hPushedTex);	
	}
}

void Button::SetHighlightTextureHuires(HUIRES h)
{
	g_pDisplay->AddRef(h);
	g_pDisplay->ReleaseUIRes(m_hHighlightTex);
	m_hHighlightTex = h;

	m_pOverlayRegion->SetTextureHuires(m_hHighlightTex);
}

HUIRES Button::GetHighlightTextureHuires()
{
	return m_hHighlightTex;
}

void Button::SetBlendAlpha(float alpha)
{
	if ( m_pNormalRegion == NULL )
	{
		return;
	}

	m_pNormalRegion->SetBlendAlpha( alpha );
}

void Button::SetGray (bool bGray)
{
	m_pNormalRegion->SetGray( bGray );
}

void Button::SetGrayEx( bool bGray )
{
	m_pNormalRegion->SetGray( bGray );
	m_pOverlayRegion->SetGray( bGray );
}
void Button::SetDisableTextureHuires(HUIRES h)
{
	g_pDisplay->AddRef(h);
	g_pDisplay->ReleaseUIRes(m_hDisableTex);
	m_hDisableTex = h;

	m_pOverlayRegion->SetTextureHuires(m_hDisableTex);
}

HUIRES Button::GetDisableTextureHuires()
{
	return m_hDisableTex;
}

void Button::SetHightlightTexture(const char *path, const char *strdrawmode)
{
	if (path)
	{
		m_HighlightTexName = path;

		UIBlendMode drawmode = BLEND_ALPHABLEND;

		if (strdrawmode)
		{
			if(!stricmp(strdrawmode, "opaque"))
			{
				drawmode = BLEND_OPAQUE;
			}
			if (!stricmp(strdrawmode, "add"))
			{
				drawmode = BLEND_ADD;
			}
		}

		Ogre::HUIRES h = m_hHighlightTex;
		if (strlen(path) != 0)
		{
			m_hHighlightTex = g_pDisplay->CreateTexture(path, drawmode,	&m_pOverlayRegion->m_iRelWidth, &m_pOverlayRegion->m_iRealHeight);
		}
		else
		{
			m_hHighlightTex = g_pDisplay->GetNullTexture();
		}
		g_pDisplay->ReleaseUIRes(h);

		//m_pOverlayRegion->DrawShow(false);
		m_pOverlayRegion->SetTextureHuires(m_hHighlightTex);

		//m_State |= FRMSTATE_CLICKDOWN;
	}

}

void Button::SetText(const char *str)
{
	if (str)
	{
		m_pNormalFontString->SetText(str);
	}
}

const char * Button::GetText()
{
	return m_pNormalFontString->GetText();
}

void Button::SetTextColor(int r, int g, int b)
{
	m_pNormalFontString->SetTextColor(r,g,b);
}

bool Button::IsCooldown()
{
	return m_bCooldown;
}

void Button::SetCooldownTimer(float start, float duration, bool enable, bool bFade )
{
	m_bCooldown = enable;
	if (enable)
	{
		assert(start >= 0.0f && duration >= 0.0f);
		m_fCooldownStart = start;
		m_fCooldownDuration = duration;
		m_bFade	= bFade;
        if(m_pAddon && m_bLocked)
            static_cast<LayoutFrame*>(m_pAddon)->Show();

	}
	else
	{
		//assert(0 && "not a cooldown button");
	}
}

void Button::SetIntonateTimer(float start, float total)
{
	m_bIntonate = true;

	assert(start >= 0.0f && total >= 0.0f);

	m_fIntonateStart = start;
	m_fIntonateTotal = total;
}

int Button::OnLButtonDown()
{
	if( m_State & FRMSTATE_DISABLE )
	{
		return 1;
	}

	{
		g_pFrameMgr->SetBeginDragFrame(this);
		if (g_pFrameMgr->m_pCurCursor->IsInDragState())
		{
			if ( hasScriptsEvent(SC_ONRECEIVEDRAG))
			{
				CallScript(SC_ONRECEIVEDRAG);
				return 0;
			}
			else
			{
				//g_pFrameMgr->m_pCurCursor->EndDrag();
			}
		}else{
			
		}

		m_State |= FRMSTATE_CLICKDOWN;

		if (hasScriptsEvent(SC_ONMOUSEDOWN))
		{
			CallScript(SC_ONMOUSEDOWN, "s", "LeftButton" );
			return 0;
		}
		return 0;
	}

	return 1;
}

int Button::OnLButtonUp()
{
	if( m_State & FRMSTATE_DISABLE )
	{
		return 1;
	}
	
	if( m_State & FRMSTATE_CLICKDOWN )
	{	
		m_State &= ~FRMSTATE_CLICKDOWN;
		
		// 点击按钮的时候播放点击音效
		if( !m_strClickSoundPath.empty() )
		{
			playUISound( m_strClickSoundPath.c_str() );
		}

		if (hasScriptsEvent(SC_ONMOUSEUP))
		{
			CallScript(SC_ONMOUSEUP, "s", "LeftButton");
		}
		
		if (hasScriptsEvent(SC_ONCLICK))
		{
			CallScript(SC_ONCLICK, "s", "LeftButton");
		}else
		{
			//return Frame::OnMessage(msgtype, wparam, lparam);
			return 1;
		}
		if( !IsShown() )
		{
			DisHighlight();
		}

		return 0;
	}

	return 1;
}

void Button::SwapCooldownMembers(Button *pdest)
{
	if( pdest != NULL )
	{
		std::swap( pdest->m_bCooldown, m_bCooldown );
		std::swap( pdest->m_fCooldownStart,m_fCooldownStart );
		std::swap( pdest->m_fCooldownDuration, m_fCooldownDuration );
		std::swap( pdest->m_bLocked, m_bLocked );    
		std::swap( pdest->m_CooldownRect, m_CooldownRect );
	}
}

void Button::CopyCooldownMembers( Button* pdest )
{
	if( pdest != NULL )
	{
		m_bCooldown			= pdest->m_bCooldown;
		m_fCooldownStart	= pdest->m_fCooldownStart;
		m_fCooldownDuration = pdest->m_fCooldownDuration;
		m_bLocked			= pdest->m_bLocked;    
		m_CooldownRect		= pdest->m_CooldownRect;
	}
}

// 设置冷却贴图的绘制区域
void Button::SetCooldownTextureRect( int offsetleft, int offsettop, int width, int height )
{
	m_CooldownRect.set( offsetleft, offsettop, width, height );
}

int Button::OnInputMessage(const Ogre::InputEvent &event)
{
	if( m_State & FRMSTATE_DISABLE )
	{
		return 0;
	}

	switch(event.msg)
	{
	case GIE_LBTNDBLCLK:
		{
			if (m_bShow)
			{
				if (hasScriptsEvent(SC_ONDOUBLECLICK))
				{
					CallScript(SC_ONDOUBLECLICK, "s", "LDoubleClick");
					return 0;
				}
			}
		}
	case GIE_LBTNDOWN:
		{
			return OnLButtonDown();
		}

	case GIE_LBTNUP:
		{
			return OnLButtonUp();
		}

	case GIE_RBTNDOWN:
		{	
			if( event.mouse.flags == VK_LBUTTON )
			{
				return 0;
			}

			if (m_bShow)
			{
				m_State |= FRMSTATE_CLICKDOWN;

				if (hasScriptsEvent(SC_ONMOUSEDOWN))
				{
					CallScript(SC_ONMOUSEDOWN, "s", "RightButton");
					return 0;
				}
				return 0;
			}
			break;
		}
	case GIE_RBTNUP:
		{
			if( event.mouse.flags == VK_LBUTTON )
			{
				return 0;
			}

			if( m_State & FRMSTATE_CLICKDOWN )
			{	
				m_State &= ~FRMSTATE_CLICKDOWN;

				if (hasScriptsEvent(SC_ONMOUSEUP))
				{
					CallScript(SC_ONMOUSEUP, "s", "RightButton");
				}

				if (hasScriptsEvent(SC_ONCLICK))
				{
					CallScript(SC_ONCLICK, "s", "RightButton");

					char szScript[256];
					sprintf( szScript, "PlayClickSound(\"%s\",\"%s\")", "RightButton", GetName() );
					g_pUIScriptVM->callString( szScript );	
				}else
				{
					return Frame::OnInputMessage(event);
				}
				if( !IsShown() )
				{
					DisHighlight();
				}
				return 0;
				//if(GetMsgStyle()) OnCommand();
			}
			break;
		}

	case GIE_KEYUP:
		{
			if( m_State & FRMSTATE_CLICKDOWN )
			{	
				m_State &= ~FRMSTATE_CLICKDOWN;	
			}
			return 1;
		}

	case GIE_MOUSEMOVE:
		{
			if (m_bShow)
			{
				if ( hasScriptsEvent(SC_ONMOUSEMOVE))
				{
					CallScript(SC_ONMOUSEMOVE);
					return 0;
				}
			}
			return 0;
		}

	case GIE_MOUSEIN:
		{			
			if (m_bShow)
			{
				if( g_pFrameMgr->GetFocusFrame() == this && g_pFrameMgr->m_bMouseClickDown == true )
				{
					m_State |= FRMSTATE_CLICKDOWN;
				}
				Highlight();
				if (hasScriptsEvent(SC_ONENTER) )
				{
					CallScript(SC_ONENTER);
				}
			}
			return 0;
		}

	case GIE_MOUSEOUT:
		{
			if (m_bShow)
			{
				DisHighlight();
				ClearPushState();
				for ( size_t i = 0, size = m_Children.size(); i < size; ++i )
				{
					m_Children[i]->OnInputMessage( event );
				}

				if (hasScriptsEvent(SC_ONLEAVE))
				{
					CallScript(SC_ONLEAVE);
				}
			}
			return 0;
		}

	default:
		{
			return Frame::OnInputMessage(event);
			//return 0;
		}
	}

	return 1;
}

void Button::SetViewStartPointRecursive(int x, int y)
{
	Frame::SetViewStartPointRecursive(x, y);

	if ( m_pOverlayRegion != NULL )
	{
		m_pOverlayRegion->SetViewStartPoint(x, y);
	}

	if ( m_pNormalRegion != NULL )
	{
		m_pNormalRegion->SetViewStartPoint(x, y);
	}
}

void Button::UpdateSelf(float deltatime)
{
	if (!m_bShow)
	{
		return;
	}
	Frame::UpdateSelf(deltatime);

	if(m_State & FRMSTATE_DISABLE)
	{
		if ((m_hNormalTex != (HUIRES)0) && (m_hDisableTex != (HUIRES)0))
		{
			m_pNormalRegion->DrawShow(false);
			SetDisableTextureHuires(m_hDisableTex);
			OVERLAYIT iter = m_OverlayTexUV.find( "DisabledTexture" );
			if( iter != m_OverlayTexUV.end() )
			{
				ButtonOverlay& overlay = iter->second;
				if( overlay.bTemplate )
				{
					m_pOverlayRegion->SetTextureTemplate( overlay.templateName );
				}else{
					m_pOverlayRegion->SetTexUV( overlay.uv.x, overlay.uv.y, overlay.uv.w, overlay.uv.h );
					m_pOverlayRegion->m_DrawType = eDraw_Normal;
				}
				m_pOverlayRegion->DrawShow(true);
				
			}else{
				m_pOverlayRegion->DrawShow(false);
				m_pNormalRegion->DrawShow(true);
			}
			m_pNormalFontString->SetTextColor(m_DisableColor.r,m_DisableColor.g,m_DisableColor.b);
		}
	}
	else if (m_State & FRMSTATE_CLICKDOWN)
	{
		if ((m_hNormalTex != (HUIRES)0) && (m_hPushedTex != (HUIRES)0))
		{
			m_pNormalRegion->DrawShow(false);
			SetPushedTexture(m_hPushedTex);
			OVERLAYIT iter = m_OverlayTexUV.find( "PushedTexture" );
			if( iter != m_OverlayTexUV.end() )
			{
				ButtonOverlay& overlay = iter->second;
				if( overlay.bTemplate )
				{
					m_pOverlayRegion->SetTextureTemplate( overlay.templateName );
				}else{
					m_pOverlayRegion->SetTexUV( overlay.uv.x, overlay.uv.y, overlay.uv.w, overlay.uv.h );
					m_pOverlayRegion->m_DrawType = eDraw_Normal;
				}				
			}
			m_pNormalFontString->SetTextColor(m_PushedColor.r,m_PushedColor.g,m_PushedColor.b);
			m_pOverlayRegion->DrawShow(true);
		}
		if (hasScriptsEvent(SC_ONBUTTONCLICK))
		{
			CallScript(SC_ONBUTTONCLICK);
		}
	}
	else if ( m_State & FRMSTATE_MOUSEOVER )
	{
		if ((m_hNormalTex != (HUIRES)0) && (m_hHighlightTex != (HUIRES)0))
		{
			if( !( m_State & FRMSTATE_CHECKED) )
			{
				m_pNormalRegion->DrawShow(false);
				SetHighlightTextureHuires(m_hHighlightTex);
				OVERLAYIT iter = m_OverlayTexUV.find( "HighlightTexture" );
				if( iter != m_OverlayTexUV.end() )
				{
					ButtonOverlay& overlay = iter->second;
					if( overlay.bTemplate )
					{
						m_pOverlayRegion->SetTextureTemplate( overlay.templateName );
					}else{
						m_pOverlayRegion->SetTexUV( overlay.uv.x, overlay.uv.y, overlay.uv.w, overlay.uv.h );
						m_pOverlayRegion->m_DrawType = eDraw_Normal;
					}
				}
				m_pNormalFontString->SetTextColor(m_HighlightColor.r,m_HighlightColor.g,m_HighlightColor.b);
				m_pOverlayRegion->DrawShow(true);
			}
		}
	}
	else if (m_State & FRMSTATE_CHECKED)
	{
		if ((m_hNormalTex != (HUIRES)0) && (m_hPushedTex != (HUIRES)0))
		{
			m_pNormalRegion->DrawShow(false);
			SetHighlightTextureHuires(m_hPushedTex);
			OVERLAYIT iter = m_OverlayTexUV.find( "PushedTexture" );
			if( iter != m_OverlayTexUV.end() )
			{
				ButtonOverlay& overlay = iter->second;
				if( overlay.bTemplate )
				{
					m_pOverlayRegion->SetTextureTemplate( overlay.templateName );
				}else{
					m_pOverlayRegion->SetTexUV( overlay.uv.x, overlay.uv.y, overlay.uv.w, overlay.uv.h );
					m_pOverlayRegion->m_DrawType = eDraw_Normal;
				}
			}
			m_pNormalFontString->SetTextColor(m_PushedColor.r,m_PushedColor.g,m_PushedColor.b);
			m_pOverlayRegion->DrawShow(true);
		}
	}
	else
	{
		if (m_hNormalTex != (HUIRES)0)
		{
			m_pNormalRegion->DrawShow(true);
		}
		if(m_hPushedTex != (HUIRES)0)
		{
			m_pOverlayRegion->DrawShow(false);
		}
		m_pNormalFontString->SetTextColor(m_NormalColor.r,m_NormalColor.g,m_NormalColor.b);
	}

	if (m_bCooldown)
	{
		m_fCooldownStart += deltatime;
	}

	if (m_bIntonate)
	{
		m_fIntonateStart += deltatime;
	}
}

void Button::FillDrawItems()
{
	if (m_bShow)
	{
		if (m_pParent)
		{
			m_iDrawLevel = m_pParent->m_iDrawLevel + 1;
		}

		g_pFrameMgr->AddDrawItems(static_cast<LayoutFrame *>(this));

		Frame::FillChildren();
	}
}

void Button::DrawCooldown()
{
	float fRate = m_fCooldownStart/m_fCooldownDuration;

	float fAngle = fRate * 360.0f;
	if ( m_bFade )
	{
		fAngle = 360.0f - fAngle;
	}

	float width = (float)m_AbsRect.getWidth();
	float height = (float)m_AbsRect.getHeight();

	assert(fAngle >= 0.0f);

	// 每增加45度就少绘制一个三角形，初始的时候绘制8个
	// 这用于获取已经转过的三角形数目
	int iCurTriangle = ( int )( fAngle / 45.0f );
	// 这个是剩下的三角形数目
	int numTriangle = 8 - iCurTriangle;
	Vector2* pVertexBuffer = new Vector2[numTriangle * 3];

	for ( int i = 8; i > ( iCurTriangle + 1 ); --i )
	{
		DrawCooldownBaseTriangle(pVertexBuffer, i, (float)m_AbsRect.getWidth(), (float)m_AbsRect.getHeight() );
	}

	DrawCooldownLastTriangle(pVertexBuffer, numTriangle, (float)m_AbsRect.getWidth(), (float)m_AbsRect.getHeight() , fAngle);

	//绘制cooldown button 的三角形
	g_pDisplay->BeginDraw(g_pDisplay->GetNullTexture());
	g_pDisplay->DrawTriangleList(pVertexBuffer, numTriangle, ColorQuad(0,0,0,150).c);
	g_pDisplay->EndDraw();

	delete(pVertexBuffer);

}

void Button::DrawFade()
{
	float fRate = m_fCooldownStart / m_fCooldownDuration;

	float fAngle = fRate * 360.0f;

	float width = (float)m_AbsRect.getWidth();
	float height = (float)m_AbsRect.getHeight();

	assert( fAngle >= 0.0f );

	int numTriangle = (int)ceil( fAngle / 45.0f );
	
	//int numTriangle = 8 - iCurTriangle;
	//int numTriangle = (int)( fAngle / 45.0f );

	if ( numTriangle == 0 )
	{
		return;
	}

	Vector2* pVertexBuffer = new Vector2[numTriangle * 3];

	for ( int i = 1; i < numTriangle; ++i )
	{
		DrawFadeBaseTriangle( pVertexBuffer, i, (float)m_AbsRect.getWidth(), (float)m_AbsRect.getHeight() );
	}

	DrawFadeLastTriangle( pVertexBuffer, numTriangle, (float)m_AbsRect.getWidth(), (float)m_AbsRect.getHeight() , fAngle );

	//绘制cooldown button 的三角形
	g_pDisplay->BeginDraw(g_pDisplay->GetNullTexture());
	g_pDisplay->DrawTriangleList(pVertexBuffer, numTriangle, ColorQuad(0,0,0,150).c);
	g_pDisplay->EndDraw();

	delete(pVertexBuffer);
}

void Button::Draw()
{
	Frame::Draw();

	if (m_bCooldown)
	{
		if (m_fCooldownStart < m_fCooldownDuration)
		{            
            if ( m_bFade )
            {
				DrawFade();
            }
			else
			{
				DrawCooldown();
			}
		}
		else
		{
			m_bCooldown = false;//表示CD时间已经结束
            if(m_pAddon && m_bLocked)
            {
                static_cast<LayoutFrame*>(m_pAddon)->Hide();
                m_bLocked = false;
            }
		}
	}

	if (m_bIntonate)
	{
		float width = 0.0f;
		LayoutDim ld = m_pNormalRegion->GetSize();
		if (m_fIntonateStart <= m_fIntonateTotal)
		{
			width = m_fIntonateStart / m_fIntonateTotal;
		}
		else
		{
			width = 1.0f;
			m_bIntonate = false;
		}

		ld.SetAbsDim(FloatToInt(width * this->GetSize().GetX()), (int)ld.GetY());
		m_pNormalRegion->SetLayOutSize(ld);
	}

	//if(g_pFrameMgr->GetEditMode() == UI_MODE_DEBUG)
	//{
	//	g_pDisplay->DrawRect(m_AbsRect.m_Left, m_AbsRect.m_Top, m_AbsRect.getWidth(), 
	//		m_AbsRect.getHeight(), ColorQuad(255, 0, 0, 255).c, 0, 0);
	//}

	////绘制button上的文字
	//if (m_pNormalFontString)
	//{
	//	if(!m_pNormalFontString->m_Caption.empty())
	//	{
	//		m_pNormalFontString->Draw();
	//	}
	//}
}

TiXmlElement *Button::Save(TiXmlElement *pElement)
{
	TiXmlElement *pButton = Frame::Save(pElement);

	//Normaltexture
	if (m_pNormalRegion)
	{
		if (m_pNormalRegion->m_Size != m_Size)
		{
			TiXmlElement *pNormalTexture = m_pNormalRegion->Save(pButton);
			pNormalTexture->SetValue("NormalTexture");
		}
		else
		{
			if (!m_NormalTexName.empty())
			{
				TiXmlElement *pNormalTexture = new TiXmlElement("NormalTexture");
				pButton->LinkEndChild(pNormalTexture);
				pNormalTexture->SetAttribute("file", m_NormalTexName.c_str());
			}
		}
	}

	//Pushedtexture
	if (!m_PushedTexName.empty())
	{
		TiXmlElement *pPushedTexture = new TiXmlElement("PushedTexture");
		pButton->LinkEndChild(pPushedTexture);
		pPushedTexture->SetAttribute("file", m_PushedTexName.c_str());
	}

	//Disabledtexture
	if (!m_DisabledTexName.empty())
	{
		TiXmlElement *pDisabledTexture = new TiXmlElement("DisabledTexture");
		pButton->LinkEndChild(pDisabledTexture);
		pDisabledTexture->SetAttribute("file", m_DisabledTexName.c_str());
	}

	//Pushedtexture
	if (!m_HighlightTexName.empty())
	{
		TiXmlElement *pHighlightTexture = new TiXmlElement("HighlightTexture");
		pButton->LinkEndChild(pHighlightTexture);
		pHighlightTexture->SetAttribute("file", m_HighlightTexName.c_str());
	}

	return pButton;
}

//绘制CoolDown Button的第i个三角形
void Button::DrawCooldownBaseTriangle(Vector2 *pVertexBuffer, int iThTriangle, float width, float height)
{
	if ( pVertexBuffer )
	{
		assert((iThTriangle>1) && (iThTriangle <= 8));
		
		float fScaleY = 1.0f;
		if ( m_CooldownRect.m_Right!= 0  && m_CooldownRect.m_Bottom != 0 )
		{
			width	= ( float )m_CooldownRect.m_Right;
			height	= ( float )m_CooldownRect.m_Bottom;

			/*width	= (float)m_CooldownRect.getWidth();
			height	= (float)m_CooldownRect.getHeight();*/
			fScaleY = g_pFrameMgr->GetScreenScaleY();
		}
		
		switch(iThTriangle)
		{
		case 2:
			{
				pVertexBuffer[18].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[18].y = (float)m_AbsRect.m_Top	+ m_CooldownRect.m_Top * fScaleY;
				pVertexBuffer[19].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[19].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[20].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[20].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				break;
			}

		case 3:
			{
				pVertexBuffer[15].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[15].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[16].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[16].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[17].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[17].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				break;
			}

		case 4:
			{
				pVertexBuffer[12].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[12].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				pVertexBuffer[13].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[13].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[14].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[14].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;		
				break;
			}

		case 5:
			{
				pVertexBuffer[9].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[9].y = (float)m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				pVertexBuffer[10].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[10].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[11].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[11].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				break;
			}

		case 6:
			{
				pVertexBuffer[6].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[6].y = (float)m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				pVertexBuffer[7].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[7].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[8].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[8].y = (float)m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				break;
			}	

		case 7:
			{
				pVertexBuffer[3].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[3].y = (float)m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[4].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[4].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[5].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[5].y = (float)m_AbsRect.m_Top		+ m_CooldownRect.m_Top * fScaleY;
				break;
			}

		case 8:
			{
				pVertexBuffer[0].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[0].y = (float)m_AbsRect.m_Top		+ m_CooldownRect.m_Top * fScaleY;
				pVertexBuffer[1].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[1].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[2].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[2].y = (float)m_AbsRect.m_Top		+ m_CooldownRect.m_Top * fScaleY;
				break;
			}

		default:
			{
				assert(0 && "Draw Cooldown button Base triangle error");
				break;
			}
		}
	}
}

//绘制cooldown button 的最后一个可变的三角形
// numTriangle 为要绘制的三角形数目；fAngle指已经转过的角度
void Button::DrawCooldownLastTriangle(Vector2 *pVertexBuffer, int numTriangle, float width, float height, 
										float fAngle)
{
	if (pVertexBuffer)
	{
		assert((numTriangle>0) && (numTriangle <= 8));
		
		float fScaleY = 1.0f;
		if( m_CooldownRect.m_Right!= 0  && m_CooldownRect.m_Bottom != 0 )
		{
			width	= (float)m_CooldownRect.m_Right;
			height	= (float)m_CooldownRect.m_Bottom;

			/*width	= (float)m_CooldownRect.getWidth();
			height	= (float)m_CooldownRect.getHeight();*/
			fScaleY = g_pFrameMgr->GetScreenScaleY();
		}
		// tan( a )在( 0, 90 )是0到正无穷，在( 90, 180 )是0到负无穷
		switch(numTriangle)
		{
		case 8:
			{
				pVertexBuffer[21].x = m_AbsRect.m_Left + ( m_CooldownRect.m_Left + width / 2 + height / 2 * Tan(fAngle) ) * fScaleY;

				pVertexBuffer[21].y = (float)m_AbsRect.m_Top	+ m_CooldownRect.m_Top * fScaleY;
				pVertexBuffer[22].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[22].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[23].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[23].y = (float)m_AbsRect.m_Top	+ m_CooldownRect.m_Top * fScaleY;
				break;
			}

		case 7:
			{
				pVertexBuffer[18].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[18].y = (float)m_AbsRect.m_Top	+ 
										( m_CooldownRect.m_Top + height / 2 - width / 2 * Tan( 90.0f - fAngle) ) * fScaleY;
				pVertexBuffer[19].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[19].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[20].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[20].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				break;
			}

		case 6:
			{
				// tan( -a ) = -tan( a )，tan( 90 - a ) = 1/tan( a )
				pVertexBuffer[15].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[15].y = (float)m_AbsRect.m_Top	+ 
										( m_CooldownRect.m_Top + height / 2 + width / 2 * Tan( fAngle-90.0f ) ) * fScaleY;
				pVertexBuffer[16].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[16].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[17].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width ) * fScaleY;
				pVertexBuffer[17].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				break;
			}

		case 5:
			{
				// tan( 180-a ) = -tan( a )
				pVertexBuffer[12].x = m_AbsRect.m_Left + 
										( m_CooldownRect.m_Left + width/2 + height/2*Tan(180.0f-fAngle) ) * fScaleY;

				pVertexBuffer[12].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				pVertexBuffer[13].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[13].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[14].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				pVertexBuffer[14].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				break;
			}

		case 4:
			{
				pVertexBuffer[9].x = m_AbsRect.m_Left + ( m_CooldownRect.m_Left + width/2 - height/2*Tan(fAngle-180.0f) ) * fScaleY;
				pVertexBuffer[9].y = (float)m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				pVertexBuffer[10].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[10].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[11].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[11].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				break;
			}

		case 3:
			{
				pVertexBuffer[6].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[6].y = (float)m_AbsRect.m_Top		+ 
										( m_CooldownRect.m_Top + height / 2 + width / 2 * Tan(270.0f - fAngle) ) * fScaleY;
				pVertexBuffer[7].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[7].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[8].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[8].y = (float)m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				break;
			}

		case 2:
			{
				pVertexBuffer[3].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[3].y = (float)m_AbsRect.m_Top		+ 
										( m_CooldownRect.m_Top + height / 2 - width / 2*Tan(fAngle-270.0f) ) * fScaleY;
				pVertexBuffer[4].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[4].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[5].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left * fScaleY;
				pVertexBuffer[5].y = (float)m_AbsRect.m_Top		+ m_CooldownRect.m_Top * fScaleY;
				break;
			}

		case 1:
			{
				pVertexBuffer[0].x = m_AbsRect.m_Left + ( m_CooldownRect.m_Left + width/2 - height/2*Tan(360.0f-fAngle) ) * fScaleY;
				pVertexBuffer[0].y = (float)m_AbsRect.m_Top		+ m_CooldownRect.m_Top * fScaleY;
				pVertexBuffer[1].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[1].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[2].x = (float)m_AbsRect.m_Left	+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[2].y = (float)m_AbsRect.m_Top		+ m_CooldownRect.m_Top * fScaleY;
				break;
			}

		default:
			{
				assert(0 && "Draw Cooldown button Last triangle error");
				break;
			}
		}
	}
}

void Button::DrawFadeBaseTriangle(Vector2 *pVertexBuffer, int iThTriangle, float width, float height)
{
	if ( pVertexBuffer != NULL )
	{
		assert((iThTriangle>=1) && (iThTriangle < 8));
		
		float fScaleY = 1.0f;
		if ( m_CooldownRect.m_Right!= 0  && m_CooldownRect.m_Bottom != 0 )
		{
			width	= (float)m_CooldownRect.m_Right;
			height	= (float)m_CooldownRect.m_Bottom;
			fScaleY = g_pFrameMgr->GetScreenScaleY();
		}

		switch ( iThTriangle )
		{
		case 1:
			{
				pVertexBuffer[0].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[0].y = (float)m_AbsRect.m_Top + m_CooldownRect.m_Top					* fScaleY;
				pVertexBuffer[1].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[1].y = m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[2].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[2].y = (float)m_AbsRect.m_Top + m_CooldownRect.m_Top					* fScaleY;
				break;
			}

		case 2:
			{
				pVertexBuffer[3].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[3].y = (float)m_AbsRect.m_Top + m_CooldownRect.m_Top					* fScaleY;
				pVertexBuffer[4].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[4].y = m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[5].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[5].y = (float)m_AbsRect.m_Top + ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				break;
			}

		case 3:
			{
				pVertexBuffer[6].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[6].y = (float)m_AbsRect.m_Top + ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[7].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[7].y = m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[8].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[8].y = (float)m_AbsRect.m_Top + ( m_CooldownRect.m_Top + height )		* fScaleY;
				break;
			}

		case 4:
			{
				pVertexBuffer[9].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[9].y = (float)m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height )		* fScaleY;
				pVertexBuffer[10].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[10].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[11].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[11].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height )		* fScaleY;		
				break;
			}

		case 5:
			{
				pVertexBuffer[12].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[12].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height )		* fScaleY;
				pVertexBuffer[13].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[13].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[14].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[14].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height )		* fScaleY;
				break;
			}

		case 6:
			{
				pVertexBuffer[15].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[15].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height )		* fScaleY;
				pVertexBuffer[16].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[16].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[17].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[17].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				break;
			}	

		case 7:
			{
				pVertexBuffer[18].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[18].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[19].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[19].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[20].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[20].y = (float)m_AbsRect.m_Top	+ m_CooldownRect.m_Top					* fScaleY;
				break;
			}

			/*case 8:
			{
			pVertexBuffer[21].x = (float)m_AbsRect.m_Left + m_CooldownRect.m_Left;
			pVertexBuffer[21].y = (float)m_AbsRect.m_Top + m_CooldownRect.m_Top;
			pVertexBuffer[22].x = m_AbsRect.m_Left + m_CooldownRect.m_Left + width/2;
			pVertexBuffer[22].y = m_AbsRect.m_Top + m_CooldownRect.m_Top + height/2;
			pVertexBuffer[23].x = m_AbsRect.m_Left + m_CooldownRect.m_Left + width/2;
			pVertexBuffer[23].y = (float)m_AbsRect.m_Top + m_CooldownRect.m_Top;
			break;
			}*/

		default:
			{
				assert(0 && "Draw Cooldown button Base triangle error");
				break;
			}
		}
	}
}

void Button::DrawFadeLastTriangle(Vector2 *pVertexBuffer, int numTriangle, float width, float height, 
									  float fAngle)
{
	if ( pVertexBuffer != NULL )
	{
		//assert((numTriangle>0) && (numTriangle <= 8));
		
		float fScaleY = 1.0f;

		if ( m_CooldownRect.m_Right!= 0  && m_CooldownRect.m_Bottom != 0 )
		{
			width	= (float)m_CooldownRect.m_Right;
			height	= (float)m_CooldownRect.m_Bottom;
			fScaleY = g_pFrameMgr->GetScreenScaleY();
		}

		// tan( a )在( 0, 90 )是0到正无穷，在( 90, 180 )是0到负无穷
		switch ( numTriangle )
		{
		case 1:
			{
				pVertexBuffer[0].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[0].y = (float)m_AbsRect.m_Top + m_CooldownRect.m_Top					* fScaleY;
				pVertexBuffer[1].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width / 2 )	* fScaleY;
				pVertexBuffer[1].y = m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 )	* fScaleY;
				pVertexBuffer[2].x = m_AbsRect.m_Left		+ 
										( m_CooldownRect.m_Left + width / 2 + height / 2 * Tan( fAngle ) ) * fScaleY;

				pVertexBuffer[2].y = (float)m_AbsRect.m_Top + m_CooldownRect.m_Top * fScaleY;
				break;
			}

		case 2:
			{
				pVertexBuffer[3].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[3].y = (float)m_AbsRect.m_Top + m_CooldownRect.m_Top					* fScaleY;
				pVertexBuffer[4].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[4].y = m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 )	* fScaleY;
				pVertexBuffer[5].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[5].y = (float)m_AbsRect.m_Top + 
									( m_CooldownRect.m_Top + width / 2 * Tan( fAngle - ( numTriangle - 1 ) * 45.0f ) )	* fScaleY;
				break;
			}

		case 3:
			{
				// tan( -a ) = -tan( a )，tan( 90 - a ) = 1/tan( a )

				pVertexBuffer[6].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[6].y = ( float )m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[7].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 )	* fScaleY;
				pVertexBuffer[7].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 )	* fScaleY;
				pVertexBuffer[8].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[8].y = ( float )m_AbsRect.m_Top	+ 
									( m_CooldownRect.m_Top + height / 2 + width / 2 * Tan( fAngle - 90.0f ) ) * fScaleY;
				break;
			}

		case 4:
			{
				// tan( 180-a ) = -tan( a )
				pVertexBuffer[9].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width )		* fScaleY;
				pVertexBuffer[9].y = (float)m_AbsRect.m_Top + ( m_CooldownRect.m_Top + height )		* fScaleY;
				pVertexBuffer[10].x = m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width / 2 )	* fScaleY;
				pVertexBuffer[10].y = m_AbsRect.m_Top		+ ( m_CooldownRect.m_Top + height / 2 )	* fScaleY;
				pVertexBuffer[11].x =  m_AbsRect.m_Left		+ ( m_CooldownRect.m_Left + width -
															height / 2 * Tan( fAngle - ( numTriangle - 1 ) * 45.0f ) ) * fScaleY;
				pVertexBuffer[11].y = (float)m_AbsRect.m_Top + ( m_CooldownRect.m_Top + height )	* fScaleY;
				break;
			}

		case 5:
			{
				pVertexBuffer[12].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 )	* fScaleY;
				pVertexBuffer[12].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height )		* fScaleY;
				pVertexBuffer[13].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 )	* fScaleY;
				pVertexBuffer[13].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 )	* fScaleY;
				pVertexBuffer[14].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 - 
																		height / 2 * Tan( fAngle - 180.0f ) ) * fScaleY;
				pVertexBuffer[14].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height ) * fScaleY;
				break;
			}

		case 6:
			{
				pVertexBuffer[15].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[15].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height )		* fScaleY;
				pVertexBuffer[16].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 ) * fScaleY;
				pVertexBuffer[16].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 ) * fScaleY;
				pVertexBuffer[17].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[17].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height - width / 2 * 
																	Tan( fAngle - ( numTriangle - 1 ) * 45.0f ) ) * fScaleY;
				break;
			}

		case 7:
			{
				pVertexBuffer[18].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[18].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height / 2 )	* fScaleY;
				pVertexBuffer[19].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 )	* fScaleY;
				pVertexBuffer[19].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 )	* fScaleY;
				pVertexBuffer[20].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[20].y = (float)m_AbsRect.m_Top	+ ( m_CooldownRect.m_Top + height / 2 - width / 2 * 
																	Tan( fAngle - ( numTriangle - 1 ) * 45.0f ) ) * fScaleY;
				break;
			}

		case 8:
			{
				pVertexBuffer[21].x = (float)m_AbsRect.m_Left	+ m_CooldownRect.m_Left					* fScaleY;
				pVertexBuffer[21].y = (float)m_AbsRect.m_Top	+ m_CooldownRect.m_Top					* fScaleY;
				pVertexBuffer[22].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + width / 2 )	* fScaleY;
				pVertexBuffer[22].y = m_AbsRect.m_Top			+ ( m_CooldownRect.m_Top + height / 2 )	* fScaleY;
				pVertexBuffer[23].x = m_AbsRect.m_Left			+ ( m_CooldownRect.m_Left + height / 2 *
																	Tan( fAngle - ( numTriangle - 1 ) * 45.0f ) ) * fScaleY;
				pVertexBuffer[23].y = (float)m_AbsRect.m_Top	+ m_CooldownRect.m_Top					* fScaleY;
				break;
			}

		default:
			{
				//assert(0 && "Draw Cooldown button Last triangle error");
				break;
			}
		}
	}
}

void Button::SetNormalRegionWidth(float width)
{
	LayoutDim ld = m_pNormalRegion->GetSize();
	ld.SetAbsDim(FloatToInt(width * this->GetSize().GetX()), (int)ld.GetY());
	m_pNormalRegion->SetLayOutSize(ld);
}

void Button::SetNormalRegionAbsWidth(float width)
{
	LayoutDim ld = m_pNormalRegion->GetSize();
	ld.SetAbsDim(FloatToInt(width), (int)ld.GetY());
	m_pNormalRegion->SetLayOutSize(ld);
}

float Button::GetNormalRegionWidth()
{
	float width = m_pNormalRegion->GetSize().GetX()/this->GetSize().GetX();

	return width;
}

void Button::SetNormalRegionHight(float Hight)
{
	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		if (!strcmp(m_DrawRegions[i].pregion->GetTypeName(), "Texture"))
		{
			static_cast<::Texture *>(m_DrawRegions[i].pregion)->setTextOffsetY( 1-Hight );
		}
	}	
}

float Button::GetNormalRegionHight()
{
	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		if (!strcmp(m_DrawRegions[i].pregion->GetTypeName(), "Texture"))
		{
			return static_cast<::Texture *>(m_DrawRegions[i].pregion)->getTextOffsetY();
		}
	}	
	return 0;
}

void Button::SetOverlayRegionAbsWidth(float width)
{
	if ( m_pOverlayRegion == NULL )
	{
		return;
	}

	LayoutDim ld = m_pOverlayRegion->GetSize();
	ld.SetAbsDim(FloatToInt( width ), (int)ld.GetY());
	m_pOverlayRegion->SetLayOutSize(ld);
}
float Button::GetOverlayRegionWidth()
{
	if ( m_pOverlayRegion == NULL )
	{
		return 0.0f;
	}

	float width = m_pOverlayRegion->GetSize().GetX()/this->GetSize().GetX();

	return width;
}

void Button::SetPushedState()
{
	m_State |= FRMSTATE_CLICKDOWN;
}

bool Button::GetPushedState()
{
	return (m_State & FRMSTATE_CLICKDOWN)?true:false;
}

void Button::Disable()
{
	m_State |= FRMSTATE_DISABLE;
	m_State &= ~FRMSTATE_MOUSEOVER;
}

void Button::Enable()
{
	m_State &= ~FRMSTATE_DISABLE;
}

bool Button::IsEnable()
{
    return (m_State & FRMSTATE_DISABLE) ? false : true;
}
void Button::Highlight()
{
	m_State |= FRMSTATE_MOUSEOVER;
}

void Button::DisHighlight()
{
	m_State &= ~FRMSTATE_MOUSEOVER;
}

bool Button::IsHighlight()
{
	return ( m_State & FRMSTATE_MOUSEOVER ) ? true : false;
}

void Button::Checked()
{
	m_State |= FRMSTATE_CHECKED;
}

void Button::DisChecked()
{
	m_State &= ~FRMSTATE_CHECKED;
}

bool Button::IsChecked()
{
    return (m_State & FRMSTATE_CHECKED) ? true : false;
}

float Button::GetAngle()
{
	return m_fAngle;
}

void Button::SetAngle(float fAngle)
{
	m_fAngle = fAngle;

	for (size_t i=0; i<m_DrawRegions.size(); i++)
	{
		if (!strcmp(m_DrawRegions[i].pregion->GetTypeName(), "Texture"))
		{
			static_cast<::Texture *>(m_DrawRegions[i].pregion)->SetAngle(m_fAngle);
		}
	}
}

void Button::SetNormalTextureSize( int width, int height )
{
	m_pNormalRegion->SetSize( width, height );
}

void Button::ChangeNormalTexture( int x, int y, int width, int height )
{
	m_pNormalRegion->SetTexUV( x, y, width, height );
}

void Button::SetNormalTextureRelUV( float x, float y )
{
	m_pNormalRegion->SetTexRelUV( 1, 1 );
}

//void Button::SetHighlightTextureSize( int width, int height )
//{
//	m_OverlayTexUV["HighlightTexture"]->SetSize( width, height );
//}

void Button::ChangeHighlightTexture( int x, int y, int width, int height )
{
	ButtonOverlay overlay;
	overlay.bTemplate = false;
	overlay.uv.x = x;
	overlay.uv.y = y;
	overlay.uv.w = width;
	overlay.uv.h = height;
	m_OverlayTexUV["HighlightTexture"] = overlay;
}

void Button::ChangePushedTexture( int x, int y, int width, int height )
{
	ButtonOverlay overlay;
	overlay.bTemplate = false;
	overlay.uv.x = x;
	overlay.uv.y = y;
	overlay.uv.w = width;
	overlay.uv.h = height;
	m_OverlayTexUV["PushedTexture"] = overlay;
}

void Button::ChangeDisableTexture( int x, int y, int width, int height )
{
	ButtonOverlay overlay;
	overlay.bTemplate = false;
	overlay.uv.x = x;
	overlay.uv.y = y;
	overlay.uv.w = width;
	overlay.uv.h = height;
	m_OverlayTexUV["DisabledTexture"] = overlay;
}

void Button::SetOverlayRegion( int offsetX, int offsetY, int width, int height )
{
	m_pOverlayRegion->SetPoint( "topleft", GetName(), "topleft", offsetX, offsetY );
	m_pOverlayRegion->SetSize( width, height );
}

void Button::ClearPushState()
{
	m_State &= ~FRMSTATE_CLICKDOWN;
}

void Button::SetNormalTextureTemplate( const char* template_name )
{
	m_pNormalRegion->SetTextureTemplate( template_name );
	g_pDisplay->ReleaseUIRes(m_hNormalTex);
	m_hNormalTex = AssignHUIRes(m_pNormalRegion->m_hTex);
}

void Button::SetHighlightTextureTemplate( const char* template_name )
{
	UIObject* pObj = g_pFrameMgr->getTemplateObject(template_name);
	if( pObj != NULL && strcmp(pObj->GetTypeName(),"Texture") ==0 )
	{
		g_pDisplay->ReleaseUIRes(m_hHighlightTex);
		m_hHighlightTex = AssignHUIRes(((::Texture*)pObj)->m_hTex);
		ButtonOverlay overlay;
		overlay.bTemplate = true;
		overlay.templateName = template_name;
		m_OverlayTexUV["HighlightTexture"] = overlay;
	}
}

void Button::SetPushedTextureTemplate( const char* template_name )
{
	UIObject* pObj = g_pFrameMgr->getTemplateObject(template_name);
	if( pObj != NULL && strcmp(pObj->GetTypeName(),"Texture") ==0 )
	{
		g_pDisplay->ReleaseUIRes(m_hPushedTex);
		m_hPushedTex = AssignHUIRes(((::Texture*)pObj)->m_hTex);
		ButtonOverlay overlay;
		overlay.bTemplate = true;
		overlay.templateName = template_name;
		m_OverlayTexUV["PushedTexture"] = overlay;
	}
}
void Button::SetDisableTextureTemplate( const char* template_name )
{
	UIObject* pObj = g_pFrameMgr->getTemplateObject(template_name);
	if( pObj != NULL && strcmp(pObj->GetTypeName(),"Texture") ==0 )
	{
		g_pDisplay->ReleaseUIRes(m_hDisableTex);
		m_hDisableTex = AssignHUIRes(((::Texture*)pObj)->m_hTex);
		ButtonOverlay overlay;
		overlay.bTemplate = true;
		overlay.templateName = template_name;
		m_OverlayTexUV["DisabledTexture"] = overlay;
	}
}

void Button::SetNormalTextColor(int r,int g,int b)
{
	m_NormalColor = Ogre::ColorQuad(r,g,b);
}
void Button::SetHighlightTextColor(int r,int g,int b)
{
	m_HighlightColor = Ogre::ColorQuad(r,g,b);
}
void Button::SetDisableTextColor(int r,int g,int b)
{
	m_DisableColor = Ogre::ColorQuad(r,g,b);
}
void Button::SetPushedTextColor(int r,int g,int b)
{
	m_PushedColor = Ogre::ColorQuad(r,g,b);
}

void Button::OnBeginDrag(unsigned int millisecond, int dx, int dy)
{
	int w = GetWidth();
	int h = GetHeight();
	int distance = dx * dx + dy * dy;
	if (millisecond >= 0 && (abs(dx) >= w / 4 || abs(dy) >= h / 4 || distance >= 200))
	{
		g_pFrameMgr->SetBeginDragFrame();
		if (hasScriptsEvent(SC_ONBEGINDRAG))
		{
			m_State &= ~FRMSTATE_CLICKDOWN;
			CallScript(SC_ONBEGINDRAG);
		}
	}
}

void Button::SetButtonColor( int r,int g,int b )
{
	if( m_pNormalRegion )
	{
		m_pNormalRegion->SetColor(r,g,b);
	}
	if( m_pOverlayRegion )
	{
		m_pOverlayRegion->SetColor(r,g,b);
	}	
}

/*
//-----------------------------------------------------------------------------
// button lua function
//-----------------------------------------------------------------------------
DECLARE_CLASS_TO_SCRIPT_EX(Button, Frame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

//Button
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetCooldownTimer, SVT_EMPTY, SVT_FLOAT, SVT_FLOAT, SVT_BOOL);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, IsCooldown, SVT_BOOL);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetNormalRegionWidth, SVT_EMPTY, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, GetNormalRegionWidth, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetNormalTexture, SVT_EMPTY, SVT_PTR_STRING, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, GetNormalTexture, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetNormalTextureHuires, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, GetNormalTextureHuires, SVT_INT);
//DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetPushedTexture, SVT_EMPTY, SVT_INT);
//DECLARE_MEM_FUNC_TO_SCRIPT(Button, GetPushedTexture, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetText, SVT_EMPTY, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, GetText, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetIntonateTimer, SVT_EMPTY, SVT_FLOAT, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetHightlightTexture, SVT_EMPTY, SVT_PTR_STRING, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetPushedState, SVT_EMPTY);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, GetPushedState, SVT_BOOL);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetDisableTextureHuires, SVT_EMPTY, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, GetDisableTextureHuires, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, Disable);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, Enable);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, Highlight);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, DisHighlight);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, OnLButtonDown, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, OnLButtonUp, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, SetAngle, SVT_EMPTY, SVT_FLOAT);
DECLARE_MEM_FUNC_TO_SCRIPT(Button, GetAngle, SVT_FLOAT);
*/
