#include "ui_fontstring.h"
#include "ui_framemgr.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// Font Instance
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;

FontInstance::FontInstance()
{
	m_TextColor = ColorQuad(200,200,200,255);
	//m_hFont = 0;
	m_FontStyle = 0;
	m_iFontIndex = 0;	//默认用font.xml里面定义的3号字体
	m_FontName = "";
	m_JustifyH = JUSTIFYH_LEFT;
	m_fLineInterval = 0.0f;
}

FontInstance::~FontInstance()
{
}

void FontInstance::SetFont(Ogre::HFONT iFont)
{
	//m_hFont = iFont;
}

void FontInstance::SetJustifyV(JUSTIFYV_T justifyV)
{
	m_JustifyV = justifyV;
}

void FontInstance::SetJustifyH(JUSTIFYH_T justifyh)
{
	m_JustifyH = justifyh;
}

void FontInstance::SetShadowColor(ColorQuad color)
{
	m_ShadowColor = color;
}

void FontInstance::SetSpacing(float space)
{
	m_fSpace = space;
}

//-----------------------------------------------------------------------------
// FontString
//-----------------------------------------------------------------------------
IMPLEMENT_UIOBJECT_CLONE(FontString)
FontString::FontString()
{
	m_OffsetX = 0;
	m_OffsetY = 0;
	m_bAutoWrap = false;
	m_fScale = 1.0f;

	m_bBuff = false;
	m_fBuffStart = m_fBuffTotal = 0.0f;

	//滚动数字
	m_bScrollNum = false;
	m_bScrollNumUint = false;
	m_fScrollNumTotal = 0.0f;
	m_fScrollNumAcc = 0.0f;
}

FontString::~FontString()
{
}

void FontString::CopyMembers(FontString *pdest)
{
	if (pdest)
	{
		LayoutFrame::CopyMembers(pdest);

		//pdest->m_hFont = m_hFont;
		pdest->m_iFontIndex = m_iFontIndex;
		pdest->m_FontName = m_FontName;
		pdest->m_TextColor = m_TextColor;
		pdest->m_ShadowColor = m_ShadowColor;
		pdest->m_Caption = m_Caption;
		pdest->m_fSpace = m_fSpace;
		pdest->m_outline = m_outline;
		pdest->m_bMonochrome = m_bMonochrome;
		pdest->m_bNonspacewra = m_bNonspacewra;
		pdest->m_JustifyV = m_JustifyV;
		pdest->m_JustifyH = m_JustifyH;

		pdest->m_MaxLines = m_MaxLines;

		pdest->m_FontStyle = m_FontStyle;

		pdest->m_fLineInterval = m_fLineInterval;

		pdest->m_OffsetX = m_OffsetX;
		pdest->m_OffsetY = m_OffsetY;
		pdest->m_bAutoWrap = m_bAutoWrap;
		pdest->m_fScale = m_fScale;

		pdest->m_bBuff = m_bBuff;
		pdest->m_fBuffStart = m_fBuffStart;
		pdest->m_fBuffTotal = m_fBuffTotal;
	}
}

void FontString::SetScrollNumber(unsigned int begin, unsigned int end, float time)
{
	m_iBegin = begin;
	m_iEnd = end;

	m_fScrollNumTotal = time;

	m_bScrollNum = true;
}

void FontString::SetScrollNumberWithUint(unsigned int begin, unsigned int end, float time)
{
	m_bScrollNumUint = true;
	SetScrollNumber(begin, end, time);
}

int FontString::GetTextExtentWidth( const char* pszContent )
{
	float width, height;
	const UIFont& oneUIFontConfig = g_pFrameMgr->getUIFontByIndex( m_iFontIndex );
	g_pDisplay->GetTextExtent( oneUIFontConfig.h, pszContent, width, height );

	return FloatToInt(width);
}

void FontString::SetText(const char *str)
{
	if (str)
	{
		m_Caption = str;
	}
}

const char * FontString::GetText()
{
	return m_Caption.c_str();
}

void FontString::SetTextColor(int r, int g, int b)
{
	m_TextColor = ColorQuad(r, g, b, 255);
}

void FontString::SetBufferTimer(float start, float total)
{
	m_bBuff = true;

	assert(start >= 0.0f && total >= 0.0f);

	m_fBuffStart = start;
	m_fBuffTotal = total;
}

