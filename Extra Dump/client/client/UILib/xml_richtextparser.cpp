#include "xml_richtextparser.h"
#include "ui_fontstring.h"
#include "ui_framemgr.h"
#include "xml_fontstringparser.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML RichText Parser
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;
XMLRichTextParser::XMLRichTextParser()
{
}

XMLRichTextParser::~XMLRichTextParser()
{
}

bool XMLRichTextParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	XMLFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestRichText = static_cast<RichText *>(pdestobj);

	if (pNode.attribToString("lineInterval"))
	{
		m_pDestRichText->m_nLineInterval = pNode.attribToInt("lineInterval");
	}
    
    if( pNode.attribToString("autoextend") )
    {
        m_pDestRichText->m_bAutoExtend = pNode.attribToBool("autoextend");
    }
    
	if (pNode.attribToString("maxlines"))
	{
		m_pDestRichText->m_iMaxLines = pNode.attribToInt("maxlines");
	}
	
	if (pNode.attribToString("fontStyle"))
	{
		if (stricmp(pNode.attribToString("fontStyle"), "normal") == 0)
		{
			m_pDestRichText->m_FontStyle = FONTSYTLE_NORMAL;
		}

		if (stricmp(pNode.attribToString("fontStyle"), "shadow") == 0)
		{
			m_pDestRichText->m_FontStyle = FONTSYTLE_SHADOW;
		}

		if (stricmp(pNode.attribToString("fontStyle"), "border") == 0)
		{
			m_pDestRichText->m_FontStyle = FONTSYTLE_BORDER;
		}

		if (stricmp(pNode.attribToString("fontStyle"), "embolden") == 0)
		{
			m_pDestRichText->m_FontStyle = FONTSYTLE_EMBOLDEN;
		}
	}

	const char* pszFontName = pNode.attribToString("fonttype");
	if ( pszFontName != NULL )
	{
		const UIFont* pOneUIFontConfig = g_pFrameMgr->getUIFontByName( pszFontName );
		if ( pOneUIFontConfig != NULL )
		{
			m_pDestRichText->m_iFontIndex	= g_pFrameMgr->getUIFontIndexByName( pszFontName );

			int height = pOneUIFontConfig->height;
			m_pDestRichText->m_hLinkFont	= g_pDisplay->CreateTrueTypeFont( height, height,
												pOneUIFontConfig->type.c_str(), CCODING_GBK, FONTTYPE_UNDERLINE );
		}
		else
		{
			assert( false && "wrong Font Type");
		}
	}

	//XMLNode pChildNode = pNode.iterateChild();
	//while( !pChildNode.isNull() )
	//{
	//	//if (stricmp(pChildNode.getName(), "FontString") == 0)
	//	//{
	//	//	XMLUIObjectParser *parser = NULL;

	//	//	parser = new XMLFontStringParser;
	//	//	if (!parser->LoadUIObjectParam(m_pDestButton->m_pNormalFontString, pChildNode, bTemplate))
	//	//		return false;
	//	//	m_pDestRichText->AddFontString(DL_ARTWORK, static_cast<FontString *>(m_pDestButton->m_pNormalFontString));


	//	//}

	//	//if (stricmp(pChildNode.getName(), "Color") == 0)
	//	//{
	//	//	if (pChildNode.attribToString("r"))
	//	//	{
	//	//		m_pDestFontString->m_TextColor.r = pChildNode.attribToInt("r");
	//	//	}

	//	//	if (pChildNode.attribToString("g"))
	//	//	{
	//	//		m_pDestFontString->m_TextColor.g = pChildNode.attribToInt("g");
	//	//	}

	//	//	if (pChildNode.attribToString("b"))
	//	//	{
	//	//		m_pDestFontString->m_TextColor.b = pChildNode.attribToInt("b");
	//	//	}
	//	//}
	//	pChildNode = pNode.iterateChild(pChildNode);
	//}
	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		if (stricmp(pChildNode.getName(), "ShadowColor") == 0)
		{
			if (pChildNode.attribToString("r"))
			{
				m_pDestRichText->m_shadowColor.r = pChildNode.attribToInt("r");
			}

			if (pChildNode.attribToString("g"))
			{
				m_pDestRichText->m_shadowColor.g = pChildNode.attribToInt("g");
			}

			if (pChildNode.attribToString("b"))
			{
				m_pDestRichText->m_shadowColor.b = pChildNode.attribToInt("b");
			}
		}

		if (stricmp(pChildNode.getName(), "LinkColor") == 0)
		{
			int nColorR	= 0;
			int nColorG = 0;
			int nColorB = 0;
			if (pChildNode.attribToString("r"))
			{
				nColorR = pChildNode.attribToInt("r");
			}

			if (pChildNode.attribToString("g"))
			{
				nColorG = pChildNode.attribToInt("g");
			}

			if (pChildNode.attribToString("b"))
			{
				nColorB = pChildNode.attribToInt("b");
			}

			m_pDestRichText->SetLinkTextColor( nColorR, nColorG, nColorB );
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}

	return true;
}