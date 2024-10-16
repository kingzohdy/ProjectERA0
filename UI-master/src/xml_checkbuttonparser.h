#pragma once
#include "ui_checkbutton.h"
#include "xml_buttonparser.h"
#include "xml_frameparser.h"

//-----------------------------------------------------------------------------
// UI XML CheckButton Parser
//-----------------------------------------------------------------------------
class XMLCheckButtonParser : public XMLButtonParser
{
public:
	XMLCheckButtonParser();
	~XMLCheckButtonParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:
	CheckButton *m_pDestCheckButton;
};