void FontString::UpdateSelf(float deltatime)
{
	if(m_bShow)
	{
		AdjustFrameByViewPoint();

		//if (g_pFrameMgr->GetScreenScale() != 1.0f)
		//{
		//	g_pDisplay->ReleaseTrueTypeFont(m_hFont);

		//	UINT fontHeight = (int)(m_iDrawFont * g_pFrameMgr->GetScreenScale() + 0.5f);

		//	m_hFont = g_pDisplay->CreateTrueTypeFont(fontHeight, fontHeight, m_FontType.c_str());
		//}

		if (m_bBuff)
		{
			m_fBuffStart += deltatime;
		}

		if (m_bScrollNum)
		{
			m_fScrollNumAcc += deltatime;

			char buff[1024];
			memset( buff, 0, sizeof( buff ) );
			if (m_fScrollNumAcc <= m_fScrollNumTotal)
			{
				unsigned int num = m_iBegin + FloatToInt((m_fScrollNumAcc/m_fScrollNumTotal) * (m_iEnd - m_iBegin));
				
				if (m_bScrollNumUint)
				{
					//亿: # 万：$ 点:%
					unsigned int million_100 = int(num/100000000); //亿
					unsigned int thousand_10 = int((num%100000000)/10000);//万
					unsigned int point = int(num%10000);

					if (million_100 > 0)
					{
						_snprintf_s(buff,sizeof(buff), "%d#%04d$%04d%%", million_100, thousand_10, point);
					}
					else if (thousand_10 > 0)
					{
						_snprintf_s(buff,sizeof(buff), "%d$%04d%%", thousand_10, point);
					}
					else
					{
						_snprintf_s(buff,sizeof(buff), "%d%%", point);
					}
				}
				else
				{
					_snprintf_s(buff,sizeof(buff), "%d", num);
				}
				SetText(buff);
			}
			else
			{
				if (m_bScrollNumUint)
				{
					//亿: # 万：$ 点:%
					unsigned int million_100 = int(m_iEnd/100000000); //亿
					unsigned int thousand_10 = int((m_iEnd%100000000)/10000);//万
					unsigned int point = int(m_iEnd%10000);

					if (million_100 > 0)
					{
						_snprintf_s(buff,sizeof(buff), "%d#%04d$%04d%%", million_100, thousand_10, point);
					}
					else if (thousand_10 > 0)
					{
						_snprintf_s(buff,sizeof(buff), "%d$%04d%%", thousand_10, point);
					}
					else
					{
						_snprintf_s(buff,sizeof(buff), "%d%%", point);
					}
				}
				else
				{
					_snprintf_s(buff,sizeof(buff), "%d", m_iEnd);
				}
				SetText(buff);
				m_fScrollNumAcc = 0.0f;
				m_fScrollNumTotal = 0.0f;
				m_bScrollNum = false;
			}
		}
	}
}

void FontString::SetBlendAlpha(float alpha)
{
	assert((alpha >= 0.0f) && (alpha <= 1.0f));

	int iAlpha = FloatToInt(alpha * 255);

	m_TextColor.a = iAlpha;

}

