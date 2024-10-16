#pragma once
#include "ui_richtext.h"
#include "xml_frameparser.h"

//-----------------------------------------------------------------------------
// UI XML RichText Parser
//-----------------------------------------------------------------------------
class XMLRichTextParser : public XMLFrameParser
{
public:
	XMLRichTextParser();
	~XMLRichTextParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:
	RichText *m_pDestRichText;
};