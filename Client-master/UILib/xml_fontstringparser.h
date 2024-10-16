#pragma once
#include "xml_layoutframeparser.h"
#include "ui_fontstring.h"

class XMLFontStringParser : public XMLLayoutFrameParser
{
public:
	XMLFontStringParser();
	~XMLFontStringParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:

	FontString *m_pDestFontString;
};

