#pragma warning( disable : 4996 )

#include <math.h>
#include <string>
#include "ui_editbox.h"
#include "ui_framemgr.h"
#include "ui_fontstring.h"


using namespace Ogre;
using namespace std;

//-----------------------------------------------------------------------------
// EditBox control
//-----------------------------------------------------------------------------
IMPLEMENT_UIOBJECT_CLONE(EditBox)

extern FrameManager *g_pFrameMgr;

const int EDIT_MAX_VALUE_DEFAULT = 2147483647;//2^31-1,最大整数

EditBox::EditBox() : m_pFontString(NULL)
{
	m_Type = FRM_EDITBOX;

	m_Style = FRMSTYLE_EDITABLE|FRMSTYLE_CLIPSELF;

	m_NumHistory = 0;
	m_CurHistory = -1;

	m_MaxChar = EDIT_MAX_VALUE_DEFAULT;
	m_bMultiLine = false;

	m_CursorPos = 0;
	m_DispBegin = 0;
	m_SelBegin = 0;

	m_CursorTimer = 0;
	m_CursorFlag = 1;
	m_CursorLive = 500;
	m_CursorDeath = 400;

	m_MaxValue = EDIT_MAX_VALUE_DEFAULT;
	m_MinValue = 0.0;

	m_EditSelColor = ColorQuad(0, 0, 255, 128);

	m_EditMethod = UI_EDITMETHOD_ENGLISH|UI_EDITMETHOD_NUMBER|UI_EDITMETHOD_SYMBOL|UI_EDITMETHOD_CHINESE|UI_EDITMOTHOD_UNDERLINE;
}

EditBox::~EditBox()
{
	OGRE_RELEASE(m_pFontString);
}

void EditBox::CopyMembers(EditBox *pdest)
{
	if (pdest)
	{
		Frame::CopyMembers(pdest);

		pdest->m_DrawRegions.resize(0);

		if(m_pFontString)
		{
			pdest->m_pFontString = static_cast<FontString *>(m_pFontString->CreateClone());
			pdest->AddFontString(DL_ARTWORK, pdest->m_pFontString);
		}

		//if (m_hFontString)
		//	pdest->m_hFontString = AssignHUIRes(m_hFontString);

		pdest->m_MaxChar = m_MaxChar;
		pdest->m_bMultiLine = m_bMultiLine;
		pdest->m_NumHistory = m_NumHistory;
		pdest->m_EditMethod = m_EditMethod;

		//pdest->m_Caption = m_Caption;
		pdest->m_CursorPos = m_CursorPos;
		pdest->m_DispBegin = m_DispBegin;
		pdest->m_SelBegin = m_SelBegin;
		pdest->m_CursorTimer = m_CursorTimer;
		pdest->m_CursorFlag = m_CursorFlag;
		pdest->m_CursorLive = m_CursorLive;
		pdest->m_CursorDeath = m_CursorDeath;
		pdest->m_Value = m_Value;
		pdest->m_MaxValue = m_MaxValue;
		pdest->m_MinValue = m_MinValue;
		pdest->m_InputFlag = m_InputFlag;
		pdest->m_History = m_History;

		pdest->m_CurHistory = m_CurHistory;
		pdest->m_EncryptCaption = m_EncryptCaption;
		pdest->m_EditSelColor = m_EditSelColor;
		pdest->m_GrayColor = m_GrayColor;
		//pdest->m_hFontString = m_hFontString;
	}
}

void EditBox::UpdateSelf(float deltatime)
{
	if (m_bShow)
	{
		Frame::UpdateSelf(deltatime);

		CheckForReason();

		//闪烁光标
		//if((m_ExtendStyle&EDIT_STYLE_FLASHCURSOR)||(g_pRootWgt->GetFocusWgt() == this))
		if(g_pFrameMgr->GetFocusFrame() == this)
		{
			int dtick = int(deltatime*1000);
			m_CursorTimer += dtick;
			if( (m_CursorFlag && m_CursorTimer>m_CursorLive) || (!m_CursorFlag && m_CursorTimer>m_CursorDeath) )
			{
				m_CursorFlag = 1 - m_CursorFlag;
				m_CursorTimer = 0;
			}
		}
		else 
		{
			m_CursorFlag = 1;
			m_CursorTimer = 0;
		}
	}
}

void EditBox::FillDrawItems()
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

