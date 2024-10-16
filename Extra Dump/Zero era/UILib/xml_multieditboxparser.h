#pragma once
#include "ui_multieditbox.h"
#include "xml_frameparser.h"

class XMLMultiEditBoxParser : public XMLFrameParser
{
public:
	XMLMultiEditBoxParser();
	~XMLMultiEditBoxParser();

	virtual bool LoadUIObjectParam( UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false );
private:

	MultiEditBox *m_pDestMultiEditBox;
};

