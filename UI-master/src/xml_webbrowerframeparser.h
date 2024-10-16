#pragma once
#include "ui_webbrowerframe.h"
#include "xml_frameparser.h"

//-----------------------------------------------------------------------------
// UI XML Button Parser
//-----------------------------------------------------------------------------
class XMLWebBrowerFrameParser : public XMLFrameParser
{
public:
	XMLWebBrowerFrameParser();
	~XMLWebBrowerFrameParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
};