int EditBox::OnInputMessage(const Ogre::InputEvent &event)
{
	switch(event.msg)
	{
	case GIE_CHAR:
		{
			return	OnChar(event);
		}

	case GIE_KEYDOWN:
		{
			return OnKeyDown(event);
		}

	case GIE_MOUSEMOVE:
		{
			int x=event.mouse.x;
			int y=event.mouse.y;

			if( m_State & FRMSTATE_CLICKDOWN )
			{
				Ogre::RectInt abscaprect= m_AbsRect;
				int width=x-abscaprect.m_Left;

				//EditBox窗口的自动滚动
				if(width < 10)
				{
					if((m_CursorPos >= 2) && IsDBCSHead(m_pFontString->m_Caption.c_str(),m_CursorPos-2))
						m_CursorPos -= 2;
					else if(m_CursorPos > 0) 
						m_CursorPos -= 1;					
				}
				else if(width>abscaprect.getWidth()-10) 
				{
					if( (size_t)m_CursorPos < m_pFontString->m_Caption.size()
						&& IsDBCSHead(m_pFontString->m_Caption.c_str(), m_CursorPos) ) 
						m_CursorPos += 2;
					else if( (size_t)m_CursorPos < m_pFontString->m_Caption.size())
						m_CursorPos += 1;
				}
				else 
				{
					const char* str = m_pFontString->m_Caption.c_str() + m_DispBegin;

					int numchar = 0;
					float fRealWidth;
					HUIRES h = g_pFrameMgr->m_HMAYFONT[m_pFontString->m_iFontIndex].h;
					if (h != NULL)
					{
						g_pDisplay->GetTextExtentFitInWidth(h,
							m_pFontString->m_Caption.c_str(), float(width + 0.5f), fRealWidth, numchar);
						m_CursorPos = numchar+m_DispBegin;
					}
				}

				CheckForReason();
			} 
		}
		return 0;

	case GIE_LBTNUP:
		{
			if( m_State & FRMSTATE_CLICKDOWN )
			{	
				m_State &= ~FRMSTATE_CLICKDOWN;
				//CallSafeScript(SCRIPT_FUNC_ON_CLICK);
			}	
			return 0;
		}

	case GIE_LBTNDBLCLK:
		{		
			SelectAllText();

			//CallSafeScript(SCRIPT_FUNC_ON_DBCLICK);	
			return 0;
		}

	case GIE_LBTNDOWN:
		{
			m_State |= FRMSTATE_CLICKDOWN;

			g_pFrameMgr->SetFocusFrame(this);

			int width = event.mouse.x-m_AbsRect.m_Left;

			if(width<0)
			{
				width = 0;
			}
			else if(width > m_AbsRect.getWidth())
			{
				width = m_AbsRect.getWidth();
			}

			const char* str = m_pFontString->m_Caption.c_str() + m_DispBegin;

			int numchar = 0;
			float fRealWidth;
			if (g_pFrameMgr->m_HMAYFONT[m_pFontString->m_iFontIndex].h != NULL)
			{
				g_pDisplay->GetTextExtentFitInWidth(g_pFrameMgr->m_HMAYFONT[m_pFontString->m_iFontIndex].h,
					str, float(width + 0.5f), fRealWidth, numchar);

				m_CursorPos = numchar + m_DispBegin;

				CheckForReason();

				m_SelBegin = m_CursorPos;
			}
		}

		return 0;

	case GIE_LOSTFOCUS:
		{
			//if(!(m_ExtendStyle&EDIT_STYLE_KEEPSEL))
			CancelSel(true,false);
		}

		return 0;	

	default:
		{
			return Frame::OnInputMessage(event);
		}
	}

	return 1;
}

//选择文字
//返回值:0-失败,1-成功
int EditBox::SetSel(int start,int end)
{
	if(start < 0)
		start = 0;
	else if((size_t)start > m_pFontString->m_Caption.size())
		start = (int)m_pFontString->m_Caption.size();

	if(end < 0)
		end = 0;
	else if((size_t)end > m_pFontString->m_Caption.size())
		end = (int)m_pFontString->m_Caption.size();

	m_SelBegin = start;
	m_CursorPos = end;

	return 1;
}

//void EditBox::SetFont(std::string fontname)
//{
//	//m_hFontString = g_pDisplay->CreateGameFont(fontname.c_str());
//	//m_pFontString->SetFont(m_hFontString);
//
//	m_pFontString->SetFont(g_pDisplay->CreateGameFont(fontname.c_str()));
//}
//
//HUIRES EditBox::GetFont()
//{
//	//return m_hFontString;
//	return 0;
//}

bool EditBox::IsCursorVisible()
{
	//绘制光标
	bool bCursorVis = false;
	//if(IsEnabled() && (m_SelBegin == m_CursorPos))
	if(m_SelBegin == m_CursorPos)
	{
		if(g_pFrameMgr->GetFocusFrame() == this)
		{
			bCursorVis = true;
		}
		//else if(m_ExtendStyle&EDIT_STYLE_KEEPCURSOR)
		//{			
		//	bCursorVis=true;			
		//}
		//else if(m_ExtendStyle&EDIT_STYLE_SMARTCURSOR)
		//{	
		//	/*
		//	CUIWidget* pFocusWgt=g_pRootWgt->GetFocusWgt();

		//	if(pFocusWgt&&pFocusWgt->IsAcceptInput())
		//	bCursorVis=false;
		//	else
		//	bCursorVis=true;
		//	*/
		//	bCursorVis=(g_pRootWgt->GetHotWgt()==this);

		//}
	}	

	if(bCursorVis && m_CursorFlag) 
		return true;
	else 
		return false;
}

void EditBox::CheckForReason()
{
	assert(m_SelBegin >= 0);
	assert((m_CursorPos >= 0) && ((size_t)m_CursorPos <= m_pFontString->m_Caption.size()));
	assert(m_DispBegin >= 0);

	//确保光标位置合理
	if(m_SelBegin < 0)
		m_SelBegin = 0;
	else if((size_t)m_SelBegin > m_pFontString->m_Caption.size())
		m_SelBegin = (int)m_pFontString->m_Caption.size();

	if(m_CursorPos < 0)
		m_CursorPos = 0;
	else if((size_t)m_CursorPos > m_pFontString->m_Caption.size())
		m_CursorPos = (int)m_pFontString->m_Caption.size();

	if(m_DispBegin < 0)
		m_DispBegin = 0;
	else if(m_DispBegin > m_CursorPos)
		m_DispBegin = m_CursorPos;

	const char* str = m_pFontString->m_Caption.c_str() + m_DispBegin;
	int numChar = m_CursorPos - m_DispBegin;

	float fRealWidth = 0.0f;
	float fRealHeight = 0.0f;
	int numchar = 0;

	std::string strTemp = m_pFontString->m_Caption.substr(m_DispBegin, m_CursorPos-m_DispBegin);
	HUIRES h = g_pFrameMgr->m_HMAYFONT[m_pFontString->m_iFontIndex].h;
	if (h != NULL)
	{
		g_pDisplay->GetTextExtent(h, strTemp.c_str(), fRealWidth, fRealHeight);

		float fwidth = (float)(m_AbsRect.getWidth() + 0.5f);
		float fTempWidth = 0.0f;
		g_pDisplay->GetTextExtentFitInWidth( g_pFrameMgr->m_HMAYFONT[m_pFontString->m_iFontIndex].h,
			m_pFontString->m_Caption.c_str(), fwidth, fTempWidth, numchar);

		//can't print all
		if( fRealWidth > m_AbsRect.getWidth() && m_AbsRect.getWidth() != 0 )
		{
			// TODO:绘制区域超出可视区域后开始位置的字符索引+1 m_DispBegin += numChar - numchar +1;
			m_DispBegin ++;
			if( m_DispBegin < 0 )
			{
				m_DispBegin = 0;
			}
			if( IsDBCSTail( m_pFontString->m_Caption.c_str(), m_DispBegin ) )
			{
				m_DispBegin++;
			}

		}
	}
}

