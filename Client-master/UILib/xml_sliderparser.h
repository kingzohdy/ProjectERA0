#pragma once
#include "ui_slider.h"
#include "xml_frameparser.h"

//-----------------------------------------------------------------------------
// XML Slider Parser
//-----------------------------------------------------------------------------
class XMLSliderParser : public XMLFrameParser
{
public:
	XMLSliderParser();
	~XMLSliderParser();

	virtual bool LoadUIObjectParam(UIObject *pdestobj, Ogre::XMLNode pNode, bool bTemplate = false);
private:
	Slider *m_pDestSlider;
};