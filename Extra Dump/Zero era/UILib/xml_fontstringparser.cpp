#include "xml_fontstringparser.h"
#include "ui_framemgr.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML Texture
//-----------------------------------------------------------------------------
XMLFontStringParser::XMLFontStringParser()
{

}

XMLFontStringParser::~XMLFontStringParser()
{
}

bool XMLFontStringParser::LoadUIObjectParam(UIObject *pdestobj, XMLNode pNode, bool bTemplate)
{
	XMLLayoutFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestFontString = static_cast<FontString *>(pdestobj);

	const char* pszFontName			= pNode.attribToString("fonttype");
	const UIFont* pOneUIFontConfig	= g_pFrameMgr->getUIFontByName( pszFontName );
	if ( pOneUIFontConfig != NULL )
	{
		m_pDestFontString->m_iFontIndex = g_pFrameMgr->getUIFontIndexByName( pszFontName );
		m_pDestFontString->m_FontName	= pszFontName;
	}
	else
	{
		assert(0 && "wrong Font Type");
	}

	m_pDestFontString->m_FontStyle = 0;
	if (pNode.attribToString("fontStyle"))
	{
		if (stricmp(pNode.attribToString("fontStyle"), "shadow") == 0)
		{
			m_pDestFontString->m_FontStyle = FONTSYTLE_SHADOW;
		}

		if (stricmp(pNode.attribToString("fontStyle"), "border") == 0)
		{
			m_pDestFontString->m_FontStyle = FONTSYTLE_BORDER;
		}
	}

	if (pNode.attribToString("autowrap"))
	{
		m_pDestFontString->m_bAutoWrap = pNode.attribToBool("autowrap");
	}

	if (pNode.attribToString("lineInterval"))
	{
		m_pDestFontString->m_fLineInterval = pNode.attribToFloat("lineInterval");
	}

	if (pNode.attribToString("text"))
	{
		if (pNode.attribToString("name"))
			m_pDestFontString->SetName(pNode.attribToString("name"));

		std::string str = pNode.attribToString("text");

		char *p = Utf8ToGb(str.c_str());

		m_pDestFontString->SetText(p);

		delete(p);
	}

	if (pNode.attribToString("justifyH"))
	{
		if (stricmp(pNode.attribToString("justifyH"), "LEFT") == 0)
		{
			m_pDestFontString->m_JustifyH = JUSTIFYH_LEFT;
		}
		if (stricmp(pNode.attribToString("justifyH"), "CENTER") == 0)
		{
			m_pDestFontString->m_JustifyH = JUSTIFYH_CENTER;
		}
		if (stricmp(pNode.attribToString("justifyH"), "RIGHT") == 0)
		{
			m_pDestFontString->m_JustifyH = JUSTIFYH_RIGHT;
		}
	}

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "Color") == 0)
		{
			if (pChildNode.attribToString("r"))
			{
				m_pDestFontString->m_TextColor.r = pChildNode.attribToInt("r");
			}

			if (pChildNode.attribToString("g"))
			{
				m_pDestFontString->m_TextColor.g = pChildNode.attribToInt("g");
			}

			if (pChildNode.attribToString("b"))
			{
				m_pDestFontString->m_TextColor.b = pChildNode.attribToInt("b");
			}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}
	return true;
}