void EditBox::Draw()
{
	Frame::Draw();

	int cursorx = 0;
	float fcursorx = 0.0f;
	float fcursory = 0.0f;
	int offsetx = 0;
	if(!m_pFontString->m_Caption.empty())
	{
		const char *str = m_pFontString->m_Caption.c_str() + m_DispBegin;

		float fTempWidth = float(m_AbsRect.getWidth() + 0.5f);
		float fRealWidth = 0.0f;
		int iWidthCount = 0;

		HUIRES h = g_pFrameMgr->m_HMAYFONT[m_pFontString->m_iFontIndex].h;
		if ((m_CursorPos > m_DispBegin) && (h != NULL))
		{
			//cursorx
			std::string strTemp = m_pFontString->m_Caption.substr(m_DispBegin, m_CursorPos - m_DispBegin);

			g_pDisplay->GetTextExtent(h, strTemp.c_str(), fcursorx, fcursory);
		}

		RectFloat rc;
		rc.m_Top = (float)m_AbsRect.m_Top;
		rc.m_Left = (float)m_AbsRect.m_Left;
		rc.m_Bottom = (float)m_AbsRect.m_Bottom;
		rc.m_Right = (float)m_AbsRect.m_Right;
		if (m_pFontString->m_JustifyH != JUSTIFYH_LEFT)
		{
			float fcursorx = 0.0f;
			float fcursory = 0.0f;
			const UIFont& oneUIFontConfig = g_pFrameMgr->getUIFontByIndex(m_pFontString->m_iFontIndex);
			g_pDisplay->GetTextExtent(oneUIFontConfig.h, str, fcursorx, fcursory);
			fcursorx = fcursorx * oneUIFontConfig.scale;
			if (m_pFontString->m_JustifyH == JUSTIFYH_CENTER)
			{
				offsetx = FloatToInt(((m_AbsRect.m_Right - m_AbsRect.m_Left) - fcursorx)/2);
			}
			else if (m_pFontString->m_JustifyH == JUSTIFYH_RIGHT)
			{
				offsetx = FloatToInt((m_AbsRect.m_Right - m_AbsRect.m_Left) - fcursorx);
			}
			else
			{
				assert(0 && "Fontstring JustfyH error!");
			}
		}
		g_pDisplay->renderTextRect(g_pFrameMgr->m_HMAYFONT[m_pFontString->m_iFontIndex].h, m_pFontString->m_FontStyle, 
			str, rc, offsetx, 0, false, m_pFontString->m_TextColor, 1.0f*m_fSelfScale);

		float fTempy = 0.0f;
		//draw selection
		if((h != NULL) && (m_SelBegin != m_CursorPos) && (g_pFrameMgr->GetFocusFrame() == this))
		{
			float dispBeginX = 0.0f;
			float selBeginX = 0.0f;
			float selEndX = 0.0f;

			if (m_DispBegin > 0)
			{
				float strLend = 0.0f;
				g_pDisplay->GetTextExtent(h, m_pFontString->m_Caption.c_str()+m_DispBegin, dispBeginX, fTempy);
				g_pDisplay->GetTextExtent(h, m_pFontString->m_Caption.c_str()+m_DispBegin, strLend, fTempy);
				dispBeginX = dispBeginX - strLend;
			}
			if (m_SelBegin > 0)
			{
				g_pDisplay->GetTextExtent(h, m_pFontString->m_Caption.substr(m_DispBegin, m_SelBegin-m_DispBegin).c_str(),
					selBeginX, fTempy);
			}
			selBeginX -= dispBeginX;

			if (m_CursorPos > 0)
			{
				g_pDisplay->GetTextExtent(h, m_pFontString->m_Caption.substr(m_DispBegin, 
					m_CursorPos-m_DispBegin).c_str(), selEndX, fTempy);
			}
			selEndX -= dispBeginX;

			if(selEndX < 0) selEndX = 0;
			else if(selEndX > m_AbsRect.getWidth()) selEndX = (float)(m_AbsRect.getWidth() +0.5f);

			if(selBeginX < 0) selBeginX=0;
			else if(selBeginX > m_AbsRect.getWidth()) selBeginX = (float)(m_AbsRect.getWidth()+0.5f);

			int barBeginX = int(selBeginX);
			int barEndX = int(selEndX);
			if(selBeginX > selEndX) 
			{
				barBeginX = int(selEndX);
				barEndX = int(selBeginX);
			}
			g_pDisplay->BeginDraw(g_pDisplay->GetNullTexture());
			g_pDisplay->DrawRect(m_AbsRect.m_Left+barBeginX+offsetx, m_AbsRect.m_Top, barEndX-barBeginX,
				m_AbsRect.getHeight(), m_EditSelColor.c);
			g_pDisplay->EndDraw();
		}
	}
	else
	{
		if (m_pFontString->m_JustifyH != JUSTIFYH_LEFT)
		{
			if (m_pFontString->m_JustifyH == JUSTIFYH_CENTER)
			{
				offsetx = FloatToInt((m_AbsRect.m_Right - m_AbsRect.m_Left) / 2);
			}
			else if (m_pFontString->m_JustifyH == JUSTIFYH_RIGHT)
			{
				offsetx = FloatToInt(m_AbsRect.m_Right - m_AbsRect.m_Left);
			}
			else
			{
				assert(0 && "Fontstring JustfyH error!");
			}
		}
	}

	cursorx = FloatToInt(fcursorx) + offsetx;
	//cursorx = 2;

	//draw cursor
	if ( IsCursorVisible() )
	{
		g_pDisplay->BeginDraw(g_pDisplay->GetNullTexture());
		g_pDisplay->DrawRect( m_AbsRect.m_Left + min( m_AbsRect.getWidth(), cursorx ), 
			m_AbsRect.m_Top, 1, m_AbsRect.getHeight(), 
			ColorQuad(200, 200, 200, 255).c );
		g_pDisplay->EndDraw();
	}
}

