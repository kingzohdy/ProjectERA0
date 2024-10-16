#include "ui_checkbutton.h"
#include "ui_layoutframe.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// CheckButton control
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

IMPLEMENT_UIOBJECT_CLONE(CheckButton)

CheckButton::CheckButton()
{
	m_Type = FRM_CHECKBUTTON;
	m_State = 0;

	m_bChecked = false;

	m_Caption = "";
}

CheckButton::~CheckButton()
{
	//g_pDisplay->ReleaseUIRes(m_hDisabledTex);
	//g_pDisplay->ReleaseUIRes(m_hEnabledTex);

	//delete m_pCheckRegion;
}

void CheckButton::CopyMembers(CheckButton *pdest)
{
	if (pdest)
	{
		Button::CopyMembers(pdest);

		pdest->m_bChecked = m_bChecked;
	}
}

//void CheckButton::SetCurrentTexture(HUIRES hTex)
//{
//	m_pCheckRegion->SetTexture(hTex);
//}
//
//HUIRES CheckButton::GetCurrentTexture()
//{
//	if (m_bChecked)
//	{
//		return m_hEnabledTex;
//	}else
//	{
//		return m_hDisabledTex;
//	}
//}

int CheckButton::OnInputMessage(const Ogre::InputEvent &event)
{
	if( ( m_State & FRMSTATE_DISABLE ) != 0 )
	{
		return 0;
	}

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

				m_bChecked = !m_bChecked;

				if (hasScriptsEvent(SC_ONCLICK))
				{
					CallScript(SC_ONCLICK);
				}
			}
			return 0;
		}

	case GIE_MOUSEIN:
		{			
			if (m_bShow)
			{
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
		}
	}
	return 1;
}

void CheckButton::UpdateSelf(float deltatime)
{
	if (m_bShow)
	{
		Frame::UpdateSelf(deltatime);
	}
}

void CheckButton::FillDrawItems()
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

void CheckButton::Draw()
{
	Frame::Draw();

	if (m_bChecked)
	{
		//SetCurrentTexture(m_hEnabledTex);

		if ((m_hNormalTex != (HUIRES)0) && (m_hPushedTex != (HUIRES)0))
		{
			m_pNormalRegion->DrawShow(false);
			m_pOverlayRegion->DrawShow(true);
		}
	}
	else
	{
		//SetCurrentTexture(m_hDisabledTex);
		if (m_hNormalTex != (HUIRES)0)
		{
			m_pNormalRegion->DrawShow(true);
			m_pOverlayRegion->DrawShow(false);
		}
	}

	HUIRES h = g_pFrameMgr->m_HMAYFONT[m_pNormalFontString->m_iFontIndex].h;
	if (this == g_pFrameMgr->GetFocusFrame() && (h != NULL))
	{	
		const char *str = m_pNormalFontString->m_Caption.c_str();

		float fsizex = 0.0f;
		float fsizey = 0.0f;
		g_pDisplay->GetTextExtent(g_pFrameMgr->m_HMAYFONT[m_pNormalFontString->m_iFontIndex].h, str, fsizex, fsizey);
		//int count=g_pDisplay->GetTextExtent(m_hNormalFont,str,strlen(str),-1,sizex,sizey);

		//checkbutton在激活状态,阴影显示文字
		//获取的宽度有问题,暂时用控件的宽度绘制
		g_pDisplay->BeginDraw(g_pDisplay->GetNullTexture());
		g_pDisplay->StretchRect((float)m_pNormalFontString->m_AbsRect.m_Left, (float)m_pNormalFontString->m_AbsRect.m_Top, 
			fsizex, (float)m_pNormalFontString->m_AbsRect.getHeight(), ColorQuad(0, 0, 0, 20).c,false);
		g_pDisplay->EndDraw();
	}
}

TiXmlElement *CheckButton::Save(TiXmlElement *pElement)
{
	TiXmlElement *pCheckButton = Frame::Save(pElement);
	
	if (m_bChecked)
	{
		pCheckButton->SetAttribute("checked", "true");
	}
	else
	{
		pCheckButton->SetAttribute("checked", "false");
	}

	if (m_pNormalFontString)
	{
		m_pNormalFontString->Save(pCheckButton);
	}

	return pCheckButton;
}

bool CheckButton::GetCheckState()
{
	return m_bChecked;
}

void CheckButton::setCheckState( bool bChecked )
{ 
	m_bChecked = bChecked;
	if (m_bChecked)
	{
		//SetCurrentTexture(m_hEnabledTex);

		if ((m_hNormalTex != (HUIRES)0) && (m_hPushedTex != (HUIRES)0))
		{
			m_pNormalRegion->DrawShow(false);
			m_pOverlayRegion->DrawShow(true);
		}
	}
	else
	{
		//SetCurrentTexture(m_hDisabledTex);
		if (m_hNormalTex != (HUIRES)0)
		{
			m_pNormalRegion->DrawShow(true);
			m_pOverlayRegion->DrawShow(false);
		}
	}
};



/*
//-----------------------------------------------------------------------------
// checkbutton lua function
//-----------------------------------------------------------------------------
DECLARE_CLASS_TO_SCRIPT_EX(CheckButton, Button, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

DECLARE_MEM_FUNC_TO_SCRIPT(CheckButton, GetCheckState, SVT_BOOL);
*/