void FontString::Draw()
{
	const UIFont& oneUIFontConfig = g_pFrameMgr->getUIFontByIndex( m_iFontIndex );
	if ( oneUIFontConfig.h == Ogre::HFONT(0) )
	{
		return;
	}
	assert( oneUIFontConfig.h != Ogre::HFONT(0) );
	//buff time
	if (m_bBuff)
	{
		if (m_fBuffStart * 1000.0f <= m_fBuffTotal)
		{
			//std::string str = m_fBuffStart;
			char strTime[256];
			int iremainTime = FloatToInt((m_fBuffTotal - m_fBuffStart * 1000.0f) / 1000.0f);
            if(iremainTime / 60 == 0)
			    _snprintf_s(strTime,sizeof(strTime), "%d s", iremainTime);
            else if(iremainTime / 3600 == 0)
                _snprintf_s(strTime,sizeof(strTime), "%d m", iremainTime / 60);
            else
                _snprintf_s(strTime,sizeof(strTime), "%d h", iremainTime / 3600);
			m_Caption = strTime;
		}
		else
		{
			m_Caption = "0";
			//m_bBuff = false;

			//this->Hide();
		}
	}

	if (m_JustifyH != JUSTIFYH_LEFT)
	{
		float fcursorx = 0.0f;
		float fcursory = 0.0f;
		const UIFont& oneUIFontConfig = g_pFrameMgr->getUIFontByIndex( m_iFontIndex );
		g_pDisplay->GetTextExtent(oneUIFontConfig.h, m_Caption.c_str(), fcursorx, fcursory );
		fcursorx = fcursorx * oneUIFontConfig.scale;
		if (m_JustifyH == JUSTIFYH_CENTER)
		{
			m_OffsetX = FloatToInt(((m_AbsRect.m_Right - m_AbsRect.m_Left) - fcursorx)/2);
		}else if (m_JustifyH == JUSTIFYH_RIGHT)
		{
			m_OffsetX = FloatToInt((m_AbsRect.m_Right - m_AbsRect.m_Left) - fcursorx);
		}else
		{
			assert(0 && "Fontstring JustfyH error!");
		}
	}

	RectFloat rc;
	rc.m_Top = (float)m_AbsRect.m_Top;
	rc.m_Left = (float)m_AbsRect.m_Left;
	rc.m_Bottom = (float)m_AbsRect.m_Bottom;
	rc.m_Right = (float)m_AbsRect.m_Right;

	//保存行间距
	float lineinterval = g_pDisplay->GetLineInterval( oneUIFontConfig.h );
	
	g_pDisplay->SetLineInterval( oneUIFontConfig.h, m_fLineInterval );

	g_pDisplay->renderTextRect( oneUIFontConfig.h,m_FontStyle, m_Caption.c_str(), rc, (float)m_OffsetX, (float)m_OffsetY,
								m_bAutoWrap, m_TextColor, m_fScale * oneUIFontConfig.scale*m_fSelfScale );

	g_pDisplay->SetLineInterval( oneUIFontConfig.h, lineinterval );
}

TiXmlElement *FontString::Save(TiXmlElement *pElement)
{
	TiXmlElement *pFontString = LayoutFrame::Save(pElement);

	//fonttype
	if (!m_FontName.empty())
	{
		pFontString->SetAttribute("fonttype", m_FontName.c_str());
	}

	//fontstyle
	if (m_FontStyle != 0)
	{
		if (m_FontStyle == 1)
		{
			pFontString->SetAttribute("fontStyle", "shadow");
		}
		else if (m_FontStyle == 2)
		{
			pFontString->SetAttribute("fontStyle", "border");
		}
	}

	//AutoWrap
	if (m_bAutoWrap)
	{
		pFontString->SetAttribute("autowrap", "true");
	}

	//justify
	if (m_JustifyH != JUSTIFYH_LEFT)
	{
		if (m_JustifyH == JUSTIFYH_CENTER)
		{
			pFontString->SetAttribute("justifyH", "CENTER");
		}
		else
		{
			pFontString->SetAttribute("justifyH", "RIGHT");
		}
	}

	//text
	if (!m_Caption.empty())
	{
		pFontString->SetAttribute("text", m_Caption.c_str());
	}

	//color
	if (!( m_TextColor == ColorQuad(200,200,200,255)))
	{
		TiXmlElement *pTextColor = new TiXmlElement("Color");
		pFontString->LinkEndChild(pTextColor);

		pTextColor->SetAttribute("r", m_TextColor.r);
		pTextColor->SetAttribute("g", m_TextColor.g);
		pTextColor->SetAttribute("b", m_TextColor.b);
	}

	return pFontString;
}

/*
//-----------------------------------------------------------------------------
// fontstring lua function
//-----------------------------------------------------------------------------
//LayoutFrame

DECLARE_CLASS_TO_SCRIPT_EX(FontString, LayoutFrame, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_, _EP_)

//FontString
DECLARE_MEM_FUNC_TO_SCRIPT(FontString, SetText, SVT_EMPTY, SVT_PTR_STRING);
DECLARE_MEM_FUNC_TO_SCRIPT(FontString, GetText, SVT_PTR_STRING);
typedef void (FontString::*TSetTextColor)(int, int, int);
DECLARE_MEM_FUNC_TO_SCRIPT2(FontString, SetTextColor, TSetTextColor, SVT_EMPTY, SVT_INT, SVT_INT, SVT_INT);
DECLARE_MEM_FUNC_TO_SCRIPT(FontString, SetBufferTimer, SVT_EMPTY, SVT_FLOAT, SVT_FLOAT);

*/