TiXmlElement *EditBox::Save(TiXmlElement *pElement)
{
	TiXmlElement *pEditBox = Frame::Save(pElement);

	//letters
	if (m_MaxChar != EDIT_MAX_VALUE_DEFAULT)
	{
		pEditBox->SetAttribute("letters", m_MaxChar);
	}

	//multiline
	if (m_bMultiLine)
	{
		pEditBox->SetAttribute("multiLine", "true");
	}

	//historyLines
	if (m_NumHistory > 0)
	{
		pEditBox->SetAttribute("historyLines", m_NumHistory);		
	}

	//password
	if( m_EditMethod & UI_EDITMETHOD_CODER )
	{
		pEditBox->SetAttribute("password", "true");
	}

	//fontstring
	if (m_pFontString)
	{
		TiXmlElement *pFontString = new TiXmlElement("FontString");
		pEditBox->LinkEndChild(pFontString);
		//fonttype
		if (!m_pFontString->m_FontName.empty())
		{
			pFontString->SetAttribute("fonttype", m_pFontString->m_FontName.c_str());
		}
	}

	return pEditBox;
}

void EditBox::SetPassword( const char *ptext )
{
	SetText( ptext );
	Encrypt();
}

int EditBox::OnChar(const Ogre::InputEvent &event, bool bVirtual)
{
	char thechar = event.chars.str[0];
	if (hasScriptsEvent(SC_ONCHAR))
	{
		CallScript( SC_ONCHAR, "iii", thechar, m_CursorPos, m_SelBegin );
	}
	if( !(m_Style&FRMSTYLE_EDITABLE) )
	{
		return 1;
	}
	//非打印字符
	switch(thechar) 
	{			
	case VK_BACK:
		//if(IsEditable())
		{
			if ( ( m_EditMethod & UI_EDITMETHOD_DISABLE ) != 0 )
			{
				return 1;
			}

			if(!ClearSel())
			{
				if( m_CursorPos <= 0 ) break;
				if(IsDBCSChar(m_pFontString->m_Caption.c_str(), m_CursorPos-1) )
				{				 
					if(m_CursorPos >= 2)
					{
						m_CursorPos -= 2;
						m_pFontString->m_Caption.erase( m_CursorPos, 2 );

						if(m_DispBegin >= 2) m_DispBegin -= 2;
					}
					else if(m_CursorPos > 0)
					{
						m_CursorPos -= 1;
						m_pFontString->m_Caption.erase( m_CursorPos, 1 );

						if (m_DispBegin > 0) m_DispBegin -= 1;
					}
				}
				//光标前是E文
				else
				{
					if(m_DispBegin >= 1&&!IsDBCSTail(m_pFontString->m_Caption.c_str(), m_DispBegin-1))	
						m_DispBegin -= 1;

					m_CursorPos -= 1;
					if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
					m_pFontString->m_Caption.erase( m_CursorPos, 1 );
					if( m_EditMethod & UI_EDITMETHOD_CODER )
					{
						Encrypt();
					}

				}
				m_SelBegin = m_CursorPos;

				if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
				{
					CaptionToValue();
				}
				OnChangeText();
			}
		}
		return 1;

	case VK_RETURN:	//enterpressed
		{
			if (hasScriptsEvent(SC_ONENTERPRESSED))
			{
				CallScript(SC_ONENTERPRESSED);
				return 0;
			}else
			{
				return Frame::OnInputMessage(event);
			}
		}
		return 1;

	case VK_ESCAPE:	//enterpressed
		{
			if (hasScriptsEvent(SC_ONESCAPEPRESSED))
			{
				CallScript(SC_ONESCAPEPRESSED);
				return 0;
			}else
			{
				return Frame::OnInputMessage(event);
			}
		}
		return 1;

	case VK_TAB:
		{
			if (hasScriptsEvent(SC_ONTABPRESSED))
			{
				CallScript(SC_ONTABPRESSED);
				return 0;
			}else
			{
				return Frame::OnInputMessage(event);
			}
		}
		return 1;
	default:
		break;
	}

	//非打印字符
	if(!IsPrint(thechar) || !IsLegal(thechar) )
	{
		return Frame::OnInputMessage(event);
	}

	if ( hasScriptsEvent( SC_ONQUERYCHAR ) )
	{
		bool isContinueGetChar = true;
		CallFunction( SC_ONQUERYCHAR, "ii>b", m_CursorPos, m_SelBegin, &isContinueGetChar );
		if ( !isContinueGetChar )
		{
			return 0;
		}
	}

	ClearSel();
	//SetUpdateDirty(true);

	if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();

	char buffer[2];
	buffer[0] = (char)thechar;
	buffer[1] = 0;

	if(QueryInputFlag(buffer[0]))
	{
		m_pFontString->m_Caption.insert( m_CursorPos, buffer );
		m_CursorPos++;
		m_SelBegin=m_CursorPos;
		if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
		{
			SetVar(0, m_pFontString->m_Caption.c_str());
		}
		OnChangeText();
	}

	////{{{密码控件改进
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
	////}}}

	//截流指定消息,自己处理,不向父亲发送	
	return 0;
}

