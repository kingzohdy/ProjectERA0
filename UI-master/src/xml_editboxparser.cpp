#include "xml_editboxparser.h"
//#include "xml_textureparser.h"
#include "ui_fontstring.h"
#include "ui_framemgr.h"
#include "xml_fontstringparser.h"

using namespace Ogre;

//-----------------------------------------------------------------------------
// UI XML EditBox Parser
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;
XMLEditBoxParser::XMLEditBoxParser()
{
}

XMLEditBoxParser::~XMLEditBoxParser()
{
}

bool XMLEditBoxParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	XMLFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestEditBox = static_cast<EditBox *>(pdestobj);

	if (pNode.attribToString("letters"))
	{
		m_pDestEditBox->m_MaxChar = pNode.attribToInt("letters");
	}

	if (pNode.attribToString("historyLines"))
	{
		m_pDestEditBox->m_NumHistory = pNode.attribToInt("historyLines");
	}

	if (pNode.attribToString("multiLine"))
	{
		m_pDestEditBox->m_bMultiLine = pNode.attribToBool("multiLine");
	}

	if (pNode.attribToBool("password"))
	{
		m_pDestEditBox->m_EditMethod |= UI_EDITMETHOD_CODER;
	}
	
	if(pNode.attribToString("editMethod"))
	{
		m_pDestEditBox->m_EditMethod = pNode.attribToInt("editMethod");
	}

	std::string nodename = pdestobj->m_Name;
	if (m_pDestEditBox->m_pFontString)
	{
		m_pDestEditBox->m_pFontString->SetName((nodename + "FontString").c_str());
		m_pDestEditBox->m_pFontString->m_Anchor[0].m_RelFrame = "$parent";
	}
	else
	{
		m_pDestEditBox->m_pFontString = static_cast<FontString *>(g_pFrameMgr->CreateObject("FontString", (nodename + "FontString").c_str()));
		m_pDestEditBox->m_pFontString->m_Anchor[0].m_RelFrame = nodename;
		m_pDestEditBox->m_pFontString->SetName("");
		m_pDestEditBox->m_pFontString->DrawShow(true);
		m_pDestEditBox->m_pFontString->m_TextColor = ColorQuad(200, 200, 200, 255);
	}

	m_pDestEditBox->m_pFontString->m_Size = static_cast<LayoutFrame *>(pdestobj)->m_Size;
	m_pDestEditBox->m_pFontString->m_Anchor[0].m_MyPoint = m_pDestEditBox->m_pFontString->m_Anchor[0].m_RelPoint = FP_TOPLEFT;
	m_pDestEditBox->m_pFontString->m_Anchor[0].m_Offset.SetAbsDim(0,0);

	m_pDestEditBox->m_pFontString->m_FrameStrata = m_pDestEditBox->m_FrameStrata;

	XMLNode pChildNode = pNode.iterateChild();
	while(!pChildNode.isNull())
	{
		std::string name;
		if(pChildNode.attribToString("name"))
			name = pChildNode.attribToString("name");

		if (stricmp(pChildNode.getName(), "FontString") == 0)
		{
			XMLUIObjectParser *parser = NULL;
			parser = new XMLFontStringParser;
			if (!parser->LoadUIObjectParam(m_pDestEditBox->m_pFontString, pChildNode, bTemplate))
			{
				return false;
			}

			delete(parser);
			//m_pDestEditBox->AddFontString(DL_ARTWORK, static_cast<FontString *>(m_pDestEditBox->m_pFontString));

			//std::string s = pChildNode.attribToString("font");
			//char *p;
			//p = Utf8ToGb(s.c_str());
			//m_pDestEditBox->SetFont(p);
			//if (pChildNode.attribToString("text"))
			//{
			//	m_pDestEditBox->m_pFontString->SetText(pChildNode.attribToString("text"));
			//}
			//
			//delete p;
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}

	return true;
}