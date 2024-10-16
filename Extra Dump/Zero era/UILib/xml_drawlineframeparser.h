#pragma once
#include "xml_layoutframeparser.h"
#include "ui_drawlineframe.h"

class XMLDrawLineFrameParser : public XMLLayoutFrameParser
{
public:
	XMLDrawLineFrameParser();
	~XMLDrawLineFrameParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:

	DrawLineFrame *m_pDestLineFrame;
};