int EditBox::OnKeyDown(const Ogre::InputEvent &event, bool bVirtual)
{
	//{{{密码控件改进
	if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
	//}}}
	bool bDefault=false;
	switch(event.key.vkey)
	{
	case VK_LEFT:
		{
			//按下了ctrl键,同home
			if((event.key.flags&MK_CONTROL))
			{
				m_CursorPos = 0;
			}
			else 
			{
				if (!(event.key.flags&MK_SHIFT))
				{
					if(CancelSel(true))	break;
				}
				////没有按下shift键
				//if((lparam&MK_SHIFT)==0)
				//	if(CancelSel(true))	break;

				if(m_CursorPos>=2&&IsDBCSHead(m_pFontString->m_Caption.c_str(),m_CursorPos-2)) 
					m_CursorPos -= 2;
				else if(m_CursorPos>0) 
					m_CursorPos -= 1;

				assert( (size_t)m_CursorPos <= m_pFontString->m_Caption.size() );
			}

			if (!(event.key.flags&MK_SHIFT))
			{
				m_SelBegin=m_CursorPos;
			}
			////没有按下shift键
			//if((lparam&MK_SHIFT)==0)
			//	m_SelBegin=m_CursorPos;
			break;
		}

	case VK_RIGHT:
		{
			//按下了ctrl键,同end
			if((event.key.flags&MK_CONTROL))
			{
				m_CursorPos = (int)m_pFontString->m_Caption.size();
			}
			else 
			{
				//没有按下shift键
				//if((lparam&MK_SHIFT)==0)
				if (!(event.key.flags&MK_SHIFT))
					if(CancelSel(false))	break;

				if( (size_t)m_CursorPos<m_pFontString->m_Caption.size()&&IsDBCSHead(m_pFontString->m_Caption.c_str(), m_CursorPos) ) 
					m_CursorPos += 2;
				else if( (size_t)m_CursorPos<m_pFontString->m_Caption.size())
					m_CursorPos += 1;

				assert( (size_t)m_CursorPos <= m_pFontString->m_Caption.size() );
			}

			//没有按下shift键
			//if((lparam&MK_SHIFT)==0)
			if (!(event.key.flags&MK_SHIFT))
				m_SelBegin=m_CursorPos;
			break;
		}

	case VK_HOME:
		{
			m_CursorPos = 0;

			//没有按下shift键
			//if((lparam&MK_SHIFT)==0)
			if (!(event.key.flags&MK_SHIFT))
				m_SelBegin=m_CursorPos;
			break;
		}

	case VK_END:
		{
			m_CursorPos = (int)m_pFontString->m_Caption.size();

			//没有按下shift键
			//if((lparam&MK_SHIFT)==0)
			if (!(event.key.flags&MK_SHIFT))
				m_SelBegin=m_CursorPos;
			break;
		}
	case VK_BACK:
		{
			if (hasScriptsEvent(SC_ONKEYDOWN))
			{
				CallScript( SC_ONKEYDOWN, "iii", event.key.vkey, ( m_CursorPos + 1 ),(  m_SelBegin + 1 ) );
			}
			break;
		}
	case VK_DELETE:
		{
			//if(IsEditable())
			{
				if (hasScriptsEvent(SC_ONKEYDOWN))
				{
					CallScript( SC_ONKEYDOWN, "iii", event.key.vkey, ( m_CursorPos + 1 ),(  m_SelBegin + 1 ) );
				}

				if(!ClearSel())
				{
					if( m_CursorPos == m_pFontString->m_Caption.size() ) break;

					if( IsDBCSHead(m_pFontString->m_Caption.c_str(), m_CursorPos) )
					{
						if( m_pFontString->m_Caption.size() >= (size_t)m_CursorPos+2 )
							m_pFontString->m_Caption.erase( m_CursorPos, 2 );
						else if( m_pFontString->m_Caption.size() >= (size_t)m_CursorPos+1 )
							m_pFontString->m_Caption.erase( m_CursorPos, 1 );
					}
					else
					{
						if( m_pFontString->m_Caption.size() >= (size_t)m_CursorPos+1 )
							m_pFontString->m_Caption.erase( m_CursorPos, 1 );
					}

					if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
					{
						CaptionToValue();
					}
					OnChangeText();
				}
			}
			break;
		}

	case VK_UP:
		{
			if(!m_History.empty())
			{		
				if( m_CurHistory+1 < m_History.size() )
				{
					m_CurHistory++;
					m_pFontString->m_Caption = m_History[m_CurHistory];
					m_CursorPos = (int)m_pFontString->m_Caption.size();
					m_DispBegin = 0;
					m_SelBegin=m_CursorPos;

					if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
					{
						CaptionToValue();
					}	

					if (hasScriptsEvent(SC_ONKEYDOWN))
					{
						CallScript( SC_ONKEYDOWN, "iii", event.key.vkey, ( m_CursorPos + 1 ),(  m_SelBegin + 1 ) );
					}
				}
			}
			break;
		}

	case VK_DOWN:
		{
			if(!m_History.empty())
			{
				if( m_CurHistory > 0 )
				{
					m_CurHistory--;
					m_pFontString->m_Caption = m_History[m_CurHistory];
					m_CursorPos = (int)m_pFontString->m_Caption.size();
					m_DispBegin = 0;
					m_SelBegin=m_CursorPos;

					if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
					{
						CaptionToValue();
					}

					if (hasScriptsEvent(SC_ONKEYDOWN))
					{
						CallScript( SC_ONKEYDOWN, "iiii", event.key.vkey, ( m_CursorPos + 1 ),(  m_SelBegin + 1 ), EDIT_EVENT_KEY );
					}
				}
			}
			break;
		}
	case 86:// Ctrl+V
		{
			if( (GetKeyState(VK_CONTROL) & 0xff00))
			{
				OnPaste();
			}
		}
		break;
	case 67:// Ctrl+C
		{
			if( (GetKeyState(VK_CONTROL) & 0xff00))
			{
				OnCopy();
			}
		}
		break;
	case 88:// Ctrl+X
		{
			if( (GetKeyState(VK_CONTROL) & 0xff00))
			{
				OnCut();
			}
		}
		break;
	default:
		{
			bDefault = true;
		}
	}

	if( ( GetKeyState(VK_MENU) & 0xff00 ) )
	{
		char szScript[256];
		sprintf( szScript, "Accelkey_AltGroup(\"%s\",%d)", GetName(), event.key.vkey );
		g_pUIScriptVM->callString( szScript );	
	}

	////add by lincoln [2007:4:1] [13:45]
	////{{{密码控件改进
	if( m_EditMethod & UI_EDITMETHOD_CODER )
	{
		Encrypt();
	}
	////}}}

	//if(bDefault) return Frame::OnKeyDown(wparam,lparam,bVirtual);

	return 0;
}

