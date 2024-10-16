#include "xml_scrollframeparser.h"
#include "ui_framemgr.h"
#include "xml_fontstringparser.h"

using namespace Ogre;
//-----------------------------------------------------------------------------
// XML ScrollFrame Parser
//-----------------------------------------------------------------------------
extern FrameManager *g_pFrameMgr;
XMLScrollFrameParser::XMLScrollFrameParser()
{
}

XMLScrollFrameParser::~XMLScrollFrameParser()
{
}

bool XMLScrollFrameParser::LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate)
{
	XMLFrameParser::LoadUIObjectParam(pdestobj, pNode, bTemplate);

	m_pDestScrollFrame = static_cast<ScrollFrame *>(pdestobj);

	std::string nodename = pdestobj->m_Name;

	//FontString parser
	if (m_pDestScrollFrame->m_pFontString)
	{
		m_pDestScrollFrame->m_pFontString->SetName((nodename + "FontString").c_str());
		m_pDestScrollFrame->m_pFontString->m_Anchor[0].m_RelFrame = "$parent";
	}
	else
	{
		m_pDestScrollFrame->m_pFontString = static_cast<FontString *>(g_pFrameMgr->CreateObject("FontString", 
																		(nodename + "FontString").c_str()));
		m_pDestScrollFrame->m_pFontString->m_Anchor[0].m_RelFrame = nodename;
		m_pDestScrollFrame->m_pFontString->SetName("");
		m_pDestScrollFrame->m_pFontString->m_TextColor = ColorQuad(200, 200, 200, 255);
	}

	m_pDestScrollFrame->m_pFontString->m_Size = static_cast<LayoutFrame *>(pdestobj)->m_Size;
	m_pDestScrollFrame->m_pFontString->m_Anchor[0].m_MyPoint = 
		m_pDestScrollFrame->m_pFontString->m_Anchor[0].m_RelPoint = FP_TOPLEFT;
	m_pDestScrollFrame->m_pFontString->m_Anchor[0].m_Offset.SetAbsDim(0,0);

	m_pDestScrollFrame->m_pFontString->m_FrameStrata = m_pDestScrollFrame->m_FrameStrata;

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
			if (!parser->LoadUIObjectParam(m_pDestScrollFrame->m_pFontString, pChildNode, bTemplate))
			{
				return false;
			}
			m_pDestScrollFrame->AddFontString(DL_ARTWORK, static_cast<FontString *>(m_pDestScrollFrame->m_pFontString));

			delete(parser);
		}

		if (stricmp(pChildNode.getName(), "ScrollChild") == 0)
		{
			if (!FrameParserRecursive(m_pDestScrollFrame, pChildNode))
			{
				return false;
			}
		}

		pChildNode = pNode.iterateChild(pChildNode);
	}

	return true;
}