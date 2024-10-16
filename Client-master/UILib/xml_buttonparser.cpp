#include "xml_buttonparser.h"
#include "xml_textureparser.h"
#include "ui_framemgr.h"
#include "xml_fontstringparser.h"

#include "OgreStringUtil.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML Button Parser
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;
XMLButtonParser::XMLButtonParser()
{
}

XMLButtonParser::~XMLButtonParser()
{
}

bool XMLButtonParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	XMLFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestButton = static_cast<Button *>(pdestobj);
	std::string nodename = pdestobj->m_Name;


	if (pNode.attribToString("text"))
	{
		wchar_t buff[1024];
		StringUtil::UTF8ToUnicode(buff , 1024 , pNode.attribToString("text"));
		char buff2[1024];
		StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
		m_pDestButton->m_pNormalFontString->SetText(buff2);
		//m_pDestButton->AddFontString(DL_OVERLAY, m_pDestButton->m_pNormalFontString);
	}

	if (pNode.attribToString("checked"))
	{
		bool bChecked = pNode.attribToBool("checked");
		if( bChecked )
		{
			m_pDestButton->m_State |= FRMSTATE_CHECKED; 
		}
	}

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		std::string name;
		if(pChildNode.attribToString("name"))
			name = pChildNode.attribToString("name");

		XMLUIObjectParser *parser = NULL;
		if (stricmp(pChildNode.getName(), "NormalTexture") == 0)
		{
			parser = new XMLTextureParser;
			m_pDestButton->AddTexture(DL_ARTWORK, m_pDestButton->m_pNormalRegion);
			if (!parser->LoadUIObjectParam(m_pDestButton->m_pNormalRegion, pChildNode, bTemplate))
			{
				assert(0 && "NormalTexture Parse Error!");
				return false;
			}
			if( pChildNode.attribToString("file") )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("file"));
				char buff2[1024];
				StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
				m_pDestButton->SetNormalTexture(buff2, "blend");
			}
			if( pChildNode.attribToString("inherits") )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("inherits"));
				char buff2[1024];
				StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
				m_pDestButton->SetNormalTextureTemplate(buff2);
			}
			if( pChildNode.attribToString("r") )
			{
				m_pDestButton->SetNormalTextColor( pChildNode.attribToInt("r"),pChildNode.attribToInt("g"),pChildNode.attribToInt("b") );
			}
			//m_pDestButton->SetNormalTexture(pChildNode.attribToString("file"), "blend");

			delete(parser);
		}
		if (stricmp(pChildNode.getName(), "PushedTexture") == 0)
		{
			parser = new XMLTextureParser;
			m_pDestButton->AddTexture(DL_ARTWORK, m_pDestButton->m_pOverlayRegion);
			if (!parser->LoadUIObjectParam(m_pDestButton->m_pOverlayRegion, pChildNode, bTemplate))
			{
				return false;
			}
			if( pChildNode.attribToString("file") )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("file"));
				char buff2[1024];
				StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
				m_pDestButton->SetPushedTexture(buff2, "blend");
			}
			if( pChildNode.attribToString("inherits") )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("inherits"));
				char buff2[1024];
				StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
				m_pDestButton->SetPushedTextureTemplate(buff2);
			}
			if( pChildNode.attribToString("x") )
			{
				m_pDestButton->ChangePushedTexture(	pChildNode.attribToInt("x"),
					pChildNode.attribToInt("y"),
					pChildNode.attribToInt("w"),
					pChildNode.attribToInt("h") );
			}
			if( pChildNode.attribToString("r") )
			{
				m_pDestButton->SetPushedTextColor( pChildNode.attribToInt("r"),pChildNode.attribToInt("g"),pChildNode.attribToInt("b") );
			}
			//m_pDestButton->SetPushedTexture(pChildNode.attribToString("file"), "blend");
			delete(parser);
		}
		if (stricmp(pChildNode.getName(), "DisabledTexture") == 0)
		{
			if( pChildNode.attribToString("file") )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("file"));
				char buff2[1024];
				StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
				m_pDestButton->SetDisableTexture(buff2, "blend");
			}
			if( pChildNode.attribToString("inherits") )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("inherits"));
				char buff2[1024];
				StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
				m_pDestButton->SetDisableTextureTemplate(buff2);
			}
			if( pChildNode.attribToString("x") )
			{
				m_pDestButton->ChangeDisableTexture( pChildNode.attribToInt("x"),
					pChildNode.attribToInt("y"),
					pChildNode.attribToInt("w"),
					pChildNode.attribToInt("h") );
			}
			if( pChildNode.attribToString("r") )
			{
				m_pDestButton->SetDisableTextColor( pChildNode.attribToInt("r"),pChildNode.attribToInt("g"),pChildNode.attribToInt("b") );
			}

			//m_pDestButton->SetDisableTexture(pChildNode.attribToString("file"), "blend");
		}
		if (stricmp(pChildNode.getName(), "HighlightTexture") == 0)
		{
			if( pChildNode.attribToString("file") )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("file"));
				char buff2[1024];
				StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
				m_pDestButton->SetHightlightTexture(buff2, "blend");
			}
			if( pChildNode.attribToString("inherits") )
			{
				wchar_t buff[1024];
				StringUtil::UTF8ToUnicode(buff , 1024 , pChildNode.attribToString("inherits"));
				char buff2[1024];
				StringUtil::UnicodeToAnsi(buff2 , 1024 , buff);
				m_pDestButton->SetHighlightTextureTemplate(buff2);
			}
			if( pChildNode.attribToString("x") )
			{
				m_pDestButton->ChangeHighlightTexture( pChildNode.attribToInt("x"),
					pChildNode.attribToInt("y"),
					pChildNode.attribToInt("w"),
					pChildNode.attribToInt("h") );
			}
			if( pChildNode.attribToString("r") )
			{
				m_pDestButton->SetHighlightTextColor( pChildNode.attribToInt("r"),pChildNode.attribToInt("g"),pChildNode.attribToInt("b") );
			}
            //m_pDestButton->SetHightlightTexture(pChildNode.attribToString("file"), "blend");
		}

		if (stricmp(pChildNode.getName(), "FontString") == 0)
		{
			XMLUIObjectParser *parser = NULL;

			parser = new XMLFontStringParser;
			if (!parser->LoadUIObjectParam(m_pDestButton->m_pNormalFontString, pChildNode, bTemplate))
			{
				return false;
			}
			m_pDestButton->AddFontString(DL_OVERLAY, static_cast<FontString *>(m_pDestButton->m_pNormalFontString));

			delete(parser);

			//std::string s = pChildNode.attribToString("font");
			//char *p;
			//p = Utf8ToGb(s.c_str());

			//m_pDestButton->AddFontString(DL_ARTWORK, m_pDestButton->m_pNormalFontString);

			//m_pDestButton->SetNormalFont(p);
			//if (pChildNode.attribToString("text"))
			//{
			//	m_pDestButton->SetNormalText(pChildNode.attribToString("text"));
			//}

			//delete p;
		}
		
		if( stricmp( pChildNode.getName(), "OnClickSound" ) == 0 )
		{
			if (pChildNode.attribToString("file"))
			{
				m_pDestButton->m_strClickSoundPath = pChildNode.attribToString("file");
			}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}
	if( m_pDestButton->m_pNormalRegion )
	{
		m_pDestButton->m_pNormalRegion->m_Size = m_pDestButton->m_Size;
	}
	if( m_pDestButton->m_pOverlayRegion )
	{
		m_pDestButton->m_pOverlayRegion->m_Size = m_pDestButton->m_Size;
	}	
	return true;
}