void EditBox::OnPaste()
{
	if(m_EditMethod&UI_EDITMOTHOD_DISABLE_PASTE) 
	{
		return;
	}
	if( !(m_Style&FRMSTYLE_EDITABLE) )
	{
		return;
	}
	//if(!IsEditable()) return;

	////{{{密码控件改进
	//if( m_EditMethod & UI_EDITMETHOD_CODER ) Decrypt();
	////}}}

	ClearSel();

	if( ::OpenClipboard(NULL) )
	{
		HANDLE handle = ::GetClipboardData(CF_TEXT);
		::CloseClipboard();
		if( handle )
		{
			std::string  pasteTxt((const char*)handle);

			for(size_t i=0;i<pasteTxt.size();i++)
			{ 
				char buffer[2];
				buffer[0] = pasteTxt[i];
				buffer[1] = 0;

				//非打印字符
				if(!IsPrint(buffer[0]) || !IsLegal(buffer[0]))
					continue;
				
				if(!QueryInputFlag(buffer[0]))
				{
					continue;
				}

				m_pFontString->m_Caption.insert( m_CursorPos, buffer );
				m_CursorPos++;
				m_SelBegin=m_CursorPos;
				OnChangeText();
			}
		}
	}

	////{{{密码控件改进
	//if( m_EditMethod & UI_EDITMETHOD_CODER ) Encrypt();
	////}}}

	if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
	{
		//SetVar(stUIVar(VALUE_CHANNEL_VALUE,0,atof(m_Caption.c_str()),0));
	}

	//SetUpdateDirty(true);


}

void EditBox::OnCopy()
{
	if(m_EditMethod&UI_EDITMETHOD_DISABLE_COPY) 
	{
		return;
	}

	std::string srcCopyStr;
	if( m_EditMethod & UI_EDITMETHOD_CODER )
	{
		srcCopyStr = m_EncryptCaption;
	}
	else
	{
		srcCopyStr = m_pFontString->m_Caption;
	}

	if(m_SelBegin!=m_CursorPos)
	{
		//得到选择文本
		int selBegin=min( m_SelBegin, m_CursorPos );
		int selEnd=max( m_SelBegin, m_CursorPos );

		std::string selText = srcCopyStr.substr(selBegin,selEnd-selBegin);
		if(selText.empty()) return;

		//得到共享内存
		HGLOBAL hGlobal=GlobalAlloc(GHND|GMEM_SHARE,selText.size()+1);
		if(!hGlobal) return;

		//填充共享内存
		LPVOID pGlobal=GlobalLock(hGlobal);
		strncpy((char*)pGlobal,selText.c_str(),selText.size()+1);
		GlobalUnlock(hGlobal);


		//设置剪贴板
		if( !::OpenClipboard(NULL) ) return;
		::EmptyClipboard();
		::SetClipboardData(CF_TEXT,hGlobal);
		::CloseClipboard();
	}
}

void EditBox::OnCut()
{
	if(m_EditMethod&UI_EDITMETHOD_DISABLE_COPY) 
	{
		return;
	}

	OnCopy();
	//if(!IsEditable()) return;

	////{{{密码控件改进
	// TODO
	/*if( m_EditMethod & UI_EDITMETHOD_CODER ) 
	{
		Decrypt();
	}*/
	////}}}

	ClearSel();

	////{{{密码控件改进
	// TODO
	/*if( m_EditMethod & UI_EDITMETHOD_CODER ) 
	{
		Encrypt();
	}*/

	//SetUpdateDirty(true);
}

void EditBox::OnChangeText()
{
	if ( hasScriptsEvent( SC_ONFINISHCHAR ) )
	{
		CallScript( SC_ONFINISHCHAR );
	}
}

void EditBox::SetVar(double fValue, const char *sValue)
{
	if (sValue)
	{
		if(m_Value!=fValue)
		{
			m_Value=fValue;	

			CheckValueRange();
		}

		ValueToCaption();
	}
}

void EditBox::ValueToCaption()
{
	char buffer[UI_TEMPORARY_BUFSIZE];

	sprintf(buffer,"%d",(int)m_Value);
	m_pFontString->m_Caption=buffer;
	m_CursorPos = (int)m_pFontString->m_Caption.size();
	m_DispBegin = 0;
	m_SelBegin=m_CursorPos;
}

