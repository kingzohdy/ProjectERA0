#pragma once
#include "ui_editbox.h"
#include "xml_frameparser.h"

//-----------------------------------------------------------------------------
// UI XML EditBox Parser
//-----------------------------------------------------------------------------
class XMLEditBoxParser : public XMLFrameParser
{
public:
	XMLEditBoxParser();
	~XMLEditBoxParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:
	EditBox *m_pDestEditBox;
};