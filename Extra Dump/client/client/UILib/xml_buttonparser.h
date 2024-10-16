#pragma once
#include "ui_button.h"
#include "xml_frameparser.h"

//-----------------------------------------------------------------------------
// UI XML Button Parser
//-----------------------------------------------------------------------------
class XMLButtonParser : public XMLFrameParser
{
public:
	XMLButtonParser();
	~XMLButtonParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:
	Button *m_pDestButton;
};