//检查长度空位
//只要是caption改变的地方都应该调用该函数
void EditBox::UpdateInputFlag()
{
	if(m_MaxChar-m_pFontString->m_Caption.size()<1) 
		m_InputFlag=INPUT_FLAG_NONE;
	else if(m_MaxChar-m_pFontString->m_Caption.size()<2 && !IsDBCSHead(m_pFontString->m_Caption.c_str(),m_CursorPos-1)) 
		m_InputFlag=INPUT_FLAG_CHAR;
	else 
		m_InputFlag=INPUT_FLAG_WORD;

}

bool EditBox::QueryInputFlag(char key)
{
	char buffer[2];
	buffer[0]=key;
	buffer[1]=0;

	UpdateInputFlag();

	//检查长度
	if( m_InputFlag==INPUT_FLAG_NONE )
	{
		return false;
	}
	else if(m_InputFlag==INPUT_FLAG_CHAR)
	{
		if(IsDBCSChar(buffer,0) ) return false;//汉字
	}

	if ( ( m_EditMethod & UI_EDITMETHOD_DISABLE ) != 0 )
	{
		return false;
	}

	//mask屏蔽
	if(IsAlpha(buffer[0]))//英文
	{
		if((m_EditMethod&UI_EDITMETHOD_ENGLISH)==0)
			return false;
	}
	else if(IsDigit(buffer[0]))//数字
	{
		if((m_EditMethod&UI_EDITMETHOD_NUMBER)==0)
			return false;
	}
	else if(IsSymbol(buffer[0]))//符号
	{
		if((m_EditMethod&UI_EDITMETHOD_SYMBOL)==0)
			return false;
	}
	else if(IsDBCSChar(buffer,0))//汉字
	{
		if((m_EditMethod&UI_EDITMETHOD_CHINESE)==0)
			return false;	 
	}

	//value属性
	if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0)
	{
		if(!IsDigit(buffer[0]))
			return false;
	}

	//pwd属性
	if((m_EditMethod&UI_EDITMETHOD_CODER)!=0)
	{
		if(!IsPrint(buffer[0]) )
			return false;
	}
	return true;
}


// 检查字符串长度 截断超出部分
void EditBox::QueryInputChar( std::string& strSrc )
{
	//检查长度
	if( m_MaxChar - (int)strSrc.size() > 0 )
	{
		m_CursorPos += (int)strSrc.size();
		return;
	}else if( m_MaxChar - (int)strSrc.size() < 0 && IsDBCSHead( strSrc.c_str(),m_MaxChar-1) ){
		strSrc =  strSrc.substr( 0, ( m_MaxChar - 1) );
		m_CursorPos = m_MaxChar - 1;
	}else{
		strSrc =  strSrc.substr( 0, m_MaxChar );
		m_CursorPos = m_MaxChar;
	}
	if( m_CursorPos >(int)m_pFontString->m_Caption.size() )
	{
		m_CursorPos = m_pFontString->m_Caption.size();
	}
}


//返回1:之前有选择
//返回0:之前没有选择
int EditBox::ClearSel()
{
	//历史列表回到开始
	m_CurHistory = -1;	

	if(m_SelBegin!=m_CursorPos)
	{
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;
		if ( m_EditMethod & UI_EDITMETHOD_CODER )
		{
			m_pFontString->m_Caption = m_EncryptCaption;
		}		
		m_pFontString->m_Caption.erase(selBegin,selEnd-selBegin);
		if( m_EditMethod & UI_EDITMETHOD_CODER )
		{
			Encrypt();
		}
		if((m_EditMethod&UI_EDITMETHOD_VALUE)!=0) 
		{
			CaptionToValue();
		}

		m_CursorPos=selBegin;
		m_SelBegin =selBegin;

		//SetUpdateDirty(true);
		OnChangeText();

		return 1;
	}
	else return 0;
}


//bLeft:是否将光标靠左
//bMove:是否移动光标
int EditBox::CancelSel(bool bLeft,bool bMove)
{
	if(m_SelBegin!=m_CursorPos)
	{
		int selBegin=(m_SelBegin<m_CursorPos)?m_SelBegin:m_CursorPos;
		int selEnd=(m_SelBegin>m_CursorPos)?m_SelBegin:m_CursorPos;

		if(bMove)	
			m_CursorPos=(bLeft)?selBegin:selEnd;

		m_SelBegin=m_CursorPos;
		return 1;
	}
	else return 0;
}

//密码框
void EditBox::Encrypt()
{
	int len = (int)m_pFontString->m_Caption.length();

	m_EncryptCaption = m_pFontString->m_Caption;
	m_iSizeEncrypt = len;

	if (len == 0)
	{
		return;
	}

	memset((void*)m_pFontString->m_Caption.c_str(),'*',len);
}

void EditBox::Decrypt()
{
	int len = (int)m_pFontString->m_Caption.length();
	if (len == 0)
	{
		return;
	}

	int length = (int )m_EncryptCaption.length();
	m_pFontString->m_Caption = m_EncryptCaption;
	assert(len==( int )m_pFontString->m_Caption.length());
}

int EditBox::GetSelctTexLen()
{
	return abs( m_SelBegin - m_CursorPos );
}

bool EditBox::IsAnyTextSelect()
{
	return m_SelBegin != m_CursorPos;
}

void EditBox::ReplaceSelectText( const char* pszReplaceText )
{
	if ( m_SelBegin == m_CursorPos )
	{
		return;
	}
	
	string preNotSelectText		= m_pFontString->m_Caption.substr( 0, min( m_SelBegin, m_CursorPos ) );
	string posteNotSelectText	= m_pFontString->m_Caption.substr( max( m_SelBegin, m_CursorPos ) );
	string replacedText			= preNotSelectText + pszReplaceText + posteNotSelectText;
	
	m_pFontString->SetText( replacedText.c_str() );
	if ( m_SelBegin < m_CursorPos )
	{
		m_CursorPos = m_SelBegin + strlen( pszReplaceText );
	}
	
	m_SelBegin=m_CursorPos;
}

