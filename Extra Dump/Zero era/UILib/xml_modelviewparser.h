#pragma once
#include "ui_modelview.h"
#include "xml_frameparser.h"

class XMLModelViewParser : public XMLFrameParser
{
public:
	XMLModelViewParser();
	~XMLModelViewParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:
	ModelView *m_pDestView;
};