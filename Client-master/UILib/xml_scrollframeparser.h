#pragma once
#include "ui_scrollframe.h"
#include "xml_frameparser.h"

//-----------------------------------------------------------------------------
// XML ScrollFrame Parser
//-----------------------------------------------------------------------------
class XMLScrollFrameParser : public XMLFrameParser
{
public:
	XMLScrollFrameParser();
	~XMLScrollFrameParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:
	ScrollFrame *m_pDestScrollFrame;

};