void EditBox::Clear()
{
	m_pFontString->m_Caption.erase();
	m_EncryptCaption.erase();
	m_iSizeEncrypt=0;

	m_CursorPos = 0;
	m_DispBegin = 0;

	m_SelBegin=0;
	m_Value=0;	
	OnChangeText();
}


void EditBox::CaptionToValue()
{
	double value=0.0;

	value=atof(m_pFontString->m_Caption.c_str());

	//检测值是否改变
	if(m_Value!=value)
	{
		m_Value=value;	

		CheckValueRange();
	}
}

bool EditBox::CheckValueRange()
{
	bool bFixed=false;

	if(m_MaxValue<m_MinValue) 
	{
		m_MaxValue=m_MinValue;
		bFixed=true;
	}

	if(m_Value<m_MinValue) 
	{
		m_Value=m_MinValue;
		bFixed=true;
	}

	if(m_Value>m_MaxValue) 
	{
		m_Value=m_MaxValue;
		bFixed=true;
	}

	return bFixed;
}

void EditBox::SetText(const char *ptext)
{
	if (ptext)
	{
		Clear();
		std::string strText(ptext);
		QueryInputChar( strText );
		m_pFontString->SetText( strText.c_str() );
		m_SelBegin=m_CursorPos;
		OnChangeText();
		if( m_EditMethod & UI_EDITMETHOD_CODER )
		{
			Encrypt();
		}
	}
}

void EditBox::AddText( const char *pszText )
{
	for(size_t i = 0, size = strlen( pszText );i < size; ++i )
	{ 
		char buffer[2];
		buffer[0] = pszText[i];
		buffer[1] = 0;

		//非打印字符
		if(!IsPrint(buffer[0]))
			continue;

		if(!QueryInputFlag(buffer[0]))
		{
			continue;
		}

		m_pFontString->m_Caption.insert( m_CursorPos, buffer );
		m_CursorPos++;
		m_SelBegin=m_CursorPos;
		OnChangeText();
	}
}

const char *EditBox::GetText()
{
	return m_pFontString->GetText();
}

const char *EditBox::GetPassWord()
{
	return m_EncryptCaption.c_str();
}

void EditBox::SetTextColor(int r, int g, int b)
{
	m_pFontString->SetTextColor(r, g, b);
}

void EditBox::AddStringToHistory(const char* text)
{
	//{{{密码控件改进
	if( m_EditMethod & UI_EDITMETHOD_CODER )
	{
		return;
	}
	//}}}

	if(!text || !strcmp(text,"")) return;
	std::string strText(text);

	std::vector<std::string>::iterator iter = find( m_History.begin(), m_History.end(), strText );
	if ( iter != m_History.end() )
	{
		m_History.erase( iter );
	}
	else
	{
		if( m_NumHistory < UI_EDIT_HISTORY ) m_NumHistory++;
	}

	m_History.insert( m_History.begin(), strText );

	if ( m_History.size() > UI_EDIT_HISTORY )
	{
		m_History.resize( UI_EDIT_HISTORY );
	}	
	m_CurHistory = -1;
}

void EditBox::clearHistory()
{
	m_History.clear();
}

void EditBox::SelectAllText()
{
	SetSel( 0, (int)m_pFontString->m_Caption.size() );
}

void EditBox::enableIME( bool b )
{
	static HIMC m_hImc = (HIMC) g_pFrameMgr->getImeContext() ;
	if( !b )
		ImmAssociateContext( (HWND)g_pFrameMgr->getWndHandle(), NULL ); 
	else
		ImmAssociateContext( (HWND)g_pFrameMgr->getWndHandle(), m_hImc ); 
}

void EditBox::enableEdit( bool b )
{
	if( b )
	{
		m_Style |= FRMSTYLE_EDITABLE;
	}else{
		m_Style &= ~FRMSTYLE_EDITABLE;
	}
}

int EditBox::GetTextExtentWidth( const char* pszContent )
{
	float width, height;										
	g_pDisplay->GetTextExtent( g_pFrameMgr->m_HMAYFONT[m_pFontString->m_iFontIndex].h, pszContent, width, height );

	return FloatToInt(width);
}

void EditBox::setMaxChar( int nMax )
{
	m_MaxChar = nMax;
}

int EditBox::getCursorPos()
{
	return m_CursorPos;
}

void EditBox::setCursorPos( int nPos )
{
	if ( nPos < 0 )
	{
		nPos = 0;
	}
	if( nPos > (int)m_pFontString->m_Caption.size() )
	{
		nPos = (int)m_pFontString->m_Caption.size();
	}
	m_CursorPos = nPos;
}

int EditBox::getSelBegin()
{
	return m_SelBegin;
}

void EditBox::setSelBegin( int nPos )
{
	if ( nPos < 0 )
	{
		nPos = 0;
	}
	m_SelBegin = nPos;
}
/*
//-----------------------------------------------------------------------------
// Editbox lua function
//-----------------------------------------------------------------------------
DECLARE_CLASS_TO_SCRIPT_EX(EditBox, Frame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

//EditBox
DECLARE_MEM_FUNC_TO_SCRIPT(EditBox, Clear);
DECLARE_MEM_FUNC_TO_SCRIPT(EditBox, SetText, SVT_EMPTY, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(EditBox, GetText, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(EditBox, GetPassWord, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(EditBox, SetTextColor, SVT_EMPTY, SVT_INT, SVT_INT, SVT_INT);
*/